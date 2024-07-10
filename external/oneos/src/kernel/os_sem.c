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
 * @file        oneos_sem.c
 *
 * @brief       This file implements the mutex functions.
 *
 * @revision
 * Date         Author          Notes
 * 2020-12-24   OneOS team      First Version
 ***********************************************************************************************************************
 */
#include "os_sem.h"
#include "os_api.h"

/**
 ***********************************************************************************************************************
 * @brief           This function will create a semaphore object from heap.
 *
 * @param[in]       name            The name of semaphore.
 * @param[in]       value           The init value of semaphore.
 * @param[in]       max_value       The max value of semaphore.
 *
 * @return          The pointer to the created semaphore.
 * @retval          pointer         If operation successful.
 * @retval          OS_NULL         Error occurred.
 ***********************************************************************************************************************
 */
os_sem_t *os_sem_create(const char *name, os_uint32_t value, os_uint32_t max_value)
{
    return (os_sem_t *)SCI_CreateSemaphore(name, value);
}

/**
 ***********************************************************************************************************************
 * @brief           Destory the specific semaphore object created from heap.
 *
 * @param[in]       sem             The semaphore to destroy.
 *
 * @return          The operation result.
 * @retval          OS_SUCCESS      If the operation successful.
 * @retval          else            Error code.
 ***********************************************************************************************************************
 */
os_err_t os_sem_destroy(os_sem_t *sem)
{
    os_err_t ret = OS_ERROR;
    if(SCI_DeleteSemaphore((SCI_SEMAPHORE_PTR)sem) == SCI_SUCCESS)
    {
        ret = OS_SUCCESS;
    }
    return ret;
}

/**
 ***********************************************************************************************************************
 * @brief           This function decrements the semaphore's count. If the semaphore's count greater than 0, the function
 *                  decrements semaphore's count and returns immediately. Otherwise, the calling task blocks until either
 *                  the semaphore's count greater than 0 or waiting time expires.
 *
 * @param[in]       sem             The pointer to a semaphore.
 * @param[in]       timeout         Wait time (in ms).OS_WAIT_FOREVER, OS_NO_WAIT,1~0x0FFFFFFFE wait time(ms)
 *
 * @return          The operation result.
 * @retval          OS_SUCESS       If the operation successful.
 * @retval          else            Error code.
 ***********************************************************************************************************************
 */
os_err_t os_sem_wait(os_sem_t *sem, os_tick_t timeout)
{
    os_err_t ret = OS_ERROR;
    if(SCI_GetSemaphore((SCI_SEMAPHORE_PTR)sem, timeout) == SCI_SUCCESS)
    {
        ret = OS_SUCCESS;
    }
    return ret;
}

/**
 ***********************************************************************************************************************
 * @brief           This function increments the semaphore's count. If the semaphore's count becomes greater than 0. A
 *                  task blocked on it will be woken up.
 *
 * @param[in]       sem             The pointer to a semaphore.
 *
 * @return          The operation result.
 * @retval          OS_SUCESS       If the operation successful.
 * @retval          else            Error code.
 ***********************************************************************************************************************
 */
os_err_t os_sem_post(os_sem_t *sem)
{
    os_err_t ret = OS_ERROR;
    if(SCI_PutSemaphore((SCI_SEMAPHORE_PTR)sem) == SCI_SUCCESS)
    {
        ret = OS_SUCCESS;
    }
    return ret;
}
