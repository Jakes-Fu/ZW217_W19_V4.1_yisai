/****************************************************************************
** File Name:      mmisms_securitywin.c                                    *
** Author:         liming deng                                             *
** Date:           1/4/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe deleting SMS.              *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 01/2012        minghu.mao       Create
** 
****************************************************************************/

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
#include "mmisms_app.h"
#include "mmisms_id.h"
#include "mmisms_text.h"
#include "mmisms_image.h"
#include "mmi_pubmenuwin.h"
#include "mmisms_menutable.h"
#include "mmisms_order.h"
#include "mmisms_pushmsgboxwin.h"
#include "mmiota_id.h"
#include "mmiudisk_export.h"
#include "mmipub.h"
#ifdef MMI_OTA_SUPPORT 
#include "mmiota_export.h"
#endif

#ifndef MMI_GUI_STYLE_TYPICAL 
#include "mmisms_commonui.h"
#endif
#include "mmisms_appcontrol.h"
#include "guictrl_api.h"
#include "mmiidle_statusbar.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//END

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct 
{

    MMI_WIN_ID_T            win_id;
    uint32                  menu_id;
    uint32                  group_id;
    uint32            marked_sms_num;
    MMISMS_ORDER_ID_T cur_focus_order_id;
}MMISMS_PUSHBOX_WINDATA_T;

/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUCTION DEFINITION                                *
 **--------------------------------------------------------------------------*/
 
/*****************************************************************************/
//     Description : to handle the message of push message ,OTA window
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePushMsgWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

/*****************************************************************************/
//  Description : handle push msg mark all
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_HandlePushMsgMarkAll(MMISMS_PUSHBOX_WINDATA_T *win_data_ptr,BOOLEAN is_mark_all,MMI_WIN_ID_T win_id);

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandlePushMsgOpenMenuOpt(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//     Description : to handle read push message window
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void HandleReadPushSms(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:fengming.huang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePushMsgDelQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );
#if 0
/*****************************************************************************/
//     Description : to handle the message of delete waiting window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E PushMsgHandleDelWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     );

#endif
/*****************************************************************************/
//     Description : to handle the message of push message window
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void HandlePushmsgWinOpenMenuOKOpt(MMISMS_PUSHBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandlePushMsgOpenMenuInit(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//     Description : to open longok opt 
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void HandlePushmsgWinOpenLongOKOpt(MMISMS_PUSHBOX_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePushMsgWinDeleteCallBack(void *user_date,MMISMS_DELETE_DATE_INFO *data_info_ptr);
/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/

WINDOW_TABLE( MMISMS_PUSH_MESSAGE_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandlePushMsgWinMsg),    
    WIN_ID( MMISMS_PUSH_MESSAGE_WIN_ID ),
    WIN_TITLE(TXT_SMS_PUSH_MESSAGE),
    WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),
#ifdef     MMI_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISMS_PUSH_MESSAGE_FORM_CTRL_ID),    
    CHILD_SOFTKEY_CTRL(TXT_MARK_ALL,TXT_DELETE, STXT_CANCEL, MMICOMMON_SOFTKEY_CTRL_ID, MMISMS_PUSH_MESSAGE_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMISMS_PUSHMSG_LISTBOX_CTRL_ID, MMISMS_PUSH_MESSAGE_FORM_CTRL_ID),
#else
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_PUSHMSG_LISTBOX_CTRL_ID),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN ),  
#endif
    END_WIN
};
 
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle mark all
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_HandlePushMsgMarkAll(MMISMS_PUSHBOX_WINDATA_T *win_data_ptr,BOOLEAN is_mark_all,MMI_WIN_ID_T win_id)
{
    MMI_TEXT_ID_T left_id = 0;
    MMI_MENU_ID_T    menu_id = 0;
    MMI_CTRL_ID_T cur_ctrl_id = MMISMS_PUSHMSG_LISTBOX_CTRL_ID;
    uint16 i = 0;

    if(is_mark_all)
    {                              
        left_id = TXT_MARK_ALL;
        menu_id = MMISMS_MENU_CANCLE_MARKALL_ITEM_ID;
    }
    else
    {
        left_id = TXT_CANCEL_SELECT_ALL;
        menu_id = MMISMS_MENU_MARKALL_ITEM_ID;
    }
    
    switch(menu_id)   
    {
    case MMISMS_MENU_MARKALL_ITEM_ID:
        GUILIST_SetTextListMarkable(cur_ctrl_id, TRUE);
        GUILIST_SetMaxSelectedItem(cur_ctrl_id, MMISMS_GetMaxSmsNumInOrder());
        for (i = 0; i< GUILIST_GetTotalItemNum(cur_ctrl_id); i++)
        {
            GUILIST_SetSelectedItem(cur_ctrl_id, i, TRUE);
        }
        win_data_ptr->marked_sms_num = GUILIST_GetTotalItemNum(cur_ctrl_id);
        
        MMISMS_SetAllPushMsgMarked(TRUE);               
        break;
        
    case MMISMS_MENU_CANCLE_MARKALL_ITEM_ID:
        for (i = 0; i< GUILIST_GetTotalItemNum(cur_ctrl_id); i++)
        {
            GUILIST_SetSelectedItem(cur_ctrl_id, i, FALSE);
        }
        win_data_ptr->marked_sms_num = 0;        
        MMISMS_SetAllPushMsgMarked(FALSE);            
        break;

    default:
        break;
    }
    GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, left_id, TXT_DELETE,STXT_CANCEL,TRUE);     
    if (MMK_IsFocusWin(win_id))
    {
      MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0); //刷新窗口
    }
    return !is_mark_all;
}

/*****************************************************************************/
//  Description : open push message win
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_EnterPushMessageWin(void)
{
    MMI_HANDLE_T win_handle = 0;
    MMISMS_PUSHBOX_WINDATA_T *win_data_ptr = PNULL;    
    
    if(MMK_IsOpenWin(MMISMS_PUSH_MESSAGE_WIN_ID))
    {
        MMK_CloseWin(MMISMS_PUSH_MESSAGE_WIN_ID);
    }
    
    win_data_ptr = (MMISMS_PUSHBOX_WINDATA_T*)SCI_ALLOCAZ(sizeof(MMISMS_PUSHBOX_WINDATA_T));    
    if(PNULL == win_data_ptr)
    {
       SCI_TRACE_LOW("mmisms win_data_ptr alloc fail"); 
       return;
    }
        
    win_handle = MMK_CreateWin((uint32 *)MMISMS_PUSH_MESSAGE_WIN_TAB, PNULL);
    if(win_handle != PNULL)
    {
        MMK_SetWinUserData(win_handle, (ADD_DATA)win_data_ptr);  
    }
}

/*****************************************************************************/
//     Description : to handle read push message window
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void HandleReadPushSms(MMI_CTRL_ID_T ctrl_id)
{
    uint16 selected_index = 0;
    MMISMS_ORDER_ID_T order_id = 0;

    // save the current msg_id
    //to get the info of current selected item
    selected_index = GUILIST_GetCurItemIndex(ctrl_id);
    MMISMS_SetPushMsgCurOperationOrderId(selected_index);
    switch(MMISMS_GetMsgType())
    {       
    case MMISMS_TYPE_WAP_PUSH:
        if (MMIAPIUDISK_UdiskIsRun())
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        }
        else
        {
            order_id = MMISMS_GetCurOperationOrderId();
            if (PNULL == order_id)
            {
                //SCI_TRACE_LOW:"MMISMS:HandleOkMsgInSmsList order_id = PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_32273_112_2_18_2_55_27_882,(uint8*)"");
                break;
            }
            MMISMS_ClearOperInfo();
#ifdef MMI_OTA_SUPPORT                
            MMIAPIOTA_WapPushEnter((uint16)order_id->record_id);
#endif
            //避免左右键切换到彩信，按返回键后回到了刚刚读短信界面 cr117926
            if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
            {
                MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
            }
            if (MMK_IsOpenWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID))
            {
                MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);
            }
        }
        break;
            
    case MMISMS_TYPE_WAP_OTA:
    case MMISMS_TYPE_MMS_OTA:
        if (MMIAPIUDISK_UdiskIsRun())
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        }
        else
        {
            order_id = MMISMS_GetCurOperationOrderId();
            if (PNULL == order_id)
            {
                //SCI_TRACE_LOW:"MMISMS:HandleOkMsgInSmsList order_id = PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_32302_112_2_18_2_55_27_883,(uint8*)"");
                break;
            }                
            MMISMS_ClearOperInfo();
#ifdef MMI_OTA_SUPPORT
            MMIAPIOTA_OTAEnter((uint16)order_id->record_id);
#endif
            //避免左右键切换到彩信，按返回键后回到了刚刚读短信界面 cr117926
            if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
            {
                MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
            }
            if (MMK_IsOpenWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID))
            {
                MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
            }
        }
        break;
    default: 
        break;
    }
}  
/*****************************************************************************/
//     Description : to handle the message of SMS box window
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandlePushMsgChangeCallBack(MMIEVENT_INFO_T *event_info_ptr)
{
    MMI_WIN_ID_T win_id = MMISMS_PUSH_MESSAGE_WIN_ID;
    if(PNULL == event_info_ptr)
    {
       SCI_TRACE_LOW("mmisms chatbox event ptr PNULL");
       return;
    }
    if(SMS_DATA_CHANGE_EVENT == event_info_ptr->event_type)
    {  
        MMK_SetWinDirty(win_id, TRUE);
        if(MMK_IsFocusWin(win_id))
        {
           MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
    }    
}

/*****************************************************************************/
//     Description : to handle the message of push message window
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePushMsgWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_PUSHMSG_LISTBOX_CTRL_ID;
    uint16 selected_index = 0;
    uint16 total_num = 0;
    LOCAL BOOLEAN s_have_items = FALSE;
    LOCAL BOOLEAN s_need_update = FALSE;
    LOCAL BOOLEAN s_need_close_new_msg_win   = FALSE;
    GUILIST_MARK_ITEM_T *list_param_ptr = NULL;
    uint16 index = 0;
    uint16 top_index = 0;
    uint16 page_num = 0;
    // GUI_RECT_T  list_rect = {0};
#ifndef MMI_PDA_SUPPORT
    GUI_BOTH_RECT_T list_both_rect = MMITHEME_GetWinClientBothRect(win_id);
#endif
    #ifdef MMI_PDA_SUPPORT
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    LOCAL BOOLEAN s_is_mark_all = FALSE;
    #endif
    MMISMS_PUSHBOX_WINDATA_T *win_data_ptr = (MMISMS_PUSHBOX_WINDATA_T *)MMK_GetWinUserData(win_id);
    //SCI_TRACE_LOW:"MMISMS: HandlePushMsgWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_32450_112_2_18_2_55_27_884,(uint8*)"d", msg_id);
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("MMISMS win_data_ptr PNULL");
        return recode;
    }
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_PDA_SUPPORT
        GUIAPICTRL_SetBothRect(ctrl_id,&list_both_rect);
#endif        
        win_data_ptr->marked_sms_num = 0;
        win_data_ptr->win_id = win_id;
        s_need_update = FALSE;
        s_need_close_new_msg_win = FALSE;

        GUILIST_SetNeedPrgbar(ctrl_id, TRUE);

        // load items to list box
        s_have_items = MMISMS_LoadPushMsgFromOrder( ctrl_id );
        if (s_have_items)
        {            
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
        }
        else
        {

            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
        }
#ifdef MMI_PDA_SUPPORT
        child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
        GUIFORM_SetChildHeight(MMISMS_PUSH_MESSAGE_FORM_CTRL_ID,ctrl_id, &child_height);
        GUIFORM_IsSlide(MMISMS_PUSH_MESSAGE_FORM_CTRL_ID, FALSE);
        GUIFORM_PermitChildBg(MMISMS_PUSH_MESSAGE_FORM_CTRL_ID,FALSE);
        GUILIST_PermitBorder(ctrl_id,FALSE);
        if (s_have_items)
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
        else
        {
            GUIWIN_SetTitleButtonStatus(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE);
        }
        GUIFORM_SetChildDisplay(MMISMS_PUSH_MESSAGE_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
        GUILIST_SetDefaultIcon(ctrl_id, IMAGE_SMS_CHAT_DEFAULT, IMAGE_SMS_CHAT_DEFAULT);
        s_is_mark_all = FALSE;
#endif
        MMISMS_SetCurType(MMISMS_BOX_MT);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandlePushMsgChangeCallBack, TRUE); 
 #ifdef MMI_GUI_STYLE_TYPICAL
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
 #endif
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:    
        {
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
            
            if (PNULL == need_item_data_ptr)
            {
                //SCI_TRACE_LOW:"MMISMS:MSG_CTL_LIST_NEED_ITEM_DATA need_item_data_ptr = PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSAPP_WINTAB_32497_112_2_18_2_55_27_885,(uint8*)"");
                break;
            }
            index = need_item_data_ptr->item_index;
            MMISMS_SetPushMsgListItemData( ctrl_id, index);
        }
        break;
#ifndef MMI_GUI_STYLE_TYPICAL
   case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            //add anim into list item
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if(PNULL != need_item_content_ptr)
            {
                MMISMS_DrawListBoxItemWithIconData(need_item_content_ptr->item_index,need_item_content_ptr->item_content_index,ctrl_id,MMISMS_BOX_MT);
            }
            break;
        }        
#endif
        
    case MMISMS_MSG_NEEDUPDATELIST:
        s_need_update = TRUE;
        break;

    case MSG_LOSE_FOCUS:
        if (s_need_close_new_msg_win)
        {
            MMK_CloseWin(MMISMS_NEWMSG_WIN_ID);
            s_need_close_new_msg_win = FALSE;
        }
        break;

    case MSG_GET_FOCUS:       
        break;

    case MSG_FULL_PAINT:
        if(MMK_GetWinDirty(win_id))
        {
            selected_index = GUILIST_GetCurItemIndex(ctrl_id);
            total_num = GUILIST_GetTotalItemNum(ctrl_id);
            top_index = GUILIST_GetTopItemIndex(ctrl_id);
            page_num = GUILIST_GetPageItemNum(ctrl_id);
    
            s_have_items = MMISMS_LoadPushMsgFromOrder( ctrl_id);
            if (s_have_items)
            {            
                GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
            }
            else
            {
                GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
            }
    
            if (total_num < GUILIST_GetTotalItemNum(ctrl_id)) //有增加新信息
            {
                if((top_index == 0) && (selected_index != (page_num - 1)))
                {
                    GUILIST_SetCurItemIndex(ctrl_id, (uint16)(selected_index + 1));
                    GUILIST_SetTopItemIndex(ctrl_id, top_index);
                }
                else
                {
                    GUILIST_SetCurItemIndex(ctrl_id, (uint16)(selected_index + 1));
                    GUILIST_SetTopItemIndex(ctrl_id, top_index + 1);
                }
            }
    
            else
            {
                if(selected_index >= GUILIST_GetTotalItemNum(ctrl_id)) // 删除的 情况
                {
                    GUILIST_SetCurItemIndex(ctrl_id, 0);
                    GUILIST_SetTopItemIndex(ctrl_id, 0);
                }
                else
                {           
                    GUILIST_SetCurItemIndex(ctrl_id, selected_index);
                    GUILIST_SetTopItemIndex(ctrl_id, top_index);
                }
            }
            MMK_SetWinDirty(win_id, FALSE);
        }

        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:         
        {
            if(GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
            {            
                uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;               
                switch(src_id)
                {
                    case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                    {  
#ifdef MMI_PDA_SUPPORT                        
                        s_is_mark_all = MMISMS_HandlePushMsgMarkAll(win_data_ptr,s_is_mark_all,win_id);
#endif
                    }
                    break;
                      
                    case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                    if(win_data_ptr->marked_sms_num > 0 )
                    {
                        if (MMISMS_IsUpdateOk())
                        {                                                                  
                            MMISMS_OpenDelteQueryWin(MMISMS_DELETE_PART, TRUE, win_data_ptr->cur_focus_order_id,MMISMS_BOX_MT,HandlePushMsgDelQueryWinMsg);             
                        }
                    }
                    else
                    {
                        MMISMS_OpenAlertWarningWin(TXT_SMS_SELECT_ONT_PROMPT);    
                    }
                    break;

                   case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                   {
                       win_data_ptr->marked_sms_num = 0;       
#ifdef MMI_PDA_SUPPORT                       
                       s_is_mark_all = FALSE;
#endif
                       GUILIST_SetTextListMarkable(ctrl_id, FALSE);
                       MMISMS_SetAllPushMsgMarked(FALSE);
                       
                       GUIFORM_SetChildDisplay(MMISMS_PUSH_MESSAGE_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);    
                       if (MMK_IsFocusWin(win_id))
                       {
                           MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0); //刷新窗口
                       }
                   }
                 break;

                 default:
                    break;
                }
                break;
            }                
            if(s_have_items)
            {
                HandleReadPushSms(ctrl_id);    
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_LIST_LONGOK:
        {              
            if (s_have_items && !GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
            {
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                MMISMS_SetPushMsgCurOperationOrderId(selected_index);
                win_data_ptr->cur_focus_order_id = MMISMS_GetPushMsgLinkedArrayPtrId(index);              
                HandlePushmsgWinOpenLongOKOpt(win_data_ptr);                 
            }   
        
        } 
        break;
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
#ifdef MMI_PDA_SUPPORT
        if (s_have_items && !GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
#endif
        { 
#ifdef MMI_PDA_SUPPORT            
            s_is_mark_all = FALSE;
            GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_MARK_ALL, TXT_DELETE,STXT_CANCEL,TRUE);  
#endif
#ifdef MMI_SMS_CHAT_SUPPORT
            //MMK_CreateWin((uint32 *)MMISMS_PUSH_MESSAGE_MENUKEY_WIN_TAB,PNULL);    
            HandlePushmsgWinOpenMenuOKOpt(win_data_ptr);
#endif
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MMISMS_MSG_READ_MSG:
        if (s_have_items && (win_data_ptr->marked_sms_num == 0))
        {
            HandleReadPushSms(ctrl_id);    
        }
        break;

    case MSG_CTL_LIST_MARK_ITEM:   
        //to get the info of current selected item
        list_param_ptr = (GUILIST_MARK_ITEM_T*)param;

        MMISMS_SetPushMsgCurOperationOrderId(list_param_ptr->item_idx);

        #ifdef MMI_PDA_SUPPORT
        MMISMS_SetSoftkeyAfterMarkOpera(win_id,ctrl_id,&s_is_mark_all);            
        #endif

        if (ctrl_id == list_param_ptr->ctrl_id)
        {
            if (list_param_ptr->had_maked)
            {
                MMISMS_SetCurMsgMarked(TRUE);
                win_data_ptr->marked_sms_num++;
            }
            else
            {
                MMISMS_SetCurMsgMarked(FALSE);
                win_data_ptr->marked_sms_num--;                
            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:

        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        MMISMS_SetAllPushMsgMarked(FALSE);
        MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandlePushMsgChangeCallBack, FALSE); 
        SCI_FREE(win_data_ptr);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
/*****************************************************************************/
//     Description : to open longok opt 
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void HandlePushmsgWinOpenLongOKOpt(MMISMS_PUSHBOX_WINDATA_T *win_data_ptr)
{
    MMIPUB_MENU_PARAM_T menuparam = {0};

    menuparam.group_id = MENU_SMS_PUSHMSG_ITEM_OPT;

    win_data_ptr->group_id = menuparam.group_id;
    menuparam.win_id =   MMISMS_PUSHMSG_ITEM_OPT_WIN_ID;
    menuparam.ctrl_id =  MMISMS_PUSHMSG_ITEMS_OPT_CTRL_ID;    
    menuparam.dst_handle = win_data_ptr->win_id;
    menuparam.func.OnMenuOk = HandlePushMsgOpenMenuOpt;  
    //menuparam.func.OnMenuInit = HandleMsgWinOpenChatBoxInit;       
    MMIPUB_OpenPubMenuWin( &menuparam ); 
}

/*****************************************************************************/
//     Description : to handle the message of push message window
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void HandlePushmsgWinOpenMenuOKOpt(MMISMS_PUSHBOX_WINDATA_T *win_data_ptr)
{
    MMIPUB_MENU_PARAM_T menuparam = {0};

    menuparam.group_id = MENU_SMS_PUSHMSG_MENUKEY_OPT;

    win_data_ptr->group_id = menuparam.group_id;
    menuparam.win_id =   MMISMS_PUSHMSG_MENUKEY_WIN_ID;
    menuparam.ctrl_id =  MMISMS_PUSHMSG_MENUKEY_OPT_MENU_CTRL_ID;    
    menuparam.dst_handle = win_data_ptr->win_id;
    menuparam.func.OnMenuOk = HandlePushMsgOpenMenuOpt;  
    menuparam.func.OnMenuInit = HandlePushMsgOpenMenuInit;       
    MMIPUB_OpenPubMenuWin( &menuparam ); 
}

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandlePushMsgOpenMenuInit(MMIPUB_MENU_NOTIFY_T *param )
{
    MMISMS_PUSHBOX_WINDATA_T *win_data_ptr = PNULL;    
    win_data_ptr = (MMISMS_PUSHBOX_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }    
    
    if ((win_data_ptr->marked_sms_num == 0 && MMISMS_CurMsgIsLocked())
                    ||(win_data_ptr->marked_sms_num > 0 && (0 == MMISMS_GetPushMsgDeleteAvailNum())))
    {
        MMIAPICOM_EnableGrayed(win_data_ptr->win_id, MENU_SMS_PUSHMSG_ITEM_OPT,MMISMS_MENU_DEL_PUSH_ITEM_ID,TRUE);
    }    
}


/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandlePushMsgOpenMenuOpt(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 group_id, menu_id;
   
    MMISMS_PUSHBOX_WINDATA_T *win_data_ptr = PNULL;       
    MMISMS_DELETE_TYPE_E delete_type = MMISMS_DELETE_MAX;
        
    win_data_ptr = (MMISMS_PUSHBOX_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);

    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    
    group_id = param->group_id;
    menu_id = param->menu_id;
    
    win_data_ptr->menu_id = menu_id;
    win_data_ptr->group_id = group_id;
    switch(menu_id)
    {
        case MMISMS_MENU_DEL_PUSH_ITEM_ID:
            if (MMIAPIUDISK_UdiskIsRun())
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                break;
            }
            delete_type = (0 == win_data_ptr->marked_sms_num)?MMISMS_DELETE_SINGLE:MMISMS_DELETE_PART;
            MMISMS_OpenDelteQueryWin(delete_type, TRUE, win_data_ptr->cur_focus_order_id,MMISMS_BOX_MT,HandlePushMsgDelQueryWinMsg);
            break;
            
        case MMISMS_MENU_PUSH_MSG_MARK_ID:        
            GUILIST_SetTextListMarkable(MMISMS_PUSHMSG_LISTBOX_CTRL_ID, TRUE);    
            GUILIST_SetMaxSelectedItem(MMISMS_PUSHMSG_LISTBOX_CTRL_ID, MMISMS_GetMaxSmsNumInOrder());
            GUIFORM_SetChildDisplay(MMISMS_PUSH_MESSAGE_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);   
            break;
#ifdef MMI_SMS_CHAT_SUPPORT
        case MMISMS_MENU_CHAT_OPT_MARK_ITEM_ID:
            MMISMS_MarkItem(win_data_ptr->cur_focus_order_id,MMISMS_PUSHMSG_LISTBOX_CTRL_ID,&win_data_ptr->marked_sms_num);
            break;

        case MMISMS_MENU_CHAT_CANCEL_MARK_ITEM_ID:
            MMISMS_CancelMarkItem(win_data_ptr->cur_focus_order_id,MMISMS_PUSHMSG_LISTBOX_CTRL_ID,&win_data_ptr->marked_sms_num);
            break;
 #endif           
        case MMISMS_MENU_MARKALL_ITEM_ID: 
            MMISMS_MarkAllItem(MMISMS_PUSHMSG_LISTBOX_CTRL_ID,MMISMS_BOX_MT,&win_data_ptr->marked_sms_num,TRUE);
            break;

        case MMISMS_MENU_CANCLE_MARKALL_ITEM_ID:
            MMISMS_CancelMarkAllItem(MMISMS_PUSHMSG_LISTBOX_CTRL_ID,MMISMS_BOX_MT,&win_data_ptr->marked_sms_num,TRUE);
            break;
            
        default:
            break;
    }
}


/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePushMsgWinDeleteCallBack(void *user_date,MMISMS_DELETE_DATE_INFO *data_info_ptr)
{
    MMISMS_PUSHBOX_WINDATA_T *win_data_ptr = (MMISMS_PUSHBOX_WINDATA_T*)user_date;
    if(PNULL == win_data_ptr)
    {
        return FALSE;
    }
    win_data_ptr->marked_sms_num = 0;
    GUILIST_SetTextListMarkable(MMISMS_PUSHMSG_LISTBOX_CTRL_ID, FALSE);
    //MMISMS_SetBoxSoftkeyDisplay(win_data_ptr->box_type, GUIFORM_CHILD_DISP_HIDE);
    GUIFORM_SetChildDisplay(MMISMS_PUSH_MESSAGE_FORM_CTRL_ID, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
    MMISMS_SetAllPushMsgMarked(FALSE);

    MMK_CloseWin(MMISMS_MTBOX_WIN_ID);
    MMK_CloseWin(MMISMS_SENDSUCCBOX_WIN_ID);
    MMK_CloseWin(MMISMS_MOBOX_WIN_ID);

    MMISMS_SetDelAllFlag(FALSE);  
    //全部删除完毕，将未处理链表中的sms插入到相应的链表中
    MMISMS_InsertUnhandledSmsToOrder();
    MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
    if(MMISMS_FINISH_OPER_ERR == data_info_ptr->ret_value)
    {
        MMISMS_OpenMsgBox(TXT_ERROR, IMAGE_PUBWIN_WARNING);
        MMISMS_ClearOperInfo();       
    }
    MAIN_SetMsgState(TRUE);
    return TRUE;
}

/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:fengming.huang
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePushMsgDelQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T cur_ctrl_id = MMISMS_PUSHMSG_LISTBOX_CTRL_ID;

    MMISMS_OPERATE_DATA_INFO_T data_info = {0};
    MMISMS_PUSHBOX_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_PUSHBOX_WINDATA_T *)MMK_GetWinUserData(MMISMS_PUSH_MESSAGE_WIN_ID);
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr PNULL");
        return recode;
    }
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:
        //MMISMS_OperateDelorDelAll(win_data_ptr->cur_focus_order_id,win_data_ptr->marked_sms_num,PushMsgHandleDelWaitWinMsg);

#ifdef MMI_TIMERMSG_SUPPORT
        if (MMISMS_GetIsTimerMsg())
        {
            MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
    
            return recode;
        }
#endif
        MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,TRUE);
        data_info.box_type = MMISMS_BOX_MT; 

        MMISMS_EnterDelWaitWin(&data_info,HandlePushMsgWinDeleteCallBack,(void*)win_data_ptr);
      
        if(win_data_ptr->marked_sms_num > 0)
        {
            GUILIST_SetCurItemIndex(cur_ctrl_id, 0);
            GUILIST_SetTopItemIndex(cur_ctrl_id, 0);
        }

        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMISMS_CancelMarkAllItem(cur_ctrl_id,MMISMS_BOX_MT, &win_data_ptr->marked_sms_num, TRUE);
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}
#endif
