// VersionInfo.cpp: implementation of the CVerInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VerInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma comment(lib, "Version.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVerInfo::CVerInfo()
{
	m_pVerInfoBuf = NULL;
	memset(m_szSubBlock, 0, sizeof(m_szSubBlock) );
	::GetModuleFileName(NULL, m_szFileName, sizeof(m_szFileName));
}

CVerInfo::~CVerInfo()
{
	delete []m_pVerInfoBuf;
}

BOOL CVerInfo::Init(LPCTSTR pszFileName /* = NULL */)
{
	if( pszFileName != NULL )
		_tcscpy(m_szFileName, pszFileName);

	DWORD dwHandle = 0;
	DWORD dwVerSize = GetFileVersionInfoSize(m_szFileName, &dwHandle);
	if( dwVerSize == 0 )
	{
		TRACE0("GetFileVersionInfoSize fail!\r\n");
		return FALSE;
	}

	if( m_pVerInfoBuf != NULL )
		delete []m_pVerInfoBuf;

	m_pVerInfoBuf = new BYTE[dwVerSize];
	if( NULL == m_pVerInfoBuf )
		return FALSE;

	if( GetFileVersionInfo(m_szFileName, dwHandle, dwVerSize, m_pVerInfoBuf) == 0 )
	{
		TRACE0("GetFileVersionInfo Fail!\r\n");
		return FALSE;
	}
	
	struct LANGANDCODEPAGE {
		WORD wLanguage;
		WORD wCodePage;
	} * lpTranslate;
	
	lpTranslate = NULL;
	UINT uTranslate = 0;

	int nQueryed = 0;
	nQueryed = VerQueryValue( m_pVerInfoBuf, 
		                      _T("\\VarFileInfo\\Translation"),
		                      (LPVOID*)&lpTranslate,
		                      &uTranslate );
	_ASSERTE( nQueryed > 0 && uTranslate == sizeof(LANGANDCODEPAGE) );

	_stprintf( m_szSubBlock, _T("\\StringFileInfo\\%04x%04x\\"), 
		       lpTranslate->wLanguage,
			   lpTranslate->wCodePage );

	return TRUE;
}

LPCTSTR CVerInfo::GetComments()
{
	_ASSERTE( m_pVerInfoBuf != NULL );

	static const _TCHAR szComments[] = _T("Comments");
	  
	_TCHAR szSubBlock[ 128 ] = { 0 };
	_stprintf(szSubBlock, _T("%s%s"), m_szSubBlock, szComments);

	LPTSTR lpComments = NULL;
	UINT   uLen       = 0;
	VerQueryValue( m_pVerInfoBuf,
				   szSubBlock,
				   (LPVOID *)&lpComments,
				   &uLen );

	return lpComments;
}

LPCTSTR CVerInfo::GetCompanyName()
{
	_ASSERTE( m_pVerInfoBuf != NULL );

	static const _TCHAR szCompanyName[] = _T("CompanyName");
	  
	_TCHAR szSubBlock[ 128 ] = { 0 };
	_stprintf(szSubBlock, _T("%s%s"), m_szSubBlock, szCompanyName);

	LPTSTR lpszCompanyName = NULL;
	UINT   uLen            = 0;
	VerQueryValue( m_pVerInfoBuf,
				   szSubBlock,
				   (LPVOID *)&lpszCompanyName,
				   &uLen );

	return lpszCompanyName;
}

LPCTSTR CVerInfo::GetLegalCopyright()
{
	_ASSERTE( m_pVerInfoBuf != NULL );

	static const _TCHAR szLegalCopyright[] = _T("LegalCopyright");
	  
	_TCHAR szSubBlock[ 128 ] = { 0 };
	_stprintf(szSubBlock, _T("%s%s"), m_szSubBlock, szLegalCopyright);

	LPTSTR lpszLegalCopyright = NULL;
	UINT   uLen            = 0;
	VerQueryValue( m_pVerInfoBuf,
				   szSubBlock,
				   (LPVOID *)&lpszLegalCopyright,
				   &uLen );

	return lpszLegalCopyright;
}

LPCTSTR CVerInfo::GetLegalTrademarks()
{
	_ASSERTE( m_pVerInfoBuf != NULL );

	static const _TCHAR szLegalTrademarks[] = _T("LegalTrademarks");
	  
	_TCHAR szSubBlock[ 128 ] = { 0 };
	_stprintf(szSubBlock, _T("%s%s"), m_szSubBlock, szLegalTrademarks);

	LPTSTR lpszLegalTrademarks = NULL;
	UINT   uLen            = 0;
	VerQueryValue( m_pVerInfoBuf,
				   szSubBlock,
				   (LPVOID *)&lpszLegalTrademarks,
				   &uLen );

	return lpszLegalTrademarks;
}

LPCTSTR CVerInfo::GetProductName()
{
	_ASSERTE( m_pVerInfoBuf != NULL );

	static const _TCHAR szProductName[] = _T("ProductName");
	  
	_TCHAR szSubBlock[ 128 ] = { 0 };
	_stprintf(szSubBlock, _T("%s%s"), m_szSubBlock, szProductName);

	LPTSTR lpszProductName = NULL;
	UINT   uLen            = 0;
	VerQueryValue( m_pVerInfoBuf,
				   szSubBlock,
				   (LPVOID *)&lpszProductName,
				   &uLen );

	return lpszProductName;
}

LPCTSTR CVerInfo::GetProductVersion()
{
	_ASSERTE( m_pVerInfoBuf != NULL );

	static const _TCHAR szProductVersion[] = _T("ProductVersion");
	  
	_TCHAR szSubBlock[ 128 ] = { 0 };
	_stprintf(szSubBlock, _T("%s%s"), m_szSubBlock, szProductVersion);

	LPTSTR lpszProductVersion = NULL;
	UINT   uLen            = 0;
	VerQueryValue( m_pVerInfoBuf,
				   szSubBlock,
				   (LPVOID *)&lpszProductVersion,
				   &uLen );

	return lpszProductVersion;
}

LPCTSTR CVerInfo::GetPrivateBuild()
{
	_ASSERTE( m_pVerInfoBuf != NULL );

	static const _TCHAR szPrivateBuild[] = _T("PrivateBuild");
	  
	_TCHAR szSubBlock[ 128 ] = { 0 };
	_stprintf(szSubBlock, _T("%s%s"), m_szSubBlock, szPrivateBuild);

	LPTSTR lpszPrivateBuild = NULL;
	UINT   uLen            = 0;
	VerQueryValue( m_pVerInfoBuf,
				   szSubBlock,
				   (LPVOID *)&lpszPrivateBuild,
				   &uLen );

	return lpszPrivateBuild;
}

LPCTSTR CVerInfo::GetSpecialBuild()
{
	_ASSERTE( m_pVerInfoBuf != NULL );

	static const _TCHAR szSpecialBuild[] = _T("SpecialBuild");
	  
	_TCHAR szSubBlock[ 128 ] = { 0 };
	_stprintf(szSubBlock, _T("%s%s"), m_szSubBlock, szSpecialBuild);

	LPTSTR lpszSpecialBuild = NULL;
	UINT   uLen            = 0;
	VerQueryValue( m_pVerInfoBuf,
				   szSubBlock,
				   (LPVOID *)&lpszSpecialBuild,
				   &uLen );

	return lpszSpecialBuild;
}
