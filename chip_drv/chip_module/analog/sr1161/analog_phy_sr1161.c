/******************************************************************************
 ** File Name:      analog_phy_v5.c                                              *
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
#include "adi_hal_internal.h"
#include "analog_drvapi.h"
#include "../analog_phy.h"
//#include "analog_reg_sr1131.h"

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

LOCAL ANA_DEV_PARAM_T s_ana_dev_param_tab[ANA_DEV_ID_MAX] = 
{
    /* id,             , step,      interval(ms) */
    {ANA_DEV_ID_LCM_BL,     1,      10},
    {ANA_DEV_ID_KPD_BL,     1,      10},
    {ANA_DEV_ID_VIBRATOR,     36,      10},
    {ANA_INVALID_VALUE,     ANA_INVALID_VALUE,      ANA_INVALID_VALUE},
    {ANA_INVALID_VALUE,     ANA_INVALID_VALUE,      ANA_INVALID_VALUE},
    {ANA_INVALID_VALUE,     ANA_INVALID_VALUE,      ANA_INVALID_VALUE},
};

LOCAL ANA_DEV_PARAM_T* _ANA_Get_Dev_Param_Handle(ANA_DEV_ID_E id)
{
    uint32 i;
    ANA_DEV_PARAM_T* ret = (ANA_DEV_PARAM_T*)ANA_INVALID_VALUE;
    for (i = 0; i < ANA_DEV_ID_MAX; i++)
    {
         if (id == s_ana_dev_param_tab[i].id)
         {
            ret = &s_ana_dev_param_tab[i];
            break;
         }
    }
    return ret;
}

LOCAL void _ANA_SetValByStep(LEVEL_INFO_T* level_info)
{
    uint32 cur_val;
    cur_val = (ANA_REG_GET(level_info->reg_addr) & level_info->mask) >> level_info->shift;
    while( level_info->value != cur_val)
    {
        if ( level_info->value > cur_val)
        {
            cur_val += level_info->step;
            if ( cur_val > level_info->value)
            {
                cur_val = level_info->value;
            }
        }
        else
        {
            /* to avoid overflow */
            if ( cur_val < level_info->step )
            {
                cur_val = level_info->value;
            }
            else
            {
                cur_val -= level_info->step;
                if ( cur_val < level_info->value )
                {
                    cur_val = level_info->value;
                }
            }
        }
        ANA_REG_MSK_OR (level_info->reg_addr, ( (cur_val << level_info->shift) & level_info->mask), level_info->mask);
        ANA_PHY_PRINT(("_ANA_SetValByStep: 0x%x, addr:0x%x, sts:0x%x", cur_val, level_info->reg_addr, ANA_REG_GET(level_info->reg_addr)));
        SCI_Sleep(level_info->interval);
    }
}

LOCAL void _ANA_SetKPDBackLight (uint32 value)
{
    uint32 cur_v;
    BOOLEAN is_off;
    ANA_DEV_PARAM_T* ana_param_ptr;
    LEVEL_INFO_T kpd_level_info;

    ana_param_ptr = _ANA_Get_Dev_Param_Handle(ANA_DEV_ID_KPD_BL);
    SCI_ASSERT(ANA_INVALID_VALUE != (uint32)ana_param_ptr);/*assert to do*/
    kpd_level_info.reg_addr = ANA_KPLED_CTRL;
    kpd_level_info.mask = KPLED_V_MSK;
    kpd_level_info.shift = KPLED_V_SHIFT;
    kpd_level_info.step = ana_param_ptr->tuning_step;  //s_kpd_tuning_step; //1;
    kpd_level_info.interval = ana_param_ptr->tuning_interval; //s_kpd_tuning_interval;
    kpd_level_info.value = value;

    if (ANA_DEV_VAL_MIN == value)
    {
        is_off = SCI_TRUE;
    }
    else
    {
        is_off = SCI_FALSE;
    }

    if ( SCI_TRUE == is_off )
    {
        ANA_REG_OR (ANA_KPLED_CTRL, KPLED_PD);
        ANA_PHY_PRINT(("_ANA_TurnOffKPDBackLight: 0x%x, sts:0x%x", cur_v, ANA_REG_GET(ANA_KPLED_CTRL)));
    }
    else
    {
        cur_v = ANA_REG_GET(ANA_KPLED_CTRL);

        if (cur_v & KPLED_PD) /* kpd led is power off, open first */
        {
            ANA_REG_MSK_OR (ANA_KPLED_CTRL, ( (0 << KPLED_V_SHIFT) &KPLED_V_MSK), KPLED_V_MSK);
            /* open kpd backlight */
            ANA_REG_AND (ANA_KPLED_CTRL, ~KPLED_PD);
        }
        ANA_PHY_PRINT(("_ANA_TuningKPDBackLight: 0x%x, sts:0x%x", cur_v, ANA_REG_GET(ANA_KPLED_CTRL)));
        _ANA_SetValByStep(&kpd_level_info);
        
    }
}


/*****************************************************************************/
//  Description:    Adjust vibrator current level
//  Author:         Jeff.li
//  Note:
/*****************************************************************************/
LOCAL void _ANA_SetVibrator (uint32 value)
{
    uint32 cur_v;
    BOOLEAN is_off;
    ANA_DEV_PARAM_T* ana_param_ptr;
    LEVEL_INFO_T vibr_level_info;

    ana_param_ptr = _ANA_Get_Dev_Param_Handle(ANA_DEV_ID_VIBRATOR);
    SCI_ASSERT(ANA_INVALID_VALUE != (uint32)ana_param_ptr);/*assert to do*/
    vibr_level_info.reg_addr = ANA_VIBR_CTRL; 
    vibr_level_info.mask = LDO_VIBR_V_MSK;
    vibr_level_info.shift = LDO_VIBR_V_SHIFT;
    vibr_level_info.step = ana_param_ptr->tuning_step;  //s_vibr_tuning_step; //1;
    vibr_level_info.interval = ana_param_ptr->tuning_interval; //s_vibr_tuning_interval;//20;
    vibr_level_info.value = value;

    if (ANA_DEV_VAL_MIN == value)
    {
    	is_off = SCI_TRUE;
    }
    else
    {
        is_off = SCI_FALSE;
    }

    if ( SCI_TRUE == is_off )
    {
        ANA_REG_OR (ANA_VIBR_CTRL, LDO_VIBR_PD);
        ANA_PHY_PRINT(("_ANA_TurnOffVIBR: sts:0x%x", ANA_REG_GET(ANA_VIBR_CTRL)));
    }
    else
    {
        cur_v = ANA_REG_GET(ANA_VIBR_CTRL);

        if (cur_v & LDO_VIBR_PD)  /* vibr is power off */
        {
            ANA_REG_MSK_OR (ANA_VIBR_CTRL, ( (0 << LDO_VIBR_V_SHIFT) &LDO_VIBR_V_MSK), LDO_VIBR_V_MSK);
            /* open vibr */
            ANA_REG_AND (ANA_VIBR_CTRL, ~ (LDO_VIBR_PD));
        }
        ANA_PHY_PRINT(("_ANA_TuningVIBR: 0x%x, sts:0x%x", cur_v, ANA_REG_GET(ANA_VIBR_CTRL)));
        _ANA_SetValByStep(&vibr_level_info);
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
    uint32 cur_brgt_val;
    uint32 lcm_onoff_sts;
    ANA_DEV_PARAM_T* ana_param_ptr;
    LEVEL_INFO_T lcm_level_info;
    BOOLEAN is_off;

    ana_param_ptr = _ANA_Get_Dev_Param_Handle(ANA_DEV_ID_LCM_BL);
    SCI_ASSERT(ANA_INVALID_VALUE != (uint32)ana_param_ptr);/*assert to do*/
    lcm_level_info.reg_addr = ANA_WHTLED_CTRL;
    lcm_level_info.mask = WHTLED_V_MSK;
    lcm_level_info.shift = WHTLED_V_SHIFT;
    lcm_level_info.step = ana_param_ptr->tuning_step;
    lcm_level_info.interval = ana_param_ptr->tuning_interval;
    //lcm_level_info.value = cur_brgt_val;

    cur_brgt_val = ( ANA_REG_GET(ANA_WHTLED_CTRL) & WHTLED_V_MSK ) >> WHTLED_V_SHIFT;
    lcm_onoff_sts = ANA_REG_GET(ANA_WHTLED_CTRL);

    
    lcm_level_info.value = value;


    if ( (lcm_onoff_sts & WHTLED_PD) == 1)  // led power down, set value directly
    {
        ANA_REG_MSK_OR (ANA_WHTLED_CTRL, value << WHTLED_V_SHIFT, WHTLED_V_MSK);
    }
    else
    {
        if(cur_brgt_val != value)
            _ANA_SetValByStep(&lcm_level_info);
    }

}

/*****************************************************************************/
//  Description:    Get LCD backlight current level
//  Author:         Jeff.li
//  Note:
/*****************************************************************************/
PUBLIC uint32 _ANA_GetLCMBrightness ()
{
    uint32 val;

    val = ANA_REG_GET (ANA_WHTLED_CTRL);

    val = (val & WHTLED_V_MSK) >> WHTLED_V_SHIFT;
    return val;
}

/*****************************************************************************/
//  Description:    Open or Close LCM backlight
//  Author:         Jeff.li
//  Note:
/*****************************************************************************/
PUBLIC void _ANA_SetLCMBackLight (uint32  value)
{
    uint32 brightness_val;
    uint32 lcm_onoff_sts;
    ANA_DEV_PARAM_T* ana_param_ptr;
    LEVEL_INFO_T lcm_level_info;
    BOOLEAN is_off;

    ana_param_ptr = _ANA_Get_Dev_Param_Handle(ANA_DEV_ID_LCM_BL);
    SCI_ASSERT(ANA_INVALID_VALUE != (uint32)ana_param_ptr);/*assert to do*/
    lcm_level_info.reg_addr = ANA_WHTLED_CTRL;
    lcm_level_info.mask = WHTLED_V_MSK;
    lcm_level_info.shift = WHTLED_V_SHIFT;
    lcm_level_info.step = ana_param_ptr->tuning_step; 
    lcm_level_info.interval = ana_param_ptr->tuning_interval;
    //lcm_level_info.value = brightness_val;

    brightness_val = ( ANA_REG_GET(ANA_WHTLED_CTRL) & WHTLED_V_MSK ) >> WHTLED_V_SHIFT;
    lcm_onoff_sts = ANA_REG_GET(ANA_WHTLED_CTRL);

    if (ANA_DEV_CLOSE == value)
    {
        ANA_PHY_PRINT(("_ANA_TurnOffLCM: value:%d, cur_brgt:%d", value, brightness_val));
        if((lcm_onoff_sts & WHTLED_PD) == 0) // if opened
        {
            lcm_level_info.value = 0;
           _ANA_SetValByStep(&lcm_level_info);

            //close lcm backlight
           ANA_REG_OR (ANA_WHTLED_CTRL, WHTLED_PD);
        }
    }
    else
    {
        // for 1131, it does not set the input value, but use the value in register, like lcm_level_info.value = brightness_val, use it?
        //lcm_level_info.value = value;
        lcm_level_info.value = brightness_val;
        
        if((lcm_onoff_sts & WHTLED_PD) == 0)    // opened, do nothing, for backlight, we just care about open or close, do not update value
        {                                                           // it should not happened, open and close should interleaved. 
            //_ANA_SetValByStep(&lcm_level_info);
        }
        else    // if closed , open it 
        {
            ANA_PHY_PRINT(("_ANA_TurnOnLCM: value:%d, cur_brgt:%d", value, brightness_val));
            /* set level value to 0 */
            ANA_REG_MSK_OR (ANA_WHTLED_CTRL, ( (0 << WHTLED_V_SHIFT) &WHTLED_V_MSK), WHTLED_V_MSK);
            //open lcm backlight
            ANA_REG_AND (ANA_WHTLED_CTRL, ~ (WHTLED_PD));
            //ANA_REG_MSK_OR (ANA_WHTLED_CTRL, ( (brightness_val << WHTLED_V_SHIFT) &WHTLED_V_MSK), WHTLED_V_MSK);
            _ANA_SetValByStep(&lcm_level_info);
        }
       
   }
}

/*****************************************************************************/
//  Description:    Control Flash Light On/Off(While-led IB4/5)
//  Author:         Jeff.li
//  Note:
/*****************************************************************************/
LOCAL void _ANA_SetFlashLight (uint32 value)
{
	return;
}

/*****************************************************************************/
//  Description:    Open or Close LCM backlight
//  Author:         Jeff.li
//  Note:
/*****************************************************************************/
PUBLIC void _ANA_SelectLCMBackLight (uint32  value)
{
   return;
}


/*****************************************************************************/
//  Description:    Initialize analog related registers
//  Author:         Wenjun.Shi
//  Note:
/*****************************************************************************/
PUBLIC void ANA_PHY_Init (void)
{
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)

#else
    //need to disable RF get into debug mode before tp init, because rf debug mode shares TP_XL pin
    ANA_REG_AND( ANA_MISC_STS0, ~RF_DEB_SPI_MCU_EN );
#endif
    //init itf module
    //ANA_ITF_Init();

}

/**---------------------------------------------------------------------------*
 **                         Configure Table                                   *
 **---------------------------------------------------------------------------*/
// LCM Back light switch configure table according to voltage
LOCAL CONST ANA_SW_T s_ana_bln_sw_tab[] =
{
    // max current value is 25 mA
    {300, 360, 0, 40},      // 3.5V  // 10mA
    {355, 370, 0, 65},      // 3.6V  // 13mA
    {365, 380, 0, 65},      // 3.7V  // 16mA
    {375, 480, 0, 65},      // 3.8V  // 20mA
    {ANA_INVALID_VALUE, ANA_INVALID_VALUE, 0, 100} // invalid
    //modify the lcd backlight to two step :1-> 3.0-3.6 (40% * 15);  2-> 3.55-4.8  (65% * 15)
};

// Analog Device information table
LOCAL ANA_DEV_T s_ana_dev_tab[ANA_DEV_ID_MAX] =
{
    {ANA_DEV_ID_LCM_BL,                     15,  0,      NULL,                        ANA_SW_INDEX_MAX,      _ANA_SetLCMBackLight},
    {ANA_DEV_ID_LCM_BRIGHTNESS,     15,     0,      (ANA_SW_T_PTR)s_ana_bln_sw_tab,   ANA_SW_INDEX_MAX,       _ANA_SetLCMBrightness}, /*lint !e605*/
    {ANA_DEV_ID_KPD_BL,                     15,      0,      NULL,               ANA_SW_INDEX_MAX,       _ANA_SetKPDBackLight},
    {ANA_DEV_ID_VIBRATOR,                   255,      0,      NULL,               ANA_SW_INDEX_MAX,       _ANA_SetVibrator},
    {ANA_DEV_ID_SD_PWR,                     1,      0,      NULL,               ANA_SW_INDEX_MAX,       _ANA_SetSDPower},
    {ANA_DEV_ID_FLASH_LIGHT,              1,      0,      NULL,               ANA_SW_INDEX_MAX,       _ANA_SetFlashLight},
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

PUBLIC uint32 ANA_GetLCMBackLightStatus (void)
{
    uint32 ret = 0;
    
    if(ANA_REG_GET(ANA_WHTLED_CTRL) & WHTLED_PD)  //bl off
    {
        ret = 0;
    }
    else  //bl on
    {
        ret = 1;
    }   

    return ret;
}

/*****************************************************************************/
//  Description:    Select Fucntion for analog device
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void ANA_PHY_SelectDevFunc (ANA_DEV_ID_E dev_id, uint32 value)
{
    switch (dev_id)
    {
    case ANA_DEV_ID_LCM_BL:
        _ANA_SelectLCMBackLight(value);
        break;

    default:
        break;
    }
}


/*****************************************************************************/
//  Description:    Set Params for analog device
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void ANA_PHY_SetDevParam (ANA_DEV_ID_E dev_id, ANA_DEV_PARAM_T* param)
{
    ANA_DEV_PARAM_T* ana_param_ptr;
    ana_param_ptr = _ANA_Get_Dev_Param_Handle(dev_id);
    SCI_ASSERT(ANA_INVALID_VALUE != (uint32)ana_param_ptr);/*assert to do*/

    ana_param_ptr->tuning_step = param->tuning_step;
    ana_param_ptr->tuning_interval = param->tuning_interval;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}

#endif  // End of analog_cfg.c
