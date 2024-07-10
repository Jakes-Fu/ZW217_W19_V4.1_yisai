/*****************************************************************************
** File Name:      mmitheme_prgbox.c                                         *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      Jassmine              Creat
******************************************************************************/

#define _MMITHEME_PRGBOX_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "guiblock.h"
#include "mmitheme_prgbox.h"
#include "mmi_image.h"
#include "mmidisplay_data.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifdef MONOCHROME_LCD_SUPPORT 
#define MMITHEME_PROCESS_BG_COLOR   MMI_BLACK_COLOR
#define MMITHEME_PROCESS_FG_COLOR   MMI_WHITE_COLOR
#else
#define MMITHEME_PROCESS_BG_COLOR   MMI_DARK_GRAY_COLOR
#define MMITHEME_PROCESS_FG_COLOR   MMI_GREEN_COLOR
#endif
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#if defined(SCREEN_SHAPE_CIRCULAR) && defined(LOW_MEMORY_SUPPORT)

LOCAL uint32 scroll_bar_list[] = {
        res_common_scroll_bar_00,
        res_common_scroll_bar_01,
        res_common_scroll_bar_02,
        res_common_scroll_bar_03,
        res_common_scroll_bar_04,
        res_common_scroll_bar_05,
        res_common_scroll_bar_06,
        res_common_scroll_bar_07,
        res_common_scroll_bar_08,
        res_common_scroll_bar_09,
        res_common_scroll_bar_10,
        res_common_scroll_bar_11,
        res_common_scroll_bar_12,
        res_common_scroll_bar_13,
        res_common_scroll_bar_14,
        res_common_scroll_bar_15,
        res_common_scroll_bar_16,
        res_common_scroll_bar_17,
        res_common_scroll_bar_18,
        res_common_scroll_bar_19,
        res_common_scroll_bar_20,
        res_common_scroll_bar_21,
        res_common_scroll_bar_22,
        res_common_scroll_bar_23,
        res_common_scroll_bar_24,
        res_common_scroll_bar_25,
        res_common_scroll_bar_26,
        res_common_scroll_bar_27,
        res_common_scroll_bar_28,
        res_common_scroll_bar_29,
        res_common_scroll_bar_30,
        res_common_scroll_bar_31,
        res_common_scroll_bar_32,
        res_common_scroll_bar_33,
        res_common_scroll_bar_34,
        res_common_scroll_bar_35,
        res_common_scroll_bar_36
};
#endif
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

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
                                            MMITHEME_PRGBOX_TYPE_E        theme_type,
                                            MMITHEME_PRGBOX_VER_SCROLL_T   *theme_ptr //in/out:
                                            )
{
    //SCI_ASSERT(PNULL != theme_ptr);/*assert verified*/
    if (PNULL == theme_ptr)
    {
        return;
    }
    
    if( MMITHEME_PRGBOX_DEFAULT_TYPE == theme_type )
    {
#ifndef MMI_RES_LOW_COST
        //set groove arrow
        theme_ptr->up_arrow_id         = IMAGE_SCROLL_UP_ARROW;
        theme_ptr->up_focus_arrow_id   = IMAGE_SCROLL_UP_ARROW_FOCUS;
        theme_ptr->down_arrow_id       = IMAGE_SCROLL_DOWN_ARROW;
        theme_ptr->down_focus_arrow_id = IMAGE_SCROLL_DOWN_ARROW_FOCUS;
#endif
        
        //set scroll bar
        theme_ptr->scroll_bar_id = IMAGE_SCROLL_VER_BAR;
#ifndef MMI_RES_LOW_COST        
        //set groove bg
        theme_ptr->groove_bg_id = IMAGE_SCROLL_VER_GROOVE;
#endif

#if defined(GUIF_PRGBOX_AUTOHIDE)
        theme_ptr->thumb_img_id = IMAGE_SCROLL_THUMB;
        theme_ptr->hide_tick = 1000;
        theme_ptr->out_tick = 100;
        theme_ptr->thumb_threshold = 5;
        theme_ptr->bar_width = 4;
#endif
    }
#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
    else if(MMITHEME_PRGBOX_TYPE_ARC == theme_type)
    {
        theme_ptr->up_arrow_id         = 0;
        theme_ptr->up_focus_arrow_id   = 0;
        theme_ptr->down_arrow_id       = 0;
        theme_ptr->down_focus_arrow_id = 0;
#if defined(SCREEN_SHAPE_CIRCULAR) && defined(LOW_MEMORY_SUPPORT)
        theme_ptr->fg_img_list_ptr= scroll_bar_list;
        theme_ptr->scroll_bar_id = *scroll_bar_list;
        theme_ptr->groove_bg_id = 0;
        theme_ptr->total_arc_degrees = 36;
        theme_ptr->scroll_bar_rect.left = DP2PX_VALUE(216); 
        theme_ptr->scroll_bar_rect.top = DP2PX_VALUE(72);
        theme_ptr->scroll_bar_rect.right = DP2PX_VALUE(239);
        theme_ptr->scroll_bar_rect.bottom = DP2PX_VALUE(167);
#else
        theme_ptr->scroll_bar_id = res_aw_scroll_bar;
        theme_ptr->groove_bg_id = res_aw_scroll_bar_bg;
        theme_ptr->total_arc_degrees = 34;
        theme_ptr->initial_thumb_rect.left = DP2PX_VALUE(220); //arc progressbar thumb initial rect
        theme_ptr->initial_thumb_rect.top = DP2PX_VALUE(78);
        theme_ptr->initial_thumb_rect.right = DP2PX_VALUE(234);
        theme_ptr->initial_thumb_rect.bottom = DP2PX_VALUE(97);
#endif
#if defined(GUIF_PRGBOX_AUTOHIDE)
        theme_ptr->thumb_img_id = IMAGE_SCROLL_THUMB;
        theme_ptr->hide_tick = 1000;
        theme_ptr->out_tick = 100;
        theme_ptr->thumb_threshold = 5;
        theme_ptr->bar_width = 4;
#endif
    }
#endif
    else
    {
#ifndef MMI_RES_LOW_COST       
		//set groove arrow
        theme_ptr->up_arrow_id         = IMAGE_SCROLL_UP_ARROW_2;
        theme_ptr->up_focus_arrow_id   = IMAGE_SCROLL_UP_ARROW_FOCUS_2;
        theme_ptr->down_arrow_id       = IMAGE_SCROLL_DOWN_ARROW_2;
        theme_ptr->down_focus_arrow_id = IMAGE_SCROLL_DOWN_ARROW_FOCUS_2;
        
        //set scroll bar
        theme_ptr->scroll_bar_id = IMAGE_SCROLL_VER_BAR_2;
#ifdef GUIF_PRGBOX_AUTOHIDE
        //set groove bg
        theme_ptr->groove_bg_id = IMAGE_SCROLL_VER_BG;
#else
		theme_ptr->groove_bg_id = IMAGE_SCROLL_VER_BAR_2;
#endif
#endif
    }
}

/*****************************************************************************/
//  Description : get horizontal scroll box theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetHorizontalScrollTheme(
                                              MMITHEME_PRGBOX_TYPE_E       theme_type,
                                              MMITHEME_PRGBOX_HOR_SCROLL_T *theme_ptr //in/out:
                                              )
{
    //SCI_ASSERT(PNULL != theme_ptr);/*assert verified*/
    if (PNULL == theme_ptr)
    {
        return;
    }
    
    if( MMITHEME_PRGBOX_DEFAULT_TYPE == theme_type )
    {
#ifndef MMI_RES_LOW_COST 
        //set groove arrow
        theme_ptr->left_arrow_id        = IMAGE_SCROLL_LEFT_ARROW;
        theme_ptr->left_focus_arrow_id  = IMAGE_SCROLL_LEFT_ARROW_FOCUS;
        theme_ptr->right_arrow_id       = IMAGE_SCROLL_RIGHT_ARROW;
        theme_ptr->right_focus_arrow_id = IMAGE_SCROLL_RIGHT_ARROW_FOCUS;
#endif        
        //set scroll bar
        theme_ptr->scroll_bar_id = IMAGE_SCROLL_VER_BAR;
#ifndef MMI_RES_LOW_COST         
        //set groove bg
        theme_ptr->groove_bg_id = IMAGE_SCROLL_HOR_GROOVE;
#endif
    }
    else
    {
#ifndef MMI_RES_LOW_COST  
		//set groove arrow
        theme_ptr->left_arrow_id        = IMAGE_SCROLL_LEFT_ARROW_2;
        theme_ptr->left_focus_arrow_id  = IMAGE_SCROLL_LEFT_ARROW_FOCUS_2;
        theme_ptr->right_arrow_id       = IMAGE_SCROLL_RIGHT_ARROW_2;
        theme_ptr->right_focus_arrow_id = IMAGE_SCROLL_RIGHT_ARROW_FOCUS_2;
        
        //set scroll bar
        theme_ptr->scroll_bar_id = IMAGE_SCROLL_VER_BAR_2;
        
        //set groove bg
        theme_ptr->groove_bg_id = IMAGE_SCROLL_HOR_GROOVE_2;
#endif
    }
}

/*****************************************************************************/
//  Description : get process box theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetProcessTheme(
                                    MMITHEME_PRGBOX_TYPE_E       theme_type,
                                    MMITHEME_PRGBOX_PROCESS_T     *theme_ptr //in/out:
                                     )
{
    //SCI_ASSERT(PNULL != theme_ptr);/*assert verified*/
    if (PNULL == theme_ptr)
    {
        return;
    }

    //set process color
    theme_ptr->fg_color = MMITHEME_PROCESS_FG_COLOR;
    theme_ptr->bg_color = MMITHEME_PROCESS_BG_COLOR;

#ifdef ADULT_WATCH_SUPPORT
    if(MMITHEME_PRGBOX_TYPE_WIDEBAR_PROGRESS == theme_type)
    {
        theme_ptr->fg_img_id = res_aw_adjust;
        theme_ptr->bg_img_id = res_aw_adjust_bg;
    }
    else
#endif
    {
#ifndef MMI_RES_LOW_COST
        //////////////////////////////////////////////////////////////////////////
        // added by feng.xiao for NEWMS00180461
        // set progress img
        theme_ptr->fg_img_id = IMAGE_CONTROL_PROGRESS;
        theme_ptr->bg_img_id = IMAGE_CONTROL_PROGRESS_BG;
#endif
    }

    theme_ptr->bg_type = GUI_BG_IMG;

    return;
}

/*****************************************************************************/
//  Description : get process box theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetPrgLcdDev(
                                  GUI_LCD_DEV_INFO *lcd_dev_info_ptr//in/out:
                                  )
{
    if (PNULL != lcd_dev_info_ptr)
    {
        //set background lcd dev
        lcd_dev_info_ptr->lcd_id   = GUI_MAIN_LCD_ID;
        lcd_dev_info_ptr->block_id = GUI_BLOCK_MAIN;
    }
    return;
}

/*==============================================================================
Description: 获取prgbox的初始控制宽度
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
1 控制高度根据使用场景定义
2 因为在非自动隐藏的时候，控制宽度和显示宽度是一致的，故而使用默认命名
==============================================================================*/
PUBLIC uint16 MMITHEME_GetScrollBarWidth(void)
{
#if defined(GUIF_PRGBOX_AUTOHIDE)
    return 0;
#else
    return SCROLL_BAR_WIDTH;
#endif
}


/*****************************************************************************/
// 	Description : get the slide win scroll bar width
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetSlideScrollBarWidth(void)
{
#if defined(GUIF_PRGBOX_AUTOHIDE)
    return MMITHEME_SLIDEWIN_SCROLLBAR_WIDTH;
#else
    return MMITHEME_SLIDEWIN_SCROLLBAR_WIDTH;
#endif
}

/*****************************************************************************/
//  Description : get scroll bar width
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetScrollBarWidthByState(
                                                BOOLEAN is_narrow_state
                                                )
{
    uint16      bar_width = 0;

    if (is_narrow_state)
    {
        bar_width = MMITHEME_GetSlideScrollBarWidth();
    }
    else
    {
        bar_width = MMITHEME_GetScrollBarWidth();
    }

    return (bar_width);
}
