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
 * @file     slider.js
 * @brief    -
 * @author   ningbiao.huang@unisoc.com
 * @version  V1.0.0
 * @date     2022-6-27
 * @history
 *****************************************************************************
 **/

import { Component } from 'component';
export class Slider extends Component {
    constructor() {
        super();

        Object.defineProperty(this, "min", {
            enumerable: true,
            get: function () {
                let value = js_slider_getmin(this._nativeHandle);
                return value;
            },
            set: function (newValue) {
                if (typeof newValue !== "number") {
                    console.log("type error:" + newValue);
                } else {
                    var ret = js_slider_setmin(this._nativeHandle, newValue);
                    if (ret == false) {
                        console.log("set min fail newValue:" + newValue);
                    }
                }
            }
        });

        Object.defineProperty(this, "max", {
            enumerable: true,
            get: function () {
                let value = js_slider_getmax(this._nativeHandle);
                return value;
            },
            set: function (newValue) {
                if (typeof newValue !== "number") {
                    console.log("type error:" + newValue);
                } else {
                    var ret = js_slider_setmax(this._nativeHandle, newValue);
                    if (ret == false) {
                        console.log("set max fail newValue:" + newValue);
                    }
                }
            }
        });

        Object.defineProperty(this, "step", {
            enumerable: true,
            get: function () {
                let value = js_slider_get_step(this._nativeHandle);
                return value;
            },
            set: function (newValue) {
                if (typeof newValue !== "number") {
                    console.log("type error:" + newValue);
                } else {
                    var ret = js_slider_set_step(this._nativeHandle, newValue);
                    if (ret == false) {
                        console.log("set step fail newValue:" + newValue);
                    }
                }
            }
        });

        Object.defineProperty(this, "value", {
            enumerable: true,
            get: function () {
                let value = js_slider_get_cur_value(this._nativeHandle);
                return value;
            },
            set: function (newValue) {
                if (typeof newValue !== "number") {
                    console.log("type error:" + newValue);
                } else {
                    var ret = js_slider_set_cur_value(this._nativeHandle, newValue);
                    if (ret == false) {
                        console.log("set value fail newValue:" + newValue);
                    }
                }
            }
        });

        Object.defineProperty(this, "type", {
            enumerable: true,
            get: function () {
                let value = js_slider_get_display_type(this._nativeHandle);
                return value;
            },
            set: function (newValue) {
                if (typeof newValue !== "string") {
                    console.log("type error:" + newValue);
                } else {
                    var ret = js_slider_set_display_type(this._nativeHandle, newValue);
                    if (ret == false) {
                        console.log("set type fail newValue:" + newValue);
                    }
                }
            }
        });

        Object.defineProperty(this, "show-block", {
            enumerable: true,
            get: function () {
                let value = js_slider_get_is_need_thumb(this._nativeHandle);
                return value;
            },
            set: function (newValue) {
                if (typeof newValue !== "boolean") {
                    console.log("type error:" + newValue);
                } else {
                    var ret = js_slider_set_need_thumb(this._nativeHandle, newValue);
                    if (ret == false) {
                        console.log("set show-block fail newValue:" + newValue);
                    }
                }
            }
        });

        this.blockWidth = 0;
        Object.defineProperty(this, "block-width", {
            enumerable: true,
            get: function () {
                let value = js_slider_get_thumb_width(this._nativeHandle);
                return value;
            },
            set: function (newValue) {
                if (typeof newValue !== "number") {
                    console.log("type error:" + newValue);
                } else {

                    var ret = js_slider_set_thumb_size(this._nativeHandle, newValue, this.blockHeight);
                    if (ret == true) {
                        this.blockWidth = any2number(newValue);
                    } else {
                        console.log("set block-width fail newValue:" + newValue);
                    }
                }
            }
        });

        this.blockHeight = 0;
        Object.defineProperty(this, "block-height", {
            enumerable: true,
            get: function () {
                let value = js_slider_get_thumb_height(this._nativeHandle);
                return value;
            },
            set: function (newValue) {
                if (typeof newValue !== "number") {
                    console.log("type error:" + newValue);
                } else {
                    var ret = js_slider_set_thumb_size(this._nativeHandle, this.blockWidth, newValue);
                    if (ret == true) {
                        this.blockHeight = any2number(newValue);
                    } else {
                        console.log("set block-height fail newValue:" + newValue);
                    }
                }
            }
        });

        Object.defineProperty(this, "stroke-width", {
            enumerable: true,
            get: function () {
                let size = js_slider_get_channel_size(this._nativeHandle);
                return size.width;
            },
            set: function (newValue) {
                if (this._nativeHandle != null) {
                    var ret = js_slider_set_channel_size(this._nativeHandle, newValue, this._height);
                    if (ret == false) {
                        console.log("set width fail newValue:" + newValue);
                    }
                }
            }
        });

        Object.defineProperty(this, "stroke-height", {
            enumerable: true,
            get: function () {
                let size = js_slider_get_channel_size(this._nativeHandle);
                return size.height;
            },
            set: function (newValue) {
                if (this._nativeHandle != null) {
                    var ret = js_slider_set_channel_size(this._nativeHandle, this._width, newValue);
                    if (ret == false) {
                        console.log("set height fail newValue:" + newValue);
                    }
                }
            }
        });

        Object.defineProperty(this, "color", {
            enumerable: true,
            get: function () {
                let value = js_slider_get_channel_bgcolor(this._nativeHandle);
                return value;
            },
            set: function (newValue) {
                var ret = js_slider_set_channel_bgcolor(this._nativeHandle, newValue);
                if (ret == true) {
                    this._background_color = any2number(newValue);
                } else {
                    console.log("set background-color fail newValue:" + newValue);
                }
            }
        });

        Object.defineProperty(this, "active-color", {
            enumerable: true,
            get: function () {
                let value = js_slider_get_channel_fgcolor(this._nativeHandle);
                return value;
            },
            set: function (newValue) {
                var ret = js_slider_set_channel_fgcolor(this._nativeHandle, newValue);
                if (ret == false) {
                    console.log("set active-color fail newValue:" + newValue);
                }
            }
        });

        Object.defineProperty(this, "block-color", {
            enumerable: true,
            get: function () {
                let value = js_slider_get_thumb_color(this._nativeHandle);
                return value;
            },
            set: function (newValue) {
                var ret = js_slider_set_thumb_color(this._nativeHandle, newValue);
                if (ret == false) {
                    console.log("set block-color fail newValue:" + newValue);
                }
            }
        });
    }

    //overwrite
    onChange(event) {
        console.log("Slider: onChange value=" + event.value);
        console.log("Slider: onChange mode=" + event.mode);
        return false;
    }

    create(handle, id) {
        super.create(handle, id);
        this._nativeHandle = js_slider_create(this, this._parentHandle);
        return this._nativeHandle;
    }

    delete() {
        super.delete();
        js_slider_destory(this._nativeHandle);
    }
}