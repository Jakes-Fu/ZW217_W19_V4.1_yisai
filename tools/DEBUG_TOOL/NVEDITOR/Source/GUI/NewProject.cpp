// NewProject.cpp : implementation file
//

#include "stdafx.h"
#include "nveditor.h"
#include "NewProject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CNVEditorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CNewProject dialog


CNewProject::CNewProject(CWnd* pParent /*=NULL*/)
	: CDialog(CNewProject::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewProject)
	m_szPrjFileName = _T("C:\\");
	m_szPrjDesc = _T("");
	m_szPrjName = _T("");
	m_szTargetFileName = _T("C:\\");
	//}}AFX_DATA_INIT
}


void CNewProject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewProject)
	DDX_Control(pDX, IDC_PROJECT_NAME, m_ctlPrjName);
	DDX_Control(pDX, IDC_TARGET_FILE_NAME, m_ctlTargetFileName);
	DDX_Control(pDX, IDC_FILE_NAME, m_ctlPrjFileName);
	DDX_Text(pDX, IDC_FILE_NAME, m_szPrjFileName);
	DDX_Text(pDX, IDC_PROJECT_DESCRIPTION, m_szPrjDesc);
	DDX_Text(pDX, IDC_PROJECT_NAME, m_szPrjName);
	DDX_Text(pDX, IDC_TARGET_FILE_NAME, m_szTargetFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewProject, CDialog)
	//{{AFX_MSG_MAP(CNewProject)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_FOLDER, OnButtonBrowseFolder)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_FOLDER2, OnButtonBrowseFolder2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewProject message handlers
void CNewProject::OnOK() 
{
	// TODO: Add extra validation here
    UpdateData();
    
    if(m_szPrjFileName.GetLength () > (int)strlen("C:\\"))
    {
        /*xml file support by xmzhang modify begin*/
        if( !theApp.m_bNvmFormat)
        {
            m_szPrjFileName = m_szPrjFileName +  "\\" + m_szPrjName + ".xprj";
        }
        else
        {
            m_szPrjFileName = m_szPrjFileName +  "\\" + m_szPrjName + ".prj";
        }     
        /*xml file support by xmzhang modify end*/
    }
    else
    {
        /*xml file support by xmzhang modify begin*/
        if( !theApp.m_bNvmFormat )
        {
            m_szPrjFileName = m_szPrjFileName + m_szPrjName + ".xprj";
        }
        else
        {
            m_szPrjFileName = m_szPrjFileName + m_szPrjName + ".prj";
        }       
        /*xml file support by xmzhang modify end*/
    }

    if(m_szTargetFileName.GetLength () > (int)strlen("C:\\"))
    {
	    m_szTargetFileName = m_szTargetFileName +  "\\" + m_szPrjName + ".bin";
    }
    else
    {
        m_szTargetFileName = m_szTargetFileName + m_szPrjName + ".bin";
    }

    UpdateData(FALSE);

    CDialog::OnOK();
}

extern BOOL BrowForFolder(HWND hwdnOwner,TCHAR* szPath);

void CNewProject::OnButtonBrowseFolder() 
{
    TCHAR szPath[MAX_PATH];
    
    if(::BrowForFolder(GetSafeHwnd(),szPath))
    {
        UpdateData(TRUE);
        m_szPrjFileName = szPath;
        UpdateData(FALSE);
    }
}

void CNewProject::OnButtonBrowseFolder2() 
{
    TCHAR szPath[MAX_PATH];
    
    if(::BrowForFolder(GetSafeHwnd(),szPath))
    {
        UpdateData(TRUE);
        m_szTargetFileName = szPath;
        UpdateData(FALSE);
    }
}
