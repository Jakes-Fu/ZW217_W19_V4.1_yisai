/*****************************************************************************
** Copyright 2020-2022 Unisoc(Shanghai) Technologies Co.,Ltd                 *
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
** File Name:   ual_bt_dev.h                                                 *
** Author:      chunjuan.liang                                               *
** Date:        2020/01/06                                                   *
** Description: This file is used to define data struct & functions for      *
                device manager                                               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
**   DATE                  NAME              DESCRIPTION                     *
** 02/06/2021           chunjuan.liang           Create                      *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#ifndef _UAL_BT_DEV_H_
#define _UAL_BT_DEV_H_

#include "ual_bt_common.h"
#include "ual_cms_export.h"

#define UAL_BT_DEV_ADDRESS_SIZE     (6)//蓝牙地址长度
#define UAL_BT_DEV_NAME_MAX_LEN     (41)//蓝牙名称最大长度
#define UAL_BT_DEV_MAX_COUNT        (20)//支持的最多设备个数,和BT Stack一致
#define UAL_BT_DEV_SSP_NUM_MAX_LEN  (7)//SSP code max len

#define UAL_BT_PHONE                (1)// means phone
#define UAL_BT_DIALER               (0)// means headset

typedef enum
{
    UAL_BT_DEV_STATUS_INIT,
    UAL_BT_DEV_STATUS_OPENING,
    UAL_BT_DEV_STATUS_OPENED,
    UAL_BT_DEV_STATUS_CLOSING,
    UAL_BT_DEV_STATUS_CLOSED,
    UAL_BT_DEV_STATUS_SEARCHING,
    UAL_BT_DEV_STATUS_PAIRING,
    UAL_BT_DEV_STATUS_MAX
}ual_bt_dev_status_e;

typedef enum
{
    UAL_BT_DEV_NONE_VISIBLE,
    UAL_BT_DEV_ONLY_SEARCH_VISIBLE,
    UAL_BT_DEV_ONLY_CONNECTION_VISIBLE,
    UAL_BT_DEV_SEARCH_CONNECTION_VISIBLE,
    UAL_BT_DEV_VISIBLILITY_MAX
}ual_bt_visibility_opt_e;

typedef struct
{
    uint16 name[UAL_BT_DEV_NAME_MAX_LEN];//unicode
    uint8  addr[UAL_BT_DEV_ADDRESS_SIZE];
    uint32 dev_class;
    uint32 rssi;
} ual_bt_dev_info_t;

typedef struct
{
    uint16          name[UAL_BT_DEV_NAME_MAX_LEN];//unicode
    uint8           addr[UAL_BT_DEV_ADDRESS_SIZE];
    uint32          dev_class;
    uint8           ssp_numric_key[UAL_BT_DEV_SSP_NUM_MAX_LEN];// 6 valid bytes, add 1 more "\0"
} ual_bt_dev_info_ssp_t;
//ual transfer to app msg
typedef enum
{
    UAL_APP_BT_DEV_START,
    UAL_APP_BT_DEV_OPENED,   //蓝牙已打开
    UAL_APP_BT_DEV_OPEN_ERR, //蓝牙打开失败
    UAL_APP_BT_DEV_CLOSED,   //蓝牙已关闭
    UAL_APP_BT_DEV_CLOSE_ERR,//蓝牙关闭失败
    UAL_APP_BT_DEV_SEARCH_COMPLETE,    //搜索完成
    UAL_APP_BT_DEV_CANCEL_SEARCHED,    //取消搜索
    UAL_APP_BT_DEV_SEARCH_COMPLETE_ERR,//搜索失败
    UAL_APP_BT_DEV_PAIR_PIN_INPUT,   //输入pin码
    UAL_APP_BT_DEV_SEARCH_ONE, //搜到一个设备
    UAL_APP_BT_DEV_SEARCH_NONE,//搜索完成，未搜到设备
    UAL_APP_BT_DEV_PAIR_SSP_NUM, //输入ssp code
    UAL_APP_BT_DEV_PAIRED,      //配对完成
    UAL_APP_BT_DEV_PAIR_IND,   //被动配对和取消配对
    UAL_APP_BT_DEV_PAIR_ERR,   //配对出错
    UAL_APP_BT_DEV_DELETED,   //删除完成
    UAL_APP_BT_DEV_DELETE_ERR,//删除出错
    UAL_APP_BT_DEV_MAX
}ual_app_bt_dev_e;

/*device service*/
#define UAL_BT_SERVICE_NONE               0x0000
#define UAL_BT_SERVICE_HANDFREE           0x0001
#define UAL_BT_SERVICE_HANDSET            0x0002
#define UAL_BT_SERVICE_OPP                0x0004
#define UAL_BT_SERVICE_FTP                0x0008
#define UAL_BT_SERVICE_A2DP               0x0010 
#define UAL_BT_SERVICE_AVRCP              0x0020
#define UAL_BT_SERVICE_SPP                0x0040
#define UAL_BT_SERVICE_A2DP_SRC           0x0080
#define UAL_BT_SERVICE_AVRCP_TG           0x0100
#define UAL_BT_SERVICE_MAP_SERVER         0x0200
#define UAL_BT_SERVICE_PBAP_SERVER        0x0400
#define UAL_BT_SERVICE_HANDFREE_GATEWAY   0x0800
#define UAL_BT_SERVICE_HEADSET_GATEWAY    0x1000
#define UAL_BT_SERVICE_PBAP_CLIENT        0x2000
#define UAL_BT_SERVICE_BLE_HID            0x3000
#define UAL_BT_SERVICE_ALL                0xFFFF

/*device type*/
#define UAL_BT_DEVICE_CLASS_NONE          0x00000000
#define UAL_BT_DEVICE_CLASS_COMPUTER      0x00000001
#define UAL_BT_DEVICE_CLASS_PHONE         0x00000002
#define UAL_BT_DEVICE_CLASS_AUDIO         0x00000004
#define UAL_BT_DEVICE_CLASS_PRINTER       0x00000008


/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description: ual_bt_init
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
void ual_bt_init(void);

/*****************************************************************************/
//  Description: ual_bt_dev_register
//  Parameter: [In] callback
//             [Out] none
//             [Return] ual_bt_register_res_e
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
boolean ual_bt_dev_register(_ual_cms_client_register_callback p_callback,uint32 *p_handle);

/*****************************************************************************/
//  Description: ual_bt_dev_unregister
//  Parameter: [In] handle,register时的返回值
//             [Out] none
//             [Return] true:success,false:fail
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
boolean ual_bt_dev_unregister(uint32 handle);

/*****************************************************************************/
//  Description: ual_bt_dev_open
//  Parameter: [In] none
//             [Out] none
//             [Return] ual_bt_status_e
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_dev_open(void);

/*****************************************************************************/
//  Description: ual_bt_dev_close
//  Parameter: [In] none
//             [Out] none
//             [Return] none
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_dev_close(void);

/*****************************************************************************/
//  Description: ual_bt_dev_start_search
//  Parameter: [In] type
//             [Out] none
//             [Return] ual_bt_status_e
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_dev_start_search(uint32 type);

/*****************************************************************************/
//  Description: ual_bt_dev_cancel_search
//  Parameter: [In] none
//             [Out] none
//             [Return] ual_bt_status_e
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_dev_cancel_search(void);

/*****************************************************************************/
//  Description: ual_bt_dev_start_pair
//  Parameter: [In] bt addr
//             [Out] none
//             [Return] ual_bt_status_e 
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_dev_start_pair(uint8 *p_addr);

/*****************************************************************************/
//  Description: ual_bt_dev_cancel_pair
//  Parameter: [In] bt addr
//             [Out] none
//             [Return] ual_bt_status_e
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_dev_cancel_pair(uint8 *p_addr);


/*****************************************************************************/
//  Description: ual_bt_dev_input_pinpair
//  Parameter: [In] bt addr,pin/ssp code,pin/ssp code len
//             [Out] none
//             [Return] ual_bt_status_e
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_dev_input_pinpair(uint8 *p_addr,uint8 *p_pin,uint16 pin_len);

/*****************************************************************************/
//  Description: ual_bt_dev_delete_paired_dev
//  Parameter: [In] bt addr
//             [Out] none
//             [Return] ual_bt_status_e
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_dev_delete_paired_dev(uint8 *p_addr);

/*****************************************************************************/
//  Description: ual_bt_dev_get_is_on
//  Parameter: [In] none
//             [Out] none
//             [Return] true:open;false:close
//  Author: chunjuan.liang
//  Note:get bt stack status
/*****************************************************************************/
boolean ual_bt_dev_get_is_on(void);

/*****************************************************************************/
//  Description: ual_bt_dev_get_status
//  Parameter: [In] none
//             [Out] none
//             [Return]:bt service status
//  Author: chunjuan.liang
//  Note:get bt service status
/*****************************************************************************/
ual_bt_dev_status_e  ual_bt_dev_get_status(void);

/*****************************************************************************/
//  Description: ual_bt_dev_is_in_pairedlist
//  Parameter: [In] bt addr
//             [Out] none
//             [Return] boolean
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
boolean ual_bt_dev_is_in_paired_list(uint8 *p_addr);

//  Description: ual_bt_dev_get_searched_list
//  Parameter: [In] none
//             [Out] none
//             [Return] 
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
ual_bt_dev_info_t * ual_bt_dev_get_searched_list(void);

/*****************************************************************************/
//  Description: ual_bt_dev_get_searched_num
//  Parameter: [In] none
//             [Out] none
//             [Return] searched dev number
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
uint8 ual_bt_dev_get_searched_num(void);

/*****************************************************************************/
//  Description: ual_bt_dev_destroy_devinfo
//  Parameter: [In] none
//             [Out] none
//             [Return] 
//  Author: chunjuan.liang
//  Note:close app win,destroy device info
/*****************************************************************************/
//boolean ual_bt_dev_destroy_devinfo(void);

/*****************************************************************************/
//  Description: ual_bt_get_product_role
//  Parameter: [In] None
//             [Out] None
//             [Return] bt role,1:phone,0:dialer
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
uint8 ual_bt_get_product_role(void);

/*****************************************************************************/
//  Description: ual_bt_set_product_role
//  Parameter: [In] bt role,1:phone,0:dialer
//             [Out] None
//             [Return] TRUE,set success,FALSE,set fail
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
boolean ual_bt_set_product_role(uint8 role);

/*****************************************************************************/
//  Description: ual_bt_dev_get_is_on
//  Parameter: [In] none
//             [Out] none
//             [Return] true:open;false:close
//  Author: chunjuan.liang
//  Note:get bt stack on/off status
/*****************************************************************************/
boolean ual_bt_dev_get_is_on(void);
/*****************************************************************************/
//  Description: set bt local name
//  Parameter: [In] *p_local_name: local name wchar string
//             [Out] None
//             [Return] set result
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_dev_set_local_name(wchar *p_local_name);
/*****************************************************************************/
//  Description: get bt visible
//  Parameter: [In] None
//             [Out] None
//             [Return] bt visible state
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
ual_bt_visibility_opt_e ual_bt_dev_get_visible(void);
/*****************************************************************************/
//  Description: set bt visible
//  Parameter: [In] visible_opt:visible option in ual
//             [Out] None
//             [Return] set result
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_dev_set_visible(ual_bt_visibility_opt_e visible_opt);
/*****************************************************************************/
//  Description: get bt local name
//  Parameter: [In] *p_local_name: local name wchar string
//             [In] max_local_name_len: p_local_name max len 
//             [Out] None
//             [Return] set result
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
ual_bt_status_e ual_bt_dev_get_local_name(wchar *p_local_name, uint32 max_local_name_len);

/*****************************************************************************/
//  Description: ual_bt_dev_init_status
//  Parameter: [In] none
//             [Out] none
//             [Return] true:open;false:close
//  Author: chunjuan.liang
//  Note:get bt stack on/off status
/*****************************************************************************/
boolean ual_bt_dev_init_status(void);

/*****************************************************************************/
//  Description: ual_bt_dev_get_pair_info
//  Parameter: [In] none
//             [Out] none
//             [Return] true:open;false:close
//  Author: chunjuan.liang
//  Note:get pair info
/*****************************************************************************/
ual_bt_status_e ual_bt_dev_get_pair_info(uint32 service_type,uint16 index,ual_bt_dev_info_t *p_dev_info);

/*****************************************************************************/
//  Description: ual_bt_dev_get_dev_type
//  Parameter: [In] ual_bt_dev
//             [Out] dev type
//             [Return] None
//  Author: chunjuan.liang
//  Note:根据蓝牙设备信息，获取蓝牙类型
/*****************************************************************************/
boolean ual_bt_dev_get_dev_type(ual_bt_dev_info_t *p_bt_dev,uint16 *p_dev_type);

/*****************************************************************************/
//  Description: ual_bt_dev_get_paired_dev_count
//  Parameter: [In] ual_service_type:对应类型
//             [Out] none
//             [Return] uint32:dev num，设备数目
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/

uint32 ual_bt_dev_get_paired_dev_count(uint32 ual_service_type);

/*****************************************************************************/
//  Description: ual_bt_dev_enable_phone
//  Parameter: [In] none
//             [Out] none
//             [Return] ual_bt_status_e
//  Author: chunjuan.liang
//  Note:用户打开蓝牙phone形态，先判断之前的状态是否是dialermode,,如果为dialermode
//       需要先把BT关掉，设置mode，再打开BT
/*****************************************************************************/
ual_bt_status_e ual_bt_dev_enable_phone(void);

/*****************************************************************************/
//  Description: ual_bt_dev_enable_dialer
//  Parameter: [In] none
//             [Out] none
//             [Return] ual_bt_status_e
//  Author: chunjuan.liang
//  Note:用户打开蓝牙dialer mode，先判断之前的状态是否是phone mode,,如果为phone mode
//       需要先把BT关掉，设置mode，再打开BT
/*****************************************************************************/
ual_bt_status_e ual_bt_dev_enable_dialer(void);

/*****************************************************************************/
//  Description: ual_bt_dev_disable_dialer
//  Parameter: [In] none
//             [Out] none
//             [Return] none
//  Author: chunjuan.liang
//  Note:用户关闭dialer mode，先判断之前的状态是否是dilaer mode,如果为dialer mode
//       关BT,如果不是dialer mode,不去关闭
/*****************************************************************************/
void ual_bt_dev_disable_dialer(void);

/*****************************************************************************/
//  Description: ual_bt_dev_disable_phone
//  Parameter: [In] none
//             [Out] none
//             [Return] ual_bt_status_e
//  Author: chunjuan.liang
//  Note:用户关闭phone mode，先判断之前的状态是否是phone mode,如果为phone mode
//       关BT,如果不是phone mode,不去关闭
/*****************************************************************************/
void ual_bt_dev_disable_phone(void);

#endif

