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
 * @file     pm_internal.c
 * @brief    -
 * @author   ningbiao.huang
 * @version  V1.0.0
 * @date
 * @history  2022/5/18 Fix can't delete temp issue.
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

// jee dir define
#include "jee_def.h"

#include "cJSON.h"

#include "pm_adapter_sfs.h"
#include "pm_internal.h"
#include "pm_utils.h"

#include "os_api.h"

#include "priority_system.h"

#include "pm_data_cache.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

#ifndef PM_TRACE_LOW
#define PM_TRACE_LOW(_format, ...)                                             \
    do                                                                         \
    {                                                                          \
        SCI_TRACE_LOW("{%s}[%s]<%d> "_format, "PM", __FUNCTION__, __LINE__,    \
                      ##__VA_ARGS__);                                          \
    } while (0)
#endif

#ifdef WIN32
#define snprintf _snprintf
#endif

//#define strncasecmp(s1, s2, len) CFLCHAR_StrNICmp(s1, s2, len)

#define PM_THREAD_STACK_SIZE (24 * 1024)
#define PM_THREAD_TASK_PRIORITY (PRI_APP + 1)
#define PM_THREAD_QUEUE_NUM 10
#define PM_THREAD_TASK_NAME "T_THREAD_PM"
#define PM_THREAD_QUEUE_NAME "Q_THREAD_PM"

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

LOCAL const wchar *s_packagelist_json_file = L"E:\\app\\package_list.json";
LOCAL const wchar *s_packagelist_backup_json_file =
    L"E:\\app\\package_list_backup.json";

// Dest app dir
LOCAL const wchar *g_device_name_c = L"C";
LOCAL const wchar *g_app_dir = JEE_APP_INSTALL_DIR;
LOCAL const wchar *g_package_dir = JEE_APP_INSTALL_DIR;
LOCAL const wchar *g_app_temp_dir_name = L".temp";

// preload app dir
LOCAL const wchar *g_preload_package_dir = JEE_PRELOAD_APP_DIR;

// app file type
LOCAL const wchar *g_urapk_filter = L"*.urapk";

// File filter
LOCAL const wchar *g_ctl_file = L"/A:+NF-NF";  // find file not include folder;
LOCAL const wchar *g_ctl_folder = L"/A:+NF-F"; // find folder not include file
LOCAL const wchar *g_filter_all = L"*";        // all files
LOCAL const wchar *g_filter_app_json = L"app.json"; // app.json files
LOCAL const wchar *g_char_slash = L"\\";
LOCAL const wchar *g_urapk_suffix = L"urapk";

/**---------------------------------------------------------------------------*
 **                         Local Variables                                *
 **---------------------------------------------------------------------------*/

LOCAL BLOCK_ID g_pm_thread_id = SCI_INVALID_BLOCK_ID;

LOCAL list_node *g_package_list_head = PNULL; /* List of package */
LOCAL SCI_MUTEX_PTR g_pmlistMutex =
    SCI_NULL; /* For exclusive access of global data */
LOCAL const char *MUTEX_NAME = "PACKMANAGER_MUTEX"; /* Name of the mutex */

/* Makesure main function init only once */
LOCAL BOOLEAN g_main_init_flag = 0;

/**---------------------------------------------------------------------------*
 **                         Local Functions define                       *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         LOCAL Functions                                *
 **---------------------------------------------------------------------------*/

LOCAL pm_package_info_t *parse_app_info(uint16 *p_json_path, int path_len);

LOCAL void main_init(void);

LOCAL void scan_dir(wchar *p_dir, int len, BOOLEAN is_file_only);

LOCAL BOOLEAN check_app_exist(char *app_id);

LOCAL BOOLEAN check_app_version_higher(pm_package_info_t *new_package_info);

LOCAL void thread_entry(uint32 argc, void *argv);

LOCAL void threadLoop();

LOCAL int unzip(uint16 *src_file_name_ptr, uint16 *dest_base_dir,
                uint16 *dest_dir_name);

LOCAL int unzip_single_file(wchar *src_zip_file, wchar *decode_file_name,
                            wchar *dst_dir_path);

LOCAL void write_package_info_to_file(pm_package_info_t *p_package_info);

LOCAL int add_package_to_list(pm_package_info_t *p_package_info);

LOCAL int remove_package_from_list(char *p_app_id);

LOCAL void pm_find_json_file(uint16 *find_path, uint32 find_path_len,
                             list_node *list_head);

LOCAL void pm_find_all_files(uint16 *find_path, uint32 find_path_len,
                             list_node *list_head);

LOCAL void pm_find_all_files_filter(uint16 *find_path, uint16 find_path_len,
                                    uint16 *filter_str, uint16 *ctl_str,
                                    list_node *list_head);

LOCAL int pm_find_files_filter_ctrl(uint16 *find_path, uint16 find_path_len,
                                    uint16 *filter_str, uint16 *ctl_str,
                                    list_node *list_head);

LOCAL void pm_parse_all_files(list_node *list_head);

LOCAL void pm_write_packagelist_file(list_node *list_head);

LOCAL cJSON *create_app_json_root_item(pm_package_info_t *p_package_info);

LOCAL int remove_app_dir(char *app_id);

LOCAL int preload_app(uint16 *preload_app_path, uint32 find_path_len);

LOCAL BOOLEAN _acquire_mutex(SCI_MUTEX_PTR mutex);

LOCAL void _release_mutex(SCI_MUTEX_PTR mutex);

LOCAL void info_to_string(pm_package_info_t *p_package_info);

LOCAL pm_package_info_t *parse_app_json_by_file_list(list_node *list_head);

LOCAL BOOLEAN create_package_dir(pm_package_info_t *app_info, uint16 *dst_path,
                                 int *dst_path_len);

LOCAL void clone_package_list_head(list_node *src_list_head,
                                   list_node *dst_list_head);

LOCAL void parse_permission_info(cJSON *object_permissions,
                                 pm_package_info_t *info);

LOCAL void parse_pages_info(cJSON *object_pages, pm_package_info_t *info);

LOCAL void parse_app_version(cJSON *object_app_version,
                             pm_package_info_t *info);

LOCAL void parse_api_version(cJSON *object_api_version,
                             pm_package_info_t *info);

/**---------------------------------------------------------------------------*
 **                         Local Functions                                *
 **---------------------------------------------------------------------------*/
LOCAL __inline BOOLEAN _acquire_mutex(SCI_MUTEX_PTR mutex)
{
    SCI_ASSERT(mutex); /*assert verified*/
    return (SCI_SUCCESS
            == SCI_GetMutex(mutex, SCI_INVALID_BLOCK_ID != SCI_IdentifyThread()
                                       ? SCI_WAIT_FOREVER
                                       : 0));
}

LOCAL __inline void _release_mutex(SCI_MUTEX_PTR mutex)
{
    SCI_ASSERT(SCI_SUCCESS == SCI_PutMutex(mutex)); /*assert verified*/
}

LOCAL void pm_find_json_file(uint16 *find_path, uint32 find_path_len,
                             list_node *list_head)
{
    int ret = -1;
    ret = pm_find_files_filter_ctrl(find_path, find_path_len,
                                    (uint16 *)g_filter_app_json,
                                    (uint16 *)g_ctl_file, list_head);
}

LOCAL int pm_find_files_filter_ctrl(uint16 *find_path, uint16 find_path_len,
                                    uint16 *filter_str, uint16 *ctl_str,
                                    list_node *list_head)
{
    SFS_HANDLE file_handle = NULL;
    SFS_FIND_DATA_T *find_data = NULL;

    SFS_ERROR_E result = SFS_NO_ERROR;
    pm_found_files_t *found_file;

    if (PNULL == find_path)
    {
        return -1;
    }

    if (PNULL == list_head)
    {
        return -1;
    }

    // alloc memory
    find_data = (SFS_FIND_DATA_T *)SCI_ALLOC_APP(sizeof(SFS_FIND_DATA_T));

    if (PNULL == find_data)
    {
        return -1;
    }

    SCI_MEMSET(find_data, 0, sizeof(SFS_FIND_DATA_T)); /*lint !e718 */

    result = SFS_FindFirstFileCtrl(find_path, filter_str, ctl_str, &file_handle,
                                   find_data, PNULL);
    do
    {
        if (NULL != file_handle && SFS_NO_ERROR == result)
        {
            // add to link list
            found_file =
                (pm_found_files_t *)SCI_ALLOC_APP(sizeof(pm_found_files_t));

            found_file->found_file_path_len = SFS_MAX_PATH * 2;
            found_file->found_file =
                (wchar_t *)SCI_ALLOC_APP(found_file->found_file_path_len);
            SCI_MEMSET(found_file->found_file, 0,
                       (size_t)found_file->found_file_path_len);

            wstrcat((uint16 *)found_file->found_file, find_path);
            if (found_file->found_file[wstrlen(found_file->found_file) - 1]
                != *g_char_slash)
            {
                wstrcat((uint16 *)found_file->found_file,
                        (uint16 *)g_char_slash);
            }
            wstrcat((uint16 *)found_file->found_file,
                    (uint16 *)find_data->name);

            found_file->found_file_path_len = wstrlen(found_file->found_file);
            listnode_add_tail(&found_file->node, list_head);
        }
    } while (SFS_NO_ERROR == SFS_FindNextFile(file_handle, find_data));

    if (NULL != file_handle)
    {
        SFS_FindClose(file_handle);
    }
    SCI_FREE(find_data);
    return result;
}

LOCAL void pm_find_all_files(uint16 *find_path, uint32 find_path_len,
                             list_node *list_head)
{
    SFS_HANDLE file_handle = NULL;
    SFS_FIND_DATA_T *find_data = NULL;

    SFS_HANDLE file_handle_floder = NULL;
    SFS_FIND_DATA_T *find_data_floder = NULL;

    SFS_ERROR_E result = SFS_NO_ERROR;
    pm_found_files_t *pm_found_file = NULL;

    uint16 *find_path_file = NULL;

    if (PNULL == find_path)
    {
        return;
    }

    // alloc memory
    find_data = (SFS_FIND_DATA_T *)SCI_ALLOC_APP(sizeof(SFS_FIND_DATA_T));
    if (PNULL == find_data)
    {
        return;
    }
    find_data_floder =
        (SFS_FIND_DATA_T *)SCI_ALLOC_APP(sizeof(SFS_FIND_DATA_T));

    if (PNULL == find_data_floder)
    {
        SCI_FREE(find_data);
        find_data = NULL;
        return;
    }

    SCI_MEMSET(find_data, 0, sizeof(SFS_FIND_DATA_T));
    SCI_MEMSET(find_data_floder, 0, sizeof(SFS_FIND_DATA_T));

    // st search package dir
    result = SFS_FindFirstFileCtrl(find_path, (uint16 *)g_filter_all,
                                   (uint16 *)g_ctl_folder, &file_handle_floder,
                                   find_data_floder, PNULL);

    if (NULL != file_handle_floder && SFS_NO_ERROR == result)
    {
        do
        {
            size_t file_len = MAX_PATH_LENTH * 2;
            find_path_file = (uint16 *)SCI_ALLOC_APP(file_len);
            SCI_MEMSET(find_path_file, 0, file_len);

            wstrcat(find_path_file, find_path);
            if (find_path_file[wstrlen(find_path_file) - 1] != *g_char_slash)
            {
                wstrcat(find_path_file, (uint16 *)g_char_slash);
            }
            wstrcat(find_path_file, find_data_floder->name);

            result = SFS_FindFirstFileCtrl(
                find_path_file, (uint16 *)g_filter_app_json,
                (uint16 *)g_ctl_file, &file_handle, find_data, PNULL);
            if (NULL != file_handle && SFS_NO_ERROR == result)
            {
                uint32 appjson_file_len = MAX_PATH_LENTH * 2;
                // add to list
                pm_found_file =
                    (pm_found_files_t *)SCI_ALLOC_APP(sizeof(pm_found_files_t));
                pm_found_file->found_file_path_len = appjson_file_len;
                pm_found_file->found_file =
                    (wchar_t *)SCI_ALLOC_APP(appjson_file_len);
                SCI_MEMSET(pm_found_file->found_file, 0,
                           appjson_file_len); /*lint !e516 */

                wstrcat((uint16 *)pm_found_file->found_file, find_path_file);
                if (pm_found_file
                        ->found_file[wstrlen(pm_found_file->found_file) - 1]
                    != *g_char_slash)
                {
                    wstrcat((uint16 *)pm_found_file->found_file,
                            (uint16 *)g_char_slash);
                }
                wstrcat((uint16 *)pm_found_file->found_file,
                        (uint16 *)find_data->name);
                listnode_add_tail(&pm_found_file->node, list_head);
            }
            SCI_FREE(find_path_file);
        } while (SFS_NO_ERROR
                 == SFS_FindNextFile(file_handle_floder, find_data_floder));

        if (NULL != file_handle)
        {
            SFS_FindClose(file_handle);
            SCI_FREE(find_data);
        }
        SFS_FindClose(file_handle_floder);
        SCI_Free(find_data_floder);
    }
}

LOCAL void pm_find_all_files_filter(uint16 *find_path, uint16 find_path_len,
                                    uint16 *filter_str, uint16 *ctl_str,
                                    list_node *list_head)
{
    int ret = -1;
    ret = pm_find_files_filter_ctrl(find_path, find_path_len,
                                    (uint16 *)filter_str, (uint16 *)ctl_str,
                                    list_head);
}

LOCAL void pm_parse_all_files(list_node *list_head)
{
    list_node *find_node = PNULL;
    pm_found_files_t *app_json_file = PNULL;
    pm_package_info_t *package_info;

    if (list_head == NULL)
    {
        PM_TRACE_LOW("%s list_head is NULL \n", __FUNCTION__);
        return;
    }
    listnode_for_each(find_node, list_head)
    {
        app_json_file =
            listnode_entry(find_node, pm_found_files_t, node); /*lint !e413 */

        // parse json file
        package_info = parse_app_info((uint16 *)app_json_file->found_file,
                                      app_json_file->found_file_path_len);
        if (package_info)
        {
            // add package list
            add_package_to_list(package_info);
        }
    }
}

LOCAL void pm_write_packagelist_file(list_node *list_head)
{
    list_node *find_node;
    pm_package_info_t *package_info;

    if (list_head == NULL)
    {
        PM_TRACE_LOW("%s list_head is NULL \n", __FUNCTION__);
        return;
    }
    listnode_for_each(find_node, list_head)
    {
        package_info = listnode_entry(find_node, pm_package_info_t,
                                      package_info_node); /*lint !e413 */
        write_package_info_to_file(package_info);
    }
}

LOCAL int remove_app_dir(char *app_id)
{
    int ret = -1;
    uint16 *app_dir_name = NULL;

    if (app_id == NULL)
    {
        return -1;
    }
    PM_TRACE_LOW("%s app_id=%s \n", __FUNCTION__, app_id);
    app_dir_name = (uint16 *)SCI_ALLOC_APP(SFS_MAX_PATH * 2);
    if (app_dir_name == NULL)
    {
        return -1;
    }
    str_to_wstr((uint8 *)app_id, app_dir_name);

    ret = delete_dir((uint16 *)g_app_dir, app_dir_name);
    PM_TRACE_LOW("%s ret=%d \n", __FUNCTION__, ret);

    SCI_FREE(app_dir_name);
    return ret;
}

LOCAL int preload_app(uint16 *preload_app_path, uint32 find_path_len)
{
    int ret = -1;
    list_node *find_node = NULL;
    list_node found_files_head;
    pm_found_files_t *found_file_node = NULL;

    PM_TRACE_LOW("%s enter \n", __FUNCTION__);
    init_listnode_head(&found_files_head);

    // Read all preload urapk
    pm_find_all_files_filter(
        (wchar *)preload_app_path, wstrlen((wchar *)preload_app_path),
        (wchar *)g_urapk_filter, (wchar *)g_ctl_file, &found_files_head);
	
    // Install all urapk
    listnode_for_each(find_node, &found_files_head)
    {
        size_t file_len;

        found_file_node =
            listnode_entry(find_node, pm_found_files_t, node); /*lint !e413 */
        file_len = wstrlen((wchar *)found_file_node->found_file);

        ret = app_install_internal((wchar *)found_file_node->found_file,
                                   file_len);

        // free found file
        if (found_file_node->found_file)
        {
            SCI_FREE(found_file_node->found_file);
            found_file_node->found_file = NULL;
        }
        // release found_file_node
        listnode_del(find_node);
        SCI_FREE(found_file_node);
        found_file_node = NULL;
    }
    return ret;
}

LOCAL int unzip(uint16 *src_file_name_ptr, uint16 *dest_base_dir,
                uint16 *dest_dir_name)
{
    int ret = 0; // 0:pass. other:fail

    // urapk zip-codec
    ret = decode_zip_by_file(src_file_name_ptr, dest_base_dir, dest_dir_name);
    return ret;
}

LOCAL int unzip_single_file(wchar *src_zip_file, wchar *decode_file_name,
                            wchar *dst_dir_path)
{
    int ret = 0; // 0:pass. other:fail

    // unzip one file in zip
    ret =
        decode_single_file_inzip(src_zip_file, decode_file_name, dst_dir_path);
    return ret;
}

LOCAL int add_package_to_list(pm_package_info_t *p_package_info)
{
    int ret = -1;
    list_node *find_node = NULL;
    pm_package_info_t *package_info = NULL;

    PM_TRACE_LOW("%s enter \n", __FUNCTION__);
    if (p_package_info == NULL)
    {
        PM_TRACE_LOW("%s p_package_info is NULL \n", __FUNCTION__);
        return ret;
    }
    if (g_package_list_head == PNULL)
    {
        PM_TRACE_LOW("%s g_package_list_head is NULL \n", __FUNCTION__);
        return ret;
    }
    listnode_for_each(find_node, g_package_list_head)
    {
        package_info =
            listnode_entry(find_node, pm_package_info_t, package_info_node);
        if (strlen(p_package_info->app_id)
                == strlen(package_info->app_id) /*lint !e718 */
            && (strcmp(p_package_info->app_id, package_info->app_id)
                == 0)) /*lint !e718 */
        {
            PM_TRACE_LOW("%s FOUND: %s ,Already add ignore! \n", __FUNCTION__,
                         package_info->app_id);
            return ret;
        }
    }
    listnode_add_tail(&p_package_info->package_info_node, g_package_list_head);
    return ret;
}

LOCAL int remove_package_from_list(char *p_app_id)
{
    int ret = -1;
    list_node *find_node = NULL;
    pm_package_info_t *package_info = NULL;

    if (p_app_id == NULL)
    {
        PM_TRACE_LOW("%s p_app_id is NULL \n", __FUNCTION__);
        return ret;
    }
    PM_TRACE_LOW("%s p_app_id: %s\n", __FUNCTION__, p_app_id);
    if (g_package_list_head == PNULL)
    {
        PM_TRACE_LOW("%s g_package_list_head is NULL \n", __FUNCTION__);
        return ret;
    }

    _acquire_mutex(g_pmlistMutex);
    listnode_for_each(find_node, g_package_list_head)
    {
        size_t str_len1 = 0;
        size_t str_len2 = 0;

        package_info = listnode_entry(find_node, pm_package_info_t,
                                      package_info_node); /*lint !e413 */
        str_len1 = strlen(p_app_id);
        str_len2 = strlen(package_info->app_id);
        if (str_len1 == str_len2
            && (strcmp(p_app_id, package_info->app_id) == 0))
        {
            PM_TRACE_LOW("%s FOUND: %s\n", __FUNCTION__, p_app_id);

            // delete package_info node from linklist
            listnode_del(find_node);

            // release permission buff
            listhead_clean(package_info->p_permisson_list_head,
                           pm_permisson_info_t, permisson_info_node);

            // release pages buff
            listhead_clean(package_info->p_pages_list_head, pm_pages_info_t,
                           page_info_node);

            SCI_FREE(package_info);
            ret = 0;
            break;
        }
    }
    _release_mutex(g_pmlistMutex);
    return ret;
}

LOCAL void write_package_info_to_file(pm_package_info_t *p_package_info)
{
    SFS_HANDLE sfs_fd_ptr;

    // Open exist file.
    sfs_fd_ptr = SFS_CreateFile(
        (uint16 *)s_packagelist_backup_json_file,
        SFS_MODE_OPEN_EXISTING | SFS_MODE_WRITE | SFS_MODE_READ, NULL, NULL);
    if (sfs_fd_ptr == SCI_NULL)
    {
        cJSON *root, *app_array;
        cJSON *app_item;
        char *buff_write = NULL;
        uint32 bytes_written = 0;
        SFS_ERROR_E write_ret = SFS_NO_ERROR;

        // Create new file if file not exist,will replace file if existed.
        sfs_fd_ptr = SFS_CreateFile((uint16 *)s_packagelist_backup_json_file,
                                    SFS_MODE_CREATE_ALWAYS | SFS_MODE_WRITE
                                        | SFS_MODE_READ,
                                    NULL, NULL);
        if (sfs_fd_ptr == SCI_NULL)
        {
            PM_TRACE_LOW("SFS_CreateFile fail!!");
            return;
        }
        // Create packagelist root json object
        root = cJSON_CreateObject();     /*lint !e746 */
        app_array = cJSON_CreateArray(); /*lint !e746 */
        cJSON_AddItemToObject(root, APP_JSON_PACKAGELIST_ROOT,
                              app_array); /*lint !e746 */
        // create new app json item.
        app_item =
            create_app_json_root_item(p_package_info); /*lint !e746 !e64 */
        // cJSON_AddItemToArray(root, app_item);
        cJSON_AddItemToObject(app_array, APP_JSON_ROOT,
                              app_item); /*lint !e746 */
        app_item =
            create_app_json_root_item(p_package_info); /*lint !e746 !e64 */
        // cJSON_AddItemToArray(root, app_item);
        cJSON_AddItemToObject(app_array, APP_JSON_ROOT, app_item);

        // write to file
        buff_write = cJSON_Print(root);
        write_ret = SFS_WriteFile(sfs_fd_ptr, buff_write, strlen(buff_write),
                                  &bytes_written, NULL);
        PM_TRACE_LOW("%s write_ret=%d,bytes_written=%d \n", __FUNCTION__,
                     write_ret, bytes_written);
        SFS_FlushFile(sfs_fd_ptr, NULL);
        SCI_FREE(buff_write);

        // end
        SFS_CloseFile(sfs_fd_ptr);
        cJSON_Delete(root);
    }
    else
    {
        /*
            cJSON* root;
            cJSON* subobj;

            SFS_SetFilePointer(sfs_fd_ptr, 0, SFS_SEEK_BEGIN);
            SFS_GetFileSize(sfs_fd_ptr, &file_size);
            PM_TRACE_LOW("%s file_size:%d\n",__FUNCTION__, file_size);

            json_data = (char *)SCI_ALLOC_APP(file_size);
            SCI_MEMSET(json_data, 0, file_size);

            SCI_ASSERT(json_data != PNULL);

            device_result = SFS_ReadFile(sfs_fd_ptr, (void *)json_data,
           file_size, (uint32 *)&file_size_readed, NULL); PM_TRACE_LOW("%s
           device_result:%d,file_size_readed=%d \n",__FUNCTION__,
           device_result,file_size_readed);

            root = cJSON_Parse(json_data);
            if(root)
            {
                cJSON* item = NULL;
                cJSON* s_subobj = NULL;
                cJSON* item_array = NULL;

                subobj = cJSON_GetObjectItem(root, APP_JSON_ROOT);
                if(subobj)
                {
                    if(cJSON_Array == subobj->type)
                    {

                    }else{

                    }
                }else{

                }
            }
                */
    }
}

LOCAL cJSON *create_app_json_root_item(pm_package_info_t *p_package_info)
{ /*lint !e401 */
    cJSON *root = NULL;
    cJSON *object = NULL;
    cJSON *sub_object = NULL;
    cJSON *permission_array = NULL;
    char*   p_string = PNULL;

    if (p_package_info == SCI_NULL)
    {
        PM_TRACE_LOW("%s p_package_info NULL!! \n", __FUNCTION__);
        return NULL;
    }
    // Add new json object
    root = cJSON_CreateObject();
    // Add object {}
    cJSON_AddItemToObject(root, APP_JSON_ROOT, object = cJSON_CreateObject());
    // Add id
    cJSON_AddItemToObject(object, APP_JSON_ID,
                          cJSON_CreateString("com.unisoc.app1"));
    // Add name
    cJSON_AddItemToObject(object, APP_JSON_NAME, cJSON_CreateString("app1234"));
    // 5. Add vendor
    cJSON_AddItemToObject(object, APP_JSON_VENDOR,
                          cJSON_CreateString("UNISOC"));
    // 6. Add version object
    cJSON_AddItemToObject(object, APP_JSON_VERSION,
                          sub_object = cJSON_CreateObject());
    // 6.1 Add version code
    cJSON_AddItemToObject(sub_object, APP_JSON_VERSION_CODE,
                          cJSON_CreateNumber(21.0));
    // 6.2 Add version name
    cJSON_AddItemToObject(sub_object, APP_JSON_VERSION_NAME,
                          cJSON_CreateString("v2.0"));
    // 7. Add api version object
    cJSON_AddItemToObject(object, APP_JSON_APIVERSION,
                          sub_object = cJSON_CreateObject());
    // 7.1 Add api version compatible
    cJSON_AddItemToObject(sub_object, APP_JSON_APIVERSION_COMPATIBLE,
                          cJSON_CreateString("3.4"));
    // 7.2 Add api version target
    cJSON_AddItemToObject(sub_object, APP_JSON_APIVERSION_TARGET,
                          cJSON_CreateString("5.0"));
    // 8. Add icon
    cJSON_AddItemToObject(object, APP_JSON_ICON, cJSON_CreateString("icon"));
    // 9. Add permisson array
    cJSON_AddItemToObject(object, APP_JSON_PERMISSONS,
                          permission_array = cJSON_CreateArray());
    // 9.1 Add permisson1
    cJSON_AddItemToObject(permission_array, "\r\n",
                          sub_object = cJSON_CreateObject());
    cJSON_AddItemToObject(sub_object, APP_JSON_PERMISSON_NAME,
                          cJSON_CreateString("CAMERA"));
    // 9.2 Add permisson2
    cJSON_AddItemToObject(permission_array, "\r\n",
                          sub_object = cJSON_CreateObject());
    cJSON_AddItemToObject(sub_object, APP_JSON_PERMISSON_NAME,
                          cJSON_CreateString("CALL"));

    p_string = cJSON_Print(root);
    PM_TRACE_LOW("root_item:\n %s\n", p_string);
    SCI_FREE(p_string);

    return root;
}

/*****************************************************************************/
//  Description: threadLoop for install/uninstall work.
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void threadLoop()
{
    xSignalHeader sig_ptr = SCI_NULL;

    BLOCK_ID thread_id = SCI_IdentifyThread();
    PM_TRACE_LOW("entry thread_id=%d! \n", thread_id);

    for (;;)
    {
        int ret = -1;
        sig_ptr = SCI_GetSignal(thread_id);
        SCI_ASSERT(SCI_NULL != sig_ptr); /*assert verified*/

        PM_TRACE_LOW("%s SignalCode=%d \n", __FUNCTION__, sig_ptr->SignalCode);
        switch (sig_ptr->SignalCode)
        {
        case PM_SIG_INSTALL_APP:
            ret = app_install_internal(
                ((pm_install_sign_t *)sig_ptr)->app_path,
                ((pm_install_sign_t *)sig_ptr)->app_path_len);
            PM_TRACE_LOW("%s app_install_internal ret=%d \n", __FUNCTION__,
                         ret);
            break;
        case PM_SIG_UNINSTALL_APP:
            PM_TRACE_LOW("%s app_id=%s \n", __FUNCTION__,
                         ((pm_uninstall_sign_t *)sig_ptr)->app_id);
            ret = app_uninstall_internal(
                ((pm_uninstall_sign_t *)sig_ptr)->app_id);
            PM_TRACE_LOW("%s app_uninstall_internal ret=%d \n", __FUNCTION__,
                         ret);
            break;
        case PM_SIG_INIT:
            main_init();
            break;
        default:
            break;
        }
        SCI_FREE(sig_ptr);
    }
}

LOCAL void thread_entry(uint32 argc, void *argv)
{
    threadLoop();
}

/*****************************************************************************/
//  Description: scan all JS app dir to parse package info
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void scan_dir(wchar *p_dir, int len, BOOLEAN is_file_only)
{
    /* List of json files */
    list_node *appjson_files_head = PNULL;

    if (p_dir == PNULL)
    {
        return;
    }

    appjson_files_head = (list_node *)SCI_ALLOC_APP(sizeof(list_node));
    init_listnode_head(appjson_files_head);
    // find all app.json file
    if (is_file_only)
    {
        pm_find_json_file((uint16 *)p_dir, wstrlen(p_dir), appjson_files_head);
    }
    else
    {
        pm_find_all_files((uint16 *)p_dir, wstrlen(p_dir), appjson_files_head);
    }

    // parse all app.json file
    pm_parse_all_files(appjson_files_head);
    // write all package info to packagelist.json
    // pm_write_packagelist_file(g_package_list_head);

    // release
    if (appjson_files_head)
    {
        list_node *find_node = NULL;
        pm_found_files_t *found_file_node = NULL;
        listnode_for_each(find_node, appjson_files_head)
        {
            found_file_node = listnode_entry(find_node, pm_found_files_t,
                                             node); /*lint !e413 */

            // free found file
            if (found_file_node->found_file)
            {
                SCI_FREE(found_file_node->found_file);
                found_file_node->found_file = NULL;
            }
        }
        listhead_clean(appjson_files_head, pm_found_files_t, node);
    }
}

LOCAL BOOLEAN check_app_exist(char *app_id)
{
    list_node *find_node = NULL;
    pm_package_info_t *info = NULL;

    if (app_id == NULL)
    {
        PM_TRACE_LOW("%s app_id is NULL \n", __FUNCTION__);
        return FALSE;
    }
    if (g_package_list_head == PNULL)
    {
        PM_TRACE_LOW("%s g_package_list_head is NULL \n", __FUNCTION__);
        return FALSE;
    }

    _acquire_mutex(g_pmlistMutex);
    PM_TRACE_LOW("%s app_id: %s\n", __FUNCTION__, app_id);
    listnode_for_each(find_node, g_package_list_head)
    {
        info = listnode_entry(find_node, pm_package_info_t,
                              package_info_node); /*lint !e413 */
        if (strlen(app_id) == strlen(info->app_id)
            && (strcmp(app_id, info->app_id) == 0))
        {
            PM_TRACE_LOW("%s FOUND: %s\n", __FUNCTION__, app_id);
            _release_mutex(g_pmlistMutex);
            return TRUE;
        }
    }
    _release_mutex(g_pmlistMutex);
    return FALSE;
}

LOCAL BOOLEAN check_app_version_higher(pm_package_info_t *new_package_info)
{
    list_node *find_node = NULL;
    pm_package_info_t *info = NULL;

    if (new_package_info == NULL)
    {
        PM_TRACE_LOW("%s new_package_info is NULL \n", __FUNCTION__);
        return FALSE;
    }
    if (g_package_list_head == PNULL)
    {
        PM_TRACE_LOW("%s g_package_list_head is NULL \n", __FUNCTION__);
        return FALSE;
    }

    _acquire_mutex(g_pmlistMutex);
    listnode_for_each(find_node, g_package_list_head)
    {
        info = listnode_entry(find_node, pm_package_info_t,
                              package_info_node); /*lint !e413 */
        if (strlen(new_package_info->app_id) == strlen(info->app_id)
            && (strcmp(new_package_info->app_id, info->app_id) == 0))
        {
            PM_TRACE_LOW("%s FOUND: %s\n", __FUNCTION__,
                         new_package_info->app_id);
            // Only higher version pass
            if (new_package_info->app_version_code > info->app_version_code)
            {
                _release_mutex(g_pmlistMutex);
                return TRUE;
            }
        }
    }
    _release_mutex(g_pmlistMutex);
    return FALSE;
}

LOCAL pm_package_info_t *parse_app_json_by_file_list(list_node *list_head)
{
    list_node *find_node = NULL;
    pm_package_info_t *app_info = NULL;

    if (list_head == NULL)
    {
        return NULL;
    }
    listnode_for_each(find_node, list_head)
    {
        uint16 file_name[MAX_PATH_LENTH] = {0};
        uint16 temp_file[MAX_PATH_LENTH] = {0};
        pm_found_files_t *file_block_node = NULL;

        // func
        file_block_node =
            listnode_entry(find_node, pm_found_files_t, node); /*lint !e413 */

        wstrncpy(temp_file, (uint16 *)file_block_node->found_file,
                 wstrlen((uint16 *)file_block_node->found_file));
        split_path_file_name(temp_file, file_name);
        if (wstrncmp(file_name, (wchar *)g_filter_app_json,
                     wstrlen((wchar *)g_filter_app_json))
            == 0)
        {
            app_info = parse_app_info((uint16 *)file_block_node->found_file,
                                      file_block_node->found_file_path_len);
            return app_info;
        }
    }
    return NULL;
}

LOCAL BOOLEAN create_package_dir(pm_package_info_t *app_info, uint16 *dst_path,
                                 int *dst_path_len)
{
    BOOLEAN ret = FALSE;

    if (app_info)
    {
        SFS_ERROR_E errCode;
        wchar package_path[MAX_NAME_LENTH] = {0};
        char package_name[MAX_NAME_LENTH] = {0};

        str_to_wstr((uint8 *)app_info->app_id, (uint16 *)package_name);
        PM_TRACE_LOW("%s package_name :%s,app_info->app_id=%s \n", __FUNCTION__,
                     package_name, app_info->app_id);
        // Create \\app\package_name dir
        wstrcat(package_path, (wchar *)g_app_dir);
        // slash
        if (package_path[wstrlen(package_path) - 1] != *g_char_slash)
        {
            wstrcat(package_path, (wchar *)g_char_slash);
        }
        wstrcat(package_path, (uint16 *)package_name);

        // Dst dir still exist, delete dir here
        if (pm_is_file_exist(package_path, wstrlen(package_path), FALSE))
        {
            // Delete all file in \app\{packagename}
            remove_app_dir(app_info->app_id);
        }

        errCode = SFS_CreateDirectory((uint16 *)package_path);
        PM_TRACE_LOW("%s package_path errCode :%d \n", __FUNCTION__, errCode);
        if (errCode == SFS_NO_ERROR || errCode == SFS_ERROR_HAS_EXIST)
        {
            // For save case
            if (errCode == SFS_ERROR_HAS_EXIST)
            {
                // Delete all file in \app\{packagename}
                remove_app_dir(app_info->app_id);
            }
            ret = TRUE;
            wstrncpy(dst_path, package_path, wstrlen(package_path));
            *dst_path_len = wstrlen(package_path);
        }
    }
    return ret;
}

/*****************************************************************************/
//  Description: Install JS APP
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC int app_install_internal(wchar *p_path, int len)
{
    int ret = -1;
    uint16 dst_path[MAX_PATH_LENTH] = {0};
    uint16 temp_app_path[MAX_PATH_LENTH] = {0};
    int dst_len = 0;

    BOOLEAN is_create_package_dir = FALSE;
    BOOLEAN is_app_exist = FALSE;
    BOOLEAN is_app_version_higher = FALSE;
    pm_package_info_t *package_info = NULL;
    list_node *find_node = NULL;
    BOOLEAN del_ret = FALSE;
    list_node block_file_list_head;

    if (g_package_list_head == PNULL)
    {
        return PM_INSTALL_INIT_ERROR;
    }
    if (p_path == PNULL)
    {
        return PM_INSTALL_PARAM_ERROR;
    }

    // check path string is ASCII.
    if (!MMIAPICOM_IsASCIIString(p_path, wstrlen(p_path)))
    {
        PM_TRACE_LOW("PM_INSTALL_PARAM_ERROR:p_path=%ls \n", p_path);
        return PM_INSTALL_PARAM_ERROR;
    }

    if (!check_file_suffix(p_path, (uint16 *)g_urapk_suffix))
    {
        PM_TRACE_LOW("PM_INSTALL_SUFFIX_ERROR \n");
        return PM_INSTALL_SUFFIX_ERROR;
    }

    if (!check_device_enough_space(p_path, (uint16 *)g_device_name_c))
    {
        PM_TRACE_LOW("PM_INSTALL_NO_ENOUGH_SPACE_ERROR \n");
        return PM_INSTALL_NO_ENOUGH_SPACE_ERROR;
    }

    // Delete all file in \package_temp
    // package_temp dir still exist, delete dir here
    if (pm_is_file_exist(g_package_dir, wstrlen(g_package_dir), FALSE))
    {
        del_ret =
            delete_dir((uint16 *)g_package_dir, (uint16 *)g_app_temp_dir_name);
        PM_TRACE_LOW("%s still del_ret=%d \n", __FUNCTION__, del_ret);
    }

    pm_path_combine((uint16 *)g_package_dir, (uint16 *)g_app_temp_dir_name,
                    temp_app_path);

    // For performance unzip app.json only.
    ret = unzip_single_file(p_path, g_filter_app_json, temp_app_path);

    if (ret != 0)
    {
        PM_TRACE_LOW("unzip fail! \n");
        return PM_INSTALL_UNZIP_ERROR;
    }

    init_listnode_head(&block_file_list_head);
    // Read all app.json to list g_block_file_list_head
    pm_find_json_file((wchar *)temp_app_path, wstrlen((wchar *)temp_app_path),
                      &block_file_list_head);

    // parse_app_json
    package_info = parse_app_json_by_file_list(&block_file_list_head);

    if (package_info == NULL)
    {
        PM_TRACE_LOW("parse_app_json fail! \n");
        ret = PM_INSTALL_PARSE_JSON_ERROR;
        goto exit;
    }

    // Check app signature certificate
    // Check app is running
    // Check is app exist
    is_app_exist = check_app_exist(package_info->app_id);
    if (is_app_exist)
    {
        // 5.Check is update app
        is_app_version_higher = check_app_version_higher(package_info);
        if (is_app_version_higher)
        {
            // Update app , Uninstall app first
            app_uninstall_internal(package_info->app_id);
        }
        else
        {
            // App version is lower,ignore this install work
            ret = PM_INSTALL_VERSION_LOWER_ERROR;
            goto exit;
        }
    }

    // 6.Create package dir
    is_create_package_dir =
        create_package_dir(package_info, dst_path, &dst_len);
    if (is_create_package_dir)
    {
        wchar package_name[MAX_NAME_LENTH] = {0};
        str_to_wstr((uint8 *)package_info->app_id, (uint16 *)package_name);
        // unzip all file to app_id dir
        ret = unzip((uint16 *)p_path, (uint16 *)g_package_dir, package_name);
        if (ret != 0)
        {
            PM_TRACE_LOW("unzip all file fail! \n");
            ret = PM_INSTALL_UNZIP_ERROR;
            goto exit;
        }
        /*
        PM_TRACE_LOW(" pm_copy_tree ! \n");
        // move temp file to package dir
        pm_copy_tree(dst_path, temp_app_path);
        */

        // create app data/cache dir
        create_data_appid_dir(package_info->app_id);
    }
    else
    {
        PM_TRACE_LOW("%s is_create_package_dir fail! \n", __FUNCTION__);
    }
    // Add to package link list
    scan_dir((wchar *)dst_path, wstrlen((wchar *)dst_path), TRUE);
    // Return ret ok
    ret = PM_INSTALL_OK;

exit:
    listnode_for_each(find_node, &block_file_list_head)
    {
        pm_found_files_t *file_block_node = NULL;
        file_block_node =
            listnode_entry(find_node, pm_found_files_t, node); /*lint !e413 */

        // free found file
        if (file_block_node->found_file)
        {
            SCI_FREE(file_block_node->found_file);
            file_block_node->found_file = NULL;
        }

        // delete link list node
        listnode_del(find_node);
        SCI_FREE(file_block_node);
        file_block_node = NULL;
    }
    // Delete all file in \.temp
    del_ret =
        delete_dir((uint16 *)g_package_dir, (uint16 *)g_app_temp_dir_name);
    PM_TRACE_LOW("%s del_ret=%d \n", __FUNCTION__, del_ret);

    SCI_FREE(package_info);
    package_info = NULL;
    return ret;
}

/*****************************************************************************/
//  Description: Uninstall JS APP
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC int app_uninstall_internal(char *app_id)
{
    int ret = -1;

    if (g_package_list_head == PNULL)
    {
        return PM_UNINSTALL_INIT_ERROR;
    }
    if (app_id == PNULL)
    {
        return PM_UNINSTALL_PARAM_ERROR;
    }

    // remove info from link list
    ret = remove_package_from_list(app_id);
    // remove app dir
    ret = remove_app_dir(app_id);
    // remove app data/cache dir
    remove_data_appid_dir(app_id);

    ret = PM_UNINSTALL_OK;

    return ret;
}

LOCAL void main_init(void)
{
    PM_TRACE_LOW("[main_init:Initialize] IN()\r\n");

    if (g_main_init_flag == 1)
    {
        PM_TRACE_LOW("[main_init:Initialize Already done,ignore!] IN()\r\n");
        return;
    }

    do
    {
        g_pmlistMutex = SCI_CreateMutex(MUTEX_NAME, SCI_INHERIT);
        if (SCI_NULL == g_pmlistMutex)
        {
            PM_TRACE_LOW("[main_init:Initialize] Create Mutex failed.\r\n");
            break;
        }
    } while (0);

    _acquire_mutex(g_pmlistMutex);
    // just in case
    if (g_package_list_head)
    {
        SCI_FREE(g_package_list_head);
        g_package_list_head = NULL;
    }
    g_package_list_head = (list_node *)SCI_ALLOC_APP(sizeof(list_node));
    init_listnode_head(g_package_list_head);
    // Load all installed app
    scan_dir((wchar *)g_app_dir, wstrlen((wchar *)g_app_dir), FALSE);
    // Load all preinstalled app
    scan_dir((wchar *)g_preload_package_dir, wstrlen((wchar *)g_preload_package_dir), FALSE);

    // preload all app .urapk file
    preload_app((uint16 *)g_preload_package_dir,
                wstrlen((uint16 *)g_preload_package_dir));

    g_main_init_flag = 1;
    _release_mutex(g_pmlistMutex);
    PM_TRACE_LOW("[main_init:Initialize] OUT()\r\n");
}

LOCAL void info_to_string(pm_package_info_t *package_info)
{
    list_node *find_node_perm = NULL;
    pm_permisson_info_t *perm_info = NULL;
    pm_pages_info_t *page_info = NULL;

    if (package_info == NULL)
    {
        return;
    }
    PM_TRACE_LOW("***------ APP ------*** \n");
    PM_TRACE_LOW("app_id=%s:\n", package_info->app_id);
    PM_TRACE_LOW("app_name=%s,vendor_name=%s \n", package_info->app_name,
                 package_info->vendor_name);
    PM_TRACE_LOW("parameter=%s \n", package_info->parameter);
    PM_TRACE_LOW("app_version_code=%d, "
                 "app_version_name=%s,api_version_compatible=%s,api_version_"
                 "target=%s \n",
                 package_info->app_version_code, package_info->app_version_name,
                 package_info->api_version_compatible,
                 package_info->api_version_target);
    PM_TRACE_LOW("app_icon=%s, app_install_path=%s \n", package_info->app_icon,
                 package_info->app_install_path);

    if (package_info->p_permisson_list_head != NULL)
    {
        PM_TRACE_LOW(" **--- permissons ---**\n");
        listnode_for_each(find_node_perm, package_info->p_permisson_list_head)
        {
            perm_info = listnode_entry(find_node_perm, pm_permisson_info_t,
                                       permisson_info_node); /*lint !e413 */
            PM_TRACE_LOW("permisson_name=%s \n", perm_info->permisson_name);
        }
    }

    // page list
    if (package_info->p_pages_list_head != NULL)
    {
        find_node_perm = NULL;
        PM_TRACE_LOW(" **--- pages ---** \n");
        listnode_for_each(find_node_perm, package_info->p_pages_list_head)
        {
            page_info = listnode_entry(find_node_perm, pm_pages_info_t,
                                       page_info_node); /*lint !e413 */
            PM_TRACE_LOW("page_name=%s \n", page_info->page_name);
        }
    }
    PM_TRACE_LOW("***------ APP END ------*** \n");
}

LOCAL pm_package_info_t *parse_app_info(uint16 *p_json_path, int path_len)
{
    pm_package_info_t *info = NULL;
    char *json_data = NULL;
    uint32 file_size = 0;
    uint32 file_size_readed = 0;
    SFS_HANDLE sfs_fd_ptr;
    cJSON *root = NULL;
    cJSON *subobj = NULL;
    SFS_ERROR_E device_result = SFS_ERROR_NO_SPACE;

    sfs_fd_ptr = SFS_CreateFile(
        p_json_path, SFS_MODE_OPEN_EXISTING | SFS_MODE_WRITE | SFS_MODE_READ,
        NULL, NULL);

    if (sfs_fd_ptr == SCI_NULL)
    {
        PM_TRACE_LOW(" SFS_CreateFile FAIL!\n");
        return NULL;
    }
    else
    {
        SFS_SetFilePointer(sfs_fd_ptr, (int64)0, SFS_SEEK_BEGIN);
        SFS_GetFileSize(sfs_fd_ptr, &file_size);

        json_data = (char *)SCI_ALLOC_APP(file_size);
        SCI_ASSERT(json_data != PNULL); /*assert verified*/
        SCI_MEMSET(json_data, 0, (size_t)file_size);

        device_result = SFS_ReadFile(sfs_fd_ptr, (void *)json_data, file_size,
                                     (uint32 *)&file_size_readed, NULL);

        root = cJSON_Parse(json_data);
        if (root)
        {
            subobj = cJSON_GetObjectItem(root, APP_JSON_ROOT);
            if (subobj)
            {
                cJSON *item = NULL;
                cJSON *s_subobj = NULL;

                info = (pm_package_info_t *)SCI_ALLOC_APP(
                    sizeof(pm_package_info_t));
                SCI_ASSERT(info != PNULL); /*assert verified*/
                memset(info, 0, sizeof(pm_package_info_t));

                // parse app id
                item = cJSON_GetObjectItem(subobj, APP_JSON_ID);
                if (item == NULL)
                {
                    PM_TRACE_LOW("app_id is NULL, it's not a cpmplete APP!");
                    SCI_FREE(json_data);
                    SFS_CloseFile(sfs_fd_ptr);
                    cJSON_Delete(root);
                    return NULL;
                }

                strcpy(info->app_id, item->valuestring); /*lint !e718 */

                // parse app name
                item = cJSON_GetObjectItem(subobj, APP_JSON_NAME);
                if (item != NULL)
                {
                    strcpy(info->app_name, item->valuestring);
                }

                // parse vendor name
                item = cJSON_GetObjectItem(subobj, APP_JSON_VENDOR);
                if (item != NULL)
                {
                    strcpy(info->vendor_name, item->valuestring);
                }

                // parse parameter name
                item = cJSON_GetObjectItem(subobj, APP_JSON_PARAMTER);
                if (item != NULL)
                {
                    strcpy(info->parameter, item->valuestring);
                }

                // parse js app version
                s_subobj = cJSON_GetObjectItem(subobj, APP_JSON_VERSION);
                parse_app_version(s_subobj, info);

                // parse js api version
                s_subobj = cJSON_GetObjectItem(subobj, APP_JSON_APIVERSION);
                parse_api_version(s_subobj, info);

                // parse app icon
                item = cJSON_GetObjectItem(subobj, APP_JSON_ICON);
                if (item != NULL)
                {
                    strcpy(info->app_icon, item->valuestring);
                }

                // install path
                wstrn_to_str(p_json_path, (uint8 *)info->app_install_path,
                             (wstrlen(p_json_path)
                              - wstrlen((wchar *)g_filter_app_json)));

                // parse permission
                s_subobj = cJSON_GetObjectItem(subobj, APP_JSON_PERMISSONS);
                parse_permission_info(s_subobj, info);

                // parse pages
                s_subobj = cJSON_GetObjectItem(subobj, APP_JSON_PAGES);
                parse_pages_info(s_subobj, info);
            }

            // free in final
            cJSON_Delete(root);
        }
        SCI_FREE(json_data);
        SFS_CloseFile(sfs_fd_ptr);
    }
    return info;
}

LOCAL void parse_app_version(cJSON *object_app_version, pm_package_info_t *info)
{
    cJSON *item = NULL;

    if (object_app_version != NULL)
    {
        item = cJSON_GetObjectItem(object_app_version, APP_JSON_VERSION_CODE);
        if (item != NULL)
        {
            info->app_version_code = item->valueint;
        }

        item = cJSON_GetObjectItem(object_app_version, APP_JSON_VERSION_NAME);
        if (item != NULL)
        {
            strcpy(info->app_version_name, item->valuestring);
        }
    }
}

LOCAL void parse_api_version(cJSON *object_api_version, pm_package_info_t *info)
{
    cJSON *item = NULL;

    if (object_api_version != NULL)
    {
        item = cJSON_GetObjectItem(object_api_version,
                                   APP_JSON_APIVERSION_COMPATIBLE);
        if (item != NULL)
        {
            strcpy(info->api_version_compatible, item->valuestring);
        }

        item =
            cJSON_GetObjectItem(object_api_version, APP_JSON_APIVERSION_TARGET);
        if (item != NULL)
        {
            strcpy(info->api_version_target, item->valuestring);
        }
    }
}

LOCAL void parse_permission_info(cJSON *object_permissions,
                                 pm_package_info_t *info)
{
    cJSON *item = NULL;
    cJSON *item_array = NULL;
    pm_permisson_info_t *perm_info = NULL;
    int i = 0;

    if (object_permissions == NULL)
    {
        return;
    }
    info->p_permisson_list_head = (list_node *)SCI_ALLOC_APP(sizeof(list_node));
    init_listnode_head(info->p_permisson_list_head);
    if (cJSON_Array == object_permissions->type)
    {
        for (i = 0; i < cJSON_GetArraySize(object_permissions); i++)
        {
            item = cJSON_GetArrayItem(object_permissions, i);
            if (item != NULL)
            {
                item_array = cJSON_GetObjectItem(item, APP_JSON_PERMISSON_NAME);
                if (item_array != NULL)
                {
                    perm_info = (pm_permisson_info_t *)SCI_ALLOC_APP(
                        sizeof(pm_permisson_info_t));

                    strcpy(perm_info->permisson_name, item_array->valuestring);
                    listnode_add_tail(&perm_info->permisson_info_node,
                                      info->p_permisson_list_head);
                }
            }
        }
    }
    else
    {
        item = cJSON_GetObjectItem(object_permissions, APP_JSON_PERMISSON_NAME);
        if (item != NULL)
        {
            perm_info = (pm_permisson_info_t *)SCI_ALLOC_APP(
                sizeof(pm_permisson_info_t));

            strcpy(perm_info->permisson_name, item->valuestring);
            listnode_add_tail(&perm_info->permisson_info_node,
                              info->p_permisson_list_head);
        }
    }
}

// page list
LOCAL void parse_pages_info(cJSON *object_pages, pm_package_info_t *info)
{
    cJSON *item = NULL;
    pm_pages_info_t *pages_info = NULL;
    int i = 0;

    if (object_pages == NULL)
    {
        return;
    }
    info->p_pages_list_head = (list_node *)SCI_ALLOC_APP(sizeof(list_node));
    init_listnode_head(info->p_pages_list_head);
    if (cJSON_Array == object_pages->type)
    {
        for (i = 0; i < cJSON_GetArraySize(object_pages); i++)
        {
            item = cJSON_GetArrayItem(object_pages, i);
            if (item != NULL)
            {
                pages_info =
                    (pm_pages_info_t *)SCI_ALLOC_APP(sizeof(pm_pages_info_t));

                strcpy(pages_info->page_name, item->valuestring);
                listnode_add_tail(&pages_info->page_info_node,
                                  info->p_pages_list_head);
            }
        }
    }
}

LOCAL void clone_package_list_head(list_node *src_list_head,
                                   list_node *dst_list_head)
{
    list_node *find_node = NULL;

    if (src_list_head == NULL)
    {
        PM_TRACE_LOW("src_list_head is NULL \n");
        return;
    }
    if (dst_list_head == NULL)
    {
        PM_TRACE_LOW("src_list_head is NULL \n");
        return;
    }

    listnode_for_each(find_node, src_list_head)
    {
        pm_package_info_t *src_info =
            listnode_entry(find_node, pm_package_info_t, package_info_node);
        if (src_info == NULL)
        {
            continue;
        }
        pm_package_info_t *dst_info =
            (pm_package_info_t *)SCI_ALLOC_APP(sizeof(pm_package_info_t));
        if (dst_info == NULL)
        {
            continue;
        }

        // clone pm_package_info_t
        SCI_MEMCPY((void *)dst_info, src_info,
                   sizeof(pm_package_info_t)); /*lint !e718 -e628*/

        // clone permission list
        if (src_info->p_permisson_list_head != NULL)
        {
            list_node *perm_list_head =
                (list_node *)SCI_ALLOC_APP(sizeof(list_node));

            listhead_clone(src_info->p_permisson_list_head, perm_list_head,
                           pm_permisson_info_t, permisson_info_node);
            dst_info->p_permisson_list_head = perm_list_head;
        }

        // clone page list
        if (src_info->p_pages_list_head != NULL)
        {
            list_node *page_list_head =
                (list_node *)SCI_ALLOC_APP(sizeof(list_node));

            listhead_clone(src_info->p_pages_list_head, page_list_head,
                           pm_pages_info_t, page_info_node);
            dst_info->p_pages_list_head = page_list_head;
        }

        // add to dst head
        listnode_add_tail(&dst_info->package_info_node, dst_list_head);
    }
}

/**---------------------------------------------------------------------------*
 **                         External Variables                                *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

PUBLIC void pm_dump_internal(void)
{
    list_node *find_node = NULL;
    pm_package_info_t *package_info = NULL;

    if (g_package_list_head == PNULL)
    {
        return;
    }
    if (listnode_empty(g_package_list_head))
    {
        PM_TRACE_LOW("App list is empty!\n");
        return;
    }
    PM_TRACE_LOW("\n "
                 "************************************************\n");
    _acquire_mutex(g_pmlistMutex);
    listnode_for_each(find_node, g_package_list_head)
    {
        package_info = listnode_entry(find_node, pm_package_info_t,
                                      package_info_node); /*lint !e413 */
        info_to_string(package_info);
    }
    _release_mutex(g_pmlistMutex);
    PM_TRACE_LOW("\n "
                 "*************************************************\n");
}

/*****************************************************************************/
//  Description: Parse JS app info from app.json
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void create_packagemanager_thread(void)
{
    uint32 status = SCI_IsThreadExist(g_pm_thread_id);
    if (SCI_SUCCESS != status)
    {
        // SCI_CreateThread/SCI_CreateAppThread
        g_pm_thread_id = SCI_CreateThread(
            PM_THREAD_TASK_NAME, PM_THREAD_QUEUE_NAME, thread_entry, 0, NULL,
            PM_THREAD_STACK_SIZE, PM_THREAD_QUEUE_NUM, PM_THREAD_TASK_PRIORITY,
            SCI_PREEMPT, SCI_AUTO_START);
        SCI_ASSERT(SCI_INVALID_BLOCK_ID != g_pm_thread_id
                   && 0 != g_pm_thread_id);
    }
    else
    {
        PM_TRACE_LOW("Thread exist g_pm_thread_id:%d \n", g_pm_thread_id);
    }
}

int veryfySig()
{
    int ret = -1;
    return ret;
}

PUBLIC int get_app_info_internal(char *app_id, pm_package_info_t *output_info)
{
    list_node *find_node = NULL;
    pm_package_info_t *info = NULL;

    if (app_id == NULL)
    {
        PM_TRACE_LOW("%s app_id is NULL \n", __FUNCTION__);
        return -1;
    }
    if (output_info == NULL)
    {
        PM_TRACE_LOW("%s output is NULL \n", __FUNCTION__);
        return -1;
    }
    if (g_package_list_head == PNULL)
    {
        PM_TRACE_LOW("%s g_package_list_head is NULL \n", __FUNCTION__);
        return -1;
    }

    _acquire_mutex(g_pmlistMutex);
    PM_TRACE_LOW("%s app_id: %s\n", __FUNCTION__, app_id);
    listnode_for_each(find_node, g_package_list_head)
    {
        info = listnode_entry(find_node, pm_package_info_t,
                              package_info_node); /*lint !e413 */
		PM_TRACE_LOW("%s app_path: %s \n", __FUNCTION__, info->app_install_path);
		PM_TRACE_LOW("%s app_name: %s \n", __FUNCTION__, info->app_name);
		PM_TRACE_LOW("%s app_id: %s \n", __FUNCTION__, info->app_id);

        if (strlen(app_id) == strlen(info->app_id)
            && (strcmp(app_id, info->app_id) == 0))
        {
            PM_TRACE_LOW("%s FOUND: %s\n", __FUNCTION__, app_id);
            // Set output buff
            SCI_MEMCPY((void *)output_info, info, sizeof(pm_package_info_t));

            if (info->p_permisson_list_head != NULL)
            {
                // Set new permission listnode head
                list_node *perm_list_node_head =
                    (list_node *)SCI_ALLOC_APP(sizeof(list_node));

                listhead_clone(info->p_permisson_list_head, perm_list_node_head,
                               pm_permisson_info_t, permisson_info_node);
                output_info->p_permisson_list_head = perm_list_node_head;
            }

            // clone page list
            if (info->p_pages_list_head != NULL)
            {
                list_node *page_list_head =
                    (list_node *)SCI_ALLOC_APP(sizeof(list_node));

                listhead_clone(info->p_pages_list_head, page_list_head,
                               pm_pages_info_t, page_info_node);
                output_info->p_pages_list_head = page_list_head;
            }

            _release_mutex(g_pmlistMutex);
            return 0;
        }
    }
	PM_TRACE_LOW("%s end_app_id: %s\n", __FUNCTION__, app_id);
    _release_mutex(g_pmlistMutex);
    return -1;
}

PUBLIC int get_app_list_head(list_node *output_list_head)
{
    if (output_list_head == NULL)
    {
        PM_TRACE_LOW("%s output_list_head is NULL \n", __FUNCTION__);
        return -1;
    }
    if (g_package_list_head == PNULL)
    {
        PM_TRACE_LOW("%s g_package_list_head is NULL \n", __FUNCTION__);
        return -1;
    }

    _acquire_mutex(g_pmlistMutex);
    clone_package_list_head(g_package_list_head, output_list_head);
    _release_mutex(g_pmlistMutex);

    return 0;
}

PUBLIC int send_uninstall_app_sig(char *app_id)
{
    int ret = -1;
    pm_uninstall_sign_t *sig_ptr = SCI_NULL;

    if (app_id == NULL)
    {
        return -1;
    }
    sig_ptr = (pm_uninstall_sign_t *)SCI_ALLOC_APP(sizeof(pm_uninstall_sign_t));
    if (sig_ptr)
    {
        sig_ptr->SignalSize = sizeof(pm_install_sign_t);
        sig_ptr->SignalCode = PM_SIG_UNINSTALL_APP;
        sig_ptr->Sender = SCI_IdentifyThread();

        sig_ptr->app_id = (char *)SCI_ALLOC_APP(MAX_NAME_LENTH);
        strncpy(sig_ptr->app_id, app_id, strlen(app_id) + 2); /*lint !e718 */
        // Send
        SCI_SendSignal((xSignalHeader)sig_ptr, g_pm_thread_id);
    }
    else
    {
        ret = PM_UNINSTALL_UNKNOW_ERROR;
    }
    return ret;
}

PUBLIC int send_install_app_sig(wchar *p_path, int len)
{
    int ret = -1;
    pm_install_sign_t *sig_ptr = SCI_NULL;

    if (p_path == NULL)
    {
        return -1;
    }
    sig_ptr = (pm_install_sign_t *)SCI_ALLOC_APP(sizeof(pm_install_sign_t));
    if (sig_ptr)
    {
        sig_ptr->SignalSize = sizeof(pm_install_sign_t);
        sig_ptr->SignalCode = PM_SIG_INSTALL_APP;
        sig_ptr->Sender = SCI_IdentifyThread();

        sig_ptr->app_path = (wchar *)SCI_ALLOC_APP(MAX_NAME_LENTH * 2);
        wstrncpy(sig_ptr->app_path, p_path, wstrlen(p_path) + 2);
        sig_ptr->app_path_len = wstrlen(p_path) + 2;
        // Send
        SCI_SendSignal((xSignalHeader)sig_ptr, g_pm_thread_id);
    }
    else
    {
        ret = PM_INSTALL_UNKNOW_ERROR;
    }
    return ret;
}

PUBLIC int send_init_sig()
{
    int ret = -1;
    xSignalHeader sig_ptr = SCI_NULL;

    sig_ptr = (xSignalHeader)SCI_ALLOC_APP(sizeof(xSignalHeaderRec));
    if (sig_ptr)
    {
        sig_ptr->SignalSize = sizeof(pm_install_sign_t);
        sig_ptr->SignalCode = PM_SIG_INIT;
        sig_ptr->Sender = SCI_IdentifyThread();

        // Send
        SCI_SendSignal((xSignalHeader)sig_ptr, g_pm_thread_id);
    }
    // Do init here avoid app init crash.
    // main_init();//bug1922321
    return ret;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
