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
 * @file     toast.js
 * @brief    -
 * @author   ningbiao.huang@unisoc.com
 * @version  V1.0.0
 * @date     2022-5-30
 * @history
 *****************************************************************************
 **/

import { Component } from 'component';
import { Page } from 'page';
import { Image } from 'image';
import { Label } from 'label';
import { KeyCode } from 'keycode';
//import { $r } from 'resourceload';

class IToast extends Component {
    constructor(id, string) {
        super();
        this.TAG = "IToast:";
        console.log(this.TAG + "constructor");
        this.bgType = "image";

        this.pageToast = new Page();
        this.pageToast.create(null, "toastPage");
        this.pageToast.x = 0;
        this.pageToast.y = 0;
        this.pageToast.width = 0;
        this.pageToast.height = 0;
        this.pageToast.moveStyle = "MOVE_NOT_FULL_SCREEN_WINDOW";
        this.pageToast.onKey = this.onKey;

        if (this.bgType == "image") {
            this.toastImg = new Image(30, 156, 180, 48);
            this.toastImg.create(this.pageToast, "toastImg");
            //var toastSrc = $r("sys:img:res_aw_toast_1.png");
            var toastSrc = "localfile:\/\/res\/img\/common\/res_aw_toast_1.png";
            if (toastSrc != null) {
                this.toastImg.src = toastSrc;
            } else {
                console.log("toastSrc is null!");
            }
        }

        this.toastLabel = new Label();
        this.toastLabel.create(this.pageToast, "toastLabel");
        this.toastLabel.x = 30;
        this.toastLabel.y = 156;
        this.toastLabel.width = 180;
        this.toastLabel.height = 48;
        this.toastLabel.setAttribute("font-size", "32");//bug1957369
        this.toastLabel.setAttribute("color", "0xffff");
        this.toastLabel.setAttribute("text-align", "center");
        this.toastLabel.value = string;
        //this.toastLabel.background-color = 0x8410;
    }

    delete() {
        super.delete();
        console.log(this.TAG + "delete");
        if (this.bgType == "image") {
            this.toastImg.delete();
        }
        this.toastLabel.delete();
        this.pageToast.delete();
    }

    onKey(event) {
        console.log("note: onKey", event);
        if (event.code == KeyCode.KEY_RED && event.action == 0) {
            return true;
        }
        return true;
    }
}

class Toast {
    constructor() {
    }

    /***************************************************************************
    **
    **  Description:    show a toast.
    **  Note:
    **  Parameter:      timeout:time of dissmiss toast (ms).
    **                  string :display string of toast.
    **                  onTimeout :timeout callback function,could be null or empty.
    **  Returns:        NONE
    **  Useage:        import { Toast } from 'toast';
    **                 Toast.show(1000, "test toast"); OR
    **                 Toast.show(1000, "test toast", callbakfunc);
    ***************************************************************************/
    show(timeout, string, onTimeout) {
        var toastInstance = new IToast("Toast-instance", string);
        var timerId = setTimeout(() => {
            toastInstance.delete();
            toastInstance = null;
            if (onTimeout != null) {
                onTimeout();
            }
            console.log("clearTimeout timerId=" + timerId);
            clearTimeout(timerId);
            timerId = 0;
        }, timeout);
        console.log("Toast show is ok");
    }
}

var toast = new Toast();

export {
    toast as Toast,
}
