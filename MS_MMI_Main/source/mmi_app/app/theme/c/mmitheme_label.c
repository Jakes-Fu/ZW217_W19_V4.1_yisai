/*****************************************************************************
** File Name:      mmitheme_label.c                                          *
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

#define _MMITHEME_LABEL_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmi_theme.h"
#include "mmitheme_label.h"
#include "mmidisplay_data.h"
#include "mmi_image.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#if defined(MAINLCD_SIZE_128X64)//@ alex added
#define MMITHEME_LABEL_MARGIN_SPACE    0 //added by taiping.lai 2010/12/31 Dphone 上该值固定为0
#define MMITHEME_LABEL_ICON_SPACE       2
#define MMITHEME_LABEL_SCROLL_PERIOD    500
#else
#define MMITHEME_LABEL_MARGIN_SPACE    DP2PX_VALUE(2)
#define MMITHEME_LABEL_ICON_SPACE       DP2PX_VALUE(2)
#define MMITHEME_LABEL_SCROLL_PERIOD    500
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
//  Description : get label theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetLabelTheme(
                            MMITHEME_LABEL_T    *theme_ptr  //in/out:
                            )
{
    //set space
    theme_ptr->margin_lr_space  = MMITHEME_LABEL_MARGIN_SPACE;
    theme_ptr->margin_tb_space  = MMITHEME_LABEL_MARGIN_SPACE;
    theme_ptr->char_space       = MMITHEME_LABEL_CHAR_SPACE;
    theme_ptr->icon_space       = MMITHEME_LABEL_ICON_SPACE;

    //set scroll
    theme_ptr->scroll_pixel     = (uint16)(MMITHEME_ScrollPixelNum());
    theme_ptr->scroll_period    = MMITHEME_LABEL_SCROLL_PERIOD;

    //set border
    theme_ptr->border.type      = GUI_BORDER_NONE;
    theme_ptr->border.width     = DP2PX_VALUE(1);
    theme_ptr->border.color     = MMI_DARK_GRAY_COLOR;
    theme_ptr->bg.bg_type       = GUI_BG_NONE;
    theme_ptr->bg.color         = MMI_BACKGROUND_COLOR;
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
    theme_ptr->bg.img_id        = MMITHEME_GetCommonLightBg().img_id;//IMAGE_COMMON_BG_LIGHT;//IMAGE_COMMON_BG;
#else
    theme_ptr->bg.img_id        = IMAGE_COMMON_BG;
#endif
    theme_ptr->bg.shape         = GUI_SHAPE_ROUNDED_RECT;

    //set font
    theme_ptr->font.font         = MMI_DEFAULT_TEXT_FONT;
    theme_ptr->font.color        = MMITHEME_GetCurThemeFontColor(MMI_THEME_LABEL_FONT);

    theme_ptr->lcd_dev.lcd_id   = GUI_MAIN_LCD_ID;
    theme_ptr->lcd_dev.block_id = GUI_BLOCK_MAIN;
}
