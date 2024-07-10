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
 * @file     packagemanager_test.c
 * @brief    -
 * @author   ningbiao.huang
 * @version  V1.0.0
 * @date
 * @history
 * @note
 * detailed description:This is a JS app packagemanager
 *****************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2021 unisoc </center></h2>
 **/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

//#include "pm_list.h"
#include "packagemanager.h"
#include "packagemanager_test.h"
#include "pm_utils.h"

#include "pm_data_cache.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/

LOCAL const wchar *test_app_watch = L"E:\\package\\jswatch.urapk";
LOCAL const wchar *test_app_qq = L"E:\\package\\qq.urapk";
LOCAL const wchar *test_app_qq2 = L"E:\\package\\qq2.urapk";
LOCAL const wchar *test_app_wechat = L"E:\\package\\wechat.urapk";
LOCAL const wchar *test_app_name_jswatch = L"com.unisoc.jswatch";
LOCAL const wchar *test_app_name_watch = L"com.unisoc.watch";

// ZIP
LOCAL const wchar *test_src_zip = L"E:\\package\\test.zip";

LOCAL const wchar *test_src_zip2 = L"E:\\package\\com.unisoc.qq.zip";

LOCAL const wchar *test_dst_zip_package_dir = L"E:\\package";
LOCAL const wchar *test_dst_zip_dir_name = L"temp";
LOCAL const wchar *test_dst_zip_dir_name2 = L"temp2";

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

// For lint issue
#ifndef PM_TRACE_LOW
#define PM_TRACE_LOW(_format, ...)                                             \
    do                                                                         \
    {                                                                          \
        SCI_TRACE_LOW("{%s}[%s]<%d> "_format, "PM", __FUNCTION__, __LINE__,    \
                      ##__VA_ARGS__);                                          \
    } while (0)
#endif

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local Functions                                *
 **---------------------------------------------------------------------------*/
LOCAL void test_install_compress_work(uint32, void *);

LOCAL void info_to_string(pm_package_info_t *package_info)
{
    PM_TRACE_LOW("app_id=%s:\n", package_info->app_id);
    PM_TRACE_LOW("app_name=%s,vendor_name=%s \n", package_info->app_name,
                 package_info->vendor_name);
    PM_TRACE_LOW("app_version_code=%d, "
                 "app_version_name=%s,api_version_compatible=%s,api_version_"
                 "target=%s \n",
                 package_info->app_version_code, package_info->app_version_name,
                 package_info->api_version_compatible,
                 package_info->api_version_target);
    PM_TRACE_LOW("app_icon=%s, app_install_path=%s \n\n",
                 package_info->app_icon, package_info->app_install_path);
}

/**---------------------------------------------------------------------------*
 **                         External Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

// TEST CASE
PUBLIC void test_app_install(void)
{
    int ret = -1;
    PM_TRACE_LOW("[PM_TEST]:test_app_install test!");
    ret = js_app_install((wchar *)test_app_watch,
                         wstrlen((wchar *)test_app_watch));
    PM_TRACE_LOW("[PM_TEST]:test_app_install test ret=%d", ret);
    // app_install((wchar *)test_app_qq, wstrlen((wchar *)test_app_qq));
    // app_install((wchar *)test_app_wechat, wstrlen((wchar *)test_app_wechat));
    PM_TRACE_LOW("[PM_TEST]:test_app_install test end!!");
}

PUBLIC void test_app_uninstall(void)
{
    PM_TRACE_LOW("[PM_TEST]: test_app_uninstall test!");
    js_app_uninstall("com.unisoc.test.device");
    js_app_uninstall("com.unisoc.umusic");
    js_app_uninstall("com.unisoc.wechat");
    PM_TRACE_LOW("[PM_TEST]: test_app_uninstall test end!!");
}

PUBLIC void test_get_app_info(void)
{
    pm_package_info_t *pack_info = NULL;

    list_node *find_node_perm = NULL;
    pm_permisson_info_t *perm_info = NULL;

    PM_TRACE_LOW("[PM_TEST]: get_app_info!");
    pack_info = (pm_package_info_t *)SCI_ALLOC_APP(sizeof(pm_package_info_t));
    if (pack_info == NULL)
    {
        return;
    }

    SCI_MEMSET(pack_info, '\0', sizeof(pm_package_info_t)); /*lint !e718 */
    js_get_app_info("com.unisoc.watch", pack_info);
    PM_TRACE_LOW("[PM_TEST]: name=%s,path=%s", pack_info->app_name,
                 pack_info->app_install_path);
    info_to_string(pack_info);

    // release permission
    if (pack_info->p_permisson_list_head)
    {
        listnode_for_each(find_node_perm, pack_info->p_permisson_list_head)
        {
            perm_info = listnode_entry(find_node_perm, pm_permisson_info_t,
                                       permisson_info_node);
            SCI_FREE(perm_info);
            perm_info = NULL;
        }
        SCI_FREE(pack_info->p_permisson_list_head);
        pack_info->p_permisson_list_head = NULL;
    }

    SCI_FREE(pack_info);
    pack_info = NULL;
    PM_TRACE_LOW("[PM_TEST]: get_app_info end!!");
}

PUBLIC void test_get_all_app_info(void)
{
    pm_package_info_t *pack_info = NULL;
    list_node pack_list_head;
    list_node *find_node = NULL;

    PM_TRACE_LOW("[PM_TEST]: get_all_app_info!");
    init_listnode_head(&pack_list_head);
    js_get_all_app_info(&pack_list_head);
    listnode_for_each(find_node, &pack_list_head)
    {
        pack_info =
            listnode_entry(find_node, pm_package_info_t, package_info_node);
        info_to_string(pack_info);

        // release
        listnode_del(find_node);

        // release permission buff
        if (pack_info->p_permisson_list_head)
        {
            list_node *find_node_perm = NULL;
            pm_permisson_info_t *perm_info = NULL;

            listnode_for_each(find_node_perm, pack_info->p_permisson_list_head)
            {
                perm_info = listnode_entry(find_node_perm, pm_permisson_info_t,
                                           permisson_info_node);

                // delete permisson_info node from linklist
                listnode_del(find_node_perm);
                SCI_FREE(perm_info);
                perm_info = NULL;
            }
            SCI_FREE(pack_info->p_permisson_list_head);
            pack_info->p_permisson_list_head = NULL;
        }

        SCI_FREE(pack_info);
        pack_info = NULL;
    }
    PM_TRACE_LOW("[PM_TEST]: get_all_app_info!");
}

PUBLIC void test_start_pacakge_manager_service(void)
{
    PM_TRACE_LOW("[PM_TEST]: js_start_pacakge_manager_service!");
    js_start_pacakge_manager_service();
    PM_TRACE_LOW("[PM_TEST]: js_start_pacakge_manager_service end!!");
}

PUBLIC void test_pm_dump(void)
{
    PM_TRACE_LOW("[PM_TEST]: pm_dump!");
    js_pm_dump();
    PM_TRACE_LOW("[PM_TEST]: pm_dump end!!");
}

PUBLIC void test_zip_file(void)
{
    PM_TRACE_LOW("[PM_TEST]:test_zip_file test!");
    decode_zip_by_file((uint16 *)test_app_qq2,
                       (uint16 *)test_dst_zip_package_dir,
                       (uint16 *)test_dst_zip_dir_name2);
    PM_TRACE_LOW("[PM_TEST]:test_zip_file test end!!");
}

#define PM_TEST_APPE "com.unisoc.test"
#define PM_TEST_APPE2 PM_TEST_APPE "123"

PUBLIC void test_data_cache(void)
{
    PM_TRACE_LOW("[PM_TEST]:test_data_cache test!");
    create_data_appid_dir(PM_TEST_APPE);
    create_data_appid_dir(PM_TEST_APPE2);

    // remove_data_app_dir(PM_TEST_APPE);
    // remove_data_app_dir(PM_TEST_APPE2);
    remove_data_appid_data_dir(PM_TEST_APPE);
    remove_data_appid_cache_dir(PM_TEST_APPE);
    remove_data_appid_dir(PM_TEST_APPE);
    PM_TRACE_LOW("[PM_TEST]:test_data_cache test end!!");
}

#define PM_TEST_THREAD_STACK_SIZE (8 * 1024)
#define PM_TEST_THREAD_TASK_PRIORITY 15
#define PM_TEST_THREAD_QUEUE_NUM 10
#define PM_TEST_THREAD_TASK_NAME "T_THREAD_PM_TEST"
#define PM_TEST_THREAD_QUEUE_NAME "Q_THREAD_PM_TEST"

LOCAL void test_install_compress_work(uint32 arg_c, void *arg_v)
{
    int ret = -1;
    int test_count = 300;
    int i = 0;

    PM_TRACE_LOW("[PM_TEST]: begin!");
    for (i = 0; i < test_count; i++)
    {
        PM_TRACE_LOW("[PM_TEST]:  test i=%d!", i);
        ret = js_app_install((wchar *)test_app_watch,
                             wstrlen((wchar *)test_app_watch));
        PM_TRACE_LOW("[PM_TEST]:js_app_install ret=%d", ret);
        SCI_Sleep(100);
        ret = js_app_uninstall("com.unisoc.watch");
        PM_TRACE_LOW("[PM_TEST]:js_app_uninstall ret=%d", ret);
        SCI_Sleep(100);
    }
    PM_TRACE_LOW("[PM_TEST]:  end!!");
}

PUBLIC void test_install_compress(void)
{
    BLOCK_ID _thread_id = SCI_INVALID_BLOCK_ID;

    PM_TRACE_LOW("[PM_TEST]: begin!!");
    _thread_id = SCI_CreateThread(
        PM_TEST_THREAD_TASK_NAME, PM_TEST_THREAD_QUEUE_NAME,
        test_install_compress_work, 0, NULL, PM_TEST_THREAD_STACK_SIZE,
        PM_TEST_THREAD_QUEUE_NUM, PM_TEST_THREAD_TASK_PRIORITY, SCI_PREEMPT,
        SCI_AUTO_START);
    SCI_ASSERT(SCI_INVALID_BLOCK_ID != _thread_id && 0 != _thread_id);
    PM_TRACE_LOW("[PM_TEST]: end!!");
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif