/******************************************************************************
 ** File Name:      analog_phy_v0.c                                              *
 ** Author:         Jeff.Li                                                   *
 ** DATE:           24/02/2009                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    Analog interface.                                         *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 24/02/2009     Jeff.Li          Create.                                   *
 ** 11/06/2010     Jeff.Li          Create for new chip driver architecture.  *
 *****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "os_api.h"
//#include "sci_types.h"
#include "chip_plf_export.h"
#include "analog_drvapi.h"
#include "../analog_phy.h"
#include "analog_reg_v0.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
 **                         MACOR Definitions                                 *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local Function Definitions                        *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Adjust keypad backlight current level
//  Author:         Jeff.li
//  Note:
/*****************************************************************************/
LOCAL void _ANA_SetKPDBackLight (uint32 value)
{
    volatile uint32 reg_val=0;
    uint32 bright = 0;
    reg_val = REG32 (GR_ANA_CTL);

    if (ANA_DEV_VAL_MIN == value)
    {
        reg_val &= ~ANA_CTL_KPLED_PD_RST_SHIFT;
        reg_val |=  ANA_CTL_KPLED_PD_SET_SHIFT;
    }
    else
    {
        if (value > ANA_DEV_VAL_MAX)
        {
            bright = ANA_DEV_VAL_MAX;
        }
        else
        {
            bright = value;
        }

        reg_val &= ~ANA_CTL_KPLED_V_MASK;
        reg_val |= (bright&0x07) << ANA_CTL_KPLED_V_SHIFT;

        reg_val &= ~ANA_CTL_KPLED_PD_SET_SHIFT;
        reg_val |=  ANA_CTL_KPLED_PD_RST_SHIFT;
    }

    REG32 (GR_ANA_CTL) = reg_val;

}

/*****************************************************************************/
//  Description:    Adjust vibrator current level
//  Author:         Jeff.li
//  Note:
/*****************************************************************************/
LOCAL void _ANA_SetVibrator (uint32 value)
{
    volatile uint32 reg_val=0;
    uint32 bright = 0;
    reg_val = REG32 (GR_ANA_CTL);

    //SCI_TRACE_LOW:"_ANA_SetVibrator: Value=%d\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ANALOG_PHY_V0_82_112_2_17_23_2_32_28,(uint8*)"d", value);

    if (ANA_DEV_VAL_MIN == value)
    {
        reg_val &= ~ANA_CTL_VIBR_PD_RST_SHIFT;
        reg_val |=  ANA_CTL_VIBR_PD_SET_SHIFT;
    }
    else
    {
        if (value > ANA_DEV_VAL_MAX)
        {
            bright = ANA_DEV_VAL_MAX;
        }
        else
        {
            bright = value;
        }

        reg_val &=  ~ANA_CTL_VIBR_V_MASK;
        reg_val |= (bright&0x07) << ANA_CTL_VIBR_V_SHIFT;

        reg_val &= ~ANA_CTL_VIBR_PD_SET_SHIFT;
        reg_val |=  ANA_CTL_VIBR_PD_RST_SHIFT;
    }

    REG32 (GR_ANA_CTL) = reg_val;

}

/*****************************************************************************/
//  Description:    Open/Close SD power supply
//  Author:         Jeff.li
//  Note:
/*****************************************************************************/
LOCAL void _ANA_SetSDPower (uint32 value)
{
}

/*****************************************************************************/
//  Description:    Adjust LCD backlight current level
//  Author:         Jeff.li
//  Note:
/*****************************************************************************/
PUBLIC void _ANA_SetLCMBrightness (uint32  value)
{
    uint32 reg_val = 0;
    uint32 bright = 0;

    if (value > ANA_DEV_VAL_MAX)
    {
        bright = ANA_DEV_VAL_MAX;
    }
    else
    {
        bright = value;
    }

    reg_val = REG32 (GR_ANA_CTL);
    reg_val &= ~ANA_CTL_WHTLED_V_MASK;
    reg_val |= ( (bright&0x1F) << ANA_CTL_WHTLED_V_SHIFT);
    REG32 (GR_ANA_CTL) =reg_val;

}

/*****************************************************************************/
//  Description:    Open or Close LCM backlight
//  Author:         Jeff.li
//  Note:
/*****************************************************************************/
PUBLIC void _ANA_SetLCMBackLight (uint32  value)
{
    volatile  uint32 reg_val = REG32 (GR_ANA_CTL);

    if (ANA_DEV_CLOSE == value)
    {
        reg_val  &= ~ANA_CTL_WHTLED_PD_RST_SHIFT;
        reg_val  |=  ANA_CTL_WHTLED_PD_SET_SHIFT;
    }
    else
    {
        reg_val  &= ~ANA_CTL_WHTLED_PD_SET_SHIFT;
        reg_val  |=  ANA_CTL_WHTLED_PD_RST_SHIFT;
    }

    REG32 (GR_ANA_CTL) = reg_val;

}

/*****************************************************************************/
//  Description:    Initialize analog related registers
//  Author:         Wenjun.Shi
//  Note:
/*****************************************************************************/
PUBLIC void ANA_PHY_Init (void)
{
    /* power on handset detect circuit*/
    CHIP_REG_AND(GR_ANA_CTL, ~BIT_21);
}

/**---------------------------------------------------------------------------*
 **                         Configure Table                                   *
 **---------------------------------------------------------------------------*/
// LCM Back light switch configure table according to voltage
LOCAL CONST ANA_SW_T s_ana_bln_sw_tab[] =
{
    // max current value is 25 mA
    {300, 360, 0, 40},      // 3.5V  // 10mA
    {355, 370, 0, 50},      // 3.6V  // 13mA
    {365, 380, 0, 65},      // 3.7V  // 16mA
    {375, 480, 0, 80},      // 3.8V  // 20mA
    {ANA_INVALID_VALUE, ANA_INVALID_VALUE, 0, 100} // invalid
};

// Analog Device information table
LOCAL ANA_DEV_T s_ana_dev_tab[ANA_DEV_ID_MAX] =
{
    {ANA_DEV_ID_LCM_BL,            1,  0,      NULL,               ANA_SW_INDEX_MAX,   _ANA_SetLCMBackLight},
    {ANA_DEV_ID_LCM_BRIGHTNESS,    32, 0, (ANA_SW_T_PTR) s_ana_bln_sw_tab,   ANA_SW_INDEX_MAX,   _ANA_SetLCMBrightness},
    {ANA_DEV_ID_KPD_BL,            8,  0,      NULL,               ANA_SW_INDEX_MAX,   _ANA_SetKPDBackLight},
    {ANA_DEV_ID_VIBRATOR,          8,  0,      NULL,               ANA_SW_INDEX_MAX,   _ANA_SetVibrator},
    {ANA_DEV_ID_SD_PWR,            1,  0,      NULL,               ANA_SW_INDEX_MAX,   _ANA_SetSDPower},

    //{ANA_DEV_ID_MAX,             0,  0,      NULL,               ANA_SW_INDEX_MAX,   NULL},  // END don't need
};

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Get analog device information table
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC ANA_DEV_T_PTR ANA_GetDevCfgTab (void)
{
    return (ANA_DEV_T_PTR) s_ana_dev_tab;
}

PUBLIC uint32 ANA_GetLCMBackLightStatus (void)
{
    uint32 ret = 0;
    
    if(CHIP_REG_GET(GR_BOND_OPT) & WHTLED_PD_STS)  //bl off
    {
        ret = 0;
    }
    else  //bl on
    {
        ret = 1;
    }   

    return ret;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}

#endif  // End of analog_cfg.c
