/*****************************************************************************
** File Name:      mmivp_id.h                                                *
** Author:                                                                   *
** Date:           2010/03/26                                                *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe vp win and control id       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010/03/26     apple.zhang        Create                                    *
******************************************************************************/

#ifndef _MMIVP_ID_H_
#define _MMIVP_ID_H_
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
    VP_WIN_ID_START = (MMI_MODULE_VP << 16),

#include "mmivp_id.def"

    VP_MAX_WIN_ID
}VP_WINDOW_ID_E;

#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
#undef WIN_ID_DEF

// control ID
typedef enum
{
    VP_CTRL_ID_ID_START = VP_MAX_WIN_ID,
    VP_PLAY_NAME_LABEL_CTRL_ID,    
    VP_STATION_LABEL_CTRL_ID,
    VP_OPT_MENU_CTRL_ID,
    VP_NET_OPT_MENU_CTRL_ID,
    #ifdef MMI_VIDEOPLAYER_HELP_SUPPORT
    VP_HELP_TEXT_CTRL_ID,
#endif
    VP_MENU_SET_OPT_CTRL_ID,
//    VP_PLAY_MODE_FORM_CTRL_ID,
//    VP_PLAY_REPEEAT_MODE_LABEL_CTRL_ID,
//    VP_PLAY_RANDOM_MODE_LABEL_CTRL_ID,
//    VP_PLAY_REPEAT_MODE_DROPLIST_CTRL_ID,
 //   VP_PLAY_RANDOM_MODE_DROPLIST_CTRL_ID,
//    VP_PLAY_REPEAT_MODE_LIST_CTRL_ID,
//    VP_PLAY_RANDOM_MODE_LIST_CTRL_ID,
    VP_LISTBOX_LINK_SETTINGCTRL_ID,
    VP_RTCP_PORT_INPUT_FORM_CTRL_ID,
    VP_RTCP_LOWEST_PORT_FORM_CTRL_ID,
    VP_RTCP_LOWEST_PORT_LABEL_CTRL_ID,
    VP_RTCP_LOWEST_PORT_INPUT_CTRL_ID,
    VP_RTCP_HIGHEST_PORT_FROM_CTRL_ID,
    VP_RTCP_HIGHEST_PORT_INPUT_CTRL_ID,
    VP_RTCP_HIGHEST_PORT_LABEL_CTRL_ID,
    VP_AUTH_INPUT_FORM_CTRL_ID,
    VP_AUTH_USERNAME_FORM_CTRL_ID,
    VP_AUTH_USERNAME_LABEL_CTRL_ID,
    VP_AUTH_USERNAME_INPUT_CTRL_ID,
    VP_AUTH_PASSWORD_FORM_CTRL_ID,
    VP_AUTH_PASSWORD_LABEL_CTRL_ID,
    VP_AUTH_PASSWORD_INPUT_CTRL_ID,
    VP_BOOKMARK_LIST_CTRL_ID,
    VP_HISTORY_LIST_CTRL_ID,
    VP_COMMON_EDIT_CTRL_ID,
    VP_SHOUTCUT_OPT_MENU_CTRL_ID,
    VP_NET_RICHTEXT_CTRL_ID,
    VP_NET_FORM_CTRL_ID,
    VP_URL_INPUT_NAME_FORM_CTRL_ID,
    VP_URL_INPUT_NAME_LABEL_CTRL_ID,
    VP_NET_URL_INPUT_CTRL_ID,
    VP_NET_EDIT_INPUT_URL_CTRL_ID,
    VP_CMCC_NAME_FORM_CTRL_ID,
    VP_CMCC_NAME_LABEL_CTRL_ID,
    VP_NET_CMCC_INPUT_CTRL_ID,
    VP_LOCALLIST_CTRL_ID,
    VP_DEFAULT_LOCALLIST_CTRL_ID,
    VP_USER_LOCALLIST_CTRL_ID,
   #ifdef MMI_PDA_SUPPORT
    VP_USER_LOCALLIST_FORM_CTRL_ID,
   #endif
    VP_FILE_DETAIL_TEXT_CTRL_ID,
    VP_ADD_BOOKMARK_FORM_CTRL_ID,
    VP_BOOKMARK_NAME_FORM_CTRL_ID,
    VP_BOOKMARK_NAME_INPUT_CTRL_ID,
    VP_BOOKMARK_NAME_LABEL_CTRL_ID,
    VP_BOOKMARK_URL_FORM_CTRL_ID,
    VP_BOOKMARK_URL_INPUT_CTRL_ID,
    VP_BOOKMARK_URL_LABEL_CTRL_ID,
    VP_BOOKMARK_OPT_MENU_CTRL_ID,
    VP_BOOKMARK_DETAIL_TEXT_CTRL_ID,
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
    VP_HISTORY_OPT_MENU_CTRL_ID,
#endif
    VP_NET_DETAIL_TEXT_CTRL_ID,
    VP_DEFAULT_LOCALLIST_OPT_MENU_CTRL_ID,
    VP_USER_LOCALLIST_OPT_MENU_CTRL_ID,
    VP_AGENT_FORM_CTRL_ID,
    VP_AGENT_OPEN_FORM_CTRL_ID,
    VP_AGENT_LABEL_CTRL_ID,
    VP_AGENT_INPUT_SETLIST_CTRL_ID,
    VP_AGENT_IP_FORM_CTRL_ID,
    VP_AGENT_IP_LABEL_CTRL_ID,
    VP_AGENT_IP_INPUT_CTRL_ID,
    VP_AGENT_PORT_FORM_CTRL_ID,
    VP_AGENT_PORT_LABEL_CTRL_ID,
    VP_AGENT_PORT_INPUT_CTRL_ID,
    VP_MENU_FILE_OPT_CTRL_ID,
    VP_SELECT_SIM_LIST_CTRL_ID,

//    VP_NET_OPT_MENU_CTRL_ID,
    VP_PLAY_PANEL_FORM_CTRL_ID,
    VP_PLAY_PANEL_TITLE_FORM_CTRL_ID,
    VP_PLAY_PANEL_FILE_BUTTON_CTRL_ID,
    VP_PLAY_PANEL_REPEAT_BUTTON_CTRL_ID,
    VP_PLAY_PANEL_RANDOM_BUTTON_CTRL_ID,
    VP_PLAY_PANEL_VIDEO_OWNDRAW_CTRL_ID,
    VP_PLAY_PANEL_PROCESS_OWNDRAW_CTRL_ID,
    VP_PLAY_PANEL_STATION_LABEL_CTRL_ID,
    VP_PLAY_PANEL_BUTTON_FORM_CTRL_ID,
    VP_PLAY_PANEL_VOL_BUTTON_CTRL_ID,
    VP_PLAY_PANEL_PRE_BUTTON_CTRL_ID,
    VP_PLAY_PANEL_PLAY_BUTTON_CTRL_ID,
    VP_PLAY_PANEL_NEXT_BUTTON_CTRL_ID,
    VP_PLAY_PANEL_FULLSCREEN_BUTTON_CTRL_ID,
    VP_PLAY_PANEL_VIDEO_FORM_CTRL_ID,
    VP_PLAY_PANEL_MODE_FORM_CTRL_ID,
    VP_PLAY_PANEL_OPT_BUTTON_CTRL_ID,
    VP_PLAY_PANEL_RETURN_BUTTON_CTRL_ID,
    VP_PLAY_PANEL_SPACE_OWNDRAW_CTRL_ID,
#ifdef MMI_PDA_SUPPORT//@andy.he_MP4PDA
    VP_PLAY_PANEL_BTN_SPACE_OWNDRAW_CTRL_ID,
#endif
    VP_PLAY_PANEL_VOL_FORM_CTRL_ID,
    VP_PLAY_PANEL_PRE_FORM_CTRL_ID,
    VP_PLAY_PANEL_SCREEN_FORM_CTRL_ID,
    VP_PLAY_PANEL_NEXT_FORM_CTRL_ID,
    VP_PLAY_PANEL_PLAY_FORM_CTRL_ID,
    VP_PLAY_PANEL_V_SPACE_OWNDRAW_CTRL_ID,

#ifdef MMI_PDA_SUPPORT
    //form
    VP_URL_INPUT_NAME_TITLE_FORM_CTRL_ID,
    VP_CMCC_NAME_TITLE_FORM_CTRL_ID,
    VP_URL_LINK_BUTTON1_CTRL_ID,
    VP_URL_LINK_BUTTON2_CTRL_ID,
    
    VP_BOOKMARK_LONGOK_OPT_MENU_CTRL_ID,
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
    VP_HISTORY_LONGOK_OPT_MENU_CTRL_ID,
#endif
    VP_DEFAULT_LOCALLIST_LONGOK_OPT_MENU_CTRL_ID,
    VP_USER_LOCALLIST_LONGOK_OPT_MENU_CTRL_ID,
    MMIVP_REPEAT_SET_LIST_CTRL_ID,
	
#endif
	MMIVP_PLAYBACK_MODE_SET_LIST_CTRL_ID,
#ifndef MMI_GUI_STYLE_TYPICAL
    MMIVP_PDA_SETTINGS_LIST_CTRL_ID,
#endif  

#ifdef MMI_VIDEO_PLAYER_MINI_LAYOUT//6530 MINI 界面布局
    VP_PLAY_PANEL_BACK_LABEL_CTRL_ID,
    VP_PLAY_PANEL_OPTIONS_LABEL_CTRL_ID,
    VP_PLAY_MODE_SHUFFLE_CTRL_ID,
    VP_PLAY_MODE_REPEAT_CTRL_ID,
#endif
    VP_MAX_CTRL_ID
}VP_CONTROL_ID_E;
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Register acc menu group
//  Global resource dependence : none
//  Author: haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIVP_RegWinIdNameArr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
