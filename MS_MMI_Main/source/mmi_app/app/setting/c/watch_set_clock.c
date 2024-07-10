/*****************************************************************************
** File Name:      watch_set_clock.c      //clock settings                   *
** Author:         bin.wang1                                                 *
** Date:           03/16/2020                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:  alarm list window                                           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2020      bin.wang1              Creat                                 *
******************************************************************************/
#ifndef DYNAMIC_WATCHFACE_SUPPORT
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "guilistbox.h"
#include "watch_common_list.h"
#include "os_api.h"
#include "sci_types.h"
#include "window_parse.h"
#include "guilabel.h"
#include "guibutton.h"
#include "mmidisplay_data.h"

#include "mmicom_trace.h"
#include "mmiset_image.h"
#include "mmiset_text.h"
#include "mmiset_id.h"
#include "mmiset_export.h"
#include "mmiacc_event.h"

#include "watch_set_clock.h"
#include "watch_common_radiolist_win.h"
#include "watch_launcher_editclockwin.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

typedef enum
{
    SETTINGS_CLOCK_STYLE,   // pointer or digit
    SETTINGS_CLOCK_MODE,    // 12h or 24h
    SETTINGS_CLOCK_ITEM_MAX
}
SETTINGS_CLOCK_ITEM_E;

//================================================================================/
//===================== settings->clock->mode select win data ====================/
//================================================================================/

LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_clock_mode_select_12hour      = { TXT_COMM_12_HOURS };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_clock_mode_select_24hour      = { TXT_COMM_24_HOURS };

LOCAL WATCHCOM_LIST_ITEM__ST s_clock_mode_select_list[] =
{
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_RADIO,   &s_clock_mode_select_12hour,     PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_RADIO,   &s_clock_mode_select_24hour,     PNULL },
};

LOCAL WATCHCOM_RADIOLIST_WIN_INFO_ST s_clock_mode_select_win         = { MMISET_CLOCK_MODE_SELECT_WIN_ID,
                                                                         MMISET_CLOCK_MODE_SELECT_LIST_CTRL_ID,
                                                                         s_clock_mode_select_list,
                                                                         PNULL,
                                                                         0,
                                                                         MMISET_MAX_TIME_DISPLAY_TYPE};

LOCAL MMI_RESULT_E _Callback_ClockModeSelectWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;

    switch(msg_id)
    {

        case MSG_NOTIFY_LIST_SET_SELECT:
        {
            uint16 curIdx = GUILIST_GetCurItemIndex( MMISET_CLOCK_MODE_SELECT_LIST_CTRL_ID );

            MMIAPISET_SetTimeDisplayType( (MMISET_TIME_DISPLAY_TYPE_E)curIdx );

            break;
        }

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMK_CloseWin( win_id );
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

PUBLIC void Settings_ClockModeSelectWin_Enter( void )
{
    s_clock_mode_select_win.listCurIdx      = MMIAPISET_GetTimeDisplayType();
    s_clock_mode_select_win.pCallback       = _Callback_ClockModeSelectWin;

    WatchCOM_RadioListWin_Create( &s_clock_mode_select_win );
}

//================================================================================/
//======================= settings->clock win ====================================/
//================================================================================/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

LOCAL BOOLEAN SettingsClockGetModeStr( MMI_STRING_T* style_str_ptr )
{
    MMI_TEXT_ID_T modeStrId = TXT_COMM_12_HOURS;

    if( MMISET_TIME_24HOURS == MMIAPISET_GetTimeDisplayType() )
    {
        modeStrId = TXT_COMM_24_HOURS;
    }
    MMI_GetLabelTextByLang( modeStrId, style_str_ptr );
}

LOCAL void SettingsClockGetStyleStr( MMI_STRING_T* style_str_ptr )
{
    MMI_TEXT_ID_T styleStrId = TXT_SET_CLOCK_STYLE_ONE;

    if( MMISET_CLOCK_STYLE_DIGIT == MMIAPISET_GetClockDisplayStyle() )
    {
        styleStrId = TXT_SET_CLOCK_STYLE_TWO;
    }
    MMI_GetLabelTextByLang( styleStrId, style_str_ptr );
}

LOCAL void Settings_Clock_OPEN_WINDOW( MMI_WIN_ID_T win_id )
{
    MMI_CTRL_ID_T       listId = MMISET_CLOCK_LIST_CTRL_ID;

    TRACE_APP_WLAN("Enter");

    MMK_SetAtvCtrl( win_id, listId );

    GUILIST_SetMaxItem( listId, SETTINGS_CLOCK_ITEM_MAX, FALSE );

}

LOCAL void Settings_Clock_FULL_PAINT( MMI_WIN_ID_T win_id )
{
    uint16              curIndex = 0;
    uint16              topIndex = 0;
    MMI_STRING_T        itemStr = { 0 };
    MMI_STRING_T        itemStr2 ={ 0 };
    MMI_CTRL_ID_T       listId = MMISET_CLOCK_LIST_CTRL_ID;

    curIndex = GUILIST_GetCurItemIndex( listId );
    topIndex = GUILIST_GetTopItemIndex( listId );
    GUILIST_RemoveAllItems( listId );

    MMI_GetLabelTextByLang( TXT_SET_CLOCK_STYLE, &itemStr );
    SettingsClockGetStyleStr( &itemStr2 );
    WatchCOM_ListItem_Draw_2Str( MMISET_CLOCK_LIST_CTRL_ID, itemStr, itemStr2 );

    MMI_GetLabelTextByLang( TXT_SET_CLOCK_MODE, &itemStr );
    SettingsClockGetModeStr( &itemStr2 ); // 12h or 24h
    WatchCOM_ListItem_Draw_2Str( MMISET_CLOCK_LIST_CTRL_ID, itemStr, itemStr2 );

    GUILIST_SetTopItemIndex( listId, topIndex );
    GUILIST_SetCurItemIndex( listId, curIndex );
}

LOCAL void Settings_Clock_APP_OK( MMI_WIN_ID_T win_id )
{
    uint16 index = GUILIST_GetCurItemIndex( MMISET_CLOCK_LIST_CTRL_ID );

    switch (index)
    {
        case SETTINGS_CLOCK_STYLE:
        {
            WatchLAUNCHER_EditClockWin_Enter();
            break;
        }

        case SETTINGS_CLOCK_MODE:
        {
            Settings_ClockModeSelectWin_Enter();
            break;
        }

        default:
        {
            TRACE_APP_SETTINGS(" index error %d", index);
            break;
        }
    }
}

LOCAL void Settings_Clock_APP_CANCEL( MMI_WIN_ID_T win_id )
{
    MMK_CloseWin( win_id );
}

LOCAL MMI_RESULT_E  HandleSettingsClockWindow(
                                         MMI_WIN_ID_T       win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E reCode =  MMI_RESULT_TRUE;

    switch(msg_id)
    {

        case MSG_OPEN_WINDOW:
        {
            Settings_Clock_OPEN_WINDOW( win_id );
            break;
        }

        case MSG_FULL_PAINT:
        {
            Settings_Clock_FULL_PAINT( win_id );
            break;
        }

        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        {
            Settings_Clock_APP_OK( win_id );
            break;
        }

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            Settings_Clock_APP_CANCEL( win_id );
            break;
        }

        default:
            reCode = MMI_RESULT_FALSE;
            break;
    }

    return reCode;
}

//idle -> settings -> clock
WINDOW_TABLE( MMISET_CLOCK_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleSettingsClockWindow ),
    WIN_ID( MMISET_CLOCK_WIN_ID ),
    WIN_HIDE_STATUS,
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_CLOCK_LIST_CTRL_ID ),
    WIN_SOFTKEY( STXT_OK, TXT_NULL, STXT_RETURN ),
    END_WIN
};

/*****************************************************************************/
//  Discription: enter setttings->clock window                               *
//  Global resource dependence:                                              *
//  Author: bin.wang1                                                        *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC void Settings_ClockWin_Enter( void )
{
    MMK_CreateWin( (uint32 *)MMISET_CLOCK_WIN_TAB, PNULL );
}
#endif
