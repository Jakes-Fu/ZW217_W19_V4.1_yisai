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
 * @file     urjs_art_module.c
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

#ifndef _URJS_ART_MODULE_H_
#define _URJS_ART_MODULE_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "sci_types.h"
#include "os_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
**                         Data Structures                                   *
**---------------------------------------------------------------------------*/

enum
{
    JS_APP_SRV = 0x00,
    JS_APP_GUI,
    JS_APP_FEF
};

/**---------------------------------------------------------------------------*
**                         External Functions                                *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    init app page list.
//  Note:
//  Parameter:      guid: app guid
//  Returns:        void
//****************************************************************************/

PUBLIC void js_art_module_init_pages(const char *guid);

/*****************************************************************************/
//  Description:    uninit app page list.
//  Note:
//  Parameter:
//  Returns:        void
//****************************************************************************/

PUBLIC void js_art_module_uninit_pages();

/*****************************************************************************/
//  Description:    get app type
//  Note:
//  Parameter:
//  Returns:        JS_APP_SRV/JS_APP_GUI/JS_APP_FEF
//****************************************************************************/

PUBLIC int js_art_module_get_app_type();

/*****************************************************************************/
//  Description:    get app root page
//  Note:
//  Parameter:
//  Returns:        the page string, like: /pages/index/index
//****************************************************************************/

PUBLIC char *js_art_module_get_app_root_page();

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif
