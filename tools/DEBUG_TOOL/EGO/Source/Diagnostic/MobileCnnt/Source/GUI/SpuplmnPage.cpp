// SpuplmnPage.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "SpuplmnPage.h"
#include "DlgOthers.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpuplmnPage property page

IMPLEMENT_DYNCREATE(CSpuplmnPage, CPropertyPage)

CSpuplmnPage::CSpuplmnPage() : CPropertyPage(CSpuplmnPage::IDD)
{
	//{{AFX_DATA_INIT(CSpuplmnPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bInit = FALSE;

	VERIFY( m_brshDlgBckGrnd.CreateSolidBrush(BK_COLOR) );
}

CSpuplmnPage::~CSpuplmnPage()
{
}

void CSpuplmnPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpuplmnPage)
	DDX_Control(pDX, IDC_LST_UPLMN_READ, m_lstUplmn);
	DDX_Control(pDX, IDC_COMBO_UPLMN_INDEX, m_boxIndex);
	DDX_Text(pDX, IDC_EDT_UPLMN_PLMN, m_strPlmn);
	DDV_MaxChars(pDX, m_strPlmn, 6);
	DDX_Control(pDX, IDC_COMBO_UPLMN_TYPE, m_boxType);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpuplmnPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSpuplmnPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_READ_UPLMN, OnBtnReadUplmn)
	ON_BN_CLICKED(IDC_BTN_SET_UPLMN, OnBtnSetUplmn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpuplmnPage message handlers

HBRUSH CSpuplmnPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nCtlColor);

	pDC->SetBkColor( BK_COLOR );

	// TODO: Return a different brush if the default is not desired
	return (HBRUSH)(m_brshDlgBckGrnd.m_hObject);
}

BOOL CSpuplmnPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
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
		CString StrItem;
		StrItem.Format(_T("%d"), i);
		m_boxIndex.AddString(StrItem);
		m_lstUplmn.InsertItem(i, StrItem);
	}
	m_boxIndex.SetCurSel(0);

	int nCount = sizeof(UPLMN_TYPE)/sizeof(UPLMN_TYPE[0]);

	for(i =0 ; i < nCount; i++)
	{
		m_boxType.AddString(UPLMN_TYPE[i]);
	}
	m_boxType.SetCurSel(0);
	
	m_bInit = TRUE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSpuplmnPage::OnBtnReadUplmn() 
{
	// TODO: Add your control notification handler code here
	SendCheckRequest();
	
}

void CSpuplmnPage::OnBtnSetUplmn() 
{
	// TODO: Add your control notification handler code here
	SendSetRequest();
	
}

void CSpuplmnPage::SendCheckRequest()
{	
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{	
		theApp.m_pEtionFactory->SendATCmd(UPLMN_CHECK, 0, NULL, 0);
	}
}

void CSpuplmnPage::ShowRespondContent(LPCSTR szRet)
{
	if(!m_bInit)
	{
		return;
	}
	CString strText = szRet;
	int nIndex = -1;

	if((nIndex = strText.Find(_T("+SPUPLMN:")) ) != -1)
	{
		strText.Delete(0, nIndex + 9);
		
		nIndex = strText.Find(_T(","));
		if(nIndex != -1)
		{
			UINT nIndexUplmn = 0;
#ifdef _UNICODE
			swscanf(strText.Left(nIndex), _T("%d"), &nIndexUplmn);
#else
			sscanf(strText.Left(nIndex), _T("%d"), &nIndexUplmn);
#endif
			m_lstUplmn.SetItemText(nIndexUplmn, 0, strText.Left(nIndex));
			strText.Delete(0, nIndex + 1);
			nIndex = strText.Find(_T(","));
			if(nIndex != -1)
			{
				//CString strPlmn = ((CMainFrame *)AfxGetMainWnd())->MapNetProvider(strText.Left(nIndex));
				m_lstUplmn.SetItemText(nIndexUplmn, 1, strText.Left(nIndex));
				strText.Delete(0,nIndex + 1);
				
				if(strText == _T("0"))
				{
					m_lstUplmn.SetItemText(nIndexUplmn, 2, _T("UTRAN"));
				}
				else
				{
					m_lstUplmn.SetItemText(nIndexUplmn, 2, _T("GSM"));
				}
			}
		}
		
	}
	UpdateData(FALSE);
}

void CSpuplmnPage::SendSetRequest()
{
	UpdateData();

	if(m_strPlmn.GetLength() < 5 || m_strPlmn.IsEmpty())
	{
		AfxMessageBox(IDS_PLMN_PROMPT);
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDT_UPLMN_PLMN);
		pEdit->SetSel(0,-1);
		pEdit->SetFocus();
		return;
	}

	if(theApp.m_pEtionFactory != NULL  && theApp.m_bConnected)
	{
		char szRet[MAX_PATH] = {0};	
		TCHAR szPara[MAX_PATH] = {0};
		
		int nLstIndex = m_boxIndex.GetCurSel();
		
		
#ifdef _UNICODE
		swprintf(szPara, _T("%d,\"%s\",%d"), nLstIndex, m_strPlmn, m_boxType.GetCurSel());
#else
		sprintf(szPara, _T("%d,\"%s\",%d"), nLstIndex, m_strPlmn, m_boxType.GetCurSel());
#endif
		LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(UPLMN_SET, 0, (LPVOID)szPara, _tcslen(szPara));
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
}

void CSpuplmnPage::ClearAll()
{
	for(int i = 0; i < INDEX_NUM; i++)
	{
		CString strText;
		strText = _T(" ");
		m_lstUplmn.SetItemText(i, 1, strText);
		m_lstUplmn.SetItemText(i, 2, strText);
	}

	UpdateData(FALSE);

}

BOOL CSpuplmnPage::PreTranslateMessage(MSG* pMsg) 
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
	
	return CPropertyPage::PreTranslateMessage(pMsg);
}
