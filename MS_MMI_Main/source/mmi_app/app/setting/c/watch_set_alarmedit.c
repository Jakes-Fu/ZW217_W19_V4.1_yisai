/*****************************************************************************
** File Name:      watch_set_alarmedit.c                                     *
** Author:         bin.wang1                                                 *
** Date:           03/17/2020                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:  Alarm add&edit window                                       *
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
#include "guilabel.h"
#include "guibutton.h"
#include "guictrl_api.h"
#include "guiedit.h"
#include "mmidisplay_data.h"

#include "mmicom_trace.h"
#include "watch_common_btn.h"
#include "mmiset_image.h"
#include "mmiset_text.h"
#include "mmiset_id.h"
#include "mmiset_export.h"
#include "mmiacc_event.h"
#include "mmialarm_export.h"

#include "watch_common.h"
#include "watch_set_alarmedit.h"
#include "watch_set_position.h"

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

typedef enum
{
    SETTINGS_ALARM_ADD,
    SETTINGS_ALARM_EDIT
}
SETTINGS_ALARM_EDIT_TYPE_E;

//================================================================================/
//========== settings->alarm list->alarm add or edit->set repeat win =============/
//================================================================================/

#define SETTINGS_ALARM_REPEAT_LIST_MAX_ITEM             (8)
#define SETTINGS_ALARM_REPEAT_LIST_ITEM_ONCE_INDEX      (0)
#define SETTINGS_ALARM_REPEAT_LIST_ITEM_SUNDAY_INDEX    (1)
#define SETTINGS_ALARM_REPEAT_LIST_ITEM_SATURDAY_INDEX  (7)

typedef struct
{
    uint32                      eventId;
    SETTINGS_ALARM_EDIT_TYPE_E  type;
    SCI_TIME_T                  time;
}
SETTINGS_ALARM_REPEAT_WIN_T;

LOCAL uint8 s_alarm_repeat_week_mask[ ALM_MODE_WEEK_NUM ] = { MASK_B0, MASK_B1, MASK_B2, MASK_B3, MASK_B4, MASK_B5, MASK_B6 };

LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST s_alarm_repeat_item_once       = { TXT_ALARM_MODE_ONCE };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST s_alarm_repeat_item_sunday     = { TXT_SUNDAY };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST s_alarm_repeat_item_mondey     = { TXT_MONDAY };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST s_alarm_repeat_item_tuesday    = { TXT_TUESDAY };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST s_alarm_repeat_item_wednesday  = { TXT_WEDNESDAY };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST s_alarm_repeat_item_thursday   = { TXT_THURSDAY };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST s_alarm_repeat_item_friday     = { TXT_FRIDAY };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST s_alarm_repeat_item_saturday   = { TXT_SATURDAY };

LOCAL WATCHCOM_LIST_ITEM__ST s_alarm_repeat_list[] =
{
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_CHECK,   &s_alarm_repeat_item_once,      PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_CHECK,   &s_alarm_repeat_item_sunday,    PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_CHECK,   &s_alarm_repeat_item_mondey,    PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_CHECK,   &s_alarm_repeat_item_tuesday,   PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_CHECK,   &s_alarm_repeat_item_wednesday, PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_CHECK,   &s_alarm_repeat_item_thursday,  PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_CHECK,   &s_alarm_repeat_item_friday,    PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_CHECK,   &s_alarm_repeat_item_saturday,  PNULL },
};

// weekday index and ui list index mapping
LOCAL uint16 SettingsAlarmRepeatGetUIListIndex( uint16 weekday_index )
{
    if( weekday_index > ALM_MODE_WEEK_NUM )
    {
        TRACE_APP_SETTINGS("weekday_index error!!");
        return SETTINGS_ALARM_REPEAT_LIST_ITEM_SUNDAY_INDEX;
    }
    return  weekday_index+1;
}

LOCAL uint16 SettingsAlarmRepeatGetWeekdayIndex( uint16 uilist_index )
{
    if( SETTINGS_ALARM_REPEAT_LIST_ITEM_ONCE_INDEX == uilist_index || uilist_index > SETTINGS_ALARM_REPEAT_LIST_ITEM_SATURDAY_INDEX )
    {
        TRACE_APP_SETTINGS("uilist_index error!!");
        return (SETTINGS_ALARM_REPEAT_LIST_ITEM_SUNDAY_INDEX-1);
    }
    return (uilist_index-1);
}

LOCAL MMI_RESULT_E _Callback_AlarmRepeatBtnCancel( void )
{
    MMK_CloseWin( MMISET_ALARM_REPEAT_WIN_ID );
    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E _Callback_AlarmRepeatBtnFixed( void )
{
    uint16                                  selectedNum = 0;
    uint16                                  eventId = 0;
    uint16                                  selectedIndex[SETTINGS_ALARM_REPEAT_LIST_MAX_ITEM] = { 0 };
    MMI_WIN_ID_T                            winId = MMISET_ALARM_REPEAT_WIN_ID;
    MMI_CTRL_ID_T                           listCtrlId = MMISET_ALARM_REPEAT_LIST_CTRL_ID;
    SCI_DATE_T                              dateValue = { 0 };
    SETTINGS_ALARM_REPEAT_WIN_T*            pWinData = PNULL;
    MMIACC_SMART_EVENT_FAST_TABLE_T*        pEventTable = PNULL;
    const GUILIST_ITEM_T*                   pListItem = PNULL;

    pWinData = (SETTINGS_ALARM_REPEAT_WIN_T*)MMK_GetWinAddDataPtr( MMISET_ALARM_REPEAT_WIN_ID );
    if( PNULL == pWinData )
    {
        return MMI_RESULT_FALSE;
    }

    //get eventId
    if( SETTINGS_ALARM_ADD == pWinData->type )
    {
        BOOLEAN reCode = FALSE;
        reCode = MMIALARM_GetOneUnusedAlarm( &eventId );
        if( FALSE == reCode )
        {
            TRACE_APP_SETTINGS("MMIALARM_GetOneUnusedAlarm fail!!");
            return MMI_RESULT_FALSE;
        }
    }
    else
    {
        eventId = pWinData->eventId;
    }

    pEventTable = (MMIACC_SMART_EVENT_FAST_TABLE_T*)SCI_ALLOC_APPZ( sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T) );
    if( PNULL == pEventTable )
    {
        return MMI_RESULT_FALSE;
    }

    // date && time
    TM_GetSysDate( &dateValue );

    pEventTable->start_year  = dateValue.year;
    pEventTable->start_mon   = dateValue.mon;
    pEventTable->start_day   = dateValue.mday;

    pEventTable->year        = dateValue.year;
    pEventTable->mon         = dateValue.mon;
    pEventTable->day         = dateValue.mday;
    pEventTable->hour        = pWinData->time.hour;
    pEventTable->minute      = pWinData->time.min;

    pEventTable->is_on       = TRUE;
    pEventTable->is_valid    = TRUE;

    // repeat
    selectedNum = GUILIST_GetSelectedItemNum( listCtrlId );
    if( 0 == selectedNum )
    {
        TRACE_APP_SETTINGS("Please select one at least");
        return MMI_RESULT_FALSE;
    }

    pListItem = GUILIST_GetItemPtrByIndex( listCtrlId, SETTINGS_ALARM_REPEAT_LIST_ITEM_ONCE_INDEX );
    if( PNULL == pListItem )
    {
        TRACE_APP_SETTINGS("pListItem is PNULL!!");
        return MMI_RESULT_FALSE;
    }
    if( TRUE == GUILIST_GetItemState(pListItem, GUIITEM_STATE_SELECTED) )
    {
        pEventTable->fre_mode   = ALM_MODE_ONCE;
        pEventTable->fre        = 0;
    }
    else
    {
        uint16 index = SETTINGS_ALARM_REPEAT_LIST_ITEM_SUNDAY_INDEX;
        pEventTable->fre_mode   = ALM_MODE_EVERYWEEK;
        pEventTable->fre        = 0;
        for( index = SETTINGS_ALARM_REPEAT_LIST_ITEM_SUNDAY_INDEX; index < SETTINGS_ALARM_REPEAT_LIST_MAX_ITEM; index++ )
        {
            pListItem = GUILIST_GetItemPtrByIndex( listCtrlId, index );
            if( PNULL == pListItem )
            {
                TRACE_APP_SETTINGS("pListItem is PNULL!!");
                return MMI_RESULT_FALSE;
            }
            if( TRUE == GUILIST_GetItemState(pListItem, GUIITEM_STATE_SELECTED) )
            {
                uint16 weekdayIndex = SettingsAlarmRepeatGetWeekdayIndex( index );
                pEventTable->fre |= s_alarm_repeat_week_mask[ weekdayIndex ];
            }
        }
    }

    MMIALARM_ModifyOneAlarm( eventId, pEventTable );

    SCI_FREE( pEventTable );

    MMK_CloseWin( winId );
    MMK_CloseWin( MMISET_ALARM_EDIT_WIN_ID );
    MMK_CloseWin( MMISET_ALARM_OPTION_WIN_ID );
    return MMI_RESULT_TRUE;
}

LOCAL void Settings_AlarmRepeat_OPEN_WINDOW( MMI_WIN_ID_T win_id )
{
    MMI_CTRL_ID_T   listCtrlId = MMISET_ALARM_REPEAT_LIST_CTRL_ID;
    GUI_RECT_T      listRect = { WATCH_SET_ALARM_LIST_X, WATCH_SET_ALARM_LIST_X, WATCH_SET_ALARM_LIST_X+WATCH_SET_ALARM_LIST_W, WATCH_SET_ALARM_LIST_Y+WATCH_SET_ALARM_LIST_H };

    BOOLEAN         reCode = FALSE;
    MMI_CTRL_ID_T   cancelBtn = MMISET_ALARM_REPEAT_CANCEL_BTN_CTRL_ID;
    MMI_CTRL_ID_T   fixedBtn = MMISET_ALARM_REPEAT_FIXED_BTN_CTRL_ID;

    //create list
    SETTINGS_ALARM_REPEAT_WIN_T*            pWinData = PNULL;
    MMIACC_SMART_EVENT_FAST_TABLE_T*        pEventTable = PNULL;

    GUILIST_SetRect( listCtrlId, &listRect );
    WatchCOM_CheckList_Create( s_alarm_repeat_list, SETTINGS_ALARM_REPEAT_LIST_MAX_ITEM, listCtrlId );
    GUILIST_SetCurItemIndex( listCtrlId, 0 );
    GUILIST_ChangeCheckListDefSoftkeyID( listCtrlId, TXT_NULL, TXT_NULL, TXT_NULL );
    GUILIST_SetOwnSofterKey( listCtrlId, TRUE );

    //set selected items
    pWinData = (SETTINGS_ALARM_REPEAT_WIN_T*)MMK_GetWinAddDataPtr( MMISET_ALARM_REPEAT_WIN_ID );
    if( PNULL == pWinData )
    {
        TRACE_APP_SETTINGS("pWinData is PNULL!!");
        return;
    }

    if( SETTINGS_ALARM_ADD == pWinData->type )
    {
        GUILIST_SetSelectedItem( listCtrlId, SETTINGS_ALARM_REPEAT_LIST_ITEM_ONCE_INDEX, TRUE ); // once selected
    }
    else
    {
        pEventTable = MMIALM_GeEvent( pWinData->eventId );
        if( ALM_MODE_EVERYWEEK == pEventTable->fre_mode )
        {
            uint16 index = 0;   // sunday

            for( index = 0; index <= ALM_MODE_WEEK_NUM; index++ )
            {
                if( s_alarm_repeat_week_mask[index] & pEventTable->fre )
                {
                    uint16 uiListIndex = SettingsAlarmRepeatGetUIListIndex( index );
                    GUILIST_SetSelectedItem( listCtrlId, uiListIndex, TRUE );
                }
            }
        }
        else
        {
            GUILIST_SetSelectedItem( listCtrlId, SETTINGS_ALARM_REPEAT_LIST_ITEM_ONCE_INDEX, TRUE ); // once selected
        }
    }

    //button
    WatchCOM_Btn_SetPos_2Btn( cancelBtn, fixedBtn );//Bug - 1998774
    reCode = GUIBUTTON_SetCallBackFunc( fixedBtn, _Callback_AlarmRepeatBtnFixed );
    if( reCode != TRUE )
    {
        TRACE_APP_SETTINGS("GUIBUTTON_SetCallBackFunc fixedBtn set error");
    }
    reCode = GUIBUTTON_SetCallBackFunc( cancelBtn, _Callback_AlarmRepeatBtnCancel );
    if( reCode != TRUE )
    {
        TRACE_APP_SETTINGS("GUIBUTTON_SetCallBackFunc cancelBtn set error");
    }

    MMK_SetAtvCtrl( win_id, listCtrlId );
}

LOCAL void Settings_AlarmRepeat_FULL_PAINT( MMI_WIN_ID_T win_id )
{
    //bg color
    WATCHCOM_DisplayBackground( win_id );
}

LOCAL void Settings_AlarmRepeat_APP_OK( void )
{
    _Callback_AlarmRepeatBtnFixed();
}

LOCAL void Settings_AlarmRepeat_APP_WEB( void )
{
    BOOLEAN                 reCode = FALSE;
    BOOLEAN                 isCurItemSelected = FALSE;
    uint16                  curIndex = 0;
    uint16                  index = 0;
    MMI_WIN_ID_T            winId = MMISET_ALARM_REPEAT_WIN_ID;
    MMI_CTRL_ID_T           listCtrlId = MMISET_ALARM_REPEAT_LIST_CTRL_ID;
    const GUILIST_ITEM_T*   pListItem = PNULL;

    curIndex = GUILIST_GetCurItemIndex( listCtrlId );
    pListItem = GUILIST_GetItemPtrByIndex( listCtrlId, curIndex );

    isCurItemSelected = (GUIITEM_STATE_SELECTED == pListItem->item_state) ? TRUE : FALSE;

    // check mutex logic
    if( SETTINGS_ALARM_REPEAT_LIST_ITEM_ONCE_INDEX == curIndex )  // once item selected
    {
        if( FALSE == isCurItemSelected ) // once item will change to selected, so change other item
        {
            for( index = SETTINGS_ALARM_REPEAT_LIST_ITEM_SUNDAY_INDEX; index < SETTINGS_ALARM_REPEAT_LIST_MAX_ITEM; index++ )
            {
                reCode = GUILIST_SetSelectedItem( listCtrlId, index, FALSE );
                if( FALSE == reCode )
                {
                    TRACE_APP_SETTINGS("GUILIST_SetSelectedItem other item set FALSE fail");
                }
            }
        }
    }
    else
    {
        if( FALSE == isCurItemSelected )
        {
            pListItem = GUILIST_GetItemPtrByIndex( listCtrlId, SETTINGS_ALARM_REPEAT_LIST_ITEM_ONCE_INDEX );
            if( GUIITEM_STATE_SELECTED == pListItem->item_state )
            {
                reCode = GUILIST_SetSelectedItem( listCtrlId, SETTINGS_ALARM_REPEAT_LIST_ITEM_ONCE_INDEX, FALSE );
                if( FALSE == reCode )
                {
                    TRACE_APP_SETTINGS("GUILIST_SetSelectedItem once item set FLASE fail");
                }
            }
        }
    }

    // change current item state
    if( TRUE == isCurItemSelected )
    {
        reCode = GUILIST_SetSelectedItem( listCtrlId, curIndex, FALSE );
        if( FALSE == reCode )
        {
            TRACE_APP_SETTINGS("GUILIST_SetSelectedItem set curIndex FALSE fail");
        }
    }
    else
    {
        reCode = GUILIST_SetSelectedItem( listCtrlId, curIndex, TRUE );
        if( FALSE == reCode )
        {
            TRACE_APP_SETTINGS("GUILIST_SetSelectedItem set curIndex TRUE fail");
        }
    }

    MMK_SendMsg( winId, MSG_FULL_PAINT, PNULL );
}

LOCAL void Settings_AlarmRepeat_APP_CANCEL( MMI_WIN_ID_T win_id )
{
    MMK_CloseWin( win_id );
}

LOCAL void Settings_AlarmRepeat_CLOSE_WINDOW( MMI_WIN_ID_T win_id )
{
    MMK_FreeWinAddData( win_id );
}

LOCAL MMI_RESULT_E  HandleSettingsAlarmRepeatWindow(
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
            Settings_AlarmRepeat_OPEN_WINDOW( win_id );
            break;
        }
        break;

        case MSG_FULL_PAINT:
        {
            Settings_AlarmRepeat_FULL_PAINT( win_id );
            break;
        }

        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        {
            Settings_AlarmRepeat_APP_WEB();
            break;
        }

        case MSG_APP_OK:
        case MSG_CTL_OK:
        {
            Settings_AlarmRepeat_APP_OK();
            break;
        }

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            Settings_AlarmRepeat_APP_CANCEL( win_id );
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            Settings_AlarmRepeat_CLOSE_WINDOW( win_id );
            break;
        }

        default:
            reCode = MMI_RESULT_FALSE;
            break;
    }

    return reCode;
}

//idle -> settings -> alarm add or edit->set repeat window
WINDOW_TABLE( MMISET_ALARM_REPEAT_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleSettingsAlarmRepeatWindow ),
    WIN_ID( MMISET_ALARM_REPEAT_WIN_ID ),
    WIN_HIDE_STATUS,
    CREATE_LISTBOX_CTRL( GUILIST_CHECKLIST_E, MMISET_ALARM_REPEAT_LIST_CTRL_ID ),
    CREATE_BUTTON_CTRL( IMAGE_SET_COM_BTN_CANCEL, MMISET_ALARM_REPEAT_CANCEL_BTN_CTRL_ID ),
    CREATE_BUTTON_CTRL( IMAGE_SET_COM_BTN_FIXED, MMISET_ALARM_REPEAT_FIXED_BTN_CTRL_ID ),
    WIN_SOFTKEY( STXT_OK, TXT_NULL, STXT_RETURN ),
    END_WIN
};

/*****************************************************************************/
//  Discription: enter setttings->alarm list->alarm add/edit->repeat window  *
//  Global resource dependence:                                              *
//  Author: bin.wang1                                                        *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC void Settings_AlarmRepeatWin_Enter( SETTINGS_ALARM_REPEAT_WIN_T* win_data_ptr )
{
    SETTINGS_ALARM_REPEAT_WIN_T* pWinData = (SETTINGS_ALARM_REPEAT_WIN_T*)SCI_ALLOC_APPZ( sizeof(SETTINGS_ALARM_REPEAT_WIN_T) );

    pWinData->type      = win_data_ptr->type;
    pWinData->eventId   = win_data_ptr->eventId;
    pWinData->time.hour = win_data_ptr->time.hour;
    pWinData->time.min  = win_data_ptr->time.min;

    MMK_CreateWin( (uint32 *)MMISET_ALARM_REPEAT_WIN_TAB, (ADD_DATA)pWinData );
}


//================================================================================/
//================= settings->alarm list->alarm add or edit win ==================/
//================================================================================/

typedef struct
{
    uint32                      eventId;
    SETTINGS_ALARM_EDIT_TYPE_E  type;
}
SETTINGS_ALARM_EDIT_WIN_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

LOCAL MMI_RESULT_E _Callback_AlarmEditBtnCancel( void )
{
    MMK_CloseWin( MMISET_ALARM_EDIT_WIN_ID );
    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E _Callback_AlarmEditBtnFixed( void )
{
    uint8                           hour = 0;
    uint8                           minute = 0;
    uint8                           second = 0;
    SETTINGS_ALARM_REPEAT_WIN_T     repeatWin = { 0 };

    SETTINGS_ALARM_EDIT_WIN_T*  pEditWin = (SETTINGS_ALARM_EDIT_WIN_T*)MMK_GetWinAddDataPtr( MMISET_ALARM_EDIT_WIN_ID );
    if( PNULL == pEditWin )
    {
        TRACE_APP_SETTINGS("pEditWin is PNULL!");
        return MMI_RESULT_FALSE;
    }

    GUIEDIT_GetTime( MMISET_ALARM_EDIT_TIME_EDIT_CTRL_ID, &hour, &minute, &second );

    repeatWin.eventId   = pEditWin->eventId;
    repeatWin.type      = pEditWin->type;
    repeatWin.time.hour = hour;
    repeatWin.time.min  = minute;

    Settings_AlarmRepeatWin_Enter( &repeatWin );
    return MMI_RESULT_TRUE;
}

LOCAL void Settings_AlarmEdit_OPEN_WINDOW( MMI_WIN_ID_T win_id )
{
    GUI_RECT_T      editRect = { WATCH_SET_ALARM_EDIT_X, WATCH_SET_ALARM_EDIT_Y, WATCH_SET_ALARM_EDIT_X+WATCH_SET_ALARM_EDIT_W, WATCH_SET_ALARM_EDIT_Y+WATCH_SET_ALARM_EDIT_H };
    MMI_CTRL_ID_T   editCtrId = MMISET_ALARM_EDIT_TIME_EDIT_CTRL_ID;

    MMI_CTRL_ID_T   cancelBtn = MMISET_ALARM_EDIT_CANCEL_BTN_CTRL_ID;
    MMI_CTRL_ID_T   fixedBtn  = MMISET_ALARM_EDIT_FIXED_BTN_CTRL_ID;

    BOOLEAN                         reCode = FALSE;
    GUI_FONT_ALL_T                  normalFont = { 0 };
    GUI_FONT_ALL_T                  activeFont = { 0 };
    SCI_TIME_T                      timeValue = { 0 };
    GUIEDIT_TIME_STYLE_E            timeStyle = GUIEDIT_TIME_STYLE_12;
    GUIEDIT_TIME_DISPLAY_STYLE_E    dislayStyle = GUIEDIT_TIME_DISPLAY_STYLE_HM;

    SETTINGS_ALARM_EDIT_WIN_T*      pWinData = PNULL;

    pWinData = (SETTINGS_ALARM_EDIT_WIN_T*)MMK_GetWinAddDataPtr( win_id );
    if( PNULL == pWinData )
    {
        TRACE_APP_SETTINGS("pWinData is PNULL!!");
        return;
    }

    //edit position
    reCode = GUIAPICTRL_SetBothRect( editCtrId, &editRect);
    if( reCode != TRUE )
    {
        TRACE_APP_SETTINGS("GUIAPICTRL_SetBothRect set error");
    }

    //edit tips
    reCode = GUIEDIT_SetTipsPos( editCtrId, GUIEDIT_TIPS_POS_NONE );
    if( reCode != TRUE )
    {
        TRACE_APP_SETTINGS("GUIEDIT_SetTipsPos set error");
    }

    //edit digit font
    normalFont.color = 0x632c;//Bug - 1998774
    normalFont.font = WATCH_DEFAULT_BIG_FONT;
    activeFont.color = MMI_WHITE_COLOR;
    activeFont.font = WATCH_DEFAULT_BIG_FONT;
    GUIEDIT_SetPickerFont( editCtrId, &normalFont, &activeFont);

    //edit item height
    reCode = GUIEDIT_SetPickerLineHeight( editCtrId, WATCH_SET_ALARM_ITEM_H );
    if( reCode != TRUE )
    {
        TRACE_APP_SETTINGS("GUIEDIT_SetPickerLineHeight set error");
    }
    //edit time style
    if( MMISET_TIME_24HOURS == MMIAPISET_GetTimeDisplayType() )
    {
        timeStyle = GUIEDIT_TIME_STYLE_24;
    }
    GUIEDIT_SetTimeStyle( editCtrId, PNULL, &timeStyle, &dislayStyle, FALSE );

    //edit time value
    if( SETTINGS_ALARM_ADD == pWinData->type )
    {
        TM_GetSysTime( &timeValue );
    }
    else
    {
        const MMIACC_SMART_EVENT_FAST_TABLE_T*  pEventTable = PNULL;
        pEventTable = MMIALM_GeEvent( pWinData->eventId );
        timeValue.hour = pEventTable->hour;
        timeValue.min  = pEventTable->minute;
    }
    GUIEDIT_SetTime( editCtrId, timeValue.hour, timeValue.min, 0 );
    MMK_SetAtvCtrl( win_id, editCtrId );

    //button
    WatchCOM_Btn_SetPos_2Btn( cancelBtn, fixedBtn );//Bug - 1998774
    reCode = GUIBUTTON_SetCallBackFunc( fixedBtn, _Callback_AlarmEditBtnFixed );
    if( reCode != TRUE )
    {
        TRACE_APP_SETTINGS("GUIBUTTON_SetCallBackFunc fixedBtn set error");
    }
    reCode = GUIBUTTON_SetCallBackFunc( cancelBtn, _Callback_AlarmEditBtnCancel );
    if( reCode != TRUE )
    {
        TRACE_APP_SETTINGS("GUIBUTTON_SetCallBackFunc cancelBtn set error");
    }
}

LOCAL void Settings_AlarmEdit_FULL_PAINT( MMI_WIN_ID_T win_id )
{
    //bg color
    WATCHCOM_DisplayBackground( win_id );
}

LOCAL void Settings_AlarmEdit_APP_OK( void )
{
    _Callback_AlarmEditBtnFixed();
}

LOCAL void Settings_AlarmEdit_APP_CANCEL( MMI_WIN_ID_T win_id )
{
    MMK_CloseWin( win_id );
}

LOCAL void Settings_AlarmEdit_CLOSE_WINDOW( MMI_WIN_ID_T win_id )
{
    MMK_FreeWinAddData( win_id );
}

LOCAL MMI_RESULT_E  HandleSettingsAlarmEditWindow(
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
            Settings_AlarmEdit_OPEN_WINDOW( win_id );
            break;
        }

        case MSG_FULL_PAINT:
        {
            Settings_AlarmEdit_FULL_PAINT( win_id );
            break;
        }

        case MSG_APP_OK:
        case MSG_CTL_OK:
        {
            Settings_AlarmEdit_APP_OK();
            break;
        }

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            Settings_AlarmEdit_APP_CANCEL( win_id );
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            Settings_AlarmEdit_CLOSE_WINDOW( win_id );
            break;
        }

        default:
            reCode = MMI_RESULT_FALSE;
            break;
    }

    return reCode;
}

//idle -> settings -> alarm add or edit window
WINDOW_TABLE( MMISET_ALARM_EDIT_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleSettingsAlarmEditWindow ),
    WIN_ID( MMISET_ALARM_EDIT_WIN_ID ),
    WIN_HIDE_STATUS,
#ifdef TOUCH_PANEL_SUPPORT
    CREATE_EDIT_TOUCH_TIME_CTRL( MMISET_ALARM_EDIT_TIME_EDIT_CTRL_ID ),
#else
    CREATE_EDIT_TIME_CTRL( MMISET_ALARM_EDIT_TIME_EDIT_CTRL_ID ),
#endif
    CREATE_BUTTON_CTRL( IMAGE_SET_COM_BTN_CANCEL, MMISET_ALARM_EDIT_CANCEL_BTN_CTRL_ID ),
    CREATE_BUTTON_CTRL( IMAGE_SET_COM_BTN_FIXED, MMISET_ALARM_EDIT_FIXED_BTN_CTRL_ID ),
    WIN_SOFTKEY( STXT_OK, TXT_NULL, STXT_RETURN ),
    END_WIN
};

/*****************************************************************************/
//  Discription: enter setttings->alarm list->alarm add window               *
//  Global resource dependence:                                              *
//  Author: bin.wang1                                                        *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC void Settings_AlarmAddWin_Enter( void )
{
    SETTINGS_ALARM_EDIT_WIN_T* pWinData = (SETTINGS_ALARM_EDIT_WIN_T*)SCI_ALLOC_APPZ( sizeof(SETTINGS_ALARM_EDIT_WIN_T) );

    if( PNULL == pWinData )
    {
        TRACE_APP_SETTINGS("pWinData is PNULL!!");
        return;
    }

    pWinData->type = SETTINGS_ALARM_ADD;

    MMK_CreateWin( (uint32 *)MMISET_ALARM_EDIT_WIN_TAB, (ADD_DATA)pWinData );
}

/*****************************************************************************/
//  Discription: enter setttings->alarm list->alarm edit window               *
//  Global resource dependence:                                              *
//  Author: bin.wang1                                                        *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC void Settings_AlarmEditWin_Enter( uint32 event_id )
{
    SETTINGS_ALARM_EDIT_WIN_T* pWinData = (SETTINGS_ALARM_EDIT_WIN_T*)SCI_ALLOC_APPZ( sizeof(SETTINGS_ALARM_EDIT_WIN_T) );

    if( PNULL == pWinData )
    {
        TRACE_APP_SETTINGS("pWinData is PNULL!!");
        return;
    }

    pWinData->type      = SETTINGS_ALARM_EDIT;
    pWinData->eventId   = event_id;

    MMK_CreateWin( (uint32 *)MMISET_ALARM_EDIT_WIN_TAB, (ADD_DATA)pWinData );
}
