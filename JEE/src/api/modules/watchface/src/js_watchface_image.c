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
  * @file     js_watchface_image.c
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

#include "js_watchface_image.h"

DECLARE_PUBLIC_HANDLER(js_watchface_image_create) {
    JERRY_CHECK_ARGS(6, object, number, number, number, string, number);
    int img_handle = NULL;
    jerry_value_t obj = JERRY_GET_ARG(0, object);
    int page_handle = jerry_get_number_value(args_p[1]);
    double x = jerry_get_number_value(args_p[2]);
    double y = jerry_get_number_value(args_p[3]);
    jerry_string_t img_path = JERRY_GET_ARG(4, string);
    float scale = jerry_get_number_value(args_p[5]);

    img_handle = js_native_watchface_image_create(obj, page_handle, x, y, jerry_string_data(&img_path), scale);

    jerry_string_destroy(&img_path);

    return jerry_create_number(img_handle);
}

DECLARE_PUBLIC_HANDLER(js_watchface_image_delete) {
    JERRY_CHECK_ARGS(1, number);
    int img_handle = jerry_get_number_value(args_p[0]);
    js_native_watchface_image_destory(img_handle);

    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_watchface_image_get_index)
{
    uint32_t cur_wf_idx = 0;
#ifdef DYNAMIC_WATCHFACE_SUPPORT	
    WATCHFACE_Get_CurWatchface_Index(&cur_wf_idx);
#else
	// do sonthing
#endif
    return jerry_create_number(cur_wf_idx);
}