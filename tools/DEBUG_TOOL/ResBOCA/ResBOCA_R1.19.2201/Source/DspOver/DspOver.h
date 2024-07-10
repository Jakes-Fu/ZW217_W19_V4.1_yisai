// DspOver.h : main header file for the DSPOVER application
//

#if !defined(AFX_DSPOVER_H__33BD8A0D_792E_40E5_89E1_7476AF9C04C0__INCLUDED_)
#define AFX_DSPOVER_H__33BD8A0D_792E_40E5_89E1_7476AF9C04C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDspOverApp:
// See DspOver.cpp for the implementation of this class
//

class CDspOverApp : public CWinApp
{
public:
	CDspOverApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDspOverApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDspOverApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSPOVER_H__33BD8A0D_792E_40E5_89E1_7476AF9C04C0__INCLUDED_)
