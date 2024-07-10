// DlgSecurity.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "DlgSecurity.h"
#include "ModifyPin.h"
#include "DlgUnlockPin2.h"
#include "DlgInputPuk1.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSecurity dialog


CDlgSecurity::CDlgSecurity(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSecurity::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSecurity)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strPin1.Empty();
	m_strPin2.Empty();
	m_strPin2RemainNum.Empty();
	m_strPin1RemainNum.Empty();
	m_uPin2RemainNum = 0;
	m_uPin1RemainNum = 0;
	m_bFirstCheck = FALSE;
	m_bPin1OrPin2 = TRUE;
	m_uCount = 0;

	VERIFY( m_brshDlgBckGrnd.CreateSolidBrush(BK_COLOR) );
}


void CDlgSecurity::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSecurity)
	DDX_Control(pDX, IDC_RADIO_PIN_ON, m_btnPinOn);
	DDX_Control(pDX, IDC_RADIO_PIN_OFF, m_btnPinOff);
	DDX_Control(pDX, IDC_RADIO_OPEN_FDN, m_btnFdnOn);
	DDX_Control(pDX, IDC_RADIO_CLOSE_FDN, m_btnFdnOff);
	DDX_Text(pDX, IDC_EDT_PIN, m_strPin1);
	DDV_MaxChars(pDX,m_strPin1, 8);
	DDX_Text(pDX, IDC_EDT_PIN2, m_strPin2);
	DDV_MaxChars(pDX,m_strPin2, 8);
	DDX_Text(pDX, IDC_STC_PIN2_REMAIN, m_strPin2RemainNum);
	DDX_Text(pDX, IDC_STC_PIN1_REMAIN, m_strPin1RemainNum);
	DDX_Control(pDX, IDC_BTN_MODIFY_PIN1, m_btnNodifyPin1);
	DDX_Control(pDX, IDC_BTN_UNLOCK_PIN2, m_btnUnlockPin2);
	DDX_Control(pDX, IDC_EDT_PIN, m_edtPin1);
	DDX_Control(pDX, IDC_EDT_PIN2, m_edtPin2);
	DDX_Control(pDX, IDC_BTN_UNLOCK_PIN1, m_btnUnlockPin1);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSecurity, CDialog)
	//{{AFX_MSG_MAP(CDlgSecurity)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_MODIFY_PIN1, OnBtnModifyPin1)
	ON_BN_CLICKED(IDC_BTN_PIN2_MODIFY, OnBtnPin2Modify)
	ON_BN_CLICKED(IDC_BTN_UNLOCK_PIN2, OnBtnUnlockPin2)
	ON_BN_CLICKED(IDC_BTN_PIN1_APPLY, OnBtnPin1Apply)
	ON_BN_CLICKED(IDC_BTN_PIN2_APPLY, OnBtnPin2Apply)
	ON_BN_CLICKED(IDC_BTN_UNLOCK_PIN1, OnBtnUnlockPin1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSecurity message handlers

HBRUSH CDlgSecurity::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nCtlColor);

	pDC->SetBkColor( BK_COLOR );

	// TODO: Return a different brush if the default is not desired
	return (HBRUSH)(m_brshDlgBckGrnd.m_hObject);
}

void CDlgSecurity::OnBtnModifyPin1() 
{
	// TODO: Add your control notification handler code here
	CModifyPin dlg;
	dlg.m_bModifyPin1OrPin2 = TRUE;
	dlg.DoModal();
	
}

void CDlgSecurity::OnBtnPin2Modify() 
{
	// TODO: Add your control notification handler code here
	CModifyPin dlg;
	dlg.m_bModifyPin1OrPin2 = FALSE;
	dlg.DoModal();
	
}

void CDlgSecurity::OnBtnUnlockPin2() 
{
	// TODO: Add your control notification handler code here
	CDlgUnlockPin2 dlg;
	if(dlg.DoModal() == IDOK)
	{
		m_btnUnlockPin2.EnableWindow(FALSE);
		CheckPinRemainNum(FALSE);
		m_strPin2.Empty();
		UpdateData(FALSE);
	}
	
}

void CDlgSecurity::SendCheckRequest()
{
	UpdateData();
	if(!m_strPin1RemainNum.IsEmpty() && !m_strPin2RemainNum.IsEmpty())
	{
		return;
	}
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		theApp.m_pEtionFactory->SendATCmd(PIN_CHECK, 0, NULL, 0);
		
		theApp.m_pEtionFactory->SendATCmd(FDN_CHECK, 0, NULL, 0);
	}

	CheckPinRemainNum(TRUE);
	
	CheckPinRemainNum(FALSE);
}

void CDlgSecurity::SendSetRequest(UINT uFlag)
{
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		char szRet[MAX_PATH] = {0};
		TCHAR szPara[MAX_PATH] = {0};

		if(uFlag == FOR_PIN1)
		{
			BOOL bOn = FALSE;
			if(m_strPin1.IsEmpty())
			{
				AfxMessageBox(IDS_PIN_EMPTY);
				m_edtPin1.SetFocus();
				return;
			}
			
			if(m_btnPinOn.GetCheck() == 1)
			{
#ifdef _UNICODE
				swprintf(szPara, _T("1,%s"), m_strPin1);
#else
				sprintf(szPara, _T("1,%s"), m_strPin1);
#endif
				bOn = TRUE;
			}
			else
			{
#ifdef _UNICODE
				swprintf(szPara, _T("0,%s"), m_strPin1);
#else
				sprintf(szPara, _T("0,%s"), m_strPin1);
#endif
				bOn = FALSE;
			}
			LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(PIN_SET, 0, (LPVOID)szPara, _tcslen(szPara));
			if(lpRet != NULL)
			{
				strcpy(szRet, lpRet);
			}
			
			if(strstr(szRet, "OK") != NULL)
			{
				AfxMessageBox(IDS_SET_SUCCESS);
				m_strPin1.Empty();
				if(bOn)
				{
					m_btnNodifyPin1.EnableWindow(TRUE);
				}
				else
				{
					m_btnNodifyPin1.EnableWindow(FALSE);
				}
				CheckPinRemainNum(TRUE);
			}
			if(strstr(szRet, "ERROR") != NULL)
			{
				if(strstr(szRet, "SIM PIN required") != NULL || strstr(szRet, "16") != NULL )
				{
					AfxMessageBox(IDS_PWD_ERROR, MB_ICONERROR);
					m_strPin1.Empty();
					m_edtPin1.SetFocus();
					UpdateData(FALSE);
				}
				else
				{
					AfxMessageBox(IDS_SET_FAILED, MB_ICONERROR);
				}
				CheckPinRemainNum(TRUE);
			}
			UpdateData(FALSE);
				
		}

		if(uFlag == FOR_FDN)
		{
			if(m_strPin2.IsEmpty())
			{
				AfxMessageBox(IDS_PIN_EMPTY);
				m_edtPin2.SetFocus();
				return;
			}

			
			if(m_btnFdnOn.GetCheck() == 1)
			{
#ifdef _UNICODE
				swprintf(szPara, _T("1,%s"), m_strPin2);
#else
				sprintf(szPara, _T("1,%s"), m_strPin2);
#endif
			}
			else
			{
#ifdef _UNICODE
				swprintf(szPara, _T("0,%s"), m_strPin2);
#else
				sprintf(szPara, _T("0,%s"), m_strPin2);
#endif
				
			}
			LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(FDN_SET, 0, (LPVOID)szPara, _tcslen(szPara));
			if(lpRet == NULL)
			{
				ZeroMemory(szRet, MAX_PATH);
			}
			else
			{
				strcpy(szRet, lpRet);
			}
			
			if(strstr(szRet, "OK") != NULL)
			{
				AfxMessageBox(IDS_SET_SUCCESS);
				m_strPin2.Empty();
				CheckPinRemainNum(FALSE);
			}
			if(strstr(szRet, "ERROR") != NULL)
			{
				if(strstr(szRet, "SIM PIN2 required") != NULL || strstr(szRet, "16") != NULL)
				{
					AfxMessageBox(IDS_PWD_ERROR, MB_ICONERROR);
					m_strPin2.Empty();
					m_edtPin2.SetFocus();
					UpdateData(FALSE);
				}
				else
				{
					AfxMessageBox(IDS_SET_FAILED, MB_ICONERROR);
				}
				CheckPinRemainNum(FALSE);
			}
			UpdateData(FALSE);
		}		
	}
}


void CDlgSecurity::CheckPinRemainNum(BOOL bPin1OrPin2)
{
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		if(bPin1OrPin2)
		{
			theApp.m_pEtionFactory->SendATCmd(PIN_REMAIN_NUM, 0, NULL, 0);
			m_bPin1OrPin2 = TRUE;
		}
		else
		{
			theApp.m_pEtionFactory->SendATCmd(PIN2_REMAIN_NUM, 0, NULL, 0);
			m_bPin1OrPin2 = FALSE;
		}
	}
}

void CDlgSecurity::ShowRespondContent(LPCSTR szRet)
{
	CString strText = szRet;
	int nIndex = strText.Find(_T("+XX: "));
	if(nIndex != -1)
	{
		strText.Delete(0, nIndex + 5);
		UINT uNum = 0;
#ifdef _UNICODE
		swscanf(strText, _T("%d"), &uNum);
#else
		sscanf(strText, _T("%d"), &uNum);
#endif
		if(m_uCount == 0)
		{
			m_uPin1RemainNum = uNum;
			m_strPin1RemainNum = strText;
			UpdateData(FALSE);
			
			if(m_uPin1RemainNum < 1)
			{
				AfxMessageBox(IDS_PUK_NEED);
				m_btnUnlockPin1.EnableWindow(TRUE);
			}
			else
			{
				m_btnUnlockPin1.EnableWindow(FALSE);
			}
		}
		else if(m_uCount == 1)
		{
			m_uPin2RemainNum = uNum;
			m_strPin2RemainNum = strText;
			UpdateData(FALSE);
			
			if(m_uPin2RemainNum < 1)
			{
				AfxMessageBox(IDS_UNLOCK_PIN2);
				m_btnUnlockPin2.EnableWindow(TRUE);
			}
			else
			{
				m_btnUnlockPin2.EnableWindow(FALSE);
			}
		}
		else
		{
			if(m_bPin1OrPin2)
			{
				m_uPin1RemainNum = uNum;
				m_strPin1RemainNum = strText;
				UpdateData(FALSE);
				
				if(m_uPin1RemainNum < 1)
				{
					AfxMessageBox(IDS_PUK_NEED);
					m_btnUnlockPin1.EnableWindow(TRUE);
				}
				else
				{
					m_btnUnlockPin1.EnableWindow(FALSE);
				}
			}
			else
			{
				m_uPin2RemainNum = uNum;
				m_strPin2RemainNum = strText;
				UpdateData(FALSE);
				
				if(m_uPin2RemainNum < 1)
				{
					AfxMessageBox(IDS_UNLOCK_PIN2);
					m_btnUnlockPin2.EnableWindow(TRUE);
				}
				else
				{
					m_btnUnlockPin2.EnableWindow(FALSE);
				}
			}
		}
		m_uCount ++;
	}

	nIndex = strText.Find(_T("+CLCK: "));
	if(nIndex != -1)
	{
		CString strTmp;
		strTmp = strText.Right(1);
		if(strTmp.Find(_T("0")) != -1)
		{
			if(!m_bFirstCheck)
			{
				m_btnPinOff.SetCheck(1);
				m_btnPinOn.SetCheck(0);
				m_btnNodifyPin1.EnableWindow(FALSE);
			}
			else
			{
				m_btnFdnOff.SetCheck(1);
				m_btnFdnOn.SetCheck(0);
			}
		}
		if(strTmp.Find(_T("1")) != -1)
		{
			if(!m_bFirstCheck)
			{
				m_btnPinOff.SetCheck(0);
				m_btnPinOn.SetCheck(1);
				m_btnNodifyPin1.EnableWindow(TRUE);
			}
			else
			{
				m_btnFdnOff.SetCheck(0);
				m_btnFdnOn.SetCheck(1);
			}
		}
		m_bFirstCheck = TRUE;
	}
	
	UpdateData(FALSE);
}

BOOL CDlgSecurity::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_btnUnlockPin2.EnableWindow(FALSE);
	m_btnNodifyPin1.EnableWindow(FALSE);
	m_btnUnlockPin1.EnableWindow(FALSE);

	SendCheckRequest();

	UpdateData(FALSE);
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	if(pFrm != NULL && pFrm->m_bSimInvalid)
	{
		EnableWindowAll(FALSE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSecurity::OnBtnPin1Apply() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if(!m_strPin1.IsEmpty() && m_strPin1.GetLength() < 4)
	{
		AfxMessageBox(IDS_PIN_PROMT);
		m_edtPin1.SetSel(0,-1);
		m_edtPin1.SetFocus();
		return;
	}

	SendSetRequest(FOR_PIN1);
	
}

void CDlgSecurity::OnBtnPin2Apply() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if(!m_strPin2.IsEmpty() && m_strPin2.GetLength() < 4)
	{
		AfxMessageBox(IDS_PIN_PROMT);
		m_edtPin2.SetSel(0,-1);
		m_edtPin2.SetFocus();
		return;
	}

	SendSetRequest(FOR_FDN);
	
}

void CDlgSecurity::ClearAll()
{
	m_strPin1.Empty();
	m_strPin2.Empty();
	m_strPin2RemainNum.Empty();
	m_uPin2RemainNum = 0;
	m_bFirstCheck = FALSE;
	UpdateData(FALSE);
}

void CDlgSecurity::OnBtnUnlockPin1() 
{
	// TODO: Add your control notification handler code here
	CDlgInputPuk1 dlg;
	if(dlg.DoModal() == IDOK)
	{
		m_btnUnlockPin1.EnableWindow(FALSE);
		CheckPinRemainNum(TRUE);
		m_strPin1.Empty();
		UpdateData(FALSE);
	}
	
}

void CDlgSecurity::EnableButton(BOOL bPin1OrPin2)
{
	if(bPin1OrPin2)
	{
		m_btnUnlockPin1.EnableWindow(TRUE);
	}
	else
	{
		m_btnUnlockPin2.EnableWindow(TRUE);
	}
}
void CDlgSecurity::EnableWindowAll(BOOL bEnable)
{	
	m_btnPinOff.EnableWindow(bEnable);
	m_btnFdnOn.EnableWindow(bEnable);
	m_btnFdnOff.EnableWindow(bEnable);
	m_edtPin1.EnableWindow(bEnable);
	m_edtPin2.EnableWindow(bEnable);
	m_btnPinOn.EnableWindow(bEnable);
	m_btnPinOn.EnableWindow(bEnable);
	m_btnPinOn.EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_PIN1_APPLY)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_PIN2_APPLY)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_PIN2_MODIFY)->EnableWindow(bEnable);
}
