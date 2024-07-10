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
 * @file     slider.c
 * @brief    -
 * @author   ningbiao.huang@unisoc.com
 * @version  V1.0.0
 * @date     2022-6-27
 * @history
 *****************************************************************************
 **/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_app.h"
#include "mmk_type.h"
#include "os_api.h"

#include "ctrlslider_export.h"
#include "slider.h"

// support form child create
#include "form.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

PUBLIC uint32 js_native_slider_create(uint32 this_value, uint32 parent_handle)
{
    MMI_HANDLE_T page_handle = parent_handle;
    MMI_HANDLE_T handle = NULL;
    MMI_CTRL_ID_T ctrl_id = js_native_gui_lookupCtrlId();

    // Use diff create fun by page_handle.
    if (MMI_HANDLE_CONTROL == MMK_GetHandleType(page_handle)
        && is_form_ctrl(page_handle))
    {
        GUISLIDER_INIT_DATA_T init_data = {0};
        handle = js_native_form_create_child(page_handle, ctrl_id,
                                             SPRD_GUI_SLIDER_ID, &init_data);
    }
    else
    {
        handle = CTRLSLIDER_Create(page_handle, NULL, ctrl_id);
        MMK_SetAtvCtrl(page_handle, ctrl_id);
    }
    if (this_value != 0)
    {
        CTRLBASE_SetUserData(handle, jerry_acquire_value(this_value));
    }

    return handle;
}

PUBLIC void js_native_slider_destory(uint32 handle)
{
    js_native_gui_releaseCtrlId(MMK_GetCtrlId(handle));
    jerry_release_value(CTRLBASE_GetUserData(handle));
    MMK_DestroyControl(handle);
}

PUBLIC BOOLEAN js_native_slider_setmin(uint32 handle, int16 min)
{
    return CTRLSLIDER_SetRangeMin(handle, min);
}

PUBLIC int16 js_native_slider_getmin(uint32 handle)
{
    return CTRLSLIDER_GetRangeMin(handle);
}

PUBLIC BOOLEAN js_native_slider_setmax(uint32 handle, int16 max)
{
    return CTRLSLIDER_SetRangeMax(handle, max);
}

PUBLIC int16 js_native_slider_getmax(uint32 handle)
{
    return CTRLSLIDER_GetRangeMax(handle);
}

PUBLIC BOOLEAN js_native_slider_set_step(uint32 handle, uint16 step)
{
    // Step of change value by direction or volume key.
    return CTRLSLIDER_SetLineSize(handle, step);
}

PUBLIC int16 js_native_slider_get_step(uint32 handle)
{
    return CTRLSLIDER_GetLineSize(handle);
}

// display whether vertical
PUBLIC BOOLEAN js_native_slider_set_display_vertical(uint32 handle,
                                                     BOOLEAN is_vertical)
{
    return CTRLSLIDER_SetVerDisp(handle, is_vertical);
}

PUBLIC BOOLEAN js_native_slider_get_display_vertical(uint32 handle)
{
    return CTRLSLIDER_GetVerDisp(handle);
}

PUBLIC BOOLEAN js_native_slider_set_cur_value(uint32 handle, int16 cur)
{
    return CTRLSLIDER_SetSelection(handle, cur);
}

PUBLIC int16 js_native_slider_get_cur_value(uint32 handle)
{
    return CTRLSLIDER_GetSelection(handle);
}

PUBLIC BOOLEAN js_native_slider_set_need_bgcolor(uint32 handle, BOOLEAN is_need)
{
    return CTRLSLIDER_SetNeedBg(handle, is_need);
}

// channel
PUBLIC BOOLEAN js_native_slider_set_channel_size(uint32 handle, uint16 width,
                                                 uint16 height)
{
    return CTRLSLIDER_SetChannelSize(handle, width, height);
}

PUBLIC BOOLEAN js_native_slider_get_channel_size(uint32 handle, uint16 *width,
                                                 uint16 *height)
{
    return CTRLSLIDER_GetChannelSize(handle, width, height);
}

PUBLIC BOOLEAN js_native_slider_set_channel_bgcolor(uint32 handle,
                                                    uint16 bg_color)
{
    return CTRLSLIDER_SetChannelBgColor(handle, bg_color);
}

PUBLIC BOOLEAN js_native_slider_get_channel_bgcolor(uint32 handle)
{
    return CTRLSLIDER_GetChannelBgColor(handle);
}

PUBLIC BOOLEAN js_native_slider_set_channel_fgcolor(uint32 handle,
                                                    uint16 fg_color)
{
    CTRLSLIDER_SetNeedChannelFg(handle, TRUE);
    return CTRLSLIDER_SetChannelFgColor(handle, fg_color);
}

PUBLIC uint16 js_native_slider_get_channel_fgcolor(uint32 handle)
{
    return CTRLSLIDER_GetChannelFgColor(handle);
}

// Thumb support
PUBLIC BOOLEAN js_native_slider_set_thumb_size(uint32 handle, uint16 width,
                                               uint16 height)
{
    return CTRLSLIDER_SetThumbSize(handle, width, height);
}

PUBLIC BOOLEAN js_native_slider_get_thumb_size(uint32 handle, uint16 *width,
                                               uint16 *height)
{
    return CTRLSLIDER_GetThumbSize(handle, width, height);
}

PUBLIC BOOLEAN js_native_slider_set_need_thumb(uint32 handle, BOOLEAN is_need)
{
    return CTRLSLIDER_SetNeedThumb(handle, is_need);
}

PUBLIC BOOLEAN js_native_slider_get_is_need_thumb(uint32 handle)
{
    return CTRLSLIDER_GetIsNeedThumb(handle);
}

PUBLIC BOOLEAN js_native_slider_set_thumb_color(uint32 handle,
                                                uint16 thumb_color)
{
    return CTRLSLIDER_SetThumbColor(handle, thumb_color);
}

PUBLIC uint16 js_native_slider_get_thumb_color(uint32 handle)
{
    return CTRLSLIDER_GetThumbColor(handle);
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif