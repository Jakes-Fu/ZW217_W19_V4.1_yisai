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
  * @file     timer.h
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

#ifndef __ADAPTOR_TIMER_H__
#define __ADAPTOR_TIMER_H__

#include "sci_types.h"
#include "mmk_timer.h"
#include "js_list.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct js_timer_tag {
        struct list_head node;
        void* self;

        uint32 time_id;
        uint32 this_value;
        uint32 this_ref;
    }js_timer_t;

    uint32 js_timer_setTimeout(uint32 this_value, uint32 delays);
    void js_timer_clearTimeout(uint32 timeId);
    uint32 js_timer_setInterval(uint32 this_value, uint32 delays);
    void js_timer_clearInterval(uint32 timeId);

#ifdef __cplusplus
}
#endif

#endif // __ADAPTOR_TIMER_H__