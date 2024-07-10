// DlgPBType.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "DlgPBType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPBType dialog


CDlgPBType::CDlgPBType(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPBType::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPBType)
		// NOTE: the ClassWizard will add member initialization here
	m_uPBUsed = 0;
	m_uPBMax = 0;
	//}}AFX_DATA_INIT
}


void CDlgPBType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPBType)
	DDX_Control(pDX, IDC_COMBO_PB_TYPE, m_boxType);
	DDX_Text(pDX, IDC_STC_PB_USED, m_uPBUsed);
	DDX_Text(pDX, IDC_STC_PB_MAX, m_uPBMax);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPBType, CDialog)
	//{{AFX_MSG_MAP(CDlgPBType)
	ON_CBN_SELCHANGE(IDC_COMBO_PB_TYPE, OnSelchangeComboPbType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPBType message handlers

void CDlgPBType::OnOK() 
{
	// TODO: Add extra validation here
	int nIndex = m_boxType.GetCurSel();

	if(SendSetRequest(nIndex))
	{
		CDialog::OnOK();
	}
	else
	{
		AfxMessageBox(IDS_SET_FAILED);
	}
	
	//CDialog::OnOK();
}

BOOL CDlgPBType::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_boxType.AddString(_T("SM"));
	m_boxType.AddString(_T("FD"));
	m_boxType.AddString(_T("LD"));
	m_boxType.AddString(_T("MC"));
	m_boxType.AddString(_T("RC"));
	m_boxType.AddString(_T("ME"));
	//m_boxType.SetCurSel(0);

	SendCheckRequest();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPBType::SendCheckRequest()
{
	CString strRet;
	if(theApp.m_pPBFactory != NULL && theApp.m_bConnected)
	{
		strRet = theApp.m_pPBFactory->GetPBType();
		if(!strRet.IsEmpty())
		{
			ParseCPBS(strRet);
			UpdateData(FALSE);
		}
	}
}

BOOL CDlgPBType::SendSetRequest(DWORD dwType)
{
	if(theApp.m_pPBFactory != NULL && theApp.m_bConnected)
	{
		return theApp.m_pPBFactory->SetPBType(dwType);
	}
	return FALSE;
}

void CDlgPBType::ParseCPBS(CString strCPBS)
{
	CString strTmp;
	int nIndex = strCPBS.Find(_T(","));
	if(nIndex != -1)
	{
		strTmp = strCPBS.Left(nIndex);
		if(strTmp.Find(_T("SM")) != -1)
		{
			m_boxType.SetCurSel(0);
		}
		if(strTmp.Find(_T("FD")) != -1)
		{
			m_boxType.SetCurSel(1);
		}
		if(strTmp.Find(_T("LD")) != -1)
		{
			m_boxType.SetCurSel(2);
		}
		if(strTmp.Find(_T("MC")) != -1)
		{
			m_boxType.SetCurSel(3);
		}
		if(strTmp.Find(_T("RC")) != -1)
		{
			m_boxType.SetCurSel(4);
		}
		if(strTmp.Find(_T("ME")) != -1)
		{
			m_boxType.SetCurSel(5);
		}
		strCPBS.Delete(0, nIndex + 1);
		nIndex = strCPBS.Find(_T(","));
		if(nIndex != -1 )
		{
#ifdef _UNICODE
			wscanf(strCPBS.Left(nIndex), _T("%d"), &m_uPBUsed);
#else
			sscanf(strCPBS.Left(nIndex), _T("%d"), &m_uPBUsed);
#endif
			strCPBS.Delete(0, nIndex + 1);
#ifdef _UNICODE
			wscanf(strCPBS, _T("%d"), &m_uPBMax);
#else
			sscanf(strCPBS, _T("%d"), &m_uPBMax);
#endif
		}

	}
}

void CDlgPBType::OnSelchangeComboPbType() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_boxType.GetCurSel();

	SendSetRequest(nIndex);

	SendCheckRequest();
	
}
