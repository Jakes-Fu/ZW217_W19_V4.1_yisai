/******************************************************************************
 ** File Name:      sc6531_clock_cfg.c                                      *
 ** Author:         Yi.Qiu                                                  *
 ** DATE:           02/21/2010                                              *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.       *
 ** Description:                                                             *
 ******************************************************************************

 ******************************************************************************
 **                 Edit History                                               *
 ** ------------------------------------------------------------------------*
 ** DATE               NAME             DESCRIPTION                                                              *
 ** 09/17/2012      yong.li           Create.                                                                       *
 ******************************************************************************/
#include "chip_drv_trc.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "../clock.h"
#include "ref_outport.h"

#define ARM_APLLCLK_SUPPORT

static const char *err_string = "%s choosed a wrong source [%s]\r\n please choose one of: [%s]\r\n";

#define DELAY_CYCLE(n)  {\
                                            uint32 k=0;\
                                             for(k=0; k<n; k++){}\
                                         }

/******************************************************************************
 * _ArmSrcSet
 ******************************************************************************/
LOCAL void _ArmSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _ApbSrcSet
 ******************************************************************************/
LOCAL void _ApbSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _SfcSrcSet
 ******************************************************************************/
LOCAL void _SfcSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _SfcSrcDivSet
 ******************************************************************************/
LOCAL void _SfcSrcDivSet (int Div)
{
}

/******************************************************************************
 * _DcamSrcSet
 ******************************************************************************/
LOCAL void _DcamSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _SensorClkSrcSet
 ******************************************************************************/
LOCAL void _SensorClkSrcSet (const char *SrcName)
{
}
/******************************************************************************
 * _SensorSrcDivSet
 ******************************************************************************/
LOCAL void _SensorSrcDivSet (int Div)
{
}

/******************************************************************************
 * _SdioSrcSet
 ******************************************************************************/
LOCAL void _SdioSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _I2s0SrcSet
 ******************************************************************************/
LOCAL void _I2s0SrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _I2s0SrcDivSet
 ******************************************************************************/
LOCAL void _I2s0SrcDivSet (int Div)
{
}

/******************************************************************************
 * _Uart0SrcSet
 ******************************************************************************/
LOCAL void _Uart0SrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _Uart1SrcSet
 ******************************************************************************/
LOCAL void _Uart1SrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _Pwm0SrcSet
 ******************************************************************************/
LOCAL void _PwmSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _Spi0SrcSet
 ******************************************************************************/
LOCAL void _Spi0SrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _Spi0SrcDivSet
 ******************************************************************************/
LOCAL void _Spi0SrcDivSet (int Div)
{
}

/******************************************************************************
 * _Spi1SrcSet
 ******************************************************************************/
LOCAL void _Spi1SrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _Spi1SrcDivSet
 ******************************************************************************/
LOCAL void _Spi1SrcDivSet (int Div)
{
}

/******************************************************************************
 * _AuxSrcSet
 ******************************************************************************/
LOCAL void _AuxSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _AuxSrcDivSet
 ******************************************************************************/
LOCAL void _AuxSrcDivSet (int Div)
{
}

/******************************************************************************
 * _Usb48SrcSet
 ******************************************************************************/
LOCAL void _Usb48SrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _Usb12SrcSet
 ******************************************************************************/
LOCAL void _Usb12SrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _32KBtSrcSet
 ******************************************************************************/
LOCAL void _32KBtSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _32KStcSrcSet
 ******************************************************************************/
LOCAL void _32KStcSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _32KOutSrcSet
 ******************************************************************************/
LOCAL void _32KOutSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _1KOutSrcSet
 ******************************************************************************/
LOCAL void _1KOutSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _IIC0SrcSet
 ******************************************************************************/
LOCAL void _IIC0SrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _IIC1SrcSet
 ******************************************************************************/
LOCAL void _IIC1SrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _IIC2SrcSet
 ******************************************************************************/
LOCAL void _IIC2SrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _LzmaSrcSet
 ******************************************************************************/
LOCAL void _LzmaSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _EfsSrcSet
 ******************************************************************************/
LOCAL void _EfsSrcSet (const char *SrcName)
{
}


/******************************************************************************
 * _Cm4AsyncSrcSet
 ******************************************************************************/
LOCAL void _Cm4AsyncSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _Cm4AsyncSrcDivSet
 ******************************************************************************/
LOCAL void _Cm4AsyncSrcDivSet (int Div)
{
}

/******************************************************************************
 * _Cm4ApbSrcSet
 ******************************************************************************/
LOCAL void _Cm4ApbSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _FmSrcSet
 ******************************************************************************/
LOCAL void _FmSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _FmRfCtrlSrcSet
 ******************************************************************************/
LOCAL void _FmRfCtrlSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _DapSrcSet
 ******************************************************************************/
LOCAL void _DapSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _DspSrcSet
 ******************************************************************************/
LOCAL void _DspSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _AudSrcSet
 ******************************************************************************/
LOCAL void _AudSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _AudIfSrcSet
 ******************************************************************************/
LOCAL void _AudIfSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _AdiSrcSet
 ******************************************************************************/
LOCAL void _AdiSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _RfFm26MSrcSet
 ******************************************************************************/
LOCAL void _RfFm26MSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _RfBt26MSrcSet
 ******************************************************************************/
LOCAL void _RfBt26MSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _Bt64MSrcSet
 ******************************************************************************/
LOCAL void _Bt64MSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _Bt64MSrcDivSet
 ******************************************************************************/
LOCAL void _Bt64MSrcDivSet (int Div)
{
}

/******************************************************************************
 * _Bt24MSrcSet:name is 24M,but the real value is 64M which base on clock plan.
 ******************************************************************************/
LOCAL void _Bt24MSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _BtLpoSrcSet
 ******************************************************************************/
LOCAL void _BtLpoSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _BtRfCtrlSrcSet
 ******************************************************************************/
LOCAL void _BtRfCtrlSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _PmuSrcSet
 ******************************************************************************/
LOCAL void _PmuSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _CcirSrcSet
 ******************************************************************************/
LOCAL void _CcirSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _SdSlv1LoopSrcSet
 ******************************************************************************/
LOCAL void _SdSlv1LoopSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _SdSlv2LoopSrcSet
 ******************************************************************************/
LOCAL void _SdSlv2LoopSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _FuncdmaSrcSet
 ******************************************************************************/
LOCAL void _FuncdmaSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _FmAdcSrcSet
 ******************************************************************************/
LOCAL void _FmAdcSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _BtAdcSrcSet
 ******************************************************************************/
LOCAL void _BtAdcSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _BtDacSrcSet
 ******************************************************************************/
LOCAL void _BtDacSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _BtRssiAdcAnaSrcSet
 ******************************************************************************/
LOCAL void _BtRssiAdcAnaSrcSet (const char *SrcName)
{
}

/******************************************************************************
 * _RtcXtlEnable
 ******************************************************************************/
LOCAL void _RtcXtlEnable (void)
{
}

/******************************************************************************
 * _RtcXtlDisable
 ******************************************************************************/
LOCAL void _RtcXtlDisable (void)
{
}

/******************************************************************************
 * _MpllEnable
 ******************************************************************************/
LOCAL void _MpllEnable (void)
{
}

/******************************************************************************
 * _MpllDisable
 ******************************************************************************/
LOCAL void _MpllDisable (void)
{
}

/******************************************************************************
 * _26MXtlEnable
 ******************************************************************************/
PUBLIC void _26MXtlEnable (void)
{
}

/******************************************************************************
 * _26MXtlDisable
 ******************************************************************************/
LOCAL void _26MXtlDisable (void)
{
}

/******************************************************************************
 * _adieClk26MEnable
 ******************************************************************************/
PUBLIC void _adieClk26MEnable (void)
{
    /* Enable 26M clock in ADie */
    //CLOCK_LOG_TRACE:"[_26MXtlEnable] default is close"
    ANA_REG_OR (ANA_XTL_WAIT_CTRL, XTL_EN);
}

/******************************************************************************
 * _adieClk26MDisable
 ******************************************************************************/
LOCAL void _adieClk26MDisable (void)
{
    /* Disable 26M_XTL XTL */
    //CLOCK_LOG_TRACE:"[_26MXtlEnable] can't be closed."
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_487_112_2_17_23_3_47_525,(uint8*)"");
    ANA_REG_AND (ANA_XTL_WAIT_CTRL, ~XTL_EN);
}

/******************************************************************************
 * _SensorClkEnable
 ******************************************************************************/
LOCAL void _SensorClkEnable (void)
{
}

/******************************************************************************
 * _SensorClkDisable
 ******************************************************************************/
LOCAL void _SensorClkDisable (void)
{
}

/******************************************************************************
 * _TimerEnable
 ******************************************************************************/
LOCAL void _TimerEnable (void)
{
}

/******************************************************************************
 * _TimerDisable
 ******************************************************************************/
LOCAL void _TimerDisable (void)
{
}

/******************************************************************************
 * _TimerReset
 ******************************************************************************/
LOCAL void _TimerReset (void)
{
}

/******************************************************************************
 * _TimerbkEnable
 ******************************************************************************/
LOCAL void _TimerbkEnable (void)
{
}

/******************************************************************************
 * _TimerbkDisable
 ******************************************************************************/
LOCAL void _TimerbkDisable (void)
{
}

/******************************************************************************
 * _TimerbkReset
 ******************************************************************************/
LOCAL void _TimerbkReset (void)
{
}

/******************************************************************************
 * _KpdEnable
 ******************************************************************************/
LOCAL void _KpdEnable (void)
{
    /* Enable keypad apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_KpdEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_563_112_2_17_23_3_47_532,(uint8*)"");

    hwp_sysCtrl->CLK_AON_APB_ENABLE = SYS_CTRL_ENABLE_AON_APB_CLK_ID_KEYPAD;
}

/******************************************************************************
 * _KpdDisable
 ******************************************************************************/
LOCAL void _KpdDisable (void)
{
    /* Disable keypad apb clock */
    //CLOCK_LOG_TRACE:"[_KpdDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_574_112_2_17_23_3_47_533,(uint8*)"");

    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_AON_APB_DISABLE = SYS_CTRL_DISABLE_AON_APB_CLK_ID_KEYPAD;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _KpdReset
 ******************************************************************************/
LOCAL void _KpdReset (void)
{
    //CLOCK_LOG_TRACE:"[_KpdReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_584_112_2_17_23_3_47_534,(uint8*)"");

    /* Reset kpd clock */
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
	//reset adi module
    hwp_sysCtrl->AON_APB_RST_SET = SYS_CTRL_SET_AON_APB_RST_ID_KEYPAD;
    DELAY_CYCLE(1000)
	
    hwp_sysCtrl->AON_APB_RST_CLR = SYS_CTRL_CLR_AON_APB_RST_ID_KEYPAD;
    DELAY_CYCLE(1000)
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _IIC0Enable
 ******************************************************************************/
LOCAL void _IIC0Enable (void)
{
    /* Enable keypad apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_IIC0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_599_112_2_17_23_3_47_535,(uint8*)"");

    hwp_sysCtrl->CLK_AP_APB_ENABLE = SYS_CTRL_ENABLE_AP_APB_CLK_ID_I2C1;
}

/******************************************************************************
 * _IIC0Disable
 ******************************************************************************/
LOCAL void _IIC0Disable (void)
{
    /* Enable keypad apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_IIC0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_609_112_2_17_23_3_47_536,(uint8*)"");

    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_AP_APB_DISABLE = SYS_CTRL_DISABLE_AP_APB_CLK_ID_I2C1;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _IIC0Reset
 ******************************************************************************/
LOCAL void _IIC0Reset (void)
{
}

/******************************************************************************
 * _IIC1Enable
 ******************************************************************************/
LOCAL void _IIC1Enable (void)
{
    /* Enable keypad apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_IIC1Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_633_112_2_17_23_3_47_538,(uint8*)"");

    hwp_sysCtrl->CLK_AON_APB_ENABLE = SYS_CTRL_ENABLE_AON_APB_CLK_ID_I2C2;
}

/******************************************************************************
 * _IIC1Disable
 ******************************************************************************/
LOCAL void _IIC1Disable (void)
{
    /* Enable keypad apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_IIC1Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_643_112_2_17_23_3_47_539,(uint8*)"");

    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_AON_APB_DISABLE = SYS_CTRL_DISABLE_AON_APB_CLK_ID_I2C2;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}


/******************************************************************************
 * _IIC1Enable
 ******************************************************************************/
 void _GoudaClkEnable (void)
{
        hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
        hwp_sysCtrl->CLK_SYS_AHB_ENABLE = SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_GOUDA;
        hwp_sysCtrl->CLK_AP_APB_ENABLE = SYS_CTRL_ENABLE_AP_APB_CLK_ID_GOUDA;
        //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/

}

/******************************************************************************
 * _IIC1Disable
 ******************************************************************************/
 void _GoudaClkDisable (void)
{
        hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
        hwp_sysCtrl->CLK_SYS_AHB_DISABLE = SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_GOUDA;
        hwp_sysCtrl->CLK_AP_APB_DISABLE = SYS_CTRL_DISABLE_AP_APB_CLK_ID_GOUDA;
        //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _IIC1Reset
 ******************************************************************************/
LOCAL void _IIC1Reset (void)
{
}

/******************************************************************************
 * _SyscntEnable
 ******************************************************************************/
LOCAL void _SyscntEnable (void)
{
}

/******************************************************************************
 * _SyscntDisable
 ******************************************************************************/
LOCAL void _SyscntDisable (void)
{
}

/******************************************************************************
 * _SyscntReset
 ******************************************************************************/
LOCAL void _SyscntReset (void)
{
}

/******************************************************************************
 * _AdiEnable
 ******************************************************************************/
LOCAL void _AdiEnable (void)
{
    /* Enable Adi apb clock */
    //CLOCK_LOG_TRACE:"[_AdiEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_703_112_2_17_23_3_47_544,(uint8*)"");

    hwp_sysCtrl->CLK_AON_APB_ENABLE = SYS_CTRL_ENABLE_AON_APB_CLK_ID_ADI;
}

/******************************************************************************
 * _AdiDisable
 ******************************************************************************/
LOCAL void _AdiDisable (void)
{
    /* Disable Adi apb clock */
    //CLOCK_LOG_TRACE:"[_AdiDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_713_112_2_17_23_3_47_545,(uint8*)"");

    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_AON_APB_DISABLE = SYS_CTRL_DISABLE_AON_APB_CLK_ID_ADI;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _AdiReset
 ******************************************************************************/
LOCAL void _AdiReset (void)
{
    //CLOCK_LOG_TRACE:"[_AdiReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_722_112_2_17_23_3_47_546,(uint8*)"");

    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
	//reset adi module
    hwp_sysCtrl->AON_APB_RST_SET = SYS_CTRL_SET_AON_APB_RST_ID_ADI;
    DELAY_CYCLE(1000)
	
    hwp_sysCtrl->AON_APB_RST_CLR = SYS_CTRL_CLR_AON_APB_RST_ID_ADI;
    DELAY_CYCLE(1000)
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _RtcEnable
 ******************************************************************************/
LOCAL void _RtcEnable (void)
{
    /* Enable rtc clock */
    //CLOCK_LOG_TRACE:"[_RtcEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_736_112_2_17_23_3_47_547,(uint8*)"");
    ANA_REG_OR (ANA_MODULE_EN0,  ANA_RTC_EN);
    ANA_REG_OR (ANA_RTC_CLK_EN0,  ANA_RTC_RTC_EN);
}

/******************************************************************************
 * _RtcDisable
 ******************************************************************************/
LOCAL void _RtcDisable (void)
{
    /* Disable rtc apb clock */
    //CLOCK_LOG_TRACE:"[_RtcDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_747_112_2_17_23_3_47_548,(uint8*)"");
    ANA_REG_AND (ANA_MODULE_EN0,  ~ANA_RTC_EN);
    ANA_REG_AND (ANA_RTC_CLK_EN0,  ~ANA_RTC_RTC_EN);
}

/******************************************************************************
 * _RtcReset
 ******************************************************************************/
LOCAL void _RtcReset (void)
{
    //CLOCK_LOG_TRACE:"[_RtcReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_757_112_2_17_23_3_47_549,(uint8*)"");
    ANA_REG_OR (ANA_SOFT_RST0,  ANA_RTC_SOFT_RST);
    DELAY_CYCLE(1000)

    ANA_REG_AND (ANA_SOFT_RST0,  ~ANA_RTC_SOFT_RST);
    DELAY_CYCLE(1000)
}	

/******************************************************************************
 * _WdgEnable
 ******************************************************************************/
LOCAL void _WdgEnable (void)
{
    /* Enable watchdog apb clock */
    //CLOCK_LOG_TRACE:"[_WdgEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_771_112_2_17_23_3_47_550,(uint8*)"");
    ANA_REG_OR (ANA_MODULE_EN0,  ANA_WDG_EN);
    ANA_REG_OR (ANA_RTC_CLK_EN0,  ANA_RTC_WDG_EN);
}

/******************************************************************************
 * _WdgDisable
 ******************************************************************************/
LOCAL void _WdgDisable (void)
{
    /* Disable watchdog apb clock */
    //CLOCK_LOG_TRACE:"[_WdgDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_782_112_2_17_23_3_47_551,(uint8*)"");
    ANA_REG_AND (ANA_MODULE_EN0,  ~ANA_WDG_EN);
    ANA_REG_AND (ANA_RTC_CLK_EN0,  ~ANA_RTC_WDG_EN);
}

/******************************************************************************
 * _WdgReset
 ******************************************************************************/
LOCAL void _WdgReset (void)
{
    //CLOCK_LOG_TRACE:"[_WdgReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_792_112_2_17_23_3_47_552,(uint8*)"");
    ANA_REG_OR (ANA_SOFT_RST0,  ANA_WDG_SOFT_RST);
    DELAY_CYCLE(1000)

    ANA_REG_AND (ANA_SOFT_RST0,  ~ANA_WDG_SOFT_RST);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _EfuseEnable
 ******************************************************************************/
LOCAL void _PmicEfuseEnable (void)
{
    uint32 value;
    
    /* efuse must set ADI sck all on */   
    value = hwp_adiMst->adi_cfg0; 
    value |= ADI_MST_RF_GSSI_SCK_ALL_ON;   
    hwp_adiMst->adi_cfg0 = value;

//    SCI_TRACE_LOW("_PmicEfuseEnable, adi_cfg0=0x%x",  hwp_adiMst->adi_cfg0);

    ANA_REG_OR (ANA_MODULE_EN0,  (ANA_EFS_EN));
}

/******************************************************************************
 * _EfuseDisable
 ******************************************************************************/
LOCAL void _PmicEfuseDisable (void)
{
    uint32 value;
    
    /* restore ADI clock auto gate */   
    value = hwp_adiMst->adi_cfg0; 
    value &= ~ADI_MST_RF_GSSI_SCK_ALL_ON;   
    hwp_adiMst->adi_cfg0 = value;

    ANA_REG_AND (ANA_MODULE_EN0,  ~ANA_EFS_EN);
}

/******************************************************************************
 * _EfuseReset
 ******************************************************************************/
LOCAL void _PmicEfuseReset (void)
{
    ANA_REG_OR (ANA_SOFT_RST0,  ANA_EFS_SOFT_RST);
    DELAY_CYCLE(1000)

    ANA_REG_AND (ANA_SOFT_RST0,  ~ANA_EFS_SOFT_RST);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _AdcEnable
 ******************************************************************************/
LOCAL void _AdcEnable (void)
{
    /* Enable Adc apb clock */
    //CLOCK_LOG_TRACE:"[_AdcEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_877_112_2_17_23_3_47_559,(uint8*)"");
    ANA_REG_OR (ANA_MODULE_EN0,  ANA_ADC_EN);

#if 0
    /* for adc 26M clk */
    hwp_sysreg->RF_ANA_26M_CTRL_SET = 0x04;  //RF_ANA_26M_CTRL_SET bit2
    *((volatile unsigned long *)0x500304c8) = 0x1;  //ANA_ctrl_reg32=1
#endif
    
    //ANA_REG_SET (ANA_CLK_SET0,  CLK_ADC_EN_SET);	
    ANA_REG_OR (ANA_ARM_CLK_EN0,  (ANA_CLK_AUXADC_EN | ANA_CLK_AUXAD_EN));
}

/******************************************************************************
 * _AdcDisable
 ******************************************************************************/
LOCAL void _AdcDisable (void)
{
    /* Disable Adc apb clock */
    //CLOCK_LOG_TRACE:"[_AdcDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_890_112_2_17_23_3_47_560,(uint8*)"");
    ANA_REG_AND (ANA_MODULE_EN0,  ~ANA_ADC_EN);

    //ANA_REG_SET (ANA_CLK_CLR0,  CLK_ADC_EN_CLR);		
    ANA_REG_AND (ANA_ARM_CLK_EN0,  ~(ANA_CLK_AUXADC_EN | ANA_CLK_AUXAD_EN));	
}


/******************************************************************************
 * _AdcReset
 ******************************************************************************/
LOCAL void _AdcReset (void)
{
    //CLOCK_LOG_TRACE:"[_AdcReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_903_112_2_17_23_3_47_561,(uint8*)"");
    ANA_REG_OR (ANA_SOFT_RST0,  ANA_ADC_SOFT_RST);
    DELAY_CYCLE(10)

    ANA_REG_AND (ANA_SOFT_RST0,  ~ANA_ADC_SOFT_RST);
    DELAY_CYCLE(10)
}

/******************************************************************************
 * _GpiodEnable
 ******************************************************************************/
LOCAL void _GpiodEnable (void)
{
	/* Enable Gpio apb clock */
    //CLOCK_LOG_TRACE:"[_GpiodEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_917_112_2_17_23_3_47_562,(uint8*)"");

    // D-die
    hwp_sysCtrl->CLK_AON_APB_ENABLE = SYS_CTRL_ENABLE_AON_APB_CLK_ID_GPIO;
}

/******************************************************************************
 * _GpiodDisable
 ******************************************************************************/
LOCAL void _GpiodDisable (void)
{
    /* Disable Gpio apb clock */
    //CLOCK_LOG_TRACE:"[_GpiodDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_929_112_2_17_23_3_47_563,(uint8*)"");

    // D-die
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_AON_APB_DISABLE = SYS_CTRL_DISABLE_AON_APB_CLK_ID_GPIO;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _GpiodReset
 ******************************************************************************/
LOCAL void _GpiodReset (void)
{
    //CLOCK_LOG_TRACE:"[_GpioReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_940_112_2_17_23_3_47_564,(uint8*)"");

    /* Reset gpio in D-die */
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
	//reset adi module
    hwp_sysCtrl->AON_APB_RST_SET = SYS_CTRL_SET_AON_APB_RST_ID_GPIO;
    DELAY_CYCLE(1000)
	
    hwp_sysCtrl->AON_APB_RST_CLR = SYS_CTRL_CLR_AON_APB_RST_ID_GPIO;
    DELAY_CYCLE(1000)
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _EicdEnable
 ******************************************************************************/
LOCAL void _EicdEnable (void)
{
	/* Enable eic apb clock */
    //CLOCK_LOG_TRACE:"[_EicdEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_991_112_2_17_23_3_48_568,(uint8*)"");

    // D-die
    //CHIP_REG_SET (APB_EB0_SET,  EIC_RTC_EB);
    //CHIP_REG_SET (APB_EB0_SET,  EIC_EB);
}

/******************************************************************************
 * _EicdDisable
 ******************************************************************************/
LOCAL void _EicdDisable (void)
{
    /* Disable eic apb clock */
    //CLOCK_LOG_TRACE:"[_EicdDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1003_112_2_17_23_3_48_569,(uint8*)"");

    // D-die
    //CHIP_REG_SET (APB_EB0_CLR,  EIC_RTC_EB);
    //CHIP_REG_SET (APB_EB0_CLR,  EIC_EB);	
}

/******************************************************************************
 * _EicdReset
 ******************************************************************************/
LOCAL void _EicdReset (void)
{
    //CLOCK_LOG_TRACE:"[_EicdReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1014_112_2_17_23_3_48_570,(uint8*)"");

}

/******************************************************************************
 * _EicaEnable
 ******************************************************************************/
LOCAL void _EicaEnable (void)
{
	/* Enable eic adi clock */
    //CLOCK_LOG_TRACE:"[_EicaEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1029_112_2_17_23_3_48_571,(uint8*)"");

    // A-die
    ANA_REG_OR (ANA_RTC_CLK_EN0,  ANA_RTC_EIC_EN);	
    ANA_REG_OR (ANA_MODULE_EN0,  ANA_EIC_EN);
}

/******************************************************************************
 * _EicaDisable
 ******************************************************************************/
LOCAL void _EicaDisable (void)
{
    /* Disable eic adi clock */
    //CLOCK_LOG_TRACE:"[_EicaDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1041_112_2_17_23_3_48_572,(uint8*)"");

    // A-die
    ANA_REG_AND (ANA_RTC_CLK_EN0,  ~ANA_RTC_EIC_EN);	    
    ANA_REG_AND (ANA_MODULE_EN0,  ~ANA_EIC_EN);
}

/******************************************************************************
 * _EicaReset
 ******************************************************************************/
LOCAL void _EicaReset (void)
{
    //CLOCK_LOG_TRACE:"[_EicaReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1052_112_2_17_23_3_48_573,(uint8*)"");
}

/******************************************************************************
 * _Sim0Enable
 ******************************************************************************/
LOCAL void _Sim0Enable (void)
{
    /* Enable Sim0 apb clock */
    //CLOCK_LOG_TRACE:"[_Sim0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1067_112_2_17_23_3_48_574,(uint8*)"");

    hwp_sysCtrl->CLK_AP_APB_ENABLE = SYS_CTRL_ENABLE_AP_APB_CLK_ID_SCI1;
}

/******************************************************************************
 * _Sim0Disable
 ******************************************************************************/
LOCAL void _Sim0Disable (void)
{
    /* Disable Sim0 apb clock */
    //CLOCK_LOG_TRACE:"[_Sim0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1077_112_2_17_23_3_48_575,(uint8*)"");

    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_AP_APB_DISABLE = SYS_CTRL_DISABLE_AP_APB_CLK_ID_SCI1;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _Sim0Reset
 ******************************************************************************/
LOCAL void _Sim0Reset (void)
{
    //CLOCK_LOG_TRACE:"[_Sim0Reset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1086_112_2_17_23_3_48_576,(uint8*)"");

    /* Reset sim clock */
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
	//reset adi module
    hwp_sysCtrl->AP_APB_RST_SET = SYS_CTRL_SET_AP_APB_RST_ID_SCI1;
    DELAY_CYCLE(1000)
	
    hwp_sysCtrl->AP_APB_RST_CLR = SYS_CTRL_CLR_AP_APB_RST_ID_SCI1;
    DELAY_CYCLE(1000)
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _GeaEnable
 ******************************************************************************/
LOCAL void _GeaEnable (void)
{
    /* Enable Gea apb clock */
    //CLOCK_LOG_TRACE:"[_GeaEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1101_112_2_17_23_3_48_577,(uint8*)"");

    hwp_sysCtrl->CLK_SYS_AHB_ENABLE = SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_GEA3;
}

/******************************************************************************
 * _GeaDisable
 ******************************************************************************/
LOCAL void _GeaDisable (void)
{
    /* Disable Gea apb clock */
    //CLOCK_LOG_TRACE:"[_GeaDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1111_112_2_17_23_3_48_578,(uint8*)"");

    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_SYS_AHB_DISABLE = SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_GEA3;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _GeaReset
 ******************************************************************************/
LOCAL void _GeaReset (void)
{
    //CLOCK_LOG_TRACE:"[_GeaReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1120_112_2_17_23_3_48_579,(uint8*)"");

    /* Reset GEA clock */
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
	//reset adi module
    hwp_sysCtrl->SYS_AHB_RST_SET = SYS_CTRL_SET_SYS_AHB_RST_ID_GEA3;
    DELAY_CYCLE(1000)
	
    hwp_sysCtrl->AON_APB_RST_CLR = SYS_CTRL_CLR_SYS_AHB_RST_ID_GEA3;
    DELAY_CYCLE(1000)
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}	

/******************************************************************************
 * _EfuseEnable
 ******************************************************************************/
LOCAL void _EfuseEnable (void)
{
    /* Enable Efuse apb clock */
    //CLOCK_LOG_TRACE:"[_EfuseEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1135_112_2_17_23_3_48_580,(uint8*)"");

    hwp_sysCtrl->CLK_AON_APB_ENABLE = SYS_CTRL_ENABLE_AON_APB_CLK_ID_EFUSE;
}

/******************************************************************************
 * _EfuseDisable
 ******************************************************************************/
LOCAL void _EfuseDisable (void)
{
    /* Disable Efuse apb clock */
    //CLOCK_LOG_TRACE:"[_EfuseDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1145_112_2_17_23_3_48_581,(uint8*)"");

    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_AON_APB_DISABLE = SYS_CTRL_DISABLE_AON_APB_CLK_ID_EFUSE;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _Spi0Enable
 ******************************************************************************/
LOCAL void _Spi0Enable (void)
{
    /* Enable Spi0 apb clock */
    //CLOCK_LOG_TRACE:"[_Spi0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1155_112_2_17_23_3_48_582,(uint8*)"");

    //CHIP_REG_OR (APB_EB0,  SPI0_GLB_EB_SET);
    hwp_sysCtrl->CLK_AP_APB_ENABLE = SYS_CTRL_ENABLE_AP_APB_CLK_ID_SPI1;
}

/******************************************************************************
 * _Spi0Disable
 ******************************************************************************/
LOCAL void _Spi0Disable (void)
{
    /* Disable Spi0 apb clock */
    //CLOCK_LOG_TRACE:"[_SpiDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1166_112_2_17_23_3_48_583,(uint8*)"");

    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_AP_APB_DISABLE = SYS_CTRL_DISABLE_AP_APB_CLK_ID_SPI1;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _Spi0Reset
 ******************************************************************************/
LOCAL void _Spi0Reset (void)
{
    //CLOCK_LOG_TRACE:"[_Spi0Reset]"

    /* Reset SPI0 clock */
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
	//reset adi module
    hwp_sysCtrl->AP_APB_RST_SET = SYS_CTRL_SET_AP_APB_RST_ID_SPI1;
    DELAY_CYCLE(1000)
	
    hwp_sysCtrl->AP_APB_RST_CLR = SYS_CTRL_CLR_AP_APB_RST_ID_SPI1;
    DELAY_CYCLE(1000)
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _Spi1Enable
 ******************************************************************************/
LOCAL void _Spi1Enable (void)
{
    /* Enable Spi1 apb clock */
    //CLOCK_LOG_TRACE:"[_Spi1Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1175_112_2_17_23_3_48_584,(uint8*)"");

    //CHIP_REG_OR (APB_EB0,  SPI1_EB_SET);
    hwp_sysCtrl->CLK_AP_APB_ENABLE = SYS_CTRL_ENABLE_AP_APB_CLK_ID_SPI2;
}

/******************************************************************************
 * _Spi1Disable
 ******************************************************************************/
LOCAL void _Spi1Disable (void)
{
    /* Disable Spi1 apb clock */
    //CLOCK_LOG_TRACE:"[_Spi1Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1185_112_2_17_23_3_48_585,(uint8*)"");

    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_AP_APB_DISABLE = SYS_CTRL_DISABLE_AP_APB_CLK_ID_SPI2;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _Spi1Reset
 ******************************************************************************/
LOCAL void _Spi1Reset (void)
{
    //CLOCK_LOG_TRACE:"[_Spi1Reset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1194_112_2_17_23_3_48_586,(uint8*)"");

    /* Reset SPI1 clock */
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
	//reset adi module
    hwp_sysCtrl->AP_APB_RST_SET = SYS_CTRL_SET_AP_APB_RST_ID_SPI2;
    DELAY_CYCLE(1000)
	
    hwp_sysCtrl->AP_APB_RST_CLR = SYS_CTRL_CLR_AP_APB_RST_ID_SPI2;
    DELAY_CYCLE(1000)
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _I2s0Enable
 ******************************************************************************/
LOCAL void _I2s0Enable (void)
{
}	

/******************************************************************************
 * _I2s0Disable
 ******************************************************************************/
LOCAL void _I2s0Disable (void)
{
}

/******************************************************************************
 * _I2sReset
 ******************************************************************************/
LOCAL void _I2sReset (void)
{
}

/******************************************************************************
 * _Uart0Enable
 ******************************************************************************/
LOCAL void _Uart0Enable (void)
{
    /* Enable Uart0 apb clock */
    //CLOCK_LOG_TRACE:"[_Uart0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1243_112_2_17_23_3_48_590,(uint8*)"");
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    //hwp_sysCtrl->CLK_AP_APB_ENABLE = SYS_CTRL_ENABLE_AP_APB_CLK_ID_UART1;
    hwp_sysCtrl->CLK_OTHERS_ENABLE = SYS_CTRL_ENABLE_OC_ID_UART1;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _Uart0Disable
 ******************************************************************************/
LOCAL void _Uart0Disable (void)
{
    /* Disable Uart0 apb clock */
    //CLOCK_LOG_TRACE:"[_Uart0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1253_112_2_17_23_3_48_591,(uint8*)"");
	
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    //hwp_sysCtrl->CLK_AP_APB_DISABLE = SYS_CTRL_DISABLE_AP_APB_CLK_ID_UART1;
    hwp_sysCtrl->CLK_OTHERS_DISABLE = SYS_CTRL_DISABLE_OC_ID_UART1;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _Uart0Reset
 ******************************************************************************/
LOCAL void _Uart0Reset (void)
{
    //CLOCK_LOG_TRACE:"[_Uart0Reset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1262_112_2_17_23_3_48_592,(uint8*)"");

    /* Reset uart0 clock */
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
	//reset adi module
    hwp_sysCtrl->AON_APB_RST_SET = SYS_CTRL_SET_AON_APB_RST_ID_UART1;
    DELAY_CYCLE(1000)
	
    hwp_sysCtrl->AON_APB_RST_CLR = SYS_CTRL_CLR_AON_APB_RST_ID_UART1;
    DELAY_CYCLE(1000)
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _Uart1Enable
 ******************************************************************************/
LOCAL void _Uart1Enable (void)
{
    /* Enable Uart0 apb clock */
    //CLOCK_LOG_TRACE:"[_Uart1Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1277_112_2_17_23_3_48_593,(uint8*)"");
    /* set uart2 pll clock to 96MHz */
//    hwp_sysCtrl->Cfg_Clk_Uart[2] |= (1 << 25);
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    //hwp_sysCtrl->CLK_AP_APB_ENABLE = SYS_CTRL_ENABLE_AP_APB_CLK_ID_UART1;
    hwp_sysCtrl->CLK_OTHERS_ENABLE = SYS_CTRL_ENABLE_OC_ID_UART2;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _Uart1Disable
 ******************************************************************************/
LOCAL void _Uart1Disable (void)
{
    /* Disable Uart1 apb clock */
    //CLOCK_LOG_TRACE:"[_Uart1Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1287_112_2_17_23_3_48_594,(uint8*)"");
	
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    //hwp_sysCtrl->CLK_AP_APB_DISABLE = SYS_CTRL_DISABLE_AP_APB_CLK_ID_UART2;
    hwp_sysCtrl->CLK_OTHERS_DISABLE = SYS_CTRL_DISABLE_OC_ID_UART2;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _Uart1Reset
 ******************************************************************************/
LOCAL void _Uart1Reset (void)
{
    //CLOCK_LOG_TRACE:"[_Uart1Reset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1296_112_2_17_23_3_48_595,(uint8*)"");

    /* Reset uart1 clock */
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
	//reset adi module
    hwp_sysCtrl->AP_APB_RST_SET = SYS_CTRL_SET_AP_APB_RST_ID_UART2;
    DELAY_CYCLE(1000)
	
    hwp_sysCtrl->AP_APB_RST_CLR = SYS_CTRL_CLR_AP_APB_RST_ID_UART2;
    DELAY_CYCLE(1000)
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}



/******************************************************************************
 * _PwmEnable
 ******************************************************************************/
LOCAL void _PwmEnable (void)
{
    /* Enable Pwm apb clock */
    //CLOCK_LOG_TRACE:"[_PwmEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1311_112_2_17_23_3_48_596,(uint8*)"");
	
    hwp_sysCtrl->CLK_AON_APB_ENABLE = SYS_CTRL_ENABLE_AON_APB_CLK_ID_PWM;
}

/******************************************************************************
 * _PwmDisable
 ******************************************************************************/
LOCAL void _PwmDisable (void)
{
    /* Disable Pwm apb clock */
    //CLOCK_LOG_TRACE:"[_PwmDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1321_112_2_17_23_3_48_597,(uint8*)"");

    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_AON_APB_DISABLE = SYS_CTRL_DISABLE_AON_APB_CLK_ID_PWM;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _PinEnable
 ******************************************************************************/
LOCAL void _PinEnable (void)
{
    /* Enable Pin apb clock */
    //CLOCK_LOG_TRACE:"[_PinEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1331_112_2_17_23_3_48_598,(uint8*)"");
    //CHIP_REG_SET (APB_EB0_SET,  PIN_REG_EB);
    ANA_REG_OR(ANA_MODULE_EN0, ANA_PINREG_EN);
}

/******************************************************************************
 * _PinDisable
 ******************************************************************************/
LOCAL void _PinDisable (void)
{
    /* Disable Pin apb clock */
    //CLOCK_LOG_TRACE:"[_PinDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1342_112_2_17_23_3_48_599,(uint8*)"");

    //CHIP_REG_SET (APB_EB0_CLR,  PIN_REG_EB);
    ANA_REG_AND(ANA_MODULE_EN0, ~ANA_PINREG_EN);
}

/******************************************************************************
 * _PwmReset
 ******************************************************************************/
LOCAL void _PwmReset (void)
{
    //CLOCK_LOG_TRACE:"[_PwmReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1352_112_2_17_23_3_48_600,(uint8*)"");
    /* Reset GEA clock */
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
	//reset adi module
    hwp_sysCtrl->AON_APB_RST_SET = SYS_CTRL_SET_AON_APB_RST_ID_PWM;
    DELAY_CYCLE(1000)
	
    hwp_sysCtrl->AON_APB_RST_CLR = SYS_CTRL_CLR_AON_APB_RST_ID_PWM;
    DELAY_CYCLE(1000)
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _Aux0Enable
 ******************************************************************************/
LOCAL void _Aux0Enable (void)
{
}

/******************************************************************************
 * _Aux0Disable
 ******************************************************************************/
LOCAL void _Aux0Disable (void)
{
}

/******************************************************************************
 * _CcirReset
 ******************************************************************************/
LOCAL void _CcirReset (void)
{
}

/******************************************************************************
 * _DcamEnable
 ******************************************************************************/
LOCAL void _DcamEnable (void)
{
    /* Enable Dcam ahb clock */
    //CLOCK_LOG_TRACE:"[_DcamEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1401_112_2_17_23_3_48_604,(uint8*)"");

    hwp_sysCtrl->CLK_AP_APB_ENABLE = SYS_CTRL_ENABLE_AP_APB_CLK_ID_CAMERA;
}

/******************************************************************************
 * _DcamDisable
 ******************************************************************************/
LOCAL void _DcamDisable (void)
{
    /* Disable Dcam apb clock */
    //CLOCK_LOG_TRACE:"[_DcamDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1411_112_2_17_23_3_48_605,(uint8*)"");

    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_AP_APB_DISABLE = SYS_CTRL_DISABLE_AP_APB_CLK_ID_CAMERA;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _DcamReset
 ******************************************************************************/
LOCAL void _DcamReset (void)
{
    //CLOCK_LOG_TRACE:"[_DcamReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1420_112_2_17_23_3_48_606,(uint8*)"");

    /* Reset DCAM device */
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
	//reset adi module
    hwp_sysCtrl->AP_APB_RST_SET = SYS_CTRL_SET_AP_APB_RST_ID_CAMERA;
    DELAY_CYCLE(1000)
	
    hwp_sysCtrl->AP_APB_RST_CLR = SYS_CTRL_CLR_AP_APB_RST_ID_CAMERA;
    DELAY_CYCLE(1000)
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _LzmaEnable
 ******************************************************************************/
 void _LzmaEnable (void)
{
    /* Enable Sdio apb clock */
    //CLOCK_LOG_TRACE:"[_LzmaEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1435_112_2_17_23_3_48_607,(uint8*)"");
	
    hwp_sysCtrl->CLK_SYS_AXI_ENABLE = SYS_CTRL_ENABLE_SYS_AXI_CLK_ID_LZMA;
}

/******************************************************************************
 * _LzmaDisable
 ******************************************************************************/
 void _LzmaDisable (void)
{
    /* Disable Sdio apb clock */
    //CLOCK_LOG_TRACE:"[_LzmaDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1445_112_2_17_23_3_48_608,(uint8*)"");
      
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_SYS_AXI_DISABLE = SYS_CTRL_DISABLE_SYS_AXI_CLK_ID_LZMA;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _LzmaReset
 ******************************************************************************/
 void _LzmaReset (void)
{
    //CLOCK_LOG_TRACE:"[_LzmaReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1454_112_2_17_23_3_48_609,(uint8*)"");
   
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
	//reset adi module
    hwp_sysCtrl->SYS_AXI_RST_SET = SYS_CTRL_SET_SYS_AXI_RST_ID_LZMA;
    hwp_sysCtrl->SYS_AHB_RST_SET = SYS_CTRL_SET_SYS_AHB_RST_ID_LZMA;
    hwp_sysCtrl->AP_APB_RST_SET = SYS_CTRL_SET_AP_APB_RST_ID_LZMA;

    DELAY_CYCLE(1000)
	
    hwp_sysCtrl->SYS_AXI_RST_CLR = SYS_CTRL_CLR_SYS_AXI_RST_ID_LZMA;
    hwp_sysCtrl->SYS_AHB_RST_CLR = SYS_CTRL_CLR_SYS_AHB_RST_ID_LZMA;
    hwp_sysCtrl->AP_APB_RST_CLR = SYS_CTRL_CLR_AP_APB_RST_ID_LZMA;

    DELAY_CYCLE(1000)
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _LcmEnable
 ******************************************************************************/
LOCAL void _LcmEnable (void)
{
    /* Enable lcm ahb clock */
    //CLOCK_LOG_TRACE:"[_LcmEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1468_112_2_17_23_3_48_610,(uint8*)"");
	
    hwp_sysCtrl->CLK_SYS_AHB_ENABLE = SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_LCD | SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_GOUDA;
}

/******************************************************************************
 * _LcmDisable
 ******************************************************************************/
LOCAL void _LcmDisable (void)
{
    /* Disable lcm ahb clock */
    //CLOCK_LOG_TRACE:"[_LcmDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1478_112_2_17_23_3_48_611,(uint8*)"");
      
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_SYS_AHB_DISABLE = SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_LCD | SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_GOUDA;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _LcmReset
 ******************************************************************************/
LOCAL void _LcmReset (void)
{
    //CLOCK_LOG_TRACE:"[_LcmReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1487_112_2_17_23_3_48_612,(uint8*)"");
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
	//reset adi module
    hwp_sysCtrl->SYS_AHB_RST_SET = SYS_CTRL_SET_SYS_AHB_RST_ID_LCD | SYS_CTRL_SET_SYS_AHB_RST_ID_GOUDA;
    DELAY_CYCLE(1000)
	
    hwp_sysCtrl->SYS_AHB_RST_CLR = SYS_CTRL_CLR_SYS_AHB_RST_ID_LCD | SYS_CTRL_CLR_SYS_AHB_RST_ID_GOUDA;
    DELAY_CYCLE(1000)
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _LcdcEnable
 ******************************************************************************/
LOCAL void _LcdcEnable (void)
{
    /* Enable lcdc ahb clock */
    //CLOCK_LOG_TRACE:"[_LcdcEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1501_112_2_17_23_3_49_613,(uint8*)"");
	
    hwp_sysCtrl->CLK_SYS_AHB_ENABLE = SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_LCD;
}

/******************************************************************************
 * _LcdcDisable
 ******************************************************************************/
LOCAL void _LcdcDisable (void)
{
    /* Disable lcdc ahb clock */
    //CLOCK_LOG_TRACE:"[_LcdcDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1511_112_2_17_23_3_49_614,(uint8*)"");
      
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_SYS_AHB_DISABLE = SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_LCD;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _LcdcReset
 ******************************************************************************/
LOCAL void _LcdcReset (void)
{
    //CLOCK_LOG_TRACE:"[_LcdcReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1520_112_2_17_23_3_49_615,(uint8*)"");
   
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
	//reset adi module
    hwp_sysCtrl->SYS_AHB_RST_SET = SYS_CTRL_SET_SYS_AHB_RST_ID_LCD;
    DELAY_CYCLE(1000)
	
    hwp_sysCtrl->SYS_AHB_RST_CLR = SYS_CTRL_CLR_SYS_AHB_RST_ID_LCD;
    DELAY_CYCLE(1000)
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _RotaEnable
 ******************************************************************************/
LOCAL void _RotaEnable (void)
{
}

/******************************************************************************
 * _RotaDisable
 ******************************************************************************/
LOCAL void _RotaDisable (void)
{
}

/******************************************************************************
 * _RotaReset
 ******************************************************************************/
LOCAL void _RotaReset (void)
{
}

/******************************************************************************
 * _SfcEnable
 ******************************************************************************/
LOCAL void _SfcEnable (void)
{
}

/******************************************************************************
 * _SfcDisable
 ******************************************************************************/
LOCAL void _SfcDisable (void)
{
}

/******************************************************************************
 * _SfcReset
 ******************************************************************************/
LOCAL void _SfcReset (void)
{
}

/******************************************************************************
 * _VspReset
 ******************************************************************************/
LOCAL void _VspReset (void)
{
}

/******************************************************************************
 * _Sdio0Enable
 ******************************************************************************/
 void _Sdio0Enable (void)
{
    /* Enable Sdio apb clock */
    //CLOCK_LOG_TRACE:"[_SdioEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1666_112_2_17_23_3_49_628,(uint8*)"");
	
    hwp_sysCtrl->CLK_SYS_AXI_ENABLE = SYS_CTRL_ENABLE_SYS_AXI_CLK_ID_SDMMC1;
}

/******************************************************************************
 * _Sdio0Disable
 ******************************************************************************/
 void _Sdio0Disable (void)
{
    /* Disable Sdio apb clock */
    //CLOCK_LOG_TRACE:"[_SdioDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1676_112_2_17_23_3_49_629,(uint8*)"");
      
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_SYS_AXI_DISABLE = SYS_CTRL_DISABLE_SYS_AXI_CLK_ID_SDMMC1;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _SdioReset
 ******************************************************************************/
void _SdioReset (void)
{
    //CLOCK_LOG_TRACE:"[_SdioReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1685_112_2_17_23_3_49_630,(uint8*)"");
   
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
	//reset adi module
    hwp_sysCtrl->AP_APB_RST_SET = SYS_CTRL_SET_AP_APB_RST_ID_SDMMC1;
    DELAY_CYCLE(1000)
	
    hwp_sysCtrl->AP_APB_RST_CLR = SYS_CTRL_CLR_AP_APB_RST_ID_SDMMC1;
    DELAY_CYCLE(1000)
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _UsbEnable
 ******************************************************************************/
LOCAL void _UsbEnable (void)
{
	/* Enable Usb apb clock */
    //CLOCK_LOG_TRACE:"[_UsbEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1699_112_2_17_23_3_49_631,(uint8*)"");

    hwp_sysCtrl->CLK_SYS_AHB_ENABLE = SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_USBC;
}

/******************************************************************************
 * _UsbDisable
 ******************************************************************************/
LOCAL void _UsbDisable (void)
{
    /* Disable Usb apb clock */
    //CLOCK_LOG_TRACE:"[_UsbDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1709_112_2_17_23_3_49_632,(uint8*)"");

    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_SYS_AHB_DISABLE = SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_USBC;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _UsbReset
 ******************************************************************************/
LOCAL void _UsbReset (void)
{
    //CLOCK_LOG_TRACE:"[_UsbReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1718_112_2_17_23_3_49_633,(uint8*)"");

    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
	//reset adi module
    hwp_sysCtrl->SYS_AHB_RST_SET = SYS_CTRL_SET_SYS_AHB_RST_ID_USBC;
    DELAY_CYCLE(1000)
	
    hwp_sysCtrl->SYS_AHB_RST_CLR = SYS_CTRL_CLR_SYS_AHB_RST_ID_USBC;
    DELAY_CYCLE(1000)
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _DmaEnable
 ******************************************************************************/
LOCAL void _DmaEnable (void)
{
    /* Enable Dma apb clock */
    //CLOCK_LOG_TRACE:"[_DmaEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1732_112_2_17_23_3_49_634,(uint8*)"");

    hwp_sysCtrl->CLK_SYS_AHB_ENABLE = SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_AXIDMA;
}

/******************************************************************************
 * _DmaDisable
 ******************************************************************************/
LOCAL void _DmaDisable (void)
{
    /* Disable Dma apb clock */
    //CLOCK_LOG_TRACE:"[_DmaDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1742_112_2_17_23_3_49_635,(uint8*)"");

    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_SYS_AHB_DISABLE = SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_AXIDMA;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _DmaReset
 ******************************************************************************/
LOCAL void _DmaReset (void)
{
    //CLOCK_LOG_TRACE:"[_DmaReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1751_112_2_17_23_3_49_636,(uint8*)"");

    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
	//reset adi module
    hwp_sysCtrl->SYS_AHB_RST_SET = SYS_CTRL_SET_SYS_AHB_RST_ID_AXIDMA;
    DELAY_CYCLE(1000)
	
    hwp_sysCtrl->SYS_AHB_RST_CLR = SYS_CTRL_CLR_SYS_AHB_RST_ID_AXIDMA;
    DELAY_CYCLE(1000)
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
}

/******************************************************************************
 * _Bm0Enable
 ******************************************************************************/
LOCAL void _Bm0Enable (void)
{
}

/******************************************************************************
 * _Bm0Disable
 ******************************************************************************/
LOCAL void _Bm0Disable (void)
{
}

/******************************************************************************
 * _AhbEnable
 ******************************************************************************/
LOCAL void _AhbEnable (void)
{
}

/******************************************************************************
 * _AhbDisable
 ******************************************************************************/
LOCAL void _AhbDisable (void)
{
}

/******************************************************************************
 * _ApbEnable
 ******************************************************************************/
LOCAL void _ApbEnable (void)
{
}

/******************************************************************************
 * _ApbDisable
 ******************************************************************************/
LOCAL void _ApbDisable (void)
{
}

/******************************************************************************
 * _EmcEnable
 ******************************************************************************/
LOCAL void _EmcEnable (void)
{
    //CLOCK_LOG_TRACE:"[_EmcEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1860_112_2_17_23_3_49_647,(uint8*)"");

    hwp_sysCtrl->CLK_AP_APB_ENABLE = SYS_CTRL_ENABLE_AP_APB_CLK_ID_SDMMC2;
    return;
}

/******************************************************************************
 * _EmcDisable
 ******************************************************************************/
LOCAL void _EmcDisable (void)
{
    //CLOCK_LOG_TRACE:"[_AhbDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1870_112_2_17_23_3_49_648,(uint8*)"");

    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_AP_APB_DISABLE = SYS_CTRL_DISABLE_AP_APB_CLK_ID_SDMMC2;
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/
    return;
}

/******************************************************************************
 * _EmcReset
 ******************************************************************************/
LOCAL void _EmcReset (void)
{
    //CLOCK_LOG_TRACE:"[_EmcReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1880_112_2_17_23_3_49_649,(uint8*)"");

    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
	//reset adi module
    hwp_sysCtrl->AP_APB_RST_SET = SYS_CTRL_SET_AP_APB_RST_ID_SDMMC2;
    DELAY_CYCLE(1000)
	
    hwp_sysCtrl->AP_APB_RST_CLR = SYS_CTRL_CLR_AP_APB_RST_ID_SDMMC2;
    DELAY_CYCLE(1000)
    //hwp_sysCtrl->REG_DBG = 0xa50000;     /*do not allow to set disable_reg*/

    return;
}

/******************************************************************************
 * _AudioEnable
 ******************************************************************************/
LOCAL void _AudioEnable (void)
{
}

/******************************************************************************
 * _AudioDisable
 ******************************************************************************/
LOCAL void _AudioDisable (void)
{
}

/******************************************************************************
 * _AudioReset
 ******************************************************************************/
LOCAL void _AudioReset (void)
{
}

/******************************************************************************
 * _Sim0Enable
 ******************************************************************************/
LOCAL void _DummyEnable (void)
{
}

/******************************************************************************
 * _Sim0Disable
 ******************************************************************************/
LOCAL void _DummyDisable (void)
{
}

/******************************************************************************
 * _Sim0Reset
 ******************************************************************************/
LOCAL void _DummyReset (void)
{
}

/******************************************************************************
 * _CALEnable
 ******************************************************************************/
 LOCAL void _CALEnable()
{
// 	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1948_112_2_17_23_3_49_653,(uint8*)"");

	ANA_REG_OR (ANA_MODULE_EN0, ANA_CAL_EN);
	ANA_REG_OR (ANA_ARM_CLK_EN0,  ANA_CLK_CAL_EN);
}

/******************************************************************************
* _CALDisable
******************************************************************************/
LOCAL void _CALDisable()
{
//	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1956_112_2_17_23_3_49_654,(uint8*)"");

	ANA_REG_AND (ANA_MODULE_EN0, ~ANA_CAL_EN);
	ANA_REG_AND (ANA_ARM_CLK_EN0,  ~ANA_CLK_CAL_EN);
}
/******************************************************************************
* _CALReset
******************************************************************************/
LOCAL void _CALReset()
{
//	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1964_112_2_17_23_3_49_655,(uint8*)"");

	ANA_REG_OR (ANA_SOFT_RST0, ANA_CAL_SOFT_RST);
	DELAY_CYCLE(1000)

	ANA_REG_AND (ANA_SOFT_RST0, ~ANA_CAL_SOFT_RST);
	DELAY_CYCLE(1000)
}

/******************************************************************************
 * _AUDIFEnable
 ******************************************************************************/
 LOCAL void _AudIfEnable()
{
	//CHIP_REG_SET (APB_EB0_SET,  AUD_EB);
	ANA_REG_OR (ANA_ARM_CLK_EN0,  ANA_CLK_AUD_IF_EN);
}

/******************************************************************************
* _AUDIFDisable
******************************************************************************/
LOCAL void _AudIfDisable()
{
	//CHIP_REG_SET(APB_EB0_CLR,  AUD_EB);
	ANA_REG_AND (ANA_ARM_CLK_EN0,  ~ANA_CLK_AUD_IF_EN);
}
/******************************************************************************
* _AUDIFReset
******************************************************************************/
LOCAL void _AudIfReset()
{
}

/******************************************************************************
 * Static functions prototypes
 ******************************************************************************/
LOCAL const CLOCK system_clock[] =
{
    //.Name     //.value   //.DefaultSrc //.bEnable //.Enable //.Disable
    {
	    "CLK_32K",     32768,  "CLK_32K",    SCI_TRUE, _RtcXtlEnable,_RtcXtlDisable,//RC32K && DCXO-32K MUX 
    },    
    {
	    "26M_XTL",26000000,  "26M_XTL",    SCI_TRUE, _26MXtlEnable,_26MXtlDisable,
    },
    {
	    "PAD_CLK",0,  "PAD_CLK",    SCI_TRUE, SCI_NULL,SCI_NULL,
    },
    {
	    "26M_UNTUNED",26000000,  "26M_XTL",    SCI_TRUE, SCI_NULL,SCI_NULL,
    },
    {
	    "26M_RAW",26000000,  "26M_XTL",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
	    "APLL",624000000,  "26M_RAW",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
	    "CPLL",832000000,  "26M_RAW",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
	    "26M_TUNED",26000000,  "CPLL",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
//
    {
	    "CLK_26M",26000000,  "26M_UNTUNED",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
//    
    {
	    "CLK_26M_RAW_INV",26000000,  "26M_RAW",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
	    "CLK_26M_RAW",26000000,  "26M_RAW",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
	    "CLK_1M_RAW",1000000,  "26M_RAW",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
	    "CLK_32K768",32768,  "26M_RAW",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
//
    {
	    "CLK_26M_TUNED_INV",26000000,  "26M_TUNED",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
	    "CLK_26M_TUNED",26000000,  "26M_TUNED",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
	    "CLK_13M",13000000,  "26M_TUNED",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
	    "CLK_6M5",6500000,  "26M_TUNED",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
	    "CLK_6M5_INV",6500000,  "26M_TUNED",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
	    "CLK_1M083",1083000,  "26M_TUNED",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
//
    {
	    "CLK_208M",208000000,  "CPLL",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
	    "CLK_166M4",166400000,  "CPLL",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
	    "CLK_138M6",138600000,  "CPLL",       SCI_TRUE, SCI_NULL, SCI_NULL, //NC
    },
#ifdef ARM_APLLCLK_SUPPORT
    {
	    "CLK_104M_APLL",104000000,  "APLL",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
#endif
    {
	    "CLK_104M",104000000,  "CPLL",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
	    "CLK_83M2",83200000,  "CPLL",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
	    "CLK_75M6",75600000,  "CPLL",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
	    "CLK_64M",64000000,  "CPLL",       SCI_TRUE, SCI_NULL, SCI_NULL, //CLK_64M_BT
    },
    {
	    "CLK_64M_FM",64000000,  "CPLL",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
	    "CLK_64M_FM_INV",64000000,  "CPLL",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
	    "CLK_52M",52000000,  "CPLL",       SCI_TRUE, SCI_NULL, SCI_NULL, //NC
    },
    {
	    "CLK_46M2",46200000,  "CPLL",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
	    "CLK_37M8",37800000,  "CPLL",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
//
    {
	    "CLK_208M_APLL",208000000,  "APLL",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
	    "CLK_156M",156000000,  "APLL",       SCI_TRUE, SCI_NULL, SCI_NULL,
    },
    {
	    "CLK_124M8",124800000,  "APLL",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
#ifndef ARM_APLLCLK_SUPPORT
    {
	    "CLK_104M_APLL",104000000,  "APLL",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
#endif
    {
	    "CLK_89M1",89100000,  "APLL",       SCI_TRUE, SCI_NULL, SCI_NULL, //NC
    },
    {
	    "CLK_78M",78000000,  "APLL",       SCI_TRUE, SCI_NULL, SCI_NULL, //NC
    },
    {
	    "CLK_48M",48000000,  "APLL",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
	    "CLK_24M",24000000,  "APLL",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
	    "CLK_12M",12000000,  "APLL",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
//
    {
	    "CLK_32K",32768,  "CLK_32K",       SCI_TRUE, SCI_NULL, SCI_NULL,
    },
    {
	    "CLK_1K",1000,  "CLK_32K",       SCI_TRUE, SCI_NULL, SCI_NULL,
    }, 
	// This clock inputs in ADC, CAL, AUDIO modules in ADIE.
	// By default, it's closed.
	// Access it after ADI interfaces initialized.
    {
	    "ADIE_CLK_26M",  26000000,  "26M_UNTUNED", SCI_TRUE, _adieClk26MEnable, _adieClk26MDisable,        
    },
};

LOCAL const DEVICE system_device[] =
{
  //.Name         //.DefaultSrc  //.ParentDe //.Div        //.bEnable
  //.Enable       //.Disable     //.SrcSet   //.DivSet     //.Reset
{
    "ARM",        208000000,      "ARM",      0,            SCI_TRUE,
    SCI_NULL,     SCI_NULL,      SCI_NULL, SCI_NULL,     SCI_NULL,
},
{
    "AHB",        104000000,      "ARM",      1,            SCI_TRUE,
    _AhbEnable,   _AhbDisable,   SCI_NULL,   SCI_NULL,     SCI_NULL,
},
{
    "APB",        26000000,      "AHB",      0,            SCI_TRUE,
    _ApbEnable,   _ApbDisable,   SCI_NULL,   SCI_NULL,     SCI_NULL,
},


  // Description for Devices in Analog Die
  //.Name         //.DefaultSrc  //.ParentDe //.Div        //.bEnable
  //.Enable       //.Disable     //.SrcSet   //.DivSet     //.Reset
{
    "ADI",        SCI_NULL,      "ADI",      0,            SCI_TRUE, 
    _AdiEnable,   _AdiDisable,   _AdiSrcSet,   SCI_NULL,     _AdiReset,   
},  
{
    "WATCHDOG",   SCI_NULL,      "ADI",      0,            SCI_TRUE,
    _WdgEnable,   _WdgDisable,   SCI_NULL,   SCI_NULL,     _WdgReset, 
}, 
{
    "ADC",        SCI_NULL,      "ADI",      0,            SCI_TRUE, 
    _AdcEnable,   _AdcDisable,   SCI_NULL,   SCI_NULL,     _AdcReset,
},
{
    "RTC",        SCI_NULL,      "ADI",      0,            SCI_TRUE, 
    _RtcEnable,   _RtcDisable,   SCI_NULL,    SCI_NULL,    _RtcReset,
},
/* add begin by haibing.chen, 2018-10-12 */
{
    "PMIC_EFUSE",        SCI_NULL,      "ADI",      0,            SCI_TRUE, 
    _PmicEfuseEnable,   _PmicEfuseDisable,   SCI_NULL,    SCI_NULL,    _PmicEfuseReset,
},
/* add end by haibing.chen, 2018-10-12 */

{
    "EIC_A",      SCI_NULL,       "ADI",      0,           SCI_TRUE,      
    _EicaEnable,  _EicaDisable,   SCI_NULL,   SCI_NULL,    _EicaReset,      
},

  // Description for Devices in APB BUS
  //.Name         //.DefaultSrc  //.ParentDe //.Div        //.bEnable
  //.Enable       //.Disable     //.SrcSet   //.DivSet     //.Reset
{
    "TMR",        SCI_NULL,      "APB",      0,            SCI_TRUE,      
    _TimerEnable, _TimerDisable, SCI_NULL,   SCI_NULL,     _TimerReset,      
},
{
    "TMRBK",      SCI_NULL,      "APB",      0,            SCI_TRUE,      
    _TimerbkEnable,_TimerbkDisable,SCI_NULL, SCI_NULL,     _TimerbkReset,      
},
{
    "SYSCOUNTER", SCI_NULL,       "APB",      0,           SCI_TRUE,      
    _SyscntEnable,_SyscntDisable, SCI_NULL,   SCI_NULL,    _SyscntReset,      
},
{
    "KEYPAD",     SCI_NULL,       "APB",      0,           SCI_TRUE,      
    _KpdEnable,   _KpdDisable,    SCI_NULL,   SCI_NULL,    _KpdReset,      
},	
{
     "GPIO_D",    SCI_NULL,       "APB",      0,           SCI_TRUE,      
    _GpiodEnable, _GpiodDisable, SCI_NULL,   SCI_NULL,    _GpiodReset,      
},
{
     "EIC_D",     SCI_NULL,       "APB",      0,           SCI_TRUE,      
     _EicdEnable,  _EicdDisable,  SCI_NULL,   SCI_NULL,    _EicdReset,      
},
{
     "SIM0",      SCI_NULL,       "APB",      0,           SCI_FALSE,      
     _Sim0Enable, _Sim0Disable,   SCI_NULL,   SCI_NULL,    _Sim0Reset,      
},
{
     "SIM1",      SCI_NULL,       "APB",      0,           SCI_FALSE,      
     _DummyEnable, _DummyDisable,   SCI_NULL,   SCI_NULL,    _DummyReset,      
},
{
     "GEA",       SCI_NULL,       "APB",      0,           SCI_TRUE,       
     _GeaEnable,  _GeaDisable,    SCI_NULL,   SCI_NULL,    _GeaReset,       
},
/*remove SPI1*/
#ifdef FPGA_VERIFICATION     
{
     "SPI1",      48000000,       "AHB",      0,           SCI_TRUE,       
     _Spi1Enable, _Spi1Disable,   _Spi1SrcSet,_Spi1SrcDivSet,_Spi1Reset,       
},
#else
{
     "SPI1",      104000000,       "AHB",      0,           SCI_TRUE,       
     _Spi1Enable, _Spi1Disable,   _Spi1SrcSet,_Spi1SrcDivSet,_Spi1Reset,       
},
#endif
{
     "UART0",     SCI_NULL,       "APB",      0,           SCI_TRUE,      
     _Uart0Enable,_Uart0Disable,  _Uart0SrcSet,SCI_NULL,   _Uart0Reset,      
},
{
     "UART1",     SCI_NULL,       "APB",      0,           SCI_TRUE,      
     _Uart1Enable,_Uart1Disable,  _Uart1SrcSet,SCI_NULL,   _Uart1Reset,      
},
{
     "PWM",       26000000,       "APB",      0,           SCI_TRUE,     
     _PwmEnable,  _PwmDisable,    _PwmSrcSet, SCI_NULL,    _PwmReset,     
},
{
     "PIN",       SCI_NULL,       "APB",      0,           SCI_TRUE,     
     _PinEnable,  _PinDisable,    SCI_NULL, SCI_NULL,    SCI_NULL,     
},
{
     "PCM",      SCI_NULL,       "APB",      0,           SCI_TRUE,      
     _I2s0Enable, _I2s0Disable,   _I2s0SrcSet, _I2s0SrcDivSet,_I2sReset,      
},
{
     "AUX0",      SCI_NULL,       "APB",      0,           SCI_TRUE,      
     _Aux0Enable, _Aux0Disable,   _AuxSrcSet, _AuxSrcDivSet,SCI_NULL,      
},
{
     "I2C0",      SCI_NULL,       "APB",      0,           SCI_TRUE,     
     _IIC0Enable, _IIC0Disable,   _IIC0SrcSet,   SCI_NULL,    _IIC0Reset,     
},
{
     "I2C1",      SCI_NULL,       "APB",      0,           SCI_TRUE,     
     _IIC1Enable, _IIC1Disable,   _IIC1SrcSet,   SCI_NULL,    _IIC1Reset,     
},
{
     "AUDIO",     SCI_NULL,       "APB",      0,           SCI_FALSE,      
     _AudioEnable,_AudioDisable,  _AudSrcSet,   SCI_NULL,    _AudioReset,       
},
  // Description for Devices in AHB BUS
  //.Name         //.DefaultSrc  //.ParentDe //.Div        //.bEnable
  //.Enable       //.Disable     //.SrcSet   //.DivSet     //.Reset
{
     "CCIR",      48000000,       "AHB",      0,           SCI_TRUE,     
     _SensorClkEnable,    _SensorClkDisable,       _SensorClkSrcSet,   _SensorSrcDivSet,    _CcirReset,   
},
#ifdef ARM_APLLCLK_SUPPORT
{
     "SPI0",      104000000,       "AHB",      0,           SCI_TRUE,       
     _Spi0Enable, _Spi0Disable,   _Spi0SrcSet,_Spi0SrcDivSet,_Spi0Reset,       
},
#else
{
     "SPI0",      75600000,       "AHB",      0,           SCI_TRUE,       
     _Spi0Enable, _Spi0Disable,   _Spi0SrcSet,_Spi0SrcDivSet,_Spi0Reset,       
},
#endif
{
     "DCAM",      SCI_NULL,       "AHB",      0,           SCI_TRUE,      
     _DcamEnable, _DcamDisable,   _DcamSrcSet,SCI_NULL,    _DcamReset,    
},
{
     "SDIO0",     48000000,       "AHB",      0,           SCI_TRUE,       
     _Sdio0Enable,_Sdio0Disable,  _SdioSrcSet,SCI_NULL,    _SdioReset,     
},
{
     "EMC",       SCI_NULL,       "AHB",      0,           SCI_TRUE, 
	 _EmcEnable,  _EmcDisable,    SCI_NULL,   SCI_NULL,    _EmcReset,     
},
{
     "USB",       SCI_NULL,       "AHB",      0,           SCI_FALSE,     
     _UsbEnable,  _UsbDisable,    _Usb12SrcSet,   SCI_NULL,    _UsbReset,     
},
{
     "DMA",       SCI_NULL,       "AHB",      0,           SCI_TRUE,      
     _DmaEnable,  _DmaDisable,    SCI_NULL,   SCI_NULL,    _DmaReset,     
},
{
     "BUSMONITOR0",SCI_NULL,      "AHB",      0,           SCI_TRUE,      
     _Bm0Enable,  _Bm0Disable,    SCI_NULL,   SCI_NULL,    SCI_NULL,     
},
{
      "LZMA",     SCI_NULL,       "AHB",      0,           SCI_TRUE,      
      _LzmaEnable,_LzmaDisable,   _LzmaSrcSet,SCI_NULL,    _LzmaReset,     
},
{
      "ROTA",     SCI_NULL,       "AHB",      0,           SCI_TRUE,      
      _RotaEnable,_RotaDisable,   SCI_NULL,   SCI_NULL,    _RotaReset,     
},
{
      "VSP",      SCI_NULL,       "AHB",      0,           SCI_TRUE,      
      SCI_NULL,   SCI_NULL,      SCI_NULL,   SCI_NULL,     _VspReset,     
},
{
      "LCDC",     SCI_NULL,       "AHB",      0,           SCI_TRUE,      
      _LcdcEnable,_LcdcDisable,   SCI_NULL,   SCI_NULL,    _LcdcReset,     
},
{
      "LCM",     SCI_NULL,        "AHB",      0,           SCI_TRUE,      
      _LcmEnable,_LcmDisable,     SCI_NULL,   SCI_NULL,    _LcmReset,     
},
{
      "SFC",     SCI_NULL,        "AHB",      0,           SCI_TRUE,      
      _SfcEnable,_SfcDisable,     SCI_NULL,   SCI_NULL,    _SfcReset,     
},
};


/******************************************************************************
 * Implement
 ******************************************************************************/
PUBLIC void CLOCKCFG_Get (CLOCK_CFG *thiz)
{
    SCI_ASSERT (SCI_NULL != thiz);/*assert verified*/

    thiz->Clock = (CLOCK *)system_clock;
    thiz->ClockNr = sizeof (system_clock) / sizeof (struct _CLOCK);
    thiz->Device = (DEVICE *)system_device;
    thiz->DeviceNr = sizeof (system_device) / sizeof (struct _DEVICE);
}

