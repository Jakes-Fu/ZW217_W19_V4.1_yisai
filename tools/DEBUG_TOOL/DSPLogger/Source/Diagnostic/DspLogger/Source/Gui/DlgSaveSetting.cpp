// DlgSaveSetting.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "DlgSaveSetting.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSaveSetting dialog


CDlgSaveSetting::CDlgSaveSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSaveSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSaveSetting)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_dwSaveMode = MODE_ALL;
}


void CDlgSaveSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSaveSetting)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSaveSetting, CDialog)
	//{{AFX_MSG_MAP(CDlgSaveSetting)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSaveSetting message handlers

void CDlgSaveSetting::OnOK() 
{
	// TODO: Add extra validation here
	int nID = GetCheckedRadioButton(IDC_RADIO_ALL, IDC_RADIO_CURRENT);
	if(nID == IDC_RADIO_ALL)
	{
		m_dwSaveMode = MODE_ALL;
	}
	else if(nID == IDC_RADIO_RECENT)
	{
		m_dwSaveMode = MODE_RECENTLY;
	}
	else
	{
		m_dwSaveMode = MODE_CURRENT;
	}
	
	CDialog::OnOK();
}

BOOL CDlgSaveSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	((CButton *)GetDlgItem(IDC_RADIO_ALL))->SetCheck(1);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
