/*****************************************************************************
** File Name:      mmitheme_iconlist.c                                       *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2007      Jassmine              Creat
******************************************************************************/

#define _MMITHEME_ICONLIST_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmi_theme.h"
#include "mmitheme_iconlist.h"
#include "mmi_text.h"
#include "guiiconlist.h"
#include "mmidisplay_data.h"
#include "mmi_image.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmifmm_text.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMITHEME_ICON_SLIDE_TIME            10      //slide timer period
#define MMITHEME_ICON_RESET_SLIDE_START     12      //reset slide the start frame
#define MMITHEME_ICON_RESET_SLIDE_FRAME     20      //reset slide frame number

#define MMITHEME_ICON_LIST_MARGIN           5       //margin
#define MMITHEME_ICON_LIST_SCROLL_PERIOD    500     //scroll timer

#define MMITHEME_ICON_SHOW_HSPACE           5       //icon display horizontal space
#define MMITHEME_ICON_SHOW_VSPACE           10      //icon display vertical space

#ifdef MAINLCD_SIZE_320X480
#define MMITHEME_FOLDER_SHOW_WIDTH          150     //folder width
#define MMITHEME_FOLDER_SHOW_HEIGHT         130     //folder height
#elif  defined(MAINLCD_SIZE_176X220)
#define MMITHEME_FOLDER_SHOW_WIDTH          74     //folder width
#define MMITHEME_FOLDER_SHOW_HEIGHT         62     //folder height
#elif  defined(MAINLCD_SIZE_128X160)
#define MMITHEME_FOLDER_SHOW_WIDTH          52     //folder width
#define MMITHEME_FOLDER_SHOW_HEIGHT         46     //folder height
#else
#define MMITHEME_FOLDER_SHOW_WIDTH          100     //folder width
#define MMITHEME_FOLDER_SHOW_HEIGHT         90      //folder height
#endif

#define MMITHEME_FOLDER_SHOW_HSPACE         5       //icon display horizontal space
#define MMITHEME_FOLDER_SHOW_VSPACE         10      //icon display vertical space

#define MMITHEME_ICON_FONT_VSPACE           5       //icon and font display vertical space
#define MMITHEME_ICON_BG_COLOR              MMI_GRAY_WHITE_COLOR    //icon bg color

#ifdef GUIF_ICONLIST_PDASTYLE
#define MMITHEME_ICON_FONT_COLOR            MMI_GRAY_WHITE_COLOR    //font color
#else
#ifdef BAIDU_DRIVE_SUPPORT
#define MMITHEME_ICON_FONT_COLOR            MMI_GRAY_WHITE_COLOR    //font color
#else
#define MMITHEME_ICON_FONT_COLOR            (MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0)) //font color
#endif
#endif


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
//  Description : get icon list theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan.ma edit 2011.12
/*****************************************************************************/
void MMITHEME_GetIconlistTheme(
                               MMITHEME_ICONLIST_T  *theme_ptr //in/out:
                               )
{
    //SCI_ASSERT(PNULL != theme_ptr); /*assert verified*/
    if (PNULL == theme_ptr)
    {
        return;
    }

    //is support slide
    theme_ptr->is_support_slide = TRUE;

    //tp slide time period
    theme_ptr->slide_period = MMITHEME_ICON_SLIDE_TIME;
    
    //reset silde frame number
    theme_ptr->reset_slide_start = MMITHEME_ICON_RESET_SLIDE_START;
    theme_ptr->reset_slide_frame = MMITHEME_ICON_RESET_SLIDE_FRAME;

    //set scroll groove width
    if (theme_ptr->is_support_slide)
    {
        theme_ptr->groove_width = MMITHEME_GetSlideScrollBarWidth();
    }
    else
    {
        theme_ptr->groove_width = SCROLL_BAR_WIDTH;
    }

    //set margin
    theme_ptr->vmargin_space = MMITHEME_ICON_LIST_MARGIN;
    theme_ptr->hmargin_space = MMITHEME_ICON_LIST_MARGIN;
	
    //set scroll
    theme_ptr->scroll_pixel     = (uint16)(MMITHEME_ScrollPixelNum());
    theme_ptr->scroll_period    = MMITHEME_ICON_LIST_SCROLL_PERIOD;

    //set icon item
    theme_ptr->icon_item.width  = MMITHEME_ICON_SHOW_WIDTH;
    theme_ptr->icon_item.height = MMITHEME_ICON_SHOW_HEIGHT;
    theme_ptr->icon_item.hspace = MMITHEME_ICON_SHOW_HSPACE;
    theme_ptr->icon_item.vspace = MMITHEME_ICON_SHOW_VSPACE;

    //set folder item
    theme_ptr->folder_item.width  = MMITHEME_FOLDER_SHOW_WIDTH;
    theme_ptr->folder_item.height = MMITHEME_FOLDER_SHOW_HEIGHT;
    theme_ptr->folder_item.hspace = MMITHEME_FOLDER_SHOW_HSPACE;
    theme_ptr->folder_item.vspace = MMITHEME_FOLDER_SHOW_VSPACE;

    //set font
    theme_ptr->font_vspace = MMITHEME_ICON_FONT_VSPACE;
    theme_ptr->font.font   = MMI_DEFAULT_SMALL_FONT;
    theme_ptr->font.color  = MMITHEME_ICON_FONT_COLOR;

    //set icon bg
    theme_ptr->icon_bg.bg_type = GUI_BG_NONE;
    theme_ptr->icon_bg.color   = MMITHEME_ICON_BG_COLOR;
    theme_ptr->icon_bg.shape   = GUI_SHAPE_RECT;

    //set select border
    theme_ptr->select_border.type  = GUI_BORDER_ROUNDED;
    theme_ptr->select_border.width = 2;
    theme_ptr->select_border.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
    
    //set unselect border
    theme_ptr->unselect_border.type  = GUI_BORDER_ROUNDED;
    theme_ptr->unselect_border.width = 2;
    theme_ptr->unselect_border.color = MMITHEME_UNSELECT_BORDER_COLOR;
    
    //set default icon
    theme_ptr->default_icon_id = IMAGE_COMMON_PIC_ICON;
	theme_ptr->error_icon_id   = IMAGE_COMMON_PIC_ERROR;

    //set border
    theme_ptr->border.type  = GUI_BORDER_NONE;
    theme_ptr->border.width = 1;
    theme_ptr->border.color = MMI_DARK_GRAY_COLOR;
    
    //set bg info
    theme_ptr->bg.bg_type = GUI_BG_IMG;
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
    theme_ptr->bg.img_id  = MMITHEME_GetCommonLightBg().img_id;//IMAGE_COMMON_BG_LIGHT;
#else
    theme_ptr->bg.img_id  = IMAGE_COMMON_BG;
#endif

    theme_ptr->bg.color   = MMI_GRAY_WHITE_COLOR;
    theme_ptr->bg.shape   = GUI_SHAPE_ROUNDED_RECT;

    //set string id
    theme_ptr->empty_text_id  = TXT_EMPTY_LIST;
    theme_ptr->mark_text_id   = STXT_MARK;
    theme_ptr->cancel_text_id = STXT_CANCEL;

    //set image id
    theme_ptr->mark_img_id = IMAGE_CHECK_SELECTED_ICON;
    theme_ptr->unmark_img_id = IMAGE_CHECK_UNSELECTED_ICON;

    theme_ptr->lcd_dev.lcd_id    = GUI_MAIN_LCD_ID;
    theme_ptr->lcd_dev.block_id  = GUI_BLOCK_MAIN;

#ifdef GUIF_ICONLIST_DELIMITER
    theme_ptr->delimiter_height            = MMITHEME_DELIMITER_HEIGHT;
	theme_ptr->delimiter_width             = MMITHEME_DELIMITER_WIDTH;
	theme_ptr->delimiter_str_left_space    = MMITHEME_DELIMITER_STR_LEFT_SPACE;
	//set font
    theme_ptr->delimiter_text_style.font   = MMI_DEFAULT_SMALL_FONT;
    theme_ptr->delimiter_text_style.color  = MMITHEME_ICON_FONT_COLOR;
	//set delimiter bg space
	theme_ptr->delimiter_bg_space.top      = MMITHEME_DELIMITER_BG_TOP_SPACE;
	theme_ptr->delimiter_bg_space.left     = MMITHEME_DELIMITER_BG_LEFT_SPACE;
	theme_ptr->delimiter_bg_space.width    = MMITHEME_DELIMITER_BG_WIDTH;	
	theme_ptr->delimiter_bg_space.height   = MMITHEME_DELIMITER_BG_HEIGHT;
#endif
    theme_ptr->is_display_softkey          = FALSE;
}


