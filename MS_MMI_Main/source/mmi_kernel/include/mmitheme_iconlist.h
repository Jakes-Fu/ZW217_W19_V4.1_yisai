/*****************************************************************************
** File Name:      mmitheme_iconlist.h                                       *
** Author:                                                                   *
** Date:           04/26/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2007      Jassmine              Creat
******************************************************************************/

#ifndef _MMITHEME_ICONLIST_H_
#define _MMITHEME_ICONLIST_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "cafcontrol.h"
#include "guiblock.h"
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
#ifdef MAINLCD_SIZE_320X480
#define MMITHEME_ICON_SHOW_WIDTH            90      //icon display width
#define MMITHEME_ICON_SHOW_HEIGHT           70      //icon display height
#elif MAINLCD_SIZE_240X320//NEWMS00145771
#define MMITHEME_ICON_SHOW_WIDTH            50      //icon display width
#define MMITHEME_ICON_SHOW_HEIGHT           40      //icon display height
#else
#define MMITHEME_ICON_SHOW_WIDTH            70      //icon display width
#define MMITHEME_ICON_SHOW_HEIGHT           60      //icon display height
#endif
        
#ifdef GUIF_ICONLIST_PDASTYLE
#define MMITHEME_UNSELECT_BORDER_COLOR      MMI_GRAY_WHITE_COLOR
#else
#define MMITHEME_UNSELECT_BORDER_COLOR      MMI_DARK_GRAY_COLOR
#endif

#define GUIF_ICONLIST_DELIMITER
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//item info
typedef struct
{
    uint16      width;      //item display width
    uint16      height;     //item display height

    uint16      hspace;     //item display horizontal space
    uint16      vspace;     //item display vertical space
} MMITHEME_ITEM_T;

//bg space
typedef struct
{
    uint16      top;
    uint16      left;
    uint16      width;
    uint16      height; 
} DELIMITER_BG_SPACE_T;

//the anim theme
typedef struct
{
    BOOLEAN             is_support_slide;   //is support slide
    uint16              slide_period;       //slide period
    uint16              reset_slide_start;  //reset slide the start frame
    uint16              reset_slide_frame;  //reset slide frame number

    uint16              groove_width;       //scroll groove width
    uint16              hmargin_space;      //left right margin
    uint16              vmargin_space;      //top bottom margin

    uint16              scroll_pixel;       //scroll pixel
    uint32              scroll_period;      //scroll period

    MMITHEME_ITEM_T     icon_item;          //icon item
    MMITHEME_ITEM_T     folder_item;        //folder item

    uint16              font_vspace;        //font and icon display vertical space
    GUI_FONT_ALL_T      font;               //font

    GUI_BG_T            icon_bg;            //bg
    GUI_BORDER_T        select_border;      //selected border
    GUI_BORDER_T        unselect_border;    //unselected border

    MMI_IMAGE_ID_T      default_icon_id;    //default display image icon id
    MMI_IMAGE_ID_T      error_icon_id;      //icon item err img id

    GUI_BORDER_T        border;             //border
    GUI_BG_T            bg;                 //bg

    MMI_TEXT_ID_T       empty_text_id;      //empty text id
    MMI_TEXT_ID_T       mark_text_id;       //mark text id
    MMI_TEXT_ID_T       cancel_text_id;     //cancel text id

    MMI_IMAGE_ID_T      mark_img_id;        //mark image id
    MMI_IMAGE_ID_T      unmark_img_id;        //unmark image id

    GUI_LCD_DEV_INFO    lcd_dev;            //lcd dev

#ifdef GUIF_ICONLIST_DELIMITER
    uint16              delimiter_height;         //分隔符高度
	uint16              delimiter_width;          //分隔符宽度
	uint16              delimiter_str_left_space; //分隔符内字符的左侧留白大小
    GUI_BG_T            delimiter_bg;             //分隔符背景
	GUI_FONT_ALL_T      delimiter_text_style;     //分隔符字符信息
    DELIMITER_BG_SPACE_T delimiter_bg_space;      //分隔符仅一部分需要背景的特殊情况使用
#endif
    BOOLEAN             is_display_softkey;  //是否由用户自绘softkey，FLASE时，用户自绘
} MMITHEME_ICONLIST_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get icon list theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetIconlistTheme(
                               MMITHEME_ICONLIST_T  *theme_ptr //in/out:
                               );
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

