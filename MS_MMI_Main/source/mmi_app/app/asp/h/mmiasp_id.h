/*****************************************************************************
** File Name:      mmiasp_id.h                                               *
** Author:         plum.peng                                                 *
** Date:           2008/11/27                                                *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2008/11/27     plum.peng        Create                                    *
******************************************************************************/

#ifndef _MMIASP_ID_H_
#define _MMIASP_ID_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

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
    MMIASP_WIN_ID_START = (MMI_MODULE_ASP << 16),

#include "mmiasp_id.def"

    MMIASP_MAX_WIN_ID
}MMIASP_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIASP_CTRL_ID_ID_START = MMIASP_MAX_WIN_ID, 
    MMIASP_NAME_LABEL_CTRL_ID,
    MMIASP_NUM_LABEL_CTRL_ID,
    MMIASP_DURATION_LABEL_CTRL_ID,
    MMIASP_PLAY_TIME_LABEL_CTRL_ID,
    MMIASP_MSG_LIST_CTRL_ID,
    MMIASP_MSG_OPT_MENU_CTRL_ID,
    MMIASP_MSG_PLAY_WIN_OPT_MENU_CTRL_ID,
    MMIASP_MSG_RING_SELECT_MENU_CTRL_ID,
    MMIASP_HELP_TEXT_CTRL_ID,
    
    MMIASP_SETTING_FORM_CTRL_ID,
    
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMIASP_SETTING_FLAG_SIM1_FORM_CTRL_ID,
    MMIASP_SETTING_FLAG_SIM1_LABEL_CTRL_ID,
    MMIASP_SETTING_FLAG_SIM1_DROPDOWNLIST_CTRL_ID,
    MMIASP_SETTING_FLAG_SIM2_FORM_CTRL_ID,
    MMIASP_SETTING_FLAG_SIM2_LABEL_CTRL_ID,
    MMIASP_SETTING_FLAG_SIM2_DROPDOWNLIST_CTRL_ID,
 #if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    MMIASP_SETTING_FLAG_SIM3_FORM_CTRL_ID,
    MMIASP_SETTING_FLAG_SIM3_LABEL_CTRL_ID,
    MMIASP_SETTING_FLAG_SIM3_DROPDOWNLIST_CTRL_ID,
 #endif
 #if defined(MMI_MULTI_SIM_SYS_QUAD)
    MMIASP_SETTING_FLAG_SIM4_FORM_CTRL_ID,
    MMIASP_SETTING_FLAG_SIM4_LABEL_CTRL_ID,
    MMIASP_SETTING_FLAG_SIM4_DROPDOWNLIST_CTRL_ID,
 #endif
 #else
    //开关
    MMIASP_SETTING_FLAG_FORM_CTRL_ID,
    MMIASP_SETTING_FLAG_LABEL_CTRL_ID,
    MMIASP_SETTING_FLAG_DROPDOWNLIST_CTRL_ID,
 #endif
    // 答录语
    MMIASP_SETTING_AUDIO_FORM_CTRL_ID,
    MMIASP_SETTING_AUDIO_LABEL_CTRL_ID,
    MMIASP_SETTING_AUDIO_SELECT_LABEL_CTRL_ID,
    // 留言长度
    MMIASP_SETTING_AUDIO_TIME_FORM_CTRL_ID,
    MMIASP_SETTING_AUDIO_TIME_LABEL_CTRL_ID,
    MMIASP_SETTING_AUDIO_TIME_DROPDOWNLIST_CTRL_ID,
    // n秒开始答录
    MMIASP_SETTING_WAIT_TIME_FORM_CTRL_ID,
    MMIASP_SETTING_WAIT_TIME_LABEL_CTRL_ID,
    MMIASP_SETTING_WAIT_TIME_DROPDOWNLIST_CTRL_ID,
    // 扬声器
    MMIASP_SETTING_SPEAKER_FLAG_FORM_CTRL_ID,
    MMIASP_SETTING_SPEAKER_FLAG_LABEL_CTRL_ID,
    MMIASP_SETTING_SPEAKER_FLAG_DROPDOWNLIST_CTRL_ID,
     // 留言存储位置
     MMIASP_SETTING_SAVE_PATH_FORM_CTRL_ID,
    MMIASP_SETTING_SAVE_PATH_LABEL_CTRL_ID,
    MMIASP_SETTING_SAVE_PATH_DROPDOWNLIST_CTRL_ID,
    
    MMIASP_SETTING_FIXED_ANSWERRING_LIST_CTRL_ID,
     
    MMIASP_EDIT_RENAME_MSG_FILE_CTRL_ID,

    MMIASP_PLAY_FORM_CTRL_ID,
    MMIASP_LABEL_NAME_CTRL_ID,
    MMIASP_LABEL_TEL_CTRL_ID,
    MMIASP_LABEL_TOTALTIME_CTRL_ID,
    MMIASP_PROGRESS_OWNER_DRAW_CTRL_ID,
    MMIASP_TIMER_OWNER_DRAW_CTRL_ID,
   
    MMIASP_PLAY_PANEL_FORM_CTRL_ID,
    MMIASP_BUTTON_VOL_CTRL_ID,
    MMIASP_BUTTON_PREV_CTRL_ID,
    MMIASP_BUTTON_PLAY_CTRL_ID,
    MMIASP_BUTTON_NEXT_CTRL_ID,
    
    // 来电答录窗口的一些控件
    MMIASP_RECORDING_FORM_CTRL_ID,
    MMIASP_RECORDING_IMAGE_ANIM_CTRL_ID,
    MMIASP_RECORDING_TEXT_FORM_CTRL_ID,
    MMIASP_RECORDING_NAME_LABEL_CTRL_ID,
    MMIASP_RECORDING_NUMBER_LABEL_CTRL_ID,
    MMIASP_RECORDING_TIMING_LABEL_CTRL_ID,
    
#ifdef MMI_PDA_SUPPORT
    MMIASP_PLAY_PROGRESS_BG_NULL_FORM_CTRL_ID,
    MMIASP_PLAY_PROGRESS_BG_FORM_CTRL_ID,
    MMIASP_PLAY_OWNDRAW_BUTTON_CTRL_ID,
    MMIASP_PLAY_OWNDRAW_NULL_CTRL_ID,
    MMIASP_RECORDING_OWNDRAW_CTRL_ID,
    MMIASP_MSGLIST_LONGOK_OPT_MENU_CTRL_ID,
#endif
    
    MMIASP_MAX_CTRL_ID
}MMIASP_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIASP_ID_H_
