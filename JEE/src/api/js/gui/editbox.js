
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
  * Created on: 2022/03/01
  * Author: jie.yang@unisoc.com
  */

import { Component } from 'component';
class IEditbox extends Component {
    constructor(x, y, width, height) {
        if (arguments.length == 4) {
            super(x, y, width, height);
        } else {
            super();
        }

        console.log("Editbox: constructor");


        this._value = "";
        this._fontColor = "";
        this._readonly = false;
        this._mmvmPropDefine = {
            'value' : {'type':'String'}
        };
        this._view2ModeKey ='value';
        Object.defineProperty(this, "color", {
            enumerable: true,
            get: function () {
                return this._fontColor;
            },
            set: function (newValue) {

                this._fontColor = any2number(newValue);
                if (null !== this._nativeHandle) {
                    js_native_gui_editbox_set_text_color(this._nativeHandle, this._fontColor);
                }

            }
        })
         Object.defineProperty(this, "value", {
            enumerable: true,
            get: function () {
                return js_native_gui_editbox_get_text(this._nativeHandle);

            },
            set: function (newValue) {
                if (this._nativeHandle !== null) {
                    js_native_gui_editbox_set_text(this._nativeHandle, newValue);
                }
            }
        })

    }
    create(handle, id) {
        super.create(handle, id);
        this._nativeHandle = js_edittext_ctl_create(this, this._parentHandle, this.x, this.y, this.width, this.height);
        return this._nativeHandle;
    }
    delete() {
        super.delete();
        js_edittext_ctl_destroy(this._nativeHandle);
    }

}

export function Editbox(x, y, width, height) {
    if (arguments.length == 4) {
        return new IEditbox(x, y, width, height);
    } else {
        return new IEditbox();
    }
}

console.log("editbox.js ok");