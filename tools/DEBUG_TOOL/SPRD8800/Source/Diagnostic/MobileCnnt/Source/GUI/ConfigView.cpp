// ConfigView.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "ConfigView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CConfigView

IMPLEMENT_DYNCREATE(CConfigView, CFormView)

CConfigView::CConfigView()
	: CCoolFormView(CConfigView::IDD)
{
	//{{AFX_DATA_INIT(CConfigView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSelNetDlg = NULL;
	m_pDialNetDlg = NULL;
	m_pSecurityDlg = NULL;
	m_pOthersDlg = NULL;
	m_pDlgTDGSM = NULL;
}

CConfigView::~CConfigView()
{
	m_pSelNetDlg = NULL;
	m_pDialNetDlg = NULL;
	m_pSecurityDlg = NULL;
	m_pOthersDlg = NULL;
	m_pDlgTDGSM = NULL;
}

void CConfigView::DoDataExchange(CDataExchange* pDX)
{
	CCoolFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigView)
	DDX_Control(pDX, IDC_TAB_CFG, m_tabCtrl);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigView, CCoolFormView)
	//{{AFX_MSG_MAP(CConfigView)
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CFG, OnSelchangeTabCfg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigView diagnostics

#ifdef _DEBUG
void CConfigView::AssertValid() const
{
	CFormView::AssertValid();
}

void CConfigView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CConfigView message handlers


void CConfigView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	m_pSelNetDlg = new CDlgSelectNetwork();
	m_pDialNetDlg = new CDlgDialNetwork();
	m_pSecurityDlg = new CDlgSecurity();
	m_pOthersDlg = new CDlgOthers();
	m_pDlgTDGSM = new CDlgTDGSM();

	m_pSelNetDlg->Create(IDD_DLG_SELECT_NETWORK, &m_tabCtrl);
	m_pDialNetDlg->Create(IDD_DLG_DIAL_NETWORK, &m_tabCtrl);
	m_pSecurityDlg->Create(IDD_DLG_SECURITY, &m_tabCtrl);
	m_pDlgTDGSM->Create(IDD_DLG_TDGSM, &m_tabCtrl);
	m_pOthersDlg->Create(IDD_DLG_OTHERS, &m_tabCtrl);

	CString strTitle;
	strTitle.LoadString(IDS_SELECTNETWORK);
	m_tabCtrl.InsertItem(0,(LPCTSTR)strTitle);
	strTitle.LoadString(IDS_CONNECT_DIAL);
	m_tabCtrl.InsertItem(1,(LPCTSTR)strTitle);
	strTitle.LoadString(IDS_SECURITY);
	m_tabCtrl.InsertItem(2,(LPCTSTR)strTitle);
	strTitle.LoadString(IDS_TDGSM);
	m_tabCtrl.InsertItem(3,(LPCTSTR)strTitle);
	strTitle.LoadString(IDS_OTHERS);
	m_tabCtrl.InsertItem(4,(LPCTSTR)strTitle);

	m_tabCtrl.SetMinTabWidth(-1);
	m_tabCtrl.SetCurSel(0);
	DoTab(0);
	
}

void CConfigView::DoTab(int nTab)
{
	BOOL bTab[CFG_PAGE_NUM];
	for(int i = 0; i < CFG_PAGE_NUM; i++)
	{
		bTab[i] = FALSE;
	}

	bTab[nTab]=TRUE;

	SetDlgState(m_pSelNetDlg,bTab[0]);
	SetDlgState(m_pDialNetDlg,bTab[1]);
	SetDlgState(m_pSecurityDlg,bTab[2]);
	SetDlgState(m_pDlgTDGSM, bTab[3]);
	SetDlgState(m_pOthersDlg, bTab[4]);

}

void CConfigView::SetDlgState(CWnd *pWnd, BOOL bShow)
{
	pWnd->EnableWindow(bShow);
	if (bShow)
	{
		pWnd->ShowWindow(SW_SHOW);
		pWnd->CenterWindow();
	} 
	else  pWnd->ShowWindow(SW_HIDE);
}

void CConfigView::OnDestroy() 
{
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_pSelNetDlg)
	{
		delete m_pSelNetDlg;
		m_pSelNetDlg = NULL;
	}

	if(m_pDialNetDlg)
	{
		delete m_pDialNetDlg;
		m_pDialNetDlg = NULL;
	}

	if(m_pSecurityDlg)
	{
		delete m_pSecurityDlg;
		m_pSecurityDlg = NULL;
	}
	if(m_pOthersDlg)
	{
		delete m_pOthersDlg;
		m_pOthersDlg = NULL;
	}
	if(m_pDlgTDGSM)
	{
		delete m_pDlgTDGSM;
		m_pDlgTDGSM = NULL;
	}
}

void CConfigView::OnSelchangeTabCfg(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);

	// TODO: Add your control notification handler code here
	int nSelect=m_tabCtrl.GetCurSel();
	if(nSelect>=0) 
	{
		DoTab(nSelect);
		switch(nSelect) {
		case 0:
			m_pSelNetDlg->SendCheckRequest();
			break;
		case 1:
			m_pDialNetDlg->SendCheckRequest();
			break;
		case 2:
			m_pSecurityDlg->SendCheckRequest();
			break;
		case 3:
			m_pDlgTDGSM->SendCheckRequest();
			break;
		case 4:
			m_pOthersDlg->SendCheckRequest();
			break;
		default:
			break;
		}
	}
	*pResult = 0;
}

void CConfigView::ClearAll()
{
	if(m_pOthersDlg != NULL)
	{
		m_pOthersDlg->ClearAll();
	}

	if(m_pSecurityDlg != NULL)
	{
		m_pSecurityDlg->ClearAll();
	}
	
	if(m_pSelNetDlg != NULL)
	{
		m_pSelNetDlg->ClearAll();
	}

	if(m_pDialNetDlg != NULL)
	{
		m_pDialNetDlg->ClearAll();
	}

	if(m_pDlgTDGSM != NULL)
	{
		m_pDlgTDGSM->ClearAll();
	}

	m_tabCtrl.SetCurSel(0);
	DoTab(0);
}

void CConfigView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	UNREFERENCED_PARAMETER(pSender);
	UNREFERENCED_PARAMETER(lHint);
	UNREFERENCED_PARAMETER(pHint);
	// TODO: Add your specialized code here and/or call the base class
}
