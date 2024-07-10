// DlgHsdpaVersion.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "DlgHsdpaVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHsdpaVersion dialog


CDlgHsdpaVersion::CDlgHsdpaVersion(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHsdpaVersion::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgHsdpaVersion)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgHsdpaVersion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHsdpaVersion)
	DDX_Text(pDX, IDC_STC_HSDPA_VERSION, m_strVersion);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNCREATE(CDlgHsdpaVersion, CDialog)
BEGIN_MESSAGE_MAP(CDlgHsdpaVersion, CDialog)
	//{{AFX_MSG_MAP(CDlgHsdpaVersion)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHsdpaVersion message handlers

BOOL CDlgHsdpaVersion::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SendCheckRequest();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgHsdpaVersion::SendCheckRequest()
{
	if(theApp.m_pEtionFactory != NULL && theApp.m_bConnected)
	{
		char szRet[MAX_PATH] = {0};
		LPCSTR lpRet = (LPCSTR)theApp.m_pEtionFactory->SendATCmd(HSDPA_VERSION, 0, NULL, 0);
		if(lpRet != NULL)
		{
			strcpy(szRet, lpRet);
		}
		
		ShowRespondContent(szRet);

	}
}

void CDlgHsdpaVersion::ShowRespondContent (LPCSTR szRet)
{
	CString strText = szRet;
	int nIndex = strText.Find(_T("SW version: "));
	if(nIndex != -1)
	{
		strText.Delete(0, 12 + nIndex);
	}
	
	nIndex = strText.Find(_T(";"));
	while (nIndex != -1)
	{
		m_strVersion = m_strVersion + strText.Left(nIndex) + "\r\n";
		strText.Delete(0, nIndex + 1);
		nIndex = strText.Find(_T(";"));
	}
	nIndex = strText.Find(_T("OK"));
	if(nIndex != -1)
	{
		m_strVersion += strText.Left(nIndex);
	}
	else
	{
		m_strVersion += strText;
	}
	

	UpdateData(FALSE);
}

void CDlgHsdpaVersion::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
