/*****************************************************************************
 ** File Name:   mmisms_wordlistwin.c
 ** Author:      sam.hua
 ** Date:        21/aug/2012
 ** Copyright:   2012 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description: This file is used to describe word list win for select
 *****************************************************************************
 **                         Important Edit History                           *
 ** -------------------------------------------------------------------------*
 *  ** DATE           NAME             DESCRIPTION                           *
 *  21/aug/2012      sam.hua       written
 ****************************************************************************/

#include "guilistbox.h"
#include "guimenu.h"
#include "mmisms_text.h"
#include "mmisms_image.h"
#include "mmisms_anim.h"
#include "mmisms_id.h"
#include "mmisms_app.h"
#include "mmisms_internal.h"
#include "mmisms_mainmenuwin.h"
#include "mmisms_messagecountwin.h"
#include "mmisms_messageboxwin.h"
#include "mmisms_commonui.h"
#include "mmisms_nv.h"
#include "mmisms_send.h"
//#include "Mmisms_set.h"
#include "mmimms_id.h"
//#include "mmisms_edit.h"
#include "mmisms_set.h"
#include "mmismsapp_interface.h"
#include "mmimms_export.h"

typedef enum _MMISMS_PRESET_SMS_TYPE_E
{
    MMISMS_PRESET_SMS_TEMPLETE,
    MMISMS_PRESET_SMS_FOR_EDIT,
    MMISMS_PRESET_SMS_FOR_CC
} MMISMS_PRESET_SMS_TYPE_E;

/** -------------------------------------------------------------------------*/
//                          LOCAL FUNCTIONS                                 //
/** -------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : to handle the message of window which choice the word
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWordListWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM                param
);

/*****************************************************************************/
//     Description : to handle the word list
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSelectWordWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);
/*****************************************************************************/
//     Description : to handle the message of edit preset word window.
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputWordWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E    msg_id,        //IN:
    DPARAM                param        //IN:
);

#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
/*****************************************************************************/
//     Description : to handle the message of preset SMS for cc
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWordListForCcWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM                param
);
#endif

/*****************************************************************************/
//     Description : load preset word to listbox
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL uint8 LoadPresetWord2List(                                    //RETURN: the number of preset word
    MMI_CTRL_ID_T            ctrl_id,    //IN:
    MMISMS_PRESET_SMS_TYPE_E    type        //IN:
);
/*****************************************************************************/
//     Description : get preset SMS from editbox and set to NV
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void HandleSetPresetSms(
    MMI_CTRL_ID_T                ctrl_id,    //IN:
    MMI_STRING_T    *text_ptr    //IN:
);



/** -------------------------------------------------------------------------*/
//                          WINTAB DEFINES                                  //
/** -------------------------------------------------------------------------*/
// the window of  2.1.1.4  for option window
WINDOW_TABLE( MMISMS_WORDLIST_WIN_TAB ) =
{
    WIN_TITLE( TXT_SMS_FREQWORD ),
    WIN_FUNC((uint32) HandleWordListWinMsg ),
    WIN_ID( MMISMS_WORDLIST_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_WORDWIN_LISTBOX_CTRL_ID ),
    END_WIN
};

WINDOW_TABLE( MMISMS_SELECTWORD_WIN_TAB ) =
{
    WIN_TITLE( TXT_SMS_FREQWORD ),
    WIN_FUNC((uint32) HandleSelectWordWinMsg ),
    WIN_ID( MMISMS_SELECTWORD_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_SELECTWORD_LIST_CTRL_ID ),
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE( MMISMS_INPUTWORD_WIN_TAB ) =
{
    //+CR226157
#ifdef MAINLCD_SIZE_128X64
    WIN_TITLE( TXT_NULL ),
#else
    WIN_TITLE( TXT_EDIT ),
#endif
    //-CR226157
    WIN_FUNC((uint32) HandleInputWordWinMsg ),
    WIN_ID( MMISMS_INPUTWORD_WIN_ID ),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISMS_TEMPLATE_INPUT_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE, 40, MMISMS_INPUTWORDWIN_TEXT_CTRL_ID, MMISMS_TEMPLATE_INPUT_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL(TXT_COMMON_OK, TXT_NULL, STXT_RETURN, MMICOMMON_SOFTKEY_CTRL_ID, MMISMS_TEMPLATE_INPUT_FORM_CTRL_ID),
    END_WIN
};
#else
WINDOW_TABLE( MMISMS_INPUTWORD_WIN_TAB ) =
{
    //+CR226157
#ifdef MAINLCD_SIZE_128X64
    WIN_TITLE( TXT_NULL ),
#else
    WIN_TITLE( TXT_EDIT ),
#endif
    //-CR226157
    WIN_FUNC((uint32) HandleInputWordWinMsg ),
    WIN_ID( MMISMS_INPUTWORD_WIN_ID ),

    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(40, MMISMS_INPUTWORDWIN_TEXT_CTRL_ID),
    END_WIN
};
#endif

#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
// the preset SMS list window is called by cc module
WINDOW_TABLE( MMISMS_WORDLIST_FOR_CC_WIN_TAB ) =
{
    WIN_TITLE( TXT_SMS_FREQWORD ),
    WIN_FUNC((uint32) HandleWordListForCcWinMsg),
    WIN_ID( MMISMS_WORDLIST_FOR_CC_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_WORDWIN_LISTBOX_FOR_CC_CTRL_ID),
    END_WIN
};
#endif

/** -------------------------------------------------------------------------*/
//                          Function definition                              //
/** -------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : to open write SMS word window
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_EnterWordListWin(void)
{
#ifdef MMI_PDA_SUPPORT
    MMK_CreatePubListWin((uint32 *)MMISMS_WORDLIST_WIN_TAB, PNULL);
#else
    MMK_CreateWin((uint32 *)MMISMS_WORDLIST_WIN_TAB, PNULL);
#endif

}

/*****************************************************************************/
//  Description : to handle the create select word win
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMISMS_CreateSelectWordWin(MMI_HANDLE_T ctrl_handle)
{
#ifdef MMI_PDA_SUPPORT
    MMK_CreatePubListWin((uint32 *)MMISMS_SELECTWORD_WIN_TAB, (ADD_DATA)ctrl_handle);
#else
    MMK_CreateWin((uint32 *)MMISMS_SELECTWORD_WIN_TAB, (ADD_DATA)ctrl_handle);
#endif
}



#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : to handle the message of window which choice the word
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWordListWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM                param
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_WORDWIN_LISTBOX_CTRL_ID;
    uint16 cur_index = 0;
    MMI_STRING_T string_info = {0};

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            LoadPresetWord2List(ctrl_id, MMISMS_PRESET_SMS_TEMPLETE);

            MMK_SetAtvCtrl(win_id, ctrl_id);
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
            break;


        case MMISMS_UPDATE_LIST:
            GUILIST_RemoveAllItems(ctrl_id);
            LoadPresetWord2List(ctrl_id, MMISMS_PRESET_SMS_TEMPLETE);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, PNULL);
            break;

        case MSG_SMS_DELSELECTED:
            cur_index = GUILIST_GetCurItemIndex(ctrl_id);

            SCI_MEMSET(&string_info, 0x00, sizeof(string_info));

            HandleSetPresetSms(ctrl_id, &string_info);
            MMK_PostMsg(win_id, MMISMS_UPDATE_LIST, PNULL, PNULL);
            break;

        case MSG_SMS_SETWORD:
            if (PNULL != param)
            {
                HandleSetPresetSms(ctrl_id, (MMI_STRING_T *)param);
            }
            MMK_PostMsg(win_id, MMISMS_UPDATE_LIST, PNULL, PNULL);
            break;


        case MSG_NOTIFY_LIST_CHECK_CONTENT:
        {
            GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = PNULL;
            GUILIST_ITEM_T*  list_item_ptr      = PNULL;

            item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;
            cur_index = item_content_ptr->item_index;
            list_item_ptr = (GUILIST_ITEM_T *)GUILIST_GetItemPtrByIndex(ctrl_id, cur_index);
            if (PNULL == list_item_ptr)
            {
                //SCI_TRACE_LOW:"HandleEntryDetailPdaWinMsg list_item_ptr %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_16276_112_2_18_2_54_54_784, (uint8*)"d", list_item_ptr);
                break;
            }
            if (item_content_ptr->item_content_index == 1)
            {
                if (list_item_ptr->item_data_ptr->item_content[1].item_data.image_id != IMAGE_NULL)
                {
                    MMINV_SMS_FREQUENT_WORD_T freq_word = {0};
                    MMI_STRING_T sms_init_str = {0};

                    cur_index = GUILIST_GetCurItemIndex(ctrl_id);
                    MMISMS_GetFrequentWord(&freq_word);

                    if (0 < freq_word.word[cur_index].wstr_len)
                    {
                        sms_init_str.wstr_len = freq_word.word[cur_index].wstr_len;
                        sms_init_str.wstr_ptr = freq_word.word[cur_index].wstr_data;

                        MMISMS_WriteNewMessage(MN_DUAL_SYS_MAX, &sms_init_str, FALSE, PNULL, 0);
                        MMK_CloseWin(win_id);
                    }
                    else
                    {
                        MMK_CreateWin((uint32 *)MMISMS_INPUTWORD_WIN_TAB, PNULL);
                    }
                }
            }
        }

        break;

        case MSG_CTL_PENOK:
        {
            MMINV_SMS_FREQUENT_WORD_T freq_word = {0};
            cur_index = GUILIST_GetCurItemIndex(ctrl_id);
            MMISMS_GetFrequentWord(&freq_word);

            if (0 < freq_word.word[cur_index].wstr_len)
            {
                MMK_CreateWin((uint32 *)MMISMS_INPUTWORD_WIN_TAB, PNULL);

                GUIEDIT_SetString(
                    MMISMS_INPUTWORDWIN_TEXT_CTRL_ID,
                    freq_word.word[cur_index].wstr_data,
                    freq_word.word[cur_index].wstr_len
                );
            }
            else
            {
                //打开编辑窗口
                MMK_CreateWin((uint32 *)MMISMS_INPUTWORD_WIN_TAB, PNULL);
            }
        }
        break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return (recode);
}

#else
/*****************************************************************************/
//     Description : to handle the message of window which choice the word
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWordListWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM                param
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_WORDWIN_LISTBOX_CTRL_ID;
    uint16 cur_index = 0;
    MMI_STRING_T string_info = {0};

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            LoadPresetWord2List(ctrl_id, MMISMS_PRESET_SMS_TEMPLETE);

            MMK_SetAtvCtrl(win_id, ctrl_id);
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_EDIT, TXT_SEND, STXT_RETURN, FALSE);
            break;

        case MSG_SMS_DELSELECTED:
            cur_index = GUILIST_GetCurItemIndex(ctrl_id);

            SCI_MEMSET(&string_info, 0x00, sizeof(string_info));

            HandleSetPresetSms(ctrl_id, &string_info);
            break;

        case MSG_SMS_SETWORD:
            if (PNULL != param)
            {
                HandleSetPresetSms(ctrl_id, (MMI_STRING_T *)param);
            }
            break;

        case MSG_CTL_OK:
        case MSG_APP_OK:
        {
            MMINV_SMS_FREQUENT_WORD_T freq_word = {0};
            cur_index = GUILIST_GetCurItemIndex(ctrl_id);
            MMISMS_GetFrequentWord(&freq_word);

            if (0 < freq_word.word[cur_index].wstr_len)
            {
                MMK_CreateWin((uint32 *)MMISMS_INPUTWORD_WIN_TAB, PNULL);

                GUIEDIT_SetString(
                    MMISMS_INPUTWORDWIN_TEXT_CTRL_ID,
                    freq_word.word[cur_index].wstr_data,
                    freq_word.word[cur_index].wstr_len
                );
            }
            else
            {
                //打开编辑窗口
                MMK_CreateWin((uint32 *)MMISMS_INPUTWORD_WIN_TAB, PNULL);
            }
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        {
            MMINV_SMS_FREQUENT_WORD_T freq_word = {0};
            MMI_STRING_T sms_init_str = {0};

            cur_index = GUILIST_GetCurItemIndex(ctrl_id);
            MMISMS_GetFrequentWord(&freq_word);

            if (0 < freq_word.word[cur_index].wstr_len)
            {
                sms_init_str.wstr_len = freq_word.word[cur_index].wstr_len;
                sms_init_str.wstr_ptr = freq_word.word[cur_index].wstr_data;

                MMISMS_WriteNewMessage(MN_DUAL_SYS_MAX, &sms_init_str, FALSE, PNULL, 0);
                MMK_CloseWin(win_id);
            }
            else
            {
                MMK_CreateWin((uint32 *)MMISMS_INPUTWORD_WIN_TAB, PNULL);
            }
        }
        break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return (recode);
}

#endif

/*****************************************************************************/
//     Description : to handle the word list
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectWordWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E    msg_id,        //IN:
    DPARAM            param        //IN:
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_SELECTWORD_LIST_CTRL_ID;

    uint8 selected_index = 0;
    BOOLEAN is_right = 0;
    const GUILIST_ITEM_T *cur_list_ptr = {0};
    MMI_HANDLE_T ctrl_handle = (MMI_HANDLE_T)MMK_GetWinAddDataPtr(win_id) ;
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            if (0 < LoadPresetWord2List(ctrl_id, MMISMS_PRESET_SMS_FOR_EDIT))
            {
                MMK_SetAtvCtrl(win_id, ctrl_id);
                GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_COMMON_EMPTY_LIST);
                MMK_CloseWin(win_id);
            }
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifndef MMI_PDA_SUPPORT
        case MSG_CTL_OK:
        case MSG_APP_OK:
#endif
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            selected_index = GUILIST_GetCurItemIndex(ctrl_id);

            cur_list_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, selected_index);
            if (PNULL == cur_list_ptr)
            {
                //SCI_TRACE_LOW:"MMISMS:HandleSelectWordWinMsg cur_list_ptr = PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_18062_112_2_18_2_54_58_796, (uint8*)"");
                break;
            }
            // set the selected frequent word to edit box

#if !defined(MMI_PDA_SUPPORT) && !defined(MMIMMS_SMS_IN_1_SUPPORT) && defined(MMS_SUPPORT)
            if(MMIMMS_EDIT_MENU_CTRL_ID == ctrl_handle)
            {
                is_right = MMIAPIMMS_SetEditTextContent(cur_list_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_ptr,
                    cur_list_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_len);
            }
            else
#endif
            is_right = GUIEDIT_InsertString(
                           ctrl_handle,
                           cur_list_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_ptr,
                           cur_list_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_len);

            if (!is_right)
            {
                // can not insert to edit, because the wstr_len is out of range.
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_BEYOND_LEN);
            }
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

    return (recode);
}

/*****************************************************************************/
//     Description : to handle the message of edit preset word window.
//    Global resource dependence :
//  Author:louis.wei
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputWordWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E    msg_id,        //IN:
    DPARAM                param        //IN:
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_INPUTWORDWIN_TEXT_CTRL_ID;
    MMI_STRING_T edit_string = {0};

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            //+CR226157
#ifdef MAINLCD_SIZE_128X64
            {
                GUI_RECT_T edit_rect = {0};
                GUI_RECT_T title_rect = {0};

                edit_rect = MMITHEME_GetClientRect();
                edit_rect.top += MMI_STATUSBAR_HEIGHT  ;
                GUIEDIT_SetRect(ctrl_id, &edit_rect);

                //Adjust title rect and refresh title back ground..
                title_rect.right = title_rect.left +  MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
                title_rect.bottom = EDIT_IM_ICON_HEIGHT;
                GUIWIN_SetTitleRect(win_id, title_rect);
                GUIWIN_SetTitleBackground(win_id, 0,  MMI_BACKGROUND_COLOR);
            }
#endif
            //-CR226157

            MMK_SetAtvCtrl(win_id, ctrl_id);
            //GUIEDIT_SetTextMaxLen(ctrl_id,40/sizeof(wchar), 40);
            break;

            //+CR226157
#ifdef MAINLCD_SIZE_128X64
        case MSG_FULL_PAINT:
        {
            GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
            GUI_RECT_T rect = {0};

            rect.left = 0;
            rect.right = rect.left + MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
            rect.top = EDIT_IM_ICON_HEIGHT;
            rect.bottom = MMI_MAINSCREEN_BOTTOM_MAX_PIXEL - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;

            GUI_FillRect(&lcd_dev_info, rect, MMI_BACKGROUND_COLOR);
        }
        break;
#endif
        //-CR226157
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
            if (PNULL != param)
            {
                uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
                switch (src_id)
                {
                    case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                        // to get the content which the user input
                        GUIEDIT_GetString(ctrl_id, &edit_string);
                        if (0 < edit_string.wstr_len)
                        {
                            MMK_SendMsg(MMISMS_WORDLIST_WIN_ID, MSG_SMS_SETWORD, &edit_string);
                            MMK_CloseWin(win_id);
                        }
                        else
                        {
                            MMK_PostMsg(MMISMS_WORDLIST_WIN_ID, MSG_SMS_DELSELECTED, PNULL, 0);
                            MMK_CloseWin(win_id);
                        }
                        break;
                    case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:

                        break;
                    case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                        // close the edit window
                        MMK_CloseWin(win_id);
                        break;
                    default:
                        break;
                }
            }
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            // to get the content which the user input
            GUIEDIT_GetString(ctrl_id, &edit_string);
            if (0 < edit_string.wstr_len)
            {
                MMK_SendMsg(MMISMS_WORDLIST_WIN_ID, MSG_SMS_SETWORD, &edit_string);
                MMK_CloseWin(win_id);
            }
            else
            {
                MMK_PostMsg(MMISMS_WORDLIST_WIN_ID, MSG_SMS_DELSELECTED, PNULL, 0);
                MMK_CloseWin(win_id);
            }
            break;
#endif
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            // close the edit window
            MMK_CloseWin(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return (recode);
}


#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence :
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void AppendPdaItem1Line_1Icon(
    MMI_CTRL_ID_T  ctrl_id,    //控件ID
    MMI_STRING_T  text_str,     //副文本
    MMI_IMAGE_ID_T imgae_id,   //图标ID
    uint32         user_data   //用户数据
)
{

    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    if (imgae_id != IMAGE_NULL)
    {
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
        item_t.item_state |= GUIITEM_STATE_CONTENT_CHECK;
    }
    else
    {
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    }
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;

    //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = text_str.wstr_ptr;
    item_data.item_content[0].item_data.text_buffer.wstr_len = text_str.wstr_len;

    if (imgae_id != IMAGE_NULL)
    {
        item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.image_id = imgae_id;
    }

    GUILIST_AppendItem(ctrl_id, &item_t);
}
/*****************************************************************************/
//     Description : load preset word to listbox
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/

LOCAL uint8 LoadPresetWord2List(                                    //RETURN: the number of preset word
    MMI_CTRL_ID_T            ctrl_id,    //IN:
    MMISMS_PRESET_SMS_TYPE_E    type        //IN:
)
{
    uint16 i = 0;
    uint16 item_num = 0;
    MMI_TEXT_ID_T left_softkey = TXT_NULL;
    MMI_TEXT_ID_T mid_softkey = TXT_NULL;
    MMI_STRING_T empty_str = {0};
    MMINV_SMS_FREQUENT_WORD_T freq_word = {0};
    MMI_STRING_T  text_str = {0};
    // get saved frequent word
    MMISMS_GetFrequentWord(&freq_word);

    // set max item number to listbox
    GUILIST_SetMaxItem(ctrl_id, MMISMS_FREQUENT_WORD_NUM, FALSE );//max item 10

    // get empty string
    MMI_GetLabelTextByLang(TXT_SMS_EMPTY_WORD, &empty_str);

    if (MMISMS_PRESET_SMS_TEMPLETE == type)
    {
        for (i = 0; i < MMISMS_FREQUENT_WORD_NUM; i++)
        {


            if (0 < freq_word.word[i].wstr_len)
            {
                text_str.wstr_ptr = freq_word.word[i].wstr_data;
                text_str.wstr_len = freq_word.word[i].wstr_len;
                AppendPdaItem1Line_1Icon(ctrl_id, text_str,  IMAGE_COMMON_TXT_ICON,  i);
            }
            else
            {
                text_str.wstr_ptr = empty_str.wstr_ptr;
                text_str.wstr_len = empty_str.wstr_len;
                AppendPdaItem1Line_1Icon(ctrl_id, text_str,  IMAGE_NULL,  i);
            }
            item_num++;
        }
    }
    else
    {
        for (i = 0; i < MMISMS_FREQUENT_WORD_NUM; i++)
        {
            if (0 < freq_word.word[i].wstr_len)
            {
                MMISMS_AppendListItem(
                    freq_word.word[i].wstr_data,
                    freq_word.word[i].wstr_len,
                    ctrl_id,
                    left_softkey,
                    mid_softkey,
                    i,
                    MMISMS_APPEND_LIST_ITEM_TYPE
                );
                item_num++;
            }
        }
    }


    return item_num;
}
#else
LOCAL uint8 LoadPresetWord2List(                                    //RETURN: the number of preset word
    MMI_CTRL_ID_T            ctrl_id,    //IN:
    MMISMS_PRESET_SMS_TYPE_E    type        //IN:
)
{
    uint16 i = 0;
    uint16 item_num = 0;
    MMI_TEXT_ID_T left_softkey = TXT_NULL;
    MMI_TEXT_ID_T mid_softkey = TXT_NULL;
    MMI_STRING_T empty_str = {0};
    MMINV_SMS_FREQUENT_WORD_T freq_word = {0};

    // get saved frequent word
    MMISMS_GetFrequentWord(&freq_word);

    if (MMISMS_PRESET_SMS_TEMPLETE == type)
    {
        left_softkey = TXT_EDIT;
        mid_softkey = TXT_SEND;
    }
    else
    {
        if (MMISMS_PRESET_SMS_FOR_EDIT == type)
        {
            left_softkey = TXT_COMMON_OK;
        }
        else
        {
            left_softkey = TXT_EDIT;
            mid_softkey = TXT_SEND;
        }
    }

    // set max item number to listbox
    GUILIST_SetMaxItem(ctrl_id, MMISMS_FREQUENT_WORD_NUM, FALSE );//max item 10

    // get empty string
    MMI_GetLabelTextByLang(TXT_SMS_EMPTY_WORD, &empty_str);

    for (i = 0; i < MMISMS_FREQUENT_WORD_NUM; i++)
    {
        if (0 < freq_word.word[i].wstr_len)
        {
            MMISMS_AppendListItem(
                freq_word.word[i].wstr_data,
                freq_word.word[i].wstr_len,
                ctrl_id,
                left_softkey,
                mid_softkey,
                i,
                MMISMS_APPEND_LIST_ITEM_TYPE
            );
            item_num++;
        }
        else if (MMISMS_PRESET_SMS_TEMPLETE == type)
        {
            MMISMS_AppendListItem(
                empty_str.wstr_ptr,
                (uint8)empty_str.wstr_len,
                ctrl_id,
                left_softkey,
                TXT_NULL,
                i,
                MMISMS_APPEND_LIST_ITEM_TYPE
            );
            item_num++;
        }
    }

    return item_num;
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : get preset SMS from edit box and set to NV
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSetPresetSms(
    MMI_CTRL_ID_T                ctrl_id,    //IN:
    MMI_STRING_T    *text_ptr    //IN:
)
{
    MMI_NV_SMS_PHRASE_T phase = {0};
    uint16 cur_pos = 0;

    if (PNULL == text_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:HandleSetPresetSms text_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_16706_112_2_18_2_54_55_787, (uint8*)"");
        return;
    }
    cur_pos = GUILIST_GetCurItemIndex(ctrl_id);

    SCI_MEMSET(&phase, 0x00, sizeof(phase));

    phase.wstr_len = MIN(text_ptr->wstr_len, MMISMS_FREQUENT_WORD_LEN);

    MMI_WSTRNCPY(phase.wstr_data,
                 MMISMS_FREQUENT_WORD_LEN,
                 text_ptr->wstr_ptr,
                 text_ptr->wstr_len,
                 phase.wstr_len);
    if (0 < text_ptr->wstr_len)//modify
    {
        phase.wstr_len = MIN(text_ptr->wstr_len, MMISMS_FREQUENT_WORD_LEN);

        MMISMS_ModifyPointedWord(cur_pos, &phase);

    }
    else//delete
    {
        // delete the selected preset word
        MMISMS_DeletePointedWord((uint8)cur_pos);
    }
}
#else
/*****************************************************************************/
//     Description : get preset SMS from edit box and set to NV
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void HandleSetPresetSms(
    MMI_CTRL_ID_T                ctrl_id,    //IN:
    MMI_STRING_T    *text_ptr    //IN:
)
{
    MMI_NV_SMS_PHRASE_T phase = {0};
    uint16 cur_pos = 0;
    MMI_STRING_T empty_str = {0};

    if (PNULL == text_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:HandleSetPresetSms text_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_16749_112_2_18_2_54_55_788, (uint8*)"");
        return;
    }
    cur_pos = GUILIST_GetCurItemIndex(ctrl_id);

    SCI_MEMSET(&phase, 0x00, sizeof(phase));

    phase.wstr_len = MIN(text_ptr->wstr_len, MMISMS_FREQUENT_WORD_LEN);

    MMI_WSTRNCPY(phase.wstr_data,
                 MMISMS_FREQUENT_WORD_LEN,
                 text_ptr->wstr_ptr,
                 text_ptr->wstr_len,
                 phase.wstr_len);
    if (0 < text_ptr->wstr_len)//modify
    {
        phase.wstr_len = MIN(text_ptr->wstr_len, MMISMS_FREQUENT_WORD_LEN);

        MMISMS_ModifyPointedWord(cur_pos, &phase);

        // modify the list item
        MMISMS_AppendListItem(
            text_ptr->wstr_ptr,
            (uint8)text_ptr->wstr_len,
            ctrl_id,
            TXT_EDIT,
            TXT_SEND,
            (uint16)cur_pos,
            MMISMS_REPLACE_LIST_ITEM_TYPE
        );
    }
    else//delete
    {
        // delete the selected preset word
        MMISMS_DeletePointedWord((uint8)cur_pos);

        // get empty string
        MMI_GetLabelTextByLang(TXT_SMS_EMPTY_WORD, &empty_str);

        MMISMS_AppendListItem(
            empty_str.wstr_ptr,
            (uint8)empty_str.wstr_len,
            ctrl_id,
            TXT_EDIT,
            TXT_NULL,
            (uint16)cur_pos,
            MMISMS_REPLACE_LIST_ITEM_TYPE
        );
    }
}
#endif

#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
typedef struct
{
    //input
    MN_DUAL_SYS_E dual_sys;
    uint8   number[MMISMS_NUMBER_MAX_LEN + 2];
    uint8   num_len;
    //Controls
    uint16 cur_index;
    MMI_STRING_T list_string;
} MMI_SMS_WORDLISTWIN_DATA_T;



/*****************************************************************************/
//  Description : SEND SMS. should be replaced by app control api later
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL void SMS_SEND(MN_DUAL_SYS_E dual_sys, MMI_HANDLE_T  win_handle)
{
    MMISMS_DEST_ADDR_INFO_T  sms_addr = {0};
    MMI_SMS_WORDLISTWIN_DATA_T *windata_p = (MMI_SMS_WORDLISTWIN_DATA_T*)MMK_GetWinUserData(win_handle);

    if (windata_p == PNULL)
    {
        return;
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPICC_IsSimServiceAvail(dual_sys))
    {
        MMIAPICC_OpenSIMCallingPrompt();
        return;
    }
#endif

    if (0 < windata_p->num_len)
    {
        MMISMS_ClearDestNum();
        MMISMS_SetAddressToMessage(windata_p->number, windata_p->num_len, &sms_addr.dest_list);
    }

    MMISMSAPP_SendSmsWithData(
        windata_p->dual_sys,
        FALSE,
        &sms_addr,
        windata_p->list_string.wstr_ptr,
        windata_p->list_string.wstr_len,
        PNULL);
}


/*****************************************************************************/
//  Description : SEND SMS. should be replaced by app control api later
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/\
LOCAL void SMS_WRITE(MN_DUAL_SYS_E dual_sys, MMI_HANDLE_T  win_handle)
{
    MMI_SMS_WORDLISTWIN_DATA_T *windata_p = (MMI_SMS_WORDLISTWIN_DATA_T*)MMK_GetWinUserData(win_handle);

    if (windata_p == PNULL)
    {
        return;
    }
    MMISMS_WriteNewMessage(windata_p->dual_sys,
                           &windata_p->list_string, FALSE, windata_p->number, windata_p->num_len);

    return;
}


LOCAL BOOLEAN SetCurrentWinData(MMI_SMS_WORDLISTWIN_DATA_T *windata_p, MMI_HANDLE_T ctrl_handle)
{
    const GUILIST_ITEM_T *cur_list_ptr = {0};
    uint16 cur_index = 0;

    cur_index = GUILIST_GetCurItemIndex(ctrl_handle);
    cur_list_ptr = GUILIST_GetItemPtrByIndex(ctrl_handle, cur_index);

    if (PNULL == cur_list_ptr || windata_p == PNULL )
    {
        //SCI_TRACE_LOW:"MMISMS:HandleWordListForCcWinMsg cur_list_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_16552_112_2_18_2_54_55_785, (uint8*)"");
        return FALSE;
    }

    windata_p->list_string.wstr_len = cur_list_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_len;
    windata_p->list_string.wstr_ptr = cur_list_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_ptr;

    return TRUE;
}



#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//  Description : select sim card callback
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  SendSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_HANDLE_T win_handle = (MMI_HANDLE_T)MMK_GetWinAddDataPtr(MMK_GetFocusWinId());
    if (isReturnOK)
    {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        if (!MMIAPICC_IsSimServiceAvail(dual_sys))
        {
            MMIAPICC_OpenSIMCallingPrompt();
            return recode;
        }
#endif

        SMS_SEND(dual_sys, win_handle);
        //MMK_CloseWin(win_handle);    //msg is sent, close edit win
    }
    return recode;
}

#endif

/*****************************************************************************/
//     Description : Send msg
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void HandleWordlistSendMsg(MN_DUAL_SYS_E dual_sys, MMI_HANDLE_T win_handle )
{
    MN_DUAL_SYS_E my_dual_sys  = MN_DUAL_SYS_1;

    if (MMIAPISET_GetAnswerWithOriginalSimType(MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_SMS)
            && dual_sys < MMI_DUAL_SYS_MAX
            && MMIAPIPHONE_IsSimAvailable(dual_sys))
    {
        my_dual_sys = dual_sys;
    }
    else
    {
        my_dual_sys = MN_DUAL_SYS_MAX;
    }


#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (my_dual_sys == MN_DUAL_SYS_MAX)
    {
        MMISET_MAJOR_SIM_SEL_TYPE_E major_select_sim = MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_SMS);
        if (major_select_sim < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
        {
            my_dual_sys = (MN_DUAL_SYS_E)((uint32)major_select_sim - (uint32)MMISET_MAJOR_SIM_SEL_SIM1);
        }
        else
        {
#ifdef MMI_SMS_FIX_SEND_SIM
            if (MMISMS_GetFixSIMSendingOnOFF())
            {
                my_dual_sys = (MN_DUAL_SYS_E)MMISMS_GetFixSIMSendingSIMSYS();
            }
            else
#endif
            {
                MMISMS_SelectSIMFunc(SendSelectSimCallback, (ADD_DATA)win_handle, TRUE);
                return;
            }
        }
        SMS_SEND(my_dual_sys, win_handle);

    }
    else
    {
        SMS_SEND(my_dual_sys, win_handle);
    }
#else
    my_dual_sys = MN_DUAL_SYS_1;
    SMS_SEND(my_dual_sys, win_handle);
#endif
    //MMK_CloseWin(win_handle);

    return;

}



/*****************************************************************************/
//     Description : to handle the message of preset SMS for cc
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWordListForCcWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM                param
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_WORDWIN_LISTBOX_FOR_CC_CTRL_ID;
    MMI_SMS_WORDLISTWIN_DATA_T *windata_p = (MMI_SMS_WORDLISTWIN_DATA_T*)MMK_GetWinUserData(win_id);

    if (windata_p == PNULL)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            if (0 < LoadPresetWord2List(ctrl_id, MMISMS_PRESET_SMS_FOR_CC))
            {
                GUILIST_SetNeedPrgbar(ctrl_id, FALSE);
                MMK_SetAtvCtrl(win_id, ctrl_id);
            }
            else
            {
                MMISMS_WriteNewMessage(windata_p->dual_sys, PNULL, FALSE, PNULL, 0);/*lint !e64*/
                MMK_CloseWin(win_id);
            }
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_CTL_OK:
        case MSG_APP_OK:

            if (SetCurrentWinData(windata_p, ctrl_id))
            {
                SMS_WRITE(windata_p->dual_sys, win_id);
                MMK_CloseWin(win_id);
            }
            break;

        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_APP_WEB:
            if (SetCurrentWinData(windata_p, ctrl_id))
            {
                HandleWordlistSendMsg(windata_p->dual_sys, win_id);
            }
            break;
#if 0
        case MSG_PROMPTWIN_OK:
            MMISMS_SetSaveFlag(FALSE);
            send_err_code = MMISMS_WintabSendSMS(FALSE);
            MMISMS_DisplaySendErr( MMISMS_GetCurSendDualSys(), send_err_code );
            MMIPUB_CloseQuerytWin(&query_win_id);
            MMK_CloseWin(win_id);
            break;

        case MSG_PROMPTWIN_CANCEL:
            MMIPUB_CloseQuerytWin(&query_win_id);
            MMK_CloseWin(win_id);
            break;
#endif
        case MSG_CLOSE_WINDOW:
            SCI_FREE(windata_p);

            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return (recode);
}

/*****************************************************************************/
//     Description : open frequent word list for cc module
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_OpenFreqWordListForCc(
    MN_DUAL_SYS_E dual_sys,
    uint8    number_len,    //IN:
    uint8    *number_str    //IN:
)
{

    MMI_SMS_WORDLISTWIN_DATA_T *windata_p = PNULL;
    MMI_HANDLE_T    win_handle = PNULL;

    windata_p = (MMI_SMS_WORDLISTWIN_DATA_T*)SCI_ALLOCAZ(sizeof(MMI_SMS_WORDLISTWIN_DATA_T));

    if (windata_p == PNULL)
    {
        return;
    }

    win_handle = MMK_CreateWin((uint32 *)MMISMS_WORDLIST_FOR_CC_WIN_TAB, PNULL);

    if (win_handle == PNULL)
    {
        SCI_FREE(windata_p);
        return;
    }

    //copy input parameters
    if ((0 < number_len) && (PNULL != number_str))
    {
        SCI_MEMCPY(windata_p->number, number_str, number_len);
    }
    windata_p->dual_sys = dual_sys;
    windata_p->num_len = number_len;

    MMK_SetWinUserData(win_handle, windata_p);
}
#endif

