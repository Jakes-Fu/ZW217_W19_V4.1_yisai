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
/*****************************************************************************
** File Name:      js_text.c *
** Author:         miao.liu2 *
** Date:           11/10/2021 *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved. *
** Description:    This file is used to describe punctuation                  *
******************************************************************************
**                         Important Edit History *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION *
** 11/2021       miao.liu2          Create *
******************************************************************************/
#include "js_text.h"
#include "mmk_app.h"
#include "ctrlbase.h"
#include "ctrllabel_export.h"
#include "mmi_text.h"
#include "jerry_util.h"

#include "form.h"
/*****************************************************************************/
//  Description : create text
//  Parameter: [In] object :text JS object
//             [In] number :page_handle
//             [Out] None
//             [Return] jerry_create_number
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
DECLARE_PUBLIC_HANDLER(js_gui_text_create)
{
    JERRY_CHECK_ARGS(2, object, number);
    jerry_value_t obj = JERRY_GET_ARG(0, object);
    uint32 page_handle = jerry_get_number_value(args_p[1]);
    JS_DEBUG_LOG("[text_creat]:page_handle = %d", page_handle);

    MMI_CONTROL_CREATE_T create = {0};
    MMI_HANDLE_T handle = NULL;
    MMI_CTRL_ID_T ctrl_id = js_native_gui_lookupCtrlId();

    MMI_HANDLE_T parentHandle = page_handle;
    // Use diff create fun by parentHandle.
    if (MMI_HANDLE_CONTROL == MMK_GetHandleType(parentHandle)
        && is_form_ctrl(parentHandle))
    {
        GUILABEL_INIT_DATA_T init_data = {0};
        handle = js_native_form_create_child(parentHandle, ctrl_id,
                                             SPRD_GUI_LABEL_ID, &init_data);
    }
    else
    {
        GUILABEL_INIT_DATA_T data = {0};
        CTRLBASE_OBJ_T *p_ctrl = NULL;

        create.ctrl_id = ctrl_id;
        create.guid = SPRD_GUI_LABEL_ID;
        create.parent_win_handle = page_handle;
        create.parent_ctrl_handle = NULL;
        create.init_data_ptr = &data;

        p_ctrl = (CTRLBASE_OBJ_T *)MMK_CreateControl(&create);
        if (PNULL != p_ctrl)
        {
            handle = p_ctrl->handle;
        }
        else
        {
            JS_DEBUG_LOG("[text_creat]:ctrl_ptr == PNULL");
            js_art_set_force_exit_flag(TRUE);//need exit js
        }
    }

    CTRLBASE_SetUserData(handle, jerry_acquire_value(obj));

    // return handle;
    return jerry_create_number(handle);
}
/*****************************************************************************/
//  Description :destroy
//  Parameter: [In] number :text_handle
//             [Out] None
//             [Return] jerry_create_null
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
DECLARE_PUBLIC_HANDLER(js_gui_text_destroy)
{
    JERRY_CHECK_ARGS(1, number);
    uint32 text_handle = jerry_get_number_value(args_p[0]);
    JS_DEBUG_LOG("[text_destroy]:text_handle = %d", text_handle);

    js_native_gui_releaseCtrlId(MMK_GetCtrlId(text_handle));
    jerry_release_value(CTRLBASE_GetUserData(text_handle));
    MMK_DestroyControl(text_handle);
    //MMK_UpdateScreen();
    return jerry_create_null();
}

/*****************************************************************************/
//  Description :set_string
//  Parameter: [In] number :text_handle
//             [In] string :inout string
//             [In] number :string number
//             [Out] None
//             [Return] jerry_create_null
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
DECLARE_PUBLIC_HANDLER(js_gui_text_set_string)
{
    JERRY_CHECK_ARGS(2, number, string);
    uint32 text_handle = jerry_get_number_value(args_p[0]);
    jerry_string_t content = JERRY_GET_ARG(1, string);
    jerry_size_t size = jerry_string_size(&content);
    JS_DEBUG_LOG("[text_set_string]:string = %s", content.data);
    JS_DEBUG_LOG("[text_set_string]:size = %d", size);

    MMI_STRING_T text_ptr = {0};
    wchar *p_dst = NULL;
    wchar *p_buff = jerry_buffer_allocate((size + 1) * sizeof(wchar));
    if (NULL == p_buff)
    {
        return jerry_create_null();
    }
    p_dst = GUI_UTF8ToWstr(p_buff, size, jerry_string_data(&content), size);
    text_ptr.wstr_ptr = p_buff;
    text_ptr.wstr_len = size;
    CTRLLABEL_SetText(text_handle, &text_ptr, true);
    jerry_buffer_release(p_buff);
    p_buff = NULL;
    jerry_string_destroy(&content);
    return jerry_create_null();
}

/*****************************************************************************/
//  Description :set_font
//  Parameter: [In] number :text_handle
//             [In] number :font_size
//             [In] number :font_color
//             [Out] None
//             [Return] jerry_create_null
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
DECLARE_PUBLIC_HANDLER(js_gui_text_set_font)
{
    JERRY_CHECK_ARGS(3, number, number, number);
    uint32 text_handle = jerry_get_number_value(args_p[0]);
    uint32 font_size = jerry_get_number_value(args_p[1]);
    uint16 font_color = jerry_get_number_value(args_p[2]);
    JS_DEBUG_LOG("[text_set_font]:font_size = %d, font_color = %d", font_size,
                 font_color);

    GUI_FONT_T font = {0};
    font = font_size;
    CTRLLABEL_SetFont(text_handle, font, font_color);
    return jerry_create_null();
}

/*****************************************************************************/
//  Description :set_align
//  Parameter: [In] number :text_handle
//             [In] number :text_align
//             [Out] None
//             [Return] jerry_create_null
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
DECLARE_PUBLIC_HANDLER(js_gui_text_set_align)
{
    JERRY_CHECK_ARGS(2, number, string);
    uint32 text_handle = jerry_get_number_value(args_p[0]);
    jerry_string_t text_align = JERRY_GET_ARG(1, string);
    JS_DEBUG_LOG("[text_set_align]:text_align = %s", text_align.data);

    GUILABEL_ALIGN_E align = GUILABEL_ALIGN_MAX;

    if (PNULL == jerry_string_data(&text_align))
    {
        JS_DEBUG_LOG("[text_set_align]:text_align error");
        return jerry_create_null();
    }
    else if (0
             == MMIAPICOM_StrCmp(jerry_string_data(&text_align),
                                 jerry_string_size(&text_align), "left",
                                 SCI_STRLEN("left")))
    {
        align = GUILABEL_ALIGN_LEFT;
    }
    else if (0
             == MMIAPICOM_StrCmp(jerry_string_data(&text_align),
                                 jerry_string_size(&text_align), "center",
                                 SCI_STRLEN("center")))
    {
        align = GUILABEL_ALIGN_MIDDLE;
    }
    else if (0
             == MMIAPICOM_StrCmp(jerry_string_data(&text_align),
                                 jerry_string_size(&text_align), "right",
                                 SCI_STRLEN("right")))
    {
        align = GUILABEL_ALIGN_RIGHT;
    }
    else
    {
        align = GUILABEL_ALIGN_NONE;
    }
    JS_DEBUG_LOG("[text_set_align]:align = %d", align);
    CTRLLABEL_SetAlign(text_handle, align);
    return jerry_create_null();
}

/*****************************************************************************/
//  Description :set_scroll
//  Parameter: [In] number :text_handle
//             [In] boolean :is_scroll
//             [Out] None
//             [Return] jerry_create_null
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
DECLARE_PUBLIC_HANDLER(js_gui_text_set_scroll)
{
    JERRY_CHECK_ARGS(2, number, boolean);
    uint32 text_handle = jerry_get_number_value(args_p[0]);
    BOOLEAN is_scroll = jerry_get_boolean_value(args_p[1]);
    JS_DEBUG_LOG("[text_set_scroll]:is_scroll = %d", is_scroll);

    CTRLLABEL_SetIsAutoScroll(text_handle, is_scroll);
    return jerry_create_null();
}

/*****************************************************************************/
//  Description :set_ellipsis
//  Parameter: [In] number :text_handle
//             [In] number :is_ellipsis_ex
//             [Out] None
//             [Return] jerry_create_null
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
DECLARE_PUBLIC_HANDLER(js_gui_text_set_ellipsis_ex)
{
    JERRY_CHECK_ARGS(2, number, boolean);
    uint32 text_handle = jerry_get_number_value(args_p[0]);
    BOOLEAN is_ellipsis_ex = jerry_get_boolean_value(args_p[1]);
    JS_DEBUG_LOG("[set_ellipsis_ex]:is_ellipsis_ex = %d", is_ellipsis_ex);

    CTRLLABEL_SetIsEllipsisEx(text_handle, is_ellipsis_ex);
    return jerry_create_null();
}
