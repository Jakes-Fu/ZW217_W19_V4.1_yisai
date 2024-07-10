/*************************************************************************
 ** File Name:      mmiweather_id.h                                      *
 ** Author:         Gaily.Wang                                           *
 ** Date:           2011/04/18                                           *
 ** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the IDs for weather module         *
 *************************************************************************/

#ifndef _MMIWEATHER_ID_H_
#define _MMIWEATHER_ID_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "sci_types.h"
//#include "os_api.h"
//#include "mmk_type.h"
//#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
 
#define WIN_ID_DEF(win_id)          win_id
 
// window ID
typedef enum
{
    MMIWEATHER_WIN_ID_START = (MMI_MODULE_WEATHER << 16),

#include "mmiweather_id.def"

    MMIWEATHER_MAX_WIN_ID
}MMIWEATHER_WIN_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIWEATHER_CTRL_ID_START = MMIWEATHER_MAX_WIN_ID,
    MMIWEATHER_CITY_LIST_FORM_CTRL_ID,

    // city0
    MMIWEATHER_CITY_LIST_CITY0_FORM_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY0_CW_FORM_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY0_CW_CITY_LABEL_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY0_CW_WEAH_LABEL_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY0_TEMP_LABEL_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY0_WICON_ANIM_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY0_HELP_LABEL_CTRL_ID,

    // city1
    MMIWEATHER_CITY_LIST_CITY1_FORM_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY1_CW_FORM_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY1_CW_CITY_LABEL_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY1_CW_WEAH_LABEL_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY1_TEMP_LABEL_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY1_WICON_ANIM_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY1_HELP_LABEL_CTRL_ID,

    // city2
    MMIWEATHER_CITY_LIST_CITY2_FORM_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY2_CW_FORM_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY2_CW_CITY_LABEL_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY2_CW_WEAH_LABEL_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY2_TEMP_LABEL_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY2_WICON_ANIM_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY2_HELP_LABEL_CTRL_ID,

    // city3
    MMIWEATHER_CITY_LIST_CITY3_FORM_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY3_CW_FORM_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY3_CW_CITY_LABEL_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY3_CW_WEAH_LABEL_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY3_TEMP_LABEL_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY3_WICON_ANIM_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY3_HELP_LABEL_CTRL_ID,

    // city4
    MMIWEATHER_CITY_LIST_CITY4_FORM_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY4_CW_FORM_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY4_CW_CITY_LABEL_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY4_CW_WEAH_LABEL_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY4_TEMP_LABEL_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY4_WICON_ANIM_CTRL_ID,
    MMIWEATHER_CITY_LIST_CITY4_HELP_LABEL_CTRL_ID,

    // add city
    MMIWEATHER_CITY_LIST_ADD_CITY_FORM_CTRL_ID,
    MMIWEATHER_CITY_LIST_ADD_CITY_ANIM_CTRL_ID,
    MMIWEATHER_CITY_LIST_ADD_CITY_LABEL_CTRL_ID,
    MMIWEATHER_CITY_LIST_ADD_CITY_HELP_LABEL_CTRL_ID,

    MMIWEATHER_SETTING_FORM_CTRL_ID,
    
    MMIWEATHER_SETTING_AUTO_UPDATE_FORM_CTRL_ID,
    MMIWEATHER_SETTING_AUTO_UPDATE_LABEL_CTRL_ID,
    MMIWEATHER_SETTING_AUTO_UPDATE_SETLIST_CTRL_ID,

    MMIWEATHER_SETTING_UPDATE_INTERVAL_FORM_CTRL_ID,
    MMIWEATHER_SETTING_UPDATE_INTERVAL_LABEL_CTRL_ID,
    MMIWEATHER_SETTING_UPDATE_INTERVAL_DROPDOWNLIST_CTRL_ID,

    MMIWEATHER_SETTING_UPDATE_BEGIN_TIME_FORM_CTRL_ID,
    MMIWEATHER_SETTING_UPDATE_BEGIN_TIME_LABEL_CTRL_ID,
    MMIWEATHER_SETTING_UPDATE_BEGIN_TIME_DROPDOWNLIST_CTRL_ID,

    MMIWEATHER_SETTING_UPDATE_END_TIME_FORM_CTRL_ID,
    MMIWEATHER_SETTING_UPDATE_END_TIME_LABEL_CTRL_ID,
    MMIWEATHER_SETTING_UPDATE_END_TIME_DROPDOWNLIST_CTRL_ID,

    MMIWEATHER_SETTING_TEMPERATURE_UNIT_FORM_CTRL_ID,
    MMIWEATHER_SETTING_TEMPERATURE_UNIT_LABEL_CTRL_ID,
    MMIWEATHER_SETTING_TEMPERATURE_UNIT_DROPDOWNLIST_CTRL_ID,
    
    //ÍøÂçÉèÖÃ===========
    MMIWEATHER_SETTING_NET_SELECT_FORM_CTRL_ID,
    MMIWEATHER_SETTING_NET_SELECT_LABEL_CTRL_ID,
    MMIWEATHER_SETTING_NET_SELECT_LABEL1_CTRL_ID,

    MMIWEATHER_NET_SETTING_FORM_ID,
    
    MMIWEATHER_NET_SELECT_FORM_ID,
    MMIWEATHER_NET_SELECT_LABEL_ID,
    MMIWEATHER_NET_SELECT_DROPDOWNLIST_ID,
    
    MMIWEATHER_NET_SELECT_FORM_SIM1_ID,
    MMIWEATHER_NET_SELECT_LABEL_SIM1_ID,
    MMIWEATHER_NET_SELECT_DROPDOWNLIST_SIM1_ID,
    
    MMIWEATHER_NET_SELECT_FORM_SIM2_ID,
    MMIWEATHER_NET_SELECT_LABEL_SIM2_ID,
    MMIWEATHER_NET_SELECT_DROPDOWNLIST_SIM2_ID,
    
    MMIWEATHER_NET_SELECT_FORM_SIM3_ID,
    MMIWEATHER_NET_SELECT_LABEL_SIM3_ID,
    MMIWEATHER_NET_SELECT_DROPDOWNLIST_SIM3_ID,
    
    MMIWEATHER_NET_SELECT_FORM_SIM4_ID,
    MMIWEATHER_NET_SELECT_LABEL_SIM4_ID,
    MMIWEATHER_NET_SELECT_DROPDOWNLIST_SIM4_ID,
    //==================

    MMIWEATHER_ADD_CITY_FORM_CTRL_ID,
    MMIWEATHER_ADD_CITY_EDIT_CTRL_ID,
    MMIWEATHER_ADD_CITY_BUTTON_CTRL_ID,
    MMIWEATHER_ADD_CITY_LIST_CTRL_ID,

    MMIWEATHER_MAINMENU_CTRL_ID,
    MMIWEATHER_COUNTRY_LIST_CTRL_ID,    

    MMIWEATHER_MAX_CTRL_ID
}MMIWIFI_CONTROL_ID_E;

enum
{
    MMIWEATHER_FORM_CTRL_ID_OFFSET = 0,
    MMIWEATHER_CW_FORM_CTRL_ID_OFFSET,
    MMIWEATHER_CW_CITY_LABEL_CTRL_ID_OFFSET,
    MMIWEATHER_CW_WEAH_LABEL_CTRL_ID_OFFSET,
    MMIWEATHER_TEMP_LABEL_CTRL_ID_OFFSET,
    MMIWEATHER_WICON_ANIM_CTRL_ID_OFFSET,
    MMIWEATHER_HELP_LABEL_CTRL_ID_OFFSET,
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : Register weather win id
//	Global resource dependence : none
//  Author: Gaily.Wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIWEATHER_RegWinIdNameArr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIWEATHER_ID_H_
