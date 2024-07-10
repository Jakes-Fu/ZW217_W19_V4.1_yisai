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
** File Name:       ual_bt_ble_dev.h                                         *
** Author:          changli.jiang                                            *
** Date:             09/02/2021                                              *
** Description:    This file is used to define ual_bt_ble api                *
** this api will not change even if ble ps change,we can support goodix and  *
** unisoc ps and other third party.now we only support goodix,unisoc will be *
** added later
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 09/02/2021           changli.jiang           Create                       *
******************************************************************************/
#ifndef _UAL_BT_BLE_DEV_H_
#define _UAL_BT_BLE_DEV_H_
#include "ual_cms_export.h"

#define UAL_BT_BLE_VENDOR_UNISOC 1 //unisoc
#define UAL_BT_BLE_VENDOR_OTHER  0 //other

#define MAX_ADV_CONTENT_LEN  18 //标准数值，协议规定长度26去除了8byte
#define MAX_BLE_ADDRESS_LEN  6//
#define MAX_DEVICE_NAME_LEN 40
#define MAX_RANDOM_VALUE_FOR_RSSI_LEN 6//random value len

typedef enum
{
    UAL_BT_BLE_RESULT_START = 0,
    UAL_BT_BLE_RESULT_SUCESS,//调用接口成功
    UAL_BT_BLE_RESULT_STATE_ERROR,//状态错误，当前ble状态下不能调用此接口
    UAL_BT_BLE_RESULT_NOT_INIT_ERROR,//ble 模块初始化错误
    UAL_BT_BLE_RESULT_PARAM_ERROR,//参数错误    
    UAL_BT_BLE_RESULT_BUSY,//当前ble正在忙
    UAL_BT_BLE_RESULT_OTHER_ERROR,//其他错误    
    UAL_BT_BLE_RESULT_MAX,//
}ual_bt_ble_result_e;
typedef enum
{
    UAL_BT_BLE_ADDRESS_PUBLIC,
    UAL_BT_BLE_ADDRESS_RANDOM,
}ual_bt_ble_address_type_e;

typedef struct
{
    uint8 address[MAX_BLE_ADDRESS_LEN];
}ual_bt_ble_address_t;

typedef struct
{
    // TODO:直接define成协议规定的值
    uint8 content_type;//数据类型 ref SIG_CSS_V9 
    uint8 content_len;//真实数据长度
    uint8 content[MAX_ADV_CONTENT_LEN];//数据
}ual_bt_ble_set_adv_content_t;//整个结构最大28byte
typedef enum
{
    UAL_BT_BLE_UNDIRECT_SCAN_CONNET_MODE,//不定向可扫描可连接模式
    UAL_BT_BLE_NON_CONNECT_AND_SCAN_MODE,//不能连接不能扫描模式
    UAL_BT_BLE_UNDIRECT_SCAN_MODE,//可扫描不定向
    UAL_BT_BLE_DIRECT_HIGH_DUTY_CYCLE_MODE,//快速定向
    UAL_BT_BLE_DIRECT_LOW_DUTY_CYCLE_MODE,//慢速定向
}ual_bt_ble_adv_mode_e;
typedef enum
{
    UAL_BT_BLE_NON_DISCOVERABLE_MODE,//不可发现
    UAL_BT_BLE_NORMAL_DISCOVERABLE_MODE,//普通发现模式
    UAL_BT_BLE_LIMITED_DISCOVERABLE_MODE,//有限的时间内，可以被发现
    UAL_BT_BLE_BROADCASTER_MODE,//广播出去，但不能被扫描及连接
}ual_bt_ble_discoverable_mode_e;
typedef enum
{
    UAL_BT_BLE_ALLOW_SCAN_CONNECT_ANY = 0,/**< Allow both scan and connection requests from anyone. */
    UAL_BT_BLE_ALLOW_SCAN_WHITE_CONNECT_ANY, /**< Allow scan req from white-list devices only and connection req from anyone. */
    UAL_BT_BLE_ALLOW_SCAN_ANY_CONNECT_WHITE,/**< Allow scan req from anyone and connection req from white-list devices only. */
    UAL_BT_BLE_ALLOW_SCAN_WHITE_CONNECT_WHITE,/**< Allow scan and connection requests from white-list devices only. */      
}ual_bt_ble_filter_policy_e;

typedef enum
{
    UAL_BT_BLE_OWN_ADDR_PUBLIC,/*public device address*/
    UAL_BT_BLE_OWN_ADDR_RANDOM_STATIC,/*random static address*/
    UAL_BT_BLE_OWN_ADDR_RANDOM_PRIVATE_RESOLVABLE,/**< Generated resolvable private random address. */
    UAL_BT_BLE_OWN_ADDR_RANDOM_PRIVATE_NON_RESOLVABLE, /**< Generated non-resolvable private random address. */
}ual_bt_ble_own_address_type_e;

typedef struct
{
    ual_bt_ble_address_type_e remote_address_type;
    ual_bt_ble_address_t      remote_address;
}ual_bt_ble_remote_address_t;

typedef enum
{
    UAL_BT_BLE_CHANNEL_MAP_37_38_39,
    UAL_BT_BLE_CHANNEL_MAP_37,
    UAL_BT_BLE_CHANNEL_MAP_38,
    UAL_BT_BLE_CHANNEL_MAP_39,
}ual_bt_ble_channel_map_e;

typedef struct
{
    uint16                          adv_interval_min;/**< Minimum advertising interval (in unit of 625 us). Must be greater than 20 ms. */
    uint16                          adv_interval_max; /**< Maximum advertising interval (in unit of 625 us). Must be greater than 20 ms. */
    ual_bt_ble_adv_mode_e           adv_mode;/**< Advertising mode*/
    ual_bt_ble_discoverable_mode_e  adv_display_mode;/**< Discovery mode*/
    ual_bt_ble_filter_policy_e      adv_adv_filter_policy;/**< Advertising filtering policy*/    
    ual_bt_ble_own_address_type_e        own_addr_type;   /**< Own BD address source of the local device. */
    ual_bt_ble_remote_address_t     remote_addr;       /**< remote address configuration (only used in case of directed advertising, or used to locate the IRK list). */
    ual_bt_ble_channel_map_e        channel_map;        /**< Advertising channel map. See @ref BLE_GAP_ADV_CHANNEL. */
}ual_bt_ble_adv_param_t;

typedef enum
{
    UAL_BT_BLE_IDLE,//空闲中
    UAL_BT_BLE_ADERVISING,//广播中
    UAL_BT_BLE_PARIED,//已配对
    UAL_BT_BLE_CONNECTED,//连接中
    UAL_BT_BLE_BUSY,//忙
    UAL_BT_BLE_MAX
}ual_bt_ble_state_e;

typedef enum
{
    UAL_BT_BLE_DEV_OPERATE_SUCESS = 0,//operate sucess
    UAL_BT_BLE_DEV_OPERATE_FAIL,
}ual_bt_ble_dev_operate_state_e;

typedef struct
{
    uint16 connect_interval_min;/**< Minimum value for the connection interval. This shall be less than or equal to Conn_Interval_Max.
                                        Range: 0x0006 to 0x0C80, unit: 1.25 ms, time range: 7.5 ms to 4 s*/
    uint16 connect_interval_max;/**< Maximum value for the connection interval. This shall be greater than or equal to Conn_Interval_Min.
                                      Range: 0x0006 to 0x0C80, unit: 1.25 ms, time range: 7.5 ms to 4 s.*/
    uint16 slave_latency;       /**< Slave latency for the connection in number of connection events. Range: 0x0000 to 0x01F3. */
    //监控超时，两次成功连接事件间的最大间隔
    uint16 supervision_timeout; /**< Supervision timeout for the LE link. range: 0x000A to 0x0C80, unit: 10 ms, Time range: 100 ms to 32 s. */
}ual_bt_ble_dev_connect_param_t;

typedef enum
{
    UAL_BT_BLE_INIT_TYPE_DIRECT_CONNECT_EST,/**< Direct connection establishment: establish a connection with an indicated device. */
    UAL_BT_BLE_INIT_TYPE_AUTO_CONNECT_EST, /**< Automatic connection establishment: establish a connection with all devices whose address is present in the white list. */
    UAL_BT_BLE_NAME_DISCOVERY, /**< Name discovery: establish a connection with an indicated device in order to read content of its device name characteristic. Connection is closed once this operation is stopped. */
}ual_bt_ble_connect_init_type;

typedef struct
{
    ual_bt_ble_connect_init_type  type;/**< Initiating type (see enum @ref ual_bt_ble_connect_init_type). */
    ual_bt_ble_remote_address_t   remote_addr;/**< Peer device address. */
    ual_bt_ble_dev_connect_param_t connect_param;/*connect param */
}ual_bt_ble_connect_init_param_t;

typedef enum
{
    UAL_BT_BLE_ACTIVE_SCAN_TYPE = 0, /**< Active scan type. */
    UAL_BT_BLE_PASSIVE_SCAN_TYPE,    /**< Passive scan type. */
}ual_bt_ble_scan_type_t;

typedef enum
{
    UAL_BT_BLE_GENERAL_DISC_SCAN_MODE = 0, /**< General discoverable mode. */
    UAL_BT_BLE_LIMITED_DISC_SCAN_MODE,     /**< Limited discoverable mode. */
    UAL_BT_BLE_OBSERVER_SCAN_MODE,         /**< Observer mode. */
}ual_bt_ble_scan_mode_t;

typedef enum
{
    UAL_BT_BLE_SCAN_DISABLE_FILT_DUPLICATE  = 0, /**< Disable filtering of duplicate packets. */
    UAL_BT_BLE_SCAN_ENABLE_FILT_DUPLICATE,       /**< Enable filtering of duplicate packets. */
}ual_bt_ble_scan_dup_filt_policy_t;

typedef struct
{
    ual_bt_ble_scan_type_t                 scan_type; /**< Active scanning or passive scanning. */
    ual_bt_ble_scan_mode_t                 scan_mode; /**< Scan mode. */
    ual_bt_ble_scan_dup_filt_policy_t      scan_dup_filt;/**< Duplicate filter policy. */
    uint8                                  use_whitelist;/**< Filter policy. */
    uint16                                 scan_interval;/**< Scan interval between 0x0004 and 0x4000 in 0.625 ms (range: 2.5 ms to 10.24s). */
    uint16                                 scan_window;/**< Scan window between 0x0004 and 0x4000 in 0.625 ms (range: 2.5 ms to 10.24s). */
    uint16                                 timeout;/**< Scan timeout should be a value between 0x0001 and 0xFFFF(unit: 10 ms). 0x0000 indicates that the timeout has no effect. */
}ual_bt_ble_scan_param_t;

// this is corresponding to ual_bt_ble_dev_msg_e,msg 
typedef struct
{
    ual_bt_ble_dev_operate_state_e dev_operate_state;
}ual_bt_ble_config_adv_param_cnf_t,
 ual_bt_ble_set_adv_content_cnf_t,
 ual_bt_ble_start_adv_cnf_t,
 ual_bt_ble_stop_adv_cnf_t,
 ual_bt_ble_active_disconnect_cnf_t,
 ual_bt_ble_passive_disconnect_cnf_t,
 ual_bt_ble_set_addr_cnf_t,
 ual_bt_ble_set_adv_policy_cnf_t,
 ual_bt_ble_report_connect_status_cnf_t,
 ual_bt_ble_report_pair_status_cnf_t,
 ual_bt_ble_clear_bound_info_cnf_t,
 ual_bt_ble_passive_pair_cnf_t,
 ual_bt_ble_active_connect_cnf_t,
 ual_bt_ble_active_cancel_connect_cnf_t,
 ual_bt_ble_scan_param_set_cnf_t,
 ual_bt_ble_scan_start_cnf_t,
 ual_bt_ble_scan_stop_cnf_t;

typedef struct
{
    ual_bt_ble_address_type_e remote_address_type;
    ual_bt_ble_address_t      remote_address;
}ual_bt_ble_passive_connect_cnf_t;

typedef struct
{
    uint16 remote_device_name_len;
    uint8 remote_device_name[MAX_DEVICE_NAME_LEN];
}ual_bt_ble_remote_device_name_cnf_t;

typedef struct
{
    ual_bt_ble_dev_operate_state_e dev_operate_state;//connect update rsult if sucess read follow parameter
    uint16    interval;           /**< Connection interval. Range: 0x0006 to 0x0C80. Unit: 1.25 ms. Time range: 7.5 ms to 4 s. */
    uint16    latency;            /**< Latency for the connection in number of connection events. Range: 0x0000 to 0x01F3. */
    uint16    sup_timeout;        /**< Supervision timeout for the LE link. Range: 0x000A to 0x0C80, unit: 10 ms, time range: 100 ms to 32 s. */
}ual_bt_ble_connect_param_update_cnf_t;

typedef struct
{
    ual_bt_ble_address_type_e   adv_address_type;
    ual_bt_ble_address_t        adv_address;
    uint8 content_type;//数据类型 ref SIG_CSS_V9
    uint8 content_len;//真实数据长度
    uint8 content[MAX_ADV_CONTENT_LEN];//数据
}ual_bt_ble_report_adv_info_cnf_t;

typedef struct
{
    uint8 random_value[MAX_RANDOM_VALUE_FOR_RSSI_LEN];
}ual_bt_ble_rssi_random_value_t;

typedef struct
{
    boolean rssi_is_ok;
}ual_bt_ble_rssi_status_cnf_t;

typedef enum
{
    UAL_BT_BLE_DEV_MSG_CONFIG_ADV_PARAM_CNF,
    UAL_BT_BLE_DEV_MSG_SET_ADV_CONTENT_CNF,
    UAL_BT_BLE_DEV_MSG_START_ADV_CNF,
    UAL_BT_BLE_DEV_MSG_START_ADV_TIMEOUT_CNF,
    UAL_BT_BLE_DEV_MSG_STOP_ADV_CNF,
    UAL_BT_BLE_DEV_MSG_ACTIVE_DISCONNECT_CNF,
    UAL_BT_BLE_DEV_MSG_PASSIVE_DISCONNECT_CNF,
    UAL_BT_BLE_DEV_MSG_SET_ADDR_CNF,
    UAL_BT_BLE_DEV_MSG_SET_ADV_POLICY_CNF,
    UAL_BT_BLE_DEV_MSG_REPORT_CONNECTION_STATUS_CNF,
    UAL_BT_BLE_DEV_MSG_REPORT_PAIR_STATUS_CNF,
    UAL_BT_BLE_DEV_MSG_REPORT_REMOTE_DEVICE_NAME_CNF,
    UAL_BT_BLE_DEV_MSG_CLEAR_BOUND_INFO_CNF,
    UAL_BT_BLE_DEV_MSG_PASSIVE_PAIR_CNF,
    UAL_BT_BLE_DEV_MSG_PASSIVE_CONNECT_CNF,
    UAL_BT_BLE_DEV_MSG_SCAN_PARAM_SET_CNF,
    UAL_BT_BLE_DEV_MSG_SCAN_START_CNF,
    UAL_BT_BLE_DEV_MSG_SCAN_STOP_CNF,
    UAL_BT_BLE_DEV_MSG_REPORT_ADV_INFO_CNF,
    UAL_BT_BLE_DEV_MSG_ACTIVE_CONNECT_CNF,
    UAL_BT_BLE_DEV_MSG_ACTIVE_CANCEL_CONNECT_CNF,
    UAL_BT_BLE_DEV_MSG_CONNET_PARAM_UPDATE_CNF,
    UAL_BT_BLE_DEV_MSG_CHECK_RSSI_STATUS_CNF,
}ual_bt_ble_dev_msg_e;
/*****************************************************************************/
//  Description : initial ble for registering ble call back for third party
//  Global resource dependence:
//  Param in:none
//  Pram out:none
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
void ual_bt_ble_init(void);
/*****************************************************************************/
//  Description: ual_bt_ble_dev_register
//  Parameter: [In] p_callback private call back
//             [Out] p_handle out of phandle
//             [Return] ual_bt_register_res_e
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_result_e ual_bt_ble_dev_register(_ual_cms_client_register_callback p_callback,uint32 *p_handle);
/*****************************************************************************/
//  Description: ual_bt_ble_dev_unregister
//  Parameter: [in] handle
//             [Out] none
//             [Return] true:success,false:fail
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
boolean ual_bt_ble_dev_unregister(uint32 handle);

/*****************************************************************************/
//  Description : this api used to set adv data,data type ref SIG CSS_V8
//  Global resource dependence:
//  Param in:ual_set_adv_content_t struct of adv data
//  Pram return:ual_ble_service_return_enum this api return value
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_result_e ual_bt_ble_set_adv_content(ual_bt_ble_set_adv_content_t *p_adv_content);
/*****************************************************************************/
//  Description : this api used to set adv param
//  Global resource dependence:
//  Param in:ual_bt_ble_adv_param_t details see struct definition
//  Pram return:ual_ble_service_return_enum this api return value
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_result_e ual_bt_ble_config_adv_param(ual_bt_ble_adv_param_t *p_adv_param);
/*****************************************************************************/
//  Description : this api used to start adv
//  Global resource dependence:
//  Param in:duration_time of adv:0 continuous broadcast,
//                                other value:timeout of adv
//  Pram return:ual_ble_service_return_enum this api return value
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_result_e ual_bt_ble_start_adv(uint16 duration_time);
/*****************************************************************************/
//  Description : this api used to stop adv
//  Global resource dependence:
//  Param in:none
//  Pram return:ual_ble_service_return_enum this api return value
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_result_e ual_bt_ble_stop_adv(void);
/*****************************************************************************/
//  Description : this api used to set ble address of this phone
//  Global resource dependence:
//  Param in:ual_bt_ble_address_t see sturct def
//  Pram return:ual_ble_service_return_enum this api return value
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_result_e ual_bt_ble_set_ble_address(ual_bt_ble_address_t* p_ble_addr);
/*****************************************************************************/
//  Description : this api used to get ble address of this phone
//  Global resource dependence:
//  Param out:ual_bt_ble_address_t see sturct def
//  Pram return:ual_ble_service_return_enum this api return value
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_result_e ual_bt_ble_get_ble_address(ual_bt_ble_address_t* p_ble_addr);
/*****************************************************************************/
//  Description : this api used to set ble address of this phone
//  Global resource dependence:
//  Param in:ual_bt_ble_address_t see sturct def
//  Pram return:ual_ble_service_return_enum this api return value
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_result_e ual_bt_ble_set_ble_random_address(ual_bt_ble_address_t* p_ble_addr);
/*****************************************************************************/
//  Description : this api used to get ble address of this phone
//  Global resource dependence:
//  Param out:ual_bt_ble_address_t see sturct def
//  Pram return:ual_ble_service_return_enum this api return value
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_result_e ual_bt_ble_get_ble_random_address(ual_bt_ble_address_t* p_ble_addr);
/*****************************************************************************/
//  Description : this api used to disconnect ble
//  Global resource dependence:
//  Param in:none
//  Pram return:ual_ble_service_return_enum this api return value
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_result_e ual_bt_ble_disconnect_ble(void);
/*****************************************************************************/
//  Description : this api used to set if ble disconnect,it is go to adervising or idle
//  Global resource dependence:
//  Param in:boolean is_need_adv: 1 need to adervising,0 not need to adervising
//  Pram return:ual_ble_service_return_enum this api return value
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_result_e ual_bt_ble_set_auto_adv(boolean is_need_adv);
/*****************************************************************************/
//  Description : this api used to clear bound info
//  Global resource dependence:
//  Param in:none
//  Pram return:ual_ble_service_return_enum this api return value
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_result_e ual_bt_ble_clear_bound_info(void);
/*****************************************************************************/
//  Description :update connect param when ble is connect
//  Global resource dependence:
//  Param in:none
//  Pram return:ual_bt_ble_state_e this api return value
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_result_e ual_bt_ble_update_connect_param(ual_bt_ble_dev_connect_param_t* p_connect_param);
/*****************************************************************************/
//  Description : this api is used to active connect remote phone,our device 
//                will be master
//  Global resource dependence:
//  Param in:none
//  Pram return:ual_bt_ble_state_e this api return value
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_result_e ual_bt_ble_active_connect(ual_bt_ble_own_address_type_e own_addr_type,ual_bt_ble_connect_init_param_t* p_init_param);
/*****************************************************************************/
//  Description : this api is used to cancel active connect,our device
//                will be master
//  Global resource dependence:
//  Param in:none
//  Pram return:ual_bt_ble_state_e this api return value
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_result_e ual_bt_ble_cancel_active_connect(void);
/*****************************************************************************/
//  Description : this api is used to set param before scan,our device
//                will be master
//  Global resource dependence:
//  Param in:none
//  Pram return:ual_bt_ble_state_e this api return value
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_result_e ual_bt_ble_set_scan_param(ual_bt_ble_own_address_type_e own_addr_type,ual_bt_ble_scan_param_t*p_scan_param);
/*****************************************************************************/
//  Description : this api is used to scan around device,our device
//                will be master
//  Global resource dependence:
//  Param in:none
//  Pram return:ual_bt_ble_state_e this api return value
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_result_e ual_bt_ble_scan(void);
/*****************************************************************************/
//  Description : this api is used to stop scan around device,our device
//                will be master
//  Global resource dependence:
//  Param in:none
//  Pram return:ual_bt_ble_state_e this api return value
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_result_e ual_bt_ble_stop_scan(void);
/*****************************************************************************/
//  Description: ual_bt_ble_dev_set_vendor
//  Parameter: [in] vendor id
//             [Out] none
//             [Return] 
//  Author: chunjuan.liang
//  Note:1:unisoc,0:other
/*****************************************************************************/
void ual_bt_ble_dev_set_vendor(uint8 vendor_id);

/*****************************************************************************/
//  Description : this api can abtain ble status
//  Global resource dependence:
//  Param in:none
//  Pram return:ual_bt_ble_state_e this api return value
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_state_e ual_bt_ble_get_current_ble_status(void);
/*****************************************************************************/
//  Description : this api is used to test ble rssi check rssi is ok or not
//  Global resource dependence:none
//  Param in:none
//  Pram return:ual_bt_ble_result_e enum return value
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_result_e ual_bt_ble_start_check_rssi(void);

/*****************************************************************************/
//  Description : this api is to set random vale which is important for rssi
//  Global resource dependence:
//  Param in:p_random_value:six byte for random value
//  Pram return:ual_bt_ble_state_e enum this api return value
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_result_e ual_bt_ble_set_random_value_for_rssi(ual_bt_ble_rssi_random_value_t *p_random_value);
#endif
