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
 * @file        oneos_sem.h
 *
 * @brief       Header file for semaphore interface.
 *
 * @revision
 * Date         Author          Notes
 * 2020-12-28   OneOS Team      First Version
 ***********************************************************************************************************************
 */

#ifndef __OS_SEM_H__
#define __OS_SEM_H__

#include "os_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct os_semaphore
{

    os_uint32_t count;             /* Current count */
    os_uint32_t max_count;         /* Semaphore support maximum value */
    os_uint8_t  object_inited;     /* If semaphore object is inited, value is OS_KOBJ_INITED */
    os_uint8_t  object_alloc_type; /* Indicates whether memory is allocated dynamically or statically,
                                      value is OS_KOBJ_ALLOC_TYPE_STATIC or OS_KOBJ_ALLOC_TYPE_DYNAMIC */
    os_uint8_t wake_type;          /* The type to wake up blocking tasks, value is OS_SEM_WAKE_TYPE_PRIO
                                      or OS_SEM_WAKE_TYPE_FIFO */

    char name[10]; /* Semaphore name */
};
typedef struct os_semaphore os_sem_t;

extern os_sem_t *os_sem_create(const char *name, os_uint32_t value, os_uint32_t max_value);
extern os_err_t  os_sem_destroy(os_sem_t *sem);

extern os_err_t os_sem_wait(os_sem_t *sem, os_tick_t timeout);
extern os_err_t os_sem_post(os_sem_t *sem);

#ifdef __cplusplus
}
#endif

#endif /* __OS_SEM_H__ */
