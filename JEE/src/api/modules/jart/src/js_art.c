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

#include "urjs_art.h"
#include "jee_def.h"

DECLARE_PUBLIC_HANDLER(js_art_exitApp)
{
    js_art_req_exit();
    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_art_start_app)
{

    JERRY_CHECK_ARGS(1, string);

    jerry_string_t guid = JERRY_GET_ARG(0, string);

    js_art_restart_app(jerry_string_data(&guid));
    jerry_string_destroy(&guid);
    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_art_getAppRoot)
{
    char approot[256] = {0};

    js_art_get_app_path(approot, sizeof(approot) - 1);
    return jerry_create_string(approot);
}

DECLARE_PUBLIC_HANDLER(js_art_getMemInfo)
{
#ifdef JERRY_DEBUGGER
    char buff[128] = {0};
    jerry_debugger_get_memstats(buff, sizeof(buff));
    alogi("memory: %s", buff);
#else
    alogi("javascript exception! but not open debugger");
#endif
    return jerry_create_null();
	
}


LOCAL void ucs2str(uint8* ucs2_str, uint8* str)
{
    uint16   i = 0;
    uint16 str_len = wcslen(ucs2_str);
    if (PNULL == str || PNULL == ucs2_str) return;

    for (i = 0; i < str_len; i++)
    {
        str[i] = *(uint16*)& ucs2_str[2 * i];
    }
    str[i] = 0;
}

int js_jee_get_native_path(char *urlfile, char *nativefile, int len)
{
    char appid[APP_ID_LENGTH] = {0};
    char str[MAX_PATH] = { 0 };
    jee_get_app_guid(appid, sizeof(appid) - 1);
    if (strstr(urlfile, APP_DATA_URL) == urlfile)
    {
        ucs2str(JEE_APP_DATA_DIR, str);
        strcpy(nativefile, str);
        strcat(nativefile, "\\");
        strcat(nativefile, appid);
        strcat(nativefile, "\\");
        ucs2str(JEE_APP_LOCAL_DATA_DIR, str);
        strcat(nativefile, str);
        strcat(nativefile, "\\");
        strcat(nativefile, urlfile + strlen(APP_DATA_URL));
    }
    else if (strstr(urlfile, APP_CACHE_URL) == urlfile)
    {
        ucs2str(JEE_APP_DATA_DIR, str);
        strcpy(nativefile, str);
        strcat(nativefile, "\\");
        strcat(nativefile, appid);
        strcat(nativefile, "\\");
        ucs2str(JEE_APP_LOCAL_CACHE_DIR, str);
        strcat(nativefile, str);
        strcat(nativefile, "\\");
        strcat(nativefile, urlfile + strlen(APP_CACHE_URL));
    }
    else if (strstr(urlfile, APP_LOCAL_URL) == urlfile)
    {
        js_art_get_app_path(nativefile, len - 1);
        strcat(nativefile, urlfile + strlen(APP_LOCAL_URL));
    }
    else if (strstr(urlfile, SYS_LOCAL_URL) == urlfile)
    {
        ucs2str(JEE_PRELOAD_SYS_DATA_DIR, str);
        strcpy(nativefile, str);
    }
    else
    {
        return -1;
    }
    for (int i = 0; i < strlen(nativefile); i++)
    {
        if (nativefile[i] == '/')
        {
            nativefile[i] = '\\';
        }
    }
    return 0;
}

DECLARE_HANDLER(js_jee_getRootDir)
{
    return jerry_create_string(JEE_ROOT_DIR);
}

DECLARE_HANDLER(js_jee_getAppInstallDir)
{
    char str[MAX_PATH] = { 0 };
    ucs2str(JEE_APP_INSTALL_DIR, str);
    return jerry_create_string(str);
}

DECLARE_HANDLER(js_jee_getSystemDataDir)
{
    char str[MAX_PATH] = { 0 };
    ucs2str(JEE_SYSTEM_DATA_DIR, str);
    return jerry_create_string(str);
}

DECLARE_HANDLER(js_jee_getAppDataDir)
{
    char str[MAX_PATH] = { 0 };
    ucs2str(JEE_APP_DATA_DIR, str);
    return jerry_create_string(str);
}

DECLARE_HANDLER(js_jee_getFWKDir)
{
    return jerry_create_string(JEE_FWK_DIR);
}

DECLARE_HANDLER(js_jee_getPreloadAppDir)
{
    char str[MAX_PATH] = { 0 };
    ucs2str(JEE_PRELOAD_APP_DIR, str);
    return jerry_create_string(str);
}

DECLARE_HANDLER(js_jee_getAppLocalCacheDir)
{
    char str[MAX_PATH] = { 0 };
    ucs2str(JEE_APP_LOCAL_CACHE_DIR, str);
    return jerry_create_string(str);
}

DECLARE_HANDLER(js_jee_getAppLocalDataDir)
{
    char str[MAX_PATH] = { 0 };
    ucs2str(JEE_APP_LOCAL_DATA_DIR, str);
    return jerry_create_string(str);
}

DECLARE_HANDLER(js_jee_getNativePath)
{
    int ret = -1;
    char file[256] = {0};
    jerry_string_t path = JERRY_GET_ARG(0, string);
    int fsflag = 0;
    int len = 0;
    fsflag = SFS_MODE_READ | SFS_MODE_OPEN_EXISTING;
    ret = js_jee_get_native_path(jerry_string_data(&path), file, sizeof(file) - 1);
    if(-1 == ret)
    {
        aloge("js_jee_getNativePath ret error!");
    }
    jerry_string_destroy(&path);
    return jerry_create_string(file);
}

void js_register_art_functions()
{

    REGISTER_HANDLER(js_art_exitApp);
    REGISTER_HANDLER(js_art_start_app);
    REGISTER_HANDLER(js_art_getAppRoot);
    REGISTER_HANDLER(js_art_getMemInfo);

    REGISTER_HANDLER(js_jee_getNativePath);
    REGISTER_HANDLER(js_jee_getRootDir);
    REGISTER_HANDLER(js_jee_getAppInstallDir);
    REGISTER_HANDLER(js_jee_getSystemDataDir);
    REGISTER_HANDLER(js_jee_getAppDataDir);
    REGISTER_HANDLER(js_jee_getFWKDir);
    REGISTER_HANDLER(js_jee_getPreloadAppDir);
    REGISTER_HANDLER(js_jee_getAppLocalCacheDir);
    REGISTER_HANDLER(js_jee_getAppLocalDataDir);
};