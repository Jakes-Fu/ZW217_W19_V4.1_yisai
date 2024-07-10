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
 * @file     device.js
 * @brief    -
 * @author   ningbiao.huang@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-27
 * @history
 *****************************************************************************
 **/

console.log("device.js loading");
class Device {
    constructor() {
        console.log("Device: constructor");
        Object.defineProperty(this, "deviceId", {
            enumerable: true,
            get: function () {
                return device_get_id();
            }
        });
        Object.defineProperty(this, "osVersion", {
            enumerable: true,
            get: function () {
                return device_get_platform_ver();
            }
        });
        Object.defineProperty(this, "jeeVersion", {
            enumerable: true,
            get: function () {
                return device_get_jee_platform_ver();
            }
        });
        Object.defineProperty(this, "btMAC", {
            enumerable: true,
            get: function () {
                return device_get_bt_mac();
            }
        });
        Object.defineProperty(this, "wifiMAC", {
            enumerable: true,
            get: function () {
                return device_get_wifi_mac();
            }
        });

        // Support Property :screenWidth
        Object.defineProperty(this, "screenWidth", {
            enumerable: true,
            get: function () {
                var screenSize = device_get_screen_size();
                if (null == screenSize) {
                    console.log("screenSize empty!");
                    return 0;
                }
                return screenSize[0];
            }
        })
        // Support Property :screenHeight
        Object.defineProperty(this, "screenHeight", {
            enumerable: true,
            get: function () {
                var screenSize = device_get_screen_size();
                if (null == screenSize) {
                    console.log("screenSize empty!");
                    return 0;
                }
                return screenSize[1];
            }
        })
    }
}

//export device much in last
device = new Device();