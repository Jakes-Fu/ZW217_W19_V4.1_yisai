/*****************************************************************************
** File Name:      watch_set_display.c                                       *
** Author:         xiaoju.cheng                                              *
** Date:           01/06/2021                                                *
** Copyright:      All Rights Reserved.                                      *
** Description: Display display settings items                               *
******************************************************************************
**                         Important Edit History
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 01/06/2020           xiaoju.cheng             Create                      *
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
#include "mmiset_export.h"
#include "mmiphone_export.h"
#include "watch_commonwin_export.h"
#ifdef UAL_SS_SUPPORT
#include "watch_set_call_forwarding.h"
#endif
#include "mmieng_export.h"
#include "mmiset_func.h"
#include "mmiphone_export.h"
#include "watch_set_about.h"
#include "mmi_appmsg.h"
#ifdef DYNAMIC_WATCHFACE_SUPPORT
#include "watchface_config.h"
#include "watchface_factory_reset_waiting_win_view.h"
#endif
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
typedef enum
{
    SETTINGS_SYSTEM_ABOUT,
    SETTINGS_SYSTEM_ATTACH_NEW_PHONE,
    SETTINGS_SYSTEM_POWER_RESET,
    SETTINGS_SYSTEM_POWER_OFF,
    SETTINGS_SYSTEM_FACTORY_RESET,
    SETTINGS_SYSTEM_DIVERT,
    SETTINGS_SYSTEM_ITEM_MAX
}
SETTINGS_SYSTEM_LIST_ITEM_E;

/**---------------------------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DEFINITION                                           *
 **---------------------------------------------------------------------------------------------*/

LOCAL void Settings_System_OPEN_WINDOW( MMI_WIN_ID_T win_id )
{
    MMI_CTRL_ID_T   listCtrlId =  MMISET_SYSTEM_LIST_CTRL_ID;

    GUILIST_SetMaxItem( listCtrlId, SETTINGS_SYSTEM_ITEM_MAX,  FALSE );
    MMK_SetAtvCtrl( win_id, listCtrlId );
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
            MMISET_ResetFactorySetting();
            MMK_CloseWin( win_id );
#endif
            break;
        }
#ifdef DYNAMIC_WATCHFACE_SUPPORT
        case MSG_WATCHFACE_FACTORY_RESET:
        {
            WATCHFACE_CloseWaitingFactoryResetWin();
            MMISET_ResetFactorySetting();
            MMK_CloseWin( win_id );
            break;//bug 1962665 CID 590215
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

    MMI_GetLabelTextByLang( TXT_SET_SYSTEM_FAC_RESET_ALERT, &queryTipStr );
    AdultWatchCOM_QueryWin_2Btn_Enter(
                                    MMISET_FACTORY_RESET_WIN_ID,
                                    &queryTipStr,
                                    PNULL,
                                    PNULL,
                                    IMAGE_SET_COM_AW_BTN_CANCEL,
                                    IMAGE_SET_COM_AW_BTN_CONFIRM,
                                    _Callback_FacotryResetWin
                                );
}

LOCAL MMI_RESULT_E _Callback_PowerResetWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_NOTIFY_OK:
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        {
            MMIAPIPHONE_PowerReset();
            break;
        }
        case MSG_NOTIFY_CANCEL:
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

LOCAL MMI_RESULT_E _Callback_PowerOffWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;

    switch(msg_id)
    {

        case MSG_NOTIFY_OK:
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        {
            MMIAPIPHONE_PowerOffEx();
            break;
        }
        case MSG_NOTIFY_CANCEL:
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

LOCAL void Settings_System_APP_OK( void )
{
    MMI_STRING_T  NoteTipStr = { 0 };
    uint16 index = GUILIST_GetCurItemIndex( MMISET_SYSTEM_LIST_CTRL_ID );

    switch (index)
    {
        case SETTINGS_SYSTEM_ABOUT:
        {
            Settings_RegardWin_Enter();
            break;
        }

        case SETTINGS_SYSTEM_ATTACH_NEW_PHONE:
        {
            MMI_GetLabelTextByLang( TXT_SET_SYSTEM_ALERT, &NoteTipStr );
            Adult_WatchCOM_NoteWin_1Line_Toast_Enter(MMISET_POWER_RESET_WIN_ID,&NoteTipStr,PNULL,PNULL);
            break;
        }

        case SETTINGS_SYSTEM_POWER_RESET:
        {
            if(!MMIENG_IsMonkeyTestMode())
            {
                MMI_GetLabelTextByLang( TXT_SET_SYSTEM_POWER_RESET_ALERT, &NoteTipStr );
                AdultWatchCOM_QueryWin_1Btn_1Str_Enter(MMISET_POWER_RESET_WIN_ID,PNULL,res_sys_btn_restart,&NoteTipStr,_Callback_PowerResetWin);
            }
            break;
        }

        case SETTINGS_SYSTEM_POWER_OFF:
        {
            if(!MMIENG_IsMonkeyTestMode())
            {
                MMI_GetLabelTextByLang( TXT_SET_SYSTEM_POWER_OFF_ALERT, &NoteTipStr );
                AdultWatchCOM_QueryWin_1Btn_1Str_Enter(MMISET_POWER_OFF_WIN_ID,PNULL,res_sys_btn_shutdown,&NoteTipStr,_Callback_PowerOffWin);
            }
            break;
        }

        case SETTINGS_SYSTEM_FACTORY_RESET:
        {
            if(!MMIENG_IsMonkeyTestMode())
            {
                SettingsFacotryResetWin_Enter();
            }
            break;
        }
#ifdef UAL_SS_SUPPORT
        case SETTINGS_SYSTEM_DIVERT:
            Watch_Settings_CallForwardingWin_Enter();
            break;
#endif
        default:
        {
            TRACE_APP_SETTINGS(" index error %d", index);
            break;
        }
    }
}

LOCAL void Settings_System_APP_CANCEL( MMI_WIN_ID_T win_id )
{
    MMK_CloseWin( win_id );
}

LOCAL void Settings_System_FULL_PAINT( MMI_WIN_ID_T win_id )
{
    uint16          curSelection = 0;
    int32           listOffset = 0;
    uint16          ringId = 0;
    uint16          modeIndex = 0;
    MN_DUAL_SYS_E   dualSys = MMIAPISET_GetActiveSim();
    MMI_TEXT_ID_T*  pStr2TextId = TXT_NULL;
    MMI_STRING_T    itemStr = { 0 };
    MMI_CTRL_ID_T   listCtrlId =  MMISET_SYSTEM_LIST_CTRL_ID;

    // get top item offset
    GUILIST_GetTopItemOffset( listCtrlId, &listOffset );
    // get current select item index
    curSelection = GUILIST_GetCurItemIndex( listCtrlId );
    GUILIST_RemoveAllItems( listCtrlId );

    //about
    MMI_GetLabelTextByLang( TXT_SET_SYSTEM_ABOUT, &itemStr );
    AdultWatchCOM_ListItem_Draw_1StrLeftAlign( listCtrlId, itemStr );

    //connect new phone
    MMI_GetLabelTextByLang( TXT_SET_SYSTEM_CONNECT_NEW_PHONE, &itemStr );
    AdultWatchCOM_ListItem_Draw_1StrLeftAlign( listCtrlId, itemStr );

    //power reset
    MMI_GetLabelTextByLang( TXT_SET_SYSTEM_POWER_RESET, &itemStr );
    AdultWatchCOM_ListItem_Draw_1StrLeftAlign( listCtrlId, itemStr );

    //power off
    MMI_GetLabelTextByLang( TXT_SET_SYSTEM_POWER_OFF, &itemStr );
    AdultWatchCOM_ListItem_Draw_1StrLeftAlign( listCtrlId, itemStr );

    //factory reset
    MMI_GetLabelTextByLang( TXT_SET_SYSTEM_FACTORY_RESET,  &itemStr );
    AdultWatchCOM_ListItem_Draw_1StrLeftAlign( listCtrlId, itemStr );

#ifdef UAL_SS_SUPPORT
    //divert
    MMI_GetLabelTextByLang( TXT_SET_CALL_FORWARDING,  &itemStr );
    AdultWatchCOM_ListItem_Draw_1StrLeftAlign( listCtrlId, itemStr );
#endif
    //set current item
    GUILIST_SetCurItemIndex( listCtrlId, curSelection );
    //set top item offset
    GUILIST_SetTopItemOffset( listCtrlId, listOffset );
#ifdef UAL_SS_SUPPORT
    if((TRUE == MMIENG_IsMonkeyTestMode()) || (FALSE == MMIAPIENG_GetCallForwardEnable()))
    {
        GUILIST_SetItemGray(listCtrlId,SETTINGS_SYSTEM_DIVERT,TRUE);//set item ss gray
    }
#endif
}

/*****************************************************************************/
//  Description : to handle Settings window message
//  Global resource dependence :
//  Author:bin.wang1
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingsSystemWindow(
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
            Settings_System_OPEN_WINDOW( win_id );
            break;
        }
        case MSG_FULL_PAINT:
        {
            Settings_System_FULL_PAINT( win_id );
            break;
        }
        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        {
            Settings_System_APP_OK();
            break;
        }
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            Settings_System_APP_CANCEL( win_id );
            break;
        }
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
WINDOW_TABLE( MMISET_SYSTEM_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleSettingsSystemWindow ),
    WIN_ID( MMISET_SYSTEM_WIN_ID ),
    WIN_HIDE_STATUS,
    WIN_TITLE(TXT_SET_LIST_SYSTEM),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SYSTEM_LIST_CTRL_ID),
    END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
// idle -> settings -> system
PUBLIC void Settings_SystemWin_Enter( void )
{
    MMK_CreateWin( (uint32 *)MMISET_SYSTEM_WIN_TAB, PNULL );
}
