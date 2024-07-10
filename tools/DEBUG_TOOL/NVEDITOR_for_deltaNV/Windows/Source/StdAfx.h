// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__75462894_3A79_436D_BD4E_6028BDC23EE5__INCLUDED_)
#define AFX_STDAFX_H__75462894_3A79_436D_BD4E_6028BDC23EE5__INCLUDED_

#include <afxwin.h>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable: 4786 4663 4996 4121 4100 4245 4018)

#define SAFE_DELETE_ARRAY(p)  \
	do \
{\
    if (p != NULL) {\
	delete []p; \
	   p=NULL;\
	}\
} while(0) 

// TODO: reference additional headers your program requires here
DWORD GetFileSize( LPCTSTR lpFileName);
BOOL LoadFileFromLocal(LPCTSTR pszFileName, LPBYTE &pBuf,DWORD &dwSize);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__75462894_3A79_436D_BD4E_6028BDC23EE5__INCLUDED_)
