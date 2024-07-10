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
  * @file     watchface_image.h
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

#ifndef __ADAPTOR_WATCHFACE_IMAGE_H__
#define __ADAPTOR_WATCHFACE_IMAGE_H__

#include "sci_types.h"

#ifdef __cplusplus
extern "C" {
#endif

    uint32 js_native_watchface_image_create(uint32 this_value, uint32 win_handle, double center_x, double center_y, char* img_path, double scale);
    void js_native_watchface_image_destory(uint32 handle);

#ifdef __cplusplus
}
#endif

#endif // __ADAPTOR_BUTTON_H__