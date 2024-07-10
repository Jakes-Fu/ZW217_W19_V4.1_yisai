// DlgConvSetting.cpp : implementation file
//

#include "stdafx.h"
#include "ownership.h"
#include "DlgConvSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgConvSetting dialog


CDlgConvSetting::CDlgConvSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConvSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgConvSetting)
	m_strQCMidFile = _T("R_ACC_CITY_TYPE_QUHAO");
	m_strQCTxtFile = _T("");
	//}}AFX_DATA_INIT
}


void CDlgConvSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgConvSetting)
	DDX_Text(pDX, IDC_CS_EDT_QUHAO_CITY_MID_FILE, m_strQCMidFile);
	DDX_Text(pDX, IDC_CS_EDT_QUHAO_CITY_TXT_FILE, m_strQCTxtFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgConvSetting, CDialog)
	//{{AFX_MSG_MAP(CDlgConvSetting)
	ON_BN_CLICKED(IDC_CS_BTN_QCM_BROW, OnCsBtnQcmBrow)
	ON_BN_CLICKED(IDC_CS_BTN_QCT_BROW, OnCsBtnQctBrow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgConvSetting message handlers

void CDlgConvSetting::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(m_strQCTxtFile.IsEmpty() || m_strQCMidFile.IsEmpty())
	{
		AfxMessageBox(_T("Please fill the edit"));
		return;
	}
	CDialog::OnOK();
}

void CDlgConvSetting::OnCsBtnQcmBrow() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,_T(".dat"),_T("R_ACC_CITY_TYPE_QUHAO"),OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, 
		_T("City quhao files (*.dat)|*.dat|All files (*.*)|*.*||"));
	if(dlg.DoModal() == IDOK)
	{
		m_strQCMidFile = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CDlgConvSetting::OnCsBtnQctBrow() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(FALSE,_T(".txt"),_T("R_ACC_CITY_TYPE_QUHAO"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		_T("City quhao files (*.txt)|*.txt||"));
	if(dlg.DoModal() == IDOK)
	{
		m_strQCTxtFile = dlg.GetPathName();
		UpdateData(FALSE);
	}
}
