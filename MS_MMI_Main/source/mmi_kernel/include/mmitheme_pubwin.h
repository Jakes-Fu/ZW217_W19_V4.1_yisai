/*****************************************************************************
** File Name:      mmitheme_pubwin.h                                         *
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

#ifndef _MMITHEME_PUBWIN_H_
#define _MMITHEME_PUBWIN_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guilcd.h"
#include "guisoftkey.h"

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
#define MMIPUB_IM_BG_COLOR        GUI_RGB2RGB565(98,98,98)
#define MMIPUB_IM_BUTTON_BG_COLOR GUI_RGB2RGB565(189,189,189)
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// common pubwin layout: shared by pubwin and context menu
typedef struct
{
    // title部分
    uint16                  title_start_margin;     // title内容开始位置离窗口左边的间距(包括了左边的留白)
    uint16                  title_image_text_space; // title中text与image的间距
    uint16                  title_v_top_margin;     // title离顶部的距离
    uint16                  title_v_bottom_margin;  // title离顶部的距离

    // client部分
    uint16                  client_h_margin;        // PUBWIN客户区离窗口左边的距离(包括了左边的留白)
    uint16                  client_v_margin;        // PUBWIN客户区离上下边的距离
    uint16                  win_h_add_margin;       // PUBWIN横屏情况下，两边需要留更多的空白，这个是增量值

    // button部分
    uint16                  button_margin;          // BUTTON离左右的边距(包括了左边的留白)
    uint16                  button_bottom_margin;   // BUTTON下边距
    uint16                  button_space;           // BUTTON之间的间距

    // 字体颜色
    GUI_FONT_ALL_T          button_font;
    GUI_COLOR_T             ctrl_bg_color;

    // 图片
    MMI_IMAGE_ID_T          top_bg_id;
    MMI_IMAGE_ID_T          client_bg_id;
    MMI_IMAGE_ID_T          bottom_bg_id;
    MMI_IMAGE_ID_T          bottom_bg_without_button_id;
    MMI_IMAGE_ID_T          button_img_id;
    MMI_IMAGE_ID_T          button_img_hl_id;
    MMI_IMAGE_ID_T          button_img_gray_id;

    MMI_IMAGE_ID_T          button_ok_normal_img;       // button的OK图片
    MMI_IMAGE_ID_T          button_ok_press_img;        // button的OK图片
    MMI_IMAGE_ID_T          button_ok_gray_img;         // button的OK图片
    MMI_IMAGE_ID_T          button_cancel_normal_img;   // button的cancel图片
    MMI_IMAGE_ID_T          button_cancel_press_img;    // button的cancel图片
    MMI_IMAGE_ID_T          button_cancel_gray_img;     // button的cancel图片
} MMITHEME_PUBWIN_LAYOUT_T;

//the public window theme
typedef struct
{
    uint32                  update_delay_ticks; //延迟刷新的时间
    uint32                  bg_id;              //background image id
    //uint32                  up_bg_id;           //upper background image id
    uint32                  anim_bg_image_id;   //wait anim id
    BOOLEAN                 is_transparent_softkey;     //是否透明显示softkey
    
    GUI_LCD_DEV_INFO        bg_lcd_dev;         //background image lcd dev
    GUI_COLOR_T             bg_color;
    uint8                   bg_alpha;
    BOOLEAN                 is_support_anim;

    GUI_FONT_T              big_font;
    GUI_FONT_T              normal_font;
    GUI_FONT_T              small_font;
    GUI_COLOR_T             font_color;
    uint16                  line_space;             // text的行间距

    MMITHEME_PUBWIN_LAYOUT_T    layout;
} MMITHEME_PUBWIN_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get public windows theme,alert or query windows
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetPubTheme(
                          uint32             type,
                          MMITHEME_PUBWIN_T* theme_ptr
                          );

/*****************************************************************************/
//  Description : get common public windows theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetPubLayoutTheme(
                                MMITHEME_PUBWIN_LAYOUT_T    *layout_theme_ptr
                                );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
