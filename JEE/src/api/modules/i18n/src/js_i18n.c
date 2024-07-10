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

#include "jerry_bindings.h"
#include "mmi_base_common.h"
#include "mmk_app.h"
#include "ctrlbase.h"
#include "ctrllabel_export.h"
#include "mmi_text.h"

DECLARE_PUBLIC_HANDLER(i18n_getLocale) {
    MMISET_LANGUAGE_TYPE_E cur_language_type = 0;


    MMIAPISET_GetLanguageType(&cur_language_type);
    char locale[16] = { 0 };

    switch (cur_language_type)
    {
    case MMISET_LANGUAGE_SIMP_CHINESE:
        //"zh-CN"
        strcpy(locale, "zh");

        break;

    default:
        //"en-US"
        strcpy(locale, "en");

        break;
    }
    //strcpy(locale, "en");
    return jerry_create_string(locale);
}


void js_register_i18n_functions() {

    REGISTER_HANDLER(i18n_getLocale);


};