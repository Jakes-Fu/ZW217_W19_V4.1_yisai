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
 * @file     timer.js
 * @brief    -
 * @author   qingjun.yu@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-12
 * @history
 *****************************************************************************
 **/

_timer_console = require('console');
_timer_console.log("timer.js loading");

function Timeout(handler, args) {
    this.handler = handler;
    this.param = args;
/*
    //this.ref_num = 1;
    this.close = function () {

    };

    this.hasRef = function () {
        return (ref_num > 0);
    };

    this.ref = function () {
        if (ref_num <= 0) {
            this.ref_num = 1;
        }
    };

    this.refresh = function () {

    };

    this.unref = function () {
        if (ref_num > 0) {
            this.ref_num = 0;
        }
    };
*/
    this.callback = function(){
        var arg = this.param;
        this.handler(...arg);
    }
}

function setTimeout(handler, delays) {
    if (arguments.length <= 0) {
        console.log("setTimeout need param!");
        return;
    }

    if (typeof handler !== "function") {
        console.log("invalidate  handler: need function");
        return;
    }

    var args = Array.from(arguments);
    //param:delays may be equ null, so we need default delays = 1ms
    var dly = 1;
    if (args.length >= 2) {
        dly = args[1];
        args.splice(0, 1);
    }
    if (args.length >= 3) {
        args.splice(0, 1);
    }

    var tm = new Timeout(handler, args);
    js_timer_setTimeout(tm, dly);

    return tm;
}

function clearTimeout(timeout) {
    if (timeout !== null) {
        js_timer_clearTimeout(timeout);
    }
}

function setInterval(handler, delays) {
    if (arguments.length <= 0) {
        console.log("setInterval need param!");
        return;
    }

    if (typeof handler !== "function") {
        console.log("invalidate  handler: need function");
        return;
    }

    var args = Array.from(arguments);
    var dly = 1;
    if (args.length >= 2) {
        dly = args[1];
        args.splice(0, 1);
    }
    if (args.length >= 3) {
        args.splice(0, 1);
    }

    var tm = new Timeout(handler, args);
    js_timer_setInterval(tm, dly);

    return tm;
}

function clearInterval(timeout) {
    if (timeout !== null) {
        js_timer_clearInterval(timeout);
    }
}

_timer_console.log("timer.js ok");
