//
//	_Timer.h
//
#ifndef	__TIMER_API_H
#define	__TIMER_API_H

#ifdef	__cplusplus
extern "C"
{
#endif

typedef _VOID (*TimerCallbackProc)(_HANDLE, _VOID*);

_HANDLE _SetTimer(_INT, TimerCallbackProc, _VOID* pParam);
_VOID _CancelTimer(_HANDLE);


typedef _VOID (*TaskCallbackProc)(_HANDLE, _VOID*);
_HANDLE _SetScheduleTask(_INT, _INT, TaskCallbackProc, _VOID*);
_VOID _CancelScheduleTask(_HANDLE);

#ifdef	__cplusplus
}
#endif

#endif
