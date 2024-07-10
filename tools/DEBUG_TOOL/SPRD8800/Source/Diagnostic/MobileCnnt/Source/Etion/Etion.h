// Etion.h : main header file for the ETION DLL
//

#if !defined(AFX_ETION_H__589F01FA_5A43_44EC_BCCE_CF9E4C125CE5__INCLUDED_)
#define AFX_ETION_H__589F01FA_5A43_44EC_BCCE_CF9E4C125CE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CEtionApp
// See Etion.cpp for the implementation of this class
//

class CEtionApp : public CWinApp
{
public:
	CEtionApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEtionApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CEtionApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETION_H__589F01FA_5A43_44EC_BCCE_CF9E4C125CE5__INCLUDED_)
