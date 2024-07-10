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
 * @file     js_slider.c
 * @brief    -
 * @author   ningbiao.huang@unisoc.com
 * @version  V1.0.0
 * @date     2022-6-27
 * @history  2022-6-27       ningbiao.huang          Create
 *****************************************************************************
 **/
#include <stdlib.h>

#include "js_slider.h"

#include "slider.h"

/**---------------------------------------------------------------------------*
 * **                         Compiler Flag *
 * **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 * **                         Global Variables *
 * **---------------------------------------------------------------------------*/

#define TYPE_HORIZONTAL "horizontal"
#define TYPE_VERTICAL "vertical"

/**---------------------------------------------------------------------------*
 * **                         Local Functions *
 * **---------------------------------------------------------------------------*/
LOCAL void init_slider_default_param(uint32 handle);

LOCAL void init_slider_default_param(uint32 handle)
{
    if (handle == 0)
    {
        return;
    }
    // set min=0
    js_native_slider_setmin(handle, 0);
    // set max=100
    js_native_slider_setmax(handle, 100);
    // set step=1
    js_native_slider_set_step(handle, 1);
    // set display type=horizontal
    js_native_slider_set_display_vertical(handle, FALSE);
    // set show-block=FALSE
    js_native_slider_set_need_thumb(handle, FALSE);
    // set thumb size=8
    js_native_slider_set_thumb_size(handle, 10, 10);
    // set channel bg color=0xffff
    js_native_slider_set_channel_bgcolor(handle, 0xffff);
    // set channel fg color=0x26ff
    js_native_slider_set_channel_fgcolor(handle, 0x26ff);
    // set whether nedd bg color
    js_native_slider_set_need_bgcolor(handle, TRUE);
}

/**---------------------------------------------------------------------------*
 * **                         External Functions *
 * **---------------------------------------------------------------------------*/

DECLARE_PUBLIC_HANDLER(js_slider_create)
{
    JERRY_CHECK_ARGS(2, object, number);
    jerry_value_t obj = JERRY_GET_ARG(0, object);
    uint32 parent_handle = jerry_get_number_value(args_p[1]);

    uint32 handle = js_native_slider_create(obj, parent_handle);
    // init
    init_slider_default_param(handle);
    return jerry_create_number(handle);
}

DECLARE_PUBLIC_HANDLER(js_slider_destory)
{
    JERRY_CHECK_ARGS(1, number);
    uint32 handle = jerry_get_number_value(args_p[0]);

    js_native_slider_destory(handle);
    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_slider_setmin)
{
    JERRY_CHECK_ARGS(2, number, number);
    uint32 handle = jerry_get_number_value(args_p[0]);
    uint32 min = jerry_get_number_value(args_p[1]);

    BOOLEAN result = js_native_slider_setmin(handle, (int16)min);

    return jerry_create_boolean(result);
}

DECLARE_PUBLIC_HANDLER(js_slider_getmin)
{
    JERRY_CHECK_ARGS(1, number);
    uint32 handle = jerry_get_number_value(args_p[0]);

    int16 min = js_native_slider_getmin(handle);

    return jerry_create_number(min);
}

DECLARE_PUBLIC_HANDLER(js_slider_setmax)
{
    JERRY_CHECK_ARGS(2, number, number);
    uint32 handle = jerry_get_number_value(args_p[0]);
    uint32 max = jerry_get_number_value(args_p[1]);

    BOOLEAN result = js_native_slider_setmax(handle, (int16)max);

    return jerry_create_boolean(result);
}

DECLARE_PUBLIC_HANDLER(js_slider_getmax)
{
    JERRY_CHECK_ARGS(1, number);
    uint32 handle = jerry_get_number_value(args_p[0]);

    int16 max = js_native_slider_getmax(handle);

    return jerry_create_number(max);
}

DECLARE_PUBLIC_HANDLER(js_slider_set_channel_size)
{
    JERRY_CHECK_ARGS(3, number, number, number);
    uint32 handle = jerry_get_number_value(args_p[0]);
    uint32 width = jerry_get_number_value(args_p[1]);
    uint32 height = jerry_get_number_value(args_p[2]);

    BOOLEAN result =
        js_native_slider_set_channel_size(handle, (int16)width, (int16)height);

    return jerry_create_boolean(result);
}

DECLARE_PUBLIC_HANDLER(js_slider_get_channel_size)
{
    JERRY_CHECK_ARGS(1, number);
    uint32 handle = jerry_get_number_value(args_p[0]);

    uint16 width = 0;
    uint16 height = 0;
    js_native_slider_get_channel_size(handle, &width, &height);

    // construct object
    jerry_value_t object = jerry_create_object();

    jerry_value_t prop_value = jerry_create_number(width);
    jerry_value_t prop_name =
        jerry_create_string((const jerry_char_t *)"width");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    prop_value = jerry_create_number(height);
    prop_name = jerry_create_string((const jerry_char_t *)"height");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    return object;
}

DECLARE_PUBLIC_HANDLER(js_slider_set_step)
{
    JERRY_CHECK_ARGS(2, number, number);
    uint32 handle = jerry_get_number_value(args_p[0]);
    uint32 step = jerry_get_number_value(args_p[1]);

    BOOLEAN result = js_native_slider_set_step(handle, (uint16)step);

    return jerry_create_boolean(result);
}

DECLARE_PUBLIC_HANDLER(js_slider_get_step)
{
    JERRY_CHECK_ARGS(1, number);
    uint32 handle = jerry_get_number_value(args_p[0]);

    int16 step = js_native_slider_get_step(handle);

    return jerry_create_number(step);
}

// display type
DECLARE_PUBLIC_HANDLER(js_slider_set_display_type)
{
    JERRY_CHECK_ARGS(2, number, string);
    uint32 handle = jerry_get_number_value(args_p[0]);
    jerry_string_t type = JERRY_GET_ARG(1, string);

    char *type_name = jerry_string_data(&type);
    if (NULL == type_name)
    {
        return jerry_create_boolean(FALSE);
    }
    BOOLEAN result = FALSE;

    if (stricmp(type_name, TYPE_HORIZONTAL) == 0)
    {
        result = js_native_slider_set_display_vertical(handle, FALSE);
    }
    else if (stricmp(type_name, TYPE_VERTICAL) == 0)
    {
        result = js_native_slider_set_display_vertical(handle, TRUE);
    }
    else
    {
        result = js_native_slider_set_display_vertical(handle, FALSE);
    }

    return jerry_create_boolean(result);
}

DECLARE_PUBLIC_HANDLER(js_slider_get_display_type)
{
    JERRY_CHECK_ARGS(1, number);
    uint32 handle = jerry_get_number_value(args_p[0]);

    BOOLEAN is_vertical = js_native_slider_get_display_vertical(handle);
    jerry_value_t type_string = 0;
    if (is_vertical)
    {
        type_string =
            jerry_create_string(((const jerry_char_t *)TYPE_VERTICAL));
    }
    else
    {
        type_string =
            jerry_create_string(((const jerry_char_t *)TYPE_HORIZONTAL));
    }
    return type_string;
}

DECLARE_PUBLIC_HANDLER(js_slider_set_cur_value)
{
    JERRY_CHECK_ARGS(2, number, number);
    uint32 handle = jerry_get_number_value(args_p[0]);
    uint32 cur_value = jerry_get_number_value(args_p[1]);

    BOOLEAN result = js_native_slider_set_cur_value(handle, (int16)cur_value);

    return jerry_create_boolean(result);
}

DECLARE_PUBLIC_HANDLER(js_slider_get_cur_value)
{
    JERRY_CHECK_ARGS(1, number);
    uint32 handle = jerry_get_number_value(args_p[0]);

    int16 cur_value = js_native_slider_get_cur_value(handle);

    return jerry_create_number(cur_value);
}

// channel color
DECLARE_PUBLIC_HANDLER(js_slider_set_channel_bgcolor)
{
    JERRY_CHECK_ARGS(2, number, number);
    uint32 handle = jerry_get_number_value(args_p[0]);
    uint16 bg_color = jerry_get_number_value(args_p[1]);

    BOOLEAN result = js_native_slider_set_channel_bgcolor(handle, bg_color);

    return jerry_create_boolean(result);
}

DECLARE_PUBLIC_HANDLER(js_slider_get_channel_bgcolor)
{
    JERRY_CHECK_ARGS(1, number);
    uint32 handle = jerry_get_number_value(args_p[0]);

    uint16 bg_color = js_native_slider_get_channel_bgcolor(handle);

    return jerry_create_number(bg_color);
}

DECLARE_PUBLIC_HANDLER(js_slider_set_channel_fgcolor)
{
    JERRY_CHECK_ARGS(2, number, number);
    uint32 handle = jerry_get_number_value(args_p[0]);
    uint16 fg_color = jerry_get_number_value(args_p[1]);

    BOOLEAN result = js_native_slider_set_channel_fgcolor(handle, fg_color);

    return jerry_create_boolean(result);
}

DECLARE_PUBLIC_HANDLER(js_slider_get_channel_fgcolor)
{
    JERRY_CHECK_ARGS(1, number);
    uint32 handle = jerry_get_number_value(args_p[0]);

    uint16 fg_color = js_native_slider_get_channel_fgcolor(handle);

    return jerry_create_number(fg_color);
}

// Thumb support
DECLARE_PUBLIC_HANDLER(js_slider_set_thumb_size)
{
    JERRY_CHECK_ARGS(3, number, number, number);
    uint32 handle = jerry_get_number_value(args_p[0]);
    uint32 width = jerry_get_number_value(args_p[1]);
    uint32 height = jerry_get_number_value(args_p[2]);

    BOOLEAN result =
        js_native_slider_set_thumb_size(handle, (int16)width, (int16)height);

    return jerry_create_boolean(result);
}

DECLARE_PUBLIC_HANDLER(js_slider_get_thumb_width)
{
    JERRY_CHECK_ARGS(1, number);
    uint32 handle = jerry_get_number_value(args_p[0]);

    int16 width = 0;
    int16 height = 0;
    js_native_slider_get_thumb_size(handle, &width, &height);

    return jerry_create_number(width);
}

DECLARE_PUBLIC_HANDLER(js_slider_get_thumb_height)
{
    JERRY_CHECK_ARGS(1, number);
    uint32 handle = jerry_get_number_value(args_p[0]);

    int16 width = 0;
    int16 height = 0;
    js_native_slider_get_thumb_size(handle, &width, &height);

    return jerry_create_number(height);
}

DECLARE_PUBLIC_HANDLER(js_slider_set_need_thumb)
{
    JERRY_CHECK_ARGS(2, number, boolean);
    uint32 handle = jerry_get_number_value(args_p[0]);
    BOOLEAN is_need = jerry_get_boolean_value(args_p[1]);

    BOOLEAN result = js_native_slider_set_need_thumb(handle, is_need);

    return jerry_create_boolean(result);
}

DECLARE_PUBLIC_HANDLER(js_slider_get_is_need_thumb)
{
    JERRY_CHECK_ARGS(1, number);
    uint32 handle = jerry_get_number_value(args_p[0]);

    BOOLEAN result = js_native_slider_get_is_need_thumb(handle);

    return jerry_create_boolean(result);
}

DECLARE_PUBLIC_HANDLER(js_slider_set_thumb_color)
{
    JERRY_CHECK_ARGS(2, number, number);
    uint32 handle = jerry_get_number_value(args_p[0]);
    uint32 thumb_color = jerry_get_number_value(args_p[1]);

    BOOLEAN result = js_native_slider_set_thumb_color(handle, thumb_color);

    return jerry_create_boolean(result);
}

DECLARE_PUBLIC_HANDLER(js_slider_get_thumb_color)
{
    JERRY_CHECK_ARGS(1, number);
    uint32 handle = jerry_get_number_value(args_p[0]);

    uint16 thumb_color = js_native_slider_get_thumb_color(handle);

    return jerry_create_number(thumb_color);
}

PUBLIC void js_register_slider_functions()
{
    REGISTER_HANDLER(js_slider_create);
    REGISTER_HANDLER(js_slider_destory);

    REGISTER_HANDLER(js_slider_setmin);
    REGISTER_HANDLER(js_slider_getmin);
    REGISTER_HANDLER(js_slider_setmax);
    REGISTER_HANDLER(js_slider_getmax);

    REGISTER_HANDLER(js_slider_set_step);
    REGISTER_HANDLER(js_slider_get_step);
    REGISTER_HANDLER(js_slider_set_display_type);
    REGISTER_HANDLER(js_slider_get_display_type);
    REGISTER_HANDLER(js_slider_set_cur_value);
    REGISTER_HANDLER(js_slider_get_cur_value);
    // channel
    REGISTER_HANDLER(js_slider_set_channel_size);
    REGISTER_HANDLER(js_slider_get_channel_size);
    REGISTER_HANDLER(js_slider_set_channel_bgcolor);
    REGISTER_HANDLER(js_slider_get_channel_bgcolor);
    REGISTER_HANDLER(js_slider_set_channel_fgcolor);
    REGISTER_HANDLER(js_slider_get_channel_fgcolor);

    // thumb
    REGISTER_HANDLER(js_slider_set_thumb_size);
    REGISTER_HANDLER(js_slider_get_thumb_width);
    REGISTER_HANDLER(js_slider_get_thumb_height);
    REGISTER_HANDLER(js_slider_set_need_thumb);
    REGISTER_HANDLER(js_slider_get_is_need_thumb);
    REGISTER_HANDLER(js_slider_set_thumb_color);
    REGISTER_HANDLER(js_slider_get_thumb_color);
}
/**---------------------------------------------------------------------------*
 * **                         Compiler Flag *
 * **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
