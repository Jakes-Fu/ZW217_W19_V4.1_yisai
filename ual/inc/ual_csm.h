/*****************************************************************************
** Copyright 2022 Unisoc(Shanghai) Technologies Co.,Ltd                      *
** Licensed under the Unisoc General Software License,                       *
** version 1.0 (the License);                                                *
** you may not use this file except in compliance with the License.          *
** You may obtain a copy of the License at                                   *
** https://www.unisoc.com/en_us/license/UNISOC_GENERAL_LICENSE_V1.0-EN_US    *
** Software distributed under the License is distributed on an "AS IS" BASIS,*
** WITHOUT WARRANTIES OF ANY KIND, either express or implied.                *
** See the Unisoc General Software License, version 1.0 for more details.    *
******************************************************************************/

/*****************************************************************************
** File Name:       ual_csm.h                                                *
** Author:            zirui.li                                               *
** Date:            03/11/2021                                               *
** Description:    This file is used to define uil csm                       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 03/11/2021        zirui.li                  Create                        *
******************************************************************************/
#ifndef _UAL_CSM_H_
#define _UAL_CSM_H_
#include "ual_type.h"
#include "ual_common.h"

typedef uint32 ual_csm_handle;

//Action函数指针
typedef uint32 (*csm_action_func) (ual_csm_handle csm_handle, uint32 event_id, void* p_param, uint32 param_size);


//状态机迁移结构体
typedef struct
{
    uint32 current_state;    //当前状态
    uint32 event_id;        //event id
    csm_action_func p_action_func;    //action函数指针
    uint32 to_state;        //去向的状态
}ual_csm_statetransfer_t;

//ual csm模块返回的错误码
typedef enum
{
    UAL_CSM_RES_SUCCESS,      //成功
    UAL_CSM_RES_PARAM_ERR = UAL_MODULE_CSM<<16,   //参数错误
    UAL_CSM_RES_HANDLE_INVALID,  //状态机对象handle无效
    UAL_CSM_RES_MALLOC_FAIL,     //malloc空间失败
    UAL_CSM_RES_STATE_ERR,       //状态错误
    UAL_CSM_RES_CREATE_MUTEX_FAIL,  //创建锁失败
    UAL_CSM_RES_CREATE_NODE_FAIL,   //创建链表节点失败
    UAL_CSM_RES_REGISTER_CMS_FAIL,   //注册cms失败
    UAL_CSM_RES_MAX
}ual_csm_result_e;

//状态机event结构体
typedef struct
{
    uint32 csm_handle;    //状态机handle
    uint32 event_id;      //event id
    void*  p_param;    //event参数
    uint32 param_size;    //event size
}ual_csm_event_t;

/********************************************************************/
// Description:初始化csm
// Parameter:[In] none
//           [Out] none
//           [Return] none
// Author:zirui.li
// Note:
/********************************************************************/
void ual_csm_init(void);

/***************************************************************/
// Description:创建状态机的接口
// Parameter:[In] p_csm_handle : fsm句柄的指针
//                      init_state : 初始状态
//                      p_table : 状态迁移表的指针
//                      table_item_num : 状态迁移表的项数
//           [Out] p_csm_handle : fsm句柄的指针
//           [Return] 错误码
// Author:zirui.li
// Note:
/***************************************************************/
ual_csm_result_e ual_csm_create(ual_csm_handle*          p_csm_handle,
                       uint32                   init_state,
                       uint32                   terminate_state,
                       ual_csm_statetransfer_t* p_table,
                       uint32                   table_item_num);

/***************************************************************/
// Description:销毁状态机的接口
// Parameter: [In] csm_handle : 状态机句柄
//            [Out] 无
//            [Return] 错误码
// Author:zirui.li
// Note:
/***************************************************************/
ual_csm_result_e ual_csm_destroy(ual_csm_handle csm_handle);

/***********************************************/
// Description:csm同步处理event接口
// Parameter: [In] csm_handle : 状态机句柄
//                 event : event id
//                 p_param : event参数
//                 param_size : 参数 size
//            [Out] p_err_code:错误码
//            [Return] 错误码
// Author:zirui.li
// Note:
/***************************************************************/
ual_csm_result_e ual_csm_state_control(ual_csm_handle csm_handle,
                              uint32         event,
                              void*          p_param,
                              uint32         param_size,
                              uint32*        p_err_code);

/************************************************************************/
// Description:异步发送消息到当前task
// Parameter: [In] csm_handle : 状态机句柄
//                 event : event id
//                 param : event参数
//                 param_size : param size
//            [Out] 无
//            [Return] 错误码
// Author:zirui.li
// Note:
/************************************************************************/
ual_csm_result_e ual_csm_state_control_async(ual_csm_handle csm_handle,
                                       uint32         event,
                                       void*          p_param,
                                       uint32         param_size);

/***********************************************************************/
// Description:获取当前状态的接口
// Parameter: [In] csm_handle : 状态机句柄
//            [Out] p_state: 当前状态的指针
//            [Return] 错误码
// Author:zirui.li
// Note:
/***********************************************************************/
ual_csm_result_e ual_csm_get_cur_state(ual_csm_handle csm_handle, uint32* p_state);

/****************************************************************/
// Description:Is csm handle valid
// Parameter: [In] csm_handle : 状态机句柄
//            [Out] None
//            [Return] boolean
// Author:zirui.li
// Note:
/****************************************************************/
boolean ual_csm_is_csmhandle_valid(ual_csm_handle csm_handle);

#endif
