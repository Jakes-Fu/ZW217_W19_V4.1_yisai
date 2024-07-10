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
 * @file     js_slider.h
 * @brief    -
 * @author   ningbiao.huang@unisoc.com
 * @version  V1.0.0
 * @date     2022-6-27
 * @history  2022-6-27       ningbiao.huang          Create
 *****************************************************************************
 **/

#ifndef __JS_SLIDER_H__
#define __JS_SLIDER_H__

#include "jerry_bindings.h"

#ifdef __cplusplus
extern "C" {
#endif

DECLARE_PUBLIC_HANDLER(js_slider_create);
DECLARE_PUBLIC_HANDLER(js_slider_destory);

DECLARE_PUBLIC_HANDLER(js_slider_setmin);
DECLARE_PUBLIC_HANDLER(js_slider_getmin);
DECLARE_PUBLIC_HANDLER(js_slider_setmax);
DECLARE_PUBLIC_HANDLER(js_slider_getmax);

DECLARE_PUBLIC_HANDLER(js_slider_set_step);
DECLARE_PUBLIC_HANDLER(js_slider_get_step);
DECLARE_PUBLIC_HANDLER(js_slider_set_display_type);
DECLARE_PUBLIC_HANDLER(js_slider_get_display_type);
DECLARE_PUBLIC_HANDLER(js_slider_set_cur_value);
DECLARE_PUBLIC_HANDLER(js_slider_get_cur_value);
// channel
DECLARE_PUBLIC_HANDLER(js_slider_set_channel_size);
DECLARE_PUBLIC_HANDLER(js_slider_get_channel_size);
DECLARE_PUBLIC_HANDLER(js_slider_set_channel_bgcolor);
DECLARE_PUBLIC_HANDLER(js_slider_get_channel_bgcolor);
DECLARE_PUBLIC_HANDLER(js_slider_set_channel_fgcolor);
DECLARE_PUBLIC_HANDLER(js_slider_get_channel_fgcolor);

// thumb
DECLARE_PUBLIC_HANDLER(js_slider_set_thumb_size);
DECLARE_PUBLIC_HANDLER(js_slider_get_thumb_width);
DECLARE_PUBLIC_HANDLER(js_slider_get_thumb_height);
DECLARE_PUBLIC_HANDLER(js_slider_set_need_thumb);
DECLARE_PUBLIC_HANDLER(js_slider_get_is_need_thumb);
DECLARE_PUBLIC_HANDLER(js_slider_set_thumb_color);
DECLARE_PUBLIC_HANDLER(js_slider_get_thumb_color);

void js_register_slider_functions();

#ifdef __cplusplus
}
#endif

#endif // __JS_SLIDER_H__
