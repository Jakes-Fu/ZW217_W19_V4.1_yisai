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
 * @file     jerry_device.c
 * @brief    -
 * @author   ningbiao.huang@unisoc.com
 * @version  V1.0.0
 * @date     2022-5-30
 * @history  5/2022       ningbiao.huang          Create
 *****************************************************************************
 **/

#include "jerry_bindings.h"
#include "js_device.h"
//#include "ual_device_info.h"
#include "urjs_art.h"

// mocor
#include "os_api.h"
#include "sci_types.h"
#include "guilcd.h"

#ifndef WIN32
#include "version.h"
extern const char *const s_version_info[];
#endif
#include "wifi_api.h"

#ifdef BLUETOOTH_SUPPORT
#include "bt_abs.h"
#endif

#define MAX_DEVICE_BUF_LEN (256)

#if 0

DECLARE_HANDLER(device_print)
{
    if (JERRY_UNLIKELY(args_cnt < 1))
    {
        // something need to do
        return jerry_create_boolean(false);
    }
    jerry_char_t str_buffer[JERRY_MAX_READ_BUFFER_SIZE + 1] = {0};
    for (jerry_length_t i = 0; i < args_cnt; i++)
    {
        jerry_value_t jstr = jerry_value_to_string(args_p[i]);
        jerry_length_t start_pos = 0;
        jerry_length_t end_pos = JERRY_MAX_READ_BUFFER_SIZE;
        jerry_size_t size = 0;
        while (true)
        {
            size = jerry_substring_to_char_buffer(jstr, start_pos, end_pos,
                                                  str_buffer,
                                                  JERRY_MAX_READ_BUFFER_SIZE);
            if (size > 0 && size < JERRY_MAX_READ_BUFFER_SIZE)
            {
                str_buffer[size] = 0;
#ifdef IDL_BINDING_TEST_WITH_STUB
                ual_dev_print((char *)str_buffer);
#else
                JS_DEBUG_LOG("%s", str_buffer);
#endif
            }
            else
            {
                break;
            }
            start_pos += JERRY_MAX_READ_BUFFER_SIZE;
            end_pos += JERRY_MAX_READ_BUFFER_SIZE;
        };
        jerry_release_value(jstr);
    }

    return jerry_create_boolean(true);
}

#ifdef IDL_BINDING_TEST_WITH_STUB
DECLARE_HANDLER(device_get_manufacture)
{
    char *result = ual_dev_get_manufacture();
    return jerry_create_string(result);
}
#endif // IDL_BINDING_TEST_WITH_STUB

#ifdef IDL_BINDING_TEST_WITH_STUB
DECLARE_HANDLER(device_get_brand)
{
    char *result = ual_dev_get_brand();
    return jerry_create_string(result);
}
#endif // IDL_BINDING_TEST_WITH_STUB

#ifdef IDL_BINDING_TEST_WITH_STUB
DECLARE_HANDLER(device_get_product)
{
    char *result = ual_dev_get_product();
    return jerry_create_string(result);
}
#endif // IDL_BINDING_TEST_WITH_STUB

#ifdef IDL_BINDING_TEST_WITH_STUB
DECLARE_HANDLER(device_get_model)
{
    char *result = ual_dev_get_model();
    return jerry_create_string(result);
}
#endif // IDL_BINDING_TEST_WITH_STUB

#ifdef IDL_BINDING_TEST_WITH_STUB
DECLARE_HANDLER(device_get_platform_ver)
{
    char *result = ual_dev_get_platform_ver();
    return jerry_create_string(result);
}
#endif // IDL_BINDING_TEST_WITH_STUB

#ifdef IDL_BINDING_TEST_WITH_STUB
DECLARE_HANDLER(device_get_base_ver)
{
    char *result = ual_dev_get_base_ver();
    return jerry_create_string(result);
}
#endif // IDL_BINDING_TEST_WITH_STUB

#ifdef IDL_BINDING_TEST_WITH_STUB
DECLARE_HANDLER(device_get_hw_ver)
{
    char *result = ual_dev_get_hw_ver();
    return jerry_create_string(result);
}
#endif // IDL_BINDING_TEST_WITH_STUB

#ifdef IDL_BINDING_TEST_WITH_STUB
DECLARE_HANDLER(device_get_build_time)
{
    char *result = ual_dev_get_build_time();
    return jerry_create_string(result);
}
#endif // IDL_BINDING_TEST_WITH_STUB

#ifdef IDL_BINDING_TEST_WITH_STUB
DECLARE_HANDLER(device_get_build_type)
{
    char *result = ual_dev_get_build_type();
    return jerry_create_string(result);
}
#endif // IDL_BINDING_TEST_WITH_STUB

#ifdef IDL_BINDING_TEST_WITH_STUB
DECLARE_HANDLER(device_get_ual_sdk_level)
{
    char *result = ual_dev_get_ual_sdk_level();
    return jerry_create_string(result);
}
#endif // IDL_BINDING_TEST_WITH_STUB

#ifdef IDL_BINDING_TEST_WITH_STUB
DECLARE_HANDLER(device_get_hardware_profile)
{
    char *result = ual_dev_get_hardware_profile();
    return jerry_create_string(result);
}
#endif // IDL_BINDING_TEST_WITH_STUB
#endif

//#ifdef IDL_BINDING_TEST_WITH_STUB
/*****************************************************************************/
//  Description: device_get_screen_size
//  Parameter: [In]:NONE
//  Return: Array of width and height
//  Author: ningbiao.huang
//  Note:
/*****************************************************************************/
DECLARE_HANDLER(device_get_screen_size)
{
    // Get device LCD width and height
    uint16 width = 0;
    uint16 height = 0;
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &width, &height);

    jerry_value_t out_array = jerry_create_array(2);
    jerry_value_t val;

    val = jerry_create_number(width);
    jerry_release_value(jerry_set_property_by_index(out_array, 0, val));
    jerry_release_value(val);

    val = jerry_create_number(height);
    jerry_release_value(jerry_set_property_by_index(out_array, 1, val));
    jerry_release_value(val);

    return out_array;
}

DECLARE_HANDLER(device_get_id)
{
#ifdef IDL_BINDING_TEST_WITH_STUB
    char *result = ual_dev_get_id();
#else
    // TODO:return true ID
    char *result = "ID-0000";
#endif // IDL_BINDING_TEST_WITH_STUB

    return jerry_create_string(result);
}

DECLARE_HANDLER(device_get_platform_ver)
{
    char platform_version_info[MAX_DEVICE_BUF_LEN +1] = "";
#ifndef WIN32
    int len = strlen(s_version_info[PLATFORM_VERSION]);
    len = (len > MAX_DEVICE_BUF_LEN) ? MAX_DEVICE_BUF_LEN : len;
    strncat(platform_version_info, s_version_info[PLATFORM_VERSION], len);
#endif
    return jerry_create_string(platform_version_info);
}

DECLARE_HANDLER(device_get_jee_platform_ver)
{
    char project_version_info[MAX_DEVICE_BUF_LEN] = "";
    return jerry_create_string(project_version_info);
}

DECLARE_HANDLER(device_get_bt_mac)
{
    char bt_mac[MAX_DEVICE_BUF_LEN] = "";
#ifdef BLUETOOTH_SUPPORT
    BT_ADDRESS device_addr = {0};

    MMIAPIBT_GetAddress(&device_addr);
    sprintf((char *)bt_mac, "%02x:%02x:%02x:%02x:%02x:%02x",
            device_addr.addr[5], device_addr.addr[4], device_addr.addr[3],
            device_addr.addr[2], device_addr.addr[1], device_addr.addr[0]);
#endif
    return jerry_create_string(bt_mac);
}

DECLARE_HANDLER(device_get_wifi_mac)
{
    char wifi_mac[MAX_DEVICE_BUF_LEN] = "";
#ifndef WIN32
	// unsupport
    // wifi_ap_get_mac(wifi_mac);
#endif

    return jerry_create_string(wifi_mac);
}

#if 0
#ifdef IDL_BINDING_TEST_WITH_STUB
DECLARE_HANDLER(device_get_screen_density)
{
    unsigned long result = ual_dev_get_screen_density();
    return jerry_create_number(result);
}
#endif // IDL_BINDING_TEST_WITH_STUB
#endif
/*****************************************************************************/
//  Description: js_device_get_platform_ver
//  Parameter: [In]
//             [Out]
//             [Return]
//  Author: xiaotong.su
//  Note:
/*****************************************************************************/
#if 0
DECLARE_HANDLER(js_device_get_platform_ver)
{
    char *p_version = PNULL;
    UAL_DEVICE_RESULT_E result = UAL_DEVICE_RETURN_FAILURE;

    //result = ual_device_info_get_platform_version(&p_version);
    if (UAL_DEVICE_RETURN_SUCCESS != result)
    {
        aloge("jerry_device get platform version failed!");
    }
    return jerry_create_string(p_version);
}
#endif

void js_register_device_functions()
{
    REGISTER_HANDLER(device_get_id);
    // version
    REGISTER_HANDLER(device_get_platform_ver);
    REGISTER_HANDLER(device_get_jee_platform_ver);
    // LCD
    REGISTER_HANDLER(device_get_screen_size);
    // BT
    REGISTER_HANDLER(device_get_bt_mac);
    // WIFI
    REGISTER_HANDLER(device_get_wifi_mac);
#if 0
#ifdef IDL_BINDING_TEST_WITH_STUB
REGISTER_HANDLER(device_print);
    REGISTER_HANDLER(device_get_manufacture);
    REGISTER_HANDLER(device_get_brand);
    REGISTER_HANDLER(device_get_product);
    REGISTER_HANDLER(device_get_model);
    REGISTER_HANDLER(device_get_base_ver);
    REGISTER_HANDLER(device_get_hw_ver);
    REGISTER_HANDLER(device_get_build_time);
    REGISTER_HANDLER(device_get_build_type);
    REGISTER_HANDLER(device_get_ual_sdk_level);
    REGISTER_HANDLER(device_get_hardware_profile);
    REGISTER_HANDLER(device_get_screen_density);
#endif
#endif
};
