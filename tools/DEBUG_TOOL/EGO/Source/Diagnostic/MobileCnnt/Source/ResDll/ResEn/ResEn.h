// ResEn.h : main header file for the RESEN DLL
//

#if !defined(AFX_RESEN_H__751C83FA_7EA7_4E7F_B616_58D8E05239FB__INCLUDED_)
#define AFX_RESEN_H__751C83FA_7EA7_4E7F_B616_58D8E05239FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CResEnApp
// See ResEn.cpp for the implementation of this class
//

class CResEnApp : public CWinApp
{
public:
	CResEnApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResEnApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CResEnApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESEN_H__751C83FA_7EA7_4E7F_B616_58D8E05239FB__INCLUDED_)
