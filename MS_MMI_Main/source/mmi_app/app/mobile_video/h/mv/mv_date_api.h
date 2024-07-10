#ifndef  _MV_TIME_API_H
#define  _MV_TIME_API_H

#include "_Types.h"

typedef struct CM_tagSysTime	CM_SYSTIME;
struct CM_tagSysTime
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

_TIME CM_GetCurrentTime(void);

_UINT32 CM_GetTickTime(void);

_BOOLEAN CM_GetSystemTime(_TIME nTime,CM_SYSTIME* pSysTime);





#endif