/******************************************************************************
 ** File Name:      dsp_sleep_v3.c                                               *
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
#include "sci_types.h"
#include "sci_api.h"
#include "chip_plf_export.h"
#include "ref_param.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
//#include "fiq_drvapi.h"
#include "task_monitor.h"
#include "dal_audio.h"
//#include "com_drvapi.h"
//#include "chip_drvapi.h"
//#include "syscnt_drvapi.h"
#include "pin_sleep.h"
#include "../sleep_phy.h"
#include "sleep_phy_v3.h"
#include "../../mmu/mmu_phy.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                                                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
#define DS_TASK_TRACK
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
**---------------------------------------------------------------------------*/

#define DSP_OVERRUN_REQ             0xffff
#define DSP_DEACTIVE_WAKEUP_REQ     0x3
#define DSP_SLEEP_START             0x0
#define DSP_END_WAKEUP              0x2
#define DEEPSLEEP_RETETION_ON       0x3 // this value is from nv

LOCAL uint32 reg_gen0_val, reg_busclk_alm, reg_ahb_ctl0_val, reg_gen_clk_en, reg_gen_clk_gen5, reg_ahb_ctl3;

//define mask for register save
#define GR_GEN0_MASK    (GEN0_SIM0_EN | GEN0_I2C_EN  | GEN0_GPIO_EN | \
                         GEN0_EFUSE_EN  | GEN0_I2S_EN | GEN0_PIN_EN | \
                         GEN0_EPT_EN | GEN0_SIM1_EN | GEN0_SPI_EN |GEN0_UART0_EN | \
                         GEN0_UART1_EN |   GEN0_UART2_EN)

#define GR_CLK_EN_MASK (CLK_PWM0_EN | CLK_PWM1_EN | CLK_PWM2_EN | CLK_PWM3_EN)
#define BUSCLK_ALM_MASK (ARM_VB_MCLKON|ARM_VB_DA0ON|ARM_VB_DA1ON|ARM_VB_ADCON|ARM_VB_ANAON|ARM_VB_ACC)

#define AHB_CTL0_MASK   (AHB_CTL0_DCAM_EN|AHB_CTL0_CCIR_EN|AHB_CTL0_LCDC_EN|    \
                         AHB_CTL0_SDIO_EN|AHB_CTL0_DMA_EN|     \
                         AHB_CTL0_BM0_EN |AHB_CTL0_NFC_EN|AHB_CTL0_BM1_EN|       \
                         AHB_CTL0_VSP_EN|AHB_CTL0_ROT_EN)

//register save
#define SAVE_REG(_reg_save, _reg_addr, _reg_mask)  {_reg_save = (*(volatile uint32*)(_reg_addr) & ((uint32)_reg_mask));}
#define SAVE_ANA_REG(_reg_save, _reg_addr, _reg_mask) {_reg_save = (ANA_REG_GET(_reg_addr) & ((uint32)_reg_mask));}

#define SAVE_GLOBAL_REG  do{ \
        SAVE_REG(reg_gen_clk_en, GR_CLK_EN, GR_CLK_EN_MASK); \
        SAVE_REG(reg_gen0_val, GR_GEN0, GR_GEN0_MASK);   \
        SAVE_REG(reg_busclk_alm, GR_BUSCLK_ALM, BUSCLK_ALM_MASK);    \
        SAVE_REG(reg_ahb_ctl0_val, AHB_CTL0, AHB_CTL0_MASK);\
    }while(0)

//register restore
#define REG_LOCAL_VALUE_DEF   uint32 reg_val;
#define RESTORE_REG(_reg_addr, _reg_mask, _reg_save)    do{reg_val = *(volatile uint32*)(_reg_addr); \
        reg_val &= ~((uint32)_reg_mask);    \
        reg_val |= ((_reg_save) & ((uint32)_reg_mask));    \
        *(volatile uint32*)(_reg_addr) = reg_val; \
    }while(0)

#define RESTORE_ANA_REG(_reg_addr, _reg_mask, _reg_save)    do{reg_val = ANA_REG_GET(_reg_addr); \
        reg_val &= ~((uint32)_reg_mask);    \
        reg_val |= ((_reg_save) & ((uint32)_reg_mask));    \
        ANA_REG_SET(_reg_addr, reg_val); \
    }while(0)

#define RESTORE_GLOBAL_REG   do{  \
        RESTORE_REG(GR_CLK_EN, GR_CLK_EN_MASK, reg_gen_clk_en);   \
        RESTORE_REG(GR_BUSCLK_ALM, BUSCLK_ALM_MASK, reg_busclk_alm);   \
        RESTORE_REG(GR_GEN0, GR_GEN0_MASK, reg_gen0_val);\
        RESTORE_REG(AHB_CTL0, AHB_CTL0_MASK, reg_ahb_ctl0_val); \
    }while(0)

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *

**---------------------------------------------------------------------------*/
uint32  RF_enable_flag              = 0;
uint32  MCU_is_sleep                = 0x0;// Status flag indicate whether MCU is in sleep or not
uint32  MCU_is_wakeup               = 0x0;
uint32     MCU_is_waiting_tick      = 0x0;
uint32     s_dma_sleep_mode         = 0;

LOCAL BOOLEAN     is_deepsleep      = FALSE;
LOCAL uint32 s_sleep_xtl_open_cnt = 0;

//extern BOOLEAN enable_deep_sleep;
//extern BOOLEAN REFPARAM_GetEnableArmSleepFlag(void);

/**---------------------------------------------------------------------------*
 **                         Function Prototype
*/
LOCAL void SLEEP_RestoreReg (void);
extern BOOLEAN RM_GetEnableArmSleepFlag (void);
extern int RM_SetEnableArmSleepFlag (int flg);
/**---------------------------------------------------------------------------*/
extern int     SCI_BeforeEnterDeepSleep (void);
extern int     SCI_BeforeEnterHalfDeepSleep (void);
extern int     SCI_AfterExitDeepSleep (void);

PUBLIC void DSLP_SetEnableSleepFlag (void)
{
    if (RM_GetEnableArmSleepFlag())
    {
        RM_SetEnableArmSleepFlag (SCI_TRUE);
    }
    else
    {
        RM_SetEnableArmSleepFlag (SCI_FALSE);
    }
}

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
**---------------------------------------------------------------------------*/
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
        REG32 (GR_BUSCLK) &= ~ (uint32) BUSCLK_ALM_MASK;
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
        //disable apb_force_on
        //clear apb device enable bit
        REG32 (GR_GEN0) &= ~ (uint32) (GR_GEN0_MASK);
        REG32 (GR_CLK_EN) &= ~ (uint32) (GR_CLK_EN_MASK);

        is_disabled = SCI_TRUE;
    }

    return is_disabled;
}
/*****************************************************************************
 *  Description:    this function is used to wait for the lcdc dcam vsp emc  *
 *                  channel idle                                             *
 *  Global Resource Dependence:                                              *
 *  Author: Junqiang.Wang                                                    *
 *  Note:   return SCI_TRUE when channels all idle, else return SCI_FALSE   *
******************************************************************************/
LOCAL BOOLEAN SLEEP_WaitEmcSleep()
{
    int i ;
    uint32 emc_wait_channel_mask = (BIT_6 | BIT_7 | BIT_8);

    for (i =  0; i < 0x200; i++)
    {
        if ( (REG32 ( (uint32) EXT_MEM_STS3) & emc_wait_channel_mask) == emc_wait_channel_mask)
        {
            return SCI_TRUE;
        }
    }

    return SCI_FALSE;
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
        if (!SLEEP_WaitEmcSleep()) //IF EMC is not idle can clean the eb
        {
            return;
        }

        REG32 (AHB_CTL0) &= ~ (uint32) (AHB_CTL0_MASK) ;
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

    /*get the sleep table entry*/
    s_slp_ctl_tab = Slp_Get_Cfg();
    /*config all sleep level common parameters*/
    Slp_Reg_Cfg (0,SLP_CFG_COM);

    REG32 (AHB_CTL1) |= (uint32) (AHB_CTRL1_ARMMTX_AUTO_GATE_EN | AHB_CTRL1_MSTMTX_AUTO_GATE_EN \
                                  | AHB_CTRL1_EMC_CH_AUTO_GATE_EN | AHB_CTRL1_EMC_AUTO_GATE_EN);
    /* wakeup ldo when xtl wakeup */
    REG32 (GR_CLK_EN)   |= BIT_18/*MCU_XTLEN_AUTOPD_EN*/;

    ANA_REG_OR (ANA_ANA_CTL0, BIT_15/*FSM_LDOPA_BP_EN*/);

    REG32 (GR_POWCTL1) = POWCTL1_CONFIG; //mcu&&mm power down auto en
    /*deepsleep flag init*/
    is_deepsleep  = SCI_FALSE;
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
        case SLEEP_DRV_LCD:
            {
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

                if ( (slp_index == AHB_CAN_SLP_APB_CAN_SLP)) // andy fix, @cr232700
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

                if ( (slp_index == AHB_CAN_SLP_APB_CAN_SLP) ||
                        (slp_index == AHB_CAN_SLP_APB_NO_SLP))
                {
                    //disable apb
                    SLEEP_DisableAHBModule();
                }
            }
            break;

        default:    //enter ARM sleep
            {
                is_deepsleep = SCI_FALSE;
                slp_index = AHB_NO_SLP_APB_NO_SLP;
            }
    }

    // Enter different sleep routine according to ahb and apb state
#if SLEEP_SUPPORT_RETENTION

    if (is_deepsleep)
    {
        MMU_DisableIDCM();
    }

#endif

    TM_SendTestPointRequest (slp_index,            100414);
    TM_SendTestPointRequest (REG32 (AHB_STS),      100415);
    TM_SendTestPointRequest (REG32 (GR_STC_STATE), 100416);
    TM_SendTestPointRequest (REG32 (GR_CLK_DLY),   100417);
    TM_SendTestPointRequest (REG32 (GR_POWCTL1),   100418);
    TM_SendTestPointRequest (SCI_Getsleepmode(),   100419);
    TM_SendTestPointRequest (REG32 (GR_GEN0),      100420);
    TM_SendTestPointRequest (REG32 (GR_CLK_EN),    100421);
    TM_SendTestPointRequest (REG32 (GR_BUSCLK),    100422);
    TM_SendTestPointRequest (REG32 (AHB_CTL0),     100423);

    SCI_Enter_SleepRoutine (slp_index);

#if SLEEP_SUPPORT_RETENTION

    if (is_deepsleep)
    {
        MMU_EnableIDCM ();
    }

#endif

    SLEEP_RestoreReg();
    TX_RESTORE
    // Read completed, DSP querying for changing
#endif
}

void SCI_InitDSP_MCU_SHR (void)
{

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

    // Clear Sleep flag
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
    if (SCI_TRUE == is_open)
    {
        if ( 0 == s_sleep_xtl_open_cnt)
        {
            /*output 26Mhz clk in deep sleep*/
            ANA_REG_AND (ANA_LDO_SLP,  ~BIT_0);
            ANA_REG_OR (ANA_LED_CTL, BIT_14);
        }
        s_sleep_xtl_open_cnt++;
    }
    else
    {
        /* to guarantee xtl is closed*/
        if ( 0 == s_sleep_xtl_open_cnt)
        {
            s_sleep_xtl_open_cnt = 1;
        }
        if ( 1 == s_sleep_xtl_open_cnt)
        {
            /*do not output 26Mhz clk in deep sleep*/
            ANA_REG_AND (ANA_LED_CTL,  ~BIT_14);
            ANA_REG_OR (ANA_LDO_SLP, BIT_0);
        }
        s_sleep_xtl_open_cnt--;
    }
}

/*****************************************************************************/
// Description :    This function is used to Enable
// LDO_RF power down when enter deepsleep
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
void DSSLP_EnableLDORFAutoPD (void)
{
}

/*****************************************************************************/
// Description :    This function is used to production test
// Author :         hao.liu
// Note :
/*****************************************************************************/
PUBLIC void SCI_Deep_Sleep_Test (void)
{
    int  loop;


    INT_HAL_DisableINT();

    ANA_REG_SET (ANA_LDO_PD_CTL, ANA_LDO_PD_CTL_MSK);               ///turn off all module ldo

    ANA_REG_SET (ANA_LED_CTL, 0xA081); //LED_CTRL
    ANA_REG_SET (ANA_PA_CTL, 0xC055); //PA_CTRL

    ///CHIP_REG_OR (GR_CLK_GEN5,LDO_USB_PD);
    CHIP_REG_AND (AHB_CTL3, ~BIT_6); //disable usb clock
    CHIP_REG_OR (GR_CLK_GEN5, BIT_9);
    CHIP_REG_AND (AHB_CTL0, ~BIT_5); //usbd enable

    sleep_active_modules = 0;
    sleep_apb_active_modues = 0;
    sleep_ahb_active_modules = 0;


    SCI_EnterDeepSleep();

    for (loop = 0; loop < 0x4000; loop++)
    {
    }

    //GPIO_SetLcdBackLight(1);

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
