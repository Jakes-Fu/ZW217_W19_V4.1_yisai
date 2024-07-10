// NVEditor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "NVEditor.h"
#include "MiniDump.h"
#include "MainFrm.h"
#include "NVEditorDoc.h"
#include "NVEditorView.h"
#include "StiSettingDlg.h"
//#include <conio.h>
#include <stdio.h>

#include "XAboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HANDLE g_hConsole = INVALID_HANDLE_VALUE;
HANDLE g_hInConsole = INVALID_HANDLE_VALUE;

extern BOOL g_bBigEndian;

// HandleExceptionFilter function
LONG WINAPI HandleExceptionFilter(_EXCEPTION_POINTERS* lpExceptionInfo)
{
	//AfxGetMainWnd()->SendMessage(WM_CLOSE,0,0);
	return EXCEPTION_EXECUTE_HANDLER;
}
/////////////////////////////////////////////////////////////////////////////
// CNVEditorApp

BEGIN_MESSAGE_MAP(CNVEditorApp, CWinApp)
	//{{AFX_MSG_MAP(CNVEditorApp)
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

void CNVCmdLine::ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast )
{
	UNREFERENCED_PARAMETER(bLast);
//	if( bFlag )
//	{
		// Do not support any flag now
//		return;
//	}

//	if( m_nParaCount > m_nMaxParaCount )
//	{
//		// Only support three parameters
//		return;
//	}

	CString strParam = lpszParam;
	strParam.TrimLeft();
	strParam.TrimRight();

	if( 0 == m_nParaCount )
	{
		if(!bFlag && strParam.Right(4).CompareNoCase(_T(".prj")) == 0)
		{
			m_strFileName = strParam;
		}
		/*xml file support by xmzhang add begin*/
		else if(!bFlag && strParam.Right(5).CompareNoCase(_T(".xprj")) == 0)
		{
			m_strFileName = strParam;
		}
		/*xml file support by xmzhang add end*/
	}
	else
	{
		if(strParam.Right(3).CompareNoCase(_T(".nv")) == 0)
		{
			m_strImportFile = lpszParam;
		}
		else if(strParam.Right(4).CompareNoCase(_T(".bin")) == 0)
		{
			m_strDspBinFile = lpszParam;
		}
		else if(strParam.Right(4).CompareNoCase(_T(".txt")) == 0)
		{
			m_strErrorFile = lpszParam;
		}
		else if(strParam.CompareNoCase(_T("LittleEndian")) == 0 ||
		  strParam.CompareNoCase(_T("LE")) == 0 || 
		  (bFlag && (strParam.CompareNoCase(_T("l")) == 0)) )
		{
			m_bBigEndian = FALSE;
		}
		else if(bFlag && strParam.CompareNoCase(_T("w")) == 0)
		{
			m_bWait = TRUE;
		}
		//[ add by Joly
		else if (bFlag && 0 == strParam.CompareNoCase(_T("t")))
		{
			m_bTail = FALSE;
		}
		// add for version info
		else if (bFlag && 0 == strParam.CompareNoCase(_T("v")))
		{
			m_bVersion = TRUE;
		}
		//]]
		
	}
	m_nParaCount++;
}
//[[ by Joly
//-id ID FileName
void CNVCmdLine::ParseCommandLine()
{
	for ( int i = 1; i < __argc; i++ )
	{
		LPCTSTR pszPara = __targv[i];
		BOOL    bFlag = FALSE;
		BOOL    bLast = ( ( i + 1 ) == __argc );


		CString str1(pszPara);
		if ( 0 == str1.CompareNoCase(_T("-id")) )
		{	
			ST_FILE_INFO sFileInfo;
			if(
				NULL != _tcschr(__targv[i+1], 'x')	||
				NULL != _tcschr(__targv[i+1], 'X')
				)
			{
				sFileInfo.nFileID  = _tcstol(__targv[i+1], NULL, 16);
			}
			else
			{
				sFileInfo.nFileID  = _tcstol(__targv[i+1], NULL, 10);
			}													
			_tcscpy(sFileInfo.szFilePath, __targv[i+2]);
			sFileInfo.bCheckFlag = TRUE;
			
			m_vecFileInfo.push_back(sFileInfo);
			i += 2;


		}
		// [[add for SamSung NV update
		else if (0 == str1.CompareNoCase(_T("-m")))
		{
			m_strMergePath.Format(_T("%s"), __targv[i + 1]);
			CString strParam = __targv[i + 2];
			strParam.TrimLeft();
        	strParam.TrimRight();
			if(0 == strParam.Right(2).CompareNoCase(_T(".h")))
			{
				m_strHeaderFile = strParam;
				i += 2;
			}
			else
			{
				i += 1;
			}
		}//]]
		// add for Other customer NV update 
		else if (0 == str1.CompareNoCase(_T("-c")))
		{
			m_bHeaderFile = FALSE;
			m_strMergePath.Format(_T("%s"), __targv[i + 1]);
			CString strParam = __targv[i + 2];
			strParam.TrimLeft();
			strParam.TrimRight();
			if(0 == strParam.Right(4).CompareNoCase(_T(".bin")))
			{
				m_strHeaderFile = strParam;
				i += 2;
			}
			else
			{
				i += 1;
			}
		}
		//[[ add for 7731  nv modify: merge fix bin
		else if (0 == str1.CompareNoCase(_T("-f")))
		{
			m_strFixBinFile.Format(_T("%s"), __targv[i + 1]);
			i += 1;
		}
		// add for hash value
		else if (0 == str1.CompareNoCase(_T("-h")))
		{
			m_bVersion = TRUE; //若要生成包含hash value的头文件，.bin文件必须有版本信息，以便cp比较。
			m_strHashHeader.Format(_T("%s"), __targv[i + 1]);
			i += 1;
		}
		// add for check item name
		else if (0 == str1.CompareNoCase(_T("-FastMode")))
		{
			m_bCheckItemName = FALSE;
		}
		else 
		{
			if ( pszPara[0] == '-' || pszPara[0] == '/' )
			{
				bFlag = TRUE;
			    ++pszPara;
			}
			ParseParam( pszPara, bFlag, bLast );
		}
	}
}
//]]
/////////////////////////////////////////////////////////////////////////////
// CNVEditorApp construction

CNVEditorApp::CNVEditorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bCmdMode					= FALSE;
	m_nCmdRet					= 0;
	m_pErrorFile				= NULL;
	m_bWriteReadOnlyZeroItem	= FALSE;
	m_bNvmFormat				= TRUE;
	m_bShowWorning				= FALSE;
	m_dwEnumPortTimeOut			= 5000;
	m_dwPermission				= READ_PERMISSION;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CNVEditorApp object

CNVEditorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CNVEditorApp initialization

BOOL CNVEditorApp::InitInstance()
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

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey( _T("Spreadtrum") );

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

    GetVersion();
	::SetUnhandledExceptionFilter(HandleExceptionFilter);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CNVEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CNVEditorView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CNVCmdLine cmdInfo;
	cmdInfo.ParseCommandLine();  //modify by Joly
	/*xml file support by xmzhang modify begin*/
	CoInitialize( NULL );//xmzhang move ahead for xmlparser

	TCHAR  szMduPath[_MAX_PATH]	= {0};
	_TCHAR szTitle[_MAX_PATH]	= {0};
	BOOL   bShowVer				= TRUE;
	::GetModuleFileName(m_hInstance,szMduPath,_MAX_PATH);
	TCHAR* pResult = _tcsrchr(szMduPath,'.');
	if(pResult != NULL)
	{
		*pResult = 0;
		_tcscat(szMduPath,_T(".ini"));	
	}
	m_bNvmFormat = ::GetPrivateProfileInt(_T("Settings"),_T("ModuleFormat"),1,szMduPath);
	m_bShowWorning = ::GetPrivateProfileInt(_T("Settings"),_T("ShowWarning"),0,szMduPath);
	  
	if( !cmdInfo.m_strFileName.IsEmpty() )
	{
		if(!cmdInfo.m_strErrorFile.IsEmpty())
		{
			m_pErrorFile = _tfopen(cmdInfo.m_strErrorFile,_T("wt"));
		}
		m_nCmdRet = !DoCmdLine(cmdInfo); // modify by Joly	

		if(m_pErrorFile != NULL)
		{
			fclose(m_pErrorFile);
			m_pErrorFile = NULL;
		}
	
		if(cmdInfo.m_bWait)
		{
			BYTE ag[10] = {0};
			unsigned long lRead = 0;
			ReadConsole(g_hInConsole,ag,1,&lRead,NULL);
		}

		// Exit program
		return FALSE;
	}
		
	g_bBigEndian = ::GetPrivateProfileInt(_T("Settings"),_T("BigEndian"),1,szMduPath);
	
	m_dwPermission				= ::GetPrivateProfileInt(_T("Settings"),_T("Super"),READ_PERMISSION,szMduPath);
	m_bWriteReadOnlyZeroItem	= ::GetPrivateProfileInt(_T("Settings"),_T("WriteReadOnlyZeroItem"),0,szMduPath);
	bShowVer					= ::GetPrivateProfileInt(_T("GUI"),_T("ShowVer"),1,szMduPath);  
	m_dwEnumPortTimeOut			= ::GetPrivateProfileInt(_T("SecondEnumPort"),_T("EnumPortTimeOut"),5000,szMduPath);
	GetPrivateProfileString(_T("GUI"),_T("Title"),_T(""),szTitle,_MAX_PATH,szMduPath);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	

	CString strTitle;
	CString strVersion;
	CString strEndian;
	
	if(bShowVer)
	{
		CXVerInfo ver;
		strVersion = ver.GetSPRDVersionString();
	}
	
	if(_tcslen(szTitle) != 0)
	{
		strTitle.Format(_T("NVEditor [ %s ]"),szTitle);
	}
	else
	{
		strTitle = _T("NVEditor");
	}	
	
	if(bShowVer)
	{
#ifdef _DEBUG
		strTitle += _T(" - D");
#else 
		strTitle += _T(" - R");
#endif
		strTitle += strVersion;	
	}

	strEndian.Format(_T(" - %s"),g_bBigEndian?_T("BigEndian"):_T("LittleEndian"));
	
	strTitle += strEndian;
    
    m_pMainWnd->SetWindowText (strTitle);

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
void CNVEditorApp::OnAppAbout()
{
	CXAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CNVEditorApp message handlers

void CNVEditorApp::GetVersion()
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
        m_strVersion = (TCHAR*)lpBuffer;
        m_strVersion.Replace( _T(","),_T(".") );
        m_strVersion.Replace( _T(" "),_T("") );
        int nIndex = m_strVersion.ReverseFind( _T('.') );
        if( nIndex != -1 )
        {
            // The last field is used for build number
            m_strBuild = m_strVersion.Right( 1 );
            m_strBuild.Insert( 0,_T("00") );
            m_strVersion.Delete( nIndex,2 );
        }
    }

    
    delete[] pVer;
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    GetDlgItem( IDC_STATIC_VERSION ) ->SetWindowText( theApp.m_strVersion );
    GetDlgItem( IDC_STATIC_BUILD ) ->SetWindowText( theApp.m_strBuild );
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CNVEditorApp::ExitInstance() 
{
	if( m_bCmdMode )
	{
		return m_nCmdRet;
	}
	else
	{
		CoUninitialize();	
		return CWinApp::ExitInstance();
	}
}

// open a nv project file and generate nvitem.bin without gui
// modify by Joly[[
BOOL CNVEditorApp::DoCmdLine(CNVCmdLine& cmdInfo)
{
	m_bCmdMode = TRUE;
	TCHAR *szappend;/*xml file support by xmzhang add*/

	extern BOOL g_bBigEndian;
	g_bBigEndian = cmdInfo.m_bBigEndian;

	AllocConsole();
	HANDLE hWrite = GetStdHandle(STD_OUTPUT_HANDLE);
	g_hConsole = hWrite;	
	unsigned long lWrite;

	g_hInConsole = GetStdHandle(STD_INPUT_HANDLE);

	TCHAR sz_not_find_prj[] = _T( "Not find prj/xprj bin file\r\n" );/*xml file support by xmzhang modify*/
	TCHAR sz_not_find_dsp[] = _T( "Not find dsp bin file\r\n" );
	TCHAR sz_not_find_nv[] = _T( "Not find nv bin file\r\n" );
	TCHAR sz_open_prj[] = _T( "Open project...\r\n" );
	TCHAR sz_save_img[] = _T( "Save nvitem bin file...\r\n" );
	TCHAR sz_success[] = _T( "finished\r\n" );
	TCHAR sz_failed[] = _T( "failed\r\n" );
	TCHAR sz_load_imp_file_failed[] = _T( "Load import file failed\r\n" );
	TCHAR sz_save_prj_failed[] = _T( "Save project file failed\r\n" );
	TCHAR sz_nv_config_failed[] = _T("Merge configure nv failed\r\n");  // for SamSung update NV

	BOOL bCheckOK = TRUE;
	CFileFind finder;
	if(!cmdInfo.m_strFileName.IsEmpty() && !finder.FindFile(cmdInfo.m_strFileName))
	{
		WriteConsole(hWrite, sz_not_find_prj, _tcslen( sz_not_find_prj ), &lWrite,NULL);
		bCheckOK = FALSE;
	}

	if(!cmdInfo.m_strImportFile.IsEmpty() && !finder.FindFile(cmdInfo.m_strImportFile))
	{
		WriteConsole(hWrite, sz_not_find_nv, _tcslen(sz_not_find_nv), &lWrite,NULL);
		bCheckOK = FALSE;
	}

	if(!cmdInfo.m_strDspBinFile.IsEmpty() && !finder.FindFile(cmdInfo.m_strDspBinFile))
	{
		WriteConsole(hWrite, sz_not_find_dsp, _tcslen(sz_not_find_dsp), &lWrite, NULL);
		bCheckOK = FALSE;
	}
	if(!bCheckOK)
	{
		g_hConsole = INVALID_HANDLE_VALUE;
		return FALSE;
	}

	std::auto_ptr<CFileParser>  fp(new CFileParser);
		

	//CFileParser* fp = new CFileParser();
	BOOL bRet = FALSE;/*xml file support by xmzhang add*/
	WriteConsole(hWrite, sz_open_prj, _tcslen(sz_open_prj), &lWrite, NULL);

	/*xml file support by xmzhang modify begin*/
	CStringArray agWarning;
	szappend = _tcsrchr(cmdInfo.m_strFileName,'.');
	if(0 == _tcscmp(szappend,_T(".xprj")))
	{
		bRet = fp->LoadProjectXML(cmdInfo.m_strFileName.GetBuffer(0),agWarning,cmdInfo.m_bCheckItemName);
	}
	else
	{
		bRet = fp->LoadProject(cmdInfo.m_strFileName.GetBuffer(0),agWarning,cmdInfo.m_bCheckItemName);
	}
	if( !bRet )
	{
		WriteConsole( hWrite,sz_failed,_tcslen( sz_failed ),&lWrite,NULL );
		g_hConsole = INVALID_HANDLE_VALUE;
		return bRet;
	}
	/*xml file support by xmzhang modify end*/

	fp->ClearCalibrationFlag();

	if(!cmdInfo.m_strImportFile.IsEmpty())
	{
		// Import file 
		bRet = fp->ImportSingleNVFile(cmdInfo.m_strImportFile);
		if(!bRet)
		{
			WriteConsole( hWrite,sz_load_imp_file_failed,_tcslen( sz_load_imp_file_failed ),&lWrite,NULL );
			g_hConsole = INVALID_HANDLE_VALUE;
			return bRet;
		}
		
	}
	/*xml file support by xmzhang modify begin*/
	if(0 == _tcscmp(szappend,_T(".xprj")))
	{
		bRet = fp->SaveProjectXML();
	}
	else
	{
		bRet = fp->SaveProject();
	}
	/*xml file support by xmzhang modify end*/
	if(!bRet)
	{
		WriteConsole(hWrite, sz_save_prj_failed, _tcslen(sz_save_prj_failed), &lWrite, NULL);
		g_hConsole = INVALID_HANDLE_VALUE;
		return bRet;
	}
	//[[ add for fix bin merge
	if (!cmdInfo.m_strFixBinFile.IsEmpty())
	{
		fp->SetWriteFix(cmdInfo.m_strFixBinFile.operator LPCTSTR());
	}
	//]]
	// for hash header
	if (!cmdInfo.m_strHashHeader.IsEmpty())
	{
		fp->SetHashHeaderPath(cmdInfo.m_strHashHeader.operator LPCTSTR());
	}

	if(!cmdInfo.m_strDspBinFile.IsEmpty())
	{
		fp->SetWriteDsp(TRUE);
	}
	else
	{
		fp->SetWriteDsp(FALSE);
	}
	// [[ by Joly
	if (cmdInfo.m_vecFileInfo.size()> 0)
	{
		CUIntArray uIntArray;
		fp->SetMulFileAg(TRUE, cmdInfo.m_vecFileInfo);
		if(fp->IsExistedID(uIntArray))
		{
			CString strErrMsg(_T("The following ID had existed,please input news:\n"));
			for (int i = 0; i < uIntArray.GetSize(); i++)
			{
				CString strMsg;
				strMsg.Format(_T("FileID %d existed \n"),uIntArray.GetAt(i));
				strErrMsg += strMsg;
			}			
			WriteConsole(hWrite, strErrMsg, _tcslen( strErrMsg ), &lWrite, NULL);
			g_hConsole = INVALID_HANDLE_VALUE;
	
	    	return 0;
		}
		
	}
	// ]]

	WriteConsole(hWrite,sz_save_img,_tcslen( sz_save_img ),&lWrite,NULL);

	fp->SetWriteTail(cmdInfo.m_bTail);

	fp->SetVersionFlag(cmdInfo.m_bVersion);
	
	bRet = fp->SaveToImage(cmdInfo.m_strDspBinFile.GetBuffer(0));

	if(bRet)
	{
		WriteConsole(hWrite, sz_success, _tcslen( sz_success), &lWrite, NULL);
	}
	else
	{
		WriteConsole(hWrite, sz_failed, _tcslen(sz_failed), &lWrite, NULL);
	}

	//[[ support for NV update by Joly
	
	if (!cmdInfo.m_strMergePath.IsEmpty())
	{
		if (cmdInfo.m_bHeaderFile)
		{
			if (cmdInfo.m_strHeaderFile.IsEmpty())
			{ 
				cmdInfo.m_strHeaderFile.Format(_T("%s\\ss_nv_config.h"), cmdInfo.m_strMergePath);
			}
	
		}
		bRet = fp->GenerateConfigFile(cmdInfo.m_strMergePath.GetBuffer(0), cmdInfo.m_strHeaderFile.GetBuffer(0), cmdInfo.m_bHeaderFile);
		if( !bRet )
		{
			WriteConsole(hWrite, sz_nv_config_failed, _tcslen(sz_nv_config_failed), &lWrite, NULL);
			g_hConsole = INVALID_HANDLE_VALUE;
			return bRet;
		}
	}
	//]]
	g_hConsole = INVALID_HANDLE_VALUE;

// 	delete fp;
// 	fp = NULL;

	return bRet;
}
//]]
BOOL CNVEditorApp::WriteConsole(
    HANDLE hConsoleOutput,
    CONST VOID *lpBuffer,
    DWORD nNumberOfCharsToWrite,
    LPDWORD lpNumberOfCharsWritten,
    LPVOID lpReserved
    )
{
	if(m_pErrorFile != NULL)
	{
#if defined(UNICODE) || defined(_UNICODE)
		int nSize = WideCharToMultiByte(CP_ACP,0,(LPCTSTR)lpBuffer,nNumberOfCharsToWrite,NULL,0,NULL,NULL);
		char * pBuf = new char[nSize+1];
		WideCharToMultiByte(CP_ACP,0,(LPCTSTR)lpBuffer,nNumberOfCharsToWrite,pBuf,nSize+1,NULL,NULL);
		fwrite(pBuf,nSize,1,m_pErrorFile);
		delete [] pBuf;
#else	 
	 fwrite(lpBuffer,nNumberOfCharsToWrite,1,m_pErrorFile);
#endif
	}

	return ::WriteConsole(hConsoleOutput,lpBuffer,nNumberOfCharsToWrite,lpNumberOfCharsWritten,lpReserved);
}

int  CNVEditorApp::GetPermission()
{
	TCHAR szCfgFile[MAX_PATH] = {0};
	GetIniFilePath(szCfgFile);
	m_dwPermission = ::GetPrivateProfileInt(_T("Settings"),_T("Super"),READ_PERMISSION,szCfgFile);
	return m_dwPermission;
}
BOOL CNVEditorApp::GetIniFilePath(TCHAR* pName)
{
    if(pName == NULL)
        return FALSE;
    
    DWORD dwRet = ::GetModuleFileName(AfxGetApp()->m_hInstance,pName,_MAX_PATH);
    if(dwRet == 0)
    {
        pName[0] = 0;
        return FALSE;
    }
    
    TCHAR* pResult = _tcsrchr(pName,'.');
    if(pResult == NULL)
    {
        pName[0] = 0;
        return FALSE;
    }
    
    *pResult = 0;
    _tcscat(pName,_T(".ini"));
    
    return TRUE;    
}