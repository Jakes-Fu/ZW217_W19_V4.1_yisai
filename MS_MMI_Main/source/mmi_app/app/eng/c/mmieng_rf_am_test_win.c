/*****************************************************************************
** Copyright 2021-2022<Name of Copyright Owner: zhenpeng.yao>                *
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
** File Name:   mmieng_rf_am_test_win.c                                      *
** Author:      zhenpeng.yao                                                 *
** Date:        10/10/2022                                                   *
** Description: This file is used as eng rf arbitration test file            *
******************************************************************************/

/*---------------------------------------------------------------------------*/
/*                          Include Files                                    */
/*---------------------------------------------------------------------------*/
#include "mmieng_rf_am_test_win.h"
#include "sci_types.h"
#include "mmk_type.h"
#include "guilistbox.h"
#include "mmieng_text.h"
#include "mmieng_menutable.h"
#include "window_parse.h"
#include "mmieng_id.h"
#include "mmicom_trace.h"
#include "ual_rf_am.h"
#include "mmieng_export.h"
#include "mmidisplay_color.h"
/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description: ENG RF AM Test win handle msg
//  Parameter: [In] win_id     //窗口id
//             [In] msg_id     //消息id
//             [In] param      //参数
//             [Out] None
//             [Return] MMI_RESULT_E
//  Author: zhenpeng.yao
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E Eng_RF_AM_TestWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint8 i=0;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            SCI_TRACE_LOW("[RF_AM]:open Eng_RF_AM_TestWinHandleMsg, MSG_OPEN_WINDOW\n");
            CTRLMENU_SetSecMenuFontSize(ID_ENG_RF_AM_TEST_MENU_ID,MMIENG_MENU_ITEM_FONT,MMIENG_MENU_ITEM_FONT);
            CTRLMENU_SetSecMenuItemHeight(ID_ENG_RF_AM_TEST_MENU_ID,MMIENG_MENU_ITEM_HEIGHT,MMIENG_MENU_ITEM_HEIGHT);
            CTRLMENU_SetFontColor(ID_ENG_RF_AM_TEST_MENU_ID,MMI_WHITE_COLOR);
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_ENG_OK, TXT_NULL, TXT_ENG_RETURN, FALSE);

            MMK_SetAtvCtrl(win_id,ID_ENG_RF_AM_TEST_MENU_ID);

            break;
        }
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        {
            uint32 group_id = 0;
            uint32 menu_id = 0;

            GUIMENU_GetId(ID_ENG_RF_AM_TEST_MENU_ID,&group_id,&menu_id);

            SCI_TRACE_LOW("[RF_AM]:menu_id:%d",menu_id);
            switch (menu_id)
            {
#ifdef GPS_SUPPORT
                case ID_ENG_RF_AM_TEST_BACKGROUND:
                {
#ifdef    UAL_RF_AM_SUPPORT
                    ual_rf_am_set_mode(UAL_RF_AM_BACKGROUND_POSITION_MODE);
#endif
                    MMIENG_OpenGPSTestWin();
                    break;
                }

                case ID_ENG_RF_AM_TEST_SPORT:
                {
#ifdef    UAL_RF_AM_SUPPORT
                    ual_rf_am_set_mode(UAL_RF_AM_SPORT_POSITION_MODE);
#endif
                    MMIENG_OpenGPSTestWin();
                    break;
                }
                case ID_ENG_RF_AM_TEST_COMMUNICATION:
                {
#ifdef    UAL_RF_AM_SUPPORT
                    ual_rf_am_set_mode(UAL_RF_AM_COMMUNICATION_MODE);
#endif
                    MMIENG_OpenGPSTestWin();
                    //WatchCC_DialpadWin_Enter();
                    break;
                }
#endif
                default:
                    break;
            }

            break;
        }
        case MSG_FULL_PAINT:
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMK_CloseWin(win_id);
            break;
        }
        default:
        {
            result = MMI_RESULT_FALSE;
            break;
        }
    }
    return result;
}

WINDOW_TABLE(MMIENG_RF_AM_Test_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)Eng_RF_AM_TestWinHandleMsg),
    WIN_ID(MMIENG_TEST_RF_AM_WIN_ID),
    WIN_TITLE(TXT_ENG_RF_AM_TEST),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_RF_AM_INFO, ID_ENG_RF_AM_TEST_MENU_ID),
    END_WIN

};
/**--------------------------------------------------------------------------*
**                          PUBLIC FUNCTION                                  *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: RF Arbitration Test win enter
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: zhenpeng.yao
//  Note:
/*****************************************************************************/
void Eng_RF_AM_Test_Win_Enter(void)
{
    MMK_CreateWin((uint32*)MMIENG_RF_AM_Test_WIN_TAB, PNULL);
}

