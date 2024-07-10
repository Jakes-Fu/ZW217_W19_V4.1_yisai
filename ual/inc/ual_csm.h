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

//Action����ָ��
typedef uint32 (*csm_action_func) (ual_csm_handle csm_handle, uint32 event_id, void* p_param, uint32 param_size);


//״̬��Ǩ�ƽṹ��
typedef struct
{
    uint32 current_state;    //��ǰ״̬
    uint32 event_id;        //event id
    csm_action_func p_action_func;    //action����ָ��
    uint32 to_state;        //ȥ���״̬
}ual_csm_statetransfer_t;

//ual csmģ�鷵�صĴ�����
typedef enum
{
    UAL_CSM_RES_SUCCESS,      //�ɹ�
    UAL_CSM_RES_PARAM_ERR = UAL_MODULE_CSM<<16,   //��������
    UAL_CSM_RES_HANDLE_INVALID,  //״̬������handle��Ч
    UAL_CSM_RES_MALLOC_FAIL,     //malloc�ռ�ʧ��
    UAL_CSM_RES_STATE_ERR,       //״̬����
    UAL_CSM_RES_CREATE_MUTEX_FAIL,  //������ʧ��
    UAL_CSM_RES_CREATE_NODE_FAIL,   //��������ڵ�ʧ��
    UAL_CSM_RES_REGISTER_CMS_FAIL,   //ע��cmsʧ��
    UAL_CSM_RES_MAX
}ual_csm_result_e;

//״̬��event�ṹ��
typedef struct
{
    uint32 csm_handle;    //״̬��handle
    uint32 event_id;      //event id
    void*  p_param;    //event����
    uint32 param_size;    //event size
}ual_csm_event_t;

/********************************************************************/
// Description:��ʼ��csm
// Parameter:[In] none
//           [Out] none
//           [Return] none
// Author:zirui.li
// Note:
/********************************************************************/
void ual_csm_init(void);

/***************************************************************/
// Description:����״̬���Ľӿ�
// Parameter:[In] p_csm_handle : fsm�����ָ��
//                      init_state : ��ʼ״̬
//                      p_table : ״̬Ǩ�Ʊ��ָ��
//                      table_item_num : ״̬Ǩ�Ʊ������
//           [Out] p_csm_handle : fsm�����ָ��
//           [Return] ������
// Author:zirui.li
// Note:
/***************************************************************/
ual_csm_result_e ual_csm_create(ual_csm_handle*          p_csm_handle,
                       uint32                   init_state,
                       uint32                   terminate_state,
                       ual_csm_statetransfer_t* p_table,
                       uint32                   table_item_num);

/***************************************************************/
// Description:����״̬���Ľӿ�
// Parameter: [In] csm_handle : ״̬�����
//            [Out] ��
//            [Return] ������
// Author:zirui.li
// Note:
/***************************************************************/
ual_csm_result_e ual_csm_destroy(ual_csm_handle csm_handle);

/***********************************************/
// Description:csmͬ������event�ӿ�
// Parameter: [In] csm_handle : ״̬�����
//                 event : event id
//                 p_param : event����
//                 param_size : ���� size
//            [Out] p_err_code:������
//            [Return] ������
// Author:zirui.li
// Note:
/***************************************************************/
ual_csm_result_e ual_csm_state_control(ual_csm_handle csm_handle,
                              uint32         event,
                              void*          p_param,
                              uint32         param_size,
                              uint32*        p_err_code);

/************************************************************************/
// Description:�첽������Ϣ����ǰtask
// Parameter: [In] csm_handle : ״̬�����
//                 event : event id
//                 param : event����
//                 param_size : param size
//            [Out] ��
//            [Return] ������
// Author:zirui.li
// Note:
/************************************************************************/
ual_csm_result_e ual_csm_state_control_async(ual_csm_handle csm_handle,
                                       uint32         event,
                                       void*          p_param,
                                       uint32         param_size);

/***********************************************************************/
// Description:��ȡ��ǰ״̬�Ľӿ�
// Parameter: [In] csm_handle : ״̬�����
//            [Out] p_state: ��ǰ״̬��ָ��
//            [Return] ������
// Author:zirui.li
// Note:
/***********************************************************************/
ual_csm_result_e ual_csm_get_cur_state(ual_csm_handle csm_handle, uint32* p_state);

/****************************************************************/
// Description:Is csm handle valid
// Parameter: [In] csm_handle : ״̬�����
//            [Out] None
//            [Return] boolean
// Author:zirui.li
// Note:
/****************************************************************/
boolean ual_csm_is_csmhandle_valid(ual_csm_handle csm_handle);

#endif
