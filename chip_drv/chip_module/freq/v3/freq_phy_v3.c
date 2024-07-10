/******************************************************************************
 ** File Name:      freq_phy_v3.c                                                    *
 ** Author:         Yuhua.Shi                                              *
 ** DATE:           08/17/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the physical layer of FREQ module.          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/17/2010     Yuhua.Shi     Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "../freq_phy.h"
#include "dal_lcd.h"
#include "nlc_drvapi.h"
#include "emc_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/
//#define CLK_DIVIDER_MASK     0x3FF

/**---------------------------------------------------------------------------*
 **                            Local Variables
 **---------------------------------------------------------------------------*/
//used to save the pointer of customized freq config
LOCAL uint32   *s_customized_freq_cfg_ptr = PNULL;

/**---------------------------------------------------------------------------*
 **                     LOCAL Function Prototypes                    *
 **---------------------------------------------------------------------------*/
LOCAL BOOLEAN _FREQ_PHY_IsChngFreqEnable (
    void
);
LOCAL uint32 *_FREQ_PHY_GetCustomFreqCfg (
    void
);

/**---------------------------------------------------------------------------*
 **                         Function defines                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    This function is used to get the flag if need to change frequency
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
LOCAL BOOLEAN _FREQ_PHY_IsChngFreqEnable (
    void
)
{
    CHNG_FREQ_CONFIG_PTR freq_cfg_ptr = FREQ_GetFreqCfg();
    FREQ_CONTROL_PTR         freq_cont_ptr  = freq_cfg_ptr->freq_control_info_ptr;

    SCI_ASSERT (freq_cfg_ptr != PNULL);/*assert verified*/
    SCI_ASSERT (freq_cont_ptr != PNULL);/*assert verified*/

    return freq_cont_ptr->is_chng_freq_enable;
}

/*****************************************************************************/
// Description :    This function is used to get the customized freq config(base address)
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
LOCAL uint32 *_FREQ_PHY_GetCustomFreqCfg (
    void
)
{
    return s_customized_freq_cfg_ptr;
}

/*****************************************************************************/
// Description :    This function is used to set the customized freq config(base address)
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC void FREQ_PHY_SetCustomFreqCfg (
    uint32 *p_customized_freq_ptr//pointer of customized freqency config table
)
{
    SCI_ASSERT (p_customized_freq_ptr != PNULL);/*assert verified*/

    s_customized_freq_cfg_ptr = p_customized_freq_ptr;
}

/*****************************************************************************/
// Description :    This function is used to detect validity of arm/ahb clk level
// Global resource dependence :
// Author :         Yuhua.Shi
// Note :
//
/*****************************************************************************/
PUBLIC BOOLEAN FREQ_PHY_DetectClkValidity (
    uint32 arm_ahb_clk_level//the arm/ahb clk level would be set
)
{
    BOOLEAN valid;
    CHNG_FREQ_CONFIG_PTR freq_cfg_ptr = FREQ_GetFreqCfg();
    FREQ_CONTROL_PTR         freq_cont_ptr  = freq_cfg_ptr->freq_control_info_ptr;

    SCI_ASSERT (freq_cfg_ptr != PNULL);/*assert verified*/
    SCI_ASSERT (freq_cont_ptr != PNULL);/*assert verified*/

    if ( (arm_ahb_clk_level < ( (uint32) freq_cont_ptr->min_clk_lvl))
            || (arm_ahb_clk_level > ( (uint32) freq_cont_ptr->max_clk_lvl)))
    {
        valid = SCI_FALSE;
    }
    else
    {
        valid = SCI_TRUE;
    }

    return valid;
}

/*****************************************************************************/
// Description :    This function is used to get the min clk level of arm/ahb clock
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC uint32 FREQ_PHY_GetMinClkLevel (
    void
)
{
    CHNG_FREQ_CONFIG_PTR freq_cfg_ptr = FREQ_GetFreqCfg();
    FREQ_CONTROL_PTR         freq_cont_ptr  = freq_cfg_ptr->freq_control_info_ptr;

    SCI_ASSERT (freq_cfg_ptr != PNULL);/*assert verified*/
    SCI_ASSERT (freq_cont_ptr != PNULL);/*assert verified*/

    return freq_cont_ptr->min_clk_lvl;
}

/*****************************************************************************/
// Description :    This function is used to get the max clk level of arm/ahb clock
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC uint32 FREQ_PHY_GetMaxClkLevel (
    void
)
{
    CHNG_FREQ_CONFIG_PTR freq_cfg_ptr = FREQ_GetFreqCfg();
    FREQ_CONTROL_PTR         freq_cont_ptr  = freq_cfg_ptr->freq_control_info_ptr;

    SCI_ASSERT (freq_cfg_ptr != PNULL);/*assert verified*/
    SCI_ASSERT (freq_cont_ptr != PNULL);/*assert verified*/

    return freq_cont_ptr->max_clk_lvl;
}

/*****************************************************************************/
// Description :    This function is used to get the clock level
// Global resource dependence :
// Author :         Yuhua.Shi
// Note :
/*****************************************************************************/
PUBLIC uint32 FREQ_PHY_GetClkLevel (
    CHNG_FREQ_INDEX_E freq_type_index//index of frequency type
)
{
    uint32 i = 0;
    uint32  arm_ahb_clk_level = 0;
    uint32    *p_customized_freq_ptr = PNULL;
    CHNG_FREQ_CONFIG_PTR freq_cfg_ptr = FREQ_GetFreqCfg();
    FREQ_APP_CLK_LVL_PTR   app_clk_lvl_table_ptr  = freq_cfg_ptr->app_clk_lvl_table_ptr;

    SCI_ASSERT (freq_type_index < FREQ_INDEX_MAX);/*assert verified*/
    SCI_ASSERT (freq_cfg_ptr != PNULL);/*assert verified*/
    SCI_ASSERT (app_clk_lvl_table_ptr != PNULL);/*assert verified*/

    if (freq_type_index < FREQ_INDEX_CUSTOMIZED_BASE)
    {
        //get the clk level of intenal application
        i = 0;

        while ( ( (uint32) app_clk_lvl_table_ptr[i].freq_index) != (uint32) FREQ_INDEX_MAX)
        {
            if ( ( (uint32) app_clk_lvl_table_ptr[i].freq_index) == (uint32) freq_type_index)
            {
                arm_ahb_clk_level = (uint32) (app_clk_lvl_table_ptr[i].clk_lvl);
                break;
            }
            else
            {
                i++;
            }
        }
    }
    else
    {
        //get the external customized freq config
        p_customized_freq_ptr = _FREQ_PHY_GetCustomFreqCfg();
        arm_ahb_clk_level = p_customized_freq_ptr[freq_type_index-FREQ_INDEX_CUSTOMIZED_BASE];/*lint !e656 confirmed by xuepeng*/
    }

    SCI_PASSERT (FREQ_PHY_DetectClkValidity (arm_ahb_clk_level), /*assert verified*/  \
                 ("FREQ_PHY_DetectClkValidity:Invalid clock level value (%d)!", arm_ahb_clk_level));

    return arm_ahb_clk_level;
}

/*********************************************************************************************/
//Description:   This function is used to config Ahb device timing befor change arm/ahb clock
// Global resource dependence :
//Author:        Yuhua.Shi
//Note:
/*********************************************************************************************/
PUBLIC void FREQ_PHY_BeforeChangeClk (
    uint32 handler, //device handler
    uint32 arm_ahb_clk_level,//the arm/ahb clk level would be set
    FREQ_SendReq_CALLBACK SendReq_Callback_Func//callback function to send freq request
)
{
    uint32  cur_ahb_clk,ahb_clk_to_set;
    CHNG_FREQ_MESSAGE_T freq_param;
    CHNG_FREQ_CONFIG_PTR freq_cfg_ptr = FREQ_GetFreqCfg();
    FREQ_CLK_LVL_PTR          clk_lvl_table_ptr  = freq_cfg_ptr->clk_lvl_table_ptr;
    FREQ_CONTROL_PTR         freq_cont_ptr  = freq_cfg_ptr->freq_control_info_ptr;

    SCI_PASSERT (FREQ_PHY_DetectClkValidity (arm_ahb_clk_level), ("Invalid clock level value (%d)!", arm_ahb_clk_level));/*assert verified*/
    SCI_ASSERT (freq_cfg_ptr != PNULL);/*assert verified*/
    SCI_ASSERT (clk_lvl_table_ptr != PNULL);/*assert verified*/

    if (_FREQ_PHY_IsChngFreqEnable())
    {
        cur_ahb_clk = CHIP_GetAhbClk();

        ahb_clk_to_set = (uint32) (clk_lvl_table_ptr[arm_ahb_clk_level-1UL].ahb_clk);
        freq_param.freq_param1 = arm_ahb_clk_level;
        freq_param.freq_param4 = freq_cont_ptr->is_ahbmaster_event_enable;

        if (ahb_clk_to_set > cur_ahb_clk)
        {
            /*
              note, since there is a semophore get operation in LCD_UpdateTiming(),
              and the semaphore put operation will occur in lcd irq handler, so
              don't disable irq when do the LCD_UpdateTiming(), or else a deadlock
              will happen in some case;
            */
            LCD_UpdateTiming (ahb_clk_to_set);

            if (CHIP_DetectMemType())     //Use Nand +SDRAM
            {
                NANDCTL_SetParam (ahb_clk_to_set);
            }

            if (SendReq_Callback_Func)
            {
                (*SendReq_Callback_Func) (handler, &freq_param);
            }
        }
        else
        {
            if (SendReq_Callback_Func)
            {
                (*SendReq_Callback_Func) (handler, &freq_param);
            }

            if (CHIP_DetectMemType())     //Use Nand +SDRAM
            {
                NANDCTL_SetParam (ahb_clk_to_set);
            }

            LCD_UpdateTiming (ahb_clk_to_set);
        }

    }

}

#pragma arm section code = "FREQ_CHNG_IN_IRAM"
/*********************************************************************************************/
//Description:   This function is used to change clk(e.g. arm/ahb/emc clock)
// Global resource dependence :
//Author:        Yuhua.Shi
//Note:
/*********************************************************************************************/
PUBLIC void FREQ_PHY_ChangeClk (
    CHNG_FREQ_MESSAGE_T *freq_param//parameter of freq
)
{
    //volatile uint32 reg_value;
    uint32 arm_ahb_div, arm_ahb_clk_lvl, ahb_clk_to_set;
    CHNG_FREQ_CONFIG_PTR freq_cfg_ptr = FREQ_GetFreqCfg();
    FREQ_CLK_LVL_PTR          clk_lvl_table_ptr  = freq_cfg_ptr->clk_lvl_table_ptr;
    uint32                             *clk_div_table_ptr  = freq_cfg_ptr->clk_div_table_ptr;

    arm_ahb_clk_lvl = freq_param->freq_param1;

    arm_ahb_div  = clk_div_table_ptr[arm_ahb_clk_lvl-1];
    ahb_clk_to_set  = clk_lvl_table_ptr[arm_ahb_clk_lvl-1].ahb_clk;

    if (!CHIP_DetectMemType())     //Use Nand +SDRAM
    {
        EMC_Timing_chng (ahb_clk_to_set);
    }

    //change arm/ahb/emc clock
    //reg_value = (CHIP_REG_GET (AHB_CLK_CFG0) & (~CLK_DIVIDER_MASK)) | arm_ahb_div;
    //CHIP_REG_SET (AHB_CLK_CFG0, reg_value);
}
#pragma arm section code

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/


#ifdef   __cplusplus
}
#endif

/*  End Of File */
