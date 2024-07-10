#include "sci_types.h"
#include "cmd_def.h"
#include "packet.h"
#include "dl_engine.h"
#include "sc_reg.h"
#include "fdl_nand.h"
#include "sci_ftl_api.h"
#include "nand_spec.h"
#include "nand_partitionCfg.h"
#include "nand_controller.h"
#include "fdl_nand.h"
#include "fdl_channel.h"

#define SWECC_E_ERROR                1    /* ECC error                       */  
#define SWECC_N_ERROR                0    /* no error                        */
#define SWECC_C_ERROR               -1    /* one bit data error              */
#define SWECC_U_ERROR               -2    /* uncorrectable error             */

#define BW_X08                       0
#define BW_X16                       1

#define PACKET_MAX_NUM    3
#define ADDR_MASK               0x80000000
#define HEADER_OFFSET	0x3A00
#define BOOTLOADER0_SIZE	0x4000
#define BOOT0_NUMBER 3
#define BOOT_SCT_SIZE 0x200

uint32 s_intel_m18_serial = 0;
typedef struct 
{
	unsigned int rsa_key[32];
	unsigned int rsa_e;
	unsigned int image_check_flag; //if 0, check fail, else check sucessful
}BOOT_CHECK_CONTEX;
typedef struct _DL_FILE_STATUS
{
    unsigned long   total_size;
    unsigned long   recv_size;
} DL_FILE_STATUS, *PDL_FILE_STATUS;
static unsigned long g_checksum;
static unsigned long g_sram_addr;
static BOOT_CHECK_CONTEX s_boot1_header = {0};
static __align (4) unsigned char g_fixnv_buf[0x20000];
static __align (4) unsigned char g_bootheader_buf[0x20000];
static const char VERSION_STR[] = {"Spreadtrum Boot Block version 1.1"};
static uint32 md5_key[5];
#define CHECKSUM_OTHER_DATA       0x5555aaaa
static DL_FILE_STATUS g_status;
static int g_prevstatus;
static NAND_SPEC	*s_spec;
static void _BOOT_UpdateCheckImageFlag(int pass);
static void SystemDelay (unsigned int delay_ms);
static int check_enable = 0 ;
//only for compile sucessful!!!
struct FDL_ChannelHandler gUart0Channel;
struct FDL_ChannelHandler gUart1Channel;
void sio_trace (char *msg)
{
}
static __inline DLSTATUS BOOT_ConvertErr (int err)
{
    switch (err)
    {
        case NAND_SUCCESS:
            return BSL_REP_ACK;
        case NAND_INVALID_ADDR:
            return BSL_REP_DOWN_DEST_ERROR;
        case NAND_INVALID_SIZE:
            return BSL_REP_DOWN_SIZE_ERROR;
        case NAND_UNKNOWN_DEVICE:
            return BSL_UNKNOWN_DEVICE;
        case NAND_INVALID_DEVICE_SIZE:
            return BSL_INVALID_DEVICE_SIZE;
        case NAND_INCOMPATIBLE_PART:
            return BSL_INCOMPATIBLE_PARTITION;
        case NAND_INVALID_STL_SIZE:
            return BSL_REP_DOWN_STL_SIZE_ERROR; 
        default:
            return BSL_REP_OPERATION_FAILED;
    }
}
static __inline void BOOT_SendRep (unsigned long err)
{
    FDL_SendAckPacket (BOOT_ConvertErr (err));
}
unsigned long BOOT_GetRecvDataSize (void)
{
    return g_status.recv_size;
}
BOOLEAN BOOT_VerifyFlashAdd(unsigned long start_addr)
{
	// 1 get part ID
	if(0 == (ADDR_MASK&start_addr))
	{
		// not support
		return FALSE;
	}
	return TRUE;
}

int BOOT_DataStart (PACKET_T *packet, void *arg)
{
    unsigned long *data = (unsigned long *) (packet->packet_body.content);
    unsigned long start_addr = *data;
    unsigned long size = * (data + 1);
    int           ret;
#if defined(CHIP_ENDIAN_LITTLE)
    start_addr = EndianConv_32 (start_addr);
    size = EndianConv_32 (size);
#endif
	/* if the start_addr is illegal */
	if(!BOOT_VerifyFlashAdd(start_addr))
	{
		FDL_SendAckPacket (BSL_REP_DOWN_DEST_ERROR); /*lint !e527*/
		return 0;
	}
	//only check kernel and user image
	if((start_addr & 0xffff0000) == 0x80000000)
	{
		check_enable = 1;
		FDL_CryptoInit(size, md5_key, 0);		
	}
	else
	{
		check_enable = 0;
	}	
    if (packet->packet_body.size == 12)
    {
        g_checksum = * (data+2);
        g_sram_addr = (unsigned long) g_fixnv_buf;
    }
    else
    {
        g_checksum = CHECKSUM_OTHER_DATA;
    }

    if (0 == (g_checksum & 0xffffff))
    {
        //The fixnv checksum is error.
        SEND_ERROR_RSP (BSL_EEROR_CHECKSUM); /*lint !e527*/
    }

    do
    {
        /* Check the validity of the address and size of the file to be downloaded,
         * and erase this space of NAND flash. */
        ret = nand_start_write (start_addr, size);

        if (NAND_SUCCESS != ret)
        {
            break;
        }

        g_status.total_size  = size;
        g_status.recv_size   = 0;
        g_prevstatus = NAND_SUCCESS;

        FDL_SendAckPacket (BSL_REP_ACK);

        return 1;
    }
    while (0);

    BOOT_SendRep (ret);
    return 0;
}
int BOOT_DataMidst (PACKET_T *packet, void *arg)
{
    unsigned short  size;
	int valid_size = 0;

    /* The previous download step failed. */
    if (NAND_SUCCESS != g_prevstatus)
    {
        BOOT_SendRep (g_prevstatus);
        return 0;
    }

    size = packet->packet_body.size;

    if ( (g_status.recv_size + size) > g_status.total_size)
    {
        g_prevstatus = NAND_INVALID_SIZE;
        BOOT_SendRep (g_prevstatus);
        return 0;
    }
	if(check_enable)
	{
		valid_size = FDL_CryptoUpdate((unsigned char *) (packet->packet_body.content), (unsigned int) size);
		if(-1 == valid_size)
		{
			SEND_ERROR_RSP (BSL_REP_VERIFY_ERROR); /*lint !e527*/
			return 0;
		}
		size = valid_size & 0xffff;
	}	
    if (CHECKSUM_OTHER_DATA == g_checksum)
    {
        g_prevstatus = nand_write ( (unsigned int) size, (unsigned char *) (packet->packet_body.content));

        if (NAND_SUCCESS == g_prevstatus)
        {
            g_status.recv_size += size;

            if (!packet->ack_flag)
            {
                packet->ack_flag = 1;
                FDL_SendAckPacket (BSL_REP_ACK);
                return NAND_SUCCESS == g_prevstatus;
            }
        }

        BOOT_SendRep (g_prevstatus);
        return NAND_SUCCESS == g_prevstatus;
    }
    else //It's fixnv data. We should backup it.
    {
        memcpy ( (unsigned char *) g_sram_addr, (char *) (packet->packet_body.content), size); /*lint !e18 !e718 for "memcpy" */
        g_sram_addr+=size;
        FDL_SendAckPacket (BSL_REP_ACK);
        return 1;
    }	
}
int BOOT_DataEnd (PACKET_T *packet, void *arg)
{
    unsigned long i, fix_nv_size, fix_nv_checksum;
	if(check_enable)
	{
		if(0 == FDL_CryptoCheck())
		{
			_BOOT_UpdateCheckImageFlag(FALSE);
			SEND_ERROR_RSP (BSL_REP_VERIFY_ERROR); /*lint !e527*/
			return 0;
		}
		else
		{
			_BOOT_UpdateCheckImageFlag(TRUE);
		}
	}
    if (CHECKSUM_OTHER_DATA != g_checksum)
    {
        fix_nv_size = g_sram_addr - (unsigned long) g_fixnv_buf;

        fix_nv_checksum = Get_CheckSum ( (unsigned char *) g_fixnv_buf, fix_nv_size);

        fix_nv_checksum = EndianConv_32 (fix_nv_checksum);

        if (fix_nv_checksum != g_checksum)
        {
            SEND_ERROR_RSP (BSL_CHECKSUM_DIFF); /*lint !e527*/
        }

        for (i=0; i<3; i++)
        {
            if (nand_write (fix_nv_size, g_fixnv_buf) == NAND_SUCCESS)
            {
                //Double check the flash nv area is correct. If it is Nand, we needn't do this.
                g_prevstatus = NAND_SUCCESS;
                break;
            }
        }

        if (i==3)
        {
            //Write error happened
            SEND_ERROR_RSP (BSL_WRITE_ERROR); /*lint !e527*/
        }
    }

    if (NAND_SUCCESS != g_prevstatus)
    {
        BOOT_SendRep (g_prevstatus);
        return 0;
    }

    g_prevstatus = nand_end_write();
    BOOT_SendRep (g_prevstatus);
	if(NAND_SUCCESS == g_prevstatus)
	{
		//BOOT_UpdateCheckImageFlag(TRUE);
	}
    return (NAND_SUCCESS == g_prevstatus);
}
int BOOT_ReadFlash (PACKET_T *packet, void *arg)
{
    unsigned long *data = (unsigned long *) (packet->packet_body.content);
    unsigned long addr = *data;
    unsigned long size = * (data + 1);
    unsigned long off = 0;
    int           ret;

#if defined(CHIP_ENDIAN_LITTLE)
    addr = EndianConv_32 (addr);
    size = EndianConv_32 (size);
#endif
	/* if the start_addr is illegal */
	if(!BOOT_VerifyFlashAdd(addr))
	{
		FDL_SendAckPacket (BSL_REP_DOWN_DEST_ERROR); /*lint !e527*/
		return 0;
	}

    if (size > MAX_PKT_SIZE)
    {
        FDL_SendAckPacket (BSL_REP_DOWN_SIZE_ERROR);
        return 0;
    }

    if (packet->packet_body.size > 8)
    {
        off = EndianConv_32 (* (data + 2));
    }

    ret = nand_read (addr, off, size, (unsigned char *) (packet->packet_body.content));

    if (NAND_SUCCESS == ret)
    {

        packet->packet_body.type = BSL_REP_READ_FLASH;
        packet->packet_body.size = size;
        FDL_SendPacket (packet);
        return 1;
    }
    else
    {
        BOOT_SendRep (ret);
        return 0;
    }
}
int BOOT_EraseFlash (PACKET_T *packet, void *arg)
{
    unsigned long *data = (unsigned long *) (packet->packet_body.content);
    unsigned long addr = *data;
    unsigned long size = * (data + 1);
    int           ret;

    addr = EndianConv_32 (addr);
    size = EndianConv_32 (size);
	/* if the start_addr is illegal */
	if(!BOOT_VerifyFlashAdd(addr))
	{
		FDL_SendAckPacket (BSL_REP_DOWN_DEST_ERROR); /*lint !e527*/
		return 0;
	}

    ret = nand_erase (addr, size);
    BOOT_SendRep (ret);
    return (NAND_SUCCESS == ret);
}
static int BOOT_NandInit()
{
	uint32			nandId;
	uint8	idArr[4] = {0,0,0,0};
	unsigned __int64	ID = 0;
	uint32 i;
	NAND_PARA_T		nandPara;
	NAND_TIMING_T	nandTiming;
	
	NANDCTL_Read_ID(idArr);
	ID = (((unsigned __int64)idArr[0])<<8)|((unsigned __int64)idArr[1]);
	s_spec = GetNandParam(ID);
	if(0 == s_spec)
	{
		return NAND_SYSTEM_ERROR;
	}
	nandTiming.nTeTime	= s_spec->nTeTime;
	nandTiming.nTpTime	= s_spec->nTwTime;
	nandTiming.nTrTime	= s_spec->nTrTime;
	nandTiming.nTwcTime	= s_spec->nTfTime;
	NANDCTL_InitParam(&nandTiming);
	nandPara.acycle = s_spec->nCycleDev;
	if(1 == s_spec->nSctPerPg)
	{
		nandPara.pagesize	= 512;
		nandPara.sparesize	= 16;
	}
	else
	{
		nandPara.pagesize	= 2048;
		nandPara.sparesize	= 64;
	}
	nandPara.buswidth	= s_spec->nBWidth;
	nandPara.advance	= s_spec->nAdvance;
	nandPara.eccpos		= s_spec->mEccPos;
	NANDCTL_Open(&nandPara);

	if(-1 != s_spec->safeDataPos)
	{
		NANDCTL_ONDIE_ECCOpen(FALSE);
	}
}
static int BOOT_ReadBoot1Header(BOOT_CHECK_CONTEX *header)
{
#if 1
	uint8 sBuf[64];
	uint8 sEcc[16];
	uint32 i;
	uint32 ret;
	uint32 ecc_pos;

	for(i = 0; i < 3; i++)
	{
		//small page
		
		if(1 == s_spec->nSctPerPg)
		{
			ecc_pos = 6;
			if(ERR_NF_SUCCESS != NANDCTL_MS_Read_S ((i * BOOTLOADER0_SIZE)  + HEADER_OFFSET, g_bootheader_buf, sBuf, sEcc))
			{
				
				return NAND_SYSTEM_ERROR;
			}
		}
		else
		{
			ecc_pos = 8;
			if(ERR_NF_SUCCESS != NANDCTL_MS_Read_L ((i * BOOTLOADER0_SIZE)  + HEADER_OFFSET, 0, 1, g_bootheader_buf, sBuf, sEcc))
			{
				
				return NAND_SYSTEM_ERROR;
			}
		}
		ret = ECC_CompM(sBuf + ecc_pos, sEcc, g_bootheader_buf, s_spec->nBWidth);
		if(ret != SWECC_U_ERROR)
		{
			break;
		}
	}
	if(i == 3)
	{
		return NAND_SYSTEM_ERROR;
	}
	memcpy(header, g_bootheader_buf, sizeof(*header));
#endif	
	return NAND_SUCCESS;	
}
/******************************************************************************/
//  Description:  read the data at bootloader
//  Parameter:
//      buf: [output]
//		offset: the address at the bootloader
//		size: read length
//  Return:
//      NAND_SUCCESS
//		NAND_SYSTEM_ERROR
/******************************************************************************/
static int _BOOT_NandReadBootloaderSct(uint8 *buf, uint32 offset, uint32 size)
{
	uint32 i;
	uint32 ecc_pos;
	uint32 nsct;
	uint8 sBuf[64];
	uint8 sEcc[16];
	uint32 ret;

	if(1 == s_spec->nSctPerPg)
	{
		ecc_pos = 6;
		if(ERR_NF_SUCCESS != NANDCTL_MS_Read_S (offset, buf, sBuf, sEcc))
		{
              return NAND_SYSTEM_ERROR;
		}
	}
	else
	{
		ecc_pos = 8;
		if(ERR_NF_SUCCESS != NANDCTL_MS_Read_L (offset, 0, 1, buf, sBuf, sEcc))
		{
        	return NAND_SYSTEM_ERROR;
		}
	}
	ECC_CompM(sBuf + ecc_pos, sEcc, buf, s_spec->nBWidth);
}  
/******************************************************************************/
//  Description:  read the image boot header 
//  Parameter:
//		NONE
//  Return:
//      NAND_SUCCESS
//		NAND_SYSTEM_ERROR
/******************************************************************************/
static void _BOOT_GetImageCheckFlag(void)
{
	BOOT_NandInit();
	BOOT_ReadBoot1Header(&s_boot1_header);
}
static void _BOOT_UpdateCheckImageFlag(int pass)
{
	int i;
	int add;
	int readSize;
	BOOT_CHECK_CONTEX *contx;
	uint8 sBuf[64];
	uint8 sEcc[16];

	if((s_boot1_header.image_check_flag && pass) || (!s_boot1_header.image_check_flag && !pass))
	{
		return;
	}
	if(1 == s_spec->nSctPerPg)
	{
		readSize = BOOTLOADER0_SIZE * 3;
	}
	else
	{
		readSize = 0x20000;
	}
	//read image	
	if(1 == s_spec->nSctPerPg)
	{
		for(add = 0; add < readSize; add += BOOT_SCT_SIZE)		
		{
			_BOOT_NandReadBootloaderSct(g_bootheader_buf + add, add, BOOT_SCT_SIZE);
		}
	}
	//erase block
	if(1 == s_spec->nSctPerPg)
	{
		for(i = 0; i < BOOT0_NUMBER; i++)
		{
			NANDCTL_Erase_Block_S(i * BOOTLOADER0_SIZE);
		}
	}
	else
	{
		NANDCTL_Erase_Block_L(0);
	}
	//update boot0 and boot1
	for(i = 0;  i < BOOT0_NUMBER; i++)
	{
		contx = g_bootheader_buf + (i * BOOTLOADER0_SIZE) + HEADER_OFFSET;
		contx->image_check_flag = pass;
	}
	//write image
	for(add = 0; add < (readSize);)
	{
		if(1 == s_spec->nSctPerPg)
		{
			NANDCTL_MS_Write_S(add, g_bootheader_buf + add, sBuf, 1);
			add += BOOT_SCT_SIZE;
		}
		else
		{
			NANDCTL_MS_Write_L(add,  0,4, g_bootheader_buf + add,sBuf, 1);
			add += (BOOT_SCT_SIZE * 4);
		}
	}
}

/******************************************************************************/
//  Description:   check if in download mode
//  Parameter:
//      None
//  Return:
//      TRUE: in download mode
//		FALSE: not in download mode
/******************************************************************************/
BOOLEAN BOOT_CheckDownloadMode(void)
{
	uint32 	key_raw_int_sts = 0;
	uint32 key_sts = 0;
	_BOOT_GetImageCheckFlag();
	//enable keypad
	*(volatile uint32 *)(0x8b000000) |= BIT_5;
	SystemDelay(20);
	key_raw_int_sts = *(volatile uint32 *)(0x87000008);
	key_sts = *(volatile uint32 *)(0x8700002c);
	key_raw_int_sts &= 0x3;
	key_sts &= 0x7777;
	
    if ((key_raw_int_sts == 0x3) && ((key_sts == 0x3130) || (key_sts == 0x3031)))    
    {
		return TRUE;
    }
	else
	{
		if(s_boot1_header.image_check_flag != 0)
		{
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

unsigned int SystemCountGet (void)
{
  volatile unsigned int clock;
  volatile unsigned int clock_c;

  /*read two times all the same to make sure*/
  clock = *(volatile unsigned int *) (SYSTEM_CURRENT_CLOCK);
  clock_c = *(volatile unsigned int *) (SYSTEM_CURRENT_CLOCK);

  while (clock != clock_c)
    {
      clock = *(volatile unsigned int *) (SYSTEM_CURRENT_CLOCK);
      clock_c = *(volatile unsigned int *) (SYSTEM_CURRENT_CLOCK);
    }

  return (clock_c);
}
void
SystemDelay (unsigned int delay_ms)
{
  unsigned int tmp;

  tmp = SystemCountGet () + delay_ms;

  while (tmp > SystemCountGet ());
}
#ifdef PLATFORM_SC6800H
PUBLIC void usb_ldo_enable(unsigned char is_usb_ldo_enabled)
{
    volatile unsigned long usb_ldo = 0;
    
    usb_ldo  = *(volatile unsigned long *)(GR_LDO_PD);
    
    if (is_usb_ldo_enabled)
    {
        usb_ldo &= (unsigned int)(~(BIT_10));
        usb_ldo |= (unsigned int)(BIT_11);
    }
    else
    {
        usb_ldo |= (unsigned int)(BIT_10);
        usb_ldo &= (unsigned int)(~(BIT_11));
    }
    
    *(volatile unsigned long *)(GR_LDO_PD) = usb_ldo;
    
    return;
}
void BOOT_UsbConfig()
{
	volatile unsigned long ext_clk_26M = 1;

    /* Tell fdl USB Boot */
    *(volatile uint32 *) (0x8b00003c) |= (0x5a << 8);

    usb_ldo_enable (FALSE);

    /* enable usb pull up resistance */
    *(volatile uint32 *) (0x8b000030) |= BIT_27;
	 /* Please wait for a moment */
    SystemDelay(300);
	usb_boot(1);  
	usb_ldo_enable(TRUE); 
}
#else
//not support now
void BOOT_UsbConfig()
{
}
#endif
int BOOT_SysConnect (PACKET_T *packet, void *arg)
{
    FDL_SendAckPacket (BSL_REP_ACK);
    return 1;
}
//Only support usb download
int BOOT_SetBaudrate (PACKET_T *packet, void *arg)
{
    FDL_SendAckPacket (BSL_REP_ACK);
    return 0;
}
/******************************************************************************/
//  Description:   begin to download
//  Parameter:
//      None
//  Return:
//      None
/******************************************************************************/
void Boot_Download(void)
{
	int err;
	uint8 ch;
	PACKET_T         *packet;
	MMU_Init();
	BOOT_UsbConfig();
	FDL_PacketInit();
    /* Initialize NAND flash. */
    err = nand_flash_init();
    if ( (NAND_SUCCESS != err) && (NAND_INCOMPATIBLE_PART != err))
    {
        BOOT_SendRep (BOOT_ConvertErr (err));
        return;
    }

    /* Register command handler */
	FDL_DlInit();
	FDL_DlReg (BSL_CMD_CONNECT,    	  BOOT_SysConnect,  0);
	FDL_DlReg(BSL_CMD_START_DATA,     BOOT_DataStart,         0);
	FDL_DlReg(BSL_CMD_MIDST_DATA,     BOOT_DataMidst,         0);
	FDL_DlReg(BSL_CMD_END_DATA,       BOOT_DataEnd,           0);
	FDL_DlReg(BSL_CMD_READ_FLASH,     BOOT_ReadFlash,         0);
	FDL_DlReg(BSL_ERASE_FLASH,        BOOT_EraseFlash,        0);
	FDL_DlReg (BSL_SET_BAUDRATE,   BOOT_SetBaudrate, 0);
    for ( ; ; )
    {
        ch = gFdlUsedChannel->GetChar (gFdlUsedChannel);

        if (0x7e == ch)
        {
            break;
        }
    }


    //when got the signal of 0x7e ,send response to pc    
    packet = FDL_MallocPacket();
    packet->packet_body.type = BSL_REP_VER;
    packet->packet_body.size = sizeof (VERSION_STR);
    FDL_memcpy (packet->packet_body.content, VERSION_STR, sizeof (VERSION_STR));
    FDL_SendPacket (packet);
    FDL_FreePacket (packet);
	
    /* Start the download process. */
    FDL_DlEntry (DL_STAGE_CONNECTED);

}
