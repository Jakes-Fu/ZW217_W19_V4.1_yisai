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
 * @file     storage.js
 * @brief    -
 * @author   ningbiao.huang@unisoc.com
 * @version  V1.0.0
 * @date     2022-5-30
 * @history
 *****************************************************************************
 **/

console.log("storage.js loading");
export class Storage {
    getSync(key, defValue) {
        if (typeof key !== "string") {
            console.log("type error");
            return defValue;
        }
        let value = js_kv_get_value(key);
        if (value == null) {
            return defValue;
        }
        return value;
    }

    putSync(key, value) {
        if (typeof value !== "string") {
            console.log("type error");
            return -1;
        }
        return js_kv_set_value(key, value);
    }

    deleteSync(key) {
        return js_kv_delete_value(key);
    }
}

console.log("storage.js ok");

