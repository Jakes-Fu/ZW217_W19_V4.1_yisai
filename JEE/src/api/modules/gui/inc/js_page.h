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
  * @file     js_page.h
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

#ifndef __JS_PAGE_H__
#define __JS_PAGE_H__

#include "jerry_bindings.h"

#ifdef __cplusplus
extern "C" {
#endif

    DECLARE_PUBLIC_HANDLER(js_page_create);
    DECLARE_PUBLIC_HANDLER(js_page_set_move_style);
    DECLARE_PUBLIC_HANDLER(js_page_delete);

    DECLARE_PUBLIC_HANDLER(js_print);

#ifdef __cplusplus
}
#endif

#endif // __JS_PAGE_H__