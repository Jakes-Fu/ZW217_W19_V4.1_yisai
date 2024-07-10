// SCIShortMsg.h : main header file for the SCISHORTMSG DLL
//

#if !defined(AFX_SCISHORTMSG_H__27416329_ED62_44C7_B16B_D13B2093A6A3__INCLUDED_)
#define AFX_SCISHORTMSG_H__27416329_ED62_44C7_B16B_D13B2093A6A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSCIShortMsgApp
// See SCIShortMsg.cpp for the implementation of this class
//

class CSCIShortMsgApp : public CWinApp
{
public:
	CSCIShortMsgApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCIShortMsgApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSCIShortMsgApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCISHORTMSG_H__27416329_ED62_44C7_B16B_D13B2093A6A3__INCLUDED_)
