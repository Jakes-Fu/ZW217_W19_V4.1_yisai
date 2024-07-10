
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

fs = require("fs");
app = require("@app");

function $t(arg1) {
  var index = arg1.split('.');

  var filename = 'localfile://i18n/' + app.getLocaleSync() + '.json';
  var ret = fs.statSync(filename)
  var size = ret.size;
  if (0 == size) {
    return '';
  }
  var fd = fs.openSync(filename, "r", 0);
  var buf = new Buffer(" ".repeat(size));
  fs.readSync(fd, buf, 0, size, null);
  var content = buf.toString();
  fs.closeSync(fd);

  var jsonfile = JSON.parse(content.replace(/[\r\n\s+]/g, ''));
  if (index.length == 2) {
    return jsonfile[index[0]][index[1]];
  } else {
    return jsonfile[index[0]];
  }
}