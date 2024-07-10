// ChannelServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ChannelServer.h"
#include "MainFrm.h"
#include "ChannelServerDoc.h"
#include "ChannelServerView.h"

#include "XAboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChannelServerApp

BEGIN_MESSAGE_MAP(CChannelServerApp, CWinApp)
	//{{AFX_MSG_MAP(CChannelServerApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChannelServerApp construction

CChannelServerApp::CChannelServerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CChannelServerApp object
//lint -esym(765,theApp)
CChannelServerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CChannelServerApp initialization

BOOL CChannelServerApp::InitInstance()
{
	// Start instance detecting
	// [
	// if it is true that  an instance is running 
	// then disable create a new instance, otherwise create a 
	// new instance
	/*
	m_hInstInd = CreateEvent(NULL, TRUE, FALSE, "Global\\ChannelServer");
    if(NULL != m_hInstInd)
	{
		DWORD dwError = GetLastError();
		if(ERROR_ALREADY_EXISTS == dwError)
		{
			AfxMessageBox("Already has an ChannelServer is running", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}
	}
	*/
	// ]
	//End instance Detecting 

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
#if _MSC_VER < 1300
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CChannelServerDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CChannelServerView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

// App command to run the dialog
void CChannelServerApp::OnAppAbout()
{
	CXAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CChannelServerApp message handlers

int CChannelServerApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	/*
	if(NULL != m_hInstInd)
	{
		CloseHandle(m_hInstInd);
	}
	*/

	return CWinApp::ExitInstance();
}
