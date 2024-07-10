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

_app_console = require('console');

function $App() {
}

$App.prototype.exitSync = function (code) {
  //tbd: exit code&router.exit
  if ($global.router != null) {
    $global.router.clear();
  }

  js_art_exitApp();
}

$App.prototype.getLocaleSync = function (fd) {
  console.log('app.getLocaleSync()');
  return i18n_getLocale();
}

$App.prototype.runApp = function (guid) {
  console.log('@app.js runApp:' + guid);
  if ($global.router != null) {
    $global.router.clear();
  }
  js_art_start_app(guid);
}




_app_console.log("@app.js ok");
app = new $App();


