#ifndef     _MMIWRE_THREAD_H_
#define     _MMIWRE_THREAD_H_

// Auto start option on thread
#define WRE_DONT_START              0
#define WRE_AUTO_START              1

// Indicates if the thread is preemptable.
#define WRE_NO_PREEMPT              0
#define WRE_PREEMPT                 1

// Auto start option on timer.
#define WRE_NO_ACTIVATE             0
#define WRE_AUTO_ACTIVATE           1

// Thread priority define
#define WRE_PRIORITY_KERNEL         0 
#define WRE_PRIORITY_TIME_CRITICAL  5 
#define WRE_PRIORITY_HIGHEST        10
#define WRE_PRIORITY_ABOVE_NORMAL   15
#define WRE_PRIORITY_NORMAL         20
#define WRE_PRIORITY_BELOW_NORMAL   24
#define WRE_PRIORITY_LOWEST         28
#define WRE_PRIORITY_IDLE           31

#define WRE_INVALID_BLOCK_ID        0xFFFFFFFF

#define WRE_SUCCESS                 0x00
#define WRE_ERROR                   0x01


void WRE_DisableIRQ(void);
void WRE_RestoreIRQ(void);
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
    );
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
    );
uint32 WRE_DeleteThread(uint32 thread_id);
uint32 WRE_ResumeThread(uint32 thread_id);
uint32 WRE_IdentifyThread(void);
uint32 WRE_TerminateThread(uint32 thread_id);
void   WRE_ThreadExit(void);
uint32 WRE_ChangeThreadPriority(uint32 thread_id, uint32 new_priority, uint32 *old_priority_ptr);
uint32 WRE_GetThreadInfo(uint32 thread_id, char *thread_name, char *queue_name, uint32 *state, uint32 *priority, uint32 *preempt);
uint32 WRE_SuspendThread(uint32 thread_id);
void   WRE_Sleep(uint32 time_in_ms);
uint32 WRE_GetTickCount(void);
BOOLEAN MMK_IsMocorAppTask(void);

#endif
