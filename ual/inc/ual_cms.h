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
** File Name:      ual_cms.h
** Author: Xiuyun.Wang
** Description:
*****************************************************************************/

#ifndef _UAL_CMS_H_
#define _UAL_CMS_H_
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "ual_common.h"
#include "ual_type.h"

#ifndef THIN_MODEM
#include "sig_code.h"
#endif

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#ifdef THIN_MODEM
#define UAL_CMS_GROUP 0xCC //signal code中同步分配
#endif
#define UAL_CMS_SIGNAL_CODE UAL_CMS_GROUP << 8 | UAL_CMS_GROUP //通过此signalcode拦截app task中消息分发中的UAL CMS的消息
#define UAL_CMS_CLIENT_LISTEN_ALL_MSG_IN_MODULE  0xFFFF //当前module下的所有message

#define UAL_CMS_RPC_EVENT_ID (UAL_CMS_GROUP<<16| UAL_CMS_GROUP)

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/


typedef struct
{
    ual_cms_msg_t *p_msg_body;
    _ual_cms_client_register_callback callback;
} ual_cms_rpc_event_t;

/*client register data format define*/
typedef struct
{
    uint32                           client_listen_start_msg_id;        /* 注册的client关注的消息段起始msg_id */
    uint32                           client_listen_end_msg_id;          /* 注册的client关注的消息段终止msg_id  */
}ual_cms_client_listen_msg_t;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DECLARE                                 *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : dispatch the meesage to every module
//  Parameter: [In] signal_id: the signal id dispatched from app task
//             [In] p_dispatch_msg: the msg data with current sinal
//             [Out] NONE
//             [Return]NONE
//  Author:xiuyun.wang
// Note:
/*****************************************************************************/
void ual_cms_msg_dispatch(void * p_dispatch_msg);

#endif
