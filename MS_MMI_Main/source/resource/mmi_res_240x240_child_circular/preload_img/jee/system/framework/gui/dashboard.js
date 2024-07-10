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
 * @file     dashboard.js
 * @brief    -
 * @author   qingjun.yu@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-12
 * @history
 *****************************************************************************
 **/

class Dashboard extends Component {
    constructor(x, y, width, height) {
        if (arguments.length == 4) {
            super(x, y, width, height);
        } else {
            super();
        }

        console.log("Dashboard: constructor");
        this.img_bg_path = "";
        this.img_function_bg_path = "";
        this.img_function_hand_path = "";
        this.img_function_minute_path = "";
        this.img_function_second_path = "";

        this._date_rect = [];
        this._img_bg = new Image(x, y, width, height);
        this._img_function_bg = new Image(x, y, width, height);
        this._img_function_hand = new Image(x, y, width, height);
        this._img_function_minute = new Image(x, y, width, height);
        this._img_function_second = new Image(x, y, width, height);
    }

    create(handle, id) {
        super.create(handle, id);

        console.log("Dashboard: create");

        this._img_bg.x = this.x;
        this._img_bg.y = this.y;
        this._img_bg.width = this.width;
        this._img_bg.height = this.height;
        this._img_bg.create(pageMain, "img");
        this._img_bg.src = this.img_bg_path;

        this._img_function_bg.x = this.x;
        this._img_function_bg.y = this.y;
        this._img_function_bg.width = this.width;
        this._img_function_bg.height = this.height;
        this._img_function_bg.create(pageMain, "img");
        this._img_function_bg.src = this.img_function_bg_path;

        this._img_function_hand.x = this.x;
        this._img_function_hand.y = this.y;
        this._img_function_hand.width = this.width;
        this._img_function_hand.height = this.height;
        this._img_function_hand.create(pageMain, "img");
        this._img_function_hand.src = this.img_function_hand_path;

        this._img_function_minute.x = this.x;
        this._img_function_minute.y = this.y;
        this._img_function_minute.width = this.width;
        this._img_function_minute.height = this.height;
        this._img_function_minute.create(pageMain, "img");
        this._img_function_minute.src = this.img_function_minute_path;

        this._img_function_second.x = this.x;
        this._img_function_second.y = this.y;
        this._img_function_second.width = this.width;
        this._img_function_second.height = this.height;
        this._img_function_second.create(pageMain, "img");
        this._img_function_second.src = this.img_function_second_path;
    };

    delete() {
        super.delete();

        console.log("Dashboard: delete");

        this._img_bg.delete();
        this._img_function_bg.delete();
        this._img_function_hand.delete();
        this._img_function_minute.delete();
        this._img_function_second.delete();
    }

    caliAngle(hour, minute, second) {
        var angle_sec = 0;
        var angle_min = 0;
        var angle_hour = 0;

        if (second < 60) {
            angle_sec = second * 6;
        }
        else {
            angle_sec = 0;
        }

        if (minute < 60 && hour < 24) {
            if (hour >= 12) hour -= 12;
            angle_min = minute * 6;
            angle_hour = hour * 30 + (minute >> 1);
        }
        else {
            angle_hour = 0;
            angle_min = 0;
        }

        return { "angle_hour": angle_hour, "angle_min": angle_min, "angle_sec": angle_sec };
    }

    update(time) {
        var angle = this.caliAngle(time.hour, time.minute, time.second);
        console.log("angle = ", time.hour, time.minute, time.second);
        this._img_bg.update();
        this._img_function_bg.update();
        this._img_function_hand.rotate(angle.angle_hour);
        this._img_function_minute.rotate(angle.angle_min);
        this._img_function_second.rotate(angle.angle_sec);
    }
}

console.log("dashboard.js ok");