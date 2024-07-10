// DlgSelSim.cpp : implementation file
//

#include "stdafx.h"
#include "..\mobilecnnt.h"
#include "DlgSelSim.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelSim dialog


CDlgSelSim::CDlgSelSim(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelSim::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelSim)
	m_nSim = 0;
	//}}AFX_DATA_INIT
}


void CDlgSelSim::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelSim)
	DDX_Control(pDX, ID_SELSIM_BTN_OK, m_btnOK);
	DDX_Control(pDX, ID_SELSIM_BTN_CANCEL, m_btnCancel);
	DDX_Radio(pDX, IDC_SELSIM_RDO_SIM1, m_nSim);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelSim, CDialog)
	//{{AFX_MSG_MAP(CDlgSelSim)
	ON_BN_CLICKED(ID_SELSIM_BTN_OK, OnSelsimBtnOk)
	ON_BN_CLICKED(ID_SELSIM_BTN_CANCEL, OnSelsimBtnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelSim message handlers

void CDlgSelSim::OnSelsimBtnOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CDialog::OnOK();
}

void CDlgSelSim::OnSelsimBtnCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

BOOL CDlgSelSim::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(!theApp.IsPluggedSIM(1))
	{
		GetDlgItem(IDC_SELSIM_RDO_SIM1)->EnableWindow(FALSE);
	}
	else
	{
		m_nSim = 0;
	}

	if(!theApp.IsPluggedSIM(2))
	{
		GetDlgItem(IDC_SELSIM_RDO_SIM2)->EnableWindow(FALSE);
	}
	else
	{
		m_nSim = 1;
	}

	if(!theApp.IsPluggedSIM(3))
	{
		GetDlgItem(IDC_SELSIM_RDO_SIM3)->EnableWindow(FALSE);
	}
	else
	{
		m_nSim = 2;
	}
	
	if(!theApp.IsPluggedSIM(4))
	{
		GetDlgItem(IDC_SELSIM_RDO_SIM4)->EnableWindow(FALSE);
	}
	else
	{
		m_nSim = 3;
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
