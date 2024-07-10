/*****************************************************************************
** Copyright 2021-2022<Name of Copyright Owner: zirui.li>                    *
** Licensed under the Unisoc General Software License,                       *
** version 1.0 (the License);                                                *
** you may not use this file except in compliance with the License.          *
** You may obtain a copy of the License at                                   *
** https://www.unisoc.com/en_us/license/UNISOC_GENERAL_LICENSE_V1.0-EN_US    *
** Software distributed under the License is distributed on an "AS IS" BASIS,*
** WITHOUT WARRANTIES OF ANY KIND, either express or implied.                *
** See the Unisoc General Software License, version 1.0 for more details.    *
******************************************************************************/

/*****************************************************************************
** File Name:   audioplayer_demoapp_main_win.c                               *
** Author:      zirui.li                                                     *
** Date:        10/29/2022                                                   *
** Description: This file is used as audioplayer demoapp main win file       *
******************************************************************************/

/*---------------------------------------------------------------------------*/
/*                          Include Files                                    */
/*---------------------------------------------------------------------------*/
#include "audioplayer_demoapp_main_win.h"
#include "audioplayer_demoapp_flow.h"
#include "ctrlmenu_export.h"
#include "mmiapwin_main.h"
#include "mmimp3_id.h"
#include "mmimp3_text.h"
#include "mmidisplay_color.h"
#include "window_parse.h"
#include "audioplayer_demoapp_menutable.h"
#include "mmiap_appmain.h"
/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define AP_MENU_ITEM_HEIGHT (55)
/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description: Audioplayer demoapp type win handle msg
//  Parameter: [In] win_id   //窗口id
//             [In] msg_id   //消息id
//             [In] param    //参数
//             [Out] None
//             [Return] MMI_RESULT_E //窗口处理结果
//  Author: zirui.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E Audioplayer_View_MainWinHandleMsg(MMI_WIN_ID_T win_id,
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM param)
{
    MMI_RESULT_E    recode   = MMI_RESULT_TRUE;
    uint32          group_id = 0;
    uint32          menu_id  = 0;
    SCI_TRACE_LOW("[Audioplayer Test]Enter Audioplayer test main win, msg_id is:%d", msg_id);
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            CTRLMENU_SetSecMenuFontSize(AP_MAINMENU_CTRL_ID,SONG_FONT_25,SONG_FONT_25);
            CTRLMENU_SetSecMenuItemHeight(AP_MAINMENU_CTRL_ID,AP_MENU_ITEM_HEIGHT,AP_MENU_ITEM_HEIGHT);
            CTRLMENU_SetFontColor(AP_MAINMENU_CTRL_ID,MMI_WHITE_COLOR);
            MMK_SetAtvCtrl(AP_TYPE_WIN_ID, AP_MAINMENU_CTRL_ID);
            break;
        case MSG_FULL_PAINT:
            break;
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            break;
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        case MSG_CTL_MIDSK:
        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            GUIMENU_GetId(AP_MAINMENU_CTRL_ID, &group_id, &menu_id);
            SCI_TRACE_LOW("[Audioplayer Test]menu_id:%d", menu_id);
            switch (menu_id)
            {
                case ID_AUDIOPLAYER_DEMOAPP_NETWORK_MUSIC:
                    AudioPlayer_Enter();
                    break;
                case ID_AUDIOPLAYER_DEMOAPP_LOCAL_MUSIC:
                    MMIAPIMP3_OpenMp3Player();
                    break;
                default:
                    recode = MMI_RESULT_FALSE;
                    break;
            }
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    SCI_TRACE_LOW("[Audioplayer Test]recode is:%d", recode);
    return recode;
}

WINDOW_TABLE(AP_TYPE_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)Audioplayer_View_MainWinHandleMsg),
    WIN_ID(AP_TYPE_WIN_ID),
    WIN_TITLE(TXT_AUDIOPLAYER_DEMOAPP_MUSIC_TYPE),
    CREATE_MENU_CTRL(MENU_AUDIOPLAYER_DEMOAPP_TYPE, AP_MAINMENU_CTRL_ID),
    END_WIN
};
/**--------------------------------------------------------------------------*
**                          PUBLIC FUNCTION                                  *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: Audioplayer test play win enter
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC void Audioplayer_View_MainWin_Enter(void)
{
    MMK_CreateWin((uint32*)AP_TYPE_WIN_TAB, PNULL);
}

