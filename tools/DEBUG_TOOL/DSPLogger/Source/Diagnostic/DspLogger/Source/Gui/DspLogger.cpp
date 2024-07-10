// DspLogger.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ProcessMgr.h"
#include "DspLogger.h"

#include "MainFrm.h"
#include "BasicChildFrm.h"

#include "DspLoggerDoc.h"
#include "FindDoc.h"

#include "TestPointsView.h"
#include "FindMainFrame.h"
#include "FindListView.h"

#include "FilterMainFrame.h"
#include "FilterChartView.h"
#include "FilterDoc.h"

#include "KeyInfoFrame.h"
#include "KeyInfoDoc.h"
#include "KeyinfoView.h"
#include "XAboutDlg.h"

#include "LUA_FUNC.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDspLoggerApp

BEGIN_MESSAGE_MAP(CDspLoggerApp, CWinApp)
	//{{AFX_MSG_MAP(CDspLoggerApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDspLoggerApp construction

CDspLoggerApp::CDspLoggerApp()
{
    m_hRssiIcon    = NULL;
    m_pTestPointDocTemplate = NULL;
    m_pFindTemplate         = NULL;
    m_pFilterTemplate       = NULL;
    m_pTestPointFrame       = NULL;
	m_pKeyinfoTemplate		= NULL;
	m_pGraphInfo			= NULL;

	//init chart wnd point	
	for (int i=0;i<MAX_CHART_WND_NUM; i++)
	{
		m_pFindWnd[i]=NULL;
	}
   
    m_AppFont.CreateFont( 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Courier" );
	m_bCreateKeyInfoFrame	= FALSE;
	m_bSupportMultiChart	= FALSE;

	m_pKeyInfoDoc			= NULL;
	m_bCmdMode				= FALSE;
    //lint !e1401
}

CDspLoggerApp::~CDspLoggerApp()
{
    m_AppFont.DeleteObject();
	m_pKeyInfoDoc = NULL;
	m_cXmlParse.Release(m_pGraphInfo);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDspLoggerApp object

CDspLoggerApp g_theApp;

/////////////////////////////////////////////////////////////////////////////
// CDspLoggerApp initialization

CFont * CDspLoggerApp::GetAppFont()
{
    return &m_AppFont;
}

BOOL CDspLoggerApp::ParseCMDParam(int argc, TCHAR** argv)
{
    BOOL bRet = FALSE;
    if (argc>1)
    {
        for(int i=1; i<argc; ++i)
        {
            if( 0 == _tcsicmp(argv[i],_T("-tp")) )   
            {
                if((i+1)<argc)
                {
                    m_strTpFile = argv[++i];
                }
            }
            else if( 0 == _tcsicmp(argv[i],_T("-log")) )   
            {
                if((i+1)<argc)
                {
                     m_strLogFile = argv[++i];
                }
            }
			
        }
        bRet = TRUE;
		CFileFind finder;
		
		if ( m_strTpFile.IsEmpty() || m_strLogFile.IsEmpty())
		{
			_tprintf(_T("Invalid arguments\n"));
			bRet = FALSE;		
		}
		if(m_strTpFile.IsEmpty() && !finder.FindFile(m_strTpFile))
		{
			bRet = FALSE;
		}
    }
	
    return bRet;
}

BOOL CDspLoggerApp::InitInstance()
{
	TCHAR szModuleFile[MAX_PATH]= {0};
	TCHAR szCfgFile[MAX_PATH]	= {0};
	TCHAR szGraphXml[MAX_PATH] = {0};
	
	BOOL bSupportMultiApp		= FALSE;
	ProcessMgr	cProMgr;
	GetModuleFileName(NULL,szModuleFile,_MAX_PATH);	

	_tcscpy(szCfgFile,szModuleFile);
  
	TCHAR* p=_tcsrchr(szCfgFile,_T('\\'));
    if(p != NULL)
    {
        *p = _T('\0');
    } 

	_tcscpy(szGraphXml,szCfgFile);
	_tcscat(szCfgFile,_T("\\DspLogger.ini"));
	_tcscat(szGraphXml,_T("\\CustomizeGraph.xml"));
	bSupportMultiApp = GetPrivateProfileInt( _T("Settings"),_T("SupportMultiApp"),FALSE, szCfgFile );
	m_bSupportMultiChart = GetPrivateProfileInt( _T("Settings"),_T("SupportMultiChart"),FALSE, szCfgFile );

	m_cXmlParse.LoadXml(szGraphXml,m_pGraphInfo);


	if ( __argc > 1 )
    {
        m_bCmdMode = ParseCMDParam(__argc,__targv);
    }

	if(!m_bCmdMode && !bSupportMultiApp && !cProMgr.IsSingleIntance(szModuleFile))
	{
		AfxMessageBox(_T("DspLogger application don't support multiple instances,\nThe same directory of applications just allowed to run only one instance!"));
		return FALSE;
	}
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif


    GetVersion();
    
	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Spreadtrum"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	// serve as the connection between documents, frame windows and views.
	m_pTestPointDocTemplate = new CMultiDocTemplate(
		IDR_DSPLOGTYPE,
		RUNTIME_CLASS(CDspLoggerDoc),
		RUNTIME_CLASS(CBasicChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTestPointsView)
    );
	AddDocTemplate( m_pTestPointDocTemplate);

    m_pFindTemplate = new CMultiDocTemplate(
        IDR_FIND,
        RUNTIME_CLASS(CFindDoc),
		RUNTIME_CLASS(CFindMainFrame), // custom MDI child frame
		RUNTIME_CLASS(CFindListView)
    );
    AddDocTemplate(m_pFindTemplate);

    m_pFilterTemplate = new CMultiDocTemplate(
        IDR_FILTER,
        RUNTIME_CLASS(CFilterDoc),
        RUNTIME_CLASS(CFilterMainFrame),
        RUNTIME_CLASS(CFilterChartView)
        );
    AddDocTemplate(m_pFilterTemplate);

	m_pKeyinfoTemplate = new CMultiDocTemplate(
        IDR_KEYINFO,
        RUNTIME_CLASS(CKeyInfoDoc),
        RUNTIME_CLASS(CKeyInfoFrame),
        RUNTIME_CLASS(CKeyinfoView)
        );
    AddDocTemplate(m_pKeyinfoTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (NULL ==pMainFrame  ||
		!pMainFrame->LoadFrame(IDR_MAINFRAME)
		)
	{
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

    // Create all the child frames
    CreateChildFrames();

#ifdef DSPLOGGER_AUDIO

	pMainFrame->SetWindowText("Dsp Logger - Audio Debug");
	
#endif

	// The main window has been initialized, so show and update it.
	if(m_bCmdMode)
	{
		m_pMainWnd->ShowWindow(SW_HIDE);
	}
	else
	{
		m_pMainWnd->ShowWindow(SW_SHOW);
	}
    pMainFrame->UpdateWindow();

    //check memory leak!!!
    //_CrtSetBreakAlloc(764);

	lua_init();

	if(m_bCmdMode)
	{	

		BOOL bOK = pMainFrame->ProcExport(m_strTpFile.operator LPCTSTR(),m_strLogFile.operator LPCTSTR());
		if (bOK)
		{
			_tprintf(_T("Convert tp to log sucessfully!\n"));
		}
		else
		{
			_tprintf(_T("Convert tp to log failed!\n"));
		}
		pMainFrame->SendMessage(WM_CLOSE);
		
		return bOK;
		
	}
	return TRUE;
}

void CDspLoggerApp::CreateChildFrames()
{
    CDocument* pDocument = NULL;

    if(m_pTestPointDocTemplate != NULL)
    {
        // Create the TestPoints frame window
        pDocument = m_pTestPointDocTemplate->CreateNewDocument();
        m_pTestPointFrame = (CBasicChildFrame *) m_pTestPointDocTemplate->CreateNewFrame( pDocument, NULL );
        m_pTestPointFrame->SetIcon( m_hRssiIcon, TRUE );    
        //pDocument->SetTitle( "Test Points" );
        pDocument->OnNewDocument();
        m_pTestPointDocTemplate->InitialUpdateFrame( m_pTestPointFrame, pDocument );
        m_pTestPointFrame->ShowWindow(SW_SHOWMAXIMIZED);
		( (CMainFrame *)AfxGetMainWnd() )->UpdatedTitleInfo();
    }

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
	CString	m_strCompiler;
	CString	m_strTime;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
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
void CDspLoggerApp::OnAppAbout()
{
	CXAboutDlg dlg;
	dlg.SetProductName(_T("DspLogger"));
    dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CDspLoggerApp message handlers

void CDspLoggerApp::OnFileNew() 
{
    // Don't create a new child frame window when starting up.
    return;
}

void CDspLoggerApp::OnFileOpen() 
{
	//teana hu 2012.04.11
	CString strFile = m_dlgImport.m_strConvertFile;
	if(!strFile.IsEmpty())
	{
		::DeleteFile(strFile);
	}
	m_dlgImport.DoModal();

}

void CDspLoggerApp::GetVersion()
{
    char szFileName[_MAX_PATH];
    ::GetModuleFileName(AfxGetApp()->m_hInstance,szFileName,_MAX_PATH);
    
    DWORD dwSize,dwHandle;
    dwSize = ::GetFileVersionInfoSize(szFileName,&dwHandle);	
    if( dwSize == 0 )
    {
        return;
    }
    
    char* pVer = new char[dwSize];
	if (NULL == pVer)
	{
		return;
	}
    BOOL bRet = ::GetFileVersionInfo(szFileName,dwHandle,dwSize,pVer);
    if(!bRet)
    {
        delete[] pVer;
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

    m_strVersion = m_strVersion.Left( m_strVersion.GetLength() - 2 );
    
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
    
    delete[] pVer;
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    GetDlgItem( IDC_STATIC_VERSION )->SetWindowText( g_theApp.m_strVersion );
    GetDlgItem( IDC_STATIC_BUILD )->SetWindowText( g_theApp.m_strBuild );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CDspLoggerApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	lua_clean();

	CString strFile = m_dlgImport.m_strConvertFile;
	if(!strFile.IsEmpty())
	{
		::DeleteFile(strFile);
	}

	return CWinApp::ExitInstance();
}
void CDspLoggerApp::LookUpDescription(int nAddr, int nData,char* szDesCription,int nMaxLen)
{
	char szDesc[MAX_LUA_DESCTRIPTION_LEN] = {0};
	lua_description_ex(nAddr, nData,szDesc);
	if(szDesCription)
	{
		strncpy(szDesCription,szDesc,nMaxLen);
	}
}
BOOL CDspLoggerApp::GetDescription(WORD wAddr, WORD wData, LPSTR lpDesc, int nMatchMode)
{
// 	char szDesc[256] = {0};
// 	char* str_tmp=(char *)malloc(256);
// 	lua_description((int)wAddr, (int)wData, &str_tmp);
// 	sprintf(szDesc,"%s",str_tmp);
// 	free(str_tmp);
	char szDesc[MAX_LUA_DESCTRIPTION_LEN] = {0};
	lua_description_ex((int)wAddr, (int)wData,szDesc);

	BOOL bFind = FALSE;
	if(nMatchMode == 1)
	{
		if(strcmp(lpDesc, szDesc) == 0)
		{
			bFind = TRUE;
		}
		else
		{
			bFind = FALSE;
		}
	}
	else
	{
		CString strDesc(szDesc);
		CString strSubDesc(lpDesc);
		strDesc.MakeLower();
		strSubDesc.MakeLower();
		if (strSubDesc.IsEmpty())
		{
			return TRUE;
		}
		if (strDesc.Find(strSubDesc.operator LPCTSTR()) != -1)
		{
			bFind = TRUE;
		}
		else
		{
			bFind = FALSE;
		}
	}
	
	return bFind;
}
