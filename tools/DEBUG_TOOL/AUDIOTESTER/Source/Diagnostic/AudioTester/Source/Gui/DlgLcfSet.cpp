// DlgLcfSet.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgLcfSet.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLcfSet dialog


CDlgLcfSet::CDlgLcfSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLcfSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLcfSet)
		// NOTE: the ClassWizard will add member initialization here
	m_nCBLcfSw = 0;
	m_nCBFilterType = 0;
	m_nFp = 0;
	m_nF1fp = 0;
	m_nF2fp = 0;
	m_fF1g0 = 0.0;
	m_fF1g1 = 0.0;
	m_fF2g0 = 0.0;
	m_fF2g1 = 0.0;
	//}}AFX_DATA_INIT
	memset(&m_VolumeData, 0, sizeof(VOLUME_STRUCT));
	m_bInit = FALSE;
}


void CDlgLcfSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLcfSet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_STATIC_LCF_GRAPH, m_stcGraph);
	DDX_CBIndex(pDX, IDC_COMBO_LCF_SW, m_nCBLcfSw);
	DDX_CBIndex(pDX, IDC_COMBO_FILTER_TYPE, m_nCBFilterType);
	DDX_Text(pDX, IDC_EDIT_FP, m_nFp);
	DDV_MinMaxInt(pDX, m_nFp, 0, 22050);
	DDX_Text(pDX, IDC_EDIT_F1_G0, m_fF1g0);
	DDV_MinMaxFloat(pDX, m_fF1g0, -72.0, 18.0);
	DDX_Text(pDX, IDC_EDIT_F1_G1, m_fF1g1);
	DDV_MinMaxFloat(pDX, m_fF1g1, -72.0, 18.0);
	DDX_Text(pDX, IDC_EDIT_F1_FP, m_nF1fp);
	DDV_MinMaxInt(pDX, m_nF1fp, 0, 22050);
	DDX_Text(pDX, IDC_EDIT_F2_G0, m_fF2g0);
	DDV_MinMaxFloat(pDX, m_fF2g0, -72.0, 18.0);
	DDX_Text(pDX, IDC_EDIT_F2_G1, m_fF2g1);
	DDV_MinMaxFloat(pDX, m_fF2g1, -72.0, 18.0);
	DDX_Text(pDX, IDC_EDIT_F2_FP, m_nF2fp);
	DDV_MinMaxInt(pDX, m_nF2fp, 0, 22050);
	DDX_Text(pDX, IDC_EDIT_SAMPLE_RATE, m_nSamplerate);
	DDX_Control(pDX, IDC_COMBO_FILTER_TYPE, m_boxFilterType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLcfSet, CDialog)
	//{{AFX_MSG_MAP(CDlgLcfSet)
	ON_CBN_SELCHANGE(IDC_COMBO_FILTER_TYPE, OnSelchangeComboFilterType)
	ON_BN_CLICKED(IDC_BTN_LCF_REFLASH, OnBtnLcfReflash)
	ON_CBN_SELCHANGE(IDC_COMBO_LCF_SW, OnSelchangeComboLcfSw)
	ON_EN_CHANGE(IDC_EDIT_F1_FP, OnChangeEditF1Fp)
	ON_EN_CHANGE(IDC_EDIT_F1_G0, OnChangeEditF1G0)
	ON_EN_CHANGE(IDC_EDIT_F1_G1, OnChangeEditF1G1)
	ON_EN_CHANGE(IDC_EDIT_F2_FP, OnChangeEditF2Fp)
	ON_EN_CHANGE(IDC_EDIT_F2_G0, OnChangeEditF2G0)
	ON_EN_CHANGE(IDC_EDIT_F2_G1, OnChangeEditF2G1)
	ON_EN_CHANGE(IDC_EDIT_FP, OnChangeEditFp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLcfSet message handlers

BOOL CDlgLcfSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//GetDlgItem(IDC_EDIT_SAMPLE_RATE)->SetWindowText(_T("44100"));
	if(m_bSerialPara)
	{
		InitEQValue(m_eq_data);
		//GetDlgItem(IDC_COMBO_FILTER_TYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_FILTER_TYPE)->EnableWindow(FALSE);
	}
	else
	{
		InitValue(m_VolumeData);
		GetDlgItem(IDC_COMBO_FILTER_TYPE)->EnableWindow(FALSE);
	}
	
	m_stcGraph.SetValue(m_nF1fp,m_nF2fp,m_fF1g0,m_fF1g1,m_fF2g0,m_fF2g1, m_nCBLcfSw, m_nCBFilterType,m_nFp);
	m_stcGraph.SetSampleRate(m_nSamplerate);
	m_bInit = TRUE;

	//teana hu 2012.06.14
	for(int i = 0; i < NUM_FILTER_TYPE; i++)
	{
		m_boxFilterType.AddString(strFilterType[i]);
	}
	m_boxFilterType.SetCurSel(m_nCBFilterType);
	//
	RefreshControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLcfSet::OnOK() 
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

void CDlgLcfSet::SetValue(VOLUME_STRUCT& volume_data)
{
	memcpy(&m_VolumeData, &volume_data, sizeof(VOLUME_STRUCT));
}

void CDlgLcfSet::InitValue(VOLUME_STRUCT& volume_data)
{
	m_nCBLcfSw = volume_data.app_config_info_set.aud_proc_exp_control[1] & 0x1;
	Enable(m_nCBLcfSw);
	m_nCBFilterType = (volume_data.reserve[0] & 0x700) >> 8;
	if(m_nCBFilterType == 0)
	{
		GetDlgItem(IDC_EDIT_FP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_HZ)->ShowWindow(SW_HIDE);
		m_nF1fp = volume_data.reserve[3];
		m_nF2fp = volume_data.reserve[6];
		m_fF1g0 = (float)volume_data.reserve[1] / 10.0;
		m_fF1g0 = (float)((short)floorf(m_fF1g0 * 10) / 10.0);
		m_fF1g1 = (float)volume_data.reserve[2] / 10.0;
		m_fF1g1 = (float)((short)floorf(m_fF1g1 * 10) / 10.0);
		m_fF2g0 = (float)volume_data.reserve[4] / 10.0;
		m_fF2g0 = (float)((short)floorf(m_fF2g0 * 10) / 10.0);
		m_fF2g1 = (float)volume_data.reserve[5] / 10.0;
		m_fF2g1 = (float)((short)floorf(m_fF2g1 * 10) / 10.0);
	}
	else
	{
		GetDlgItem(IDC_EDIT_F1_G0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_F1_G1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_F1_FP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_F2_G0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_F2_G1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_F2_FP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_HZ)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_FP)->ShowWindow(SW_SHOW);
		m_nFp = volume_data.reserve[1];
	}
	
	UpdateData(FALSE);
}

void CDlgLcfSet::GetValue(VOLUME_STRUCT& volume_data)
{
	if(!UpdateData())
	{
		return;
	}
	if(m_nCBFilterType != 0)
	{
		volume_data.reserve[1] = m_nFp;
	}
	else
	{
		volume_data.reserve[3] = m_nF1fp;
		volume_data.reserve[6] = m_nF2fp;
		volume_data.reserve[1] = ceilf(m_fF1g0 * 10.0 );
		volume_data.reserve[2] = ceilf(m_fF1g1 * 10.0 );
		volume_data.reserve[4] = ceilf(m_fF2g0 * 10.0 );
		volume_data.reserve[5] = ceilf(m_fF2g1 * 10.0 );
	}

	volume_data.reserve[0] = (volume_data.reserve[0] & 0xf8ff) | (m_nCBFilterType << 8);
	int n = volume_data.app_config_info_set.aud_proc_exp_control[1];
	n = (n & 0xfffe) | m_nCBLcfSw;
	volume_data.app_config_info_set.aud_proc_exp_control[1] = n;
}

void CDlgLcfSet::FetchValue(VOLUME_STRUCT& volume_data)
{
	memcpy(&volume_data, &m_VolumeData, sizeof(VOLUME_STRUCT));
}

void CDlgLcfSet::OnSelchangeComboFilterType() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	RefreshControl();

	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgLcfSet::Enable(BOOL bEnable)
{
	GetDlgItem(IDC_EDIT_F1_G0)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_F1_G1)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_F1_FP)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_F2_G0)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_F2_G1)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_F2_FP)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_FP)->EnableWindow(bEnable);
	//GetDlgItem(IDC_COMBO_FILTER_TYPE)->EnableWindow(bEnable);
}

void CDlgLcfSet::OnBtnLcfReflash() 
{
	// TODO: Add your control notification handler code here
	ReflashGraph();
}

void CDlgLcfSet::ReflashGraph()
{
	if(!UpdateData())
	{
		return;
	}
	m_stcGraph.SetValue(m_nF1fp,m_nF2fp,m_fF1g0,m_fF1g1,m_fF2g0,m_fF2g1, m_nCBLcfSw, m_nCBFilterType,m_nFp);
	m_stcGraph.SetSampleRate(m_nSamplerate);
//	m_stcGraph.EraseBK();
	m_stcGraph.Invalidate(FALSE);
}

void CDlgLcfSet::OnSelchangeComboLcfSw() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	Enable(m_nCBLcfSw);
	if(m_bInit)
	{
		ReflashGraph();
	}
	
}

void CDlgLcfSet::OnChangeEditF1Fp() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgLcfSet::OnChangeEditF1G0() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_F1_G0)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgLcfSet::OnChangeEditF1G1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_F1_G1)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgLcfSet::OnChangeEditF2Fp() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgLcfSet::OnChangeEditF2G0() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_F2_G0)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgLcfSet::OnChangeEditF2G1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strtext;
	GetDlgItem(IDC_EDIT_F2_G1)->GetWindowText(strtext);
	if(strtext == _T("-"))
	{
		return;
	}
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgLcfSet::OnChangeEditFp() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bInit)
	{
		ReflashGraph();
	}
}

void CDlgLcfSet::SetEQValue(EQ_STRUCT_G_T& eq_data)
{
	memcpy(&m_eq_data, &eq_data, sizeof(EQ_STRUCT_G_T));
}

void CDlgLcfSet::FetchEQValue(EQ_STRUCT_G_T& eq_data)
{
	memcpy(&eq_data, &m_eq_data, sizeof(EQ_STRUCT_G_T));
}

void CDlgLcfSet::GetEQValue(EQ_STRUCT_G_T& eq_data)
{
	if(!UpdateData())
	{
		return;
	}
	if(m_nCBFilterType != 0)
	{
		m_eq_data.eq_mode[0].eq_band[6].boostdB = m_nFp;
	}
	else
	{
		m_eq_data.eq_mode[0].eq_band[5].boostdB = m_nF1fp;
		m_eq_data.eq_mode[0].eq_band[6].q = m_nF2fp;
		m_eq_data.eq_mode[0].eq_band[5].f0 = ceilf(m_fF1g0 * 10.0 );
		m_eq_data.eq_mode[0].eq_band[5].q = ceilf(m_fF1g1 * 10.0 );
		m_eq_data.eq_mode[0].eq_band[5].gaindB = ceilf(m_fF2g0 * 10.0 );
		m_eq_data.eq_mode[0].eq_band[6].f0 = ceilf(m_fF2g1 * 10.0 );
	}
	int n = m_eq_data.eq_mode[0].band_control;
	n = (n & 0xf87f) | (m_nCBFilterType << 8);
	n |= (m_nCBLcfSw << 7);
	
	m_eq_data.eq_mode[0].band_control = n;
}

void CDlgLcfSet::InitEQValue(EQ_STRUCT_G_T& eq_data)
{
	m_nCBLcfSw = (m_eq_data.eq_mode[0].band_control & 0x0080) >> 7;
	Enable(m_nCBLcfSw);
	m_nCBFilterType = (m_eq_data.eq_mode[0].band_control & 0x0700) >> 8;
	if(m_nCBFilterType == 0)
	{
		GetDlgItem(IDC_EDIT_FP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_HZ)->ShowWindow(SW_HIDE);
		m_nF1fp = m_eq_data.eq_mode[0].eq_band[5].boostdB;
		m_nF2fp = m_eq_data.eq_mode[0].eq_band[6].q;
		m_fF1g0 = (float)m_eq_data.eq_mode[0].eq_band[5].f0/ 10.0;
		m_fF1g0 = (float)((short)floorf(m_fF1g0 * 10) / 10.0);
		m_fF1g1 = (float)m_eq_data.eq_mode[0].eq_band[5].q / 10.0;
		m_fF1g1 = (float)((short)floorf(m_fF1g1 * 10) / 10.0);
		m_fF2g0 = (float)m_eq_data.eq_mode[0].eq_band[5].gaindB / 10.0;
		m_fF2g0 = (float)((short)floorf(m_fF2g0 * 10) / 10.0);
		m_fF2g1 = (float)m_eq_data.eq_mode[0].eq_band[6].f0 / 10.0;
		m_fF2g1 = (float)((short)floorf(m_fF2g1 * 10) / 10.0);
	}
	else
	{
		GetDlgItem(IDC_EDIT_F1_G0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_F1_G1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_F1_FP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_F2_G0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_F2_G1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_F2_FP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_HZ)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_FP)->ShowWindow(SW_SHOW);
		m_nFp = m_eq_data.eq_mode[0].eq_band[6].boostdB;
	}

	UpdateData(FALSE);
}

void CDlgLcfSet::RefreshControl()
{
	if(m_nCBFilterType == 0)
	{
		GetDlgItem(IDC_EDIT_FP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_HZ)->ShowWindow(SW_HIDE);
		
		GetDlgItem(IDC_EDIT_F1_G0)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_F1_G1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_F1_FP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_F2_G0)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_F2_G1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_F2_FP)->ShowWindow(SW_SHOW);
		
		GetDlgItem(IDC_STATIC_F1G0)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_F1G1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_F2G0)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_F2G1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_F1FP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_F2FP)->ShowWindow(SW_SHOW);
		
		GetDlgItem(IDC_STATIC_DB_F1G0)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_DB_F1G1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_DB_F2G0)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_DB_F2G1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_HZ_F1FP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_HZ_F2FP)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_EDIT_F1_G0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_F1_G1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_F1_FP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_F2_G0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_F2_G1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_F2_FP)->ShowWindow(SW_HIDE);
		
		GetDlgItem(IDC_STATIC_F1G0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_F1G1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_F2G0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_F2G1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_F1FP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_F2FP)->ShowWindow(SW_HIDE);
		
		GetDlgItem(IDC_STATIC_DB_F1G0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DB_F1G1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DB_F2G0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DB_F2G1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_HZ_F1FP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_HZ_F2FP)->ShowWindow(SW_HIDE);
		
		GetDlgItem(IDC_STATIC_FP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_HZ)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_FP)->ShowWindow(SW_SHOW);
	}

}

BOOL CDlgLcfSet::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->wParam == VK_RETURN) 
	{
		ReflashGraph();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
