// Ownership.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Ownership.h"

#include "MainFrm.h"
#include "OwnershipDoc.h"
#include "OwnershipListView.h"
#include "XAboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COwnershipApp

BEGIN_MESSAGE_MAP(COwnershipApp, CWinApp)
	//{{AFX_MSG_MAP(COwnershipApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COwnershipApp construction

COwnershipApp::COwnershipApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bCityToTxt = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only COwnershipApp object

COwnershipApp g_theApp;

/////////////////////////////////////////////////////////////////////////////
// COwnershipApp initialization

BOOL COwnershipApp::InitInstance()
{
	UNUSED_ALWAYS(IDR_CITYMATYPE); // only for PC lint 

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

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	if(!LoadConfig())
	{
		AfxMessageBox(_T("Load configure file failed!"));
		return FALSE;
	}

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(COwnershipDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(COwnershipListView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	//cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void COwnershipApp::OnAppAbout()
{
	CXAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// COwnershipApp message handlers
BOOL COwnershipApp::LoadConfig()
{
	TCHAR szConfigFile[MAX_PATH] = {0};
	::GetModuleFileName(g_theApp.m_hInstance,szConfigFile,MAX_PATH);
	TCHAR * pChar = _tcsrchr(szConfigFile,_T('.'));
	*(pChar+1) = _T('\0');
	_tcscat(szConfigFile,_T("ini"));
	
	TCHAR szNumberSector[1024] = {0};
	
	GetPrivateProfileSection(_T("Number Sector"),szNumberSector,1024,szConfigFile);
	CStringArray agNumberSector;
	EnumKeys(szNumberSector,&agNumberSector);
	
	for(int i=0;i<agNumberSector.GetSize();i=i+2)
	{
		CString strSector = agNumberSector.GetAt(i+1);
		strSector.TrimLeft();
		strSector.TrimRight();
		
		int nFind = strSector.Find(_T(','));
		
		CString strSectorBegin = strSector.Left(nFind);
		CString strSectorEnd   = strSector.Right(strSector.GetLength() - nFind -1);
		strSectorBegin.TrimRight();
		strSectorEnd.TrimLeft();
		
		m_agNumberSector.Add(_ttoi(strSectorBegin));
		m_agNumberSector.Add(_ttoi(strSectorEnd));
	}	

	if(m_agNumberSector.GetSize()== 0)
		return FALSE;

	m_bCityToTxt = GetPrivateProfileInt(_T("Settings"),_T("CityToTxt"),0,szConfigFile);

	return TRUE;
}


BOOL COwnershipApp::RemoveFileReadOnlyAttr(LPCTSTR lpszFileName)
{
	if(lpszFileName == NULL)
		return FALSE;
	CFileFind finder;
	if(finder.FindFile(lpszFileName))
	{
		DWORD dwAttr = GetFileAttributes(lpszFileName);
		if(((DWORD)-1) != dwAttr)
		{
			dwAttr &= ~FILE_ATTRIBUTE_READONLY;
			::SetFileAttributes(lpszFileName,dwAttr);
		}
	}
	return TRUE;
}