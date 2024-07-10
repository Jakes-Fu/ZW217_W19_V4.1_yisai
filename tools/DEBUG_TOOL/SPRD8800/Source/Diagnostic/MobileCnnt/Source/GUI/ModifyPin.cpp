// ModifyPin.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "ModifyPin.h"
#include "DlgSecurity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModifyPin dialog


CModifyPin::CModifyPin(CWnd* pParent /*=NULL*/)
	: CDialog(CModifyPin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModifyPin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bModifyPin1OrPin2 = TRUE;
	m_uRemainNum = 3;
}


void CModifyPin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModifyPin)
	DDX_Control(pDX, IDC_EDT_OLD_PIN, m_edtOldPin);
	DDX_Control(pDX, IDC_EDT_NEW_PIN, m_edtNewPin);
	DDX_Control(pDX, IDC_EDT_NEW_PIN_CONFIM, m_edtNewPinConfirm);
	DDX_Text(pDX, IDC_EDT_OLD_PIN, m_strOldPin);
	DDV_MaxChars(pDX,m_strOldPin, 8);
	DDX_Text(pDX, IDC_EDT_NEW_PIN, m_strNewPin);
	DDV_MaxChars(pDX,m_strNewPin, 8);
	DDX_Text(pDX, IDC_EDT_NEW_PIN_CONFIM, m_strNewPinConfirm);
	DDV_MaxChars(pDX,m_strNewPinConfirm, 8);
	DDX_Text(pDX, IDC_STC_PIN_REMAIN, m_uRemainNum);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNCREATE(CModifyPin, CDialog)
BEGIN_MESSAGE_MAP(CModifyPin, CDialog)
	//{{AFX_MSG_MAP(CModifyPin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModifyPin message handlers

BOOL CModifyPin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SendCheckRequest();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CModifyPin::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	BOOL bPromtPin = FALSE;

	if(!m_strOldPin.IsEmpty() && m_strOldPin.GetLength() < 4)
	{
		bPromtPin = TRUE;
		m_edtOldPin.SetSel(0,-1);
		m_edtOldPin.SetFocus();
	}

	if(!m_strNewPin.IsEmpty() && m_strNewPin.GetLength() < 4)
	{
		bPromtPin= TRUE;
		m_edtNewPin.SetSel(0,-1);
		m_edtNewPin.SetFocus();
	}

	if(!m_strNewPinConfirm.IsEmpty() && m_strNewPinConfirm.GetLength() < 4)
	{
		bPromtPin= TRUE;
		m_edtNewPinConfirm.SetSel(0,-1);
		m_edtNewPinConfirm.SetFocus();
	}
	
	if(bPromtPin)
	{
		AfxMessageBox(IDS_PIN_PROMT);
		return;
	}

	if(m_strNewPinConfirm != m_strNewPin)
	{
		AfxMessageBox(IDS_PIN_ERROR, MB_ICONERROR);
		m_strNewPin.Empty();
		m_strNewPinConfirm.Empty();
		m_edtNewPin.SetFocus();
		UpdateData(FALSE);
		return;
	}

	if(SendSetRequest())
	{
		CDialog::OnOK();
	}
}

BOOL CModifyPin::SendSetRequest()
{
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		char szRet[MAX_PATH] = {0};
		TCHAR szPara[MAX_PATH] = {0};

#ifdef _UNICODE
		swprintf(szPara, _T("%s,%s"), m_strOldPin, m_strNewPin);
#else
		sprintf(szPara, _T("%s,%s"), m_strOldPin, m_strNewPin);
#endif
		LPCSTR lpRet = NULL;
		if(m_bModifyPin1OrPin2)
		{
			lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(PIN_MODIFY, 0, (LPVOID)szPara, _tcslen(szPara));
			if(lpRet != NULL)
			{
				strcpy(szRet, lpRet);
			}
		}
		else
		{
			lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(PIN2_MODIFY, 0, (LPVOID)szPara, _tcslen(szPara));
			if(lpRet != NULL)
			{
				strcpy(szRet, lpRet);
			}
		}
		if(strstr(szRet, "OK") != NULL)
		{
			AfxMessageBox(IDS_SET_SUCCESS);
			m_strOldPin.Empty();
			m_strNewPinConfirm.Empty();
			m_strNewPin.Empty();
			UpdateData(FALSE);
			return TRUE;
		}
		if(strstr(szRet, "ERROR") != NULL)
		{
			AfxMessageBox(IDS_SET_FAILED, MB_ICONERROR);
			SendCheckRequest();
			return FALSE;
		}

	}
	return FALSE;
}

void CModifyPin::SendCheckRequest()
{	
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		if(m_bModifyPin1OrPin2)
		{
			theApp.m_pEtionFactory->SendATCmd(PIN_REMAIN_NUM, 0, NULL, 0);
		}
		else
		{
			theApp.m_pEtionFactory->SendATCmd(PIN2_REMAIN_NUM, 0, NULL, 0);
		}
	}
}

void CModifyPin::ShowRespondContent(LPCSTR szRet)
{
	CString strText = szRet;
	int nIndex = strText.Find(_T("+XX: "));
	if(nIndex != -1)
	{
		strText.Delete(0, nIndex + 5);
#ifdef _UNICODE
		swscanf(strText, _T("%d"), &m_uRemainNum);
#else
		sscanf(strText, _T("%d"), &m_uRemainNum);
#endif
		UpdateData(FALSE);
		
		if(m_uRemainNum < 1)
		{
			if(m_bModifyPin1OrPin2)
			{
				AfxMessageBox(IDS_PUK_NEED);
				((CDlgSecurity *)GetParent())->EnableButton(TRUE);
			}
			else
			{
				AfxMessageBox(IDS_UNLOCK_PIN2);
				((CDlgSecurity *)GetParent())->EnableButton(FALSE);
			}
		}
	}
	
	UpdateData(FALSE);
}