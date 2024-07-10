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
 * @file     js_timer.c
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

#include "os_api.h"
#include "jerry_bindings.h"

#include "js_timer.h"
#include "urjs_art.h"
DECLARE_PUBLIC_HANDLER(js_timer_setTimeout)
{
    JERRY_CHECK_ARGS(2, object, number);
    jerry_value_t obj = JERRY_GET_ARG(0, object);
    int delays = jerry_get_number_value(args_p[1]);

    int timeId = js_native_timer_setTimerout(obj, delays);

    return jerry_create_number(timeId);
}

DECLARE_PUBLIC_HANDLER(js_timer_clearTimeout)
{
    JERRY_CHECK_ARGS(1, object);
    jerry_value_t obj = JERRY_GET_ARG(0, object);

    js_native_timer_clearTimerout(obj);
}

DECLARE_PUBLIC_HANDLER(js_timer_setInterval)
{
    JERRY_CHECK_ARGS(2, object, number);
    jerry_value_t obj = JERRY_GET_ARG(0, object);
    int delays = jerry_get_number_value(args_p[1]);

    int timeId = js_native_timer_setInterval(obj, delays);

    return jerry_create_number(timeId);
}

DECLARE_PUBLIC_HANDLER(js_timer_clearInterval)
{
    JERRY_CHECK_ARGS(1, object);
    jerry_value_t obj = JERRY_GET_ARG(0, object);

    js_native_timer_clearInterval(obj);
}

void js_timer_call(jerry_value_t this_value)
{
    jerry_value_t cb_name = jerry_create_string((jerry_char_t *)"callback");
    jerry_value_t cb_func = jerry_get_property((jerry_value_t)this_value, cb_name);

    jerry_value_t call_func = jerry_call_function(cb_func, this_value, NULL, 0);
    /* Check the call function return value if there is any error */
    if (TRUE == jerry_value_is_error(call_func))
    {
        aloge("js_timer_call call function error is %d\n",
              jerry_get_error_type(call_func));
    }
    jerry_release_value(call_func);
    jerry_release_value(cb_name);
    jerry_release_value(cb_func);
}
