// DlgTDGSM.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "DlgTDGSM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTDGSM dialog


CDlgTDGSM::CDlgTDGSM(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTDGSM::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTDGSM)
		// NOTE: the ClassWizard will add member initialization here
	m_nRoam = 0;
	m_nDomain = 0;
	//}}AFX_DATA_INIT

	VERIFY( m_brshDlgBckGrnd.CreateSolidBrush(BK_COLOR) );
}


void CDlgTDGSM::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTDGSM)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_COMBO_TDGSM_MODE, m_boxMode);
	DDX_Control(pDX, IDC_COMBO_TDGSM_ORDER, m_boxOrder);
	DDX_Control(pDX, IDC_BTN_APPLY_SYSCONFIG, m_btnApply);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTDGSM, CDialog)
	//{{AFX_MSG_MAP(CDlgTDGSM)
	ON_BN_CLICKED(IDC_BTN_APPLY_SYSCONFIG, OnBtnApplySysconfig)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTDGSM message handlers

BOOL CDlgTDGSM::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
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

	m_boxMode.SetCurSel(0);
	m_boxOrder.SetCurSel(0);

	SendCheckRequest();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTDGSM::ShowRespondContent(LPCSTR szRet)
{
	CString strText = szRet;
	int nIndex = -1;

	int nMode = 0;
	int nOrder = 0;

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
				swscanf(strText.Left(nIndex), _T("%d"), &m_nRoam);
#else
				sscanf(strText.Left(nIndex), _T("%d"), &m_nRoam);
#endif
			}
			strText.Delete(0, nIndex + 1);
#ifdef _UNICODE
			swscanf(strText, _T("%d"), &m_nDomain);
#else
			sscanf(strText, _T("%d"), &m_nDomain);
#endif
		}
	}

	switch(nMode) {
	case 2:
		m_boxMode.SetCurSel(0);
		break;
	case 13:
		m_boxMode.SetCurSel(1);
		break;
	case 14:
		m_boxMode.SetCurSel(2);
		break;
	case 15:
		m_boxMode.SetCurSel(3);
		break;
	case 16:
		m_boxMode.SetCurSel(4);
		break;
	default:
		break;
	}
	m_boxOrder.SetCurSel(nOrder);
	
	UpdateData(FALSE);
}

void CDlgTDGSM::SendCheckRequest()
{
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		theApp.m_pEtionFactory->SendATCmd(SYSCONFIG_READ, 0, NULL, 0);
	}
}

void CDlgTDGSM::SendSetRequest()
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
		
#ifdef _UNICODE
		swprintf(szPara, _T("%d,%d,%d,%d"), nIndexMode, nIndexOrder, m_nRoam, m_nDomain);
#else
		sprintf(szPara, _T("%d,%d,%d,%d"), nIndexMode, nIndexOrder, m_nRoam, m_nDomain);
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
			UpdateData(FALSE);
		}
		if(szRet[0] == '\0')
		{
			AfxMessageBox(_T("Time Out!"));
			UpdateData(FALSE);
		}
		
	}
}


void CDlgTDGSM::ClearAll()
{
	m_nDomain = 0;
	m_nRoam = 0;
	m_boxMode.SetCurSel(0);
	m_boxOrder.SetCurSel(0);
}

void CDlgTDGSM::OnBtnApplySysconfig() 
{
	// TODO: Add your control notification handler code here
	SendSetRequest();
}

HBRUSH CDlgTDGSM::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nCtlColor);

	pDC->SetBkColor( BK_COLOR );

	// TODO: Return a different brush if the default is not desired
	return (HBRUSH)(m_brshDlgBckGrnd.m_hObject);
}
