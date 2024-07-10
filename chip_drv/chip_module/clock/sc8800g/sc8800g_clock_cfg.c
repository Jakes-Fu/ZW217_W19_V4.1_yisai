/******************************************************************************
 ** File Name:        Application.c                                           *
 ** Author:         Yi.Qiu                                                  *
 ** DATE:           02/21/2010                                              *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.       *
 ** Description:                                                             *
 ******************************************************************************

 ******************************************************************************
 **                 Edit History                                               *
 ** ------------------------------------------------------------------------*
 ** DATE               NAME             DESCRIPTION                            *
 ** 02/21/2010      Yi.Qiu             Create.                                 *
 ******************************************************************************/
#include "chip_drv_trc.h"
#include "os_api.h"
#include "chip_plf_export.h"
//#include "../../analog/v3/analog_reg_v3.h"
#include "../clock.h"
#include "Ref_outport.h"

static const char *err_string = "%s choosed a wrong source [%s]\r\n please choose one of: [%s]\r\n";
/******************************************************************************
 * _McuSrcSet
 ******************************************************************************/
LOCAL void _McuSrcSet (const char *SrcName)
{
    CHIP_REG_AND (AHB_ARM_CLK,  ~ (BIT_24|BIT_23));

    if (strcmp ("MPLL", SrcName) == 0)
    {
    }
    else if (strcmp ("TDPLLDIV3", SrcName) == 0)
    {
        CHIP_REG_OR (AHB_ARM_CLK, (BIT_23));
    }
    else if (strcmp ("TDPLLDIV4", SrcName) == 0)
    {
        CHIP_REG_OR (AHB_ARM_CLK, (BIT_24));
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR (AHB_ARM_CLK, (BIT_24|BIT_23));
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_McuSrcSet", SrcName, "MPLL, TDPLLDIV3, TDPLLDIV4, 26M_XTL"));
    }
}

/******************************************************************************
 * _AhbSrcDivSet
 ******************************************************************************/
LOCAL void _AhbSrcDivSet (int Div)
{
    SCI_PASSERT ( (0x07 >= Div), ("_AhbSrcDivSet exceeds the max_value."));/*assert verified*/

    CHIP_REG_AND (AHB_ARM_CLK,  ~ (BIT_6|BIT_5|BIT_4));
    CHIP_REG_OR (AHB_ARM_CLK, (Div<<4));
}
/******************************************************************************
 * _DcamSrcSet
 ******************************************************************************/
LOCAL void _DcamSrcSet (const char *SrcName)
{
    CHIP_REG_AND (GR_PLL_SCR,  ~ (BIT_5|BIT_4));

    if (strcmp ("96M", SrcName) == 0)
    {
    }
    else if (strcmp ("64M", SrcName) == 0)
    {
        CHIP_REG_OR (GR_PLL_SCR, (BIT_4));
    }
    else if (strcmp ("48M", SrcName) == 0)
    {
        CHIP_REG_OR (GR_PLL_SCR, (BIT_5));
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR (GR_PLL_SCR, (BIT_5|BIT_4));
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_CcirSrcSet", "96M, 64M, 48M, 26M_XTL"));
    }
}

/******************************************************************************
 * _DcamSrcDivSet
 ******************************************************************************/
LOCAL void _DcamSrcDivSet (int Div)
{
    SCI_PASSERT ( (3 >= Div), ("_CcirSrcDivSet : without clock divide."));/*assert verified*/
}
/******************************************************************************
 * _DcamEnable
 ******************************************************************************/
LOCAL void _DcamEnable (void)
{
    /* Enable dcam ahb clock */
    //CLOCK_LOG_TRACE:"[_DcamEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_97_112_2_17_23_3_59_808,(uint8*)"");

    //enable
    CHIP_REG_OR (AHB_CTL0,  BIT_1);
}

/******************************************************************************
 * _DcamDisable
 ******************************************************************************/
LOCAL void _DcamDisable (void)
{
    /* Disable dcam ahb clock */
    //CLOCK_LOG_TRACE:"[_DcamDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_108_112_2_17_23_3_59_809,(uint8*)"");

    //enable
    CHIP_REG_AND (AHB_CTL0,  ~BIT_1);
}

/******************************************************************************
 * _VspSrcSet
 ******************************************************************************/
LOCAL void _VspSrcSet (const char *SrcName)
{
    CHIP_REG_AND (GR_PLL_SCR,  ~ (BIT_3|BIT_2));

    if (strcmp ("96M", SrcName) == 0)
    {
    }
    else if (strcmp ("64M", SrcName) == 0)
    {
        CHIP_REG_OR (GR_PLL_SCR, (BIT_2));
    }
    else if (strcmp ("48M", SrcName) == 0)
    {
        CHIP_REG_OR (GR_PLL_SCR, (BIT_3));
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR (GR_PLL_SCR, (BIT_3|BIT_2));
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_VspSrcSet", "96M, 64M, 48M, 26M_XTL"));
    }
}

/******************************************************************************
 * _VspSrcDivSet
 ******************************************************************************/
LOCAL void _VspSrcDivSet (int Div)
{
    SCI_PASSERT ( (3 >= Div), ("_VspSrcDivSet : without clock divide."));/*assert verified*/
}
/******************************************************************************
 * _VspEnable
 ******************************************************************************/
LOCAL void _VspEnable (void)
{
    /* Enable VSP ahb clock */
    //CLOCK_LOG_TRACE:"[_VspEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_153_112_2_17_23_3_59_810,(uint8*)"");

    //enable
    CHIP_REG_OR (AHB_CTL0,  BIT_13);
}
/******************************************************************************
 * _VspDisable
 ******************************************************************************/
LOCAL void _VspDisable (void)
{
    /* Disable vsp ahb clock */
    //CLOCK_LOG_TRACE:"[_VspDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_163_112_2_17_23_3_59_811,(uint8*)"");

    //enable
    CHIP_REG_AND (AHB_CTL0,  ~BIT_13);
}
/******************************************************************************
 * _VspReset
 ******************************************************************************/
LOCAL void _VspReset (void)
{
    AHB_SOFT_RST_U *ahb_rst = (AHB_SOFT_RST_U *) (AHB_SOFT_RST);

    //CLOCK_LOG_TRACE:"[_VspReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_174_112_2_17_23_4_0_812,(uint8*)"");

    /* Enable DMA clock */
    ahb_rst->mBits.vsp_rst = SCI_FALSE;
    /* Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.vsp_rst = SCI_TRUE;
    /*Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.vsp_rst = SCI_FALSE;
}

/******************************************************************************
 * _LcdcSrcSet
 ******************************************************************************/
LOCAL void _LcdcSrcSet (const char *SrcName)
{
    CHIP_REG_AND (GR_PLL_SCR,  ~ (BIT_7|BIT_6));

    if (strcmp ("96M", SrcName) == 0)
    {
    }
    else if (strcmp ("64M", SrcName) == 0)
    {
        CHIP_REG_OR (GR_PLL_SCR, (BIT_6));
    }
    else if (strcmp ("12M", SrcName) == 0)
    {
        CHIP_REG_OR (GR_PLL_SCR, (BIT_7));
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR (GR_PLL_SCR, (BIT_7|BIT_6));
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_LcdcSrcSet", "96M, 64M, 12M, 26M_XTL"));
    }
}

/******************************************************************************
 * _LcdcSrcDivSet
 ******************************************************************************/
LOCAL void _LcdcSrcDivSet (int Div)
{
    SCI_PASSERT ( (7 >= Div), ("_LcdcSrcDivSet exceeds the max_value."));/*assert verified*/

    CHIP_REG_AND (GR_GEN4,  ~ (BIT_2|BIT_1|BIT_0));
    CHIP_REG_OR (GR_GEN4, (Div<<0));
}
/******************************************************************************
 * _LcdcEnable
 ******************************************************************************/
LOCAL void _LcdcEnable (void)
{
    // Enable lcdc ahb clock
    //CLOCK_LOG_TRACE:"[_LcdcEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_228_112_2_17_23_4_0_813,(uint8*)"");

    //enable
    CHIP_REG_OR (AHB_CTL0,  BIT_3);
}

/******************************************************************************
 * _LcdcDisable
 ******************************************************************************/
LOCAL void _LcdcDisable (void)
{
    // Disable lcdc ahb clock
    //CLOCK_LOG_TRACE:"[_LcdcDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_239_112_2_17_23_4_0_814,(uint8*)"");

    //disable
    CHIP_REG_AND (AHB_CTL0,  ~BIT_3);
}

/******************************************************************************
 * _LcdcReset
 ******************************************************************************/
LOCAL void _LcdcReset (void)
{
    AHB_SOFT_RST_U *ahb_rst = (AHB_SOFT_RST_U *) (AHB_SOFT_RST);

    //CLOCK_LOG_TRACE:"[_LcdcReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_251_112_2_17_23_4_0_815,(uint8*)"");

    /* Enable DMA clock */
    ahb_rst->mBits.lcdc_rst = SCI_FALSE;
    /* Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.lcdc_rst = SCI_TRUE;
    /*Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.lcdc_rst = SCI_FALSE;
}

/******************************************************************************
 * _SdioSrcSet
 ******************************************************************************/
LOCAL void _SdioSrcSet (const char *SrcName)
{
    CHIP_REG_AND (GR_CLK_GEN5,  ~ (BIT_18|BIT_17));

    if (strcmp ("96M", SrcName) == 0)
    {
    }
    else if (strcmp ("64M", SrcName) == 0)
    {
        CHIP_REG_OR (GR_CLK_GEN5, (BIT_17));
    }
    else if (strcmp ("48M", SrcName) == 0)
    {
        CHIP_REG_OR (GR_CLK_GEN5, (BIT_18));
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR (GR_CLK_GEN5, (BIT_18|BIT_17));
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_SdioSrcSet", "96M, 64M, 48M, 26M_XTL"));
    }
}

/******************************************************************************
 * _SdioSrcDivSet
 ******************************************************************************/
LOCAL void _SdioSrcDivSet (int Div)
{
    SCI_PASSERT ( (3 >= Div), ("_SdioSrcDivSet : without clock divide."));/*assert verified*/
}
/******************************************************************************
 * _SdioEnable
 ******************************************************************************/
LOCAL void _SdioEnable (void)
{
    /* Enable _Sdio ahb clock */
    //CLOCK_LOG_TRACE:"[_SdioEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_302_112_2_17_23_4_0_816,(uint8*)"");

    //enable
    CHIP_REG_OR (AHB_CTL0,  BIT_4);
}

/******************************************************************************
 * _SdioDisable
 ******************************************************************************/
LOCAL void _SdioDisable (void)
{
    /* Disable _Sdio ahb clock */
    //CLOCK_LOG_TRACE:"[_SdioDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_313_112_2_17_23_4_0_817,(uint8*)"");

    //disable
    CHIP_REG_AND (AHB_CTL0,  ~BIT_4);
}
/******************************************************************************
 * _SdioReset
 ******************************************************************************/
/*LOCAL void _SdioReset (void)
{
    AHB_SOFT_RST_U *ahb_rst = (AHB_SOFT_RST_U *) (AHB_SOFT_RST);

    //CLOCK_LOG_TRACE:"[_SdioReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_323_112_2_17_23_4_0_818,(uint8*)"");

    // Enable DMA clock
    ahb_rst->mBits.lcdc_rst = SCI_FALSE;
    // Delay //
    OS_TickDelay (10);
    ahb_rst->mBits.lcdc_rst = SCI_TRUE;
    //Delay //
    OS_TickDelay (10);
    ahb_rst->mBits.lcdc_rst = SCI_FALSE;
}
*/
LOCAL void _SdioReset (void)
{
    CHIP_REG_OR(AHB_SOFT_RST, AHB_SDIO_SOFT_RST);
    CHIP_REG_AND(AHB_SOFT_RST, ~AHB_SDIO_SOFT_RST);
}

/******************************************************************************
 * _SpiSrcSet
 ******************************************************************************/
LOCAL void _SpiSrcSet (const char *SrcName)
{
    CHIP_REG_AND (GR_CLK_DLY,  ~ (BIT_27|BIT_26));

    if (strcmp ("TDPLLDIV4", SrcName) == 0)
    {
    }
    else if (strcmp ("TDPLLDIV5", SrcName) == 0)
    {
        CHIP_REG_OR (GR_CLK_DLY, (BIT_26));
    }
    else if (strcmp ("96M", SrcName) == 0)
    {
        CHIP_REG_OR (GR_CLK_DLY, (BIT_27));
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR (GR_CLK_DLY, (BIT_27|BIT_26));
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_SpiSrcSet", "TDPLLDIV5, TDPLLDIV5, 96M, 26M_XTL"));
    }
}

/******************************************************************************
 * _SpiSrcDivSet
 ******************************************************************************/
LOCAL void _SpiSrcDivSet (int Div)
{
    SCI_PASSERT ( (7 >= Div), ("_SpiSrcDivSet exceeds the max_value."));/*assert verified*/

    CHIP_REG_AND (GR_GEN2,  ~ (BIT_23|BIT_22|BIT_21));
    CHIP_REG_OR (GR_GEN2, (Div<<21));
}
/******************************************************************************
 * _SpiEnable
 ******************************************************************************/
LOCAL void _SpiEnable (void)
{
    /* Enable SPI apb clock */
    //CLOCK_LOG_TRACE:"[_SpiEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_383_112_2_17_23_4_0_819,(uint8*)"");

    CHIP_REG_OR (GR_GEN0, BIT_17);
}
/******************************************************************************
 * _SpiDisable
 ******************************************************************************/
LOCAL void _SpiDisable (void)
{
    /* Disable SPI apb clock */
    //CLOCK_LOG_TRACE:"[_SpiDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_392_112_2_17_23_4_0_820,(uint8*)"");

    CHIP_REG_AND (GR_GEN0, ~BIT_17);
}

/******************************************************************************
 * _I2sSrcSet
 ******************************************************************************/
LOCAL void _I2sSrcSet (const char *SrcName)
{
    CHIP_REG_AND (GR_PLL_SCR,  ~ (BIT_9|BIT_8));

    if (strcmp ("128M", SrcName) == 0)
    {
    }
    else if (strcmp ("51M2", SrcName) == 0)
    {
        CHIP_REG_OR (GR_PLL_SCR, (BIT_8));
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR (GR_PLL_SCR, (BIT_9|BIT_8));
    }
    else
    {
        //from pad 2'B10
        CHIP_REG_OR (GR_PLL_SCR, (BIT_9));
    }
}

/******************************************************************************
 * _I2sSrcDivSet
 ******************************************************************************/
LOCAL void _I2sSrcDivSet (int Div)
{
    SCI_PASSERT ( (0x3F >= Div), ("_I2sSrcDivSet exceeds the max_value."));/*assert verified*/

    CHIP_REG_AND (GR_GEN2,  ~ (BIT_29|BIT_28|BIT_27|BIT_26|BIT_25|BIT_24));
    CHIP_REG_OR (GR_GEN2, (Div<<24));
}
/******************************************************************************
 * _I2sEnable
 ******************************************************************************/
LOCAL void _I2sEnable (void)
{
    // arm control:0
    CHIP_REG_AND (GR_PCTL, ~MISC0_IIS_MUX_SEL);

    /* Enable i2s apb clock */
    //CLOCK_LOG_TRACE:"[_I2sEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_438_112_2_17_23_4_0_821,(uint8*)"");

    CHIP_REG_OR (GR_GEN0, GEN0_I2S_EN);
}
/******************************************************************************
 * _I2sDisable
 ******************************************************************************/
LOCAL void _I2sDisable (void)
{
    /* Disable i2s apb clock */
    //CLOCK_LOG_TRACE:"[_I2sDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_447_112_2_17_23_4_0_822,(uint8*)"");

    CHIP_REG_AND (GR_GEN0, ~GEN0_I2S_EN);
}

/******************************************************************************
 * _TpcEnable
 ******************************************************************************/
LOCAL void _TpcEnable (void)
{
    /* Enable Tpc apb clock */
    //CLOCK_LOG_TRACE:"[_TpcEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_457_112_2_17_23_4_0_823,(uint8*)"");

    //Set SD1_D4-D6 for TPC Function
    ANA_REG_OR (ANA_AGEN, AGEN_TPC_EN | AGEN_RTC_TPC_EN);
}

/******************************************************************************
 * _TpcDisable
 ******************************************************************************/
LOCAL void _TpcDisable (void)
{
    /* Disable Tpc apb clock */
    //CLOCK_LOG_TRACE:"[_TpcDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_468_112_2_17_23_4_0_824,(uint8*)"");

    ANA_REG_AND (ANA_AGEN,  ~AGEN_TPC_EN);
}

/******************************************************************************
 * _RtcEnable
 ******************************************************************************/
LOCAL void _RtcEnable (void)
{
    //CLOCK_LOG_TRACE:"[_RtcEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_477_112_2_17_23_4_0_825,(uint8*)"");

    // Enable RTC device.
    ANA_REG_OR (ANA_AGEN, AGEN_RTC_EN | AGEN_RTC_RTC_EN);
}

/******************************************************************************
 * _RtcDisable
 ******************************************************************************/
LOCAL void _RtcDisable (void)
{
    //CLOCK_LOG_TRACE:"[_RtcDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_487_112_2_17_23_4_0_826,(uint8*)"");

    // Enable RTC device.
    ANA_REG_AND (ANA_AGEN, AGEN_RTC_EN);
}

/******************************************************************************
 * _RtcTimer0Enable
 ******************************************************************************/
LOCAL void _RtcTimer0Enable (void)
{
    //CLOCK_LOG_TRACE:"[_RtcTimer0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_497_112_2_17_23_4_0_827,(uint8*)"");

    CHIP_REG_OR (GR_GEN0,  GEN0_TIMER_EN | GEN0_TMR_RTC_EN);
}

/******************************************************************************
 * _RtcTimer0Disable
 ******************************************************************************/
LOCAL void _RtcTimer0Disable (void)
{
    //CLOCK_LOG_TRACE:"[_RtcTimer0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_506_112_2_17_23_4_0_828,(uint8*)"");

    CHIP_REG_AND (GR_GEN0,  ~GEN0_TIMER_EN);

}

/******************************************************************************
 * _AdcEnable
 ******************************************************************************/
LOCAL void _AdcEnable (void)
{
    /* Enable Adc apb clock */
    //CLOCK_LOG_TRACE:"[_AdcEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_517_112_2_17_23_4_0_829,(uint8*)"");

    ANA_REG_OR (ANA_AGEN, AGEN_ADC_EN);
    ANA_REG_OR (ANA_CLK_CTL, ACLK_CTL_AUXAD_EN | ACLK_CTL_AUXADC_EN);
}

/******************************************************************************
 * _AdcDisable
 ******************************************************************************/
LOCAL void _AdcDisable (void)
{
    /* Disable Adc apb clock */
    //CLOCK_LOG_TRACE:"[_AdcDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_528_112_2_17_23_4_0_830,(uint8*)"");

    ANA_REG_AND (ANA_AGEN,  ~AGEN_ADC_EN);
}

/******************************************************************************
 * _IIC0Enable
 ******************************************************************************/
LOCAL void _I2C0Enable (void)
{
    /* Enable keypad apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_I2C0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_538_112_2_17_23_4_0_831,(uint8*)"");

    CHIP_REG_OR (GR_GEN0, GEN0_I2C_EN);

    CHIP_REG_OR (GR_CLK_DLY, BIT_3);
}

/******************************************************************************
 * _I2C0Disable
 ******************************************************************************/
LOCAL void _I2C0Disable (void)
{
    /* Enable keypad apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_I2C0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_550_112_2_17_23_4_0_832,(uint8*)"");

    CHIP_REG_AND (GR_GEN0,  ~GEN0_I2C_EN);
}

/******************************************************************************
 * _KpdEnable
 ******************************************************************************/
LOCAL void _KpdEnable (void)
{
    /* Disable keypad apb clock */
    //CLOCK_LOG_TRACE:"[_KpdDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_560_112_2_17_23_4_0_833,(uint8*)"");

    CHIP_REG_OR (GR_GEN0,  GEN0_KPD_EN | GEN0_KPD_RTC_EN);
}

/******************************************************************************
 * _KpdDisable
 ******************************************************************************/
LOCAL void _KpdDisable (void)
{
    /* Disable keypad apb clock */
    //CLOCK_LOG_TRACE:"[_KpdDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_570_112_2_17_23_4_0_834,(uint8*)"");

    CHIP_REG_AND (GR_GEN0,  ~GEN0_KPD_EN);
}

/******************************************************************************
 * _SyscntEnable
 ******************************************************************************/
LOCAL void _SyscntEnable (void)
{
    /* Enable Syscnt apb clock */
    //CLOCK_LOG_TRACE:"[_SyscntEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_580_112_2_17_23_4_0_835,(uint8*)"");

    CHIP_REG_OR (GR_GEN0,  GEN0_SYST_EN | GEN0_SYST_RTC_EN);
}

/******************************************************************************
 * _SyscntDisable
 ******************************************************************************/
LOCAL void _SyscntDisable (void)
{
    /* Disable Syscnt apb clock */
    //CLOCK_LOG_TRACE:"[_SyscntDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_590_112_2_17_23_4_0_836,(uint8*)"");

    CHIP_REG_AND (GR_GEN0,  ~GEN0_SYST_EN);
}

/******************************************************************************
 * _Uart0SrcSet
 ******************************************************************************/
LOCAL void _Uart0SrcSet (const char *SrcName)
{
    CHIP_REG_AND (GR_CLK_DLY,  ~ (BIT_21|BIT_20));

    if (strcmp ("96M", SrcName) == 0)
    {
    }
    else if (strcmp ("51M2", SrcName) == 0)
    {
        CHIP_REG_OR (GR_CLK_DLY, (BIT_20));
    }
    else if (strcmp ("48M", SrcName) == 0)
    {
        CHIP_REG_OR (GR_CLK_DLY, (BIT_21));
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR (GR_CLK_DLY, (BIT_21|BIT_20));
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Uart0SrcSet", "96M, 64M, 48M, 26M_XTL"));
    }
}

/******************************************************************************
 * _Uart0SrcDivSet
 ******************************************************************************/
LOCAL void _Uart0SrcDivSet (int Div)
{
    SCI_PASSERT ( (7 >= Div), ("_Uart0SrcDivSet exceeds the max_value."));/*assert verified*/

    CHIP_REG_AND (GR_CLK_GEN5,  ~ (BIT_2|BIT_1|BIT_0));
    CHIP_REG_OR (GR_CLK_GEN5, (Div<<0));
}
/******************************************************************************
 * _Uart0Enable
 ******************************************************************************/
LOCAL void _Uart0Enable (void)
{
    /* Enable UART0 apb clock */
    //CLOCK_LOG_TRACE:"[_Uart0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_637_112_2_17_23_4_0_837,(uint8*)"");

    CHIP_REG_OR (GR_GEN0, BIT_20);
}

/******************************************************************************
 * _Uart0Disable
 ******************************************************************************/
LOCAL void _Uart0Disable (void)
{
    /* Disable UART0 apb clock */
    //CLOCK_LOG_TRACE:"[_Uart0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_647_112_2_17_23_4_0_838,(uint8*)"");

    CHIP_REG_AND (GR_GEN0, ~BIT_20);
}

/******************************************************************************
 * _Uart1SrcSet
 ******************************************************************************/
LOCAL void _Uart1SrcSet (const char *SrcName)
{
    CHIP_REG_AND (GR_CLK_DLY,  ~ (BIT_23|BIT_22));

    if (strcmp ("96M", SrcName) == 0)
    {
    }
    else if (strcmp ("51M2", SrcName) == 0)
    {
        CHIP_REG_OR (GR_CLK_DLY, (BIT_22));
    }
    else if (strcmp ("48M", SrcName) == 0)
    {
        CHIP_REG_OR (GR_CLK_DLY, (BIT_23));
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR (GR_CLK_DLY, (BIT_23|BIT_22));
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Uart1SrcSet", "96M, 51M2, 48M, 26M_XTL"));
    }
}

/******************************************************************************
 * _Uart1SrcDivSet
 ******************************************************************************/
LOCAL void _Uart1SrcDivSet (int Div)
{
    SCI_PASSERT ( (7 >= Div), ("_Uart1SrcDivSet exceeds the max_value."));/*assert verified*/

    CHIP_REG_AND (GR_CLK_GEN5,  ~ (BIT_5|BIT_4|BIT_3));
    CHIP_REG_OR (GR_CLK_GEN5, (Div<<3));
}
/******************************************************************************
 * _Uart1Enable
 ******************************************************************************/
LOCAL void _Uart1Enable (void)
{
    /* Enable UART1 apb clock */
    //CLOCK_LOG_TRACE:"[_Uart1Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_694_112_2_17_23_4_1_839,(uint8*)"");

    CHIP_REG_OR (GR_GEN0, BIT_21);
}

/******************************************************************************
 * _Uart1Disable
 ******************************************************************************/
LOCAL void _Uart1Disable (void)
{
    /* Disable UART1 apb clock */
    //CLOCK_LOG_TRACE:"[_Uart1Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_704_112_2_17_23_4_1_840,(uint8*)"");

    CHIP_REG_AND (GR_GEN0, ~BIT_21);
}

/******************************************************************************
 * _Uart2SrcSet
 ******************************************************************************/
LOCAL void _Uart2SrcSet (const char *SrcName)
{
    CHIP_REG_AND (GR_CLK_DLY,  ~ (BIT_25|BIT_24));

    if (strcmp ("96M", SrcName) == 0)
    {
    }
    else if (strcmp ("51M2", SrcName) == 0)
    {
        CHIP_REG_OR (GR_CLK_DLY, (BIT_24));
    }
    else if (strcmp ("48M", SrcName) == 0)
    {
        CHIP_REG_OR (GR_CLK_DLY, (BIT_25));
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR (GR_CLK_DLY, (BIT_25|BIT_24));
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Uart2SrcSet", "96M, 51M2, 48M, 26M_XTL"));
    }
}

/******************************************************************************
 * _Uart2SrcDivSet
 ******************************************************************************/
LOCAL void _Uart2SrcDivSet (int Div)
{
    SCI_PASSERT ( (7 >= Div), ("_Uart2SrcDivSet exceeds the max_value."));/*assert verified*/

    CHIP_REG_AND (GR_CLK_GEN5,  ~ (BIT_8|BIT_7|BIT_6));
    CHIP_REG_OR (GR_CLK_GEN5, (Div<<6));
}
/******************************************************************************
 * _Uart2Enable
 ******************************************************************************/
LOCAL void _Uart2Enable (void)
{
    /* Enable UART2 apb clock */
    //CLOCK_LOG_TRACE:"[_Uart2Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_751_112_2_17_23_4_1_841,(uint8*)"");

    CHIP_REG_OR (GR_GEN0, BIT_22);
}
/******************************************************************************
 * _Uart2Disable
 ******************************************************************************/
LOCAL void _Uart2Disable (void)
{
    /* Disable UART2 apb clock */
    //CLOCK_LOG_TRACE:"[_Uart2Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_760_112_2_17_23_4_1_842,(uint8*)"");

    CHIP_REG_AND (GR_GEN0, ~BIT_22);
}


/******************************************************************************
 * _Pwm0SrcSet
 ******************************************************************************/
LOCAL void _Pwm0SrcSet (const char *SrcName)
{
    CHIP_REG_AND (GR_CLK_EN,  ~ (BIT_25));

    if (strcmp ("32K", SrcName) == 0)
    {
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR (GR_CLK_EN, (BIT_25));
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Pwm0SrcSet", "32K, 26M_XTL "));
    }
}
/******************************************************************************
 * _Pwm0Enable
 ******************************************************************************/
LOCAL void _Pwm0Enable (void)
{
    /* Enable pwm0 apb clock */
    //CLOCK_LOG_TRACE:"[_Pwm0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_790_112_2_17_23_4_1_843,(uint8*)"");

    CHIP_REG_OR (GR_CLK_EN, BIT_21);
}
/******************************************************************************
 * _Pwm0Disable
 ******************************************************************************/
LOCAL void _Pwm0Disable (void)
{
    /* Disable PWM0 apb clock */
    //CLOCK_LOG_TRACE:"[_Pwm0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_799_112_2_17_23_4_1_844,(uint8*)"");

    CHIP_REG_AND (GR_CLK_EN, ~BIT_21);
}

/******************************************************************************
 * _Pwm1SrcSet
 ******************************************************************************/
LOCAL void _Pwm1SrcSet (const char *SrcName)
{
    CHIP_REG_AND (GR_CLK_EN,  ~ (BIT_26));

    if (strcmp ("32K", SrcName) == 0)
    {
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR (GR_CLK_EN, (BIT_26));
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Pwm1SrcSet", "32K, 26M_XTL "));
    }
}
/******************************************************************************
 * _Pwm1Enable
 ******************************************************************************/
LOCAL void _Pwm1Enable (void)
{
    /* Enable pwm1 apb clock */
    //CLOCK_LOG_TRACE:"[_Pwm1Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_828_112_2_17_23_4_1_845,(uint8*)"");

    CHIP_REG_OR (GR_CLK_EN, BIT_22);
}
/******************************************************************************
 * _Pwm1Disable
 ******************************************************************************/
LOCAL void _Pwm1Disable (void)
{
    /* Disable PWM1 apb clock */
    //CLOCK_LOG_TRACE:"[_Pwm1Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_837_112_2_17_23_4_1_846,(uint8*)"");

    CHIP_REG_AND (GR_CLK_EN, ~BIT_22);
}

/******************************************************************************
 * _Pwm2SrcSet
 ******************************************************************************/
LOCAL void _Pwm2SrcSet (const char *SrcName)
{
    CHIP_REG_AND (GR_CLK_EN,  ~ (BIT_27));

    if (strcmp ("32K", SrcName) == 0)
    {
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR (GR_CLK_EN, (BIT_27));
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Pwm2SrcSet", "32K, 26M_XTL "));
    }
}
/******************************************************************************
 * _Pwm2Enable
 ******************************************************************************/
LOCAL void _Pwm2Enable (void)
{
    /* Enable pwm2 apb clock */
    //CLOCK_LOG_TRACE:"[_Pwm2Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_866_112_2_17_23_4_1_847,(uint8*)"");

    CHIP_REG_OR (GR_CLK_EN, BIT_23);
}
/******************************************************************************
 * _Pwm2Disable
 ******************************************************************************/
LOCAL void _Pwm2Disable (void)
{
    /* Disable PWM2 apb clock */
    //CLOCK_LOG_TRACE:"[_Pwm2Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_875_112_2_17_23_4_1_848,(uint8*)"");

    CHIP_REG_AND (GR_CLK_EN, ~BIT_23);
}

/******************************************************************************
 * _Pwm3SrcSet
 ******************************************************************************/
LOCAL void _Pwm3SrcSet (const char *SrcName)
{
    CHIP_REG_AND (GR_CLK_EN,  ~ (BIT_28));

    if (strcmp ("32K", SrcName) == 0)
    {
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR (GR_CLK_EN, (BIT_28));
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Pwm3SrcSet", "32K, 26M_XTL "));
    }
}
/******************************************************************************
 * _Pwm3Enable
 ******************************************************************************/
LOCAL void _Pwm3Enable (void)
{
    /* Enable pwm3 apb clock */
    //CLOCK_LOG_TRACE:"[_Pwm3Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_904_112_2_17_23_4_1_849,(uint8*)"");

    CHIP_REG_OR (GR_CLK_EN, BIT_24);
}
/******************************************************************************
 * _Pwm3Disable
 ******************************************************************************/
LOCAL void _Pwm3Disable (void)
{
    /* Disable PWM3 apb clock */
    //CLOCK_LOG_TRACE:"[_Pwm3Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_913_112_2_17_23_4_1_850,(uint8*)"");

    CHIP_REG_AND (GR_CLK_EN, ~BIT_24);
}

/******************************************************************************
 * _Aux0SrcSet
 ******************************************************************************/
LOCAL void _Aux0SrcSet (const char *SrcName)
{
    CHIP_REG_AND (GR_PLL_SCR,  ~ (BIT_11|BIT_10));

    if (strcmp ("96M", SrcName) == 0)
    {
    }
    else if (strcmp ("76M8", SrcName) == 0)
    {
        CHIP_REG_OR (GR_PLL_SCR, (BIT_10));
    }
    else if (strcmp ("32K7_XTL", SrcName) == 0)
    {
        CHIP_REG_OR (GR_PLL_SCR, (BIT_11));
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR (GR_PLL_SCR, (BIT_11|BIT_10));
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Aux0SrcSet", "96M, 76M8, 32K7_XTL, 26M_XTL"));
    }
}

/******************************************************************************
 * _Aux0SrcDivSet
 ******************************************************************************/
LOCAL void _Aux0SrcDivSet (int Div)
{
    SCI_PASSERT ( (0x3F >= Div), ("_Aux0SrcDivSet exceeds the max_value."));/*assert verified*/

    CHIP_REG_AND (GR_GEN1,  ~ (BIT_5|BIT_4|BIT_3|BIT_2|BIT_1|BIT_0));
    CHIP_REG_OR (GR_GEN1, (Div<<0));
}
/******************************************************************************
 * _Aux0Enable
 ******************************************************************************/
LOCAL void _Aux0Enable (void)
{
    //CLOCK_LOG_TRACE:"[_Aux0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_959_112_2_17_23_4_1_851,(uint8*)"");

    /* Enable aux0  */
    CHIP_REG_OR (GR_GEN1, BIT_10);
}
/******************************************************************************
 * _Aux0Disable
 ******************************************************************************/
LOCAL void _Aux0Disable (void)
{
    //CLOCK_LOG_TRACE:"[_Aux0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_968_112_2_17_23_4_1_852,(uint8*)"");

    /* Disable aux0 */
    CHIP_REG_AND (GR_GEN1, ~BIT_10);
}

/******************************************************************************
 * _Aux1SrcSet
 ******************************************************************************/
LOCAL void _Aux1SrcSet (const char *SrcName)
{
    CHIP_REG_AND (GR_PLL_SCR,  ~ (BIT_13|BIT_12));

    if (strcmp ("96M", SrcName) == 0)
    {
    }
    else if (strcmp ("76M8", SrcName) == 0)
    {
        CHIP_REG_OR (GR_PLL_SCR, (BIT_12));
    }
    else if (strcmp ("32K7_XTL", SrcName) == 0)
    {
        CHIP_REG_OR (GR_PLL_SCR, (BIT_13));
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR (GR_PLL_SCR, (BIT_13|BIT_12));
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Aux1SrcSet", "96M, 76M8, 32K7_XTL, 26M_XTL"));
    }
}

/******************************************************************************
 * _Aux1SrcDivSet
 ******************************************************************************/
LOCAL void _Aux1SrcDivSet (int Div)
{
    SCI_PASSERT ( (0x3F >= Div), ("_Aux0SrcDivSet exceeds the max_value."));/*assert verified*/

    CHIP_REG_AND (GR_PCTL,  ~ (BIT_27|BIT_26|BIT_25|BIT_24|BIT_23|BIT_22));
    CHIP_REG_OR (GR_PCTL, (Div<<22));
}
/******************************************************************************
 * _Aux1Enable
 ******************************************************************************/
LOCAL void _Aux1Enable (void)
{
    //CLOCK_LOG_TRACE:"[_Aux1Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1015_112_2_17_23_4_1_853,(uint8*)"");

    /* Enable aux1  */
    CHIP_REG_OR (GR_GEN1, BIT_11);
}
/******************************************************************************
 * _Aux1Disable
 ******************************************************************************/
LOCAL void _Aux1Disable (void)
{
    //CLOCK_LOG_TRACE:"[_Aux1Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1024_112_2_17_23_4_1_854,(uint8*)"");

    /* Disable aux1 */
    CHIP_REG_AND (GR_GEN1, ~BIT_11);
}


/******************************************************************************
 * _Sim0Enable
 ******************************************************************************/
LOCAL void _Sim0Enable (void)
{
    /* Enable SIM0 clock */
    //CLOCK_LOG_TRACE:"[_Sim0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1036_112_2_17_23_4_1_855,(uint8*)"");

    CHIP_REG_OR (GR_GEN0,  GEN0_SIM0_EN);
}

/******************************************************************************
 * _Sim0Disable
 ******************************************************************************/
LOCAL void _Sim0Disable (void)
{
    /* Disable the peripheral SIM0 */
    //CLOCK_LOG_TRACE:"[_Sim0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1046_112_2_17_23_4_1_856,(uint8*)"");

    CHIP_REG_AND (GR_GEN0,  ~GEN0_SIM0_EN);
}

/******************************************************************************
 * _Sim1Enable
 ******************************************************************************/
LOCAL void _Sim1Enable (void)
{
    /* Enable SIM1 clock */
    //CLOCK_LOG_TRACE:"[_Sim1Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1056_112_2_17_23_4_1_857,(uint8*)"");

    CHIP_REG_OR (GR_GEN0,  GEN0_SIM1_EN);
}

/******************************************************************************
 * _Sim1Disable
 ******************************************************************************/
LOCAL void _Sim1Disable (void)
{
    /* Disable the peripheral SIM1 */
    //CLOCK_LOG_TRACE:"[_Sim1Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1066_112_2_17_23_4_1_858,(uint8*)"");

    CHIP_REG_AND (GR_GEN0,  ~GEN0_SIM1_EN);
}

/******************************************************************************
 * _EptEnable
 ******************************************************************************/
LOCAL void _EptEnable (void)
{
    /* Enable ept clock */
    //CLOCK_LOG_TRACE:"[_EptEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1076_112_2_17_23_4_1_859,(uint8*)"");

    CHIP_REG_OR (GR_GEN0,  GEN0_EPT_EN);
}

/******************************************************************************
 * _EptDisable
 ******************************************************************************/
LOCAL void _EptDisable (void)
{
    /* Disable the peripheral ept */
    //CLOCK_LOG_TRACE:"[_EptDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1086_112_2_17_23_4_1_860,(uint8*)"");

    CHIP_REG_AND (GR_GEN0,  ~GEN0_EPT_EN);
}

/******************************************************************************
 * _DmaEnable
 ******************************************************************************/
LOCAL void _DmaEnable (void)
{
    //CLOCK_LOG_TRACE:"[_DmaEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1095_112_2_17_23_4_1_861,(uint8*)"");

    /* Enable DMA clock */
    CHIP_REG_OR (AHB_CTL0, (BIT_6));
}

/******************************************************************************
 * _DmaDisable
 ******************************************************************************/
LOCAL void _DmaDisable (void)
{
    //CLOCK_LOG_TRACE:"[_DmaEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1105_112_2_17_23_4_1_862,(uint8*)"");

    /* disable DMA clock */
    CHIP_REG_AND (AHB_CTL0, ~ (BIT_6));
}

/******************************************************************************
 * _DmaReset
 ******************************************************************************/
LOCAL void _DmaReset (void)
{
    AHB_SOFT_RST_U *ahb_rst = (AHB_SOFT_RST_U *) (AHB_SOFT_RST);

    //CLOCK_LOG_TRACE:"[_DmaReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1117_112_2_17_23_4_1_863,(uint8*)"");

    /* Enable DMA clock */
    ahb_rst->mBits.dma_rst = SCI_FALSE;
    /* Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.dma_rst = SCI_TRUE;
    /*Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.dma_rst = SCI_FALSE;
}


/******************************************************************************
 * _NfcEnable
 ******************************************************************************/
LOCAL void _NfcEnable (void)
{
    //CLOCK_LOG_TRACE:"[_NfcEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1134_112_2_17_23_4_1_864,(uint8*)"");

    /* Enable nfc clock */
    CHIP_REG_OR (AHB_CTL0, BIT_8);
}

/******************************************************************************
 * _NfcDisable
 ******************************************************************************/
LOCAL void _NfcDisable (void)
{
    //CLOCK_LOG_TRACE:"[_NfcDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1144_112_2_17_23_4_1_865,(uint8*)"");

    /* Disable Nfc clock */
    CHIP_REG_AND (AHB_CTL0, ~BIT_8);
}

/******************************************************************************
 * _NfcReset
 ******************************************************************************/
LOCAL void _NfcReset (void)
{
    AHB_SOFT_RST_U *ahb_rst = (AHB_SOFT_RST_U *) (AHB_SOFT_RST);

    //CLOCK_LOG_TRACE:"[_DmaReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1156_112_2_17_23_4_1_866,(uint8*)"");

    /* Enable DMA clock */
    ahb_rst->mBits.nfc_rst = SCI_FALSE;
    /* Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.nfc_rst = SCI_TRUE;
    /*Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.nfc_rst = SCI_FALSE;
}

/******************************************************************************
 * _Bm0Enable
 ******************************************************************************/
LOCAL void _Bm0Enable (void)
{
    //CLOCK_LOG_TRACE:"[_Bm0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1172_112_2_17_23_4_1_867,(uint8*)"");

    /* Enable BM0 clock */
    CHIP_REG_OR (AHB_CTL0, BIT_7);
}

/******************************************************************************
 * _Bm0Disable
 ******************************************************************************/
LOCAL void _Bm0Disable (void)
{
    //CLOCK_LOG_TRACE:"[_Bm0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1182_112_2_17_23_4_1_868,(uint8*)"");

    /* Enable BM0 clock */
    CHIP_REG_AND (AHB_CTL0, ~BIT_7);
}

/******************************************************************************
 * _Bm1Enable
 ******************************************************************************/
LOCAL void _Bm1Enable (void)
{
    //CLOCK_LOG_TRACE:"[_Bm1Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1192_112_2_17_23_4_1_869,(uint8*)"");

    /* Enable BM1 clock */
    CHIP_REG_OR (AHB_CTL0, BIT_11);
}

/******************************************************************************
 * _Bm1Disable
 ******************************************************************************/
LOCAL void _Bm1Disable (void)
{
    //CLOCK_LOG_TRACE:"[_Bm1Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1202_112_2_17_23_4_1_870,(uint8*)"");

    /* Enable BM1 clock */
    CHIP_REG_AND (AHB_CTL0, ~BIT_11);
}

/******************************************************************************
 * _RotEnable
 ******************************************************************************/
LOCAL void _RotEnable (void)
{
    //CLOCK_LOG_TRACE:"[_RotEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1212_112_2_17_23_4_1_871,(uint8*)"");

    /* Enable ROT clock */
    CHIP_REG_OR (AHB_CTL0, BIT_14);
}
/******************************************************************************
 * _RotDisable
 ******************************************************************************/
LOCAL void _RotDisable (void)
{
    //CLOCK_LOG_TRACE:"[_RotDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1221_112_2_17_23_4_1_872,(uint8*)"");

    /* Enable ROT clock */
    CHIP_REG_AND (AHB_CTL0, ~BIT_14);
}

/******************************************************************************
 * _RotReset
 ******************************************************************************/
LOCAL void _RotReset (void)
{
    AHB_SOFT_RST_U *ahb_rst = (AHB_SOFT_RST_U *) (AHB_SOFT_RST);

    //CLOCK_LOG_TRACE:"[_RotReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1233_112_2_17_23_4_2_873,(uint8*)"");

    /* Enable DMA clock */
    ahb_rst->mBits.rot_rst = SCI_FALSE;
    /* Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.rot_rst = SCI_TRUE;
    /*Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.rot_rst = SCI_FALSE;
}

/******************************************************************************
 * _UsbSrcSet
 ******************************************************************************/
LOCAL void _UsbSrcSet (const char *SrcName)
{
    CHIP_REG_AND (AHB_CTL3,  ~ (BIT_0));

    if (strcmp ("12M", SrcName) == 0)
    {
    }
    else if (strcmp ("24M", SrcName) == 0)
    {
        CHIP_REG_OR (AHB_CTL3, (BIT_0));
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_UsbSrcSet", "12M, 24M"));
    }
}
/******************************************************************************
 * _UsbEnable
 ******************************************************************************/
LOCAL void _UsbEnable (void)
{
    //CLOCK_LOG_TRACE:"[_UsbEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1268_112_2_17_23_4_2_874,(uint8*)"");

    /* Enable USB clock */
    CHIP_REG_OR (AHB_CTL0, BIT_5);
}

/******************************************************************************
 * _UsbDisable
 ******************************************************************************/
LOCAL void _UsbDisable (void)
{
    //CLOCK_LOG_TRACE:"[_UsbDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1278_112_2_17_23_4_2_875,(uint8*)"");

    /* disable usb clock */
    CHIP_REG_AND (AHB_CTL0, ~BIT_5);
}

/******************************************************************************
 * _UsbReset
 ******************************************************************************/
LOCAL void _UsbReset (void)
{
    AHB_SOFT_RST_U *ahb_rst = (AHB_SOFT_RST_U *) (AHB_SOFT_RST);

    //CLOCK_LOG_TRACE:"[_IspReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1290_112_2_17_23_4_2_876,(uint8*)"");

    /* Enable DMA clock */
    ahb_rst->mBits.usbd_utmi_rst = SCI_FALSE;
    /* Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.usbd_utmi_rst = SCI_TRUE;
    /*Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.usbd_utmi_rst = SCI_FALSE;

    ahb_rst->mBits.usbphy_rst = SCI_FALSE;
    /* Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.usbphy_rst = SCI_TRUE;
    /*Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.usbphy_rst = SCI_FALSE;
}

/******************************************************************************
 * _CcirEnable
 ******************************************************************************/
LOCAL void _CcirEnable (void)
{
    //CLOCK_LOG_TRACE:"[_CcirEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1314_112_2_17_23_4_2_877,(uint8*)"");

    CHIP_REG_OR (GR_GEN0, BIT_14); // CCIR CLK Enable
}


/******************************************************************************
 * _CcirDisable
 ******************************************************************************/
LOCAL void _CcirDisable (void)
{
    //CLOCK_LOG_TRACE:"[_CcirDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1324_112_2_17_23_4_2_878,(uint8*)"");

    CHIP_REG_AND (GR_GEN0, ~BIT_14);
}

/******************************************************************************
 * _CcirSrcSet
 ******************************************************************************/
LOCAL void _CcirSrcSet (const char *SrcName)
{
    uint32 value = 0;
    value = CHIP_REG_GET (GR_PLL_SCR);

    value &= ~ (BIT_19  | BIT_18);

    if (strcmp ("48M", SrcName) == 0)
    {
        value |= 0x0;//00:48Mhz
    }
    else if (strcmp ("TDPLL", SrcName) == 0)
    {
        value |= BIT_18;
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        value |= BIT_19;
    }
    else
    {
        SCI_PASSERT (SCI_FALSE, ("%s , %s %s", "_CcirSrcSet", "48M, BPLLDIV10")); /*assert verified*/
    }

    CHIP_REG_SET (GR_PLL_SCR, value);
}


/******************************************************************************
 * _CcirSrcDivSet
 ******************************************************************************/
LOCAL void _CcirSrcDivSet (int Div)
{
    int val = 0;

    if (Div > (BIT_0 | BIT_1))
    {
        Div = (BIT_0 | BIT_1);
    }

    Div<<= 24;
    val = CHIP_REG_GET (GR_GEN3);
    val &= ~ (BIT_24 | BIT_25);
    val |= Div;
    CHIP_REG_SET (GR_GEN3, val); // CCIR divide factor
}

/******************************************************************************
 * _CcirReset
 ******************************************************************************/
LOCAL void _CcirReset (void)
{
    AHB_SOFT_RST_U *ahb_rst = (AHB_SOFT_RST_U *) (AHB_SOFT_RST);

    //CLOCK_LOG_TRACE:"[_CcirReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1383_112_2_17_23_4_2_879,(uint8*)"");

    /* Enable DMA clock */
    ahb_rst->mBits.ccir_rst = SCI_FALSE;
    /* Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.ccir_rst = SCI_TRUE;
    /*Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.ccir_rst = SCI_FALSE;
}

/******************************************************************************
 * _AhbEnable
 ******************************************************************************/
LOCAL void _AhbEnable (void)
{
    //CLOCK_LOG_TRACE:"[_AhbEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1399_112_2_17_23_4_2_880,(uint8*)"");

    return;
}

/******************************************************************************
 * _AhbDisable
 ******************************************************************************/
LOCAL void _AhbDisable (void)
{
    //CLOCK_LOG_TRACE:"[_AhbDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1408_112_2_17_23_4_2_881,(uint8*)"");

    return;
}

/******************************************************************************
 * _ApbEnable
 ******************************************************************************/
LOCAL void _ApbEnable (void)
{
    //CLOCK_LOG_TRACE:"[_ApbEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1417_112_2_17_23_4_2_882,(uint8*)"");

    return;
}

/******************************************************************************
 * _ApbDisable
 ******************************************************************************/
LOCAL void _ApbDisable (void)
{
    //CLOCK_LOG_TRACE:"[_ApbDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1426_112_2_17_23_4_2_883,(uint8*)"");

    return;
}

/******************************************************************************
 * _WdgEnable
 ******************************************************************************/
LOCAL void _WdgEnable (void)
{
    /* Enable watchdog apb clock */
    //CLOCK_LOG_TRACE:"[_WdgEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1436_112_2_17_23_4_2_884,(uint8*)"");

    ANA_REG_OR (ANA_AGEN, (AGEN_WDG_EN | AGEN_RTC_WDG_EN));
}

/******************************************************************************
 * _WdgDisable
 ******************************************************************************/
LOCAL void _WdgDisable (void)
{
    /* Disable watchdog apb clock */
    //CLOCK_LOG_TRACE:"[_WdgDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1446_112_2_17_23_4_2_885,(uint8*)"");

    // Disable watchdog programming
    ANA_REG_OR (ANA_AGEN, (~AGEN_WDG_EN));
}

/******************************************************************************
 * _PcmSrcSet
 ******************************************************************************/
LOCAL void _PcmSrcSet (const char *SrcName)
{
    CHIP_REG_AND (GR_PLL_SCR,  ~ (BIT_9|BIT_8));

    if (strcmp ("128M", SrcName) == 0)
    {
    }
    else if (strcmp ("51M2", SrcName) == 0)
    {
        CHIP_REG_OR (GR_PLL_SCR, (BIT_8));
    }
    else if (strcmp ("26M_XTL", SrcName) == 0)
    {
        CHIP_REG_OR (GR_PLL_SCR, (BIT_9|BIT_8));
    }
    else
    {
        //from pad 2'B10
        CHIP_REG_OR (GR_PLL_SCR, (BIT_9));
    }
}

/******************************************************************************
 * _PcmSrcDivSet
 ******************************************************************************/
LOCAL void _PcmSrcDivSet (int Div)
{
    SCI_PASSERT ( (0x3F >= Div), ("_PcmSrcDivSet exceeds the max_value."));/*assert verified*/

    CHIP_REG_AND (GR_GEN2,  ~ (BIT_29|BIT_28|BIT_27|BIT_26|BIT_25|BIT_24));
    CHIP_REG_OR (GR_GEN2, (Div<<24));
}

/******************************************************************************
 * _PcmEnable
 ******************************************************************************/
LOCAL void _PcmEnable (void)
{
    /* Enable Pcm apb clock */
    //CLOCK_LOG_TRACE:"[_PcmEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1491_112_2_17_23_4_2_886,(uint8*)"");

    /*ken.kuang Select ARM ctrl*/
    // arm control:0
    CHIP_REG_AND (GR_PCTL, ~MISC0_IIS_MUX_SEL);

    CHIP_REG_OR (GR_GEN0,GEN0_I2S_EN);
}

/******************************************************************************
 * _PcmDisable
 ******************************************************************************/
LOCAL void _PcmDisable (void)
{
    /* Disable Pcm apb clock */
    //CLOCK_LOG_TRACE:"[_PcmDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1505_112_2_17_23_4_2_887,(uint8*)"");

    REG32 (GR_GEN0) &= ~GEN0_I2S_EN;
}

/******************************************************************************
 * _GeaEnable
 ******************************************************************************/
LOCAL void _GeaEnable (void)
{
    /* Enable Gea apb clock */
    //CLOCK_LOG_TRACE:"[_GeaEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1515_112_2_17_23_4_2_888,(uint8*)"");

    CHIP_REG_OR (GR_GEN0, GEN0_EPT_EN);   // gea enable
}

/******************************************************************************
 * _GeaDisable
 ******************************************************************************/
LOCAL void _GeaDisable (void)
{
    /* Disable Gea apb clock */
    //CLOCK_LOG_TRACE:"[_GeaDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1525_112_2_17_23_4_2_889,(uint8*)"");
    CHIP_REG_AND (GR_GEN0,  ~GEN0_EPT_EN);
}

/******************************************************************************
 * _AudioEnable
 ******************************************************************************/
LOCAL void _AudioEnable (void)
{
    /* Enable Audio apb clock */
    //CLOCK_LOG_TRACE:"[_AudioEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1534_112_2_17_23_4_2_890,(uint8*)"");

    // Enable voiceband module
    CHIP_REG_OR((GR_GEN0) , GEN0_VB_EN);
}

/******************************************************************************
 * _AudioDisable
 ******************************************************************************/
LOCAL void _AudioDisable (void)
{
    /* Disable Audio apb clock */
    //CLOCK_LOG_TRACE:"[_AudioDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC8800G_CLOCK_CFG_1545_112_2_17_23_4_2_891,(uint8*)"");

    // Disable voiceband module
    CHIP_REG_AND((GR_GEN0) , ~GEN0_VB_EN);
}


/******************************************************************************
 * Static functions prototypes
 ******************************************************************************/
LOCAL const CLOCK system_clock[] =
{
    {
        "26M_XTL",       //.Name
        26000000,
        "26M_XTL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,     //.Enable
        SCI_NULL,    //.Disable
    },
    {
        "32K7_XTL",      //.Name
        32768,
        "32K7_XTL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,     //.Enable
        SCI_NULL,    //.Disable
    },
    {
        "MPLL",      //.Name
        400000000,   //400M
        "26M_XTL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,     //.Enable
        SCI_NULL,    //.Disable
    },
    {
        "TDPLL",      //.Name
        768000000,    //768M
        "26M_XTL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,     //.Enable
        SCI_NULL,    //.Disable
    },
    {
        "TDPLLDIV3",      //.Name
        256000000,    //256M
        "TDPLL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,     //.Enable
        SCI_NULL,    //.Disable
    },
    {
        "TDPLLDIV4",      //.Name
        192000000,
        "TDPLL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,     //.Enable
        SCI_NULL,    //.Disable
    },
    {
        "TDPLLDIV5",      //.Name
        153600000,
        "TDPLL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,     //.Enable
        SCI_NULL,    //.Disable
    },
    {
        "128M",      //.Name
        128000000,
        "TDPLLDIV3",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,     //.Enable
        SCI_NULL,    //.Disable
    },
    {
        "64M",      //.Name
        64000000,
        "TDPLLDIV3",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,     //.Enable
        SCI_NULL,    //.Disable
    },
    {
        "96M",      //.Name
        96000000,
        "TDPLLDIV4",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,     //.Enable
        SCI_NULL,    //.Disable
    },
    {
        "48M",      //.Name
        48000000,
        "TDPLLDIV4",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,     //.Enable
        SCI_NULL,    //.Disable
    },
    {
        "24M",      //.Name
        24000000,
        "TDPLLDIV4",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,     //.Enable
        SCI_NULL,    //.Disable
    },
    {
        "12M",      //.Name
        12000000,
        "TDPLLDIV4",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,     //.Enable
        SCI_NULL,    //.Disable
    },
    {
        "13M",      //.Name
        13000000,
        "26M_XTL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,     //.Enable
        SCI_NULL,    //.Disable
    },
    {
        "6M5",      //.Name
        6500000,
        "26M_XTL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,     //.Enable
        SCI_NULL,    //.Disable
    },
    {
        "QBC",      //.Name
        1083000,
        "26M_XTL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,     //.Enable
        SCI_NULL,    //.Disable
    },
    {
        "200M",      //.Name
        200000000,
        "MPLL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,     //.Enable
        SCI_NULL,    //.Disable
    },
    {
        "100M",      //.Name
        100000000,
        "MPLL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,     //.Enable
        SCI_NULL,    //.Disable
    },
    {
        "51M2",      //.Name
        51200000,
        "TDPLLDIV5",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,     //.Enable
        SCI_NULL,    //.Disable
    },
    {
        "10M24",      //.Name
        10240000,
        "TDPLLDIV5",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,     //.Enable
        SCI_NULL,    //.Disable
    },
    {
        "5M12",      //.Name
        5120000,
        "TDPLLDIV5",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,     //.Enable
        SCI_NULL,    //.Disable
    },
};

LOCAL const DEVICE system_device[] =
{
    {
        "AHB",           //.Name
        SCI_NULL,           //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _AhbEnable,    //.Enable
        _AhbDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,        //.Reset
    },
    {
        "APB",           //.Name
        SCI_NULL,       //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _ApbEnable,    //.Enable
        _ApbDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,        //.Reset
    },
    {
        "WATCHDOG",     //.Name
        SCI_NULL,          //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _WdgEnable,    //.Enable
        _WdgDisable,   //.Disable
        SCI_NULL,       //.SrcSel
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "RTC",    //.Name
        SCI_NULL,          //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _RtcEnable,  //.Enable
        _RtcDisable, //.Disable
        SCI_NULL,       //.SrcSel
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "RTCTIMER0",    //.Name
        SCI_NULL,          //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _RtcTimer0Enable,  //.Enable
        _RtcTimer0Disable, //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "KEYPAD",       //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _KpdEnable,    //.Enable
        _KpdDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "SYSCOUNTER",   //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _SyscntEnable, //.Enable
        _SyscntDisable,//.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "ADC",   //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _AdcEnable, //.Enable
        _AdcDisable,//.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "TPC",          //.Name


        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _TpcEnable,    //.Enable
        _TpcDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "I2C0",  //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _I2C0Enable,    //.Enable
        _I2C0Disable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "UART0",  //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Uart0Enable,    //.Enable
        _Uart0Disable,   //.Disable
        _Uart0SrcSet,       //.SrcSet
        _Uart0SrcDivSet,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "UART1",  //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Uart1Enable,    //.Enable
        _Uart1Disable,   //.Disable
        _Uart1SrcSet,       //.SrcSet
        _Uart1SrcDivSet,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "UART2",  //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Uart2Enable,    //.Enable
        _Uart2Disable,   //.Disable
        _Uart2SrcSet,       //.SrcSet
        _Uart2SrcDivSet,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "SIM0",  //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Sim0Enable,    //.Enable
        _Sim0Disable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "SIM1",  //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Sim1Enable,    //.Enable
        _Sim1Disable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "DMA",  //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _DmaEnable,    //.Enable
        _DmaDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "NFC",  //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _NfcEnable,    //.Enable
        _NfcDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "USB",  //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _UsbEnable,    //.Enable
        _UsbDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "CCIR",         //.Name
        48000000,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _CcirEnable,   //.Enable
        _CcirDisable,  //.Disable
        _CcirSrcSet,       //.SrcSet
        _CcirSrcDivSet,
        _CcirReset,        //.Reset
    },
    {
        "LCDC",  //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _LcdcEnable,    //.Enable
        _LcdcDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "BM0",  //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Bm0Enable,    //.Enable
        _Bm0Disable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "BM1",  //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Bm1Enable,    //.Enable
        _Bm1Disable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "ROT",  //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _RotEnable,    //.Enable
        _RotDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "AUDIO",    //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _AudioEnable,   //.Enable
        _AudioDisable,  //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "PCM",  //.Name
        51200000,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _PcmEnable,    //.Enable
        _PcmDisable,   //.Disable
        _PcmSrcSet,       //.SrcSet
        _PcmSrcDivSet,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "I2S",  //.Name
        51200000,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _I2sEnable,    //.Enable
        _I2sDisable,   //.Disable
        _I2sSrcSet,       //.SrcSet
        _I2sSrcDivSet,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "SPI",  //.Name
        192000000,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _SpiEnable,    //.Enable
        _SpiDisable,   //.Disable
        _SpiSrcSet,       //.SrcSet
        _SpiSrcDivSet,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "PWM0",  //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Pwm0Enable,    //.Enable
        _Pwm0Disable,   //.Disable
        _Pwm0SrcSet,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "PWM1",  //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Pwm1Enable,    //.Enable
        _Pwm1Disable,   //.Disable
        _Pwm1SrcSet,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "PWM2",  //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Pwm2Enable,    //.Enable
        _Pwm2Disable,   //.Disable
        _Pwm2SrcSet,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "PWM3",  //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Pwm3Enable,    //.Enable
        _Pwm3Disable,   //.Disable
        _Pwm3SrcSet,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "AUX0",  //.Name
        32768,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Aux0Enable,    //.Enable
        _Aux0Disable,   //.Disable
        _Aux0SrcSet,       //.SrcSet
        _Aux0SrcDivSet,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "AUX1",  //.Name
        32768,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Aux1Enable,    //.Enable
        _Aux1Disable,   //.Disable
        _Aux1SrcSet,       //.SrcSet
        _Aux0SrcDivSet,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "EPT",  //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _EptEnable,    //.Enable
        _EptDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "GEA",  //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _GeaEnable,    //.Enable
        _GeaDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,       //.DivSet
        SCI_NULL,       //.Reset
    },

    //--------------- PAY ATTENTION below--------------SET .bEnable FALSE
    {
        "DCAM",  //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_FALSE,       //.bEnable
        _DcamEnable,    //.Enable
        _DcamDisable,   //.Disable
        _DcamSrcSet,       //.SrcSet
        _DcamSrcDivSet,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "VSP",  //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_FALSE,       //.bEnable
        _VspEnable,    //.Enable
        _VspDisable,   //.Disable
        _VspSrcSet,       //.SrcSet
        _VspSrcDivSet,       //.DivSet
        SCI_NULL,       //.Reset
    },
    {
        "SDIO",  //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_FALSE,       //.bEnable
        _SdioEnable,    //.Enable
        _SdioDisable,   //.Disable
        _SdioSrcSet,       //.SrcSet
        _SdioSrcDivSet,       //.DivSet
        _SdioReset,       //.Reset
    },
    //--------------- PAY ATTENTION above--------------SET .bEnable FALSE
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
//End
