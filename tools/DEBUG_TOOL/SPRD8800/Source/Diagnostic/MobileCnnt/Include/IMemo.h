
#if !defined(__MEMO_H_)
#define __MEMO_H_

#pragma once

struct _MEMO;
struct _DATE_TIME_T;
class IPort;

#define MEMO_PARAM_VERSION  1

class IMemo
{
public:

    virtual BOOL   StartLog( LPCTSTR pszFileName ) = 0;
    virtual void   StopLog( void ) = 0;

	virtual void   SetPort( IPort * pPort ) = 0;
	virtual BOOL   Init(void)               = 0;

	virtual BOOL   SelectStorePlace( WORD wStorePlace ) = 0;
	virtual BOOL   GetMemoNum( int * pUsedNum, int * pTotalNum ) = 0;
	virtual BOOL   GetMemoParam(DWORD dwFlag,DWORD **ppParam )=0;
	virtual BOOL   SetMemoParam(DWORD dwFlag,DWORD *pParam )=0;
    virtual _MEMO *  GetMemo( int nUsedIdx) = 0;

	virtual BOOL   AddMemo( _MEMO * pMemo )    = 0;
	virtual BOOL   ModifyMemo( _MEMO * pMemo ) = 0;
	virtual BOOL   DeleteMemo( _MEMO * pMemo ) = 0;
	virtual _DATE_TIME_T *   GetPhoneDate( void ) = 0;
    virtual BOOL   SetPhoneDate(_DATE_TIME_T * pDateTime) = 0;

	virtual void   Release(void) = 0;
public:
	IMemo() {};
	virtual ~IMemo() {};
};

#ifdef MEMO_EXPORTS
	#define MEMO_API extern "C" __declspec(dllexport)
#else
	#define MEMO_API extern "C" __declspec(dllimport)
#endif

MEMO_API BOOL CreateMemo( IMemo ** pMemo );

#endif // __MEMO_H_