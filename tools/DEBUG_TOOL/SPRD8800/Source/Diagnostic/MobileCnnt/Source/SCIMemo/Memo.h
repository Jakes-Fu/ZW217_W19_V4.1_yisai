// Memo.h: interface for the CMemo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMO_H__8951D8E6_50B4_4D2D_8478_A9EC7DB92402__INCLUDED_)
#define AFX_MEMO_H__8951D8E6_50B4_4D2D_8478_A9EC7DB92402__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IMemo.h"
#include "mmiatc.h"
#include "Log.h"
#include "Converter.h"

class CMemo : public IMemo  
{
public:
    virtual BOOL   StartLog( LPCTSTR pszFileName );
    virtual void   StopLog( void );

	virtual void   SetPort( IPort * pPort );
	virtual BOOL   Init(void);

	virtual BOOL   SelectStorePlace( WORD wStorePlace );
	virtual BOOL   GetMemoNum( int * pUsedNum, int * pTotalNum );
	virtual BOOL   GetMemoParam(DWORD dwFlag,DWORD **ppParam );
	virtual BOOL   SetMemoParam(DWORD dwFlag,DWORD *pParam );
	virtual _MEMO *  GetMemo( int nUsedIdx);

	virtual BOOL   AddMemo( _MEMO * pMemo );
	virtual BOOL   ModifyMemo( _MEMO * pMemo );
	virtual BOOL   DeleteMemo( _MEMO * pMemo );

	virtual _DATE_TIME_T *   GetPhoneDate( void );
    virtual BOOL   SetPhoneDate(_DATE_TIME_T * pDateTime);

	virtual void   Release(void);
public:
	CMemo();
	virtual ~CMemo();
protected:
	BOOL ExecuteCmd(LPCSTR lpCmd);

	void ConvertMemoToMemoET( _MEMO * pMemo , MEMO_ENTRY_T & pet);
	LPSTR FindStr(LPCSTR pSrc, int nSrcLen, LPCSTR pDes, int nDesLen);
	__inline char  GetMMIUsedStorePlace(WORD wStorePlace);

	__inline int AsciiToUcs2( LPCSTR lpszSrc,  int nSrcLen, WCHAR * lpszDes, int nDesSize );
	__inline int Ucs2ToAscii( LPCWSTR lpszSrc, int nSrcLen, LPSTR lpszDes, int nDesSize );

	__inline int GetUnusedID(WORD wStore);
	BOOL HasUcs2Char(LPCSTR pStr) const;
	BOOL HasUcs2Char(LPCTSTR pStr) const;
	
	void CompressZero(char *pSrc, int nSrcLen,char * pDstBuf, int &nDstSize);
	void UnCompressZero(char *pSrc, int nSrcLen, char * pDstBuf, int &nDstSize);

private:
    CLog    m_Log;
	IPort * m_pPort;
	_MEMO   * m_pMemo;
    _DATE_TIME_T * m_pDateTime;
	WORD  m_wStorePlace;
	int   m_nUsedNum;
	int   m_nTotalNum;

	DWORD m_dwVersion;

    CConverter m_convtr;

	BOOL m_bAT; // TRUE, AT+; FALSE AT##

	CUIntArray m_arrMSID;
	CUIntArray m_arrSIMID;
	
	DWORD m_dwRead;

	enum { 
        MAX_BUF_LEN = 512, 
    };
    char m_szInBuf[MAX_BUF_LEN + 1];
	char m_szOutBuf[MAX_BUF_LEN + 1];

	char m_szCmpCmd[MAX_BUF_LEN + 1];

	//teana hu 2010.05.06
	_TCHAR m_szCfgPathName[MAX_PATH];
	BOOL m_bBigEndian;
	//

	int      m_nRetryTimes;
	DWORD    m_nReadOneTimeout;

	static const int  MMI_ID_LEN;
	static const char MMI_GET_INFO[];
	static const char MMI_GET_MEMO[];
	static const char MMI_ADD_MEMO[];
	static const char MMI_MODIFY_MEMO[];
	static const char MMI_DELETE_MEMO[];

	static const char MMI_MEMO_STORE_SIM;
	static const char MMI_MEMO_STORE_MS;
	
	static const char AT_GET_PHONE_DATE[];
	static const char AT_SET_PHONE_DATE[];
	static const char AT_DATE_RET_PREFIX[];



	static const char MMI_LR[];
	static const char MMI_PREFIX[];
	static const char AT_RES_OK[];
    static const char AT_RES_ERROR[];
};

#endif // !defined(AFX_MEMO_H__8951D8E6_50B4_4D2D_8478_A9EC7DB92402__INCLUDED_)
