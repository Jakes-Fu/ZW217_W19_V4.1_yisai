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
  * @file     component.h
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

#ifndef __ADAPTOR_COMPONENT_H__
#define __ADAPTOR_COMPONENT_H__

#include "sci_types.h"

#ifdef __cplusplus
extern "C" {
#endif

    void js_native_component_set_rect(uint32 handle, int x, int y, int width,
        int height);
    void js_native_component_set_bg_color(uint32 handle, uint32 color);
    void js_native_component_set_text_color(uint32 handle, uint32 color);
    void js_native_component_set_disable(uint32 handle, BOOLEAN value);
    void js_native_component_set_focusable(uint32 handle, BOOLEAN value);
    void js_native_component_set_visibility(uint32 handle, BOOLEAN value);

    // padding
    void js_native_component_set_padding(uint32 handle, uint32 padding_left,
        uint32 padding_top, uint32 padding_right,
        uint32 padding_bottom);

#ifdef __cplusplus
}
#endif

#endif // __ADAPTOR_COMPONENT_H__