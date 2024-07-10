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
** File Name:       ual_bt_ble_private.h                                     *
** Author:          changli.jiang                                            *
** Date:             02/27/2021                                              *
** Description:    This file is used to define ual_bt_ble_private api        *
** this api will not change even if ble ps change,we can support goodix and  *
** unisoc ps and other third party.now we only support goodix,unisoc will be *
** added later
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 02/27/2021           changli.jiang           Create                       *
******************************************************************************/
#ifndef _UAL_BT_BLE_PRIVATE_H_
#define _UAL_BT_BLE_PRIVATE_H_
// TODO:
//#include "ual_bt_common.h"
#define UAL_MAX_PRIVATE_DATA_LEN 238
typedef enum
{
    UAL_BT_BLE_PRIVATE_RESULT_START = 0,
    UAL_BT_BLE_PRIVATE_RESULT_SUCESS,//调用接口成功
    UAL_BT_BLE_PRIVATE_RESULT_PARAM_ERROR,//参数错误
    UAL_BT_BLE_PRIVATE_RESULT_OTHER_ERROR,//其他错误
}ual_bt_ble_private_result_e;
typedef enum
{
    UAL_BT_BLE_PRIVATE_ESIM = 0xffbe0000,//esim through ble
    UAL_BT_BLE_PRIVATE_MESSAGE,//receive a message through ble
    UAL_BT_BLE_PRIVATE_HEART_RATE,//heart rate
    UAL_BT_BLE_PRIVATE_STEP_COUNT,// step counter
    UAL_BT_BLE_PRIVATE_WATCHFACE,//watchface through ble
    UAL_BT_BLE_PRIVATE_NFC,//nfc module
    UAL_BT_BLE_PRIVATE_INCALL,//incoming call
    UAL_BT_BLE_PRIVATE_APPSTORE,//appstore through ble
    // TODO:不同类型可以从此地方扩充
    UAL_BT_BLE_PRIVATE_MAX
}ual_bt_ble_private_message_type_e;
/**************************************************************/
/*           p_msg_data is match with msg_type                */
/* EX:MSG TYPE:UAL_BT_BLE_PRIVATE_INCALL:                     */
/*    P_MSG_DATA:{CALL NUMBER://,CALL NAME://}                */
/**************************************************************/
typedef struct
{
    ual_bt_ble_private_message_type_e msg_type;//以此区分是否是不同的模块
    uint16 msg_len;// real data len
    uint8 msg_data[UAL_MAX_PRIVATE_DATA_LEN];//real data
}ual_bt_ble_private_message_data_t;

typedef enum
{
    UAL_BT_BLE_PRIVATE_OPERATE_SUCESS = 0,//operate sucess
    UAL_BT_BLE_PRIVATE_OPERATE_FAIL,
}ual_bt_ble_private_operate_e;
// this is corresponding to ual_bt_ble_private_msg_e,msg UAL_BT_BLE_PRIVATE_MSG_RECEIVE_DATA_CNF
// not carry data
typedef struct
{
    ual_bt_ble_private_operate_e private_operate_state;
}ual_bt_ble_private_send_data_cnf_t;

typedef enum
{
    UAL_BT_BLE_PRIVATE_MSG_SEND_DATA_CNF,
    UAL_BT_BLE_PRIVATE_MSG_RECEIVE_DATA_CNF,
}ual_bt_ble_private_msg_e;
// TODO:
/*****************************************************************************/
//  Description: ual_bt_ble_private_register
//  Parameter: [In] p_callback private call back
//             [Out] p_handle out of phandle
//             [Return] ual_bt_register_res_e
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_private_result_e ual_bt_ble_private_register(_ual_cms_client_register_callback p_callback,uint32 *p_handle);
/*****************************************************************************/
//  Description: ual_bt_ble_private_unregister
//  Parameter: [in] handle
//             [Out] none
//             [Return] true:success,false:fail
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
boolean ual_bt_ble_private_unregister(uint32 handle);
/*****************************************************************************/
//  Description : this api used to send private data
//  Global resource dependence:
//  Param in:p_data:data of private
//           data_len:data_len of private
//  Pram out:ual_bt_ble_private_result_e error
//  Author: changli.jiang
//  Note:
/*****************************************************************************/
ual_bt_ble_private_result_e ual_bt_ble_private_send_data(ual_bt_ble_private_message_data_t* p_data,uint16 data_len);
#endif
