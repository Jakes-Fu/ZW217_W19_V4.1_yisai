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
 * @file     pm_data_cache.h
 * @brief    -
 * @author   ningbiao.huang
 * @version  V1.0.0
 * @date     2022-6-13
 * @history
 * @note
 * detailed description:
 *****************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2021 unisoc </center></h2>
 **/

#ifndef PM_DATA_CACHE_H
#define PM_DATA_CACHE_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#include "os_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

// data/app/[app_id] dir
PUBLIC BOOLEAN create_data_appid_dir(char *p_app_id);
PUBLIC BOOLEAN remove_data_appid_dir(char *p_app_id);

// data/app/[app_id]/data dir
PUBLIC BOOLEAN create_data_appid_data_dir(char *p_app_id);
PUBLIC BOOLEAN remove_data_appid_data_dir(char *p_app_id);

// data/app/[app_id]/cache dir
PUBLIC BOOLEAN create_data_appid_cache_dir(char *p_app_id);
PUBLIC BOOLEAN remove_data_appid_cache_dir(char *p_app_id);

// data/app/[app_id]/ path
PUBLIC void get_data_app_id_path(char *p_app_id, wchar *output_path);
PUBLIC void get_app_data_path(char *p_app_id, wchar *output_path);
PUBLIC void get_app_cache_path(char *p_app_id, wchar *output_path);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // End