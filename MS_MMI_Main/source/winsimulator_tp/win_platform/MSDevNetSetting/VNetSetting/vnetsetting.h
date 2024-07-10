// VNetSetting.h : main header file for the VNETSETTING DLL
//

#if !defined(AFX_VNETSETTING_H__11F0F72E_2C9A_48C0_AB39_6E2A6BABF5ED__INCLUDED_)
#define AFX_VNETSETTING_H__11F0F72E_2C9A_48C0_AB39_6E2A6BABF5ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

// global property page
#include "NetstateDlg.h"
extern CNetstateDlg *g_pPage;


/////////////////////////////////////////////////////////////////////////////
// CVNetSettingApp
// See VNetSetting.cpp for the implementation of this class
//

class CVNetSettingApp : public CWinApp
{
public:
	CVNetSettingApp();
    ~CVNetSettingApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVNetSettingApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CVNetSettingApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VNETSETTING_H__11F0F72E_2C9A_48C0_AB39_6E2A6BABF5ED__INCLUDED_)
