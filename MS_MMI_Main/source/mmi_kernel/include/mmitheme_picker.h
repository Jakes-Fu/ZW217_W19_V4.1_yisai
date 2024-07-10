/***************************************************************************
** File Name:      mmitheme_picker.h                                       *
** Author:         hua.fang                                                *
** Date:           08/17/2012                                              *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2012        hua.fang         Create                                  *
**																		   *
****************************************************************************/
#ifndef _MMITHEME_PICKER_H_
#define _MMITHEME_PICKER_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "guifont.h"
#include "mmi_font.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef _cplusplus
	extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

#define MMITHEME_BASEPICKER_TPSCROLL_TIME               100     // picker在TP移动时重绘间隔
#define MMITHEME_BASEPICKER_FLING_TIME                  10      // 滑动状态时的定时器间隔
#define MMITHEME_BASEPICKER_RECOVER_TIME                100     // 恢复原位时的定时器间隔
#define MMITHEME_BASEPICKER_RECOVER_DISTANCE            5       // 恢复原位的步长

#define MMITHEME_BASEPICKER_MAX_FIELD_COUNT             MMITHEME_TIMEPICKER_FIELD_COUNT

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
// 触摸域
//  具体的值放在XXX_START和XXX_END之间

// field for picker
typedef enum
{
    MMITHEME_PICKER_FIELD_START  = 0,                                           // picker开始值
    MMITHEME_PICKER_FIELD        = MMITHEME_PICKER_FIELD_START,
    MMITHEME_PICKER_FIELD_END,

    MMITHEME_PICKER_FIELD_COUNT  = MMITHEME_PICKER_FIELD_END,                   // picker总个数
} MMITHEME_PICKER_FIELD_E;

// field for time picker
//  分为了12小时制,24小时制和定时器风格，它们被分在一个段里，维护自己的值
typedef enum
{
    // 24hour
    MMITHEME_TIMEPICKER_FIELD_24H_START     = 0,                                //  24小时制的开始值
    MMITHEME_TIMEPICKER_FIELD_24H_HOUR      = MMITHEME_TIMEPICKER_FIELD_24H_START,
    MMITHEME_TIMEPICKER_FIELD_24H_MINUTE,
    MMITHEME_TIMEPICKER_FIELD_24H_END,

    // 12hour
    MMITHEME_TIMEPICKER_FIELD_12H_START     = MMITHEME_TIMEPICKER_FIELD_24H_END,        //  12小时制的开始值
    MMITHEME_TIMEPICKER_FIELD_12H_HOUR      = MMITHEME_TIMEPICKER_FIELD_12H_START,
    MMITHEME_TIMEPICKER_FIELD_12H_MINUTE,
    MMITHEME_TIMEPICKER_FIELD_12H_AMPM,
    MMITHEME_TIMEPICKER_FIELD_12H_END,

    // timer for HM
    MMITHEME_TIMEPICKER_FIELD_TIMER_START   = MMITHEME_TIMEPICKER_FIELD_12H_END,        // 定时器开始值
    MMITHEME_TIMEPICKER_FIELD_TIMER_HOUR    = MMITHEME_TIMEPICKER_FIELD_TIMER_START,
    MMITHEME_TIMEPICKER_FIELD_TIMER_MINUTE,
    MMITHEME_TIMEPICKER_FIELD_TIMER_END,

    // timer for HMS
    MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_START   = MMITHEME_TIMEPICKER_FIELD_TIMER_END,  // 带秒定时器开始值
    MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_HOUR    = MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_START,
    MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_MINUTE,
    MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_SECOND,
    MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_END,

    MMITHEME_TIMEPICKER_FIELD_COUNT             = MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_END,  // 时间域的总个数
} MMITHEME_TIMEPICKER_FIELD_E;

// field for date picker
typedef enum
{
    MMITHEME_DATEPICKER_FIELD_START         = 0,                                //  日期域的开始值
    MMITHEME_DATEPICKER_FIELD_YEAR          = MMITHEME_DATEPICKER_FIELD_START,
    MMITHEME_DATEPICKER_FIELD_MONTH,
    MMITHEME_DATEPICKER_FIELD_DAY,
    MMITHEME_DATEPICKER_FIELD_END,

    MMITHEME_DATEPICKER_FIELD_COUNT         = MMITHEME_DATEPICKER_FIELD_END,    // 日期域的总个数
} MMITHEME_DATEPICKER_FIELD_E;

// base picker theme
typedef struct
{
    uint16              char_space;             //space between chars,must >=1,include cursor
    uint16              line_height;            // 行高
    uint16              border_width;           // 边线宽度
    uint16              separator_width;        // 分隔线宽度
    uint16              client_width;           // 客户区宽度
    uint16              client_height;          // 客户区高度
    int16               top_margin;             // 内容的顶位置
    int16               bottom_margin;          // 内容的底位置
    GUI_ALIGN_E         align;                  // 对齐方式

    GUI_BG_T            bg;                     //background

    BOOLEAN             is_mask_on_top;         // 蒙层是否在文字之上
    GUI_BG_T            mask_bg;                //tips background

    GUI_FONT_ALL_T      tips_font;              // tips font
    GUI_FONT_ALL_T      tips_font_middle;       // tips font
    GUI_FONT_ALL_T      content_font;           // content font
    GUI_FONT_ALL_T      cur_content_font;       // content font

    MMI_IMAGE_ID_T      separator_image_id;     // 分隔线图ID
    MMI_IMAGE_ID_T      border_image_id;        // 边线图ID
    MMI_IMAGE_ID_T      tips_image_id;          // 分隔线图ID
} MMITHEME_BASEPICKER_T;

// picker theme
typedef struct
{
    uint16              field_width[MMITHEME_PICKER_FIELD_COUNT];      // 各个域的宽度
    MMI_TEXT_ID_T       field_tips[MMITHEME_PICKER_FIELD_COUNT];       // 提示文本
} MMITHEME_PICKER_T;

// date picker theme
typedef struct
{
    uint32              date_style;                                             // GUIEDIT_DATE_STYLE_E
    uint32              year_range;                                             // 年的范围

    uint16              field_width[MMITHEME_DATEPICKER_FIELD_COUNT];           // 各个域的宽度
    MMI_TEXT_ID_T       field_tips[MMITHEME_DATEPICKER_FIELD_COUNT];            // 提示文本
} MMITHEME_DATEPICKER_T;

// time picker theme
typedef struct
{
    uint32              time_style;                                             //GUIEDIT_TIME_STYLE_E
    uint32              display_style;                                          //GUIEDIT_TIME_DISPLAY_STYLE_E

    uint16              field_width[MMITHEME_TIMEPICKER_FIELD_COUNT];      // 各个域的宽度
    MMI_TEXT_ID_T       field_tips[MMITHEME_TIMEPICKER_FIELD_COUNT];       // 提示文本
} MMITHEME_TIMEPICKER_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get theme for base picker
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetBasePickerTheme(
    MMITHEME_BASEPICKER_T    *theme_ptr
    );

/*****************************************************************************/
//  Description : get theme for picker
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetPickerTheme(
    MMITHEME_PICKER_T    *theme_ptr
    );

/*****************************************************************************/
//  Description : get theme for time picker
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetTimePickerTheme(
    MMITHEME_TIMEPICKER_T    *theme_ptr
    );

/*****************************************************************************/
//  Description : get theme for date picker
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetDatePickerTheme(
    MMITHEME_DATEPICKER_T    *theme_ptr
    );

/*****************************************************************************/
//  Description : get date min and max year
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
void MMITHEME_GetDatePickerYearRange(
    uint32    year_range_type,//in:CTRLDATEPICKER_YEAR_RANGE_E
    uint16    *min_year_ptr,  //in/out:
    uint16    *max_year_ptr   //in/out:
    );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif  // _MMITHEME_PICKER_H_
