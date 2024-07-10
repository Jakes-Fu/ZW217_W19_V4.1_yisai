
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

#include "pedometer_export.h"
#include "pedometer_id.h"
#include "pedometer_image.h"
#include "pedometer_text.h"
#include "pedometer_nv.h"
#include "msensor_drv.h"
#include "mmi_common.h"
#include "zdt_app.h"

LOCAL uint8 timer_id = 0;
LOCAL int16 pedometer_today_tp_down_x = 0;
LOCAL int16 pedometer_today_tp_down_y =0;

extern BOOLEAN MMIZDT_IsInChargingWin();

LOCAL void PedometerTodayWin_showStepBg(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO lcd_dev_info, uint32 target_step, uint32 step)
{
	GUI_POINT_T point = {120, 112};
	GUI_RECT_T clicp_rect = {0, 0, 239, 239};
	int16 angle = 0;
	float percent = 0.00;
	
	LCD_FillArc(&lcd_dev_info, &clicp_rect, point.x, point.y, 60, 0, 360, GUI_RGB2RGB565(0x24,0x24,0x24));
	//SCI_TRACE_LOW("step = %d, target_step = %d", step, target_step);
	if(step >= target_step){
		LCD_FillArc(&lcd_dev_info, &clicp_rect, point.x, point.y, 59, 0, 360, GUI_RGB2RGB565(0x0b,0x84,0xff));
	}else if(step * 4 <= target_step){
		percent = (float)step / ((float)target_step/4);
		SCI_TRACE_LOW("01percent = %f", percent);
		angle = 90 - 90 * percent;
		LCD_FillArc(&lcd_dev_info, &clicp_rect, point.x, point.y, 59, angle, 90, GUI_RGB2RGB565(0x0b,0x84,0xff));
	}else{
		percent = (float)step / (float)target_step;
		SCI_TRACE_LOW("02percent = %f", percent);
		angle = 360 - 270 * percent;
		LCD_FillArc(&lcd_dev_info, &clicp_rect, point.x, point.y, 59, angle, 90, GUI_RGB2RGB565(0x0b,0x84,0xff));
	}
	LCD_FillArc(&lcd_dev_info, &clicp_rect, point.x, point.y, 51, 0, 360, GUI_RGB2RGB565(0x24,0x24,0x24));
	LCD_FillArc(&lcd_dev_info, &clicp_rect, point.x, point.y, 50, 0, 360, MMI_BLACK_COLOR);
}

LOCAL void PedometerTodayWin_fullPaint(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO lcd_dev_info)
{
	wchar txt_str[30] = {0};
	char step_str[30] = {0};
	uint32 target_step = 1000;
	int new_step = 0;
	float km = 0;
	float cal = 0;
	MMI_STRING_T text_str = {0};
	MMI_STRING_T text_string = {0};
	GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN;
	GUISTR_STYLE_T text_style = {0};
	GUI_RECT_T title_rect = {12, 20, 100, 40};
	GUI_RECT_T count_bg_rect = {66,58,174,166};
	GUI_RECT_T count_img_rect = {66,58,174,166};
	GUI_RECT_T km_cal_rect = {60,179,180,199};
	GUI_RECT_T week_bg_rect = {28, 210, 220, 260};

	text_style.align = ALIGN_LVMIDDLE;
	text_style.font = SONG_FONT_16;
	text_style.font_color = MMI_WHITE_COLOR;
	MMIRES_GetText(TXT_PEDOMETER_COUNT,win_id,&text_string);
	GUISTR_DrawTextToLCDInRect(
		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		&title_rect,
		&title_rect,
		&text_string,
		&text_style,
		text_state,
		GUISTR_TEXT_DIR_AUTO
		);

#ifdef WIN32
	new_step = 100;
#else
	new_step = 2800;//ZDT_GSensor_GetStepOneDay();
#endif

	PedometerTodayWin_showStepBg(win_id, lcd_dev_info, target_step, new_step);
	
	itoa(new_step,step_str,10);
	text_string.wstr_ptr = MMIAPICOM_StrToWstr((uint8 *)step_str,txt_str);
	text_string.wstr_len = MMIAPICOM_Wstrlen(text_string.wstr_ptr);
	count_bg_rect.bottom -= 30;
	text_style.align = ALIGN_HVMIDDLE;
	text_style.font = SONG_FONT_20;
	text_style.font_color = GUI_RGB2RGB565(0x0b, 0x84, 0xff);
	GUISTR_DrawTextToLCDInRect(
		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		&count_bg_rect,
		&count_bg_rect,
		&text_string,
		&text_style,
		text_state,
		GUISTR_TEXT_DIR_AUTO
		);

	MMIAPICOM_CustomGenerateNumReplaceStrByTextId(TXT_PEDOMETER_TARGET, L"%N", target_step, 50, &text_string);
	count_bg_rect.top += 50;
	text_style.font_color = 0x7BEF;
	text_style.font = SONG_FONT_16;
	GUISTR_DrawTextToLCDInRect(
		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		&count_bg_rect,
		&count_bg_rect,
		&text_string,
		&text_style,
		text_state,
		GUISTR_TEXT_DIR_AUTO
		);

	memset(&step_str, 0, 30);
	memset(&txt_str, 0, 30);
	km = (float)(new_step * 0.73) / 1000.00;
	sprintf(step_str, "%.2f¹«Àï", km);
	GUI_GBToWstr(txt_str, step_str, strlen(step_str));
	text_string.wstr_ptr = txt_str;
	text_string.wstr_len = MMIAPICOM_Wstrlen(text_string.wstr_ptr);
	text_style.align = ALIGN_LVMIDDLE;
	text_style.font = SONG_FONT_12;
	text_style.font_color = MMI_WHITE_COLOR;
	GUISTR_DrawTextToLCDInRect(
		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		&km_cal_rect,
		&km_cal_rect,
		&text_string,
		&text_style,
		text_state,
		GUISTR_TEXT_DIR_AUTO
		);

	memset(&step_str, 0, 30);
	memset(&txt_str, 0, 30);
	cal	= (float)(new_step * 24) / 1000.00;
	sprintf(step_str, "%.2fÇ§¿¨", cal);
	GUI_GBToWstr(txt_str, step_str, strlen(step_str));
	text_string.wstr_ptr = txt_str;
	text_string.wstr_len = MMIAPICOM_Wstrlen(text_string.wstr_ptr);
	text_style.align = ALIGN_RVMIDDLE;
	GUISTR_DrawTextToLCDInRect(
		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		&km_cal_rect,
		&km_cal_rect,
		&text_string,
		&text_style,
		text_state,
		GUISTR_TEXT_DIR_AUTO
		);

	GUIRES_DisplayImg(PNULL, &week_bg_rect, PNULL, win_id, IMAGE_PEDOMETER_WEEK_BG, &lcd_dev_info);
	week_bg_rect.left = 55;
	week_bg_rect.top = 212;
	GUIRES_DisplayImg(PNULL, &week_bg_rect, PNULL, win_id, IMAGE_PEDOMETER_TOTAL, &lcd_dev_info);

	text_style.align = ALIGN_LEFT;
	text_style.font = SONG_FONT_20;
	week_bg_rect.left = 100;
	week_bg_rect.top = 226;
	MMIRES_GetText(TXT_PEDOMETER_WEEK_STEP,win_id,&text_string);
	GUISTR_DrawTextToLCDInRect(
		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		&week_bg_rect,
		&week_bg_rect,
		&text_string,
		&text_style,
		text_state,
		GUISTR_TEXT_DIR_AUTO
		);
}

LOCAL void PedometerTodayRefresh(uint8 time_id, void * param)
{
	MMK_SendMsg(MMI_PEDOMETER_TODAY_EXERCISE_WIN_ID, MSG_FULL_PAINT, PNULL);
}

LOCAL MMI_RESULT_E HandlePedometerTodayWin( 
	MMI_WIN_ID_T win_id, 
	MMI_MESSAGE_ID_E msg_id,
	DPARAM param)
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	MMI_CheckAllocatedMemInfo();

	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		{
			
		}
		break;
	case MSG_GET_FOCUS:
		{
			if(timer_id != 0){
				MMK_StopTimer(timer_id);
				timer_id = 0;
			}
			if(!MMIZDT_IsInChargingWin()){
				timer_id = MMK_CreateTimerCallback(1500, PedometerTodayRefresh, PNULL, TRUE);
			}
		}
		break;
	case MSG_FULL_PAINT:
		{
		#ifdef ZTE_SUPPORT_240X284
			GUI_RECT_T win_rect = {0,0,240,284};
		#elif
			GUI_RECT_T win_rect = {0,0,240,240};
		#endif
			GUI_FillRect(&lcd_dev_info, win_rect, MMI_BLACK_COLOR);

			PedometerTodayWin_fullPaint(win_id, lcd_dev_info);
		}
		break;
	case MSG_CLOSE_WINDOW:
		{
			if(timer_id != 0){
				MMK_StopTimer(timer_id);
				timer_id = 0;
			}
		}
		break;
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		{
			MMK_CloseWin(win_id);
			if(timer_id != 0){
				MMK_StopTimer(timer_id);
				timer_id = 0;
			}
		}
		break;
	case MSG_TP_PRESS_DOWN:
		{
			GUI_POINT_T   point = {0};
	    		point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			pedometer_today_tp_down_x = point.x;
			pedometer_today_tp_down_y = point.y;
		}
		break;
	case MSG_TP_PRESS_UP:
		{
			GUI_POINT_T   point = {0};
			uint16 tp_offset_x = 0;
			uint16 tp_offset_y = 0;
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			tp_offset_x = point.x - pedometer_today_tp_down_x;
			tp_offset_y = point.y - pedometer_today_tp_down_y;
			if(ABS(tp_offset_x) > ABS(tp_offset_y))
			{
				if((point.x - pedometer_today_tp_down_x) > 60  && tp_offset_x > 0)
				{
				     MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
				}
			}
			if((point.x > 20 && point.x < 230) && (point.y > 210 && point.y < 260))
			{
				MMIPEDOMETER_CreateStepWeekWin();
			}
		}
		break;
	default:
		break;
	}

	return recode;

}

WINDOW_TABLE(MMI_PEDOMETER_TODAY_WIN_TAB) = 
{
	WIN_ID(MMI_PEDOMETER_TODAY_EXERCISE_WIN_ID),
	WIN_FUNC((uint32)HandlePedometerTodayWin),
	WIN_HIDE_STATUS,
	END_WIN
};

PUBLIC void MMIPEDOMETER_CreateTodayWin()
{
	MMI_WIN_ID_T win_id = MMI_PEDOMETER_TODAY_EXERCISE_WIN_ID;
	MMI_HANDLE_T win_handle = 0;
	GUI_RECT_T rect = {0,0,240,240};
	if (MMK_IsOpenWin(win_id))
	{
		MMK_CloseWin(win_id);
	}

	win_handle = MMK_CreateWin((uint32*)MMI_PEDOMETER_TODAY_WIN_TAB, NULL);
	MMK_SetWinRect(win_handle, &rect);
}
