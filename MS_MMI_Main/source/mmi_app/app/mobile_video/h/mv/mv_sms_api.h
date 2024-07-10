#ifndef  _MV_SMS_API_H
#define  _MV_SMS_API_H

#include "_Types.h"

#include "mmisms_app.h"
#include "mn_type.h"
#include "_SMS.h"

#define  SMS_TIME_START_YEAR    2000
#define  SMS_TIME_START_MONTH   1
#define  SMS_TIME_START_DAY     1
#define  SMS_TIME_START_HOUR    0
#define  SMS_TIME_START_MINUTE  0
#define  SMS_TIME_START_SECOND  0

#if 0
_HANDLE CM_SMS_AppendCallbackFunc(
									_INT nPriority,
									SMSCallbackProc pSMSCallbackProc,
									_VOID* pSMSCallbackProcParam
);

_BOOLEAN CM_SMS_RemoveCallbackFunc(_HANDLE hSMSHandle);

#endif

//_BOOLEAN  CM_Check_SMS(_UINT32 dwParam1, _UINT32 dwParam2, _TIME ntime);

_BOOLEAN CM_IsMvSms(APP_SMS_USER_DATA_T* sms_user_data_ptr);
#endif