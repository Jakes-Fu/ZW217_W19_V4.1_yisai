/*****************************************************************************
 ** File Name:   watch_sms_chat.c
 ** Author:      qi.liu1
 ** Date:        20/3/2020
 ** Copyright:   2020 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description: This file is used to describe chat win
 *****************************************************************************
 **                         Important Edit History                           *
 ** -------------------------------------------------------------------------*
 *  ** DATE           NAME             DESCRIPTION                           *
 *  21/3/2020       qi.liu1                presetlist
 ****************************************************************************/

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmicom_trace.h"
#include "mmisms_export.h"
#include "watch_sms_chat.h"
#include "watch_sms_presetlist.h"
#include "mmisms_read.h"
#include "mmisms_edit.h"

#ifndef MMI_GUI_STYLE_TYPICAL
#include "guibutton.h"
#endif
#include "mmisms_text.h"
#include "mmisms_image.h"
#include "mmisms_anim.h"
#include "mmisms_id.h"
#include "guimenu.h"
#include "guilistbox.h"
#include "guitext.h"
#include "mmi_text.h"
#include "mmidisplay_data.h"
#include "mmiidle_export.h"
#include "tb_dal.h"
#include "guilabel.h"
#include "guimsgbox.h"
#include "mmisms_menutable.h"
#include "mmisms_commonui.h"
#include "guiform.h"
#include "mmienvset_export.h"

#include "mmisms_set.h"
#include "mmisms_send.h"
#include "mmisms_appcontrol.h"
#include "mmisms_delete.h"
#include "mmialarm_export.h"
#include "mmisms_receive.h"

#include "mmifmm_export.h"
#include "mmismsapp_interface.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
// define the margin between text and bubble
#define MMISMS_PROMPT_STR_LENGTH      (52)

#define SMS_CHAT_REPLY_BG_RECT                DP2PX_RECT(0,179,239,239)

#ifdef SCREEN_SHAPE_CIRCULAR
#ifdef ADULT_WATCH_SUPPORT
#define SMS_CHAT_REPLY_BUTTON_RECT       DP2PX_RECT(50,179,120,229)//bug: 1921533 
#define SMS_CHAT_EMOJI_BUTTON_RECT       DP2PX_RECT(121,179,191,229)
#define MMISMS_CHAT_TEXT_MARGIN_RIGHT       DP2PX_VALUE(36)
#define MMISMS_CHAT_TEXT_MARGIN_LEFT          DP2PX_VALUE(36)
#define SMS_CHAT_FORM_BG_RECT                 DP2PX_RECT(0,48,239,179)
#define MMI_SMS_GREEN_COLOR	             0x4CAF50
#define MMISMS_CHAT_BUBBLE_GAP         DP2PX_VALUE(12)    //气泡间距 显示日期的空间
#define MMISMS_CHAT_TEXT_MARGIN_TOP           DP2PX_VALUE(8) //修改text与气泡上方的间距
#define MMISMS_CHAT_TEXT_MARGIN_BOTTOM   DP2PX_VALUE(8) //修改text与气泡下方的间距
#define MMISMS_TEXT_CHAR_SPACE        DP2PX_VALUE(2)//字符间距
#define MMISMS_SMS_MIN_TEXT_BG_WIDTH   DP2PX_VALUE(40)
#define MMISMS_SMS_MIN_MARGIN          DP2PX_VALUE(36)
#define MMISMS_CHAT_SEND_BUBBLE_WIDTH    DP2PX_VALUE(200)  //发送气泡长度
#define MMISMS_CHAT_RECEIVE_BUBBLE_WIDTH    DP2PX_VALUE(204)  //接收气泡长度
#else
#define SMS_CHAT_FORM_BG_RECT                 DP2PX_RECT(0,30,239,183)
#define SMS_CHAT_REPLY_BUTTON_RECT       DP2PX_RECT(0,183,239,239)
#define MMISMS_CHAT_TEXT_MARGIN_RIGHT       DP2PX_VALUE(8)
#define MMISMS_CHAT_TEXT_MARGIN_LEFT          DP2PX_VALUE(8)
#define MMISMS_CHAT_BUBBLE_WIDTH    DP2PX_VALUE(168)  //气泡长度
#define MMISMS_CHAT_BUBBLE_GAP         DP2PX_VALUE(12)    //气泡间距 显示日期的空间
#define MMISMS_CHAT_TEXT_MARGIN_BOTTOM   DP2PX_VALUE(16) //修改text与气泡下方的间距
#define MMISMS_CHAT_TEXT_MARGIN_TOP           DP2PX_VALUE(16) //修改text与气泡上方的间距
#endif
#else
#define MMISMS_CHAT_BUBBLE_WIDTH    DP2PX_VALUE(228)  //气泡长度
#define SMS_CHAT_REPLY_BUTTON_RECT       DP2PX_RECT(0,179,119,239)
#define SMS_CHAT_EMOJI_BUTTON_RECT       DP2PX_RECT(121,179,239,239)
#define MMISMS_CHAT_TEXT_MARGIN_RIGHT       DP2PX_VALUE(5)
#define MMISMS_CHAT_TEXT_MARGIN_LEFT          DP2PX_VALUE(15)
#define SMS_CHAT_FORM_BG_RECT                 DP2PX_RECT(0,0,239,179)
#define MMISMS_CHAT_BUBBLE_GAP         DP2PX_VALUE(20)    //气泡间距
#define MMISMS_CHAT_TEXT_MARGIN_BOTTOM   DP2PX_VALUE(22) //修改text与气泡下方的间距
#define MMISMS_CHAT_TEXT_MARGIN_TOP           DP2PX_VALUE(10) //修改text与气泡上方的间距
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct 
{
    MMI_WIN_ID_T            win_id;
    MMISMS_CHAT_ITEM_T      chat_item[MMISMS_CHAT_MAX_ITEM];
    MMISMS_BCD_NUMBER_T     chat_number;
    MMISMS_READ_MSG_T   read_result;
}MMISMS_CHAT_WINDATA_T;

extern const MMISMS_EMOJI_INFO_T s_emoji_image_text_convert_table[];
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : draw one chat bubble 
//  Parameter: [In] win Id
//             [In] both_rect
//             [In] order_id
//             [Out] None
//  Author: qi.liu1
//  Note:SetChatItemParam
/*****************************************************************************/
LOCAL void DrawOneChatBubbleItem(
    MMI_WIN_ID_T win_id,
    GUI_BOTH_RECT_T both_rect,
    MMISMS_ORDER_ID_T order_id
);

/*****************************************************************************/
//  Description : to handle the message of show chat SMS window 
//  Parameter: [In] win Id
//             [In] msg_id
//             [In] param
//             [Out] None
//  Author: qi.liu1
//  Note:SetChatItemParam
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleShowChatSmsWinMsg(
    MMI_WIN_ID_T        win_id, 
    MMI_MESSAGE_ID_E msg_id,
    DPARAM            param
);

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
WINDOW_TABLE( MMISMS_SHOW_CHAT_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleShowChatSmsWinMsg),
    WIN_ID(MMISMS_SHOW_CHAT_SMS_WIN_ID),
    WIN_HIDE_STATUS,
#ifdef ADULT_WATCH_SUPPORT
    WIN_TITLE(TXT_NULL),
#endif
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID),
    CREATE_BUTTON_CTRL( PNULL,MMISMS_SHOW_CHAT_SMS_CHAT_BUTTON_CTRL_ID ),
#if (defined SCREEN_SHAPE_SQUARE) || (defined ADULT_WATCH_SUPPORT)
    CREATE_BUTTON_CTRL( PNULL,MMISMS_SHOW_CHAT_SMS_EMOJI_BUTTON_CTRL_ID ),
#endif
#ifndef TOUCH_PANEL_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
#endif
    END_WIN
};

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description : read one chat SMS callback
//  Parameter: [In] callback_data
//             [Out] None
//  Author: qi.liu1
//  Note: send one msg :MSG_SMS_FINISH_READ
/*****************************************************************************/
LOCAL BOOLEAN ChatWinReadSmsCallback(SMSSRV_CALLBACK_STRUCT_T *callback_data)
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMISMS_CHAT_WINDATA_T *win_data_ptr = (MMISMS_CHAT_WINDATA_T *)MMK_GetWinAddDataPtr(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if(PNULL == callback_data)
    {
        TRACE_APP_MESSAGES("WatchSMS Chat:read SMS callback_data = PNULL");
        return FALSE;
    }

    if(MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID))  //此窗口如已关闭，则win_data_ptr已被释放
    {
        win_data_ptr->read_result = MMISMS_GetReadResult();
        if(callback_data->result)
        {
            order_id = MMISMS_GetChatOperOrderID();
            MMK_SendMsg(
                        (MMI_HANDLE_T)win_data_ptr->win_id,
                        MSG_SMS_FINISH_READ,
                        (ADD_DATA)order_id
                       );
        }
        else
        {
            MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
            MMISMS_ClearOperInfo();
        }
    }
    return TRUE;
}

 /*****************************************************************************/
//  Description : read a receive new SMS callback
//  Parameter: [In] event_info_ptr
//             [Out] None
//  Author: qi.liu1
//  Note: when a new SMS come, do this callback
/*****************************************************************************/
LOCAL void ChatReadWinNewEventCallback(MMIEVENT_INFO_T *event_info_ptr)
{
    MMISMS_NEW_SMS_INFO_T *new_sms_info_ptr = PNULL;
    MMIPB_BCD_NUMBER_T mt_num = {0};
    MN_SMS_MT_SMS_T *mt_sms_ptr = PNULL;
    BOOLEAN is_num_compare = FALSE;
    MN_DUAL_SYS_E           dual_sys = MN_DUAL_SYS_MAX;
    MN_SMS_STORAGE_E        storage = MN_SMS_NO_STORED;
    MN_SMS_RECORD_ID_T        record_id = 0;
    MMISMS_CHAT_WINDATA_T *win_data_ptr = (MMISMS_CHAT_WINDATA_T*)MMK_GetWinAddDataPtr(MMISMS_SHOW_CHAT_SMS_WIN_ID);

    if(PNULL == event_info_ptr || PNULL == win_data_ptr)
    {
        return;
    }
    new_sms_info_ptr = (MMISMS_NEW_SMS_INFO_T *)event_info_ptr->param_ptr;
    if(PNULL == new_sms_info_ptr)
    {
        TRACE_APP_MESSAGES("WatchSMS Chat:read NEW SMS new_sms_info_ptr = PNULL");
        return;
    }

    TRACE_APP_MESSAGES("WatchSMS Chat: NewSMS enter!");

    dual_sys = new_sms_info_ptr->dual_sys;
    storage = new_sms_info_ptr->storage;
    record_id = new_sms_info_ptr->record_id;

    mt_sms_ptr = &new_sms_info_ptr->mt_sms;
    mt_num.number_len = mt_sms_ptr->origin_addr_t.num_len;
    mt_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(mt_sms_ptr->origin_addr_t.number_type,MN_NUM_PLAN_UNKNOW);
    SCI_MEMCPY(
               mt_num.number,
               mt_sms_ptr->origin_addr_t.party_num,
               mt_num.number_len
               );

    if(0 == MMIAPIPB_BcdSmartCmp(&mt_num,(MMIPB_BCD_NUMBER_T *)&win_data_ptr->chat_number))
    {
        is_num_compare = TRUE;
    }

    if( !MMK_IsFocusWin(MMISMS_READSMS_WAIT_WIN_ID)
        && MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID)
        &&is_num_compare
      )
    {
        // to modify the status from to_be_read to READ
        MMISMS_SetOperStatus(MMISMS_READ_CHAT_NUM_SMS);

        MNSMS_UpdateSmsStatusEx(dual_sys, storage, record_id, MN_SMS_MT_READED);
        MMISMS_SetNeedUpdateInfo();

        if(new_sms_info_ptr->cur_chat_order_id)
		{
           new_sms_info_ptr->cur_chat_order_id->flag.is_need_update = TRUE;
		}

        // add the readed SMS to global
        MMISMS_ChatSetMTGlobalReadMsg(
            dual_sys,
            &new_sms_info_ptr->user_data,
            storage,
            record_id,
            mt_sms_ptr->dcs.alphabet_type,
            &mt_sms_ptr->time_stamp_t,
            &mt_sms_ptr->origin_addr_t,
            MMISMS_MT_HAVE_READ
            );

        win_data_ptr->read_result = MMISMS_GetReadResult();
        if((new_sms_info_ptr->cur_chat_order_id != PNULL) && ((new_sms_info_ptr->cur_chat_order_id->flag.is_concatenate_sms && new_sms_info_ptr->cur_chat_order_id->flag.is_receive_all)
            || (!new_sms_info_ptr->cur_chat_order_id->flag.is_concatenate_sms))
        )
        {
            //light and ring
            MMIDEFAULT_TurnOnBackLight();
            MMISMS_SetNewSMSRingFlag(TRUE);
            MMISMS_PlayNewSMSRing(new_sms_info_ptr->dual_sys);
        }

        MMK_SendMsg(
                    MMISMS_SHOW_CHAT_SMS_WIN_ID,
                    MSG_SMS_NEW_SMS,
                    (ADD_DATA)new_sms_info_ptr->cur_chat_order_id
                    );

#ifdef DROPDOWN_NOTIFY_SHORTCUT
        {
            MMIEVENT_INFO_T event_info = {0};
            MMISMS_NEW_SMS_INFO_T sms_info = {0};
            event_info.event_type = SMS_READSMS_EVENT;
            event_info.param_ptr = &sms_info;
            sms_info.storage = new_sms_info_ptr->cur_chat_order_id->flag.storage;
            sms_info.record_id = new_sms_info_ptr->cur_chat_order_id->record_id;
            sms_info.cur_chat_order_id = new_sms_info_ptr->cur_chat_order_id;
            SCI_TRACE_LOW("SMSNotify: trigger app readed sms event");
            MMIFW_TriggerEvent(&event_info);
        }
#endif
    }

}

/*****************************************************************************/
//  Description : the send new SMS callback
//  Parameter: [In] event_info_ptr
//             [Out] None
//  Author: qi.liu1
//  Note: when send a new SMS, do this callback;when receive, do nothing
/*****************************************************************************/
LOCAL void ChatReadWinDataChangeCallback(MMIEVENT_INFO_T *event_info_ptr)
{
    MMI_WIN_ID_T win_id = MMISMS_SHOW_CHAT_SMS_WIN_ID;
    MMISMS_ORDER_ID_T chat_add_order_id = PNULL;
    MMISMS_DATACHANGE_TYPE_E *data_change_ptr = PNULL;
    MMISMS_CHAT_WINDATA_T *win_data_ptr = (MMISMS_CHAT_WINDATA_T*)MMK_GetWinAddDataPtr(win_id);
    BOOLEAN is_num_compare = FALSE;
    if(PNULL == event_info_ptr)
    {
       TRACE_APP_MESSAGES("WatchSMS Chat: chatbox event ptr PNULL");
       return;
    }

    TRACE_APP_MESSAGES("WatchSMS Chat: DataChange enter! event_type=%d",event_info_ptr->event_type);

    if(SMS_DATA_CHANGE_EVENT == event_info_ptr->event_type)
    {
       data_change_ptr = (MMISMS_DATACHANGE_TYPE_E *)event_info_ptr->param_ptr;
       if(MMISMS_DATACHANGE_TYPE_ADD == *data_change_ptr)
       {
           chat_add_order_id = MMISMS_GetChatAddOrderID();
           if(chat_add_order_id != PNULL)
           {
               if(0 == MMIAPIPB_BcdSmartCmp((MMIPB_BCD_NUMBER_T *)&chat_add_order_id->number.bcd_number,(MMIPB_BCD_NUMBER_T *)&win_data_ptr->chat_number))
               {
                   is_num_compare = TRUE;
                   win_data_ptr->read_result = MMISMS_GetReadResult();
                   TRACE_APP_MESSAGES("WatchSMS Chat: read_result mo_mt_type=%d",win_data_ptr->read_result);
               }

               if( !MMK_IsFocusWin(MMISMS_READSMS_WAIT_WIN_ID)
                   && MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID)
                   && is_num_compare
                 )
               {
                   MMK_SendMsg(
                        win_id,
                        MSG_SMS_NEW_SMS,
                        (ADD_DATA)chat_add_order_id
                        );
               }
               MMISMS_SetChatAddOrderID(PNULL);
           }
       }
    }
}

/*****************************************************************************/
//  Description : display chat win reply button
//  Parameter: [In] ctrl_id
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL void  SMS_DisplayChatButton(
    MMI_CTRL_ID_T    ctrl_id
)
{
    GUI_BG_T    bg_info ={0};
    GUI_RECT_T  button_rect = SMS_CHAT_REPLY_BUTTON_RECT;
    MMI_STRING_T    button_text = {0};
    GUI_FONT_ALL_T  font_info = {0};
    MMI_STRING_T reply_str = {0};

    MMI_GetLabelTextByLang(TXT_SMS_CHAT_REPLY, &reply_str);

    font_info.font = WATCH_DEFAULT_NORMAL_FONT;
    font_info.color = MMI_WHITE_COLOR;

    button_text.wstr_ptr = reply_str.wstr_ptr;
    button_text.wstr_len = reply_str.wstr_len;

    bg_info.bg_type = GUI_BG_IMG;
#ifdef SCREEN_SHAPE_CIRCULAR
    bg_info.img_id = IMAGE_SMS_CHAT_REPLY;
#else
    bg_info.img_id = IMAGE_SMS_CHAT_REPLY_BG;
#endif
    GUIBUTTON_SetRect(ctrl_id, &button_rect);
//    GUIBUTTON_SetFg(ctrl_id, &fg_info);
    GUIBUTTON_SetBg(ctrl_id, &bg_info);
    GUIBUTTON_SetTextAlign(ctrl_id, ALIGN_HVMIDDLE);
#ifdef SCREEN_SHAPE_SQUARE
    GUIBUTTON_SetText(ctrl_id, button_text.wstr_ptr,button_text.wstr_len );
#endif
    GUIBUTTON_SetFont(ctrl_id, &font_info);
    GUIBUTTON_SetVisible(ctrl_id, TRUE, FALSE);
}

/*****************************************************************************/
//  Description : display chat win reply button bg
//  Parameter: [In] ctrl_id
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL void  SMS_DisplayChatButtonBg(
    MMI_CTRL_ID_T    ctrl_id
)
{
    GUI_BG_T    bg_info ={0};
    GUI_RECT_T  form_rect = SMS_CHAT_REPLY_BG_RECT;

    bg_info.bg_type = GUI_BG_COLOR;
    bg_info.color = MMI_BLACK_COLOR;
    CTRLFORM_SetRect(ctrl_id, &form_rect);
    CTRLFORM_SetBg(ctrl_id, &bg_info);
    GUIFORM_SetVisible(ctrl_id, TRUE, FALSE);
}

#if (defined SCREEN_SHAPE_SQUARE) || (defined ADULT_WATCH_SUPPORT)
/*****************************************************************************/
//  Description : display chat win reply button
//  Parameter: [In] ctrl_id
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL void  SMS_DisplayEmojiButton(
    MMI_CTRL_ID_T    ctrl_id
    )
{
    GUI_BG_T    fg_info ={0};
    GUI_BG_T    bg_info ={0};
    GUI_RECT_T  button_rect = SMS_CHAT_EMOJI_BUTTON_RECT;
    MMI_STRING_T    button_text = {0};
    GUI_FONT_ALL_T  font_info = {0};
    MMI_STRING_T reply_str = {0};

    MMI_GetLabelTextByLang(TXT_SMS_EMOJI, &reply_str);

    font_info.font = WATCH_DEFAULT_NORMAL_FONT;
    font_info.color = MMI_WHITE_COLOR;

    button_text.wstr_ptr = reply_str.wstr_ptr;
    button_text.wstr_len = reply_str.wstr_len;

    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMAGE_SMS_CHAT_EMOJI_BG;

    GUIBUTTON_SetRect(ctrl_id, &button_rect);
    //    GUIBUTTON_SetFg(ctrl_id, &fg_info);
    GUIBUTTON_SetBg(ctrl_id, &bg_info);
    GUIBUTTON_SetTextAlign(ctrl_id, ALIGN_HVMIDDLE);
//#ifndef ADULT_WATCH_SUPPORT
    GUIBUTTON_SetText(ctrl_id, button_text.wstr_ptr,button_text.wstr_len );
//#endif
    GUIBUTTON_SetFont(ctrl_id, &font_info);
    GUIBUTTON_SetVisible(ctrl_id, TRUE, FALSE);
}
#endif

/*****************************************************************************/
//  Description : display chat win bg
//  Parameter: [In] ctrl_id
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL void  SMS_DisplayChatBg(
    MMI_CTRL_ID_T    ctrl_id        //IN:
)
{
    GUI_BG_T    bg_info ={0};
    GUI_RECT_T  form_rect = SMS_CHAT_FORM_BG_RECT;

    bg_info.bg_type = GUI_BG_COLOR;
    bg_info.color = MMI_BLACK_COLOR;
    CTRLFORM_SetRect(ctrl_id, &form_rect);
    CTRLFORM_SetBg(ctrl_id, &bg_info);
    GUIFORM_SetVisible(ctrl_id, TRUE, FALSE);
}

/*****************************************************************************/
//  Description : close ctrl func
//  Parameter: [In] win_data_ptr
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL void ChatWinCloseCtrlFunc (MMISMS_CHAT_WINDATA_T *win_data_ptr)
{
    if (win_data_ptr != PNULL)
    {
        SCI_FREE(win_data_ptr);
    }
    MMIFW_SetEventListener(SMS_NEWSMS_EVENT, ChatReadWinNewEventCallback, FALSE);
    MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, ChatReadWinDataChangeCallback, FALSE);
}

/*****************************************************************************/
//  Description : set chat win index
//  Parameter: [In] win_data_ptr
//             [In] index
//             [In] is_active
//             [In] order_id
//             [Out] None
//  Author: qi.liu1
//  Note: 
/*****************************************************************************/
LOCAL void SetChatWinIndex(
    MMISMS_CHAT_WINDATA_T *win_data_ptr,
    uint32 index,
    BOOLEAN is_active,
    MMISMS_ORDER_ID_T order_id
)
{
    if(PNULL == win_data_ptr)
    {
        return;
    }
    win_data_ptr->chat_item[index].is_valid = is_active;
    win_data_ptr->chat_item[index].cur_order_id = order_id;
    win_data_ptr->chat_item[index].parent_ctrl_id = MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID + index;

    TRACE_APP_MESSAGES("WatchSMS Chat: SetChatWinIndex: index is %d,is_active is %d",index, is_active);
}

LOCAL void UpdateChatItemDisplayAfterDelSingle(MMISMS_CHAT_WINDATA_T *win_data_ptr)
{
    MMI_CTRL_ID_T del_child_ctrl = 0;
    int32 i = 0;
    uint16 deleteIndex = 0;
    BOOLEAN isHaveItem = FALSE;

    MMI_CTRL_ID_T ctrl_id = 0;

    if(PNULL == win_data_ptr)
    {
        TRACE_APP_MESSAGES("WatchSMS chat: win_data_ptr = PNULL!");
        return ;
    }
    deleteIndex = MMISMS_GetDelIndex();
    TRACE_APP_MESSAGES("WatchSMS chat: UpdateChatItem deleteIndex = %d !",deleteIndex);
    if (deleteIndex < MMISMS_CHAT_MAX_ITEM)
    {
        del_child_ctrl = win_data_ptr->chat_item[deleteIndex].parent_ctrl_id;
    }
    else
    {
        return;
    }
    GUIFORM_DestroyDynaChildCtrl(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, del_child_ctrl);
    SCI_MEMSET(&win_data_ptr->chat_item[deleteIndex], 0x0, sizeof(MMISMS_CHAT_ITEM_T));
    for (i = 0; i < MMISMS_CHAT_MAX_ITEM; i++)
    {
        ctrl_id = GUIFORM_GetChildCtrlId(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, (i + 1));
        if (0 == ctrl_id || (ctrl_id < MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID || ctrl_id >= MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID + MMISMS_CHAT_MAX_ITEM))
        {
            ctrl_id = GUIFORM_GetChildCtrlId(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, i);
            if (ctrl_id >= MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID && ctrl_id < MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID + MMISMS_CHAT_MAX_ITEM)
            {
                isHaveItem = TRUE;
                break;
            }

        }
    }
    if (isHaveItem)
    {
        if (MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID))
        {
            MMK_SetAtvCtrl(MMISMS_SHOW_CHAT_SMS_WIN_ID, ctrl_id);
        }
#ifdef TOUCH_PANEL_SUPPORT         
        if (!MMK_IsFocusWin(MMISMS_SHOW_CHAT_SMS_WIN_ID))
        {
            GUIFORM_ForceLayout(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID);
        }
#endif        
    }

    deleteIndex = MMISMS_CHAT_MAX_ITEM;
    MMISMS_SetDelIndex(deleteIndex);
    MMISMS_SetGlobalChatItem(win_data_ptr->chat_item,MMISMS_CHAT_MAX_ITEM);
}

/*****************************************************************************/
//  Description : to handle the message of show chat SMS window
//  Parameter: [In] win_id , msg_id , param
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleShowChatSmsWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM            param
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    uint8 num_len = 0;
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    MMISMS_ORDER_ID_T oper_order_id = PNULL;
    MMI_CTRL_ID_T ctrl_id = MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID;
    MMISMS_CHAT_WINDATA_T *win_data_ptr = (MMISMS_CHAT_WINDATA_T*)MMK_GetWinAddDataPtr(win_id);
#ifdef ADULT_WATCH_SUPPORT
    MMI_STRING_T name = {0};
    wchar temp_arr[MMISMS_PBNAME_MAX_LEN + 1] = {0};
#endif

    TRACE_APP_MESSAGES("WatchSMS Chat: HandleShowChatSmsWinMsg msg_id = 0x%x",msg_id);
    if(PNULL == win_data_ptr)
    {
        TRACE_APP_MESSAGES("WatchSMS Chat: ShowChatSmsWin win_data_ptr pnull");
        return recode;
    }
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
#ifdef  ADULT_WATCH_SUPPORT
            //set title text color
            {
                GUI_COLOR_T      font_color = MMI_GREEN_COLOR;
                GUIWIN_SetTitleFontColor(win_id,font_color);
            }
#endif
            cur_order_id = MMISMS_GetCurOperationOrderId();
            win_data_ptr->win_id = win_id;

            //set bg and button
            SMS_DisplayChatBg( MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID );
            SMS_DisplayChatButton( MMISMS_SHOW_CHAT_SMS_CHAT_BUTTON_CTRL_ID );

#if (defined SCREEN_SHAPE_SQUARE) || (defined ADULT_WATCH_SUPPORT)
            SMS_DisplayEmojiButton( MMISMS_SHOW_CHAT_SMS_EMOJI_BUTTON_CTRL_ID );
#endif
            SCI_MEMSET(&win_data_ptr->chat_item, 0x0, sizeof(MMISMS_CHAT_ITEM_T) * MMISMS_CHAT_MAX_ITEM);
#ifdef ADULT_WATCH_SUPPORT
            name.wstr_ptr = temp_arr;
            MMISMS_GetNameInfo((MMISMS_STATE_T *)cur_order_id, &name, MMISMS_PBNAME_MAX_LEN);
            GUIWIN_SetTitleText(win_id, name.wstr_ptr, name.wstr_len, FALSE);
#endif
            //read SMS
            if(cur_order_id != PNULL && MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
            {
                SCI_MEMSET(&win_data_ptr->read_result,0,sizeof(MMISMS_READ_MSG_T));
                if (!MMISMSAPP_ReadChatSms(cur_order_id,
                        ChatWinReadSmsCallback,
                        PNULL,
                        PNULL))
                {
                    MMK_CloseWin(win_id);
                    break;
                }
            }
            else if(cur_order_id != PNULL&& MMISMS_TYPE_MMS == cur_order_id->flag.msg_type)//if type is not SMS ,load next chat
            {
                MMK_PostMsg(MMISMS_SHOW_CHAT_SMS_WIN_ID,  MSG_SMS_FINISH_READ, (ADD_DATA)cur_order_id, 0);
            }

            MMIFW_SetEventListener(SMS_NEWSMS_EVENT, ChatReadWinNewEventCallback, TRUE);
            MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, ChatReadWinDataChangeCallback, TRUE);
            break;
        }

        case MSG_FULL_PAINT:
            {
                GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
                GUI_RECT_T rect={0, 0, MMI_MAINSCREEN_WIDTH - 1, MMI_MAINSCREEN_HEIGHT - 1};

                GUI_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
            }
            break;

        case MSG_SMS_FINISH_READ:
        {
            oper_order_id = (MMISMS_ORDER_ID_T)param;
            if (PNULL == oper_order_id)
            {
                break;
            }

            if(MMISMS_TYPE_SMS == oper_order_id->flag.msg_type)
            {
                //load one chat message bubble 加载一条聊天气泡
                DrawOneChatBubbleItem(win_id, both_rect, oper_order_id);
                MMISMS_SetGlobalChatItem(win_data_ptr->chat_item,MMISMS_CHAT_MAX_ITEM);
                if (MMK_IsFocusWin(win_id))
                {
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
                }
            }

            //continue read next chat SMS
            if (PNULL != oper_order_id->next_chat_ptr)
            {
                if (MMISMS_TYPE_SMS == oper_order_id->next_chat_ptr->flag.msg_type)
                {
                    SCI_MEMSET(&win_data_ptr->read_result,0,sizeof(MMISMS_READ_MSG_T));
                    MMISMSAPP_ReadChatSms(oper_order_id->next_chat_ptr,
                            ChatWinReadSmsCallback,
                            PNULL,
                            PNULL
                        );
                }
                else if( MMISMS_TYPE_MMS == oper_order_id->next_chat_ptr->flag.msg_type)//if type is not SMS ,load next chat 
                {
                    MMK_PostMsg(MMISMS_SHOW_CHAT_SMS_WIN_ID,  MSG_SMS_FINISH_READ, (ADD_DATA)oper_order_id->next_chat_ptr, 0);
                }
            }
            else
            {
                //finish load SMS, close Waiting win
                MMISMS_UpdateUnreadOrder();
                MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
                MMISMS_ClearOperInfo();
            }
            break;
        }

        //receive or send one SMS, load new SMS chat bubble
        case MSG_SMS_NEW_SMS:
        {
            oper_order_id = (MMISMS_ORDER_ID_T)param;
            if (PNULL == oper_order_id)
            {
                break;
            }
            MMK_CloseWin(MMISMS_PRESET_LIST_WIN_ID);//bug:1382898
            TRACE_APP_MESSAGES("WatchSMS chat: msg MSG_SMS_NEW_SMS enter!");
            //load one chat message bubble
            DrawOneChatBubbleItem(win_id, both_rect, oper_order_id);
            MMISMS_SetGlobalChatItem(win_data_ptr->chat_item,MMISMS_CHAT_MAX_ITEM);
            if (MMK_IsFocusWin(win_id))
            {
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
            }
            break;
        }

        case MMISMS_MSG_UPDATELIST: //when delete one  chat bubble or send one,update index
        {
            TRACE_APP_MESSAGES("WatchSMS chat: msg MMISMS_MSG_UPDATELIST enter!");
            UpdateChatItemDisplayAfterDelSingle(win_data_ptr);
            MMK_CloseWin(MMISMS_PRESET_LIST_WIN_ID);//1382898
            if (MMK_IsFocusWin(win_id))
            {
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
            }
            break;
        }

#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
	{
            ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;
            if(MMISMS_SHOW_CHAT_SMS_CHAT_BUTTON_CTRL_ID == ctrl_id)
            {
                //enter preset list
                WatchSMS_PresetListWin_Enter(PNULL);
            }
            else if(MMISMS_SHOW_CHAT_SMS_EMOJI_BUTTON_CTRL_ID == ctrl_id)
            {
                //enter emoji list
                MMISMS_OpenEmojiWin();
            }
            break;
	}
#endif

        case MSG_APP_OK:
        case MSG_CTL_OK:
        {
            //enter preset list
            WatchSMS_PresetListWin_Enter(PNULL);
            break;
        }

        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        {
            //enter emoji list
            MMISMS_OpenEmojiWin();
            break;
        }

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMK_CloseWin( win_id );
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            MMK_CloseWin(win_id);
#ifdef ADULT_WATCH_SUPPORT
            MMISMS_SetExceedUnreadNeedUpdate();
            MMISMS_UpdateUnreadOrder();
#endif
            ChatWinCloseCtrlFunc(win_data_ptr);
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

/*****************************************************************************/
//  Description : get chat win index type
//  Parameter: [In] win_data_ptr , order_id
//             [Out] index_ptr , index type
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL MMISMS_CHAT_INDEX_E GetChatWinIndexAndType(
    MMISMS_CHAT_WINDATA_T *win_data_ptr,
    uint32 *index_ptr,
    MMISMS_ORDER_ID_T order_id
)
{
    uint32 max = MMISMS_CHAT_MAX_ITEM;
    uint32 i = 0;
    MMISMS_CHAT_INDEX_E result = MMISMS_CHAT_INDEX_MAX;
    MMISMS_ORDER_ID_T cur_longsms_order_id = PNULL;

    //long sms
    if (order_id->flag.is_concatenate_sms)
    {
        for (i = 0; i < max; i++)
        {
            if ((win_data_ptr->chat_item[i].is_valid)
                    && (PNULL != win_data_ptr->chat_item[i].cur_order_id)
               )
            {
                cur_longsms_order_id = win_data_ptr->chat_item[i].cur_order_id;
                {
                    if (MMISMS_HaveSamePart(cur_longsms_order_id, order_id))
                    {
                        *index_ptr = i;
                        return MMISMS_CHAT_INDEX_SAME_LONG_SMS;
                    }
                }
            }
        }
    }

    for (i = 0; i < max; i++)
    {
        if (!win_data_ptr->chat_item[i].is_valid)
        {
            *index_ptr = i;
            result = MMISMS_CHAT_INDEX_NEW;
            break;
        }
    }
    TRACE_APP_MESSAGES("WatchSMS Chat: GetChatWinIndexAndType index = %d,result = %d", i, result);
    return result;
}

/*****************************************************************************/
//  Description : enter SMS chat  win
//  Parameter: [In] cur_order_id
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void WatchSMS_ChatWin_Enter(MMISMS_ORDER_ID_T cur_order_id)
{
    MMI_HANDLE_T win_handle = 0;
    MMISMS_CHAT_WINDATA_T *win_data_ptr = PNULL;

    win_data_ptr = (MMISMS_CHAT_WINDATA_T*)SCI_ALLOCAZ(sizeof(MMISMS_CHAT_WINDATA_T));

    if (win_data_ptr == PNULL)
    {
        TRACE_APP_MESSAGES("WatchSMS Chat: ChatWin_Enter malloc error!");
        return;
    }

    if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
    {
        SCI_MEMSET(&win_data_ptr->chat_number, 0x0, sizeof(MMISMS_BCD_NUMBER_T));
        SCI_MEMCPY(&win_data_ptr->chat_number, &cur_order_id->number.bcd_number, sizeof(MMISMS_BCD_NUMBER_T));
    }

    MMISMS_SetGlobalChatNumber(win_data_ptr->chat_number,PNULL);
    win_handle  = MMK_CreateWin((uint32*)MMISMS_SHOW_CHAT_WIN_TAB, win_data_ptr);
    MMISMS_SetGlobalOperationOrderId(cur_order_id);
    TRACE_APP_MESSAGES("WatchSMS Chat: MMISMS_SHOW_CHAT_WIN_TAB create success");
}

/*****************************************************************************/
//  Description : 处理阅读界面来一条相同号码的情况
//  Global resource dependence :
//  Author:fengming.huang
//  Note: MMISMS_SetChatLongSmsItemContent
/*****************************************************************************/
LOCAL BOOLEAN ChatWinSetLongSmsItemContent(
                                               MMISMS_CHAT_WINDATA_T *win_data_ptr,
                                               uint32  index,
                                               MMISMS_ORDER_ID_T order_id,
                                               MMI_STRING_T orig_content,
                                               MMI_STRING_T new_insert_content,
                                               MMI_STRING_T *string_ptr //out
                                               )
{
    int32 i = 0;
    MMISMS_CHAT_ITEM_T *chat_item = PNULL;
    uint16 pos = 0;
    uint16 prompt_len = 0;

    MMI_STRING_T sms_lost_promt = {0};
    uint16 max_num = 0;
    uint16 seq_num = 0;

    uint16 split_pos_before_len = 0;
    uint16 split_pos_after_len = 0;
    uint16 split_pos_after_pos = 0;

    if (PNULL == order_id || PNULL == win_data_ptr)
    {
        TRACE_APP_MESSAGES("WatchSMS Chat: LongSmsItemContent param error");
        return FALSE ;
    }

    chat_item = &win_data_ptr->chat_item[index];

    TRACE_APP_MESSAGES("WatchSMS Chat: seq_len is %d,max is %d,seq is %d",chat_item->split_sms_length[order_id->longsms_seq_num],order_id->longsms_max_num,order_id->longsms_seq_num);
    max_num = order_id->longsms_max_num % MMISMS_SPLIT_MAX_NUM;
    max_num = (0 == max_num ) ? MMISMS_SPLIT_MAX_NUM : max_num;
    seq_num = order_id->longsms_seq_num % MMISMS_SPLIT_MAX_NUM;
    seq_num = (0 == seq_num ) ? MMISMS_SPLIT_MAX_NUM : seq_num;

    MMI_GetLabelTextByLang(TXT_SMS_LOST_PROMPT,&sms_lost_promt);
    TRACE_APP_MESSAGES("WatchSMS Chat: orig_content.wstr_len is %d",orig_content.wstr_len);
    TRACE_APP_MESSAGES("WatchSMS Chat: split[0] is %d,split[1] is %d,split[2] is %d,split[3] is %d",chat_item->split_sms_length[0],chat_item->split_sms_length[1],chat_item->split_sms_length[2],chat_item->split_sms_length[3]);
    if(orig_content.wstr_len == 0)  //当前长短信第一个到达的信息
    {
        for(i = 0; i < max_num; i++)
        {
            if(i + 1 == seq_num)
            {
                MMIAPICOM_Wstrncpy(
                &string_ptr->wstr_ptr[pos],
                new_insert_content.wstr_ptr,
                new_insert_content.wstr_len
                );
                pos += new_insert_content.wstr_len;
                chat_item->split_sms_length[i] = new_insert_content.wstr_len;
                chat_item->split_sms_enable[i] = TRUE;
            }
            else
            {
                if (MN_SMS_UCS2_ALPHABET != win_data_ptr->read_result.read_content.alphabet
                && MMIAPICOM_IsASCIIString(sms_lost_promt.wstr_ptr, sms_lost_promt.wstr_len))//ascii
                {
                    uint8 temp_prompt_str[MMISMS_PROMPT_STR_LENGTH + 1] = {0};
                    uint16 default_len = 0;

                    sms_lost_promt.wstr_len = MIN(sms_lost_promt.wstr_len, MMISMS_PROMPT_STR_LENGTH);
                    MMI_WSTRNTOSTR(temp_prompt_str, MMISMS_PROMPT_STR_LENGTH, sms_lost_promt.wstr_ptr, MMISMS_PROMPT_STR_LENGTH, sms_lost_promt.wstr_len);
                    prompt_len = sms_lost_promt.wstr_len;
                    default_len = MMIAPICOM_Default2Wchar(temp_prompt_str, &string_ptr->wstr_ptr[pos], sms_lost_promt.wstr_len);
                }
                else
                {
                    MMI_WSTRNCPY(&string_ptr->wstr_ptr[pos],
                    sms_lost_promt.wstr_len,
                    sms_lost_promt.wstr_ptr,
                    sms_lost_promt.wstr_len,
                    sms_lost_promt.wstr_len);
                }
                pos += sms_lost_promt.wstr_len;
                chat_item->split_sms_length[i] = sms_lost_promt.wstr_len;
                chat_item->split_sms_enable[i] = FALSE;
            }
        } 
    }
    else
    {
        for(i = 0; i < seq_num - 1; i++)
        {
            split_pos_before_len += chat_item->split_sms_length[i];
        }

        split_pos_after_pos = split_pos_before_len + sms_lost_promt.wstr_len;

        for(i = seq_num; i < max_num; i++)
        {
            split_pos_after_len += chat_item->split_sms_length[i];
        }

        if(split_pos_before_len != 0)
        {
            MMI_WSTRNCPY(&string_ptr->wstr_ptr[pos],
            split_pos_before_len,
            orig_content.wstr_ptr,
            split_pos_before_len,
            split_pos_before_len);
            pos += split_pos_before_len;
        }

        MMI_WSTRNCPY(&string_ptr->wstr_ptr[pos],
        new_insert_content.wstr_len,
        new_insert_content.wstr_ptr,
        new_insert_content.wstr_len,
        new_insert_content.wstr_len);
        pos += new_insert_content.wstr_len;
        chat_item->split_sms_length[seq_num - 1] = new_insert_content.wstr_len;

        if(split_pos_after_len != 0)
        {
            MMI_WSTRNCPY(&string_ptr->wstr_ptr[pos],
            orig_content.wstr_len,
            orig_content.wstr_ptr + split_pos_after_pos,
            split_pos_after_len,
            split_pos_after_len);
            pos += split_pos_after_len;
        }
    }

    string_ptr->wstr_len = pos;
    return TRUE;
}

/*****************************************************************************/
//  Description : to set chat long sms content
//  Global resource dependence :
//  Author:fengming.huang
//  Note: MMISMS_CalcChatItemSplitLenth
/*****************************************************************************/
LOCAL void ChatWinCalcChatItemSplitLenth(MMISMS_CHAT_WINDATA_T *win_data_ptr,uint32  index)
{
    MMISMS_CHAT_ITEM_T *chat_item = PNULL;
    MMI_STRING_T sms_lost_promt = {0};
    uint16 max_num = 0;
    uint16 seq_num = 0;
    uint16 i = 0;
    MMISMS_ORDER_ID_T         cur_order_id = PNULL;
    if(PNULL == win_data_ptr)
    {
        TRACE_APP_MESSAGES("WatchSMS Chat: mmisms win_data_ptr error!");
        return ;
    }

    MMI_GetLabelTextByLang(TXT_SMS_LOST_PROMPT,&sms_lost_promt);
    chat_item = &win_data_ptr->chat_item[index];
    cur_order_id = MMISMS_GetOperationOrderId(0);
    if(cur_order_id != PNULL)
    {
        max_num = cur_order_id->longsms_max_num % MMISMS_SPLIT_MAX_NUM;
    }
    max_num = (0 == max_num ) ? MMISMS_SPLIT_MAX_NUM : max_num;

    while( i < MMISMS_SPLIT_MAX_NUM )
    {
        cur_order_id = MMISMS_GetOperationOrderId(i);
        if(cur_order_id != PNULL)
        {
            seq_num = cur_order_id->longsms_seq_num % MMISMS_SPLIT_MAX_NUM;
            seq_num = (0 == seq_num ) ? MMISMS_SPLIT_MAX_NUM : seq_num;
            index = seq_num - 1;
            chat_item->split_sms_length[index] = win_data_ptr->read_result.read_content.split_sms_length[index];
            chat_item->split_sms_enable[i] = TRUE;
        }
        i++;
    }
    for(i = 0; i < max_num; i++)
    {
        if(0 == chat_item->split_sms_length[i])
        {
            chat_item->split_sms_length[i] = sms_lost_promt.wstr_len; 
            chat_item->split_sms_enable[i] = FALSE;
        }
    }
}

/*****************************************************************************/
//  Description : draw one chat bubble
//  Parameter: [In] win Id
//             [In] both_rect
//             [In] order_id
//             [Out] None
//  Author: qi.liu1
//  Note:SetChatItemParam
/*****************************************************************************/
LOCAL void DrawOneChatBubbleItem(
    MMI_WIN_ID_T win_id,
    GUI_BOTH_RECT_T both_rect,
    MMISMS_ORDER_ID_T order_id
)
{
    GUIFORM_DYNA_CHILD_T parent_form_child_ctrl = {0};
    GUIFORM_DYNA_CHILD_T text_form_child_ctrl = {0};
    MMI_STRING_T content_str = {0};
    GUIFORM_INIT_DATA_T form_init = {0};
#ifdef SCREEN_SHAPE_CIRCULAR
#ifdef ADULT_WATCH_SUPPORT
    GUI_FONT_T font_size = MMI_DEFAULT_BIG_FONT;
#else
    GUI_FONT_T font_size = DP_FONT_16;
#endif
#else
    GUI_FONT_T font_size = WATCH_DEFAULT_NORMAL_FONT;
#endif
    GUI_COLOR_T font_color = MMI_WHITE_COLOR;
    uint32 index = 0;
    GUI_BG_T chat_bg = {0};
    GUI_BG_T chat_focus_bg = {0};
    GUILABEL_INIT_DATA_T    label_init = {0};
    GUITEXT_INIT_DATA_T date_text_init = {0};
    GUIFORM_CHILD_WIDTH_T child_width = {0};
    MMI_CTRL_ID_T replace_child_ctrl = 0;
    MMI_STRING_T text_content = {0};
    BOOLEAN is_need_replace = FALSE;
    MMI_STRING_T long_content_str = {0};
    int32 i = 0;
    uint16 long_content_alloc_len = 0;
    GUI_BORDER_T border = {0};
    GUIFORM_CHILD_HEIGHT_T   child_height = {0};
    GUIFORM_DYNA_CHILD_T date_form_child_ctrl = {0};
    uint16 ver_space_ptr = 0;
    MMISMS_CHAT_INDEX_E chat_index_type = MMISMS_CHAT_INDEX_NEW;
    uint16 child_form_width = 0;
    uint16 width = 0;
    MMI_STRING_T sms_time_str = {0};
    uint8 time_len = 0;
    wchar sms_time[20] = {0};
    MMISMS_CHAT_WINDATA_T *win_data_ptr = PNULL;

    win_data_ptr = (MMISMS_CHAT_WINDATA_T *)MMK_GetWinAddDataPtr(win_id);
    if(PNULL == win_data_ptr)
    {
        return;
    }

    chat_index_type = GetChatWinIndexAndType(win_data_ptr,&index, order_id);
    TRACE_APP_MESSAGES("WatchSMS chat: DrawOneChatBubbleItem index=%d momt_type=%d!",index,win_data_ptr->read_result.mo_mt_type);
#if 1
    //index is full
    if (MMISMS_CHAT_INDEX_MAX == chat_index_type)
    {
        //get msg content
        MMISMS_ShowMsgContent(
            win_data_ptr->read_result.read_content.alphabet,
            win_data_ptr->read_result.read_content.length,
            &win_data_ptr->read_result,
            &win_data_ptr->read_result.time_stamp,
            &win_data_ptr->read_result.address,
            win_data_ptr->read_result.language,
            TRUE,
            FALSE,
            &content_str
        );

        TRACE_APP_MESSAGES("WatchSMS Chat: ChatBubbleItem not find index!!");

        replace_child_ctrl = GUIFORM_GetChildCtrlId(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, 0);
        GUIFORM_DestroyDynaChildCtrl(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, replace_child_ctrl);
        if(0 == replace_child_ctrl)//cid606915
        {
            index = 0;
        }
        else
        {
            index = replace_child_ctrl - MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID;;
        }
        TRACE_APP_MESSAGES("WatchSMS Chat: ChatBubbleItem MMISMS_CHAT_INDEX_MAX index=%d!!",index);
    }
    else if (MMISMS_CHAT_INDEX_SAME_LONG_SMS == chat_index_type)
    {
        //get msg content
        MMISMS_ShowMsgContent(
            win_data_ptr->read_result.read_content.alphabet,
            win_data_ptr->read_result.read_content.length,
            &win_data_ptr->read_result,
            &win_data_ptr->read_result.time_stamp,
            &win_data_ptr->read_result.address,
            win_data_ptr->read_result.language,
            TRUE,
            FALSE,
            &content_str
        );

        GUITEXT_GetString(MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID + index, &text_content);

        long_content_alloc_len = (content_str.wstr_len + text_content.wstr_len);
        long_content_alloc_len = MAX(long_content_alloc_len, MMISMS_MAX_MESSAGE_LEN);
        long_content_alloc_len = long_content_alloc_len * sizeof(wchar);
        long_content_str.wstr_ptr = SCI_ALLOCAZ((long_content_alloc_len + 1) * sizeof(wchar));
        if (PNULL == long_content_str.wstr_ptr)
        {
            return;
        }
        SCI_MEMCPY(long_content_str.wstr_ptr, text_content.wstr_ptr, text_content.wstr_len * sizeof(wchar));

        ChatWinSetLongSmsItemContent(win_data_ptr,index, order_id, text_content, content_str, &long_content_str);
        GUIFORM_DestroyDynaChildCtrl(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID + index);
        for (i = 0; i < MMISMS_SPLIT_MAX_NUM; i++)
        {
            if (win_data_ptr->chat_item[index].split_sms_enable[i])
            {
                break;
            }
        }
        if (
            ((order_id->longsms_seq_num % MMISMS_SPLIT_MAX_NUM != 0) && (order_id->longsms_seq_num % MMISMS_SPLIT_MAX_NUM - 1) > i)
            || ((order_id->longsms_seq_num % MMISMS_SPLIT_MAX_NUM == 0) && ((MMISMS_SPLIT_MAX_NUM - 1) > i)))
        {
            order_id = win_data_ptr->chat_item[index].cur_order_id;
        }

        is_need_replace = TRUE;
    }
    else
    {
        //get msg content
        MMISMS_ShowMsgContent(
            win_data_ptr->read_result.read_content.alphabet,
            win_data_ptr->read_result.read_content.length,
            &win_data_ptr->read_result,
            &win_data_ptr->read_result.time_stamp,
            &win_data_ptr->read_result.address,
            win_data_ptr->read_result.language,
            TRUE,
            FALSE,
            &content_str
        );

        //first reached of a long sms
        if (order_id->flag.is_concatenate_sms)
        {
            if ( !order_id->flag.is_receive_all)
            {
                if (!MMK_IsFocusWin(MMISMS_READSMS_WAIT_WIN_ID)) //聊天界面来短信，而非读短信
                {
                    long_content_alloc_len = content_str.wstr_len;
                    long_content_alloc_len = MAX(long_content_alloc_len, MMISMS_MAX_MESSAGE_LEN);
                    long_content_alloc_len = long_content_alloc_len * sizeof(wchar);
                    long_content_str.wstr_ptr = SCI_ALLOCAZ((long_content_alloc_len + 1) * sizeof(wchar));

                    if (PNULL == long_content_str.wstr_ptr)
                    {
                        SCI_FREE( content_str.wstr_ptr );
                        return;
                    }

                    ChatWinSetLongSmsItemContent(win_data_ptr,index, order_id, text_content, content_str, &long_content_str);
                    is_need_replace = TRUE;
                }
                else
                {
                    order_id = MMISMS_GetOperationOrderId(0);
                    ChatWinCalcChatItemSplitLenth(win_data_ptr,index);
                }
            }
            else
            {
                order_id = MMISMS_GetOperationOrderId(0);
                TRACE_APP_MESSAGES("WatchSMS Chat: ChatBubbleItem not receive all!!");
            }
        }
    }
#endif
    SetChatWinIndex(win_data_ptr,index, TRUE, order_id);//save index.  get  from GetChatWinIndexType
    TRACE_APP_MESSAGES("WatchSMS chat: DrawOneChatBubbleItem index=%d ",index);

    //create parent form
    form_init.layout_type = GUIFORM_LAYOUT_ORDER;
    form_init.both_rect = both_rect;
    //set parent ctrl param
    parent_form_child_ctrl.guid = SPRD_GUI_FORM_ID;
    parent_form_child_ctrl.is_bg = FALSE;
    parent_form_child_ctrl.is_get_active = TRUE;
    parent_form_child_ctrl.init_data_ptr = &form_init;
    parent_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID + index;
    GUIFORM_CreatDynaChildCtrl(win_id, MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID,  &parent_form_child_ctrl);

    //set text param
    text_form_child_ctrl.guid = SPRD_GUI_TEXTBOX_ID;
    text_form_child_ctrl.is_bg = FALSE;
    text_form_child_ctrl.is_get_active = TRUE;
    text_form_child_ctrl.init_data_ptr = &date_text_init;
    text_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID + index;

    if (!is_need_replace)
    {
        width =GUI_GetStringWidth(font_size, content_str.wstr_ptr, content_str.wstr_len);
#ifdef ADULT_WATCH_SUPPORT
        width += MMISMS_TEXT_CHAR_SPACE * content_str.wstr_len;
#else
        width += MMITHEME_TEXT_CHAR_SPACE * content_str.wstr_len;
#endif
    }
    else
    {
        width = GUI_GetStringWidth(font_size, long_content_str.wstr_ptr, long_content_str.wstr_len);
#ifdef ADULT_WATCH_SUPPORT
        width += MMISMS_TEXT_CHAR_SPACE * long_content_str.wstr_len;
#else
        width += MMITHEME_TEXT_CHAR_SPACE * long_content_str.wstr_len;
#endif
    }

    {//temp debug	
        SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
        MMI_CheckAllocatedMemInfo();
    }

    //creat form ctrl ,can set distance between bubbles
    //set the distance between bubbles
    child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
    child_height.add_data = MMISMS_CHAT_BUBBLE_GAP;//distance between bubbles

    form_init.layout_type = GUIFORM_LAYOUT_SBS;
    date_form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
    date_form_child_ctrl.is_bg = FALSE;
    date_form_child_ctrl.is_get_active = FALSE;
    date_form_child_ctrl.init_data_ptr = &label_init;
    date_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_DATE_ITEM_FORM_CTRL_ID + index;

    GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &date_form_child_ctrl);
    GUIFORM_SetChildHeight(parent_form_child_ctrl.child_handle, date_form_child_ctrl.child_handle, &child_height);

#ifdef ADULT_WATCH_SUPPORT
    //显示发送失败和发送中的图标
    if (MMISMS_MO_SEND_FAIL == win_data_ptr->read_result.mo_mt_type) //发送失败才显示图标
    {
        uint16 disp_width = 0;
        GUIANIM_DISPLAY_INFO_T display_info = {0};
        BOOLEAN is_send_msg = FALSE;
        GUIANIM_INIT_DATA_T anim_init = {0};
        GUIANIM_CTRL_INFO_T ctrl_info = {0};
        GUIANIM_DATA_INFO_T data_info = {0};
        GUIFORM_DYNA_CHILD_T anim_form_child_ctrl = {0};

        anim_form_child_ctrl.guid = SPRD_GUI_ANIM_ID;
        anim_form_child_ctrl.is_bg = FALSE;
        anim_form_child_ctrl.is_get_active = FALSE;
        anim_form_child_ctrl.init_data_ptr = &anim_init;
        anim_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_LABEL_ITEM_FORM_CTRL_ID + index;
        anim_init.both_rect = both_rect;

        ctrl_info.is_ctrl_id = TRUE;
        ctrl_info.ctrl_id = MMISMS_SHOW_CHAT_SMS_LABEL_ITEM_FORM_CTRL_ID + index;

        if (MMISMS_TYPE_SMS == order_id->flag.msg_type
                && (MMISMS_SENDSTATUS_SENDING == order_id->backsend_status || MMISMS_SENDSTATUS_WAITING == order_id->backsend_status))
        {
            is_send_msg = TRUE;
        }

        if (is_send_msg)
        {
            data_info.img_id = IMAGE_SMS_CHATBOX_MO_SENDING_ICON;
        }
        else
        {
            data_info.img_id = IMAGE_SMS_CHATBOX_MO_FAIL_ICON;
        }

        GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &anim_form_child_ctrl);
        GUIFORM_SetChildAlign(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, parent_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_HMIDDLE);

        //CTRLFORM_SetRect();
        //发送中和发送失败图标居中显示
        display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
        GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
    }
#endif

    //display time of chat (false)
#ifdef SCREEN_SHAPE_CIRCULAR
    time_len = MMISMS_GetDateTimeStringByOrder(order_id,TRUE,TRUE,TRUE,sms_time);
    GUILABEL_SetAlign(date_form_child_ctrl.child_handle, GUILABEL_ALIGN_MIDDLE);
#else
    time_len = MMISMS_GetDateTimeStringByOrder(order_id,FALSE,FALSE,FALSE,sms_time);
#endif
    sms_time_str.wstr_len = (uint16)time_len ;
    sms_time_str.wstr_ptr = sms_time ;
    GUILABEL_SetText(date_form_child_ctrl.child_handle, &sms_time_str, FALSE);
#ifdef ADULT_WATCH_SUPPORT
    GUILABEL_SetFont(date_form_child_ctrl.child_handle, DP_FONT_10, MMI_WHITE_COLOR);
#else
    GUILABEL_SetFont(date_form_child_ctrl.child_handle, DP_FONT_12, MMI_WHITE_COLOR);
#endif
    GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &text_form_child_ctrl);

    //set  chat bubble length
#ifdef ADULT_WATCH_SUPPORT
    child_form_width = width +  MMISMS_SMS_MIN_MARGIN;
    if ((MMISMS_MT_TO_BE_READ == win_data_ptr->read_result.mo_mt_type) ||
            (MMISMS_MT_HAVE_READ == win_data_ptr->read_result.mo_mt_type))
    {
        if (child_form_width > MMISMS_CHAT_RECEIVE_BUBBLE_WIDTH)
        {
            child_form_width =  MMISMS_CHAT_RECEIVE_BUBBLE_WIDTH;
        }
        child_width.add_data = MAX(MMISMS_SMS_MIN_TEXT_BG_WIDTH, child_form_width);
    }
    else
    {
        if (child_form_width > MMISMS_CHAT_SEND_BUBBLE_WIDTH)
        {
            child_form_width =  MMISMS_CHAT_SEND_BUBBLE_WIDTH;
        }
        child_width.add_data = MAX(MMISMS_SMS_MIN_TEXT_BG_WIDTH, child_form_width);
    }
#else
    child_form_width = MMISMS_CHAT_BUBBLE_WIDTH;
    child_width.add_data = child_form_width;
#endif
    child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
    GUIFORM_SetChildWidth(parent_form_child_ctrl.child_handle, text_form_child_ctrl.child_handle, &child_width);

    GUIFORM_PermitChildFont(parent_form_child_ctrl.child_handle, FALSE);

    if ((MMISMS_MO_SEND_SUCC == win_data_ptr->read_result.mo_mt_type)
        ||(MMISMS_MO_SEND_FAIL == win_data_ptr->read_result.mo_mt_type))
    {
        GUIFORM_SetChildAlign(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, parent_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_RIGHT);
    }
#ifdef SCREEN_SHAPE_CIRCULAR
        GUIFORM_SetChildAlign(parent_form_child_ctrl.child_handle, text_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_HMIDDLE);
#else
        GUIFORM_SetChildAlign(parent_form_child_ctrl.child_handle, text_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_RIGHT);
#endif

    {//temp debug	
        SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
        MMI_CheckAllocatedMemInfo();
    }

    //set bg
    chat_bg.bg_type = GUI_BG_IMG;
    chat_focus_bg.bg_type = GUI_BG_IMG;
    if ((MMISMS_MT_TO_BE_READ == win_data_ptr->read_result.mo_mt_type) ||
            (MMISMS_MT_HAVE_READ == win_data_ptr->read_result.mo_mt_type))
    {
#ifdef ADULT_WATCH_SUPPORT
        //成人手表不显示聊天气泡
        chat_bg.img_id = PNULL;
        chat_focus_bg.img_id = PNULL;
        //set margin
        GUITEXT_SetMarginEx(text_form_child_ctrl.child_handle,
                            MMISMS_CHAT_TEXT_MARGIN_LEFT,
                            0,
                            MMISMS_CHAT_TEXT_MARGIN_TOP,
                            MMISMS_CHAT_TEXT_MARGIN_BOTTOM);
#else
        chat_bg.img_id       =IMAGE_SMS_CHAT_RECEIVE;
        chat_focus_bg.img_id =IMAGE_SMS_CHAT_RECEIVE;
        //set margin
        GUITEXT_SetMarginEx(text_form_child_ctrl.child_handle,
                            MMISMS_CHAT_TEXT_MARGIN_LEFT,
                            MMISMS_CHAT_TEXT_MARGIN_RIGHT,
                            MMISMS_CHAT_TEXT_MARGIN_TOP,
                            MMISMS_CHAT_TEXT_MARGIN_BOTTOM);
#endif
    }
    else
    {
#ifdef ADULT_WATCH_SUPPORT
        //成人手表不显示聊天气泡
        chat_bg.img_id = PNULL;
        chat_focus_bg.img_id = PNULL;
        //set margin
        GUITEXT_SetMarginEx(text_form_child_ctrl.child_handle,
                            0,
                            MMISMS_CHAT_TEXT_MARGIN_LEFT,
                            MMISMS_CHAT_TEXT_MARGIN_TOP,
                            MMISMS_CHAT_TEXT_MARGIN_BOTTOM);
#else
        if (MMISMS_MO_SEND_FAIL == win_data_ptr->read_result.mo_mt_type) 
        {
            if (MMISMS_TYPE_SMS == order_id->flag.msg_type
                    && (MMISMS_SENDSTATUS_SENDING == order_id->backsend_status || MMISMS_SENDSTATUS_WAITING == order_id->backsend_status))
            {
                chat_bg.img_id = IMAGE_SMS_CHAT_SENT;//sending
                chat_focus_bg.img_id = IMAGE_SMS_CHAT_SENT;//sending
            }
            else
            {
                chat_bg.img_id = IMAGE_SMS_CHAT_SENTFAIL;//send fail
                chat_focus_bg.img_id = IMAGE_SMS_CHAT_SENTFAIL;//send fail
            }
        }
        else
        {
            chat_bg.img_id = IMAGE_SMS_CHAT_SENT;//send success
            chat_focus_bg.img_id = IMAGE_SMS_CHAT_SENT;//send success
        }
        //set margin
        GUITEXT_SetMarginEx(text_form_child_ctrl.child_handle,
                            MMISMS_CHAT_TEXT_MARGIN_RIGHT,
                            MMISMS_CHAT_TEXT_MARGIN_LEFT,
                            MMISMS_CHAT_TEXT_MARGIN_TOP,
                            MMISMS_CHAT_TEXT_MARGIN_BOTTOM);
#endif
    }

    GUIFORM_SetSpace(parent_form_child_ctrl.child_handle, PNULL, &ver_space_ptr);
    GUIFORM_SetChildTheme(parent_form_child_ctrl.child_handle, &chat_bg, PNULL, PNULL);
    GUIFORM_SetActiveChildTheme(parent_form_child_ctrl.child_handle, &chat_focus_bg, PNULL, PNULL);
    GUIFORM_SetSpace(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, PNULL, &ver_space_ptr);

    //no border
    border.type = GUI_BORDER_NONE;
    GUIFORM_SetChildTheme(parent_form_child_ctrl.child_handle , PNULL, &border, PNULL);
    GUIFORM_SetActiveChildTheme(parent_form_child_ctrl.child_handle , PNULL, &border, PNULL);

    //set font
    GUITEXT_SetFont(text_form_child_ctrl.child_handle, &font_size, &font_color);
    GUITEXT_SetClipboardEnabled (text_form_child_ctrl.child_handle,FALSE);//bugid:1625685

    //set content text align
    if ((MMISMS_MT_TO_BE_READ == win_data_ptr->read_result.mo_mt_type) ||
            (MMISMS_MT_HAVE_READ == win_data_ptr->read_result.mo_mt_type))
    {
        GUITEXT_SetAlign(text_form_child_ctrl.child_handle, ALIGN_LEFT);
    }
    else
    {
        GUITEXT_SetAlign(text_form_child_ctrl.child_handle, ALIGN_LEFT);
#ifdef ADULT_WATCH_SUPPORT
        //set font
        font_color = MMI_SMS_GREEN_COLOR;
        GUITEXT_SetFont(text_form_child_ctrl.child_handle, &font_size, &font_color);
        GUITEXT_SetAlign(text_form_child_ctrl.child_handle, ALIGN_LEFT);
#endif
    }

    //set content text string
    if (!is_need_replace)
    {
        GUITEXT_SetString(
            text_form_child_ctrl.child_handle,
            content_str.wstr_ptr,
            content_str.wstr_len,
            FALSE
        );
    }
    else
    {
        GUITEXT_SetString(
            text_form_child_ctrl.child_handle,
            long_content_str.wstr_ptr,
            long_content_str.wstr_len,
            FALSE
        );
    }

    //set active index
    if (MMK_IsFocusWin(MMISMS_READSMS_WAIT_WIN_ID)
            || MMK_IsFocusWin(win_id)
            || MMK_IsFocusWin(MMISMS_SEND_ANIM_WIN_ID))
    {
        MMK_SetAtvCtrl(win_id, text_form_child_ctrl.child_handle);
    }

    if (PNULL != content_str.wstr_ptr)
    {
        SCI_FREE( content_str.wstr_ptr );
        content_str.wstr_ptr = PNULL;
    }
    if (PNULL != long_content_str.wstr_ptr)
    {
        SCI_FREE( long_content_str.wstr_ptr );
        long_content_str.wstr_ptr = PNULL;
    }

    {//temp debug	
        SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
        MMI_CheckAllocatedMemInfo();
    }
}
