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
** File Name:       ual_kv.h                                                 *
** Author:          longwei.qiao                                             *
** Date:            10/15/2021                                               *
** Description:    This file is used to define ual key value store export    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 10/15/2021        longwei.qiao               Create                       *
******************************************************************************/

#ifndef UAL_KV_H
#define UAL_KV_H
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ual_type.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
typedef enum
{
    UAL_KV_RESULT_SUCCESS,
    UAL_KV_RESULT_KEY_NOT_EXIST,
    UAL_KV_RESULT_PARAMETER_ERROR,
    UAL_KV_RESULT_SYSTEM_ERROR,
    UAL_KV_RESULT_NO_CAPACITY,
    UAL_KV_RESULT_MAX
}ual_kv_result_e;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Obtains the value matching a specified key from the storage
//  Parameter: [In] key: Indicates the key to be indexed.it allows only uppercase or lowercase letters,
//                       digits, underscores (_), and dots (.).
//                       Its length cannot exceed 32 bytes (including the end-of-text character in the string).
//             [In/Out] len: (in)Indicates the size of the value space in the buffer.(out)indicates the length of the value in the kv
//             [Out] value :indicates the buffer for storing the value that matches the key.
//             [Return] ual_kv_result_e:the result of API
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
ual_kv_result_e ual_kv_get_value(const char* key, char* value, uint8* len);

/*****************************************************************************/
//  Description : Set the value matching a specified key from the storage
//  Parameter: [In] key: Indicates the key to be indexed.it allows only uppercase or lowercase letters,
//                       digits, underscores (_), and dots (.).
//                       Its length cannot exceed 32 bytes (including the end-of-text character in the string).
//             [In] value :Indicates the value to be added or updated.
//                         Its length cannot exceed 128 bytes (including the end-of-text character in the string).
//             [Return] ual_kv_result_e:the result of API
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
ual_kv_result_e ual_kv_set_value(const char* key, const char* value);

/*****************************************************************************/
//  Description : Get the value matching a specified key from the storage
//  Parameter: [In] key: Indicates the key to be indexed.it allows only uppercase or lowercase letters,
//                       digits, underscores (_), and dots (.).
//                       Its length cannot exceed 32 bytes (including the end-of-text character in the string).
//             [Return] ual_kv_result_e:the result of API
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
ual_kv_result_e ual_kv_delete_value(const char* key);

#endif  // UAL_KV_STORE_H
