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
 * @file        oneos_task.h
 *
 * @brief       Header file for task interface.
 *
 * @revision
 * Date         Author          Notes
 * 2021-04-20   OneOS Team      First Version.
 ***********************************************************************************************************************
 */

#ifndef __OS_TASK_H__
#define __OS_TASK_H__

#include "os_types.h"
#ifdef __cplusplus
extern "C" {
#endif

// task struct, can be redefined
struct os_task
{
    /* begin: The order, position and content cannot be changed */
    void *stack_top;   /* Point to SP */
    void *stack_begin; /* The begin address of task stack */
    void *stack_end;   /* The end address of task stack */

    os_uint16_t state; /* Task state, refer to OS_TASK_STATE_INIT, OS_TASK_STATE_READY ... */
    /* end:   The order, position and content cannot be changed */

    os_uint8_t current_priority; /* Current priority. */
    os_uint8_t backup_priority;  /* Backup priority. */

    os_err_t err_code;      /* Error code. */
    os_err_t switch_retval; /* Task switch return value, defined in os_errno.h*/

    os_uint8_t object_inited;     /* If task object is inited, value is OS_KOBJ_INITED */
    os_uint8_t object_alloc_type; /* Indicates whether memory is allocated dynamically or statically,
                                     value is OS_KOBJ_ALLOC_TYPE_STATIC or OS_KOBJ_ALLOC_TYPE_DYNAMIC */
    os_uint8_t pad[2];

    os_tick_t      tick_timeout;  /* Timeout */
    os_tick_t      tick_absolute; /* Absolute time of timeout */

    os_tick_t time_slice;           /* Task's slice time (unit: tick). */
    os_tick_t remaining_time_slice; /* Task's remaining slice time (unit: tick). */

    os_bool_t       is_wake_prio;    /* The wake type to task at block_list_head, according to priority or not */


    os_ubase_t swap_data;

    void (*cleanup)(void *user_data); /* The cleanup function is provided by the user */
    void *user_data;                  /* Private user data beyond this task. */

    char name[10]; /* Task name */
};
typedef struct os_task os_task_t;

extern os_task_t *os_task_create(const char   *name,
                                   void        (*entry)(void *arg),
                                   void         *arg,
                                   os_uint32_t   stack_size,
                                   os_uint8_t    priority);
extern os_err_t os_task_destroy(os_task_t *task);
extern os_err_t os_task_startup(os_task_t *task);
extern os_err_t os_task_msleep(os_uint32_t ms);

#ifdef __cplusplus
}
#endif


#endif /* __OS_TASK_H__ */
