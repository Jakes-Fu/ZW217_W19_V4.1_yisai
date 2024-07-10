/*****************************************************************************
 ** File Name:   mmisms_phizlistwin.c
 ** Author:      sam.hua
 ** Date:        23/aug/2012
 ** Copyright:   2012 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description: This file is used to describe phizlist Win
 *****************************************************************************
 **                         Important Edit History                           *
 ** -------------------------------------------------------------------------*
 *  ** DATE           NAME             DESCRIPTION                           *
 *  23/aug/2012      sam.hua       written
 ****************************************************************************/

#include "guilistbox.h"
#include "guimenu.h"
#include "mmisms_text.h"
#include "mmisms_image.h"
#include "mmisms_anim.h"
#include "mmisms_id.h"
#include "mmisms_app.h"
#include "mmisms_mainmenuwin.h"
#include "mmisms_messagecountwin.h"
#include "mmisms_messageboxwin.h"
#include "mmisms_commonui.h"


/*---------------------------------------------------------------------------*/
/*                            DEFINES                                        */
/*---------------------------------------------------------------------------*/
#define MMISMS_PHIZ_NUM    21    //
#define PHIZ_MAX_LEN 7

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
const char * const s_sms_phiz[MMISMS_PHIZ_NUM] =
{
    ":-)",
    "^O^",
    ":-]",
    "*^_^*",
    ";-)",
    ":~)",
    "i-)",
    "<@_@>",
    ":-(",
    ":-O",
    ":-D",
    "?_?",
    "<:I",
    "(:-...",
    ":-(*)",
    ":-')",
    "X-<",
    ":-\\",
    ":-e",
    "#-)",
    ":-7"
};//²åÈë±íÇé·û


/** -------------------------------------------------------------------------*/
//                          LOCAL FUNCTIONS                                 //
/** -------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : to handle the message of window which choice the phiz
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePhizListWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E    msg_id,        //IN:
    DPARAM                param        //IN:
);

/*****************************************************************************/
//     Description : load total phiz to list
//    Global resource dependence :
//  Author: Tracy zhang
//    Note:
/*****************************************************************************/
LOCAL void LoadTotalPhiz2List(
    MMI_CTRL_ID_T        ctrl_id        //IN:
);


/** -------------------------------------------------------------------------*/
//                          WINTAB DEFINES                                  //
/** -------------------------------------------------------------------------*/

// the phiz list window
WINDOW_TABLE( MMISMS_PHIZLIST_WIN_TAB ) =
{
    WIN_TITLE( TXT_SMS_PHIZ ),
    WIN_FUNC((uint32) HandlePhizListWinMsg),
    WIN_ID( MMISMS_PHIZLIST_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISMS_PHIZ_LISTBOX_CTRL_ID ),

    END_WIN
};


/** -------------------------------------------------------------------------*/
//                          Function definition                              //
/** -------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : enter phiz win
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_OpenPhizWin(MMI_HANDLE_T ctrl_handle)
{
#ifdef MMI_PDA_SUPPORT
    MMK_CreatePubListWin((uint32 *)MMISMS_PHIZLIST_WIN_TAB, (ADD_DATA)ctrl_handle);
#else
    MMK_CreateWin((uint32 *)MMISMS_PHIZLIST_WIN_TAB, (ADD_DATA)ctrl_handle);
#endif
}

/*****************************************************************************/
//     Description : to handle the message of window which choice the phiz
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePhizListWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E    msg_id,        //IN:
    DPARAM                param        //IN:
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISMS_PHIZ_LISTBOX_CTRL_ID;
    uint16 cur_index = 0;
    BOOLEAN is_right = TRUE;
    wchar sms_phiz[20] = {0};
    MMI_HANDLE_T    ctrl_handle= (MMI_HANDLE_T)MMK_GetWinAddDataPtr(win_id);
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            LoadTotalPhiz2List(ctrl_id);
            GUILIST_SetNeedPrgbar(ctrl_id, TRUE);
            MMK_SetAtvCtrl(win_id, ctrl_id);
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
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
            cur_index = GUILIST_GetCurItemIndex(ctrl_id);

            MMI_STRNTOWSTR( sms_phiz,
                            20,
                            (const uint8 *)s_sms_phiz[cur_index],
                            strlen((char *)s_sms_phiz[cur_index]),
                            strlen((char *)s_sms_phiz[cur_index]));

            // insert the selected phiz to editbox
            is_right = GUIEDIT_InsertString(
                            ctrl_handle,
                           sms_phiz,
                           (uint16)MMIAPICOM_Wstrlen(sms_phiz));
            if (!is_right)
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_BEYOND_LEN);
            }
            MMK_CloseWin(MMISMS_EDITOPTION_WIN_ID);
            MMK_CloseWin(win_id);
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin( win_id );
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

/*****************************************************************************/
//     Description : load total phiz to list
//    Global resource dependence :
//  Author: Tracy zhang
//    Note:
/*****************************************************************************/
LOCAL void LoadTotalPhiz2List(
    MMI_CTRL_ID_T        ctrl_id        //IN:
)
{
    uint16 i = 0;
    uint8 j = 0;
    uint8 phiz_len = 0;
    MMI_STRING_T item_str = {0};
    MMI_STRING_T prompt_str = {0};

    item_str.wstr_ptr = SCI_ALLOC_APP((GUILIST_STRING_MAX_NUM + 1) * sizeof(wchar));

    GUILIST_SetMaxItem(ctrl_id, MMISMS_PHIZ_NUM, FALSE );//max item 10

    for (i = 0; i < MMISMS_PHIZ_NUM; i++)
    {
        SCI_MEMSET(item_str.wstr_ptr, 0, ((GUILIST_STRING_MAX_NUM + 1)*sizeof(wchar)));

        phiz_len = strlen((char *)s_sms_phiz[i]);
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)(TXT_SMS_SMILE + i), &prompt_str);

        MMI_STRNTOWSTR(item_str.wstr_ptr, GUILIST_STRING_MAX_NUM, (const uint8 *)s_sms_phiz[i], phiz_len, phiz_len);

        for (j = phiz_len; j < PHIZ_MAX_LEN; j++)
        {
            item_str.wstr_ptr[j] = 0x0020;
        }

        MMI_WSTRNCPY(&(item_str.wstr_ptr[PHIZ_MAX_LEN]),
                     MIN(GUILIST_STRING_MAX_NUM - PHIZ_MAX_LEN , prompt_str.wstr_len),
                     prompt_str.wstr_ptr,
                     MIN(GUILIST_STRING_MAX_NUM - PHIZ_MAX_LEN , prompt_str.wstr_len),
                     MIN(GUILIST_STRING_MAX_NUM - PHIZ_MAX_LEN , prompt_str.wstr_len));

        item_str.wstr_len = MIN(PHIZ_MAX_LEN + prompt_str.wstr_len, GUILIST_STRING_MAX_NUM);

        MMISMS_AppendListItem(
            item_str.wstr_ptr,
            (uint8)item_str.wstr_len,
            ctrl_id,
            TXT_COMMON_OK,
            TXT_NULL,
            i,
            MMISMS_APPEND_LIST_ITEM_TYPE
        );
    }

    if (PNULL != item_str.wstr_ptr)
    {
        SCI_FREE(item_str.wstr_ptr);
        item_str.wstr_ptr = PNULL;
    }
}

