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
** File Name:       ual_wifi.h                                           *
** Author:          miao.xi                                               *
** Date:            09/22/2022                                               *
** Description:    This file is used to define uil ual wifi export       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 09/22/2022         miao.xi                 Create                       *
******************************************************************************/
#ifndef __UAL_WIFI_H
#define __UAL_WIFI_H

#include "ual_type.h"
#include "ual_common.h"
/**--------------------------------------------------------------------------*
**                               MACOR                                       *
**--------------------------------------------------------------------------*/
#define UAL_WIFI_CHANNEL_NUMBER 13
#define UAL_WIFI_SSID_MAX_LEN 32
#define UAL_WIFI_BSSID_MAX_LEN 6
/*---------------------------------------------------------------------------*/
/*                          ENUM AND STRUCT                                  */
/*---------------------------------------------------------------------------*/
typedef enum
{
    UAL_WIFI_RES_SUCCESS,
    UAL_WIFI_RES_PARAM_ERR =  UAL_MODULE_WIFI << 16,
    UAL_WIFI_RES_STATE_ERR,
    UAL_WIFI_RES_OTHER_ERR,
    UAL_WIFI_RES_CREATE_FSM_FAIL,
    UAL_WIFI_RES_MAX
} ual_wifi_result_e;

typedef boolean (*ual_wifi_injection_func)(uint32 event, void* p_param, uint32 param_size);

typedef struct
{
    uint8 ssid[UAL_WIFI_SSID_MAX_LEN];
    uint8 ssid_len;
}ual_wifi_ssid_t;

typedef struct
{
    uint8 bssid[UAL_WIFI_BSSID_MAX_LEN];
}ual_wifi_bssid_t;

typedef struct
{
    int8 rssival;
}ual_wifi_rssival_t;

typedef struct
{
    ual_wifi_ssid_t    ssid;
    ual_wifi_bssid_t    bssid_info;
    ual_wifi_rssival_t   rssival;
    uint8 channel;     // channel id
    uint16 reserved;
}ual_wifi_scan_ap_info_t;

typedef enum
{
   UAL_WIFI_STATE_IDLE,
   UAL_WIFI_STATE_OPENED,
   UAL_WIFI_STATE_SCANING,
   UAL_WIFI_STATE_MAX
}ual_wifi_state_e;

typedef enum
{
     UAL_WIFI_SCAN_BY_CHANNEL,
     UAL_WIFI_SCAN_BY_SSID,
     UAL_WIFI_SCAN_START_MODE_MAX
}ual_wifi_scan_start_mode_e;

typedef struct
{
    ual_wifi_scan_start_mode_e scan_mode;// scan_by_channle(0) OR scan_by_ssid(1)
    union
    {
        struct
        {
            uint16 nChNum;
            uint16 nMaxNodeNum;
            uint16 nch[UAL_WIFI_CHANNEL_NUMBER];
        }ual_wifi_scan_by_channle_t;
        struct
        {
            uint8 ssid[UAL_WIFI_SSID_MAX_LEN];
            uint8 ssid_len;
        }ual_wifi_scan_by_ssid_t;
    }ual_wifi_scan_param_u;
} ual_wifi_scan_req_param_t;

typedef enum
{
    //发给注入函数
    MSG_UAL_WIFI_OPEN_REQ =  UAL_MODULE_WIFI << 16,
    //确认app下发的请求
    MSG_UAL_WIFI_OPEN_CNF,
    MSG_UAL_WIFI_CLOSE_CNF,
    MSG_UAL_WIFI_START_SCAN_CNF,
    MSG_UAL_WIFI_STOP_SCAN_CNF,

    /*主动上报*/
    MSG_UAL_WIFI_SCAN_AP_INFO_IND,    //ap_list
    MSG_UAL_WIFI_SCAN_AP_INFO_CNF,

    MSG_UAL_WIFI_MAX
} ual_wifi_msg_e;

typedef struct
{
    boolean is_ok;   //相应的请求是否成功
}ual_wifi_open_cnf_t,
ual_wifi_start_scan_cnf_t,
ual_wifi_stop_scan_cnf_t;
/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description: init  wifi
//  Parameter:
//             [Return] ual_wifi_result_e
//  Author: miao.xi
//  Note:
/*****************************************************************************/
ual_wifi_result_e ual_wifi_init (void);
 /*****************************************************************************/
//  Description:  register cms
//  Parameter: [In] p_callback //注册的回调函数
//             [Out] p_handle //cms handle
//             [Return] ual_wifi_result_e
//  Author: miao.xi
//  Note:
/*****************************************************************************/
ual_wifi_result_e ual_wifi_register(_ual_cms_client_register_callback p_callback, uint32* p_handle);
/*****************************************************************************/
//  Description: unregister cms
//  Parameter: [In] handle //cms handle
//             [Return] ual_wifi_result_e
//  Author: miao.xi
//  Note:
/*****************************************************************************/
ual_wifi_result_e ual_wifi_unregister(uint32 handle);
 /*****************************************************************************/
//  Description: open wifi
//  Parameter:
//             [Return] ual_wifi_result_e
//  Author: miao.xi
//  Note:
/*****************************************************************************/
ual_wifi_result_e ual_wifi_open (void);
 /*****************************************************************************/
//  Description: close wifi
//  Parameter:
//             [Return] ual_wifi_result_e
//  Author: miao.xi
//  Note:
/*****************************************************************************/
ual_wifi_result_e ual_wifi_close (void);
 /*****************************************************************************/
//  Description: start scan
//  Parameter: [In] ual_wifi_scan_req_param_t //scan param
//             [Return] ual_wifi_result_e
//  Author: miao.xi
//  Note:
/*****************************************************************************/
ual_wifi_result_e ual_wifi_start_scan(ual_wifi_scan_req_param_t* p_wifi_scan_param);
 /*****************************************************************************/
//  Description: stop scan
//  Parameter:
//             [Return] ual_wifi_result_e
//  Author: miao.xi
//  Note:
/*****************************************************************************/
ual_wifi_result_e ual_wifi_stop_scan(void);
 /*****************************************************************************/
//  Description: get scan ap list
//  Parameter: [In]  need_node_number:the number of ap_list you want
//                   [Out]ap_list:allocated array space
//                   [Return] ual_wifi_result_e
//  Author: miao.xi
//  Note:
/*****************************************************************************/
ual_wifi_result_e ual_wifi_get_scaned_ap_list(ual_wifi_scan_ap_info_t* p_ap_list,uint32 need_node_number);
 /*****************************************************************************/
//  Description:
//  Parameter: [In]
//             [Out] status
//             [Return] ual_wifi_result_e
//  Author: miao.xi
//  Note:
/*****************************************************************************/
ual_wifi_result_e ual_wifi_get_current_state(ual_wifi_state_e * p_wifi_state);
 /*****************************************************************************/
//  Description: set注入函数
//  Parameter: [In] p_injection_func       //外部模块注入的函数
//             [Out] none
//             [Return] ual_wifi_result_e
//  Author: miao.xi
//  Note:
/*****************************************************************************/
ual_wifi_result_e ual_wifi_inject_func(ual_wifi_injection_func p_func);
/*****************************************************************************/
//  Description: clear注入函数
//  Parameter: [In] none
//             [Out] none
//             [Return] ual_wifi_result_e
//  Author: miao.x
//  Note:
/*****************************************************************************/
ual_wifi_result_e ual_wifi_unject_func();
#endif
