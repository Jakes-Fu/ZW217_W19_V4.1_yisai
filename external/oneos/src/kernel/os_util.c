/**
 ***********************************************************************************************************************
 * Copyright (c) 2020, China Mobile Communications Group Co.,Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
 * an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations under the License.
 *
 * @file        os_util.c
 *
 * @brief       This file provides some utility functions similar to C library, so kernel does not depend on any
 *              C library.
 *
 * @revision
 * Date         Author          Notes
 * 2020-02-20   OneOS Team      First version.
 ***********************************************************************************************************************
 */
#include <stdarg.h>
#include "os_api.h"

/**
 ***********************************************************************************************************************
 * @brief           This function will print a formatted string on system console.
 *
 * @param[in]       fmt             The format.
 *
 * @return          No return value.
 ***********************************************************************************************************************
 */
void os_kprintf(const char *fmt, ...)
{
    va_list ap;
    char buffer[256] = {0};

    va_start(ap, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    va_end(ap);

    SCI_TRACE_LOW(buffer);
}
