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
** File Name:       ual_bt_hfu.h                                             *
** Author:          fangfang.yao                                             *
** Date:            2021/01/08                                               *
** Description:     This file is used to define export ual api of bt hfu     *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
**   DATE                 NAME              DESCRIPTION                      *
** 01/08/2021           fangfang.yao          Create                         *
******************************************************************************/

#ifndef _UAL_BT_HFU_H_
#define _UAL_BT_HFU_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "ual_bt_common.h"
#include "ual_cms_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define UAL_BT_MAX_TELE_NUM_LEN (40)

/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description: 实现HFU init
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
void ual_bt_hfu_init( void );

/*****************************************************************************/
//  Description: 实现HFU注册功能
//  Parameter: [In] p_callback : 回调函数
//             [In] p_regiser_handle: HFU处理handle id
//             [Out] None
//             [Return] 返回注册事件处理的结果
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
boolean ual_bt_hfu_register(_ual_cms_client_register_callback *p_callback , uint32 *p_regiser_handle);

/*****************************************************************************/
//  Description: 实现HFU去注册功能
//  Parameter: [In] hfu_handle : HFU处理handle id
//             [Out] None
//             [Return] TRUE: 去注册成功 ; FALSE: 去注册失败
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
boolean ual_bt_hfu_unregister(uint32 hfu_handle);

/*****************************************************************************/
//  Description: 获取HFU连接状态
//  Parameter: [In] None
//             [Out] None
//             [Return] 返回事件处理的结果
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
boolean ual_bt_hfu_get_connect_status(void);

/*****************************************************************************/
//  Description: 发送DTMF
//  Parameter: [In] dtmf_ch :  待发送的单个DTMF字符
//             [Out] None
//             [Return] 返回事件处理的错误类型
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_send_dtmf(uint8 dtmf_ch);

/*****************************************************************************/
//  Description: 对来电请求进行应答
//  Parameter: [In] None
//             [Out] None
//             [Return] 返回事件处理的错误类型
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_anwser_call(void);

/*****************************************************************************/
//  Description: 发送呼叫号码
//  Parameter: [In] p_phone_num :  待发送的号码
//             [Out] None
//             [Return] 返回事件处理的错误类型
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_make_call(const uint8 *p_phone_num);

/*****************************************************************************/
//  Description: 单通通话挂断功能
//  Parameter: [In] None
//             [Out] None
//             [Return] 返回事件处理的错误类型
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_release_call(void);

/*****************************************************************************/
//  Description: 全部通话挂断功能
//  Parameter: [In] None
//             [Out] None
//             [Return] 返回事件处理的错误类型
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_release_allcall(void);

/*****************************************************************************/
//  Description: 返回当前通话信息
//  Parameter: [In] p_phone_num : 当前通话的号码
//             [In] p_num_len：当前通话号码的长度
//             [Out] None
//             [Return] TRUE: 获取成功 ; FALSE: 获取失败
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
boolean  ual_bt_get_call_info( 
                                uint8 *p_phone_num,
                                uint8 *p_num_len
                                );
/*****************************************************************************/
//  Description: 返回当前通话开始时间
//  Parameter: [In/Out] p_stat_time : 当前通话的开始时间
//             [Return] TRUE: 获取成功 ; FALSE: 获取失败
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
boolean ual_bt_get_current_call_start_time( uint32 *p_stat_time );

/*****************************************************************************/
//  Description : get whether exist bt call
//  Parameter: [In] None
//             [Out] None
//             [Return] s_bt_call_flag
//  Author: fangfang.yao
//  Note: 
/*****************************************************************************/
boolean ual_bt_get_bt_call_flag(void);

/*****************************************************************************/
//  Description : set whether exist  bt call
//  Parameter: [In] bt_call_flag
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note: 
/*****************************************************************************/
void ual_bt_set_bt_call_flag(boolean bt_call_flag);

/*****************************************************************************/
//  Description : set whether is exist incoming call
//  Parameter: [In] None
//             [Out] None
//             [Return] TRUE : 存在正在呼入的电话 ; FALSE : 不存在
//  Author: fangfang.yao
//  Note: 
/*****************************************************************************/
boolean ual_bt_is_exist_incoming_call(void);

/*****************************************************************************/
//  Description : set whether is exist outgoing call
//  Parameter: [In] None
//             [Out] None
//             [Return] TRUE : 存在正在通话拨出的电话 ; FALSE : 不存在
//  Author: fangfang.yao
//  Note: 
/*****************************************************************************/
boolean ual_bt_is_exist_outgoing_call(void);

/*****************************************************************************/
//  Description : set whether is exist active call
//  Parameter: [In] None
//             [Out] None
//             [Return] TRUE : 存在正在通话的电话 ; FALSE : 不存在
//  Author: fangfang.yao
//  Note: 
/*****************************************************************************/
boolean ual_bt_is_exist_active_call(void);

/*****************************************************************************/
//  Description : set whether is exist active call
//  Parameter: [In] None
//             [Out] None
//             [Return] TRUE : 存在正在保持的电话 ; FALSE : 不存在
//  Author: fangfang.yao
//  Note: 
/*****************************************************************************/
boolean ual_bt_is_exist_hold_call(void);

/*****************************************************************************/
//  Description : set whether is exist mpty call
//  Parameter: [In] None
//             [Out] None
//             [Return] TRUE : 存在会议电话 ; FALSE : 不存在
//  Author: fangfang.yao
//  Note: 
/*****************************************************************************/
boolean ual_bt_is_exist_mpty_call(void);

/*****************************************************************************/
//  Description : return current index
//  Parameter: [In] None
//             [Out] None
//             [Return] current index
//  Author: fangfang.yao
//  Note: 
/*****************************************************************************/
uint8 ual_bt_get_current_index(void);

/*****************************************************************************/
//  Description : 设置下行通话音量
//  Parameter: [In] volume
//             [In] type LOCAL:只设置本端 BOTH:同时设置本端和远端
//             [Out] None
//             [Return] 成功/失败
//  Author: bin.wang1
//  Note: 
/*****************************************************************************/
boolean ual_bt_hfu_set_volume( ual_bt_hfu_aud_type_e type, uint32 volume );

/*****************************************************************************/
//  Description : 获取下行通话音量
//  Parameter: [In] None
//             [Out] None
//             [Return] 下行通话音量
//  Author: bin.wang1
//  Note: 
/*****************************************************************************/
uint32 ual_bt_hfu_get_volume( void );

/*****************************************************************************/
//  Description : 设置上行通话静音
//  Parameter: [In] volume
//             [In] type LOCAL:只设置本端 BOTH:同时设置本端和远端
//             [Out] None
//             [Return] 成功/失败
//  Author: bin.wang1
//  Note: 
/*****************************************************************************/
boolean ual_bt_hfu_set_mute( ual_bt_hfu_aud_type_e type, boolean is_mute );

/*****************************************************************************/
//  Description : 获取上行通话是否静音
//  Parameter: [In] None
//             [Out] None
//             [Return] 上行通话是否静音
//  Author: bin.wang1
//  Note: 
/*****************************************************************************/
boolean ual_bt_hfu_get_is_mute( void );

/*****************************************************************************/
//  Description : 切换音频连接通道 本端/远端
//  Parameter: [In] direction
//             [Out] None
//             [Return] 切换是否成功
//  Author: bin.wang1
//  Note: 
/*****************************************************************************/
boolean ual_bt_hfu_trans_aud_conn( ual_bt_hfu_aud_trans_e direction );

/*****************************************************************************/
//  Description : 当前通话声音是否在本端播出
//  Parameter: [In] None
//             [Out] None
//             [Return] 当前通话声音是否在本端播出
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
boolean ual_bt_hfu_aud_is_conn( void );

/*****************************************************************************/
//  Description:实现HFU连接功能
//  Parameter: [In] p_addr : bt dialer 地址
//             [Out] None
//             [Return] 返回事件处理的错误类型
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_hfu_connect ( uint8 *p_addr );

/*****************************************************************************/
//  Description: 实现HFU去连接功能
//  Parameter: [In] None
//             [Out] None
//             [Return] 返回事件处理的错误类型
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_hfu_disconnect( void );

/*****************************************************************************/
//  Description : return total call num
//  Parameter: [In] None
//             [Out] None
//             [Return] total call num
//  Author: fangfang.yao
//  Note: 
/*****************************************************************************/
uint8 ual_bt_get_total_call_num(void);

#endif
