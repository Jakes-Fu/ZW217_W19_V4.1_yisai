// PromptInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nveditor.h"
#include "PromptInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPromptInfoDlg dialog


CPromptInfoDlg::CPromptInfoDlg(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CPromptInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPromptInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPromptInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPromptInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPromptInfoDlg, CResizableDialog)
	//{{AFX_MSG_MAP(CPromptInfoDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPromptInfoDlg message handlers

void CPromptInfoDlg::SetPromptText(CStringArray &agText)
{
	m_agPromptInfo.RemoveAll();
	m_agPromptInfo.Copy(agText);
}

BOOL CPromptInfoDlg::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ShowSizeGrip(TRUE);
	AddAnchor(IDC_EDT_PROMPT_TEXT,TOP_LEFT,BOTTOM_RIGHT);
	int nSize = m_agPromptInfo.GetSize();
	CString strText;
	for (int i=0; i<nSize; ++i)
	{
		strText += m_agPromptInfo[i];
		strText += _T("\r\n");
	}
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDT_PROMPT_TEXT);
	pEdit->SetWindowText(strText);
    pEdit->SetFocus();
    pEdit->SetSel(-1, -1);
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPromptInfoDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizableDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
