// AlertMsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chiptester.h"
#include "AlertMsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlertMsgDlg dialog


CAlertMsgDlg::CAlertMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAlertMsgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlertMsgDlg)
	m_strAlertMsg = _T("");
	m_bNotNeedNextTime = FALSE;
	//}}AFX_DATA_INIT
}


void CAlertMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlertMsgDlg)
	DDX_Text(pDX, IDC_EDIT_ALERT_MESSAGE, m_strAlertMsg);
	DDX_Check(pDX, IDC_CHECK_NEED_NEXT_TIME, m_bNotNeedNextTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlertMsgDlg, CDialog)
	//{{AFX_MSG_MAP(CAlertMsgDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlertMsgDlg message handlers

void CAlertMsgDlg::ChangeFont()
{
    SetFont( GetParent()->GetFont() );
    // iterate through and move all child windows and change their font.
    CWnd* pChildWnd = GetWindow(GW_CHILD);
    
    while (pChildWnd)
    {
        pChildWnd->SetFont(GetParent()->GetParent()->GetFont()); 
        pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
    }
}

BOOL CAlertMsgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	ChangeFont();
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
