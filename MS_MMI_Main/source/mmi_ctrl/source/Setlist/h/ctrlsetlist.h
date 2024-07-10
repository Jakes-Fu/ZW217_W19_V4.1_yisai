/*****************************************************************************
** File Name:      ctrlsetlist.h                                               *
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

#ifndef _CTRLSETLIST_H_
#define _CTRLSETLIST_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrlsetlist_export.h"

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

// setlist type info
#define CTRL_SETLIST_TYPE     (SETLIST_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
// setlist init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;

    // self inif param ...
} CTRLSETLIST_INIT_PARAM_T;

//setlist control
typedef struct _setlist_obj_tag
{
    CTRLBASE_OBJ_T          base_ctrl;
    
    GUISETLIST_STATE_T      ctrl_state;      /*!< 记录控件状态*/
    GUISETLIST_TYPE_E       list_type;       /*!< 记录控件状态*/   

    BOOLEAN                 is_valid;        /*!< 无效的时候,不可见,
也不会接受任何消息,但默认情况下可见*/
    BOOLEAN                 is_need_arrow;   /*!< 是否需要显示左右箭头,
默认情况下不显示，用户可调用相关函数设置*/
    BOOLEAN                 left_is_pressed; /*!< 左侧是否被按下*/
    BOOLEAN                 right_is_pressed;/*!< 右侧是否被按下*/
    
    MMI_GUISETLIST_STYLE_T  default_style;   /*!< 控件的默认设置*/

    uint32                  item_max_num;    /*!< 最大数*/
    uint32                  cur_item_index;  /*!< 当前item索引号*/
    uint32                  selected_cnt;    /*!< 选中item个数*/
    
    BOOLEAN                 is_disptxt_auto; /*!< 
说明字符是由控件组合还是有用户传入*/
    MMI_STRING_T            title_str_info;  /*!< 这个字符串是setlist的说明*/
    MMI_STRING_T            disp_str;        /*!< 
这个字符串用在多选时显示字符*/
    GUI_RECT_T              tp_rect;         /*!< receive tp msg*/
    
    GUISETLIST_ITEM_T       item_info;       /*!< item数据指针*/

    MMI_WIN_ID_T            win_id;          /*!< window id*/

    // self member ...
} CTRLSETLIST_OBJ_T;

// setlist class for public or protected method
typedef struct _setlist_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

    // self vtbl ...
} CTRLSETLIST_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get setlist type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T SETLIST_GetType(void);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
