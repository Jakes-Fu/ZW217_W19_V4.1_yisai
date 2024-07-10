/******************************************************************************
 ** File Name:      sleep_phy_v5.c                                          *
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
#include "sleep_phy_v5.h"
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
LOCAL uint32 reg_apb_eb0_val=0,  reg_apb_peri_ctl0_ctl=0;
LOCAL uint32 reg_ahb_eb0_val = 0;

// Define mask for register save
#define APB_EB0_MASK    (I2C1_EB|FM_EB|AUD_EB|EFUSE_EB|    \
                         EIC_EB|TMR_EB|PIN_REG_EB|IIS_EB|SPI1_EB|    \
                         UART1_EB|UART0_EB|SYST_EB|GEA_EB|GPIO_EB|KPD_EB|RTC_EB|\
                         PWM_EB|I2C_EB|SIM_EB|TMRBK_EB|WDG_EB)
//#define APB_SLP_CTL0_MASK   (APB_FORCE_SLEEP|APB_FORCE_ON)
#define APB_PERI_CTL0_MASK  (ARM_VB_MCLKON|ARM_VB_DA0ON|ARM_VB_DA1ON|ARM_VB_ADC0ON|ARM_VB_ADC1ON| \
                             ARM_VB_ANAON|ARM_VB_ACC)
#define AHB_EB0_MASK   (DMA_EB|DCAM_EB|BUSMON0_EB|    \
                        BUSMON1_EB|USBD_EB|TIC_EB|    \
                        LCM_EB|      \
                        SDIO_EB|BUSMON2_EB|LCDC_EB|MIDI_EB |   \
                        SPI0_EB|ROTA_EB|ZIP_EB)
// Register save
#define SAVE_REG(_reg_save, _reg_addr, _reg_mask)  {_reg_save = ( (REG32(_reg_addr)) & ((uint32)_reg_mask));}
#define SAVE_GLOBAL_REG  do{\
        SAVE_REG(reg_apb_eb0_val, APB_EB0, APB_EB0_MASK);     \
        SAVE_REG(reg_apb_peri_ctl0_ctl, APB_PERI_CTL0, APB_PERI_CTL0_MASK);   \
        SAVE_REG(reg_ahb_eb0_val, AHB_EB0, AHB_EB0_MASK);\
    }while(0)

// Register restore
#define REG_LOCAL_VALUE_DEF   uint32 reg_val;
#define RESTORE_REG(_reg_addr, _reg_mask, _reg_save)    do{reg_val = REG32(_reg_addr); \
        reg_val &= ~((uint32)_reg_mask);    \
        reg_val |= (((uint32)_reg_save) & ((uint32)_reg_mask));    \
        CHIP_REG_SET(_reg_addr, reg_val);}while(0)

#define RESTORE_GLOBAL_REG   do{  \
        RESTORE_REG(APB_PERI_CTL0, APB_PERI_CTL0_MASK, reg_apb_peri_ctl0_ctl); \
        RESTORE_REG(APB_EB0, APB_EB0_MASK, reg_apb_eb0_val); \
        RESTORE_REG(AHB_EB0, AHB_EB0_MASK, reg_ahb_eb0_val);\
    }while(0)

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

#define SLEEP_DEBUG

#ifdef SLEEP_DEBUG
PUBLIC uint32 s_sleep_step = 0;
#endif
/**---------------------------------------------------------------------------*
 **                     Global Function Prototypes                            *
 **---------------------------------------------------------------------------*/
extern void     SCI_BeforeEnterDeepSleep (void);
extern void     SCI_BeforeEnterHalfDeepSleep (void);
extern void     SCI_AfterExitDeepSleep (void);

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
        CHIP_REG_AND (APB_PERI_CTL0,
                      ~ (ARM_VB_MCLKON|ARM_VB_DA0ON|ARM_VB_DA1ON|ARM_VB_ADC0ON|ARM_VB_ADC1ON|
                         ARM_VB_ANAON));
        //ensure VB controlled by ARM
        CHIP_REG_OR (APB_PERI_CTL0, ARM_VB_ACC);

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
        REG32 (APB_SLP_CTL0) &= ~APB_FORCE_ON;

        // Clear apb device enable bit
        CHIP_REG_AND (APB_EB0, ~ (APB_EB0_MASK));

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
        CHIP_REG_AND (AHB_EB0, ~ (AHB_EB0_MASK));
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
    // Get the sleep table entry
    s_slp_ctl_tab = Slp_Get_Cfg();

    // Config all sleep level common parameters
    Slp_Reg_Cfg (0,SLP_CFG_COM);

    // Deepsleep flag init
    is_deepsleep  = SCI_FALSE;

    ANA_REG_OR (ANA_SLP_LDO_CTL2, SLP_PWR_SET_EN);  //Set to 1 will enable all LDOs and DCDC sleep control

    // Switch contorlling of XTL and PLL wait time to DSP
    // CHIP_REG_AND (APB_XTL_CTL0,~XTL_WAIT_SEL);
    // CHIP_REG_AND (APB_PLL_CTL1, ~PLL_WAIT_SEL);

    /*
        CHIP_REG_OR (APB_XTL_CTL0,XTL_WAIT_SEL);
        CHIP_REG_OR (APB_PLL_CTL1, PLL_WAIT_SEL);
        CHIP_REG_OR (APB_XTL_CTL0, MCU_XTLEN_AUTOPD_EN);

       {
        uint32 temp = CHIP_REG_GET(APB_XTL_CTL0);
        temp &= (~(XTL_WAIT_MSK | XTL_BUF_WAIT_MSK));
        temp |= (120 <<XTL_WAIT_SHIFT ) | (30 <<XTL_BUF_WAIT_SHIFT );
        CHIP_REG_SET(APB_XTL_CTL0, temp);
       }
        ANA_REG_SET(ANA_XTL_CTL0, 100);

       {
        uint32 temp = CHIP_REG_GET(APB_PLL_CTL1);
        temp &= (~PLL_WAIT_MSK);
        temp |= (64 << PLL_WAIT_SHIFT);
        CHIP_REG_SET(APB_PLL_CTL1, temp);
       }
    */

    CHIP_REG_OR (APB_MISC_CTL0, BT_XTL_EN_IE);

    {
        uint32 temp = CHIP_REG_GET (APB_XTL_CTL0);
        temp &= (~ (XTL_BUF_WAIT_MSK));
        temp |= (32 <<XTL_BUF_WAIT_SHIFT);
        CHIP_REG_SET (APB_XTL_CTL0, temp);
    }
    ANA_REG_SET (ANA_XTL_CTL0, 32);

#ifdef VDDCORE_SUPPLY_LDO
    ANA_REG_AND (ANA_LDO_PD_RST0, ~DCDC_PD_RST); //dcdc pd
    ANA_REG_OR (ANA_LDO_PD_SET0, DCDC_PD);
#else
    ANA_REG_AND (ANA_LDO_PD_RST0, ~LDO_CORE_PD_RST); //core ldo pd
    ANA_REG_OR (ANA_LDO_PD_SET0, LDO_CORE_PD);
#endif

    ANA_REG_OR (ANA_SLP_LDO_CTL2, SLP_PWR_SET_EN);
    ANA_REG_AND (ANA_SLP_LDO_CTL0, ~ (XTL_SLP_LDOABB_PD_EN|SLP_LDOABB_PD_EN|SLP_LDOMEMS_PD_EN));
    
    //CHIP_REG_OR (CHIP_PWR_CTL0, MEM_SLEEP_PD_EN_MSK);   //Iram goto sleep mode

    ANA_REG_AND (ANA_DCDC_CTL2, ~DCDC_PFM_CLR); //close PFM
    ANA_REG_OR (ANA_DCDC_CTL2, DCDC_PFM_SET);

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

#ifdef SLEEP_DEBUG
    s_sleep_step = 0;
#endif

    // Send enter sleep request
    TM_SendSleepRequest (TM_ENTER_SLEEP_REQ);

    // Get slp status
    slp_index = Slp_Get_Slp_Status();

    // Save global register value
    SAVE_GLOBAL_REG;

    switch (SCI_Getsleepmode())
    {
        case SLEEP_DRV_NONE:
            //lint -fallthrough
        case SLEEP_DRV_LCD:
        case SLEEP_DRV_LINEIN:
        case SLEEP_DRV_LCD|SLEEP_DRV_LINEIN:
            {
#ifdef SLEEP_DEBUG
                s_sleep_step |= BIT_0;
#endif

                if (slp_index == AHB_CAN_SLP_APB_CAN_SLP)
                {
                    // Enter deepsleep
                    is_deepsleep = SCI_TRUE;

                    // Add customre sleep routine here
                    SCI_BeforeEnterDeepSleep();
                }
                else
                {
                    is_deepsleep = SCI_FALSE;
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
            {
#ifdef SLEEP_DEBUG
                s_sleep_step |= BIT_1;
#endif
                is_deepsleep = SCI_FALSE;
                SCI_BeforeEnterHalfDeepSleep();

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
#ifdef SLEEP_DEBUG
                s_sleep_step |= BIT_2;
#endif
                is_deepsleep = SCI_FALSE;
                slp_index = AHB_NO_SLP_APB_NO_SLP;
            }
    }

#ifdef SLEEP_DEBUG
    s_sleep_step |= BIT_3;
    TM_SendTestPointRequest (slp_index,   SCI_Getsleepmode());
    TM_SendTestPointRequest (SLEEP_GetAHBModuleStatus(), SLEEP_GetAPBModuleStatus());
    TM_SendTestPointRequest (REG32 (AHB_SLP_STS0),    REG32 (APB_SLP_STS0));
    s_sleep_step |= BIT_4;
#endif

    // Enter different sleep routine according to ahb and apb state
    SCI_Enter_SleepRoutine (slp_index);

#ifdef SLEEP_DEBUG
    s_sleep_step |= BIT_7;
#endif

    REG32 (AHB_SLP_CTL1) &=  ~ (MCU_DEEP_SLP_EN | MCU_SYS_SLP_EN);

    //to check share memory flag whether need to hold arm

    if (SCI_IsEnalbeArmHold())
    {
        DSP_HoldArmByFIQ(HOLD_ARM_TIME_BY_POLLING);
    }

#ifdef SLEEP_DEBUG
    s_sleep_step |= BIT_8;
#endif

    SLEEP_RestoreReg();

#ifdef SLEEP_DEBUG
    s_sleep_step |= BIT_9;
#endif

    TX_RESTORE

#ifdef SLEEP_DEBUG
    s_sleep_step |= BIT_10;
#endif

#endif
}

void SCI_RTC_TimerHandler (void)
{
    // Check if MCU is wakeup by other interrupt
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
