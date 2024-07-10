/*****************************************************************************
 ** File Name:   watch_sms_presetlist.c
 ** Author:      qi.liu1
 ** Date:        20/2/2020
 ** Copyright:   2020 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description: This file is used to describe preset word list win for select
 *****************************************************************************
 **                         Important Edit History                           *
 ** -------------------------------------------------------------------------*
 *  ** DATE           NAME             DESCRIPTION                           *
 *  21/2/2020       qi.liu1                presetlist
 ****************************************************************************/
#include "mmicom_trace.h"
#include "mmipb_datalist.h"
#include "watch_sms_presetlist.h"
#include "watch_common_list.h"
#include "watch_pb_view.h"
#include "mmisms_export.h"
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
#include "mmisms_set.h"
#include "mmismsapp_interface.h"
#ifdef SCREEN_SHAPE_CIRCULAR
#include "watch_commonwin_export.h"
#include "mmidisplay_color.h"
#endif
#include "watch_common.h"
#include "watch_cc_view.h"
/** -------------------------------------------------------------------------*/
//                         Global                                                     //
/** -------------------------------------------------------------------------*/
SmsSrvCallbackFunc preset_callback_func = PNULL;

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//preset list num
#define MMISMS_PRESETLIST_NUM   (10)

/** -------------------------------------------------------------------------*/
//                          LOCAL FUNCTIONS                                 //
/** -------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Draw Sms Preset word List Item
//  Parameter: [In] ctrl Id
//             [Out] None
//             [Return] the number of preset word
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL uint8 DrawSmsPresetListItem(MMI_CTRL_ID_T  ctrl_id ,MMINV_SMS_FREQUENT_WORD_T* freqWord)
{
    uint16 i = 0;
    uint16 itemNum = 0;
    MMI_STRING_T freqStr = {0};

    if(PNULL == freqWord)
    {
       TRACE_APP_MESSAGES("WatchSMS Preset: freqWord = PNULL!");
       return itemNum;
    }

    GUILIST_RemoveAllItems(ctrl_id);
    TRACE_APP_MESSAGES("WatchSMS Preset: DrawSmsPresetListItem enter");

    for(i = 0; i < MMISMS_PRESETLIST_NUM; i++)
    {
        if (0 < freqWord->word[i].wstr_len)
        {
            freqStr.wstr_ptr = freqWord->word[i].wstr_data;
            freqStr.wstr_len = freqWord->word[i].wstr_len;
#ifdef ADULT_WATCH_SUPPORT
            AdultWatchCOM_ListItem_Draw_1Icon_1Str_1line(ctrl_id, IMAGE_SMS_PRESET_SENT,freqStr);
#else
            WatchCOM_ListItem_Draw_1Str_1Icon(ctrl_id, freqStr , IMAGE_SMS_PRESET_SENT);
#endif
            itemNum++;
        }
        else
        {
            break;
        }
    }
    return itemNum;
}

LOCAL void SMS_Preset_APP_WEB( MMI_WIN_ID_T win_id , MMINV_SMS_FREQUENT_WORD_T* freqWord)
{
    uint16 curIndex = 0;
    MMI_CTRL_ID_T ctrlId = MMISMS_PRESETWIN_LIST_CTRL_ID;
    MMISMS_PRESETFROM_E presetFrom = MMISMS_PRESETFROM_NONE;
    MMISMS_DEST_ADDR_INFO_T *contactInfo = PNULL;
    uint16 sim_num = 0;
    uint16 sim_ok = 0;

    if(PNULL == freqWord)
    {
       TRACE_APP_MESSAGES("WatchSMS preset: freqWord = PNULL!");
       return;
    }

    curIndex = GUILIST_GetCurItemIndex(ctrlId);

    sim_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);
    if (0 == sim_num)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
        return;
    }

    //save current select preset word
    MMISMS_SetPresetForSend(freqWord->word[curIndex].wstr_data , freqWord->word[curIndex].wstr_len);

    presetFrom = MMISMS_GetPresetFrom();
    TRACE_APP_MESSAGES("WatchSMS Presetlist: presetFrom = %d",presetFrom);
    switch(presetFrom)
    {
        case MMISMS_PRESETFROM_CHAT:
        {
            //send sms
            contactInfo = MMISMS_GetNumberForSend();
            MMISMSAPP_SendSmsWithData(MN_DUAL_SYS_1,
                    FALSE,
                    contactInfo,//sms->addr
                    freqWord->word[curIndex].wstr_data,//freq->data
                    freqWord->word[curIndex].wstr_len,
                    PNULL
                    );

            MMK_CloseWin(win_id);
            break;
        }

        case MMISMS_PRESETFROM_NORMAL:
        {
            WatchPB_ContactWin_Enter();
            break;
        }

        case MMISMS_PRESETFROM_CALL:
        {
#ifndef ADULT_WATCH_SUPPORT
            WatchCC_RejectMtBySms(); //Bug 1417922
            //send sms from call
            contactInfo = MMISMS_GetNumberForSend();
            MMISMSAPP_SendSmsWithData(MN_DUAL_SYS_1,
                    FALSE,
                    contactInfo,//sms->addr
                    freqWord->word[curIndex].wstr_data,//freq->data
                    freqWord->word[curIndex].wstr_len,
                    preset_callback_func
                    );
            MMK_CloseWin(win_id);
#endif
            break;
        }

        default:
            break;
    }

}

/*****************************************************************************/
//  Description : to handle the preset list window message
//  Parameter: [In] win Id
//             [In] msg id
//             [In] param
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePresetWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM                param
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrlId = MMISMS_PRESETWIN_LIST_CTRL_ID;
    MMINV_SMS_FREQUENT_WORD_T*    userData = (MMINV_SMS_FREQUENT_WORD_T *)MMK_GetWinAddDataPtr(win_id);
#ifdef SCREEN_SHAPE_CIRCULAR
    GUI_RECT_T list_rect = WATCHLIST_RECT;
#endif
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
            TRACE_APP_MESSAGES("WatchSMS Preset: MSG_OPEN_WINDOW");
            GUILIST_SetMaxItem(MMISMS_PRESETWIN_LIST_CTRL_ID,MMISMS_PRESETLIST_NUM, FALSE);
            DrawSmsPresetListItem(ctrlId,userData);
            MMK_SetAtvCtrl( win_id, MMISMS_PRESETWIN_LIST_CTRL_ID );
            break;
        }

        case MSG_FULL_PAINT:
        {
#ifdef SCREEN_SHAPE_CIRCULAR
            WATCHCOM_DisplayBackground(win_id);
#endif

            DrawSmsPresetListItem(ctrlId,userData);
            break;
        }

#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        {
            TRACE_APP_MESSAGES("WatchSMS Preset: MSG_APP_WEB");
            SMS_Preset_APP_WEB(win_id,userData);
            break;
        }

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMK_CloseWin(win_id);
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            TRACE_APP_MESSAGES("WatchSMS Message: MSG_CLOSE_WINDOW");
            if (userData != NULL)
            {
                SCI_FREE(userData);
            }
            break;
        }

        default:
        {
            recode = MMI_RESULT_FALSE;
            break;
        }
    }
    return (recode);
}


/** -------------------------------------------------------------------------*/
//                          WINTAB DEFINES                                  //
/** -------------------------------------------------------------------------*/
// the window of  preset list
/*****************************************************************************/
//  Description : the window of preset list
//  Parameter: NULL
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
WINDOW_TABLE( MMISMS_PRESETLIST_WIN_TAB ) =
{
    WIN_FUNC((uint32) HandlePresetWinMsg),
    WIN_ID(MMISMS_PRESET_LIST_WIN_ID),
    WIN_HIDE_STATUS,
#ifdef ADULT_WATCH_SUPPORT
    WIN_TITLE(TXT_SMS_PRESET),
#endif
#ifndef TOUCH_PANEL_SUPPORT
    WIN_SOFTKEY(TXT_NULL, STXT_OK, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_PRESETWIN_LIST_CTRL_ID ),
    END_WIN
};

/** -------------------------------------------------------------------------*/
//                          Function definition                              //
/** -------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : to open the preset list window message
//  Parameter: Null
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void WatchSMS_PresetListWin_Enter(SmsSrvCallbackFunc send_call_back)
{
    MMI_HANDLE_T winHandle =0;
    MMINV_SMS_FREQUENT_WORD_T *viewData = NULL;

#ifdef SCREEN_SHAPE_CIRCULAR
    BOOLEAN sNotInited = FALSE;
    sNotInited = MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1);
    if(FALSE == sNotInited)
    {
        MMI_STRING_T no_sim_text = {0};
        wchar temp_arr[20 + 1] = {0};
        WATCH_SOFTKEY_TEXT_ID_T softKey = {0};
        no_sim_text.wstr_ptr = temp_arr;
        MMI_GetLabelTextByLang(TXT_SMS_INSERT_SIM, &no_sim_text);
        TRACE_APP_MESSAGES(" Please insert SIM Card");
#ifdef ADULT_WATCH_SUPPORT
        Adult_WatchCOM_NoteWin_1Icon_1Str_Enter(MMISMS_NOTE_WIN_ID,&no_sim_text,IMAGE_SMS_NO_SIM,PNULL);
#else
        WatchCOM_NoteWin_1Line_Enter(MMISMS_NOTE_WIN_ID, &no_sim_text, NULL, softKey, NULL);
#endif
        return;
    }
#endif
    viewData = (MMINV_SMS_FREQUENT_WORD_T *)SCI_ALLOC_APPZ(sizeof(MMINV_SMS_FREQUENT_WORD_T));

    if(PNULL == viewData)
    {
        TRACE_APP_MESSAGES("WatchSMS Preset: viewData alloc fail");
        return;
    }

    //get NV preset word
    MMISMS_GetFrequentWord(viewData);

    if (0 == viewData->word[0].wstr_len) //如果预置信息第一项为0，则list为空，弹note win
    {
        //tip empty note win
        MMI_WIN_ID_T noteId = MMISMS_EMPTY_PRESET_LIST_WIN_ID;
        MMISMS_OpenFullScreenNoteWin( noteId,TXT_SMS_EMPTY_WORD,IMAGE_SMS_MESSAGE_NONE,PNULL );

        if (viewData != PNULL)
        {
            SCI_FREE(viewData);
        }
        TRACE_APP_MESSAGES("WatchSMS Message: MMISMS_PRESETLIST_WIN_TAB list num = 0");
    }
    else
    {
        winHandle = MMK_CreateWin((uint32 *)MMISMS_PRESETLIST_WIN_TAB, viewData);
    }

    preset_callback_func = send_call_back;
    TRACE_APP_MESSAGES("WatchSMS Message: MMISMS_PRESETLIST_WIN_TAB create success");
}

