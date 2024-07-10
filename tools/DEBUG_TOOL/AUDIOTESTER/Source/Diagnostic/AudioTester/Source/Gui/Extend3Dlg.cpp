// Extend3Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "Extend3Dlg.h"

#include "AudioTesterSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtend3Dlg dialog


CExtend3Dlg::CExtend3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExtend3Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExtend3Dlg)
	m_ul_no_vad_cnt_thd = 0;
	m_ul_min_psne = 0;
	m_ul_max_temp_uamn = 0;
	m_ul_vad_thd = 0;
	m_ul_active_thd = 0;
	m_ul_noise_thd = 0;
	m_ul_max_psne = 0;
	m_ul_voise_burst = 0;
	m_ul_noise_tail = 0;
	m_ul_rfilter_delay = 0;
	m_ul_rfilter_tail = 0;
	m_ul_rfilter = 0;
	m_ul_dgain = 0;
	m_ul_sim_M = 0;
	m_ul_sim_fac = 0;
	m_ul_ns_factor = 0;
	m_ul_ns_limit = 0;
	m_ul_dis_snr_thd = 0;
	m_ul_dis_band_1k = 0;
	m_ul_dis_limit = 0;
	m_ul_drc_thd = 0;
	m_ul_drc_ratio = 0;
	m_ul_drc_dstep = 0;
	m_ul_drc_ustep = 0;
	m_ul_drc_cnt = 0;
	m_ul_clarity_switch = 0;
	m_aec_sa_in_select = 0;
	m_aec_sa_off = 0;
	m_aec_coeff_frozen = 0;
	m_ul_vad_switch = 0;
	m_ul_complex_switch = 0;
	m_ul_noise_est_switch = 0;
	m_ul_nr_switch = 0;
	m_ul_dis_switch = 0;
	m_ul_amb_noise_est_switch = 0;
	m_ul_amb_eq_switch = 0;
	m_ul_drc_switch = 0;
	m_ul_echo_residual_switch = 0;
	m_ul_hpf_set = 0;
	m_ul_lpf_set = 0;
	m_ul_amb_attack = 0;
	m_ul_amb_ndefault = 0;
	m_ul_amb_release = 0;
	m_ul_dac_limit = 0;
	m_ul_echo_ns_limit = 0;
	m_ul_ns_up_factor = 0;
	m_pArea = NULL;
	//}}AFX_DATA_INIT
}


void CExtend3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExtend3Dlg)
    DDX_Text(pDX, IDC_EDIT_UL_NO_VAD_CNT_THD, m_ul_no_vad_cnt_thd);
    DDV_MinMaxInt(pDX, m_ul_no_vad_cnt_thd, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_MIN_PSNE, m_ul_min_psne);
    DDV_MinMaxInt(pDX, m_ul_min_psne, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_MAX_TEMP_UAMN, m_ul_max_temp_uamn);
    DDV_MinMaxInt(pDX, m_ul_max_temp_uamn, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_VAD_THD, m_ul_vad_thd);
    DDV_MinMaxInt(pDX, m_ul_vad_thd, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_ACTIVE_THD, m_ul_active_thd);
    DDV_MinMaxInt(pDX, m_ul_active_thd, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_NOISE_THD, m_ul_noise_thd);
    DDV_MinMaxInt(pDX, m_ul_noise_thd, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_MAX_PSNE, m_ul_max_psne);
    DDV_MinMaxInt(pDX, m_ul_max_psne, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_VOISE_BURST, m_ul_voise_burst);
    DDV_MinMaxInt(pDX, m_ul_voise_burst, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_NOISE_TAIL, m_ul_noise_tail);
    DDV_MinMaxInt(pDX, m_ul_noise_tail, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_RFILTER_DELAY, m_ul_rfilter_delay);
    DDV_MinMaxInt(pDX, m_ul_rfilter_delay, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_RFILTER_TAIL, m_ul_rfilter_tail);
    DDV_MinMaxInt(pDX, m_ul_rfilter_tail, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_RFILTER, m_ul_rfilter);
    DDV_MinMaxInt(pDX, m_ul_rfilter, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_DGAIN, m_ul_dgain);
    DDV_MinMaxInt(pDX, m_ul_dgain, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_SIM_M, m_ul_sim_M);
    DDV_MinMaxInt(pDX, m_ul_sim_M, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_SIM_FAC, m_ul_sim_fac);
    DDV_MinMaxInt(pDX, m_ul_sim_fac, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_NS_FACTOR, m_ul_ns_factor);
    DDV_MinMaxInt(pDX, m_ul_ns_factor, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_NS_LIMIT, m_ul_ns_limit);
    DDV_MinMaxInt(pDX, m_ul_ns_limit, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_DIS_SNR_THD, m_ul_dis_snr_thd);
    DDV_MinMaxInt(pDX, m_ul_dis_snr_thd, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_DIS_BAND_1K, m_ul_dis_band_1k);
    DDV_MinMaxInt(pDX, m_ul_dis_band_1k, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_DIS_LIMIT, m_ul_dis_limit);
    DDV_MinMaxInt(pDX, m_ul_dis_limit, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_DRC_THD, m_ul_drc_thd);
    DDV_MinMaxInt(pDX, m_ul_drc_thd, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_DRC_RATIO, m_ul_drc_ratio);
    DDV_MinMaxInt(pDX, m_ul_drc_ratio, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_DRC_DSTEP, m_ul_drc_dstep);
    DDV_MinMaxInt(pDX, m_ul_drc_dstep, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_DRC_USTEP, m_ul_drc_ustep);
    DDV_MinMaxInt(pDX, m_ul_drc_ustep, -32768, 32767);
    DDX_Text(pDX, IDC_EDIT_UL_DRC_CNT, m_ul_drc_cnt);
    DDV_MinMaxInt(pDX, m_ul_drc_cnt, -32768, 32767);
	DDX_Text(pDX, IDC_EDIT_UL_CLARITY_SWITCH, m_ul_clarity_switch);
    DDV_MinMaxInt(pDX, m_ul_clarity_switch, 0, 15);
    DDX_Text(pDX, IDC_EDIT_AEC_SA_IN_SELECT, m_aec_sa_in_select);
    DDV_MinMaxInt(pDX, m_aec_sa_in_select, 0, 15);
    DDX_Text(pDX, IDC_EDIT_AEC_SA_OFF, m_aec_sa_off);
    DDV_MinMaxInt(pDX, m_aec_sa_off, 0, 15);
    DDX_Text(pDX, IDC_EDIT_AEC_COEFF_FROZEN, m_aec_coeff_frozen);
    DDV_MinMaxInt(pDX, m_aec_coeff_frozen, 0, 15);
    DDX_Text(pDX, IDC_EDIT_UL_VAD_SWITCH, m_ul_vad_switch);
    DDV_MinMaxInt(pDX, m_ul_vad_switch, 0, 15);
    DDX_Text(pDX, IDC_EDIT_UL_COMPLEX_SWITCH, m_ul_complex_switch);
    DDV_MinMaxInt(pDX, m_ul_complex_switch, 0, 15);
    DDX_Text(pDX, IDC_EDIT_UL_NOISE_EST_SWITCH, m_ul_noise_est_switch);
    DDV_MinMaxInt(pDX, m_ul_noise_est_switch, 0, 15);
    DDX_Text(pDX, IDC_EDIT_UL_NR_SWITCH, m_ul_nr_switch);
    DDV_MinMaxInt(pDX, m_ul_nr_switch, 0, 15);
    DDX_Text(pDX, IDC_EDIT_UL_DIS_SWITCH, m_ul_dis_switch);
    DDV_MinMaxInt(pDX, m_ul_dis_switch, 0, 15);
    DDX_Text(pDX, IDC_EDIT_UL_AMB_NOISE_EST_SWITCH, m_ul_amb_noise_est_switch);
    DDV_MinMaxInt(pDX, m_ul_amb_noise_est_switch, 0, 15);
    DDX_Text(pDX, IDC_EDIT_UL_AMB_EQ_SWITCH, m_ul_amb_eq_switch);
    DDV_MinMaxInt(pDX, m_ul_amb_eq_switch, 0, 15);
    DDX_Text(pDX, IDC_EDIT_UL_DRC_SWITCH, m_ul_drc_switch);
    DDV_MinMaxInt(pDX, m_ul_drc_switch, 0, 15);
    DDX_Text(pDX, IDC_EDIT_UL_ECHO_RESIDUAL_SWITCH, m_ul_echo_residual_switch);
    DDV_MinMaxInt(pDX, m_ul_echo_residual_switch, 0, 15);
    DDX_Text(pDX, IDC_EDIT_UL_HPF_SET, m_ul_hpf_set);
    DDV_MinMaxInt(pDX, m_ul_hpf_set, 0, 15);
    DDX_Text(pDX, IDC_EDIT_UL_LPF_SET, m_ul_lpf_set);
    DDV_MinMaxInt(pDX, m_ul_lpf_set, 0, 15);
	DDX_Text(pDX, IDC_EDIT_UL_AMB_ATTACK, m_ul_amb_attack);
	DDV_MinMaxInt(pDX, m_ul_amb_attack, -32767, 32768);
	DDX_Text(pDX, IDC_EDIT_UL_AMB_NDEFAULT, m_ul_amb_ndefault);
	DDV_MinMaxInt(pDX, m_ul_amb_ndefault, -32767, 32768);
	DDX_Text(pDX, IDC_EDIT_UL_AMB_RELEASE, m_ul_amb_release);
	DDV_MinMaxInt(pDX, m_ul_amb_release, -32767, 32768);
	DDX_Text(pDX, IDC_EDIT_UL_DAC_LIMIT, m_ul_dac_limit);
	DDV_MinMaxInt(pDX, m_ul_dac_limit, -32767, 32768);
	DDX_Text(pDX, IDC_EDIT_UL_ECHO_NS_LIMIT, m_ul_echo_ns_limit);
	DDV_MinMaxInt(pDX, m_ul_echo_ns_limit, -32767, 32768);
	DDX_Text(pDX, IDC_EDIT_UL_NS_UP_FACTOR, m_ul_ns_up_factor);
	DDV_MinMaxInt(pDX, m_ul_ns_up_factor, -32767, 32768);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExtend3Dlg, CDialog)
	//{{AFX_MSG_MAP(CExtend3Dlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtend3Dlg message handlers

BOOL CExtend3Dlg::OnInitDialog() 
{
    m_ul_no_vad_cnt_thd = m_pArea->m_ul_no_vad_cnt_thd;
    m_ul_min_psne = m_pArea->m_ul_min_psne;
    m_ul_max_temp_uamn = m_pArea->m_ul_max_temp_uamn;
    m_ul_vad_thd = m_pArea->m_ul_vad_thd;
    m_ul_active_thd = m_pArea->m_ul_active_thd;
    m_ul_noise_thd = m_pArea->m_ul_noise_thd;
    m_ul_max_psne = m_pArea->m_ul_max_psne;
    m_ul_voise_burst = m_pArea->m_ul_voise_burst;
    m_ul_noise_tail = m_pArea->m_ul_noise_tail;
    m_ul_rfilter_delay = m_pArea->m_ul_rfilter_delay;
    m_ul_rfilter_tail = m_pArea->m_ul_rfilter_tail;
    m_ul_rfilter = m_pArea->m_ul_rfilter;
    m_ul_dgain = m_pArea->m_ul_dgain;
    m_ul_sim_M = m_pArea->m_ul_sim_M;
    m_ul_sim_fac = m_pArea->m_ul_sim_fac;
    m_ul_ns_factor = m_pArea->m_ul_ns_factor;
    m_ul_ns_limit = m_pArea->m_ul_ns_limit;
    m_ul_dis_snr_thd = m_pArea->m_ul_dis_snr_thd;
    m_ul_dis_band_1k = m_pArea->m_ul_dis_band_1k;
    m_ul_dis_limit = m_pArea->m_ul_dis_limit;
    m_ul_drc_thd = m_pArea->m_ul_drc_thd;
    m_ul_drc_ratio = m_pArea->m_ul_drc_ratio;
    m_ul_drc_dstep = m_pArea->m_ul_drc_dstep;
    m_ul_drc_ustep = m_pArea->m_ul_drc_ustep;
    m_ul_drc_cnt = m_pArea->m_ul_drc_cnt;

	m_ul_clarity_switch = m_pArea->m_ul_clarity_switch;
    
    m_aec_sa_in_select = m_pArea->m_aec_sa_in_select;
    m_aec_sa_off = m_pArea->m_aec_sa_off;
    m_aec_coeff_frozen = m_pArea->m_aec_coeff_frozen;
    
    
    m_ul_vad_switch = m_pArea->m_ul_vad_switch;
    m_ul_complex_switch = m_pArea->m_ul_complex_switch;
    m_ul_noise_est_switch = m_pArea->m_ul_noise_est_switch;
    m_ul_nr_switch = m_pArea->m_ul_nr_switch;
    
    m_ul_dis_switch = m_pArea->m_ul_dis_switch;
    m_ul_amb_noise_est_switch = m_pArea->m_ul_amb_noise_est_switch;
    m_ul_amb_eq_switch = m_pArea->m_ul_amb_eq_switch;
    
    m_ul_drc_switch = m_pArea->m_ul_drc_switch;
    m_ul_echo_residual_switch = m_pArea->m_ul_echo_residual_switch;
    m_ul_hpf_set = m_pArea->m_ul_hpf_set;
    m_ul_lpf_set = m_pArea->m_ul_lpf_set;

	m_ul_amb_attack = m_pArea->m_ul_amb_attack;
	m_ul_amb_ndefault = m_pArea->m_ul_amb_ndefault;
	m_ul_amb_release = m_pArea->m_ul_amb_release;
	m_ul_dac_limit = m_pArea->m_ul_dac_limit;
	m_ul_echo_ns_limit = m_pArea->m_ul_echo_ns_limit;
	m_ul_ns_up_factor = m_pArea->m_ul_ns_up_factor;

    CDialog::OnInitDialog();
    
	CAudioTesterSheet* pSheet = (CAudioTesterSheet*)AfxGetMainWnd();
	CString strAll("All");
	pSheet->m_CtrlStatus.RefreshControl( strAll,IDD_DIALOG_MORE_PARAMETER3,this );
	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExtend3Dlg::OnOK() 
{
	CDialog::OnOK();

    m_pArea->m_ul_no_vad_cnt_thd = m_ul_no_vad_cnt_thd;
    m_pArea->m_ul_min_psne = m_ul_min_psne;
    m_pArea->m_ul_max_temp_uamn = m_ul_max_temp_uamn;
    m_pArea->m_ul_vad_thd = m_ul_vad_thd;
    m_pArea->m_ul_active_thd = m_ul_active_thd;
    m_pArea->m_ul_noise_thd = m_ul_noise_thd;
    m_pArea->m_ul_max_psne = m_ul_max_psne;
    m_pArea->m_ul_voise_burst = m_ul_voise_burst;
    m_pArea->m_ul_noise_tail = m_ul_noise_tail;
    m_pArea->m_ul_rfilter_delay = m_ul_rfilter_delay;
    m_pArea->m_ul_rfilter_tail = m_ul_rfilter_tail;
    m_pArea->m_ul_rfilter = m_ul_rfilter;
    m_pArea->m_ul_dgain = m_ul_dgain;
    m_pArea->m_ul_sim_M = m_ul_sim_M;
    m_pArea->m_ul_sim_fac = m_ul_sim_fac;
    m_pArea->m_ul_ns_factor = m_ul_ns_factor;
    m_pArea->m_ul_ns_limit = m_ul_ns_limit;
    m_pArea->m_ul_dis_snr_thd = m_ul_dis_snr_thd;
    m_pArea->m_ul_dis_band_1k = m_ul_dis_band_1k;
    m_pArea->m_ul_dis_limit = m_ul_dis_limit;
    m_pArea->m_ul_drc_thd = m_ul_drc_thd;
    m_pArea->m_ul_drc_ratio = m_ul_drc_ratio;
    m_pArea->m_ul_drc_dstep = m_ul_drc_dstep;
    m_pArea->m_ul_drc_ustep = m_ul_drc_ustep;
    m_pArea->m_ul_drc_cnt = m_ul_drc_cnt;

	m_pArea->m_ul_clarity_switch = m_ul_clarity_switch;
    
    m_pArea->m_aec_sa_in_select = m_aec_sa_in_select;
    m_pArea->m_aec_sa_off = m_aec_sa_off;
    m_pArea->m_aec_coeff_frozen = m_aec_coeff_frozen;
    
    
    m_pArea->m_ul_vad_switch = m_ul_vad_switch;
    m_pArea->m_ul_complex_switch = m_ul_complex_switch;
    m_pArea->m_ul_noise_est_switch = m_ul_noise_est_switch;
    m_pArea->m_ul_nr_switch = m_ul_nr_switch;
    
    m_pArea->m_ul_dis_switch = m_ul_dis_switch;
    m_pArea->m_ul_amb_noise_est_switch = m_ul_amb_noise_est_switch;
    m_pArea->m_ul_amb_eq_switch = m_ul_amb_eq_switch;
    
    m_pArea->m_ul_drc_switch = m_ul_drc_switch;
    m_pArea->m_ul_echo_residual_switch = m_ul_echo_residual_switch;
    m_pArea->m_ul_hpf_set = m_ul_hpf_set;
    m_pArea->m_ul_lpf_set = m_ul_lpf_set;

	m_pArea->m_ul_amb_attack = m_ul_amb_attack;
	m_pArea->m_ul_amb_ndefault = m_ul_amb_ndefault;
	m_pArea->m_ul_amb_release = m_ul_amb_release;
	m_pArea->m_ul_dac_limit = m_ul_dac_limit;
	m_pArea->m_ul_echo_ns_limit = m_ul_echo_ns_limit;
	m_pArea->m_ul_ns_up_factor = m_ul_ns_up_factor;

}
