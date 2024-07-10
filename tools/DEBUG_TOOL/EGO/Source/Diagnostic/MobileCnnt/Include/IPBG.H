// IPBG.h: interface
//
//////////////////////////////////////////////////////////////////////
#if !defined(__PBG_H_)
#define __PBG_H_

#pragma once

struct _PBGROUP;
class IPort;

#define PBG_PARAM_VERSION  1

class IPBG
{
public:

    virtual BOOL		StartLog( LPCTSTR pszFileName ) = 0;
    virtual void		StopLog( void ) = 0;

	virtual void		SetPort( IPort * pPort ) = 0;
	virtual BOOL		Init(void)               = 0;

	virtual BOOL		SelectStorePlace( WORD wStorePlace ) = 0;
	virtual BOOL		GetPBGNum( int * pUsedNum, int * pTotalNum ) = 0;
	virtual BOOL		GetPBGParam(DWORD dwFlag,DWORD **ppParam )=0;
	virtual BOOL		SetPBGParam(DWORD dwFlag,DWORD *pParam )=0;
    virtual _PBGROUP *   GetPBG( int nUsedIdx) = 0;

	virtual BOOL		AddPBG( _PBGROUP * pPBGroup )    = 0;
	virtual BOOL		ModifyPBG( _PBGROUP * pPBGroup ) = 0;
	virtual BOOL		DeletePBG( _PBGROUP * pPBGroup ) = 0;

	virtual void		Release(void) = 0;
public:
	IPBG() {};
	virtual ~IPBG() {};
};

#ifdef PBG_EXPORTS
	#define PBG_API extern "C" __declspec(dllexport)
#else
	#define PBG_API extern "C" __declspec(dllimport)
#endif

PBG_API BOOL CreatePBG( IPBG ** pPBG );

#endif // __PBG_H_