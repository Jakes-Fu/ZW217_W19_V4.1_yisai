// DlgRecordDpEq.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgRecordDpEq.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRecordDpEq dialog


CDlgRecordDpEq::CDlgRecordDpEq(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRecordDpEq::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRecordDpEq)
		// NOTE: the ClassWizard will add member initialization here
	m_nSampleRate = 44100;
	m_fBoost1 = 0.0;
	m_fBoost2 = 0.0;
	m_fBoost3 = 0.0;
	m_fBoost4 = 0.0;
	m_fBoost5 = 0.0;
	m_fBoost6 = 0.0;
	m_nFO1 = 0;
	m_nFO2 = 0;
	m_nFO3 = 0;
	m_nFO4 = 0;
	m_nFO5 = 0;
	m_nFO6 = 0;
	m_nDF1 = 0;
	m_nDF2 = 0;
	m_nDF3 = 0;
	m_nDF4 = 0;
	m_nDF5 = 0;
	m_nDF6 = 0;
	m_fGain1 = 0.0;
	m_fGain2 = 0.0;
	m_fGain3 = 0.0;
	m_fGain4 = 0.0;
	m_fGain5 = 0.0;
	m_fGain6 = 0.0;
	m_fMasterGain = 0.0;
	m_nSampleRate = 44100;
	m_nCBIndexStereo = 0;
	m_nCBIndexSW = 0;
	//}}AFX_DATA_INIT
	m_fMasterGainBak = 0.0;
	m_crBack = RGB(192,192,192);
	m_bInit = FALSE;
}


void CDlgRecordDpEq::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRecordDpEq)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_SLIDER_BOOST1, m_SliderBoost1);
	DDX_Control(pDX, IDC_SLIDER_BOOST2, m_SliderBoost2);
	DDX_Control(pDX, IDC_SLIDER_BOOST3, m_SliderBoost3);
	DDX_Control(pDX, IDC_SLIDER_BOOST4, m_SliderBoost4);
	DDX_Control(pDX, IDC_SLIDER_BOOST5, m_SliderBoost5);
	DDX_Control(pDX, IDC_SLIDER_BOOST6, m_SliderBoost6);
	DDX_Text(pDX, IDC_EDIT_BOOST1, m_fBoost1);
	DDV_MinMaxFloat(pDX, m_fBoost1, -72, 18);
	DDX_Text(pDX, IDC_EDIT_BOOST2, m_fBoost2);
	DDV_MinMaxFloat(pDX, m_fBoost2, -72, 18);
	DDX_Text(pDX, IDC_EDIT_BOOST3, m_fBoost3);
	DDV_MinMaxFloat(pDX, m_fBoost3, -72, 18);
	DDX_Text(pDX, IDC_EDIT_BOOST4, m_fBoost4);
	DDV_MinMaxFloat(pDX, m_fBoost4, -72, 18);
	DDX_Text(pDX, IDC_EDIT_BOOST5, m_fBoost5);
	DDV_MinMaxFloat(pDX, m_fBoost5, -72, 18);
	DDX_Text(pDX, IDC_EDIT_BOOST6, m_fBoost6);
	DDV_MinMaxFloat(pDX, m_fBoost6, -72, 18);
	DDX_Control(pDX, IDC_CHECK_SW1, m_chkSw1);
	DDX_Control(pDX, IDC_CHECK_SW2, m_chkSw2);
	DDX_Control(pDX, IDC_CHECK_SW3, m_chkSw3);
	DDX_Control(pDX, IDC_CHECK_SW4, m_chkSw4);
	DDX_Control(pDX, IDC_CHECK_SW5, m_chkSw5);
	DDX_Control(pDX, IDC_CHECK_SW6, m_chkSw6);
	DDX_Control(pDX, IDC_SLIDER_FO1, m_SliderFO1);
	DDX_Control(pDX, IDC_SLIDER_FO2, m_SliderFO2);
	DDX_Control(pDX, IDC_SLIDER_FO3, m_SliderFO3);
	DDX_Control(pDX, IDC_SLIDER_FO4, m_SliderFO4);
	DDX_Control(pDX, IDC_SLIDER_FO5, m_SliderFO5);
	DDX_Control(pDX, IDC_SLIDER_FO6, m_SliderFO6);
	DDX_Text(pDX, IDC_EDIT_FO1, m_nFO1);
	DDV_MinMaxInt(pDX, m_nFO1, 0, 22050);
	DDX_Text(pDX, IDC_EDIT_FO2, m_nFO2);
	DDV_MinMaxInt(pDX, m_nFO2, 0, 22050);
	DDX_Text(pDX, IDC_EDIT_FO3, m_nFO3);
	DDV_MinMaxInt(pDX, m_nFO3, 0, 22050);
	DDX_Text(pDX, IDC_EDIT_FO4, m_nFO4);
	DDV_MinMaxInt(pDX, m_nFO4, 0, 22050);
	DDX_Text(pDX, IDC_EDIT_FO5, m_nFO5);
	DDV_MinMaxInt(pDX, m_nFO5, 0, 22050);
	DDX_Text(pDX, IDC_EDIT_FO6, m_nFO6);
	DDV_MinMaxInt(pDX, m_nFO6, 0, 22050);
	DDX_Text(pDX, IDC_EDIT_DF1, m_nDF1);
	DDV_MinMaxInt(pDX, m_nDF1, 0, 22050);
	DDX_Text(pDX, IDC_EDIT_DF2, m_nDF2);
	DDV_MinMaxInt(pDX, m_nDF2, 0, 22050);
	DDX_Text(pDX, IDC_EDIT_DF3, m_nDF3);
	DDV_MinMaxInt(pDX, m_nDF3, 0, 22050);
	DDX_Text(pDX, IDC_EDIT_DF4, m_nDF4);
	DDV_MinMaxInt(pDX, m_nDF4, 0, 22050);
	DDX_Text(pDX, IDC_EDIT_DF5, m_nDF5);
	DDV_MinMaxInt(pDX, m_nDF5, 0, 22050);
	DDX_Text(pDX, IDC_EDIT_DF6, m_nDF6);
	DDV_MinMaxInt(pDX, m_nDF6, 0, 22050);
	DDX_Text(pDX, IDC_EDIT_EQ_GAIN1, m_fGain1);
	DDV_MinMaxFloat(pDX, m_fGain1, -72, 0);
	DDX_Text(pDX, IDC_EDIT_EQ_GAIN2, m_fGain2);
	DDV_MinMaxFloat(pDX, m_fGain2, -72, 0);
	DDX_Text(pDX, IDC_EDIT_EQ_GAIN3, m_fGain3);
	DDV_MinMaxFloat(pDX, m_fGain3, -72, 0);
	DDX_Text(pDX, IDC_EDIT_EQ_GAIN4, m_fGain4);
	DDV_MinMaxFloat(pDX, m_fGain4, -72, 0);
	DDX_Text(pDX, IDC_EDIT_EQ_GAIN5, m_fGain5);
	DDV_MinMaxFloat(pDX, m_fGain5, -72, 0);
	DDX_Text(pDX, IDC_EDIT_EQ_GAIN6, m_fGain6);
	DDV_MinMaxFloat(pDX, m_fGain6, -72, 0);
	DDX_Text(pDX, IDC_EDIT_EQ_MATSER_GAIN, m_fMasterGain);
	DDV_MinMaxFloat(pDX, m_fMasterGain, -60, 30);
	DDX_Text(pDX, IDC_EDT_DP_SAMPLE_RATE, m_nSampleRate);
	DDX_Control(pDX, IDC_STATIC_DP_EQ, m_stcGraph);
	DDX_CBIndex(pDX, IDC_COMBO_RECORD_EQ_STEREO, m_nCBIndexStereo);
	DDX_CBIndex(pDX, IDC_COMBO_RECORD_EQ_SW, m_nCBIndexSW);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRecordDpEq, CDialog)
	//{{AFX_MSG_MAP(CDlgRecordDpEq)
	ON_BN_CLICKED(IDC_CHECK_SW1, OnCheckSw1)
	ON_BN_CLICKED(IDC_CHECK_SW2, OnCheckSw2)
	ON_BN_CLICKED(IDC_CHECK_SW3, OnCheckSw3)
	ON_BN_CLICKED(IDC_CHECK_SW4, OnCheckSw4)
	ON_BN_CLICKED(IDC_CHECK_SW5, OnCheckSw5)
	ON_BN_CLICKED(IDC_CHECK_SW6, OnCheckSw6)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_BOOST1, OnChangeEditBoost1)
	ON_EN_CHANGE(IDC_EDIT_BOOST2, OnChangeEditBoost2)
	ON_EN_CHANGE(IDC_EDIT_BOOST3, OnChangeEditBoost3)
	ON_EN_CHANGE(IDC_EDIT_BOOST4, OnChangeEditBoost4)
	ON_EN_CHANGE(IDC_EDIT_BOOST5, OnChangeEditBoost5)
	ON_EN_CHANGE(IDC_EDIT_BOOST6, OnChangeEditBoost6)
	ON_EN_CHANGE(IDC_EDIT_FO1, OnChangeEditFo1)
	ON_EN_CHANGE(IDC_EDIT_FO2, OnChangeEditFo2)
	ON_EN_CHANGE(IDC_EDIT_FO3, OnChangeEditFo3)
	ON_EN_CHANGE(IDC_EDIT_FO4, OnChangeEditFo4)
	ON_EN_CHANGE(IDC_EDIT_FO5, OnChangeEditFo5)
	ON_EN_CHANGE(IDC_EDIT_FO6, OnChangeEditFo6)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_BOOST1, OnReleasedcaptureSliderBoost1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_BOOST2, OnReleasedcaptureSliderBoost2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_BOOST3, OnReleasedcaptureSliderBoost3)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_BOOST4, OnReleasedcaptureSliderBoost4)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_BOOST5, OnReleasedcaptureSliderBoost5)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_BOOST6, OnReleasedcaptureSliderBoost6)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_FO1, OnReleasedcaptureSliderFo1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_FO2, OnReleasedcaptureSliderFo2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_FO3, OnReleasedcaptureSliderFo3)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_FO4, OnReleasedcaptureSliderFo4)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_FO5, OnReleasedcaptureSliderFo5)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_FO6, OnReleasedcaptureSliderFo6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRecordDpEq message handlers

BOOL CDlgRecordDpEq::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitValue(m_eq_data);
	InitSLider();
	ReflashGraph();
	m_bInit = TRUE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRecordDpEq::SetValue(EQ_STRUCT_G_T& eq_data)
{
	memcpy(&m_eq_data, &eq_data, sizeof(EQ_STRUCT_G_T));
}

void CDlgRecordDpEq::FetchValue(EQ_STRUCT_G_T& eq_data)
{
	memcpy(&eq_data, &m_eq_data, sizeof(EQ_STRUCT_G_T));
}

void CDlgRecordDpEq::GetValue(EQ_STRUCT_G_T& eq_data)
{
	UpdateData();
	short nValue = 0;

	if(m_chkSw1.GetCheck() == BST_CHECKED)
	{
		nValue |= 0x100;
	}

	if(m_chkSw2.GetCheck() == BST_CHECKED)
	{
		nValue |= 0x200;
	}

	if(m_chkSw3.GetCheck() == BST_CHECKED)
	{
		nValue |= 0x400;
	}

	if(m_chkSw4.GetCheck() == BST_CHECKED)
	{
		nValue |= 0x800;
	}

	if(m_chkSw5.GetCheck() == BST_CHECKED)
	{
		nValue |= 0x1000;
	}

	if(m_chkSw6.GetCheck() == BST_CHECKED)
	{
		nValue |= 0x2000;

	}

	eq_data.extend[19] &= 0x00ff;
	eq_data.extend[19] |= nValue;
	eq_data.extend[19] |= (m_nCBIndexStereo << 14);
	eq_data.extend[19] |= (m_nCBIndexSW << 15);

	
	eq_data.extend[21] = m_nFO1;
	eq_data.extend[25] = m_nFO2;
	eq_data.extend[29] = m_nFO3;
	eq_data.extend[33] = m_nFO4;
	eq_data.extend[37] = m_nFO5;
	eq_data.extend[41] = m_nFO6;
	
	eq_data.extend[22] = m_nDF1;
	eq_data.extend[26] = m_nDF2;
	eq_data.extend[30] = m_nDF3;
	eq_data.extend[34] = m_nDF4;
	eq_data.extend[38] = m_nDF5;
	eq_data.extend[42] = m_nDF6;
	
	eq_data.extend[23] = ceilf(m_fBoost1 * 10.0);
	eq_data.extend[27] = ceilf(m_fBoost2 * 10.0);
	eq_data.extend[31] = ceilf(m_fBoost3 * 10.0);
	eq_data.extend[35] = ceilf(m_fBoost4 * 10.0);
	eq_data.extend[39] = ceilf(m_fBoost5 * 10.0);
	eq_data.extend[43] = ceilf(m_fBoost6 * 10.0);
	
	eq_data.extend[24] = ceilf(m_fGain1 * 10.0);
	eq_data.extend[28] = ceilf(m_fGain2 * 10.0);
	eq_data.extend[32] = ceilf(m_fGain3 * 10.0);
	eq_data.extend[36] = ceilf(m_fGain4 * 10.0);
	eq_data.extend[40] = ceilf(m_fGain5 * 10.0);
	eq_data.extend[44] = ceilf(m_fGain6 * 10.0);

	if(m_fMasterGain != m_fMasterGainBak)
	{
		eq_data.extend[20] = ceilf(((double)pow(10, m_fMasterGain/20.0)) * 1024.0);
	}
}

void CDlgRecordDpEq::InitValue(EQ_STRUCT_G_T& eq_data)
{
	short nValue = eq_data.extend[19];

	m_chkSw1.SetCheck((nValue & 0x100) >> 8);
	m_chkSw2.SetCheck((nValue & 0x200) >> 9);
	m_chkSw3.SetCheck((nValue & 0x400) >> 10);
	m_chkSw4.SetCheck((nValue & 0x800) >> 11);
	m_chkSw5.SetCheck((nValue & 0x1000) >> 12);
	m_chkSw6.SetCheck((nValue & 0x2000) >> 13);

	m_nFO1 = eq_data.extend[21];
	m_nFO2 = eq_data.extend[25];
	m_nFO3 = eq_data.extend[29];
	m_nFO4 = eq_data.extend[33];
	m_nFO5 = eq_data.extend[37];
	m_nFO6 = eq_data.extend[41];

	m_nDF1 = eq_data.extend[22];
	m_nDF2 = eq_data.extend[26];
	m_nDF3 = eq_data.extend[30];
	m_nDF4 = eq_data.extend[34];
	m_nDF5 = eq_data.extend[38];
	m_nDF6 = eq_data.extend[42];

	m_fBoost1 = eq_data.extend[23];
	m_fBoost2 = eq_data.extend[27];
	m_fBoost3 = eq_data.extend[31];
	m_fBoost4 = eq_data.extend[35];
	m_fBoost5 = eq_data.extend[39];
	m_fBoost6 = eq_data.extend[43];
	m_fBoost1 = (float)m_fBoost1 / 10.0;
	m_fBoost1 = (float)((short)floorf(m_fBoost1 * 10) / 10.0);
	m_fBoost2 = (float)m_fBoost2 / 10.0;
	m_fBoost2 = (float)((short)floorf(m_fBoost2 * 10) / 10.0);
	m_fBoost3 = (float)m_fBoost3 / 10.0;
	m_fBoost3 = (float)((short)floorf(m_fBoost3 * 10) / 10.0);
	m_fBoost4 = (float)m_fBoost4 / 10.0;
	m_fBoost4 = (float)((short)floorf(m_fBoost4 * 10) / 10.0);
	m_fBoost5 = (float)m_fBoost5 / 10.0;
	m_fBoost5 = (float)((short)floorf(m_fBoost5 * 10) / 10.0);
	m_fBoost6 = (float)m_fBoost6 / 10.0;
	m_fBoost6 = (float)((short)floorf(m_fBoost6 * 10) / 10.0);

	m_fGain1 = eq_data.extend[24];
	m_fGain2 = eq_data.extend[28];
	m_fGain3 = eq_data.extend[32];
	m_fGain4 = eq_data.extend[36];
	m_fGain5 = eq_data.extend[40];
	m_fGain6 = eq_data.extend[44];
	m_fGain1 = (float)m_fGain1 / 10.0;
	m_fGain1 = (float)((short)floorf(m_fGain1 * 10) / 10.0);
	m_fGain2 = (float)m_fGain2 / 10.0;
	m_fGain2 = (float)((short)floorf(m_fGain2 * 10) / 10.0);
	m_fGain3 = (float)m_fGain3 / 10.0;
	m_fGain3 = (float)((short)floorf(m_fGain3 * 10) / 10.0);
	m_fGain4 = (float)m_fGain4 / 10.0;
	m_fGain4 = (float)((short)floorf(m_fGain4 * 10) / 10.0);
	m_fGain5 = (float)m_fGain5 / 10.0;
	m_fGain5 = (float)((short)floorf(m_fGain5 * 10) / 10.0);
	m_fGain6 = (float)m_fGain6 / 10.0;
	m_fGain6 = (float)((short)floorf(m_fGain6 * 10) / 10.0);
	

	nValue = eq_data.extend[20];
	if(nValue == 0)
	{
		nValue = 1;
	}
	m_fMasterGain = (double)(20 * (double)log10((double)nValue/(double)1024));
	m_fMasterGain = (double)((short)floorf(m_fMasterGain * 10) / 10.0);
	m_fMasterGain = (m_fMasterGain < - 60) ? -60 : m_fMasterGain;
	m_fMasterGainBak = m_fMasterGain;

	m_nCBIndexStereo = (eq_data.extend[19] & 0x4000) >> 14;
	m_nCBIndexSW = (eq_data.extend[19] & 0x8000) >> 15;

	RefreshControls();

	UpdateData(FALSE);
}

void CDlgRecordDpEq::OnOK() 
{
	// TODO: Add extra validation here
	GetValue(m_eq_data);
	
	CDialog::OnOK();
}

void CDlgRecordDpEq::RefreshControls()
{
	if(m_chkSw1.GetCheck() == BST_CHECKED)
	{
		m_SliderBoost1.EnableWindow(TRUE);
		m_SliderBoost1.SetPrimaryColor(RGB(128,128,128));
		m_SliderFO1.EnableWindow(TRUE);
		m_SliderFO1.SetPrimaryColor(RGB(128,128,128));
		GetDlgItem(IDC_EDIT_BOOST1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FO1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DF1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_EQ_GAIN1)->EnableWindow(TRUE);
	}
	else
	{
		m_SliderBoost1.EnableWindow(FALSE);
		m_SliderBoost1.SetPrimaryColor(m_crBack);
		m_SliderFO1.EnableWindow(FALSE);
		m_SliderFO1.SetPrimaryColor(m_crBack);
		GetDlgItem(IDC_EDIT_BOOST1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DF1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_EQ_GAIN1)->EnableWindow(FALSE);
	}

	if(m_chkSw2.GetCheck() == BST_CHECKED)
	{
		m_SliderBoost2.EnableWindow(TRUE);
		m_SliderBoost2.SetPrimaryColor(RGB(128,128,128));
		m_SliderFO2.EnableWindow(TRUE);
		m_SliderFO2.SetPrimaryColor(RGB(128,128,128));
		GetDlgItem(IDC_EDIT_BOOST2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FO2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DF2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_EQ_GAIN2)->EnableWindow(TRUE);
	}
	else
	{
		m_SliderBoost2.EnableWindow(FALSE);
		m_SliderBoost2.SetPrimaryColor(m_crBack);
		m_SliderFO2.EnableWindow(FALSE);
		m_SliderFO2.SetPrimaryColor(m_crBack);
		GetDlgItem(IDC_EDIT_BOOST2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FO2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DF2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_EQ_GAIN2)->EnableWindow(FALSE);
	}

	if(m_chkSw3.GetCheck() == BST_CHECKED)
	{
		m_SliderBoost3.EnableWindow(TRUE);
		m_SliderBoost3.SetPrimaryColor(RGB(128,128,128));
		m_SliderFO3.EnableWindow(TRUE);
		m_SliderFO3.SetPrimaryColor(RGB(128,128,128));
		GetDlgItem(IDC_EDIT_BOOST3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FO3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DF3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_EQ_GAIN3)->EnableWindow(TRUE);
	}
	else
	{
		m_SliderBoost3.EnableWindow(FALSE);
		m_SliderBoost3.SetPrimaryColor(m_crBack);
		m_SliderFO3.EnableWindow(FALSE);
		m_SliderFO3.SetPrimaryColor(m_crBack);
		GetDlgItem(IDC_EDIT_BOOST3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FO3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DF3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_EQ_GAIN3)->EnableWindow(FALSE);
	}

	if(m_chkSw4.GetCheck() == BST_CHECKED)
	{
		m_SliderBoost4.EnableWindow(TRUE);
		m_SliderBoost4.SetPrimaryColor(RGB(128,128,128));
		m_SliderFO4.EnableWindow(TRUE);
		m_SliderFO4.SetPrimaryColor(RGB(128,128,128));
		GetDlgItem(IDC_EDIT_BOOST4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FO4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DF4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_EQ_GAIN4)->EnableWindow(TRUE);
	}
	else
	{
		m_SliderBoost4.EnableWindow(FALSE);
		m_SliderBoost4.SetPrimaryColor(m_crBack);
		m_SliderFO4.EnableWindow(FALSE);
		m_SliderFO4.SetPrimaryColor(m_crBack);
		GetDlgItem(IDC_EDIT_BOOST4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FO4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DF4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_EQ_GAIN4)->EnableWindow(FALSE);
	}

	if(m_chkSw5.GetCheck() == BST_CHECKED)
	{
		m_SliderBoost5.EnableWindow(TRUE);
		m_SliderBoost5.SetPrimaryColor(RGB(128,128,128));
		m_SliderFO5.EnableWindow(TRUE);
		m_SliderFO5.SetPrimaryColor(RGB(128,128,128));
		GetDlgItem(IDC_EDIT_BOOST5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FO5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DF5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_EQ_GAIN5)->EnableWindow(TRUE);
	}
	else
	{
		m_SliderBoost5.EnableWindow(FALSE);
		m_SliderBoost5.SetPrimaryColor(m_crBack);
		m_SliderFO5.EnableWindow(FALSE);
		m_SliderFO5.SetPrimaryColor(m_crBack);
		GetDlgItem(IDC_EDIT_BOOST5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FO5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DF5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_EQ_GAIN5)->EnableWindow(FALSE);
	}

	if(m_chkSw6.GetCheck() == BST_CHECKED)
	{
		m_SliderBoost6.EnableWindow(TRUE);
		m_SliderBoost6.SetPrimaryColor(RGB(128,128,128));
		m_SliderFO6.EnableWindow(TRUE);
		m_SliderFO6.SetPrimaryColor(RGB(128,128,128));
		GetDlgItem(IDC_EDIT_BOOST6)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FO6)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DF6)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_EQ_GAIN6)->EnableWindow(TRUE);
	}
	else
	{
		m_SliderBoost6.EnableWindow(FALSE);
		m_SliderBoost6.SetPrimaryColor(m_crBack);
		m_SliderFO6.EnableWindow(FALSE);
		m_SliderFO6.SetPrimaryColor(m_crBack);
		GetDlgItem(IDC_EDIT_BOOST6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FO6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DF6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_EQ_GAIN6)->EnableWindow(FALSE);
	}
}

void CDlgRecordDpEq::OnCheckSw1() 
{
	// TODO: Add your control notification handler code here
	RefreshControls();
	
}

void CDlgRecordDpEq::OnCheckSw2() 
{
	// TODO: Add your control notification handler code here
	RefreshControls();
}

void CDlgRecordDpEq::OnCheckSw3() 
{
	// TODO: Add your control notification handler code here
	RefreshControls();
}

void CDlgRecordDpEq::OnCheckSw4() 
{
	// TODO: Add your control notification handler code here
	RefreshControls();
}

void CDlgRecordDpEq::OnCheckSw5() 
{
	// TODO: Add your control notification handler code here
	RefreshControls();
}

void CDlgRecordDpEq::OnCheckSw6() 
{
	// TODO: Add your control notification handler code here
	RefreshControls();
}

void CDlgRecordDpEq::InitSLider()
{	
	m_SliderBoost1.SetBuddy( GetDlgItem( IDC_EDIT_BOOST1 ));
	m_SliderBoost1.SetRange(MIN_STEP_SLIDER_V,MAX_STEP_SLIDER_V);
	m_SliderBoost1.SetPos(m_fBoost1, STEP_SLIDER_V);
	m_SliderBoost2.SetBuddy( GetDlgItem( IDC_EDIT_BOOST2 ));
	m_SliderBoost2.SetRange(MIN_STEP_SLIDER_V,MAX_STEP_SLIDER_V);
	m_SliderBoost2.SetPos(m_fBoost2, STEP_SLIDER_V);
	m_SliderBoost3.SetBuddy( GetDlgItem( IDC_EDIT_BOOST3 ));
	m_SliderBoost3.SetRange(MIN_STEP_SLIDER_V,MAX_STEP_SLIDER_V);
	m_SliderBoost3.SetPos(m_fBoost3, STEP_SLIDER_V);
	m_SliderBoost4.SetBuddy( GetDlgItem( IDC_EDIT_BOOST4 ));
	m_SliderBoost4.SetRange(MIN_STEP_SLIDER_V,MAX_STEP_SLIDER_V);
	m_SliderBoost4.SetPos(m_fBoost4, STEP_SLIDER_V);
	m_SliderBoost5.SetBuddy( GetDlgItem( IDC_EDIT_BOOST5 ));
	m_SliderBoost5.SetRange(MIN_STEP_SLIDER_V,MAX_STEP_SLIDER_V);
	m_SliderBoost5.SetPos(m_fBoost5, STEP_SLIDER_V);
	m_SliderBoost6.SetBuddy( GetDlgItem( IDC_EDIT_BOOST6 ));
	m_SliderBoost6.SetRange(MIN_STEP_SLIDER_V,MAX_STEP_SLIDER_V);
	m_SliderBoost6.SetPos(m_fBoost6, STEP_SLIDER_V);

	
	m_SliderFO1.SetBuddy( GetDlgItem( IDC_EDIT_FO1 ) );
	m_SliderFO1.SetRange(MIN_STEP_SLIDER_H,MAX_STEP_SLIDER_H);
	m_SliderFO1.SetPos(m_nFO1,0,1);
	m_SliderFO2.SetBuddy( GetDlgItem( IDC_EDIT_FO2 ) );
	m_SliderFO2.SetRange(MIN_STEP_SLIDER_H,MAX_STEP_SLIDER_H);
	m_SliderFO2.SetPos(m_nFO2,0,1);
	m_SliderFO3.SetBuddy( GetDlgItem( IDC_EDIT_FO3 ) );
	m_SliderFO3.SetRange(MIN_STEP_SLIDER_H,MAX_STEP_SLIDER_H);
	m_SliderFO3.SetPos(m_nFO3,0,1);
	m_SliderFO4.SetBuddy( GetDlgItem( IDC_EDIT_FO4) );
	m_SliderFO4.SetRange(MIN_STEP_SLIDER_H,MAX_STEP_SLIDER_H);
	m_SliderFO4.SetPos(m_nFO4,0,1);
	m_SliderFO5.SetBuddy( GetDlgItem( IDC_EDIT_FO5 ) );
	m_SliderFO5.SetRange(MIN_STEP_SLIDER_H,MAX_STEP_SLIDER_H);
	m_SliderFO5.SetPos(m_nFO5,0,1);
	m_SliderFO6.SetBuddy( GetDlgItem( IDC_EDIT_FO6 ) );
	m_SliderFO6.SetRange(MIN_STEP_SLIDER_H,MAX_STEP_SLIDER_H);
	m_SliderFO6.SetPos(m_nFO6,0,1);
	
}


void CDlgRecordDpEq::SetSliderPos()
{
	if(!UpdateData())
	{
		return;
	}
	
	m_SliderBoost1.SetPos(m_fBoost1, STEP_SLIDER_V,0,0,FALSE);
	m_SliderBoost2.SetPos(m_fBoost2, STEP_SLIDER_V,0,0,FALSE);
	m_SliderBoost3.SetPos(m_fBoost3, STEP_SLIDER_V,0,0,FALSE);
	m_SliderBoost4.SetPos(m_fBoost4, STEP_SLIDER_V,0,0,FALSE);
	m_SliderBoost5.SetPos(m_fBoost5, STEP_SLIDER_V,0,0,FALSE);
	m_SliderBoost6.SetPos(m_fBoost6, STEP_SLIDER_V,0,0,FALSE);
	
	m_SliderFO1.SetPos(m_nFO1,0,1,0,FALSE);
	m_SliderFO2.SetPos(m_nFO2,0,1,0,FALSE);
	m_SliderFO3.SetPos(m_nFO3,0,1,0,FALSE);
	m_SliderFO4.SetPos(m_nFO4,0,1,0,FALSE);
	m_SliderFO5.SetPos(m_nFO5,0,1,0,FALSE);
	m_SliderFO6.SetPos(m_nFO6,0,1,0,FALSE);
}

void CDlgRecordDpEq::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CBuddiedSliderCtrl* pSlider = (CBuddiedSliderCtrl*)pScrollBar;
	pSlider->ReflectedScrollMessage(0,1);
	ReflashGraph();
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgRecordDpEq::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CBuddiedSliderCtrl* pSlider = (CBuddiedSliderCtrl*)pScrollBar;
	pSlider->ReflectedScrollMessage(STEP_SLIDER_V);
	
	ReflashGraph();
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CDlgRecordDpEq::OnChangeEditBoost1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_BOOST1)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		//m_fBoost1 = (m_fBoost1 > 18) ? 18 : m_fBoost1;
		//m_fBoost1 = (m_fBoost1 < -72) ? -72 : m_fBoost1;
		m_SliderBoost1.SetPos(m_fBoost1, STEP_SLIDER_V, 0, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgRecordDpEq::OnChangeEditBoost2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_BOOST2)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		//m_fBoost2 = (m_fBoost2 > 18) ? 18 : m_fBoost2;
		//m_fBoost2 = (m_fBoost2 < -72) ? -72 : m_fBoost2;
		m_SliderBoost2.SetPos(m_fBoost2, STEP_SLIDER_V, 0, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgRecordDpEq::OnChangeEditBoost3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_BOOST3)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		//m_fBoost3 = (m_fBoost3 > 18) ? 18 : m_fBoost3;
		//m_fBoost3 = (m_fBoost3 < -72) ? -72 : m_fBoost3;
		m_SliderBoost3.SetPos(m_fBoost3, STEP_SLIDER_V, 0, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgRecordDpEq::OnChangeEditBoost4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_BOOST4)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		//m_fBoost4 = (m_fBoost4 > 18) ? 18 : m_fBoost4;
		//m_fBoost4 = (m_fBoost4 < -72) ? -72 : m_fBoost4;
		m_SliderBoost4.SetPos(m_fBoost4, STEP_SLIDER_V, 0, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgRecordDpEq::OnChangeEditBoost5() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_BOOST5)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		//m_fBoost5 = (m_fBoost5 > 18) ? 18 : m_fBoost5;
		//m_fBoost5 = (m_fBoost5 < -72) ? -72 : m_fBoost5;
		m_SliderBoost5.SetPos(m_fBoost5, STEP_SLIDER_V, 0, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgRecordDpEq::OnChangeEditBoost6() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_BOOST6)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		//m_fBoost6 = (m_fBoost6 > 18) ? 18 : m_fBoost6;
		//m_fBoost6 = (m_fBoost6 < -72) ? -72 : m_fBoost6;
		m_SliderBoost6.SetPos(m_fBoost6, STEP_SLIDER_V, 0, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgRecordDpEq::OnChangeEditFo1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		//m_nFO1 = (m_nFO1 > 22050) ? 22050 : m_nFO1;
		//m_nFO1 = (m_nFO1 < 0) ? 0 : m_nFO1;
		m_SliderFO1.SetPos(m_nFO1, 0, 1, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgRecordDpEq::OnChangeEditFo2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		//m_nFO2 = (m_nFO2 > 22050) ? 22050 : m_nFO2;
		//m_nFO2 = (m_nFO2 < 0) ? 0 : m_nFO2;
		m_SliderFO2.SetPos(m_nFO2, 0, 1, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgRecordDpEq::OnChangeEditFo3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		//m_nFO3 = (m_nFO3 > 22050) ? 22050 : m_nFO3;
		//m_nFO3 = (m_nFO3 < 0) ? 0 : m_nFO3;
		m_SliderFO3.SetPos(m_nFO3, 0, 1, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgRecordDpEq::OnChangeEditFo4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		//m_nFO4 = (m_nFO4 > 22050) ? 22050 : m_nFO4;
		//m_nFO4 = (m_nFO4 < 0) ? 0 : m_nFO4;
		m_SliderFO4.SetPos(m_nFO4, 0, 1, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgRecordDpEq::OnChangeEditFo5() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		//m_nFO5 = (m_nFO5 > 22050) ? 22050 : m_nFO5;
		//m_nFO5 = (m_nFO5 < 0) ? 0 : m_nFO5;
		m_SliderFO5.SetPos(m_nFO5, 0, 1, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgRecordDpEq::OnChangeEditFo6() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		if(!UpdateData())
		{
			return;
		}
		//m_nFO6 = (m_nFO6 > 22050) ? 22050 : m_nFO6;
		//m_nFO6 = (m_nFO6 < 0) ? 0 : m_nFO6;
		m_SliderFO6.SetPos(m_nFO6, 0, 1, 0, FALSE);
		ReflashGraph();
	}
}

void CDlgRecordDpEq::ReflashGraph()
{
	if(!UpdateData())
	{
		return;
	}

	m_stcGraph.SetMasterGain(m_fMasterGain);

	m_stcGraph.SetValue(0, m_chkSw1.GetCheck(), m_nFO1, m_nDF1, m_fBoost1, m_fGain1);
	m_stcGraph.SetValue(1, m_chkSw2.GetCheck(), m_nFO2, m_nDF2, m_fBoost2, m_fGain2);
	m_stcGraph.SetValue(2, m_chkSw3.GetCheck(), m_nFO3, m_nDF3, m_fBoost3, m_fGain3);
	m_stcGraph.SetValue(3, m_chkSw4.GetCheck(), m_nFO4, m_nDF4, m_fBoost4, m_fGain4);
	m_stcGraph.SetValue(4, m_chkSw5.GetCheck(), m_nFO5, m_nDF5, m_fBoost5, m_fGain5);
	m_stcGraph.SetValue(5, m_chkSw6.GetCheck(), m_nFO6, m_nDF6, m_fBoost6, m_fGain6);

	m_stcGraph.Invalidate(FALSE);
}

HBRUSH CDlgRecordDpEq::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if( pWnd->GetDlgCtrlID() == IDC_STATIC_EQ_BAND1 )
    {
        return CreateSolidBrush( RGB( 255,255,0 ) );
    }
	if( pWnd->GetDlgCtrlID() == IDC_STATIC_EQ_BAND2 )
    {
        return CreateSolidBrush( RGB( 255,0,0 ) );
    }
	if( pWnd->GetDlgCtrlID() == IDC_STATIC_EQ_BAND3 )
    {
        return CreateSolidBrush( RGB( 128,0,255 ) );
    }
	if( pWnd->GetDlgCtrlID() == IDC_STATIC_EQ_BAND4 )
    {
        return CreateSolidBrush( RGB( 0,255,255 ) );
    }
	if( pWnd->GetDlgCtrlID() == IDC_STATIC_EQ_BAND5 )
    {
        return CreateSolidBrush( RGB( 0,0,255 ) );
    }
	if( pWnd->GetDlgCtrlID() == IDC_STATIC_EQ_BAND6 )
    {
        return CreateSolidBrush( RGB( 128,64,64 ) );
    }
	if( pWnd->GetDlgCtrlID() == IDC_STATIC_EQ_RESULT )
	{
		return CreateSolidBrush( RGB( 0,255,0 ) );
	}
	// TODO: Return
	return hbr;
}

void CDlgRecordDpEq::OnReleasedcaptureSliderBoost1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgRecordDpEq::OnReleasedcaptureSliderBoost2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgRecordDpEq::OnReleasedcaptureSliderBoost3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgRecordDpEq::OnReleasedcaptureSliderBoost4(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgRecordDpEq::OnReleasedcaptureSliderBoost5(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgRecordDpEq::OnReleasedcaptureSliderBoost6(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgRecordDpEq::OnReleasedcaptureSliderFo1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgRecordDpEq::OnReleasedcaptureSliderFo2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgRecordDpEq::OnReleasedcaptureSliderFo3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgRecordDpEq::OnReleasedcaptureSliderFo4(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgRecordDpEq::OnReleasedcaptureSliderFo5(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

void CDlgRecordDpEq::OnReleasedcaptureSliderFo6(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
	*pResult = 0;
}

BOOL CDlgRecordDpEq::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->wParam == VK_RETURN) 
	{
		if(!UpdateData())
		{
			return FALSE;
		}

		if(pMsg->hwnd == GetDlgItem(IDC_EDIT_BOOST1)->GetSafeHwnd())
		{
			m_SliderBoost1.SetPos(m_fBoost1, STEP_SLIDER_V);
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_BOOST2)->GetSafeHwnd())
		{
			m_SliderBoost2.SetPos(m_fBoost2, STEP_SLIDER_V);
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_BOOST3)->GetSafeHwnd())
		{
			m_SliderBoost3.SetPos(m_fBoost3, STEP_SLIDER_V);
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_BOOST4)->GetSafeHwnd())
		{
			m_SliderBoost4.SetPos(m_fBoost4, STEP_SLIDER_V);
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_BOOST5)->GetSafeHwnd())
		{
			m_SliderBoost5.SetPos(m_fBoost5, STEP_SLIDER_V);
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_BOOST6)->GetSafeHwnd())
		{
			m_SliderBoost6.SetPos(m_fBoost6, STEP_SLIDER_V);
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_FO1)->GetSafeHwnd())
		{
			m_SliderFO1.SetPos(m_nFO1,0,1);
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_FO2)->GetSafeHwnd())
		{
			m_SliderFO2.SetPos(m_nFO2,0,1);
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_FO3)->GetSafeHwnd())
		{
			m_SliderFO3.SetPos(m_nFO3,0,1);
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_FO4)->GetSafeHwnd())
		{
			m_SliderFO4.SetPos(m_nFO4,0,1);
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_FO5)->GetSafeHwnd())
		{
			m_SliderFO5.SetPos(m_nFO5,0,1);
		}
		else if(pMsg->hwnd == GetDlgItem(IDC_EDIT_FO6)->GetSafeHwnd())
		{
			m_SliderFO6.SetPos(m_nFO6,0,1);
		}

		ReflashGraph();

		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
