/*****************************************************************************
** File Name:      mmicalendar_export.h                                               
** Author:           mark.hu    
** Date:            2005/0907
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe calendar window             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2005/09/07    mark.hu              Create                                 *
** 2006/08/09    liqing.peng          Modify
******************************************************************************/


#ifndef _MMICALENDAR_EXPORT_H_
#define _MMICALENDAR_EXPORT_H_



/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef CALENDAR_SUPPORT 
#include "sci_types.h"
#include "mmicom_time.h"



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif



/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#ifndef MMI_PDA_SUPPORT
#ifndef MMI_CALENDAR_KEY_STYLE
#define MMI_CALENDAR_KEY_STYLE
#endif
#endif

#ifndef MMI_CALENDAR_KEY_STYLE
#ifdef MMI_SLIDE_SUPPORT
#ifndef MMI_CALENDAR_WIN_SLIDE
#define MMI_CALENDAR_WIN_SLIDE
#endif
#endif
#endif  //CR143382 //CR144896

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef enum
{
    CALENDAR_OPT_SCH_PERIOD,
    CALENDAR_OPT_SCH_NOT_PERIOD,
    CALENDAR_OPT_MAX
}CALENDAR_DEL_SCH_TYPE_E;

typedef enum
{
    CALENDAR_TO_DATE_CALENDAR,
    CALENDAR_TO_DATE_SCH_LIST,
    CALENDAR_TO_DATE_ALL_SCH_LIST,
    CALENDAR_TO_DATE_DAILY_CALENDAR,
    CALENDAR_TO_DATE_MAX
}CALENDAR_TO_DATE_TYPE_E;

typedef enum
{
	GUI_CALENDAR_NEED_NOT_REDRAW,
	GUI_CALENDAR_REDRAW_PART,
    GUI_CALENDAR_REDRAW_ALL

} CALENDAR_REDRAW_MODE_E;

typedef enum
{
	GUI_CALENDAR_MONTH_VIEW,
	GUI_CALENDAR_WEEK_VIEW,

} CALENDAR_VIEW_TYPE_E;

typedef struct
{
    uint16      year;
    uint8       month;
    uint8       day;
}CALE_SOLAR_DATE_T;
/*! \brief calendar control data structrue */
typedef struct
{
    GUI_RECT_T      rect;
    uint16          solar_year;         /*!< year */
    uint8           solar_month;        /*!< month */
    uint8           solar_date;         /*!< date */
    uint16          block_width;        /*!<  block width，get from calculation */
    uint16          block_height;       /*!< block height，get from calculation */
    //BOOLEAN         is_active;          /*!< is Active */
    //BOOLEAN         is_focus;           /*!< is Focus */

    //GUI_RECT_T      title_rect;
    //GUI_RECT_T      week_rect;
    //GUI_RECT_T      day_rect;

    CALENDAR_VIEW_TYPE_E   view_type;
    CALENDAR_REDRAW_MODE_E redraw_mode; /* redraw mode */
    uint8           title_button_index;
    uint16          pre_year;               /* pre year */
    uint8           pre_month;              /* pre month */
    uint8           pre_date;               /* pre date */

    BOOLEAN         is_need_lunar_date;     /* is need lunar date */
    uint8           first_day_of_week;      /* first day of week */

    //uint16          title_height;           /*!< title height */
    //uint16          week_height;            /*!< week height */
    //MMI_IMAGE_ID_T  back_image;             /*!< background picture */
    //GUI_FONT_T      day_font;               /*!< date font */
    //GUI_FONT_T      week_font;              /*!< week font */
    //GUI_FONT_T      title_font;             /*!< title font */
    //GUI_COLOR_T     day_font_color;         /*!< date font color */
    //GUI_COLOR_T     day_highlight_color;    /*!< date highlight color */  
    //GUI_COLOR_T     week_font_color;        /*!< week font color */
    //GUI_COLOR_T     saturday_color;         /*!< saturday font color*/
    //GUI_COLOR_T     sunday_color;           /*!< sunday font color */   
    //GUI_COLOR_T     title_font_color;       /*!< title font color */
    //GUI_COLOR_T     title_back_color;       /*!< title background color */
    //GUI_COLOR_T     week_back_color;        /*!<  week background color */
    //GUI_COLOR_T     day_back_color;         /*!< datebackground color */
	MMI_WIN_ID_T win_id;
    uint16          cur_dis_event_id;
}MMI_CALENDAR_T;

typedef enum
{
    MOVE_PREV_DAY,
    MOVE_NEXT_DAY,
    MOVE_PREV_WEEK,
    MOVE_NEXT_WEEK,
    MOVE_PREV_MONTH,
    MOVE_NEXT_MONTH,
    MOVE_PREV_YEAR,
    MOVE_NEXT_YEAR,
    MOVE_MODE_MAX
}CALENDAR_MOVE_MODE_E;

#ifdef MMI_CALENDAR_SLIDE
#ifndef MMI_CALENDAR_PLAY_EFFECT
#define MMI_CALENDAR_PLAY_EFFECT
#endif
#endif

#ifdef MMI_CALENDAR_SLIDE
typedef enum
{
    MOVE_NONE,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN
}CALENDAR_SLIDE_DIRECTION;

typedef struct
{
    MMK_TP_SLIDE_E              slide_state;                            //current slide state
    //MMK_TP_SLIDE_E              tpdown_slide_state;                     //slide state when tp down
    GUI_POINT_T                 slide_start_point;                      //Slide时候使用
    GUI_POINT_T                 slide_pre_point;                        //Slide时候使用
    uint8                       redraw_timer_id;
    uint8                       fling_timer_id;
    uint8                       recover_timer_id;
    MMI_HANDLE_T                handle;
    BOOLEAN                     is_second_layer_draw;
    BOOLEAN                     is_first_layer_draw;
    MMI_CALENDAR_T*             calendar_ctrl_ptr;
    CALENDAR_SLIDE_DIRECTION    direction; 
    float                       velocity;
    CALENDAR_MOVE_MODE_E        move_mode;
    CALENDAR_VIEW_TYPE_E        view_type;
}MMI_SLIDE_CALENDAR_T;
#endif

#ifdef MMI_CALENDAR_PLAY_EFFECT
#define  MMI_CALENDAR_PLAY_FRAME_NUM  5
#endif

typedef struct
{
    MMI_WIN_ID_T    win_id;
    GUI_LCD_DEV_INFO* dev_info;
    //uint16          year;         
    //uint8           month;       
    //uint8           day;        
    CALE_SOLAR_DATE_T   date;
    int16           cur_offset_y;
    GUI_RECT_T      item_rect;
    uint16          max_dis_item_num;
    uint16          item_height;
    int16           down_index;
    int16           focus_index;
#ifdef MMI_CALENDAR_SLIDE
    MMI_SLIDE_CALENDAR_T    slide_calendar;
#endif
    
}MMI_DAILY_CALENDAR_T;
/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : open calendar main window
//	Global resource dependence :
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICALEND_OpenMainWin(void);

/*****************************************************************************/
// 	Description : open calendar query by date win
//	Global resource dependence :
//  Author: xiangjei
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICALEND_OpenCalendarQueryByDate(ADD_DATA add_data_ptr);

/*****************************************************************************/
// 	Description : delete current day all schedule
//	Global resource dependence : 
//  Author: xiangjie
//	Note:   
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPICALENDAR_DelCurDaySch(BOOLEAN is_del_periodsch);

/*****************************************************************************/
//    Description : enter calendar by idle
//    Global resource dependence : 
//    Author: zhaohui
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPICALE_EnterTodyScheduleList(void);

/*****************************************************************************/
//    Description : free calendar resouce
//    Global resource dependence : 
//    Author: zhaohui
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPICALENDAR_FreeRes(void);

/*****************************************************************************/
//    Description : MMIAPICALENDAR_SetCurDayHasPeriodSch
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPICALENDAR_SetCurDayHasPeriodSch(BOOLEAN is_has_periodsch);

/*****************************************************************************/
//    Description : MMIAPICALENDAR_GetCurDayHasPeriodSch
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICALENDAR_GetCurDayHasPeriodSch( void );

/*****************************************************************************/
//    Description : MMIAPICALENDAR_SetIfDelPeriodSch
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPICALENDAR_SetIfDelPeriodSch(BOOLEAN is_del_periodsch);

/*****************************************************************************/
//    Description : MMIAPICALENDAR_GetIfDelPeriodSch
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICALENDAR_GetIfDelPeriodSch( void );

/*****************************************************************************/
// 	Description : check if cur day has period sch or other sch
//	Global resource dependence : 
//  Author: 
//	Note:   
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPICALENDAR_IsCurDayHasAppointSch(CALENDAR_DEL_SCH_TYPE_E sch_type );

#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
/*****************************************************************************/
// 	Description : delete current day all schedule
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
PUBLIC  void MMIAPICALENDAR_OpenLunarDetailWin(ADD_DATA add_data_ptr);
#endif
/**---------------------------------------------------------------------------*
 **                    Old Interface  Macro Declaration    				      *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define  MMICALEND_OpenMainWin                              MMIAPICALEND_OpenMainWin
#define  MMICALEND_OpenCalendarQueryByDate                 MMIAPICALEND_OpenCalendarQueryByDate
#define  MMICALENDAR_DelCurDaySch                           MMIAPICALENDAR_DelCurDaySch
#define  MMICALE_EnterTodyScheduleList                     MMIAPICALE_EnterTodyScheduleList
#define  MMICALENDAR_FreeRes                                MMIAPICALENDAR_FreeRes
#define  MMICALENDAR_SetCurDayHasPeriodSch                MMIAPICALENDAR_SetCurDayHasPeriodSch
#define  MMICALENDAR_GetCurDayHasPeriodSch                MMIAPICALENDAR_GetCurDayHasPeriodSch
#define  MMICALENDAR_SetIfDelPeriodSch                    MMIAPICALENDAR_SetIfDelPeriodSch
#define  MMICALENDAR_GetIfDelPeriodSch                    MMIAPICALENDAR_GetIfDelPeriodSch
#define  MMICALENDAR_IsCurDayHasAppointSch               MMIAPICALENDAR_IsCurDayHasAppointSch
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
#define  MMICALENDAR_OpenLunarDetailWin                  MMIAPICALENDAR_OpenLunarDetailWin
#endif

#ifdef MMI_PDA_SUPPORT
#define  MMICALENDAR_GetGridIstyleInterval              MMIAPICALENDAR_GetGridIstyleInterval
#define  MMICALENDAR_GetBlockHeight                      MMIAPICALENDAR_GetBlockHeight
#define  MMICALENDAR_GetWeeklySelectHeight              MMIAPICALENDAR_GetWeeklySelectHeight
#endif

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

#ifdef ATEST_SUPPORT
/*****************************************************************************/
//    Description : get the date in calendar window
//    Global resource dependence : 
//    Author: lin.chen
//    Note:
/*****************************************************************************/
PUBLIC void ATESTCALENDAR_GetCurDate(uint8 *date,uint8 *month,uint16 *year);

/*****************************************************************************/
//    Description : get the rect of date
//    Global resource dependence : 
//    Author:  lin.chen
//    Note:
/*****************************************************************************/
PUBLIC void ATESTCALENDAR_GetDateRect(uint8 date,uint8 month,uint16 year,GUI_RECT_T *rect);
#endif	//ATEST_SUPPORT

/*****************************************************************************/
//    Description : MMIAPICALENDAR_IsOpenCalendar
//    Global resource dependence : 
//    Author: 
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICALENDAR_IsOpenCalendar(void);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


    
#endif  //_MMICALENDAR_MAIN_H_
#endif

