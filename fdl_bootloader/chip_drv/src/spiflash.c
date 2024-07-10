/******************************************************************************
 ** File Name:      spiflash.c                                             *
 ** Author:         Fei.Zhang                                              *
 ** DATE:           01/15/2011                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                 SPI flash Low-level drvier.                             *
 **                 This driver support ALL SPI flash                    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01-15-2010     Fei.Zhang          Create.                                   *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_api.h"
#include "sfc_drvapi.h"
#include "spiflash.h"
#include "fdl_main.h"
#include "timer_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif

#if defined(PLATFORM_UWS6121E)
#ifdef FLASH_DELAY_US
#undef FLASH_DELAY_US
#define FLASH_DELAY_US TIMER_DELAY_US
#endif
#endif

#ifdef SCI_ASSERT
#undef SCI_ASSERT
//#define SCI_ASSERT(_EXP)   NULL
#endif

#define SCI_ASSERT(_EXP)    \
    {\
        if((_EXP) == 0)\
        {\
            FDL_SendErrLog("ASSERT! %s %d", __func__, __LINE__);\
            while(1);\
        }\
    }

/**---------------------------------------------------------------------------*
 **              variable declare                                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Variable                                   *
 **---------------------------------------------------------------------------*/
#if defined(PLATFORM_UWS6121E)
#define FLASH_START_ADRESS            0x0 // spiflash_cfg use absolute address
#if defined(CHIP_VER_UWS6121E)
#ifdef SIMULATE_FLASH2_SUPPORT
#define FLASH2_START_ADRESS            0x80800000     //phy_addr from chip
#else
#define FLASH2_START_ADRESS            0x70000000     //phy_addr from chip
#endif
#endif
#else
#ifdef TF_UPDATE_SC6530
#define FLASH_START_ADRESS            0x0
#else
#if defined(PLATFORM_SC6531EFM)
#define FLASH_START_ADRESS   ((*(volatile uint32 *)0x205000E0 & 0x1) ? 0x10000000 : 0x0000000)
#else
#define FLASH_START_ADRESS   ((*(volatile uint32 *)0x205000E0 & 0x1) ? 0x30000000 : 0x0000000)
#endif
#endif
#endif

 PUBLIC uint32 flash_cs0_desity = 0;
 PUBLIC uint8   flash_cs0_MID = 0;
 PUBLIC uint8   flash_cs0_DID1 = 0;
 PUBLIC uint8   flash_cs0_DID2 = 0;

 PUBLIC uint32 flash_cs1_desity = 0;
 PUBLIC uint8   flash_cs1_MID = 0;
 PUBLIC uint8   flash_cs1_DID1 = 0;
 PUBLIC uint8   flash_cs1_DID2 = 0;

#if defined(CHIP_VER_UWS6121E)
#define SPIFLASH_CS1_BASE_ADDRESS	    (FLASH2_START_ADRESS)
#else
#define SPIFLASH_CS1_BASE_ADDRESS	    (FLASH_START_ADRESS + flash_cs0_desity)
#endif

//common command
#define CMD_READ			0x03
#define CMD_FAST_READ		0x0B	//QPI 4READ
#define CMD_4READ			0xEB	//SPI 4READ/QPI 4READ
#define CMD_WREN			0x06
#define CMD_WRDI			0x04
#define CMD_RDSR1			0x05
#define CMD_RDSR2			0x35
#define CMD_WRSR			0x01
#define CMD_4PP				0x32
#define CMD_SE				0x20
#define CMD_32KBE			0x52
#define CMD_64KBE			0xD8
#define CMD_CE				0xC7	//or 0x60
#define CMD_PP				0x02
#define CMD_RDID			0x9F
#define CMD_RSTEN			0x66
#define CMD_RST				0x99
#define CMD_SETBURST		0xC0
#define CMD_DP				0xB9
#define CMD_RDP				0xAB
#define CMD_WBPR            0x42
#define CMD_RDVECR          0x65
#define CMD_WRVECR          0x61
#define CMD_RFSR            0x70
#define CMD_CLFSR           0x50
#define CMD_RDVCR           0x85
#define CMD_WRVCR           0x81
#define CMD_RDCR            0x15
#define CMD_4BYTEADDREN		0xB7
#define CMD_4BYTEADDRDI		0xE9
#define CMD_QUAD_FAST_READ      0x6B


#define FLASH_DO_IDLE              FDL_PacketDoIdle();


#define SFC_CLK_DIV_0  0
#define SFC_CLK_DIV_2  2

#define SFC_CLK_SAMPLE_DELAY_SEL  4
#define SFC_CLK_2X_EN             BIT_10
#define SFC_CLK_OUT_2X_EN         BIT_9
#define SFC_CLK_SAMPLE_2X_PHASE   BIT_8
#define SFC_CLK_SAMPLE_2X_EN      BIT_7

LOCAL uint8 s_nMID[SFC_MAX_CS] = {0};			/* nMID*/
LOCAL uint8 s_nDID1[SFC_MAX_CS] = {0};              /* nDID1*/
LOCAL uint8 s_nDID2[SFC_MAX_CS] = {0};              /* nDID2*/
LOCAL uint8 nQEPos[SFC_MAX_CS] = {0};			/* QE bit Poisition, 0xFF indicates no QE bit*/
LOCAL uint8 nWIPPos[SFC_MAX_CS] = {0};        /* WIP bit Poisition, 0xFF indicates no WIP bit*/
LOCAL uint8 EnterQPI[SFC_MAX_CS] = {0};		/* Command to enter QPI mode*/
LOCAL uint8 ExitQPI[SFC_MAX_CS] = {0};			/* Command to exit QPI mode,  0x00 indicates not support*/
LOCAL uint8 cmd_4pp[SFC_MAX_CS] = {0};		/* Command to write in 4 I/O mode */
LOCAL uint8 cmd_4read[SFC_MAX_CS] = {0};      /* Command to read in 4 I/O mode */
LOCAL BOOLEAN reset_support[SFC_MAX_CS] = {0};	/* sw reset support*/
LOCAL BOOLEAN QPI_support[SFC_MAX_CS] = {0};	/* QPI mode support*/
LOCAL uint8 CMD_PES[SFC_MAX_CS] = {0};		/* Program/Erase Suspend Command */
LOCAL uint8 CMD_PER[SFC_MAX_CS] = {0};		/* Program/Erase Resume Command */
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#if defined(CHIP_VER_UWS6121E)

#if defined(FLASH_SIZE_128MBITX64MBIT_NEW) || defined(FLASH_SIZE_128MBITX64MBIT_OLD) || defined(FLASH_SIZE_128MBITX32MBIT)
LOCAL BYTE_NUM_E   s_address_cycle[SFC_MAX_CS] = {BYTE_NUM_3, BYTE_NUM_3}; /* address cycle info */
LOCAL uint32 s_address_mask[SFC_MAX_CS] = {SPIFLASH_3_ADDR_CYCLE_MASK, SPIFLASH_3_ADDR_CYCLE_MASK}; /* address cycle info */
LOCAL EXT_CFG_PTR    pExtcfg[SFC_MAX_CS] = {0, 0}; /* extension cfg info */
LOCAL uint8 work_mode[SFC_MAX_CS] = {SPI_MODE, SPI_MODE};	/* work mode: SPI mode or QPI mode*/
#elif FLASH_SIZE_128MBIT
LOCAL BYTE_NUM_E   s_address_cycle[SFC_MAX_CS] = {BYTE_NUM_3}; /* address cycle info */
LOCAL uint32 s_address_mask[SFC_MAX_CS] = {SPIFLASH_3_ADDR_CYCLE_MASK}; /* address cycle info */
LOCAL EXT_CFG_PTR    pExtcfg[SFC_MAX_CS] = {0}; /* extension cfg info */
LOCAL uint8 work_mode[SFC_MAX_CS] = {SPI_MODE};	/* work mode: SPI mode or QPI mode*/
#endif

#else
LOCAL BYTE_NUM_E   s_address_cycle[SFC_MAX_CS] = {BYTE_NUM_3}; /* address cycle info */
LOCAL uint32 s_address_mask[SFC_MAX_CS] = {SPIFLASH_3_ADDR_CYCLE_MASK}; /* address cycle info */
LOCAL EXT_CFG_PTR    pExtcfg[SFC_MAX_CS] = {0}; /* extension cfg info */

LOCAL uint8 work_mode[SFC_MAX_CS] = {SPI_MODE};	/* work mode: SPI mode or QPI mode*/
#endif
#else
LOCAL BYTE_NUM_E   s_address_cycle[SFC_MAX_CS] = {BYTE_NUM_3, BYTE_NUM_3}; /* address cycle info */
LOCAL uint32 s_address_mask[SFC_MAX_CS] = {SPIFLASH_3_ADDR_CYCLE_MASK, SPIFLASH_3_ADDR_CYCLE_MASK}; /* address cycle info */
LOCAL EXT_CFG_PTR    pExtcfg[SFC_MAX_CS] = {0}; /* extension cfg info */

LOCAL uint8 work_mode[SFC_MAX_CS] = {SPI_MODE, SPI_MODE};	/* work mode: SPI mode or QPI mode*/
#endif

#define WINBOND_DUMMYCLK_2_FREQ_50MHZ			(0x0)
#define WINBOND_DUMMYCLK_4_FREQ_80MHZ			(0x10)
#define WINBOND_DUMMYCLK_6_FREQ_104MHZ			(0x20)
#define WINBOND_DUMMYCLK_8_FREQ_104MHZ			(0x30)

#define GD_DUMMYCLK_4_FREQ_80MHZ           (0x0)
#define GD_DUMMYCLK_6_FREQ_104MHZ         (0x10)
#define GD_LQ32M_DUMMYCLK_6_FREQ_104MHZ   (0x20)
#define GD_LQ64M_DUMMYCLK_6_FREQ_104MHZ   (0x20)
#define GD_DUMMYCLK_8_FREQ_120MHZ         (0x20)
#define GD_DUMMYCLK_12_FREQ_140MHZ       (0x30)

#define FM_DUMMYCLK_4_FREQ_80MHZ           (0x0)
#define FM_DUMMYCLK_6_FREQ_104MHZ         (0x10)
#define FM_25M_DUMMYCLK_6_FREQ_104MHZ         (0x20)


#define MICRON_DUMMYCLK_6_FREQ_80MHZ      (0x60)
#define MICRON_DUMMYCLK_10_FREQ_104MHZ  (0xa0)

#define MICRON_VCR_DUMMYCLK_MASK        (0xf0)

#define BURST_LENGTH_8					(0x0)
#define BURST_LENGTH_16					(0x01)
#define BURST_LENGTH_32					(0x02)
#define BURST_LENGTH_64					(0x03)
#define BURST_LENGTH_128				(0x04)
#define BURST_LENGTH_256				(0x05)

//#define SFC_WRITE_BUF_LEN               0x20
#define SFC_WRITE_BUF_LEN               0x100
#define SPIFLASH_PAGE_LEN               0x100
#define DWORD_LEN                       0x4
#define BLOCK_SIZE     s_flash_sector_size
#define BLOCK_MASK    (BLOCK_SIZE - 1)

//Flash type define
#define SPIFLASH_GD_ID         0xC8
#define SPIFLASH_EON_ID         0x1C
#define SPIFLASH_MICRON_ID      0x2C
#define SPIFLASH_PCT_ID         0xBF
#define SPIFLASH_MXIC_ID        0xC2
#define SPIFLASH_WINBOND_ID     0xEF
#define SPIFLASH_FIDELIX_ID     0xF8
#define SPIFLASH_ESMT_ID        0x8C
#define SPIFLASH_BG_ID        0xE0

#define UNSUPPORT_CMD  0xFF

#define FDL_INVALID_SIZE       0xFFFFFFFF

LOCAL uint32 dummyclk_freq[SFC_MAX_CS] = {0};	/* work mode: SPI mode or QPI mode*/

PUBLIC uint32  s_intel_m18_serial = 0;
PUBLIC uint32  g_flash_start_addr;
PUBLIC uint32  g_fdl_spiflash_disable_do_idle = FALSE;
PUBLIC uint32  g_fdl_spiflash_disable_qpi = FALSE;

LOCAL uint32 s_flash_init_flag = 0;
LOCAL FLASH_PTR_TYPE s_flash_start_ptr;

LOCAL uint32 s_flash_flash_size;
LOCAL uint32 s_flash_bank_num;
LOCAL uint32 s_flash_sector_size;
LOCAL uint32 s_flash_sector_num;
LOCAL uint32 s_flash_wb_len;
LOCAL uint32 s_flash_wb_flag;
LOCAL uint32 latest_erase_end_addr = 0;

typedef VOLATILE uint16 *FLASH_PTR_TYPE;

LOCAL BOOLEAN SPIFLASH_EraseSector(uint32 addr);
LOCAL BOOLEAN SPIFLASH_EraseBlock_32KB(uint32 addr);
LOCAL BOOLEAN SPIFLASH_EraseBlock_64KB(uint32 addr);
LOCAL void SPIFLASH_EnterQPI(int cs);
LOCAL void SPIFLASH_EnterQPIRead(int cs);
LOCAL void SPIFLASH_ResetSPIFlash(int cs);
LOCAL void SPIFLASH_Enter4ByteAddrMode(int cs);
LOCAL void SPIFLASH_Exit4ByteAddrMode(int cs);
LOCAL void SPIFLASH_SetReadPara(int cs, uint8 para);
LOCAL void SPIFLASH_Unlock(uint32   addr);
LOCAL void SPIFLASH_ResetRegister(int cs);

#define BLOCK_SIZE_64K      0x0010000UL
#define BLOCK_SIZE_32K      0x0008000UL
#define BLOCK_SIZE_4K       0x0001000UL

//running optr status
#define FLASH_STORE_OPTR_MAGIC          (0x4D534F46)    // FOSM
#define FLASH_STORE_OPTR_PROGRAMING (0x464F5350)    // FOSP
#define FLASH_STORE_OPTR_ERASING        (0x464F5345)    // FOSE
#define FLASH_STORE_OPTR_SUSPEND        (0x464F5353)    // FOSS
#define FLASH_STORE_OPTR_IDLE           (0x0)
#define FLASH_IRQ_FIQ_DISABLE  0xC0

#define EFS_SECTOR_GOOD_STATUS        (0x55aa)   // The sector is "GOOD"

//#define FLASH_WR(b,d)         *( (FLASH_PTR_TYPE)(b)) = (d)
#define FLASH_RD(b)           *( (FLASH_PTR_TYPE)(b))

#define FIRST_BYTE( _word )         (uint8)( (_word) >> 8 )
#define SECOND_BYTE( _word )        (uint8)( (_word) & 0xFF )
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
PUBLIC void SPIFLASH_SetCS0DesityFlag(uint8 MID, uint8 DID1, uint8 DID2, uint32 desity)
{
    flash_cs0_desity = desity;
    flash_cs0_MID = MID;
    flash_cs0_DID1 = DID1;
    flash_cs0_DID2 = DID2;
}

PUBLIC void SPIFLASH_SetCS1DesityFlag(uint8 MID, uint8 DID1, uint8 DID2, uint32 desity)
{
    flash_cs1_desity = desity;
    flash_cs1_MID = MID;
    flash_cs1_DID1 = DID1;
    flash_cs1_DID2 = DID2;
}

PUBLIC void FDL_Mark_SPIFlash_Type(void)
{
    SPIFLASH_TYPE_CONFIG_T* s_spiflash_type_cfg_ptr = NULL;
    uint32 len = 0;

    while(len < 0x11000)
    {
        s_spiflash_type_cfg_ptr = (SPIFLASH_TYPE_CONFIG_T*)(FLASH_START_ADRESS + len);

        if((s_spiflash_type_cfg_ptr->magic_first == SPIFLASH_MAGIC_FIRST) &&
            (s_spiflash_type_cfg_ptr->magic_second == SPIFLASH_MAGIC_SECOND) &&
            (s_spiflash_type_cfg_ptr->magic_end == SPIFLASH_MAGIC_END))
        {
                /* record spiflash type info in PS*/
                SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs0_desity), flash_cs0_desity & 0xFFFF);
                SPIFLASH_WriteWord((uint32)(&s_spiflash_type_cfg_ptr->cs0_desity) + sizeof(uint16), flash_cs0_desity >> 16);
                SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs0_MID), flash_cs0_MID);
                SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs0_MID) + sizeof(uint16), 0);
                SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs0_DID1), flash_cs0_DID1);
                SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs0_DID1) + sizeof(uint16), 0);
                SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs0_DID2), flash_cs0_DID2);
                SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs0_DID2) + sizeof(uint16), 0);

                SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs1_desity), flash_cs1_desity & 0xFFFF);
                SPIFLASH_WriteWord((uint32)(&s_spiflash_type_cfg_ptr->cs1_desity) + sizeof(uint16), flash_cs1_desity >> 16);
                SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs1_MID), flash_cs1_MID);
                SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs1_MID) + sizeof(uint16), 0);
                SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs1_DID1), flash_cs1_DID1);
                SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs1_DID1) + sizeof(uint16), 0);
                SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs1_DID2), flash_cs1_DID2);
                SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs1_DID2) + sizeof(uint16), 0);

                break;
        }

        len += 4;
    }

}

PUBLIC BOOLEAN Is_FlashID_Valid(uint8 MID, uint8 DID1, uint8 DID2)
{
    if((SPIFLASH_GD_ID== MID) || (SPIFLASH_EON_ID == MID) || (SPIFLASH_MICRON_ID == MID) ||
        (SPIFLASH_PCT_ID== MID) || (SPIFLASH_MXIC_ID == MID) || (SPIFLASH_WINBOND_ID == MID) ||
         (SPIFLASH_FIDELIX_ID== MID) || (SPIFLASH_ESMT_ID == MID)|| (SPIFLASH_BG_ID == MID))
    {
        return SCI_TRUE;
    }

    return SCI_FALSE;
}

PUBLIC void *_memcpy(void * dst, const void *src, uint32 count)
{
    char *tmp = (char *) dst, *s = (char *) src;

    while (count--)
        *tmp++ = *s++;

    return dst;
}

LOCAL uint32 SPIFLASH_GetPollStatus( int cs )
{
	uint32 status1 = 0, status2 = 0;
	uint32 status = 0;
	BIT_MODE_E bitmode = BIT_MODE_1;
	if(QPI_MODE == work_mode[cs])
	{
		bitmode = BIT_MODE_4;
	}

	/* clear cmd buffer and type buffer*/
	SFCDRV_TypeBufClr(cs);
	SFCDRV_CMDBufClr(cs);

	/* set write command */
	SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, bitmode, INI_CMD_BUF_7);

	/* write command 05, big endian, 1byte */

	SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_RDSR1);
	//SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

	/* read cmdbuf7, big endian, 1byte */
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_7, bitmode, BYTE_NUM_1, CMD_MODE_READ, SEND_MODE_0);

	/* clear int status */
	SFCDRV_IntClr(cs);
#if defined(CHIP_VER_UWS6121E)
	/* start soft request */
	SFCDRV_SoftReq(cs);

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
	/* start soft request */
	SFCDRV_SoftReq();

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif
	/* clear int status */
	SFCDRV_IntClr(cs);

	status1 = (SFCDRV_GetCMDBuf(cs, CMD_BUF_7)>>24);

	if(nQEPos[cs]>=8 && nQEPos[cs]!=UNSUPPORT_CMD)
	{
		/* clear cmd buffer and type buffer*/
		SFCDRV_TypeBufClr(cs);
		SFCDRV_CMDBufClr(cs);

		/* set write command */
		SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, bitmode, INI_CMD_BUF_7);

		/* write command 05, big endian, 1byte */
		SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_RDSR2);
		//SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

		/* read cmdbuf7, big endian, 1byte */
		SFCDRV_SetTypeInfBuf(cs, CMD_BUF_7, bitmode, BYTE_NUM_1, CMD_MODE_READ, SEND_MODE_0);

		/* clear int status */
		SFCDRV_IntClr(cs);
#if defined(CHIP_VER_UWS6121E)
		/* start soft request */
		SFCDRV_SoftReq(cs);

		while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
		/* start soft request */
		SFCDRV_SoftReq();

		while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
 #endif
		/* clear int status */
		SFCDRV_IntClr(cs);

		status2 = (SFCDRV_GetCMDBuf(cs, CMD_BUF_7)>>24);

	}

	status = (status1 | status2<<8);

    //SPIFLASH_ResetRegister(cs);

	return status;
}

LOCAL void SPIFLASH_WriteEnable(int cs)
{
	BIT_MODE_E bitmode = BIT_MODE_1;

	if(QPI_MODE == work_mode[cs])
	{
		bitmode = BIT_MODE_4;
	}

	  /* clear cmd buffer and type buffer*/
	SFCDRV_TypeBufClr(cs);
	SFCDRV_CMDBufClr(cs);

	/* set write command */
	SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, bitmode, INI_CMD_BUF_7);

	/* write command 06, big endian, 1byte */
	SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_WREN);
	//SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

        /* clear int status */
	SFCDRV_IntClr(cs);
#if defined(CHIP_VER_UWS6121E)
	/* start soft request */
	SFCDRV_SoftReq(cs);

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
	/* start soft request */
	SFCDRV_SoftReq();

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif
	/* clear int status */
	SFCDRV_IntClr(cs);
}

LOCAL void SPIFLASH_WriteStatusReg(int cs, uint32 value, BOOLEAN bSet)
{
	uint32 status = 0;
	uint32 reg_value = 0;
	BIT_MODE_E bitmode = BIT_MODE_1;

	if(QPI_MODE == work_mode[cs])
	{
		bitmode = BIT_MODE_4;
	}

	do
	{
	    SPIFLASH_WriteEnable(cs);
	    status = SPIFLASH_GetPollStatus(cs);
	}while(!(status & BIT_1));

	  /* clear cmd buffer and type buffer*/
	SFCDRV_TypeBufClr(cs);
	SFCDRV_CMDBufClr(cs);

	/* set write command */
	SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, bitmode, INI_CMD_BUF_7);

	/* write command 01, big endian, 1byte */
	SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_WRSR);
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

	if(bSet)
	{
	    reg_value = status | value;
	}
	else
	{
	    reg_value = ~value;
	    reg_value = status & reg_value;
	    reg_value |= BIT_1;
	}

	if(nQEPos[cs]>=8 && nQEPos[cs]!=UNSUPPORT_CMD)
	{
		SFCDRV_SetCMDBuf(cs, CMD_BUF_1, reg_value);
		SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, bitmode, BYTE_NUM_2, CMD_MODE_WRITE, SEND_MODE_0);
	}
	else
	{
		SFCDRV_SetCMDBuf(cs, CMD_BUF_1, reg_value);
		SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);
	}

	/* clear int status */
	SFCDRV_IntClr(cs);
#if defined(CHIP_VER_UWS6121E)
	/* start soft request */
	SFCDRV_SoftReq(cs);

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
	/* start soft request */
	SFCDRV_SoftReq();

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif
	/* clear int status */
	SFCDRV_IntClr(cs);

	do
	{
	    status = SPIFLASH_GetPollStatus(cs);
    }while(status & (1<<nWIPPos[cs]));

    SPIFLASH_ResetRegister(cs);

}

LOCAL void SPIFLASH_ResetStatusReg(int cs)
{
	uint32 status = 0;
	uint32 status_reg_val[3] = {0x00, 0x02, 0x60};
	uint32 cmd_write_reg[3] = {0x01, 0x31, 0x11};
	BIT_MODE_E bitmode = BIT_MODE_1;
	uint32 i = 0;

	if(QPI_MODE == work_mode[cs])
	{
		bitmode = BIT_MODE_4;
	}

	for(i = 0; i < 3; i++)
	{
		do
		{
		    SPIFLASH_WriteEnable(cs);
		    status = SPIFLASH_GetPollStatus(cs);
		}while(!(status & BIT_1));

		  /* clear cmd buffer and type buffer*/
		SFCDRV_TypeBufClr(cs);
		SFCDRV_CMDBufClr(cs);

		/* set write command */
		SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, bitmode, INI_CMD_BUF_7);

		/* write command 01, big endian, 1byte */
		SFCDRV_SetCMDBuf(cs, CMD_BUF_0, cmd_write_reg[i]);
		SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

		SFCDRV_SetCMDBuf(cs, CMD_BUF_1, status_reg_val[i]);
		SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

		/* clear int status */
		SFCDRV_IntClr(cs);
#if defined(CHIP_VER_UWS6121E)
		/* start soft request */
		SFCDRV_SoftReq(cs);

		while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
		/* start soft request */
		SFCDRV_SoftReq();

		while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif
		/* clear int status */
		SFCDRV_IntClr(cs);

		do
		{
		    status = SPIFLASH_GetPollStatus(cs);
	    }while(status & (1<<nWIPPos[cs]));
	}

    SPIFLASH_ResetRegister(cs);

}

LOCAL uint8 SPIFLASH_ReadReg_VCR(int cs)
{
    uint32 reg_value = 0;
    uint32 bitmode = 0;

    if(QPI_MODE == work_mode[cs])
    {
        bitmode = BIT_MODE_4;
    }

    SFCDRV_TypeBufClr(cs);
    SFCDRV_CMDBufClr(cs);

    SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, bitmode, INI_CMD_BUF_7);

    SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_RDVCR);
    SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

    SFCDRV_SetTypeInfBuf(cs, CMD_BUF_7, bitmode, BYTE_NUM_1, CMD_MODE_READ, SEND_MODE_0);

    SFCDRV_IntClr(cs);
#if defined(CHIP_VER_UWS6121E)
    SFCDRV_SoftReq(cs);

    while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
    SFCDRV_SoftReq();

    while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif

    SFCDRV_IntClr(cs);

    reg_value = SFCDRV_GetCMDBuf(cs, CMD_BUF_7);

    return (uint8)(reg_value >> 24);

}

LOCAL void SPIFLASH_WriteReg_VCR(int cs, uint8 reg_val)
{
    uint32 status = 0;
    uint32 bitmode = 0;

    if(QPI_MODE == work_mode[cs])
    {
        bitmode = BIT_MODE_4;
    }

    do
    {
        SPIFLASH_WriteEnable(cs);
        status = SPIFLASH_GetPollStatus(cs);
    }while(!(status & BIT_1));

    SFCDRV_TypeBufClr(cs);
    SFCDRV_CMDBufClr(cs);

    SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, bitmode, INI_CMD_BUF_7);

    SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_WRVCR);
    SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

    SFCDRV_SetCMDBuf(cs, CMD_BUF_1, reg_val);
    SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

    SFCDRV_IntClr(cs);
#if defined(CHIP_VER_UWS6121E)
    SFCDRV_SoftReq(cs);

    while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
    SFCDRV_SoftReq();

    while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif
    SFCDRV_IntClr(cs);

    do
    {
        status = SPIFLASH_GetPollStatus(cs);
    }while(status & (1<<nWIPPos[cs]));
}

LOCAL uint8 SPIFLASH_ReadReg_VECR(int cs)
{
    uint32 vecr_val = 0;
    uint32 bitmode = 0;

    if(QPI_MODE == work_mode[cs])
    {
        bitmode = BIT_MODE_4;
    }

    /* clear cmd buffer and type buffer*/
    SFCDRV_TypeBufClr(cs);
    SFCDRV_CMDBufClr(cs);

    /* set write command */
    SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, bitmode, INI_CMD_BUF_7);

    SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_RDVECR);
    SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

    SFCDRV_SetTypeInfBuf(cs, CMD_BUF_7, bitmode, BYTE_NUM_4, CMD_MODE_READ, SEND_MODE_0);

    /* clear int status */
    SFCDRV_IntClr(cs);
#if defined(CHIP_VER_UWS6121E)
    /* start soft request */
    SFCDRV_SoftReq(cs);

    while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
    /* start soft request */
    SFCDRV_SoftReq();

    while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif
    /* clear int status */
    SFCDRV_IntClr(cs);

    vecr_val = SFCDRV_GetCMDBuf(cs, CMD_BUF_7);

    return (uint8)(vecr_val >> 24);

}

LOCAL void SPIFLASH_WriteReg_VECR(int cs, uint8 reg_val)
{
    uint32 status = 0;
    uint32 bitmode = 0;

    if(QPI_MODE == work_mode[cs])
    {
        bitmode = BIT_MODE_4;
    }

    do
    {
        SPIFLASH_WriteEnable(cs);
        status = SPIFLASH_GetPollStatus(cs);
    }while(!(status & BIT_1));


    /* clear cmd buffer and type buffer*/
    SFCDRV_TypeBufClr(cs);
    SFCDRV_CMDBufClr(cs);

    /* set write command */
    SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, bitmode, INI_CMD_BUF_7);

    /* cmd buf 0: write cmd 0x61, big endian, 1byte, 1 bit mode */
    SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_WRVECR);
    SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

    SFCDRV_SetCMDBuf(cs, CMD_BUF_1, reg_val);
    SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

    /* clear int status */
    SFCDRV_IntClr(cs);
#if defined(CHIP_VER_UWS6121E)
    /* start soft request */
    SFCDRV_SoftReq(cs);

    while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
    /* start soft request */
    SFCDRV_SoftReq();

    while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif
    /* clear int status */
    SFCDRV_IntClr(cs);

    do
    {
        status = SPIFLASH_GetPollStatus(cs);
    }while(status & (1<<nWIPPos[cs]));
}

LOCAL void SPIFLASH_EnableQuadIO(int cs)
{
	if(nQEPos[cs] != UNSUPPORT_CMD)
	{
		SPIFLASH_WriteStatusReg(cs, 1<<nQEPos[cs], TRUE);
	}
	else
	{

	}
}

LOCAL void SPIFLASH_DisableQuadIO(int cs)
{
	if(nQEPos[cs] != UNSUPPORT_CMD)
	{
		SPIFLASH_WriteStatusReg(cs, 1<<nQEPos[cs], FALSE);
	}
	else
	{

	}
}

LOCAL void SPIFLASH_EnterSPIRead(int cs)
{
    /* clear cmd buffer and type buffer*/
    SFCDRV_TypeBufClr(cs);
    SFCDRV_CMDBufClr(cs);

    /* Set clock dividor to 0, running at (78/4)MHz */
    //SFCDRV_ClkCfg(cs, SFC_CLK_SAMPLE_DELAY_SEL<<9 | SFC_CLK_DIV_2);

    /* set AHB Read mode, 4 bit mode */
    SFCDRV_SetCMDCfgReg(cs, CMD_MODE_READ, BIT_MODE_1, INI_CMD_BUF_7);

    /* write cmd 0xEB, big endian, 1byte, 1 bit mode */
    SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_READ);
    SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, BIT_MODE_1, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);
    /* write address, big endian, 3byte, 1 bit mode */
    SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, BIT_MODE_1, s_address_cycle[cs], CMD_MODE_WRITE, SEND_MODE_1);

	//CMD_READ requires clock under 50MHz
#if !defined(PLATFORM_UWS6121E)
	SFCDRV_ClkCfg(cs, SFC_CLK_DIV_2);
#endif
}

LOCAL void SPIFLASH_EnterSPI4Read(int cs)
{
	/* clear cmd buffer and type buffer*/
	SFCDRV_TypeBufClr(cs);
	SFCDRV_CMDBufClr(cs);

    /* set AHB Read mode, 4 bit mode */
    SFCDRV_SetCMDCfgReg(cs, CMD_MODE_READ, BIT_MODE_4, INI_CMD_BUF_7);

	/* write cmd 0xEB, big endian, 1byte, 1 bit mode */
	SFCDRV_SetCMDBuf(cs, CMD_BUF_0, cmd_4read[cs]);
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, BIT_MODE_1, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

    if (cmd_4read[cs] == CMD_QUAD_FAST_READ)
    {
        SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, BIT_MODE_1, s_address_cycle[cs], CMD_MODE_WRITE, SEND_MODE_1);
    }
    else
    {
        SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, BIT_MODE_4, s_address_cycle[cs], CMD_MODE_WRITE, SEND_MODE_1);
    }

    if ((s_nMID[cs] == SPIFLASH_WINBOND_ID)
            || (s_nMID[cs] == SPIFLASH_FIDELIX_ID)
            || (s_nMID[cs] == SPIFLASH_GD_ID)
            || (s_nMID[cs] == SPIFLASH_BG_ID)
        )
    {
        /* write dummy clock */
        SFCDRV_SetTypeInfBuf(cs, CMD_BUF_2, BIT_MODE_4, BYTE_NUM_3, CMD_MODE_WRITE, SEND_MODE_0);
    }
    else
    {
        /* write dummy, big endian, 3byte, 4 bit mode */
        SFCDRV_SetTypeInfBuf(cs, CMD_BUF_2, BIT_MODE_4, BYTE_NUM_4, CMD_MODE_WRITE, SEND_MODE_0);
    }

	if(s_nMID[cs] == SPIFLASH_MICRON_ID)
	{
		/* write dummy, big endian, 2byte, 4 bit mode */
		SFCDRV_SetTypeInfBuf(cs, CMD_BUF_3, BIT_MODE_4, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);
	}

#if defined(PLATFORM_UWS6121E)
    FLASH_DELAY_US(30);
#else
    FLASH_DELAY_US(30)
#endif
}


LOCAL void SPIFLASH_EnterQPI(int cs)
{
    if(s_nMID[cs] == SPIFLASH_MICRON_ID)
    {
        uint8 vecr_val = 0;
        vecr_val = SPIFLASH_ReadReg_VECR(cs);
        SPIFLASH_WriteReg_VECR(cs, vecr_val & ~BIT_7);
        return;
    }

	SPIFLASH_EnableQuadIO(cs);

	/* clear cmd buffer and type buffer*/
	SFCDRV_TypeBufClr(cs);
	SFCDRV_CMDBufClr(cs);

	/* set write command */
	SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, BIT_MODE_1, INI_CMD_BUF_7);

	/* cmd buf 0: write cmd EnterQPI, big endian, 1byte, 1 bit mode */
	SFCDRV_SetCMDBuf(cs, CMD_BUF_0, EnterQPI[cs]);
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, BIT_MODE_1, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

	/* clear int status */
	SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
	/* start soft request */
	SFCDRV_SoftReq(cs);

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
	/* start soft request */
	SFCDRV_SoftReq();

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif
	/* clear int status */
	SFCDRV_IntClr(cs);
}


LOCAL void SPIFLASH_ExitQPI(int cs)
{
    if(s_nMID[cs] == SPIFLASH_MICRON_ID)
    {
        uint8 vecr_val = 0;
        vecr_val = SPIFLASH_ReadReg_VECR(cs);
        SPIFLASH_WriteReg_VECR(cs, vecr_val | BIT_7);
        //work_mode = SPI_MODE;
        return;
    }

	SFCDRV_TypeBufClr(cs);
	SFCDRV_CMDBufClr(cs);

	/* set write command */
	SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, BIT_MODE_1, INI_CMD_BUF_7);

	/* cmd buf 0: write cmd ExitQPI, big endian, 1byte, 4 bit mode */
	SFCDRV_SetCMDBuf(cs, CMD_BUF_0, ExitQPI[cs]);
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, BIT_MODE_4, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

	/* clear int status */
	SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
	/* start soft request */
	SFCDRV_SoftReq(cs);

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
	/* start soft request */
	SFCDRV_SoftReq();

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif
	/* clear int status */
	SFCDRV_IntClr(cs);
}

/*****************************************************************************/
// Description :    This function is used to enter 4-Byte Address Mode
// Global resource dependence :
// Author :         yuhua.shi
// Note :
/*****************************************************************************/
LOCAL void SPIFLASH_Enter4ByteAddrMode(int cs)
{
	BIT_MODE_E bitmode = BIT_MODE_1;

	if(QPI_MODE == work_mode[cs])
	{
		bitmode = BIT_MODE_4;
	}

	/* clear cmd buffer and type buffer*/
	SFCDRV_TypeBufClr(cs);
	SFCDRV_CMDBufClr(cs);

	/* set AHB Write mode, 4 bit mode */
	SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, BIT_MODE_1, INI_CMD_BUF_7);

	/* write cmd CMD_SETBURST, big endian, 1byte, 4 bit mode */
	SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_4BYTEADDREN);
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

 	/* clear int status */
	SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
	/* start soft request */
	SFCDRV_SoftReq(cs);

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
	/* start soft request */
	SFCDRV_SoftReq();

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif
	/* clear int status */
	SFCDRV_IntClr(cs);
}

/*****************************************************************************/
// Description :    This function is used to exit 4-Byte Address Mode
// Global resource dependence :
// Author :         yuhua.shi
// Note :
/*****************************************************************************/
LOCAL void SPIFLASH_Exit4ByteAddrMode(int cs)
{
	BIT_MODE_E bitmode = BIT_MODE_1;

	if(QPI_MODE == work_mode[cs])
	{
		bitmode = BIT_MODE_4;
	}

	/* clear cmd buffer and type buffer*/
	SFCDRV_TypeBufClr(cs);
	SFCDRV_CMDBufClr(cs);

	/* set AHB Write mode, 4 bit mode */
	SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, BIT_MODE_1, INI_CMD_BUF_7);

	/* write cmd CMD_SETBURST, big endian, 1byte, 4 bit mode */
	SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_4BYTEADDRDI);
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

 	/* clear int status */
	SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
	/* start soft request */
	SFCDRV_SoftReq(cs);

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
	/* start soft request */
	SFCDRV_SoftReq();

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif

	/* clear int status */
	SFCDRV_IntClr(cs);
}

LOCAL void SPIFLASH_SetReadPara(int cs, uint8 para)
{
	BIT_MODE_E bitmode = BIT_MODE_1;

	if(QPI_MODE == work_mode[cs])
	{
		bitmode = BIT_MODE_4;
	}
	else
	{
		return;
	}

	dummyclk_freq[cs] = para;

	/* clear cmd buffer and type buffer*/
	SFCDRV_TypeBufClr(cs);
	SFCDRV_CMDBufClr(cs);

	/* set AHB Write mode, 4 bit mode */
	SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, BIT_MODE_1, INI_CMD_BUF_7);

	/* write cmd CMD_SETBURST, big endian, 1byte, 4 bit mode */
	SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_SETBURST);
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

	/* write value, big endian, 1byte */
	SFCDRV_SetCMDBuf(cs, CMD_BUF_1, para);	//0xE0:6 dummy clock when QPI Read
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_1);

 	/* clear int status */
	SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
	/* start soft request */
	SFCDRV_SoftReq(cs);

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
	/* start soft request */
	SFCDRV_SoftReq();

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif

	/* clear int status */
	SFCDRV_IntClr(cs);
}


LOCAL void SPIFLASH_EnterQPIRead(int cs)
{
	BYTE_NUM_E bytenum = BYTE_NUM_3;

	SFCDRV_TypeBufClr(cs);
	SFCDRV_CMDBufClr(cs);

	SFCDRV_SetCMDCfgReg(cs, CMD_MODE_READ, BIT_MODE_4, INI_CMD_BUF_7);

	SFCDRV_SetCMDBuf(cs, CMD_BUF_0, cmd_4read[cs]);
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, BIT_MODE_4, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, BIT_MODE_4, s_address_cycle[cs], CMD_MODE_WRITE, SEND_MODE_1);

	if(s_nMID[cs] == SPIFLASH_WINBOND_ID)
	{
		switch(dummyclk_freq[cs])
		{
			case WINBOND_DUMMYCLK_2_FREQ_50MHZ:
				bytenum = BYTE_NUM_1;
			break;

			case WINBOND_DUMMYCLK_4_FREQ_80MHZ:
				bytenum = BYTE_NUM_2;
			break;

			case WINBOND_DUMMYCLK_6_FREQ_104MHZ:
				bytenum = BYTE_NUM_3;
			break;

			case WINBOND_DUMMYCLK_8_FREQ_104MHZ:
				bytenum = BYTE_NUM_4;
			break;

			default:
				bytenum = BYTE_NUM_3;
			break;
		}
	}
    else if(s_nMID[cs] == SPIFLASH_GD_ID)
    {
        switch(dummyclk_freq[cs])
        {
            case GD_DUMMYCLK_4_FREQ_80MHZ:
                bytenum = BYTE_NUM_2;
            break;

            case GD_DUMMYCLK_6_FREQ_104MHZ:
                if ((s_nDID2[cs] == 0x17)
                        || (s_nDID2[cs] == 0x16)
                        || (s_nDID2[cs] == 0x15)
                    )
                {
                    bytenum = BYTE_NUM_2;
                }
                else
                {
                    bytenum = BYTE_NUM_3;
                }
            break;

            default:
                bytenum = BYTE_NUM_3;
            break;
        }
    }
    else if(s_nMID[cs] == SPIFLASH_BG_ID)
    {
        switch(dummyclk_freq[cs])
        {
            case GD_DUMMYCLK_4_FREQ_80MHZ:
                bytenum = BYTE_NUM_2;
            break;

            case GD_DUMMYCLK_6_FREQ_104MHZ:
                if (s_nDID2[cs] == 0x18)
                {
                    bytenum = BYTE_NUM_2;
                }
                else
                {
                    bytenum = BYTE_NUM_3;
                }
            break;

            default:
                bytenum = BYTE_NUM_3;
            break;
        }
    }
    else if(s_nMID[cs] == SPIFLASH_PCT_ID)
    {
        bytenum = BYTE_NUM_2;
    }
    else if((s_nMID[cs] == SPIFLASH_FIDELIX_ID))
    {
        if (s_nDID1[cs] == 0x42)
        {
            switch(dummyclk_freq[cs])
            {
                case FM_DUMMYCLK_4_FREQ_80MHZ:
                    bytenum = BYTE_NUM_2;
                    break;

                case FM_DUMMYCLK_6_FREQ_104MHZ:
                    bytenum = BYTE_NUM_3;
                    break;

                default:
                    bytenum = BYTE_NUM_3;
                    break;
            }
        }
        else if (s_nDID1[cs]  == 0x43 && s_nDID2[cs]  == 0x17)
        {
            bytenum = BYTE_NUM_3;
        }
    }
    else if(s_nMID[cs] == SPIFLASH_MICRON_ID)
    {
        SFCDRV_SetTypeInfBuf(cs, CMD_BUF_3, BIT_MODE_4, BYTE_NUM_2, CMD_MODE_WRITE, SEND_MODE_0);
    }

    SFCDRV_SetTypeInfBuf(cs, CMD_BUF_2, BIT_MODE_4, bytenum, CMD_MODE_WRITE, SEND_MODE_0);
#if defined(PLATFORM_UWS6121E)
    FLASH_DELAY_US(30);
#else
    FLASH_DELAY_US(30)
#endif
}


LOCAL void SPIFLASH_ResetRegister(int cs)
{
	if(QPI_MODE == work_mode[cs])
	{
		SPIFLASH_EnterQPIRead(cs);
	}
	else
	{
		SPIFLASH_EnterSPI4Read(cs);
	}

}


LOCAL void SPIFLASH_ResetSPIFlash(int cs)
{
  	/* clear cmd buffer and type buffer*/
    SFCDRV_TypeBufClr(cs);
	SFCDRV_CMDBufClr(cs);

	/* set AHB Read mode, 4 bit mode */
	SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, BIT_MODE_1, INI_CMD_BUF_7);

	/* write cmd CMD_RSTEN, big endian, 1byte, 1 bit mode */
	SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_RSTEN);
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, BIT_MODE_1, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

 	/* clear int status */
	SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
	/* start soft request */
	SFCDRV_SoftReq(cs);

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
	/* start soft request */
	SFCDRV_SoftReq();

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif

	/* clear int status */
	SFCDRV_IntClr(cs);

  	/* clear cmd buffer and type buffer*/
       SFCDRV_TypeBufClr(cs);
	SFCDRV_CMDBufClr(cs);

	/* set AHB Read mode, 4 bit mode */
	SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, BIT_MODE_1, INI_CMD_BUF_7);

	/* write cmd CMD_RST, big endian, 1byte, 1 bit mode */
	SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_RST);
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, BIT_MODE_1, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

 	/* clear int status */
	SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
	/* start soft request */
	SFCDRV_SoftReq(cs);

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
	/* start soft request */
	SFCDRV_SoftReq();

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif

	/* clear int status */
	SFCDRV_IntClr(cs);

    TIMER_DELAY_US(30);    /*need delay 30us after CMD_RST*/
}

PUBLIC void SPIFLASH_ResetQPIFlash(int cs)
{
  	/* clear cmd buffer and type buffer*/
	SFCDRV_TypeBufClr(cs);
	SFCDRV_CMDBufClr(cs);

	/* set AHB Read mode, 4 bit mode */
	SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, BIT_MODE_1, INI_CMD_BUF_7);

	/* write cmd CMD_RSTEN, big endian, 1byte, 1 bit mode */
	SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_RSTEN);
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, BIT_MODE_4, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

 	/* clear int status */
	SFCDRV_IntClr(cs);

	/* start soft request */
#if defined(CHIP_VER_UWS6121E)
	SFCDRV_SoftReq(cs);
	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
	SFCDRV_SoftReq();
	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif
	/* clear int status */
	SFCDRV_IntClr(cs);


  	/* clear cmd buffer and type buffer*/
	SFCDRV_TypeBufClr(cs);
	SFCDRV_CMDBufClr(cs);

	/* set AHB Read mode, 4 bit mode */
	SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, BIT_MODE_1, INI_CMD_BUF_7);

	/* write cmd CMD_RST, big endian, 1byte, 1 bit mode */
	SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_RST);
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, BIT_MODE_4, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

 	/* clear int status */
	SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
	/* start soft request */
	SFCDRV_SoftReq(cs);

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
	/* start soft request */
	SFCDRV_SoftReq();

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif

	/* clear int status */
	SFCDRV_IntClr(cs);

    TIMER_DELAY_US(30);    /*need delay 30us after CMD_RST*/
	work_mode[cs] = SPI_MODE;

	SPIFLASH_EnterSPIRead(cs);
}

/*****************************************************************************/
// Description :    This function is used to init MX flash
// Global resource dependence :
// Author :         Nick.Zhao
// Note :
/*****************************************************************************/
PUBLIC int SPIFLASH_Init(int cs)  /* If Init success, return true, else return false */
{
    uint8   nMID, nDID1, nDID2;
    uint8  i;
    uint32 reg_val;
    SPIFLASH_SPEC_T *pSpiFlashSpec = NULL;

    s_flash_init_flag = 1;

    for(i=0; i<100; i++);

    SPIFLASH_ResetSPIFlash(cs);
    SPIFLASH_ResetQPIFlash(cs);
    work_mode[cs] = SPI_MODE;

    if (SPIFLASH_ReadID(cs, &nMID, &nDID1, &nDID2) != TRUE)
    {
        return SCI_FALSE;
    }

    pSpiFlashSpec = SPIFLASH_SPEC_Get(nMID, nDID1, nDID2);
    if(pSpiFlashSpec == NULL)
    {
        return SCI_FALSE;
    }

    nQEPos[cs] = pSpiFlashSpec->nQEPos;
    nWIPPos[cs] = pSpiFlashSpec->nWIPPos;
    EnterQPI[cs] = pSpiFlashSpec->EnterQPI;
    ExitQPI[cs] = pSpiFlashSpec->ExitQPI;
    cmd_4pp[cs] = pSpiFlashSpec->write_4pp;
    cmd_4read[cs] = pSpiFlashSpec->cmd_4read;
    reset_support[cs] = pSpiFlashSpec->reset_support;
    QPI_support[cs]	 = pSpiFlashSpec->QPI_support;
    s_nMID[cs] = pSpiFlashSpec->nMID;
    s_nDID1[cs] = pSpiFlashSpec->nDID1;
    s_nDID2[cs] = pSpiFlashSpec->nDID2;
    CMD_PES[cs] = pSpiFlashSpec->CMD_PES;
    CMD_PER[cs] = pSpiFlashSpec->CMD_PER;
    pExtcfg[cs] = pSpiFlashSpec->pExtcfg;

    s_flash_sector_size = pSpiFlashSpec->spiflash_cfg->sect_size;
    s_flash_flash_size = pSpiFlashSpec->spiflash_cfg->flash_size;

    if(pSpiFlashSpec->pExtcfg != SCI_NULL)
    {
        struct SPIFLASH_ExtCfg* cfg = (struct SPIFLASH_ExtCfg*)(pSpiFlashSpec->pExtcfg);

        if(cfg->voltage == SPIFLASH_VOLTAGE_3V)
        {
#if !defined(PLATFORM_UWS6121E)
            REG32(PIN_ESMCSN0_REG) |= BIT_10;
            REG32(PIN_ESMD0_REG) |= BIT_10;
            REG32(PIN_ESMD1_REG) |= BIT_10;
            REG32(PIN_ESMD2_REG) |= BIT_10;
            REG32(PIN_ESMD3_REG) |= BIT_10;
            REG32(PIN_ESMCLK_REG) |= BIT_10;
#else
#if defined(CHIP_VER_UIX8910MPW)
            REG32(REG_IOMUX_PAD_SPI_FLASH_CS0_CFG_REG) |= IOMUX_PAD_SPI_FLASH_CS0_DRV_STRENGTH(1);
            REG32(REG_IOMUX_PAD_SPI_FLASH_SIO_0_CFG_REG) |= IOMUX_PAD_SPI_FLASH_SIO_0_DRV_STRENGTH(1);
            REG32(REG_IOMUX_PAD_SPI_FLASH_SIO_1_CFG_REG) |= IOMUX_PAD_SPI_FLASH_SIO_1_DRV_STRENGTH(1);
            REG32(REG_IOMUX_PAD_SPI_FLASH_SIO_2_CFG_REG) |= IOMUX_PAD_SPI_FLASH_SIO_2_DRV_STRENGTH(1);
            REG32(REG_IOMUX_PAD_SPI_FLASH_SIO_3_CFG_REG) |= IOMUX_PAD_SPI_FLASH_SIO_3_DRV_STRENGTH(1);
            REG32(REG_IOMUX_PAD_SPI_FLASH_CLK_CFG_REG) |= IOMUX_PAD_SPI_FLASH_CLK_DRV_STRENGTH(1);
#endif
#if defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E)
            REG32(REG_IOMUX_PAD_SPI_FLASH_CS_CFG_REG) |= IOMUX_PAD_SPI_FLASH_CS_DRV_STRENGTH(1);
            REG32(REG_IOMUX_PAD_SPI_FLASH_SIO_0_CFG_REG) |= IOMUX_PAD_SPI_FLASH_SIO_0_DRV_STRENGTH(1);
            REG32(REG_IOMUX_PAD_SPI_FLASH_SIO_1_CFG_REG) |= IOMUX_PAD_SPI_FLASH_SIO_1_DRV_STRENGTH(1);
            REG32(REG_IOMUX_PAD_SPI_FLASH_SIO_2_CFG_REG) |= IOMUX_PAD_SPI_FLASH_SIO_2_DRV_STRENGTH(1);
            REG32(REG_IOMUX_PAD_SPI_FLASH_SIO_3_CFG_REG) |= IOMUX_PAD_SPI_FLASH_SIO_3_DRV_STRENGTH(1);
            REG32(REG_IOMUX_PAD_SPI_FLASH_CLK_CFG_REG) |= IOMUX_PAD_SPI_FLASH_CLK_DRV_STRENGTH(1);
#endif
#endif
        }

        if(cfg->desity > SPIFLASH_3_ADDR_CYCLE_MAX_REGION)
        {
            //s_address_cycle[cs] = cfg->address_cycle;
            s_address_cycle[cs] = BYTE_NUM_4;
            s_address_mask[cs] = SPIFLASH_4_ADDR_CYCLE_MASK;
            SPIFLASH_Enter4ByteAddrMode(cs);
        }
    }

    //ATE test causes winbond flash been locked, so we do flash reset here during flash initialization
    if(nMID == SPIFLASH_WINBOND_ID)
    {
        SPIFLASH_ResetStatusReg(cs);
    }

#if !defined(PLATFORM_UWS6121E)
    SFCDRV_ClkCfg(cs, SFC_CLK_DIV_0 | SFC_CLK_2X_EN | SFC_CLK_OUT_2X_EN | SFC_CLK_SAMPLE_2X_PHASE | SFC_CLK_SAMPLE_2X_EN);
#else
    SFCDRV_ClkCfg(cs, (2<<4) | (2<<8));
#endif

    //FDL_SendTrace("status register 0x%x", SPIFLASH_GetPollStatus(cs));
    SPIFLASH_WriteStatusReg(cs, 0xFFFF, FALSE);	//clear all status bits to unlock

    if(QPI_support[cs] && (!g_fdl_spiflash_disable_qpi))
    {
        SPIFLASH_EnterQPI(cs);
        work_mode[cs] = QPI_MODE;

        if (nMID == SPIFLASH_PCT_ID)
        {
            SPIFLASH_Unlock(0);
        }

        if(nMID == SPIFLASH_WINBOND_ID)
        {
            SPIFLASH_SetReadPara(cs, WINBOND_DUMMYCLK_6_FREQ_104MHZ);
        }
        else if((nMID == SPIFLASH_GD_ID) || (nMID == SPIFLASH_BG_ID))
        {
            if (0x16 == nDID2)//LQ32M
			{
				SPIFLASH_SetReadPara(cs, GD_LQ32M_DUMMYCLK_6_FREQ_104MHZ);
           	}
			else
			{
				SPIFLASH_SetReadPara(cs, GD_DUMMYCLK_6_FREQ_104MHZ);
			}
        }
        else if((nMID == SPIFLASH_FIDELIX_ID))
        {
            if (nDID1 == 0x42)
            {
                SPIFLASH_SetReadPara(cs, FM_25M_DUMMYCLK_6_FREQ_104MHZ);
            }
            else if (nDID1 == 0x43 && nDID2 == 0x17)
            {
                SPIFLASH_SetReadPara(cs, FM_25M_DUMMYCLK_6_FREQ_104MHZ);
            }
        }
        else if(nMID == SPIFLASH_MICRON_ID)
        {
            reg_val = SPIFLASH_ReadReg_VCR(cs);
            reg_val &= ~MICRON_VCR_DUMMYCLK_MASK;
            reg_val |= MICRON_DUMMYCLK_10_FREQ_104MHZ;
            SPIFLASH_WriteReg_VCR(cs, reg_val);
        }
	}
    else
    {
        SPIFLASH_EnableQuadIO(cs);
    }

    SPIFLASH_ResetRegister(cs);

    return SCI_TRUE;
}

/*****************************************************************************/
// Description :    This function is used to close MX flash
// Global resource dependence :
// Author :         Nick.Zhao
// Note :           it is called when power down
/*****************************************************************************/
LOCAL void SPIFLASH_Close(void)
{
    s_flash_init_flag = 0;
}

/*****************************************************************************/
// Description :    This function is used to read manufacturer ID and device ID
// Global resource dependence :
// Author :         Fei.Zhang
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN SPIFLASH_ReadID(int cs, uint8 *pMID, uint8 *pDID1, uint8 *pDID2)
{
	uint32 id = 0;

	SFCDRV_CSCfg(cs);

	/* clear cmd buffer and type buffer*/
	SFCDRV_TypeBufClr(cs);
	SFCDRV_CMDBufClr(cs);

	/* set write command */
	SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, BIT_MODE_1, INI_CMD_BUF_7);

	/* cmd buf 0: write cmd 0x9F, big endian, 1byte, 1 bit mode */
	SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_RDID);
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, BIT_MODE_1, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);
	/* cmd buf 7: read, big endian, 3byte ID(1byte manufacturer ID + 2byte device ID), 1 bit mode */
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_7, BIT_MODE_1, BYTE_NUM_3, CMD_MODE_READ, SEND_MODE_0);

	/* clear int status */
	SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
	/* start soft request */
	SFCDRV_SoftReq(cs);

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
	/* start soft request */
	SFCDRV_SoftReq();

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif

	/* clear int status */
	SFCDRV_IntClr(cs);

	id = SFCDRV_GetCMDBuf(cs, CMD_BUF_7);
#if !defined(PLATFORM_UWS6121E)
    *pMID = (id&0xFF000000)>>24;
    *pDID1 = (id&0x00FF0000)>>16;
    *pDID2 = (id&0x0000FF00)>>8;
#else
    *pDID2 = (id&0xFF000000)>>24;
    *pDID1 = (id&0x00FF0000)>>16;
    *pMID = (id&0x0000FF00)>>8;
#endif

	SPIFLASH_EnterSPIRead(cs);

	return SCI_TRUE;
}

/*****************************************************************************/
// Description :    write function
// Global resource dependence :
//
// Author :         henry.he
// Parameter:
// Note :
/*****************************************************************************/
LOCAL BOOLEAN SPIFLASH_WriteByte(    // If the data being written to flash, return TRUE.
    uint32       addr,              // the start address that will be written.
    const uint8  * buf,             // Pointer to the buffer containing the data to be written.
    uint32       counter)           // number of byte to write.
{

	uint32              dest_addr;
	uint32 status;
	BYTE_NUM_E bytenum = BYTE_NUM_1;
	BIT_MODE_E bitmode = BIT_MODE_1;
	int cs = SPIFLASH_CS0;

#if defined(PLATFORM_SC6530) || defined(CHIP_VER_UWS6121E)
    if((addr >= SPIFLASH_CS1_BASE_ADDRESS) && (s_nMID[SPIFLASH_CS1] > 0))
    {
        cs = SPIFLASH_CS1;
        addr -= SPIFLASH_CS1_BASE_ADDRESS;
    }
#endif

    SFCDRV_CSCfg(cs);

    if(QPI_MODE == work_mode[cs])
    {
        bitmode = BIT_MODE_4;
    }

    SCI_ASSERT(counter <= 4);/*assert verified*/

    do
    {
        SPIFLASH_WriteEnable(cs);
        status = SPIFLASH_GetPollStatus(cs);
    }while(!(status & BIT_1));

      /* clear cmd buffer and type buffer*/
    SFCDRV_TypeBufClr(cs);
    SFCDRV_CMDBufClr(cs);

    /* set write command */
    SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, bitmode, INI_CMD_BUF_7);

    if((!QPI_support[cs]) || g_fdl_spiflash_disable_qpi)
    {
        /* cmd buf 0: write cmd cmd_4pp, big endian, 1byte, 1 bit mode */
        SFCDRV_SetCMDBuf(cs, CMD_BUF_0, cmd_4pp[cs]);
        //SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

        if(cmd_4pp[cs] == CMD_4PP)
        {
            SFCDRV_SetCMDBuf(cs, CMD_BUF_1, (addr & s_address_mask[cs]));
            SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, bitmode, s_address_cycle[cs], CMD_MODE_WRITE, SEND_MODE_1);

            bitmode = BIT_MODE_4;
        }
        else
        {
            bitmode = BIT_MODE_4;

            SFCDRV_SetCMDBuf(cs, CMD_BUF_1, (addr & s_address_mask[cs]));
            SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, bitmode, s_address_cycle[cs], CMD_MODE_WRITE, SEND_MODE_1);
        }
    }
    else
    {
        /* cmd buf 0: write cmd 0x02, big endian, 1byte, 1 bit mode */
        SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_PP);
        //SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

        SFCDRV_SetCMDBuf(cs, CMD_BUF_1, (addr & s_address_mask[cs]));
        SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, bitmode, s_address_cycle[cs], CMD_MODE_WRITE, SEND_MODE_1);

    }

    SFCDRV_SetCMDBufEx(cs, CMD_BUF_2, buf, counter);

    /* clear int status */
    SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
    /* start soft request */
    SFCDRV_SoftReq(cs);

    while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
    /* start soft request */
    SFCDRV_SoftReq();

    while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif

    /* clear int status */
    SFCDRV_IntClr(cs);

    do
    {
        status = SPIFLASH_GetPollStatus(cs);
        if(!g_fdl_spiflash_disable_do_idle)
            FLASH_DO_IDLE
    }while(status & (1<<nWIPPos[cs]));

    SPIFLASH_ResetRegister(cs);

    return SCI_TRUE;
}

/*****************************************************************************/
// Description :    buffer write function
// Global resource dependence :
//
// Author :         Younger.yang
// Parameter:
// Note :
/*****************************************************************************/
LOCAL BOOLEAN SPIFLASH_WriteBuffer(    // If the data being written to flash, return TRUE.
    uint32       addr,              // the start address that will be written.
    const uint8  * buf,             // Pointer to the buffer containing the data to be written.
    uint32       counter)           // number of byte to write.
{

	uint32              dest_addr, status;
	BIT_MODE_E bitmode = BIT_MODE_1;
	int cs = SPIFLASH_CS0;

#if defined(PLATFORM_SC6530) || defined(CHIP_VER_UWS6121E)
	if((addr >= SPIFLASH_CS1_BASE_ADDRESS) && (s_nMID[SPIFLASH_CS1] > 0))
	{
		cs = SPIFLASH_CS1;
		addr -= SPIFLASH_CS1_BASE_ADDRESS;
	}
	#endif

    SFCDRV_CSCfg(cs);

    if(QPI_MODE == work_mode[cs])
    {
        bitmode = BIT_MODE_4;
    }

    //SCI_ASSERT(!(addr    % 4));/*assert verified*/
    //SCI_ASSERT(!(counter % 4));/*assert verified*/
    //SCI_ASSERT(counter <= 40);/*assert verified*/

    do
    {
        SPIFLASH_WriteEnable(cs);
        status = SPIFLASH_GetPollStatus(cs);
    }while(!(status & BIT_1));

    /* clear cmd buffer and type buffer*/
    SFCDRV_TypeBufClr(cs);
    SFCDRV_CMDBufClr(cs);

    /* set write command */
    SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, bitmode, INI_CMD_BUF_7);
    if((!QPI_support[cs]) || g_fdl_spiflash_disable_qpi)
    {
        /* cmd buf 0: write cmd cmd_4pp, big endian, 1byte, 1 bit mode */
        SFCDRV_SetCMDBuf(cs, CMD_BUF_0, cmd_4pp[cs]);

        if(cmd_4pp[cs] == CMD_4PP)
    	{
        	SFCDRV_SetCMDBuf(cs, CMD_BUF_1, (addr & s_address_mask[cs]));
        	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, bitmode, s_address_cycle[cs], CMD_MODE_WRITE, SEND_MODE_1);

            bitmode = BIT_MODE_4;
    	}
    	else
    	{
    		bitmode = BIT_MODE_4;

        	SFCDRV_SetCMDBuf(cs, CMD_BUF_1, (addr & s_address_mask[cs]));
        	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, bitmode, s_address_cycle[cs], CMD_MODE_WRITE, SEND_MODE_1);
        }
    }
    else
    {
        /* cmd buf 0: write cmd 0x02, big endian, 1byte, 1 bit mode */
        SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_PP);

    	SFCDRV_SetCMDBuf(cs, CMD_BUF_1, (addr & s_address_mask[cs]));
    	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, bitmode, s_address_cycle[cs], CMD_MODE_WRITE, SEND_MODE_1);
    }

    /* cmd buf 2: write value, big endian, n byte, 1 bit mode */
    SFCDRV_SetCMDBufEx(cs, CMD_BUF_2, buf, counter);

    /* clear int status */
    SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
    /* start soft request */
    SFCDRV_SoftReq(cs);

    while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
    /* start soft request */
    SFCDRV_SoftReq();

    while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif

    /* clear int status */
    SFCDRV_IntClr(cs);

    do
    {
        status = SPIFLASH_GetPollStatus(cs);
        if(!g_fdl_spiflash_disable_do_idle)
            FLASH_DO_IDLE
    }while(status & (1<<nWIPPos[cs]));

    //SPIFLASH_ResetRegister(cs);
    return SCI_TRUE;

}


/*****************************************************************************/
// Description :    page write function
// Global resource dependence :
//
// Author :         Yuhua.Shi
// Parameter:
// Note :
/*****************************************************************************/
LOCAL BOOLEAN SPIFLASH_WritePage(    // If the data being written to flash, return TRUE.
    uint32       addr,              // the start address that will be written.
    const uint8  * buf,             // Pointer to the buffer containing the data to be written.
    uint32       counter)           // number of byte to write.  must be SPIFLASH_PAGE_LEN in 8910
{
    uint8                data_write_type;
    uint32              status=0, status1=0, status2=0;
    BIT_MODE_E bitmode = BIT_MODE_1;
    int cs = SPIFLASH_CS0;

#if defined(PLATFORM_SC6530) || defined(CHIP_VER_UWS6121E)
    if((addr >= SPIFLASH_CS1_BASE_ADDRESS) && (s_nMID[SPIFLASH_CS1] > 0))
    {
        cs = SPIFLASH_CS1;
        addr -= SPIFLASH_CS1_BASE_ADDRESS;
    }
	#endif

    SFCDRV_CSCfg(cs);

    if(QPI_MODE == work_mode[cs])
    {
        bitmode = BIT_MODE_4;
    }

    SCI_ASSERT(!(addr    % 4));/*assert verified*/
    SCI_ASSERT(counter == SPIFLASH_PAGE_LEN);/*assert verified*/

    /* set write command */
    SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, bitmode, INI_CMD_BUF_7);

    /* clear cmd buffer and type buffer*/
    SFCDRV_TypeBufClr(cs);
    SFCDRV_CMDBufClr(cs);

    do
    {
        //write enable
        /* clear cmd buffer and type buffer*/
        SFCDRV_TypeBufClr(cs);

        /* write command 06, big endian, 1byte */
        SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_WREN);
        //SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

        /* clear int status */
        SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
        /* start soft request */
        SFCDRV_SoftReq(cs);

        while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
        /* start soft request */
        SFCDRV_SoftReq();

        while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif

        //poll status
        /* write command 05, big endian, 1byte */
        SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_RDSR1);

        /* read cmdbuf7, big endian, 1byte */
        SFCDRV_SetTypeInfBuf(cs, CMD_BUF_7, bitmode, BYTE_NUM_1, CMD_MODE_READ, SEND_MODE_0);

        /* clear int status */
        SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
        /* start soft request */
        SFCDRV_SoftReq(cs);

        while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
        /* start soft request */
        SFCDRV_SoftReq();

        while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif

        status1 = (SFCDRV_GetCMDBuf(cs, CMD_BUF_7)>>24);

        if(nQEPos[cs]>=8 && nQEPos[cs]!=UNSUPPORT_CMD)
        {
            /* write command 05, big endian, 1byte */
            SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_RDSR2);

            /* clear int status */
            SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
            /* start soft request */
            SFCDRV_SoftReq(cs);

            while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
            /* start soft request */
            SFCDRV_SoftReq();

            while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif

            status2 = (SFCDRV_GetCMDBuf(cs, CMD_BUF_7)>>24);
        }

        status = (status1 | status2<<8);
    }while(!(status & BIT_1));

    if((!QPI_support[cs])  || g_fdl_spiflash_disable_qpi)
    {
        /* cmd buf 0: write cmd cmd_4pp, big endian, 1byte, 1 bit mode */
        SFCDRV_SetCMDBuf(cs, CMD_BUF_0, cmd_4pp[cs]);

    	if(cmd_4pp[cs] == CMD_4PP)
    	{
        	SFCDRV_SetCMDBuf(cs, CMD_BUF_1, (addr & s_address_mask[cs]));
        	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, bitmode, s_address_cycle[cs], CMD_MODE_WRITE, SEND_MODE_1);

            bitmode = BIT_MODE_4;
    	}
    	else
    	{
    		bitmode = BIT_MODE_4;

        	SFCDRV_SetCMDBuf(cs, CMD_BUF_1, (addr & s_address_mask[cs]));
        	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, bitmode, s_address_cycle[cs], CMD_MODE_WRITE, SEND_MODE_1);
        }
    }
    else
    {
        /* cmd buf 0: write cmd 0x02, big endian, 1byte, 1 bit mode */
        SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_PP);

    	SFCDRV_SetCMDBuf(cs, CMD_BUF_1, (addr & s_address_mask[cs]));
    	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, bitmode, s_address_cycle[cs], CMD_MODE_WRITE, SEND_MODE_1);
    }

    /* cmd buf 4: write value, big endian, n byte, 4 bit mode */
    SFCDRV_SetCMDBufEx(cs, CMD_BUF_2, buf, counter);

    /* clear int status */
    SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
    /* start soft request */
    SFCDRV_SoftReq(cs);

    while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
    /* start soft request */
    SFCDRV_SoftReq();

    while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif

    if(QPI_MODE == work_mode[cs])
    {
        bitmode = BIT_MODE_4;
    }
    else
    {
        bitmode = BIT_MODE_1;
    }

    do
    {
        //poll status
        /* clear cmd buffer and type buffer*/
        SFCDRV_TypeBufClr(cs);

        /* write command 05, big endian, 1byte */
        SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_RDSR1);
        //SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

        /* read cmdbuf7, big endian, 1byte */
        SFCDRV_SetTypeInfBuf(cs, CMD_BUF_7, bitmode, BYTE_NUM_1, CMD_MODE_READ, SEND_MODE_0);

        /* clear int status */
        SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
        /* start soft request */
        SFCDRV_SoftReq(cs);

        while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));

#else
        /* start soft request */
        SFCDRV_SoftReq();

        while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif

        status1 = (SFCDRV_GetCMDBuf(cs, CMD_BUF_7)>>24);

        if(nQEPos[cs]>=8 && nQEPos[cs]!=UNSUPPORT_CMD)
        {
            /* write command 05, big endian, 1byte */
            SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_RDSR2);

            /* clear int status */
            SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
            /* start soft request */
            SFCDRV_SoftReq(cs);

            while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
            /* start soft request */
            SFCDRV_SoftReq();

            while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif

            status2 = (SFCDRV_GetCMDBuf(cs, CMD_BUF_7)>>24);
        }

        status = (status1 | status2<<8);
        if(!g_fdl_spiflash_disable_do_idle)
            FLASH_DO_IDLE
    }while(status & (1<<nWIPPos[cs]));

    SPIFLASH_ResetRegister(cs);

    return SCI_TRUE;

}

/*****************************************************************************/
//     Description :
//      The function write word to flash.
//
//    Global resource dependence :
//      None
//  Author:
//      Lin.liu
//    Note:
//      The address MUST be word boundary.
//
//      We not verify the data that written.
//      If some bit can't convert from 0 to 1, the data that has written
//      is not correct.
/*****************************************************************************/
PUBLIC BOOLEAN  SPIFLASH_WriteWord(
    uint32      addr,
    uint16      data)
{
	FLASH_PTR_TYPE          addr_ptr = (FLASH_PTR_TYPE)addr;
	volatile uint16         tmp;
	int status;
	BIT_MODE_E bitmode = BIT_MODE_1;
	int cs = SPIFLASH_CS0;

#if defined(PLATFORM_SC6530) || defined(CHIP_VER_UWS6121E)
	if((addr >= SPIFLASH_CS1_BASE_ADDRESS) && (s_nMID[SPIFLASH_CS1] > 0))
	{
		cs = SPIFLASH_CS1;
		addr -= SPIFLASH_CS1_BASE_ADDRESS;
	}
	#endif

    SFCDRV_CSCfg(cs);

	if(QPI_MODE == work_mode[cs])
	{
		bitmode = BIT_MODE_4;
	}

    SCI_ASSERT( !(addr & 1));/*assert verified*/

	tmp = *addr_ptr;

	if( tmp == data )
	{
	   // It is same, needn't write
	   return SCI_TRUE;
	}
	else if( (tmp & data) != data )
	{
	   // some bits from 0 to 1, can't write
	    return SCI_FALSE;
	}

	do
	{
	    SPIFLASH_WriteEnable(cs);
	    status = SPIFLASH_GetPollStatus(cs);
	}while(!(status & BIT_1));

	/* clear cmd buffer and type buffer*/
	SFCDRV_TypeBufClr(cs);
	SFCDRV_CMDBufClr(cs);

	/* set write command */
	SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, bitmode, INI_CMD_BUF_7);

    if((!QPI_support[cs]) || g_fdl_spiflash_disable_qpi)
    {
        /* cmd buf 0: write cmd cmd_4pp, big endian, 1byte, 1 bit mode */
        SFCDRV_SetCMDBuf(cs, CMD_BUF_0, cmd_4pp[cs]);
        SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

        if(cmd_4pp[cs] == CMD_4PP)
        {
        	SFCDRV_SetCMDBuf(cs, CMD_BUF_1, (addr & s_address_mask[cs]));
        	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, bitmode, s_address_cycle[cs], CMD_MODE_WRITE, SEND_MODE_1);

            bitmode = BIT_MODE_4;
    	}
    	else
    	{
    		bitmode = BIT_MODE_4;

        	SFCDRV_SetCMDBuf(cs, CMD_BUF_1, (addr & s_address_mask[cs]));
        	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, bitmode, s_address_cycle[cs], CMD_MODE_WRITE, SEND_MODE_1);
        }
    }
    else
    {
        /* cmd buf 0: write cmd 0x02, big endian, 1byte, 1 bit mode */
        SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_PP);
        SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

    	SFCDRV_SetCMDBuf(cs, CMD_BUF_1, (addr & s_address_mask[cs]));
    	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, bitmode, s_address_cycle[cs], CMD_MODE_WRITE, SEND_MODE_1);
    }


	/* cmd buf 2: write value, big endian, 2 byte, 1 bit mode */
	SFCDRV_SetCMDBuf(cs, CMD_BUF_2, data);
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_2, bitmode, BYTE_NUM_2, CMD_MODE_WRITE, SEND_MODE_0);

	/* clear int status */
	SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
	/* start soft request */
	SFCDRV_SoftReq(cs);

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
	/* start soft request */
	SFCDRV_SoftReq();

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif

	/* clear int status */
	SFCDRV_IntClr(cs);

	do
	{
	    status = SPIFLASH_GetPollStatus(cs);
        if(!g_fdl_spiflash_disable_do_idle)
    		FLASH_DO_IDLE
    }while(status & (1<<nWIPPos[cs]));

	SPIFLASH_ResetRegister(cs);

    tmp = FLASH_RD(addr_ptr);
    //ret = (tmp != data) ? 0 : 1;

    return ( (tmp == data)?SCI_TRUE:SCI_FALSE );
}

LOCAL BOOLEAN SPIFLASH_IsStartAddrOf_64KB( uint32  addr )
{
    uint32  sector_size, start_addr;
    sector_size = BLOCK_SIZE_64K;
    start_addr  = addr & (~(sector_size - 1) );
    return (start_addr == addr) ? TRUE : FALSE;
}

LOCAL BOOLEAN SPIFLASH_IsStartAddrOf_4KB( uint32  addr )
{
    uint32  sector_size, start_addr;
    sector_size = BLOCK_SIZE_4K;
    start_addr  = addr & (~(sector_size - 1) );
    return (start_addr == addr) ? TRUE : FALSE;
}

LOCAL BOOLEAN SPIFLASH_IsStartAddrOfSector( uint32  addr )
{
    uint32  sector_size, start_addr;
    sector_size = BLOCK_SIZE;
    start_addr  = addr & (~(sector_size - 1) );
    return (start_addr == addr) ? TRUE : FALSE;
}

LOCAL BOOLEAN SPIFLASH_IsStartAddrOfPage( uint32  addr )
{
    uint32  page_size, start_addr;
    page_size = SPIFLASH_PAGE_LEN;
    start_addr  = addr & (~(page_size - 1) );
    return (start_addr == addr) ? TRUE : FALSE;
}

/*****************************************************************************/
//     Description :
//      The function write data to flash.
//
//    Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//    Note:
//
//      We not verify the data that written.
//      If some bit can't convert from 0 to 1, the data that has written
//      is not correct.
/*****************************************************************************/
LOCAL BOOLEAN SPIFLASH_Write(    // If the data being written to flash, return TRUE.
    uint32       addr,  // the start address that will be written.
    const uint8      * buf,   // Pointer to the buffer containing the data to be written.
    uint32       len)   // number of bytes to write.
{
    int tmp;
    uint8 *data_buf = (uint8 *)buf;
	int cs = SPIFLASH_CS0;

	#if defined(PLATFORM_SC6530) || defined(CHIP_VER_UWS6121E)
	if(addr >= SPIFLASH_CS1_BASE_ADDRESS)
	{
		cs = SPIFLASH_CS1;
        #ifdef SIMULATE_FLASH2_SUPPORT
            memcpy((uint8 *)addr, buf, len);
            return SCI_TRUE;
        #endif
	}
	#endif

#if 0
	if(SPIFLASH_IsStartAddrOfSector(addr))
    {
        if( 0 == FDL_FlashErase_Block(addr, BLOCK_SIZE) )
        {
            return SCI_FALSE;
        }
    }
#endif
    while(len)
    {
        /* 1 sector 256 bytes align */
        int size = SPIFLASH_PAGE_LEN - (addr & (SPIFLASH_PAGE_LEN - 1));
        size = (size >= len) ? (len) : (size);
        SPIFLASH_WriteBuffer(addr, data_buf, size);
        addr += size;
        data_buf += size;
        len -= size;
    }

    return SCI_TRUE;
}

// enter_read/exit_read should be called with pair
//
LOCAL void SPIFLASH_EnterRead( void )
{

}

LOCAL void SPIFLASH_ExitRead( void )
{

}

/*****************************************************************************/
//     Description :
//      Read word(two byte) from the given address.
//
//    Global resource dependence :
//      None
//  Author:
//      Lin.liu
//    Note:
//      the address MUST be word boundary.
/*****************************************************************************/
LOCAL uint16 SPIFLASH_ReadWord(  // The word that has read from the given address.
    uint32 addr)        // the address that will be read.
{
    uint16   read_data; // the data that be read.

    // Word boundary.
    SCI_ASSERT( !(addr & 1) );/*assert verified*/

    read_data = *((FLASH_PTR_TYPE)addr);

    return read_data;
}


LOCAL uint32 SPIFLASH_ReadDWord(uint32 addr)
{
	uint32 read_data;
	SCI_ASSERT( !(addr & 3) );/*assert verified*/

	read_data = *((uint32 *)addr);

    return read_data;

}
/*****************************************************************************/
//     Description :
//      The function reads up to size bytes from the device and stores them in buffer.
//
//    Global resource dependence :
//      None
//  Author:
//      Lin.liu
//    Note:
//
/*****************************************************************************/
LOCAL BOOLEAN SPIFLASH_Read(      // If read successfully, return true, else false;
    uint32    addr,      // the start address that will be read
    uint8   * buf,       // store the data that being read to this buffer.
    uint32    read_len)  // the count of bytes that will be read.
{
    uint32 i;


    for(i=0; i<read_len; i++)
    {
        *(buf + i) = *(uint8 *)(addr + i);
    }

    return SCI_TRUE;
}

/*****************************************************************************/
//     Description :
//      The function unlock the sector that contain the address.
//
//    Global resource dependence :
//      None
//  Author:
//      Lin.liu
//    Note:
//
/*****************************************************************************/
LOCAL void SPIFLASH_Unlock(
    uint32   addr)  // the sector that contain the address will be unlock.
{
    uint32 status = 0;
    uint32 unlock_value = 0;
    int cs = SPIFLASH_CS0;

	#ifdef PLATFORM_SC6530
    if((addr >= SPIFLASH_CS1_BASE_ADDRESS) && (s_nMID[SPIFLASH_CS1] > 0))
    {
        cs = SPIFLASH_CS1;
        addr -= SPIFLASH_CS1_BASE_ADDRESS;
    }
	#endif

    SFCDRV_CSCfg(cs);

    if(s_nMID[cs] == SPIFLASH_PCT_ID)
    {
        do
        {
            SPIFLASH_WriteEnable(cs);
            status = SPIFLASH_GetPollStatus(cs);
        }while(!(status & BIT_1));

        SFCDRV_TypeBufClr(cs);
        SFCDRV_CMDBufClr(cs);

        SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, BIT_MODE_4, INI_CMD_BUF_7);

        SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_WBPR);
        SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, BIT_MODE_4, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

        SFCDRV_SetCMDBuf(cs, CMD_BUF_1, unlock_value);
        SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, BIT_MODE_4, BYTE_NUM_4, CMD_MODE_WRITE, SEND_MODE_0);

        SFCDRV_SetCMDBuf(cs, CMD_BUF_2, unlock_value);
        SFCDRV_SetTypeInfBuf(cs, CMD_BUF_2, BIT_MODE_4, BYTE_NUM_4, CMD_MODE_WRITE, SEND_MODE_0);

        SFCDRV_SetCMDBuf(cs, CMD_BUF_3, unlock_value);
        SFCDRV_SetTypeInfBuf(cs, CMD_BUF_3, BIT_MODE_4, BYTE_NUM_2, CMD_MODE_WRITE, SEND_MODE_0);

        /* clear int status */
        SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
        /* start soft request */
        SFCDRV_SoftReq(cs);

        while (!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
        /* start soft request */
        SFCDRV_SoftReq();

        while (!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif

        /* clear int status */
        SFCDRV_IntClr(cs);

        return;
    }


}

/*****************************************************************************/
//     Description :
//      The function lock down the sector that contain the address.
//
//    Global resource dependence :
//      None
//  Author:
//      Lin.liu
//    Note:
//
/*****************************************************************************/
LOCAL void SPIFLASH_Lock(uint32  addr )
{
}


/*****************************************************************************/
//     Description :
//      Is the last erase optr completed?
//
//    Global resource dependence :
//      None
//
//    Return :
//      If the erase has been completely, return true, other false
//  Author:
//      Lin.liu
//  Note:
//      This function MUST be called when system assert only,
/*****************************************************************************/
LOCAL BOOLEAN SPIFLASH_IsEraseCompleted(void)
{
    uint32  status;

    // Other case, we think has been completed.
    return SCI_TRUE;
}


/*****************************************************************************/
//     Description :
//      Erase the 4KB sector that contain the address.
//
//    Global resource dependence :
//      None
//
//  Author:
//      Fei.zhang
//    Note:
//      This will take a long time, typecal 1 second.
//      And this function must be called from a thread.
//
/*****************************************************************************/
LOCAL BOOLEAN SPIFLASH_EraseSector(
    uint32 addr)
{
	uint32 status = 0;
	BIT_MODE_E bitmode = BIT_MODE_1;
	int cs = SPIFLASH_CS0;

	//FDL_SendTrace("%s %d", __func__, __LINE__);
#if defined(PLATFORM_SC6530) || defined(CHIP_VER_UWS6121E)
	if((addr >= SPIFLASH_CS1_BASE_ADDRESS) && (s_nMID[SPIFLASH_CS1] > 0))
	{
		cs = SPIFLASH_CS1;
		addr -= SPIFLASH_CS1_BASE_ADDRESS;
	}
	#endif

    SFCDRV_CSCfg(cs);

	if(QPI_MODE == work_mode[cs])
	{
		bitmode = BIT_MODE_4;
	}

	do
	{
	    SPIFLASH_WriteEnable(cs);
	    status = SPIFLASH_GetPollStatus(cs);
	}while(!(status & BIT_1));

	/* clear cmd buffer and type buffer*/
	SFCDRV_TypeBufClr(cs);
	SFCDRV_CMDBufClr(cs);

	/* set write command */
	SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, bitmode, INI_CMD_BUF_7);

	/* cmd buf 0: write cmd 0x20, big endian, 1byte, 1 bit mode */
	SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_SE);
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

	/* cmd buf 1: write address, big endian, 3byte, 1 bit mode */
	SFCDRV_SetCMDBuf(cs, CMD_BUF_1, (addr & s_address_mask[cs]));
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, bitmode, s_address_cycle[cs], CMD_MODE_WRITE, SEND_MODE_1);

	/* clear int status */
	SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
	/* start soft request */
	SFCDRV_SoftReq(cs);

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
	/* start soft request */
	SFCDRV_SoftReq();

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif

	/* clear int status */
	SFCDRV_IntClr(cs);
#if defined(PLATFORM_UWS6121E)
	FLASH_DELAY_US(30);
#else
	FLASH_DELAY_US(30) //wait the WIP bit been set to 1
#endif

	do
	{
	    status = SPIFLASH_GetPollStatus(cs);
        if(!g_fdl_spiflash_disable_do_idle)
    		FLASH_DO_IDLE
    }while(status & (1<<nWIPPos[cs]));

	SPIFLASH_ResetRegister(cs);

    return SCI_TRUE;
}

LOCAL BOOLEAN SPIFLASH_EraseMultSetor(uint32 addr, uint32 sec_num)
{
	 BOOLEAN ret = TRUE;
	 uint32 t = 0;
	 for(t = 0; t < sec_num; t++)
	 {
	 	  ret = SPIFLASH_EraseSector(addr + t*BLOCK_SIZE_4K);
	 	  if(!ret)
	 	  {
	 	  	break;
	 	  }
	 }
	 return ret;
}

/*****************************************************************************/
//     Description :
//      Erase the 32K Block that contain the address.
//
//    Global resource dependence :
//      None
//
//  Author:
//      Fei.zhang
//    Note:
//      This will take a long time, typecal 1 second.
//      And this function must be called from a thread.
//
/*****************************************************************************/
LOCAL BOOLEAN SPIFLASH_EraseBlock_32KB(
    uint32 addr)
{
	int   ret = TRUE;
	uint32 status = 0;
	BIT_MODE_E bitmode = BIT_MODE_1;
	int cs = SPIFLASH_CS0;

	//FDL_SendTrace("%s %d", __func__, __LINE__);
#if defined(PLATFORM_SC6530) || defined(CHIP_VER_UWS6121E)
	if((addr >= SPIFLASH_CS1_BASE_ADDRESS) && (s_nMID[SPIFLASH_CS1] > 0))
	{
		cs = SPIFLASH_CS1;
		addr -= SPIFLASH_CS1_BASE_ADDRESS;
	}
	#endif

    SFCDRV_CSCfg(cs);

	if(QPI_MODE == work_mode[cs])
	{
		bitmode = BIT_MODE_4;
	}
	do
	{
	    SPIFLASH_WriteEnable(cs);
	    status = SPIFLASH_GetPollStatus(cs);
	}while(!(status & BIT_1));

	/* clear cmd buffer and type buffer*/
	SFCDRV_TypeBufClr(cs);
	SFCDRV_CMDBufClr(cs);

	/* set write command */
	SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, bitmode, INI_CMD_BUF_7);

	/* cmd buf 0: write cmd 0x52, big endian, 1byte, 1 bit mode */
	SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_32KBE);
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

	/* cmd buf 1: write address, big endian, 3byte, 1 bit mode */
	SFCDRV_SetCMDBuf(cs, CMD_BUF_1, (addr & s_address_mask[cs]));
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, bitmode, s_address_cycle[cs], CMD_MODE_WRITE, SEND_MODE_1);


	/* clear int status */
	SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
	/* start soft request */
	SFCDRV_SoftReq(cs);

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
	/* start soft request */
	SFCDRV_SoftReq();

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif

	/* clear int status */
	SFCDRV_IntClr(cs);

#if defined(PLATFORM_UWS6121E)
	FLASH_DELAY_US(30);
#else
	FLASH_DELAY_US(30) //wait the WIP bit been set to 1
#endif

	do
	{
	    status = SPIFLASH_GetPollStatus(cs);
        if(!g_fdl_spiflash_disable_do_idle)
    		FLASH_DO_IDLE
    }while(status & (1<<nWIPPos[cs]));

	SPIFLASH_ResetRegister(cs);

	return (BOOLEAN)ret;
}

/*****************************************************************************/
//     Description :
//      Erase the 64K Block that contain the address.
//
//    Global resource dependence :
//      None
//
//  Author:
//      Fei.zhang
//    Note:
//      This will take a long time, typecal 1 second.
//      And this function must be called from a thread.
//
/*****************************************************************************/
LOCAL BOOLEAN SPIFLASH_EraseBlock_64KB(
    uint32 addr)
{
	int cs = SPIFLASH_CS0;
	uint32 status = 0;
	BIT_MODE_E bitmode = BIT_MODE_1;

	//FDL_SendTrace("%s %d, addr=0x%x", __func__, __LINE__, addr);
#if defined(PLATFORM_SC6530) || defined(CHIP_VER_UWS6121E)
	if((addr >= SPIFLASH_CS1_BASE_ADDRESS) && (s_nMID[SPIFLASH_CS1] > 0))
	{
		cs = SPIFLASH_CS1;
		addr -= SPIFLASH_CS1_BASE_ADDRESS;
	}
	#endif

    SFCDRV_CSCfg(cs);

	if(QPI_MODE == work_mode[cs])
	{
		bitmode = BIT_MODE_4;
	}
	do
	{
	    SPIFLASH_WriteEnable(cs);
	    status = SPIFLASH_GetPollStatus(cs);
	}while(!(status & BIT_1));

	/* clear cmd buffer and type buffer*/
	SFCDRV_TypeBufClr(cs);
	SFCDRV_CMDBufClr(cs);

	/* set write command */
	SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, bitmode, INI_CMD_BUF_7);

	/* cmd buf 0: write cmd 0xD8, big endian, 1byte, 1 bit mode */
	SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_64KBE);
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

	/* cmd buf 1: write address, big endian, 3byte, 1 bit mode */
	SFCDRV_SetCMDBuf(cs, CMD_BUF_1, (addr & s_address_mask[cs]));
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_1, bitmode, s_address_cycle[cs], CMD_MODE_WRITE, SEND_MODE_1);

	/* clear int status */
	SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
	/* start soft request */
	SFCDRV_SoftReq(cs);

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
	/* start soft request */
	SFCDRV_SoftReq();

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif

	/* clear int status */
	SFCDRV_IntClr(cs);

#if defined(PLATFORM_UWS6121E)
	FLASH_DELAY_US(30);
#else
	FLASH_DELAY_US(30) //wait the WIP bit been set to 1
#endif

	do
	{
	    status = SPIFLASH_GetPollStatus(cs);
        if(!g_fdl_spiflash_disable_do_idle)
		    FLASH_DO_IDLE
    }while(status & (1<<nWIPPos[cs]));

	SPIFLASH_ResetRegister(cs);

    return SCI_TRUE;
}


/*****************************************************************************/
//     Description :
//      Erase the whole chip.
//
//    Global resource dependence :
//      None
//
//  Author:
//      Fei.zhang
//    Note:
//      This will take a long time, typecal 1 second.
//      And this function must be called from a thread.
//
/*****************************************************************************/
LOCAL BOOLEAN SPIFLASH_EraseChip(int cs)
{
	uint32 status = 0;
	BIT_MODE_E bitmode = BIT_MODE_1;

	if(QPI_MODE == work_mode[cs])
	{
		bitmode = BIT_MODE_4;
	}

	do
	{
	    SPIFLASH_WriteEnable(cs);
	    status = SPIFLASH_GetPollStatus(cs);
	}while(!(status & BIT_1));

	/* clear cmd buffer and type buffer*/
	SFCDRV_TypeBufClr(cs);
	SFCDRV_CMDBufClr(cs);

	/* set write command */
	SFCDRV_SetCMDCfgReg(cs, CMD_MODE_WRITE, bitmode, INI_CMD_BUF_7);

	/* cmd buf 0: write cmd 0x60/0xC7, big endian, 1byte, 1 bit mode */
	SFCDRV_SetCMDBuf(cs, CMD_BUF_0, CMD_CE);
	SFCDRV_SetTypeInfBuf(cs, CMD_BUF_0, bitmode, BYTE_NUM_1, CMD_MODE_WRITE, SEND_MODE_0);

	/* clear int status */
	SFCDRV_IntClr(cs);

#if defined(CHIP_VER_UWS6121E)
	/* start soft request */
	SFCDRV_SoftReq(cs);

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus(cs)));
#else
	/* start soft request */
	SFCDRV_SoftReq();

	while(!(SFC_IDLE_STATUS & SFCDRV_GetStatus()));
#endif

	/* clear int status */
	SFCDRV_IntClr(cs);

	do
	{
	    status = SPIFLASH_GetPollStatus(cs);
        if(!g_fdl_spiflash_disable_do_idle)
            FLASH_DO_IDLE
    }while(status & (1<<nWIPPos[cs]));

	SPIFLASH_ResetRegister(cs);

    return SCI_TRUE;
}

/*****************************************************************************/
//     Description :
//      Erase the sector that contain the address.
//
//    Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//    Note:
//      This will take a long time, typecal 1 second.
//      And this function must be called from a thread.
//
/*****************************************************************************/
LOCAL BOOLEAN SPIFLASH_Erase(
    uint32 addr)
{
       FLASH_PTR_TYPE addr_ptr;
	addr_ptr = (FLASH_PTR_TYPE)(addr & 0xFFFFFFFE);

    return FDL_FlashErase_Block((uint32)addr_ptr, BLOCK_SIZE);

}

/*****************************************************************************/
//     Description :
//      Suspend the program/erase operation
//
//    Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//    Note:
//
/*****************************************************************************/
LOCAL void SPIFLASH_Suspend(void)
{

}

/*****************************************************************************/
//     Description :
//      Resume the program/erase operation
//      In some case, system will restart( not power on ) while erasing/programing.
//      so, flash manager need detect this casae and  recover/continue the last optr
//    Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//    Note:
//
/*****************************************************************************/
LOCAL void SPIFLASH_Resume(void)
{

}


uint16 FDL_FlashReadWord(uint32 addr)
{
    return SPIFLASH_ReadWord(addr);
}

uint32 FDL_FlashReadDWord(uint32 addr)
{
	return SPIFLASH_ReadDWord(addr);
}

uint16 FDL_FlashWriteWord(uint32 addr, uint16 data)
{
    return SPIFLASH_WriteWord(addr, data);
}


uint32 FDL_FlashWriteEnable (uint32 start_addr, uint32  enable)
{
    if( enable )
    {
        g_flash_start_addr    = start_addr;
        s_flash_start_ptr     = (FLASH_PTR_TYPE)(start_addr);
    }

    return 1;
}

uint32 FDL_FlashWriteArea (uint32 addr, char *buf, uint32 size)
{
    uint32 fdl_file_remain_size = 0;
    uint32 fdl_file_end_addr;
    static uint32 erase_sector_size = 0;
    uint32 i;

    if (BLOCK_SIZE < BLOCK_SIZE_64K)
    {
        if ((addr >= latest_erase_end_addr)
                || (addr < (latest_erase_end_addr - erase_sector_size))
            )
        {
            fdl_file_end_addr = Get_fdl_file_end_addr();

            //FDL_SendTrace("%s %d 0x%x 0x%x 0x%x 0x%x", __func__, __LINE__, fdl_file_end_addr, addr, size, *(uint32 *)buf);

            // for speedup erase spiflash, henry.he 20120514
            if ((fdl_file_end_addr > addr)
                    && (fdl_file_end_addr < FLASH_START_ADRESS + flash_cs0_desity + flash_cs1_desity)
                )
            {
                fdl_file_remain_size = fdl_file_end_addr - addr;

                erase_sector_size = (fdl_file_remain_size <= BLOCK_SIZE) ? BLOCK_SIZE : BLOCK_SIZE_64K;
            }
            else
            {
                erase_sector_size = BLOCK_SIZE;
            }

            if ((erase_sector_size == BLOCK_SIZE_64K)
                    && (SPIFLASH_IsStartAddrOf_64KB(addr))
                )
            {
                if( 0 == FDL_FlashErase_Block(addr, BLOCK_SIZE_64K) )
                {
                    return 0;
                }
            }
            else if (SPIFLASH_IsStartAddrOfSector(addr))
            {
                if( 0 == FDL_FlashErase_Block(addr, BLOCK_SIZE) )
                {
                    return 0;
                }
            }
        }
    }
    else
    {
    	if(SPIFLASH_IsStartAddrOfSector(addr))
        {
        	for(i=0;i<size;i+=BLOCK_SIZE)
        	{
	            if( 0 == FDL_FlashErase_Block(addr+i, BLOCK_SIZE) )
	            {
	                return 0;
	            }
        	}
        }
    }

    //FDL_SendTrace("%s %d 0x%x 0x%x 0x%x", __func__, __LINE__, addr, size, *(uint32 *)buf);

    SPIFLASH_Write(addr, (const uint8 *)buf, size);

    return size;
}

uint32 FDL_FlashCheckSectorSize (uint32 start_addr)
{
    return  BLOCK_SIZE;
}

uint32 FDL_FlashRead (
    uint32 addr,
    uint8 *buf,
    uint32 read_len)
{
    SPIFLASH_Read(addr, buf, read_len);

    return read_len;
}

int FDL_FlashErase_Block(uint32 addr, uint32 block_size)
{
    int cs = SPIFLASH_CS0;

#if defined(PLATFORM_SC6530) || defined(CHIP_VER_UWS6121E)
    if((addr >= SPIFLASH_CS1_BASE_ADDRESS) && (s_nMID[SPIFLASH_CS1] > 0))
    {
        cs = SPIFLASH_CS1;
        #ifdef SIMULATE_FLASH2_SUPPORT
            return 1;
        #endif
	}
	#endif

    //FDL_SendTrace("erase 0x%x 0x%x", addr, block_size);

	switch(block_size)
	{
        case  BLOCK_SIZE_64K:
    	{
    	    if(s_nMID[cs] == SPIFLASH_PCT_ID)
    	    {
                SPIFLASH_EraseMultSetor(addr, 16);
            }
    	    else
    	    {
    	        SPIFLASH_EraseBlock_64KB(addr);
    	    }
    		break;
    	}
        case  BLOCK_SIZE_32K:
    	{
    		if (/*(s_nMID[cs] == SPIFLASH_MXIC_ID)
    		        ||  */(s_nMID[cs] == SPIFLASH_WINBOND_ID)
    		        || (s_nMID[cs] == SPIFLASH_GD_ID)
    		        || (s_nMID[cs] == SPIFLASH_FIDELIX_ID)
    		        || (s_nMID[cs] == SPIFLASH_BG_ID)
    		    )
    		{
    			SPIFLASH_EraseBlock_32KB(addr);
    		}
    		else
    		{
    			SPIFLASH_EraseMultSetor(addr, 8);
    		}
    		break;
    	}
        case  BLOCK_SIZE_4K:
    	{
    		SPIFLASH_EraseSector(addr);
    		break;
    	}
        default:
    	{
    		return 0;
    		break;
    	}
	}

	latest_erase_end_addr = addr + block_size;

    return 1;
}

uint32 FDL_FlashErase(uint32 start_addr,  uint32 size)
{
    uint32     sector_size, tmp;
    int        ret = 0;
    uint32     addr,tmp_addr;
    uint16    status = EFS_SECTOR_GOOD_STATUS;
    BOOLEAN is_top_stru = SCI_FALSE;

    sector_size = BLOCK_SIZE;

    tmp = start_addr & (sector_size - 1);

    // roundary start address to sector boundary
    start_addr -= tmp;

    addr = start_addr;

    // after roundary the start address, need roundary erase size
    //size += tmp;
    size -= tmp;

    if ( start_addr == FLASH_START_ADRESS )
    {
        if ( size >= flash_cs0_desity + flash_cs1_desity )
        {
            #if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
                ret = SPIFLASH_EraseChip(0);
                return ret;
            #else
                ret = SPIFLASH_EraseChip(0);
                if ( 0 == ret )
                {
    			return ret;
                }
                ret = SPIFLASH_EraseChip(1);
                return ret;
            #endif
        }
        else if ( size >= flash_cs0_desity )
        {
            ret = SPIFLASH_EraseChip(0);
            return ret;
        }
    }

    while( (size != 0) && (0 == (size & 0x80000000))  )  // check overflow
    {
        if ((size >= BLOCK_SIZE_64K)
                && (SPIFLASH_IsStartAddrOf_64KB(addr))
            )
        {
            sector_size = BLOCK_SIZE_64K;
        }
        else
        {
            sector_size = BLOCK_SIZE;
        }

        if (addr >= latest_erase_end_addr)
        {
            ret = FDL_FlashErase_Block(addr, sector_size);
            if( 0 == ret )
            {
                return ret;
            }
        }

        if(FDL_InEfsSpace(addr))
        {
            tmp_addr = addr;
            addr += BLOCK_SIZE;
            size -= BLOCK_SIZE;

            //FDL_SendTrace("%s %d addr=0x%x", __func__, __LINE__, addr);

            if (SPIFLASH_IsStartAddrOfSector(tmp_addr))
            {
                SPIFLASH_WriteWord((tmp_addr + BLOCK_SIZE - 2), status);
            }
        }
        else
        {
            addr += sector_size;
            size-= sector_size;
        }
    }

    return ret;
}



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

/*      End flash.c       */

