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
 * @file     componentGroup.js
 * @brief    -
 * @author   ningbiao.huang@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-27
 * @history
 *****************************************************************************
 **/

import { Component } from 'component';
export class ComponentGroupImpl extends Component {
    constructor(x, y, width, height) {
        if (arguments.length == 4) {
            super(x, y, width, height);
        }
        else {
            super();
        }

        this.$className = "ComponentGroup";
        this.$childArray = new Array();
    }

    add(cmpt) {
        //TODO:
    };

    remove(cmpt) {
        //TODO:
    };

    addChild(cmpt) {
        this.$childArray.push(cmpt);
    };

    removeChild(cmpt) {
        this.$childArray.pop(cmpt);
    };

    create(handle, id) {
        super.create(handle, id);
    };

    delete() {
        super.delete();

        console.log("ComponentGroup childArray delete");
        for (var i = 0; i < this.$childArray.length; i++) {
            this.$childArray[i].delete();
        }
    };

    $element(id) {
        for (var i = 0; i < this.$childArray.length; i++) {
            if (this.$childArray[i]._id == id) {
                var elem = this.$childArray[i];
                return elem;
            }
            if (this.$childArray[i] instanceof ComponentGroupImpl) {
                console.log("ComponentGroup search in child id=" + id);
                var elem = this.$childArray[i].$element(id);
                if (elem != null) {
                    return elem;
                }
            }
        }
        return null;
    };

    dumpElement() {
        super.delete();

        console.log("ComponentGroup childArray dumpElement");
        for (var i = 0; i < this.$childArray.length; i++) {
            console.log(" " + this.$childArray[i]._id);
        }
    };
}

export function ComponentGroup() {
    return new ComponentGroupImpl();
}

console.log("ComponentGroup.js ok");