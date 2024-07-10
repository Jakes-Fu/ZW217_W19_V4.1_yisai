// SptestPage.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "SptestPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSptestPage property page

IMPLEMENT_DYNCREATE(CSptestPage, CPropertyPage)

CSptestPage::CSptestPage() : CPropertyPage(CSptestPage::IDD)
{
	//{{AFX_DATA_INIT(CSptestPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bInit = FALSE;

	VERIFY( m_brshDlgBckGrnd.CreateSolidBrush(BK_COLOR) );
}

CSptestPage::~CSptestPage()
{
}

void CSptestPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSptestPage)
	DDX_Control(pDX, IDC_LST_SPTEST_READ, m_lstRead);
	DDX_Control(pDX, IDC_COMBO_SPTEST_TYPE, m_boxType);
	DDX_Control(pDX, IDC_COMBO_SPTEST_ITEM, m_boxItem);
	DDX_Control(pDX, IDC_BTN_READ_SPTEST, m_btnRead);
	DDX_Control(pDX, IDC_BTN_SET_SPTEST, m_btnSet);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSptestPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSptestPage)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_SPTEST_TYPE, OnSelchangeComboSptestType)
	ON_BN_CLICKED(IDC_BTN_READ_SPTEST, OnBtnReadSptest)
	ON_BN_CLICKED(IDC_BTN_SET_SPTEST, OnBtnSetSptest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSptestPage message handlers

HBRUSH CSptestPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nCtlColor);

	pDC->SetBkColor( BK_COLOR );

	// TODO: Return a different brush if the default is not desired
	return (HBRUSH)(m_brshDlgBckGrnd.m_hObject);
}

BOOL CSptestPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	DWORD dwStyle = m_lstRead.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	m_lstRead.SetExtendedStyle(dwStyle);


	CString strText;

	strText.LoadString(IDS_SPTEST_FTA);
	m_lstRead.InsertColumn(0,strText,LVCFMT_LEFT,90);
	m_boxType.AddString(strText);
	strText.LoadString(IDS_SPTEST_PROTECT);
	m_lstRead.InsertColumn(1,strText,LVCFMT_LEFT,90);
	m_boxType.AddString(strText);
	strText.LoadString(IDS_SPTEST_NETWORK);
	m_lstRead.InsertColumn(2,strText,LVCFMT_LEFT,90);
	m_boxType.AddString(strText);
	strText.LoadString(IDS_SPTEST_AOC);
	m_lstRead.InsertColumn(3,strText,LVCFMT_LEFT,90);
	m_boxType.AddString(strText);
	strText.LoadString(IDS_SPTEST_GSM_SIM);
	m_lstRead.InsertColumn(4,strText,LVCFMT_LEFT,90);
	m_boxType.AddString(strText);
	strText.LoadString(IDS_SPTEST_VERSION);
	m_lstRead.InsertColumn(5,strText,LVCFMT_LEFT,90);
	m_boxType.AddString(strText);
	strText.LoadString(IDS_SPTEST_PS);
	m_lstRead.InsertColumn(6,strText,LVCFMT_LEFT,90);
	m_boxType.AddString(strText);
	strText.LoadString(IDS_SPTEST_UE);
	m_lstRead.InsertColumn(7,strText,LVCFMT_LEFT,90);
	m_boxType.AddString(strText);

	m_boxType.SetCurSel(0);
	SetItemBox(FTA);
	
	m_lstRead.InsertItem(0, _T(""));


	m_bInit = TRUE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSptestPage::SetItemBox(DWORD dwType)
{
	for(int i =0; i < m_boxItem.GetCount(); i++)
	{
		m_boxItem.DeleteString(i);
		i--;
	}

	CString strText;

	switch(dwType) 
	{
	case FTA:
		strText.LoadString(IDS_FTA_0);
		m_boxItem.AddString(strText);
		strText.LoadString(IDS_FTA_1);
		m_boxItem.AddString(strText);
		strText.LoadString(IDS_FTA_2);
		m_boxItem.AddString(strText);
		break;
	case PROTECT:
		strText.LoadString(IDS_PROTECT_0);
		m_boxItem.AddString(strText);
		strText.LoadString(IDS_PROTECT_1);
		m_boxItem.AddString(strText);
		break;
	case NETWORK_REAL:
		strText.LoadString(IDS_NETWORK_0);
		m_boxItem.AddString(strText);
		strText.LoadString(IDS_NETWORK_1);
		m_boxItem.AddString(strText);
		break;
	case AOC:
		strText.LoadString(IDS_AOC_0);
		m_boxItem.AddString(strText);
		strText.LoadString(IDS_AOC_1);
		m_boxItem.AddString(strText);
		break;
	case GSM_SIM:
		strText.LoadString(IDS_GSM_SIM_0);
		m_boxItem.AddString(strText);
		strText.LoadString(IDS_GSM_SIM_1);
		m_boxItem.AddString(strText);
		break;
	case PROTOCOL_VERSION:
		strText.LoadString(IDS_VERSION_0);
		m_boxItem.AddString(strText);
		strText.LoadString(IDS_VERSION_1);
		m_boxItem.AddString(strText);
		break;
	case PS:
		strText.LoadString(IDS_PS_0);
		m_boxItem.AddString(strText);
		strText.LoadString(IDS_PS_1);
		m_boxItem.AddString(strText);
		strText.LoadString(IDS_PS_2);
		m_boxItem.AddString(strText);
		strText.LoadString(IDS_PS_3);
		m_boxItem.AddString(strText);
		break;
	case UE:
		strText.LoadString(IDS_UE_0);
		m_boxItem.AddString(strText);
		strText.LoadString(IDS_UE_1);
		m_boxItem.AddString(strText);
		strText.LoadString(IDS_UE_2);
		m_boxItem.AddString(strText);
		break;
	default:
		break;
	}

	m_boxItem.SetCurSel(0);
}

void CSptestPage::OnSelchangeComboSptestType() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_boxType.GetCurSel();

	SetItemBox(nIndex);
	
}

void CSptestPage::SendCheckRequest()
{	
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		theApp.m_pEtionFactory->SendATCmd(SPTEST_READ, 0, NULL, 0);
	}
}

void CSptestPage::ShowRespondContent(LPCSTR szRet)
{
	if(!m_bInit)
	{
		return;
	}
	CString strText = szRet;
	int nIndex = -1;
	CString strType, strItem;

	if(strText.Find(_T("+SPTEST:")) != -1)
	{
		nIndex = strText.Find(_T("+SPTEST:"));
		
		strText.Delete(0, nIndex + 8);
		
		nIndex = strText.Find(_T(","));
		if(nIndex != -1)
		{
			strType = strText.Left(nIndex);
			strText.Delete(0, nIndex + 1);
			strItem = strText;
		}

		UINT uType, uItem;
#ifdef _UNICODE
		swscanf(strType, _T("%d"), &uType);
		swscanf(strItem, _T("%d"), &uItem);
#else
		sscanf(strType, _T("%d"), &uType);
		sscanf(strItem, _T("%d"), &uItem);
#endif
		strItem = MapIndexToString(uType, uItem);
		
		m_lstRead.SetItemText(0, uType, strItem);
	}
	
	UpdateData(FALSE);
}

void CSptestPage::SendSetRequest()
{
	UpdateData();

	if(theApp.m_pEtionFactory != NULL  && theApp.m_bConnected)
	{
		char szRet[MAX_PATH] = {0};	
		TCHAR szPara[MAX_PATH] = {0};
		
		int nIndexType = m_boxType.GetCurSel();
		int nIndexValue = m_boxItem.GetCurSel();
		
#ifdef _UNICODE
		swprintf(szPara, _T("%d,%d"), nIndexType, nIndexValue);
#else
		sprintf(szPara, _T("%d,%d"), nIndexType, nIndexValue);
#endif
		
		LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(SPTEST_SET, 0, (LPVOID)szPara, _tcslen(szPara));
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

			CString strText = MapIndexToString(nIndexType, nIndexValue);
			m_lstRead.SetItemText(0, nIndexType, strText);
			
			UpdateData(FALSE);
		}
		
	}
}

void CSptestPage::ClearAll()
{

	m_boxType.SetCurSel(0);
	m_boxItem.SetCurSel(0);

	
	for(int i = 0; i < NUM_AUTOATT; i++)
	{
		m_lstRead.SetItemText(0, i, _T(""));
	}
	UpdateData(FALSE);

}

CString CSptestPage::MapIndexToString(UINT uIndexType, UINT uIndexItem)
{
	CString strText;

	switch(uIndexType) 
	{
	case FTA:
		if(uIndexItem == 0)
		{
			strText.LoadString(IDS_FTA_0);
		}
		else if(uIndexItem == 1)
		{
			strText.LoadString(IDS_FTA_1);
		}
		else
		{
			strText.LoadString(IDS_FTA_2);
		}
		break;
	case PROTECT:
		if(uIndexItem == 0)
		{
			strText.LoadString(IDS_PROTECT_0);
		}
		else
		{
			strText.LoadString(IDS_PROTECT_1);
		}
		break;
	case NETWORK_REAL:
		if(uIndexItem == 0)
		{
			strText.LoadString(IDS_NETWORK_0);
		}
		else
		{
			strText.LoadString(IDS_NETWORK_1);
		}
		break;
	case AOC:
		if(uIndexItem == 0)
		{
			strText.LoadString(IDS_AOC_0);
		}
		else
		{
			strText.LoadString(IDS_AOC_1);
		}
		break;
	case GSM_SIM:
		if(uIndexItem == 0)
		{
			strText.LoadString(IDS_GSM_SIM_0);
		}
		else
		{
			strText.LoadString(IDS_GSM_SIM_1);
		}
		break;
	case PROTOCOL_VERSION:
		if(uIndexItem == 0)
		{
			strText.LoadString(IDS_VERSION_0);
		}
		else
		{
			strText.LoadString(IDS_VERSION_1);
		}
		break;
	case PS:
		if(uIndexItem == 0)
		{
			strText.LoadString(IDS_PS_0);
		}
		else if(uIndexItem == 1)
		{
			strText.LoadString(IDS_PS_1);
		}
		else if(uIndexItem == 2)
		{
			strText.LoadString(IDS_PS_2);
		}
		else
		{
			strText.LoadString(IDS_PS_3);
		}
		break;
	case UE:
		if(uIndexItem == 0)
		{
			strText.LoadString(IDS_UE_0);
		}
		else if(uIndexItem == 1)
		{
			strText.LoadString(IDS_UE_1);
		}
		else
		{
			strText.LoadString(IDS_UE_2);
		}
		break;
	default:
		break;
	}
	return strText;
}

void CSptestPage::OnBtnReadSptest() 
{
	// TODO: Add your control notification handler code here
	SendCheckRequest();
	
}

void CSptestPage::OnBtnSetSptest() 
{
	// TODO: Add your control notification handler code here
	SendSetRequest();
	
}
