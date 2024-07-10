/*****************************************************************************
** File Name:      mmitheme_widget.h                                         *
** Author:                                                                   *
** Date:           04/26/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006     bin.ji              Creat
******************************************************************************/

#ifndef _MMITHEME_WIDGET_H_
#define _MMITHEME_WIDGET_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guilcd.h"
#include "mmi_font.h"

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

//页数
#define MMITHEME_WIDGET_PAGE_NUM          3


/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

//shortcut bar的初始化数据
typedef struct 
{
    GUI_RECT_T shortcut_bar_rect;    //shortcut bar的活动区域
    GUI_RECT_T shortcut_bar_bg_rect;    //shortcut bar的背景显示区域   
    GUI_COLOR_T shortcut_bar_color;    //shortcut bar的颜色
    MMI_IMAGE_ID_T shortcut_bar_bg_icon;        //shortcut bar的背景图
}MMIWIDGET_SHORTCUT_BAR_DATA_T;

//shortcut开关按钮的初始化数据
typedef struct 
{
    GUI_RECT_T shortcut_bar_switch_rect;    //shortcut bar switch的区域
    GUI_COLOR_T shortcut_bar_switch_color;    //shortcut bar的颜色
    MMI_IMAGE_ID_T shortcut_bar_switch_close_off_icon;    //shortcut bar的button图标
    MMI_IMAGE_ID_T shortcut_bar_switch_close_on_icon;    //shortcut bar的button图标
    MMI_IMAGE_ID_T shortcut_bar_switch_open_off_icon;    //shortcut bar的button图标
    MMI_IMAGE_ID_T shortcut_bar_switch_open_on_icon;    //shortcut bar的button图标
}MMIWIDGET_SHORTCUT_BAR_SWITCH_DATA_T;

//shortcut的初始化数据
typedef struct 
{
    int16 disp_interval;    //相邻两个shortcut的间隔
    MMIWIDGET_SHORTCUT_BAR_DATA_T bar_data;
    MMIWIDGET_SHORTCUT_BAR_SWITCH_DATA_T switch_data;
}MMIWIDGET_SHORTCUT_DATA_T;


//page的初始化数据
typedef struct 
{
    MMI_IMAGE_ID_T sel_bg_image;    //选中图标
    MMI_IMAGE_ID_T unsel_bg_image;  //非选中图标
    GUI_RECT_T img_disp_rect[MMITHEME_WIDGET_PAGE_NUM];   //图标显示位置
}MMIWIDGET_PAGE_DATA_T;

//the public window theme
typedef struct
{
    uint16                      slide_edge;             //拖动widget到屏幕边缘触发滑动的间隔
    uint16                      slidepage_run_steps;    //滑页效果每次移动像素
    uint16                      slidepage_delay_ticks;  //滑页效果每次移动延时
#ifdef MMIWIDGET_SUPPORT
    MMIWIDGET_SHORTCUT_DATA_T   shortcut_data;
#endif    
    MMIWIDGET_PAGE_DATA_T       page_data;

    GUI_FONT_T	     shortcut_text_font;       // font of string
    GUI_COLOR_T	     shortcut_text_color; // color of font
    MMI_IMAGE_ID_T   shortcut_text_bg;    
} MMITHEME_WIDGET_T;

//the pb widget theme
typedef struct
{
    GUI_COLOR_T rect_color;
    MMI_IMAGE_ID_T bg_img;
    MMI_IMAGE_ID_T default_img;
    MMI_IMAGE_ID_T error_img;
} MMITHEME_WIDGET_PB_T;

//the mp3 widget theme
typedef struct
{
    MMI_IMAGE_ID_T bg_img;
    MMI_IMAGE_ID_T entry_on_img;
    GUI_FONT_T mp3name_font;
    GUI_COLOR_T mp3name_color;
} MMITHEME_WIDGET_MP3_T;

//the clock widget theme
typedef struct
{
    GUI_FONT_T time_font;
    GUI_FONT_T date_font;
    GUI_FONT_T week_font;
    GUI_FONT_T city_font;    
    GUI_COLOR_T clock_font_color;
    GUI_COLOR_T date_font_color;    
    GUI_COLOR_T city_font_color;
} MMITHEME_WIDGET_CLOCK_T;

//the fm widget theme
typedef struct
{
    GUI_FONT_T name_font;
    GUI_COLOR_T name_color;
} MMITHEME_WIDGET_FM_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get public windows theme,alert or query windows
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetWidgetTheme(
                          MMITHEME_WIDGET_T     *theme_ptr
                          );

#ifdef MMIWIDGET_SUPPORT
/*****************************************************************************/
//  Description : get public windows theme,alert or query windows
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetPBWidgetTheme(
                          MMITHEME_WIDGET_PB_T     *theme_ptr
                          );
#endif

/*****************************************************************************/
//  Description : get public windows theme,alert or query windows
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetMp3WidgetTheme(
                          MMITHEME_WIDGET_MP3_T     *theme_ptr
                          );

/*****************************************************************************/
//  Description : get public windows theme,alert or query windows
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetClockWidgetTheme(
                          MMITHEME_WIDGET_CLOCK_T     *theme_ptr
                          );

/*****************************************************************************/
//  Description : get widget dual clock(digital & analog) theme.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetDualClockWidgetTheme(MMITHEME_WIDGET_CLOCK_T  *theme_ptr);

/*****************************************************************************/
//  Description : get widget dual clock(digital & analog) theme.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetADualClockWidgetTheme(MMITHEME_WIDGET_CLOCK_T     *theme_ptr);

/*****************************************************************************/
//  Description : get default widget theme color.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T MMITHEME_GetDefaultWidgetThemeColor(void);

#if defined(FM_SUPPORT)
/*****************************************************************************/
//  Description : get widget fm theme.
//  Global resource dependence : 
//  Author: paul.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetFMWidgetTheme(
                          MMITHEME_WIDGET_FM_T     *theme_ptr
                          );
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
