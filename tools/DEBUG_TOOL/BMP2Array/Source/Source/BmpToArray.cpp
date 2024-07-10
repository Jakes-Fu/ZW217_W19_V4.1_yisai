// BmpToArray.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "BmpToArray.h"

#include "MainFrm.h"
#include "BmpToArrayDoc.h"
#include "BmpToArrayView.h"
#include "VerInfo.h"
#include "Dlg3D.h"
#include "se_table_gen.h"
#include "Generater.h"
#include "XAboutDlg.h"
#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CMyCmdLine : public CCommandLineInfo
{
public:
	CMyCmdLine() 
	{ 
		m_nFuncType = -1;

		m_nB2AType = -1;
		m_strSrcPath = _T("");		
	    m_strDstPath = _T("");
		m_bBigEndian = TRUE;	    

		m_uSWidth = 0;
		m_uSHeight = 0;
		m_uDWidth = 0;
		m_uDheight = 0;
		m_strOutputPath = _T("");	
	
		m_nParaCount = 0; 
		m_nMaxParaCount = 6; 
	}
	virtual void ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast );

	int		m_nFuncType;
//  B2A
	int		m_nB2AType;
	CString m_strSrcPath;
	CString m_strDstPath;	
	BOOL    m_bBigEndian;

//  3DTable
	UINT	m_uSWidth;
	UINT	m_uSHeight;
	UINT	m_uDWidth;
	UINT	m_uDheight;
	CString m_strOutputPath;

protected:
	int		m_nParaCount;
	int		m_nMaxParaCount;
};



void CMyCmdLine::ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast )
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
		if(strParam.CompareNoCase(_T("B2A")) == 0)
		{
			m_nFuncType = FT_B2A;
		}
		else if(strParam.CompareNoCase(_T("3dtable")) == 0)
		{
			m_nFuncType = FT_3DTABLE;
		}
		else
		{
			m_nFuncType = -1;
		}
	}
	else if( 1 == m_nParaCount )
	{
		if(m_nFuncType == FT_B2A && bFlag)
		{
			if(strParam.CompareNoCase(_T("b")) == 0)
			{
				m_nB2AType = BMP_FILE;
			}
			else if(strParam.CompareNoCase(_T("f")) == 0)
			{
				m_nB2AType = BMP_FOLDER;
			}
			else if(strParam.CompareNoCase(_T("a")) == 0)
			{
				m_nB2AType = ANIM_FOLDER;
			}
			else if(strParam.CompareNoCase(_T("j")) == 0)
			{
				m_nB2AType = JPG_FILE;
			}
			else if(strParam.CompareNoCase(_T("jf")) == 0)
			{
				m_nB2AType = JPG_FOLDER;
			}
			else if(strParam.CompareNoCase(_T("g")) == 0)
			{
				m_nB2AType = GIF_FILE;
			}
			else if(strParam.CompareNoCase(_T("gf")) == 0)
			{
				m_nB2AType = GIF_FOLDER;
			}
			else if(strParam.CompareNoCase(_T("p")) == 0)
			{
				m_nB2AType = PNG_FILE;
			}
			else if(strParam.CompareNoCase(_T("pf")) == 0)
			{
				m_nB2AType = PNG_FOLDER;
			}
		}
		else if(m_nFuncType == FT_3DTABLE && bFlag)
		{
			m_uSWidth = _ttoi(strParam);
		}
	}
	else if( 2 == m_nParaCount )
	{
		if(m_nFuncType == FT_B2A)
		{
			m_strSrcPath = strParam;
		}
		else if(m_nFuncType == FT_3DTABLE && bFlag)
		{
			m_uSHeight = _ttoi(strParam);
		}
	}
	else if( 3 == m_nParaCount )
	{
		if(m_nFuncType == FT_B2A)
		{
			m_strDstPath = strParam;
		}
		else if(m_nFuncType == FT_3DTABLE && bFlag )
		{
			m_uDWidth = _ttoi(strParam);
		}
	}
	else if( 4 == m_nParaCount )
	{
		if(m_nFuncType == FT_B2A && bFlag)
		{
			if(strParam.CompareNoCase(_T("l")) == 0)
			{
				m_bBigEndian = FALSE;
			}
		}
		else if(m_nFuncType == FT_3DTABLE && bFlag )
		{
			m_uDheight= _ttoi(strParam);
		}
	}
	else if( 5 == m_nParaCount )
	{
		if(m_nFuncType == FT_3DTABLE )
		{
			m_strOutputPath= strParam;
		}
		else if(m_nFuncType == FT_B2A && bFlag)
		{			
			if(strParam.CompareNoCase(_T("l")) == 0)
			{
				m_bBigEndian = FALSE;
			}		
		}
	}

	m_nParaCount++;
}

/////////////////////////////////////////////////////////////////////////////
// CBmpToArrayApp

BEGIN_MESSAGE_MAP(CBmpToArrayApp, CWinApp)
	//{{AFX_MSG_MAP(CBmpToArrayApp)
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
// CBmpToArrayApp construction

CBmpToArrayApp::CBmpToArrayApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bCmd = FALSE;
	m_bShowOSD = FALSE;
	m_bGenBinFile = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBmpToArrayApp object

CBmpToArrayApp g_theApp;

/////////////////////////////////////////////////////////////////////////////
// CBmpToArrayApp initialization

BOOL CBmpToArrayApp::InitInstance()
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
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	
	{
		_TCHAR szPathName[_MAX_PATH];
		::GetModuleFileName(NULL, szPathName, _MAX_PATH);
		
		LPTSTR pFind = _tcsrchr(szPathName, _T('.'));
		_tcscpy( pFind + 1, _T("ini") );

		m_bShowOSD = GetPrivateProfileInt(_T("Setting"),_T("ShowOSD"),FALSE,szPathName);
		m_bGenBinFile= GetPrivateProfileInt(_T("Setting"),_T("GenBinfile"),FALSE,szPathName);
		GetPrivateProfileString(_T("Setting"),_T("BinSuffixed"),_T(".bin"),m_szBinSuffixed,MAX_PATH, szPathName);
	}


	//teana hu 2009.06.29
	CString strCmdLine = m_lpCmdLine;

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CBmpToArrayDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CBmpToArrayView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CMyCmdLine cmdInfo;	
	ParseCommandLine(cmdInfo);

	if(!strCmdLine.IsEmpty())
	{	
		m_bCmd = TRUE;
		if( cmdInfo.m_nFuncType == FT_3DTABLE && 
			!cmdInfo.m_strOutputPath.IsEmpty() )
		{
			BOOL bRet = Gen3DtableFile(cmdInfo.m_uSWidth, cmdInfo.m_uSHeight, 
									   cmdInfo.m_uDWidth, cmdInfo.m_uDheight, 
									   cmdInfo.m_strOutputPath);
			if(bRet)
			{
				_tprintf(_T("\nSuccessfully! Generate c file at: %s\\ui_special_effect_table.c.\n"), cmdInfo.m_strOutputPath.operator LPCTSTR());
			}
			else
			{
				_tprintf(_T("\nFailed to generate c file!\n"));
			}
		}
		else if( cmdInfo.m_nFuncType == FT_B2A && 
				 cmdInfo.m_nB2AType != -1 && 
			    !cmdInfo.m_strSrcPath.IsEmpty() &&
				!cmdInfo.m_strDstPath.IsEmpty())
		{
			BOOL bRet = GenBmpArray(cmdInfo.m_strSrcPath,cmdInfo.m_strDstPath,
									cmdInfo.m_nB2AType, cmdInfo.m_bBigEndian);
			if(bRet)
			{
				_tprintf(_T("\nGenerate succss.\n"));
			}
			else
			{
				_tprintf(_T("\nGenerate fail.\n"));
			}
		}
		else
		{
			_tprintf(_T("Usage:\n"));
			_tprintf(_T("\tBTACmd 3dtable [Slide Width] [Slide Height] [Desktop Width] [Desktop Height] [Output Path]\n"));
			_tprintf(_T("\tBTACmd b2a  <-b> <bmp file path> <array file path> [-l]\n"));
			_tprintf(_T("\tBTACmd b2a  <-f> <bmp folder path> <array file path> [-l]\n"));
			_tprintf(_T("\tBTACmd b2a  <-a> <anim folder path> <array file path> [-l]\n"));
			_tprintf(_T("\tBTACmd b2a  <-j> <jpg file path> <array file path> [-l]\n"));
			_tprintf(_T("\tBTACmd b2a  <-jf> <jpg folder path> <array file path> [-l]\n"));
			_tprintf(_T("\tBTACmd b2a  <-g> <png file path> <array file path> [-l]\n"));
			_tprintf(_T("\tBTACmd b2a  <-gf> <png folder path> <array file path> [-l]\n"));
			_tprintf(_T("\tBTACmd b2a  <-p> <png file path> <array file path> [-l]\n"));
			_tprintf(_T("\tBTACmd b2a  <-pf> <png folder path> <array file path> [-l]\n"));
			//_tprintf(_T("\ttype: its value can be one of SCI_BMP565,SCI_JPG,SCI_GIF,SCI_PNG\n"));
			
		}

		return FALSE; // Exit program
	}

	m_bCmd = FALSE;


//	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

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
void CBmpToArrayApp::OnAppAbout()
{
	CXAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CBmpToArrayApp message handlers


BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
/*	CVerInfo ver;
    if( ver.Init() )
    {		
		CWnd * pWnd = GetDlgItem(IDC_STC_VER);
		_ASSERTE( pWnd != NULL );
		CString strVer = ver.GetProductVersion();
		strVer.Remove(_T(' '));
		strVer.Replace( _T(","),_T(".") );	
		
		int nLastDot = strVer.ReverseFind(_T('.')); 
		CString strBuild = strVer.Right(strVer.GetLength() - nLastDot-1);		
		strVer = strVer.Left(nLastDot);
		
		pWnd->SetWindowText(strVer);
		
		int nBuild = _ttoi(strBuild);
		strBuild.Format(_T("%03d"),nBuild);
		
		pWnd = GetDlgItem(IDC_STC_BUILD);
		_ASSERTE( pWnd != NULL );
		pWnd->SetWindowText(strBuild);
		
		pWnd = GetDlgItem(IDC_STC_COPYRIGHT);
		_ASSERTE( pWnd != NULL );
		pWnd->SetWindowText(ver.GetLegalCopyright());
    }*/
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CBmpToArrayApp::SelectFolder(HWND hWnd, LPCTSTR lpcszTitle)
{
    ::CoInitialize(NULL);
	
    _TCHAR szBuffer[_MAX_PATH];
    _tcscpy(szBuffer, _T(""));
	
    LPMALLOC lpMalloc = NULL;
	
    if(::SHGetMalloc(&lpMalloc) == E_FAIL)
    {
        return _T("");
    }
	
    BROWSEINFO browseInfo;
	
    browseInfo.hwndOwner      = hWnd;
    browseInfo.pidlRoot       = NULL; 
    browseInfo.pszDisplayName = NULL;
    browseInfo.lpszTitle      = lpcszTitle;   
    browseInfo.ulFlags        = BIF_RETURNONLYFSDIRS;      
    browseInfo.lpfn           = NULL;       // not used
    browseInfo.lParam         = 0;    
	
    LPITEMIDLIST lpItemIDList = NULL;
    if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo)) != NULL)
    {
        ::SHGetPathFromIDList(lpItemIDList, szBuffer);
		
        lpMalloc->Free(lpItemIDList);
        lpMalloc->Release();      
    }
    
    ::CoUninitialize();
	
    return szBuffer;
}

CString CBmpToArrayApp::SelectFile(BOOL bOpen, LPCTSTR pExtension)
{
    _ASSERTE(pExtension != NULL);
	
    static _TCHAR szFormat[] = _T("Image Files (*.%s)|*.%s|All Files (*.*)|*.*||");
    static _TCHAR szFilter[_MAX_PATH] = { 0 };
	
    _stprintf(szFilter, szFormat, pExtension, pExtension);
	
    CFileDialog fd(bOpen, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	
    if( fd.DoModal() == IDOK )
    {
        CString strName = fd.GetPathName();
        CString strExt  = fd.GetFileExt();
        if( strExt.IsEmpty() )
        {
            strName += _T(".");
            strName += pExtension;
        }
        
        return strName;
    }
    else
    {
        return _T("");
    }
}

int CBmpToArrayApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	DeleteFile(SE_SLIDE_MOVE_FILE);
	DeleteFile(SE_DISTORT_MOVE_FILE);
	DeleteFile(SE_DESKTOP_ROTATE_FILE);
	DeleteFile(SE_MICORS);
	
	return CWinApp::ExitInstance();
}

BOOL CBmpToArrayApp::GenBmpArray(LPCTSTR lpszSrcPath, LPCTSTR lpszDstPath,int nType,BOOL bBigEndian/*=TRUE*/)
{
	if(lpszDstPath == NULL || lpszSrcPath == NULL)
		return FALSE;

	CString strSrcPath = lpszSrcPath;
	CString strDstPath = lpszDstPath;
	if(strSrcPath.IsEmpty() || strDstPath.IsEmpty())
		return FALSE;

	CFileFind finder;
	if(!finder.FindFile(strSrcPath))
	{
		_tprintf(_T("\nsource path [%s] not exist.\n"),strSrcPath);
		return FALSE;
	}
	if(finder.FindFile(strDstPath))
	{
		DWORD dwAtt = ::GetFileAttributes(strDstPath);
		if(dwAtt != (DWORD)(-1))
		{
			dwAtt &= ~FILE_ATTRIBUTE_READONLY;
			::SetFileAttributes(strDstPath,dwAtt);
		}
	}

	uint8 * pBmpContent      = NULL;
    uint32  nBmpContentSize  = 0;
    uint8 * pAnimContent     = NULL;
    uint32  nAnimContentSize = 0;

    CString strFileName;
    BOOL bOK  = FALSE;

    CGenerater generater(bBigEndian);

	int nCmpType = BMP565_RLE;

    if( nType==BMP_FILE || nType == JPG_FILE || nType == GIF_FILE || nType == PNG_FILE)
    {
		switch(nType)
		{
		case BMP_FILE:
			nCmpType = BMP565_RLE;
			break;
		case JPG_FILE:
			nCmpType = SJPG;
			break;
		case GIF_FILE:
			nCmpType = SGIF;
		    break;
		case PNG_FILE:
			nCmpType = SPNG;
		    break;
		default:
		    break;
		}
        bOK = generater.LoadImage(strSrcPath, &pBmpContent, &nBmpContentSize,nCmpType);
        if( bOK )
		{
			if(generater.BeginGenerate( strDstPath ))
			{
				bOK = generater.Generate(DistillFileName(strSrcPath), pBmpContent, nBmpContentSize);
				generater.EndGenerate();
			}
			if (g_theApp.m_bGenBinFile) //gen bin file
			{
				CString strBmpBinFileName;
				strBmpBinFileName = strDstPath.Left(strDstPath.ReverseFind(_T('.')));
				strBmpBinFileName += g_theApp.m_szBinSuffixed;
				generater.GenerateBinFile(strBmpBinFileName, pBmpContent, nBmpContentSize);
			}
		}   		
        generater.FreeImage(pBmpContent);
		pBmpContent = NULL;
		nBmpContentSize = 0;
		return bOK;
    }
	else if(nType == BMP_FOLDER || nType == JPG_FOLDER || nType == GIF_FOLDER  || nType == PNG_FOLDER)
	{		
		switch(nType)
		{
		case BMP_FOLDER:
			nCmpType = BMP565_RLE;
			break;
		case JPG_FOLDER:
			nCmpType = SJPG;
			break;
		case GIF_FOLDER:
			nCmpType = SGIF;
			break;
		case PNG_FOLDER:
			nCmpType = SPNG;
		    break;
		default:
		    break;
		}
		CStringArray arrFiles;
		EnumFiles(arrFiles, strSrcPath,nCmpType);
        
        if( generater.BeginGenerate( strDstPath ) )
        {
            CString strFile;
            for( int i = 0; i < arrFiles.GetSize(); ++i)
            {
                strFile = arrFiles[i];
                bOK = generater.LoadImage(strFile, &pBmpContent, &nBmpContentSize,nCmpType);
                if( bOK )
                {
                    generater.Generate(DistillFileName(strFile), pBmpContent, nBmpContentSize);

					if (g_theApp.m_bGenBinFile) //gen bin file
					{
						CString strDestBinFile;
						strDestBinFile = strDstPath.Left(strDstPath.ReverseFind(_T('\\')) +1) +\
							DistillFileName(strFile) + g_theApp.m_szBinSuffixed;
						generater.GenerateBinFile(strDestBinFile, pBmpContent, nBmpContentSize);
					}

                }
				else
					_tprintf(_T("\n[Error] LoadImage File failed:\n"));
                generater.FreeImage(pBmpContent);
            }

			generater.EndGenerate();
        }
        
        arrFiles.RemoveAll();
		pBmpContent = NULL;
		nBmpContentSize = 0;
		return bOK;
	}
	else if(nType == ANIM_FOLDER)
	{
		bOK = generater.LoadAnim(strSrcPath, &pAnimContent, &nAnimContentSize,0);
        if( bOK )
		{
			if( generater.BeginGenerate( strDstPath ) )
			{
				bOK = generater.Generate(_T("init_logo"), pAnimContent, nAnimContentSize);
				generater.EndGenerate();
			}	
			if (g_theApp.m_bGenBinFile) //gen bin file
			{
				CString strAnimBinFile;
				strAnimBinFile = strDstPath.Left(strDstPath.ReverseFind(_T('.')));
				strAnimBinFile += g_theApp.m_szBinSuffixed;
				generater.GenerateBinFile(strAnimBinFile, pAnimContent, nAnimContentSize);
			}

		}		
        generater.FreeAnim(pAnimContent);
		pAnimContent = NULL;
		nAnimContentSize= 0;
		return bOK;
	}
	else
	{
		return FALSE;
	}


}

void CBmpToArrayApp::XDelete3DFiles(LPCTSTR lpszPath)
{
	if(lpszPath== NULL)
		return;

	CFileFind finder;
	if(!finder.FindFile(lpszPath))
	{
		return;
	}
	_TCHAR szPath[_MAX_PATH] = {0};
	_TCHAR * pFiles[4] = { SE_MICORS,SE_SLIDE_MOVE_FILE,SE_DISTORT_MOVE_FILE,SE_DESKTOP_ROTATE_FILE};

	for(int i = 0; i< 4; i++)
	{
		_stprintf(szPath,_T("%s\\%s"),lpszPath,pFiles[i]);
		if(finder.FindFile(szPath))
		{
			DeleteFile(szPath);
		}
	}

	RemoveDirectory(lpszPath);
}


BOOL CBmpToArrayApp::Gen3DtableFile(UINT uSWidth, UINT uSHeight, UINT uDWidth, UINT uDHeight, CString strOutputPath)
{
	USES_CONVERSION;

	if(uSWidth == 0 || uSHeight == 0 || uDWidth == 0 || uDHeight == 0 || strOutputPath.IsEmpty())
	{
		return FALSE;
	}

	BOOL bOK = TRUE;
	
	_TCHAR szTmpPath[_MAX_PATH] = {0};
	_stprintf(szTmpPath,_T("%s\\gen_3dtable_tmp%d"),_tgetenv(_T("tmp")),GetTickCount());

	if(!CreateDirectory(szTmpPath,NULL))
	{
		if( GetLastError() != ERROR_ALREADY_EXISTS )
        {     
			_tprintf(_T("\nCan not create temp folder [%s], [error:0x%X].\n"),szTmpPath,GetLastError());
            return FALSE;
        }
	}

	//_tcscat(szTmpPath,_T("\\"));

	SetTempPath(W2A(szTmpPath));

	if(!CreateSlideMoveTable(uSWidth, uSHeight))
	{
		bOK = FALSE;
		_tprintf(_T("CreateSlideMoveTable failed.\n"));
	}

	if(!CreateDistortMoveTable(uSWidth, uSHeight))
	{
		bOK = FALSE;
		_tprintf(_T("CreateDistortMoveTable failed.\n"));
	}
	if(!CreateRotDesktopTable(uDWidth, uDHeight))
	{
		bOK = FALSE;
		_tprintf(_T("CreateRotDesktopTable failed.\n"));
	}
	if(!CreateMicros(uSWidth, uSHeight, uDWidth, uDHeight))
	{
		bOK = FALSE;
		_tprintf(_T("CreateMicros failed.\n"));
	}
	if(!bOK)
	{
		XDelete3DFiles(szTmpPath);
		return FALSE;
	}

	if(strOutputPath.Right(1) != _T("\\") && strOutputPath.Right(1) != _T("/"))
	{
		strOutputPath += _T("\\");
	}

	strOutputPath.Replace(_T("/"), _T("\\"));
	
	CString strOutput = strOutputPath + OUT_PUT_FILE;

	_TCHAR szDirectory[MAX_PATH];		
	GetModuleFileName(NULL, szDirectory, MAX_PATH);
	CString strCfile= "";
	strCfile += szDirectory;
	strCfile = strCfile.Left(strCfile.ReverseFind('\\') + 1);	
	strCfile += OUT_PUT_FILE;

	FILE *fpCfile = NULL;
	fpCfile = _tfopen(strCfile, _T("r"));
	if(fpCfile == NULL)
	{
		_tprintf(_T("open [%s] failed.\n"),strCfile);
		XDelete3DFiles(szTmpPath);
		return FALSE;
	}

	BOOL bSame = FALSE;
	if(strOutput == strCfile)
	{
		bSame = TRUE;
		strOutput += _T(".bak");
		//remove readonly
		CFileFind finder;
		if(finder.FindFile(strCfile))
		{
			DWORD dwAttr = ::GetFileAttributes(strCfile);
			dwAttr &= ~FILE_ATTRIBUTE_READONLY;
			::SetFileAttributes(strCfile, dwAttr);
		}
	}

	FILE *fpOutput = NULL;
	fpOutput = _tfopen(strOutput, _T("w"));
	if(fpOutput == NULL)
	{
		_tprintf(_T("create [%s] failed.\n"),strOutput);
		XDelete3DFiles(szTmpPath);
		return FALSE;
	}

	_TCHAR szPath[_MAX_PATH] = {0};
	_stprintf(szPath,_T("%s\\%s"),szTmpPath,SE_MICORS);

	FILE *fp_Icon = NULL;
	fp_Icon = _tfopen(szPath, _T("r"));
	if(fp_Icon == NULL)
	{
		_fcloseall();
		XDelete3DFiles(szTmpPath);
		_tprintf(_T("open [%s] failed.\n"),szPath);
		return FALSE;
	}

	_stprintf(szPath,_T("%s\\%s"),szTmpPath,SE_SLIDE_MOVE_FILE);

	FILE *fp_Slide = NULL;
	fp_Slide = _tfopen(szPath, _T("r"));
	if(fp_Slide == NULL)
	{
		_fcloseall();
		XDelete3DFiles(szTmpPath);
		_tprintf(_T("open [%s] failed.\n"),szPath);
		return FALSE;
	}

	_stprintf(szPath,_T("%s\\%s"),szTmpPath,SE_DISTORT_MOVE_FILE);

	FILE *fp_Distork = NULL;
	fp_Distork = _tfopen(szPath, _T("r"));
	if(fp_Distork == NULL)
	{
		_fcloseall();
		XDelete3DFiles(szTmpPath);
		_tprintf(_T("open [%s] failed.\n"),szPath);
		return FALSE;
	}

	_stprintf(szPath,_T("%s\\%s"),szTmpPath,SE_DESKTOP_ROTATE_FILE);

	FILE *fp_Desktop = NULL;
	fp_Desktop = _tfopen(szPath, _T("r"));
	if(fp_Desktop == NULL)
	{
		_fcloseall();
		XDelete3DFiles(szTmpPath);
		_tprintf(_T("open [%s] failed.\n"),szPath);
		return FALSE;
	}


	char szLine[4096] = {0};
	char szRead[4096] = {0};

	while(!feof(fpCfile))
	{
		fgets(szLine, 4096, fpCfile);
		fwrite(szLine, 1, strlen(szLine), fpOutput);
		fflush(fpOutput);
		if(strstr(szLine, ICON_FLAG) != NULL)
		{
			while(!feof(fp_Icon))
			{
				if(fgets(szRead, 4096, fp_Icon) != NULL)
				{
					fwrite(szRead, 1, strlen(szRead), fpOutput);
					memset(szRead, 0, 4096);
					fflush(fpOutput);
				}
			}
			fwrite("\n", 1, 1, fpOutput);
			fflush(fpOutput);
			continue;
		}
		else if(strstr(szLine, SLIDE_FLAG) != NULL)
		{
			while (!feof(fp_Slide))
			{
				if(fgets(szRead, 4096, fp_Slide) != NULL)
				{
					fwrite(szRead, 1, strlen(szRead), fpOutput);
					memset(szRead, 0, 4096);
					fflush(fpOutput);
				}
			}
			fwrite("\n", 1, 1, fpOutput);
			fflush(fpOutput);
			continue;
		}
		else if(strstr(szLine, DISTORT_FLAG) != NULL)
		{
			while (!feof(fp_Distork))
			{
				if(fgets(szRead, 4096, fp_Distork) != NULL)
				{
					fwrite(szRead, 1, strlen(szRead), fpOutput);
					memset(szRead, 0, 4096);
					fflush(fpOutput);
				}
			}
			fwrite("\n", 1, 1, fpOutput);
			fflush(fpOutput);
			continue;
			
		}
		else if(strstr(szLine, DESKTOP_FLAG) != NULL)
		{
			while (!feof(fp_Desktop))
			{
				if(fgets(szRead, 4096, fp_Desktop) != NULL)
				{
					fwrite(szRead, 1, strlen(szRead), fpOutput);
					memset(szRead, 0, 4096);
					fflush(fpOutput);
				}
			}
			fwrite("\n", 1, 1, fpOutput);
			fflush(fpOutput);
			continue;
		}

		memset(szLine, 0, 4096);
	}

	_fcloseall();
	fpCfile = NULL;
	fpOutput = NULL;
	fp_Icon = NULL;
	fp_Slide = NULL;
	fp_Distork = NULL;
	fp_Desktop = NULL;

	if(bSame)
	{
		CopyFile(strOutput, strCfile, FALSE);
		DeleteFile(strOutput);
	}

	XDelete3DFiles(szTmpPath);

	return TRUE;
}

void  CBmpToArrayApp::EnumFiles(CStringArray &arrFiles, LPCTSTR pDir,int nCmpType)
{
    _ASSERTE( pDir != NULL );
	
    arrFiles.RemoveAll();
	
    CString strDir  = pDir;
    CString strName = pDir;
	CString strName2 = _T("");
	if(nCmpType == SPNG)
	{
		strName += _T("\\*.png");
	}
	else if(nCmpType == SGIF)
	{
		strName += _T("\\*.gif");
	}
	else if(nCmpType == SJPG)
	{
		strName += _T("\\*.jpg");
		strName2 = pDir;
		strName2 +=  _T("\\*.jpeg");
	}
	else if(nCmpType == BMP565_RLE)
	{
		strName += _T("\\*.bmp");
	}
	else
	{
		return;
	}
	
    WIN32_FIND_DATA findData;
    HANDLE hFile = ::FindFirstFile(strName, &findData);
    if( hFile != INVALID_HANDLE_VALUE )
    {        
        do
        {
            strName = findData.cFileName;
            strName.MakeLower();
            if( (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 )
            {
                arrFiles.Add( strDir + _T("\\") + strName);
            }
        }while( ::FindNextFile(hFile, &findData) );
		
		::FindClose(hFile);
    }
	
	if(!strName2.IsEmpty())
	{
		hFile = ::FindFirstFile(strName, &findData);
		if( hFile != INVALID_HANDLE_VALUE )
		{
			//::FindNextFile(hFile, &findData);
			while( ::FindNextFile(hFile, &findData) )
			{
				strName = findData.cFileName;
				strName.MakeLower();
				if( (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 )
				{
					arrFiles.Add( strDir + _T("\\") + strName);
				}
			}
			::FindClose(hFile);
		}
	}
	
}

CString CBmpToArrayApp::DistillFileName(LPCTSTR pPath)
{
    _ASSERTE( pPath != NULL );
	
    CString strRet;
    LPTSTR pFind = _tcsrchr(pPath, _T('\\'));
    if( pFind != NULL )
    {
        strRet = pFind + 1;
        int nFind = strRet.ReverseFind(_T('.'));
        if( nFind != -1 )
        {
            strRet = strRet.Left(nFind);
        }
    }
    else
    {
		strRet = pPath;
		int nFind = strRet.ReverseFind(_T('.'));
		if( nFind != -1 )
        {
            strRet = strRet.Left(nFind);
        }       
    }

	if(strRet.IsEmpty())
	{
		 strRet = _T("ArrayName");
	}
	TCHAR c = strRet.GetAt(0);
	if( c > 'z' || c < 'a')
	{
		if( c > 'Z' || c < 'A')
		{
			if( c != '_')
			{
				strRet.Insert(0,'_');
			}
		}
	}
    
    return strRet;
}