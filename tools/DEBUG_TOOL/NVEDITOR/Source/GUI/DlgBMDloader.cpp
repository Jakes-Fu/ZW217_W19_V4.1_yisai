// DlgBMDloader.cpp : implementation file
//

#include "stdafx.h"
#include "nveditor.h"
#include "DlgBMDloader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBMDloader dialog


CDlgBMDloader::CDlgBMDloader(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBMDloader::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBMDloader)
	m_strState = _T("");
	m_strStep = _T("");
	m_strProg = _T("");
	//}}AFX_DATA_INIT
	m_nState = -1;
}


void CDlgBMDloader::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBMDloader)
	DDX_Control(pDX, IDC_PRG_PROGRESS, m_prgrsCtrl);
	DDX_Text(pDX, IDC_STC_STATE, m_strState);
	DDX_Text(pDX, IDC_STC_STEP, m_strStep);
	DDX_Text(pDX, IDC_STC_PROG, m_strProg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBMDloader, CDialog)
	//{{AFX_MSG_MAP(CDlgBMDloader)
	ON_BN_CLICKED(IDC_BTN_STOP, OnStop)
	//}}AFX_MSG_MAP
	ON_MESSAGE(BM_PASS_MSG,OnPass)
	ON_MESSAGE(BM_FAIL_MSG,OnFail)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBMDloader message handlers

BOOL CDlgBMDloader::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgBMDloader::SetStep(LPCTSTR step)
{
	m_strStep = step;
	HWND hwnd = this->GetSafeHwnd();
	if(hwnd != NULL && IsWindow(hwnd))
	{
		UpdateData(FALSE);
	}
}

void CDlgBMDloader::SetState(LPCTSTR state)
{
	m_strState = state;

	HWND hwnd = this->GetSafeHwnd();
	if(hwnd != NULL && IsWindow(hwnd))
	{
		UpdateData(FALSE);
	}
	
}

void CDlgBMDloader::SetProg(LPCTSTR prog)
{
	m_strProg = prog;
	HWND hwnd = this->GetSafeHwnd();
	if(hwnd != NULL && IsWindow(hwnd))
	{
		UpdateData(FALSE);
	}
}

void CDlgBMDloader::OnStop() 
{
	// TODO: Add your control notification handler code here
// 	if(m_nState == -1)
// 		::SendMessage(((AfxGetMainWnd()->GetActiveWindow()))->GetSafeHwnd(),BM_STOP_MSG,0,0);
	CDialog::OnCancel();
}

void CDlgBMDloader::OnPass()
{
	m_nState = 1;
}

void CDlgBMDloader::OnFail()
{
	m_nState = 0; 
}


BOOL CDlgBMDloader::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )	
	{		
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)	
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);			
			return 1;
		}					
		
	}
	return CDialog::PreTranslateMessage(pMsg);
}
