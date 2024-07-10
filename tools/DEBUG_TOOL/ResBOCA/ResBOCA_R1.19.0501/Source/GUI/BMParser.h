// BMParser.h: interface for the CBMParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMPARSER_H__94C21162_3333_4F03_AE04_3F99B89F6BEA__INCLUDED_)
#define AFX_BMPARSER_H__94C21162_3333_4F03_AE04_3F99B89F6BEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(push,3)
#include <vector>
#pragma warning(pop)

#define BKMK_MAGIC  0x4B4D4B42

#define BKMK_NAME_LEN		16
#define BKMK_URL_LEN	    512
#define BKMK_MAX_NUM        0xFFFF
#define BKMK_VER            1

typedef struct _BKMK_FILE_HEAD_T
{
	DWORD magic;
	WORD  version;
	WORD  nCount;	
	_BKMK_FILE_HEAD_T()
	{
		memset(this,0,sizeof(_BKMK_FILE_HEAD_T));
	}
}BKMK_FILE_HEAD_T;

typedef struct _BOOKMARK_T
{ 
	DWORD active_flag;
	WORD  nMcc;
	WORD  nMnc;
    WCHAR name[BKMK_NAME_LEN+1];//书签名称 最长16字符
    WCHAR url[BKMK_URL_LEN+1];  //书签地址 最长512byte
	_BOOKMARK_T()
	{
		Init();
	}
	void Init()
	{
		memset(this,0,sizeof(_BOOKMARK_T));
		_tcscpy(url,_T("http://"));
	}

}BOOKMARK_T;//书签管理的结构




class CBMParser  
{
public:
	CBMParser();
	virtual ~CBMParser();
	BOOL DecodeBKMK(LPBYTE pBuf,DWORD dwSize);
	BOOL EncodeBKMK(LPBYTE &pBuf,DWORD &dwSize);

public:
	std::vector<BOOKMARK_T>			m_vBKMK;
	BKMK_FILE_HEAD_T	m_tHead;

private: 
	BOOL DecodeString(LPBYTE &pBuf, LPBYTE pEnd, WCHAR *pString,WORD wStrMaxLen,BOOL bUnicode=FALSE);
	BOOL EncodeString(LPBYTE &pBuf, WCHAR *pString,BOOL bUnicode=FALSE);
	VOID Clear();
};

#endif // !defined(AFX_BMPARSER_H__94C21162_3333_4F03_AE04_3F99B89F6BEA__INCLUDED_)
