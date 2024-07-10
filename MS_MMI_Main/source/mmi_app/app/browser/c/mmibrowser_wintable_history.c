/*****************************************************************************
** File Name:      mmibrowser_wintable_history.c                             *
** Author:          li.li                                                    *
** Date:           23/06/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser window table    *
*****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_browser_trc.h"
#include "window_parse.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "guilistbox.h"
#include "guimenu.h"
#include "mmi_mainmenu_export.h"
#include "mmipub.h"
#include "guirichtext.h"
#include "mmibrowser_id.h"
#include "browser_text.h"
#include "mmibrowser_menutable.h"
#include "mmi_appmsg.h"
#include "cfl_wchar.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmibrowser_export.h"
#include "browser_image.h"
#include "mmisms_export.h"
#include "mmibrowser_setting.h"
#include "mmibrowser_func.h"
#include "mmi_resource.h"
#include "guitab.h"
#ifdef BROWSER_START_PAGE_THEME1
#include "mmi_slide.h"
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
 //记录当前history列表信息的数据结构(与control关联)
LOCAL BRW_VISIT_HIS_INDEX_QUE_T    s_index_que = {0};
#ifdef BROWSER_START_PAGE_THEME1
LOCAL BOOLEAN   s_is_history_need_update = FALSE;
LOCAL BOOLEAN s_is_need_restore_list_slide = FALSE; //是否需要恢复list的slide状态
#endif

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle history list window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleHistoryListWinMsg(
                                           MMI_WIN_ID_T     win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM           param
                                           );

/*****************************************************************************/
//  Description : handle history menu window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleHistoryMenuWinMsg(
                                           MMI_WIN_ID_T     win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM           param
                                           );

/*****************************************************************************/
//  Description : handle history detail window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleHistoryDetailWinMsg(
                                               MMI_WIN_ID_T     win_id,
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM           param
                                               );

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : handle bookmark detail popmenu window message
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleHistoryPopUpWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  BrwCreateHistoryPopUpMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void BrwOpenHistoryPropertyWin(BRW_VISIT_HIS_INFO_T *visit_info_ptr);

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  BrwHandleHistoryAlertWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

#endif /* MMI_PDA_SUPPORT */                                               
/*****************************************************************************/
//  Description : delete one history
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN BrwDelOneHistory(MMI_HANDLE_T list_ctrl);

/*****************************************************************************/
//	Description : the handle function of history list delete all menu window
//	Global resource dependence : 
//	Author:fen.xie
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleHistoryDeleteAllQueryWinMsg(
                MMI_WIN_ID_T win_id, 
                MMI_MESSAGE_ID_E msg_id, 
                DPARAM param
                );

/**--------------------------------------------------------------------------*
 **                         WINDOW DEFINITION                              *
 **--------------------------------------------------------------------------*/
//window define
WINDOW_TABLE(BRW_HISTORYLIST_WIN_TAB) = 
{
    WIN_FUNC((uint32)BrwHandleHistoryListWinMsg),  
#ifndef BROWSER_BOOKMARK_HISTORY_TAB
    WIN_TITLE(TXT_COMMON_HISTORY),
#endif	
    WIN_ID(MMIBROWSER_HISTORYLIST_WIN_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif    
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIBROWSER_HISTORY_LIST_CTRL_ID),
    
    END_WIN
};

WINDOW_TABLE(BRW_HISTORY_DETAIL_WIN_TAB) = 
{
    WIN_FUNC((uint32)BrwHandleHistoryDetailWinMsg),
    WIN_ID(MMIBROWSER_HISTORY_DETAIL_WIN_ID),
    WIN_TITLE(TXT_COMMON_HISTORY),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif /* MMI_PDA_SUPPORT */
    WIN_SOFTKEY(TXT_COMM_OPEN, TXT_NULL, STXT_RETURN),
    //参数为测试用
    CREATE_RICHTEXT_CTRL(MMIBROWSER_RICHTEXT_HIS_DETAIL_CTRL_ID),
    END_WIN
};

WINDOW_TABLE( MMIBROWSER_HISTORYMENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)BrwHandleHistoryMenuWinMsg),
    WIN_ID(MMIBROWSER_HISTORYMENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_HIDE_STATUS,
    CREATE_POPMENU_CTRL(MENU_BRW_HISTORY,MMIBROWSER_MENU_HISTORY_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

#ifdef MMI_PDA_SUPPORT

WINDOW_TABLE(MMIBROWSER_HISTORY_POPUP_WIN_TAB) = 
{
    WIN_FUNC((uint32) BrwHandleHistoryPopUpWinMsg), 
    WIN_ID(MMIBROWSER_HISTORY_POPUP_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

#endif /* MMI_PDA_SUPPORT */

/*****************************************************************************/
//  Description : handle history list window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleHistoryListWinMsg(
                                           MMI_WIN_ID_T	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM param
                                           )
{
    uint16  list_index=0;    
    BRW_VISIT_HIS_INFO_T    *temp_visit_info_ptr=PNULL;
    MMI_CTRL_ID_T   list_ctrl_id = MMIBROWSER_HISTORY_LIST_CTRL_ID;
    MMI_RESULT_E    ret = MMI_RESULT_TRUE;
    BRW_VISIT_HIS_INFO_T    *visit_his_info_ptr = PNULL;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
#ifdef BROWSER_START_PAGE_THEME1
            MMIBROWSER_SetIsHistoryUpdate(FALSE);//init
#endif
            GUILIST_SetMaxItem(list_ctrl_id, BRW_MAX_VISIT_HIS_ITEM_NUM, FALSE);
            
            GUILIST_SetTitleIndexType( list_ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
#ifdef MMI_PDA_SUPPORT
#ifndef BROWSER_BOOKMARK_HISTORY_TAB					
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_DELALL, FALSE);
#endif
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, list_ctrl_id);

#ifdef BROWSER_START_PAGE_THEME1
            if (!MMK_IsOpenWin(MMIBROWSER_MAIN_WIN_ID))
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_NORMAL, FALSE);
                GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_EXIT, FALSE);
            }
#endif
#endif
            
            MMIBROWSER_UpdateHistoryList(win_id);
            
            MMK_SetAtvCtrl(win_id,list_ctrl_id);  
        }
        break;
        
#ifdef BROWSER_START_PAGE_THEME1
    case MSG_SLIDE_MOVE_BEGIN:
        if (GUILIST_GetSlideState(list_ctrl_id))
        {
            GUILIST_SetSlideState(list_ctrl_id, FALSE);
            s_is_need_restore_list_slide = TRUE;
        }
        break;
    case MSG_SLIDE_MOVE_END:
        if (s_is_need_restore_list_slide)
        {
            GUILIST_SetSlideState(list_ctrl_id, TRUE);
            s_is_need_restore_list_slide = FALSE;
        }
        break;
        
    case MSG_FULL_PAINT:
        if  ((MMIBROWSER_TAB1_HISTORY == GUITAB_GetCurSel(MMIBROWSER_BK_AND_HISTORY_TAB_CTRL_ID))
            && MMIBROWSER_GetIsHistoryUpdate())
        {
            MMIBROWSER_UpdateHistoryList(MMIBROWSER_HISTORYLIST_WIN_ID);
            MMIBROWSER_SetIsHistoryUpdate(FALSE);
        }
        break;
#endif        
        
    case MSG_GET_FOCUS:
    {
        MMI_TEXT_ID_T text3_id = STXT_RETURN;

#ifdef BROWSER_START_PAGE_THEME1
        if (!MMK_IsOpenWin(MMIBROWSER_MAIN_WIN_ID))
        {
            text3_id = STXT_EXIT;
        }
#endif            

        if(GUILIST_GetTotalItemNum(list_ctrl_id) == 0)
        {
			GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, text3_id, FALSE);
        }
#ifdef MMI_PDA_SUPPORT
        if (GUILIST_GetTotalItemNum(list_ctrl_id) > 0)
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }

#ifdef BROWSER_START_PAGE_THEME1
        GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, text3_id, FALSE);
#endif
#endif
        }
        break;
        
#ifdef MMI_PDA_SUPPORT
    case MSG_NOTIFY_TITLE_MENU_ICON:
        if (GUILIST_GetTotalItemNum(list_ctrl_id) > 0)
        {
            MMIBROWSER_OpenQueryWinByTextId(TXT_BROWSER_INQUIREDELETEALL,IMAGE_PUBWIN_QUERY,PNULL,BrwHandleHistoryDeleteAllQueryWinMsg);
        }
        break;
#endif
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        if (GUILIST_GetTotalItemNum(list_ctrl_id) > 0)
        {                
            list_index=GUILIST_GetCurItemIndex(list_ctrl_id);
            
            visit_his_info_ptr = (BRW_VISIT_HIS_INFO_T *)SCI_ALLOCA(sizeof(BRW_VISIT_HIS_INFO_T));
            if (PNULL == visit_his_info_ptr)
            {
                //SCI_TRACE_LOW:"\n MMIBROWSER BrwHandleHistoryListWinMsg:malloc mem failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_HISTORY_290_112_2_18_2_6_36_288,(uint8*)"");
                return FALSE;
            }
            SCI_MEMSET(visit_his_info_ptr, 0x00, sizeof(BRW_VISIT_HIS_INFO_T));
            
            temp_visit_info_ptr = BRW_VisitHisGetHisInfoByIndex(s_index_que.index_arr[list_index]);
            if (PNULL == temp_visit_info_ptr)
            {
                SCI_FREE(visit_his_info_ptr);//need to free
                //SCI_TRACE_LOW:"\n MMIBROWSER BrwHandleHistoryListWinMsg BRW_VisitHisGetHisInfoByIndex error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_HISTORY_299_112_2_18_2_6_36_289,(uint8*)"");
                break;
            }
            SCI_MEMCPY(visit_his_info_ptr, temp_visit_info_ptr, sizeof(BRW_VISIT_HIS_INFO_T));
            
            MMK_CreateWin((uint32*)MMIBROWSER_HISTORYMENU_WIN_TAB,visit_his_info_ptr); 
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
#ifdef BROWSER_START_PAGE_THEME1
        if (MMK_IsOpenWin(MMIBROWSER_START_PAGE_WIN_ID))
        {
            if (MMK_IsOpenWin(MMIBROWSER_MAIN_WIN_ID))
            {
                MMK_WinGrabFocus(MMIBROWSER_MAIN_WIN_ID);
            }
            else
            {
                MMIBROWSER_QueryExit();
            }
        }
        else
#endif
        {
            MMIBROWSER_CloseHisListWin();
        }
        break;
        
#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_LONGOK:
        if (BRW_VisitHisGenIndexArr(&s_index_que) > 0)
        {
            MMK_CreateWin((uint32*)MMIBROWSER_HISTORY_POPUP_WIN_TAB, PNULL);
        }
        break;
#endif /* MMI_PDA_SUPPORT */
        
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if(GUILIST_GetTotalItemNum(list_ctrl_id) > 0)//@fen.xie MS00186092
        {                
            list_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            temp_visit_info_ptr = BRW_VisitHisGetHisInfoByIndex(s_index_que.index_arr[list_index]);
            
            if (PNULL == temp_visit_info_ptr)
            {
                //SCI_TRACE_LOW:"[BRW]BrwHandleHistoryListWinMsg temp_visit_info_ptr = 0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_HISTORY_340_112_2_18_2_6_36_290,(uint8*)"d", temp_visit_info_ptr);
            }
            else
            {
                if (Brw_IsIncludeInvalidUrl(temp_visit_info_ptr->url_ptr))
                {
                    MMIBROWSER_AlertUrlInvalid();//alert the url is invalid
                }
                else
                {
                    MMIBROWSER_StartWeb(MMIBROWSER_WEB_ENTRY_URL, temp_visit_info_ptr->url_ptr, PNULL);
#ifdef BROWSER_START_PAGE_THEME1
                    if (MMK_IsOpenWin(MMIBROWSER_START_PAGE_WIN_ID))
                    {
                        //don't close the window
                    }
                    else
#endif
                    {
                        MMIBROWSER_CloseHisListWin();
                    }
                }
            }
        }
        break;
        
    default:
        ret = MMI_RESULT_FALSE;
        break;
    }
    
    return ret;
}

/*****************************************************************************/
//  Description : open history list window
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenHisListWin(void)
{
#ifdef BROWSER_BOOKMARK_HISTORY_TAB
    MMIBROWSER_OpenBkHistoryWindow(MMIBROWSER_TAB1_HISTORY, PNULL);
#else	
    MMK_CreateWin((uint32*)BRW_HISTORYLIST_WIN_TAB,PNULL);
#endif
}

/*****************************************************************************/
//  Description : close history list window
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_CloseHisListWin(void)
{
#ifdef BROWSER_BOOKMARK_HISTORY_TAB
    MMK_CloseWin(MMK_GetParentWinHandle(MMIBROWSER_HISTORYLIST_WIN_ID));
#else
    MMK_CloseWin(MMIBROWSER_HISTORYLIST_WIN_ID);
#endif     
}

#ifdef BROWSER_BOOKMARK_HISTORY_TAB
/*****************************************************************************/
//  Description : open the history child window
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_CreateChildHistoryWin(MMI_WIN_ID_T parent_win_id, ADD_DATA add_data_ptr)
{
	MMK_CreateChildWin(parent_win_id, (uint32*)BRW_HISTORYLIST_WIN_TAB, add_data_ptr);
}
#endif


/*****************************************************************************/
//  Description : handle history menu window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleHistoryMenuWinMsg(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM param
                                           )
{
    MMI_MENU_GROUP_ID_T			        group_id = 0;
    MMI_MENU_ID_T			        menu_id = 0;
    MMI_RESULT_E	        ret = MMI_RESULT_TRUE;
    BRW_VISIT_HIS_INFO_T*   visit_info_ptr = (BRW_VISIT_HIS_INFO_T*)MMK_GetWinAddDataPtr(win_id);
    MMI_CTRL_ID_T           ctrl_id = MMIBROWSER_MENU_HISTORY_CTRL_ID;
    //     uint16                  item_index = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(ctrl_id, &group_id,  &menu_id);  
        
        if (PNULL == visit_info_ptr)//handle error
        {
            //SCI_TRACE_LOW:"[BRW] BrwHandleHistoryMenuWinMsg menu_id = %d visit_info_ptr == PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_HISTORY_440_112_2_18_2_6_36_291,(uint8*)"d", menu_id);
            MMIPUB_OpenAlertFailWin(TXT_UNKNOWN_ERROR);
            break;
        }
        
        switch(menu_id)
        {
        case ID_POPUP_MENU_HISTORY_ENTER:
            if (Brw_IsIncludeInvalidUrl(visit_info_ptr->url_ptr))
            {
                MMIBROWSER_AlertUrlInvalid();//alert the url is invalid
                MMK_CloseWin(win_id);
            }
            else
            {
                MMIBROWSER_StartWeb(MMIBROWSER_WEB_ENTRY_URL, visit_info_ptr->url_ptr, PNULL);
                MMK_CloseWin(win_id);
#ifdef BROWSER_START_PAGE_THEME1
                if (MMK_IsOpenWin(MMIBROWSER_START_PAGE_WIN_ID))
                {
                    //don't close the window
                }
                else
#endif
                {
                    MMIBROWSER_CloseHisListWin();
                }            
            }
            break;
            
        case ID_POPUP_MENU_HISTORY_DETAIL:
            {
                BRW_VISIT_HIS_INFO_T    *new_visit_info_ptr = PNULL;
                
                new_visit_info_ptr = (BRW_VISIT_HIS_INFO_T*)SCI_ALLOCA(sizeof(BRW_VISIT_HIS_INFO_T));
                if (PNULL != new_visit_info_ptr)
                {
                    //SCI_MEMSET(new_visit_info_ptr, 0x00, sizeof(BRW_VISIT_HIS_INFO_T));
                    SCI_MEMCPY(new_visit_info_ptr, visit_info_ptr, sizeof(BRW_VISIT_HIS_INFO_T));/*lint !e668*/
                    MMK_CreateWin((uint32*)BRW_HISTORY_DETAIL_WIN_TAB,new_visit_info_ptr);
                    MMK_CloseWin(win_id);
                }      
                else//ERROR
                {
                    //SCI_TRACE_LOW:"[BRW] ID_POPUP_MENU_HISTORY_DETAIL ALLOC FAIL"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_HISTORY_477_112_2_18_2_6_36_292,(uint8*)"");
                    MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
                }
            }            
            break;
            
        case ID_POPUP_MENU_HISTORY_DELETE:
            {
                MMI_STRING_T                dst_temp1 = {0};
                MMI_STRING_T                dst_temp2 = {0};
                
                MMI_GetLabelTextByLang(TXT_COMMON_CONFIRM_DELETE, &dst_temp1);
                dst_temp2.wstr_len = visit_info_ptr->name_len;
                dst_temp2.wstr_ptr = visit_info_ptr->name_ptr;
                MMIBROWSER_OpenQueryWinByTextPtr(&dst_temp1,&dst_temp2,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            }
            break;
            
        case ID_POPUP_MENU_HISTORY_SEND_URL:
            {
                uint16  title[MMIBRW_BOOKMARK_MAX_TITLE_LENGTH + 1] = {0};
                uint16  title_len = 0;
                wchar   url[MMIBROWSER_MAX_URL_LEN + 1] = {0};
                uint16  url_len = 0;
                
                title_len = MMIAPICOM_Wstrlen(visit_info_ptr->name_ptr);
                MMI_WSTRNCPY((wchar *) title, 
                    MMIBRW_BOOKMARK_MAX_TITLE_LENGTH, 
                    visit_info_ptr->name_ptr,
                    title_len,
                    MIN(title_len,MMIBROWSER_MAX_URL_LEN));
                title_len = MMIAPICOM_Wstrlen(title);
                
                url_len = strlen((char*)visit_info_ptr->url_ptr);
                MMI_STRNTOWSTR((wchar *)url, 
                    MMIBROWSER_MAX_URL_LEN, 
                    (const uint8 *)visit_info_ptr->url_ptr,
                    url_len,
                    MIN(url_len,MMIBROWSER_MAX_URL_LEN));
                url_len = MMIAPICOM_Wstrlen(url);
                
                if(MMIBROWSER_SendUrlContentBySms(title, title_len, url, url_len))
                {
                    //successful
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
                }
                
                MMK_CloseWin(win_id);
            }
            break;
            
        case ID_POPUP_MENU_HISTORY_DELETEALL:
            MMIBROWSER_OpenQueryWinByTextId(TXT_BROWSER_INQUIREDELETEALL,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;
            
        case ID_POPUP_MENU_HISTORY_AS_HOMEPAGE:
            {
                MMI_STRING_T    url_str = {0};
                wchar   url[MMIBROWSER_MAX_URL_LEN + 1] = {0};
                
                CFLWCHAR_StrNCpyWithAsciiStr(url,(char*)visit_info_ptr->url_ptr,MMIBROWSER_MAX_URL_LEN + 1);
                url_str.wstr_len = (uint16)CFLWCHAR_StrLen(url);
                url_str.wstr_ptr = url;
                
                if (MMIBROWSER_SetHomepageUrl(&url_str))
                {
#ifdef MMI_GUI_STYLE_TYPICAL
                    MMIPUB_OpenAlertSuccessWin(TXT_BROWSER_SET_HOMEPAGE_SUCC);
#endif /* MMI_GUI_STYLE_TYPICAL */
                }
                //            else
                //            {
                //                MMIPUB_OpenAlertWarningWin(TXT_BROWSER_SET_HOMEPAGE_FAIL);
                //            }
                MMK_CloseWin(win_id);
            }
            break;
            
#ifdef MMIDORADO_MY_NAV_SUPPORT
        case ID_POPUP_MENU_HISTORY_ADD2MYNAV:
            {
                MMI_STRING_T    url_str = {0};
                MMI_STRING_T    title_str = {0};
                wchar   url[MMIBROWSER_MAX_URL_LEN + 1] = {0};
                
                title_str.wstr_ptr = visit_info_ptr->name_ptr;
                title_str.wstr_len = visit_info_ptr->name_len;
                
                CFLWCHAR_StrNCpyWithAsciiStr(url,(char*)visit_info_ptr->url_ptr,MMIBROWSER_MAX_URL_LEN + 1);
                url_str.wstr_len = (uint16)CFLWCHAR_StrLen(url);
                url_str.wstr_ptr = url;
                
                MMIBROWSER_Add2MyNav(title_str, url_str);
                MMK_CloseWin(win_id);
            }
            break;
#endif
            
        default:
            break;
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_PROMPTWIN_OK:
        MMIBROWSER_CloseQuerytWin(PNULL);
        GUIMENU_GetId(ctrl_id, &group_id,  &menu_id);  
        switch(menu_id)
        {
        case ID_POPUP_MENU_HISTORY_DELETE:
            if (BrwDelOneHistory(MMIBROWSER_HISTORY_LIST_CTRL_ID))
            {
#ifdef MMI_GUI_STYLE_TYPICAL
                MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
#endif /* MMI_GUI_STYLE_TYPICAL */
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_DEL_FAIL);
            }
            break;
            
        case ID_POPUP_MENU_HISTORY_DELETEALL:
            MMIBROWSER_DeleteAllHistory();
            GUILIST_RemoveAllItems(MMIBROWSER_HISTORY_LIST_CTRL_ID);
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
#endif /* MMI_GUI_STYLE_TYPICAL */
            break;
            
        default:
            break;
        }
        MMK_CloseWin(win_id);
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIBROWSER_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData(win_id);//free visit_info_ptr
        break;
        
    default:
        ret=MMI_RESULT_FALSE;
    }
    return ret;
}

/*****************************************************************************/
//  Description : handle history detail window message
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleHistoryDetailWinMsg(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id,DPARAM	param)
{
    MMI_RESULT_E            ret=MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id = MMIBROWSER_RICHTEXT_HIS_DETAIL_CTRL_ID;
    BRW_VISIT_HIS_INFO_T    *visit_his_info_ptr = MMK_GetWinAddDataPtr(win_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            uint16              index = 0;
            wchar               temp_uint16_buf[MMIBROWSER_MAX_URL_LEN+1] ={0};
            GUIRICHTEXT_ITEM_T  item_data = {0};
            

            //TITLE
            item_data.text_type = GUIRICHTEXT_TEXT_RES;
            item_data.text_data.res.id = TXT_COMM_TITLE;
            GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

            //TITLE DETAIL
            SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
            if ((PNULL != visit_his_info_ptr) && (visit_his_info_ptr->name_len > 0))
            {
                item_data.text_type = GUIRICHTEXT_TEXT_BUF;
                item_data.text_data.buf.len = visit_his_info_ptr->name_len;
                item_data.text_data.buf.str_ptr=visit_his_info_ptr->name_ptr;
            }
            else//error
            {
                item_data.text_type = GUIRICHTEXT_TEXT_RES;
                item_data.text_data.res.id = TXT_NO_TITLE;
            }
            GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

            //URL
            SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
            item_data.text_type = GUIRICHTEXT_TEXT_RES;
            item_data.text_data.res.id = TXT_INPUT_URL_ADRESS;
            GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

            //URL DETAIL
            SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
            item_data.text_type = GUIRICHTEXT_TEXT_BUF;
            if (PNULL != visit_his_info_ptr)
            {
                CFLWCHAR_StrNCpyWithAsciiStr(temp_uint16_buf,(char*)(visit_his_info_ptr->url_ptr),MMIBROWSER_MAX_URL_LEN + 1);
                item_data.text_data.buf.len = MMIAPICOM_Wstrlen(temp_uint16_buf);
                item_data.text_data.buf.str_ptr = temp_uint16_buf;
            }
            else//error
            {
                //SCI_TRACE_LOW:"[BRW]BrwHandleHistoryDetailWinMsg MSG_OPEN_WINDOW visit_his_info_ptr == PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_HISTORY_664_112_2_18_2_6_36_293,(uint8*)"");
                item_data.text_data.buf.len = 1;
                item_data.text_data.buf.str_ptr = L" ";
            }
            GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
        }
        MMK_SetAtvCtrl(win_id,ctrl_id);    
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
        if (PNULL != visit_his_info_ptr)
        {
            if (Brw_IsIncludeInvalidUrl(visit_his_info_ptr->url_ptr))
            {
                MMIBROWSER_AlertUrlInvalid();//alert the url is invalid
            }
            else
            {
                MMIBROWSER_StartWeb(MMIBROWSER_WEB_ENTRY_URL, visit_his_info_ptr->url_ptr, PNULL);
                MMK_CloseWin(win_id);
#ifdef BROWSER_START_PAGE_THEME1
                if (MMK_IsOpenWin(MMIBROWSER_START_PAGE_WIN_ID))
                {
                    //don't close the window
                }
                else
#endif
                {
                    MMIBROWSER_CloseHisListWin();
                }
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[BRW]BrwHandleHistoryDetailWinMsg msg_id=%d, visit_his_info_ptr == PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_HISTORY_692_112_2_18_2_6_36_294,(uint8*)"d", msg_id);
        }
        break;

    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData(win_id);//free visit_his_info_ptr
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        {
            MMI_CTRL_ID_T       src_ctrl_id = 0;

            if (PNULL != param)
            {
                src_ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;

                if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                }
                else if (src_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
                {
                    MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                }
            }
        }
        break;
#endif /* MMI_PDA_SUPPORT */

    default:
        ret=MMI_RESULT_FALSE;
        break;
    }
    return ret;
}
/*****************************************************************************/
//  Description : delete one history
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN BrwDelOneHistory(MMI_HANDLE_T list_ctrl)
{
    uint16  cur_index = 0;
    uint16  total_index = 0;
    uint16  i=0;

    cur_index = GUILIST_GetCurItemIndex(list_ctrl);
    total_index = GUILIST_GetTotalItemNum(list_ctrl);

    if (cur_index >= total_index) //error
    {
        return FALSE;
    }
    
    if (BRW_VisitHisDelItem(s_index_que.index_arr[cur_index]))//delete the history info in control
    {
        for (i = cur_index; i < (total_index - 1);i++)//update the list info
        {
            s_index_que.index_arr[i] = s_index_que.index_arr[i + 1];
        }
        s_index_que.index_arr[i] = 0;
        
        GUILIST_RemoveItem(list_ctrl,cur_index);

        return TRUE;
    }
    else
    {
        return FALSE;
    }    
    
}

#ifdef MMI_PDA_SUPPORT

/*****************************************************************************/
//  Description : handle bookmark detail popmenu window message
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleHistoryPopUpWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"BrwHandleHistoryPopUpWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_HISTORY_781_112_2_18_2_6_37_295,(uint8*)"d", msg_id);

    switch (msg_id) {
    case MSG_OPEN_WINDOW:
        BrwCreateHistoryPopUpMenu(win_id, MMIBROWSER_HISTORY_POPUP_MENU_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIBROWSER_HISTORY_POPUP_MENU_CTRL_ID);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            MMI_MENU_ID_T           menu_id = 0;
            uint16                  list_index=0;
            BRW_VISIT_HIS_INFO_T    *visit_info_ptr = PNULL;

            menu_id = GUIMENU_GetCurNodeId(MMIBROWSER_HISTORY_POPUP_MENU_CTRL_ID);

            list_index = GUILIST_GetCurItemIndex(MMIBROWSER_HISTORY_LIST_CTRL_ID);

            visit_info_ptr = BRW_VisitHisGetHisInfoByIndex(s_index_que.index_arr[list_index]);

            if (PNULL == visit_info_ptr)
            {
                //SCI_TRACE_LOW:"BrwHandleHistoryPopUpWinMsg visit_info_ptr = %x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_HISTORY_807_112_2_18_2_6_37_296,(uint8*)"d", list_index);
                MMK_CloseWin(win_id);
                break;
            }

            switch (menu_id)
            {
            case ID_POPUP_MENU_HISTORY_DETAIL:
                //BrwOpenHistoryPropertyWin(visit_info_ptr);
                {
                    BRW_VISIT_HIS_INFO_T    *new_visit_info_ptr = PNULL;
                    
                    new_visit_info_ptr = (BRW_VISIT_HIS_INFO_T*)SCI_ALLOCA(sizeof(BRW_VISIT_HIS_INFO_T));
                    if (PNULL != new_visit_info_ptr)
                    {
                        //SCI_MEMSET(new_visit_info_ptr, 0x00, sizeof(BRW_VISIT_HIS_INFO_T));
                        SCI_MEMCPY(new_visit_info_ptr, visit_info_ptr, sizeof(BRW_VISIT_HIS_INFO_T));/*lint !e668*/
                        MMK_CreateWin((uint32*)BRW_HISTORY_DETAIL_WIN_TAB,new_visit_info_ptr);
                        MMK_CloseWin(win_id);
                    }      
                    else//ERROR
                    {
                        //SCI_TRACE_LOW:"[BRW] ID_POPUP_MENU_HISTORY_DETAIL ALLOC FAIL"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_HISTORY_829_112_2_18_2_6_37_297,(uint8*)"");
                        MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
                    }
                }
                MMK_CloseWin(win_id);
                break;

            case ID_POPUP_MENU_HISTORY_DELETE:
                {
                    MMI_STRING_T                dst_temp1 = {0};
                    MMI_STRING_T                dst_temp2 = {0};
                
                    MMI_GetLabelTextByLang(TXT_COMMON_CONFIRM_DELETE, &dst_temp1);
                    dst_temp2.wstr_len = visit_info_ptr->name_len;
                    dst_temp2.wstr_ptr = visit_info_ptr->name_ptr;
                    MMIBROWSER_OpenQueryWinByTextPtr(&dst_temp1,&dst_temp2,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                }
                break;

            case ID_POPUP_MENU_HISTORY_SEND_URL:
                {
                    uint16  title[MMIBRW_BOOKMARK_MAX_TITLE_LENGTH + 1] = {0};
                    uint16  title_len = 0;
                    wchar   url[MMIBROWSER_MAX_URL_LEN + 1] = {0};
                    uint16   url_len  = 0;

                    title_len  = MMIAPICOM_Wstrlen(visit_info_ptr->name_ptr);
                    MMI_WSTRNCPY((wchar *) title, 
                                                        MMIBRW_BOOKMARK_MAX_TITLE_LENGTH, 
                                                        visit_info_ptr->name_ptr,
                                                        title_len,
                                                        MIN(title_len,MMIBROWSER_MAX_URL_LEN));
                    title_len = MMIAPICOM_Wstrlen(title);
                   
                    url_len = strlen((char*)visit_info_ptr->url_ptr);
                    MMI_STRNTOWSTR((wchar *) url, 
                                                        MMIBROWSER_MAX_URL_LEN, 
                                                        (const uint8 *)visit_info_ptr->url_ptr,
                                                        url_len,
                                                        MIN(url_len,MMIBROWSER_MAX_URL_LEN));
                    url_len = MMIAPICOM_Wstrlen(url);
                    
                    if(MMIBROWSER_SendUrlContentBySms(title, title_len, url, url_len))
                    {
                        //successful
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
                    }

                    MMK_CloseWin(win_id);
                }
                break;
                
            case ID_POPUP_MENU_HISTORY_AS_HOMEPAGE:
            {
                MMI_STRING_T    url_str = {0};
                wchar   url[MMIBROWSER_MAX_URL_LEN + 1] = {0};
            
                CFLWCHAR_StrNCpyWithAsciiStr(url,(char*)visit_info_ptr->url_ptr,MMIBROWSER_MAX_URL_LEN + 1);
                url_str.wstr_len = (uint16)CFLWCHAR_StrLen(url);
                url_str.wstr_ptr = url;
               
                if (MMIBROWSER_SetHomepageUrl(&url_str))
                {
#ifdef MMI_GUI_STYLE_TYPICAL
                    MMIPUB_OpenAlertSuccessWin(TXT_BROWSER_SET_HOMEPAGE_SUCC);
#endif /* MMI_GUI_STYLE_TYPICAL */
                }
//                else
//                {
//                    MMIPUB_OpenAlertWarningWin(TXT_BROWSER_SET_HOMEPAGE_FAIL);
//                }
                MMK_CloseWin(win_id);
            }
                break;   
                
#ifdef MMIDORADO_MY_NAV_SUPPORT
            case ID_POPUP_MENU_HISTORY_ADD2MYNAV:
            {
                MMI_STRING_T    url_str = {0};
                MMI_STRING_T    title_str = {0};
                wchar   url[MMIBROWSER_MAX_URL_LEN + 1] = {0};
            
                title_str.wstr_ptr = visit_info_ptr->name_ptr;
                title_str.wstr_len = visit_info_ptr->name_len;
                    
                CFLWCHAR_StrNCpyWithAsciiStr(url,(char*)visit_info_ptr->url_ptr,MMIBROWSER_MAX_URL_LEN + 1);
                url_str.wstr_len = (uint16)CFLWCHAR_StrLen(url);
                url_str.wstr_ptr = url;
                
                MMIBROWSER_Add2MyNav(title_str, url_str);
                MMK_CloseWin(win_id);
            }
                break;
#endif

            default:
                break;
            }

            break;
        }

    case MSG_PROMPTWIN_OK:
        MMIBROWSER_CloseQuerytWin(PNULL);

        if (BrwDelOneHistory(MMIBROWSER_HISTORY_LIST_CTRL_ID))
        {
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
#endif /* MMI_GUI_STYLE_TYPICAL */
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_DEL_FAIL);
        }

        MMK_CloseWin(win_id);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIBROWSER_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
LOCAL void  BrwCreateHistoryPopUpMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    uint32                          num = 0;
    uint16                          index = 0;
    MMI_STRING_T                    title_str = {0};
    BRW_VISIT_HIS_INFO_T            *temp_visit_info_ptr=PNULL;

    MMI_TEXT_ID_T tex_index[] = 
        {
            TXT_DELETE, 
            TXT_WWW_SEND_LINK, 
#ifndef CMCC_UI_STYLE
            TXT_BROWSER_PDA_SAVE_AS_HOMEPAGE,
#endif
#ifdef MMIDORADO_MY_NAV_SUPPORT
            TXT_BROWSER_ADD_TO_NAV,
#endif
            TXT_COMM_DETAIL
        };
    
    uint32 node_index[] = 
        {
            ID_POPUP_MENU_HISTORY_DELETE, 
            ID_POPUP_MENU_HISTORY_SEND_URL, 
#ifndef CMCC_UI_STYLE
            ID_POPUP_MENU_HISTORY_AS_HOMEPAGE,
#endif
#ifdef MMIDORADO_MY_NAV_SUPPORT
            ID_POPUP_MENU_HISTORY_ADD2MYNAV,
#endif
            ID_POPUP_MENU_HISTORY_DETAIL
        };

    index = GUILIST_GetCurItemIndex(MMIBROWSER_HISTORY_LIST_CTRL_ID);

    BRW_VisitHisGenIndexArr(&s_index_que);

    temp_visit_info_ptr = BRW_VisitHisGetHisInfoByIndex(s_index_que.index_arr[index]);

    if (PNULL != temp_visit_info_ptr)
    {
        title_str.wstr_ptr = temp_visit_info_ptr->name_ptr;
        title_str.wstr_len = temp_visit_info_ptr->name_len;
    }

    num = sizeof(node_index) / sizeof(node_index[0]);

    MMIBROWSER_CreatePopupAutoMenu(win_id, ctrl_id, &title_str, num, tex_index, node_index);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void BrwOpenHistoryPropertyWin(BRW_VISIT_HIS_INFO_T *visit_info_ptr)
{
    uint32              alert_time_period = 0;
    wchar               property[MMIBROWSER_MAX_URL_LEN + GUIRICHTEXT_TEXT_MAX_LEN + MMIBROWSER_PROPERTY_EXT_LEN] = {0};
    MMI_STRING_T        str = {0};
    wchar               *property_ptr = PNULL;
    wchar               temp_uint16_buf[MMIBROWSER_MAX_URL_LEN+1] ={0};
    uint16              url_len = 0;

    if (PNULL == visit_info_ptr)
    {
        return;
    }

    property_ptr = property;

    /* Title: */
    MMIRES_GetText(TXT_COMM_TITLE, PNULL, &str);
    MMIAPICOM_Wstrncpy(property_ptr, str.wstr_ptr, str.wstr_len);
    property_ptr += str.wstr_len;

    property_ptr[0] = '\n';
    property_ptr += 1;
    /* Title */
    MMIAPICOM_Wstrncpy(property_ptr, visit_info_ptr->name_ptr, visit_info_ptr->name_len);
    property_ptr += visit_info_ptr->name_len;

    property_ptr[0] = '\n';
    property_ptr += 1;

    /* URL: */
    MMIRES_GetText(TXT_INPUT_URL_ADRESS, PNULL, &str);
    MMIAPICOM_Wstrncpy(property_ptr, str.wstr_ptr, str.wstr_len);
    property_ptr += str.wstr_len;

    property_ptr[0] = '\n';
    property_ptr += 1;

    /* URL */
    CFLWCHAR_StrNCpyWithAsciiStr(temp_uint16_buf,(char*)(visit_info_ptr->url_ptr),MMIBROWSER_MAX_URL_LEN + 1);
    url_len = MMIAPICOM_Wstrlen(temp_uint16_buf);

    MMIAPICOM_Wstrncpy(property_ptr, temp_uint16_buf, url_len);
    property_ptr += url_len;

    str.wstr_ptr = property;
    str.wstr_len = MMIAPICOM_Wstrlen(property);

    MMIPUB_OpenAlertWinByTextPtr(&alert_time_period, &str, PNULL, IMAGE_PUBWIN_SUCCESS, PNULL, PNULL, MMIPUB_SOFTKEY_CUSTOMER, BrwHandleHistoryAlertWinMsg);
}

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author:Jiaoyou.wu
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  BrwHandleHistoryAlertWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"BrwHandleHistoryAlertWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_WINTABLE_HISTORY_1055_112_2_18_2_6_37_298,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_STRING_T    title_str = {0};

            MMIPUB_SetWinSoftkey(win_id, TXT_NULL, TXT_COMMON_OK, FALSE);

            MMIRES_GetText(TXT_COMMON_HISTORY, PNULL, &title_str);
            MMIPUB_SetWinTitleText(win_id, &title_str, FALSE);
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

#endif /* MMI_PDA_SUPPORT */

/*****************************************************************************/
//	Description : the handle function of history list delete all menu window
//	Global resource dependence : 
//	Author:fen.xie
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E BrwHandleHistoryDeleteAllQueryWinMsg(
                MMI_WIN_ID_T win_id, 
                MMI_MESSAGE_ID_E msg_id, 
                DPARAM param
                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
#ifndef MMI_PDA_SUPPORT
    case MSG_APP_RED:
#else
    case MSG_KEYUP_RED:
#endif
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        MMIBROWSER_DeleteAllHistory();
        GUILIST_RemoveAllItems(MMIBROWSER_HISTORY_LIST_CTRL_ID);
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
#endif /* MMI_GUI_STYLE_TYPICAL */
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description :update the history list(remove all and re-append)
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIBROWSER_UpdateHistoryList(MMI_WIN_ID_T win_id)
{
    uint16  his_item_num=0;
    uint16  i=0;
    BRW_VISIT_HIS_INFO_T    *temp_visit_info_ptr=PNULL;
    MMI_CTRL_ID_T   list_ctrl_id = MMIBROWSER_HISTORY_LIST_CTRL_ID;
    MMI_TEXT_ID_T text3_id = STXT_RETURN;

#ifdef BROWSER_START_PAGE_THEME1
    if (!MMK_IsOpenWin(MMIBROWSER_MAIN_WIN_ID))
    {
        text3_id = STXT_EXIT;
    }
#endif            

    GUILIST_RemoveAllItems(list_ctrl_id);
    his_item_num = BRW_VisitHisGenIndexArr(&s_index_que);
    his_item_num = MIN(his_item_num, BRW_MAX_VISIT_HIS_ITEM_NUM);
    if (his_item_num > 0)            
    {
        wchar   unicode_url_arr[MMIBROWSER_MAX_URL_LEN+1]={0};
        MMI_STRING_T    name_str={0};
        MMI_STRING_T    url_str={0};

        for(;i < his_item_num; i++)
        {
            temp_visit_info_ptr = BRW_VisitHisGetHisInfoByIndex(s_index_que.index_arr[i]);
            if(PNULL != temp_visit_info_ptr)
            {
                name_str.wstr_ptr = temp_visit_info_ptr->name_ptr;
                name_str.wstr_len = temp_visit_info_ptr->name_len;
                
                CFLWCHAR_StrNCpyWithAsciiStr(unicode_url_arr, (char*)(temp_visit_info_ptr->url_ptr), MMIBROWSER_MAX_URL_LEN + 1);
                url_str.wstr_ptr = unicode_url_arr;
                url_str.wstr_len = strlen((char*)(temp_visit_info_ptr->url_ptr));
                url_str.wstr_len = MMI_MIN(url_str.wstr_len, MMIBROWSER_MAX_URL_LEN);
#ifdef MMI_PDA_SUPPORT
                MMIBROWSER_AppendTwoLineTextListItem(list_ctrl_id,STXT_OPTION,name_str,url_str,IMAGE_BROWSER_PAGE,i,FALSE);
#else
                MMIBROWSER_AppendTwoLineTextListItem(list_ctrl_id,STXT_OPTION,name_str,url_str,IMAGE_BROWSER_HISTORY_LIST_ICON,i,FALSE);
#endif /* MMI_PDA_SUPPORT */                
            }                   
        }
        
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif /* MMI_PDA_SUPPORT */                
    }
    else
    {
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, text3_id, FALSE);
    }
}

#ifdef BROWSER_START_PAGE_THEME1
/*****************************************************************************/
//  Description : get s_is_history_need_update
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetIsHistoryUpdate(void)
{
    return s_is_history_need_update;
}

/*****************************************************************************/
//  Description : set s_is_history_need_update
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetIsHistoryUpdate(BOOLEAN is_need_update)
{
    s_is_history_need_update = is_need_update;
}
#endif

/*****************************************************************************/
//  Description : Delete All History
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_DeleteAllHistory(void)
{
    Brw_VisitHisClean();
    SCI_MEMSET(&s_index_que, 0x00, sizeof(s_index_que));
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


/*Edit by script, ignore 5 case. Fri Apr 27 09:38:48 2012*/ //IGNORE9527
