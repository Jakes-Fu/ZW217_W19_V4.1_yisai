// PBG.h: interface for the CPBG class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PBG_H__B37CC89D_AB60_4830_AF40_34D854AFC268__INCLUDED_)
#define AFX_PBG_H__B37CC89D_AB60_4830_AF40_34D854AFC268__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IPBG.h"
#include "mmiatc.h"
#include "Log.h"
#include "Converter.h"

class CPBG : public IPBG
{
public:
    virtual BOOL   StartLog( LPCTSTR pszFileName );
    virtual void   StopLog( void );

	virtual void   SetPort( IPort * pPort );
	virtual BOOL   Init(void);

	virtual BOOL   SelectStorePlace( WORD wStorePlace );
	virtual BOOL   GetPBGNum( int * pUsedNum, int * pTotalNum );
	virtual BOOL		GetPBGParam(DWORD dwFlag,DWORD **ppParam );
	virtual BOOL		SetPBGParam(DWORD dwFlag,DWORD *pParam );
	virtual _PBGROUP *  GetPBG( int nUsedIdx);

	virtual BOOL   AddPBG( _PBGROUP * pPBGroup );
	virtual BOOL   ModifyPBG( _PBGROUP * pPBGroup );
	virtual BOOL   DeletePBG( _PBGROUP * pPBGroup );

	virtual void   Release(void);
public:
	CPBG();
	virtual ~CPBG();
protected:
	BOOL ExecuteCmd(LPCSTR lpCmd);

	void ConvertPBGroupToGET( _PBGROUP * pPBGroup, GROUP_ENTRY_T & pet);
	__inline LPSTR FindStr(LPCSTR pSrc, int nSrcLen, LPCSTR pDes, int nDesLen);
	__inline char  GetMMIUsedStorePlace(WORD wStorePlace);

	__inline int AsciiToUcs2( LPCSTR lpszSrc,  int nSrcLen, WCHAR * lpszDes, int nDesSize );
	__inline int Ucs2ToAscii( LPCWSTR lpszSrc, int nSrcLen, LPSTR lpszDes, int nDesSize );

	__inline int GetUnusedID(WORD wStore);
#ifdef _UNICODE
	BOOL HasUcs2Char(LPCWSTR pStr) const;
#else
	BOOL HasUcs2Char(LPCSTR pStr) const;
#endif
	
	
	
	void CompressZero(char *pSrc, int nSrcLen,char * pDstBuf, int &nDstSize);
	void UnCompressZero(char *pSrc, int nSrcLen, char * pDstBuf, int &nDstSize);

private:
    CLog    m_Log;
	IPort * m_pPort;
	_PBGROUP   * m_pPBG;
	CConverter m_convtr;

	WORD  m_wStorePlace;
	int   m_nUsedNum;

	DWORD m_dwVersion;

	CUIntArray m_arrMSID;
	CUIntArray m_arrSIMID;
	
	DWORD m_dwRead;

	enum { 
        MAX_BUF_LEN = 512, 
    };
    char m_szInBuf[MAX_BUF_LEN + 1];
	char m_szOutBuf[MAX_BUF_LEN + 1];

	char m_szCmpCmd[MAX_BUF_LEN + 1];

	char m_szTempA[MAX_BUF_LEN + 1];
	_TCHAR m_szTempW[MAX_BUF_LEN + 1];
	
	//teana hu 2010.05.06
	_TCHAR m_szCfgPathName[MAX_PATH];
	BOOL m_bBigEndian;
	//

	int   m_nRetryTimes;
	DWORD m_nReadOneTimeout;

	static const int  MMI_ID_LEN;
	static const char MMI_GET_INFO[];
	static const char MMI_GET_PBG[];
	static const char MMI_ADD_PBG[];
	static const char MMI_MODIFY_PBG[];
	static const char MMI_DELETE_PBG[];

	static const char MMI_PBG_STORE_SIM;
	static const char MMI_PBG_STORE_MS;

	static const char MMI_LR[];
	static const char MMI_PREFIX[];
	static const char AT_RES_OK[];
    static const char AT_RES_ERROR[];
};

#endif // !defined(AFX_PBG_H__B37CC89D_AB60_4830_AF40_34D854AFC268__INCLUDED_)
