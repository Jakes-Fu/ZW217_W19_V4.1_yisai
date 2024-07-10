// DlgOthers.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "DlgOthers.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOthers dialog


CDlgOthers::CDlgOthers(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOthers::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOthers)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nUplmnIndex = 0;
	m_strPlmn.Empty();

	VERIFY( m_brshDlgBckGrnd.CreateSolidBrush(BK_COLOR) );
}


void CDlgOthers::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOthers)
	DDX_Control(pDX, IDC_LIST_UPLMN, m_lstUplmn);
	DDX_Control(pDX, IDC_COMBO_INDEX, m_boxIndex);
	DDX_Text(pDX, IDC_EDT_UPLMN, m_strPlmn);
	DDV_MaxChars(pDX, m_strPlmn, 6);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_boxType);	
	DDX_Control(pDX, IDC_BTN_UPLMN_APPLY, m_btnApplyPlmn);
	DDX_Control(pDX, IDC_BTN_AOC_APPLY, m_btnApplyAoc);
	DDX_Control(pDX, IDC_RADIO_AOC_ON, m_btnAOCOn);
	DDX_Control(pDX, IDC_RADIO_AOC_OFF, m_btnAOCOff);
	DDX_Control(pDX, IDC_RADIO_R4, m_btnR4);
	DDX_Control(pDX, IDC_RADIO_R5, m_btnR5);
	DDX_Control(pDX, IDC_RADIO_R7, m_btnR7);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOthers, CDialog)
	//{{AFX_MSG_MAP(CDlgOthers)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_UPLMN_APPLY, OnBtnUplmnApply)
	ON_BN_CLICKED(IDC_BTN_AOC_APPLY, OnBtnAocApply)
	ON_BN_CLICKED(IDC_BTN_VERSION_APPLY, OnBtnVersionApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOthers message handlers

BOOL CDlgOthers::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	DWORD dwStyle = m_lstUplmn.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
    m_lstUplmn.SetExtendedStyle(dwStyle);


	CString strText;
	strText.LoadString(IDS_UPLMN_INDEX);
	m_lstUplmn.InsertColumn(0,strText,LVCFMT_LEFT,60);
	strText.LoadString(IDS_UPLMN_UPLMN);
	m_lstUplmn.InsertColumn(1,strText,LVCFMT_LEFT,80);
	strText.LoadString(IDS_UPLMN_TYPE);
    m_lstUplmn.InsertColumn(2,strText,LVCFMT_LEFT,80);

	for(int i = 0; i < INDEX_NUM; i++)
	{
		CString strItem;
		strItem.Format(_T("%d"), i);
		m_boxIndex.AddString(strItem);
		m_lstUplmn.InsertItem(i, strItem);
	}
	m_boxIndex.SetCurSel(0);

	for(i =0 ; i < UPLMN_TYPE_NUM; i++)
	{
		m_boxType.AddString(UPLMN_TYPE[i]);
	}
	m_boxType.SetCurSel(0);

	SendCheckRequest();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDlgOthers::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nCtlColor);

	pDC->SetBkColor( BK_COLOR );

	// TODO: Return a different brush if the default is not desired
	return (HBRUSH)(m_brshDlgBckGrnd.m_hObject);
}

void CDlgOthers::SendCheckRequest()
{	
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		theApp.m_pEtionFactory->SendATCmd(SPTEST_READ, 0, NULL, 0);
		
		theApp.m_pEtionFactory->SendATCmd(UPLMN_CHECK, 0, NULL, 0);
	}
}

void CDlgOthers::ShowRespondContent(LPCSTR szRet)
{
	CString strText = szRet;
	strText.Replace(_T("\r\n"), _T(""));
	int nIndex = -1;

	if((nIndex = strText.Find(_T("+SPTEST:3,")) ) != -1)
	{
		strText.Delete(0, nIndex + 10);
		if(strText == _T("0"))
		{
			m_btnAOCOff.SetCheck(1);
			m_btnAOCOn.SetCheck(0);
		}
		else
		{
			m_btnAOCOff.SetCheck(0);
			m_btnAOCOn.SetCheck(1);
		}
	}


	if((nIndex = strText.Find(_T("+SPTEST:5,")) ) != -1)
	{
		strText.Delete(0, nIndex + 10);
		if(strText == _T("0"))
		{
			m_btnR4.SetCheck(1);
			m_btnR5.SetCheck(0);
			m_btnR7.SetCheck(0);
		}
		else if(strText == _T("1"))
		{
			m_btnR4.SetCheck(0);
			m_btnR5.SetCheck(1);
			m_btnR7.SetCheck(0);
		}
		else if(strText == _T("3")) //teana hu 2011.05.25
		{
			m_btnR7.SetCheck(1);
			m_btnR4.SetCheck(0);
			m_btnR5.SetCheck(0);
		}
	}

	if((nIndex = strText.Find(_T("+SPUPLMN:")) ) != -1)
	{
		strText.Delete(0, nIndex + 9);
		
		nIndex = strText.Find(_T(","));
		if(nIndex != -1)
		{
			m_lstUplmn.SetItemText(m_nUplmnIndex, 0, strText.Left(nIndex));
			strText.Delete(0, nIndex + 1);
			nIndex = strText.Find(_T(","));
			if(nIndex != -1)
			{
				//CString strPlmn = ((CMainFrame *)AfxGetMainWnd())->MapNetProvider(strText.Left(nIndex));
				m_lstUplmn.SetItemText(m_nUplmnIndex, 1, strText.Left(nIndex));
				strText.Delete(0,nIndex + 1);
				
				if(strText == _T("0"))
				{
					m_lstUplmn.SetItemText(m_nUplmnIndex, 2, _T("UTRAN"));
				}
				else
				{
					m_lstUplmn.SetItemText(m_nUplmnIndex, 2, _T("GSM"));
				}
			}
			m_nUplmnIndex ++;
		}
		
	}
	UpdateData(FALSE);
}

void CDlgOthers::SendSetRequest(UINT uFlag)
{
	UpdateData();

	if(theApp.m_pEtionFactory != NULL  && theApp.m_bConnected)
	{
		char szRet[MAX_PATH] = {0};	
		TCHAR szPara[MAX_PATH] = {0};

		if(uFlag == FOR_AOC)
		{
			if(m_btnAOCOn.GetCheck() == 1)
			{
#ifdef _UNICODE
				wcscpy(szPara, _T("1"));
#else
				strcpy(szPara, _T("1"));
#endif
				
			}
			else
			{
#ifdef _UNICODE
				wcscpy(szPara, _T("0"));
#else
				strcpy(szPara, _T("0"));
#endif
				
			}
			LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(AOC_SET, 0, (LPVOID)szPara, _tcslen(szPara));
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

		if(uFlag == FOR_PLMN)
		{
			if(m_strPlmn.GetLength() < 5 || m_strPlmn.IsEmpty())
			{
				AfxMessageBox(IDS_PLMN_PROMPT);
				CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDT_UPLMN);
				pEdit->SetSel(0,-1);
				pEdit->SetFocus();
				return;
			}
			int nLstIndex = m_boxIndex.GetCurSel();

#ifdef _UNICODE
			swprintf(szPara, _T("%d,\"%s\",%d"), nLstIndex, m_strPlmn, m_boxType.GetCurSel());
#else
			sprintf(szPara, _T("%d,\"%s\",%d"), nLstIndex, m_strPlmn, m_boxType.GetCurSel());
#endif
			LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(UPLMN_SET, 0, (LPVOID)szPara, _tcslen(szPara));
			if(lpRet == NULL)
			{
				ZeroMemory(szRet, MAX_PATH);
			}
			else
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
				
				CString strLstIndex;
				strLstIndex.Format(_T("%d"), nLstIndex);

				m_lstUplmn.SetItemText(nLstIndex, 0, strLstIndex);

				m_lstUplmn.SetItemText(nLstIndex, 1, m_strPlmn);
				if(m_boxType.GetCurSel() == 0)
				{
					m_lstUplmn.SetItemText(nLstIndex, 2, _T("UTRAN"));
				}
				else
				{
					m_lstUplmn.SetItemText(nLstIndex, 2, _T("GSM"));
				}
				UpdateData(FALSE);
			}
		}

		if(uFlag == R_VERSION)
		{
			if(m_btnR4.GetCheck() == 1)
			{
#ifdef _UNICODE
				wcscpy(szPara, _T("5,0"));
#else
				strcpy(szPara, _T("5,0"));
#endif
				
			}
			else if(m_btnR5.GetCheck() == 1)
			{
#ifdef _UNICODE
				wcscpy(szPara, _T("5,1"));
#else
				strcpy(szPara, _T("5,1"));
#endif
				
			}
			else if(m_btnR7.GetCheck() == 1) //teana hu 2011.05.25
			{
#ifdef _UNICODE
				wcscpy(szPara, _T("5,3"));
#else
				strcpy(szPara, _T("5,3"));
#endif
			}
			LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(SPTEST_SET, 0, (LPVOID)szPara, _tcslen(szPara));
			if(lpRet == NULL)
			{
				ZeroMemory(szRet, MAX_PATH);
			}
			else
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
}

void CDlgOthers::OnBtnUplmnApply() 
{
	// TODO: Add your control notification handler code here
	SendSetRequest(FOR_PLMN);
	
}

void CDlgOthers::OnBtnAocApply() 
{
	// TODO: Add your control notification handler code here
	SendSetRequest(FOR_AOC);
	
}

void CDlgOthers::ClearAll()
{
	m_nUplmnIndex = 0;
	for(int i = 0; i < INDEX_NUM; i++)
	{
		CString strText;
		strText = _T(" ");
		m_lstUplmn.SetItemText(i, 1, strText);
		m_lstUplmn.SetItemText(i, 2, strText);
	}
	m_strPlmn.Empty();

	UpdateData(FALSE);

}

BOOL CDlgOthers::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_CHAR )
	{
		
		_TCHAR cValue = (_TCHAR)(pMsg->wParam);
		
		if( cValue < _T('0') || cValue > _T('9') )
		{
			if(	cValue != VK_BACK && 
				cValue != 3 && cValue != 22 && cValue != 24) //Ctrl+c Ctrl+v Ctrl+x
			{
				return TRUE;
			}	
		}	
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgOthers::OnBtnVersionApply() 
{
	// TODO: Add your control notification handler code here
	SendSetRequest(R_VERSION);
	
}
