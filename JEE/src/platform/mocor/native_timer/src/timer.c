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
  * @file     timer.c
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

#include "timer.h"
#include "js_timer.h"
#include "mmk_timer.h"

LOCAL LIST_HEAD(s_js_timer_head);

js_timer_t *lookupJSTimer(uint32 id)
{
    struct list_head* find;
    js_timer_t* tm;
    list_for_each(find, &s_js_timer_head) {
        tm = (js_timer_t*)find;
        if (tm->this_value == id) {
            return tm;
        }
    }

    return NULL;
}

LOCAL void TimerCallBack(uint32 time_id, uint32 param) {
    js_timer_t* tm = param;
    if (tm != NULL) {
        js_timer_call(tm->this_value);
    }
}

uint32 js_native_timer_setTimerout(uint32 this_value, uint32 delays) {
    if (delays == 0) {
        js_timer_call(this_value);
        return 0;
    }

    js_timer_t* tm = art_malloc(sizeof(js_timer_t));
    SCI_MEMSET(tm, 0, sizeof(js_timer_t));
    tm->this_value = this_value;
    tm->this_ref  =  jerry_acquire_value(this_value);
    tm->self = tm;

    //do not implement with threadx's timer because js is running in the P_APP thread
    //the callback function will be call with the interrept context if use threadx's timer, so it's too bad.
/*
    tm->time_ptr = SCI_CreateTimer(
        "JS timer",
        TimerCallBack,
        tm,
        delays,
        TRUE);
*/
    tm->time_id = MMK_CreateTimerCallback(delays, TimerCallBack, tm, FALSE);
    if (tm->time_id == 0)
    {
        return 0;
    }
    list_add_tail(&tm->node, &s_js_timer_head);

    return tm->time_id;
}

void js_native_timer_clearTimerout(uint32 timeId) {
    js_timer_t* tm = lookupJSTimer(timeId);
    if (tm != NULL) {
        //SCI_DeleteTimer(timeId);
        MMK_StopTimer(tm->time_id);
        list_remove(tm);
        jerry_release_value(tm->this_ref);
        art_free(tm);
        tm = NULL;
    }
}

uint32 js_native_timer_setInterval(uint32 this_value, uint32 delays) {
    if (delays == 0) {
        js_timer_call(this_value);
        return 0;
    }

    js_timer_t* tm = art_malloc(sizeof(js_timer_t));
    SCI_MEMSET(tm, 0, sizeof(js_timer_t));
    tm->this_value = this_value;
    tm->this_ref = jerry_acquire_value(this_value);
    tm->self = tm;
    /*
        tm->time_id = SCI_CreatePeriodTimer(
            "JS Period timer",
            TimerCallBack,
            tm,
            delays,
            TRUE);
    */
    tm->time_id = MMK_CreateTimerCallback(delays, TimerCallBack, tm, TRUE);
    list_add_tail(&tm->node, &s_js_timer_head);

    return tm->time_id;
}

void js_native_timer_clearInterval(uint32 timeId) {
    js_timer_t* tm = lookupJSTimer(timeId);
    if (tm != NULL) {
        //SCI_DeleteTimer(timeId);
        MMK_StopTimer(tm->time_id);
        list_remove(tm);
        jerry_release_value(tm->this_ref);
        art_free(tm);
        tm = NULL;
    }
}
