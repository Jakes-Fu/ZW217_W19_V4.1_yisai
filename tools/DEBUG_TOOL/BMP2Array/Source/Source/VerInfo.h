// VersionInfo.h: interface for the CVerInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERSIONINFO_H__B5A3B61F_C8B5_4CF3_A35D_E7A3BD56A300__INCLUDED_)
#define AFX_VERSIONINFO_H__B5A3B61F_C8B5_4CF3_A35D_E7A3BD56A300__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVerInfo  
{
public:
	LPCTSTR GetSpecialBuild(void);

	LPCTSTR GetPrivateBuild(void);

	LPCTSTR GetProductVersion(void);

	LPCTSTR GetProductName(void);

	LPCTSTR GetLegalTrademarks(void);

	LPCTSTR GetLegalCopyright(void);

	LPCTSTR GetCompanyName(void);

	LPCTSTR GetComments(void);

	BOOL Init(LPCTSTR pszFileName = NULL);
public:
	CVerInfo();
	~CVerInfo();

private:
	LPBYTE m_pVerInfoBuf;
	_TCHAR m_szSubBlock[ 64 ];
	_TCHAR m_szFileName[ _MAX_PATH ];
};

#endif // !defined(AFX_VERSIONINFO_H__B5A3B61F_C8B5_4CF3_A35D_E7A3BD56A300__INCLUDED_)
