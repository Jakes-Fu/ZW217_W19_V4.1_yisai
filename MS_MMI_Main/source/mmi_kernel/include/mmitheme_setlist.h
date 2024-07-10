/******************************************************************************
** File Name:      mmitheme_setlist.h                                         *
** Author:         xiaoqing.lu                                                *
** Date:           11/04/2009                                                 *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.          *
** Description:    This file is used to describe the data struct of           *
**                              guistatusbar control                          *
*******************************************************************************
**                         Important Edit History                             *
** ---------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 04/2009        Xiaoqing.lu      Create			                          *
******************************************************************************/

#ifndef _MMITHEME_SETLIST_H_
#define _MMITHEME_SETLIST_H_ 

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "mmk_type.h"
#include "guicommon.h"
#include "guiblock.h"
#include "guifont.h"
#include "guistring.h"
#include "guilistbox.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern   "C"
    {
#endif
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
#define MMISETLIST_ICONANIM_TIMEOUT                200 // 动画启动时的时间间隔
#define MMISETLIST_ITEM_MAX                        32  // 默认最多10个item
#define MMISETLIST_TEXT_MARGIN                     4   // 默认最多10个item


typedef enum
{
    MMI_SETLIST_NONE, // 不处理

    MMI_SETLIST_ALLOC,
    MMI_SETLIST_APPEND_ITEM,        // append list item
    MMI_SETLIST_SET_ITEM_STYLE,     // set list item style
    MMI_SETLIST_GET_CUR_RECT,       // 获得setlist区域
    MMI_SETLIST_GET_TITLE_STRLEN,   // 获取setlist title字符串的长度
    MMI_SETLIST_GET_TITLE_STRDATA,  // 获取setlist title 字符串
    MMI_SETLIST_IS_OPEN_FULL_WIN,   // 是否打开全屏窗口

    MMI_SETLIST_MAX
}MMI_SETLIST_CALL_TYPE_E;

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

// 定义显示风格，这个枚举既说明了布局也说明了item能显示的内容
typedef enum
{
    GUISETLIST_LAYOUT_NONE,         // 
    GUISETLIST_LAYOUT_TEXT,         // 只有文本的时候,文本显示以align确定
    GUISETLIST_LAYOUT_IMAGE_LEFT,   // 只有图片,图片显示在左侧,上下居中
    GUISETLIST_LAYOUT_IMAGE_RIGHT,  // 只有图片,图片显示在右侧,上下居中
    GUISETLIST_LAYOUT_IMAGE_MIDDLE, // 只有图片,图片显示在中间,上下居中
    GUISETLIST_LAYOUT_IMAGE_TEXT,   // 图片在前,文本在后,默认值。图片靠左,文本显示以align确定
    GUISETLIST_LAYOUT_TEXT_IMAGE,   // 文本在前,图片在后,文本显示以align确定
    GUISETLIST_LAYOUT_MAX
}MMI_GUISETLIST_LAYOUT_TYPE_E;

// 定义背景的类型
typedef enum
{
    GUISETLIST_BG_NONE,
    GUISETLIST_BG_IMAGE,
    GUISETLIST_BG_COLOR,
    GUISETLIST_BG_MAX
}MMI_GUISETLIST_BG_TYPE_E;

// setting list的默认信息
typedef struct
{
    // 文字风格
    int16               char_space;  // char space 
    GUI_FONT_T          font_type;   // font type
    GUI_COLOR_T         font_color;  // font color
    GUI_ALIGN_E         align;       // font align
    GUISTR_STATE_T      str_state;   // string state
    GUIFONT_EFFECT_T    effect;     //string effect
    
    uint16              left_margin;
    uint16              right_margin;

    // 背景及其他
    GUI_BG_T            bg_info;
    uint32              timer_out;   // timer out
    MMI_GUISETLIST_LAYOUT_TYPE_E layout;

    // 边框,只有控件设置了GUISETLIST_STATE_FRAME状态之后才会显示边框
    GUI_BORDER_T        border; // 边框

    // 箭头图标ID
	MMI_IMAGE_ID_T      left_arrow_icon_down;
    MMI_IMAGE_ID_T      left_arrow_icon_up;
    MMI_IMAGE_ID_T      left_arrow_icon_gray;
	MMI_IMAGE_ID_T      right_arrow_icon_down;
    MMI_IMAGE_ID_T      right_arrow_icon_up;
    MMI_IMAGE_ID_T      right_arrow_icon_gray;

}MMI_GUISETLIST_STYLE_T;

// 打开setlist数据信息
typedef struct  
{
    MMI_HANDLE_T parant_win_handle;
    MMI_HANDLE_T parant_ctrl_handle;
    GUILIST_TYPE_E list_type;
    uint16 cur_item_index;
    GUI_FONT_T list_font;
    GUI_COLOR_T font_color;
    GUI_COLOR_T title_font_color;
    GUI_BOTH_RECT_T win_both_rect;
    GUI_BOTH_RECT_T button_both_rect;
    uint32 check_allow_max_num;
}MMI_OPEN_SETLIST_WIN_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get the setting list style, is called by tab control
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetSetlistStyle(
                                        MMI_GUISETLIST_STYLE_T  *style_ptr
                                        );

/*****************************************************************************/
//  Description : 设置默认的最大item数值
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMITHEME_GetSetMaxItemNum(void);

/*****************************************************************************/
//  Description : 当显示动画的时候,获取动画显示的默认图片
//  Global resource dependence : 
//  Author: 
//  Note: 当动画解析错误或者解析之前显示的图片
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMITHEME_GetDefaultItemIcon(void);

/*****************************************************************************/
//  Description : 当显示动画的时候,获取动画显示的默认图片
//  Global resource dependence : 
//  Author: 
//  Note: 当动画解析错误或者解析之前显示的图片
/*****************************************************************************/
PUBLIC void MMITHEME_SetDefaultItemIcon(
                                        MMI_IMAGE_ID_T icon_id
                                        );

/*****************************************************************************/
//  Description : 打开setlist窗口
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_CreateSetlistChildWin(
                                           MMI_OPEN_SETLIST_WIN_T *data_ptr
                                           );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // _MMITHEME_SETLIST_H_
/*! @} */
