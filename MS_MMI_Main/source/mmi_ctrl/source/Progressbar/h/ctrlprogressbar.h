/*****************************************************************************
** File Name:      ctrlprogressbar.h                                   *
** Author:                                                                   *
** Date:              07/23/2012                                              *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                                                                                          *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                                                                      *
** 07/2012     xiyuan.ma              Creat
******************************************************************************/

#ifndef _CTRLPROGRESSBAR_H_
#define _CTRLPROGRESSBAR_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "cafcontrol.h"

#include "ctrlbase.h"
#include "ctrlprogressbar_export.h"
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
//progressbar type info
#define CTRL_PROGRESSBAR_TYPE (CTRLPROGRESSBAR_GetType())
#define RECT_PROGRESSBAR_HEIGHT     (15)
#if defined (MAINLCD_SIZE_128X160)
#define RECT_PROGRESSBAR_WIDTH		(128)
#elif   defined (MAINLCD_SIZE_240X284)
#define RECT_PROGRESSBAR_WIDTH		(240)
#elif   defined (MAINLCD_SIZE_240X240)
#define RECT_PROGRESSBAR_WIDTH		(240)
#elif   defined (MAINLCD_SIZE_240X320)
#define RECT_PROGRESSBAR_WIDTH		(240)
#else
#define RECT_PROGRESSBAR_WIDTH		(240)
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//progressbar init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T      base_ctrl_param;

    BOOLEAN                                is_forbid_paint;/*!< 是否刷新 */
    uint32                                     lcd_id;         /*!< lcd_id */
    GUIPRGBOX_STYLE_E             style;          /*!< 滚动条模式 */
    MMITHEME_PRGBOX_TYPE_E  theme_type;     /*!< 滚动条主题 */
} CTRLPROGRESSBAR_INIT_PARAM_T;

//struct CTRLTIPS_CTRL_T
typedef struct _progressbar_obj_tag
{
    CTRLBASE_OBJ_T                  base_ctrl;

    BOOLEAN                         is_forbid_paint;    /*!< 滚动条是否刷新 */
    CAF_LCD_DEV_INFO                disp_layer;         /*!< 滚动条显示的层 */
#if defined(GUIF_PRGBOX_AUTOHIDE)
    BOOLEAN                         is_active;          /*!< 显式记录激活状态，避免在非激活状态的操作 */
    BOOLEAN                         is_fobid_thumb;          /*!< 显式记录激活状态，避免在非激活状态的操作 */
    uint8                           remove_timer_id;    /*!< 隐藏滚动条的timer */
    int16                           last_tp_pos;        /*!< 最后一个有效移动点 */
    int16                           last_tp_offset;     /*!< 最后一个有效滑块top，因为消息是post，不实时更新 */
    uint32                          out_state;          /*!< 消隐的状态 */
    uint16                          thumb_width;        /*!< 滑块的宽，为了避免重复计算，记录下来 */
    uint16                          thumb_height;       /*!< 消隐的高，同上 */
#endif

    uint32                          cur_item_index;     /*!< 当前item序号，从"0"开始 */ 
    uint32							first_item_index;   /*!< 第一个item序号，从"0"开始 */ 
    uint32                          total_item_num;     /*!< 所有item个数 */ 
	int16                          *letter_postion_table;     /*<每个字母的y坐标>*/
    uint32                          items_per_page;     /*!< 每页item个数 */ 

    GUIPRGBOX_STYLE_E               prgbox_style;       /*!< 滚动条显示方式 */ 
    MMITHEME_PRGBOX_TYPE_E    theme_type;         /*!< 主题模式 */  
    GUI_RECT_T                      pre_arrow_rect;     /*!< 前一个箭头区域(针对触屏) */  
    GUI_RECT_T                      next_arrow_rect;    /*!< 下一个箭头区域(针对触屏) */ 
    GUI_RECT_T                      pre_groove_rect;    /*!< 前一个滚动条区域(针对触屏) */ 
    GUI_RECT_T                      next_groove_rect;   /*!< 下一个滚动条区域(针对触屏) */ 
    GUI_RECT_T                      scroll_bar_rect;    /*!< 滚动条滑块区域(针对触屏) */ 
    
    MMITHEME_PRGBOX_VER_SCROLL_T     ver_scroll_theme;   /*!< 纵向滚动条主题 */ 
    MMITHEME_PRGBOX_HOR_SCROLL_T    hor_scroll_theme;   /*!< 横向滚动条主题 */ 
    MMITHEME_PRGBOX_PROCESS_T          process_theme;      /*!< 进度条主题 */ 
    
    GUI_POINT_T                     pre_tp_point;       /*!< 当前触笔触点 */  
    uint32                          point_offset;       /*!< 点在bar内的偏移位置 */  
    BOOLEAN                         is_move_valid;      /*!< 是否可移动 */
    BOOLEAN                         is_loop_scroll;     /*!< 是否循环滚动条 */  
    
    BOOLEAN                         is_need_slide_block;/*!< 是否需要滑块 */ 
    BOOLEAN                         is_need_bg;         /*!< 是否要背景 */ 
    BOOLEAN                         is_letter_bar_valid;/*!< 字母滚动条是否有效，在显示空间比较小的情况下无效，且不显示*/ 
    uint16                          *base_letters_table;/*<基本字母表>*/
    uint16                          letters_table_len;  /*<基本字母表的长度>*/
    MMI_WIN_ID_T win_id;                                /*!< 窗口id */ 

    uint32                          top_limit;    // for wide progress bar adjustable region.
    uint32                          bottom_limit;
    
	GUI_RECT_T                      label_rect;//文本显示区域
	GUI_RECT_T                      fg_rect;//前景显示区域
	BOOLEAN							is_waiting_indicator;
    MMI_STRING_T                    *rect_progress_title; 
    // GUIANIM_CTRL_INFO_T             *waiting_ind_animctrl_handle;
} CTRLPROGRESSBAR_OBJ_T;

//progressbar class for public or protected method
typedef struct _progressbar_vtbl_tag
{
    CTRLBASE_VTBL_T                       base_ctrl_vtbl;
} CTRLPROGRESSBAR_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*********************************************************************/
//  Description : get progressbar type
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*********************************************************************/
PUBLIC TYPE_TAG_T CTRLPROGRESSBAR_GetType();

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif //_CTRLPROGRESSBAR__H_

