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
		BOOLEAN is_ucs2;        //字符串的编码类型
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
		BOOLEAN is_ucs2;                     // 字符串的编码类型
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

	__inline int AsciiToUcs2( LPCSTR lpszSrc, WCHAR * lpszDes, int nDesSize );
	__inline int Ucs2ToAscii( LPCWSTR lpszSrc, int nSrcLen, LPSTR lpszDes, int nDesSize );

	__inline int GetUnusedID(CUIntArray * pArrID);
#ifndef _UNICODE
	BOOL HasUcs2Char(LPCSTR pStr) const;
#else
	BOOL HasUcs2Char(LPCTSTR pStr) const;
#endif
	
	void CompressZero(char *pSrc, int nSrcLen,char * pDstBuf, int &nDstSize);
	void UnCompressZero(char *pSrc, int nSrcLen, char * pDstBuf, int &nDstSize);


	BOOL IsUSIM();
	__inline int GetNextUsedID(CUIntArray * pArrID);



	_PB *  GetPBStd( int nUsedIdx);
	BOOL   AddPBStd( _PB * pPB );
	BOOL   ModifyPBStd( _PB * pPB );
	BOOL   DeletePBStd( _PB * pPB );

	_PB *  GetPBEx( int nUsedIdx);
	BOOL   AddPBEx( _PB * pPB );
	BOOL   ModifyPBEx( _PB * pPB );
	BOOL   DeletePBEx( _PB * pPB );
	BOOL   WritePBEx(_PB * pPB, int nMode); // 0:add,1:modify,2:delete

	BOOL   WritePB(_PB * pPB, int nMode); // 0:add,1:modify,2:delete

	__inline CUIntArray *GetIDArray(WORD wStore);

	int  Transfer7Bitsto8Bits( LPBYTE tpdu_ptr, int sms_length, LPBYTE offset_octets_ptr,
                               LPBYTE offset_bits_ptr, LPBYTE user_data_ptr );
    BYTE GetBits( LPBYTE byte_in, BYTE bits_length, LPBYTE offset_bits, LPBYTE offset_octets );	
	int  Transfer8Bitsto7Bits(const char* pSrc, unsigned char* pDst, int nSrcLength);
	BYTE Map7BitCharTo8Bit(LPBYTE lpData_in);

	BOOL GetUsedIDEx();


private:
	CConverter m_convtr;
    CLog    m_Log;
	IPort * m_pPort;
	_PB   * m_pPB;

	WORD  m_wStorePlace;
	int   m_nUsedNum;
	int   m_nTotalNum;

	CUIntArray m_arrMSID;
	CUIntArray m_arrSIMID;
	CUIntArray m_arrSIMID2;
	
	DWORD m_dwRead;

	DWORD m_dwVersion;

	CConverter m_converter;

	BOOL  m_bUSIM;

	enum { 
        MAX_BUF_LEN = 1152, 
    };
    char m_szInBuf[MAX_BUF_LEN + 1];
	char m_szOutBuf[MAX_BUF_LEN + 1];

	char m_szCmpCmd[MAX_BUF_LEN + 1];

	char m_szTempA[MAX_BUF_LEN + 1];
	_TCHAR m_szTempW[MAX_BUF_LEN + 1];

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

	static const char MMI_LR[];
	static const char MMI_PREFIX[];
	static const char AT_RES_OK[];
    static const char AT_RES_ERROR[];

	///////////////////////////////////////
	static const char  AT_SET_STORE_PLACE_SM[];
	static const char  AT_SET_STORE_PLACE_ME[];
	static const char  AT_GET_PB_NUM[];
	static const char  AT_GET_PB_INFO[];

	
	static const char AT_READ_ALL_PB[];
	static const char AT_READ_ONE_PB[];
	static const char AT_READ_FIRST[];
	static const char AT_READ_NEXT[];
	static const char AT_READ_NEXT_RET_PRE[];
	static const char AT_WRITE_PB[];
	static const char AT_DELETE_PB[];

	static const char  AT_GET_PB_NUM_RET_PR[];
	static const char  AT_GET_PB_INFO_RET_PRE[]; 
	static const char AT_READ_PB_RET_PRE[];

	static const char AT_SET_PB_TYPE[];

	static const char AT_GET_CARD_MODE[];
	static const char AT_GET_CARD_MODE_RET_PRE[];

	static const char AT_READ_PB_EX[];
	static const char AT_READ_PB_EX_RET_PRE[];

	static const char AT_WRITE_PB_EX[];
	static const char AT_DELETE_PB_EX[];

	static const char AT_GET_PB_USED_ID_EX[];
	
};

#endif // !defined(AFX_PB_H__FDF54D1C_C542_447E_907F_C7CD63CC1803__INCLUDED_)
