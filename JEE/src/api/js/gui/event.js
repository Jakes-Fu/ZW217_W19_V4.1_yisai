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
 * @file     event.js
 * @brief    -
 * @author   qingjun.yu@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-12
 * @history
 *****************************************************************************
 **/


//define base event
export function BaseEvent() {
    this.type = "";
    this.timestamp = 0;
    this.deviceId = 0;
    this.target = null;
}

function TouchInfo() {
    this.globalX = 0;
    this.globalY = 0;
    this.localX = 0;
    this.localY = 0;
    this.size = 0;
    this.force = 0;
}

export function TouchEvent() {
    BaseEvent.call(this);

    this.touches = [];
    this.changedTouches = [];
}

TouchEvent.prototype = Object.create(BaseEvent.prototype);
TouchEvent.prototype.constructor = TouchEvent;


export function KeyEvent() {
    BaseEvent.call(this);

    this.code = 0;
    this.action = 0;
    this.repeatCount = 0;
    this.timestampStart = 0;
}

KeyEvent.prototype = Object.create(BaseEvent.prototype);
KeyEvent.prototype.constructor = KeyEvent;

export function SwipeEvent() {
    BaseEvent.call(this);

    this.direction = "left";
    this.distance = 0;
}

SwipeEvent.prototype = Object.create(BaseEvent.prototype);
SwipeEvent.prototype.constructor = SwipeEvent;

export function ChangeEvent() {
    BaseEvent.call(this);

    this.value = 0;
}

ChangeEvent.prototype = Object.create(BaseEvent.prototype);
ChangeEvent.prototype.constructor = ChangeEvent;

export function CustomEvent() {
    BaseEvent.call(this);

    this.detail = new Object();
}

CustomEvent.prototype = Object.create(BaseEvent.prototype);
CustomEvent.prototype.constructor = CustomEvent;

