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
 * @file     page.js
 * @brief    -
 * @author   qingjun.yu@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-12
 * @history
 *****************************************************************************
 **/

import { Component } from 'component';

class IPage extends Component {
    constructor(x, y, width, height) {
        if (arguments.length == 4) {
            super(x, y, width, height);
        } else {
            super();
        }

        this.$class_name = "Page";
        this._type = "";
        this._title = "";
        this.flex_direction = "column";
        this.$refs = new Array();
        this._moveStyle = "";

        Object.defineProperty(this, "moveStyle", {
            enumerable: true,
            get: function () {
                return this._moveStyle;
            },
            set: function (newValue) {
                if (typeof newValue !== "string") {
                    console.log("Page: moveStyle type error");
                    //throw "type error";
                }
                else {
                    this._moveStyle = newValue;
                    if (null !== this._nativeHandle) {
                        js_page_set_move_style(this._nativeHandle, newValue);
                    }
                    else {
                        console.log("Page: this._nativeHandle === null")
                    }
                }
            }
        })
    }

    add(cmpt) {
        this.$refs.push(cmpt);
    }

    remove(cmpt) {
        this.$refs.pop(cmpt);
    }

    draw() {
        super.draw();

        console.log("Page: draw");
    }

    create(handle, id) {
        super.create(handle, id);
        this.id = id;
        console.log("Page: create");
        this._nativeHandle = js_page_create(this, this._type, this._title);
        return this._nativeHandle;
    }

    delete() {
        super.delete();

        console.log("page ref delete");
        for (var i = 0; i < this.$refs.length; i++) {
            console.log("id = ", this.$refs[i]._id);
            this.$refs[i].delete();
        }

        console.log("Page: delete");
        js_page_delete(this._nativeHandle);
    }

    $element(id) {
        console.log("element: id=" + id);
        for (var i = 0; i < this.$refs.length; i++) {
            if (this.$refs[i].$element != null) {
                var ele = this.$refs[i].$element(id);
                if (ele != null) {
                    return ele;
                }

            } else {
                if (this.$refs[i]._id == id) {
                    return this.$refs[i];
                }
            }
        }

        return null;
    }

    bindViewObj(object) {
        //this.viewObj = object;
    }

    onTouchStart(TouchEvent) {
        console.log("Page: onTouchStart");
        return false;
    }

    onTouchMove(TouchEvent) {
        console.log("Page: onTouchMove");
        return false;
    }

    onTouchEnd(TouchEvent) {
        console.log("Page: onTouchEnd");
        return false;
    }

    onSwipe(SwipeEvent) {
        console.log("Page: onSwipe");
        if ("toastPage" == this.id)
        {
            console.log("Page: page id is toastPage");
            return true;
        }
        return false;
    }

    onPageShow() {
        console.log("Page: onPageShow");
        if (this.viewObj != undefined) {
            this.viewObj.onShow();
        }

        return false;
    }

    onPageHide() {
        console.log("Page: onPageHide");
        if (this.viewObj != undefined) {
            this.viewObj.onHide();
        }

        return false;
    }
}

export function Page() {
    return new IPage();
}

console.log("page.js ok");