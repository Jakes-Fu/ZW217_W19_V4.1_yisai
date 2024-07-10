/*****************************************************************************
** File Name:      mmiwidget_clock.c                                        *
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

#define _MMIWIDGET_CLOCK_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"

#ifdef MMI_WIDGET_DUAL_CLOCK
#include "window_parse.h"
#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
//#include "mmicc_text.h"
#include "mmiwidget.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "guires.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmitheme_widget.h"
#include "mmi_appmsg.h"
#include "mmiidle_statusbar.h"
#include "mmiset_export.h"
#include "mmiwidget_position.h"
#include "mmiidle_export.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define MAX_MMIWIDGET_CLOCK_TEXT_LEN              50


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL GUI_RECT_T  s_time_rect={0};

LOCAL GUI_RECT_T  s_date_rect={0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 获得mp3 widget上的文本风格
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void GetClockWidgetTextStyle(
                            GUISTR_STYLE_T *time_style_ptr,
                            GUISTR_STYLE_T *date_style_ptr,
                            GUISTR_STYLE_T *week_style_ptr
                            );

/*****************************************************************************/
//     Description : Handle WidgetClock Win Msg
//    Global resource dependence : 
//  Author:bin.ji
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetClockWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );


/*****************************************************************************/
//  Description : display clock
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void DisplayClock(
                       MMI_WIN_ID_T win_id
                       );

LOCAL MMI_RESULT_E  MMIWIDGET_Clock_HandleTpPressUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/

PUBLIC const MMIWIDGET_ITEM_INFO_T g_clock_widget =
{
    MMIWIDGET_CLOCK_ID, 
    HandleWidgetClockWinMsg, 
    PNULL, 
    PNULL,
    PNULL,
    WIDGET_CLOCK_WIN_ID, 
    IMAGE_WIDGET_DIGITALCLOCK_SHORTCUT, 
    IMAGE_WIDGET_DIGITALCLOCK,
    TXT_WIDGET_CLOCK,
    50,//    75, 
    50,//    75
    FALSE
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : Handle WidgetClock Win Msg
//    Global resource dependence : 
//  Author:bin.ji
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetClockWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    switch (msg_id)
    {
    case MSG_WIDGET_TP_PRESS_UP:
           recode= MMIWIDGET_Clock_HandleTpPressUp(win_id,param);
           break;
        
    case MSG_IDLE_UPDATE_DATETIME:
    case MSG_FULL_PAINT:
        DisplayClock(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;        
}

/*****************************************************************************/
//  Description : get time string
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void GetDisplayTimeString(
                                wchar *time_wstr, 
                                uint16 wlen
                                )
{
    MAIN_StatusbarGetSysTime(time_wstr, wlen);
}

/*****************************************************************************/
//  Description : display clock
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void DisplayClock(
                       MMI_WIN_ID_T win_id
                       )
{
    GUI_RECT_T win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
    GUI_RECT_T img_rect = {0};
    //SCI_TIME_T                  sys_time        = {0};
    SCI_DATE_T                  sys_date        = {0};
    GUISTR_STYLE_T time_text_style = {0};
    GUISTR_STYLE_T date_text_style = {0};
    GUISTR_STYLE_T week_text_style = {0};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;
    //char u8_time_str[MAX_MMIWIDGET_CLOCK_TEXT_LEN] = {0};
    char u8_date_str[MAX_MMIWIDGET_CLOCK_TEXT_LEN] = {0};
    MMI_STRING_T time_str = {0};
    MMI_STRING_T date_str = {0};
    MMI_STRING_T week_str = {0};
    GUI_RECT_T time_rect = {0};
    GUI_RECT_T date_rect = {0};
    GUI_RECT_T week_rect = {0};
    // GUI_POINT_T point = {0};
    MMI_TEXT_ID_T       week_text[] = {TXT_IDLE_SHORT_SUNDAY,
                                        TXT_IDLE_SHORT_MONDAY,
                                        TXT_IDLE_SHORT_TUESDAY,
                                        TXT_IDLE_SHORT_WEDNESDAY,
                                        TXT_IDLE_SHORT_THURSDAY,
                                        TXT_IDLE_SHORT_FRIDAY,
                                        TXT_IDLE_SHORT_SATURDAY};
     uint16 image_width =0;
     uint16 image_height =0;	
    GUISTR_INFO_T	 str_info={0};
    //显示数字时钟背景
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    img_rect.left = 0;
    img_rect.top = 0;
    img_rect.right = win_rect.right+1-win_rect.left;
    img_rect.bottom = win_rect.bottom+1-win_rect.top;        
    GUIRES_DisplayImg(
        PNULL,
        &win_rect,
        &img_rect,
        win_id,
        IMAGE_WIDGET_DIGITALCLOCK,
        &lcd_dev_info
        );    
    GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_WIDGET_DIGITALCLOCK,  win_id);
    //get system time
    //TM_GetSysTime(&sys_time);    
    //get system time
    TM_GetSysDate(&sys_date);  
    
    
    time_str.wstr_ptr = (wchar*)SCI_ALLOC_APP((MAX_MMIWIDGET_CLOCK_TEXT_LEN+1)*sizeof(wchar));
    SCI_MEMSET(time_str.wstr_ptr,0,((MAX_MMIWIDGET_CLOCK_TEXT_LEN+1)*sizeof(wchar)));  
    date_str.wstr_ptr = (wchar*)SCI_ALLOC_APP((MAX_MMIWIDGET_CLOCK_TEXT_LEN+1)*sizeof(wchar));
    SCI_MEMSET(date_str.wstr_ptr,0,((MAX_MMIWIDGET_CLOCK_TEXT_LEN+1)*sizeof(wchar)));  
    
    //get time type
    GetDisplayTimeString(time_str.wstr_ptr, (MAX_MMIWIDGET_CLOCK_TEXT_LEN - 1));
    time_str.wstr_len = MMIAPICOM_Wstrlen(time_str.wstr_ptr);
    
    GetClockWidgetTextStyle(&time_text_style, &date_text_style, &week_text_style);
    //显示时间
    GUISTR_GetStringInfo(&time_text_style, &time_str, text_state,&str_info);	
    time_rect.left  = 0;
    time_rect.top = MMIWIDGET_CLOCK_TIME_TOP;
    time_rect.bottom =  time_rect.top + str_info.height -1;
    time_rect.right = time_rect.left + image_width -1;
    s_time_rect = time_rect;
    time_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, time_rect);
    
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &time_rect,
        &time_rect,
        (const MMI_STRING_T     *)&time_str,
        &time_text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
        );
     
    // 显示date：xx-xx
    MMIAPISET_FormatDateStrByDateStyle(sys_date.year, sys_date.mon, sys_date.mday,
        '-', (uint8*)u8_date_str, sizeof(u8_date_str) - 1);
    MMIAPICOM_StrToWstr((const uint8*)u8_date_str, date_str.wstr_ptr);	
    date_str.wstr_len = MMIAPICOM_Wstrlen(date_str.wstr_ptr);

    GUISTR_GetStringInfo(&date_text_style, &date_str, text_state, &str_info);

    date_rect.left  = 0;	
    date_rect.right = date_rect.left + image_width -1;
    date_rect.bottom = image_height - MMIWIDGET_CLOCK_DATE_BOTTOM +1;
    date_rect.top = date_rect.bottom - str_info.height +1;
     
    s_date_rect = date_rect;
    s_date_rect.bottom = image_height - 1;

    date_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, date_rect);

    GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &date_rect,
            &date_rect,
            (const MMI_STRING_T     *)&date_str,
            &date_text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO
            );

    //显示week
    MMI_GetLabelTextByLang(week_text[sys_date.wday], &week_str);
    GUISTR_GetStringInfo(&date_text_style, &week_str, text_state, &str_info);

    week_rect.left = 0;
    week_rect.right = week_rect.left + image_width -1;
    week_rect.bottom = image_height - MMIWIDGET_CLOCK_WEEK_BOTTOM + 1;
    week_rect.top = week_rect.bottom - str_info.height +1;
    week_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, week_rect);
    
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &week_rect,
        &week_rect,
        (const MMI_STRING_T     *)&week_str,
        &date_text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    SCI_FREE(time_str.wstr_ptr);
    SCI_FREE(date_str.wstr_ptr);
    //    SCI_FREE(week_str.wstr_ptr);
}

/*****************************************************************************/
//  Description : 获得mp3 widget上的文本风格
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void GetClockWidgetTextStyle(
                            GUISTR_STYLE_T *time_style_ptr,
                            GUISTR_STYLE_T *date_style_ptr,
                            GUISTR_STYLE_T *week_style_ptr
                            )
{
    MMITHEME_WIDGET_CLOCK_T clock_widget_theme = {0};

    MMITHEME_GetClockWidgetTheme(&clock_widget_theme);    

    //time
    if (PNULL != time_style_ptr)
    {
        time_style_ptr->effect = FONT_EFFECT_NONE; 
        time_style_ptr->angle = ANGLE_0;
        time_style_ptr->align = ALIGN_HVMIDDLE;
        time_style_ptr->font = clock_widget_theme.time_font;
        time_style_ptr->font_color = clock_widget_theme.clock_font_color;
    }
    //date
    if (PNULL != date_style_ptr)
    {
        date_style_ptr->effect = FONT_EFFECT_NONE; 
        date_style_ptr->angle = ANGLE_0;
        date_style_ptr->align = ALIGN_HVMIDDLE;
        date_style_ptr->font = clock_widget_theme.date_font;
        date_style_ptr->font_color = clock_widget_theme.date_font_color;
    }

    //week
    if (PNULL != week_style_ptr)
    {
        week_style_ptr->effect = FONT_EFFECT_NONE; 
        week_style_ptr->angle = ANGLE_0;
        week_style_ptr->align = ALIGN_HVMIDDLE;
        week_style_ptr->font = clock_widget_theme.week_font;
        week_style_ptr->font_color = clock_widget_theme.date_font_color;
    }
}
LOCAL MMI_RESULT_E  MMIWIDGET_Clock_HandleTpPressUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};
    GUI_RECT_T button_time_rect = s_time_rect;
    GUI_RECT_T button_date_rect = s_date_rect;    

     point.x = MMK_GET_TP_X(param);
     point.y = MMK_GET_TP_Y(param);
     point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);

    if (GUI_PointIsInRect(point, button_time_rect))
    {
         MMIAPISET_SetTime();
    }
    else if (GUI_PointIsInRect(point, button_date_rect))
    {
        MMIAPISET_SetDate();
    }

    return recode;
}
#endif
