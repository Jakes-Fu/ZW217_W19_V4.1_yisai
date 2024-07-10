/*****************************************************************************
** File Name:      mmitheme_label.h                                          *
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

#ifndef _MMITHEME_LABEL_H_
#define _MMITHEME_LABEL_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guictrl.h"
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
#define MMITHEME_LABEL_CHAR_SPACE       0
        
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//the label theme
typedef struct
{
    uint16              margin_lr_space;    //margin
    uint16              margin_tb_space;    //margin
    uint16              char_space;         //space between chars
    uint16              icon_space;         //space icon

    uint16              scroll_pixel;       //scroll pixel
    uint32              scroll_period;      //scroll period

    GUI_BORDER_T        border;             //border
    GUI_BG_T            bg;                 //background
    GUI_FONT_ALL_T      font;               //font

    GUI_LCD_DEV_INFO    lcd_dev;            //lcd dev
} MMITHEME_LABEL_T;

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
                            );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

