
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

function FS() {
}

FS.prototype.openSync =function(path,mode,flag) {
    return js_file_openSync(path,mode,0x666);
}

FS.prototype.readSync =function(fd,buf,offset,length,position) {
    var value= js_file_readSync(fd,buf,offset,length,position);
    console.log('readSync11:'+value);
    var bs=new Buffer(value);
   // buf.fill(' ');
    bs.copy(buf);
    
    return 0;
}

FS.prototype.statSync =function(path) {
    var size=js_file_sizeSync(path);
    var ret = new Object();
    ret.size= size;
    return ret;

  
}
FS.prototype.closeSync =function(fd) {
    js_file_closeSync(fd);
  
}
 fs = new FS();