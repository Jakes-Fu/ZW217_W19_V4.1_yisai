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
** File Name:       ual_device.h                                               *
** Author:          chaonan.hao                                                *
** Date:            03/15/2023                                                 *
** Description:     This file is used to define ual device export api          *
********************************************************************************
**                         Important Edit History                              *
** ----------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                      *
** 03/15/2023          chaonan.hao                Create                       *
********************************************************************************/
#ifndef _UAL_DEVICE_H_
#define _UAL_DEVICE_H_


/**----------------------------------------------------------------------------*
**                         Include Files                                       *
**-----------------------------------------------------------------------------*/
#include "ual_type.h"
#include "ual_common.h"

/**----------------------------------------------------------------------------*
**                         MACRO DEFINITION                                    *
**-----------------------------------------------------------------------------*/


/* API result*/
typedef enum
{
    UAL_DEVICE_RES_SUCCESS,
    UAL_DEVICE_RES_PARAM_ERROR,
    UAL_DEVICE_RES_FAIL,
    UAL_DEVICE_RES_MAX
} ual_device_result_e;

//DEVICE Service MSG
typedef enum
{
    MSG_UAL_DEVICE_THERMAL_CLOSE_BUSINESS = (UAL_MODULE_DEVICE << 16) + 0x01,
    MSG_UAL_DEVICE_THERMAL_REOPEN_BUSINESS,
    MSG_UAL_DEVICE_THERMAL_POWER_OFF,
    MSG_UAL_DEVICE_MAX_NUM
}ual_device_msg_e;

/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:  register ual device
//  Parameter: [In] p_callback //注册的回调函数
//             [Out] p_handle //cms handle
//             [Return] ual_device_result_e
//  Author: chaonan.hao
//  Note:
/*****************************************************************************/
ual_device_result_e ual_device_register(_ual_cms_client_register_callback p_callback, uint32* p_handle);
/*****************************************************************************/
//  Description: unregister ual device
//  Parameter: [In] handle //cms handle
//             [Return] ual_device_result_e
//  Author: chaonan.hao
//  Note:
/*****************************************************************************/
ual_device_result_e ual_device_unregister(uint32 handle);

 /*****************************************************************************/
//  Description: init ual device
//  Parameter:
//             [Return] ual_device_result_e
//  Author: chaonan.hao
//  Note:
/*****************************************************************************/
ual_device_result_e ual_device_init (void);

/*****************************************************************************/
//  Description: deinit ual device
//  Parameter:
//             [Return] ual_device_result_e
//  Author: chaonan.hao
//  Note:
/*****************************************************************************/
ual_device_result_e ual_device_deinit (void);
#endif
