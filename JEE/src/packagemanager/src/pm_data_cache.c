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
 * @file     pm_data_cache.c
 * @brief    -
 * @author   ningbiao.huang
 * @version  V1.0.0
 * @date
 * @history
 * @note
 * detailed description:
 *****************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2021 unisoc </center></h2>
 **/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "jee_def.h"
#include "pm_data_cache.h"
#include "pm_adapter_sfs.h"
#include "pm_utils.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

#ifndef MAX_DATA_PATH_LEN
#define MAX_DATA_PATH_LEN SFS_MAX_PATH
#endif // !MAX_DATA_PATH_LEN

/**---------------------------------------------------------------------------*
 **                         Local Variables                                *
 **---------------------------------------------------------------------------*/

LOCAL const wchar *g_char_slash = L"\\";

// data app dir
LOCAL const wchar *g_data_app_dir = JEE_APP_DATA_DIR;

LOCAL const wchar *g_app_cache_dir_name = JEE_APP_LOCAL_CACHE_DIR;
LOCAL const wchar *g_app_data_dir_name = JEE_APP_LOCAL_DATA_DIR;

/**---------------------------------------------------------------------------*
 **                         Local Functions                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

// return data/app/[app_id] dir path
PUBLIC void get_data_app_id_path(char *p_app_id, wchar *output_path)
{
    if ((p_app_id == NULL) || (NULL == output_path))
    {
        return;
    }
    wchar app_id_name[MAX_DATA_PATH_LEN] = {'\0'};

    // data app root dir
    wstrcat(output_path, g_data_app_dir);
    // slash
    if (output_path[wstrlen(output_path) - 1] != *g_char_slash)
    {
        wstrcat(output_path, g_char_slash);
    }
    // app id name
    str_to_wstr((uint8 *)p_app_id, app_id_name);
    wstrcat(output_path, app_id_name);
}

// return data/app/[app_id]/data dir path
PUBLIC void get_app_data_path(char *p_app_id, wchar *output_path)
{
    if ((p_app_id == NULL) || (NULL == output_path))
    {
        return;
    }
    wchar app_path[MAX_DATA_PATH_LEN] = {'\0'};
    get_data_app_id_path(p_app_id, app_path);
    wstrcat(output_path, app_path);
    // slash
    if (app_path[wstrlen(app_path) - 1] != *g_char_slash)
    {
        wstrcat(output_path, g_char_slash);
    }
    wstrcat(output_path, g_app_data_dir_name);
}

// return data/app/[app_id]/cache dir path
PUBLIC void get_app_cache_path(char *p_app_id, wchar *output_path)
{
    if ((p_app_id == NULL) || (NULL == output_path))
    {
        return;
    }
    wchar app_path[MAX_DATA_PATH_LEN] = {'\0'};
    get_data_app_id_path(p_app_id, app_path);
    wstrcat(output_path, app_path);
    // slash
    if (app_path[wstrlen(app_path) - 1] != *g_char_slash)
    {
        wstrcat(output_path, g_char_slash);
    }
    wstrcat(output_path, g_app_cache_dir_name);
}

// data/app/[app_id]/ dir
PUBLIC BOOLEAN create_data_appid_dir(char *p_app_id)
{
    BOOLEAN ret = FALSE;
    // create data dir
    ret = create_data_appid_data_dir(p_app_id);
    SCI_TRACE_LOW("create_data_appid_data_dir ret=%d", ret);
    // create cache dir
    ret = create_data_appid_cache_dir(p_app_id);
    SCI_TRACE_LOW("create_data_app_cache_dir ret=%d", ret);
    return ret;
}

PUBLIC BOOLEAN remove_data_appid_dir(char *p_app_id)
{
    BOOLEAN ret = FALSE;
    if (p_app_id == NULL)
    {
        return FALSE;
    }
    wchar app_id_path[MAX_DATA_PATH_LEN] = {'\0'};
    // app id name
    str_to_wstr((uint8 *)p_app_id, app_id_path);

    // delete data/[app_id] dir
    ret = delete_dir(g_data_app_dir, app_id_path);
    return ret;
}

// data/app/[app_id]/data  dir
PUBLIC BOOLEAN create_data_appid_data_dir(char *p_app_id)
{
    BOOLEAN ret = FALSE;
    if (p_app_id == NULL)
    {
        return FALSE;
    }
    wchar app_data_path[MAX_DATA_PATH_LEN] = {'\0'};

    get_app_data_path(p_app_id, app_data_path);
    // create data dir
    ret = (pm_create_dir(app_data_path) == SFS_NO_ERROR) ? TRUE : FALSE;
    return ret;
}

PUBLIC BOOLEAN remove_data_appid_data_dir(char *p_app_id)
{
    BOOLEAN ret = FALSE;
    if (p_app_id == NULL)
    {
        return FALSE;
    }
    wchar app_root_path[MAX_DATA_PATH_LEN] = {'\0'};

    get_data_app_id_path(p_app_id, app_root_path);
    // delete data dir
    ret = delete_dir(app_root_path, g_app_data_dir_name);
    return ret;
}

// data/app/[app_id]/cache  dir
PUBLIC BOOLEAN create_data_appid_cache_dir(char *p_app_id)
{
    BOOLEAN ret = FALSE;
    if (p_app_id == NULL)
    {
        return FALSE;
    }
    wchar app_cache_path[MAX_DATA_PATH_LEN] = {'\0'};

    get_app_cache_path(p_app_id, app_cache_path);
    // create cache dir
    ret = (pm_create_dir(app_cache_path) == SFS_NO_ERROR) ? TRUE : FALSE;
    return ret;
}
PUBLIC BOOLEAN remove_data_appid_cache_dir(char *p_app_id)
{
    BOOLEAN ret = FALSE;
    if (p_app_id == NULL)
    {
        return FALSE;
    }
    wchar app_root_path[MAX_DATA_PATH_LEN] = {'\0'};

    get_data_app_id_path(p_app_id, app_root_path);
    // delete cache dir
    ret = delete_dir(app_root_path, g_app_cache_dir_name);
    return ret;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
