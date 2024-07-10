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
 * Created on: 2021/09/19
 * Author: jie.yang@unisoc.com
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "jerry_bindings.h"

#ifdef WIN32
#ifdef USE_MINGW
#define snprintf(...) _snprintf(__VA_ARGS__)
#define vsnprintf(...) _vsnprintf(__VA_ARGS__)
#endif
#endif

#define MAX_LOG_BUFFER_SIZE (256)
#define MAX_JS_LOG_BUFFER_SIZE (8192 + 1)
#define ELLIPSIS "......"

static char js_log_buf[MAX_LOG_BUFFER_SIZE];
jerry_char_t log_buffer[MAX_JS_LOG_BUFFER_SIZE] = {0};

void native_printlog(const char *format, ...)
{

    va_list args;
    size_t length = 0;
    size_t tagLen = 0;
    va_start(args, format);

    // tagLen =sprintf(js_log_buf,"<JS>  %d  :  ", SCI_GetTickCount() / 1000);
    tagLen = 0;
    length = vsnprintf(js_log_buf + tagLen, MAX_LOG_BUFFER_SIZE - 1 - tagLen,
                       format, args);
    if ((length + tagLen) > MAX_LOG_BUFFER_SIZE - 1)
    {
        js_log_buf[MAX_LOG_BUFFER_SIZE - 1] = '\0';
    }
    else
    {
        js_log_buf[length + tagLen] = '\0';
    }

    pal_printlog(" JSA   %s", js_log_buf);

    va_end(args);
}

DECLARE_HANDLER(js_console_log)
{
    int total_size = 0;
    if (JERRY_UNLIKELY(args_cnt < 1))
    {
        return jerry_create_boolean(false);
    }
    jerry_char_t str_buffer[JERRY_MAX_READ_BUFFER_SIZE + 1] = {0};
    log_buffer[0] = 0;
    for (jerry_length_t i = 0; i < args_cnt; i++)
    {
        jerry_value_t jstr = jerry_value_to_string(args_p[i]);
        jerry_length_t start_pos = 0;
        jerry_length_t end_pos = JERRY_MAX_READ_BUFFER_SIZE;
        jerry_size_t size = 0;
        while (true)
        {
            size = jerry_substring_to_char_buffer(jstr, start_pos, end_pos,
                                                  str_buffer,
                                                  JERRY_MAX_READ_BUFFER_SIZE);
            total_size += size;
            if ((total_size) > MAX_JS_LOG_BUFFER_SIZE)
            {
                int freespaceLen =
                    (MAX_JS_LOG_BUFFER_SIZE - (total_size - size));
                int ellipsisLen = strlen(ELLIPSIS);
                strncat(log_buffer, ELLIPSIS,
                        freespaceLen > ellipsisLen ? ellipsisLen
                                                   : freespaceLen);
                break;
            }
            if (size > 0 && size < JERRY_MAX_READ_BUFFER_SIZE)
            {
                str_buffer[size] = 0;
                strcat(log_buffer, str_buffer);
            }
            else
            {
                break;
            }
            start_pos += JERRY_MAX_READ_BUFFER_SIZE;
            end_pos += JERRY_MAX_READ_BUFFER_SIZE;
        };
        jerry_release_value(jstr);
    }
    native_printlog("%s", log_buffer);
    return jerry_create_boolean(true);
}

void js_register_console_functions()
{
    REGISTER_HANDLER(js_console_log);
}