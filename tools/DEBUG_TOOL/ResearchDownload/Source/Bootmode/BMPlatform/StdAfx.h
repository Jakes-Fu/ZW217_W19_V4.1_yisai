// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__B959FB05_3779_4A9D_982F_5FD6466DEF12__INCLUDED_)
#define AFX_STDAFX_H__B959FB05_3779_4A9D_982F_5FD6466DEF12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable : 4996 4121)
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

typedef unsigned (WINAPI *PBEGINTHREADEX_THREADFUNC )( LPVOID lpThreadParamter);
typedef unsigned *PBEGINTHREADEX_THREADID;
void GetModuleFilePath( HMODULE hModule, LPTSTR szAppPath );
BOOL CreateDeepDirectory(LPCTSTR lpszDir);

#define SAFE_FREE(p) {\
if(NULL != p) \
{\
	delete p;\
	p=NULL;\
}\
}

#define SAFE_FREE_ARRAY(p) {\
if(NULL != p) \
{\
	delete []p;\
	p=NULL;\
}\
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__B959FB05_3779_4A9D_982F_5FD6466DEF12__INCLUDED_)
