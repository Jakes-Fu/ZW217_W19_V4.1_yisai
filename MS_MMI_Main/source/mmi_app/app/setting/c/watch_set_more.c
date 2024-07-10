/*****************************************************************************
** File Name:      watch_set_more.c                                          *
** Author:         bin.wang1                                                 *
** Date:           03/13/2020                                                *
** Copyright:      All Rights Reserved.                                      *
** Description: display some more settings information                       *
******************************************************************************
**                         Important Edit History
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 03/13/2020           bin.wang1             Create                         *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/

#include "guilistbox.h"
#include "watch_common_list.h"
#include "os_api.h"
#include "sci_types.h"
#include "window_parse.h"
#include "mmicom_trace.h"
#include "mmisrveng.h"
#include "mmiset_image.h"
#include "mmiset_text.h"
#include "mmiset_id.h"
#include "watch_common_radiolist_win.h"
#include "watch_commonwin_export.h"
#include "mmiphone_export.h"
#include "mmi_appmsg.h"
#include "mmiset_func.h"
#ifdef DYNAMIC_WATCHFACE_SUPPORT
#include "watchface_config.h"
#include "watchface_factory_reset_waiting_win_view.h"
#endif
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
#define  RESET_SLEEP_TIME  1000
typedef enum
{
#ifdef MMI_WIFI_SUPPORT
    SETTINGS_MORE_WIFI,
#endif
#ifdef MMI_VOLTE_SUPPORT
    SETTINGS_MORE_VOLTE,
#endif
    SETTINGS_MORE_FACTORY,
    SETTINGS_MORE_ITEM_MAX
}SETTINGS_MORE_LIST_ITEM_E;
typedef void ( *SET_MORE_ITEM_FUNC )( void );

//================================================================================/
//===================== settings->more->volte select win data ====================/
//================================================================================/
#ifdef MMI_VOLTE_SUPPORT

typedef enum
{
    SETTINGS_VOLTE_SELECT_OPEN,
    SETTINGS_VOLTE_SELECT_CLOSE,
    SETTINGS_VOLTE_SELECT_ITEM_MAX
}
SETTINGS_VOLTE_SELECT_LIST_ITEM_E;

LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_volte_select_on      = { TXT_SET_OPEN };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_volte_select_off     = { TXT_SET_CLOSE };

LOCAL WATCHCOM_LIST_ITEM__ST s_volte_select_list[] =
{
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_RADIO,   &s_volte_select_on,     PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_RADIO,   &s_volte_select_off,    PNULL },
};

LOCAL WATCHCOM_RADIOLIST_WIN_INFO_ST s_volte_select_win              = { MMISET_VOLTE_SELECT_WIN_ID,
                                                                         MMISET_VOLTE_SELECT_LIST_CTRL_ID,
                                                                         s_volte_select_list,
                                                                         PNULL,
                                                                         0,
                                                                         SETTINGS_VOLTE_SELECT_ITEM_MAX};

LOCAL MMI_RESULT_E _Callback_VolteSelectWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_NOTIFY_LIST_SET_SELECT:
        {
            uint8           index = MN_DUAL_SYS_1;
            BOOLEAN         isOpen = FALSE;
            uint16          curIdx = GUILIST_GetCurItemIndex( MMISET_VOLTE_SELECT_LIST_CTRL_ID );
            MMI_TEXT_ID_T   noteText = TXT_SET_VOLTE_CLOSED;
            MMI_IMAGE_ID_T  noteImage = IMAGE_SET_VOLTE_CLOSED;
            MMI_STRING_T    noteStr = { 0 };
            WATCH_SOFTKEY_TEXT_ID_T softKey = { 0 };
            MMI_RESULT_E    result = MMI_RESULT_FALSE;

            if( SETTINGS_VOLTE_SELECT_OPEN == curIdx )
            {
                isOpen = TRUE;
                noteText = TXT_SET_VOLTE_OPENED;
                noteImage = IMAGE_SET_VOLTE_OPENED;
            }
            for( index = MN_DUAL_SYS_1; index < MMI_DUAL_SYS_MAX; index++ )
            {
                result = MMIAPIPHONE_SetVolteInfo( index, isOpen );
            }

            if(MMI_RESULT_TRUE == result)
            {
                MMI_GetLabelTextByLang( noteText, &noteStr );
                // Bug 1346793 
                WatchCOM_NoteWin_FullScreen_Enter( MMISET_VOLTE_CHANGED_WIN_ID, NULL, &noteStr, noteImage, NULL, softKey, PNULL );
            }
            else
            {
                MMI_GetLabelTextByLang( TXT_SET_SETTING_FAIL, &noteStr );
                WatchCOM_NoteWin_1Line_Enter( MMISET_VOLTE_CHANGED_WIN_ID, &noteStr, NULL, softKey, NULL );
            }
            MMK_CloseWin( win_id );
            break;
        }
        case MSG_FULL_PAINT:
        {
            uint8 volte = MMIAPIPHONE_GetVolteInfo();
            BOOLEAN is_volte_on = FALSE;

            is_volte_on =  volte & (1 << MN_DUAL_SYS_1);
            if( TRUE == is_volte_on )// on
            {
                s_volte_select_win.listCurIdx   = SETTINGS_VOLTE_SELECT_OPEN;
            }
            else
            {
                s_volte_select_win.listCurIdx   = SETTINGS_VOLTE_SELECT_CLOSE;
            }
            GUILIST_SetCurItemIndex( MMISET_VOLTE_SELECT_LIST_CTRL_ID, s_volte_select_win.listCurIdx );
            GUILIST_SetSelectedItem( MMISET_VOLTE_SELECT_LIST_CTRL_ID, s_volte_select_win.listCurIdx, TRUE );
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

PUBLIC void Settings_VolteSelectWin_Enter( void )
{
    uint8 volte = MMIAPIPHONE_GetVolteInfo();
    BOOLEAN is_volte_on = FALSE;
    GUI_RECT_T          list_rect = WATCHLIST_RECT;

    is_volte_on =  volte & (1 << MN_DUAL_SYS_1);
    if( TRUE == is_volte_on )// on
    {
        s_volte_select_win.listCurIdx   = SETTINGS_VOLTE_SELECT_OPEN;
    }
    else
    {
        s_volte_select_win.listCurIdx   = SETTINGS_VOLTE_SELECT_CLOSE;
    }
    s_volte_select_win.pCallback        = _Callback_VolteSelectWin;
    WatchCOM_RadioListWin_Create( &s_volte_select_win );
#ifdef SCREEN_SHAPE_CIRCULAR
    GUILIST_SetRect(MMISET_VOLTE_SELECT_LIST_CTRL_ID,&list_rect);
#endif
}
#endif
//================================================================================/
//=================== settings->more->factory reset query win ====================/
//================================================================================/

LOCAL void HandleResetPhoneTimer(uint8 timer_id,uint32 param)
{
    MMIAPIPHONE_PowerReset();
}

LOCAL void FactoryResetWaitingWinEnter(void)
{
    MMI_STRING_T            operating_str = {0};
    WATCH_SOFTKEY_TEXT_ID_T softkey_text  = {NULL, NULL, NULL};

    MMI_GetLabelTextByLang(TXT_SET_FACTORY_RESETTING, &operating_str);
#ifdef  ADULT_WATCH_SUPPORT
    AdultWatchCOM_WaitingWin_1Line_Enter(MMISET_FACTORY_RESET_WAITING_WIN_ID,&operating_str,0,PNULL);
#else
    WatchCOM_WaitingWin_1Line_Enter(MMISET_FACTORY_RESET_WAITING_WIN_ID,&operating_str,0,softkey_text,PNULL);
#endif
}

LOCAL MMI_RESULT_E _Callback_FacotryResetWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;

    switch(msg_id)
    {

        case MSG_NOTIFY_OK:
        {
            // Left cancel button selected
            MMK_CloseWin( win_id );
            break;
        }
        case MSG_NOTIFY_CANCEL:
        {
            // Right ok button selected
#ifdef DYNAMIC_WATCHFACE_SUPPORT
            WATCHFACE_FactoryReset();
#else
            MMISET_CleanUserData();
            MMISET_ResetFactorySetting();
            FactoryResetWaitingWinEnter();
            MMK_CreateTimerCallback(RESET_SLEEP_TIME, HandleResetPhoneTimer, 0, FALSE);   //bug2113714
#endif
            break;
        }
#ifdef DYNAMIC_WATCHFACE_SUPPORT
        case MSG_WATCHFACE_FACTORY_RESET:
        {
            WATCHFACE_CloseWaitingFactoryResetWin();
            MMISET_CleanUserData();
            MMISET_ResetFactorySetting();
            FactoryResetWaitingWinEnter();
            MMK_CreateTimerCallback(RESET_SLEEP_TIME, HandleResetPhoneTimer, 0, FALSE);   //bug2113714
            break;
        }
#endif
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

LOCAL void SettingsFacotryResetWin_Enter( void )
{
    MMI_STRING_T            queryTipStr = { 0 };
    WATCH_SOFTKEY_TEXT_ID_T softkey = { STXT_RETURN, PNULL, STXT_OK };

    MMI_GetLabelTextByLang( TXT_COMMON_RESTORE_FACTORY_SETTINGS, &queryTipStr );
    WatchCOM_QueryWin_2Btn_Enter(
                                    MMISET_FACTORY_RESET_WIN_ID,
                                    &queryTipStr,
                                    PNULL,
                                    PNULL,
                                    IMAGE_SET_COM_BTN_CANCEL,
                                    IMAGE_SET_COM_BTN_FIXED,
                                    softkey,
                                    _Callback_FacotryResetWin
                                );
}


//================================================================================/
//======================= settings->more win =====================================/
//================================================================================/

/**---------------------------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DEFINITION                                           *
 **---------------------------------------------------------------------------------------------*/

#ifdef MMI_VOLTE_SUPPORT
LOCAL void SettingsMoreGetVolteStr( MMI_STRING_T* volte_str_ptr )
{
    MMI_TEXT_ID_T volteTextId = TXT_SET_CLOSED;
    uint8 volte = MMIAPIPHONE_GetVolteInfo();
    if( volte > 0 ) // open
    {
        volteTextId = TXT_SET_OPENED;
    }
    MMI_GetLabelTextByLang( volteTextId, volte_str_ptr );
}
#endif

LOCAL void HandleSettingsFacotryReset( void )
{
    if(!MMIENG_IsMonkeyTestMode())
    {
        SettingsFacotryResetWin_Enter();
    }
}
LOCAL void Settings_More_APP_OK( void )
{
    uint16 index = GUILIST_GetCurItemIndex( MMISET_MORE_LIST_CTRL_ID );
    uint32 userData = 0;
    SET_MORE_ITEM_FUNC itemFunc = PNULL;

    GUILIST_GetItemData( MMISET_MORE_LIST_CTRL_ID, index, &userData );
    itemFunc = (SET_MORE_ITEM_FUNC)userData;

    if(PNULL != itemFunc)
    {
        itemFunc();
    }
    else
    {
        TRACE_APP_SETTINGS("Set_more item data is null");
    }

    TRACE_APP_SETTINGS("itemFunc = %0x.", itemFunc);
}

LOCAL void Settings_More_APP_CANCEL( MMI_WIN_ID_T win_id )
{
    MMK_CloseWin( win_id );
}

LOCAL void Settings_More_OPEN_WINDOW( MMI_WIN_ID_T win_id )
{
    MMI_CTRL_ID_T listCtrlId = MMISET_MORE_LIST_CTRL_ID;
#ifdef SCREEN_SHAPE_CIRCULAR
    GUI_RECT_T          list_rect = WATCHLIST_RECT;
    GUILIST_SetRect(listCtrlId,&list_rect);
#endif
    TRACE_APP_WLAN("Enter");

    MMK_SetAtvCtrl( win_id, listCtrlId );

    GUILIST_SetMaxItem( listCtrlId, SETTINGS_MORE_ITEM_MAX,  FALSE );
}

LOCAL void Settings_More_FULL_PAINT( MMI_WIN_ID_T win_id )
{
    uint16              curIndex = 0;
    uint16              topIndex = 0;
    uint32              i        = MN_DUAL_SYS_1;
    uint32              user_data = 0;
    MMI_STRING_T        itemStr = { 0 };
    MMI_STRING_T        itemStr2 ={ 0 };
    MMI_CTRL_ID_T       listCtrlId =  MMISET_MORE_LIST_CTRL_ID;

    curIndex = GUILIST_GetCurItemIndex( listCtrlId );
    topIndex = GUILIST_GetTopItemIndex( listCtrlId );
    GUILIST_RemoveAllItems( listCtrlId );

#ifdef MMI_WIFI_SUPPORT
    //wifi
    MMI_GetLabelTextByLang( TXT_SET_CONNECT_WIFI, &itemStr );
    WatchCOM_ListItem_Draw_1Str( listCtrlId, itemStr );
    user_data = (uint32)WatchWIFI_MainWin_Enter;
    GUILIST_SetItemUserData(listCtrlId,GUILIST_GetTotalItemNum(listCtrlId)-1,&user_data);
#endif

#ifdef MMI_VOLTE_SUPPORT
    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
       if(SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(i))
       {
            //volte
            MMI_GetLabelTextByLang( TXT_SET_VOLTE, &itemStr );
            SettingsMoreGetVolteStr( &itemStr2 );
            WatchCOM_ListItem_Draw_2Str( listCtrlId, itemStr, itemStr2 );
            user_data = (uint32)Settings_VolteSelectWin_Enter;
            GUILIST_SetItemUserData(listCtrlId,GUILIST_GetTotalItemNum(listCtrlId)-1,&user_data);
        }
    }
#endif
    //factory reset
    MMI_GetLabelTextByLang( TXT_SET_FACTORY_RESET,  &itemStr );
    WatchCOM_ListItem_Draw_1Str( listCtrlId, itemStr );
    user_data = (uint32)HandleSettingsFacotryReset;
    GUILIST_SetItemUserData(listCtrlId,GUILIST_GetTotalItemNum(listCtrlId)-1,&user_data);

    GUILIST_SetTopItemIndex( listCtrlId, topIndex );
    GUILIST_SetCurItemIndex( listCtrlId, curIndex );
}

LOCAL MMI_RESULT_E  HandleSettingsMoreWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            Settings_More_OPEN_WINDOW( win_id );
            break;
        }

        case MSG_FULL_PAINT:
        {
            Settings_More_FULL_PAINT( win_id );
            break;
        }

        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        {
            Settings_More_APP_OK();
            break;
        }

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            Settings_More_APP_CANCEL( win_id );
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
WINDOW_TABLE( MMISET_MORE_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleSettingsMoreWindow ),
    WIN_ID( MMISET_MORE_WIN_ID ),
    WIN_HIDE_STATUS,
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_MORE_LIST_CTRL_ID),
    WIN_SOFTKEY( STXT_OK, TXT_NULL, STXT_RETURN ),
    END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
// idle -> settings -> sound
PUBLIC void Settings_MoreWin_Enter( void )
{
    MMK_CreateWin( (uint32 *)MMISET_MORE_WIN_TAB, PNULL );
}
