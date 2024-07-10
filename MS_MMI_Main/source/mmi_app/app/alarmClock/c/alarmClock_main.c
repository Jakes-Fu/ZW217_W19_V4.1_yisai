
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
#include "guibutton.h"
#include "guicommon.h"
#include "mmk_timer.h"
#include "mmi_common.h"
#include "mmiacc_event.h"
#include "mmialarm_export.h"
#include "watch_alarm_main.h"
#include "alarmClock_export.h"
#include "alarmClock_image.h"
#include "alarmClock_text.h"
#include "alarmClock_id.h"
#include "alarmClock_nv.h"
#ifdef WIN32
#include "zdt_yx_net.h"
#endif
//#include "dslchat_image.h"
//#include "common_mdu_def.h"

#define ALARMCLOCK_MAX_ITEM	 5
#define WATCHSET_ALARM_TEMP_STR_LEN 32

LOCAL uint8 timer_id = 0;

/*****************************************************************************/
//  Description : AlarmMainListGetAlarmNum
//  Parameter: [In] None
//             [Out] None
//  Author: qi.liu1
//  Note:获取当前闹钟个数
/*****************************************************************************/
LOCAL uint16 MMIALARMCLOCK_GetAlarmNum( void )
{
    uint16      index = 0;
    uint16      alarmNum = 0;

    for( index = 0; index < ALM_ALARM_NUM; index++ )
    {
        const MMIACC_SMART_EVENT_FAST_TABLE_T* pEventTable = PNULL;

        pEventTable = MMIALM_GeEvent( (uint16)(EVENT_ALARM_0+index) );
        if( PNULL == pEventTable )
        {
            SCI_TRACE_LOW("pEventTable is PNULL");
            return alarmNum;
        }
        if( TRUE == pEventTable->is_valid )
        {
            alarmNum++;
        }
    }
    return alarmNum;
}

//---------------------AlarmMaxWin----------------------//

LOCAL void MMIALARMCLOCK_CloseAlarmMaxWin(uint32 param)
{
	BOOLEAN is_stop;
	if (MMK_IsOpenWin(MMI_ALARMCLOCK_ALARM_MAX_WIN_ID))
	{
		MMK_CloseWin(MMI_ALARMCLOCK_ALARM_MAX_WIN_ID);
	}
	is_stop = MMK_StopTimer(timer_id);
	SCI_TRACE_LOW("MMIALARMCLOCK_CloseAlarmMaxWin is_stop = %d",is_stop);
}

LOCAL MMI_RESULT_E HandleAlarmClockMaxWin(
	MMI_WIN_ID_T win_id, 
	MMI_MESSAGE_ID_E msg_id,
	DPARAM param)
{
	MMI_RESULT_E reCode =  MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		{
			timer_id = MMK_CreateTimerCallback(1000,MMIALARMCLOCK_CloseAlarmMaxWin,PNULL,FALSE);
			SCI_TRACE_LOW("HandleAlarmClockMaxWin time_id = %d",timer_id);
		}
		break;
	case MSG_FULL_PAINT:
		{
			GUI_RECT_T clientRect = {0,0,240,240};
			GUI_RECT_T win_rect = {0};

			win_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, clientRect);
			GUI_FillRect(&lcd_dev_info, win_rect, MMI_BLACK_COLOR);

			GUIRES_DisplayImg(NULL,&win_rect,NULL,win_id,IMAGE_ALARM_MAX,&lcd_dev_info);

		}
		break;
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
	case MSG_CLOSE_WINDOW:
		{
			MMK_CloseWin(win_id);
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

WINDOW_TABLE(MMI_ALARMCLOCK_ALARM_MAX_WIN_TAB) = 
{
	WIN_ID(MMI_ALARMCLOCK_ALARM_MAX_WIN_ID),
	WIN_FUNC((uint32)HandleAlarmClockMaxWin),
	WIN_HIDE_STATUS,
	END_WIN
};

PUBLIC void MMIALARMCLOCK_CreateAlarmMaxWin()
{
	MMK_CreateWin((uint32*)MMI_ALARMCLOCK_ALARM_MAX_WIN_TAB, NULL);
}


//----------------------------------------main------------------------------------//

LOCAL BOOLEAN AlarmClockConver24HourTo12Hour( uint8 *hour_ptr )
{
	BOOLEAN result = FALSE;

	if( PNULL == hour_ptr )
	{
		SCI_TRACE_LOW("SettingsConver24HourTo12Hour hour_ptr is PNULL!");
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

LOCAL BOOLEAN SettingsAlarmListGetTimeStr( 
	SCI_TIME_T* sys_time_ptr,
	MMI_STRING_T* time_str_ptr )
{
	BOOLEAN     isAM = FALSE;
	uint8       timeStr[WATCHSET_ALARM_TEMP_STR_LEN] = { 0 };

	if( PNULL == time_str_ptr || PNULL == sys_time_ptr )
	{
		SCI_TRACE_LOW("input is PNULL!");
		return FALSE;
	}
	if( MMISET_TIME_12HOURS == MMIAPISET_GetTimeDisplayType() )
	{
		isAM = AlarmClockConver24HourTo12Hour( &(sys_time_ptr->hour) );
		if( TRUE == isAM )
		{
			sprintf( (char*)timeStr," %02d:%02d AM", sys_time_ptr->hour, sys_time_ptr->min );
		}
		else
		{
			sprintf( (char*)timeStr," %02d:%02d PM", sys_time_ptr->hour, sys_time_ptr->min );
		}
	}else
	{
		sprintf( (char*)timeStr," %02d:%02d ", sys_time_ptr->hour, sys_time_ptr->min );
	}
	MMIAPICOM_StrToWstr( timeStr, time_str_ptr->wstr_ptr );
	time_str_ptr->wstr_len = MMIAPICOM_Wstrlen( time_str_ptr->wstr_ptr );
	return TRUE;

}

LOCAL uint16 SettingsAlarmListGetAlarmNum(void)
{
	uint16      index = 0;
	uint16      alarmNum = 0;

	for( index = 0; index < ALARMCLOCK_MAX_ITEM; index++ )
	{
		const MMIACC_SMART_EVENT_FAST_TABLE_T* pEventTable = PNULL;

		pEventTable = MMIALM_GeEvent( (uint16)(EVENT_ALARM_0+index) );
		if( PNULL == pEventTable )
		{
			SCI_TRACE_LOW("pEventTable is PNULL");
			return alarmNum;
		}
		if( TRUE == pEventTable->is_valid )
		{
			alarmNum++;
		}
	}
	return alarmNum;

}

LOCAL MMI_RESULT_E _AddAlarmClock_ButtonCallBack()
{
	uint8 eventNum = GetAlarmTotalNum();
	SCI_TRACE_LOW("_AddAlarmClock_ButtonCallBack eventNum = %d",eventNum);
	if(eventNum >= ALARMCLOCK_MAX_ITEM ){
		MMIALARMCLOCK_CreateAlarmMaxWin();
		return MMI_RESULT_FALSE;
	}
	//MMIALARMCLOCK_CreateAddAlarmWin();

	return MMI_RESULT_TRUE;
}

LOCAL void AddAlarmAlertTitle(MMI_WIN_ID_T win_id)
{
	MMI_CTRL_ID_T ctrl_id = MMI_ALARMCLOCK_ADD_CLOCK_CTRL_ID;
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

	GUI_RECT_T img_rect = {0,45,239,85};
	GUI_RECT_T text_rect = {0,45,239,85};
	MMI_STRING_T text_string = {0};
	GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN;
	GUISTR_STYLE_T text_style = {0};
	GUI_FONT_ALL_T font_type = {0};
	GUI_BG_T button_bg = {0};

	MMK_SetAtvCtrl(win_id, ctrl_id);

	text_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,text_rect);
	GUIBUTTON_SetRect(ctrl_id,&text_rect);

	font_type.font = SONG_FONT_22;
	font_type.color = MMI_RED_COLOR;
	GUIBUTTON_SetFont(ctrl_id,&font_type);

	MMIRES_GetText(TXT_ADD_ALARMCLOCK,win_id,&text_string);
	GUIBUTTON_SetText(ctrl_id,text_string.wstr_ptr,text_string.wstr_len);
	
	button_bg.bg_type = GUI_BG_IMG;
	button_bg.img_id = IMAGE_ITEM_LIST_BG;
	GUIBUTTON_SetPressedBg(ctrl_id, &button_bg);
	GUIBUTTON_SetRunSheen(ctrl_id, FALSE);
	
	img_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,img_rect);
	GUIRES_DisplayImg(NULL,&img_rect,NULL,win_id,IMAGE_ITEM_LIST_BG,&lcd_dev_info);
}

LOCAL void AlarmClock_AppendAlarmListItem(
	MMI_WIN_ID_T win_id,
	MMI_CTRL_ID_T ctrl_id,
	MMI_IMAGE_ID_T image_id,
	MMI_STRING_T time_str,
	MMI_STRING_T repeat_str,
	BOOLEAN is_on
	)
{
	GUILIST_ITEM_T item_t = {0};
	GUIITEM_STATE_T item_state = {0};
	GUILIST_ITEM_DATA_T item_data = {0};
#ifdef ZTE_SUPPORT_240X284
	GUI_RECT_T list_rect = {0,20,240,283};
#elif ZTE_SUPPORT_240X240
	GUI_RECT_T list_rect = {0,20,240,240};
#endif
	GUI_COLOR_T list_color = {0};

	item_t.item_style = GUIITEM_STYLE_TWO_TEXT_AND_ONE_ICON_MS;
	item_t.item_data_ptr = &item_data;
	item_t.item_state = GUIITEM_STATE_SELFADAPT_RECT|GUIITEM_STATE_CONTENT_CHECK;

	item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	item_data.item_content[0].item_data.text_buffer = time_str;
	item_data.item_content[0].custom_font_color = GUI_RGB2RGB565(0x0b, 0x84, 0xff);
	item_data.item_content[0].custom_font_color_focus = GUI_RGB2RGB565(0x0b, 0x84, 0xff);
	item_data.item_content[0].is_custom_font_color = is_on;

	item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	item_data.item_content[1].item_data.text_buffer = repeat_str;
	item_data.item_content[1].custom_font_color = 0x7BEF;
	item_data.item_content[1].custom_font_color_focus = 0x7BEF;
	item_data.item_content[1].is_custom_font_color = TRUE;

	item_data.item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
	item_data.item_content[2].item_data.image_id = image_id;

	//不画分割线
	GUILIST_SetListState( ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE );
	//不画高亮条
	GUILIST_SetListState( ctrl_id, GUILIST_STATE_NEED_HIGHTBAR, FALSE );

	GUILIST_SetNeedPrgbarBlock(ctrl_id,FALSE);

	list_color = MMI_BLACK_COLOR;
	list_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,list_rect);
	GUILIST_SetRect(ctrl_id,&list_rect);
	GUILIST_SetBgColor(ctrl_id,list_color);

	GUILIST_AppendItem(ctrl_id, &item_t);
}


LOCAL void DisplayAlarmListItem( 
					MMI_WIN_ID_T win_id, 
					MMI_CTRL_ID_T list_ctr_id,
					uint16 event_num )
{
	//AddAlarmAlertTitle(win_id);
	if (event_num != 0)
	{
		uint16      index = 0;
		uint16      itemIndex = 0;
		uint16      itemNum = 0;
		uint16      curSelection = 0;

		curSelection = GUILIST_GetCurItemIndex( list_ctr_id );

		GUILIST_RemoveAllItems( list_ctr_id );

		for (index = 0;index < ALARMCLOCK_MAX_ITEM && index < event_num; index++)
		{
			MMI_IMAGE_ID_T   iconId = 0;
			SCI_TIME_T       alarmTime = { 0 };
			wchar            wTimeStr[WATCHSET_ALARM_TEMP_STR_LEN] = { 0 };
			wchar            wRepeatStr[WATCHSET_ALARM_TEMP_STR_LEN] = { 0 };
			MMI_STRING_T     repeatStr = { 0 };
			uint16           event_id = EVENT_ALARM_0 + index;
			uint32           userData = 0;
			const MMIACC_SMART_EVENT_FAST_TABLE_T*  event_delay_ptr = PNULL;
			MMIACC_SMART_EVENT_T    smart_event = {0};
			BOOLEAN is_on = FALSE;
			wchar temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
			wchar connect[] = {L" "};
			MMI_STRING_T connect_str = {0};
			MMI_STRING_T name_str = {0};
		    	MMI_STRING_T time_str = {0};
		    	MMI_STRING_T txt_temp = {0};

			event_delay_ptr = GetDelayEvent(event_id);
			if( PNULL == event_delay_ptr )
			{
				SCI_TRACE_LOW("DisplayAlarmListItem event_delay_ptr is PNULL");
				continue;
			}
			
			MMIAPIALM_FormInteEventInfo(event_id, &smart_event);
			//if(!smart_event.event_content_info.is_add && event_id != EVENT_AUTO_POWERON && event_id != EVENT_AUTO_POWEROFF)
        	//	{
            //			continue;
        	//	}

			is_on = smart_event.event_fast_info.is_on | event_delay_ptr->is_on;
			SCI_TRACE_LOW("DisplayAlarmListItem: is_on = %d", is_on);
			
			// alarmClock open or close
			if( TRUE == is_on )
			{
				iconId = IMAGE_ALARM_OPEN;
			}
			else
			{
				iconId = IMAGE_ALARM_CLOSE;
			}

			//alarmClock times
			if (event_id <= EVENT_ALARM_MAX)
        		{
        				SCI_TIME_T  alarmTime = { 0 };
		            	name_str.wstr_ptr = smart_event.event_content_info.w_content;
		            	name_str.wstr_len = smart_event.event_content_info.w_length;

						//time_str.wstr_ptr    = wTimeStr;
						//alarmTime.hour      = smart_event.event_fast_info.hour;
            			//alarmTime.min       = smart_event.event_fast_info.minute;
						//AlarmListGetTimeStr(&alarmTime, &time_str);
		            	MMIAPIALARM_GetEventTimeString(&smart_event.event_fast_info, TRUE, temp_wstr, GUILIST_STRING_MAX_NUM );
		            	time_str.wstr_ptr = temp_wstr;
		            	time_str.wstr_len = MMIAPICOM_Wstrlen(temp_wstr);

		            	connect_str.wstr_ptr = connect;
		            	connect_str.wstr_len = MMIAPICOM_Wstrlen(connect);

		            	MMIAPICOM_CatTwoString(&txt_temp,&time_str,&connect_str);
				SCI_TRACE_LOW("DisplayAlarmListItem: txt_temp.wstr_ptr = %ls, txt_temp.wstr_len = %d", txt_temp.wstr_ptr, txt_temp.wstr_len);
        		}

			//alarmClock repeatStr
			//repeatStr.wstr_ptr = wRepeatStr;
            //AlarmListGetRepeatStr(event_id, &repeatStr);
			SCI_MEMSET(&repeatStr,0,sizeof(MMI_STRING_T));
        	GetRepeatModeString(&repeatStr,smart_event.event_fast_info.fre_mode,smart_event.event_fast_info.fre);

			AlarmClock_AppendAlarmListItem(win_id, list_ctr_id, iconId, txt_temp, repeatStr, is_on);
			
			userData = (uint32)event_id;
			SCI_TRACE_LOW("DisplayAlarmListItem itemIndex = %d", itemIndex);
			SCI_TRACE_LOW("DisplayAlarmListItem userData = %d", userData);
			GUILIST_SetItemUserData( list_ctr_id, itemIndex, &userData );
			itemIndex++;
			
		}

		itemNum = GUILIST_GetTotalItemNum(list_ctr_id);
		if( curSelection >= itemNum )
		{
			curSelection = itemNum - 1;
		}
		GUILIST_SetCurItemIndex( list_ctr_id, curSelection );
		
	}else{
		GUI_RECT_T  emptyListRect = { 0, 0, 0, 0 };
		GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
		GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN;
		GUISTR_STYLE_T text_style = {0};
		MMI_STRING_T text_title = {0};
#ifdef ZTE_SUPPORT_240X284
		GUI_RECT_T icon_rect = {65, 60, 180, 180};
		GUI_RECT_T tips_rect = {40, 178, 200, 208};
#elif ZTE_SUPPORT_240X240
		GUI_RECT_T icon_rect = {65, 50, 180, 170};
		GUI_RECT_T tips_rect = {40, 168, 200, 198};
#endif

		GUILIST_SetRect( list_ctr_id, &emptyListRect );
		GUILIST_RemoveAllItems( list_ctr_id );
		
		GUIRES_DisplayImg(PNULL, &icon_rect, PNULL, win_id, IMAGE_ALARMCLOCK_ICON, &lcd_dev_info);
		
		text_style.align = ALIGN_HVMIDDLE;
		text_style.font = SONG_FONT_16;
		text_style.font_color = MMI_WHITE_COLOR;
		MMIRES_GetText(TXT_NO_ALARMCLOCK,win_id,&text_title);
		GUISTR_DrawTextToLCDInRect(
			(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
			&tips_rect,
			&tips_rect,
			&text_title,
			&text_style,
			text_state,
			GUISTR_TEXT_DIR_AUTO
			);

	}
	if(0)
	{
		GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
		GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN;
		GUISTR_STYLE_T text_style = {0};
		MMI_STRING_T text_title = {0};
		GUI_RECT_T add_rect = {12, 240, 228, 280};

		LCD_DrawRoundedRect(&lcd_dev_info, add_rect, add_rect, 0x52d8ff);
		LCD_FillRoundedRect(&lcd_dev_info, add_rect, add_rect, 0x52d8ff);

		text_style.align = ALIGN_HVMIDDLE;
		text_style.font = SONG_FONT_16;
		text_style.font_color = MMI_WHITE_COLOR;
		MMIRES_GetText(TXT_ADD_ALARMCLOCK,win_id,&text_title);
		GUISTR_DrawTextToLCDInRect(
			(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
			&add_rect,
			&add_rect,
			&text_title,
			&text_style,
			text_state,
			GUISTR_TEXT_DIR_AUTO
			);
	}

}

LOCAL void AlarmClockMain_Full_Paint(
	GUI_LCD_DEV_INFO lcd_dev_info,
	MMI_CTRL_ID_T ctrl_id,
	MMI_WIN_ID_T win_id
	)
{	
	GUI_RECT_T clientRect = {0,0,239,239};
	GUI_RECT_T win_rect = {0};
	GUI_RECT_T title_img_rect = {0};
	GUI_RECT_T title_rect = {0,0,239,45};//{0,0,240,45}
	GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN;
	GUISTR_STYLE_T text_style = {0};
	MMI_STRING_T text_title = {0};

	text_style.align = ALIGN_HVMIDDLE;//ALIGN_HMIDDLE;
	text_style.font = SONG_FONT_22;//SONG_FONT_26;
	text_style.font_color = MMI_WHITE_COLOR;//MMI_WHITE_COLOR;

	win_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, clientRect);
	GUI_FillRect(&lcd_dev_info, win_rect, MMI_BLACK_COLOR);

	//title_img_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,clientRect);
	//GUIRES_DisplayImg(NULL,&title_img_rect,NULL,win_id,IMAGE_THEME_BLACK_BG_SPEC,&lcd_dev_info);//IMAGE_BG_HEADER

	/*title_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,title_rect);
	MMIRES_GetText(TXT_ALARMCLOCK_APP_NAME,win_id,&text_title);
	GUISTR_DrawTextToLCDInRect(
		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		&title_rect,
		&title_rect,
		&text_title,
		&text_style,
		text_state,
		GUISTR_TEXT_DIR_AUTO
		);*/

	MMK_SetAtvCtrl(win_id, ctrl_id);
	GUILIST_RemoveAllItems(ctrl_id);
	GUILIST_SetMaxItem(ctrl_id,ALARMCLOCK_MAX_ITEM,FALSE);

	{
		uint8 eventNum = MMIALARMCLOCK_GetAlarmNum();
		SCI_TRACE_LOW("AlarmClockMain_Full_Paint eventNum = %d",eventNum);

		DisplayAlarmListItem(win_id,ctrl_id,eventNum);
	}
}
static int s_alarm_tp_x;
static int s_alarm_tp_y;
static BOOLEAN s_is_alarm_tp_down = FALSE;
#define z_abs(x)  ((x) > 0 ? (x) : (-(x)))

LOCAL void AlarmClockMain_APP_OK(
	MMI_WIN_ID_T win_id,
	MMI_CTRL_ID_T ctrl_id
	)
{
	uint16			index = 0;
	uint32          eventId = 0;
	BOOLEAN         reCode = FALSE;

	index = GUILIST_GetCurItemIndex(ctrl_id);
	SCI_TRACE_LOW("AlarmClockMainAppOk index = %d", index);

	eventId = GetAlarmRealId(index);
	SCI_TRACE_LOW("AlarmClockMainAppOk eventId = %d", eventId);

	if(ALM_ALARM_NUM > eventId)
	{
		MMIALARMCLOCK_CreateSelectWin(eventId);
	}
	else
	{
		SCI_TRACE_LOW("GUILIST_GetItemData error!");
	}

}

LOCAL MMI_RESULT_E HandleAlarmClockMainWin(
	MMI_WIN_ID_T win_id, 
	MMI_MESSAGE_ID_E msg_id,
	DPARAM param
	)
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	MMI_CTRL_ID_T ctrl_id = MMI_ALARMCLOCK_SETLIST_CTRL_ID;
	MMI_CheckAllocatedMemInfo();
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		{
			//GUIBUTTON_SetCallBackFunc(MMI_ALARMCLOCK_ADD_CLOCK_CTRL_ID,_AddAlarmClock_ButtonCallBack);
		}
		break;
	case MSG_FULL_PAINT:
		{
			AlarmClockMain_Full_Paint(lcd_dev_info,ctrl_id,win_id);
		}
		break;
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		{
			MMK_CloseWin(win_id);
		}
		break;
#ifdef TOUCH_PANEL_SUPPORT
			case MSG_TP_PRESS_DOWN:
				{
					GUI_POINT_T   point = {0};
					point.x = MMK_GET_TP_X(param);
					point.y = MMK_GET_TP_Y(param);
					s_alarm_tp_x = point.x;
					s_alarm_tp_y = point.y;
					s_is_alarm_tp_down = TRUE;
					
				}
				break;
				
			case MSG_TP_PRESS_UP:
				{
					GUI_POINT_T   point = {0};
					point.x = MMK_GET_TP_X(param);
					point.y = MMK_GET_TP_Y(param);
					if(point.x - s_alarm_tp_x > 120 && z_abs(point.y - s_alarm_tp_y) < 100)
					{
						MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
					}else if((point.x > 0&& point.x <42+43)&&(point.y > 0 &&	point.y < 45))
					{
						MMK_CloseWin(win_id);
						
					}else if((point.x > 0&& point.x < 240) && (point.y > 240 &&	point.y < 280))
					{
						//_AddAlarmClock_ButtonCallBack();
					}
				}
				break;
				
			case MSG_TP_PRESS_MOVE:
				{
					GUI_POINT_T   point = {0};
					point.x = MMK_GET_TP_X(param);
					point.y = MMK_GET_TP_Y(param);
					
				}
				break;
			case MSG_TP_PRESS_LONG:
				{
						
				}
				break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            
	case MSG_KEYDOWN_OK:
	case MSG_APP_WEB:
	case MSG_CTL_OK:
	case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT
	case MSG_CTL_PENOK:
#endif
		{
            //        uint8 eventNum = GetAlarmTotalNum();
			//SCI_TRACE_LOW("HandleAlarmClockMainWin MSG_APP_WEB");
            //       if(eventNum == 0)
            //        {
            //            MMIALARMCLOCK_CreateAddAlarmWin();
            //        }
			//AlarmClockMain_APP_OK(win_id,ctrl_id);
		}
		break;
	default:
		{
			recode = MMI_RESULT_FALSE;
		}
		break;
	}

	return recode;

}

WINDOW_TABLE(MMI_ALARMCLOCK_MAIN_WIN_TAB) = 
{
	WIN_ID(MMI_ALARMCLOCK_MAIN_WIN_ID),
	WIN_FUNC((uint32)HandleAlarmClockMainWin),
	WIN_HIDE_STATUS,
	//CREATE_BUTTON_CTRL(NULL,MMI_ALARMCLOCK_ADD_CLOCK_CTRL_ID),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMI_ALARMCLOCK_SETLIST_CTRL_ID),
	END_WIN
};

PUBLIC void MMIALARMCLOCK_CreateMainWin()
{
	MMI_WIN_ID_T win_id = MMI_ALARMCLOCK_MAIN_WIN_ID;
	MMI_HANDLE_T win_handle = 0;
#ifdef ZTE_SUPPORT_240X284
	GUI_RECT_T rect = {0,0,239,284};
#elif ZTE_SUPPORT_240X240
	GUI_RECT_T rect = {0,0,239,239};
#endif
	if (MMK_IsOpenWin(win_id))
	{
		MMK_CloseWin(win_id);
	}

	win_handle = MMK_CreateWin((uint32*)MMI_ALARMCLOCK_MAIN_WIN_TAB, NULL);
	MMK_SetWinRect(win_handle, &rect);

}

PUBLIC GUI_BG_T MMIALARMCLOCK_GETLISTBG(void)
{
	GUI_BG_T bg_info = {0};
	bg_info.bg_type = GUI_BG_IMG;
	bg_info.img_id = IMAGE_ITEM_LIST_BG;

	return bg_info;
}
