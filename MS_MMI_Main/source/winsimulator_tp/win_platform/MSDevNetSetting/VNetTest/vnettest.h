// VNetTest.h : main header file for the VNETTEST application
//

#if !defined(AFX_VNETTEST_H__EE9186BE_794B_429C_B577_4010AA09F885__INCLUDED_)
#define AFX_VNETTEST_H__EE9186BE_794B_429C_B577_4010AA09F885__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CVNetTestApp:
// See VNetTest.cpp for the implementation of this class
//

class CVNetTestApp : public CWinApp
{
public:
	CVNetTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVNetTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVNetTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VNETTEST_H__EE9186BE_794B_429C_B577_4010AA09F885__INCLUDED_)
