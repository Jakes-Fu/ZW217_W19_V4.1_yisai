/*****************************************************************************
** File Name:      watch_alarm_edit.c      //alarm settings                  *
** Author:         qi.liu1                                                   *
** Date:           26/2/2021                                                 *
** Copyright:      All Rights Reserved.                                      *
** Description:  alarm edit window                                           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2021      qi.liu1              Creat                                   *
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
#include "watch_alarm_edit.h"
#include "watch_alarm_main.h"
#include "mmialarm_image.h"
#include "mmialarm_id.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define ALARM_REPEAT_LIST_MAX_ITEM             (7)
#define ALARM_REPEAT_LIST_ITEM_SUNDAY_INDEX    (0)
#define ALARM_REPEAT_LIST_ITEM_SATURDAY_INDEX  (6)

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL uint8 s_adult_alarm_repeat_week_mask[ ALM_MODE_WEEK_NUM ] = { MASK_B0, MASK_B1, MASK_B2, MASK_B3, MASK_B4, MASK_B5, MASK_B6 };

//日期
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST s_alarm_repeat_item_sunday     = { TXT_CLOCK_SUNDAY };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST s_alarm_repeat_item_mondey     = { TXT_CLOCK_MONDAY };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST s_alarm_repeat_item_tuesday    = { TXT_CLOCK_TUESDAY };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST s_alarm_repeat_item_wednesday  = { TXT_CLOCK_WEDNESDAY };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST s_alarm_repeat_item_thursday   = { TXT_CLOCK_THURSDAY };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST s_alarm_repeat_item_friday     = { TXT_CLOCK_FRIDAY };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST s_alarm_repeat_item_saturday   = { TXT_CLOCK_SATURDAY };

LOCAL WATCHCOM_LIST_ITEM__ST s_adult_alarm_repeat_list[] =
{
#ifdef ADULT_WATCH_SUPPORT
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_CHECK_LEFT_ADULTWATCH,   &s_alarm_repeat_item_sunday,    PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_CHECK_LEFT_ADULTWATCH,   &s_alarm_repeat_item_mondey,    PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_CHECK_LEFT_ADULTWATCH,   &s_alarm_repeat_item_tuesday,   PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_CHECK_LEFT_ADULTWATCH,   &s_alarm_repeat_item_wednesday, PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_CHECK_LEFT_ADULTWATCH,   &s_alarm_repeat_item_thursday,  PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_CHECK_LEFT_ADULTWATCH,   &s_alarm_repeat_item_friday,    PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_CHECK_LEFT_ADULTWATCH,   &s_alarm_repeat_item_saturday,  PNULL },
#else
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_CHECK,   &s_alarm_repeat_item_sunday,    PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_CHECK,   &s_alarm_repeat_item_mondey,    PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_CHECK,   &s_alarm_repeat_item_tuesday,   PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_CHECK,   &s_alarm_repeat_item_wednesday, PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_CHECK,   &s_alarm_repeat_item_thursday,  PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_CHECK,   &s_alarm_repeat_item_friday,    PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_CHECK,   &s_alarm_repeat_item_saturday,  PNULL },
#endif
};

/*****************************************************************************/
//  Description : HandleAlarmPressNextBtn
//  Parameter: [In] None
//             [Out] None
//  Author: qi.liu1
//  Note:按下alarm edit界面的next button后的处理函数
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAlarmPressNextBtn( void )
{
    uint8                           hour = 0;
    uint8                           minute = 0;
    uint8                           second = 0;
    ALARM_REPEAT_WIN_T     repeatWin = { 0 };

    ALARM_EDIT_WIN_T*  pEditWin = (ALARM_EDIT_WIN_T*)MMK_GetWinAddDataPtr( WATCHALM_EDIT_WIN_ID );
    if( PNULL == pEditWin )
    {
        TRACE_APP_ALARM("pEditWin is PNULL!");
        return MMI_RESULT_FALSE;
    }

    GUIEDIT_GetTime( MMIALM_EDIT_ALARM_TIME_CTRL_ID, &hour, &minute, &second );

    repeatWin.eventId   = pEditWin->eventId;
    repeatWin.type      = pEditWin->type;
    repeatWin.time.hour = hour;
    repeatWin.time.min  = minute;

    WatchAlarm_RepeatWin_Enter(&repeatWin);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : HandleAlarmPressRepeatFixBtn
//  Parameter: [In] uilist_index
//             [Out] None
//  Author: qi.liu1
//  Note:按下alarm repeat界面的next button后的处理函数
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAlarmPressRepeatFixBtn( void )
{
    uint16                                  selectedNum = 0;
    uint16                                  eventId = 0;
    MMI_WIN_ID_T                            winId = WATCHALM_REPEAT_WIN_ID;
    MMI_CTRL_ID_T                           listCtrlId = MMIALM_REPEAT_LIST_CTRL_ID;
    SCI_DATE_T                              dateValue = { 0 };
    ALARM_REPEAT_WIN_T*            pWinData = PNULL;
    MMIACC_SMART_EVENT_FAST_TABLE_T*        pEventTable = PNULL;
    const GUILIST_ITEM_T*                   pListItem = PNULL;
    BOOLEAN reCode = FALSE;

    pWinData = (ALARM_REPEAT_WIN_T*)MMK_GetWinAddDataPtr( WATCHALM_REPEAT_WIN_ID );
    if( PNULL == pWinData )
    {
        return MMI_RESULT_FALSE;
    }

    //get eventId
    if( WATCH_ALARM_ADD == pWinData->type )
    {

        reCode = MMIALARM_GetOneUnusedAlarm( &eventId );
        if( FALSE == reCode )
        {
            TRACE_APP_ALARM("MMIALARM_GetOneUnusedAlarm fail!!");
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
        TRACE_APP_ALARM("only alarm once!");
        pEventTable->fre_mode   = ALM_MODE_ONCE;
        pEventTable->fre        = 0;
        MMIALARM_ModifyOneAlarm( eventId, pEventTable );
    }
    else
    {
        uint16 index = ALARM_REPEAT_LIST_ITEM_SUNDAY_INDEX;
        pEventTable->fre_mode   = ALM_MODE_EVERYWEEK;
        pEventTable->fre        = 0;
        for( index = ALARM_REPEAT_LIST_ITEM_SUNDAY_INDEX; index < ALARM_REPEAT_LIST_MAX_ITEM; index++ )
        {
            pListItem = GUILIST_GetItemPtrByIndex( listCtrlId, index );
            if( PNULL == pListItem )
            {
                TRACE_APP_ALARM("pListItem is PNULL!!");
                return MMI_RESULT_FALSE;
            }
            if( TRUE == GUILIST_GetItemState(pListItem, GUIITEM_STATE_SELECTED) )
            {
                uint16 weekdayIndex = index;
                pEventTable->fre |= s_adult_alarm_repeat_week_mask[ weekdayIndex ];
            }
        }
    }

    MMIALARM_ModifyOneAlarm( eventId, pEventTable );

    SCI_FREE( pEventTable );

    MMK_CloseWin( winId );
    MMK_CloseWin( WATCHALM_EDIT_WIN_ID );
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : AlarmEdit_OPEN_WINDOW
//  Parameter: [In] win_id
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL void AlarmEdit_OPEN_WINDOW( MMI_WIN_ID_T win_id )
{
    GUI_BOTH_RECT_T      editRect = {0};
    MMI_CTRL_ID_T   editCtrId = MMIALM_EDIT_ALARM_TIME_CTRL_ID;

    MMI_CTRL_ID_T   nextBtn = MMIALM_EDIT_ALARM_TIME_NEXT_BTN_CTRL_ID;
    GUI_RECT_T      btnRect = WATCH_ALARM_BTN_RECT;

    BOOLEAN                         reCode = FALSE;
    GUI_FONT_ALL_T                  normalFont = { 0 };
    GUI_FONT_ALL_T                  activeFont = { 0 };
    SCI_TIME_T                      timeValue = { 0 };
    GUIEDIT_TIME_STYLE_E            timeStyle = GUIEDIT_TIME_STYLE_12;
    GUIEDIT_TIME_DISPLAY_STYLE_E    dislayStyle = GUIEDIT_TIME_DISPLAY_STYLE_HM;

    ALARM_EDIT_WIN_T*      pWinData = PNULL;

    //set title text color
    {
        GUI_COLOR_T      font_color = MMI_RED_COLOR;
        GUIWIN_SetTitleFontColor(win_id,font_color);
    }

    pWinData = (ALARM_EDIT_WIN_T*)MMK_GetWinAddDataPtr( win_id );
    if( PNULL == pWinData )
    {
        TRACE_APP_ALARM("pWinData is PNULL!!");
        return;
    }

    //edit position
    editRect.v_rect.bottom = WATCH_ALARM_EDIT_Y+WATCH_ALARM_EDIT_H;
    editRect.v_rect.left = ALARM_TIME_EDIT_RECT_LEFT;
    editRect.v_rect.right = ALARM_TIME_EDIT_RECT_RIGHT;
    editRect.v_rect.top = WATCH_ALARM_EDIT_Y;
    reCode = GUIAPICTRL_SetBothRect( editCtrId, &editRect);//GUI_BOTH_RECT_T
    if( reCode != TRUE )
    {
        TRACE_APP_ALARM("GUIAPICTRL_SetBothRect set error");
    }

    //edit tips
    reCode = GUIEDIT_SetTipsPos( editCtrId, GUIEDIT_TIPS_POS_NONE );
    if( reCode != TRUE )
    {
        TRACE_APP_ALARM("GUIEDIT_SetTipsPos set error");
    }

    //edit digit font
    normalFont.color = MMI_DARK_GRAY_COLOR;//MMI_WHITE_COLOR
    normalFont.font = WATCH_DEFAULT_BIG_FONT;
    activeFont.color = MMI_DARK_RED_COLOR;//MMI_WHITE_COLOR  MMI_RED_COLOR
    activeFont.font = WATCH_DEFAULT_BIG_FONT;
    GUIEDIT_SetPickerFont( editCtrId, &normalFont, &activeFont);
    //edit item height
    reCode = GUIEDIT_SetPickerLineHeight( editCtrId, WATCH_ALARM_ITEM_H );
    if( reCode != TRUE )
    {
        TRACE_APP_ALARM("GUIEDIT_SetPickerLineHeight set error");
    }
    //edit time style
    if( MMISET_TIME_24HOURS == MMIAPISET_GetTimeDisplayType() )
    {
        timeStyle = GUIEDIT_TIME_STYLE_24;
    }
    GUIEDIT_SetTimeStyle( editCtrId, PNULL, &timeStyle, &dislayStyle, FALSE );

    if( WATCH_ALARM_ADD == pWinData->type )
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

    //set button rect
    GUIBUTTON_SetRect(nextBtn, &btnRect);
}

/*****************************************************************************/
//  Description : AlarmRepeat_OPEN_WINDOW
//  Parameter: [In] win_id
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL void AlarmRepeat_OPEN_WINDOW( MMI_WIN_ID_T win_id )
{
    MMI_CTRL_ID_T   listCtrlId = MMIALM_REPEAT_LIST_CTRL_ID;
    GUI_RECT_T      listRect = { WATCH_ALARM_LIST_X, WATCH_ALARM_LIST_Y, WATCH_ALARM_LIST_X+WATCH_ALARM_LIST_W, WATCH_ALARM_LIST_Y+WATCH_ALARM_LIST_H };
    MMI_CTRL_ID_T   fixedBtn = MMIALM_REPEAT_FIXED_CTRL_ID;
    GUI_RECT_T      btnRect = WATCH_ALARM_BTN_RECT;
    GUI_COLOR_T      font_color = MMI_RED_COLOR;

    //create list
    ALARM_REPEAT_WIN_T*            pWinData = PNULL;
    const MMIACC_SMART_EVENT_FAST_TABLE_T*        pEventTable = PNULL;
    //set title text color
    GUIWIN_SetTitleFontColor(win_id,font_color);

    GUILIST_SetRect( listCtrlId, &listRect );

    //draw list
    WATCHCOM_CHECKLIST_CREATE( s_adult_alarm_repeat_list, ALARM_REPEAT_LIST_MAX_ITEM, listCtrlId );
    GUILIST_SetCurItemIndex( listCtrlId, 0 );
    GUILIST_ChangeCheckListDefSoftkeyID( listCtrlId, TXT_NULL, TXT_NULL, TXT_NULL );
    GUILIST_SetOwnSofterKey( listCtrlId, TRUE );

    //set selected items
    pWinData = (ALARM_REPEAT_WIN_T*)MMK_GetWinAddDataPtr( WATCHALM_REPEAT_WIN_ID );
    if( PNULL == pWinData )
    {
        TRACE_APP_ALARM("pWinData is PNULL!!");
        return;
    }

    if( WATCH_ALARM_ADD == pWinData->type )
    {
#ifdef ADULT_WATCH_SUPPORT
        GUILIST_SetSelectedItem( listCtrlId, ALARM_REPEAT_LIST_ITEM_SUNDAY_INDEX, TRUE ); // once selected
#endif
    }
    else
    {
        pEventTable = MMIALM_GeEvent( pWinData->eventId );
        if( ALM_MODE_EVERYWEEK == pEventTable->fre_mode )
        {
            uint16 index = 0;   // sunday

            for( index = 0; index < ALM_MODE_WEEK_NUM; index++ )
            {
                if( s_adult_alarm_repeat_week_mask[index] & pEventTable->fre )
                {
                    uint16 uiListIndex = index;
                    GUILIST_SetSelectedItem( listCtrlId, uiListIndex, TRUE );
                }
            }
        }
        else
        {
            TRACE_APP_ALARM("fre_mode error!!");
        }
    }

    //set button rect
    GUIBUTTON_SetRect(fixedBtn, &btnRect);
    MMK_SetAtvCtrl( win_id, listCtrlId );
}

/*****************************************************************************/
//  Description : AlarmRepeat_List_Select
//  Parameter: [In] winId
//             [Out] None
//  Author: qi.liu1
//  Note:在alarm repeat界面，选中一个repeat日期
/*****************************************************************************/
LOCAL void AlarmRepeat_List_Select(MMI_WIN_ID_T winId )
{
    BOOLEAN                 reCode = FALSE;
    BOOLEAN                 isCurItemSelected = FALSE;
    uint16                  curIndex = 0;
    MMI_CTRL_ID_T           listCtrlId = MMIALM_REPEAT_LIST_CTRL_ID;
    const GUILIST_ITEM_T*   pListItem = PNULL;

    curIndex = GUILIST_GetCurItemIndex( listCtrlId );
    pListItem = GUILIST_GetItemPtrByIndex( listCtrlId, curIndex );

    isCurItemSelected = (GUIITEM_STATE_SELECTED == pListItem->item_state) ? TRUE : FALSE;

    // change current item state
    if( TRUE == isCurItemSelected )
    {
        reCode = GUILIST_SetSelectedItem( listCtrlId, curIndex, FALSE );
        if( FALSE == reCode )
        {
            TRACE_APP_ALARM("GUILIST_SetSelectedItem set curIndex FALSE fail");
        }
    }
    else
    {
        reCode = GUILIST_SetSelectedItem( listCtrlId, curIndex, TRUE );
        if( FALSE == reCode )
        {
            TRACE_APP_ALARM("GUILIST_SetSelectedItem set curIndex TRUE fail");
        }
    }

    MMK_SendMsg( winId, MSG_FULL_PAINT, PNULL );
}

/*****************************************************************************/
//  Description : HandleAlarmEditWindow
//  Parameter: [In] winId
//             [Out] None
//  Author: qi.liu1
//  Note:alarm edit界面处理函数
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmEditWindow(
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
            AlarmEdit_OPEN_WINDOW(win_id);
            break;
        }

        case MSG_FULL_PAINT:
        {
            //bg color
            WATCHCOM_DisplayBackground( win_id );
            break;
        }

#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
        {
            //Add Alarm
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;
            if(MMIALM_EDIT_ALARM_TIME_NEXT_BTN_CTRL_ID == ctrl_id)
            {
                //Press Next btn:(->)
                HandleAlarmPressNextBtn();
            }
            break;
        }
#endif

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMK_CloseWin( win_id );
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            MMK_FreeWinAddData( win_id );
            break;
        }

        default:
            reCode = MMI_RESULT_FALSE;
            break;
    }

    return reCode;
}

/*****************************************************************************/
//  Description : HandleAlarmRepeatWindow
//  Parameter: [In] winId
//             [Out] None
//  Author: qi.liu1
//  Note:alarm repeat界面处理函数
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmRepeatWindow(
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
            AlarmRepeat_OPEN_WINDOW( win_id );
            break;
        }
        break;

        case MSG_FULL_PAINT:
        {
            //bg color
            WATCHCOM_DisplayBackground( win_id );
            break;
        }

        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        {
            //select Sunday. Monday ... e.g.
            AlarmRepeat_List_Select(win_id);
            break;
        }

#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
        {
            //Add Alarm
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;
            if(MMIALM_REPEAT_FIXED_CTRL_ID == ctrl_id)
            {
                //Press Check Ok btn:(√)
                HandleAlarmPressRepeatFixBtn();
            }
            else
            {
               //select Sunday. Monday ... e.g.
               AlarmRepeat_List_Select(win_id);
            }
            break;
         }
#endif

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMK_CloseWin( win_id );
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            MMK_FreeWinAddData( win_id );
            break;
        }

        default:
            reCode = MMI_RESULT_FALSE;
            break;
    }

    return reCode;
}

//idle ->  alarm add window
WINDOW_TABLE( MMIALARM_EDIT_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleAlarmEditWindow ),
    WIN_ID( WATCHALM_EDIT_WIN_ID ),
    WIN_HIDE_STATUS,
#ifdef ADULT_WATCH_SUPPORT
    WIN_TITLE(TXT_ALARM_TIME_SELECT),
#endif
#ifdef TOUCH_PANEL_SUPPORT
    CREATE_EDIT_TOUCH_TIME_CTRL( MMIALM_EDIT_ALARM_TIME_CTRL_ID ),
#else
    CREATE_EDIT_TIME_CTRL( MMIALM_EDIT_ALARM_TIME_CTRL_ID ),
#endif
    CREATE_BUTTON_CTRL( res_aw_alarm_ic_next, MMIALM_EDIT_ALARM_TIME_NEXT_BTN_CTRL_ID ),
#ifndef TOUCH_PANEL_SUPPORT
    WIN_SOFTKEY( STXT_OK, TXT_NULL, STXT_RETURN ),
#endif
    END_WIN
};

WINDOW_TABLE( MMIALARM_REPEAT_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleAlarmRepeatWindow ),
    WIN_ID( WATCHALM_REPEAT_WIN_ID ),
    WIN_HIDE_STATUS,
#ifdef ADULT_WATCH_SUPPORT
    WIN_TITLE(TXT_ALARM_REPEAT),
#endif
    CREATE_LISTBOX_CTRL( GUILIST_CHECKLIST_E, MMIALM_REPEAT_LIST_CTRL_ID ),
    CREATE_BUTTON_CTRL( res_aw_alarm_ic_add, MMIALM_REPEAT_FIXED_CTRL_ID ),
#ifndef TOUCH_PANEL_SUPPORT
    WIN_SOFTKEY( STXT_OK, TXT_NULL, STXT_RETURN ),
#endif
    END_WIN
};

/*****************************************************************************/
//  Description : WatchAlarm_RepeatWin_Enter
//  Parameter: [In] win_data_ptr
//             [Out] None
//  Author: qi.liu1
//  Note:alarm repeat win的入口函数
/*****************************************************************************/
PUBLIC void WatchAlarm_RepeatWin_Enter( ALARM_REPEAT_WIN_T* win_data_ptr )
{
    ALARM_REPEAT_WIN_T* pWinData = PNULL;

    pWinData = (ALARM_REPEAT_WIN_T*)SCI_ALLOC_APPZ( sizeof(ALARM_REPEAT_WIN_T) );
    if( PNULL == win_data_ptr )
    {
        TRACE_APP_ALARM("win_data_ptr is PNULL!!");
        return;
    }
    pWinData->type      = win_data_ptr->type;
    pWinData->eventId   = win_data_ptr->eventId;
    pWinData->time.hour = win_data_ptr->time.hour;
    pWinData->time.min  = win_data_ptr->time.min;

    MMK_CreateWin( (uint32 *)MMIALARM_REPEAT_WIN_TAB, (ADD_DATA)pWinData );
}

/*****************************************************************************/
//  Description : WatchAlarm_EditWin_Enter
//  Parameter: [In] event_id
//             [Out] None
//  Author: qi.liu1
//  Note:alarm edit win的入口函数
/*****************************************************************************/
PUBLIC void WatchAlarm_EditWin_Enter( uint32 event_id )
{
    ALARM_EDIT_WIN_T* pWinData = PNULL;

    pWinData = (ALARM_EDIT_WIN_T*)SCI_ALLOC_APPZ( sizeof(ALARM_EDIT_WIN_T) );
    if( PNULL == pWinData )
    {
        TRACE_APP_ALARM("pWinData is PNULL!!");
        return;
    }

    pWinData->type      = WATCH_ALARM_EDIT;
    pWinData->eventId   = event_id;

    MMK_CreateWin( (uint32 *)MMIALARM_EDIT_WIN_TAB, (ADD_DATA)pWinData );
}

/*****************************************************************************/
//  Description : WatchAlarm_AddWin_Enter
//  Parameter: [In] event_id
//             [Out] None
//  Author: qi.liu1
//  Note:alarm add win的入口函数
/*****************************************************************************/
PUBLIC void WatchAlarm_AddWin_Enter( void )
{
    ALARM_EDIT_WIN_T* pWinData = (ALARM_EDIT_WIN_T*)SCI_ALLOC_APPZ( sizeof(ALARM_EDIT_WIN_T) );

    if( PNULL == pWinData )
    {
        TRACE_APP_ALARM("pWinData is PNULL!!");
        return;
    }

    pWinData->type = WATCH_ALARM_ADD;

    MMK_CreateWin( (uint32 *)MMIALARM_EDIT_WIN_TAB, (ADD_DATA)pWinData );
}
