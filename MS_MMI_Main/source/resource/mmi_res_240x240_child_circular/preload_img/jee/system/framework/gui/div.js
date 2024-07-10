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
 * @file     div.js
 * @brief    -
 * @author   ningbiao.huang@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-27
 * @history
 *****************************************************************************
 **/

import { ComponentGroupImpl } from 'componentGroup';
export class Div extends ComponentGroupImpl {
    constructor(handle, id) {
        super();

        this._flex_direction = "column";
        this._flex_wrap = "nowrap";
        this._justify_content = "flex-start";
        this._align_items = "stretch";

        //flex_direction support param: row,column;
        //default: row
        Object.defineProperty(this, "flex-direction", {
            enumerable: true,
            get: function () {
                return this._flex_direction;
            },
            set: function (newValue) {
                if (typeof newValue !== "string") {
                    console.log(this.TAG + "flex_direction type error");
                } else {
                    if ((this._nativeHandle != null) && (newValue != "")) {
                        var ret = js_div_set_property(this._nativeHandle, "flex-direction", newValue);
                        if (ret == true) {
                            this._flex_direction = newValue;
                        } else {
                            console.log(this.TAG + "js_div_set_property _flex_direction fail!");
                        }
                    } else {
                        console.log(this.TAG + "this._nativeHandle === null");
                    }
                }
            }
        })

        //flex-wrap support param: wrap,nowrap;
        //default: nowrap
        Object.defineProperty(this, "flex-wrap", {
            enumerable: true,
            get: function () {
                return this._flex_wrap;
            },
            set: function (newValue) {
                if (typeof newValue !== "string") {
                    console.log(this.TAG + "flex-wrap type error");
                } else {
                    if ((this._nativeHandle != null) && (newValue != "")) {
                        var ret = js_div_set_property(this._nativeHandle, "flex-wrap", newValue);
                        if (ret == true) {
                            this._flex_wrap = newValue;
                        } else {
                            console.log(this.TAG + "js_div_set_property _flex_wrap fail!");
                        }
                    } else {
                        console.log(this.TAG + "this._nativeHandle === null");
                    }
                }
            }
        })

        //justify-content support param: flex-start,flex-end,center;
        //default:flex-start
        Object.defineProperty(this, "justify-content", {
            enumerable: true,
            get: function () {
                return this._justify_content;
            },
            set: function (newValue) {
                if (typeof newValue !== "string") {
                    console.log(this.TAG + "justify-content type error");
                } else {
                    if ((this._nativeHandle != null) && (newValue != "")) {
                        var ret = js_div_set_property(this._nativeHandle, "justify-content", newValue);
                        if (ret == true) {
                            this._justify_content = newValue;
                        } else {
                            console.log(this.TAG + "js_div_set_property _justify_content fail!");
                        }
                    } else {
                        console.log(this.TAG + "this._nativeHandle === null");
                    }
                }
            }
        })

        //align-items support param: stretch,flex-start,flex-end,center;
        //default:stretch
        Object.defineProperty(this, "align-items", {
            enumerable: true,
            get: function () {
                return this._align_items;
            },
            set: function (newValue) {
                if (typeof newValue !== "string") {
                    console.log(this.TAG + "align-items type error");
                } else {
                    if ((this._nativeHandle != null) && (newValue != "")) {
                        var ret = js_div_set_property(this._nativeHandle, "align-items", newValue);
                        if (ret == true) {
                            this._align_items = newValue;
                        } else {
                            console.log(this.TAG + "js_div_set_property _align_items fail!");
                        }
                    } else {
                        console.log(this.TAG + "this._nativeHandle === null");
                    }
                }
            }
        })
    }

    addChild(cmpt) {
        super.addChild(cmpt);
        js_div_addchild(this._parentHandle, this._nativeHandle, cmpt._nativeHandle);
    };

    removeChild(cmpt) {
        super.removeChild(cmpt);
        js_div_removechild(this._nativeHandle, cmpt._nativeHandle);
    };

    create(handle, id) {
        super.create(handle, id);
        this._nativeHandle = js_div_create(this, this._parentHandle);
        return this._nativeHandle;
    }

    delete() {
        super.delete();

        console.log("Div delete");
        js_div_delete(this._nativeHandle);
    };
}

console.log("Div.js ok");