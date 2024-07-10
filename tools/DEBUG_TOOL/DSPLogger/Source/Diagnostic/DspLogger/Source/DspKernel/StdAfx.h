// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__E968F909_6D20_4A72_A3A4_71713A8787AC__INCLUDED_)
#define AFX_STDAFX_H__E968F909_6D20_4A72_A3A4_71713A8787AC__INCLUDED_
#pragma warning( disable : 4100 4511 4512 4663 4018 4245 )

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>

#pragma warning(push,3)
#include <string>
#pragma warning(pop)
#pragma warning( disable : 4245 4710)
// TODO: reference additional headers your program requires here
/*============================================================================*
 *                          DLL exporting symbols
 *============================================================================*/

#ifdef DSPKERNEL_EXPORTS
#define DSPKERNEL_API __declspec(dllexport)
#else
#define DSPKERNEL_API __declspec(dllimport)
#endif  // DSPKERNEL_EXPORTS

std::string		Format(const char *lpszFmt, ...);
std::string		GetLocalTime(void);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E968F909_6D20_4A72_A3A4_71713A8787AC__INCLUDED_)
