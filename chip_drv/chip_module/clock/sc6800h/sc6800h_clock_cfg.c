/******************************************************************************
 ** File Name:        clock_cfg.c                                           *
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
#include "../clock.h"
#include "Ref_outport.h"

static const char *err_string = "%s choosed a wrong source [%s]\r\n please choose one of: [%s]\r\n";

/******************************************************************************
 * _DcamSrcSet
 ******************************************************************************/
LOCAL void _DcamSrcSet (const char *SrcName)
{
    AHB_CLK_CFG0_U *clk_cfg = (AHB_CLK_CFG0_U *) (AHB_CLK_CFG0);

    if(SC6800H == CHIP_GetChipType())
    {
        if (strcmp ("MPLLDIV4", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_dcam = 2;
        }
        else if (strcmp ("MPLLDIV6", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_dcam = 1;
        }
        else if (strcmp ("MPLLDIV8", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_dcam = 0;
        }
        else
        {
            SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                         (err_string, "_DcamSrcSet", SrcName, "MPLLDIV4, MPLLDIV6, MPLLDIV8"));
        }
    }
    else
    {
        if (strcmp ("26M_XTL", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_dcam = 0;
        }
        else if (strcmp ("MPLLDIV15", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_dcam = 1;
        }
        else if (strcmp ("MPLLDIV10", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_dcam = 2;
        }
        else if (strcmp ("MPLLDIV60", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_dcam = 3;
        }
        else
        {
            SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                         (err_string, "_DcamSrcSet", SrcName, "26M_XTL, MPLLDIV15, MPLLDIV10, MPLLDIV60"));
        }
    }
}

/******************************************************************************
 * _McuSrcSet
 ******************************************************************************/
LOCAL void _McuSrcSet (const char *SrcName)
{
    AHB_CLK_CFG0_U *clk_cfg = (AHB_CLK_CFG0_U *) (AHB_CLK_CFG0);

    if(SC6800H == CHIP_GetChipType())
    {
        if (strcmp ("26M_XTL", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_arm = 0;
        }
        else if (strcmp ("MPLLDIV2", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_arm = 1;
        }
        else
        {
            SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                         (err_string, "_McuSrcSet", SrcName, "26M_XTL, MPLLDIV2"));
        }
    }
    else
    {
        if (strcmp ("26M_XTL", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_arm = 0;
        }
        else if (strcmp ("MPLLDIV4", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_arm = 1;
        }
        else
        {
            SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                         (err_string, "_McuSrcSet", SrcName, "26M_XTL, MPLLDIV4"));
        }
    }
}

/******************************************************************************
 * _ArmSrcDivSet
 ******************************************************************************/
LOCAL void _ArmSrcDivSet (int Div)
{
    AHB_CLK_CFG0_U *clk_cfg = (AHB_CLK_CFG0_U *) (AHB_CLK_CFG0);

    SCI_PASSERT ( (3 >= Div), ("_McuSrcDivSet exceeds the max_value."));/*assert verified*/

    clk_cfg->mBits.div_cfg_arm = Div;
}

/******************************************************************************
 * _AhbSrcDivSet
 ******************************************************************************/
LOCAL void _AhbSrcDivSet (int Div)
{
    AHB_CLK_CFG0_U *clk_cfg = (AHB_CLK_CFG0_U *) (AHB_CLK_CFG0);

    SCI_PASSERT ( (1 >= Div), ("_AhbSrcDivSet exceeds the max_value."));/*assert verified*/

    clk_cfg->mBits.div_cfg_ahb = Div;
}

/******************************************************************************
 * _EmcSrcSet
 ******************************************************************************/
LOCAL void _EmcSrcSet (const char *SrcName)
{
    AHB_CLK_CFG0_U *clk_cfg = (AHB_CLK_CFG0_U *) (AHB_CLK_CFG0);

    if(SC6800H == CHIP_GetChipType())
    {
        if (strcmp ("26M_XTL", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_emc = 0;
        }
        else if (strcmp ("MPLLDIV2", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_emc = 0xC;
        }
        else if (strcmp ("MPLLDIV3", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_emc = 0xD;
        }
        else if (strcmp ("BPLLDIV3", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_emc = 0xE;
        }
        else
        {
            SCI_PASSERT (SCI_FALSE, /*assert verified*/
                         (err_string, "_EmcSrcSet", SrcName, "26M_XTL, MPLLDIV2, MPLLDIV3, BPLLDIV3"));
        }
    }
    else
    {
        if (strcmp ("26M_XTL", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_emc = 0;
        }
        else if (strcmp ("MPLLDIV4", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_emc = 1;
        } 
        else
        {
            SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                         (err_string, "_EmcSrcSet", SrcName, "26M_XTL, MPLLDIV4 "));
        }
    }
}

/******************************************************************************
 * _EmcSrcDivSet
 ******************************************************************************/
LOCAL void _EmcSrcDivSet (int Div)
{
    AHB_CLK_CFG0_U *clk_cfg = (AHB_CLK_CFG0_U *) (AHB_CLK_CFG0);

    SCI_PASSERT ( (3 >= Div), ("_EmcSrcDivSet exceeds the max_value."));/*assert verified*/

    clk_cfg->mBits.div_cfg_emc = Div;
}

/******************************************************************************
 * _CcirSrcSet
 ******************************************************************************/
LOCAL void _CcirSrcSet (const char *SrcName)
{
    AHB_CLK_CFG0_U *clk_cfg = (AHB_CLK_CFG0_U *) (AHB_CLK_CFG0);

    if(SC6800H == CHIP_GetChipType())
    {
        if (strcmp ("26M_XTL", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_ccir = 0;
        }
        else if (strcmp ("BPLLDIV10", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_ccir = 1;
        }
        else
        {
            SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                         (err_string, "_CcirSrcSet", "26M_XTL, BPLLDIV10"));
        }
    }
    else
    {
        if (strcmp ("26M_XTL", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_ccir = 0;
        }
        else if (strcmp ("MPLLDIV15", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_ccir = 1;
        }
        else
        {
            SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                         (err_string, "_CcirSrcSet", "26M_XTL, MPLLDIV15"));
        }
    }
}

/******************************************************************************
 * _CcirSrcDivSet
 ******************************************************************************/
LOCAL void _CcirSrcDivSet (int Div)
{
    AHB_CLK_CFG0_U *clk_cfg = (AHB_CLK_CFG0_U *) (AHB_CLK_CFG0);

    SCI_PASSERT ( (7 >= Div), ("_CcirSrcDivSet exceeds the max_value."));/*assert verified*/

    clk_cfg->mBits.div_cfg_ccir = Div;
}

/******************************************************************************
 * _SdioSrcSet
 ******************************************************************************/
LOCAL void _SdioSrcSet (const char *SrcName)
{
    AHB_CLK_CFG1_U *clk_cfg = (AHB_CLK_CFG1_U *) (AHB_CLK_CFG1);

    if(SC6800H == CHIP_GetChipType())
    {
        if (strcmp ("26M_XTL", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_sdio = 0;
        }
        else if (strcmp ("MPLLDIV8", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_sdio = 1;
        }
        else if (strcmp ("MPLLDIV4", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_sdio = 2;
        }
        else
        {
            SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                         (err_string, "_SdioSrcSet", SrcName, "26M_XTL, MPLLDIV8, MPLLDIV4"));
        }
    }
    else
    {
        if (strcmp ("26M_XTL", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_sdio = 0;
        }
        else if (strcmp ("MPLLDIV16", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_sdio = 1;
        }
        else if (strcmp ("MPLLDIV15", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_sdio = 2;
        }
        else if (strcmp ("MPLLDIV8", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_sdio = 3;
        }
        else
        {
            SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                         (err_string, "_SdioSrcSet", SrcName, "26M_XTL, MPLLDIV16, MPLLDIV15, MPLLDIV8"));
        }
    }
}

/******************************************************************************
 * _LcdcSrcSet
 ******************************************************************************/
LOCAL void _LcdcSrcSet (const char *SrcName)
{
    AHB_CLK_CFG1_U *clk_cfg = (AHB_CLK_CFG1_U *) (AHB_CLK_CFG1);

    if (strcmp ("26M_XTL", SrcName) == 0)
    {
        clk_cfg->mBits.src_sel_lcdc = 0;
    }
    else if (strcmp ("MPLLDIV8", SrcName) == 0)
    {
        clk_cfg->mBits.src_sel_lcdc = 2;
    }
    else if (strcmp ("MPLLDIV6", SrcName) == 0)
    {
        clk_cfg->mBits.src_sel_lcdc = 3;
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_LcdcSrcSet", SrcName, "26M_XTL, MPLLDIV8, MPLLDIV6"));
    }
}

/******************************************************************************
 * _48MSrcSet
 ******************************************************************************/
LOCAL void _48MSrcSet (const char *SrcName)
{
    if(SC6800H == CHIP_GetChipType())
    {
        AHB_CLK_CFG1_U *clk_cfg = (AHB_CLK_CFG1_U *) (AHB_CLK_CFG1);

        if (strcmp ("MPLLDIV8", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_48m = 0;
        }
        else if (strcmp ("BPLLDIV10", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_48m = 1;
        }
        else
        {
            SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                         (err_string, "_48MSrcSet", SrcName, "MPLLDIV8, BPLLDIV10"));
        }
    }
}

/******************************************************************************
 * _I2sSrcSet
 ******************************************************************************/
LOCAL void _I2sSrcSet (const char *SrcName)
{
    GR_CLK_CTL0_U *clk_cfg = (GR_CLK_CTL0_U *) (GR_CLK_CFG0);

    if(SC6800H == CHIP_GetChipType())
    {
        if (strcmp ("26M_XTL", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_iis = 0;
        }
        else if (strcmp ("BPLLDIV3", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_iis = 0x6;
        }
        else if (strcmp ("MPLLDIV3", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_iis = 0x5;
        }
        else if (strcmp ("MPLLDIV4", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_iis = 0x4;
        }
        else
        {
            SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                         (err_string, "_I2sSrcSet", SrcName, "26M_XTL, BPLLDIV3, MPLLDIV8, BPLLDIV10"));
        }
    }
    else
    {
        if (strcmp ("26M_XTL", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_iis = 0;
        }
        else if (strcmp ("MPLLDIV16", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_iis = 0x1;
        }
        else if (strcmp ("MPLLDIV15", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_iis = 0x2;
        }
        else
        {
            SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                         (err_string, "_I2sSrcSet", SrcName, "26M_XTL, MPLLDIV16, MPLLDIV15"));
        }
    }
}

/******************************************************************************
 * _I2sSrcDivSet
 ******************************************************************************/
LOCAL void _I2sSrcDivSet (int Div)
{
    GR_CLK_CTL0_U *clk_cfg = (GR_CLK_CTL0_U *) (GR_CLK_CFG0);

    SCI_PASSERT ( (0x1F >= Div), ("_I2sSrcDivSet exceeds the max_value."));/*assert verified*/

    clk_cfg->mBits.div_cfg_iis = Div;
}

/******************************************************************************
 * _UartSrcSet
 ******************************************************************************/
LOCAL void _UartSrcSet (const char *SrcName)
{
    GR_CLK_CTL1_U *clk_cfg = (GR_CLK_CTL1_U *) (GR_CLK_CFG1);

    if(SC6800H == CHIP_GetChipType())
    {
        if (strcmp ("26M_XTL", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_uart0 = 0;
        }
        else if (strcmp ("MPLLDIV4", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_uart0 = 1;
        }
        else
        {
            SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                         (err_string, "_UartSrcSet", SrcName, "26M_XTL, MPLLDIV4"));
        }
    }
    else
    {
        if (strcmp ("26M_XTL", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_uart0 = 0;
        }
        else if (strcmp ("MPLLDIV10", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_uart0 = 1;
        }
        else
        {
            SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                         (err_string, "_UartSrcSet", SrcName, "26M_XTL, MPLLDIV10"));
        }
    }
}

/******************************************************************************
 * _UartSrcDivSet
 ******************************************************************************/
LOCAL void _UartSrcDivSet (int Div)
{
    GR_CLK_CTL1_U *clk_cfg = (GR_CLK_CTL1_U *) (GR_CLK_CFG1);

    SCI_PASSERT ( (7 >= Div), ("_UartSrcDivSet exceeds the max_value."));/*assert verified*/

    clk_cfg->mBits.div_cfg_uart0 = Div;
}

/******************************************************************************
 * _Pwm0SrcSet
 ******************************************************************************/
LOCAL void _Pwm0SrcSet (const char *SrcName)
{
    GR_CLK_CTL1_U *clk_cfg = (GR_CLK_CTL1_U *) (GR_CLK_CFG1);

    if (strcmp ("26M_XTL", SrcName) == 0)
    {
        clk_cfg->mBits.src_sel_pwm0 = 0;
    }
    else if (strcmp ("32K7_XTL", SrcName) == 0)
    {
        clk_cfg->mBits.src_sel_pwm0 = 1;
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Pwm0SrcSet", SrcName, "26M_XTL, 32K7_XTL"));
    }
}

/******************************************************************************
 * _Pwm1SrcSet
 ******************************************************************************/
LOCAL void _Pwm1SrcSet (const char *SrcName)
{
    GR_CLK_CTL1_U *clk_cfg = (GR_CLK_CTL1_U *) (GR_CLK_CFG1);

    if (strcmp ("26M_XTL", SrcName) == 0)
    {
        clk_cfg->mBits.src_sel_pwm1 = 0;
    }
    else if (strcmp ("32K7_XTL", SrcName) == 0)
    {
        clk_cfg->mBits.src_sel_pwm1 = 1;
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Pwm1SrcSet", SrcName, "26M_XTL, 32K7_XTL"));
    }
}
/******************************************************************************
 * _Pwm2SrcSet
 ******************************************************************************/
LOCAL void _Pwm2SrcSet (const char *SrcName)
{
    GR_CLK_CTL1_U *clk_cfg = (GR_CLK_CTL1_U *) (GR_CLK_CFG1);

    if (strcmp ("26M_XTL", SrcName) == 0)
    {
        clk_cfg->mBits.src_sel_pwm2 = 0;
    }
    else if (strcmp ("32K7_XTL", SrcName) == 0)
    {
        clk_cfg->mBits.src_sel_pwm2 = 1;
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Pwm2SrcSet", SrcName, "26M_XTL, 32K7_XTL"));
    }
}
/******************************************************************************
 * _Pwm3SrcSet
 ******************************************************************************/
LOCAL void _Pwm3SrcSet (const char *SrcName)
{
    GR_CLK_CTL1_U *clk_cfg = (GR_CLK_CTL1_U *) (GR_CLK_CFG1);

    if (strcmp ("26M_XTL", SrcName) == 0)
    {
        clk_cfg->mBits.src_sel_pwm3 = 0;
    }
    else if (strcmp ("32K7_XTL", SrcName) == 0)
    {
        clk_cfg->mBits.src_sel_pwm3 = 1;
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Pwm3SrcSet", SrcName, "26M_XTL, 32K7_XTL"));
    }
}

/******************************************************************************
 * _Pwm4SrcSet
 ******************************************************************************/
LOCAL void _Pwm4SrcSet (const char *SrcName)
{
    GR_CLK_CTL1_U *clk_cfg = (GR_CLK_CTL1_U *) (GR_CLK_CFG1);

    if (strcmp ("26M_XTL", SrcName) == 0)
    {
        clk_cfg->mBits.src_sel_pwm4 = 0;
    }
    else if (strcmp ("32K7_XTL", SrcName) == 0)
    {
        clk_cfg->mBits.src_sel_pwm4 = 1;
    }
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     (err_string, "_Pwm4SrcSet", SrcName, "26M_XTL, 32K7_XTL"));
    }
}

/******************************************************************************
 * _SpiSrcSet
 ******************************************************************************/
LOCAL void _SpiSrcSet (const char *SrcName)
{
    GR_CLK_CTL0_U *clk_cfg = (GR_CLK_CTL0_U *) (GR_CLK_CFG0);

    if(SC6800H == CHIP_GetChipType())
    {
        if (strcmp ("26M_XTL", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_spi = 0;
        }
        else if (strcmp ("MPLLDIV4", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_spi = 1;
        }
        else
        {
            SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                         (err_string, "_SpiSrcSet", SrcName, "26M_XTL, MPLLDIV4"));
        }
    }
    else
    {
        if (strcmp ("26M_XTL", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_spi = 0;
        }
        else if (strcmp ("MPLLDIV8", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_spi = 1;
        }
        else
        {
            SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                         (err_string, "_SpiSrcSet", SrcName, "26M_XTL, MPLLDIV8"));
        }
    }
}

/******************************************************************************
 * _SpiSrcDivSet
 ******************************************************************************/
LOCAL void _SpiSrcDivSet (int Div)
{
    GR_CLK_CTL0_U *clk_cfg = (GR_CLK_CTL0_U *) (GR_CLK_CFG0);

    SCI_PASSERT ( (7 >= Div), ("_SpiSrcDivSet exceeds the max_value."));/*assert verified*/

    clk_cfg->mBits.div_cfg_spi = Div;
}

/******************************************************************************
 * _AuxSrcSet
 ******************************************************************************/
LOCAL void _AuxSrcSet (const char *SrcName)
{
    GR_CLK_CTL1_U *clk_cfg = (GR_CLK_CTL1_U *) (GR_CLK_CFG1);

    if(SC6800H == CHIP_GetChipType())
    {
        if (strcmp ("32K_XTL", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_aux = 0;
        }
        else if (strcmp ("26M_XTL", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_aux = 2;
        }
        else if (strcmp ("BPLLDIV10", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_aux = 3;
        }
        else
        {
            SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                         (err_string, "_AuxSrcSet", SrcName, "32K_XTL, 26M_XTL, BPLLDIV10"));
        }
    }
    else
    {
        if (strcmp ("32K_XTL", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_aux = 0;
        }
        else if (strcmp ("26M_XTL", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_aux = 1;
        }
        else if (strcmp ("MPLLDIV15", SrcName) == 0)
        {
            clk_cfg->mBits.src_sel_aux = 2;
        }
        else
        {
            SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                         (err_string, "_AuxSrcSet", SrcName, "32K_XTL, 26M_XTL, MPLLDIV15"));
        }
    }
}

/******************************************************************************
 * _SpiSrcDivSet
 ******************************************************************************/
LOCAL void _AuxSrcDivSet (int Div)
{
    GR_CLK_CTL1_U *clk_cfg = (GR_CLK_CTL1_U *) (GR_CLK_CFG1);

    SCI_PASSERT ( (3 >= Div), ("_AuxSrcDivSet exceeds the max_value."));/*assert verified*/

    clk_cfg->mBits.div_cfg_aux = Div;
}

/******************************************************************************
 * _RtcXtlEnable
 ******************************************************************************/
LOCAL void _RtcXtlEnable (void)
{
    /* Enable 32.768K RTC XTL */
    //CLOCK_LOG_TRACE:"[_RtcxtlEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_678_112_2_17_23_3_55_723,(uint8*)"");

    CHIP_REG_GET (GR_MISC1) |= MISC1_MCU_XTL_FORCE_EN;
}

/******************************************************************************
 * _RtcXtlDisable
 ******************************************************************************/
LOCAL void _RtcXtlDisable (void)
{
    /* Disable 32.768K RTC XTL */
    //CLOCK_LOG_TRACE:"[_RtcxtlDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_688_112_2_17_23_3_55_724,(uint8*)"");

    CHIP_REG_GET (GR_MISC1) &= ~MISC1_MCU_XTL_FORCE_EN;
}

/******************************************************************************
 * _MpllEnable
 ******************************************************************************/
LOCAL void _MpllEnable (void)
{
    /* Enable MPLL */
    //CLOCK_LOG_TRACE:"[_MpllEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_698_112_2_17_23_3_55_725,(uint8*)"");

    CHIP_REG_GET (GR_MISC1) |= MISC1_MCU_MPLL_FORCE_PD_EN;
    if(SC6800H == CHIP_GetChipType())
    {
        CHIP_REG_GET (GR_MISC1) |= MISC1_BPLL_SEL;
    }
}

/******************************************************************************
 * _MpllDisable
 ******************************************************************************/
LOCAL void _MpllDisable (void)
{
    /* Disable MPLL */
    //CLOCK_LOG_TRACE:"[_MpllDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_712_112_2_17_23_3_55_726,(uint8*)"");

    CHIP_REG_GET (GR_MISC1) &= ~MISC1_MCU_MPLL_FORCE_PD_EN;
    if(SC6800H == CHIP_GetChipType())
    {
        CHIP_REG_GET (GR_MISC1) |= MISC1_BPLL_SEL;
    }
}

/******************************************************************************
 * _BpllEnable
 ******************************************************************************/
LOCAL void _BpllEnable (void)
{
    if(SC6800H == CHIP_GetChipType())
    {
        /* Enable BPLL */
        //CLOCK_LOG_TRACE:"[_BpllEnable]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_728_112_2_17_23_3_55_727,(uint8*)"");

        // Clear the protect control
        CHIP_REG_SET (GR_MCU_PORT,  PLL_MCU_PROT_VALUE);

        CHIP_REG_AND (GR_MISC1, ~MISC1_MCU_BPLL_FORCE_PD_EN);

        // Wait 5ms
        OS_TickDelay (5);    
        
        CHIP_REG_OR (GR_MISC1, MISC1_BPLL_CONT_DONE);
            
        // Set the protect control
        CHIP_REG_SET (GR_MCU_PORT,  0);    
    }
}

/******************************************************************************
 * _DcamSrcSet
 ******************************************************************************/
LOCAL void _BpllDisable (void)
{
#if 0
    if(SC6800H == CHIP_GetChipType())
    {
        /* Disable BPLL */
        //CLOCK_LOG_TRACE:"[_BpllDisable]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_753_112_2_17_23_3_55_728,(uint8*)"");

        // Clear the protect control
        CHIP_REG_SET (GR_MCU_PORT,  PLL_MCU_PROT_VALUE);
        
        CHIP_REG_OR (GR_MISC1, MISC1_MCU_BPLL_FORCE_PD_EN);

        // Set the protect control
        CHIP_REG_SET (GR_MCU_PORT,  0);

        // Wait 5ms
        OS_TickDelay (5);    
    }
#endif
}

/******************************************************************************
 * _26MXtlEnable
 ******************************************************************************/
PUBLIC void _26MXtlEnable (void)
{
    /* Enable 26M_XTL XTL */
    //CLOCK_LOG_TRACE:"[_26MXtlEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_774_112_2_17_23_3_55_729,(uint8*)"");

}

/******************************************************************************
 * _26MXtlDisable
 ******************************************************************************/
LOCAL void _26MXtlDisable (void)
{
    /* Disable 26M_XTL XTL */
    //CLOCK_LOG_TRACE:"[_26MXtlEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_783_112_2_17_23_3_55_730,(uint8*)"");

}

/******************************************************************************
 * _WdgEnable
 ******************************************************************************/
LOCAL void _WdgEnable (void)
{
    /* Enable watchdog apb clock */
    //CLOCK_LOG_TRACE:"[_WdgEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_792_112_2_17_23_3_55_731,(uint8*)"");

    CHIP_REG_OR (GR_GEN0,  GEN0_WDG_EN);
    CHIP_REG_OR (GR_GEN0,  GEN0_WDG_RTC_EN);
}

/******************************************************************************
 * _WdgDisable
 ******************************************************************************/
LOCAL void _WdgDisable (void)
{
    /* Disable watchdog apb clock */
    //CLOCK_LOG_TRACE:"[_WdgDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_803_112_2_17_23_3_55_732,(uint8*)"");

    CHIP_REG_AND (GR_GEN0,  ~GEN0_WDG_EN);
}

/******************************************************************************
 * _TimerEnable
 ******************************************************************************/
LOCAL void _TimerEnable (void)
{
    /* Enable timer apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_TimerEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_813_112_2_17_23_3_55_733,(uint8*)"");

    CHIP_REG_OR (GR_GEN0,  GEN0_TIMER_EN);
    CHIP_REG_OR (GR_GEN0,  GEN0_TMR_RTC_EN);
}

/******************************************************************************
 * _TimerDisable
 ******************************************************************************/
LOCAL void _TimerDisable (void)
{
    /* Disable timer apb clock */
    //CLOCK_LOG_TRACE:"[_TimerDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_824_112_2_17_23_3_55_734,(uint8*)"");

    CHIP_REG_AND (GR_GEN0,  ~GEN0_TIMER_EN);
}

/******************************************************************************
 * _RtcEnable
 ******************************************************************************/
LOCAL void _RtcEnable (void)
{
    /* Enable rtc apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_RtcEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_834_112_2_17_23_3_55_735,(uint8*)"");

    CHIP_REG_OR (GR_GEN0,  GEN0_RTC_EN);
    CHIP_REG_OR (GR_GEN0,  GEN0_RTC_RTC_EN);
}

/******************************************************************************
 * _RtcDisable
 ******************************************************************************/
LOCAL void _RtcDisable (void)
{
    /* Disable rtc apb clock */
    //CLOCK_LOG_TRACE:"[_RtcDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_845_112_2_17_23_3_55_736,(uint8*)"");

    CHIP_REG_AND (GR_GEN0,  ~GEN0_RTC_EN);
}

/******************************************************************************
 * _KpdEnable
 ******************************************************************************/
LOCAL void _KpdEnable (void)
{
    /* Enable keypad apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_KpdEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_855_112_2_17_23_3_55_737,(uint8*)"");

    CHIP_REG_OR (GR_GEN0,  GEN0_KPD_EN);
    CHIP_REG_OR (GR_GEN0,  GEN0_KPD_RTC_EN);
}

/******************************************************************************
 * _IIC0Enable
 ******************************************************************************/
LOCAL void _IIC0Enable (void)
{
    /* Enable keypad apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_IIC0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_866_112_2_17_23_3_55_738,(uint8*)"");

    CHIP_REG_OR (GR_GEN1,  GEN1_I2C_EN);
}

/******************************************************************************
 * _IIC0Disable
 ******************************************************************************/
LOCAL void _IIC0Disable (void)
{
    /* Enable keypad apb and rtc clock */
    //CLOCK_LOG_TRACE:"[_IIC0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_876_112_2_17_23_3_55_739,(uint8*)"");

    CHIP_REG_AND (GR_GEN1,  ~GEN1_I2C_EN);
}


/******************************************************************************
 * _IIC0Enable
 ******************************************************************************/
LOCAL void _IIC1Enable (void)
{
    //for 68h1 chip, only one i2c controller
    if(SC6800H != CHIP_GetChipType())
    {
        /* Enable keypad apb and rtc clock */
        //CLOCK_LOG_TRACE:"[_IIC0Enable]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_890_112_2_17_23_3_55_740,(uint8*)"");

        CHIP_REG_OR (GR_GEN1,  GEN1_I2C1_EN);
    }
}

/******************************************************************************
 * _IIC0Disable
 ******************************************************************************/
LOCAL void _IIC1Disable (void)
{
    if(SC6800H != CHIP_GetChipType())
    {
        /* Enable keypad apb and rtc clock */
        //CLOCK_LOG_TRACE:"[_IIC0Enable]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_903_112_2_17_23_3_55_741,(uint8*)"");

        CHIP_REG_AND (GR_GEN1,  ~GEN1_I2C1_EN);
    }
}

/******************************************************************************
 * _KpdDisable
 ******************************************************************************/
LOCAL void _KpdDisable (void)
{
    /* Disable keypad apb clock */
    //CLOCK_LOG_TRACE:"[_KpdDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_914_112_2_17_23_3_55_742,(uint8*)"");

    CHIP_REG_AND (GR_GEN0,  ~GEN0_KPD_EN);
    CHIP_REG_AND (GR_GEN0,  ~GEN0_KPD_RTC_EN);
}

/******************************************************************************
 * _SyscntEnable
 ******************************************************************************/
LOCAL void _SyscntEnable (void)
{
    /* Enable Syscnt apb clock */
    //CLOCK_LOG_TRACE:"[_SyscntEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_925_112_2_17_23_3_55_743,(uint8*)"");

    CHIP_REG_OR (GR_GEN0,  GEN0_SYST_EN);
    CHIP_REG_OR (GR_GEN0,  GEN0_SYST_RTC_EN);
}

/******************************************************************************
 * _SyscntDisable
 ******************************************************************************/
LOCAL void _SyscntDisable (void)
{
    /* Disable Syscnt apb clock */
    //CLOCK_LOG_TRACE:"[_SyscntDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_936_112_2_17_23_3_55_744,(uint8*)"");

    CHIP_REG_AND (GR_GEN0,  ~GEN0_SYST_EN);
}

/******************************************************************************
 * _TpcEnable
 ******************************************************************************/
LOCAL void _TpcEnable (void)
{
    /* Enable Tpc apb clock */
    //CLOCK_LOG_TRACE:"[_TpcEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_946_112_2_17_23_3_55_745,(uint8*)"");

    CHIP_REG_OR (GR_GEN0,  GEN0_TPC_EN);
    CHIP_REG_OR (GR_GEN0,  GEN0_TPC_RTC_EN);
}

/******************************************************************************
 * _TpcDisable
 ******************************************************************************/
LOCAL void _TpcDisable (void)
{
    /* Disable Tpc apb clock */
    //CLOCK_LOG_TRACE:"[_TpcDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_957_112_2_17_23_3_55_746,(uint8*)"");

    CHIP_REG_AND (GR_GEN0,  ~GEN0_TPC_EN);
}

/******************************************************************************
 * _AdcEnable
 ******************************************************************************/
LOCAL void _AdcEnable (void)
{
    /* Enable Adc apb clock */
    //CLOCK_LOG_TRACE:"[_TpcEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_967_112_2_17_23_3_55_747,(uint8*)"");

    CHIP_REG_OR (GR_GEN0,  GEN0_ADC_EN);
}

/******************************************************************************
 * _AdcDisable
 ******************************************************************************/
LOCAL void _AdcDisable (void)
{
    /* Disable Adc apb clock */
    //CLOCK_LOG_TRACE:"[_TpcDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_977_112_2_17_23_3_55_748,(uint8*)"");

    CHIP_REG_AND (GR_GEN0,  ~GEN0_ADC_EN);
}


/******************************************************************************
 * _AdcDisable
 ******************************************************************************/
LOCAL void _AdcReset (void)
{
    //CLOCK_LOG_TRACE:"[_AdcReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_987_112_2_17_23_3_55_749,(uint8*)"");

    CHIP_REG_AND (GR_SOFT_RST,  ~SWRST_ADC_RST);

    OS_TickDelay (2);

    CHIP_REG_OR (GR_SOFT_RST,  SWRST_ADC_RST);

    OS_TickDelay (2);

    CHIP_REG_AND (GR_SOFT_RST,  ~SWRST_ADC_RST);
}

/******************************************************************************
 * _GpioEnable
 ******************************************************************************/
LOCAL void _GpioEnable (void)
{
    /* Enable Gpio apb clock */
    //CLOCK_LOG_TRACE:"[_GpioEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1005_112_2_17_23_3_55_750,(uint8*)"");

    CHIP_REG_OR (GR_GEN0,  GEN0_GPIO_EN);
    CHIP_REG_OR (GR_GEN0,  GEN0_GPIO_RTC_EN);
}

/******************************************************************************
 * _GpioDisable
 ******************************************************************************/
LOCAL void _GpioDisable (void)
{
    /* Disable Gpio apb clock */
    //CLOCK_LOG_TRACE:"[_GpioDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1016_112_2_17_23_3_55_751,(uint8*)"");

    CHIP_REG_AND (GR_GEN0,  ~GEN0_GPIO_EN);
}

/******************************************************************************
 * _Sim0Enable
 ******************************************************************************/
LOCAL void _Sim0Enable (void)
{
    /* Enable Sim0 apb clock */
    //CLOCK_LOG_TRACE:"[_Sim0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1026_112_2_17_23_3_55_752,(uint8*)"");

    CHIP_REG_OR (GR_GEN1,  GEN1_SIM0_EN);
}

/******************************************************************************
 * _Sim0Disable
 ******************************************************************************/
LOCAL void _Sim0Disable (void)
{
    /* Disable Sim0 apb clock */
    //CLOCK_LOG_TRACE:"[_Sim0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1036_112_2_17_23_3_55_753,(uint8*)"");

    CHIP_REG_AND (GR_GEN1,  ~GEN1_SIM0_EN);
}

/******************************************************************************
 * _Sim1Enable
 ******************************************************************************/
LOCAL void _Sim1Enable (void)
{
    /* Enable Sim1 apb clock */
    //CLOCK_LOG_TRACE:"[_Sim1Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1046_112_2_17_23_3_55_754,(uint8*)"");

    CHIP_REG_OR (GR_GEN1,  GEN1_SIM1_EN);
}

/******************************************************************************
 * _Sim1Disable
 ******************************************************************************/
LOCAL void _Sim1Disable (void)
{
    /* Disable Sim1 apb clock */
    //CLOCK_LOG_TRACE:"[_Sim1Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1056_112_2_17_23_3_56_755,(uint8*)"");

    CHIP_REG_AND (GR_GEN1,  ~GEN1_SIM1_EN);
}

/******************************************************************************
 * _GeaEnable
 ******************************************************************************/
LOCAL void _GeaEnable (void)
{
    /* Enable Gea apb clock */
    //CLOCK_LOG_TRACE:"[_GeaEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1066_112_2_17_23_3_56_756,(uint8*)"");

    CHIP_REG_OR (GR_GEN1,  GEN1_GEA_EN);
}

/******************************************************************************
 * _GeaDisable
 ******************************************************************************/
LOCAL void _GeaDisable (void)
{
    /* Disable Gea apb clock */
    //CLOCK_LOG_TRACE:"[_GeaDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1076_112_2_17_23_3_56_757,(uint8*)"");

    CHIP_REG_AND (GR_GEN1,  ~GEN1_GEA_EN);
}

/******************************************************************************
 * _EfuseEnable
 ******************************************************************************/
LOCAL void _EfuseEnable (void)
{
    /* Enable Efuse apb clock */
    //CLOCK_LOG_TRACE:"[_EfuseEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1086_112_2_17_23_3_56_758,(uint8*)"");

    CHIP_REG_OR (GR_GEN1,  GEN1_EFUSE_EN);
}

/******************************************************************************
 * _EfuseDisable
 ******************************************************************************/
LOCAL void _EfuseDisable (void)
{
    /* Disable Efuse apb clock */
    //CLOCK_LOG_TRACE:"[_EfuseDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1096_112_2_17_23_3_56_759,(uint8*)"");

    CHIP_REG_AND (GR_GEN1,  ~GEN1_EFUSE_EN);
}

/******************************************************************************
 * _Spi0Enable
 ******************************************************************************/
LOCAL void _Spi0Enable (void)
{
    /* Enable Spi0 apb clock */
    //CLOCK_LOG_TRACE:"[_SpiEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1106_112_2_17_23_3_56_760,(uint8*)"");

    CHIP_REG_OR (GR_GEN1,  GEN1_SPI_EN);
}

/******************************************************************************
 * _Spi0Disable
 ******************************************************************************/
LOCAL void _Spi0Disable (void)
{
    /* Disable Spi0 apb clock */
    //CLOCK_LOG_TRACE:"[_SpiDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1116_112_2_17_23_3_56_761,(uint8*)"");

    CHIP_REG_AND (GR_GEN1,  ~GEN1_SPI_EN);
}

/******************************************************************************
 * _I2s0Enable
 ******************************************************************************/
LOCAL void _I2s0Enable (void)
{
    /* Enable I2s0 apb clock */
    //CLOCK_LOG_TRACE:"[_I2s0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1126_112_2_17_23_3_56_762,(uint8*)"");

    CHIP_REG_AND (GR_CLK_CFG0, (~0x1fff));
    CHIP_REG_OR (GR_GEN1,  GEN1_IIS_EN);
}

/******************************************************************************
 * _I2s0Disable
 ******************************************************************************/
LOCAL void _I2s0Disable (void)
{
    /* Disable I2s0 apb clock */
    //CLOCK_LOG_TRACE:"[_I2s0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1137_112_2_17_23_3_56_763,(uint8*)"");

    CHIP_REG_AND (GR_GEN1,  ~GEN1_IIS_EN);
}

/******************************************************************************
 * _Uart0Enable
 ******************************************************************************/
LOCAL void _Uart0Enable (void)
{
    /* Enable Uart0 apb clock */
    //CLOCK_LOG_TRACE:"[_Uart0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1147_112_2_17_23_3_56_764,(uint8*)"");

    CHIP_REG_OR (GR_GEN1,  GEN1_UART0_EN);
}

/******************************************************************************
 * _Uart0Disable
 ******************************************************************************/
LOCAL void _Uart0Disable (void)
{
    /* Disable Uart0 apb clock */
    //CLOCK_LOG_TRACE:"[_Uart0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1157_112_2_17_23_3_56_765,(uint8*)"");

    CHIP_REG_AND (GR_GEN1,  ~GEN1_UART0_EN);
}

/******************************************************************************
 * _Pwm0Enable
 ******************************************************************************/
LOCAL void _Pwm0Enable (void)
{
    /* Enable Pwm0 apb clock */
    //CLOCK_LOG_TRACE:"[_Pwm0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1167_112_2_17_23_3_56_766,(uint8*)"");

    CHIP_REG_OR (GR_GEN1,  GEN1_PWM0_EN);
}

/******************************************************************************
 * _Pwm0Disable
 ******************************************************************************/
LOCAL void _Pwm0Disable (void)
{
    /* Disable Pwm0 apb clock */
    //CLOCK_LOG_TRACE:"[_Pwm0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1177_112_2_17_23_3_56_767,(uint8*)"");

    CHIP_REG_AND (GR_GEN1,  ~GEN1_PWM0_EN);
}

/******************************************************************************
 * _Pwm1Enable
 ******************************************************************************/
LOCAL void _Pwm1Enable (void)
{
    /* Enable Pwm1 apb clock */
    //CLOCK_LOG_TRACE:"[_Pwm1Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1187_112_2_17_23_3_56_768,(uint8*)"");

    CHIP_REG_OR (GR_GEN1,  GEN1_PWM1_EN);
}

/******************************************************************************
 * _Pwm1Disable
 ******************************************************************************/
LOCAL void _Pwm1Disable (void)
{
    /* Disable Pwm1 apb clock */
    //CLOCK_LOG_TRACE:"[_Pwm1Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1197_112_2_17_23_3_56_769,(uint8*)"");

    CHIP_REG_AND (GR_GEN1,  ~GEN1_PWM1_EN);
}

/******************************************************************************
 * _Pwm2Enable
 ******************************************************************************/
LOCAL void _Pwm2Enable (void)
{
    /* Enable Pwm2 apb clock */
    //CLOCK_LOG_TRACE:"[_Pwm2Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1207_112_2_17_23_3_56_770,(uint8*)"");

    CHIP_REG_OR (GR_GEN1,  GEN1_PWM2_EN);
}

/******************************************************************************
 * _Pwm2Disable
 ******************************************************************************/
LOCAL void _Pwm2Disable (void)
{
    /* Disable Pwm2 apb clock */
    //CLOCK_LOG_TRACE:"[_Pwm2Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1217_112_2_17_23_3_56_771,(uint8*)"");

    CHIP_REG_AND (GR_GEN1,  ~GEN1_PWM2_EN);
}

/******************************************************************************
 * _Pwm3Enable
 ******************************************************************************/
LOCAL void _Pwm3Enable (void)
{
    /* Enable Pwm3 apb clock */
    //CLOCK_LOG_TRACE:"[_Pwm3Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1227_112_2_17_23_3_56_772,(uint8*)"");

    CHIP_REG_OR (GR_GEN1,  GEN1_PWM3_EN);
}

/******************************************************************************
 * _Pwm3Disable
 ******************************************************************************/
LOCAL void _Pwm3Disable (void)
{
    /* Disable Pwm3 apb clock */
    //CLOCK_LOG_TRACE:"[_Pwm3Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1237_112_2_17_23_3_56_773,(uint8*)"");

    CHIP_REG_AND (GR_GEN1,  ~GEN1_PWM3_EN);
}

/******************************************************************************
 * _Pwm4Enable
 ******************************************************************************/
LOCAL void _Pwm4Enable (void)
{
    /* Enable Pwm4 apb clock */
    //CLOCK_LOG_TRACE:"[_Pwm4Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1247_112_2_17_23_3_56_774,(uint8*)"");

    CHIP_REG_OR (GR_GEN1,  GEN1_PWM4_EN);
}

/******************************************************************************
 * _Pwm4Disable
 ******************************************************************************/
LOCAL void _Pwm4Disable (void)
{
    /* Disable Pwm4 apb clock */
    //CLOCK_LOG_TRACE:"[_Pwm4Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1257_112_2_17_23_3_56_775,(uint8*)"");

    CHIP_REG_AND (GR_GEN1,  ~GEN1_PWM4_EN);
}

/******************************************************************************
 * _Aux0Enable
 ******************************************************************************/
LOCAL void _Aux0Enable (void)
{
    /* Enable Aux0 apb clock */
    //CLOCK_LOG_TRACE:"[_Aux0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1267_112_2_17_23_3_56_776,(uint8*)"");

    CHIP_REG_OR (GR_GEN1,  GEN1_AUX_EN);
}

/******************************************************************************
 * _Aux0Disable
 ******************************************************************************/
LOCAL void _Aux0Disable (void)
{
    /* Disable Aux0 apb clock */
    //CLOCK_LOG_TRACE:"[_Aux0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1277_112_2_17_23_3_56_777,(uint8*)"");

    CHIP_REG_AND (GR_GEN1,  ~GEN1_AUX_EN);
}

/******************************************************************************
 * _CcirEnable
 ******************************************************************************/
LOCAL void _CcirEnable (void)
{
    /* Enable Ccir apb clock */
    //CLOCK_LOG_TRACE:"[_CcirEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1287_112_2_17_23_3_56_778,(uint8*)"");

    CHIP_REG_OR (AHB_CTL0,  AHB_CTL0_CCIR_EN);
}

/******************************************************************************
 * _CcirDisable
 ******************************************************************************/
LOCAL void _CcirDisable (void)
{
    /* Disable Ccir apb clock */
    //CLOCK_LOG_TRACE:"[_CcirDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1297_112_2_17_23_3_56_779,(uint8*)"");

    CHIP_REG_AND (AHB_CTL0,  ~AHB_CTL0_CCIR_EN);
}

/******************************************************************************
 * _CcirReset
 ******************************************************************************/
LOCAL void _CcirReset (void)
{
    AHB_SOFT_RST_U *ahb_rst = (AHB_SOFT_RST_U *) (AHB_SOFT_RST);

    //CLOCK_LOG_TRACE:"[_CcirReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1308_112_2_17_23_3_56_780,(uint8*)"");

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
 * _DcamEnable
 ******************************************************************************/
LOCAL void _DcamEnable (void)
{
    /* Enable Dcam apb clock */
    //CLOCK_LOG_TRACE:"[_DcamEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1325_112_2_17_23_3_56_781,(uint8*)"");

    CHIP_REG_OR (AHB_CTL0,  AHB_CTL0_DCAM_EN);
}

/******************************************************************************
 * _DcamDisable
 ******************************************************************************/
LOCAL void _DcamDisable (void)
{
    /* Disable Dcam apb clock */
    //CLOCK_LOG_TRACE:"[_DcamDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1335_112_2_17_23_3_56_782,(uint8*)"");

    CHIP_REG_AND (AHB_CTL0,  ~AHB_CTL0_DCAM_EN);
}

/******************************************************************************
 * _DcamReset
 ******************************************************************************/
LOCAL void _DcamReset (void)
{
    AHB_SOFT_RST_U *ahb_rst = (AHB_SOFT_RST_U *) (AHB_SOFT_RST);

    //CLOCK_LOG_TRACE:"[_DcamReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1346_112_2_17_23_3_56_783,(uint8*)"");

    /* Enable DMA clock */
    ahb_rst->mBits.dcam_rst = SCI_FALSE;
    /* Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.dcam_rst = SCI_TRUE;
    /*Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.dcam_rst = SCI_FALSE;
}

/******************************************************************************
 * _Sdio0Enable
 ******************************************************************************/
LOCAL void _Sdio0Enable (void)
{
    /* Enable Sdio apb clock */
    //CLOCK_LOG_TRACE:"[_SdioEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1363_112_2_17_23_3_56_784,(uint8*)"");

    CHIP_REG_OR (AHB_CTL0,  AHB_CTL0_SDIO_EN);
}

/******************************************************************************
 * _Sdio0Disable
 ******************************************************************************/
LOCAL void _Sdio0Disable (void)
{
    /* Disable Sdio apb clock */
    //CLOCK_LOG_TRACE:"[_SdioDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1373_112_2_17_23_3_56_785,(uint8*)"");

    CHIP_REG_AND (AHB_CTL0,  ~AHB_CTL0_SDIO_EN);
}

/******************************************************************************
 * _SdioReset
 ******************************************************************************/
LOCAL void _SdioReset (void)
{
    AHB_SOFT_RST_U *ahb_rst = (AHB_SOFT_RST_U *) (AHB_SOFT_RST);

    //CLOCK_LOG_TRACE:"[_SdioReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1384_112_2_17_23_3_56_786,(uint8*)"");

    /* Enable DMA clock */
    ahb_rst->mBits.sdio_rst = SCI_FALSE;
    /* Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.sdio_rst = SCI_TRUE;
    /*Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.sdio_rst = SCI_FALSE;
}

/******************************************************************************
 * _UsbEnable
 ******************************************************************************/
LOCAL void _UsbEnable (void)
{
    /* Enable Usb apb clock */
    //CLOCK_LOG_TRACE:"[_UsbEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1401_112_2_17_23_3_56_787,(uint8*)"");

    CHIP_REG_OR (AHB_CTL0,  AHB_CTL0_USBD_EN);
}

/******************************************************************************
 * _UsbDisable
 ******************************************************************************/
LOCAL void _UsbDisable (void)
{
    /* Disable Usb apb clock */
    //CLOCK_LOG_TRACE:"[_UsbDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1411_112_2_17_23_3_56_788,(uint8*)"");

    CHIP_REG_AND (AHB_CTL0,  ~AHB_CTL0_USBD_EN);
}

/******************************************************************************
 * _UsbReset
 ******************************************************************************/
LOCAL void _UsbReset (void)
{
    AHB_SOFT_RST_U *ahb_rst = (AHB_SOFT_RST_U *) (AHB_SOFT_RST);

    //CLOCK_LOG_TRACE:"[_UsbReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1422_112_2_17_23_3_56_789,(uint8*)"");

    /* Enable DMA clock */
    ahb_rst->mBits.usb_rst = SCI_FALSE;
    /* Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.usb_rst = SCI_TRUE;
    /*Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.usb_rst = SCI_FALSE;
}

/******************************************************************************
 * _DmaEnable
 ******************************************************************************/
LOCAL void _DmaEnable (void)
{
    /* Enable Dma apb clock */
    //CLOCK_LOG_TRACE:"[_DmaEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1439_112_2_17_23_3_56_790,(uint8*)"");

    CHIP_REG_OR (AHB_CTL0,  AHB_CTL0_DMA_EN);
}

/******************************************************************************
 * _DmaDisable
 ******************************************************************************/
LOCAL void _DmaDisable (void)
{
    /* Disable Dma apb clock */
    //CLOCK_LOG_TRACE:"[_DmaDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1449_112_2_17_23_3_56_791,(uint8*)"");

    CHIP_REG_AND (AHB_CTL0,  ~AHB_CTL0_DMA_EN);
}

/******************************************************************************
 * _DmaReset
 ******************************************************************************/
LOCAL void _DmaReset (void)
{
    AHB_SOFT_RST_U *ahb_rst = (AHB_SOFT_RST_U *) (AHB_SOFT_RST);

    //CLOCK_LOG_TRACE:"[_DmaReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1460_112_2_17_23_3_56_792,(uint8*)"");

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
 * _Bm0Enable
 ******************************************************************************/
LOCAL void _Bm0Enable (void)
{
    /* Enable Bm0 apb clock */
    //CLOCK_LOG_TRACE:"[_Bm0Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1478_112_2_17_23_3_56_793,(uint8*)"");

    CHIP_REG_OR (AHB_CTL0,  AHB_CTL0_BM0_EN);
}

/******************************************************************************
 * _Bm0Disable
 ******************************************************************************/
LOCAL void _Bm0Disable (void)
{
    /* Disable Bm0 apb clock */
    //CLOCK_LOG_TRACE:"[_Bm0Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1488_112_2_17_23_3_56_794,(uint8*)"");

    CHIP_REG_AND (AHB_CTL0,  ~AHB_CTL0_BM0_EN);
}

/******************************************************************************
 * _Bm0Reset
 ******************************************************************************/
LOCAL void _Bm0Reset (void)
{
    AHB_SOFT_RST_U *ahb_rst = (AHB_SOFT_RST_U *) (AHB_SOFT_RST);

    //CLOCK_LOG_TRACE:"[_Bm0Reset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1499_112_2_17_23_3_56_795,(uint8*)"");

    /* Enable DMA clock */
    ahb_rst->mBits.bm0_rst = SCI_FALSE;
    /* Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.bm0_rst = SCI_TRUE;
    /*Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.bm0_rst = SCI_FALSE;
}

/******************************************************************************
 * _Bm1Enable
 ******************************************************************************/
LOCAL void _Bm1Enable (void)
{
    /* Enable Bm1 apb clock */
    //CLOCK_LOG_TRACE:"[_Bm1Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1516_112_2_17_23_3_56_796,(uint8*)"");

    CHIP_REG_OR (AHB_CTL0,  AHB_CTL0_BM1_EN);
}

/******************************************************************************
 * _Bm1Disable
 ******************************************************************************/
LOCAL void _Bm1Disable (void)
{
    /* Disable Bm1 apb clock */
    //CLOCK_LOG_TRACE:"[_Bm1Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1526_112_2_17_23_3_56_797,(uint8*)"");

    CHIP_REG_AND (AHB_CTL0,  ~AHB_CTL0_BM1_EN);
}

/******************************************************************************
 * _Bm1Reset
 ******************************************************************************/
LOCAL void _Bm1Reset (void)
{
    AHB_SOFT_RST_U *ahb_rst = (AHB_SOFT_RST_U *) (AHB_SOFT_RST);

    //CLOCK_LOG_TRACE:"[_Bm1Reset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1537_112_2_17_23_3_56_798,(uint8*)"");

    /* Enable DMA clock */
    ahb_rst->mBits.bm1_rst = SCI_FALSE;
    /* Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.bm1_rst = SCI_TRUE;
    /*Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.bm1_rst = SCI_FALSE;
}

/******************************************************************************
 * _Bm2Enable
 ******************************************************************************/
LOCAL void _Bm2Enable (void)
{
    /* Enable Bm2 apb clock */
    //CLOCK_LOG_TRACE:"[_Bm2Enable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1554_112_2_17_23_3_56_799,(uint8*)"");

    CHIP_REG_OR (AHB_CTL0,  AHB_CTL0_BM2_EN);
}

/******************************************************************************
 * _Bm2Disable
 ******************************************************************************/
LOCAL void _Bm2Disable (void)
{
    /* Disable Bm2 apb clock */
    //CLOCK_LOG_TRACE:"[_Bm2Disable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1564_112_2_17_23_3_56_800,(uint8*)"");

    CHIP_REG_AND (AHB_CTL0,  ~AHB_CTL0_BM2_EN);
}

/******************************************************************************
 * _Bm2Reset
 ******************************************************************************/
LOCAL void _Bm2Reset (void)
{
    AHB_SOFT_RST_U *ahb_rst = (AHB_SOFT_RST_U *) (AHB_SOFT_RST);

    //CLOCK_LOG_TRACE:"[_Bm2Reset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1575_112_2_17_23_3_56_801,(uint8*)"");

    /* Enable DMA clock */
    ahb_rst->mBits.bm2_rst = SCI_FALSE;
    /* Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.bm2_rst = SCI_TRUE;
    /*Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.bm2_rst = SCI_FALSE;
}

/******************************************************************************
 * _AhbEnable
 ******************************************************************************/
LOCAL void _AhbEnable (void)
{
    //CLOCK_LOG_TRACE:"[_AhbEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1591_112_2_17_23_3_57_802,(uint8*)"");

    return;
}

/******************************************************************************
 * _AhbDisable
 ******************************************************************************/
LOCAL void _AhbDisable (void)
{
    //CLOCK_LOG_TRACE:"[_AhbDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1600_112_2_17_23_3_57_803,(uint8*)"");

    return;
}

/******************************************************************************
 * _ApbEnable
 ******************************************************************************/
LOCAL void _ApbEnable (void)
{
    //CLOCK_LOG_TRACE:"[_ApbEnable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1609_112_2_17_23_3_57_804,(uint8*)"");

    return;
}

/******************************************************************************
 * _ApbDisable
 ******************************************************************************/
LOCAL void _ApbDisable (void)
{
    //CLOCK_LOG_TRACE:"[_ApbDisable]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1618_112_2_17_23_3_57_805,(uint8*)"");

    return;
}

/******************************************************************************
 * _EmcReset
 ******************************************************************************/
LOCAL void _EmcReset (void)
{
    AHB_SOFT_RST_U *ahb_rst = (AHB_SOFT_RST_U *) (AHB_SOFT_RST);

    //CLOCK_LOG_TRACE:"[_EmcReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1629_112_2_17_23_3_57_806,(uint8*)"");

    /* Enable DMA clock */
    ahb_rst->mBits.emc_rst = SCI_FALSE;
    /* Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.emc_rst = SCI_TRUE;
    /*Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.emc_rst = SCI_FALSE;

    return;
}

/******************************************************************************
 * _NfcReset
 ******************************************************************************/
LOCAL void _NfcReset (void)
{
    AHB_SOFT_RST_U *ahb_rst = (AHB_SOFT_RST_U *) (AHB_SOFT_RST);

    //CLOCK_LOG_TRACE:"[_NfcReset]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SC6800H_CLOCK_CFG_1649_112_2_17_23_3_57_807,(uint8*)"");

    /* Enable DMA clock */
    ahb_rst->mBits.nlc_rst = SCI_FALSE;
    /* Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.nlc_rst = SCI_TRUE;
    /*Delay */
    OS_TickDelay (10);
    ahb_rst->mBits.nlc_rst = SCI_FALSE;

    return;
}

/******************************************************************************
 * Static functions prototypes
 ******************************************************************************/
LOCAL const CLOCK system_clock[] =
{
    {
        "26M_XTL",       //.Name
        26000000,
        "26M_XTL",      //.DefaultSrc
        SCI_TRUE,        //.bEnable
        _26MXtlEnable,   //.Enable
        _26MXtlDisable,  //.Disable
    },
    {
        "32K7_XTL",      //.Name
        32768,
        "32K7_XTL",      //.DefaultSrc
        SCI_TRUE,        //.bEnable
        _RtcXtlEnable,   //.Enable
        _RtcXtlDisable,  //.Disable
    },
    {
        "MPLL",          //.Name
        720000000,
        "26M_XTL",       //.DefaultSrc
        SCI_TRUE,        //.bEnable
        _MpllEnable,     //.Enable
        _MpllDisable,    //.Disable
    },
    {
        "6M5",           //.Name
        6500000,
        "26M_XTL",       //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,        //.Enable
        SCI_NULL,        //.Disable
    },
    {
        "13M",           //.Name
        13000000,
        "26M_XTL",       //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,        //.Enable
        SCI_NULL,        //.Disable
    },
    {
        "MPLLDIV4",      //.Name
        180000000,
        "MPLL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,        //.Enable
        SCI_NULL,        //.Disable
    },
    {
        "MPLLDIV8",      //.Name
        90000000,
        "MPLL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,        //.Enable
        SCI_NULL,        //.Disable
    },
    {
        "MPLLDIV15",      //.Name
        48000000,
        "MPLL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,        //.Enable
        SCI_NULL,        //.Disable
    },
    {
        "MPLLDIV16",      //.Name
        45000000,
        "MPLL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,        //.Enable
        SCI_NULL,        //.Disable
    },
    {
        "MPLLDIV60",      //.Name
        12000000,
        "MPLL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,        //.Enable
        SCI_NULL,        //.Disable
    },
};

LOCAL const DEVICE system_device[] =
{
    {
        "MCU",         //.Name
        180000000,       //.DefaultSrc
        "MCU",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,        //.bEnable
        SCI_NULL,        //.Enable
        SCI_NULL,        //.Disable
        _McuSrcSet,      //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "ARM",       //.Name
        SCI_NULL,          //.DefaultSrc
        "MCU",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,        //.bEnable
        SCI_NULL,        //.Enable
        SCI_NULL,        //.Disable
        SCI_NULL,      //.SrcSet
        _ArmSrcDivSet,
        SCI_NULL,        //.Reset
    },
    {
        "AHB",           //.Name
        SCI_NULL,           //.DefaultSrc
        "MCU",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _AhbEnable,    //.Enable
        _AhbDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        _AhbSrcDivSet,
        SCI_NULL,        //.Reset
    },
    {
        "APB",           //.Name
        26000000,       //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _ApbEnable,    //.Enable
        _ApbDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
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
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "RTCTIMER0",    //.Name
        SCI_NULL,          //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _TimerEnable,  //.Enable
        _TimerDisable, //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "RTC",          //.Name
        SCI_NULL,          //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _RtcEnable,    //.Enable
        _RtcDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
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
        SCI_NULL,
        SCI_NULL,        //.Reset
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
        SCI_NULL,
        SCI_NULL,        //.Reset
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
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "ADC",          //.Name
        6500000,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _AdcEnable,    //.Enable
        _AdcDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        _AdcReset,        //.Reset
    },
    {
        "GPIO",         //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _GpioEnable,   //.Enable
        _GpioDisable,  //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "SIM0",         //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Sim0Enable,   //.Enable
        _Sim0Disable,  //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "SIM1",         //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Sim1Enable,   //.Enable
        _Sim1Disable,  //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "GEA",          //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _GeaEnable,    //.Enable
        _GeaDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "AUDIO",    //.Name
        48000000,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_FALSE,       //.bEnable
        SCI_NULL,       //.Enable
        SCI_NULL,       //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "SPI",         //.Name
        90000000,         //.DefaultSrc
        "APB",           //.ParentDev
        1,                       //.Div
        SCI_TRUE,       //.bEnable
        _Spi0Enable,   //.Enable
        _Spi0Disable,  //.Disable
        _SpiSrcSet,       //.SrcSet
        _SpiSrcDivSet,
        SCI_NULL,        //.Reset
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
        "UART0",        //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Uart0Enable,  //.Enable
        _Uart0Disable, //.Disable
        _UartSrcSet,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "PWM0",         //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Pwm0Enable,   //.Enable
        _Pwm0Disable,  //.Disable
        _Pwm0SrcSet,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "PWM1",         //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Pwm1Enable,   //.Enable
        _Pwm1Disable,  //.Disable
        _Pwm1SrcSet,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "PWM2",         //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Pwm2Enable,   //.Enable
        _Pwm2Disable,  //.Disable
        _Pwm2SrcSet,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "PWM3",         //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Pwm3Enable,   //.Enable
        _Pwm3Disable,  //.Disable
        _Pwm3SrcSet,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "PWM4",         //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Pwm4Enable,   //.Enable
        _Pwm4Disable,  //.Disable
        _Pwm4SrcSet,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "AUX0",         //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Aux0Enable,   //.Enable
        _Aux0Disable,  //.Disable
        _AuxSrcSet,       //.SrcSet
        _AuxSrcDivSet,
        SCI_NULL,        //.Reset
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
        "DCAM",         //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _DcamEnable,   //.Enable
        _DcamDisable,  //.Disable
        _DcamSrcSet,       //.SrcSet
        SCI_NULL,
        _DcamReset,        //.Reset
    },
    {
        "SDIO0",        //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Sdio0Enable,  //.Enable
        _Sdio0Disable, //.Disable
        _SdioSrcSet,       //.SrcSet
        SCI_NULL,
        _SdioReset,        //.Reset
    },
    {
        "EMC",          //.Name
        180000000,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        SCI_NULL,       //.Enable
        SCI_NULL,       //.Disable
        _EmcSrcSet,       //.SrcSet
        _EmcSrcDivSet,
        _EmcReset,        //.Reset
    },
    {
        "USB",          //.Name
        48000000,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_FALSE,  //.bEnable
        _UsbEnable,    //.Enable
        _UsbDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        _UsbReset,        //.Reset
    },
    {
        "DMA",          //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _DmaEnable,    //.Enable
        _DmaDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        _DmaReset,        //.Reset
    },
    {
        "BUSMONITOR0",  //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Bm0Enable,    //.Enable
        _Bm0Disable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        _Bm0Reset,        //.Reset
    },
    {
        "BUSMONITOR1",  //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Bm1Enable,    //.Enable
        _Bm1Disable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        _Bm1Reset,        //.Reset
    },
    {
        "BUSMONITOR2",  //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Bm2Enable,    //.Enable
        _Bm2Disable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        _Bm2Reset,        //.Reset
    },
    {
        "I2C0",  //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _IIC0Enable,    //.Enable
        _IIC0Disable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "I2C1",  //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _IIC1Enable,    //.Enable
        _IIC1Disable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "NFC",  //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        SCI_NULL,    //.Enable
        SCI_NULL,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        _NfcReset,        //.Reset
    },
};

//clock define for sc6800h1 chip
LOCAL const CLOCK system_clock_68h1[] =
{
    {
        "26M_XTL",       //.Name
        26000000,
        "26M_XTL",      //.DefaultSrc
        SCI_TRUE,        //.bEnable
        _26MXtlEnable,   //.Enable
        _26MXtlDisable,  //.Disable
    },
    {
        "32K7_XTL",      //.Name
        32768,
        "32K7_XTL",      //.DefaultSrc
        SCI_TRUE,        //.bEnable
        _RtcXtlEnable,   //.Enable
        _RtcXtlDisable,  //.Disable
    },
    {
        "MPLL",          //.Name
        400000000,
        "26M_XTL",       //.DefaultSrc
        SCI_TRUE,        //.bEnable
        _MpllEnable,     //.Enable
        _MpllDisable,    //.Disable
    },
    {
        "BPLL",          //.Name
        480000000,
        "26M_XTL",       //.DefaultSrc
        SCI_FALSE,        //.bEnable
        _BpllEnable,     //.Enable
        _BpllDisable,    //.Disable
    },
    {
        "6M5",           //.Name
        6500000,
        "26M_XTL",       //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,        //.Enable
        SCI_NULL,        //.Disable
    },
    {
        "13M",           //.Name
        13000000,
        "26M_XTL",       //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,        //.Enable
        SCI_NULL,        //.Disable
    },
    {
        "MPLLDIV2",      //.Name
        200000000,
        "MPLL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,        //.Enable
        SCI_NULL,        //.Disable
    },
    {
        "MPLLDIV3",      //.Name
        133000000,
        "MPLL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,        //.Enable
        SCI_NULL,        //.Disable
    },
    {
        "MPLLDIV4",      //.Name
        100000000,
        "MPLL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,        //.Enable
        SCI_NULL,        //.Disable
    },
    {
        "MPLLDIV6",      //.Name
        66000000,
        "MPLL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,        //.Enable
        SCI_NULL,        //.Disable
    },
    {
        "MPLLDIV8",      //.Name
        50000000,
        "MPLL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,        //.Enable
        SCI_NULL,        //.Disable
    },
    {
        "MPLLDIV16",     //.Name
        25000000,
        "MPLL",          //.DefaultSrc
        SCI_TRUE,        //.bEnable
        SCI_NULL,        //.Enable
        SCI_NULL,        //.Disable
    },
    {
        "BPLLDIV10",     //.Name
        48000000,
        "BPLL",          //.DefaultSrc
        SCI_FALSE,        //.bEnable
        SCI_NULL,        //.Enable
        SCI_NULL,        //.Disable
    },
    {
        "BPLLDIV3",      //.Name
        160000000,
        "BPLL",          //.DefaultSrc
        SCI_FALSE,        //.bEnable
        SCI_NULL,        //.Enable
        SCI_NULL,        //.Disable
    },
};

LOCAL const DEVICE system_device_68h1[] =
{
    {
        "MCU",         //.Name
        200000000,       //.DefaultSrc
        "MCU",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,        //.bEnable
        SCI_NULL,        //.Enable
        SCI_NULL,        //.Disable
        _McuSrcSet,      //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "ARM",       //.Name
        SCI_NULL,          //.DefaultSrc
        "MCU",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,        //.bEnable
        SCI_NULL,        //.Enable
        SCI_NULL,        //.Disable
        SCI_NULL,      //.SrcSet
        _ArmSrcDivSet,
        SCI_NULL,        //.Reset
    },
    {
        "AHB",           //.Name
        SCI_NULL,           //.DefaultSrc
        "MCU",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _AhbEnable,    //.Enable
        _AhbDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        _AhbSrcDivSet,
        SCI_NULL,        //.Reset
    },
    {
        "APB",           //.Name
        26000000,       //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _ApbEnable,    //.Enable
        _ApbDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
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
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "RTCTIMER0",    //.Name
        SCI_NULL,          //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _TimerEnable,  //.Enable
        _TimerDisable, //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "RTC",          //.Name
        SCI_NULL,          //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _RtcEnable,    //.Enable
        _RtcDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
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
        SCI_NULL,
        SCI_NULL,        //.Reset
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
        SCI_NULL,
        SCI_NULL,        //.Reset
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
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "ADC",          //.Name
        6500000,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _AdcEnable,    //.Enable
        _AdcDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        _AdcReset,        //.Reset
    },
    {
        "GPIO",         //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _GpioEnable,   //.Enable
        _GpioDisable,  //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "SIM0",         //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Sim0Enable,   //.Enable
        _Sim0Disable,  //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "SIM1",         //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Sim1Enable,   //.Enable
        _Sim1Disable,  //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "GEA",          //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _GeaEnable,    //.Enable
        _GeaDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "EFUSE",        //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _EfuseEnable,  //.Enable
        _EfuseDisable, //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "AUDIO",    //.Name
        48000000,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_FALSE,       //.bEnable
        SCI_NULL,       //.Enable
        SCI_NULL,       //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "SPI",         //.Name
        100000000,         //.DefaultSrc
        "APB",           //.ParentDev
        1,                       //.Div
        SCI_TRUE,       //.bEnable
        _Spi0Enable,   //.Enable
        _Spi0Disable,  //.Disable
        _SpiSrcSet,       //.SrcSet
        _SpiSrcDivSet,
        SCI_NULL,        //.Reset
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
        "UART0",        //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Uart0Enable,  //.Enable
        _Uart0Disable, //.Disable
        _UartSrcSet,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "PWM0",         //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Pwm0Enable,   //.Enable
        _Pwm0Disable,  //.Disable
        _Pwm0SrcSet,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "PWM1",         //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Pwm1Enable,   //.Enable
        _Pwm1Disable,  //.Disable
        _Pwm1SrcSet,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "PWM2",         //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Pwm2Enable,   //.Enable
        _Pwm2Disable,  //.Disable
        _Pwm2SrcSet,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "PWM3",         //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Pwm3Enable,   //.Enable
        _Pwm3Disable,  //.Disable
        _Pwm3SrcSet,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "PWM4",         //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Pwm4Enable,   //.Enable
        _Pwm4Disable,  //.Disable
        _Pwm4SrcSet,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "AUX0",         //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Aux0Enable,   //.Enable
        _Aux0Disable,  //.Disable
        _AuxSrcSet,       //.SrcSet
        _AuxSrcDivSet,
        SCI_NULL,        //.Reset
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
        "DCAM",         //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _DcamEnable,   //.Enable
        _DcamDisable,  //.Disable
        _DcamSrcSet,       //.SrcSet
        SCI_NULL,
        _DcamReset,        //.Reset
    },
    {
        "SDIO0",        //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Sdio0Enable,  //.Enable
        _Sdio0Disable, //.Disable
        _SdioSrcSet,       //.SrcSet
        SCI_NULL,
        _SdioReset,        //.Reset
    },
    {
        "EMC",          //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        SCI_NULL,       //.Enable
        SCI_NULL,       //.Disable
        _EmcSrcSet,       //.SrcSet
        _EmcSrcDivSet,
        _EmcReset,        //.Reset
    },
    {
        "USB",          //.Name
        48000000,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_FALSE,  //.bEnable
        _UsbEnable,    //.Enable
        _UsbDisable,   //.Disable
        _48MSrcSet,       //.SrcSet
        SCI_NULL,
        _UsbReset,        //.Reset
    },
    {
        "DMA",          //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _DmaEnable,    //.Enable
        _DmaDisable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        _DmaReset,        //.Reset
    },
    {
        "BUSMONITOR0",  //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Bm0Enable,    //.Enable
        _Bm0Disable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        _Bm0Reset,        //.Reset
    },
    {
        "BUSMONITOR1",  //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Bm1Enable,    //.Enable
        _Bm1Disable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        _Bm1Reset,        //.Reset
    },
    {
        "BUSMONITOR2",  //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _Bm2Enable,    //.Enable
        _Bm2Disable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        _Bm2Reset,        //.Reset
    },
    {
        "I2C0",  //.Name
        SCI_NULL,         //.DefaultSrc
        "APB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        _IIC0Enable,    //.Enable
        _IIC0Disable,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        SCI_NULL,        //.Reset
    },
    {
        "NFC",  //.Name
        SCI_NULL,         //.DefaultSrc
        "AHB",           //.ParentDev
        0,                       //.Div
        SCI_TRUE,       //.bEnable
        SCI_NULL,    //.Enable
        SCI_NULL,   //.Disable
        SCI_NULL,       //.SrcSet
        SCI_NULL,
        _NfcReset,        //.Reset
    },
};

/******************************************************************************
 * Implement
 ******************************************************************************/
PUBLIC void CLOCKCFG_Get (CLOCK_CFG *thiz)
{
    SCI_ASSERT (SCI_NULL != thiz);/*assert verified*/

    if(SC6800H == CHIP_GetChipType())
    {
        thiz->Clock = (CLOCK *)system_clock_68h1;
        thiz->ClockNr = sizeof (system_clock_68h1) / sizeof (struct _CLOCK);
        thiz->Device = (DEVICE *)system_device_68h1;
        thiz->DeviceNr = sizeof (system_device_68h1) / sizeof (struct _DEVICE);
    }
    else
    {
        thiz->Clock = (CLOCK *)system_clock;
        thiz->ClockNr = sizeof (system_clock) / sizeof (struct _CLOCK);
        thiz->Device = (DEVICE *)system_device;
        thiz->DeviceNr = sizeof (system_device) / sizeof (struct _DEVICE);
    }
}

