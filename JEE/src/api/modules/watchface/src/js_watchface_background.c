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
  * @file     js_watchface_background.c
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

#include "js_watchface_background.h"
#include "urjs_art.h"

DECLARE_PUBLIC_HANDLER(js_watchface_background_create) {
    JERRY_CHECK_ARGS(7, object, number, number, number, object, number, number);
    int img_handle = NULL;
    jerry_value_t obj = JERRY_GET_ARG(0, object);
    int page_handle = jerry_get_number_value(args_p[1]);
    double x = jerry_get_number_value(args_p[2]);
    double y = jerry_get_number_value(args_p[3]);
    int update_internal = jerry_get_number_value(args_p[5]);
    float scale = jerry_get_number_value(args_p[6]);

    //handle json array [image_list]
    char** pStrArray = NULL;
    uint32_t length = 0;
    if (jerry_value_is_array(args_p[4])) {
        length = jerry_get_array_length(args_p[4]);
        pStrArray = art_malloc((length + 1) * sizeof(char*));
        memset(pStrArray, 0, length + 1);
        for (uint32_t i = 0; i < length; i++) {
            char str_buf[256] = { 0 };
            jerry_value_t item_val = jerry_get_property_by_index(args_p[4], i);
            if (jerry_value_is_string(item_val)) {
                jerry_size_t str_size = jerry_get_utf8_string_size(item_val);
                jerry_size_t string_end = jerry_string_to_utf8_char_buffer(item_val, str_buf, str_size);
                str_buf[string_end] = 0;
                char path[MAX_PATH] = {0};
                if (-1 == js_jee_get_native_path(str_buf, path, sizeof(path) - 1))
                {
                    aloge("invalidate path: %s", str_buf);
                    pStrArray[i] = NULL;
                }else{
                    pStrArray[i] = art_strdup(path);
                    JS_DEBUG_LOG("IMG[%d]:%s \n", i, pStrArray[i]);
                }
            }
            jerry_release_value(item_val);
        }
    }

    img_handle = js_native_watchface_background_create(obj, page_handle, x, y, pStrArray, length, update_internal, scale);

    for (uint32_t i = 0; i < length; i++) {
        if(pStrArray[i] != NULL){
          art_free(pStrArray[i]);
        }
    }
    art_free(pStrArray);
    pStrArray = NULL;

    return jerry_create_number(img_handle);
}

DECLARE_PUBLIC_HANDLER(js_watchface_background_delete) {
    JERRY_CHECK_ARGS(1, number);
    int img_handle = jerry_get_number_value(args_p[0]);
    js_native_watchface_background_destory(img_handle);

    return jerry_create_null();
}
