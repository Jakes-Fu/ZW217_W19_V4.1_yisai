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
** File Name:      ual_autotest_atc_bt_ble.h                                 *
** Author:         dandan.cao                                                *
** Date:           05/20/2021                                                *
** Description:    This file is used to describe AT command                  *
******************************************************************************/

#ifndef _UAL_ATC_BT_AT_H_
#define _UAL_ATC_BT_AT_H_
#define BT_AT_SPEC_VERSION          "V2.0.3"
//BLE COMM CMD
#define UAL_ATC_BT_BLE_COMMKEYWORD              "AT+BLECOMM="
#define UAL_ATC_BT_BLE_COMM_KEYWORD_LEN         (11)
#define UAL_ATC_BT_BLE_COMM_RESP_ERR            "+BLECOMM:ERR="
#define UAL_ATC_BT_BLE_COMM_RESP_OK             "+BLECOMM:OK"

//BLE ADV CMD
#define UAL_ATC_BT_BLE_ADV_KEYWORD              "AT+BLEADV="
#define UAL_ATC_BT_BLE_ADV_KEYWORD_LEN          (10)
#define UAL_ATC_BT_BLE_ADV_RESP_ERR             "+BLEADV:ERR="
#define UAL_ATC_BT_BLE_ADV_RESP_OK              "+BLEADV:OK"

//BLE DATA CMD
#define UAL_ATC_BT_BLE_DATA_KEYWORD             "AT+BLEDATA="
#define UAL_ATC_BT_BLE_DATA_KEYWORD_LEN         (11)
#define UAL_ATC_BT_BLE_DATA_RESP_ERR            "+BLEDATA:ERR="
#define UAL_ATC_BT_BLE_DATA_RESP_OK             "+BLEDATA:OK"

//BLE SCAN CMD
#define UAL_ATC_BT_BLE_SCAN_KEYWORD             "AT+BLESCAN="
#define UAL_ATC_BT_BLE_SCAN_KEYWORD_LEN         (11)
#define UAL_ATC_BT_BLE_SCAN_RESP_ERR            "+BLESCAN:ERR="
#define UAL_ATC_BT_BLE_SCAN_RESP_OK             "+BLESCAN:OK"

typedef unsigned char    boolean;//UAL_TODO: 包头文件的方式
typedef enum{
    UAL_ATC_BT_BLE_OK,
    UAL_ATC_BT_BLE_ERROR,
    UAL_ATC_BT_BLE_WAIT,
}ual_atc_bt_ble_return_state_e;

typedef enum{
    UAL_ATC_BT_BLE_REGISTER_ADV,
    UAL_ATC_BT_BLE_REGISTER_COMMON,// 注册连接信息
    UAL_ATC_BT_BLE_REGISTER_SCAN,
    UAL_ATC_BT_BLE_REGISTER_DATA,
}ual_atc_bt_ble_register_type_e;//UAL_TODO:分类的讨论

typedef struct  {
  int index;
  char *name;
  int (*handler)(char *argv[], int argc, char *rsp, int *rsp_len);
  char * help;
} ual_atc_bt_ble_cmd_t;

/*****************************************************************************/
//  Description : registering ble call back for engpc
//  Global resource dependence:
//  Param in:none
//  Pram out:none
//  Author: dandan.cao
//  Note:
/*****************************************************************************/
boolean ual_bt_ble_atc_init(ual_atc_bt_ble_register_type_e register_type);

#endif //_UAL_ATC_BT_AT_H_

