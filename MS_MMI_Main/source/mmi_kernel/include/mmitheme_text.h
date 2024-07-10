/*****************************************************************************
** File Name:      mmitheme_text.h                                           *
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

#ifndef _MMITHEME_TEXT_H_
#define _MMITHEME_TEXT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guifont.h"
#include "mmi_font.h"
#include "mmitheme_clipbrd.h"

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
#define MMITHEME_TEXT_MARGIN_SPACE      DP2PX_VALUE(5)
#define MMITHEME_TEXT_CHAR_SPACE        DP2PX_VALUE(2)
#define MMITHEME_TEXT_LINE_SPACE        DP2PX_VALUE(2)	//该间距下，PUBWIN纯文本可显示4行
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//the text common theme
typedef struct
{
    BOOLEAN                     is_support_slide;   //is support slide
    BOOLEAN                     is_circular_handle; //is circular display
    BOOLEAN                     is_handle_tp;       //is handle tp msg

    uint16                      slide_period;       //slide period
    uint16                      reset_slide_start;  //reset slide the start frame
    uint16                      reset_slide_frame;  //reset slide frame number

    uint16                      char_space;         //space between chars,must >=1,include cursor
    uint16                      line_space;         //space between rows,include dividing line
    uint16                      margin_left_space;    //margin
    uint16                      margin_right_space;    //margin
    uint16                      margin_top_space;    //margin
    uint16                      margin_bottom_space;    //margin

    GUI_BORDER_T                border;             //border

    uint16                      line_width;         //dividing line width,must <= line space
    GUI_COLOR_T                 line_color;         //dividing line color

    GUI_COLOR_T                 underline_color;    //underline color

    GUI_BG_T                    bg;                 //background
    GUI_FONT_ALL_T              font;               //font

    uint16                      scroll_step;        //一次滚动页数,行数或者像素数
    uint32                      scroll_mode;        //GUITEXT_SCROLL_MODE_E scroll mode
    uint32                      scroll_period;      //定时滚动时间间隔
    uint32                      tp_scroll_period;   //TP定时滚动时间间隔

    uint32                      tag_type;           //GUITEXT_TAG_TYPE_E parse type

    GUI_ALIGN_E                 align;              //align

    GUI_LCD_DEV_INFO            lcd_dev;            //lcd dev

    MMITHEME_DRAGMODE_E         drag_mode;              // 滑块操作模式
    uint32                      slider_display_period;  // 滑块显示时间
} MMITHEME_TEXT_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get text theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetTextTheme(
                           MMITHEME_TEXT_T  *theme_ptr  //in/out:
                           );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
