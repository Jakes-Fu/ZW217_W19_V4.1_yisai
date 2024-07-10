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
 * @file     resourceload.js
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

#include "jee_def.h"
#include "os_api.h"
#include "sfs.h"
#include "sci_types.h"
//#include "pm_list.h"
#include "packagemanager.h"
//#include "pm_internal.h"

#include "resourceload.h"

LOCAL const wchar *g_ctl_folder = L"/A:+NF-F"; // find folder not include file
LOCAL const wchar *g_filter_all = L"*";        // all files
LOCAL const wchar *g_char_slash = L"\\";

#define JS_APP_ROOT_DIR JEE_APP_INSTALL_DIR
#define JS_APP_RESOURCE_NAME L"res\\img" //L"\\res\\img"
#define APP_ID_NAME_MAX (128)
#define APP_PATH_LEN_MAX (256)

#define JS_APP_RESOURE_FILE_PATH_MAX (256)

LOCAL wchar *rl_find_all_files(uint16 *find_path, uint32 find_path_len,
                               list_node *list_head)
{
    SCI_TRACE_LOW(" rl_find_all_files... start! \n");
    SFS_HANDLE file_handle_folder = NULL;
    SFS_FIND_DATA_T *find_data_folder = NULL;

    SFS_ERROR_E result = SFS_NO_ERROR;
    rl_found_files_t *rl_found_file = NULL;

    uint16 *find_path_file = NULL;

    if (PNULL == find_path)
    {
        return NULL;
    }
    // alloc memory
    find_data_folder =
        (SFS_FIND_DATA_T *)SCI_ALLOC_APP(sizeof(SFS_FIND_DATA_T));
    //判断文件夹是否为空
    if (PNULL == find_data_folder)
    {
        return NULL;
    }
    SCI_MEMSET(find_data_folder, 0, sizeof(SFS_FIND_DATA_T));

    // 找到安装目录
    result = SFS_FindFirstFileCtrl(find_path, (uint16 *)g_filter_all,
                                   (uint16 *)g_ctl_folder, &file_handle_folder,
                                   find_data_folder, PNULL);

    if (NULL != file_handle_folder && SFS_NO_ERROR == result)
    {
        do
        {
            rl_found_file =
                (rl_found_files_t *)SCI_ALLOC_APP(sizeof(rl_found_files_t));
            SCI_MEMSET(rl_found_file, 0, sizeof(rl_found_files_t));

            wstrncpy((uint16 *)rl_found_file->found_file,
                     find_data_folder->name, wstrlen(find_data_folder->name));

            listnode_add_tail(&rl_found_file->node, list_head);

        } while (SFS_NO_ERROR
                 == SFS_FindNextFile(file_handle_folder, find_data_folder));

        SFS_FindClose(file_handle_folder);
    }
    //释放内存
    SCI_Free(find_data_folder);
    SCI_TRACE_LOW(" rl_find_all_files... end! \n");
}

// void get_all_resource_dir_info()
list_node *get_all_resource_dir_info()
{
    SCI_TRACE_LOW(" get_all_resource_dir_info  start! \n");
    char appid[APP_ID_NAME_MAX] = {0};
    char apppath[APP_PATH_LEN_MAX] = {0};
    int ret = 0;
    wchar *res_path = NULL;

    list_node *list_head = (list_node *)SCI_ALLOC_APP(sizeof(list_node));
    wchar *l_app_path = (wchar *)SCI_ALLOC_APPZ(sizeof(wchar) * APP_PATH_LEN_MAX);
    wchar *l_app_id = (wchar *)SCI_ALLOC_APP(sizeof(wchar) * APP_ID_NAME_MAX);
    memset(l_app_id, 0, sizeof(wchar) * APP_ID_NAME_MAX);

    if ((list_head == NULL) || (l_app_id == NULL))
    {
        return NULL;
    }

    res_path =
        (wchar *)SCI_ALLOC_APP(sizeof(wchar) * JS_APP_RESOURE_FILE_PATH_MAX);
    memset(res_path, 0, sizeof(wchar) * JS_APP_RESOURE_FILE_PATH_MAX);

    if (res_path == NULL)
    {
        return NULL;
    }
    // get app_path
    ret = js_art_get_app_path(apppath,APP_PATH_LEN_MAX);
    str_to_wstr(apppath, l_app_path);
    wstrcat(res_path, l_app_path);
#if 0
    // get app_id
    ret = jee_get_app_guid(appid, APP_ID_NAME_MAX);

    str_to_wstr(appid, l_app_id);
    // get filename dir
    wstrcat(res_path, JS_APP_ROOT_DIR);
    if (res_path[wstrlen(res_path) - 1] != *g_char_slash)
    {
        wstrcat(res_path, (wchar *)g_char_slash);
    }
    wstrncat(res_path, l_app_id, wstrlen(l_app_id));
#endif
    wstrcat(res_path, JS_APP_RESOURCE_NAME);

    init_listnode_head(list_head);

    rl_find_all_files(res_path, wstrlen(res_path), list_head);
    SCI_TRACE_LOW(" get_all_resource_dir_info... end! \n");
	SCI_FREE(res_path);
	res_path = PNULL;
	SCI_FREE(l_app_id);
	l_app_id = PNULL;
	SCI_FREE(l_app_path);
	l_app_path = PNULL;
    return list_head;
}
