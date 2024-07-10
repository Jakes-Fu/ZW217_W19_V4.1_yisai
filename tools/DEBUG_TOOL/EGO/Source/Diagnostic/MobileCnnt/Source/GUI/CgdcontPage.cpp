// CgdcontPage.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "CgdcontPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCgdcontPage property page

IMPLEMENT_DYNCREATE(CCgdcontPage, CPropertyPage)

CCgdcontPage::CCgdcontPage() : CPropertyPage(CCgdcontPage::IDD)
{
	//{{AFX_DATA_INIT(CCgdcontPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bInit = FALSE;
	m_strEdtCnntIn.Empty();
	m_strEdtType.Empty();

	VERIFY( m_brshDlgBckGrnd.CreateSolidBrush(BK_COLOR) );
}

CCgdcontPage::~CCgdcontPage()
{
}

void CCgdcontPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCgdcontPage)
	DDX_Control(pDX, IDC_LST_CGDCONT_READ, m_lstRead);
	DDX_Control(pDX, IDC_LST_CGDCONT_TEST, m_lstTest);
	DDX_Control(pDX, IDC_COMBO_CGDCONT_ID, m_boxID);
	DDX_Control(pDX, IDC_COMBO_CGDCONT_TYPE, m_boxType);
	DDX_Control(pDX, IDC_COMBO_CGDCONT_CNNT_IN, m_boxCnntIn);
	DDX_Control(pDX, IDC_COMBO_CGDCONT_DATA_COMP, m_boxDComp);
	DDX_Control(pDX, IDC_COMBO_CGDCONT_HEAD_COMP, m_boxHComp);
	DDX_Control(pDX, IDC_EDT_CGDCONT_ADDR, m_edtAddr);
	DDX_Text(pDX, IDC_EDT_CGDCONT_ADDR, m_strAddr);
	DDX_Control(pDX, IDC_BTN_READ_CGDCONT, m_btnRead);
	DDX_Control(pDX, IDC_BTN_SET_CGDCONT, m_btnSet);
	DDX_Control(pDX, IDC_BTN_TEST_CGDCONT, m_btnTest);
	DDX_Control(pDX, IDC_CHECK_CGDCONT_ADDR, m_btnAddr);
	DDX_Text(pDX, IDC_EDT_CGDCONT_TYPE, m_strEdtType);
	DDX_Text(pDX, IDC_EDT_CGDCONT_CNNTIN, m_strEdtCnntIn);
	DDX_Control(pDX, IDC_EDT_CGDCONT_TYPE, m_edtType);
	DDX_Control(pDX, IDC_EDT_CGDCONT_CNNTIN, m_edtCnntIn);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCgdcontPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCgdcontPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_CGDCONT_ADDR, OnCheckCgdcontAddr)
	ON_BN_CLICKED(IDC_BTN_READ_CGDCONT, OnBtnReadCgdcont)
	ON_BN_CLICKED(IDC_BTN_TEST_CGDCONT, OnBtnTestCgdcont)
	ON_BN_CLICKED(IDC_BTN_SET_CGDCONT, OnBtnSetCgdcont)
	ON_CBN_SELCHANGE(IDC_COMBO_CGDCONT_TYPE, OnSelchangeComboCgdcontType)
	ON_CBN_SELCHANGE(IDC_COMBO_CGDCONT_CNNT_IN, OnSelchangeComboCgdcontCnntIn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCgdcontPage message handlers

BOOL CCgdcontPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strText;

	for(int i = 0; i < 16; i++)
	{
		strText.Format(_T("%d"), i);
		m_boxID.AddString(strText);
	}
	m_boxID.SetCurSel(0);
	
	int nCount = sizeof(CGDCONT_TYPE) / sizeof(CGDCONT_TYPE[0]);
	for(i = 0; i < nCount; i++)
	{
		m_boxType.AddString(CGDCONT_TYPE[i]);
	}

	m_boxType.SetCurSel(0);

	nCount = sizeof(CGDCONT_CNNT_IN)/sizeof(CGDCONT_CNNT_IN[0]);
	for(i = 0; i < nCount; i++)
	{
		m_boxCnntIn.AddString(CGDCONT_CNNT_IN[i]);
	}

	m_boxCnntIn.SetCurSel(0);

	nCount = sizeof(CGDCONT_COMP)/sizeof(CGDCONT_COMP[0]);
	for(i = 0; i < nCount; i++)
	{
		m_boxDComp.AddString(CGDCONT_COMP[i]);
		m_boxHComp.AddString(CGDCONT_COMP[i]);
	}

	m_boxDComp.SetCurSel(0);
	m_boxHComp.SetCurSel(0);


	DWORD dwStyle = m_lstTest.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
    m_lstTest.SetExtendedStyle(dwStyle);
	m_lstRead.SetExtendedStyle(dwStyle);

	strText.LoadString(IDS_CGDCONT_ID);
	m_lstTest.InsertColumn(0,strText,LVCFMT_LEFT,60);
	m_lstRead.InsertColumn(0,strText,LVCFMT_LEFT,30);
	strText.LoadString(IDS_CGDCONT_TYPE);
	m_lstTest.InsertColumn(1,strText,LVCFMT_LEFT,60);
	m_lstRead.InsertColumn(1,strText,LVCFMT_LEFT,60);
	strText.LoadString(IDS_CGDCONT_CNNT_IN);
	m_lstTest.InsertColumn(2,strText,LVCFMT_LEFT,60);
	m_lstRead.InsertColumn(2,strText,LVCFMT_LEFT,60);
	strText.LoadString(IDS_CGDCONT_ADDR);
	m_lstTest.InsertColumn(3,strText,LVCFMT_LEFT,60);
	m_lstRead.InsertColumn(3,strText,LVCFMT_LEFT,60);
	strText.LoadString(IDS_CGDCONT_DCOMP);
	m_lstTest.InsertColumn(4,strText,LVCFMT_LEFT,60);
	m_lstRead.InsertColumn(4,strText,LVCFMT_LEFT,60);
	strText.LoadString(IDS_CGDCONT_HCOMP);
	m_lstTest.InsertColumn(5,strText,LVCFMT_LEFT,80);
	m_lstRead.InsertColumn(5,strText,LVCFMT_LEFT,80);
	
	for(i = 0; i < LST_CGDCONT_READ_NUM; i++)
	{
		CString strItem;
		strItem.Format(_T("%d"), i);
		m_lstRead.InsertItem(i, strItem);
	}

	m_lstTest.InsertItem(0, _T(""));
	
	m_btnAddr.SetCheck(0);
	m_edtAddr.EnableWindow(FALSE);
	m_boxDComp.EnableWindow(FALSE);
	m_boxHComp.EnableWindow(FALSE);
	m_edtCnntIn.EnableWindow(FALSE);
	m_edtType.EnableWindow(FALSE);

	m_bInit = TRUE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CCgdcontPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nCtlColor);

	pDC->SetBkColor( BK_COLOR );

	// TODO: Return a different brush if the default is not desired
	return (HBRUSH)(m_brshDlgBckGrnd.m_hObject);
}

void CCgdcontPage::OnCheckCgdcontAddr() 
{
	// TODO: Add your control notification handler code here
	if(m_btnAddr.GetCheck() == 0)
	{
		m_strAddr.Empty();
		m_boxDComp.SetCurSel(0);
		m_boxHComp.SetCurSel(0);
		UpdateData(FALSE);
		m_edtAddr.EnableWindow(FALSE);
		m_boxDComp.EnableWindow(FALSE);
		m_boxHComp.EnableWindow(FALSE);
	}
	else
	{
		m_edtAddr.EnableWindow(TRUE);
		m_boxDComp.EnableWindow(TRUE);
		m_boxHComp.EnableWindow(TRUE);
	}
	
}

void CCgdcontPage::OnBtnReadCgdcont() 
{
	// TODO: Add your control notification handler code here
	SendCheckRequest(FOR_CGDCONT_READ);
	
}

void CCgdcontPage::OnBtnTestCgdcont() 
{
	// TODO: Add your control notification handler code here
	SendCheckRequest(FOR_CGDCONT_TEST);
	
}

void CCgdcontPage::OnBtnSetCgdcont() 
{
	// TODO: Add your control notification handler code here
	SendSetRequest();
	
}

void CCgdcontPage::SendCheckRequest(UINT uFlag)
{	
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		if(uFlag == FOR_CGDCONT_READ)
		{
			theApp.m_pEtionFactory->SendATCmd(CGDCONT_READ, 0, NULL, 0);
		}

		if(uFlag == FOR_CGDCONT_TEST)
		{
			theApp.m_pEtionFactory->SendATCmd(CGDCONT_TEST, 0, NULL, 0);
		}
	}
}

void CCgdcontPage::ShowRespondContent(LPCSTR szRet)
{
	if(!m_bInit)
	{
		return;
	}
	CString strText = szRet;
	int nIndex = -1;
	CString strID, strType, strCnntIn, strAddr, strDComp, strHComp;

	if(strText.Find(_T("+CGDCONT:")) != -1 && strText.Find(_T("+CGDCONT:(")) == -1)
	{
		nIndex = strText.Find(_T("+CGDCONT:"));
		
		strText.Delete(0, nIndex + 9);
		
		nIndex = strText.Find(_T(","));
		if(nIndex != -1)
		{
			strID = strText.Left(nIndex);
			strText.Delete(0, nIndex + 1);
			nIndex = strText.Find(_T(","));
			if(nIndex != -1)
			{
				strType = strText.Left(nIndex);
				strText.Delete(0, nIndex + 1);
				nIndex = strText.Find(_T(","));
				if(nIndex != -1)
				{
					strCnntIn = strText.Left(nIndex);
					strText.Delete(0, nIndex + 1);
					nIndex = strText.Find(_T(","));
					if(nIndex != -1)
					{
						strAddr = strText.Left(nIndex);
						strText.Delete(0, nIndex + 1);
						nIndex = strText.Find(_T(","));
						if(nIndex != -1)
						{
							strDComp = strText.Left(nIndex);
							strText.Delete(0, nIndex + 1);
							strHComp = strText;
						}
					}
				}
			}
		}

		UINT uID = 0;
#ifdef _UNICODE
		swscanf(strID, _T("%d"), &uID);
#else
		sscanf(strID, _T("%d"), &uID);
#endif
		m_lstRead.SetItemText(uID, 1, strType);
		m_lstRead.SetItemText(uID, 2, strCnntIn);
		m_lstRead.SetItemText(uID, 3, strAddr);
		m_lstRead.SetItemText(uID, 4, strDComp);
		m_lstRead.SetItemText(uID, 5, strHComp);
		
	}

	if((nIndex = strText.Find(_T("+CGDCONT:("))) != -1)
	{
		strText.Delete(0, nIndex + 9);
		nIndex = strText.Find(_T(","));
		if(nIndex != -1)
		{
			strID = strText.Left(nIndex);
			strText.Delete(0, nIndex + 1);
			nIndex = strText.Find(_T(","));
			if(nIndex != -1)
			{
				strType = strText.Left(nIndex);
				strText.Delete(0, nIndex + 1);
				nIndex = strText.Find(_T(","));
				if(nIndex != -1)
				{
					strCnntIn = strText.Left(nIndex);
					strText.Delete(0, nIndex + 1);
					nIndex = strText.Find(_T(","));
					if(nIndex != -1)
					{
						strAddr = strText.Left(nIndex);
						strText.Delete(0, nIndex + 1);
						nIndex = strText.Find(_T(")"));
						if(nIndex != -1)
						{
							strDComp = strText.Left(nIndex + 1);
							strText.Delete(0, nIndex + 2);
							strHComp = strText;
						}
					}
				}
			}
		}
		m_lstTest.SetItemText(0, 0, strID);
		m_lstTest.SetItemText(0, 1, strType);
		m_lstTest.SetItemText(0, 2, strCnntIn);
		m_lstTest.SetItemText(0, 3, strAddr);
		m_lstTest.SetItemText(0, 4, strDComp);
		m_lstTest.SetItemText(0, 5, strHComp);

	}
	UpdateData(FALSE);
}

void CCgdcontPage::SendSetRequest()
{
	UpdateData();

	if(theApp.m_pEtionFactory != NULL  && theApp.m_bConnected)
	{
		char szRet[MAX_PATH] = {0};	
		TCHAR szPara[MAX_PATH] = {0};
		
		int nIndexID = m_boxID.GetCurSel();
		int nIndexType = m_boxType.GetCurSel();
		int nIndexConnIn = m_boxCnntIn.GetCurSel();
		CString strType;
		m_boxType.GetLBText(nIndexType, strType);
		CString strCnntIn;
		m_boxCnntIn.GetLBText(nIndexConnIn, strCnntIn);

		if(nIndexType == 1)
		{
			strType = m_strEdtType;
		}

		if(nIndexConnIn == 2)
		{
			strCnntIn = m_strEdtCnntIn;
		}
		
		if(m_btnAddr.GetCheck() == 1)
		{
#ifdef _UNICODE
		swprintf(szPara, _T("%d,\"%s\",\"%s\",\"%s\",%d,%d"), nIndexID, strType, strCnntIn, m_strAddr, m_boxDComp.GetCurSel(), m_boxHComp.GetCurSel());
#else
		sprintf(szPara, _T("%d,\"%s\",\"%s\",\"%s\",%d,%d"), nIndexID, strType, strCnntIn, m_strAddr, m_boxDComp.GetCurSel(), m_boxHComp.GetCurSel());
#endif
		}
		else
		{
#ifdef _UNICODE
		swprintf(szPara, _T("%d,\"%s\",\"%s\",,0,0"), nIndexID, strType, strCnntIn);
#else
		sprintf(szPara, _T("%d,\"%s\",\"%s\",,0,0"), nIndexID, strType, strCnntIn);
#endif
		}
		
		LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(CGDCONT_SET, 0, (LPVOID)szPara, _tcslen(szPara));
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

			CString strId;
			
			m_lstRead.SetItemText(nIndexID, 1, strType);
			m_lstRead.SetItemText(nIndexID, 2, strCnntIn);
			m_lstRead.SetItemText(nIndexID, 3, m_strAddr);
			strId.Format(_T("%d"), m_boxDComp.GetCurSel());
			m_lstRead.SetItemText(nIndexID, 4, strId);
			strId.Format(_T("%d"), m_boxHComp.GetCurSel());
			m_lstRead.SetItemText(nIndexID, 5, strId);
			UpdateData(FALSE);
		}
		
	}
}

void CCgdcontPage::ClearAll()
{

	m_boxID.SetCurSel(0);
	m_boxType.SetCurSel(0);
	m_boxCnntIn.SetCurSel(0);
	m_boxDComp.SetCurSel(0);
	m_boxHComp.SetCurSel(0);
	
	m_btnAddr.SetCheck(0);
	m_edtAddr.EnableWindow(FALSE);
	m_boxDComp.EnableWindow(FALSE);
	m_boxHComp.EnableWindow(FALSE);
	m_strAddr.Empty();

	for(int i = 0; i < LST_CGDCONT_READ_NUM; i++)
	{
		m_lstRead.SetItemText(i, 0, _T(" "));
		m_lstRead.SetItemText(i, 1, _T(" "));
		m_lstRead.SetItemText(i, 2, _T(" "));
		m_lstRead.SetItemText(i, 3, _T(" "));
		m_lstRead.SetItemText(i, 4, _T(" "));
		m_lstRead.SetItemText(i, 5, _T(" "));
	}
	

	UpdateData(FALSE);

}


void CCgdcontPage::OnSelchangeComboCgdcontType() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_boxType.GetCurSel();
	if(nIndex == 1)
	{
		m_edtType.EnableWindow(TRUE);
	}
	else
	{
		m_edtType.EnableWindow(FALSE);
	}
	
}

void CCgdcontPage::OnSelchangeComboCgdcontCnntIn() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_boxCnntIn.GetCurSel();
	if(nIndex == 2)
	{
		m_edtCnntIn.EnableWindow(TRUE);
	}
	else
	{
		m_edtCnntIn.EnableWindow(FALSE);
	}
}
