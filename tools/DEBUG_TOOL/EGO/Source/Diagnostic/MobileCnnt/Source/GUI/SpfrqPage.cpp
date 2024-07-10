// SpfrqPage.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "SpfrqPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpfrqPage property page

IMPLEMENT_DYNCREATE(CSpfrqPage, CPropertyPage)

CSpfrqPage::CSpfrqPage() : CPropertyPage(CSpfrqPage::IDD)
{
	//{{AFX_DATA_INIT(CSpfrqPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bInit = FALSE;
	m_uFrenq = 0;
	m_uCell1 = 0;
	m_uCell2 = 0;
	m_uCell3 = 0;
	m_uIndex = 0;
	
	VERIFY( m_brshDlgBckGrnd.CreateSolidBrush(BK_COLOR) );
}

CSpfrqPage::~CSpfrqPage()
{
}

void CSpfrqPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpfrqPage)
	DDX_Control(pDX, IDC_LST_SPFRQ_READ, m_lstRead);
	DDX_Control(pDX, IDC_LST_SPFRQ_TEST, m_lstTest);
	DDX_Control(pDX, IDC_COMBO_SPFRQ_OPR, m_boxOpr);
	DDX_Control(pDX, IDC_COMBO_SPFRQ_ID, m_boxIndex);
	DDX_Control(pDX, IDC_BTN_READ_SPFRQ, m_btnRead);
	DDX_Control(pDX, IDC_BTN_TEST_SPFRQ, m_btnTest);
	DDX_Control(pDX, IDC_BTN_SET_SPFRQ, m_btnSet);
	DDX_Control(pDX, IDC_EDT_SPFRQ_FREQN, m_edtFrenq);
	DDX_Control(pDX, IDC_EDT_SPFRQ_CELL1, m_edtCell1);
	DDX_Control(pDX, IDC_EDT_SPFRQ_CELL2, m_edtCell2);
	DDX_Control(pDX, IDC_EDT_SPFRQ_CELL3, m_edtCell3);
	DDX_Text(pDX, IDC_EDT_SPFRQ_FREQN, m_uFrenq);
	DDX_Text(pDX, IDC_EDT_SPFRQ_CELL1, m_uCell1);
	DDX_Text(pDX, IDC_EDT_SPFRQ_CELL2, m_uCell2);
	DDX_Text(pDX, IDC_EDT_SPFRQ_CELL3, m_uCell3);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpfrqPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSpfrqPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_READ_SPFRQ, OnBtnReadSpfrq)
	ON_BN_CLICKED(IDC_BTN_TEST_SPFRQ, OnBtnTestSpfrq)
	ON_BN_CLICKED(IDC_BTN_SET_SPFRQ, OnBtnSetSpfrq)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpfrqPage message handlers

BOOL CSpfrqPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strText;

	
	for(int i = 0; i < 4; i++)
	{
		strText.Format(_T("%d"), i);
		m_boxIndex.AddString(strText);
	}

	m_boxIndex.SetCurSel(0);

	strText.LoadString(IDS_SPFRQ_LOCK);
	m_boxOpr.AddString(strText);
	strText.LoadString(IDS_SPFRQ_UNLOCK);
	m_boxOpr.AddString(strText);
	
	m_boxOpr.SetCurSel(0);


	DWORD dwStyle = m_lstTest.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
    m_lstTest.SetExtendedStyle(dwStyle);
	m_lstRead.SetExtendedStyle(dwStyle);

	strText.LoadString(IDS_SPFRQ_OPERATE);
	m_lstTest.InsertColumn(0,strText,LVCFMT_LEFT,70);
	strText.LoadString(IDS_SPFRQ_INDEX);
	m_lstTest.InsertColumn(1,strText,LVCFMT_LEFT,70);
	strText.LoadString(IDS_SPFRQ_FREQUENCE);
	m_lstTest.InsertColumn(2,strText,LVCFMT_LEFT,70);
	m_lstRead.InsertColumn(0,strText,LVCFMT_LEFT,70);
	strText.LoadString(IDS_SPFRQ_CELL1);
	m_lstTest.InsertColumn(3,strText,LVCFMT_LEFT,70);
	m_lstRead.InsertColumn(1,strText,LVCFMT_LEFT,70);
	strText.LoadString(IDS_SPFRQ_CELL2);
	m_lstTest.InsertColumn(4,strText,LVCFMT_LEFT,70);
	m_lstRead.InsertColumn(2,strText,LVCFMT_LEFT,70);
	strText.LoadString(IDS_SPFRQ_CELL3);
	m_lstTest.InsertColumn(5,strText,LVCFMT_LEFT,70);
	m_lstRead.InsertColumn(3,strText,LVCFMT_LEFT,70);

	for(i = 0; i < NUM_SPFRQ; i++)
	{
		m_lstRead.InsertItem(i, _T(""));
	}
	m_lstTest.InsertItem(0, _T(""));

	m_bInit = TRUE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CSpfrqPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nCtlColor);

	pDC->SetBkColor( BK_COLOR );

	// TODO: Return a different brush if the default is not desired
	return (HBRUSH)(m_brshDlgBckGrnd.m_hObject);
}

void CSpfrqPage::SendCheckRequest(UINT uFlag)
{	
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		if(uFlag == FOR_SPFRQ_READ)
		{
			theApp.m_pEtionFactory->SendATCmd(SPFRQ_READ, 0, NULL, 0);
		}

		if(uFlag == FOR_SPFRQ_TEST)
		{
			theApp.m_pEtionFactory->SendATCmd(SPFRQ_TEST, 0, NULL, 0);
		}
	}
}

void CSpfrqPage::ShowRespondContent(LPCSTR szRet)
{
	if(!m_bInit)
	{
		return;
	}
	CString strText = szRet;
	int nIndex = -1;
	CString strOper, strIndex, strFrenq, strCell1, strCell2, strCell3;

	if(strText.Find(_T("+SPFRQ:")) != -1 && strText.Find(_T("+SPFRQ:(")) == -1)
	{
		nIndex = strText.Find(_T("+SPFRQ:"));
		
		strText.Delete(0, nIndex + 7);
		
		nIndex = strText.Find(_T(","));
		if(nIndex != -1)
		{
			strFrenq = strText.Left(nIndex);
			strText.Delete(0, nIndex + 1);
			nIndex = strText.Find(_T(","));
			if(nIndex != -1)
			{
				strCell1 = strText.Left(nIndex);
				strText.Delete(0, nIndex + 1);
				nIndex = strText.Find(_T(","));
				if(nIndex != -1)
				{
					strCell2 = strText.Left(nIndex);
					strText.Delete(0, nIndex + 1);
					strCell3 = strText;
				}
			}
		}

		m_lstRead.SetItemText(m_uIndex, 0, strFrenq);
		m_lstRead.SetItemText(m_uIndex, 1, strCell1);
		m_lstRead.SetItemText(m_uIndex, 2, strCell2);
		m_lstRead.SetItemText(m_uIndex, 3, strCell3);
		
		m_uIndex ++;
	}

	if((nIndex = strText.Find(_T("+SPFRQ:("))) != -1)
	{
		strText.Delete(0, nIndex + 7);
		nIndex = strText.Find(_T(","));
		if(nIndex != -1)
		{
			strOper = strText.Left(nIndex);
			strText.Delete(0, nIndex + 1);
			nIndex = strText.Find(_T(")"));
			if(nIndex != -1)
			{
				strIndex = strText.Left(nIndex + 1);
				strText.Delete(0, nIndex + 2);
				nIndex = strText.Find(_T(")"));
				if(nIndex != -1)
				{
					strFrenq = strText.Left(nIndex + 1);
					strText.Delete(0, nIndex + 2);
					nIndex = strText.Find(_T(")"));
					if(nIndex != -1)
					{
						strCell1 = strText.Left(nIndex + 1);
						strText.Delete(0, nIndex + 2);
						nIndex = strText.Find(_T(")"));
						if(nIndex != -1)
						{
							strCell2 = strText.Left(nIndex + 1);
							strText.Delete(0, nIndex + 2);
							strCell3 = strText;
						}
					}
				}
			}
		}
		m_lstTest.SetItemText(0, 0, strOper);
		m_lstTest.SetItemText(0, 1, strIndex);
		m_lstTest.SetItemText(0, 2, strFrenq);
		m_lstTest.SetItemText(0, 3, strCell1);
		m_lstTest.SetItemText(0, 4, strCell2);
		m_lstTest.SetItemText(0, 5, strCell3);

	}
	UpdateData(FALSE);
}

void CSpfrqPage::SendSetRequest()
{
	UpdateData();

	if(theApp.m_pEtionFactory != NULL  && theApp.m_bConnected)
	{
		char szRet[MAX_PATH] = {0};	
		TCHAR szPara[MAX_PATH] = {0};
		
		int nIndexOper = m_boxOpr.GetCurSel();
		int nIndex = m_boxIndex.GetCurSel();
		
#ifdef _UNICODE
		swprintf(szPara, _T("%d,%d,%d,%d,%d,%d"), nIndexOper, nIndex, m_uFrenq, m_uCell1, m_uCell2, m_uCell3);
#else
		sprintf(szPara, _T("%d,%d,%d,%d,%d,%d"), nIndexOper, nIndex, m_uFrenq, m_uCell1, m_uCell2, m_uCell3);
#endif
		
		LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(SPFRQ_SET, 0, (LPVOID)szPara, _tcslen(szPara));
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

			CString strText;
			strText.Format(_T("%d"), m_uFrenq);
			m_lstRead.SetItemText(nIndex, 0, strText);
			strText.Format(_T("%d"), m_uCell1);
			m_lstRead.SetItemText(nIndex, 1, strText);
			strText.Format(_T("%d"), m_uCell2);
			m_lstRead.SetItemText(nIndex, 2, strText);
			strText.Format(_T("%d"), m_uCell3);
			m_lstRead.SetItemText(nIndex, 3, strText);
			
			UpdateData(FALSE);
		}
		
	}
}

void CSpfrqPage::ClearAll()
{

	m_boxOpr.SetCurSel(0);
	m_boxIndex.SetCurSel(0);

	m_uFrenq = 0;
	m_uCell1 = 0;
	m_uCell2 = 0;
	m_uCell3 = 0;
	m_uIndex = 0;
	
	for(int i = 0; i < NUM_SPFRQ; i++)
	{
		m_lstRead.SetItemText(i, 0, _T(""));
		m_lstRead.SetItemText(i, 1, _T(""));
		m_lstRead.SetItemText(i, 2, _T(""));
		m_lstRead.SetItemText(i, 3, _T(""));
	}

	m_lstTest.SetItemText(0,0,_T(""));
	m_lstTest.SetItemText(0,1,_T(""));
	m_lstTest.SetItemText(0,2,_T(""));
	m_lstTest.SetItemText(0,3,_T(""));
	m_lstTest.SetItemText(0,4,_T(""));
	m_lstTest.SetItemText(0,5,_T(""));

	UpdateData(FALSE);

}

void CSpfrqPage::OnBtnReadSpfrq() 
{
	// TODO: Add your control notification handler code here
	SendCheckRequest(FOR_SPFRQ_READ);
	
}

void CSpfrqPage::OnBtnTestSpfrq() 
{
	// TODO: Add your control notification handler code here
	SendCheckRequest(FOR_SPFRQ_TEST);
	
}

void CSpfrqPage::OnBtnSetSpfrq() 
{
	// TODO: Add your control notification handler code here
	SendSetRequest();
	
}
