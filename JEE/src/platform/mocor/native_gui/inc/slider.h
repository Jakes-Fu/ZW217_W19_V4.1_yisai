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
 * @file     slider.h
 * @brief    -
 * @author   ningbiao.huang@unisoc.com
 * @version  V1.0.0
 * @date     2022-6-27
 * @history
 *****************************************************************************
 **/

#ifndef __NATIVE_SLIDER_H__
#define __NATIVE_SLIDER_H__

#include "sci_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

PUBLIC uint32 js_native_slider_create(uint32 this_value, uint32 parent_handle);

PUBLIC void js_native_slider_destory(uint32 handle);

PUBLIC BOOLEAN js_native_slider_setmin(uint32 handle, int16 min);

PUBLIC int16 js_native_slider_getmin(uint32 handle);

PUBLIC BOOLEAN js_native_slider_setmax(uint32 handle, int16 max);

PUBLIC int16 js_native_slider_getmax(uint32 handle);

PUBLIC BOOLEAN js_native_slider_set_step(uint32 handle, uint16 step);

PUBLIC int16 js_native_slider_get_step(uint32 handle);

// display whether vertical
PUBLIC BOOLEAN js_native_slider_set_display_vertical(uint32 handle,
                                                     BOOLEAN is_vertical);

PUBLIC BOOLEAN js_native_slider_get_display_vertical(uint32 handle);

PUBLIC BOOLEAN js_native_slider_set_cur_value(uint32 handle, int16 cur);

PUBLIC int16 js_native_slider_get_cur_value(uint32 handle);

PUBLIC BOOLEAN js_native_slider_set_need_bgcolor(uint32 handle,
                                                 BOOLEAN is_need);

// channel
PUBLIC BOOLEAN js_native_slider_set_channel_size(uint32 handle, uint16 width,
                                                 uint16 height);

PUBLIC BOOLEAN js_native_slider_get_channel_size(uint32 handle, uint16 *width,
                                                 uint16 *height);

PUBLIC BOOLEAN js_native_slider_set_channel_bgcolor(uint32 handle,
                                                    uint16 bg_color);

PUBLIC BOOLEAN js_native_slider_get_channel_bgcolor(uint32 handle);

PUBLIC BOOLEAN js_native_slider_set_channel_fgcolor(uint32 handle,
                                                    uint16 fg_color);

PUBLIC uint16 js_native_slider_get_channel_fgcolor(uint32 handle);

// Thumb support
PUBLIC BOOLEAN js_native_slider_set_thumb_size(uint32 handle, uint16 width,
                                               uint16 height);

PUBLIC BOOLEAN js_native_slider_get_thumb_size(uint32 handle, uint16 *width,
                                               uint16 *height);

PUBLIC BOOLEAN js_native_slider_set_need_thumb(uint32 handle, BOOLEAN is_need);

PUBLIC BOOLEAN js_native_slider_get_is_need_thumb(uint32 handle);

PUBLIC BOOLEAN js_native_slider_set_thumb_color(uint32 handle,
                                                uint16 thumb_color);

PUBLIC uint16 js_native_slider_get_thumb_color(uint32 handle);

#ifdef __cplusplus
}
#endif

#endif // __ADAPTOR_BUTTON_H__