// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__4A19563E_1F7E_4A87_8BF5_422A493ECABD__INCLUDED_)
#define AFX_STDAFX_H__4A19563E_1F7E_4A87_8BF5_422A493ECABD__INCLUDED_

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

#ifndef ULONG_PTR
#define ULONG_PTR unsigned long* 
#endif 
//#pragma comment(lib, "gdiplus.lib")
#include "gdiplus.h"
using namespace Gdiplus;

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED			0x00080000
#endif

#ifndef WS_EX_NOACTIVATE
#define WS_EX_NOACTIVATE		0x08000000L
#endif

#ifndef LWA_ALPHA
#define LWA_ALPHA				0x00000002
#endif

#ifndef ULW_ALPHA
#define ULW_ALPHA				0x00000002
#endif

#ifndef AC_SRC_ALPHA
#define AC_SRC_ALPHA			0x01
#endif


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__4A19563E_1F7E_4A87_8BF5_422A493ECABD__INCLUDED_)
