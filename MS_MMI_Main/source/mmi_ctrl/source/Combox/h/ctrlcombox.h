/*****************************************************************************
** File Name:      ctrlcombox.h                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      xiyuan.ma & nan.ji              Create
******************************************************************************/

#ifndef _CTRLCOMBOX_H_
#define _CTRLCOMBOX_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrlcombox_export.h"
#include "ctrltextedit_export.h"
//#include "ctrldropdownlist_export.h"

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
// combox type info
#define CTRL_COMBOX_TYPE     (COMBOX_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// combox init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;
    uint16                      page_item_num; /*!< 每页显示最大数，即下拉后所能显示的数*/
    CTRLCOMBOX_SHOW_TYPE_E       expand_direction; /*!< 打开下拉列表时显示方式*/
    // self inif param ...
} CTRLCOMBOX_INIT_PARAM_T;

//combox control
typedef struct _combox_obj_tag
{
    CTRLBASE_OBJ_T              base_ctrl;

    //control setting
    GUI_RECT_T   box_rect;      /*!< list区域*/
    CTRLCOMBOX_SHOW_TYPE_E expand_direction;
    uint16       left_space;
    uint16       box_list_space;
    BOOLEAN      is_right_aligned; //??

    BOOLEAN      is_list_fixed;
    BOOLEAN      is_dirty;
    BOOLEAN      is_mid_softkey_open_list;
    BOOLEAN      is_web_key_open_list;

    //icon setting
    uint32       icon_id;
    BOOLEAN      has_icon;

    //text setting
    MMI_HANDLE_T text_handle;
    CTRLCOMBOX_TEXT_TYPE_T edit_info;

    //expand button setting
    MMI_HANDLE_T expand_button_handle;
    MMI_IMAGE_ID_T    unpressed_arrow_icon;   //未按下的下拉箭头图标
    MMI_IMAGE_ID_T    pressed_arrow_icon;     //已按下的下拉箭头图标
    BOOLEAN      has_expand_button;

    //function button setting
    MMI_HANDLE_T function_button_handle;
    BOOLEAN      has_function_button;
    MMI_IMAGE_ID_T    func_button_icon;     //功能按钮图标

    //list setting
    MMI_HANDLE_T list_handle;
    BOOLEAN      is_list_expand; // 内部，无需客户设置
    uint16       page_item_num;
    uint16       cur_page_item_num;
    uint16       item_height;

    // self member ...
} CTRLCOMBOX_OBJ_T;

// combox class for public or protected method
typedef struct _combox_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

    // self vtbl ...
} CTRLCOMBOX_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get combox type
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T COMBOX_GetType (void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif
