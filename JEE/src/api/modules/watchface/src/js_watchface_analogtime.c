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
  * @file     js_watchface_analogtime.c
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

#include "js_watchface_analogtime.h"

DECLARE_PUBLIC_HANDLER(js_watchface_analogtime_create) {
    JERRY_CHECK_ARGS(9, object, number, number, number, string, string, string, number, number);
    int img_handle = NULL;
    jerry_value_t obj = JERRY_GET_ARG(0, object);
    int page_handle = jerry_get_number_value(args_p[1]);
    double x = jerry_get_number_value(args_p[2]);
    double y = jerry_get_number_value(args_p[3]);
    jerry_string_t hour = JERRY_GET_ARG(4, string);
    jerry_string_t minute = JERRY_GET_ARG(5, string);
    jerry_string_t second = JERRY_GET_ARG(6, string);
    int update_internal = jerry_get_number_value(args_p[7]);
    float scale = jerry_get_number_value(args_p[8]);


    img_handle = js_native_watchface_analogtime_create(obj, page_handle, x, y,
        jerry_string_data(&hour),
        jerry_string_data(&minute),
        jerry_string_data(&second),
        update_internal, scale);

    jerry_string_destroy(&hour);
    jerry_string_destroy(&minute);
    jerry_string_destroy(&second);

    return jerry_create_number(img_handle);
}

DECLARE_PUBLIC_HANDLER(js_watchface_analogtime_delete) {
    JERRY_CHECK_ARGS(1, number);
    int img_handle = jerry_get_number_value(args_p[0]);
    js_native_watchface_analogtime_destory(img_handle);

    return jerry_create_null();
}
