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
#include "sc6531efm_reg_global.h"
#include "sys_ctrl.h"
#include "adi_reg_uws6121e.h"
#include "uws6121e_timer_cfg.h"
#include "uws6121e_reg_analog.h"

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


BOOLEAN g_LdoEmmcPwrOffFlag = FALSE;

/**---------------------------------------------------------------------------*
**                         Compiler Flag
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif

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

#define ADI_FIFO_IS_EMPTY   (hwp_adiMst->arm_cmd_status & ADI_MST_WFIFO_EMPTY)
#define ADI_FIFO_IS_FULL    (hwp_adiMst->arm_cmd_status & ADI_MST_WFIFO_FULL)

#define SCI_32TICKTOMILLISECOND(_TICK)    (((uint64)_TICK *1000) / 32768)

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
//	Note:
/*****************************************************************************/
PUBLIC void SCI_Sleep(
    uint32  time_in_ms      // Number of milliseconds to sleep.
    )

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

PUBLIC void ADI_Phy_init (void)
{
    uint32 value;

    rprintf("Bootloader ADI_Phy_init...\n");

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

#define HW_RST_MASK       		0x1F
#define ADI_MST_ARM_RD_DATA_MASK (0xFFFF)
#define ADI_MST_ARM_RD_ADDR_SHIFT(n)  (((n>>16)&0x7FFF))

PUBLIC uint32 CHIP_PHY_GetANAReg(uint32 addr)
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
    if (rd_addr != addr >> 2)
    {
        rprintf("high 15bits rd_data (0x%x) != (0x%x)addr\n",adi_rd_data,addr);/*assert verified*/
    }

    return((uint16)(adi_rd_data & ADI_MST_ARM_RD_CMD_X1(0xFFFF)));
}

uint32 CHIP_PHY_GetHwRstFlag (void)
{
    return (CHIP_PHY_GetANAReg(ANA_POR_RST_MONITOR) & HW_RST_MASK);
}

uint32 timer_TimRealTickGet(uint32 timer_id)
{
    HWP_TIMER_T_1 *timer_ctl_1;
    HWP_TIMER_T_2_5 *timer_ctl_2;
    uint32 val = 0;

    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }

    switch(timer_id)
    {
        case TIMER_1:
        {
            timer_ctl_1 = (HWP_TIMER_T_1*)(REG_TIMER1_BASE);
            val = timer_ctl_1->HWTimer_CurVal;
            break;
        }
        case TIMER_2:
        {
            timer_ctl_2 = (HWP_TIMER_T_2_5*)(REG_TIMER2_BASE);
            val = timer_ctl_2->hwtimer_curval_l;
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

    //hwp_sysCtrl->CLK_SYS_AXI_ENABLE = SYS_CTRL_ENABLE_SYS_AXI_CLK_ID_SDMMC1;
}

/******************************************************************************
 * _Sdio0Disable
 ******************************************************************************/
 void _Sdio0Disable (void)
{
    /* Disable Sdio apb clock */
    //CLOCK_LOG_TRACE:"[_SdioDisable]"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1676_112_2_17_23_3_49_629,(uint8*)"");

    //hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    //hwp_sysCtrl->CLK_SYS_AXI_DISABLE = SYS_CTRL_DISABLE_SYS_AXI_CLK_ID_SDMMC1;
}

/******************************************************************************
 * _SdioReset
 ******************************************************************************/
void _SdioReset (void)
{
    //CLOCK_LOG_TRACE:"[_SdioReset]"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1685_112_2_17_23_3_49_630,(uint8*)"");

    //hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
	//reset adi module
    //hwp_sysCtrl->AP_APB_RST_SET = SYS_CTRL_SET_AP_APB_RST_ID_SDMMC1;
    DELAY_CYCLE(1000)

    //hwp_sysCtrl->AP_APB_RST_CLR = SYS_CTRL_CLR_AP_APB_RST_ID_SDMMC1;
    DELAY_CYCLE(1000)
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

PUBLIC uint32 OS_GetTickCount(void)
{
    return (SCI_32TICKTOMILLISECOND(hwp_idleLps->lps_32k_ref));
}

void FDL_PacketDoIdle(void)
{
    return;
}
void FDL_SendErrLog(const char *x_format, ...)
{
	return;
}
uint32 Get_fdl_file_end_addr(void)
{
    return 0;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

