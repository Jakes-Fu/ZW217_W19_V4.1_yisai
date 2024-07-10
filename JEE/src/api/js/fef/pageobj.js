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
 * @file     pageobj.js
 * @brief    -
 * @author   qingjun.yu@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-12
 * @history
 *****************************************************************************
 **/


import { PAGE_STATUS } from "pagestatus.js";

export class PageObj {
    constructor(uri, initdata) {
        this.uri = uri;
        this.initdata = initdata;
        this.logic_obj = null;
        this.view_obj = null;
        this.status_machine = null;
        this.status = PAGE_STATUS.PAGE_STATUS_NONE;
    }

    dataInit() {
        if (this.initdata != null) {
            Object.assign(this.logic_obj.data, this.initdata);
        }
    }

    uiInit() {
        this.view_obj.root = this.view_obj._ui_start(this.logic_obj);

        if (this.view_obj.root != null) {

            this.logic_obj.$element = function (id) {
                return this.view_obj.root.$element(id);
            }.bind(this);

            //ui root bind view object
            this.view_obj.root.bindViewObj(this.view_obj);
            this.view_obj.onShow = function () {
                this.changeToShow();
            }.bind(this);
            this.view_obj.onHide = function () {
                this.changeToHide();
            }.bind(this);

        }
    }

    dataBind() {
        _mvvm_observe(this.logic_obj.data);
    }

    show() {
        console.log("PageObj: loop begin");

        console.log("PageObj: init data");
        this.dataInit();
        console.log("PageObj: init data end");
        this.changeToInit();

        console.log("PageObj: ui create");
        this.uiInit();
        console.log("PageObj: ui create end");
        this.dataBind();
        this.changeToReady();

        this.changeToShow();
        console.log("PageObj: loop end");
    }

    changeTo(status) {
        this.status_machine.changeTo(status);
    }

    changeToInit() {
        this.changeTo(PAGE_STATUS.PAGE_STATUS_INIT);
    }

    changeToReady() {
        this.changeTo(PAGE_STATUS.PAGE_STATUS_READY);
    }

    changeToHide() {
        this.changeTo(PAGE_STATUS.PAGE_STATUS_HIDE);
    }

    changeToShow() {
        this.changeTo(PAGE_STATUS.PAGE_STATUS_SHOW);
    }

    changeToDestroy() {
        this.changeTo(PAGE_STATUS.PAGE_STATUS_DESTROY);
    }

    onPageInit() {
        console.log("onPageInit");
        if (this.logic_obj.onInit != null) {
            this.logic_obj.onInit();
        }
    }

    onPageReady() {
        console.log("onPageReady");
        if (this.logic_obj.onReady != null) {
            this.logic_obj.onReady();
        }
    }

    onPageShow() {
        console.log("onPageShow");
        if (this.logic_obj.onShow != null) {
            this.logic_obj.onShow();
        }
    }

    onPageHide() {
        console.log("onPageHide");
        if (this.logic_obj.onHide != null) {
            this.logic_obj.onHide();
        }
    }

    onPageDestroy() {
        console.log("onPageDestroy");
        if (this.logic_obj.onDestroy != null) {
            this.logic_obj.onDestroy();
        }
        if (this.view_obj.root != null) {
            this.view_obj.root.delete();
        }
    }
}

console.log("pageobj.js ok");