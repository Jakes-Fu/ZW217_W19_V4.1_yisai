
#include "std_header.h"
#include <stdlib.h>
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmiacc_text.h"
#include "mmipub.h"
#include "mmicc_id.h"
#include "mmi_modu_main.h"
#include "mmi_textfun.h"
#include "mmi_string.h"
#include "guilcd.h"
#include "guifont.h"
#include "guistring.h"
#include "guitext.h"
#include "guilistbox.h"
#include "guitips.h"
#include "guilabel.h"
#include "guibutton.h"
#include "guictrl_api.h"
#include "guiedit.h"
#include "mmi_common.h"
#include "mmiacc_event.h"
#include "mmialarm_export.h"
#include "alarmClock_export.h"
#include "alarmClock_image.h"
#include "alarmClock_text.h"
#include "alarmClock_id.h"
#include "alarmClock_nv.h"
/*LOCAL uint16  FindAlarmUnuseId(void)
{
    return 0;
}
LOCAL BOOLEAN ALARMCLOCK_SaveInteEventInfo(uint16 id,MMIACC_SMART_EVENT_T*    event)
{
	return 1;
}

LOCAL uint16  GetAlarmRealId(uint16 id)
{
	return 0;
}
LOCAL void MMIALM_RemoveIdlePushMsg(
                                uint16 event_id,
                                MMIACC_SMART_EVENT_E alarm_type
                                )
{
}
*/

#define ALARMCLOCK_ITEM_TIME_HIGHT 50

#define ALARMCLOCK_REPEAT_LIST_MAX_ITEM 8

LOCAL MMI_TEXT_ID_T text_id[] = {
	TXT_ONLY_ONCE,TXT_DAY_Sunday,TXT_DAY_Monday,TXT_DAY_Tuesday,
	TXT_DAY_Wednesday,TXT_DAY_Thursday,TXT_DAY_Friday,TXT_DAY_Saturday
};

LOCAL BOOLEAN s_weeks_alarm_selected[ALARMCLOCK_REPEAT_LIST_MAX_ITEM] = {FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE};

LOCAL uint8 s_alarm_repeat_week_mask[ ALM_MODE_WEEK_NUM ] = { MASK_B0, MASK_B1, MASK_B2, MASK_B3, MASK_B4, MASK_B5, MASK_B6 };

extern EVENT_CURRENT_DATA_T  alarm_current_event_info;
//-------------------------------------edit date------------------------------------//

LOCAL uint16  AlarmClock_FindAlarmUnuseId(void)
{
    uint16 i = 0;
    
    for ( i = 0; i < 5; i++ )
    {
        MMIACC_SMART_EVENT_T smart_event = {0};

        MMIAPIALM_FormInteEventInfo(i, &smart_event);
        if(!smart_event.event_content_info.is_add)
        {
            return i;
        }
    }

    return 5;
}

LOCAL void AlarmClock_CheckList_Create(MMI_CTRL_ID_T ctrl_id,MMI_TEXT_ID_T text_id)
{
	GUILIST_ITEM_T		 item_t = {0};
	GUILIST_ITEM_DATA_T	 item_data = {0};
	GUI_COLOR_T			 list_color = MMI_BLACK_COLOR;
	GUI_RECT_T			 listRect = {0,40,240,180};

	item_t.item_style = GUIITEM_STYLE_RADIO_ONE_LINE_TEXT;
	item_t.item_data_ptr = &item_data;

	item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
	item_data.item_content[0].item_data.text_id = text_id;

	GUILIST_SetRect( ctrl_id, &listRect );
	GUILIST_SetBgColor(ctrl_id,list_color);

	//不画分割线
	GUILIST_SetListState(ctrl_id,GUILIST_STATE_SPLIT_LINE, FALSE );
	//不画高亮条
	//GUILIST_SetListState(ctrl_id,GUILIST_STATE_NEED_HIGHTBAR, FALSE );

	GUILIST_AppendItem(ctrl_id,&item_t);

}

LOCAL uint16 AlarmClockRepeatGetUIListIndex(uint16 weekday_index )
{
	if( weekday_index > ALM_MODE_WEEK_NUM )
	{
		SCI_TRACE_LOW("AlarmClockRepeatGetUIListIndex weekday_index error!!");
		return 1;
	}
	return  weekday_index + 1;
}

LOCAL uint16 AlarmClockRepeatGetWeekdayIndex( uint16 uilist_index )
{
	if( 0 == uilist_index || uilist_index > 7 )
	{
		SCI_TRACE_LOW("uilist_index error!!");
		return 0;
	}
	return (uilist_index - 1);
}

LOCAL MMI_RESULT_E _Callback_AlarmEditBtnSave(void)
{
	uint16                                  selectedNum = 0;
	uint16                                  eventId = 0;
	uint16                                  selectedIndex[ALARMCLOCK_REPEAT_LIST_MAX_ITEM] = { 0 };
	MMI_CTRL_ID_T                           listCtrlId = MMI_ALARMCLOCK_SELECT_WEEK_CTRL_ID;
	SCI_DATE_T                              dateValue = { 0 };
	SETTINGS_ALARM_REPEAT_WIN_T*            pWinData = PNULL;
	MMIACC_SMART_EVENT_FAST_TABLE_T*        pEventTable = PNULL;
	const GUILIST_ITEM_T*                   pListItem = PNULL;
	BOOLEAN save_result = FALSE;
	int day_select_num = 0;
	int	i = 0;
	uint16 SelectedItemIndexArray[8] = {0};


	pWinData = (SETTINGS_ALARM_REPEAT_WIN_T*)MMK_GetWinAddDataPtr( MMI_ALARMCLOCK_SET_DATE_WIN_ID );
	if( PNULL == pWinData )
	{
		return MMI_RESULT_FALSE;
	}

	//get eventId
	SCI_TRACE_LOW("_Callback_AlarmEditBtnSave: pWinData->type = %d",pWinData->type);
	if( SETTINGS_ALARM_ADD == pWinData->type )
	{
		eventId = FindAlarmUnuseId();
		
	}else
	{
		SCI_TRACE_LOW("_Callback_AlarmEditBtnSave: pWinData->eventId = %d",pWinData->eventId);
		eventId = GetAlarmRealId(pWinData->eventId);
	}
	
	SCI_TRACE_LOW("_Callback_AlarmEditBtnSave: eventId = %d", eventId);
	if(ALM_ALARM_NUM > eventId)
       {
       	alarm_current_event_info.id = eventId;
		alarm_current_event_info.type = SMART_EVENT_ALARM;
		alarm_current_event_info.event.event_content_info.ring_type = ALM_FIX_RING;
		alarm_current_event_info.event.event_content_info.ring = 1;
		if(SETTINGS_ALARM_ADD == pWinData->type){
			alarm_current_event_info.event.event_content_info.is_add = TRUE;
		}
		alarm_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;
		alarm_current_event_info.event.event_fast_info.is_on = TRUE;
		alarm_current_event_info.event.event_fast_info.is_valid = TRUE;
        }

	alarm_current_event_info.event.event_fast_info.snooze_type = ALM_SNOOZE_TIME_10_MIN;
	alarm_current_event_info.event.event_fast_info.hour = pWinData->time.hour;
	alarm_current_event_info.event.event_fast_info.minute = pWinData->time.min;

	//get repeat
	selectedNum = GUILIST_GetSelectedItemNum(listCtrlId);
	if( 0 == selectedNum )
	{
		SCI_TRACE_LOW("_Callback_AlarmEditBtnSave: Please select one at least");
		return MMI_RESULT_FALSE;
	}

	pListItem = GUILIST_GetItemPtrByIndex(listCtrlId, 0);
	if( PNULL == pListItem )
	{
		SCI_TRACE_LOW("_Callback_AlarmEditBtnSave: pListItem is PNULL!!");
		return MMI_RESULT_FALSE;
	}

	if(TRUE == GUILIST_GetItemState(pListItem, GUIITEM_STATE_SELECTED))
	{
		alarm_current_event_info.event.event_fast_info.fre_mode    = ALM_MODE_ONCE;
		alarm_current_event_info.event.event_fast_info.fre        	= 0;
	}
	else
	{
		uint16 index = 1;
		uint8 num = 0;
		alarm_current_event_info.event.event_fast_info.fre_mode    = ALM_MODE_EVERYWEEK;
		alarm_current_event_info.event.event_fast_info.fre        	= 0;
		for(index = 1; index < ALARMCLOCK_REPEAT_LIST_MAX_ITEM; index++)
		{
			pListItem = GUILIST_GetItemPtrByIndex(listCtrlId, index);
			if(PNULL == pListItem)
			{
				SCI_TRACE_LOW("_Callback_AlarmEditBtnSave: pListItem is PNULL!!");
				continue;
			}
			if(TRUE == GUILIST_GetItemState(pListItem, GUIITEM_STATE_SELECTED))
			{
				uint16 weekdayIndex = AlarmClockRepeatGetWeekdayIndex(index);
				alarm_current_event_info.event.event_fast_info.fre |= s_alarm_repeat_week_mask[weekdayIndex];// |=(按位或之后赋值)
				num++;
			}
		}
		SCI_TRACE_LOW("_Callback_AlarmEditBtnSave: num = %d", num);
		if(num == 7){
			alarm_current_event_info.event.event_fast_info.fre_mode = ALM_MODE_EVERYDAY;
		}
	}

	save_result = ALARMCLOCK_SaveInteEventInfo(alarm_current_event_info.id, &alarm_current_event_info.event);
	SCI_TRACE_LOW("_Callback_AlarmEditBtnSave: save_result = %d", save_result);
	if(save_result){
		MMIALM_RemoveIdlePushMsg(alarm_current_event_info.id, SMART_EVENT_ALARM);
	}else{
		MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
	}
	
	SCI_FREE(pEventTable);

	MMK_CloseWin( MMI_ALARMCLOCK_SET_DATE_WIN_ID );
	MMK_CloseWin( MMI_ALARMCLOCK_SET_TIME_WIN_ID );
	MMK_CloseWin( MMI_ALARMCLOCK_SET_ALARM_WIN_ID );
	
	return MMI_RESULT_TRUE;

}

BOOLEAN MMIZDT_CloseSaveWin(void)
{
    if(MMK_IsOpenWin(MMI_ALARMCLOCK_SET_DATE_WIN_ID))
    {
        MMK_CloseWin(MMI_ALARMCLOCK_SET_DATE_WIN_ID);
    }
    return TRUE;
}


LOCAL void AlarmClockSelectWeekWin_OPEN_WINDOW(MMI_WIN_ID_T win_id)
{
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	BOOLEAN         reCode = FALSE;
	MMI_CTRL_ID_T   listCtrlId = MMI_ALARMCLOCK_SELECT_WEEK_CTRL_ID;
	MMI_CTRL_ID_T   saveBtn = MMI_ALARMCLOCK_SAVE_BUTTON_CTRL_ID;
	GUI_RECT_T		saveRect = {0,185,75,235};//{0,185,240,235};

	GUI_RECT_T		closeRect = {240-75,185,240,235};//{0,185,240,235};
	MMI_CTRL_ID_T   closeBtn = MMI_ALARMCLOCK_CLOSE2_CTRL_ID;

	uint32 user_data = 0;
	uint16 item_index = 0;
	int	i = 0;

	//create list
	SETTINGS_ALARM_REPEAT_WIN_T*            pWinData = PNULL;
	MMIACC_SMART_EVENT_FAST_TABLE_T*        pEventTable = PNULL;

	MMK_SetAtvCtrl( win_id, listCtrlId );
	GUILIST_RemoveAllItems(listCtrlId);
	GUILIST_SetMaxItem(listCtrlId,ALARMCLOCK_REPEAT_LIST_MAX_ITEM,FALSE);
	for (i = 0;i < ALARMCLOCK_REPEAT_LIST_MAX_ITEM;i++)
	{
		AlarmClock_CheckList_Create( listCtrlId,text_id[i] );
		GUILIST_SetItemUserData(listCtrlId, item_index, &user_data);
		item_index++;
	}
	GUILIST_SetMaxSelectedItem(listCtrlId, item_index);
	GUILIST_SetCurItemIndex( listCtrlId, 0 );
	GUILIST_ChangeCheckListDefSoftkeyID( listCtrlId, TXT_NULL, TXT_NULL, TXT_NULL );
	GUILIST_SetOwnSofterKey( listCtrlId, TRUE );

	//set selected items
	pWinData = (SETTINGS_ALARM_REPEAT_WIN_T*)MMK_GetWinAddDataPtr( win_id );
	if( PNULL == pWinData )
	{
		SCI_TRACE_LOW("AlarmClockRepeat_OPEN_WINDOW: pWinData is PNULL!!");
		return;
	}

	SCI_TRACE_LOW("AlarmClockRepeat_OPEN_WINDOW: pWinData->type = %d",pWinData->type);
	if( SETTINGS_ALARM_ADD == pWinData->type )
	{
		GUILIST_SetSelectedItem( listCtrlId, 0, TRUE ); // once selected
	}else
	{
		SCI_TRACE_LOW("AlarmClockRepeat_OPEN_WINDOW: pWinData->eventId = %d",pWinData->eventId);
		pEventTable = MMIALM_GeEvent( pWinData->eventId );
		SCI_TRACE_LOW("AlarmClockRepeat_OPEN_WINDOW: pEventTable->fre_mode = %d",pEventTable->fre_mode);
		if( ALM_MODE_NO == pEventTable->fre_mode )
		{
			uint16 index = 0;
			for( index = 0; index <= ALM_MODE_WEEK_NUM; index++ )
			{
				SCI_TRACE_LOW("AlarmClockRepeat_OPEN_WINDOW: pEventTable->fre = %d",pEventTable->fre);
				if( s_alarm_repeat_week_mask[index] & pEventTable->fre )
				{
					uint16 uiListIndex = AlarmClockRepeatGetUIListIndex( index );
					GUILIST_SetSelectedItem( listCtrlId, uiListIndex, TRUE );
				}
			}
		}else if (ALM_MODE_EVERYDAY == pEventTable->fre_mode)
		{
			int index = 0;
			for (index = 1;index < 8;index++)
			{
				GUILIST_SetSelectedItem( listCtrlId, index, TRUE );
			}
		}else if (ALM_MODE_EVERYWEEK == pEventTable->fre_mode)
		{
			int index = 0;
			for (index = 1;index < ALARMCLOCK_REPEAT_LIST_MAX_ITEM;index++)
			{
				if( s_alarm_repeat_week_mask[index-1] & pEventTable->fre )
				{
                                GUILIST_SetSelectedItem(listCtrlId,index,TRUE);
				}
			}
		}else
		{
			GUILIST_SetSelectedItem( listCtrlId, 0, TRUE ); // once selected
		}
	}

	saveRect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,closeRect);
	GUIBUTTON_SetRect(saveBtn , &closeRect);
	GUIBUTTON_SetRunSheen(saveBtn, FALSE);
	reCode = GUIBUTTON_SetCallBackFunc( saveBtn, _Callback_AlarmEditBtnSave );
	if( reCode != TRUE )
	{
		SCI_TRACE_LOW("AlarmClockRepeat_OPEN_WINDOW: GUIBUTTON_SetCallBackFunc saveBtn set error");
	}
#if 0
	closeRect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,closeRect);
	GUIBUTTON_SetRect(closeBtn , &saveRect);
	GUIBUTTON_SetRunSheen(closeBtn, FALSE);
	reCode = GUIBUTTON_SetCallBackFunc( closeBtn, MMIZDT_CloseSaveWin );
	if( reCode != TRUE )
	{
		SCI_TRACE_LOW("AlarmClockRepeat_OPEN_WINDOW: GUIBUTTON_SetCallBackFunc closeBtn set error");
	}
#endif

}

LOCAL void AlarmClockSelectWeekWin_FULL_PAINT(MMI_WIN_ID_T win_id)
{
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_RECT_T     clientRect = {0,0,239,239};
	GUI_RECT_T     win_rect   = {0};

	GUI_RECT_T	   title_rect = {0,10,239,40};
	MMI_STRING_T   title_text = {0};
	GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN;
	GUISTR_STYLE_T text_style = {0};

	int	i = 0;
	uint16 SelectedItemIndexArray[8] = {0};

	text_style.align = ALIGN_HMIDDLE;
	text_style.font = SONG_FONT_26;
	text_style.font_color = MMI_DARK_GRAY_COLOR;//MMI_WHITE_COLOR;

	win_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, clientRect);
	GUI_FillRect(&lcd_dev_info, win_rect, MMI_BLACK_COLOR);

	title_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,title_rect);
	MMIRES_GetText(TXT_ALARMCLOCK_SET_DATE,win_id,&title_text);
	GUISTR_DrawTextToLCDInRect(
		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		&title_rect,
		&title_rect,
		&title_text,
		&text_style,
		text_state,
		GUISTR_TEXT_DIR_AUTO
		);

}

LOCAL void AlarmClockSelectWeekWin_APP_WEB(MMI_WIN_ID_T win_id)
{
	BOOLEAN                 reCode = FALSE;
	BOOLEAN                 isCurItemSelected = FALSE;
	uint16                  curIndex = 0;
	uint16                  index = 0;
	MMI_CTRL_ID_T           listCtrlId = MMI_ALARMCLOCK_SELECT_WEEK_CTRL_ID;
	const GUILIST_ITEM_T*   pListItem = PNULL;
	
	curIndex = GUILIST_GetCurItemIndex( listCtrlId );
	pListItem = GUILIST_GetItemPtrByIndex( listCtrlId, curIndex );
	isCurItemSelected = (GUIITEM_STATE_SELECTED == pListItem->item_state) ? TRUE : FALSE;

	// 当前选中的选项
	if( 0 == curIndex )  // 仅一次
	{
		if( FALSE == isCurItemSelected ) // once item will change to selected, so change other item
		{
			for( index = 1; index < ALARMCLOCK_REPEAT_LIST_MAX_ITEM; index++ )
			{
				reCode = GUILIST_SetSelectedItem( listCtrlId, index, FALSE );
				if( FALSE == reCode )
				{
					SCI_TRACE_LOW("AlarmClockRepeat_APP_WEB GUILIST_SetSelectedItem(0 == curIndex) other item set FALSE fail");
				}
			}
		}
	}else
	{
		if( FALSE == isCurItemSelected )
		{
			pListItem = GUILIST_GetItemPtrByIndex( listCtrlId, 0 );
			if( GUIITEM_STATE_SELECTED == pListItem->item_state )
			{
				reCode = GUILIST_SetSelectedItem( listCtrlId, 0, FALSE );
				if( FALSE == reCode )
				{
					SCI_TRACE_LOW("AlarmClockRepeat_APP_WEB GUILIST_SetSelectedItem once item set FLASE fail");
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
			SCI_TRACE_LOW("GUILIST_SetSelectedItem set curIndex FALSE fail");
		}
	}else
	{
		reCode = GUILIST_SetSelectedItem( listCtrlId, curIndex, TRUE );
		if( FALSE == reCode )
		{
			SCI_TRACE_LOW("GUILIST_SetSelectedItem set curIndex TRUE fail");
		}
	}

	MMK_SendMsg( win_id, MSG_FULL_PAINT, PNULL );

}

LOCAL MMI_RESULT_E HandleAlarmClockSelectWeekWin(
	MMI_WIN_ID_T	   win_id,
	MMI_MESSAGE_ID_E   msg_id,
	DPARAM             param
	)
{
	MMI_RESULT_E reCode =  MMI_RESULT_TRUE;

	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		{
			AlarmClockSelectWeekWin_OPEN_WINDOW(win_id);
		}
		break;
	case MSG_FULL_PAINT:
		{
			AlarmClockSelectWeekWin_FULL_PAINT(win_id);
		}
		break;
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
                MMK_CloseWin(win_id);
            break;
	case MSG_APP_WEB:
	case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT
	case MSG_CTL_PENOK:
#endif
		{
			AlarmClockSelectWeekWin_APP_WEB(win_id);
		}
		break;
	case MSG_APP_OK:
	case MSG_CTL_OK:
		{
                  _Callback_AlarmEditBtnSave();
			//MMK_CloseWin(win_id);
		}
		break;
	case MSG_CLOSE_WINDOW:
		{
			MMK_FreeWinAddData(win_id);
		}
		break;
	default:
		{
			reCode = MMI_RESULT_FALSE;
		}
		break;
	}

	return reCode;

}

WINDOW_TABLE( ALARMCLOCK_SELECT_WEEK_WIN_TAB ) =
{
	WIN_FUNC( (uint32)HandleAlarmClockSelectWeekWin ),
	WIN_ID( MMI_ALARMCLOCK_SET_DATE_WIN_ID ),
	WIN_HIDE_STATUS,
	CREATE_LISTBOX_CTRL( GUILIST_CHECKLIST_E, MMI_ALARMCLOCK_SELECT_WEEK_CTRL_ID ),
	CREATE_BUTTON_CTRL( IMAGE_L_BUTTON, MMI_ALARMCLOCK_SAVE_BUTTON_CTRL_ID ),
	//CREATE_BUTTON_CTRL( IMAGE_R_BUTTON, MMI_ALARMCLOCK_CLOSE2_CTRL_ID ),
	END_WIN
};

PUBLIC void AlarmClockSelectWeekWin_Enter(SETTINGS_ALARM_REPEAT_WIN_T *win_data_ptr)
{
	SETTINGS_ALARM_REPEAT_WIN_T* pWinData = (SETTINGS_ALARM_REPEAT_WIN_T*)SCI_ALLOC_APPZ( sizeof(SETTINGS_ALARM_REPEAT_WIN_T) );

	pWinData->type      = win_data_ptr->type;
	pWinData->eventId   = win_data_ptr->eventId;
	pWinData->time.hour = win_data_ptr->time.hour;
	pWinData->time.min  = win_data_ptr->time.min;

	MMK_CreateWin( (uint32 *)ALARMCLOCK_SELECT_WEEK_WIN_TAB, (ADD_DATA)pWinData );

}

//--------------------------------------edit time------------------------------------------//

LOCAL MMI_RESULT_E _Callback_AlarmEditBtnNext( void )
{
	uint8                           hour = 0;
	uint8                           minute = 0;
	uint8                           second = 0;
	SETTINGS_ALARM_REPEAT_WIN_T     repeatWin = { 0 };

	SETTINGS_ALARM_EDIT_WIN_T*  pEditWin = (SETTINGS_ALARM_EDIT_WIN_T*)MMK_GetWinAddDataPtr( MMI_ALARMCLOCK_SET_TIME_WIN_ID );
	if( PNULL == pEditWin )
	{
		SCI_TRACE_LOW("pEditWin is PNULL!");
		return MMI_RESULT_FALSE;
	}

	GUIEDIT_GetTime( MMI_ALARMCLOCK_SELECT_TIME_CTRL_ID, &hour, &minute, &second );

	repeatWin.eventId   = pEditWin->eventId;
	repeatWin.type      = pEditWin->type;
	repeatWin.time.hour = hour;
	repeatWin.time.min  = minute;

	SCI_TRACE_LOW("_Callback_AlarmEditBtnNext repeatWin.eventId = %d,repeatWin.type = %d",repeatWin.eventId,repeatWin.type);
	SCI_TRACE_LOW("_Callback_AlarmEditBtnNext repeatWin.time.hour = %d,repeatWin.time.min = %d",repeatWin.time.hour,repeatWin.time.min);

	AlarmClockSelectWeekWin_Enter(&repeatWin);
	return MMI_RESULT_TRUE;

}

BOOLEAN MMIZDT_CloseSetTimeWin(void)
{
    if(MMK_IsOpenWin(MMI_ALARMCLOCK_SET_TIME_WIN_ID))
    {
        MMK_CloseWin(MMI_ALARMCLOCK_SET_TIME_WIN_ID);
    }
    return TRUE;
}


PUBLIC void AlarmClockEdit_OPEN_WINDOW(MMI_WIN_ID_T win_id)
{
	GUI_RECT_T      editRect = {0,40,239,180};
	MMI_CTRL_ID_T   editCtrId = MMI_ALARMCLOCK_SELECT_TIME_CTRL_ID;

	GUI_RECT_T		nextRect = {0,185,75,235};//{0,185,240,235};
	MMI_CTRL_ID_T   nextBtnId = MMI_ALARMCLOCK_NEXT_PAGE_CTRL_ID;

	GUI_RECT_T		closeRect = {240-75,185,239,235};//{0,185,240,235};
	MMI_CTRL_ID_T   closeBtnId = MMI_ALARMCLOCK_CLOSE_CTRL_ID;

	BOOLEAN                         reCode = FALSE;
	GUI_FONT_ALL_T                  normalFont = { 0 };
	GUI_FONT_ALL_T                  activeFont = { 0 };
	SCI_TIME_T                      timeValue = { 0 };
	GUIEDIT_TIME_STYLE_E            timeStyle = GUIEDIT_TIME_STYLE_12;
	GUIEDIT_TIME_DISPLAY_STYLE_E    dislayStyle = GUIEDIT_TIME_DISPLAY_STYLE_HM;

	GUI_BG_T						edit_bg = {0};
	GUIEDIT_STYLE_E					edit_style = {0};

	SETTINGS_ALARM_EDIT_WIN_T*      pWinData = PNULL;

	pWinData = (SETTINGS_ALARM_EDIT_WIN_T*)MMK_GetWinAddDataPtr( win_id );
	if( PNULL == pWinData )
	{
		SCI_TRACE_LOW("AlarmClockEdit_OPEN_WINDOW pWinData is PNULL!!");
		return;
	}

	//edit position
	editRect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,editRect);
	reCode = GUIAPICTRL_SetBothRect( editCtrId, &editRect);
	if( reCode != TRUE )
	{
		SCI_TRACE_LOW("GUIAPICTRL_SetBothRect set error");
	}

	//edit tips
	reCode = GUIEDIT_SetTipsPos( editCtrId, GUIEDIT_TIPS_POS_NONE );
	if( reCode != TRUE )
	{
		SCI_TRACE_LOW("GUIEDIT_SetTipsPos set error");
	}


	//edit digit font
	normalFont.color = MMI_DARK_GRAY_COLOR;//MMI_WHITE_COLOR;
	normalFont.font = SONG_FONT_30;
	activeFont.color = MMI_BLACK_COLOR;
	activeFont.font = SONG_FONT_42;
	GUIEDIT_SetPickerFont( editCtrId, &normalFont, &activeFont);

	//edit bg
	edit_bg.color = MMI_BLACK_COLOR;
	edit_bg.bg_type = GUI_BG_COLOR;
	edit_bg.shape = GUI_SHAPE_RECT;
	GUIEDIT_SetBg(editCtrId,&edit_bg);

	//edit item height
	reCode = GUIEDIT_SetPickerLineHeight( editCtrId, ALARMCLOCK_ITEM_TIME_HIGHT );
	if( reCode != TRUE )
	{
		SCI_TRACE_LOW("GUIEDIT_SetPickerLineHeight set error");
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
	}else
	{
		const MMIACC_SMART_EVENT_FAST_TABLE_T*  pEventTable = PNULL;
		pEventTable = MMIALM_GeEvent( pWinData->eventId );
		timeValue.hour = pEventTable->hour;
		timeValue.min  = pEventTable->minute;
	}
	GUIEDIT_SetTime( editCtrId, timeValue.hour, timeValue.min, 0 );//设置edit time ctrl的初始时间
	MMK_SetAtvCtrl( win_id, editCtrId );
	MMK_SetAtvCtrl( win_id, nextBtnId );

	nextRect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,closeRect);
	GUIBUTTON_SetRect(nextBtnId,&closeRect);
	GUIBUTTON_SetRunSheen(nextBtnId, FALSE);
	reCode = GUIBUTTON_SetCallBackFunc(nextBtnId, _Callback_AlarmEditBtnNext);
	if( reCode != TRUE )
	{
		SCI_TRACE_LOW("AlarmClockEdit_OPEN_WINDOW GUIBUTTON_SetCallBackFunc nextBtn set error");
	}
#if 0
	closeRect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,closeRect);
	GUIBUTTON_SetRect(closeBtnId,&closeRect);
	GUIBUTTON_SetRunSheen(closeBtnId, FALSE);	
	reCode = GUIBUTTON_SetCallBackFunc(closeBtnId, MMIZDT_CloseSetTimeWin);
	if( reCode != TRUE )
	{
		SCI_TRACE_LOW("AlarmClockEdit_OPEN_WINDOW GUIBUTTON_SetCallBackFunc closeBtn set error");
	}
#endif
}

PUBLIC void AlarmClockEdit_FULL_PAINT(MMI_WIN_ID_T win_id)
{
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_RECT_T clientRect = {0,0,239,239};
	GUI_RECT_T win_rect = {0};

	GUI_RECT_T next_rect = {0,190,239,230};
	GUI_RECT_T title_rect = {0,10,239,40};
	GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN;
	GUISTR_STYLE_T text_style = {0};
	MMI_STRING_T text_title = {0};
	MMI_STRING_T text_next = {0};

	text_style.align = ALIGN_HMIDDLE;
	text_style.font = SONG_FONT_26;
	text_style.font_color = MMI_DARK_GRAY_COLOR;//MMI_WHITE_COLOR;

	win_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, clientRect);
	GUI_FillRect(&lcd_dev_info, win_rect, MMI_BLACK_COLOR);

	title_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,title_rect);
	MMIRES_GetText(TXT_ALARMCLOCK_SET_TIME,win_id,&text_title);
	GUISTR_DrawTextToLCDInRect(
		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		&title_rect,
		&title_rect,
		&text_title,
		&text_style,
		text_state,
		GUISTR_TEXT_DIR_AUTO
		);

	/*next_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,next_rect);
	MMIRES_GetText(TXT_ALARMCLOCK_NEXT_PAGE,win_id,&text_next);
	GUISTR_DrawTextToLCDInRect(
		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		&next_rect,
		&next_rect,
		&text_next,
		&text_style,
		text_state,
		GUISTR_TEXT_DIR_AUTO
		);*/

}

LOCAL MMI_RESULT_E  HandleAlarmClockEditWindow(
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
			AlarmClockEdit_OPEN_WINDOW(win_id);
		}
		break;
	case MSG_FULL_PAINT:
		{
			AlarmClockEdit_FULL_PAINT(win_id);
		}
		break;
	case MSG_APP_OK:
	case MSG_CTL_OK:
		{
			_Callback_AlarmEditBtnNext();
		}
		break;
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		{
			MMK_CloseWin(win_id);
		}
		break;
	case MSG_CLOSE_WINDOW:
		{
			MMK_FreeWinAddData(win_id);
		}
		break;
	default:
		{
			reCode = MMI_RESULT_FALSE;
		}
		break;
	}

	return reCode;

}


WINDOW_TABLE( ALARMCLOCK_EDIT_WIN_TAB ) =
{
	WIN_FUNC( (uint32)HandleAlarmClockEditWindow ),
	WIN_ID( MMI_ALARMCLOCK_SET_TIME_WIN_ID ),
	WIN_HIDE_STATUS,
#ifdef TOUCH_PANEL_SUPPORT
	CREATE_EDIT_TOUCH_TIME_CTRL( MMI_ALARMCLOCK_SELECT_TIME_CTRL_ID ),
#else
	CREATE_EDIT_TIME_CTRL( MMI_ALARMCLOCK_SELECT_TIME_CTRL_ID ),
#endif
	CREATE_BUTTON_CTRL( IMAGE_L_BUTTON, MMI_ALARMCLOCK_NEXT_PAGE_CTRL_ID ),
	//CREATE_BUTTON_CTRL( IMAGE_R_BUTTON, MMI_ALARMCLOCK_CLOSE_CTRL_ID ),
	END_WIN
};

PUBLIC void MMIALARMCLOCK_CreateEditAlarmWin( uint32 event_id )
{
	SETTINGS_ALARM_EDIT_WIN_T* pWinData = (SETTINGS_ALARM_EDIT_WIN_T*)SCI_ALLOC_APPZ( sizeof(SETTINGS_ALARM_EDIT_WIN_T) );

	if( PNULL == pWinData )
	{
		SCI_TRACE_LOW("MMIALARMCLOCK_CreateEditAlarmWin pWinData is PNULL!!");
		return;
	}

	pWinData->type = SETTINGS_ALARM_EDIT;
	pWinData->eventId = event_id;

	MMK_CreateWin( (uint32 *)ALARMCLOCK_EDIT_WIN_TAB, (ADD_DATA)pWinData );
}

PUBLIC void MMIALARMCLOCK_CreateAddAlarmWin()
{
	SETTINGS_ALARM_EDIT_WIN_T* pWinData = (SETTINGS_ALARM_EDIT_WIN_T*)SCI_ALLOC_APPZ( sizeof(SETTINGS_ALARM_EDIT_WIN_T) );

	if( PNULL == pWinData )
	{
		SCI_TRACE_LOW("MMIALARMCLOCK_CreateAddAlarmWin pWinData is PNULL!!");
		return;
	}

	pWinData->type = SETTINGS_ALARM_ADD;

	MMK_CreateWin( (uint32 *)ALARMCLOCK_EDIT_WIN_TAB, (ADD_DATA)pWinData );
}
