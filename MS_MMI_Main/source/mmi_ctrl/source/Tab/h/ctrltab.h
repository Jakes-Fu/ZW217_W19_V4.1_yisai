/*****************************************************************************
** File Name:      ctrltab.h                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Create
******************************************************************************/

#ifndef _CTRLTAB_H_
#define _CTRLTAB_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrltab_export.h"

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
// tab type info
#define CTRL_TAB_TYPE     (TAB_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
// tab init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T        base_ctrl_param;
    GUITAB_STATE_T               state;					/*!< tab状态*/
    GUI_TAB_STYLE_E              style;	                /*!< 对其方式 */
    uint32                       max_item_num;	        /*!< 最大项数，最大值为 ：\link #GUITAB_MAX_ITEM_NUM \endlink*/
    BOOLEAN                      is_active_item;        /*!< 无*/    // to load item actively

    // self inif param ...
} CTRLTAB_INIT_PARAM_T;

//tab control
typedef struct _tab_obj_tag
{
    CTRLBASE_OBJ_T          base_ctrl;
    GUITAB_STATE_T          tab_state;                  /*!< 控件状态*/

    // long press timer parameter
    uint32                  timeout;                    /*!< timer启动的时限间隔，系统保留*/
    uint32                  auto_switch_time;           /*!< 自动切换到下一个，系统保留*/
    GUITAB_TIMER_STATE_T    timer_status;               /*!< 当前timer状态，系保留*/
    uint8                   timer_id;                   /*!< timer id 系统保留*/
    uint8                   text_timer_id;              /*!<item text滚动定时器*/
    uint16                  str_index;                  /*!<current item string start index*/

    BOOLEAN                 is_tp_down;                 /*!< 是否tp down*/
    uint32                  tp_down_item_index;         /*!< tp down item索引号*/
    GUI_POINT_T             tp_prev_point;              /*!< tp前一个点的坐标*/

    // information of tab item
    uint32                  max_item_num;               /*!< 最大item数*/
    uint32                  total_item_num;             /*!< item总数，小于等于最大数max_item_num*/
    uint32                  show_item_num;              /*!< 实际能显示的item数，根据控件宽度计算而得*/
    uint32                  cur_sel_item_index;         /*!< 当前item索引号*/
    uint32                  pre_sel_item_index;         /*!< 前一个item索引号*/
    uint32                  start_show_item_index;      /*!< 起始显示*/
    uint32                  item_space;                 /*!< 间距*/
    MMI_TAB_STYLE_T         tab_style;                  /*!< 控件默认风格*/

    uint32                  item_height;                /*!< 有效区域高度，不包括显示title区域*/
    uint32                  item_width;                 /*!< tab项有效区域的宽度*/
    GUI_RECT_T              item_rect;                  /*!< tab项显示的总区域，如果右左右箭头，则要去除箭头所占区域*/
    GUITAB_ITEM_T*          item_ptr;                   /*!< 数据项链表*/
    GUITAB_ITEM_LIST_T      valid_item_list;            /*!< 有效的数据项链表*/

    int32                   cur_object_index;           /*!< 解析tab项图片所用，表示当前正在解析的项的索引号*/
    // scroll parameter
    BOOLEAN                 is_scroll;                  /*!< 控件是否需要滚动箭头*/
    BOOLEAN                 is_rotative;                /*!< 是否循环滚动*/
    GUITAB_SCROLL_CARD_T    left_scroll_card;           /*!< 左侧滚动卡信息*/
    GUITAB_SCROLL_CARD_T    right_scroll_card;          /*!< 右侧滚动卡信息*/

    int32                   offset_x;                   /*!< 第一个item的左端相对于控件区域x方向的偏移*/

    // display style
    GUI_TAB_STYLE_E         style;                      /*!< TAB显示风格，即对齐方式，目前支持左对齐*/
    
    MMI_STRING_T            title_str_info;             /*!< 这个字符串是TAB控件的说明，当GUITAB_STATE_COMMON_TITLE有效时使用*/
    
    GUITAB_CAPTION_T        added_string;               /*!< 附加信息*/
    BOOLEAN                 is_reset_title;             /*!< 是否重新设置内容*/
    BOOLEAN                 is_reset_common_title;      /*!< 是否重新设置统一的title，只需要设置一次*/
    ADD_DATA                title_ptr;                  /*!< 用于记录内嵌的title控件指针*/

    // self member ...
} CTRLTAB_OBJ_T;

// tab class for public or protected method
typedef struct _tab_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

    // self vtbl ...
} CTRLTAB_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get tab type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T TAB_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
