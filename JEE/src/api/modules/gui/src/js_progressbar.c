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
 * @file     js_progressbar.c
 * @brief    -
 * @author   ningbiao.huang@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-27
 * @history  11/2021       shuting.ma          Create
 * @history  4/2022       ningbiao.huang          update
 *****************************************************************************
 **/

#include "js_progressbar.h"
#include "ctrlbase.h"
#include "guiprgbox.h"
#include "jerry_util.h"
#include "watch_common_progressbar.h"

#include "form.h"

DECLARE_PUBLIC_HANDLER(js_gui_progressbar_create)
{
    JERRY_CHECK_ARGS(2, object, number);
    jerry_value_t obj = JERRY_GET_ARG(0, object);
    uint32 page_handle = jerry_get_number_value(args_p[1]);
    JS_DEBUG_LOG("[progressbar_create]:page_handle = %d", page_handle);

    MMI_CONTROL_CREATE_T create = {0};
    MMI_HANDLE_T progress_handle = NULL;
    GUIPRGBOX_INIT_DATA_T init_data = {0};
    CTRLBASE_OBJ_T *p_ctrl = PNULL;
    MMI_CTRL_ID_T ctrl_id = js_native_gui_lookupCtrlId();

    // Use diff create fun by parentHandle.
    MMI_HANDLE_T parentHandle = page_handle;
    if (MMI_HANDLE_CONTROL == MMK_GetHandleType(parentHandle)
        && is_form_ctrl(parentHandle))
    {
        init_data.is_forbid_paint = FALSE;
        init_data.theme_type = MMITHEME_PRGBOX_TYPE_WIDEBAR_PROGRESS;
        init_data.lcd_id = GUI_MAIN_LCD_ID;
        progress_handle = js_native_form_create_child(
            parentHandle, ctrl_id, SPRD_GUI_PRGBOX_ID, &init_data);
        if (progress_handle != 0)
        {
            GUIAPICTRL_SetState(progress_handle, GUICTRL_STATE_TOPMOST_1, TRUE);
        }
    }
    else
    {
        init_data.is_forbid_paint = FALSE;
        init_data.theme_type = MMITHEME_PRGBOX_TYPE_WIDEBAR_PROGRESS;
        init_data.lcd_id = GUI_MAIN_LCD_ID;

        create.ctrl_id = ctrl_id;
        create.guid = SPRD_GUI_PRGBOX_ID;
        create.parent_win_handle = page_handle;
        create.parent_ctrl_handle = NULL;
        create.init_data_ptr = &init_data;

        p_ctrl = (CTRLBASE_OBJ_T *)MMK_CreateControl(&create);
        if (PNULL != p_ctrl)
        {
            progress_handle = p_ctrl->handle;
            MMK_SetActiveCtrl(progress_handle, TRUE);
            GUIAPICTRL_SetState(progress_handle, GUICTRL_STATE_TOPMOST_1, TRUE);
        }
        else
        {
            JS_DEBUG_LOG("[progressbar_create]:p_ctrl == PNULL");
        }
    }

    CTRLBASE_SetUserData(progress_handle, jerry_acquire_value(obj));

    // return handle;
    return jerry_create_number(progress_handle);
}

DECLARE_PUBLIC_HANDLER(js_gui_progressbar_set_step)
{
    JERRY_CHECK_ARGS(2, number, number);
    uint32 progress_handle = jerry_get_number_value(args_p[0]);
    uint32 step = jerry_get_number_value(args_p[1]);
    uint32 total_item_count = 0;
    JS_DEBUG_LOG("[progressbar_set_step]:step = %d", step);

    if (0 == step)
    {
        total_item_count = 100;
    }
    else
    {
        total_item_count = 100 / step;
    }
    GUIPRGBOX_SetParamByCtrl(progress_handle, total_item_count, 1);
    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_gui_progressbar_set_slide)
{
    JERRY_CHECK_ARGS(2, number, boolean);
    uint32 progress_handle = jerry_get_number_value(args_p[0]);
    BOOLEAN is_slide = jerry_get_boolean_value(args_p[1]);
    JS_DEBUG_LOG("[progressbar_set_slide]:is_slide = %d", is_slide);

    CTRLPROGRESSBAR_SetIsTpEnable(progress_handle, is_slide);
    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_gui_progressbar_set_type)
{
    JERRY_CHECK_ARGS(2, number, string);
    uint32 progress_handle = jerry_get_number_value(args_p[0]);
    jerry_string_t type = JERRY_GET_ARG(1, string);
    JS_DEBUG_LOG("[progressbar_set_type]:string = %s", type.data);

    GUIPRGBOX_STYLE_E progressbar_type = GUIPRGBOX_STYLE_MAX;

    if (PNULL == jerry_string_data(&type))
    {
        JS_DEBUG_LOG("[progressbar_set_type]:type error");
        return jerry_create_null();
    }

    if (0 == strcmp(jerry_string_data(&type), "VERTICAL"))
    {
        progressbar_type = GUIPRGBOX_STYLE_VERTICAL_PROCESS;
    }

    if (0 == strcmp(jerry_string_data(&type), "HORIZONTAL"))
    {
        progressbar_type = GUIPRGBOX_STYLE_HORIZONTAL_PROCESS;
    }

    JS_DEBUG_LOG("[progressbar_set_type]:progressbar_type = s%",
                 progressbar_type);
    CTRLPROGRESSBAR_SetType(progress_handle, progressbar_type);
    return jerry_create_null();
}
DECLARE_PUBLIC_HANDLER(js_gui_progressbar_set_bg_color)
{
    JERRY_CHECK_ARGS(2, number, number);
    uint32 progress_handle = jerry_get_number_value(args_p[0]);
    uint16 prg_bg_color = jerry_get_number_value(args_p[1]);
    JS_DEBUG_LOG("[progressbar_set_bg_color]:prg_bg_color = %d", prg_bg_color);

    CTRLPROGRESSBAR_SetBgColor(progress_handle, prg_bg_color);
    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_gui_progressbar_set_fg_color)
{
    JERRY_CHECK_ARGS(2, number, number);
    uint32 progress_handle = jerry_get_number_value(args_p[0]);
    uint16 prg_fg_color = jerry_get_number_value(args_p[1]);
    JS_DEBUG_LOG("[progressbar_set_fg_color]:prg_fg_color = %d", prg_fg_color);

    CTRLPROGRESSBAR_SetFgColor(progress_handle, prg_fg_color);
    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_gui_progressbar_set_percent)
{
    JERRY_CHECK_ARGS(2, number, number);
    uint32 progress_handle = jerry_get_number_value(args_p[0]);
    uint32 percent = jerry_get_number_value(args_p[1]);
    JS_DEBUG_LOG("[progressbar_set_percent]:percent = %d", percent);

    WatchCOM_Progressbar_Set_Percent(percent, progress_handle);
    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_gui_progressbar_destory)
{
    JERRY_CHECK_ARGS(1, number);
    uint32 progress_handle = jerry_get_number_value(args_p[0]);
    JS_DEBUG_LOG("[progressbar_destory]:progress_handle = %d", progress_handle);

    js_native_gui_releaseCtrlId(MMK_GetCtrlId(progress_handle));
    jerry_release_value(CTRLBASE_GetUserData(progress_handle));
    MMK_DestroyControl(progress_handle);
    return jerry_create_null();
}
