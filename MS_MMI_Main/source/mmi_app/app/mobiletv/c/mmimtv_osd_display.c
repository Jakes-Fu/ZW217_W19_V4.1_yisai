/*****************************************************************************
** File Name:      mmimtv_osd_display.c                                      *
** Author:                                                                   *
** Date:           2008-12	                                                 *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe mtv window table            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2008        xin.li           Create									 *
*****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "mmi_app_mobiletv_trc.h"
#ifdef CMMB_SUPPORT
#include "window_parse.h"
#include "mmk_timer.h"
#include "guires.h"
#include "mmi_menutable.h"
#include "mmimtv_id.h"
#include "mmidisplay_data.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmimultim_image.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_theme.h"
#include "mmidc_export.h"
#include "mmidc_display.h"
#include "mmimtv_osd_display.h"
#include "mmimtv_text.h"
#include "mmimtv_image.h"
#include "guimsgbox.h"
#include "mmimtv_data.h"
#include "mmicom_time.h"
#include "mmidc_gui.h"
#include "mmidc_camera_image.h"
#include "guilistbox.h"
#include "mmisms_image.h"
#include "mmimtv_main.h"
#include "mmimtv_data.h"
#include "guilabel.h"
#include "mmifmm_export.h"
#include "mmifmm_id.h"
#include "guirichtext.h"
#include "guitext.h"
#include "mmiudisk_export.h"
#include "mmisms_export.h"
#include "mmi_mainmenu_export.h"
#include "mmimms_export.h"
//#include "mmieng_win.h"
#include "mmieng_export.h"
#include "guiedit.h"
#include "guistring.h"
//#include "mmiset.h"
#include "mmiset_export.h"
//#include "mmiaudio.h"
#include "mmisrvaud_api.h"
#include "mmisrvmgr.h"

#include "mmiidle_export.h"
#include "mmiphone_export.h"
#include "mmitheme_statusbar.h"
#ifdef MBBMS_SUPPORT
#include "mmimbbms_wintab.h"
#include "mmimbbms_main.h"
#endif
#include "mmimtv_menutable.h"

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
typedef enum
{
    MTV_PUBWIN_TIP,
    MTV_PUBWIN_TIP_SR_OPT,
    MTV_PUBWIN_CONFIRM,
    MTV_PUBWIN_WAIT,
    MTV_PUBWIN_QUERY,
    MTV_PUBWIN_QUERY_SR_EXIT,
    MTV_PUBWIN_WAIT_NO_SOFTKEY,
    MTV_PUBWIN_TIP_NO_SOFTKEY
}MTV_PUBWIN_TYPE_E;

#define     MMIMTV_SYSTIME_SERVICE_NUMBER           1
#define     MMIMTV_SYSTIME_SHORTCUT_PANEL           (MMIMTV_SYSTIME_SERVICE_NUMBER * 2)
#define     MMIMTV_SYSTIME_VOLUME_PROGRESS_BAR      (MMIMTV_SYSTIME_SHORTCUT_PANEL * 2)
#define     MMIMTV_SYSTIME_WAITING_PUBWIN           (MMIMTV_SYSTIME_VOLUME_PROGRESS_BAR * 2)
#define     MMIMTV_SYSTIME_TEXT_SCROLL              (MMIMTV_SYSTIME_WAITING_PUBWIN * 2)
#define     MMIMTV_SYSTIME_WATCH_TIME_TIP           (MMIMTV_SYSTIME_TEXT_SCROLL * 2)
#define     MMIMTV_SYSTIME_WATCH_TIME_DISAPPEAR_TIP (MMIMTV_SYSTIME_WATCH_TIME_TIP * 2)
#define     MMIMTV_SYSTIME_TIP_PUBWIN               (MMIMTV_SYSTIME_WATCH_TIME_DISAPPEAR_TIP * 2)
#define     MMIMTV_SYSTIME_FILE_SAVED_TIP           (MMIMTV_SYSTIME_TIP_PUBWIN * 2)
#define		MMIMTV_MBBMS_PLAY_CONNECTING			(MMIMTV_SYSTIME_FILE_SAVED_TIP * 2)
#define     MMIMTV_SYSTIME_NUM_SWITCH               (MMIMTV_MBBMS_PLAY_CONNECTING * 2)

#define     MTV_DIGITAL_EDITBOX_MAX_DIGITS          6
#define     MMIMTV_SYSTEM_TIMER_UNIT                500
#define     MMIMTV_TEMP_STRING_BUFFER_MAX_LEN       30
#define     MMIMTV_LONG_TIME_MAX_DIGITS             5
#define     MMIMTV_SERVICE_NUMBER_MAX_DIGITS        5
#define     MMIMTV_RECORD_TIME_MAX_DIGITS           8
#define     MMIMTV_SERVICE_SHORTCUT_KEY_NUMBER      3
#define     MMIMTV_OSD_ICON_MENU_MAX_NUM            4   //OSD菜单的个数

#define     MMIMTV_3S_TIMEOUT                       3
#define     MMIMTV_5S_TIMEOUT                       5
#define     MMIMTV_10S_TIMEOUT                      10
#define     MMIMTV_15S_TIMEOUT                      15
#define     MMIMTV_SECONDS_PER_MINUTE               60
#define     MMIMTV_MINUTES_PER_HOUR                 60
#define     MMIMTV_SECONDS_PER_HOUR                 (60 * 60)

#define     MMIMTV_EBLINE1_BUFFER_MAX_LEN           50
#define     MMIMTV_EBLINE2_BUFFER_MAX_LEN           70
#define     MMIMTV_PROGRAM_HINT_CURRENT_NEXT_LEN    30

#define     MMIMTV_OSD_MASK_BG_COLOR                0xaab5
#define     MMIMTV_NORMAL_FONT                      MMI_DEFAULT_NORMAL_FONT
#define     MMIMTV_NORMAL_FONT_HEIGHT               18
#define     MMIMTV_BIG_FONT                         MMI_DEFAULT_BIG_FONT
#define     MMIMTV_SMALL_FONT                       MMI_DEFAULT_SMALL_FONT
//因小字号不含中文，仅用于显示数字或字符
#define     MMIMTV_STRING_FONT                      SONG_FONT_14


/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/
typedef struct MMIMTV_OSD_DISPLAY_DATA_tag
{
    uint8            sys_timer;
    uint8            waiting_step;
    uint8            systime_count;
    uint8            servicetime_count;
    uint8            volumetime_count;
    uint8            watch_time_tip_count;
    uint8            file_saved_tip_count;
	uint32			 play_connecting_count;
    BOOLEAN          is_scene_close;
    uint8            num_switch_count;
    
    uint8            switch_play_timer_id;
    uint8            switch_play_time_count;

    uint8            stb_disable_timer_id;
    uint8            stb_disable_timer_count;    
    
//    uint32           icon_text_index;
    uint32           systime_type;
    uint32           start_watch_time;
    uint32           prompt_times;               //提示次数
    wchar*           display_hint;
    DC_ICONS_OP*     osd_icon_list;
    uint32           wait_pubwin_text_id;   
    
    GUI_RECT_T       box_right;	
    GUI_RECT_T       box_left;
    GUI_RECT_T       box_middle;
    
    GUI_RECT_T       left_arrow_rect;           //the rect of arrow icon to change net
    GUI_RECT_T       right_arrow_rect;          //the rect of arrow icon to change net
    GUI_RECT_T       service_left_arrow;        //the rect of arrow icon to change service
    GUI_RECT_T       service_right_arrow;       //the rect of arrow icon to change service

    GUI_RECT_T       esg_service_left_arrow;    //the rect of arrow icon to change service esg
    GUI_RECT_T       esg_service_right_arrow;   //the rect of arrow icon to change service esg
    GUI_RECT_T       esg_date_left_arrow;       //the rect of arrow icon to change date esg
    GUI_RECT_T       esg_date_right_arrow;      //the rect of arrow icon to change date esg

    GUI_RECT_T       eb_left_arrow;             //the rect of arrow icon to change eb
    GUI_RECT_T       eb_right_arrow;            //the rect of arrow icon to change eb

    GUI_RECT_T       progress_rect;             //the rect of common progress for shortcuticon scene, brightness,constract etc.
    GUI_RECT_T       volume_progress_rect;      //the rect of volume progress
}MMIMTV_OSD_DISPLAY_DATA;

typedef struct MMIMTV_FMM_CALLBACK_INFO_T_tag
{
    MMIFMM_DATA_INFO_T  *list_data_ptr;
    uint32              index;
}MMIMTV_FMM_CALLBACK_INFO_T;

static MMIMTV_OSD_DISPLAY_DATA*     s_osd_display_data_ptr = PNULL;
#define MMIMTV_OSD_DISPLAY_PTR      (PNULL != s_osd_display_data_ptr ? s_osd_display_data_ptr : (MMIMTV_OSD_DISPLAY_DATA*)MMIMTV_Aassert())





// 定义状态栏信号的图标数组
LOCAL const MMI_IMAGE_ID_T s_signal_level_image_id[] = 
{
    IMAGE_IDLE_TITLE_ICON_TVSIGNAL1,
    IMAGE_IDLE_TITLE_ICON_TVSIGNAL2,
    IMAGE_IDLE_TITLE_ICON_TVSIGNAL3,
    IMAGE_IDLE_TITLE_ICON_TVSIGNAL4,
    IMAGE_IDLE_TITLE_ICON_TVSIGNAL5,
    IMAGE_IDLE_TITLE_ICON_TVSIGNAL6

//eb grade icon
};

//eb grade icon
LOCAL const MMI_IMAGE_ID_T  s_eb_grade_icon[MMIMTV_BROADCAST_MAX_LEVEL] =
{
	IMAGE_MTV_EB_NUMBER_1,
	IMAGE_MTV_EB_NUMBER_2,
	IMAGE_MTV_EB_NUMBER_3,
	IMAGE_MTV_EB_NUMBER_4
};

LOCAL  BOOLEAN s_is_open_swtich_service_tab = FALSE;

/*****************************************************************************/
// 	Description : handle record list option window msg
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRecordListOptWinMsg(
                                              MMI_WIN_ID_T win_id,
                                              MMI_MESSAGE_ID_E msg_id,        
                                              DPARAM param
                                              );

/*****************************************************************************/
// 	Description : handle MTV window msg
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditNameWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E	msg_id,
                                        DPARAM              param
                                        );

/*****************************************************************************/
// 	Description : handle MTV window msg
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditDateWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E	msg_id,
                                        DPARAM              param
                                        );

/*****************************************************************************/
// 	Description : handle MTV window msg
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditInputTimeWinMsg(
                                             MMI_WIN_ID_T        win_id,
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM              param
                                             );

/*****************************************************************************/
// 	Description : handle MTV window msg
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditInputLongWinMsg(
                                             MMI_WIN_ID_T        win_id,
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM              param
                                             );

/*****************************************************************************/
// 	Description : open MMIMTV_QUIT_QUERY scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void OpenPubWin(
                      MTV_PUBWIN_TYPE_E type,
                      uint32 string_id
                      );

/*****************************************************************************/
// 	Description : open MMIMTV_QUIT_QUERY scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void OpenPubWinByTextPtr(
                               MTV_PUBWIN_TYPE_E type,
                               MMI_STRING_T *text_ptr
                               );

/*****************************************************************************/
//  Description : stop text scroll timer
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void CreateDisappearTimer(uint32 type);

/*****************************************************************************/
// 	Description : 启动频道切换栏定时器
//	Global resource dependence : none
//  Author: xin.li
//	Note: 某Tab项停留超500ms，播放该频道
/*****************************************************************************/
LOCAL void StartSwitchPlayTabTimer(void);

/*****************************************************************************/
// 	Description : 停止频道切换栏定时器
//	Global resource dependence : none
//  Author: xin.li
//	Note: 某Tab项停留超500ms，播放该频道
/*****************************************************************************/
LOCAL void StopSwitchPlayTabTimer(void);



LOCAL WINDOW_TABLE(MMIMTV_MAIN_MENU_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_MENU_CTRL(MMIMTV_MAIN_MENU, MTV_MENU_CTRL_ID),
    END_WIN
};

#ifdef MBBMS_SUPPORT
//用于MBBMS无信号时兼容CMMB功能, 此菜单前两项在无MBBMS信号时影射为网络列表/搜索
LOCAL WINDOW_TABLE(MMIMTV_MAIN_MENU2_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_MENU_CTRL(MMIMTV_MAIN_MENU2, MTV_MENU_CTRL_ID),
    END_WIN
};
LOCAL WINDOW_TABLE(MMIMTV_MAIN_MENU3_WIN_TAB) = 
{
    CREATE_MENU_CTRL(MMIMTV_MAIN_MENU3, MTV_MENU_CTRL_ID),
    END_WIN
};
#endif

LOCAL WINDOW_TABLE(MMIMTV_SEARCH_MENU_WIN_TAB) =
{
    //CLEAR_LCD,
    CREATE_MENU_CTRL(MMIMTV_SEARCH_MENU, MTV_SEARCH_CTRL_ID),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_BOOK_RECORD_SETTING_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MTV_BOOK_RECORD_LIST_CTRL_ID),  
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_HELP_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_TEXT_CTRL(MTV_HELP_TEXT_CTRL_ID),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_ESG_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MTV_ESG_TEXT_CTRL_ID),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_EDIT_RECORD_FILE_NAME_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleEditNameWinMsg),
    WIN_ID(MMIMTV_EDIT_RECORD_FILE_NAME_ID),
    WIN_TITLE(TXT_MTV_RECORD_FILE_NAME),    
    CREATE_EDIT_TEXT_CTRL(MMIMTV_BOOK_RECORD_FILE_NAME_MAX_LEN,MTV_EDIT_RECORD_FILE_NAME_TEXT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_EDIT_RECORD_FILE_TIME_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MTV_ID_RECORD_FILE_TIME_CTRL_ID),  
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_EDIT_RECORD_FILE_DATE_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleEditDateWinMsg),
    WIN_ID(MMIMTV_EDIT_RECORD_FILE_DATE_ID),
    WIN_TITLE(TXT_DATE), 
    CREATE_EDIT_DATE_CTRL(MTV_EDIT_RECORD_FILE_CALENDAR_TEXT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_EDIT_RECORD_FILE_TIME2_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleEditInputTimeWinMsg),
    WIN_ID(MMIMTV_EDIT_RECORD_FILE_TIME_ID),
    WIN_TITLE(TXT_TIME),
    CREATE_EDIT_TIME_CTRL(MTV_EDIT_RECORD_FILE_TIME_TEXT_CTRL_ID),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_EDIT_RECORD_FILE_LONG_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleEditInputLongWinMsg),
    WIN_ID(MMIMTV_EDIT_RECORD_FILE_TIME_ID),
    WIN_TITLE(TXT_TIME),
    CREATE_EDIT_DIGITAL_CTRL(3,MTV_EDIT_RECORD_FILE_LONG_TEXT_CTRL_ID),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_MANUAL_SEARCH_WIN_TAB ) = 
{
    //CLEAR_LCD,
    CREATE_EDIT_DIGITAL_CTRL(MTV_DIGITAL_EDITBOX_MAX_DIGITS, MTV_EDIT_MANUAL_SEARCH_TEXT_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MTV_EDIT_MANUAL_SEARCH_LABEL_CTRL_ID),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_ICON_LABEL_WIN_TAB ) = 
{
    //CLEAR_LCD,
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_RIGHT, MTV_ICON_LABEL1_CTRL_ID),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_SETTING_LIST_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MTV_SETTING_LIST_CTRL_ID),  
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_LANGUAGE_LIST_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MTV_LANGUAGE_LIST_CTRL_ID),  
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_SUBTITLE_LIST_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MTV_SUBTITLE_LIST_CTRL_ID),  
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_AUDIO_MODE_LIST_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MTV_AUDIO_MODE_LIST_CTRL_ID),  
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_ZOOM_MODE_LIST_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MTV_ZOOM_MODE_LIST_CTRL_ID),  
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_RECORD_STORE_LIST_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MTV_RECORD_STORE_LIST_CTRL_ID),  
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_SNAPSHOT_LIST_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MTV_SNAPSHOT_STORE_LIST_CTRL_ID),  
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_TRANSPARENCY_LIST_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MTV_TRANSPARENCY_LIST_CTRL_ID),  
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_DISPLAY_TIME_LIST_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MTV_OSD_DISPLAY_TIME_LIST_CTRL_ID),  
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_WATCH_TIP_LIST_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MTV_WATCH_TIP_LIST_CTRL_ID),  
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_BROADCAST_BG_PIC_LIST_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MTV_BROADCAST_BG_PIC_LIST_CTRL_ID),  
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_SERVICE_LIST_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MTV_SERVICE_LIST_CTRL_ID),  
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_NET_LIST_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MTV_NET_LIST_CTRL_ID),  
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_SERVICE_OPTION_MENU_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_MENU_CTRL(MMIMTV_SERVICE_OPTION_MENU, MTV_SERVICE_OPTION_MENU_CTRL_ID),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_NET_OPTION_MENU_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_MENU_CTRL(MMIMTV_NET_OPTION_MENU, MTV_NET_OPTION_MENU_CTRL_ID),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_SYSTEM_INFOR_MENU_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_MENU_CTRL(MMIMTV_SYSTEM_INFOR_MENU, MTV_ID_SYSTEM_INFOR_CTRL_MENU),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_OTHER_FUNCTION_MENU_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_MENU_CTRL(MMIMTV_OTHER_FUNCTION_MENU, MTV_ID_OTHER_FUNCTIONS_CTRL_MENU),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_EB_LIST_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MTV_ID_EB_LIST_CTRL_ID), 
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_EB_LIST_OPT_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_MENU_CTRL(MMIMTV_EB_LIST_OPT_MENU, MTV_ID_EB_OPTION_CTRL_MENU),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_EB_LIST_CONTENT_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_RICHTEXT_CTRL(MTV_ID_EB_RICHTEXT_CTRL_ID),
    END_WIN
};

LOCAL WINDOW_TABLE( MMIMTV_POPMENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleRecordListOptWinMsg),
    WIN_ID(MMIMTV_POPMENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MMIMTV_RECORD_LIST_OPTION_MENU,MTV_RECORD_LIST_OPT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

LOCAL WINDOW_TABLE(MMIMTV_SWITCH_PLAY_WIN_TAB) = 
{
    //CLEAR_LCD,
    CREATE_TAB_CTRL(GUI_TAB_ITEM_MIDDLE, GUITAB_STATE_SINGLE_LINE, MTV_SWITCH_PLAY_TAB_CTRL_ID),
    END_WIN
};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          CONSTANT VARIABLES                               */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : fill rect in mtv
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_NewOSDDisplayData(void)
{
    //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_NewOSDDisplayData"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_586_112_2_18_2_40_40_860,(uint8*)"");
    
    if(PNULL == s_osd_display_data_ptr)
    {
        s_osd_display_data_ptr = (MMIMTV_OSD_DISPLAY_DATA*)SCI_ALLOC_APP(sizeof(MMIMTV_OSD_DISPLAY_DATA));
        SCI_MEMSET(s_osd_display_data_ptr, 0, sizeof(MMIMTV_OSD_DISPLAY_DATA));
        s_osd_display_data_ptr->is_scene_close = TRUE;  
    }
}

/*****************************************************************************/
// 	Description : fill rect in mtv
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DeleteOSDDisplayData(void)
{
    if(PNULL != s_osd_display_data_ptr)
    {
        SCI_Free(s_osd_display_data_ptr);
        s_osd_display_data_ptr = PNULL;
    }
}

/*****************************************************************************/
// 	Description : fill rect in mtv
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void ClearRect(GUI_RECT_T rect)
{
    if(MMK_IsFocusWin(MMIMTV_MAIN_WIN_ID))
    {
        GUI_LCD_DEV_INFO dev_info_ptr= {0};

        GUI_FillRect(&dev_info_ptr, rect, MMIMTV_TRANSPARENT_COLOR);
    }
}

/*****************************************************************************/
// 	Description : Display normal string
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void DisplayString(
                         GUI_RECT_T rect,
                         wchar* str,
                         uint16 len,
                         GUI_FONT_T font
                         )
{
    if(MMK_IsFocusWin(MMIMTV_MAIN_WIN_ID))
    {
        GUI_LCD_DEV_INFO dev_info_ptr= {0};
        GUISTR_STYLE_T      text_style = {0};
        GUISTR_STATE_T      state = GUISTR_STATE_EFFECT | GUISTR_STATE_NOT_HALF_CHAR | GUISTR_STATE_WORDBREAK;
        MMI_STRING_T        text_str = {0};

        // text style
        text_style.char_space = 1;
        text_style.line_space = 1;
        text_style.effect = FONT_EFFECT_NONE;
        text_style.font = font;
        text_style.font_color = MMI_WHITE_COLOR;
        // string
        text_str.wstr_len = len;
        text_str.wstr_ptr = str;
        // display
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&dev_info_ptr,
            (const GUI_RECT_T      *)&rect,       //the fixed display area
            (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&text_str,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
    }
}

/*****************************************************************************/
// 	Description : display osd mask picture
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: 用特定OSD背景图填充制定区域
/*****************************************************************************/
LOCAL void FillRectWithOSDMaskPicture(
                                      GUI_RECT_T rect
                                      )
{
    MMI_IMAGE_ID_T image_id = IMG_DC_PNG_MASK;
    
    if(MMK_IsFocusWin(MMIMTV_MAIN_WIN_ID))
    {
        GUI_LCD_DEV_INFO dev_info_ptr= {0};
        
        GUIRES_DisplayImg(PNULL,
            &rect,
            PNULL,
            MMIMTV_MAIN_WIN_ID,
            image_id,
            &dev_info_ptr);
    }
}

/*****************************************************************************/
// 	Description : Display effect string
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: 用以背景色为纯黑(TV Color Key)的矢量文本显示
/*****************************************************************************/
LOCAL void DisplayEffectString(
                               GUI_RECT_T rect,
                               wchar* str,
                               uint16 len,
                               GUI_FONT_T font,
                               BOOLEAN is_auto_ellipsis
                               )
{
    GUI_RECT_T real_rect = rect;
    
    if(!is_auto_ellipsis)
    {
        real_rect.right = real_rect.left + GUI_CalculateStringPiexlNum(str, len, (GUI_FONT_T)font, 1) + 1;
    }
    
    if(MMK_IsFocusWin(MMIMTV_MAIN_WIN_ID))
    {
        GUI_LCD_DEV_INFO    dev_info_ptr= {0};
        GUISTR_STYLE_T      text_style = {0};
        GUISTR_STATE_T      state = GUISTR_STATE_EFFECT | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_NOT_HALF_CHAR;
        MMI_STRING_T        text_str = {0};
        
        // text style
        if(is_auto_ellipsis)
        {
            state = state | GUISTR_STATE_ELLIPSIS;
        }
        text_style.align = ALIGN_LEFT;
        text_style.char_space = 1;
        text_style.line_space = 1;
        text_style.effect = FONT_EFFECT_CUSTOM;
        text_style.font = font;
        text_style.font_color = MMI_WHITE_COLOR;
        text_style.edge_color = MMI_BACKGROUND_COLOR;
        // string
        text_str.wstr_len = len;
        text_str.wstr_ptr = str;
        
        //fill rect with osd mask background
        FillRectWithOSDMaskPicture(real_rect);
        
        // display
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&dev_info_ptr,
            (const GUI_RECT_T      *)&real_rect,       //the fixed display area
            (const GUI_RECT_T      *)&real_rect,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&text_str,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
    }
}

/*****************************************************************************/
// 	Description : Display effect string
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: 用以背景色为纯黑(TV Color Key)的矢量文本显示
/*****************************************************************************/
LOCAL void DisplayEffectStringByDevPtr(
                                       GUI_LCD_DEV_INFO *dev_ptr,
                                       GUI_RECT_T rect,
                                       wchar* str,
                                       uint16 len,
                                       GUI_FONT_T font,
                                       BOOLEAN is_auto_ellipsis
                                       )
{
    GUI_RECT_T real_rect = rect;
    
    if(!is_auto_ellipsis)
    {
        real_rect.right = real_rect.left + GUI_CalculateStringPiexlNum(str, len, (GUI_FONT_T)font, 1) + 1;
    }
    
    if(MMK_IsFocusWin(MMIMTV_MAIN_WIN_ID))
    {
        GUISTR_STYLE_T      text_style = {0};
        GUISTR_STATE_T      state = GUISTR_STATE_NONE | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_NOT_HALF_CHAR;
        MMI_STRING_T        text_str = {0};
        
        // text style
        if(is_auto_ellipsis)
        {
            state = state | GUISTR_STATE_ELLIPSIS;
        }
        text_style.align = ALIGN_LEFT;
        text_style.char_space = 1;
        text_style.line_space = 1;
        text_style.effect = FONT_EFFECT_NONE;
        text_style.font = font;
        text_style.font_color = MMI_WHITE_COLOR;
        text_style.edge_color = MMI_BACKGROUND_COLOR;
        // string
        text_str.wstr_len = len;
        text_str.wstr_ptr = str;
        
        //fill rect with osd mask background
        //FillRectWithOSDMaskPicture(real_rect);
        
        // display
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)dev_ptr,
            (const GUI_RECT_T      *)&real_rect,       //the fixed display area
            (const GUI_RECT_T      *)&real_rect,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&text_str,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
    }
}

/*****************************************************************************/
// 	Description : display picture
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void DisplayPicture(
                          uint16 left, 
                          uint16 top,
                          MMI_IMAGE_ID_T image_id
                          )
{
    GUI_POINT_T         dis_point = {0};

    if(MMK_IsFocusWin(MMIMTV_MAIN_WIN_ID))
    {
        GUI_LCD_DEV_INFO dev_info_ptr= {0};
        
        dis_point.x = left;
        dis_point.y = top;
        GUIRES_DisplayImg(&dis_point,
                PNULL,
                PNULL,
                MMIMTV_MAIN_WIN_ID,
                image_id,
                &dev_info_ptr);
    }
}

/*****************************************************************************/
// 	Description : display picture in rect
//	Global resource dependence : 
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void DisplayPictureInRect(
                                GUI_RECT_T      *rect_ptr,
                                MMI_IMAGE_ID_T  image_id
                                )
{
    if(MMK_IsFocusWin(MMIMTV_MAIN_WIN_ID))
    {
        GUI_LCD_DEV_INFO dev_info_ptr= {0};
        
        GUIRES_DisplayImg(PNULL,
            rect_ptr,
            PNULL,
            MMIMTV_MAIN_WIN_ID,
            image_id,
            &dev_info_ptr);
    }
}

/*****************************************************************************/
// 	Description : fill rect in mtv
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void DisplayPictureByDevPtr(
                                  GUI_LCD_DEV_INFO *dev_ptr,
                                  uint16 left, 
                                  uint16 top,
                                  MMI_IMAGE_ID_T image_id
                                  )
{
    GUI_POINT_T         dis_point = {0};

    if(MMK_IsFocusWin(MMIMTV_MAIN_WIN_ID))
    {
        dis_point.x = left;
        dis_point.y = top;
        GUIRES_DisplayImg(&dis_point,
                PNULL,
                PNULL,
                MMIMTV_MAIN_WIN_ID,
                image_id,
                dev_ptr);
    }
}

/*****************************************************************************/
// 	Description : display soft key base hor or ver screen
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void DisplaySoftKey(
                          uint32 left_string_id,
                          uint32 middle_string_id, 
                          uint32 right_string_id
                          )
{
    MMI_STRING_T text_str = {0};
    GUI_RECT_T   *right_rect_ptr = &(MMIMTV_OSD_DISPLAY_PTR->box_right);
    GUI_RECT_T   *middle_rec_ptr = &(MMIMTV_OSD_DISPLAY_PTR->box_middle);
    GUI_RECT_T   *left_rect_ptr = &(MMIMTV_OSD_DISPLAY_PTR->box_left);

    if(MMIMTV_IsVRunning())
    {
        ClearRect(MMIMTV_OSD_DISPLAY_PTR->box_right);
        ClearRect(MMIMTV_OSD_DISPLAY_PTR->box_middle);
        ClearRect(MMIMTV_OSD_DISPLAY_PTR->box_left);
    }
 
    MMIMTV_OSD_DISPLAY_PTR->box_left.bottom = MMIMTV_LCD_HEIGHT - 1 - 1;
    MMIMTV_OSD_DISPLAY_PTR->box_left.right = MMIMTV_LCD_WIDTH - 1 - 1;
    
    MMIMTV_OSD_DISPLAY_PTR->box_right.top = 2;
    MMIMTV_OSD_DISPLAY_PTR->box_right.right = MMIMTV_LCD_WIDTH - 1 - 1;
    
    MMIMTV_OSD_DISPLAY_PTR->box_middle.top = MMIMTV_OSD_DISPLAY_PTR->box_middle.bottom = MMIMTV_LCD_HEIGHT / 2;
    MMIMTV_OSD_DISPLAY_PTR->box_middle.right = MMIMTV_LCD_WIDTH - 1 - 1;

    if(TXT_NULL != right_string_id)
    {
        MMI_GetLabelTextByLang(right_string_id, &text_str);
        MMIMTV_OSD_DISPLAY_PTR->box_right.left = MMIMTV_OSD_DISPLAY_PTR->box_right.right - \
            GUI_CalculateStringPiexlNum(text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT, 1);
        MMIMTV_OSD_DISPLAY_PTR->box_right.bottom = MMIMTV_OSD_DISPLAY_PTR->box_right.top + \
            GUI_GetStringHeight(MMIMTV_NORMAL_FONT, text_str.wstr_ptr, text_str.wstr_len) + 2;
        DisplayEffectString(MMIMTV_OSD_DISPLAY_PTR->box_right, text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT, FALSE);
    }
    else
    {
        SCI_MEMSET(right_rect_ptr, 0, (sizeof(GUI_RECT_T)));
    }
    if(TXT_NULL != middle_string_id)
    {
        MMI_GetLabelTextByLang(middle_string_id, &text_str);
        MMIMTV_OSD_DISPLAY_PTR->box_middle.left = MMIMTV_OSD_DISPLAY_PTR->box_middle.right - \
            GUI_CalculateStringPiexlNum(text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT, 1);
        MMIMTV_OSD_DISPLAY_PTR->box_middle.top -= (GUI_GetStringHeight(MMIMTV_NORMAL_FONT, text_str.wstr_ptr, text_str.wstr_len) + 2) / 2;
        MMIMTV_OSD_DISPLAY_PTR->box_middle.bottom += (GUI_GetStringHeight(MMIMTV_NORMAL_FONT, text_str.wstr_ptr, text_str.wstr_len) + 1) / 2;
        DisplayEffectString(MMIMTV_OSD_DISPLAY_PTR->box_middle, text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT, FALSE);
    }
    else
    {
        SCI_MEMSET(middle_rec_ptr, 0, sizeof(GUI_RECT_T));
    }
    if(TXT_NULL != left_string_id)
    {
        MMI_GetLabelTextByLang(left_string_id, &text_str);
        MMIMTV_OSD_DISPLAY_PTR->box_left.top = MMIMTV_OSD_DISPLAY_PTR->box_left.bottom - \
            (GUI_GetStringHeight(MMIMTV_NORMAL_FONT, text_str.wstr_ptr, text_str.wstr_len) + 2);
        MMIMTV_OSD_DISPLAY_PTR->box_left.left = MMIMTV_OSD_DISPLAY_PTR->box_left.right - \
            GUI_CalculateStringPiexlNum(text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT, 1);
        DisplayEffectString(MMIMTV_OSD_DISPLAY_PTR->box_left, text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT, FALSE);
    }
    else
    {
        SCI_MEMSET(left_rect_ptr, 0, sizeof(GUI_RECT_T));
    }
}

/*****************************************************************************/
// 	Description : display soft key base hor or ver screen
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_GetSoftKeyRect(
                                  GUI_RECT_T *left_rect,
                                  GUI_RECT_T *middle_rect,
                                  GUI_RECT_T *right_rect
                                  )
{
    SCI_MEMCPY(left_rect, &(MMIMTV_OSD_DISPLAY_PTR->box_left), sizeof(GUI_RECT_T));
    SCI_MEMCPY(middle_rect, &(MMIMTV_OSD_DISPLAY_PTR->box_middle), sizeof(GUI_RECT_T));
    SCI_MEMCPY(right_rect, &(MMIMTV_OSD_DISPLAY_PTR->box_right), sizeof(GUI_RECT_T));
}

/*****************************************************************************/
// 	Description : display soft key base hor or ver screen
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void DisplayTitle(uint32 text_id)
{
    MMI_STRING_T text_str = {0};
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP - MTV_TITLE_HEIGHT, MTV_OSD_RIGHT, MTV_OSD_TOP};
    uint16 fontwidth = 0;
   
    DisplayPictureInRect(&rect, IMAGE_TITLE_BAR2);
    MMI_GetLabelTextByLang(text_id, &text_str);
    fontwidth = GUI_CalculateStringPiexlNum(text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT, 1);
    rect.left = rect.left + (rect.right - rect.left - fontwidth) / 2;
    rect.top += MMIMTV_TITLE_STRING_OFFSET;
    DisplayString(rect, text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT);
}

/*****************************************************************************/
// 	Description : display soft key base hor or ver screen
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void ClearTitle(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP - MTV_TITLE_HEIGHT, MTV_OSD_RIGHT, MTV_OSD_TOP};
    ClearRect(rect);
}

/*****************************************************************************/
// 	Description :open MMIMTV_SERVICE_LIST scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void AppendListItemForBookRecordViaId(
                                            MTV_CONTROL_ID_E ctrl_id, 
                                            MTV_MMI_TEXT_ID_E text_id, 
                                            wchar *value
                                            )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    item_t.item_style    = GUIITEM_STYLE_ONE_TWO_LINE_NUMBER_TEXT_AND_TEXT;
    item_t.item_data_ptr = &item_data;    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;
    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(value);
    
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = value;

    //MMIAPICOM_Wstrcpy(item_data.item_content[1].item_data.text_buffer.wstr, value);
    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
// 	Description :open MMIMTV_SERVICE_LIST scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void AppendListItemForSettingViaId(
                                         MTV_CONTROL_ID_E  ctrl_id, 
                                         MTV_MMI_TEXT_ID_E text1_id,
                                         MTV_MMI_TEXT_ID_E text2_id,
                                         uint32            user_data  
                                         )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    item_t.item_style    = GUIITEM_STYLE_ONE_TWO_LINE_NUMBER_TEXT_AND_TEXT;
    item_t.user_data     = user_data;
    item_t.item_data_ptr = &item_data;
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text1_id;
    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].item_data.text_id = text2_id;
    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
// 	Description : open OSD_MAIN_MENU scene
//	Global resource dependence :
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_OpenMainMenuScene(uint32 param)
{
    uint32      first_index = 0;
    GUI_RECT_T  menu_rect = {0};
    MMI_MENU_GROUP_ID_T  group_id = 0;
    MMI_MENU_ID_T        menu_id = 0;

#ifndef MBBMS_SUPPORT
    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_MAIN_MENU_WIN_TAB);
#else
    
    if(MMIMBBMS_IsCurPlayServiceInFavoriteList())
    {
        //当前频道已收藏
        MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_MAIN_MENU3_WIN_TAB);
    }
    else
    {
        //当前频道未收藏
        MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_MAIN_MENU_WIN_TAB);
    }
#endif
    //set menu rect
    menu_rect.left   = MTV_OSD_LEFT;
    menu_rect.top    = MTV_OSD_TOP;
    menu_rect.right  = MTV_OSD_RIGHT;
    menu_rect.bottom = MTV_OSD_BOTTOM;
    GUIMENU_SetRect(MTV_MENU_CTRL_ID,&menu_rect);
    GUIMENU_SetMenuBgColor(MTV_MENU_CTRL_ID, MMIMTV_BACKGROUND_COLOR);
    GUIMENU_SetMenuFontColor(MTV_MENU_CTRL_ID, MMI_GRAY_WHITE_COLOR);
    
    if(MBBMS_SERVICE_MODE_CMMB == MMIMBBMS_GetServiceMode())
    {
        GUIMENU_GetId(MTV_MENU_CTRL_ID, &group_id, &menu_id);
        GUIMENU_SetItemGrayed(MTV_MENU_CTRL_ID, group_id, menu_id, TRUE);
    }
    
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_MENU_CTRL_ID);
    if(param >= MMIMTV_ITEM_MAX_INDEX)
    {
        first_index = param - MMIMTV_ITEM_MAX_INDEX + 1;
    }
    GUIMENU_SetFirstAndSelectedItem(first_index, param, MTV_MENU_CTRL_ID);
    
    DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    DisplayTitle(TXT_COMM_MENU);
}

/*****************************************************************************/
// 	Description : close MMIMTV_MAIN_MENU scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseMainMenuScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_MENU_CTRL_ID);
    ClearRect(rect);
    ClearTitle();
}

/*****************************************************************************/
// 	Description : open OSD_SEARCH_MENU scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenSearchMenuScene(uint32 param)
{
    uint32      first_index = 0;
    GUI_RECT_T  menu_rect = {0};

    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_SEARCH_MENU_WIN_TAB);

    //set menu rect
    menu_rect.left   = MTV_OSD_LEFT;
    menu_rect.top    = MTV_OSD_TOP;
    menu_rect.right  = MTV_OSD_RIGHT;
    menu_rect.bottom = MTV_OSD_BOTTOM;
    GUIMENU_SetRect(MTV_SEARCH_CTRL_ID,&menu_rect);
    GUIMENU_SetMenuBgColor(MTV_SEARCH_CTRL_ID, MMIMTV_BACKGROUND_COLOR);
    GUIMENU_SetMenuFontColor(MTV_SEARCH_CTRL_ID, MMI_GRAY_WHITE_COLOR);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_SEARCH_CTRL_ID);
    if(param >= MMIMTV_ITEM_MAX_INDEX)
    {
        first_index = param - MMIMTV_ITEM_MAX_INDEX + 1;
    }
    GUIMENU_SetFirstAndSelectedItem(first_index, param, MTV_SEARCH_CTRL_ID);
    DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    DisplayTitle(STXT_SEARCH);
}

/*****************************************************************************/
// 	Description : close OSD_SEARCH_MENU scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseSearchMenuScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_SEARCH_CTRL_ID);
    ClearRect(rect);
    ClearTitle();
}

/*****************************************************************************/
// 	Description : display volume progress
//	Global resource dependence :
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_DisplayVolumeProgress(MMIMTV_VOLUME_E volume)
{
    uint16 icon_width = 0;
    uint16 icon_height = 0;
    uint16 offset_x = 0;
    uint16 progress_width = 0;
    uint16 progress_height = 0;

    GUIRES_GetImgWidthHeight(&icon_width,&icon_height,IMAGE_MTV_VOLUME_PROGRESS_BG,MMIMTV_MAIN_WIN_ID);
    GUIRES_GetImgWidthHeight(&progress_width,&progress_height,IMAGE_MTV_VOLUME_PROGRESS_1,MMIMTV_MAIN_WIN_ID);
    offset_x = (icon_width - progress_width) / 2;
    
    if(volume >= MMIMTV_VOLUME_LEVEL_MAX)
    {
        return;
    }
    
    MMIMTV_OSD_DISPLAY_PTR->volume_progress_rect.left = MMIMTV_LCD_WIDTH / 32;
    MMIMTV_OSD_DISPLAY_PTR->volume_progress_rect.top = (MMIMTV_LCD_HEIGHT - icon_height) / 2;
    MMIMTV_OSD_DISPLAY_PTR->volume_progress_rect.right = MMIMTV_OSD_DISPLAY_PTR->volume_progress_rect.left + icon_width;
    MMIMTV_OSD_DISPLAY_PTR->volume_progress_rect.bottom = MMIMTV_OSD_DISPLAY_PTR->volume_progress_rect.top + icon_height;

    DisplayPicture(MMIMTV_OSD_DISPLAY_PTR->volume_progress_rect.left, MMIMTV_OSD_DISPLAY_PTR->volume_progress_rect.top, IMAGE_MTV_VOLUME_PROGRESS_BG);
    if(0 == volume)
    {
        DisplayPicture(MMIMTV_OSD_DISPLAY_PTR->volume_progress_rect.left, MMIMTV_OSD_DISPLAY_PTR->volume_progress_rect.top + MMIMTV_VOL_MUTE_OFFSET_Y, IMAGE_MTV_MUTE_PROGRESS_BG);
    }
    else
    {
        DisplayPicture(MMIMTV_OSD_DISPLAY_PTR->volume_progress_rect.left + offset_x, MMIMTV_OSD_DISPLAY_PTR->volume_progress_rect.top + MMIMTV_VOL_OFFSET_Y, IMAGE_MTV_VOLUME_PROGRESS_1 + volume - 1);
    }
    CreateDisappearTimer(MMIMTV_SYSTIME_VOLUME_PROGRESS_BAR);
    MMIMTV_OSD_DISPLAY_PTR->volumetime_count = 0;
}

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ClearVolumeProgress(void)
{
    GUI_RECT_T  *rect_ptr = &(MMIMTV_OSD_DISPLAY_PTR->volume_progress_rect);

    ClearRect(MMIMTV_OSD_DISPLAY_PTR->volume_progress_rect);
    SCI_MEMSET(rect_ptr, 0, sizeof(GUI_RECT_T));
    MMIMTV_StopSysTimer(MMIMTV_SYSTIME_VOLUME_PROGRESS_BAR);
}

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_GetProgressRect(GUI_RECT_T *rect)
{
    SCI_MEMCPY(rect, &(MMIMTV_OSD_DISPLAY_PTR->progress_rect), sizeof(GUI_RECT_T));
}

/*****************************************************************************/
// 	Description : get volume progress rect
//	Global resource dependence :
//  Author:
//	Note: if volume progress pannel is not opened, rect info is null
/*****************************************************************************/
PUBLIC void MMIMTV_GetVolumeProgressRect(GUI_RECT_T *rect)
{
    SCI_MEMCPY(rect, &(MMIMTV_OSD_DISPLAY_PTR->volume_progress_rect), sizeof(GUI_RECT_T));
}

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void ClearLabelText(MMI_CTRL_ID_T ctrl_id)
{
    GUI_RECT_T rect = {0};

    GUILABEL_GetRect(ctrl_id,&rect);
    MMK_DestroyControl(ctrl_id);
    ClearRect(rect);
}

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void DisplayLabelText(
                            uint32 x,
                            uint32 y,
                            uint32 string_id
                            )
{
    MMI_STRING_T text_str = {0};
    GUI_RECT_T rect = {0};
    GUI_BG_T    label_bg = {0};
    
    rect.left = 0;
    rect.top = (int16)y;
    rect.right = (int16)x;
    rect.bottom = (int16)((int32)y + MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconGetIconWidth());
    MMI_GetLabelTextByLang(string_id, &text_str);
    ClearLabelText(MTV_ICON_LABEL1_CTRL_ID);

    //计算字符串实际宽度
    rect.left = rect.right - (GUI_CalculateStringPiexlNum(text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT, 1) + 1);
    
    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_ICON_LABEL_WIN_TAB);

    label_bg.bg_type = GUI_BG_IMG;
    label_bg.img_id = IMG_DC_PNG_MASK;
    //label_bg.bg_type = GUI_BG_COLOR;
    //label_bg.color = MMIMTV_OSD_MASK_BG_COLOR;
    GUILABEL_SetRect(MTV_ICON_LABEL1_CTRL_ID, &rect, FALSE);
    GUILABEL_SetBg(MTV_ICON_LABEL1_CTRL_ID,&label_bg);
    GUILABEL_SetFont(MTV_ICON_LABEL1_CTRL_ID,MMIMTV_NORMAL_FONT,MMI_WHITE_COLOR);
    GUILABEL_SetText(MTV_ICON_LABEL1_CTRL_ID, &text_str, TRUE);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_ICON_LABEL1_CTRL_ID);
}

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void ChangeLabelText(
                           uint32 string_id
                           )
{
    MMI_STRING_T text_str = {0};
    GUI_RECT_T rect = {0};
    GUI_BG_T    label_bg = {0};

    MMI_GetLabelTextByLang(string_id, &text_str);
    GUILABEL_GetRect(MTV_ICON_LABEL1_CTRL_ID,&rect);
    ClearLabelText(MTV_ICON_LABEL1_CTRL_ID);

    //计算字符串实际宽度
    rect.left = rect.right - (GUI_CalculateStringPiexlNum(text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT, 1) + 1);
    
    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_ICON_LABEL_WIN_TAB);

    label_bg.bg_type = GUI_BG_IMG;
    label_bg.img_id = IMG_DC_PNG_MASK;
    //label_bg.bg_type = GUI_BG_COLOR;
    //label_bg.color = MMIMTV_OSD_MASK_BG_COLOR;
    GUILABEL_SetRect(MTV_ICON_LABEL1_CTRL_ID, &rect, FALSE);
    GUILABEL_SetBg(MTV_ICON_LABEL1_CTRL_ID,&label_bg);
    GUILABEL_SetFont(MTV_ICON_LABEL1_CTRL_ID,MMIMTV_NORMAL_FONT,MMI_WHITE_COLOR);
    GUILABEL_SetText(MTV_ICON_LABEL1_CTRL_ID, &text_str, TRUE);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_ICON_LABEL1_CTRL_ID);
}

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void DisplayBrightnessProgress(
                                     uint32 x,
                                     uint32 y
                                     )
{
    uint16 icon_width = 0;
    uint16 icon_height = 0;
    uint16 offset_y = 0;
    uint16 progress_width = 0;
    uint16 progress_height = 0;

    DisplayLabelText(x, y, TXT_BRIGHT_ADJUST); 
    GUIRES_GetImgWidthHeight(&icon_width,&icon_height,IMAGE_MTV_BRIGHTNESS_PROGRESS_BG,MMIMTV_MAIN_WIN_ID);
    GUIRES_GetImgWidthHeight(&progress_width,&progress_height,IMAGE_MTV_PROGRESS_1,MMIMTV_MAIN_WIN_ID);
    offset_y = (icon_height - progress_height) / 2;

    MMIMTV_OSD_DISPLAY_PTR->progress_rect.left = (MMIMTV_LCD_WIDTH - MMIMTV_SOFTKEY_WIDTH - icon_width) / 2;
    MMIMTV_OSD_DISPLAY_PTR->progress_rect.top = MMIMTV_LCD_HEIGHT * 5 / 6;
    MMIMTV_OSD_DISPLAY_PTR->progress_rect.right = MMIMTV_OSD_DISPLAY_PTR->progress_rect.left + icon_width;
    MMIMTV_OSD_DISPLAY_PTR->progress_rect.bottom = MMIMTV_OSD_DISPLAY_PTR->progress_rect.top + icon_height;
    
    DisplayPicture(MMIMTV_OSD_DISPLAY_PTR->progress_rect.left, MMIMTV_OSD_DISPLAY_PTR->progress_rect.top, IMAGE_MTV_BRIGHTNESS_PROGRESS_BG);
    if(MMIMTV_GetBrightness() > MMIMTV_BRIGHTNESS_LEVEL_0)
    {
        DisplayPicture(MMIMTV_OSD_DISPLAY_PTR->progress_rect.left + MMIMTV_BRI_OFFSET_X, MMIMTV_OSD_DISPLAY_PTR->progress_rect.top + offset_y, IMAGE_MTV_PROGRESS_1 + MMIMTV_GetBrightness() - 1);
    }
}

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void DisplayProNameIcon(
                              uint32 x,
                              uint32 y
                              )
{
    GUI_RECT_T  *rect_ptr = &(MMIMTV_OSD_DISPLAY_PTR->progress_rect);

    ClearRect(MMIMTV_OSD_DISPLAY_PTR->progress_rect);
    SCI_MEMSET(rect_ptr, 0, sizeof(GUI_RECT_T));
    if(!MMIMTV_IsDisplayProgramName())
    {
        DisplayLabelText(x, y, TXT_MTV_DISPLAY_PROGRAM_NAME);
    }
    else
    {
        DisplayLabelText(x, y, TXT_MTV_HIDE_PROGRAM_NAME);
    }
}

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void DisplayMuteIcon(
                           uint32 x,
                           uint32 y
                           )
{
    GUI_RECT_T  *rect_ptr = &(MMIMTV_OSD_DISPLAY_PTR->progress_rect);
        
    ClearRect(MMIMTV_OSD_DISPLAY_PTR->progress_rect);
    SCI_MEMSET(rect_ptr, 0, sizeof(GUI_RECT_T));
    if(!MMIMTV_IsMute())
    {
        DisplayLabelText(x, y, TXT_TURNON_MUTE);
        
    }
    else
    {
        DisplayLabelText(x, y, TXT_TURNOFF_MUTE);
        
    }
}

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void DisplayRecordIcon(
                             uint32 x,
                             uint32 y
                             )
{
    GUI_RECT_T  *rect_ptr = &(MMIMTV_OSD_DISPLAY_PTR->progress_rect);
    
    ClearRect(MMIMTV_OSD_DISPLAY_PTR->progress_rect);
    SCI_MEMSET(rect_ptr, 0, sizeof(GUI_RECT_T));
    
#ifndef MBBMS_SUPPORT
    if(MMIMTV_RECORD_NONE == MMIMTV_GetPlayInfo()->record_status)
    {
        DisplayLabelText(x, y, TXT_MTV_RECORD_PROGRAM);
    }
    else
    {
        //is recording
        DisplayLabelText(x, y, TXT_MTV_STOP_RECORD);  
    }
#else
    //MBBMS可用，此项映射为 暂停播放、恢复播放
    if(MMIMTV_PLAY_PLAYING == MMIMBBMS_GetPlayInfo()->cmmb_play_status
        || MMIMTV_PLAY_WAITING == MMIMBBMS_GetPlayInfo()->cmmb_play_status)
    {
        DisplayLabelText(x, y, TXT_MTV_STOP_PLAY); 
    }
    else
    {
        DisplayLabelText(x, y, TXT_MTV_RESUME_PLAY);
    }
#endif
}

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void DisplaySnapshotIcon(
                               uint32 x, 
                               uint32 y
                               )
{
    GUI_RECT_T  *rect_ptr = &(MMIMTV_OSD_DISPLAY_PTR->progress_rect);

    ClearRect(MMIMTV_OSD_DISPLAY_PTR->progress_rect);
    SCI_MEMSET(rect_ptr, 0, sizeof(GUI_RECT_T));
    DisplayLabelText(x, y, TXT_MTV_SNAP_SHOT);
}

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void DisplayESGIcon(
                          uint32 x, 
                          uint32 y
                          )
{
    GUI_RECT_T  *rect_ptr = &(MMIMTV_OSD_DISPLAY_PTR->progress_rect);

    ClearRect(MMIMTV_OSD_DISPLAY_PTR->progress_rect);
    SCI_MEMSET(rect_ptr, 0, sizeof(GUI_RECT_T));

    DisplayLabelText(x, y, TXT_MTV_ESG);
}

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void OKProNameIcon(void)
{
    MMIMTV_SetIsDisplayProgramName((BOOLEAN)!MMIMTV_IsDisplayProgramName());
    if(!MMIMTV_IsDisplayProgramName())
    {
        MMIMTV_GetIconHandle()->IconSetImage(MMIMTV_ICON_PROGRAM_NAME, IMAGE_MTV_HIDE_PROGRAM_NAME, IMAGE_MTV_HIDE_PROGRAM_NAME_FOCUS);
        ChangeLabelText(TXT_MTV_DISPLAY_PROGRAM_NAME);
    }
    else
    {
        MMIMTV_GetIconHandle()->IconSetImage(MMIMTV_ICON_PROGRAM_NAME, IMAGE_MTV_DISPLAY_PROGRAM_NAME, IMAGE_MTV_DISPLAY_PROGRAM_NAME_FOCUS);
        ChangeLabelText(TXT_MTV_HIDE_PROGRAM_NAME);
    }
}
/*lint -e528*/

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void OKMuteIcon(void)
{
    MMIMTV_VOLUME_E volume = MMIMTV_VOLUME_LEVEL_0;
    AUDIO_DEVICE_MODE_TYPE_E audio_mode = 0;
    MMISRV_HANDLE_T audio_handle = MMIMTV_GetAudioHandle();

    MMIMTV_SetIsMute((BOOLEAN)!MMIMTV_IsMute());
    if(!MMIMTV_IsMute())
    {
        MMIMTV_GetIconHandle()->IconSetImage(MMIMTV_ICON_MUTE, IMAGE_MTV_CANCEL_MUTE, IMAGE_MTV_CANCEL_MUTE_FOCUS);
        
        //SCI_TRACE_LOW:"[MMIMTV]: OKMuteIcon UnMute"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_1486_112_2_18_2_40_42_861,(uint8*)"");
        volume = MMIMTV_GetVolume();
        if(volume < MMIMTV_VOLUME_LEVEL_1)
        {
            volume = MMIMTV_VOLUME_LEVEL_1;
            MMIMTV_SetVolume(volume);
        }
        MMIMTV_SetPanelVolume(volume);
        
        CMMB_UnMute();
        //MMIAUDIO_SetMute(FALSE);
        
        MMIAPISET_GetDeviceMode(&audio_mode);
        //MMIAUDIO_SetVolume(audio_mode, volume);
        
        ChangeLabelText(TXT_TURNON_MUTE);
    }
    else
    {
        MMIMTV_GetIconHandle()->IconSetImage(MMIMTV_ICON_MUTE, IMAGE_MTV_MUTE, IMAGE_MTV_MUTE_FOCUS);
        
        //SCI_TRACE_LOW:"[MMIMTV]: OKMuteIcon Mute"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_1507_112_2_18_2_40_42_862,(uint8*)"");
        MMIMTV_SetPanelVolume(MMIMTV_VOLUME_LEVEL_0);
        
        CMMB_Mute();
        //MMIAUDIO_SetMute(TRUE);
        volume = MMIMTV_GetPanelVolume();
        
        MMIAPISET_GetDeviceMode(&audio_mode);
        //MMIAUDIO_SetVolume(audio_mode, MMIMTV_GetPanelVolume());
        
        ChangeLabelText(TXT_TURNOFF_MUTE);
    }
    
    if(audio_handle > 0)
    {
        MMISRVAUD_SetVolume(audio_handle, volume);
    }
}

/*****************************************************************************/
// 	Description : 提示录像已保存
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DisplayRecordSaveTip(void)
{
    GUI_RECT_T   rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_TOP + MMIMTV_NORMAL_FONT_HEIGHT};
    MMI_STRING_T text_str = {0};
    wchar*       record_name_ptr = PNULL;
    uint16       record_name_len = 0;
    uint16       str_width = 0;
    //wchar        temp[MMIMTV_FULL_PATH_MAX_LEN] = {0};
    
    //先显示"已存为"
    MMI_GetLabelTextByLang(TXT_MTV_SAVE_OK, &text_str);
    str_width = GUI_GetStringWidth(MMIMTV_NORMAL_FONT, text_str.wstr_ptr, text_str.wstr_len);
    DisplayEffectString(rect, text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT, FALSE);
    
    //显示文件名
    record_name_ptr = MMIMTV_GetRecordFileName();
    record_name_len = MMIAPICOM_Wstrlen(record_name_ptr);
    rect.left = MTV_OSD_LEFT + str_width + GUIFONT_GetHeight(MMIMTV_NORMAL_FONT)/2;
    DisplayEffectString(rect, record_name_ptr, record_name_len, MMIMTV_STRING_FONT, FALSE);
    
    //启动已保存提示的定时器
    CreateDisappearTimer(MMIMTV_SYSTIME_FILE_SAVED_TIP);
    MMIMTV_OSD_DISPLAY_PTR->file_saved_tip_count = 0;
}

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void OKRecordIcon(void)
{
    BOOLEAN is_operation_success = TRUE;
	
    //SCI_TRACE_LOW:"[MMIMTV]: OKRecordIcon s_focus_osd = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_1563_112_2_18_2_40_42_863,(uint8*)"d", MMIMTV_GetFocusOSDScene());
    if(OSD_SHORTCUT_ICON != MMIMTV_GetFocusOSDScene())
    {
        return;
    }
    
#ifndef MBBMS_SUPPORT
    if(MMIMTV_RECORD_NONE == MMIMTV_GetPlayInfo()->record_status)
    {
        //start record
        if(MMIMTV_StartRecord())
        {
            MMIMTV_GetIconHandle()->IconSetImage(MMIMTV_ICON_RECORD, IMAGE_MTV_CANCEL_RECORD, IMAGE_MTV_CANCEL_RECORD_FOCUS);
            ChangeLabelText(TXT_MTV_STOP_RECORD);
        }
    }
    else if(MMIMTV_RECORD_RECORDING == MMIMTV_GetPlayInfo()->record_status)
    {
        MMIMTV_StopRecord();
        MMIMTV_GetIconHandle()->IconSetImage(MMIMTV_ICON_RECORD, IMAGE_MTV_RECORD, IMAGE_MTV_RECORD_FOCUS);
        ChangeLabelText(TXT_MTV_RECORD_PROGRAM);
    }
#else
#ifdef ENG_SUPPORT
    if(!MMIAPIENG_IsAudioServiceEnable())
    {
        if(MMIMBBMS_IsCurrentServiceAudioTyle())
        {
            MMIMTV_SetPreOSDScene(OSD_IDLE);
            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(OSD_PUB);
            MMIMTV_SetOSDRestoreParam((uint32)TXT_MBBMS_UNSUPPORT_AUDIO_SERVICE);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MBBMS_UNSUPPORT_AUDIO_SERVICE);

            return;
        }
    }
#endif

    //SCI_TRACE_LOW:"[MMIMTV]: OKRecordIcon mbbms flow_type=%d, flow_status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_1602_112_2_18_2_40_42_864,(uint8*)"dd", MMIMBBMS_GetFlowInfo()->flow_type, MMIMBBMS_GetFlowInfo()->status);
    
    //MBBMS可用，此项映射为 暂停播放、恢复播放
    if(MMIMBBMS_FLOW_PLAY == MMIMBBMS_GetFlowInfo()->flow_type
        && MMIMBBMS_FLOW_STATE_STARTING == MMIMBBMS_GetFlowInfo()->status)
    {
        //暂停播放
        is_operation_success = MMIMBBMS_StopPlay();
        if(is_operation_success)
        {
            MMIMTV_GetIconHandle()->IconSetImage(MMIMTV_ICON_RECORD, IMAGE_MTV_PLAY, IMAGE_MTV_PLAY_FOCUS);
            ChangeLabelText(TXT_MTV_RESUME_PLAY);		
        }	
    }
    else
    {
        //恢复播放
        is_operation_success = MMIMBBMS_StartPlay();
        if(is_operation_success)
        {
            MMIMTV_GetIconHandle()->IconSetImage(MMIMTV_ICON_RECORD, IMAGE_MTV_PAUSE, IMAGE_MTV_PAUSE_FOCUS);
            ChangeLabelText(TXT_MTV_STOP_PLAY);
        }	        
    }
#endif
}

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void OKSnapshotIcon(void)
{
    if(MMIMTV_SNAPSHOT_ERROR_NONE == MMIMTV_Snapshot())
    {
        //关闭OSD_SHORTCUT_ICON场景，避免显示冲突
        MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
        MMIMTV_SetFocusOSDScene(OSD_IDLE);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        MMIMTV_DisplaySnapshotTip();
    }
}

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void OKESGIcon(void)
{
    if(MMK_IsOpenWin(MMIMBBMS_PROGRAM_LIST_MAIN_WIN_ID))
    {
        MMK_CloseWin(MMIMBBMS_PROGRAM_LIST_MAIN_WIN_ID);
    }
    
    MMIMBBMS_OpenProgramListWin();
    if(OSD_SHORTCUT_ICON == MMIMTV_GetFocusOSDScene())
    {
        MMIMTV_SetFocusOSDScene(OSD_IDLE);
    }
}

/*****************************************************************************/
//  Description : stop text scroll timer
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void CreateDisappearTimer(uint32 type)
{   
    if(0 == MMIMTV_OSD_DISPLAY_PTR->sys_timer)
    {        
        MMIMTV_OSD_DISPLAY_PTR->sys_timer = MMK_CreateWinTimer(MMIMTV_MAIN_WIN_ID, MMIMTV_SYSTEM_TIMER_UNIT, TRUE);
    }
    MMIMTV_OSD_DISPLAY_PTR->systime_type = MMIMTV_OSD_DISPLAY_PTR->systime_type | type;
}

/*****************************************************************************/
//  Description : stop text scroll timer
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMTV_StopSysTimer(uint32 type)
{
    if(PNULL != s_osd_display_data_ptr)
    {
        type = ~type;
        MMIMTV_OSD_DISPLAY_PTR->systime_type = MMIMTV_OSD_DISPLAY_PTR->systime_type & type;
        if (0 < MMIMTV_OSD_DISPLAY_PTR->sys_timer && 0 == MMIMTV_OSD_DISPLAY_PTR->systime_type)
        {
            MMK_StopTimer(MMIMTV_OSD_DISPLAY_PTR->sys_timer);
            MMIMTV_OSD_DISPLAY_PTR->sys_timer = 0;
        }
    }
}

/*****************************************************************************/
//  Description : stop text scroll timer
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMTV_HandleSystimer(void)
{
    uint8 time_out = 0;

    if(MMIMTV_SYSTIME_WAITING_PUBWIN == (MMIMTV_OSD_DISPLAY_PTR->systime_type & MMIMTV_SYSTIME_WAITING_PUBWIN))
    {
        OpenPubWin(MTV_PUBWIN_WAIT, MMIMTV_OSD_DISPLAY_PTR->wait_pubwin_text_id);
    }
    
    if(MMIMTV_MBBMS_PLAY_CONNECTING == (MMIMTV_OSD_DISPLAY_PTR->systime_type & MMIMTV_MBBMS_PLAY_CONNECTING))
    {
        //update loop progress bar
        MMIMTV_OSD_DISPLAY_PTR->play_connecting_count ++;
        MMIMTV_UpdatePlayConnectingScene();
    }
    
    
    if(MMIMTV_SYSTIME_SHORTCUT_PANEL == (MMIMTV_OSD_DISPLAY_PTR->systime_type & MMIMTV_SYSTIME_SHORTCUT_PANEL))
    {
        time_out = 0;
        
        switch(MMIMTV_GetOSDDisplayTime())
        {
        case MMIMTV_DISPLAY_TIME_3S:
            time_out = MMIMTV_3S_TIMEOUT;
            break;
            
        case MMIMTV_DISPLAY_TIME_5S:
            time_out = MMIMTV_5S_TIMEOUT;
            break;
            
        case MMIMTV_DISPLAY_TIME_10S:
            time_out = MMIMTV_10S_TIMEOUT;
            break;
            
        case MMIMTV_DISPLAY_TIME_15S:
            time_out = MMIMTV_15S_TIMEOUT;
            break;
        default:
            break;
        }
        time_out = time_out * 2;
        MMIMTV_OSD_DISPLAY_PTR->systime_count ++;
        if(MMIMTV_OSD_DISPLAY_PTR->systime_count == time_out)
        {            
            MMIMTV_CloseShortcutIconScene();
            MMIMTV_SetFocusOSDScene(OSD_IDLE);
            MMIMTV_ClearFileSavedTip();
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            MMIMTV_OSD_DISPLAY_PTR->systime_count = 0;
        }
    }
    if(MMIMTV_SYSTIME_TIP_PUBWIN == (MMIMTV_OSD_DISPLAY_PTR->systime_type & MMIMTV_SYSTIME_TIP_PUBWIN))
    {
        MMIMTV_OSD_DISPLAY_PTR->servicetime_count ++;
        if((MMIMTV_5S_TIMEOUT * 2) == MMIMTV_OSD_DISPLAY_PTR->servicetime_count)
        {
            //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_HandleSystimer MMIMTV_SYSTIME_TIP_PUBWIN"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_1755_112_2_18_2_40_42_865,(uint8*)"");
            //3s时间到，恢复之前打断的场景
            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(MMIMTV_GetPreOSDScene());
            MMIMTV_SetPreOSDScene(OSD_IDLE);
            MMIMTV_SetOSDRestoreParam(0);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            MMIMTV_OSD_DISPLAY_PTR->servicetime_count = 0;
        }
    }

    if(MMIMTV_SYSTIME_SERVICE_NUMBER == (MMIMTV_OSD_DISPLAY_PTR->systime_type & MMIMTV_SYSTIME_SERVICE_NUMBER))
    {
        MMIMTV_OSD_DISPLAY_PTR->servicetime_count ++;
        if((MMIMTV_3S_TIMEOUT * 2) == MMIMTV_OSD_DISPLAY_PTR->servicetime_count)
        {
            MMIMTV_ClearServiceNumber();
            MMIMTV_OSD_DISPLAY_PTR->servicetime_count = 0;
        }
    }
    
    if(MMIMTV_SYSTIME_NUM_SWITCH == (MMIMTV_OSD_DISPLAY_PTR->systime_type & MMIMTV_SYSTIME_NUM_SWITCH))
    {
        MMIMTV_OSD_DISPLAY_PTR->num_switch_count ++;
        if((MMIMTV_3S_TIMEOUT * 2) == MMIMTV_OSD_DISPLAY_PTR->num_switch_count)
        {
            MMIMTV_StopNumSwitchTimer();
            MMIMTV_OSD_DISPLAY_PTR->servicetime_count = 0;
            //num switch play
            MMIMTV_NumSwitchPlay();
        }
    }
    
    if(MMIMTV_SYSTIME_FILE_SAVED_TIP == (MMIMTV_OSD_DISPLAY_PTR->systime_type & MMIMTV_SYSTIME_FILE_SAVED_TIP))
    {
        //提示 录制文件或捕捉图像 文件已保存，3S消失
        MMIMTV_OSD_DISPLAY_PTR->file_saved_tip_count ++;
        if((MMIMTV_3S_TIMEOUT * 2) == MMIMTV_OSD_DISPLAY_PTR->file_saved_tip_count)
        {
            MMIMTV_ClearFileSavedTip();
            MMIMTV_OSD_DISPLAY_PTR->file_saved_tip_count = 0;

            //恢复OSD_SHORTCUT_ICON场景
            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(OSD_SHORTCUT_ICON);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_ICON_SNAPSHOT);
        }
    }
    if(MMIMTV_SYSTIME_VOLUME_PROGRESS_BAR == (MMIMTV_OSD_DISPLAY_PTR->systime_type & MMIMTV_SYSTIME_VOLUME_PROGRESS_BAR))
    {
        MMIMTV_OSD_DISPLAY_PTR->volumetime_count ++;
        if((MMIMTV_3S_TIMEOUT * 2) == MMIMTV_OSD_DISPLAY_PTR->volumetime_count)
        {
            MMIMTV_ClearVolumeProgress();
            MMIMTV_OSD_DISPLAY_PTR->volumetime_count = 0;
        }
    }
    if(MMIMTV_SYSTIME_WATCH_TIME_DISAPPEAR_TIP == (MMIMTV_OSD_DISPLAY_PTR->systime_type & MMIMTV_SYSTIME_WATCH_TIME_DISAPPEAR_TIP))
    {
        MMIMTV_OSD_DISPLAY_PTR->watch_time_tip_count ++;
        if((MMIMTV_3S_TIMEOUT * 2) == MMIMTV_OSD_DISPLAY_PTR->watch_time_tip_count)
        {
            MMIMTV_ClearWatchTip();
            MMIMTV_OSD_DISPLAY_PTR->watch_time_tip_count = 0;
        }
        else if(1 == MMIMTV_OSD_DISPLAY_PTR->watch_time_tip_count)
        {
            if(MMIMTV_SYSTIME_TEXT_SCROLL == (MMIMTV_OSD_DISPLAY_PTR->systime_type & MMIMTV_SYSTIME_TEXT_SCROLL))
            {
                MMIMTV_DisplayProgramHint(FALSE);
                MMIMTV_OSD_DISPLAY_PTR->watch_time_tip_count = 0;
            }
            else
            {
                //录像回放中不提示
                if(OSD_RECORD_PLAY != MMIMTV_GetFocusOSDScene())
                {
                    MMI_STRING_T text_str = {0};
                    uint16 index = 0;
                    wchar temp[MMIMTV_TEMP_STRING_BUFFER_MAX_LEN] = {0};
                    uint16 text_width = 0;
                    char c_long_time[MMIMTV_LONG_TIME_MAX_DIGITS] = {0};
                    wchar w_long_time[MMIMTV_LONG_TIME_MAX_DIGITS] = {0};
                    GUI_RECT_T rect = {MMIMTV_WATCH_TIME_TIP_LEFT, MMIMTV_LCD_HEIGHT - MMIMTV_WATCH_TIME_TIP_HEIGHT,
                        MMIMTV_LCD_WIDTH - MMIMTV_SOFTKEY_WIDTH, MMIMTV_LCD_HEIGHT - 1};
                    
                    MMI_GetLabelTextByLang(TXT_MTV_WATCH_TIP_CONTENT, &text_str);
                    MMIAPICOM_Wstrcpy(temp, text_str.wstr_ptr);
                    index = sprintf(c_long_time, "%d", (MMIAPICOM_GetCurTime() - MMIMTV_OSD_DISPLAY_PTR->start_watch_time) / 60);
                    MMIAPICOM_StrToWstr((uint8*)c_long_time, w_long_time);
                    MMIAPICOM_Wstrncpy(temp + text_str.wstr_len, w_long_time, index);
                    index += text_str.wstr_len;
                    
                    MMI_GetLabelTextByLang(TXT_COMM_MINUTES, &text_str);
                    MMIAPICOM_Wstrncpy(temp + index, text_str.wstr_ptr, text_str.wstr_len + 1);
                    index += text_str.wstr_len;
                    
                    text_width = GUI_CalculateStringPiexlNum(temp, index, MMIMTV_NORMAL_FONT, 1);
                    rect.left += (rect.right - rect.left - text_width) / 2;
                    DisplayEffectString(rect, temp, index,MMIMTV_NORMAL_FONT, FALSE);
                }
            }
        }
    }
    else if(MMIMTV_SYSTIME_TEXT_SCROLL == (MMIMTV_OSD_DISPLAY_PTR->systime_type & MMIMTV_SYSTIME_TEXT_SCROLL))
    {
        MMIMTV_DisplayProgramHint(FALSE);
    }
    if(MMIMTV_SYSTIME_WATCH_TIME_TIP == (MMIMTV_OSD_DISPLAY_PTR->systime_type & MMIMTV_SYSTIME_WATCH_TIME_TIP))
    {
        uint32 long_time = MMIAPICOM_GetCurTime() - MMIMTV_OSD_DISPLAY_PTR->start_watch_time;
        uint32 watch_time = 0;
        uint32 prompt_times = 0;
        
        switch(MMIMTV_GetWatchTimeTip())
        {
        case MMIMTV_WATCH_TIME_TIP_10M:
            watch_time = 10 * MMIMTV_SECONDS_PER_MINUTE;
            break;
            
        case MMIMTV_WATCH_TIME_TIP_30M:
            watch_time = 30 * MMIMTV_SECONDS_PER_MINUTE;
            break;
            
        case MMIMTV_WATCH_TIME_TIP_60M:
            watch_time = 60 * MMIMTV_SECONDS_PER_MINUTE;
            break;

        default:
            break;
        }

        if( 0 != watch_time )
        {
            prompt_times = long_time / watch_time;
        }

        //增加prompt_times字段, 防止由于阻塞导致不显示提示
        if( prompt_times != MMIMTV_OSD_DISPLAY_PTR->prompt_times )
        {
            MMIMTV_OSD_DISPLAY_PTR->prompt_times = prompt_times;

            MMIMTV_OSD_DISPLAY_PTR->watch_time_tip_count = 0;
            CreateDisappearTimer(MMIMTV_SYSTIME_WATCH_TIME_DISAPPEAR_TIP);
        }
    }
}

/*****************************************************************************/
// 	Description : 启动数字键切换频道定时器
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_StartNumSwitchTimer(void)
{
    CreateDisappearTimer(MMIMTV_SYSTIME_NUM_SWITCH);
    MMIMTV_OSD_DISPLAY_PTR->num_switch_count = 0;
}

/*****************************************************************************/
// 	Description : 停止数字键切换频道定时器
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_StopNumSwitchTimer(void)
{
    MMIMTV_StopSysTimer(MMIMTV_SYSTIME_NUM_SWITCH);
}

/*****************************************************************************/
// 	Description : 启动频道切换栏定时器
//	Global resource dependence : none
//  Author: xin.li
//	Note: 某Tab项停留超500ms，播放该频道
/*****************************************************************************/
LOCAL void StartSwitchPlayTabTimer(void)
{
    if(0 == MMIMTV_OSD_DISPLAY_PTR->switch_play_timer_id)
    {        
        MMIMTV_OSD_DISPLAY_PTR->switch_play_timer_id = MMK_CreateWinTimer(MMIMTV_MAIN_WIN_ID, MMIMTV_SYSTEM_TIMER_UNIT, TRUE);
        MMIMTV_OSD_DISPLAY_PTR->switch_play_time_count = 0;
    }
}

/*****************************************************************************/
// 	Description : 停止频道切换栏定时器
//	Global resource dependence : none
//  Author: xin.li
//	Note: 某Tab项停留超500ms，播放该频道
/*****************************************************************************/
LOCAL void StopSwitchPlayTabTimer(void)
{
    if(PNULL != s_osd_display_data_ptr)
    {
        if (MMIMTV_OSD_DISPLAY_PTR->switch_play_timer_id > 0)
        {
            MMK_StopTimer(MMIMTV_OSD_DISPLAY_PTR->switch_play_timer_id);
            MMIMTV_OSD_DISPLAY_PTR->switch_play_timer_id = 0;
            MMIMTV_OSD_DISPLAY_PTR->switch_play_time_count = 0;
        }
    }
}

/*****************************************************************************/
//  Description : get switch play timer id
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIMTV_GetSwitchPlayTimerId(void)
{
    return MMIMTV_OSD_DISPLAY_PTR->switch_play_timer_id;
}

/*****************************************************************************/
//  Description : get switch play timer id
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMTV_HandleSwitchPlayTimer(void)
{
    MMIMTV_OSD_DISPLAY_PTR->switch_play_time_count ++;
    if(5 == MMIMTV_OSD_DISPLAY_PTR->switch_play_time_count)
    {
        if(MMIMTV_IsOpenSwitchServiceTab())
        {
            MMIMTV_TimeOutPlay();
        }
        MMIMTV_OSD_DISPLAY_PTR->switch_play_time_count = 0;
    }
}

/*****************************************************************************/
//  Description : reset switch play timer count
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ResetSwitchPlayTimer(void)
{
    MMIMTV_OSD_DISPLAY_PTR->switch_play_time_count = 0;
}

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ClearWatchTip(void)
{
    GUI_RECT_T rect = {MMIMTV_WATCH_TIME_TIP_LEFT, MMIMTV_LCD_HEIGHT - MMIMTV_WATCH_TIME_TIP_HEIGHT,
                    MMIMTV_LCD_WIDTH - MMIMTV_SOFTKEY_WIDTH, MMIMTV_LCD_HEIGHT - 1};

    ClearRect(rect);
    MMIMTV_StopSysTimer(MMIMTV_SYSTIME_WATCH_TIME_DISAPPEAR_TIP);
}

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenShortcutIconScene(uint32 param)
{
    uint16 screen_width = MMIMTV_LCD_WIDTH;
    uint16 screen_height = MMIMTV_LCD_HEIGHT;
    uint16 softkey_width = MMIMTV_SOFTKEY_WIDTH;
    //uint16 softkey_height = MMIMTV_SOFTKEY_HEIGHT;
    uint16 icon_width = 0;
    uint16 icon_height = 0;
    uint32 x = 0;
    uint32 y = 0;
    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list = MMIAPIDC_InitIcon();
    if(PNULL == MMIMTV_OSD_DISPLAY_PTR->osd_icon_list)
    {
        return;
    }
    
	//静音
    if(!MMIMTV_IsMute())
    {
        MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconAppendImage(IMAGE_MTV_CANCEL_MUTE, IMAGE_MTV_CANCEL_MUTE_FOCUS);
    }
    else
    {
        MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconAppendImage(IMAGE_MTV_MUTE, IMAGE_MTV_MUTE_FOCUS);
    }
#ifndef MBBMS_SUPPORT
	//录制
    if(MMIMTV_RECORD_NONE == MMIMTV_GetPlayInfo()->record_status)// is recording
    {
        MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconAppendImage(IMAGE_MTV_RECORD, IMAGE_MTV_RECORD_FOCUS);
    }
    else
    {
        MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconAppendImage(IMAGE_MTV_CANCEL_RECORD, IMAGE_MTV_CANCEL_RECORD_FOCUS);
    }
#else
        //MBBMS可用，此项映射为 暂停播放、恢复播放
        if(MMIMTV_PLAY_PLAYING == MMIMBBMS_GetPlayInfo()->cmmb_play_status
            || MMIMTV_PLAY_WAITING == MMIMBBMS_GetPlayInfo()->cmmb_play_status)
        {
            //is playing
            MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconAppendImage(IMAGE_MTV_PAUSE, IMAGE_MTV_PAUSE_FOCUS);
        }
        else
        {
            MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconAppendImage(IMAGE_MTV_PLAY, IMAGE_MTV_PLAY_FOCUS);
        }
#endif
	//捕捉图像
    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconAppendImage(IMAGE_MTV_SNAPSHOT, IMAGE_MTV_SNAPSHOT_FOCUS);
	//节目指南
    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconAppendImage(IMAGE_MTV_SHARP, IMAGE_MTV_SHARP_FOCUS);
	//亮度调节
    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconAppendImage(IMAGE_MTV_BRIGHTNESS, IMAGE_MTV_BRIGHTNESS_FOCUS);
#ifndef MBBMS_SUPPORT
	//节目名称
    if(!MMIMTV_IsDisplayProgramName())
    {
        MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconAppendImage(IMAGE_MTV_HIDE_PROGRAM_NAME, IMAGE_MTV_HIDE_PROGRAM_NAME_FOCUS);
    }
    else
    {
        MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconAppendImage(IMAGE_MTV_DISPLAY_PROGRAM_NAME, IMAGE_MTV_DISPLAY_PROGRAM_NAME_FOCUS);
    }
#endif

#ifdef MMIMTV_SOFTKEY_ON_BOTTOM    
    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconSetDisplayMode(DISPLAY_DIRECT_HORIZONTAL);
#else
    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconSetDisplayMode(DISPLAY_DIRECT_VERTICAL);
#endif

    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconSetCallback(MMIMTV_ICON_MUTE, DisplayMuteIcon);
//#ifndef MBBMS_SUPPORT
    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconSetCallback(MMIMTV_ICON_RECORD, DisplayRecordIcon);
//#endif
    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconSetCallback(MMIMTV_ICON_SNAPSHOT, DisplaySnapshotIcon);
    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconSetCallback(MMIMTV_ICON_ESG, DisplayESGIcon);
    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconSetCallback(MMIMTV_ICON_BRIGHTNESS, DisplayBrightnessProgress);
#ifndef MBBMS_SUPPORT
	MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconSetCallback(MMIMTV_ICON_PROGRAM_NAME, DisplayProNameIcon);
#endif
	
    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconSetOKCallback(MMIMTV_ICON_MUTE, OKMuteIcon);
//#ifndef MBBMS_SUPPORT
    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconSetOKCallback(MMIMTV_ICON_RECORD, OKRecordIcon);
//#endif
    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconSetOKCallback(MMIMTV_ICON_SNAPSHOT, OKSnapshotIcon);
    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconSetOKCallback(MMIMTV_ICON_ESG, OKESGIcon);
#ifndef MBBMS_SUPPORT
	MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconSetOKCallback(MMIMTV_ICON_PROGRAM_NAME, OKProNameIcon);
#endif
	
    //当SOFTKEY在下时需要区别处理
    //GUIRES_GetImgWidthHeight(&icon_width,&icon_height,IMAGE_MTV_CANCEL_MUTE,MMIMTV_MAIN_WIN_ID);
    icon_width = MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconGetIconWidth();
    icon_height = MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconGetIconWidth();
#ifdef MMIMTV_SOFTKEY_ON_BOTTOM    
    x = (screen_width - (MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconGetDisplayNumber()) * icon_width)/2;
    y = screen_height - icon_height - softkey_height - 1;
#else
    x = screen_width - MAX(softkey_width,icon_width);
    y = (screen_height - MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconGetDisplayNumber() * icon_height)/2;
#endif
    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconSetStartPositionX(x);
    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconSetStartPositionY(y);

    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconFocus(param);
    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconDisplay();

    //MBBMS 移动要求左软键为选项
    DisplaySoftKey(STXT_OPTION, TXT_NULL, STXT_CANCEL);
    MMIMTV_DisplayServiceNumber();
    MMIMTV_EnableStb();
    CreateDisappearTimer(MMIMTV_SYSTIME_SHORTCUT_PANEL);
    MMIMTV_OSD_DISPLAY_PTR->systime_count = 0;

}

/*****************************************************************************/
// 	Description : get icon handle
//	Global resource dependence : none
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC DC_ICONS_OP* MMIMTV_GetIconHandle(void)
{
    if(PNULL == MMIMTV_OSD_DISPLAY_PTR->osd_icon_list)
    {
        return PNULL;
    }
    
    return MMIMTV_OSD_DISPLAY_PTR->osd_icon_list;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseShortcutIconScene(void)
{
    GUI_RECT_T  *rect_ptr = &(MMIMTV_OSD_DISPLAY_PTR->progress_rect);

    MMIMTV_GetIconHandle()->IconClose();
    ClearLabelText(MTV_ICON_LABEL1_CTRL_ID);
    MMIMTV_DisableStb();
    MMIMTV_ClearScreen();
    SCI_MEMSET(rect_ptr, 0, sizeof(GUI_RECT_T));
    MMIMTV_StopSysTimer(MMIMTV_SYSTIME_SHORTCUT_PANEL);
    MMIMTV_StopSysTimer(MMIMTV_SYSTIME_SERVICE_NUMBER);
    MMIMTV_StopSysTimer(MMIMTV_SYSTIME_FILE_SAVED_TIP);
}

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenSettingPictureScene(uint32 param)
{
//    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list = MMIAPIDC_InitIcon();
//    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconAppendImage(IMAGE_MTV_BRIGHTNESS, IMAGE_MTV_BRIGHTNESS_FOCUS);
//    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconAppendImage(IMAGE_MTV_CONTRAST, IMAGE_MTV_CONTRAST_FOCUS);
//    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconAppendImage(IMAGE_MTV_COLOR_SATURATION, IMAGE_MTV_COLOR_SATURATION_FOCUS);
//    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconAppendImage(IMAGE_MTV_SHARP, IMAGE_MTV_SHARP_FOCUS);
//    
//    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconSetDisplayMode(0);
//    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconSetCallback(0, DisplayBrightnessProgress);
//    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconSetCallback(1, DisplayContrastProgress);
//    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconSetCallback(2, DisplayColorSaturationProgress);
//    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconSetCallback(3, DisplaySharpnessProgress);
//    
//    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconSetStartPositionX(MMIMTV_LCD_WIDTH - 37);
//    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconSetStartPositionY(55);
//    
//    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconFocus(0);
//    MMIMTV_OSD_DISPLAY_PTR->osd_icon_list->IconDisplay();
//    DisplaySoftKey(STXT_OK, TXT_NULL, STXT_RETURN);
}

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseSettingPictureScene(void)
{
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ClearScreen(void)
{
    GUI_LCD_DEV_INFO dev_info = {0};
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    
    if(MMK_IsFocusWin(MMIMTV_MAIN_WIN_ID))
    {
        GUIBLOCK_ClearByColor(MMIMTV_TRANSPARENT_COLOR, &dev_info);
        MMITHEME_StoreUpdateRect(&dev_info, rect);
        GUIWIN_UpdateStb();
    }
}

/*****************************************************************************/
// 	Description : open MMIMTV_QUIT_QUERY scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void OpenPubWin(
                      MTV_PUBWIN_TYPE_E type,
                      uint32 string_id
                      )
{
    MMI_STRING_T  text_str = {0};
    uint16        text_line_num = 0;
    uint16        text_pixel_width = 0;
    wchar         text_wait[MMIMTV_TEMP_STRING_BUFFER_MAX_LEN + 1] = {0};
    GUI_RECT_T    text_rect = MTV_MSGBOX_TEXT_RECT;
    GUI_FONT_T    text_font = MMIMTV_NORMAL_FONT;
    GUI_RECT_T    img_rect = {MTV_MSGBOX_LEFT, MTV_MSGBOX_TOP, MTV_MSGBOX_RIGHT, MTV_MSGBOX_BOTTOM};
    uint16        line_space = 0;
    uint16        char_space = 1;
    uint16      bg_img_width = 0;
    uint16      bg_img_height = 0;
    uint16      pub_icon_width = 0;
    uint16      pub_icon_height = 0;	
    
    //get bg image size
    GUIRES_GetImgWidthHeight(&bg_img_width, &bg_img_height, IMAGE_PUBWIN_BG, MMIMTV_MAIN_WIN_ID);
    if(bg_img_width > 0 && bg_img_height > 0)
    {
        int16       bg_real_right = 0;
        int16       bg_real_bottom = 0;
        
        bg_real_right   = img_rect.left + bg_img_width;
        bg_real_bottom  = img_rect.top + bg_img_height;
        
        //recalculate text rect
        text_rect.right = MIN(bg_real_right, text_rect.right) -1;
        text_rect.bottom = MIN(bg_real_bottom, text_rect.bottom) -1;
    }
    
    MMI_GetLabelTextByLang(string_id, &text_str);
    
    text_line_num =  GUI_CalculateStringLinesByPixelNum(text_rect.right - text_rect.left,
                        text_str.wstr_ptr,
                        text_str.wstr_len,
                        text_font,
                        char_space,
                        FALSE);
    if(text_rect.bottom - text_rect.top > text_line_num * (GUI_GetFontHeight(text_font, UNICODE_HANZI)+line_space))
    {
        text_rect.top = text_rect.top + (text_rect.bottom - text_rect.top - text_line_num * (GUI_GetFontHeight(text_font, UNICODE_HANZI)+line_space))/2;
    }
    
    if(1 == text_line_num)
    {
        //单行文本居中显示
        text_pixel_width = GUI_CalculateStringPiexlNum(text_str.wstr_ptr,
            text_str.wstr_len,
            text_font,
            char_space);
        if(text_rect.right - text_rect.left > text_pixel_width)
        {
            text_rect.left = text_rect.left + (text_rect.right - text_rect.left - text_pixel_width)/2;
        }
    }
    
    if(MTV_PUBWIN_WAIT == type)
    {
        uint8 i = 0;
        
        if(text_str.wstr_len +  MMIMTV_PUBWIN_POINTS_NUMBER < MMIMTV_TEMP_STRING_BUFFER_MAX_LEN)
        {
            while(i < MMIMTV_TEMP_STRING_BUFFER_MAX_LEN)
            {
                *(text_wait + i++) = ' ';
            }
            MMIAPICOM_Wstrncpy(text_wait, text_str.wstr_ptr, text_str.wstr_len);
            if(MMIMTV_PUBWIN_POINTS_NUMBER <= MMIMTV_OSD_DISPLAY_PTR->waiting_step)
            {
                MMIMTV_OSD_DISPLAY_PTR->waiting_step = 0;
            }
            else
            {
                MMIMTV_OSD_DISPLAY_PTR->waiting_step +=2;
            }
            i = 0;
            while(i < MMIMTV_OSD_DISPLAY_PTR->waiting_step)
            {
                if(text_str.wstr_len + i+1 < MMIMTV_TEMP_STRING_BUFFER_MAX_LEN)
                {
                    *(text_wait + text_str.wstr_len + i++) = ' ';
                    *(text_wait + text_str.wstr_len + i++) = '.';
                }
            }
            *(text_wait + text_str.wstr_len + MMIMTV_PUBWIN_POINTS_NUMBER) = 0;
            *(text_wait + text_str.wstr_len + MMIMTV_PUBWIN_POINTS_NUMBER + 1) = 0;
            text_str.wstr_ptr = text_wait;
            text_str.wstr_len = MMIAPICOM_Wstrlen(text_wait);
        }
    }

    DisplayPictureInRect(&img_rect, IMAGE_PUBWIN_BG);
    DisplayString(text_rect, text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT);	  
	
    if(MTV_PUBWIN_TIP == type)
    {
    	GUIRES_GetImgWidthHeight(&pub_icon_width, &pub_icon_height, IMAGE_PUBWIN_WARNING, MMIMTV_MAIN_WIN_ID); 
		//居中显示pub win icon
        DisplayPicture(MMIMTV_PUBWIN_ICON_LEFT, MMIMTV_PUBWIN_ICON_TOP +(MTV_MSGBOX_BOTTOM - MTV_MSGBOX_TOP - pub_icon_height)/2, IMAGE_PUBWIN_WARNING);
        DisplaySoftKey(TXT_NULL, TXT_NULL, STXT_RETURN);
    }
    else if (MTV_PUBWIN_TIP_NO_SOFTKEY == type)
    {
    	GUIRES_GetImgWidthHeight(&pub_icon_width, &pub_icon_height, IMAGE_PUBWIN_WARNING, MMIMTV_MAIN_WIN_ID);       
        DisplayPicture(MMIMTV_PUBWIN_ICON_LEFT, MMIMTV_PUBWIN_ICON_TOP +(MTV_MSGBOX_BOTTOM - MTV_MSGBOX_TOP - pub_icon_height)/2, IMAGE_PUBWIN_WARNING);
        DisplaySoftKey(TXT_NULL, TXT_NULL, TXT_NULL);
    }
    else if(MTV_PUBWIN_CONFIRM == type)
    {	
    	GUIRES_GetImgWidthHeight(&pub_icon_width, &pub_icon_height, IMAGE_PUBWIN_WARNING, MMIMTV_MAIN_WIN_ID);  
        DisplayPicture(MMIMTV_PUBWIN_ICON_LEFT, MMIMTV_PUBWIN_ICON_TOP +(MTV_MSGBOX_BOTTOM - MTV_MSGBOX_TOP - pub_icon_height)/2, IMAGE_PUBWIN_WARNING);
        DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, TXT_NULL);
    }
    else if(MTV_PUBWIN_QUERY == type)
    {
    	GUIRES_GetImgWidthHeight(&pub_icon_width, &pub_icon_height, IMAGE_PUBWIN_QUERY, MMIMTV_MAIN_WIN_ID);     
        DisplayPicture(MMIMTV_PUBWIN_ICON_LEFT, MMIMTV_PUBWIN_ICON_TOP +(MTV_MSGBOX_BOTTOM - MTV_MSGBOX_TOP - pub_icon_height)/2, IMAGE_PUBWIN_QUERY);
        DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL);
    }
    else if(MTV_PUBWIN_QUERY_SR_EXIT == type)
    {
    	GUIRES_GetImgWidthHeight(&pub_icon_width, &pub_icon_height, IMAGE_PUBWIN_QUERY, MMIMTV_MAIN_WIN_ID);     
        DisplayPicture(MMIMTV_PUBWIN_ICON_LEFT, MMIMTV_PUBWIN_ICON_TOP +(MTV_MSGBOX_BOTTOM - MTV_MSGBOX_TOP - pub_icon_height)/2, IMAGE_PUBWIN_QUERY);
        DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, STXT_EXIT);
    }
    else if(MTV_PUBWIN_WAIT_NO_SOFTKEY == type)
    {
    	GUIRES_GetImgWidthHeight(&pub_icon_width, &pub_icon_height, IMAGE_PUBWIN_WAIT, MMIMTV_MAIN_WIN_ID);     
        DisplayPicture(MMIMTV_PUBWIN_ICON_LEFT, MMIMTV_PUBWIN_ICON_TOP +(MTV_MSGBOX_BOTTOM - MTV_MSGBOX_TOP - pub_icon_height)/2, IMAGE_PUBWIN_WAIT);
        DisplaySoftKey(TXT_NULL, TXT_NULL, TXT_NULL);
    }
    else if(MTV_PUBWIN_WAIT == type)
    {
    	GUIRES_GetImgWidthHeight(&pub_icon_width, &pub_icon_height, IMAGE_PUBWIN_WAIT, MMIMTV_MAIN_WIN_ID);     
        DisplayPicture(MMIMTV_PUBWIN_ICON_LEFT, MMIMTV_PUBWIN_ICON_TOP +(MTV_MSGBOX_BOTTOM - MTV_MSGBOX_TOP - pub_icon_height)/2, IMAGE_PUBWIN_WAIT);
        DisplaySoftKey(TXT_NULL, TXT_NULL, STXT_CANCEL);
    }
}

/*****************************************************************************/
// 	Description : open MMIMTV_QUIT_QUERY scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void OpenPubWinByTextPtr(
                               MTV_PUBWIN_TYPE_E type,
                               MMI_STRING_T *text_ptr
                               )
{
    uint16      text_line_num = 0;
    uint16      text_pixel_width = 0;
    GUI_RECT_T  text_rect = MTV_MSGBOX_TEXT_RECT;
    GUI_FONT_T  text_font = MMIMTV_NORMAL_FONT;
    GUI_RECT_T  img_rect = {MTV_MSGBOX_LEFT, MTV_MSGBOX_TOP, MTV_MSGBOX_RIGHT, MTV_MSGBOX_BOTTOM};
    uint16      line_space = 0;
    uint16      char_space = 1;
    uint16      bg_img_width = 0;
    uint16      bg_img_height = 0;
    uint16      pub_icon_width = 0;
    uint16      pub_icon_height = 0;	
	
    //get bg image size
    GUIRES_GetImgWidthHeight(&bg_img_width, &bg_img_height, IMAGE_PUBWIN_BG, MMIMTV_MAIN_WIN_ID);
    if(bg_img_width > 0 && bg_img_height > 0)
    {
        int16       bg_real_right = 0;
        int16       bg_real_bottom = 0;
        
        bg_real_right   = img_rect.left + bg_img_width;
        bg_real_bottom  = img_rect.top + bg_img_height;
        
        //recalculate text rect
        text_rect.right = MIN(bg_real_right, text_rect.right) -1;
        text_rect.bottom = MIN(bg_real_bottom, text_rect.bottom) -1;
    }
    
    text_line_num =  GUI_CalculateStringLinesByPixelNum(text_rect.right - text_rect.left,
        text_ptr->wstr_ptr,
        text_ptr->wstr_len,
        text_font,
        char_space,
        FALSE);
    
    if(text_rect.bottom - text_rect.top > text_line_num * (GUI_GetFontHeight(text_font, UNICODE_HANZI)+line_space))
    {
        text_rect.top = text_rect.top + (text_rect.bottom - text_rect.top - text_line_num * (GUI_GetFontHeight(text_font, UNICODE_HANZI)+line_space))/2;
    }
    
    if(1 == text_line_num)
    {
        //单行文本居中显示
        text_pixel_width = GUI_CalculateStringPiexlNum(text_ptr->wstr_ptr,
            text_ptr->wstr_len,
            text_font,
            char_space);
        if(text_rect.right - text_rect.left > text_pixel_width)
        {
            text_rect.left = text_rect.left + (text_rect.right - text_rect.left - text_pixel_width)/2;
        }
    }
    
    if(MTV_PUBWIN_WAIT == type)
    {
        //don't support PUBWIN_WAIT
        return;
    }
    
    DisplayPictureInRect(&img_rect, IMAGE_PUBWIN_BG);
    DisplayString(text_rect, text_ptr->wstr_ptr, text_ptr->wstr_len, MMIMTV_NORMAL_FONT);
    
	
    if(MTV_PUBWIN_TIP == type)
    {
    	GUIRES_GetImgWidthHeight(&pub_icon_width, &pub_icon_height, IMAGE_PUBWIN_WARNING, MMIMTV_MAIN_WIN_ID); 
        //居中显示pub win icon
        DisplayPicture(MMIMTV_PUBWIN_ICON_LEFT, MMIMTV_PUBWIN_ICON_TOP +(MTV_MSGBOX_BOTTOM - MTV_MSGBOX_TOP - pub_icon_height)/2, IMAGE_PUBWIN_WARNING);
        DisplaySoftKey(TXT_NULL, TXT_NULL, STXT_RETURN);
    }
    else if(MTV_PUBWIN_QUERY == type)
    {
    	GUIRES_GetImgWidthHeight(&pub_icon_width, &pub_icon_height, IMAGE_PUBWIN_QUERY, MMIMTV_MAIN_WIN_ID);     
        DisplayPicture(MMIMTV_PUBWIN_ICON_LEFT, MMIMTV_PUBWIN_ICON_TOP +(MTV_MSGBOX_BOTTOM - MTV_MSGBOX_TOP - pub_icon_height)/2, IMAGE_PUBWIN_QUERY);
        DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL);
    }
    else if(MTV_PUBWIN_QUERY_SR_EXIT == type)
    {
    	GUIRES_GetImgWidthHeight(&pub_icon_width, &pub_icon_height, IMAGE_PUBWIN_QUERY, MMIMTV_MAIN_WIN_ID);     
        DisplayPicture(MMIMTV_PUBWIN_ICON_LEFT, MMIMTV_PUBWIN_ICON_TOP +(MTV_MSGBOX_BOTTOM - MTV_MSGBOX_TOP - pub_icon_height)/2, IMAGE_PUBWIN_QUERY);
        DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, STXT_EXIT);
    }
    else if(MTV_PUBWIN_WAIT_NO_SOFTKEY == type)
    {
    	GUIRES_GetImgWidthHeight(&pub_icon_width, &pub_icon_height, IMAGE_PUBWIN_WAIT, MMIMTV_MAIN_WIN_ID);     
        DisplayPicture(MMIMTV_PUBWIN_ICON_LEFT, MMIMTV_PUBWIN_ICON_TOP +(MTV_MSGBOX_BOTTOM - MTV_MSGBOX_TOP - pub_icon_height)/2, IMAGE_PUBWIN_WAIT);
        DisplaySoftKey(TXT_NULL, TXT_NULL, TXT_NULL);
    }
    else if(MTV_PUBWIN_TIP_SR_OPT == type)
    {
     	GUIRES_GetImgWidthHeight(&pub_icon_width, &pub_icon_height, IMAGE_PUBWIN_WARNING, MMIMTV_MAIN_WIN_ID);     
        DisplayPicture(MMIMTV_PUBWIN_ICON_LEFT, MMIMTV_PUBWIN_ICON_TOP +(MTV_MSGBOX_BOTTOM - MTV_MSGBOX_TOP - pub_icon_height)/2, IMAGE_PUBWIN_WARNING);   
        DisplaySoftKey(STXT_OPTION, TXT_NULL, STXT_RETURN);
    }
}

/*****************************************************************************/
// 	Description : open MMIMTV_QUIT_QUERY scene
//	Global resource dependence :
//  Author:
//	Note: 通用确认场景，左软键"确认"，右软键"取消"
/*****************************************************************************/
PUBLIC void MMIMTV_OpenCommonQueryScene(uint32 param)
{
    OpenPubWin(MTV_PUBWIN_QUERY, param);
}

/*****************************************************************************/
// 	Description : open MMIMTV_QUIT_QUERY scene
//	Global resource dependence :
//  Author:
//	Note: open by text
/*****************************************************************************/
PUBLIC void MMIMTV_OpenCommonQuerySceneByTextPtr(MMI_STRING_T *text_ptr)
{
    OpenPubWinByTextPtr(MTV_PUBWIN_QUERY, text_ptr);
}

/*****************************************************************************/
// 	Description : open MMIMTV_QUIT_QUERY scene
//	Global resource dependence :
//  Author: xin.li
//	Note: 右软键为"退出"的询问场景
/*****************************************************************************/
PUBLIC void MMIMTV_OpenCommonQuerySceneWithSRExit(uint32 param)
{
    OpenPubWin(MTV_PUBWIN_QUERY_SR_EXIT, param);
}

/*****************************************************************************/
// 	Description : open MMIMTV_QUIT_QUERY scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenCommonTipScene(uint32 param)
{
    OpenPubWin(MTV_PUBWIN_TIP, param);
    CreateDisappearTimer(MMIMTV_SYSTIME_TIP_PUBWIN);
}

/*****************************************************************************/
// 	Description : open common tip scene by text
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenCommonTipSceneByTextPtr(MMI_STRING_T *text_ptr)
{
    OpenPubWinByTextPtr(MTV_PUBWIN_TIP, text_ptr);
    CreateDisappearTimer(MMIMTV_SYSTIME_TIP_PUBWIN);
}

/*****************************************************************************/
// 	Description : open common confirm scene
//	Global resource dependence :
//  Author:
//	Note: 确定后消失
/*****************************************************************************/
PUBLIC void MMIMTV_OpenCommonConfirmScene(uint32 param)
{
    OpenPubWin(MTV_PUBWIN_CONFIRM, param);
}

/*****************************************************************************/
// 	Description : open MMIMTV_QUIT_QUERY scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenCommonWaitWithNoCancelScene(uint32 param)
{
    OpenPubWin(MTV_PUBWIN_WAIT_NO_SOFTKEY, param);
    //CreateDisappearTimer(MMIMTV_SYSTIME_TIP_PUBWIN);
}

/*****************************************************************************/
// 	Description : open MMIMTV_QUIT_QUERY scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenCommonWaitScene(uint32 param)
{
    MMIMTV_OSD_DISPLAY_PTR->waiting_step = 0;
    MMIMTV_OSD_DISPLAY_PTR->wait_pubwin_text_id = param;
    OpenPubWin(MTV_PUBWIN_WAIT, param);
    CreateDisappearTimer(MMIMTV_SYSTIME_WAITING_PUBWIN);
}

/*****************************************************************************/
// 	Description : open OSD_WARNING scene
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseCommonMsgBox(void)
{
    GUI_RECT_T rect = {MTV_MSGBOX_LEFT, MTV_MSGBOX_TOP, MTV_MSGBOX_RIGHT, MTV_MSGBOX_BOTTOM};

    ClearRect(rect);
    MMIMTV_StopSysTimer(MMIMTV_SYSTIME_WAITING_PUBWIN);
    MMIMTV_StopSysTimer(MMIMTV_SYSTIME_TIP_PUBWIN);
}

/*****************************************************************************/
// 	Description :open MMIMTV_SERVICE_LIST scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void AppendListItem(
                          MTV_CONTROL_ID_E ctrl_id,
                          wchar *text_buf
                          )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(text_buf);    

    item_data.item_content[0].item_data.text_buffer.wstr_ptr = text_buf;

//     MMIAPICOM_Wstrncpy(item_data.item_content[0].item_data.text_buffer.wstr, text_buf, 
//         item_data.item_content[0].item_data.text_buffer.wstr_len);
    GUILIST_AppendItem(ctrl_id, &item_t);   
}

/*****************************************************************************/
// 	Description :open MMIMTV_SERVICE_LIST scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void AppendListItemViaId(
                               MMI_CTRL_ID_T ctrl_id,
                               MMI_TEXT_ID_T text_id
                               )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;
    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
// 	Description :open MMIMTV_SERVICE_LIST scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenServiceListScene(uint32 param)
{
    MMIMTV_SAVE_NETEID_INFO_T* net = PNULL;
    MMIMTV_SAVE_SERVICE_INFO_T* cursor = PNULL;
    uint32 list_num = 0;
    uint32 index = 0;
    GUI_RECT_T rect = {MMIMTV_CMMB_LOGO_LEFT, MMIMTV_CMMB_LOGO_TOP, MTV_OSD_RIGHT, MTV_OSD_TOP};
    GUI_RECT_T  list_rect = {0};
    
    if(PNULL == (MMIMTV_SAVE_NETEID_INFO_T*)param)
    {
        return;
    }
    
    net = (MMIMTV_SAVE_NETEID_INFO_T*)param;
    list_num = MMIMTV_GetServiceListNumber(net);
    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_SERVICE_LIST_WIN_TAB);
    list_rect.left   = MTV_OSD_LEFT;
    list_rect.top    = MTV_OSD_TOP;
    list_rect.right  = MTV_OSD_RIGHT;
    list_rect.bottom = MTV_OSD_BOTTOM;
    GUILIST_SetRect(MTV_SERVICE_LIST_CTRL_ID,&list_rect);

    GUILIST_SetBgColor(MTV_SERVICE_LIST_CTRL_ID, MMIMTV_BACKGROUND_COLOR);
    GUILIST_SetTextFont(MTV_SERVICE_LIST_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMI_GRAY_WHITE_COLOR);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_SERVICE_LIST_CTRL_ID);
    GUILIST_SetMaxItem(MTV_SERVICE_LIST_CTRL_ID, list_num, FALSE);
    //特殊处理, 响应左右按键
    GUILIST_SetHandleLeftRight( MTV_SERVICE_LIST_CTRL_ID, TRUE );

    if(0 == list_num)
    {
        MMI_STRING_T text_str = {0};

        GUILIST_SetMaxItem(MTV_SERVICE_LIST_CTRL_ID, 1, FALSE);
        MMI_GetLabelTextByLang(TXT_EMPTY, &text_str);
        AppendListItem(MTV_SERVICE_LIST_CTRL_ID, text_str.wstr_ptr);
        DisplaySoftKey(TXT_COMMON_UPDATE, TXT_NULL, STXT_RETURN);
    }
    else
    {
        while (list_num --)
        {
            cursor = MMIMTV_GetServiceNodeViaIndex(net, index);
            AppendListItem(MTV_SERVICE_LIST_CTRL_ID, cursor->service_name);
            index ++;
        }
        DisplaySoftKey(STXT_OPTION, TXT_NULL, STXT_RETURN);
    }
    DisplayTitle(TXT_COMMON_COMMON_CHANNEL_LIST);
    
#ifndef MBBMS_SUPPORT
    if(MMIMTV_GetNetListNumber() > 1)
    {
        MMIMTV_OSD_DISPLAY_PTR->left_arrow_rect.left = MMIMTV_LEFT_ARROW_LEFT;
        MMIMTV_OSD_DISPLAY_PTR->left_arrow_rect.top = MMIMTV_LEFT_ARROW_TOP;
        MMIMTV_OSD_DISPLAY_PTR->left_arrow_rect.right = MMIMTV_LEFT_ARROW_RIGHT;
        MMIMTV_OSD_DISPLAY_PTR->left_arrow_rect.bottom = MMIMTV_LEFT_ARROW_BOTTOM;
        
        MMIMTV_OSD_DISPLAY_PTR->right_arrow_rect.left = MMIMTV_RIGHT_ARROW_LEFT;
        MMIMTV_OSD_DISPLAY_PTR->right_arrow_rect.top = MMIMTV_RIGHT_ARROW_TOP;
        MMIMTV_OSD_DISPLAY_PTR->right_arrow_rect.right = MMIMTV_RIGHT_ARROW_RIGHT;
        MMIMTV_OSD_DISPLAY_PTR->right_arrow_rect.bottom = MMIMTV_RIGHT_ARROW_BOTTOM;
        
        DisplayPicture(MMIMTV_OSD_DISPLAY_PTR->left_arrow_rect.left, MMIMTV_OSD_DISPLAY_PTR->left_arrow_rect.top, IMAGE_LIST_TITLE_ARROW_LEFT_UP);
        DisplayPicture(MMIMTV_OSD_DISPLAY_PTR->right_arrow_rect.left, MMIMTV_OSD_DISPLAY_PTR->right_arrow_rect.top, IMAGE_LIST_TITLE_ARROW_RIGHT_UP); 
    }
#else
    //display cmmb logo
    DisplayPicture(rect.left, rect.top, IMAGE_MTV_LOGO);
#endif
}

/*****************************************************************************/
// 	Description : close MMIMTV_SERVICE_LIST scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseServiceListScene(void)
{
    GUI_RECT_T  rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};
    GUI_RECT_T  *left_arrow_rect_ptr = &(MMIMTV_OSD_DISPLAY_PTR->left_arrow_rect);
    GUI_RECT_T  *right_arrow_rect_ptr = &(MMIMTV_OSD_DISPLAY_PTR->right_arrow_rect);

    MMK_DestroyControl(MTV_SERVICE_LIST_CTRL_ID);
    ClearRect(rect);
    SCI_MEMSET(left_arrow_rect_ptr, 0, sizeof(GUI_RECT_T));
    SCI_MEMSET(right_arrow_rect_ptr, 0, sizeof(GUI_RECT_T));
    ClearTitle();
}

/*****************************************************************************/
// 	Description : close MMIMTV_SERVICE_LIST scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_GetChangeNetArrowRect(
                                         GUI_RECT_T *left_rect,
                                         GUI_RECT_T *right_rect
                                         )
{
    SCI_MEMCPY(left_rect, &(MMIMTV_OSD_DISPLAY_PTR->left_arrow_rect), sizeof(GUI_RECT_T));
    SCI_MEMCPY(right_rect, &(MMIMTV_OSD_DISPLAY_PTR->right_arrow_rect), sizeof(GUI_RECT_T));
}

/*****************************************************************************/
// 	Description : open MMIMTV_SERVICE_LIST_OPT scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenServiceListOptScene(uint32 param)
{
    uint32      first_index = 0;
    GUI_RECT_T  menu_rect = {0};
    
    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_SERVICE_OPTION_MENU_WIN_TAB);
    //set menu rect
    menu_rect.left   = MTV_OSD_LEFT;
    menu_rect.top    = MTV_OSD_TOP;
    menu_rect.right  = MTV_OSD_RIGHT;
    menu_rect.bottom = MTV_OSD_BOTTOM;
    GUIMENU_SetRect(MTV_SERVICE_OPTION_MENU_CTRL_ID,&menu_rect);
    GUIMENU_SetMenuBgColor(MTV_SERVICE_OPTION_MENU_CTRL_ID, MMIMTV_BACKGROUND_COLOR);
    GUIMENU_SetMenuFontColor(MTV_SERVICE_OPTION_MENU_CTRL_ID, MMI_GRAY_WHITE_COLOR);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_SERVICE_OPTION_MENU_CTRL_ID);
    if(param >= MMIMTV_ITEM_MAX_INDEX)
    {
        first_index = param - MMIMTV_ITEM_MAX_INDEX + 1;
    }
    GUIMENU_SetFirstAndSelectedItem(first_index, param, MTV_SERVICE_OPTION_MENU_CTRL_ID);
    DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    DisplayTitle(STXT_OPTION);
}

/*****************************************************************************/
// 	Description : close MMIMTV_SERVICE_LIST_OPT scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseServiceListOptScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_SERVICE_OPTION_MENU_CTRL_ID);
    ClearRect(rect);
    ClearTitle();
}

/*****************************************************************************/
// 	Description : open MMIMTV_NET_LIST scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenNetListScene(uint32 param)
{
    MMIMTV_SAVE_NETEID_INFO_T* cursor = PNULL;
    uint32 list_num = MMIMTV_GetNetListNumber();
    uint32 index = 0;
    GUI_RECT_T rect = {MMIMTV_CMMB_LOGO_LEFT, MMIMTV_CMMB_LOGO_TOP, MTV_OSD_RIGHT, MTV_OSD_TOP};
        GUI_RECT_T          list_rect = {0};
    
    //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_OpenNetListScene list_num = %d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_2751_112_2_18_2_40_44_866,(uint8*)"d", list_num);

    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_NET_LIST_WIN_TAB);
    list_rect.left   = MTV_OSD_LEFT;
    list_rect.top    = MTV_OSD_TOP;
    list_rect.right  = MTV_OSD_RIGHT;
    list_rect.bottom = MTV_OSD_BOTTOM;
    GUILIST_SetRect(MTV_NET_LIST_CTRL_ID,&list_rect);

    GUILIST_SetBgColor(MTV_NET_LIST_CTRL_ID, MMIMTV_BACKGROUND_COLOR);
    GUILIST_SetTextFont(MTV_NET_LIST_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMI_GRAY_WHITE_COLOR);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_NET_LIST_CTRL_ID);
    GUILIST_SetMaxItem(MTV_NET_LIST_CTRL_ID, list_num, FALSE);
    //特殊处理, 响应左右按键
    GUILIST_SetHandleLeftRight( MTV_NET_LIST_CTRL_ID, TRUE );
    
    if(0 == list_num)
    {
        MMI_STRING_T text_str = {0};

        GUILIST_SetMaxItem(MTV_NET_LIST_CTRL_ID, 1, FALSE);
        MMI_GetLabelTextByLang(TXT_EMPTY, &text_str);
        AppendListItem(MTV_NET_LIST_CTRL_ID, text_str.wstr_ptr);
        DisplaySoftKey(TXT_COMMON_UPDATE, TXT_NULL, STXT_RETURN);
    }
    else
    {
        while (list_num --)
        {
            cursor = MMIMTV_GetNetNodeViaIndex(index);
            AppendListItem(MTV_NET_LIST_CTRL_ID, cursor->net_name);
            index ++;
        }
        DisplaySoftKey(STXT_OPTION, TXT_NULL, STXT_RETURN);
    }
    DisplayTitle(TXT_NETWORK_LIST);
    //display cmmb logo
#ifdef MBBMS_SUPPORT
    DisplayPicture(rect.left, rect.top, IMAGE_MTV_LOGO);
#endif  
}

/*****************************************************************************/
// 	Description : close MMIMTV_NET_LIST scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseNetListScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_NET_LIST_CTRL_ID);
    ClearRect(rect);
    ClearTitle();
}

/*****************************************************************************/
// 	Description : open MMIMTV_NET_LIST_OPT scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenNetListOptScene(uint32 item_index)
{
    uint32      first_index = 0;
    GUI_RECT_T  menu_rect = {0};
    
    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_NET_OPTION_MENU_WIN_TAB);
    //set menu rect
    menu_rect.left   = MTV_OSD_LEFT;
    menu_rect.top    = MTV_OSD_TOP;
    menu_rect.right  = MTV_OSD_RIGHT;
    menu_rect.bottom = MTV_OSD_BOTTOM;
    GUIMENU_SetRect(MTV_NET_OPTION_MENU_CTRL_ID,&menu_rect);
    GUIMENU_SetMenuBgColor(MTV_NET_OPTION_MENU_CTRL_ID, MMIMTV_BACKGROUND_COLOR);
    GUIMENU_SetMenuFontColor(MTV_NET_OPTION_MENU_CTRL_ID, MMI_GRAY_WHITE_COLOR);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_NET_OPTION_MENU_CTRL_ID);
    if(item_index >= MMIMTV_ITEM_MAX_INDEX)
    {
        first_index = item_index - MMIMTV_ITEM_MAX_INDEX + 1;
    }
    GUIMENU_SetFirstAndSelectedItem(first_index, item_index, MTV_NET_OPTION_MENU_CTRL_ID);
    DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    DisplayTitle(STXT_OPTION);
}

/*****************************************************************************/
// 	Description : close MMIMTV_NET_LIST_OPT scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseNetListOptScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_NET_OPTION_MENU_CTRL_ID);
    ClearRect(rect);
    ClearTitle();
}

/*****************************************************************************/
// 	Description : open MMIMTV_NET_LIST_OPT scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenOtherFunctionScene(uint32 param)
{
    uint32      first_index = 0;
    GUI_RECT_T  menu_rect = {0};

    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_OTHER_FUNCTION_MENU_WIN_TAB);
    //set menu rect
    menu_rect.left   = MTV_OSD_LEFT;
    menu_rect.top    = MTV_OSD_TOP;
    menu_rect.right  = MTV_OSD_RIGHT;
    menu_rect.bottom = MTV_OSD_BOTTOM;
    GUIMENU_SetRect(MTV_ID_OTHER_FUNCTIONS_CTRL_MENU,&menu_rect);
    GUIMENU_SetMenuBgColor(MTV_ID_OTHER_FUNCTIONS_CTRL_MENU, MMIMTV_BACKGROUND_COLOR);
    GUIMENU_SetMenuFontColor(MTV_ID_OTHER_FUNCTIONS_CTRL_MENU, MMI_GRAY_WHITE_COLOR);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_ID_OTHER_FUNCTIONS_CTRL_MENU);
    if(param >= MMIMTV_ITEM_MAX_INDEX)
    {
        first_index = param - MMIMTV_ITEM_MAX_INDEX + 1;
    }
    GUIMENU_SetFirstAndSelectedItem(first_index, param, MTV_ID_OTHER_FUNCTIONS_CTRL_MENU);
    DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    DisplayTitle(TXT_MTV_OTHER_FUNCTIONS);
}

/*****************************************************************************/
// 	Description : close MMIMTV_NET_LIST_OPT scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseOtherFunctionScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_ID_OTHER_FUNCTIONS_CTRL_MENU);
    ClearRect(rect);
    ClearTitle();
	MMITHEME_UpdateRect();
}

/*****************************************************************************/
// 	Description : open setting scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenSettingScene(uint32 param)
{
    BOOLEAN is_udisk_exist = FALSE;
    BOOLEAN is_sdcard_exist = FALSE;
    GUI_RECT_T  list_rect = {0};
    
    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_SETTING_LIST_WIN_TAB);
    list_rect.left   = MTV_OSD_LEFT;
    list_rect.top    = MTV_OSD_TOP;
    list_rect.right  = MTV_OSD_RIGHT;
    list_rect.bottom = MTV_OSD_BOTTOM;
    GUILIST_SetRect(MTV_SETTING_LIST_CTRL_ID,&list_rect);

    GUILIST_SetBgColor(MTV_SETTING_LIST_CTRL_ID, MMIMTV_BACKGROUND_COLOR);
    GUILIST_SetTextFont(MTV_SETTING_LIST_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMI_GRAY_WHITE_COLOR);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_SETTING_LIST_CTRL_ID);
    GUILIST_SetMaxItem(MTV_SETTING_LIST_CTRL_ID, MMIMTV_SETTING_MAX, FALSE);
    
    //AppendListItemForSettingViaId(MTV_SETTING_LIST_CTRL_ID, TXT_MTV_AUDIO_LANGUAGE,
    //    MMIMTV_GetCurrentAudioLanguage(), MMIMTV_AUDIO_LANGUAGE);
    
    //AppendListItemForSettingViaId(MTV_SETTING_LIST_CTRL_ID, TXT_MTV_SUBTITLE_STREAM,     
    //    MMIMTV_GetSettingInfo()->subtitle_streams + TXT_COMMON_ENABLE_ON);
    
    //AppendListItemForSettingViaId(MTV_SETTING_LIST_CTRL_ID, TXT_MTV_AUDIO_MODE,
    //    MMIMTV_GetSettingInfo()->audio_mode + TXT_MTV_SINGLE_CHANNEL);

    //AppendListItemForSettingViaId(MTV_SETTING_LIST_CTRL_ID, TXT_MTV_ZOOM_MODE,
    //    MMIMTV_GetZoomMode() + TXT_MTV_16_9, MMIMTV_ZOOM_MODE);

    //只有U盘和SD卡同时存在，才显示录像/截图优先存储位置
    is_udisk_exist  = MMIAPIFMM_GetDeviceTypeStatus(MMI_DEVICE_UDISK);
    is_sdcard_exist = MMISD_IsSDExistEx();
    if(is_udisk_exist && is_sdcard_exist)
    {
        AppendListItemForSettingViaId(MTV_SETTING_LIST_CTRL_ID, TXT_MTV_SNAPSHOT_STORE, 
            MMIAPIFMM_GetDeviceName((MMIFILE_DEVICE_E)MMIMTV_GetSnapshotStore()), MMIMTV_SNAPSHOT_STORE);
    }

    AppendListItemForSettingViaId(MTV_SETTING_LIST_CTRL_ID, TXT_MTV_OSD_TRANSPARENCY,
        MMIMTV_GetTransparency() + TXT_MTV_15PERCENT, MMIMTV_OSD_TRANSPARENCY);

    AppendListItemForSettingViaId(MTV_SETTING_LIST_CTRL_ID, TXT_MTV_OSD_DISPLAY_TIME,
        MMIMTV_GetOSDDisplayTime() + TXT_MTV_3S, MMIMTV_OSD_DISPLAY_TIME);

//    AppendListItemForSettingViaId(MTV_SETTING_LIST_CTRL_ID, TXT_MTV_WATCH_TIME_TIP,
//        MMIMTV_GetWatchTimeTip() + TXT_COMM_10MINUTES, MMIMTV_WATCH_TIME_TIP);
    
// #ifndef MBBMS_SUPPORT
//     AppendListItemForSettingViaId(MTV_SETTING_LIST_CTRL_ID, TXT_MTV_BROADCAST_BG_PIC, 
//         MMIMTV_GetBroadcastBG() + TXT_MTV_BG_PIC_1, MMIMTV_BROADCAST_BG_PIC);
// #else
// #ifdef ENG_SUPPORT
//     if(MMIAPIENG_IsAudioServiceEnable())
//     {
//         AppendListItemForSettingViaId(MTV_SETTING_LIST_CTRL_ID, TXT_MTV_BROADCAST_BG_PIC, 
//             MMIMTV_GetBroadcastBG() + TXT_MTV_BG_PIC_1, MMIMTV_BROADCAST_BG_PIC);
//     }
// #endif
// #endif
    
    //计算item停留位置
    if(is_udisk_exist && is_sdcard_exist)
    {
        GUILIST_SetCurItemIndex(MTV_SETTING_LIST_CTRL_ID,param);
    }
    else
    {
        if(param > MMIMTV_SNAPSHOT_STORE)
        {
            param --;
        }
        GUILIST_SetCurItemIndex(MTV_SETTING_LIST_CTRL_ID,param);
    }
    DisplaySoftKey(STXT_MAIN_SETTING, TXT_NULL, STXT_RETURN);
    DisplayTitle(STXT_MAIN_SETTING);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseSettingScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_SETTING_LIST_CTRL_ID);
    ClearRect(rect);
    ClearTitle();
}

/*****************************************************************************/
// 	Description : open setting scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenAudioLanguageScene(uint32 param)
{
    MMIMTV_AUDIO_LANGUAGE_SCENE_T* scene_param = PNULL;
    MMI_CTRL_ID_T ctrl_id = MTV_LANGUAGE_LIST_CTRL_ID;
    uint16 cur_selection = 0;
    uint16 i = 0;
    GUI_RECT_T  list_rect = {0};
    
    if(PNULL == param)
    {
        return;
    }

    scene_param = (MMIMTV_AUDIO_LANGUAGE_SCENE_T*)param;
    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_LANGUAGE_LIST_WIN_TAB);

    list_rect.left   = MTV_OSD_LEFT;
    list_rect.top    = MTV_OSD_TOP;
    list_rect.right  = MTV_OSD_RIGHT;
    list_rect.bottom = MTV_OSD_BOTTOM;
    GUILIST_SetRect(ctrl_id,&list_rect);

    GUILIST_SetBgColor(ctrl_id, MMIMTV_BACKGROUND_COLOR);
    GUILIST_SetTextFont(ctrl_id, MMI_DEFAULT_TEXT_FONT, MMI_GRAY_WHITE_COLOR);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, ctrl_id);

    if(scene_param->audio_lang_count > 0 && PNULL != scene_param->lang_code_ptr)
    {
        GUILIST_SetMaxItem(ctrl_id,scene_param->audio_lang_count,FALSE);
        for(i=0; i<scene_param->audio_lang_count; i++)
        {
            //SCI_TRACE_LOW:"MMIMTV_OpenAudioLanguageScene lang_code_ptr = %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_3019_112_2_18_2_40_45_867,(uint8*)"d",scene_param->lang_code_ptr[i]);
            if(scene_param->lang_code_ptr[i] == MMIMTV_CHINESE_CODE)
            {
                AppendListItemViaId(ctrl_id, TXT_COMMON_LANG_CHINESE);
                if (scene_param->lang_code_ptr[i] == scene_param->audio_language_code)
                {
                    cur_selection = GUILIST_GetTotalItemNum(ctrl_id);
                }
            }
            else if(scene_param->lang_code_ptr[i] == MMIMTV_ENGLISH_CODE)
            {
                AppendListItemViaId(ctrl_id, TXT_LANG_ENGLISH);
                if (scene_param->lang_code_ptr[i] == scene_param->audio_language_code)
                {
                    cur_selection = GUILIST_GetTotalItemNum(ctrl_id);
                }
            }
            else    //未知语言
            {
                AppendListItemViaId(ctrl_id, TXT_UNKNOWN);
                //SCI_TRACE_LOW:"MMIMTV_OpenAudioLanguageScene lang_code = %x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_3039_112_2_18_2_40_45_868,(uint8*)"d",scene_param->audio_language_code);
                if (scene_param->lang_code_ptr[i] == scene_param->audio_language_code)
                {
                    cur_selection = GUILIST_GetTotalItemNum(ctrl_id);
                }
            }
        }      
    }
    else
    {
        GUILIST_SetMaxItem(ctrl_id,1,FALSE);
        AppendListItemViaId(ctrl_id, TXT_EMPTY);
        GUILIST_SetItemGray(ctrl_id,0,TRUE);
        cur_selection =1;
            
    }
    GUILIST_SetSelectedItem(ctrl_id, cur_selection - 1, TRUE);
    GUILIST_SetCurItemIndex(ctrl_id, cur_selection - 1); 
    DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    DisplayTitle(TXT_MTV_AUDIO_LANGUAGE);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseAudioLanguageScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_LANGUAGE_LIST_CTRL_ID);
    ClearRect(rect);
    ClearTitle();
}

/*****************************************************************************/
// 	Description : open setting scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenSubtitleSettingScene(uint32 param)
{
    GUI_RECT_T          list_rect = {0};
    //MMIMTV_SETTING_INFO_T* setting = PNULL;
    
    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_SUBTITLE_LIST_WIN_TAB);

    list_rect.left   = MTV_OSD_LEFT;
    list_rect.top    = MTV_OSD_TOP;
    list_rect.right  = MTV_OSD_RIGHT;
    list_rect.bottom = MTV_OSD_BOTTOM;
    GUILIST_SetRect(MTV_SUBTITLE_LIST_CTRL_ID,&list_rect);

    GUILIST_SetBgColor(MTV_SUBTITLE_LIST_CTRL_ID, MMIMTV_BACKGROUND_COLOR);
    GUILIST_SetTextFont(MTV_SUBTITLE_LIST_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMI_GRAY_WHITE_COLOR);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_SUBTITLE_LIST_CTRL_ID);

    GUILIST_SetMaxItem(MTV_SUBTITLE_LIST_CTRL_ID, 2, FALSE);
    AppendListItemViaId(MTV_SUBTITLE_LIST_CTRL_ID, TXT_COMMON_ENABLE_ON);
    AppendListItemViaId(MTV_SUBTITLE_LIST_CTRL_ID, TXT_CLOSE);
    GUILIST_SetCurItemIndex(MTV_SUBTITLE_LIST_CTRL_ID, MMIMTV_GetSubtitleStreams());
    GUILIST_SetSelectedItem(MTV_SUBTITLE_LIST_CTRL_ID, MMIMTV_GetSubtitleStreams(), TRUE);
    DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    DisplayTitle(TXT_MTV_SUBTITLE_STREAM);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseSubtitleSettingScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_SUBTITLE_LIST_CTRL_ID);
    ClearRect(rect);
    ClearTitle();
}

/*****************************************************************************/
// 	Description : open setting scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenAudioModeScene(uint32 param)
{
    GUI_RECT_T          list_rect = {0};

    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_AUDIO_MODE_LIST_WIN_TAB);

    list_rect.left   = MTV_OSD_LEFT;
    list_rect.top    = MTV_OSD_TOP;
    list_rect.right  = MTV_OSD_RIGHT;
    list_rect.bottom = MTV_OSD_BOTTOM;
    GUILIST_SetRect(MTV_AUDIO_MODE_LIST_CTRL_ID,&list_rect);

    GUILIST_SetBgColor(MTV_AUDIO_MODE_LIST_CTRL_ID, MMIMTV_BACKGROUND_COLOR);
    GUILIST_SetTextFont(MTV_AUDIO_MODE_LIST_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMI_GRAY_WHITE_COLOR);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_AUDIO_MODE_LIST_CTRL_ID);

    GUILIST_SetMaxItem(MTV_AUDIO_MODE_LIST_CTRL_ID, 2, FALSE);
    AppendListItemViaId(MTV_AUDIO_MODE_LIST_CTRL_ID, TXT_MTV_SINGLE_CHANNEL);
    AppendListItemViaId(MTV_AUDIO_MODE_LIST_CTRL_ID, TXT_MTV_STEREO);
    GUILIST_SetCurItemIndex(MTV_AUDIO_MODE_LIST_CTRL_ID, MMIMTV_GetAudioMode());
    GUILIST_SetSelectedItem(MTV_AUDIO_MODE_LIST_CTRL_ID, MMIMTV_GetAudioMode(), TRUE);
    DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    DisplayTitle(TXT_MTV_AUDIO_MODE);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseAudioModeScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_AUDIO_MODE_LIST_CTRL_ID);
    ClearRect(rect);
    ClearTitle();
}

/*****************************************************************************/
// 	Description : open setting scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenZoomModeScene(uint32 param)
{
    GUI_RECT_T          list_rect = {0};

    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_ZOOM_MODE_LIST_WIN_TAB);

    list_rect.left   = MTV_OSD_LEFT;
    list_rect.top    = MTV_OSD_TOP;
    list_rect.right  = MTV_OSD_RIGHT;
    list_rect.bottom = MTV_OSD_BOTTOM;
    GUILIST_SetRect(MTV_ZOOM_MODE_LIST_CTRL_ID,&list_rect);

    GUILIST_SetBgColor(MTV_ZOOM_MODE_LIST_CTRL_ID, MMIMTV_BACKGROUND_COLOR);
    GUILIST_SetTextFont(MTV_ZOOM_MODE_LIST_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMI_GRAY_WHITE_COLOR);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_ZOOM_MODE_LIST_CTRL_ID);

    GUILIST_SetMaxItem(MTV_ZOOM_MODE_LIST_CTRL_ID, MMIMTV_ZOOM_MODE_MAX, FALSE);
    AppendListItemViaId(MTV_ZOOM_MODE_LIST_CTRL_ID, TXT_MTV_16_9);
    AppendListItemViaId(MTV_ZOOM_MODE_LIST_CTRL_ID, TXT_MTV_4_3);
    AppendListItemViaId(MTV_ZOOM_MODE_LIST_CTRL_ID, TXT_MTV_ZOOM_FRAME);
    GUILIST_SetCurItemIndex(MTV_ZOOM_MODE_LIST_CTRL_ID, MMIMTV_GetZoomMode());
    GUILIST_SetSelectedItem(MTV_ZOOM_MODE_LIST_CTRL_ID, MMIMTV_GetZoomMode(), TRUE);
    DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    DisplayTitle(TXT_MTV_ZOOM_MODE);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseZoomModeScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_ZOOM_MODE_LIST_CTRL_ID);
    ClearRect(rect);
    ClearTitle();
}

/*****************************************************************************/
// 	Description : open setting scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenRecordStoreScene(uint32 param)
{
    GUI_RECT_T          list_rect = {0};
	uint16              index = 0;
    MMIFILE_DEVICE_E    device = MMI_DEVICE_UDISK;
    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_RECORD_STORE_LIST_WIN_TAB);

    list_rect.left   = MTV_OSD_LEFT;
    list_rect.top    = MTV_OSD_TOP;
    list_rect.right  = MTV_OSD_RIGHT;
    list_rect.bottom = MTV_OSD_BOTTOM;
    GUILIST_SetRect(MTV_RECORD_STORE_LIST_CTRL_ID,&list_rect);

    GUILIST_SetBgColor(MTV_RECORD_STORE_LIST_CTRL_ID, MMIMTV_BACKGROUND_COLOR);
    GUILIST_SetTextFont(MTV_RECORD_STORE_LIST_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMI_GRAY_WHITE_COLOR);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_RECORD_STORE_LIST_CTRL_ID);

    GUILIST_SetMaxItem(MTV_RECORD_STORE_LIST_CTRL_ID, MMI_DEVICE_NUM, FALSE);
    for(device = MMI_DEVICE_UDISK; device < MMI_DEVICE_NUM; device++)
	{
		AppendListItemViaId(MTV_RECORD_STORE_LIST_CTRL_ID, MMIAPIFMM_GetDeviceName(device));
		if(MMIMTV_GetRecordStore() < device)
		{
              index++;
		}
	}
    GUILIST_SetCurItemIndex(MTV_RECORD_STORE_LIST_CTRL_ID, index);
    GUILIST_SetSelectedItem(MTV_RECORD_STORE_LIST_CTRL_ID, index, TRUE);
    DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    DisplayTitle(TXT_MTV_RECORD_STORE);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseRecordStoreScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_RECORD_STORE_LIST_CTRL_ID);
    ClearRect(rect);
    ClearTitle();
}

/*****************************************************************************/
// 	Description : open setting scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenSnapShotStoreScene(uint32 param)
{
    GUI_RECT_T          list_rect = {0};
    MMIFILE_DEVICE_E    dev = MMI_DEVICE_UDISK;
    
    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_SNAPSHOT_LIST_WIN_TAB);

    list_rect.left   = MTV_OSD_LEFT;
    list_rect.top    = MTV_OSD_TOP;
    list_rect.right  = MTV_OSD_RIGHT;
    list_rect.bottom = MTV_OSD_BOTTOM;
    GUILIST_SetRect(MTV_SNAPSHOT_STORE_LIST_CTRL_ID,&list_rect);

    GUILIST_SetBgColor(MTV_SNAPSHOT_STORE_LIST_CTRL_ID, MMIMTV_BACKGROUND_COLOR);
    GUILIST_SetTextFont(MTV_SNAPSHOT_STORE_LIST_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMI_GRAY_WHITE_COLOR);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_SNAPSHOT_STORE_LIST_CTRL_ID);

    GUILIST_SetMaxItem(MTV_SNAPSHOT_STORE_LIST_CTRL_ID, MMI_DEVICE_NUM, FALSE);

    do
    {
        AppendListItemViaId(MTV_SNAPSHOT_STORE_LIST_CTRL_ID, MMIAPIFMM_GetDeviceName(dev));
    }while(++dev < MMI_DEVICE_NUM);

    GUILIST_SetCurItemIndex(MTV_SNAPSHOT_STORE_LIST_CTRL_ID, MMIMTV_GetSnapshotStore());
    GUILIST_SetSelectedItem(MTV_SNAPSHOT_STORE_LIST_CTRL_ID, MMIMTV_GetSnapshotStore(), TRUE);
    DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    DisplayTitle(TXT_MTV_SNAPSHOT_STORE);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseSnapShotStoreScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_SNAPSHOT_STORE_LIST_CTRL_ID);
    ClearRect(rect);
    ClearTitle();
}

/*****************************************************************************/
// 	Description : open setting scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenTransparencySettingScene(uint32 param)
{
    GUI_RECT_T  list_rect = {0};

    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_TRANSPARENCY_LIST_WIN_TAB);

    list_rect.left   = MTV_OSD_LEFT;
    list_rect.top    = MTV_OSD_TOP;
    list_rect.right  = MTV_OSD_RIGHT;
    list_rect.bottom = MTV_OSD_BOTTOM;
    GUILIST_SetRect(MTV_TRANSPARENCY_LIST_CTRL_ID,&list_rect);

    GUILIST_SetBgColor(MTV_TRANSPARENCY_LIST_CTRL_ID, MMIMTV_BACKGROUND_COLOR);
    GUILIST_SetTextFont(MTV_TRANSPARENCY_LIST_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMI_GRAY_WHITE_COLOR);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_TRANSPARENCY_LIST_CTRL_ID);

    GUILIST_SetMaxItem(MTV_TRANSPARENCY_LIST_CTRL_ID, MMIMTV_OSD_TRANSPARENCY_MAX, FALSE);
    AppendListItemViaId(MTV_TRANSPARENCY_LIST_CTRL_ID, TXT_MTV_15PERCENT);
    AppendListItemViaId(MTV_TRANSPARENCY_LIST_CTRL_ID, TXT_MTV_25PERCENT);
    AppendListItemViaId(MTV_TRANSPARENCY_LIST_CTRL_ID, TXT_COMM_50PERCENT);
    AppendListItemViaId(MTV_TRANSPARENCY_LIST_CTRL_ID, TXT_COMM_75PERCENT);
    GUILIST_SetCurItemIndex(MTV_TRANSPARENCY_LIST_CTRL_ID, MMIMTV_GetTransparency());
    GUILIST_SetSelectedItem(MTV_TRANSPARENCY_LIST_CTRL_ID, MMIMTV_GetTransparency(), TRUE);
    DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    DisplayTitle(TXT_MTV_OSD_TRANSPARENCY);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseTransparencySettingScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_TRANSPARENCY_LIST_CTRL_ID);
    ClearRect(rect);
    ClearTitle();
}

/*****************************************************************************/
// 	Description : open setting scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenDisplayTimeSettingScene(uint32 param)
{
    GUI_RECT_T          list_rect = {0};

    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_DISPLAY_TIME_LIST_WIN_TAB);

    list_rect.left   = MTV_OSD_LEFT;
    list_rect.top    = MTV_OSD_TOP;
    list_rect.right  = MTV_OSD_RIGHT;
    list_rect.bottom = MTV_OSD_BOTTOM;
    GUILIST_SetRect(MTV_OSD_DISPLAY_TIME_LIST_CTRL_ID,&list_rect);

    GUILIST_SetBgColor(MTV_OSD_DISPLAY_TIME_LIST_CTRL_ID, MMIMTV_BACKGROUND_COLOR);
    GUILIST_SetTextFont(MTV_OSD_DISPLAY_TIME_LIST_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMI_GRAY_WHITE_COLOR);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_OSD_DISPLAY_TIME_LIST_CTRL_ID);

    GUILIST_SetMaxItem(MTV_OSD_DISPLAY_TIME_LIST_CTRL_ID, MMIMTV_DISPLAY_TIME_MAX, FALSE);
    AppendListItemViaId(MTV_OSD_DISPLAY_TIME_LIST_CTRL_ID, TXT_MTV_3S);
    AppendListItemViaId(MTV_OSD_DISPLAY_TIME_LIST_CTRL_ID, TXT_MTV_5S);
    AppendListItemViaId(MTV_OSD_DISPLAY_TIME_LIST_CTRL_ID, TXT_COMM_10SECONDS);
    AppendListItemViaId(MTV_OSD_DISPLAY_TIME_LIST_CTRL_ID, TXT_MTV_15S);
    GUILIST_SetCurItemIndex(MTV_OSD_DISPLAY_TIME_LIST_CTRL_ID, MMIMTV_GetOSDDisplayTime());
    GUILIST_SetSelectedItem(MTV_OSD_DISPLAY_TIME_LIST_CTRL_ID, MMIMTV_GetOSDDisplayTime(), TRUE);
    DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    DisplayTitle(TXT_MTV_OSD_DISPLAY_TIME);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseDisplayTimeSettingScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_OSD_DISPLAY_TIME_LIST_CTRL_ID);
    ClearRect(rect);
    ClearTitle();
}

/*****************************************************************************/
// 	Description : open setting scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenWatchTipSettingScene(uint32 param)
{
    GUI_RECT_T          list_rect = {0};

    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_WATCH_TIP_LIST_WIN_TAB);

    list_rect.left   = MTV_OSD_LEFT;
    list_rect.top    = MTV_OSD_TOP;
    list_rect.right  = MTV_OSD_RIGHT;
    list_rect.bottom = MTV_OSD_BOTTOM;
    GUILIST_SetRect(MTV_WATCH_TIP_LIST_CTRL_ID,&list_rect);

    GUILIST_SetBgColor(MTV_WATCH_TIP_LIST_CTRL_ID, MMIMTV_BACKGROUND_COLOR);
    GUILIST_SetTextFont(MTV_WATCH_TIP_LIST_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMI_GRAY_WHITE_COLOR);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_WATCH_TIP_LIST_CTRL_ID);

    GUILIST_SetMaxItem(MTV_WATCH_TIP_LIST_CTRL_ID, MMIMTV_WATCH_TIME_TIP_MAX, FALSE);
    AppendListItemViaId(MTV_WATCH_TIP_LIST_CTRL_ID, TXT_COMM_10MINUTES);
    AppendListItemViaId(MTV_WATCH_TIP_LIST_CTRL_ID, TXT_MTV_30M);
    AppendListItemViaId(MTV_WATCH_TIP_LIST_CTRL_ID, TXT_MTV_60M);
    GUILIST_SetCurItemIndex(MTV_WATCH_TIP_LIST_CTRL_ID, MMIMTV_GetWatchTimeTip());
    GUILIST_SetSelectedItem(MTV_WATCH_TIP_LIST_CTRL_ID, MMIMTV_GetWatchTimeTip(), TRUE);
    DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    DisplayTitle(TXT_MTV_WATCH_TIME_TIP);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseWatchTipSettingScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_WATCH_TIP_LIST_CTRL_ID);
    ClearRect(rect);
    ClearTitle();
}

/*****************************************************************************/
// 	Description : open setting scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenBroadcastBgPicSettingScene(uint32 param)
{
    GUI_RECT_T          list_rect = {0};

    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_BROADCAST_BG_PIC_LIST_WIN_TAB);

    list_rect.left   = MTV_OSD_LEFT;
    list_rect.top    = MTV_OSD_TOP;
    list_rect.right  = MTV_OSD_RIGHT;
    list_rect.bottom = MTV_OSD_BOTTOM;
    GUILIST_SetRect(MTV_BROADCAST_BG_PIC_LIST_CTRL_ID,&list_rect);

    GUILIST_SetBgColor(MTV_BROADCAST_BG_PIC_LIST_CTRL_ID, MMIMTV_BACKGROUND_COLOR);
    GUILIST_SetTextFont(MTV_BROADCAST_BG_PIC_LIST_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMI_GRAY_WHITE_COLOR);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_BROADCAST_BG_PIC_LIST_CTRL_ID);

    GUILIST_SetMaxItem(MTV_BROADCAST_BG_PIC_LIST_CTRL_ID, MMIMTV_BROADCAST_BG_PIC_MAX, FALSE);
    AppendListItemViaId(MTV_BROADCAST_BG_PIC_LIST_CTRL_ID, TXT_MTV_BG_PIC_1);
    AppendListItemViaId(MTV_BROADCAST_BG_PIC_LIST_CTRL_ID, TXT_MTV_BG_PIC_2);
    AppendListItemViaId(MTV_BROADCAST_BG_PIC_LIST_CTRL_ID, TXT_MTV_BG_PIC_3);
    GUILIST_SetCurItemIndex(MTV_BROADCAST_BG_PIC_LIST_CTRL_ID, MMIMTV_GetBroadcastBG());
    GUILIST_SetSelectedItem(MTV_BROADCAST_BG_PIC_LIST_CTRL_ID, MMIMTV_GetBroadcastBG(), TRUE);
    DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    DisplayTitle(TXT_MTV_BROADCAST_BG_PIC);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseBroadcastBgPicSettingScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_BROADCAST_BG_PIC_LIST_CTRL_ID);
    ClearRect(rect);
    ClearTitle();
}

/*****************************************************************************/
// 	Description : add a item to eb list
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void AppendEBListItem(
                            wchar *content0, 
                            wchar *content1, 
                            BOOLEAN is_read,
							uint32	eb_grade
                            )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    uint32 len = MMIAPICOM_Wstrlen(content1);
    
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ICON;
    item_t.item_data_ptr = &item_data;   
    
	//title icon
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    if(is_read)
    {
        item_data.item_content[0].item_data.image_id = IMAGE_SMS_READ_SMS_ICON;
    }
    else
    {
        item_data.item_content[0].item_data.image_id = IMAGE_SMS_NOT_READ_SMS_ICON;
    }

	//eb grade icon
	item_data.item_content[2].item_data_type	 = GUIITEM_DATA_IMAGE_ID;
	item_data.item_content[2].item_data.image_id = s_eb_grade_icon[eb_grade - 1];

	//eb info
	item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    if(len > GUILIST_STRING_MAX_NUM)
    {
        item_data.item_content[1].item_data.text_buffer.wstr_len = GUILIST_STRING_MAX_NUM;
    }  
    else
    {
        item_data.item_content[1].item_data.text_buffer.wstr_len = len;
    }
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = content1;
	
	//time info
    item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[3].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(content0);
	item_data.item_content[3].item_data.text_buffer.wstr_ptr = content0;
	
    GUILIST_AppendItem(MTV_ID_EB_LIST_CTRL_ID, &item_t);   
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenEBListScene(uint32 param)
{
    wchar wline1[MMIMTV_EBLINE1_BUFFER_MAX_LEN] = {0};
    wchar wline2[MMIMTV_EBLINE2_BUFFER_MAX_LEN] = {0};
    char  cline1[MMIMTV_EBLINE1_BUFFER_MAX_LEN] = {0};
    uint32 list_num = MMIMTV_GetEBListNumber();
    uint32 index = 0;
    MMIMTV_EB_NODE_T* cursor = PNULL;
    MMI_TM_T time = {0};
    MMIACC_RTC_DATE_T rtc_date = {0};
    MMIACC_RTC_TIME_T rtc_time = {0};
    GUI_RECT_T          list_rect = {0};

    //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_OpenEBListScene eb_list_number = %d  param = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_3542_112_2_18_2_40_46_869,(uint8*)"dd", list_num, param);

    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_EB_LIST_WIN_TAB);

    list_rect.left   = MTV_OSD_LEFT;
    list_rect.top    = MTV_OSD_TOP;
    list_rect.right  = MTV_OSD_RIGHT;
    list_rect.bottom = MTV_OSD_BOTTOM;
    GUILIST_SetRect(MTV_ID_EB_LIST_CTRL_ID,&list_rect);

    GUILIST_SetBgColor(MTV_ID_EB_LIST_CTRL_ID, MMIMTV_BACKGROUND_COLOR);
    GUILIST_SetTextFont(MTV_ID_EB_LIST_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMI_GRAY_WHITE_COLOR);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_ID_EB_LIST_CTRL_ID);
    GUILIST_SetMaxItem(MTV_ID_EB_LIST_CTRL_ID, list_num, FALSE);
    //特殊处理, 响应左右按键
    GUILIST_SetHandleLeftRight( MTV_ID_EB_LIST_CTRL_ID, TRUE );
    
    if(0 == list_num)
    {
        MMI_STRING_T text_str = {0};

        GUILIST_SetMaxItem(MTV_ID_EB_LIST_CTRL_ID, 1, FALSE);
        MMI_GetLabelTextByLang(TXT_EMPTY, &text_str);
        AppendListItem(MTV_ID_EB_LIST_CTRL_ID, text_str.wstr_ptr);
        DisplaySoftKey(TXT_NULL, TXT_NULL, STXT_RETURN);
    }
    else
    {
        while (list_num --)
        {
            SCI_MEMSET(wline1, 0, sizeof(wline1));
            SCI_MEMSET(wline2, 0, sizeof(wline2));
            SCI_MEMSET(cline1, 0, sizeof(cline1));
            cursor = MMIMTV_GetEBNodeViaIndex(index);
            time = MMIAPICOM_Second2Tm(cursor->eb_save_time);
            sprintf((char*)rtc_time, "%02d:%02d", time.tm_hour, time.tm_min);
            sprintf((char*)rtc_date, "%02d/%02d", time.tm_mon, time.tm_mday);
            sprintf(cline1, "%s  %s", (char*)rtc_date, (char*)rtc_time);
            MMIAPICOM_StrToWstr((uint8*)cline1, wline1);
            MMIAPICOM_Wstrcpy(wline2, cursor->eb_type);
            MMIAPICOM_Wstrcpy(wline2 + MMIAPICOM_Wstrlen(cursor->eb_type), wline1);
            AppendEBListItem(wline2, cursor->eb_content, cursor->eb_is_read, cursor->eb_grade);
            index ++;
        }
        GUILIST_SetCurItemIndex(MTV_ID_EB_LIST_CTRL_ID, param);
        DisplaySoftKey(STXT_OPTION, TXT_NULL, STXT_RETURN);
    }
    DisplayTitle(TXT_MTV_EMERGENCY_BROADCASTING);
	
	//display eb list cur index and total number in title
    
	if(MMIMTV_GetEBListNumber() > 0)
	{
		MMIMTV_DisplayEBCountInfo(param + 1, MMIMTV_GetEBListNumber());
	}

}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseEBListScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_ID_EB_LIST_CTRL_ID);
    ClearRect(rect);
    ClearTitle();
}

/*****************************************************************************/
//  Description : open record list pop menu callback
//  Global resource dependence : 
//  Author: 
//  Note: 资源管理器窗口左键callback函数, 弹出选项菜单
/*****************************************************************************/
LOCAL void OpenPopmenuCallBack(MMIFMM_DATA_INFO_T *list_data_ptr,uint32 index)
{
    //callback获取文件信息
    MMIMTV_FMM_CALLBACK_INFO_T* fmm_callback_info_ptr = (MMIMTV_FMM_CALLBACK_INFO_T*)SCI_ALLOC_APP(sizeof(MMIMTV_FMM_CALLBACK_INFO_T));
    if(PNULL == fmm_callback_info_ptr)
    {
        return;
    }
    
    SCI_MEMSET(fmm_callback_info_ptr, 0, sizeof(MMIMTV_FMM_CALLBACK_INFO_T));      
    fmm_callback_info_ptr->list_data_ptr = list_data_ptr;
    fmm_callback_info_ptr->index = index;
    
    //open popmenu
    MMK_CreateWin((uint32 *)MMIMTV_POPMENU_WIN_TAB,(ADD_DATA)fmm_callback_info_ptr);
}

/*****************************************************************************/
// 	Description : handle record list option window msg
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRecordListOptWinMsg(
                                              MMI_WIN_ID_T win_id,
                                              MMI_MESSAGE_ID_E msg_id,        
                                              DPARAM param
                                              )
{
    MMI_RESULT_E            result   = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T     group_id = 0; 
    MMI_MENU_ID_T           menu_id  = 0;
    MMIMTV_FMM_CALLBACK_INFO_T* fmm_callback_info_ptr = PNULL;
    
    fmm_callback_info_ptr = (MMIMTV_FMM_CALLBACK_INFO_T*)MMK_GetWinAddDataPtr(win_id);
    if(PNULL == fmm_callback_info_ptr)
    {
        return result;
    }
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(0 == fmm_callback_info_ptr->list_data_ptr->file_num)
        {
            //无.mtv录制文件时灰化菜单选项
            GUIMENU_SetItemGrayed(MTV_RECORD_LIST_OPT_CTRL_ID,MMIMTV_RECORD_LIST_OPTION_MENU,MTV_ID_PLAY_RECORD_MENU,TRUE);
            GUIMENU_SetItemGrayed(MTV_RECORD_LIST_OPT_CTRL_ID,MMIMTV_RECORD_LIST_OPTION_MENU,MTV_ID_DELETE_RECORD_MENU,TRUE);
            GUIMENU_SetItemGrayed(MTV_RECORD_LIST_OPT_CTRL_ID,MMIMTV_RECORD_LIST_OPTION_MENU,MTV_ID_DELETE_ALL_RECORD_MENU,TRUE);
            GUIMENU_SetItemGrayed(MTV_RECORD_LIST_OPT_CTRL_ID,MMIMTV_RECORD_LIST_OPTION_MENU,MTV_ID_RENAME_RECORD_MENU,TRUE);
            GUIMENU_SetItemGrayed(MTV_RECORD_LIST_OPT_CTRL_ID,MMIMTV_RECORD_LIST_OPTION_MENU,MTV_ID_RECORD_DETAIL_MENU,TRUE);
        }
        MMK_SetAtvCtrl(win_id, MTV_RECORD_LIST_OPT_CTRL_ID);
        break;
        
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        //获得菜单项
        GUIMENU_GetId(MTV_RECORD_LIST_OPT_CTRL_ID, &group_id, &menu_id);
        switch(menu_id)
        {
        case MTV_ID_PLAY_RECORD_MENU:
            //播放录制文件
            MMIMTV_PlayRecordCallBack(fmm_callback_info_ptr->list_data_ptr, fmm_callback_info_ptr->index);
            MMK_CloseWin(win_id);
            break;
            
        case MTV_ID_DELETE_RECORD_MENU:
            //删除
            MMIAPIFMM_HandleMenuOpt(MMIFMM_DELETE_OPT);
            MMK_CloseWin(win_id);
            break;
            
        case MTV_ID_DELETE_ALL_RECORD_MENU:
            //全部删除
            MMIAPIFMM_HandleMenuOpt(MMIFMM_MARK_ALL_OPT);   //标记全部
            MMIAPIFMM_HandleMenuOpt(MMIFMM_DELETE_OPT);
            MMK_CloseWin(win_id);
            break;
            
        case MTV_ID_RENAME_RECORD_MENU:
            //重命名
            MMIAPIFMM_HandleMenuOpt(MMIFMM_RENAME_OPT);
            MMK_CloseWin(win_id);
            break;
            
        case MTV_ID_RECORD_DETAIL_MENU:
            //详情
            MMIAPIFMM_HandleMenuOpt(MMIFMM_DETAIL_OPT);
            MMK_CloseWin(win_id);
            break;
            
        default:
            break;
        }
        break;
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        case MSG_CLOSE_WINDOW:
            //if(PNULL != fmm_callback_info_ptr)
            //{
                SCI_FREE(fmm_callback_info_ptr);
            //}
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : close record list scene callback
//  Global resource dependence : 
//  Author: 
//  Note: 资源管理器窗口右键键callback函数，直接关闭资源管理器窗口
/*****************************************************************************/
LOCAL void CloseRecordListCallBack(MMIFMM_DATA_INFO_T *list_data_ptr,uint32 index)
{
    //SCI_TRACE_LOW:"[MMIMTV]: CloseRecordListCallBack"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_3741_112_2_18_2_40_46_870,(uint8*)"");
    
    MMIMTV_SetFocusOSDScene(OSD_MAIN_MENU);
    MMIMTV_SetOSDRestoreParam(MMIMTV_RECORD_LIST);

    //close FMM ExplorerWin via win_id directly
    MMK_CloseWin(MMIFMM_MAIN_WIN_ID);
}

/*****************************************************************************/
// 	Description : open record list scene
//	Global resource dependence :
//  Author:
//	Note: 直接使用资源管理器打开列表窗口，MTV不维护录像列表
/*****************************************************************************/
PUBLIC void MMIMTV_OpenRecordListScene(void)
{
    wchar path[] = {'E', ':', '\\', 'm', 't', 'v', '\\', 'v', 'i', 'd', 'e', 'o',0};
    MMIFMM_OPENWIN_INFO_T openwin_info = {0};
    
    if(MMI_DEVICE_NUM > MMIMTV_GetRecordStore())
    {

        MMIAPICOM_Wstrncpy(path, MMIAPIFMM_GetDevicePath(MMIMTV_GetRecordStore()), MMIFILE_DEVICE_LEN);
    }
    
    openwin_info.path_ptr = path;
    openwin_info.path_len = MMIAPICOM_Wstrlen(path);
    openwin_info.filename = PNULL;
    openwin_info.file_len = 0;
    openwin_info.is_need_display  = TRUE;
    openwin_info.find_suffix_type = MMIFMM_MOVIE_MTV;   //仅查看.mtv后缀
    openwin_info.sk_text_id.leftsoft_id   = STXT_OPTION;
    openwin_info.sk_text_id.middlesoft_id = TXT_NULL;
    openwin_info.sk_text_id.rightsoft_id  = STXT_RETURN;
    openwin_info.callback.callback_ok     = OpenPopmenuCallBack;        //选项菜单   
    openwin_info.callback.callback_web    = MMIMTV_PlayRecordCallBack;  //播放录制文件
    openwin_info.callback.callback_can    = CloseRecordListCallBack;    //关闭FMM窗口
    MMIMTV_SetOSDRestoreParam(MMIMTV_RECORD_LIST);  //返回OSD_MAIN_MENU焦点停留在Record List
    
    MMIAPIFMM_OpenExplorerWin(&openwin_info);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenEBListOptScene(uint32 param)
{
    GUI_RECT_T  menu_rect = {0};

    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_EB_LIST_OPT_WIN_TAB);
    //set menu rect
    menu_rect.left   = MTV_OSD_LEFT;
    menu_rect.top    = MTV_OSD_TOP;
    menu_rect.right  = MTV_OSD_RIGHT;
    menu_rect.bottom = MTV_OSD_BOTTOM;
    GUIMENU_SetRect(MTV_ID_EB_OPTION_CTRL_MENU,&menu_rect);
    GUIMENU_SetMenuBgColor(MTV_ID_EB_OPTION_CTRL_MENU, MMIMTV_BACKGROUND_COLOR);
    GUIMENU_SetMenuFontColor(MTV_ID_EB_OPTION_CTRL_MENU, MMI_GRAY_WHITE_COLOR);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_ID_EB_OPTION_CTRL_MENU);
    DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    DisplayTitle(STXT_OPTION);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseEBListOptScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_ID_EB_OPTION_CTRL_MENU);
    ClearRect(rect);
    ClearTitle();
}

/*****************************************************************************/
// 	Description : add eb content item
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void AppendEBContentItem(uint32 eb_index)
{
	MMI_CTRL_ID_T ctrl_id = MTV_ID_EB_RICHTEXT_CTRL_ID;
	GUIRICHTEXT_ITEM_T  richtext_item = {0};
	uint16	index = 0;
	uint16	eb_grade = MMIMTV_GetEBNodeViaIndex(eb_index)->eb_grade;

	richtext_item.img_type = GUIRICHTEXT_IMAGE_RES;
	richtext_item.img_data.res.id = s_eb_grade_icon[eb_grade - 1];
	richtext_item.img_frame_type  = GUIRICHTEXT_FRAME_NORMAL;
	
	richtext_item.text_type = GUIRICHTEXT_TEXT_BUF;
	richtext_item.text_data.buf.str_ptr = MMIMTV_GetEBNodeViaIndex(eb_index)->eb_content;
	richtext_item.text_data.buf.len		= MMIAPICOM_Wstrlen(richtext_item.text_data.buf.str_ptr);
	
	richtext_item.align_type = GUIRICHTEXT_ALIGN_DOWN;
    GUIRICHTEXT_SetBgColor(ctrl_id,MMIMTV_BACKGROUND_COLOR);
	GUIRICHTEXT_SetImgTxtSpace(ctrl_id, 6);
	GUIRICHTEXT_SetLineSpace(ctrl_id, 2);
	GUIRICHTEXT_AddItem(ctrl_id, &richtext_item, &index);
	GUIRICHTEXT_Update(ctrl_id);
}

/*****************************************************************************/
// 	Description : open EB detail scene via index
//	Global resource dependence :
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_OpenEBListContentScene(uint32 param)
{
	MMI_CTRL_ID_T ctrl_id = MTV_ID_EB_RICHTEXT_CTRL_ID;
    GUI_RECT_T  richtext_rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_OpenEBListContentScene, param = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_3853_112_2_18_2_40_47_871,(uint8*)"d", param);
    
    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_EB_LIST_CONTENT_WIN_TAB);  
    GUIRICHTEXT_SetRect(ctrl_id,richtext_rect);

    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, ctrl_id);

	AppendEBContentItem(param);
    
    //本条EB消息已读
    MMIMTV_GetEBNodeViaIndex(param)->eb_is_read = TRUE;
    
    DisplaySoftKey(TXT_NULL, TXT_NULL, STXT_RETURN);
    DisplayTitle(TXT_MBBMS_EB_DETAIL);
    if(MMIMTV_GetEBListNumber() > 1)
    {
        MMIMTV_OSD_DISPLAY_PTR->eb_left_arrow.left	 = MMIMTV_LEFT_ARROW_LEFT;
        MMIMTV_OSD_DISPLAY_PTR->eb_left_arrow.top	 = MMIMTV_LEFT_ARROW_TOP;
        MMIMTV_OSD_DISPLAY_PTR->eb_left_arrow.right	 = MMIMTV_LEFT_ARROW_RIGHT;
        MMIMTV_OSD_DISPLAY_PTR->eb_left_arrow.bottom = MMIMTV_LEFT_ARROW_BOTTOM;
        
        MMIMTV_OSD_DISPLAY_PTR->eb_right_arrow.left	  = MMIMTV_RIGHT_ARROW_LEFT;
        MMIMTV_OSD_DISPLAY_PTR->eb_right_arrow.top	  = MMIMTV_RIGHT_ARROW_TOP;
        MMIMTV_OSD_DISPLAY_PTR->eb_right_arrow.right  = MMIMTV_RIGHT_ARROW_RIGHT;
        MMIMTV_OSD_DISPLAY_PTR->eb_right_arrow.bottom = MMIMTV_RIGHT_ARROW_BOTTOM;
        
        DisplayPicture(MMIMTV_OSD_DISPLAY_PTR->eb_left_arrow.left, MMIMTV_OSD_DISPLAY_PTR->eb_left_arrow.top, IMAGE_LIST_TITLE_ARROW_LEFT_UP);
        DisplayPicture(MMIMTV_OSD_DISPLAY_PTR->eb_right_arrow.left, MMIMTV_OSD_DISPLAY_PTR->eb_right_arrow.top, IMAGE_LIST_TITLE_ARROW_RIGHT_UP); 
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseEBListContentScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};
    GUI_RECT_T  *eb_left_arrow_ptr = &(MMIMTV_OSD_DISPLAY_PTR->eb_left_arrow);
    GUI_RECT_T  *eb_right_arrow_ptr = &(MMIMTV_OSD_DISPLAY_PTR->eb_right_arrow);

    MMK_DestroyControl(MTV_ID_EB_RICHTEXT_CTRL_ID);
    ClearRect(rect);
    ClearTitle();
    SCI_MEMSET(eb_left_arrow_ptr, 0, sizeof(GUI_RECT_T));
    SCI_MEMSET(eb_right_arrow_ptr, 0, sizeof(GUI_RECT_T));
}


/*****************************************************************************/
// 	Description : 显示频道号
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DisplayServiceNumber(void)
{
    MMI_STRING_T    text_str = {0};
    char            c_service[MMIMTV_SERVICE_NUMBER_MAX_DIGITS +1] = {0};
    wchar           w_service[MMIMTV_SERVICE_NUMBER_MAX_DIGITS +1] = {0};
    uint32          index = 0;
    GUI_RECT_T      text_rect = {0};
    uint16		    text_width = 0;
    
    //get service number index
    index = (uint32)MMIMBBMS_GetPlayingServiceIndex();
    
    //serviec num string
    if(index < 10)
    {
        sprintf(c_service, "CH0%d", index);
    }
    else
    {
        sprintf(c_service, "CH%d", index);
    }
    MMIAPICOM_StrToWstr((uint8*)c_service, w_service);
    text_str.wstr_ptr = w_service;
    text_str.wstr_len = MMIAPICOM_Wstrlen(w_service);
    
    //get string pixel width
    if(text_str.wstr_len > 0)
    {
        text_width = GUI_CalculateStringPiexlNum(text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT, 1);
    }
    text_rect.top       = MMIMTV_SERVICE_TEXT_TOP;
    text_rect.bottom    = MMIMTV_SERVICE_TEXT_BOTTOM;
    text_rect.left      = MMIMTV_SERVICE_TEXT_LEFT;
    text_rect.right     = MMIMTV_SERVICE_TEXT_LEFT + text_width;

    MMIMTV_OSD_DISPLAY_PTR->service_left_arrow.left     = MMIMTV_SERVICE_LEFT_ARROW_LEFT;
    MMIMTV_OSD_DISPLAY_PTR->service_left_arrow.top      = MMIMTV_SERVICE_LEFT_ARROW_TOP;
    MMIMTV_OSD_DISPLAY_PTR->service_left_arrow.right    = MMIMTV_SERVICE_LEFT_ARROW_RIGHT;
    MMIMTV_OSD_DISPLAY_PTR->service_left_arrow.bottom   = MMIMTV_SERVICE_LEFT_ARROW_BOTTOM;

    MMIMTV_OSD_DISPLAY_PTR->service_right_arrow.left    = text_rect.right;
    MMIMTV_OSD_DISPLAY_PTR->service_right_arrow.top     = MMIMTV_SERVICE_RIGHT_ARROW_TOP;
    MMIMTV_OSD_DISPLAY_PTR->service_right_arrow.right   = text_rect.right + (MMIMTV_SERVICE_LEFT_ARROW_RIGHT - MMIMTV_SERVICE_LEFT_ARROW_LEFT);
    MMIMTV_OSD_DISPLAY_PTR->service_right_arrow.bottom  = MMIMTV_SERVICE_RIGHT_ARROW_BOTTOM;

    DisplayPicture(MMIMTV_OSD_DISPLAY_PTR->service_left_arrow.left, MMIMTV_OSD_DISPLAY_PTR->service_left_arrow.top, IMAGE_LIST_TITLE_ARROW_LEFT_UP);
    DisplayPicture(MMIMTV_OSD_DISPLAY_PTR->service_right_arrow.left, MMIMTV_OSD_DISPLAY_PTR->service_right_arrow.top, IMAGE_LIST_TITLE_ARROW_RIGHT_UP);

    ClearRect(text_rect);
    DisplayEffectString(text_rect, text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT, FALSE);
    CreateDisappearTimer(MMIMTV_SYSTIME_SERVICE_NUMBER);
    MMIMTV_OSD_DISPLAY_PTR->servicetime_count = 0;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ClearServiceNumber(void)
{
    GUI_RECT_T      rect = {0};
    GUI_RECT_T      *left_rect_ptr = &(MMIMTV_OSD_DISPLAY_PTR->service_left_arrow);
    GUI_RECT_T      *right_rect_ptr = &(MMIMTV_OSD_DISPLAY_PTR->service_right_arrow);
    
    rect.left   = MMIMTV_OSD_DISPLAY_PTR->service_left_arrow.left;
    rect.top    = MMIMTV_SERVICE_LEFT_ARROW_TOP;
    rect.right  = MMIMTV_OSD_DISPLAY_PTR->service_right_arrow.right;
    rect.bottom = MMIMTV_SERVICE_TEXT_BOTTOM;
    
    ClearRect(rect);
    SCI_MEMSET(left_rect_ptr, 0, sizeof(GUI_RECT_T));
    SCI_MEMSET(right_rect_ptr, 0, sizeof(GUI_RECT_T));
    MMIMTV_StopSysTimer(MMIMTV_SYSTIME_SERVICE_NUMBER);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_GetChangeServiceArrowRect(GUI_RECT_T *left_rect,
                                             GUI_RECT_T *right_rect)
{
    SCI_MEMCPY(left_rect, &(MMIMTV_OSD_DISPLAY_PTR->service_left_arrow), sizeof(GUI_RECT_T));
    SCI_MEMCPY(right_rect, &(MMIMTV_OSD_DISPLAY_PTR->service_right_arrow), sizeof(GUI_RECT_T));
}

/*****************************************************************************/
// 	Description : display input num when num switch
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DisplayInputNum(uint32 num)
{
    MMI_STRING_T    text_str = {0};
    char            c_service[MMIMTV_SERVICE_NUMBER_MAX_DIGITS +1] = {0};
    wchar           w_service[MMIMTV_SERVICE_NUMBER_MAX_DIGITS +1] = {0};
    GUI_RECT_T      rect = {0};
    
    rect.left   = MMIMTV_SERVICE_LEFT_ARROW_LEFT;
    rect.top    = MMIMTV_SERVICE_TEXT_TOP;
    rect.right  = MMIMTV_SERVICE_TEXT_RIGHT;
    rect.bottom = MMIMTV_SERVICE_TEXT_BOTTOM;
    
    sprintf(c_service, "%d", num);
    MMIAPICOM_StrToWstr((uint8*)c_service, w_service);
    text_str.wstr_ptr = w_service;
    text_str.wstr_len = MMIAPICOM_Wstrlen(w_service);
    
    ClearRect(rect);
    DisplayEffectString(rect, text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT, FALSE);
}

/*****************************************************************************/
// 	Description : open help scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenHelpScene(uint32 param)
{
    GUI_BG_T        bg = {0};
    MMI_STRING_T text_str = {0};
    GUI_RECT_T      text_rect = {0};

    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_HELP_WIN_TAB);  
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_HELP_TEXT_CTRL_ID);

    text_rect.left   = MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM;
    text_rect.top    = MTV_OSD_TOP;
    text_rect.right  = MTV_OSD_RIGHT;
    text_rect.bottom = MTV_OSD_BOTTOM;
    GUITEXT_SetRect(MTV_HELP_TEXT_CTRL_ID,&text_rect);

    bg.bg_type = GUI_BG_COLOR;
    bg.color = MMIMTV_BACKGROUND_COLOR;
    GUITEXT_SetBg(MTV_HELP_TEXT_CTRL_ID, &bg);
    MMI_GetLabelTextByLang(TXT_MTV_HELP_CONTENT, &text_str);
    GUITEXT_SetString(MTV_HELP_TEXT_CTRL_ID, text_str.wstr_ptr, text_str.wstr_len, FALSE);
    DisplaySoftKey(TXT_NULL, TXT_NULL, STXT_RETURN);
    DisplayTitle(TXT_HELP);
}

/*****************************************************************************/
// 	Description : open help scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseHelpScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_HELP_TEXT_CTRL_ID);
    ClearRect(rect);
    ClearTitle();
}

/*****************************************************************************/
// 	Description : open help scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenBookRecordScene(uint32 param)
{
    MMI_STRING_T text_str = {0};
    MMI_TM_T time = {0};
    char date[MMIMTV_EBLINE1_BUFFER_MAX_LEN] = {0};
    //char record_long[MMIMTV_LONG_TIME_MAX_DIGITS] = {0};
    wchar display[MMIMTV_EBLINE1_BUFFER_MAX_LEN] = {0};
    uint16 num = 0;   
    GUI_RECT_T          list_rect = {0};
    MMIMTV_SAVE_SERVICE_INFO_T*  service = MMIMTV_GetServiceNodeViaID(MMIMTV_GetBookRecordNetID(), MMIMTV_GetBookRecordServiceID());

    if(MMIMTV_OSD_DISPLAY_PTR->is_scene_close)
    {
        MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_BOOK_RECORD_SETTING_WIN_TAB);
        list_rect.left   = MTV_OSD_LEFT;
        list_rect.top    = MTV_OSD_TOP;
        list_rect.right  = MTV_OSD_RIGHT;
        list_rect.bottom = MTV_OSD_BOTTOM;
        GUILIST_SetRect(MTV_BOOK_RECORD_LIST_CTRL_ID,&list_rect);

        GUILIST_SetBgColor(MTV_BOOK_RECORD_LIST_CTRL_ID, MMIMTV_BACKGROUND_COLOR);
        GUILIST_SetTextFont(MTV_BOOK_RECORD_LIST_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMI_GRAY_WHITE_COLOR);
        MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_BOOK_RECORD_LIST_CTRL_ID);
        GUILIST_SetMaxItem(MTV_BOOK_RECORD_LIST_CTRL_ID, 3, FALSE);
        
        AppendListItemForBookRecordViaId(MTV_BOOK_RECORD_LIST_CTRL_ID, TXT_MTV_RECORD_FILE_NAME, MMIMTV_GetRecordFileName());
        
        if(PNULL == service)
        {
            AppendListItemForBookRecordViaId(MTV_BOOK_RECORD_LIST_CTRL_ID, TXT_MTV_TV_PROGRAM, PNULL);
        }
        else
        {
            AppendListItemForBookRecordViaId(MTV_BOOK_RECORD_LIST_CTRL_ID, TXT_MTV_TV_PROGRAM, service->service_name);
        }
        time = MMIAPICOM_Second2Tm(MMIMTV_GetBookRecordStartTime());
        num = MMIMTV_GetBookRecordLong();
        if(num > MMIMTV_BOOK_RECORD_FILE_LONG_MAX)
        {
            return;
        }
        num = sprintf(date, "%4d-%02d-%02d %02d:%02d %d", time.tm_year, time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, num);
        MMIAPICOM_StrToWstr((uint8*)date, display);    
        MMI_GetLabelTextByLang(TXT_COMM_MINUTES, &text_str);
        MMIAPICOM_Wstrcpy(display + num, text_str.wstr_ptr);
        *(display + num + text_str.wstr_len) = 0;
        AppendListItemForBookRecordViaId(MTV_BOOK_RECORD_LIST_CTRL_ID, TXT_MTV_RECORD_TIME, display);
        MMIMTV_OSD_DISPLAY_PTR->is_scene_close = FALSE;
    }
    else
    {
        MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_BOOK_RECORD_LIST_CTRL_ID);
    }

    DisplaySoftKey(STXT_MAIN_SETTING, TXT_NULL, STXT_RETURN);
    DisplayTitle(TXT_MTV_BOOKING_RECORD);
}

/*****************************************************************************/
// 	Description : open help scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseBookRecordScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_BOOK_RECORD_LIST_CTRL_ID);
    ClearRect(rect);
    ClearTitle();
    MMIMTV_OSD_DISPLAY_PTR->is_scene_close = TRUE;
}

/*****************************************************************************/
// 	Description : open help scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void MMIMTV_OpenEditRecordFileNameScene(uint32 param)
{
    MMK_CreateWin((uint32*)MMIMTV_EDIT_RECORD_FILE_NAME_WIN_TAB, (ADD_DATA)PNULL);
}

/*****************************************************************************/
// 	Description : handle MTV window msg
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditNameWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E	msg_id,
                                        DPARAM              param
                                        )
{
	MMI_RESULT_E      result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T     ctrl_id = MTV_EDIT_RECORD_FILE_NAME_TEXT_CTRL_ID;
    MMI_STRING_T  string = {0};

	switch( msg_id ) 
	{
	case MSG_OPEN_WINDOW:
		MMK_SetAtvCtrl(win_id, ctrl_id);
		break;

    case MSG_CTL_OK:
	case MSG_CTL_MIDSK:
        GUIEDIT_GetString(ctrl_id, &string);
        MMIMTV_SetBookRecordFileName(string.wstr_ptr);
        MMK_CloseWin(win_id);
        break;
		
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		MMK_CloseWin(win_id);
		break;
		
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return result;
}

/*****************************************************************************/
// 	Description : open help scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenEditRecordFileTimeScene(uint32 param)
{
    MMI_STRING_T text_str = {0};
    MMI_TM_T time = {0};
    MMIACC_RTC_DATE_T rtc_date = {0};
    MMIACC_RTC_TIME_T rtc_time = {0};
    char record_long[MMIMTV_LONG_TIME_MAX_DIGITS] = {0};
    wchar display[MMIMTV_TEMP_STRING_BUFFER_MAX_LEN] = {0};
    uint16 num = 0;   
    GUI_RECT_T  list_rect = {0};
    
    if(MMIMTV_OSD_DISPLAY_PTR->is_scene_close)
    {
        MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_EDIT_RECORD_FILE_TIME_WIN_TAB);

        list_rect.left   = MTV_OSD_LEFT;
        list_rect.top    = MTV_OSD_TOP;
        list_rect.right  = MTV_OSD_RIGHT;
        list_rect.bottom = MTV_OSD_BOTTOM;
        GUILIST_SetRect(MTV_ID_RECORD_FILE_TIME_CTRL_ID,&list_rect);

        GUILIST_SetBgColor(MTV_ID_RECORD_FILE_TIME_CTRL_ID, MMIMTV_BACKGROUND_COLOR);
        GUILIST_SetTextFont(MTV_ID_RECORD_FILE_TIME_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMI_GRAY_WHITE_COLOR);
        MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_ID_RECORD_FILE_TIME_CTRL_ID);
        GUILIST_SetMaxItem(MTV_ID_RECORD_FILE_TIME_CTRL_ID, 3, FALSE);
        
        time = MMIAPICOM_Second2Tm(MMIMTV_GetBookRecordStartTime());
        sprintf((char*)rtc_time, "%02d:%02d", time.tm_hour, time.tm_min);
        sprintf((char*)rtc_date, "%4d-%02d-%02d", time.tm_year, time.tm_mon, time.tm_mday);
        num = MMIMTV_GetBookRecordLong();
        if(num > MMIMTV_BOOK_RECORD_FILE_LONG_MAX)
        {
            return;
        }
        num = sprintf(record_long, "%d", num);
        
        MMIAPICOM_StrToWstr((uint8*)rtc_date, display);
        AppendListItemForBookRecordViaId(MTV_ID_RECORD_FILE_TIME_CTRL_ID, TXT_DATE, display);
        
        MMIAPICOM_StrToWstr((uint8*)rtc_time, display);
        AppendListItemForBookRecordViaId(MTV_ID_RECORD_FILE_TIME_CTRL_ID, TXT_TIME, display);
        
        MMIAPICOM_StrToWstr((uint8*)record_long, display);
        MMI_GetLabelTextByLang(TXT_COMM_MINUTES, &text_str);
        MMIAPICOM_Wstrcpy(display + num, text_str.wstr_ptr);
        *(display + num + text_str.wstr_len) = 0;
        AppendListItemForBookRecordViaId(MTV_ID_RECORD_FILE_TIME_CTRL_ID, TXT_MTV_LONG_TIME, display);
        MMIMTV_OSD_DISPLAY_PTR->is_scene_close = FALSE;
    }
    else
    {
        MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_ID_RECORD_FILE_TIME_CTRL_ID);
    }

    DisplayTitle(TXT_MTV_RECORD_TIME);
    DisplaySoftKey(STXT_MAIN_SETTING, TXT_NULL, STXT_RETURN);
}

/*****************************************************************************/
// 	Description : open help scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseEditRecordFileTimeScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_ID_RECORD_FILE_TIME_CTRL_ID);
    ClearRect(rect);
    ClearTitle();
    MMIMTV_OSD_DISPLAY_PTR->is_scene_close = TRUE;
}

/*****************************************************************************/
// 	Description : open help scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void MMIMTV_OpenInputDateScene(uint32 param)
{
    MMK_CreateWin((uint32*)MMIMTV_EDIT_RECORD_FILE_DATE_WIN_TAB, (ADD_DATA)PNULL);
}

/*****************************************************************************/
// 	Description : open help scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void MMIMTV_OpenInputTimeScene(uint32 param)
{
    MMK_CreateWin((uint32*)MMIMTV_EDIT_RECORD_FILE_TIME2_WIN_TAB, (ADD_DATA)PNULL);
}

/*****************************************************************************/
// 	Description : open help scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void MMIMTV_OpenInputRecordLongScene(uint32 param)
{
    MMK_CreateWin((uint32*)MMIMTV_EDIT_RECORD_FILE_LONG_WIN_TAB, (ADD_DATA)PNULL);
}

/*****************************************************************************/
// 	Description : handle MTV window msg
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditDateWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E	msg_id,
                                        DPARAM              param
                                        )
{
	MMI_RESULT_E      result = MMI_RESULT_TRUE;
    static MMI_TM_T time = {0};

	switch( msg_id ) 
	{
	case MSG_OPEN_WINDOW:
        time = MMIAPICOM_Second2Tm(MMIMTV_GetBookRecordStartTime());
		MMK_SetAtvCtrl(win_id, MTV_EDIT_RECORD_FILE_CALENDAR_TEXT_CTRL_ID);
		GUIEDIT_SetDate(MTV_EDIT_RECORD_FILE_CALENDAR_TEXT_CTRL_ID,time.tm_year,
            time.tm_mon,
            time.tm_mday);
		break;

    case MSG_APP_OK:
    case MSG_CTL_OK: 
		GUIEDIT_GetDate(MTV_EDIT_RECORD_FILE_CALENDAR_TEXT_CTRL_ID, (uint16*)&(time.tm_year), &(time.tm_mon), &(time.tm_mday));
        MMIMTV_SetBookRecordStartTime(MMIAPICOM_Tm2Second(time.tm_sec, time.tm_min, time.tm_hour, time.tm_mday, 
            time.tm_mon, time.tm_year));
        MMK_CloseWin(win_id);
        break;

    case MSG_FULL_PAINT:
        MMIMTV_ClearScreen();
        break;

	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		MMK_CloseWin(win_id);
		break;
        
    case MSG_CLOSE_WINDOW:
        break;
		
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return result;
}

/*****************************************************************************/
// 	Description : handle MTV window msg
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditInputTimeWinMsg(
                                             MMI_WIN_ID_T        win_id,
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM              param
                                             )
{
    MMI_RESULT_E      result = MMI_RESULT_TRUE;
    static MMI_TM_T time = {0};
    
    switch( msg_id ) 
    {
    case MSG_OPEN_WINDOW:
        time = MMIAPICOM_Second2Tm(MMIMTV_GetBookRecordStartTime());
		MMK_SetAtvCtrl(win_id, MTV_EDIT_RECORD_FILE_TIME_TEXT_CTRL_ID);
        GUIEDIT_SetTime(MTV_EDIT_RECORD_FILE_TIME_TEXT_CTRL_ID, time.tm_hour,
            time.tm_min,0);
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:    
		GUIEDIT_GetTime(MTV_EDIT_RECORD_FILE_TIME_TEXT_CTRL_ID, &(time.tm_hour), &(time.tm_min),PNULL);
        MMIMTV_SetBookRecordStartTime(MMIAPICOM_Tm2Second(time.tm_sec, time.tm_min, time.tm_hour, time.tm_mday, 
            time.tm_mon, time.tm_year));
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_FULL_PAINT:
        MMIMTV_ClearScreen();
        break;
        
    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
// 	Description : handle MTV window msg
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditInputLongWinMsg(
                                             MMI_WIN_ID_T        win_id,
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM              param
                                             )
{
    MMI_RESULT_E      result = MMI_RESULT_TRUE;
    uint32 num = 0;
    char record_long[MMIMTV_LONG_TIME_MAX_DIGITS] = {0};
    wchar display[MMIMTV_LONG_TIME_MAX_DIGITS] = {0};
    MMI_STRING_T text_str = {0};

    switch( msg_id ) 
    {
    case MSG_OPEN_WINDOW:
		MMK_SetAtvCtrl(win_id, MTV_EDIT_RECORD_FILE_LONG_TEXT_CTRL_ID);
        num = MMIMTV_GetBookRecordLong();
        if(num > MMIMTV_BOOK_RECORD_FILE_LONG_MAX)
        {
            return;
        }
        num = sprintf(record_long, "%d", num);
        MMIAPICOM_StrToWstr((uint8*)record_long, display);
        GUIEDIT_SetString(MTV_EDIT_RECORD_FILE_LONG_TEXT_CTRL_ID, display, num); 
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
        GUIEDIT_GetString(MTV_EDIT_RECORD_FILE_LONG_TEXT_CTRL_ID, &text_str);
        MMIAPICOM_WstrToNum(text_str.wstr_ptr, text_str.wstr_len, &num);
        if(num <= MMIMTV_BOOK_RECORD_FILE_LONG_MAX)
        {
            MMIMTV_SetBookRecordLong(num);
            MMK_CloseWin(win_id);
        }
        else
        {
            uint32 time_out = MMI_3SECONDS;
            MMIPUB_OpenAlertWinByTextId(&time_out, TXT_MTV_LESS_600, PNULL, IMAGE_PUBWIN_FAIL, PNULL, 
                PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_FULL_PAINT:
        MMIMTV_ClearScreen();
        break;
        
    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
// 	Description : open manual search input scene
//	Global resource dependence :
//  Author:
//	Note: 手动输入频率
/*****************************************************************************/
PUBLIC void MMIMTV_OpenManualSearchInputScene(uint32 param)
{
    MMI_STRING_T text_str = {0};
    wchar   wstr_freq[MMIMTV_FREQ_STR_LEN +1] = {0};
    uint16  wstr_freq_len = 0;
    uint8   str_freq[MMIMTV_FREQ_STR_LEN +1] = {0};
    uint32  freq_num = 0;
    GUI_BG_T    bg = {0};
    GUI_RECT_T  edit_rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_TOP + MMIMTV_EDITBOX_HEIGHT};
    
    if(MMIMTV_OSD_DISPLAY_PTR->is_scene_close)
    {
        //获取用户之前输入的数据
        freq_num = MMIMTV_GetLastInputFreq();
        //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_OpenManualSearchInputScene, freq_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_4468_112_2_18_2_40_48_872,(uint8*)"d", freq_num);
        sprintf((char*)str_freq, "%d", freq_num);
        MMIAPICOM_StrToWstr((uint8*)str_freq, wstr_freq);
        wstr_freq_len = MMIAPICOM_Wstrlen(wstr_freq);
        //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_OpenManualSearchInputScene, wstr_freq_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_4472_112_2_18_2_40_48_873,(uint8*)"d", wstr_freq_len);
        if(wstr_freq_len > MMIMTV_FREQ_STR_LEN)
        {
            wstr_freq_len = MMIMTV_FREQ_STR_LEN;
        }
        
        MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_MANUAL_SEARCH_WIN_TAB);  
		{
        	GUI_RECT_T lab_rect = {MTV_OSD_LEFT, MTV_OSD_TOP + MMIMTV_EDITBOX_HEIGHT * 2, MTV_OSD_RIGHT, MTV_OSD_TOP + MMIMTV_EDITBOX_HEIGHT * 3};
			GUILABEL_SetRect(MTV_EDIT_MANUAL_SEARCH_LABEL_CTRL_ID, &lab_rect, FALSE);
        }		
				
        GUIEDIT_SetRect(MTV_EDIT_MANUAL_SEARCH_TEXT_CTRL_ID,&edit_rect);
        MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_EDIT_MANUAL_SEARCH_TEXT_CTRL_ID);
        
        bg.bg_type = GUI_BG_COLOR;
        bg.color = MMIMTV_BACKGROUND_COLOR;
        GUIEDIT_SetBg(MTV_EDIT_MANUAL_SEARCH_TEXT_CTRL_ID,&bg);
        GUIEDIT_SetMaxLen(MTV_EDIT_MANUAL_SEARCH_TEXT_CTRL_ID, MMIMTV_FREQ_STR_LEN);
        GUIEDIT_SetString(MTV_EDIT_MANUAL_SEARCH_TEXT_CTRL_ID, wstr_freq, wstr_freq_len);
        GUILABEL_SetTextById(MTV_EDIT_MANUAL_SEARCH_LABEL_CTRL_ID, TXT_MTV_CENTER_FREQUENCY_RANGE, FALSE);
        MMIMTV_OSD_DISPLAY_PTR->is_scene_close = FALSE;

                
    }
    
    //获取当前字符串信息, 字符为空时右软键盘显示"返回"
    GUIEDIT_GetString(MTV_EDIT_MANUAL_SEARCH_TEXT_CTRL_ID, &text_str);
    if(0 == text_str.wstr_len)
    {
        DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    }
    else
    {
        DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, TXT_DELETE);
    }
    DisplayTitle(TXT_MTV_INPUT_CENTENER_FREQ);
}

/*****************************************************************************/
// 	Description : update OSD_MAIN_MENU scene
//	Global resource dependence :
//  Author: xin.li
//	Note: 仅更新softkey，根据EDITBOX情况显示“删除”或“返回”
/*****************************************************************************/
PUBLIC void MMIMTV_UpdateManualSearchInputScene(void)
{
    MMI_STRING_T text_str = {0};

    //获取当前字符串信息
    GUIEDIT_GetString(MTV_EDIT_MANUAL_SEARCH_TEXT_CTRL_ID, &text_str);
    if(0 == text_str.wstr_len)
    {
        DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    }
    else
    {
        DisplaySoftKey(TXT_COMMON_OK, TXT_NULL, TXT_DELETE);
    }
}

/*****************************************************************************/
// 	Description : open OSD_MAIN_MENU scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseManualSearchInputScene(void)
{
    GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};

    MMK_DestroyControl(MTV_EDIT_MANUAL_SEARCH_TEXT_CTRL_ID);
    MMK_DestroyControl(MTV_EDIT_MANUAL_SEARCH_LABEL_CTRL_ID);
    ClearRect(rect);
    ClearTitle();
    MMIMTV_OSD_DISPLAY_PTR->is_scene_close = TRUE;
}

/*****************************************************************************/
// 	Description : display record file name
//	Global resource dependence :
//  Author: xin.li
//	Note: 显示录像文件名称
/*****************************************************************************/
LOCAL void DisplayRecordFileName(void)
{
    MMI_STRING_T text_str = {0};
    GUI_RECT_T   text_rect = {0};
    wchar*       file_name_ptr = PNULL;
    uint16       path_len = 0;
    wchar        path[] = {'D', ':', '\\', 'm', 't', 'v', '\\', 'v', 'i', 'd', 'e', 'o','\\',0};
    
    //get reocrd file name
    file_name_ptr     = MMIMTV_GetPlayInfo()->record_file_full_name;
    path_len          = MMIAPICOM_Wstrlen(path);
    text_str.wstr_ptr = file_name_ptr + path_len;   //ignore path
    text_str.wstr_len = MMIAPICOM_Wstrlen(text_str.wstr_ptr);
    
    text_rect.left    = MMIMTV_RECORD_FILE_NAME_LEFT;
    text_rect.top     = MMIMTV_RECORD_FILE_NAME_TOP;
    text_rect.bottom  = MMIMTV_RECORD_FILE_NAME_BOTTOM;

    //文件名太长则显示省略号
    text_rect.right = text_rect.left + GUI_CalculateStringPiexlNum(text_str.wstr_ptr, text_str.wstr_len, (GUI_FONT_T)MMIMTV_STRING_FONT, 1) + 1;
    if(text_rect.right > MMIMTV_RECORD_FILE_NAME_RIGHT)
    {
        text_rect.right = MMIMTV_RECORD_FILE_NAME_RIGHT;
    }
    
    ClearRect(text_rect);
    DisplayEffectString(text_rect, text_str.wstr_ptr, text_str.wstr_len, MMIMTV_STRING_FONT, TRUE);
}

/*****************************************************************************/
// 	Description : display record file play total time
//	Global resource dependence :
//  Author: xin.li
//	Note: 显示录像文件总时长
/*****************************************************************************/
LOCAL void DisplayRecordFileTime(uint32 total_time)
{
    MMI_STRING_T text_str = {0};
    GUI_RECT_T   text_rect = {0};
    char         str_time[MMIMTV_RECORD_TIME_MAX_DIGITS +1] = {0};
    wchar        wstr_time[MMIMTV_RECORD_TIME_MAX_DIGITS +1] = {0};
    uint8        hour   = 0;
    uint8        minute = 0;
    uint8        second = 0;
    
    hour    = total_time / 3600;
    minute  = (total_time % 3600) / 60;
    second  = total_time % 60;
    sprintf(str_time, "%02d:%02d:%02d", hour, minute, second);
    MMIAPICOM_StrToWstr((uint8*)str_time, wstr_time);
    text_str.wstr_ptr = wstr_time;
    text_str.wstr_len = MMIAPICOM_Wstrlen(wstr_time);
    
    text_rect.left   = MMIMTV_RECORD_TOTAL_TIME_LEFT;
    text_rect.top    = MMIMTV_RECORD_TOTAL_TIME_TOP;
    text_rect.right  = MMIMTV_RECORD_TOTAL_TIME_RIGHT;
    text_rect.bottom = MMIMTV_RECORD_TOTAL_TIME_BOTTOM;
    
    ClearRect(text_rect);
    DisplayEffectString(text_rect, text_str.wstr_ptr, text_str.wstr_len, MMIMTV_STRING_FONT, FALSE);
}


/*****************************************************************************/
// 	Description : open OSD_RECORD_PLAY scene
//	Global resource dependence :
//  Author: xin.li
//	Note: 打开本地回放场景
/*****************************************************************************/
PUBLIC void MMIMTV_OpenRecordPlayScene(uint32 param)
{
    uint32 time_count = 0;
    uint32 time_total = 0;
    
    //get time info
    MMIMTV_GetRecordPlayTimeInfo(&time_count, &time_total);
    
    DisplayRecordFileName();                  //文件名
    DisplayRecordFileTime(time_total);        //文件总时长
    MMIMTV_DisplayRecordPlayTime(time_count); //播放时间
    MMIMTV_DisplayRecordPlayProgress(time_count, time_total);
    
    //display soft key
    if(MMIMTV_PLAY_PLAYING == MMIMTV_GetPlayInfo()->play_status)
    {
        DisplaySoftKey(
            TXT_PAUSE,  //暂停
            TXT_NULL,
            STXT_CANCEL);   //返回
    }
    else
    {
        DisplaySoftKey(
            TXT_COMMON_PLAY,   //播放
            TXT_NULL,
            STXT_CANCEL);   //返回
    }
}

/*****************************************************************************/
// 	Description : close OSD_RECORD_PLAY scene
//	Global resource dependence :
//  Author: xin.li
//	Note: 关闭本地回放场景
/*****************************************************************************/
PUBLIC void MMIMTV_CloseRecordPlayScene(void)
{
    MMIMTV_ClearScreen();
}

/*****************************************************************************/
// 	Description : update OSD_RECORD_PLAY scene
//	Global resource dependence :
//  Author: xin.li
//	Note: 主要是软键文字显示更新
/*****************************************************************************/
PUBLIC void MMIMTV_UpdateRecordPlayScene(void)
{
    if(MMIMTV_PLAY_PLAYING == MMIMTV_GetPlayInfo()->play_status)
    {
        DisplaySoftKey(
            TXT_PAUSE,  //暂停
            TXT_NULL,
            STXT_CANCEL);   //返回
    }
    else
    {
        DisplaySoftKey(
            TXT_COMMON_PLAY,   //播放
            TXT_NULL,
            STXT_CANCEL);   //返回
    }
}

/*****************************************************************************/
// 	Description : display record play time count
//	Global resource dependence :
//  Author: xin.li
//	Note: 显示当前录像播放时间
/*****************************************************************************/
PUBLIC void MMIMTV_DisplayRecordPlayTime(uint32 time_count)
{
    MMI_STRING_T text_str = {0};
    GUI_RECT_T   text_rect = {0};
    char         str_time[MMIMTV_RECORD_TIME_MAX_DIGITS +1] = {0};
    wchar        wstr_time[MMIMTV_RECORD_TIME_MAX_DIGITS +1] = {0};
    uint8        hour   = 0;
    uint8        minute = 0;
    uint8        second = 0;

    hour    = time_count / MMIMTV_SECONDS_PER_HOUR;
    minute  = (time_count % MMIMTV_SECONDS_PER_HOUR) / MMIMTV_SECONDS_PER_MINUTE;
    second  = time_count % MMIMTV_SECONDS_PER_MINUTE;
    sprintf(str_time, "%02d:%02d:%02d", hour, minute, second);
    MMIAPICOM_StrToWstr((uint8*)str_time, wstr_time);
    text_str.wstr_ptr = wstr_time;
    text_str.wstr_len = MMIAPICOM_Wstrlen(wstr_time);
    
    //to do 
    text_rect.left   = MMIMTV_RECORD_PLAY_TIME_LEFT;
    text_rect.top    = MMIMTV_RECORD_PLAY_TIME_TOP;
    text_rect.right  = MMIMTV_RECORD_PLAY_TIME_RIGHT;
    text_rect.bottom = MMIMTV_RECORD_PLAY_TIME_BOTTOM;

    ClearRect(text_rect);
    DisplayEffectString(text_rect, text_str.wstr_ptr, text_str.wstr_len, MMIMTV_STRING_FONT, FALSE);
}

/*****************************************************************************/
// 	Description : display record play progress
//	Global resource dependence :
//  Author: xin.li
//	Note: 显示当前播放进度
/*****************************************************************************/
PUBLIC void MMIMTV_DisplayRecordPlayProgress(uint32 time_count, uint32 time_total)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T       progress_groove_rect ={MMIMTV_RECORD_PLAY_PROGRESS_GROOVE_LEFT, \
                                            MMIMTV_RECORD_PLAY_PROGRESS_GROOVE_TOP, \
                                            MMIMTV_RECORD_PLAY_PROGRESS_GROOVE_LEFT + MMIMTV_RECORD_PLAY_PROGRESS_GROOVE_WIDTH -1, \
                                            MMIMTV_RECORD_PLAY_PROGRESS_GROOVE_TOP + MMIMTV_RECORD_PLAY_PROGRESS_GROOVE_HEITHT -1};
    GUI_RECT_T       progress_rect        ={MMIMTV_RECORD_PLAY_PROGRESS_LEFT, \
                                            MMIMTV_RECORD_PLAY_PROGRESS_TOP, \
                                            MMIMTV_RECORD_PLAY_PROGRESS_LEFT + MMIMTV_RECORD_PLAY_PROGRESS_WIDTH -1, \
                                            MMIMTV_RECORD_PLAY_PROGRESS_TOP + MMIMTV_RECORD_PLAY_PROGRESS_HEITHT};
    GUI_RECT_T       progress_image_rect  ={0, 0, MMIMTV_RECORD_PLAY_PROGRESS_WIDTH -1, MMIMTV_RECORD_PLAY_PROGRESS_HEITHT -1};
    int16            progress_offset = 0;
    
    //display progress groove
    ClearRect(progress_groove_rect);
    DisplayPicture(progress_groove_rect.left, progress_groove_rect.top, IMAGE_COMMON_PROCESS_GROOVE_1);
    
    //display progress and slide
    progress_offset = (time_count * MMIMTV_RECORD_PLAY_PROGRESS_WIDTH) / time_total;
    progress_rect.right = progress_rect.left + progress_offset;
    if(MMK_IsFocusWin(MMIMTV_MAIN_WIN_ID))
    {
        //display slide
        DisplayPicture(progress_rect.right - (MMIMTV_RECORD_PALY_PROGRESS_SLIDE_WIDTH)/2, progress_groove_rect.top, IMAGE_COMMON_PLAYER_PROCESS_SLIDE);
        
        //display progress
        GUIRES_DisplayImg(PNULL,
            &progress_rect,
            &progress_image_rect,
            MMIMTV_MAIN_WIN_ID,
            IMAGE_COMMON_PLAYER_PROCESS,
            &lcd_dev_info);
    }
}

/*****************************************************************************/
//  Description : stop text scroll timer
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIMTV_GetDisappearTimer(void)
{
    return MMIMTV_OSD_DISPLAY_PTR->sys_timer;
}

/*****************************************************************************/
//  Description : stop text scroll timer
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ResetDisappearTimer(void)
{
    MMIMTV_OSD_DISPLAY_PTR->systime_count = 0;
}

/*****************************************************************************/
// 	Description : display program hint
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DisplayProgramHint(BOOLEAN is_new)
{
    static MMI_STRING_T hint_str = {0};
    static uint32 s_index = 0;
    GUI_RECT_T rect = {MMIMTV_WATCH_TIME_TIP_LEFT, MMIMTV_LCD_HEIGHT - MMIMTV_WATCH_TIME_TIP_HEIGHT, 
        MMIMTV_LCD_WIDTH - MMIMTV_SOFTKEY_WIDTH, MMIMTV_LCD_HEIGHT - 1};
    static uint32 s_start_index = 0;
    static uint32 s_rect_left = 0;

    if(is_new)
    {
        s_index = 0;
        
        if(PNULL != MMIMTV_OSD_DISPLAY_PTR->display_hint)
        {
            return;
        }
        
        MMIMTV_OSD_DISPLAY_PTR->display_hint = (wchar*)SCI_ALLOC_APP((MMIMTV_PROG_HINT_STR_LEN * 2 + MMIMTV_PROGRAM_HINT_CURRENT_NEXT_LEN) * sizeof(wchar));
        if(PNULL != MMIMTV_OSD_DISPLAY_PTR->display_hint)
        {
            SCI_MEMSET(&hint_str, 0, sizeof(MMI_STRING_T));
            MMI_GetLabelTextByLang(TXT_MTV_CURRENT_PROGRAM, &hint_str);
            MMIAPICOM_Wstrcpy(MMIMTV_OSD_DISPLAY_PTR->display_hint, hint_str.wstr_ptr);
            s_index += hint_str.wstr_len;
            
            if(MMIMTV_GetCurrentProgramHint() != PNULL)
            {
                MMI_GetLabelTextByLang(TXT_MTV_WARNING, &hint_str);
                MMIAPICOM_Wstrcpy(MMIMTV_OSD_DISPLAY_PTR->display_hint + s_index, MMIMTV_GetCurrentProgramHint());
                s_index += MMIAPICOM_Wstrlen(MMIMTV_GetCurrentProgramHint());
            }
            MMI_GetLabelTextByLang(TXT_MTV_NEXT_PROGRAM, &hint_str);
            MMIAPICOM_Wstrcpy(MMIMTV_OSD_DISPLAY_PTR->display_hint + s_index, hint_str.wstr_ptr);
            s_index += hint_str.wstr_len;
            if(MMIMTV_GetNextProgramHint() != PNULL)
            {
                MMI_GetLabelTextByLang(TXT_MTV_WARNING, &hint_str);
                MMIAPICOM_Wstrcpy(MMIMTV_OSD_DISPLAY_PTR->display_hint + s_index, MMIMTV_GetNextProgramHint());
                s_index += MMIAPICOM_Wstrlen(MMIMTV_GetNextProgramHint());
            }        
            hint_str.wstr_ptr = MMIMTV_OSD_DISPLAY_PTR->display_hint;
            hint_str.wstr_len = MMIAPICOM_Wstrlen(MMIMTV_OSD_DISPLAY_PTR->display_hint);
            s_index = 0;
            s_rect_left = rect.right;
            s_start_index = 0;
            CreateDisappearTimer(MMIMTV_SYSTIME_TEXT_SCROLL);
        }
    }
    else
    {
        ClearRect(rect);
        if(s_index <= hint_str.wstr_len || s_rect_left > MMIMTV_WATCH_TIME_TIP_LEFT)
        {
            s_rect_left = s_rect_left - GUI_GetFontWidth(MMIMTV_NORMAL_FONT, *(hint_str.wstr_ptr + s_index));
            if(s_rect_left < MMIMTV_WATCH_TIME_TIP_LEFT)
            {
                s_start_index ++;
                s_rect_left = MMIMTV_WATCH_TIME_TIP_LEFT;
            }
            else
            {
                rect.left = s_rect_left;
            }
            DisplayEffectString(rect, hint_str.wstr_ptr + s_start_index, s_index - s_start_index, MMIMTV_NORMAL_FONT, FALSE);
        }
        else if(s_start_index < hint_str.wstr_len)
        {
            s_start_index ++;
            DisplayEffectString(rect, hint_str.wstr_ptr + s_start_index, hint_str.wstr_len - s_start_index + 1, MMIMTV_NORMAL_FONT, FALSE);
        }
        else
        {
            MMIMTV_ClearProgramHint();
        }
    }
    s_index ++;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ClearProgramHint(void)
{
    GUI_RECT_T rect = {MMIMTV_WATCH_TIME_TIP_LEFT, MMIMTV_LCD_HEIGHT - MMIMTV_WATCH_TIME_TIP_HEIGHT, 
        MMIMTV_LCD_WIDTH - MMIMTV_SOFTKEY_WIDTH, MMIMTV_LCD_HEIGHT - 1};
    wchar *display_hint_ptr = MMIMTV_OSD_DISPLAY_PTR->display_hint;

    ClearRect(rect);
    if(PNULL != MMIMTV_OSD_DISPLAY_PTR->display_hint)
    {
        SCI_FREE(display_hint_ptr);
        MMIMTV_OSD_DISPLAY_PTR->display_hint = PNULL;
    }
    MMIMTV_StopSysTimer(MMIMTV_SYSTIME_TEXT_SCROLL);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CreateWatchTimeTipTimer(void)
{
    CreateDisappearTimer(MMIMTV_SYSTIME_WATCH_TIME_TIP);
    MMIMTV_OSD_DISPLAY_PTR->start_watch_time = MMIAPICOM_GetCurTime();

    MMIMTV_OSD_DISPLAY_PTR->prompt_times = 0;
}

/*****************************************************************************/
// 	Description : open OSD_MAIN_MENU scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenESGListScene(uint32 param)
{
    MMIMTV_ESG_LIST_SCENE_T* esg_list = (MMIMTV_ESG_LIST_SCENE_T*)param;
    uint32 i = 0;
    GUI_RECT_T rect2 = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_TOP + MTV_TITLE_HEIGHT};
    GUI_RECT_T cmmb_logo_rect = {MMIMTV_CMMB_LOGO_LEFT, MMIMTV_CMMB_LOGO_TOP, MMIMTV_CMMB_LOGO_LEFT+MMIMTV_CMMB_LOGO_WIDTH, MTV_OSD_TOP};
    GUI_RECT_T  list_rect = {0};
    
    if(PNULL == esg_list)
    {
        return;
    }
    
    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_ESG_WIN_TAB);
    list_rect.left   = MTV_OSD_LEFT;
    list_rect.top    = MTV_OSD_TOP;
    list_rect.right  = MTV_OSD_RIGHT;
    list_rect.bottom = MTV_OSD_BOTTOM;
    GUILIST_SetRect(MTV_ESG_TEXT_CTRL_ID,&list_rect);

    GUILIST_SetBgColor(MTV_ESG_TEXT_CTRL_ID, MMIMTV_BACKGROUND_COLOR);
    GUILIST_SetTextFont(MTV_ESG_TEXT_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMI_GRAY_WHITE_COLOR);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, MTV_ESG_TEXT_CTRL_ID);
    if(esg_list->is_exist)
    {        
        MMI_STRING_T text_str = {0};
        GUI_RECT_T rect = {MTV_OSD_LEFT, MTV_OSD_TOP - MTV_TITLE_HEIGHT, MTV_OSD_RIGHT, MTV_OSD_TOP};
        uint16 fontwidth = 0;
        //char* wday[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
        const MMI_TEXT_ID_T weekday_info[] ={ 
                                              TXT_SUNDAY, 
                                              TXT_MONDAY,
                                              TXT_TUESDAY, 
                                              TXT_WEDNESDAY,
                                              TXT_THURSDAY, 
                                              TXT_FRIDAY,
                                              TXT_SATURDAY};
        MMI_STRING_T str_wday = {0};
        MMIACC_RTC_DATE_T date = {0};
        wchar wline1[MMIMTV_EBLINE1_BUFFER_MAX_LEN] = {0};
        wchar temp[GUIRICHTEXT_TEXT_MAX_LEN] = {0};
        char  cline1[MMIMTV_EBLINE1_BUFFER_MAX_LEN] = {0};
        MMI_TM_T time = {0};
        MMIACC_RTC_TIME_T rtc_time = {0};
        MMIACC_RTC_TIME_T rtc_time2 = {0};
        uint32 final = 0;
        
        GUILIST_SetMaxItem(MTV_ESG_TEXT_CTRL_ID, esg_list->proglist.NumOfProg, FALSE);
        //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_OpenESGListScene, NumOfProg = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_4932_112_2_18_2_40_49_874,(uint8*)"d", esg_list->proglist.NumOfProg);
        for (i = 0; i < esg_list->proglist.NumOfProg; i ++)
        {
            MMIMTV_PROGRAMME_T* program = esg_list->proglist.prog_ptr + i;
            
            final = 0;
            SCI_MEMSET(wline1, 0, sizeof(wline1));
            SCI_MEMSET(temp, 0, sizeof(temp));
            SCI_MEMSET(cline1, 0, sizeof(cline1));
            SCI_MEMSET(&time, 0, sizeof(time));
            SCI_MEMSET(&rtc_time, 0, sizeof(rtc_time));
            SCI_MEMSET(&rtc_time2, 0, sizeof(rtc_time2));
            
            
            //节目起始时间
            sprintf((char*)rtc_time, "%02d:%02d", program->StartTime.hours, program->StartTime.minutes);
            final = MMIAPICOM_Tm2Second(program->StartTime.seconds, program->StartTime.minutes, program->StartTime.hours,
                program->StartTime.days, program->StartTime.months, program->StartTime.years) + program->Duration;
            time = MMIAPICOM_Second2Tm(final);
            sprintf((char*)rtc_time2, "%02d:%02d", time.tm_hour, time.tm_min);
            sprintf(cline1, "%s - %s  ", rtc_time, rtc_time2);
            MMIAPICOM_StrToWstr((uint8*)cline1, wline1);
            MMIAPICOM_Wstrcpy(temp, wline1);
            
            //节目名
            MMIMTV_Mtvstring2Unicode(&((esg_list->proglist.prog_ptr + i)->Title), temp + MMIAPICOM_Wstrlen(wline1), GUIRICHTEXT_TEXT_MAX_LEN);

            //add list item
            AppendListItem(MTV_ESG_TEXT_CTRL_ID, temp);
        }

        //日期, 星期
        DisplayPicture(rect.left, rect.top, MMITHEME_GetWinTitleBar());

        MMI_GetLabelTextByLang(weekday_info[(esg_list->current_date.wday+WEEKDAY_NUM-1)%WEEKDAY_NUM], &str_wday);
        
        text_str.wstr_len = sprintf((char*)date, "%02d/%02d  ", esg_list->current_date.mon, 
            esg_list->current_date.mday);
        SCI_MEMSET(wline1, 0, sizeof(wline1));
        MMIAPICOM_StrToWstr((uint8*)date, wline1);
		MMIAPICOM_Wstrncpy(wline1+text_str.wstr_len, str_wday.wstr_ptr, str_wday.wstr_len);
        text_str.wstr_ptr = wline1;
        text_str.wstr_len += str_wday.wstr_len;
        fontwidth = GUI_CalculateStringPiexlNum(text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT, 1);
#ifdef MBBMS_SUPPORT
        rect.left = rect.left + MMIMTV_CMMB_LOGO_WIDTH;
#endif
        rect.left = rect.left + (rect.right - rect.left - fontwidth) / 2;
        rect.top += MMIMTV_TITLE_STRING_OFFSET;
        DisplayString(rect, text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT);

        //频道名
        DisplayPicture(rect2.left, rect2.top, MMITHEME_GetWinTitleBar());
        SCI_MEMSET(wline1, 0, sizeof(wline1));
        MMIMTV_Mtvstring2Unicode(&esg_list->proglist.ServiceName, wline1, GUIRICHTEXT_TEXT_MAX_LEN);
        text_str.wstr_len = MMIAPICOM_Wstrlen(wline1);
        text_str.wstr_ptr = wline1;
        fontwidth = GUI_CalculateStringPiexlNum(wline1, text_str.wstr_len, MMIMTV_NORMAL_FONT, 1);
        rect2.left = rect2.left + (rect2.right - rect2.left - fontwidth) / 2;
        rect2.top += MMIMTV_TITLE_STRING_OFFSET;
        DisplayString(rect2, text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT);
        
        //date L/R button
        MMIMTV_OSD_DISPLAY_PTR->esg_date_left_arrow.left = MMIMTV_LEFT_ARROW_LEFT;
        MMIMTV_OSD_DISPLAY_PTR->esg_date_left_arrow.top = MMIMTV_LEFT_ARROW_TOP;
        MMIMTV_OSD_DISPLAY_PTR->esg_date_left_arrow.right = MMIMTV_LEFT_ARROW_RIGHT;
        MMIMTV_OSD_DISPLAY_PTR->esg_date_left_arrow.bottom = MMIMTV_LEFT_ARROW_BOTTOM;
        
        MMIMTV_OSD_DISPLAY_PTR->esg_date_right_arrow.left = MMIMTV_RIGHT_ARROW_LEFT;
        MMIMTV_OSD_DISPLAY_PTR->esg_date_right_arrow.top = MMIMTV_RIGHT_ARROW_TOP;
        MMIMTV_OSD_DISPLAY_PTR->esg_date_right_arrow.right = MMIMTV_RIGHT_ARROW_RIGHT;
        MMIMTV_OSD_DISPLAY_PTR->esg_date_right_arrow.bottom = MMIMTV_RIGHT_ARROW_BOTTOM;
        
#ifdef MBBMS_SUPPORT
        MMIMTV_OSD_DISPLAY_PTR->esg_date_left_arrow.left = MMIMTV_LEFT_ARROW_LEFT + MMIMTV_CMMB_LOGO_WIDTH;
        MMIMTV_OSD_DISPLAY_PTR->esg_date_left_arrow.right = MMIMTV_LEFT_ARROW_RIGHT + MMIMTV_CMMB_LOGO_WIDTH;
        //cmmb logo
        DisplayPicture(cmmb_logo_rect.left, cmmb_logo_rect.top, IMAGE_MTV_LOGO);
#endif
        
        DisplayPicture(MMIMTV_OSD_DISPLAY_PTR->esg_date_left_arrow.left,
            MMIMTV_OSD_DISPLAY_PTR->esg_date_left_arrow.top,
            IMAGE_LIST_TITLE_ARROW_LEFT_UP);
        DisplayPicture(MMIMTV_OSD_DISPLAY_PTR->esg_date_right_arrow.left,
            MMIMTV_OSD_DISPLAY_PTR->esg_date_right_arrow.top,
            IMAGE_LIST_TITLE_ARROW_RIGHT_UP); 

        //service L/R button
        MMIMTV_OSD_DISPLAY_PTR->esg_service_left_arrow.left = MMIMTV_LEFT_ARROW_LEFT;
        MMIMTV_OSD_DISPLAY_PTR->esg_service_left_arrow.top = MMIMTV_LEFT_ARROW_TOP + MTV_TITLE_HEIGHT;
        MMIMTV_OSD_DISPLAY_PTR->esg_service_left_arrow.right = MMIMTV_LEFT_ARROW_RIGHT;
        MMIMTV_OSD_DISPLAY_PTR->esg_service_left_arrow.bottom = MMIMTV_LEFT_ARROW_BOTTOM + MTV_TITLE_HEIGHT;
        
        MMIMTV_OSD_DISPLAY_PTR->esg_service_right_arrow.left = MMIMTV_RIGHT_ARROW_LEFT;
        MMIMTV_OSD_DISPLAY_PTR->esg_service_right_arrow.top = MMIMTV_RIGHT_ARROW_TOP + MTV_TITLE_HEIGHT;
        MMIMTV_OSD_DISPLAY_PTR->esg_service_right_arrow.right = MMIMTV_RIGHT_ARROW_RIGHT;
        MMIMTV_OSD_DISPLAY_PTR->esg_service_right_arrow.bottom = MMIMTV_RIGHT_ARROW_BOTTOM + MTV_TITLE_HEIGHT;

        DisplayPicture(MMIMTV_OSD_DISPLAY_PTR->esg_service_left_arrow.left,
            MMIMTV_OSD_DISPLAY_PTR->esg_service_left_arrow.top,
            IMAGE_LIST_TITLE_ARROW_LEFT_UP);
        DisplayPicture(MMIMTV_OSD_DISPLAY_PTR->esg_service_right_arrow.left,
            MMIMTV_OSD_DISPLAY_PTR->esg_service_right_arrow.top,
            IMAGE_LIST_TITLE_ARROW_RIGHT_UP);
    }
    else
    {
        MMI_STRING_T text_str = {0};
        
        GUILIST_SetMaxItem(MTV_ESG_TEXT_CTRL_ID, 1, FALSE);
        MMI_GetLabelTextByLang(TXT_EMPTY, &text_str);
        AppendListItem(MTV_ESG_TEXT_CTRL_ID, text_str.wstr_ptr);
        DisplayTitle(TXT_MTV_ESG);
        DisplayPicture(rect2.left, rect2.top, MMITHEME_GetWinTitleBar());
#ifdef MBBMS_SUPPORT
        //cmmb logo
        DisplayPicture(cmmb_logo_rect.left, cmmb_logo_rect.top, IMAGE_MTV_LOGO);
#endif
    }
    DisplaySoftKey(TXT_COMMON_UPDATE, TXT_NULL, STXT_RETURN);
}

/*****************************************************************************/
// 	Description : open OSD_MAIN_MENU scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseESGListScene(uint32 param)
{
    GUI_RECT_T  rect = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_BOTTOM};
    GUI_RECT_T  *esg_date_left_arrow_ptr = &(MMIMTV_OSD_DISPLAY_PTR->esg_date_left_arrow);
    GUI_RECT_T  *esg_date_right_arrow_ptr = &(MMIMTV_OSD_DISPLAY_PTR->esg_date_right_arrow);
    GUI_RECT_T  *esg_service_left_arrow_ptr = &(MMIMTV_OSD_DISPLAY_PTR->esg_service_left_arrow);
    GUI_RECT_T  *esg_service_right_arrow = &(MMIMTV_OSD_DISPLAY_PTR->esg_service_right_arrow);

    MMK_DestroyControl(MTV_ESG_TEXT_CTRL_ID);
    ClearRect(rect);
    SCI_MEMSET(esg_date_left_arrow_ptr, 0, sizeof(GUI_RECT_T));
    SCI_MEMSET(esg_date_right_arrow_ptr, 0, sizeof(GUI_RECT_T));
    SCI_MEMSET(esg_service_left_arrow_ptr, 0, sizeof(GUI_RECT_T));
    SCI_MEMSET(esg_service_right_arrow, 0, sizeof(GUI_RECT_T));
    ClearTitle();
}

/*****************************************************************************/
// 	Description : open switch service tab ctrl
//	Global resource dependence :
//  Author: xin.li
//	Note: 频道切换栏 for mbbms2.0
/*****************************************************************************/
PUBLIC void MMIMTV_OpenSwitchServiceTab(uint32 param)
{
    MMI_CTRL_ID_T   ctrl_id = MTV_SWITCH_PLAY_TAB_CTRL_ID;
    GUI_RECT_T rect = {MMIMTV_SWITCH_TAB_LEFT, MMIMTV_SWITCH_TAB_TOP, MMIMTV_SWITCH_TAB_RIGHT, MMIMTV_SWITCH_TAB_BOTTOM};

    
    if(s_is_open_swtich_service_tab)
    {
        return;
    }
    
    MMK_CreateCtrlByWinTab(MMIMTV_MAIN_WIN_ID, (uint32*)MMIMTV_SWITCH_PLAY_WIN_TAB);
    GUITAB_SetRect(ctrl_id,&rect);
    GUITAB_SetFontParam(ctrl_id, MMI_DEFAULT_SMALL_FONT, MMI_GRAY_WHITE_COLOR, 0);
    GUITAB_SetBackGroundImage(ctrl_id, 0);
    GUITAB_SetTabRotative(ctrl_id, TRUE);
    GUITAB_SetBackGroundColor(ctrl_id, MMIMTV_BACKGROUND_COLOR);
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, ctrl_id);
    
    //add tab items
    MMIMBBMS_AddServiceItemToTabCtrl(ctrl_id);
    //SCI_TRACE_LOW:"[MMIMTV] MMIMTV_OpenSwitchServiceTab MMIMTV_EnableStb"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_5100_112_2_18_2_40_49_875,(uint8*)"");
    MMIMTV_EnableStb();
    MMIMTV_StartStbDisableTimer();
    MMK_PostMsg(MMIMTV_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL, NULL);
    
    //create timer
    StartSwitchPlayTabTimer();
    s_is_open_swtich_service_tab = TRUE;
}

/*****************************************************************************/
// 	Description : close switch service tab ctrl
//	Global resource dependence :
//  Author: xin.li
//	Note: 频道切换栏 for mbbms2.0
/*****************************************************************************/
PUBLIC void MMIMTV_CloseSwitchServiceTab(void)
{
    GUI_RECT_T rect = {MMIMTV_SWITCH_TAB_LEFT, MMIMTV_SWITCH_TAB_TOP, MMIMTV_SWITCH_TAB_RIGHT, MMIMTV_SWITCH_TAB_BOTTOM};
    
    if(s_is_open_swtich_service_tab)
    {
        MMK_DestroyControl(MTV_SWITCH_PLAY_TAB_CTRL_ID);
        ClearRect(rect);
        
        //close timer
        StopSwitchPlayTabTimer();
        s_is_open_swtich_service_tab = FALSE;
    }
}

/*****************************************************************************/
// 	Description : is opened switch service tab ctrl
//	Global resource dependence :
//  Author: xin.li
//	Note: 频道切换栏 for mbbms2.0
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_IsOpenSwitchServiceTab(void)
{
    return s_is_open_swtich_service_tab;
}

/*****************************************************************************/
// 	Description : 播放连接等待
//	Global resource dependence :
//  Author:
//	Note: MBBMS使用
/*****************************************************************************/
PUBLIC void MMIMTV_OpenPlayConnectingScene(void)
{
    //MMIMTV_SAVE_SERVICE_INFO_T  *cmmb_service_note_ptr = PNULL;
    MMI_STRING_T text_str = {0};
    uint16		 text_width = 0;
    GUI_RECT_T	 text_rect = {MTV_MSGBOX_LEFT, MTV_MSGBOX_TOP, MTV_MSGBOX_RIGHT, MTV_MSGBOX_BOTTOM};
    GUI_RECT_T	 bg_rect = {MTV_MSGBOX_LEFT, MTV_MSGBOX_TOP-MMIMTV_FONT_HALF_HEIGHT, MTV_MSGBOX_RIGHT, MTV_MSGBOX_BOTTOM+MMIMTV_FONT_HALF_HEIGHT};
    wchar        service_name[MMIMTV_MAX_NAME_LEN +1] = {0};
    //mbbms preview text
    GUI_RECT_T	 welcome_rect = {MTV_MSGBOX_LEFT, MMIMBBMS_WELCOME_WORDS_TOP, MTV_MSGBOX_RIGHT, MMIMBBMS_WELCOME_WORDS_BOTTOM};
    wchar        welcome_wchar[MMIMTV_COMMON_MAX_LEN+1] = {0};
    uint16       welcome_len = 0;
    MMI_STRING_T welcome_str = {0};	
    //"loading" text
    MMI_STRING_T load_text = {0};
    GUI_RECT_T	 load_text_rect = {MTV_MSGBOX_LEFT, MMIMBBMS_WELCOME_WORDS_TOP, MTV_MSGBOX_RIGHT, MMIMBBMS_WELCOME_WORDS_BOTTOM};
    uint16 tv_logo_height = 0;
    uint16 space_h = 4;
    MBBMSSG_PREVIEW_DATA_T data_info = {0};
    MMI_CONTROL_CREATE_T                anim_ctrl = {0};
    GUIANIM_INIT_DATA_T                 anim_init = {0};
    GUIANIM_CTRL_INFO_T                 ctrl_info = {0};
    GUIANIM_FILE_INFO_T                  file_info_ptr = {0}; 
    GUIANIM_DISPLAY_INFO_T              display_info = {0};    
    GUI_LCD_DEV_INFO    lcd_dev={GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_RECT_T          display_rect={MMIMBBMS_PLAY_CONNECT_STARTUP_LEFT, MMIMBBMS_PLAY_CONNECT_STARTUP_TOP, 
                              MMIMBBMS_PLAY_CONNECT_STARTUP_LEFT+48, MMIMBBMS_PLAY_CONNECT_STARTUP_TOP+59};
    LOCAL uint8   data_size = 0;
    LOCAL uint8* data_ptr = PNULL;    
    GUI_POINT_T         dis_point = {0};
    void            *anim_ctrl_ptr = PNULL;    
	
    //get mbbms service name
    MMIMBBMS_GetCurrServiceName(service_name, MMIMTV_MAX_NAME_LEN);
    text_str.wstr_ptr = service_name;
    text_str.wstr_len = (uint16)MMIAPICOM_Wstrlen(service_name);
    //display bg image
    FillRectWithOSDMaskPicture(bg_rect);
    
    if(text_str.wstr_len > 0)
    {
        text_width = GUI_CalculateStringPiexlNum(text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT, 1);
        text_rect.left += (text_rect.right - text_rect.left - text_width) / 2;
        
        //display service name
        DisplayString(text_rect, text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT);
    }

	//display starup image
	//DisplayPicture(MMIMBBMS_PLAY_CONNECT_STARTUP_LEFT, MMIMBBMS_PLAY_CONNECT_STARTUP_TOP, IMAGE_MBBMS_TV_LOGO);

    //display service number
    MMIMTV_DisplayServiceNumberAlways();

    //display load text
    MMI_GetLabelTextByLang(TXT_MBBMS_PLAY_CONNECTING, &load_text);	
    text_width = GUI_CalculateStringPiexlNum(load_text.wstr_ptr, load_text.wstr_len, MMIMTV_NORMAL_FONT, 1);
    load_text_rect.left += (load_text_rect.right - load_text_rect.left - text_width) / 2;
    GUIRES_GetImgWidthHeight(PNULL,&tv_logo_height,IMAGE_MBBMS_TV_LOGO,MMIMTV_MAIN_WIN_ID);
    load_text_rect.top = MMIMBBMS_PLAY_CONNECT_STARTUP_TOP + tv_logo_height + space_h;
    DisplayEffectString(load_text_rect, load_text.wstr_ptr, load_text.wstr_len, MMIMTV_NORMAL_FONT, FALSE);
		
	//display progress image
	DisplayPicture(MMIMBBMS_PLAY_CONNECT_PROGRESS_LEFT,
        MMIMBBMS_PLAY_CONNECT_PROGRESS_TOP,
        IMAGE_MTV_LOOP_PROGRESS1 + MMIMTV_OSD_DISPLAY_PTR->play_connecting_count % 2);
    
    //only mbbms has preview text
#ifdef MBBMS_SUPPORT
    MMIMBBMS_GetPreviewText(welcome_wchar, &data_info);
    welcome_len = (uint16)MMIAPICOM_Wstrlen(welcome_wchar);
    
    //SCI_TRACE_LOW:"[MMIMBBMS]:mbbms welcome string len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_5214_112_2_18_2_40_49_876,(uint8*)"d", welcome_len);
    if(welcome_len > 0)
    {
        text_width = GUI_CalculateStringPiexlNum(welcome_wchar, welcome_len, MMIMTV_NORMAL_FONT, 1);
        welcome_rect.left += (welcome_rect.right - welcome_rect.left - text_width) / 2;
        DisplayEffectString(welcome_rect, welcome_wchar, welcome_len, MMIMTV_NORMAL_FONT, FALSE);
    }
    //SCI_TRACE_LOW:"MMIMTV_OpenPlayConnectingScene picture_ptr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_5221_112_2_18_2_40_49_877,(uint8*)"d", data_info.picture_ptr);
    if (PNULL != data_info.picture_ptr)
    {
        data_size = data_info.picture_ptr->PictureData.data_len;
        if (data_size > SG_MAX_BASE64BIN_LEN)
        {
            data_ptr = data_info.picture_ptr->PictureData.data_ptr;
        }
        else
        {
            data_ptr = data_info.picture_ptr->PictureData.data_arr;
        }
            
        if(PNULL != data_info.picture_ptr->file_name_ptr && 0 != data_info.picture_ptr->file_name_len)
        {
            uint16 i=0;
            for(i=0; i<data_info.picture_ptr->file_name_len; i++ )
            {
                //SCI_TRACE_LOW:"MMIMTV_OpenPlayConnectingScene filename[%d] = 0x%x, len = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_5239_112_2_18_2_40_49_878,(uint8*)"ddd", i, data_info.picture_ptr->file_name_ptr[i], data_info.picture_ptr->file_name_len);
            }
            //creat anim
            anim_init.both_rect.h_rect = display_rect;
            anim_ctrl.ctrl_id           = MMIMBBMS_CHANGE_SERVICE_PREVIEW_dATA_CTRL_ID;
            anim_ctrl.guid              = SPRD_GUI_ANIM_ID;
            anim_ctrl.init_data_ptr     = &anim_init;
            anim_ctrl.parent_win_handle = MMIMTV_MAIN_WIN_ID;

            anim_ctrl_ptr = MMK_CreateControl(&anim_ctrl);
                
            ctrl_info.is_ctrl_id = TRUE;
            ctrl_info.ctrl_id = MMIMBBMS_CHANGE_SERVICE_PREVIEW_dATA_CTRL_ID; 
            ctrl_info.ctrl_ptr = anim_ctrl_ptr;
                
            display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
            //display_info.is_bg_img = TRUE;
            display_info.is_disp_one_frame = TRUE;
            //display_info.is_need_resize = TRUE;
            display_info.is_update = TRUE;
            display_info.is_play_once = TRUE;
            display_info.is_syn_decode = TRUE;

            file_info_ptr.full_path_wstr_ptr = (wchar*)data_info.picture_ptr->file_name_ptr;//preview_data_path;
            file_info_ptr.full_path_wstr_len =   data_info.picture_ptr->file_name_len/2;
            GUIANIM_SetDefaultIconByPtr(anim_ctrl_ptr,NULL,NULL);
            GUIANIM_SetParam(&ctrl_info,PNULL,&file_info_ptr,&display_info);    

        }
        else if (data_ptr != PNULL && data_size > 0)
        {
            dis_point.x = MMIMBBMS_PLAY_CONNECT_STARTUP_LEFT;
            dis_point.y = MMIMBBMS_PLAY_CONNECT_STARTUP_TOP;
            GUIRES_DisplayImgByPtr(data_ptr, data_size, &dis_point, PNULL, PNULL, &lcd_dev);
        }
    }
    else
    {
        DisplayPicture(MMIMBBMS_PLAY_CONNECT_STARTUP_LEFT, MMIMBBMS_PLAY_CONNECT_STARTUP_TOP, IMAGE_MBBMS_TV_LOGO);
    }
    MBBMSSG_FreePreviewData(&data_info);
#endif

    
    CreateDisappearTimer(MMIMTV_MBBMS_PLAY_CONNECTING);
	DisplaySoftKey(TXT_NULL, TXT_NULL, STXT_RETURN);
}

/*****************************************************************************/
// 	Description : 更新连接等待
//	Global resource dependence :
//  Author:
//	Note: MBBMS使用
/*****************************************************************************/
PUBLIC void MMIMTV_UpdatePlayConnectingScene(void)
{
    //loop progress image
    DisplayPicture(MMIMBBMS_PLAY_CONNECT_PROGRESS_LEFT,
        MMIMBBMS_PLAY_CONNECT_PROGRESS_TOP,
        IMAGE_MTV_LOOP_PROGRESS1 + MMIMTV_OSD_DISPLAY_PTR->play_connecting_count % 2);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note: MBBMS使用
/*****************************************************************************/
PUBLIC void MMIMTV_ClosePlayConnectingScene(void)
{
	GUI_RECT_T  rect = {MTV_MSGBOX_LEFT, MTV_MSGBOX_TOP, MTV_MSGBOX_RIGHT, MTV_MSGBOX_BOTTOM};
	GUI_RECT_T  welcome_rect = {MTV_MSGBOX_LEFT, MMIMBBMS_WELCOME_WORDS_TOP, MTV_MSGBOX_RIGHT, MMIMBBMS_WELCOME_WORDS_BOTTOM};
    
    MMK_DestroyControl(MMIMBBMS_CHANGE_SERVICE_PREVIEW_dATA_CTRL_ID);		
    MMIMTV_ClearServiceNumber();
    ClearRect(rect);
    ClearRect(welcome_rect);
	MMIMTV_StopSysTimer(MMIMTV_MBBMS_PLAY_CONNECTING);
	MMIMTV_OSD_DISPLAY_PTR->play_connecting_count = 0;
}

/*****************************************************************************/
// 	Description : display audio service playing tip
//	Global resource dependence :
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_DisplayAudioServiceTip(GUI_LCD_DEV_INFO *dev_ptr)
{
    //MMIMTV_SAVE_SERVICE_INFO_T  *cmmb_service_note_ptr = PNULL;
    MMI_STRING_T text_head = {0};
    MMI_STRING_T text_str = {0};
    uint16		 text_width = 0;
    GUI_RECT_T	 text_rect = {MTV_MSGBOX_LEFT, MMIMBBMS_PLAY_CONNECT_PROGRESS_TOP-MMIMTV_NORMAL_FONT_HEIGHT, MTV_MSGBOX_RIGHT, MMIMBBMS_PLAY_CONNECT_PROGRESS_TOP};
    //wchar        service_name[MMIMTV_MAX_NAME_LEN +1] = {0};
    wchar        final_wstr[MMIMTV_MAX_NAME_LEN +1] = {0};
    
    //显示形式“正在播出***”
    
    //display bg image
    DisplayPictureByDevPtr(dev_ptr, 0, 0, IMAGE_MTV_INIT_BG);
    
    //get mbbms service name
    //MMIMBBMS_GetCurrServiceName(service_name, MMIMTV_MAX_NAME_LEN);
    
    //merge two string
    MMI_GetLabelTextByLang(TXT_MBBMS_AUDIO_SERVICE, &text_head);
    MMIAPICOM_Wstrncpy(final_wstr, text_head.wstr_ptr, text_head.wstr_len);
    //MMIAPICOM_Wstrcat(final_wstr, service_name);
    
    text_str.wstr_ptr = final_wstr;
    text_str.wstr_len = (uint16)MMIAPICOM_Wstrlen(final_wstr);
    if(text_str.wstr_len > 0)
    {
        text_width = GUI_CalculateStringPiexlNum(text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT, 1);
        text_rect.left += (text_rect.right - text_rect.left - text_width) / 2;
        DisplayEffectStringByDevPtr(dev_ptr, text_rect, text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT, FALSE);
    }
    
    //starup image
    DisplayPictureByDevPtr(dev_ptr, MMIMBBMS_PLAY_CONNECT_STARTUP_LEFT, MMIMBBMS_PLAY_CONNECT_STARTUP_TOP-MMIMTV_NORMAL_FONT_HEIGHT, IMAGE_MBBMS_TV_LOGO);
}

#ifdef MBBMS_SUPPORT
/*****************************************************************************/
// 	Description : 3,4级紧急广播询问
//	Global resource dependence :
//  Author:
//	Note: MBBMS使用
/*****************************************************************************/
PUBLIC void MMIMTV_OpenNewEBQueryScene(void)
{
    int8 char_num[MMIMTV_SERVICE_NUMBER_MAX_DIGITS +1] = {0};
    wchar wchar_num[MMIMTV_SERVICE_NUMBER_MAX_DIGITS +1] = {0};
    wchar wchar_dstbuf[MMIMBBMS_SERVICE_NAME_MAX_LEN+1] = {0};
    MMI_STRING_T prompt_str1 = {0};
    MMI_STRING_T prompt_str2 = {0};
    MMI_STRING_T text_str = {0};
    uint16 curr_len = 0;
    uint16 unread_eb_num = 0;
    
    //"您有n条紧急广播，点击选项查看"
    unread_eb_num = MMIMTV_GetUnreadEBMsgNum();
    sprintf(char_num, "%d", unread_eb_num);
    MMIAPICOM_StrToWstr((uint8*)char_num, wchar_num);
    
    MMI_GetLabelTextByLang(TXT_MBBMS_EB_QUERY_HEAD, &prompt_str1);
    MMI_GetLabelTextByLang(TXT_MBBMS_EB_QUERY_TAIL, &prompt_str2);
    
    MMIAPICOM_Wstrncpy(wchar_dstbuf, prompt_str1.wstr_ptr, prompt_str1.wstr_len);
    MMIAPICOM_Wstrcat(wchar_dstbuf, wchar_num);

    curr_len = (uint16)MMIAPICOM_Wstrlen(wchar_dstbuf);
    MMIAPICOM_Wstrncpy(wchar_dstbuf+curr_len, prompt_str2.wstr_ptr, prompt_str2.wstr_len);
    
    text_str.wstr_ptr = wchar_dstbuf;
    text_str.wstr_len = curr_len + prompt_str2.wstr_len;

    OpenPubWinByTextPtr(MTV_PUBWIN_TIP_SR_OPT, &text_str);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note: MBBMS使用
/*****************************************************************************/
PUBLIC void MMIMTV_CloseNewEBQueryScene(void)
{
    MMIMTV_CloseCommonMsgBox();
}
#endif

/*****************************************************************************/
// 	Description : 一直显示频道号，无3秒定时器
//	Global resource dependence :
//  Author:
//	Note: 为MBBMS移动Case增加
/*****************************************************************************/
PUBLIC void MMIMTV_DisplayServiceNumberAlways(void)
{
    MMIMTV_DisplayServiceNumber();
    //stop servie_number timer
    MMIMTV_StopSysTimer(MMIMTV_SYSTIME_SERVICE_NUMBER);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_GetChangeESGServiceArrowRect(GUI_RECT_T *left_rect,
                                                GUI_RECT_T *right_rect)
{
    SCI_MEMCPY(left_rect, &(MMIMTV_OSD_DISPLAY_PTR->esg_service_left_arrow), sizeof(GUI_RECT_T));
    SCI_MEMCPY(right_rect, &(MMIMTV_OSD_DISPLAY_PTR->esg_service_right_arrow), sizeof(GUI_RECT_T));
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_GetChangeESGDateArrowRect(GUI_RECT_T *left_rect,
                                             GUI_RECT_T *right_rect)
{
    SCI_MEMCPY(left_rect, &(MMIMTV_OSD_DISPLAY_PTR->esg_date_left_arrow), sizeof(GUI_RECT_T));
    SCI_MEMCPY(right_rect, &(MMIMTV_OSD_DISPLAY_PTR->esg_date_right_arrow), sizeof(GUI_RECT_T));
}

/*****************************************************************************/
// 	Description : display recording tip
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DisplayRecordTip(uint32 time)
{
    MMI_STRING_T text_str = {0};
    uint8 str_buffer_arr[MMIMTV_TEMP_STRING_BUFFER_MAX_LEN] = {0};
    wchar w_buffer_arr[MMIMTV_TEMP_STRING_BUFFER_MAX_LEN] = {0};
    GUI_RECT_T system_time = {MMIMTV_RECORD_TIME_LEFT, MTV_OSD_TOP - MTV_TITLE_HEIGHT*2, \
                                MMIMTV_RECORD_TIME_LEFT + MMIMTV_RECORD_TIME_WIDTH, MTV_OSD_TOP - MTV_TITLE_HEIGHT*2 + MMIMTV_NORMAL_FONT_HEIGHT};

    system_time.left -= MMIMTV_REC_TEXT_WIDTH;
    ClearRect(system_time);
    system_time.left += MMIMTV_REC_TEXT_WIDTH;
    text_str.wstr_len = sprintf((char*)str_buffer_arr, "REC %.2ld:%.2ld:%.2ld", \
        time / MMIMTV_SECONDS_PER_HOUR, (time % MMIMTV_SECONDS_PER_HOUR) / MMIMTV_SECONDS_PER_MINUTE, time % MMIMTV_SECONDS_PER_MINUTE);
    MMIAPICOM_StrToWstr((uint8*)str_buffer_arr, w_buffer_arr);
    text_str.wstr_ptr = w_buffer_arr;
    DisplayEffectString(system_time, text_str.wstr_ptr, text_str.wstr_len, MMIMTV_NORMAL_FONT, FALSE);
    if((time % 2) == 0)
    {
        DisplayPicture(system_time.left - MMIMTV_REC_TEXT_WIDTH, system_time.top, IMAGE_MTV_RECORD_TIP);
    }
}

/*****************************************************************************/
// 	Description : clear recording tip
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ClearRecordTip(void)
{
    GUI_RECT_T system_time = {MMIMTV_RECORD_TIME_LEFT, MTV_OSD_TOP - MTV_TITLE_HEIGHT*2, \
                                MMIMTV_RECORD_TIME_LEFT + MMIMTV_RECORD_TIME_WIDTH, MTV_OSD_TOP - MTV_TITLE_HEIGHT*2 + MMIMTV_NORMAL_FONT_HEIGHT};
    system_time.left -= MMIMTV_REC_TEXT_WIDTH;

    ClearRect(system_time);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DisplaySnapshotTip(void)
{
    GUI_RECT_T rect1 = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_TOP + MTV_TITLE_HEIGHT};
    GUI_RECT_T rect2 = {MTV_OSD_LEFT, MTV_OSD_TOP + MMIMTV_NORMAL_FONT_HEIGHT, MTV_OSD_RIGHT, MTV_OSD_TOP + MTV_TITLE_HEIGHT + MMIMTV_NORMAL_FONT_HEIGHT + MMIMTV_NORMAL_FONT_HEIGHT};
    GUI_RECT_T bg_rect = {0};
    MMI_STRING_T text_str1 = {0};
    MMI_STRING_T text_str2 = {0};
    wchar  wstr_file_tip[MMIMTV_MAX_NAME_LEN] = {0};
    //uint16 str_width = 0;
    
    bg_rect.left = rect1.left;
    bg_rect.top  = rect1.top;
    bg_rect.right = rect1.right;
    bg_rect.bottom = rect2.bottom;
    ClearRect(bg_rect);
    FillRectWithOSDMaskPicture(bg_rect);

    //文件***
    //ClearRect(rect1);
    MMI_GetLabelTextByLang(TXT_FILE, &text_str1);
    MMIAPICOM_Wstrcpy(wstr_file_tip, text_str1.wstr_ptr);
    MMIAPICOM_Wstrcpy(wstr_file_tip + text_str1.wstr_len, MMIMTV_GetSnapshotPictureName());
    DisplayString(rect1, wstr_file_tip, MMIAPICOM_Wstrlen(wstr_file_tip), MMIMTV_NORMAL_FONT);
    

    //已存储至***文件夹
    //ClearRect(rect2);
	MMI_GetLabelTextByLang(MMIAPIFMM_GetDeviceName(MMIMTV_GetSnapshotStore()), &text_str2);
    //str_width = GUI_GetStringWidth(MMIMTV_NORMAL_FONT, text_str2.wstr_ptr, text_str2.wstr_len);
    DisplayString(rect2, text_str2.wstr_ptr, text_str2.wstr_len, MMIMTV_NORMAL_FONT);
    

    //启动已保存提示的定时器
    CreateDisappearTimer(MMIMTV_SYSTIME_FILE_SAVED_TIP);
    MMIMTV_OSD_DISPLAY_PTR->file_saved_tip_count = 0;
}

/*****************************************************************************/
// 	Description : 刷新文件已保存区域
//	Global resource dependence :
//  Author:
//	Note: 文件已保存提示有：录制文件、捕捉图像
/*****************************************************************************/
PUBLIC void MMIMTV_ClearFileSavedTip(void)
{
    GUI_RECT_T rect1 = {MTV_OSD_LEFT, MTV_OSD_TOP, MTV_OSD_RIGHT, MTV_OSD_TOP + MTV_TITLE_HEIGHT};
    GUI_RECT_T rect2 = {MTV_OSD_LEFT, MTV_OSD_TOP + MMIMTV_NORMAL_FONT_HEIGHT, MTV_OSD_RIGHT, MTV_OSD_TOP + MTV_TITLE_HEIGHT + MMIMTV_NORMAL_FONT_HEIGHT};
    
    ClearRect(rect1);
    ClearRect(rect2);
    MMIMTV_StopSysTimer(MMIMTV_SYSTIME_FILE_SAVED_TIP);
}

/*****************************************************************************/
// 	Description : get signal icon id for status bar
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIMTV_GetTVStatusImageID(uint32 signal)
{
    //MMI_IMAGE_ID_T image_id =  IMAGE_NULL;
    
    if(signal > MMIMTV_SIGNAL_LEVEL_MAX)
    {
        signal = MMIMTV_SIGNAL_LEVEL_MAX;
    }
    
    return s_signal_level_image_id[signal];
}

/*****************************************************************************/
// 	Description : display eb current index and total number
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DisplayEBCountInfo(uint16 cur_index, uint16 total_num)
{
    MMI_STRING_T text_str = {0};
	char	str_info[MMIMTV_EBLINE1_BUFFER_MAX_LEN] = {0};
	wchar	wstr_info[MMIMTV_EBLINE1_BUFFER_MAX_LEN] = {0};
    GUI_RECT_T rect = {MTV_OSD_RIGHT - 36, MTV_OSD_TOP - 22, MTV_OSD_RIGHT, MTV_OSD_TOP};

	sprintf(str_info, "%d%s%d", cur_index, "/", total_num);
	MMIAPICOM_StrToWstr((uint8*)str_info, wstr_info);
	text_str.wstr_ptr = wstr_info;
	text_str.wstr_len = MMIAPICOM_Wstrlen(wstr_info);
   
	DisplayTitle(TXT_MTV_EMERGENCY_BROADCASTING);
    DisplayString(rect, text_str.wstr_ptr, text_str.wstr_len, MMI_LISTNUM_FONT);
}


/*****************************************************************************/
// 	Description :start the status bar disable timer
//	Global resource dependence : none
//  Author: yexiong.liu
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_StartStbDisableTimer(void)
{
    if(PNULL != s_osd_display_data_ptr)
    {
        if(0 == MMIMTV_OSD_DISPLAY_PTR->stb_disable_timer_id)
        {        
            MMIMTV_OSD_DISPLAY_PTR->stb_disable_timer_id = MMK_CreateWinTimer(MMIMTV_MAIN_WIN_ID, (MMIMTV_SYSTEM_TIMER_UNIT*2), TRUE);
            MMIMTV_OSD_DISPLAY_PTR->stb_disable_timer_count = 0;
            //SCI_TRACE_LOW:"[MMIMTV] MMIMTV_StartStbDisableTimer if timer id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_5578_112_2_18_2_40_50_879,(uint8*)"d", MMIMTV_OSD_DISPLAY_PTR->stb_disable_timer_id);
        }
        //SCI_TRACE_LOW:"[MMIMTV] MMIMTV_StartStbDisableTimer else timer id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_5580_112_2_18_2_40_50_880,(uint8*)"d", MMIMTV_OSD_DISPLAY_PTR->stb_disable_timer_id);
    }
}

/*****************************************************************************/
// 	Description : start the status bar disable timer
//	Global resource dependence : none
//  Author:yexiong.liu
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_StopStbDisableTimer(void)
{
    if(PNULL != s_osd_display_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIMTV] MMIMTV_StopStbDisableTimer timer id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_5592_112_2_18_2_40_50_881,(uint8*)"d", MMIMTV_OSD_DISPLAY_PTR->stb_disable_timer_id);
        if (MMIMTV_OSD_DISPLAY_PTR->stb_disable_timer_id > 0)
        {
            MMK_StopTimer(MMIMTV_OSD_DISPLAY_PTR->stb_disable_timer_id);
            MMIMTV_OSD_DISPLAY_PTR->stb_disable_timer_id = 0;
            MMIMTV_OSD_DISPLAY_PTR->stb_disable_timer_count = 0;
        }
    }
}

/*****************************************************************************/
//  Description : get stb disable timer id
//  Global resource dependence : 
//  Author: yexiong.liu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIMTV_GetStbDisableTimerId(void)
{
    return MMIMTV_OSD_DISPLAY_PTR->stb_disable_timer_id;
}

/*****************************************************************************/
//  Description : handle stb disable timer
//  Global resource dependence : 
//  Author: yexiong.liu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMTV_HandleStbDisableTimer(void)
{
    uint8 time_out = 0;
    //SCI_TRACE_LOW:"[MMIMTV] MMIMTV_HandleStbDisableTimer type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_5618_112_2_18_2_40_50_882,(uint8*)"d", MMIMTV_GetOSDDisplayTime());
    switch(MMIMTV_GetOSDDisplayTime())
    {
    case MMIMTV_DISPLAY_TIME_3S:
        time_out = MMIMTV_3S_TIMEOUT;
        break;
        
    case MMIMTV_DISPLAY_TIME_5S:
        time_out = MMIMTV_5S_TIMEOUT;
        break;
        
    case MMIMTV_DISPLAY_TIME_10S:
        time_out = MMIMTV_10S_TIMEOUT;
        break;
        
    case MMIMTV_DISPLAY_TIME_15S:
        time_out = MMIMTV_15S_TIMEOUT;
        break;
    default:
        time_out = MMIMTV_5S_TIMEOUT;
        break;
    }        
    //time_out = time_out * 2;
    MMIMTV_OSD_DISPLAY_PTR->stb_disable_timer_count ++;
    if(time_out == MMIMTV_OSD_DISPLAY_PTR->stb_disable_timer_count)
    {
        MMIMTV_StopStbDisableTimer();
        MMIMTV_DisableStb();
    }
}

/*****************************************************************************/
//  Description : Disable Stb
//  Global resource dependence : 
//  Author: yexiong.liu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DisableStb(void)
{
    MMI_HANDLE_T        ctrl_handle = NULL;
    GUI_BOTH_RECT_T           both_rect = {0};  
        
    ctrl_handle = MMK_GetCtrlHandleByWin(MMIMTV_MAIN_WIN_ID, MMITHEME_GetStatusBarCtrlId());
    //SCI_TRACE_LOW:"[MMIMTV] MMIMTV_DisableStb ctrl_handle = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_5659_112_2_18_2_40_50_883,(uint8*)"d", ctrl_handle);
    if(PNULL != ctrl_handle)
    {
        IGUICTRL_SetState(MMK_GetCtrlPtr(ctrl_handle), GUICTRL_STATE_INVISIBLE, TRUE); 
        both_rect = MMITHEME_GetStatusBarBothRect();
        ClearRect(both_rect.h_rect);
    }
}

/*****************************************************************************/
//  Description : enable Stb
//  Global resource dependence : 
//  Author: yexiong.liu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMTV_EnableStb(void)
{
    MMI_HANDLE_T        ctrl_handle = NULL;
        
    ctrl_handle = MMK_GetCtrlHandleByWin(MMIMTV_MAIN_WIN_ID, MMITHEME_GetStatusBarCtrlId());
    //SCI_TRACE_LOW:"[MMIMTV] MMIMTV_EnableStb ctrl_handle = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_OSD_DISPLAY_5677_112_2_18_2_40_50_884,(uint8*)"d", ctrl_handle);
    if(PNULL != ctrl_handle)
    {
        IGUICTRL_SetState(MMK_GetCtrlPtr(ctrl_handle), GUICTRL_STATE_INVISIBLE , FALSE); 
        GUIWIN_UpdateStb();
    }
}

#endif


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:52 2012*/ //IGNORE9527
