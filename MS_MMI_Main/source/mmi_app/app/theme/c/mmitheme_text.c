/*****************************************************************************
** File Name:      mmitheme_text.c                                           *
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

#define _MMITHEME_EDIT_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "guitext.h"
#include "guiblock.h"
#include "mmitheme_text.h"
#include "mmi_image.h"
#include "mmidisplay_data.h"
#include "mmi_theme.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMITHEME_TEXT_UNDERLINE_COLOR   0xF324

#define MMITHEME_TEXT_SLIDE_TIME        10      //slide timer period
#define MMITHEME_TEXT_RESET_SLIDE_START 12      //reset slide the start frame
#define MMITHEME_TEXT_RESET_SLIDE_FRAME 20      //reset slide frame number

#define MMITHEME_TEXT_SCROLL_PERIOD     500
#define MMITHEME_TEXT_TP_SCROLL_PERIOD  100

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

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get edit common theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetTextTheme(
                           MMITHEME_TEXT_T  *theme_ptr  //in/out:
                           )
{
    //is support slide
    theme_ptr->is_support_slide = TRUE;
    
    //set circular handle key msg
    theme_ptr->is_circular_handle = TRUE;
    
    //set text handle tp
    theme_ptr->is_handle_tp = TRUE;

    //tp slide time period
    theme_ptr->slide_period = MMITHEME_TEXT_SLIDE_TIME;
    
    //reset silde frame number
    theme_ptr->reset_slide_start = MMK_RECOVER_SLIDE_START_FRAME;
    theme_ptr->reset_slide_frame = MMK_RECOVER_SLIDE_TOTAL_FRAME;

    //set space
    theme_ptr->char_space       = MMITHEME_TEXT_CHAR_SPACE;
    theme_ptr->line_space       = MMITHEME_TEXT_LINE_SPACE;
    theme_ptr->margin_left_space = MMITHEME_TEXT_MARGIN_SPACE;
    theme_ptr->margin_right_space  = MMITHEME_TEXT_MARGIN_SPACE;
    theme_ptr->margin_top_space  = MMITHEME_TEXT_MARGIN_SPACE;
    theme_ptr->margin_bottom_space  = MMITHEME_TEXT_MARGIN_SPACE;

    //set border
    theme_ptr->border.type      = GUI_BORDER_NONE;
    theme_ptr->border.width     = DP2PX_VALUE(1);
    theme_ptr->border.color     = MMI_DARK_GRAY_COLOR;

    //set dividing
    theme_ptr->line_width       = 0;
    theme_ptr->line_color       = MMI_DARK_GRAY_COLOR;

    //set underline
    theme_ptr->underline_color  = MMITHEME_TEXT_UNDERLINE_COLOR;

    //set background
    theme_ptr->bg.bg_type       = GUI_BG_IMG;
    theme_ptr->bg.color         = MMI_BACKGROUND_COLOR;
//@for light function modify
//#if defined MAINLCD_SIZE_240X320 
//    theme_ptr->bg.img_id        = IMAGE_COMMON_BG_LIGHT; 
// #else
    theme_ptr->bg.img_id        = IMAGE_COMMON_BG;
//#endif

    theme_ptr->bg.shape         = GUI_SHAPE_ROUNDED_RECT;

    //set font
    theme_ptr->font.font        = MMI_DEFAULT_TEXT_FONT;
    theme_ptr->font.color       = MMITHEME_GetCurThemeFontColor(MMI_THEME_TEXT_FONT);

    //set scroll
    theme_ptr->scroll_step      = 75;
    theme_ptr->scroll_mode      = GUITEXT_SCROLL_RATIO;
    theme_ptr->scroll_period    = MMITHEME_TEXT_SCROLL_PERIOD;
    theme_ptr->tp_scroll_period = MMITHEME_TEXT_TP_SCROLL_PERIOD;
    
    //set tag type
    theme_ptr->tag_type         = GUITEXT_TAG_NONE;
    
    //set align
    theme_ptr->align            = ALIGN_LEFT;

    theme_ptr->lcd_dev.lcd_id   = GUI_MAIN_LCD_ID;
    theme_ptr->lcd_dev.block_id = GUI_BLOCK_MAIN;

#ifdef MAINLCD_SIZE_128X64
    //set border
    theme_ptr->border.width     = 0;

    //set dividing
    theme_ptr->line_width       = 0;
#endif
#ifdef MONOCHROME_LCD_SUPPORT 
    theme_ptr->line_color       = MMI_WHITE_COLOR;

    //set background
    theme_ptr->bg.color         = MMI_WHITE_COLOR;

    //set font
    theme_ptr->font.color       = MMI_BLACK_COLOR;
#endif

    theme_ptr->drag_mode = MMITHEME_GetDragMode();
    theme_ptr->slider_display_period = CLIPBRD_SLIDER_DIS_PERIOD;
}
