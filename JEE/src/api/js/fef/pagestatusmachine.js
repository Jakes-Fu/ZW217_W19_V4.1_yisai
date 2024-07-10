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
 * @file     pagestatusmachine.js
 * @brief    -
 * @author   qingjun.yu@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-12
 * @history
 *****************************************************************************
 **/


import { PAGE_STATUS } from "pagestatus.js";

export class PageStatusMachine {
    constructor(obj) {
        console.log("PageStatusMachine constructor");
        this.pageObj = obj;
    }

    changeTo(status) {
        this.pageObj.status = status;
        if (status == PAGE_STATUS.PAGE_STATUS_INIT) {
            this.pageObj.onPageInit();
        } else if (status == PAGE_STATUS.PAGE_STATUS_READY) {
            this.pageObj.onPageReady();
        } else if (status == PAGE_STATUS.PAGE_STATUS_SHOW) {
            this.pageObj.onPageShow();
        } else if (status == PAGE_STATUS.PAGE_STATUS_HIDE) {
            this.pageObj.onPageHide();
        } else if (status == PAGE_STATUS.PAGE_STATUS_DESTROY) {
            this.pageObj.onPageDestroy();
        } else {
            console.log("unknow status");
        }
    }
}

console.log("pagestatusmachine.js ok");