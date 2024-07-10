/******************************************************************************
 ** File Name:      sleep_phy_v0.c                                          *
 ** Author:         Richard Yang                                              *
 ** DATE:           08/25/2003                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 DSP and MCU for time compensate of the bug for DSP.       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/25/2003     Richard.Yang     Create.                                   *

******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *

**---------------------------------------------------------------------------*/
#include "os_api.h"
#include "sci_api.h"
#include "chip_plf_export.h"
#include "../sleep_phy.h"
#include "sleep_phy_v0.h"

#include "task_monitor.h"
#include "pin_sleep.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#include "fiq_drvapi.h"
#include "dal_audio.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *

**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/*----------------------------------------------------------------------------*
**                            Macro Definitions                               *
**---------------------------------------------------------------------------*/


LOCAL uint32 reg_gen0_val, reg_gen1_val, reg_gr_vb_ctl, reg_misc0_val, reg_slp_ctl_val;
LOCAL uint32 reg_ahb_ctl0_val = 0;

// Define mask for register save
#define GR_GEN0_MASK    (GEN0_ADC_EN|GEN0_TPC_EN|GEN0_WDG_EN|GEN0_TIMER_EN|    \
                         GEN0_RTC_EN|GEN0_KPD_EN|GEN0_GPIO_EN|GEN0_SYST_EN|    \
                         GEN0_PIN_EN)
#define GR_GEN1_MASK    (GEN1_UART0_EN|GEN1_UART2_EN|GEN1_IIS_EN|              \
                         GEN1_PWM0_EN|GEN1_PWM1_EN|GEN1_PWM2_EN|GEN1_PWM3_EN|  \
                         GEN1_PWM4_EN|GEN1_SPI_EN|GEN1_I2C_EN|GEN1_I2C1_EN|                 \
                         GEN1_EFUSE_EN|GEN1_GEA_EN|GEN1_SIM0_EN|GEN1_SIM1_EN)
#define GR_MISC0_MASK   (MISC0_APB_FORCE_SLEEP|MISC0_APB_FORCE_ON)
#define GR_VB_CTL_MASK  (ARM_VB_MCLKON|ARM_VB_DA0ON|ARM_VB_DA1ON|ARM_VB_ADCON| \
                         ARM_VB_ANAON|ARM_VB_ACC)
#define GR_SLP_CTL_MASK (SLPCTRL_FSM_BGLUPD_EN)
#define AHB_CTL0_MASK   (AHB_CTL0_CCIR_EN|AHB_CTL0_DCAM_EN|AHB_CTL0_LCM_EN|    \
                         AHB_CTL0_DMA_EN|AHB_CTL0_SDIO_EN|AHB_CTL0_LCDC_EN|    \
                         AHB_CTL0_ROT_EN|AHB_CTL0_NFC_EN|AHB_CTL0_BM0_EN|      \
                         AHB_CTL0_BM1_EN|AHB_CTL0_BM2_EN)


// Register save
#define SAVE_REG(_reg_save, _reg_addr, _reg_mask)  {_reg_save = ( (REG32(_reg_addr)) & ((uint32)_reg_mask));}
#define SAVE_GLOBAL_REG  do{SAVE_REG(reg_gen0_val, GR_GEN0, GR_GEN0_MASK);     \
        SAVE_REG(reg_gen1_val, GR_GEN1, GR_GEN1_MASK);        \
        SAVE_REG(reg_gr_vb_ctl, GR_VB_CTL, GR_VB_CTL_MASK);   \
        SAVE_REG(reg_misc0_val, GR_MISC0, GR_MISC0_MASK);     \
        SAVE_REG(reg_slp_ctl_val, GR_SLP_CTL, GR_SLP_CTL_MASK);\
        SAVE_REG(reg_ahb_ctl0_val, AHB_CTL0, AHB_CTL0_MASK);\
    }while(0)

// Register restore
#define REG_LOCAL_VALUE_DEF   uint32 reg_val;
#define RESTORE_REG(_reg_addr, _reg_mask, _reg_save)    do{reg_val = REG32(_reg_addr); \
        reg_val &= ~((uint32)_reg_mask);    \
        reg_val |= (((uint32)_reg_save) & ((uint32)_reg_mask));    \
        CHIP_REG_SET(_reg_addr, reg_val);}while(0)

#define RESTORE_GLOBAL_REG   do{  \
        RESTORE_REG(GR_VB_CTL, GR_VB_CTL_MASK, reg_gr_vb_ctl); \
        RESTORE_REG(GR_GEN1, GR_GEN1_MASK, reg_gen1_val); \
        RESTORE_REG(GR_GEN0, GR_GEN0_MASK, reg_gen0_val); \
        RESTORE_REG(GR_MISC0, GR_MISC0_MASK, reg_misc0_val);\
        RESTORE_REG(GR_SLP_CTL, GR_SLP_CTL_MASK, reg_slp_ctl_val);\
        RESTORE_REG(AHB_CTL0, AHB_CTL0_MASK, reg_ahb_ctl0_val);\
    }while(0)

// Clear the auto close flag of rfldo and xtlen
#define XTL_RF_FORCEON do{\
        CHIP_REG_OR(GR_MISC1,  APBMISC1_MCU_XTL_FORCE_EN);\
        CHIP_REG_AND(GR_SLP_CTL, ~SLPCTRL_FSM_LDORFPD_EN);}while(0)

// Set the auto close flag of rfldo and xtlen
#define XTL_RF_AUTOOFF do{\
        CHIP_REG_AND(GR_MISC1, ~APBMISC1_MCU_XTL_FORCE_EN);\
        CHIP_REG_OR(GR_SLP_CTL, SLPCTRL_FSM_LDORFPD_EN);}while(0)


/**---------------------------------------------------------------------------*
 **                         Global Variables                                                                                    *
 **---------------------------------------------------------------------------*/
PUBLIC uint32  RF_enable_flag              = 0;

// Status flag indicate whether MCU is in sleep or not
/*lint -e552 for symbol "MCU_is_sleep" */
PUBLIC uint32  MCU_is_sleep                = 0x0;

PUBLIC uint32  MCU_is_wakeup               = 0x0;
PUBLIC uint32  MCU_is_waiting_tick         = 0x0;
PUBLIC uint32  s_dma_sleep_mode     = 0x0;

LOCAL BOOLEAN     is_deepsleep      = FALSE;


/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL void SLEEP_RestoreReg (void);

/**---------------------------------------------------------------------------*
 **                     Global Function Prototypes                            *
 **---------------------------------------------------------------------------*/
extern int     SCI_BeforeEnterDeepSleep (void);
extern int     SCI_BeforeEnterHalfDeepSleep (void);
extern int     SCI_AfterExitDeepSleep (void);

/*----------------------------------------------------------------------------*
**                         Function Definitions                                                                                *
**--------------------------------------------------------------------------- */
PUBLIC void SLEEP_PHY_PCM_EnableSleep (uint32 mode)
{
    if (mode)
    {
        sleep_active_modules &= ~SLEEP_DRV_PCM;
        sleep_apb_active_modues &= ~SLEEP_APB_IIS;
    }
    else
    {
        sleep_active_modules |= SLEEP_DRV_PCM;
        sleep_apb_active_modues |= SLEEP_APB_IIS;
    }
}

PUBLIC void SLEEP_PHY_SIM_EnableSleep (uint32 mode)
{
    if (mode)
    {
        sleep_active_modules &= ~SLEEP_DRV_SIM;
        sleep_apb_active_modues &= ~SLEEP_APB_SIM;
    }
    else
    {
        sleep_active_modules |= SLEEP_DRV_SIM;
        sleep_apb_active_modues |= SLEEP_APB_SIM;
    }
}

PUBLIC void SLEEP_PHY_TPC_EnableSleep (uint32 mode)
{
    if (mode)
    {
        sleep_apb_active_modues &= ~SLEEP_APB_TPC;
    }
    else
    {
        sleep_apb_active_modues |= SLEEP_APB_TPC;
    }
}

/*****************************************************************************
 *  Description:    this function is used to disable any module if they are  *
 *                  not used now                                             *
 *  Global Resource Dependence:                                              *
 *  Author: jiexia.yu                                                        *
 *  Note:   return SCI_TRUE when module was disabled, else return SCI_FALSE  *
******************************************************************************/
LOCAL BOOLEAN SLEEP_DisableAudioModule (void)
{
    BOOLEAN is_disabled = SCI_FALSE;

    if (! (sleep_active_modules & SLEEP_DRV_AUDIO))
    {
        //turn off VB clock gate & cut off data path
        CHIP_REG_AND (GR_VB_CTL,
                      ~ (ARM_VB_MCLKON|ARM_VB_DA0ON|ARM_VB_DA1ON|ARM_VB_ADCON|ARM_VB_ANAON));
        //ensure VB controlled by ARM
        CHIP_REG_OR (GR_VB_CTL, ARM_VB_ACC);

        is_disabled = SCI_TRUE;
    }

    return is_disabled;
}

/*****************************************************************************
 *  Description:    this function is used to disable the clock to APB module *
 *                  if they are not used now                                 *
 *  Global Resource Dependence:                                              *
 *  Author: jiexia.yu                                                        *
 *  Note:   return SCI_TRUE when module was disabled, else return SCI_FALSE  *
******************************************************************************/
LOCAL BOOLEAN SLEEP_DisableAPBModule (void)
{
    BOOLEAN is_disabled = SCI_FALSE;

    if (!sleep_apb_active_modues)
    {
        // Disable apb_force_on
        REG32 (GR_MISC0) &= ~MISC0_APB_FORCE_ON;

        // Enable APB force sleep
        CHIP_REG_OR (GR_MISC0, MISC0_APB_FORCE_SLEEP);

        // Clear apb device enable bit
        CHIP_REG_AND (GR_GEN0, ~ (GR_GEN0_MASK));
        CHIP_REG_AND (GR_GEN1, ~ (GR_GEN1_MASK));

        is_disabled = SCI_TRUE;
    }

    return is_disabled;
}

/*****************************************************************************
 *  Description:    this function is used to disable the clock to AHB module *
 *                  if they are not used now                                 *
 *  Global Resource Dependence:                                              *
 *  Author: Jeff.Li                                                          *
 *  Note:   return SCI_TRUE when module was disabled, else return SCI_FALSE  *
******************************************************************************/
LOCAL void SLEEP_DisableAHBModule (void)
{
    if (!sleep_ahb_active_modules)
    {
        CHIP_REG_AND (AHB_CTL0, ~ (AHB_CTL0_MASK));
    }
}

/*****************************************************************************
 *  Description:    deepsleep init at system startup time                    *
 *  Global Resource Dependence:                                              *
 *  Author:                                                                  *
 *  Note:                                                                    *
******************************************************************************/
PUBLIC void SLEEP_Init (void)
{
    volatile uint32 temp;

    // Get the sleep table entry
    s_slp_ctl_tab = Slp_Get_Cfg();

    // Config all sleep level common parameters
    Slp_Reg_Cfg (0,SLP_CFG_COM);

    // Deepsleep flag init
    is_deepsleep  = SCI_FALSE;

    // Switch contorlling of XTL and PLL wait time to DSP
    temp = CHIP_REG_GET (GR_MISC0);
    temp |= (uint32) (MISC0_PLL_WAIT_SEL | MISC0_XTL_WAIT_SEL | MISC0_XTLBUF_WAIT_SEL);
    CHIP_REG_SET (GR_MISC0, temp);

    //otp enable.
    CHIP_REG_OR (GR_LDO_PD, GR_LDO_PD_OTP_EN_RST);
}

/*****************************************************************************/
//  Description:    This function is used to set system into sleep status.
//  Author:
//  Note:
//****************************************************************************/
PUBLIC void SCI_EnterDeepSleep (void)
{
#ifndef  JTAG_SUPPORT
    SLP_AHB_APB_TYPE_E  slp_index = 0;

    TX_INTERRUPT_SAVE_AREA
    TX_DISABLE_FIQ_IRQ

    // Send enter sleep request
    TM_SendSleepRequest (TM_ENTER_SLEEP_REQ);

    // Get slp status
    slp_index = Slp_Get_Slp_Status();

    // Save global register value
    SAVE_GLOBAL_REG;

    switch (SCI_Getsleepmode())
    {
        case SLEEP_DRV_NONE:
            {
                // Auto PD BG(analog) to reduce sleep current
                CHIP_REG_OR (GR_SLP_CTL, SLPCTRL_FSM_BGLUPD_EN);
            }
            //lint -fallthrough
        case SLEEP_DRV_LCD:
        case SLEEP_DRV_LINEIN:
        case SLEEP_DRV_LCD|SLEEP_DRV_LINEIN:
            {
                if (slp_index == AHB_CAN_SLP_APB_CAN_SLP)
                {
                    // Enter deepsleep
                    is_deepsleep = SCI_TRUE;

                    // Add customre sleep routine here
                    SCI_BeforeEnterDeepSleep();

                    if (RF_enable_flag)
                    {
                        XTL_RF_FORCEON;
                    }
                    else
                    {
                        XTL_RF_AUTOOFF;
                    }
                }
                else
                {
                    is_deepsleep = SCI_FALSE;
                    XTL_RF_FORCEON;
                }

                SLEEP_DisableAudioModule();

                if ( (slp_index == AHB_CAN_SLP_APB_CAN_SLP) ||
                        (slp_index == AHB_NO_SLP_APB_CAN_SLP))
                {
                    // Disable APB related moudles, in order to enter further
                    // deep sleep state
                    SLEEP_DisableAPBModule();
                }

                if ( (slp_index == AHB_CAN_SLP_APB_CAN_SLP) ||
                        (slp_index == AHB_CAN_SLP_APB_NO_SLP))
                {
                    // Disable AHB masters, to reduce the power consume by clock
                    // tree
                    SLEEP_DisableAHBModule();
                }
            }
            break;

            // When playing MP3, MIDI etc, the system can enter half-deepsleep to
            // save power
        case SLEEP_DRV_MMI|SLEEP_DRV_AUDIO:
        case SLEEP_DRV_AUDIO:
        case SLEEP_DRV_BTCLK|SLEEP_DRV_AUDIO:
        case SLEEP_DRV_BTCLK|SLEEP_DRV_MMI|SLEEP_DRV_AUDIO:
        case SLEEP_DRV_BTCLK:
        case SLEEP_DRV_CMMB:
        case SLEEP_DRV_WIFI:
        case SLEEP_DRV_CMMB|SLEEP_DRV_WIFI:
            {
                is_deepsleep = SCI_FALSE;
                SCI_BeforeEnterHalfDeepSleep();
                XTL_RF_FORCEON;

                if ( (slp_index == AHB_CAN_SLP_APB_CAN_SLP) ||
                        (slp_index == AHB_CAN_SLP_APB_NO_SLP))
                {
                    // Disable AHB masters, to reduce the power consume by clock
                    // tree
                    SLEEP_DisableAHBModule();
                }
            }
            break;

            // Enter ARM core sleep
        default:
            {
                is_deepsleep = SCI_FALSE;
                slp_index = AHB_NO_SLP_APB_NO_SLP;
                XTL_RF_FORCEON;
            }
    }

    TM_SendTestPointRequest (slp_index,   SCI_Getsleepmode());
    TM_SendTestPointRequest (SLEEP_GetAHBModuleStatus(), SLEEP_GetAPBModuleStatus());
    TM_SendTestPointRequest (REG32 (AHB_SLEEP_STS),    REG32 (GR_STC_STATUS) );
    
    // Enter different sleep routine according to ahb and apb state
    SCI_Enter_SleepRoutine (slp_index);

    //to check share memory flag whether need to hold arm
    if(SCI_IsEnalbeArmHold())
    {
        DSP_HoldArmByFIQ();
    }
    
    SLEEP_RestoreReg();
    TX_RESTORE
#endif
}

void SCI_RTC_TimerHandler (void)
{
    // Check if MCU is wakeup by other interrupt
}

LOCAL void SLEEP_RestoreReg (void)
{
    REG_LOCAL_VALUE_DEF

    RESTORE_GLOBAL_REG;

    if (is_deepsleep)
    {
        SCI_AfterExitDeepSleep();
    }

    is_deepsleep = SCI_FALSE;

    TM_SendSleepRequest (TM_LEAVE_SLEEP_REQ);

    MCU_is_sleep = 0;
}

/*****************************************************************************/
// Description :    This function is used to set the flag which indicate if xtl
//                  need open in deepsleep
// Global resource dependence :s_sleep_xtl_open_cnt
// Author :         yuhua.shi
// Note :
/*****************************************************************************/
PUBLIC void DPSLP_XTLOpenInSleep (
    BOOLEAN is_open             //indicated if xtl need open in deepsleep,
    //SCI_TRUE: need open xtl in deepsleep; SCI_FALSE: can close xtl in deepsleep
)
{
    //do nothing
}

/*****************************************************************************/
// Description :    This function is used to production test
// Author :         hao.liu
// Note :
/*****************************************************************************/
PUBLIC void SCI_Deep_Sleep_Test (void)
{
    int  loop;

    //make sure all deepsleep condition met
    RF_enable_flag = 0;
    sleep_active_modules = 0;
    sleep_apb_active_modues = 0;
    sleep_ahb_active_modules = 0;

    SCI_EnterDeepSleep();

    for (loop = 0; loop < 0x4000; loop++)
    {
    }

    // for we has been delay a little time after set the deep sleep bit,
    // so shouldn't reach here.
    // If reach here, we need find the reason.
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *

**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif //End of file
