
/*
  * SPDX-FileCopyrightText: 2022-present Unisoc (Shanghai) Technologies Co., Ltd
  * SPDX-License-Identifier: LicenseRef-Unisoc-General-1.0
  * Copyright 2021-present Unisoc (Shanghai) Technologies Co., Ltd.
  * Licensed under the Unisoc General Software License V1;
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  * Software distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
  * See the Unisoc General Software License v1 for more details.
  *
  * Created on: 2022/01/01
  * Author: changli.jiang@unisoc.com
  */
console.log("audio_event.js loading");

export function AudioPlayConformEvent() {
    this.type = "";
}

export function AudioSuspendEvent() {
    this.type = "";
}

export function AudioResumeEvent() {
    this.type = "";
}
export function AudioPlayEndEvent() {
    this.type = "";
}
export function AudioPlayProgressEvent() {
    this.type = "";
    this.cur_time = 0;
    this.total_time = 0;
}
export function AudioPauseEvent() {
    this.type = "";
}
export function AudioBeginBufferEvent() {
    this.type = "";
}
export function AudioEndBufferEvent() {
    this.type = "";
}
export function AudioStopEvent() {
    this.type = "";
}
export function AudioErrorEvent() {
    this.type = "";
    this.errtype = "";
}

