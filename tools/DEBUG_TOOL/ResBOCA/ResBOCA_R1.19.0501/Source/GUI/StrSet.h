// StrSet.h: interface for the CStrSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRSET_H__E1D03AAF_B31C_49DA_8ED1_E2CC9895FEE4__INCLUDED_)
#define AFX_STRSET_H__E1D03AAF_B31C_49DA_8ED1_E2CC9895FEE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "IResConverter.h"
#pragma warning(push,3)
#include <algorithm>
#include <afxtempl.h>
#pragma warning(pop)

typedef CMap<CString, LPCTSTR, PX_TEXT_INFO_T, PX_TEXT_INFO_T> TEXT_MAP;
typedef CMap<CString, LPCTSTR, int, int>   STRINT_MAP;

typedef CStringArray* PSTRARRAY;
typedef CMap<CString, LPCTSTR, PSTRARRAY, PSTRARRAY> STRING_MAP;
typedef STRING_MAP* PSTRING_MAP;
typedef CMap<CString, LPCTSTR, CString, LPCTSTR>    STRID_MAPS;


class CStrSet  
{
public:
	CStrSet();
	virtual ~CStrSet();

	BOOL AddString(LPCTSTR pszID,CStringArray &agRow);
	BOOL RepString(LPCTSTR pszID,CStringArray *pagRow);

	void CalcTextCrossRepeat(int nBaseLangIndex,BOOL bOrg);
	int  GetTextRepeatIndex(LPCTSTR pszStr, int nLangIdx,BOOL bOrg);

	void ClearAll();
	void Clear(BOOL bOrg);

	BOOL GetStrData(LPBYTE &pBuf, DWORD &dwSize, DWORD dwBase,BOOL bBigEdn);

	

public:
	BOOL m_bAllUnicode;
	BOOL m_bStrNoFlag;
	int  m_nPrecomposedFlag;
	int  m_nLangNum;
	
	IResConverter *m_prcConverter;
	CStringArray    m_agOrgStrID;
	CStringArray    m_agDstStrID;

private:
	int GetOrgStr( LPCTSTR pszID, CStringArray &agRow );
	DWORD CalcTextInfo();
	__inline DWORD Addr4ByteAlign( DWORD dwAddr )
    {
        return ( (dwAddr + 3) & ~3 );
    }

	PX_TEXT_INFO_T GetTextInfo(int nIndex);
	WORD BigEdnToLitEdn( WORD wBigEdn );	
	BOOL BigEdnToLitEdn( LPWORD pwBigEdn, int nNum );	
	WORD LitEdnToBigEdn( WORD wLitEdn );	
	BOOL LitEdnToBigEdn( LPWORD pwLitEdn, int nNum );	
	DWORD ConvEdn_INT( DWORD dwEdn );

	TEXT_MAP        m_mapOrgStr;
	TEXT_MAP        m_mapDstStr;	

	STRINT_MAP      m_mapOrgStrInt[256];
	STRINT_MAP      m_mapDstStrInt[256];






};

#endif // !defined(AFX_STRSET_H__E1D03AAF_B31C_49DA_8ED1_E2CC9895FEE4__INCLUDED_)
