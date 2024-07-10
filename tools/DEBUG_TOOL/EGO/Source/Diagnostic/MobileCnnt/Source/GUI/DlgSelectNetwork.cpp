// DlgSelectNetwork.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "DlgSelectNetwork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectNetwork dialog


CDlgSelectNetwork::CDlgSelectNetwork(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelectNetwork::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelectNetwork)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strPlmn = _T("");
	m_strCurMode = _T("");

	m_bSearchPlmn = FALSE;
	m_bApply = FALSE;

	VERIFY( m_brshDlgBckGrnd.CreateSolidBrush(BK_COLOR) );
}


void CDlgSelectNetwork::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelectNetwork)
	DDX_Control(pDX,IDC_COMBO_MODE, m_boxMode);
	DDX_Control(pDX, IDC_BTD_SEL_NET_APPLY, m_btnApply);
	DDX_Control(pDX, IDC_LIST_PLMN, m_lstPlmn);
	DDX_Control(pDX, IDC_CHECK_MODIFY, m_btnModifyMode);
	DDX_Text(pDX, IDC_STC_NETWORK_MODE, m_strCurMode);
	DDX_Text(pDX, IDC_STATIC_INFO, m_strInfo);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNCREATE(CDlgSelectNetwork, CDialog)
BEGIN_MESSAGE_MAP(CDlgSelectNetwork, CDialog)
	//{{AFX_MSG_MAP(CDlgSelectNetwork)
	ON_WM_DESTROY()
#ifdef SPRD8800
	ON_WM_CTLCOLOR()
#endif
	ON_BN_CLICKED(IDC_CHECK_MODIFY, OnCheckModify)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, OnSelchangeComboMode)
	ON_BN_CLICKED(IDC_BTD_SEL_NET_APPLY, OnBtdSelNetApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectNetwork message handlers

void CDlgSelectNetwork::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

HBRUSH CDlgSelectNetwork::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nCtlColor);

	pDC->SetBkColor( BK_COLOR );

	// TODO: Return a different brush if the default is not desired
	return (HBRUSH)(m_brshDlgBckGrnd.m_hObject);
}

BOOL CDlgSelectNetwork::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strText;
	strText.LoadString(IDS_PLEASE_SELECT);
	m_boxMode.AddString(strText);
	strText.LoadString(IDS_AUTO_SELECT_NET);
	m_boxMode.AddString(strText);
	strText.LoadString(IDS_HAND_SELECT_NET);
	m_boxMode.AddString(strText);
	m_boxMode.SetCurSel(0);


	if(m_btnModifyMode.GetCheck() == 1)
	{
		m_boxMode.EnableWindow(TRUE);
		m_lstPlmn.EnableWindow(TRUE);
	}
	else
	{
		m_boxMode.EnableWindow(FALSE);
		m_lstPlmn.EnableWindow(FALSE);
	}

	SendCheckRequest();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSelectNetwork::OnCheckModify() 
{
	// TODO: Add your control notification handler code here
	if(m_btnModifyMode.GetCheck() == 1)
	{
		m_boxMode.EnableWindow(TRUE);
		m_lstPlmn.EnableWindow(TRUE);
	}
	else
	{
		m_boxMode.EnableWindow(FALSE);
		m_lstPlmn.EnableWindow(FALSE);
	}
	
}

void CDlgSelectNetwork::SendCheckRequest()
{
	UpdateData();
	if(!m_strCurMode.IsEmpty())
	{
		return;
	}
	
	if(theApp.m_pEtionFactory == NULL || !theApp.m_bConnected)
	{
		return;
	}

	theApp.m_pEtionFactory->SendATCmd(NETWORK, 0, NULL, 0);
}

void CDlgSelectNetwork::CheckPLMN()
{
	char szRet[MAX_PATH] = {0};
	
	if(theApp.m_pEtionFactory == NULL || !theApp.m_bConnected)
	{
		return;
	}

	m_bApply = FALSE;
	m_strInfo.LoadString(IDS_SEARCHING_PLMN);
	UpdateData(FALSE);
	
	CWaitCursor cursor;
	EnableWindow(FALSE);
	LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(CHECK_NETWORK, 0, NULL, 0);
	if(lpRet != NULL)
	{
		strcpy(szRet, lpRet);
	}
	
	EnableWindow(TRUE);

	if(strstr(szRet, "ERROR: 30") != NULL)
	{
		m_strInfo.LoadString(IDS_SEARCH_PLMN_FAIELD);
		UpdateData(FALSE);
	}
	else if(strstr(szRet, "ERROR: 0") != NULL)
	{
		m_strInfo.LoadString(IDS_SEARCH_PLMN_REJECT);
		UpdateData(FALSE);
	}
	else if(szRet[0] != '\0')
	{
		ShowRespondContent(szRet);
	}

}

BOOL CDlgSelectNetwork::GetInfo(LPCSTR szRet)
{
	CString strText = szRet;
	CString strTmp;

	int nIndex = strText.Find(_T("("));

	while(nIndex != -1)
	{
		int nSign = -1;

		strText.Delete(0, nIndex + 1);
		nIndex = strText.Find(_T(")"));
		if(nIndex != -1)
		{
			//teana hu 2009.07.28  //+COPS: (),,(0-4),(0-2)
			if(nIndex == 0)
			{
				return FALSE;
			}

			strTmp = strText.Left(nIndex);

			strText.Delete(0, nIndex + 1);
			nIndex = strTmp.Find(_T(","));
			if(nIndex != -1)
			{
				strTmp.Delete(0, nIndex + 1);
				nIndex = strTmp.Find(_T(","));
				if(nIndex != -1 )
				{
					strTmp.Delete(0, nIndex + 1);
					nIndex = strTmp.Find(_T(","));
					if(nIndex != -1)
					{
						strTmp.Delete(0, nIndex + 1);
						nIndex = strTmp.Find(_T(","));
						if(nIndex == -1)
						{
							m_strPlmn = strTmp.Left(nIndex);
						}
						else
						{
							m_strPlmn = strTmp.Left(nIndex);
							strTmp.Delete(0, nIndex + 1);
							nSign = _ttoi(strTmp);							
						}
					}
				}
			}
		}
		if(!m_strPlmn.IsEmpty())
		{
			CString strCtn = m_strPlmn;
			m_strPlmn.Replace(_T("\""),NULL);

			if(nSign != -1)
			{
				CString strTmpSign;
				strTmpSign.Format(_T(",%d"), nSign);
				strCtn += strTmpSign;
			}
			m_strArray.Add(strCtn);
			ChangeIdToString(nSign);
			m_lstPlmn.AddString(m_strPlmn);
			int nCount = m_lstPlmn.GetCount();
			m_lstPlmn.SetTopIndex(nCount - 1);
			if (nCount > 0)
			{
				m_lstPlmn.SetCurSel(nCount-1);
			}
		}

		nIndex = strText.Find(_T("("));
		m_strPlmn.Empty();
		
	}

	return TRUE;
	
}

void CDlgSelectNetwork::ChangeIdToString(int nSign)
{
	if(m_strPlmn.Find(_T("46000")) != -1)
	{
		if(nSign == -1 || nSign == 0 || nSign == 1)
		{
			m_strPlmn.LoadString(IDS_CMCC_GSM);
		}
		else if(nSign == 2)
		{
			m_strPlmn.LoadString(IDS_CMCC_TD);
		}
	}
	else if(m_strPlmn.Find(_T("46007")) != -1)
	{
		m_strPlmn.LoadString(IDS_CMCC_TD);
	}
	else if(m_strPlmn.Find(_T("46001")) != -1)
	{
		m_strPlmn.LoadString(IDS_CUC);
		//test
		//m_strPlmn += _T("\(forbidden\)");
		//test
	}
	else
	{
		if(nSign == -1 || nSign ==0 || nSign ==1)
		{
			m_strPlmn = m_strPlmn + _T("(GSM)");
		}
		else if(nSign == 2)
		{
			m_strPlmn = m_strPlmn + _T("(TD)");
		}
	}
	
}

void CDlgSelectNetwork::ShowRespondContent(LPCSTR szRet)
{
	CString strRet = szRet;
	if(strRet.Find(_T("+COPS: (")) != -1)
	{
		BOOL bRet = GetInfo(szRet);
		if(bRet)
		{
			m_strInfo.LoadString(IDS_SEARCH_FINISH);
		}
		else
		{
			m_strInfo.LoadString(IDS_SEARCH_PLMN_FAIELD); //for +COPS: (),,(0-4),(0-2)
		}
		
		UpdateData(FALSE);
	}
	else
	{
		int nIndex = strRet.Find(_T(","));
		
		if(nIndex != -1)
		{
			CString strTmp = strRet.Left(nIndex);
			UINT uCurNet;
#ifdef _UNICODE
			swscanf(strTmp.Right(1),_T("%d"), &uCurNet);
#else
			sscanf(strTmp.Right(1),_T("%d"), &uCurNet);
#endif
			m_strCurMode = GetNetWorkMode(uCurNet);
			UpdateData(FALSE);
		}
		else
		{
			nIndex = strRet.Find(_T(":"));
			if(nIndex != -1)
			{
				UINT uCurNet;
#ifdef _UNICODE
				swscanf(strRet.Right(1),_T("%d"), &uCurNet);
#else
				sscanf(strRet.Right(1),_T("%d"), &uCurNet);
#endif
				m_strCurMode = GetNetWorkMode(uCurNet);
				UpdateData(FALSE);
			}
		}
		
	}
}

CString CDlgSelectNetwork::GetNetWorkMode(UINT uID)
{
	CString strCurMode;

	switch(uID)
	{
	case 0:
		strCurMode.LoadString(IDS_AUTO_SELECT_NET);
		break;
	case 1:
		strCurMode.LoadString(IDS_HAND_SELECT_NET);
		break;
	case 2:
		strCurMode.LoadString(IDS_NETWORK_MODE_2);
		break;
	case 3:
		strCurMode.LoadString(IDS_NETWORK_MODE_3);
		break;
	case 4:
		strCurMode.LoadString(IDS_NETWORK_MODE_4);
		break;
	default:
		break;
	}
	return strCurMode;
}

void CDlgSelectNetwork::OnSelchangeComboMode() 
{
	// TODO: Add your control notification handler code here
	int nCount  = m_lstPlmn.GetCount();
	for(int i = nCount - 1; i >= 0; i--)
	{
		m_lstPlmn.DeleteString(i);
	}
	m_strArray.RemoveAll();
	if(m_boxMode.GetCurSel() == 1)
	{
		if(m_bSearchPlmn)
		{
			m_bSearchPlmn = FALSE;
		}
	}
	if(m_boxMode.GetCurSel() == 2)
	{
		m_bSearchPlmn = TRUE;
		CheckPLMN();
		
	}
	
}

void CDlgSelectNetwork::OnBtdSelNetApply() 
{
	// TODO: Add your control notification handler code here
	char szRet[MAX_PATH] = {0};
	CString strText;
	UpdateData();
	
	if(theApp.m_pEtionFactory == NULL || !theApp.m_bConnected)
	{
		return;
	}

	if(m_btnModifyMode.GetCheck() == 1)
	{
		if(m_boxMode.GetCurSel() == 0)
		{
			m_strInfo.LoadString(IDS_PLEASE_SELECT_MODE);
			UpdateData(FALSE);
			return;
		}
		m_bApply = TRUE;

		if(m_boxMode.GetCurSel() == 1)
		{
			m_strInfo.LoadString(IDS_DOING_SELECT_NET);
			UpdateData(FALSE);
			CWaitCursor cursor;
			EnableWindow(FALSE);
			LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(AUTO_SELECT_NETWORK, 0, NULL, 0);
			if(lpRet != NULL)
			{
				strcpy(szRet, lpRet);
			}
			
			EnableWindow(TRUE);

			if(szRet[0] == '\0')
			{
				m_strInfo = _T("time out");
				UpdateData(FALSE);
			}
		}
		
		if(m_boxMode.GetCurSel() == 2)
		{
			TCHAR szPara[MAX_PATH] = {0};
			if(m_lstPlmn.GetCount() == 0)
			{
				m_strInfo.LoadString(IDS_NO_PLMN);
				UpdateData(FALSE);
				return;
			}
			m_strInfo.LoadString(IDS_DOING_SELECT_NET);
			UpdateData(FALSE);

			int nIndex = m_lstPlmn.GetCurSel();

			m_strPlmn = m_strArray.GetAt(nIndex);

			//test
//			if(m_strPlmn == _T("\"46001\""))
//			{
//				CString strPrompt;
//				strPrompt.LoadString(IDS_CUC_FORBIDDEN);
//				AfxMessageBox(strPrompt);
//				return;
//			}
			//test
			
			_tcscpy(szPara, m_strPlmn);
			CWaitCursor cursor;
			EnableWindow(FALSE);
			LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(HAND_SELECT_NETWORK, 0, (LPVOID)szPara, _tcslen(szPara));
			if(lpRet == NULL)
			{
				ZeroMemory(szRet, MAX_PATH);
			}
			else
			{
				strcpy(szRet, lpRet);
			}
			
			EnableWindow(TRUE);
			if(szRet[0] == '\0')
			{
				m_strInfo = _T("time out");
				UpdateData(FALSE);
			}
		}

		if(strstr(szRet, "ERROR") != NULL)
		{
			m_strInfo.LoadString(IDS_SELECTNET_FAILED);

		}
		if(strstr(szRet, "OK") != NULL)
		{
			m_strInfo.LoadString(IDS_SELECTNET_SUCCESS);
			m_strCurMode = GetNetWorkMode(m_boxMode.GetCurSel() - 1);
		}
		UpdateData(FALSE);
	}
	
}

void CDlgSelectNetwork::ClearAll()
{
	m_strPlmn = _T("");
	m_strCurMode = _T("");
	m_strInfo.Empty();

	m_bSearchPlmn = FALSE;

	int nCount  = m_lstPlmn.GetCount();
	for(int i = nCount -  1; i >= 0; i--)
	{
		m_lstPlmn.DeleteString(i);
	}
	m_strArray.RemoveAll();

	m_boxMode.SetCurSel(0);

	m_btnModifyMode.SetCheck(0);
	m_boxMode.EnableWindow(FALSE);
	m_lstPlmn.EnableWindow(FALSE);

	UpdateData(FALSE);
}
