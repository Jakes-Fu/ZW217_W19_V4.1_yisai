// SecBinPac.h : main header file for the SECBINPAC DLL
//

#if !defined(AFX_SECBINPAC_H__4D696700_DF7D_484D_B657_3F38FEA43637__INCLUDED_)
#define AFX_SECBINPAC_H__4D696700_DF7D_484D_B657_3F38FEA43637__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSecBinPacApp
// See SecBinPac.cpp for the implementation of this class
//

class CSecBinPacApp : public CWinApp
{
public:
	CSecBinPacApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSecBinPacApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSecBinPacApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECBINPAC_H__4D696700_DF7D_484D_B657_3F38FEA43637__INCLUDED_)
