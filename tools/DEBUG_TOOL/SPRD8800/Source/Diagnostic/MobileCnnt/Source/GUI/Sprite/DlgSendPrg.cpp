// DlgSendPrg.cpp : implementation file
//

#include "stdafx.h"
#include "..\mobilecnnt.h"
#include "DlgSendPrg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSendPrg dialog


CDlgSendPrg::CDlgSendPrg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSendPrg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSendPrg)
	m_strSendItem = _T("");
	m_strTo = _T("");
	//}}AFX_DATA_INIT
}


void CDlgSendPrg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSendPrg)
	DDX_Control(pDX, IDC_PRG_SENDING, m_prgSend);
	DDX_Text(pDX, IDC_STC_SEND_ITEM, m_strSendItem);
	DDX_Text(pDX, IDC_STC_TO, m_strTo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSendPrg, CDialog)
	//{{AFX_MSG_MAP(CDlgSendPrg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSendPrg message handlers
