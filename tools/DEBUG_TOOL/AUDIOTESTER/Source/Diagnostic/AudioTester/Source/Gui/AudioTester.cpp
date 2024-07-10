// AudioTester.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AudioTester.h"
#include "AudioTesterSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAudioTesterApp

BEGIN_MESSAGE_MAP(CAudioTesterApp, CWinApp)
	//{{AFX_MSG_MAP(CAudioTesterApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAudioTesterApp construction

CAudioTesterApp::CAudioTesterApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAudioTesterApp object

CAudioTesterApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAudioTesterApp initialization

BOOL CAudioTesterApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

    LoadSettings();

    CAudioTesterSheet sheet( IDS_SHEET_TITLE );
	m_pMainWnd = &sheet;
    sheet.DoModal();
    
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL CAudioTesterApp::GetIniFilePath(char* pName)
{
    if(pName == NULL)
        return FALSE;

#ifdef _UNICODE
	TCHAR szDirectory[MAX_PATH] = {0};
    DWORD dwRet = ::GetModuleFileName(AfxGetApp()->m_hInstance,szDirectory,_MAX_PATH);
	WideCharToMultiByte(CP_ACP,0,szDirectory,-1,pName,_MAX_PATH,NULL,NULL);
#else
	DWORD dwRet = ::GetModuleFileName(AfxGetApp()->m_hInstance,pName,_MAX_PATH);
#endif

    if(dwRet == 0)
    {
        pName[0] = 0;
        return FALSE;
    }
    
    char* pResult = strrchr(pName,'.');
    if(pResult == NULL)
    {
        pName[0] = 0;
        return FALSE;
    }
    
    *pResult = 0;
    strcat(pName,".ini");
    
    return TRUE;    
}

void CAudioTesterApp::LoadSettings()
{
    char szFilePath[_MAX_PATH];
    if(!GetIniFilePath(szFilePath))
    {
        return;
    }

#ifdef SIMU_TEST
    // For test only
    extern int g_data_size;
    g_data_size = GetPrivateProfileInt( "Test","DataSize",176,szFilePath );

	extern int g_data_from;
	g_data_from = GetPrivateProfileInt( "Test","DataFrom",1,szFilePath );
#endif
}