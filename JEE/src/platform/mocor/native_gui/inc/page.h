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
  * @file     page.h
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


#ifndef __ADAPTOR_PAGE_H__
#define __ADAPTOR_PAGE_H__

#include "sci_types.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct _PAGE_UCSS
    {
        uint32 bg_color;
        uint32 text_color;
    } page_ucss;

    typedef struct _PAGE_DATA
    {
        page_ucss uc;
    } js_native_page_data;

    LOCAL MMI_RESULT_E js_native_gui_win_func(MMI_WIN_ID_T win_id,
        MMI_MESSAGE_ID_E msg_id,
        DPARAM param);
    uint32 js_native_page_create(uint32 this_value, char* type, char* title);
    void js_native_page_set_move_style(uint32 handle, char* move_style);
    void js_native_page_destroy(uint32 handle);

#ifdef __cplusplus
}
#endif

#endif // __ADAPTOR_PAGE_H__