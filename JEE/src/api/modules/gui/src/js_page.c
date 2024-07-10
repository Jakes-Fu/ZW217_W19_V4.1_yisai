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
  * @file     js_page.c
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

#include "os_api.h"
#include "jerry_bindings.h"

#include "js_page.h"
#include "mmk_app.h"

DECLARE_PUBLIC_HANDLER(js_page_create)
{
    JERRY_CHECK_ARGS(3, object, string, string);
    jerry_value_t obj = JERRY_GET_ARG(0, object);
    jerry_string_t pageType = JERRY_GET_ARG(1, string);
    jerry_string_t pageTitle = JERRY_GET_ARG(2, string);
    int handler = js_native_page_create(obj, jerry_string_data(&pageType),
        jerry_string_data(&pageTitle));
    if (handler != NULL)
    {
        int bg_color = js_component_get_prop_int(obj, "background_color");
        int text_color = js_component_get_prop_int(obj, "color");
        js_native_component_set_bg_color(handler, bg_color);
        js_native_component_set_text_color(handler, text_color);
    }

    jerry_string_destroy(&pageType);
    jerry_string_destroy(&pageTitle);

    return jerry_create_number(handler);
}

DECLARE_PUBLIC_HANDLER(js_page_set_move_style)
{
    JERRY_CHECK_ARGS(2, number, string);

    int handle = jerry_get_number_value(args_p[0]);
    jerry_string_t page_move_style = JERRY_GET_ARG(1, string);
    JS_DEBUG_LOG("[page_move_style]:page_move_style = %s",
        jerry_string_data(&page_move_style));

    WINDOW_MOVE_STYLE move_style = MMI_MAX_HARDWARE_ICON;

    if (PNULL == jerry_string_data(&page_move_style))
    {
        JS_DEBUG_LOG("[page_move_style]:move_style error");
        jerry_string_destroy(&page_move_style);
        return jerry_create_null();
    }
    else if (0
        == MMIAPICOM_StrCmp(jerry_string_data(&page_move_style),
            jerry_string_size(&page_move_style),
            "MOVE_HORIZONTAL_DIRECTION",
            SCI_STRLEN("MOVE_HORIZONTAL_DIRECTION")))
    {
        move_style = MOVE_HORIZONTAL_DIRECTION;
    }
    else if (0
        == MMIAPICOM_StrCmp(jerry_string_data(&page_move_style),
            jerry_string_size(&page_move_style),
            "MOVE_VERTICAL_DIRECTION",
            SCI_STRLEN("MOVE_VERTICAL_DIRECTION")))
    {
        move_style = MOVE_VERTICAL_DIRECTION;
    }
    else if (0
        == MMIAPICOM_StrCmp(jerry_string_data(&page_move_style),
            jerry_string_size(&page_move_style),
            "MOVE_NOT_FULL_SCREEN_WINDOW",
            SCI_STRLEN("MOVE_NOT_FULL_SCREEN_WINDOW")))
    {
        move_style = MOVE_NOT_FULL_SCREEN_WINDOW;
    }
    else if (0
        == MMIAPICOM_StrCmp(jerry_string_data(&page_move_style),
            jerry_string_size(&page_move_style),
            "MOVE_FORBIDDEN",
            SCI_STRLEN("MOVE_FORBIDDEN")))
    {
        move_style = MOVE_FORBIDDEN;
    }
    else
    {
        move_style = MOVE_BY_SETTING;
    }

    JS_DEBUG_LOG("[page_move_style]:move_style = %d", move_style);
    js_native_page_set_move_style(handle, move_style);

    jerry_string_destroy(&page_move_style);

    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_page_delete)
{
    JERRY_CHECK_ARGS(1, number);

    int handle = jerry_get_number_value(args_p[0]);
    js_native_page_destroy(handle);

    return jerry_create_null();
}
