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
 * @file     router.js
 * @brief    -
 * @author   qingjun.yu@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-12
 * @history
 *****************************************************************************
 **/


import { PageManager } from "pagemanager.js";

function IRouter() {

    var Router = (function () {
        function Router() {
            this.stack = new Stack();
        }
        return Router;
    })();

    Router.prototype.navigateTo = function (uri, param, callback) {
        console.log("Router: navigateTo: uri = " + uri + " param = " + param);
        if (PageManager.find(uri) == null) {
            console.log("invalidate uri");
            if (callback != null && callback.fail != null) {
                callback.fail();
            }

            return;
        }

        var obj = this.stack.peek();
        if (obj != null) {
            obj.changeToHide();
        }

        obj = PageManager.createPage(uri, param);
        this.stack.push(obj);

        obj.show();

        if (callback != null && callback.succ != null) {
            callback.succ();
        }
    }

    Router.prototype.navigateBack = function (uri, callback) {
        console.log("Router: navigateBack: " + uri);

        if (uri != null) {
            var idx = this.find(uri);
            if (idx != -1) {
                var size = this.stack.size() - idx - 1;
                while (--size > 0) {
                    PageManager.destroyPage(this.stack.peek());
                    this.stack.pop();
                }
            } else {
                console.log("invalidate uri");
                if (callback != null && callback.fail != null) {
                    callback.fail();
                }

                return;
            }
        }

        var obj = this.stack.peek();
        if (obj != null) {
            PageManager.destroyPage(obj);
            obj = null;
        }

        this.stack.pop();

        obj = this.stack.peek();
        if (obj != null) {
            obj.changeToShow();
            if (callback != null && callback.succ != null) {
                callback.succ();
            }
        } else {
            console.log("there is no page, so exit");
            this.exit();
        }
    }

    Router.prototype.redirectTo = function (uri, param, callback) {
        console.log("Router: redirectTo");
        this.navigateBack();
        this.navigateTo(uri, param, callback);
    }

    Router.prototype.clear = function () {
        console.log("Router: clear");

        var obj;
        while ((obj = this.stack.peek()) != null) {
            PageManager.destroyPage(obj);
            obj = null;
            this.stack.pop();
        }
    }

    Router.prototype.find = function (uri) {
        console.log("Router: find " + uri);
        var size = this.stack.size();
        for (var i = 0; i < size; i++) {
            var obj = this.stack.get(i);
            if (obj != null && obj.uri != null && obj.uri == uri) {
                return i;
            }
        }

        return -1;
    }

    Router.prototype.exit = function () {
        console.log("Router: exit");

        var app = require("@app");
        if (app !== null) {
            app.exitSync();
        }else{
            console.log("fatal error: there is no exitSync for app framework");
        }
    }

    return Router;
};


export let Router = new IRouter();

console.log("router.js ok");
