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
 * @file     image.js
 * @brief    -
 * @author   qingjun.yu@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-12
 * @history
 *****************************************************************************
 **/

import { Component } from 'component';
export class Image extends Component {
    constructor(x, y, width, height) {
        if (arguments.length == 4) {
            super(x, y, width, height);
        } else {
            super();
        }

        console.log("Image: constructor");
        this._src = "";

        Object.defineProperty(this, "src", {
            enumerable: true,
            get: function () {
                return this._src;
            },
            set: function (newValue) {
                this._src = newValue;
                if (this._nativeHandle !== null) {
                    js_image_set_path(this._nativeHandle, newValue);
                    js_image_update(this._nativeHandle);
                }
            }
        })
    }

    create(handle, id) {
        super.create(handle, id);

        console.log("Image: create");
        this._nativeHandle = js_image_create(this, this._parentHandle, this.x, this.y, this.width, this.height);
        if (this._nativeHandle !== null) {
            js_image_set_path(this._nativeHandle, this._src);
        }
        return this._nativeHandle;
    }

    delete() {
        super.delete();

        console.log("Image: delete");
        js_image_delete(this._nativeHandle);
    }

    rotate(angle) {
        console.log("Image: rotate");
        js_image_rotate(this._nativeHandle, angle);
    }

    addLayer() {
        console.log("Image: addLayer");
        js_image_add_layer(this._parentHandle, this._nativeHandle);
    }

    removeLayer() {
        console.log("Image: removeLayer");
        js_image_remove_layer(this._nativeHandle);
    }

    update() {
        console.log("Image: update");
        js_image_update(this._nativeHandle);
    }
}

console.log("image.js ok");