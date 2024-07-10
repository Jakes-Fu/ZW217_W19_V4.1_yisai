/*****************************************************************************
** File Name:      mmitheme_prgbox.h                                         *
** Author:                                                                   *
** Date:           04/26/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      Jassmine              Creat
******************************************************************************/

#ifndef _MMITHEME_PRGBOX_H_
#define _MMITHEME_PRGBOX_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guilcd.h"
#include "cafcontrol.h"
#include "sci_types.h"
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

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//滚动条支持多套图片图片
typedef enum
{
    MMITHEME_PRGBOX_DEFAULT_TYPE,
	MMITHEME_PRGBOX_TYPE_1 = MMITHEME_PRGBOX_DEFAULT_TYPE,
    MMITHEME_PRGBOX_TYPE_2,
	MMITHEME_PRGBOX_TYPE_VOLUME,
    MMITHEME_PRGBOX_TYPE_WIDEBAR_PROGRESS,
#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
    //MMITHEME_PRGBOX_TYPE_ARC,
#endif
    MMITHEME_PRGBOX_TYPE_ARC,
    MMITHEME_PRGBOX_TYPE_MAX

}MMITHEME_PRGBOX_TYPE_E;

//the vertical scroll box theme
typedef struct
{
    uint32              up_arrow_id;            //scroll groove up arrow image id
    uint32              up_focus_arrow_id;      //focus scroll groove up arrow image id
    uint32              down_arrow_id;          //scroll groove down arrow image id
    uint32              down_focus_arrow_id;    //focus scroll groove down arrow image id
    uint32              scroll_bar_id;          //scroll bar image id
    uint32              groove_bg_id;           //scroll groove background image id
#if defined(GUIF_PRGBOX_AUTOHIDE)
    uint32              thumb_img_id;           //滑块图片的id
    uint16              hide_tick;              //自动隐藏启动的毫秒数
    uint16              out_tick;               //自动隐藏动画的帧间隔
    uint8               thumb_threshold;        //滚动条出拖动滑块的页数阈值
    uint8               bar_width;              //滚动条提示条的宽度
#endif
#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
    uint32              total_arc_degrees;      //arc style vertical progress bar total degrees.
#if defined(SCREEN_SHAPE_CIRCULAR) && defined(LOW_MEMORY_SUPPORT)
    uint32              *fg_img_list_ptr;
    GUI_RECT_T      scroll_bar_rect;

#else
    GUI_RECT_T          initial_thumb_rect;
#endif
#endif
} MMITHEME_PRGBOX_VER_SCROLL_T;

//the horizontal scroll box theme
typedef struct
{
    uint32              left_arrow_id;          //scroll groove left arrow image id
    uint32              left_focus_arrow_id;    //focus scroll groove left arrow image id
    uint32              right_arrow_id;         //scroll groove right arrow image id
    uint32              right_focus_arrow_id;   //focus scroll groove right arrow image id
    uint32              scroll_bar_id;          //scroll bar image id
    uint32              groove_bg_id;           //scroll groove background image id
} MMITHEME_PRGBOX_HOR_SCROLL_T;

//the process box theme
typedef struct
{
    GUI_BG_TYPE_E       bg_type;
    //////////////////////////////////////////////////////////////////////////
    // fixed by feng.xiao for NEWMS00180461
    MMI_IMAGE_ID_T      bg_img_id;
    MMI_IMAGE_ID_T      fg_img_id;
    GUI_COLOR_T         fg_color;               //foreground color
    GUI_COLOR_T         bg_color;               //background color
    GUI_COLOR_T         bg_rect_color;          //background color
    GUI_COLOR_T         focus_other_color;      
    GUI_COLOR_T         unfocus_fg_color;       
    GUI_COLOR_T         text_color;
	GUI_COLOR_T         label_text_focus_color;
	MMI_TEXT_ID_T       text1_id;
	MMI_TEXT_ID_T       text2_id;
	MMI_TEXT_ID_T       text3_id;
    GUI_FONT_T          title_font;
	GUI_FONT_T          text_font;
    BOOLEAN             is_dialog;    
    MMI_IMAGE_ID_T      waiting_indicator_focused_img_id;    
    MMI_IMAGE_ID_T      waiting_indicator_unfocused_img_id;

} MMITHEME_PRGBOX_PROCESS_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get vertical scroll box theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetVerticalScrollTheme(
                                            MMITHEME_PRGBOX_TYPE_E         theme_type,
                                            MMITHEME_PRGBOX_VER_SCROLL_T   *theme_ptr //in/out:
                                            );

/*****************************************************************************/
//  Description : get horizontal scroll box theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetHorizontalScrollTheme(
                                              MMITHEME_PRGBOX_TYPE_E       theme_type,
                                              MMITHEME_PRGBOX_HOR_SCROLL_T *theme_ptr //in/out:
                                              );

/*****************************************************************************/
//  Description : get process box theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetProcessTheme(
                                     MMITHEME_PRGBOX_TYPE_E       theme_type,
                                     MMITHEME_PRGBOX_PROCESS_T     *theme_ptr //in/out:
                                     );

/*****************************************************************************/
//  Description : get process box theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetPrgLcdDev(
                                  GUI_LCD_DEV_INFO *lcd_dev_info_ptr//in/out:
                                  );

/*==============================================================================
Description: 获取prgbox的初始控制宽度
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
1 控制高度根据使用场景定义
2 因为在非自动隐藏的时候，控制宽度和显示宽度是一致的，故而使用默认命名
==============================================================================*/
PUBLIC uint16 MMITHEME_GetScrollBarWidth(void);

/*****************************************************************************/
// 	Description : get the slide win scroll bar width
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetSlideScrollBarWidth(void);

/*****************************************************************************/
//  Description : get scroll bar width
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetScrollBarWidthByState(
                                               BOOLEAN is_narrow_state
                                               );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
