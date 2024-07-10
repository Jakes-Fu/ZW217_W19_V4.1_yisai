/*
  * SPDX-FileCopyrightText: 2022-present Unisoc (Shanghai) Technologies Co., Ltd
  * SPDX-License-Identifier: LicenseRef-Unisoc-General-1.0
  * Copyright 2021-present Unisoc (Shanghai) Technologies Co., Ltd.
  * Licensed under the Unisoc General Software License V1;
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  * Software distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
  * See the Unisoc General Software License v1 for more details.
  *
  * Created on: 2022/08/01
  * Author: qi.zhang3@unisoc.com
  */
console.log("http.js loading.....");
export class httpRequest {
    constructor() {
        console.log("httpRequest: constructor");
        this.TAG = "httpRequest:";
        this._url = "";
        this._callbackFun = null ;

        Object.defineProperty(this, "url", {
            enumerable: true,
            get: function () {
                return this._url;
            },
            set: function (newValue) {
                if (typeof newValue !== "string") {
                    console.log(this.TAG + "string type error");
                }
                else {
                    this._url = newValue;
                    console.log(this.TAG + " url:%s", this._url);
                }
            }
        })
    }
    /*****************************************************************************/
    //  Description : 发起http post request
    //  Parameter: [In] url:服务器url
    //             [In] content_type:"application/x-www-form-urlencode;
    //                  application/json;multipart/form-data;application/octet-stream"
    //             [In] extra_data:post request extra data
    //             [In] handler:callback
    //             [Return] None
    //  Author: qi.zhang
    //  Note:
    /*****************************************************************************/
    Post(url, content_type, extra_data, handler){
        if (typeof handler !== "function") {
            console.log("httpRequest.post: need function");
            return;
        }
        this._url = url;
        this._callbackFun = handler;
        console.log("httpRequest.post: start");
        var result = false;
        result = js_http_post_request_start(url, content_type, extra_data, this);
        if(!result)
        {
            console.log("httpRequest.post: start request failed");
        }
    }
    /*****************************************************************************/
    //  Description : 发起http get request
    //  Parameter: [In] url:访问服务器的url
    //             [In] handler:callback
    //             [Return] None
    //  Author: qi.zhang
    //  Note:
    /*****************************************************************************/
    Get(url, handler){
        if (typeof handler !== "function") {
            console.log("httpRequest.get: need function");
            return;
        }
        console.log("httpRequest.get: start");
        this._url = url;
        this._callbackFun = handler;
        var result = false;
        result = js_http_get_request_start(url, this);
        if(!result)
        {
            console.log("httpRequest.post: start request failed");
        }
    }
    /*****************************************************************************/
    //  Description : 发起http options request
    //  Parameter: [In] url:访问服务器的url
    //             [In] handler:callback
    //             [Return] None
    //  Author: qi.zhang
    //  Note:
    /*****************************************************************************/
    Options(url, handler){
        if (typeof handler !== "function") {
            console.log("httpRequest.get: need function");
            return;
        }
        console.log("httpRequest.options: start");
        this._url = url;
        this._callbackFun = handler;
        var result = false;
        result = js_http_options_request_start(url, this);
        if(!result)
        {
            console.log("httpRequest.post: start request failed");
        }
    }
    onFailed() {
        console.log(this.TAG + " onFailed");
    }
    ResponseFun(param){
        var result = false;
        if(param.result == "Success")
        {
            if (this._callbackFun !== null) {
                result = this._callbackFun(param);
            }
        }
        else if(param.result == "Failed")
        {
            this.onFailed(param);
            console.log(this.TAG + param.errcode);
        }
    }

}
