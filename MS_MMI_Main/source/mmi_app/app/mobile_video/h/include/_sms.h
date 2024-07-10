#ifndef __SMS_H
#define __SMS_H

#include "_Types.h"

#if __cplusplus
extern "C"
{
#endif

typedef enum
{
	ESMEEvent_NewMessage,
	
	ESMEEventCount
}TSMSEvent;

typedef _BOOLEAN (*SMSCallbackProc)(_UINT uEvent, _UINT32 dwParam1, _UINT32 dwParam2, _VOID* pSMSCallbackProcParam);

extern _HANDLE SMS_AppendCallbackFunc(_INT nPriority, SMSCallbackProc pSMSCallbackProc,_VOID* pSMSCallbackProcParam);
extern _VOID SMS_RemoveCallbackFunc(_HANDLE hSMSHandle);

extern _INT SMS_GetMessageCount(_HANDLE hSMSHandle);
extern _BOOLEAN SMS_Send(_HANDLE hSMSHandle, _WCHAR* wstrTelNum, _BYTE* pBuf, _INT nBufSize);
extern _BOOLEAN SMS_GetMessage(_HANDLE hSMSHandle, _INT nIndex, _BYTE* pBuf, _INT nBufSize);
extern _BOOLEAN SMS_DelMessage(_HANDLE hSMSHandle, _INT nIndex);

extern _BOOLEAN SMS_ParseMessageID(_CONST _WCHAR* wstrMessage, _INT nLen, _WCHAR* wszNumber, _INT nSize);
extern _BOOLEAN Check_SMS(_UINT32 dwParam1, _UINT32 dwParam2, _TIME ntime);

extern _BOOLEAN    CM_Check_SMS(_UINT32 dwParam1, _UINT32 dwParam2, _TIME ntime);
extern _BOOLEAN Check_PhoneNumber(_WCHAR *pSMSMail);

#if __cplusplus
}
#endif

#endif
