/*****************************************************************************
** File Name:      mmiap_multimediamenu.c                                    *
** Author:                                                                   *
** Date:           28/08/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to audio player                         *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2012       robert.wang       Create                                    *
******************************************************************************/

#define _MMIAP_MULTIMEDIA_C_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef OPTION_PAGE_MAIN_MENU_SUPPORT // 目前查看，只有option menu有这个功能，暂时屏蔽

#include "mmipub.h"
#include "mmi_text.h"
#include "sig_code.h"

//other module
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif

#include "mmifm_export.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmimp3_export.h"
#include "mmimtv_export.h"
//local module

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle player main menu window message
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePlayerMenuWinMsg(
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E  msg_id,
    DPARAM            param
);

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
// the window for player menu window
LOCAL WINDOW_TABLE(MMIPLAYER_MAIN_MENU_WIN_TAB) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandlePlayerMenuWinMsg),
    WIN_ID(MMIPLAYER_MAIN_MENU_WIN_ID),
    WIN_TITLE(STXT_MAIN_PLAYER),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_PLAYER, MMIPLAYER_MAIN_MENU_CTRL_ID),
    END_WIN
};
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : handle player main menu window message
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePlayerMenuWinMsg(
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E  msg_id,
    DPARAM            param
)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id = MMIPLAYER_MAIN_MENU_CTRL_ID;
    MMI_MENU_ID_T           menu_id = 0;
    MMI_MENU_GROUP_ID_T     group_id = 0;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);

#ifdef STREAMING_SUPPORT_NEXTREAMING

        if((MMI_GMMREG_RAT_3G == MMIAPIPHONE_GetTDOrGsm(MMI_DUAL_SYS_MAX)) || (MMI_GMMREG_RAT_GPRS_AND_3G == MMIAPIPHONE_GetTDOrGsm(MMI_DUAL_SYS_MAX)))
        {
            GUIMENU_SetItemGrayed(MMIPLAYER_MAIN_MENU_CTRL_ID, MENU_PLAYER, ID_PLAYER_ST, FALSE);
        }
        else
        {
            GUIMENU_SetItemGrayed(MMIPLAYER_MAIN_MENU_CTRL_ID, MENU_PLAYER, ID_PLAYER_ST, TRUE);
        }

#endif
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);

        switch(menu_id)
        {
        case ID_PLAYER_MP3:
            MMIAPIMP3_OpenMp3Player();

            break;

        case ID_PLAYER_VIDEO:
#ifdef VIDEO_PLAYER_SUPPORT
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
            MMIAPIVP_MainEntry();
#endif
#endif
            break;

#ifdef FM_SUPPORT

        case ID_PLAYER_FM:
            MMIAPIFM_OpenMainWin();
            break;
#endif

#ifdef STREAMING_SUPPORT_NEXTREAMING

        case ID_PLAYER_ST:
            MMIAPIST_SetFromNet(FALSE);
            MMIAPIST_OpenPlayWin();

            break;
#endif

#ifdef KURO_SUPPORT

        case ID_PLAYER_KUR:
            MMIAPIKUR_OpenKurPlayer();
            break;
#endif

        case ID_PLAYER_MTV:
            MMIAPIMTV_OpenMTVPlayer();
            break;

        default:
            //SCI_TRACE_LOW:"[MMIMP3]:HandlePlayerMenuWinMsg, mmenu_id = %d error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_2595_112_2_18_2_4_42_298, (uint8*)"d", menu_id);
            break;
        }

        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : open player menu window
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_OpenPlayerMenuWin(void)
{
    MMK_CreateWin((uint32 *)MMIPLAYER_MAIN_MENU_WIN_TAB, PNULL);
}

#endif

