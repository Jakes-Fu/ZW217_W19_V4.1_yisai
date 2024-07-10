// SpidPage.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "SpidPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpidPage property page

IMPLEMENT_DYNCREATE(CSpidPage, CPropertyPage)

CSpidPage::CSpidPage() : CPropertyPage(CSpidPage::IDD)
{
	//{{AFX_DATA_INIT(CSpidPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	VERIFY( m_brshDlgBckGrnd.CreateSolidBrush(BK_COLOR) );
}

CSpidPage::~CSpidPage()
{
}

void CSpidPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpidPage)
	DDX_Text(pDX, IDC_STC_TMSI, m_strTmsi);
	DDX_Text(pDX, IDC_STC_PTMSI, m_strPtmsi);
	DDX_Text(pDX, IDC_STC_IMEISY, m_strImeisy);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpidPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSpidPage)
	ON_BN_CLICKED(IDC_BTN_SPID_REAR, OnBtnSpidRear)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpidPage message handlers
void CSpidPage::SendCheckRequest()
{
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{	
		theApp.m_pEtionFactory->SendATCmd(SPID_CHECK, 0, NULL, 0);
	}
}

BOOL CSpidPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSpidPage::OnBtnSpidRear() 
{
	// TODO: Add your control notification handler code here
	SendCheckRequest();
}

void CSpidPage::ShowRespondContent(LPCSTR szRet)
{
	CString strText = szRet;
	int nIndex = -1;

	if((nIndex = strText.Find(_T("IMEISV:")) ) != -1)
	{
		strText.Delete(0, nIndex + 7);
		m_strImeisy = strText;
		UpdateData(FALSE);		
	}
	if((nIndex = strText.Find(_T("PTMSI:")) ) != -1)
	{
		strText.Delete(0, nIndex + 6);
		m_strPtmsi = strText;
		UpdateData(FALSE);		
	}
	if(strText.Find(_T("TMSI:")) != -1 && strText.Find(_T("PTMSI:")) == -1)
	{
		nIndex = strText.Find(_T("TMSI:"));
		strText.Delete(0, nIndex + 5);
		m_strTmsi = strText;
		UpdateData(FALSE);		
	}
	UpdateData(FALSE);
}

void CSpidPage::ClearAll()
{
	m_strTmsi.Empty();
	m_strPtmsi.Empty();
	m_strImeisy.Empty();
	UpdateData(FALSE);
}

HBRUSH CSpidPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nCtlColor);

	pDC->SetBkColor( BK_COLOR );

	// TODO: Return a different brush if the default is not desired
	return (HBRUSH)(m_brshDlgBckGrnd.m_hObject);
}
