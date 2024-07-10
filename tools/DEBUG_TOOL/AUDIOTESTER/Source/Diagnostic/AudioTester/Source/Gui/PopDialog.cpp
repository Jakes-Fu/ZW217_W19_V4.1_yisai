// PopDialog.cpp : implementation file
//

#include "stdafx.h"
#include "AudioTester.h"
#include "PopDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPopDialog dialog


CPopDialog::CPopDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPopDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPopDialog)
	m_nPopSwitch = -1;
	m_nDaLimit = 0;
	m_bWriteToPhone = FALSE;
	//}}AFX_DATA_INIT
    
    m_pArea = NULL;
}


void CPopDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPopDialog)
	DDX_CBIndex(pDX, IDC_COMBO_POP_SWITCH, m_nPopSwitch);
	DDX_Text(pDX, IDC_EDIT_DA_LIMIT, m_nDaLimit);
	DDV_MinMaxInt(pDX, m_nDaLimit, 0, 32767);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPopDialog, CDialog)
	//{{AFX_MSG_MAP(CPopDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopDialog message handlers

BOOL CPopDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_nPopSwitch = m_pArea->m_bPopEnable;
    m_nDaLimit = m_pArea->m_nDaLimit;
    m_bWriteToPhone = m_pArea->m_bImmediateWrite;
    UpdateData( FALSE );
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPopDialog::OnOK() 
{
    if (!UpdateData(TRUE))
    {
        TRACE0("UpdateData failed during dialog termination.\n");
        // the UpdateData routine will set focus to correct item
        return;
    }

    m_pArea->m_bPopEnable = m_nPopSwitch;
    m_pArea->m_nDaLimit = m_nDaLimit;
    m_pArea->m_bImmediateWrite = m_bWriteToPhone;
    EndDialog(IDOK);
}
