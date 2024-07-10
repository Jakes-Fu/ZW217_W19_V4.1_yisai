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
#include "Ref_outport.h"

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
    CHIP_REG_AND(APB_CLK_CFG3, ~CLK_MCU_SEL_MSK);

    if (strcmp ("CLK_26M", SrcName) == 0)
    {//<<NemoG CLK PRE DIV Control Register.xlsx>>:cgm_26m_switch_aon_sel select CLK_26M or CLK_26M_TUNED,default is CLK_26M
        CHIP_REG_OR(APB_CLK_CFG3, 0<<CLK_MCU_SEL_SHIFT);
    }
    else if (strcmp ("CLK_104M", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG3, 1<<CLK_MCU_SEL_SHIFT);
    }		
    else if (strcmp ("CLK_208M", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG3, 2<<CLK_MCU_SEL_SHIFT);
    }
    else if (strcmp ("CLK_208M_APLL", SrcName) == 0)
    {
        CHIP_REG_OR(APB_CLK_CFG3, 3<<CLK_MCU_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_ArmSrcSet", SrcName, "CLK_26M, CLK_104M, CLK_208M, CLK_208M_APLL"));
    }
}

/******************************************************************************
 * _ApbSrcSet
 ******************************************************************************/
LOCAL void _ApbSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_APB_CFG, ~CGM_APB_SEL_MSK);

    if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_APB_CFG, 0<<CGM_APB_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_APB_CFG, 1<<CGM_APB_SEL_SHIFT);
    }		
    else if (strcmp ("CLK_24M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_APB_CFG, 2<<CGM_APB_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_ApbSrcSet", SrcName, "CLK_26M_TUNED, CLK_26M, CLK_24M"));
    }
}

/******************************************************************************
 * _SfcSrcSet
 ******************************************************************************/
LOCAL void _SfcSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_SFC_2X_CFG, ~CGM_SFC_2X_SEL_MSK);

    if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SFC_2X_CFG, 0<<CGM_SFC_2X_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SFC_2X_CFG, 1<<CGM_SFC_2X_SEL_SHIFT);
    }		
    else if (strcmp ("CLK_156M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SFC_2X_CFG, 2<<CGM_SFC_2X_SEL_SHIFT);
    }
    else if (strcmp ("CLK_166M4", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SFC_2X_CFG, 3<<CGM_SFC_2X_SEL_SHIFT);
    }
    else if (strcmp ("CLK_208M_APLL", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SFC_2X_CFG, 4<<CGM_SFC_2X_SEL_SHIFT);
    }
    else if (strcmp ("CLK_208M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SFC_2X_CFG, 5<<CGM_SFC_2X_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_SfcSrcSet", SrcName, "CLK_26M_TUNED, CLK_26M, CLK_156M, CLK_166M4, CLK_208M_APLL, CLK_208M"));
    }
}

/******************************************************************************
 * _SfcSrcDivSet
 ******************************************************************************/
LOCAL void _SfcSrcDivSet (int Div)
{
    SCI_PASSERT ( (3 >= Div), ("_SfcSrcDivSet exceeds the max_value."));/*assert verified*/
	
    CHIP_REG_AND(CGM_SFC_1X_CFG, ~CGM_SFC_1X_DIV_MSK);
	CHIP_REG_OR(CGM_SFC_1X_CFG, Div<<CGM_SFC_1X_DIV_SHIFT);
}

/******************************************************************************
 * _DcamSrcSet
 ******************************************************************************/
LOCAL void _DcamSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_DCAM_CFG, ~CGM_DCAM_SEL_MSK);

    if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_DCAM_CFG, 0<<CGM_DCAM_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_DCAM_CFG, 1<<CGM_DCAM_SEL_SHIFT);
    }		
    else if (strcmp ("CLK_83M2", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_DCAM_CFG, 2<<CGM_DCAM_SEL_SHIFT);
    }
    else if (strcmp ("CLK_104M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_DCAM_CFG, 3<<CGM_DCAM_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_DcamSrcSet", SrcName, "CLK_26M_TUNED, CLK_26M, CLK_83M2, CLK_104M, CLK_138M6"));
    }
}

/******************************************************************************
 * _SensorClkSrcSet
 ******************************************************************************/
LOCAL void _SensorClkSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_SENSOR_CFG, ~CGM_SENSOR_SEL_MSK);

    if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SENSOR_CFG, 0<<CGM_SENSOR_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SENSOR_CFG, 1<<CGM_SENSOR_SEL_SHIFT);
    }		
    else if (strcmp ("CLK_48M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SENSOR_CFG, 2<<CGM_SENSOR_SEL_SHIFT);
    }
    else if (strcmp ("CLK_64M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SENSOR_CFG, 3<<CGM_SENSOR_SEL_SHIFT);
    }
    else if (strcmp ("CLK_83M2", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SENSOR_CFG, 4<<CGM_SENSOR_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_SensorClkSrcSet", SrcName, "CLK_26M_TUNED, CLK_26M, CLK_48M, CLK_64M, CLK_83M2"));
    }
}
/******************************************************************************
 * _SensorSrcDivSet
 ******************************************************************************/
LOCAL void _SensorSrcDivSet (int Div)
{
    SCI_PASSERT ( (7 >= Div), ("_SensorSrcDivSet exceeds the max_value."));/*assert verified*/
	
    CHIP_REG_AND(CGM_SENSOR_CFG, ~CGM_SENSOR_DIV_MSK);
	CHIP_REG_OR(CGM_SENSOR_CFG, Div<<CGM_SENSOR_DIV_SHIFT);
}

/******************************************************************************
 * _SdioSrcSet
 ******************************************************************************/
LOCAL void _SdioSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_SDIO_CFG, ~CGM_SDIO_SEL_MSK);
   
    if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SDIO_CFG, 0<<CGM_SDIO_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SDIO_CFG, 1<<CGM_SDIO_SEL_SHIFT);
    }
    else if (strcmp ("CLK_48M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SDIO_CFG, 2<<CGM_SDIO_SEL_SHIFT);
    }
    else if (strcmp ("CLK_75M6", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SDIO_CFG, 3<<CGM_SDIO_SEL_SHIFT);
    }
    else if (strcmp ("CLK_104M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SDIO_CFG, 4<<CGM_SDIO_SEL_SHIFT);
    }
    else if (strcmp ("CLK_104M_APLL", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SDIO_CFG, 5<<CGM_SDIO_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_SdioSrcSet", SrcName, "CLK_26M_TUNED, CLK_26M, CLK_48M, CLK_75M6, CLK_104M, CLK_104M_APLL"));
    }
}

/******************************************************************************
 * _I2s0SrcSet
 ******************************************************************************/
LOCAL void _I2s0SrcSet (const char *SrcName)
{
	if (strcmp ("PAD_CLK", SrcName) == 0)
	{
		CHIP_REG_OR(CGM_IIS0_CFG, CGM_IIS0_PAD_SEL_MSK);
	}
	else
	{
	    CHIP_REG_AND(CGM_IIS0_CFG, ~CGM_IIS0_SEL_MSK);
		
	    if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
	    {
	        CHIP_REG_OR(CGM_IIS0_CFG, 0<<CGM_IIS0_SEL_SHIFT);
	    }
	    else if (strcmp ("CLK_26M", SrcName) == 0)
	    {
	        CHIP_REG_OR(CGM_IIS0_CFG, 1<<CGM_IIS0_SEL_SHIFT);
	    }
	    else if (strcmp ("CLK_64M_FM", SrcName) == 0)
	    {
	        CHIP_REG_OR(CGM_IIS0_CFG, 2<<CGM_IIS0_SEL_SHIFT);
	    }
	    else if (strcmp ("CLK_83M2", SrcName) == 0)
	    {
	        CHIP_REG_OR(CGM_IIS0_CFG, 3<<CGM_IIS0_SEL_SHIFT);
	    }
	    else if (strcmp ("CLK_124M8", SrcName) == 0)
	    {
	        CHIP_REG_OR(CGM_IIS0_CFG, 4<<CGM_IIS0_SEL_SHIFT);
	    }
	    else
	    {
	        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
	                     (err_string, "_I2s0SrcSet", SrcName, "CLK_26M_TUNED, CLK_26M, CLK_64M_FM, CLK_83M2, CLK_124M8"));
	    }
	}
}

/******************************************************************************
 * _I2s0SrcDivSet
 ******************************************************************************/
LOCAL void _I2s0SrcDivSet (int Div)
{
    SCI_PASSERT ( (0x3F >= Div), ("_I2s0SrcDivSet exceeds the max_value."));/*assert verified*/
	
    CHIP_REG_AND(CGM_IIS0_CFG, ~CGM_IIS0_DIV_MSK);
	CHIP_REG_OR(CGM_IIS0_CFG, Div<<CGM_IIS0_DIV_SHIFT);
}

/******************************************************************************
 * _Uart0SrcSet
 ******************************************************************************/
LOCAL void _Uart0SrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_UART0_CFG, ~CGM_UART0_SEL_MSK);
	
    if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_UART0_CFG, 0<<CGM_UART0_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_UART0_CFG, 1<<CGM_UART0_SEL_SHIFT);
    }
    else if (strcmp ("CLK_104M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_UART0_CFG, 2<<CGM_UART0_SEL_SHIFT);
    }
    else if (strcmp ("CLK_104M_APLL", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_UART0_CFG, 3<<CGM_UART0_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Uart0SrcSet", SrcName, "CLK_26M_TUNED, CLK_26M, CLK_104M, CLK_104M_APLL"));
    }
}

/******************************************************************************
 * _Uart1SrcSet
 ******************************************************************************/
LOCAL void _Uart1SrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_UART1_CFG, ~CGM_UART1_SEL_MSK);
	
    if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_UART1_CFG, 0<<CGM_UART1_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_UART1_CFG, 1<<CGM_UART1_SEL_SHIFT);
    }
    else if (strcmp ("CLK_104M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_UART1_CFG, 2<<CGM_UART1_SEL_SHIFT);
    }
    else if (strcmp ("CLK_104M_APLL", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_UART1_CFG, 3<<CGM_UART1_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Uart1SrcSet", SrcName, "CLK_26M_TUNED, CLK_26M, CLK_104M, CLK_104M_APLL"));
    }
}

/******************************************************************************
 * _Pwm0SrcSet
 ******************************************************************************/
LOCAL void _PwmSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_PWM_CFG, ~CGM_PWM_SEL_MSK);

    if (strcmp ("CLK_32K", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_PWM_CFG, 0<<CGM_PWM_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_PWM_CFG, 1<<CGM_PWM_SEL_SHIFT);
    }
    /****************************************************
    *** owner: qingsheng.li
    *** bug: can't output 5K PWM
    *** resolution: Because 26M_XTL value is same as CLK_26M_TUNED,
    ***			so we set same config for CGM_PWM_CFG
    ****************************************************/
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_PWM_CFG, 1<<CGM_PWM_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_PWM_CFG, 2<<CGM_PWM_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_PwmSrcSet", SrcName, "CLK_32K, CLK_26M_TUNED, CLK_26M"));
    }
}

/******************************************************************************
 * _Spi0SrcSet
 ******************************************************************************/
LOCAL void _Spi0SrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_SPI0_CFG, ~CGM_SPI0_SEL_MSK);
		
    if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SPI0_CFG, 0<<CGM_SPI0_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SPI0_CFG, 1<<CGM_SPI0_SEL_SHIFT);
    }
    else if (strcmp ("CLK_46M2", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SPI0_CFG, 2<<CGM_SPI0_SEL_SHIFT);
    }
    else if (strcmp ("CLK_75M6", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SPI0_CFG, 3<<CGM_SPI0_SEL_SHIFT);
    }	
    else if (strcmp ("CLK_104M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SPI0_CFG, 4<<CGM_SPI0_SEL_SHIFT);
    }
    else if (strcmp ("CLK_104M_APLL", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SPI0_CFG, 5<<CGM_SPI0_SEL_SHIFT);
    }	
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Spi0SrcSet", SrcName, "CLK_26M_TUNED, CLK_26M, CLK_46M2,CLK_75M6, CLK_104M, CLK_104M_APLL"));
    }
}

/******************************************************************************
 * _Spi0SrcDivSet
 ******************************************************************************/
LOCAL void _Spi0SrcDivSet (int Div)
{
    SCI_PASSERT ( (3 >= Div), ("_Spi0SrcDivSet exceeds the max_value."));/*assert verified*/
	
    CHIP_REG_AND(CGM_SPI0_CFG, ~CGM_SPI0_DIV_MSK);
	CHIP_REG_OR(CGM_SPI0_CFG, Div<<CGM_SPI0_DIV_SHIFT);
}

/******************************************************************************
 * _Spi1SrcSet
 ******************************************************************************/
LOCAL void _Spi1SrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_SPI1_CFG, ~CGM_SPI1_SEL_MSK);
		
    if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SPI1_CFG, 0<<CGM_SPI1_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SPI1_CFG, 1<<CGM_SPI1_SEL_SHIFT);
    }
    else if (strcmp ("CLK_46M2", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SPI1_CFG, 2<<CGM_SPI1_SEL_SHIFT);
    }
    else if (strcmp ("CLK_48M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SPI1_CFG, 3<<CGM_SPI1_SEL_SHIFT);
    }
    else if (strcmp ("CLK_75M6", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SPI1_CFG, 4<<CGM_SPI1_SEL_SHIFT);
    }	
    else if (strcmp ("CLK_104M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SPI1_CFG, 5<<CGM_SPI1_SEL_SHIFT);
    }
    else if (strcmp ("CLK_104M_APLL", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_SPI1_CFG, 6<<CGM_SPI1_SEL_SHIFT);
    }	
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Spi1SrcSet", SrcName, "CLK_26M_TUNED, CLK_26M, CLK_46M2, CLK_48M, CLK_75M6, CLK_104M, CLK_104M_APLL"));
    }
}

/******************************************************************************
 * _Spi1SrcDivSet
 ******************************************************************************/
LOCAL void _Spi1SrcDivSet (int Div)
{
    SCI_PASSERT ( (3 >= Div), ("_Spi1SrcDivSet exceeds the max_value."));/*assert verified*/
	
    CHIP_REG_AND(CGM_SPI1_CFG, ~CGM_SPI1_DIV_MSK);
	CHIP_REG_OR(CGM_SPI1_CFG, Div<<CGM_SPI1_DIV_SHIFT);
}

/******************************************************************************
 * _AuxSrcSet
 ******************************************************************************/
LOCAL void _AuxSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_AUX_CFG, ~CGM_AUX_SEL_MSK);
		
    if (strcmp ("CLK_32K", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_AUX_CFG, 0<<CGM_AUX_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M_RAW", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_AUX_CFG, 1<<CGM_AUX_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_AUX_CFG, 2<<CGM_AUX_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_AUX_CFG, 3<<CGM_AUX_SEL_SHIFT);
    }
    else if (strcmp ("CLK_46M2", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_AUX_CFG, 4<<CGM_AUX_SEL_SHIFT);
    }
    else if (strcmp ("CLK_48M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_AUX_CFG, 5<<CGM_AUX_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_AuxSrcSet", SrcName, "CLK_32K, CLK_26M_RAW, CLK_26M_TUNED, CLK_26M, CLK_46M2, CLK_48M"));
    }
}

/******************************************************************************
 * _AuxSrcDivSet
 ******************************************************************************/
LOCAL void _AuxSrcDivSet (int Div)
{
    SCI_PASSERT ( (3 >= Div), ("_AuxSrcDivSet exceeds the max_value."));/*assert verified*/
	
    CHIP_REG_AND(CGM_AUX_CFG, ~CGM_AUX_DIV_MSK);
	CHIP_REG_OR(CGM_AUX_CFG, Div<<CGM_AUX_DIV_SHIFT);
}

/******************************************************************************
 * _Usb48SrcSet
 ******************************************************************************/
LOCAL void _Usb48SrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_USB48_CFG, ~CGM_USB48_SEL_MSK);

    if (strcmp ("CLK_48M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_USB48_CFG, 0<<CGM_USB48_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Usb48SrcSet", SrcName, "CLK_48M"));
    }
}

/******************************************************************************
 * _Usb12SrcSet
 ******************************************************************************/
LOCAL void _Usb12SrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_USB12_CFG, ~CGM_USB12_SEL_MSK);

    if (strcmp ("CLK_12M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_USB12_CFG, 0<<CGM_USB12_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Usb12SrcSet", SrcName, "CLK_12M"));
    }
}

/******************************************************************************
 * _32KBtSrcSet
 ******************************************************************************/
LOCAL void _32KBtSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_32K_BT_CFG, ~CGM_32K_BT_SEL_MSK);

    if (strcmp ("CLK_32K", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_32K_BT_CFG, 0<<CGM_32K_BT_SEL_SHIFT);
    }
    else if (strcmp ("CLK_32K768", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_32K_BT_CFG, 1<<CGM_32K_BT_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_32KBtSrcSet", SrcName, "CLK_32K, CLK_32K768"));
    }
}

/******************************************************************************
 * _32KStcSrcSet
 ******************************************************************************/
LOCAL void _32KStcSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_32K_STC_CFG, ~CGM_32K_STC_SEL_MSK);

    if (strcmp ("CLK_32K", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_32K_STC_CFG, 0<<CGM_32K_STC_SEL_SHIFT);
    }
    else if (strcmp ("CLK_32K768", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_32K_STC_CFG, 1<<CGM_32K_STC_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_32KStcSrcSet", SrcName, "CLK_32K, CLK_32K768"));
    }
}

/******************************************************************************
 * _32KOutSrcSet
 ******************************************************************************/
LOCAL void _32KOutSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_32K_OUT_CFG, ~CGM_32K_OUT_SEL_MSK);

    if (strcmp ("CLK_32K", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_32K_OUT_CFG, 0<<CGM_32K_OUT_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_32KOutSrcSet", SrcName, "CLK_32K"));
    }
}

/******************************************************************************
 * _1KOutSrcSet
 ******************************************************************************/
LOCAL void _1KOutSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_1K_OUT_CFG, ~CGM_1K_OUT_SEL_MSK);

    if (strcmp ("CLK_1K", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_1K_OUT_CFG, 0<<CGM_1K_OUT_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_1KOutSrcSet", SrcName, "CLK_1K"));
    }
}

/******************************************************************************
 * _IIC0SrcSet
 ******************************************************************************/
LOCAL void _IIC0SrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_I2C0_CFG, ~CGM_I2C0_SEL_MSK);
	
    if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_I2C0_CFG, 0<<CGM_I2C0_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_I2C0_CFG, 1<<CGM_I2C0_SEL_SHIFT);
    }
    else if (strcmp ("CLK_48M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_I2C0_CFG, 2<<CGM_I2C0_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_IIC0SrcSet", SrcName, "CLK_26M_TUNED, CLK_26M, CLK_48M"));
    }
}

/******************************************************************************
 * _IIC1SrcSet
 ******************************************************************************/
LOCAL void _IIC1SrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_I2C1_CFG, ~CGM_I2C1_SEL_MSK);
	
    if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_I2C1_CFG, 0<<CGM_I2C1_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_I2C1_CFG, 1<<CGM_I2C1_SEL_SHIFT);
    }
    else if (strcmp ("CLK_48M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_I2C1_CFG, 2<<CGM_I2C1_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_IIC1SrcSet", SrcName, "CLK_26M_TUNED, CLK_26M, CLK_48M"));
    }
}

/******************************************************************************
 * _IIC2SrcSet
 ******************************************************************************/
LOCAL void _IIC2SrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_I2C2_CFG, ~CGM_I2C2_SEL_MSK);
	
    if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_I2C2_CFG, 0<<CGM_I2C2_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_I2C2_CFG, 1<<CGM_I2C2_SEL_SHIFT);
    }
    else if (strcmp ("CLK_48M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_I2C2_CFG, 2<<CGM_I2C2_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_IIC2SrcSet", SrcName, "CLK_26M_TUNED, CLK_26M, CLK_48M"));
    }
}

/******************************************************************************
 * _LzmaSrcSet
 ******************************************************************************/
LOCAL void _LzmaSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_ZIP0_CFG, ~CGM_ZIP0_SEL_MSK);
	
    if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_ZIP0_CFG, 0<<CGM_ZIP0_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_ZIP0_CFG, 1<<CGM_ZIP0_SEL_SHIFT);
    }
    else if (strcmp ("CLK_104M_APLL", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_ZIP0_CFG, 2<<CGM_ZIP0_SEL_SHIFT);
    }
    else if (strcmp ("CLK_208M_APLL", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_ZIP0_CFG, 3<<CGM_ZIP0_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_LzmaSrcSet", SrcName, "CLK_26M_TUNED, CLK_26M, CLK_104M_APLL, CLK_208M_APLL"));
    }
}

/******************************************************************************
 * _EfsSrcSet
 ******************************************************************************/
LOCAL void _EfsSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_EFS_CFG, ~CGM_EFS_SEL_MSK);
	
    if (strcmp ("CLK_13M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_EFS_CFG, 0<<CGM_EFS_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_EFS_CFG, 1<<CGM_EFS_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_EFS_CFG, 2<<CGM_EFS_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_EfsSrcSet", SrcName, "CLK_13M, CLK_26M_TUNED, CLK_26M"));
    }
}


/******************************************************************************
 * _Cm4AsyncSrcSet
 ******************************************************************************/
LOCAL void _Cm4AsyncSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_CM4_ASYNC_CFG, ~CGM_CM4_ASYNC_SEL_MSK);
		
    if (strcmp ("CLK_1M_RAW", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_CM4_ASYNC_CFG, 0<<CGM_CM4_ASYNC_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_CM4_ASYNC_CFG, 1<<CGM_CM4_ASYNC_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_CM4_ASYNC_CFG, 2<<CGM_CM4_ASYNC_SEL_SHIFT);
    }
    else if (strcmp ("CLK_64M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_CM4_ASYNC_CFG, 3<<CGM_CM4_ASYNC_SEL_SHIFT);
    }
    else if (strcmp ("CLK_104M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_CM4_ASYNC_CFG, 4<<CGM_CM4_ASYNC_SEL_SHIFT);
    }
    else if (strcmp ("CLK_104M_APLL", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_CM4_ASYNC_CFG, 5<<CGM_CM4_ASYNC_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Cm4AsyncSrcSet", SrcName, "CLK_1M_RAW, CLK_26M_TUNED, CLK_26M, CLK_64M, CLK_104M, CLK_104M_APLL"));
    }
}

/******************************************************************************
 * _Cm4AsyncSrcDivSet
 ******************************************************************************/
LOCAL void _Cm4AsyncSrcDivSet (int Div)
{
    SCI_PASSERT ( (3 >= Div), ("_Cm4AsyncSrcDivSet exceeds the max_value."));/*assert verified*/
	
    CHIP_REG_AND(CGM_CM4_ASYNC_CFG, ~CGM_CM4_ASYNC_DIV_MSK);
	CHIP_REG_OR(CGM_CM4_ASYNC_CFG, Div<<CGM_CM4_ASYNC_DIV_SHIFT);
}

/******************************************************************************
 * _Cm4ApbSrcSet
 ******************************************************************************/
LOCAL void _Cm4ApbSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_CM4_APB_CFG, ~CGM_CM4_APB_SEL_MSK);
		
    if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_CM4_APB_CFG, 0<<CGM_CM4_APB_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_CM4_APB_CFG, 1<<CGM_CM4_APB_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Cm4ApbSrcSet", SrcName, "CLK_26M_TUNED, CLK_26M"));
    }
}

/******************************************************************************
 * _FmSrcSet
 ******************************************************************************/
LOCAL void _FmSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_FM_CFG, ~CGM_FM_SEL_MSK);
		
    if (strcmp ("CLK_64M_FM", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_FM_CFG, 0<<CGM_FM_SEL_SHIFT);
    }
    else if (strcmp ("CLK_64M_FM_INV", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_FM_CFG, 1<<CGM_FM_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_FmSrcSet", SrcName, "CLK_64M_FM, CLK_64M_FM_INV"));
    }
}

/******************************************************************************
 * _FmRfCtrlSrcSet
 ******************************************************************************/
LOCAL void _FmRfCtrlSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_FM_RF_CTRL_CFG, ~CGM_FM_RF_CTRL_SEL_MSK);
		
    if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_FM_RF_CTRL_CFG, 0<<CGM_FM_RF_CTRL_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_FM_RF_CTRL_CFG, 1<<CGM_FM_RF_CTRL_SEL_SHIFT);
    }
    else if (strcmp ("CLK_64M_FM", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_FM_RF_CTRL_CFG, 2<<CGM_FM_RF_CTRL_SEL_SHIFT);
    }
    else if (strcmp ("CLK_64M_FM_INV", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_FM_RF_CTRL_CFG, 3<<CGM_FM_RF_CTRL_SEL_SHIFT);
    }
    else if (strcmp ("CLK_83M2", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_FM_RF_CTRL_CFG, 4<<CGM_FM_RF_CTRL_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_FmRfCtrlSrcSet", SrcName, " CLK_26M_TUNED, CLK_26M, CLK_64M_FM, CLK_64M_FM_INV, CLK_83M2"));
    }
}

/******************************************************************************
 * _DapSrcSet
 ******************************************************************************/
LOCAL void _DapSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_DAP_CFG, ~CGM_DAP_SEL_MSK);
		
    if (strcmp ("CLK_13M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_DAP_CFG, 0<<CGM_DAP_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_DAP_CFG, 1<<CGM_DAP_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_DAP_CFG, 2<<CGM_DAP_SEL_SHIFT);
    }
    else if (strcmp ("CLK_64M_FM", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_DAP_CFG, 3<<CGM_DAP_SEL_SHIFT);
    }
    else if (strcmp ("CLK_83M2", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_DAP_CFG, 4<<CGM_DAP_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_DapSrcSet", SrcName, "CLK_13M, CLK_26M_TUNED, CLK_26M, CLK_64M_FM, CLK_83M2"));
    }
}

/******************************************************************************
 * _DspSrcSet
 ******************************************************************************/
LOCAL void _DspSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_DSP_CFG, ~CGM_DSP_SEL_MSK);
		
    if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_DSP_CFG, 0<<CGM_DSP_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_DSP_CFG, 1<<CGM_DSP_SEL_SHIFT);
    }
    else if (strcmp ("CLK_75M6", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_DSP_CFG, 2<<CGM_DSP_SEL_SHIFT);
    }
    else if (strcmp ("CLK_104M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_DSP_CFG, 3<<CGM_DSP_SEL_SHIFT);
    }
    else if (strcmp ("CLK_104M_APLL", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_DSP_CFG, 4<<CGM_DSP_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_DspSrcSet", SrcName, "CLK_26M_TUNED, CLK_26M, CLK_75M6, CLK_104M, CLK_104M_APLL"));
    }
}

/******************************************************************************
 * _AudSrcSet
 ******************************************************************************/
LOCAL void _AudSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_AUD_CFG, ~CGM_AUD_SEL_MSK);
		
    if (strcmp ("CLK_26M_RAW", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_AUD_CFG, 0<<CGM_AUD_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M_RAW_INV", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_AUD_CFG, 1<<CGM_AUD_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_AUD_CFG, 2<<CGM_AUD_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M_TUNED_INV", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_AUD_CFG, 3<<CGM_AUD_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_AudSrcSet", SrcName, "CLK_26M_RAW, CLK_26M_RAW_INV, CLK_26M_TUNED, CLK_26M_TUNED_INV"));
    }
}

/******************************************************************************
 * _AudIfSrcSet
 ******************************************************************************/
LOCAL void _AudIfSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_AUDIF_CFG, ~CGM_AUDIF_SEL_MSK);
		
    if (strcmp ("CLK_26M_RAW", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_AUDIF_CFG, 0<<CGM_AUDIF_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M_RAW_INV", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_AUDIF_CFG, 1<<CGM_AUDIF_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_AUDIF_CFG, 2<<CGM_AUDIF_SEL_SHIFT);
    }
    else if (strcmp ("CLK_37M8", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_AUDIF_CFG, 3<<CGM_AUDIF_SEL_SHIFT);
    }
    else if (strcmp ("CLK_46M2", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_AUDIF_CFG, 4<<CGM_AUDIF_SEL_SHIFT);
    }
    else if (strcmp ("CLK_48M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_AUDIF_CFG, 5<<CGM_AUDIF_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_AudIfSrcSet", SrcName, "CLK_26M_RAW, CLK_26M_RAW_INV, CLK_26M_TUNED, CLK_37M8, CLK_46M2, CLK_48M"));
    }
}

/******************************************************************************
 * _AdiSrcSet
 ******************************************************************************/
LOCAL void _AdiSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_ADI_CFG, ~CGM_ADI_SEL_MSK);

    if (strcmp ("CLK_26M_RAW", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_ADI_CFG, 0<<CGM_ADI_SEL_SHIFT);
    }
    else if (strcmp ("CLK_48M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_ADI_CFG, 1<<CGM_ADI_SEL_SHIFT);
    }
    else if (strcmp ("CLK_75M6", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_ADI_CFG, 2<<CGM_ADI_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_AdiSrcSet", SrcName, "CLK_26M_RAW, CLK_48M, CLK_75M6"));
    }
}

/******************************************************************************
 * _RfFm26MSrcSet
 ******************************************************************************/
LOCAL void _RfFm26MSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_RF_FM_26M_CFG, ~CGM_RF_FM_26M_SEL_MSK);

    if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_RF_FM_26M_CFG, 0<<CGM_RF_FM_26M_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_RF_FM_26M_CFG, 1<<CGM_RF_FM_26M_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_RfFm26MSrcSet", SrcName, "CLK_26M_TUNED, CLK_26M"));
    }
}

/******************************************************************************
 * _RfBt26MSrcSet
 ******************************************************************************/
LOCAL void _RfBt26MSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_RF_BT_26M_CFG, ~CGM_RF_BT_26M_SEL_MSK);

    if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_RF_BT_26M_CFG, 0<<CGM_RF_BT_26M_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_RF_BT_26M_CFG, 1<<CGM_RF_BT_26M_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_RfBt26MSrcSet", SrcName, "CLK_26M_TUNED, CLK_26M"));
    }
}

/******************************************************************************
 * _Bt64MSrcSet
 ******************************************************************************/
LOCAL void _Bt64MSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_BT_64M_CFG, ~CGM_BT_64M_SEL_MSK);

    if (strcmp ("CLK_64M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_BT_64M_CFG, 0<<CGM_BT_64M_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Bt64MSrcSet", SrcName, "CLK_64M"));
    }
}

/******************************************************************************
 * _Bt64MSrcDivSet
 ******************************************************************************/
LOCAL void _Bt64MSrcDivSet (int Div)
{
    if (Div == 0x1)
    {
		CHIP_REG_AND(CGM_BT_32M_CFG, ~CGM_BT_32M_DIV_MSK);
		CHIP_REG_OR(CGM_BT_32M_CFG, Div<<CGM_BT_32M_DIV_SHIFT);
    }
    else if (Div == 0x3)
    {
		CHIP_REG_AND(CGM_BT_16M_CFG, ~CGM_BT_16M_DIV_MSK);
		CHIP_REG_OR(CGM_BT_16M_CFG, Div<<CGM_BT_16M_DIV_SHIFT);
    }
    else if (Div == 0x7)
    {
		CHIP_REG_AND(CGM_BT_8M_CFG, ~CGM_BT_8M_DIV_MSK);
		CHIP_REG_OR(CGM_BT_8M_CFG, Div<<CGM_BT_8M_DIV_SHIFT);
    }
    else if (Div == 0xF)
    {
		CHIP_REG_AND(CGM_BT_4M_CFG, ~CGM_BT_4M_DIV_MSK);
		CHIP_REG_OR(CGM_BT_4M_CFG, Div<<CGM_BT_4M_DIV_SHIFT);
    }		
    else
    {
		SCI_PASSERT (SCI_FALSE, ("_Bt64MSrcDivSet exceeds the max_value."));/*assert verified*/
    }
}

/******************************************************************************
 * _Bt24MSrcSet:name is 24M,but the real value is 64M which base on clock plan.
 ******************************************************************************/
LOCAL void _Bt24MSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_BT_24M_CFG, ~CGM_BT_24M_SEL_MSK);

    if (strcmp ("CLK_64M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_BT_24M_CFG, 0<<CGM_BT_24M_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Bt64MSrcSet", SrcName, "CLK_64M"));
    }
}

/******************************************************************************
 * _BtLpoSrcSet
 ******************************************************************************/
LOCAL void _BtLpoSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_BT_LPO_CFG, ~CGM_BT_LPO_SEL_MSK);

    if (strcmp ("CLK_32K", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_BT_LPO_CFG, 0<<CGM_BT_LPO_SEL_SHIFT);
    }
    else if (strcmp ("CLK_32K768", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_BT_LPO_CFG, 1<<CGM_BT_LPO_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_BtLpoSrcSet", SrcName, "CLK_32K, CLK_32K768"));
    }
}

/******************************************************************************
 * _BtRfCtrlSrcSet
 ******************************************************************************/
LOCAL void _BtRfCtrlSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_BT_RF_CTRL_CFG, ~CGM_BT_RF_CTRL_SEL_MSK);
		
    if (strcmp ("CLK_26M_TUNED", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_BT_RF_CTRL_CFG, 0<<CGM_BT_RF_CTRL_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_BT_RF_CTRL_CFG, 1<<CGM_BT_RF_CTRL_SEL_SHIFT);
    }
    else if (strcmp ("CLK_64M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_BT_RF_CTRL_CFG, 2<<CGM_BT_RF_CTRL_SEL_SHIFT);
    }
    else if (strcmp ("CLK_83M2", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_BT_RF_CTRL_CFG, 3<<CGM_BT_RF_CTRL_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_BtRfCtrlSrcSet", SrcName, " CLK_26M_TUNED, CLK_26M, CLK_64M, CLK_83M2"));
    }
}

/******************************************************************************
 * _PmuSrcSet
 ******************************************************************************/
LOCAL void _PmuSrcSet (const char *SrcName)
{
    CHIP_REG_AND(CGM_PMU_CFG, ~CGM_PMU_SEL_MSK);

    if (strcmp ("CLK_32K", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_PMU_CFG, 0<<CGM_PMU_SEL_SHIFT);
    }
    else if (strcmp ("CLK_26M", SrcName) == 0)
    {
        CHIP_REG_OR(CGM_PMU_CFG, 1<<CGM_PMU_SEL_SHIFT);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_PmuSrcSet", SrcName, "CLK_32K, CLK_26M"));
    }
}

/******************************************************************************
 * _CcirSrcSet
 ******************************************************************************/
LOCAL void _CcirSrcSet (const char *SrcName)
{
	if (strcmp ("PAD_CLK", SrcName) == 0)
	{
		CHIP_REG_OR(CGM_CCIR_CFG, CGM_CCIR_PAD_SEL_MSK);
	}
	else
	{
	        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
	                     (err_string, "_CcirSrcSet", SrcName, "PAD_CLK"));
	}
}

/******************************************************************************
 * _SdSlv1LoopSrcSet
 ******************************************************************************/
LOCAL void _SdSlv1LoopSrcSet (const char *SrcName)
{
	if (strcmp ("PAD_CLK", SrcName) == 0)
	{
		CHIP_REG_OR(CGM_SD_SLV1_LOOP_CFG, CGM_SD_SLV1_LOOP_PAD_SEL_MSK);
	}
	else
	{
	        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
	                     (err_string, "_SdSlv1LoopSrcSet", SrcName, "PAD_CLK"));
	}
}

/******************************************************************************
 * _SdSlv2LoopSrcSet
 ******************************************************************************/
LOCAL void _SdSlv2LoopSrcSet (const char *SrcName)
{
	if (strcmp ("PAD_CLK", SrcName) == 0)
	{
		CHIP_REG_OR(CGM_SD_SLV2_LOOP_CFG, CGM_SD_SLV2_LOOP_PAD_SEL_MSK);
	}
	else
	{
	        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
	                     (err_string, "_SdSlv2LoopSrcSet", SrcName, "PAD_CLK"));
	}
}

/******************************************************************************
 * _FuncdmaSrcSet
 ******************************************************************************/
LOCAL void _FuncdmaSrcSet (const char *SrcName)
{
	if (strcmp ("PAD_CLK", SrcName) == 0)
	{
		CHIP_REG_OR(CGM_FUNCDMA_CFG, CGM_FUNCDMA_PAD_SEL_MSK);
	}
	else
	{
	        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
	                     (err_string, "_FuncdmaSrcSet", SrcName, "PAD_CLK"));
	}
}

/******************************************************************************
 * _FmAdcSrcSet
 ******************************************************************************/
LOCAL void _FmAdcSrcSet (const char *SrcName)
{
	if (strcmp ("PAD_CLK", SrcName) == 0)
	{
		CHIP_REG_OR(CGM_FM_ADC_CFG, CGM_FM_ADC_PAD_SEL_MSK);
	}
	else
	{
	        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
	                     (err_string, "_FmAdcSrcSet", SrcName, "PAD_CLK"));
	}
}

/******************************************************************************
 * _BtAdcSrcSet
 ******************************************************************************/
LOCAL void _BtAdcSrcSet (const char *SrcName)
{
	if (strcmp ("PAD_CLK", SrcName) == 0)
	{
		CHIP_REG_OR(CGM_BT_ADC_CFG, CGM_BT_ADC_PAD_SEL_MSK);
	}
	else
	{
	        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
	                     (err_string, "_BtAdcSrcSet", SrcName, "PAD_CLK"));
	}
}

/******************************************************************************
 * _BtDacSrcSet
 ******************************************************************************/
LOCAL void _BtDacSrcSet (const char *SrcName)
{
	if (strcmp ("PAD_CLK", SrcName) == 0)
	{
		CHIP_REG_OR(CGM_BT_DAC_CFG, CGM_BT_DAC_PAD_SEL_MSK);
	}
	else
	{
	        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
	                     (err_string, "_BtDacSrcSet", SrcName, "PAD_CLK"));
	}
}

/******************************************************************************
 * _BtRssiAdcAnaSrcSet
 ******************************************************************************/
LOCAL void _BtRssiAdcAnaSrcSet (const char *SrcName)
{
	if (strcmp ("PAD_CLK", SrcName) == 0)
	{
		CHIP_REG_OR(CGM_BT_RSSI_ADC_ANA_CFG, CGM_BT_RSSI_ADC_ANA_PAD_SEL_MSK);
	}
	else
	{
	        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
	                     (err_string, "_BtRssiAdcAnaSrcSet", SrcName, "PAD_CLK"));
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
 * _TimerEnable
 ******************************************************************************/
LOCAL void _TimerEnable (void)
{
    /* Enable timer apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_TimerEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_495_112_2_17_23_3_47_526,(uint8*)"");

    CHIP_REG_SET (APB_EB0_SET,  TMR_RTC_EB);
    CHIP_REG_SET (APB_EB0_SET,  TMR_EB);
}

/******************************************************************************
 * _TimerDisable
 ******************************************************************************/
LOCAL void _TimerDisable (void)
{
    /* Disable timer apb clock */
    //CLOCK_LOG_TRACE:"[_TimerDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_506_112_2_17_23_3_47_527,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  TMR_EB);
}

/******************************************************************************
 * _TimerReset
 ******************************************************************************/
LOCAL void _TimerReset (void)
{
    //CLOCK_LOG_TRACE:"[_TimerReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_515_112_2_17_23_3_47_528,(uint8*)"");

    CHIP_REG_SET (APB_RST0_SET,  TMR_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  TMR_SOFT_RST);
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

    CHIP_REG_SET (APB_EB0_SET,  TMRBK_RTC_EB);
    CHIP_REG_SET (APB_EB0_SET,  TMRBK_EB);
}

/******************************************************************************
 * _TimerbkDisable
 ******************************************************************************/
LOCAL void _TimerbkDisable (void)
{
    /* Disable timerbk apb clock */
    //CLOCK_LOG_TRACE:"[_TimerbkDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_540_112_2_17_23_3_47_530,(uint8*)"");

	CHIP_REG_SET (APB_EB0_CLR,  TMRBK_RTC_EB);
    CHIP_REG_SET (APB_EB0_CLR,  TMRBK_EB);
}

/******************************************************************************
 * _TimerbkReset
 ******************************************************************************/
LOCAL void _TimerbkReset (void)
{
    //CLOCK_LOG_TRACE:"[_TimerbkReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_549_112_2_17_23_3_47_531,(uint8*)"");

    CHIP_REG_SET (APB_RST0_SET,  TMRBK_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  TMRBK_SOFT_RST);
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

    CHIP_REG_SET (APB_EB0_SET,  KPD_RTC_EB);
    CHIP_REG_SET (APB_EB0_SET,  KPD_EB);
}

/******************************************************************************
 * _KpdDisable
 ******************************************************************************/
LOCAL void _KpdDisable (void)
{
    /* Disable keypad apb clock */
    //CLOCK_LOG_TRACE:"[_KpdDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_574_112_2_17_23_3_47_533,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  KPD_RTC_EB);
    CHIP_REG_SET (APB_EB0_CLR,  KPD_EB);
}

/******************************************************************************
 * _KpdReset
 ******************************************************************************/
LOCAL void _KpdReset (void)
{
    //CLOCK_LOG_TRACE:"[_KpdReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_584_112_2_17_23_3_47_534,(uint8*)"");

    /* Reset kpd clock */
    CHIP_REG_SET (APB_RST0_SET,  KPD_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  KPD_SOFT_RST);
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

    CHIP_REG_SET (APB_EB0_SET,  I2C_EB);
}

/******************************************************************************
 * _IIC0Disable
 ******************************************************************************/
LOCAL void _IIC0Disable (void)
{
    /* Enable keypad apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_IIC0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_609_112_2_17_23_3_47_536,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  I2C_EB);
}

/******************************************************************************
 * _IIC0Reset
 ******************************************************************************/
LOCAL void _IIC0Reset (void)
{
    //CLOCK_LOG_TRACE:"[_IIC0Reset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_618_112_2_17_23_3_47_537,(uint8*)"");

    /* Reset IIC clock */
    CHIP_REG_SET (APB_RST0_SET,  I2C_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  I2C_SOFT_RST);
    DELAY_CYCLE(1000)
}

/******************************************************************************
 * _IIC1Enable
 ******************************************************************************/
LOCAL void _IIC1Enable (void)
{
    /* Enable keypad apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_IIC1Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_633_112_2_17_23_3_47_538,(uint8*)"");

    CHIP_REG_SET (APB_EB0_SET,  I2C1_EB);
}

/******************************************************************************
 * _IIC1Disable
 ******************************************************************************/
LOCAL void _IIC1Disable (void)
{
    /* Enable keypad apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_IIC1Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_643_112_2_17_23_3_47_539,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  I2C1_EB);
}

/******************************************************************************
 * _IIC1Reset
 ******************************************************************************/
LOCAL void _IIC1Reset (void)
{
    //CLOCK_LOG_TRACE:"[_IIC1Reset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_652_112_2_17_23_3_47_540,(uint8*)"");

    /* Reset SPI1 clock */
    CHIP_REG_SET (APB_RST0_SET,  I2C1_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  I2C1_SOFT_RST);
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

    CHIP_REG_SET (APB_EB0_SET,  SYST_RTC_EB);
    CHIP_REG_SET (APB_EB0_SET,  SYST_EB);
}

/******************************************************************************
 * _SyscntDisable
 ******************************************************************************/
LOCAL void _SyscntDisable (void)
{
    /* Disable Syscnt apb clock */
    //CLOCK_LOG_TRACE:"[_SyscntDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_678_112_2_17_23_3_47_542,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  SYST_RTC_EB);
    CHIP_REG_SET (APB_EB0_CLR,  SYST_EB);
}

/******************************************************************************
 * _SyscntReset
 ******************************************************************************/
LOCAL void _SyscntReset (void)
{
    //CLOCK_LOG_TRACE:"[_SyscntReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_688_112_2_17_23_3_47_543,(uint8*)"");

    /* Reset system conter clock */
    CHIP_REG_SET (APB_RST0_SET,  SYST_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  SYST_SOFT_RST);
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

    CHIP_REG_SET (APB_EB0_SET,  ADI_EB);
}

/******************************************************************************
 * _AdiDisable
 ******************************************************************************/
LOCAL void _AdiDisable (void)
{
    /* Disable Adi apb clock */
    //CLOCK_LOG_TRACE:"[_AdiDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_713_112_2_17_23_3_47_545,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  ADI_EB);
}

/******************************************************************************
 * _AdiReset
 ******************************************************************************/
LOCAL void _AdiReset (void)
{
    //CLOCK_LOG_TRACE:"[_AdiReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_722_112_2_17_23_3_47_546,(uint8*)"");

    CHIP_REG_SET (APB_RST0_SET,  ADI_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  ADI_SOFT_RST);
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
 * _AdcEnable
 ******************************************************************************/
LOCAL void _AdcEnable (void)
{
    /* Enable Adc apb clock */
    //CLOCK_LOG_TRACE:"[_AdcEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_877_112_2_17_23_3_47_559,(uint8*)"");
    ANA_REG_OR (ANA_MODULE_EN0,  ANA_ADC_EN);

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
    CHIP_REG_SET (APB_EB0_SET,  GPIO_RTC_EB);
    CHIP_REG_SET (APB_EB0_SET,  GPIO_EB);
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
    CHIP_REG_SET (APB_EB0_CLR,  GPIO_RTC_EB);
    CHIP_REG_SET (APB_EB0_CLR,  GPIO_EB);
}

/******************************************************************************
 * _GpiodReset
 ******************************************************************************/
LOCAL void _GpiodReset (void)
{
    //CLOCK_LOG_TRACE:"[_GpioReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_940_112_2_17_23_3_47_564,(uint8*)"");

    /* Reset gpio in D-die */
    CHIP_REG_SET (APB_RST0_SET,  GPIO_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  GPIO_SOFT_RST);
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
    CHIP_REG_SET (APB_EB0_SET,  EIC_RTC_EB);
    CHIP_REG_SET (APB_EB0_SET,  EIC_EB);
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
    CHIP_REG_SET (APB_EB0_CLR,  EIC_RTC_EB);
    CHIP_REG_SET (APB_EB0_CLR,  EIC_EB);	
}

/******************************************************************************
 * _EicdReset
 ******************************************************************************/
LOCAL void _EicdReset (void)
{
    //CLOCK_LOG_TRACE:"[_EicdReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1014_112_2_17_23_3_48_570,(uint8*)"");

    /* Reset gpio in D-die */
    CHIP_REG_SET (APB_RST0_SET,  EIC_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  EIC_SOFT_RST);
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

    /* Reset gpio in A-die */
    ANA_REG_OR (ANA_SOFT_RST0,  ANA_EIC_SOFT_RST);
    DELAY_CYCLE(1000)

    ANA_REG_AND (ANA_SOFT_RST0,  ~ANA_EIC_SOFT_RST);
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

    CHIP_REG_SET (APB_EB0_SET,  SIM_EB);
}

/******************************************************************************
 * _Sim0Disable
 ******************************************************************************/
LOCAL void _Sim0Disable (void)
{
    /* Disable Sim0 apb clock */
    //CLOCK_LOG_TRACE:"[_Sim0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1077_112_2_17_23_3_48_575,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  SIM_EB);
}

/******************************************************************************
 * _Sim0Reset
 ******************************************************************************/
LOCAL void _Sim0Reset (void)
{
    //CLOCK_LOG_TRACE:"[_Sim0Reset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1086_112_2_17_23_3_48_576,(uint8*)"");

    /* Reset sim clock */
    CHIP_REG_SET (APB_RST0_SET,  SIM_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  SIM_SOFT_RST);
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

    CHIP_REG_SET (APB_EB0_SET,  GEA_EB);
}

/******************************************************************************
 * _GeaDisable
 ******************************************************************************/
LOCAL void _GeaDisable (void)
{
    /* Disable Gea apb clock */
    //CLOCK_LOG_TRACE:"[_GeaDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1111_112_2_17_23_3_48_578,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  GEA_EB);
}

/******************************************************************************
 * _GeaReset
 ******************************************************************************/
LOCAL void _GeaReset (void)
{
    //CLOCK_LOG_TRACE:"[_GeaReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1120_112_2_17_23_3_48_579,(uint8*)"");

    /* Reset GEA clock */
    CHIP_REG_SET (APB_RST0_SET,  GEA_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  GEA_SOFT_RST);
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

    CHIP_REG_SET(APB_EB0_SET,  EFUSE_EB);
}

/******************************************************************************
 * _EfuseDisable
 ******************************************************************************/
LOCAL void _EfuseDisable (void)
{
    /* Disable Efuse apb clock */
    //CLOCK_LOG_TRACE:"[_EfuseDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1145_112_2_17_23_3_48_581,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  EFUSE_EB);
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
    CHIP_REG_SET(AHB_EB0_SET, SPI0_EB);
}

/******************************************************************************
 * _Spi0Disable
 ******************************************************************************/
LOCAL void _Spi0Disable (void)
{
    /* Disable Spi0 apb clock */
    //CLOCK_LOG_TRACE:"[_SpiDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1166_112_2_17_23_3_48_583,(uint8*)"");

    CHIP_REG_SET(AHB_EB0_CLR, SPI0_EB);
}

/******************************************************************************
 * _Spi0Reset
 ******************************************************************************/
LOCAL void _Spi0Reset (void)
{
    //CLOCK_LOG_TRACE:"[_Spi0Reset]"

    /* Reset SPI0 clock */
	CHIP_REG_SET (AHB_RST0_SET,  SPI0_SOFT_RST);
    DELAY_CYCLE(1000)

	CHIP_REG_SET (AHB_RST0_CLR,  SPI0_SOFT_RST);
    DELAY_CYCLE(1000)
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
    CHIP_REG_SET (AHB_EB0_SET,  SPI1_EB_SET);
}

/******************************************************************************
 * _Spi1Disable
 ******************************************************************************/
LOCAL void _Spi1Disable (void)
{
    /* Disable Spi1 apb clock */
    //CLOCK_LOG_TRACE:"[_Spi1Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1185_112_2_17_23_3_48_585,(uint8*)"");

    //CHIP_REG_AND (APB_EB0,  ~SPI1_EB);
    CHIP_REG_SET (AHB_EB0_CLR,  SPI1_EB);
}

/******************************************************************************
 * _Spi1Reset
 ******************************************************************************/
LOCAL void _Spi1Reset (void)
{
    //CLOCK_LOG_TRACE:"[_Spi1Reset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1194_112_2_17_23_3_48_586,(uint8*)"");

    /* Reset SPI1 clock */
	CHIP_REG_SET (AHB_RST0_SET,  SPI1_SOFT_RST);
    DELAY_CYCLE(1000)

	CHIP_REG_SET (AHB_RST0_CLR,  SPI1_SOFT_RST);
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
	
    CHIP_REG_SET (APB_EB0_SET,  IIS_EB);
}	

/******************************************************************************
 * _I2s0Disable
 ******************************************************************************/
LOCAL void _I2s0Disable (void)
{
    /* Disable I2s0 apb clock */
    //CLOCK_LOG_TRACE:"[_I2s0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1219_112_2_17_23_3_48_588,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  IIS_EB);
}

/******************************************************************************
 * _I2sReset
 ******************************************************************************/
LOCAL void _I2sReset (void)
{
    //CLOCK_LOG_TRACE:"[_I2sReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1228_112_2_17_23_3_48_589,(uint8*)"");

    /* Reset I2s clock */
    CHIP_REG_SET (APB_RST0_SET,  IIS_SOFT_RST_MCU);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  IIS_SOFT_RST_MCU);
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

    CHIP_REG_SET (APB_EB0_SET,  UART0_EB);
}

/******************************************************************************
 * _Uart0Disable
 ******************************************************************************/
LOCAL void _Uart0Disable (void)
{
    /* Disable Uart0 apb clock */
    //CLOCK_LOG_TRACE:"[_Uart0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1253_112_2_17_23_3_48_591,(uint8*)"");
	
    CHIP_REG_SET (APB_EB0_CLR,  UART0_EB);
}

/******************************************************************************
 * _Uart0Reset
 ******************************************************************************/
LOCAL void _Uart0Reset (void)
{
    //CLOCK_LOG_TRACE:"[_Uart0Reset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1262_112_2_17_23_3_48_592,(uint8*)"");

    /* Reset uart0 clock */
    CHIP_REG_SET (APB_RST0_SET,  UART0_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  UART0_SOFT_RST);
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

    CHIP_REG_SET (APB_EB0_SET,  UART1_EB);
}

/******************************************************************************
 * _Uart1Disable
 ******************************************************************************/
LOCAL void _Uart1Disable (void)
{
    /* Disable Uart1 apb clock */
    //CLOCK_LOG_TRACE:"[_Uart1Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1287_112_2_17_23_3_48_594,(uint8*)"");
	
    CHIP_REG_SET (APB_EB0_CLR,  UART1_EB);
}

/******************************************************************************
 * _Uart1Reset
 ******************************************************************************/
LOCAL void _Uart1Reset (void)
{
    //CLOCK_LOG_TRACE:"[_Uart1Reset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1296_112_2_17_23_3_48_595,(uint8*)"");

    /* Reset uart1 clock */
    CHIP_REG_SET (APB_RST0_SET,  UART1_SOFT_RST_MCU);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (APB_RST0_CLR,  UART1_SOFT_RST_MCU);
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
	
    CHIP_REG_SET (APB_EB0_SET,  PWM_EB);
}

/******************************************************************************
 * _PwmDisable
 ******************************************************************************/
LOCAL void _PwmDisable (void)
{
    /* Disable Pwm apb clock */
    //CLOCK_LOG_TRACE:"[_PwmDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1321_112_2_17_23_3_48_597,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  PWM_EB);
}

/******************************************************************************
 * _PinEnable
 ******************************************************************************/
LOCAL void _PinEnable (void)
{
    /* Enable Pin apb clock */
    //CLOCK_LOG_TRACE:"[_PinEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1331_112_2_17_23_3_48_598,(uint8*)"");
    CHIP_REG_SET (APB_EB0_SET,  PIN_REG_EB);
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

    CHIP_REG_SET (APB_EB0_CLR,  PIN_REG_EB);
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
    CHIP_REG_OR (ANA_SOFT_RST0,  ANA_PWM_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_AND (ANA_SOFT_RST0,  ~ANA_PWM_SOFT_RST);
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

    CHIP_REG_SET (APB_EB0_SET,  CLK_AUX_EB);	
}

/******************************************************************************
 * _Aux0Disable
 ******************************************************************************/
LOCAL void _Aux0Disable (void)
{
    /* Disable Aux0 apb clock */
    //CLOCK_LOG_TRACE:"[_Aux0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1377_112_2_17_23_3_48_602,(uint8*)"");

    CHIP_REG_SET (APB_EB0_CLR,  CLK_AUX_EB);	
}

/******************************************************************************
 * _CcirReset
 ******************************************************************************/
LOCAL void _CcirReset (void)
{
    //CLOCK_LOG_TRACE:"[_CcirReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1386_112_2_17_23_3_48_603,(uint8*)"");

    /* Reset ccir clock */
    CHIP_REG_SET (AHB_RST0_SET,  CCIR_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (AHB_RST0_CLR,  CCIR_SOFT_RST);
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

    CHIP_REG_SET (AHB_EB0_SET,  DCAM_EB);
}

/******************************************************************************
 * _DcamDisable
 ******************************************************************************/
LOCAL void _DcamDisable (void)
{
    /* Disable Dcam apb clock */
    //CLOCK_LOG_TRACE:"[_DcamDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1411_112_2_17_23_3_48_605,(uint8*)"");

    CHIP_REG_SET (AHB_EB0_CLR,  DCAM_EB);
}

/******************************************************************************
 * _DcamReset
 ******************************************************************************/
LOCAL void _DcamReset (void)
{
    //CLOCK_LOG_TRACE:"[_DcamReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1420_112_2_17_23_3_48_606,(uint8*)"");

    /* Reset DCAM device */
    CHIP_REG_SET (AHB_RST0_SET,  DCAM_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET (AHB_RST0_CLR,  DCAM_SOFT_RST);
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
	
    CHIP_REG_SET(AHB_EB0_SET, ZIP0_EB);
}

/******************************************************************************
 * _LzmaDisable
 ******************************************************************************/
LOCAL void _LzmaDisable (void)
{
    /* Disable Sdio apb clock */
    //CLOCK_LOG_TRACE:"[_LzmaDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1445_112_2_17_23_3_48_608,(uint8*)"");
      
    CHIP_REG_SET (AHB_EB0_CLR,  ZIP0_EB);
}

/******************************************************************************
 * _LzmaReset
 ******************************************************************************/
LOCAL void _LzmaReset (void)
{
    //CLOCK_LOG_TRACE:"[_LzmaReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1454_112_2_17_23_3_48_609,(uint8*)"");
   
    CHIP_REG_SET(AHB_RST0_SET, ZIP0_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET(AHB_RST0_CLR, ZIP0_SOFT_RST);
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
	
    CHIP_REG_SET(AHB_EB0_SET, LCM_EB);
}

/******************************************************************************
 * _LcmDisable
 ******************************************************************************/
LOCAL void _LcmDisable (void)
{
    /* Disable lcm ahb clock */
    //CLOCK_LOG_TRACE:"[_LcmDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1478_112_2_17_23_3_48_611,(uint8*)"");
      
    CHIP_REG_SET (AHB_EB0_CLR,  LCM_EB);
}

/******************************************************************************
 * _LcmReset
 ******************************************************************************/
LOCAL void _LcmReset (void)
{
    //CLOCK_LOG_TRACE:"[_LcmReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1487_112_2_17_23_3_48_612,(uint8*)"");
   #if 0
    CHIP_REG_OR(AHB_RST0, LCM_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_AND(AHB_RST0, ~LCM_SOFT_RST);
    DELAY_CYCLE(1000)
    #else
    CHIP_REG_SET(AHB_RST0_SET, LCM_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET(AHB_RST0_CLR, LCM_SOFT_RST);
    DELAY_CYCLE(1000)
    #endif
}

/******************************************************************************
 * _LcdcEnable
 ******************************************************************************/
LOCAL void _LcdcEnable (void)
{
    /* Enable lcdc ahb clock */
    //CLOCK_LOG_TRACE:"[_LcdcEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1501_112_2_17_23_3_49_613,(uint8*)"");
	
    CHIP_REG_SET(AHB_EB0_SET, LCDC_EB);
}

/******************************************************************************
 * _LcdcDisable
 ******************************************************************************/
LOCAL void _LcdcDisable (void)
{
    /* Disable lcdc ahb clock */
    //CLOCK_LOG_TRACE:"[_LcdcDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1511_112_2_17_23_3_49_614,(uint8*)"");
      
    CHIP_REG_SET (AHB_EB0_CLR,  LCDC_EB);
}

/******************************************************************************
 * _LcdcReset
 ******************************************************************************/
LOCAL void _LcdcReset (void)
{
    //CLOCK_LOG_TRACE:"[_LcdcReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1520_112_2_17_23_3_49_615,(uint8*)"");
   
    CHIP_REG_SET(AHB_RST0_SET, LCDC_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET(AHB_RST0_CLR, LCDC_SOFT_RST);
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
	
    CHIP_REG_SET(AHB_EB0_SET, ROTA_EB);
}

/******************************************************************************
 * _RotaDisable
 ******************************************************************************/
LOCAL void _RotaDisable (void)
{
    /* Disable rota ahb clock */
    //CLOCK_LOG_TRACE:"[_RotaDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1544_112_2_17_23_3_49_617,(uint8*)"");
      
    CHIP_REG_SET (AHB_EB0_CLR,  ROTA_EB);
}

/******************************************************************************
 * _RotaReset
 ******************************************************************************/
LOCAL void _RotaReset (void)
{
    //CLOCK_LOG_TRACE:"[_RotaReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1553_112_2_17_23_3_49_618,(uint8*)"");
   
    CHIP_REG_SET(AHB_RST0_SET, ROTA_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET(AHB_RST0_CLR, ROTA_SOFT_RST);
    DELAY_CYCLE(1000)   
}

/******************************************************************************
 * _SfcEnable
 ******************************************************************************/
LOCAL void _SfcEnable (void)
{
    /* Enable sfc ahb clock */
    //CLOCK_LOG_TRACE:"[_SfcEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1620_112_2_17_23_3_49_624,(uint8*)"");
	
    CHIP_REG_SET(AHB_EB0_SET, SFC_EB);
}

/******************************************************************************
 * _SfcDisable
 ******************************************************************************/
LOCAL void _SfcDisable (void)
{
    /* Disable sfc ahb clock */
    //CLOCK_LOG_TRACE:"[_SfcDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1630_112_2_17_23_3_49_625,(uint8*)"");
      
    CHIP_REG_SET (AHB_EB0_CLR,  SFC_EB);
}

/******************************************************************************
 * _SfcReset
 ******************************************************************************/
LOCAL void _SfcReset (void)
{
    //CLOCK_LOG_TRACE:"[_SfcReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1639_112_2_17_23_3_49_626,(uint8*)"");
   
    CHIP_REG_SET(AHB_RST0_SET, SFC_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET(AHB_RST0_CLR, SFC_SOFT_RST);
    DELAY_CYCLE(1000)   
}

/******************************************************************************
 * _VspReset
 ******************************************************************************/
LOCAL void _VspReset (void)
{
    //CLOCK_LOG_TRACE:"[_VspReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1652_112_2_17_23_3_49_627,(uint8*)"");
   
    CHIP_REG_SET(AHB_RST0_SET, VSP_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET(AHB_RST0_CLR, VSP_SOFT_RST);
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
	
    CHIP_REG_SET(AHB_EB0_SET, SDIO_EB);
}

/******************************************************************************
 * _Sdio0Disable
 ******************************************************************************/
LOCAL void _Sdio0Disable (void)
{
    /* Disable Sdio apb clock */
    //CLOCK_LOG_TRACE:"[_SdioDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1676_112_2_17_23_3_49_629,(uint8*)"");
      
    CHIP_REG_SET (AHB_EB0_CLR,  SDIO_EB);
}

/******************************************************************************
 * _SdioReset
 ******************************************************************************/
LOCAL void _SdioReset (void)
{
    //CLOCK_LOG_TRACE:"[_SdioReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1685_112_2_17_23_3_49_630,(uint8*)"");
   
    CHIP_REG_SET(AHB_RST0_SET, SDIO_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET(AHB_RST0_CLR, SDIO_SOFT_RST);
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

    CHIP_REG_SET(AHB_EB0_SET, USBD_EB);
}

/******************************************************************************
 * _UsbDisable
 ******************************************************************************/
LOCAL void _UsbDisable (void)
{
    /* Disable Usb apb clock */
    //CLOCK_LOG_TRACE:"[_UsbDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1709_112_2_17_23_3_49_632,(uint8*)"");

    CHIP_REG_SET(AHB_EB0_CLR, USBD_EB);
}

/******************************************************************************
 * _UsbReset
 ******************************************************************************/
LOCAL void _UsbReset (void)
{
    //CLOCK_LOG_TRACE:"[_UsbReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1718_112_2_17_23_3_49_633,(uint8*)"");

    CHIP_REG_SET(AHB_RST0_SET, USBD_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET(AHB_RST0_CLR, USBD_SOFT_RST);
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

    CHIP_REG_SET (AHB_EB0_SET,  DMA_EB);
}

/******************************************************************************
 * _DmaDisable
 ******************************************************************************/
LOCAL void _DmaDisable (void)
{
    /* Disable Dma apb clock */
    //CLOCK_LOG_TRACE:"[_DmaDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1742_112_2_17_23_3_49_635,(uint8*)"");

    CHIP_REG_SET (AHB_EB0_CLR,  DMA_EB);
}

/******************************************************************************
 * _DmaReset
 ******************************************************************************/
LOCAL void _DmaReset (void)
{
    //CLOCK_LOG_TRACE:"[_DmaReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1751_112_2_17_23_3_49_636,(uint8*)"");

    CHIP_REG_SET(AHB_RST0_SET, DMA_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET(AHB_RST0_CLR, DMA_SOFT_RST);
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

    CHIP_REG_SET (AHB_EB0_SET,  BUSMON0_EB);
}

/******************************************************************************
 * _Bm0Disable
 ******************************************************************************/
LOCAL void _Bm0Disable (void)
{
    /* Disable Bm0 apb clock */
    //CLOCK_LOG_TRACE:"[_Bm0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1775_112_2_17_23_3_49_638,(uint8*)"");

    CHIP_REG_SET (AHB_EB0_CLR,  BUSMON0_EB);
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

    CHIP_REG_SET (AHB_EB0_SET,  EMC_EB);	
    return;
}

/******************************************************************************
 * _EmcDisable
 ******************************************************************************/
LOCAL void _EmcDisable (void)
{
    //CLOCK_LOG_TRACE:"[_AhbDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1870_112_2_17_23_3_49_648,(uint8*)"");

    CHIP_REG_SET (AHB_EB0_CLR,  EMC_EB);	
    return;
}

/******************************************************************************
 * _EmcReset
 ******************************************************************************/
LOCAL void _EmcReset (void)
{
    //CLOCK_LOG_TRACE:"[_EmcReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1880_112_2_17_23_3_49_649,(uint8*)"");

    CHIP_REG_SET(AHB_RST0_SET, EMC_SOFT_RST);
    DELAY_CYCLE(1000)

    CHIP_REG_SET(AHB_RST0_CLR, EMC_SOFT_RST);
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
    CHIP_REG_SET (APB_EB0_SET,  AUD_EB);

}

/******************************************************************************
 * _AudioDisable
 ******************************************************************************/
LOCAL void _AudioDisable (void)
{
    /* Disable rtc apb clock */
    //CLOCK_LOG_TRACE:"[_AudioDisable] IS NULL"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6530_CLOCK_CFG_1906_112_2_17_23_3_49_651,(uint8*)"");
    CHIP_REG_SET(APB_EB0_CLR,  AUD_EB);

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
	CHIP_REG_SET (APB_EB0_SET,  AUD_EB);
	ANA_REG_OR (ANA_ARM_CLK_EN0,  ANA_CLK_AUD_IF_EN);
}

/******************************************************************************
* _AUDIFDisable
******************************************************************************/
LOCAL void _AudIfDisable()
{
	CHIP_REG_SET(APB_EB0_CLR,  AUD_EB);
	ANA_REG_AND (ANA_ARM_CLK_EN0,  ~ANA_CLK_AUD_IF_EN);
}
/******************************************************************************
* _AUDIFReset
******************************************************************************/
LOCAL void _AudIfReset()
{
	ANA_REG_OR (ANA_SOFT_RST0, ANA_AUD_IF_SOFT_RST);
	DELAY_CYCLE(1000)

	ANA_REG_AND (ANA_SOFT_RST0, ~ANA_AUD_IF_SOFT_RST);
	DELAY_CYCLE(1000)
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

