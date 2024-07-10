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

/*******************************************************************************
** File Name:       ual_tele_call.h                                            *
** Author:          donglin.su                                                 *
** Date:             18/08/2022                                                *
** Description:    This file is used to define ual tele export api             *
********************************************************************************
**                         Important Edit History                              *
** ----------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                      *
** 18/08/2022          donglin.su               Create                         *
********************************************************************************/
#ifndef _UAL_TELE_CALL_H_
#define _UAL_TELE_CALL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define UAL_TELE_CALL_TELE_NUM_MAX        40
#define UAL_TELE_CALL_TELE_NAME_MAX_LEN   40
#define UAL_TELE_CALL_ECC_MAX_COUNT       (20)
#define UAL_TELE_CALL_ECC_NUM_MAX_LEN     (10)

#define UAL_TELE_CALL_COMMON_BASE_ATTR \
                uint32              sim_id; \
                uint8               call_id; \
                char                tele_num[UAL_TELE_CALL_TELE_NUM_MAX + 1]; \
                uint8               tele_num_len; \
                boolean             is_ps_call;


/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
/*                          ENUM AND STRUCT                                  *
/*---------------------------------------------------------------------------*/
typedef enum
{
    UAL_TELE_CALL_RES_SUCCESS = UAL_TELE_RES_SUCCESS,
    UAL_TELE_CALL_RES_ERROR = UAL_MODULE_TELE_CALL <<16,
    UAL_TELE_CALL_RES_ERR_NUM_NULL, //号码为空
    UAL_TELE_CALL_RES_ERR_NUM_INVALID, //号码格式错误
    UAL_TELE_CALL_RES_ERR_SIM_ID, //SIM ID error
    UAL_TELE_CALL_RES_ERR_CRSS, //CRSS解析错误
    UAL_TELE_CALL_RES_ERR_BUSY, //MO/MT/呼叫中
    UAL_TELE_CALL_RES_ERR_FLY_MODE, //飞行模式开启
    UAL_TELE_CALL_RES_ERR_ECC_IN_FLY_MODE, //紧急呼叫且飞行模式开启
    UAL_TELE_CALL_RES_ERR_NETWORK_LIMIT, //网络状态为无服务或注册中
    UAL_TELE_CALL_RES_ERR_ECC_ONLY, //仅限紧急呼叫
    UAL_TELE_CALL_RES_ERR_SIM_NOT_READY, //SIM状态异常
    UAL_TELE_CALL_RES_ERR_CALL_FULL, //呼叫数目达到上限
    UAL_TELE_CALL_RES_ERR_IMS_NOT_READY, //IMS没有成功注册
    UAL_TELE_CALL_RES_ERR_ECC_NOT_SUPPORT,//不支持紧急呼叫
    UAL_TELE_CALL_RES_MAX
}ual_tele_call_result_e;

//定义单通通话的状态
typedef enum
{
    UAL_TELE_CALL_STATE_NULL,       //空闲状态
    UAL_TELE_CALL_STATE_OUTGOING,   //正在呼出
    UAL_TELE_CALL_STATE_INCOMING,   //来电中
    UAL_TELE_CALL_STATE_ACTIVE,     //通话中
    UAL_TELE_CALL_STATE_HOLD,       //挂起中
    UAL_TELE_CALL_STATE_MAX
} ual_tele_call_state_e;

//事件定义
typedef enum
{
    MSG_UAL_TELE_CALL_MO_START_IND = UAL_MODULE_TELE_CALL << 16,     //协议栈已经受理呼叫发起请求
    MSG_UAL_TELE_CALL_MT_START_IND,     //本机接收到来电建立请求
    MSG_UAL_TELE_CALL_MO_ALERT_IND,     //远端已经进入振铃阶段
    MSG_UAL_TELE_CALL_MO_CONNECTED_CNF, //去电建立成功
    MSG_UAL_TELE_CALL_MT_CONNECTED_CNF, //来电建立成功
    MSG_UAL_TELE_CALL_DISCONNECTED_CNF, //通话主动挂断成功
    MSG_UAL_TELE_CALL_DISCONNECTED_IND, //通话被动挂断成功，对方挂断或网络原因断开
    MSG_UAL_TELE_CALL_HOLD_CNF,         //通话挂起
    MSG_UAL_TELE_CALL_UNHOLD_CNF,       //通话恢复
    MSG_UAL_TELE_CALL_AMR_TYPE_IND,     //是否显示HD
    MSG_UAL_TELE_CALL_HO_PROGRESS_IND,  //回落通知
    MSG_UAL_TELE_CALL_QUERY_DTMF_IND,   //通话建立后，若需要询问是否发送DTMF字符串，则发送此事件询问
    MSG_UAL_TELE_CALL_START_DTMF_IND,   //开始发送DTMF字符串
    MSG_UAL_TELE_CALL_SEND_DTMF_CNF,    //发送一条字符串/字符完成
#if defined (BT_HFG_SUPPORT)
    MSG_UAL_TELE_CALL_BT_AUDIO_VOL_UP_CNF,    //通话过程中，蓝牙设备调高音量
    MSG_UAL_TELE_CALL_BT_AUDIO_VOL_DOWN_CNF,  //通话过程中，蓝牙设备调低音量
    MSG_UAL_TELE_CALL_BT_AUDIO_OPENED_CNF,    //通话过程中，蓝牙设备打开
    MSG_UAL_TELE_CALL_BT_AUDIO_CLOSED_CNF,    //通话过程中，蓝牙设备关闭
#endif
    MSG_UAL_TELE_CALL_MAX
} ual_tele_call_msg_e;

//定义呼出时回铃音的播放来源
typedef enum
{
    UAL_TELE_CALL_ALERT_TYPE_REMOTE,         //远端振铃
    UAL_TELE_CALL_ALERT_TYPE_LOCAL,          //本地振铃
    UAL_TELE_CALL_ALERT_TYPE_MAX
} ual_tele_call_alert_type_e;

//定义通话结束的原因
typedef enum
{
    UAL_TELE_CALL_DISC_CAUSE_ERROR_NONE,
    UAL_TELE_CALL_DISC_CAUSE_NORMAL_CLEARING,//正常挂断
    UAL_TELE_CALL_DISC_CAUSE_USER_BUSY,      //用户正忙
    UAL_TELE_CALL_DISC_CAUSE_OTHER,          //其他
    UAL_TELE_CALL_DISC_CAUSE_MAX
} ual_tele_call_disconnect_cause_e;


typedef enum
{
    UAL_TELE_CALL_AUDIO_DEV_HH,        //听筒
    UAL_TELE_CALL_AUDIO_DEV_EP,        //耳机
    UAL_TELE_CALL_AUDIO_DEV_HF,        //免提
    UAL_TELE_CALL_AUDIO_DEV_BT,        //蓝牙异步设置模式
    UAL_TELE_CALL_AUDIO_DEV_MAX        //枚举最大值
} ual_tele_call_audio_dev_e;

typedef struct
{
    ual_tele_sim_id_e       sim_id; //当前的使用SIM卡ID
    boolean                 is_valid;//本路call_info是否有效
    uint8                   call_id; //本通CALL的ID
    uint8                   tele_num[UAL_TELE_CALL_TELE_NUM_MAX + 1];
    uint8                   tele_num_len;
    boolean                 is_cnap_name_exist;
    uint16                  cnap_name[UAL_TELE_CALL_TELE_NUM_MAX + 1];
    uint8                   cnap_name_len;
    ual_tele_call_state_e   call_state;   //本通CALL的状态
    uint32                  start_call_time; //通话起始的时间,记录绝对时间
    uint32                  end_call_time; //通话结束的时间
    uint8                   call_direction; //MO or MT
    uint16                  call_type; //通话类型
    uint16                  call_route; //发起源
    boolean                 is_ps_call;
    boolean                 is_dtmf_exist;
    uint8                   dtmf_str[UAL_TELE_CALL_TELE_NUM_MAX + 1];
    uint8                   dtmf_len;
}ual_tele_call_data_info_t; //每通CALL的信息

typedef struct
{
    UAL_TELE_CALL_COMMON_BASE_ATTR
} ual_tele_call_mo_start_ind_t;

typedef struct
{
    UAL_TELE_CALL_COMMON_BASE_ATTR

    boolean is_exist_cnap_name; //是否有网络下发的CNAP
    uint16  cnap_name[UAL_TELE_CALL_TELE_NAME_MAX_LEN +1]; //CNAP名称
    uint8   cnap_name_len;      //CNAP名称长度
} ual_tele_call_mt_start_ind_t;

typedef struct
{
    UAL_TELE_CALL_COMMON_BASE_ATTR
    uint32  alert_type; //振铃类型，远端或本地振铃 //ual_tele_call_alert_type_e
} ual_tele_call_mo_alert_ind_t;

typedef struct
{
    UAL_TELE_CALL_COMMON_BASE_ATTR
} ual_tele_call_mo_connected_cnf_t;

typedef struct
{
    UAL_TELE_CALL_COMMON_BASE_ATTR
} ual_tele_call_mt_connected_cnf_t;

typedef struct
{
    UAL_TELE_CALL_COMMON_BASE_ATTR
    uint32 cause; //通话断开原因 //ual_tele_call_disconnect_cause_e
    uint32 start_call_time; //通话起始的时间
    uint32 end_call_time;   //通话结束的时间
    uint8  call_direction;  //MO or MT
} ual_tele_call_disconnected_cnf_t;

typedef struct
{
    UAL_TELE_CALL_COMMON_BASE_ATTR
    uint32 cause; //ual_tele_call_disconnect_cause_e
    uint32 start_call_time; //通话起始的时间
    uint32 end_call_time;   //通话结束的时间
    uint8  call_direction;  //MO or MT
} ual_tele_call_disconnected_ind_t;

typedef struct
{
    UAL_TELE_CALL_COMMON_BASE_ATTR
    boolean is_accepted; //协议栈是否受理请求成功
} ual_tele_call_hold_cnf_t;

typedef struct
{
    UAL_TELE_CALL_COMMON_BASE_ATTR
    boolean is_accepted;
} ual_tele_call_unhold_cnf_t;

typedef struct
{
    uint32              sim_id; //ual_tele_sim_id_e
    uint8               amr_type; /*0:AMR-NB, 1:AMR-WB*/
} ual_tele_call_amr_type_ind_t;

typedef struct
{
    uint32              sim_id; //ual_tele_sim_id_e
    uint8               ho_progress_type; /*0:PS->CS, 1:CS->PS*/
} ual_tele_call_ho_progress_ind_t;


//用来描述单个要发送的字符串
typedef struct
{
    uint32 sim_id; //ual_tele_sim_id_e
    uint8 dtmf_len;
    uint8 dtmf_str[UAL_TELE_CALL_TELE_NUM_MAX + 1];
}ual_tele_call_dtmf_info_t;

typedef struct
{
    ual_tele_call_dtmf_info_t dtmf_info;
} ual_tele_call_query_dtmf_ind_t;

typedef struct
{
    ual_tele_call_dtmf_info_t dtmf_info;
} ual_tele_call_start_dtmf_ind_t;

typedef struct
{
    ual_tele_call_dtmf_info_t dtmf_info;
    boolean is_success; //协议栈是否受理请求成功
} ual_tele_call_send_dtmf_cnf_t;


//设置紧急号码类型
typedef enum
{
    UAL_TELE_CALL_NOT_ECC,       //不是紧急号码
    UAL_TELE_CALL_CPHS_ECC,     //CPHS协议规定的紧急号码，通常是999
    UAL_TELE_CALL_NETWORK_ECC,  //网络下发的紧急号码
    UAL_TELE_CALL_SIM_ECC,      //保存中SIM卡中的紧急号码
    UAL_TELE_CALL_NOSIM_ECC,    //无卡紧急号码
    UAL_TELE_CALL_FAKE_ECC,     //假紧急呼叫，网络状态好的时候，call时传给协议栈normal类型，网络状态不好时，传给底层emergency类型
    UAL_TELE_CALL_CUS_ECC,
    UAL_TELE_CALL_LOCAL_ECC,
    UAL_TELE_CALL_MAX
}ual_tele_call_ecc_type_e;

//设置紧急号码数据结构体
typedef struct
{
    uint32 sim_id; //ual_tele_sim_id_e
    uint8 ecc_num[UAL_TELE_CALL_ECC_NUM_MAX_LEN];  //emmergency number
    uint8 ecc_mum_len;  //emmergency number length
    boolean category_bit_flag;//is have categorybit
    uint8 category_bit;
    uint32 ecc_type; //ual_tele_call_ecc_type_e
}ual_tele_call_ecc_t;

/*外部模块注入的函数定义*/
typedef boolean (*ual_tele_call_injection_func) (uint32 event, void* p_param, uint32 param_size);

/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description:use to make call
// Parameter: [In] dual_sys:sim id
//                 tele_num:tele number
//            [Out] none
//            [Return]ual_tele_call_result_e
//  Author:donglin.su
//  Note:
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_make_call(
    ual_tele_sim_id_e dual_sys,
    uint8 tele_num[UAL_TELE_CALL_TELE_NUM_MAX + 1]);

/*****************************************************************************/
// Description:use to reject current call
// Parameter: [In] none
//            [Out] none
//            [Return]ual_tele_call_result_e
//  Author:donglin.su
//  Note:
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_reject_call(void);

/*****************************************************************************/
// Description:use to release current call
// Parameter: [In] call_id
//            [Out] none
//            [Return]ual_tele_call_result_e
//  Author:donglin.su
//  Note:
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_release_call(uint8 call_id);

/*****************************************************************************/
// Description:use to release all call
// Parameter: [In] none
//            [Out] none
//            [Return]ual_tele_call_result_e
//  Author:donglin.su
//  Note:
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_release_all(void);

/*****************************************************************************/
// Description:use to answer call
// Parameter: [In] none
//            [Out] none
//            [Return]ual_tele_call_result_e
//  Author:donglin.su
//  Note:
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_answer_call(void);

/*****************************************************************************/
// Description:use to release and answer call
// Parameter: [In] none
//            [Out] none
//            [Return]ual_tele_call_result_e
//  Author:donglin.su
//  Note:
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_release_and_answer(void);

/*****************************************************************************/
// Description:use to hold call
// Parameter: [In] none
//            [Out] none
//            [Return]ual_tele_call_result_e
//  Author:donglin.su
//  Note:
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_hold_call(void);

/*****************************************************************************/
// Description:use to unhold call
// Parameter: [In] none
//            [Out] none
//            [Return]ual_tele_call_result_e
//  Author:donglin.su
//  Note:
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_unhold_call(void);

/*****************************************************************************/
// Description: get current call state
// Parameter: [In] none
//            [Out] none
//            [Return]ual_tele_call_state_e
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
ual_tele_call_state_e ual_tele_call_get_current_call_state(void);

/*****************************************************************************/
// Description: get call state by call id
// Parameter: [In] call_id: 协议栈为一路通话分配的id
//            [Out] none
//            [Return]ual_tele_call_state_e
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
ual_tele_call_state_e ual_tele_call_get_call_state(uint8 call_id);

/*****************************************************************************/
//  Description: ual_tele_call_set_cus_emergency_num
//  Parameter: [In] sim id:current sim id
//             [In] p_ecc_data:emergency data info
//             [In] ecc_data_count:emergency data count
//             [Out] none
//             [Return] ual_tele_call_result_e
//  Author: chunjuan.liang
//  Note:app set customise emergency num to ual
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_set_cus_emergency_num(ual_tele_sim_id_e sim_id,ual_tele_call_ecc_t *p_ecc_data,uint16 ecc_data_count);

/*****************************************************************************/
//  Description: ual_tele_call_is_emergency_num
//  Parameter: [In] sim id:current sim id
//             [In] p_tele_num:emergency num
//             [In] tele_num_len:emergency num length
//             [Out] none
//             [Return] BOOLEAN:TRUE:是紧急号码；FALSE:非紧急号码
//  Author: chunjuan.liang
//  Note:input tele number ,judge this number whether is emergency
/*****************************************************************************/
boolean ual_tele_call_is_emergency_num(ual_tele_sim_id_e sim_id,uint8 *p_tele_num,uint8 tele_num_len);

/*****************************************************************************/
//  Description: ual_tele_call_is_part_emergency_num
//  Parameter: [In] sim id:current sim id
//             [In] tele_num:emergency num
//             [In] tele_num_len:emergency num length
//             [Out] none
//             [Return] BOOLEAN:TRUE:是紧急号码；FALSE:非紧急号码
//  Author: chunjuan.liang
//  Note:input 1 or 2,3 number,judge this number whether is emergency
/*****************************************************************************/
boolean ual_tele_call_is_part_emergency_num(ual_tele_sim_id_e sim_id,uint8 *p_tele_num,uint8 tele_num_len);

/*****************************************************************************/
//  Description: ual_tele_call_get_ecc_type
//  Parameter: [In] sim id:current sim id
//             [In] p_tele_num:emergency num
//             [In] tele_num_len:emergency num length
//             [In/Out] p_ecc_type:emergency type
//             [Return] ual_tele_call_result_e:success
//  Author: chunjuan.liang
//  Note:get ecc type,当前只有客户设置，才会有fake ecc
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_get_ecc_type(ual_tele_sim_id_e sim_id,uint8 *p_tele_num,uint16 tele_num_len,ual_tele_call_ecc_type_e *p_ecc_type);

/*****************************************************************************/
//  Description : 发送 stop dtmf
//  Parameter: [In] sim_id: 卡槽号
//             [Out] none
//             [Return] ual_tele_call_result_e:接口的处理结果
//  Author: mingwei.jia
//  Note: APP控制发送stop dtmf时调用此接口
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_stop_single_dtmf(ual_tele_sim_id_e sim_id);

/*****************************************************************************/
//  Description : 发送单个 dtmf 字符
//  Parameter: [In] sim_id: 卡槽号
//             [In] dtmf_char :要发送的一个字符
//             [In] is_stop_immediately :是否立刻停止
//             [Out] none
//             [Return] ual_tele_call_result_e:接口的处理结果
//  Author: mingwei.jia
//  Note: APP控制发送单个字符时调用此接口
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_start_single_dtmf(ual_tele_sim_id_e sim_id,
                                                               uint8 dtmf_char,
                                                               boolean is_stop_immediately);

/*****************************************************************************/
//  Description : 不发送 dtmf 字符串
//  Parameter: [In] sim_id: 卡槽号
//             [Out] none
//             [Return] ual_tele_call_result_e:接口的处理结果
//  Author: mingwei.jia
//  Note: APP控制取消发送一个字符串时调用此接口
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_cancel_string_dtmf(ual_tele_sim_id_e sim_id);

/*****************************************************************************/
//  Description : 发送 dtmf 字符串
//  Parameter: [In] dtmf_ch :待发送的DTMF字符串
//             [Out] none
//             [Return] ual_tele_call_result_e:接口的处理结果
//  Author: mingwei.jia
//  Note: APP控制发送一个字符串时调用此接口
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_send_string_dtmf(ual_tele_call_dtmf_info_t dtmf_info);

/*******************************************************************************/
//  Description: 在通话过程中，切换音频通路
//  Parameter: [In] is_enable : 打开、关闭dev标识的音频设备
//             [In] dev :音频设备
//             [Out] None
//             [Return] ual_tele_call_result_e :切换音频通路的结果
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_call_result_e ual_tele_call_switch_audio_dev(boolean is_enable, ual_tele_call_audio_dev_e dev);

/*******************************************************************************/
//  Description: 获取当前音频通路
//  Parameter: [In] None
//             [Out] None
//             [Return] ual_tele_call_audio_dev_e :当前音频通路
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_call_audio_dev_e ual_tele_call_get_current_audio_dev(void);

/*******************************************************************************/
//  Description: 设置通话音量
//  Parameter: [In] volume : 通话音量
//             [Out] None
//             [Return] ual_tele_call_result_e :设置通话音量的结果
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_call_result_e ual_tele_call_set_call_volume(uint8 volume);

/*******************************************************************************/
//  Description: 获取通话音量
//  Parameter: [In] None
//             [Out] None
//             [Return] uint8 : 通话音量
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
uint8 ual_tele_call_get_call_volume(void);

/*******************************************************************************/
//  Description: 在拨打电话时，对方已经振铃，但是未收到网络下发的回铃音时，播放本地回铃音
//  Parameter: [In] duration : 本地回铃音的持续时长，播放的时长
//             [In] ring_timer :本地回铃音timer
//             [In] freq1 :混频时的第一个频率
//             [In] freq2 :混频时的第二个频率
//             [In] freq3 :混频时的第三个频率
//             [Out] None
//             [Return] ual_tele_call_result_e :??????????
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_call_result_e ual_tele_call_play_local_netring(uint32 duration, uint32 ring_timer, uint32 freq1, uint32 freq2, uint32 freq3);

/*******************************************************************************/
//  Description: 停止本地回铃音
//  Parameter: [In] None
//             [Out] None
//             [Return] ual_tele_call_result_e :停止播放本地回铃音
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_call_result_e ual_tele_call_stop_local_netring(void);

/*******************************************************************************/
//  Description: 设置上行链路静音
//  Parameter: [In] is_mute : 打开/关闭上行链路静音
//             [Out] None
//             [Return] ual_tele_call_result_e :设置上行链路静音的结果
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
ual_tele_call_result_e ual_tele_call_set_mic_mute(boolean is_mute);

/*******************************************************************************/
//  Description: 获取上行链路静音是否打开
//  Parameter: [In] None
//             [Out] None
//             [Return] boolean : 上行链路静音是否打开的结果
//  Author: miao.liu2
//  Note:
/*******************************************************************************/
boolean ual_tele_call_get_mic_mute(void);

/*****************************************************************************/
//  Description: 当前sim卡volte开关设置
//  Parameter: [In] sim_id //当前sim卡
//             [In] is_on //开关状态, TRUE or FALSE
//             [Out] none
//             [Return] ual_tele_radio_result_e //设置结果
//  Author: yanli.yang
//  Note: 该接口与ual_tele_radio_set_volte功能一致，两个模块均有定义，用户选择
//        使用哪个均可
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_set_volte(ual_tele_sim_id_e sim_id, boolean is_on);

/*****************************************************************************/
//     Description :设置sim卡的VoLTE开关
//     Parameter : [In]  sim_id : sim卡
//                 [Out] none
//                 [Return] 打开(TRUE)、关闭(FALSE)
//     Author: yanli.yang
//     Note: 该接口与ual_tele_radio_get_volte_state功能一致，两个模块均有定义，
//           用户选择使用哪个均可
/*****************************************************************************/
boolean ual_tele_call_get_volte_state(ual_tele_sim_id_e sim_id);

/*****************************************************************************/
// Description: get current call info
// Parameter: [In] none
//            [Out] current call info
//            [Return]result
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_get_current_call_info(ual_tele_call_data_info_t* p_call_info);

/*****************************************************************************/
// Description: get call info
// Parameter: [In] call_id: 协议栈为一路通话分配的id
//            [Out] call info
//            [Return]result
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_get_call_info(uint8 call_id,ual_tele_call_data_info_t* p_call_info);

/*****************************************************************************/
// Description:user to get all calls number
// Parameter: [In] none
//            [Out] none
//            [Return] all calls number
//  Author:mingwei.jia
//  Note:
/*****************************************************************************/
uint8 ual_tele_call_get_call_count(void);

/*******************************************************************************/
//	Description: ual_tele_call_register
//	Parameter: [In] p_callback private call back
//	           [Out] p_handle out of phandle
//	           [Return] ual_tele_call_result_e
//	Author: mingwei.jia
//	Note:
/*******************************************************************************/
ual_tele_call_result_e ual_tele_call_register(_ual_cms_client_register_callback p_callback, uint32 *p_handle);

/*******************************************************************************/
//	Description: ual_tele_call_unregister
//	Parameter: [in] handle
//	           [Out] none
//	           [Return] ual_tele_call_result_e
//	Author: mingwei.jia
//	Note:
/*******************************************************************************/
ual_tele_call_result_e ual_tele_call_unregister(uint32 handle);

/*****************************************************************************/
//  Description: 注入函数
//  Parameter: [In] p_func   //外部模块向ual tele call注入的函数
//             [Out] none
//             [Return] 错误码
//  Author: mingwei.jia
//  Note:外部模块可调用此接口注入函数，p_func在make call接口中会被调用
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_inject_function(ual_tele_call_injection_func p_func);

/*****************************************************************************/
//  Description:取消注入函数
//  Parameter: [In] none
//             [Out] none
//             [Return] 错误码
//  Author: mingwei.jia
//  Note:外部模块可调用此接口取消注入函数，调用完此接口后，make call中将不会再调用注入函数
/*****************************************************************************/
ual_tele_call_result_e ual_tele_call_uninject_function(void);

/*******************************************************************************/
//	Description: ual_tele_call_init
//	Parameter: [in] none
//	           [Out] none
//	           [Return] none
//	Author: tingting.zheng
//	Note:
/*******************************************************************************/
void ual_tele_call_init( void );

#endif

