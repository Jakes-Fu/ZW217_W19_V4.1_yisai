//
//	_Time.h
//
#ifndef	__TIME_API_H
#define	__TIME_API_H

#ifdef	__cplusplus
extern "C"
{
#endif

struct tagSysTime
{
	_WORD wYear;
	_WORD wMonth;
	_WORD wDayOfWeek;
	_WORD wDayOfYear;
	_WORD wDay;
	_WORD wHour;
	_WORD wMinute;
	_WORD wSecond;
};
typedef struct tagSysTime	_SYSTIME;

//extern _INT _GetTimeZone();
//extern _BOOLEAN _SetTimeZone(_INT);
extern _TIME _GetCurrentTime();
extern _UINT32 _GetTickTime();
//extern _BOOLEAN _SetTime(_TIME);
extern _VOID _GetSystemTime(_TIME, _SYSTIME*);
//extern _TIME _GetTime(_SYSTIME*);
//extern _VOID _TimeFormat(_WCHAR*, _INT, _CONST _WCHAR*, _CONST _SYSTIME*);

//***2009-06-20
extern _TIME _GetServerTime();
extern _BOOLEAN _SetServerTime(_TIME);

#ifdef	__cplusplus
}
#endif

#endif
