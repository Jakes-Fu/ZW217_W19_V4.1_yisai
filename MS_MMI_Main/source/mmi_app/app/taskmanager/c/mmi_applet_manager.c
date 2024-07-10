/*****************************************************************************
** File Name:      mmi_applet_manager.c                                      *
** Author:                                                                   *
** Date:           17/12/2009                                                *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to applet manager                       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2007       James.Zhang       Create                                    *
******************************************************************************/

#define _MMI_APPLET_MANAGER_C_
#ifdef MMI_PDA_SUPPORT
#define MMI_MANAGER_BUTTON_SUPPORT
#endif
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_taskmanager_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef MMI_TASK_MANAGER
#include "mmi_applet_manager.h"
#include "mmidisplay_data.h"
#include "mmi_id.h"
#include "window_parse.h"
#include "guilistbox.h"
#include "mmk_app.h"
#include "mmk_modinfo.h"
#include "mmi_appmsg.h"
#include "mmicc_export.h"
#ifdef MMI_MANAGER_BUTTON_SUPPORT
#include "mmipub.h"
#endif
#include "mmi_resource.h"
#ifdef CMCC_UI_STYLE
#include "mmi_menutable.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMI_APPLET_DISPLAY_TIME_MAX_LEN 30
#define MMI_1MINUTES 60000
#ifdef MMI_MANAGER_BUTTON_SUPPORT
#define MMI_CLEANTASK_TOTAL_NUM 100
#endif

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
LOCAL MMI_APPLET_INFO_LIST_T* s_applet_list_ptr = PNULL;

//新增状态MMI_APPLET_STATE_TASK_MANAGER表示任务管理器可显示的应用，去掉全局变量s_applet_gui_id_arr
// LOCAL const uint32  s_applet_gui_id_arr[] = 
// {
// #ifdef MMI_CALL_MINIMIZE_SUPPORT
//     SPRD_CC_APPLET_ID,
// #endif
//     SPRD_GAME_LINK_APPLET_ID,
//     SPRD_GAME_BOX_APPLET_ID,
//     SPRD_MP3_APPLET_ID,
//     SPRD_FM_APPLET_ID,
//     SPRD_EBOOK_APPLET_ID,
//     SPRD_JAVA_APPLET_ID,
// #ifdef MCARE_V31_SUPPORT
//     MCARE_APPLET_ID
// #endif
// 
// };

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct 
{
    //uint8 tm_sec;     // secondsafter the minute - [0,59] 
    uint8 tm_min;     // minutesafter the hour - [0,59] 
    uint8 tm_hour;    // hours since midnight - [0,...] 
    //uint32 tm_day;    // days
}MMI_APPLET_TIME_T;


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
 
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef MMI_MANAGER_BUTTON_SUPPORT
/*****************************************************************************/
//  Description :  draw manager window when lcd switch
//  Global resource dependence : 
//  Author: yanxian.zhou
//  Note: 
/*****************************************************************************/
LOCAL void DrawManagerWin(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : clean systerm buffer in manager window
//  Global resource dependence : 
//  Author: yanxian.zhou
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E CleanBufferButtonCallBack(void);

/*****************************************************************************/
//  Description : to handle the message of clean buffer progress window    
//  Global resource dependence : 
//  Author:yanxian.zhou
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCleanProcessWinMsg(
                                            MMI_WIN_ID_T     win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM           param
                                            );
#endif

/*****************************************************************************/
//  Description : handle applet manager list win msg
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAppletManagerListWinMsg(
                                                 MMI_WIN_ID_T        win_id,
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                                 );
#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Description : HandleAppletManagerOptionWinMsg
//  Global resource dependence : 
//  Author: murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAppletManagerOptionWinMsg(
                                                 MMI_WIN_ID_T        win_id,
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                                 );
#endif
/*****************************************************************************/
//  Description : append list item
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AppendAppletListItem(
                                   MMI_CTRL_ID_T           ctrl_id,
                                   MMI_APPLET_INFO_LIST_T* applet_list_ptr,
                                   uint16                  item_index,
                                   uint16                  content_index,
                                   BOOLEAN                 is_item_data
                                   );

/*****************************************************************************/
//  Description : set applet list
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void AppendAppletList(
                            MMI_CTRL_ID_T           ctrl_id,
                            MMI_APPLET_INFO_LIST_T* applet_list_ptr
                            );

/*****************************************************************************/
//  Description : set applet active
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void SetAppletActive(
                           MMI_CTRL_ID_T           ctrl_id,
                           MMI_APPLET_INFO_LIST_T* applet_list_ptr
                           );

/*****************************************************************************/
//  Description : set applet hide
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void SetAppletHide(
                         MMI_CTRL_ID_T           ctrl_id,
                         MMI_APPLET_INFO_LIST_T* applet_list_ptr
                         );
                         
/*****************************************************************************/
//  Description : set applet close
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void SetAppletClose(
                           MMI_CTRL_ID_T           ctrl_id,
                           MMI_APPLET_INFO_LIST_T* applet_list_ptr
                           );

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Description : close all running application
//  Global resource dependence : 
//  Author: murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL void SetAppletAllClose(
                           MMI_APPLET_INFO_LIST_T* applet_list_ptr
                           );
#endif
                          
/*****************************************************************************/
//  Description : turn second into tm struct
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_APPLET_TIME_T Second2Time(uint32 second);

/*****************************************************************************/
//  Description : get running time of one applet
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void GetRunningTime(MMI_HANDLE_T applet_handle, wchar *str_ptr, uint16  str_ma_len);

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void HandleTimer(uint8 timer_id, uint32 param);
/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
WINDOW_TABLE( MMIAPPLET_MANAGER_LIST_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleAppletManagerListWinMsg),
    WIN_ID(MMIAPPLET_MANAGER_LIST_WIN_ID),
    WIN_TITLE(TXT_APPLICATION_MANAGER),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIAPPLET_MANAGER_LIST_ID),
#ifdef MMI_MANAGER_BUTTON_SUPPORT
    CREATE_BUTTON_CTRL(PNULL,MMIAPPLET_MANAGER_BOTTON_ID),
#endif
    WIN_SOFTKEY(TXT_CLOSE,TXT_ACTIVE,STXT_RETURN),
    END_WIN
};

#ifdef CMCC_UI_STYLE
WINDOW_TABLE( MMIAPPLET_MANAGER_OPTION_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleAppletManagerOptionWinMsg),
    WIN_ID(MMIAPPLET_MANAGER_OPTION_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_TASK_MANAGER_OPTION,MMIAPPLET_MANAGER_OPTION_ID),
    END_WIN
};
#endif

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : open applet manager
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMI_OpenAppletManager( void )
{
    if ( ! (MMK_IsOpenWin(MMI_PIN_INPUTING_WIN_ID) || MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID) ) )
    {
        MMK_CreateWin((uint32 *)MMIAPPLET_MANAGER_LIST_WIN_TAB, PNULL );
    }    
}

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Description : HandleAppletManagerOptionWinMsg
//  Global resource dependence : 
//  Author: murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAppletManagerOptionWinMsg(
                                                 MMI_WIN_ID_T        win_id,
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                                 )
{
    MMI_RESULT_E          result  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T         ctrl_id = MMIAPPLET_MANAGER_OPTION_ID;

    switch( msg_id )
    {
    case MSG_OPEN_WINDOW:
       MMK_SetAtvCtrl(win_id,ctrl_id);     
       break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
        {
            MMI_MENU_GROUP_ID_T group_id = 0;
            MMI_MENU_ID_T       menu_id = 0;
            GUIMENU_GetId(MMIAPPLET_MANAGER_OPTION_ID, &group_id, &menu_id);
            
            switch (menu_id)
            {
                case ID_MENU_TASK_MANAGE_OPTION_CLOSE_ID:
                    SetAppletClose( MMIAPPLET_MANAGER_LIST_ID, s_applet_list_ptr );

                    break;
                case ID_MENU_TASK_MANAGE_OPTION_CLOSE_ALL_ID:
                    SetAppletAllClose(s_applet_list_ptr);
                    break;
                default:
                    break;
            }
        }
        MMK_CloseWin( win_id );
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}
#endif

/*****************************************************************************/
//  Description : handle applet manager list win msg
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAppletManagerListWinMsg(
                                                 MMI_WIN_ID_T        win_id,
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                                 )
{
    MMI_RESULT_E                  result            = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                 ctrl_id           = MMIAPPLET_MANAGER_LIST_ID;
    GUILIST_NEED_ITEM_DATA_T*     need_data_ptr     = PNULL;
    GUILIST_NEED_ITEM_CONTENT_T*  need_content_ptr  = PNULL;
    LOCAL uint32 s_minute_timer = 0;

    switch( msg_id )
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_MANAGER_BUTTON_SUPPORT
        
        GUIBUTTON_SetTextAlign(MMIAPPLET_MANAGER_BOTTON_ID,  ALIGN_HVMIDDLE);
        GUIBUTTON_SetRunSheen( MMIAPPLET_MANAGER_BOTTON_ID, TRUE);
        GUIBUTTON_SetTextId( MMIAPPLET_MANAGER_BOTTON_ID,  TXT_APPLICATION_CLEAN_BUTTON);
        GUIBUTTON_SetCallBackFunc(MMIAPPLET_MANAGER_BOTTON_ID, CleanBufferButtonCallBack);
#endif
        MMK_SetAtvCtrl(win_id,ctrl_id);
        s_applet_list_ptr = MMK_CreateAppletInfoList( 0 );
        AppendAppletList( ctrl_id, s_applet_list_ptr );
        if (GUILIST_GetTotalItemNum(ctrl_id)> 0)
        {
            s_minute_timer = MMK_CreateTimerCallback(MMI_1MINUTES, HandleTimer, 0, TRUE);
        }
        else
        {
            s_minute_timer = 0;
        }
        break;

#ifdef MMI_MANAGER_BUTTON_SUPPORT
    case MSG_FULL_PAINT:
        DrawManagerWin(win_id);
        break;
#endif

    case MSG_APPLET_MANAGER_UPDATE:

        MMK_DestroyAppletInfoList( s_applet_list_ptr );

        s_applet_list_ptr = MMK_CreateAppletInfoList( 0 );
        
        AppendAppletList( ctrl_id, s_applet_list_ptr );
        if (MMK_IsFocusWin(win_id))
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        
        if (GUILIST_GetTotalItemNum(ctrl_id) == 0)//stop the timer
        {
            if (s_minute_timer != 0)
            {
                MMK_StopTimer(s_minute_timer);
                s_minute_timer = 0;
            }
        }
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:

        need_data_ptr = (GUILIST_NEED_ITEM_DATA_T *)param;
        SCI_ASSERT(PNULL != need_data_ptr);/*assert verified*/
        
        AppendAppletListItem( ctrl_id, s_applet_list_ptr, 
            need_data_ptr->item_index, 0, TRUE );
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_CONTENT:

        need_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;
        SCI_ASSERT(PNULL != need_content_ptr);/*assert verified*/
        
        AppendAppletListItem( ctrl_id, s_applet_list_ptr, 
            need_content_ptr->item_index, need_content_ptr->item_content_index, FALSE );
        break;
 
    case MSG_CTL_OK:
    case MSG_APP_OK:
 #ifdef CMCC_UI_STYLE
         if (GUILIST_GetTotalItemNum(ctrl_id)> 0)
         {
            MMK_CreateWin((uint32 *)MMIAPPLET_MANAGER_OPTION_WIN_TAB, PNULL );
         }
 #else
        SetAppletClose( ctrl_id, s_applet_list_ptr );
 #endif
        // SetAppletHide( ctrl_id, s_applet_list_ptr );        
        //MMK_CloseWin( win_id );
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
         if (GUILIST_GetTotalItemNum(ctrl_id) > 0)
         {
            SetAppletActive( ctrl_id, s_applet_list_ptr );
            MMK_CloseWin( win_id );
         }
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CLOSE_WINDOW:
        MMK_DestroyAppletInfoList( s_applet_list_ptr );
        if (s_minute_timer != 0)
        {
            MMK_StopTimer(s_minute_timer);
            s_minute_timer = 0;
        }
        break;
        
     case MSG_CTL_LIST_CHECK_CONTENT:
        SetAppletClose( ctrl_id, s_applet_list_ptr );
        //MMK_CloseWin( win_id );
        break;

    case MSG_LOSE_FOCUS:
        if (s_minute_timer != 0)//stop the timer
        {
            MMK_StopTimer(s_minute_timer);
            s_minute_timer = 0;
        }
        break;
    
    case MSG_GET_FOCUS:
// #ifdef MMI_MANAGER_BUTTON_SUPPORT
//         DrawManagerWin(win_id);
// #endif
        if (GUILIST_GetTotalItemNum(ctrl_id) > 0)//resume the timer
        {
            s_minute_timer = MMK_CreateTimerCallback(MMI_1MINUTES, HandleTimer, 0, TRUE);
            if ( PNULL != s_applet_list_ptr )
            {
                GUILIST_RemoveAllItems(ctrl_id);
                AppendAppletList( ctrl_id, s_applet_list_ptr );
            }
        }
        else
        {
            s_minute_timer = 0;
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

#ifdef MMI_MANAGER_BUTTON_SUPPORT
/*****************************************************************************/
//  Description :  draw manager window when lcd switch
//  Global resource dependence : 
//  Author: yanxian.zhou
//  Note: 
/*****************************************************************************/
LOCAL void DrawManagerWin(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T      list_rect = {0};
    GUI_LCD_DEV_INFO        lcd_dev_info = {0, 0};
    GUI_FONT_ALL_T          button_font_all = {0};
    GUI_BG_T   bg_btn = {0};
    GUI_RECT_T button1_rc = {0}; 
    GUI_RECT_T clientwin_rect ={0};
    //GUI_RECT_T fill_rect = {0};
    MMI_HANDLE_T win_handle = MMK_ConvertIdToHandle(win_id);

    clientwin_rect = MMITHEME_GetClientRectEx(win_handle);
    button1_rc.top = clientwin_rect.bottom - 60;
    button1_rc.left = (clientwin_rect.left+clientwin_rect.right)/2 - 80;
    button1_rc.right = (clientwin_rect.left+clientwin_rect.right)/2 + 80;
    button1_rc.bottom = clientwin_rect.bottom - 20;
    button_font_all.font = MMI_DEFAULT_SMALL_FONT ;
    button_font_all.color =  MMITHEME_GetCurThemeFontColor(MMI_THEME_MENU);
    bg_btn.bg_type = GUI_BG_COLOR;
    bg_btn.color = MMI_DARK_WHITE_COLOR;
    list_rect.left   = 0;
    list_rect.top    = clientwin_rect.top;
    list_rect.right  = clientwin_rect.right;
    list_rect.bottom = button1_rc.top-1;

    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    GUIBUTTON_SetFont(MMIAPPLET_MANAGER_BOTTON_ID, &button_font_all);
    GUIBUTTON_SetBg(MMIAPPLET_MANAGER_BOTTON_ID, &bg_btn);
    GUI_FillRect(&lcd_dev_info, clientwin_rect, MMI_WHITE_COLOR);
    GUILIST_SetRect(MMIAPPLET_MANAGER_LIST_ID, &list_rect);
    GUIBUTTON_SetRect(MMIAPPLET_MANAGER_BOTTON_ID, &button1_rc);
}

/*****************************************************************************/
//  Description : clean systerm buffer in manager window
//  Global resource dependence : 
//  Author: yanxian.zhou
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E CleanBufferButtonCallBack()
{
    MMI_HANDLE_T applet_handle = MMK_GetFirstAppletHandle(); 
    MMIPUB_OpenProgressWinEx(applet_handle, PNULL, PNULL, PNULL, MMIPUB_SOFTKEY_CUSTOMER, HandleCleanProcessWinMsg);
    MMIPUB_SetProgressTotalSize(PNULL, MMI_CLEANTASK_TOTAL_NUM);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : to handle the message of clean buffer progress window    
//  Global resource dependence : 
//  Author:yanxian.zhou
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCleanProcessWinMsg(
                                            MMI_WIN_ID_T     win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM           param
                                            )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    static uint32 num_counted = 0;
    MMI_STRING_T    title_str = {0};
    static uint32 old_tick_count = 0;
    uint32 cur_tick_count = 0;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        old_tick_count = 0;
        MMIRES_GetText(TXT_APPLICATION_CLEAN, PNULL, &title_str);
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE); 
        MMIPUB_SetWinTitleText(win_id, &title_str, FALSE);
        result = MMIPUB_HandleProgressWinMsg(win_id, msg_id, param);
        break;
      
    case MSG_TIMER: 
        num_counted = num_counted + 10;
        if(MMI_CLEANTASK_TOTAL_NUM < num_counted)//CR140661
        {
            MMK_CloseWin(win_id);
            num_counted = 0;
			break;
        }
        MMIPUB_UpdateProgressBarEx(PNULL, num_counted, TRUE);
        break;

    case MSG_APP_RED:    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        num_counted = 0;
        break;
    case MSG_LOSE_FOCUS:
        old_tick_count = SCI_GetTickCount();
        result = MMIPUB_HandleProgressWinMsg(win_id, msg_id, param);
        break;
    case MSG_GET_FOCUS:
        cur_tick_count = SCI_GetTickCount();
        if(cur_tick_count > old_tick_count)
        {
            num_counted += (cur_tick_count - old_tick_count) * 10 / MMI_500MSECONDS;
            
            if(MMI_CLEANTASK_TOTAL_NUM < num_counted)//CR140661
            {
                MMK_CloseWin(win_id);
                num_counted = 0;
                break;
            }
        }
        MMIPUB_UpdateProgressBarEx(PNULL, num_counted, TRUE);
        result = MMIPUB_HandleProgressWinMsg(win_id, msg_id, param);                        
        break;
    default:
        result = MMIPUB_HandleProgressWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (result);
}
#endif
/*****************************************************************************/
//  Description : search static applet guid
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: TRUE success, FALSE fail
/*****************************************************************************/
// LOCAL BOOLEAN SearchStaticAppletGuid( 
//                                      const MMI_LINK_NODE_T* node_ptr, 
//                                      uint32 condition1, 
//                                      uint32 condition2
//                                      )
// {
// 	if( condition1 == (uint32)((CAF_STATIC_APPLET_T*)node_ptr->data)->guid )
// 	{
// 		return TRUE;
// 	}
// 
// 	return FALSE;
// }

/*****************************************************************************/
//  Description : append applet list item
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AppendAppletListItem(
                                   MMI_CTRL_ID_T           ctrl_id,
                                   MMI_APPLET_INFO_LIST_T* applet_list_ptr,
                                   uint16                  item_index,
                                   uint16                  content_index,
                                   BOOLEAN                 is_item_data
                                   )
{
    BOOLEAN                 result      = FALSE;
    uint32  applet_list_index = 0;

    GUILIST_GetItemData(ctrl_id, item_index, &applet_list_index);
    if ( PNULL != applet_list_ptr )
    {
        //CAF_GUID_T          guid        = applet_list_ptr->list_ptr[applet_list_index].guid;
        //MMI_MODINFO_NODE_T* modinfo_ptr = MMK_SearchModInfo( guid, MMI_QUERY_BY_GUID );
        GUILIST_ITEM_DATA_T item_data   = {0};
        wchar   str[MMI_APPLET_DISPLAY_TIME_MAX_LEN + 1] = {0};
        const CAF_STATIC_APPLET_T* static_applet_ptr = MMK_GetStaticAppletPtr( applet_list_ptr->list_ptr[applet_list_index].guid );

        if ( PNULL != static_applet_ptr )
        {
            item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[0].item_data.image_id = static_applet_ptr->image_id;
            
            item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[1].item_data.text_id = static_applet_ptr->text_id;
            
            GetRunningTime(static_applet_ptr->guid, str, MMI_APPLET_DISPLAY_TIME_MAX_LEN);
            
            item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[3].item_data.text_buffer.wstr_ptr = str;
            item_data.item_content[3].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(str);
            if (MMK_ConvertIdToHandle(static_applet_ptr->guid) != MMK_GetFirstAppletHandle())//fisrt applet(idle) can't be stoped
            {
                item_data.item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
#ifdef MMI_PDA_SUPPORT
		  item_data.item_content[2].item_data.image_id = IMAGE_COMMON_OPTMENU_DELETE;
#endif
            }
        }

//         if ( PNULL != modinfo_ptr
//             && CAF_MOD_STATIC == modinfo_ptr->mod_type )
//         {
//             MMI_LINK_NODE_T* node_ptr = MMILINK_SearchNode( modinfo_ptr->module_info.class_info_ptr, TRUE,
//                 SearchStaticAppletGuid, guid, 0 );
// 
//             if ( PNULL != node_ptr )
//             {
//                 CAF_STATIC_APPLET_T* static_applet_ptr = (CAF_STATIC_APPLET_T*)node_ptr->data;
// 
//                 if ( PNULL != static_applet_ptr )
//                 {
//                     item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
//                     item_data.item_content[0].item_data.image_id = static_applet_ptr->image_id;
//                     
//                     item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
//                     item_data.item_content[1].item_data.text_id = static_applet_ptr->text_id;
// 
//                     GetRunningTime(static_applet_ptr->guid, str, MMI_APPLET_DISPLAY_TIME_MAX_LEN);
//                     
//                     item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
//                     item_data.item_content[3].item_data.text_buffer.wstr_ptr = str;
//                     item_data.item_content[3].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(str);
//                     
//                     if (MMK_ConvertIdToHandle(static_applet_ptr->guid) != MMK_GetFirstAppletHandle())//fisrt applet(idle) can't be stoped
//                     {
//                         item_data.item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
// #ifdef MMI_PDA_SUPPORT
//                         item_data.item_content[2].item_data.image_id = IMAGE_COMMON_OPTMENU_DELETE;
// #endif
//                     }
//                 }
//             }
//         }

        if ( is_item_data )
        {
            //add item text etc.
            result = GUILIST_SetItemData( ctrl_id,&item_data,item_index );
        }
        else
        {
            //add item anim
            result = GUILIST_SetItemContent( ctrl_id, &item_data.item_content[content_index],item_index, content_index );
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : append applet list
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void AppendAppletList(
                            MMI_CTRL_ID_T           ctrl_id,
                            MMI_APPLET_INFO_LIST_T* applet_list_ptr
                            )
{
    if ( PNULL != applet_list_ptr )
    {
        uint32          i = 0;
        //uint32          j = 0;
        GUILIST_ITEM_T  item = {0};
        //CAF_GUID_T          guid = 0;
        //MMI_MODINFO_NODE_T  *modinfo_ptr = PNULL;
        //MMI_LINK_NODE_T *node_ptr = PNULL;
        const CAF_STATIC_APPLET_T *static_applet_ptr = PNULL;
        
        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
        GUILIST_SetMaxItem( ctrl_id, applet_list_ptr->num_of_list, TRUE );

        //set item style
        item.item_style =  GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ANIM;
        item.item_state = GUIITEM_STATE_CONTENT_CHECK;
        for ( i = 0; i < applet_list_ptr->num_of_list; i++ )
        {
            static_applet_ptr = MMK_GetStaticAppletPtr( applet_list_ptr->list_ptr[i].guid );

            if ( PNULL != static_applet_ptr
                && MMK_GetAppletState( applet_list_ptr->list_ptr[i].handle, MMI_APPLET_STATE_TASK_MANAGER ) )
            {
                item.user_data = i;
                GUILIST_AppendItem( ctrl_id, &item );

//                 for ( j = 0; j < sizeof(s_applet_gui_id_arr)/sizeof(s_applet_gui_id_arr[0]); j++ )
//                 {
//                     if (s_applet_gui_id_arr[j] == static_applet_ptr->guid)
//                     {
//                         item.user_data = i;
//                         GUILIST_AppendItem( ctrl_id, &item );
//                         break;
//                     }
//                 }
            }
            
//             modinfo_ptr = MMK_SearchModInfo( guid, MMI_QUERY_BY_GUID );
//             
//             if ( PNULL != modinfo_ptr
//                 && CAF_MOD_STATIC == modinfo_ptr->mod_type )
//             {
//                 node_ptr = MMILINK_SearchNode( modinfo_ptr->module_info.class_info_ptr, TRUE,
//                         SearchStaticAppletGuid, guid, 0 );
// 
//                 if ( PNULL != node_ptr )
//                 {
//                     static_applet_ptr = (CAF_STATIC_APPLET_T*)node_ptr->data;
//                     if ( PNULL != static_applet_ptr )
//                     {
//                         for ( j = 0; j < sizeof(s_applet_gui_id_arr)/sizeof(s_applet_gui_id_arr[0]); j++ )
//                         {
//                             if (s_applet_gui_id_arr[j] == static_applet_ptr->guid)
//                             {
//                                 item.user_data = i;
//                                 GUILIST_AppendItem( ctrl_id, &item );
//                                 break;
//                             }
//                         }
//                     }
//                 }
//             }    
        }
    }

    if (GUILIST_GetTotalItemNum(ctrl_id) > 0)
    {
#ifdef CMCC_UI_STYLE
        GUIWIN_SetSoftkeyTextId(MMIAPPLET_MANAGER_LIST_WIN_ID, STXT_OPTION, TXT_ACTIVE, STXT_RETURN, FALSE);
#else
        GUIWIN_SetSoftkeyTextId(MMIAPPLET_MANAGER_LIST_WIN_ID, TXT_CLOSE, TXT_ACTIVE, STXT_RETURN, FALSE);
#endif
    }
    else
    {
        GUIWIN_SetSoftkeyTextId(MMIAPPLET_MANAGER_LIST_WIN_ID, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE); 
    }
}

/*****************************************************************************/
//  Description : set applet active
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void SetAppletActive(
                           MMI_CTRL_ID_T           ctrl_id,
                           MMI_APPLET_INFO_LIST_T* applet_list_ptr
                           )
{
    if ( PNULL != applet_list_ptr )
    {
        uint16 index = GUILIST_GetCurItemIndex( ctrl_id );
        MMI_HANDLE_T       handle = 0;
        uint32  applet_list_index = 0;

        GUILIST_GetItemData(ctrl_id, index, &applet_list_index);

        handle = applet_list_ptr->list_ptr[applet_list_index].handle;
        if (handle != MMK_GetFirstAppletHandle())//fisrt applet(idle)
        {
            MMK_GrabWinFocusByApplet( handle );

            MMK_PostMsg( handle, MSG_APPLET_SWITCH, PNULL, 0 );
        }
    }
}

/*****************************************************************************/
//  Description : set applet hide
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void SetAppletHide(
                         MMI_CTRL_ID_T           ctrl_id,
                         MMI_APPLET_INFO_LIST_T* applet_list_ptr
                         )
{
    if ( PNULL != applet_list_ptr )
    {
        uint16 index = GUILIST_GetCurItemIndex( ctrl_id );
        MMI_HANDLE_T       handle = 0;
        uint32  applet_list_index = 0;

        GUILIST_GetItemData(ctrl_id, index, &applet_list_index);

        handle = applet_list_ptr->list_ptr[applet_list_index].handle;
        if (handle != MMK_GetFirstAppletHandle())//fisrt applet(idle)
        {
            MMK_HideWinByApplet( handle );
        }
    }
}

/*****************************************************************************/
//  Description : set applet close
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void SetAppletClose(
                           MMI_CTRL_ID_T           ctrl_id,
                           MMI_APPLET_INFO_LIST_T* applet_list_ptr
                           )
{
    if ( PNULL != applet_list_ptr )
    {
        uint16 index = GUILIST_GetCurItemIndex( ctrl_id );
        MMI_HANDLE_T       handle = 0;
        uint32  applet_list_index = 0;

        GUILIST_GetItemData(ctrl_id, index, &applet_list_index);

        handle = applet_list_ptr->list_ptr[applet_list_index].handle;
        if (handle != MMK_GetFirstAppletHandle())//fisrt applet(idle)
        {
#ifdef MMI_CALL_MINIMIZE_SUPPORT        
            if(handle == MMIAPICC_GetCCAppletHandle())
            {
                MMK_PostMsg( handle, MSG_CC_RELEASE_ALL_CALL, PNULL, 0 );
            }
            else
#endif                
            {
                MMK_CloseApplet(handle);
            }
            //MMK_PostMsg( applet_list_ptr->list_ptr[index].handle, MSG_APPLET_SWITCH, PNULL, 0 );
        }
    }
}

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Description : close all running application
//  Global resource dependence : 
//  Author: murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL void SetAppletAllClose(
                           MMI_APPLET_INFO_LIST_T* applet_list_ptr
                           )
{
    if ( PNULL != applet_list_ptr )
    {
        uint16 total_item_num = GUILIST_GetTotalItemNum( MMIAPPLET_MANAGER_LIST_ID );
        int32   i = 0;
        
        for ( ; i < total_item_num; i++ )
        {
                MMI_HANDLE_T       handle = 0;
                uint32  applet_list_index = 0;

                GUILIST_GetItemData(MMIAPPLET_MANAGER_LIST_ID, i, &applet_list_index);

                handle = applet_list_ptr->list_ptr[applet_list_index].handle;
                if (handle != MMK_GetFirstAppletHandle())//fisrt applet(idle)
                {
#ifdef MMI_CALL_MINIMIZE_SUPPORT        
                    if(handle == MMIAPICC_GetCCAppletHandle())
                    {
                        MMK_PostMsg( applet_list_ptr->list_ptr[i].handle, MSG_CC_RELEASE_ALL_CALL, PNULL, 0 );
                    }
                    else
#endif                
                    {
                        MMK_CloseApplet(handle);
                    }
                    //MMK_PostMsg( applet_list_ptr->list_ptr[index].handle, MSG_APPLET_SWITCH, PNULL, 0 );
                }
        }
    }
}
#endif

/*****************************************************************************/
//  Description : turn second into tm struct
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_APPLET_TIME_T Second2Time(uint32 second)
{
    MMI_APPLET_TIME_T tm = {0};
    uint32 rem = 0;
    
    //tm.tm_sec = second % 60;
    rem = second / 60;
    tm.tm_min = rem % 60;
    tm.tm_hour /= 60;//>24,without days

    return tm;
}

/*****************************************************************************/
//  Description : get running time of one applet
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void GetRunningTime(MMI_HANDLE_T applet_handle, wchar *str_ptr, uint16  str_max_len)
{
    SCI_TICK_TIME_T tick_time = {0};
    MMI_APPLET_TIME_T tm = {0};
    uint8   timestr[MMI_APPLET_DISPLAY_TIME_MAX_LEN + 1] = {0};
    uint32  start_time = 0;

    start_time = MMK_GetAppletStartTime(applet_handle);
    SCI_GetTickTime(&tick_time);

    if (tick_time.second > start_time)
    {
        tm = Second2Time(tick_time.second - start_time);
        sprintf((char*)timestr,"%02d:%02d", tm.tm_hour, tm.tm_min);
        MMI_STRNTOWSTR(str_ptr, str_max_len, 
            timestr, MMI_APPLET_DISPLAY_TIME_MAX_LEN, 
            MIN(str_max_len, MMI_APPLET_DISPLAY_TIME_MAX_LEN) );
    }
    else
    {
        //SCI_TRACE_LOW:"GetRunningTime: start_time =%d, tick_time.second =%d, "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_APPLET_MANAGER_595_112_2_18_2_56_22_0,(uint8*)"dd", start_time, tick_time.second );
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void HandleTimer(uint8 timer_id, uint32 param)
{
    if ( PNULL != s_applet_list_ptr )
    {
        if (GUILIST_GetTotalItemNum(MMIAPPLET_MANAGER_LIST_ID) > 0)
        {
            GUILIST_RemoveAllItems(MMIAPPLET_MANAGER_LIST_ID);
        }

        AppendAppletList( MMIAPPLET_MANAGER_LIST_ID, s_applet_list_ptr );
        if (MMK_IsFocusWin(MMIAPPLET_MANAGER_LIST_WIN_ID))
        {
            MMK_SendMsg(MMIAPPLET_MANAGER_LIST_WIN_ID, MSG_FULL_PAINT, PNULL);
        }

    }
}
#endif/*MMI_TASK_MANAGER*/



/*Edit by script, ignore 1 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
