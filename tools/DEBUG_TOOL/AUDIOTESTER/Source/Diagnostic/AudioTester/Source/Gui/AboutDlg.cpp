// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog


CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strBuild = _T("");
	m_strVersion = _T("");
	//}}AFX_DATA_INIT
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_STATIC_BUILD, m_strBuild);
	DDX_Text(pDX, IDC_STATIC_VERSION, m_strVersion);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    GetVersion();

    UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::GetVersion()
{
    TCHAR szFileName[_MAX_PATH];
    ::GetModuleFileName(AfxGetApp()->m_hInstance,szFileName,_MAX_PATH);
    
    DWORD dwSize,dwHandle;
    dwSize = ::GetFileVersionInfoSize(szFileName,&dwHandle);	
    if( dwSize == 0 )
    {
        return;
    }
    
    char* pVer = new char[dwSize];
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
        m_strVersion.Replace( _T(","),_T(".") );
        m_strVersion.Replace( _T(" "),_T("") );
        int nIndex = m_strVersion.ReverseFind( '.' );
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
