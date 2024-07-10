#ifndef _PROCESSFLOWLIB_H_
#define _PROCESSFLOWLIB_H_
#include "GeneralParam.h"


#ifdef PROCESSFLOW_H
#define PROCESSFLOW_API extern "C" _declspec(dllexport)
#else
#define PROCESSFLOW_API extern "C" _declspec(dllimport)
#endif

PROCESSFLOW_API BOOL CheckProcessEnable(void);
PROCESSFLOW_API BOOL InitProcessFlow(HWND hwnd, LPTSTR p1OrderNum, int *SnLength, LPTSTR p1Msg);
PROCESSFLOW_API	BOOL ConnectDB(LPSTR p1Msg);
PROCESSFLOW_API	void DisconnectDB(void);

PROCESSFLOW_API BOOL WriteDataToFlow(LPCTSTR p1Sn, LPCTSTR p1Infor, LPCTSTR p1StatusType,
				   LPCTSTR p1Date, LPCTSTR p1Elapse, LPCTSTR p1ErrType, LPCTSTR p1ErrCode,
				   LPCTSTR p1ErrMsg, LPCTSTR p1SwVer, LPCTSTR p1TsVer, LPTSTR p1Msg);
PROCESSFLOW_API BOOL SearchLastSNStatusFrFlow(LPCTSTR p1Sn, LPTSTR p1Msg);

PROCESSFLOW_API BOOL ReadDataFrFlow(int *p1nId, LPTSTR p1Sn, LPTSTR p1Project, LPTSTR p1OrderNum, LPTSTR p1Infor, LPTSTR p1StatusType,
					   LPTSTR p1CurStation, LPTSTR p1Address, LPTSTR p1StartDate, LPTSTR p1EndDate, LPTSTR p1ErrType,
						LPTSTR p1ErrCode, LPTSTR p1ErrMsg, LPTSTR p1SwVer, LPTSTR p1TsVer, LPTSTR p1Time, LPTSTR p1Msg);
PROCESSFLOW_API BOOL StatisticFrFlow(int *p1nCount, int *p1nPass, int *p1nOnePass, int *p1nRePass,LPTSTR p1Project, LPTSTR p1OrderNum, LPTSTR p1CurStation, LPTSTR p1SDate, LPTSTR p1EDate,LPTSTR p1Msg);
PROCESSFLOW_API BOOL SearchErrListFrFlow(LPTSTR p1errType, LPTSTR p1errCode, LPTSTR p1Msg);
PROCESSFLOW_API BOOL ExitProcessFlow(void);

PROCESSFLOW_API BOOL SearchAccountFromDB(LPSTR p1Account, LPSTR p1Password, LPSTR p1Msg);
PROCESSFLOW_API BOOL SearchPrivilegeFromDB(LPSTR p1Account, LPSTR p1Password, LPSTR p1Privilege, LPSTR p1Msg);

#endif