// MSDev.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MSDev.h"
#include "MSDevDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const char MSSIM_CFG_FILE[]      = "MSSimCfg.ini";

static const char SECTION_FILE[]        = "FILE";
static const char FILE_ENTRY_BMPPATH[]  = "bmp_path";
static const char FILE_ENTRY_INIPATH[]  = "ini_path";
static const char FILE_ENTRY_BINPATH[]  = "bin_path";

static const char SECTION_WNDPOS[]      = "WINDOW_POS";
static const char WNDPOS_ENTRY_LEFT[]   = "left";
static const char WNDPOS_ENTRY_TOP[]    = "top";
static const char WNDPOS_ENTRY_RIGHT[]  = "right";
static const char WNDPOS_ENTRY_BOTTOM[] = "bottom";

static const char SECTION_SOCKET[] = "SOCKET";
static const char SOCKET_PORT[] = "port";

static const char SECTION_TRACE[] = "TRACE";
static const char TRACE_STATE[] = "state";

/////////////////////////////////////////////////////////////////////////////
// CMSDevApp

BEGIN_MESSAGE_MAP(CMSDevApp, CWinApp)
	//{{AFX_MSG_MAP(CMSDevApp)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMSDevApp construction

CMSDevApp::CMSDevApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bAutoStart = FALSE;
	m_pGdiToken = NULL;
	m_nPort = 0;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMSDevApp object

CMSDevApp g_theApp;

/////////////////////////////////////////////////////////////////////////////
// CMSDevApp initialization

BOOL CMSDevApp::InitInstance()
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

	GdiplusStartupInput m_gdiplusStartupInput;
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);

	for (int i = 1; i < __argc; i++)
	{
		LPCTSTR pszParam = __targv[i];
		if (pszParam[0] == '-' || pszParam[0] == '/')
		{
			++pszParam;
			if( stricmp( pszParam, "start" ) == 0 )
			{
				m_bAutoStart = TRUE;
			}
			else if(stricmp( pszParam, "port" ) == 0 && (i+1) < __argc)
			{
				pszParam = __targv[i+1];
				sscanf(pszParam,"%d",&m_nPort);
				i++;
			}
		}
	}

	CMSDevDlg dlg;
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

CString CMSDevApp::GetCurrentPathName()
{
    // Get the .full path of the .exe file
    char szFileName[MAX_PATH];
    GetModuleFileName(AfxGetInstanceHandle(), szFileName, MAX_PATH);

    //
    // Get the current directory path
    //
    int nIndex;
    CString strPathName;

    strPathName= szFileName;
    nIndex = strPathName.ReverseFind('\\');
    strPathName = strPathName.Left(nIndex + 1);

    return strPathName;
}

//get relative path from MSSimCfg.ini
void CMSDevApp::GetRelativePath(CString &strBmpPath, CString &strIniPath, CString &strBinPath)
{
    char szBmpPath[MAX_PATH];
    char szIniPath[MAX_PATH];
    char szBinPath[MAX_PATH];

    CString strCurrentPath = GetCurrentPathName();
    CString strCfgFilePath = strCurrentPath + MSSIM_CFG_FILE;
    CString strDefBmpPath  = "";
    CString strDefIniPath  = "";
    CString strDefBinPath  = "";

    GetPrivateProfileString(SECTION_FILE,
                            FILE_ENTRY_BMPPATH,
                            strDefBmpPath,
                            szBmpPath,
                            MAX_PATH,
                            strCfgFilePath);

    GetPrivateProfileString(SECTION_FILE,
                            FILE_ENTRY_INIPATH,
                            strDefIniPath,
                            szIniPath,
                            MAX_PATH,
                            strCfgFilePath);
    GetPrivateProfileString(SECTION_FILE,
                            FILE_ENTRY_BINPATH,
                            strDefBinPath,
                            szBinPath,
                            MAX_PATH,
                            strCfgFilePath);

	strBmpPath = szBmpPath;
	strIniPath = szIniPath;
	strBinPath = szBinPath;

}

//get path from MSSimCfg.ini and convert relative path to full path 
void CMSDevApp::GetImageFilesPath(CString &strBmpPath,CString &strIniPath,CString &strBinPath)
{
    CString BmpPath;
    CString IniPath;
    CString BinPath;

	
	//get relative path from MSSimCfg.ini
	GetRelativePath(BmpPath, IniPath, BinPath);
	
	//set current directory 
	CString strCurPath=	GetCurrentPathName();
	SetCurrentDirectory(strCurPath);		

	//convert relative path to full path 
	if (NULL != _fullpath(strBmpPath.GetBuffer(MAX_PATH), BmpPath,MAX_PATH))
	{
		strBmpPath.ReleaseBuffer();		
	}
	else
	{
		strBmpPath = BmpPath;
	}

	if (NULL != _fullpath(strIniPath.GetBuffer(MAX_PATH), IniPath,MAX_PATH))
	{
		strIniPath.ReleaseBuffer();		
	}
	else
	{
		strIniPath = IniPath;
	}

	if (NULL != _fullpath(strBinPath.GetBuffer(MAX_PATH), BinPath,MAX_PATH))
	{
		strBinPath.ReleaseBuffer();		
	}
	else
	{
		strBinPath = BinPath;
	}

}

void CMSDevApp::SetImageFilesPath(CString strBmpPath, CString strIniPath,CString strBinPath)
{
    CString strCurrentPath = GetCurrentPathName();
    CString strCfgFilePath = strCurrentPath + MSSIM_CFG_FILE;

	if (strBmpPath != "")
	{
		WritePrivateProfileString(SECTION_FILE, FILE_ENTRY_BMPPATH, strBmpPath, strCfgFilePath);
	}
	if (strIniPath != "")
	{
		WritePrivateProfileString(SECTION_FILE, FILE_ENTRY_INIPATH, strIniPath, strCfgFilePath);
	}
	if (strBinPath != "")
	{
		WritePrivateProfileString(SECTION_FILE, FILE_ENTRY_BINPATH, strBinPath, strCfgFilePath);
	}
	
}

void CMSDevApp::GetWindowPos(CRect& rect)
{
    CString strCurrentPath = GetCurrentPathName();
    CString strCfgFilePath = strCurrentPath + MSSIM_CFG_FILE;

    rect.left   = GetPrivateProfileInt(SECTION_WNDPOS, WNDPOS_ENTRY_LEFT, 0, strCfgFilePath);
    rect.top    = GetPrivateProfileInt(SECTION_WNDPOS, WNDPOS_ENTRY_TOP, 0, strCfgFilePath);
    rect.right  = GetPrivateProfileInt(SECTION_WNDPOS, WNDPOS_ENTRY_RIGHT, 0, strCfgFilePath);
    rect.bottom = GetPrivateProfileInt(SECTION_WNDPOS, WNDPOS_ENTRY_BOTTOM, 0, strCfgFilePath);
}

void CMSDevApp::SaveWindowPos(CRect rect)
{
    CString strLeft, strTop, strRight, strBottom;

    CString strCurrentPath = GetCurrentPathName();
    CString strCfgFilePath = strCurrentPath + MSSIM_CFG_FILE;

    strLeft.Format("%d", rect.left);
    strTop.Format("%d", rect.top);
    strRight.Format("%d", rect.right);
    strBottom.Format("%d", rect.bottom);

    WritePrivateProfileString(SECTION_WNDPOS, WNDPOS_ENTRY_LEFT, strLeft, strCfgFilePath);
    WritePrivateProfileString(SECTION_WNDPOS, WNDPOS_ENTRY_TOP,  strTop, strCfgFilePath);
    WritePrivateProfileString(SECTION_WNDPOS, WNDPOS_ENTRY_RIGHT, strRight, strCfgFilePath);
    WritePrivateProfileString(SECTION_WNDPOS, WNDPOS_ENTRY_BOTTOM, strBottom, strCfgFilePath);
}

int CMSDevApp::ExitInstance() 
{
	GdiplusShutdown(m_pGdiToken);
	return CWinApp::ExitInstance();
}

void CMSDevApp::SaveSocketPort(DWORD dwPort)
{
	CString strCurrentPath = GetCurrentPathName();
    CString strCfgFilePath = strCurrentPath + MSSIM_CFG_FILE;

	CString strPort;
	strPort.Format("%d",dwPort);

    WritePrivateProfileString(SECTION_SOCKET, SOCKET_PORT, strPort, strCfgFilePath);
}

void CMSDevApp::SaveTraceState(BOOL bOn)
{
	CString strCurrentPath = GetCurrentPathName();
    CString strCfgFilePath = strCurrentPath + MSSIM_CFG_FILE;
	
	CString strState;
	strState.Format("%d",bOn);
	
    WritePrivateProfileString(SECTION_TRACE, TRACE_STATE, strState, strCfgFilePath);

}

BOOL CMSDevApp::GetTraceState()
{
	CString strCurrentPath = GetCurrentPathName();
    CString strCfgFilePath = strCurrentPath + MSSIM_CFG_FILE;
	
	int nState = GetPrivateProfileInt(SECTION_TRACE, TRACE_STATE, TRUE, strCfgFilePath);
	return (nState == 0) ? FALSE:TRUE;
}

