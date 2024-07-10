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
** File Name:       ual_rf_am.h                                              *
** Author:          zhenpeng.yao                                             *
** Date:            09/21/2022                                               *
** Description:    This file is used to define ual rf_am export              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 09/21/2022         zhenpeng.yao              Create                       *
******************************************************************************/
#ifndef _UAL_RF_AM_H
#define _UAL_RF_AM_H
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "ual_type.h"
#include "ual_common.h"

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          ENUM AND STRUCT                                  */
/*---------------------------------------------------------------------------*/
/*cur state*/
typedef enum
{
    RF_AM_STATE_IDLE,              //wifi and gnss and lte not work
    RF_AM_STATE_GNSS_COLD_WORK,    //gnss work
    RF_AM_STATE_GNSS_HOT_WORK,     //gnss wokr
    RF_AM_STATE_WIFISCAN_WORK,     //wifiscan work
    RF_AM_STATE_LTE_WORK,          //lte work
    RF_AM_STATE_MAX
}ual_rf_am_state_e;

//事件定义
typedef enum
{
    MSG_UAL_RF_AM_START_GNSS_REJECT = UAL_MODULE_RF_AM << 16,
    MSG_UAL_RF_AM_NOTIFY_CC_HANGUP,
    MSG_UAL_RF_AM_MAX
} ual_rf_am_msg_e;

/*cur state*/
typedef enum
{
    RF_AM_LTE_STATE_IDLE,
    RF_AM_LTE_STATE_CONNECT,
    RF_AM_LTE_STATE_MAX
}ual_rf_am_lte_state_e;

/*运行模式*/
typedef enum
{
    UAL_RF_AM_BACKGROUND_POSITION_MODE = 1,//后台定位模式
    UAL_RF_AM_SPORT_POSITION_MODE,     //运动模式
    UAL_RF_AM_COMMUNICATION_MODE,      //LTE通信模式
    UAL_RF_AM_MODE_MAX
}ual_rf_am_mode_e;

/*接口调用返回值*/
typedef enum
{
    UAL_RF_AM_RES_SUCCESS,
    UAL_RF_AM_RES_PARAM_ERR = UAL_MODULE_RF_AM<<16,
    UAL_RF_AM_RES_STATE_ERR,
    UAL_RF_AM_RES_OTHER_ERR,
    UAL_RF_AM_RES_PERMIT_RUN,
    UAL_RF_AM_RES_MAX
}ual_rf_am_result_e;

#ifdef  UAL_TELE_SUPPORT
#define CALL_NUM_MAX_LEN            (40) //最大号码长度
typedef struct
{
    char    call_num[CALL_NUM_MAX_LEN + 1];//号码信息
    uint8   call_num_len;                       //号码长度
    boolean  is_ecc_present;
    uint8    emgcy_ctgry_value;                 //紧急号码携带catgry值
    uint32  clir_type;                //是否隐层号码
    uint32  call_type;
} call_make_call_t;
typedef struct
{
    uint32 sim_id;
    call_make_call_t make_call_info;
}ual_rf_am_make_call_info_t;//make call info
#endif

typedef struct
{
    MN_DUAL_SYS_E dual_sys;
    MN_SMS_MO_SMS_T   mo_sms;
    MN_SMS_STORAGE_E storage;
    MN_SMS_PATH_E send_path;
    boolean is_more_msg;
}ual_rf_am_sms_send_info_t;//send sms info

/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description: 设置工作模式
//  Parameter: [In] mode   //要设置的工作模式
//             [Out] none
//             [Return] 错误码
//  Author: zhenpeng.yao
//  Note:
/*****************************************************************************/
ual_rf_am_result_e ual_rf_am_set_mode(ual_rf_am_mode_e mode);

/*****************************************************************************/
//  Description: 获取工作模式
//  Parameter: [In] none
//             [Out] none
//             [Return] 当前工作模式
//  Author: zhenpeng.yao
//  Note:
/*****************************************************************************/
ual_rf_am_mode_e ual_rf_am_get_current_mode(void);

/*****************************************************************************/
//  Description: 开机初始化
//  Parameter: [In] none
//             [Out] none
//             [Return] ual_rf_am_result_e
//  Author: zhenpeng.yao
//  Note:
/*****************************************************************************/
ual_rf_am_result_e ual_rf_am_init(void);
/*****************************************************************************/
//  Description: 接收从sms来的消息
//  Parameter:
//             [In] msg_id
//             [In] param
//             [Out] none
//             [Return] MMI_RESULT_E
//  Author: zhenpeng.yao
//  Note:
/*****************************************************************************/
ual_rf_am_result_e rf_am_handle_sms_msg(void* p_param, uint32 param_size);

/*****************************************************************************/
//  Description: 获取工作状态
//  Parameter: [In] none
//             [Out] none
//             [Return] 当前工作状态
//  Author: zhenpeng.yao
//  Note:
/*****************************************************************************/
ual_rf_am_state_e ual_rf_am_get_current_status(void);

/*****************************************************************************/
//  Description: 获取lte工作状态
//  Parameter: [In] none
//             [Out] none
//             [Return] 当前工作状态
//  Author: zhenpeng.yao
//  Note:
/*****************************************************************************/
ual_rf_am_lte_state_e ual_rf_am_get_current_lte_status(void);

/*****************************************************************************/
//  Description: 强制lte回idel
//  Parameter: [In] none
//             [Out] none
//             [Return] uint32
//  Author: zhenpeng.yao
//  Note:
/*****************************************************************************/
ual_rf_am_result_e ual_rf_am_force_lte_to_idle(void);

/*****************************************************************************/
//  Description: 注册cb
//  Parameter: [In] p_callback   //app向ual rf am注册的callback
//             [Out] p_handle    //注册得到的cms handle，去注册的时候还需要传入此handle
//             [Return] 错误码
//  Author: zhenpeng.yao
//  Note:
/*****************************************************************************/
ual_rf_am_result_e ual_rf_am_register(_ual_cms_client_register_callback p_callback, uint32* p_handle);

/*****************************************************************************/
//  Description: callback去注册
//  Parameter: [In] handle
//             [Return] 错误码
//  Author: zhenpeng.yao
//  Note:
/*****************************************************************************/
ual_rf_am_result_e ual_rf_am_unregister(uint32 handle);

#endif
