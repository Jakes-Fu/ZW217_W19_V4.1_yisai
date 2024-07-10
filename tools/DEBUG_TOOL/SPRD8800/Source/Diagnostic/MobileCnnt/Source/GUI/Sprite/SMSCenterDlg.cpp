// SMSCenterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\mobilecnnt.h"
#include "SMSCenterDlg.h"
#include "isms.h"
#include "GlobalDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const int CSMSCenterDlg::CENTERNUM_MAX_LEN = 20;
/////////////////////////////////////////////////////////////////////////////
// CSMSCenterDlg dialog


CSMSCenterDlg::CSMSCenterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSMSCenterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMSCenterDlg)
	m_strCenterNum = _T("");
	m_strCenterNum2 = _T("");
	m_strCenterNum3 = _T("");
	m_strCenterNum4 = _T("");
	//}}AFX_DATA_INIT

	m_pSMSFactory = NULL;
}


void CSMSCenterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMSCenterDlg)
	DDX_Control(pDX, IDC_BTN_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_SET, m_btnSet);
	DDX_Text(pDX, IDC_EDT_SMS_CENTER, m_strCenterNum);
	DDX_Text(pDX, IDC_EDT_SMS_CENTER2, m_strCenterNum2);
	DDX_Text(pDX, IDC_EDT_SMS_CENTER3, m_strCenterNum3);
	DDX_Text(pDX, IDC_EDT_SMS_CENTER4, m_strCenterNum4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMSCenterDlg, CDialog)
	//{{AFX_MSG_MAP(CSMSCenterDlg)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMSCenterDlg message handlers

void CSMSCenterDlg::OnBtnSet()
{
	UpdateData(TRUE);

	if( theApp.IsPluggedSIM(1) && m_strCenterNum.IsEmpty() )
	{
		AfxMessageBox(IDS_ERR_CENTER_NUM_EMPTY);
		return;
	}

	if( theApp.IsPluggedSIM(2) && m_strCenterNum2.IsEmpty() )
	{
		AfxMessageBox(IDS_ERR_CENTER_NUM_EMPTY);
		return;
	}

	if( theApp.IsPluggedSIM(3) && m_strCenterNum3.IsEmpty() )
	{
		AfxMessageBox(IDS_ERR_CENTER_NUM_EMPTY);
		return;
	}

	if( theApp.IsPluggedSIM(4) && m_strCenterNum4.IsEmpty() )
	{
		AfxMessageBox(IDS_ERR_CENTER_NUM_EMPTY);
		return;
	}

	if( SetSMSCenterNum() )
	{
		CDialog::OnOK();
	}
	else
	{
		AfxMessageBox(IDS_ERR_CENTER_NUM);

		theApp.TestIsConnected();

		CDialog::OnCancel();
	}
}

void CSMSCenterDlg::OnBtnCancel() 
{
	// TODO: Add your control notification handler code here
	//
	CDialog::OnCancel();
}

BOOL CSMSCenterDlg::OnInitDialog() 
{
	CWaitCursor waitCursor;

	CDialog::OnInitDialog();

    if( !theApp.TestIsConnected() )
	{
		CDialog::OnCancel();
		return FALSE;
	}

	// TODO: Add extra initialization here

	m_edtCenterNum.SubclassDlgItem(IDC_EDT_SMS_CENTER, this);
	m_edtCenterNum.SetLimitText(CENTERNUM_MAX_LEN); // sms spec define 40
	
	m_edtCenterNum2.SubclassDlgItem(IDC_EDT_SMS_CENTER2, this);
	m_edtCenterNum2.SetLimitText(CENTERNUM_MAX_LEN); // sms spec define 40

	m_edtCenterNum3.SubclassDlgItem(IDC_EDT_SMS_CENTER3, this);
	m_edtCenterNum3.SetLimitText(CENTERNUM_MAX_LEN); // sms spec define 40

	m_edtCenterNum4.SubclassDlgItem(IDC_EDT_SMS_CENTER4, this);
	m_edtCenterNum4.SetLimitText(CENTERNUM_MAX_LEN); // sms spec define 40

	m_pSMSFactory = theApp.m_pSMSFactory;
	_ASSERTE( m_pSMSFactory != NULL );

	if(theApp.IsPluggedSIM(4))
	{
		if(m_pSMSFactory->SetActiveSim(SMS_SP_SIM4))
		{
			m_strCenterNum4 = m_pSMSFactory->QueryServiceCenter();
		}
	}
	else
	{
		GetDlgItem(IDC_EDT_SMS_CENTER4)->EnableWindow(FALSE);
	}

	if(theApp.IsPluggedSIM(3))
	{
		if(m_pSMSFactory->SetActiveSim(SMS_SP_SIM3))
		{
			m_strCenterNum3 = m_pSMSFactory->QueryServiceCenter();
		}
	}
	else
	{
		GetDlgItem(IDC_EDT_SMS_CENTER3)->EnableWindow(FALSE);
	}

	if(theApp.IsPluggedSIM(2))
	{
		if(m_pSMSFactory->SetActiveSim(SMS_SP_SIM2))
		{
			m_strCenterNum2 = m_pSMSFactory->QueryServiceCenter();
		}
	}
	else
	{
		GetDlgItem(IDC_EDT_SMS_CENTER2)->EnableWindow(FALSE);
	}
	
	if(theApp.IsPluggedSIM(1))
	{
		if(theApp.m_nMMIVer == MMI_VERSION_2 && theApp.m_nSIMCount > 1)			
		{
			if(m_pSMSFactory->SetActiveSim(SMS_SP_SIM))
				m_strCenterNum = m_pSMSFactory->QueryServiceCenter();
		}
		else
		{
			m_strCenterNum = m_pSMSFactory->QueryServiceCenter();
		}
	}	
	else
	{
		GetDlgItem(IDC_EDT_SMS_CENTER)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSMSCenterDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_CHAR && 
		( m_edtCenterNum.m_bFocus || m_edtCenterNum2.m_bFocus ||
		  m_edtCenterNum3.m_bFocus || m_edtCenterNum4.m_bFocus) )
	{
		// notice: 0-9,+,#,*,a,b,c,A,B,C there are sms spec define
		// please reference "AT command user guid.doc"
		_TCHAR cValue = (_TCHAR)(pMsg->wParam);
		if( cValue < _T('0') || cValue > _T('9') )
		{
			if( cValue != _T('+') && cValue != _T('#') && cValue != _T('*') )
			{
				if( cValue >= _T('a') && cValue <= _T('c') || 
					cValue >= _T('A') && cValue <= _T('C') )
				{
					return CDialog::PreTranslateMessage(pMsg);
				}
				else if( cValue == VK_BACK )
				{
					return CDialog::PreTranslateMessage(pMsg);
				}
				return TRUE;
			}	
		}
		return CDialog::PreTranslateMessage(pMsg);
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CSMSCenterDlg::SetSMSCenterNum()
{
	_ASSERTE( m_pSMSFactory != NULL );

	BOOL bSim1OK = TRUE;
	BOOL bSim2OK = TRUE;
	BOOL bSim3OK = TRUE;
	BOOL bSim4OK = TRUE;

	if( theApp.m_nMMIVer == MMI_VERSION_2 && theApp.IsPluggedSIM(4))		
	{
		if( m_pSMSFactory->SetActiveSim(SMS_SP_SIM4) )
		{
			bSim4OK = m_pSMSFactory->SetServiceCenter(m_strCenterNum4);
		}
		else
		{
			AfxMessageBox(IDS_SMS_E_SET_ACTIVE_SIM);
		}
	}

	if( theApp.m_nMMIVer == MMI_VERSION_2 && theApp.IsPluggedSIM(3))		
	{
		if( m_pSMSFactory->SetActiveSim(SMS_SP_SIM3) )
		{
			bSim3OK = m_pSMSFactory->SetServiceCenter(m_strCenterNum3);
		}
		else
		{
			AfxMessageBox(IDS_SMS_E_SET_ACTIVE_SIM);
		}
	}

	if( theApp.m_nMMIVer == MMI_VERSION_2 && theApp.IsPluggedSIM(2))		
	{
		if( m_pSMSFactory->SetActiveSim(SMS_SP_SIM2) )
		{
			bSim2OK = m_pSMSFactory->SetServiceCenter(m_strCenterNum2);
		}
		else
		{
			AfxMessageBox(IDS_SMS_E_SET_ACTIVE_SIM);
		}
	}

	if(theApp.IsPluggedSIM(1))
	{
		if( theApp.m_nMMIVer == MMI_VERSION_2 )
		{
			if( m_pSMSFactory->SetActiveSim(SMS_SP_SIM) )
			{
				bSim1OK = m_pSMSFactory->SetServiceCenter(m_strCenterNum);
			}
			else
			{
				AfxMessageBox(IDS_SMS_E_SET_ACTIVE_SIM);
			}
		}
		else
		{
			bSim1OK = m_pSMSFactory->SetServiceCenter(m_strCenterNum);
		}
	}

	return (bSim4OK && bSim3OK && bSim2OK && bSim1OK);
}
