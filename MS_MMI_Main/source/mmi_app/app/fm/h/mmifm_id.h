/*****************************************************************************
** File Name:      fm_id.h                                                   *
** Author:                                                                   *
** Date:           2006/05/17                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe fm win and control id       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/05/17     liqing.peng      Create                                    *
******************************************************************************/

#ifndef _FM_ID_H_
#define _FM_ID_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

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
    FM_WIN_ID_START = (MMI_MODULE_FM << 16),

#include "mmifm_id.def"

    FM_MAX_WIN_ID
}FM_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    FM_CTRL_ID_ID_START = FM_MAX_WIN_ID, //(MMI_MODULE_FM << 16),
    FM_SETTING_MENU_CTRL_ID,        // FM setting menu id
    FM_OPTION_LIST_CTRL_ID,         // FM option list id
    FM_SENSI_LIST_CTRL_ID,          // FM sensi list id
    FM_SOUNDMODE_LIST_CTRL_ID,      // FM sound mode list id
    FM_FREQEUCY_LEFT_TEXT_CTRL_ID,
    FM_FREQEUCY_RIGHT_TEXT_CTRL_ID,
    FM_CHANNEL_NAME_TEXT_CTRL_ID,
    MMIFM_ADJ_FREQ_LEFT_BUTTON_ID, 
    MMIFM_ADJ_FREQ_RIGHT_BUTTON_ID,
    MMIFM_LEFT_BUTTON_ID,           
    MMIFM_RIGHT_BUTTON_ID,         
    MMIFM_UP_BUTTON_ID,            
    MMIFM_DOWN_BUTTON_ID,  
    MMI_FM_CHANNEL_LIST_CTRL_ID,
    MMIFM_EDITBOX_CTRL_ID,
    MMIFM_DETAILS_FROM_ID,
    MMIFM_DETAILS_NAME_FORM_CTRL_ID,
    MMIFM_DETAILS_NAME_LABEL_CTRL_ID,
    MMIFM_DETAILS_NAME_EDIT_LABEL_CTRL_ID,
    MMIFM_DETAILS_FREQ_FORM_CTRL_ID,
    MMIFM_DETAILS_FREQ_LABEL_CTRL_ID,
    MMIFM_DETAILS_FREQ_EDIT_LABEL_CTRL_ID,
    MMIFM_TEXTBOX_CTRL_ID,
    MMIFM_FREQUENCY_BAR_CTRL_ID,
    MMIFM_FREQUENCY_TEXT_SUBFROM_ID, 
    MMIFM_FREQUENCY_TEXT_CTRL_ID, 
    MMIFM_VOLUMN_DIRECTION_SUBFROM_ID,
    MMIFM_VOLUMN_SUBFROM_ID,
    MMIFM_VOLUMN_ANIM_ID,
    #ifdef FM_RDS_SUPPORT
    MMIFM_RDS_TEXT_CTRL_ID, 
    #endif
    MMIFM_CHANGE_FREQUENCY_SUBFROM_ID,
    MMIFM_GET_UPFRE_BUTTON_CTRL_ID,
    MMIFM_START_SUSPEND_BUTTON_CTRL_ID,
    MMIFM_GET_DOWNFRE_BUTTON_CTRL_ID,
    MMIFM_ADJ_VOLUMN_BUTTON_CTRL_ID,
    MMIFM_DIRECTION_BUTTON_CTRL_ID,
    MMIFM_KPD_CTRL_ID,
    MMIFM_MENU_BUTTON_CTRL_ID,
    MMIFM_RETURN_BUTTON_CTRL_ID,
    FM_MAIN_ABOVE_FRE_OWNER_DRAW_CTRL_ID,
    FM_MAIN_BELOW_FRE_OWNER_DRAW_CTRL_ID,
    MMIFM_MAIN_FROM_ID,
    MMI_ALARM_FM_CHANNEL_LIST_CTRL_ID,
    MMIFM_NAME_LABEL_CTRL_ID,
    MMIFM_MAINPLAY_FORM_CTRL_ID,
    MMIFM_FORM_ANIM_CTRL_ID,
    MMIFM_TIME_FORM_CTRL_ID,
    MMIFM_FORM_ANIM_H_CTRL_ID,
    MMIFM_TIME_ORDER_FORM_CTRL_ID,
    MMIFM_HSPACE_OWNDRAW_H_CTRL_ID,   
    MMIFM_BUTTON_FORM_CTRL_ID,
    MMIFM_PLAY_BUTTON_CTRL_ID,
    MMIFM_STOP_BUTTON_CTRL_ID,
    MMIFM_TIME_OWNDRAW_CTRL_ID,
    MMIFM_RECORD_RENAME_EDITBOX_CTRL_ID,

    MMIFM_TIMER_RECORD_LIST_CTRL_ID,
    MMIFM_TIMER_RECORD_FORM_CTRL_ID,
    MMIFM_SET_STATUS_BUTTON_CTRL_ID,
    MMIFM_SET_VALUE_BUTTON_CTRL_ID,

    MMIFM_EDITWIN_FORM_CTRL_ID,
    MMIFM_EDIT_TIME_FORM_CTRL_ID,
	MMIFM_EDIT_TIME_LABEL_CTRL_ID,
	MMIFM_EDIT_TIME_EDITBOX_CTRL_ID,	
	MMIFM_EDIT_TIME_ENDURE_FORM_CTRL_ID,
	MMIFM_EDIT_TIME_ENDURE_LABEL_CTRL_ID,
	MMIFM_EDIT_TIME_ENDURE_EDITBOX_CTRL_ID,   
    MMIFM_EDIT_FREQUENCY_FORM_CTRL_ID,
	MMIFM_EDIT_FREQUENCY_LABEL_CTRL_ID,
	MMIFM_EDIT_FREQUENCY_EDITBOX_CTRL_ID,
    MMIFM_EDIT_MODE_FORM_CTRL_ID,
	MMIFM_EDIT_MODE_LABEL_CTRL_ID,
	MMIFM_EDIT_MODE_SETLIST_CTRL_ID,
  	MMIFM_EDITWIN_CHILD_FORM_CTRL_ID,
  	MMIFM_EDITWIN_BUTTON0_CTRL_ID,
  	MMIFM_EDITWIN_BUTTON1_CTRL_ID,
  	MMIFM_EDITWIN_BUTTON2_CTRL_ID,
  	MMIFM_EDITWIN_BUTTON3_CTRL_ID,
  	MMIFM_EDITWIN_BUTTON4_CTRL_ID,
  	MMIFM_EDITWIN_BUTTON5_CTRL_ID,
  	MMIFM_EDITWIN_BUTTON6_CTRL_ID,
#ifdef MMI_PDA_SUPPORT
    MMIFM_CHANNEL_LIST_MENU_OPTION,
    MMIFM_EDITWIN_PDA_FORM_CTRL_ID,
    MMIFM_EDIT_TIME_EDIT_LABEL_CTRL_ID,
    MMIFM_EDIT_TIME_ENDURE_EDIT_LABEL_CTRL_ID, 
    MMIFM_EDIT_FREQUENCY_EDIT_LABEL_CTRL_ID,
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
    MMIFM_TIME_EDIT_FORM_CTRL_ID,
#endif
#ifdef MMI_PDA_SUPPORT
    MMIFM_RENAME_FORM_CTRL_ID,
#endif
    MMIFM_LIST_BUTTON_CTRL_ID, 	
    FM_MAX_CTRL_ID
}FM_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
