// MobileCnnt.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MobileCnnt.h"

#include "MainFrm.h"
#include "MobileCnntDoc.h"
#include "MobileCnntView.h"
#include "MSInfoView.h"
#include "ConnectView.h"
#include "XAboutDlg.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "USkin.h"
#include "CrashReport.h"

/////////////////////////////////////////////////////////////////////////////
// CMobileCnntApp

BEGIN_MESSAGE_MAP(CMobileCnntApp, CWinApp)
	//{{AFX_MSG_MAP(CMobileCnntApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
//	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMobileCnntApp construction

CMobileCnntApp::CMobileCnntApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pSMSFrame = NULL;
	m_pSMSTemplate = NULL;
	m_pInfoWrapper = NULL;
	m_nSIMState = SIM_PLUGED_SINGLE;
	m_nSIMCount  = 1;

	m_pPortMain        = NULL;
	m_pPortSub		   = NULL;
	m_pPortAttrMain    = NULL;
	m_pPortAttrSub   = NULL;
	m_pPortAttrVModem1 = NULL;
	m_pPortAttrVModem2 = NULL;
	m_pPortAttrUsbModem = NULL;
	m_pEtionFactory = NULL;
	m_bConnected = FALSE;

	m_pSMSFactory  = NULL;
	m_pPBFactory   = NULL;
	m_pPBGFactory   = NULL;
	m_pMemoFactory   = NULL;
	m_tsPcPhone    = COleDateTime::GetCurrentTime() - COleDateTime::GetCurrentTime();	

	m_nMMIVer   = MMI_VERSION_1;
	m_nValidPBNameLen = MAX_PERSON_NAME_LEN;
	m_nValidPBMailLen = 0;
	m_nValidPBNumLen = 0;
	m_hRasConn = NULL;
	m_bHangup = FALSE;

	m_bMuxUsed = FALSE;
	m_bSkinUsed = TRUE;

	m_bManual = FALSE;
	m_nManualPort = 12;
	m_nManualBaud = 115200;
	memset(&m_szCfgPathName, 0, MAX_PATH);

	m_bUSIM = FALSE;
	m_bSupportMemo = FALSE;
	m_bSupportLongSMS = FALSE;
	m_bLoadDialPage = TRUE; 	
	m_bSupportSync = TRUE;
	
	//teana hu 2010.07.26
	m_bLoadAudioPage = TRUE;
	//teana hu 2010.08.12
	ZeroMemory(m_szResDllPathName, MAX_PATH);
	
	m_nPortOpenNum = 1;
	m_nPortOpenInternal = 1000;
	
}/*lint !e1041*/

/////////////////////////////////////////////////////////////////////////////
// The one and only CMobileCnntApp object

CMobileCnntApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMobileCnntApp initialization
_CRT_REPORT_HOOK lpcrh = NULL;
 
int OurReportingFunction(int reportType, char * userMessage, int * retVal)
{
	if(reportType == _CRT_ASSERT)
	{
		theApp.m_pEtionFactory->AutoReportStop();
	}
	return lpcrh(reportType, userMessage, retVal);
}

LPTOP_LEVEL_EXCEPTION_FILTER lpUnexpected = NULL;

LONG WINAPI UnhandledExceptionFilter1(_EXCEPTION_POINTERS *ExceptionInfo)
{
	theApp.m_pEtionFactory->AutoReportStop();
	return lpUnexpected(ExceptionInfo);
}

HOOKCALLBACK lpCrashReportCallback = NULL;

int CrashReportCallBackFunc(_EXCEPTION_POINTERS* lpExceptionInfo)
{
	UNREFERENCED_PARAMETER(lpExceptionInfo);
	//return lpCrashReportCallback(lpExceptionInfo);
	return CRCB_NEED_NO_PROCESS;
}

BOOL CMobileCnntApp::InitInstance()
{
	HANDLE mMutex = ::CreateMutex(NULL, FALSE, m_pszAppName);
	DWORD dwLastErr = GetLastError();
	
	UNREFERENCED_PARAMETER(mMutex);
	::GetModuleFileName( NULL, m_szCfgPathName, _MAX_PATH );
    LPTSTR pExt = _tcsrchr(m_szCfgPathName, _T('.'));
    _tcscpy(pExt + 1, _T("ini"));

	BOOL bMutex = GetPrivateProfileInt( _T("Window"), _T("Mutex"), 1, m_szCfgPathName );


	//当出现Assert或者异常的时候，因为没有关闭自动接收的线程可能工具释放不掉，此处规避该问题
#ifdef _DEBUG
	lpcrh = _CrtSetReportHook(OurReportingFunction);
#endif
	
	lpUnexpected = SetUnhandledExceptionFilter(UnhandledExceptionFilter1);

	AfxEnableControlContainer();

	m_bSkinUsed = GetPrivateProfileInt( _T("Skin"), _T("UseSkin"), 1, m_szCfgPathName );

	m_nPortOpenNum = GetPrivateProfileInt( _T("PortOpenRetry"), _T("RetryTimes"), 1, m_szCfgPathName );
	if(m_nPortOpenNum <=0)
	{
		m_nPortOpenNum = 1;
	}
	m_nPortOpenInternal = GetPrivateProfileInt( _T("PortOpenRetry"), _T("RetryInternal"), 1000, m_szCfgPathName );
	if(m_nPortOpenInternal<0)
	{
		m_nPortOpenInternal = 10;
	}

#ifndef _UNICODE
	if(m_bSkinUsed)
	{
		USkinInit(NULL,NULL,_T("sliver.u3"));
	}
#endif

	int nFalg = 0;	//0: chinese 1: english

	nFalg = GetPrivateProfileInt( _T("Lang"), _T("lang"), 0, m_szCfgPathName );
	_TCHAR       szPathName[_MAX_PATH] = {0};
	_tcscpy(szPathName, m_szCfgPathName);

	if(nFalg == 1)
	{
		LPTSTR pFind = NULL;
		pFind = _tcsrchr(szPathName, _T('\\'));
#ifndef _UNICODE
#ifdef  _DEBUG
		_tcscpy( pFind + 1, _T("ResEnD.dll") );
#else
		_tcscpy( pFind + 1, _T("ResEn.dll") );
#endif
		
#else
#ifdef  _DEBUG
		_tcscpy( pFind + 1, _T("ResEnUD.dll") );
#else
		_tcscpy( pFind + 1, _T("ResEnU.dll") );
#endif
#endif
		_tcscpy(m_szResDllPathName, szPathName);
		
		HINSTANCE m_hResDll = ::LoadLibrary(szPathName);
		if(m_hResDll != NULL)
		{
			AfxSetResourceHandle(m_hResDll);
		}
			
	}

	//放在这里是因为字符串显示需要装载正确的语言之后。
	if(bMutex)
	{
		if(dwLastErr == ERROR_ALREADY_EXISTS)
		{
			AfxMessageBox(IDS_WARN_ALREAY_OPEN);
			return FALSE;
		}
	}

	m_bMuxUsed = GetPrivateProfileInt( _T("MUX"), _T("UseMux"), 0, m_szCfgPathName );

	m_bManual = GetPrivateProfileInt(_T("Setting"),_T("manual"),0,m_szCfgPathName);
	m_nManualPort = GetPrivateProfileInt(_T("Setting"),_T("port"),12,m_szCfgPathName);
	m_nManualBaud = GetPrivateProfileInt(_T("Setting"),_T("baud"),115200,m_szCfgPathName);
	m_bSupportMemo = GetPrivateProfileInt(_T("Setting"),_T("CalendarEnable"),0,m_szCfgPathName);
	m_bSupportLongSMS = GetPrivateProfileInt(_T("Setting"),_T("LongSMSEnable"),0,m_szCfgPathName);
	m_bSupportSync = GetPrivateProfileInt(_T("Setting"),_T("SyncEnable"),1,m_szCfgPathName);
	m_bLoadDialPage = GetPrivateProfileInt(_T("Setting"),_T("LoadDialPage"),1,m_szCfgPathName);
	//teana hu 2010.07.26
	m_bLoadAudioPage = GetPrivateProfileInt(_T("Page"),_T("LoadAudioPage"),1,m_szCfgPathName);

//	if(!IsAdmin())
//	{
//		AfxMessageBox(IDS_ADMIN_PROMPT);
//		return FALSE;
//	}

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

	GetVersion();

	//////////////////////////////////////////////////////////////////////////
	// log
	_TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(NULL,szPath,MAX_PATH);
	_TCHAR *pFind = _tcsrchr(szPath,'.');
	*pFind = '\0';
	_tcscat(szPath,_T(".log"));
	m_log.Open(szPath);


	//teana hu 2009.06.05
#ifdef SPRD8800
	_TCHAR szVersion[20] = {0};
	_tcscpy(szVersion, m_strVersion);

	_TCHAR szDmpFile[MAX_PATH] = {0};
	CString strDump = m_szCfgPathName;
	int nIndex = strDump.ReverseFind('.');
	SYSTEMTIME systime;
	::GetLocalTime(&systime);
	_TCHAR szDmpName[MAX_PATH] = {0};
	_stprintf(szDmpName, _T("_%2d_%2d_%2d_%2d_%2d_%2d_%2d.dmp"), systime.wYear, systime.wMonth, systime.wDay,systime.wHour,systime.wMinute,systime.wSecond,systime.wMilliseconds);
	strDump = strDump.Left(nIndex) + szDmpName;

	_tcscpy(szDmpFile, strDump);
	DWORD dwRet = InitCrashReport(szVersion, FALSE, szDmpFile, CrashReportCallBackFunc);
#endif
	//
	
	if( !CreateMSInfo() )
	{
		AfxMessageBox(IDS_RESOURCE_NOT_ENOUGHT);
		return FALSE;
	}

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMobileCnntDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CConnectView));
//		RUNTIME_CLASS(CMobileCnntView));
	AddDocTemplate(pDocTemplate);

	m_pSMSTemplate = new CMultiDocTemplate(
        IDR_SMS,
        RUNTIME_CLASS(CSMSDoc),
        RUNTIME_CLASS(CSMSFrame),
        RUNTIME_CLASS(CSMSView)
        );

    AddDocTemplate(m_pSMSTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	CreateMSInfoView();

	m_RasSpd.InitSpeedMonitor();

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
void CMobileCnntApp::OnAppAbout()
{
	CXAboutDlg aboutDlg;
#if defined(_EGO)
	aboutDlg.SetProductName(_T("eGo"));
#else
	aboutDlg.SetProductName(_T("SPRD8800"));
#endif
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMobileCnntApp message handlers

void CMobileCnntApp::CreateSMSFrame()
{
	InitWrapper();
	InitSMSPB();
	//create chart frame
	CDocument* pDocument = m_pSMSTemplate->CreateNewDocument();
	_ASSERTE( m_pSMSTemplate != NULL );
	m_pSMSFrame = (CSMSFrame* )m_pSMSTemplate->CreateNewFrame(pDocument, NULL);
	_ASSERTE( m_pSMSFrame != NULL );
	//init frame
	m_pSMSTemplate->InitialUpdateFrame(m_pSMSFrame, pDocument);
//	m_pSMSFrame->ShowWindow(SW_SHOW);

	
}

BOOL CMobileCnntApp::CreateMSInfo(void)
{
	_ASSERTE( m_pPortMain == NULL );
	_ASSERTE( m_pPortSub == NULL );

	if( !CreatePort(&m_pPortMain, PORT_TYPE_COM) )
		return FALSE;

	if(m_bMuxUsed)
	{
		if(!CreatePort(&m_pPortSub, PORT_TYPE_COM))
		{
			return FALSE;
		}
	}

	if( NULL == m_pPortAttrMain )
	{
		m_pPortAttrMain = new PORT_ATTRIBUTE;
		if( NULL == m_pPortAttrMain )
			return FALSE;	

		memset(m_pPortAttrMain, 0, sizeof(PORT_ATTRIBUTE));
	}

	if( NULL == m_pPortAttrSub )
	{
		m_pPortAttrSub = new PORT_ATTRIBUTE;
		if( NULL == m_pPortAttrSub )
			return FALSE;	

		memset(m_pPortAttrSub, 0, sizeof(PORT_ATTRIBUTE));
	}

	if( NULL == m_pPortAttrVModem1 )
	{
		m_pPortAttrVModem1 = new PORT_ATTRIBUTE;
		if( NULL == m_pPortAttrVModem1 )
			return FALSE;	

		memset(m_pPortAttrVModem1, 0, sizeof(PORT_ATTRIBUTE));
	}

	if( NULL == m_pPortAttrVModem2 )
	{
		m_pPortAttrVModem2 = new PORT_ATTRIBUTE;
		if( NULL == m_pPortAttrVModem2 )
			return FALSE;	

		memset(m_pPortAttrVModem2, 0, sizeof(PORT_ATTRIBUTE));
	}

	if( NULL == m_pPortAttrUsbModem )
	{
		m_pPortAttrUsbModem = new PORT_ATTRIBUTE;
		if( NULL == m_pPortAttrUsbModem )
			return FALSE;	

		memset(m_pPortAttrUsbModem, 0, sizeof(PORT_ATTRIBUTE));
	}
	
	if( !CreateSMS( &m_pSMSFactory ) )
		return FALSE;
	
	if( !CreatePB( &m_pPBFactory ) )
		return FALSE;

	if( !CreatePBG( &m_pPBGFactory) )
		return FALSE;

	if( !CreateMemo( &m_pMemoFactory) )
		return FALSE;

	if(!CreateEtion( &m_pEtionFactory))
		return FALSE;

	_TCHAR szFileName[_MAX_PATH] = { 0 };

    ::GetModuleFileName( NULL, szFileName, _MAX_PATH );
    LPTSTR pExt = _tcsrchr(szFileName, _T('.'));
    _tcscpy(pExt + 1, _T("ini"));

    BOOL bLog = GetPrivateProfileInt( _T("Log"), _T("Enable"), 0, szFileName );
	_tcscpy(szFileName, m_szCfgPathName);
    if( bLog )
    {
		SYSTEMTIME systime;
		::GetLocalTime(&systime);
		_TCHAR szPostFix[50] = {0};
		if(m_bMuxUsed)
		{
			_stprintf(szPostFix, _T("_%2d_%2d_%2d_%2d_%2d_%2d_%2d_com_main.log"), systime.wYear,systime.wMonth,
				systime.wDay,systime.wHour ,systime.wMinute,systime.wSecond,systime.wMilliseconds);
			_tcscpy(pExt, szPostFix);
			m_pPortMain->StartLog( szFileName );

			_stprintf(szPostFix, _T("_%2d_%2d_%2d_%2d_%2d_%2d_%2d_com_sub.log"), systime.wYear,systime.wMonth,
				systime.wDay,systime.wHour,systime.wMinute,systime.wSecond,systime.wMilliseconds);
			_tcscpy(pExt, szPostFix);
			m_pPortSub->StartLog( szFileName );
		}
		else
		{
#if defined(_EGO)// && defined(_DEBUG)
		_tcscpy(pExt, _T("_com.log"));
		m_pPortMain->StartLog(szFileName);
#else
			_stprintf(szPostFix, _T("_%2d_%2d_%2d_%2d_%2d_%2d_%2d_com.log"), systime.wYear,systime.wMonth,
				systime.wDay,systime.wHour,systime.wMinute,systime.wSecond,systime.wMilliseconds);
			_tcscpy(pExt, szPostFix);
			m_pPortMain->StartLog( szFileName );
#endif

		}
#if defined(_EGO) //&& defined(_DEBUG)
		_tcscpy(pExt, _T("_sms.log"));
		m_pSMSFactory->StartLog(szFileName);
		
		_tcscpy(pExt, _T("_pb.log"));
		m_pPBFactory->StartLog(szFileName);
#endif
    }

	return TRUE;
}

void CMobileCnntApp::ReleaseMSInfo(void)
{
	if(m_pPortMain  != NULL)
	{
		ClosePort();
	}

	if( m_pPortAttrMain != NULL )
	{
		delete m_pPortAttrMain;
		m_pPortAttrMain = NULL;
	}

	if( m_pPortAttrSub != NULL )
	{
		delete m_pPortAttrSub;
		m_pPortAttrSub = NULL;
	}

	if( m_pPortAttrVModem1 != NULL )
	{
		delete m_pPortAttrVModem1;
		m_pPortAttrVModem1 = NULL;
	}

	if( m_pPortAttrVModem2 != NULL )
	{
		delete m_pPortAttrVModem2;
		m_pPortAttrVModem2 = NULL;
	}

	if( m_pPortAttrUsbModem != NULL )
	{
		delete m_pPortAttrUsbModem;
		m_pPortAttrUsbModem = NULL;
	}

	if( m_pPortMain != NULL )
    {
        m_pPortMain->StopLog();
		m_pPortMain->Release();
		m_pPortMain = NULL;
    }

	if( m_pPortSub != NULL)
	{
		m_pPortSub->StopLog();
		m_pPortSub->Release();
		m_pPortSub = NULL;
	}

		if( m_pSMSFactory != NULL )
    {
        m_pSMSFactory->StopLog();
		m_pSMSFactory->Release();
		m_pSMSFactory = NULL;
    }

	if( m_pPBGFactory != NULL )
    {
        m_pPBGFactory->StopLog();
		m_pPBGFactory->Release();
		m_pPBGFactory = NULL;
    }

	if( m_pPBFactory != NULL )
    {
        m_pPBFactory->StopLog();
		m_pPBFactory->Release();
		m_pPBFactory = NULL;
    }

	if( m_pMemoFactory != NULL )
    {
        m_pMemoFactory->StopLog();
		m_pMemoFactory->Release();
		m_pMemoFactory = NULL;
    }
	
	if( m_pEtionFactory != NULL)
	{
		m_pEtionFactory->Release();
		m_pEtionFactory = NULL;
	}

}

void CMobileCnntApp::GetVersion()
{
    TCHAR szFileName[_MAX_PATH];
    ::GetModuleFileName(AfxGetApp()->m_hInstance,szFileName,_MAX_PATH);
    
    DWORD dwSize,dwHandle;
    dwSize = ::GetFileVersionInfoSize(szFileName,&dwHandle);	
    if( dwSize == 0 )
    {
        return;
    }
    
    TCHAR* pVer = new TCHAR[dwSize];
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
        m_strVersion = (TCHAR*)lpBuffer;
        m_strVersion.Replace( _T(","),_T(".") );
        m_strVersion.Replace( _T(" "),_T("") );
    }

	CString strVer = m_strVersion;
	int nLastDot = strVer.ReverseFind(_T('.')); 
	CString strBuild = strVer.Right(strVer.GetLength() - nLastDot-1);		
	m_strVersion = strVer.Left(nLastDot);
	
	int nBuild = _ttoi(strBuild);
	m_strBuild.Format(_T("%03d"),nBuild);
    
    // Read the build information
//     wsprintf( SubBlock, 
//         TEXT("\\StringFileInfo\\%04x%04x\\PrivateBuild"),
//         lpTranslate[0].wLanguage,
//         lpTranslate[0].wCodePage);
//     
//     // Retrieve file description for language and code page "i". 
//     bRet = VerQueryValue(pVer, 
//         SubBlock, 
//         &lpBuffer, 
//         &cbTranslate); 
//     if( bRet )
//     {
//         m_strBuild = (char*)lpBuffer;
//         m_strBuild.Replace( _T(","),_T(".") );
//     }
    
    delete [] pVer;
}

int CMobileCnntApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class

#ifdef _EGO
	if(m_pPortMain)
	{
			m_pPortMain->StopAutoReceive();
	}
#endif
	
	m_RasSpd.ClearSpeedMonitor();

	ReleaseWrapper();

	ReleaseMSInfo();
#ifndef _UNICODE
	if(m_bSkinUsed)
	{
		USkinExit();
	}
#endif

	m_log.Close();

	return CWinApp::ExitInstance();
}

BOOL CMobileCnntApp::OpenPort()
{
	_ASSERTE( m_pPortMain != NULL );
	_ASSERTE( m_pPortAttrMain != NULL );	

	m_pPortMain->Close();

	Sleep(500);

	if(m_bMuxUsed)
	{
		_ASSERTE( m_pPortSub != NULL );
		_ASSERTE( m_pPortAttrSub != NULL );
		m_pPortSub->Close();

		Sleep(500);
		
		
		if( _tcslen(m_pPortAttrSub->Com.szName) > 0 && m_pPortAttrSub->Com.dwBaudRate > 0 )
		{
			BOOL bOpen = FALSE;
			int nCount = m_nPortOpenNum;
			while(nCount--)
			{
				bOpen = m_pPortSub->Open(m_pPortAttrSub);
				if(bOpen)
					break;
				m_pPortSub->Close();
				Sleep(m_nPortOpenInternal);

			}
			if( bOpen )
			{
				m_pEtionFactory->SetPort(m_pPortSub, TRUE);
				m_bConnected = TRUE;
			}
			else
			{				
				m_pPortSub->Close();
				m_bConnected = FALSE;
			}
			
		}
	}
	
#if defined(_EGO)

    if(m_bManual)
	{
		CString strPort;
		strPort.Format(_T("COM%d"),m_nManualPort);
		_tcscpy(m_pPortAttrMain->Com.szName,strPort);
		m_pPortAttrMain->Com.dwBaudRate = m_nManualBaud;
	}
#endif

	if( _tcslen(m_pPortAttrMain->Com.szName) > 0 && m_pPortAttrMain->Com.dwBaudRate > 0 )
	{
		BOOL bOpen = FALSE;
		int nCount = m_nPortOpenNum;
		while(nCount--)
		{
			bOpen = m_pPortMain->Open(m_pPortAttrMain);
			if(bOpen)
				break;
			m_pPortMain->Close();
			Sleep(m_nPortOpenInternal);
			
		}

		if( bOpen )
		{
			//CPhone phone;
#ifdef _EGO
			m_pPortMain->StartAutoReceive(m_pMainWnd->GetSafeHwnd(), NULL, 0, WM_RCV_AUTO_REPORT);
#endif
			m_Phone.SetPort( m_pPortMain );
			m_bConnected = m_Phone.IsConnected();
			if( m_bConnected )
			{
#ifdef _EGO
				m_nSIMState = m_Phone.GetSIMState();
				m_nMMIVer   = m_Phone.GetMMIVersion();
				m_nSIMCount = m_Phone.GetSIMCount();
#endif
				m_pSMSFactory->SetPort( m_pPortMain );
				m_pPBFactory->SetPort( m_pPortMain );
				m_pPBGFactory->SetPort( m_pPortMain );
				m_pMemoFactory->SetPort( m_pPortMain );

#if defined(SPRD8800)	
				m_pPBFactory->GetPBParam(PB_PARAM_IS_USIM,(DWORD*)&theApp.m_bUSIM);
#endif
				
				m_pEtionFactory->SetPort(m_pPortMain);
				m_bConnected = TRUE;
			}
		}
		else
		{
			m_pPortMain->Close();
			m_bConnected = FALSE;
		}

	}

	if(m_bConnected)
	{
		((CMainFrame *)m_pMainWnd)->StartAutoReport();
	}

	return m_bConnected;
}

BOOL CMobileCnntApp::ClosePort()
{
	_ASSERTE( m_pPortMain  != NULL );
	m_pEtionFactory->AutoReportStop();
	if(m_bMuxUsed)
	{
		_ASSERTE(m_pPortSub != NULL);
		m_pPortSub->Close();
	}
	m_pPortMain->Close();
	m_bConnected = FALSE;
	Sleep(500);
	
	return TRUE;
}


BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem( IDC_STATIC_VERSION )->SetWindowText( theApp.m_strVersion );
    GetDlgItem( IDC_STATIC_BUILD )->SetWindowText( theApp.m_strBuild );
#ifndef SPRD8800
	GetDlgItem(IDC_STC_PRD_NAME)->SetWindowText(_T("eGo"));
	CString strText;
	GetWindowText(strText);
	strText.Replace(_T("SPRD8800"),_T("eGo"));
	SetWindowText(strText);
#endif

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMobileCnntApp::CreateMSInfoView()
{
	CView *pNewActiveView = NULL; 
 	
    pNewActiveView = (CView *) new CMSInfoView();
	((CMainFrame *)m_pMainWnd)->m_pMSInfoView = (CMSInfoView *)pNewActiveView;


	CMobileCnntDoc *pDoc = (CMobileCnntDoc *)(((CMainFrame *)m_pMainWnd)->GetActiveDocument());
		
	CCreateContext context;
	context.m_pNewViewClass = NULL;
	context.m_pNewDocTemplate = NULL;
	context.m_pLastView = NULL;
	context.m_pCurrentDoc   = pDoc;
	context.m_pCurrentFrame = NULL;
	
	pNewActiveView->Create( NULL, 
		NULL, 
		WS_BORDER|WS_CHILD, 
		CFrameWnd::rectDefault, 
		m_pMainWnd, 
		IDD_FORMVIEW_MSINFO, 
		&context );
	
	pNewActiveView->OnInitialUpdate();
}

BOOL CMobileCnntApp::HangUp()
{
	if(m_hRasConn != NULL)
	{
		m_bHangup = FALSE;
		
//		if(((CMainFrame *)m_pMainWnd)->m_pDialUPView != NULL)
//		{
//			((CMainFrame *)m_pMainWnd)->m_pDialUPView->SendDetachPS();
//		}
		
		RasHangUp(m_hRasConn);
		
		m_hRasConn = NULL;
		Sleep(3000);
		m_bHangup = TRUE;
	}
	return TRUE;
}

BOOL CMobileCnntApp::IsAdmin()
{
    HANDLE                   hAccessToken;
    BYTE                     *InfoBuffer = NULL;
    PTOKEN_GROUPS            ptgGroups;
    DWORD                    dwInfoBufferSize;
    PSID                     psidAdministrators;
    SID_IDENTIFIER_AUTHORITY siaNtAuthority = SECURITY_NT_AUTHORITY;
    UINT                     i;
    BOOL                     bRet = FALSE;
	
    if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&hAccessToken))
		goto cleanup;
    
    InfoBuffer = new BYTE[1024];
    if(!InfoBuffer)
		goto cleanup;
	
    bRet = GetTokenInformation(hAccessToken,
		TokenGroups,
		InfoBuffer,
		1024,
		&dwInfoBufferSize);
	
    CloseHandle( hAccessToken );
	
    if(!bRet)
		goto cleanup;
	
    if(!AllocateAndInitializeSid(&siaNtAuthority,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0,0,0,0,0,0,
		&psidAdministrators))
		goto cleanup;
	
    bRet = FALSE;
	
    ptgGroups = (PTOKEN_GROUPS)InfoBuffer;
	
    for(i=0;i<ptgGroups->GroupCount;i++)
    {
        if(EqualSid(psidAdministrators,ptgGroups->Groups[i].Sid))
        {
            bRet = TRUE;
            break;
        }
    }
	
    FreeSid(psidAdministrators);
	
cleanup:
	
    if (InfoBuffer)
	{
		delete [] InfoBuffer;
		InfoBuffer = NULL;
	}
        
	
    return bRet;
}

BOOL CMobileCnntApp::InitWrapper()
{
	if( NULL == m_pInfoWrapper )
	{
		m_pInfoWrapper = new CInfoWrapper;
		if( NULL == m_pInfoWrapper || !m_pInfoWrapper->Init() )
		{
			AfxMessageBox(IDS_RESOURCE_NOT_ENOUGHT);
			return FALSE;
		}
	}
	return TRUE;
}

void CMobileCnntApp::ReleaseWrapper()
{
	if( m_pInfoWrapper != NULL )
	{
		m_pInfoWrapper->Release();
		delete m_pInfoWrapper;
		m_pInfoWrapper = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
///move from mobilesprite
BOOL CMobileCnntApp::TestIsConnected( BOOL bShowInfo /* = TRUE */, BOOL bTestMore /*= FALSE*/)
{
	UNREFERENCED_PARAMETER(bTestMore);

	_ASSERTE( m_pPortMain  != NULL );

    //CPhone phone;
    m_Phone.SetPort( m_pPortMain );

	m_bConnected = m_Phone.IsConnected();
    if( !m_bConnected && bShowInfo )
		AfxMessageBox( IDS_ERR_NOT_CONNECT );

	return m_bConnected;
}

BOOL CMobileCnntApp::InitSMSPB()
{
#ifndef SPRD8800
	//CPhone phone;
	m_Phone.SetPort( theApp.m_pPortMain );
	m_Phone.InitPhone();
	m_nSIMState = m_Phone.GetSIMState();
	m_nMMIVer   = m_Phone.GetMMIVersion();
	m_nSIMCount = m_Phone.GetSIMCount();
#endif
				
	m_pPBFactory->SetPBParam(PB_PARAM_VERSION,(DWORD *)&m_nMMIVer);
	m_pPBGFactory->SetPBGParam(PBG_PARAM_VERSION,(DWORD *)&m_nMMIVer);
	m_pMemoFactory->SetMemoParam(MEMO_PARAM_VERSION,(DWORD *)&m_nMMIVer);

#ifndef SPRD8800
	if(m_bConnected)
	{
		if(theApp.m_bSupportMemo)
		{
			//取得手机的时间
			COleDateTime tPc = COleDateTime::GetCurrentTime();
			_DATE_TIME_T * pDT = NULL;
			pDT = m_pMemoFactory->GetPhoneDate();
			if(pDT != NULL)
			{
				int nYear = pDT->nYear;
				if(nYear >=80)
				{
					nYear+=1900;
				}
				else
				{
					nYear+=2000;
				}
				COleDateTime tPhone(nYear,pDT->nMonth,pDT->nDay,
				pDT->nHour,pDT->nMinut,pDT->nSecond);
							
				m_tsPcPhone = tPc - tPhone;		
			}
			else
			{
				m_tsPcPhone = 0;
			}	
		}
					
		DWORD *pdwLen = new DWORD[3];
		memset(pdwLen,0,sizeof(DWORD)*3);
		if(m_pPBFactory->GetPBParam(PB_PARAM_NAME_LEN,pdwLen))
		{
			m_nValidPBNameLen = pdwLen[0];
			m_nValidPBMailLen = pdwLen[1];
			m_nValidPBNumLen = pdwLen[2];
		}

		delete [] pdwLen;
	}

#endif			

	return TRUE;
}

BOOL CMobileCnntApp::IsPluggedSIM(int nSIMNum)
{
	if(nSIMNum<1 || nSIMNum> MAX_SIM_NUM)
		return FALSE;

	if(nSIMNum == 1 && m_nSIMState == SIM_PLUGED_SINGLE)
	{
		return TRUE;
	}

	BYTE bTemp = (BYTE)m_nSIMState;
    if( ((bTemp>>(nSIMNum-1)) & 1) == 1)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}