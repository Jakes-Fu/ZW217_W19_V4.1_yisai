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
 * @file     pagemanager.js
 * @brief    -
 * @author   qingjun.yu@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-12
 * @history
 *****************************************************************************
 **/


import { PageConstructor } from "pageconstructor.js";
import { PageDestructor } from "pagedestructor.js";

function IPageManager() {

}

IPageManager.prototype.createPage = function (uri, param) {
    console.log("pm: create page: uri =  " + uri + ", param = " + param);

    var initData = param;
    var obj = PageConstructor(this, uri, initData);

    return obj;
};

IPageManager.prototype.destroyPage = function (pageObj) {
    console.log("pm: destroy page ");

    pageObj.changeToDestroy();
    PageDestructor(pageObj);
};

IPageManager.prototype.Init = function (pagelist) {
    console.log("pm: init  ", pagelist);

    this.page_pool = pagelist;
};

IPageManager.prototype.find = function (uri) {
    console.log("pm: find  " + uri);

    if (this.page_pool == null || this.page_pool[uri] == null) {
        return null;
    }

    return this.page_pool[uri];
};

export let PageManager = new IPageManager();

console.log("pagemanager.js ok");