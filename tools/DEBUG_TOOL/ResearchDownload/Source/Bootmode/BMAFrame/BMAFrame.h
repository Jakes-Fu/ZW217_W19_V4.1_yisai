// BMAFrame.h : main header file for the BMAFRAME DLL
//

#if !defined(AFX_BMAFRAME_H__6EFD7319_CCD8_4E7B_91B5_5AF4887CFD2D__INCLUDED_)
#define AFX_BMAFRAME_H__6EFD7319_CCD8_4E7B_91B5_5AF4887CFD2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

class IBootModeHandler;

typedef BOOL (*pfCreateBMObject)( IBootModeHandler **);
typedef void (*pfReleaseBMObject)( IBootModeHandler *);

/////////////////////////////////////////////////////////////////////////////
// CBMAFrameApp
// See BMAFrame.cpp for the implementation of this class
//

class CBMAFrameApp : public CWinApp
{
public:
	CBMAFrameApp();

	pfCreateBMObject m_pfCreateBMObject;
	pfReleaseBMObject m_pfReleaseBMObject;

	HMODULE  m_hBMPlatfromLib;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBMAFrameApp)
	public:
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CBMAFrameApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual BOOL InitInstance();
	virtual int ExitInstance();
};

extern CBMAFrameApp g_theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMAFRAME_H__6EFD7319_CCD8_4E7B_91B5_5AF4887CFD2D__INCLUDED_)
