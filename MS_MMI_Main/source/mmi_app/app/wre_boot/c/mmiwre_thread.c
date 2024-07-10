#ifdef WRE_SUPPORT
#include "os_api.h"
#include "tasks_id.h"
#include "mmiwre_thread.h"
#include "priority_app.h"

#define TASK_WRE_KERNEL_PRI                         PRI_WRE_KERNEL_TASK   //SCI_PRIORITY_KERNEL
#define TASK_WRE_CRITICAL_PRI                      PRI_WRE_CRITICAL_TASK   //SCI_PRIORITY_TIME_CRITICAL
#define TASK_WRE_HIGHEST_PRI                       PRI_WRE_HIGHEST_TASK   //SCI_PRIORITY_HIGHEST
#define TASK_WRE_ABOVE_NORMAL_PRI            PRI_WRE_ABOVE_NORMAL_TASK   //SCI_PRIORITY_ABOVE_NORMAL
#define TASK_WRE_NORMAL_PRI                        PRI_WRE_NORMAL_TASK   //SCI_PRIORITY_NORMAL
#define TASK_WRE_BELOW_NORMAL_PRI           PRI_WRE_BELOW_NORMAL_TASK   //SCI_PRIORITY_BELOW_NORMAL
#define TASK_WRE_LOWEST_PRI                        PRI_WRE_LOWEST_TASK   //SCI_PRIORITY_LOWEST
#define TASK_WRE_IDLE_PRI                              PRI_WRE_IDLE_TASK   //SCI_PRIORITY_IDLE

#define WRE_PRI_OLD(_NEW_PRI_)   (((_NEW_PRI_) < SCI_SYSTEM_END) ? ((_NEW_PRI_) - SCI_SYSTEM_START): MAX_SYSTEM_PRI_NUM)

void WRE_DisableIRQ(void)
{
    SCI_DisableIRQ();
}

void WRE_RestoreIRQ(void)
{
    SCI_RestoreIRQ();
}



uint32 WRE_CreateThread(
    const char  *thread_name,
    const char  *queue_name,
    void    (*entry)(uint32, void*),
    uint32  argc,
    void    *argv,
    uint32  stack_size,
    uint32  queue_num,
    uint32  priority,
    uint32  preempt,
    uint32  auto_start
    )
{
    uint32  thread_id = WRE_INVALID_BLOCK_ID;
    uint32  sci_priority = TASK_WRE_NORMAL_PRI;
    uint32  sci_preempt  = SCI_NO_PREEMPT;
    uint32  sci_auto_start = SCI_DONT_START;

    sci_priority = SCI_SYSTEM_PRI(priority);
#if 0
    switch(priority)
    {
        case WRE_PRIORITY_KERNEL:
            sci_priority = TASK_WRE_KERNEL_PRI;
            break;
        case WRE_PRIORITY_TIME_CRITICAL:
            sci_priority = TASK_WRE_CRITICAL_PRI;
            break;
        case WRE_PRIORITY_HIGHEST:
            sci_priority = TASK_WRE_HIGHEST_PRI;
            break;
        case WRE_PRIORITY_ABOVE_NORMAL:
            sci_priority = TASK_WRE_ABOVE_NORMAL_PRI;
            break;
        case WRE_PRIORITY_NORMAL:
            sci_priority = TASK_WRE_NORMAL_PRI;
            break;
        case WRE_PRIORITY_BELOW_NORMAL:
            sci_priority = TASK_WRE_BELOW_NORMAL_PRI;
            break;
        case WRE_PRIORITY_LOWEST:
            sci_priority = TASK_WRE_LOWEST_PRI;
            break;
        case WRE_PRIORITY_IDLE:
            sci_priority = TASK_WRE_IDLE_PRI;
            break;
        default:
            break;
    }
#endif

    switch(preempt)
    {
        case WRE_PREEMPT:
            sci_preempt = SCI_PREEMPT;
            break;
        default:
            sci_preempt = SCI_NO_PREEMPT;
            break;
    }

    switch(auto_start)
    {
        case WRE_AUTO_START:
            sci_auto_start = SCI_AUTO_START;
            break;
        default:
            sci_auto_start = SCI_DONT_START;
            break;
    }

    thread_id = SCI_CreateThread(thread_name, queue_name, entry, argc, argv, stack_size, queue_num, sci_priority, sci_preempt, sci_auto_start);
    if(SCI_INVALID_BLOCK_ID == thread_id)
    {
        thread_id = WRE_INVALID_BLOCK_ID;
    }

    return thread_id;
}

uint32 WRE_CreateAppThread(
    const char  *thread_name,
    const char  *queue_name,
    void    (*entry)(uint32, void*),
    uint32  argc,
    void    *argv,
    uint32  stack_size,
    uint32  queue_num,
    uint32  priority,
    uint32  preempt,
    uint32  auto_start
    )
{
    uint32  thread_id = WRE_INVALID_BLOCK_ID;
    uint32  sci_priority = PRI_WRE_BELOW_NORMAL_TASK;
    uint32  sci_preempt  = SCI_NO_PREEMPT;
    uint32  sci_auto_start = SCI_DONT_START;

    sci_priority = SCI_SYSTEM_PRI(priority);
#if 0
    switch(priority)
    {
        case WRE_PRIORITY_KERNEL:
            sci_priority = PRI_WRE_KERNEL_TASK;
            break;
        case WRE_PRIORITY_TIME_CRITICAL:
            sci_priority = PRI_WRE_CRITICAL_TASK;
            break;
        case WRE_PRIORITY_HIGHEST:
            sci_priority = PRI_WRE_HIGHEST_TASK;
            break;
        case WRE_PRIORITY_ABOVE_NORMAL:
            sci_priority = PRI_WRE_ABOVE_NORMAL_TASK;
            break;
        case WRE_PRIORITY_NORMAL:
            sci_priority = PRI_WRE_NORMAL_TASK;
            break;
        case WRE_PRIORITY_BELOW_NORMAL:
            sci_priority = PRI_WRE_BELOW_NORMAL_TASK;
            break;
        case WRE_PRIORITY_LOWEST:
            sci_priority = PRI_WRE_LOWEST_TASK;
            break;
        case WRE_PRIORITY_IDLE:
            sci_priority = PRI_WRE_IDLE_TASK;
            break;
        default:
            break;
    }
#endif

    switch(preempt)
    {
        case WRE_PREEMPT:
            sci_preempt = SCI_PREEMPT;
            break;
        default:
            sci_preempt = SCI_NO_PREEMPT;
            break;
    }

    switch(auto_start)
    {
        case WRE_AUTO_START:
            sci_auto_start = SCI_AUTO_START;
            break;
        default:
            sci_auto_start = SCI_DONT_START;
            break;
    }

    thread_id = SCI_CreateAppThread(thread_name, queue_name, entry, argc, argv, stack_size, queue_num, sci_priority, sci_preempt, sci_auto_start);
    if(SCI_INVALID_BLOCK_ID == thread_id)
    {
        thread_id = WRE_INVALID_BLOCK_ID;
    }

    return thread_id;
}

uint32 WRE_DeleteThread(uint32 thread_id)
{
    return SCI_DeleteThread(thread_id);
}

uint32 WRE_ResumeThread(uint32 thread_id)
{
    return SCI_ResumeThread(thread_id);
}

uint32 WRE_IdentifyThread(void)
{
    return SCI_IdentifyThread();
}


BOOLEAN MMK_IsMocorAppTask(void)
{
    BLOCK_ID    thread_id;
    thread_id = SCI_IdentifyThread();
    
    if(P_APP != thread_id)
    {
        return FALSE;
    }
    return TRUE;
}


uint32 WRE_TerminateThread(uint32 thread_id)
{
    return SCI_TerminateThread(thread_id);
}

void WRE_ThreadExit(void)
{
    SCI_ThreadExit();
}

uint32 WRE_ChangeThreadPriority(uint32 thread_id, uint32 new_priority, uint32 *old_priority_ptr)
{
    uint32 ret = 0;
    uint32 prev_pri = 0;

    ret = SCI_ChangeThreadPriority(thread_id, SCI_SYSTEM_PRI(new_priority), &prev_pri);
    *old_priority_ptr = WRE_PRI_OLD(prev_pri);
    return ret;
}

uint32 WRE_GetThreadInfo(uint32 thread_id, char *thread_name, char *queue_name, uint32 *state, uint32 *priority, uint32 *preempt)
{
    uint32 retval = WRE_SUCCESS;
    uint32 prev_pri = 0;

    retval = SCI_GetThreadInfo(thread_id, thread_name, queue_name, state, &prev_pri, preempt);
    *priority = WRE_PRI_OLD(prev_pri);
    if(retval != SCI_SUCCESS)
    {
        retval = WRE_ERROR;
    }
    else
    {
        retval = WRE_SUCCESS;
    }

    return retval;
}

uint32 WRE_SuspendThread(uint32 thread_id)
{
    return SCI_SuspendThread(thread_id);
}

void WRE_Sleep(uint32 time_in_ms)
{
    SCI_Sleep(time_in_ms);
}

uint32 WRE_GetTickCount(void)
{
    return SCI_GetTickCount();
}

#endif
