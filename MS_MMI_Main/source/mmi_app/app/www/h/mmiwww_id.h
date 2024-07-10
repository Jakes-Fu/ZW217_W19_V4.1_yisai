/***************************************************************************
** File Name:      mmiwww_id.h                                             *
** Author:                                                                 *
** Date:           15/06/2009                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to www browser internal define        *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 06/2009        jiaoyou.wu    Create
****************************************************************************/
#ifndef  _MMIWWW_ID_H_    
#define  _MMIWWW_ID_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h" 
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
#define WIN_ID_DEF(win_id)          win_id
 
// WWW Window ID
typedef enum
{
    MMIWWW_WIN_ID_START = (MMI_MODULE_WWW << 16),

#include "mmiwww_id.def"

    MMIWWW_MAX_WIN_ID
}MMIWWW_WINDOW_ID_E;

#undef WIN_ID_DEF
// WWW Control ID
typedef enum
{
    MMIWWW_CTRL_ID_START = MMIWWW_MAX_WIN_ID,
    MMIWWW_MAINMENU_LABEL_ADDRESS_CTRL_ID,
    MMIWWW_MAINMENU_LABEL_SEARCH_CTRL_ID,
    MMIWWW_MAINMENU_TAB_CTRL_ID,
    MMIWWW_POPMENU_MAINMENU_OPT_CTRL_ID,
    MMIWWW_POPMENU_BOOKMARK_OPT_CTRL_ID,
    MMIWWW_BOOKMARK_DETAIL_SUBMENU_CTRL_ID,
    MMIWWW_BOOKMARK_LIST_CTRL_ID,
    MMIWWW_BOOKMARK_LABEL_URL_INPUT_CTRL_ID,
    MMIWWW_BOOKMARK_LABEL_TITLE_INPUT_CTRL_ID,
    MMIWWW_BOOKMARK_INPUT_RICHTEXT_CTRL_ID,
    MMIWWW_BOOKMARK_SHOW_RICHTEXT_CTRL_ID,
    
    MMIWWW_INLINE_IME_CTRL_ID,
    MMIWWW_COMMON_EDIT_CTRL_ID,
    MMIWWW_MENU_SET_OPT_CTRL_ID,
    //MMIWWW_MENU_CC_SELECT_CTRL_ID,
    MMIWWW_LISTBOX_SET_CTRL_ID,
    MMIWWW_LISTBOX_HISTORY_CTRL_ID,
    MMIWWW_LISTBOX_PAGEMEMO_CTRL_ID,
    MMIWWW_LISTBOX_CERTIFICATE_CTRL_ID,
    MMIWWW_LISTBOX_LINK_SETTINGCTRL_ID,
    MMIWWW_LISTBOX_CHARSET_ID,
    MMIWWW_LISTBOX_POINTERMODE_ID,
    MMIWWW_LISTBOX_MATCHKEY_CTRL_ID,
    MMIWWW_LISTBOX_MATCHURL_CTRL_ID,
    MMIWWW_BROWSER_FORM_CTRL_ID,
    MMIWWW_PAGE_OPTION_BUTTON_CTRL_ID,
    MMIWWW_PAGE_RELOAD_BUTTON_CTRL_ID,
    MMIWWW_PAGE_PREV_BUTTON_CTRL_ID,
    MMIWWW_PAGE_NEXT_BUTTON_CTRL_ID,
    MMIWWW_PAGE_FULL_BUTTON_CTRL_ID,
    MMIWWW_PAGE_ANIMATE_LOADING_CTRL_ID,
    MMIWWW_PAGE_LOAD_LABEL_CTRL_ID,
    MMIWWW_PAGE_PROG_LABEL_CTRL_ID,
    MMIWWW_PAGE_SECURE_LABEL_CTRL_ID,
    MMIWWW_PAGEMEMO_NUM_LAB_CTRL_ID,

    MMIWWW_POPMENU_HISTORY_OPT_CTRL_ID,
    MMIWWW_POPMENU_PAGEMEMO_OPT_CTRL_ID,
    MMIWWW_RICHTEXT_HISTORY_ATTRIBUTE_CTRL_ID,
    MMIWWW_RICHTEXT_CERTIFICATE_ATTRIBUTE_CTRL_ID,
    MMIWWW_PAGE_OPTION_MENU_CTRL_ID,
    MMIWWW_PAGE_SEARCH_EDIT_CTRL_ID,
    MMIWWW_PAGE_PROPERTY_RICHTEXT_CTRL_ID,
    MMIWWW_PAGE_SAVE_AS_RICHTEXT_CTRL_ID,
    MMIWWW_PAGE_SAVE_AS_EDIT_CTRL_ID,
    MMIWWW_JSCONFIRM_TEXT_CTRL_ID,
    MMIWWW_JSPROMPT_RICHTEXT_CTRL_ID,
    MMIWWW_JSPROMPT_EDIT_CTRL_ID,
    MMIWWW_WMLSCONFIRM_TEXT_CTRL_ID,
    MMIWWW_WMLSPROMPT_RICHTEXT_CTRL_ID,
    MMIWWW_WMLSPROMPT_EDIT_CTRL_ID,

//    MMIWWW_LAST_SENT_NAME_CTRL_ID,
//    MMIWWW_LAST_SENT_COUNT_CTRL_ID,
//    MMIWWW_LAST_RECV_NAME_CTRL_ID,
//    MMIWWW_LAST_RECV_COUNT_CTRL_ID,
//    MMIWWW_TOTAL_SENT_NAME_CTRL_ID,
//    MMIWWW_TOTAL_SENT_COUNT_CTRL_ID,
//    MMIWWW_TOTAL_RECV_NAME_CTRL_ID,
//    MMIWWW_TOTAL_RECV_COUNT_CTRL_ID,
    MMIWWW_SETTINGS_DISPLAY_FORM_CTRL_ID,
    MMIWWW_SETTINGS_CACHE_FORM_CTRL_ID,
    MMIWWW_SETTINGS_COOKIE_FORM_CTRL_ID,
    MMIWWW_SETTINGS_WEBDATA_FORM_CTRL_ID,
    MMIWWW_SETTINGS_SCRIPT_FORM_CTRL_ID,
    MMIWWW_SETTINGS_SCRIPT_LABLE_CTRL_ID,
    MMIWWW_SETTINGS_SCRIPT_DROPLIST_CTRL_ID,
    MMIWWW_SETTINGS_ANIMAL_FORM_CTRL_ID,
    MMIWWW_SETTINGS_ANIMAL_LABLE_CTRL_ID,
    MMIWWW_SETTINGS_ANIMAL_DROPLIST_CTRL_ID,
    MMIWWW_SETTINGS_IMAGE_FORM_CTRL_ID,
    MMIWWW_SETTINGS_IMAGE_LABLE_CTRL_ID,
    MMIWWW_SETTINGS_IMAGE_DROPLIST_CTRL_ID,
    MMIWWW_SETTINGS_TABLE_FORM_CTRL_ID,
    MMIWWW_SETTINGS_TABLE_LABLE_CTRL_ID,
    MMIWWW_SETTINGS_TABLE_DROPLIST_CTRL_ID,
    MMIWWW_SETTINGS_SOUND_FORM_CTRL_ID,
    MMIWWW_SETTINGS_SOUND_LABLE_CTRL_ID,
    MMIWWW_SETTINGS_SOUND_DROPLIST_CTRL_ID,
    MMIWWW_SETTINGS_POPUP_LABLE_CTRL_ID,
    MMIWWW_SETTINGS_POPUP_DROPLIST_CTRL_ID,
    MMIWWW_SETTINGS_CACHE_MODE_FORM_CTRL_ID,
    MMIWWW_SETTINGS_CACHE_MODE_LABLE_CTRL_ID,
    MMIWWW_SETTINGS_CACHE_MODE_DROPLIST_CTRL_ID,
    MMIWWW_SETTINGS_QUERY_DISK_FORM_CTRL_ID,
    MMIWWW_SETTINGS_QUERY_DISK_LABLE_CTRL_ID,
    MMIWWW_SETTINGS_QUERY_DISK_DROPLIST_CTRL_ID,
    MMIWWW_SETTINGS_CLEAR_CACHE_FORM_CTRL_ID,
    MMIWWW_SETTINGS_CLEAR_CACHE_LABLE_CTRL_ID,
    MMIWWW_SETTINGS_COOKIE_MODE_FORM_CTRL_ID,
    MMIWWW_SETTINGS_COOKIE_MODE_LABLE_CTRL_ID,
    MMIWWW_SETTINGS_COOKIE_MODE_DROPLIST_CTRL_ID,
    MMIWWW_SETTINGS_CLEAR_COOKIE_FORM_CTRL_ID,
    MMIWWW_SETTINGS_CLEAR_COOKIE_LABLE_CTRL_ID,
    MMIWWW_SETTINGS_WEBDATA_FORM1_CTRL_ID,
    MMIWWW_SETTINGS_WEBDATA_LABLE_CTRL_ID,
    MMIWWW_SETTINGS_WEBDATA_DROPLIST_CTRL_ID,
    MMIWWW_SETTINGS_CLEAR_WEBDATA_FORM_CTRL_ID,
    MMIWWW_SETTINGS_CLEAR_WEBDATA_LABLE_CTRL_ID,
    MMIWWW_SETTINGS_HOMEPAGE_LABLE_CTRL_ID,
    MMIWWW_SETTINGS_HOMEPAGE_TEXT_CTRL_ID,
    MMIWWW_SETTINGS_HOMEPAGE_FORM_CTRL_ID,
    MMIWWW_SETTINGS_HOMEPAGE_FORM1_CTRL_ID,
    MMIWWW_SHUTCUT_KEY_OPT_CTRL_ID,
    MMIWWW_AUTH_ACCOUNT_CTRL_ID,
    MMIWWW_AUTH_ACCOUNT_VALUE_CTRL_ID,
    MMIWWW_AUTH_PASSWORD_CTRL_ID,
    MMIWWW_AUTH_PASSWORD_VALUE_CTRL_ID,
    MMIWWW_AUTH_INPUT_CTRL_ID,
    MMIWWW_SELECT_SIM_CTRL_ID,

    MMIWWW_AUTH_FORM_CTRL_ID,
    MMIWWW_AUTH_ACCOUNT_FORM_CTRL_ID,
    MMIWWW_AUTH_PASSWORD_FORM_CTRL_ID,

    MMIWWW_QUERY_GPRS_FORM_CTRL_ID,
    MMIWWW_QUERY_GPRS_NOTIFICATION_LABEL_CTRL_ID,
    MMIWWW_QUERY_NEVER_NOTIFY_LABEL_CTRL_ID,
    
    MMIWWW_WINDOWN_MANAER_LIST_CTRL_ID,
    MMIWWW_POPMENU_WINDOW_MANAGER_OPT_CTRL_ID,
    MMIWWW_TOOLBOX_FULL_DISPLAY_BUTTON_CTRL_ID,

    MMIWWW_MAX_CTRL_ID         // 控件ID的总数目

}MMIWWW_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : Register www win id name
//    Global resource dependence : none
//  Author: bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void MMIWWW_RegWinIdNameArr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif


