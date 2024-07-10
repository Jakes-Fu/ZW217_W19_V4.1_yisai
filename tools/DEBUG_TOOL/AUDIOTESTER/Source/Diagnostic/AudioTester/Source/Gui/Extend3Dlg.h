#if !defined(AFX_EXTEND3DLG_H__8614CA88_5471_4413_9CBA_EDE27CBAB649__INCLUDED_)
#define AFX_EXTEND3DLG_H__8614CA88_5471_4413_9CBA_EDE27CBAB649__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Extend3Dlg.h : header file
//
#include "BMPCtrlWithSpecRect.h"

/////////////////////////////////////////////////////////////////////////////
// CExtend3Dlg dialog

class CExtend3Dlg : public CDialog
{
// Construction
public:
	CExtend3Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExtend3Dlg)
	enum { IDD = IDD_DIALOG_MORE_PARAMETER3 };
    short		m_ul_no_vad_cnt_thd;
    short m_ul_min_psne;
    short m_ul_max_temp_uamn;
    short m_ul_vad_thd;
    short m_ul_active_thd;
    short m_ul_noise_thd;
    short m_ul_max_psne;
    short m_ul_voise_burst;
    short m_ul_noise_tail;
    short m_ul_rfilter_delay;
    short m_ul_rfilter_tail;
    short m_ul_rfilter;
    short m_ul_dgain;
    short m_ul_sim_M;
    short m_ul_sim_fac;
    short m_ul_ns_factor;
    short m_ul_ns_limit;
    short m_ul_dis_snr_thd;
    short m_ul_dis_band_1k;
    short m_ul_dis_limit;
    short m_ul_drc_thd;
    short m_ul_drc_ratio;
    short m_ul_drc_dstep;
    short m_ul_drc_ustep;
    short m_ul_drc_cnt;
	short m_ul_clarity_switch;
    short m_aec_sa_in_select;
    short m_aec_sa_off;
    short m_aec_coeff_frozen;
    short m_ul_vad_switch;
    short m_ul_complex_switch;
    short m_ul_noise_est_switch;
    short m_ul_nr_switch;
    short m_ul_dis_switch;
    short m_ul_amb_noise_est_switch;
    short m_ul_amb_eq_switch;
    short m_ul_drc_switch;
    short m_ul_echo_residual_switch;
    short m_ul_hpf_set;
    short m_ul_lpf_set;
	short		m_ul_amb_attack;
	short		m_ul_amb_ndefault;
	short		m_ul_amb_release;
	short		m_ul_dac_limit;
	short		m_ul_echo_ns_limit;
	short		m_ul_ns_up_factor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtend3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
    SpecialRectOfUlSettings* m_pArea;
        
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExtend3Dlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTEND3DLG_H__8614CA88_5471_4413_9CBA_EDE27CBAB649__INCLUDED_)
