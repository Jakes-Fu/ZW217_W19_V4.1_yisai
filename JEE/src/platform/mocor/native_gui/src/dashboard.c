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
  * @file     dashboard.c
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

#include "sci_types.h"
#include "mmk_app.h"
#include "mmk_type.h"
#include "caf.h"
#include "ctrlbutton_export.h"

#include "dashboard.h"

uint32_t js_native_dashboard_create(uint32_t this_value, uint32_t win_handle,
    int x, int y, int width, int height)
{
    MMI_HANDLE_T parentHandle = win_handle;
    MMI_CTRL_ID_T ctrl_id = js_native_gui_lookupCtrlId();
    GUIBUTTON_INIT_DATA_T data = { 0 };
    data.both_rect.v_rect.left = x;
    data.both_rect.v_rect.top = y;
    data.both_rect.v_rect.right = x + width;
    data.both_rect.v_rect.bottom = y + height;

    return CTRLBUTTON_CreateDynamic(parentHandle, ctrl_id, &data);
}

void js_native_dashboard_destory(uint32_t handle)
{
    js_native_gui_releaseCtrlId(MMK_GetCtrlId(handle));
    MMK_DestroyControl(handle);
}
