// GotoItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "GotoItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGotoItemDlg dialog


CGotoItemDlg::CGotoItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGotoItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGotoItemDlg)
	m_nIndex = 0;
	//}}AFX_DATA_INIT
}


void CGotoItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGotoItemDlg)
	DDX_Text(pDX, IDC_EDIT_INDEX, m_nIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGotoItemDlg, CDialog)
	//{{AFX_MSG_MAP(CGotoItemDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGotoItemDlg message handlers

BOOL CGotoItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_INDEX);
    pEdit->SetSel(0,-1);
    pEdit->SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
