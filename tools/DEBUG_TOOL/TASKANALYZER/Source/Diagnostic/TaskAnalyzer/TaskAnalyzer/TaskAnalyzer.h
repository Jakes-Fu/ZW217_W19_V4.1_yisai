// TaskAnalyzer.h : main header file for the TASKANALYZER application
//

#if !defined(AFX_TASKANALYZER_H__FD17A3A7_FF67_4613_870E_3F5264E1C699__INCLUDED_)
#define AFX_TASKANALYZER_H__FD17A3A7_FF67_4613_870E_3F5264E1C699__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

//#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTaskAnalyzerApp:
// See TaskAnalyzer.cpp for the implementation of this class
//

class CTaskAnalyzerApp : public CWinApp
{
public:
	CTaskAnalyzerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskAnalyzerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

public:
	CString m_strVersion;
    CString m_strBuild;
    void GetVersion();
// Implementation

	//{{AFX_MSG(CTaskAnalyzerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKANALYZER_H__FD17A3A7_FF67_4613_870E_3F5264E1C699__INCLUDED_)
