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
  * Author: miao.liu2@unisoc.com
  *
  */

import { Component } from 'component';
class ILabel extends Component {
    constructor(handle, id) {
        if (arguments.length == 2) {
            super.create(handle, id);
            this._nativeHandle = js_gui_text_create(this, this._parentHandle);
        } else {
            super();
        }


        console.log("Label: constructor");
        this.TAG = "Label:";
        this._string = "";
        this._fontSize = "";
        this._fontColor = "";
        this._align = "";

        this._mmvmPropDefine = {
            'value': { 'type': 'String' },
            'color': { 'type': 1 },
        };




        Object.defineProperty(this, "font-size", {
            enumerable: true,
            get: function () {
                return this._fontSize;
            },
            set: function (newValue) {
                if ('string' == typeof newValue) {
                    this._fontSize = any2number(newValue);
                } else {
                    this._fontSize = newValue;
                }
            }
        })



        Object.defineProperty(this, "color", {
            enumerable: true,
            get: function () {
                return this._fontColor;
            },
            set: function (newValue) {

                this._fontColor = any2number(newValue);
                if (null !== this._nativeHandle) {
                    js_gui_text_set_font(this._nativeHandle, this._fontSize, this._fontColor);
                }

            }
        })

        Object.defineProperty(this, "text-align", {
            enumerable: true,
            get: function () {
                return this._align;
            },
            set: function (newValue) {
                if (typeof newValue !== "string") {
                    console.log(this.TAG + "align type error");
                    //throw "type error";
                }
                else {
                    this._align = newValue;
                    if (null !== this._nativeHandle) {
                        js_gui_text_set_align(this._nativeHandle, newValue);
                    }
                    else {
                        console.log(this.TAG + "this._nativeHandle === null");
                    }
                }
            }
        })




        Object.defineProperty(this, "value", {
            enumerable: true,
            get: function () {
                return this._string;
            },
            set: function (newValue) {
                if (typeof newValue !== "string") {
                    console.log(this.TAG + "string type error");
                    return;
                }
                else {
                    if (null !== this._nativeHandle) {
                        js_gui_text_set_string(this._nativeHandle, newValue);
                    }
                    else {
                        console.log(this.TAG + "this._nativeHandle === null");
                    }
                }
            }
        })

    }

    delete() {
        super.delete();
        js_gui_text_destroy(this._nativeHandle);
    }

    create(handle, id) {
        super.create(handle, id);
        console.log("Lable: create");
        this._nativeHandle = js_gui_text_create(this, this._parentHandle);
        return this._nativeHandle;
    }
}

export function Label(x, y, width, height) {
    if (arguments.length == 4) {
        return new ILabel(x, y, width, height);
    } else {
        return new ILabel();
    }
}

console.log("label.js ok");