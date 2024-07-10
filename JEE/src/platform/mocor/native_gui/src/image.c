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
 * @file     image.c
 * @brief    -
 * @author   ningbiao.huang@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-27
 * @history
 *****************************************************************************
 **/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_app.h"
#include "mmk_type.h"
#include "caf.h"
#include "ctrlbutton_export.h"
#include "guianim.h"
#include "ctrlbase.h"
#include "mmi_image.h"
#include "urjs_art.h"

#include "button.h"

#ifdef UI_MULTILAYER_SUPPORT
LOCAL GUI_LCD_DEV_INFO s_image_layer = {0};
/*****************************************************************************/
//  Description  :create image layer
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN addImageLayer(uint32 win_handle)
{
    UILAYER_CREATE_T create_info = {0};
    GUI_RECT_T win_rect = {0};
    UILAYER_APPEND_BLT_T append_layer = {0};
    BOOLEAN result = FALSE;
    UILAYER_RESULT_E layer_result = UILAYER_RESULT_SUCCESS;
    MMK_GetWinRect(win_handle, &win_rect);

    if ((s_image_layer.block_id == 0)
        || (s_image_layer.block_id == UILAYER_NULL_HANDLE))
    {
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_handle;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = win_rect.right - win_rect.left + 1;
        create_info.height = win_rect.bottom - win_rect.top + 1;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        UILAYER_CreateLayer(&create_info, &s_image_layer);
        if (UILAYER_RESULT_SUCCESS == layer_result)
        {
            UILAYER_SetLayerColorKey(&s_image_layer, TRUE,
                                     UILAYER_TRANSPARENT_COLOR);
            UILAYER_Clear(&s_image_layer);

            append_layer.layer_level = UILAYER_LEVEL_NORMAL;
            append_layer.lcd_dev_info = s_image_layer;
            UILAYER_AppendBltLayer(&append_layer);
            result = TRUE;
        }
    }
    else
    {
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        append_layer.lcd_dev_info = s_image_layer;
        UILAYER_AppendBltLayer(&append_layer);
        result = TRUE;
    }
    return result;
}
#endif

uint32 js_native_image_create(uint32 this_value, uint32 win_handle, int x,
                              int y, int width, int height)
{
    MMI_HANDLE_T parentHandle = win_handle;
    MMI_CONTROL_CREATE_T create = {0};
    MMI_HANDLE_T handle = NULL;
    MMI_CTRL_ID_T ctrl_id = js_native_gui_lookupCtrlId();

    GUIANIM_INIT_DATA_T data = {0};
    data.both_rect.v_rect.left = x;
    data.both_rect.v_rect.top = y;
    data.both_rect.v_rect.right = x + width;
    data.both_rect.v_rect.bottom = y + height;

    // Use diff create fun by parentHandle.
    if (MMI_HANDLE_CONTROL == MMK_GetHandleType(parentHandle)
        && is_form_ctrl(parentHandle))
    {
        handle = js_native_form_create_child(parentHandle, ctrl_id,
                                             SPRD_GUI_ANIM_ID, &data);
    }
    else
    {

        CTRLBASE_OBJ_T *ctrl_ptr = NULL;

        create.ctrl_id = ctrl_id;
        create.guid = SPRD_GUI_ANIM_ID;
        create.parent_win_handle = win_handle;
        create.parent_ctrl_handle = NULL;
        create.init_data_ptr = &data;

        ctrl_ptr = (CTRLBASE_OBJ_T *)MMK_CreateControl(&create);
        if (PNULL != ctrl_ptr) { handle = ctrl_ptr->handle; }
    }

    CTRLBASE_SetUserData(handle, jerry_acquire_value(this_value));
    CTRLANIM_IsTpEnable(handle, TRUE);

    return handle;
}

void js_native_image_destory(uint32 handle)
{
    js_native_gui_releaseCtrlId(MMK_GetCtrlId(handle));
    jerry_release_value(CTRLBASE_GetUserData(handle));
    MMK_DestroyControl(handle);
}

void js_native_image_set_path(uint32 handle, char *path, int len)
{
    GUIANIM_FILE_INFO_T file_info = {0};
    GUIANIM_CTRL_INFO_T control_info = {0};
    GUIANIM_DISPLAY_INFO_T display_info = {0};
    GUIANIM_DATA_INFO_T anim_data_info = {0};
    char file[MAX_PATH] = {0};

    if (-1 == js_jee_get_native_path(path, file, sizeof(file) - 1))
    {
        aloge("invalidate path: %s", path);
        return;
    }
    len = strlen(file);

    wchar *p_dst = NULL;
    wchar *p_buff = jerry_buffer_allocate((len + 1) * sizeof(wchar));
    if (NULL == p_buff)
    {
        // return jerry_create_null();
        return;
    }
    p_dst = GUI_UTF8ToWstr(p_buff, len, file, len);

    file_info.full_path_wstr_ptr = p_buff;
    file_info.full_path_wstr_len = len;

    control_info.is_ctrl_id = FALSE;
    control_info.ctrl_ptr = GUIANIM_GetPtr(handle);

    display_info.is_update = TRUE;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_disp_one_frame = TRUE;
    display_info.is_crop_rect = TRUE;
    display_info.is_syn_decode = TRUE;
    display_info.is_handle_transparent = TRUE;

    display_info.is_zoom = TRUE;
    display_info.is_auto_zoom_in = TRUE;

    // set bg anim
    GUIANIM_SetParam(&control_info, PNULL, &file_info, &display_info);

    jerry_buffer_release(p_buff);
    p_buff = NULL;
}

void js_native_image_rotate(uint32 handle, int angle)
{
    CTRLANIM_RotateArbitrary(handle, angle, TRUE);
}

void js_native_image_update(uint32 handle)
{
    CTRLANIM_UpdateDisplay(handle);
}

void js_native_image_add_layer(uint32 win_handle, uint32 ctrl_handle)
{
    if (TRUE == addImageLayer(win_handle))
    {
        GUIAPICTRL_SetLcdDevInfo(ctrl_handle, &s_image_layer);
    }
}
void js_native_image_remove_layer(uint32 ctrl_handle)
{
    UILAYER_RELEASELAYER(&s_image_layer);
}
