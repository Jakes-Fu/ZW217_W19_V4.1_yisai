// ImageConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MSDev.h"
#include "ImageConfigDlg.h"
#include "MSDevDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageConfigDlg dialog


CImageConfigDlg::CImageConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImageConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImageConfigDlg)
	m_strIniPath = _T("");
	m_strBmpPath = _T("");
    m_strBinPath = _T("");
	//}}AFX_DATA_INIT
}


void CImageConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageConfigDlg)
	DDX_Text(pDX, IDC_INI_FILE, m_strIniPath);
	DDX_Text(pDX, IDC_BMP_FILE, m_strBmpPath);
	DDX_Text(pDX, IDC_RES_FILE, m_strBinPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImageConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CImageConfigDlg)
	ON_BN_CLICKED(IDC_OPEN_BMP_FILE, OnOpenBmpFile)
	ON_BN_CLICKED(IDC_OPEN_INI_FILE, OnOpenIniFile)
	ON_BN_CLICKED(IDC_OPEN_RES_FILE, OnOpenBinFile)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageConfigDlg message handlers

BOOL CImageConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    CMSDevDlg *pWnd = (CMSDevDlg *)AfxGetMainWnd();
	if (pWnd->m_bStarted)
	{
		((CEdit*)GetDlgItem(IDC_RES_FILE))->SetReadOnly();	
		GetDlgItem(IDC_OPEN_RES_FILE)->EnableWindow(FALSE);
	}
    
    ((CMSDevApp *)AfxGetApp())->GetRelativePath(m_strBmpPath, m_strIniPath,m_strBinPath);

    UpdateData(FALSE);
	return TRUE;  
}

BOOL CImageConfigDlg::PreTranslateMessage(MSG* pMsg) 
{

    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
        return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CImageConfigDlg::OnOpenBmpFile() 
{
    static char BASED_CODE szFilter[] = "Image Files (*.png)|*.png||";
    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter, NULL);
    
    if (dlg.DoModal() == IDOK)
    {
        m_strBmpPath = dlg.GetPathName();
		
		int nFind = m_strBmpPath.ReverseFind(_T('.'));
		if(nFind != -1)
		{
			m_strIniPath = m_strBmpPath;
			m_strIniPath = m_strIniPath.Left(nFind);
			m_strIniPath += _T(".ini");
		}
        UpdateData(FALSE);
    }
}

void CImageConfigDlg::OnOpenIniFile() 
{
    static char BASED_CODE szFilter[] = "Config Files (*.ini)|*.ini||";
    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter, NULL);
    
    if (dlg.DoModal() == IDOK)
    {
        m_strIniPath = dlg.GetPathName();
        UpdateData(FALSE);
    }    
}

void CImageConfigDlg::OnOpenBinFile() 
{
    static char BASED_CODE szFilter[] = "MS resouce Files (*.bin)|*.bin||";
    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter, NULL);
    
    if (dlg.DoModal() == IDOK)
    {
        m_strBinPath = dlg.GetPathName();
        UpdateData(FALSE);
    }    
}

void CImageConfigDlg::OnOK() 
{

	CFileFind finder;
	if(finder.FindFile(m_strBmpPath) &&
		finder.FindFile(m_strIniPath) &&
		finder.FindFile(m_strBinPath))
	{
		RefreshHandsetImage();
	}
	else
	{
		AfxMessageBox(_T("Please input existed file!"));
		return;
	}

	CDialog::OnOK();
}

void CImageConfigDlg::OnApply() 
{
	CFileFind finder;
	if(finder.FindFile(m_strBmpPath) &&
		finder.FindFile(m_strIniPath) &&
		finder.FindFile(m_strBinPath))
	{
		RefreshHandsetImage();
	}
	else
	{
		AfxMessageBox(_T("Please input existed file!"));
		return;
	}
}

void CImageConfigDlg::RefreshHandsetImage()
{
	UpdateData(TRUE);
    //Save the new file path
    ((CMSDevApp *)AfxGetApp())->SetImageFilesPath(m_strBmpPath, m_strIniPath,m_strBinPath);
	
    //
    // Refresh the handset image
    //
    CMSDevDlg *pWnd = (CMSDevDlg *)AfxGetMainWnd();
	//pWnd->ShowWindow(SW_HIDE);
	
    pWnd->ChangeHandsetImage(m_strBmpPath, m_strIniPath,m_strBinPath);
    
    //pWnd->ShowWindow(SW_SHOW);
}
