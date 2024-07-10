// SppsratePage.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "SppsratePage.h"
#include "DialUpView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSppsratePage property page

IMPLEMENT_DYNCREATE(CSppsratePage, CPropertyPage)

CSppsratePage::CSppsratePage() : CPropertyPage(CSppsratePage::IDD)
{
	//{{AFX_DATA_INIT(CSppsratePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_uSpeedUp = 64;
	m_uSpeedDown = 384;
	m_bInit = FALSE;

	VERIFY( m_brshDlgBckGrnd.CreateSolidBrush(BK_COLOR) );
}

CSppsratePage::~CSppsratePage()
{
}

void CSppsratePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSppsratePage)
	DDX_Control(pDX, IDC_LST_SRATE_READ, m_lstRead);
	DDX_Control(pDX, IDC_LST_SRATE_TEST, m_lstTest);
	DDX_Text(pDX, IDC_EDT_SRATE_UP_SPEED, m_uSpeedUp);
	DDX_Text(pDX, IDC_EDT_SRATE_DOWN_SPEED, m_uSpeedDown);
	DDX_Control(pDX, IDC_COMBO_SRATE_TYPE, m_boxType);
	DDX_Control(pDX, IDC_BTN_READ_SRATE, m_btnRead);
	DDX_Control(pDX, IDC_BTN_TEST_SRATE, m_btnTest);
	DDX_Control(pDX, IDC_BTN_SET_SRATE, m_btnSet);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSppsratePage, CPropertyPage)
	//{{AFX_MSG_MAP(CSppsratePage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_READ_SRATE, OnBtnReadSrate)
	ON_BN_CLICKED(IDC_BTN_TEST_SRATE, OnBtnTestSrate)
	ON_BN_CLICKED(IDC_BTN_SET_SRATE, OnBtnSetSrate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSppsratePage message handlers

BOOL CSppsratePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strText;

	
	int nCount = sizeof(SRATE_TYPE) / sizeof(SRATE_TYPE[0]);
	for(int i = 0; i < nCount; i++)
	{
		m_boxType.AddString(SRATE_TYPE[i]);
	}

	m_boxType.SetCurSel(0);


	DWORD dwStyle = m_lstTest.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
    m_lstTest.SetExtendedStyle(dwStyle);
	m_lstRead.SetExtendedStyle(dwStyle);

	strText.LoadString(IDS_SRATE_TYPE);
	m_lstTest.InsertColumn(0,strText,LVCFMT_LEFT,70);
	m_lstRead.InsertColumn(0,strText,LVCFMT_LEFT,70);
	strText.LoadString(IDS_SRATE_UP_LINK);
	m_lstTest.InsertColumn(1,strText,LVCFMT_LEFT,70);
	m_lstRead.InsertColumn(1,strText,LVCFMT_LEFT,70);
	strText.LoadString(IDS_SRATE_DOWN_LINK);
	m_lstTest.InsertColumn(2,strText,LVCFMT_LEFT,70);
	m_lstRead.InsertColumn(2,strText,LVCFMT_LEFT,70);

	for(i = 0; i < NUM_SPRATE; i++)
	{
		m_lstRead.InsertItem(i, _T(""));
	}
	m_lstTest.InsertItem(0, _T(""));

	m_bInit = TRUE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CSppsratePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nCtlColor);

	pDC->SetBkColor( BK_COLOR );

	// TODO: Return a different brush if the default is not desired
	return (HBRUSH)(m_brshDlgBckGrnd.m_hObject);
}


void CSppsratePage::SendCheckRequest(UINT uFlag)
{	
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		if(uFlag == FOR_SRATE_READ)
		{
			theApp.m_pEtionFactory->SendATCmd(SRATE_READ, 0, NULL, 0);
		}

		if(uFlag == FOR_SRATE_TEST)
		{
			theApp.m_pEtionFactory->SendATCmd(SRATE_TEST, 0, NULL, 0);
		}
	}
}

void CSppsratePage::ShowRespondContent(LPCSTR szRet)
{
	if(!m_bInit)
	{
		return;
	}
	CString strText = szRet;
	int nIndex = -1;
	CString strType, strSpeedUp, strSpeedDown;

	if(strText.Find(_T("+SPPSRATE:")) != -1 && strText.Find(_T("+SPPSRATE:(")) == -1)
	{
		nIndex = strText.Find(_T("+SPPSRATE:"));
		
		strText.Delete(0, nIndex + 10);
		
		nIndex = strText.Find(_T(","));
		if(nIndex != -1)
		{
			strType = strText.Left(nIndex);
			strText.Delete(0, nIndex + 1);
			nIndex = strText.Find(_T(","));
			if(nIndex != -1)
			{
				strSpeedUp = strText.Left(nIndex);
				strText.Delete(0, nIndex + 1);
				strSpeedDown = strText;
			}
		}

		UINT uID;
#ifdef _UNICODE
		swscanf(strType, _T("%d"), &uID);
#else
		sscanf(strType, _T("%d"), &uID);
#endif

		strType = SRATE_TYPE[uID];


		m_lstRead.SetItemText(uID, 0, strType);
		m_lstRead.SetItemText(uID, 1, strSpeedUp);
		m_lstRead.SetItemText(uID, 2, strSpeedDown);	
	}

	if((nIndex = strText.Find(_T("+SPPSRATE:("))) != -1)
	{
		strText.Delete(0, nIndex + 10);
		nIndex = strText.Find(_T(","));
		if(nIndex != -1)
		{
			strType = strText.Left(nIndex);
			strText.Delete(0, nIndex + 1);
			nIndex = strText.Find(_T(")"));
			if(nIndex != -1)
			{
				strSpeedUp = strText.Left(nIndex + 1);
				strText.Delete(0, nIndex + 2);
				strSpeedDown = strText;
			}
		}
		m_lstTest.SetItemText(0, 0, strType);
		m_lstTest.SetItemText(0, 1, strSpeedUp);
		m_lstTest.SetItemText(0, 2, strSpeedDown);

	}
	UpdateData(FALSE);
}

void CSppsratePage::SendSetRequest()
{
	UpdateData();

	if(theApp.m_pEtionFactory != NULL  && theApp.m_bConnected)
	{
		char szRet[MAX_PATH] = {0};	
		TCHAR szPara[MAX_PATH] = {0};
		
		int nIndex = m_boxType.GetCurSel();
#ifdef _UNICODE
		swprintf(szPara, _T("%d,%d,%d"), nIndex, m_uSpeedUp, m_uSpeedDown);
#else
		sprintf(szPara, _T("%d,%d,%d"), nIndex, m_uSpeedUp, m_uSpeedDown);
#endif
		
		LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(SRATE_SET, 0, (LPVOID)szPara, _tcslen(szPara));
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

			CString strType, strSpeed;
			m_boxType.GetLBText(nIndex, strType);
			m_lstRead.SetItemText(nIndex, 0, strType);
			strSpeed.Format(_T("%d"), m_uSpeedUp);
			m_lstRead.SetItemText(nIndex, 1, strSpeed);
			strSpeed.Format(_T("%d"), m_uSpeedDown);
			m_lstRead.SetItemText(nIndex, 2, strSpeed);
			
			UpdateData(FALSE);
		}
		
	}
}

void CSppsratePage::ClearAll()
{

	m_boxType.SetCurSel(0);
	m_uSpeedUp = 64;
	m_uSpeedDown = 384;
	
	for(int i = 0; i < NUM_SPRATE; i++)
	{
		m_lstRead.SetItemText(i, 0, _T(""));
		m_lstRead.SetItemText(i, 1, _T(""));
		m_lstRead.SetItemText(i, 2, _T(""));
	}

	m_lstTest.SetItemText(0,0,_T(""));
	m_lstTest.SetItemText(0,1,_T(""));
	m_lstTest.SetItemText(0,2,_T(""));

	UpdateData(FALSE);

}

void CSppsratePage::OnBtnReadSrate() 
{
	// TODO: Add your control notification handler code here
	SendCheckRequest(FOR_SRATE_READ);
	
}

void CSppsratePage::OnBtnTestSrate() 
{
	// TODO: Add your control notification handler code here
	SendCheckRequest(FOR_SRATE_TEST);
	
}

void CSppsratePage::OnBtnSetSrate() 
{
	// TODO: Add your control notification handler code here
	SendSetRequest();
	
}
