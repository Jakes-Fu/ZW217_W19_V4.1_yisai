/****************************************************************************
** File Name:      mmimtv_id.h                                            *
** Author:         xin.li                                                   *
** Date:           2008-12                                                  *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe window and control id.     *
****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 12/2008        xin.li           Create
****************************************************************************/
#ifdef CMMB_SUPPORT
#ifndef _MMIMTV_ID_H_
#define _MMIMTV_ID_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
#define WIN_ID_DEF(win_id)          win_id
 
// window ID
typedef enum
{
    MTV_WIN_ID_START = (MMI_MODULE_MTV << 16),
#include "mmimtv_id.def"

    MTV_MAX_WIN_ID
}MTV_WINDOW_ID_E;

#undef WIN_ID_DEF



// control id
typedef enum
{
    MTV_CTRL_ID_START = MTV_MAX_WIN_ID,

    //主菜单
    MTV_MENU_CTRL_ID, 

    //录制节目列表
    MTV_RECORD_LIST_OPT_CTRL_ID, //record list popmenu ctrl id

    //网络列表 / 选项
    MTV_NET_LIST_CTRL_ID,
    MTV_NET_OPTION_MENU_CTRL_ID,
    MTV_ID_UPDATE_NET,
    MTV_ID_DELETE_NET,

    //搜索菜单
    MTV_SEARCH_CTRL_ID,
 
    //频道列表 / 选项
    MTV_SERVICE_LIST_CTRL_ID,
    MTV_SERVICE_OPTION_MENU_CTRL_ID,
    MTV_ID_ESG,
    MTV_ID_UPDATE_SERVICE,
    MTV_ID_DELETE_SERVICE,
    MTV_ID_SORT_SERVICE,

    MTV_ID_EB_OPTION_SORT_BY_TIME,
    MTV_ID_EB_OPTION_SORT_BY_GRADE,
    MTV_ID_EB_OPTION_DELETE,
    MTV_ID_EB_OPTION_DELETE_ALL,
    MTV_ID_EB_RICHTEXT_CTRL_ID,

    MTV_LANGUAGE_LIST_CTRL_ID,
    MTV_SUBTITLE_LIST_CTRL_ID,
    MTV_MUTE_LIST_CTRL_ID,
    MTV_AUDIO_MODE_LIST_CTRL_ID,
    MTV_ZOOM_MODE_LIST_CTRL_ID,
    MTV_DISPLAY_PROGRAM_NAME_LIST_CTRL_ID,
    MTV_RECORD_STORE_LIST_CTRL_ID,
    MTV_EB_STORE_LIST_CTRL_ID,
    MTV_SNAPSHOT_STORE_LIST_CTRL_ID,
    MTV_TRANSPARENCY_LIST_CTRL_ID,
    MTV_OSD_DISPLAY_TIME_LIST_CTRL_ID,
    MTV_WATCH_TIP_LIST_CTRL_ID,
    MTV_BROADCAST_BG_PIC_LIST_CTRL_ID,
    MTV_ID_EB_LIST_CTRL_ID,
    MTV_ID_RECORD_FILE_TIME_CTRL_ID,
    MTV_BOOK_RECORD_LIST_CTRL_ID,
    MTV_SETTING_LIST_CTRL_ID,

    //MTV_MENU_CTRL_ID,
    //MTV_SERVICE_OPTION_MENU_CTRL_ID,
    //MTV_NET_OPTION_MENU_CTRL_ID,
    
    MTV_ICON_LABEL1_CTRL_ID,
    MTV_ICON_LABEL2_CTRL_ID,
    MTV_ICON_LABEL3_CTRL_ID,
    MTV_ICON_LABEL4_CTRL_ID,
    MTV_HELP_TEXT_CTRL_ID,
    MTV_ESG_TEXT_CTRL_ID,
    MTV_EDIT_RECORD_FILE_NAME_TEXT_CTRL_ID,

    MTV_EDIT_RECORD_FILE_TIME_TEXT_CTRL_ID,
    MTV_EDIT_RECORD_FILE_CALENDAR_TEXT_CTRL_ID,
    MTV_EDIT_RECORD_FILE_LONG_TEXT_CTRL_ID,

    MTV_EDIT_MANUAL_SEARCH_TEXT_CTRL_ID,
    MTV_EDIT_MANUAL_SEARCH_LABEL_CTRL_ID,

    MTV_SWITCH_PLAY_TAB_CTRL_ID,

#ifdef MBBMS_SUPPORT 
    MMIMBBMS_STARTUP_FORM_CTRL_ID,
    MMIMBBMS_ANIM_START_CTRL_ID,
    MMIMBBMS_ANIM_PROGRESS_CTRL_ID,
    MMIMBBMS_TEXT_START_CTRL_ID,

    MMIMBBMS_OPEN_FORM_CTRL_ID,
    MMIMBBMS_EDITBOX_OPEN_CTRL_ID,
    MMIMBBMS_DETAIL_FORM_CTRL_ID,
    MMIMBBMS_EDITBOX_DETAIL_CTRL_ID,

    MMIMBBMS_PAUSE_FORM_CTRL_ID,
    MMIMBBMS_EDITBOX_PAUSE_CTRL_ID,
    MMIMBBMS_PAUSE_FORM1_CTRL_ID,
    MMIMBBMS_PAUSE_FORM2_CTRL_ID,
    MMIMBBMS_LABEL_YES_CTRL_ID,
    MMIMBBMS_LABEL_NO_CTRL_ID,
    MMIMBBMS_BUTTON_YES_CTRL_ID,
    MMIMBBMS_BUTTON_NO_CTRL_ID,

	MMIMBBMS_SEARCH_FORM_CTRL_ID,
    MMIMBBMS_KEYWORD_FORM_CTRL_ID,
    MMIMBBMS_CHANNEL_FORM_CTRL_ID,
    MMIMBBMS_DATE_FORM_CTRL_ID,
    MMIMBBMS_PROGRAM_TYPE_FORM_CTRL_ID,
    MMIMBBMS_LABEL_KEYWORD_CTRL_ID,
    MMIMBBMS_EDITBOX_KEYWORD_CTRL_ID,
    MMIMBBMS_LABEL_CHANNEL_CTRL_ID,
    MMIMBBMS_DROPDOWNLIST_CHANNEL_CTRL_ID,
    MMIMBBMS_LABEL_DATE_CTRL_ID,
    MMIMBBMS_DROPDOWNLIST_DATE_CTRL_ID,
    MMIMBBMS_LABEL_PROGRAM_TYPE_CTRL_ID,
    MMIMBBMS_DROPDOWNLIST_PROGRAM_TYPE_CTRL_ID,

    MMIMBBMS_SETTING_FORM_CTRL_ID,
    MMIMBBMS_NOTIFY_TIME_FORM_CTRL_ID,
    MMIMBBMS_NETWORK_FORM_CTRL_ID,
    MMIMBBMS_NEW_CONNECTION_FORM_CTRL_ID,
    MMIMBBMS_NEW_CONNECTION_HELP_FORM_CTRL_ID,    
    MMIMBBMS_SG_DNS_FORM_CTRL_ID,
    MMIMBBMS_LABEL_NOTIFY_TIME_CTRL_ID,
    MMIMBBMS_DROPDOWNLIST_NOTIFY_TIME_CTRL_ID,
    MMIMBBMS_LABEL_NETWORK_CTRL_ID,
    MMIMBBMS_LABEL_NEW_CONNECTION_CTRL_ID, 
    MMIMBBMS_LABEL_NEW_CONNECTION_HELP_CTRL_ID, 
    MMIMBBMS_DROPDOWNLIST_NETWORK_CTRL_ID,
    MMIMBBMS_LABEL_SG_DNS_CTRL_ID,
    MMIMBBMS_EDITBOX_SG_DNS_CTRL_ID,

    MMIMBBMS_PROGRAM_DETAIL_FORM_CTRL_ID,
    MMIMBBMS_PROGRAM_DETAIL_FORM1_CTRL_ID,
    MMIMBBMS_ANIM_DETAIL_CTRL_ID,
    MMIMBBMS_TEXT_TIME_CTRL_ID,
    MMIMBBMS_TEXT_DETAIL_CTRL_ID,

    MMIMBBMS_LABEL_DAY_CTRL_ID,
    MMIMBBMS_SERVICE_PAUSE_OPT_MENU_CTRL_ID,
    MMIMBBMS_SEARCH_RESULT_LIST_CTRL_ID,
    MMIMBBMS_SEARCH_RST_OPT_MENU_CTRL_ID,

    MMIMBBMS_MENU_COLLECT_ITEM_ID,
    MMIMBBMS_MENU_DETAIL_ITEM_ID,
    MMIMBBMS_MENU_COMBO_MANAGE_ITEM_ID,

    // 节目单
    MMIMBBMS_PROGRAM_LIST_OPT_MENU_CTRL_ID,
    MMIMBBMS_MENU_WATCH_ITEM_ID,
    MMIMBBMS_PROGRAM_DETAIL_OPT_MENU_CTRL_ID,

  //订购
   MMIMBBMS_SUBSCRIBE_TYPE_MAIN_TAB_CTRL_ID,  
   
   MMIMBBMS_PURCHASEDITEM_LISTBOX_CTRL_ID,

   MMIMBBMS_LOCAL_PURCHASEITEM_LISTBOX_CTRL_ID,
#if 0   
   MMIMBBMS_PURCHASEITEM_LISTBOX_CTRL_ID,

   MMIMBBMS_SUBSCRIBE_TYPE_LISTBOX_CTRL_ID,
#endif   
    //订购
    MMIMBBMS_PURCHASEITEM_LIST_RICHTEXT_CTRL_ID,
    //订购详情
    MMIMBBMS_PURCHASEITEM_DETAIL_RICHTEXT_CTRL_ID,
    MMIMBBMS_PURCHASE_ORDER_RICHTEXT_CTRL_ID,

     //主界面部分
     MMIMBBMS_MAIN_INTERFACE_TAB_CTRL_ID,
     MMIMBBMS_ALL_SERVICE_LISTBOX_CTRL_ID,
     MMIMBBMS_FAVORITE_SERVICE_LISTBOX_CTRL_ID,
     MMIMBBMS_FAVORITE_CONTENT_LISTBOX_CTRL_ID,
     //主界面选项部分
     MMIMBBMS_MAIN_INTERFACE_POPMENU_CTRL_ID,
     MMIMBBMS_MAIN_INTERFACE_FS_POPMENU_CTRL_ID,
     MMIMBBMS_MAIN_INTERFACE_FC_POPMENU_CTRL_ID,
 
    // program list
    MMIMBBMS_PROGRAM_TAB_CTRL_ID,
    MMIMBBMS_DIALED_PROGRAM_LIST_CTRL_ID,
    MMIMBBMS_BUTTON_LEFT_CTRL_ID,
    MMIMBBMS_BUTTON_RIGHT_CTRL_ID,
    MMIMBBMS_EB_LIST_CTRL_ID,
    MMIMBBMS_EB_DETAIL_RICHTEXT_CTRL_ID,
	MMIMBBMS_HELP_INFO_TEXTBOX_CTRL_ID,

    //update win
    MMIMBBMS_LABEL_PLS_SELECT_CTL_ID,
//    MMIMBBMS_UPDATE_MAIN_LIST_CTRL_ID,
    
    MMIMBBMS_UPDATE_MENU_CTRL_ID,
    MMIMBBMS_CITY_UPDATE_FORM_CTRL_ID,
    MMIMBBMS_CITY_UPDATE_FORM1_CTRL_ID,
    MMIMBBMS_TEXT_CITY_UPDATE_CTRL_ID,
    MMIMBBMS_EDITTEXT_CITY_UPDATE_CTRL_ID,
    //设置界面
    MMIMBBMS_NEW_CONNECTION_HELP_INFO_TEXTBOX_CTRL_ID,
    //显示下发欢迎图片
    MMIMBBMS_PROGRAM_DETAIL_PREVIEW_DATA_CTRL_ID,
    MMIMBBMS_CHANGE_SERVICE_PREVIEW_dATA_CTRL_ID,
    MMIMBBMS_SIM_SELECT_LIST_CTRL_ID,
#endif
    MTV_MAX_CTRL_ID
} MTV_CONTROL_ID_E;


/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif  /* _MMIMTV_ID_H_ */
#endif