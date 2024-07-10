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
//#include "analog_reg_v5.h"

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
    {ANA_INVALID_VALUE,     ANA_INVALID_VALUE,      ANA_INVALID_VALUE},
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
    kpd_level_info.reg_addr = ANA_LED_CTL1;
    kpd_level_info.mask = KPLED_V_MSK;
    kpd_level_info.shift = KPLED_V_SHIFT;
    kpd_level_info.step = ana_param_ptr->tuning_step;  //s_kpd_tuning_step; //1;
    kpd_level_info.value = value;
    kpd_level_info.interval = ana_param_ptr->tuning_interval; //s_kpd_tuning_interval;//20;
    if (ANA_DEV_VAL_MIN == value)
    {
        is_off = SCI_TRUE;
    }
    else
    {
        is_off = SCI_FALSE;
        value--;
    }
    cur_v = ANA_REG_GET(ANA_LED_CTL1);
    if ( ((cur_v & KPLED_PD) || ( 0 == (cur_v & KPLED_PD_RST) )) &&
          (SCI_FALSE == is_off)  ) /* kpd led is power off */
    {
        ANA_REG_MSK_OR (ANA_LED_CTL1, ( (0 << KPLED_V_SHIFT) &KPLED_V_MSK), KPLED_V_MSK);
        /* open kpd backlight */
        ANA_REG_AND (ANA_LED_CTL1, ~ (KPLED_PD_RST|KPLED_PD));
        ANA_REG_OR (ANA_LED_CTL1, KPLED_PD_RST);
    }
    ANA_PHY_PRINT(("_ANA_TuningKPDBackLight: 0x%x, sts:0x%x", cur_v, ANA_REG_GET(ANA_LED_CTL1)));
    _ANA_SetValByStep(&kpd_level_info);
    //if (ANA_DEV_VAL_MIN == value)
    if ( SCI_TRUE == is_off )
    {
        ANA_REG_AND (ANA_LED_CTL1, ~ (KPLED_PD_RST|KPLED_PD));
        ANA_REG_OR (ANA_LED_CTL1, KPLED_PD);
        ANA_PHY_PRINT(("_ANA_TurnOffKPDBackLight: 0x%x, sts:0x%x", cur_v, ANA_REG_GET(ANA_LED_CTL1)));
    }
    else
    {
    }

}

/*****************************************************************************/
//  Description:    Adjust vibrator current level
//  Author:         Jeff.li
//  Note:
/*****************************************************************************/
LOCAL void _ANA_SetVibrator (uint32 value)
{
    ANA_REG_SET(ANA_PROT_CTL1, 0xa1b2);
    if (ANA_DEV_VAL_MIN == value)
    {
        ANA_REG_AND (ANA_VIBR_CTL0, ~VIBR_PON);
    }
    else
    {
        /* power on by hw control flow */
        value--;
        // Ana clk eb
        ANA_REG_SET(ANA_EB_SET1, ANA_RTC_VIB_CNT_EB_SET);
        // Set Output Current
        ANA_REG_AND (ANA_VIBR_CTL1, ~VIBR_SW_EN);
        ANA_REG_MSK_OR (ANA_VIBR_CTL1, ( (VIBR_INIT_V << VIBR_INIT_V_HW_SHIFT) &VIBR_INIT_V_HW_MSK), VIBR_INIT_V_HW_MSK);
        ANA_REG_MSK_OR (ANA_VIBR_CTL1, ( (value << VIBR_STABLE_V_HW_SHIFT) &VIBR_STABLE_V_HW_MSK), VIBR_STABLE_V_HW_MSK);
        ANA_REG_MSK_OR (ANA_VIBR_CTL2, ( (VIBR_CONVERT_V << VIBR_CONVERT_V_COUNT_SHIFT) &VIBR_CONVERT_V_COUNT_MSK), VIBR_CONVERT_V_COUNT_MSK);
        // Open
        ANA_REG_OR (ANA_VIBR_CTL0, VIBR_PON);
    }
    ANA_REG_SET(ANA_PROT_CTL1, 0x0);
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
    
    ana_param_ptr = _ANA_Get_Dev_Param_Handle(ANA_DEV_ID_LCM_BL);
    SCI_ASSERT(ANA_INVALID_VALUE != (uint32)ana_param_ptr);/*assert to do*/
    lcm_level_info.reg_addr = ANA_LED_CTL0;
    lcm_level_info.mask = WHTLED_V_MSK;
    lcm_level_info.shift = WHTLED_V_SHIFT;
    lcm_level_info.step = ana_param_ptr->tuning_step;
    lcm_level_info.value = value;
    lcm_level_info.interval = ana_param_ptr->tuning_interval;
    
    cur_brgt_val = ( ANA_REG_GET(ANA_LED_CTL0) & WHTLED_V_MSK ) >> WHTLED_V_SHIFT;
    lcm_onoff_sts = ANA_REG_GET(ANA_LED_CTL0);
    
    if (value > ANA_DEV_VAL_MIN)
    {
        value--;
    }
    
    if ( lcm_onoff_sts & WHTLED_PD )
    {
        ANA_REG_MSK_OR (ANA_LED_CTL0, ( (value << WHTLED_V_SHIFT) &WHTLED_V_MSK), WHTLED_V_MSK);
    }
    else
    {

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
    val = ANA_REG_GET (ANA_LED_CTL0);
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
    ANA_DEV_PARAM_T* ana_param_ptr;
    LEVEL_INFO_T lcm_level_info;

    ana_param_ptr = _ANA_Get_Dev_Param_Handle(ANA_DEV_ID_LCM_BL);
    SCI_ASSERT(ANA_INVALID_VALUE != (uint32)ana_param_ptr);/*assert to do*/
    lcm_level_info.reg_addr = ANA_LED_CTL0;
    lcm_level_info.mask = WHTLED_V_MSK;
    lcm_level_info.shift = WHTLED_V_SHIFT;
    lcm_level_info.step = ana_param_ptr->tuning_step; 
    lcm_level_info.value = value;
    lcm_level_info.interval = ana_param_ptr->tuning_interval; 
    brightness_val = ( ANA_REG_GET(ANA_LED_CTL0) & WHTLED_V_MSK ) >> WHTLED_V_SHIFT;
    if (ANA_DEV_CLOSE == value)
    {
        ANA_PHY_PRINT(("_ANA_TurnOffLCM: value:%d, cur_brgt:%d", value, brightness_val));
         lcm_level_info.value = 1;
        _ANA_SetValByStep(&lcm_level_info);

         //close lcm backlight
        ANA_REG_AND (ANA_LED_CTL0, ~ (WHTLED_PD_RST|WHTLED_PD));
        ANA_REG_OR (ANA_LED_CTL0, WHTLED_PD);
    }
    else
    {
        ANA_PHY_PRINT(("_ANA_TurnOnLCM: value:%d, cur_brgt:%d", value, brightness_val));
        /* set level value to 0 */
        ANA_REG_MSK_OR (ANA_LED_CTL0, ( (0 << WHTLED_V_SHIFT) &WHTLED_V_MSK), WHTLED_V_MSK);
        //open lcm backlight
        ANA_REG_AND (ANA_LED_CTL0, ~ (WHTLED_PD_RST|WHTLED_PD));
        ANA_REG_OR (ANA_LED_CTL0, WHTLED_PD_RST);
        lcm_level_info.value = brightness_val;
        _ANA_SetValByStep(&lcm_level_info);
    }
}
/*****************************************************************************/
//  Description:    Control Flash Light On/Off(While-led IB4/5)
//  Author:         Jeff.li
//  Note:
/*****************************************************************************/
LOCAL void _ANA_SetFlashLight (uint32 value)
{
    if (ANA_DEV_CLOSE == value)
    {
        //close white-led IB4/5 output
        ANA_REG_OR (ANA_LED_CTL0, IB5_SW_OFF|IB4_SW_OFF);  
        ANA_REG_AND (ANA_LED_CTL0, ~(IB5_SW_ON|IB4_SW_ON));    
    }
    else
    {
        //open white-led IB4/5 output
        ANA_REG_OR (ANA_LED_CTL0, IB5_SW_ON|IB4_SW_ON);  
        ANA_REG_AND (ANA_LED_CTL0, ~(IB5_SW_OFF|IB4_SW_OFF));    
    }

}

/*****************************************************************************/
//  Description:    Open or Close LCM backlight
//  Author:         Jeff.li
//  Note:
/*****************************************************************************/
PUBLIC void _ANA_SelectLCMBackLight (uint32  value)
{
    if (ANA_LCM_BL_IB4_IB5_CLOSE == value)
    {
        //enable ib4/5 sw control
        ANA_REG_OR (ANA_LED_CTL0, IB5_SW_EN|IB4_SW_EN);  //set whtled4-5 control by software
        ANA_REG_AND (ANA_LED_CTL0, ~(IB5_SW_EN_RST|IB4_SW_EN_RST));

        ANA_REG_OR (ANA_LED_CTL0, IB5_SW_OFF|IB4_SW_OFF);  
        ANA_REG_AND (ANA_LED_CTL0, ~(IB5_SW_ON|IB4_SW_ON));    
    }
    else
    {
        //enable ib4/5 hw control
        ANA_REG_OR (ANA_LED_CTL0, IB5_SW_EN_RST|IB4_SW_EN_RST);  //set whtled4-5 control by hardware
        ANA_REG_AND (ANA_LED_CTL0, ~(IB5_SW_EN|IB4_SW_EN));
    }
}

#if 0
/*****************************************************************************/
//  Description:    Initialize itf related registers
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void ANA_ITF_Init (void)
{
    uint32 reg_value = 0;
    
    //config D-Die
    //REC_26M_BUF_PD = 0
    CHIP_REG_AND(APB_PLL_CTL3, ~REC_26M_BUF_PD);
    //analog 26M is selected as pll and top clock source, clk_26m 
    CHIP_REG_OR(APB_CLK_CFG2, (CLK26M_ANA_SEL|CLK26M_TOP_SEL));
    // disable the bypass path of FM/BRX/BTX/AUD data
    CHIP_REG_AND(APB_CLK_CFG2, ~ITF_FORCE_BYPASS_EN);
    //disable the bypass path of AUD data
    CHIP_REG_AND(APB_CLK_CFG2, ~ITF_AUD_BYPASS_EN);
    
    //config A-Die
    //disable itf force bypass
    ANA_REG_AND(ANA_MISC_CTL3,~ANA_ITF_FORCE_BYPASS_EN);
    //the transfer of audio data will through ANA_IF module
    ANA_REG_AND(ANA_ANAIF_FIFO_CTL1,~ANA_ITF_AUD_BYPASS_EN);

    //clk_itf_sel: 0:31.2MHz; 1:39MHz; 2:44.6MHz; 3:48MHz
    reg_value = CHIP_REG_GET(APB_CLK_CFG1);
    reg_value &= ~CLK_ITF_SEL_ARM_MSK;
    reg_value |= (0x3 << CLK_ITF_SEL_ARM_SHIFT);
    CHIP_REG_SET(APB_CLK_CFG1, reg_value);

    //force enable clk_itf and clk_26m_itf
    CHIP_REG_OR(APB_PERI_CTL0, (CLK_26M_ITF_FORCE_EN|CLK_ITF_FORCE_EN));
    //enable 26M clock and CLK_ITF clock of ANA_IF module
    ANA_REG_OR(ANA_ANAIF_CLK_EN_SET0,(CLK_ANAIF_ITF_EN_SET|CLK_ANAIF_26M_EN_SET));
    
    //transfer 26Mhz clock directly from D-die to A-die
    //set A-Die CLK_ITF pin is input
    ANA_REG_AND(ANA_MISC_CTL2,~ANA_CLK_ITF_OE);
    //set D-Die CLK_ITF pin is output
    CHIP_REG_OR(APB_CLK_CFG2,CLK_ITF_OE);
    
}
#endif

/*****************************************************************************/
//  Description:    Initialize analog related registers
//  Author:         Wenjun.Shi
//  Note:
/*****************************************************************************/
PUBLIC void ANA_PHY_Init (void)
{
    //need to disable RF get into debug mode before tp init, because rf debug mode shares TP_XL pin
    ANA_REG_AND( ANA_MISC_STS0, ~RF_DEB_SPI_MCU_EN );
    
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
    {355, 370, 0, 50},      // 3.6V  // 13mA
    {365, 380, 0, 65},      // 3.7V  // 16mA
    {375, 480, 0, 80},      // 3.8V  // 20mA
    {ANA_INVALID_VALUE, ANA_INVALID_VALUE, 0, 100} // invalid
};

// Analog Device information table
LOCAL ANA_DEV_T s_ana_dev_tab[ANA_DEV_ID_MAX] =
{
    {ANA_DEV_ID_LCM_BL,                     1,  0,      NULL,                        ANA_SW_INDEX_MAX,      _ANA_SetLCMBackLight},
    {ANA_DEV_ID_LCM_BRIGHTNESS,     32,     0,      (ANA_SW_T_PTR)s_ana_bln_sw_tab,   ANA_SW_INDEX_MAX,       _ANA_SetLCMBrightness}, /*lint !e605*/
    {ANA_DEV_ID_KPD_BL,                     16,      0,      NULL,               ANA_SW_INDEX_MAX,       _ANA_SetKPDBackLight},
    {ANA_DEV_ID_VIBRATOR,                   8,      0,      NULL,               ANA_SW_INDEX_MAX,       _ANA_SetVibrator},
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
    
    if(ANA_REG_GET(ANA_MISC_STS0) & WHTLED_PD_STS)  //bl off
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
