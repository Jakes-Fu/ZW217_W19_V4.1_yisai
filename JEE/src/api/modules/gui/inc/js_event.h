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
  * @file     js_event.h
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

#ifndef __JS_EVENT_H__
#define __JS_EVENT_H__

#include "sci_types.h"
#include "jerry_bindings.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EVENT_TYPE_NAME_LENGTH  (32)
#define TOUCHEVENT_LENGTH_MAX   (16)
#define SWIPEEVENT_DIRE_LENGTH  (16)
#define CHANGE_EVENT_MAX_LEN  (8)

    typedef enum _EVENT_TYPE {
        EVENT_TOUCHSTART,
        EVENT_TOUCHMOVE,
        EVENT_TOUCHCANCEL,
        EVENT_TOUCHEND,
        EVENT_CLICK,
        EVENT_LONGPRESS,
        EVENT_FOCUS,
        EVENT_BLUR,
        EVENT_KEY,
        EVENT_SWIPE,
        EVENT_CHANGE,
        EVENT_PAGESHOW,
        EVENT_PAGEHIDE,

        EVENT_UNKNOWN
    }EVENT_TYPE;

    typedef struct _EVENT_NAME {
        EVENT_TYPE type;
        char       name[EVENT_TYPE_NAME_LENGTH];
    }EVENT_NAME;

    //define base event
    typedef struct _BaseEvent {
        EVENT_TYPE type;
        uint32   timestamp;
        uint32   deviceId;
    }BaseEvent;

    typedef struct _TouchInfo {
        int globalX;
        int globalY;
        int localX;
        int localY;
        int size;
        int force;
    }TouchInfo;

    typedef struct _TouchEvent {
        BaseEvent base;
        TouchInfo touches[TOUCHEVENT_LENGTH_MAX];
        TouchInfo changedTouches[TOUCHEVENT_LENGTH_MAX];
    }TouchEvent;

    typedef struct _KeyEvent {
        BaseEvent base;
        int code;
        int action;
        int repeatCount;
        int timestampStart0;
    }KeyEvent;

    typedef struct _SwipeEvent {
        BaseEvent base;
        char direction[SWIPEEVENT_DIRE_LENGTH];
        int distance;
    }SwipeEvent;

    typedef struct _ChangeEvent {
        BaseEvent base;
        int value;
        char mode[CHANGE_EVENT_MAX_LEN];
    }ChangeEvent;

    jerry_value_t makeChangeEvent(jerry_value_t viewObj, ChangeEvent* pEvent);
    jerry_value_t makeBaseEvent(jerry_value_t viewObj, jerry_value_t object, BaseEvent* pEvent);
    jerry_value_t makeTouchInfo(TouchInfo* pEvent);
    jerry_value_t makeTouchEvent(jerry_value_t viewObj, TouchEvent* pEvent);
    jerry_value_t makeKeyEvent(jerry_value_t viewObj, KeyEvent* pEvent);
    jerry_value_t makeSwipeEvent(jerry_value_t viewObj, SwipeEvent* pEvent);

    int js_gui_send_msg(jerry_value_t obj, BaseEvent* event);


#ifdef __cplusplus
}
#endif

#endif // __JS_EVENT_H__
