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
 * @file        oneos_mutex.c
 *
 * @brief       This file implements the mutex functions.
 *
 * @revision
 * Date         Author          Notes
 * 2020-12-24   OneOS team      First Version
 ***********************************************************************************************************************
 */
#include "os_errno.h"
#include "os_mutex.h"
#include "os_clock.h"
#include "os_api.h"

/**
 ***********************************************************************************************************************
 * @brief           This function will create a mutex object from heap.
 *
 * @param[in]       name            The name of mutex.
 * @param[in]       recursive       A recursive mutex or not.
 *
 * @return          The pointer to the created mutex.
 * @retval          pointer         If operation successful.
 * @retval          OS_NULL         Error occurred.
 ***********************************************************************************************************************
 */
os_mutex_t *os_mutex_create(const char *name, os_bool_t recursive)
{
    uint32 priority_inherit = (recursive == OS_TRUE) ? SCI_INHERIT : SCI_NO_INHERIT;
    SCI_MUTEX_PTR tmp = SCI_CreateMutex(name, priority_inherit);
    // SCI_TRACE_LOW("cms: os_mutex_create(%s)=0x%x.", name, tmp);
    return (os_mutex_t *)tmp;
}


/**
 ***********************************************************************************************************************
 * @brief           Destory a mutex object created from heap.
 *
 * @param[in]       mutex           The mutex to destroy.
 *
 * @return          The operation result.
 * @retval          OS_SUCCESS      If the operation successful.
 * @retval          else            Error code.
 ***********************************************************************************************************************
 */
os_err_t os_mutex_destroy(os_mutex_t *mutex)
{
    // SCI_TRACE_LOW("cms: os_mutex_destroy=0x%x.", mutex);
    return SCI_DeleteMutex((SCI_MUTEX_PTR)mutex);
}

/**
 ***********************************************************************************************************************
 * @brief           This function recursively locks a mutex and the increments it's nested count. If the mutex is
 *                  already locked by other task, the calling task will block until either the mutex becomes available
 *                  or waiting time expires. In contrast to os_mutex_lock(), it's ok to lock a mutex multiple times.
 *
 * @param[in]       mutex           The pointer to a mutex.
 * @param[in]       timeout         Waitting time (in clock ticks).
 *
 * @return          The operation result.
 * @retval          OS_EOK          If the operation successful.
 * @retval          else            Error code.
 ***********************************************************************************************************************
 */
os_err_t os_mutex_recursive_lock(os_mutex_t *mutex, os_tick_t timeout)
{
    os_uint32_t ms = timeout / (OS_TICK_PER_SECOND / 1000);
    // SCI_TRACE_LOW("cms: os_mutex_recursive_lock=0x%x-0x%x-0x%x.", mutex, timeout, SCI_IdentifyThread());
    return SCI_GetMutex((SCI_MUTEX_PTR)mutex, timeout);;
}

/**
 ***********************************************************************************************************************
 * @brief           This function recursively unlocks a mutex and decrements it's nested count. If the nested count is 0
 *                  and there are tasks blocked on the mutex, the highest priority task will be woken up and
 *                  aqcuire the mutex. In contrast to os_mutex_unlock(), it's ok to unlock a mutex multiple times.
 *
 * @param[in]       mutex           The pointer to a mutex.
 *
 * @return          The operation result.
 * @retval          OS_EOK          If the operation successful.
 * @retval          else            Error code.
 ***********************************************************************************************************************
 */
os_err_t os_mutex_recursive_unlock(os_mutex_t *mutex)
{
    // SCI_TRACE_LOW("cms: os_mutex_recursive_unlock=0x%x-0x%x.", mutex, SCI_IdentifyThread());
    return SCI_PutMutex((SCI_MUTEX_PTR)mutex);
}