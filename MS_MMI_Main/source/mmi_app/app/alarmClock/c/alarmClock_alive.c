
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
#include "guibutton.h"
#include "guilistbox.h"
#include "mmk_timer.h"
#include "mmi_common.h"
#include "mmiacc_event.h"
#include "alarmClock_export.h"
#include "alarmClock_image.h"
#include "alarmClock_text.h"
#include "alarmClock_id.h"
#include "alarmClock_nv.h"
#include "mmialarm_export.h"
#include "mmischedule_export.h"
#include "mmicc_export.h"
#include "mmi_appmsg.h"
#include "mmikl_export.h"
#include "mmimms_export.h"
#include "mmiset_export.h"

LOCAL uint16 clock_alive_tp_x = 0;
LOCAL uint16 clock_alive_tp_y = 0;
LOCAL uint8  alarm_pen_timer_falg=0;
static void MMIAlarmClock_BackLight(BOOLEAN is_alway_on)
{
      if(is_alway_on)
      {
		MMIDEFAULT_SetAlwaysHalfOnBackLight(FALSE);
		MMIDEFAULT_AllowTurnOffBackLight(FALSE);
		MMIDEFAULT_TurnOnBackLight();
      }
      else
      {
		MMIDEFAULT_AllowTurnOffBackLight(TRUE);
		MMIDEFAULT_SetAlwaysHalfOnBackLight(FALSE);
      }
}

LOCAL MMI_RESULT_E _callBack_ClickDelayAlarmClock()
{
	MMI_WIN_ID_T win_id = MMI_ALARMCLOCK_ALARM_CLOCK_WIN_ID;
	SETTINGS_ALARM_ARRIVED_WIN* clock_event = {0};
	BOOLEAN is_click_delay = TRUE;

	clock_event = (SETTINGS_ALARM_ARRIVED_WIN *)MMK_GetWinAddDataPtr( win_id );

	if (NULL == clock_event)
	{
		SCI_TRACE_LOW("_callBack_ClickDelayAlarmClock arrived_event == NULL");
		return MMI_RESULT_FALSE;
	}

	MMI_WriteNVItem(MMINV_ALARMCLOCK_IS_DELAY_NV,&is_click_delay);

	StopRingOrVib(TRUE);
	SetDelayAlarmTime(clock_event->event_id, &clock_event->s_arrived_event, FALSE);
#ifdef CALENDAR_SUPPORT
	MMIAPISCH_DeleteUnreadEvent(clock_event->event_id);
#endif
	SCI_TRACE_LOW("_callBack_ClickDelayAlarmClock is_valid = %d  is_incall=%d sch_list_total=%d",clock_event->s_arrived_event.event_fast_info.is_valid,MMIAPICC_IsInState(CC_IN_CALL_STATE),clock_event->sch_list_total);
	if(((clock_event->s_arrived_event.event_fast_info.is_valid)
		&& !MMIAPICC_IsInState(CC_IN_CALL_STATE))
		&&(0==clock_event->sch_list_total))
	{
		CheckandStartExpiredEvent();
	}else
	{
		CheckandStartExpiredEvent();
	}
	MMK_CloseWin(win_id);
}

LOCAL MMI_RESULT_E HandleAlarmClockWinMsg_OPEN_WINDOW(MMI_WIN_ID_T win_id)
{
	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_RECT_T			clientRect = {0,0,239,239};
	GUI_RECT_T			win_rect = {0};
	MMI_CTRL_ID_T		ctrl_id = MMI_ALARMCLOCK_EXTEND_CTRL_ID;
	MMI_HANDLE_T		win_handle = 0;

	win_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, clientRect);
	GUI_FillRect(&lcd_dev_info, win_rect, MMI_BLACK_COLOR);
       MMIALARM_HandleWinOpen();
}

LOCAL MMI_RESULT_E HandleAlarmClockWinMsg_FULL_PAINT(MMI_WIN_ID_T win_id)
{
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
      GUI_POINT_T                 point               = {0};
	GUI_RECT_T img_rect = {35,20,239,170};
	GUI_POINT_T start_point = {0,175};
	GUI_POINT_T clock_point = {-5,85};
	MMI_STRING_T close_text = {0};
	MMI_STRING_T delay_text = {0};
	GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN;
	GUISTR_STYLE_T text_style = {0};
	MMI_STRING_T clock_text = {0};
	wchar clock_wchar[20] = {'1','2',':','0','0',0};

      GUIRES_DisplayImg(&point,PNULL,PNULL,MAIN_IDLE_WIN_ID, IMG_DIGIT_CLOCK0_BG,&lcd_dev_info);
      
	img_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,img_rect);
	GUIRES_DisplayImg(NULL,&img_rect,NULL,win_id,IMAGE_ALARM_ALERT2,&lcd_dev_info);

	
	text_style.align = ALIGN_HMIDDLE;
	text_style.font = SONG_FONT_28;
	text_style.font_color = MMI_YELLOW_COLOR;
	start_point.y = 195;
	MMIRES_GetText(TXT_ALARMCLOCK_SLIDE_CLOSE,win_id,&close_text);
	GUISTR_DrawTextToLCDSingleLine(
		&lcd_dev_info,
		&start_point,
		&close_text,
		&text_style,
		text_state
		);

	{

		GUI_RECT_T delay_rect = {20, 135, 220, 220};
		MMI_STRING_T delay_text = {0};

		text_style.align = ALIGN_HVMIDDLE;
		text_style.font = SONG_FONT_26;
		text_style.font_color = MMI_WHITE_COLOR;

		delay_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,delay_rect);
		MMIRES_GetText(TXT_ALARMCLOCK_CLICK_DELAY,win_id,&delay_text);
		GUISTR_DrawTextToLCDInRect(
			(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
			&delay_rect,
			&delay_rect,
			&delay_text,
			&text_style,
			text_state,
			GUISTR_TEXT_DIR_AUTO
		);
	}

	{
		SETTINGS_ALARM_ARRIVED_WIN* arrived_event = {0};
		MMI_STRING_T clock_text = {0};
		char clock_time[20] = {0};
		wchar clock_wchar[20] = {0};
		uint8 clock_hour = 0;
		uint8 clock_minute = 0;	

		arrived_event = (SETTINGS_ALARM_ARRIVED_WIN *)MMK_GetWinAddDataPtr( win_id );

		if (NULL == arrived_event)
		{
			SCI_TRACE_LOW("HandleAlarmClockWinMsg_FULL_PAINT NULL == arrived_event");
			return MMI_RESULT_FALSE;
		}

		SCI_TRACE_LOW("HandleAlarmClockWinMsg_FULL_PAINT event_fast_info.hour = %d",arrived_event->s_arrived_event.event_fast_info.hour);
		SCI_TRACE_LOW("HandleAlarmClockWinMsg_FULL_PAINT event_fast_info.minute = %d",arrived_event->s_arrived_event.event_fast_info.minute);
		clock_hour = arrived_event->s_arrived_event.event_fast_info.hour;
		clock_minute = arrived_event->s_arrived_event.event_fast_info.minute;

		MMIAPISET_FormatTimeStrByTime(clock_hour,clock_minute,(uint8*)clock_time,MMIALM_TIME_STR_12HOURS_LEN + 1);

		//sprintf(clock_time,"%d:%d",arrived_event->s_arrived_event.event_fast_info.hour,arrived_event->s_arrived_event.event_fast_info.minute);
		SCI_TRACE_LOW("HandleAlarmClockWinMsg_FULL_PAINT clock_time = %s",clock_time);

		clock_text.wstr_ptr = MMIAPICOM_StrToWstr((uint8 *)clock_time,clock_wchar);
		clock_text.wstr_len = MMIAPICOM_Wstrlen(clock_text.wstr_ptr);
		SCI_TRACE_LOW("HandleAlarmClockWinMsg_FULL_PAINT clock_text.wstr_ptr = %ls",clock_text.wstr_ptr);
		text_style.font_color = MMI_BLACK_COLOR;
		text_style.font = SONG_FONT_22;
		GUISTR_DrawTextToLCDSingleLine(
			&lcd_dev_info,
			&clock_point,
			&clock_text,
			&text_style,
			text_state
			);

	}

	return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E HandleAlarmClockWinMsg_Close_Clock(MMI_WIN_ID_T win_id)
{
	SETTINGS_ALARM_ARRIVED_WIN* clock_event = {0};
	uint16 event_id = 0;
	BOOLEAN is_click_delay = FALSE;

	clock_event = (SETTINGS_ALARM_ARRIVED_WIN *)MMK_GetWinAddDataPtr( win_id );

	if (NULL == clock_event)
	{
		SCI_TRACE_LOW("HandleAlarmClockWinMsg_CLOSE_WINDOW NULL == clock_event");
		return MMI_RESULT_FALSE;
	}

	if (clock_event->event_id < EVENT_SCH_0)
	{
		if(clock_event->event_id <= EVENT_ALARM_MAX)
		{
			event_id = clock_event->event_id;
		}else if(clock_event->event_id >= EVENT_ALARM_DELAY_0 && clock_event->event_id <= EVENT_ALARM_DELAY_MAX)
		{
			event_id = clock_event->event_id - EVENT_ALARM_DELAY_0; 
		}
#ifdef CALENDAR_SUPPORT
		MMIAPISCH_DeleteUnreadEvent(event_id);
#endif            
		StopRingOrVib(TRUE);

		MMI_ReadNVItem(MMINV_ALARMCLOCK_IS_DELAY_NV,&is_click_delay);
		SCI_TRACE_LOW("HandleAlarmClockWinMsg_CLOSE_WINDOW is_click_delay = %d",is_click_delay);
	
		if (!is_click_delay)
		{
			CloseDelayAlarm(clock_event->event_id, clock_event->s_arrived_event.event_fast_info, FALSE);
		}
		is_click_delay = FALSE;
		MMI_WriteNVItem(MMINV_ALARMCLOCK_IS_DELAY_NV,&is_click_delay);

		if(((clock_event->s_arrived_event.event_fast_info.is_valid) 
			&& !MMIAPICC_IsInState(CC_IN_CALL_STATE) 
			&& !MMIAPIMMS_IsSendingOrRecving())
			&&(0 == clock_event->sch_list_total)
			)
		{
			CheckandStartExpiredEvent();
			MMK_CloseWin(win_id);
		}else
		{
			CheckandStartExpiredEvent();
			MMK_CloseWin(win_id);
		}

	}

}

PUBLIC MMI_RESULT_E HandleAlarmClockWinMsg(
	MMI_WIN_ID_T      win_id,
	MMI_MESSAGE_ID_E  msg_id, 
	DPARAM            param
	)
{
	MMI_RESULT_E    result = MMI_RESULT_TRUE;
	
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		{
                   MMIAlarmClock_BackLight(TRUE);
			HandleAlarmClockWinMsg_OPEN_WINDOW(win_id);
		}
		break;
	case MSG_FULL_PAINT:
		{
			HandleAlarmClockWinMsg_FULL_PAINT(win_id);
		}
		break;
        
    case MSG_ALM_START_RING:
        StartRingOrVib();
        break;

	case MSG_APP_WEB:
	case MSG_APP_OK:
	case MSG_APP_BACKWARD:
	case MSG_APP_GREEN:
		_callBack_ClickDelayAlarmClock();
		MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        break;
        
	case MSG_APP_RED:
	case MSG_APP_CANCEL:
		{
			SCI_TRACE_LOW("HandleAlarmClockWinMsg MSG_APP_CANCEL");
			MMIDEFAULT_AllowTurnOffBackLight(TRUE);
			HandleAlarmClockWinMsg_Close_Clock(win_id);
			MMK_CloseWin(win_id);
			MMK_CloseInsertSimWin();//xx_digit_bug
		}
		break;
        
	case MSG_KEYUP_RED:
             break;
        
	case MSG_CLOSE_WINDOW:
		{
			MMK_CloseInsertSimWin();//xx_digit_bug
			MMIALARM_HandleWinClose(win_id);
                   MMIAlarmClock_BackLight(FALSE);
			SCI_TRACE_LOW("HandleAlarmClockWinMsg MSG_CLOSE_WINDOW");
		}
		break;
	case MSG_TP_PRESS_DOWN:
		{
			GUI_POINT_T   point = {0};
	    		point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			clock_alive_tp_x = point.x;
			clock_alive_tp_y = point.y;
                   alarm_pen_timer_falg = 1;
		}
		break;
	case MSG_TP_PRESS_UP:
		{
			BOOLEAN from_list = TRUE;
			GUI_POINT_T   point = {0};
                   int offset_x = 0;
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
                    if(alarm_pen_timer_falg)
                    {
                        offset_x = clock_alive_tp_x - point.x;
                        if(offset_x <= -20)	
                        {
                            MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                        }	
                        else if(offset_x >= 20)
                        {
                            MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                        }
                        else
                        {
                            _callBack_ClickDelayAlarmClock();
                        }
                    }
                    alarm_pen_timer_falg = 0;
		}
		break;
        
    case MSG_GET_FOCUS:
            MMIAlarmClock_BackLight(TRUE);
            StartRingOrVib();
        break;
        
    case MSG_LOSE_FOCUS:
            MMIAlarmClock_BackLight(FALSE);
            StopRingOrVib(FALSE);
        break;

    case MSG_TIMER:
        {
            result = MMIALARM_HandleTimerExt(win_id,*( GUI_TIMER_ID_T*)param);
        }        
        break;
	default:
		{
			result = MMI_RESULT_FALSE;
		}
		break;
	}

	return result;
}

PUBLIC void MMI_CloseALARMCLOCKWin(void )
{
	SCI_TRACE_LOW("ALARMCLOCK MMI_CloseALARMCLOCKWin ");
    MMK_CloseWin(MMI_ALARMCLOCK_ALARM_CLOCK_WIN_ID);
	MMK_CloseInsertSimWin();//xx_digit_bug
}

PUBLIC BOOLEAN MMI_IsOpenALARMCLOCKWin(void)
{
    BOOLEAN ret = FALSE;
    if(MMK_IsOpenWin(MMI_ALARMCLOCK_ALARM_CLOCK_WIN_ID))
    {
        ret = TRUE;
    }
	SCI_TRACE_LOW("ALARMCLOCK MMI_IsOpenALARMCLOCKWin %d",ret);
    return ret;
}

PUBLIC void MMIALARMCLOCK_CreateAliveWin( SETTINGS_ALARM_ARRIVED_WIN arrived_event, MMI_WIN_PRIORITY_E  win_priority)
{
      MMI_WINDOW_CREATE_T win_create = {0};
	SETTINGS_ALARM_ARRIVED_WIN* pWinData = (SETTINGS_ALARM_ARRIVED_WIN*)SCI_ALLOC_APPZ( sizeof(SETTINGS_ALARM_ARRIVED_WIN) );

	if( PNULL == pWinData )
	{
		SCI_TRACE_LOW("MMIALARMCLOCK_CreateAliveWin pWinData is PNULL!!");
		return;
	}

			if((MMIAPICC_IsInState(CC_MT_CONNECTING_STATE) && MMK_IsOpenWin(MMICC_ANIMATION_WIN_ID)) 
				|| MMIAPICC_IsInState(CC_MO_CONNECTING_STATE) || MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE)||YX_DB_TIMER_IsValid_Slient()
	#ifdef  ZDT_VIDEOCHAT_SUPPORT
				|| MMIVIDEO_IsOpenVideoWin()
	#endif 	
				){
				SCI_TRACE_LOW("%s: not allow clock ring!!", __FUNCTION__);
				SCI_FREE(pWinData);
				pWinData = NULL;
				return ;
			}


	*pWinData = arrived_event;
	SCI_TRACE_LOW("MMIALARMCLOCK_CreateAliveWin pWinData->event_id = %d",pWinData->event_id);
	SCI_TRACE_LOW("MMIALARMCLOCK_CreateAliveWin event_clock_time = %ls",pWinData->event_clock_time.wstr_ptr);
			
	win_create.applet_handle = MMK_GetFirstAppletHandle();
	win_create.win_id        = MMI_ALARMCLOCK_ALARM_CLOCK_WIN_ID;
	win_create.func          = HandleAlarmClockWinMsg;
	win_create.win_priority  = win_priority;
	win_create.add_data_ptr  = (ADD_DATA)pWinData;

	MMK_CreateWindow(&win_create);
       
	//turn on backlight,use after create win
	#if 0	
	{
		MMI_WIN_ID_T win_id = MMI_ALARMCLOCK_ALARM_CLOCK_WIN_ID;
		MMI_HANDLE_T win_handle = MMK_ConvertIdToHandle(win_id);

		MMIPUB_HandleWaitWinMsg(win_id, MSG_FULL_PAINT, PNULL);
				
		if (STARTUP_ALARM != MMIAPIPHONE_GetStartUpCondition())
		{
			if( MMK_GetWinPriority(win_handle) != WIN_LOWEST_LEVEL)
			{
				MMIDEFAULT_TurnOnBackLight();
				MMIDEFAULT_AllowTurnOffBackLight(FALSE);
			}
		}else
		{
			MMIDEFAULT_TurnOnBackLight();
			MMIDEFAULT_AllowTurnOffBackLight(FALSE);
		}

		if(MMIAPIKL_IsPhoneLocked())
		{
			MMK_PostMsg(win_id,MSG_ALM_START_RING,PNULL,0);//解决来电挂断铃声起不来问题
		}else
		{
			StartRingOrVib();
		}
	}
	#endif
}
