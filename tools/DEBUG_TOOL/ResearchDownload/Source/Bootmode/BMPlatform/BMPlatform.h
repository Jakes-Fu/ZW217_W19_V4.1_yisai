// BMPlatform.h : main header file for the BMPLATFORM DLL
//

#if !defined(AFX_BMPLATFORM_H__5EB0C285_40C8_436C_A7B1_8E1B166175E4__INCLUDED_)
#define AFX_BMPLATFORM_H__5EB0C285_40C8_436C_A7B1_8E1B166175E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "ICommChannel.h"

typedef BOOL (*pfCreateChannel)( ICommChannel ** , CHANNEL_TYPE);
typedef void (*pfReleaseChannel)( ICommChannel *);

/////////////////////////////////////////////////////////////////////////////
// CBMPlatformApp
// See BMPlatform.cpp for the implementation of this class
//

class CBMPlatformApp : public CWinApp
{
public:
	CBMPlatformApp();

	pfCreateChannel m_pfCreateChannel;
	pfReleaseChannel m_pfReleaseChannel;

	HMODULE  m_hChannelLib;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBMPlatformApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CBMPlatformApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual BOOL InitInstance();
	virtual int ExitInstance();
};

extern CBMPlatformApp g_theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMPLATFORM_H__5EB0C285_40C8_436C_A7B1_8E1B166175E4__INCLUDED_)

