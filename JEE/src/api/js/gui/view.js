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
 * @file     view.js
 * @brief    -
 * @author   qingjun.yu@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-12
 * @history
 *****************************************************************************
 **/

export class view {
    constructor() {
        this.border = 0;

        this.border_style = "solid";

        this.border_left_style = "solid";
        this.border_top_style = "solid";
        this.border_right_style = "solid";
        this.border_bottom_style = "solid";

        this.border_left = "solid";
        this.border_top = "solid";
        this.border_right = "solid";
        this.border_bottom = "solid";

        this.border_width = 0;

        this.border_left_width = 0;
        this.border_top_width = 0;
        this.border_right_width = 0;
        this.border_bottom_width = 0;

        this.border_color = 0x000000;

        this.border_left_color = 0x000000;
        this.border_top_color = 0x000000;
        this.border_right_color = 0x000000;
        this.border_bottom_color = 0x000000;

        this.border_radius = 0;

        this.border_top_left_radius = 0;
        this.border_top_right_radius = 0;
        this.border_bottom_left_radius = 0;
        this.border_bottom_right_radius = 0;

        //padding support
        this._padding = 0;
        this._padding_left = 0;
        this._padding_top = 0;
        this._padding_right = 0;
        this._padding_bottom = 0;
        Object.defineProperty(this, "padding", {
            enumerable: true,
            get: function () {
                return this._padding;
            },
            set: function (newValue) {
                this._padding = any2number(newValue);
                js_component_set_property(this, this._nativeHandle, "padding");
            }
        })
        Object.defineProperty(this, "padding_left", {
            enumerable: true,
            get: function () {
                return this._padding_left;
            },
            set: function (newValue) {
                this._padding_left = any2number(newValue);
                js_component_set_property(this, this._nativeHandle, "padding_left");
            }
        })
        Object.defineProperty(this, "padding_top", {
            enumerable: true,
            get: function () {
                return this._padding_top;
            },
            set: function (newValue) {
                this._padding_top = any2number(newValue);
                js_component_set_property(this, this._nativeHandle, "padding_top");
            }
        })
        Object.defineProperty(this, "_padding_right", {
            enumerable: true,
            get: function () {
                return this._padding_right;
            },
            set: function (newValue) {
                this._padding_right = any2number(newValue);
                js_component_set_property(this, this._nativeHandle, "padding_right");
            }
        })
        Object.defineProperty(this, "padding_bottom", {
            enumerable: true,
            get: function () {
                return this._padding_bottom;
            },
            set: function (newValue) {
                this._padding_bottom = any2number(newValue);
                js_component_set_property(this, this._nativeHandle, "padding_bottom");
            }
        })

        this.background = "";
        this._background_color = 0xFFFFFF;
        Object.defineProperty(this, "background-color", {
            enumerable: true,
            get: function () {
                return this._background_color;
            },
            set: function (newValue) {

                this._background_color = any2number(newValue);
                js_component_set_bg_color(this._nativeHandle, this._background_color);
            }
        })

        this._visibility = true;
        Object.defineProperty(this, "visibility", {
            enumerable: true,
            get: function () {
                return this._visibility;
            },
            set: function (newValue) {
                if (this._nativeHandle == null) {
                    return ;
                }

                var val = this.getBoolean(newValue);
                if(val != null){
                  this._visibility = val;
                  js_component_set_visibility(this._nativeHandle, this._visibility);
                }else{
                  console.log("invalidate visibility value: "+newValue);
                }
            }
        })

        this.background_image = "";
        this.background_size = 0;
        this.background_repeat = "repeat";
        this.background_position = "0px 0px";

        this.opacity = 0;
        this.visibility = "visible";
        this.left = 0;
        this.top = 0;
        this.right = 0;
        this.bottom = 0;

        this.font_family = "";
        this.text_align = "center";
        this.font_size = "";
        this._color = 0x000000;
        Object.defineProperty(this, "color", {
            enumerable: true,
            configurable: true,
            get: function () {
                return this._color;
            },
            set: function (newValue) {

                this._color = any2number(newValue);
                js_component_set_text_color(this._nativeHandle, this._color);
            }
        })

    }

    getBoolean(value) {
      if (typeof (value) === 'boolean') {
        return value;
      }else if (value === 'true' || value === 'false') {
        return (value === 'true') ? true : false;
      } else {
        return null;
      }
    }
}

console.log("view.js ok");