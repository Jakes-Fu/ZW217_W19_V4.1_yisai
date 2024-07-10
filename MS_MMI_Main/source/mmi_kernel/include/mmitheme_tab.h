/*****************************************************************************
** File Name:      mmi_theme.h                                               *
** Author:         Great.Tian                                                *
** Date:           06/21/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe the mmi theme               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2004        Great.Tian       Create								     *
**																		     *
*****************************************************************************/

#ifndef  _MMITHEME_TAB_H_    
#define  _MMITHEME_TAB_H_   

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "guicommon.h"
#include "guiblock.h"
#include "guifont.h"

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


/*----------------------------------------------------------------------------*/
/*                         TYPE AND STRUCT                                    */
/*----------------------------------------------------------------------------*/
// default tab hight
#define MMITAB_DEFAULT_ITEM_HEIGHT			            MMI_TAB_ITEM_DEFAULT_HEIGHT

// default tab width
#define MMITAB_DEFAULT_ITEM_WIDTH			            MMI_TAB_ITEM_DEFAULT_WIDTH

// default space value between two items
#define GUITAB_DEFAULT_ITEM_SPACE			            5

// timeout interval of timer
#define GUITAB_DEFAULT_TIMEOUT				            200

// item显示文字相对于底边的space
#define MMITAB_DEFAULT_ITEM_TEXT_BOTTOM_SPACE			MMI_TAB_ITEM_TEXT_BOTTOM_SPACE

// TAB style
typedef struct
{
	BOOLEAN				is_display_prg;	                    //是否显示进度条
    uint8				font_space;                         //字间距
	MMI_IMAGE_ID_T		prg_scroll_id;	                    //滚动条图片
	MMI_IMAGE_ID_T		bg_image_id;                        //背景图片 
    GUI_COLOR_T			bg_color;                           //背景颜色 
	MMI_IMAGE_ID_T		left_scroll_pressed_image_id;       //左滚pressed图片 
	MMI_IMAGE_ID_T		left_scroll_released_image_id;      //左滚released图片
	MMI_IMAGE_ID_T		left_scroll_disabled_image_id;      //左滚disabled图片 
	MMI_IMAGE_ID_T		right_scroll_pressed_image_id;      //右滚pressed图片  
	MMI_IMAGE_ID_T		right_scroll_released_image_id;     //右滚released图片
	MMI_IMAGE_ID_T		right_scroll_disabled_image_id;     //右滚disabled图片 
    GUI_COLOR_T			sel_item_color;                     //选中tab的颜色
	GUI_COLOR_T			unsel_item_color;                   //未选中tab的颜色
    GUI_COLOR_T         focus_rect_color;                   // 当不需要焦点item显示图片时，使用该颜色画焦点框
    GUI_COLOR_T			font_color;                         //字体颜色
	GUI_COLOR_T			sel_font_color;                     //选中字体颜色
    GUI_FONT_T			font;                               //字体类型
    uint32				item_default_width;                 // 当item只显示图片时，item的宽度，这个值小于等于item的总宽度
    uint32			    item_default_height;                // 当item只显示图片时，item的宽度，这个值小于等于item的总宽度
    MMI_IMAGE_ID_T      item_deficon_id;                    // 当支持动画图标的时候,如果解析不成功,则设置默认的显示图标
    MMI_IMAGE_ID_T      item_error_id;                      // 当支持动画图标的时候,如果解析不成功,则设置默认的显示图标
    uint32              scroll_card_def_width;              // card 默认宽度
    GUI_BG_T            height_bg_info;
	GUI_RECT_T          height_bg_margin;                   // 上下左右四个方向的边距
    GUI_RECT_T          text_margin;                        // 上下左右四个方向的边距
    BOOLEAN             is_show_cur_item_only;              // 只显示当前的item
	uint16				prg_height;	                        //滚动条高度
	uint16				default_item_num;	                //默认一页显示最多item
	uint32				item_state;	                        //GUITAB_ITEM_STATE_T:item初始状态
	MMI_IMAGE_ID_T		tp_sel_img_id;	                    //tp down选中图片
	MMI_IMAGE_ID_T		dividing_line;	                    //tab分割线
} MMI_TAB_STYLE_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get the tab style, is called by tab control
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetTabStyle(MMI_TAB_STYLE_T  * tab_style_ptr);

/*****************************************************************************/
// 	Description : get the tab child win top
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetTabChildWinTop(
                                         MMI_HANDLE_T win_handle
                                         );

/*****************************************************************************/
// 	Description : get the tab child win top
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetTabHeight(
                                    void
                                    );

/*****************************************************************************/
// 	Description : get the tab default width
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetTabDefWidth(void);

/*****************************************************************************/
// 	Description : get the tab default display item num
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetTabDefDispItemNum(void);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif /* END _MMITHEME_TAB_H_  */
/*! @} */
