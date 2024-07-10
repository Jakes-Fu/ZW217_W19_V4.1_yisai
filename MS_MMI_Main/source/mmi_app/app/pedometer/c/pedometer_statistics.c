
#include "std_header.h"
#include <stdlib.h>
#include "window_parse.h"
#include "sci_types.h"
#include "mmidisplay_data.h"
#include "mmiacc_text.h"
#include "mmipub.h"
#include "mmicc_id.h"
#include "mmi_modu_main.h"
//#include "mmi_adapt_screen.h"
#include "mmi_textfun.h"
#include "mmi_string.h"
#include "guilcd.h"
#include "guifont.h"
#include "guistring.h"
#include "guitext.h"
#include "guibutton.h"
//#include "watch_common.h"
#include "pedometer_export.h"
#include "pedometer_id.h"
#include "pedometer_image.h"
#include "pedometer_text.h"
#include "pedometer_nv.h"
#include "msensor_drv.h"
#include "mmi_common.h"
//#include "zdt_app.h"

#define MMI_PEDO_YELLOW_COLOR		0xff00 //步数颜色

extern BOOLEAN MMIZDT_IsInChargingWin();

LOCAL uint8 timer_id = 0;
LOCAL int16 pedometer_statis_tp_down_x = 0;
LOCAL int16 pedometer_statis_tp_down_y =0;

LOCAL BOOLEAN is_first_open = TRUE;

LOCAL GUI_COLOR_T statistics_color[] = 
{
		0xff8bff03,

		0xfffebe16,//1
		0xff88ff38,//2
		0xffff7800,//3
		0xff2096e6,//4
		0xffff352c,//5
		0xffc01d2e,//6
		0xff34c16e,//7

		0xff8bff03
};

GUI_COLOR_T pros_bar_color = GUI_RGB2RGB565(0x0b, 0x84, 0xff);

LOCAL int click_num = 0;//点击哪一天

LOCAL int max_num = 0;//7天中最高一天的步数

LOCAL int getLargestInteger(float f)//返回f的四舍五入整数值
{
	return (int)((f)>0.0? (f+0.5):(f-0.5));
}

LOCAL void getMaxNumOfSeven(int num1,int num2,int num3,int num4,int num5,int num6,int num7)
{
	max_num = (num1>num2)?num1:num2;
	max_num = (max_num>num3)?max_num:num3;
	max_num = (max_num>num4)?max_num:num4;
	max_num = (max_num>num5)?max_num:num5;
	max_num = (max_num>num6)?max_num:num6;
	max_num = (max_num>num7)?max_num:num7;
}


LOCAL void setButtonControl(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO dev_info_ptr)
{
#ifdef ZTE_SUPPORT_240X284
	GUI_RECT_T button1_rect = {6, 240, 39, 283};
	GUI_RECT_T button2_rect = {39, 240, 72, 283};
	GUI_RECT_T button3_rect = {72, 240, 105, 283};
	GUI_RECT_T button4_rect = {105, 240, 138, 283};
	GUI_RECT_T button5_rect = {138, 240, 172, 283};
	GUI_RECT_T button6_rect = {172, 240, 206, 283};
	GUI_RECT_T button7_rect = {206, 240, 240, 283};
	GUI_RECT_T split_rect = {12, 235, 228, 240};
	GUIRES_DisplayImg(PNULL, &split_rect, PNULL, win_id, IMAGE_PEDOMETER_WEEK_SPLIT, &dev_info_ptr);
#elif ZTE_SUPPORT_240X240
	GUI_RECT_T button1_rect = {6, 200, 39, 240};
	GUI_RECT_T button2_rect = {39, 200, 72, 240};
	GUI_RECT_T button3_rect = {72, 200, 105, 240};
	GUI_RECT_T button4_rect = {105, 200, 138, 240};
	GUI_RECT_T button5_rect = {138, 200, 172, 240};
	GUI_RECT_T button6_rect = {172, 200, 206, 240};
	GUI_RECT_T button7_rect = {206, 200, 240, 240};
	GUI_RECT_T split_rect = {12, 195, 228, 200};
	GUIRES_DisplayImg(PNULL, &split_rect, PNULL, win_id, IMAGE_PEDOMETER_WEEK_SPLIT, &dev_info_ptr);
#endif
	
	button1_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,button1_rect);
	button2_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,button2_rect);
	button3_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,button3_rect);
	button4_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,button4_rect);
	button5_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,button5_rect);
	button6_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,button6_rect);
	button7_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,button7_rect);
	GUIBUTTON_SetRect(MMI_PEDOMETER_WEEK_1_CTRL_ID,&button7_rect);
	GUIBUTTON_SetRect(MMI_PEDOMETER_WEEK_2_CTRL_ID,&button6_rect);
	GUIBUTTON_SetRect(MMI_PEDOMETER_WEEK_3_CTRL_ID,&button5_rect);
	GUIBUTTON_SetRect(MMI_PEDOMETER_WEEK_4_CTRL_ID,&button4_rect);
	GUIBUTTON_SetRect(MMI_PEDOMETER_WEEK_5_CTRL_ID,&button3_rect);
	GUIBUTTON_SetRect(MMI_PEDOMETER_WEEK_6_CTRL_ID,&button2_rect);
	GUIBUTTON_SetRect(MMI_PEDOMETER_WEEK_7_CTRL_ID,&button1_rect);
	
	{
		GUI_FONT_ALL_T font_type = {0};
		wchar text_day1[10] = {0};
		wchar text_day2[10] = {0};
		wchar text_day3[10] = {0};
		wchar text_day4[10] = {0};
		wchar text_day5[10] = {0};
		wchar text_day6[10] = {0};
		wchar text_day7[10] = {0};
		int i = 0;
		BOOLEAN is_get_day = TRUE;

		getTextDay(text_day1,0,is_get_day);
		getTextDay(text_day2,1,is_get_day);
		getTextDay(text_day3,2,is_get_day);
		getTextDay(text_day4,3,is_get_day);
		getTextDay(text_day5,4,is_get_day);
		getTextDay(text_day6,5,is_get_day);
		getTextDay(text_day7,6,is_get_day);

		font_type.color = MMI_GRAY_COLOR;
		font_type.font = SONG_FONT_16;
		GUIBUTTON_SetFont(MMI_PEDOMETER_WEEK_1_CTRL_ID,&font_type);
		GUIBUTTON_SetFont(MMI_PEDOMETER_WEEK_2_CTRL_ID,&font_type);
		GUIBUTTON_SetFont(MMI_PEDOMETER_WEEK_3_CTRL_ID,&font_type);
		GUIBUTTON_SetFont(MMI_PEDOMETER_WEEK_4_CTRL_ID,&font_type);
		GUIBUTTON_SetFont(MMI_PEDOMETER_WEEK_5_CTRL_ID,&font_type);
		GUIBUTTON_SetFont(MMI_PEDOMETER_WEEK_6_CTRL_ID,&font_type);
		GUIBUTTON_SetFont(MMI_PEDOMETER_WEEK_7_CTRL_ID,&font_type);
		font_type.color = MMI_WHITE_COLOR;
		if(click_num == 1)
		{
			GUIBUTTON_SetFont(MMI_PEDOMETER_WEEK_2_CTRL_ID,&font_type);
		}else if(click_num == 2)
		{
			GUIBUTTON_SetFont(MMI_PEDOMETER_WEEK_3_CTRL_ID,&font_type);
		}else if(click_num == 3)
		{
			GUIBUTTON_SetFont(MMI_PEDOMETER_WEEK_4_CTRL_ID,&font_type);
		}else if(click_num == 4)
		{
			GUIBUTTON_SetFont(MMI_PEDOMETER_WEEK_5_CTRL_ID,&font_type);
		}else if(click_num == 5)
		{
			GUIBUTTON_SetFont(MMI_PEDOMETER_WEEK_6_CTRL_ID,&font_type);
		}else if(click_num == 6)
		{
			GUIBUTTON_SetFont(MMI_PEDOMETER_WEEK_7_CTRL_ID,&font_type);
		}else //click_num = 0
		{
			GUIBUTTON_SetFont(MMI_PEDOMETER_WEEK_1_CTRL_ID,&font_type);
		}


		/*GUIBUTTON_SetText(MMI_PEDOMETER_WEEK_1_CTRL_ID,text_day1,MMIAPICOM_Wstrlen(text_day1));
		GUIBUTTON_SetText(MMI_PEDOMETER_WEEK_2_CTRL_ID,text_day2,MMIAPICOM_Wstrlen(text_day2));
		GUIBUTTON_SetText(MMI_PEDOMETER_WEEK_3_CTRL_ID,text_day3,MMIAPICOM_Wstrlen(text_day3));
		GUIBUTTON_SetText(MMI_PEDOMETER_WEEK_4_CTRL_ID,text_day4,MMIAPICOM_Wstrlen(text_day4));
		GUIBUTTON_SetText(MMI_PEDOMETER_WEEK_5_CTRL_ID,text_day5,MMIAPICOM_Wstrlen(text_day5));
		GUIBUTTON_SetText(MMI_PEDOMETER_WEEK_6_CTRL_ID,text_day6,MMIAPICOM_Wstrlen(text_day6));
		GUIBUTTON_SetText(MMI_PEDOMETER_WEEK_7_CTRL_ID,text_day7,MMIAPICOM_Wstrlen(text_day7));*/

		{
			SCI_DATE_T  sys_date = {0};
			TM_GetSysDate(&sys_date);
			if(sys_date.wday == 0){
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_1_CTRL_ID, TXT_PEDOMETER_7);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_2_CTRL_ID, TXT_PEDOMETER_6);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_3_CTRL_ID, TXT_PEDOMETER_5);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_4_CTRL_ID, TXT_PEDOMETER_4);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_5_CTRL_ID, TXT_PEDOMETER_3);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_6_CTRL_ID, TXT_PEDOMETER_2);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_7_CTRL_ID, TXT_PEDOMETER_1);	
			}else if(sys_date.wday == 1){
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_1_CTRL_ID, TXT_PEDOMETER_1);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_2_CTRL_ID, TXT_PEDOMETER_2);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_3_CTRL_ID, TXT_PEDOMETER_3);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_4_CTRL_ID, TXT_PEDOMETER_4);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_5_CTRL_ID, TXT_PEDOMETER_5);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_6_CTRL_ID, TXT_PEDOMETER_6);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_7_CTRL_ID, TXT_PEDOMETER_7);	
			}else if(sys_date.wday == 2){
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_1_CTRL_ID, TXT_PEDOMETER_2);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_2_CTRL_ID, TXT_PEDOMETER_3);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_3_CTRL_ID, TXT_PEDOMETER_4);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_4_CTRL_ID, TXT_PEDOMETER_5);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_5_CTRL_ID, TXT_PEDOMETER_6);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_6_CTRL_ID, TXT_PEDOMETER_7);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_7_CTRL_ID, TXT_PEDOMETER_1);	
			}else if(sys_date.wday == 3){
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_1_CTRL_ID, TXT_PEDOMETER_3);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_2_CTRL_ID, TXT_PEDOMETER_4);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_3_CTRL_ID, TXT_PEDOMETER_5);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_4_CTRL_ID, TXT_PEDOMETER_6);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_5_CTRL_ID, TXT_PEDOMETER_7);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_6_CTRL_ID, TXT_PEDOMETER_1);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_7_CTRL_ID, TXT_PEDOMETER_2);
			}else if(sys_date.wday == 4){
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_1_CTRL_ID, TXT_PEDOMETER_4);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_2_CTRL_ID, TXT_PEDOMETER_5);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_3_CTRL_ID, TXT_PEDOMETER_6);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_4_CTRL_ID, TXT_PEDOMETER_7);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_5_CTRL_ID, TXT_PEDOMETER_1);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_6_CTRL_ID, TXT_PEDOMETER_2);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_7_CTRL_ID, TXT_PEDOMETER_3);
			}else if(sys_date.wday == 5){
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_1_CTRL_ID, TXT_PEDOMETER_5);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_2_CTRL_ID, TXT_PEDOMETER_4);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_3_CTRL_ID, TXT_PEDOMETER_3);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_4_CTRL_ID, TXT_PEDOMETER_2);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_5_CTRL_ID, TXT_PEDOMETER_1);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_6_CTRL_ID, TXT_PEDOMETER_7);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_7_CTRL_ID, TXT_PEDOMETER_6);	
			}else if(sys_date.wday == 6){
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_1_CTRL_ID, TXT_PEDOMETER_6);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_2_CTRL_ID, TXT_PEDOMETER_5);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_3_CTRL_ID, TXT_PEDOMETER_4);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_4_CTRL_ID, TXT_PEDOMETER_3);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_5_CTRL_ID, TXT_PEDOMETER_2);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_6_CTRL_ID, TXT_PEDOMETER_1);
				GUIBUTTON_SetTextId(MMI_PEDOMETER_WEEK_7_CTRL_ID, TXT_PEDOMETER_7);	
			}
		}
	}


}

LOCAL void setTextControl(MMI_WIN_ID_T win_id)
{
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	SCI_DATE_T  sys_date = {0};
	uint8 day = 0;
	int step_num = 0;
	int	new_day_step = 0;
	int day1_step = 0;
	int day2_step = 0;
	int day3_step = 0;
	int day4_step = 0;
	int day5_step = 0;
	int day6_step = 0;
	int day7_step = 0;

	//先把截至今天存储的7条数据读出
	MMI_ReadNVItem(MMINV_PEDOMETER_DAY1_STEP_NV,&day1_step);
	MMI_ReadNVItem(MMINV_PEDOMETER_DAY2_STEP_NV,&day2_step);
	MMI_ReadNVItem(MMINV_PEDOMETER_DAY3_STEP_NV,&day3_step);
	MMI_ReadNVItem(MMINV_PEDOMETER_DAY4_STEP_NV,&day4_step);
	MMI_ReadNVItem(MMINV_PEDOMETER_DAY5_STEP_NV,&day5_step);
	MMI_ReadNVItem(MMINV_PEDOMETER_DAY6_STEP_NV,&day6_step);
	MMI_ReadNVItem(MMINV_PEDOMETER_DAY7_STEP_NV,&day7_step);

	//存储的步数不是最新步数，则需要存储最新步数，new_day_step是需要使用ZDT_GSensor_GetStepOneDay函数获得的
#ifdef WIN32
	new_day_step = 100;
#else
	new_day_step = 2800;//ZDT_GSensor_GetStepOneDay();
#endif
	SCI_TRACE_LOW("setTextControl new_day_step = %d",new_day_step);
	if (new_day_step > day1_step)
	{	
		MMI_WriteNVItem(MMINV_PEDOMETER_DAY1_STEP_NV,&new_day_step);
	}

	MMI_ReadNVItem(MMINV_PEDOMETER_FLAG,&is_first_open);

	//存储的最后一天日期不是最新日期，则需要重新存数据
	TM_GetSysDate(&sys_date);
	MMI_ReadNVItem(MMINV_PEDOMETER_DATE_NV,&day);
	if (is_first_open)
	{
		is_first_open = FALSE;
		MMI_WriteNVItem(MMINV_PEDOMETER_FLAG,&is_first_open);
		day = sys_date.mday;
		MMI_WriteNVItem(MMINV_PEDOMETER_DATE_NV,&day);
		MMI_WriteNVItem(MMINV_PEDOMETER_DAY1_STEP_NV,&new_day_step);
		MMI_WriteNVItem(MMINV_PEDOMETER_DAY2_STEP_NV,&day2_step);
		MMI_WriteNVItem(MMINV_PEDOMETER_DAY3_STEP_NV,&day3_step);
		MMI_WriteNVItem(MMINV_PEDOMETER_DAY4_STEP_NV,&day4_step);
		MMI_WriteNVItem(MMINV_PEDOMETER_DAY5_STEP_NV,&day5_step);
		MMI_WriteNVItem(MMINV_PEDOMETER_DAY6_STEP_NV,&day6_step);
		MMI_WriteNVItem(MMINV_PEDOMETER_DAY7_STEP_NV,&day7_step);
	}else
	{
		if (day != sys_date.mday)
		{
			day = sys_date.mday;
			MMI_WriteNVItem(MMINV_PEDOMETER_DATE_NV,&day);
			MMI_WriteNVItem(MMINV_PEDOMETER_DAY1_STEP_NV,&new_day_step);
			MMI_WriteNVItem(MMINV_PEDOMETER_DAY2_STEP_NV,&day1_step);
			MMI_WriteNVItem(MMINV_PEDOMETER_DAY3_STEP_NV,&day2_step);
			MMI_WriteNVItem(MMINV_PEDOMETER_DAY4_STEP_NV,&day3_step);
			MMI_WriteNVItem(MMINV_PEDOMETER_DAY5_STEP_NV,&day4_step);
			MMI_WriteNVItem(MMINV_PEDOMETER_DAY6_STEP_NV,&day5_step);
			MMI_WriteNVItem(MMINV_PEDOMETER_DAY7_STEP_NV,&day6_step);
		}
	}

	if (click_num == 1)
	{
		MMI_ReadNVItem(MMINV_PEDOMETER_DAY2_STEP_NV,&step_num);
	}else if (click_num == 2)
	{
		MMI_ReadNVItem(MMINV_PEDOMETER_DAY3_STEP_NV,&step_num);
	}else if (click_num == 3)
	{
		MMI_ReadNVItem(MMINV_PEDOMETER_DAY4_STEP_NV,&step_num);
	}else if (click_num == 4)
	{
		MMI_ReadNVItem(MMINV_PEDOMETER_DAY5_STEP_NV,&step_num);
	}else if (click_num == 5)
	{
		MMI_ReadNVItem(MMINV_PEDOMETER_DAY6_STEP_NV,&step_num);
	}else if (click_num == 6)
	{
		MMI_ReadNVItem(MMINV_PEDOMETER_DAY7_STEP_NV,&step_num);
	}else
	{
		MMI_ReadNVItem(MMINV_PEDOMETER_DAY1_STEP_NV,&step_num);
	}
	
	{
		GUI_POINT_T start_point = {10,50};
		MMI_STRING_T text_date = {0};
		MMI_STRING_T text_step = {0};
		MMI_STRING_T text_foot = {0};
		GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN;
		GUISTR_STYLE_T text_style = {0};

		int i = 0;
		BOOLEAN is_get_day = FALSE;
		wchar date_text[10] = {0};
		char step_char[20] = {0};
		wchar step_text[20] = {0};

		GUI_RECT_T date_rect = {10,50,140,80};
		GUI_RECT_T step_rect = {120,50,230,80};
		GUI_RECT_T display_rect = {0};

		for (i = 0;i < 7;i++)
		{
			if (click_num == i)
			{
				getTextDay(date_text,i,is_get_day);
			}
		}

		text_date.wstr_ptr = date_text;
		text_date.wstr_len = MMIAPICOM_Wstrlen(date_text);
		text_style.align = ALIGN_LEFT;
		text_style.font = SONG_FONT_24;
		text_style.font_color = MMI_WHITE_COLOR;

		//日期
		display_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,date_rect);
		GUISTR_DrawTextToLCDInRect(
			(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
			&display_rect,
			&display_rect,
			&text_date,
			&text_style,
			text_state,
			GUISTR_TEXT_DIR_AUTO);

		sprintf(step_char,"%d步",step_num);
		GUI_GBToWstr(step_text, step_char, strlen(step_char));
		
		text_step.wstr_ptr = step_text;
		text_step.wstr_len = MMIAPICOM_Wstrlen(step_text);

		//步数
		text_style.align = ALIGN_RIGHT;
		display_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,step_rect);
		GUISTR_DrawTextToLCDInRect(
			(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
			&display_rect,
			&display_rect,
			&text_step,
			&text_style,
			text_state,
			GUISTR_TEXT_DIR_AUTO);
		
	}

}


LOCAL void DisplayWeekStepData(MMI_WIN_ID_T win_id)
{
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	//left,top,right,bottom
#ifdef ZTE_SUPPORT_240X284
	GUI_RECT_T box1 = {13, 85, 30, 230};
	GUI_RECT_T box2 = {46, 85, 63, 230};
	GUI_RECT_T box3 = {79, 85, 96, 230};
	GUI_RECT_T box4 = {112, 85, 129, 230};
	GUI_RECT_T box5 = {145, 85, 164, 230};
	GUI_RECT_T box6 = {178, 85, 195, 230};
	GUI_RECT_T box7 = {211, 85, 226, 230};
#elif ZTE_SUPPORT_240X240
	GUI_RECT_T box1 = {14, 85, 29, 190};
	GUI_RECT_T box2 = {47, 85, 62, 190};
	GUI_RECT_T box3 = {80, 85, 95, 190};
	GUI_RECT_T box4 = {113, 85, 128, 190};
	GUI_RECT_T box5 = {146, 85, 163, 190};
	GUI_RECT_T box6 = {179, 85, 194, 190};
	GUI_RECT_T box7 = {212, 85, 227, 190};
#endif
	int day1_step = 0;
	int day2_step = 0;
	int day3_step = 0;
	int day4_step = 0;
	int day5_step = 0;
	int day6_step = 0;
	int day7_step = 0;

	MMI_ReadNVItem(MMINV_PEDOMETER_DAY1_STEP_NV,&day1_step);
	MMI_ReadNVItem(MMINV_PEDOMETER_DAY2_STEP_NV,&day2_step);
	MMI_ReadNVItem(MMINV_PEDOMETER_DAY3_STEP_NV,&day3_step);
	MMI_ReadNVItem(MMINV_PEDOMETER_DAY4_STEP_NV,&day4_step);
	MMI_ReadNVItem(MMINV_PEDOMETER_DAY5_STEP_NV,&day5_step);
	MMI_ReadNVItem(MMINV_PEDOMETER_DAY6_STEP_NV,&day6_step);
	MMI_ReadNVItem(MMINV_PEDOMETER_DAY7_STEP_NV,&day7_step);

	getMaxNumOfSeven(day1_step,day2_step,day3_step,day4_step,day5_step,day6_step,day7_step);
	//SCI_TRACE_LOW("%s: max_num = %d, step = %d,%d,%d,%d,%d,%d,%d", __FUNCTION__,
	//	max_num, day1_step, day2_step, day3_step, day4_step, day5_step,day6_step, day7_step);
#ifdef ZTE_SUPPORT_240X284
	box7.top = 232 - getLargestInteger(((float)day1_step / (float)max_num) * 185);
	box6.top = 232 - getLargestInteger(((float)day2_step / (float)max_num) * 185);
	box5.top = 232 - getLargestInteger(((float)day3_step / (float)max_num) * 185);
	box4.top = 232 - getLargestInteger(((float)day4_step / (float)max_num) * 185);
	box3.top = 232 - getLargestInteger(((float)day5_step / (float)max_num) * 185);
	box2.top = 232 - getLargestInteger(((float)day6_step / (float)max_num) * 185);
	box1.top = 232 - getLargestInteger(((float)day7_step / (float)max_num) * 185);
#elif ZTE_SUPPORT_240X240
	box7.top = 192 - getLargestInteger(((float)day1_step / (float)max_num) * 115);
	box6.top = 192 - getLargestInteger(((float)day2_step / (float)max_num) * 115);
	box5.top = 192 - getLargestInteger(((float)day3_step / (float)max_num) * 115);
	box4.top = 192 - getLargestInteger(((float)day4_step / (float)max_num) * 115);
	box3.top = 192 - getLargestInteger(((float)day5_step / (float)max_num) * 115);
	box2.top = 192 - getLargestInteger(((float)day6_step / (float)max_num) * 115);
	box1.top = 192 - getLargestInteger(((float)day7_step / (float)max_num) * 115);
#endif
	//SCI_TRACE_LOW("%s: top = %d,%d,%d,%d,%d,%d,%d", __FUNCTION__, box7.top, box6.top, box5.top, box4.top, box3.top,box2.top, box1.top);
	box1 = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,box1);
	box2 = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,box2);
	box3 = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,box3);
	box4 = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,box4);
	box5 = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,box5);
	box6 = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,box6);
	box7 = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,box7);
	
	/*LCD_FillRect(&lcd_dev_info, box1, statistics_color[1]);LCD_DrawRect(&lcd_dev_info, box1, statistics_color[1]);
	LCD_FillRect(&lcd_dev_info, box2, statistics_color[2]);LCD_DrawRect(&lcd_dev_info, box2, statistics_color[2]);
	LCD_FillRect(&lcd_dev_info, box3, statistics_color[3]);LCD_DrawRect(&lcd_dev_info, box3, statistics_color[3]);
	LCD_FillRect(&lcd_dev_info, box4, statistics_color[4]);LCD_DrawRect(&lcd_dev_info, box4, statistics_color[4]);
	LCD_FillRect(&lcd_dev_info, box5, statistics_color[5]);LCD_DrawRect(&lcd_dev_info, box5, statistics_color[5]);
	LCD_FillRect(&lcd_dev_info, box6, statistics_color[6]);LCD_DrawRect(&lcd_dev_info, box6, statistics_color[6]);
	LCD_FillRect(&lcd_dev_info, box7, statistics_color[7]);LCD_DrawRect(&lcd_dev_info, box7, statistics_color[7]);*/

	LCD_FillRoundedRect(&lcd_dev_info, box1, box1, pros_bar_color);
	LCD_FillRoundedRect(&lcd_dev_info, box2, box2, pros_bar_color);
	LCD_FillRoundedRect(&lcd_dev_info, box3, box3, pros_bar_color);
	LCD_FillRoundedRect(&lcd_dev_info, box4, box4, pros_bar_color);
	LCD_FillRoundedRect(&lcd_dev_info, box5, box5, pros_bar_color);
	LCD_FillRoundedRect(&lcd_dev_info, box6, box6, pros_bar_color);
	LCD_FillRoundedRect(&lcd_dev_info, box7, box7, pros_bar_color);
}

LOCAL MMI_RESULT_E dayButton1CallBack()
{

	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_1_CTRL_ID,MMI_WHITE_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_2_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_3_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_4_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_5_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_6_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_7_CTRL_ID,MMI_GRAY_COLOR);

	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_1_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_2_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_3_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_4_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_5_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_6_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_7_CTRL_ID);

	click_num = 0;
	MMK_SendMsg(MMI_PEDOMETER_STEP_WEEK_WIN_ID, MSG_FULL_PAINT, NULL);

	//Trace_Readboy_Log("dayButton1CallBack");
}

LOCAL MMI_RESULT_E dayButton2CallBack()
{
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_1_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_2_CTRL_ID,MMI_WHITE_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_3_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_4_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_5_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_6_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_7_CTRL_ID,MMI_GRAY_COLOR);

	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_1_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_2_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_3_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_4_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_5_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_6_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_7_CTRL_ID);

	click_num = 1;
	MMK_SendMsg(MMI_PEDOMETER_STEP_WEEK_WIN_ID, MSG_FULL_PAINT, NULL);

	//Trace_Readboy_Log("dayButton2CallBack");
}

LOCAL MMI_RESULT_E dayButton3CallBack()
{

	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_1_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_2_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_3_CTRL_ID,MMI_WHITE_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_4_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_5_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_6_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_7_CTRL_ID,MMI_GRAY_COLOR);

	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_1_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_2_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_3_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_4_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_5_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_6_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_7_CTRL_ID);

	click_num = 2;
	MMK_SendMsg(MMI_PEDOMETER_STEP_WEEK_WIN_ID, MSG_FULL_PAINT, NULL);

	//Trace_Readboy_Log("dayButton3CallBack");
}

LOCAL MMI_RESULT_E dayButton4CallBack()
{

	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_1_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_2_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_3_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_4_CTRL_ID,MMI_WHITE_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_5_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_6_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_7_CTRL_ID,MMI_GRAY_COLOR);

	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_1_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_2_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_3_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_4_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_5_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_6_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_7_CTRL_ID);

	click_num = 3;
	MMK_SendMsg(MMI_PEDOMETER_STEP_WEEK_WIN_ID, MSG_FULL_PAINT, NULL);

	//Trace_Readboy_Log("dayButton4CallBack");
}

LOCAL MMI_RESULT_E dayButton5CallBack()
{
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_1_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_2_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_3_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_4_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_5_CTRL_ID,MMI_WHITE_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_6_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_7_CTRL_ID,MMI_GRAY_COLOR);

	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_1_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_2_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_3_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_4_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_5_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_6_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_7_CTRL_ID);

	click_num = 4;
	MMK_SendMsg(MMI_PEDOMETER_STEP_WEEK_WIN_ID, MSG_FULL_PAINT, NULL);

	//Trace_Readboy_Log("dayButton5CallBack");
}

LOCAL MMI_RESULT_E dayButton6CallBack()
{

	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_1_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_2_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_3_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_4_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_5_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_6_CTRL_ID,MMI_WHITE_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_7_CTRL_ID,MMI_GRAY_COLOR);

	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_1_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_2_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_3_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_4_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_5_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_6_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_7_CTRL_ID);

	click_num = 5;
	MMK_SendMsg(MMI_PEDOMETER_STEP_WEEK_WIN_ID, MSG_FULL_PAINT, NULL);

	//Trace_Readboy_Log("dayButton6CallBack");
}

LOCAL MMI_RESULT_E dayButton7CallBack()
{
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_1_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_2_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_3_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_4_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_5_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_6_CTRL_ID,MMI_GRAY_COLOR);
	//GUIBUTTON_SetFontColor(MMI_PEDOMETER_WEEK_7_CTRL_ID,MMI_WHITE_COLOR);

	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_1_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_2_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_3_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_4_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_5_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_6_CTRL_ID);
	GUIBUTTON_Update(MMI_PEDOMETER_WEEK_7_CTRL_ID);

	click_num = 6;
	MMK_SendMsg(MMI_PEDOMETER_STEP_WEEK_WIN_ID, MSG_FULL_PAINT, NULL);

	//Trace_Readboy_Log("dayButton7CallBack");
}

LOCAL void PedometerStepWeekRefresh(uint8 time_id, void * param)
{
	MMK_SendMsg(MMI_PEDOMETER_STEP_WEEK_WIN_ID, MSG_FULL_PAINT, PNULL);
}

PUBLIC MMI_RESULT_E HandlePedometerStepWeekWin(
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
			//GUI_FillRect(&lcd_dev_info,rect,MMI_BLACK_COLOR);//填充整个布局的颜色
			/*GUIBUTTON_SetCallBackFunc(MMI_PEDOMETER_WEEK_1_CTRL_ID,dayButton1CallBack);
			GUIBUTTON_SetCallBackFunc(MMI_PEDOMETER_WEEK_2_CTRL_ID,dayButton2CallBack);
			GUIBUTTON_SetCallBackFunc(MMI_PEDOMETER_WEEK_3_CTRL_ID,dayButton3CallBack);
			GUIBUTTON_SetCallBackFunc(MMI_PEDOMETER_WEEK_4_CTRL_ID,dayButton4CallBack);
			GUIBUTTON_SetCallBackFunc(MMI_PEDOMETER_WEEK_5_CTRL_ID,dayButton5CallBack);
			GUIBUTTON_SetCallBackFunc(MMI_PEDOMETER_WEEK_6_CTRL_ID,dayButton6CallBack);
			GUIBUTTON_SetCallBackFunc(MMI_PEDOMETER_WEEK_7_CTRL_ID,dayButton7CallBack);*/
			
		}
		break;
	case MSG_GET_FOCUS:
		{
			if(timer_id != 0){
				MMK_StopTimer(timer_id);
				timer_id = 0;
			}
			if(!MMIZDT_IsInChargingWin()){
				timer_id = MMK_CreateTimerCallback(1500, PedometerStepWeekRefresh, PNULL, TRUE);
			}
		}
		break;
	case MSG_FULL_PAINT:
		{
			GUI_RECT_T title_rect = {0,10,240,45};
			MMI_STRING_T text_week_title = {0};
			GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN;
			GUISTR_STYLE_T text_style = {0};

			text_style.align = ALIGN_HMIDDLE;
			text_style.font = SONG_FONT_26;
			text_style.font_color = MMI_PEDO_YELLOW_COLOR;

			{
			#ifdef ZTE_SUPPORT_240X284
				GUI_RECT_T clientRect = {0,0,240,284};
			#elif ZTE_SUPPORT_240X240
				GUI_RECT_T clientRect = {0,0,240,240};
			#endif
				GUI_RECT_T win_rect = {0};
				win_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, clientRect);
				GUI_FillRect(&lcd_dev_info, win_rect, MMI_BLACK_COLOR);
			}

			/*title_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,title_rect);
			MMIRES_GetText(TXT_PEDOMETER_STEP_WEEK,win_id,&text_week_title);
			GUISTR_DrawTextToLCDInRect(
				(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
				&title_rect,
				&title_rect,
				&text_week_title,
				&text_style,
				text_state,
				GUISTR_TEXT_DIR_AUTO
				);*/

			setButtonControl(win_id, lcd_dev_info);

			//setTextControl(win_id);

			DisplayWeekStepData(win_id);
			
		}
		break;
	case MSG_TP_PRESS_DOWN:
		{
			GUI_POINT_T   point = {0};
	    		point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			pedometer_statis_tp_down_x = point.x;
			pedometer_statis_tp_down_y = point.y;
		}
		break;
	case MSG_TP_PRESS_UP:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			if((pedometer_statis_tp_down_x - point.x) > 40)
			{
				 //MMIPEDOMETER_CreateFriendRankWin();
				 //MMK_CloseWin(win_id);
				 break;
			}
			else if((point.x - pedometer_statis_tp_down_x ) > 40)
			{
				MMIPEDOMETER_CreateTodayWin();
				MMK_CloseWin(win_id);
				break;
			}
		}
		break;
	case MSG_APP_CANCEL:
	case MSG_CLOSE_WINDOW:
		{
			click_num = 0;
			if(timer_id != 0){
				MMK_StopTimer(timer_id);
				timer_id = 0;
			}
		}
		break;
	default:
		break;

	}

	return recode;

}


WINDOW_TABLE(MMI_PEDOMETER_STATISTICS_WIN_TAB) = 
{
	WIN_ID(MMI_PEDOMETER_STEP_WEEK_WIN_ID),
	WIN_FUNC((uint32)HandlePedometerStepWeekWin),
	WIN_HIDE_STATUS,
	CREATE_BUTTON_CTRL(NULL,MMI_PEDOMETER_WEEK_1_CTRL_ID),
	CREATE_BUTTON_CTRL(NULL,MMI_PEDOMETER_WEEK_2_CTRL_ID),
	CREATE_BUTTON_CTRL(NULL,MMI_PEDOMETER_WEEK_3_CTRL_ID),
	CREATE_BUTTON_CTRL(NULL,MMI_PEDOMETER_WEEK_4_CTRL_ID),
	CREATE_BUTTON_CTRL(NULL,MMI_PEDOMETER_WEEK_5_CTRL_ID),
	CREATE_BUTTON_CTRL(NULL,MMI_PEDOMETER_WEEK_6_CTRL_ID),
	CREATE_BUTTON_CTRL(NULL,MMI_PEDOMETER_WEEK_7_CTRL_ID),
	//WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
	END_WIN
};

PUBLIC void MMIPEDOMETER_CreateStepWeekWin()
{
	MMI_WIN_ID_T win_id = MMI_PEDOMETER_STEP_WEEK_WIN_ID;
	MMI_HANDLE_T win_handle = 0;
	GUI_RECT_T rect = {0,0,240,240};
	if (MMK_IsOpenWin(win_id))
	{
		MMK_CloseWin(win_id);
	}

	win_handle = MMK_CreateWin((uint32*)MMI_PEDOMETER_STATISTICS_WIN_TAB, NULL);
	MMK_SetWinRect(win_handle, &rect);
}
