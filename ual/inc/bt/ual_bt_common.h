/*****************************************************************************
** Copyright 2021-2022 Unisoc(Shanghai) Technologies Co.,Ltd                 *
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
** File Name:       ual_bt_common.h                                          *
** Author:          fangfang.yao                                             *
** Date:             01/16/2021                                              *
** Description:    This file is used to define ual bt export TYPE and API    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 01/16/2021           fangfang.yao           Create                         *
******************************************************************************/
#ifndef _UAL_BT_COMMON_H_
#define _UAL_BT_COMMON_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"

/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/
typedef enum
{
    UAL_BT_HFU_AUD_LOCAL,   // set local call volume
    UAL_BT_HFU_AUD_BOTH     // set local and remote call volume
}ual_bt_hfu_aud_type_e;

typedef enum
{
    UAL_BT_HFU_AUD_AG_TO_HF,   //  切换到Dialer本端
    UAL_BT_HFU_AUD_HF_TO_AG    //  切换到手机远端
}ual_bt_hfu_aud_trans_e;

typedef struct _UAL_BT_MSG_Tag
{
    uint32  msg_id;
    uint32  body_size;
    void*   p_body;
} ual_bt_msg_t;

typedef enum{
    UAL_BT_REGISTER_SUCC,
    UAL_BT_REGISTER_INVALID_PARAM,
    UAL_BT_REGISTER_ALREADY_REGISTERED,//已经注册过
    UAL_BT_REGISTER_NO_SPACE,//不能分配节点空间
    UAL_BT_REGISTER_HANDLE_NO_MATCH,
    UAL_BT_REGISTER_HANDLE_FULL,//handle的个数已经达到BTSRV_HANDLE_MAX_NUM
    UAL_BT_REGISTER_PROFILE_ERR,
    UAL_BT_REGISTER_MAX
}ual_bt_register_res_e;

typedef enum
{
    UAL_BT_CAUSE_ERROR_NONE,         //正常状态
    UAL_BT_CAUSE_OPERATION_ERROR,    //操作失败
    UAL_BT_CAUSE_MAX
}ual_bt_disconnect_call_cause_e;

typedef unsigned char boolean;// UAL_TODO : 放在ual更common的地方

typedef boolean(* _ual_bt_register_callback )(ual_bt_msg_t param);

#ifndef local
//#define local static // UAL_TODO : 放在ual更common的地方
#endif

typedef enum
{
    UAL_BT_STATUS_SUCCESS = 0,
    UAL_BT_STATUS_ERROR,
    UAL_BT_STATUS_PENDING,
    UAL_BT_STATUS_BUSY,
    UAL_BT_STATUS_NO_RESOURCE,
    UAL_BT_STATUS_NOT_FOUND,
    UAL_BT_STATUS_DEVICE_NOT_FOUND,
    UAL_BT_STATUS_CONNECTION_FAILED,
    UAL_BT_STATUS_TIMEOUT,
    UAL_BT_STATUS_NO_CONNECTION,
    UAL_BT_STATUS_INVALID_PARAM,
    UAL_BT_STATUS_NO_SUPPPORTED,
    UAL_BT_STATUS_CANCELLED,
    UAL_BT_STATUS_IN_PROGRESS,
    UAL_BT_STATUS_RESTRICTED,
    UAL_BT_STATUS_INVALID_HANDLE,
    UAL_BT_STATUS_PACKET_TOO_SMALL,
    UAL_BT_STATUS_NO_PAIR,
    UAL_BT_STATUS_FAILED,
    UAL_BT_STATUS_DISCONNECT,
    UAL_BT_STATUS_NO_CONNECT,
    UAL_BT_STATUS_IN_USE,
    UAL_BT_STATUS_MEDIA_BUSY,
    UAL_BT_STATUS_OFF,
    UAL_BT_STATUS_MAX
}ual_bt_status_e;

typedef enum // UAL_TODO : 当UAL消息传递不使用mmk机制时， 此消息变成BTSRV_内部消息
{
    //HFU Message begin (BT Dialer)
    UAL_BT_MSG_HFU_START = ((0x98 << 8) | 1),
    UAL_BT_MSG_HFU_SLC_CONNECTED,
    UAL_BT_MSG_HFU_SLC_DISCONNECTED,
    UAL_BT_MSG_HFU_DIAL_NUMBER_RESULT_IND,
    UAL_BT_MSG_HFU_CIEV_IND,
    UAL_BT_MSG_HFU_AUDIO_CONNECTED,
    UAL_BT_MSG_HFU_AUDIO_DISCONNECTED,
    UAL_BT_MSG_HFU_CLCC_RECORD_IND,
    UAL_BT_MSG_HFU_CLCC_RESULT_IND,
    UAL_BT_MSG_HFU_CLIP_IND,
    UAL_BT_MSG_HFU_RING_IND,
    UAL_BT_MSG_HFU_ATA_RESULT_IND,
    UAL_BT_MSG_HFU_HANGUP_RESULT_IND,
    UAL_BT_MSG_HFU_SEND_DTMF_RESULT_IND,
    UAL_BT_MSG_HFU_CIND_IND,
    UAL_BT_MSG_HFU_CALLHOLD_RESULT_IND,
    UAL_BT_MSG_HFU_SPK_VOL_IND,
    UAL_BT_MSG_HFU_MIC_VOL_IND,
    UAL_BT_MSG_HFU_CCWA_IND,
    UAL_BT_MSG_HFU_ENABLE_VOICE_RECOGN_RESULT_IND,
    UAL_BT_MSG_HFU_VOICE_RECOGN_STATE_IND,
    UAL_BT_MSG_HFU_REDIAL_RESULT_IND,
    UAL_BT_MSG_HFU_MEMORY_DIAL_RESULT_IND,
    UAL_BT_MSG_HFU_END,
    //HFU Message end
    UAL_BT_MSG_MAX
}ual_bt_msg_e;

typedef enum
{
    UAL_BT_SLC_CONNECTED_IND = ((0x98 << 8) | 1) + 300,//992D
    UAL_BT_SLC_DISCONNECTED_IND,
    UAL_BT_AUDIO_CONNECTED_IND,
    UAL_BT_AUDIO_DISCONNECTED_IND,
    UAL_BT_MO_PRE_START_IND ,
    UAL_BT_MO_START_IND,
    UAL_BT_MT_START_IND,
    UAL_BT_MO_ALERT_IND,
    UAL_BT_MO_CONNECTED_IND,
    UAL_BT_MT_CONNECTED_IND,
    UAL_BT_DISCONNECT_IND,
    UAL_BT_HOLD_IND,
    UAL_BT_RETRIEVED_IND,
    UAL_BT_BUILD_MPTY_IND,
    UAL_BT_OPERATION_ERROR_IND,
    UAL_BT_CALL_MSG_MAX
} ual_bt_call_msg_e;

/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/

#endif
