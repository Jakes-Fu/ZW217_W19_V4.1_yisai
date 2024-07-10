// SCIMemo.h : main header file for the SCIMEMO DLL
//

#if !defined(AFX_SCIMEMO_H__000985AE_E57D_486A_B686_4273B0EDA984__INCLUDED_)
#define AFX_SCIMEMO_H__000985AE_E57D_486A_B686_4273B0EDA984__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSCIMemoApp
// See SCIMemo.cpp for the implementation of this class
//

class CSCIMemoApp : public CWinApp
{
public:
	CSCIMemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCIMemoApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSCIMemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCIMEMO_H__000985AE_E57D_486A_B686_4273B0EDA984__INCLUDED_)
