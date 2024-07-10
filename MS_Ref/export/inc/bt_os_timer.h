#ifndef __OS_TIMER_H__
#define  __OS_TIMER_H__
#include "os_api.h"

/* System time, in microseconds. */
typedef uint32                TIME;
typedef uint32                UNSIGNED;
typedef uint32                  scheduler_identifier   ;
typedef scheduler_identifier    tid;


void deleteThreadxTimer(uint32 *timer);
void get_timed_message(UNSIGNED id);

#if 0
void OS_StartTimeOut(TIMER_FUN func, uint32 delay_ms);
void OS_DeleteTimeOut(void);
BOOLEAN  OS_IsTimeOutStart(void);
void OS_StopTimeOut(void);
#endif
BOOLEAN OS_StartTimer(uint32 *timer_id_ptr, uint32 delay, uint32 cbParam);
void OS_DeleteTimer(uint32 *timer);
uint32 OS_GetSystemCounterTime(void);

tid timed_event_in(TIME delay, void (*fn)(uint16, void *), uint16 fniarg, void *fnvarg);
BOOLEAN cancel_timed_event(tid eventid, uint16 *pmi, void **pmv);

TIME get_time(void);

#endif /* __OS_TIMER_H__*/
