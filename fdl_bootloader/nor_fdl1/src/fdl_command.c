#include "sci_types.h"
#include "fdl_command.h"
#include "fdl_main.h"
#include "fdl_stdio.h"
#include "fdl_conf.h"
#include "packet.h"
#include "sio_drv.h"
//#include "sio_api.h"
#include "usb_boot.h"
#ifdef SECURE_BOOT_SUPPORT
#include "hal_rom_api.h"
#endif
#ifdef PLATFORM_UWS6121E
#include "adi_mst.h"
#include "uws6121e_reg_analog.h"
#endif

#ifdef SECURE_BOOT_SUPPORT
volatile uint32 loop_var1=0;
#endif

BOOLEAN  g_is_keep_charge_enable = FALSE;

extern void JumpToTarget (unsigned long addr);

typedef struct _DL_FILE_STATUS
{
    unsigned long start_address;
    unsigned long total_size;
    unsigned long recv_size;
    unsigned long next_address;
} DL_FILE_STATUS, *PDL_FILE_STATUS;

static DL_FILE_STATUS g_file;

#if defined(PLATFORM_UWS6121E)
#define ADI_MST_ARM_RD_ADDR_SHIFT(n)  (((n>>16)&0x7FFF))
#define ADI_FIFO_IS_EMPTY   (hwp_adiMst->arm_cmd_status & ADI_MST_WFIFO_EMPTY)
#define ADI_FIFO_IS_FULL    (hwp_adiMst->arm_cmd_status & ADI_MST_WFIFO_FULL)

PUBLIC uint32 CHIP_PHY_GetANAReg (uint32 addr)
{
    uint32 adi_rd_data;
    uint32 rd_addr = 0;

    // Set read address
    hwp_adiMst->arm_rd_cmd = ADI_MST_ARM_RD_CMD(addr);

    // wait read operation complete, RD_data[31] will be cleared after the read operation complete
    do
    {
        adi_rd_data = hwp_adiMst->arm_rd_data;
    }
    while(adi_rd_data & ADI_MST_ARM_RD_CMD_BUSY);

    // rd_data high part should be the address of the last read operation
    rd_addr = ADI_MST_ARM_RD_ADDR_SHIFT(adi_rd_data);
    if(rd_addr != addr >> 2)
    {
        //SCI_PASSERT(0, ("high 15bits rd_data (0x%x) != (0x%x)addr\n",adi_rd_data,addr));/*assert verified*/
    }

    return((uint16)(adi_rd_data & ADI_MST_ARM_RD_CMD_X1(0xFFFF)));
}

PUBLIC void CHIP_PHY_SetANAReg (uint32 addr, uint32 value)
{
    for(;;)//wait until if these have one empty
    {
        if(ADI_FIFO_IS_FULL == 0)
        {
            break;
        }
    }

    REG32(REG_ADI_MST_BASE + 0x8000 + addr) = (uint32)value;

    // wait write fifo empty after adi analog write
    while(ADI_FIFO_IS_EMPTY == 0)//wait until all data have been sent
    {
    }
}

PUBLIC void CHIP_PHY_AndANAReg (uint32 addr, uint32 value)
{
	uint16 tmp_value = 0;

	tmp_value = CHIP_PHY_GetANAReg(addr);
	tmp_value &= (uint16)value;
    CHIP_PHY_SetANAReg(addr, tmp_value);
}

PUBLIC void CHIP_PHY_OrANAReg (uint32 addr, uint32 value)
{
    uint16 tmp_value = 0;

	tmp_value = CHIP_PHY_GetANAReg(addr);
	tmp_value |= (uint16)value;
    CHIP_PHY_SetANAReg(addr, tmp_value);
}

PUBLIC void ADI_Phy_init (void)
{
    uint32 value;

    /* initialize the adi registers */
    hwp_adiMst->adi_ctrl = 0;

    /* set the channel priority */
    hwp_adiMst->adi_pril = ADI_MST_CHNL0_PRI(0)|ADI_MST_CHNL1_PRI(1);

    /* get the readback addr mask */

    /* value = 0x7490835d; */
    value = ADI_MST_RF_GSSI_FAST_MODE |
            ADI_MST_RF_GSSI_DUMMY_CLK_EN |
            ADI_MST_RF_GSSI_OE_CFG |
            ADI_MST_RF_GSSI_SYNC_MODE |
            ADI_MST_RF_GSSI_WR_POS(16) |
            ADI_MST_RF_GSSI_DATA_LEN(16) |
            ADI_MST_RF_GSSI_CMD_LEN(12+1) |
            ADI_MST_RF_GSSI_FRAME_LEN(12+1+16) |
            ADI_MST_RF_GSSI_SCK_ALL_ON;
    hwp_adiMst->adi_cfg0 = value;

    /* value = 0x80040143; ADI_MST_RF_GSSI_DUMMY_LEN need set to 4 */
    value = ADI_MST_RF_GSSI_STRTBIT_MODE |
            ADI_MST_RF_GSSI_DUMMY_LEN(4) |
            ADI_MST_RF_GSSI_CLK_DIV(1) |
            ADI_MST_RF_GSSI_NG_RX |
            ADI_MST_RF_GSSI_NG_TX;
    hwp_adiMst->adi_cfg1 = value;

    /* set ADI clock auto gate */
    value = hwp_adiMst->adi_cfg0;
    value &= ~ADI_MST_RF_GSSI_SCK_ALL_ON;
    hwp_adiMst->adi_cfg0 = value;
}
#endif

int NOR_FDL1_SysConnect (PACKET_T *packet, void *arg)
{
    rprintf("NOR_FDL1_SysConnect\n");
    FDL_SendAckPacket (BSL_REP_ACK);
    return 1;
}


int NOR_FDL1_DataStart (PACKET_T *packet, void *arg)
{
    //unsigned long *ptr = (unsigned long *) packet->packet_body.content;

    unsigned long *data = (unsigned long *) (packet->packet_body.content);
    unsigned long start_addr = EndianConv_32 (*data);
    unsigned long file_size = EndianConv_32 (* (data + 1));

    rprintf("NOR_FDL1_DataStart: 0x%x %u\n", start_addr, file_size);
    if(!g_is_keep_charge_enable)
	{
#if defined(PLATFORM_UWS6121E)
		CHIP_PHY_OrANAReg(ANA_CHGR_CTL0, CHGR_PD); //shutdown charge
#else
		ANA_REG_OR (ANA_CHGR_CTL0,CHGR_PD); //shutdown charge
#endif
	}

    if ( (start_addr < MEMORY_START) || (start_addr >= MEMORY_START + MEMORY_SIZE))
    {
        FDL_SendAckPacket (BSL_REP_DOWN_DEST_ERROR);
        return 0;
    }

    if ( (start_addr + file_size) > (MEMORY_START + MEMORY_SIZE))
    {
        FDL_SendAckPacket (BSL_REP_DOWN_SIZE_ERROR);
        return 0;
    }

    g_file.start_address = start_addr;
    g_file.total_size = file_size;
    g_file.recv_size = 0;
    g_file.next_address = start_addr;

	FDL_memset ( (void *) start_addr, 0, file_size);

    if (!packet->ack_flag)
    {
        packet->ack_flag = 1;
        FDL_SendAckPacket (BSL_REP_ACK);
    }

    return 1;
}



int NOR_FDL1_DataMidst (PACKET_T *packet, void *arg)
{
    unsigned short data_len = packet->packet_body.size;

    rprintf("NOR_FDL1_DataMidst\n");
    if ( (g_file.recv_size + data_len) > g_file.total_size)
    {
        FDL_SendAckPacket (BSL_REP_DOWN_SIZE_ERROR);
        return 0;
    }

	FDL_memcpy ( (void *) (g_file.next_address), (const void *) (packet->packet_body.content), data_len);

    g_file.next_address += data_len;
    g_file.recv_size += data_len;

    if (!packet->ack_flag)
    {
        packet->ack_flag = 1;
        FDL_SendAckPacket (BSL_REP_ACK);
    }

    return 1;
}

int NOR_FDL1_DataEnd (PACKET_T *packet, void *arg)
{
#ifdef SECURE_BOOT_SUPPORT

    rprintf("NOR_FDL1_DataEnd\n");
       bootGetEfuseCtx();
	if (!bootSimageCheckSign((const simageHeader_t *)(g_file.start_address)))
	{
		loop_var1 = 1;
    	        rprintf("NOR_FDL1 secure boot check fail\n");
		FDL_SendAckPacket(BSL_REP_SECURITY_VERIFICATION_FAIL);
		while(loop_var1 == 1)
		{
			;//Do Nothing
		}
	}
#endif
    if (!packet->ack_flag)
    {
        packet->ack_flag = 1;
        FDL_SendAckPacket (BSL_REP_ACK);
    }

    return 1;
}

int NOR_FDL1_DataExec (PACKET_T *packet, void *arg)
{
    rprintf("NOR_FDL1_DataExec 0x%x\n", g_file.start_address + FDL_SECURE_BOOT_HEAD_SIZE+FDL_SIMAGE_HEAD_SIZE);
    //BSL_REP_ACK is sent in the begin of Nor FDL2

    JumpToTarget (g_file.start_address + FDL_SECURE_BOOT_HEAD_SIZE+FDL_SIMAGE_HEAD_SIZE);

    return 0;
}

int NOR_FDL1_SetBaudrate (PACKET_T *packet, void *arg)
{
    unsigned long baudrate = * (unsigned long *) (packet->packet_body.content);
    //int           i;
    baudrate = EndianConv_32 (baudrate);

    rprintf("NOR_FDL1_SetBaudrate\n");

    if (!packet->ack_flag)
    {
        packet->ack_flag = 1;
        FDL_SendAckPacket (BSL_REP_ACK);
    }

    gFdlUsedChannel->SetBaudrate (gFdlUsedChannel, baudrate);
    return 0;

}

int NOR_FDL1_KeepCharge (PACKET_T *packet, void *arg)
{
    rprintf("NOR_FDL1_KeepCharge\n");

    if (!packet->ack_flag)
    {
        packet->ack_flag = 1;
        FDL_SendAckPacket (BSL_REP_ACK);
    }

    g_is_keep_charge_enable = TRUE;

#if defined(PLATFORM_UWS6121E)
	CHIP_PHY_OrANAReg (ANA_CHGR_CTL0, ~CHGR_PD); //keep charge
#else
	ANA_REG_OR (ANA_CHGR_CTL0, ~CHGR_PD); //keep charge
#endif

    return 1;
}

