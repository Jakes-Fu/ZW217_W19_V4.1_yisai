/*****************************************************************************
** File Name:      mmitheme_form.h                                           *
** Author:                                                                   *
** Date:           04/26/2004                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      Jassmine              Creat
******************************************************************************/

#ifndef _MMITHEME_FORM_H_
#define _MMITHEME_FORM_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guifont.h"
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
//child control theme
typedef struct
{
    uint16      num1;
    uint16      num2;
} MMITHEME_RATIO_T;

//child control theme
typedef struct
{
    GUI_BG_T            bg;         //background
    GUI_BORDER_T        border;     //border
    GUI_FONT_ALL_T      font;       //font
} MMITHEME_CHILD_T;

//form child control theme
typedef struct
{
    GUI_BG_T            bg;         //background
    GUI_BORDER_T        border;     //border
} MMITHEME_FORM_CHILD_T;

//the form common theme
typedef struct
{
    BOOLEAN                 is_cir_handle_ud;   //is circular handle up/down
    BOOLEAN                 is_cir_handle_lr;   //is circular handle left/right

    BOOLEAN                 is_support_slide;   //is support slide
    uint16                  slide_period;       //slide period
    uint16                  reset_slide_start;  //reset slide the start frame
    uint16                  reset_slide_frame;  //reset slide frame number

    uint16                  hor_space;      //child horizontal space
    uint16                  ver_space;      //child vertical space

    GUI_BG_T                bg;             //background
    GUI_BORDER_T            border;         //border
    GUI_PADDING_T           padding;        //padding

    uint16                  line_width;     //dividing line width
    GUI_COLOR_T             line_color;     //dividing line color

    MMITHEME_RATIO_T        img_ratio;      //image height/width ratio
    MMITHEME_RATIO_T        scroll_ratio;   //scroll step/form height ratio
    MMITHEME_RATIO_T        indent_ratio;   //unit form indentation ratio

    GUI_LCD_DEV_INFO        lcd_dev;        //lcd dev

    GUI_FONT_ALL_T          unit_font;      //unit font,,except the first child
    
    MMITHEME_CHILD_T        child;          //child theme
    MMITHEME_CHILD_T        active_child;   //active child theme
    MMITHEME_CHILD_T        gray_child;     //active child theme

    MMITHEME_FORM_CHILD_T   form_child;         //form child theme
    MMITHEME_FORM_CHILD_T   form_active_child;  //active form child theme
    MMITHEME_FORM_CHILD_T   form_gray_child;    //active form child theme
} MMITHEME_FORM_T;

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
                           );

/*****************************************************************************/
//  Description : modify form theme by type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_ModifyFormTheme(
                              uint32            type,       //in:
                              MMITHEME_FORM_T   *theme_ptr  //in/out:
                              );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

