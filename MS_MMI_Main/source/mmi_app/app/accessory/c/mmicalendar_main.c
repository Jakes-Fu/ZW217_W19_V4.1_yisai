/*****************************************************************************
** File Name:      mmicalendar_main.c                                               
** Author:           mark.hu    
** Date:            2005/0907
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe calendar window             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2005/09/07    mark.hu              Create                                 *
** 2006/08/09    liqing.peng          Modify
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef CALENDAR_SUPPORT
#include "mmi_app_accessory_trc.h"
#include "window_parse.h"
//#include "win.def"
#include "mmk_app.h"
#include "mmi_text.h"
//#include "sci_types.h"
#include "guisoftkey.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "guifont.h"
#include "guilabel.h"
#include "mmi_theme.h"
#include "mmicalendar_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmischedule_export.h"
#include "mmialarm_export.h"
#include "mmi_appmsg.h"
#include "mmi_solarlunar.h"
#include "mmialarm_id.h"
#include "mmialarm_text.h"
#include "mmialarm_image.h"
//#include "mmialarm_menutable.h"
#include "guimenu.h"
#include "mmiacc_text.h"
#include "mmipub.h"
#include "mmi_image.h"
#include "mmiacc_position.h"
#include "mmi_mainmenu_export.h"
#include "guires.h"
#include "guiedit.h"
#include "guistring.h"
#include "mmi_default.h"
#ifdef MMI_PDA_SUPPORT
#include "guiform.h"
#endif
#include "guirichtext.h"
#ifdef MMI_CALENDAR_SLIDE
#include "Mmk_tp.h"
#endif
#include "guitab.h"
#include "mmiacc_image.h"
#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
#include "mmiwidget_id.h"
#endif
#ifdef MMI_SLIDE_SUPPORT
#include "mmi_slide.h"
#endif
#include "guistatusbar.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define DATE_LENGTH                     2       //日期(1~31)的长度
#define LINENUM                         6       //日历上显示日期的行数
#define COLNUM                          WEEKDAY_NUM       //日历上显示日期的列数
#define DECEMBER                        12      //十二月
#define NUM_LENGTH                      10//5       //uint16整数的最大位数,转换成阿拉伯语的数字为unicode，0-9对应为0x06F0-0X06F9
       
#define CALENDAR_YEAR_COLOR             MMI_BLUE_COLOR//0X2157
#define CALENDAR_MONTH_COLOR            MMI_BLUE_COLOR//0X2157
#define CALENDAR_DAY_RECT_NUM           (LINENUM*COLNUM) // 6行，7列
#define CALENDAR_TASK_COLOR             0X72BC

#define CALENDAR_LEAP_MONTH_STR_LEN      30
#define CALENDAR_FESTIVAL_STRING_LEN     (CALENDAR_LEAP_MONTH_STR_LEN + 10)

#define MMI_CALENDAR_BG_COLOR  MMI_WHITE_COLOR
#define MMI_CALENDAR_MONTH_BG_COLOR   MMI_DARK_YELLOW_COLOR
#define MMI_CALENDAR_SCH_BG_COLOR   MMI_GRAY_COLOR
#ifdef MMI_GUI_STYLE_MINISCREEN
#define MMI_CALENDAR_MONTHLY_TITLE_COLOR  0x3B9801
#endif
#define MMI_CALENDAR_QINGMING_FES_20_CENTRY    48100
#define MMI_CALENDAR_QINGMING_FES_19_CENTRY    55900

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

//solar date struct

typedef enum
{/*lint !e749*/
    CALENDAR_OPT_ROOT,/*lint !e749*/
    CALENDAR_OPT_LOOK,
    CALENDAR_OPT_NEW,
    CALENDAR_OPT_ALL,
    CALENDAR_OPT_CLEAR_TODAY,
    CALENDAR_OPT_CLEAR_ALL,    
    CALENDAR_OPT_QUERY,/*lint !e749*/
    CALENDAR_OPT_MEMO,/*lint !e749*/
    CALENDAR_OPT_WEEK,
    CALENDAR_OPT_MONTH,/*lint !e751*/
    CALENDAR_OPT_LUNAR_DETAIL,
    CALENDAR_OPT_MARK,
    CALENDAR_OPT_IMPORT,
    CALENDAR_OPT_DAY
}CALENDAR_OPTION_MENU_NODE_E;/*lint !e751*/

typedef struct
{
    //GUI_RECT_T day_rect;
    BOOLEAN    is_valid;
    CALE_SOLAR_DATE_T date;
}CALE_WEEKLY_DAYRECT_INFO;


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

LOCAL const MMI_TEXT_ID_T s_week_text_id[COLNUM] =
{
    TXT_SHORT_SUNDAY,
	TXT_SHORT_MONDAY,
	TXT_SHORT_TUESDAY,
	TXT_SHORT_WEDNESDAY,
	TXT_SHORT_THURSDAY,
	TXT_SHORT_FRIDAY,
	TXT_SHORT_SATURDAY
};

// 按周日期中每一天的位置
LOCAL CALE_WEEKLY_DAYRECT_INFO s_weekly_dayrect_info[WEEKDAY_NUM] =//该数组不是常量,可赋值
{
    {FALSE, {0}},
    {FALSE, {0}},
    {FALSE, {0}},
    {FALSE, {0}},
    {FALSE, {0}},
    {FALSE, {0}},
    {FALSE, {0}}
};

LOCAL  GUI_LCD_DEV_INFO s_calendar_lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
#ifdef MMI_CALENDAR_SLIDE
LOCAL  GUI_LCD_DEV_INFO s_first_lcd_dev_info = { 0,UILAYER_NULL_HANDLE };
LOCAL  GUI_LCD_DEV_INFO s_second_lcd_dev_info = { 0,UILAYER_NULL_HANDLE };
LOCAL  GUI_LCD_DEV_INFO s_statusbar_dev_info = { 0,UILAYER_NULL_HANDLE };
#ifndef MMI_PDA_SUPPORT
LOCAL  GUI_LCD_DEV_INFO s_softkey_dev_info = { 0,UILAYER_NULL_HANDLE };
#endif
LOCAL  GUI_LCD_DEV_INFO *s_cur_lcd_dev_info_ptr=PNULL;
LOCAL MMI_SLIDE_CALENDAR_T   s_slide_calendar={0};
#define MMICALENDAR_SLIDE_REDRAW_TIME               10
#define MMICALENDAR_SLIDE_FLING_TIME               10
#ifdef MMI_CALENDAR_WIN_SLIDE
#define CALENDAR_TP_MOVE_DIS      (MMISLIDE_TP_MOVE_DIS-1)
#else
#define CALENDAR_TP_MOVE_DIS      5
#endif
#define TOUCH_FLING_TIME                ((float)0.05)
#define SLIDE_RECOVER_STEP                10
#endif
#ifdef MMI_CALENDAR_WIN_SLIDE
LOCAL MMI_SLIDE_HANDLE s_win_slide_handle=MMI_SLIDE_HANDLE_INVALID;
#endif
#ifdef TOUCH_PANEL_SUPPORT
LOCAL MMI_WIN_ID_T s_cur_tp_win=0;
#endif
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
MMI_CALENDAR_T g_calendar_show = {0};

LOCAL MMISCH_LIST_WIN_PARAM_T      *s_win_list_param     = NULL;
LOCAL MMISCH_DETAIL_WIN_PARAM_T    *s_win_param          = NULL;

LOCAL BOOLEAN                      s_is_today_has_periodsch  = FALSE;
LOCAL BOOLEAN                      s_is_del_periodsch  = FALSE;

LOCAL BOOLEAN s_is_appointed_date=FALSE;
LOCAL MMISET_DATE_T  s_appointed_date={0};
LOCAL GUI_RECT_T  s_weekly_down_rect={0};

LOCAL MMI_DAILY_CALENDAR_T   s_daily_calendar={0};
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

#ifdef MMI_GUI_STYLE_MINISCREEN
/*****************************************************************************/
//    Description : RGB888 translate to RGB565
//    Global resource dependence :
//    Author:
//    Note:
/*****************************************************************************/
LOCAL __inline uint16 RGB888TO565(uint32 rgb888);
#endif

/*****************************************************************************/
//  Description : to handle the message of waiting window
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWaitWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);

/*****************************************************************************/
//    Description : calendar main window
//    Global resource dependence : 
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCalendarWinMsg(
									MMI_WIN_ID_T        win_id, 
									MMI_MESSAGE_ID_E    msg_id, 
									DPARAM              param
									);

/*****************************************************************************/
//    Description : calendar option window
//    Global resource dependence : 
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCalendarOptionWinMsg(
									    MMI_WIN_ID_T        win_id, 
									    MMI_MESSAGE_ID_E    msg_id, 
									    DPARAM              param
									    );

/*****************************************************************************/
//    Description : calendar weekly display window
//    Global resource dependence : 
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCalendarWeeklyWinMsg(
									        MMI_WIN_ID_T        win_id, 
									        MMI_MESSAGE_ID_E    msg_id, 
									        DPARAM              param
									        );

/*****************************************************************************/
//    Description : calendar query window
//    Global resource dependence : 
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCalendarQueryWinMsg(
									        MMI_WIN_ID_T        win_id, 
									        MMI_MESSAGE_ID_E    msg_id, 
									        DPARAM              param
									        );
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
/*****************************************************************************/
//    Description:  format lunar to string
//    Global resource dependence :
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL void FormatLunarInfo( uint16  iYear,
                            uint16  iMonth,
                            uint16  iDate,
                            wchar   *monthday_ptr,
                            uint16   *monthday_len_ptr,
                            wchar   *year_ptr,
                            uint16   *year_len_ptr);
#endif
/*****************************************************************************/
//    Description : get date rect
//    Global resource dependence : 
//    Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetDateRect(
                             MMI_CALENDAR_T* calendar_ctrl_ptr,
                             uint32               row,
                             uint32               col
                             );

/*****************************************************************************/
//    Description : create num string
//    Global resource dependence : 
//    Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL MMI_STRING_T CreateNumString(
                                   uint16 num
                                   );
/*****************************************************************************/
//    Description : draw one date
//    Global resource dependence : 
//    Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN DrawMonthDate(
                            MMI_CALENDAR_T* calendar_ctrl_ptr,
                            uint16               year,
                            uint8                month,
                            uint8                day,
                            uint8                month_day,
                            GUI_RECT_T*    rect_ptr,
                            BOOLEAN              is_highlight
                            );

/*****************************************************************************/
//    Description : get pos by date
//    Global resource dependence : 
//    Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void GetPosByDate( 
                        MMI_CALENDAR_T*    calendar_ctrl_ptr,
                        uint16                  year,
                        uint8                   month,
                        uint8                   day,
                        uint32*                 row_ptr,
                        uint32*                 col_ptr
                        );

/*****************************************************************************/
//    Description : draw date of a month
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL void DrawMonthDates(
                          MMI_CALENDAR_T *calendar_ctrl_ptr
                          );

/*****************************************************************************/
//    Description : draw calendar main window
//    Global resource dependence : 
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL void DrawMonthCalendar(
                             MMI_CALENDAR_T *calendar_ctrl_ptr,
                             BOOLEAN             is_draw_all
                             );

/*****************************************************************************/
//    Description : draw weekly window
//    Global resource dependence : 
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL void DrawWeekCalendar(
                            MMI_CALENDAR_T *calendar_ctrl_ptr,
                            BOOLEAN             is_draw_all
                            );

/*****************************************************************************/
//    Description : draw week date
//    Global resource dependence : 
//    Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawWeekDate(
                        MMI_CALENDAR_T* calendar_ctrl_ptr,
                        GUI_RECT_T*    rect_ptr,
                        uint16               year,
                        uint8                month,
                        uint8                day,
                        uint32               row,
                        BOOLEAN              is_highlight
                        );

/*****************************************************************************/
//    Description : draw week dates
//    Global resource dependence : 
//    Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawWeekDates(
                         MMI_CALENDAR_T* calendar_ctrl_ptr
                         );

/*****************************************************************************/
//    Description : move one month backward
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL void MovePrevMonth(
                         CALE_SOLAR_DATE_T  date_in,
                         CALE_SOLAR_DATE_T  *date_out_ptr
                         );

/*****************************************************************************/
//    Description : move one month forward
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL void MoveNextMonth(
                         CALE_SOLAR_DATE_T  date_in,
                         CALE_SOLAR_DATE_T  *date_out_ptr
                         );

/*****************************************************************************/
//    Description : move one day backward
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL void MovePrevDay(
                         CALE_SOLAR_DATE_T  date_in,
                         CALE_SOLAR_DATE_T  *date_out_ptr
                       );

/*****************************************************************************/
//    Description : move one day forward
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL void MoveNextDay(
                         CALE_SOLAR_DATE_T  date_in,
                         CALE_SOLAR_DATE_T  *date_out_ptr
                       );

/*****************************************************************************/
//    Description : move one week backward
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL void MovePrevWeek(
                         CALE_SOLAR_DATE_T  date_in,
                         CALE_SOLAR_DATE_T  *date_out_ptr
                         );

/*****************************************************************************/
//    Description : move one week forward
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL void MoveNextWeek(
                         CALE_SOLAR_DATE_T  date_in,
                         CALE_SOLAR_DATE_T  *date_out_ptr
                         );

/*****************************************************************************/
//    Description : 日历控件的参数设置
//    Global resource dependence : 
//    Author: Jibin
//    Note:
/*****************************************************************************/
LOCAL void MainCALENDAR_SetParam(     //返回PNULL，表示初始化失败，不等于PNULL，表示初始化成功。
                                  MMI_CALENDAR_T       *calendar_ptr,
                                  uint16 			        solar_year,	    //阳历年		
                                  uint8 			        solar_month,	//阳历月
                                  uint8			            solar_date	    //阳历日
                                  );

/*****************************************************************************/
//    Description : move days backward
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL void MoveDays(
                    CALE_SOLAR_DATE_T  date_in,
                    uint16             mov_days,
                    BOOLEAN            is_next,
                    CALE_SOLAR_DATE_T  *date_out_ptr
                    );

/*****************************************************************************/
//    Description : compare date
//    Global resource dependence : 
//    Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL int8 CompareDate(
                       CALE_SOLAR_DATE_T  left_date,
                       CALE_SOLAR_DATE_T  right_date
                       );

/*****************************************************************************/
//    Description : 日历控件按键的辅助处理
//    Global resource dependence : 
//    Author: Jibin
//    Note:
/*****************************************************************************/
LOCAL void HandleCalendarKey(
                             MMI_CALENDAR_T* 	calendar_ctrl_ptr,
                             CALENDAR_MOVE_MODE_E 	mode
                             );

/*****************************************************************************/
// 	Description : set redraw mode
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void SetRedrawMode(
                         MMI_CALENDAR_T* calendar_ctrl_ptr,
                         CALENDAR_REDRAW_MODE_E   redraw_mode		//IN:
                         );

/*****************************************************************************/
// 	Description : set redraw mode
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void ResetRedrawMode(
                           MMI_CALENDAR_T* calendar_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : draw background
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawBackground(
                          MMI_CALENDAR_T* calendar_ctrl_ptr
                          );

/*****************************************************************************/
//  Description : draw background
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawBackgroundByRect(
                                MMI_CALENDAR_T* calendar_ctrl_ptr,
                                const GUI_RECT_T*   rect_ptr
                                );

/*****************************************************************************/
//    Description : 日历控件的初始化
//    Global resource dependence : 
//    Author: Jibin
//    Note:
/*****************************************************************************/
LOCAL void* MainCALENDAR_Init(
                              GUI_RECT_T rect //日历的显示区域
                              );

/*****************************************************************************/
//    Description : DrawWeekBackgroundLines
//    Global resource dependence : 
//    Author: zhaohui
//    Note:
/*****************************************************************************/
LOCAL void DrawWeekBackgroundLines( void );

#if 0
/*****************************************************************************/
//    Description : draw calendar background WeekDateRect
//    Global resource dependence : 
//    Author: zhaohui
//    Note:
/*****************************************************************************/
LOCAL void DrawWeekDateRect(
                           const GUI_RECT_T* rect_ptr
                           );
#endif
/*****************************************************************************/
// 	Description : set isAppointedDate
//	Global resource dependence : s_is_appointed_date
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SetIsAppointedDate(BOOLEAN is_appointed);

/*****************************************************************************/
// 	Description : get isAppointedDate
//	Global resource dependence : s_is_appointed_date
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN GetIsAppointedDate(void);

#if 0
/*****************************************************************************/
// 	Description : set AppointedDate
//	Global resource dependence : s_appointed_date
//  Author: renyi.hu
//	Note:
/*****************************************************************************/
LOCAL void SetAppointedDate(uint8 date,uint8 month,uint16 year);
#endif
/*****************************************************************************/
// 	Description : Get AppointedDate
//	Global resource dependence : s_appointed_date
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL MMISET_DATE_T* GetAppointedDate(void);

#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:  renyi.hu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleViewLunarDetailWinMsg(
                                             MMI_WIN_ID_T		win_id,
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM				param
                                             );
#endif
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
/*****************************************************************************/
// 	Description : set lunar detail win
//	Global resource dependence :
//  Author:  renyi.hu
//	Note:
/*****************************************************************************/
LOCAL void InitLunarDetail(
                                MMISET_DATE_T*	date,
                                MMI_CTRL_ID_T ctrl_id
                                );
#endif
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
/*****************************************************************************/
// Description : set common character
// Global resource dependence :
// Author:renyi.hu
// Note:
/*****************************************************************************/
LOCAL void SetCommonCharacter( GUIRICHTEXT_ITEM_T* p_item );
#endif
/*****************************************************************************/
// Description : GetCurLcdInfo
// Global resource dependence :
// Author:renyi.hu
// Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO * GetCurLcdInfo( void);
#ifdef MMI_CALENDAR_SLIDE
/*****************************************************************************/
//  Description : create  latyer
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CalendarCreateLayer(GUI_LCD_DEV_INFO *dev_info_ptr,MMI_HANDLE_T handle);

/*****************************************************************************/
// Description : SetCurLcdInfo
// Global resource dependence :
// Author:renyi.hu
// Note:
/*****************************************************************************/
LOCAL void SetCurLcdInfo( GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : HandleSlideTpDown
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleSlideTpDown(
                                GUI_POINT_T       *tp_point_ptr,    // [in]
                                MMI_SLIDE_CALENDAR_T    *slide_calendar_ptr   // [in]
                                );
/*****************************************************************************/
//  Description : to check whether need to redraw
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
__inline LOCAL BOOLEAN IsTpMoved(
                                 MMI_SLIDE_CALENDAR_T    *slide_calendar_ptr,  // [in]
                                 GUI_POINT_T    *point_ptr
                                 );

/*****************************************************************************/
//  Description : HandleSlideTpDown
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleSlideTpMove(
                                GUI_POINT_T       *point_ptr,    // [in]
                                MMI_SLIDE_CALENDAR_T    *slide_calendar_ptr,    // [in]
                                MMI_HANDLE_T    handle,
                                CALENDAR_VIEW_TYPE_E view_type
                                );

/*****************************************************************************/
//  Description : handle timer
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSlideCalendarTimer(
                                       MMI_SLIDE_CALENDAR_T    *slide_calendar_ptr, // [in]
                                       DPARAM           param           // [in]
                                       );

/*****************************************************************************/
// Description : StartSlideRedrawTimer
// Global resource dependence :
// Author:renyi.hu
// Note:
/*****************************************************************************/
LOCAL void StartSlideRedrawTimer( MMI_SLIDE_CALENDAR_T *slide_calendar_ptr);

/*****************************************************************************/
// Description : StopSlideRedrawTimer
// Global resource dependence :
// Author:renyi.hu
// Note:
/*****************************************************************************/
LOCAL void StopSlideRedrawTimer( MMI_SLIDE_CALENDAR_T *slide_calendar_ptr);

/*****************************************************************************/
// Description : InitSlidecalendar
// Global resource dependence :
// Author:renyi.hu
// Note:
/*****************************************************************************/
LOCAL void InitSlidecalendar( MMI_SLIDE_CALENDAR_T *slide_calendar_ptr,
                                MMI_HANDLE_T    handle);

/*****************************************************************************/
// Description : set common character
// Global resource dependence :
// Author:renyi.hu
// Note:
/*****************************************************************************/
LOCAL void ExitSlidecalendar( MMI_SLIDE_CALENDAR_T *slide_calendar_ptr,BOOLEAN is_update_calendar);

/*****************************************************************************/
//  Description : handle timer
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void UpdateMoveDirection( MMI_SLIDE_CALENDAR_T    *slide_calendar_ptr);

/*****************************************************************************/
// Description : StartSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StartSlideFlingTimer( MMI_SLIDE_CALENDAR_T *slide_calendar_ptr);

/*****************************************************************************/
// Description : StopSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StopSlideFlingTimer( MMI_SLIDE_CALENDAR_T *slide_calendar_ptr);

/*****************************************************************************/
// Description : StartSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StartSlideRecoverTimer( MMI_SLIDE_CALENDAR_T *slide_calendar_ptr);

/*****************************************************************************/
// Description : StopSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StopSlideRecoverTimer( MMI_SLIDE_CALENDAR_T *slide_calendar_ptr);

/*****************************************************************************/
//  Description : HandleSlideTpDown
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleSlideTpUp(
                                GUI_POINT_T       *point_ptr,    // [in]
                                MMI_SLIDE_CALENDAR_T    *slide_calendar_ptr ,   // [in]
                                CALENDAR_VIEW_TYPE_E view_type
                                );

/*****************************************************************************/
//  Description : create  latyer
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CalendarCreateStatusBarLayer(GUI_LCD_DEV_INFO *dev_info_ptr,MMI_HANDLE_T handle);

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : create  latyer
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CalendarCreateSoftkeyLayer(GUI_LCD_DEV_INFO *dev_info_ptr,MMI_HANDLE_T handle);
#endif

#endif

#ifndef MMI_GUI_STYLE_MINISCREEN
LOCAL void DrawMonthDateRectLine(uint16 row_num);
#endif

#ifdef MMI_CALENDAR_PLAY_EFFECT
/*****************************************************************************/
// Description : playCalendareffect
// Global resource dependence :
// Author:renyi.hu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN playCalendareffect(CALENDAR_MOVE_MODE_E move_mode,CALENDAR_VIEW_TYPE_E view_type);
#endif

#ifdef MMI_CALENDAR_DAILY_SUPPORT
/*****************************************************************************/
//    Description : calendar weekly display window
//    Global resource dependence : 
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCalendarDailyWinMsg(
									        MMI_WIN_ID_T        win_id,
									        MMI_MESSAGE_ID_E    msg_id,
									        DPARAM              param
									        );

/*****************************************************************************/
//    Description :
//    Global resource dependence :
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL int GetCurHourDisSchduleText(wchar*text,uint16 *text_len,uint16 text_max_len,uint8 hour);

/*****************************************************************************/
//    Description : d
//    Global resource dependence :
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN IsInDailyItemRange(int16 y);

/*****************************************************************************/
//    Description : d
//    Global resource dependence :
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL void DrawDailyItem(
                             MMI_DAILY_CALENDAR_T* daily_calendar
                             );

/*****************************************************************************/
//    Description : d
//    Global resource dependence :
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL void DrawDailyTitle(
                             MMI_DAILY_CALENDAR_T* daily_calendar
                             );

/*****************************************************************************/


/*****************************************************************************/
//    Description :
//    Global resource dependence :
//    Author:
//    Note:
/*****************************************************************************/
LOCAL void DrawDailyCalendar(
                             MMI_DAILY_CALENDAR_T* daily_calendar
                             );
#endif 
#if (defined TOUCH_PANEL_SUPPORT && defined MMI_CALENDAR_DAILY_SUPPORT)
/*****************************************************************************/
// Description : set common character
// Global resource dependence :
// Author:renyi.hu
// Note:
/*****************************************************************************/
LOCAL void ExitSlideDailycalendar( MMI_DAILY_CALENDAR_T* daily_calendar);

/*****************************************************************************/
//  Description : HandleSlideTpDown
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleDailySlideTpDown(
                                GUI_POINT_T       *tp_point_ptr,    
                                MMI_DAILY_CALENDAR_T* daily_calendar
                                );
/*****************************************************************************/
//  Description : HandleSlideTpDown
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleDailySlideTpMove(
                                GUI_POINT_T       *point_ptr,    
                                MMI_DAILY_CALENDAR_T* daily_calendar
                                );

/*****************************************************************************/
//  Description : handle timer
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDailySlideTimer(
                                       MMI_DAILY_CALENDAR_T* daily_calendar, // [in]
                                       DPARAM           param           // [in]
                                       );

/*****************************************************************************/
//  Description : HandleDailySlideTpUp
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleDailySlideTpUp(
                                GUI_POINT_T       *point_ptr,    
                                MMI_DAILY_CALENDAR_T* daily_calendar,
                                MMISCH_DETAIL_WIN_PARAM_T **detail_win_param
                                );
#endif

#ifdef MMI_CALENDAR_DAILY_SUPPORT
/*****************************************************************************/
//  Description : InitDailyCalendar
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void InitDailyCalendar(    
                                MMI_DAILY_CALENDAR_T* daily_calendar,
                                uint16 year,
                                uint8 mon,
                                uint8 day,
                                MMI_WIN_ID_T win_id
                                );
#endif
#if (defined MMI_CALENDAR_DAILY_SUPPORT)&&(defined TOUCH_PANEL_SUPPORT)
/*****************************************************************************/
//  Description : InitWeeklyButtonChtrl
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void InitDailyButtonChtrl(MMI_WIN_ID_T win_id);
#endif

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : Init Weekly Button
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void InitWeeklyButtonChtrl(MMI_WIN_ID_T win_id);
#endif

#ifdef MMI_CALENDAR_DAILY_SUPPORT
/*****************************************************************************/
//    Description : calendar option window
//    Global resource dependence :
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCalendarDailyOptionWinMsg(
                                              MMI_WIN_ID_T        win_id,
                                              MMI_MESSAGE_ID_E    msg_id,
                                              DPARAM              param
                                              );
#endif
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : GetTpIndex
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL int16 GetTpIndex(
                                GUI_POINT_T       *point_ptr,    
                                MMI_DAILY_CALENDAR_T* daily_calendar
                                );
#endif
#ifdef MMI_CALENDAR_DAILY_SUPPORT
/*****************************************************************************/
//  Description : GetCurHourEvent
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetCurHourEvent(  
                                MMI_DAILY_CALENDAR_T* daily_calendar,
                                uint8 hour,
                                MMIACC_SCH_LIST_T*cur_hour_list
                                );
#endif
#if 0
/*****************************************************************************/
//    Description : DisplayWeekTitle
//    Global resource dependence : 
//    Author: xiangjie
//    Note:
/*****************************************************************************/
LOCAL void DrawWeeklyArrow(
						  MMI_CALENDAR_T* calendar_ctrl_ptr
						  );
#endif

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//    Description :GetFestivalString
//    Global resource dependence : 
//    Author: juan.wu
//    Note: 获得节日字串
/*****************************************************************************/
LOCAL void        GetFestivalString(
                                    MMI_CALENDAR_T * calendar_ctrl_ptr,
                                    MMI_STRING_T * fest_str_ptr
                                    );
#endif

/**--------------------------------------------------------------------------*
 **                          CONSTANT DEFINITION                             *
 **--------------------------------------------------------------------------*/
WINDOW_TABLE(CALENDAR_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleCalendarWinMsg),
    WIN_ID(MMICALENDAR_WIN_ID),
#ifdef MMI_CALENDAR_KEY_STYLE
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
#endif
#ifdef MMI_CALENDAR_WIN_SLIDE
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
#endif
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(CALENDAR_OPTION_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleCalendarOptionWinMsg),
	WIN_ID(MMICALENDAR_OPTION_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
#ifdef MMI_GUI_STYLE_MINISCREEN
    WIN_TITLE(TXT_NULL),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
	END_WIN
};

WINDOW_TABLE(CALENDAR_WEEKLY_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleCalendarWeeklyWinMsg),
    WIN_ID(MMICALENDAR_WEEKLY_WIN_ID),
#ifdef MMI_CALENDAR_KEY_STYLE
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
#endif
//#ifdef MMI_CALENDAR_WIN_SLIDE
    //WIN_MOVE_STYLE(MOVE_FORBIDDEN),
//#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#ifdef TOUCH_PANEL_SUPPORT
    CREATE_BUTTON_CTRL(IMAGE_CLOCK_CALENDAR_LEFT_DEFAULT,MMICALENDAR_WEEK_LEFT_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_CLOCK_CALENDAR_RIGHT_DEFAULT,MMICALENDAR_WEEK_RIGHT_BUTTON_CTRL_ID),
#endif
    END_WIN
};

WINDOW_TABLE(CALENDAR_QUERY_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleCalendarQueryWinMsg),    
    WIN_ID(MMICALENDAR_QUERY_WIN_ID),      
    WIN_TITLE( TXT_CALENDAR_TO_DATE ),//@zhaohui modify for cr88094
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#ifdef MMI_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMICALENDAR_EDIT_DATE_FORM_CTRL_ID),
    CHILD_EDIT_TOUCH_DATE_CTRL(FALSE,MMICALENDAR_QUERY_DATEEDITBOX_CTRL_ID,MMICALENDAR_EDIT_DATE_FORM_CTRL_ID),
    //CREATE_EDIT_TOUCH_DATE_CTRL(MMICALENDAR_QUERY_DATEEDITBOX_CTRL_ID),
#else
    CREATE_EDIT_DATE_CTRL(MMICALENDAR_QUERY_DATEEDITBOX_CTRL_ID),
#endif
    END_WIN
};

#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
//查看阴历详细内容
WINDOW_TABLE(CALENDAR_LUNAR_DETAIL_WIN_TAB ) =
{
    WIN_FUNC((uint32) HandleViewLunarDetailWinMsg ),    
    WIN_ID( MMICALENDAR_LUNAR_DETAIL_WIN_ID ), 
#ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_CALENDAR_LUNAR_DETAIL),
    WIN_BACKGROUND_ID((MMI_IMAGE_ID_T)IMAGE_COMMON_BG),
    CREATE_RICHTEXT_CTRL(MMICALENDAR_LUNAR_DETAIL_TEXT_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

#ifdef MMI_CALENDAR_DAILY_SUPPORT
WINDOW_TABLE(CALENDAR_DAILY_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleCalendarDailyWinMsg),
    WIN_ID(MMICALENDAR_DAILY_WIN_ID),
#ifdef MMI_CALENDAR_KEY_STYLE
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,      //junxue zhu
#endif
#endif
#ifdef MMI_CALENDAR_WIN_SLIDE
    //WIN_MOVE_STYLE(MOVE_FORBIDDEN),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#ifdef TOUCH_PANEL_SUPPORT
    CREATE_BUTTON_CTRL(IMAGE_CLOCK_CALENDAR_LEFT_DEFAULT,MMICALENDAR_LEFT_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_CLOCK_CALENDAR_RIGHT_DEFAULT,MMICALENDAR_RIGHT_BUTTON_CTRL_ID),
#endif
    END_WIN
};

WINDOW_TABLE(CALENDAR_DAILY_OPTION_WIN_TAB) =
{
    WIN_FUNC( (uint32)HandleCalendarDailyOptionWinMsg),
	WIN_ID(MMICALENDAR_DAILY_OPTION_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
#ifdef MMI_GUI_STYLE_MINISCREEN
    WIN_TITLE(TXT_NULL),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
	END_WIN
};
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : open calendar main window
//	Global resource dependence :
//  Author: liqing.peng
//	Note:modified by liyan.zhu //CR144896
/*****************************************************************************/
PUBLIC void MMIAPICALEND_OpenMainWin(void)
{
#ifdef MMI_CALENDAR_WIN_SLIDE 
    MMI_SLIDE_INIT_T slide_init={0};
    MMI_WIN_ID_T  win_slide_id[3]={MMICALENDAR_WIN_ID,
                                    MMICALENDAR_WEEKLY_WIN_ID,
                                    MMICALENDAR_DAILY_WIN_ID};

    MMK_CreateWin((uint32 *)CALENDAR_WIN_TAB,PNULL);
    MMK_CreateWin((uint32 *)CALENDAR_WEEKLY_WIN_TAB, PNULL);
#ifdef MMI_CALENDAR_DAILY_SUPPORT
    MMK_CreateWin((uint32 *)CALENDAR_DAILY_WIN_TAB,PNULL);
#endif
    slide_init.default_win=MMICALENDAR_WIN_ID;
    slide_init.slide_rect_type=SLIDE_RECT_TYPE_CLIENTRECT;
    slide_init.type=MMI_SLIDE_ALL_NO_CHILD_WIN;
    slide_init.win_id_arr_ptr=win_slide_id;
    slide_init.win_id_arr_size=3;
#if defined(MMI_ISTYLE_SUPPORT)	
    if(MMITHEME_IsIstyle())
    {
        slide_init.is_has_symbol=TRUE;
    }
#endif	
    MMISLIDE_Creat(&slide_init, &s_win_slide_handle);
#else
    MMK_CreateWin((uint32 *)CALENDAR_WIN_TAB,PNULL);  
#endif

}

/*****************************************************************************/
// 	Description : open calendar query by date win
//	Global resource dependence :
//  Author: xiangjei
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICALEND_OpenCalendarQueryByDate(ADD_DATA add_data_ptr)
{
#ifdef MMI_PDA_SUPPORT
    MMK_CreatePubFormWin((uint32 *)CALENDAR_QUERY_WIN_TAB, add_data_ptr);
#else
 	MMK_CreateWin((uint32 *)CALENDAR_QUERY_WIN_TAB, add_data_ptr);
#endif
}

#ifdef MMI_GUI_STYLE_MINISCREEN
/*****************************************************************************/
//    Description : RGB888 translate to RGB565
//    Global resource dependence :
//    Author:
//    Note:
/*****************************************************************************/
LOCAL __inline uint16 RGB888TO565(uint32 rgb888)
{
    unsigned int r888 = (rgb888 & 0xff0000) >> 16;
    unsigned int g888 = (rgb888 & 0x00ff00) >> 8;
    unsigned int b888 = (rgb888 & 0x0000ff);
    unsigned int r565 = r888 >> 3;
    unsigned int g565 = g888 >> 2;
    unsigned int b565 = b888 >> 3;
    unsigned int rgb565 = (r565 << (5 + 6)) | (g565 << 5) | b565;

    return rgb565;
}
#endif

/*****************************************************************************/
//    Description : move one month backward
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL void MovePrevMonth(
                         CALE_SOLAR_DATE_T  date_in,
                         CALE_SOLAR_DATE_T  *date_out_ptr
                         )
{
    uint16  year   = date_in.year;
    uint8   month   = date_in.month;
    uint8   date    = date_in.day;
    uint8   monthday = 0;
    
    //SCI_ASSERT(NULL != date_out_ptr);/*assert verified*/
    if(PNULL == date_out_ptr)
    {
        return;
    }
    
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

    date_out_ptr->year = year;
    date_out_ptr->month = month;
    date_out_ptr->day = date;

    return;
}

/*****************************************************************************/
//    Description : move one month forward
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL void MoveNextMonth(
                         CALE_SOLAR_DATE_T  date_in,
                         CALE_SOLAR_DATE_T  *date_out_ptr
                         )
{
    uint16  year   = date_in.year;
    uint8   month   = date_in.month;
    uint8   date    = date_in.day;
    uint8   monthday = 0; 

    //SCI_ASSERT(NULL != date_out_ptr);/*assert verified*/
    if(PNULL == date_out_ptr)
    {
        return;
    }
    
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

    date_out_ptr->year = year;
    date_out_ptr->month = month;
    date_out_ptr->day = date;

    return;
}

/*****************************************************************************/
//    Description : move one day backward
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL void MovePrevDay(
                         CALE_SOLAR_DATE_T  date_in,
                         CALE_SOLAR_DATE_T  *date_out_ptr
                       )
{
    uint16  year     = date_in.year;
    uint8   month    = date_in.month;
    uint8   date     = date_in.day;

    //SCI_ASSERT(NULL != date_out_ptr);/*assert verified*/
    if(PNULL == date_out_ptr)
    {
        return;
    }
    
    if (1 != date)   //不是第一天，移到前一天
    {
	    date--;
    }
    else if (1 != month) //第一天且不是一月，移到上月最后一天
    {
        month--;
        date = MMICALENDAR_GetMonthday(year, month);
    }
    else if (QUERY_YEAR_BOTTOM < year)   //一月一日且年没超出范围，移到上年最后一天
    {
        year--;
        month = DECEMBER;
        date = MMICALENDAR_GetMonthday(year, month);
    }
    else if (QUERY_YEAR_BOTTOM == year)
    {
        year = QUERY_YEAR_TOP;
        month = DECEMBER;
        date = MMICALENDAR_GetMonthday(year, month);
    }

    date_out_ptr->year = year;
    date_out_ptr->month = month;
    date_out_ptr->day = date;

    return;
}

/*****************************************************************************/
//    Description : move one day forward
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL void MoveNextDay(
                         CALE_SOLAR_DATE_T  date_in,
                         CALE_SOLAR_DATE_T  *date_out_ptr
                       )
{
    uint16      year   = date_in.year;
    uint8       month   = date_in.month;
    uint8       date    = date_in.day;

    //SCI_ASSERT(NULL != date_out_ptr);/*assert verified*/
    if(PNULL == date_out_ptr)
    {
        return;
    }
    
    if (MMICALENDAR_GetMonthday(year, month) != date)   //不是最后一天, 移到后一天
    {
	    date++;
    }
    else if (DECEMBER !=month) //最后一天且不是十二月，移到下月第一天
    {
        month++;
        date = 1;
    }
    else if (QUERY_YEAR_TOP > year)   //十二月最后一天且年不超出范围，移到下年第一天
    {
        year++;
        month = 1;
        date = 1;
    }
    else if (QUERY_YEAR_TOP == year)
    {
        year = QUERY_YEAR_BOTTOM;
        month = 1;
        date = 1;
    }

    date_out_ptr->year = year;
    date_out_ptr->month = month;
    date_out_ptr->day = date;

    return;
}

/*****************************************************************************/
//    Description : move one week backward
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL void MovePrevWeek(
                         CALE_SOLAR_DATE_T  date_in,
                         CALE_SOLAR_DATE_T  *date_out_ptr
                         )
{
    uint16      year   = date_in.year;
    uint8       month   = date_in.month;
    uint8       date    = date_in.day;

    //SCI_ASSERT(NULL != date_out_ptr);/*assert verified*/
    if(PNULL == date_out_ptr)
    {
        return;
    }
    
    if (WEEKDAY_NUM < date)   //大于7，前移7天
    {
	    date -= WEEKDAY_NUM;
    }
    else if (1 != month) //小于等于7且不是一月，移到上月
    {
        month--;
        date = MMICALENDAR_GetMonthday(year, month) + date - WEEKDAY_NUM;
    }
    else if (QUERY_YEAR_BOTTOM < year)   //一月且年没超出范围，移到上年
    {
        year--;
        month = DECEMBER;
        date = MMICALENDAR_GetMonthday(year, month) + date - WEEKDAY_NUM;
    }
    else if (QUERY_YEAR_BOTTOM == year)
    {
        year = QUERY_YEAR_TOP;
        month = DECEMBER;
        date = MMICALENDAR_GetMonthday(year, month) + date - WEEKDAY_NUM;
    }

    date_out_ptr->year = year;
    date_out_ptr->month = month;
    date_out_ptr->day = date;

    return;
}

/*****************************************************************************/
//    Description : move one week forward
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL void MoveNextWeek(
                         CALE_SOLAR_DATE_T  date_in,
                         CALE_SOLAR_DATE_T  *date_out_ptr
                           )
{
    uint16      year   = date_in.year;
    uint8       month   = date_in.month;
    uint8       date    = date_in.day;
    uint8       monthday = 0;

    //SCI_ASSERT(NULL != date_out_ptr);/*assert verified*/
    if(PNULL == date_out_ptr)
    {
        return;
    }
    
    monthday = MMICALENDAR_GetMonthday(year, month);
    if (monthday >= (date + WEEKDAY_NUM))   //当前月
    {
	    date += WEEKDAY_NUM;
    }
    else if (DECEMBER != month) //移到下月
    {
        month++;
        date = date + WEEKDAY_NUM - monthday;
    }
    else if (QUERY_YEAR_TOP > year)   //移到下年
    {
        year++;
        month = 1;
        date = date + WEEKDAY_NUM - monthday;
    }
    else if (QUERY_YEAR_TOP == year)
    {
        year = QUERY_YEAR_BOTTOM;
        month = 1;
        date = date + WEEKDAY_NUM - monthday;
    }

    date_out_ptr->year = year;
    date_out_ptr->month = month;
    date_out_ptr->day = date;

    return;
}

/*****************************************************************************/
//    Description : move days
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL void MoveDays(
                    CALE_SOLAR_DATE_T  date_in,
                    uint16             mov_days,
                    BOOLEAN            is_next,
                    CALE_SOLAR_DATE_T  *date_out_ptr
                    )
{
    uint16              i = 0;
    CALE_SOLAR_DATE_T   date = {0};

    //SCI_ASSERT(NULL != date_out_ptr);/*assert verified*/
    if(PNULL == date_out_ptr)
    {
        return;
    }
    
    date = date_in;

    for ( ; i < mov_days; i++ )
    {
        if( is_next )
        {
            MoveNextDay( date, date_out_ptr );
        }
        else
        {
            MovePrevDay( date, date_out_ptr );
        }
        
        date = *date_out_ptr;
    }
}

/*****************************************************************************/
//    Description : compare date
//    Global resource dependence : 
//    Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL int8 CompareDate(
                       CALE_SOLAR_DATE_T  left_date,
                       CALE_SOLAR_DATE_T  right_date
                       )
{
    int8 result = 0;
    
    if ( left_date.year > right_date.year )
    {
        result = 1;
    }
    else if ( left_date.year < right_date.year )
    {
        result = -1;
    }
    else
    {
        if ( left_date.month > right_date.month )
        {
            result = 1;
        }
        else if ( left_date.month < right_date.month )
        {
            result = -1;
        }
        else
        {
            if ( left_date.day > right_date.day )
            {
                result = 1;
            }
            else if ( left_date.day < right_date.day )
            {
                result = -1;
            }
        }
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : 文本风格
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL __inline void SetCalendarTextStyle(
                                         GUISTR_STYLE_T *style_ptr,
                                         GUI_ALIGN_E align,
                                         GUI_FONT_T  font,
                                         GUI_COLOR_T font_color
                                         )
{
    //SCI_ASSERT(PNULL != style_ptr);/*assert verified*/
    if(PNULL == style_ptr)
    {
        return;
    }
    
    style_ptr->align = align;
    style_ptr->font = font;
    style_ptr->font_color = font_color;
    style_ptr->line_space = 0;
}

#ifndef MMI_GUI_STYLE_MINISCREEN
/*****************************************************************************/
//    Description : draw year on title
//    Global resource dependence :
//    Author: xiaoqing.lu
//    Note:
/*****************************************************************************/
LOCAL void DrawYearTitle(
						 MMI_CALENDAR_T* calendar_ctrl_ptr
						 )
{
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    GUI_RECT_T  rect_year = {0};
    GUI_RECT_T  rect_year_v=CALENDAR_YEAR_TITLE_RECT_V;
    GUI_RECT_T  rect_year_h=CALENDAR_YEAR_TITLE_RECT_H;
    MMI_STRING_T title_str = {0};
    uint16      year = 0;
    GUI_COLOR_T year_color = MMI_BLACK_COLOR;//MMITHEME_GetCurThemeFontColor(MMI_THEME_CALENDAR_YEAR);
            
    //SCI_ASSERT(NULL != calendar_ctrl_ptr);/*assert verified*/
    if(PNULL == calendar_ctrl_ptr)
    {
        return;
    }
    

    if( GUI_CALENDAR_REDRAW_PART <= calendar_ctrl_ptr->redraw_mode )
    {

        if(!MMITHEME_IsMainScreenLandscape())
        {
            rect_year = rect_year_v;
        }
        else
        {
            rect_year = rect_year_h;
        }
#if defined(MMI_ISTYLE_SUPPORT)        
        if(MMITHEME_IsIstyle())
        {
            rect_year.top+=MMITHEME_GetWinTitleHeight();
            rect_year.bottom+=MMITHEME_GetWinTitleHeight();
        }
#endif
        year = calendar_ctrl_ptr->solar_year;
      
        title_str = CreateNumString( year );

        SetCalendarTextStyle(&text_style, ALIGN_LVMIDDLE, CALENDAR_YEAR_FONT, year_color);
        // display
        LCD_FillRect(GetCurLcdInfo(),rect_year,MMI_CALENDAR_BG_COLOR);
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)GetCurLcdInfo(),
            (const GUI_RECT_T      *)&rect_year,       //the fixed display area
            (const GUI_RECT_T      *)&rect_year,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&title_str,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            ); 

        MMIAPICOM_DestroyString( title_str );
    }
}
#endif
/*****************************************************************************/
//    Description : draw month on title
//    Global resource dependence :
//    Author: xiaoqing.lu
//    Note:
/*****************************************************************************/
LOCAL void DrawMonthTitle(
						  MMI_CALENDAR_T* calendar_ctrl_ptr
						  )
{
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    GUI_RECT_T  rect_month = {0};
    GUI_RECT_T  rect_month_v=CALENDAR_MONTH_TITLE_RECT_V;
    GUI_RECT_T  rect_month_h=CALENDAR_MONTH_TITLE_RECT_H;
    MMI_STRING_T title_str = {0};
    uint16      month = 0;
#ifdef MMI_GUI_STYLE_MINISCREEN
    uint16      zero = 0;
    uint16      year = 0;
    MMI_STRING_T month_str ={0};
    MMI_STRING_T year_str ={0};
    MMI_STRING_T zero_str ={0};
    wchar minus[2]={0x002d,0};       //表示'-'号 ,junxue zhu
    //wchar space[2]={0x0020,0};       //空格
    wchar         str[150] = {0};
    uint16        str_len   = 0;
#endif

#ifdef MMI_GUI_STYLE_MINISCREEN
    GUI_COLOR_T month_color=RGB888TO565(MMI_CALENDAR_MONTHLY_TITLE_COLOR);
#else
    GUI_COLOR_T month_color = MMI_WHITE_COLOR;//MMITHEME_GetCurThemeFontColor(MMI_THEME_CALENDAR_MONTH);
#endif

    //SCI_ASSERT(NULL != calendar_ctrl_ptr);/*assert verified*/
    if(PNULL == calendar_ctrl_ptr)
    {
        return;
    }
    
    if( GUI_CALENDAR_REDRAW_PART <= calendar_ctrl_ptr->redraw_mode )
    {
        if(!MMITHEME_IsMainScreenLandscape ())
        {
            rect_month = rect_month_v;
        }
        else
        {
            rect_month = rect_month_h;
        }
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
        if(MMITHEME_IsIstyle())
        {
            rect_month.top+=MMITHEME_GetWinTitleHeight();
            rect_month.bottom+=MMITHEME_GetWinTitleHeight();
        }
#endif
        month = calendar_ctrl_ptr->solar_month;

#ifdef MMI_GUI_STYLE_MINISCREEN
        month_str = CreateNumString( month );
#else
        title_str = CreateNumString( month );
#endif
 SetCalendarTextStyle(&text_style, ALIGN_HVMIDDLE, CALENDAR_MONTH_FONT, month_color);
#ifdef MMI_GUI_STYLE_MINISCREEN
        year = calendar_ctrl_ptr->solar_year;
        year_str = CreateNumString( year );
        zero_str = CreateNumString( zero );

     MMI_WSTRNCPY( str + str_len, year_str.wstr_len,year_str.wstr_ptr, year_str.wstr_len,year_str.wstr_len );   //junxue zhu
        str_len += year_str.wstr_len;

    // MMI_WSTRNCPY( str + str_len, 1,space, 1,1);    //junxue zhu
      //  str_len += 1;

     MMI_WSTRNCPY( str + str_len, 1,minus, 1,1);    //junxue zhu
        str_len += 1;

    // MMI_WSTRNCPY( str + str_len, 1,space, 1,1);    //junxue zhu
       // str_len += 1;

     if(month<10)
     {
        MMI_WSTRNCPY( str + str_len, zero_str.wstr_len,zero_str.wstr_ptr, zero_str.wstr_len,zero_str.wstr_len );
        str_len += zero_str.wstr_len;
     }

	 MMI_WSTRNCPY( str + str_len, month_str.wstr_len,month_str.wstr_ptr, month_str.wstr_len,month_str.wstr_len );
        str_len += month_str.wstr_len;

	 MMIAPICOM_DestroyString( year_str );
     MMIAPICOM_DestroyString( zero_str );
     title_str.wstr_len = str_len;
     title_str.wstr_ptr = str;
#endif

        SetCalendarTextStyle(&text_style, ALIGN_HVMIDDLE, CALENDAR_MONTH_FONT, month_color);
        // display
#ifndef MMI_GUI_STYLE_MINISCREEN
        GUIRES_DisplayImg(PNULL,
        &rect_month,               /*lint !e605*/
        PNULL,               /*lint !e605*/
        calendar_ctrl_ptr->win_id,
        IMAGE_CLOCK_CALENDAR_DEFAULT,
        GetCurLcdInfo());
#endif

        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)GetCurLcdInfo(),
            (const GUI_RECT_T      *)&rect_month,       //the fixed display area
            (const GUI_RECT_T      *)&rect_month,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&title_str,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
#ifdef MMI_GUI_STYLE_MINISCREEN
     MMIAPICOM_DestroyString( month_str );
#else
     MMIAPICOM_DestroyString( title_str );
#endif
    }
}

/*****************************************************************************/
//    Description : DisplayWeekTitle
//    Global resource dependence : 
//    Author: xiangjie
//    Note:
/*****************************************************************************/
LOCAL void DrawWeeklyTitle(
						  MMI_CALENDAR_T* calendar_ctrl_ptr
						  )
{
    GUI_ALIGN_E align_type = ALIGN_HVMIDDLE;
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    MMI_STRING_T year_str ={0};
	MMI_STRING_T month_str ={0};
	int8     dir=GUISTR_TEXT_DIR_AUTO;
	MMISET_LANGUAGE_TYPE_E lang_type=MMISET_LANGUAGE_ENGLISH;
#ifdef MMI_GUI_STYLE_MINISCREEN
    GUI_COLOR_T  month_color =RGB888TO565(MMI_CALENDAR_MONTHLY_TITLE_COLOR);//MMITHEME_GetCurThemeFontColor(MMI_THEME_CALENDAR_MONTH);
#else
    GUI_COLOR_T  month_color =MMI_BLACK_COLOR;
#endif
    //SCI_ASSERT(NULL != calendar_ctrl_ptr);/*assert verified*/
    if(PNULL == calendar_ctrl_ptr)
    {
        return;
    }

    if( GUI_CALENDAR_REDRAW_PART <= calendar_ctrl_ptr->redraw_mode )
    {
        BOOLEAN is_need_draw   = TRUE;
        uint8   weeks_of_year  = MMICALENDAR_GetWeekInYear( calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, calendar_ctrl_ptr->solar_date );

        if( GUI_CALENDAR_REDRAW_PART == calendar_ctrl_ptr->redraw_mode
            && weeks_of_year == MMICALENDAR_GetWeekInYear( calendar_ctrl_ptr->pre_year, calendar_ctrl_ptr->pre_month, calendar_ctrl_ptr->pre_date ) )
        {
            is_need_draw = FALSE;
        }

        if( is_need_draw )
        {
            GUI_RECT_T    week_rect = {0};
            MMI_STRING_T  item_str  = {0};
            MMI_STRING_T  num_str   = {0};
            MMI_STRING_T  title_str   = {0};
//             wchar        temp_str[100] = {0};
            wchar         str[200] = {0};
            uint16        str_len   = 0;
            GUI_RECT_T    week_rect_v = CALENDAR_WEEKLY_WEEK_CONTENT_RECT_V;
            GUI_RECT_T    week_rect_h= CALENDAR_WEEKLY_WEEK_CONTENT_RECT_H;
			wchar dot[2]={0x002e,0};
			wchar space[2]={0x0020,0};
			
            if(!MMITHEME_IsMainScreenLandscape())
            {
                week_rect = week_rect_v;
            }
            else
            {
                week_rect = week_rect_h;
            }
#if defined(MMI_ISTYLE_SUPPORT)
            if(MMITHEME_IsIstyle())
            {
                week_rect.top+=MMITHEME_GetWinTitleHeight();
                if(!MMITHEME_IsMainScreenLandscape())
                {
                    week_rect.bottom+=MMITHEME_GetWinTitleHeight()-(CALENDAR_WEEKLY_DATE_Y_V-CALENDAR_ISSTYLE_WEEKLY_DATE_Y_V);
                }
                else
                {
                    week_rect.bottom+=MMITHEME_GetWinTitleHeight()-(CALENDAR_WEEKLY_DATE_Y_H-CALENDAR_ISSTYLE_WEEKLY_DATE_Y_H);
                }
                
            }
#endif
            weeks_of_year = MMICALENDAR_GetWeekInYear(calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, calendar_ctrl_ptr->solar_date);
            //如果是第一周且不是周日开始，就按照上去年上周计算
            /*if(1==weeks_of_year && 0!=MMICALENDAR_GetWeekday(calendar_ctrl_ptr->solar_year,calendar_ctrl_ptr->solar_month, calendar_ctrl_ptr->solar_date))
            {
                weeks_of_year = MMICALENDAR_GetWeekInYear((calendar_ctrl_ptr->solar_year-1),12,31);
            }*/
            year_str = CreateNumString(calendar_ctrl_ptr->solar_year);
			month_str = CreateNumString(calendar_ctrl_ptr->solar_month);
			
			MMI_WSTRNCPY( str + str_len, year_str.wstr_len,year_str.wstr_ptr, year_str.wstr_len,year_str.wstr_len );
            str_len += year_str.wstr_len;

			MMI_WSTRNCPY( str + str_len, 1,dot, 1,1);
            str_len += 1;

			MMI_WSTRNCPY( str + str_len, month_str.wstr_len,month_str.wstr_ptr, month_str.wstr_len,month_str.wstr_len );
            str_len += month_str.wstr_len;

			MMI_WSTRNCPY( str + str_len, 1,space, 1,1);
            str_len += 1;
			/*
            MMI_GetLabelTextByLang( TXT_CALENDAR_WEEK_NO, &item_str);	                
            MMI_WSTRNCPY( str + str_len, item_str.wstr_len,item_str.wstr_ptr, item_str.wstr_len,item_str.wstr_len );
            str_len += item_str.wstr_len;
			*/
            item_str = num_str = CreateNumString( weeks_of_year );      
            MMI_WSTRNCPY( str + str_len, item_str.wstr_len,item_str.wstr_ptr, item_str.wstr_len,item_str.wstr_len );
            str_len += item_str.wstr_len;

            MMIAPICOM_DestroyString( num_str );
			MMIAPICOM_DestroyString( year_str );
			MMIAPICOM_DestroyString( month_str );

            MMI_GetLabelTextByLang( TXT_CALENDAR_WEEK, &item_str );	
            MMI_WSTRNCPY( str + str_len, item_str.wstr_len,item_str.wstr_ptr, item_str.wstr_len,item_str.wstr_len );
            str_len += item_str.wstr_len;

            if(MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
            {
#ifdef MMI_GUI_STYLE_MINISCREEN
                align_type = ALIGN_HVMIDDLE;
#else
                align_type = ALIGN_LEFT;
#endif
            }

            SetCalendarTextStyle(&text_style, align_type, CALENDAR_WEEKLY_TITLE_FONT, month_color);
            // display
            title_str.wstr_len = str_len;
            title_str.wstr_ptr = str;
			MMIAPISET_GetLanguageType(&lang_type);
			if(lang_type==MMISET_LANGUAGE_ARABIC
				||lang_type==MMISET_LANGUAGE_URDU
				||lang_type==MMISET_LANGUAGE_PERSIAN
				||lang_type==MMISET_LANGUAGE_HEBREW)
			{
				dir=GUISTR_TEXT_DIR_RTL;
				state =GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ALIGN_BY_DIR;
			}

            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)GetCurLcdInfo(),
                (const GUI_RECT_T      *)&week_rect,       //the fixed display area
                (const GUI_RECT_T      *)&week_rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&title_str,
                &text_style,
                state,
                dir
                );

        }
    }
}


#if 0
/*****************************************************************************/
//    Description : DisplayWeekTitle
//    Global resource dependence : 
//    Author: xiangjie
//    Note:
/*****************************************************************************/
LOCAL void DrawWeeklyArrow(
						  MMI_CALENDAR_T* calendar_ctrl_ptr
						  )
{
    uint16 img_width=0;
    uint16 img_height=0;
    GUI_POINT_T point={0};
    uint16 lcd_width=0;
    uint16 lcd_height=0;
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    
    if(PNULL == calendar_ctrl_ptr)
    {
        return;
    }
    
    if( GUI_CALENDAR_REDRAW_PART <= calendar_ctrl_ptr->redraw_mode )
    {
        BOOLEAN is_need_draw   = TRUE;
        uint8   weeks_of_year  = MMICALENDAR_GetWeekInYear( calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, calendar_ctrl_ptr->solar_date );
        
        if( GUI_CALENDAR_REDRAW_PART == calendar_ctrl_ptr->redraw_mode 
            && weeks_of_year == MMICALENDAR_GetWeekInYear( calendar_ctrl_ptr->pre_year, calendar_ctrl_ptr->pre_month, calendar_ctrl_ptr->pre_date ) )
        { 
            is_need_draw = FALSE;
        }
        
        if( is_need_draw )
        {
            GUI_RECT_T    week_rect = {0};
            GUI_RECT_T    week_rect_v = CALENDAR_WEEKLY_WEEK_CONTENT_RECT_V;
            GUI_RECT_T    week_rect_h= CALENDAR_WEEKLY_WEEK_CONTENT_RECT_H;
			
            if(!MMITHEME_IsMainScreenLandscape())
            {
                week_rect = week_rect_v;
            }
            else
            {
                week_rect = week_rect_h;
            }

            GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_CLOCK_CALENDAR_LEFT_DEFAULT, calendar_ctrl_ptr->win_id);
            point.x=5;
            if((week_rect.bottom-week_rect.top)>=img_height)
            {
                point.y=week_rect.top+(week_rect.bottom-week_rect.top-img_height)/2;
            }
            else
            {
                point.y=week_rect.top+(img_height-week_rect.bottom+week_rect.top)/2;
            }
            GUIRES_DisplayImg(&point, 
                              PNULL, 
                              PNULL, 
                              calendar_ctrl_ptr->win_id,
                              IMAGE_CLOCK_CALENDAR_LEFT_DEFAULT, 
                              (const GUI_LCD_DEV_INFO *)GetCurLcdInfo());
            point.x=lcd_width-5-img_width;
            GUIRES_DisplayImg(&point, 
                              PNULL, 
                              PNULL, 
                              calendar_ctrl_ptr->win_id,
                              IMAGE_CLOCK_CALENDAR_RIGHT_DEFAULT, 
                              (const GUI_LCD_DEV_INFO *)GetCurLcdInfo());


        }
    }
}
#endif

/*****************************************************************************/
//    Description : draw week on title
//    Global resource dependence : 
//    Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawWeeks(
                     MMI_CALENDAR_T* calendar_ctrl_ptr
                     )
{
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 

    //SCI_ASSERT( NULL != calendar_ctrl_ptr );/*assert verified*/
    if(PNULL == calendar_ctrl_ptr)
    {
        return;
    }
    
    if( GUI_CALENDAR_REDRAW_ALL <= calendar_ctrl_ptr->redraw_mode )
    {
        GUI_RECT_T   rect = {0};
        MMI_STRING_T string = {0};
        uint32       i = 0;
    	//int16 block_height=0;
    	int16 weekly_right_width=0;
        int16 week_y_top=0;
        int16 week_y_bottom=0;
    	if(!MMITHEME_IsMainScreenLandscape())
    	{
    		//block_height=CALENDAR_BLOCK_HEIGHT_V;
    		weekly_right_width=CALENDAR_BLOCK_WIDTH_V;
            week_y_top=CALENDAR_MONTH_WEEKLY_Y_V;
            week_y_bottom=CALENDAR_MONTH_LINE0_TOP_V;
    	}
    	else
    	{
    		//block_height=CALENDAR_BLOCK_HEIGHT_H;
    		weekly_right_width=CALENDAR_BLOCK_WIDTH_H;
            week_y_top=CALENDAR_MONTH_WEEKLY_Y_H;
            week_y_bottom=CALENDAR_MONTH_LINE0_TOP_H;
    	}
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
        if(MMITHEME_IsIstyle())
        {
            week_y_top+=MMITHEME_GetWinTitleHeight();
            week_y_bottom+=MMITHEME_GetWinTitleHeight();
        }
#endif        
        for( ; i < WEEKDAY_NUM; i++ )
        {
            GUI_COLOR_T color    = 0;
            uint32      week_day = ( calendar_ctrl_ptr->first_day_of_week + i ) % WEEKDAY_NUM;

            if ( 0 == week_day )
            {
                color =0xfdc4; //MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);//GUI_RGB2RGB565(76, 128, 1);/*lint !e778 !e572*/
            }
            else if( 6 == week_day )
            {
                color = 0xfdc4; //MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
            }
            else
            {				
                color = MMI_BLACK_COLOR;//MMITHEME_GetCurThemeFontColor(MMI_THEME_CALENDAR_WEEK);//  CALENDAR_WEEK_COLOR; //MMI_GRAY_WHITE_COLOR;
            }
            
            if( GUI_CALENDAR_MONTH_VIEW == calendar_ctrl_ptr->view_type )
            {
                rect = GetDateRect( calendar_ctrl_ptr, 0, i );

                rect.top    = week_y_top;
                rect.bottom = week_y_bottom;
            }
            else
            {
                rect = GetDateRect( calendar_ctrl_ptr, i, 0 );
                rect.left  -= weekly_right_width;
                rect.right -= weekly_right_width;
            }
            
            MMI_GetLabelTextByLang( s_week_text_id[ week_day ], &string );
            SetCalendarTextStyle(&text_style, ALIGN_HVMIDDLE, CALENDAR_DATE_FONT, color);
            // display
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)GetCurLcdInfo(),
                (const GUI_RECT_T      *)&rect,       //the fixed display area
                (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&string,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                ); 

        }
    }
}

/*****************************************************************************/
//    Description : get date rect
//    Global resource dependence : 
//    Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetDateRect(
                             MMI_CALENDAR_T* calendar_ctrl_ptr,
                             uint32               row,
                             uint32               col
                             )
{
    GUI_RECT_T rect = {0};
    int16 istyle_height=0;
    int16 istyle_week_height=0;

    if( GUI_CALENDAR_MONTH_VIEW == calendar_ctrl_ptr->view_type )
    { 
        if(row >= LINENUM || col >= COLNUM)
        {
            return rect;
        }
        
        if(!MMITHEME_IsMainScreenLandscape())
        {
            rect.left   = CALENDAR_MONTH_ITEM0_LEFT_V+col*CALENDAR_BLOCK_WIDTH_V;
            rect.top    = CALENDAR_MONTH_LINE0_TOP_V+row*CALENDAR_BLOCK_HEIGHT_V;

            rect.right  = rect.left + CALENDAR_BLOCK_WIDTH_V - 1;
            rect.bottom = rect.top + CALENDAR_BLOCK_HEIGHT_V  - 1;
        }
        else
        {
#if defined(MMI_ISTYLE_SUPPORT)
            if(MMITHEME_IsIstyle())
            {
                istyle_height=CALENDAR_ISSTYLE_DATE_HEIGHT_OFFSET_H;
            }
            else
#endif
            {
                istyle_height=CALENDAR_BLOCK_HEIGHT_H;
            }
            rect.left   = CALENDAR_MONTH_ITEM0_LEFT_H+col*CALENDAR_BLOCK_WIDTH_H;
            rect.top    = CALENDAR_MONTH_LINE0_TOP_H+row*istyle_height;/*lint !e737*/

            rect.right  = rect.left + CALENDAR_BLOCK_WIDTH_H - 1;
            rect.bottom = rect.top + istyle_height  - 1;
        }
    }
    else
    {
        if(!MMITHEME_IsMainScreenLandscape())
        {
#if defined(MMI_ISTYLE_SUPPORT)        
            if(MMITHEME_IsIstyle())
            {
                istyle_height=CALENDAR_ISSTYLE_WEEKLY_DATE_Y_V;
                istyle_week_height=CALENDAR_ISSTYLE_WEEKLY_SELECT_HEIGHT_V;
            }
            else
#endif				
            {
                istyle_height=CALENDAR_WEEKLY_DATE_Y_V;
                istyle_week_height=CALENDAR_WEEKLY_SELECT_HEIGHT_V;
            }
            rect.left   = CALENDAR_BLOCK_WIDTH_V;
            rect.top    = (uint16)istyle_height + istyle_week_height * row;/*lint !e737*/
            rect.right  = rect.left + CALENDAR_BLOCK_WIDTH_V - 1;
		    rect.bottom = rect.top + istyle_week_height  - 1;
        }
        else
        {
#if defined(MMI_ISTYLE_SUPPORT)        
            if(MMITHEME_IsIstyle())
            {
                istyle_height=CALENDAR_ISSTYLE_WEEKLY_DATE_Y_H;
                istyle_week_height=CALENDAR_ISSTYLE_WEEKLY_SELECT_HEIGHT_H;
            }
            else
#endif				
            {
                istyle_height=CALENDAR_WEEKLY_DATE_Y_H;
                istyle_week_height=CALENDAR_WEEKLY_SELECT_HEIGHT_H;
            }
            rect.left   = CALENDAR_BLOCK_WIDTH_H;
            rect.top    = (uint16)istyle_height + istyle_week_height * row;/*lint !e737*/
            rect.right  = rect.left + CALENDAR_BLOCK_WIDTH_H - 1;
		    rect.bottom = rect.top + istyle_week_height  - 1;
        }
    }
#if defined(MMI_ISTYLE_SUPPORT)
    if(MMITHEME_IsIstyle())
    {
        rect.top+=MMITHEME_GetWinTitleHeight();
        rect.bottom+=MMITHEME_GetWinTitleHeight();
    }
#endif
    return rect;
}

/*****************************************************************************/
//    Description : create num string
//    Global resource dependence : 
//    Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL MMI_STRING_T CreateNumString(
                                   uint16 num
                                   )
{
    wchar*       num_ptr = SCI_ALLOC_APP( (NUM_LENGTH+1)*sizeof(wchar) );
    char         buf[20] = {0};
    
    MMI_STRING_T num_string = {0};
    MMIAPICOM_Int2Str(num,buf,20); /*lint !e64*/
    MMI_STRNTOWSTR(num_ptr,NUM_LENGTH,buf,20,NUM_LENGTH);/*lint !e64*/
    num_string.wstr_ptr = num_ptr;
    num_string.wstr_len = MMIAPICOM_Wstrlen(num_string.wstr_ptr);

    return num_string;
}

/*****************************************************************************/
//    Description : draw one date
//    Global resource dependence : 
//    Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN DrawMonthDate(
                            MMI_CALENDAR_T* calendar_ctrl_ptr,
                            uint16               year,
                            uint8                month,
                            uint8                day,
                            uint8                month_day,
                            GUI_RECT_T*    rect_ptr,
                            BOOLEAN              is_highlight
                            )
{
    BOOLEAN result = FALSE;
//     MMI_IMAGE_ID_T	    bmp_id = IMAGE_NULL;
    SCI_DATE_T          today_t = {0};
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    GUI_LCD_DEV_INFO *lcd_info_ptr=PNULL;

    uint16              img_width = 0;
    uint16              img_height = 0;
    GUI_RECT_T          img_rect = {0};

#ifdef MMI_GUI_STYLE_MINISCREEN
        GUI_RECT_T rect_day = {0};
#endif

    lcd_info_ptr=GetCurLcdInfo();

    //SCI_ASSERT( PNULL != calendar_ctrl_ptr );/*assert verified*/
    //SCI_ASSERT( PNULL != rect_ptr );/*assert verified*/
    if(PNULL == calendar_ctrl_ptr || PNULL == rect_ptr)
    {
        return FALSE;
    }
    
    TM_GetSysDate(&today_t);
    
    if ( day <= month_day )
    {
        GUI_COLOR_T  color = MMITHEME_GetCurThemeFontColor(MMI_THEME_CALENDAR_DAY);// CALENDAR_DAY_COLOR/*0*/;
        MMI_STRING_T num_string = CreateNumString( day );
        uint8        weekday   = MMICALENDAR_GetWeekday( year, month, day );
        color = MMI_BLACK_COLOR;
#ifdef MMI_GUI_STYLE_MINISCREEN
        rect_day.top = rect_ptr->top;
        rect_day.bottom = rect_ptr->bottom;
        rect_day.left = rect_ptr->left+1;
        rect_day.right = rect_ptr->right;
        rect_ptr->top = rect_ptr->top+1;
        rect_ptr->bottom = rect_ptr->bottom-1;
      //  rect_ptr->right = rect_ptr->right; //coverity12573
      //  rect_ptr->left = rect_ptr->left;
#else
//         rect_ptr->right = rect_ptr->right;
//         rect_ptr->left = rect_ptr->left;
#endif
//#else
//       rect_ptr->right = rect_ptr->right;
//       rect_ptr->left = rect_ptr->left +1;
//#endif
        if( weekday ==0)
        {
             color =MMI_BLACK_COLOR; //GUI_RGB2RGB565(76, 128, 1);/*lint !e778 !e572*/
            
        }
        else if (weekday == 6)
        {
           color =MMI_BLACK_COLOR; //GUI_RGB2RGB565(204, 102, 51);
        }
        // 当前日期
        if ( is_highlight )
        {

            color = GUI_RGB2RGB565(255, 255, 255);
            if (day == today_t.mday && today_t.year == year && today_t.mon == month)
            {
                GUIRES_DisplayImg(PNULL,
					rect_ptr,
					PNULL,
					calendar_ctrl_ptr->win_id,
					IMAGE_CLOCK_CALENDAR_DEFAULT,
					lcd_info_ptr);
				
				if (MMIAPISCH_IsOneDateHasSche(year, month, day)) // 今天有任务
				{
                    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_CLOCK_CALENDAR_COMMENT, calendar_ctrl_ptr->win_id);
                    img_rect = GUI_CreateRect(0, 0, img_width-1, img_height-1);
                    GUIRES_DisplayImg(PNULL,
					rect_ptr,
					&img_rect,
					calendar_ctrl_ptr->win_id,
					IMAGE_CLOCK_CALENDAR_COMMENT,
					lcd_info_ptr);
				}
            }
            else
            {
                GUIRES_DisplayImg(PNULL,
					rect_ptr,
					PNULL,
					calendar_ctrl_ptr->win_id,
					IMAGE_CLOCK_CALENDAR_GREEN_DEFAULT,
					lcd_info_ptr);
                
				if (MMIAPISCH_IsOneDateHasSche(year, month, day)) // 其他日有任务
				{
                    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_CLOCK_CALENDAR_COMMENT, calendar_ctrl_ptr->win_id);
                    img_rect = GUI_CreateRect(0, 0, img_width-1, img_height-1);
                    GUIRES_DisplayImg(PNULL,
					rect_ptr,
					&img_rect,
					calendar_ctrl_ptr->win_id,
					IMAGE_CLOCK_CALENDAR_COMMENT,
					lcd_info_ptr);

				}
            }
        }
        else
        {
            if (day == today_t.mday && today_t.year == year && today_t.mon == month)
            {
                color = GUI_RGB2RGB565(255, 255, 255);
                GUIRES_DisplayImg(PNULL,
					rect_ptr,
					PNULL,
					calendar_ctrl_ptr->win_id,
					IMAGE_CLOCK_CALENDAR_FOCUSED,
					lcd_info_ptr);
				if(MMIAPISCH_IsOneDateHasSche(year, month, day))
				{
                    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_CLOCK_CALENDAR_COMMENT, calendar_ctrl_ptr->win_id);
                    img_rect = GUI_CreateRect(0, 0, img_width-1, img_height-1);
                    GUIRES_DisplayImg(PNULL,
					rect_ptr,
					&img_rect,
					calendar_ctrl_ptr->win_id,
					IMAGE_CLOCK_CALENDAR_COMMENT,
					lcd_info_ptr);
				}
            }
            else
            {
                if(MMIAPISCH_IsOneDateHasSche(year, month, day))
				{ 
                    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_CLOCK_CALENDAR_COMMENT, calendar_ctrl_ptr->win_id);
                    img_rect = GUI_CreateRect(0, 0, img_width-1, img_height-1);
                    GUIRES_DisplayImg(PNULL,
					rect_ptr,
					&img_rect,
					calendar_ctrl_ptr->win_id,
					IMAGE_CLOCK_CALENDAR_COMMENT,
					lcd_info_ptr);
				}
            }
        }
        SetCalendarTextStyle(&text_style, ALIGN_HVMIDDLE, CALENDAR_DATE_FONT, color);
        // display

#ifdef MMI_GUI_STYLE_MINISCREEN
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)lcd_info_ptr,
            (const GUI_RECT_T      *)&rect_day,       //the fixed display area
            (const GUI_RECT_T      *)&rect_day,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&num_string,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
#else
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)lcd_info_ptr,
            (const GUI_RECT_T      *)rect_ptr,       //the fixed display area
            (const GUI_RECT_T      *)rect_ptr,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&num_string,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
#endif

        MMIAPICOM_DestroyString( num_string );

        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//    Description : get pos by date
//    Global resource dependence : 
//    Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void GetPosByDate( 
                        MMI_CALENDAR_T*    calendar_ctrl_ptr,
                        uint16                  year,
                        uint8                   month,
                        uint8                   day,
                        uint32*                 row_ptr,
                        uint32*                 col_ptr
                        )
{
    uint32 offset;
    
    //SCI_ASSERT( NULL != calendar_ctrl_ptr );/*assert verified*/
    //SCI_ASSERT( NULL != row_ptr );/*assert verified*/
    //SCI_ASSERT( NULL != col_ptr );/*assert verified*/
    if(PNULL == calendar_ctrl_ptr || PNULL == row_ptr || PNULL == col_ptr)
    {
        return;
    }
    
    offset = ( WEEKDAY_NUM + MMICALENDAR_GetWeekday( year, month, 1 ) - calendar_ctrl_ptr->first_day_of_week ) % WEEKDAY_NUM + day - 1;

    if( GUI_CALENDAR_MONTH_VIEW == calendar_ctrl_ptr->view_type )
    {
        *row_ptr = offset / WEEKDAY_NUM;
        *col_ptr = offset % WEEKDAY_NUM;
    }
    else
    {
        *row_ptr = offset % WEEKDAY_NUM;
        *col_ptr = 0;
    }
}

/*****************************************************************************/
//    Description : draw gray date of a month
//    Global resource dependence : 
//    Author: mary.xiao
//    Note:
/*****************************************************************************/
LOCAL void DrawGrayMonthDate(MMI_CALENDAR_T*     calendar_ctrl_ptr,uint8 day, GUI_RECT_T* rect_ptr)
{

    GUI_LCD_DEV_INFO *lcd_info_ptr=PNULL;
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    GUI_COLOR_T  color = GUI_RGB2RGB565(194, 194, 194 );//MMITHEME_GetCurThemeFontColor(MMI_THEME_RICHTEXT_BORDER);// CALENDAR_DAY_COLOR/*0*/;
    MMI_STRING_T num_string = CreateNumString( day );
    GUI_RECT_T rect_bg={0};

#if defined (MAINLCD_SIZE_128X160) || defined (MAINLCD_SIZE_176X220)
    rect_bg.left=rect_ptr->left+1;
    rect_bg.right=rect_ptr->right-2;
    rect_bg.top=rect_ptr->top+1;
    rect_bg.bottom=rect_ptr->bottom-2;
#else
    rect_bg.left=rect_ptr->left+1;
    rect_bg.right=rect_ptr->right-1;
    rect_bg.top=rect_ptr->top+1;
    rect_bg.bottom=rect_ptr->bottom-1;
#endif

    lcd_info_ptr=GetCurLcdInfo();
    
    DrawBackgroundByRect(calendar_ctrl_ptr, &rect_bg ); 
    
    SetCalendarTextStyle(&text_style, ALIGN_HVMIDDLE, CALENDAR_DATE_FONT, color);
    
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)lcd_info_ptr,
        (const GUI_RECT_T      *)rect_ptr,       //the fixed display area
        (const GUI_RECT_T      *)rect_ptr,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&num_string,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    MMIAPICOM_DestroyString( num_string );
}

/*****************************************************************************/
//    Description : draw date of a month
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL void DrawMonthDates(
                          MMI_CALENDAR_T*    calendar_ctrl_ptr
                          )
{
    //SCI_ASSERT( PNULL != calendar_ctrl_ptr );/*assert verified*/
    uint32 offset1 = 0;
    uint32 offset2 = 0;
    uint32 pre_month_day = 0;
    uint32 next_month_day = 0;
    uint8 day = 0;
    uint16 colum = 0;
    uint16 row_ = 0;
    GUI_RECT_T rect      = {0};
    uint32     row       = 0;
    uint32     col       = 0;
    uint8      month_day = 0;//coverity12688

    if(PNULL == calendar_ctrl_ptr)
    {
        return;
    }
    month_day = MMICALENDAR_GetMonthday( calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month );
    if( GUI_CALENDAR_REDRAW_PART <= calendar_ctrl_ptr->redraw_mode )
    {   
        if( GUI_CALENDAR_REDRAW_PART == calendar_ctrl_ptr->redraw_mode )
        {
            if( calendar_ctrl_ptr->pre_date != calendar_ctrl_ptr->solar_date )
            {
                GetPosByDate( calendar_ctrl_ptr, calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month,
                    calendar_ctrl_ptr->solar_date, &row, &col );
                
                rect = GetDateRect( calendar_ctrl_ptr, row, col );
                DrawBackgroundByRect( calendar_ctrl_ptr, &rect );
                DrawMonthDate( calendar_ctrl_ptr, calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, 
                    calendar_ctrl_ptr->solar_date, month_day, &rect, TRUE );
                
                GetPosByDate( calendar_ctrl_ptr, calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month,
                    calendar_ctrl_ptr->pre_date, &row, &col );
                rect = GetDateRect( calendar_ctrl_ptr, row, col );
                DrawBackgroundByRect( calendar_ctrl_ptr, &rect );
                DrawMonthDate( calendar_ctrl_ptr, calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, 
                    calendar_ctrl_ptr->pre_date, month_day, &rect, FALSE );
            }
        }
        else
        {
            for( day = 1; day <= month_day; day++ )
            {
                GetPosByDate( calendar_ctrl_ptr, calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, day, &row, &col );

                rect = GetDateRect( calendar_ctrl_ptr, row, col );
                
                DrawMonthDate( calendar_ctrl_ptr, calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, 
                    day, month_day, &rect, (BOOLEAN)( day == calendar_ctrl_ptr->solar_date ) );
            }
        }

        //MMICALENDAR_GetMonthday( calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month );
        

        offset1 = ( WEEKDAY_NUM + MMICALENDAR_GetWeekday( calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, 1 ) - calendar_ctrl_ptr->first_day_of_week ) % WEEKDAY_NUM + 1 - 1;
        offset2 = ( WEEKDAY_NUM + MMICALENDAR_GetWeekday( calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, 1 ) - calendar_ctrl_ptr->first_day_of_week ) % WEEKDAY_NUM + month_day - 1;

        pre_month_day = offset1;
        next_month_day = WEEKDAY_NUM -((offset2+1)%WEEKDAY_NUM);

        for( day = 1; day <= pre_month_day; day++ )
        {
            uint8 dis_day = 0;
            row_ = 0;
            colum = day-1;
            rect = GetDateRect( calendar_ctrl_ptr, row_, colum );

            if(calendar_ctrl_ptr->solar_month !=1)
            {
                dis_day = MMICALENDAR_GetMonthday( calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month -1) -pre_month_day + day;
            }
            else
            {
                
                dis_day = MMICALENDAR_GetMonthday( calendar_ctrl_ptr->solar_year-1, 12) - pre_month_day + day;
            }
             
           DrawGrayMonthDate(calendar_ctrl_ptr,dis_day, &rect);
        }

        //if(next_month_day != WEEKDAY_NUM)
        {

            for( day = 1; day <= next_month_day; day++ )
            {
                //if((offset2+1)%WEEKDAY_NUM)
                //{
                    row_ = (offset2+1)/WEEKDAY_NUM;
                //}
                //else
                //{
                 //   row_ = (offset2+1)/WEEKDAY_NUM+1;
                //}
                colum = WEEKDAY_NUM - next_month_day + day-1;
                rect = GetDateRect( calendar_ctrl_ptr, row_, colum);
                DrawGrayMonthDate(calendar_ctrl_ptr,day, &rect);
            }
            if(row_<5)
            {
                row_+=1;
                colum=0;
                for( day = next_month_day+1; day <= next_month_day+WEEKDAY_NUM; day++ )
                {
                    rect = GetDateRect( calendar_ctrl_ptr, row_, colum);
                    DrawGrayMonthDate(calendar_ctrl_ptr,day, &rect);
                    colum++;
                }
            }
        }

#ifndef MMI_GUI_STYLE_MINISCREEN
      DrawMonthDateRectLine(row+1);   //junxue zhu
#endif
    }
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//    Description : 
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL void DrawPageIndication(
                          MMI_WIN_ID_T win_id,
                          int32 cur_index,
                          const GUI_LCD_DEV_INFO *dev_info_ptr
                          )
{
    int i=0;
    uint16 img_w=0;
    uint16 img_h=0;
    MMI_IMAGE_ID_T img_id=IMAGE_CLOCK_CALENDAR_POINT_DEFAULT;
    GUI_POINT_T start_point={0};
    int16 x=0;
    uint16 lcd_width=0;
    uint16 lcd_height=0;
    
    GUIRES_GetImgWidthHeight(&img_w, &img_h,IMAGE_CLOCK_CALENDAR_POINT_FOCUSED,win_id);
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    if(!MMITHEME_IsMainScreenLandscape())
    {
        start_point.y=CALENDAR_POINT_Y_V;
    }
    else
    {
        start_point.y=CALENDAR_POINT_Y_H;
    }
    start_point.x=(lcd_width-5*img_w)/2;
    x=start_point.x;
    for(i=0;i<3;i++)
    {
        start_point.x=x+2*img_w*i;
        if(i==cur_index)
        {
           img_id=IMAGE_CLOCK_CALENDAR_POINT_FOCUSED; 
        }
        else
        {
            img_id=IMAGE_CLOCK_CALENDAR_POINT_DEFAULT;
        }
        GUIRES_DisplayImg(&start_point, 
                              PNULL, 
                              PNULL, 
                              win_id,
                              img_id, 
                              dev_info_ptr);
    }
}
#endif
/*****************************************************************************/
//    Description : 
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL void DrawSchduleText(
                          MMI_CALENDAR_T*    calendar_ctrl_ptr,
                          GUI_RECT_T* rect_str,
                          uint16 year,
                          uint8 month,
                          uint8 day
                          )
{
    BOOLEAN is_list_has_item=FALSE;
    int i=0;
    SCI_TIME_T              cur_time = {0};
    SCI_TIME_T              sch_time = {0};
    MMIACC_SCH_LIST_T*  user_list=PNULL;
    MMIACC_SMART_EVENT_T event={0};
    uint16 cur_index=0;
    BOOLEAN is_find=FALSE;
    uint8  item_str[MMIALM_TIME_STR_12HOURS_LEN + 1] = {0};
    wchar  sch_str[MMIALM_TIME_STR_12HOURS_LEN+MMIACC_EVENT_TEXT_MAX_LEN+5]={0};
    wchar space[2]={0x20,0};
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
#ifdef MMI_GUI_STYLE_MINISCREEN//CR139055
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;
#else
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK;
#endif
    MMI_STRING_T str={0};

    is_list_has_item = MMIAPISCH_OpenEventList(FALSE, year, month, day);

    if(!is_list_has_item)
    {
        calendar_ctrl_ptr->cur_dis_event_id=0;
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CALENDAR_NEW_SCHEDULE, &str);
        SetCalendarTextStyle(&text_style, ALIGN_LVMIDDLE, CALENDAR_SCH_TEXT_FONT, MMI_BLACK_COLOR);
#ifndef TOUCHPANEL_TYPE_NONE
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)GetCurLcdInfo(),
            (const GUI_RECT_T      *)rect_str,       //the fixed display area
            (const GUI_RECT_T      *)rect_str,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&str,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO);
#endif
    }
    else
    {
        TM_GetSysTime(&cur_time);
        user_list=MMIAPISCH_GetUserList();
        for(i=0;i<user_list->total;i++)
        {
            MMIAPIALM_FormInteEventInfo(user_list->entry_id[i],&event);
            sch_time.hour=event.event_fast_info.hour;
            sch_time.min=event.event_fast_info.minute;
            sch_time.sec=cur_time.sec;//不比较秒，设为相等
            if(MMIAPISCH_CompareTime(sch_time, cur_time)==CMP_SECOND_SMALL
                ||MMIAPISCH_CompareTime(sch_time, cur_time)==CMP_EQUAL)
            {
                cur_index=i;
                is_find=TRUE;
                break;
            }
        }
        if(!is_find)
        {
            cur_index=user_list->total-1;
        }
        calendar_ctrl_ptr->cur_dis_event_id=user_list->entry_id[cur_index];
        SCI_MEMSET(&event,0,sizeof(MMIACC_SMART_EVENT_T));
        MMIAPIALM_FormInteEventInfo(user_list->entry_id[cur_index],&event);
        MMIAPISET_FormatTimeStrByTime(event.event_fast_info.hour,event.event_fast_info.minute,item_str,MMIALM_TIME_STR_12HOURS_LEN + 1);
        MMI_STRNTOWSTR( sch_str,
                 MMIALM_TIME_STR_12HOURS_LEN+1,
                item_str,
                strlen((char*)item_str),
                strlen((char*)item_str));
        MMIAPICOM_Wstrcat(sch_str,space);
        MMIAPICOM_Wstrcat(sch_str,space);
        if(event.event_content_info.w_length>0)
        {
            MMIAPICOM_Wstrcat(sch_str,event.event_content_info.w_content);
        }
        else
        {
            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_BLANK, &str);
            MMIAPICOM_Wstrncpy(sch_str+MMIAPICOM_Wstrlen(sch_str), str.wstr_ptr,str.wstr_len);
        }

        str.wstr_ptr=sch_str;
        str.wstr_len=MMIAPICOM_Wstrlen(sch_str);

        SetCalendarTextStyle(&text_style, ALIGN_LVMIDDLE, CALENDAR_SCH_TEXT_FONT, MMI_BLACK_COLOR);
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)GetCurLcdInfo(),
            (const GUI_RECT_T      *)rect_str,       //the fixed display area
            (const GUI_RECT_T      *)rect_str,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&str,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
        );
    }
}
//#ifndef MMI_GUI_STYLE_MINISCREEN
/*****************************************************************************/
//    Description : draw date of a month
//    Global resource dependence :
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL void DrawSchdule(
                          MMI_CALENDAR_T*    calendar_ctrl_ptr
                          )
{
    GUI_RECT_T rect={0};
    GUI_RECT_T rect_str={0};
    uint16 lcd_width=0;
    uint16 lcd_height=0;
    //uint16 img_w=0;
    //uint16 img_h=0;
#ifdef MMI_CALENDAR_KEY_STYLE
    GUI_RECT_T client_rect=MMITHEME_GetClientRect();
#endif
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
//#ifdef MMI_PDA_SUPPORT
    //GUIRES_GetImgWidthHeight(&img_w, &img_h,IMAGE_CLOCK_CALENDAR_POINT_FOCUSED,calendar_ctrl_ptr->win_id);
//#endif
    rect.bottom=lcd_height-1;
    rect.right=lcd_width-1;
    if(!MMITHEME_IsMainScreenLandscape())
    {//显示当天日程的矩形不用offset，小屏时显示半个
        rect.top=CALENDAR_MONTH_LINE0_TOP_V+LINENUM*CALENDAR_BLOCK_HEIGHT_V;
        rect_str.top=rect.top;//+CALENDAR_SCH_TEXT_OFFSET_V;
        rect_str.left=rect.left+CALENDAR_SCH_TEXT_OFFSET_V;
        rect_str.right=rect.right;
        rect_str.bottom=rect.bottom - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;//CR139055
    }
    else
    {//横屏时日历和阴历的显示地方也不是在右边而是在下面，计算rect的方法错误。
		rect.top=CALENDAR_MONTH_LINE0_TOP_H+LINENUM*CALENDAR_BLOCK_HEIGHT_H;
        rect_str.top=rect.top;//+CALENDAR_SCH_TEXT_OFFSET_V;
        rect_str.left=rect.left+CALENDAR_SCH_TEXT_OFFSET_H;
        rect_str.right=rect.right;
        rect_str.bottom=rect.bottom - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT; //CR139055
		/*
        rect.top=CALENDAR_MONTH_LINE0_TOP_H;
        rect.left=CALENDAR_MONTH_ITEM0_LEFT_H+WEEKDAY_NUM*CALENDAR_BLOCK_WIDTH_H;
        rect_str.top=rect.top+CALENDAR_SCH_TEXT_OFFSET_H;
        rect_str.left=rect.left;//+CALENDAR_SCH_TEXT_OFFSET_H;
        rect_str.right=rect.right;
#if defined (MAINLCD_SIZE_128X160) || defined (MAINLCD_SIZE_176X220)
        rect_str.bottom=rect_str.top+GUIFONT_GetHeight(CALENDAR_SCH_TEXT_FONT)+1;
#else
        rect_str.bottom=rect_str.top+GUIFONT_GetHeight(CALENDAR_SCH_TEXT_FONT)*2+1;
#endif*/

    }
#if defined MAINLCD_SIZE_240X320
    if(!MMITHEME_IsMainScreenLandscape())
#endif
    {
#ifdef MMI_CALENDAR_KEY_STYLE
       rect.bottom=client_rect.bottom;
#endif
#if defined(MMI_ISTYLE_SUPPORT)
        if(MMITHEME_IsIstyle())
        {
            rect.top+=MMITHEME_GetWinTitleHeight();
            rect.bottom = MIN((lcd_height - 1), rect.bottom + MMITHEME_GetWinTitleHeight());
        }
#endif		
        GUIRES_DisplayImg(PNULL,
            &rect,               /*lint !e605*/
            PNULL,               /*lint !e605*/
            calendar_ctrl_ptr->win_id,
            IMAGE_CLOCK_CALENDAR_BG,
            GetCurLcdInfo());
#ifndef MMI_CALENDAR_KEY_STYLE
        if(MMITHEME_IsMainScreenLandscape())
        {
//CR139055
            rect.left  = WEEKDAY_NUM*CALENDAR_BLOCK_WIDTH_H;
            rect.right = lcd_width - 1;//CALENDAR_MONTH_ITEM0_LEFT_H+WEEKDAY_NUM*CALENDAR_BLOCK_WIDTH_H;
            rect.top   = CALENDAR_MONTH_WEEKLY_Y_H;//+LINENUM*CALENDAR_BLOCK_HEIGHT_H; 
			rect.bottom= lcd_height - 1;


			rect_str.top   =rect.top + CALENDAR_SCH_TEXT_OFFSET_V;
			rect_str.left  =rect.left + CALENDAR_SCH_TEXT_OFFSET_H;
			rect_str.right =rect.right;
            rect_str.bottom=rect.bottom;
#if defined(MMI_ISTYLE_SUPPORT)			
            if(MMITHEME_IsIstyle())
            {
                rect.top+=MMITHEME_GetWinTitleHeight();
                rect.bottom = MIN((lcd_height - 1), rect.bottom + MMITHEME_GetWinTitleHeight());;
            }
#endif			
            GUIRES_DisplayImg(PNULL,
                &rect,               /*lint !e605*/
                PNULL,               /*lint !e605*/
                calendar_ctrl_ptr->win_id,
                IMAGE_CLOCK_CALENDAR_BG,
                GetCurLcdInfo());
        }
#endif
#if defined(MMI_ISTYLE_SUPPORT)
        if(MMITHEME_IsIstyle())
        {
            rect_str.top+=MMITHEME_GetWinTitleHeight();
            rect.bottom = MIN((lcd_height - 1), rect.bottom + MMITHEME_GetWinTitleHeight());
        }
#endif		
        DrawSchduleText(calendar_ctrl_ptr,
            &rect_str,
            calendar_ctrl_ptr->solar_year,
            calendar_ctrl_ptr->solar_month,
            calendar_ctrl_ptr->solar_date);
    }
#ifndef MMI_CALENDAR_KEY_STYLE
    if(!MMITHEME_IsIstyle())
    {
        DrawPageIndication(calendar_ctrl_ptr->win_id,0,GetCurLcdInfo());
    }
#endif
}
// #endif
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
/*****************************************************************************/
//    Description:  format lunar to string
//    Global resource dependence :
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL void FormatLunarInfo( uint16      iYear,
                            uint16      iMonth,
                            uint16      iDate,
                            wchar       *monthday_ptr,
                            uint16       *monthday_len_ptr,
                            wchar       *year_ptr,
                            uint16       *year_len_ptr)
{
    //"甲乙丙丁戊 辛壬癸"
	const wchar szText1[]={0x7532,0x4E59,0x4E19,0x4E01,0x620A,0x5DF1,0x5E9A,0x8F9B,0x58EC,0x7678,0};
//    char szText1[]="\x75\x32\x4E\x59\x4E\x19\x4E\x01\x62\x0A\x5D\xF1\x5E\x9A\x8F\x9B\x58\xEC\x76\x78";
    //"子丑寅卯辰巳午未申酉戌亥"
    const wchar szText2[]={0x5B50,0x4E11,0x5BC5,0x536F,0x8FB0,0x5DF3,0x5348,0x672A,0x7533,0x9149,0x620C,0x4EA5,0};
//    char szText2[]="\x5B\x50\x4E\x11\x5B\xC5\x53\x6F\x8F\xB0\x5D\xF3\x53\x48\x67\x2A\x75\x33\x91\x49\x62\x0C\x4E\xA5";
    //"一二三四五六七八九十"
    const wchar szText4[]={0x4e00,0x4e8c,0x4e09,0x56db,0x4e94,0x516d,0x4e03,0x516b,0x4e5d,0x5341,0};
//    char szText4[]="\x4e\x00\x4e\x8c\x4e\x09\x56\xdb\x4e\x94\x51\x6d\x4e\x03\x51\x6b\x4e\x5d\x53\x41";
    //"初十廿三"
	const wchar szText5[]={0x521d,0x5341,0x5eff,0x4e09,0};	
//	char szText5[]="\x52\x1d\x53\x41\x5e\xff\x4e\x09";
    //"正"
    const wchar szText6[]={0x6b63,0};
//    char szText6[]="\x6b\x63";
    //年
    const wchar szTextYear[]={0x5E74,0};
//    char szTextYear[]="\x5E\x74";
    //月
	const wchar szTexMonth[]={0x6708,0};
//    char szTexMonth[]="\x67\x08";

    //闰
	const wchar szTexLeap[]={0x95f0,0};

    uint8 add_count=0;

    //年信息
    if(NULL != year_ptr && NULL != year_len_ptr)
    {
        MMI_WSTRNCPY( year_ptr, 1,  szText1+((iYear-4)%10), 1,1 );
        MMI_WSTRNCPY( year_ptr+1, 1, szText2+((iYear-4)%12), 1,1 );
        MMI_WSTRNCPY( year_ptr+2, 1,szTextYear, 1,1 );
        *year_len_ptr = 3;
    }

    //月信息
    if(NULL != monthday_ptr && NULL != monthday_len_ptr)
    {
        if(MMICALENDAR_IsLunarLeapMonth())
        {
            MMI_WSTRNCPY(monthday_ptr,1, szTexLeap, 1,1);
            add_count=1;
        }
        if(10 >= iMonth)
        {
            if(1 == iMonth)
            {
                MMI_WSTRNCPY(monthday_ptr+add_count,1, szText6, 1,1);
            }
            else
            {
                MMI_WSTRNCPY(monthday_ptr+add_count, 1,szText4+(iMonth-1), 1,1);
            }
            MMI_WSTRNCPY(monthday_ptr+add_count+1, 1,szTexMonth, 1,1);
            if(10 >= iDate)
            {
                MMI_WSTRNCPY(monthday_ptr+add_count+2,1, szText5, 1,1);
            }
            else if(20 == iDate)
			{
				MMI_WSTRNCPY(monthday_ptr+add_count+2,1, szText4+1, 1,1);
			}
			else
            {
                MMI_WSTRNCPY(monthday_ptr+add_count+2, 1,szText5+((iDate)/10), 1,1);
            }
            MMI_WSTRNCPY(monthday_ptr+add_count+3, 1,szText4+((iDate-1)%10), 1,1);

            *monthday_len_ptr = 4+add_count;
        }
        else
        {
			MMI_WSTRNCPY(monthday_ptr+add_count,1, szText4+9, 1,1);
            MMI_WSTRNCPY(monthday_ptr+add_count+1, 1,szText4+((iMonth-1)%10), 1,1);
            MMI_WSTRNCPY(monthday_ptr+add_count+2,1, szTexMonth, 1,1);
            if(10 >= iDate)
            {
                MMI_WSTRNCPY(monthday_ptr+add_count+3, 1,szText5, 1,1);
            }
			else if(20 == iDate)
			{
				MMI_WSTRNCPY(monthday_ptr+add_count+3,1, szText4+1, 1,1);
			}
            else
            {
                MMI_WSTRNCPY(monthday_ptr+add_count+3, 1,szText5+((iDate)/10), 1,1);
            }
            MMI_WSTRNCPY(monthday_ptr+add_count+4, 1, szText4+((iDate-1)%10), 1,1);

            *monthday_len_ptr = 5+add_count;
        }
    }

    return;
}
#endif
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
/*****************************************************************************/
//    Description :draw lunar date
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:liyan.zhu 2013.01.11修改：显示WEEK去掉，无用。提取出的阴历显示修改宏。
//    Note:返回值为TRUE时才真正画上了阴历。否则没有
/*****************************************************************************/
LOCAL BOOLEAN DrawLunarDate(
                        MMI_CALENDAR_T*    calendar_ctrl_ptr
                        )
{
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
//#ifndef MMI_GUI_STYLE_MINISCREEN
//     MMI_TEXT_ID_T week_table[] = {
//                                         TXT_SHORT_IDLE_SUNDAY,
//                                         TXT_SHORT_IDLE_MONDAY,
//                                         TXT_SHORT_IDLE_TUESDAY,
//                                         TXT_SHORT_IDLE_WEDNESDAY,
//                                         TXT_SHORT_IDLE_THURSDAY,
//                                         TXT_SHORT_IDLE_FRIDAY,
//                                         TXT_SHORT_IDLE_SATURDAY
//                                     };
// 
//     uint8               wday=0;
//#endif
    uint16              lun_year = 0;
    uint8               lun_month = 0;
    uint8               lun_day = 0;
    wchar               lun_text[CALENDAR_LEAP_MONTH_STR_LEN] = {0};
    uint16              lun_text_len = 0;
    GUI_RECT_T          rect = {0};
    GUI_RECT_T          rect_v = CALENDAR_LUNAR_TITLE_RECT_V;
    GUI_RECT_T          rect_h = CALENDAR_LUNAR_TITLE_RECT_H;
    MMI_STRING_T        str = {0};
    wchar               text[CALENDAR_FESTIVAL_STRING_LEN] = {0};
    MMI_STRING_T        fest_str = {0};
    uint16              copy_len = 0;
#ifdef CMCC_UI_STYLE
    uint8               fest_index = 0;
    wchar               temp_str[20] = {0};
#endif

#ifdef MMI_GUI_STYLE_MINISCREEN
    GUI_COLOR_T  lunar_color =RGB888TO565(MMI_CALENDAR_MONTHLY_TITLE_COLOR);
#endif
    if( GUI_CALENDAR_REDRAW_PART <= calendar_ctrl_ptr->redraw_mode )
    {
		if(!MMITHEME_IsMainScreenLandscape())
		{
			 rect = rect_v;
		}
		else
		{
			 rect = rect_h;
		}
#if defined(MMI_ISTYLE_SUPPORT)		
        if(MMITHEME_IsIstyle())
        {
            rect.top+=MMITHEME_GetWinTitleHeight();
            rect.bottom+=MMITHEME_GetWinTitleHeight();
        }
#endif
        if(calendar_ctrl_ptr->is_need_lunar_date)
        {
            MMICALENDAR_SolarToLunar(calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, calendar_ctrl_ptr->solar_date,
                &lun_year, &lun_month, &lun_day);
            FormatLunarInfo(lun_year, lun_month, lun_day, lun_text, &lun_text_len, NULL, NULL);
            MMI_GetLabelTextByLang(TXT_CALENDAR_LUNAR, &str);
            MMIAPICOM_Wstrncpy(text,str.wstr_ptr,str.wstr_len);
            
            copy_len = MIN(CALENDAR_FESTIVAL_STRING_LEN - str.wstr_len ,lun_text_len);
            MMIAPICOM_Wstrncpy(text+str.wstr_len,lun_text,copy_len);
            //MMIAPICOM_Wstrcat(text, lun_text);
#ifdef CMCC_UI_STYLE
            //优先显示节日，没有节日再显示节气
            fest_str.wstr_ptr = temp_str;
            GetFestivalString(calendar_ctrl_ptr,&fest_str);
            if(0 == fest_str.wstr_len)
            {
            	//节气:
                if(MMICALENDAR_IsLunarFeast(calendar_ctrl_ptr->solar_year, 
                                            calendar_ctrl_ptr->solar_month, 
                                            calendar_ctrl_ptr->solar_date,
                                            &fest_index))
                {
                    wchar * solar_term_str = MMICALENDAR_GetLunarFeastString(fest_index);
                    uint16  solar_term_strlen = 2;//节气字串长度

                    fest_str.wstr_ptr[0] = 0x0020;
                    MMIAPICOM_Wstrncpy(fest_str.wstr_ptr + 1, solar_term_str, solar_term_strlen);
                    fest_str.wstr_len = solar_term_strlen + 1;
                }
            }
            copy_len = MIN(CALENDAR_FESTIVAL_STRING_LEN - str.wstr_len - lun_text_len,
                            fest_str.wstr_len);
            MMIAPICOM_Wstrncpy(text+str.wstr_len + lun_text_len,
                                fest_str.wstr_ptr,
                                copy_len);
#endif

            
            str.wstr_len = lun_text_len+str.wstr_len + fest_str.wstr_len;
            str.wstr_ptr = text;
#ifdef MMI_GUI_STYLE_MINISCREEN
        SetCalendarTextStyle(&text_style, ALIGN_HVMIDDLE, CALENDAR_YEAR_FONT,lunar_color );
#else
        SetCalendarTextStyle(&text_style, ALIGN_LVMIDDLE, CALENDAR_YEAR_FONT,MMI_BLACK_COLOR );
#endif
        LCD_FillRect(GetCurLcdInfo(),rect,MMI_CALENDAR_BG_COLOR);
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)GetCurLcdInfo(),
            (const GUI_RECT_T      *)&rect,       //the fixed display area
            (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&str,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
			return TRUE;
        }
		else
		{
			return FALSE;
		}

//#ifndef MMI_GUI_STYLE_MINISCREEN
//         else
//         {
//             wday=MMICALENDAR_GetWeekday(calendar_ctrl_ptr->solar_year,calendar_ctrl_ptr->solar_month,calendar_ctrl_ptr->solar_date);
//             MMI_GetLabelTextByLang(week_table[wday], &str);
//         }
//#endif

    }
    return FALSE;
}
#endif
/*****************************************************************************/
//    Description : draw calendar main window
//    Global resource dependence : 
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL void DrawMonthCalendar(
                             MMI_CALENDAR_T* calendar_ctrl_ptr,
                             BOOLEAN              is_draw_all
                             )
{
    //SCI_ASSERT(NULL != calendar_ctrl_ptr);/*assert verified*/
#ifdef MMI_GUI_STYLE_MINISCREEN 
	BOOLEAN is_lunar_draw = FALSE;
#endif
    if(NULL == calendar_ctrl_ptr)
    {
        return;
    }
            
    calendar_ctrl_ptr->view_type = GUI_CALENDAR_MONTH_VIEW;
	

    if( is_draw_all )
    {
        SetRedrawMode( calendar_ctrl_ptr, GUI_CALENDAR_REDRAW_ALL );

#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
	    GUIWIN_SetSoftkeyTextId(MMICALENDAR_WIN_ID,  STXT_OPTION, TXT_NULL, STXT_RETURN, TRUE);
	    GUIWIN_SeSoftkeytButtonIconId(MMICALENDAR_WIN_ID, IMAGE_NULL, 1, TRUE);
#else
		GUIWIN_SetSoftkeyTextId(MMICALENDAR_WIN_ID,  STXT_OPTION, TXT_NULL, STXT_RETURN, TRUE);
#endif

    }

    if( GUI_CALENDAR_REDRAW_ALL == calendar_ctrl_ptr->redraw_mode )
    {
        DrawBackground( calendar_ctrl_ptr );
    }

    DrawMonthTitle( calendar_ctrl_ptr );
#ifndef MMI_GUI_STYLE_MINISCREEN
    DrawYearTitle( calendar_ctrl_ptr );
#endif
	//小屏只能显示schedule或者阴历，后者优先级高,
	//物理LCD=160x128时屏幕最短，两者均不显示。
#ifdef MMI_GUI_STYLE_MINISCREEN 
	#ifdef MAINLCD_DEV_SIZE_160X128
	#else
		#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
			is_lunar_draw = DrawLunarDate( calendar_ctrl_ptr );
		#endif
		if (!is_lunar_draw)/*lint !e774*/
		{
			DrawSchdule( calendar_ctrl_ptr);
		}
	#endif
#else
	#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
		DrawLunarDate( calendar_ctrl_ptr );
	#endif 
	DrawSchdule( calendar_ctrl_ptr);
#endif 

    DrawWeeks( calendar_ctrl_ptr );
	DrawMonthDates( calendar_ctrl_ptr );
    
//#endif

    ResetRedrawMode( calendar_ctrl_ptr );
}

/*****************************************************************************/
// 	Description : set redraw mode
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void SetRedrawMode(
                         MMI_CALENDAR_T* calendar_ctrl_ptr,
                         CALENDAR_REDRAW_MODE_E   redraw_mode		//IN:
                         )
{
    calendar_ctrl_ptr->redraw_mode = MAX( calendar_ctrl_ptr->redraw_mode, redraw_mode );
}

/*****************************************************************************/
// 	Description : set redraw mode
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void ResetRedrawMode(
                           MMI_CALENDAR_T* calendar_ctrl_ptr
                           )
{
    calendar_ctrl_ptr->redraw_mode =GUI_CALENDAR_NEED_NOT_REDRAW;
}

/*****************************************************************************/
//  Description : draw background
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawBackground(
                          MMI_CALENDAR_T*     calendar_ctrl_ptr
                          )
{
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    //uint32 statusBar_h=MMITHEME_GetStatusBarHeight();
    GUI_RECT_T client_rect= MMITHEME_GetClientRectEx(calendar_ctrl_ptr->win_id);
    rect.top=client_rect.top;
    rect.bottom=client_rect.bottom;
    DrawBackgroundByRect( calendar_ctrl_ptr, &rect );

    if( GUI_CALENDAR_WEEK_VIEW == calendar_ctrl_ptr->view_type )
    {
        DrawWeekBackgroundLines();
    }
    
}

/*****************************************************************************/
//    Description : DrawWeekBackgroundLines
//    Global resource dependence : 
//    Author: zhaohui
//    Note:
/*****************************************************************************/
LOCAL void DrawWeekBackgroundLines( void )
{
    
    GUI_LCD_DEV_INFO* lcd_dev_info=GetCurLcdInfo();
    uint32 i=0;
    //GUI_COLOR_T color1 = GUI_RGB2RGB565(212, 212, 212);
    GUI_COLOR_T color2 = GUI_RGB2RGB565(212, 212, 212);
    uint16 lcd_width=0;
    uint16 lcd_height=0;
    int16 isstyle_h=0;

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    if(!MMITHEME_IsMainScreenLandscape())
    {
        uint16 top = CALENDAR_WEEKLY_DATE_Y_V;
#if defined(MMI_ISTYLE_SUPPORT)		
        if(MMITHEME_IsIstyle())
        {
            top=MMITHEME_GetWinTitleHeight()+CALENDAR_ISSTYLE_WEEKLY_DATE_Y_V;
            isstyle_h=CALENDAR_ISSTYLE_WEEKLY_SELECT_HEIGHT_V;
        }
        else
#endif			
        {
            isstyle_h=CALENDAR_WEEKLY_SELECT_HEIGHT_V;
        }
#ifdef MMI_GUI_STYLE_MINISCREEN
	    LCD_DrawVLine(lcd_dev_info, CALENDAR_BLOCK_WIDTH_V-2, top, (top + CALENDAR_WEEKLY_SELECT_HEIGHT_V * WEEKDAY_NUM-2), color2);
#else
	    LCD_DrawVLine(lcd_dev_info, CALENDAR_BLOCK_WIDTH_V, top, (top + isstyle_h * WEEKDAY_NUM-2), color2);
#endif
	    LCD_DrawVLine(lcd_dev_info, 2*CALENDAR_BLOCK_WIDTH_V - 1, top, (top + isstyle_h * WEEKDAY_NUM-2), color2); 
	    //LCD_DrawVLine(lcd_dev_info, CALENDAR_BLOCK_WIDTH_V-1, top, (top + CALENDAR_WEEKLY_SELECT_HEIGHT_V * WEEKDAY_NUM-2), color1);
	    //LCD_DrawVLine(lcd_dev_info, 2*CALENDAR_BLOCK_WIDTH_V - 1-1, top, (top + CALENDAR_WEEKLY_SELECT_HEIGHT_V * WEEKDAY_NUM-2), color1); 

        for (i=0;i<WEEKDAY_NUM+1;i++)

	    {   
            //top = CALENDAR_WEEKLY_DATE_Y_V;

            //LCD_DrawHLine(lcd_dev_info, 0, ((uint16)top + CALENDAR_WEEKLY_SELECT_HEIGHT_V * i)-1, lcd_width, color1);
            LCD_DrawHLine(lcd_dev_info, 0, ((uint16)top + isstyle_h * i), lcd_width, color2);/*lint !e737*/

	    }
    }
    else
    {
        uint16 top = CALENDAR_WEEKLY_DATE_Y_H;
#if defined(MMI_ISTYLE_SUPPORT)		
        if(MMITHEME_IsIstyle())
        {
            top=MMITHEME_GetWinTitleHeight()+CALENDAR_ISSTYLE_WEEKLY_DATE_Y_H;
            isstyle_h=CALENDAR_ISSTYLE_WEEKLY_SELECT_HEIGHT_H;
        }
        else
#endif			
        {
            isstyle_h=CALENDAR_WEEKLY_SELECT_HEIGHT_H;
        }
#ifdef MMI_GUI_STYLE_MINISCREEN
        LCD_DrawVLine(lcd_dev_info, CALENDAR_BLOCK_WIDTH_H-2, top, (top + CALENDAR_WEEKLY_SELECT_HEIGHT_H * WEEKDAY_NUM-2), color2);
#else		
        LCD_DrawVLine(lcd_dev_info, CALENDAR_BLOCK_WIDTH_H, top, (top + isstyle_h * WEEKDAY_NUM-2), color2);
#endif
        //LCD_DrawVLine(lcd_dev_info, CALENDAR_BLOCK_WIDTH_H-1, top, (top + CALENDAR_WEEKLY_SELECT_HEIGHT_H * WEEKDAY_NUM-2), color1);
	    LCD_DrawVLine(lcd_dev_info, 2*CALENDAR_BLOCK_WIDTH_H  - 1, top, (top + isstyle_h * WEEKDAY_NUM-2), color2); 
        //LCD_DrawVLine(lcd_dev_info, 2*CALENDAR_BLOCK_WIDTH_H  - 1-1, top, (top + CALENDAR_WEEKLY_SELECT_HEIGHT_H * WEEKDAY_NUM-2), color1);

        for (i=0;i<WEEKDAY_NUM+1;i++)
	    { 
            //top = CALENDAR_WEEKLY_DATE_Y_H;

            //LCD_DrawHLine(lcd_dev_info, 0, ((uint16)top + CALENDAR_WEEKLY_SELECT_HEIGHT_H * i)-1, lcd_width, color1);
            LCD_DrawHLine(lcd_dev_info, 0, ((uint16)top + isstyle_h * i), lcd_width, color2);/*lint !e737*/
	    } 
    }
}

/*****************************************************************************/
//  Description : draw background
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawBackgroundByRect(
                                MMI_CALENDAR_T*     calendar_ctrl_ptr,
                                const GUI_RECT_T*        rect_ptr         
                                )
{
    SCI_ASSERT(NULL != calendar_ctrl_ptr);/*assert verified*/
    LCD_FillRect(GetCurLcdInfo(),*rect_ptr,MMI_CALENDAR_BG_COLOR);
}

#if 0
/*****************************************************************************/
//  Description : draw background
//  Global resource dependence : 
//  Author: maryxiao
//  Note:
/*****************************************************************************/
LOCAL void DrawBackgroundByRectAndImage(
                                MMI_CALENDAR_T*     calendar_ctrl_ptr,
                                const GUI_RECT_T*        rect_ptr,
                                MMI_IMAGE_ID_T           image_id
                                )
{

    //SCI_ASSERT(NULL != calendar_ctrl_ptr);/*assert verified*/
	if(NULL == calendar_ctrl_ptr)
    {
        return;
    }
    GUIRES_DisplayImg(PNULL,
        rect_ptr,               /*lint !e605*/
        rect_ptr,               /*lint !e605*/
        calendar_ctrl_ptr->win_id,
        image_id,
        GetCurLcdInfo());
}
/*****************************************************************************/
//    Description : draw calendar background WeekDateRect
//    Global resource dependence : 
//    Author: zhaohui
//    Note:
/*****************************************************************************/
LOCAL void DrawWeekDateRect(    
                           const GUI_RECT_T* rect_ptr
                           )
{
    GUI_LCD_DEV_INFO* lcd_dev_info=GetCurLcdInfo();
    GUI_COLOR_T color = GUI_RGB2RGB565(212, 212, 212);
    GUI_RECT_T rect = {0};
	int16 top=0;

	if(!MMITHEME_IsMainScreenLandscape())
	{
		top= CALENDAR_WEEKLY_DATE_Y_V;
	}
	else
	{
		top= CALENDAR_WEEKLY_DATE_Y_H;
	}

    rect.left = rect_ptr->left;
    rect.right = rect_ptr->right;
    rect.top = rect_ptr->top;
    rect.bottom = rect_ptr->bottom;

    if (top != rect.top)
    {
        LCD_DrawHLine(lcd_dev_info, rect.left, rect.top, rect.right, color);
    }
    LCD_DrawVLine(lcd_dev_info, rect.left, rect.top, rect.bottom, color);
    LCD_DrawVLine(lcd_dev_info, rect.right, rect.top, rect.bottom, color);
}
#endif
#ifndef MMI_GUI_STYLE_MINISCREEN
LOCAL void DrawMonthDateRectLine(uint16 row_num)
{
    GUI_LCD_DEV_INFO* lcd_dev_info=GetCurLcdInfo();
    GUI_RECT_T rect = MMITHEME_GetClientRect();
    //GUI_COLOR_T color1 = GUI_RGB2RGB565(212, 212, 212);
    GUI_COLOR_T color2 = GUI_RGB2RGB565(212, 212, 212);
	int16 top=0;
    uint16 left = 0;
    uint16 i=0;
    int16 istyle_height=0;

	if(!MMITHEME_IsMainScreenLandscape())
	{
		top= CALENDAR_MONTH_LINE0_TOP_V;
        left = CALENDAR_MONTH_ITEM0_LEFT_V;
#if defined(MMI_ISTYLE_SUPPORT)		
        if(MMITHEME_IsIstyle())
        {
            top+=MMITHEME_GetWinTitleHeight();
        }
#endif	
        for(i=0; i<=6/*row_num*/; i++)
        {
            //LCD_DrawHLine(lcd_dev_info, rect.left, top+i*CALENDAR_BLOCK_HEIGHT_V, rect.right, color1);
            LCD_DrawHLine(lcd_dev_info, rect.left, top+i*CALENDAR_BLOCK_HEIGHT_V-1, rect.right, color2);
        }
        for(i=0; i<=8; i++)
        {
            //LCD_DrawVLine(lcd_dev_info, left+i*CALENDAR_BLOCK_WIDTH_V, top, top+6*CALENDAR_BLOCK_HEIGHT_V, color1);
            LCD_DrawVLine(lcd_dev_info, left+i*CALENDAR_BLOCK_WIDTH_V-1, top, top+6*CALENDAR_BLOCK_HEIGHT_V, color2);
        }
	}
	else
	{
		top= CALENDAR_MONTH_LINE0_TOP_H;
        left = CALENDAR_MONTH_ITEM0_LEFT_H;
#if defined(MMI_ISTYLE_SUPPORT)		
        if(MMITHEME_IsIstyle())
        {
            top+=MMITHEME_GetWinTitleHeight();
            istyle_height=CALENDAR_ISSTYLE_DATE_HEIGHT_OFFSET_H;
        }
        else
#endif			
        {
            istyle_height=CALENDAR_BLOCK_HEIGHT_H;
        }
        for(i=0; i<=6/*row_num*/; i++)
        {
            //LCD_DrawHLine(lcd_dev_info, left, top+i*CALENDAR_BLOCK_HEIGHT_H, left+7*CALENDAR_BLOCK_WIDTH_H, color1);
            LCD_DrawHLine(lcd_dev_info, left, top+i*istyle_height-1, left+7*CALENDAR_BLOCK_WIDTH_H, color2);
        }
        for(i=0; i<=7; i++)//CR139055
        {
            //LCD_DrawVLine(lcd_dev_info, left+i*CALENDAR_BLOCK_WIDTH_H, top, top+6*CALENDAR_BLOCK_HEIGHT_H, color1);
            LCD_DrawVLine(lcd_dev_info, left+i*CALENDAR_BLOCK_WIDTH_H-1, top, top+6*istyle_height, color2);
        }
	}

}
#endif
/*****************************************************************************/
//    Description : 日历控件按键的辅助处理
//    Global resource dependence : 
//    Author: Jibin
//    Note:
/*****************************************************************************/
LOCAL void HandleCalendarKey(
                             MMI_CALENDAR_T* 	calendar_ctrl_ptr,
                             CALENDAR_MOVE_MODE_E 	mode
                             )
{
    CALE_SOLAR_DATE_T   date = {0};

    //SCI_ASSERT(PNULL != calendar_ctrl_ptr);/*assert verified*/
    if(NULL == calendar_ctrl_ptr)
    {
        return;
    }

    calendar_ctrl_ptr->pre_year  = date.year  = calendar_ctrl_ptr->solar_year;
    calendar_ctrl_ptr->pre_month = date.month = calendar_ctrl_ptr->solar_month;
    calendar_ctrl_ptr->pre_date  = date.day = calendar_ctrl_ptr->solar_date;

	switch( mode )
    {
    case  MOVE_PREV_WEEK:     //移到上一行
        MovePrevWeek(date, &date);
        break;

    case  MOVE_NEXT_WEEK:   //移到下一行
        MoveNextWeek(date, &date);
        break;
    
    case  MOVE_PREV_DAY:   //移到上一天
        MovePrevDay(date, &date);
        break;
    
    case  MOVE_NEXT_DAY:  //移到下一天
        MoveNextDay(date, &date);
	    break;

    case MOVE_PREV_MONTH:  //移到上一月
        MovePrevMonth(date, &date);
        break;

    case MOVE_NEXT_MONTH: //移到下一月
        MoveNextMonth(date, &date);
        break;

    case MOVE_PREV_YEAR:  //移到上一年
        if (--(date.year) < QUERY_YEAR_BOTTOM)
    	{
			date.year = QUERY_YEAR_TOP;
    	}
        break;

    case MOVE_NEXT_YEAR: //移到下一年
        if (++(date.year) > QUERY_YEAR_TOP)
		{
			date.year = QUERY_YEAR_BOTTOM;
		}
        break;

    default:
		break;
    }
    if (date.year != QUERY_YEAR_BOTTOM - 1)
    {
        calendar_ctrl_ptr->solar_year = date.year;
        calendar_ctrl_ptr->solar_month = date.month;
        calendar_ctrl_ptr->solar_date = date.day;//直接这样处理肯定不行，闰年2月的最后一天怎么办?
	  //加上闰年2月的特殊处理
	  if(2==calendar_ctrl_ptr->solar_month )//cr186304
	  {
		  if((!GUI_IsLeapYear(date.year))&&calendar_ctrl_ptr->solar_date>=29 )
		  {
	        	calendar_ctrl_ptr->solar_date =28;
		  }
	  }
    }

    if( GUI_CALENDAR_MONTH_VIEW == calendar_ctrl_ptr->view_type )
    {
        if( calendar_ctrl_ptr->pre_year != calendar_ctrl_ptr->solar_year
            || calendar_ctrl_ptr->pre_month != calendar_ctrl_ptr->solar_month )
        {
            SetRedrawMode( calendar_ctrl_ptr, GUI_CALENDAR_REDRAW_ALL );
        }
        else
        {
            SetRedrawMode( calendar_ctrl_ptr, GUI_CALENDAR_REDRAW_PART );
        }
    }
    else
    {
        uint16  i           = 0;
        uint16  move_days   = 0;
        int8    offset      = 0;
        BOOLEAN is_next     = FALSE;
        uint8   pre_weekday = MMICALENDAR_GetWeekday( calendar_ctrl_ptr->pre_year, calendar_ctrl_ptr->pre_month, calendar_ctrl_ptr->pre_date );
        CALE_SOLAR_DATE_T pre_date = {0};
        CALE_SOLAR_DATE_T cur_date = {0};

        pre_date.year  = calendar_ctrl_ptr->pre_year;
        pre_date.month = calendar_ctrl_ptr->pre_month;
        pre_date.day   = calendar_ctrl_ptr->pre_date;

        cur_date.year  = calendar_ctrl_ptr->solar_year;
        cur_date.month = calendar_ctrl_ptr->solar_month;
        cur_date.day   = calendar_ctrl_ptr->solar_date;

        offset = CompareDate(cur_date,pre_date );

        if( 1 == offset )
        {
            is_next = TRUE;

            move_days = ( WEEKDAY_NUM + calendar_ctrl_ptr->first_day_of_week - pre_weekday - 1 ) % WEEKDAY_NUM;
        }
        else if( -1 == offset )
        {
            move_days = ( WEEKDAY_NUM + pre_weekday - calendar_ctrl_ptr->first_day_of_week ) % WEEKDAY_NUM;
        }

        if( 0 != offset )
        {
            BOOLEAN is_redraw = FALSE;

            for( ; i < move_days; i++ )
            {
                if( is_next )
                {
                    MoveNextDay( pre_date, &cur_date );
                }
                else
                {
                    MovePrevDay( pre_date, &cur_date );
                }

                if( cur_date.year == calendar_ctrl_ptr->solar_year
                    && cur_date.month == calendar_ctrl_ptr->solar_month
                    && cur_date.day == calendar_ctrl_ptr->solar_date )
                {
                    is_redraw = TRUE;
                    break;
                }

                pre_date = cur_date;
            }

            if( is_redraw )
            {
                SetRedrawMode( calendar_ctrl_ptr, GUI_CALENDAR_REDRAW_PART );
            }
            else
            {
                SetRedrawMode( calendar_ctrl_ptr, GUI_CALENDAR_REDRAW_ALL );
            }
        }
    }
}


/*****************************************************************************/
//    Description : 日历控件的初始化
//    Global resource dependence : 
//    Author: Jibin
//    Note:
/*****************************************************************************/
LOCAL void* MainCALENDAR_Init(
                              GUI_RECT_T rect //日历的显示区域
                              )
{
    MMI_CALENDAR_T* calendar_ptr = &g_calendar_show;
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
    MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;
    MMIAPISET_GetLanguageType(&lang_type);
#endif
    SCI_MEMSET(calendar_ptr, 0x00, sizeof(MMI_CALENDAR_T));

    calendar_ptr->rect          = rect;     //控件的显示区域
    calendar_ptr->solar_year    = 2000;     //年
    calendar_ptr->solar_month   = 1;        //月
    calendar_ptr->solar_date    = 1;        //日

    //SetDefaultCalendarStyle(calendar_ptr);
    //CalCalendarRect(calendar_ptr);

    calendar_ptr->first_day_of_week = MMIAPISET_GetFirstDayOfWeek();

#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
    if( MMISET_LANGUAGE_SIMP_CHINESE == lang_type
        || MMISET_LANGUAGE_TRAD_CHINESE == lang_type
        //|| MMISET_LANGUAGE_ENGLISH == lang_type
        )
    {
        calendar_ptr->is_need_lunar_date = TRUE;
    }
#endif

    if (s_win_param != NULL)
    {
       SCI_FREE(s_win_param);
       s_win_param = NULL;
    }
    s_win_param = SCI_ALLOC_APP(sizeof(MMISCH_DETAIL_WIN_PARAM_T));
    SCI_MEMSET(s_win_param, 0, sizeof(MMISCH_DETAIL_WIN_PARAM_T));

    if(NULL != s_win_list_param)
    {
        SCI_FREE(s_win_list_param);
    }
    s_win_list_param = SCI_ALLOC_APP(sizeof(MMISCH_LIST_WIN_PARAM_T));
    SCI_MEMSET(s_win_list_param, 0, sizeof(MMISCH_LIST_WIN_PARAM_T));

    return (void*)calendar_ptr;
}

/*****************************************************************************/
//    Description : 日历控件的参数设置
//    Global resource dependence : 
//    Author: Jibin
//    Note:
/*****************************************************************************/
LOCAL void MainCALENDAR_SetParam(     //返回PNULL，表示初始化失败，不等于PNULL，表示初始化成功。
                                  MMI_CALENDAR_T       *calendar_ptr,
                                  uint16 			        solar_year,	    //阳历年		
                                  uint8 			        solar_month,	//阳历月
                                  uint8			            solar_date	    //阳历日
                                  )
{
    calendar_ptr->pre_year  = calendar_ptr->solar_year  = solar_year;       //年
    calendar_ptr->pre_month = calendar_ptr->solar_month = solar_month;      //月
    calendar_ptr->pre_date  = calendar_ptr->solar_date  = solar_date;       //日
}
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//    Description : HandleWeeklyWinTPDown
//    Global resource dependence : 
//    Author: xiangjie
//    Note:
/*****************************************************************************/
LOCAL void HandleCalendarWinTpPress(
								MMI_MESSAGE_ID_E        msg_id,
								MMI_CALENDAR_T* 	calendar_ctrl_ptr,
								GUI_POINT_T             point
								)
{
#ifdef TOUCH_PANEL_SUPPORT   
   // uint32 				i = 0;
    uint8           	firstday_inweek = 0;
    uint8				month_day = 0;
    GUI_RECT_T      	day_rect = {0};
    uint8           	row_count = 0;
    uint8          	 	col_count = 0; 	
    uint8           	total_count = 0;
    uint8           	select_day = 0;	
    uint8           	lastday_inweek = 0;
    static BOOLEAN		s_in_date_rect = FALSE;
	uint16 block_w=0;
	uint16 block_h=0;
	GUI_RECT_T      	day_rect_v = {0};
	GUI_RECT_T      	day_rect_h = {0};

    GUI_RECT_T rect={0};
    GUI_RECT_T rect_sch={0};
    uint16 lcd_width=0;
    uint16 lcd_height=0;
    int16 isistyle_block_h=0;

    if(s_cur_tp_win!=calendar_ctrl_ptr->win_id)
    {
        return ;
    }
    
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    rect.bottom=lcd_height-1;
    rect.right=lcd_width-1;
    
    day_rect_v.left = CALENDAR_MONTH_ITEM0_LEFT_V;
    day_rect_v.top = CALENDAR_MONTH_LINE0_TOP_V;
    day_rect_v.right = CALENDAR_MONTH_ITEM0_LEFT_V+7*CALENDAR_BLOCK_WIDTH_V;
    day_rect_v.bottom = CALENDAR_MONTH_LINE0_TOP_V+6*CALENDAR_BLOCK_HEIGHT_V;
#if defined(MMI_ISTYLE_SUPPORT)
    if(MMITHEME_IsIstyle())
    {
        isistyle_block_h=CALENDAR_ISSTYLE_DATE_HEIGHT_OFFSET_H;
    }
    else
#endif		
    {
        isistyle_block_h=CALENDAR_BLOCK_HEIGHT_H;
    }
    day_rect_h.left = CALENDAR_MONTH_ITEM0_LEFT_H;
    day_rect_h.top = CALENDAR_MONTH_LINE0_TOP_H;
    day_rect_h.right = CALENDAR_MONTH_ITEM0_LEFT_H+7*CALENDAR_BLOCK_WIDTH_H;
    day_rect_h.bottom = CALENDAR_MONTH_LINE0_TOP_H+6*isistyle_block_h;


//入口参数检查
    SCI_ASSERT(calendar_ctrl_ptr != PNULL);/*assert verified*/

    //在收到TP DOWN 前，清空和tp相关的变量
 
    if (MSG_TP_PRESS_DOWN == msg_id)
    {
    	s_in_date_rect = FALSE;
    }

    if(!MMITHEME_IsMainScreenLandscape() )
    {
		day_rect = day_rect_v;
		block_w=CALENDAR_BLOCK_WIDTH_V;
		block_h=CALENDAR_BLOCK_HEIGHT_V;

        rect.top=CALENDAR_MONTH_LINE0_TOP_V+LINENUM*CALENDAR_BLOCK_HEIGHT_V;
        rect_sch.top=rect.top+CALENDAR_SCH_TEXT_OFFSET_V;
        rect_sch.left=rect.left+CALENDAR_SCH_TEXT_OFFSET_V;
        rect_sch.right=rect.right;
        rect_sch.bottom=rect_sch.top+GUIFONT_GetHeight(CALENDAR_SCH_TEXT_FONT)+1;
    }
    else
    { 
        day_rect = day_rect_h;
        block_w=CALENDAR_BLOCK_WIDTH_H;
        //block_h=CALENDAR_BLOCK_HEIGHT_H;
#if defined(MMI_ISTYLE_SUPPORT)        
        if(MMITHEME_IsIstyle())
        {
            block_h=CALENDAR_ISSTYLE_DATE_HEIGHT_OFFSET_H;
        }
        else
#endif			
        {
            block_h=CALENDAR_BLOCK_HEIGHT_H;
        }
        
        rect.top=CALENDAR_MONTH_LINE0_TOP_H;
        rect.left=CALENDAR_MONTH_ITEM0_LEFT_H+WEEKDAY_NUM*CALENDAR_BLOCK_WIDTH_H;
        rect_sch.top=rect.top+CALENDAR_SCH_TEXT_OFFSET_H;
        rect_sch.left=rect.left+CALENDAR_SCH_TEXT_OFFSET_H;
        rect_sch.right=rect.right;
#if defined (MAINLCD_SIZE_128X160) || defined (MAINLCD_SIZE_176X220)
        rect_sch.bottom=rect_sch.top+GUIFONT_GetHeight(CALENDAR_SCH_TEXT_FONT)+1;
#else
        rect_sch.bottom=rect_sch.top+GUIFONT_GetHeight(CALENDAR_SCH_TEXT_FONT)*2+1;
#endif
    }
#if defined(MMI_ISTYLE_SUPPORT)
    if(MMITHEME_IsIstyle())
    {
        day_rect.top+=MMITHEME_GetWinTitleHeight();
        day_rect.bottom+=MMITHEME_GetWinTitleHeight();
        rect_sch.top+=MMITHEME_GetWinTitleHeight();
        rect_sch.bottom+=MMITHEME_GetWinTitleHeight();
    }
#endif        
    //局部变量初始化
    firstday_inweek = MMICALENDAR_GetFirstday( calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month );
    month_day       = MMICALENDAR_GetMonthday( calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month );
    //	day_rect        = calendar_ctrl_ptr->day_rect;

    //判断触笔位置
    if ( GUI_PointIsInRect( point, day_rect )) 
    {  
        //在日历表区域
        col_count  = (uint8)MMIAPICOM_Divide((uint16)(point.x - day_rect.left), block_w );
        row_count = (uint8)MMIAPICOM_Divide((uint16)(point.y - day_rect.top), block_h );
        total_count = row_count * WEEKDAY_NUM + col_count + 1;
        firstday_inweek = ( WEEKDAY_NUM + firstday_inweek - calendar_ctrl_ptr->first_day_of_week ) % WEEKDAY_NUM;

        select_day  = total_count - firstday_inweek;
        if (MSG_TP_PRESS_DOWN == msg_id)
        {
            	s_in_date_rect = TRUE;

            	//选中效果
            	if ( 1 <= select_day && select_day <= month_day ) 
            	{
                    calendar_ctrl_ptr->pre_date   = calendar_ctrl_ptr->solar_date;
                    calendar_ctrl_ptr->solar_date = select_day;
                    SetRedrawMode( calendar_ctrl_ptr, GUI_CALENDAR_REDRAW_PART );
                    DrawMonthCalendar( calendar_ctrl_ptr, FALSE );
             	}
        }
        else//MSG_TP_PRESS_UP，响应触笔事件，效果与按OK键同
        {
            if ( MSG_TP_PRESS_UP == msg_id 
#ifdef MMI_CALENDAR_SLIDE
                && s_slide_calendar.slide_state==MMK_TP_SLIDE_NONE
#endif
                )
            {
                col_count  = (uint8)MMIAPICOM_Divide((uint16)(point.x - day_rect.left), block_w );
                row_count = (uint8)MMIAPICOM_Divide((uint16)(point.y - day_rect.top), block_h );
                total_count = row_count * WEEKDAY_NUM + col_count + 1;
                firstday_inweek = ( WEEKDAY_NUM + firstday_inweek - calendar_ctrl_ptr->first_day_of_week ) % WEEKDAY_NUM;
                lastday_inweek = firstday_inweek + MMICALENDAR_GetMonthday( calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month );

                if(total_count <= firstday_inweek)
                {
                    //MMI_CALENDAR_T  calendar_data = {0};
                   // memcpy(&calendar_data, calendar_ctrl_ptr, sizeof(MMI_CALENDAR_T));
#ifdef MMI_CALENDAR_PLAY_EFFECT
                    playCalendareffect(MOVE_PREV_MONTH,GUI_CALENDAR_MONTH_VIEW);
#endif
                    HandleCalendarKey(calendar_ctrl_ptr, MOVE_PREV_MONTH);
                    DrawMonthCalendar(calendar_ctrl_ptr, TRUE);
                    return;
                }
                else if(total_count > lastday_inweek)
                {
#ifdef MMI_CALENDAR_PLAY_EFFECT
                    playCalendareffect(MOVE_NEXT_MONTH,GUI_CALENDAR_MONTH_VIEW);
#endif
                    HandleCalendarKey(calendar_ctrl_ptr, MOVE_NEXT_MONTH);
                    DrawMonthCalendar(calendar_ctrl_ptr, TRUE);
                    return;
                }
            }
            if (calendar_ctrl_ptr->solar_date == select_day && s_in_date_rect 
#ifdef MMI_CALENDAR_SLIDE
                && s_slide_calendar.slide_state==MMK_TP_SLIDE_NONE
#endif                   
                )
            {
            	 MMK_SendMsg(MMICALENDAR_WIN_ID, MSG_APP_WEB, NULL);
            }
        }
    }
    else if(GUI_PointIsInRect( point, rect_sch )&& MSG_TP_PRESS_UP == msg_id
#ifdef MMI_CALENDAR_SLIDE
            && s_slide_calendar.slide_state==MMK_TP_SLIDE_NONE
#endif
    )
    {
        if(0==calendar_ctrl_ptr->cur_dis_event_id)
        {
            MMK_SendMsg(MMICALENDAR_WIN_ID, MSG_APP_WEB, NULL);
        }
    }

    

    //在收到TP UP 后，清空和tp相关的变量
    if (MSG_TP_PRESS_UP == msg_id)
    {
        s_in_date_rect = FALSE;
    }
#endif
}
#endif
/*****************************************************************************/
//    Description : enter calendar by idle
//    Global resource dependence : 
//    Author: zhaohui
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPICALE_EnterTodyScheduleList(void)
{
	SCI_DATE_T					date = {0};
      MMI_CALENDAR_T     *calendar_ctrl_ptr = &g_calendar_show;	
	GUI_RECT_T              rect = {0,33,128,140};
	
	TM_GetSysDate(&date);

	MainCALENDAR_Init(rect);
	MainCALENDAR_SetParam(calendar_ctrl_ptr, date.year, date.mon, date.mday);
	
       s_win_list_param->year = calendar_ctrl_ptr->solar_year;
       s_win_list_param->month = calendar_ctrl_ptr->solar_month;
       s_win_list_param->day = calendar_ctrl_ptr->solar_date;

	MMIAPISCH_OpenViewListWin((ADD_DATA)s_win_list_param);	
}

/*****************************************************************************/
//    Description : free calendar resouce
//    Global resource dependence : 
//    Author: zhaohui
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPICALENDAR_FreeRes(void)
{
        if (s_win_param != NULL)
        {
            SCI_FREE(s_win_param);
            s_win_param = NULL;
        }
        if(NULL != s_win_list_param)
        {
            SCI_FREE(s_win_list_param);
            s_win_list_param = NULL;
        }
}
/*****************************************************************************/
//    Description : calendar main window
//    Global resource dependence : 
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCalendarWinMsg(
									MMI_WIN_ID_T        win_id, 
									MMI_MESSAGE_ID_E    msg_id, 
									DPARAM              param
									)
{
    MMI_RESULT_E            ret_val = MMI_RESULT_TRUE;
    MMI_CALENDAR_T     *calendar_ctrl_ptr = &g_calendar_show;
    SCI_DATE_T              date = {0};
	GUI_RECT_T              rect = {0,80,128,140};
    BOOLEAN                 *is_weekly = NULL;
    MMISET_DATE_T*  appointed_date_ptr=PNULL;  
    uint16                   index = 0;	
    //GUI_BG_T  bg_info={0};
    //LCD_ANGLE_E  sch_mainwin_angle = LCD_ANGLE_0;
    calendar_ctrl_ptr->win_id = win_id;

	//MMK_GetWinAngle(win_id, &sch_mainwin_angle);
    switch( msg_id )
    {		
	case MSG_OPEN_WINDOW:
	//	MMIAPICALEND_OpenMainWin();
		//MMK_SetWinSupportAngle(win_id,WIN_SUPPORT_ANGLE_0);
		MainCALENDAR_Init(rect);
		if(GetIsAppointedDate())
		{
			SetIsAppointedDate(FALSE);
			appointed_date_ptr=GetAppointedDate();
			MainCALENDAR_SetParam(calendar_ctrl_ptr, 
									appointed_date_ptr->date_year,
									appointed_date_ptr->date_month, 
									appointed_date_ptr->date_day);
		}
		else
		{
			TM_GetSysDate(&date);
			MainCALENDAR_SetParam(calendar_ctrl_ptr, date.year, date.mon, date.mday);
		}
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
		GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, FALSE);
#else
		GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
#endif
#if defined(MMI_ISTYLE_SUPPORT)
        if(MMITHEME_IsIstyle())
        {
            MMI_CONTROL_CREATE_T create={0};
            GUITITLE_INIT_DATA_T   title_data   = {0};
            title_data.text_id=TXT_TOOLS_CALENDAR;
            create.ctrl_id = MMITHEME_GetTitleCtrlId();
            create.guid    = SPRD_GUI_TITLE_ID;
            create.parent_win_handle = win_id;
            create.init_data_ptr = &title_data;
            MMK_CreateControl( &create );
            MMK_SetWinTitleCtrlId( win_id, create.ctrl_id );
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
#endif		
#ifdef MMI_CALENDAR_SLIDE
        if(s_first_lcd_dev_info.block_id==UILAYER_NULL_HANDLE)
        {
            CalendarCreateLayer(&s_first_lcd_dev_info,MMICALENDAR_WIN_ID);

        }
        if(s_second_lcd_dev_info.block_id==UILAYER_NULL_HANDLE)
        {
            CalendarCreateLayer(&s_second_lcd_dev_info,MMICALENDAR_WIN_ID);
        }
        if(s_statusbar_dev_info.block_id==UILAYER_NULL_HANDLE)
        {
            CalendarCreateStatusBarLayer(&s_statusbar_dev_info,MMICALENDAR_WIN_ID);
        }
#ifndef MMI_PDA_SUPPORT
        if(s_softkey_dev_info.block_id==UILAYER_NULL_HANDLE)
        {
            CalendarCreateSoftkeyLayer(&s_softkey_dev_info,MMICALENDAR_WIN_ID);
        }
#endif

        SetCurLcdInfo(&s_calendar_lcd_dev_info);
#endif

#ifdef MMI_CALENDAR_WIN_SLIDE
        MMISLIDE_Active(&s_win_slide_handle);
#endif
        /*bg_info.bg_type=GUI_BG_COLOR;
        bg_info.color=MMI_CALENDAR_BG_COLOR;
        GUISTATUSBAR_SetBgIsDesplay(win_id,TRUE);
        GUISTATUSBAR_SetBg(win_id,bg_info);*/
		break;
		   
		
	case MSG_FULL_PAINT:
		//MMK_SetWinAngle(win_id,sch_mainwin_angle,FALSE);
		DrawMonthCalendar( calendar_ctrl_ptr, TRUE );	
		break;
		
	case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
#ifdef MMI_CALENDAR_WIN_SLIDE
        MMISLIDE_Deactive(&s_win_slide_handle);
        MMISLIDE_Release(&s_win_slide_handle);
#if 0
         MMK_CloseWin(MMICALENDAR_DAILY_WIN_ID);
         MMK_CloseWin(MMICALENDAR_WEEKLY_WIN_ID);
		 MMK_CloseWin(win_id);
#endif
#else
        MMK_CloseWin(win_id);
#endif
		break;       

    case MSG_SCH_EVENT_EDIT_FINISH:
        if (MMIALM_AddOneEvent(&s_win_param->event,&index))
        {
        }
	else
        {
		if(MMIACC_SCH_NUM<=(MMIAPISCH_GetAllSchNumber()))
		{
		    MMIPUB_OpenAlertWarningWin(TXT_SCHEDULE_FULL);
		}
		else
		{
		    MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
		}
        } 
        break;

    case MSG_CLOSE_WINDOW:
#ifdef TOUCH_PANEL_SUPPORT
        s_cur_tp_win=0;
#endif
        if (s_win_param != NULL)
        {
            SCI_FREE(s_win_param);
            s_win_param = NULL;
        }
        if(NULL != s_win_list_param)
        {
            SCI_FREE(s_win_list_param);
			s_win_list_param=PNULL;
        }
#ifdef MMI_CALENDAR_SLIDE
        ExitSlidecalendar(&s_slide_calendar,FALSE);
        if(s_first_lcd_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_first_lcd_dev_info);   /*lint !e506 !e774*/
        }
        if(s_second_lcd_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_second_lcd_dev_info);   /*lint !e506 !e774*/
        }
        if(s_statusbar_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_statusbar_dev_info);   /*lint !e506 !e774*/
        }
#ifndef MMI_PDA_SUPPORT
        if(s_softkey_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_softkey_dev_info);   /*lint !e506 !e774*/
        }
#endif
#endif
#if 0
#ifdef MMI_CALENDAR_WIN_SLIDE
        MMISLIDE_Deactive(&s_win_slide_handle);
        MMISLIDE_Release(&s_win_slide_handle);
#endif
#endif
        break;

    case MSG_APP_WEB:
        date.mday = calendar_ctrl_ptr->solar_date;
        date.year = calendar_ctrl_ptr->solar_year;
        date.mon = calendar_ctrl_ptr->solar_month;
        if (0 == MMIAPISCH_GetInvalidSchByDate(&date))
		{
            s_win_param->win_id = win_id;
            MMIAPISCH_InitNewSch(calendar_ctrl_ptr->solar_year,
                              calendar_ctrl_ptr->solar_month,
                              calendar_ctrl_ptr->solar_date,
                              &s_win_param->event);
            MMIAPISCH_OpenEditWin((ADD_DATA)s_win_param);
        }
        else
        {
            s_win_list_param->year = calendar_ctrl_ptr->solar_year;
            s_win_list_param->month = calendar_ctrl_ptr->solar_month;
            s_win_list_param->day = calendar_ctrl_ptr->solar_date;
            MMIAPISCH_OpenViewListWin((ADD_DATA)s_win_list_param);
        }
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
	case MSG_APP_MENU:
        is_weekly = SCI_ALLOC_APP(sizeof(BOOLEAN));
        *is_weekly = TRUE;
        MMK_CreateWin((uint32 *)CALENDAR_OPTION_WIN_TAB, (ADD_DATA)is_weekly);
        break;
		
    case  MSG_APP_UP:	
        HandleCalendarKey(calendar_ctrl_ptr, MOVE_PREV_WEEK);

#ifdef MMI_GUI_STYLE_MINISCREEN
        DrawMonthCalendar( calendar_ctrl_ptr, TRUE );
#else
        DrawMonthCalendar( calendar_ctrl_ptr, FALSE );
#endif
        break;

    case  MSG_APP_DOWN:

        HandleCalendarKey(calendar_ctrl_ptr, MOVE_NEXT_WEEK);

#ifdef MMI_GUI_STYLE_MINISCREEN
        DrawMonthCalendar( calendar_ctrl_ptr, TRUE );
#else
        DrawMonthCalendar( calendar_ctrl_ptr, FALSE );
#endif
        break;

    case  MSG_APP_LEFT:	
        HandleCalendarKey(calendar_ctrl_ptr, MOVE_PREV_DAY);

#ifdef MMI_GUI_STYLE_MINISCREEN
        DrawMonthCalendar( calendar_ctrl_ptr, TRUE );
#else
        DrawMonthCalendar( calendar_ctrl_ptr, FALSE );
#endif
        break;

    case  MSG_APP_RIGHT:
        HandleCalendarKey(calendar_ctrl_ptr, MOVE_NEXT_DAY);

#ifdef MMI_GUI_STYLE_MINISCREEN
        DrawMonthCalendar( calendar_ctrl_ptr, TRUE );
#else
        DrawMonthCalendar( calendar_ctrl_ptr, FALSE );
#endif
        break;

    case MSG_APP_HASH:
    case MSG_APP_STAR:
        if (MMICOM_RETURN_INCREASE ==MMIAPICOM_IsIncrease(msg_id))
        {
            HandleCalendarKey(calendar_ctrl_ptr, MOVE_NEXT_MONTH);
        }
        else
        {
            HandleCalendarKey(calendar_ctrl_ptr, MOVE_PREV_MONTH);
        }
        DrawMonthCalendar( calendar_ctrl_ptr, TRUE );
        break; 

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T point;
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            s_cur_tp_win=win_id;
            HandleCalendarWinTpPress(msg_id, calendar_ctrl_ptr, point);
#ifdef MMI_CALENDAR_SLIDE
            HandleSlideTpDown(&point,&s_slide_calendar);
#endif
		}		
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef MMI_CALENDAR_SLIDE
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T point;
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            HandleSlideTpMove(&point,&s_slide_calendar,win_id,GUI_CALENDAR_MONTH_VIEW);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TIMER:
        HandleSlideCalendarTimer(&s_slide_calendar, param);
        break;
    case MSG_LCD_SWITCH:
        ExitSlidecalendar(&s_slide_calendar,FALSE);
        if(s_first_lcd_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_first_lcd_dev_info);   /*lint !e506 !e774*/
        }
        if(s_second_lcd_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_second_lcd_dev_info);   /*lint !e506 !e774*/
        }
        if(s_statusbar_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_statusbar_dev_info);   /*lint !e506 !e774*/
        }
#ifndef MMI_PDA_SUPPORT
        if(s_softkey_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_softkey_dev_info);   /*lint !e506 !e774*/
        }
#endif
#if defined(MMI_ISTYLE_SUPPORT)
        if(MMITHEME_IsIstyle())
        {
            MMI_CONTROL_CREATE_T create={0};
            GUITITLE_INIT_DATA_T   title_data   = {0};
            title_data.text_id=TXT_TOOLS_CALENDAR;
            create.ctrl_id = MMITHEME_GetTitleCtrlId();
            create.guid    = SPRD_GUI_TITLE_ID;
            create.parent_win_handle = win_id;
            create.init_data_ptr = &title_data;
            MMK_DestroyControl(MMITHEME_GetTitleCtrlId());
            MMK_CreateControl( &create );
            MMK_SetWinTitleCtrlId( win_id, create.ctrl_id );
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
#endif		
        CalendarCreateLayer(&s_first_lcd_dev_info,MMICALENDAR_WIN_ID);
        CalendarCreateLayer(&s_second_lcd_dev_info,MMICALENDAR_WIN_ID);
        CalendarCreateStatusBarLayer(&s_statusbar_dev_info,MMICALENDAR_WIN_ID);
#ifndef MMI_PDA_SUPPORT
        CalendarCreateSoftkeyLayer(&s_softkey_dev_info,MMICALENDAR_WIN_ID);
#endif
        SetCurLcdInfo(&s_calendar_lcd_dev_info);
        break;
    case MSG_GET_FOCUS:
        if(s_first_lcd_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_first_lcd_dev_info);   /*lint !e506 !e774*/

        }
        if(s_second_lcd_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_second_lcd_dev_info);   /*lint !e506 !e774*/
        }
        if(s_statusbar_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_statusbar_dev_info);   /*lint !e506 !e774*/
        }
#ifndef MMI_PDA_SUPPORT
        if(s_softkey_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_softkey_dev_info);   /*lint !e506 !e774*/
        }
#endif
#if defined(MMI_ISTYLE_SUPPORT)
        if(MMITHEME_IsIstyle())
        {
            MMI_CONTROL_CREATE_T create={0};
            GUITITLE_INIT_DATA_T   title_data   = {0};
            title_data.text_id=TXT_TOOLS_CALENDAR;
            create.ctrl_id = MMITHEME_GetTitleCtrlId();
            create.guid    = SPRD_GUI_TITLE_ID;
            create.parent_win_handle = win_id;
            create.init_data_ptr = &title_data;
            MMK_DestroyControl(MMITHEME_GetTitleCtrlId());
            MMK_CreateControl( &create );
            MMK_SetWinTitleCtrlId( win_id, create.ctrl_id );
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
#endif		
        CalendarCreateLayer(&s_first_lcd_dev_info,MMICALENDAR_WIN_ID);
        CalendarCreateLayer(&s_second_lcd_dev_info,MMICALENDAR_WIN_ID);
        CalendarCreateStatusBarLayer(&s_statusbar_dev_info,MMICALENDAR_WIN_ID);
#ifndef MMI_PDA_SUPPORT
        CalendarCreateSoftkeyLayer(&s_softkey_dev_info,MMICALENDAR_WIN_ID);
#endif
        SetCurLcdInfo(&s_calendar_lcd_dev_info);
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        break;
#endif
    case MSG_LOSE_FOCUS:
#ifdef TOUCH_PANEL_SUPPORT
        s_cur_tp_win=0;
#endif
#ifdef MMI_CALENDAR_SLIDE
        ExitSlidecalendar(&s_slide_calendar,FALSE);
        if(s_first_lcd_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_first_lcd_dev_info);   /*lint !e506 !e774*/
        }
        if(s_second_lcd_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_second_lcd_dev_info);   /*lint !e506 !e774*/
        }
        if(s_statusbar_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_statusbar_dev_info);   /*lint !e506 !e774*/
        }
#ifndef MMI_PDA_SUPPORT
        if(s_softkey_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_softkey_dev_info);   /*lint !e506 !e774*/
        }
#endif
#endif
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_UP:	
		{
            GUI_POINT_T point;
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            HandleCalendarWinTpPress(msg_id, calendar_ctrl_ptr, point);
#ifdef MMI_CALENDAR_SLIDE
            HandleSlideTpUp(&point,&s_slide_calendar,GUI_CALENDAR_MONTH_VIEW);
#endif
		}		
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		
    default:
		ret_val = MMI_RESULT_FALSE;
	    break;
    }

    return ret_val;
}


/*****************************************************************************/
// 	Description : insert dyna menu node by label
//	Global resource dependence : 
//  Author:liqing.peng
//	Note:
/*****************************************************************************/
LOCAL void InsertMenuNodeByLabel(
                        	     MMI_CTRL_ID_T      ctrl_id,        //控件id
                        	     MMI_TEXT_ID_T   	text_id,        //插入的节点的文本
                                 MMI_IMAGE_ID_T     img_id,
                        	     uint16          	node_id,        //节点编号
                        	     uint16          	parent_node_id, //父节点编号
                        	     uint16          	index           //位置
                                 )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};

    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
#ifdef MMI_PDA_SUPPORT
    if(img_id != 0)
    {
        node_item.select_icon_id = img_id;
    }
#endif
    GUIMENU_InsertNode(index,node_id,parent_node_id,&node_item,ctrl_id);
}

/*****************************************************************************/
// 	Description : delete current day all schedule
//	Global resource dependence : 
//  Author: xiangjie
//	Note:   
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPICALENDAR_DelCurDaySch(BOOLEAN is_del_periodsch)
{
    uint16          i                   = 0;
    BOOLEAN         result              = TRUE;
    MMI_CALENDAR_T  *calendar_ctrl_ptr  = &g_calendar_show;
    const MMIACC_SMART_EVENT_FAST_TABLE_T   *event_ptr  = NULL;

    SCI_DATE_T      event_start_date    = {0};
    SCI_DATE_T      event_end_date      = {0};
    SCI_DATE_T      cur_date            = {0};

    cur_date.year = calendar_ctrl_ptr->solar_year;
    cur_date.mon = calendar_ctrl_ptr->solar_month;
    cur_date.mday = calendar_ctrl_ptr->solar_date;

    for (i = 0; i < MMIACC_SCH_NUM; i++)
    {
        event_ptr = MMIALM_GeEvent((uint16)(EVENT_SCH_0 + i));

        event_start_date.year = event_ptr->start_year;
        event_start_date.mon  = event_ptr->start_mon;
        event_start_date.mday = event_ptr->start_day;
        event_end_date.year = event_ptr->end_year;
        event_end_date.mon  = event_ptr->end_mon;
        event_end_date.mday = event_ptr->end_day;

        switch (event_ptr->fre_mode)
        {
            case ALM_MODE_ONCE:
            case ALM_MODE_NO:
                if( cur_date.year == event_ptr->year &&
                    cur_date.mon == event_ptr->mon &&
                    cur_date.mday == event_ptr->day
                    )
                {
                    if (!MMIALM_DelOneEvent((uint16)(EVENT_SCH_0 + i)))
                    {
                        result = FALSE;
                    }
                }
                break;

            case ALM_MODE_EVERYDAY:
            case ALM_MODE_EVERYWEEK:
            case ALM_MODE_EVERYMONTH:
            case ALM_MODE_EVERYYEAR:
                if (
                    (!is_del_periodsch)
                    && event_start_date.year==event_end_date.year
                    && event_start_date.mon==event_end_date.mon
                    && event_start_date.mday==event_end_date.mday
                    )
                {
                    if (!MMIALM_DelOneEvent((uint16)(EVENT_SCH_0 + i)))
                    {
                        result = FALSE;
                    }
                }
                else if (
                (is_del_periodsch)&&MMIAPISCH_IsInDateRange(event_start_date,event_end_date,cur_date)
                )
                {
                    if(ALM_MODE_EVERYDAY==event_ptr->fre_mode)
                    {
                        if (!MMIALM_DelOneEvent((uint16)(EVENT_SCH_0 + i)))
                        {
                            result = FALSE;
                        }
                    }
                    else if(ALM_MODE_EVERYWEEK==event_ptr->fre_mode)
                    {
                        if((event_ptr->fre & s_week_mask[MMICALENDAR_GetWeekday(cur_date.year,cur_date.mon,cur_date.mday)]) != 0)
                        {
                            if (!MMIALM_DelOneEvent((uint16)(EVENT_SCH_0 + i)))
                            {
                                result = FALSE;
                            }
                        }
                    }
                else if(ALM_MODE_EVERYMONTH==event_ptr->fre_mode)
                {
                    if(cur_date.mday == event_ptr->day)
                    {
                        if (!MMIALM_DelOneEvent((uint16)(EVENT_SCH_0 + i)))
                        {
                            result = FALSE;
                        }
                    }
                }
                else if(ALM_MODE_EVERYYEAR==event_ptr->fre_mode)
                {
                    if(cur_date.mday == event_ptr->day
                    && cur_date.mon== event_ptr->mon)
                    {
                        if (!MMIALM_DelOneEvent((uint16)(EVENT_SCH_0 + i)))
                        {
                            result = FALSE;
                        }
                    }
                }
            }
                break;

            default:
                break;
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description : delete current day of one schedule
//	Global resource dependence : 
//  Author: xiangjie
//	Note:   
/*****************************************************************************/
PUBLIC  BOOLEAN MMICALENDAR_DelCurDayFirstValidSch(void)
{
    uint16          i                   = 0;
    BOOLEAN         result              = FALSE;
    MMI_CALENDAR_T  *calendar_ctrl_ptr  = &g_calendar_show;
    const MMIACC_SMART_EVENT_FAST_TABLE_T   *event_ptr  = NULL;

    SCI_DATE_T      event_start_date    = {0};
    SCI_DATE_T      event_end_date      = {0};
    SCI_DATE_T      cur_date            = {0};
    MmiSignalS      *signal             = PNULL;
    cur_date.year = calendar_ctrl_ptr->solar_year;
    cur_date.mon = calendar_ctrl_ptr->solar_month;
    cur_date.mday = calendar_ctrl_ptr->solar_date;

    if(MMIALM_GetAlmInterruptFlag())
    {
        //delete be interrupted, need stop delete
        return result;
    } 

    for (i = 0; i < MMIACC_SCH_NUM; i++)
    {
        event_ptr = MMIALM_GeEvent((uint16)(EVENT_SCH_0 + i));

        event_start_date.year = event_ptr->start_year;
        event_start_date.mon  = event_ptr->start_mon;
        event_start_date.mday = event_ptr->start_day;
        event_end_date.year = event_ptr->end_year;
        event_end_date.mon  = event_ptr->end_mon;
        event_end_date.mday = event_ptr->end_day;

        switch (event_ptr->fre_mode)
        {
            case ALM_MODE_ONCE:
            case ALM_MODE_NO:
                if(   	cur_date.year == event_ptr->year &&
                cur_date.mon == event_ptr->mon &&
                cur_date.mday == event_ptr->day
                )
                {
                if (MMIALM_DelOneEvent((uint16)(EVENT_SCH_0 + i)))
                {
                //send delete cnf
                //creat signal
                MMI_CREATE_SIGNAL(signal,(uint16)MSG_SET_DEL_ALARMSET_CNF,sizeof(MmiSignalS),P_APP);
                //send signal
                MMI_SEND_SIGNAL(signal, P_APP);
                result = TRUE;
                }
                return result;
                }
                break;

            case ALM_MODE_EVERYDAY:
            case ALM_MODE_EVERYWEEK:
            case ALM_MODE_EVERYMONTH:
            case ALM_MODE_EVERYYEAR:
                if (
                MMIAPISCH_IsInDateRange(event_start_date,event_end_date,cur_date)
                )
                {
                    if (MMIALM_DelOneEvent((uint16)(EVENT_SCH_0 + i)))
                    {
                        //creat signal
                        MMI_CREATE_SIGNAL(signal,(uint16)MSG_SET_DEL_ALARMSET_CNF,sizeof(MmiSignalS),P_APP);
                        //send signal
                        MMI_SEND_SIGNAL(signal, P_APP);
                        result = TRUE;
                    }
                    return result;
                }
                break;

            default:
                break;
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description : check exist valid sch in current day
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
PUBLIC  BOOLEAN MMIALM_IsExistValidEventInCurDay(void)
{
	uint16                          i                  = 0;
	const MMIACC_SMART_EVENT_FAST_TABLE_T      *event_ptr         = NULL;
	BOOLEAN							result             = FALSE;
	MMI_CALENDAR_T             *calendar_ctrl_ptr = &g_calendar_show;
	
	SCI_DATE_T		event_start_date                         = {0};
	SCI_DATE_T		event_end_date                  = {0};
	SCI_DATE_T          	cur_date                             = {0};
	
	cur_date.year = calendar_ctrl_ptr->solar_year;
	cur_date.mon = calendar_ctrl_ptr->solar_month;
	cur_date.mday = calendar_ctrl_ptr->solar_date;    
	
	for (i = 0; i < MMIACC_SCH_NUM; i++)
	{
		event_ptr = MMIALM_GeEvent((uint16)(EVENT_SCH_0 + i));
		
		event_start_date.year = event_ptr->start_year;
		event_start_date.mon  = event_ptr->start_mon;
		event_start_date.mday = event_ptr->start_day;
		event_end_date.year = event_ptr->end_year;
		event_end_date.mon  = event_ptr->end_mon;
		event_end_date.mday = event_ptr->end_day;	
		
		switch (event_ptr->fre_mode)
		{
		case ALM_MODE_ONCE:
        case ALM_MODE_NO:
			if( cur_date.year == event_ptr->year &&
				cur_date.mon == event_ptr->mon &&
				cur_date.mday == event_ptr->day
				)
			{
				return TRUE;
			}
			break;
			
		case ALM_MODE_EVERYDAY:
		case ALM_MODE_EVERYWEEK:
		case ALM_MODE_EVERYMONTH:
		case ALM_MODE_EVERYYEAR:
			if (
				MMIAPISCH_IsInDateRange(event_start_date,event_end_date,cur_date)
				)
			{
				return TRUE;
			}
			break;
			
		default:
			break;
		}
	}
    return result;	
}

/*****************************************************************************/
//    Description : calendar option window
//    Global resource dependence : 
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCalendarOptionWinMsg(
                                              MMI_WIN_ID_T        win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              )
{
    uint16                              i                   = 0;
    uint16                              node_id             = 0;
    BOOLEAN                             *is_weekly_ptr      = NULL;
    MMI_RESULT_E                        ret_val             = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                       ctrl_id             = MMICALENDAR_OPT_MENU_CTRL_ID;
    MMI_CALENDAR_T                 *calendar_ctrl_ptr  = &g_calendar_show;
    SCI_DATE_T                          cur_date            = {0};
    SCI_DATE_T		                    date                = {0};
#ifdef MMI_GUI_STYLE_MINISCREEN
    MMI_STRING_T                    str_menutitle  ={0};
#endif
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
	MMISET_LANGUAGE_TYPE_E lang_type=MMISET_LANGUAGE_ENGLISH;
#endif
	static MMISET_DATE_T   *detail_win_param = PNULL;
	static uint32 *to_date_param=PNULL;
	uint16            index = 0;

    is_weekly_ptr = (BOOLEAN *) MMK_GetWinAddDataPtr(win_id);

    if(NULL == is_weekly_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    calendar_ctrl_ptr->win_id = win_id;
	
    switch( msg_id )
    {		
	case MSG_OPEN_WINDOW:
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
        MMIAPISET_GetLanguageType(&lang_type);
#endif
        GUIMENU_CreatDynamic(PNULL,win_id,ctrl_id,GUIMENU_STYLE_POPUP);
#ifdef MMI_GUI_STYLE_MINISCREEN
        MMI_GetLabelTextByLang(TXT_IDLE_CALENDAR,&str_menutitle);
        GUIMENU_SetMenuTitle(&str_menutitle,ctrl_id);
#endif
        i=0;
#ifdef MMI_PDA_SUPPORT
        InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_NEW, IMG_CALENDAR_MENU_ADD_ICON,CALENDAR_OPT_NEW, 0, i++);
		InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_LOOK, IMG_CALENDAR_MENU_VIEW_ICON,CALENDAR_OPT_LOOK, 0, i++);
		InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_ALL, IMG_CALENDAR_MENU_VIEW_ICON,CALENDAR_OPT_ALL, 0, i++);
        InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_CLEAR_TODAY, IMG_CALENDAR_MENU_CLEAN_TODAY_ICON,CALENDAR_OPT_CLEAR_TODAY, 0, i++);
		InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_CLEAR_ALL, IMAGE_COMMON_OPTMENU_REMOVE_ALL,CALENDAR_OPT_CLEAR_ALL, 0, i++);
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
		if(MMISET_LANGUAGE_SIMP_CHINESE==lang_type ||MMISET_LANGUAGE_TRAD_CHINESE==lang_type)
		{
			InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_LUNAR_DETAIL, IMG_CALENDAR_MENU_LURA_DETAIL_ICON,CALENDAR_OPT_LUNAR_DETAIL, 0, i++);
		}
#endif
		InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_TO_DATE, IMG_CALENDAR_MENU_CLEAN_GOTO_DATE_ICON,CALENDAR_OPT_QUERY, 0, i++);
#else
		InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_NEW, 0,CALENDAR_OPT_NEW, 0, i++);
		InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_LOOK, 0,CALENDAR_OPT_LOOK, 0, i++);
		InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_ALL,0, CALENDAR_OPT_ALL, 0, i++);
        InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_CLEAR_TODAY, 0,CALENDAR_OPT_CLEAR_TODAY, 0, i++);
		InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_CLEAR_ALL, 0,CALENDAR_OPT_CLEAR_ALL, 0, i++);
		
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
		if(MMISET_LANGUAGE_SIMP_CHINESE==lang_type ||MMISET_LANGUAGE_TRAD_CHINESE==lang_type)
		{
			InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_LUNAR_DETAIL, 0,CALENDAR_OPT_LUNAR_DETAIL, 0, i++);
		}
#endif
		InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_TO_DATE, 0,CALENDAR_OPT_QUERY, 0, i++);
#endif

#ifdef MMI_CALENDAR_KEY_STYLE
        if(*is_weekly_ptr)
        {
            InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_WEEKLY, 0, CALENDAR_OPT_WEEK, 0, i++);
#ifdef MMI_CALENDAR_DAILY_SUPPORT
            InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_DAILY, 0, CALENDAR_OPT_DAY, 0, i++);
#endif
        }
        else
        {
            InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_MONTHLY, 0, CALENDAR_OPT_MONTH, 0, i++);
#ifdef MMI_CALENDAR_DAILY_SUPPORT
            InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_DAILY, 0, CALENDAR_OPT_DAY, 0, i++);
#endif
        }
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);
		break;

    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
	case MSG_APP_WEB:
	case MSG_CTL_MIDSK:
        node_id = GUIMENU_GetCurNodeId(ctrl_id);
        switch( node_id )
        {
        case CALENDAR_OPT_LOOK:
            s_win_list_param->year = calendar_ctrl_ptr->solar_year;
            s_win_list_param->month = calendar_ctrl_ptr->solar_month;
            s_win_list_param->day = calendar_ctrl_ptr->solar_date;
            MMIAPISCH_OpenViewListWin((ADD_DATA)s_win_list_param);
            break;

        case CALENDAR_OPT_NEW:
            SCI_MEMSET(s_win_param,0,sizeof(MMISCH_DETAIL_WIN_PARAM_T));
            s_win_param->win_id = MMICALENDAR_WIN_ID;
            MMIAPISCH_InitNewSch(calendar_ctrl_ptr->solar_year,
                              calendar_ctrl_ptr->solar_month,
                              calendar_ctrl_ptr->solar_date,
                              &s_win_param->event);
            MMIAPISCH_OpenEditWin((ADD_DATA)s_win_param);
            MMK_CloseWin(win_id);
            break;

        case CALENDAR_OPT_ALL:
            TM_GetSysDate(&cur_date);
            s_win_list_param->year = cur_date.year;
            s_win_list_param->month = cur_date.mon;
            s_win_list_param->day = cur_date.mday;
            MMIAPISCH_OpenViewListAllWin(s_win_list_param);
            break;

        case CALENDAR_OPT_CLEAR_TODAY:
            MMIAPICALENDAR_SetCurDayHasPeriodSch(MMIAPICALENDAR_IsCurDayHasAppointSch(CALENDAR_OPT_SCH_PERIOD));
            if ( MMIAPICALENDAR_GetCurDayHasPeriodSch() )
            {
                MMIPUB_OpenQueryWinByTextId(TXT_CALENDAR_DEL_CDR_DAY_PERIODSCH,IMAGE_PUBWIN_QUERY,PNULL,PNULL);//当日日程有重复项,是否也删除?
            }
            else
            {
                date.mday = calendar_ctrl_ptr->solar_date;
                date.year = calendar_ctrl_ptr->solar_year;
                date.mon = calendar_ctrl_ptr->solar_month;
                if (0 == MMIAPISCH_GetTodaySchNumber(&date))
                {
                    MMIPUB_OpenAlertWarningWin(TXT_CALENDAR_EMPTY);
                    MMK_CloseWin(win_id);
                }
                else
                {
                    MMIPUB_OpenQueryWinByTextId(TXT_CALENDAR_CLEAR_TODAY,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                }
            }
            break;

        case CALENDAR_OPT_CLEAR_ALL:
            if(0==MMIAPISCH_GetAllSchNumber())
            {
                MMIPUB_OpenAlertWarningWin(TXT_CALENDAR_EMPTY);
            }
            else
            {
                MMIPUB_OpenQueryWinByTextId(TXT_CALENDAR_CLEAR_ALL,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            }
            //MMK_CloseWin(win_id);
            break;

        case CALENDAR_OPT_QUERY:
            if(PNULL!=to_date_param)
            {
                SCI_FREE(to_date_param);
                to_date_param=PNULL;
            }
            to_date_param=SCI_ALLOC_APP(sizeof(uint32));
            *to_date_param=CALENDAR_TO_DATE_CALENDAR;
            MMIAPICALEND_OpenCalendarQueryByDate((ADD_DATA)to_date_param);
            //MMK_CloseWin(win_id);
            break;

        case CALENDAR_OPT_WEEK:
            MMK_CreateWin((uint32 *)CALENDAR_WEEKLY_WIN_TAB, PNULL);
            MMK_CloseWin(win_id);
            break;

        case CALENDAR_OPT_MONTH:
            MMK_CloseWin(MMICALENDAR_WEEKLY_WIN_ID);
            MMK_CloseWin(win_id);
            break;
#ifdef MMI_CALENDAR_DAILY_SUPPORT
        case CALENDAR_OPT_DAY:
            {
               /* MMISET_DATE_T *date=(MMISET_DATE_T *)SCI_ALLOC_APP(sizeof(MMISET_DATE_T));
                if(date!=PNULL)
                {
                    date->date_day=calendar_ctrl_ptr->solar_date;
                    date->date_month=calendar_ctrl_ptr->solar_month;
                    date->date_year=calendar_ctrl_ptr->solar_year;
                    MMK_CreateWin((uint32 *)CALENDAR_DAILY_WIN_TAB,date);
                }*/
                MMK_CreateWin((uint32 *)CALENDAR_DAILY_WIN_TAB,PNULL);
                MMK_CloseWin(win_id);
            }
            break;
#endif
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
        case CALENDAR_OPT_LUNAR_DETAIL:
            if(NULL != detail_win_param)
            {
                SCI_FREE(detail_win_param);
                detail_win_param=PNULL;
            }
            detail_win_param = SCI_ALLOC_APP(sizeof(MMISET_DATE_T));
            if(detail_win_param != PNULL)
            {
                SCI_MEMSET(detail_win_param, 0, sizeof(MMISET_DATE_T));

                detail_win_param->date_year= calendar_ctrl_ptr->solar_year;
                detail_win_param->date_month= calendar_ctrl_ptr->solar_month;
                detail_win_param->date_day= calendar_ctrl_ptr->solar_date;
                MMK_CreateWin((uint32 *)CALENDAR_LUNAR_DETAIL_WIN_TAB, (ADD_DATA)detail_win_param);
            }
            break;
#endif
        default:
            break;
        }
        break;

    case MSG_PROMPTWIN_OK: 
        MMIPUB_CloseQuerytWin(PNULL);
        node_id = GUIMENU_GetCurNodeId(ctrl_id);
        switch( node_id )
		{
		case CALENDAR_OPT_CLEAR_TODAY:

            date.mday = calendar_ctrl_ptr->solar_date;
            date.year = calendar_ctrl_ptr->solar_year;
            date.mon = calendar_ctrl_ptr->solar_month;
			if (0 == MMIAPISCH_GetTodaySchNumber(&date))
			{
                MMIPUB_OpenAlertWarningWin(TXT_CALENDAR_EMPTY);
			    MMK_CloseWin(win_id);
            }
			else
			{
				MMIALM_OpenDeleteWaittingWin(MSG_SCH_LIST_DEL_CURDATE);
				if (MMIAPICALENDAR_GetCurDayHasPeriodSch())
				{
					MMIAPICALENDAR_SetIfDelPeriodSch(TRUE);
				}
				MMK_CloseWin(win_id);
			}
			break;

		case CALENDAR_OPT_CLEAR_ALL:
			if (0 == MMIAPISCH_GetAllSchNumber())
			{
                MMIPUB_OpenAlertWarningWin(TXT_CALENDAR_EMPTY);
		        MMK_CloseWin(win_id);
            }
			else
			{
				MMIALM_OpenDeleteWaittingWin(MSG_SCH_LIST_DEL_ALL);
				MMK_CloseWin(win_id);
			}
			break;	
			
        default:
            break;			
		}
        break;

    case MSG_PROMPTWIN_CANCEL: 
		 if (MMIAPICALENDAR_IsCurDayHasAppointSch(CALENDAR_OPT_SCH_NOT_PERIOD) && MMIAPICALENDAR_GetCurDayHasPeriodSch())
		{
			MMIALM_OpenDeleteWaittingWin(MSG_SCH_LIST_DEL_CURDATE);
			MMIAPICALENDAR_SetIfDelPeriodSch(FALSE);
		}
	        MMIPUB_CloseQuerytWin(PNULL);
	        MMK_CloseWin(win_id);
        	break;

    case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;

    case MSG_SCH_EVENT_EDIT_FINISH:
        if (MMIALM_AddOneEvent(&s_win_list_param->event,&index))
        {
        }
	else
        {
		if(MMIACC_SCH_NUM<=(MMIAPISCH_GetAllSchNumber()))
		{
		    MMIPUB_OpenAlertWarningWin(TXT_SCHEDULE_FULL);
		}
		else
		{
		    MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
		}      
	} 
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        SCI_FREE(is_weekly_ptr);
		if(NULL != detail_win_param)
        {
            SCI_FREE(detail_win_param);
			detail_win_param=PNULL;
        }
		if(PNULL!=to_date_param)
		{
			SCI_FREE(to_date_param);
			to_date_param=PNULL;
		}
       break;

    default:
	    ret_val = MMI_RESULT_FALSE;
	    break;
    }

    return ret_val;
}

#ifndef MMI_CALENDAR_KEY_STYLE
/*****************************************************************************/
//    Description : 
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL void DrawWeekPageIndication(
                          MMI_CALENDAR_T*    calendar_ctrl_ptr
                          )
{   
    DrawPageIndication(calendar_ctrl_ptr->win_id,1,GetCurLcdInfo());
}
#endif
/*****************************************************************************/
//    Description : draw weekly window
//    Global resource dependence : 
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL void DrawWeekCalendar(
                              MMI_CALENDAR_T* calendar_ctrl_ptr,
                              BOOLEAN              is_draw_all
                              )
{
    //SCI_ASSERT(NULL != calendar_ctrl_ptr);/*assert verified*/
    if(PNULL == calendar_ctrl_ptr)
    {
        return;
    }
    
    calendar_ctrl_ptr->view_type = GUI_CALENDAR_WEEK_VIEW;

    if( is_draw_all )
    {
        SetRedrawMode( calendar_ctrl_ptr, GUI_CALENDAR_REDRAW_ALL );
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
        GUIWIN_SetSoftkeyTextId( MMICALENDAR_WEEKLY_WIN_ID,  STXT_OPTION, TXT_NULL, STXT_RETURN, TRUE );
		GUIWIN_SeSoftkeytButtonIconId(MMICALENDAR_WEEKLY_WIN_ID, IMAGE_NULL, 1, TRUE);
#else
		GUIWIN_SetSoftkeyTextId( MMICALENDAR_WEEKLY_WIN_ID,  STXT_OPTION, TXT_NULL, STXT_RETURN, TRUE );
#endif
    }
    if( GUI_CALENDAR_REDRAW_ALL == calendar_ctrl_ptr->redraw_mode )
    {
        DrawBackground( calendar_ctrl_ptr );
    }


    DrawWeeklyTitle( calendar_ctrl_ptr );

#ifdef TOUCH_PANEL_SUPPORT
	InitWeeklyButtonChtrl(calendar_ctrl_ptr->win_id);
#if defined(MMI_ISTYLE_SUPPORT)
    if(!MMITHEME_IsIstyle())
#endif
	{
		MMK_SendMsg(MMICALENDAR_WEEK_LEFT_BUTTON_CTRL_ID,MSG_CTL_PAINT,PNULL);
        MMK_SendMsg(MMICALENDAR_WEEK_RIGHT_BUTTON_CTRL_ID,MSG_CTL_PAINT,PNULL);
	}
    //DrawWeeklyArrow(calendar_ctrl_ptr);
#endif

#ifdef MMI_GUI_STYLE_MINISCREEN
    DrawWeekDates( calendar_ctrl_ptr );
    DrawWeeks( calendar_ctrl_ptr );
#else
    DrawWeeks( calendar_ctrl_ptr );
    DrawWeekDates( calendar_ctrl_ptr );
#endif
#ifndef MMI_CALENDAR_KEY_STYLE
#ifndef MMI_GUI_STYLE_MINISCREEN
    if(!MMITHEME_IsIstyle())
    {
	    DrawWeekPageIndication(calendar_ctrl_ptr);
    }
#endif
#endif
    ResetRedrawMode( calendar_ctrl_ptr );

}

/*****************************************************************************/
//    Description : draw week date
//    Global resource dependence : 
//    Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawWeekDate(
                        MMI_CALENDAR_T* calendar_ctrl_ptr,
                        GUI_RECT_T*    rect_ptr,
                        uint16               year,
                        uint8                month,
                        uint8                day,
                        uint32               row,
                        BOOLEAN              is_highlight
                        )
{
    
    uint16          sche_num = 0;
    MMI_IMAGE_ID_T  sche_icon_1 = IMAGE_NULL;
    MMI_IMAGE_ID_T  sche_icon_2 = IMAGE_NULL;
    SCI_DATE_T          today_t = {0};
    //GUI_POINT_T         dis_point = {0};
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
	GUI_COLOR_T  color      = 0;
    GUI_LCD_DEV_INFO *lcd_info_ptr=PNULL;
    GUI_RECT_T  sch_rect={0};

#ifdef MMI_GUI_STYLE_MINISCREEN
    GUI_RECT_T     rect_day={0};  //显示day（日）的窗口
    uint16         lcd_width=0;
    uint16         lcd_height=0;
#endif

    lcd_info_ptr=GetCurLcdInfo();
    color = MMI_BLACK_COLOR;

#ifdef MMI_GUI_STYLE_MINISCREEN
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    rect_day.left=rect_ptr->left+1;
    rect_day.right=rect_ptr->right-2;
    rect_day.top=rect_ptr->top+1;
    rect_day.bottom=rect_ptr->bottom-2;
    rect_ptr->left=0;
    rect_ptr->right=lcd_width-1;
#else
//     rect_ptr->left=rect_ptr->left+1;
//     rect_ptr->right=rect_ptr->right-2;
//     rect_ptr->top=rect_ptr->top+1;
//     rect_ptr->bottom=rect_ptr->bottom-2;
#endif
    //color = MMITHEME_GetCurThemeFontColor(MMI_THEME_CALENDAR_WEEK);;
    TM_GetSysDate(&today_t);

    IMG_EnableTransparentColor( TRUE ); 
    if( MMIAPISCH_GetOneDateSche( year, month, day, &sche_num, &sche_icon_1, &sche_icon_2 ) )//有日程
    {
        if( is_highlight )//当前焦点
        {
            color = GUI_RGB2RGB565(255, 255, 255);

            if (day == today_t.mday && today_t.year == year && today_t.mon == month)//当前焦点为今天有日程
            {
                GUIRES_DisplayImg(PNULL,
    				rect_ptr,
    				PNULL,
    				calendar_ctrl_ptr->win_id,
    				IMAGE_CLOCK_CALENDAR_DEFAULT,
    				lcd_info_ptr);
                
            }
            else//当前焦点有日程
            {
                GUIRES_DisplayImg(PNULL,
					rect_ptr,
					PNULL,
					calendar_ctrl_ptr->win_id,
					IMAGE_CLOCK_CALENDAR_GREEN_DEFAULT,//IMAGE_CLOCK_CALENDAR_DEFAULT,
					lcd_info_ptr);
            }

            GUIRES_DisplayImg(PNULL,
                rect_ptr,
                PNULL,
                calendar_ctrl_ptr->win_id,
                IMAGE_CLOCK_CALENDAR_COMMENT,
                lcd_info_ptr);
        }
        else
        {
            if (day == today_t.mday && today_t.year == year && today_t.mon == month)//今天有日程
            {
                color = GUI_RGB2RGB565(255, 255, 255);
                GUIRES_DisplayImg(PNULL,
					rect_ptr,
					PNULL,
					calendar_ctrl_ptr->win_id,
					IMAGE_CLOCK_CALENDAR_FOCUSED,//IMAGE_CLOCK_CALENDAR_GREEN_DEFAULT,
					lcd_info_ptr);
            }
            
            GUIRES_DisplayImg(PNULL,
                    rect_ptr,
                    PNULL,
                    calendar_ctrl_ptr->win_id,
                    IMAGE_CLOCK_CALENDAR_COMMENT,
                    lcd_info_ptr);
        }
        
        if( GUI_CALENDAR_REDRAW_ALL == calendar_ctrl_ptr->redraw_mode )
        {
#ifndef MMI_GUI_STYLE_MINISCREEN
            uint16 lcd_width=0;
            uint16 lcd_height=0;
            GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
#endif
            sch_rect.top=rect_ptr->top;
            sch_rect.bottom=rect_ptr->bottom;
            if(!MMITHEME_IsMainScreenLandscape())
            {
#if defined (MAINLCD_SIZE_128X160) || defined (MAINLCD_SIZE_176X220)
                sch_rect.left=2*CALENDAR_BLOCK_WIDTH_V+4;
#else
                sch_rect.left=2*CALENDAR_BLOCK_WIDTH_V+5;
#endif
            }
            else
            {
#if defined (MAINLCD_SIZE_128X160) || defined (MAINLCD_SIZE_176X220)
                sch_rect.left=2*CALENDAR_BLOCK_WIDTH_H+6;
#else
                sch_rect.left=2*CALENDAR_BLOCK_WIDTH_H+5;
#endif				
            }
            sch_rect.right=lcd_width-1;
            DrawSchduleText(calendar_ctrl_ptr,
                &sch_rect,
                year,
                month,
                day);
        }
    }
    else//无日程
    {
        if( is_highlight )//当前焦点
        {
            color = GUI_RGB2RGB565(255, 255, 255);
            if (day == today_t.mday && today_t.year == year && today_t.mon == month)//当前焦点为今天有日程
            {
                GUIRES_DisplayImg(PNULL,
    				rect_ptr,
    				PNULL,
    				calendar_ctrl_ptr->win_id,
    				IMAGE_CLOCK_CALENDAR_DEFAULT,
    				lcd_info_ptr);
                
            }
            else
            {
                GUIRES_DisplayImg(PNULL,
					rect_ptr,
					PNULL,
					calendar_ctrl_ptr->win_id,
					IMAGE_CLOCK_CALENDAR_GREEN_DEFAULT,//IMAGE_CLOCK_CALENDAR_DEFAULT,
					lcd_info_ptr);
            }
        }
        else
        {
            if (day == today_t.mday && today_t.year == year && today_t.mon == month)//今天无日程
            {
                color = GUI_RGB2RGB565(255, 255, 255);
                GUIRES_DisplayImg(PNULL,
					rect_ptr,
					PNULL,
					calendar_ctrl_ptr->win_id,
					IMAGE_CLOCK_CALENDAR_FOCUSED,//IMAGE_CLOCK_CALENDAR_GREEN_DEFAULT,
					lcd_info_ptr);

            }
        }
    }

    if ( s_weekly_dayrect_info[row].is_valid )
    {
        MMI_STRING_T num_string = CreateNumString( day );
		
        SetCalendarTextStyle(&text_style, ALIGN_HVMIDDLE, CALENDAR_DATE_FONT, color);
        // display
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)lcd_info_ptr,

#ifdef MMI_GUI_STYLE_MINISCREEN
               (const GUI_RECT_T      *)&rect_day,       //the fixed display area
               (const GUI_RECT_T      *)&rect_day,
#else
               (const GUI_RECT_T      *)rect_ptr,       //the fixed display area
               (const GUI_RECT_T      *)rect_ptr,       //用户要剪切的实际区域
#endif
            (const MMI_STRING_T    *)&num_string,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
        
        MMIAPICOM_DestroyString( num_string );
    }

    IMG_EnableTransparentColor( FALSE ); 
}

/*****************************************************************************/
//    Description : draw week dates
//    Global resource dependence : 
//    Author: James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DrawWeekDates(
                         MMI_CALENDAR_T* calendar_ctrl_ptr
                         )
{
    //SCI_ASSERT(NULL != calendar_ctrl_ptr);/*assert verified*/
    if(PNULL == calendar_ctrl_ptr)
    {
        return;
    }
    
    if( GUI_CALENDAR_REDRAW_PART <= calendar_ctrl_ptr->redraw_mode )
    {
        uint8              weekday = MMICALENDAR_GetWeekday( calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, calendar_ctrl_ptr->solar_date );
        uint16             offset = ( WEEKDAY_NUM + weekday - calendar_ctrl_ptr->first_day_of_week ) % WEEKDAY_NUM;
        CALE_SOLAR_DATE_T  date_in = {0};
        CALE_SOLAR_DATE_T  date_out = {0};
        uint32             row = 0;
        uint32             col = 0;
        uint8              days_of_month = 0;
        GUI_RECT_T         rect = {0};
        GUI_RECT_T         bg_rect = {0};

        if( GUI_CALENDAR_REDRAW_PART == calendar_ctrl_ptr->redraw_mode )
        {
            if( calendar_ctrl_ptr->pre_date != calendar_ctrl_ptr->solar_date )
            {
                /* current */
                GetPosByDate( calendar_ctrl_ptr, calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, calendar_ctrl_ptr->solar_date, &row, &col );
                rect = GetDateRect( calendar_ctrl_ptr, row, col );
                bg_rect = rect;
//                bg_rect.left=rect.left+1;
//                bg_rect.right=rect.right-2;
//                bg_rect.top=rect.top+1;
//                bg_rect.bottom=rect.bottom-2;
                DrawBackgroundByRect( calendar_ctrl_ptr, &rect );
                //DrawWeekDateRect(&rect);
                DrawWeekDate( calendar_ctrl_ptr, &rect, calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, calendar_ctrl_ptr->solar_date, row, TRUE );
                
                /* pre */
                GetPosByDate( calendar_ctrl_ptr, calendar_ctrl_ptr->pre_year, calendar_ctrl_ptr->pre_month, calendar_ctrl_ptr->pre_date, &row, &col );
                rect = GetDateRect( calendar_ctrl_ptr, row, col );
                bg_rect = rect;
//                bg_rect.left=rect.left+1;
//                bg_rect.right=rect.right-2;
//                bg_rect.top=rect.top+1;
//                bg_rect.bottom=rect.bottom-2;
                DrawBackgroundByRect( calendar_ctrl_ptr, &bg_rect );
                //DrawWeekDateRect(&rect);
                DrawWeekDate( calendar_ctrl_ptr, &rect, calendar_ctrl_ptr->pre_year, calendar_ctrl_ptr->pre_month, calendar_ctrl_ptr->pre_date, row, FALSE );
                DrawWeekBackgroundLines();
            }
        }
        else
        {             
            //draw date
            date_in.year = calendar_ctrl_ptr->solar_year;
            date_in.month = calendar_ctrl_ptr->solar_month;
            date_in.day = calendar_ctrl_ptr->solar_date;

            if( calendar_ctrl_ptr->first_day_of_week == weekday )
            {
                date_out.year = date_in.year;
                date_out.month = date_in.month;
                date_out.day = date_in.day;
            }
            else
            {
                MoveDays( date_in, offset, FALSE, &date_out );
            }

            // 设置所有天的rect有效
            for (row = 0; row < WEEKDAY_NUM; row++)
            {
                s_weekly_dayrect_info[row].is_valid = TRUE;
            }
            
            // 设置无效的区域
            if (QUERY_YEAR_BOTTOM == calendar_ctrl_ptr->solar_year && 1 == calendar_ctrl_ptr->solar_month && 1 == calendar_ctrl_ptr->solar_date )
            {
                for (row = 0; row < weekday; row++)
                {
                    s_weekly_dayrect_info[row].is_valid = FALSE;
                }
            }
            // 设置无效的区域
            if (QUERY_YEAR_TOP == calendar_ctrl_ptr->solar_year && 12 == calendar_ctrl_ptr->solar_month && 31 == calendar_ctrl_ptr->solar_date )
            {
                for (row = weekday+1; row < WEEKDAY_NUM; row++)
                {
                    s_weekly_dayrect_info[row].is_valid = FALSE;
                }
            }
            
            days_of_month = MMICALENDAR_GetMonthday( date_out.year, date_out.month );

            for( row = 0; row < WEEKDAY_NUM; row++ )
            {
                rect = GetDateRect( calendar_ctrl_ptr, row, col );

                DrawWeekDate( calendar_ctrl_ptr, &rect, date_out.year, date_out.month, date_out.day, row, (BOOLEAN)( offset == row ) );
          
                s_weekly_dayrect_info[row].date.day = date_out.day;
                s_weekly_dayrect_info[row].date.month = date_out.month;
                s_weekly_dayrect_info[row].date.year = date_out.year;
                
                if (date_out.year == QUERY_YEAR_TOP && date_out.month == 12 && date_out.day == 31 && row < WEEKDAY_NUM - 1)
                {
                    s_weekly_dayrect_info[row + 1].is_valid = FALSE;
                    break;
                }
                
                if(date_out.day < days_of_month)
                {
                    date_out.day++;
                }
                else
                {
                    date_in = date_out;
                    MoveNextDay(date_in, &date_out);
                }
            }
        }
    }
}
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//    Description : HandleWeeklyWinTPDown
//    Global resource dependence : 
//    Author: xiangjie
//    Note:
/*****************************************************************************/
LOCAL void HandleWeeklyWinTPPress(
								MMI_MESSAGE_ID_E        msg_id,
								MMI_CALENDAR_T* 	calendar_ctrl_ptr,
								GUI_POINT_T             point
								)
{
#ifdef TOUCH_PANEL_SUPPORT
    uint32 				i = 0;
#ifndef MMI_GUI_STYLE_MINISCREEN
    BOOLEAN             is_in_rect = FALSE;
#endif
    //GUI_RECT_T* 			rect = PNULL;
	//MMI_IMAGE_ID_T*   bmp_on_id= PNULL;
		
    //GUI_POINT_T         dis_point = {0};

    GUI_RECT_T  day_rect={0};
    BOOLEAN is_screen_landscape=FALSE;

    GUI_RECT_T s_weekly_dayrect_v[WEEKDAY_NUM]={0};

    GUI_RECT_T s_weekly_dayrect_h[WEEKDAY_NUM]={0};
    int16 isstyle_h=0;
    int16 isstyle_week_date_y=0; 

    //SCI_ASSERT(calendar_ctrl_ptr != PNULL);/*assert verified*/
    if(PNULL == calendar_ctrl_ptr)
    {
        return;
    }

    if(s_cur_tp_win!=calendar_ctrl_ptr->win_id)
    {
        return ;
    }
    
    for (i = 0; i < WEEKDAY_NUM; i++)
    {
#if defined(MMI_ISTYLE_SUPPORT)    
        if(MMITHEME_IsIstyle())
        {
            isstyle_h=CALENDAR_ISSTYLE_WEEKLY_SELECT_HEIGHT_V;
            isstyle_week_date_y=CALENDAR_ISSTYLE_WEEKLY_DATE_Y_V;
        }
        else
#endif			
        {
            isstyle_h=CALENDAR_WEEKLY_SELECT_HEIGHT_V;
            isstyle_week_date_y=CALENDAR_WEEKLY_DATE_Y_V;
        }
        s_weekly_dayrect_v[i].left = 1;
        s_weekly_dayrect_v[i].top = isstyle_week_date_y + isstyle_h * i;/*lint !e737*/
        s_weekly_dayrect_v[i].right = MMITHEME_GetMainScreenRightPixel();
        s_weekly_dayrect_v[i].bottom = isstyle_week_date_y + isstyle_h * (i + 1);/*lint !e737*/
#if defined(MMI_ISTYLE_SUPPORT)
        if(MMITHEME_IsIstyle())
        {
            s_weekly_dayrect_v[i].top+=MMITHEME_GetWinTitleHeight();
            s_weekly_dayrect_v[i].bottom+=MMITHEME_GetWinTitleHeight();
        }
        
        if(MMITHEME_IsIstyle())
        {
            isstyle_h=CALENDAR_ISSTYLE_WEEKLY_SELECT_HEIGHT_H;
            isstyle_week_date_y=CALENDAR_ISSTYLE_WEEKLY_DATE_Y_H;
        }
        else
#endif			
        {
            isstyle_h=CALENDAR_WEEKLY_SELECT_HEIGHT_H;
            isstyle_week_date_y=CALENDAR_WEEKLY_DATE_Y_H;
        }
        s_weekly_dayrect_h[i].left = CALENDAR_BLOCK_WIDTH_H;
        s_weekly_dayrect_h[i].top = isstyle_week_date_y + isstyle_h * i;/*lint !e737*/
        s_weekly_dayrect_h[i].right = 2*CALENDAR_BLOCK_WIDTH_H - 1;
        s_weekly_dayrect_h[i].bottom = isstyle_week_date_y + isstyle_h * (i + 1);/*lint !e737*/
#if defined(MMI_ISTYLE_SUPPORT)		
        if(MMITHEME_IsIstyle())
        {
            s_weekly_dayrect_h[i].top+=MMITHEME_GetWinTitleHeight();
            s_weekly_dayrect_h[i].bottom+=MMITHEME_GetWinTitleHeight();
        }
#endif		
    }
//箭头太小，扩大TP响应范围
    if(!MMITHEME_IsMainScreenLandscape())
    {
        is_screen_landscape=FALSE;
    }
    else
    {
        is_screen_landscape=TRUE;
    }


    for ( i = 0; i < WEEKDAY_NUM; i++ )
    {
        if(is_screen_landscape)
        {
            day_rect=s_weekly_dayrect_h[i];
        }
        else
        {
            day_rect=s_weekly_dayrect_v[i];
        }
        
        if( s_weekly_dayrect_info[i].is_valid && GUI_PointIsInRect( point,day_rect ) )
        {
            if ( MSG_TP_PRESS_DOWN == msg_id )
            {
                calendar_ctrl_ptr->pre_date  = calendar_ctrl_ptr->solar_date;
                calendar_ctrl_ptr->pre_month = calendar_ctrl_ptr->solar_month;
                calendar_ctrl_ptr->pre_year  = calendar_ctrl_ptr->solar_year;
                
                calendar_ctrl_ptr->solar_date = s_weekly_dayrect_info[i].date.day;
                calendar_ctrl_ptr->solar_month = s_weekly_dayrect_info[i].date.month;
                calendar_ctrl_ptr->solar_year = s_weekly_dayrect_info[i].date.year;
                
                DrawWeekCalendar( calendar_ctrl_ptr, TRUE );
#ifndef MMI_GUI_STYLE_MINISCREEN
                is_in_rect = TRUE;
#endif
                s_weekly_down_rect=day_rect;
                break;
            }
            if ((MSG_TP_PRESS_UP == msg_id))
            {
                if(GUI_PointIsInRect( point,s_weekly_down_rect ))
                {
                    MMK_SendMsg(MMICALENDAR_WEEKLY_WIN_ID, MSG_APP_WEB, NULL);
                }
                SCI_MEMSET(&s_weekly_down_rect,0,sizeof(GUI_RECT_T));
#ifndef MMI_GUI_STYLE_MINISCREEN
                is_in_rect = TRUE;
#endif
                break;
            }
        }
    }

    if ( MSG_TP_PRESS_UP == msg_id
        && calendar_ctrl_ptr->title_button_index > 0 )
    {
        calendar_ctrl_ptr->title_button_index = 0;
    }
#endif    
}
#endif
/*****************************************************************************/
//    Description : calendar weekly display window
//    Global resource dependence : 
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCalendarWeeklyWinMsg(
									        MMI_WIN_ID_T        win_id, 
									        MMI_MESSAGE_ID_E    msg_id, 
									        DPARAM              param
									        )
{
    MMI_RESULT_E            ret_val = MMI_RESULT_TRUE;
    MMI_CALENDAR_T     *calendar_ctrl_ptr = &g_calendar_show;
    BOOLEAN                 *is_weekly_ptr = NULL;
    SCI_DATE_T              date = {0};
    uint16                       index = 0;
#ifdef TOUCH_PANEL_SUPPORT
    MMI_CTRL_ID_T   ctrl_id=MMICALENDAR_WEEK_LEFT_BUTTON_CTRL_ID;
#endif
    //GUI_BG_T  bg_info={0};
    
    calendar_ctrl_ptr->win_id = win_id;

    switch( msg_id )
    {
	case MSG_OPEN_WINDOW:
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
		GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, FALSE);
#else
		GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
#endif
#ifdef TOUCH_PANEL_SUPPORT
        InitWeeklyButtonChtrl(win_id);
#endif
#if defined(MMI_ISTYLE_SUPPORT)
        if(MMITHEME_IsIstyle())
        {
            MMI_CONTROL_CREATE_T create={0};
            GUITITLE_INIT_DATA_T   title_data   = {0};
            title_data.text_id=TXT_TOOLS_CALENDAR;
            create.ctrl_id = MMITHEME_GetTitleCtrlId();
            create.guid    = SPRD_GUI_TITLE_ID;
            create.parent_win_handle = win_id;
            create.init_data_ptr = &title_data;
            MMK_CreateControl( &create );
            MMK_SetWinTitleCtrlId( win_id, create.ctrl_id );
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
#endif		
#ifdef MMI_CALENDAR_SLIDE
        if(s_first_lcd_dev_info.block_id==UILAYER_NULL_HANDLE)
        {
            CalendarCreateLayer(&s_first_lcd_dev_info,MMICALENDAR_WEEKLY_WIN_ID);

        }
        if(s_second_lcd_dev_info.block_id==UILAYER_NULL_HANDLE)
        {
            CalendarCreateLayer(&s_second_lcd_dev_info,MMICALENDAR_WEEKLY_WIN_ID);
        }
        if(s_statusbar_dev_info.block_id==UILAYER_NULL_HANDLE)
        {
            CalendarCreateStatusBarLayer(&s_statusbar_dev_info,MMICALENDAR_WEEKLY_WIN_ID);
        }
#ifndef MMI_PDA_SUPPORT
        if(s_softkey_dev_info.block_id==UILAYER_NULL_HANDLE)
        {
            CalendarCreateSoftkeyLayer(&s_softkey_dev_info,MMICALENDAR_WEEKLY_WIN_ID);
        }
#endif
        SetCurLcdInfo(&s_calendar_lcd_dev_info);
#endif
        /*bg_info.bg_type=GUI_BG_COLOR;
        bg_info.color=MMI_CALENDAR_BG_COLOR;
        GUISTATUSBAR_SetBgIsDesplay(win_id,TRUE);
        GUISTATUSBAR_SetBg(win_id,bg_info);*/
		break;
		
	case MSG_FULL_PAINT:
        DrawWeekCalendar( calendar_ctrl_ptr, TRUE );
		break;

	case MSG_KEYDOWN_CANCEL:
#ifdef MMI_CALENDAR_WIN_SLIDE
		MMISLIDE_Deactive(&s_win_slide_handle);
        MMISLIDE_Release(&s_win_slide_handle);
#if 0//release???????
        MMK_CloseWin(MMICALENDAR_DAILY_WIN_ID);
        MMK_CloseWin(MMICALENDAR_WIN_ID);
		MMK_CloseWin(win_id);
#endif
#else
		MMK_CloseWin(win_id);
#endif
		break;       

    case MSG_SCH_EVENT_EDIT_FINISH:
        if (MMIALM_AddOneEvent(&s_win_param->event,&index))
        {
        }
	 else
        {
		if(MMIACC_SCH_NUM<=(MMIAPISCH_GetAllSchNumber()))
		{
		    MMIPUB_OpenAlertWarningWin(TXT_SCHEDULE_FULL);
		}
		else
		{
		    MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
		}      
	} 
        break;

    case MSG_CLOSE_WINDOW:

#ifdef TOUCH_PANEL_SUPPORT
        s_cur_tp_win=0;
#endif
#ifdef MMI_CALENDAR_SLIDE
        ExitSlidecalendar(&s_slide_calendar,FALSE);
        if(s_first_lcd_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_first_lcd_dev_info);   /*lint !e506 !e774*/

        }
        if(s_second_lcd_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_second_lcd_dev_info);   /*lint !e506 !e774*/
        }
        if(s_statusbar_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_statusbar_dev_info);   /*lint !e506 !e774*/
        }
#ifndef MMI_PDA_SUPPORT
        if(s_softkey_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_softkey_dev_info);   /*lint !e506 !e774*/
        }
#endif

#endif
        break;

    case MSG_APP_WEB:
        date.mday = calendar_ctrl_ptr->solar_date;
        date.year = calendar_ctrl_ptr->solar_year;
        date.mon = calendar_ctrl_ptr->solar_month;
        if (0 == MMIAPISCH_GetTodaySchNumber(&date))
		{
            s_win_param->win_id = win_id;
            MMIAPISCH_InitNewSch(calendar_ctrl_ptr->solar_year,
                              calendar_ctrl_ptr->solar_month,
                              calendar_ctrl_ptr->solar_date,
                              &s_win_param->event);
            MMIAPISCH_OpenEditWin((ADD_DATA)s_win_param);
        }
        else
        {
            s_win_list_param->year = calendar_ctrl_ptr->solar_year;
            s_win_list_param->month = calendar_ctrl_ptr->solar_month;
            s_win_list_param->day = calendar_ctrl_ptr->solar_date;
            MMIAPISCH_OpenViewListWin(s_win_list_param);
        }
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
	case MSG_APP_MENU:
		if (MMK_IsOpenWin(MMICALENDAR_OPTION_WIN_ID))
		{
			MMK_CloseWin(MMICALENDAR_OPTION_WIN_ID);
		}		
        is_weekly_ptr = SCI_ALLOC_APP(sizeof(BOOLEAN));
        *is_weekly_ptr = FALSE;
        MMK_CreateWin((uint32 *)CALENDAR_OPTION_WIN_TAB, (ADD_DATA)is_weekly_ptr);
        break;
		
    case  MSG_APP_UP:	
        HandleCalendarKey(calendar_ctrl_ptr, MOVE_PREV_DAY);
        DrawWeekCalendar( calendar_ctrl_ptr, TRUE );
        break;
		
    case  MSG_APP_DOWN:
        HandleCalendarKey(calendar_ctrl_ptr, MOVE_NEXT_DAY);
        DrawWeekCalendar( calendar_ctrl_ptr, TRUE );
        break;

    case  MSG_APP_LEFT:	
        HandleCalendarKey(calendar_ctrl_ptr, MOVE_PREV_WEEK);
#ifdef MMI_GUI_STYLE_MINISCREEN
        DrawWeekCalendar( calendar_ctrl_ptr, TRUE );
#else
        DrawWeekCalendar( calendar_ctrl_ptr, FALSE );
#endif
        break;

    case  MSG_APP_RIGHT:
        HandleCalendarKey(calendar_ctrl_ptr, MOVE_NEXT_WEEK);
#ifdef MMI_GUI_STYLE_MINISCREEN
        DrawWeekCalendar( calendar_ctrl_ptr, TRUE );
#else
        DrawWeekCalendar( calendar_ctrl_ptr, FALSE );
#endif
        break;

    case MSG_APP_HASH:
    case MSG_APP_STAR:
        if (MMICOM_RETURN_INCREASE ==MMIAPICOM_IsIncrease(msg_id))
        {	   
            HandleCalendarKey(calendar_ctrl_ptr, MOVE_NEXT_WEEK);
        }
        else
        {
            HandleCalendarKey(calendar_ctrl_ptr, MOVE_PREV_WEEK);
        }
        DrawWeekCalendar( calendar_ctrl_ptr, TRUE);
        break;

#ifdef TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
        ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
        if(MMICALENDAR_WEEK_LEFT_BUTTON_CTRL_ID==ctrl_id)
        {
            MMK_SendMsg(win_id, MSG_APP_LEFT,  PNULL);
        }
        else if(MMICALENDAR_WEEK_RIGHT_BUTTON_CTRL_ID==ctrl_id)
        {
            MMK_SendMsg(win_id, MSG_APP_RIGHT,  PNULL);
        }
        break;
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            s_cur_tp_win=win_id;
            HandleWeeklyWinTPPress(msg_id, calendar_ctrl_ptr, point);
#ifdef MMI_CALENDAR_SLIDE
            HandleSlideTpDown(&point,&s_slide_calendar);
#endif
		}		
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef MMI_CALENDAR_SLIDE
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T point;
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            HandleSlideTpMove(&point,&s_slide_calendar,win_id,GUI_CALENDAR_WEEK_VIEW);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_TIMER:
        HandleSlideCalendarTimer(&s_slide_calendar, param);
        break;
    case MSG_LCD_SWITCH:
        ExitSlidecalendar(&s_slide_calendar,FALSE);
        if(s_first_lcd_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_first_lcd_dev_info);   /*lint !e506 !e774*/

        }
        if(s_second_lcd_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_second_lcd_dev_info);   /*lint !e506 !e774*/
        }
        if(s_statusbar_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_statusbar_dev_info);   /*lint !e506 !e774*/
        }
#ifndef MMI_PDA_SUPPORT
        if(s_softkey_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_softkey_dev_info);   /*lint !e506 !e774*/
        }
#endif
#if defined(MMI_ISTYLE_SUPPORT)
        if(MMITHEME_IsIstyle())
        {
            MMI_CONTROL_CREATE_T create={0};
            GUITITLE_INIT_DATA_T   title_data   = {0};
            title_data.text_id=TXT_TOOLS_CALENDAR;
            create.ctrl_id = MMITHEME_GetTitleCtrlId();
            create.guid    = SPRD_GUI_TITLE_ID;
            create.parent_win_handle = win_id;
            create.init_data_ptr = &title_data;
            MMK_DestroyControl(MMITHEME_GetTitleCtrlId());
            MMK_CreateControl( &create );
            MMK_SetWinTitleCtrlId( win_id, create.ctrl_id );
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
#endif		
        CalendarCreateLayer(&s_first_lcd_dev_info,MMICALENDAR_WEEKLY_WIN_ID);
        CalendarCreateLayer(&s_second_lcd_dev_info,MMICALENDAR_WEEKLY_WIN_ID);
        CalendarCreateStatusBarLayer(&s_statusbar_dev_info,MMICALENDAR_WEEKLY_WIN_ID);
#ifndef MMI_PDA_SUPPORT
        CalendarCreateSoftkeyLayer(&s_softkey_dev_info,MMICALENDAR_WEEKLY_WIN_ID);
#endif

        SetCurLcdInfo(&s_calendar_lcd_dev_info);
        break;
    case MSG_GET_FOCUS:
        if(s_first_lcd_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_first_lcd_dev_info);   /*lint !e506 !e774*/

        }
        if(s_second_lcd_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_second_lcd_dev_info);   /*lint !e506 !e774*/
        }
        if(s_statusbar_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_statusbar_dev_info);   /*lint !e506 !e774*/
        }
#ifndef MMI_PDA_SUPPORT
        if(s_softkey_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_softkey_dev_info);   /*lint !e506 !e774*/
        }
#endif
#if defined(MMI_ISTYLE_SUPPORT)
        if(MMITHEME_IsIstyle())
        {
            MMI_CONTROL_CREATE_T create={0};
            GUITITLE_INIT_DATA_T   title_data   = {0};
            title_data.text_id=TXT_TOOLS_CALENDAR;
            create.ctrl_id = MMITHEME_GetTitleCtrlId();
            create.guid    = SPRD_GUI_TITLE_ID;
            create.parent_win_handle = win_id;
            create.init_data_ptr = &title_data;
            MMK_DestroyControl(MMITHEME_GetTitleCtrlId());
            MMK_CreateControl( &create );
            MMK_SetWinTitleCtrlId( win_id, create.ctrl_id );
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
#endif		
        CalendarCreateLayer(&s_first_lcd_dev_info,MMICALENDAR_WEEKLY_WIN_ID);
        CalendarCreateLayer(&s_second_lcd_dev_info,MMICALENDAR_WEEKLY_WIN_ID);
        CalendarCreateStatusBarLayer(&s_statusbar_dev_info,MMICALENDAR_WEEKLY_WIN_ID);
#ifndef MMI_PDA_SUPPORT
        CalendarCreateSoftkeyLayer(&s_softkey_dev_info,MMICALENDAR_WEEKLY_WIN_ID);
#endif
        SetCurLcdInfo(&s_calendar_lcd_dev_info);
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        break;
#endif
    case MSG_LOSE_FOCUS:
#ifdef TOUCH_PANEL_SUPPORT
        s_cur_tp_win=0;
#endif
 #ifdef MMI_CALENDAR_SLIDE
        ExitSlidecalendar(&s_slide_calendar,FALSE);
        if(s_first_lcd_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_first_lcd_dev_info);   /*lint !e506 !e774*/
        }
        if(s_second_lcd_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_second_lcd_dev_info);   /*lint !e506 !e774*/
        }
        if(s_statusbar_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_statusbar_dev_info);   /*lint !e506 !e774*/
        }
#ifndef MMI_PDA_SUPPORT
        if(s_softkey_dev_info.block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(&s_softkey_dev_info);   /*lint !e506 !e774*/
        }
#endif
#endif
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_UP:	
		{
            GUI_POINT_T point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            HandleWeeklyWinTPPress(msg_id, calendar_ctrl_ptr, point);
#ifdef MMI_CALENDAR_SLIDE
            HandleSlideTpUp(&point,&s_slide_calendar,GUI_CALENDAR_WEEK_VIEW);
#endif
		}		
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		
    default:
		ret_val = MMI_RESULT_FALSE;
	    break;
    }

    return ret_val;
}

/*****************************************************************************/
//    Description : calendar query window
//    Global resource dependence : 
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCalendarQueryWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{
    MMI_RESULT_E        ret_val             = MMI_RESULT_TRUE;
    MMI_CALENDAR_T      *calendar_ctrl_ptr  = &g_calendar_show;
    MMI_CTRL_ID_T       ctrl_id             = MMICALENDAR_QUERY_DATEEDITBOX_CTRL_ID;
    static uint16       s_old_year          = 0;
    static uint8        s_old_month         = 0;
    static uint8        s_old_day           = 0;
    //MMISET_DATE_T      appoint_date        = {0};  
    MMISET_DATE_DISPLAY_TYPE_E date_display_type=MMISET_DATE_YMD;
    GUIEDIT_DATE_STYLE_E date_display_style=GUIEDIT_DATE_STYLE_YMD;
    MMISET_DATE_T       date                = {0};
    static uint32       s_win_param_query   =0;
    //MMI_DAILY_CALENDAR_T* daily_calendar=&s_daily_calendar;

    calendar_ctrl_ptr->win_id = win_id;

    switch( msg_id )
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_PDA_SUPPORT
        GUIFORM_IsSlide(MMICALENDAR_EDIT_DATE_FORM_CTRL_ID,FALSE);
#endif
        s_win_param_query =*(uint32*) MMK_GetWinAddDataPtr(win_id);

        s_old_year = calendar_ctrl_ptr->solar_year;
        s_old_month = calendar_ctrl_ptr->solar_month;
        s_old_day = calendar_ctrl_ptr->solar_date;

        date_display_type = MMIAPISET_GetDateDisplayType();
        if(MMISET_DATE_YMD == date_display_type)
        {
            date_display_style = GUIEDIT_DATE_STYLE_YMD;
        }
        else if(MMISET_DATE_MDY == date_display_type)
        {
            date_display_style = GUIEDIT_DATE_STYLE_MDY;
        }
        else if(MMISET_DATE_DMY == date_display_type)
        {
            date_display_style = GUIEDIT_DATE_STYLE_DMY;
        }
        GUIEDIT_SetDateStyle(ctrl_id,PNULL,&date_display_style,FALSE);
        GUIEDIT_SetDateRange(ctrl_id,GUIEDIT_DATE_YEAR_RANGE_QUERY);
        if(s_win_param_query==CALENDAR_TO_DATE_ALL_SCH_LIST)
        {
            MMISET_DATE_T* all_list_date=MMIAPISCH_GetAllListCurData();
            GUIEDIT_SetDate(ctrl_id,
            all_list_date->date_year,
            all_list_date->date_month,
            all_list_date->date_day);
        }
        else
        {
            GUIEDIT_SetDate(ctrl_id,
            calendar_ctrl_ptr->solar_year,
            calendar_ctrl_ptr->solar_month,
            calendar_ctrl_ptr->solar_date);
        }

#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:          
    case MSG_APP_WEB:
    //case MSG_APP_MENU:
        GUIEDIT_GetDate(ctrl_id, 
            &date.date_year,
            &date.date_month,
            &date.date_day);
        if (MMICALENDAR_IsDateValid(date.date_year,date.date_month,date.date_day))
        {
            calendar_ctrl_ptr->solar_year = date.date_year;
            calendar_ctrl_ptr->solar_month = date.date_month;
            calendar_ctrl_ptr->solar_date = date.date_day;
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_INVALID_DATE);
            break;
        }

        if(s_win_param_query==CALENDAR_TO_DATE_SCH_LIST ||s_win_param_query==CALENDAR_TO_DATE_ALL_SCH_LIST)
        {
            if(NULL != s_win_list_param)
            {
                SCI_FREE(s_win_list_param);
            }
            s_win_list_param = SCI_ALLOC_APP(sizeof(MMISCH_LIST_WIN_PARAM_T));
            SCI_MEMSET(s_win_list_param, 0, sizeof(MMISCH_LIST_WIN_PARAM_T));

            s_win_list_param->year = calendar_ctrl_ptr->solar_year;
            s_win_list_param->month = calendar_ctrl_ptr->solar_month;
            s_win_list_param->day = calendar_ctrl_ptr->solar_date;
            MMIAPISCH_OpenViewListWin(s_win_list_param);
        }
        else if(s_win_param_query==CALENDAR_TO_DATE_DAILY_CALENDAR )
        {
            //MMK_PostMsg(MMICALENDAR_DAILY_WIN_ID, MSG_SCH_DAILY_UPDATE, PNULL, 0);
        }
        else
        {
/*//#ifdef MMI_PDA_SUPPORT
            //  if (MMK_IsOpenWin(MMICALENDAR_PDA_MAIN_WIN_ID))
            //  {
            //      MMK_ReturnAppointedWin(MMICALENDAR_PDA_MAIN_WIN_ID);
            //  }
            //#else
            if (MMK_IsOpenWin(MMICALENDAR_WIN_ID))
            {
                MMK_ReturnAppointedWin(MMICALENDAR_WIN_ID);
            }
//#endif
            else
            {
                appoint_date.date_day=calendar_ctrl_ptr->solar_date;
                appoint_date.date_month=calendar_ctrl_ptr->solar_month;
                appoint_date.date_year=calendar_ctrl_ptr->solar_year;
                if(MMIAPICOM_CheckDateIsValid(appoint_date))
                {
                    SetAppointedDate(calendar_ctrl_ptr->solar_date, 
                        calendar_ctrl_ptr->solar_month, 
                        calendar_ctrl_ptr->solar_year);
                    SetIsAppointedDate(TRUE);
                }
                else
                {
                    SetIsAppointedDate(FALSE);
                }
                MMIAPICALEND_OpenMainWin();
                if (MMK_IsOpenWin(MMISCH_VIEW_WIN_ID))
                {
                    MMK_CloseWin(MMISCH_VIEW_WIN_ID);
                }
            }*/
        }

        if(MMK_IsOpenWin(MMISCH_VIEW_DETAIL_WIN_ID))
        {
            MMK_CloseWin(MMISCH_VIEW_DETAIL_WIN_ID);
        }
        if(MMK_IsOpenWin(MMISCH_LIST_OPTION_WIN_ID))
        {
            MMK_CloseWin(MMISCH_LIST_OPTION_WIN_ID);
        }
        if(MMK_IsOpenWin(MMICALENDAR_OPTION_WIN_ID))
        {
            MMK_CloseWin(MMICALENDAR_OPTION_WIN_ID);
        }
        if(MMK_IsOpenWin(MMISCH_ALL_LIST_OPTION_WIN_ID))
        {
            MMK_CloseWin(MMISCH_ALL_LIST_OPTION_WIN_ID);
        }
        if(MMK_IsOpenWin(MMICALENDAR_DAILY_OPTION_WIN_ID))
        {
            MMK_CloseWin(MMICALENDAR_DAILY_OPTION_WIN_ID);
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        calendar_ctrl_ptr->solar_year = s_old_year;
        calendar_ctrl_ptr->solar_month = s_old_month;
        calendar_ctrl_ptr->solar_date = s_old_day;
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if(MMK_IsOpenWin(MMISCH_LIST_OPTION_WIN_ID))
        {
            MMK_CloseWin(MMISCH_LIST_OPTION_WIN_ID);
        }
        if(MMK_IsOpenWin(MMICALENDAR_OPTION_WIN_ID))
        {
            MMK_CloseWin(MMICALENDAR_OPTION_WIN_ID);
        }
        if(MMK_IsOpenWin(MMISCH_ALL_LIST_OPTION_WIN_ID))
        {
            MMK_CloseWin(MMISCH_ALL_LIST_OPTION_WIN_ID);
        }
        MMK_CloseWin(MMISCH_OPTION_WIN_ID);
        break;

    default:
        ret_val = MMI_RESULT_FALSE;
        break;
    }

    return ret_val;
}


/*****************************************************************************/
//  Description : to handle the message of waiting window    
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWaitWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E        result  = MMI_RESULT_FALSE;
	static BOOLEAN      s_is_delete_current = FALSE;
    switch (msg_id)
    {
	case MSG_OPEN_WINDOW:
		//clean interrupt flag
		s_is_delete_current = FALSE;
		MMIALM_SetAlmInterruptFlag(FALSE);
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
		GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)STXT_CANCEL, FALSE);
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, FALSE);
#else
		GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)STXT_CANCEL, FALSE);
#endif
		result  = MMI_RESULT_TRUE;
		break;
    case MSG_APP_RED:   // 响应红键
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //SCI_TRACE_LOW:"[MMIALM]HandleWaitWinMsg red key pressed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICALENDAR_MAIN_5027_112_2_18_2_1_41_74,(uint8*)"");
        MMIALM_SetAlmInterruptFlag(TRUE);
        MMK_CloseWin(win_id);
         //MMK_CloseWin(MMIPB_SEARCH_LIST_WIN_ID);
        break;
	case MSG_SCH_LIST_DEL_ALL:
        if(s_is_delete_current)
		{
			if (MMIAPICALENDAR_DelCurDaySch(MMIAPICALENDAR_GetIfDelPeriodSch()))
			{
                MMK_CloseWin(MMISCH_VIEW_WIN_ID);
				MMK_CloseWin(MMISCH_VIEW_ALL_WIN_ID);
			}
			else
			{
	            MMIPUB_OpenAlertFailWin(TXT_ERROR);
			}
	        MMK_CloseWin(win_id);
			
		}
		else
		{
			//delete all event
			if(!MMIALM_DelFirstValidEvent())//(MMIALM_DelEvents(EVENT_SCH_0, EVENT_SCH_0+MMIACC_SCH_NUM-1))
			{
				if(MMIALM_IsExistValidEvent())
				{
					//delete fail
					MMIPUB_OpenAlertFailWin(TXT_ERROR);
				}
				else
				{
                    MMK_CloseWin(MMISCH_VIEW_WIN_ID);
#ifndef  MMI_PDA_SUPPORT                   
					MMK_CloseWin(MMISCH_VIEW_ALL_WIN_ID);
#endif
				}
				MMK_CloseWin(win_id);
			}
			
		}
         break;
    case MSG_SCH_LIST_DEL_CURDATE:
		s_is_delete_current = TRUE;
		MMK_SendMsg(win_id, MSG_SCH_LIST_DEL_ALL, PNULL);
		break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.		
    case MSG_APP_FLIP:  // 不响应翻盖
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif
        
    case MSG_CLOSE_WINDOW:
        //MMIAUDIO_ResumeBgPlay(MMIBGPLAY_MODULE_PBVIEW);
        MMIAPICALENDAR_SetCurDayHasPeriodSch(FALSE);
        MMIAPICALENDAR_SetIfDelPeriodSch(FALSE);
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}


/*****************************************************************************/
// 	Description : Open Delete Waiting window
//	Global resource dependence : 
//  Author:        
//	Note:   
/*****************************************************************************/
PUBLIC void MMIALM_OpenDeleteWaittingWin(MMI_MESSAGE_ID_E pos_msg_id)
{ 
	MMI_STRING_T                prompt_str = {0};
	
	MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);
	MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISCH_DELETE_ANIM_WIN_ID,IMAGE_NULL,
		ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleWaitWinMsg);				
	MMK_PostMsg(MMISCH_DELETE_ANIM_WIN_ID, pos_msg_id, PNULL, 0);
}

/*****************************************************************************/
//    Description : MMIAPICALENDAR_SetCurDayHasPeriodSch
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPICALENDAR_SetCurDayHasPeriodSch(BOOLEAN is_has_periodsch)
{
    s_is_today_has_periodsch = is_has_periodsch;
}

/*****************************************************************************/
//    Description : MMIAPICALENDAR_GetCurDayHasPeriodSch
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICALENDAR_GetCurDayHasPeriodSch( void )
{
    return s_is_today_has_periodsch;
}

/*****************************************************************************/
//    Description : MMIAPICALENDAR_SetIfDelPeriodSch
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPICALENDAR_SetIfDelPeriodSch(BOOLEAN is_del_periodsch)
{
    s_is_del_periodsch = is_del_periodsch;
}

/*****************************************************************************/
//    Description : MMIAPICALENDAR_GetIfDelPeriodSch
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICALENDAR_GetIfDelPeriodSch( void )
{
    return s_is_del_periodsch;
}

/*****************************************************************************/
// 	Description : check if cur day has period sch or other sch
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPICALENDAR_IsCurDayHasAppointSch(CALENDAR_DEL_SCH_TYPE_E sch_type )
{
	uint16                          i                  = 0;
	const MMIACC_SMART_EVENT_FAST_TABLE_T      *event_ptr         = NULL;
	BOOLEAN							result             = FALSE;
	MMI_CALENDAR_T             *calendar_ctrl_ptr = &g_calendar_show;

	SCI_DATE_T		event_start_date                         = {0};
	SCI_DATE_T		event_end_date                  = {0};
	SCI_DATE_T          	cur_date                             = {0};

	cur_date.year = calendar_ctrl_ptr->solar_year;
	cur_date.mon = calendar_ctrl_ptr->solar_month;
	cur_date.mday = calendar_ctrl_ptr->solar_date;    

	for (i = 0; i < MMIACC_SCH_NUM; i++)
	{
		event_ptr = MMIALM_GeEvent((uint16)(EVENT_SCH_0 + i));

		event_start_date.year = event_ptr->start_year;
		event_start_date.mon  = event_ptr->start_mon;
		event_start_date.mday = event_ptr->start_day;
		event_end_date.year = event_ptr->end_year;
		event_end_date.mon  = event_ptr->end_mon;
		event_end_date.mday = event_ptr->end_day;	

		switch (event_ptr->fre_mode)
		{
            case ALM_MODE_ONCE:
            case ALM_MODE_NO:
                if(   	( CALENDAR_OPT_SCH_NOT_PERIOD == sch_type ) && 
                    (cur_date.year == event_ptr->year &&
                    cur_date.mon == event_ptr->mon &&
                    cur_date.mday == event_ptr->day)
                    )
                {
                    result = TRUE;
                    return result; 
                }
                break;
            
			case ALM_MODE_EVERYDAY:
			case ALM_MODE_EVERYWEEK:
			case ALM_MODE_EVERYMONTH:
			case ALM_MODE_EVERYYEAR:
				if( event_start_date.year==event_end_date.year
					&& event_start_date.mon==event_end_date.mon
					&& event_start_date.mday==event_end_date.mday
					)
				{
					result = FALSE;
				}
				else if (
					(CALENDAR_OPT_SCH_PERIOD == sch_type) && MMIAPISCH_IsInDateRange(event_start_date,event_end_date,cur_date)  
					)
				{
                    if(ALM_MODE_EVERYDAY==event_ptr->fre_mode)
                    {
                        result = TRUE;
                    }
                    else if(ALM_MODE_EVERYWEEK==event_ptr->fre_mode)
                    {
                        if((event_ptr->fre & s_week_mask[MMICALENDAR_GetWeekday(cur_date.year,cur_date.mon,cur_date.mday)]) != 0)
                        {
                            return TRUE;
                        }
                    }
                    else if(ALM_MODE_EVERYMONTH==event_ptr->fre_mode)
                    {
                        if(cur_date.mday == event_ptr->day)
                        {
                            return TRUE;
                        }
                    }
                    else if(ALM_MODE_EVERYYEAR==event_ptr->fre_mode)
                    {
                        if(cur_date.mday == event_ptr->day
                            && cur_date.mon== event_ptr->mon)
                        {
                            return TRUE;
                        }
                    }
                }
                break;

            default:
                break;
        }
	}

    return result;	
}
/*****************************************************************************/
// 	Description : set isAppointedDate
//	Global resource dependence : s_is_appointed_date
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SetIsAppointedDate(BOOLEAN is_appointed)
{
	s_is_appointed_date=is_appointed;
}

/*****************************************************************************/
// 	Description : get isAppointedDate
//	Global resource dependence : s_is_appointed_date
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN GetIsAppointedDate(void)
{
	return s_is_appointed_date;
}

#if 0
/*****************************************************************************/
// 	Description : set AppointedDate
//	Global resource dependence : s_appointed_date
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SetAppointedDate(uint8 date,uint8 month,uint16 year)
{
	s_appointed_date.date_day=date;
	s_appointed_date.date_month=month;
	s_appointed_date.date_year=year;
}
#endif
/*****************************************************************************/
// 	Description : Get AppointedDate
//	Global resource dependence : s_appointed_date
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL MMISET_DATE_T* GetAppointedDate(void)
{
	return &s_appointed_date;
}

#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:  renyi.hu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleViewLunarDetailWinMsg(
                                             MMI_WIN_ID_T		win_id,
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM				param
                                             )
{
	MMI_RESULT_E	            recode          = MMI_RESULT_TRUE;
	static MMISET_DATE_T   *s_win_param_detail = PNULL;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_win_param_detail =(MMISET_DATE_T*) MMK_GetWinAddDataPtr(win_id);               
        MMK_SetAtvCtrl(win_id, MMICALENDAR_LUNAR_DETAIL_TEXT_CTRL_ID);
        InitLunarDetail(s_win_param_detail, MMICALENDAR_LUNAR_DETAIL_TEXT_CTRL_ID);
        GUIRICHTEXT_SetFocusMode(MMICALENDAR_LUNAR_DETAIL_TEXT_CTRL_ID, GUIRICHTEXT_FOCUS_SHIFT_NONE);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;	
	case MSG_CLOSE_WINDOW:
		if(MMK_IsOpenWin(MMICALENDAR_OPTION_WIN_ID))
		{
			MMK_CloseWin(MMICALENDAR_OPTION_WIN_ID);
		}
		if(MMK_IsOpenWin(MMISCH_LIST_OPTION_WIN_ID))
		{
			MMK_CloseWin(MMISCH_LIST_OPTION_WIN_ID);
		}
		if(MMK_IsOpenWin(MMISCH_ALL_LIST_OPTION_WIN_ID))
		{
			MMK_CloseWin(MMISCH_ALL_LIST_OPTION_WIN_ID);
		}
		break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
/*****************************************************************************/
// 	Description : set lunar detail win
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void InitLunarDetail( 
                                MMISET_DATE_T*	date,
                                MMI_CTRL_ID_T ctrl_id
                                )
{
    uint8			    temp_str[MMISET_DATE_STR_LEN] = {0};
    wchar               wchar_temp_str[MMISET_DATE_STR_LEN +1] = {0};
    uint16              nIndex = 0;
	uint8                   lun_month = 0;
    uint8                   lun_day = 0;
    uint16                  lun_year = 0;
	wchar                   year_str[10] = {0};
    uint16                   year_len = 0;
    wchar                   month_str[CALENDAR_LEAP_MONTH_STR_LEN] = {0};
    uint16                   month_len = 0;
	MMI_STRING_T            year_month_day = {0};
    uint8      feast_index=0;
    GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
    //MMI_STRING_T detail_str = {0};
    wchar feast[3]={0x8282,0x6C14,0xff1a};
    wchar festival[3] = {0x8282,0x65E5,0xff1a};
    wchar shengxiao[3]={0x751f,0x8096,0xff1a};
    wchar solar[5]={0x9633,0x5386,0x65E5,0x671F,0xff1a};
    wchar lunar[5]={0x519c,0x5386,0x65E5,0x671F,0xff1a};
    wchar *shenxiao_str=PNULL;
    CALENDAR_FESTIVAL_TYPE_E      festival_index =0;

	
    //SCI_ASSERT(date != PNULL);/*assert verified*/
    if(PNULL == date)
    {
        return;
    }
   
    //阴历:
    MMICALENDAR_SolarToLunar(date->date_year, date->date_month, date->date_day, &lun_year, &lun_month, &lun_day);
	FormatLunarInfo(lun_year, lun_month, lun_day,month_str, &month_len,year_str, &year_len); 
	year_month_day.wstr_ptr = (wchar*)SCI_ALLOC_APP(sizeof(wchar)*(year_len + month_len + 1));
	if(PNULL != year_month_day.wstr_ptr)
	{
		SCI_MEMSET(year_month_day.wstr_ptr, 0, sizeof(wchar)*(year_len + month_len + 1));
		year_month_day.wstr_len = year_len + month_len;
		MMIAPICOM_Wstrcpy(year_month_day.wstr_ptr, year_str);
		MMIAPICOM_Wstrcat(year_month_day.wstr_ptr, month_str);			
	    //AddOneDetailItem(ctrl_id,TXT_CALENDAR_QUERY_LUNAR,PNULL,&nIndex,year_month_day.wstr_ptr,year_month_day.wstr_len);
        SCI_MEMSET(&item_data,0,sizeof(item_data));
        SetCommonCharacter(&item_data);
        item_data.text_data.buf.len = 5;
        item_data.text_data.buf.str_ptr = lunar;
        GUIRICHTEXT_AddItem (ctrl_id,&item_data,&nIndex);

        SCI_MEMSET(&item_data,0,sizeof(item_data));
        SetCommonCharacter(&item_data);
        item_data.text_data.buf.len = year_month_day.wstr_len;
        item_data.text_data.buf.str_ptr = year_month_day.wstr_ptr;
        GUIRICHTEXT_AddItem (ctrl_id,&item_data,&nIndex);
        SCI_FREE(year_month_day.wstr_ptr);
	}

	//生肖:
	SetCommonCharacter(&item_data);
    item_data.text_data.buf.len = 3;
    item_data.text_data.buf.str_ptr = shengxiao;
    GUIRICHTEXT_AddItem (ctrl_id,&item_data,&nIndex);
    
    shenxiao_str=MMICALENDAR_GetLunarShengxiaoString(lun_year);
    SCI_MEMSET(&item_data,0,sizeof(item_data));
    SetCommonCharacter(&item_data);
    item_data.text_data.buf.len = 1;
    item_data.text_data.buf.str_ptr = shenxiao_str;
    GUIRICHTEXT_AddItem (ctrl_id,&item_data,&nIndex);

	//阳历:
	SCI_MEMSET(temp_str,0,sizeof(temp_str));
    SCI_MEMSET(wchar_temp_str,0,sizeof(wchar_temp_str));   
    MMIAPISET_FormatDateStrByDateStyle(date->date_year, date->date_month, date->date_day,'.',temp_str,MMISET_DATE_STR_LEN);

    MMIAPICOM_StrToWstr(temp_str,wchar_temp_str );
    //AddOneDetailItem(ctrl_id,TXT_COMMON_DATE,PNULL,&nIndex,wchar_temp_str,(uint16)strlen((char*)temp_str)); 
    SCI_MEMSET(&item_data,0,sizeof(item_data));
    SetCommonCharacter(&item_data);
    item_data.text_data.buf.len = 5;
    item_data.text_data.buf.str_ptr = solar;
    GUIRICHTEXT_AddItem (ctrl_id,&item_data,&nIndex);

    SCI_MEMSET(&item_data,0,sizeof(item_data));
    SetCommonCharacter(&item_data);
    item_data.text_data.buf.len = (uint16)strlen((char*)temp_str);
    item_data.text_data.buf.str_ptr = wchar_temp_str;
    GUIRICHTEXT_AddItem (ctrl_id,&item_data,&nIndex);

	//节气:
	if(MMICALENDAR_IsLunarFeast(date->date_year, date->date_month, date->date_day,&feast_index))
    {
        wchar *feast_str=MMICALENDAR_GetLunarFeastString(feast_index);
        if(PNULL!=feast_str)
        {
            SCI_MEMSET(&item_data,0,sizeof(item_data));
            SetCommonCharacter(&item_data);
            item_data.text_data.buf.len = 3;
            item_data.text_data.buf.str_ptr = feast;
            GUIRICHTEXT_AddItem (ctrl_id,&item_data,&nIndex);

            SCI_MEMSET(&item_data,0,sizeof(item_data));
            SetCommonCharacter(&item_data);
            item_data.text_data.buf.len = 2;
            item_data.text_data.buf.str_ptr = feast_str;
            GUIRICHTEXT_AddItem (ctrl_id,&item_data,&nIndex);
        }
    }
    //节日
    if(MMICANLENDAR_IsLunarFestival(lun_year,lun_month,lun_day,&festival_index))
    {
        wchar *str=MMICALENDAR_GetLunarFestivalString(festival_index);
        if(PNULL!=str)
        {
            SCI_MEMSET(&item_data,0,sizeof(item_data));
            SetCommonCharacter(&item_data);
            item_data.text_data.buf.len = 3;
            item_data.text_data.buf.str_ptr = festival;
            GUIRICHTEXT_AddItem (ctrl_id,&item_data,&nIndex);

            SCI_MEMSET(&item_data,0,sizeof(item_data));
            SetCommonCharacter(&item_data);
            item_data.text_data.buf.len = 3;
            item_data.text_data.buf.str_ptr = str;
            GUIRICHTEXT_AddItem (ctrl_id,&item_data,&nIndex);
        }
    }
}

/*****************************************************************************/
// Description : set common character
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void SetCommonCharacter( GUIRICHTEXT_ITEM_T* p_item )
{
    SCI_PASSERT(PNULL != p_item,("[MMIMPEG4]:SetCommonCharacter,param error")); /*assert verified*/  

    p_item->img_type = GUIRICHTEXT_IMAGE_NONE;
    p_item->text_type = GUIRICHTEXT_TEXT_BUF;
}
#endif
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
/*****************************************************************************/
// 	Description : delete current day all schedule
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
PUBLIC  void MMIAPICALENDAR_OpenLunarDetailWin(ADD_DATA add_data_ptr)
{
	MMK_CreateWin((uint32 *)CALENDAR_LUNAR_DETAIL_WIN_TAB, add_data_ptr);
}
#endif
/*****************************************************************************/
// Description : GetCurLcdInfo
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO * GetCurLcdInfo( void)
{
#ifdef MMI_CALENDAR_SLIDE
    return s_cur_lcd_dev_info_ptr;
#else
    return &s_calendar_lcd_dev_info;
#endif
}

#ifdef MMI_CALENDAR_SLIDE
/*****************************************************************************/
//  Description : create  latyer
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CalendarCreateLayer(GUI_LCD_DEV_INFO *dev_info_ptr,MMI_HANDLE_T handle)
{
    uint16 logic_lcd_width = 0;
    uint16 logic_lcd_height = 0;
    GUI_RECT_T client_rect= MMITHEME_GetClientRectEx(handle);

    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T    create_info = {0};
        GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &logic_lcd_width, &logic_lcd_height);
        create_info.lcd_id = MAIN_LCD_ID;
        create_info.owner_handle =handle;
        create_info.offset_x = 0;
        create_info.offset_y = client_rect.bottom;
        create_info.width = logic_lcd_width;
        create_info.height = logic_lcd_height-client_rect.top-1;
        create_info.is_bg_layer = TRUE;
        create_info.is_static_layer = FALSE;   
        UILAYER_CreateLayer(&create_info, dev_info_ptr);
    }
}

/*****************************************************************************/
//  Description : create  latyer
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CalendarCreateStatusBarLayer(GUI_LCD_DEV_INFO *dev_info_ptr,MMI_HANDLE_T handle)
{
    uint16 logic_lcd_width = 0;
    uint16 logic_lcd_height = 0;
    GUI_RECT_T client_rect= MMITHEME_GetClientRectEx(handle);

    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T    create_info = {0};
        GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &logic_lcd_width, &logic_lcd_height);
        create_info.lcd_id = MAIN_LCD_ID;
        create_info.owner_handle =handle;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = logic_lcd_width;
        create_info.height =client_rect.top+1;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;   
        UILAYER_CreateLayer(&create_info, dev_info_ptr);
    }
}
#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : create  latyer
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CalendarCreateSoftkeyLayer(GUI_LCD_DEV_INFO *dev_info_ptr,MMI_HANDLE_T handle)
{
    uint16 logic_lcd_width = 0;
    uint16 logic_lcd_height = 0;
    GUI_RECT_T client_rect= MMITHEME_GetClientRectEx(handle);

    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T    create_info = {0};
        GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &logic_lcd_width, &logic_lcd_height);
        create_info.lcd_id = MAIN_LCD_ID;
        create_info.owner_handle =handle;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = logic_lcd_width;
        create_info.height =logic_lcd_height-client_rect.bottom;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;   
        UILAYER_CreateLayer(&create_info, dev_info_ptr);
    }
}
#endif

/*****************************************************************************/
// Description : SetCurLcdInfo
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void SetCurLcdInfo( GUI_LCD_DEV_INFO *dev_info_ptr)
{
    s_cur_lcd_dev_info_ptr=dev_info_ptr;
}
/*****************************************************************************/
//  Description : HandleSlideTpDown
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleSlideTpDown(
                                GUI_POINT_T       *tp_point_ptr,    // [in]
                                MMI_SLIDE_CALENDAR_T    *slide_calendar_ptr   // [in]
                                )
{
    if(slide_calendar_ptr->slide_state!=MMK_TP_SLIDE_NONE)
    {
        ExitSlidecalendar(slide_calendar_ptr,TRUE);;
    }
    SCI_MEMSET(slide_calendar_ptr,0,sizeof(MMI_SLIDE_CALENDAR_T));
    //InitSlidecalendar(slide_calendar_ptr,handle);
    // 增加速度点
    MMK_AddVelocityItem( tp_point_ptr->x, tp_point_ptr->y );

    // 记录起始点
    slide_calendar_ptr->slide_start_point.x = tp_point_ptr->x;
    slide_calendar_ptr->slide_start_point.y = tp_point_ptr->y;

    // 记录前一个点
    slide_calendar_ptr->slide_pre_point.x = tp_point_ptr->x;
    slide_calendar_ptr->slide_pre_point.y = tp_point_ptr->y;

    slide_calendar_ptr->slide_state = MMK_TP_SLIDE_NONE;

    
    //StartSlideRedrawTimer(slide_calendar_ptr);
}
/*****************************************************************************/
//  Description : to check whether need to redraw
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
__inline LOCAL BOOLEAN IsTpMoved(
                                 MMI_SLIDE_CALENDAR_T    *slide_calendar_ptr,  // [in]
                                 GUI_POINT_T    *point_ptr
                                 )
{
    if (abs(point_ptr->y - slide_calendar_ptr->slide_start_point.y) > CALENDAR_TP_MOVE_DIS
        /*|| abs(point_ptr->y - slide_calendar_ptr->slide_start_point.y) > CALENDAR_TP_MOVE_DIS*/)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : HandleSlideTpDown
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleSlideTpMove(
                                GUI_POINT_T       *point_ptr,    // [in]
                                MMI_SLIDE_CALENDAR_T    *slide_calendar_ptr,    // [in]
                                MMI_HANDLE_T    handle,
                                CALENDAR_VIEW_TYPE_E view_type
                                )
{
    GUI_RECT_T client_rect= MMITHEME_GetClientRectEx(slide_calendar_ptr->handle);
    // 增加速度点
    MMK_AddVelocityItem( point_ptr->x, point_ptr->y );

    if (MMK_TP_SLIDE_NONE == slide_calendar_ptr->slide_state)
    {
        if(IsTpMoved(slide_calendar_ptr, point_ptr)&&point_ptr->y >client_rect.top)
        {
            InitSlidecalendar(slide_calendar_ptr,handle);
            StartSlideRedrawTimer(slide_calendar_ptr);

            slide_calendar_ptr->view_type=view_type;
            
            slide_calendar_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;
            
            if(point_ptr->y < slide_calendar_ptr->slide_pre_point.y)
            {
                slide_calendar_ptr->direction=MOVE_UP;
            }
            else
            {
                slide_calendar_ptr->direction=MOVE_DOWN;
            }

            slide_calendar_ptr->slide_pre_point.x = point_ptr->x;
            slide_calendar_ptr->slide_pre_point.y = point_ptr->y;
            if(slide_calendar_ptr->slide_start_point.y <=client_rect.top)
            {
                slide_calendar_ptr->slide_start_point.x = point_ptr->x;
                slide_calendar_ptr->slide_start_point.y = client_rect.top+1;
            }
        }
    }

    
    return;
}

/*****************************************************************************/
//  Description : HandleSlideTpDown
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleSlideTpUp(
                                GUI_POINT_T       *point_ptr,    // [in]
                                MMI_SLIDE_CALENDAR_T    *slide_calendar_ptr ,   // [in]
                                CALENDAR_VIEW_TYPE_E view_type
                                )
{
    slide_calendar_ptr->view_type=view_type;
    if(MMK_TP_SLIDE_TP_SCROLL==slide_calendar_ptr->slide_state)
    {
        // 增加速度点
        StopSlideRedrawTimer(slide_calendar_ptr);
        MMK_AddVelocityItem( point_ptr->x, point_ptr->y );
        MMK_ComputeCurrentVelocity(PNULL,&slide_calendar_ptr->velocity, 0, FLING_MAX_VELOCITY);/*lint !e747*/
        slide_calendar_ptr->slide_pre_point.x = point_ptr->x;
        slide_calendar_ptr->slide_pre_point.y = point_ptr->y;
        if (FLING_MIN_VELOCITY <= abs((int32)slide_calendar_ptr->velocity))  // 达到一定速度，需要滑动
        {
            // 进入滑动状态，起动滑动定时器
            slide_calendar_ptr->slide_state = MMK_TP_SLIDE_FLING;
            StartSlideFlingTimer(slide_calendar_ptr);
        }
        else
        {
            slide_calendar_ptr->slide_state = MMK_TP_SLIDE_RECOVER;
            UpdateMoveDirection(slide_calendar_ptr);
            StartSlideRecoverTimer(slide_calendar_ptr);

        }
    }
    else
    {
        ExitSlidecalendar(slide_calendar_ptr,TRUE);
    }
    return;
}
/*****************************************************************************/
//  Description : handle timer
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL uint8 UpdateLayer(
                               MMI_SLIDE_CALENDAR_T    *slide_calendar_ptr, // [in]
                               int32 y_offset
                               )
{
    int16 first_x=0;
    int16 first_y=0;
    int16 second_x=0;
    int16 second_y=0;
    uint8 result=0;
    int16 first_y_offset=0;
    uint16 lcd_width=0;
    uint16 lcd_height=0;
    GUI_RECT_T client_rect= MMITHEME_GetClientRectEx(slide_calendar_ptr->handle);
    UILAYER_GetLayerPosition(&s_first_lcd_dev_info,&first_x,&first_y);
    UILAYER_GetLayerPosition(&s_second_lcd_dev_info,&second_x,&second_y);
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    
    
    if(y_offset>0)
    {
        first_y_offset=first_y+y_offset;
        if(MOVE_PREV_MONTH==slide_calendar_ptr->move_mode || MOVE_PREV_WEEK==slide_calendar_ptr->move_mode)
        {
            if(first_y_offset>=lcd_height)
            {
                UILAYER_SetLayerPosition(&s_first_lcd_dev_info,0,lcd_height);
                UILAYER_SetLayerPosition(&s_second_lcd_dev_info,0,client_rect.top+1);
                result=2;
            }
            else
            {
               UILAYER_SetLayerPosition(&s_first_lcd_dev_info,0,first_y_offset);
               UILAYER_SetLayerPosition(&s_second_lcd_dev_info,0,second_y+y_offset) ;
            }
        }
        else
        {
            if(first_y_offset>=client_rect.top+1)
            {
                UILAYER_SetLayerPosition(&s_first_lcd_dev_info,0,client_rect.top+1);
                UILAYER_SetLayerPosition(&s_second_lcd_dev_info,0,lcd_height);
                result=1;
            }
            else
            {
               UILAYER_SetLayerPosition(&s_first_lcd_dev_info,0,first_y_offset);
               UILAYER_SetLayerPosition(&s_second_lcd_dev_info,0,second_y+y_offset) ;
            }
        }
    }
    else if(y_offset<0)
    {
        first_y_offset=first_y+y_offset;
        if(MOVE_PREV_MONTH==slide_calendar_ptr->move_mode || MOVE_PREV_WEEK==slide_calendar_ptr->move_mode)
        {
            if(first_y_offset<=client_rect.top+1)
            {
                UILAYER_SetLayerPosition(&s_first_lcd_dev_info,0,client_rect.top+1);
                UILAYER_SetLayerPosition(&s_second_lcd_dev_info,0,2*(client_rect.top+1)-lcd_height);
                result=1;
            }
            else
            {
               UILAYER_SetLayerPosition(&s_first_lcd_dev_info,0,first_y_offset);
               UILAYER_SetLayerPosition(&s_second_lcd_dev_info,0,second_y+y_offset); 
            }
        }
        else
        {
            if(first_y_offset<=2*client_rect.top+1-lcd_height)
            {
                UILAYER_SetLayerPosition(&s_first_lcd_dev_info,0,2*(client_rect.top+1)-lcd_height);
                UILAYER_SetLayerPosition(&s_second_lcd_dev_info,0,client_rect.top+1);
                result=2;
            }
            else
            {
               UILAYER_SetLayerPosition(&s_first_lcd_dev_info,0,first_y_offset);
               UILAYER_SetLayerPosition(&s_second_lcd_dev_info,0,second_y+y_offset); 
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"UpdateLayer:error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICALENDAR_MAIN_5761_112_2_18_2_1_43_75,(uint8*)"");
    }
    
    return result;
}

/*****************************************************************************/
//  Description : handle timer
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void UpdateMoveDirection( MMI_SLIDE_CALENDAR_T    *slide_calendar_ptr)
{
    int16 first_x=0;
    int16 first_y=0;
    int16 second_x=0;
    int16 second_y=0;
    uint16 lcd_width=0;
    uint16 lcd_height=0;
    //GUI_RECT_T client_rect= MMITHEME_GetClientRectEx(slide_calendar_ptr->handle);
    UILAYER_GetLayerPosition(&s_first_lcd_dev_info,&first_x,&first_y);
    UILAYER_GetLayerPosition(&s_second_lcd_dev_info,&second_x,&second_y);
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    if(GUI_CALENDAR_MONTH_VIEW==slide_calendar_ptr->view_type)
    {
        if(slide_calendar_ptr->move_mode==MOVE_PREV_MONTH)
        {
            if(first_y<=lcd_height/3)
            {
                slide_calendar_ptr->direction=MOVE_UP;
            }
            else
            {
                slide_calendar_ptr->direction=MOVE_DOWN;
            }
        }
        else
        {
            if(second_y<=lcd_height*2/3)
            {
                slide_calendar_ptr->direction=MOVE_UP;
            }
            else
            {
                slide_calendar_ptr->direction=MOVE_DOWN;
            }
        }
    }
    else
    {
        if(slide_calendar_ptr->move_mode==MOVE_PREV_WEEK)
        {
            if(first_y<=lcd_height/3)
            {
                slide_calendar_ptr->direction=MOVE_UP;
            }
            else
            {
                slide_calendar_ptr->direction=MOVE_DOWN;
            }
        }
        else
        {
            if(second_y<=lcd_height*2/3)
            {
                slide_calendar_ptr->direction=MOVE_UP;
            }
            else
            {
                slide_calendar_ptr->direction=MOVE_DOWN;
            }
        }
    }

}
/*****************************************************************************/
//  Description : handle timer
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSlideCalendarTimer(
                                       MMI_SLIDE_CALENDAR_T    *slide_calendar_ptr, // [in]
                                       DPARAM           param           // [in]
                                       )
{
    MMI_RESULT_E        result              = MMI_RESULT_TRUE;
    GUI_POINT_T         point               = {0};
    MMI_TP_STATUS_E     state               = MMI_TP_NONE;
    int16 y_offset=0;
    //int16 y_offset=0;
    uint16 lcd_width=0;
    uint16 lcd_height=0;
    int32   old_pos = 0;        // 原来未修改之前的位置
    int32   cur_pos = 0;        // 加过偏移量后的位置
    GUI_RECT_T client_rect= MMITHEME_GetClientRectEx(slide_calendar_ptr->handle);
#ifndef MMI_PDA_SUPPORT
    GUI_RECT_T sk_rect={0};
#endif
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
#ifndef MMI_PDA_SUPPORT
     sk_rect.left=0;
     sk_rect.right=lcd_width-1;
     sk_rect.top=client_rect.bottom+1;
     sk_rect.bottom=lcd_height-1;
#endif
    if (slide_calendar_ptr->redraw_timer_id == *(uint8*)param)  // tp move timer
    {
        MMK_GetLogicTPMsg(&state, &point);
        
        // restart tp move timer
        StopSlideRedrawTimer(slide_calendar_ptr);
        StartSlideRedrawTimer(slide_calendar_ptr);
        
	    //UILAYER_SetDirectDraw( TRUE );
        if(MOVE_NONE==slide_calendar_ptr->direction || slide_calendar_ptr->slide_pre_point.y == point.y)
        {
            return MMI_RESULT_FALSE;
        }
        
        if(!slide_calendar_ptr->is_first_layer_draw)
        {
            slide_calendar_ptr->is_first_layer_draw=TRUE;
            SetCurLcdInfo(&s_first_lcd_dev_info);
            SCI_MEMCPY(slide_calendar_ptr->calendar_ctrl_ptr,&g_calendar_show,sizeof(MMI_CALENDAR_T));
            UILAYER_Clear(&s_first_lcd_dev_info);
            UILAYER_SetLayerPosition(&s_first_lcd_dev_info, 0, client_rect.top+1);
#ifndef MMI_PDA_SUPPORT
            LCD_FillRect(&s_first_lcd_dev_info,sk_rect,MMI_CALENDAR_BG_COLOR);
#endif
            if(GUI_CALENDAR_MONTH_VIEW==slide_calendar_ptr->view_type)
            {
                DrawMonthCalendar(slide_calendar_ptr->calendar_ctrl_ptr, TRUE);
            }
            else
            {
                DrawWeekCalendar(slide_calendar_ptr->calendar_ctrl_ptr, TRUE);
            }
            SetCurLcdInfo(&s_calendar_lcd_dev_info);
        }
        
        if((!slide_calendar_ptr->is_second_layer_draw)
            /*||(slide_calendar_ptr->direction!=GetMoveDirection(&point,slide_calendar_ptr,TRUE))*/)
        {
            slide_calendar_ptr->is_second_layer_draw=TRUE;
            //slide_calendar_ptr->direction=GetMoveDirection(&point,slide_calendar_ptr,TRUE);
            SetCurLcdInfo(&s_second_lcd_dev_info);
            SCI_MEMCPY(slide_calendar_ptr->calendar_ctrl_ptr,&g_calendar_show,sizeof(MMI_CALENDAR_T));
            if(GUI_CALENDAR_MONTH_VIEW==slide_calendar_ptr->view_type)
            {
                if(MOVE_UP==slide_calendar_ptr->direction)
                {
                    HandleCalendarKey(slide_calendar_ptr->calendar_ctrl_ptr, MOVE_NEXT_MONTH);
                    slide_calendar_ptr->move_mode=MOVE_NEXT_MONTH;
                }
                else if(MOVE_DOWN==slide_calendar_ptr->direction)
                {
                    HandleCalendarKey(slide_calendar_ptr->calendar_ctrl_ptr, MOVE_PREV_MONTH);
                    slide_calendar_ptr->move_mode=MOVE_PREV_MONTH;
                }
            }
            else
            {
               if(MOVE_UP==slide_calendar_ptr->direction)
                {
                    HandleCalendarKey(slide_calendar_ptr->calendar_ctrl_ptr, MOVE_NEXT_WEEK);
                    slide_calendar_ptr->move_mode=MOVE_NEXT_WEEK;
                }
                else if(MOVE_DOWN==slide_calendar_ptr->direction)
                {
                    HandleCalendarKey(slide_calendar_ptr->calendar_ctrl_ptr, MOVE_PREV_WEEK);
                    slide_calendar_ptr->move_mode=MOVE_PREV_WEEK;
                } 
            }
            UILAYER_Clear(&s_second_lcd_dev_info);
            UILAYER_SetLayerPosition(&s_second_lcd_dev_info, 0, client_rect.top+1);
#ifndef MMI_PDA_SUPPORT
            LCD_FillRect(&s_second_lcd_dev_info,sk_rect,MMI_CALENDAR_BG_COLOR);
#endif
            if(GUI_CALENDAR_MONTH_VIEW==slide_calendar_ptr->view_type)
            {
                DrawMonthCalendar(slide_calendar_ptr->calendar_ctrl_ptr, TRUE);
            }
            else
            {
                DrawWeekCalendar(slide_calendar_ptr->calendar_ctrl_ptr, TRUE);
            }
            SetCurLcdInfo(&s_calendar_lcd_dev_info);
        }
        else
        {
            int16 offset=point.y-slide_calendar_ptr->slide_start_point.y;
            CALENDAR_MOVE_MODE_E mode=slide_calendar_ptr->move_mode;
            if(GUI_CALENDAR_MONTH_VIEW==slide_calendar_ptr->view_type)
            {
                if(offset>0)
                {
                    mode=MOVE_PREV_MONTH;
                }
                else if(offset<0)
                {
                    mode=MOVE_NEXT_MONTH;
                }
                if(mode!=slide_calendar_ptr->move_mode)
                {
                    slide_calendar_ptr->move_mode=mode;
                    SetCurLcdInfo(&s_second_lcd_dev_info);
                    SCI_MEMCPY(slide_calendar_ptr->calendar_ctrl_ptr,&g_calendar_show,sizeof(MMI_CALENDAR_T));
                    if(mode==MOVE_NEXT_MONTH)
                    {
                        HandleCalendarKey(slide_calendar_ptr->calendar_ctrl_ptr, MOVE_NEXT_MONTH);
                    }
                    else if(mode==MOVE_PREV_MONTH)
                    {
                        HandleCalendarKey(slide_calendar_ptr->calendar_ctrl_ptr, MOVE_PREV_MONTH);
                    }
                    UILAYER_Clear(&s_second_lcd_dev_info);
                    UILAYER_SetLayerPosition(&s_second_lcd_dev_info, 0, client_rect.top+1);
#ifndef MMI_PDA_SUPPORT
                    LCD_FillRect(&s_second_lcd_dev_info,sk_rect,MMI_CALENDAR_BG_COLOR);
#endif
                    DrawMonthCalendar(slide_calendar_ptr->calendar_ctrl_ptr, TRUE);
                    SetCurLcdInfo(&s_calendar_lcd_dev_info); 
                }
            }
            else
            {
                if(offset>0)
                {
                    mode=MOVE_PREV_WEEK;
                }
                else if(offset<0)
                {
                    mode=MOVE_NEXT_WEEK;
                }
                if(mode!=slide_calendar_ptr->move_mode)
                {
                    slide_calendar_ptr->move_mode=mode;
                    SetCurLcdInfo(&s_second_lcd_dev_info);
                    SCI_MEMCPY(slide_calendar_ptr->calendar_ctrl_ptr,&g_calendar_show,sizeof(MMI_CALENDAR_T));
                    if(mode==MOVE_NEXT_WEEK)
                    {
                        HandleCalendarKey(slide_calendar_ptr->calendar_ctrl_ptr, MOVE_NEXT_WEEK);
                    }
                    else if(mode==MOVE_PREV_WEEK)
                    {
                        HandleCalendarKey(slide_calendar_ptr->calendar_ctrl_ptr, MOVE_PREV_WEEK);
                    }
                    UILAYER_Clear(&s_second_lcd_dev_info);
                    UILAYER_SetLayerPosition(&s_second_lcd_dev_info, 0, client_rect.top+1);
#ifndef MMI_PDA_SUPPORT
                    LCD_FillRect(&s_second_lcd_dev_info,sk_rect,MMI_CALENDAR_BG_COLOR);
#endif
                    DrawWeekCalendar(slide_calendar_ptr->calendar_ctrl_ptr, TRUE);
                    SetCurLcdInfo(&s_calendar_lcd_dev_info); 
                }
            }
        }

        if(MMK_TP_SLIDE_TP_SCROLL==slide_calendar_ptr->slide_state)
        {
            y_offset=abs((point.y-slide_calendar_ptr->slide_start_point.y));
            
            if(slide_calendar_ptr->move_mode==MOVE_NEXT_MONTH || slide_calendar_ptr->move_mode==MOVE_NEXT_WEEK)
            {
                UILAYER_SetLayerPosition(&s_first_lcd_dev_info,0,client_rect.top+1-y_offset);
                UILAYER_SetLayerPosition(&s_second_lcd_dev_info,0,lcd_height-y_offset);
            }
            else if(slide_calendar_ptr->move_mode==MOVE_PREV_MONTH || slide_calendar_ptr->move_mode==MOVE_PREV_WEEK)
            {
                UILAYER_SetLayerPosition(&s_first_lcd_dev_info,0,client_rect.top+1+y_offset);
                UILAYER_SetLayerPosition(&s_second_lcd_dev_info,0,2*(client_rect.top+1)+y_offset-lcd_height);
            }
        }

        //GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );

        // store tp point
        slide_calendar_ptr->slide_pre_point.x = point.x;
        slide_calendar_ptr->slide_pre_point.y = point.y;
    }
    else if(slide_calendar_ptr->fling_timer_id == *(uint8*)param)
    {
        if (MMK_TP_SLIDE_FLING == slide_calendar_ptr->slide_state)
        {
            int16 first_x=0;
            int16 first_y=0;
            StopSlideFlingTimer(slide_calendar_ptr);
            UILAYER_GetLayerPosition(&s_first_lcd_dev_info,&first_x,&first_y);
            //old_pos = slide_calendar_ptr->slide_pre_point.x;
            old_pos=first_y;

            // 重算加速度
            cur_pos = MMK_GetFlingOffset(
                    old_pos, slide_calendar_ptr->velocity,
                    TOUCH_FLING_TIME, &slide_calendar_ptr->velocity);

           // slide_calendar_ptr->slide_pre_point.x=cur_pos;

            if (FLING_RUN_MIN_VELOCITY <= abs((int32)slide_calendar_ptr->velocity)) // 还可以加速运动
            {
                uint8 update_result=UpdateLayer(slide_calendar_ptr, cur_pos-old_pos);
                if(0==update_result)
                {
                    StartSlideFlingTimer(slide_calendar_ptr);
                }
                else
                {
                    if((2==update_result)
                        && MMICALENDAR_IsDateValid(slide_calendar_ptr->calendar_ctrl_ptr->solar_year,slide_calendar_ptr->calendar_ctrl_ptr->solar_month,slide_calendar_ptr->calendar_ctrl_ptr->solar_date))
                    {
                        SCI_MEMCPY(&g_calendar_show,slide_calendar_ptr->calendar_ctrl_ptr,sizeof(MMI_CALENDAR_T));
                    }
                    ExitSlidecalendar(slide_calendar_ptr,TRUE);
                }
            }
            else    // 小于最小加速度，进入恢复状态
            {
                slide_calendar_ptr->slide_state = MMK_TP_SLIDE_RECOVER;
                UpdateMoveDirection(slide_calendar_ptr);
                StartSlideRecoverTimer(slide_calendar_ptr);
            }
        }
    }
    else if(slide_calendar_ptr->recover_timer_id == *(uint8*)param)
    {
        if (MMK_TP_SLIDE_RECOVER== slide_calendar_ptr->slide_state)
        {
            uint8 update_result=0;
            int16 first_x=0;
            int16 first_y=0;
            StopSlideRecoverTimer(slide_calendar_ptr);
            UILAYER_GetLayerPosition(&s_first_lcd_dev_info,&first_x,&first_y);
            old_pos=first_y;

            if(slide_calendar_ptr->direction==MOVE_UP)
            {
                cur_pos =old_pos-SLIDE_RECOVER_STEP ;
            }
            else
            {
                cur_pos =old_pos+SLIDE_RECOVER_STEP ;
            }
            
            update_result=UpdateLayer(slide_calendar_ptr, cur_pos-old_pos);
            if(0==update_result)
            {
                StartSlideRecoverTimer(slide_calendar_ptr);
            }
            else
            {
                if((2==update_result) 
                    && MMICALENDAR_IsDateValid(slide_calendar_ptr->calendar_ctrl_ptr->solar_year,slide_calendar_ptr->calendar_ctrl_ptr->solar_month,slide_calendar_ptr->calendar_ctrl_ptr->solar_date))
                {
                    SCI_MEMCPY(&g_calendar_show,slide_calendar_ptr->calendar_ctrl_ptr,sizeof(MMI_CALENDAR_T));
                }
                ExitSlidecalendar(slide_calendar_ptr,TRUE);
            }

        }
    }
    else
    {
        result = MMI_RESULT_FALSE;
    }

    return result;
}
/*****************************************************************************/
// Description : StartSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StartSlideRedrawTimer( MMI_SLIDE_CALENDAR_T *slide_calendar_ptr)
{
    if (PNULL != slide_calendar_ptr)
    {
        if(0 != slide_calendar_ptr->redraw_timer_id)
        {
            StopSlideRedrawTimer(slide_calendar_ptr);
        }
        slide_calendar_ptr->redraw_timer_id = MMK_CreateWinTimer(
            slide_calendar_ptr->handle, 
            MMICALENDAR_SLIDE_REDRAW_TIME,
            FALSE );
    }
}

/*****************************************************************************/
// Description : StopSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StopSlideRedrawTimer( MMI_SLIDE_CALENDAR_T *slide_calendar_ptr)
{
    if (PNULL != slide_calendar_ptr && 0 != slide_calendar_ptr->redraw_timer_id)
    {
        MMK_StopTimer(slide_calendar_ptr->redraw_timer_id);
        slide_calendar_ptr->redraw_timer_id = 0;
    }
}

/*****************************************************************************/
// Description : StartSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StartSlideFlingTimer( MMI_SLIDE_CALENDAR_T *slide_calendar_ptr)
{
    if (PNULL != slide_calendar_ptr)
    {
        if(0 != slide_calendar_ptr->fling_timer_id)
        {
            StopSlideFlingTimer(slide_calendar_ptr);
        }
        slide_calendar_ptr->fling_timer_id = MMK_CreateWinTimer(
            slide_calendar_ptr->handle, 
            MMICALENDAR_SLIDE_FLING_TIME,
            FALSE );
    }
}

/*****************************************************************************/
// Description : StopSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StopSlideFlingTimer( MMI_SLIDE_CALENDAR_T *slide_calendar_ptr)
{
    if (PNULL != slide_calendar_ptr && 0 != slide_calendar_ptr->fling_timer_id)
    {
        MMK_StopTimer(slide_calendar_ptr->fling_timer_id);
        slide_calendar_ptr->fling_timer_id = 0;
    }
}

/*****************************************************************************/
// Description : StartSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StartSlideRecoverTimer( MMI_SLIDE_CALENDAR_T *slide_calendar_ptr)
{
    if (PNULL != slide_calendar_ptr)
    {
        if(0 != slide_calendar_ptr->recover_timer_id)
        {
            StopSlideRecoverTimer(slide_calendar_ptr);
        }
        slide_calendar_ptr->recover_timer_id = MMK_CreateWinTimer(
            slide_calendar_ptr->handle, 
            MMICALENDAR_SLIDE_FLING_TIME,
            FALSE );
    }
}

/*****************************************************************************/
// Description : StopSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StopSlideRecoverTimer( MMI_SLIDE_CALENDAR_T *slide_calendar_ptr)
{
    if (PNULL != slide_calendar_ptr && 0 != slide_calendar_ptr->recover_timer_id)
    {
        MMK_StopTimer(slide_calendar_ptr->recover_timer_id);
        slide_calendar_ptr->recover_timer_id = 0;
    }
}

/*****************************************************************************/
// Description : InitSlidecalendar
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void InitSlidecalendar( MMI_SLIDE_CALENDAR_T *slide_calendar_ptr,
                                MMI_HANDLE_T    handle)
{
    UILAYER_APPEND_BLT_T append_layer={0};
    IGUICTRL_T    *stb_ctr_ptr  = PNULL;
    IGUICTRL_T    *title_ctr_ptr  = PNULL;
#ifndef MMI_PDA_SUPPORT
    IGUICTRL_T    *sk_ctr_ptr  = PNULL;
    //uint16 logic_lcd_width = 0;
    //uint16 logic_lcd_height = 0;
    GUI_RECT_T client_rect= MMITHEME_GetClientRectEx(handle);
    //GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &logic_lcd_width, &logic_lcd_height);
#endif

    
    //SCI_MEMSET(slide_calendar_ptr,0,sizeof(MMI_SLIDE_CALENDAR_T));
    slide_calendar_ptr->handle=handle;
    append_layer.layer_level=UILAYER_LEVEL_NORMAL;
    append_layer.lcd_dev_info=s_second_lcd_dev_info;
    UILAYER_AppendBltLayer(&append_layer);
    append_layer.lcd_dev_info=s_first_lcd_dev_info;
    UILAYER_AppendBltLayer(&append_layer);
    append_layer.lcd_dev_info=s_statusbar_dev_info;
    UILAYER_AppendBltLayer(&append_layer);
    UILAYER_SetLayerPosition(&s_statusbar_dev_info,0,0);
#ifndef MMI_PDA_SUPPORT
    append_layer.lcd_dev_info=s_softkey_dev_info;
    UILAYER_AppendBltLayer(&append_layer);
    UILAYER_SetLayerPosition(&s_softkey_dev_info,0,client_rect.bottom+1);
#endif

    UILAYER_Clear(&s_first_lcd_dev_info);
    UILAYER_Clear(&s_second_lcd_dev_info);
    SetCurLcdInfo(&s_calendar_lcd_dev_info);
    
    stb_ctr_ptr  = (IGUICTRL_T*)MMK_GetCtrlPtrByWin(slide_calendar_ptr->handle, MMITHEME_GetStatusBarCtrlId());
    if(UILAYER_NULL_HANDLE!=s_statusbar_dev_info.block_id)
    {
        GUICTRL_SetLcdDevInfo(stb_ctr_ptr, &s_statusbar_dev_info); 
        GUISTATUSBAR_UpdateCtrl(slide_calendar_ptr->handle,FALSE);
    }
#if defined(MMI_ISTYLE_SUPPORT)	
    if(MMITHEME_IsIstyle())
    {
        title_ctr_ptr  = (IGUICTRL_T*)MMK_GetCtrlPtrByWin(slide_calendar_ptr->handle, MMITHEME_GetTitleCtrlId());
        if(UILAYER_NULL_HANDLE!=s_statusbar_dev_info.block_id)
        {
            GUICTRL_SetLcdDevInfo(title_ctr_ptr, &s_statusbar_dev_info); 
            GUITITLE_Update(slide_calendar_ptr->handle,MMITHEME_GetTitleCtrlId());
        } 
    }
#endif	
#ifndef MMI_PDA_SUPPORT
    sk_ctr_ptr  = (IGUICTRL_T*)MMK_GetCtrlPtrByWin(slide_calendar_ptr->handle, MMITHEME_GetSoftkeyCtrlId());
    if(UILAYER_NULL_HANDLE!=s_softkey_dev_info.block_id)
    {
        GUICTRL_SetLcdDevInfo(sk_ctr_ptr, &s_softkey_dev_info); 
        GUISOFTKEY_Update(slide_calendar_ptr->handle,MMITHEME_GetSoftkeyCtrlId());
    }
#endif
    if(PNULL!=slide_calendar_ptr->calendar_ctrl_ptr)
    {
        SCI_FREE(slide_calendar_ptr->calendar_ctrl_ptr);
        slide_calendar_ptr->calendar_ctrl_ptr=PNULL;
    }
    slide_calendar_ptr->calendar_ctrl_ptr=SCI_ALLOC_APP(sizeof(MMI_CALENDAR_T));
    SCI_MEMSET(slide_calendar_ptr->calendar_ctrl_ptr,0,sizeof(MMI_CALENDAR_T));
}

/*****************************************************************************/
// Description : set common character
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void ExitSlidecalendar( MMI_SLIDE_CALENDAR_T *slide_calendar_ptr,BOOLEAN is_update_calendar)
{
    IGUICTRL_T    *stb_ctr_ptr  = (IGUICTRL_T*)MMK_GetCtrlPtrByWin(slide_calendar_ptr->handle, MMITHEME_GetStatusBarCtrlId()); 
    IGUICTRL_T     *title_ctr_ptr  = (IGUICTRL_T*)MMK_GetCtrlPtrByWin(slide_calendar_ptr->handle, MMITHEME_GetTitleCtrlId());
#ifndef MMI_PDA_SUPPORT
    IGUICTRL_T    *sk_ctr_ptr  = (IGUICTRL_T*)MMK_GetCtrlPtrByWin(slide_calendar_ptr->handle, MMITHEME_GetSoftkeyCtrlId());
#endif
   
    SetCurLcdInfo(&s_calendar_lcd_dev_info);
    UILAYER_RemoveBltLayer(&s_first_lcd_dev_info);
    UILAYER_RemoveBltLayer(&s_second_lcd_dev_info);
    UILAYER_RemoveBltLayer(&s_statusbar_dev_info);
#ifndef MMI_PDA_SUPPORT
    UILAYER_RemoveBltLayer(&s_softkey_dev_info);
#endif

    SetCurLcdInfo(&s_calendar_lcd_dev_info);
    if(is_update_calendar)
    {
        if(slide_calendar_ptr->view_type==GUI_CALENDAR_MONTH_VIEW)
        {
            DrawMonthCalendar(&g_calendar_show, TRUE);
        }
        else
        {
            DrawWeekCalendar(&g_calendar_show, TRUE);
        }
    }
    if(UILAYER_NULL_HANDLE!=s_calendar_lcd_dev_info.block_id)
    {
        GUICTRL_SetLcdDevInfo(stb_ctr_ptr, &s_calendar_lcd_dev_info); 
        GUISTATUSBAR_UpdateCtrl(slide_calendar_ptr->handle,FALSE);
    }
#if defined(MMI_ISTYLE_SUPPORT)	
    if(MMITHEME_IsIstyle())
    {
        if(UILAYER_NULL_HANDLE!=s_calendar_lcd_dev_info.block_id)
        {
            GUICTRL_SetLcdDevInfo(title_ctr_ptr, &s_calendar_lcd_dev_info); 
            GUITITLE_Update(slide_calendar_ptr->handle,MMITHEME_GetTitleCtrlId());
        } 
    }
#endif	
#ifndef MMI_PDA_SUPPORT
    if(UILAYER_NULL_HANDLE!=s_calendar_lcd_dev_info.block_id)
    {
        GUICTRL_SetLcdDevInfo(sk_ctr_ptr, &s_calendar_lcd_dev_info); 
        GUISOFTKEY_Update(slide_calendar_ptr->handle,MMITHEME_GetSoftkeyCtrlId());
    }
#endif
    //GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
    if(PNULL!=slide_calendar_ptr->calendar_ctrl_ptr)
    {
        SCI_FREE(slide_calendar_ptr->calendar_ctrl_ptr);
        slide_calendar_ptr->calendar_ctrl_ptr=PNULL;
    }
    StopSlideFlingTimer(slide_calendar_ptr);
    StopSlideRecoverTimer(slide_calendar_ptr);
    StopSlideRedrawTimer(slide_calendar_ptr);
    SCI_MEMSET(slide_calendar_ptr,0,sizeof(MMI_SLIDE_CALENDAR_T));
    MMK_ResetVelocityItem();
}
#endif

#ifdef MMI_CALENDAR_PLAY_EFFECT
/*****************************************************************************/
// Description : playCalendareffect
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN playCalendareffect(CALENDAR_MOVE_MODE_E move_mode,CALENDAR_VIEW_TYPE_E view_type)
{
    int i=0;
    MMI_CALENDAR_T *calendar_ctrl_ptr=PNULL;
    UILAYER_APPEND_BLT_T append_layer={0};
    uint16 logic_lcd_width=0;
    uint16 logic_lcd_height=0;
    uint16 step=0;
    BOOLEAN is_move_down=FALSE;
    GUI_BOTH_RECT_T     full_both_rect = MMITHEME_GetFullScreenBothRect();
    IGUICTRL_T    *stb_ctr_ptr  = PNULL;
    IGUICTRL_T    *title_ctr_ptr  = PNULL; 
    GUI_RECT_T client_rect= MMITHEME_GetClientRectEx(MMICALENDAR_WIN_ID);
#ifndef MMI_PDA_SUPPORT
    IGUICTRL_T    *sk_ctr_ptr  = PNULL;
    GUI_RECT_T   sk_rect={0};
    uint16 lcd_width=0;
    uint16 lcd_height=0;
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    sk_rect.left=0;
    sk_rect.right=lcd_width-1;
    sk_rect.top=client_rect.bottom+1;
    sk_rect.bottom=lcd_height-1;
#endif

    calendar_ctrl_ptr=(MMI_CALENDAR_T *)SCI_ALLOC_APP(sizeof(MMI_CALENDAR_T));
    if(PNULL==calendar_ctrl_ptr)
    {
        return FALSE;
    }
    if(move_mode==MOVE_PREV_DAY || move_mode==MOVE_NEXT_DAY || move_mode>MOVE_NEXT_YEAR)
    {
        SCI_FREE(calendar_ctrl_ptr);
        return FALSE;
    }
    
    append_layer.layer_level=UILAYER_LEVEL_NORMAL;
    append_layer.lcd_dev_info=s_second_lcd_dev_info;
    UILAYER_AppendBltLayer(&append_layer);
    append_layer.lcd_dev_info=s_first_lcd_dev_info;
    UILAYER_AppendBltLayer(&append_layer);
    append_layer.lcd_dev_info=s_statusbar_dev_info;
    UILAYER_AppendBltLayer(&append_layer);
    UILAYER_Clear(&s_first_lcd_dev_info);
    UILAYER_Clear(&s_second_lcd_dev_info);
    UILAYER_SetLayerPosition(&s_first_lcd_dev_info, 0, client_rect.top+1);
    UILAYER_SetLayerPosition(&s_second_lcd_dev_info, 0, client_rect.top+1);
    UILAYER_SetLayerPosition(&s_statusbar_dev_info, 0, 0);
#ifndef MMI_PDA_SUPPORT
    LCD_FillRect(&s_first_lcd_dev_info,sk_rect,MMI_CALENDAR_BG_COLOR);
    LCD_FillRect(&s_second_lcd_dev_info,sk_rect,MMI_CALENDAR_BG_COLOR);
#endif
    stb_ctr_ptr  = (IGUICTRL_T*)MMK_GetCtrlPtrByWin(MMICALENDAR_WIN_ID, MMITHEME_GetStatusBarCtrlId());
    if(UILAYER_NULL_HANDLE!=s_statusbar_dev_info.block_id)
    {
        GUICTRL_SetLcdDevInfo(stb_ctr_ptr, &s_statusbar_dev_info); 
        GUISTATUSBAR_UpdateCtrl(MMICALENDAR_WIN_ID,FALSE);
    }
#if defined(MMI_ISTYLE_SUPPORT)	
    if(MMITHEME_IsIstyle())
    {
        title_ctr_ptr  = (IGUICTRL_T*)MMK_GetCtrlPtrByWin(MMICALENDAR_WIN_ID, MMITHEME_GetTitleCtrlId());
        if(UILAYER_NULL_HANDLE!=s_statusbar_dev_info.block_id)
        {
            GUICTRL_SetLcdDevInfo(title_ctr_ptr, &s_statusbar_dev_info); 
            GUITITLE_Update(MMICALENDAR_WIN_ID,MMITHEME_GetTitleCtrlId());
        } 
    }
#endif	
#ifndef MMI_PDA_SUPPORT
    append_layer.lcd_dev_info=s_softkey_dev_info;
    UILAYER_AppendBltLayer(&append_layer);
    UILAYER_SetLayerPosition(&s_softkey_dev_info,0,client_rect.bottom+1);
    sk_ctr_ptr  = (IGUICTRL_T*)MMK_GetCtrlPtrByWin(MMICALENDAR_WIN_ID, MMITHEME_GetSoftkeyCtrlId());
    if(UILAYER_NULL_HANDLE!=s_softkey_dev_info.block_id)
    {
        GUICTRL_SetLcdDevInfo(sk_ctr_ptr, &s_softkey_dev_info); 
        GUISOFTKEY_Update(MMICALENDAR_WIN_ID,MMITHEME_GetSoftkeyCtrlId());
    }
#endif


    SetCurLcdInfo(&s_first_lcd_dev_info);
    if(GUI_CALENDAR_MONTH_VIEW==view_type)
    {
        DrawMonthCalendar(&g_calendar_show, TRUE);
    }
    else
    {
        DrawWeekCalendar(&g_calendar_show, TRUE);
    }
    
    SetCurLcdInfo(&s_second_lcd_dev_info);
    SCI_MEMCPY(calendar_ctrl_ptr,&g_calendar_show,sizeof(MMI_CALENDAR_T));
    HandleCalendarKey(calendar_ctrl_ptr, move_mode);
    if(GUI_CALENDAR_MONTH_VIEW==view_type)
    {
        DrawMonthCalendar(calendar_ctrl_ptr, TRUE);
    }
    else
    {
        DrawWeekCalendar(calendar_ctrl_ptr, TRUE);
    }

    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &logic_lcd_width, &logic_lcd_height);
    step=(logic_lcd_height-client_rect.top-1)/MMI_CALENDAR_PLAY_FRAME_NUM;
    if(move_mode==MOVE_PREV_WEEK
        ||move_mode==MOVE_PREV_MONTH
        ||move_mode==MOVE_PREV_YEAR)
    {
        is_move_down=TRUE;
    }
    else
    {
        is_move_down=FALSE;
    }

    for(i=0;i<MMI_CALENDAR_PLAY_FRAME_NUM;i++)
    {
        if(i==(MMI_CALENDAR_PLAY_FRAME_NUM-1))
        {
            UILAYER_SetLayerPosition(&s_first_lcd_dev_info, 0, logic_lcd_height);
            UILAYER_SetLayerPosition(&s_second_lcd_dev_info,0, client_rect.top+1);
        }
        
        if(is_move_down)
        {
            UILAYER_SetLayerPosition(&s_first_lcd_dev_info,0, client_rect.top+1+step*(i+1));
            UILAYER_SetLayerPosition(&s_second_lcd_dev_info,0,2*(client_rect.top+1)-logic_lcd_height+step*(i+1));
        }
        else
        {
            UILAYER_SetLayerPosition(&s_first_lcd_dev_info, 0,client_rect.top+1-step*(i+1));
            UILAYER_SetLayerPosition(&s_second_lcd_dev_info, 0,logic_lcd_height-step*(i+1));
        }
        
        if(MMITHEME_IsMainScreenLandscape())
        {
            GUILCD_InvalidateRect(GUI_MAIN_LCD_ID, full_both_rect.h_rect,0);
        }
        else
        {
            GUILCD_InvalidateRect(GUI_MAIN_LCD_ID, full_both_rect.v_rect,0);
        }
#ifdef WIN32
        SCI_Sleep(80);
#else
        SCI_Sleep(30);
#endif
    }

    UILAYER_RemoveBltLayer(&s_first_lcd_dev_info);
    UILAYER_RemoveBltLayer(&s_second_lcd_dev_info);
    UILAYER_RemoveBltLayer(&s_statusbar_dev_info);
#ifndef MMI_PDA_SUPPORT
    UILAYER_RemoveBltLayer(&s_softkey_dev_info);
#endif
    SetCurLcdInfo(&s_calendar_lcd_dev_info);
    if(UILAYER_NULL_HANDLE!=s_calendar_lcd_dev_info.block_id)
    {
        GUICTRL_SetLcdDevInfo(stb_ctr_ptr, &s_calendar_lcd_dev_info); 
        GUISTATUSBAR_UpdateCtrl(g_calendar_show.win_id,FALSE);
    }
#if defined(MMI_ISTYLE_SUPPORT)	
    if(MMITHEME_IsIstyle())
    {
        if(UILAYER_NULL_HANDLE!=s_calendar_lcd_dev_info.block_id)
        {
            GUICTRL_SetLcdDevInfo(title_ctr_ptr, &s_calendar_lcd_dev_info); 
            GUITITLE_Update(g_calendar_show.win_id,MMITHEME_GetTitleCtrlId());
        } 
    }
#endif	
#ifndef MMI_PDA_SUPPORT
    if(UILAYER_NULL_HANDLE!=s_calendar_lcd_dev_info.block_id)
    {
        GUICTRL_SetLcdDevInfo(sk_ctr_ptr, &s_calendar_lcd_dev_info); 
        GUISOFTKEY_Update(g_calendar_show.win_id,MMITHEME_GetSoftkeyCtrlId());
    }
#endif
    SCI_FREE(calendar_ctrl_ptr);
    
    return TRUE;
}
#endif

#ifdef MMI_CALENDAR_DAILY_SUPPORT
/*****************************************************************************/
//    Description : 
//    Global resource dependence : 
//    Author: liqing.peng
//    Note:
/*****************************************************************************/
LOCAL int GetCurHourDisSchduleText(wchar*text,uint16 *text_len,uint16 text_max_len,uint8 hour)
{
    MMIACC_SCH_LIST_T*  user_list=MMIAPISCH_GetUserList();
    SCI_TIME_T              cur_time = {0};
    SCI_TIME_T              sch_time = {0};
    SCI_TIME_T              dis_time = {0};
    uint16 cur_index=0;
    BOOLEAN is_find=FALSE;
    uint8  item_str[MMIALM_TIME_STR_12HOURS_LEN + 1] = {0};
    wchar  sch_str[MMIALM_TIME_STR_12HOURS_LEN+MMIACC_EVENT_TEXT_MAX_LEN+5]={0};
    wchar space[2]={0x20,0};
    MMIACC_SMART_EVENT_T event={0};
    int i=0;
    MMIACC_SCH_LIST_T cur_hour_list={0};
    int result=0;
    MMI_STRING_T str={0};
    uint16 len=0;
    
    if(user_list->total==0)
    {
        return result;
    }
    else
    {
        TM_GetSysTime(&cur_time);
        dis_time.hour=hour;
        for(i=0;i<user_list->total;i++)
        {
            MMIAPIALM_FormInteEventInfo(user_list->entry_id[i],&event);
            if(hour==event.event_fast_info.hour)
            {
                cur_hour_list.entry_id[cur_hour_list.total]=user_list->entry_id[i];
                cur_hour_list.total++;
            }
        }
        if(cur_hour_list.total==0)
        {
           return result; 
        }
        else if(hour<cur_time.hour)
        {
           cur_index=cur_hour_list.total-1; 
           result=2;
        }
        else if(hour>cur_time.hour)
        {
            cur_index=0;
            result=2;
        }
        else
        {
            for(i=0;i<cur_hour_list.total;i++)
            {
                MMIAPIALM_FormInteEventInfo(cur_hour_list.entry_id[i],&event);
                sch_time.hour=event.event_fast_info.hour;
                sch_time.min=event.event_fast_info.minute;
                sch_time.sec=cur_time.sec;
                if(MMIAPISCH_CompareTime(sch_time, cur_time)==CMP_SECOND_SMALL
                    ||MMIAPISCH_CompareTime(sch_time, cur_time)==CMP_EQUAL)
                {
                    cur_index=i;
                    is_find=TRUE;
                    break;
                }
            }
            if(!is_find)
            {
                cur_index=cur_hour_list.total-1;//CR179461
            }
            result=1;
        }
        SCI_MEMSET(&event,0,sizeof(MMIACC_SMART_EVENT_T));
        MMIAPIALM_FormInteEventInfo(cur_hour_list.entry_id[cur_index],&event);
        MMIAPISET_FormatTimeStrByTime(event.event_fast_info.hour,event.event_fast_info.minute,item_str,MMIALM_TIME_STR_12HOURS_LEN + 1);
        MMI_STRNTOWSTR( sch_str,
                 MMIALM_TIME_STR_12HOURS_LEN+1,
                item_str,
                strlen((char*)item_str),
                strlen((char*)item_str));
        MMIAPICOM_Wstrcat(sch_str,space);
        MMIAPICOM_Wstrcat(sch_str,space);
        if(event.event_content_info.w_length>0)
        {
            MMIAPICOM_Wstrcat(sch_str,event.event_content_info.w_content);
        }
        else
        {
            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_BLANK, &str);
            MMIAPICOM_Wstrncpy(sch_str+MMIAPICOM_Wstrlen(sch_str), str.wstr_ptr,str.wstr_len);
        }

        len=MMIAPICOM_Wstrlen(sch_str);
        *text_len=text_max_len>len ? len:text_max_len;
            
        MMIAPICOM_Wstrncpy(text, sch_str,*text_len);
    }

    return result;
}
#endif
#ifdef MMI_CALENDAR_DAILY_SUPPORT
/*****************************************************************************/
//    Description : d
//    Global resource dependence : 
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN IsInDailyItemRange(int16 y)
{
    int16 start=0;
    int16 end=0;
    int16 isstyle_start_y=0;
    int16 isstyle_item_h=0;
    if(!MMITHEME_IsMainScreenLandscape ())
    {
#if defined(MMI_ISTYLE_SUPPORT)    
        if(MMITHEME_IsIstyle())
        {
            isstyle_start_y=MMITHEME_GetWinTitleHeight()+CALENDAR_ISSTYLE_DAILY_START_Y_OFFSET_V;
            isstyle_item_h=CALENDAR_ISSTYLE_DAILY_ITEM_HEIGH_V;
        }
        else
#endif			
        {
            isstyle_start_y=CALENDAR_DAILY_START_Y_OFFSET_V;
            isstyle_item_h=CALENDAR_DAILY_ITEM_HEIGH_V;
        }
        start=isstyle_start_y;
        end=isstyle_start_y+isstyle_item_h*CALENDAR_DAILY_MAX_ITEM_NUM_V;
    }
    else
    {
#if defined(MMI_ISTYLE_SUPPORT)    
        if(MMITHEME_IsIstyle())
        {
            isstyle_start_y=MMITHEME_GetWinTitleHeight()+CALENDAR_ISSTYLE_DAILY_START_Y_OFFSET_H;
            isstyle_item_h=CALENDAR_ISSTYLE_DAILY_ITEM_HEIGH_H;
        }
        else
#endif			
        {
            isstyle_start_y=CALENDAR_DAILY_START_Y_OFFSET_H;
            isstyle_item_h=CALENDAR_DAILY_ITEM_HEIGH_H;
        }
        start=isstyle_start_y;
        end=isstyle_start_y+isstyle_item_h*CALENDAR_DAILY_MAX_ITEM_NUM_H;
    }

    if(y>=start && y<=end)
    {
        return TRUE;
    }

    return FALSE;
}
#endif
#ifdef MMI_CALENDAR_DAILY_SUPPORT
/*****************************************************************************/
//    Description : d
//    Global resource dependence : 
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL void DrawDailyItem(
                             MMI_DAILY_CALENDAR_T* daily_calendar
                             )
{
    int i=0;
    GUI_COLOR_T line_color = GUI_RGB2RGB565(212, 212, 212);
    int16 y=0;
    uint16 lcd_width=0;
    uint16 lcd_height=0;
    uint16 item_h=0;
    GUI_RECT_T time_rect={0};
    int16 time_right=0;
    GUI_RECT_T item_rect={0};
    GUI_RECT_T single_item_rect={0};
    GUISTR_STYLE_T              text_style={0};
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS; 
    uint8  item_str[MMIALM_TIME_STR_12HOURS_LEN + 1] = {0};
    wchar  time_str[MMIALM_TIME_STR_12HOURS_LEN+1]={0};
    MMI_STRING_T str={0};
    wchar  sch_str[MMIALM_TIME_STR_12HOURS_LEN+MMIACC_EVENT_TEXT_MAX_LEN+5]={0};
    uint16 sch_str_len=0;
    int result=0;
    GUI_RECT_T cross_rect={0};
    GUI_RECT_T sch_rect={0};
#ifdef MMI_CALENDAR_KEY_STYLE
    int16 focus_y=0;
#endif

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    item_rect.left=daily_calendar->item_rect.left;
    item_rect.top=daily_calendar->item_rect.top;
    item_rect.right=daily_calendar->item_rect.right;
    item_rect.bottom=daily_calendar->item_rect.bottom;
    item_h=daily_calendar->item_height;
    
    if(!MMITHEME_IsMainScreenLandscape ())
    {
        time_right=CALENDAR_DAILY_TIME_RIGHT_V;
        sch_rect.left=CALENDAR_DAILY_TIME_RIGHT_V+5;
        sch_rect.right=lcd_width-1;
    }
    else
    {
        time_right=CALENDAR_DAILY_TIME_RIGHT_H;
        sch_rect.left=CALENDAR_DAILY_TIME_RIGHT_H+5;
        sch_rect.right=lcd_width-1;
    }

    LCD_DrawHLine(daily_calendar->dev_info,0,daily_calendar->item_rect.bottom+1,lcd_width-1,MMI_CALENDAR_BG_COLOR);
 #ifdef MMI_CALENDAR_KEY_STYLE
    focus_y=daily_calendar->cur_offset_y+daily_calendar->focus_index*item_h;
    if(focus_y<daily_calendar->item_rect.top)
    {
        daily_calendar->cur_offset_y=daily_calendar->cur_offset_y+(daily_calendar->item_rect.top-focus_y);
    }
    focus_y=daily_calendar->cur_offset_y+(daily_calendar->focus_index+1)*item_h;
    if(focus_y>daily_calendar->item_rect.bottom)
    {
        daily_calendar->cur_offset_y=daily_calendar->cur_offset_y-(focus_y-daily_calendar->item_rect.bottom-1);
    }
 #endif

    for(i=0;i<24;i++)
    {
        single_item_rect.left=0;
        single_item_rect.right=lcd_width-1;
        single_item_rect.top=daily_calendar->cur_offset_y+i*item_h+1;
        single_item_rect.bottom=single_item_rect.top+item_h-2;

        sch_rect.top=single_item_rect.top;
        sch_rect.bottom=single_item_rect.bottom;

        result=GetCurHourDisSchduleText(sch_str,&sch_str_len,MMIALM_TIME_STR_12HOURS_LEN+MMIACC_EVENT_TEXT_MAX_LEN+5,i);
        if(GUI_IntersectRect(&cross_rect,single_item_rect,item_rect))
        {
            if(0==result)//没有日程
            {
               LCD_FillRect(daily_calendar->dev_info,cross_rect,MMI_CALENDAR_BG_COLOR); 
            }
            else if(1==result)//当前时间
            {
                GUIRES_DisplayImg(PNULL,
					&cross_rect,
					PNULL,
					daily_calendar->win_id,
					IMAGE_CLOCK_CALENDAR_DEFAULT,
					daily_calendar->dev_info);
            }
            else if(2==result)
            {
                GUIRES_DisplayImg(PNULL,
					&cross_rect,
					PNULL,
					daily_calendar->win_id,
					IMAGE_CLOCK_CALENDAR_ORANGE_DEFAULT,
					daily_calendar->dev_info);
            }
 #ifdef MMI_CALENDAR_KEY_STYLE
            if(daily_calendar->focus_index==i)
            {
                GUIRES_DisplayImg(PNULL,
					&cross_rect,
					PNULL,
					daily_calendar->win_id,
					IMAGE_CLOCK_CALENDAR_GREEN_DEFAULT,
					daily_calendar->dev_info);
            }
 #endif
            if(0!=result)
            {
                str.wstr_ptr=sch_str;
                str.wstr_len=sch_str_len;
                SetCalendarTextStyle(&text_style, ALIGN_LVMIDDLE, CALENDAR_DATE_FONT, MMI_WHITE_COLOR);
                GUISTR_DrawTextToLCDInRect( 
                        (const GUI_LCD_DEV_INFO *)daily_calendar->dev_info,
                        (const GUI_RECT_T      *)&sch_rect,
                        (const GUI_RECT_T      *)&item_rect,
                        (const MMI_STRING_T    *)&str,
                        &text_style,
                        state,
                        GUISTR_TEXT_DIR_AUTO);
            }
        }
        
        y=daily_calendar->cur_offset_y+i*item_h;
        if(IsInDailyItemRange(y))
        {
            LCD_DrawHLine(daily_calendar->dev_info,0,y,lcd_width,line_color); 
        }

        time_rect.left=0;
        time_rect.right=time_right;
        time_rect.top=y;
        time_rect.bottom=time_rect.top+item_h-1;
        if(IsInDailyItemRange(time_rect.bottom)||IsInDailyItemRange(time_rect.top))
        {
            MMIAPISET_FormatTimeStrByTime(i,0,item_str,MMIALM_TIME_STR_12HOURS_LEN + 1);
            MMI_STRNTOWSTR( time_str,
                    MMIALM_TIME_STR_12HOURS_LEN+1,
                    item_str,
                    strlen((char*)item_str),
                    strlen((char*)item_str));

            str.wstr_ptr=time_str;
            str.wstr_len=MMIAPICOM_Wstrlen(time_str);

            SetCalendarTextStyle(&text_style, ALIGN_HVMIDDLE, CALENDAR_DATE_FONT, MMI_BLACK_COLOR);
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)daily_calendar->dev_info,
                (const GUI_RECT_T      *)&time_rect,
                (const GUI_RECT_T      *)&item_rect,
                (const MMI_STRING_T    *)&str,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO);
        } 
    }

    y=daily_calendar->cur_offset_y+24*item_h;
    if(IsInDailyItemRange(y))
    {
        LCD_DrawHLine(daily_calendar->dev_info,0,y,lcd_width,line_color); 
    }

    LCD_DrawVLine(daily_calendar->dev_info,time_right,item_rect.top,item_rect.bottom,line_color);
    
}

/*****************************************************************************/
//    Description : d
//    Global resource dependence : 
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL void DrawDailyTitle(
                             MMI_DAILY_CALENDAR_T* daily_calendar
                             )
{
    uint8						datestr[MMISET_DATE_STR_LEN] = {0};
    uint16						len = {0}; 
    wchar                       title[MMISET_DATE_STR_LEN]={0};
    GUISTR_STYLE_T              text_style={0};
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    MMI_STRING_T string={0};
    GUI_RECT_T rect={0};
    GUI_RECT_T rect_v=CALENDAR_DAILY_TITLE_RECT_V;
    GUI_RECT_T rect_h=CALENDAR_DAILY_TITLE_RECT_H;
    
    //统一title风格
#ifdef MMI_GUI_STYLE_MINISCREEN
    GUI_COLOR_T  daily_color =RGB888TO565(MMI_CALENDAR_MONTHLY_TITLE_COLOR);
#else
    GUI_COLOR_T  daily_color =MMI_BLACK_COLOR;
#endif

    SetCalendarTextStyle(&text_style, ALIGN_HVMIDDLE, CALENDAR_WEEKLY_TITLE_FONT, daily_color);
    MMIAPISET_FormatDateStrByDateStyle(daily_calendar->date.year,daily_calendar->date.month,daily_calendar->date.day,'-',datestr,MMISET_DATE_STR_LEN);
    len = strlen((char *)datestr);
    MMI_STRNTOWSTR(title, MMISET_DATE_STR_LEN,datestr, len,len);

    string.wstr_ptr=title;
    string.wstr_len=len;

    if(!MMITHEME_IsMainScreenLandscape ())
    {
        rect = rect_v;
#if defined(MMI_ISTYLE_SUPPORT)		
        if(MMITHEME_IsIstyle())
        {
            rect.top+=MMITHEME_GetWinTitleHeight();
            rect.bottom+=MMITHEME_GetWinTitleHeight()-(CALENDAR_DAILY_START_Y_OFFSET_V-CALENDAR_ISSTYLE_DAILY_START_Y_OFFSET_V);
        }
#endif		
    }
    else
    {
        rect = rect_h;
#if defined(MMI_ISTYLE_SUPPORT)		
        if(MMITHEME_IsIstyle())
        {
            rect.top+=MMITHEME_GetWinTitleHeight();
            rect.bottom+=MMITHEME_GetWinTitleHeight()-(CALENDAR_DAILY_START_Y_OFFSET_H-CALENDAR_ISSTYLE_DAILY_START_Y_OFFSET_H);
        }
#endif		
    }

    GUISTR_DrawTextToLCDInRect( 
            daily_calendar->dev_info,
            (const GUI_RECT_T      *)&rect,      
            (const GUI_RECT_T      *)&rect,       
            (const MMI_STRING_T    *)&string,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            ); 
}

/*****************************************************************************/
//    Description : 
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
LOCAL void DrawDailyCalendar(
                             MMI_DAILY_CALENDAR_T* daily_calendar
                             )
{
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
    GUI_RECT_T client_rect= MMITHEME_GetClientRectEx(daily_calendar->win_id);
    rect.top=client_rect.top;
    LCD_FillRect(daily_calendar->dev_info,rect,MMI_CALENDAR_BG_COLOR);
    DrawDailyTitle(daily_calendar);
#ifndef MMI_CALENDAR_KEY_STYLE
    if(!MMITHEME_IsIstyle())
    {
        DrawPageIndication(daily_calendar->win_id,2,daily_calendar->dev_info);
    }
#endif
    DrawDailyItem(daily_calendar);
}
#endif
#if (defined TOUCH_PANEL_SUPPORT && defined MMI_CALENDAR_DAILY_SUPPORT)
/*****************************************************************************/
// Description : set common character
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void ExitSlideDailycalendar( MMI_DAILY_CALENDAR_T* daily_calendar)
{
#ifdef MMI_CALENDAR_SLIDE
    StopSlideFlingTimer(&daily_calendar->slide_calendar);
    StopSlideRecoverTimer(&daily_calendar->slide_calendar);
    StopSlideRedrawTimer(&daily_calendar->slide_calendar);
    SCI_MEMSET(&daily_calendar->slide_calendar,0,sizeof(MMI_SLIDE_CALENDAR_T));
    MMK_ResetVelocityItem();
#endif
}

/*****************************************************************************/
//  Description : HandleSlideTpDown
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleDailySlideTpDown(
                                GUI_POINT_T       *tp_point_ptr,    
                                MMI_DAILY_CALENDAR_T* daily_calendar
                                )
{
#ifdef MMI_CALENDAR_SLIDE
    if(daily_calendar->slide_calendar.slide_state!=MMK_TP_SLIDE_NONE)
    {
        ExitSlideDailycalendar(daily_calendar);
    }
    SCI_MEMSET(&daily_calendar->slide_calendar,0,sizeof(MMI_SLIDE_CALENDAR_T));
    daily_calendar->slide_calendar.handle=daily_calendar->win_id;
    
    MMK_AddVelocityItem( tp_point_ptr->x, tp_point_ptr->y );

    daily_calendar->slide_calendar.slide_start_point.x = tp_point_ptr->x;
    daily_calendar->slide_calendar.slide_start_point.y = tp_point_ptr->y;


    daily_calendar->slide_calendar.slide_pre_point.x = tp_point_ptr->x;
    daily_calendar->slide_calendar.slide_pre_point.y = tp_point_ptr->y;

    daily_calendar->slide_calendar.slide_state = MMK_TP_SLIDE_NONE;

    //StartSlideRedrawTimer(&daily_calendar->slide_calendar);
#endif
    daily_calendar->down_index=GetTpIndex(tp_point_ptr,daily_calendar);
#ifdef MMI_CALENDAR_KEY_STYLE
    if(-1!=daily_calendar->down_index)
    {
       daily_calendar->focus_index=daily_calendar->down_index;
       DrawDailyItem(daily_calendar);
    }
#endif
}
/*****************************************************************************/
//  Description : HandleSlideTpDown
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleDailySlideTpMove(
                                GUI_POINT_T       *point_ptr,    
                                MMI_DAILY_CALENDAR_T* daily_calendar
                                )
{
#ifdef MMI_CALENDAR_SLIDE
    // 增加速度点
    MMK_AddVelocityItem( point_ptr->x, point_ptr->y );

    if (MMK_TP_SLIDE_NONE == daily_calendar->slide_calendar.slide_state
        && IsTpMoved(&daily_calendar->slide_calendar, point_ptr)
        &&point_ptr->y >daily_calendar->item_rect.top
        &&point_ptr->y<daily_calendar->item_rect.bottom)
    {
        StartSlideRedrawTimer(&daily_calendar->slide_calendar);
    
        daily_calendar->slide_calendar.slide_state = MMK_TP_SLIDE_TP_SCROLL;
        daily_calendar->slide_calendar.slide_pre_point.x = point_ptr->x;
        daily_calendar->slide_calendar.slide_pre_point.y = point_ptr->y;
        if(daily_calendar->slide_calendar.slide_start_point.y <=daily_calendar->item_rect.top)
        {
            daily_calendar->slide_calendar.slide_start_point.x = point_ptr->x;
            daily_calendar->slide_calendar.slide_start_point.y = daily_calendar->item_rect.top+1;
        }
        else if(daily_calendar->slide_calendar.slide_start_point.y >=daily_calendar->item_rect.bottom)
        {
            daily_calendar->slide_calendar.slide_start_point.x = point_ptr->x;
            daily_calendar->slide_calendar.slide_start_point.y = daily_calendar->item_rect.bottom-1;
        }
    }
#endif
    return;
    
}

/*****************************************************************************/
//  Description : handle timer
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDailySlideTimer(
                                       MMI_DAILY_CALENDAR_T* daily_calendar, // [in]
                                       DPARAM           param           // [in]
                                       )
{
    MMI_RESULT_E        result              = MMI_RESULT_TRUE;
#ifdef MMI_CALENDAR_SLIDE
    GUI_POINT_T         point               = {0};
    MMI_TP_STATUS_E     state               = MMI_TP_NONE;
    int16 y_offset=0;
    int32   cur_pos = 0;

    if (daily_calendar->slide_calendar.redraw_timer_id == *(uint8*)param)
    {
        MMK_GetLogicTPMsg(&state, &point);
        
        StopSlideRedrawTimer(&daily_calendar->slide_calendar);
        StartSlideRedrawTimer(&daily_calendar->slide_calendar);
        
        if(MMK_TP_SLIDE_NONE==daily_calendar->slide_calendar.slide_state
            ||point.y==daily_calendar->slide_calendar.slide_pre_point.y)
        {
            return MMI_RESULT_FALSE;
        }

        if(MMK_TP_SLIDE_TP_SCROLL==daily_calendar->slide_calendar.slide_state)
       {
            y_offset=point.y-daily_calendar->slide_calendar.slide_pre_point.y;
            daily_calendar->cur_offset_y+=y_offset;
            if(daily_calendar->cur_offset_y>daily_calendar->item_rect.top)
            {
                daily_calendar->cur_offset_y=daily_calendar->item_rect.top;
            }
            else if(daily_calendar->cur_offset_y<daily_calendar->item_rect.top-(24-daily_calendar->max_dis_item_num)*daily_calendar->item_height)
            {
                daily_calendar->cur_offset_y=daily_calendar->item_rect.top-(24-daily_calendar->max_dis_item_num)*daily_calendar->item_height;
            }
            DrawDailyItem(daily_calendar);
        }

        daily_calendar->slide_calendar.slide_pre_point.x = point.x;
        daily_calendar->slide_calendar.slide_pre_point.y = point.y;
    }
    else if(daily_calendar->slide_calendar.fling_timer_id == *(uint8*)param)
    {
        StopSlideFlingTimer(&daily_calendar->slide_calendar);
        
        if (MMK_TP_SLIDE_FLING == daily_calendar->slide_calendar.slide_state)
        {
            // 重算加速度
            cur_pos = MMK_GetFlingOffset(
                    daily_calendar->cur_offset_y, daily_calendar->slide_calendar.velocity,
                    TOUCH_FLING_TIME, &daily_calendar->slide_calendar.velocity);

            daily_calendar->cur_offset_y=cur_pos;
            if(daily_calendar->cur_offset_y>daily_calendar->item_rect.top)
            {
                daily_calendar->cur_offset_y=daily_calendar->item_rect.top;
                DrawDailyItem(daily_calendar);
                ExitSlideDailycalendar(daily_calendar);
            }
            else if(daily_calendar->cur_offset_y<daily_calendar->item_rect.top-(24-daily_calendar->max_dis_item_num)*daily_calendar->item_height)
            {
                daily_calendar->cur_offset_y=daily_calendar->item_rect.top-(24-daily_calendar->max_dis_item_num)*daily_calendar->item_height;
                DrawDailyItem(daily_calendar);
                ExitSlideDailycalendar(daily_calendar);
            }
            else if (FLING_RUN_MIN_VELOCITY <= abs((int32)daily_calendar->slide_calendar.velocity)) // 还可以加速运动
            {
                DrawDailyItem(daily_calendar);
                StartSlideFlingTimer(&daily_calendar->slide_calendar);
            }
            else    // 小于最小加速度，进入恢复状态
            {
                DrawDailyItem(daily_calendar);
            }
        }
    }
    else
    {
        result = MMI_RESULT_FALSE;
    }
#endif

    return result;
}

/*****************************************************************************/
//  Description : HandleSlideTpDown
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleDailySlideTpUp(
                                GUI_POINT_T       *point_ptr,    
                                MMI_DAILY_CALENDAR_T* daily_calendar,
                                MMISCH_DETAIL_WIN_PARAM_T **detail_win_param
                                )
{
    MMIACC_SCH_LIST_T  user_list={0};
#ifdef MMI_CALENDAR_SLIDE
    if(MMK_TP_SLIDE_TP_SCROLL==daily_calendar->slide_calendar.slide_state)
    {
        StopSlideRedrawTimer(&daily_calendar->slide_calendar);
        MMK_AddVelocityItem( point_ptr->x, point_ptr->y );
        MMK_ComputeCurrentVelocity(PNULL,&daily_calendar->slide_calendar.velocity, 0, FLING_MAX_VELOCITY);/*lint !e747*/
        daily_calendar->slide_calendar.slide_pre_point.x = point_ptr->x;
        daily_calendar->slide_calendar.slide_pre_point.y = point_ptr->y;
        if (FLING_MIN_VELOCITY <= abs((int32)daily_calendar->slide_calendar.velocity))  // 达到一定速度，需要滑动
        {
            daily_calendar->slide_calendar.slide_state = MMK_TP_SLIDE_FLING;
            StartSlideFlingTimer(&daily_calendar->slide_calendar);
        }
        else
        {
            ExitSlideDailycalendar(daily_calendar);

        }
    }
    else
    {
        if(MMK_TP_SLIDE_NONE==daily_calendar->slide_calendar.slide_state)
        {
#endif   
            if(daily_calendar->down_index==GetTpIndex(point_ptr,daily_calendar)
                &&daily_calendar->down_index!=-1)
            {
                GetCurHourEvent(daily_calendar,daily_calendar->down_index,&user_list);
                if(user_list.total>1)
                {
                    s_win_list_param->year = daily_calendar->date.year;
                    s_win_list_param->month = daily_calendar->date.month;
                    s_win_list_param->day = daily_calendar->date.day;
                    MMIAPISCH_OpenViewListWin(s_win_list_param);
                }
                else if(user_list.total==1)
                {
                    if(NULL != (*detail_win_param))
                    {
                        SCI_FREE((*detail_win_param));
                    }
                    (*detail_win_param) = SCI_ALLOC_APP(sizeof(MMISCH_DETAIL_WIN_PARAM_T));
            		if((*detail_win_param) != PNULL)
            		{
            	        SCI_MEMSET((*detail_win_param), 0, sizeof(MMISCH_DETAIL_WIN_PARAM_T));
            	        
            	        (*detail_win_param)->win_id = daily_calendar->win_id;
            	        (*detail_win_param)->index = user_list.entry_id[0];

            	        MMIAPIALM_FormInteEventInfo(user_list.entry_id[0],&(*detail_win_param)->event);
            			if(!MMIAPIALM_GetRingInfo(user_list.entry_id[0],&(*detail_win_param)->event))
            			{
            	         	(*detail_win_param)->event.event_content_info.ring_type = ALM_FIX_RING;
            				(*detail_win_param)->event.event_content_info.ring = 1;
            	        }
            	        MMIAPISCH_OpenDetailWin( (ADD_DATA)(*detail_win_param));
            			MMIAPISCH_DeleteUnreadEvent(user_list.entry_id[0]);
            		}
                }
            }
#ifdef MMI_CALENDAR_SLIDE
        }
        ExitSlideDailycalendar(daily_calendar);
    }
#endif
    return;
}
/*****************************************************************************/
//  Description : GetTpIndex
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL int16 GetTpIndex(
                                GUI_POINT_T       *point_ptr,    
                                MMI_DAILY_CALENDAR_T* daily_calendar
                                )
{
    int i=0;
    uint16 lcd_width=0;
    uint16 lcd_height=0;
    GUI_RECT_T single_item_rect={0};
    int16 index=-1;



    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    single_item_rect.left=0;
    single_item_rect.right=lcd_width-1;
    
    if(!GUI_PointIsInRect( *point_ptr, daily_calendar->item_rect))
    {
       index=-1; 
    }
    else
    {
        for(i=0;i<24;i++)
        {
            single_item_rect.top=daily_calendar->cur_offset_y+i*daily_calendar->item_height;
            single_item_rect.bottom=single_item_rect.top+daily_calendar->item_height-1;
            if(GUI_PointIsInRect( *point_ptr, single_item_rect))
            {
               index=i;
               break;
            }
        }
    }

    return  index;
}
#endif
#ifdef MMI_CALENDAR_DAILY_SUPPORT
/*****************************************************************************/
//  Description : GetTpIndex
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetCurHourEvent(  
                                MMI_DAILY_CALENDAR_T* daily_calendar,
                                uint8 hour,
                                MMIACC_SCH_LIST_T*cur_hour_list
                                )
{
    MMIACC_SCH_LIST_T*  user_list=MMIAPISCH_GetUserList();
    int i=0;
    MMIACC_SMART_EVENT_T event={0};

    cur_hour_list->total=0;
    
    for(i=0;i<user_list->total;i++)
    {
        MMIAPIALM_FormInteEventInfo(user_list->entry_id[i],&event);
        if(hour==event.event_fast_info.hour)
        {
            cur_hour_list->entry_id[cur_hour_list->total]=user_list->entry_id[i];
            cur_hour_list->total++;
        }
    }

    return cur_hour_list->total;
}
#endif
#ifdef MMI_CALENDAR_DAILY_SUPPORT
/*****************************************************************************/
//  Description : HandleSlideTpDown
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void InitDailyCalendar(    
                                MMI_DAILY_CALENDAR_T* daily_calendar,
                                uint16 year,
                                uint8 mon,
                                uint8 day,
                                MMI_WIN_ID_T win_id
                                )
{
    uint16 lcd_width=0;
    uint16 lcd_height=0;
    int16 isstyle_start_y=0;
    int16 isstyle_item_h=0;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
#ifdef TOUCH_PANEL_SUPPORT
    ExitSlideDailycalendar(daily_calendar);
#endif
    SCI_MEMSET(daily_calendar,0,sizeof(MMI_DAILY_CALENDAR_T));
    daily_calendar->date.day= day;
    daily_calendar->date.month= mon;
    daily_calendar->date.year= year;
    daily_calendar->win_id=win_id;
    daily_calendar->dev_info=&s_calendar_lcd_dev_info;
    if(!MMITHEME_IsMainScreenLandscape ())
    {
#if defined(MMI_ISTYLE_SUPPORT)    
        if(MMITHEME_IsIstyle())
        {
            isstyle_start_y=MMITHEME_GetWinTitleHeight()+CALENDAR_ISSTYLE_DAILY_START_Y_OFFSET_V;
            isstyle_item_h=CALENDAR_ISSTYLE_DAILY_ITEM_HEIGH_V;
        }
        else
#endif			
        {
            isstyle_start_y=CALENDAR_DAILY_START_Y_OFFSET_V;
            isstyle_item_h=CALENDAR_DAILY_ITEM_HEIGH_V;
        }
        daily_calendar->cur_offset_y=isstyle_start_y;
        daily_calendar->item_rect.left=0;
        daily_calendar->item_rect.top=isstyle_start_y;
        daily_calendar->item_rect.right=lcd_width-1;
        daily_calendar->item_rect.bottom=isstyle_start_y+isstyle_item_h*CALENDAR_DAILY_MAX_ITEM_NUM_V-1;
        daily_calendar->max_dis_item_num=CALENDAR_DAILY_MAX_ITEM_NUM_V;
        daily_calendar->item_height=isstyle_item_h;
    }
    else
    {
#if defined(MMI_ISTYLE_SUPPORT)    
        if(MMITHEME_IsIstyle())
        {
            isstyle_start_y=MMITHEME_GetWinTitleHeight()+CALENDAR_ISSTYLE_DAILY_START_Y_OFFSET_H;
            isstyle_item_h=CALENDAR_ISSTYLE_DAILY_ITEM_HEIGH_H;
        }
        else
#endif			
        {
            isstyle_start_y=CALENDAR_DAILY_START_Y_OFFSET_H;
            isstyle_item_h=CALENDAR_DAILY_ITEM_HEIGH_H;
        }
        daily_calendar->cur_offset_y=isstyle_start_y;
        daily_calendar->item_rect.left=0;
        daily_calendar->item_rect.top=isstyle_start_y;
        daily_calendar->item_rect.right=lcd_width-1;
        daily_calendar->item_rect.bottom=isstyle_start_y+isstyle_item_h*CALENDAR_DAILY_MAX_ITEM_NUM_H-1;
        daily_calendar->max_dis_item_num=CALENDAR_DAILY_MAX_ITEM_NUM_H;
        daily_calendar->item_height=isstyle_item_h;
    }
}
#endif

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : Init Weekly Button
//  Global resource dependence :
//  Author:junxue zhu
//  Note:
/*****************************************************************************/
LOCAL void InitWeeklyButtonChtrl(MMI_WIN_ID_T win_id)
{
    GUI_BG_T bg = {0};
    GUI_RECT_T rect_l = {0};
    GUI_RECT_T rect_r = {0};
    uint16 img_width = 0;
    uint16 img_height = 0;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    int16 top = 0;
    GUI_RECT_T title_rect_v = CALENDAR_WEEKLY_WEEK_CONTENT_RECT_V;
    GUI_RECT_T title_rect_h = CALENDAR_WEEKLY_WEEK_CONTENT_RECT_H;
    GUI_RECT_T title_rect = {0};

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    bg.bg_type=GUI_BG_IMG;
    bg.img_id=IMAGE_CLOCK_CALENDAR_RIGHT_DEFAULT;
    GUIBUTTON_SetPressedBg(MMICALENDAR_WEEK_RIGHT_BUTTON_CTRL_ID,&bg);
    bg.img_id=IMAGE_CLOCK_CALENDAR_LEFT_DEFAULT;
    GUIBUTTON_SetPressedBg(MMICALENDAR_WEEK_LEFT_BUTTON_CTRL_ID,&bg);

    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_CLOCK_CALENDAR_LEFT_DEFAULT, win_id);
    
    if(!MMITHEME_IsMainScreenLandscape ())
    {
         title_rect=title_rect_v;
    }
    else
    {
         title_rect=title_rect_h;  
    }

    if((title_rect.bottom-title_rect.top)>=img_height)
    {
       top = title_rect.top+(title_rect.bottom-title_rect.top-img_height)/2;
    }
    else
    {
       top = title_rect.top+(img_height-title_rect.bottom+title_rect.top)/2;
    }

    rect_l.left= 15;
    rect_l.top = top;
    rect_l.right= rect_l.left+img_width;
    rect_l.bottom = rect_l.top+img_height;
    
    rect_r.left= lcd_width-15-img_width;;
    rect_r.top = top;
    rect_r.right= rect_r.left+img_width;
    rect_r.bottom = rect_r.top+img_height;

    GUIBUTTON_SetRect(MMICALENDAR_WEEK_LEFT_BUTTON_CTRL_ID,&rect_l);
    GUIBUTTON_SetRect(MMICALENDAR_WEEK_RIGHT_BUTTON_CTRL_ID,&rect_r);
}
#endif

#if (defined MMI_CALENDAR_DAILY_SUPPORT) && (defined TOUCH_PANEL_SUPPORT)
/*****************************************************************************/
//  Description : HandleSlideTpDown
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void InitDailyButtonChtrl(MMI_WIN_ID_T win_id)
{
    GUI_BG_T bg = {0};
    GUI_RECT_T rect_l = {0};
    GUI_RECT_T rect_r = {0};
    uint16 img_width = 0;
    uint16 img_height = 0;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    int16 top = 0;
    GUI_RECT_T title_rect_v=CALENDAR_DAILY_TITLE_RECT_V;
    GUI_RECT_T title_rect_h=CALENDAR_DAILY_TITLE_RECT_H;
    GUI_RECT_T title_rect = {0};

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    bg.bg_type=GUI_BG_IMG;
    bg.img_id=IMAGE_CLOCK_CALENDAR_RIGHT_DEFAULT;
    GUIBUTTON_SetPressedBg(MMICALENDAR_RIGHT_BUTTON_CTRL_ID,&bg);
    bg.img_id=IMAGE_CLOCK_CALENDAR_LEFT_DEFAULT;
    GUIBUTTON_SetPressedBg(MMICALENDAR_LEFT_BUTTON_CTRL_ID,&bg);

    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_CLOCK_CALENDAR_LEFT_DEFAULT, win_id);
    
    if(!MMITHEME_IsMainScreenLandscape ())
    {
         title_rect=title_rect_v;
    }
    else
    {
		title_rect=title_rect_h;  
    }
	
    if((title_rect.bottom-title_rect.top)>=img_height)
    {
		top = title_rect.top+(title_rect.bottom-title_rect.top-img_height)/2;
    }
    else
    {
		top = title_rect.top+(img_height-title_rect.bottom+title_rect.top)/2;
    }
	
    rect_l.left= 15;
    rect_l.top = top;
    rect_l.right= rect_l.left+img_width;
    rect_l.bottom = rect_l.top+img_height;
    
    rect_r.left= lcd_width-15-img_width;;
    rect_r.top = top;
    rect_r.right= rect_r.left+img_width;
    rect_r.bottom = rect_r.top+img_height;
	
    GUIBUTTON_SetRect(MMICALENDAR_RIGHT_BUTTON_CTRL_ID,&rect_r);
    GUIBUTTON_SetRect(MMICALENDAR_LEFT_BUTTON_CTRL_ID,&rect_l);

}
#endif
#ifdef MMI_CALENDAR_DAILY_SUPPORT
/*****************************************************************************/
//    Description : calendar weekly display window
//    Global resource dependence :
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCalendarDailyWinMsg(
									        MMI_WIN_ID_T        win_id,
									        MMI_MESSAGE_ID_E    msg_id,
									        DPARAM              param
									        )
{
    MMI_RESULT_E            ret_val = MMI_RESULT_TRUE;
    MMI_CALENDAR_T     *calendar_ctrl_ptr = &g_calendar_show;
    //BOOLEAN                 *is_weekly_ptr = NULL;
    uint16                       index = 0;
    //MMISET_DATE_T*win_param = (MMISCH_LIST_WIN_PARAM_T*)MMK_GetWinAddDataPtr(win_id);
    MMI_DAILY_CALENDAR_T* daily_calendar=&s_daily_calendar;
#ifdef TOUCH_PANEL_SUPPORT
    MMI_CTRL_ID_T   ctrl_id=MMICALENDAR_LEFT_BUTTON_CTRL_ID;
#endif
    static MMISCH_DETAIL_WIN_PARAM_T *detail_win_param=PNULL;
    int16 old_y=0;
#ifdef MMI_CALENDAR_KEY_STYLE
    int16 old_focus_index=0;
#endif
    //GUI_BG_T  bg_info={0};
    
    switch( msg_id )
    {		
	case MSG_OPEN_WINDOW:
#ifdef TOUCH_PANEL_SUPPORT
        InitDailyButtonChtrl(win_id);
#endif
        InitDailyCalendar(daily_calendar,calendar_ctrl_ptr->solar_year,calendar_ctrl_ptr->solar_month,calendar_ctrl_ptr->solar_date,win_id);
        MMIAPISCH_OpenEventList(FALSE, daily_calendar->date.year, daily_calendar->date.month, daily_calendar->date.day);
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#else
		GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
#endif
#if defined(MMI_ISTYLE_SUPPORT)
        if(MMITHEME_IsIstyle())
        {
            MMI_CONTROL_CREATE_T create={0};
            GUITITLE_INIT_DATA_T   title_data   = {0};
            title_data.text_id=TXT_TOOLS_CALENDAR;
            create.ctrl_id = MMITHEME_GetTitleCtrlId();
            create.guid    = SPRD_GUI_TITLE_ID;
            create.parent_win_handle = win_id;
            create.init_data_ptr = &title_data;
            MMK_CreateControl( &create );
            MMK_SetWinTitleCtrlId( win_id, create.ctrl_id );
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
#endif		
        MMK_SetAtvCtrl(win_id, MMICALENDAR_LEFT_BUTTON_CTRL_ID);
        /*bg_info.bg_type=GUI_BG_COLOR;
        bg_info.color=MMI_CALENDAR_BG_COLOR;
        GUISTATUSBAR_SetBgIsDesplay(win_id,TRUE);
        GUISTATUSBAR_SetBg(win_id,bg_info);*/
		break;
		
	case MSG_FULL_PAINT:
        MMIAPISCH_OpenEventList(FALSE, daily_calendar->date.year, daily_calendar->date.month, daily_calendar->date.day);
        DrawDailyCalendar(daily_calendar);
		break;
    case MSG_GET_FOCUS:
#if defined(MMI_ISTYLE_SUPPORT)		
        if(MMITHEME_IsIstyle())
        {
            MMI_CONTROL_CREATE_T create={0};
            GUITITLE_INIT_DATA_T   title_data   = {0};          
            title_data.text_id=TXT_TOOLS_CALENDAR;
            create.ctrl_id = MMITHEME_GetTitleCtrlId();
            create.guid    = SPRD_GUI_TITLE_ID;
            create.parent_win_handle = win_id;
            create.init_data_ptr = &title_data;
            MMK_DestroyControl(MMITHEME_GetTitleCtrlId());
            MMK_CreateControl( &create );
            MMK_SetWinTitleCtrlId( win_id, create.ctrl_id );
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
#endif		
        old_y=daily_calendar->cur_offset_y;
#ifdef MMI_CALENDAR_KEY_STYLE
        old_focus_index=daily_calendar->focus_index;
#endif
        InitDailyCalendar(daily_calendar,calendar_ctrl_ptr->solar_year,calendar_ctrl_ptr->solar_month,calendar_ctrl_ptr->solar_date,win_id);
        MMIAPISCH_OpenEventList(FALSE, daily_calendar->date.year, daily_calendar->date.month, daily_calendar->date.day);
        daily_calendar->cur_offset_y=old_y;
 #ifdef MMI_CALENDAR_KEY_STYLE
        daily_calendar->focus_index=old_focus_index;
 #endif
        break;
    case MSG_LOSE_FOCUS:
#ifdef TOUCH_PANEL_SUPPORT
        s_cur_tp_win=0;
#endif
        break;

	case MSG_KEYDOWN_CANCEL:
#ifdef MMI_CALENDAR_WIN_SLIDE
		MMISLIDE_Deactive(&s_win_slide_handle);
        MMISLIDE_Release(&s_win_slide_handle);
#if 0//release???????
        MMK_CloseWin(MMICALENDAR_WEEKLY_WIN_ID);
        MMK_CloseWin(MMICALENDAR_WIN_ID);
		MMK_CloseWin(win_id);
#endif
#else
		MMK_CloseWin(win_id);
#endif
		break;       

    case MSG_CLOSE_WINDOW:
        //SCI_FREE(win_param);
#ifdef TOUCH_PANEL_SUPPORT
        s_cur_tp_win=0;
#endif
        if(NULL != detail_win_param)
        {
            SCI_FREE(detail_win_param);
            detail_win_param=PNULL;
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
        if(MMICALENDAR_LEFT_BUTTON_CTRL_ID==ctrl_id)
        {
            MMK_SendMsg(win_id, MSG_APP_LEFT,  PNULL);
        }
        else if(MMICALENDAR_RIGHT_BUTTON_CTRL_ID==ctrl_id)
        {
            MMK_SendMsg(win_id, MSG_APP_RIGHT,  PNULL);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
#ifdef MMI_CALENDAR_KEY_STYLE
         {
            MMIACC_SCH_LIST_T  user_list={0};
            GetCurHourEvent(daily_calendar,daily_calendar->focus_index,&user_list);
            if(user_list.total>1)
            {
                s_win_list_param->year = daily_calendar->date.year;
                s_win_list_param->month = daily_calendar->date.month;
                s_win_list_param->day = daily_calendar->date.day;
                MMIAPISCH_OpenViewListWin(s_win_list_param);
            }
            else if(user_list.total==1)
            {
                if(NULL != detail_win_param)
                {
                    SCI_FREE(detail_win_param);
                }
                detail_win_param = SCI_ALLOC_APP(sizeof(MMISCH_DETAIL_WIN_PARAM_T));
        		if(detail_win_param != PNULL)
        		{
        	        SCI_MEMSET(detail_win_param, 0, sizeof(MMISCH_DETAIL_WIN_PARAM_T));
        	        
        	        detail_win_param->win_id = daily_calendar->win_id;
        	        detail_win_param->index = user_list.entry_id[0];

        	        MMIAPIALM_FormInteEventInfo(user_list.entry_id[0],&detail_win_param->event);
        			if(!MMIAPIALM_GetRingInfo(user_list.entry_id[0],&detail_win_param->event))
        			{
        	         	detail_win_param->event.event_content_info.ring_type = ALM_FIX_RING;
        				detail_win_param->event.event_content_info.ring = 1;
        	        }
        	        MMIAPISCH_OpenDetailWin( (ADD_DATA)detail_win_param);
        			MMIAPISCH_DeleteUnreadEvent(user_list.entry_id[0]);
        		}
            }
        }  
#endif
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
	case MSG_APP_MENU:
        MMK_CreateWin((uint32 *)CALENDAR_DAILY_OPTION_WIN_TAB, PNULL);
        break;
	case MSG_KEYREPEAT_UP:
    case  MSG_APP_UP:
#ifdef MMI_CALENDAR_KEY_STYLE
            {
                int16 num=CALENDAR_DAILY_MAX_ITEM_NUM_V;
                if(MMITHEME_IsMainScreenLandscape ())
                {
                   num=CALENDAR_DAILY_MAX_ITEM_NUM_H;
                }
                if(daily_calendar->focus_index>0)
                {
                   daily_calendar->focus_index--;
                   DrawDailyItem(daily_calendar);
                }
                else
                {
                   daily_calendar->focus_index=23;
                   DrawDailyItem(daily_calendar);
                }
            }
#endif
        break;
    case MSG_KEYREPEAT_DOWN:
    case  MSG_APP_DOWN:
#ifdef MMI_CALENDAR_KEY_STYLE
            {
                int16 num=CALENDAR_DAILY_MAX_ITEM_NUM_V;
                if(MMITHEME_IsMainScreenLandscape ())
                {
                   num=CALENDAR_DAILY_MAX_ITEM_NUM_H;
                }
                if(daily_calendar->focus_index<23)
                {
                   daily_calendar->focus_index++;
                   DrawDailyItem(daily_calendar);
                }
                else
                {
                   daily_calendar->focus_index=0;
                   DrawDailyItem(daily_calendar);
                }
            }
#endif
        break;

    case  MSG_APP_LEFT:
        MovePrevDay(daily_calendar->date, &daily_calendar->date);
        calendar_ctrl_ptr->solar_year = daily_calendar->date.year;
        calendar_ctrl_ptr->solar_month = daily_calendar->date.month;
        calendar_ctrl_ptr->solar_date= daily_calendar->date.day;
        InitDailyCalendar(daily_calendar,daily_calendar->date.year,daily_calendar->date.month,daily_calendar->date.day,win_id);
        MMIAPISCH_OpenEventList(FALSE, daily_calendar->date.year, daily_calendar->date.month, daily_calendar->date.day);
        MMK_SendMsg(win_id, MSG_FULL_PAINT,  PNULL);
        break;

    case  MSG_APP_RIGHT:
        MoveNextDay(daily_calendar->date, &daily_calendar->date);
        calendar_ctrl_ptr->solar_year = daily_calendar->date.year;
        calendar_ctrl_ptr->solar_month = daily_calendar->date.month;
        calendar_ctrl_ptr->solar_date= daily_calendar->date.day;
        InitDailyCalendar(daily_calendar,daily_calendar->date.year,daily_calendar->date.month,daily_calendar->date.day,win_id);
        MMIAPISCH_OpenEventList(FALSE, daily_calendar->date.year, daily_calendar->date.month, daily_calendar->date.day);
        MMK_SendMsg(win_id, MSG_FULL_PAINT,  PNULL);
        break;

     case MSG_SCH_EVENT_EDIT_FINISH:
        if (MMIALM_AddOneEvent(&s_win_param->event,&index))
        {
            MMIAPISCH_OpenEventList(FALSE, calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, calendar_ctrl_ptr->solar_date);
            MMK_SendMsg(win_id, MSG_FULL_PAINT,  PNULL);
        }
	    else
        {
    		if(MMIACC_SCH_NUM<=(MMIAPISCH_GetAllSchNumber()))
    		{
    		    MMIPUB_OpenAlertWarningWin(TXT_SCHEDULE_FULL);
    		}
    		else
    		{
    		    MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
    		}      
	    } 
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            s_cur_tp_win=win_id;
            HandleDailySlideTpDown(&point,daily_calendar);
		}		
		break;

    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T point;
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            HandleDailySlideTpMove(&point,daily_calendar);
        }
        break;

    case MSG_TIMER:
        HandleDailySlideTimer(daily_calendar, param);
        break;
#endif  
    case MSG_LCD_SWITCH:
#if defined(MMI_ISTYLE_SUPPORT)		
        if(MMITHEME_IsIstyle())
        {
            MMI_CONTROL_CREATE_T create={0};
            GUITITLE_INIT_DATA_T   title_data   = {0};
            title_data.text_id=TXT_TOOLS_CALENDAR;
            create.ctrl_id = MMITHEME_GetTitleCtrlId();
            create.guid    = SPRD_GUI_TITLE_ID;
            create.parent_win_handle = win_id;
            create.init_data_ptr = &title_data;
            MMK_DestroyControl(MMITHEME_GetTitleCtrlId());
            MMK_CreateControl( &create );
            MMK_SetWinTitleCtrlId( win_id, create.ctrl_id );
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
#endif		
#ifdef TOUCH_PANEL_SUPPORT
        InitDailyButtonChtrl(win_id);
#endif
        InitDailyCalendar(daily_calendar,calendar_ctrl_ptr->solar_year,calendar_ctrl_ptr->solar_month,calendar_ctrl_ptr->solar_date,win_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_UP:	
		{
            GUI_POINT_T point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            if(s_cur_tp_win==win_id)
            {
                HandleDailySlideTpUp(&point,daily_calendar,&detail_win_param);
            }
		}	
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		
    default:
		ret_val = MMI_RESULT_FALSE;
	    break;
    }

    return ret_val;
}
#endif


#ifdef ATEST_SUPPORT
/*****************************************************************************/
//    Description : get the date in calendar window
//    Global resource dependence : 
//    Author: lin.chen
//    Note:
/*****************************************************************************/
PUBLIC void ATESTCALENDAR_GetCurDate(uint8 *date,uint8 *month,uint16 *year)
{
	MMI_CALENDAR_T     *calendar_ctrl_ptr = &g_calendar_show;

	*date =calendar_ctrl_ptr->solar_date;
	*month = calendar_ctrl_ptr->solar_month;
	*year = calendar_ctrl_ptr->solar_year;
}

/*****************************************************************************/
//    Description : get the rect of date
//    Global resource dependence : 
//    Author:  lin.chen
//    Note:
/*****************************************************************************/
PUBLIC void ATESTCALENDAR_GetDateRect(uint8 date,uint8 month,uint16 year,GUI_RECT_T *rect)
{
	MMI_CALENDAR_T     *calendar_ctrl_ptr = &g_calendar_show;
    uint32     row       = 0;
    uint32     col       = 0;

	GetPosByDate( calendar_ctrl_ptr, year, month, date, &row, &col );	
	*rect = GetDateRect( calendar_ctrl_ptr, row, col );
}
#endif //ATEST_SUPPORT
#ifdef MMI_CALENDAR_DAILY_SUPPORT
/*****************************************************************************/
//    Description : calendar option window
//    Global resource dependence : 
//    Author: Mark.hu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCalendarDailyOptionWinMsg(
                                              MMI_WIN_ID_T        win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM              param
                                              )
{
    uint16                              i                   = 0;
    uint16                              node_id             = 0;
    MMI_RESULT_E                        ret_val             = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                       ctrl_id             = MMICALENDAR_DAILY_OPT_MENU_CTRL_ID;
    MMI_CALENDAR_T                 *calendar_ctrl_ptr  = &g_calendar_show;

	MMISET_LANGUAGE_TYPE_E lang_type=MMISET_LANGUAGE_ENGLISH;
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
	static MMISET_DATE_T   *detail_win_param = PNULL;
#endif 
	static uint32 *to_date_param=PNULL;
	//uint16            index = 0;
    SCI_DATE_T                          cur_date            = {0};
    
    switch( msg_id )
    {		
	case MSG_OPEN_WINDOW:
		{
#ifdef MMI_GUI_STYLE_MINISCREEN
			MMI_STRING_T   str_menutitle  ={0};
#endif
        MMIAPISET_GetLanguageType(&lang_type);
        GUIMENU_CreatDynamic(PNULL,win_id,ctrl_id,GUIMENU_STYLE_POPUP);
        i=0;
#ifdef MMI_GUI_STYLE_MINISCREEN
        MMI_GetLabelTextByLang(TXT_IDLE_CALENDAR,&str_menutitle);
        GUIMENU_SetMenuTitle(&str_menutitle,ctrl_id);
#endif
#ifdef MMI_PDA_SUPPORT
        InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_NEW, IMG_CALENDAR_MENU_ADD_ICON,CALENDAR_OPT_NEW, 0, i++);
        InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_LOOK, IMG_CALENDAR_MENU_VIEW_ICON,CALENDAR_OPT_LOOK, 0, i++);
		InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_ALL, IMG_CALENDAR_MENU_VIEW_ICON,CALENDAR_OPT_ALL, 0, i++);
        if(MMISET_LANGUAGE_SIMP_CHINESE==lang_type ||MMISET_LANGUAGE_TRAD_CHINESE==lang_type)
		{
			InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_LUNAR_DETAIL, IMG_CALENDAR_MENU_LURA_DETAIL_ICON,CALENDAR_OPT_LUNAR_DETAIL, 0, i++);
		}
		InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_TO_DATE, IMG_CALENDAR_MENU_CLEAN_GOTO_DATE_ICON,CALENDAR_OPT_QUERY, 0, i++);
#else
		InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_NEW, 0,CALENDAR_OPT_NEW, 0, i++);
        InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_LOOK, 0,CALENDAR_OPT_LOOK, 0, i++);
		InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_ALL,0, CALENDAR_OPT_ALL, 0, i++);
        if(MMISET_LANGUAGE_SIMP_CHINESE==lang_type ||MMISET_LANGUAGE_TRAD_CHINESE==lang_type)
		{
			InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_LUNAR_DETAIL, 0,CALENDAR_OPT_LUNAR_DETAIL, 0, i++);
		}
		InsertMenuNodeByLabel(ctrl_id, TXT_CALENDAR_TO_DATE, 0,CALENDAR_OPT_QUERY, 0, i++);
#endif

        MMK_SetAtvCtrl(win_id, ctrl_id);
		}
		break;

    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
	case MSG_APP_WEB:
	case MSG_CTL_MIDSK:
        node_id = GUIMENU_GetCurNodeId(ctrl_id);
        switch( node_id )
        {
        case CALENDAR_OPT_NEW:
            s_win_param->win_id = MMICALENDAR_DAILY_WIN_ID;
            MMIAPISCH_InitNewSch(calendar_ctrl_ptr->solar_year,
                              calendar_ctrl_ptr->solar_month,
                              calendar_ctrl_ptr->solar_date,
                              &s_win_param->event);
            MMIAPISCH_OpenEditWin((ADD_DATA)s_win_param);			
            break;
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
        case CALENDAR_OPT_LUNAR_DETAIL:
			if(NULL != detail_win_param)
	        {
	            SCI_FREE(detail_win_param);
				detail_win_param=PNULL;
	        }
	        detail_win_param = SCI_ALLOC_APP(sizeof(MMISET_DATE_T));
	        if(detail_win_param != PNULL)
	        {
		        SCI_MEMSET(detail_win_param, 0, sizeof(MMISET_DATE_T));
		        
		        detail_win_param->date_year= calendar_ctrl_ptr->solar_year;
		        detail_win_param->date_month= calendar_ctrl_ptr->solar_month;
				detail_win_param->date_day= calendar_ctrl_ptr->solar_date;

		        MMK_CreateWin((uint32 *)CALENDAR_LUNAR_DETAIL_WIN_TAB, (ADD_DATA)detail_win_param);
	        }
			break;
#endif
        case CALENDAR_OPT_QUERY:
            if(PNULL!=to_date_param)
			{
				SCI_FREE(to_date_param);
				to_date_param=PNULL;
			}
			to_date_param=SCI_ALLOC_APP(sizeof(uint32));
			*to_date_param=CALENDAR_TO_DATE_DAILY_CALENDAR;
            MMIAPICALEND_OpenCalendarQueryByDate((ADD_DATA)to_date_param);
            break;
        case CALENDAR_OPT_LOOK:
            s_win_list_param->year = calendar_ctrl_ptr->solar_year;
            s_win_list_param->month = calendar_ctrl_ptr->solar_month;
            s_win_list_param->day = calendar_ctrl_ptr->solar_date;
            MMIAPISCH_OpenViewListWin((ADD_DATA)s_win_list_param);
            break;
        case CALENDAR_OPT_ALL:
            TM_GetSysDate(&cur_date);
            s_win_list_param->year = cur_date.year;
            s_win_list_param->month = cur_date.mon;
            s_win_list_param->day = cur_date.mday;
            MMIAPISCH_OpenViewListAllWin(s_win_list_param);
            break;

        default:
            break;
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
    case MSG_CLOSE_WINDOW:
       break;

    default:
	    ret_val = MMI_RESULT_FALSE;
	    break;
    }

    return ret_val;
}
#endif
/*****************************************************************************/
//    Description : MMIAPICALENDAR_IsOpenCalendar
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICALENDAR_IsOpenCalendar(void)
{
    if(MMK_IsOpenWin(MMICALENDAR_WIN_ID)
        ||MMK_IsOpenWin(MMICALENDAR_WEEKLY_WIN_ID))
    {
        return TRUE;
    }

    return FALSE;
}

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//    Description :判断这个月是否是闰出来的，用来判断显示农历节日
//    Global resource dependence : 
//    Author: panvs
//    Note: 如果上一个月的农历和这个月的农历是同一个月，则这个月是闰出来的
/*****************************************************************************/
LOCAL BOOLEAN IsLeapMonth(
                                 uint16     solar_year,
                                 uint8      solar_month,
                                 uint8      solar_day,
                                 uint8      l_month
                                 )
{
    uint16              lun_year = 0;
    uint8               lun_month = 0;
    uint8               lun_day = 0;
    CALE_SOLAR_DATE_T   date = {0};

    date.year = solar_year;
    date.month = solar_month;
    date.day = solar_day;
    
    //转到上一月
    MovePrevMonth(date, &date);
    
    //获得上一月的农历
    MMICALENDAR_SolarToLunar(date.year, date.month, date.day,
        &lun_year, &lun_month, &lun_day);
    
    //判断两个月是否相等
    if(lun_month == l_month)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*****************************************************************************/
//    Description :GetQingMingDate
//    Global resource dependence : 
//    Author: juan.wu
//    Note: 清明节的计算方法:基本上处于4.4~4.6之间，
//    计算公式:date = [Y * D + C]- [Y/4]
//    其中Y为年份的后两位，D = 0.2422 C = 4.81(21世纪为4.81,20世纪为5.59)  
//    比如2012，Y = 12，则2012的清明日期为:[12*0.2422 + 4.81] - 3
//    这里小数全部放大10000倍
/*****************************************************************************/
LOCAL uint8 GetQingMingDate(uint16  solar_year)
{
    uint16   year = solar_year%100;
    uint32   qm_date = 0;
    uint16   qm_c  = 0;
    uint8    centry = solar_year/100;

    if(20 == centry)
    {
        qm_c = MMI_CALENDAR_QINGMING_FES_20_CENTRY;
    }
    else if(19 == centry)
    {
        qm_c = MMI_CALENDAR_QINGMING_FES_19_CENTRY;
    }
    
    qm_date = (year*2422+ qm_c)/10000 - (year/4);
    return qm_date;
}
/*****************************************************************************/
//    Description :GetFestivalString
//    Global resource dependence : 
//    Author: juan.wu
//    Note: 获得节日字串
/*****************************************************************************/
LOCAL  void      GetFestivalString(
                                    MMI_CALENDAR_T * calendar_ctrl_ptr,
                                    MMI_STRING_T * fest_str_ptr
                                    )
{
    //MMI_STRING_T        fest_str = {0};
    //wchar               fest_text[20] = {0};
    uint16              fest_text_len = 0;
    uint16              lun_year = 0;
    uint8               lun_month = 0;
    uint8               lun_day = 0;
    
    MMICALENDAR_SolarToLunar(calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, calendar_ctrl_ptr->solar_date,
        &lun_year, &lun_month, &lun_day);
        
    if((1 ==calendar_ctrl_ptr->solar_month ) && ( 1 == calendar_ctrl_ptr->solar_date ))
    {
        //元旦
        fest_str_ptr->wstr_ptr[fest_text_len] = 0x0020;
        fest_text_len++;
        fest_str_ptr->wstr_ptr[fest_text_len] = 0x5143;
        fest_text_len++;
        fest_str_ptr->wstr_ptr[fest_text_len] = 0x65E6;
        fest_text_len++;
    }
    else if((5 == calendar_ctrl_ptr->solar_month) && (1 == calendar_ctrl_ptr->solar_date))
    {
        //劳动节
        fest_str_ptr->wstr_ptr[fest_text_len] = 0x0020;
        fest_text_len++;
        fest_str_ptr->wstr_ptr[fest_text_len] = 0x52B3;
        fest_text_len++;
        fest_str_ptr->wstr_ptr[fest_text_len] = 0x52A8;
        fest_text_len++;
        fest_str_ptr->wstr_ptr[fest_text_len] = 0x8282;
        fest_text_len++;
    }
    else if((10 == calendar_ctrl_ptr->solar_month) && (1 == calendar_ctrl_ptr->solar_date))
    {
        //国庆节
        fest_str_ptr->wstr_ptr[fest_text_len] = 0x0020;
        fest_text_len++;
        fest_str_ptr->wstr_ptr[fest_text_len] = 0x56FD;
        fest_text_len++;
        fest_str_ptr->wstr_ptr[fest_text_len] = 0x5E86;
        fest_text_len++;
        fest_str_ptr->wstr_ptr[fest_text_len] = 0x8282;
        fest_text_len++;
    }
    else if((4 == calendar_ctrl_ptr->solar_month) 
            && (calendar_ctrl_ptr->solar_date 
                == GetQingMingDate(calendar_ctrl_ptr->solar_year))
            )
    {
        //清明节
        fest_str_ptr->wstr_ptr[fest_text_len] = 0x0020;
        fest_text_len++;
        fest_str_ptr->wstr_ptr[fest_text_len] = 0x6E05;
        fest_text_len++;
        fest_str_ptr->wstr_ptr[fest_text_len] = 0x660E;
        fest_text_len++;
        fest_str_ptr->wstr_ptr[fest_text_len] = 0x8282;
        fest_text_len++;
    }
    else if((1 == lun_month) && (1 == lun_day))
    {
        //春节
        if(!IsLeapMonth(calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, calendar_ctrl_ptr->solar_date, lun_month))
        {
            fest_str_ptr->wstr_ptr[fest_text_len] = 0x0020;
            fest_text_len++;
            fest_str_ptr->wstr_ptr[fest_text_len] = 0x6625;
            fest_text_len++;
            fest_str_ptr->wstr_ptr[fest_text_len] = 0x8282;
            fest_text_len++;
        }
    }
    else if((5 == lun_month) && (5 == lun_day))
    {
        //端午节
        if(!IsLeapMonth(calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, calendar_ctrl_ptr->solar_date, lun_month))
        {
            fest_str_ptr->wstr_ptr[fest_text_len] = 0x0020;
            fest_text_len++;
            fest_str_ptr->wstr_ptr[fest_text_len] = 0x7AEF;
            fest_text_len++;
            fest_str_ptr->wstr_ptr[fest_text_len] = 0x5348;
            fest_text_len++;
            fest_str_ptr->wstr_ptr[fest_text_len] = 0x8282;
            fest_text_len++;
        }
    }
    else if((8 == lun_month) && (15 == lun_day))
    {
        //中秋节
        if(!IsLeapMonth(calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, calendar_ctrl_ptr->solar_date, lun_month))
        {
            fest_str_ptr->wstr_ptr[fest_text_len] = 0x0020;
            fest_text_len++;
            fest_str_ptr->wstr_ptr[fest_text_len] = 0x4E2D;
            fest_text_len++;
            fest_str_ptr->wstr_ptr[fest_text_len] = 0x79CB;
            fest_text_len++;
            fest_str_ptr->wstr_ptr[fest_text_len] = 0x8282;
            fest_text_len++;
        }
    }
    fest_str_ptr->wstr_len = fest_text_len;
}
#endif

/*Edit by script, ignore 9 case. Thu Apr 26 19:00:48 2012*/ //IGNORE9527


/*Edit by script, ignore 3 case. Fri Apr 27 09:38:47 2012*/ //IGNORE9527

#endif
