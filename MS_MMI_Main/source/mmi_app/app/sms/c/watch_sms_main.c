/****************************************************************************
** File Name:      watch_sms_main.c                                    *
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
#include "watch_sms_main.h"
#include "watch_sms_presetlist.h"
#include "watch_sms_message.h"
#include "mmicom_trace.h"
#include "ctrllist_export.h"
#include "watch_common_list.h"
#include "guilistbox.h"
#include "guimenu.h"
#include "mmisms_text.h"
#include "mmisms_image.h"
#include "mmisms_anim.h"
#include "mmisms_id.h"
#include "mmisms_app.h"
#include "mmi_pubmenuwin.h"
#include "mmisms_menutable.h"
#include "mmisms_order.h"
#if defined(MMI_SMSCB_SUPPORT)
#include "mmisms_export.h"
#endif
#include "mmimms_export.h"
#include "mmisms_messagecountwin.h"
#include "mmisms_messageboxwin.h"
#ifdef MMI_SMS_SECURITYBOX_SUPPORT
#include "mmisms_securitywin.h"
#endif
#include "mmiudisk_export.h"
#ifdef MMISEARCH_SUPPORT_ONLYMMS
#include "mmisearch_export.h"
#endif
#include "mmisms_appcontrol.h"
#ifdef MMI_SMS_SIMBOX
#include "mmisms_simboxwin.h"
#endif
#include "mmisms_showsmswin.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMISMS_MAIN_ITEM_MAX_NUM   (2)

/**--------------------------------------------------------------------------*
 **                        STATIC DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Draw Sms Main List Item
//  Parameter: [In] ctrl Id
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL void DrawSmsMainListItem( MMI_CTRL_ID_T ctrl_id )
{
    MMI_STRING_T detailText = {0};
    MMI_STRING_T presetText = {0};
    uint16  curSelection   =   0;

    GUILIST_SetMaxItem( ctrl_id, MMISMS_MAIN_ITEM_MAX_NUM, FALSE );
    GUILIST_RemoveAllItems(ctrl_id);
    //message detail/chat
    MMI_GetLabelTextByLang(TXT_SMS_DETAIL, &detailText);
    WatchCOM_ListItem_Draw_1Icon_1Str(ctrl_id,IMAGE_SMS_MAIN_DETAIL,detailText);
    //message preset
    MMI_GetLabelTextByLang(TXT_SMS_PRESET, &presetText);
    WatchCOM_ListItem_Draw_1Icon_1Str(ctrl_id,IMAGE_SMS_MAIN_PRESET,presetText);
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, curSelection, TRUE);
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,curSelection);
}

/*****************************************************************************/
//  Description : to handle the SMS main menu list window message
//  Parameter: [In] win Id
//             [In] msg id
//             [In] param
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSmsMainMenuListWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM                param
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrlId = MMISMS_MAINWIN_LIST_CTRL_ID;
    uint16 index = 0;
    TRACE_APP_MESSAGES("win_id = %0X, msg_id = %0X.", win_id, msg_id);
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            DrawSmsMainListItem(ctrlId);
            MMK_SetAtvCtrl(win_id, ctrlId);
            break;

#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif 
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
            index = GUILIST_GetCurItemIndex(ctrlId);
            if(0 == index )
            {
                MMISMS_SetPresetFrom( MMISMS_PRESETFROM_CHAT );
                WatchSMS_MessageWin_Enter();
            }
            else if(1 == index )
            {
                MMISMS_SetPresetFrom( MMISMS_PRESETFROM_NORMAL );
                WatchSMS_PresetListWin_Enter(PNULL);
            }
            break;

        case MSG_GET_FOCUS:
            DrawSmsMainListItem(ctrlId);
            break;
            
        case MSG_LOSE_FOCUS:
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:                       
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}


/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
// the window for SMS main menu
WINDOW_TABLE( MMISMS_MENUWINLIST_TAB ) =
{
    WIN_FUNC((uint32) HandleSmsMainMenuListWinMsg),
    WIN_HIDE_STATUS,
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,  MMISMS_MAINWIN_LIST_CTRL_ID),
    WIN_ID( MMIMSG_MAIN_MENU_LIST_WIN_ID ),
#ifndef TOUCH_PANEL_SUPPORT
    WIN_SOFTKEY(TXT_NULL, STXT_OK, STXT_RETURN),
#endif
    END_WIN
};

/*****************************************************************************/
//  Description : enter SMS main menu list win
//  Parameter: [In] None
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void WatchSMS_MainMenuWin_Enter(void)
{
    MMI_HANDLE_T winHandle = 0;
    if(MMK_IsOpenWin(MMIMSG_MAIN_MENU_LIST_WIN_ID))
    {
        TRACE_APP_MESSAGES("watchsms_mainmenu_win is already open");
        return;
    }
    
    winHandle  = MMK_CreateWin((uint32*)MMISMS_MENUWINLIST_TAB, PNULL);
    TRACE_APP_MESSAGES("WatchSMS_MainMenuWin_Enter create success");
}

