/*****************************************************************************
** File Name:      mmitheme_menu.h                                           *
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

#ifndef _MMITHEME_MENU_H_
#define _MMITHEME_MENU_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guilcd.h"
#include "mmi_font.h"
#include "mmitheme_pubwin.h"

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
// #define MMIMAINMENU_SUPPORT_SET_BGD // 定义是否支持可设置的背景

#if defined(GUIPOPMENU_OPTIONS_CONTEXT_SUPPORT) || defined(GUIPOPMENU_POP_CONTEXT_SUPPORT)
#define GUISUBPOPMENU_CONTEXT_SUPPORT
#endif

#define MMITHEME_MENU_REDRAW_TIME               10   // 图标移动timer
#define MMITHEME_MENU_TPMOVE_TIME               100  // 图标移动timer


#if defined MAINLCD_SIZE_128X160
// OPTIONS菜单信息
#define MMITHEME_OPTIONSMENU_MAX_ITEM_NUM           6                           // OptionsMenu一页显示多少个项，如果超过此数，将显示More项
#define MMITHEME_OPTIONSMENU_V_MIN_ITEM_WIDTH       100                         // OptionsMenu竖屏时一个item的最小宽度
#define MMITHEME_OPTIONSMENU_H_MIN_ITEM_WIDTH       79                          // OptionsMenu横屏时一个item的最小宽度
#define MMITHEME_OPTIONSMENU_ITEM_HEIGHT            51                          // OptionsMenu一个item高度
#define MMITHEME_OPTIONSMENU_IAMGE_HEIGHT           36                          // OptionsMenu中图标的高度
#define MMITHEME_OPTIONSMENU_SHADOW_HEIGHT          8                           // OptionsMenu中顶部阴影的高度
#define MMITHEME_MOREMENU_WIDTH                     220                         // More菜单的宽度(其值为IMAGE_OPTIONSMENU_MORE_BORDER的宽度)

#elif defined MAINLCD_SIZE_176X220 
// OPTIONS菜单信息
#define MMITHEME_OPTIONSMENU_MAX_ITEM_NUM           6                           // OptionsMenu一页显示多少个项，如果超过此数，将显示More项
#define MMITHEME_OPTIONSMENU_V_MIN_ITEM_WIDTH       100                         // OptionsMenu竖屏时一个item的最小宽度
#define MMITHEME_OPTIONSMENU_H_MIN_ITEM_WIDTH       79                          // OptionsMenu横屏时一个item的最小宽度
#define MMITHEME_OPTIONSMENU_ITEM_HEIGHT            51                          // OptionsMenu一个item高度
#define MMITHEME_OPTIONSMENU_IAMGE_HEIGHT           36                          // OptionsMenu中图标的高度
#define MMITHEME_OPTIONSMENU_SHADOW_HEIGHT          8                           // OptionsMenu中顶部阴影的高度
#define MMITHEME_MOREMENU_WIDTH                     220                         // More菜单的宽度(其值为IMAGE_OPTIONSMENU_MORE_BORDER的宽度)

#elif defined MAINLCD_SIZE_240X320

// OPTIONS菜单信息
#define MMITHEME_OPTIONSMENU_MAX_ITEM_NUM           6                           // OptionsMenu一页显示多少个项，如果超过此数，将显示More项
#define MMITHEME_OPTIONSMENU_V_MIN_ITEM_WIDTH       DP2PX_VALUE(100)                         // OptionsMenu竖屏时一个item的最小宽度
#define MMITHEME_OPTIONSMENU_H_MIN_ITEM_WIDTH       DP2PX_VALUE(79)                          // OptionsMenu横屏时一个item的最小宽度
#define MMITHEME_OPTIONSMENU_ITEM_HEIGHT            DP2PX_VALUE(51)                          // OptionsMenu一个item高度
#define MMITHEME_OPTIONSMENU_IAMGE_HEIGHT           DP2PX_VALUE(36)                          // OptionsMenu中图标的高度
#define MMITHEME_OPTIONSMENU_SHADOW_HEIGHT          DP2PX_VALUE(8)                           // OptionsMenu中顶部阴影的高度
#define MMITHEME_MOREMENU_WIDTH                     DP2PX_VALUE(220)                         // More菜单的宽度(其值为IMAGE_OPTIONSMENU_MORE_BORDER的宽度)

#elif defined MAINLCD_SIZE_128X64
// OPTIONS菜单信息
#define MMITHEME_OPTIONSMENU_MAX_ITEM_NUM           6                           // OptionsMenu一页显示多少个项，如果超过此数，将显示More项
#define MMITHEME_OPTIONSMENU_V_MIN_ITEM_WIDTH       100                         // OptionsMenu竖屏时一个item的最小宽度
#define MMITHEME_OPTIONSMENU_H_MIN_ITEM_WIDTH       79                          // OptionsMenu横屏时一个item的最小宽度
#define MMITHEME_OPTIONSMENU_ITEM_HEIGHT            51                          // OptionsMenu一个item高度
#define MMITHEME_OPTIONSMENU_IAMGE_HEIGHT           36                          // OptionsMenu中图标的高度
#define MMITHEME_OPTIONSMENU_SHADOW_HEIGHT          8                           // OptionsMenu中顶部阴影的高度
#define MMITHEME_MOREMENU_WIDTH                     220                         // More菜单的宽度(其值为IMAGE_OPTIONSMENU_MORE_BORDER的宽度)

#elif  defined MAINLCD_SIZE_240X400
// OPTIONS菜单信息
#define MMITHEME_OPTIONSMENU_MAX_ITEM_NUM           6                           // OptionsMenu一页显示多少个项，如果超过此数，将显示More项
#define MMITHEME_OPTIONSMENU_V_MIN_ITEM_WIDTH       100                         // OptionsMenu竖屏时一个item的最小宽度
#define MMITHEME_OPTIONSMENU_H_MIN_ITEM_WIDTH       79                          // OptionsMenu横屏时一个item的最小宽度
#define MMITHEME_OPTIONSMENU_ITEM_HEIGHT            51                          // OptionsMenu一个item高度
#define MMITHEME_OPTIONSMENU_IAMGE_HEIGHT           36                          // OptionsMenu中图标的高度
#define MMITHEME_OPTIONSMENU_SHADOW_HEIGHT          8                           // OptionsMenu中顶部阴影的高度
#define MMITHEME_MOREMENU_WIDTH                     220                         // More菜单的宽度(其值为IMAGE_OPTIONSMENU_MORE_BORDER的宽度)

#elif  defined MAINLCD_SIZE_320X480

// OPTIONS菜单信息
#define MMITHEME_OPTIONSMENU_MAX_ITEM_NUM           6                           // OptionsMenu一页显示多少个项，如果超过此数，将显示More项
#define MMITHEME_OPTIONSMENU_V_MIN_ITEM_WIDTH       100                         // OptionsMenu竖屏时一个item的最小宽度
#define MMITHEME_OPTIONSMENU_H_MIN_ITEM_WIDTH       79                          // OptionsMenu横屏时一个item的最小宽度
#define MMITHEME_OPTIONSMENU_ITEM_HEIGHT            66                          // OptionsMenu一个item高度
#define MMITHEME_OPTIONSMENU_IAMGE_HEIGHT           48                          // OptionsMenu中图标的高度
#define MMITHEME_OPTIONSMENU_SHADOW_HEIGHT          14                          // OptionsMenu中顶部阴影的高度
#define MMITHEME_MOREMENU_WIDTH                     308                         // More菜单的宽度(其值为IMAGE_OPTIONSMENU_MORE_BORDER的宽度)

#else
#error
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

//the menu common theme
typedef struct
{
    BOOLEAN             is_used_slide;		// 是否要滑动
    BOOLEAN             is_hl_disappear;	// 高亮是否消失
    BOOLEAN             is_has_button;		// 是否有button
    BOOLEAN             is_circle;			// 是否按上下左右键循环

    uint16              gap_width;  // font and border gap width
    GUI_COLOR_T         bg_color;   // background color
    MMI_IMAGE_ID_T      bg_image;   // background image
    GUI_LCD_DEV_INFO    lcd_dev;    // lcd dev
    
    GUI_COLOR_T         split_line_color;

    MMI_TEXT_ID_T       text_null_id;
    MMI_TEXT_ID_T       text_ok_id;
    MMI_TEXT_ID_T       text_cancel_id;
    MMI_TEXT_ID_T       text_return_id;
    MMI_IMAGE_ID_T      selected_img;   // selected image
} MMITHEME_MENU_COMMON_T;

//the second menu theme
typedef struct
{
    uint16              gap_width;  // font and border gap width
    uint16              pixel_scroll_step;  // item string scroll pixel step
    uint32              item_scroll_period; // item string scroll period

    GUI_FONT_T          item_font;          // menu item font
    GUI_FONT_T          cur_item_font;      // cur item font
    uint16              item_height;
    uint16              expand_item_height;
    uint16              item_space;

    BOOLEAN             is_draw_default_icon;
    MMI_IMAGE_ID_T      title_icon_id;      // title icon id
} MMITHEME_SECONDMENU_T;

//the pop-up menu theme
typedef struct
{
    uint8               mask_alpha;         // mask alpha
    GUI_COLOR_T         mask_color;         // mask color
    
    uint8               bg_alpha;           // background alpha
    GUI_COLOR_T         bg_color;           // background color

    GUI_COLOR_T         border_color;       // border color
    uint16              border_width;       // border width
    uint16              pixel_scroll_step;  // item string scroll pixel step

    int16               sub_min_left;       // sub pop-up menu min left coordinate

    uint16              item_height;        // item height
    uint16              item_space;         // item space
    GUI_FONT_T          item_font;          // menu item font
    GUI_FONT_T          cur_item_font;      // cur item font
    GUI_FONT_T          context_title_font; // context title font

    MMI_IMAGE_ID_T      border_img;         //border image
    MMI_IMAGE_ID_T      down_arrow_img;     //down arrow image
    MMI_IMAGE_ID_T      radio_sel_img;      // radio selected image
    MMI_IMAGE_ID_T      radio_unsel_img;    // radio unselected image
    MMI_IMAGE_ID_T      check_sel_img;      // check selected image
    MMI_IMAGE_ID_T      check_unsel_img;    // check unselected image
    MMI_IMAGE_ID_T      left_img;           // left image
    MMI_IMAGE_ID_T      right_img;          // right image
    MMI_IMAGE_ID_T      left_gray_img;      // left gray image
    MMI_IMAGE_ID_T      right_gray_img;     // right gray image

#ifdef GUISUBPOPMENU_CONTEXT_SUPPORT
    MMITHEME_PUBWIN_LAYOUT_T    layout;     // layout
#endif
} MMITHEME_POPMENU_T;

// optionsmenu theme
typedef struct
{
    uint16              item_height;        // item height
    uint16              image_height;       // image height in an item
    uint16              max_item_num;       // max displayed item num
    uint16              h_min_item_width;   // horz min item width
    uint16              v_min_item_width;   // vert min item width
    uint16              border_width;       // border width

    // more item
    MMI_IMAGE_ID_T      more_item_image_id; // image id for more item
    MMI_TEXT_ID_T       more_item_text_id;  // text id for more item

    // color
    uint8               bg_alpha;           // background alpha
    GUI_COLOR_T         bg_color;           // background color
    GUI_COLOR_T         border_color;       // border color

    // font
    GUI_FONT_T          item_font;          // menu item font
    GUI_FONT_T          cur_item_font;      // cur item font

    MMI_IMAGE_ID_T      shadow_img;         //shadow image
} MMITHEME_OPTIONSMENU_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get the menu group
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void* MMITHEME_GetMenuGroup(
                                   MMI_MENU_GROUP_ID_T   group_id
                                   );

/*****************************************************************************/
//  Description : get menu common theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetCommonMenuTheme(
                                 MMITHEME_MENU_COMMON_T     *theme_ptr //in/out:
                                 );

/*****************************************************************************/
//  Description : get second menu theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetSecondMenuTheme(
                                 MMITHEME_SECONDMENU_T  *theme_ptr //in/out:
                                 );

/*****************************************************************************/
//  Description : get second menu icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMITHEME_GetSecItemIcon(
                                              uint16    item_index
                                              );

/*****************************************************************************/
//  Description : get pop-up menu theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetPopMenuTheme(
                              MMITHEME_POPMENU_T    *theme_ptr //in/out:
                              );

#ifdef MMI_OPTIONMENU_SUPPORT
/*****************************************************************************/
//  Description : get OptionsMenu theme
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
void MMITHEME_GetOptionsMenuTheme(
                                  MMITHEME_OPTIONSMENU_T    *theme_ptr // [out]
                                  );
#endif

//#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
