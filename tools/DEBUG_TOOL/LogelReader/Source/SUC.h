// SUC.h : main header file for the SUC application
//

#if !defined(AFX_SUC_H__22A0B72E_178B_4CEC_8C61_C98E11D7D5FC__INCLUDED_)
#define AFX_SUC_H__22A0B72E_178B_4CEC_8C61_C98E11D7D5FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
/////////////////////////////////////////////////////////////////////////////
// CSUCApp:
// See SUC.cpp for the implementation of this class
//

class CSUCApp : public CWinApp
{
public:
	CSUCApp();

public:
	CString m_strVersion;
    CString m_strBuild;
	void GetVersion();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSUCApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL


// Implementation

	//{{AFX_MSG(CSUCApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUC_H__22A0B72E_178B_4CEC_8C61_C98E11D7D5FC__INCLUDED_)
