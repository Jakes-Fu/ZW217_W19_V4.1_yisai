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

int art_init_binding(JSART_MALLOC* aMalloc, JSART_FREE* aFree) {
    alogi("art_init_binding");

    js_bindings_init();

    return 0;
}

int art_uninit_binding() {
    alogi("art_uninit_binding");
    return 0;
}