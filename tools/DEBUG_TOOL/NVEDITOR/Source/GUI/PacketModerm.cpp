// PacketModerm.cpp : implementation file
//

#include "stdafx.h"
#include "nveditor.h"
#include "PacketModerm.h"
#include "MainFrm.h"
#include "NVEditorView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketModerm dialog


CPacketModerm::CPacketModerm(CWnd* pParent /*=NULL*/)
	: CDialog(CPacketModerm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPacketModerm)
		// NOTE: the ClassWizard will add member initialization here
	m_bMerge = FALSE;
	m_strNVFilePath = _T("");
	m_strModermFileName = _T("");
	//}}AFX_DATA_INIT
}


void CPacketModerm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPacketModerm)
	DDX_Control(pDX, IDC_PAKET_MODERM, m_ctlModermFile);
	DDX_Control(pDX, IDC_PACKET_NV, m_ctlNVFile);
	DDX_Control(pDX, IDC_PACKETM_BROWSE, m_ctlModermBrowse);
	DDX_Control(pDX, IDC_PACKETNV_BROWSE, m_ctlNVBrowse);

	DDX_Text(pDX, IDC_PACKET_NV, m_strNVFilePath);
	DDX_Text(pDX, IDC_PAKET_MODERM, m_strModermFileName);

	DDX_Check(pDX, IDC_MODERM_CHECK, m_bMerge);

		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPacketModerm, CDialog)
	//{{AFX_MSG_MAP(CPacketModerm)
	ON_BN_CLICKED(IDC_PACKETNV_BROWSE, OnPacketnvBrowse)
	ON_BN_CLICKED(IDC_PACKETM_BROWSE, OnPacketmBrowse)
	ON_BN_CLICKED(IDC_MODERM_CHECK, OnModermCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketModerm message handlers

BOOL CPacketModerm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(FALSE);
	
    ::SetFocus(m_ctlModermFile.m_hWnd);
	
    m_ctlModermFile.EnableWindow(m_bMerge);
    m_ctlModermBrowse.EnableWindow(m_bMerge);

	return TRUE;
}

void CPacketModerm::OnPacketnvBrowse() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CMainFrame *pMF = (CMainFrame*)AfxGetMainWnd();
	CNVEditorView *pView = (CNVEditorView *)pMF->GetActiveView();
	m_strNVFilePath = pView->BrowseForFolder(_T("choose the project config_nv directory"), 
        (LPCTSTR)pView->m_pProject->m_szPrjFileName);
	
	UpdateData(FALSE);
			
}

void CPacketModerm::OnPacketmBrowse() 
{
	// TODO: Add your control notification handler code here
	static TCHAR szFilter[] = _T("Binary Files (*.bin;*.dat)|*.bin;*.dat|All Files (*.*)|*.*||");
	
    UpdateData();
	CFileDialog dlg(TRUE, NULL, m_strModermFileName, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter, NULL);
    if(dlg.DoModal() == IDOK)
    {
        // Copy the file path name
        m_strModermFileName = dlg.GetPathName();
        UpdateData(FALSE);
		
        return;
    }
}

void CPacketModerm::OnOK() 
{
	// TODO: Add extra validation here
	if (m_strNVFilePath.IsEmpty())
    {
		AfxMessageBox(_T("Please input project delta NV path!"));
        return;
    }

// 	CMainFrame *pMF = (CMainFrame*)AfxGetMainWnd();
// 	CNVEditorView *pView = (CNVEditorView *)pMF->GetActiveView();
// // 	TCHAR strFileName[20];
// // 	_tcscpy(strFileName, _T("delta_nv.bin"));
// 	if (!pView->m_pProject->GenerateConfigFile(m_strNVFilePath, m_strModermFileName.GetBuffer(0), FALSE))
// 	{
// 		AfxMessageBox(_T("Merge NVFiles failed."));
// 		m_strModermFileName.ReleaseBuffer();
// 		return;
// 	}
// 	m_strModermFileName.ReleaseBuffer();
// 	AfxMessageBox(_T("Merge NVFiles Success."));	

	
	CDialog::OnOK();
}

void CPacketModerm::OnModermCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_ctlModermFile.EnableWindow(m_bMerge);
    m_ctlModermBrowse.EnableWindow(m_bMerge);
	
}
