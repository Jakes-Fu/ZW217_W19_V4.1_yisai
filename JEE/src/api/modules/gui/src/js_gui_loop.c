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
  * @file     js_gui_loop.c
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
#include "js_event.h"

LOCAL jerry_value_t parseEvent(jerry_value_t viewObj, BaseEvent* pEvent) {
    if (pEvent == NULL) return jerry_create_undefined();
    switch (pEvent->type) {
    case EVENT_TOUCHSTART:
    case EVENT_TOUCHMOVE:
    case EVENT_TOUCHCANCEL:
    case EVENT_TOUCHEND:
    {
        return makeTouchEvent(viewObj, (TouchEvent*)pEvent);
    }
    break;
    case EVENT_CLICK:
    case EVENT_LONGPRESS:
    case EVENT_FOCUS:
    case EVENT_BLUR:
    case EVENT_PAGESHOW:
    case EVENT_PAGEHIDE:
    case EVENT_UNKNOWN:
    {
        return makeBaseEvent(viewObj, NULL, pEvent);
    }
    break;
    case EVENT_KEY:
    {
        return makeKeyEvent(viewObj, (KeyEvent*)pEvent);
    }
    break;
    case EVENT_SWIPE:
    {
        return makeSwipeEvent(viewObj, (SwipeEvent*)pEvent);
    }
    break;
    case EVENT_CHANGE:
    {
        return makeChangeEvent(viewObj, (ChangeEvent*)pEvent);
    }
    break;
    default:
        return jerry_create_undefined();
        break;
    }

    return jerry_create_undefined();
}

int js_gui_component_eventloop(jerry_value_t this_value, BaseEvent* pEvent) {
    jerry_value_t func_name = jerry_create_string((jerry_char_t*)"eventLoop");
    jerry_value_t event_func = jerry_get_property((jerry_value_t)this_value, func_name);

    jerry_value_t obj_event = parseEvent(this_value, pEvent);
    jerry_value_t call_func = jerry_call_function(event_func, this_value, &obj_event, 1);
    /* Check the call function return value if there is any error */
    if (TRUE == jerry_value_is_error(call_func))
    {
        SCI_TRACE_LOW("js_gui_component_eventloop call function error is %d\n", jerry_get_error_type(call_func));
    }
    jerry_release_value(call_func);
    jerry_release_value(func_name);
    jerry_release_value(event_func);
    jerry_release_value(obj_event);
}

int js_gui_eventloop(jerry_value_t this_value, BaseEvent* pEvent) {
    return js_gui_component_eventloop(this_value, pEvent);
}