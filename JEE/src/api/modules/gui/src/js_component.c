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
  * @file     js_component.c
  * @brief    -
  * @author   qingjun.yu@unisoc.com
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

#include "jerry_bindings.h"

#include "js_component.h"

#include "urjs_art.h"

#define PROP_PADDING "padding"
#define PROP_PADDING_LEFT "padding_left"
#define PROP_PADDING_TOP "padding_top"
#define PROP_PADDING_RIGHT "padding_right"
#define PROP_PADDING_BOTTOM "padding_bottom"

static void js_component_prop_value_change(uint32 handle,
    const jerry_value_t object,
    const jerry_string_t _prop_name)
{
    char* prop_name = jerry_string_data(&_prop_name);
    if (NULL == prop_name)
    {
        return;
    }
    if (strncmp(PROP_PADDING, prop_name, strlen(prop_name)) == 0)
    {
        int padding = js_component_get_prop_int(object, prop_name);
        js_native_component_set_padding(handle, padding, padding, padding,
            padding);
    }
    else if (strncmp(PROP_PADDING_LEFT, prop_name, strlen(prop_name)) == 0)
    {
        int padding_left = js_component_get_prop_int(object, prop_name);
        js_native_component_set_padding(handle, padding_left, 0, 0, 0);
    }
    else if (strncmp(PROP_PADDING_TOP, prop_name, strlen(prop_name)) == 0)
    {
        int padding_top = js_component_get_prop_int(object, prop_name);
        js_native_component_set_padding(handle, 0, padding_top, 0, 0);
    }
    else if (strncmp(PROP_PADDING_RIGHT, prop_name, strlen(prop_name)) == 0)
    {
        int padding_right = js_component_get_prop_int(object, prop_name);
        js_native_component_set_padding(handle, 0, 0, padding_right, 0);
    }
    else if (strncmp(PROP_PADDING_BOTTOM, prop_name, strlen(prop_name)) == 0)
    {
        int padding_bottom = js_component_get_prop_int(object, prop_name);
        js_native_component_set_padding(handle, 0, 0, 0, padding_bottom);
    }
    else
    {
        alogi("Unknow prop_name =%s", prop_name);
    }
}

PUBLIC int js_component_get_prop_int(jerry_value_t object, char* prop)
{
    jerry_value_t prop_name = jerry_create_string((jerry_char_t*)prop);
    jerry_value_t value = jerry_get_property((jerry_value_t)object, prop_name);
    jerry_release_value(prop_name);
    return jerry_get_number_value(value);
}

PUBLIC char* js_component_get_prop_string(jerry_value_t object, char* prop)
{
    return NULL;
}

DECLARE_PUBLIC_HANDLER(js_component_set_rect)
{
    JERRY_CHECK_ARGS(5, number, number, number, number, number);
    int handle = jerry_get_number_value(args_p[0]);
    int x = jerry_get_number_value(args_p[1]);
    int y = jerry_get_number_value(args_p[2]);
    int width = jerry_get_number_value(args_p[3]);
    int height = jerry_get_number_value(args_p[4]);

    js_native_component_set_rect(handle, x, y, width, height);

    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_component_set_bg_color)
{
    JERRY_CHECK_ARGS(2, number, number);
    int handle = jerry_get_number_value(args_p[0]);
    int color = jerry_get_number_value(args_p[1]);

    js_native_component_set_bg_color(handle, color);

    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_component_set_text_color)
{
    JERRY_CHECK_ARGS(2, number, number);
    int handle = jerry_get_number_value(args_p[0]);
    int color = jerry_get_number_value(args_p[1]);

    js_native_component_set_text_color(handle, color);

    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_component_set_property)
{
    JERRY_CHECK_ARGS(3, object, number, string);
    jerry_value_t obj = JERRY_GET_ARG(0, object);
    int handle = jerry_get_number_value(args_p[1]);
    jerry_string_t _prop_name = JERRY_GET_ARG(2, string);

    js_component_prop_value_change(handle, obj, _prop_name);

    jerry_string_destroy(&_prop_name);
    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_component_set_disable)
{
    JERRY_CHECK_ARGS(2, number, boolean);
    int handle = jerry_get_number_value(args_p[0]);
    BOOLEAN value = JERRY_GET_ARG(1, boolean);

    js_native_component_set_disable(handle, value);

    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_component_set_focusable)
{
    JERRY_CHECK_ARGS(2, number, boolean);
    int handle = jerry_get_number_value(args_p[0]);
    BOOLEAN value = JERRY_GET_ARG(1, boolean);

    js_native_component_set_focusable(handle, value);

    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_component_set_visibility)
{
    JERRY_CHECK_ARGS(2, number, boolean);
    int handle = jerry_get_number_value(args_p[0]);
    BOOLEAN value = JERRY_GET_ARG(1, boolean);

    js_native_component_set_visibility(handle, value);

    return jerry_create_null();
}