/**
 * SPDX-FileCopyrightText: 2022-present Unisoc (Shanghai) Technologies Co., Ltd
 * SPDX-License-Identifier: LicenseRef-Unisoc-General-1.0
 *
 * Copyright 2022-present Unisoc (Shanghai) Technologies Co., Ltd.
 * Licensed under the Unisoc General Software License V1;
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * Software distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 * See the Unisoc General Software License v1 for more details.
 *
 **/

/**
 *****************************************************************************
 * @file     permision_check.h
 * @brief    -
 * @author  zhigao.sun@unisoc.com
 * @version  V1.0.0
 * @date
 * @history
 * @note
 * detailed description:
 *****************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2022 unisoc </center></h2>
 **/

#include "jee_list.h"
#define MAX_NAME_LENTH 128
// Match app.json permission name
#define PERM_CAMERA_NAME "PERMISSION.CAMERA"
#define PERM_READ_CONTACTS_NAME "PERMISSION.READ_CONTACTS"
#define PERM_WRITE_CONTACTS_NAME "PERMISSION.WRITE_CONTACTS"
#define PERM_ACCESS_FINE_LOCATION_NAME "PERMISSION.ACCESS_FINE_LOCATION"
#define PERM_ACCESS_COARSE_LOCATION_NAME "PERMISSION.ACCESS_COARSE_LOCATION"
#define PERM_SEND_SMS_NAME "PERMISSION.SEND_SMS"
#define PERM_READ_SMS_NAME "PERMISSION.READ_SMS"
#define PERM_RECORD_AUDIO_NAME "PERMISSION.RECORD_AUDIO"
#define PERM_CALL_PHONE_NAME "PERMISSION.CALL_PHONE"
#define PERM_BODY_SENSORS_NAME "PERMISSION.BODY_SENSORS"
#define PERM_PACKAGES_MANAGER_NAME "PERMISSION.PACKAGES_MANAGER"
#define PERM_SYSTEM_STORAGE_NAME "PERM_SYSTEM_STORAGE"
#define PERM_ACCESS_WIFI_NAME "PERMISSION.ACCESS_WIFI"
#define PERM_BATTERY_STATS_NAME "PERMISSION.BATTERY_STATS"
#define PERM_BLUETOOTH_NAME "PERMISSION.BLUETOOTH"
#define PERM_DEVICE_POWER_NAME "PERMISSION.DEVICE_POWER"
#define PERM_FLASHLIGHT_NAME "PERMISSION.FLASHLIGHT"
#define PERM_INTERNET_NAME "PERMISSION.INTERNET"
#define PERM_NFC_NAME "PERMISSION.NFC"
#define PERM_VIBRATE_NAME "PERMISSION.VIBRATE"
#define PERM_EXTERNAL_STORAGE_NAME "PERMISSION.EXTERNAL_STORAGE"

typedef enum {
  PREMISSION_STATUS_GRANTED = 20001,
  PREMISSION_STATUS_GRANTED_ONCE,
  PREMISSION_STATUS_DENIED,
  PREMISSION_STATUS_NONE
} grant_permission_status_e;

typedef enum {
  PERM_CAMERA = 0,
  PERM_READ_CONTACTS,
  PERM_WRITE_CONTACTS,
  PERM_ACCESS_FINE_LOCATION,
  PERM_ACCESS_COARSE_LOCATION,
  PERM_SEND_SMS,
  PERM_READ_SMS,
  PERM_RECORD_AUDIO,
  PERM_CALL_PHONE,
  PERM_BODY_SENSORS,
  PERM_PACKAGES_MANAGER,
  PERM_SYSTEM_STORAGE,
  PERM_INTERNET,
  PERM_SENSITIVE_MAX
} sensitivepermission_id_e;

typedef enum {

  PERM_ACCESS_WIFI = 10001,
  PERM_BATTERY_STATS,
  PERM_BLUETOOTH,
  PERM_DEVICE_POWER,
  PERM_FLASHLIGHT,
  // PERM_INTERNET,
  PERM_NFC,
  PERM_VIBRATE,
  PERM_EXTERNAL_STORAGE,
  PERM_INSENSITIVE_MAX
} insensitivepermission_id_e;

typedef struct _PERMISSION_INFO {
  int id;
  char info[128];
};

typedef struct //存储某个权限的数据结构体
{
  int grant_permission_id;
  grant_permission_status_e grant_permission_status;
  list_node grant_permission_info_node;
} grant_permission_info_t;

typedef struct //某个app的权限信息结构体
{
  char app_id[MAX_NAME_LENTH];
  // char app_id;
  char permission_file_path[256];
  list_node *p_grant_permission_list_head;
  list_node app_grant_list_node;
} app_grant_permission_info_t;

typedef int (*PERM_NOTFIY)(int code);

void perm_grant_app_permissions(char *app_id, PERM_NOTFIY);

grant_permission_status_e perm_check_permission_status(int permission_id);