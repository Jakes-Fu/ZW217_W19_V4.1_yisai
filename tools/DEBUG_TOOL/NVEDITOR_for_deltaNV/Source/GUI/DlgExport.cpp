// DlgExport.cpp : implementation file
//

#include "stdafx.h"
#include "nveditor.h"
#include "DlgExport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgExport dialog


CDlgExport::CDlgExport(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgExport)
	m_bAppendMode = TRUE;
	m_bChanged = FALSE;
	m_bReadOnly = FALSE;
	m_strFile = _T("");
	//}}AFX_DATA_INIT
}


void CDlgExport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgExport)
	DDX_Check(pDX, IDC_CHECK_APPEND_NV, m_bAppendMode);
	DDX_Check(pDX, IDC_ED_CHK_CHANGED, m_bChanged);
	DDX_Check(pDX, IDC_ED_CHK_READONLY, m_bReadOnly);
	DDX_Text(pDX, IDC_ED_FILE, m_strFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgExport, CDialog)
	//{{AFX_MSG_MAP(CDlgExport)
	ON_BN_CLICKED(IDC_ED_BTN_BROW, OnBrow)
	ON_BN_CLICKED(IDC_ED_CHK_CHANGED, OnChkChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgExport message handlers

void CDlgExport::OnBrow() 
{
	// TODO: Add your control notification handler code here
	static TCHAR BASED_CODE szFilter[] = _T("NV export Files (*.nv)|*.nv|All files (*.*)|*.*||");
	UpdateData();

	CFileDialog dlg(FALSE, _T("nv"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
    if( IDOK != dlg.DoModal() )
    {
        // User cancel
        return;
    }
	
	m_strFile = dlg.GetPathName();

	UpdateData(FALSE);

}

void CDlgExport::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	if(m_strFile.IsEmpty())
	{
		AfxMessageBox(_T("Please input export file!"));
		return;
	}

	CFileFind find;
	if(find.FindFile(m_strFile))
	{
		DWORD dwAtt = GetFileAttributes(m_strFile.operator LPCTSTR());
		dwAtt &= ~FILE_ATTRIBUTE_READONLY;
		SetFileAttributes(m_strFile.operator LPCTSTR(),dwAtt);
	}

	CDialog::OnOK();
}

BOOL CDlgExport::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_CHECK_APPEND_NV)->ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgExport::OnChkChanged() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_CHECK_APPEND_NV)->ShowWindow(m_bChanged ? SW_SHOW : SW_HIDE);
	
}
