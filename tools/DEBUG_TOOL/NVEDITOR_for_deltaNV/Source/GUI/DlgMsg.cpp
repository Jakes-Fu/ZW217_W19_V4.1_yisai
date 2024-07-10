// DlgMsg.cpp : implementation file
//

#include "stdafx.h"
#include "nveditor.h"
#include "DlgMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMsg dialog


CDlgMsg::CDlgMsg(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDlgMsg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMsg)
	m_strTitle = _T("");
	//}}AFX_DATA_INIT
	m_agItem.RemoveAll();
	bShowCancelBtn = FALSE;
}

CDlgMsg::~CDlgMsg()
{

}


void CDlgMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMsg)
	DDX_Control(pDX, IDC_STC_ICON, m_stcIcon);
	DDX_Text(pDX, IDC_STC_TITLE, m_strTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMsg, CResizableDialog)
	//{{AFX_MSG_MAP(CDlgMsg)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMsg message handlers

BOOL CDlgMsg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDCANCEL)->ShowWindow(bShowCancelBtn? SW_SHOW : SW_HIDE);

	//ShowSizeGrip(TRUE);
	//AddAnchor(IDC_EDT_ERROR_CONTENT,MIDDLE_LEFT,MIDDLE_RIGHT);
	int nSize = m_agItem.GetSize();
	CString strText;
	for (int i=0; i<nSize; ++i)
	{
		strText += m_agItem[i];
		strText += _T("\r\n");
	}
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDT_ERROR_CONTENT);
	pEdit->SetWindowText(strText);
    pEdit->SetFocus();
    pEdit->SetSel(-1, -1);

	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMsg::FillList()
{
	int nCount = m_agItem.GetSize();
	for(int i = 0; i< nCount; i++)
	{
		m_lst.AddString(m_agItem[i]);
	}
}

BOOL CDlgMsg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	
	return CResizableDialog::PreTranslateMessage(pMsg);
}

void CDlgMsg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CResizableDialog::OnCancel();
}

