// DlgRecordDpInputGain.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgRecordDpInputGain.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRecordDpInputGain dialog


CDlgRecordDpInputGain::CDlgRecordDpInputGain(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRecordDpInputGain::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRecordDpInputGain)
		// NOTE: the ClassWizard will add member initialization here
	m_nDPInputGain = 0;
	//}}AFX_DATA_INIT
	m_nDPInputGainBak = 0;
}


void CDlgRecordDpInputGain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRecordDpInputGain)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Text(pDX, IDC_EDIT_DP_INPUT_GAIN, m_nDPInputGain);
	DDV_MinMaxInt(pDX, m_nDPInputGain, -32768, 30);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRecordDpInputGain, CDialog)
	//{{AFX_MSG_MAP(CDlgRecordDpInputGain)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRecordDpInputGain message handlers

BOOL CDlgRecordDpInputGain::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitValue(m_eq_data);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRecordDpInputGain::OnOK() 
{
	// TODO: Add extra validation here
	GetValue(m_eq_data);
	
	CDialog::OnOK();
}

void CDlgRecordDpInputGain::SetValue(EQ_STRUCT_G_T& eq_data)
{
	memcpy(&m_eq_data, &eq_data, sizeof(EQ_STRUCT_G_T));
}

void CDlgRecordDpInputGain::FetchValue(EQ_STRUCT_G_T& eq_data)
{
	memcpy(&eq_data, &m_eq_data, sizeof(EQ_STRUCT_G_T));
}

void CDlgRecordDpInputGain::InitValue(EQ_STRUCT_G_T& eq_data)
{
	int nValue = eq_data.extend[1];
	if(nValue == 0)
	{
		nValue = 1;
	}
	m_nDPInputGain = (double)(20 * (double)log10((double)nValue/(double)1024));
	m_nDPInputGain = (double)((short)(m_nDPInputGain * 10) / 10.0);
	m_nDPInputGain = (m_nDPInputGain > 30) ? 30 : m_nDPInputGain;
	m_nDPInputGain = (m_nDPInputGain < -32768) ? -32768 : m_nDPInputGain;
	
	m_nDPInputGainBak = m_nDPInputGain;
	
	UpdateData(FALSE);
}

void CDlgRecordDpInputGain::GetValue(EQ_STRUCT_G_T& eq_data)
{
	UpdateData();
	
	if(m_nDPInputGain != m_nDPInputGainBak)
	{
		eq_data.extend[1] = ceilf(((double)pow(10, m_nDPInputGain/20.0)) * 1024.0);
	}
	
}
