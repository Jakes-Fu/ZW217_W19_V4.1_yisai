/*****************************************************************************
** File Name:      mmitheme_picker.c                                         *
** Author:         hua.fang                                                  *
** Date:           08/17/2012                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2012        hua.fang          Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmi_image.h"
#include "mmi_text.h"
#include "mmi_theme.h"
#include "mmitheme_picker.h"
#include "ctrltimepicker_export.h"
#include "ctrldatepicker_export.h"
#include "mmidisplay_data.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

// base picker
#if  defined MAINLCD_SIZE_320X480
#define MMITHEME_BASEPICKER_LINE_SPACE              15          // 各行之间的行距
#define MMITHEME_BASEPICKER_BORDER                  0           // 边线宽度
#define MMITHEME_BASEPICKER_SEPARATOR               0           // 分隔线宽度
#define MMITHEME_BASEPICKER_TOP_MARGIN              24          // 内容的顶位置 (IMAGE_EDIT_TOUCH_BORDER.png的客户区)
#define MMITHEME_BASEPICKER_BOTTOM_MARGIN           0           // 内容的底位置
#define MMITHEME_BASEPICKER_CLIENT_WIDTH            194         // 客户区宽度
#define MMITHEME_BASEPICKER_CLIENT_HEIGHT           180         // 客户区高度
#elif defined MAINLCD_SIZE_240X400
#define MMITHEME_BASEPICKER_LINE_SPACE              8           // 各行之间的行距
#define MMITHEME_BASEPICKER_BORDER                  0           // 边线宽度
#define MMITHEME_BASEPICKER_SEPARATOR               0           // 分隔线宽度
#define MMITHEME_BASEPICKER_TOP_MARGIN              22          // 内容的顶位置
#define MMITHEME_BASEPICKER_BOTTOM_MARGIN           0           // 内容的底位置
#define MMITHEME_BASEPICKER_CLIENT_WIDTH            194         // 客户区宽度
#define MMITHEME_BASEPICKER_CLIENT_HEIGHT           160         // 客户区高度
#else
#define MMITHEME_BASEPICKER_LINE_SPACE              7           // 各行之间的行距
#define MMITHEME_BASEPICKER_BORDER                  0           // 边线宽度
#define MMITHEME_BASEPICKER_SEPARATOR               0           // 分隔线宽度
#define MMITHEME_BASEPICKER_TOP_MARGIN              22          // 内容的顶位置
#define MMITHEME_BASEPICKER_BOTTOM_MARGIN           0           // 内容的底位置
#define MMITHEME_BASEPICKER_CLIENT_WIDTH            194         // 客户区宽度
#define MMITHEME_BASEPICKER_CLIENT_HEIGHT           116         // 客户区高度
#endif

// start
// 以下是按百分比来算的，非象素值
// DatePicker
#define MMITHEME_DATEPICKER_YEAR_WIDTH              36          // 年宽度
#define MMITHEME_DATEPICKER_MONTH_WIDTH             32          // 月
#define MMITHEME_DATEPICKER_DAY_WIDTH               32          // 日

// TimePicker
#define MMITHEME_TIMEPICKER_24H_HOUR_WIDTH          50          // 24小时制的小时宽度
#define MMITHEME_TIMEPICKER_24H_MINUTE_WIDTH        50          //           分钟

#define MMITHEME_TIMEPICKER_12H_HOUR_WIDTH          32          // 12小时制的小时宽度
#define MMITHEME_TIMEPICKER_12H_MINUTE_WIDTH        32          //           分钟
#define MMITHEME_TIMEPICKER_12H_AMPM_WIDTH          36          //           AMPM

#define MMITHEME_TIMEPICKER_TIMER_HOUR_WIDTH        50          // 定时器的小时宽度
#define MMITHEME_TIMEPICKER_TIMER_MINUTE_WIDTH      50          //         分钟

#define MMITHEME_TIMEPICKER_TIMER_HMS_HOUR_WIDTH    34          // 定时器HMS格式的小时宽度
#define MMITHEME_TIMEPICKER_TIMER_HMS_MINUTE_WIDTH  33          // 月
#define MMITHEME_TIMEPICKER_TIMER_HMS_SECOND_WIDTH  33          // 日

// picker
#define MMITHEME_PICKER_WIDTH                       100         // PICKER
//end

#define MMITHEME_DATEPICKER_YEAR_SET_MIN            1980
#define MMITHEME_DATEPICKER_YEAR_SET_MAX            2099

#define MMITHEME_DATEPICKER_YEAR_QUERY_MIN          1950
#define MMITHEME_DATEPICKER_YEAR_QUERY_MAX          2099

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get theme for base picker
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetBasePickerTheme(
    MMITHEME_BASEPICKER_T    *theme_ptr
    )
{
    //SCI_ASSERT(PNULL != common_data_ptr);       /*assert verified*/
    if(PNULL == theme_ptr)
    {
        return;
    }

    theme_ptr->is_mask_on_top = FALSE;

    theme_ptr->char_space = 2;
    theme_ptr->border_width = MMITHEME_BASEPICKER_BORDER;
    theme_ptr->separator_width = MMITHEME_BASEPICKER_SEPARATOR;
    theme_ptr->client_width = MMITHEME_BASEPICKER_CLIENT_WIDTH;
    theme_ptr->client_height = MMITHEME_BASEPICKER_CLIENT_HEIGHT;
    theme_ptr->top_margin = MMITHEME_BASEPICKER_TOP_MARGIN;
    theme_ptr->bottom_margin = MMITHEME_BASEPICKER_BOTTOM_MARGIN;
    theme_ptr->align = ALIGN_HVMIDDLE;
    
    theme_ptr->tips_font.font = MMI_DEFAULT_SMALL_FONT;
    theme_ptr->tips_font.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_PICKER_TIPS);
    
    theme_ptr->tips_font_middle.font = MMI_DEFAULT_SMALL_FONT;
    theme_ptr->tips_font_middle.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_PICKER_CUR);
    
    theme_ptr->content_font.font = MMI_DEFAULT_NORMAL_FONT;
    theme_ptr->content_font.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_PICKER_NORMAL);
    
    theme_ptr->cur_content_font.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_PICKER_CUR);
    theme_ptr->cur_content_font.font = MMI_DEFAULT_BIG_FONT;
    
    theme_ptr->line_height = MMITHEME_BASEPICKER_LINE_SPACE + theme_ptr->content_font.font;

#ifndef TOUCHPANEL_TYPE_NONE
    // bg
    theme_ptr->bg.bg_type = GUI_BG_COLOR;     // 暂时用单色来填充

    theme_ptr->bg.color = MMI_BLACK_COLOR;    //全背景颜色  //Bug - 1998774

    theme_ptr->bg.shape = GUI_SHAPE_RECT;
    //theme_ptr->bg.img_id = IMAGE_EDIT_TOUCH_BG;
    
    // mask
    //theme_ptr->mask_bg.bg_type = GUI_BG_IMG;
    //theme_ptr->mask_bg.img_id = IMAGE_EDIT_TOUCH_MASK;
    theme_ptr->mask_bg.bg_type = GUI_BG_COLOR;
    theme_ptr->mask_bg.shape = GUI_SHAPE_RECT;
#ifdef SCREEN_SHAPE_CIRCULAR
    theme_ptr->mask_bg.color = MMI_BLACK_COLOR;
#else
    theme_ptr->mask_bg.color = 0x9cd3;    // 暂时用固定颜色，到时通过配置走//MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);//Bug - 1998774
#endif
    // tips
    theme_ptr->tips_image_id = IMAGE_EDIT_TOUCH_TIPS_BG;
    
    // separator
    //theme_ptr->separator_image_id = IMAGE_EDIT_TOUCH_SEPARATOR;
    
    // border
    //theme_ptr->border_image_id = IMAGE_EDIT_TOUCH_BORDER;
#endif
}

/*****************************************************************************/
//  Description : get theme for time picker
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetTimePickerTheme(
    MMITHEME_TIMEPICKER_T    *theme_ptr
    )
{
    if (PNULL == theme_ptr)
    {
        return;
    }

    theme_ptr->time_style        = CTRLTIMEPICKER_STYLE_24;
    theme_ptr->display_style     = CTRLTIMEPICKER_DISPLAY_STYLE_HM;
    
    theme_ptr->field_width[MMITHEME_TIMEPICKER_FIELD_24H_HOUR]    = MMITHEME_TIMEPICKER_24H_HOUR_WIDTH;
    theme_ptr->field_width[MMITHEME_TIMEPICKER_FIELD_24H_MINUTE]  = MMITHEME_TIMEPICKER_24H_MINUTE_WIDTH;
    
    theme_ptr->field_width[MMITHEME_TIMEPICKER_FIELD_12H_HOUR]    = MMITHEME_TIMEPICKER_12H_HOUR_WIDTH;
    theme_ptr->field_width[MMITHEME_TIMEPICKER_FIELD_12H_MINUTE]  = MMITHEME_TIMEPICKER_12H_MINUTE_WIDTH;
    theme_ptr->field_width[MMITHEME_TIMEPICKER_FIELD_12H_AMPM]    = MMITHEME_TIMEPICKER_12H_AMPM_WIDTH;
    
    theme_ptr->field_width[MMITHEME_TIMEPICKER_FIELD_TIMER_HOUR]   = MMITHEME_TIMEPICKER_TIMER_HOUR_WIDTH;
    theme_ptr->field_width[MMITHEME_TIMEPICKER_FIELD_TIMER_MINUTE] = MMITHEME_TIMEPICKER_TIMER_MINUTE_WIDTH;
    
    theme_ptr->field_width[MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_HOUR]   = MMITHEME_TIMEPICKER_TIMER_HMS_HOUR_WIDTH;
    theme_ptr->field_width[MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_MINUTE] = MMITHEME_TIMEPICKER_TIMER_HMS_MINUTE_WIDTH;
    theme_ptr->field_width[MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_SECOND] = MMITHEME_TIMEPICKER_TIMER_HMS_SECOND_WIDTH;
    
    theme_ptr->field_tips[MMITHEME_TIMEPICKER_FIELD_24H_HOUR]    = TXT_TIME_HOUR;
    theme_ptr->field_tips[MMITHEME_TIMEPICKER_FIELD_24H_MINUTE]  = TXT_TIME_MINUTE;
    
    theme_ptr->field_tips[MMITHEME_TIMEPICKER_FIELD_12H_HOUR]    = TXT_TIME_HOUR;
    theme_ptr->field_tips[MMITHEME_TIMEPICKER_FIELD_12H_MINUTE]  = TXT_TIME_MINUTE;
    theme_ptr->field_tips[MMITHEME_TIMEPICKER_FIELD_12H_AMPM]    = 0;
    
    theme_ptr->field_tips[MMITHEME_TIMEPICKER_FIELD_TIMER_HOUR]   = TXT_TIME_HOUR;
    theme_ptr->field_tips[MMITHEME_TIMEPICKER_FIELD_TIMER_MINUTE] = TXT_TIME_MINUTE;
    
    theme_ptr->field_tips[MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_HOUR]   = TXT_TIME_HOUR;
    theme_ptr->field_tips[MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_MINUTE] = TXT_TIME_MINUTE;
    theme_ptr->field_tips[MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_SECOND] = TXT_TIME_SECOND;
}

/*****************************************************************************/
//  Description : get theme for date picker
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetDatePickerTheme(
    MMITHEME_DATEPICKER_T    *theme_ptr
    )
{
    if (PNULL == theme_ptr)
    {
        return;
    }

    theme_ptr->year_range = CTRLDATEPICKER_YEAR_RANGE_SET;
    theme_ptr->date_style = CTRLDATEPICKER_STYLE_YMD;
    
    theme_ptr->field_width[MMITHEME_DATEPICKER_FIELD_YEAR]  = MMITHEME_DATEPICKER_YEAR_WIDTH;
    theme_ptr->field_width[MMITHEME_DATEPICKER_FIELD_MONTH] = MMITHEME_DATEPICKER_MONTH_WIDTH;
    theme_ptr->field_width[MMITHEME_DATEPICKER_FIELD_DAY]   = MMITHEME_DATEPICKER_DAY_WIDTH;
    
    theme_ptr->field_tips[MMITHEME_DATEPICKER_FIELD_YEAR]   = TXT_DATE_YEAR;
    theme_ptr->field_tips[MMITHEME_DATEPICKER_FIELD_MONTH]  = TXT_DATE_MONTH;
    theme_ptr->field_tips[MMITHEME_DATEPICKER_FIELD_DAY]    = TXT_DATE_DAY;
}

/*****************************************************************************/
//  Description : get theme for picker
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetPickerTheme(
    MMITHEME_PICKER_T    *theme_ptr
    )
{
    if (PNULL == theme_ptr)
    {
        return;
    }

    theme_ptr->field_width[MMITHEME_PICKER_FIELD] = MMITHEME_PICKER_WIDTH;
    theme_ptr->field_tips[MMITHEME_PICKER_FIELD] = 0;
}

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
    )
{
    if((PNULL == max_year_ptr) || (PNULL == min_year_ptr))
    {
        return;
    }

    switch ((CTRLDATEPICKER_YEAR_RANGE_E)year_range_type)
    {
    case CTRLDATEPICKER_YEAR_RANGE_SET:
        *min_year_ptr = MMITHEME_DATEPICKER_YEAR_SET_MIN;
        *max_year_ptr = MMITHEME_DATEPICKER_YEAR_SET_MAX;
        break;
        
    case CTRLDATEPICKER_YEAR_RANGE_QUERY:
        *min_year_ptr = MMITHEME_DATEPICKER_YEAR_QUERY_MIN;
        *max_year_ptr = MMITHEME_DATEPICKER_YEAR_QUERY_MAX;
        break;
        
    default:
        SCI_TRACE_LOW("MMITHEME_GetDatePickerYearRange: year range type %d is error!", year_range_type);
        break;
    }
}

