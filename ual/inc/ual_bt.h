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
** File Name:       ual_bt.h                                                 *
** Author:          xiuyun.wang                                              *
** Date:             01/11/2021                                              *
** Description:    This file is used to define ual bt export api of bt       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 01/11/2021           xiuyun.wang           Create                         *
******************************************************************************/
#ifndef _UAL_BT_H_
#define _UAL_BT_H_
#include "ual_bt_common.h"
#ifdef UAL_BT_CLASSIC_DEV_SUPPORT
#include "ual_bt_dev.h"
#endif
#ifdef UAL_BT_BLE_DEV_SUPPORT
#include "ual_bt_ble_dev.h"
#endif
#ifdef UAL_BT_HFU_SUPPORT
#include "ual_bt_hfu.h"
#endif
#endif
