/*****************************************************************************
** File Name:      mmitheme_form.c                                           *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      Jassmine              Creat
******************************************************************************/

#define _MMITHEME_FORM_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_theme_trc.h"
#include "std_header.h"
#include "guiform.h"
#include "mmitheme_form.h"
#include "mmi_theme.h"
#include "mmidisplay_data.h"
#include "mmi_image.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMITHEME_FORM_SLIDE_TIME            10      //slide timer period
#define MMITHEME_FORM_RESET_SLIDE_START     12      //reset slide the start frame
#define MMITHEME_FORM_RESET_SLIDE_FRAME     20      //reset slide frame number

#if defined MAINLCD_SIZE_128X64
#define MMITHEME_FORM_HOR_SPACE     0
#define MMITHEME_FORM_VER_SPACE     0

//image height/width ratio
#define MMITHEME_FORM_IMG_HEIGHT   0
#define MMITHEME_FORM_IMG_WIDTH    0

#define MMITHEME_FORM_INDENT_STEP   0
#else
#define MMITHEME_FORM_HOR_SPACE     5
#define MMITHEME_FORM_VER_SPACE     5

//image height/width ratio
#define MMITHEME_FORM_IMG_HEIGHT    3
#define MMITHEME_FORM_IMG_WIDTH     4

#define MMITHEME_FORM_INDENT_STEP   1
#endif

//padding
#define MMITHEME_FORM_PADDING       1//3

//scroll step/form height ratio
#define MMITHEME_FORM_SCROLL_STEP   4
#define MMITHEME_FORM_SCROLL_ALL    5

#define MMITHEME_FORM_INDENT_ALL    10

#define MMITHEME_CHILD_COLOR            MMI_WHITE_COLOR
#define MMITHEME_GRAY_CHILD_COLOR       MMI_DARK_GRAY_COLOR
#define MMITHEME_CHILD_BORDER_WIDTH     1

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
//  Description : get form theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetFormTheme(
                           MMITHEME_FORM_T  *theme_ptr  //in/out:
                           )
{
    //is circular handle direction key
	theme_ptr->is_cir_handle_ud = FALSE;
    theme_ptr->is_cir_handle_lr = FALSE;

    //is support slide
    theme_ptr->is_support_slide = TRUE;
    
    //tp slide time period
    theme_ptr->slide_period = MMITHEME_FORM_SLIDE_TIME;
    
    //reset slide frame number
    theme_ptr->reset_slide_start = MMK_RECOVER_SLIDE_START_FRAME;
    theme_ptr->reset_slide_frame = MMK_RECOVER_SLIDE_TOTAL_FRAME;

    //set space
    theme_ptr->hor_space    = MMITHEME_FORM_HOR_SPACE;
    theme_ptr->ver_space    = MMITHEME_FORM_VER_SPACE;

    //set bg background
#ifdef ADULT_WATCH_SUPPORT
    theme_ptr->bg.bg_type = GUI_BG_COLOR;//FIX Bug 1617498 
    theme_ptr->bg.color   = 0xD6BA;
#else
    theme_ptr->bg.bg_type = GUI_BG_IMG;
    theme_ptr->bg.color   = MMI_BACKGROUND_COLOR;
#endif
    //@for light function modify
#if defined MAINLCD_SIZE_240X320 
    theme_ptr->bg.img_id  = MMITHEME_GetCommonLightBg().img_id;
#else
    theme_ptr->bg.img_id  = IMAGE_COMMON_BG;
#endif
    
    //set border
    theme_ptr->border.type  = GUI_BORDER_NONE;
    theme_ptr->border.width = 0;
    theme_ptr->border.color = MMI_DARK_GRAY_COLOR;

    //set padding
    theme_ptr->padding.left   = MMITHEME_FORM_PADDING;
    theme_ptr->padding.top    = MMITHEME_FORM_PADDING;
    theme_ptr->padding.right  = MMITHEME_FORM_PADDING;
    theme_ptr->padding.bottom = MMITHEME_FORM_PADDING;

    //set dividing
    theme_ptr->line_width   = 0;
    theme_ptr->line_color   = MMITHEME_GetCurThemeFontColor(MMI_THEME_SPLIT_LINE);
    
    //set image ratio
    theme_ptr->img_ratio.num1 = MMITHEME_FORM_IMG_HEIGHT;
    theme_ptr->img_ratio.num2 = MMITHEME_FORM_IMG_WIDTH;

    //set scroll ratio
    theme_ptr->scroll_ratio.num1 = MMITHEME_FORM_SCROLL_STEP;
    theme_ptr->scroll_ratio.num2 = MMITHEME_FORM_SCROLL_ALL;
    
    //set indent ratio
    theme_ptr->indent_ratio.num1 = MMITHEME_FORM_INDENT_STEP;
    theme_ptr->indent_ratio.num2 = MMITHEME_FORM_INDENT_ALL;

    //set lcd device
    theme_ptr->lcd_dev.lcd_id   = GUI_MAIN_LCD_ID;
    theme_ptr->lcd_dev.block_id = GUI_BLOCK_MAIN;

    //set unit child font,except the first child
    theme_ptr->unit_font.font   = MMI_DEFAULT_SMALL_FONT;
    theme_ptr->unit_font.color  = MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM_SMALL_FONT);

    //set child bg,border,font
    theme_ptr->child.bg.bg_type   = GUI_BG_COLOR;
    theme_ptr->child.bg.color     = MMITHEME_CHILD_COLOR;
#ifdef MONOCHROME_LCD_SUPPORT 
    theme_ptr->child.bg.shape     = GUI_SHAPE_RECT;
    theme_ptr->child.border.type  = GUI_BORDER_SOLID;
    theme_ptr->child.border.color = MMI_WHITE_COLOR;
#else    
    theme_ptr->child.bg.shape     = GUI_SHAPE_RECT;
    theme_ptr->child.border.type  = GUI_BORDER_SOLID;
    theme_ptr->child.border.color = MMITHEME_GRAY_CHILD_COLOR;
#endif
    theme_ptr->child.border.width = MMITHEME_CHILD_BORDER_WIDTH;
    theme_ptr->child.font.font    = MMI_DEFAULT_TEXT_FONT;
    theme_ptr->child.font.color   = MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0);

    //set active child bg,border
    theme_ptr->active_child              = theme_ptr->child;
#ifdef MONOCHROME_LCD_SUPPORT 
    theme_ptr->active_child.border.color = MMI_BLACK_COLOR;
#else
    theme_ptr->active_child.border.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
#endif
    theme_ptr->active_child.font.color   = MMI_BLACK_COLOR;

    //set gray child bg,border
    theme_ptr->gray_child              = theme_ptr->child;
    theme_ptr->gray_child.font.color   = MMITHEME_GRAY_CHILD_COLOR;

    //set form child bg,border
    theme_ptr->form_child.bg.bg_type   = GUI_BG_NONE;
    theme_ptr->form_child.border.type  = GUI_BORDER_NOT_DISPLAY;
    theme_ptr->form_child.border.width = MMITHEME_CHILD_BORDER_WIDTH;

    //set active form child bg,border
    theme_ptr->form_active_child             = theme_ptr->form_child;
    theme_ptr->form_active_child.bg.bg_type  = GUI_BG_COLOR;
    theme_ptr->form_active_child.bg.shape    = GUI_SHAPE_ROUNDED_RECT;
    theme_ptr->form_active_child.bg.color    = MMITHEME_GetCurThemeFontColor(MMI_THEME_FORM_BG);
    
    theme_ptr->form_active_child.border.type  = GUI_BORDER_ROUNDED;
#ifdef MONOCHROME_LCD_SUPPORT 
    theme_ptr->form_active_child.border.color = MMI_BLACK_COLOR;
#else
    theme_ptr->form_active_child.border.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
#endif

    //set gray form child bg,border
    theme_ptr->form_gray_child = theme_ptr->form_child;
}

/*****************************************************************************/
//  Description : modify form theme by type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_ModifyFormTheme(
                              uint32			type,       //in:
                              MMITHEME_FORM_T   *theme_ptr  //in/out:
                              )
{
    switch ((GUIFORM_TYPE_E)type)
    {
    case GUIFORM_TYPE_COMMON:
        //set ver space
        theme_ptr->ver_space = MMITHEME_FORM_VER_SPACE;

        //set dividing line width
        theme_ptr->line_width = 0;

        //set indent ratio
        theme_ptr->indent_ratio.num1 = MMITHEME_FORM_INDENT_STEP;

        //set child
        theme_ptr->child.bg.bg_type   = GUI_BG_COLOR;
        theme_ptr->child.border.type  = GUI_BORDER_ROUNDED;

        //set active child
        theme_ptr->active_child.bg.bg_type  = theme_ptr->child.bg.bg_type;
        theme_ptr->active_child.border.type = theme_ptr->child.border.type;
        theme_ptr->active_child.font.color  = MMI_BLACK_COLOR;

		//set gray child
		theme_ptr->gray_child.bg.bg_type  = theme_ptr->child.bg.bg_type;
        theme_ptr->gray_child.border.type = theme_ptr->child.border.type;

        //set form child
        theme_ptr->form_child.border.type        = GUI_BORDER_NOT_DISPLAY;
        theme_ptr->form_active_child.border.type = GUI_BORDER_ROUNDED;
        theme_ptr->form_gray_child.border.type   = theme_ptr->form_child.border.type;
        break;

    case GUIFORM_TYPE_TP:
        //set ver space
        theme_ptr->ver_space = 1;

        //set dividing line width
        theme_ptr->line_width = 1;

        //set indent ratio
        theme_ptr->indent_ratio.num1 = 0;

        //set child
        theme_ptr->child.bg.bg_type   = GUI_BG_NONE;
        theme_ptr->child.border.type  = GUI_BORDER_NONE;

        //set active child
        theme_ptr->active_child.bg.bg_type  = theme_ptr->child.bg.bg_type;
        theme_ptr->active_child.border.type = theme_ptr->child.border.type;
        theme_ptr->active_child.font.color  = theme_ptr->child.font.color;

		//set gray child
		theme_ptr->gray_child.bg.bg_type  = theme_ptr->child.bg.bg_type;
        theme_ptr->gray_child.border.type = theme_ptr->child.border.type;

        //set active form child
        theme_ptr->form_child.border.type        = GUI_BORDER_NONE;
        theme_ptr->form_active_child.border.type = GUI_BORDER_NONE;
        theme_ptr->form_gray_child.border.type   = theme_ptr->form_child.border.type;
        break;

    default:
        //SCI_TRACE_LOW:"MMITHEME_ModifyFormTheme: form type %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_FORM_266_112_2_18_3_1_59_13,(uint8*)"d",type);
        break;
    }
}
