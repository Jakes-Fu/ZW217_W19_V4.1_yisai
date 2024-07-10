// SautoattPage.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "SautoattPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSautoattPage property page

IMPLEMENT_DYNCREATE(CSautoattPage, CPropertyPage)

CSautoattPage::CSautoattPage() : CPropertyPage(CSautoattPage::IDD)
{
	//{{AFX_DATA_INIT(CSautoattPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bInit = FALSE;

	VERIFY( m_brshDlgBckGrnd.CreateSolidBrush(BK_COLOR) );
}

CSautoattPage::~CSautoattPage()
{
}

void CSautoattPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSautoattPage)
	DDX_Control(pDX, IDC_BTN_AUTOATT_READ, m_btnRead);
	DDX_Control(pDX, IDC_BTN_AUTOATT_SET, m_btnSet);
	DDX_Control(pDX, IDC_COMBO_AUTOATT_MODE, m_boxMode);
	DDX_Text(pDX, IDC_STC_CUR_ATT, m_strAttch);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSautoattPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSautoattPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_AUTOATT_READ, OnBtnAutoattRead)
	ON_BN_CLICKED(IDC_BTN_AUTOATT_SET, OnBtnAutoattSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSautoattPage message handlers

HBRUSH CSautoattPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nCtlColor);

	pDC->SetBkColor( BK_COLOR );

	// TODO: Return a different brush if the default is not desired
	return (HBRUSH)(m_brshDlgBckGrnd.m_hObject);
}

BOOL CSautoattPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strText;
	strText.LoadString(IDS_AUTOATT_HAND);
	m_boxMode.AddString(strText);

	strText.LoadString(IDS_AUTOATT_AUTO);
	m_boxMode.AddString(strText);

	m_boxMode.SetCurSel(0);
	m_bInit = TRUE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSautoattPage::SendCheckRequest()
{	
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		theApp.m_pEtionFactory->SendATCmd(SAUTOATT_READ, 0, NULL, 0);
	}
}

void CSautoattPage::ShowRespondContent(LPCSTR szRet)
{
	if(!m_bInit)
	{
		return;
	}
	CString strText = szRet;
//	int nIndex = -1;

	if(strText.Find(_T("+SAUTOATT:")) != -1)
	{
		m_strAttch = strText.Right(1);
		if(m_strAttch.Find(_T("0")) != -1)
		{
			m_strAttch.LoadString(IDS_AUTOATT_HAND);
		}
		else
		{
			m_strAttch.LoadString(IDS_AUTOATT_AUTO);
		}
	}
	UpdateData(FALSE);
}

void CSautoattPage::SendSetRequest()
{
	UpdateData();

	if(theApp.m_pEtionFactory != NULL  && theApp.m_bConnected)
	{
		char szRet[MAX_PATH] = {0};	
		TCHAR szPara[MAX_PATH] = {0};
		
		int nIndex = m_boxMode.GetCurSel();
		
#ifdef _UNICODE
		swprintf(szPara, _T("%d"), nIndex);
#else
		sprintf(szPara, _T("%d"), nIndex);
#endif
		
		LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(SAUTOATT_SET, 0, (LPVOID)szPara, _tcslen(szPara));
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
			if(nIndex == 0)
			{
				m_strAttch.LoadString(IDS_AUTOATT_HAND);
			}
			else
			{
				m_strAttch.LoadString(IDS_AUTOATT_AUTO);
			}
			UpdateData(FALSE);
		}
		
	}
}

void CSautoattPage::ClearAll()
{

	m_boxMode.SetCurSel(0);
	m_strAttch.Empty();

	UpdateData(FALSE);

}

void CSautoattPage::OnBtnAutoattRead() 
{
	// TODO: Add your control notification handler code here
	SendCheckRequest();
	
}

void CSautoattPage::OnBtnAutoattSet() 
{
	// TODO: Add your control notification handler code here
	SendSetRequest();
	
}
