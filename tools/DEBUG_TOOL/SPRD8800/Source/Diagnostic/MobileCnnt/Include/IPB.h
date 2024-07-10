
#if !defined(__PB_H_)
#define __PB_H_

#pragma once

struct _PB;
class IPort;

#define PB_PARAM_NAME_LEN 0
#define PB_PARAM_VERSION  1
#define PB_PARAM_IS_USIM  2

class IPB
{
public:

    virtual BOOL   StartLog( LPCTSTR pszFileName ) = 0;
    virtual void   StopLog( void ) = 0;

	virtual void   SetPort( IPort * pPort ) = 0;
	virtual BOOL   Init(void)               = 0;

	virtual BOOL   SelectStorePlace( WORD wStorePlace ) = 0;
	virtual BOOL   GetPBNum( int * pUsedNum, int * pTotalNum , BOOL bClear = FALSE) = 0;
	virtual BOOL   GetPBParam(DWORD dwFlag,DWORD *ppParam )=0;
	virtual BOOL   SetPBParam(DWORD dwFlag,DWORD *pParam )=0;
    virtual _PB *  GetPB( int nUsedIdx) = 0;
    virtual BOOL   ReadAllPB(int &nCount)=0;

	virtual LPCSTR GetPBType()=0;
	virtual BOOL SetPBType(DWORD dwFlag)=0;


	virtual BOOL   AddPB( _PB * pPB )    = 0;
	virtual BOOL   ModifyPB( _PB * pPB ) = 0;
	virtual BOOL   DeletePB( _PB * pPB ) = 0;

	virtual void   Release(void) = 0;
public:
	IPB() {};
	virtual ~IPB() {};
};

#ifdef PB_EXPORTS
	#define PB_API extern "C" __declspec(dllexport)
#else
	#define PB_API extern "C" __declspec(dllimport)
#endif

PB_API BOOL CreatePB( IPB ** pPB );

#endif // __PB_H_