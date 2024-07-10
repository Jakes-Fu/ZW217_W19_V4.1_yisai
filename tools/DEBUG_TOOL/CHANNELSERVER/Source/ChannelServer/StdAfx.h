// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__84E88EC5_D805_4841_93FC_1A396AF55870__INCLUDED_)
#define AFX_STDAFX_H__84E88EC5_D805_4841_93FC_1A396AF55870__INCLUDED_

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

#define ENDIAN_LITTLE    0
#define ENDIAN_BIG       1
#define ENDIAN_SELFADAPT 2

#define FLOW_TYPE_DIAG   0
#define FLOW_TYPE_SMP    1

LONG XX_MulDiv( LONG  a,
			   LONG  b,
			   LONG  c );
int SplitStr(LPCTSTR lpszStr,CStringArray &agStrs,TCHAR chSeparate = _T(',') );


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__84E88EC5_D805_4841_93FC_1A396AF55870__INCLUDED_)
#include <afxdlgs.h>
