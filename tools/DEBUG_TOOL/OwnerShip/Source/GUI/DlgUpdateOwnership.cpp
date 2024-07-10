// DlgUpdateOwnership.cpp : implementation file
//

#include "stdafx.h"
#include "Ownership.h"
#include "DlgUpdateOwnership.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgUpdateOwnership dialog


CDlgUpdateOwnership::CDlgUpdateOwnership(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUpdateOwnership::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUpdateOwnership)
	m_nCityID = 0;
	m_strEndNumber = _T("");
	m_strStartNumber = _T("");
	//}}AFX_DATA_INIT
	m_pagCities = NULL;
	m_nMaxEndNum = 0;
}


void CDlgUpdateOwnership::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUpdateOwnership)
	DDX_Control(pDX, IDC_CMB_CITY, m_cmbCities);
	DDX_Text(pDX, IDC_EDT_CITY_ID, m_nCityID);
	DDX_Text(pDX, IDC_EDT_END_NUMBER, m_strEndNumber);
	DDV_MaxChars(pDX, m_strEndNumber, 7);
	DDX_Text(pDX, IDC_EDT_START_NUMBER, m_strStartNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUpdateOwnership, CDialog)
	//{{AFX_MSG_MAP(CDlgUpdateOwnership)
	ON_CBN_SELCHANGE(IDC_CMB_CITY, OnSelchangeCmbCity)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUpdateOwnership message handlers

BOOL CDlgUpdateOwnership::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if(m_pagCities != NULL)
	{
		for(int i=0; i< m_pagCities->GetSize(); i++)
		{
			m_cmbCities.AddString(m_pagCities->GetAt(i));
		}
	}

	m_cmbCities.SetCurSel(m_nCityID-1);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgUpdateOwnership::OnSelchangeCmbCity() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_nCityID = m_cmbCities.GetCurSel()+1;
	UpdateData(FALSE);
}

void CDlgUpdateOwnership::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	if(m_strEndNumber.IsEmpty())
	{
		AfxMessageBox(_T("Please input \"End Number\"."));
		GetDlgItem(IDC_EDT_END_NUMBER)->SetFocus();
		return;
	}

	UINT nEndNum = (UINT)_ttol(m_strEndNumber.operator LPCTSTR());
	UINT nStartNum = (UINT)_ttol(m_strStartNumber.operator LPCTSTR());
	if(nEndNum > m_nMaxEndNum || nEndNum < nStartNum)
	{
		CString strErr;
		strErr.Format(_T("\"End Number\" must be not larger than [%d] or smaller than \"Start Number\" [%s]."),
			            m_nMaxEndNum,m_strStartNumber.operator LPCTSTR());
		AfxMessageBox(strErr);
		GetDlgItem(IDC_EDT_END_NUMBER)->SetFocus();
		return;
	}

	CDialog::OnOK();
}

BOOL CDlgUpdateOwnership::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_CHAR )
	{
		CWnd * pWnd = GetFocus();
		_ASSERTE( pWnd != NULL );
		
		// 同MMI保持一致
		_TCHAR cValue = (_TCHAR)(pMsg->wParam);
		if( pWnd != NULL && pWnd->m_hWnd == GetDlgItem(IDC_EDT_END_NUMBER)->m_hWnd )
		{
			if( cValue < _T('0') || cValue > _T('9') )
			{
				if( cValue != VK_BACK && cValue != 3 && cValue != 22 && cValue != 24) //Ctrl+c Ctrl+v Ctrl+x
				{
					return TRUE;
				}	
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
