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
 * @file     form.h
 * @brief    -
 * @author   ningbiao.huang@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-27
 * @history
 *****************************************************************************
 **/

#ifndef __ADAPTOR_FORM_H__
#define __ADAPTOR_FORM_H__

#include "sci_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

// flex-direction support param: row,column;
typedef enum
{
    DIV_FLEX_DIRECTION_ROW,
    DIV_FLEX_DIRECTION_COLUMN,
    DIV_FLEX_DIRECTION_MAX
} div_flex_direction_e;

// flex-wrap support param: wrap,nowrap;
typedef enum
{
    DIV_FLEX_WRAP_WRAP,
    DIV_FLEX_WRAP_NOWRAP,
    DIV_FLEX_WRAP_MAX
} div_flex_wrap_e;

// justify-content support param: flex-start,flex-end,center;
typedef enum
{
    DIV_JUSTIFY_CONTENT_FLEX_START,
    DIV_JUSTIFY_CONTENT_FLEX_END,
    DIV_JUSTIFY_CONTENT_CENTER,
    DIV_JUSTIFY_CONTENT_MAX
} div_justify_content_e;

// align-items support param: stretch,flex-start,flex-end,center;
typedef enum
{
    DIV_ALIGN_ITEMS_STRETCH,
    DIV_ALIGN_ITEMS_FLEX_START,
    DIV_ALIGN_ITEMS_FLEX_END,
    DIV_ALIGN_ITEMS_CENTER,
    DIV_ALIGN_ITEMS_MAX
} div_align_items_e;

PUBLIC BOOLEAN is_form_ctrl(uint32 handle);
// child create
PUBLIC uint32 js_native_form_create_child(uint32 parent_form_id,
                                          uint32 child_id, uint32 guid,
                                          void *init_data);

PUBLIC uint32 js_native_form_create(uint32 this_value, uint32 win_handle);
PUBLIC void js_native_form_destory(uint32 form_ctrl_handle);
PUBLIC void js_native_form_add_child(uint32 win_handle, uint32 form_ctrl_handle,
                                     uint32 child_ctrl_handle);
PUBLIC void js_native_form_remove_child(uint32 form_ctrl_handle,
                                        uint32 child_ctrl_handle);

// div property
PUBLIC void
js_native_form_set_flex_direction(uint32 form_ctrl_handle,
                                  div_flex_direction_e flex_direction_);
PUBLIC void js_native_form_set_flex_wrap(uint32 form_ctrl_handle,
                                         div_flex_wrap_e flex_wrap);
PUBLIC void
js_native_form_set_justify_content(uint32 form_ctrl_handle,
                                   div_justify_content_e justify_content);
PUBLIC void js_native_form_set_align_items(uint32 form_ctrl_handle,
                                           div_align_items_e align_items);

// TEST
// void test_js_native_gui_form_add_child();

#ifdef __cplusplus
}
#endif

#endif // __ADAPTOR_BUTTON_H__