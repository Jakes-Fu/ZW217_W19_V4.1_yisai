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
  * Created on: 2021/09/18
  * Author: jie.yang@unisoc.com
  */




#include "urjs_art.h"
#include "urjs_art_memory.h"

int art_init_engine(JSART_MALLOC* aMalloc, JSART_FREE* aFree) {
    alogi("jsengine_init");

    script_controller_init();

    return 0;
}

int art_uninit_engine() {
    alogi("jsengine_unInit begin");

    script_controller_exit();
    alogi("jsengine_unInit done");
    return 0;
}


int art_engine_runcode(const char* file, char* code, int size) {
    return execute_script(file, code, size);

}

int art_engine_call_js_func(char* file, char* functionname) {
    return art_engine_runcode(file, functionname, strlen(functionname));

}
