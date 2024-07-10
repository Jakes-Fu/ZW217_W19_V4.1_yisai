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
  * Created on: 2022/03/01
  * Author: jie.yang@unisoc.com
  */

_jee_console = require('console');

function $Jee() {
}

$Jee.prototype.getNativePath = function (path) {
    return js_jee_getNativePath(path);
}
$Jee.prototype.getRootDir = function () {

    return js_jee_getRootDir();
}

$Jee.prototype.getAppInstallDir = function () {

    return js_jee_getAppInstallDir();
}
$Jee.prototype.getSystemDataDir = function () {

    return js_jee_getSystemDataDir();
}
$Jee.prototype.getAppDataDir = function () {

    return js_jee_getAppDataDir();
}
$Jee.prototype.getFwkDir = function () {

    return js_jee_getFWKDir();
}
$Jee.prototype.getPreloadAppDir = function () {

    return js_jee_getPreloadAppDir();
}

$Jee.prototype.getAppLocalCacheDir = function () {

    return js_jee_getAppLocalCacheDir();
}

$Jee.prototype.getAppLocalDataDir = function () {

    return js_jee_getAppLocalDataDir();
}




_jee_console.log("@jee.js ok");
jee = new $Jee();
