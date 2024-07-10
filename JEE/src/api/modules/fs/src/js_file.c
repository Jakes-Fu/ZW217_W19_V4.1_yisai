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
  * Created on: 2022/01/01
  * Author: zack.zhang@unisoc.com
  */


#include <stdlib.h>
#include <stdio.h>

#include "jerry_bindings.h"
#include "urjs_art.h"

#ifdef WIN_32
#include <fcntl.h>
#include <sys/stat.h>
#else
#endif //WIN_32

#include "gui_ucs2b_converter.h"


LOCAL int native_file_open(const char* file_path, int flags, int mode);
LOCAL int native_file_get_size(int fd);
LOCAL int native_file_close(int fd);
LOCAL int native_file_read(int fd, char* buffer, size_t len);

DECLARE_HANDLER(js_file_sizeSync) {
    int ret = -1;
    char file[MAX_PATH] = { 0 };
    jerry_string_t path = JERRY_GET_ARG(0, string);
    int fsflag = 0;
    int len = 0;
    fsflag = SFS_MODE_READ | SFS_MODE_OPEN_EXISTING;
    ret = js_jee_get_native_path(jerry_string_data(&path), file, sizeof(file) - 1);
    if(-1 == ret)
    {
        aloge("js_file_sizeSync ret error!");
        jerry_string_destroy(&path);
        return jerry_create_number(len);
    }
    int fd = native_file_open(file, fsflag, 0);

    len = native_file_get_size(fd);
    native_file_close(fd);
    jerry_string_destroy(&path);
    return jerry_create_number(len);
}

DECLARE_HANDLER(js_file_openSync) {
    char file[MAX_PATH] = { 0 };
    int ret = -1;
    JERRY_CHECK_ARGS(3, string, string, number);

    jerry_string_t path = JERRY_GET_ARG(0, string);

    jerry_string_t  flags = JERRY_GET_ARG(1, string);
    int mode = JERRY_GET_ARG(2, number);
    int fsflag = 0;
    if (0 == strcmp(jerry_string_data(&flags), "r")) {
        fsflag = SFS_MODE_READ | SFS_MODE_OPEN_EXISTING;
    }


    ret = js_jee_get_native_path(jerry_string_data(&path), file, sizeof(file) - 1);
    if(-1 == ret)
    {
        aloge("js_file_openSync ret error!");
        jerry_string_destroy(&path);
        jerry_string_destroy(&flags);
        return jerry_create_number(-1);
    }

    int fd = native_file_open(file, fsflag, mode);


    jerry_string_destroy(&path);
    jerry_string_destroy(&flags);
    return jerry_create_number(fd);
}


DECLARE_HANDLER(js_file_closeSync) {
    JERRY_CHECK_ARGS(1, number);
    int fd = jerry_get_number_value(args_p[0]);

    int ret = native_file_close(fd);
    if (0 == ret) {
        return jerry_create_boolean(true);
    }

    return jerry_create_boolean(false);
}

DECLARE_HANDLER(js_file_readSync) {
    JERRY_CHECK_ARGS(1, number);


    int fd = JERRY_GET_ARG(0, number);
    int len = JERRY_GET_ARG(3, number);


    int ret = -1;
    if (fd != -1) {
        len = native_file_get_size(fd);
        if (len > 0) {
            char* buffer = jerry_buffer_allocate(len + 1);
            if (buffer != NULL) {
                ret = native_file_read(fd, buffer, len);

                if (ret == len) {
                    jerry_string_t contents = jerry_string_create_with_buffer(buffer, len);
                    jerry_value_t ret_val = jerry_jval_create_string(&contents);
                    jerry_string_destroy(&contents);
                    return ret_val;
                }
            }
        }
    }

    return jerry_create_string((jerry_char_t*)"");
}

void js_register_file_functions(void)
{
    REGISTER_HANDLER(js_file_openSync);
    REGISTER_HANDLER(js_file_closeSync);
    REGISTER_HANDLER(js_file_readSync);
    REGISTER_HANDLER(js_file_sizeSync);



}

static inline bool is_within_bounds(size_t off, size_t len, size_t max) {
    if (off >= max || max - off < len)
        return false;

    return true;
}


LOCAL uint16 * allocatePName(const uint16 * pName)
{
    uint16* uName;

    if(NULL == pName)
    {
        return (uint16*)0;
    }
    if ((MAX_PATH < unistrlength(pName)))
    {
        return (uint16*)0;
    }
    uName = art_malloc(2 * (MAX_PATH + 1));
    if(NULL == uName)
    {
        return (uint16*)0;
    }
    unistrcpy(uName, pName);
#ifdef SPRD_MMI_WIN
    _unistrconvert(uName);
#endif
    return uName;
}

LOCAL int native_file_open(const char* file_path, int flags, int mode) {
    uint16* pName=NULL;
    int fd = -1;
    uint16 unicodePath[256] = { 0 };

#ifdef WIN_32
    if (file_path != NULL) {

        fd = art_open_js_file(file_path, 0);
    }
#else
    if (file_path != NULL) {
        GUI_OtherCodeToWstr(unicodePath, 256, GUI_CHAR_CODE_UTF8, (const uint8*)file_path, (uint32)strlen(file_path)); //GUI_CHAR_CODE_UTF8 5
        pName = allocatePName((uint16*)unicodePath);
        fd = SFS_CreateFile(pName, flags, 0, 0);
    }
    art_free(pName);
#endif

    return fd;
}
LOCAL int native_file_close(int fd) {
    int ret = -1;

#ifdef WIN_32
    if (fd != -1) {
        art_close_js_file(fd);

    }
#else
    ret = SFS_CloseFile(fd);
#endif

    return ret;
}

LOCAL int native_file_get_size(int fd) {
    int size = 0;
#ifdef WIN_32
    size = art_get_js_file_size(fd);
#else
    SFS_GetFileSize(fd, &size);
#endif
    return size;
}

LOCAL int native_file_read(int fd, char* buffer, size_t len) {
    int ret = -1;
    SFS_ERROR_E sfs_ret = SFS_NO_ERROR;
#ifdef WIN_32
    if (buffer != NULL && len != 0) {

        ret = art_read_js_file(fd, buffer, len);

    }
#else
    if (buffer != NULL && len != 0) {
        sfs_ret = SFS_ReadFile((unsigned int)fd, buffer, len, &ret, 0); // != SFS_ERROR_NONE
        if(SFS_ERROR_NONE != sfs_ret)
        {
            ret = -1;
            return ret;
        }

        *(buffer + len) = 0;
    }
#endif

    return ret;
}
