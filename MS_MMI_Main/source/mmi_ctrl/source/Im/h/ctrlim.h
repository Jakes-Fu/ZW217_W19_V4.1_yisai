/*****************************************************************************
** File Name:      ctrlim.h                                               *
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

#ifndef _CTRLIM_H_
#define _CTRLIM_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrlim_export.h"

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
// im type info
#define CTRL_IM_TYPE     (IM_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
// im init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;
    GUIIM_LANGUAGE_T lang_id; /*!< 初始化输入语言 */
    GUIIM_INPUT_TYPE_T input_type; /*!< 初始化输入方式 */
    GUIIM_TYPE_T im_type; /*!< 初始化输入法 */
    GUIIM_CAPITAL_MODE_T cap_mode; /*!< 初始化大小写模式 */
    
    GUIIM_INPUT_TYPE_T input_type_set; /*!< 开启输入方式集合 */
    GUIIM_TYPE_T im_type_set; /*!< 开启输入法集合 */
    
    BOOLEAN is_ban_symbol; /*!< 是否禁止符号输入 */
    GUIIM_TAG_T tag; /*!< 输入法的附加属性 */

    GUIIM_LANGUAGE_T set_input_lang; /*!< 设置输入法默认语言 */
    
    GUIIM_GetContextFunction_T f_get_context; /*!< 获取上下文函数 */
    GUIIM_GetCapacityFunction_T f_get_capacity; /*!< 获取输入字符容量函数 */

    // self inif param ...
} CTRLIM_INIT_PARAM_T;

//im control
typedef struct 
{
    CTRLBASE_OBJ_T              base_ctrl;
    
    MMI_HANDLE_T win_handle; /*!< 
我们的平台，控件是基于窗口的，所以必须继承父控件或者父窗口 */
    MMI_WIN_ID_T win_id; /*!< 窗口id是平台的遗留信息 */

    GUIIM_INIT_PARAM_T init_param; /*!< 保存初始化参数 */
    
    void* im_handle; /*!< 当前输入法应用的实例数据 */
    GUIIM_APIS_PTR_T apis_ptr; /*!< 当前输入法应用的函数列表入口 */

    GUIIM_METHOD_PARAM_T method_param; /*!< 输入法设置的参数 */
    BOOLEAN is_default_method; /*!< 
如果是获取的默认输入法，则需要做相应的设定 */
    
    GUIIM_NOTIFY_QUEUE_T notify; /*!< 反馈信息队列 */
    GUIIM_COMMIT_DATA_T last_commit; /*!< 最后提交信息，用于遗留信息处理 */
    GUIIM_EDIT_STATE_T edit_state_bak; /*!< 输入法状态控制标志，用户控制
softkey等 */

    GUIIM_CUSTOM_KEY_PARAM_T *custom_key_ptr; /*!< 定制按键信息 */

    GUIIM_APPEND_WIN_NODE_T *append_win_ptr; /*!< 输入法控件相关窗口表 */
    BOOLEAN is_msg_band;

} CTRLIM_OBJ_T;

// im class for public or protected method
typedef struct _im_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

    // self vtbl ...
} CTRLIM_VTBL_T;
/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get im type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T IM_GetType(void);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
