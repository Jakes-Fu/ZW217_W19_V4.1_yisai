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
 * @file        oneos_mutes.h
 *
 * @brief       Header file for mutex interface.
 *
 * @revision
 * Date         Author          Notes
 * 2020-12-28   OneOS Team      First Version
 ***********************************************************************************************************************
 */

#ifndef __OS_MUTEX_H__
#define __OS_MUTEX_H__

#include "os_types.h"

#ifdef __cplusplus
extern "C" {
#endif
struct os_mutex
{
    os_uint32_t lock_count;   /* Current lock count */
    os_bool_t   is_recursive; /* Support recursive call. */

    os_uint8_t object_inited;     /* If mutex object is inited, value is OS_KOBJ_INITED */
    os_uint8_t object_alloc_type; /* Indicates whether memory is allocated dynamically or statically,
                                     value is OS_KOBJ_ALLOC_TYPE_STATIC or OS_KOBJ_ALLOC_TYPE_DYNAMIC */
    os_uint8_t wake_type;         /* The type to wake up blocking tasks, value is OS_MUTEX_WAKE_TYPE_PRIO
                                     or OS_MUTEX_WAKE_TYPE_FIFO */
    os_uint8_t original_priority;

    char name[10]; /* Mutex name */
};
typedef struct os_mutex os_mutex_t;

extern os_mutex_t *os_mutex_create(const char *name, os_bool_t recursive);
extern os_err_t    os_mutex_destroy(os_mutex_t *mutex);

extern os_err_t os_mutex_recursive_lock(os_mutex_t *mutex, os_tick_t timeout);
extern os_err_t os_mutex_recursive_unlock(os_mutex_t *mutex);

#ifdef __cplusplus
}
#endif

#endif /* __OS_MUTEX_H__ */
