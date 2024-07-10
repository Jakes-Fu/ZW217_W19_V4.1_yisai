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
 * @file     permision_check.c
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

#include "permission_check.h"
#include "cJSON.h"
#include "guifont.h"
#include "jee.h"
#include "packagemanager.h"
#include "sci_types.h"
#include "sfs.h"
#include <string.h>

#define PERMISSION_JSON "permission.json"

static list_node *g_app_list_head = NULL;

LOCAL BOOLEAN file_access(char *json_path);

LOCAL struct _PERMISSION_INFO s_permission_all_descript[] = {
    {PERM_CAMERA, PERM_CAMERA_NAME},
    {PERM_READ_CONTACTS, PERM_READ_CONTACTS_NAME},
    {PERM_WRITE_CONTACTS, PERM_WRITE_CONTACTS_NAME},
    {PERM_ACCESS_FINE_LOCATION, PERM_ACCESS_FINE_LOCATION_NAME},
    {PERM_ACCESS_COARSE_LOCATION, PERM_ACCESS_COARSE_LOCATION_NAME},
    {PERM_SEND_SMS, PERM_SEND_SMS_NAME},
    {PERM_READ_SMS, PERM_READ_SMS_NAME},
    {PERM_RECORD_AUDIO, PERM_RECORD_AUDIO_NAME},
    {PERM_CALL_PHONE, PERM_CALL_PHONE_NAME},
    {PERM_BODY_SENSORS, PERM_BODY_SENSORS_NAME},
    {PERM_PACKAGES_MANAGER, PERM_PACKAGES_MANAGER_NAME},
    {PERM_ACCESS_WIFI, PERM_ACCESS_WIFI_NAME},
    {PERM_BATTERY_STATS, PERM_BATTERY_STATS_NAME},
    {PERM_BLUETOOTH, PERM_BLUETOOTH_NAME},
    {PERM_DEVICE_POWER, PERM_DEVICE_POWER_NAME},
    {PERM_FLASHLIGHT, PERM_FLASHLIGHT_NAME},
    {PERM_INTERNET, PERM_INTERNET_NAME},
    {PERM_NFC, PERM_NFC_NAME},
    {PERM_VIBRATE, PERM_VIBRATE_NAME},
    {PERM_EXTERNAL_STORAGE, PERM_EXTERNAL_STORAGE_NAME}};

//把权限名称转换为对应的权限ID
LOCAL int permission_str_to_id(const char *permission_str)
{
    int i = 0;
    for (i = 0; i < sizeof(s_permission_all_descript)
                        / sizeof(struct _PERMISSION_INFO);
         i++)
    {
        //从结构体数组中找到相应的权限
        if (strcmp(s_permission_all_descript[i].info, permission_str) == 0)
        {
            SCI_TRACE_LOW("permission_str_to_id return %d, %s",
                          s_permission_all_descript[i].id,
                          s_permission_all_descript[i].info);
            return s_permission_all_descript[i].id;
        }
    }
    return -1;
}

LOCAL char *permission_id_to_str(const int permission_id)
{
    int i = 0;
    for (i = 0; i < sizeof(s_permission_all_descript)
                        / sizeof(struct _PERMISSION_INFO);
         i++)
    {
        if (s_permission_all_descript[i].id == permission_id)
        {
            SCI_TRACE_LOW("permission_str_to_id return %d, %s",
                          s_permission_all_descript[i].id,
                          s_permission_all_descript[i].info);
            return s_permission_all_descript[i].info;
        }
    }
    return NULL;
}

//判断permission.json文件是否存在，以此判断权限系统是否初次运行
LOCAL BOOLEAN file_access(char *permission_file_path)
{
    SFS_HANDLE sfs_fd_ptr;
    BOOLEAN ret = FALSE;
    wchar file_path[SFS_MAX_PATH] = {0};

    MMIAPICOM_StrToWstr(permission_file_path, file_path);
    sfs_fd_ptr =
        SFS_CreateFile((uint16 *)file_path,
                       SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL);
    ret = (sfs_fd_ptr > 0) ? TRUE : FALSE;
    SFS_CloseFile(sfs_fd_ptr);
    SCI_TRACE_LOW("%s completed,ret is:%d \n", __FUNCTION__, ret);
    return ret;
}

LOCAL int getJsonInt(cJSON *object, const char *string)
{
    cJSON *obj = cJSON_GetObjectItem(object, string);
    return cJSON_GetObjectInt(obj);
}

LOCAL void free_app_head(int app_head_init)
{
    if (app_head_init == 0)
    {
        SCI_FREE(g_app_list_head);
    }
}

//权限模块第一次运行时数据初始化,利用Packagemanager中的信息进行初始化
LOCAL app_grant_permission_info_t *
init_app_list_perm(pm_package_info_t *pm_info)
{
    //安装包里的权限信息
    app_grant_permission_info_t *perm_app_info = NULL;
    list_node *permission_node = NULL;
    list_node *perm_list_head = NULL;
    char file_path[SFS_MAX_PATH];
    int app_head_init = -1;

    if (NULL == pm_info)
    {
        return NULL;
    }
    if (g_app_list_head == NULL)
    {
        g_app_list_head = (list_node *)SCI_ALLOC_APP(sizeof(list_node));
        if (NULL == g_app_list_head)
        {
            return NULL;
        }
        SCI_MEMSET(g_app_list_head, 0, sizeof(list_node));
        init_listnode_head(g_app_list_head);
        app_head_init = 0;
    }
    //给permissionmanager中单个app结构体分配内存
    perm_app_info = (app_grant_permission_info_t *)SCI_ALLOC_APP(
        sizeof(app_grant_permission_info_t));
    if (NULL == perm_app_info)
    {
        free_app_head(app_head_init);
        return NULL;
    }
    SCI_MEMSET(perm_app_info, 0, sizeof(app_grant_permission_info_t));

    //给权限头指针分配内存并初始化
    perm_list_head = (list_node *)SCI_ALLOC_APP(sizeof(list_node));
    if (NULL == perm_list_head)
    {
        free_app_head(app_head_init);
        SCI_FREE(perm_app_info);
        return NULL;
    }
    init_listnode_head(perm_list_head);
    //把pm中的app_id复制到PERM中  sizeof
    // 1958585  recent:  perm_app_info->app_id
    // bug id 1958585 CID 589140
    strncpy(perm_app_info->app_id, pm_info->app_id,
           strlen(pm_info->app_id));

    //根据pm中的安装路径构造Permissionmanager中的permission.json文件路径
    snprintf(file_path, sizeof(file_path) - 1, "%s%s%s",
             pm_info->app_install_path, "\\", PERMISSION_JSON);
    SCI_TRACE_LOW("%s ,file_path = %s \n", __FUNCTION__, file_path);

    strncpy(perm_app_info->permission_file_path, file_path,
            sizeof(perm_app_info->permission_file_path));

    listnode_for_each(permission_node, pm_info->p_permisson_list_head)
    {
        // 定义PM、PERM中单个APP的结构体指针变量
        pm_permisson_info_t *pm_perm_info = NULL;
        grant_permission_info_t *perm_info = NULL;

        // PM里APP中某一具体的权限
        pm_perm_info = listnode_entry(permission_node, pm_permisson_info_t,
                                      permisson_info_node);
        //给PERM中某一具体权限分配内存
        perm_info = (grant_permission_info_t *)SCI_ALLOC_APP(
            sizeof(grant_permission_info_t));
        if (NULL == perm_info)
        {
            listhead_clean(perm_list_head, grant_permission_info_t,
                           grant_permission_info_node);
            perm_list_head = NULL;
            free_app_head(app_head_init);
            SCI_FREE(perm_app_info);
            return NULL;
        }
        SCI_MEMSET(perm_info, 0, sizeof(grant_permission_info_t));

        //把PM中某APP的某一具体权限复制到PERM中
        perm_info->grant_permission_id =
            permission_str_to_id(pm_perm_info->permisson_name);

        perm_info->grant_permission_status = PREMISSION_STATUS_NONE;
        //将权限节点加入到头节点中
        listnode_add_tail(&perm_info->grant_permission_info_node,
                          perm_list_head);
    }

    perm_app_info->p_grant_permission_list_head = perm_list_head;
    listnode_add_tail(&perm_app_info->app_grant_list_node, g_app_list_head);

    return perm_app_info;
}

//打开permission.json文件
LOCAL char *open_permission_file(char file_path[SFS_MAX_PATH])
{
    char *json_data = NULL;
    uint32 file_size = 0;
    uint32 file_size_readed = 0;
    SFS_HANDLE sfs_fd_ptr = 0;
    SFS_ERROR_E device_result = SFS_ERROR_NO_SPACE;
    wchar perm_path[SFS_MAX_PATH] = {0};

    SCI_TRACE_LOW("%s start running\n", __FUNCTION__);

    if (NULL == file_path)
    {
        return NULL;
    }
    MMIAPICOM_StrToWstr(file_path, perm_path);

    sfs_fd_ptr =
        SFS_CreateFile((uint16 *)perm_path,
                       SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL);
    if (sfs_fd_ptr == 0)
    {
        return NULL;
    }
    SFS_SetFilePointer(sfs_fd_ptr, (int64)0, SFS_SEEK_BEGIN);
    SFS_GetFileSize(sfs_fd_ptr, &file_size);

    json_data = (char *)SCI_ALLOC_APP(file_size);
    SCI_ASSERT(json_data != PNULL);

    device_result = SFS_ReadFile(sfs_fd_ptr, (void *)json_data, file_size,
                                 (uint32 *)&file_size_readed, NULL);
    if (device_result != SFS_NO_ERROR)
    {
        SCI_TRACE_LOW("%s,failed to permission file \n", __FUNCTION__);
        SCI_FREE(json_data);
        return NULL;
    }
    SFS_CloseFile(sfs_fd_ptr);
    return json_data;
}

//获取权限数组(数组里每一个元素均包含权限id和状态)
LOCAL cJSON *get_permssion_arr(cJSON *root)
{
    cJSON *aim = NULL, *obj = NULL, *arr = NULL;
    //根据key值去获取对应的value
    if (NULL == root)
    {
        return NULL;
    }
    aim = cJSON_GetObjectItem(root, "permissions");
    if (NULL == aim)
    {
        return NULL;
    }
    obj = cJSON_GetObjectItem(aim, "app_id");
    if (NULL == obj)
    {
        return NULL;
    }
    arr = cJSON_GetObjectItem(aim, "grantPermissions");
    return arr;
}

//把读取的文件数据解析为cJSON格式
LOCAL cJSON *parse_json_info(char *json_data)
{
    cJSON *root = NULL;
    if (NULL == json_data)
    {
        return NULL;
    }
    root = cJSON_Parse(json_data);
    return root;
}

//通过permission.json文件初始化权限链表
LOCAL app_grant_permission_info_t *
parse_permission_file_info(pm_package_info_t *pm_info, char *json_data)
{
    int arr_index = 0;
    int len = 0;
    int i = 0;
    int app_head_init = -1;
    app_grant_permission_info_t *app_info = NULL;
    cJSON *root = NULL, *arr = NULL, *thm = NULL;
    char file_path[SFS_MAX_PATH] = {0};
    list_node *perm_list_head = NULL;

    SCI_TRACE_LOW("%s start running \n", __FUNCTION__);
    if ((NULL == pm_info) || (NULL == json_data))
    {
        return NULL;
    }

    if (g_app_list_head == NULL)
    {
        g_app_list_head = (list_node *)SCI_ALLOC_APP(sizeof(list_node));
        if (NULL == g_app_list_head)
        {
            return NULL;
        }
        SCI_MEMSET(g_app_list_head, 0, sizeof(list_node));
        init_listnode_head(g_app_list_head);
        app_head_init = 0;
    }

    root = parse_json_info(json_data);
    if (root == NULL)
    {
        free_app_head(app_head_init);
        return NULL;
    }
    arr = get_permssion_arr(root);
    if (arr == NULL)
    {
        free_app_head(app_head_init);
        cJSON_Delete(root);
        return NULL;
    }
    len = cJSON_GetArraySize(arr);

    //给permissionmanager中单个app结构体分配内存
    app_info = (app_grant_permission_info_t *)SCI_ALLOC_APP(
        sizeof(app_grant_permission_info_t));
    if (app_info == NULL)
    {
        free_app_head(app_head_init);
        cJSON_Delete(root);
        return NULL;
    }
    SCI_MEMSET(app_info, 0, sizeof(app_grant_permission_info_t));

    perm_list_head = (list_node *)SCI_ALLOC_APP(sizeof(list_node));
    if (NULL == perm_list_head)
    {
        SCI_FREE(app_info);
        free_app_head(app_head_init);
        cJSON_Delete(root);
        return NULL;
    }
    init_listnode_head(perm_list_head);
    // 1958585   recent : sizeof(app_info->app_id)
    // bug id 1958585 CID 589644  
    strncpy(app_info->app_id, pm_info->app_id, strlen(pm_info->app_id));

    snprintf(file_path, sizeof(file_path) - 1, "%s%s%s",
             pm_info->app_install_path, "\\", PERMISSION_JSON);

    strncpy(app_info->permission_file_path, file_path,
            sizeof(app_info->permission_file_path));

    for (i = 0; i < len; i++)
    {
        grant_permission_info_t *perm_info = NULL;
        perm_info = (grant_permission_info_t *)SCI_ALLOC_APP(
            sizeof(grant_permission_info_t));
        if (NULL == perm_info)
        {
            listhead_clean(perm_list_head, grant_permission_info_t,
                           grant_permission_info_node);
            perm_list_head = NULL;
            free_app_head(app_head_init);
            SCI_FREE(app_info);
            cJSON_Delete(root);
            return NULL;
        }
        SCI_MEMSET(perm_info, 0, sizeof(grant_permission_info_t));

        thm = cJSON_GetArrayItem(arr, arr_index++);

        if (NULL == thm)
        {
            listhead_clean(perm_list_head, grant_permission_info_t,
                           grant_permission_info_node);
            perm_list_head = NULL;
            free_app_head(app_head_init);
            SCI_FREE(app_info);
            cJSON_Delete(root);
            return NULL;
        }

        perm_info->grant_permission_id = getJsonInt(thm, "name");

        perm_info->grant_permission_status = getJsonInt(thm, "status");

        listnode_add_tail(&perm_info->grant_permission_info_node,
                          perm_list_head);
    }
    app_info->p_grant_permission_list_head = perm_list_head;
    listnode_add_tail(&app_info->app_grant_list_node, g_app_list_head);
    cJSON_Delete(root);
    return app_info;
}

LOCAL app_grant_permission_info_t *
init_app_list_perm_with_json(pm_package_info_t *pm_info)
{
    char file_path[SFS_MAX_PATH] = {0};
    char *perm_data = NULL;
    app_grant_permission_info_t *perm_info = NULL;

    if (NULL == pm_info)
    {
        return NULL;
    }
    snprintf(file_path, sizeof(file_path) - 1, "%s%s%s",
             pm_info->app_install_path, "\\", PERMISSION_JSON);

    perm_data = open_permission_file(file_path);
    if (NULL == perm_data)
    {
        return NULL;
    }
    perm_info = parse_permission_file_info(pm_info, perm_data);
    SCI_FREE(perm_data);
    return perm_info;
}

LOCAL app_grant_permission_info_t *init_app_info(char file_path[MAX_NAME_LENTH],
                                                 pm_package_info_t *pm_info)
{
    app_grant_permission_info_t *app_info = NULL;
    // permisson.json文件存在时
    if (file_access(file_path))
    {
        //以前运行过该APP
        app_info = init_app_list_perm_with_json(pm_info);
    }
    else
    {
        //从没有运行过该APP
        app_info = init_app_list_perm(pm_info);
    }
    return app_info;
}

PUBLIC void MMIPERMISS_CreateUI(app_grant_permission_info_t *_info,
                                PERM_NOTFIY perm_notify);
//权限授权
PUBLIC void perm_grant_app_permissions(char *app_id, PERM_NOTFIY perm_notify)
{
    pm_package_info_t *pm_info = NULL;
    app_grant_permission_info_t *app_info = NULL;
    char file_path[SFS_MAX_PATH] = {0};
    int has_found = -1;

    SCI_TRACE_LOW("%s start  running \n", __FUNCTION__);
    if (NULL == app_id)
    {
        perm_notify(0);
        return;
    }

    pm_info = (pm_package_info_t *)SCI_ALLOC_APP(sizeof(pm_package_info_t));
    if (NULL == pm_info)
    {
        return;
    }
    SCI_MEMSET(pm_info, 0, sizeof(pm_package_info_t));

    has_found = js_get_app_info(app_id, pm_info);

    if (has_found == -1)
    {
        perm_notify(0);
        // release permission buff
        listhead_clean(pm_info->p_permisson_list_head, pm_permisson_info_t,
                       permisson_info_node);
        SCI_FREE(pm_info);
        pm_info = NULL;
        return;
    }
    //根据pm中的安装路径构造permission.json文件路径
    snprintf(file_path, sizeof(file_path) - 1, "%s%s%s",
             pm_info->app_install_path, "\\", PERMISSION_JSON);

    if (g_app_list_head != NULL)
    {
        //本次开机后运行过部分APP
        app_grant_permission_info_t *info = NULL;
        list_node *find_node = NULL;
        listnode_for_each(find_node, g_app_list_head)
        {
            info = listnode_entry(find_node, app_grant_permission_info_t,
                                  app_grant_list_node);
            if (strcmp(pm_info->app_id, info->app_id) == 0)
            {
                app_info = info;
            }
        }
        //在链表中没有该app节点
    }
    if (app_info == NULL)
    {
        app_info = init_app_info(file_path, pm_info);
    }

    // release PM permission buff
    listhead_clean(pm_info->p_permisson_list_head, pm_permisson_info_t,
                   permisson_info_node);
    SCI_FREE(pm_info);
    pm_info = NULL;

    if (app_info == NULL)
    {
        SCI_TRACE_LOW("app_info == NULL");
        perm_notify(0);
        return;
    }
    MMIPERMISS_CreateUI(app_info, perm_notify);
}

//返回链表中的相应权限的状态
LOCAL grant_permission_status_e
find_status_in_app_list(app_grant_permission_info_t *info, int permission_id)
{
    list_node *find_node = NULL;
    listnode_for_each(find_node, info->p_grant_permission_list_head)
    {
        grant_permission_info_t *perm_info = NULL;
        perm_info = listnode_entry(find_node, grant_permission_info_t,
                                   grant_permission_info_node);
        if (perm_info->grant_permission_id == permission_id)
        {
            return perm_info->grant_permission_status;
        }
    }
    return PREMISSION_STATUS_NONE;
}

//返回permission.json文件中的相应权限的状态
LOCAL grant_permission_status_e find_status_in_perm_file(char *json_data,
                                                         int permission_id)
{
    int arr_index = 0;
    int len = 0;
    cJSON *root = NULL, *arr = NULL, *thm = NULL;
    int i = 0;

    SCI_TRACE_LOW("%s start running \n", __FUNCTION__);

    root = parse_json_info(json_data);
    SCI_FREE(json_data);
    if (root == NULL)
    {
        return PREMISSION_STATUS_NONE;
    }
    arr = get_permssion_arr(root);
    if (arr == NULL)
    {
        cJSON_Delete(root);
        return PREMISSION_STATUS_NONE;
    }
    len = cJSON_GetArraySize(arr);
    if (len == 0)
    {
        cJSON_Delete(root);
        return PREMISSION_STATUS_NONE;
    }
    for (i = 0; i < len; i++)
    {
        thm = cJSON_GetArrayItem(arr, arr_index++);
        if (permission_id == getJsonInt(thm, "name"))
        {
            cJSON_Delete(root);
            return getJsonInt(thm, "status");
        }
    }
    cJSON_Delete(root);
    return PREMISSION_STATUS_NONE;
}

LOCAL grant_permission_status_e
find_status_in_file(char file_path[MAX_NAME_LENTH], int permission_id)
{
    char *perm_data = NULL;
    grant_permission_status_e status = PREMISSION_STATUS_NONE;

    if (file_access(file_path))
    {
        perm_data = open_permission_file(file_path);
        if (perm_data == NULL)
        {
            return PREMISSION_STATUS_NONE;
        }
        status = find_status_in_perm_file(perm_data, permission_id);
        SCI_FREE(perm_data);
        return status;
    }
    return PREMISSION_STATUS_NONE;
}

LOCAL grant_permission_status_e check_permission(char *app_id,
                                                 int permission_id)
{
    pm_package_info_t pm_info = {0};
    app_grant_permission_info_t *info = NULL;
    char file_path[MAX_NAME_LENTH] = {0};
    char *perm_data = NULL;
    int has_found = -1;
    SCI_TRACE_LOW("%s start running \n", __FUNCTION__);

    if ((app_id == NULL) || (permission_id < 0))
    {
        return NULL;
    }

    js_get_app_info(app_id, &pm_info);
    snprintf(file_path, sizeof(file_path) - 1, "%s%s%s",
             pm_info.app_install_path, "\\", PERMISSION_JSON);

    if (g_app_list_head)
    {
        list_node *find_node = NULL;
        listnode_for_each(find_node, g_app_list_head)
        {
            info = listnode_entry(find_node, app_grant_permission_info_t,
                                  app_grant_list_node);
            if (strcmp(app_id, info->app_id) == 0)
            {
                has_found = 1;
            }
        }
    }
    if (has_found == 1)
    {
        //在链表中存在此APP节点
        return find_status_in_app_list(info, permission_id);
    }
    else
    {
        return find_status_in_file(file_path, permission_id);
    }
    return PREMISSION_STATUS_NONE;
}

//权限核查
PUBLIC grant_permission_status_e perm_check_permission_status(int permission_id)
{
    char app_id[MAX_NAME_LENTH] = {0};
    jee_get_app_guid(app_id, MAX_NAME_LENTH);
    SCI_TRACE_LOW("%s , permission_id is :%d \n", __FUNCTION__, permission_id);
    return check_permission(app_id, permission_id);
}
