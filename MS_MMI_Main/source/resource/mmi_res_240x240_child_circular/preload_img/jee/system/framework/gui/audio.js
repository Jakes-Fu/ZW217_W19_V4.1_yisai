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
  * Author: changli.jiang@unisoc.comf
  */

import { Component } from 'component';
class IAudio extends Component {
    constructor() {

        super();

        console.log("AudioComponet: constructor");
        this.TAG = "AudioComponet:";
        this._volume = 0;
        this._paused = true;
        this._curplaytime = 0;
        this._totaltime = 0;
        this.downprogress = 0;
        this.audioname = "";
        this._audioeventhandle = null;
        this._url = "";
        this.bgimg = "";//第二阶段
        this.singer = "";//第二阶段
        this.lyric = "";//第二阶段
        this.album = "";//第二阶段
        this._state = "";
        this._mmvmPropDefine = {
            'url': { 'type': 'String' }
        };

        Object.defineProperty(this, "url", {
            enumerable: true,
            get: function () {
                return this._url;
            },
            set: function (newValue) {
                if (typeof newValue !== "string") {
                    console.log(this.TAG + "string type error");
                }
                else {
                    this._url = newValue;
                    console.log(this.TAG + " url:%s", this._url);
                }
            }
        })

        Object.defineProperty(this, "volume", {
            enumerable: true,
            get: function () {
                this._volume = js_audio_get_volume(this._audiohandle);
                return this._volume;
            },
            set: function (newValue) {
                if (typeof newValue !== "number") {
                    console.log(this.TAG + "number type error");
                }
                else {
                    this._volume = newValue;
                    js_audio_set_volume(this._audiohandle, this._volume);
                    console.log(this.TAG + " volume:%s", this._volume);
                }
            }
        })

        Object.defineProperty(this, "currentTime", {
            enumerable: true,
            get: function () {
                return this._curplaytime;
            },
        })

        Object.defineProperty(this, "duration", {
            enumerable: true,
            get: function () {
                return this._totaltime;
            },
        })

        Object.defineProperty(this, "paused", {
            enumerable: true,
            get: function () {
                console.log(this.TAG + "get _paused:%s", this._paused);
                return this._paused;
            },
        })
        Object.defineProperty(this, "state", {
            enumerable: true,
            get: function () {
                this._state = js_audio_get_status(this._audiohandle);
                console.log(this.TAG + " get _state:" + this._state);
                return this._state;
            },
        })

    }

    play(url) {
        console.log(this.TAG + " enter play function");
        this._url = url;
        js_audio_play(this._audiohandle, url, 0);
    }
    pause() {
        console.log(this.TAG + " enter pause function");
        js_audio_pause(this._audiohandle);
    }
    stop() {
        console.log(this.TAG + " enter stop function");
        js_audio_stop(this._audiohandle);
    }
    resume() {
        console.log(this.TAG + " enter resume function");
        js_audio_resume(this._audiohandle);
    }

    setAudioEventHandle(eventHandle) {
        console.log(this.TAG + "setAudioEventHandle");
        this._audioeventhandle = eventHandle;
    }
    onPlay() {
        console.log(this.TAG + " onClick");
    }
    onPause() {
        console.log(this.TAG + " onPause");
    }
    onEnded() {
        console.log(this.TAG + " onPlayEnd");
    }
    onTimeupdate() {
        console.log(this.TAG + " onPlayProgress");
    }
    onError() {
        console.log(this.TAG + " onError");
    }

    onBuffer() {
        console.log(this.TAG + " onBuffer");
    }

    onEndBuffer() {
        console.log(this.TAG + " onEndBuffer");
    }

    onStop() {
        console.log(this.TAG + " onStop");
    }


    create(handle, id) {
        super.create(handle, id);
        console.log(this.TAG + " create");
        //this._nativeHandle = js_gui_text_create(this, this._parentHandle);
        this._audiohandle = js_audio_create(this);
        return this._audiohandle;
    }
    delete() {
        console.log(this.TAG + " delete");
        super.delete();
        this.stop();
        js_audio_destroy(this._audiohandle);
    }


    AudioEventLoop(event) {
        var result = false;
        console.log(this.TAG + " AudioEventLoop1 :" + event.type);
        if (this._audioeventhandle !== null) {
            result = this._audioeventhandle(event);
        }
        if (result) {
            return true;
        }

        console.log(this.TAG + " AudioEventLoop2 :" + event.type);
        if (event.type === "onPlay") {
            this._paused = false;
            this.onPlay(event);
            console.log(this.TAG + "event_type onplay");
        } else if (event.type === "onPause") {
            this._paused = true;
            this.onPause(event);
            console.log(this.TAG + "event_type onpause");
        } else if (event.type === "onSuspend") {
            this._paused = true;
            this.onPause(event);
            console.log(this.TAG + "event_type onsuspend");
        } else if (event.type === "onResume") {
            this._paused = false;
            this.onPlay(event);
            console.log(this.TAG + "event_type onresume");
        } else if (event.type === "onPlayEnd") {
            this.onEnded(event);
            console.log(this.TAG + " event_type onplayend");
        } else if (event.type === "playProgress") {

            this._curplaytime = event.cur_time;
            this._totaltime = event.total_time;
            this.onTimeupdate(event);
            //console.log(this.TAG + " event_type playprogress:cur_time:%d,total:%d", event.cur_time, event.total_time);
        } else if (event.type === "beginBuffer") {
            this._paused = true;
            this.onBuffer(event);
            console.log(this.TAG + " event_type begin buffer");
        } else if (event.type === "endBuffer") {
            this._paused = false;
            this.onEndBuffer(event);
            console.log(this.TAG + " event_type end buffer");
        } else if (event.type === "onStop") {
            this.onStop(event);
            console.log(this.TAG + " event_type onstop");
        }
        else if (event.type === "onErr") {
            this.onError(event);
            console.log(this.TAG + " event_type onErr type:%s", event.errtype);
        }
        else {
            console.log(" event_type unknow");
        }
    }
}



export function Audio(x, y, width, height) {
    if (arguments.length == 4) {
        return new IAudio(x, y, width, height);
    } else {
        return new IAudio();
    }
}


console.log("audio.js ok..");
