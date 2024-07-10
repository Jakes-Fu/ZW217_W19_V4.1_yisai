/******************************************************************************
 ** File Name:      sc6530_clock_cfg.c                                      *
 ** Author:         Yi.Qiu                                                  *
 ** DATE:           02/21/2010                                              *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.       *
 ** Description:                                                             *
 ******************************************************************************

 ******************************************************************************
 **                 Edit History                                               *
 ** ------------------------------------------------------------------------*
 ** DATE               NAME             DESCRIPTION                            *
 ** 12/28/2011         lichd            Create.                                 *
 ******************************************************************************/
#include "chip_drv_trc.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "../clock.h"
#include "Ref_outport.h"

static const char *err_string = "%s choosed a wrong source [%s]\r\n please choose one of: [%s]\r\n";

#define DELAY_CYCLE(n)  {\
                                            uint32 k=0;\
                                             for(k=0; k<n; k++){}\
                                         }

/******************************************************************************
 * _DcamSrcSet
 ******************************************************************************/
LOCAL void _DcamSrcSet (const char *SrcName)
{
    CHIP_REG_AND(APB_CLK_CFG0, ~CLK_DCAM_SEL_MSK);

    if (strcmp ("CLK_78M", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 0<<CLK_DCAM_SEL_SHIFT);
    }
    else if (strcmp ("CLK_89M1", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 1<<CLK_DCAM_SEL_SHIFT);
    }
    else if (strcmp ("CLK_104M", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 2<<CLK_DCAM_SEL_SHIFT);
    }
    else if (strcmp ("CLK_48M", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 3<<CLK_DCAM_SEL_SHIFT);
    }	
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_DcamSrcSet", SrcName, "CLK_78M, CLK_89M1, CLK_104M, CLK_48M"));
    }

}

/******************************************************************************
 * _SensorClkSrcSet
 ******************************************************************************/
LOCAL void _SensorClkSrcSet (const char *SrcName)
{
    CHIP_REG_AND(APB_CLK_CFG0, ~CLK_SENSOR_SEL_MSK);

    if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 0<<CLK_SENSOR_SEL_SHIFT);
    }
    else if (strcmp ("CLK_78M", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 1<<CLK_SENSOR_SEL_SHIFT);
    }		
    else if (strcmp ("CLK_89M1", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 2<<CLK_SENSOR_SEL_SHIFT);
    }
    else if (strcmp ("CLK_48M", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 3<<CLK_SENSOR_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_SensorClkSrcSet", SrcName, "26M_XTL, CLK_78M, CLK_89M1, CLK_48M"));
    }
}

/******************************************************************************
 * _SensorSrcDivSet
 ******************************************************************************/
LOCAL void _SensorSrcDivSet (int Div)
{
    SCI_PASSERT ( (3 >= Div), ("_SensorSrcDivSet exceeds the max_value."));/*assert verified*/
	
    CHIP_REG_AND(APB_CLK_CFG0, ~CLK_SENSOR_DIV_MSK);
	CHIP_REG_OR(APB_CLK_CFG0, Div<<CLK_SENSOR_DIV_SHIFT);
}

/******************************************************************************
 * _SensorClkEnable
 ******************************************************************************/
LOCAL void _SensorClkEnable (void)
{
	/* Enable sensor clock */
    //CLOCK_LOG_TRACE:"[_SensorClkEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_96_112_2_17_23_3_46_518,(uint8*)"");

    CHIP_REG_OR(APB_CLK_CFG0, CLK_SENSOR_EN);
}

/******************************************************************************
 * _SensorClkDisable
 ******************************************************************************/
LOCAL void _SensorClkDisable (void)
{
    /* Disable sensor clock */
    //CLOCK_LOG_TRACE:"[_SensorClkDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_106_112_2_17_23_3_46_519,(uint8*)"");

    CHIP_REG_AND(APB_CLK_CFG0, ~CLK_SENSOR_EN);
}

/******************************************************************************
 * _ArmSrcSet
 ******************************************************************************/
LOCAL void _ArmSrcSet (const char *SrcName)
{
    uint32 tmp = 0;

    tmp = CHIP_REG_GET(APB_CLK_CFG0);

    tmp &= ~CLK_MCU_SEL_MSK;
    //CHIP_REG_AND(APB_CLK_CFG0, ~CLK_MCU_SEL_MSK);

    if (strcmp ("26M_XTL", SrcName) == 0)
    {
        tmp |= 0<<CLK_MCU_SEL_SHIFT;
    }
    else if (strcmp ("CLK_208M", SrcName) == 0)
    {
        tmp |= 1<<CLK_MCU_SEL_SHIFT;
    }
    else if (strcmp ("CLK_104M", SrcName) == 0)
    {
        tmp |= 2<<CLK_MCU_SEL_SHIFT;
    }
    else if (strcmp ("CLK_156M", SrcName) == 0)
    {
        tmp |= 3<<CLK_MCU_SEL_SHIFT;
    }	
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_ArmSrcSet", SrcName, "26M_XTL, CLK_208M, CLK_104M, CLK_156M"));
    }

    CHIP_REG_SET(APB_CLK_CFG0, tmp);

}

/******************************************************************************
 * _AhbSrcDivSet
 ******************************************************************************/
LOCAL void _AhbSrcDivSet (int Div)
{
    uint32 tmp = 0;
    SCI_PASSERT ( (1 >= Div), ("_AhbSrcDivSet exceeds the max_value."));/*assert verified*/

    tmp = CHIP_REG_GET(APB_CLK_CFG0);
    
    tmp &= ~CLK_AHB_DIV2_EN;
    
    if(1 == Div)
	    tmp |= CLK_AHB_DIV2_EN;

    CHIP_REG_SET(APB_CLK_CFG0, tmp);
}

/******************************************************************************
 * _SdioSrcSet
 ******************************************************************************/
LOCAL void _SdioSrcSet (const char *SrcName)
{
    CHIP_REG_AND(APB_CLK_CFG0, ~CLK_SDIO_SEL_MSK);

    if (strcmp ("CLK_104M", SrcName) == 0)
    {
        CHIP_REG_AND(APB_CLK_CFG0, ~(1<<CLK_SDIO_SEL_SHIFT));
    }
    else if (strcmp ("CLK_48M", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 1<<CLK_SDIO_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_SdioSrcSet", SrcName, "CLK_104M, CLK_48M"));
    }

}

/******************************************************************************
 * _I2sSrcSet
 ******************************************************************************/
LOCAL void _I2sSrcSet (const char *SrcName)
{
    CHIP_REG_AND(APB_CLK_CFG1, ~CLK_IIS_SEL_MCU_MSK);
	
    if (strcmp ("CLK_78M", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG1, 0<<CLK_IIS_SEL_MCU_SHIFT);
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG1, 1<<CLK_IIS_SEL_MCU_SHIFT);
    }
    else if (strcmp ("CLK_89M1", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG1, 2<<CLK_IIS_SEL_MCU_SHIFT);
    }
    else if (strcmp ("CLK_48M", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG1, 3<<CLK_IIS_SEL_MCU_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_I2sSrcSet", SrcName, "CLK_78M,26M_XTL,CLK_89M1,CLK_48M"));
    }
}

/******************************************************************************
 * _I2sSrcDivSet
 ******************************************************************************/
LOCAL void _I2sSrcDivSet (int Div)
{
    SCI_PASSERT ( (0x3F >= Div), ("_I2sSrcDivSet exceeds the max_value."));/*assert verified*/
	
    CHIP_REG_AND(APB_CLK_CFG0, ~CLK_IIS_DIV_MCU_MSK);
	CHIP_REG_OR(APB_CLK_CFG0, Div<<CLK_IIS_DIV_MCU_SHIFT);
}

/******************************************************************************
 * _Uart0SrcSet
 ******************************************************************************/
LOCAL void _Uart0SrcSet (const char *SrcName)
{
    if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_AND(APB_CLK_CFG0, ~CLK_UART0_SEL);
    }
    else if (strcmp ("CLK_104M", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, CLK_UART0_SEL);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Uart0SrcSet", SrcName, "26M_XTL, CLK_104M"));
    }
}

/******************************************************************************
 * _Uart1SrcSet
 ******************************************************************************/
LOCAL void _Uart1SrcSet (const char *SrcName)
{
    if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_AND(APB_CLK_CFG0, ~CLK_UART1_SEL_MCU);
    }
    else if (strcmp ("CLK_104M", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, CLK_UART1_SEL_MCU);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Uart1SrcSet", SrcName, "26M_XTL, CLK_104M"));
    }
}

/******************************************************************************
 * _Pwm0SrcSet
 ******************************************************************************/
LOCAL void _PwmSrcSet (const char *SrcName)
{
    if (strcmp ("CLK_32K", SrcName) == 0)
    {
        CHIP_REG_AND(APB_CLK_CFG0, ~CLK_PWM_SEL);
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, CLK_PWM_SEL);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_PwmSrcSet", SrcName, "CLK_32K, 26M_XTL"));
    }
}

/******************************************************************************
 * _Spi0SrcSet
 ******************************************************************************/
LOCAL void _Spi0SrcSet (const char *SrcName)
{
    CHIP_REG_AND(APB_CLK_CFG0, ~CLK_SPI0_SEL_MSK);
		
    if (strcmp ("CLK_78M", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 0<<CLK_SPI0_SEL_SHIFT);
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 1<<CLK_SPI0_SEL_SHIFT);
    }
    else if (strcmp ("CLK_104M", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 2<<CLK_SPI0_SEL_SHIFT);
    }
    else if (strcmp ("CLK_48M", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 3<<CLK_SPI0_SEL_SHIFT);
    }	
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Spi0SrcSet", SrcName, "CLK_78M, 26M_XTL, CLK_104M,CLK_4M"));
    }
}

/******************************************************************************
 * _Spi0SrcDivSet
 ******************************************************************************/
LOCAL void _Spi0SrcDivSet (int Div)
{
    SCI_PASSERT ( (1 >= Div), ("_Spi0SrcDivSet exceeds the max_value."));/*assert verified*/
	
    CHIP_REG_AND(APB_CLK_CFG0, ~CLK_SPI0_DIV_MSK);
	CHIP_REG_OR(APB_CLK_CFG0, Div<<CLK_SPI0_DIV_SHIFT);
}

/******************************************************************************
 * _Spi1SrcSet
 ******************************************************************************/
LOCAL void _Spi1SrcSet (const char *SrcName)
{
    CHIP_REG_AND(APB_CLK_CFG0, ~CLK_SPI1_SEL_MSK);
		
    if (strcmp ("CLK_78M", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 0<<CLK_SPI1_SEL_SHIFT);
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 1<<CLK_SPI1_SEL_SHIFT);
    }
    else if (strcmp ("CLK_104M", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 2<<CLK_SPI1_SEL_SHIFT);
    }
    else if (strcmp ("CLK_48M", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 3<<CLK_SPI1_SEL_SHIFT);
    }		
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Spi1SrcSet", SrcName, "CLK_78M, 26M_XTL, CLK_104M,CLK_4M"));
    }
}

/******************************************************************************
 * _Spi1SrcDivSet
 ******************************************************************************/
LOCAL void _Spi1SrcDivSet (int Div)
{
    SCI_PASSERT ( (1 >= Div), ("_Spi1SrcDivSet exceeds the max_value."));/*assert verified*/
	
    CHIP_REG_AND(APB_CLK_CFG0, ~CLK_SPI1_DIV_MSK);
	CHIP_REG_OR(APB_CLK_CFG0, Div<<CLK_SPI1_DIV_SHIFT);
}

/******************************************************************************
 * _AuxSrcSet
 ******************************************************************************/
LOCAL void _AuxSrcSet (const char *SrcName)
{
    CHIP_REG_AND(APB_CLK_CFG0, ~CLK_AUX_SEL_MSK);
		
    if (strcmp ("CLK_32K", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 0<<CLK_AUX_SEL_SHIFT);
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 1<<CLK_AUX_SEL_SHIFT);
    }
    else if (strcmp ("CLK_48K", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 2<<CLK_AUX_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_AuxSrcSet", SrcName, "CLK_32K, 26M_XTL, CLK_48K"));
    }
}

/******************************************************************************
 * _SpiSrcDivSet
 ******************************************************************************/
LOCAL void _AuxSrcDivSet (int Div)
{
    SCI_PASSERT ( (3 >= Div), ("_AuxSrcDivSet exceeds the max_value."));/*assert verified*/
	
    CHIP_REG_AND(APB_CLK_CFG0, ~CLK_AUX_DIV_MSK);
	CHIP_REG_OR(APB_CLK_CFG0, Div<<CLK_AUX_DIV_SHIFT);
}

/******************************************************************************
 * _LzmaSrcSet
 ******************************************************************************/
LOCAL void _LzmaSrcSet (const char *SrcName)
{
    CHIP_REG_AND(APB_CLK_CFG0, ~CLK_ZIP_SEL_MSK);
		
    if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 0x0UL<<CLK_ZIP_SEL_SHIFT);
    }
    else if (strcmp ("CLK_208M", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 0x1UL<<CLK_ZIP_SEL_SHIFT);
    }
    else if (strcmp ("CLK_104M", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG0, 0x2UL<<CLK_ZIP_SEL_SHIFT);
    }		
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_LzmaSrcSet", SrcName, "26M_XTL, CLK_104M,CLK_208M"));
    }
}

/******************************************************************************
 * _AdiSrcSet
 ******************************************************************************/
LOCAL void _AdiSrcSet (const char *SrcName)
{

    if (strcmp ("CLK_784M", SrcName) == 0)
    {
        CHIP_REG_AND(APB_CLK_CFG1, ~CLK_ADI_SEL_MCU);
    }
    else if (strcmp ("CLK_62M4", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG1, CLK_ADI_SEL_MCU);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_AdiSrcSet", SrcName, "CLK_78M, CLK_62M4"));
    }

}

/******************************************************************************
 * _RtcXtlEnable
 ******************************************************************************/
LOCAL void _RtcXtlEnable (void)
{
    /*-----default as open--*/
    //CLOCK_LOG_TRACE:"[_RtcXtlEnable] default is open"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_447_112_2_17_23_3_47_520,(uint8*)"");
}

/******************************************************************************
 * _RtcXtlDisable
 ******************************************************************************/
LOCAL void _RtcXtlDisable (void)
{
    /*-----can't be closed--*/
    //CLOCK_LOG_TRACE:"[_RtcXtlDisable] can't be closed."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_455_112_2_17_23_3_47_521,(uint8*)"");
}

/******************************************************************************
 * _MpllEnable
 ******************************************************************************/
LOCAL void _MpllEnable (void)
{
    /*-----default as open--*/
    //CLOCK_LOG_TRACE:"[_MpllEnable] default is open"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_463_112_2_17_23_3_47_522,(uint8*)"");
}

/******************************************************************************
 * _MpllDisable
 ******************************************************************************/
LOCAL void _MpllDisable (void)
{
    /*-----can't be closed--*/	
    //CLOCK_LOG_TRACE:"[_MpllDisable] can't be closed."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_471_112_2_17_23_3_47_523,(uint8*)"");
}

/******************************************************************************
 * _26MXtlEnable
 ******************************************************************************/
PUBLIC void _26MXtlEnable (void)
{
    /* Enable 26M_XTL XTL */
    //CLOCK_LOG_TRACE:"[_26MXtlEnable] default is open"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_479_112_2_17_23_3_47_524,(uint8*)"");
}

/******************************************************************************
 * _26MXtlDisable
 ******************************************************************************/
LOCAL void _26MXtlDisable (void)
{
    /* Disable 26M_XTL XTL */
    //CLOCK_LOG_TRACE:"[_26MXtlEnable] can't be closed."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_487_112_2_17_23_3_47_525,(uint8*)"");
}

/******************************************************************************
 * _TimerEnable
 ******************************************************************************/
LOCAL void _TimerEnable (void)
{
    /* Enable timer apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_TimerEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_495_112_2_17_23_3_47_526,(uint8*)"");

    CHIP_REG_SET (APB_EB0_SET,  TMR_RTC_EB_SET);
    CHIP_REG_SET (APB_EB0_SET,  TMR_EB_SET);
}

/******************************************************************************
 * _TimerDisable
 ******************************************************************************/
LOCAL void _TimerDisable (void)
{
    /* Disable timer apb clock */
    //CLOCK_LOG_TRACE:"[_TimerDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_506_112_2_17_23_3_47_527,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  TMR_EB_CLR);
}

/******************************************************************************
 * _TimerReset
 ******************************************************************************/
LOCAL void _TimerReset (void)
{
    //CLOCK_LOG_TRACE:"[_TimerReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_515_112_2_17_23_3_47_528,(uint8*)"");

    CHIP_REG_SET (APB_RST0_SET,  TMR_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  TMR_SOFT_RST_CLR);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _TimerbkEnable
 ******************************************************************************/
LOCAL void _TimerbkEnable (void)
{
    /* Enable timerbk apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_TimerbkEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_529_112_2_17_23_3_47_529,(uint8*)"");

    CHIP_REG_SET (APB_EB0_SET,  TMRBK_RTC_EB_SET);
    CHIP_REG_SET (APB_EB0_SET,  TMRBK_EB_SET);
}

/******************************************************************************
 * _TimerbkDisable
 ******************************************************************************/
LOCAL void _TimerbkDisable (void)
{
    /* Disable timerbk apb clock */
    //CLOCK_LOG_TRACE:"[_TimerbkDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_540_112_2_17_23_3_47_530,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  TMRBK_EB_CLR);
}

/******************************************************************************
 * _TimerbkReset
 ******************************************************************************/
LOCAL void _TimerbkReset (void)
{
    //CLOCK_LOG_TRACE:"[_TimerbkReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_549_112_2_17_23_3_47_531,(uint8*)"");

    CHIP_REG_SET (APB_RST0_SET,  TMRBK_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  TMRBK_SOFT_RST_CLR);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _KpdEnable
 ******************************************************************************/
LOCAL void _KpdEnable (void)
{
    /* Enable keypad apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_KpdEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_563_112_2_17_23_3_47_532,(uint8*)"");

    CHIP_REG_SET (APB_EB0_SET,  KPD_RTC_EB_SET);
    CHIP_REG_SET (APB_EB0_SET,  KPD_EB_SET);
}

/******************************************************************************
 * _KpdDisable
 ******************************************************************************/
LOCAL void _KpdDisable (void)
{
    /* Disable keypad apb clock */
    //CLOCK_LOG_TRACE:"[_KpdDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_574_112_2_17_23_3_47_533,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  KPD_RTC_EB_CLR);
    CHIP_REG_SET (APB_EB0_CLR,  KPD_EB_CLR);
}

/******************************************************************************
 * _KpdReset
 ******************************************************************************/
LOCAL void _KpdReset (void)
{
    //CLOCK_LOG_TRACE:"[_KpdReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_584_112_2_17_23_3_47_534,(uint8*)"");

    /* Reset kpd clock */
    CHIP_REG_SET (APB_RST0_SET,  KPD_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  KPD_SOFT_RST_CLR);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _IIC0Enable
 ******************************************************************************/
LOCAL void _IIC0Enable (void)
{
    /* Enable keypad apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_IIC0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_599_112_2_17_23_3_47_535,(uint8*)"");

    CHIP_REG_SET (APB_EB0_SET,  I2C_EB_SET);
}

/******************************************************************************
 * _IIC0Disable
 ******************************************************************************/
LOCAL void _IIC0Disable (void)
{
    /* Enable keypad apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_IIC0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_609_112_2_17_23_3_47_536,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  I2C_EB_CLR);
}

/******************************************************************************
 * _IIC0Reset
 ******************************************************************************/
LOCAL void _IIC0Reset (void)
{
    //CLOCK_LOG_TRACE:"[_IIC0Reset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_618_112_2_17_23_3_47_537,(uint8*)"");

    /* Reset SPI1 clock */
    CHIP_REG_SET (APB_RST0_SET,  I2C_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  I2C_SOFT_RST_CLR);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _IIC0Enable
 ******************************************************************************/
LOCAL void _IIC1Enable (void)
{
    /* Enable keypad apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_IIC1Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_633_112_2_17_23_3_47_538,(uint8*)"");

    CHIP_REG_SET (APB_EB0_SET,  I2C1_EB_SET);
}

/******************************************************************************
 * _IIC0Disable
 ******************************************************************************/
LOCAL void _IIC1Disable (void)
{
    /* Enable keypad apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_IIC1Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_643_112_2_17_23_3_47_539,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  I2C1_EB_CLR);
}

/******************************************************************************
 * _IIC1Reset
 ******************************************************************************/
LOCAL void _IIC1Reset (void)
{
    //CLOCK_LOG_TRACE:"[_IIC1Reset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_652_112_2_17_23_3_47_540,(uint8*)"");

    /* Reset SPI1 clock */
    CHIP_REG_SET (APB_RST0_SET,  I2C1_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  I2C1_SOFT_RST_CLR);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _SyscntEnable
 ******************************************************************************/
LOCAL void _SyscntEnable (void)
{
    /* Enable Syscnt apb clock */
    //CLOCK_LOG_TRACE:"[_SyscntEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_667_112_2_17_23_3_47_541,(uint8*)"");

    CHIP_REG_SET (APB_EB0_SET,  SYST_RTC_EB_SET);
    CHIP_REG_SET (APB_EB0_SET,  SYST_EB_SET);
}

/******************************************************************************
 * _SyscntDisable
 ******************************************************************************/
LOCAL void _SyscntDisable (void)
{
    /* Disable Syscnt apb clock */
    //CLOCK_LOG_TRACE:"[_SyscntDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_678_112_2_17_23_3_47_542,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  SYST_RTC_EB_CLR);
    CHIP_REG_SET (APB_EB0_CLR,  SYST_EB_CLR);
}

/******************************************************************************
 * _SyscntReset
 ******************************************************************************/
LOCAL void _SyscntReset (void)
{
    //CLOCK_LOG_TRACE:"[_SyscntReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_688_112_2_17_23_3_47_543,(uint8*)"");

    /* Reset system conter clock */
    CHIP_REG_SET (APB_RST0_SET,  SYST_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  SYST_SOFT_RST_CLR);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _AdiEnable
 ******************************************************************************/
LOCAL void _AdiEnable (void)
{
    /* Enable Adi apb clock */
    //CLOCK_LOG_TRACE:"[_AdiEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_703_112_2_17_23_3_47_544,(uint8*)"");

    CHIP_REG_SET (APB_EB0_SET,  ADI_EB_SET);
}

/******************************************************************************
 * _AdiDisable
 ******************************************************************************/
LOCAL void _AdiDisable (void)
{
    /* Disable Adi apb clock */
    //CLOCK_LOG_TRACE:"[_AdiDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_713_112_2_17_23_3_47_545,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  ADI_EB_CLR);
}

/******************************************************************************
 * _AdiReset
 ******************************************************************************/
LOCAL void _AdiReset (void)
{
    //CLOCK_LOG_TRACE:"[_AdiReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_722_112_2_17_23_3_47_546,(uint8*)"");

    CHIP_REG_SET (APB_RST0_SET,  ADI_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  ADI_SOFT_RST_CLR);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _RtcEnable
 ******************************************************************************/
LOCAL void _RtcEnable (void)
{
    /* Enable rtc clock */
    //CLOCK_LOG_TRACE:"[_RtcEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_736_112_2_17_23_3_47_547,(uint8*)"");

    ANA_REG_SET (ANA_EB_SET0,  ANA_RTC_EB_SET);
    ANA_REG_SET (ANA_EB_SET1,  ANA_RTC_RTC_EB_SET);
}

/******************************************************************************
 * _RtcDisable
 ******************************************************************************/
LOCAL void _RtcDisable (void)
{
    /* Disable rtc apb clock */
    //CLOCK_LOG_TRACE:"[_RtcDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_747_112_2_17_23_3_47_548,(uint8*)"");

    ANA_REG_SET (ANA_EB_CLR0,  ANA_RTC_EB_CLR);
    ANA_REG_SET (ANA_EB_CLR1,  ANA_RTC_RTC_EB_CLR);
}

/******************************************************************************
 * _RtcReset
 ******************************************************************************/
LOCAL void _RtcReset (void)
{
    //CLOCK_LOG_TRACE:"[_RtcReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_757_112_2_17_23_3_47_549,(uint8*)"");

    ANA_REG_SET (ANA_SOFT_RST_SET0,  ANA_RTC_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    ANA_REG_SET (ANA_SOFT_RST_CLR0,  ANA_RTC_SOFT_RST_CLR);
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

    ANA_REG_SET (ANA_EB_SET0,  ANA_WDG_EB_SET);
    ANA_REG_SET (ANA_EB_SET1,  ANA_RTC_WDG_EB_SET);
}

/******************************************************************************
 * _WdgDisable
 ******************************************************************************/
LOCAL void _WdgDisable (void)
{
    /* Disable watchdog apb clock */
    //CLOCK_LOG_TRACE:"[_WdgDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_782_112_2_17_23_3_47_551,(uint8*)"");

    ANA_REG_SET (ANA_EB_CLR0,  ANA_WDG_EB_CLR);
    ANA_REG_SET (ANA_EB_CLR1,  ANA_RTC_WDG_EB_CLR);
}

/******************************************************************************
 * _WdgReset
 ******************************************************************************/
LOCAL void _WdgReset (void)
{
    //CLOCK_LOG_TRACE:"[_WdgReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_792_112_2_17_23_3_47_552,(uint8*)"");

    ANA_REG_SET (ANA_SOFT_RST_SET0,  ANA_WDG_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    ANA_REG_SET (ANA_SOFT_RST_CLR0,  ANA_WDG_SOFT_RST_CLR);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _ChgrwdgEnable
 ******************************************************************************/
LOCAL void _ChgrwdgEnable (void)
{
    /* Enable charger watchdog clock */
    //CLOCK_LOG_TRACE:"[_ChgrwdgEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_806_112_2_17_23_3_47_553,(uint8*)"");

    ANA_REG_SET (ANA_EB_SET0,  ANA_CHGRWDG_EB_SET);
    ANA_REG_SET (ANA_EB_SET1,  ANA_RTC_CHGRWDG_EB_SET);
}

/******************************************************************************
 * _ChgrwdgDisable
 ******************************************************************************/
LOCAL void _ChgrwdgDisable (void)
{
    /* Disable charger watchdog apb clock */
    //CLOCK_LOG_TRACE:"[_ChgrwdgDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_817_112_2_17_23_3_47_554,(uint8*)"");

    ANA_REG_SET (ANA_EB_CLR0,  ANA_CHGRWDG_EB_CLR);
    ANA_REG_SET (ANA_EB_CLR1,  ANA_RTC_CHGRWDG_EB_CLR);
}

/******************************************************************************
 * _ChgwdgReset
 ******************************************************************************/
LOCAL void _ChgrwdgReset (void)
{
    //CLOCK_LOG_TRACE:"[_ChgrwdgReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_827_112_2_17_23_3_47_555,(uint8*)"");

    ANA_REG_SET (ANA_SOFT_RST_SET0,  ANA_CHGRWDG_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    ANA_REG_SET (ANA_SOFT_RST_CLR0,  ANA_CHGRWDG_SOFT_RST_CLR);
    DELAY_CYCLE(1000)
}


/******************************************************************************
 * _TpcEnable
 ******************************************************************************/
LOCAL void _TpcEnable (void)
{
    /* Enable Tpc apb clock */
    //CLOCK_LOG_TRACE:"[_TpcEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_842_112_2_17_23_3_47_556,(uint8*)"");

    ANA_REG_SET (ANA_EB_SET0,  ANA_TPC_EB_SET);
    ANA_REG_SET (ANA_EB_SET1,  ANA_RTC_TPC_EB_SET);
}

/******************************************************************************
 * _TpcDisable
 ******************************************************************************/
LOCAL void _TpcDisable (void)
{
    /* Disable Tpc apb clock */
    //CLOCK_LOG_TRACE:"[_TpcDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_853_112_2_17_23_3_47_557,(uint8*)"");

    ANA_REG_SET (ANA_EB_CLR0,  ANA_TPC_EB_CLR);
    ANA_REG_SET (ANA_EB_CLR1,  ANA_RTC_TPC_EB_CLR);
}

/******************************************************************************
 * _TpcReset
 ******************************************************************************/
LOCAL void _TpcReset (void)
{
    //CLOCK_LOG_TRACE:"[_TpcReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_863_112_2_17_23_3_47_558,(uint8*)"");

    ANA_REG_SET (ANA_SOFT_RST_SET0,  ANA_TPC_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    ANA_REG_SET (ANA_SOFT_RST_CLR0,  ANA_TPC_SOFT_RST_CLR);
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

    ANA_REG_SET (ANA_EB_SET0,  ANA_ADC_EB_SET);	

    ANA_REG_SET (ANA_CLK_SET0,  CLK_ADC_EN_SET);		
    ANA_REG_SET (ANA_CLK_SET0,  CLK_AUXADC_EN_SET);	
}

/******************************************************************************
 * _AdcDisable
 ******************************************************************************/
LOCAL void _AdcDisable (void)
{
    /* Disable Adc apb clock */
    //CLOCK_LOG_TRACE:"[_AdcDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_890_112_2_17_23_3_47_560,(uint8*)"");

    ANA_REG_SET (ANA_EB_CLR0,  ANA_ADC_EB_CLR);

    ANA_REG_SET (ANA_CLK_CLR0,  CLK_ADC_EN_CLR);		
    ANA_REG_SET (ANA_CLK_CLR0,  CLK_AUXADC_EN_CLR);		
}


/******************************************************************************
 * _AdcReset
 ******************************************************************************/
LOCAL void _AdcReset (void)
{
    //CLOCK_LOG_TRACE:"[_AdcReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_903_112_2_17_23_3_47_561,(uint8*)"");

    ANA_REG_SET (ANA_SOFT_RST_SET0,  ANA_ADC_SOFT_RST_SET);
    DELAY_CYCLE(10)

    ANA_REG_SET (ANA_SOFT_RST_CLR0,  ANA_ADC_SOFT_RST_CLR);
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
    CHIP_REG_SET (APB_EB0_SET,  GPIO_RTC_EB_SET);
    CHIP_REG_SET (APB_EB0_SET,  GPIO_EB_SET);
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
    CHIP_REG_SET (APB_EB0_CLR,  GPIO_RTC_EB_CLR);
    CHIP_REG_SET (APB_EB0_CLR,  GPIO_EB_CLR);
}

/******************************************************************************
 * _GpiodReset
 ******************************************************************************/
LOCAL void _GpiodReset (void)
{
    //CLOCK_LOG_TRACE:"[_GpioReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_940_112_2_17_23_3_47_564,(uint8*)"");

    /* Reset gpio in D-die */
    CHIP_REG_SET (APB_RST0_SET,  GPIO_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  GPIO_SOFT_RST_CLR);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _GpioaEnable
 ******************************************************************************/
LOCAL void _GpioaEnable (void)
{
	/* Enable Gpio adi clock */
    //CLOCK_LOG_TRACE:"[_GpioaEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_955_112_2_17_23_3_47_565,(uint8*)"");

    // A-die
    ANA_REG_SET (ANA_EB_SET0,  ANA_GPIO_EB_SET);	
}

/******************************************************************************
 * _GpiodDisable
 ******************************************************************************/
LOCAL void _GpioaDisable (void)
{
    /* Disable Gpio apb clock */
    //CLOCK_LOG_TRACE:"[_GpioDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_966_112_2_17_23_3_47_566,(uint8*)"");

    // A-die
    ANA_REG_SET (ANA_EB_CLR0,  ANA_GPIO_EB_CLR);		
}

/******************************************************************************
 * _GpioaReset
 ******************************************************************************/
LOCAL void _GpioaReset (void)
{
    //CLOCK_LOG_TRACE:"[_GpioaReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_976_112_2_17_23_3_48_567,(uint8*)"");

    /* Reset gpio in A-die */
    ANA_REG_SET (ANA_SOFT_RST_SET0,  ANA_GPIO_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    ANA_REG_SET (ANA_SOFT_RST_CLR0,  ANA_GPIO_SOFT_RST_CLR);
    DELAY_CYCLE(1000)
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
    CHIP_REG_SET (APB_EB0_SET,  EIC_RTC_EB_SET);
    CHIP_REG_SET (APB_EB0_SET,  EIC_EB_SET);
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
    CHIP_REG_SET (APB_EB0_CLR,  EIC_RTC_EB_CLR);
    CHIP_REG_SET (APB_EB0_CLR,  EIC_EB_CLR);	
}

/******************************************************************************
 * _EicdReset
 ******************************************************************************/
LOCAL void _EicdReset (void)
{
    //CLOCK_LOG_TRACE:"[_EicdReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1014_112_2_17_23_3_48_570,(uint8*)"");

    /* Reset gpio in D-die */
    CHIP_REG_SET (APB_RST0_SET,  EIC_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  EIC_SOFT_RST_CLR);
    DELAY_CYCLE(1000)
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
    ANA_REG_SET (ANA_EB_SET1,  ANA_RTC_EIC_EB_SET);	
    ANA_REG_SET (ANA_EB_SET0,  ANA_EIC_EB_SET);	
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
    ANA_REG_SET (ANA_EB_CLR1,  ANA_RTC_EIC_EB_CLR);	    
    ANA_REG_SET (ANA_EB_CLR0,  ANA_EIC_EB_CLR);		
}

/******************************************************************************
 * _EicaReset
 ******************************************************************************/
LOCAL void _EicaReset (void)
{
    //CLOCK_LOG_TRACE:"[_EicaReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1052_112_2_17_23_3_48_573,(uint8*)"");

    /* Reset gpio in A-die */
    ANA_REG_SET (ANA_SOFT_RST_SET0,  ANA_EIC_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    ANA_REG_SET (ANA_SOFT_RST_CLR0,  ANA_EIC_SOFT_RST_CLR);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _Sim0Enable
 ******************************************************************************/
LOCAL void _Sim0Enable (void)
{
    /* Enable Sim0 apb clock */
    //CLOCK_LOG_TRACE:"[_Sim0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1067_112_2_17_23_3_48_574,(uint8*)"");

    CHIP_REG_SET (APB_EB0_SET,  SIM_EB_SET);
}

/******************************************************************************
 * _Sim0Disable
 ******************************************************************************/
LOCAL void _Sim0Disable (void)
{
    /* Disable Sim0 apb clock */
    //CLOCK_LOG_TRACE:"[_Sim0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1077_112_2_17_23_3_48_575,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  SIM_EB_CLR);
}

/******************************************************************************
 * _Sim0Reset
 ******************************************************************************/
LOCAL void _Sim0Reset (void)
{
    //CLOCK_LOG_TRACE:"[_Sim0Reset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1086_112_2_17_23_3_48_576,(uint8*)"");

    /* Reset sim clock */
    CHIP_REG_SET (APB_RST0_SET,  SIM_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  SIM_SOFT_RST_CLR);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _GeaEnable
 ******************************************************************************/
LOCAL void _GeaEnable (void)
{
    /* Enable Gea apb clock */
    //CLOCK_LOG_TRACE:"[_GeaEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1101_112_2_17_23_3_48_577,(uint8*)"");

    CHIP_REG_SET (APB_EB0_SET,  GEA_EB_SET);
}

/******************************************************************************
 * _GeaDisable
 ******************************************************************************/
LOCAL void _GeaDisable (void)
{
    /* Disable Gea apb clock */
    //CLOCK_LOG_TRACE:"[_GeaDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1111_112_2_17_23_3_48_578,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  GEA_EB_CLR);
}

/******************************************************************************
 * _GeaReset
 ******************************************************************************/
LOCAL void _GeaReset (void)
{
    //CLOCK_LOG_TRACE:"[_GeaReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1120_112_2_17_23_3_48_579,(uint8*)"");

    /* Reset GEA clock */
    CHIP_REG_SET (APB_RST0_SET,  GEA_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  GEA_SOFT_RST_CLR);
    DELAY_CYCLE(1000)
}	

/******************************************************************************
 * _EfuseEnable
 ******************************************************************************/
LOCAL void _EfuseEnable (void)
{
    /* Enable Efuse apb clock */
    //CLOCK_LOG_TRACE:"[_EfuseEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1135_112_2_17_23_3_48_580,(uint8*)"");

    CHIP_REG_SET (APB_EB0_SET,  EFUSE_EB_SET);
}

/******************************************************************************
 * _EfuseDisable
 ******************************************************************************/
LOCAL void _EfuseDisable (void)
{
    /* Disable Efuse apb clock */
    //CLOCK_LOG_TRACE:"[_EfuseDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1145_112_2_17_23_3_48_581,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  EFUSE_EB_CLR);
}

/******************************************************************************
 * _Spi0Enable
 ******************************************************************************/
LOCAL void _Spi0Enable (void)
{
    /* Enable Spi0 apb clock */
    //CLOCK_LOG_TRACE:"[_Spi0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1155_112_2_17_23_3_48_582,(uint8*)"");

    //CHIP_REG_OR (APB_EB0_SET,  SPI0_GLB_EB_SET);
    CHIP_REG_SET(AHB_EB0_SET, SPI0_EB_SET);
}

/******************************************************************************
 * _Spi0Disable
 ******************************************************************************/
LOCAL void _Spi0Disable (void)
{
    /* Disable Spi0 apb clock */
    //CLOCK_LOG_TRACE:"[_SpiDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1166_112_2_17_23_3_48_583,(uint8*)"");

    CHIP_REG_SET(AHB_EB0_CLR, SPI0_EB_CLR);
}
/******************************************************************************
 * _Spi1Enable
 ******************************************************************************/
LOCAL void _Spi1Enable (void)
{
    /* Enable Spi1 apb clock */
    //CLOCK_LOG_TRACE:"[_Spi1Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1175_112_2_17_23_3_48_584,(uint8*)"");

    CHIP_REG_SET (APB_EB0_SET,  SPI1_EB_SET);
}

/******************************************************************************
 * _Spi1Disable
 ******************************************************************************/
LOCAL void _Spi1Disable (void)
{
    /* Disable Spi1 apb clock */
    //CLOCK_LOG_TRACE:"[_Spi1Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1185_112_2_17_23_3_48_585,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  SPI1_EB_CLR);
}

/******************************************************************************
 * _Spi1Reset
 ******************************************************************************/
LOCAL void _Spi1Reset (void)
{
    //CLOCK_LOG_TRACE:"[_Spi1Reset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1194_112_2_17_23_3_48_586,(uint8*)"");

    /* Reset SPI1 clock */
    CHIP_REG_SET (APB_RST0_SET,  SPI1_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  SPI1_SOFT_RST_CLR);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _I2s0Enable
 ******************************************************************************/
LOCAL void _I2s0Enable (void)
{
	/* Enable I2s0 apb clock */
    //CLOCK_LOG_TRACE:"[_I2s0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1209_112_2_17_23_3_48_587,(uint8*)"");
	
    CHIP_REG_SET (APB_EB0_SET,  IIS_EB_SET);
}	

/******************************************************************************
 * _I2s0Disable
 ******************************************************************************/
LOCAL void _I2s0Disable (void)
{
    /* Disable I2s0 apb clock */
    //CLOCK_LOG_TRACE:"[_I2s0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1219_112_2_17_23_3_48_588,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  IIS_EB_CLR);
}

/******************************************************************************
 * _I2sReset
 ******************************************************************************/
LOCAL void _I2sReset (void)
{
    //CLOCK_LOG_TRACE:"[_I2sReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1228_112_2_17_23_3_48_589,(uint8*)"");

    /* Reset I2s clock */
    CHIP_REG_SET (APB_RST0_SET,  IIS_SOFT_RST_SET_MCU);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  IIS_SOFT_RST_CLR_MCU);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _Uart0Enable
 ******************************************************************************/
LOCAL void _Uart0Enable (void)
{
    /* Enable Uart0 apb clock */
    //CLOCK_LOG_TRACE:"[_Uart0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1243_112_2_17_23_3_48_590,(uint8*)"");

    CHIP_REG_SET (APB_EB0_SET,  UART0_EB_SET);
}

/******************************************************************************
 * _Uart0Disable
 ******************************************************************************/
LOCAL void _Uart0Disable (void)
{
    /* Disable Uart0 apb clock */
    //CLOCK_LOG_TRACE:"[_Uart0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1253_112_2_17_23_3_48_591,(uint8*)"");
	
    CHIP_REG_SET (APB_EB0_CLR,  UART0_EB_CLR);
}

/******************************************************************************
 * _Uart0Reset
 ******************************************************************************/
LOCAL void _Uart0Reset (void)
{
    //CLOCK_LOG_TRACE:"[_Uart0Reset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1262_112_2_17_23_3_48_592,(uint8*)"");

    /* Reset uart0 clock */
    CHIP_REG_SET (APB_RST0_SET,  UART0_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  UART0_SOFT_RST_CLR);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _Uart1Enable
 ******************************************************************************/
LOCAL void _Uart1Enable (void)
{
    /* Enable Uart0 apb clock */
    //CLOCK_LOG_TRACE:"[_Uart1Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1277_112_2_17_23_3_48_593,(uint8*)"");

    CHIP_REG_SET (APB_EB0_SET,  UART1_EB_SET);
}

/******************************************************************************
 * _Uart1Disable
 ******************************************************************************/
LOCAL void _Uart1Disable (void)
{
    /* Disable Uart1 apb clock */
    //CLOCK_LOG_TRACE:"[_Uart1Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1287_112_2_17_23_3_48_594,(uint8*)"");
	
    CHIP_REG_SET (APB_EB0_CLR,  UART1_EB_CLR);
}

/******************************************************************************
 * _Uart1Reset
 ******************************************************************************/
LOCAL void _Uart1Reset (void)
{
    //CLOCK_LOG_TRACE:"[_Uart1Reset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1296_112_2_17_23_3_48_595,(uint8*)"");

    /* Reset uart1 clock */
    CHIP_REG_SET (APB_RST0_SET,  UART1_SOFT_RST_SET_MCU);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  UART1_SOFT_RST_CLR_MCU);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _PwmEnable
 ******************************************************************************/
LOCAL void _PwmEnable (void)
{
    /* Enable Pwm apb clock */
    //CLOCK_LOG_TRACE:"[_PwmEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1311_112_2_17_23_3_48_596,(uint8*)"");
	
    CHIP_REG_SET (APB_EB0_SET,  PWM_EB_SET);
}

/******************************************************************************
 * _PwmDisable
 ******************************************************************************/
LOCAL void _PwmDisable (void)
{
    /* Disable Pwm apb clock */
    //CLOCK_LOG_TRACE:"[_PwmDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1321_112_2_17_23_3_48_597,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  PWM_EB_CLR);
}

/******************************************************************************
 * _PinEnable
 ******************************************************************************/
LOCAL void _PinEnable (void)
{
    /* Enable Pin apb clock */
    //CLOCK_LOG_TRACE:"[_PinEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1331_112_2_17_23_3_48_598,(uint8*)"");

    CHIP_REG_SET ( (APB_EB0_SET), (PIN_REG_EB));
    ANA_REG_SET(ANA_EB_SET0,ANA_PINREG_EB_SET);
}

/******************************************************************************
 * _PinDisable
 ******************************************************************************/
LOCAL void _PinDisable (void)
{
    /* Disable Pin apb clock */
    //CLOCK_LOG_TRACE:"[_PinDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1342_112_2_17_23_3_48_599,(uint8*)"");

    CHIP_REG_SET ( (APB_EB0_CLR), (PIN_REG_EB_CLR));
    ANA_REG_SET(ANA_EB_CLR0,ANA_PINREG_EB_CLR);
}

/******************************************************************************
 * _PwmReset
 ******************************************************************************/
LOCAL void _PwmReset (void)
{
    //CLOCK_LOG_TRACE:"[_PwmReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1352_112_2_17_23_3_48_600,(uint8*)"");

    /* Reset GEA clock */
    CHIP_REG_SET (APB_RST0_SET,  PWM_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  PWM_SOFT_RST_CLR);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _Aux0Enable
 ******************************************************************************/
LOCAL void _Aux0Enable (void)
{
    /* Enable Aux0 apb clock */
    //CLOCK_LOG_TRACE:"[_Aux0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1367_112_2_17_23_3_48_601,(uint8*)"");

    CHIP_REG_SET (APB_EB0_SET,  CLK_AUX_EB_SET);	
}

/******************************************************************************
 * _Aux0Disable
 ******************************************************************************/
LOCAL void _Aux0Disable (void)
{
    /* Disable Aux0 apb clock */
    //CLOCK_LOG_TRACE:"[_Aux0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1377_112_2_17_23_3_48_602,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  CLK_AUX_EB_CLR);	
}

/******************************************************************************
 * _CcirReset
 ******************************************************************************/
LOCAL void _CcirReset (void)
{
    //CLOCK_LOG_TRACE:"[_CcirReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1386_112_2_17_23_3_48_603,(uint8*)"");

    /* Reset ccir clock */
    CHIP_REG_SET (AHB_RST0_SET,  CCIR_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (AHB_RST0_CLR,  CCIR_SOFT_RST_CLR);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _DcamEnable
 ******************************************************************************/
LOCAL void _DcamEnable (void)
{
    /* Enable Dcam ahb clock */
    //CLOCK_LOG_TRACE:"[_DcamEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1401_112_2_17_23_3_48_604,(uint8*)"");

    CHIP_REG_SET (AHB_EB0_SET,  DCAM_EB_SET);
}

/******************************************************************************
 * _DcamDisable
 ******************************************************************************/
LOCAL void _DcamDisable (void)
{
    /* Disable Dcam apb clock */
    //CLOCK_LOG_TRACE:"[_DcamDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1411_112_2_17_23_3_48_605,(uint8*)"");

    CHIP_REG_SET (AHB_EB0_CLR,  DCAM_EB_CLR);
}

/******************************************************************************
 * _DcamReset
 ******************************************************************************/
LOCAL void _DcamReset (void)
{
    //CLOCK_LOG_TRACE:"[_DcamReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1420_112_2_17_23_3_48_606,(uint8*)"");

    /* Reset DCAM device */
    CHIP_REG_SET (AHB_RST0_SET,  DCAM_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (AHB_RST0_CLR,  DCAM_SOFT_RST_CLR);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _LzmaEnable
 ******************************************************************************/
LOCAL void _LzmaEnable (void)
{
    /* Enable Sdio apb clock */
    //CLOCK_LOG_TRACE:"[_LzmaEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1435_112_2_17_23_3_48_607,(uint8*)"");
	
    CHIP_REG_SET(AHB_EB0_SET, ZIP_EB_SET);
}

/******************************************************************************
 * _LzmaDisable
 ******************************************************************************/
LOCAL void _LzmaDisable (void)
{
    /* Disable Sdio apb clock */
    //CLOCK_LOG_TRACE:"[_LzmaDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1445_112_2_17_23_3_48_608,(uint8*)"");
      
    CHIP_REG_SET (AHB_EB0_CLR,  ZIP_EB_CLR);
}

/******************************************************************************
 * _LzmaReset
 ******************************************************************************/
LOCAL void _LzmaReset (void)
{
    //CLOCK_LOG_TRACE:"[_LzmaReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1454_112_2_17_23_3_48_609,(uint8*)"");
   
    CHIP_REG_SET(AHB_RST0_SET, ZIP_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET(AHB_RST0_CLR, ZIP_SOFT_RST_CLR);
    DELAY_CYCLE(1000)   
}

/******************************************************************************
 * _LcmEnable
 ******************************************************************************/
LOCAL void _LcmEnable (void)
{
    /* Enable lcm ahb clock */
    //CLOCK_LOG_TRACE:"[_LcmEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1468_112_2_17_23_3_48_610,(uint8*)"");
	
    CHIP_REG_SET(AHB_EB0_SET, LCM_EB_SET);
}

/******************************************************************************
 * _LcmDisable
 ******************************************************************************/
LOCAL void _LcmDisable (void)
{
    /* Disable lcm ahb clock */
    //CLOCK_LOG_TRACE:"[_LcmDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1478_112_2_17_23_3_48_611,(uint8*)"");
      
    CHIP_REG_SET (AHB_EB0_CLR,  LCM_EB_CLR);
}

/******************************************************************************
 * _LcmReset
 ******************************************************************************/
LOCAL void _LcmReset (void)
{
    //CLOCK_LOG_TRACE:"[_LcmReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1487_112_2_17_23_3_48_612,(uint8*)"");
   
    CHIP_REG_SET(AHB_RST0_SET, LCM_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET(AHB_RST0_CLR, LCM_SOFT_RST_CLR);
    DELAY_CYCLE(1000)   
}

/******************************************************************************
 * _LcdcEnable
 ******************************************************************************/
LOCAL void _LcdcEnable (void)
{
    /* Enable lcdc ahb clock */
    //CLOCK_LOG_TRACE:"[_LcdcEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1501_112_2_17_23_3_49_613,(uint8*)"");
	
    CHIP_REG_SET(AHB_EB0_SET, LCDC_EB_SET);
}

/******************************************************************************
 * _LcdcDisable
 ******************************************************************************/
LOCAL void _LcdcDisable (void)
{
    /* Disable lcdc ahb clock */
    //CLOCK_LOG_TRACE:"[_LcdcDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1511_112_2_17_23_3_49_614,(uint8*)"");
      
    CHIP_REG_SET (AHB_EB0_CLR,  LCDC_EB_CLR);
}

/******************************************************************************
 * _LcdcReset
 ******************************************************************************/
LOCAL void _LcdcReset (void)
{
    //CLOCK_LOG_TRACE:"[_LcdcReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1520_112_2_17_23_3_49_615,(uint8*)"");
   
    CHIP_REG_SET(AHB_RST0_SET, LCDC_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET(AHB_RST0_CLR, LCDC_SOFT_RST_CLR);
    DELAY_CYCLE(1000)   
}

/******************************************************************************
 * _RotaEnable
 ******************************************************************************/
LOCAL void _RotaEnable (void)
{
    /* Enable rota ahb clock */
    //CLOCK_LOG_TRACE:"[_RotaEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1534_112_2_17_23_3_49_616,(uint8*)"");
	
    CHIP_REG_SET(AHB_EB0_SET, ROTA_EB_SET);
}

/******************************************************************************
 * _RotaDisable
 ******************************************************************************/
LOCAL void _RotaDisable (void)
{
    /* Disable rota ahb clock */
    //CLOCK_LOG_TRACE:"[_RotaDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1544_112_2_17_23_3_49_617,(uint8*)"");
      
    CHIP_REG_SET (AHB_EB0_CLR,  ROTA_EB_CLR);
}

/******************************************************************************
 * _RotaReset
 ******************************************************************************/
LOCAL void _RotaReset (void)
{
    //CLOCK_LOG_TRACE:"[_RotaReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1553_112_2_17_23_3_49_618,(uint8*)"");
   
    CHIP_REG_SET(AHB_RST0_SET, ROTA_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET(AHB_RST0_CLR, ROTA_SOFT_RST_CLR);
    DELAY_CYCLE(1000)   
}

/******************************************************************************
 * _MidiEnable
 ******************************************************************************/
LOCAL void _MidiEnable (void)
{
    /* Enable Midi ahb clock */
    //CLOCK_LOG_TRACE:"[_MidiEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1567_112_2_17_23_3_49_619,(uint8*)"");
	
    CHIP_REG_SET(AHB_EB0_SET, MIDI_EB_SET);
}

/******************************************************************************
 * _MidiDisable
 ******************************************************************************/
LOCAL void _MidiDisable (void)
{
    /* Disable MIDI ahb clock */
    //CLOCK_LOG_TRACE:"[_MidiDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1577_112_2_17_23_3_49_620,(uint8*)"");
      
    CHIP_REG_SET (AHB_EB0_CLR,  MIDI_EB_CLR);
}

/******************************************************************************
 * _MidiReset
 ******************************************************************************/
LOCAL void _MidiReset (void)
{
    //CLOCK_LOG_TRACE:"[_MidiReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1586_112_2_17_23_3_49_621,(uint8*)"");
   
    CHIP_REG_SET(AHB_RST0_SET, MIDI_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET(AHB_RST0_CLR, MIDI_SOFT_RST_CLR);
    DELAY_CYCLE(1000)   
}

/******************************************************************************
 * _TicEnable
 ******************************************************************************/
LOCAL void _TicEnable (void)
{
    /* Enable tic ahb clock */
    //CLOCK_LOG_TRACE:"[_TicEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1600_112_2_17_23_3_49_622,(uint8*)"");
	
    CHIP_REG_SET(AHB_EB0_SET, TIC_EB_SET);
}

/******************************************************************************
 * _TicDisable
 ******************************************************************************/
LOCAL void _TicDisable (void)
{
    /* Disable tic ahb clock */
    //CLOCK_LOG_TRACE:"[_TicDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1610_112_2_17_23_3_49_623,(uint8*)"");
      
    CHIP_REG_SET (AHB_EB0_CLR,  TIC_EB_CLR);
}

/******************************************************************************
 * _SfcEnable
 ******************************************************************************/
LOCAL void _SfcEnable (void)
{
    /* Enable sfc ahb clock */
    //CLOCK_LOG_TRACE:"[_SfcEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1620_112_2_17_23_3_49_624,(uint8*)"");
	
    CHIP_REG_SET(AHB_EB0_SET, SFC_EB_SET);
}

/******************************************************************************
 * _SfcDisable
 ******************************************************************************/
LOCAL void _SfcDisable (void)
{
    /* Disable sfc ahb clock */
    //CLOCK_LOG_TRACE:"[_SfcDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1630_112_2_17_23_3_49_625,(uint8*)"");
      
    CHIP_REG_SET (AHB_EB0_CLR,  SFC_EB_CLR);
}

/******************************************************************************
 * _SfcReset
 ******************************************************************************/
LOCAL void _SfcReset (void)
{
    //CLOCK_LOG_TRACE:"[_SfcReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1639_112_2_17_23_3_49_626,(uint8*)"");
   
    CHIP_REG_SET(AHB_RST0_SET, SFC_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET(AHB_RST0_CLR, SFC_SOFT_RST_CLR);
    DELAY_CYCLE(1000)   
}

/******************************************************************************
 * _VspReset
 ******************************************************************************/
LOCAL void _VspReset (void)
{
    //CLOCK_LOG_TRACE:"[_VspReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1652_112_2_17_23_3_49_627,(uint8*)"");
   
    CHIP_REG_SET(AHB_RST0_SET, VSP_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET(AHB_RST0_CLR, VSP_SOFT_RST_CLR);
    DELAY_CYCLE(1000)   
}

/******************************************************************************
 * _Sdio0Enable
 ******************************************************************************/
LOCAL void _Sdio0Enable (void)
{
    /* Enable Sdio apb clock */
    //CLOCK_LOG_TRACE:"[_SdioEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1666_112_2_17_23_3_49_628,(uint8*)"");
	
    CHIP_REG_SET(AHB_EB0_SET, SDIO_EB_SET);
}

/******************************************************************************
 * _Sdio0Disable
 ******************************************************************************/
LOCAL void _Sdio0Disable (void)
{
    /* Disable Sdio apb clock */
    //CLOCK_LOG_TRACE:"[_SdioDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1676_112_2_17_23_3_49_629,(uint8*)"");
      
    CHIP_REG_SET (AHB_EB0_CLR,  SDIO_EB_CLR);
}

/******************************************************************************
 * _SdioReset
 ******************************************************************************/
LOCAL void _SdioReset (void)
{
    //CLOCK_LOG_TRACE:"[_SdioReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1685_112_2_17_23_3_49_630,(uint8*)"");
   
    CHIP_REG_SET(AHB_RST0_SET, SDIO_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET(AHB_RST0_CLR, SDIO_SOFT_RST_CLR);
    DELAY_CYCLE(1000)   
}

/******************************************************************************
 * _UsbEnable
 ******************************************************************************/
LOCAL void _UsbEnable (void)
{
	/* Enable Usb apb clock */
    //CLOCK_LOG_TRACE:"[_UsbEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1699_112_2_17_23_3_49_631,(uint8*)"");

    CHIP_REG_SET(AHB_EB0_SET, USBD_EB_SET);
}

/******************************************************************************
 * _UsbDisable
 ******************************************************************************/
LOCAL void _UsbDisable (void)
{
    /* Disable Usb apb clock */
    //CLOCK_LOG_TRACE:"[_UsbDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1709_112_2_17_23_3_49_632,(uint8*)"");

    CHIP_REG_SET(AHB_EB0_CLR, USBD_EB_CLR);
}

/******************************************************************************
 * _UsbReset
 ******************************************************************************/
LOCAL void _UsbReset (void)
{
    //CLOCK_LOG_TRACE:"[_UsbReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1718_112_2_17_23_3_49_633,(uint8*)"");

    CHIP_REG_SET(AHB_RST0_SET, USBD_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET(AHB_RST0_CLR, USBD_SOFT_RST_CLR);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _DmaEnable
 ******************************************************************************/
LOCAL void _DmaEnable (void)
{
    /* Enable Dma apb clock */
    //CLOCK_LOG_TRACE:"[_DmaEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1732_112_2_17_23_3_49_634,(uint8*)"");

    CHIP_REG_SET (AHB_EB0_SET,  DMA_EB_SET);
}

/******************************************************************************
 * _DmaDisable
 ******************************************************************************/
LOCAL void _DmaDisable (void)
{
    /* Disable Dma apb clock */
    //CLOCK_LOG_TRACE:"[_DmaDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1742_112_2_17_23_3_49_635,(uint8*)"");

    CHIP_REG_SET (AHB_EB0_CLR,  DMA_EB_CLR);
}

/******************************************************************************
 * _DmaReset
 ******************************************************************************/
LOCAL void _DmaReset (void)
{
    //CLOCK_LOG_TRACE:"[_DmaReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1751_112_2_17_23_3_49_636,(uint8*)"");

    CHIP_REG_SET(AHB_RST0_SET, DMA_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET(AHB_RST0_CLR, DMA_SOFT_RST_CLR);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _Bm0Enable
 ******************************************************************************/
LOCAL void _Bm0Enable (void)
{
	/* Enable Bm0 apb clock */
    //CLOCK_LOG_TRACE:"[_Bm0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1765_112_2_17_23_3_49_637,(uint8*)"");

    CHIP_REG_SET (AHB_EB0_SET,  BUSMON0_EB_SET);
}

/******************************************************************************
 * _Bm0Disable
 ******************************************************************************/
LOCAL void _Bm0Disable (void)
{
    /* Disable Bm0 apb clock */
    //CLOCK_LOG_TRACE:"[_Bm0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1775_112_2_17_23_3_49_638,(uint8*)"");

    CHIP_REG_SET (AHB_EB0_CLR,  BUSMON0_EB_CLR);
}

/******************************************************************************
 * _Bm1Enable
 ******************************************************************************/
LOCAL void _Bm1Enable (void)
{
    /* Enable Bm1 apb clock */
    //CLOCK_LOG_TRACE:"[_Bm1Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1785_112_2_17_23_3_49_639,(uint8*)"");

    CHIP_REG_SET (AHB_EB0_SET,  BUSMON1_EB_SET);
}

/******************************************************************************
 * _Bm1Disable
 ******************************************************************************/
LOCAL void _Bm1Disable (void)
{
    /* Disable Bm1 apb clock */
    //CLOCK_LOG_TRACE:"[_Bm1Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1795_112_2_17_23_3_49_640,(uint8*)"");

    CHIP_REG_SET (AHB_EB0_CLR,  BUSMON1_EB_CLR);
}

/******************************************************************************
 * _Bm2Enable
 ******************************************************************************/
LOCAL void _Bm2Enable (void)
{
    /* Enable Bm2 apb clock */
    //CLOCK_LOG_TRACE:"[_Bm2Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1805_112_2_17_23_3_49_641,(uint8*)"");

    CHIP_REG_SET (AHB_EB0_SET,  BUSMON2_EB_SET);
}

/******************************************************************************
 * _Bm2Disable
 ******************************************************************************/
LOCAL void _Bm2Disable (void)
{
    /* Disable Bm2 apb clock */
    //CLOCK_LOG_TRACE:"[_Bm2Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1815_112_2_17_23_3_49_642,(uint8*)"");

    CHIP_REG_SET (AHB_EB0_CLR,  BUSMON2_EB_CLR);
}

/******************************************************************************
 * _AhbEnable
 ******************************************************************************/
LOCAL void _AhbEnable (void)
{
    //CLOCK_LOG_TRACE:"[_AhbEnable] is NULL"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1824_112_2_17_23_3_49_643,(uint8*)"");

    return;
}

/******************************************************************************
 * _AhbDisable
 ******************************************************************************/
LOCAL void _AhbDisable (void)
{
    //CLOCK_LOG_TRACE:"[_AhbDisable] is NULL"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1833_112_2_17_23_3_49_644,(uint8*)"");

    return;
}

/******************************************************************************
 * _ApbEnable
 ******************************************************************************/
LOCAL void _ApbEnable (void)
{
    //CLOCK_LOG_TRACE:"[_ApbEnable] is NULL"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1842_112_2_17_23_3_49_645,(uint8*)"");

    return;
}

/******************************************************************************
 * _ApbDisable
 ******************************************************************************/
LOCAL void _ApbDisable (void)
{
    //CLOCK_LOG_TRACE:"[_ApbDisable] is NULL"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1851_112_2_17_23_3_49_646,(uint8*)"");

    return;
}

/******************************************************************************
 * _EmcEnable
 ******************************************************************************/
LOCAL void _EmcEnable (void)
{
    //CLOCK_LOG_TRACE:"[_EmcEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1860_112_2_17_23_3_49_647,(uint8*)"");

    CHIP_REG_SET (AHB_EB0_SET,  EMC_EB_SET);	
    return;
}

/******************************************************************************
 * _EmcDisable
 ******************************************************************************/
LOCAL void _EmcDisable (void)
{
    //CLOCK_LOG_TRACE:"[_AhbDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1870_112_2_17_23_3_49_648,(uint8*)"");

    CHIP_REG_SET (AHB_EB0_CLR,  EMC_EB_CLR);	
    return;
}

/******************************************************************************
 * _EmcReset
 ******************************************************************************/
LOCAL void _EmcReset (void)
{
    //CLOCK_LOG_TRACE:"[_EmcReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1880_112_2_17_23_3_49_649,(uint8*)"");

    CHIP_REG_SET(AHB_RST0_SET, EMC_SOFT_RST_SET);
    DELAY_CYCLE(1000)

    CHIP_REG_SET(AHB_RST0_CLR, EMC_SOFT_RST_CLR);
    DELAY_CYCLE(1000)	

    return;
}

/******************************************************************************
 * _AudioEnable
 ******************************************************************************/
LOCAL void _AudioEnable (void)
{
    /* Enable audio apb and audio clock */
    //CLOCK_LOG_TRACE:"[_AudioEnable] IS NULL"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1896_112_2_17_23_3_49_650,(uint8*)"");
    

}

/******************************************************************************
 * _AudioDisable
 ******************************************************************************/
LOCAL void _AudioDisable (void)
{
    /* Disable rtc apb clock */
    //CLOCK_LOG_TRACE:"[_AudioDisable] IS NULL"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1906_112_2_17_23_3_49_651,(uint8*)"");

}

/******************************************************************************
 * _AudioReset
 ******************************************************************************/
LOCAL void _AudioReset (void)
{
    //CLOCK_LOG_TRACE:"[_AudioReset] IS NULL"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1914_112_2_17_23_3_49_652,(uint8*)"");
}

/******************************************************************************
 * _Sim0Enable
 ******************************************************************************/
LOCAL void _DummyEnable (void)
{
    /* dummy operation */
    //CLOCK_LOG_TRACE:"[_DummyEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1922_112_2_17_23_3_49_653,(uint8*)"");
    
}

/******************************************************************************
 * _Sim0Disable
 ******************************************************************************/
LOCAL void _DummyDisable (void)
{
    /* dummy operation */
    //CLOCK_LOG_TRACE:"[_DummyDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1931_112_2_17_23_3_49_654,(uint8*)"");
    
}

/******************************************************************************
 * _Sim0Reset
 ******************************************************************************/
LOCAL void _DummyReset (void)
{
    /* dummy operation */
    //CLOCK_LOG_TRACE:"[_DummyReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1940_112_2_17_23_3_49_655,(uint8*)"");

}

/******************************************************************************
 * Static functions prototypes
 ******************************************************************************/
LOCAL const CLOCK system_clock[] =
{
    //.Name     //.value   //.DefaultSrc //.bEnable //.Enable //.Disable
    {
    "CLK_32K",     32768,  "CLK_32K",    SCI_TRUE, _RtcXtlEnable,_RtcXtlDisable, 
    },    
    {
    "26M_XTL",  26000000,  "26M_XTL",    SCI_TRUE, _26MXtlEnable,_26MXtlDisable, 
    },
    {
    "MPLL",    624000000,  "26M_XTL",    SCI_TRUE, _MpllEnable, _MpllDisable, 
    },
    {
    "CLK_208M",208000000,  "MPLL",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
    "CLK_104M",104000000,  "MPLL",       SCI_TRUE, SCI_NULL, SCI_NULL, 
    },
    {
    "CLK_156M",156000000,  "MPLL",       SCI_TRUE, SCI_NULL, SCI_NULL,    
    },
    {
    "CLK_78M",  78000000,  "MPLL",       SCI_TRUE, SCI_NULL, SCI_NULL,    
    },
    {
    "CLK_39M",  39000000,  "MPLL",       SCI_TRUE, SCI_NULL, SCI_NULL,    
    },
    {
    "CLK_89M1", 89100000,  "MPLL",       SCI_TRUE, SCI_NULL, SCI_NULL,        
    },
    {
    "CLK_44M6", 44600000,  "MPLL",       SCI_TRUE, SCI_NULL, SCI_NULL,        
    },
    {
    "CLK_62M4", 62400000,  "MPLL",       SCI_TRUE, SCI_NULL, SCI_NULL,        
    },
    {
    "CLK_31M2", 31200000,  "MPLL",       SCI_TRUE, SCI_NULL, SCI_NULL,        
    },
    {
    "CLK_48M",  48000000,  "MPLL",       SCI_TRUE, SCI_NULL, SCI_NULL,        
    },
    {
    "CLK_12M",  12000000,  "MPLL",       SCI_TRUE, SCI_NULL, SCI_NULL,        
    },
   
};

LOCAL const DEVICE system_device[] =
{
  //.Name         //.DefaultSrc  //.ParentDe //.Div        //.bEnable
  //.Enable       //.Disable     //.SrcSet   //.DivSet     //.Reset
{
    "ARM",        208000000,      "ARM",      0,            SCI_TRUE,
    SCI_NULL,     SCI_NULL,      _ArmSrcSet, SCI_NULL,     SCI_NULL,
},
{
    "AHB",        208000000,      "ARM",      1,            SCI_TRUE,
    _AhbEnable,   _AhbDisable,   SCI_NULL,   _AhbSrcDivSet,SCI_NULL,
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
	_AdiEnable,   _AdiDisable,   SCI_NULL,   SCI_NULL,     _AdiReset,   
},  
{
    "WATCHDOG",   SCI_NULL,      "ADI",      0,            SCI_TRUE,
    _WdgEnable,   _WdgDisable,   SCI_NULL,   SCI_NULL,     _WdgReset, 
},    
{
    "TPC",        SCI_NULL,      "ADI",      0,            SCI_TRUE,


    _TpcEnable,   _TpcDisable,   SCI_NULL,   SCI_NULL,     _TpcReset,
},
{
    "ADC",        SCI_NULL,      "ADI",      0,            SCI_TRUE, 
    _AdcEnable,   _AdcDisable,   SCI_NULL,   SCI_NULL,     _AdcReset,
},
{
    "CHGRWDG",    SCI_NULL,      "ADI",      0,            SCI_TRUE,   
    _ChgrwdgEnable,_ChgrwdgDisable,SCI_NULL, SCI_NULL,     _ChgrwdgReset,
},
{
    "RTC",        SCI_NULL,      "ADI",      0,            SCI_TRUE, 
    _RtcEnable,   _RtcDisable,   SCI_NULL,    SCI_NULL,    _RtcReset,
},
{
    "GPIO_A",     SCI_NULL,       "ADI",      0,           SCI_TRUE,      
    _GpioaEnable, _GpioaDisable,  SCI_NULL,   SCI_NULL,    _GpioaReset,      
},
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
{
     "SPI1",      48000000,       "APB",      0,           SCI_TRUE,       
     _Spi1Enable, _Spi1Disable,   _Spi1SrcSet,_Spi1SrcDivSet,_Spi1Reset,       
},
{
     "UART0",     SCI_NULL,       "APB",      0,           SCI_TRUE,      
     _Uart0Enable,_Uart0Disable,  _Uart0SrcSet,SCI_NULL,   _Uart0Reset,      
},
{
     "UART1",     SCI_NULL,       "APB",      0,           SCI_TRUE,      
     _Uart1Enable,_Uart1Disable,  _Uart1SrcSet,SCI_NULL,   _Uart1Reset,      
},
{
     "PWM",       SCI_NULL,       "APB",      0,           SCI_TRUE,     
     _PwmEnable,  _PwmDisable,    _PwmSrcSet, SCI_NULL,    _PwmReset,     
},
{
     "PIN",       SCI_NULL,       "APB",      0,           SCI_TRUE,     
     _PinEnable,  _PinDisable,    SCI_NULL, SCI_NULL,    SCI_NULL,     
},
{
        "PCM",         //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _I2s0Enable,   //.Enable
        _I2s0Disable,  //.Disable
        _I2sSrcSet,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
{
     "AUX0",      SCI_NULL,       "APB",      0,           SCI_TRUE,      
     _Aux0Enable, _Aux0Disable,   _AuxSrcSet, _AuxSrcDivSet,SCI_NULL,      
},
{
     "I2C0",      SCI_NULL,       "APB",      0,           SCI_TRUE,     
     _IIC0Enable, _IIC0Disable,   SCI_NULL,   SCI_NULL,    _IIC0Reset,     
},
{
     "I2C1",      SCI_NULL,       "APB",      0,           SCI_TRUE,     
     _IIC1Enable, _IIC1Disable,   SCI_NULL,   SCI_NULL,    _IIC1Reset,     
},
{
     "AUDIO",     SCI_NULL,       "APB",      0,           SCI_FALSE,      
     _AudioEnable,_AudioDisable,  SCI_NULL,   SCI_NULL,    _AudioReset,       
},

  // Description for Devices in AHB BUS
  //.Name         //.DefaultSrc  //.ParentDe //.Div        //.bEnable
  //.Enable       //.Disable     //.SrcSet   //.DivSet     //.Reset
{
     "CCIR",      48000000,       "AHB",      0,           SCI_TRUE,     
     SCI_NULL,    SCI_NULL,       SCI_NULL,   SCI_NULL,    _CcirReset,   
},
{
     "SPI0",      78000000,       "AHB",      0,           SCI_TRUE,       
     _Spi0Enable, _Spi0Disable,   _Spi0SrcSet,_Spi0SrcDivSet,SCI_NULL,       
},
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
     _UsbEnable,  _UsbDisable,    SCI_NULL,   SCI_NULL,    _UsbReset,     
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
     "BUSMONITOR1",SCI_NULL,      "AHB",      0,           SCI_TRUE,      
     _Bm1Enable,  _Bm1Disable,    SCI_NULL,   SCI_NULL,    SCI_NULL,     
},
{
     "BUSMONITOR2",SCI_NULL,      "AHB",      0,           SCI_TRUE,      
     _Bm2Enable,  _Bm2Disable,    SCI_NULL,   SCI_NULL,    SCI_NULL,     
},
{
      "LZMA",     208000000,       "AHB",      0,           SCI_TRUE,      
      _LzmaEnable,_LzmaDisable,   _LzmaSrcSet,SCI_NULL,    _LzmaReset,     
},
{
      "ROTA",     SCI_NULL,       "AHB",      0,           SCI_TRUE,      
      _RotaEnable,_RotaDisable,   SCI_NULL,   SCI_NULL,    _RotaReset,     
},
{
      "TIC",      SCI_NULL,       "AHB",      0,           SCI_TRUE,      
      _TicEnable, _TicDisable,    SCI_NULL,   SCI_NULL,     SCI_NULL,     
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

