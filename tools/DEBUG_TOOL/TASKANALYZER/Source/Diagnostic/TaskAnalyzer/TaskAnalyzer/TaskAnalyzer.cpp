// TaskAnalyzer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TaskAnalyzer.h"
#include "TaskAnalyzerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaskAnalyzerApp

BEGIN_MESSAGE_MAP(CTaskAnalyzerApp, CWinApp)
	//{{AFX_MSG_MAP(CTaskAnalyzerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskAnalyzerApp construction

CTaskAnalyzerApp::CTaskAnalyzerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTaskAnalyzerApp object

CTaskAnalyzerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTaskAnalyzerApp initialization

BOOL CTaskAnalyzerApp::InitInstance()
{
	AfxEnableControlContainer();
	GetVersion();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CTaskAnalyzerDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
void CTaskAnalyzerApp::GetVersion()
{
    char szFileName[_MAX_PATH];
    ::GetModuleFileName(AfxGetApp()->m_hInstance,szFileName,_MAX_PATH);
    
    DWORD dwSize,dwHandle;
    dwSize = ::GetFileVersionInfoSize(szFileName,&dwHandle);	
    if( dwSize == 0 )
    {
        return;
    }
    
    char* pVer = new char[dwSize];//lint !e119
    BOOL bRet = ::GetFileVersionInfo(szFileName,dwHandle,dwSize,pVer);
    if(!bRet)
    {
        delete [] pVer;
        return;
    }
    
    struct LANGANDCODEPAGE {
        WORD wLanguage;
        WORD wCodePage;
    } *lpTranslate;
    
    
    UINT cbTranslate;
    
    // Read the list of languages and code pages.    
    VerQueryValue(pVer, 
        TEXT("\\VarFileInfo\\Translation"),
        (LPVOID*)&lpTranslate,
        &cbTranslate);
    
    TCHAR SubBlock[_MAX_PATH];
    void* lpBuffer;
    
    // Read the file version
    wsprintf( SubBlock, 
        TEXT("\\StringFileInfo\\%04x%04x\\FileVersion"),
        lpTranslate[0].wLanguage,
        lpTranslate[0].wCodePage);
    
    // Retrieve file description for language and code page "i". 
    bRet = VerQueryValue(pVer, 
        SubBlock, 
        &lpBuffer, 
        &cbTranslate); 
    if( bRet )
    {
        m_strVersion = (char*)lpBuffer;
        m_strVersion.Replace( ",","." );
        m_strVersion.Replace( " ","" );
    }
	//@teana hu 2007-03-30
	//[[
	m_strVersion = m_strVersion.Left(5);
	//]]
    
    // Read the build information
    wsprintf( SubBlock, 
        TEXT("\\StringFileInfo\\%04x%04x\\PrivateBuild"),
        lpTranslate[0].wLanguage,
        lpTranslate[0].wCodePage);
    
    // Retrieve file description for language and code page "i". 
    bRet = VerQueryValue(pVer, 
        SubBlock, 
        &lpBuffer, 
        &cbTranslate); 
    if( bRet )
    {
        m_strBuild = (char*)lpBuffer;
        m_strBuild.Replace( ",","." );
    }
    
    delete [] pVer;
}
