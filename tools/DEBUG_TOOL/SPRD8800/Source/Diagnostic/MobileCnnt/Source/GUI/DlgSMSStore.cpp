// DlgSMSStore.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "DlgSMSStore.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSMSStore dialog


CDlgSMSStore::CDlgSMSStore(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSMSStore::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSMSStore)
		// NOTE: the ClassWizard will add member initialization here
	m_uRDUsed = 0;
	m_uRDMax = 0;
	m_uWSUsed = 0;
	m_uWSMax = 0;
	//}}AFX_DATA_INIT
}


void CDlgSMSStore::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSMSStore)
	DDX_Control(pDX, IDC_COMBO_RD, m_boxRD);
	DDX_Control(pDX, IDC_COMBO_WS, m_boxWS);
	DDX_Text(pDX, IDC_EDT_RD_USED, m_uRDUsed);
	DDX_Text(pDX, IDC_EDT_RD_MAX, m_uRDMax);
	DDX_Text(pDX, IDC_EDT_WS_USED, m_uWSUsed);
	DDX_Text(pDX, IDC_EDT_WS_MAX, m_uWSMax);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSMSStore, CDialog)
	//{{AFX_MSG_MAP(CDlgSMSStore)
	ON_CBN_SELCHANGE(IDC_COMBO_RD, OnSelchangeComboRd)
	ON_CBN_SELCHANGE(IDC_COMBO_WS, OnSelchangeComboWs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSMSStore message handlers

void CDlgSMSStore::OnOK() 
{
	// TODO: Add extra validation here
	int nCurSelRD = m_boxRD.GetCurSel();
	int nCurSelWS = m_boxWS.GetCurSel();


	if(SendSetRequest((SMS_STORE_PLACE_E)nCurSelRD, (SMS_STORE_PLACE_E)nCurSelWS))
	{
		CDialog::OnOK();
	}
	else
	{
		AfxMessageBox(IDS_SET_FAILED);
	}

	
//	CDialog::OnOK();
}

BOOL CDlgSMSStore::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CString strText = "BM";
	m_boxRD.AddString(strText);
	m_boxWS.AddString(strText);
	strText = "SM";
	m_boxRD.AddString(strText);
	m_boxWS.AddString(strText);

	m_boxRD.SetCurSel(0);
	m_boxWS.SetCurSel(0);

	SendCheckRequest();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSMSStore::SendCheckRequest()
{
	CString strRet;
	if(theApp.m_pSMSFactory != NULL && theApp.m_bConnected)
	{
		strRet = theApp.m_pSMSFactory->QueryStoreCapacity();
		if(!strRet.IsEmpty())
		{
			ParseCPMS(strRet);
			UpdateData(FALSE);
		}
	}
}

void CDlgSMSStore::ParseCPMS(CString strCPMS)
{
	CString strTmp;
	int nIndex = strCPMS.Find(_T(","));
	if(nIndex != -1)
	{
		strTmp = strCPMS.Left(nIndex);
		if(strTmp.Find(_T("SM")) != -1)
		{
			m_boxRD.SetCurSel(1);
		}

		if(strTmp.Find(_T("BM")) != -1)
		{
			m_boxRD.SetCurSel(0);
		}
		strCPMS.Delete(0, nIndex + 1);
		nIndex = strCPMS.Find(_T(","));
		if(nIndex != -1)
		{
			strTmp = strCPMS.Left(nIndex);
#ifdef _UNICODE
			wscanf(strTmp, _T("%d"), &m_uRDUsed);
#else
			sscanf(strTmp, _T("%d"), &m_uRDUsed);
#endif
			strCPMS.Delete(0, nIndex + 1);
			nIndex = strCPMS.Find(_T(","));
			if(nIndex != -1)
			{
				strTmp = strCPMS.Left(nIndex);
#ifdef _UNICODE
				wscanf(strTmp, _T("%d"), &m_uRDMax);
#else
				sscanf(strTmp, _T("%d"), &m_uRDMax);
#endif
				
				strCPMS.Delete(0, nIndex + 1);
				nIndex = strCPMS.Find(_T(","));
				if(nIndex != -1)
				{
					strTmp = strCPMS.Left(nIndex);
					if(strTmp.Find(_T("SM")) != -1)
					{
						m_boxWS.SetCurSel(1);
					}
					
					if(strTmp.Find(_T("BM")) != -1)
					{
						m_boxWS.SetCurSel(0);
					}

					strCPMS.Delete(0, nIndex + 1);
					nIndex = strCPMS.Find(_T(","));
					if(nIndex != -1)
					{
						strTmp = strCPMS.Left(nIndex);
#ifdef _UNICODE
						wscanf(strTmp, _T("%d"), &m_uWSUsed);
#else
						sscanf(strTmp, _T("%d"), &m_uWSUsed);
#endif
						
						strCPMS.Delete(0, nIndex + 1);
#ifdef _UNICODE
						wscanf(strCPMS, _T("%d"), &m_uWSMax);
#else
						sscanf(strCPMS, _T("%d"), &m_uWSMax);
#endif
						
					}
					
				}
			}
		}
	}
}


BOOL CDlgSMSStore::SendSetRequest(SMS_STORE_PLACE_E eStorePlaceRD,  SMS_STORE_PLACE_E eStorePlaceWS)
{
	if(theApp.m_pSMSFactory != NULL && theApp.m_bConnected)
	{
		return theApp.m_pSMSFactory->SelectStorePlaceAll(eStorePlaceRD, eStorePlaceWS);
	}
	return FALSE;
}

void CDlgSMSStore::OnSelchangeComboRd() 
{
	// TODO: Add your control notification handler code here
	int nCurSelRD = m_boxRD.GetCurSel();
	int nCurSelWS = m_boxWS.GetCurSel();


	SendSetRequest((SMS_STORE_PLACE_E)nCurSelRD, (SMS_STORE_PLACE_E)nCurSelWS);
	SendCheckRequest();
	
}

void CDlgSMSStore::OnSelchangeComboWs() 
{
	// TODO: Add your control notification handler code here
	int nCurSelRD = m_boxRD.GetCurSel();
	int nCurSelWS = m_boxWS.GetCurSel();


	SendSetRequest((SMS_STORE_PLACE_E)nCurSelRD, (SMS_STORE_PLACE_E)nCurSelWS);
	SendCheckRequest();
	
}
