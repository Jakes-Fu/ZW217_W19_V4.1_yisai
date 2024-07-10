// SCIMMIPB.h : main header file for the SCIMMIPB DLL
//

#if !defined(AFX_SCIMMIPB_H__CD047EA4_28C2_4C29_9131_B55EFCBB889D__INCLUDED_)
#define AFX_SCIMMIPB_H__CD047EA4_28C2_4C29_9131_B55EFCBB889D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSCIMMIPBApp
// See SCIMMIPB.cpp for the implementation of this class
//

class CSCIMMIPBApp : public CWinApp
{
public:
	CSCIMMIPBApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCIMMIPBApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSCIMMIPBApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCIMMIPB_H__CD047EA4_28C2_4C29_9131_B55EFCBB889D__INCLUDED_)
