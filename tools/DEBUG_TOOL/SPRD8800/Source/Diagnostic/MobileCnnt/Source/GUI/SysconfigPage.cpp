// SysconfigPage.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "SysconfigPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysconfigPage dialog

IMPLEMENT_DYNCREATE(CSysconfigPage, CPropertyPage)

CSysconfigPage::CSysconfigPage() : CPropertyPage(CSysconfigPage::IDD)
{
	//{{AFX_DATA_INIT(CSysconfigPage)
		// NOTE: the ClassWizard will add member initialization here
	m_bInit = FALSE;
	//}}AFX_DATA_INIT
	VERIFY( m_brshDlgBckGrnd.CreateSolidBrush(BK_COLOR) );
}

CSysconfigPage::~CSysconfigPage()
{
}

void CSysconfigPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysconfigPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_LST_SYSCONFIG_READ, m_lstRead);
	DDX_Control(pDX, IDC_COMBO_SYSCONFIG_MODE, m_boxMode);
	DDX_Control(pDX, IDC_COMBO_SYSCONFIG_APN, m_boxOrder);
	DDX_Control(pDX, IDC_COMBO_SYSCONFIG_ROAM, m_boxRoam);
	DDX_Control(pDX, IDC_COMBO_SYSCONFIG_DOMAIN, m_boxDomain);
	DDX_Control(pDX, IDC_BTN_READ_SYSCONFIG, m_btnRead);
	DDX_Control(pDX, IDC_BTN_SET_SYSCONFIG, m_btnSet);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysconfigPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSysconfigPage)
	ON_BN_CLICKED(IDC_BTN_READ_SYSCONFIG, OnBtnReadSysconfig)
	ON_BN_CLICKED(IDC_BTN_SET_SYSCONFIG, OnBtnSetSysconfig)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysconfigPage message handlers

BOOL CSysconfigPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	DWORD dwStyle = m_lstRead.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	m_lstRead.SetExtendedStyle(dwStyle);

	m_lstRead.InsertColumn(0,_T("Mode"),LVCFMT_LEFT,90);
	m_lstRead.InsertColumn(1,_T("AcqOrder"),LVCFMT_LEFT,90);
	m_lstRead.InsertColumn(2,_T("Roam"),LVCFMT_LEFT,90);
	m_lstRead.InsertColumn(3,_T("SrvDomain"),LVCFMT_LEFT,90);

	CString strText;

	strText.LoadString(IDS_SYSCONFIG_MODE_0);
	m_boxMode.AddString(strText);
	strText.LoadString(IDS_SYSCONFIG_MODE_1);
	m_boxMode.AddString(strText);
	strText.LoadString(IDS_SYSCONFIG_MODE_2);
	m_boxMode.AddString(strText);
	strText.LoadString(IDS_SYSCONFIG_MODE_3);
	m_boxMode.AddString(strText);
	strText.LoadString(IDS_SYSCONFIG_MODE_4);
	m_boxMode.AddString(strText);

	strText.LoadString(IDS_SYSCONFIG_ORDER_0);
	m_boxOrder.AddString(strText);
	strText.LoadString(IDS_SYSCONFIG_ORDER_1);
	m_boxOrder.AddString(strText);
	strText.LoadString(IDS_SYSCONFIG_ORDER_2);
	m_boxOrder.AddString(strText);
	strText.LoadString(IDS_SYSCONFIG_ORDER_3);
	m_boxOrder.AddString(strText);

	strText.LoadString(IDS_SYSCONFIG_ROAM_0);
	m_boxRoam.AddString(strText);
	strText.LoadString(IDS_SYSCONFIG_ROAM_1);
	m_boxRoam.AddString(strText);
	strText.LoadString(IDS_SYSCONFIG_ROAM_2);
	m_boxRoam.AddString(strText);

	strText.LoadString(IDS_SYSCONFIG_DOMAIN_0);
	m_boxDomain.AddString(strText);
	strText.LoadString(IDS_SYSCONFIG_DOMAIN_1);
	m_boxDomain.AddString(strText);
	strText.LoadString(IDS_SYSCONFIG_DOMAIN_2);
	m_boxDomain.AddString(strText);
	strText.LoadString(IDS_SYSCONFIG_DOMAIN_3);
	m_boxDomain.AddString(strText);
	strText.LoadString(IDS_SYSCONFIG_DOMAIN_4);
	m_boxDomain.AddString(strText);


	m_boxMode.SetCurSel(0);
	m_boxOrder.SetCurSel(0);
	m_boxRoam.SetCurSel(0);
	m_boxDomain.SetCurSel(0);
	
	m_lstRead.InsertItem(0, _T(""));


	m_bInit = TRUE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSysconfigPage::ShowRespondContent(LPCSTR szRet)
{
	if(!m_bInit)
	{
		return;
	}
	CString strText = szRet;
	int nIndex = -1;

	int nMode = 0;
	int nOrder = 0;
	int nRoam = 0;
	int nDomain = 0;

	if(strText.Find(_T("^SYSCONFIG:")) != -1)
	{
		nIndex = strText.Find(_T("^SYSCONFIG:"));
		
		strText.Delete(0, nIndex + 11);
		
		nIndex = strText.Find(_T(","));
		if(nIndex != -1)
		{
#ifdef _UNICODE
			swscanf(strText.Left(nIndex), _T("%d"), &nMode);
#else
			sscanf(strText.Left(nIndex), _T("%d"), &nMode);
#endif
			strText.Delete(0, nIndex + 1);
			nIndex = strText.Find(_T(","));
			if(nIndex != -1)
			{
#ifdef _UNICODE
				swscanf(strText.Left(nIndex), _T("%d"), &nOrder);
#else
				sscanf(strText.Left(nIndex), _T("%d"), &nOrder);
#endif
			}
			strText.Delete(0, nIndex + 1);
			nIndex = strText.Find(_T(","));
			if(nIndex != -1)
			{
#ifdef _UNICODE
				swscanf(strText.Left(nIndex), _T("%d"), &nRoam);
#else
				sscanf(strText.Left(nIndex), _T("%d"), &nRoam);
#endif
			}
			strText.Delete(0, nIndex + 1);
#ifdef _UNICODE
			swscanf(strText, _T("%d"), &nDomain);
#else
			sscanf(strText, _T("%d"), &nDomain);
#endif
		}
		CString strItem;
		strItem = MapIndexToString(SYSCONFIG_MODE, nMode);	
		m_lstRead.SetItemText(0, 0, strItem);
		strItem = MapIndexToString(SYSCONFIG_ORDER, nOrder);	
		m_lstRead.SetItemText(0, 1, strItem);
		strItem = MapIndexToString(SYSCONFIG_ROAM, nRoam);	
		m_lstRead.SetItemText(0, 2, strItem);
		strItem = MapIndexToString(SYSCONFIG_DOMAIN, nDomain);	
		m_lstRead.SetItemText(0, 3, strItem);
	}
	
	UpdateData(FALSE);
}

void CSysconfigPage::SendCheckRequest()
{
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		theApp.m_pEtionFactory->SendATCmd(SYSCONFIG_READ, 0, NULL, 0);
	}
}

void CSysconfigPage::SendSetRequest()
{
	UpdateData();

	if(theApp.m_pEtionFactory != NULL  && theApp.m_bConnected)
	{
		char szRet[MAX_PATH] = {0};	
		TCHAR szPara[MAX_PATH] = {0};
		
		int nIndexMode = m_boxMode.GetCurSel();
		if(nIndexMode == 0)
		{
			nIndexMode = 2;
		}
		else if(nIndexMode == 1)
		{
			nIndexMode = 13;
		}
		else if(nIndexMode == 2)
		{
			nIndexMode = 14;
		}
		else if(nIndexMode == 3)
		{
			nIndexMode = 15;
		}
		else
		{
			nIndexMode = 16;
		}
		int nIndexOrder = m_boxOrder.GetCurSel();
		int nIndexRoam = m_boxRoam.GetCurSel();
		int nIndexDomain = m_boxDomain.GetCurSel();
		
#ifdef _UNICODE
		swprintf(szPara, _T("%d,%d,%d,%d"), nIndexMode, nIndexOrder, nIndexRoam, nIndexDomain);
#else
		sprintf(szPara, _T("%d,%d,%d,%d"), nIndexMode, nIndexOrder, nIndexRoam, nIndexDomain);
#endif
		LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(SYSCONFIG_SET, 0, (LPVOID)szPara, _tcslen(szPara));
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

			CString strText = MapIndexToString(SYSCONFIG_MODE, nIndexMode);
			m_lstRead.SetItemText(0, 0, strText);
			strText = MapIndexToString(SYSCONFIG_ORDER, nIndexOrder);
			m_lstRead.SetItemText(0, 1, strText);
			strText = MapIndexToString(SYSCONFIG_ROAM, nIndexRoam);
			m_lstRead.SetItemText(0, 2, strText);
			strText = MapIndexToString(SYSCONFIG_DOMAIN, nIndexDomain);
			m_lstRead.SetItemText(0, 3, strText);
			
			UpdateData(FALSE);
		}
		
		if(szRet[0] == '\0')
		{
			AfxMessageBox(_T("Time Out!"));
			UpdateData(FALSE);
		}
		
	}
}

CString CSysconfigPage::MapIndexToString(UINT uIndexType, UINT uIndexItem)
{
	CString strText;

	switch(uIndexType) 
	{
	case SYSCONFIG_MODE:
		if(uIndexItem == 2)
		{
			strText.LoadString(IDS_SYSCONFIG_MODE_0);
		}
		else if(uIndexItem == 13)
		{
			strText.LoadString(IDS_SYSCONFIG_MODE_1);
		}
		else if(uIndexItem == 14)
		{
			strText.LoadString(IDS_SYSCONFIG_MODE_2);
		}
		else if(uIndexItem == 15)
		{
			strText.LoadString(IDS_SYSCONFIG_MODE_3);
		}
		else
		{
			strText.LoadString(IDS_SYSCONFIG_MODE_4);
		}
		break;
	case SYSCONFIG_ORDER:
		if(uIndexItem == 0)
		{
			strText.LoadString(IDS_SYSCONFIG_ORDER_0);
		}
		else if(uIndexItem == 1)
		{
			strText.LoadString(IDS_SYSCONFIG_ORDER_1);
		}
		else if(uIndexItem == 2)
		{
			strText.LoadString(IDS_SYSCONFIG_ORDER_2);
		}
		else
		{
			strText.LoadString(IDS_SYSCONFIG_ORDER_3);
		}
		break;
	case SYSCONFIG_ROAM:
		if(uIndexItem == 0)
		{
			strText.LoadString(IDS_SYSCONFIG_ROAM_0);
		}
		else if(uIndexItem == 1)
		{
			strText.LoadString(IDS_SYSCONFIG_ROAM_1);
		}
		else
		{
			strText.LoadString(IDS_SYSCONFIG_ROAM_2);
		}
		break;
	case SYSCONFIG_DOMAIN:
		if(uIndexItem == 0)
		{
			strText.LoadString(IDS_SYSCONFIG_DOMAIN_0);
		}
		else if(uIndexItem == 1)
		{
			strText.LoadString(IDS_SYSCONFIG_DOMAIN_1);
		}
		else if(uIndexItem == 2)
		{
			strText.LoadString(IDS_SYSCONFIG_DOMAIN_2);
		}
		else if(uIndexItem == 3)
		{
			strText.LoadString(IDS_SYSCONFIG_DOMAIN_3);
		}
		else
		{
			strText.LoadString(IDS_SYSCONFIG_DOMAIN_4);
		}
		break;
	default:
		break;
	}
	return strText;
}

void CSysconfigPage::ClearAll()
{
	m_boxMode.SetCurSel(0);
	m_boxOrder.SetCurSel(0);
	m_boxRoam.SetCurSel(0);
	m_boxDomain.SetCurSel(0);

	
	for(int i = 0; i < 4; i++)
	{
		m_lstRead.SetItemText(0, i, _T(""));
	}
	UpdateData(FALSE);
}

void CSysconfigPage::OnBtnReadSysconfig() 
{
	// TODO: Add your control notification handler code here
	SendCheckRequest();
}

void CSysconfigPage::OnBtnSetSysconfig() 
{
	// TODO: Add your control notification handler code here
	SendSetRequest();
}

HBRUSH CSysconfigPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nCtlColor);

	pDC->SetBkColor( BK_COLOR );

	// TODO: Return a different brush if the default is not desired
	return (HBRUSH)(m_brshDlgBckGrnd.m_hObject);
}
