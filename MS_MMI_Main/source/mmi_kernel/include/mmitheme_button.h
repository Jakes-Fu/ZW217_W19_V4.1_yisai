/*****************************************************************************
** File Name:      mmitheme_button.h                                         *
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

#ifndef _MMITHEME_BUTTON_H_
#define _MMITHEME_BUTTON_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guictrl.h"
#include "guifont.h"
#include "mmi_font.h"
#include "graphics_3d_rotate.h"//
#include "guibutton.h"
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
#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#define  MMI_DEFAULT_BUTTON_FONT   SONG_FONT_14

//added by lxf for dphone
#elif defined(MAINLCD_SIZE_128X64) 
#define  MMI_DEFAULT_BUTTON_FONT   SONG_FONT_12
//edit by hongjun.jia for NEWMS00177100
#elif defined(MAINLCD_SIZE_240X400)
#define  MMI_DEFAULT_BUTTON_FONT   SONG_FONT_14     

#elif defined(MAINLCD_SIZE_240X320)
#define  MMI_DEFAULT_BUTTON_FONT   DP_FONT_14     

#elif defined(MAINLCD_SIZE_320X480)
#define  MMI_DEFAULT_BUTTON_FONT   SONG_FONT_18  
#else
#error
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//the button image
typedef struct
{
    MMI_IMAGE_ID_T      unselect;   //unselected image
    MMI_IMAGE_ID_T      select;     //selected image
    MMI_IMAGE_ID_T      gray;       //gray image
} MMITHEME_BIMG_T;

//the button pop effect
typedef struct
{
    int32                total_frame;    
    int32                radius;         
    COLOR32              filled_color; 
    BOOLEAN              is_default_radius; //是否为默认的pop半径
} MMITHEME_POP_T;

//the button theme
typedef struct
{
    GUIBUTTON_EFFECT_TYPE_E anim_type; //button effect type

    uint16              margin_space;       //margin space
    uint16              image_text_space;   // space between image and text. for GUIBUTTON_STYLE_UIDT
    uint16              image_space;        // space for image and text. for GUIBUTTON_STYLE_LIRTM and GUIBUTTON_STYLE_LIRTL
    
    uint16              char_space;     //char space
    GUI_ALIGN_E         text_align;     //align
    
    GUI_BORDER_T        border;         //border
    GUI_FONT_ALL_T      font;           //font

    MMITHEME_BIMG_T     radio;          //radio image

    GUI_LCD_DEV_INFO    lcd_dev;        //lcd dev
    
    MMITHEME_POP_T      pop_info;       //pop theme info
} MMITHEME_BUTTON_T;

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
                             );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
