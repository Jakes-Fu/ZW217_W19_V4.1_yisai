// ChannelServer.h : main header file for the CHANNELSERVER application
//

#if !defined(AFX_CHANNELSERVER_H__6B5C30FD_2FE5_46B5_98AA_331352E4AE3A__INCLUDED_)
#define AFX_CHANNELSERVER_H__6B5C30FD_2FE5_46B5_98AA_331352E4AE3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CChannelServerApp:
// See ChannelServer.cpp for the implementation of this class
//

class CChannelServerApp : public CWinApp
{
public:
	CChannelServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChannelServerApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CChannelServerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
//	HANDLE m_hInstInd;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANNELSERVER_H__6B5C30FD_2FE5_46B5_98AA_331352E4AE3A__INCLUDED_)
