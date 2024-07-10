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
  * @file     dashboard.h
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

#ifndef __ADAPTOR_DASHBOARD_H__
#define __ADAPTOR_DASHBOARD_H__

#include "id.h"
#include "js_event.h"

#ifdef __cplusplus
extern "C" {
#endif

    uint32_t js_native_dashboard_create(uint32_t this_value, uint32_t win_handle, int x, int y, int width, int height);
    void js_native_dashboard_destory(uint32_t handle);

#ifdef __cplusplus
}
#endif

#endif // __ADAPTOR_DASHBOARD_H__