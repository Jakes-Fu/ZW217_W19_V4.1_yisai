/*****************************************************************************
** File Name:      mmiwidget_calendar.c                                        *
** Author:                                                                   *
** Date:           10/23/2009                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/23/2009        bin.ji              Creat
******************************************************************************/

#define _MMIWIDGET_CALENDAR_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "std_header.h"

#include "mmi_app_widget_trc.h"
#if defined MMI_GRID_CALENDAR

#include "window_parse.h"
#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiwidget_image.h"
#include "guibutton.h"
#include "mmiwidget.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "guires.h"
#include "mmitheme_widget.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
#include "mmiset_export.h"
//#include "mmialarm_image.h"
#include "mmiwidget_position.h"
#include "mmischedule_export.h"
#include "mmicalendar_export.h"
#include "mmiacc_image.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
#include "mmi_solarlunar.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#if !defined(BAR_PHONE_WIDGET)
#define WIDGET_EDIT_SCHE   //点击日期是否编辑日程
#endif

//#define WIDGET_WEEKEND_COLOR  //周末是否显示颜色
//#define WIDGET_PRE_NEXT_MONTH  //是否显示上个月和下个月

#define MMIWIDGET_CALENDAR_LINENUM                         6       //日历上显示日期的行数
#define MMIWIDGET_CALENDAR_COLNUM                          7       //日历上显示日期的列数

#define MMIWIDGET_CALENDAR_WEEKDAY_NUM                 7       //一周的天数
#define MMIWIDGET_CALENDAR_MONTHNUM                    12      //一年的月数

#define MMIWIDGET_CALENDAR_NUM_LENGTH                      10//5       //uint16整数的最大位数,转换成阿拉伯语的数字为unicode，0-9对应为0x06F0-0X06F9

#if defined(MAINLCD_SIZE_320X480)
#define MMIWIDGET_CALENDAR_YEAR_TEXT_FONT                    SONG_FONT_13

#define MMIWIDGET_CALENDAR_MONTH_TEXT_FONT                    SONG_FONT_20

//为兼容多国语言
#define MMIWIDGET_CALENDAR_WEEKS_TEXT_FONT                               SONG_FONT_14 

#define MMIWIDGET_CALENDAR_MONTH_DATE_TEXT_FONT                    SONG_FONT_14

#define MMIWIDGET_CALENDAR_MONTH_DAY_TEXT_FONT                    SONG_FONT_26
#else
#define MMIWIDGET_CALENDAR_YEAR_TEXT_FONT                    SONG_FONT_11

#define MMIWIDGET_CALENDAR_MONTH_TEXT_FONT                    SONG_FONT_18

//为兼容多国语言
#define MMIWIDGET_CALENDAR_WEEKS_TEXT_FONT                               SONG_FONT_12 

#define MMIWIDGET_CALENDAR_MONTH_DATE_TEXT_FONT                    SONG_FONT_12
//#ifdef MAINLCD_SIZE_240X320
#define MMIWIDGET_CALENDAR_MONTH_DAY_TEXT_FONT                    SONG_FONT_20
//#else
//#define MMIWIDGET_CALENDAR_MONTH_DAY_TEXT_FONT                    SONG_FONT_22
//#endif
#endif

#define DECEMBER                        12      //十二月

typedef enum
{
    MMIWIDGET_CALENDER_BLACK,        //黑色
    MMIWIDGET_CALENDER_GRAY,         //灰
    MMIWIDGET_CALENDER_RED,          //红
    MMIWIDGET_CALENDER_BLUE          //蓝
} MMIWIDGET_CALENDER_COLOR_E;

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

// MMIWIDGET CALENDAR
typedef struct 
{	
	MMI_WIN_ID_T    win_id;
	uint16        year;
	uint8         month;
	uint8         day;
	uint8        first_day_of_week;		  
}MMIWIDGET_CALENDAR_INFO_T;


/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

//按月显示中每一天的位置
LOCAL const GUI_POINT_T s_day_rect[MMIWIDGET_CALENDAR_LINENUM][MMIWIDGET_CALENDAR_COLNUM] = 
{
	{//显示星期"日~六"
        {MMIWIDGET_CALENDAR_MONTH_ITEM0_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE0_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM1_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE0_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM2_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE0_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM3_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE0_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM4_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE0_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM5_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE0_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM6_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE0_TOP}
    },
	{//以下5行显示具体的日期
        {MMIWIDGET_CALENDAR_MONTH_ITEM0_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE1_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM1_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE1_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM2_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE1_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM3_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE1_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM4_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE1_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM5_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE1_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM6_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE1_TOP}
    },
    {
        {MMIWIDGET_CALENDAR_MONTH_ITEM0_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE2_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM1_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE2_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM2_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE2_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM3_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE2_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM4_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE2_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM5_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE2_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM6_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE2_TOP}
    },
    {
        {MMIWIDGET_CALENDAR_MONTH_ITEM0_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE3_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM1_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE3_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM2_LEFT,MMIWIDGET_CALENDAR_MONTH_LINE3_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM3_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE3_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM4_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE3_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM5_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE3_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM6_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE3_TOP}
    },
    {
        {MMIWIDGET_CALENDAR_MONTH_ITEM0_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE4_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM1_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE4_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM2_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE4_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM3_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE4_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM4_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE4_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM5_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE4_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM6_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE4_TOP}
    },
    {
        {MMIWIDGET_CALENDAR_MONTH_ITEM0_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE5_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM1_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE5_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM2_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE5_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM3_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE5_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM4_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE5_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM5_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE5_TOP}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM6_LEFT, MMIWIDGET_CALENDAR_MONTH_LINE5_TOP}
    },
};

LOCAL const GUI_POINT_T s_day_rect_h[MMIWIDGET_CALENDAR_LINENUM][MMIWIDGET_CALENDAR_COLNUM] = 
{
	{//显示星期"日~六"
        {MMIWIDGET_CALENDAR_MONTH_ITEM0_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE0_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM1_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE0_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM2_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE0_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM3_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE0_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM4_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE0_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM5_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE0_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM6_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE0_TOP_H}
    },
	{//以下5行显示具体的日期
        {MMIWIDGET_CALENDAR_MONTH_ITEM0_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE1_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM1_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE1_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM2_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE1_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM3_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE1_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM4_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE1_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM5_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE1_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM6_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE1_TOP_H}
    },
    {
        {MMIWIDGET_CALENDAR_MONTH_ITEM0_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE2_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM1_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE2_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM2_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE2_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM3_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE2_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM4_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE2_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM5_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE2_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM6_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE2_TOP_H}
    },
    {
        {MMIWIDGET_CALENDAR_MONTH_ITEM0_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE3_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM1_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE3_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM2_LEFT_H,MMIWIDGET_CALENDAR_MONTH_LINE3_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM3_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE3_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM4_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE3_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM5_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE3_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM6_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE3_TOP_H}
    },
    {
        {MMIWIDGET_CALENDAR_MONTH_ITEM0_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE4_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM1_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE4_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM2_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE4_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM3_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE4_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM4_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE4_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM5_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE4_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM6_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE4_TOP_H}
    },
    {
        {MMIWIDGET_CALENDAR_MONTH_ITEM0_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE5_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM1_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE5_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM2_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE5_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM3_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE5_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM4_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE5_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM5_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE5_TOP_H}, 
        {MMIWIDGET_CALENDAR_MONTH_ITEM6_LEFT_H, MMIWIDGET_CALENDAR_MONTH_LINE5_TOP_H}
    },
};

LOCAL const MMI_TEXT_ID_T s_week_text_id[MMIWIDGET_CALENDAR_COLNUM] =
{
	    TXT_SHORT_SUNDAY,
		TXT_SHORT_MONDAY,
		TXT_SHORT_TUESDAY,
		TXT_SHORT_WEDNESDAY,
		TXT_SHORT_THURSDAY,
		TXT_SHORT_FRIDAY,
		TXT_SHORT_SATURDAY
};

LOCAL const uint8 s_widget_calendar_month_day[MMIWIDGET_CALENDAR_MONTHNUM] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
LOCAL const uint8 s_widget_calendar_month_day_leap[MMIWIDGET_CALENDAR_MONTHNUM] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
//非闰年，假设1月1日是周日，s_gui_calendar_first_day[i]对应每月1日的星期
//闰年，假设1月1日是周日，s_gui_calendar_first_day_leap[i]对应每月1日的星期
LOCAL const uint8 s_widget_calendar_first_day[MMIWIDGET_CALENDAR_MONTHNUM] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};
LOCAL const uint8 s_widget_calendar_first_day_leap[MMIWIDGET_CALENDAR_MONTHNUM] = {0, 3, 4, 0, 2, 5, 0, 3, 6, 1, 4, 6};
#ifdef WIDGET_EDIT_SCHE
LOCAL MMISCH_DETAIL_WIN_PARAM_T    *s_widget_param          = PNULL;
LOCAL MMISCH_LIST_WIN_PARAM_T      *s_widget_list_param     = PNULL;
LOCAL SCI_DATE_T     s_date = {0};
#ifdef CALENDAR_SUPPORT
extern MMI_CALENDAR_T g_calendar_show;
#endif
#endif

#if 0//def BAR_PHONE_WIDGET 
SCI_DATE_T              display_date = {0};
#endif


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

LOCAL MMI_RESULT_E  HandleWidgetCalendarWinMsg(
											   MMI_WIN_ID_T        win_id,        //IN:
											   MMI_MESSAGE_ID_E    msg_id,        //IN:
											   DPARAM            param        //IN:
											   );


LOCAL uint8 MMIWIDGET_Calendar_GetMonthday(
										   uint16      year,
										   uint8       month
										   );
LOCAL GUI_RECT_T MMIWIDGET_Calendar_GetDateRect(
												uint32               row,
												uint32               col
												);
LOCAL uint8 MMIWIDGET_Calendar_GetWeekday(
										  uint16        year,
										  uint8         month,
										  uint8         day
										  );
LOCAL void MMIWIDGET_Calendar_GetPosByDate( 
										   MMIWIDGET_CALENDAR_INFO_T*    calendar_info_ptr,
										   uint16                  year,
										   uint8                   month,
										   uint8                   day,
										   uint32*                 row_ptr,
										   uint32*                 col_ptr
										   );
LOCAL MMI_STRING_T MMIWIDGET_Calendar_CreateNumString(
													  uint16 num
													  );
LOCAL  void MMIWIDGET_Calendar_SetCalendarTextStyle(
													GUISTR_STYLE_T *style_ptr,
													GUI_ALIGN_E align,
													GUI_FONT_T  font,
													GUI_COLOR_T font_color
													);

LOCAL BOOLEAN MMIWIDGET_Calendar_DrawMonthDate(
											   MMIWIDGET_CALENDAR_INFO_T*    calendar_info_ptr,
											   uint16               year,
											    uint8                month,
											   uint8                day,
											   uint8                month_day,
											   const GUI_RECT_T*    rect_ptr,
											   MMIWIDGET_CALENDER_COLOR_E     color_type,
											   const GUI_LCD_DEV_INFO *dev_info_ptr       //in:                                
											   );

LOCAL void MMIWIDGET_Calendar_DrawMonthDates(
											 MMIWIDGET_CALENDAR_INFO_T*    calendar_info_ptr,
											 MMIWIDGET_CALENDAR_INFO_T*    calendar_pre_info_ptr,
											 MMIWIDGET_CALENDAR_INFO_T*    calendar_next_info_ptr,
											 const GUI_LCD_DEV_INFO *dev_info_ptr       //in:                        
											 );

LOCAL void  MMIWIDGET_Calendar_DisplayCalendar(
											   MMI_WIN_ID_T     win_id
											   );
LOCAL void MMIWIDGET_Calendar_DrawWeeks(
										MMIWIDGET_CALENDAR_INFO_T*    calendar_info_ptr,
										const GUI_LCD_DEV_INFO *dev_info_ptr       //in:     
										);
LOCAL void MMIWIDGET_Calendar_DrawYearMonth(
											MMIWIDGET_CALENDAR_INFO_T*    calendar_info_ptr,
											const GUI_LCD_DEV_INFO *dev_info_ptr       //in:                                   
											) ;

#ifdef WIDGET_EDIT_SCHE
LOCAL MMI_RESULT_E  HandleTpPressUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      );
LOCAL uint8 CALENDAR_GetFirstday(
                              uint16    solar_year,
                              uint8     solar_month
                              );
#endif

LOCAL void MMIWIDGET_Calendar_DrawCurrentDate(
											 MMIWIDGET_CALENDAR_INFO_T*    calendar_info_ptr,
											 const GUI_LCD_DEV_INFO *dev_info_ptr       //in:                        
											 );


#ifdef BAR_PHONE_WIDGET 
LOCAL void MovePrevMonth(
                         SCI_DATE_T  date_in,
                         SCI_DATE_T  *date_out_ptr
                         );


LOCAL void MoveNextMonth(
                         SCI_DATE_T  date_in,
                         SCI_DATE_T  *date_out_ptr
                         );

#endif
											 

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/

PUBLIC const MMIWIDGET_ITEM_INFO_T g_calendar_widget =
{
        MMIWIDGET_CALENDAR_ID, 
		HandleWidgetCalendarWinMsg, 
		PNULL,
		PNULL,
		PNULL,
		WIDGET_CALENDAR_WIN_ID, 
		IMAGE_WIDGET_CALENDAR_SHORTCUT, 
		IMAGE_WIDGET_CALENDAR_BG,
        TXT_TOOLS_CALENDAR,		
		50,//    75, 
		50,//    50
		FALSE,
};


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/


LOCAL MMI_RESULT_E  HandleWidgetCalendarWinMsg(
											   MMI_WIN_ID_T        win_id,        //IN:
											   MMI_MESSAGE_ID_E    msg_id,        //IN:
											   DPARAM            param        //IN:
											   )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};
    GUI_RECT_T win_rect = {0};    
    
    switch (msg_id)
    {     
	case MSG_OPEN_WINDOW:
#ifdef WIDGET_EDIT_SCHE
		if (s_widget_param != NULL)
		{
			SCI_FREE(s_widget_param);
			s_widget_param = NULL;
		}
		s_widget_param = SCI_ALLOC_APP(sizeof(MMISCH_DETAIL_WIN_PARAM_T));
		SCI_MEMSET(s_widget_param, 0, sizeof(MMISCH_DETAIL_WIN_PARAM_T));

		if(NULL != s_widget_list_param)
		{
			SCI_FREE(s_widget_list_param);
		}
		s_widget_list_param = SCI_ALLOC_APP(sizeof(MMISCH_LIST_WIN_PARAM_T));
		SCI_MEMSET(s_widget_list_param, 0, sizeof(MMISCH_LIST_WIN_PARAM_T));
#endif
		break;

#ifdef TOUCH_PANEL_SUPPORT
	case MSG_WIDGET_TP_PRESS_UP:

		if(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CALENDAR))
		{
#ifdef CALENDAR_SUPPORT
                    MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_CALENDAR, MMIAPICALEND_OpenMainWin);
#endif
		}
		else
		{
#ifdef WIDGET_EDIT_SCHE
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			MMK_GetWinRect(win_id, &win_rect);
			if (GUI_PointIsInRect(point, win_rect))
			{
				if(MMI_RESULT_TRUE == HandleTpPressUp(win_id, param))
				{
#ifdef CALENDAR_SUPPORT
            				if(!MMIAPISCH_IsOneDateHasSche(s_date.year, s_date.mon, s_date.mday))
            				{
            					s_widget_param->win_id = WIDGET_CALENDAR_WIN_ID;
            					//初始化编辑WIDGET日历
            					MMIAPISCH_InitNewSch(s_date.year,
            						s_date.mon,
            						s_date.mday,
            						&s_widget_param->event);

            					MMIAPISCH_OpenEditWin((ADD_DATA)s_widget_param);		
            				}
            				else
            				{
            					s_widget_list_param->day = s_date.mday;
            					s_widget_list_param->month = s_date.mon;
            					s_widget_list_param->year = s_date.year;
            					MMIAPISCH_OpenViewListWin((ADD_DATA)s_widget_list_param);
            				}
#endif
				}

			}
#else
#ifdef CALENDAR_SUPPORT
                        MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_CALENDAR, MMIAPICALEND_OpenMainWin);
#endif
#endif
		}

		break;
#endif

    case MSG_FULL_PAINT:
        MMIWIDGET_Calendar_DisplayCalendar(win_id);
        break;

#ifdef BAR_PHONE_WIDGET 
    case MSG_WIDGET_UPDATE_SOFTKEY:        
        MMIWIDGET_SoftkeySetTextId(win_id,TXT_VIEW,FALSE);
        break;
#endif

#if 0 //def BAR_PHONE_WIDGET 
    case MSG_APP_UP:
        {
            MovePrevMonth(display_date,&display_date);
        }
        break;
        
    case MSG_APP_DOWN:
        {
            MoveNextMonth(display_date,&display_date);
        }
        break;
#endif
    case MSG_APP_WEB:
        {
#ifdef WIDGET_EDIT_SCHE
            SCI_DATE_T              date = {0};

            TM_GetSysDate(&date);
#ifdef CALENDAR_SUPPORT
            if(!MMIAPISCH_IsOneDateHasSche(date.year, date.mon, date.mday))
            {
                s_widget_param->win_id = WIDGET_CALENDAR_WIN_ID;
                //初始化编辑WIDGET日历
                MMIAPISCH_InitNewSch(date.year,
                    date.mon,
                    date.mday,
                    &s_widget_param->event);

                MMIAPISCH_OpenEditWin((ADD_DATA)s_widget_param);               
            }
            else
            {
                s_widget_list_param->day = date.mday;
                s_widget_list_param->month = date.mon;
                s_widget_list_param->year = date.year;
                MMIAPISCH_OpenViewListWin((ADD_DATA)s_widget_list_param);
            }
#endif
#else
#ifdef CALENDAR_SUPPORT
            MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_CALENDAR, MMIAPICALEND_OpenMainWin);
#endif
#endif
        }
        break;
		
    case MSG_CLOSE_WINDOW:
#ifdef WIDGET_EDIT_SCHE
        if (NULL != s_widget_param)
        {
        SCI_FREE(s_widget_param);
        s_widget_param = NULL;
        }

        if(NULL != s_widget_list_param)
        {
        SCI_FREE(s_widget_list_param);
        s_widget_list_param = NULL;
        }
#endif
        break;

#ifdef WIDGET_EDIT_SCHE
    case MSG_SCH_EVENT_EDIT_FINISH:
        MMIALM_AddOneEvent(&s_widget_param->event,NULL);
        break;
#endif

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;        
}

LOCAL uint8 MMIWIDGET_Calendar_GetMonthday(
										   uint16      year,
										   uint8       month
										   )
{
    uint8 ret_val = 0;
	
    if (month == 0 || month > MMIWIDGET_CALENDAR_MONTHNUM)
    {
        //SCI_TRACE_LOW:"MMICALENDAR_GetMonthday error month=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_CALENDAR_555_112_2_18_3_7_56_175,(uint8*)"d",month);
        return 0;
    }
    if (MMIAPICOM_IsLeapYear(year))
    {
        ret_val = s_widget_calendar_month_day_leap[month - 1];
    }
    else
    {
        ret_val  = s_widget_calendar_month_day[month - 1];
    }
	
    return ret_val;
}

LOCAL GUI_RECT_T MMIWIDGET_Calendar_GetDateRect(
												uint32               row,
												uint32               col
												)
{
    GUI_RECT_T rect = {0};
    
	//SCI_ASSERT(row < MMIWIDGET_CALENDAR_LINENUM);
	//SCI_ASSERT(col < MMIWIDGET_CALENDAR_COLNUM);

    if (row >= MMIWIDGET_CALENDAR_LINENUM)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_Calendar_GetDateRect row >= MMIWIDGET_CALENDAR_LINENUM row=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_CALENDAR_582_112_2_18_3_7_56_176,(uint8*)"d", row);
        return rect;
    }

    if (col >= MMIWIDGET_CALENDAR_COLNUM)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_Calendar_GetDateRect col >= MMIWIDGET_CALENDAR_COLNUM col=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_CALENDAR_588_112_2_18_3_7_56_177,(uint8*)"d", col);
        return rect;
    }

    if(MMITHEME_IsMainScreenLandscape())
    {
        rect.left   = s_day_rect_h[row][col].x;
        rect.top    = s_day_rect_h[row][col].y;
        rect.right  = rect.left + MMIWIDGET_CALENDAR_LINE_WIDTH_H - 1;
        rect.bottom = rect.top + MMIWIDGET_CALENDAR_LINE_HEIGHT_H - 1;	
    }
    else
    {
        rect.left   = s_day_rect[row][col].x;
        rect.top    = s_day_rect[row][col].y;
        rect.right  = rect.left + MMIWIDGET_CALENDAR_LINE_WIDTH - 1;
        rect.bottom = rect.top + MMIWIDGET_CALENDAR_LINE_HEIGHT - 1;	
    }
	
    return rect;
}

LOCAL uint8 MMIWIDGET_Calendar_GetWeekday(
										  uint16        year,
										  uint8         month,
										  uint8         day
										  )
{
    uint8   ret_val = 0;
    uint16  year_plus = 0;   
	
    //SCI_PASSERT(1 <= month && 12 >= month, ("MMICALENDAR_GetWeekday: month = %d",month));	
    //SCI_PASSERT(1 <= day && 31 >= day, ("MMICALENDAR_GetWeekday: day = %d",day));	

    if (month < 1 || month > 12)
    {
        //SCI_TRACE_LOW:"MMICALENDAR_GetWeekday month < 1 || month > 12, month=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_CALENDAR_631_112_2_18_3_7_56_178,(uint8*)"d",month);
        return ret_val;
    }

    if (day < 1 || day > 31)
    {
        //SCI_TRACE_LOW:"MMICALENDAR_GetWeekday day < 1 || day > 31, day=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_CALENDAR_637_112_2_18_3_7_56_179,(uint8*)"d",day);
        return ret_val;
    }
	
    //等于公元元年到上年12月31日所经过的天数减去年数乘上364的值, 除以7的余数就是那天的星期
    year_plus = (uint16)((year-1) + (year-1)/4 - (year-1)/100 + (year-1)/400);
	
    if (MMIAPICOM_IsLeapYear(year))
    {
        ret_val = (uint8)(( day + s_widget_calendar_first_day_leap[month - 1] + year_plus ) % MMIWIDGET_CALENDAR_WEEKDAY_NUM);
    }
    else
    {
        ret_val = (uint8)(( day + s_widget_calendar_first_day[month - 1] + year_plus ) % MMIWIDGET_CALENDAR_WEEKDAY_NUM);
    }
	
    return ret_val;
}

LOCAL void MMIWIDGET_Calendar_GetPosByDate( 
										   MMIWIDGET_CALENDAR_INFO_T*    calendar_info_ptr,
										   uint16                  year,
										   uint8                   month,
										   uint8                   day,
										   uint32*                 row_ptr,
										   uint32*                 col_ptr
										   )
{
    uint32 offset;
    
    //SCI_ASSERT( NULL != calendar_info_ptr );
    //SCI_ASSERT( NULL != row_ptr );
    //SCI_ASSERT( NULL != col_ptr );

    if (PNULL == calendar_info_ptr || PNULL == row_ptr || PNULL == col_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_Calendar_GetPosByDate PNULL == calendar_info_ptr || PNULL == row_ptr || PNULL == col_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_CALENDAR_673_112_2_18_3_7_56_180,(uint8*)"");
        return;
    }
	
    offset = ( MMIWIDGET_CALENDAR_WEEKDAY_NUM + MMIWIDGET_Calendar_GetWeekday( year, month, 1 ) 
        - calendar_info_ptr->first_day_of_week ) % MMIWIDGET_CALENDAR_WEEKDAY_NUM + day - 1;
    {
        *row_ptr = offset / MMIWIDGET_CALENDAR_WEEKDAY_NUM;
        *col_ptr = offset % MMIWIDGET_CALENDAR_WEEKDAY_NUM;
    }
	
}

LOCAL MMI_STRING_T MMIWIDGET_Calendar_CreateNumString(
													  uint16 num
													  )
{
    wchar*       num_ptr = (wchar*)SCI_ALLOC_APP( (MMIWIDGET_CALENDAR_NUM_LENGTH+1)*sizeof(wchar) );
    char         buf[20] = {0};   
    MMI_STRING_T num_string = {0};
    MMIAPICOM_Int2Str(num,(uint8 *)buf,20); /*lint !e64*/
    MMI_STRNTOWSTR(num_ptr,MMIWIDGET_CALENDAR_NUM_LENGTH,buf,20,MMIWIDGET_CALENDAR_NUM_LENGTH);/*lint !e64*/
    num_string.wstr_ptr = num_ptr;
    num_string.wstr_len = (uint16)MMIAPICOM_Wstrlen(num_string.wstr_ptr);
	
    return num_string;
}

LOCAL  void MMIWIDGET_Calendar_SetCalendarTextStyle(
													GUISTR_STYLE_T *style_ptr,
													GUI_ALIGN_E align,
													GUI_FONT_T  font,
													GUI_COLOR_T font_color
													)
{
    //SCI_ASSERT(PNULL != style_ptr);

    if (PNULL == style_ptr)
    {
        return;
    }
	
    style_ptr->align = align;
    style_ptr->font = font;
    style_ptr->font_color = font_color;
    style_ptr->line_space = 0;
}

LOCAL void  MMIWIDGET_Calendar_DisplayCalendar(
											   MMI_WIN_ID_T     win_id
											   )
{
    GUI_RECT_T win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
    GUI_POINT_T point = {0};
    // UILAYER_HANDLE_T layer_handle = 0;
    SCI_DATE_T              date = {0};
    MMIWIDGET_CALENDAR_INFO_T    calendar_info={0};
    MMIWIDGET_CALENDAR_INFO_T    calendar_pre_info={0};
    MMIWIDGET_CALENDAR_INFO_T    calendar_next_info={0};
	
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    if (!UILAYER_IsLayerActive(&lcd_dev_info))
    {
        return;
	}	
	
	UILAYER_ClearRect(&lcd_dev_info, win_rect);
        //显示背景图
        point.x = win_rect.left;
        point.y = win_rect.top;
        GUIRES_DisplayImg(
            &point,
            PNULL,
            PNULL,
            win_id,
            IMAGE_WIDGET_CALENDAR_BG,//IMAGE_WIDGET_1,
            &lcd_dev_info
            );

            //获取系统时间yh
            TM_GetSysDate(&date);
            //初始化WIDGET日历
#if 0//def BAR_PHONE_WIDGET 
            calendar_info.win_id = win_id;
            calendar_info.day = display_date.mday;
            calendar_info.month = display_date.mon;
            calendar_info.year = display_date.year;	
#else
            calendar_info.win_id = win_id;
            calendar_info.day = date.mday;
            calendar_info.month = date.mon;
            calendar_info.year = date.year;	
#endif
            calendar_info.first_day_of_week = MMIAPISET_GetFirstDayOfWeek();
            //画年月
            MMIWIDGET_Calendar_DrawYearMonth(&calendar_info, &lcd_dev_info);	  
            //画星期
            MMIWIDGET_Calendar_DrawWeeks( &calendar_info, &lcd_dev_info);
            //画每个月
            //用上个月的末尾和下个月的开头填充这个月的月历
            //如果不是1月，前一个月在同一年
            if (1 != calendar_info.month)
            {
            	calendar_pre_info.year = calendar_info.year;
            	calendar_pre_info.month = calendar_info.month - 1;
            }
            //如果是1月，前一个月不在同一年，且前一个月为12月
            else
            {
            	calendar_pre_info.year = calendar_info.year - 1;
            	calendar_pre_info.month = 12;
            }
            //如果不是12月，后一个月在同一年
            if (12 != calendar_info.month)
            {
            	calendar_next_info.year = calendar_info.year;
            	calendar_next_info.month = calendar_info.month + 1;
            }
            //如果是12月，后一个月不在同一年，且后一个月为1月
            else
            {
            	calendar_next_info.year = calendar_info.year + 1;
            	calendar_next_info.month = 1;
            }
            calendar_pre_info.win_id = win_id;
            calendar_next_info.win_id = win_id;
            MMIWIDGET_Calendar_DrawMonthDates(&calendar_info,&calendar_pre_info,&calendar_next_info,&lcd_dev_info);
            
            MMIWIDGET_Calendar_DrawCurrentDate(&calendar_info,&lcd_dev_info);
    
}

LOCAL void MMIWIDGET_Calendar_DrawYearMonth(
											MMIWIDGET_CALENDAR_INFO_T*    calendar_info_ptr,
											const GUI_LCD_DEV_INFO *dev_info_ptr       //in:                                   
											) 
{										
    GUI_RECT_T win_rect = {0};
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    GUI_RECT_T  month_rect = MMIWIDGET_CALENDAR_MONTH_RECT;
    GUI_RECT_T  year_rect = MMIWIDGET_CALENDAR_YEAR_RECT;
    GUI_RECT_T  month_rect_h = MMIWIDGET_CALENDAR_MONTH_RECT_H;
    GUI_RECT_T  year_rect_h = MMIWIDGET_CALENDAR_YEAR_RECT_H;   
    MMI_STRING_T year_str = {0};    
    MMI_STRING_T month_str = {0};
    uint16      year = 0;
    uint16      month = 0;
    uint8      yearstr[5] = {0};
    wchar       yearwstr[5] = {0};
    uint16      len = 0;
    MMI_TEXT_ID_T month_str_id[]={
                                    TXT_IDLE_SHORTCUT_OH_TIME_JAN,    
                                    TXT_IDLE_SHORTCUT_OH_TIME_FEB,
                                    TXT_IDLE_SHORTCUT_OH_TIME_MAR,
                                    TXT_IDLE_SHORTCUT_OH_TIME_APR,
                                    TXT_IDLE_SHORTCUT_OH_TIME_MAY,
                                    TXT_IDLE_SHORTCUT_OH_TIME_JUN,
                                    TXT_IDLE_SHORTCUT_OH_TIME_JUL,
                                    TXT_IDLE_SHORTCUT_OH_TIME_AUG,
                                    TXT_IDLE_SHORTCUT_OH_TIME_SEP,
                                    TXT_IDLE_SHORTCUT_OH_TIME_OCT,
                                    TXT_IDLE_SHORTCUT_OH_TIME_NOV,
                                    TXT_IDLE_SHORTCUT_OH_TIME_DEC
                                  };
    
    //SCI_ASSERT(NULL != calendar_info_ptr);        

    if (PNULL == calendar_info_ptr)
    {
        return;
    }
    year = calendar_info_ptr->year;
    month = calendar_info_ptr->month;	
    if(MMITHEME_IsMainScreenLandscape())
    {
        month_rect = month_rect_h;
        year_rect = year_rect_h;
    }
    
    len = sprintf((char*)yearstr,"%02d", year);            
    //SCI_ASSERT(len < sizeof(datestr));
    MMI_STRNTOWSTR( yearwstr, sizeof(yearwstr), (char*)yearstr, sizeof(yearstr), len);/*lint !e64*/
    year_str.wstr_ptr = yearwstr;
    year_str.wstr_len = len;			
    year_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, calendar_info_ptr->win_id, year_rect);
    //矫正右边区域
    MMK_GetWinRect(calendar_info_ptr->win_id, &win_rect);
    if(year_rect.right > win_rect.right)
    {
        year_rect.right = win_rect.right;	
    }
    //获取日历

    MMIWIDGET_Calendar_SetCalendarTextStyle(&text_style, ALIGN_RVMIDDLE, MMIWIDGET_CALENDAR_YEAR_TEXT_FONT, MMI_BLACK_COLOR);//MMIWIDGET_CALENDER_DARKGRAY_FONT_COLOR

    GUISTR_DrawTextToLCDInRect( 
    	(const GUI_LCD_DEV_INFO *)dev_info_ptr,
    	(const GUI_RECT_T      *)&year_rect,       //the fixed display area
    	(const GUI_RECT_T      *)&year_rect,       //用户要剪切的实际区域
    	(const MMI_STRING_T    *)&year_str,
    	&text_style,
    	state,
    	GUISTR_TEXT_DIR_AUTO
    	); 

    MMI_GetLabelTextByLang(month_str_id[month-1], &month_str);

    month_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, calendar_info_ptr->win_id, month_rect);

    MMIWIDGET_Calendar_SetCalendarTextStyle(&text_style, ALIGN_LVMIDDLE, MMIWIDGET_CALENDAR_MONTH_TEXT_FONT, MMI_BLACK_COLOR);//MMIWIDGET_CALENDER_DARKGRAY_FONT_COLOR

    GUISTR_DrawTextToLCDInRect( 
    	(const GUI_LCD_DEV_INFO *)dev_info_ptr,
    	(const GUI_RECT_T      *)&month_rect,       //the fixed display area
    	(const GUI_RECT_T      *)&month_rect,       //用户要剪切的实际区域
    	(const MMI_STRING_T    *)&month_str,
    	&text_style,
    	state,
    	GUISTR_TEXT_DIR_AUTO
    	); 	
}

LOCAL void MMIWIDGET_Calendar_DrawWeeks(
										MMIWIDGET_CALENDAR_INFO_T*    calendar_info_ptr,
										const GUI_LCD_DEV_INFO *dev_info_ptr       //in:     
										)
{
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_BOLD; 

    GUI_RECT_T   rect = {0};
    MMI_STRING_T string = {0};
    uint32       i = 0;

    //SCI_ASSERT( NULL != calendar_info_ptr );

    if (PNULL == calendar_info_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_Calendar_DrawWeeks PNULL == calendar_info_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_CALENDAR_897_112_2_18_3_7_56_181,(uint8*)"");
        return;
    }
	
    for( ; i < MMIWIDGET_CALENDAR_WEEKDAY_NUM; i++ )
    {
        GUI_COLOR_T color    = 0;
        uint32      week_day = ( calendar_info_ptr->first_day_of_week + i ) % MMIWIDGET_CALENDAR_WEEKDAY_NUM;

#ifdef WIDGET_WEEKEND_COLOR
        if ( 0 == week_day )
        {
            color = MMI_DARK_RED_COLOR;
        }
        else if( 6 == week_day )
        {
            color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
        }
        else
        {				
            color = MMI_GRAY_WHITE_COLOR;
        }
#else
        color = MMI_BLACK_COLOR;
#endif

        rect = MMIWIDGET_Calendar_GetDateRect( 0, i );  
        //
        if(MMITHEME_IsMainScreenLandscape())
        {
            rect.top = MMIWIDGET_CALENDAR_WEEK_TOP_H;
            rect.bottom = rect.top + MMIWIDGET_CALENDAR_LINE_HEIGHT_H;
        }
        else
        {
            rect.top = MMIWIDGET_CALENDAR_WEEK_TOP;
            rect.bottom = rect.top + MMIWIDGET_CALENDAR_LINE_HEIGHT;
        }
        rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, calendar_info_ptr->win_id, rect);      
        MMI_GetLabelTextByLang( s_week_text_id[ week_day ], &string );

        MMIWIDGET_Calendar_SetCalendarTextStyle(&text_style, ALIGN_HVMIDDLE, MMIWIDGET_CALENDAR_WEEKS_TEXT_FONT, color);
        // display
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)dev_info_ptr,
            (const GUI_RECT_T      *)&rect,       //the fixed display area
            (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&string,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            ); 
    }
}

LOCAL void MMIWIDGET_Calendar_DrawMonthDates(
											 MMIWIDGET_CALENDAR_INFO_T*    calendar_info_ptr,
											 MMIWIDGET_CALENDAR_INFO_T*    calendar_pre_info_ptr,
											 MMIWIDGET_CALENDAR_INFO_T*    calendar_next_info_ptr,
											 const GUI_LCD_DEV_INFO *dev_info_ptr       //in:                        
											 )
{
    GUI_RECT_T rect      = {0};
    uint32     row       = 0;
    uint32     col       = 0;
#ifdef WIDGET_PRE_NEXT_MONTH
    uint32  pre_month_end_col = 0;
    uint32  next_month_start_row = 0;
    uint32  next_month_start_col = 0;
    uint8      premonth_day =MMIWIDGET_Calendar_GetMonthday( calendar_pre_info_ptr->year, calendar_pre_info_ptr->month );
    uint8      nextmonth_day =MMIWIDGET_Calendar_GetMonthday( calendar_next_info_ptr->year, calendar_next_info_ptr->month );
#endif

    //SCI_ASSERT( PNULL != calendar_info_ptr );		

    if ( PNULL != calendar_info_ptr )		
    {
        uint8 day = 0;
        uint8 month_day =MMIWIDGET_Calendar_GetMonthday( calendar_info_ptr->year, calendar_info_ptr->month );
        
        for( day = 1; day <= month_day; day++ )
        {
            MMIWIDGET_Calendar_GetPosByDate( calendar_info_ptr, calendar_info_ptr->year, calendar_info_ptr->month, day, &row, &col );
            rect = MMIWIDGET_Calendar_GetDateRect( row, col );
            rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, calendar_info_ptr->win_id, rect);
#ifdef WIDGET_WEEKEND_COLOR
            if (0 == col)
            {
                MMIWIDGET_Calendar_DrawMonthDate( calendar_info_ptr, calendar_info_ptr->year, calendar_info_ptr->month,
                    day, month_day, &rect, MMIWIDGET_CALENDER_RED, dev_info_ptr);
            }
            else if (6 == col)
            {
                MMIWIDGET_Calendar_DrawMonthDate( calendar_info_ptr, calendar_info_ptr->year, calendar_info_ptr->month,
                    day, month_day, &rect, MMIWIDGET_CALENDER_BLUE, dev_info_ptr);
            }
            else
            {
                MMIWIDGET_Calendar_DrawMonthDate( calendar_info_ptr, calendar_info_ptr->year, calendar_info_ptr->month,
                    day, month_day, &rect, MMIWIDGET_CALENDER_BLACK, dev_info_ptr);     
            }
#else
            MMIWIDGET_Calendar_DrawMonthDate( calendar_info_ptr, calendar_info_ptr->year, calendar_info_ptr->month,
                day, month_day, &rect, MMIWIDGET_CALENDER_BLACK, dev_info_ptr);
#endif
        }
#ifdef WIDGET_PRE_NEXT_MONTH
        next_month_start_row = (col + 1) >= 7 ? 5 : row;
        next_month_start_col = (col + 1) >= 7 ? 0 : col + 1;
#endif
    }

#ifdef WIDGET_PRE_NEXT_MONTH
    if ( PNULL != calendar_pre_info_ptr && PNULL != calendar_info_ptr)		
    {
        uint8 day1 = 1;
        uint8 day_pre = premonth_day;
        MMIWIDGET_Calendar_GetPosByDate( calendar_info_ptr, calendar_info_ptr->year, calendar_info_ptr->month, day1, &row, &col );
        if(0 != col)
        {
            pre_month_end_col = col - 1;
            for (; pre_month_end_col  > 0; pre_month_end_col--)
            {
                rect = MMIWIDGET_Calendar_GetDateRect( 0, pre_month_end_col );
                rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, calendar_info_ptr->win_id, rect);
                MMIWIDGET_Calendar_DrawMonthDate( calendar_pre_info_ptr, calendar_pre_info_ptr->year, calendar_pre_info_ptr->month,
                day_pre, premonth_day, &rect, MMIWIDGET_CALENDER_GRAY, dev_info_ptr);
                day_pre--;
            }
            rect = MMIWIDGET_Calendar_GetDateRect( 0, 0 );
            rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, calendar_info_ptr->win_id, rect);
            MMIWIDGET_Calendar_DrawMonthDate( calendar_pre_info_ptr, calendar_pre_info_ptr->year, calendar_pre_info_ptr->month,
                day_pre, premonth_day, &rect, MMIWIDGET_CALENDER_GRAY, dev_info_ptr);			
        }
    }

    if ( PNULL != calendar_next_info_ptr && PNULL != calendar_info_ptr)	
    {
        uint8 day2 = 1;

        for( day2 = 1; day2 <= nextmonth_day; day2++ )
        {
            //MMIWIDGET_Calendar_GetPosByDate( calendar_info_ptr, calendar_info_ptr->year, calendar_info_ptr->month, day, &row, &col );
            rect = MMIWIDGET_Calendar_GetDateRect( next_month_start_row, next_month_start_col );
            rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, calendar_info_ptr->win_id, rect);
            MMIWIDGET_Calendar_DrawMonthDate( calendar_next_info_ptr, calendar_next_info_ptr->year, calendar_next_info_ptr->month,
            day2, nextmonth_day, &rect, MMIWIDGET_CALENDER_GRAY, dev_info_ptr);
            next_month_start_col++;
            if (next_month_start_col >= 7)
            {
                next_month_start_row++;
                next_month_start_col = 0;
            }
        }
    }
#endif    
}

LOCAL BOOLEAN MMIWIDGET_Calendar_DrawMonthDate(
											   MMIWIDGET_CALENDAR_INFO_T*    calendar_info_ptr,
											     uint16               year,
											    uint8                month,
											   uint8                day,
											   uint8                month_day,
											   const GUI_RECT_T*    rect_ptr,
											   MMIWIDGET_CALENDER_COLOR_E    color_type,
											   const GUI_LCD_DEV_INFO *dev_info_ptr       //in:                                
											   )
{
    BOOLEAN result = FALSE;
    GUISTR_STYLE_T      text_style      = {0}; 
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_BOLD;
    
    //SCI_ASSERT( PNULL != calendar_info_ptr );
    //SCI_ASSERT( PNULL != rect_ptr );

    if (PNULL == calendar_info_ptr || PNULL == rect_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_Calendar_DrawMonthDate PNULL != calendar_info_ptr || PNULL == rect_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_CALENDAR_1080_112_2_18_3_7_57_182,(uint8*)"");
        return result;
    }    
    
    if ( day <= month_day )
    {
        MMI_STRING_T num_string = MMIWIDGET_Calendar_CreateNumString( day );
        GUI_COLOR_T  color = MMI_BLACK_COLOR;
        switch(color_type)
        {
            case MMIWIDGET_CALENDER_BLACK:
                color = MMI_BLACK_COLOR;
                break;
                
            case MMIWIDGET_CALENDER_GRAY:
                color = MMI_DARK_GRAY_COLOR;
                break;
                
            case MMIWIDGET_CALENDER_RED:
                color = MMI_DARK_RED_COLOR;			
                break;
                
            case MMIWIDGET_CALENDER_BLUE:
                color = MMI_DARK_BLUE_COLOR;				
                break;

            default:
                result = FALSE;
                break;
        }
#ifdef CALENDAR_SUPPORT
        if (MMIAPISCH_IsOneDateHasSche(year, month, day)) // 今天有任务
        {
            uint16              img_width = 0;
            uint16              img_height = 0;
            GUI_RECT_T          img_rect = {0};
            GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_WIDGET_CALENDAR_SCHE, calendar_info_ptr->win_id);
            //右上角
            img_rect .top = rect_ptr->top;
            img_rect.left = rect_ptr->right - img_width + 1;
            img_rect.right = rect_ptr->right;
            img_rect.bottom = rect_ptr->bottom - img_height + 1;
            GUIRES_DisplayImg(PNULL,
                (GUI_RECT_T *)&img_rect,
                PNULL,
                calendar_info_ptr->win_id,
                IMAGE_WIDGET_CALENDAR_SCHE,
                (const GUI_LCD_DEV_INFO *)dev_info_ptr);
        }
#endif
        MMIWIDGET_Calendar_SetCalendarTextStyle(&text_style, ALIGN_HVMIDDLE, MMIWIDGET_CALENDAR_MONTH_DATE_TEXT_FONT, color);
        // display
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)dev_info_ptr,
            (const GUI_RECT_T      *)rect_ptr,       //the fixed display area
            (const GUI_RECT_T      *)rect_ptr,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&num_string,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );

        MMIAPICOM_DestroyString( num_string );

        result = TRUE;
    }
    
    return result;
}

#ifdef WIDGET_EDIT_SCHE
LOCAL MMI_RESULT_E  HandleTpPressUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_FALSE;
    GUI_POINT_T point = {0};
    GUI_RECT_T day_rect = {0};
    uint8           	row_count = 0;
    uint8           	col_count = 0; 	
    uint8           	total_count = 0;
    uint8           	select_day = 0;	
    uint8           	firstday_inweek = 0;
    SCI_DATE_T              date = {0};
    MMIWIDGET_CALENDAR_INFO_T    calendar_info={0};
#ifdef WIDGET_PRE_NEXT_MONTH
    MMIWIDGET_CALENDAR_INFO_T    calendar_pre_info={0};
    MMIWIDGET_CALENDAR_INFO_T    calendar_next_info={0};    
    uint8      pre_month_day = 0;    
#endif
    uint8      month_day = 0;
#ifdef CALENDAR_SUPPORT
    MMI_CALENDAR_T* calendar_ptr = &g_calendar_show;
#endif
    BOOLEAN in_rect = FALSE;
    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);

    TM_GetSysDate(&date);
    //初始化WIDGET日历
    calendar_info.win_id = win_id;
    calendar_info.day = date.mday;
    calendar_info.month = date.mon;
    calendar_info.year = date.year;

    //局部变量初始化	
    calendar_info.first_day_of_week = CALENDAR_GetFirstday( calendar_info.year, calendar_info.month );
    month_day =MMIWIDGET_Calendar_GetMonthday( calendar_info.year, calendar_info.month );

    firstday_inweek = calendar_info.first_day_of_week;

    //在日历表区域
    for(row_count = 0; row_count < MMIWIDGET_CALENDAR_LINENUM; row_count++)
    {
        for(col_count = 0; col_count < MMIWIDGET_CALENDAR_COLNUM; col_count++)
        {
            day_rect = MMIWIDGET_Calendar_GetDateRect(row_count, col_count);            
            if(GUI_PointIsInRect(point,  day_rect))
            {
                in_rect = TRUE;
                break;
            }
        }

        if(in_rect)
        {
            break;
        }
    }

    if(in_rect)
    {
        total_count = row_count * WEEKDAY_NUM + col_count + 1;
        firstday_inweek = ( WEEKDAY_NUM + firstday_inweek - 0 ) % WEEKDAY_NUM;

        select_day  = total_count - firstday_inweek;
        if (select_day <= month_day && 0 != select_day)
        {
            if (select_day >= 1 && select_day <= 31)
            {
                s_date.year = date.year;
                s_date.mon = date.mon;
                s_date.mday = select_day;
                recode = MMI_RESULT_TRUE;
            }
        }
#ifdef WIDGET_PRE_NEXT_MONTH
        else
        {
            if (total_count <= firstday_inweek)
            {
                //用上个月的末尾和下个月的开头填充这个月的月历
                //如果不是1月，前一个月在同一年
                if (1 != calendar_info.month)
                {
                    calendar_pre_info.year = calendar_info.year;
                    calendar_pre_info.month = calendar_info.month - 1;
                }
                //如果是1月，前一个月不在同一年，则前一个月为12月
                else
                {
                    calendar_pre_info.year = calendar_info.year - 1;
                    calendar_pre_info.month = 12;
                }
                pre_month_day = MMIWIDGET_Calendar_GetMonthday( calendar_pre_info.year, calendar_pre_info.month );
                select_day  = pre_month_day - (firstday_inweek - total_count);
                s_date.year = calendar_pre_info.year;
                s_date.mon = calendar_pre_info.month;
                s_date.mday = select_day;
            }

            else
            {
                //如果不是12月，后一个月在同一年
                if (12 != calendar_info.month)
                {
                    calendar_next_info.year = calendar_info.year;
                    calendar_next_info.month = calendar_info.month + 1;
                }
                //如果是12月，后一个月不在同一年，则后一个月为1月
                else
                {
                    calendar_next_info.year = calendar_info.year + 1;
                    calendar_next_info.month = 1;
                }
                select_day  = total_count - month_day - firstday_inweek;
                s_date.year = calendar_next_info.year;
                s_date.mon = calendar_next_info.month;
                s_date.mday = select_day;

            }
            recode = MMI_RESULT_TRUE;	
        }	
#endif
    }
#ifdef CALENDAR_SUPPORT
    SCI_MEMSET(calendar_ptr, 0x00, sizeof(MMI_CALENDAR_T));

    //calendar_ptr->rect          = rect;     //控件的显示区域
    calendar_ptr->solar_year    = s_date.year;     //年
    calendar_ptr->solar_month   = s_date.mon;        //月
    calendar_ptr->solar_date    = s_date.mday;        //日
#endif
    return recode;
}

/*****************************************************************************/
//     Description : 计算某年某月第一天为星期几
//    Global resource dependence : 
//  Author: Jibin
//    Note:
/*****************************************************************************/
LOCAL uint8 CALENDAR_GetFirstday(
                              uint16    solar_year,
                              uint8     solar_month
                              )
{
    uint8   ret_val = 0;
    uint16  year = (uint16)(solar_year-1);
    uint16  year_plus = (uint16)(year + year/4 - year/100 + year/400);   //等于公元元年到上年12月31日所经过的天数减去年数乘上364的值, 除以7的余数就是那天的星期
    
    if ((0 < solar_month) &&
        (MMIWIDGET_CALENDAR_MONTHNUM >= solar_month))
    {
        if (GUI_IsLeapYear(solar_year))
        {
            ret_val = (uint8)(( 1 + s_widget_calendar_first_day_leap[solar_month - 1] + year_plus ) % WEEKDAY_NUM);
        }
        else
        {
            ret_val = (uint8)(( 1 + s_widget_calendar_first_day[solar_month - 1] + year_plus ) % WEEKDAY_NUM);
        }
    }
    else
    {
        //SCI_TRACE_LOW:"CALENDAR_GetFirstday:solar_month %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_CALENDAR_1313_112_2_18_3_7_57_183,(uint8*)"d",solar_month);
    }

    return (ret_val);
}
#endif

LOCAL void MMIWIDGET_Calendar_DrawCurrentDate(
											 MMIWIDGET_CALENDAR_INFO_T*    calendar_info_ptr,
											 const GUI_LCD_DEV_INFO *dev_info_ptr       //in:                        
											 )
{
    GUISTR_STYLE_T      text_style      = {0}; 
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    MMI_STRING_T num_string = {0};
    GUI_RECT_T rect = MMIWIDGET_CALENDAR_DAY_RECT;
    GUI_RECT_T rect_h = MMIWIDGET_CALENDAR_DAY_RECT_H;

    if (PNULL == calendar_info_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_Calendar_DrawCurrentDate PNULL != calendar_info_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_CALENDAR_1332_112_2_18_3_7_57_184,(uint8*)"");
        return;
    }       
    
    if(MMITHEME_IsMainScreenLandscape())
    {
        rect = rect_h;
    }
    num_string = MMIWIDGET_Calendar_CreateNumString( calendar_info_ptr->day);

    rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, calendar_info_ptr->win_id, rect);

    GUI_FillRect(dev_info_ptr, rect, GUI_RGB2RGB565(255,54,0));
        
    MMIWIDGET_Calendar_SetCalendarTextStyle(&text_style, ALIGN_HVMIDDLE, MMIWIDGET_CALENDAR_MONTH_DAY_TEXT_FONT, MMI_WHITE_COLOR);
    // display
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)dev_info_ptr,
        (const GUI_RECT_T      *)&rect,       //the fixed display area
        (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&num_string,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    MMIAPICOM_DestroyString( num_string );
}

#ifdef BAR_PHONE_WIDGET 
LOCAL void MovePrevMonth(
                         SCI_DATE_T  date_in,
                         SCI_DATE_T  *date_out_ptr
                         )
{
    uint16  year   = date_in.year;
    uint8   month   = date_in.mon;
    uint8   date    = date_in.mday;
    uint8   monthday = 0;
    uint8   weekday = 0; 

    SCI_ASSERT(NULL != date_out_ptr);/*assert verified*/

    if (1 != month)	//非1月，移到上月
    {
	    month--;
        monthday = MMICALENDAR_GetMonthday(year, month);
        if (date > monthday)	//日期超过最后一天，移到最后一天
        {
            date = monthday;
        }
    }
    else if (QUERY_YEAR_BOTTOM < year)	//1月且年没有超出范围，移到上年12月
    {
        year--;
        month = DECEMBER;
        monthday = MMICALENDAR_GetMonthday(year, month);
        if (date > monthday)	//日期超过最后一天，移到最后一天
        {
            date = monthday;
        }
    }
    else if (QUERY_YEAR_BOTTOM == year)
    {
        year = QUERY_YEAR_TOP;
        month = DECEMBER;
        monthday = MMICALENDAR_GetMonthday(year, month);
        if (date > monthday)	//日期超过最后一天，移到最后一天
        {
            date = monthday;
        }
    }
    weekday = MMICALENDAR_GetWeekday(year,month,date);
    date_out_ptr->year = year;
    date_out_ptr->mon = month;
    date_out_ptr->mday = date;
    date_out_ptr->wday= weekday;
    return;
}

LOCAL void MoveNextMonth(
                         SCI_DATE_T  date_in,
                         SCI_DATE_T  *date_out_ptr
                         )
{
    uint16  year   = date_in.year;
    uint8   month   = date_in.mon;
    uint8   date    = date_in.mday;
    uint8   monthday = 0; 
    uint8   weekday = 0; 

    SCI_ASSERT(NULL != date_out_ptr);/*assert verified*/

    if (DECEMBER != month)	//非12月，移到下月
    {
	    month++;
        monthday = MMICALENDAR_GetMonthday(year, month);
        if (date > monthday)	//日期超过最后一天，移到最后一天
        {
            date = monthday;
        }
    }
    else if (QUERY_YEAR_TOP > year)	//12月且年没有超出范围，移到下年的1月
    {
        year++;
        month = 1;
        monthday = MMICALENDAR_GetMonthday(year, month);
        if (date > monthday)	//日期超过最后一天，移到最后一天
        {
            date = monthday;
        }
    }
    else if (QUERY_YEAR_TOP == year)
    {
        year = QUERY_YEAR_BOTTOM;
        month = 1;
        monthday = MMICALENDAR_GetMonthday(year, month);
        if (date > monthday)	//日期超过最后一天，移到最后一天
        {
            date = monthday;
        }
    }
    weekday = MMICALENDAR_GetWeekday(year,month,date);
    date_out_ptr->year = year;
    date_out_ptr->mon = month;
    date_out_ptr->mday = date;
    date_out_ptr->wday= weekday;

    return;
}

#endif


#endif
