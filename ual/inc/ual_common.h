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

/*****************************************************************************
** File Name:      ual_common.h
** Author:
** Description:
*****************************************************************************/
#ifndef _UAL_COMMON_H_
#define _UAL_COMMON_H_

#include "ual_type.h"
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
/*UAL msg format define, 提供给发送方底层callback用，也给client注册的callback用*/
typedef struct _UAL_CMS_MSG_Tag
{
    uint32      msg_id;
    uint32      body_size;
    void*       p_body;
} ual_cms_msg_t;

typedef boolean(* _ual_cms_client_register_callback)(ual_cms_msg_t param);



/*---------------------------------------------------------------------------*/
/*                          ENUM AND STRUCT                                */
/*---------------------------------------------------------------------------*/

/*ual各模块使用的module_id*/
typedef enum
{
    UAL_MODULE_START = 0,
    UAL_MODULE_BT_CLASSIC_DEVICE = UAL_MODULE_START,
    UAL_MODULE_BT_HFU,
    UAL_MODULE_BT_HFG,
    UAL_MODULE_BT_SPP,
    UAL_MODULE_BT_FTP,
    UAL_MODULE_BT_A2DP,
    UAL_MODULE_BT_AVRCP,
    UAL_MODULE_BT_PBAP,
    UAL_MODULE_BT_MAP,
    UAL_MODULE_BT_OPP,
    UAL_MODULE_BT_BLE_DEVICE,
    UAL_MODULE_BT_BLE_PRIVATE,
    UAL_MODULE_SENSORHUB,
    UAL_MODULE_AUTOTEST,
    UAL_MODULE_AUDIOPLAYER,
    UAL_MODULE_AUDIOSOURCE,
    UAL_MODULE_NETWORKMGR,
    UAL_MODULE_TELE_SIM,
    UAL_MODULE_TELE_RADIO,
    UAL_MODULE_TELE_DATA,
    UAL_MODULE_TELE_CALL,
    UAL_MODULE_TELE_SS,
    UAL_MODULE_CSM,
    UAL_MODULE_PDP,
    UAL_MODULE_TIMER,
    UAL_MODULE_SFR,
    UAL_MODULE_GNSS,
    UAL_MODULE_WIFI,
    UAL_MODULE_ADAP_WIFI,
    UAL_MODULE_RF_AM,
    UAL_MODULE_DEVICE,
    UAL_MODULE_ADAP_DEVICE,
    UAL_MODULE_MAX
}ual_module_e;

#ifndef WIN32
#define TRACE_UAL(ual_module_id,_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, #ual_module_id, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)

#else
#define TRACE_UAL(ual_module_id,_format,...) \
    do \
    { \
        printf("{%s} "_format, #ual_module_id, ##__VA_ARGS__);\
    }while(0)
#endif

#endif // _UAL_COMMON_H_

