// DlgAlcSet.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgAlcSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAlcSet dialog


CDlgAlcSet::CDlgAlcSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAlcSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAlcSet)
		// NOTE: the ClassWizard will add member initialization here
	m_nCBAlcSw = 0;
	m_nHold = 0;
	m_nRise = 0;
	m_nFall = 0;
	m_nLimit = 0;
	m_nThreshold = 0;
	m_nRatio = 0;
	m_nCgVar = 0;
	m_nReleaseRate = 0;
	m_nAttackRate = 0;
	m_nReleaseRateEx = 0;
	m_nAttackRateEx = 0;
	//}}AFX_DATA_INIT
	memset(&m_VolumeData, 0, sizeof(VOLUME_STRUCT));
}


void CDlgAlcSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAlcSet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_CBIndex(pDX, IDC_COMBO_ALC_SW, m_nCBAlcSw);
	DDX_Text(pDX, IDC_EDIT_HOLD, m_nHold);
	DDX_Text(pDX, IDC_EDIT_RISE, m_nRise);
	DDX_Text(pDX, IDC_EDIT_FALL, m_nFall);
	DDX_Text(pDX, IDC_EDIT_LIMIT, m_nLimit);
	DDX_Text(pDX, IDC_EDIT_THRESHOLD, m_nThreshold);
	DDX_Text(pDX, IDC_EDIT_RATIO, m_nRatio);
	DDX_Text(pDX, IDC_EDIT_CG_VAR, m_nCgVar);
	DDX_Text(pDX, IDC_EDIT_RELEASE_RATE, m_nReleaseRate);
	DDX_Text(pDX, IDC_EDIT_ATTACK_RATE, m_nAttackRate);
	DDX_Text(pDX, IDC_EDIT_RELEASE_RATE_EX, m_nReleaseRateEx);
	DDX_Text(pDX, IDC_EDIT_ATTACK_RATE_EX, m_nAttackRateEx);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAlcSet, CDialog)
	//{{AFX_MSG_MAP(CDlgAlcSet)
	ON_CBN_SELCHANGE(IDC_COMBO_ALC_SW, OnSelchangeComboAlcSw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAlcSet message handlers

BOOL CDlgAlcSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_bSerialPara)
	{
		InitEQValue(m_eq_data);
	}
	else
	{
		InitValue(m_VolumeData);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAlcSet::SetValue(VOLUME_STRUCT& volume_data)
{
	memcpy(&m_VolumeData, &volume_data, sizeof(VOLUME_STRUCT));
}

void CDlgAlcSet::InitValue(VOLUME_STRUCT& volume_data)
{
	m_nCBAlcSw = (volume_data.app_config_info_set.aud_proc_exp_control[1] & 0x100) >> 8;
	if(m_nCBAlcSw == 0)
	{
		GetDlgItem(IDC_EDIT_HOLD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RISE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FALL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LIMIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_THRESHOLD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RATIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CG_VAR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RELEASE_RATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ATTACK_RATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RELEASE_RATE_EX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ATTACK_RATE_EX)->EnableWindow(FALSE);
	}
	
	m_nHold = volume_data.reserve[7];
	m_nRise = volume_data.reserve[8];
	m_nFall = volume_data.reserve[9];
	m_nLimit = volume_data.reserve[10];
	m_nThreshold = volume_data.reserve[11];
	m_nRatio = volume_data.reserve[12];
	m_nCgVar = volume_data.reserve[13];
	m_nReleaseRate = volume_data.reserve[14];
	m_nAttackRate = volume_data.reserve[15];
	m_nReleaseRateEx = volume_data.reserve[16];
	m_nAttackRateEx = volume_data.reserve[17];
	
	UpdateData(FALSE);
}

void CDlgAlcSet::GetValue(VOLUME_STRUCT& volume_data)
{
	UpdateData();
	volume_data.reserve[7] = m_nHold;
	volume_data.reserve[8] = m_nRise;
	volume_data.reserve[9] = m_nFall;
	volume_data.reserve[10] = m_nLimit;
	volume_data.reserve[11] = m_nThreshold;
	volume_data.reserve[12] = m_nRatio;
	volume_data.reserve[13] = m_nCgVar;
	volume_data.reserve[14] = m_nReleaseRate;
	volume_data.reserve[15] = m_nAttackRate;
	volume_data.reserve[16] = m_nReleaseRateEx;
	volume_data.reserve[17] = m_nAttackRateEx;

	int n = volume_data.app_config_info_set.aud_proc_exp_control[1];
	n = (n & 0xfeff) | (m_nCBAlcSw << 8);
	volume_data.app_config_info_set.aud_proc_exp_control[1] = n;

}

void CDlgAlcSet::FetchValue(VOLUME_STRUCT& volume_data)
{
	memcpy(&volume_data, &m_VolumeData, sizeof(VOLUME_STRUCT));
}

void CDlgAlcSet::OnOK() 
{
	// TODO: Add extra validation here
	if(m_bSerialPara)
	{
		GetEQValue(m_eq_data);
	}
	else
	{
		GetValue(m_VolumeData);
	}
	
	
	CDialog::OnOK();
}

void CDlgAlcSet::OnSelchangeComboAlcSw() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if(m_nCBAlcSw == 0)
	{
		GetDlgItem(IDC_EDIT_HOLD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RISE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FALL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LIMIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_THRESHOLD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RATIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CG_VAR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RELEASE_RATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ATTACK_RATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RELEASE_RATE_EX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ATTACK_RATE_EX)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_HOLD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RISE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FALL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LIMIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_THRESHOLD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RATIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CG_VAR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RELEASE_RATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ATTACK_RATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RELEASE_RATE_EX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ATTACK_RATE_EX)->EnableWindow(TRUE);
	}
	
}

void CDlgAlcSet::SetEQValue(EQ_STRUCT_G_T& eq_data)
{
	memcpy(&m_eq_data, &eq_data, sizeof(EQ_STRUCT_G_T));
}

void CDlgAlcSet::FetchEQValue(EQ_STRUCT_G_T& eq_data)
{
	memcpy(&eq_data, &m_eq_data, sizeof(EQ_STRUCT_G_T));
}

void CDlgAlcSet::GetEQValue(EQ_STRUCT_G_T& eq_data)
{
	UpdateData();

	m_eq_data.extend[48] = m_nHold;
	m_eq_data.extend[49] = m_nRise;
	m_eq_data.extend[50] = m_nFall;
	m_eq_data.extend[51] = m_nLimit;
	m_eq_data.extend[52] = m_nThreshold;
	m_eq_data.extend[53] = m_nRatio;
	m_eq_data.extend[54] = m_nCgVar;
	m_eq_data.extend[55] = m_nReleaseRate;
	m_eq_data.extend[56] = m_nAttackRate;
	m_eq_data.extend[57] = m_nReleaseRateEx;
	m_eq_data.extend[58] = m_nAttackRateEx;
	
	int n = eq_data.eq_control;
	n = (n & 0xfeff) | (m_nCBAlcSw << 8);
	eq_data.eq_control = n;
}

void CDlgAlcSet::InitEQValue(EQ_STRUCT_G_T& eq_data)
{
	m_nCBAlcSw = (eq_data.eq_control & 0x100) >> 8;
	if(m_nCBAlcSw == 0)
	{
		GetDlgItem(IDC_EDIT_HOLD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RISE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FALL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LIMIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_THRESHOLD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RATIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CG_VAR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RELEASE_RATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ATTACK_RATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RELEASE_RATE_EX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ATTACK_RATE_EX)->EnableWindow(FALSE);
	}
	
	m_nHold = m_eq_data.extend[48];
	m_nRise = m_eq_data.extend[49];
	m_nFall = m_eq_data.extend[50];
	m_nLimit = m_eq_data.extend[51];
	m_nThreshold = m_eq_data.extend[52];
	m_nRatio = m_eq_data.extend[53];
	m_nCgVar = m_eq_data.extend[54];
	m_nReleaseRate = m_eq_data.extend[55];
	m_nAttackRate = m_eq_data.extend[56];
	m_nReleaseRateEx = m_eq_data.extend[57];
	m_nAttackRateEx = m_eq_data.extend[58];
	
	UpdateData(FALSE);
}
