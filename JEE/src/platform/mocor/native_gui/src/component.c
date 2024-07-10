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
  * @file     component.c
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
#include "guictrl.h"
#include "ctrlbase_export.h"
#include "urjs_art.h"
#include "component.h"
#include "guibutton.h"
#include "page.h"

#include "form.h"

void js_native_component_set_rect(uint32 handle, int x, int y, int width,
    int height)
{
    if (handle != 0)
    {
        GUI_RECT_T rt = { 0 };
        rt.left = x;
        rt.top = y;
        rt.right = x + width;
        rt.bottom = y + height;

        if (MMI_HANDLE_WINDOW == MMK_GetHandleType(handle))
        {
            MMK_SetWinRect(handle, &rt);
        }
        else if (MMI_HANDLE_CONTROL == MMK_GetHandleType(handle))
        {
            GUICTRL_SetRect(MMK_GetCtrlPtr(handle), &rt);
        }

        // TODO:May export for independent func.
        // MMK_UpdateScreen();
    }
}

void js_native_component_set_bg_color(uint32 handle, uint32 color)
{
    GUI_BG_T bg = { 0 };
    bg.bg_type = GUI_BG_COLOR;
    bg.color = color;
    if (handle > 0)
    {
        if (MMI_HANDLE_WINDOW == MMK_GetHandleType(handle))
        {
            js_native_page_data* data = NULL;
            alogi("set window background color: 0x%x", color);
            data = MMK_GetWinUserData(handle);
            if (data == NULL)
            {
                data = SCI_ALLOC_APP(sizeof(js_native_page_data));
            }
            if (data == NULL)
            {
                return;
            }
            data->uc.bg_color = color;
            MMK_SetWinUserData(handle, data);
        }
        else
        {
            CAF_GUID_T guid = GUICTRL_GetCtrlGuid(MMK_GetCtrlPtr(handle));
            if (guid == SPRD_GUI_BUTTON_ID)
            {
                GUIBUTTON_SetBg(handle, &bg);
            }
            else
            {
                GUICTRL_SetBg(MMK_GetCtrlPtr(handle), &bg);
            }
        }

        // MMK_UpdateScreen();
    }
}

void js_native_component_set_text_color(uint32 handle, uint32 color)
{
    if (handle > 0)
    {
        if (MMI_HANDLE_WINDOW == MMK_GetHandleType(handle))
        {
            js_native_page_data* data = NULL;
            alogi("set window text color: 0x%x", color);
            data = MMK_GetWinUserData(handle);
            if (data == NULL)
            {
                // tbd :memleak?
                data = art_malloc(sizeof(js_native_page_data));
            }
            if (data == NULL)
            {
                return;
            }
            data->uc.text_color = color;
            MMK_SetWinUserData(handle, data);
        }
        else
        {
            CAF_GUID_T guid = GUICTRL_GetCtrlGuid(MMK_GetCtrlPtr(handle));
            if (guid == SPRD_GUI_BUTTON_ID)
            {
                GUIBUTTON_SetFontColor(handle, color);
            }
        }

        // MMK_UpdateScreen();
    }
}

void js_native_component_set_padding(uint32 handle, uint32 padding_left,
    uint32 padding_top, uint32 padding_right,
    uint32 padding_bottom)
{
    if (handle != 0)
    {
        GUI_PADDING_T padding = { 0 };
        padding.left = padding_left;
        padding.top = padding_top;
        padding.right = padding_right;
        padding.bottom = padding_bottom;

        if (MMI_HANDLE_WINDOW == MMK_GetHandleType(handle))
        {
            // Do nothing
        }
        else if (MMI_HANDLE_CONTROL == MMK_GetHandleType(handle))
        {
            CTRLBASE_SetPadding(handle, &padding);
        }
    }
}

void js_native_component_set_disable(uint32 handle, BOOLEAN value) {
    if(handle == 0){
      return ;
    }

    if (MMI_HANDLE_WINDOW == MMK_GetHandleType(handle)) {
        aloge("set window disable");
    }
    else if (MMI_HANDLE_CONTROL == MMK_GetHandleType(handle)) {
        CTRLBASE_SetEnabled(handle, !value);
        GUICTRL_Redraw(MMK_GetCtrlPtr(handle));
    }
}

void js_native_component_set_focusable(uint32 handle, BOOLEAN value) {
    if(handle == 0){
      return ;
    }

    if (MMI_HANDLE_WINDOW == MMK_GetHandleType(handle)) {
        aloge("set window focusable");
    }
    else if (MMI_HANDLE_CONTROL == MMK_GetHandleType(handle)) {
        CTRLBASE_SetCanActive(handle, value);
        GUICTRL_Redraw(MMK_GetCtrlPtr(handle));
    }
}

void js_native_component_set_visibility(uint32 handle, BOOLEAN value) {
    if(handle == 0){
      return ;
    }

    if (MMI_HANDLE_WINDOW == MMK_GetHandleType(handle)) {
        aloge("set window visibility");
    }
    else if (MMI_HANDLE_CONTROL == MMK_GetHandleType(handle)) {
        CTRLBASE_SetVisible(handle, value);
        GUICTRL_Redraw(MMK_GetCtrlPtr(handle));
    }
}