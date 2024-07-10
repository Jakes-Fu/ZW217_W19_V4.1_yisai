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
** File Name:       ual_tele_radio.h                                           *
** Author:            siyuan.fei                                               *
** Date:            2022/08/11                                                 *
** Description:This file is used to define ual tele radio it cases export      *
********************************************************************************
**                         Important Edit History                              *
** ----------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                      *
** 2022/08/11          siyuan.fei                  Create                      *
********************************************************************************/
#ifndef _UAL_TELE_RADIO_H_
#define _UAL_TELE_RADIO_H_

/*---------------------------------------------------------------------------*/
/*                          Include Files                                    */
/*---------------------------------------------------------------------------*/
#include "ual_tele_data.h"
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define UAL_TELE_RADIO_IMEI_MAX_LEN                (15) //imei str max len
#define UAL_TELE_RADIO_MEID_MAX_LEN                (15) //meid str max len
#define UAL_TELE_RADIO_ITEM_ARR_MAX_SIZE           (50) //item arr max count
#define UAL_TELE_RADIO_GSM_NEIGHBOR_MAX_COUNT      (5)  //gsm neighbor max count
#define UAL_TELE_RADIO_UMTS_NEIGHBOR_MAX_COUNT     (18) //umts neighbor max count
#define UAL_TELE_RADIO_LTE_NEIGHBOR_MAX_COUNT      (8)  //lte neighbor max count
#define UAL_TELE_RADIO_PLMN_MAX_NUM (16) //手动搜网的最大PLMN搜索个数

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
//RADIO模块返回结果
typedef enum
{
    UAL_TELE_RADIO_RES_SUCCESS = UAL_TELE_RES_SUCCESS,
    UAL_TELE_RADIO_RES_PARAM_ERROR = UAL_MODULE_TELE_RADIO << 16,//1179648
    UAL_TELE_RADIO_RES_FAIL,
    UAL_TELE_RADIO_RES_ASYNC = UAL_TELE_RADIO_RES_FAIL + 0xFF,
    UAL_TELE_RADIO_RES_MAX
} ual_tele_radio_result_e;

// RADIO模块返回给client端的消息枚举
typedef enum
{
    MSG_UAL_TELE_RADIO_RSSI_IND = UAL_MODULE_TELE_RADIO << 16,
    MSG_UAL_TELE_RADIO_PLMN_LIST_CNF,
    MSG_UAL_TELE_RADIO_SELECT_PLMN_CNF,
    MSG_UAL_TELE_RADIO_IMS_STATE_IND,
    MSG_UAL_TELE_RADIO_NETWORK_STATUS_IND,
    MSG_UAL_TELE_RADIO_VOLTE_STATE_IND,
    MSG_UAL_TELE_RADIO_LTE_CELL_INFO_CNF,
    MSG_UAL_TELE_RADIO_LTE_NEIGH_CELL_INFO_CNF,
    MSG_UAL_TELE_RADIO_GSM_CELL_INFO_CNF,
    MSG_UAL_TELE_RADIO_GSM_NEIGH_CELL_INFO_CNF,
    MSG_UAL_TELE_RADIO_MAX
}ual_tele_radio_msg_e;

//网络信号等级，目前分为0~5共6级，对应相应的设备信号条显示
typedef enum
{
    UAL_TELE_RADIO_RSSI_LEVEL0,
    UAL_TELE_RADIO_RSSI_LEVEL1,
    UAL_TELE_RADIO_RSSI_LEVEL2,
    UAL_TELE_RADIO_RSSI_LEVEL3,
    UAL_TELE_RADIO_RSSI_LEVEL4,
    UAL_TELE_RADIO_RSSI_LEVEL5,
    UAL_TELE_RADIO_RSSI_MAX
} ual_tele_radio_rssi_level_e;
//随MSG_UAL_TELE_RADIO_RSSI_IND上报的结构体，表示sim卡信号强度
typedef struct
{
    uint32 sim_id; //sim id //ual_tele_sim_id_e
    uint32 rssi_level; //信号强度(0-5) //ual_tele_radio_rssi_level_e
    uint32 cell_id; //小区识别码
    uint32 physical_cell_id; //物理小区识别码
}ual_tele_radio_rssi_ind_t;

// MSG_UAL_TELE_RADIO_VOLTE_STATE_IND消息上报的结构体，返回当前网络是否支持volte
typedef struct
{
    uint32 sim_id; // sim id //ual_tele_sim_id_e
    boolean is_volte_support; // 当前网络是否支持VoLTE
}ual_tele_radio_volte_state_ind_t;

//优先网络类型，分为6种
typedef enum
{
    UAL_TELE_RADIO_PREFER_NETWORK_TYPE_GSM = 0,  // only 2G
    UAL_TELE_RADIO_PREFER_NETWORK_TYPE_3G, // only 3G
    UAL_TELE_RADIO_PREFER_NETWORK_TYPE_GSM_AND_3G, // 2G & 3G
    UAL_TELE_RADIO_PREFER_NETWORK_TYPE_LTE = 0x10, // only 4G
    UAL_TELE_RADIO_PREFER_NETWORK_TYPE_GSM_LTE, // 2G & 4G
    UAL_TELE_RADIO_PREFER_NETWORK_TYPE_3G_LTE, // 3G & 4G
    UAL_TELE_RADIO_PREFER_NETWORK_TYPE_GSM_3G_LTE, // 2G & 3G & 4G
    UAL_TELE_RADIO_PREFER_NETWORK_TYPE_MAX
} ual_tele_radio_prefer_network_type_e;
//当前网络类型
typedef enum
{
    UAL_TELE_RADIO_CURRENT_NETWORK_TYPE_2G = 0, // 2G
    UAL_TELE_RADIO_CURRENT_NETWORK_TYPE_3G, // 3G
    UAL_TELE_RADIO_CURRENT_NETWORK_TYPE_4G, // 4G
    UAL_TELE_RADIO_CURRENT_NETWORK_TYPE_5G, // 5G
    UAL_TELE_RADIO_CURRENT_NETWORK_TYPE_UNKNOWN, // 未 知
    UAL_TELE_RADIO_CURRENT_NETWORK_TYPE_MAX
} ual_tele_radio_current_network_type_e;

//手动搜网网络类型
typedef enum
{
    UAL_TELE_RADIO_MANUAL_NETWORK_TYPE_GSM = 0,  // only 2G
    UAL_TELE_RADIO_MANUAL_NETWORK_TYPE_3G, // only 3G
    UAL_TELE_RADIO_MANUAL_NETWORK_TYPE_GSM_AND_3G, // 2G & 3G
    UAL_TELE_RADIO_MANUAL_NETWORK_TYPE_LTE = 0x10, // only 4G
    UAL_TELE_RADIO_MANUAL_NETWORK_TYPE_GSM_LTE, // 2G & 4G
    UAL_TELE_RADIO_MANUAL_NETWORK_TYPE_3G_LTE, // 3G & 4G
    UAL_TELE_RADIO_MANUAL_NETWORK_TYPE_GSM_3G_LTE, // 2G & 3G & 4G
    UAL_TELE_RADIO_MANUAL_NETWORK_TYPE_MAX
} ual_tele_radio_manual_network_type_e;
//plmn状态枚举,4种
typedef enum
{
    UAL_TELE_RADIO_PLMN_STATE_NO_SERVICE, // 无服务
    UAL_TELE_RADIO_PLMN_STATE_EMERGENCY, // 紧急状态
    UAL_TELE_RADIO_PLMN_STATE_REGISTERING, // 注册中
    UAL_TELE_RADIO_PLMN_STATE_NORMAL, // 正常状态
    UAL_TELE_RADIO_PLMN_STATE_MAX
} ual_tele_radio_plmn_state_e;
//APN类型,6种
typedef enum
{
    UAL_TELE_RADIO_APN_TYPE_INTERNET,
    UAL_TELE_RADIO_APN_TYPE_MMS,
    UAL_TELE_RADIO_APN_TYPE_IMS,
    UAL_TELE_RADIO_APN_TYPE_IA,
    UAL_TELE_RADIO_APN_TYPE_XCAP,
    UAL_TELE_RADIO_APN_TYPE_SOS,
    UAL_TELE_RADIO_APN_TYPE_MAX
} ual_tele_radio_apn_type_e;
//APN信息
typedef struct
{
    ual_tele_data_apn_info_t ia_apn_info[SIM_ID_MAX];
    ual_tele_data_apn_info_t internet_apn_info[SIM_ID_MAX];
    ual_tele_data_apn_info_t xcap_apn_info[SIM_ID_MAX];
    ual_tele_data_apn_info_t ims_apn_info[SIM_ID_MAX];
    ual_tele_data_apn_info_t sos_apn_info[SIM_ID_MAX];
    ual_tele_data_apn_info_t mms_apn_info[SIM_ID_MAX];
} ual_tele_radio_apn_info_t;

//IMS状态枚举,共8种
typedef enum
{
    UAL_TELE_RADIO_IMS_STATE_INACTIVE, // 未激活
    UAL_TELE_RADIO_IMS_STATE_REGISTERED, // 已注册
    UAL_TELE_RADIO_IMS_STATE_REGISTERING, // 注册中
    UAL_TELE_RADIO_IMS_STATE_FAILED, // 失败
    UAL_TELE_RADIO_IMS_STATE_UNKNOWN, // 未知
    UAL_TELE_RADIO_IMS_STATE_ROAMING, // 漫游
    UAL_TELE_RADIO_IMS_STATE_DEREGISTERING, // 注销中
    UAL_TELE_RADIO_IMS_STATE_INVALID, // 不合法
    UAL_TELE_RADIO_IMS_STATE_MAX
} ual_tele_radio_ims_state_e;
//随MSG_UAL_TELE_RADIO_IMS_STATE_IND消息上报的结构体，返回当前网络ims状态
typedef struct
{
    uint32 sim_id; // sim id //ual_tele_sim_id_e
    uint32 ims_state; // ims状态 //ual_tele_radio_ims_state_e
} ual_tele_radio_ims_state_ind_t;
//小区信息
typedef struct
{
    uint32 cell_id;
    uint16 mcc;
    uint16 mnc;
} ual_tele_radio_cell_info_t;
//gsm小区信息
typedef struct
{
    uint16 arfcn;      // 绝对无线频率编号 Absolute Radio Frequency Channel Number
    uint8 bsic;       // 基站识别码 Base-Station Identification Code
    uint16 cell_id;    // 小区识别码
    uint8 rxqual;     // 信号接受质量  Received Signal Quality
    uint8 rxlev;      // 平均接收电平 Received Signal Level
    uint16 scell_band; // 辅小区频道
    uint8 txpwr;      // 发射功率
    uint8 ta_value;   // 时间提前量 Timing advance
    uint16 lac;       // 位置区域码 Location Area Code
    uint8 mcc;        // 移动国家号 Mobile Country Code
    uint8 mnc;        // 移动网号 Mobile Country Code
} ual_tele_radio_gsm_cell_info_t;
//umts小区信息
typedef struct
{
    uint16 lac;   // 位置区域码 Location Area Code
    uint16 mcc;   // 移动国家号 Mobile Country Code
    uint16 mnc;   // 移动网号 Mobile Country Code
    int16 psc;    // 主同步码 Primary Synchronization Code
    int16 uarfcn; // 超绝对射频信道号 UTRA Absolute Radio Frequency Channel Number
    uint32 ucid;  // 通用标识符 Universal Cell Identifier
    int16 rscp;   // 导频信道信号强度 receive sigal code power,range -120dbm tp -20 dbm,-1 if unknown
    int32 rncid;  // 网络控制区标识 Radio Network Controller Identifier
    int32 ecno;   // Energy per chip over noise destiny
} ual_tele_radio_umts_cell_info_t;
//radio item
typedef struct
{
    uint8 count;
    uint32 values[UAL_TELE_RADIO_ITEM_ARR_MAX_SIZE];
} ual_tele_radio_item_t;
//lte小区信息
typedef struct
{
    ual_tele_radio_item_t band;   // 频道
    ual_tele_radio_item_t earfcn; // LTE绝对频点号 E-UTRA Absolute Radio Frequency Channel Number
    ual_tele_radio_item_t pci;    // 物理小区标识 Physical Cell Identifier
    ual_tele_radio_item_t rsrp;   // 参考信号接收功率 Reference Signal Received Power
    ual_tele_radio_item_t rsrq;   // 参考信号接收质量 Reference Signal Received Quality
    ual_tele_radio_item_t bw;     // 带宽 Bandwidth
    ual_tele_radio_item_t ta;     // 跟踪区 Tracking Area
    ual_tele_radio_item_t tac;    // 跟踪区编码 Tracking Area Code
    ual_tele_radio_item_t mcc;
    ual_tele_radio_item_t mnc;
    ual_tele_radio_item_t cell_id;// 小区识别码
    ual_tele_radio_item_t srxlev; // 接收信号等级/电平 Received Signal Level
} ual_tele_radio_lte_cell_info_t;
//gsm邻小区信息
typedef struct
{
    int16 arfcn;   // 绝对无线频率编号 Absolute Radio Frequency Channel Number
    int16 bsic;    // 基站识别码 Base-Station Identification Code
    int16 cell_id; // 小区识别码
    int16 rxqual;  // 信号接受质量  Received Signal Quality
    int16 rxlev;   // 接收信号等级/电平 Received Signal Level
    uint16 lac;    // 位置区域码 Location Area Code
    uint8 mcc;
    uint8 mnc;
}ual_tele_radio_gsm_neighbor_cell_info_t;
//随MSG_UAL_TELE_RADIO_GSM_NEIGH_CELL_INFO_CNF上报
typedef struct
{
    uint8 count;
    ual_tele_radio_gsm_neighbor_cell_info_t gsm_neighbor_info[UAL_TELE_RADIO_GSM_NEIGHBOR_MAX_COUNT];
} ual_tele_radio_gsm_ncell_cnf_t;
//umts邻小区信息
typedef struct
{
    int16 psc;    // 主同步码 Primary Synchronization Code
    int16 uarfcn; // 超绝对射频信道号 UTRA Absolute Radio Frequency Channel Number
    int16 rscp;   // 导频信道信号强度 Receive Signal Channel Power
} ual_tele_radio_umts_neighbor_cell_info_t;
//lte邻小区信息
typedef struct
{
    int16 pci;  // 物理小区标识 Physical Cell Identifier
    int16 rsrp; // 参考信号接收功率 Reference Signal Received Power
    int16 rsrq; // 参考信号接收质量 Reference Signal Received Quality
    int16 rat;  // 无线接入技术 Radio Access Technology
    int32 earfcn; // LTE绝对频点号 E-UTRA Absolute Radio Frequency Channel Number
    uint8 mcc;
    uint8 mnc;
    uint16 tac; // 跟踪区域代码
    uint32 cell_id; // 小区识别码
    int16 srxlev; // 接收信号等级/电平 Received Signal Level
    uint32 frequency;
} ual_tele_radio_lte_neighbor_cell_info_t;

//随MSG_UAL_TELE_RADIO_LTE_NEIGH_CELL_INFO_CNF上报
typedef struct
{
    uint8 count;
    ual_tele_radio_lte_neighbor_cell_info_t lte_neighbor_info[UAL_TELE_RADIO_LTE_NEIGHBOR_MAX_COUNT];
} ual_tele_radio_lte_ncell_cnf_t;

//信噪比 Signal to InterferenceNoiseRatio
typedef ual_tele_radio_item_t ual_tele_radio_snr_t;
//漫游状态
typedef enum
{
    UAL_TELE_RADIO_ROAMING_DISABLE, //关闭
    UAL_TELE_RADIO_ROAMING_ENABLE, //开启
    UAL_TELE_RADIO_ROAMING_MAX
} ual_tele_radio_roaming_mode_e;
//plmn搜索模式，手动或自动
typedef enum
{
    UAL_TELE_RADIO_PLMN_SEARCH_MODE_MANUAL, //手动
    UAL_TELE_RADIO_PLMN_SEARCH_MODE_AUTO,  //自动
    UAL_TELE_RADIO_PLMN_SEARCH_MODE_MAX
} ual_tele_radio_plmn_search_mode_e;
//随MSG_UAL_TELE_RADIO_NETWORK_STATUS_IND上报的结构体，上报网络状态
typedef struct
{
    uint32 sim_id; // sim id //ual_tele_sim_id_e
    ual_tele_plmn_info_t network_plmn_info; // 网络的plmn信息
    uint16 lac; // 区域码
    uint16 tac; // 跟踪区域代码
    boolean is_roaming; // 是否漫游中
    uint32 network_type; // 当前网络类型 //ual_tele_radio_current_network_type_e
    uint32 plmn_state; // plmn状态 //ual_tele_radio_plmn_state_e
    boolean is_gprs_available; // gprs是否可用
} ual_tele_radio_network_status_ind_t;
//单条plmn的详细信息
typedef struct
{
    ual_tele_plmn_info_t plmn_info; // sim卡的plmn信息
    uint32 reg_rat; // 网络类型 //ual_tele_radio_prefer_network_type_e
    uint16 lac; // 区域码
    boolean plmn_is_forbidden; // 是否禁止
}ual_tele_radio_plmn_single_t;
//随MSG_UAL_TELE_RADIO_PLMN_LIST_CNF上报的结构体，上报plmn列表
typedef struct
{
    uint32 sim_id; //ual_tele_sim_id_e
    uint32 result; // 搜索结果 //ual_tele_radio_result_e
    uint8 plmn_number; // 搜索到的plmn数量
    ual_tele_radio_plmn_single_t plmn_single[UAL_TELE_RADIO_PLMN_MAX_NUM]; // 单条plmn的详细信息
} ual_tele_radio_plmn_list_cnf_t;
//随MSG_UAL_TELE_RADIO_SELECT_PLMN_CNF上报的结构体，上报plmn选取的信息
typedef struct
{
    uint32 sim_id; // sim id //ual_tele_sim_id_e
    boolean plmn_is_selected; // plmn是否被选择
    uint32 cell_id; // 小区id
    ual_tele_plmn_info_t plmn_info; // sim卡plmn信息
} ual_tele_radio_select_plmn_cnf_t;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DECLARE                                 *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: 注册tele radio模块接口
//  Parameter: [In] p_callback //注册的回调函数
//             [Out] p_handle //创建的tele radio的handle
//             [Return] ual_tele_radio_result_e //注册结果
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_register(_ual_cms_client_register_callback p_callback, uint32 *p_handle);
/*****************************************************************************/
//  Description: 注销tele radio模块接口
//  Parameter: [In] handle //tele radio的handle
//             [Out] none
//             [Return] ual_tele_radio_result_e //注销结果
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_unregister(uint32 handle);
/*****************************************************************************/
//  Description: 初始化tele radio模块
//  Parameter: [In] none
//             [Out] none
//             [Return] none
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
void ual_tele_radio_init(void);
/*****************************************************************************/
//  Description: 获取当前sim卡信号强度
//  Parameter: [In] sim_id //当前sim卡
//             [Out] none
//             [Return] ual_tele_radio_rssi_level_e //信号强度rssi_level(范围0-5)
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_rssi_level_e ual_tele_radio_get_rssi_strength_level(ual_tele_sim_id_e sim_id);
/*****************************************************************************/
//  Description: 获取当前sim卡网络类型
//  Parameter: [In] sim_id //当前sim卡
//             [Out] none
//             [Return] ual_tele_radio_current_network_type_e //当前网络类型
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_current_network_type_e ual_tele_radio_get_current_network_type(ual_tele_sim_id_e sim_id);
/*****************************************************************************/
//  Description: 获取sim卡优先网络类型
//  Parameter: [In] sim_id //当前sim卡
//             [Out] none
//             [Return] ual_tele_radio_prefer_network_type_e //优先网络类型
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_prefer_network_type_e ual_tele_radio_get_prefer_network_type(ual_tele_sim_id_e sim_id);
/*****************************************************************************/
//  Description: set prefer network type
//  Parameter: [In] sim_id //当前sim卡
//             [In] prefer_type //优先网络类型
//             [Out] none
//             [Return] ual_tele_radio_result_e //设置结果
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_set_prefer_network_type(ual_tele_sim_id_e sim_id, ual_tele_radio_prefer_network_type_e prefer_type);
/*****************************************************************************/
//  Description: 设置手动搜网网络类型
//  Parameter: [In] sim_id //当前sim卡
//             [In] plmn_info //plmn信息
//             [In] network_type //设置的网络类型
//             [Out] none
//             [Return] ual_tele_radio_result_e //设置结果
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_set_manual_network_type(ual_tele_sim_id_e sim_id, ual_tele_plmn_info_t plmn_info, ual_tele_radio_manual_network_type_e network_type);
/*****************************************************************************/
//  Description: 获取当前sim卡plmn信息
//  Parameter: [In] sim_id //当前sim卡
//             [Out] p_plmn //plmn信息
//             [Return] ual_tele_radio_result_e //获取结果
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_current_plmn_info(ual_tele_sim_id_e sim_id, ual_tele_plmn_info_t* p_plmn);
/*****************************************************************************/
//  Description: 获取当前sim卡plmn状态
//  Parameter: [In] sim_id //当前sim卡
//             [Out] none
//             [Return] ual_tele_radio_plmn_state_e //plmn状态
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_plmn_state_e ual_tele_radio_get_current_plmn_state(ual_tele_sim_id_e sim_id);
/*****************************************************************************/
//  Description: 设置apn信息，用于解析匹配APN后设置给radio模块
//  Parameter: [In] sim_id: 当前sim卡的index
//             [In] p_apn_info: 当前sim卡匹配后的apn信息
//             [Out] None
//             [Return] 设置apn信息的结果枚举值
//  Author: yanli.yang
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_set_apn_info(ual_tele_sim_id_e sim_id, ual_tele_radio_apn_info_t* p_apn_info);

/*****************************************************************************/
//  Description: 获取当前sim卡apn信息
//  Parameter: [In] sim_id //当前sim卡
//             [Out] p_internet_apn //internet apn信息
//             [Return] ual_tele_radio_result_e //获取结果
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_internet_apn_info(ual_tele_sim_id_e sim_id, ual_tele_data_apn_info_t* p_internet_apn);
/*****************************************************************************/
//  Description: 更新当前sim卡Internet apn信息
//  Parameter: [In] sim_id //当前sim卡
//             [In] p_internet_apn_info //网络apn信息
//             [Out] none
//             [Return] ual_tele_radio_result_e //更新结果
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_update_internet_apn_info(ual_tele_sim_id_e sim_id, ual_tele_data_apn_info_t* p_internet_apn_info);
/*****************************************************************************/
//  Description: 获取IMS状态
//  Parameter: [In] sim_id //当前sim卡
//             [Out] none
//             [Return] ual_tele_radio_ims_state_e //IMS状态
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_ims_state_e ual_tele_radio_get_ims_state(ual_tele_sim_id_e sim_id);
/*****************************************************************************/
//  Description: 获取当前sim卡volte状态
//  Parameter: [In] sim_id //当前sim卡
//             [Out] none
//             [Return] 打开(TRUE)、关闭(FALSE)
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
boolean ual_tele_radio_get_volte_state(ual_tele_sim_id_e sim_id);
/*****************************************************************************/
//  Description: 获取当前sim卡是否支持volte
//  Parameter: [In] sim_id //当前sim卡
//             [Out] none
//             [Return] 支持(TRUE)、不支持(FALSE)
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
boolean ual_tele_radio_get_current_network_volte_state(ual_tele_sim_id_e sim_id);
/*****************************************************************************/
//  Description: 当前sim卡volte开关设置
//  Parameter: [In] sim_id //当前sim卡
//             [In] is_on //开关状态, TRUE or FALSE
//             [Out] none
//             [Return] ual_tele_radio_result_e //设置结果
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_set_volte(ual_tele_sim_id_e sim_id, boolean is_on);
/*****************************************************************************/
//  Description: 获取当前radio模块飞行模式开启状态
//  Parameter: [In] none
//             [Out] none
//             [Return] 开启状态(TRUE)、关闭状态(FALSE)
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
boolean ual_tele_radio_get_fly_mode_state(void);
/*****************************************************************************/
//  Description: 设置radio模块飞行模式
//  Parameter: [In] is_on //开启或关闭飞行模式
//             [Out] none
//             [Return] ual_tele_radio_result_e //设置结果
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_set_fly_mode(boolean is_on);
/*****************************************************************************/
//  Description: 关闭PS,关机时调用
//  Parameter: [In] none
//             [Out] none
//             [Return] none
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
void ual_tele_radio_power_off_ps(void);
/*****************************************************************************/
//  Description: 获取当前小区信息
//  Parameter: [In] sim_id //当前sim卡
//             [Out] p_current_cell_info //当前小区信息
//             [Return] ual_tele_radio_result_e //获取结果
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_current_cellinfo(ual_tele_sim_id_e sim_id, ual_tele_radio_cell_info_t *p_current_cell_info);
/*****************************************************************************/
//  Description: 获取gsm信息
//  Parameter: [In] sim_id //当前sim卡
//             [Out] p_gsm_cell_info //gsm信息
//             [Return] ual_tele_radio_result_e //获取结果
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_gsm_cell_info(ual_tele_sim_id_e sim_id, ual_tele_radio_gsm_cell_info_t *p_gsm_cell_info);
//  Description: 获取3g小区信息
//  Parameter: [In] sim_id //当前sim卡
//             [Out] p_umts_cell_info //3g信息
//             [Return] ual_tele_radio_result_e //获取结果
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_3g_cell_info (ual_tele_sim_id_e sim_id, ual_tele_radio_umts_cell_info_t *p_umts_cell_info);
/*****************************************************************************/
//  Description: 获取LTE小区信息
//  Parameter: [In] sim_id //当前sim卡
//             [Out] p_lte_cell_info //LTE信息
//             [Return] ual_tele_radio_result_e //获取结果
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_lte_cell_info(ual_tele_sim_id_e sim_id, ual_tele_radio_lte_cell_info_t *p_lte_cell_info);
/*****************************************************************************/
//  Description: 获取gsm邻小区信息
//  Parameter: [In] sim_id //当前sim卡
//             [Out] gsm_neighbor_cell_info[UAL_TELE_RADIO_GSM_NEIGHBOR_MAX_COUNT] //邻小区gsm信息
//             [Return] ual_tele_radio_result_e //获取结果
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_gsm_neighbor_cell_info(ual_tele_sim_id_e sim_id, ual_tele_radio_gsm_neighbor_cell_info_t gsm_neighbor_cell_info[UAL_TELE_RADIO_GSM_NEIGHBOR_MAX_COUNT]);
/*****************************************************************************/
//  Description: 获取umts邻小区信息
//  Parameter: [In] sim_id //当前sim卡
//             [Out] umts_neighbor_cell_info[UAL_TELE_RADIO_UMTS_NEIGHBOR_MAX_COUNT] //邻小区umts信息
//             [Return] ual_tele_radio_result_e //获取结果
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_umts_neighbor_cell_info(ual_tele_sim_id_e sim_id, ual_tele_radio_umts_neighbor_cell_info_t umts_neighbor_cell_info[UAL_TELE_RADIO_UMTS_NEIGHBOR_MAX_COUNT]);
/*****************************************************************************/
//  Description: 获取lte邻小区信息
//  Parameter: [In] sim_id //当前sim卡
//             [Out] lte_neighbor_cell_info[UAL_TELE_RADIO_LTE_NEIGHBOR_MAX_COUNT] //邻小区lte信息
//             [Return] ual_tele_radio_result_e //获取结果
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_lte_neighbor_cell_info(ual_tele_sim_id_e sim_id, ual_tele_radio_lte_neighbor_cell_info_t lte_neighbor_cell_info[UAL_TELE_RADIO_LTE_NEIGHBOR_MAX_COUNT]);
/*****************************************************************************/
//  Description: 获取lte制式信噪比，与网速相关
//  Parameter: [In] sim_id //当前sim卡
//             [Out] p_snr //信噪比
//             [Return] ual_tele_radio_result_e //获取结果
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_lte_snr(ual_tele_sim_id_e sim_id, ual_tele_radio_snr_t *p_snr);
/*****************************************************************************/
//  Description: 获取imei号
//  Parameter: [In] sim_id //当前sim卡
//             [Out] imei[UAL_TELE_RADIO_IMEI_MAX_LEN + 1] // char数组
//             [Return] ual_tele_radio_result_e //获取结果
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_imei(ual_tele_sim_id_e sim_id, char imei[UAL_TELE_RADIO_IMEI_MAX_LEN + 1]);
/*****************************************************************************/
//  Description: 设置imei号
//  Parameter: [In] sim_id //当前sim卡
//                  [In] imei[UAL_TELE_RADIO_IMEI_MAX_LEN + 1] // char数组
//             [Out] none
//             [Return] ual_tele_radio_result_e //设置结果
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_set_imei (ual_tele_sim_id_e sim_id, char imei[UAL_TELE_RADIO_IMEI_MAX_LEN + 1]);
/*****************************************************************************/
//  Description: 获取meid号
//  Parameter: [In] sim_id //当前sim卡
//             [Out] meid[UAL_TELE_RADIO_MEID_MAX_LEN + 1] // char数组
//             [Return] ual_tele_radio_result_e //设置结果
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_get_meid (ual_tele_sim_id_e sim_id, char meid[UAL_TELE_RADIO_MEID_MAX_LEN + 1]);
/*****************************************************************************/
//  Description: 获取漫游模式
//  Parameter: [In] sim_id //当前sim卡
//             [Out] none
//             [Return] ual_tele_radio_roaming_mode_e // 漫游模式
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_roaming_mode_e ual_tele_radio_get_roaming_mode(ual_tele_sim_id_e sim_id);
/*****************************************************************************/
//  Description: 获取plmn搜索模式
//  Parameter: [In] sim_id //当前sim卡
//             [Out] none
//             [Return] ual_tele_radio_plmn_search_mode_e // plmn搜索模式，手动或自动
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_plmn_search_mode_e ual_tele_radio_get_plmn_search_mode(ual_tele_sim_id_e sim_id);
/*****************************************************************************/
//  Description: 设置plmn搜索模式
//  Parameter: [In] sim_id //当前sim卡
//                  [In] search_mode // 设置的plmn搜索模式，手动或自动
//             [Out] none
//             [Return] ual_tele_radio_result_e //设置结果
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
ual_tele_radio_result_e ual_tele_radio_set_plmn_search_mode (ual_tele_sim_id_e sim_id, ual_tele_radio_plmn_search_mode_e search_mode);
/*****************************************************************************/
//  Description: 获取当前sim卡gprs状态
//  Parameter: [In] sim_id //当前sim卡
//             [Out] none
//             [Return] boolean //gprs状态
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
boolean ual_tele_radio_get_gprs_state(ual_tele_sim_id_e sim_id);
#endif
