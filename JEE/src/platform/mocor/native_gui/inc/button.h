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
 * @file     button.h
 * @brief    -
 * @author   ningbiao.huang@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-27
 * @history
 *****************************************************************************
 **/

#ifndef __ADAPTOR_BUTTON_H__
#define __ADAPTOR_BUTTON_H__

#include "sci_types.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32 js_native_button_create(uint32 this_value, uint32 win_handle, int x,
                               int y, int width, int height);
void js_native_button_destory(uint32 handle);
void js_native_button_set_text(uint32 handle, char *text, int len);
void js_native_button_set_text_color(uint32 handle, int value);
void js_native_button_set_font_size(uint32 handle, int size);
void js_native_button_set_text_align(uint32 handle, char *text);
#ifdef __cplusplus
}
#endif

#endif // __ADAPTOR_BUTTON_H__