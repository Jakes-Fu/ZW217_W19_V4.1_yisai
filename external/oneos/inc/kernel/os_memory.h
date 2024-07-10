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
 * @file        oneos_memory.h
 *
 * @brief       Header file for memory management interface.
 *
 * @revision
 * Date         Author          Notes
 * 2020-11-18   OneOS team      First Version
 ***********************************************************************************************************************
 */

#ifndef __OS_MEMORY_H__
#define __OS_MEMORY_H__

#include "os_types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void    *os_malloc(os_size_t size);
extern void     os_free(void *ptr);
extern void    *os_realloc(void *ptr, os_size_t size);
extern void    *os_calloc(os_size_t count, os_size_t size);

#ifdef __cplusplus
}
#endif

#endif /* __OS_MEMORY_H__ */
