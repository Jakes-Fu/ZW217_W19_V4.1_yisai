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
  * Created on: 2022/06/02
  * Author: jie.yang@unisoc.com
  */
#ifndef __JEE_DEF_H__
#define __JEE_DEF_H__

#include "ual_type.h"

#ifdef __cplusplus
extern "C" {
#endif


#define  APP_DATA_URL "datafile://"
#define  APP_CACHE_URL "cachefile://"
#define  APP_LOCAL_URL "localfile://"
#define  SYS_LOCAL_URL "systemfile://"
/*
#define JEE_ROOT_DIR "C:\\jee\\"

#define JEE_DATA_DIR  L"C:\\jee\\data"
#define JEE_APP_INSTALL_DIR  L"C:\\jee\\data\\app"
#define JEE_DATA_DATA_DIR  L"C:\\jee\\data\\data"
#define JEE_SYSTEM_DATA_DIR  L"C:\\jee\\data\\data\\system"
#define JEE_APP_DATA_DIR     L"C:\\jee\\data\\data\\app"

#define JEE_FWK_DIR     "C:\\jee\\system\\framework\\"


#define JEE_PRELOAD_APP_DIR     L"C:\\jee\\system\\preload"
#define JEE_PRELOAD_SYS_DATA_DIR     L"C:\\jee\\system\\data"
*/
#define JEE_ROOT_DIR "D:\\jee\\"
	
#define JEE_DATA_DIR  L"D:\\jee\\data"
#define JEE_APP_INSTALL_DIR   L"D:\\installed\\apps" // L"D:\\jee\\data\\app"
#define JEE_DATA_DATA_DIR  L"D:\\jee\\data\\data"
#define JEE_SYSTEM_DATA_DIR  L"D:\\jee\\data\\data\\system"
#define JEE_APP_DATA_DIR     L"D:\\jee\\data\\data\\app"
	
#define JEE_FWK_DIR     "D:\\jee\\system\\framework\\"
	
	
#define JEE_PRELOAD_APP_DIR     L"D:\\jee\\data\\app" //L"D:\\jee\\system\\preload"
#define JEE_PRELOAD_SYS_DATA_DIR     L"D:\\jee\\system\\data"

#define JEE_APP_LOCAL_SYSTEM_DIR L"system"
#define JEE_APP_LOCAL_CACHE_DIR L"cache"
#define JEE_APP_LOCAL_DATA_DIR L"data"

#ifdef __cplusplus
}
#endif

#endif // __JERRY_FS_H__
