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
 * @file     button.c
 * @brief    -
 * @author   ningbiao.huang@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-27
 * @history
 *****************************************************************************
 **/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_app.h"
#include "mmk_type.h"
#include "caf.h"
#include "ctrlbutton_export.h"
#include "mmi_image.h"
#include "guigraph.h"
#include "urjs_art.h"

#include "button.h"
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
uint32 js_native_button_create(uint32 this_value, uint32 parent_handle, int x,
                               int y, int width, int height)
{
    MMI_HANDLE_T parentHandle = parent_handle;
    MMI_HANDLE_T handle = NULL;
    MMI_CTRL_ID_T ctrl_id = js_native_gui_lookupCtrlId();
    GUIBUTTON_INIT_DATA_T data = {0};
    data.both_rect.v_rect.left = x;
    data.both_rect.v_rect.top = y;
    data.both_rect.v_rect.right = x + width;
    data.both_rect.v_rect.bottom = y + height;
    data.bg.bg_type = GUI_BG_COLOR;
    data.bg.color = 0x0000;

    // Use diff create fun by parentHandle.
    if (MMI_HANDLE_CONTROL == MMK_GetHandleType(parentHandle)
        && is_form_ctrl(parentHandle))
    {
        GUIBUTTON_INIT_DATA_T init_data = {0};
        init_data.bg.bg_type = GUI_BG_COLOR;
        init_data.bg.color = 0x0000;
        handle = js_native_form_create_child(parentHandle, ctrl_id,
                                             SPRD_GUI_BUTTON_ID, &init_data);
    }
    else
    {
        handle = CTRLBUTTON_CreateDynamic(parentHandle, ctrl_id, &data);
    }

    CTRLBASE_SetUserData(handle, jerry_acquire_value(this_value));

    return handle;
}

void js_native_button_destory(uint32 handle)
{
    js_native_gui_releaseCtrlId(MMK_GetCtrlId(handle));
    jerry_release_value(CTRLBASE_GetUserData(handle));
    MMK_DestroyControl(handle);
}

void js_native_button_set_text(uint32 handle, char *text, int len)
{
    MMI_STRING_T text_ptr = {0};
    wchar *p_dst = NULL;
    wchar *p_buff = jerry_buffer_allocate((len + 1) * sizeof(wchar));
    if (NULL == p_buff)
    {
        // return jerry_create_null();
        return;
    }
    p_dst = GUI_UTF8ToWstr(p_buff, len, text, len);
    text_ptr.wstr_ptr = p_buff;
    text_ptr.wstr_len = len;

    CTRLBUTTON_SetText(handle, text_ptr.wstr_ptr, len);
    jerry_buffer_release(p_buff);
    p_buff = NULL;

    MMK_UpdateScreen();
}

void js_native_button_set_text_color(uint32 handle, int value)
{
    CAF_COLOR_T color = RGB8882RGB565(value);
    CTRLBUTTON_SetFontColor(handle, color);
}

void js_native_button_set_font_size(uint32 handle, int size)
{
    GUI_FONT_T font = CAF_FONT_30;
    if (size == 16)
    {
        font = CAF_FONT_30;
    }
    else if (size == 18)
    {
        font = CAF_FONT_32;
    }
    else
    {
        aloge("invalidate font size: %d", size);
        return;
    }

    CTRLBUTTON_SetFontSize(handle, font);
}

void js_native_button_set_text_align(uint32 handle, char *text)
{
    GUI_ALIGN_E al = ALIGN_HVMIDDLE;
    if (stricmp(text, "left") == 0)
    {
        al = ALIGN_LVMIDDLE;
    }
    else if (stricmp(text, "right") == 0)
    {
        al = ALIGN_RVMIDDLE;
    }
    else if (stricmp(text, "center") == 0)
    {
        al = ALIGN_HVMIDDLE;
    }
    else
    {
        aloge("invalidate text align: %s", text);
        return;
    }

    CTRLBUTTON_SetTextAlign(handle, al);
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif