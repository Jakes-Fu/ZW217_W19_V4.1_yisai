// Extend2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "Extend2Dlg.h"

#include "AudioTesterSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtend2Dlg dialog


CExtend2Dlg::CExtend2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExtend2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExtend2Dlg)
	m_dl_no_vad_cnt_thd = 0;
	m_dl_ns_up_factor = 0;
	m_dl_min_psne = 0;
	m_dl_max_temp_uamn = 0;
	m_dl_vad_thd = 0;
	m_dl_active_thd = 0;
	m_dl_noise_thd = 0;
	m_dl_max_psne = 0;
	m_dl_voise_burst = 0;
	m_dl_noise_tail = 0;
	m_dl_rfilter_delay = 0;
	m_dl_rfilter_tail = 0;
	m_dl_rfilter = 0;
	m_dl_dgain = 0;
	m_dl_sim_M = 0;
	m_dl_sim_fac = 0;
	m_dl_dac_limt = 0;
	m_dl_ns_factor = 0;
	m_dl_ns_limit = 0;
	m_dl_dis_snr_thd = 0;
	m_dl_dis_band_1k = 0;
	m_dl_dis_limit = 0;
	m_dl_drc_thd = 0;
	m_dl_drc_ratio = 0;
	m_dl_drc_dstep = 0;
	m_dl_drc_ustep = 0;
	m_dl_drc_cnt = 0;
	m_dl_clarity_switch = 0;
	m_dl_vad_switch = 0;
	m_dl_complex_switch = 0;
	m_dl_noise_est_switch = 0;
	m_dl_nr_switch = 0;
	m_dl_dis_switch = 0;
	m_dl_amb_noise_est_switch = 0;
	m_dl_amb_eq_switch = 0;
	m_dl_acoustic_eq_switch = 0;
	m_dl_drc_switch = 0;
	m_dl_dac_limit_switch = 0;
	m_dl_hpf_set = 0;
	m_dl_lpf_set = 0;
	m_pArea = NULL;
	//}}AFX_DATA_INIT
}


void CExtend2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExtend2Dlg)
	DDX_Text(pDX, IDC_EDIT_DL_NO_VAD_CNT_THD, m_dl_no_vad_cnt_thd);
    DDV_MinMaxInt(pDX, m_dl_no_vad_cnt_thd, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_MIN_PSNE, m_dl_min_psne);
    DDV_MinMaxInt(pDX, m_dl_min_psne, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_MAX_TEMP_UAMN, m_dl_max_temp_uamn);
    DDV_MinMaxInt(pDX, m_dl_max_temp_uamn, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_VAD_THD, m_dl_vad_thd);
    DDV_MinMaxInt(pDX, m_dl_vad_thd, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_ACTIVE_THD, m_dl_active_thd);
    DDV_MinMaxInt(pDX, m_dl_active_thd, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_NOISE_THD, m_dl_noise_thd);
    DDV_MinMaxInt(pDX, m_dl_noise_thd, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_MAX_PSNE, m_dl_max_psne);
    DDV_MinMaxInt(pDX, m_dl_max_psne, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_VOISE_BURST, m_dl_voise_burst);
    DDV_MinMaxInt(pDX, m_dl_voise_burst, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_NOISE_TAIL, m_dl_noise_tail);
    DDV_MinMaxInt(pDX, m_dl_noise_tail, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_RFILTER_DELAY, m_dl_rfilter_delay);
    DDV_MinMaxInt(pDX, m_dl_rfilter_delay, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_RFILTER_TAIL, m_dl_rfilter_tail);
    DDV_MinMaxInt(pDX, m_dl_rfilter_tail, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_RFILTER, m_dl_rfilter);
    DDV_MinMaxInt(pDX, m_dl_rfilter, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_DGAIN, m_dl_dgain);
    DDV_MinMaxInt(pDX, m_dl_dgain, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_SIM_M, m_dl_sim_M);
    DDV_MinMaxInt(pDX, m_dl_sim_M, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_SIM_FAC, m_dl_sim_fac);
    DDV_MinMaxInt(pDX, m_dl_sim_fac, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_DAC_LIMIT, m_dl_dac_limt);
    DDV_MinMaxInt(pDX, m_dl_dac_limt, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_NS_FACTOR, m_dl_ns_factor);
    DDV_MinMaxInt(pDX, m_dl_ns_factor, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_NS_LIMIT, m_dl_ns_limit);
    DDV_MinMaxInt(pDX, m_dl_ns_limit, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_DIS_SNR_THD, m_dl_dis_snr_thd);
    DDV_MinMaxInt(pDX, m_dl_dis_snr_thd, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_DIS_BAND_1K, m_dl_dis_band_1k);
    DDV_MinMaxInt(pDX, m_dl_dis_band_1k, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_DIS_LIMIT, m_dl_dis_limit);
    DDV_MinMaxInt(pDX, m_dl_dis_limit, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_DRC_THD, m_dl_drc_thd);
    DDV_MinMaxInt(pDX, m_dl_drc_thd, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_DRC_RATIO, m_dl_drc_ratio);
    DDV_MinMaxInt(pDX, m_dl_drc_ratio, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_DRC_DSTEP, m_dl_drc_dstep);
    DDV_MinMaxInt(pDX, m_dl_drc_dstep, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_DRC_USTEP, m_dl_drc_ustep);
    DDV_MinMaxInt(pDX, m_dl_drc_ustep, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_DL_DRC_CNT, m_dl_drc_cnt);
    DDV_MinMaxInt(pDX, m_dl_drc_cnt, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_DL_NS_UP_FACTOR, m_dl_ns_up_factor);
	DDV_MinMaxInt(pDX, m_dl_ns_up_factor, -32767, 32768);

	DDX_Text(pDX, IDC_EDIT_DL_CLARITY_SWITCH, m_dl_clarity_switch);
    DDV_MinMaxInt(pDX, m_dl_clarity_switch, 0, 15);
	
	DDX_Text(pDX, IDC_EDIT_DL_VAD_SWITCH, m_dl_vad_switch);
    DDV_MinMaxInt(pDX, m_dl_vad_switch, 0, 15);
    DDX_Text(pDX, IDC_EDIT_DL_COMPLEX_SWITCH, m_dl_complex_switch);
    DDV_MinMaxInt(pDX, m_dl_complex_switch, 0, 15);
    DDX_Text(pDX, IDC_EDIT_DL_NOISE_EST_SWITCH, m_dl_noise_est_switch);
    DDV_MinMaxInt(pDX, m_dl_noise_est_switch, 0, 15);
    DDX_Text(pDX, IDC_EDIT_DL_NR_SWITCH, m_dl_nr_switch);
    DDV_MinMaxInt(pDX, m_dl_nr_switch, 0, 15);
    
    DDX_Text(pDX, IDC_EDIT_DL_DIS_SWITCH, m_dl_dis_switch);
    DDV_MinMaxInt(pDX, m_dl_dis_switch, 0, 15);
    DDX_Text(pDX, IDC_EDIT_DL_AMB_NOISE_EST_SWITCH, m_dl_amb_noise_est_switch);
    DDV_MinMaxInt(pDX, m_dl_amb_noise_est_switch, 0, 15);
    DDX_Text(pDX, IDC_EDIT_DL_AMB_EQ_SWITCH, m_dl_amb_eq_switch);
    DDV_MinMaxInt(pDX, m_dl_amb_eq_switch, 0, 15);
    DDX_Text(pDX, IDC_EDIT_DL_ACOUSTIC_EQ_SWITCH, m_dl_acoustic_eq_switch);
    DDV_MinMaxInt(pDX, m_dl_acoustic_eq_switch, 0, 15);
    
    DDX_Text(pDX, IDC_EDIT_DL_DRC_SWITCH, m_dl_drc_switch);
    DDV_MinMaxInt(pDX, m_dl_drc_switch, 0, 15);
    DDX_Text(pDX, IDC_EDIT_DL_DAC_LIMIT_SWITCH, m_dl_dac_limit_switch);
    DDV_MinMaxInt(pDX, m_dl_dac_limit_switch, 0, 15);
    DDX_Text(pDX, IDC_EDIT_DL_HPF_SET, m_dl_hpf_set);
    DDV_MinMaxInt(pDX, m_dl_hpf_set, 0, 15);
    DDX_Text(pDX, IDC_EDIT_DL_LPF_SET, m_dl_lpf_set);
    DDV_MinMaxInt(pDX, m_dl_lpf_set, 0, 15);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExtend2Dlg, CDialog)
	//{{AFX_MSG_MAP(CExtend2Dlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtend2Dlg message handlers

BOOL CExtend2Dlg::OnInitDialog() 
{
    m_dl_no_vad_cnt_thd = m_pArea->m_dl_no_vad_cnt_thd;
    m_dl_min_psne = m_pArea->m_dl_min_psne;
    m_dl_max_temp_uamn = m_pArea->m_dl_max_temp_uamn;
    m_dl_vad_thd = m_pArea->m_dl_vad_thd;
    m_dl_active_thd = m_pArea->m_dl_active_thd;
    m_dl_noise_thd = m_pArea->m_dl_noise_thd;
    m_dl_max_psne = m_pArea->m_dl_max_psne;
    m_dl_voise_burst = m_pArea->m_dl_voise_burst;
    m_dl_noise_tail = m_pArea->m_dl_noise_tail;
    m_dl_rfilter_delay = m_pArea->m_dl_rfilter_delay;
    m_dl_rfilter_tail = m_pArea->m_dl_rfilter_tail;
    m_dl_rfilter = m_pArea->m_dl_rfilter;
    m_dl_dgain = m_pArea->m_dl_dgain;
    m_dl_sim_M = m_pArea->m_dl_sim_M;
    m_dl_sim_fac = m_pArea->m_dl_sim_fac;
    m_dl_dac_limt = m_pArea->m_dl_dac_limt;
    m_dl_ns_factor = m_pArea->m_dl_ns_factor;
    m_dl_ns_limit = m_pArea->m_dl_ns_limit;
    m_dl_dis_snr_thd = m_pArea->m_dl_dis_snr_thd;
    m_dl_dis_band_1k = m_pArea->m_dl_dis_band_1k;
    m_dl_dis_limit = m_pArea->m_dl_dis_limit;
    m_dl_drc_thd = m_pArea->m_dl_drc_thd;
    m_dl_drc_ratio = m_pArea->m_dl_drc_ratio;
    m_dl_drc_dstep = m_pArea->m_dl_drc_dstep;
    m_dl_drc_ustep = m_pArea->m_dl_drc_ustep;
    m_dl_drc_cnt = m_pArea->m_dl_drc_cnt;

	m_dl_clarity_switch = m_pArea->m_dl_clarity_switch;

    m_dl_vad_switch = m_pArea->m_dl_vad_switch;
    m_dl_complex_switch = m_pArea->m_dl_complex_switch;
    m_dl_noise_est_switch = m_pArea->m_dl_noise_est_switch;
    m_dl_nr_switch = m_pArea->m_dl_nr_switch;
    
    m_dl_dis_switch = m_pArea->m_dl_dis_switch;
    m_dl_amb_noise_est_switch = m_pArea->m_dl_amb_noise_est_switch;
    m_dl_amb_eq_switch = m_pArea->m_dl_amb_eq_switch;
    m_dl_acoustic_eq_switch = m_pArea->m_dl_acoustic_eq_switch;
    
    m_dl_drc_switch = m_pArea->m_dl_drc_switch;
    m_dl_dac_limit_switch = m_pArea->m_dl_dac_limit_switch;
    m_dl_hpf_set = m_pArea->m_dl_hpf_set;
    m_dl_lpf_set = m_pArea->m_dl_lpf_set;

	m_dl_ns_up_factor = m_pArea->m_dl_ns_up_factor;

    CDialog::OnInitDialog();
	
	CAudioTesterSheet* pSheet = (CAudioTesterSheet*)AfxGetMainWnd();
	CString strAll("All");
	pSheet->m_CtrlStatus.RefreshControl( strAll,IDD_DIALOG_MORE_PARAMETER2,this );
	CenterWindow();
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExtend2Dlg::OnOK() 
{
	CDialog::OnOK();

    m_pArea->m_dl_no_vad_cnt_thd = m_dl_no_vad_cnt_thd;
    m_pArea->m_dl_min_psne = m_dl_min_psne;
    m_pArea->m_dl_max_temp_uamn = m_dl_max_temp_uamn;
    m_pArea->m_dl_vad_thd = m_dl_vad_thd;
    m_pArea->m_dl_active_thd = m_dl_active_thd;
    m_pArea->m_dl_noise_thd = m_dl_noise_thd;
    m_pArea->m_dl_max_psne = m_dl_max_psne;
    m_pArea->m_dl_voise_burst = m_dl_voise_burst;
    m_pArea->m_dl_noise_tail = m_dl_noise_tail;
    m_pArea->m_dl_rfilter_delay = m_dl_rfilter_delay;
    m_pArea->m_dl_rfilter_tail = m_dl_rfilter_tail;
    m_pArea->m_dl_rfilter = m_dl_rfilter;
    m_pArea->m_dl_dgain = m_dl_dgain;
    m_pArea->m_dl_sim_M = m_dl_sim_M;
    m_pArea->m_dl_sim_fac = m_dl_sim_fac;
    m_pArea->m_dl_dac_limt = m_dl_dac_limt;
    m_pArea->m_dl_ns_factor = m_dl_ns_factor;
    m_pArea->m_dl_ns_limit = m_dl_ns_limit;
    m_pArea->m_dl_dis_snr_thd = m_dl_dis_snr_thd;
    m_pArea->m_dl_dis_band_1k = m_dl_dis_band_1k;
    m_pArea->m_dl_dis_limit = m_dl_dis_limit;
    m_pArea->m_dl_drc_thd = m_dl_drc_thd;
    m_pArea->m_dl_drc_ratio = m_dl_drc_ratio;
    m_pArea->m_dl_drc_dstep = m_dl_drc_dstep;
    m_pArea->m_dl_drc_ustep = m_dl_drc_ustep;
    m_pArea->m_dl_drc_cnt = m_dl_drc_cnt;

	m_pArea->m_dl_clarity_switch = m_dl_clarity_switch;
    m_pArea->m_dl_vad_switch = m_dl_vad_switch;
    m_pArea->m_dl_complex_switch = m_dl_complex_switch;
    m_pArea->m_dl_noise_est_switch = m_dl_noise_est_switch;
    m_pArea->m_dl_nr_switch = m_dl_nr_switch;
    
    m_pArea->m_dl_dis_switch = m_dl_dis_switch;
    m_pArea->m_dl_amb_noise_est_switch = m_dl_amb_noise_est_switch;
    m_pArea->m_dl_amb_eq_switch = m_dl_amb_eq_switch;
    m_pArea->m_dl_acoustic_eq_switch = m_dl_acoustic_eq_switch;
    
    m_pArea->m_dl_drc_switch = m_dl_drc_switch;
    m_pArea->m_dl_dac_limit_switch = m_dl_dac_limit_switch;
    m_pArea->m_dl_hpf_set = m_dl_hpf_set;
    m_pArea->m_dl_lpf_set = m_dl_lpf_set;

	m_pArea->m_dl_ns_up_factor = m_dl_ns_up_factor;
}
