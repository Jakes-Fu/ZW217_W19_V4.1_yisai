// DspPage.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "DspPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDspPage property page

IMPLEMENT_DYNCREATE(CDspPage, CPropertyPage)

CDspPage::CDspPage() : CPropertyPage(CDspPage::IDD)
{
	//{{AFX_DATA_INIT(CDspPage)
		// NOTE: the ClassWizard will add member initialization here
	m_uParam1 = 0;
	m_uParam2 = 0;
	m_uParam3 = 0;
	m_uParam4 = 0;
	//}}AFX_DATA_INIT
	VERIFY( m_brshDlgBckGrnd.CreateSolidBrush(BK_COLOR) );
}

CDspPage::~CDspPage()
{
}

void CDspPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDspPage)
	DDX_Text(pDX, IDC_EDT_PARAM1, m_uParam1);
	DDX_Text(pDX, IDC_EDT_PARAM2, m_uParam2);
	DDX_Text(pDX, IDC_EDT_PARAM3, m_uParam3);
	DDX_Text(pDX, IDC_EDT_PARAM4, m_uParam4);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDspPage, CPropertyPage)
	//{{AFX_MSG_MAP(CDspPage)
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDspPage message handlers

void CDspPage::OnBtnApply() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if(theApp.m_pEtionFactory != NULL  && theApp.m_bConnected)
	{
		char szRet[MAX_PATH] = {0};	
		TCHAR szPara[MAX_PATH] = {0};
		
#ifdef _UNICODE
		swprintf(szPara, _T("%d,%d,%d,%d"), m_uParam1, m_uParam2, m_uParam3, m_uParam4);
#else
		sprintf(szPara, _T("%d,%d,%d.%d"), m_uParam1, m_uParam2, m_uParam3, m_uParam4);
#endif
		
		LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(DSP_SET, 0, (LPVOID)szPara, _tcslen(szPara));
		if(lpRet != NULL)
		{
			strcpy(szRet, lpRet);
		}
		
		if(strstr(szRet, "ERROR") != NULL)
		{
			AfxMessageBox(IDS_SET_FAILED, MB_ICONERROR);
		}
		if(strstr(szRet, "OK") != NULL)
		{
			AfxMessageBox(IDS_SET_SUCCESS);
		}
		
	}
	
}
void CDspPage::ClearAll()
{
	m_uParam1 = 0;
	m_uParam2 = 0;
	m_uParam3 = 0;
	m_uParam4 = 0;

	UpdateData(FALSE);
}

HBRUSH CDspPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nCtlColor);

	pDC->SetBkColor( BK_COLOR );

	// TODO: Return a different brush if the default is not desired
	return (HBRUSH)(m_brshDlgBckGrnd.m_hObject);
}
