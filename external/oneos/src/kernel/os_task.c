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
 * @file        os_task.c
 *
 * @brief       This file implements the task functions.
 *
 * @revision
 * Date         Author          Notes
 * 2020-04-06   OneOS team      First Version
 * 2020-11-10   OneOS team      Refactor task implementation.
 ***********************************************************************************************************************
 */
#include "os_types.h"
#include "os_task.h"
#include "os_api.h"

/**
 ***********************************************************************************************************************
 * @brief           This function creates a task with dynamic memory allocation.
 *
 * @details         Both control block and stack of the task are allocated in memory heap.
 *
 * @attention       This interface is not allowed in the following cases:
 *                      1. In interrupt context.
 *                      2. Interrupt is disabled.
 *                      3. Scheduler is locked.
 *
 * @param[in]       name            Task name.
 * @param[in]       entry           Entry function of the task.
 * @param[in]       arg             Argument of entry function.
 * @param[in]       stack_size      Stack size in bytes.
 * @param[in]       priority        Priority of task.
 *
 * @return          The address of task's control block.
 * @retval          OS_NULL         Failed to create a task with dynamic memory allocation.
 * @retval          else            Return the address of task control block.
 ***********************************************************************************************************************
 */
void os_task_entry(uint32 para1, void *para2)
{
    os_task_t *tmp = (os_task_t *)para1;
    tmp->cleanup(para2);
}
os_task_t *os_task_create(const char *name,
                          void      (*entry)(void *arg),
                          void       *arg,
                          os_uint32_t stack_size,
                          os_uint8_t  priority)
{
    os_task_t *tmp = (os_task_t *)SCI_CALLOC(1, sizeof(os_task_t));
    if (tmp == OS_NULL)
    {
        SCI_TRACE_ERROR("os_task_create os_task_t memory null.");
        return OS_NULL;
    }
    BLOCK_ID *taskid = (BLOCK_ID *)SCI_ALLOCA(sizeof(BLOCK_ID));
    if (taskid == OS_NULL)
    {
        SCI_FREE(tmp);
        SCI_TRACE_ERROR("os_task_create BLOCK_ID memory null.");
        return OS_NULL;
    }
    tmp->cleanup = entry;
    *taskid = SCI_CreateAppThread(name, name, os_task_entry, tmp, arg, stack_size, 16, priority, OS_PREEMPT, SCI_DONT_START);
    if (*taskid == SCI_INVALID_BLOCK_ID)
    {
        SCI_TRACE_ERROR("os_task_create create fail.");
        SCI_FREE(taskid);
        SCI_FREE(tmp);
        return OS_NULL;
    }
    tmp->user_data = (void *)taskid;
    return tmp;
}

/**
 ***********************************************************************************************************************
 * @brief           Destroy a task.
 *
 * @details         If the task to be destroyed is the current task, the resource will be recycled in recycle-task.
 *
 * @attention       This interface is not allowed in the following cases:
 *                      1. In interrupt context.
 *                      2. Interrupt is disabled.
 *                      3. Scheduler is locked.
 *
 * @param[in]       task            Task control block.
 *
 * @return          The result of destroying the task.
 * @retval          OS_EOK          Destroy the task successfully.
 * @retval          else            Destroy the task failed.
 ***********************************************************************************************************************
 */
os_err_t os_task_destroy(os_task_t *task)
{
    if (!task)
    {
        return OS_PARAM_ERROR;
    }
    BLOCK_ID *taskid = (BLOCK_ID *)task->user_data;
    os_err_t ret = OS_ERROR;

    if(SCI_DeleteThread(*taskid) == SCI_SUCCESS)
    {
        ret = OS_SUCCESS;
    }
    else
    {
        SCI_TRACE_ERROR("os_task_destroy delete fail.");
        ret = OS_DELETE_ERROR;
    }

    if (taskid)
    {
        SCI_FREE(taskid);
    }

    if (task)
    {
        SCI_FREE(task);
    }
    return ret;
}

/**
 ***********************************************************************************************************************
 * @brief           Startup a task.
 *
 * @details         Put the task into the ready queue and trigger a schedule.
 *
 * @param[in]       task            Task control block.
 *
 * @return          The result of starting the task.
 * @retval          OS_EOK          Startup the task successfully.
 * @retval          else            Startup the task failed.
 ***********************************************************************************************************************
 */
os_err_t os_task_startup(os_task_t *task)
{
    if (!task)
    {
        return OS_PARAM_ERROR;
    }
    BLOCK_ID *taskid = (BLOCK_ID *)task->user_data;
    return SCI_ResumeThread(*taskid);
}

/**
 ***********************************************************************************************************************
 * @brief           Force the current task to sleep.
 *
 * @details         If the time is 0, giveup the cpu.
 *
 * @attention       This function actually calls os_task_tsleep, so the minimum precision is tick.
 *                  This interface is not allowed in the following cases:
 *                      1. In interrupt context.
 *                      2. Interrupt is disabled.
 *                      3. Scheduler is locked.
 *
 * @param[in]       ms              The value of task sleep in millisecond.
 *
 * @return          The result of forcing current task to sleep.
 * @retval          OS_EOK          Force current task to sleep successfully.
 * @retval          else            Force current task to sleep failed.
 ***********************************************************************************************************************
 */
os_err_t os_task_msleep(os_uint32_t ms)
{
    SCI_Sleep(ms);
    return OS_SUCCESS;
}
