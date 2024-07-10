/****************************************************************************
** File Name:      watch_sms_message.c                                    *
** Author:         qi.liu1                                             *
** Date:           20/2/2020                                                *
** Copyright:      2020 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe main SMS.              *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 02/2020      qi.liu1                  Create
** 
****************************************************************************/

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmipb_image.h"
#include "mmipb_export.h"
#include "mmicom_trace.h"
#include "watch_commonwin_export.h"
#include "watch_sms_message.h"
#include "watch_sms_chat.h"
#include "guictrl_api.h"
#include "mmisms_appcontrol.h"
#include "mmiidle_statusbar.h"
#include "mmisms_receive.h"
#include "mmisms_send.h"
#include "mmisms_read.h"
#include "mmisms_internal.h"
#include "mmisms_export.h"
#include "mmisms_messagecountwin.h"
#include "mmismsapp_interface.h"
#include "mmisms_id.h"
#ifdef ADULT_WATCH_SUPPORT
#include "mmidisplay_color.h"
#endif
#include "mmisms_edit.h"
#ifdef SCREEN_SHAPE_CIRCULAR
#include "watch_common_list.h"
#include "watch_common.h"
#endif

/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/
typedef struct
{
    MMI_HANDLE_T         win_handle;
    MMI_WIN_ID_T            win_id;
    MMISMS_BOX_TYPE_E       box_type;
    MMI_CTRL_ID_T     cur_list_ctrl_id;
    uint32            marked_sms_num;
    MMISMS_ORDER_ID_T cur_focus_order_id;
    BOOLEAN is_del_all;
}MMISMS_MESSAGE_WINDATA_T;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                  *
 **--------------------------------------------------------------------------*/

 /*****************************************************************************/
//  Description : delete message query win
//  Parameter: [In] win param
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMessageBoxDelQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );

 /*****************************************************************************/
//  Description : delete message item call back
//  Parameter: [In] user_data
//             [In] data_info_ptr
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MessageDelFinishCallback(void *user_data,MMISMS_DELETE_DATE_INFO *data_info_ptr) ;

#ifdef ADULT_WATCH_SUPPORT
/*****************************************************************************/
//  Description : GetUnreadPhotoIDByContactID
//  Parameter: [In] contactImageId
//             [Out] unread photo id
//  Author: qi.liu1
//  Note:将联系人图片ID转换成未读短信联系人ID
/*****************************************************************************/
LOCAL uint32 GetUnreadPhotoIDByContactID(MMI_IMAGE_ID_T      contactImageId);
#endif
/*****************************************************************************/
//  Description : open chat win
//  Parameter: [In] current order id
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL void OpenChatReadWin(MMISMS_ORDER_ID_T cur_order_id)
{
    if(PNULL == cur_order_id)
    {
        return;
    }
    TRACE_APP_MESSAGES("WatchSMS Message: open chat read win cur_order_id = %d",cur_order_id);
    WatchSMS_ChatWin_Enter(cur_order_id);
    //MMISMS_EnterSMSChatReadWin(cur_order_id);
    MMISMS_OpenFullScreenWaitingWin( MMISMS_READSMS_WAIT_WIN_ID,PNULL,PNULL,MMISMS_HandleWaitWinMsg );
}

/*****************************************************************************/
//  Description : to handle the message of SMS box window
//  Parameter: [In] event info
//             [Out] None
//  Author: qi.liu1
//  Note:when receive a new SMS, can handle this call back(HandleChatBoxDateChangeCallBack)
/*****************************************************************************/
LOCAL void HandleMessageDateChangeCallBack(MMIEVENT_INFO_T *event_info_ptr)
{
    MMI_WIN_ID_T        winId = MMISMS_MESSAGE_LIST_WIN_ID;
    if(PNULL == event_info_ptr)
    {
       TRACE_APP_MESSAGES("WatchSMS Message: messagebox event ptr PNULL");
       return;
    }
    TRACE_APP_MESSAGES("WatchSMS Message: HandleMessageDateChangeCallBack ");
    if(SMS_DATA_CHANGE_EVENT == event_info_ptr->event_type)
    {
        MMK_SetWinDirty(winId, TRUE);
        if(MMK_IsFocusWin(winId))
        {
           MMK_PostMsg(winId, MSG_FULL_PAINT, PNULL, 0);
        }
    }
}

/*****************************************************************************/
//  Description : save send-contact num info
//  Parameter: [In] cur_order_id
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL void  SaveNumberForSend(MMISMS_ORDER_ID_T cur_order_id)
{
    uint8 numLen = 0;
    uint8 numberPtr[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    MMISMS_DEST_ADDR_INFO_T numberInfo = {0}; //保存号码信息

    numLen = MMIAPICOM_GenDispNumber(
                  MMIPB_GetNumberTypeFromUint8(cur_order_id->number.bcd_number.npi_ton),
                  MIN(MN_MAX_ADDR_BCD_LEN, cur_order_id->number.bcd_number.number_len),
                   cur_order_id->number.bcd_number.number,
                  numberPtr,
                  MMISMS_PBNUM_MAX_LEN + 2
              );
    MMISMS_SetAddressToMessage((uint8 *)numberPtr, (uint8)strlen((char *)numberPtr), &numberInfo.dest_list);
    //save phone number
    MMISMS_SetNumberForSend( numberInfo );
}

/*****************************************************************************/
//  Description : update  message list item
//  Parameter: [In]index
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN UpdateMessageList(MMI_CTRL_ID_T     ctrl_id)
{
    uint16 count = 0;
    uint16 list_item_num = 0;
    GUILIST_ITEM_T item_info = {0};
    uint16 idx=0;

    count = MMISMS_GetBoxTotalNumberFromOrder(MMISMS_BOX_CHAT);
    TRACE_APP_MESSAGES("WatchSMS Message: the count of list box is %d", count);

    list_item_num = MAX(count, 1);
    GUILIST_RemoveAllItems(ctrl_id);
    GUILIST_SetMaxItem(ctrl_id, list_item_num, TRUE);//max item 

    if (count > 0)
    {
        //check the order
        MMISMS_SortChatMessageOrder();

        //append all message list item
        for(idx=0;idx<count;idx++)
        {
#ifdef SCREEN_SHAPE_CIRCULAR
#ifdef ADULT_WATCH_SUPPORT
            item_info.item_style = GUIITEM_STYLE_1ICON_2STR_2LINE_ADULTWATCH;
            //不画分割线
            GUILIST_SetListState( ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE );
            //不画高亮条
            GUILIST_SetListState( ctrl_id, GUILIST_STATE_NEED_HIGHTBAR, FALSE );
            GUILIST_SetListState( ctrl_id, GUILIST_STATE_TEXTSCROLL_ENABLE | GUILIST_STATE_AUTO_SCROLL, TRUE );//fix bug1594837 1595403 1586984
#else
            item_info.item_style = GUIITEM_STYLE_3STR_2LINE_LAYOUT1;
            //不画分割线
            GUILIST_SetListState( ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE );
            GUILIST_SetListState( ctrl_id, GUILIST_STATE_TEXTSCROLL_ENABLE | GUILIST_STATE_AUTO_SCROLL, TRUE );//Bug 2125313
#endif
#else
            item_info.item_style = GUIITEM_STYLE_1ICON_3STR;
#endif
            GUILIST_AppendItem(ctrl_id, &item_info);
        }
        return TRUE;
    }
    else
    {
        //tip empty note win
        MMI_WIN_ID_T noteId = MMISMS_EMPTY_MESSAGE_LIST_WIN_ID;
#ifdef SCREEN_SHAPE_CIRCULAR
        MMI_STRING_T no_message_text = {0};
        wchar temp_arr[20 + 1] = {0};
        no_message_text.wstr_ptr = temp_arr;
        MMI_GetLabelTextByLang(TXT_NO_CONTENT, &no_message_text);
        TRACE_APP_MESSAGES(" message empty");
        Adult_WatchCOM_NoteWin_1Icon_1Str_Enter(MMISMS_NOTE_WIN_ID,&no_message_text,IMAGE_SMS_MESSAGE_NONE,PNULL);
#else
        MMISMS_OpenFullScreenNoteWin( noteId,TXT_BLANK,IMAGE_SMS_MESSAGE_NONE,PNULL);
#endif
        if(MMK_IsOpenWin(MMISMS_MESSAGE_LIST_WIN_ID))
        {
            MMK_CloseWin(MMISMS_MESSAGE_LIST_WIN_ID);
        }
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : set one message list item data
//  Parameter: [In] sms_state_ptr ,ctrl_id,index
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL void SetOneItemDataFromIdx(
                           MMISMS_STATE_T    *sms_state_ptr,
                           MMI_CTRL_ID_T     ctrl_id,
                           uint16            index
                           )
{
    GUILIST_ITEM_DATA_T item_data = {0};
    MMI_STRING_T name = {0}; 
    MMI_STRING_T time = {0};
    uint8 time_string[MMIMESSAGE_DIAPLAY_LEN] = {0};
    MMI_STRING_T subject = {0};
    MMI_TM_T sms_time = {0};
    SCI_DATE_T cur_date = {0};
    uint16 list_index = 0;
    MMI_HANDLE_T list_handle = MMK_ConvertIdToHandle( ctrl_id );
    wchar temp_arr1[MMIMESSAGE_DIAPLAY_LEN + 1] = {0};
    wchar temp_arr2[MMIMESSAGE_DIAPLAY_LEN + 1] = {0};
    wchar temp_arr3[MMISMS_PBNAME_MAX_LEN + 1] = {0};
    uint8 date_str[MMISET_DATE_STR_LEN + 1] = {0};
#ifdef ADULT_WATCH_SUPPORT
    MMI_SMS_CHAT_STATUS_T chat_status = MMISMS_FindChatSMSNumStatusInList(index);
    uint8 photoId_temp = 0;
#endif
    MMISMS_EMOJI_DISPLAY_CONTENT_T *display_data = PNULL;
    uint16 len = 0, i = 0;

    subject.wstr_ptr = temp_arr1;
    time.wstr_ptr    = temp_arr2;
    name.wstr_ptr    = temp_arr3;

    display_data = (MMISMS_EMOJI_DISPLAY_CONTENT_T *)SCI_ALLOCA(sizeof(MMISMS_EMOJI_DISPLAY_CONTENT_T ));
    if (PNULL == display_data)
    {
        SCI_TRACE_LOW("[MMISMS] SetOneItemDataFromIdx alloc display_data fail!");
        return;
    }

    SCI_MEMSET(display_data, 0x00, sizeof(MMISMS_EMOJI_DISPLAY_CONTENT_T));

    //get and deal name info
    MMISMS_GetNameInfo(sms_state_ptr, &name, MMISMS_PBNAME_MAX_LEN);

    //get and deal subject info
    if(MMISMS_IsContainEmoji(sms_state_ptr->display_content.content, sms_state_ptr->display_content.content_len, display_data))
    {
        for (i = 0; i < display_data->length; i++)
        {
            if(PNULL != display_data->item[i].data)
            {
                MMIAPICOM_Wstrcat(subject.wstr_ptr, &display_data->item[i].data);
            }
            else
            {
                len = MMIAPICOM_Wstrlen(subject.wstr_ptr);
                subject.wstr_ptr[len] = (wchar*)display_data->item[i].code;
            }
        }

        subject.wstr_len = (uint16)MMIAPICOM_Wstrlen(subject.wstr_ptr);
    }
    else
    {
        subject.wstr_len  = MIN((sms_state_ptr->display_content.content_len), MMIMESSAGE_DIAPLAY_LEN);   
        MMI_WSTRNCPY(
            subject.wstr_ptr,
            MMIMESSAGE_DIAPLAY_LEN,
            sms_state_ptr->display_content.content, 
            subject.wstr_len,
            subject.wstr_len
            );
    }
    TRACE_APP_MESSAGES("WatchSMS Message: subject.length is %d", subject.wstr_len);

    //get and deal time info
    TM_GetSysDate(&cur_date);
    sms_time = MMIAPICOM_Second2Tm(sms_state_ptr->time);
    if ((sms_time.tm_mday == cur_date.mday)
        && (sms_time.tm_mon == cur_date.mon) 
        && (sms_time.tm_year == cur_date.year))
    {
        MMIAPISET_FormatTimeStrByTime(sms_time.tm_hour, sms_time.tm_min, time_string, MMISET_DATE_STR_LEN);
        time.wstr_len = sprintf((char *)time_string, "%s", time_string);
    }
    else
    {
        MMIAPISET_FormatDateStrByDateEx(sms_time.tm_year,
            sms_time.tm_mon,
            sms_time.tm_mday,
            '-',
            date_str,
            MMISET_DATE_STR_LEN,
            FALSE);
        time.wstr_len = sprintf((char *)time_string, "%s", date_str);
    }
    MMI_STRNTOWSTR(time.wstr_ptr,
                                MMIMESSAGE_DIAPLAY_LEN,
                                (uint8 *)time_string,
                                time.wstr_len,
                                time.wstr_len
                                );

    list_index = 0;

    //compare , and set contact photo
    //set and deal photo info
    {
        MMIPB_CONTACT_T contactInfo = {0};
        BOOLEAN isFit = FALSE;
        uint8 photoId = 0;
#ifdef ADULT_WATCH_SUPPORT
        MMI_SMS_CHAT_STATUS_T chat_status = MMISMS_FindChatSMSNumStatusInList(index);

        if (TRUE == chat_status.is_unread_sms)//未读短信
        {
            item_data.item_content[list_index].item_data.image_id = IMAGE_SMS_NOT_READ_SMS_ICON;
        }
        else//只有短信
        {
            item_data.item_content[list_index].item_data.image_id = IMAGE_SMS_READ_SMS_ICON;
        }
#endif
        //set item data type
        item_data.item_content[list_index].item_data_type = GUIITEM_DATA_IMAGE_ID;

#ifndef ADULT_WATCH_SUPPORT
#ifndef SCREEN_SHAPE_CIRCULAR
        isFit = WatchPB_GetContactInfoByBcd(&sms_state_ptr->number.bcd_number, &contactInfo);
        SCI_TRACE_LOW("WATCHSMS: SetOneItemDataFromIdx isFit=%d",isFit);
        if(isFit)
        {
            item_data.item_content[list_index].item_data.image_id = WatchPB_GetImageIdByFlag(contactInfo.photo_id_flag,FALSE);
        }
        else
        {
            item_data.item_content[list_index].item_data.image_id = IAMGE_CONTACT_PORTRAIT_STRANGER;
        }
#endif
#endif
    }
#ifndef SCREEN_SHAPE_CIRCULAR
    list_index++;
#endif
    // set other value to item list
    {
        // set name value to item list
        item_data.item_content[list_index].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[list_index].item_data.text_buffer.wstr_len = name.wstr_len;
        item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = name.wstr_ptr;

        list_index++;
#ifndef ADULT_WATCH_SUPPORT   //成人手表不需要显示时间
        // set time value to item list    
        item_data.item_content[list_index].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[list_index].item_data.text_buffer.wstr_len = time.wstr_len;
        item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = time.wstr_ptr;

        list_index++;
#endif
        // set subject value to item list
        item_data.item_content[list_index].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[list_index].item_data.text_buffer.wstr_len = subject.wstr_len;

        item_data.item_content[list_index].item_data.text_buffer.wstr_ptr = subject.wstr_ptr;
        
        list_index++;
    }	
    
    GUILIST_SetItemData( list_handle, &item_data, index );
    if (PNULL != display_data)
    {
        SCI_FREE(display_data);
    }
}

/*****************************************************************************/
//  Description : set message list item data
//  Parameter: [In] ctrl_id ,index
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL void SetMessageListItemData(MMI_CTRL_ID_T ctrl_id , uint16 index)
{
    MMISMS_ORDER_ID_T current_order_id = PNULL;
    MMISMS_STATE_T sms_order_info ={0};

    current_order_id = MMISMS_GetChatLinkedArrayPtrIdInList(index);
    if (current_order_id != PNULL)
    {
        MMISMS_GetSmsInfoFromOrder(current_order_id, &sms_order_info);

        SetOneItemDataFromIdx(
            &sms_order_info,
            ctrl_id,
            index
            );
    }
    else
    {
        TRACE_APP_MESSAGES("WatchSMS Message:SetMessageListItemData current_order_id = PNULL");
        return;
    }
}

/*****************************************************************************/
//  Description : to handle message win msg
//  Parameter: [In] win_id msg_id param
//             [Out] None
//  Author: qi.liu1
//  Note:(HandleChatBoxChildWinMsg)
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMessageWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E msg_id,        //IN:
    DPARAM            param        //IN:
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrlId = MMISMS_CHATBOX_LISTBOX_CTRL_ID;
    uint16 selectIndex = 0;
    BOOLEAN sHaveItems = FALSE;
    uint16 index = 0;
    MMISMS_MESSAGE_WINDATA_T *win_data_ptr = (MMISMS_MESSAGE_WINDATA_T *)MMK_GetWinUserData(win_id);
#ifdef SCREEN_SHAPE_CIRCULAR
    GUI_RECT_T list_rect = WATCHLIST_RECT;
#endif
    if(PNULL == win_data_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    TRACE_APP_MESSAGES("WatchSMS Message: HandleMessageWinMsg msg_id = 0x%x",msg_id);
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
#ifdef SCREEN_SHAPE_CIRCULAR
            GUILIST_SetRect(ctrlId, &list_rect);
#endif

#ifdef  ADULT_WATCH_SUPPORT
            //set title text color
            {
                GUI_COLOR_T      font_color = MMI_GREEN_COLOR;
                GUIWIN_SetTitleFontColor(win_id,font_color);
            }
#endif
            // load items to message list , Append item
            sHaveItems = UpdateMessageList( ctrlId );//此接口内部可能会同步调用mmk_closewin，会同步释放win_data_ptr，所以需要重新获取win_data_ptr
            win_data_ptr = (MMISMS_MESSAGE_WINDATA_T *)MMK_GetWinUserData(win_id);//bug1384026 
            if(PNULL != win_data_ptr)
            {
                TRACE_APP_MESSAGES("sms_num != 0");
                win_data_ptr->box_type = MMISMS_BOX_CHAT;
                win_data_ptr->marked_sms_num = 0;
                MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleMessageDateChangeCallBack, TRUE);
                MMK_SetAtvCtrl(win_id, ctrlId);
            }
            break;
        }
        case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;

            if (PNULL == need_item_data_ptr)
            {
                TRACE_APP_MESSAGES("WatchSMS Message:MSG_CTL_LIST_NEED_ITEM_DATA need_item_data_ptr = PNULL"); 
                break;
            }
            //get ctrl index
            index = need_item_data_ptr->item_index;
            //set item data
            SetMessageListItemData(ctrlId, index);
            break;
        }

        case MSG_GET_FOCUS:
        {
            //MMISMS_SetCurType(MMISMS_BOX_CHAT);
            if ( MMISMS_IsNeedSort())
            {
                selectIndex = GUILIST_GetCurItemIndex(ctrlId);
                // load items to message list , Append item
                sHaveItems = UpdateMessageList( ctrlId );
                GUILIST_SetCurItemIndex(ctrlId, selectIndex);
            }
            break;
        }

        case MSG_FULL_PAINT:
        {
#ifdef SCREEN_SHAPE_CIRCULAR
            WATCHCOM_DisplayBackground(win_id);
#endif
            if(MMK_GetWinDirty(win_id))
            {
                if (MMK_IsFocusWin(win_id))
                {
                    selectIndex = GUILIST_GetCurItemIndex(ctrlId);
                    // load items to message list , Append item
                    sHaveItems = UpdateMessageList( ctrlId );
                    GUILIST_SetCurItemIndex(ctrlId, selectIndex);
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
                    MMK_SetWinDirty(win_id,FALSE);
                }
            }
            break;
        }

#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        {
			MMISMS_SetPresetFrom( MMISMS_PRESETFROM_CHAT );
            selectIndex = GUILIST_GetCurItemIndex(ctrlId);
            win_data_ptr->cur_focus_order_id = MMISMS_GetNumNodeByIndexInList(selectIndex);
            //save phone number info for send
            SaveNumberForSend(win_data_ptr->cur_focus_order_id);
            //open chat read win
            OpenChatReadWin(win_data_ptr->cur_focus_order_id);
            break;
        }

#ifdef SCREEN_SHAPE_CIRCULAR
        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_CTL_LIST_LONGPRESS:
        {
            selectIndex = GUILIST_GetCurItemIndex(ctrlId);
            MMISMS_SetChatMsgMarkedInList(selectIndex,TRUE);
            win_data_ptr->cur_focus_order_id = MMISMS_GetNumNodeByIndexInList(selectIndex);
            //open delete query win
//#ifndef ADULT_WATCH_SUPPORT
#if 0
            MMISMS_OpenFullScreenQueryWin(
                           MMISMS_DEL_QUERY_WIN_ID,
                           TXT_DELETE,
                           IMAGE_SMS_MESSAGE_BACK,
                           IMAGE_SMS_MESSAGE_DELETE,
                           HandleMessageBoxDelQueryWinMsg);
//#else
#endif
            MMISMS_OpenFullScreenQueryWin(
                           MMISMS_DEL_QUERY_WIN_ID,
                           TXT_SMS_DELETE,
                           res_common_ic_cancel,
                           res_common_ic_confirm,
                           HandleMessageBoxDelQueryWinMsg);
//#endif
            break;
        }

        case MSG_KEYDOWN_USER_FUNC://fix bug1594837 1595403 1586984
        {
            MMK_SendMsg(win_id, MSG_APP_DOWN, PNULL);
            break;
        }
#endif

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMK_CloseWin(win_id);
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            MMISMS_SetAllMsgMarked(MMISMS_BOX_CHAT, FALSE);
            MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, HandleMessageDateChangeCallBack, FALSE);
            if(PNULL != win_data_ptr)
            {
                win_data_ptr->marked_sms_num = 0;
                SCI_FREE(win_data_ptr);
                win_data_ptr = PNULL;
            }
            break;
        }

        default:
        {
            recode = MMI_RESULT_FALSE;
            break;
        }
    }
    return recode;
}


/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
WINDOW_TABLE( MMISMS_MESSAGE_WIN_TAB ) =
{
    WIN_FUNC((uint32) HandleMessageWinMsg),
    WIN_ID( MMISMS_MESSAGE_LIST_WIN_ID ),
    WIN_HIDE_STATUS,
#ifdef ADULT_WATCH_SUPPORT
    WIN_TITLE(STXT_MAIN_MESSAGE),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_CHATBOX_LISTBOX_CTRL_ID),
#ifndef TOUCH_PANEL_SUPPORT
    WIN_SOFTKEY(TXT_NULL, STXT_OK, STXT_RETURN),
#endif
    END_WIN
};

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : to Enter the list of box
//  Parameter: [In] box_type
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL void EnterSmsMessageWin(
    MMISMS_BOX_TYPE_E        box_type
)
{
    MMI_HANDLE_T winHandle = 0;
    MMISMS_MESSAGE_WINDATA_T *win_data_ptr = PNULL;

    win_data_ptr = (MMISMS_MESSAGE_WINDATA_T*)SCI_ALLOCAZ(sizeof(MMISMS_MESSAGE_WINDATA_T));
    if(PNULL == win_data_ptr)
    {
       TRACE_APP_MESSAGES("WatchSMS Message: win_data_ptr alloc fail");
       return;
    }
    win_data_ptr->box_type = MMISMS_BOX_CHAT;
    win_data_ptr->win_id = MMISMS_MESSAGE_LIST_WIN_ID;
    win_data_ptr->cur_list_ctrl_id = MMISMS_CHATBOX_LISTBOX_CTRL_ID;

    //MMISMS_SetCurType(box_type);
    winHandle = MMK_CreateWin((uint32 *)MMISMS_MESSAGE_WIN_TAB, PNULL);
    if(winHandle == PNULL)
    {
        TRACE_APP_MESSAGES("WatchSMS Message: MMISMS_MESSAGE_WIN_TAB create error");
        SCI_FREE(win_data_ptr);
        return;
    }
    TRACE_APP_MESSAGES("WatchSMS Message: MMISMS_MESSAGE_WIN_TAB create success");
    win_data_ptr->win_handle = winHandle;
    MMK_SetWinUserData(winHandle, (ADD_DATA)win_data_ptr);
}

/*****************************************************************************/
//  Description : to open the message list of box
//  Parameter: [In] box_type
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void WatchSMS_MessageWin_Enter()
{
    BOOLEAN sNotInited = FALSE;
    MMISMS_BOX_TYPE_E box_type = MMISMS_BOX_CHAT;

    sNotInited = MMISMS_IsSimNotInited();
    if(MMISMS_IsOrderOk() ) //判断SMS是否排序完毕
    {
        TRACE_APP_MESSAGES("WatchSMS Message: WatchSMS_MessageWin_Enter sucess");
        EnterSmsMessageWin(box_type);
    }
#ifndef ADULT_WATCH_SUPPORT
    else if(sNotInited)
    {
        TRACE_APP_MESSAGES("WatchSMS Message: WatchSMS_MessageWin_Enter no SIM");
        MMISMS_OpenNoteWin( MMISMS_NOTE_WIN_ID,TXT_SIM_NOT_INIT,0 );
    }
#endif
    else
    {
        TRACE_APP_MESSAGES("WatchSMS Message: WatchSMS_MessageWin_Enter error");
        return;
    }
}

#define SMSDELETE
#ifdef SMSDELETE //w317 delete start
/*****************************************************************************/
//  Description : DeleteOneSmscallback
//  Parameter: [In] None
//             [Out] callback_data
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteOneSmscallback(SMSSRV_CALLBACK_STRUCT_T* callback_data)
{
    return MMK_PostMsg(MMISMS_DELETE_ANIM_WIN_ID, MMISMS_MSG_DELETE_NEXT,PNULL, 0);
}

/*****************************************************************************/
//  Description : HandleDelWaitWinMsg
//  Parameter: [In] win_id msg_id param
//             [Out] MMI_RESULT_E
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelWaitWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM                param    //IN:
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMISMS_DELWAIT_WIN_DATA *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_DELWAIT_WIN_DATA *)MMK_GetWinUserData(win_id);

    if(PNULL == win_data_ptr)
    {
        TRACE_APP_MESSAGES("WatchSMS Message:del message data is pnull");
        return recode;
    }

    TRACE_APP_MESSAGES("WatchSMS Message: HandleDelWaitWinMsg msg_id = 0x%x",msg_id);

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMK_PostMsg(win_id, MMISMS_MSG_DELETE_NEXT,PNULL,0);
            break;

        //当前元素的每条信息逐个删除
        case MMISMS_MSG_DELETE_NEXT:
        {
            MMISMS_ORDER_ID_T cur_del_order = PNULL;

            TRACE_APP_MESSAGES("WatchSMS Message:MMISMS_MSG_DELETE_NEXT");
            cur_del_order = MMISMS_GetCurDeleteData(&win_data_ptr->opearte_data_info);
            if(PNULL == cur_del_order) //delete finish
            {
                MMISMS_DELETE_DATE_INFO data_info = {0};
                win_data_ptr->del_call_back(win_data_ptr->user_data,&data_info);
                break;
            }
            //delete!
            MMISMSAPP_DeleteSms(cur_del_order,DeleteOneSmscallback,PNULL,PNULL);
        }
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMISMS_DELETE_DATE_INFO data_info = {0};
            win_data_ptr->del_call_back(win_data_ptr->user_data,&data_info);
            MMK_CloseWin(win_id);
        }   
            break;

        case MSG_CLOSE_WINDOW:
            SCI_FREE(win_data_ptr);
            break;

        default:
            recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
            break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : DelOneMessageWaitWinEnter
//  Parameter: [In] data_info_ptr user_data
//             [Out] del_call_back
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL void DelOneMessageWaitWinEnter(MMISMS_OPERATE_DATA_INFO_T *data_info_ptr,MMISMS_DEL_CALLBACK del_call_back,void*  user_data)
{
    MMISMS_DELWAIT_WIN_DATA *del_message_data = PNULL;
    MMI_HANDLE_T win_handle = PNULL;

    TRACE_APP_MESSAGES("WatchSMS Message:MMISMS_EnterDelWaitWin");

    if(PNULL == data_info_ptr )
    {
        return;
    }
    del_message_data = SCI_ALLOCAZ(sizeof(MMISMS_DELWAIT_WIN_DATA));
    if(PNULL == del_message_data)
    {
        return;
    }

    SCI_MEMCPY(&del_message_data->opearte_data_info,data_info_ptr,sizeof(MMISMS_OPERATE_DATA_INFO_T));
    del_message_data->del_call_back = del_call_back;
    del_message_data->user_data = user_data;

    //open delete waiting win
    MMISMS_OpenFullScreenWaitingWin( MMISMS_DELETE_ANIM_WIN_ID,0,0,HandleDelWaitWinMsg);
    win_handle = MMK_GetWinHandle( MMK_GetFirstAppletHandle(), MMISMS_DELETE_ANIM_WIN_ID );
    if(win_handle)
    {
        MMK_SetWinUserData(win_handle, del_message_data);
    }
}


/*****************************************************************************/
//  Description : HandleMessageBoxDelQueryWinMsg
//  Parameter: [In] win_id msg_id param
//             [Out] MMI_RESULT_E
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMessageBoxDelQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMISMS_MESSAGE_WINDATA_T *win_data_ptr = PNULL;
    MMISMS_OPERATE_DATA_INFO_T dataInfo = {0};

    win_data_ptr = (MMISMS_MESSAGE_WINDATA_T *)MMK_GetWinUserData(MMISMS_MESSAGE_LIST_WIN_ID);
    if(PNULL == win_data_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    TRACE_APP_MESSAGES("[MMISMS] HandleMessageBoxDelQueryWinMsg msg_id = 0x%x",msg_id);
    switch (msg_id)
    {
        //cancel
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        {
            MMISMS_CancelMarkAllItem(win_data_ptr->cur_list_ctrl_id,MMISMS_BOX_CHAT, &win_data_ptr->marked_sms_num, FALSE);
            MMK_CloseWin(win_id);
            break;
        }

        //delete
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMISMS_SetCurOrderMarked(win_data_ptr->cur_focus_order_id,TRUE);
            dataInfo.box_type = win_data_ptr->box_type;
            DelOneMessageWaitWinEnter(&dataInfo,MessageDelFinishCallback,(void*)win_data_ptr->win_handle);
            break;
        }

        default:
            recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
            break;
    }

    return (recode);
}

 /*****************************************************************************/
//  Description : delete message item call back
//  Parameter: [In] user_data
//             [In] data_info_ptr
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MessageDelFinishCallback(void *user_data,MMISMS_DELETE_DATE_INFO *data_info_ptr)
{
    MMISMS_SetAllMsgMarked(MMISMS_BOX_CHAT, FALSE);

    if(MMISMS_DEL_FAIL == data_info_ptr->ret_value)
    {
        //tip delete fail
        MMISMS_OpenNoteWin( MMISMS_NOTE_WIN_ID,TXT_COMMON_DEL_FAIL,0 );
    }
    else
    {
        //tip delete sucess
        MMISMS_OpenNoteWin( MMISMS_NOTE_WIN_ID,TXT_DELETED,0 );
    }

    MMK_CloseWin(MMISMS_DEL_QUERY_WIN_ID);
    MMK_CloseWin(MMISMS_DELETE_ANIM_WIN_ID);
    return MMI_RESULT_TRUE;
}

#endif

#ifdef ADULT_WATCH_SUPPORT
// TODO:需要review和更关图片资源
/*****************************************************************************/
//  Description : GetUnreadPhotoIDByContactID
//  Parameter: [In] contactImageId
//             [Out] unread photo id
//  Author: qi.liu1
//  Note:将联系人图片ID转换成未读短信联系人ID
/*****************************************************************************/
LOCAL uint32 GetUnreadPhotoIDByContactID(MMI_IMAGE_ID_T      contactImageId)
{
    MMI_IMAGE_ID_T      unread_id = NULL;
    switch(contactImageId)
    {
        case IAMGE_CONTACT_PORTRAIT_FATHER:
        {
            unread_id = IAMGE_CONTACT_PORTRAIT_FATHER;
            break;
        }
        case IAMGE_CONTACT_PORTRAIT_MOTHER:
        {
            unread_id = IAMGE_CONTACT_PORTRAIT_MOTHER;
            break;
        }
        case IAMGE_CONTACT_PORTRAIT_GRANDPA:
        {
            unread_id = IAMGE_CONTACT_PORTRAIT_GRANDPA;
            break;
        }
        case IAMGE_CONTACT_PORTRAIT_GRANDMA:
        {
            unread_id = IAMGE_CONTACT_PORTRAIT_GRANDMA;
            break;
        }
        case IAMGE_CONTACT_PORTRAIT_TEACHER:
        {
            unread_id = IAMGE_CONTACT_PORTRAIT_TEACHER;
            break;
        }
        case IAMGE_CONTACT_PORTRAIT_STRANGER:
        {
            unread_id = IAMGE_CONTACT_PORTRAIT_FATHER;
            break;
        }
        default:
            break;
    }

    return unread_id;
}
#endif
