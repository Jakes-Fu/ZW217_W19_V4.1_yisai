/*****************************************************************************
** File Name:      mmitheme_button.c                                         *
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

#define _MMITHEME_BUTTON_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmi_theme.h"
#include "mmitheme_button.h"
#include "mmidisplay_data.h"
#include "mmi_image.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifdef MAINLCD_DEV_SIZE_128X160 //cr133106
#define MMITHEME_BUTTON_MARGIN_SPACE    DP2PX_VALUE(1)
#else
#define MMITHEME_BUTTON_MARGIN_SPACE    DP2PX_VALUE(2)
#endif
#define MMITHEME_BUTTON_CHAR_SPACE      DP2PX_VALUE(0)   //edit by hongjun.jia for NEWMS00177100
#define MMITHEME_BUTTON_IMAGE_TEXT_SPACE    DP2PX_VALUE(3)
#define MMITHEME_BUTTON_IMAGE_SPACE    DP2PX_VALUE(6)

#define MMITHEME_BUTTON_POP_COLOR  0xffffffff
#define MMITHEME_BUTTON_POP_TOTAL_FRAME  8
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
//  Description : get button theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetButtonTheme(
                             MMITHEME_BUTTON_T  *theme_ptr  //in/out:
                             )
{
    //set runsheen
    theme_ptr->anim_type = GUIBUTTON_RUNSHEEN_ANIM;

    //set char space
    theme_ptr->image_text_space = MMITHEME_BUTTON_IMAGE_TEXT_SPACE;
    theme_ptr->margin_space = MMITHEME_BUTTON_MARGIN_SPACE;
    theme_ptr->char_space   = MMITHEME_BUTTON_CHAR_SPACE;

    //set image space
    theme_ptr->image_space = MMITHEME_BUTTON_IMAGE_SPACE;
    
    //set align
    theme_ptr->text_align = ALIGN_HVMIDDLE;
    
    //set border
    theme_ptr->border.type  = GUI_BORDER_NONE;
    theme_ptr->border.width = DP2PX_VALUE(1);
    theme_ptr->border.color = MMI_DARK_GRAY_COLOR;

    //set font
    theme_ptr->font.font  = MMI_DEFAULT_BUTTON_FONT;
    theme_ptr->font.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT);
    
    //set radio image
    theme_ptr->radio.unselect = IMAGE_RADIO_UNSELECTED_ICON;
    theme_ptr->radio.select   = IMAGE_RADIO_SELECTED_ICON;
    theme_ptr->radio.gray     = IMAGE_RADIO_UNSELECTED_ICON;

    //set check image
    theme_ptr->lcd_dev.lcd_id   = GUI_MAIN_LCD_ID;
    theme_ptr->lcd_dev.block_id = GUI_BLOCK_MAIN;

    //set pop effect
    theme_ptr->pop_info.total_frame = MMITHEME_BUTTON_POP_TOTAL_FRAME;
    theme_ptr->pop_info.filled_color = MMITHEME_BUTTON_POP_COLOR;
    theme_ptr->pop_info.is_default_radius = TRUE;
}
