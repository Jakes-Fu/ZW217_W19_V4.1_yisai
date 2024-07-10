// PB.h: interface for the CPB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PB_H__FDF54D1C_C542_447E_907F_C7CD63CC1803__INCLUDED_)
#define AFX_PB_H__FDF54D1C_C542_447E_907F_C7CD63CC1803__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IPB.h"
#include "mmiatc.h"
#include "Log.h"
#include "Converter.h"

namespace MMI_PB_V1
{
    #define MMI_SYNC_PB_NAME_MAX_LEN_V1 241
	typedef struct _PB_NAME_T
	{
		uint8 is_ucs2;        //字符串的编码类型
		uint8   alpha_len;      //alpha数组的BYTE个数
		uint8   alpha[MMI_SYNC_PB_NAME_MAX_LEN_V1];      //alph string
	}PB_NAME_T;
	
	
	typedef struct PB_NUMBER_T
	{
		uint8   number_len;     //number数组的BYTE个数
		uint8   number[MMI_SYNC_PB_NUM_MAX_LEN];     //字符串
	}PB_NUMBER_T;
	
	// 电话簿中的邮箱
	typedef struct _PB_MAIL_T
	{
		uint8 is_ucs2;                     // 字符串的编码类型
		uint8   mail_len;                    // mail数组的BYTE个数
		uint8   mail[MMI_SYNC_PB_MAX_MAIL_LEN];    // 具体的mail地址
	} PB_MAIL_T;
	
	typedef struct _PB_PHONEBOOK_ENTRY_T
	{
		uint16          entry_id;       //记录的id号
		PB_NAME_T		entry_name;     //姓名
		PB_NUMBER_T	    number_t[MMI_SYNC_PB_MAX_NV_PHONE_NUM];    //电话号码，依次为个人、住宅和公司
		PB_MAIL_T	    mail;           //邮箱地址
		uint8           group_id;
	}PB_PHONEBOOK_ENTRY_T;
}

class CPB : public IPB  
{
public:
    virtual BOOL   StartLog( LPCTSTR pszFileName );
    virtual void   StopLog( void );

	virtual void   SetPort( IPort * pPort );
	virtual BOOL   Init(void);

	virtual BOOL   SelectStorePlace( WORD wStorePlace );
	virtual BOOL   GetPBNum( int * pUsedNum, int * pTotalNum, BOOL bClear = FALSE );
	virtual BOOL   GetPBParam(DWORD dwFlag,DWORD *ppParam );
	virtual BOOL   SetPBParam(DWORD dwFlag,DWORD *pParam );
	virtual _PB *  GetPB( int nUsedIdx);

	virtual BOOL   ReadAllPB(int &nCount);	
	virtual LPCSTR GetPBType();
	virtual BOOL   SetPBType(DWORD dwFlag);

	virtual BOOL   AddPB( _PB * pPB );
	virtual BOOL   ModifyPB( _PB * pPB );
	virtual BOOL   DeletePB( _PB * pPB );

	virtual void   Release(void);
public:
	CPB();
	virtual ~CPB();
protected:
	BOOL ExecuteCmd(LPCSTR lpCmd);

	void ConvertPBToPBET( _PB * pPB, MMI_PB_V1::PB_PHONEBOOK_ENTRY_T & pet);
	void ConvertPBETV1ToV0( MMI_PB_V1::PB_PHONEBOOK_ENTRY_T * pPET1, PB_PHONEBOOK_ENTRY_T & pet0);
	__inline LPSTR FindStr(LPCSTR pSrc, int nSrcLen, LPCSTR pDes, int nDesLen);
	__inline char  GetMMIUsedStorePlace(WORD wStorePlace);

	__inline int AsciiToUcs2( LPCSTR lpszSrc, int nSrcLen, WCHAR * lpszDes, int nDesSize );
	__inline int Ucs2ToAscii( LPCWSTR lpszSrc, int nSrcLen, LPSTR lpszDes, int nDesSize );

	__inline UINT GetUnusedID(WORD wStore);
	BOOL HasUcs2Char(LPCSTR pStr) const;
	BOOL HasUcs2Char(LPCTSTR pStr) const;
	
	void CompressZero(char *pSrc, int nSrcLen,char * pDstBuf, int &nDstSize);
	void UnCompressZero(char *pSrc, int nSrcLen, char * pDstBuf, int &nDstSize);

	_PB * _GetPBV1(int nUsedIdx);  // name length 20
	_PB * _GetPBV2(int nUsedIdx);  // name length 241

	CUIntArray* GetIDArrayPtr(WORD wStore);
	void        AddID(CUIntArray* pIDArray, int nID);
private:
	CConverter m_convtr;
    CLog    m_Log;
	IPort * m_pPort;
	_PB   * m_pPB;

	WORD  m_wStorePlace;
	int   m_nUsedNum;

	CUIntArray m_arrMSID;
	CUIntArray m_arrSIMID;
	CUIntArray m_arrSIMID2;
	CUIntArray m_arrSIMID3;
	CUIntArray m_arrSIMID4;

	DWORD m_dwRead;

	DWORD m_dwVersion;

	enum { 
        MAX_BUF_LEN = 1152, 
    };
    char m_szInBuf[MAX_BUF_LEN + 1];
	char m_szOutBuf[MAX_BUF_LEN + 1];

	char m_szCmpCmd[MAX_BUF_LEN + 1];

	char m_szTempA[MAX_BUF_LEN + 1];
	_TCHAR m_szTempW[MAX_BUF_LEN + 1];
	
	//teana hu 2010.05.06
	_TCHAR  m_szCfgPathName[_MAX_PATH];
	BOOL m_bBigEndian;
	//

	int      m_nRetryTimes;
	DWORD    m_nReadOneTimeout;

	static const int  MMI_ID_LEN;
	static const char MMI_GET_INFO[];
	static const char MMI_GET_PB[];
	static const char MMI_ADD_PB[];
	static const char MMI_MODIFY_PB[];
	static const char MMI_DELETE_PB[];
	
	static const char MMI_PB_STORE_MS;
	static const char MMI_PB_STORE_SIM;
	static const char MMI_PB_STORE_SIM2;
	static const char MMI_PB_STORE_SIM3;
	static const char MMI_PB_STORE_SIM4;

	static const char MMI_LR[];
	static const char MMI_PREFIX[];
	static const char AT_RES_OK[];
    static const char AT_RES_ERROR[];
};

#endif // !defined(AFX_PB_H__FDF54D1C_C542_447E_907F_C7CD63CC1803__INCLUDED_)
