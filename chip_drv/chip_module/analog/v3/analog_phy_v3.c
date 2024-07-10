/******************************************************************************
 ** File Name:      analog_cfg.c                                              *
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
 *****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
//#include "sci_types.h"
#include "chip_plf_export.h"
//#include "analog_reg_v3.h"
#include "analog_drvapi.h"
#include "../analog_phy.h"
//#include "adi_hal_internal.h"

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
    if (ANA_DEV_VAL_MIN == value)
    {
        ANA_REG_AND (KPLED_CTL, ~ (KPLED_PD_SET|KPLED_PD_RST));
        ANA_REG_OR (KPLED_CTL, KPLED_PD_SET);
    }
    else
    {
        value--;
        // Set Output Current
        ANA_REG_MSK_OR (KPLED_CTL, ( (value << KPLED_V_SHIFT) &KPLED_V_MSK), KPLED_V_MSK);
        // Open
        ANA_REG_AND (KPLED_CTL, ~ (KPLED_PD_SET|KPLED_PD_RST));
        ANA_REG_OR (KPLED_CTL, KPLED_PD_RST);
    }
}

/*****************************************************************************/
//  Description:    Adjust vibrator current level
//  Author:         Jeff.li
//  Note:
/*****************************************************************************/
LOCAL void _ANA_SetVibrator (uint32 value)
{
    if (ANA_DEV_VAL_MIN == value)
    {
        ANA_REG_AND (VIBR_CTL, ~ (VIBR_PD_SET|VIBR_PD_RST));
        ANA_REG_OR (VIBR_CTL, VIBR_PD_SET);
    }
    else
    {
        value--;
        // Set Output Current
        ANA_REG_MSK_OR (VIBR_CTL, ( (value << VIBR_V_SHIFT) &VIBR_V_MSK), VIBR_V_MSK);
        // Open
        ANA_REG_AND (VIBR_CTL, ~ (VIBR_PD_SET|VIBR_PD_RST));
        ANA_REG_OR (VIBR_CTL, VIBR_PD_RST);
    }
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

    if (value > ANA_DEV_VAL_MIN)
    {
        value--;
    }

    ANA_REG_MSK_OR (WHTLED_CTL, ( (value << WHTLED_V_SHIFT) &WHTLED_V_MSK), WHTLED_V_MSK);

}

/*****************************************************************************/
//  Description:    Open or Close LCM backlight
//  Author:         Jeff.li
//  Note:
/*****************************************************************************/
PUBLIC void _ANA_SetLCMBackLight (uint32  value)
{
    if (ANA_DEV_CLOSE == value)
    {
        //close lcm backlight
        ANA_REG_AND (WHTLED_CTL, ~ (WHTLED_PD_SET|WHTLED_PD_RST));
        ANA_REG_OR (WHTLED_CTL, WHTLED_PD_SET);
    }
    else
    {
        //open lcm backlight
        ANA_REG_AND (WHTLED_CTL, ~ (WHTLED_PD_SET|WHTLED_PD_RST));
        ANA_REG_OR (WHTLED_CTL, WHTLED_PD_RST);
    }
}

/*****************************************************************************/
//  Description:    Initialize analog related registers
//  Author:         Wenjun.Shi
//  Note:
/*****************************************************************************/
PUBLIC void ANA_PHY_Init (void)
{
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
    {ANA_DEV_ID_LCM_BL,                     1,  0,      NULL,                        ANA_SW_INDEX_MAX,      _ANA_SetLCMBackLight},
    {ANA_DEV_ID_LCM_BRIGHTNESS,     32,     0,      s_ana_bln_sw_tab,   ANA_SW_INDEX_MAX,       _ANA_SetLCMBrightness}, /*lint !e605*/
    {ANA_DEV_ID_KPD_BL,                     8,      0,      NULL,               ANA_SW_INDEX_MAX,       _ANA_SetKPDBackLight},
    {ANA_DEV_ID_VIBRATOR,                   8,      0,      NULL,               ANA_SW_INDEX_MAX,       _ANA_SetVibrator},
    {ANA_DEV_ID_SD_PWR,                     1,      0,      NULL,               ANA_SW_INDEX_MAX,       _ANA_SetSDPower},

    //{ANA_DEV_ID_MAX,      0,      0,      NULL,               ANA_SW_INDEX_MAX,       NULL},  // END don't need
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

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}

#endif  // End of analog_cfg.c
