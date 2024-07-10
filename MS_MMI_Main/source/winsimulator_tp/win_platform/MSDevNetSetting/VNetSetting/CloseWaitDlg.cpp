// CloseWaitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VNetSetting.h"
#include "CloseWaitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define CLOSE_WAITING_TIMER_ID      0x01
#define CLOSE_WAITING_TIME_LEN      1000
#define CLOSE_WAITING_SECOND        4

#define COLOR_RED                   RGB(192, 0, 0)


/////////////////////////////////////////////////////////////////////////////
// CCloseWaitDlg dialog


CCloseWaitDlg::CCloseWaitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCloseWaitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCloseWaitDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_nWait = CLOSE_WAITING_SECOND;
}


void CCloseWaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCloseWaitDlg)
	DDX_Control(pDX, IDC_STATIC_TIMER, m_staticTime);
	DDX_Control(pDX, IDC_STATIC_NOTES, m_staticNotes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCloseWaitDlg, CDialog)
	//{{AFX_MSG_MAP(CCloseWaitDlg)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCloseWaitDlg message handlers

BOOL CCloseWaitDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetWindowText(_T("Notes"));
    m_staticNotes.SetWindowText(_T("When driver is installed, Restart the simulator!"));
    
    CString str;
    str.Format("To close in %d seconds ...", m_nWait);
    m_staticTime.SetWindowText(str);

    // start closing timer
    SetTimer(CLOSE_WAITING_TIMER_ID, CLOSE_WAITING_TIME_LEN, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCloseWaitDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
    {
    case CLOSE_WAITING_TIMER_ID:
        if( 0 == --m_nWait )
        {
            KillTimer(CLOSE_WAITING_TIMER_ID);
            ::PostMessage(this->GetSafeHwnd(), WM_CLOSE, 0, 0);
        }
        else
        {
            CString str;
            str.Format("To close in %d seconds ...", m_nWait);
            m_staticTime.SetWindowText(str);
        }
        break;
    }

	CDialog::OnTimer(nIDEvent);
}

void CCloseWaitDlg::OnOK() 
{
	// TODO: Add extra validation here
	KillTimer(CLOSE_WAITING_TIMER_ID);

	CDialog::OnOK();
}

HBRUSH CCloseWaitDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if( NULL != pWnd )
    {
        switch( pWnd->GetDlgCtrlID() )
        {
        case IDC_STATIC_TIMER:
            pDC->SetTextColor( COLOR_RED );
            pDC->SetBkMode( TRANSPARENT );
            break;
        }
    }

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
