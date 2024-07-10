/*****************************************************************************
** File Name:      ctrlstatusbar.h                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2012     xiyuan.ma      Create
******************************************************************************/

#ifndef _CTRLSTATUSBAR_H_
#define _CTRLSTATUSBAR_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrlstatusbar_export.h"

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
// statusbar type info
#define CTRL_STATUSBAR_TYPE (STATUSBAR_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
// statusbar init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;

    // self inif param ...
} CTRLSTATUSBAR_INIT_PARAM_T;

//statusbar control
typedef struct _statusbar_obj_tag
{
    CTRLBASE_OBJ_T          base_ctrl;

    uint8                   timer_id;		                         /*!< 时钟 id*/
    uint8                   scroll_msg_timer_id;		 /*!< 滚动信息时钟 id*/
    MMI_STATUSBAR_STYLE_T   style_info;                               /*!< 默认风格信息*/
    GUISTATUSBAR_STATE_T    ctrl_state;                               /*!< 控件状态*/
    GUISTATUSBAR_TYPE_E     ctrl_type;                                /*!< 控件类型，水平显示*/
    UILAYER_APPEND_BLT_T    statusbar_layer;                     /*!< 状态栏支持自创建*/
    BOOLEAN                 is_display_bg;  			/*!< 是否显示背景，如果不显示，则直接使用窗口背景*/
    BOOLEAN                 is_shown;                               /*!< 是否显示*/
    BOOLEAN                 clear_customer_bg;               /*!< 是否清除用户层的背景*/
#if defined PDA_UI_DROPDOWN_WIN || defined DROPDOWN_NOTIFY_SHORTCUT
    BOOLEAN                 is_press_down;
    BOOLEAN                 dropdown_state;
#endif
} CTRLSTATUSBAR_OBJ_T;

// statusbar class for public or protected method
typedef struct _statusbar_vtbl_tag
{
    CTRLBASE_VTBL_T         base_ctrl_vtbl;

    // self vtbl ...
} CTRLSTATUSBAR_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get statusbar type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T STATUSBAR_GetType (void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif
