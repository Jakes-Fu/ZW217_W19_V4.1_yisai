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
  * @file     id.c
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
#include "mmi_id.h"

#include "id.h"

LOCAL char s_js_win_id_pool[MMI_MODULE_JS_WIN_ID_SIZE] = { 0 };
LOCAL char s_js_ctrl_id_pool[MMI_MOUULE_JS_CTRL_ID_SIZE] = { 0 };

PUBLIC int js_native_gui_lookupWinId() {
    int winId = -1;
    for (winId = 0; winId < MMI_MODULE_JS_WIN_ID_SIZE; winId++) {
        if (s_js_win_id_pool[winId] == 0) {
            s_js_win_id_pool[winId] = 1;
            break;
        }
    }

    return winId + MMI_MODULE_JS_WIN_ID_START;
}

PUBLIC void js_native_gui_releaseWinId(int winId) {
    winId -= MMI_MODULE_JS_WIN_ID_START;
    if (winId < 0 || winId > MMI_MODULE_JS_WIN_ID_SIZE) return;
    s_js_win_id_pool[winId] = 0;
}

PUBLIC int js_native_gui_lookupCtrlId() {
    int ctrlId = -1;
    for (ctrlId = 0; ctrlId < MMI_MOUULE_JS_CTRL_ID_SIZE; ctrlId++) {
        if (s_js_ctrl_id_pool[ctrlId] == 0) {
            s_js_ctrl_id_pool[ctrlId] = 1;
            break;
        }
    }

    return ctrlId + MMI_MOUULE_JS_CTRL_ID_START;
}

PUBLIC void js_native_gui_releaseCtrlId(int ctrlId) {
    ctrlId -= MMI_MOUULE_JS_CTRL_ID_START;
    if (ctrlId < 0 || ctrlId > MMI_MOUULE_JS_CTRL_ID_SIZE) return;
    s_js_ctrl_id_pool[ctrlId] = 0;
}

PUBLIC void js_native_gui_releaseALLId(void) {
    SCI_MEMSET(s_js_ctrl_id_pool,0,sizeof(char)*MMI_MOUULE_JS_CTRL_ID_SIZE);
    SCI_MEMSET(s_js_win_id_pool,0,sizeof(char)*MMI_MODULE_JS_WIN_ID_SIZE);
}