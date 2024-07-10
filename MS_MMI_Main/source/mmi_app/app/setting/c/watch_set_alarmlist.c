/*****************************************************************************
** File Name:      watch_set_alarm.c      //alarm settings                   *
** Author:         bin.wang1                                                 *
** Date:           03/10/2020                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:  alarm list window                                           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2020      bin.wang1              Creat                                 *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "guilistbox.h"
#include "watch_common_list.h"
#include "os_api.h"
#include "sci_types.h"
#include "window_parse.h"
#include "mmk_type.h"
#include "guilabel.h"
#include "guibutton.h"
#include "mmidisplay_data.h"

#include "mmicom_trace.h"
#include "mmiset_image.h"
#include "mmiset_text.h"
#include "mmiset_id.h"
#include "mmiset_export.h"
#include "mmiacc_event.h"
#include "mmialarm_export.h"

#include "watch_commonwin_export.h"
#include "watch_set_alarmedit.h"
#include "watch_set_position.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define WATCHSET_ALARM_TEMP_STR_LEN             (32)
#define WATCHSET_ALARM_LIST_ITEM_MAX            (3)

//================================================================================/
//================== settings->alarm list->del confirm win =======================/
//================================================================================/

LOCAL MMI_RESULT_E _Callback_AlarmDelConfirmWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;

    switch(msg_id)
    {

        case MSG_NOTIFY_OK:
        {
            uint32* pEventId = (uint32*)MMK_GetWinAddDataPtr( MMISET_ALARM_OPTION_WIN_ID );

            MMIALARM_DeleteOneAlarm( (uint16)(*pEventId) );
            MMK_CloseWin( win_id );
            MMK_CloseWin( MMISET_ALARM_OPTION_WIN_ID );
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

LOCAL void SettingsAlarmDelConfirmWin_Enter( void )
{
    MMI_STRING_T            queryTipStr = { 0 };
    WATCH_SOFTKEY_TEXT_ID_T softkey = { STXT_OK, PNULL, STXT_RETURN };

    MMI_GetLabelTextByLang( TXT_DELETE_QUERY, &queryTipStr );
    WatchCOM_QueryWin_2Btn_Enter(
                                    MMISET_ALARM_DEL_CONFIRM_WIN_ID,
                                    &queryTipStr,
                                    PNULL,
                                    PNULL,
                                    IMAGE_SET_COM_BTN_FIXED,
                                    IMAGE_SET_COM_BTN_CANCEL,
                                    softkey,
                                    _Callback_AlarmDelConfirmWin
                                );
}

//================================================================================/
//=================== settings->alarm list->alarm option win =====================/
//================================================================================/

typedef enum
{
    WATCHSET_ALARM_OPTION_EDIT,
    WATCHSET_ALARM_OPTION_CLOSE,
    WATCHSET_ALARM_OPTION_DELETE
}
WATCHSET_ALARM_OPTION_ITEM_E;

LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_ST s_alarm_repeat_item_edit         = { TXT_EDIT };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_ST s_alarm_repeat_item_close        = { TXT_COMMON_CLOSE };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_ST s_alarm_repeat_item_delete       = { TXT_DELETE };

LOCAL WATCHCOM_LIST_ITEM__ST s_alarm_option_list[] =
{
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR,   &s_alarm_repeat_item_edit,    PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR,   &s_alarm_repeat_item_close,   PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR,   &s_alarm_repeat_item_delete,  PNULL },
};

LOCAL void Settings_AlarmOption_OPEN_WINDOW( MMI_WIN_ID_T win_id )
{
    MMI_CTRL_ID_T   listCtrlId = MMISET_ALARM_OPTION_LIST_CTRL_ID;

    uint32 lisNum = sizeof(s_alarm_option_list)/sizeof(WATCHCOM_LIST_ITEM__ST);

    WatchCOM_TextList_Create( s_alarm_option_list, lisNum, listCtrlId );
    MMK_SetAtvCtrl( win_id, listCtrlId );
}

LOCAL void Settings_AlarmOption_APP_OK( MMI_WIN_ID_T win_id )
{
    uint16          index = 0;
    uint32*         pEventId = PNULL;
    MMI_CTRL_ID_T   listCtrlId = MMISET_ALARM_OPTION_LIST_CTRL_ID;

    index = GUILIST_GetCurItemIndex( listCtrlId );
    pEventId = (uint32*)MMK_GetWinAddDataPtr( win_id );
    if( PNULL == pEventId )
    {
        TRACE_APP_SETTINGS("pEventId is PNULL!");
        return;
    }
    switch( index )
    {
        case WATCHSET_ALARM_OPTION_EDIT:
        {
            Settings_AlarmEditWin_Enter( *pEventId );
            break;
        }

        case WATCHSET_ALARM_OPTION_CLOSE:
        {
            MMIALARM_CloseOneAlarm( (uint16)(*pEventId) );
            MMK_CloseWin( win_id );
            break;
        }

        case WATCHSET_ALARM_OPTION_DELETE:
        {
            SettingsAlarmDelConfirmWin_Enter();
            break;
        }

        default:
            TRACE_APP_SETTINGS("index error %d", index);
            break;
    }
}

LOCAL void Settings_AlarmOption_APP_CANCEL( MMI_WIN_ID_T win_id )
{
    MMK_CloseWin( win_id );
}

LOCAL void Settings_AlarmOption_CLOSE_WINDOW( MMI_WIN_ID_T win_id )
{
    MMK_FreeWinAddData( win_id );
}

LOCAL MMI_RESULT_E  HandleSettingsAlarmOptionWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E reCode =  MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            Settings_AlarmOption_OPEN_WINDOW( win_id );
            break;
        }

        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        {
            Settings_AlarmOption_APP_OK( win_id );
            break;
        }

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            Settings_AlarmOption_APP_CANCEL( win_id );
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            Settings_AlarmOption_CLOSE_WINDOW( win_id );
            break;
        }

        default:
            reCode = MMI_RESULT_FALSE;
            break;
    }

    return reCode;
}

//idle -> settings -> alarm list-> modify/close/delete option window
WINDOW_TABLE( MMISET_ALARM_OPTION_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleSettingsAlarmOptionWindow ),
    WIN_ID( MMISET_ALARM_OPTION_WIN_ID ),
    WIN_HIDE_STATUS,
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_ALARM_OPTION_LIST_CTRL_ID ),
    WIN_SOFTKEY( STXT_OK, TXT_NULL, STXT_RETURN ),
    END_WIN
};

/*****************************************************************************/
//  Discription: enter setttings->alarm list->option window                  *
//  Global resource dependence:                                              *
//  Author: bin.wang1                                                        *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC void Settings_AlarmOptionWin_Enter( uint32 event_id )
{
    uint32* pWinData = (uint32*)SCI_ALLOC_APPZ( sizeof(uint32) );

    if( PNULL == pWinData )
    {
        TRACE_APP_SETTINGS("pWinData is PNULL!!");
        return;
    }

    *pWinData = event_id;
    MMK_CreateWin( (uint32 *)MMISET_ALARM_OPTION_WIN_TAB, (ADD_DATA)pWinData );
}

//================================================================================/
//======================== settings->alarm list win ==============================/
//================================================================================/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : convert 24 hour to 12 hour
//  Global resource dependence :
//  Return: TRUE AM, FALSE PM
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SettingsConver24HourTo12Hour( uint8 *hour_ptr )
{
    BOOLEAN result = FALSE;

    if( PNULL == hour_ptr )
    {
        TRACE_APP_SETTINGS("hour_ptr is PNULL!");
    }

    if( (12 > *hour_ptr) ) //AM
    {
        result = TRUE;

        if( (1 > *hour_ptr) ) //00:00-00:59
        {
            *hour_ptr = (uint8)(*hour_ptr+12);
        }
    }
    else //PM
    {
        if( 12 == *hour_ptr ) //12:00-12:59
        {
        }
        else
        {
            *hour_ptr = (uint8)(*hour_ptr-12);
        }
    }

    return result;
}

LOCAL BOOLEAN SettingsAlarmListGetTimeStr( SCI_TIME_T* sys_time_ptr, MMI_STRING_T* time_str_ptr )
{
    BOOLEAN     isAM = FALSE;
    uint8       timeStr[WATCHSET_ALARM_TEMP_STR_LEN] = { 0 };

    if( PNULL == time_str_ptr || PNULL == sys_time_ptr )
    {
        TRACE_APP_SETTINGS("input is PNULL!");
        return FALSE;
    }
    if( MMISET_TIME_12HOURS == MMIAPISET_GetTimeDisplayType() )
    {
        isAM = SettingsConver24HourTo12Hour( &(sys_time_ptr->hour) );
        if( TRUE == isAM )
        {
            sprintf( (char*)timeStr," %02d:%02d AM", sys_time_ptr->hour, sys_time_ptr->min );
        }
        else
        {
            sprintf( (char*)timeStr," %02d:%02d PM", sys_time_ptr->hour, sys_time_ptr->min );
        }
    }
    else
    {
        sprintf( (char*)timeStr," %02d:%02d ", sys_time_ptr->hour, sys_time_ptr->min );
    }
    MMIAPICOM_StrToWstr( timeStr, time_str_ptr->wstr_ptr );
    time_str_ptr->wstr_len = MMIAPICOM_Wstrlen( time_str_ptr->wstr_ptr );
    return TRUE;
}

LOCAL void SettingsAlarmListDrawList( MMI_WIN_ID_T win_id, MMI_CTRL_ID_T list_ctr_id, uint16 event_num )
{
    if( 0 == event_num )
    {
        //hide list
        GUI_RECT_T  emptyListRect = { 0, 0, 0, 0 };

        GUILIST_SetRect( list_ctr_id, &emptyListRect );
        GUILIST_RemoveAllItems( list_ctr_id );
    }
    else
    {
        uint16      index = 0;
        uint16      itemIndex = 0;
        uint16      itemNum = 0;
        uint16      curSelection = 0;
        GUI_RECT_T  listRect = { WATCH_SET_ALARM_LIST_X, WATCH_SET_ALARM_LIST_X, WATCH_SET_ALARM_LIST_X+WATCH_SET_ALARM_LIST_W, WATCH_SET_ALARM_LIST_Y+WATCH_SET_ALARM_LIST_H };

        if( ALM_ALARM_NUM == event_num  )
        {
            listRect.bottom += WATCH_SET_ALARM_LIST_ITEM_H; // display three item
        }

        curSelection = GUILIST_GetCurItemIndex( list_ctr_id );

        GUILIST_RemoveAllItems( list_ctr_id );
        GUILIST_SetRect( list_ctr_id, &listRect );

        for ( index = 0; index < ALM_ALARM_NUM; index++ )
        {
            MMI_IMAGE_ID_T                          iconId = 0;
            SCI_TIME_T                              alarmTime = { 0 };
            MMI_STRING_T                            timeStr = { 0 };
            wchar                                   wTimeStr[WATCHSET_ALARM_TEMP_STR_LEN] = { 0 };
            MMI_STRING_T                            repeatStr = { 0 };
            uint16                                  eventId = EVENT_ALARM_0 + index;
            uint32                                  userData = 0;
            const MMIACC_SMART_EVENT_FAST_TABLE_T*  pEventTable = PNULL;

            pEventTable = MMIALM_GeEvent( eventId );
            if( PNULL == pEventTable )
            {
                TRACE_APP_SETTINGS("pEventTable is PNULL");
                return;
            }
            if( FALSE == pEventTable->is_valid )
            {
                continue;
            }

            //time string
            timeStr.wstr_ptr    = wTimeStr;
            alarmTime.hour      = pEventTable->hour;
            alarmTime.min       = pEventTable->minute;
            SettingsAlarmListGetTimeStr( &alarmTime, &timeStr );

            //repeat string
            if( ALM_MODE_ONCE == pEventTable->fre_mode )
            {
                MMI_GetLabelTextByLang( TXT_SET_ALARM_NO_REPEAT, &repeatStr );
            }
            else
            {
                MMI_GetLabelTextByLang( TXT_SET_ALARM_REPEAT, &repeatStr );
            }

            //on/off icon
            if( TRUE == pEventTable->is_on )
            {
                iconId = IMAGE_SET_ALARM_ON;
            }
            else
            {
                iconId = IMAGE_SET_ALARM_OFF;
            }

            WatchCOM_ListItem_Draw_1Icon_2Str( list_ctr_id, iconId, timeStr, repeatStr );
            userData = (uint32)eventId;
            GUILIST_SetItemUserData( list_ctr_id, itemIndex, &userData );
            itemIndex++;
        }

        itemNum = GUILIST_GetTotalItemNum(list_ctr_id);
        if( curSelection >= itemNum )
        {
            curSelection = itemNum-1;
        }
        GUILIST_SetCurItemIndex( list_ctr_id, curSelection );
    }
}

LOCAL void SettingsAlarmListSetIconLabel( uint16 event_num )
{
    MMI_CTRL_ID_T   ctrlIcon = MMISET_NO_ALARM_ICON_LABEL_CTRL_ID;
    MMI_CTRL_ID_T   ctrlLabel = MMISET_NO_ALARM_INFO_LABEL_CTRL_ID;

    if( 0 == event_num )
    {
        GUILABEL_SetVisible( ctrlIcon, TRUE, FALSE );
        GUILABEL_SetVisible( ctrlLabel, TRUE, FALSE );
    }
    else
    {
        GUILABEL_SetVisible( ctrlIcon, FALSE, FALSE );
        GUILABEL_SetVisible( ctrlLabel, FALSE, FALSE );
    }
}

LOCAL MMI_RESULT_E _Callback_AlarmListBtnAdd( void )
{
    Settings_AlarmAddWin_Enter();
    return MMI_RESULT_TRUE;
}

LOCAL void SettingsAlarmListSetButton( uint16 event_num )
{
    BOOLEAN         reCode = TRUE;
    MMI_CTRL_ID_T   addBtn = MMISET_ADD_ALARM_BTN_CTRL_ID;

    if( event_num >= ALM_ALARM_NUM )
    {
        reCode = GUIBUTTON_SetVisible( addBtn, FALSE, FALSE );
    }
    else
    {
        reCode = GUIBUTTON_SetVisible( addBtn, TRUE, FALSE );
    }
    if( TRUE != reCode )
    {
        TRACE_APP_SETTINGS("set btn callback error event_num %d", event_num);
    }
}

LOCAL uint16 SettingsAlarmListGetAlarmNum( void )
{
    uint16      index = 0;
    uint16      alarmNum = 0;

    for( index = 0; index < ALM_ALARM_NUM; index++ )
    {
        const MMIACC_SMART_EVENT_FAST_TABLE_T* pEventTable = PNULL;

        pEventTable = MMIALM_GeEvent( (uint16)(EVENT_ALARM_0+index) );
        if( PNULL == pEventTable )
        {
            TRACE_APP_SETTINGS("pEventTable is PNULL");
            return alarmNum;
        }
        if( TRUE == pEventTable->is_valid )
        {
            alarmNum++;
        }
    }
    return alarmNum;
}

LOCAL void Settings_AlarmList_OPEN_WINDOW( MMI_WIN_ID_T win_id )
{
    GUI_RECT_T      iconRect = { WATCH_SET_ALARM_NO_ALRRM_ICON_X, WATCH_SET_ALARM_NO_ALRRM_ICON_Y, WATCH_SET_ALARM_NO_ALRRM_ICON_X+WATCH_SET_ALARM_NO_ALRRM_ICON_W, WATCH_SET_ALARM_NO_ALRRM_ICON_Y+WATCH_SET_ALARM_NO_ALRRM_ICON_H };
    MMI_CTRL_ID_T   ctrlIcon = MMISET_NO_ALARM_ICON_LABEL_CTRL_ID;

    GUI_RECT_T      labelRect =  { WATCH_SET_ALARM_NO_ALRRM_LABEL_X, WATCH_SET_ALARM_NO_ALRRM_LABEL_Y, WATCH_SET_ALARM_NO_ALRRM_LABEL_X+WATCH_SET_ALARM_NO_ALRRM_LABEL_W, WATCH_SET_ALARM_NO_ALRRM_LABEL_Y+WATCH_SET_ALARM_NO_ALRRM_LABEL_H };
    MMI_CTRL_ID_T   ctrlLabel = MMISET_NO_ALARM_INFO_LABEL_CTRL_ID;
    MMI_CTRL_ID_T   listCtrlId = MMISET_ALARM_LIST_CTRL_ID;
    MMI_CTRL_ID_T   addBtn = MMISET_ADD_ALARM_BTN_CTRL_ID;

    BOOLEAN         reCode = TRUE;
    uint16          eventNum = SettingsAlarmListGetAlarmNum();

    //no alarm icon
    GUILABEL_SetRect( ctrlIcon, &iconRect, FALSE );
    GUILABEL_SetIcon( ctrlIcon, IMAGE_SET_NO_ALARM );

    //no alarm label
    GUILABEL_SetRect( ctrlLabel, &labelRect, FALSE );
    GUILABEL_SetFont( ctrlLabel, WATCH_DEFAULT_SMALL_FONT, MMI_WHITE_COLOR );
    GUILABEL_SetTextById( ctrlLabel, TXT_SET_ALARM_ADD_ONE, FALSE );

    //alarm list
    GUILIST_SetMaxItem( listCtrlId, WATCHSET_ALARM_LIST_ITEM_MAX, FALSE );
    MMK_SetAtvCtrl( win_id, listCtrlId );

    //button
    WatchCOM_Btn_SetPos_1Btn( addBtn );
    reCode= GUIBUTTON_SetCallBackFunc( addBtn, _Callback_AlarmListBtnAdd );
    if( TRUE != reCode )
    {
        TRACE_APP_SETTINGS("set addBtn callback error");
    }

}

LOCAL void Settings_AlarmList_FULL_PAINT( MMI_WIN_ID_T win_id )
{
    uint16 eventNum = SettingsAlarmListGetAlarmNum();

    //bg color
    WATCHCOM_DisplayBackground( win_id );
    //icon & label
    SettingsAlarmListSetIconLabel( eventNum );
    //list
    SettingsAlarmListDrawList( win_id, MMISET_ALARM_LIST_CTRL_ID, eventNum );
    //button
    SettingsAlarmListSetButton( eventNum );
}

LOCAL void Settings_AlarmList_APP_OK( MMI_WIN_ID_T win_id )
{
    uint16          index = 0;
    uint32          eventId = 0;
    BOOLEAN         reCode = FALSE;
    MMI_CTRL_ID_T   listCtrlId = MMISET_ALARM_LIST_CTRL_ID;

    uint16 eventNum = SettingsAlarmListGetAlarmNum();
    if( 0 == eventNum )
    {
        Settings_AlarmAddWin_Enter();
        return;
    }

    index = GUILIST_GetCurItemIndex( listCtrlId );
    reCode = GUILIST_GetItemData( listCtrlId, index, &eventId );
    if( TRUE == reCode )
    {
        Settings_AlarmOptionWin_Enter( eventId );
    }
    else
    {
        TRACE_APP_SETTINGS("GUILIST_GetItemData error!");
    }
}

LOCAL void Settings_AlarmList_APP_CANCEL( MMI_WIN_ID_T win_id )
{
    MMK_CloseWin( win_id );
}

LOCAL MMI_RESULT_E  HandleSettingsAlarmListWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E reCode =  MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            Settings_AlarmList_OPEN_WINDOW( win_id );
            break;
        }

        case MSG_FULL_PAINT:
        {
            Settings_AlarmList_FULL_PAINT( win_id );
            break;
        }

        case MSG_CTL_OK:
        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        {
            Settings_AlarmList_APP_OK( win_id );
            break;
        }

        case MSG_CTL_MIDSK: // just used this key to test add button action Bug 1323589
        case MSG_APP_WEB:
        {
            uint16 eventNum = SettingsAlarmListGetAlarmNum();

            if( eventNum < ALM_ALARM_NUM )
            {
                Settings_AlarmAddWin_Enter();
            }
            else
            {
                TRACE_APP_SETTINGS("eventNum %d", eventNum);
            }

            break;
        }

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            Settings_AlarmList_APP_CANCEL( win_id );
            break;
        }

        default:
            reCode = MMI_RESULT_FALSE;
            break;
    }

    return reCode;
}

//idle -> settings -> alarm list
WINDOW_TABLE( MMISET_ALARM_LIST_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleSettingsAlarmListWindow ),
    WIN_ID( MMISET_ALARM_LIST_WIN_ID ),
    WIN_HIDE_STATUS,
    CREATE_LABEL_CTRL( GUILABEL_ALIGN_MIDDLE, MMISET_NO_ALARM_ICON_LABEL_CTRL_ID ),
    CREATE_LABEL_CTRL( GUILABEL_ALIGN_MIDDLE, MMISET_NO_ALARM_INFO_LABEL_CTRL_ID ),
    CREATE_LISTBOX_CTRL( GUILIST_TEXTLIST_E, MMISET_ALARM_LIST_CTRL_ID ),
    CREATE_BUTTON_CTRL( IMAGE_SET_COM_BTN_INCREASE, MMISET_ADD_ALARM_BTN_CTRL_ID ),
    WIN_SOFTKEY( STXT_OK, TXT_NULL, STXT_RETURN ),
    END_WIN
};

/*****************************************************************************/
//  Discription: enter setttings->alarm list window                          *
//  Global resource dependence:                                              *
//  Author: bin.wang1                                                        *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC void Settings_AlarmListWin_Enter( void )
{
    MMK_CreateWin( (uint32 *)MMISET_ALARM_LIST_WIN_TAB, PNULL );
}

