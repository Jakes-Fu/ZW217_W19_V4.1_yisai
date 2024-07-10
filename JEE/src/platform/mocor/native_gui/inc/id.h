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
  * @file     id.h
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

#ifndef __ADAPTOR_ID_H__
#define __ADAPTOR_ID_H__

#include "sci_types.h"
#include "mmi_id.h"

#ifdef __cplusplus
extern "C" {
#endif


#define MMI_MODULE_JS_WIN_ID_START      (JS_WIN_BEGIN_ID)
#define MMI_MODULE_JS_WIN_ID_SIZE       (JS_WIN_ID_NUM_MAX)
#define MMI_MODULE_JS_WIN_ID_MAX        (JS_WIN_END_ID)
#define MMI_MOUULE_JS_CTRL_ID_START     (JS_CTRL_BEGIN_ID)
#define MMI_MOUULE_JS_CTRL_ID_SIZE      (JS_CTRL_ID_NUM_MAX)
#define MMI_MOUULE_JS_CTRL_ID_MAX       (JS_CTRL_END_ID)

    PUBLIC int js_native_gui_lookupWinId();
    PUBLIC int js_native_gui_lookupCtrlId();
    PUBLIC void js_native_gui_releaseWinId(int winId);
    PUBLIC void js_native_gui_releaseCtrlId(int ctrlId);
    PUBLIC void js_native_gui_releaseALLId(void);

#ifdef __cplusplus
}
#endif

#endif // __ADAPTOR_ID_H__