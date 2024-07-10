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
#include "sleep_phy_sc6531.h"
#include "task_monitor.h"
#include "pin_sleep.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#include "fiq_drvapi.h"
#include "dal_audio.h"

#include "ldo_drvapi.h"
#include "../../tpc/r4p0/tpc_reg_r4p0.h"

/*lint -esym(551,test_sleep_count)*/
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

#ifdef TOUCHPANEL_TYPE_HW
typedef struct {
	uint32 ldo_reg;
	uint32 pin_tpxl_reg;
	uint32 pin_tpxr_reg;
	uint32 pin_tpyu_reg;
	uint32 pin_tpyd_reg;
} SLEEP_REG_SAVE_T;

static SLEEP_REG_SAVE_T sleep_reg_save = {0};
static uint32 test_sleep_count = 0;
#define TPFIX_REG_MODIFY() do{												\
	if ( (!(SCI_Getsleepmode() & SLEEP_DRV_LCD)) && is_deepsleep)				\
	{																		\
				ANA_REG_AND(TPC_CTRL, ~TPC_CTRL_EN);						\
				ANA_REG_SET(ANA_EB_CLR0, (ANA_TPC_EB_CLR));				\
    			ANA_REG_SET(ANA_EB_CLR1, (ANA_RTC_TPC_EB_CLR));			\
				sleep_reg_save.ldo_reg = ANA_REG_GET(ANA_SLP_LDO_CTL0);	\
				test_sleep_count ++;										\
				sleep_reg_save.pin_tpxl_reg = ANA_REG_GET(ANA_PIN_TP_XL_REG);		\
				sleep_reg_save.pin_tpxr_reg = ANA_REG_GET(ANA_PIN_TP_XR_REG);		\
				sleep_reg_save.pin_tpyu_reg = ANA_REG_GET(ANA_PIN_TP_YU_REG);		\
				sleep_reg_save.pin_tpyd_reg = ANA_REG_GET(ANA_PIN_TP_YD_REG);		\
				ANA_REG_OR(ANA_SLP_LDO_CTL0, SLP_LDORF_PD_EN);						\
				ANA_REG_SET(ANA_PIN_TP_XL_REG, ( ANA_PIN_Z_EN | ANA_PIN_SPU_EN  | ANA_PIN_FUNC_DEF | ANA_PIN_FPU_EN | ANA_PIN_DS_1 ));\
				ANA_REG_SET(ANA_PIN_TP_XR_REG, ( ANA_PIN_Z_EN | ANA_PIN_SPU_EN  | ANA_PIN_FUNC_DEF | ANA_PIN_FPX_EN | ANA_PIN_DS_1 ));\
				ANA_REG_SET(ANA_PIN_TP_YU_REG, ( ANA_PIN_Z_EN | ANA_PIN_SPU_EN  | ANA_PIN_FUNC_DEF | ANA_PIN_FPX_EN | ANA_PIN_DS_1 ));\
				ANA_REG_SET(ANA_PIN_TP_YD_REG, ( ANA_PIN_Z_EN | ANA_PIN_SPU_EN  | ANA_PIN_FUNC_DEF | ANA_PIN_FPX_EN | ANA_PIN_DS_1 ));\
	}else{							\
		ANA_REG_SET(ANA_SLP_LDO_CTL0, ANA_REG_GET(ANA_SLP_LDO_CTL0) & (~SLP_LDORF_PD_EN));	\
	}							\
    }while(0)

#define TPFIX_REG_RESTORE() do{ 											\
	volatile uint32 i;\
	for(i=0;i<1000;i++) {};\
	if ((!(SCI_Getsleepmode() & SLEEP_DRV_LCD)) && is_deepsleep)				\
	{																		\
		ANA_REG_SET(ANA_SLP_LDO_CTL0, sleep_reg_save.ldo_reg);				\
		ANA_REG_SET(ANA_PIN_TP_XL_REG, sleep_reg_save.pin_tpxl_reg);		\
		ANA_REG_SET(ANA_PIN_TP_XR_REG, sleep_reg_save.pin_tpxr_reg);		\
		ANA_REG_SET(ANA_PIN_TP_YU_REG, sleep_reg_save.pin_tpyu_reg);		\
		ANA_REG_SET(ANA_PIN_TP_YD_REG, sleep_reg_save.pin_tpyd_reg);		\
		ANA_REG_SET(ANA_EB_SET0, (ANA_TPC_EB_SET));						\
		ANA_REG_SET(ANA_EB_SET1, (ANA_RTC_TPC_EB_SET));					\
		ANA_REG_OR(TPC_CTRL, TPC_CTRL_EN);									\
	}else{																		\
		ANA_REG_OR(ANA_SLP_LDO_CTL0, SLP_LDORF_PD_EN);						\
	}			\
	}while(0)	
	
#endif	


// Define mask for register save
#define APB_EB0_MASK    (IIS_EB_CLR  | PWM_EB_CLR | PIN_REG_EB_CLR\
               | UART1_EB_CLR | UART0_EB_CLR | SIM_EB_CLR | GEA_EB_CLR \
               | I2C1_EB_CLR | I2C2_EB_CLR | I2C_EB_CLR)



#define AHB_EB0_MASK  (BUSMON3_EB_CLR | BUSMON2_EB_CLR | BUSMON1_EB_CLR | BUSMON0_EB_CLR  \
                            | DMA_EB_CLR | LCDC_EB_CLR | DCAM_EB_CLR | USBD_EB_CLR  \
                            | SPI0_EB_CLR | MIDI_EB_CLR | SDIO_EB_CLR | TIC_EB_CLR  \
                            | ROTA_EB_CLR | SPI1_EB_CLR | ZIP1_EB_CLR | LCM_EB_CLR | ZIP0_EB_CLR)


// Register save
#define SAVE_REG(_reg_save, _reg_addr, _reg_mask)  {_reg_save = ( (REG32(_reg_addr))/* & ((uint32)_reg_mask)*/);}
#define SAVE_GLOBAL_REG  do{  \
        SAVE_REG(reg_apb_eb0_val, APB_EB0, APB_EB0_MASK);     \
        SAVE_REG(reg_ahb_eb0_val, AHB_EB0, AHB_EB0_MASK);  \
    }while(0)

// Register restore
#define REG_LOCAL_VALUE_DEF   uint32 reg_val;
#define RESTORE_REG(_reg_addr, _reg_mask, _reg_save)    do{CHIP_REG_SET(_reg_addr, _reg_save);}while(0)


#define RESTORE_GLOBAL_REG   do{  \
        RESTORE_REG(APB_EB0_SET, APB_EB0_MASK, reg_apb_eb0_val); \
        RESTORE_REG(AHB_EB0_SET, AHB_EB0_MASK, reg_ahb_eb0_val);\
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

LOCAL BOOLEAN s_PERI_powerdomain_en      = FALSE;

#if 0 //def ARM_CORE_POWER_DOMAIN
#pragma arm section zidata = "ARMCORE_STACK"
#define ARMCORE_STACK_LIMIT  0x30
uint32 armcore_stack_space[ARMCORE_STACK_LIMIT];
#pragma arm section zidata
#endif


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


PUBLIC BOOLEAN SLEEP_PERI_PowerDomain_Set(BOOLEAN is_enable)
{
    s_PERI_powerdomain_en = is_enable;
    return SCI_TRUE;
}

PUBLIC BOOLEAN SLEEP_PERI_PowerDomain_Get(void)
{
    return s_PERI_powerdomain_en;
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
        //CHIP_REG_AND (APB_EB0, ~ (APB_EB0_MASK));
        REG32(APB_EB0_CLR) = APB_EB0_MASK;

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
        //CHIP_REG_AND (AHB_EB0, ~ (AHB_EB0_MASK));
        REG32(AHB_EB0_CLR) = AHB_EB0_MASK;
    }
}

BOOLEAN SLEEP_BT_PowerDomain_Set(BOOLEAN is_on)
{
    BOOLEAN ret = SCI_TRUE;
    uint32 cur_tick, next_tick;
    uint32 timeout_tick;
//    return SCI_TRUE;
    
    SCI_DisableIRQ();
        
    if (is_on)
    {
        REG32(APB_RST0_SET) |= ARM7_SOFT_RST_SET;
        REG32(APB_BT_PWR_CTL) &= (~ARM7_POW_FORCE_PD);
        
        REG32(APB_MISC_CTL3) &= (~BT_DAC_PD_O_SW);
        REG32(APB_MISC_CTL3) &= (~RF_BT_ADDA_PD_SW);

        cur_tick = next_tick = SCI_GetTickCount();
        timeout_tick = (((REG32(APB_ARMCORE_PWR_CTL) & ARMCORE_PWR_ON_DLY_MSK) >> ARMCORE_PWR_ON_DLY_SHIFT)
                        + ((REG32(APB_ARMCORE_PWR_CTL) & ARMCORE_ISO_OFF_NUM_MSK) >> ARMCORE_ISO_OFF_NUM_SHIFT) + 2) << 1;

        for(;;)
        {
            if ((REG32(APB_PWR_STATE) & PD_ARM7_STATUS_MSK) == APB_PWR_STATE_WAKE)
            {
                ret = SCI_TRUE;
                break;
            }

            if ( (next_tick - cur_tick) > timeout_tick)
            {
                TM_SendTestPointRequest (0xEAEA0001, (next_tick - cur_tick));
                SCI_PASSERT(0, ("arm7 wake Timeout!"));/*assert to do*/
            }
            
            next_tick = SCI_GetTickCount();
        }
        
    }
    else
    {
        REG32(APB_MISC_CTL3) |= BT_DAC_PD_O_SW;
        REG32(APB_MISC_CTL3) |= RF_BT_ADDA_PD_SW;
        
        REG32(APB_BT_PWR_CTL) |= ARM7_POW_FORCE_PD;
//        REG32(APB_BT_PWR_CTL) |= ARM7_SLP_POWOFF_AUTO_EN;

        cur_tick = next_tick = SCI_GetTickCount();
        timeout_tick = (((REG32(APB_ARMCORE_PWR_CTL) & ARMCORE_ISO_ON_NUM_MSK) >> ARMCORE_ISO_ON_NUM_SHIFT) + 2) << 1;
        
        for(;;)
        {
            if ((REG32(APB_PWR_STATE) & PD_ARM7_STATUS_MSK)
                    == (APB_PWR_STATE_PD | APB_PWR_STATE_ISOLATION | APB_PWR_STATE_SHUTDOWN)
                )
            {
                ret = SCI_TRUE;
                break;
            }

            if ( (next_tick - cur_tick) > timeout_tick)
            {
                TM_SendTestPointRequest (0xEAEA0002, (next_tick - cur_tick));
                SCI_PASSERT(0, ("arm7 powerdown Timeout!"));/*assert to do*/
            }
            
            next_tick = SCI_GetTickCount();
        }
    }


    SCI_RestoreIRQ();

    return ret;
    
}


void SLEEP_PowerDomain_Config()
{
#ifdef ARM_CORE_POWER_DOMAIN

    REG32(APB_ARMCORE_PWR_CTL) &= (~ARMCORE_ISO_OFF_NUM_MSK);
    REG32(APB_ARMCORE_PWR_CTL) |= BIT_0;
    
    REG32(APB_ARMCORE_PWR_CTL) &= (~ARMCORE_ISO_ON_NUM_MSK);
    REG32(APB_ARMCORE_PWR_CTL) |= BIT_8;
    
    REG32(APB_ARMCORE_PWR_CTL) &= (~ARMCORE_PWR_ON_DLY_MSK);
    REG32(APB_ARMCORE_PWR_CTL) |= BIT_24;

    REG32(APB_ARMCORE_PWR_CTL) |= ARMCORE_SLP_POWOFF_AUTO_EN;
#endif

    
    REG32(APB_PERI_PWR_CTL) &= (~PERI_ISO_OFF_NUM_MSK);
//    REG32(APB_PERI_PWR_CTL) |= BIT_0;
    
    REG32(APB_PERI_PWR_CTL) &= (~PERI_ISO_ON_NUM_MSK);
//    REG32(APB_PERI_PWR_CTL) |= BIT_8;
    
    REG32(APB_PERI_PWR_CTL) &= (~PERI_PWR_ON_DLY_MSK);
//    REG32(APB_PERI_PWR_CTL) |= BIT_24;

    if (SLEEP_PERI_PowerDomain_Get())
    {
        REG32(APB_PERI_PWR_CTL) |= PERI_SLP_POWOFF_AUTO_EN;
    }
    
    REG32(APB_MM_PWR_CTL) &= (~MM_ISO_OFF_NUM_MSK);
//    REG32(APB_MM_PWR_CTL) |= BIT_0;
    
    REG32(APB_MM_PWR_CTL) &= (~MM_ISO_ON_NUM_MSK);
//    REG32(APB_MM_PWR_CTL) |= BIT_8;
    
    REG32(APB_MM_PWR_CTL) &= (~MM_PWR_ON_DLY_MSK);
//    REG32(APB_MM_PWR_CTL) |= BIT_24;
    
    REG32(APB_MM_PWR_CTL) |= MM_SLP_POWOFF_AUTO_EN;


        
    REG32(APB_BT_PWR_CTL) &= (~ARM7_ISO_OFF_NUM_MSK);
//    REG32(APB_BT_PWR_CTL) |= BIT_0;
    
    REG32(APB_BT_PWR_CTL) &= (~ARM7_ISO_ON_NUM_MSK);
//    REG32(APB_BT_PWR_CTL) |= BIT_8;
    
    REG32(APB_BT_PWR_CTL) &= (~ARM7_PWR_ON_DLY_MSK);
//    REG32(APB_BT_PWR_CTL) |= BIT_24;

}

void SLEEP_PllAutoPd(void)
{
    uint32 reg_val;
    
    reg_val = REG32(APB_PLL_CTL0);

    reg_val &= (~BIT_29);
    reg_val &= (~BIT_30);
    reg_val &= (~(0x7f<<4));

    //reg_val &= (~(0x7F<<24));       // henryhe debug
    //SCI_TraceLow("%s reg_val:0x%x \r\n", __func__, reg_val);

    REG32(APB_PLL_CTL0) = reg_val;
}

void SLEEP_MemoryEnable(void)
{
    REG32(MEM_LP_CTRL0) = 0;  // enable memory
    REG32(MEM_SLEEP_SD_EN0) = 0; 
    REG32(MEM_SLEEP_SD_EN1) = 0; 
    REG32(MEM_SLEEP_SD_EN2) = 0; 
    REG32(MEM_SLEEP_SD_EN3) = 0;     
}

void SLEEP_CoreSleep_FlagSet(void)
{
    REG32(AHB_ARCH_PORT) = 0xA1B2; // unlock AHB_SYS_CTL2

    REG32(AHB_SYS_CTL2) |= ARM9_CORE_PD_FLAG;

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
    //Slp_Reg_Cfg (0,SLP_CFG_COM);

    // Deepsleep flag init
    is_deepsleep  = SCI_FALSE;

    CHIP_REG_AND (APB_XTL_CTL0, (~XTL_WAIT_SEL));
    CHIP_REG_AND (APB_XTL_CTL0, (~MCU_XTLEN_AUTOPD_EN));
    
    CHIP_REG_AND(APB_XTL_CTL0, (~(0x140<<16)));
    CHIP_REG_OR(APB_XTL_CTL0, (0x10<<16));          // xtl buf wait time : 16 rtc cycle

    CHIP_REG_AND(APB_XTL_CTL0, (~(0x140)));
    CHIP_REG_OR(APB_XTL_CTL0, 32);              // xtl wait time : 32 rtc cycle

    CHIP_REG_AND (APB_PLL_CTL1, (~PLL_WAIT_SEL));
    
    CHIP_REG_AND(APB_PLL_CTL1, (~0x140));
    CHIP_REG_OR(APB_PLL_CTL1, 0x28);           // pll wait time : 40 rtc cycle


    //ANA_REG_SET(ANA_XTL_CTL0, 100);
    ANA_REG_SET(ANA_XTL_CTL0, 32);          // adie xtl wait time : 32 rtc cycle

#if 0

    ANA_REG_AND(ANA_SLP_LDO_CTL0, (~SLP_LDOMEMS_PD_EN));
    ANA_REG_OR(ANA_SLP_LDO_CTL0, SLP_LDOVDD33_PD_EN);
    //ANA_REG_OR(ANA_SLP_LDO_CTL0, SLP_LDORF_PD_EN);

    //ANA_REG_OR(ANA_SLP_LDO_CTL0, SLP_LDODCXO_PD_EN); // ldo dcxo shutdown
    //ANA_REG_OR(ANA_SLP_LDO_CTL0, BIT_1); // ldo28 shutdown

   // ANA_REG_AND(ANA_SLP_LDO_CTL0, (~SLP_LDOSIM0_PD_EN));    // open ldo sim0
   // ANA_REG_AND(ANA_LDO_PD_SET1, (~LDO_SIM0_PD));                      // open ldo sim0   
   // ANA_REG_OR(ANA_LDO_PD_RST1, (LDO_SIM0_PD_RST));                      // open ldo sim0   

    ANA_REG_AND(ANA_SLP_LDO_CTL1, (~SLP_AUDIO_MICBIAS_PD_EN));
    ANA_REG_AND(ANA_SLP_LDO_CTL1, (~SLP_AUDIO_BG_PD_EN));
    ANA_REG_AND(ANA_SLP_LDO_CTL1, (~SLP_AUDIO_VBO_PD_EN));
    ANA_REG_AND(ANA_SLP_LDO_CTL1, (~SLP_AUDIO_VB_PD_EN));
    ANA_REG_AND(ANA_SLP_LDO_CTL1, (~SLP_AUDIO_VCMBUF_PD_EN));
    ANA_REG_AND(ANA_SLP_LDO_CTL1, (~SLP_AUDIO_VCM_PD_EN));
    
    ANA_REG_AND (ANA_SLP_LDO_CTL2, (~SLP_LDOVDD18_V_EN)); 

    ANA_REG_OR (ANA_SLP_LDO_CTL2, SLP_PWR_SET_EN);  //Set to 1 will enable all LDOs and DCDC sleep control
#endif    

    ANA_REG_OR(ANA_MISC_CTL2, DCXO_MODE_SWT2);
    ANA_REG_AND(ANA_MISC_CTL2, (~DCXO_MODE_SWT1));

    ANA_REG_OR(ANA_MISC_CTL8, CLK_SELF_EN_CLR); 
    
    //ANA_REG_OR(ANA_CLK_SET0, SLP_FM_ACTIVE_EN_SET);  // henryhe debug

    SLEEP_PllAutoPd();
    SLEEP_PowerDomain_Config();
    SLEEP_MemoryEnable();

}

LOCAL void SLEEP_RestoreReg (void)
{
    //REG_LOCAL_VALUE_DEF

    RESTORE_GLOBAL_REG;

    if (is_deepsleep)
    {
        SCI_AfterExitDeepSleep();
    }

    //is_deepsleep = SCI_FALSE;

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
					
#ifdef TOUCHPANEL_TYPE_HW
					TPFIX_REG_MODIFY();					
#endif
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
		case SLEEP_DRV_FM:
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

#ifdef TOUCHPANEL_TYPE_HW
	TPFIX_REG_RESTORE();	//add for fix IC bug
#endif
	
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
