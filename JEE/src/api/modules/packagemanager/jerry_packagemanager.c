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
 * @file     jerry_packagemanager.c
 * @brief    -
 * @author   ningbiao.huang@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-27
 * @history
 *****************************************************************************
 **/

#include "jerry_bindings.h"
#include "jerry_packagemanager.h"

#include "os_api.h"
#include "jee_list.h"
#include "packagemanager.h"

/*lint -e64 */

/**---------------------------------------------------------------------------*
 **                         Mocor Variables                                *
 **---------------------------------------------------------------------------*/

#define JS_PM_LOG(_format, ...)                                                \
    do                                                                         \
    {                                                                          \
        SCI_TRACE_LOW("{%s}[%s]<%d> "_format, "[JS_NATIVE_PM]", __FUNCTION__,  \
                      __LINE__, ##__VA_ARGS__);                                \
    } while (0)

/**---------------------------------------------------------------------------*
 **                         Local Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         LOCAL Functions                                *
 **---------------------------------------------------------------------------*/

LOCAL wchar *str_to_wstr(const uint8 *src, wchar *dst)
{
    wchar *return_wstr = dst;

    if (PNULL == src || PNULL == dst)
    {
        return return_wstr;
    }

    while ('\0' != (*dst++ = *src++)) /* copy string */
    {
        ;
    }

    return return_wstr;
}

/**
 * list_size - get size of list
 * @head: the head of list.
 */
LOCAL int listnode_size(list_node *head)
{
    int size = 0;
    list_node *find_node = NULL;

    if (PNULL == head)
    {
        return 0;
    }
    listnode_for_each(find_node, head)
    {
        size++;
    }
    return size;
}

LOCAL jerry_value_t constructJSObject()
{
    jerry_value_t object = jerry_create_object();
    return object;
}

LOCAL void set_property_value_string(jerry_value_t object,
                                     jerry_char_t *_prop_name,
                                     jerry_char_t *_prop_value)
{
    if (NULL != _prop_value && strlen(_prop_value) > 0)
    {
        jerry_value_t prop_name =
            jerry_create_string((const jerry_char_t *)_prop_name);
        jerry_value_t prop_value =
            jerry_create_string((const jerry_char_t *)_prop_value);
        jerry_release_value(jerry_set_property(object, prop_name, prop_value));
        jerry_release_value(prop_value);
        jerry_release_value(prop_name);
    }
}

LOCAL void set_property_value_number(jerry_value_t object,
                                     jerry_char_t *_prop_name,
                                     double _prop_value)
{
    jerry_value_t prop_value = jerry_create_number((double)_prop_value);
    jerry_value_t prop_name = jerry_create_string(_prop_name);
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);
}

// construct define in packagemanager.js
LOCAL jerry_value_t make_pageInfo(pm_pages_info_t *page_info)
{
    jerry_value_t prop_value;
    jerry_value_t prop_name;
    if (NULL == page_info)
    {
        return jerry_create_null();
    }
    jerry_value_t object = constructJSObject();

    set_property_value_string(object, (const jerry_char_t *)"pageName",
                              (const jerry_char_t *)page_info->page_name);

    return object;
}

LOCAL jerry_value_t make_permissonInfo(pm_permisson_info_t *permissionInfo)
{
    jerry_value_t prop_value;
    jerry_value_t prop_name;
    if (NULL == permissionInfo)
    {
        return jerry_create_null();
    }
    jerry_value_t object = constructJSObject();

    set_property_value_string(
        object, (const jerry_char_t *)"permissonName",
        (const jerry_char_t *)permissionInfo->permisson_name);

    return object;
}

LOCAL jerry_value_t make_appInfo(pm_package_info_t *appInfo)
{
    jerry_value_t prop_value;
    jerry_value_t prop_name;
    list_node *find_node_perm = NULL;
    pm_permisson_info_t *perm_info = NULL;

    jerry_value_t object = constructJSObject();

    if (NULL == appInfo)
    {
        JS_PM_LOG(" appInfo=null \n");
        return jerry_create_null();
    }

    set_property_value_string(object, (const jerry_char_t *)"appID",
                              (const jerry_char_t *)appInfo->app_id);

    set_property_value_string(object, (const jerry_char_t *)"appName",
                              (const jerry_char_t *)appInfo->app_name);

    set_property_value_string(object, (const jerry_char_t *)"vendorName",
                              (const jerry_char_t *)appInfo->vendor_name);

    set_property_value_string(object, (const jerry_char_t *)"parameter",
                              (const jerry_char_t *)appInfo->parameter);

    set_property_value_number(object, (const jerry_char_t *)"appVersionCode",
                              (double)appInfo->app_version_code);

    set_property_value_string(object, (const jerry_char_t *)"appVersionName",
                              (const jerry_char_t *)appInfo->app_version_name);

    set_property_value_string(
        object, (const jerry_char_t *)"apiVersionCompatible",
        (const jerry_char_t *)appInfo->api_version_compatible);

    set_property_value_string(
        object, (const jerry_char_t *)"apiVersionTarget",
        (const jerry_char_t *)appInfo->api_version_target);

    set_property_value_string(object, (const jerry_char_t *)"appInstallPath",
                              (const jerry_char_t *)appInfo->app_install_path);

    set_property_value_string(object, (const jerry_char_t *)"appIcon",
                              (const jerry_char_t *)appInfo->app_icon);

    if (appInfo->p_permisson_list_head)
    {
        int index = 0;
        int list_size = listnode_size(appInfo->p_permisson_list_head);

        if (list_size > 0)
        {
            prop_value = jerry_create_array(list_size);
            listnode_for_each(find_node_perm, appInfo->p_permisson_list_head)
            {
                jerry_value_t perm_object;

                perm_info = listnode_entry(find_node_perm, pm_permisson_info_t,
                                           permisson_info_node);
                if (NULL == perm_info)
                {
                    continue;
                }
                perm_object = make_permissonInfo(perm_info);

                jerry_release_value(jerry_set_property_by_index(
                    prop_value, index, perm_object));
                jerry_release_value(perm_object);
                index++;
            }
            prop_name =
                jerry_create_string((const jerry_char_t *)"permissions");
            // jerry_set_property(object, prop_name, prop_value);
            jerry_release_value(
                jerry_set_property(object, prop_name, prop_value));
            jerry_release_value(prop_value);
            jerry_release_value(prop_name);
        }
    }

    if (appInfo->p_pages_list_head)
    {
        int index = 0;
        int list_size = listnode_size(appInfo->p_pages_list_head);
        pm_pages_info_t *page_info = NULL;

        if (list_size > 0)
        {
            find_node_perm = NULL;
            prop_value = jerry_create_array(list_size);
            listnode_for_each(find_node_perm, appInfo->p_pages_list_head)
            {
                jerry_value_t page_object;

                page_info = listnode_entry(find_node_perm, pm_pages_info_t,
                                           page_info_node);
                if (NULL == page_info)
                {
                    continue;
                }
                page_object = make_pageInfo(page_info);

                jerry_release_value(jerry_set_property_by_index(
                    prop_value, index, page_object));
                jerry_release_value(page_object);
                index++;
            }
            prop_name = jerry_create_string((const jerry_char_t *)"pages");
            // jerry_set_property(object, prop_name, prop_value);
            jerry_release_value(
                jerry_set_property(object, prop_name, prop_value));
            jerry_release_value(prop_value);
            jerry_release_value(prop_name);
        }
    }

    return object;
}

/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

DECLARE_HANDLER(js_packagemanager_app_install)
{
    jerry_size_t str_size = 0;
    wchar app_path[256] = {0};
    int result = 0;

    if (JERRY_UNLIKELY(args_cnt < 1))
    {
        return jerry_create_number(0.0);
    }

    str_size = jerry_get_string_size(args_p[0]) + 1;
    JERRY_VLA(jerry_char_t, _app_path, str_size);
    str_size = jerry_string_to_char_buffer(args_p[0], _app_path,
                                           str_size); /*lint !e119 */
    _app_path[str_size] = '\0';
    // app_path = _app_path;
    str_to_wstr(_app_path, app_path);

    result = js_app_install(app_path, str_size);
    JS_PM_LOG(" result=%d \n", result);

    return jerry_create_number((double)result);
}

DECLARE_HANDLER(js_packagemanager_app_uninstall)
{
    jerry_size_t str_size = 0;
    jerry_char_t *app_id = NULL;
    int result = 0;

    if (JERRY_UNLIKELY(args_cnt < 1))
    {
        return jerry_create_number(0.0);
    }

    str_size = jerry_get_string_size(args_p[0]) + 1;
    JERRY_VLA(jerry_char_t, _app_id, str_size);
    str_size = jerry_string_to_char_buffer(args_p[0], _app_id,
                                           str_size); /*lint !e119 */
    _app_id[str_size] = '\0';
    app_id = _app_id;

    result = js_app_uninstall((char *)app_id);
    JS_PM_LOG(" result=%d \n", result);

    return jerry_create_number((double)result);
}

DECLARE_HANDLER(js_packagemanager_get_app_info)
{
    jerry_size_t str_size = 0;
    pm_package_info_t output_info = {0};
    jerry_char_t *app_id = NULL;
    int result = 0;
    jerry_value_t out_array = 0;

    if (JERRY_UNLIKELY(args_cnt < 1))
    {
        return jerry_create_null();
    }

    str_size = jerry_get_string_size(args_p[0]) + 1;
    JERRY_VLA(jerry_char_t, _app_id, str_size);
    str_size = jerry_string_to_char_buffer(args_p[0], _app_id,
                                           str_size); /*lint !e119 */
    _app_id[str_size] = '\0';
    app_id = _app_id;

    result = js_get_app_info((char *)app_id, &output_info);
    JS_PM_LOG(" result=%d \n", result);

    if (result != 0)
    {
        return jerry_create_null();
    }

    out_array = make_appInfo(&output_info);

    // release permission list
    listhead_clean(output_info.p_permisson_list_head, pm_permisson_info_t,
                   permisson_info_node);

    // release pages list
    listhead_clean(output_info.p_pages_list_head, pm_pages_info_t,
                   page_info_node);

    return out_array;
}

DECLARE_HANDLER(js_packagemanager_get_all_app_info)
{
    pm_package_info_t *pack_info = NULL;
    list_node pack_list_head = {0};
    list_node *find_node = NULL;
    jerry_value_t out_array = 0;
    int index = 0;
    int list_size = 0;

    JS_PM_LOG(" enter! \n");
    init_listnode_head(&pack_list_head);
    js_get_all_app_info(&pack_list_head);

    list_size = listnode_size(&pack_list_head);
    if (list_size <= 0)
    {
        JS_PM_LOG(" return null ! \n");
        return jerry_create_null();
    }

    out_array = jerry_create_array(list_size);

    listnode_for_each(find_node, &pack_list_head)
    {
        jerry_value_t find_object;
        pack_info =
            listnode_entry(find_node, pm_package_info_t, package_info_node);

        // build app info object
        find_object = make_appInfo(pack_info);

        // add to array
        jerry_release_value(
            jerry_set_property_by_index(out_array, index, find_object));
        jerry_release_value(find_object);
        index++;

        if (pack_info)
        {
            // release permission list
            listhead_clean(pack_info->p_permisson_list_head,
                           pm_permisson_info_t, permisson_info_node);

            // release pages list
            listhead_clean(pack_info->p_pages_list_head, pm_pages_info_t,
                           page_info_node);
        }

        // release
        listnode_del(find_node);
        SCI_FREE(pack_info);
        pack_info = NULL;
    }

    return out_array;
}

void js_register_packagemanager_functions()
{
    /*lint -e516 */
    REGISTER_HANDLER(js_packagemanager_app_install);
    REGISTER_HANDLER(js_packagemanager_app_uninstall);
    REGISTER_HANDLER(js_packagemanager_get_app_info);
    REGISTER_HANDLER(js_packagemanager_get_all_app_info);
}