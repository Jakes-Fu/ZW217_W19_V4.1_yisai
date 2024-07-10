/******************************************************************************
 ** File Name:     Tf_dummy.c                                                   *
 ** Description:	 													 	  *		                                                          *
 ** Author:         tao.feng                                             	  *
 ** DATE:           21/01/2011                                                *
 ** Copyright:     2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 ******************************************************************************


 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           	 NAME             DESCRIPTION                              *
 ** 21/01/2011      tao.feng         Create									  *
 ******************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "sci_types.h"
#include "arm_reg.h"
#include "clock_drvapi.h"
#include "ldo_drvapi.h"
#include "uws6121e_reg_base.h"
#include "sys_ctrl.h"
#include "uws6121e_reg_analog.h"

typedef enum
{
    ISR_DONE = 0x0,
    CALL_HISR = 0xe05a05a5//magic number
} ISR_EXE_T;

typedef enum
{
   PAD_SD_STS_LOW_LEVEL_BEFORE_POWR_DOWN,
   PAD_SD_STS_RESTORE_BEFORE_POWR_UP,
   PAD_SD_STS_MAX
}PIN_SD_STS_E;

typedef enum SDHOST_DMA_BUF_SIZE_E_TAG
{
	SDIO_DMA_4K,
	SDIO_DMA_8K,
	SDIO_DMA_16K,
	SDIO_DMA_32K,
	SDIO_DMA_64K,
	SDIO_DMA_128K,
	SDIO_DMA_256K,
	SDIO_DMA_512K
}
SDHOST_DMA_BUF_SIZE_E;

typedef enum SDHOST_CMD_TYPE_E_TAG
{
	CMD_TYPE_NORMAL = 0,
	CMD_TYPE_SUSPEND,
	CMD_TYPE_RESUME,
	CMD_TYPE_ABORT
}SDHOST_CMD_TYPE_E;

typedef enum SDHOST_RST_TYPE_E_TAG
{
	RST_CMD_LINE,
	RST_DAT_LINE,
	RST_CMD_DAT_LINE,
	RST_ALL,
	RST_MODULE
}
SDHOST_RST_TYPE_E;

typedef enum SDHOST_CLK_ONOFF_E_TAG
{
	CLK_ON,
	CLK_OFF
}
SDHOST_CLK_ONOFF_E;

typedef enum SDHOST_PWR_ONOFF_E_TAG
{
	POWR_ON = 0,
	POWR_OFF
}
SDHOST_PWR_ONOFF_E;

typedef enum CMD_RSP_TYPE_E_TAG
{
	CMD_NO_RSP = 0,
	CMD_RSP_R1,
	CMD_RSP_R2,
	CMD_RSP_R3,
	CMD_RSP_R4,
	CMD_RSP_R5,
	CMD_RSP_R6,
	CMD_RSP_R7,
	CMD_RSP_R1B,
	CMD_RSP_R5B
}CMD_RSP_TYPE_E;

typedef enum SDHOST_VOL_RANGE_E_TAG
{
	VOL_1_8 = 180,	// 1.8 v
	VOL_2_65 = 265,	// 2.65 v
	VOL_2_8 = 280,	// 2.8 v
	VOL_3_0 = 300,	// 3.0 v
	VOL_RES
}
SDHOST_VOL_RANGE_E;

typedef enum SDHOST_SPEED_E_TAG
{
	SDIO_HIGHSPEED,	// high speed mode ,some card support this mode,clock rate can be up to 50MHz
	SDIO_LOWSPEED		// Normal speed mode , all the card support this mode default, clock rate can be up to 25MHz
}
SDHOST_SPEED_E;

typedef enum SDHOST_BIT_WIDTH_E_TAG
{
	SDIO_1BIT_WIDTH,
	SDIO_4BIT_WIDTH,
	SDIO_8BIT_WIDTH
}
SDHOST_BIT_WIDTH_E;

typedef enum SDHOST_SLOT_NO_TAG
{
	SDHOST_SLOT_0 = 0,
	SDHOST_SLOT_1 = 1,
	SDHOST_SLOT_2 = 2,
	SDHOST_SLOT_3 = 3,
	SDHOST_SLOT_4 = 4,
	SDHOST_SLOT_5 = 5,
	SDHOST_SLOT_6 = 6,
	SDHOST_SLOT_7 = 7,
	SDHOST_SLOT_MAX_NUM
}
SDHOST_SLOT_NO;

typedef struct SDIO_REG_CFG_TAG
{
    volatile uint32 DMA_SYS_ADD;
    volatile uint32 BLK_SIZE_COUNT;
    volatile uint32 CMD_ARGUMENT;
    volatile uint32 CMD_TRANSMODE;
    volatile uint32 RSP0;
    volatile uint32 RSP1;
    volatile uint32 RSP2;
    volatile uint32 RSP3;
    volatile uint32 BUFFER_PORT;
    volatile uint32 PRESENT_STAT;
    volatile uint32 HOST_CTL0;
    volatile uint32 HOST_CTL1;
    volatile uint32 INT_STA;
    volatile uint32 INT_STA_EN;
    volatile uint32 INT_SIG_EN;
    volatile uint32 CMD12_ERR_STA;
    volatile uint32 CAPBILITY;
    volatile uint32 CAPBILITY_RES;
    volatile uint32 CURR_CAPBILITY;
    volatile uint32 CURR_CAPBILITY_RES;
}
SDIO_REG_CFG;

typedef struct SDHOST_CAPBILIT_STRUCT_TAG
{
	uint32 capbility_function;
	uint32 cability_Max_BlkLen;	// max block size that host can be support
	uint32 sd_Base_Max_Clk;		// max base clock frequence that host can support
	uint32 timeOut_Clk_unit;		// unit of base clock frequency used to detect Data Timeout Error.
	uint32 timeOut_Base_Clk;		// base clock frequency used to detect Data Timeout Error.

	uint32 max_current_for_1_8;	// Maximum Current for 1.8V
	uint32 max_current_for_3_0;	// Maximum Current for 3.0V
	uint32 max_current_for_3_3;	// Maximum Current for 3.3V
}
SDHOST_CAPBILIT_T;

typedef void (*SDIO_CALLBACK)(uint32 msg,uint32 errCode,SDHOST_SLOT_NO slotNum);

typedef struct SDHOST_PORT_T_TAG
{
    volatile SDIO_REG_CFG *host_cfg;
    BOOLEAN open_flag;
    uint32 baseClock;
    uint32 sdClock;
    SDHOST_CAPBILIT_T capbility;
    uint32 err_filter;
    SDIO_CALLBACK sigCallBack;
} SDHOST_PORT_T;

typedef struct SDHOST_PORT_T_TAG* SDHOST_HANDLE;

/**---------------------------------------------------------------------------*
**                         Compiler Flag
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif

LOCAL SDHOST_PORT_T sdio_port_ctl[SDHOST_SLOT_MAX_NUM];

PUBLIC void SDHOST_Cfg_BusWidth (SDHOST_HANDLE sdhost_handler,SDHOST_BIT_WIDTH_E width)
{
    return;
}

PUBLIC void SDHOST_Cfg_SpeedMode (SDHOST_HANDLE sdhost_handler,SDHOST_SPEED_E speed)
{
    return;
}

PUBLIC void SDHOST_Cfg_Voltage (SDHOST_HANDLE sdhost_handler,SDHOST_VOL_RANGE_E voltage)
{
    return;
}

PUBLIC uint32 SDHOST_GetDmaAddr (SDHOST_HANDLE sdhost_handler)
{
    return 0;
}

PUBLIC uint32 SDHOST_GetPinState (SDHOST_HANDLE sdhost_handler)
{
    return 0;
}

PUBLIC void SDHOST_GetRspFromBuf (SDHOST_HANDLE sdhost_handler,CMD_RSP_TYPE_E Response,uint8 *rspBuf)
{
    return;
}

PUBLIC void SDHOST_NML_IntSig_Dis (SDHOST_HANDLE sdhost_handler,uint32 msg)
{
    return;
}

PUBLIC void SDHOST_NML_IntSig_En (SDHOST_HANDLE sdhost_handler,uint32 msg)
{
    return;
}

PUBLIC void SDHOST_NML_IntStatus_Clr (SDHOST_HANDLE sdhost_handler,uint32 msg)
{
    return;
}

PUBLIC void SDHOST_NML_IntStatus_Dis (SDHOST_HANDLE sdhost_handler,uint32 msg)
{
    return;
}

PUBLIC void SDHOST_NML_IntStatus_En (SDHOST_HANDLE sdhost_handler,uint32 msg)
{
    return;
}

PUBLIC void SDHOST_RST (SDHOST_HANDLE sdhost_handler,SDHOST_RST_TYPE_E rst_type)
{
    return;
}

PUBLIC SDHOST_HANDLE SDHOST_Register (SDHOST_SLOT_NO slot_NO,SDIO_CALLBACK fun)
{
    return &sdio_port_ctl[slot_NO];
}

PUBLIC uint32 SDHOST_SD_Clk_Freq_Set (SDHOST_HANDLE sdhost_handler,uint32 sdio_clk)
{
    return 0;
}

PUBLIC void SDHOST_SD_POWER (SDHOST_HANDLE sdhost_handler,SDHOST_PWR_ONOFF_E on_off)
{
    return;
}

PUBLIC void SDHOST_SD_clk_OnOff (SDHOST_HANDLE sdhost_handler,SDHOST_CLK_ONOFF_E onoff)
{
    return;
}

PUBLIC void SDHOST_SetCmd (SDHOST_HANDLE sdhost_handler,uint32 cmdIndex,uint32 transmode,SDHOST_CMD_TYPE_E cmd_type,CMD_RSP_TYPE_E Response)
{
    return;
}

PUBLIC void SDHOST_SetCmdArg (SDHOST_HANDLE sdhost_handler,uint32 argument)
{
    return;
}

PUBLIC void SDHOST_SetDataParam (SDHOST_HANDLE sdhost_handler,uint32 block_size,uint32 block_cnt,SDHOST_DMA_BUF_SIZE_E DMAsize)
{
    return;
}

PUBLIC void SDHOST_SetDataTimeOutValue (SDHOST_HANDLE sdhost_handler,uint8 clk_cnt) // (2 ^ (clkcnt+13))*T_BSCLK
{
    return;
}

PUBLIC void SDHOST_SetDmaAddr (SDHOST_HANDLE sdhost_handler, uint32 dmaAddr)
{
    return;
}

PUBLIC void SDHOST_SetErrCodeFilter (SDHOST_HANDLE sdhost_handler,uint32 err_msg)
{
    return;
}

PUBLIC void SDHOST_SetPad_Status (SDHOST_SLOT_NO slot_NO, PIN_SD_STS_E pin_sd_sts)
{
    return;
}

PUBLIC BOOLEAN SDHOST_UnRegister (SDHOST_HANDLE sdhost_handler)
{
    return 0;
}

PUBLIC void SDHOST_internalClk_OnOff (SDHOST_HANDLE sdhost_handler,SDHOST_CLK_ONOFF_E onoff)
{
    return;
}

PUBLIC ISR_EXE_T _SDHOST_IrqHandle (uint32 isrnum)
{
    return ISR_DONE;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

