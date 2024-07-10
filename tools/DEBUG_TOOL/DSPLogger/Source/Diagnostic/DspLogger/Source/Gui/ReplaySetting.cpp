// ReplaySetting.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "ReplaySetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReplaySetting dialog


CReplaySetting::CReplaySetting(CWnd* pParent /*=NULL*/)
	: CDialog(CReplaySetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReplaySetting)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nEdtFrom = 0;
	m_nEdtTo = 0;
	m_nMaxNum = 0;
}


void CReplaySetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReplaySetting)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Text(pDX, IDC_EDT_REPLAY_FROM, m_nEdtFrom);
	DDX_Text(pDX, IDC_EDT_REPLAY_TO, m_nEdtTo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReplaySetting, CDialog)
	//{{AFX_MSG_MAP(CReplaySetting)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReplaySetting message handlers

void CReplaySetting::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	
	if(m_nEdtFrom > m_nMaxNum)
	{
		CString str;
		str.Format("%d", m_nMaxNum);
		str = "Error: replay number can not excess " + str;
		AfxMessageBox(str);
		((CEdit *)GetDlgItem(IDC_EDT_REPLAY_FROM))->SetFocus();
		((CEdit *)GetDlgItem(IDC_EDT_REPLAY_FROM))->SetSel(0,-1);		
		return;
	}

	if(m_nEdtTo > m_nMaxNum)
	{
		CString str;
		str.Format("%d", m_nMaxNum);
		str = "Error: replay number can not excess " + str;
		AfxMessageBox(str);
		((CEdit *)GetDlgItem(IDC_EDT_REPLAY_TO))->SetFocus();
		((CEdit *)GetDlgItem(IDC_EDT_REPLAY_TO))->SetSel(0,-1);		
		return;
	}

	if(m_nEdtTo < m_nEdtFrom)
	{
		AfxMessageBox("Error: replay to number can not less than replay from number");
		((CEdit *)GetDlgItem(IDC_EDT_REPLAY_FROM))->SetFocus();
		((CEdit *)GetDlgItem(IDC_EDT_REPLAY_FROM))->SetSel(0,-1);		
		return;
	}
	
	CDialog::OnOK();
}
