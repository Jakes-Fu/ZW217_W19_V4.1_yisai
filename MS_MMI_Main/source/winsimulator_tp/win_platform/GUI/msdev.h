// MSDev.h : main header file for the MSDEV application
//

#if !defined(AFX_MSDEV_H__95275134_3BEA_4731_B3B0_63B904342AEA__INCLUDED_)
#define AFX_MSDEV_H__95275134_3BEA_4731_B3B0_63B904342AEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMSDevApp:
// See MSDev.cpp for the implementation of this class
//

class CMSDevApp : public CWinApp
{
public:
	CMSDevApp();
	ULONG_PTR m_pGdiToken; 
	BOOL      m_bAutoStart;
	UINT      m_nPort; // socket port

public:
	void GetRelativePath(CString &strBmpPath, CString &strIniPath, CString &strBinPath);

    void GetImageFilesPath(CString &strBmpPath, CString &strIniPath, CString &strBinPath);
    void SetImageFilesPath(CString strBmpPath,  CString strIniPath,  CString strBinPath);
	    
    void GetWindowPos(CRect& rect);
    void SaveWindowPos(CRect rect);
	
	void SaveSocketPort(DWORD dwPort);
	
	void SaveTraceState(BOOL bOn);
	BOOL GetTraceState();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMSDevApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMSDevApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Functions
public:
    CString GetCurrentPathName();

};

extern CMSDevApp g_theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSDEV_H__95275134_3BEA_4731_B3B0_63B904342AEA__INCLUDED_)
