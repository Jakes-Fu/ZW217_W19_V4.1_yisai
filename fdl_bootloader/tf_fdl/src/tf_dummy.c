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

#include "timer.h"
#include "Rf_registers.h"

#ifdef  PLATFORM_SC6530
#if defined(CHIP_VER_6531)
#include "sc6531_reg_base.h"
#include "sc6531_reg_global.h"
#else
#include "sc6530_reg_base.h"
#include "sc6530_reg_global.h"
#endif
#else
#ifdef PLATFORM_SC8800G
#include "sc8800g_reg_base.h"
#include "sc8800g_reg_global.h"
#else
#include "uws6121e_reg_base.h"
#include "sc6531efm_reg_global.h"
#include "sys_ctrl.h"
#include "adi_reg_uws6121e.h"
#include "uws6121e_reg_analog.h"
#include "uws6121e_timer_cfg.h"
#endif
#endif

struct HAL_IP_SUSPEND_RESUME_T
{
    char module_name[10];
    void (*reg_suspend)();                       //func addr of reg save
    void (*reg_resume)();                        //func addr of reg resume
    struct HAL_IP_SUSPEND_RESUME_T *next;        //next node addr
};

typedef enum
{
    TIMER_0,
    TIMER_1,
    TIMER_2,
    TIMER_3,
    TIMER_4,
    TIMER_5,
    TIMER_MAX,
} TIMER_ID_E; //the timer index

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

const uint32 tx_pri_PRI_HISR_PRIO_0 = 0;
const uint32 tx_pri_PRI_HISR_PRIO_1 = 1;
const uint32 tx_pri_PRI_HISR_PRIO_2 = 2;
unsigned int *g_mmu_page_table;
BOOLEAN g_LdoEmmcPwrOffFlag = FALSE;

typedef enum
{
	ERR_DISPLAY_NONE = 0,		// Success,no error
	ERR_DISPLAY_FUNC_NOT_SUPPORT,		// not support this function
	ERR_DISPLAY_PARAMETER_WRONG,		// Parameter is wrong
	ERR_DISPLAY_POINTER_NULL,			// Input pointer is PNULL
	ERR_DISPLAY_OPERATE_FAIL,			// Operate fail
	ERR_DISPLAY_NOT_FOUND				// LCD not found
} ERR_DISPLAY_E;

typedef enum
{
	MAIN_LCD_ID = 0,				//Main lcd id
	SUB_LCD_ID,						//Sub  lcd id
	MAX_LCD_ID						//Reserved, can not be used by user.
} LCD_ID_E;

typedef struct _display_rect_t_tag
{
    int16 left;  //left position of rectangle
    int16 top;   //top position of rectangle
    int16 right; //right position of rectangle
    int16 bottom;//position of rectangle
} DISPLAY_RECT_T;

typedef enum
{
    DISPLAY_ANGLE_0,
    DISPLAY_ANGLE_90,
    DISPLAY_ANGLE_180,
    DISPLAY_ANGLE_270
}DISPLAY_ANGLE_E;

typedef struct _display_convert_input_tag
{
    uint16 logic_lcd_width;
    uint16 logic_lcd_height;
    DISPLAY_RECT_T  logic_coordinate;
    DISPLAY_ANGLE_E logic_angle;
}DISPLAY_CONVERT_INPUT_T;

typedef struct _display_convert_output_tag
{
    DISPLAY_RECT_T  physical_coordinate;
    DISPLAY_ANGLE_E rotation_angle;
}DISPLAY_CONVERT_OUTPUT_T;

typedef struct
{
    uint32 id;

} SPI_DEV;

#define REG32(x)   (*((volatile uint32 *)(x)))

#define TIMER4_FULL_COUNT        0xFFFFFFFF
#define TIMER_DELAY_US(_x_)\
    {\
        volatile uint32 time_1 = 0, time_2 = 0;\
        volatile uint32 delay_count = 0;\
        delay_count = _x_ * 2;\
        time_1 = REG32(TIMER4_HWTIMER_CURVAL_L);\
        time_2 = time_1 + delay_count;\
        if (time_2 < time_1)\
        {\
            delay_count -= ((TIMER4_FULL_COUNT - time_1) + 1);\
            while(REG32(TIMER4_HWTIMER_CURVAL_L) >= time_1);\
            while(delay_count > REG32(TIMER4_HWTIMER_CURVAL_L));\
        }\
        else\
        {\
            while(time_2 >= REG32(TIMER4_HWTIMER_CURVAL_L));\
        }\
    }

#define DELAY_CYCLE(n)  {\
                            uint32 k=0;\
                             for(k=0; k<n; k++){}\
                         }
extern uint16 ADI_Analogdie_reg_read(uint32 addr);
extern void ADI_Analogdie_reg_write(uint32 addr, uint16 data);
extern  void ADI_Analogdie_reg_and (uint32 addr, uint16 data);

extern  void ADI_Analogdie_reg_mask_or (uint32 addr, uint16 data, uint16 msk);

extern  void ADI_Analogdie_reg_or (uint32 addr, uint16 data);


#define ADI_FIFO_IS_EMPTY   (hwp_adiMst->arm_cmd_status & ADI_MST_WFIFO_EMPTY)
#define ADI_FIFO_IS_FULL    (hwp_adiMst->arm_cmd_status & ADI_MST_WFIFO_FULL)

#define ANA_REG_AND(reg_addr, value)  ADI_Analogdie_reg_and(reg_addr, value)

#define ANA_REG_OR(reg_addr, value)  ADI_Analogdie_reg_or(reg_addr, value)

void MMU_DmaCacheSync (uint32 bufAddr, uint32 bufSize, uint32 dir)
{
    return;
}

void mon_Event(uint32 evt)
{
    return;
}

void SCI_Assert(void)
{
    return;
}

/*****************************************************************************/
//  Description:    The function is used to disable IRQ and save old status to
//                  stack.
//    Global resource dependence:
//  Author:         lin.liu
//    Note:
/*****************************************************************************/
PUBLIC void SCI_DisableIRQ(void)
{
    return;
}

PUBLIC uint32 SCI_GetAssertFlag (void)
{
    return TRUE;
}

/*****************************************************************************/
// Description :    This function used to get current tick count.
// Global resource dependence :
// Author :         Daniel.ding
// Note :
/*****************************************************************************/
PUBLIC uint32 SCI_GetTickCount(void)
{
    return 0;//TIMER_GetSystemCounterReg();
}

/*****************************************************************************/
//  Description:    The function is used to restore the old IRQ status.
//    Global resource dependence:
//  Author:         lin.liu
//    Note:
/*****************************************************************************/
PUBLIC void SCI_RestoreIRQ(void)
{
    return;
}

/*****************************************************************************/
//  Description:    Suspends the execution of the current thread for a
//                  specified interval.
//	Global resource dependence:
//  Author:         Xueliang.Wang
//	Note:           Number of milliseconds to sleep.
/*****************************************************************************/
PUBLIC void SCI_Sleep(uint32 time_in_ms)
{
    uint32 i;

    for (i = 0; i < time_in_ms; i++)
 	    TIMER_DELAY_US(1000);
}

/*****************************************************************************/
//  Description:    The function is typically used to trace
//    Global resource dependence:
//  Author:         Richard.Yang
//    Note:           This function is reserved.
/*****************************************************************************/

PUBLIC uint32 SCI_Trace_ID(uint32 trc_class, uint32 msg_index, const uint8* arg_type,...)
{
    return 1;
}

/*****************************************************************************/
//  Description:    The function is used to disable FIQ and save old status to
//                  stack.
//    Global resource dependence:
//  Author:         lin.liu
//    Note:           This opertion must be in SCI_DisableIRQ-SCI_RestoreIRQ
/*****************************************************************************/
PUBLIC void SCI_DisableFIQ(void)
{
    return;
}

/*****************************************************************************/
//  Description:    The function is used to restore FIQ and save old status to
//                  stack.
//    Global resource dependence:
//  Author:         lin.liu
//    Note:           This opertion must be in SCI_DisableIRQ-SCI_RestoreIRQ
/*****************************************************************************/
PUBLIC void SCI_RestoreFIQ(void)
{
    return;
}

PUBLIC void SCI_CreateSemaphore (//if successful, return semaphore
    //pointer, else return SCI_NULL
    const char *name_ptr, //name of the semaphore
    uint32 initial_count //initial value of semaphore counter
)
{
    return;
}

PUBLIC void SCI_SDIO_EnableDeepSleep (uint32 mode)
{
    return;
}

/*****************************************************************************/
//  Description:    The function is typically used to assert
//    Global resource dependence:
//  Author:         Richard.Yang
//    Note:           This function is reserved.
/*****************************************************************************/
PUBLIC void SCI_PAssert(
    void    *exp,     // Expression (including pointers) that evaluates
                      // to nonzero or 0
    void    *file,    // File name where assert
    uint32  line,      // Line number where assert
	char    *assert_info_ptr
    )
{
    return;
}


/*****************************************************************************/
//  Description:    The function is typically used to save the input parameter
//                  information as a string. Used by SCI_PAssert
//    Global resource dependence:
//  Author:         Richard.Yang
//    Note:           This function is reserved.
/*****************************************************************************/
PUBLIC char * SCI_SaveAssertInfo(
    const char *x_format,
    ...
    )
{
	return NULL;
}

void TM_SendTestPointRequest(uint32 param1, uint32 param2)
{
    return;
}

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
    return 0;
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

/*****************************************************************************/
//  Description:    Sets the register in analog die.
//  Author:
//  Note:           !It is called before __main now, so it can not call the adi
//                  interface because it contains SCI_DisableIRQ inside, below
//                  writes the adi read interface individually. Because the la-
//                  ckless of SCI_DisableIRQ, so this function must be called
//                  before system interrupt is turnned on!
/*****************************************************************************/
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

void CHIP_PHY_Set_HwRstFlag (uint32 val)
{
    ANA_REG_AND (ANA_POR_RST_MONITOR, ~0x1F);
    ANA_REG_OR (ANA_POR_RST_MONITOR, (val&0x1F));
}

void CHIP_PHY_ADI_Write(uint32 addr, uint16 data)
{
    for (;;)//wait until if these have one empty
    {
        if (ADI_FIFO_IS_FULL == 0)
        {
            break;
        }
    }

    REG32(REG_ADI_MST_BASE + 0x8000 + addr) = (uint32)data;
    rprintf("Write REG_ADI: 0x%x, val 0x%x\n",
        REG_ADI_MST_BASE + 0x8000 + addr, data);

    // wait write fifo empty after adi analog write
    while (ADI_FIFO_IS_EMPTY == 0)//wait until all data have been sent
    {
    }
}

uint16 CHIP_PHY_ADI_Read(uint32 addr)
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
    rd_addr = (adi_rd_data >> 16) & 0x7FFF;
    if(rd_addr != addr >> 2)
    {
        rprintf("high 15bits rd_data (0x%x) != (0x%x)addr\n",adi_rd_data,addr);/*assert verified*/
    }

    return((uint16)(adi_rd_data & ADI_MST_ARM_RD_CMD_X1(0xFFFF)));
}

PUBLIC void CHIP_PHY_ADI_init(void)
{
    uint32 value;

    /* initialize the adi registers */
    hwp_adiMst->adi_ctrl = 0;
    rprintf("Write REG_ADI: 0x%x, val 0x%x\n",
        &hwp_adiMst->adi_ctrl, 0);

    /* set the channel priority */
    hwp_adiMst->adi_pril = ADI_MST_CHNL0_PRI(0)|ADI_MST_CHNL1_PRI(1);
    rprintf("Write REG_ADI: 0x%x, val 0x%x\n",
        &hwp_adiMst->adi_pril,
        ADI_MST_CHNL0_PRI(0)|ADI_MST_CHNL1_PRI(1));

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
    rprintf("Write REG_ADI: 0x%x, val 0x%x\n",
        &hwp_adiMst->adi_cfg0, value);

    /* value = 0x80020143; */
    value = ADI_MST_RF_GSSI_STRTBIT_MODE |
            ADI_MST_RF_GSSI_DUMMY_LEN(2) |
            ADI_MST_RF_GSSI_CLK_DIV(1) |
            ADI_MST_RF_GSSI_NG_RX |
            ADI_MST_RF_GSSI_NG_TX;
    hwp_adiMst->adi_cfg1 = value;
    rprintf("Write REG_ADI: 0x%x, val 0x%x\n",
        &hwp_adiMst->adi_cfg1, value);

    /* set ADI clock auto gate */
    value = hwp_adiMst->adi_cfg0;
    value &= ~ADI_MST_RF_GSSI_SCK_ALL_ON;
    hwp_adiMst->adi_cfg0 = value;
    rprintf("Write REG_ADI: 0x%x, val 0x%x\n",
        &hwp_adiMst->adi_cfg0, value);
}

void CHIP_Reset_MCU(void)
{
    uint16 tmp_val;

    rprintf("CHIP_Reset_MCU\n");

    CHIP_PHY_ADI_init();                /* Init ADI */

    tmp_val = CHIP_PHY_ADI_Read(0xC08);
    tmp_val |= 0x4;
    CHIP_PHY_ADI_Write(0xC08, tmp_val); /* Enable WDG */

    CHIP_PHY_ADI_Write(0x60, 0xE551);   /* Unlock WDG */
    tmp_val = CHIP_PHY_ADI_Read(0x48);
    tmp_val |= 0x9;                     /* ANA_REG_OR(0x48, 0x9) */
    CHIP_PHY_ADI_Write(0x48, tmp_val);  /* Init WDG */

    CHIP_PHY_ADI_Write(0x70, 0x0);      /* Init IRQ Time */
    CHIP_PHY_ADI_Write(0x6C, 0x1FFFF);
    CHIP_PHY_ADI_Write(0x44, 0x0);      /* Init Reset Time */
    CHIP_PHY_ADI_Write(0x40, 0x50);

    tmp_val = CHIP_PHY_ADI_Read(0x48);
    tmp_val |= 0x2;                     /* ANA_REG_OR(0x48, 0x2) */
    CHIP_PHY_ADI_Write(0x48, tmp_val);  /* Start WDG */
}

uint32 timer_TimRealTickGet(uint32 timer_id)
{
    HWP_TIMER_T *timer_ctl;
    uint32 val = 0;

    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }

    switch(timer_id)
    {
        case TIMER_1:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER1_BASE);
            val = timer_ctl->HWTimer_CurVal;
            break;
        }
        case TIMER_2:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER2_BASE);
            val = timer_ctl->HWTimer_CurVal;
            break;
        }
    }

    return val;
}

/******************************************************************************
 * _Sdio0Enable
 ******************************************************************************/
 void _Sdio0Enable (void)
{
    /* Enable Sdio apb clock */
    //CLOCK_LOG_TRACE:"[_SdioEnable]"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1666_112_2_17_23_3_49_628,(uint8*)"");

    hwp_sysCtrl->CLK_SYS_AXI_ENABLE = SYS_CTRL_ENABLE_SYS_AXI_CLK_ID_SDMMC1;
}

/******************************************************************************
 * _Sdio0Disable
 ******************************************************************************/
 void _Sdio0Disable (void)
{
    /* Disable Sdio apb clock */
    //CLOCK_LOG_TRACE:"[_SdioDisable]"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1676_112_2_17_23_3_49_629,(uint8*)"");

    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_SYS_AXI_DISABLE = SYS_CTRL_DISABLE_SYS_AXI_CLK_ID_SDMMC1;
}

/******************************************************************************
 * _SdioReset
 ******************************************************************************/
void _SdioReset (void)
{
    //CLOCK_LOG_TRACE:"[_SdioReset]"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1685_112_2_17_23_3_49_630,(uint8*)"");

    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
	//reset adi module
    hwp_sysCtrl->AP_APB_RST_SET = SYS_CTRL_SET_AP_APB_RST_ID_SDMMC1;
    DELAY_CYCLE(1000)

    hwp_sysCtrl->AP_APB_RST_CLR = SYS_CTRL_CLR_AP_APB_RST_ID_SDMMC1;
    DELAY_CYCLE(1000)
}


/******************************************************************************
 * _LcmEnable
 ******************************************************************************/
 void _LcmEnable (void)
{
    /* Enable lcm ahb clock */
    hwp_sysCtrl->CLK_SYS_AHB_ENABLE = SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_LCD | SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_GOUDA;
    hwp_sysCtrl->CLK_AP_APB_ENABLE = SYS_CTRL_ENABLE_AP_APB_CLK_ID_LCD | SYS_CTRL_ENABLE_AP_APB_CLK_ID_GOUDA;
}

/******************************************************************************
 * _LcmDisable
 ******************************************************************************/
 void _LcmDisable (void)
{
    /* Disable lcm ahb clock */
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_SYS_AHB_DISABLE = SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_LCD | SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_GOUDA;
    hwp_sysCtrl->CLK_AP_APB_DISABLE = SYS_CTRL_DISABLE_AP_APB_CLK_ID_LCD | SYS_CTRL_DISABLE_AP_APB_CLK_ID_GOUDA;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _LcmReset
 ******************************************************************************/
 void _LcmReset (void)
{
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
	//reset adi module
    hwp_sysCtrl->SYS_AHB_RST_SET = SYS_CTRL_SET_SYS_AHB_RST_ID_LCD | SYS_CTRL_SET_SYS_AHB_RST_ID_GOUDA;
    DELAY_CYCLE(1000)

    hwp_sysCtrl->SYS_AHB_RST_CLR = SYS_CTRL_CLR_SYS_AHB_RST_ID_LCD | SYS_CTRL_CLR_SYS_AHB_RST_ID_GOUDA;
    DELAY_CYCLE(1000)
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

uint32 CHIP_GetArmClk (void)
{
    uint32 arm_clk = ARM_CLK_26M;

    uint32 clk_reg = REG32 (APB_CLK_CFG3);

    clk_reg = ( (clk_reg & CLK_MCU_SEL_MSK) >> CLK_MCU_SEL_SHIFT);

    switch (clk_reg)
    {
        case 0:
            arm_clk = ARM_CLK_26M;
            break;
        case 1:
            arm_clk = ARM_CLK_104M;
            break;
        case 2:
            arm_clk = ARM_CLK_208M;//CPLL
            break;
        case 3:
            arm_clk = ARM_CLK_208M;//APLL
            break;
        default:
            break;
    }

    return arm_clk;
}

uint32 CHIP_GetAhbClk (void)
{
    uint32 reg_val = REG32 (APB_CLK_CFG3);

    if (reg_val & CLK_MCU_RATIO)
    {
        return ARM_CLK_104M;
    }
    else
    {
        return (CHIP_GetArmClk() >> 1);
    }
}

uint8 INT_PHY_GetIrqIdStatus(uint32 int_num)
{
    return 0;
}

uint32 FDL_InEfsSpace(uint32 addr)
{
	return TRUE;
}

void FDL_SendErrLog(const char *x_format, ...)
{
    return;
}

void FDL_SendTrace(const char *x_format, ...)
{
    return;
}

void FDL_PacketDoIdle(void)
{
    return;
}

uint32 Get_fdl_file_end_addr(void)
{
    return 0xFFFFFFFF;
}

PUBLIC CLOCK_STATUS DEVICE_Open (DEVICE_HANDLE thiz)
{
    return CLOCK_SUCCESS;
}

PUBLIC CLOCK_STATUS DEVICE_Close (DEVICE_HANDLE thiz)
{
   return  CLOCK_SUCCESS;
}

PUBLIC CLOCK_STATUS DEVICE_Reset (DEVICE_HANDLE thiz)
{
   return  CLOCK_SUCCESS;
}

PUBLIC CLOCK_STATUS DEVICE_Find (const char *Name, DEVICE_HANDLE *pDevObj)
{
   return  CLOCK_SUCCESS;
}

PUBLIC uint32 DEVICE_GetClock (DEVICE_HANDLE thiz)
{
    return 0;
}

PUBLIC uint32 DEVICE_GetClockDiv (DEVICE_HANDLE thiz)
{
    return 1;
}

PUBLIC uint32 OS_GetTickCount(void)
{
    return 0;
}

void INT_HAL_IRQEnable (uint32 logic_num)
{
    return;
}

void INT_HAL_IRQDisable (uint32 logic_num)
{
    return;
}

PUBLIC void ISR_RegHandler (void)
{
    return;
}

PUBLIC void ISR_UnRegHandler (void)
{
    return;
}

PUBLIC uint32 OS_TickDelay(uint32 ticks)
{
    return 0;
}

void SCI_CreateMutex (void)
{
    return;
}

void SCI_DeleteMutex (void)
{
    return;
}

PUBLIC BOOLEAN SCI_InThreadContext (void)
{
    return TRUE;
}

void SCI_GetMutex (void)
{
    return;
}
void SCI_PutMutex (void)
{
    return;
}
void SCI_IdentifyThread (void)
{
    return;
}
void SCI_GetSignal (void)
{
    return;
}
void SCI_SendSignal (void)
{
    return;
}

typedef void    (*TIMER_FUN)(uint32);

PUBLIC void SCI_CreateTimer(void)
{
    return;
}

PUBLIC void SCI_DeactiveTimer (void)
{
    return;
}

PUBLIC void  SCI_CreatePeriodTimer (void)
{
    return;
}

PUBLIC void SCI_GetTimerInfo(void)
{
    return;
}

PUBLIC void SCI_ChangeTimer (void)
{
    return;
}

PUBLIC void SCI_ActiveTimer (void)
{
    return;
}

PUBLIC void SCI_DeleteTimer (void)
{
    return;
}

PUBLIC void SCI_IsTimerActive (void)
{
    return;
}

PUBLIC uint32 SCI_GetSemaphore (//if successful return SCI_SUCCESS,
                                     //else return SCI_ERROR
    void * sem_ptr,//semaphore pointer
    uint32 wait_option       //wait option: SCI_WAIT_FOREVER, SCI_NO_WAIT,
)
{
    return 0;
}

PUBLIC uint32 SCI_PutSemaphore (
    void * sem_ptr
)
{
    return 0;
}

PUBLIC void SCI_Assert_NoDebug (void)
{
    return;
}

uint32 CHIP_PHY_GetECOFlag(void)
{
    return hwp_rfReg->chip_id_reg2;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

