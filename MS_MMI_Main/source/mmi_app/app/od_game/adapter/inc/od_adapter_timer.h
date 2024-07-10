#ifndef __OD_ADAPTER_TIMER_H__
#define	__OD_ADAPTER_TIMER_H__
#include "od_type_def.h"
typedef void (*OD_TIMER_CALLBACK)(void);
typedef void (*Od_TimerCallbackFunc)(void* data_ptr);
typedef short (*OD_FUNC_PTR)(void);
OD_U8 Od_CreateDaemonTimer(OD_U32 time_out,OD_BOOL is_period);            
OD_BOOL Od_StopDaemonTimer(OD_U8  timer_id);														  
OD_U8 Od_CreateTimer(OD_U32  time_out,OD_FUNC_PTR callback);
void Od_StopTimer(OD_U8 timer_id);
OD_BOOL Od_OnTimerProcess(OD_U8 timer_id);
OD_U8 Od_CreateTimerEx(OD_U32  time_out,Od_TimerCallbackFunc callback,void *data_ptr);
#define Od_StopTimerEx Od_StopTimer
#endif
