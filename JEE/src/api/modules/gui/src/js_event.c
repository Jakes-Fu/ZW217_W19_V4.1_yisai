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
  * @file     js_event.c
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
#include "js_event.h"
#include "os_api.h"
#include "urjs_art.h"

const EVENT_NAME s_js_event_name[] = {
    {EVENT_TOUCHSTART, "touchstart"},
    {EVENT_TOUCHMOVE, "touchmove"},
    {EVENT_TOUCHCANCEL, "touchcancel"},
    {EVENT_TOUCHEND, "touchend"},
    {EVENT_CLICK, "click"},
    {EVENT_LONGPRESS, "longpress"},
    {EVENT_FOCUS, "focus"},
    {EVENT_BLUR, "blur"},
    {EVENT_KEY, "key"},
    {EVENT_SWIPE, "swipe"},
    {EVENT_CHANGE, "change"},
    {EVENT_PAGESHOW, "show"},
    {EVENT_PAGEHIDE, "hide"},

    {EVENT_UNKNOWN, "unknown"}
};

jerry_value_t constructObject(jerry_value_t viewObj, const jerry_char_t* name) {
    if (name == NULL) {
        return jerry_create_error(JERRY_ERROR_COMMON, "construct is not exist");
    }

    //jerry_value_t global_object = jerry_get_global_object();
    jerry_value_t construct_name = jerry_create_string((const jerry_char_t*)name);
    jerry_value_t construct_obj = jerry_get_property(viewObj, construct_name);
    jerry_value_t object = jerry_call_function(construct_obj, viewObj, NULL, 0);
    jerry_release_value(construct_obj);
    jerry_release_value(construct_name);

    //jerry_release_value(global_object);

    return object;
}

/*
typedef struct _BaseEvent{
    EVENT_TYPE type;
    uint32_t   timestamp;
    uint32_t   deviceId;
}BaseEvent;
*/

jerry_value_t makeBaseEvent(jerry_value_t viewObj, jerry_value_t object, BaseEvent* pEvent) {
    if (object == NULL) {
        object = constructObject(viewObj, "BaseEvent");
    }

    jerry_value_t prop_value = jerry_create_string((const jerry_char_t*)s_js_event_name[pEvent->type].name);
    jerry_value_t prop_name = jerry_create_string((const jerry_char_t*)"type");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    prop_value = jerry_create_number(pEvent->timestamp);
    prop_name = jerry_create_string((const jerry_char_t*)"timestamp");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    prop_value = jerry_create_number(pEvent->deviceId);
    prop_name = jerry_create_string((const jerry_char_t*)"deviceId");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    return object;
}

/*
typedef struct _TouchInfo{
    int globalX;
    int globalY;
    int localX;
    int localY;
    int size;
    int force;
}TouchInfo;
*/

jerry_value_t makeTouchInfo(TouchInfo* pEvent) {
    jerry_value_t object = jerry_create_object();
    jerry_value_t prop_value = jerry_create_number(pEvent->globalX);
    jerry_value_t prop_name = jerry_create_string((const jerry_char_t*)"globalX");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    prop_value = jerry_create_number(pEvent->globalY);
    prop_name = jerry_create_string((const jerry_char_t*)"globalY");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    prop_value = jerry_create_number(pEvent->localX);
    prop_name = jerry_create_string((const jerry_char_t*)"localX");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    prop_value = jerry_create_number(pEvent->localY);
    prop_name = jerry_create_string((const jerry_char_t*)"localY");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    prop_value = jerry_create_number(pEvent->size);
    prop_name = jerry_create_string((const jerry_char_t*)"size");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    prop_value = jerry_create_number(pEvent->force);
    prop_name = jerry_create_string((const jerry_char_t*)"force");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    return object;
}

/*
typedef struct _TouchEvent(){
    BaseEvent base;
    TouchInfo touches[TOUCHEVENT_LENGTH_MAX];
    TouchInfo changedTouches[TOUCHEVENT_LENGTH_MAX];
}TouchEvent;
*/
jerry_value_t makeTouchEvent(jerry_value_t viewObj, TouchEvent* pEvent) {
    int i = 0;
    jerry_value_t prop_name, prop_value;

    //construct TouchEvent
    jerry_value_t object = constructObject(viewObj, "TouchEvent");
    object = makeBaseEvent(viewObj, object, &pEvent->base);

    prop_value = jerry_create_array(sizeof(pEvent->touches) / sizeof(TouchInfo));
    for (i = 0; i < sizeof(pEvent->touches) / sizeof(TouchInfo); i++) {
        jerry_value_t ti = makeTouchInfo(&pEvent->touches[i]);
        jerry_release_value(jerry_set_property_by_index(prop_value, i, ti));
        jerry_release_value(ti);
    }
    prop_name = jerry_create_string((const jerry_char_t*)"touches");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    prop_value = jerry_create_array(sizeof(pEvent->changedTouches) / sizeof(TouchInfo));
    for (i = 0; i < sizeof(pEvent->changedTouches) / sizeof(TouchInfo); i++) {
        jerry_value_t ti = makeTouchInfo(&pEvent->changedTouches[i]);
        jerry_release_value(jerry_set_property_by_index(prop_value, i, ti));
        jerry_release_value(ti);
    }
    prop_name = jerry_create_string((const jerry_char_t*)"changedTouches");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    return object;
}

/*
typedef struct _KeyEvent(){
    BaseEvent base;
    int code;
    int action;
    int repeatCount;
    int timestampStart0;
}KeyEvent;
*/
jerry_value_t makeKeyEvent(jerry_value_t viewObj, KeyEvent* pEvent) {
    jerry_value_t prop_name, prop_value;

    //construct KeyEvent
    jerry_value_t object = constructObject(viewObj, "KeyEvent");
    object = makeBaseEvent(viewObj, object, &pEvent->base);

    prop_value = jerry_create_number(pEvent->code);
    prop_name = jerry_create_string((const jerry_char_t*)"code");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    prop_value = jerry_create_number(pEvent->action);
    prop_name = jerry_create_string((const jerry_char_t*)"action");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    prop_value = jerry_create_number(pEvent->repeatCount);
    prop_name = jerry_create_string((const jerry_char_t*)"repeatCount");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    prop_value = jerry_create_number(pEvent->timestampStart0);
    prop_name = jerry_create_string((const jerry_char_t*)"timestampStart0");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    return object;
}

/*
typedef struct _SwipeEvent(){
    BaseEvent base;
    char direction[SWIPEEVENT_DIRE_LENGTH];
    int distance;
}SwipeEvent;
*/
jerry_value_t makeSwipeEvent(jerry_value_t viewObj, SwipeEvent* pEvent) {
    jerry_value_t prop_name, prop_value;

    //construct SwipeEvent
    jerry_value_t object = constructObject(viewObj, "SwipeEvent");
    object = makeBaseEvent(viewObj, object, &pEvent->base);

    prop_value = jerry_create_string(pEvent->direction);
    prop_name = jerry_create_string((const jerry_char_t*)"direction");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    prop_value = jerry_create_number(pEvent->distance);
    prop_name = jerry_create_string((const jerry_char_t*)"distance");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    return object;
}

/*
typedef struct _ChangeEvent(){
    BaseEvent base;
    int value;
}ChangeEvent;
*/
jerry_value_t makeChangeEvent(jerry_value_t viewObj, ChangeEvent *pEvent)
{
    jerry_value_t prop_name, prop_value;

    // construct ChangeEvent
    jerry_value_t object = constructObject(viewObj, "ChangeEvent");
    object = makeBaseEvent(viewObj, object, &pEvent->base);

    prop_value = jerry_create_number(pEvent->value);
    prop_name = jerry_create_string((const jerry_char_t *)"value");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    prop_value = jerry_create_string(pEvent->mode);
    prop_name = jerry_create_string((const jerry_char_t *)"mode");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    return object;
}

int js_gui_send_msg(jerry_value_t obj, BaseEvent* event) {
    return js_gui_eventloop(obj, event);
}
