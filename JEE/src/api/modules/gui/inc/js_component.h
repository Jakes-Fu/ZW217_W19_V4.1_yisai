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
  * @file     js_component.h
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

#ifndef __JS_COMPONENT_H__
#define __JS_COMPONENT_H__

#include "jerry_bindings.h"
#include "js_event.h"

#ifdef __cplusplus
extern "C" {
#endif

    DECLARE_PUBLIC_HANDLER(js_component_set_rect);
    DECLARE_PUBLIC_HANDLER(js_component_set_bg_color);
    DECLARE_PUBLIC_HANDLER(js_component_set_text_color);
    DECLARE_PUBLIC_HANDLER(js_component_set_disable);
    DECLARE_PUBLIC_HANDLER(js_component_set_focusable);
    DECLARE_PUBLIC_HANDLER(js_component_set_visibility);

    //Set property
    DECLARE_PUBLIC_HANDLER(js_component_set_property);

    PUBLIC int js_component_get_prop_int(jerry_value_t object, char* prop);
    PUBLIC char* js_component_get_prop_string(jerry_value_t object, char* prop);


#ifdef __cplusplus
}
#endif

#endif // __JS_COMPONENT_H__