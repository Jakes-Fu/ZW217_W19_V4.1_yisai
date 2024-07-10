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
 * @file     button.js
 * @brief    -
 * @author   qingjun.yu@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-12
 * @history
 *****************************************************************************
 **/

import { Component } from 'component';
class IButton extends Component {
    constructor(x, y, width, height) {
        if (arguments.length == 4) {
            super(x, y, width, height);
        } else {
            super();
        }

        console.log("Button: constructor");
        this._type = "text";
        this._value = "";
        this._icon = "";

        Object.defineProperty(this, "value", {
            enumerable: true,
            get: function () {
                return this._value;
            },
            set: function (newValue) {
                if (null === this._nativeHandle) {
                    return ;
                }

                if (typeof newValue !== "string") {
                    console.log(this.TAG + "string type error");
                    //throw "type error";    //抛出异常,现阶段抛出异常时,js停止运行
                }
                else {
                    if (newValue.indexOf('{{') == -1) {
                        this._value = newValue;
                    } else {
                        this._value = '';
                    }

                    js_button_set_text(this._nativeHandle, this._value);

                    if (this._rawtext === "undefined") {
                        this._rawtext = newValue;
                    } else {
                    }

                }
            }
        })

        Object.defineProperty(this, "type", {
            enumerable: true,
            get: function () {
                return this._type;
            },
            set: function (newValue) {
                this._type = newValue;
            }
        })

        Object.defineProperty(this, "icon", {
            enumerable: true,
            get: function () {
                return this._icon;
            },
            set: function (newValue) {
                if (this._type == "circle") {
                    this._icon = newValue;
                }
            }
        })

        this._text_color = 0x000000;
        Object.defineProperty(this, "text-color", {
            enumerable: true,
            get: function () {
                return this._text_color;
            },
            set: function (newValue) {
                console.log("text-color: ", newValue);
                this._text_color = any2number(newValue);
                if (this._nativeHandle !== null) {
                    js_button_set_text_color(this._nativeHandle, this._text_color);
                }
            }
        })

        this._font_size = "16px";
        Object.defineProperty(this, "font-size", {
            enumerable: true,
            get: function () {
                return this._font_size;
            },
            set: function (newValue) {
                if (this._nativeHandle === null || typeof newValue !== "string") {
                    return ;
                }

                var pattern = /^[0-9]+px$/;
                if (pattern.test(newValue)) {
                    this._font_size = newValue;
                    var exp = /([0-9]+)(px)/;
                    var value = newValue.match(exp);
                    js_button_set_font_size(this._nativeHandle, any2number(value[1]));
                }else{
                    console.log("invalidate font-size: "+newValue);
                }
            }
        })

        this._text_align = "center";
        Object.defineProperty(this, "text-align", {
            enumerable: true,
            get: function () {
                return this._text_align;
            },
            set: function (newValue) {
                if (this._nativeHandle === null || typeof newValue !== "string") {
                    return ;
                }

                var val = newValue.toLowerCase();
                if (val === "left" || val === "right" || val === "center") {
                    this._text_align = val;
                    js_button_set_text_align(this._nativeHandle, val);
                }else{
                    console.log("invalidate text-align: ", newValue);
                }
            }
        })

    }

    create(handle, id) {
        super.create(handle, id);

        console.log("Button: create");
        this._nativeHandle = js_button_create(this, this._parentHandle, this.x, this.y, this.width, this.height);
        return this._nativeHandle;
    }

    delete() {
        super.delete();

        console.log("Button: delete");
        js_button_delete(this._nativeHandle);
    }

    update2view(obj) {
        this.value = '' + _mmvm_mode2view(this, obj);
    }
}

export function Button(x, y, width, height) {
    if (arguments.length == 4) {
        return new IButton(x, y, width, height);
    } else {
        return new IButton();
    }
}

console.log("button.js ok");