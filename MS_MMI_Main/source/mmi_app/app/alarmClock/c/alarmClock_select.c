
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
#include "mmi_common.h"
#include "mmialarm_export.h"
#include "alarmClock_export.h"
#include "alarmClock_image.h"
#include "alarmClock_text.h"
#include "alarmClock_id.h"
#include "alarmClock_nv.h"

#define ALARMSELECT_ADD_ITEM 0
#define ALARMSELECT_EDIT_ITEM 1
#define ALARMSELECT_CLOSE_ITEM 2
#define ALARMSELECT_DELETE_ITEM 3
#define ALARMSELECT_RETURN_ITEM 4

EVENT_CURRENT_DATA_T  alarm_current_event_info = {0, 0, {0}};

LOCAL MMI_TEXT_ID_T text_id[] = {
	TXT_ALARMCLOCK_SETTING,
	TXT_ALARMCLOCK_OPEN,
	TXT_ALARMCLOCK_DELETE,
	TXT_ALARMCLOCK_RETURN
};

LOCAL BOOLEAN isAlarmClockOpen(uint32 event_id)
{
	const MMIACC_SMART_EVENT_FAST_TABLE_T*  pEventTable = PNULL;

	pEventTable = MMIALM_GeEvent( event_id );
	SCI_TRACE_LOW("isAlarmClockOpen event_id = %d",event_id);
	SCI_TRACE_LOW("isAlarmClockOpen pEventTable->is_on = %d",pEventTable->is_on);
	
	if (pEventTable->is_on == TRUE)
	{
		return TRUE;
	}

	return FALSE;
}

LOCAL void CreateAlarmSelectList(
	MMI_WIN_ID_T win_id,
	MMI_CTRL_ID_T listCtrlId,
	MMI_TEXT_ID_T text_id
	)
{
	GUILIST_ITEM_T item_t = {0};
	GUILIST_ITEM_DATA_T item_data = {0};
	GUI_RECT_T list_rect = {20,20,240,240};
	GUI_COLOR_T list_color = {0};
	list_color = MMI_BLACK_COLOR;

	item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
	item_t.item_data_ptr = &item_data;

	item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
	item_data.item_content[0].item_data.text_id = text_id;
	item_data.item_content[0].custom_font_color = MMI_DEFAULT_TEXT_COLOR; //set_dark_color
	item_data.item_content[0].custom_font_color_focus = MMI_DEFAULT_TEXT_COLOR;
	item_data.item_content[0].is_custom_font_color = TRUE;

	//list_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,list_rect);
	GUILIST_SetRect(listCtrlId,&list_rect);
	GUILIST_SetBgColor(listCtrlId,list_color);

	//不画分割线
	GUILIST_SetListState(listCtrlId,GUILIST_STATE_SPLIT_LINE, FALSE );
	//不画高亮条
	//GUILIST_SetListState(listCtrlId,GUILIST_STATE_NEED_HIGHTBAR, FALSE );

	GUILIST_AppendItem(listCtrlId,&item_t);
}

LOCAL void AlarmSelectWin_Open_Window(MMI_WIN_ID_T win_id,uint32 event_id)
{
	MMI_CTRL_ID_T   listCtrlId = MMI_ALARMCLOCK_SELECT_LIST_CTR_ID;
	int i = 0;
	uint32 lisNum = 5;
	uint32 eventId = PNULL;
	const MMIACC_SMART_EVENT_FAST_TABLE_T* event_delay_ptr = NULL;
	BOOLEAN is_on = FALSE;


	MMK_SetAtvCtrl( win_id, listCtrlId );
	GUILIST_RemoveAllItems(listCtrlId);
	GUILIST_SetMaxItem(listCtrlId,lisNum,FALSE);

	alarm_current_event_info.id = event_id;
	MMIAPIALM_FormInteEventInfo(alarm_current_event_info.id,&alarm_current_event_info.event);
	event_delay_ptr = GetDelayEvent(alarm_current_event_info.id);
	is_on = alarm_current_event_info.event.event_fast_info.is_on | event_delay_ptr->is_on;

      CreateAlarmSelectList(win_id,listCtrlId,TXT_ALARMCLOCK_ADD);
	CreateAlarmSelectList(win_id,listCtrlId,TXT_ALARMCLOCK_SETTING);
	if (is_on)
	{
		CreateAlarmSelectList(win_id,listCtrlId,TXT_ALARMCLOCK_CLOSE);
	}else{
		CreateAlarmSelectList(win_id,listCtrlId,TXT_ALARMCLOCK_OPEN);
	}
	CreateAlarmSelectList(win_id,listCtrlId,TXT_ALARMCLOCK_DELETE);
	
	CreateAlarmSelectList(win_id,listCtrlId,TXT_ALARMCLOCK_RETURN);
	
}


LOCAL void AlarmSelectWin_APP_OK(MMI_WIN_ID_T win_id,uint32 event_id)
{
	uint16          index = 0;
	MMI_CTRL_ID_T   listCtrlId = MMI_ALARMCLOCK_SELECT_LIST_CTR_ID;
	const MMIACC_SMART_EVENT_FAST_TABLE_T* event_delay_ptr = NULL;
	
	index = GUILIST_GetCurItemIndex( listCtrlId );
	
	switch(index)
	{
	case ALARMSELECT_ADD_ITEM:
		{
			MMIALARMCLOCK_CreateAddAlarmWin();
                    MMK_CloseWin(win_id);
		}
		break;
	case ALARMSELECT_EDIT_ITEM:
		{
			MMIALARMCLOCK_CreateEditAlarmWin(event_id);
                  MMK_CloseWin(win_id);
		}
		break;
	case ALARMSELECT_CLOSE_ITEM:
		{
			alarm_current_event_info.id = event_id;
			SCI_TRACE_LOW("AlarmSelectAppOk: alarm_current_event_info.id = %d", alarm_current_event_info.id);
			MMIAPIALM_FormInteEventInfo(alarm_current_event_info.id,&alarm_current_event_info.event);
			
			alarm_current_event_info.type = SMART_EVENT_ALARM;
       		alarm_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;
			
			event_delay_ptr = GetDelayEvent(alarm_current_event_info.id);
			if(alarm_current_event_info.event.event_fast_info.is_on || event_delay_ptr->is_on)
			{
				alarm_current_event_info.event.event_fast_info.is_on = FALSE;
				#ifdef CALENDAR_SUPPORT
                    		MMIAPISCH_DeleteUnreadEvent(alarm_current_event_info.id);
				#endif
                    		MMIALM_RemoveIdlePushMsg(alarm_current_event_info.id, SMART_EVENT_ALARM);
			}else{
				alarm_current_event_info.event.event_fast_info.is_on = TRUE;
			}
			
			// 关闭延时闹钟
       		SetDelayAlarm(alarm_current_event_info.id, &alarm_current_event_info.event, FALSE);
                	// 修改闹钟设置
                	ALARMCLOCK_ModifyEvent(alarm_current_event_info.id, &alarm_current_event_info.event.event_fast_info, TRUE);
			
			MMK_CloseWin(win_id);
		}
		break;
	case ALARMSELECT_DELETE_ITEM:
		{
			alarm_current_event_info.id = event_id;
			SCI_TRACE_LOW("AlarmSelectAppOk: alarm_current_event_info.id = %d", alarm_current_event_info.id);
			if(ALM_ALARM_NUM > alarm_current_event_info.id){
				MMIAPIALM_FormInteEventInfo(alarm_current_event_info.id, &alarm_current_event_info.event);
                     	alarm_current_event_info.type = SMART_EVENT_ALARM;
                     	alarm_current_event_info.event.event_fast_info.type = SMART_EVENT_ALARM;
                     	alarm_current_event_info.event.event_fast_info.is_on = FALSE;
                     	alarm_current_event_info.event.event_content_info.w_length = 0;
                     	alarm_current_event_info.event.event_content_info.is_add = FALSE;
					 
                     	SetDelayAlarm(alarm_current_event_info.id, &alarm_current_event_info.event, FALSE);
                     	ALARMCLOCK_SaveInteEventInfo(alarm_current_event_info.id,&alarm_current_event_info.event);
				ALARMCLOCK_ModifyEvent(alarm_current_event_info.id, &alarm_current_event_info.event.event_fast_info, TRUE);
				MMIALM_RemoveIdlePushMsg(alarm_current_event_info.id, SMART_EVENT_ALARM);
			}
			
			MMK_CloseWin(win_id);
		}
		break;
	case ALARMSELECT_RETURN_ITEM:
		{
			MMK_CloseWin(win_id);
		}
		
	default:
		{
			SCI_TRACE_LOW("AlarmSelectWin_APP_OK: index error %d", index);
		}
		break;
	}

}

LOCAL MMI_RESULT_E HandleAlarmSelectWin(
	MMI_WIN_ID_T win_id, 
	MMI_MESSAGE_ID_E msg_id,
	DPARAM param
	)
{
	MMI_RESULT_E reCode =  MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
      uint32 * p_index = PNULL;
      uint32 event_id = 0;
      p_index = (uint32 *) MMK_GetWinAddDataPtr(win_id);
    
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		{
                    if(p_index != PNULL)
                    {
                        event_id = *p_index;
                    }
			AlarmSelectWin_Open_Window(win_id,event_id);
		}
		break;
	case MSG_FULL_PAINT:
		{
			GUI_RECT_T clientRect = {0,0,240,240};
			GUI_RECT_T win_rect = {0};

			win_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, clientRect);
			GUI_FillRect(&lcd_dev_info, win_rect, MMI_BLACK_COLOR);

		}
		break;
	case MSG_CTL_OK:
	case MSG_CTL_MIDSK:
	case MSG_APP_OK:
	case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT
	case MSG_CTL_PENOK:
#endif
		{
                    if(p_index != PNULL)
                    {
                        event_id = *p_index;
                    }
			AlarmSelectWin_APP_OK(win_id,event_id);
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
                if(p_index != PNULL)
                {
                    SCI_FREE(p_index);
                }
		}
		break;
	default:
		reCode = MMI_RESULT_FALSE;
		break;
	}

	return reCode;
}

WINDOW_TABLE(MMI_ALARMCLOCK_SELECT_WIN_TAB) = 
{
	WIN_ID(MMI_ALARMCLOCK_SET_ALARM_WIN_ID),
	WIN_FUNC((uint32)HandleAlarmSelectWin),
	WIN_HIDE_STATUS,
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMI_ALARMCLOCK_SELECT_LIST_CTR_ID),
	END_WIN
};

PUBLIC void MMIALARMCLOCK_CreateSelectWin( uint32 event_id )
{
	uint32* pWinData = (uint32*)SCI_ALLOC_APPZ( sizeof(uint32) );

	if( PNULL == pWinData )
	{
		SCI_TRACE_LOW("MMIALARMCLOCK_CreateSelectWin pWinData is PNULL!!");
		return;
	}

	*pWinData = event_id;
	SCI_TRACE_LOW("MMIALARMCLOCK_CreateSelectWin pWinData = %d",pWinData);
	MMK_CreateWin( (uint32 *)MMI_ALARMCLOCK_SELECT_WIN_TAB, (ADD_DATA)pWinData );

}
