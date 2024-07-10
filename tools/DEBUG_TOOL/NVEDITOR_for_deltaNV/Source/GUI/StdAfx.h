// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__7C12A841_DBCF_43DA_B91C_D869519D173A__INCLUDED_)
#define AFX_STDAFX_H__7C12A841_DBCF_43DA_B91C_D869519D173A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#pragma warning( disable: 4786 4100 4089 4244)

typedef enum FILE_TYPE_T 
{ 
    E_DOT_H_FILE	= 0,
	E_DOT_BIN_FILE	= 1,
	E_DOT_PRJ_FILE,
	E_DOT_XPRJ_FILE,
	E_DOT_NV_FILE,
	E_DOT_TXT_FILE,
	E_UNDEF_TYPE
};

//#include "CJLibrary.h"		// CJLibrary components.
int EnumKeys(TCHAR* pSection,CStringArray* pKeys);
int SplitStr(LPCTSTR lpszStr,CStringArray &agStrs,TCHAR chSeparate = _T(','));
BOOL IsExistFile(LPCTSTR lpFile);
DWORD GetFileSize( LPCTSTR lpFileName);
BOOL IsSpecifiedFile(TCHAR* lpFile,int nFileType);
DWORD GetDigit(LPCTSTR lpszText);
CString GetStrFilePath(LPCTSTR lpFile,	BOOL bMakeUpper = FALSE,	BOOL bPostfix = FALSE);
CString GetStrFileTitle(LPCTSTR lpFile, BOOL bMakeUpper = FALSE,	BOOL bPostfix = FALSE);
BOOL LoadFileFromLocal(LPCTSTR pszFileName, LPBYTE &pBuf,DWORD &dwSize);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#define SET_FIND_FLAG_ITEM(d,v)			((d) = (v) ? ((d)|0x1) : ((d)&0xFFFFFFFE))
#define SET_FIND_FLAG_VALUE(d,v)		((d) = (v) ? ((d)|0x2) : ((d)&0xFFFFFFFD))
#define SET_FIND_FLAG_FULLMATCH(d,v)	((d) = (v) ? ((d)|0x4) : ((d)&0xFFFFFFFB))
#define SET_FIND_FLAG_CHANGED(d,v)		((d) = (v) ? ((d)|0x8) : ((d)&0xFFFFFFF7))

#define GET_FIND_FLAG_ITEM(d)			((d)&0x1)
#define GET_FIND_FLAG_VALUE(d)			((d)&0x2)
#define GET_FIND_FLAG_FULLMATCH(d)		((d)&0x4)
#define GET_FIND_FLAG_CHANGED(d)		((d)&0x8)


#endif // !defined(AFX_STDAFX_H__7C12A841_DBCF_43DA_B91C_D869519D173A__INCLUDED_)
