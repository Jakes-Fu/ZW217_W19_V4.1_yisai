// CODEC_UT.h : main header file for the CODEC_UT application
//

#if !defined(AFX_CODEC_UT_H__74C9E29B_7B80_40A1_8312_FD727E720007__INCLUDED_)
#define AFX_CODEC_UT_H__74C9E29B_7B80_40A1_8312_FD727E720007__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCODEC_UTApp:
// See CODEC_UT.cpp for the implementation of this class
//

class CCODEC_UTApp : public CWinApp
{
public:
	CCODEC_UTApp();

	void RunTest();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCODEC_UTApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCODEC_UTApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CODEC_UT_H__74C9E29B_7B80_40A1_8312_FD727E720007__INCLUDED_)
