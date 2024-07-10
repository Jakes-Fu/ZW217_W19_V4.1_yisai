#if !defined(AFX_EXTEND2DLG_H__BEB20B42_AC35_432A_9501_E227E8235137__INCLUDED_)
#define AFX_EXTEND2DLG_H__BEB20B42_AC35_432A_9501_E227E8235137__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Extend2Dlg.h : header file
//
#include "BMPCtrlWithSpecRect.h"

/////////////////////////////////////////////////////////////////////////////
// CExtend2Dlg dialog

class CExtend2Dlg : public CDialog
{
// Construction
public:
	CExtend2Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExtend2Dlg)
	enum { IDD = IDD_DIALOG_MORE_PARAMETER2 };
	short	m_dl_no_vad_cnt_thd;
    short m_dl_min_psne;
    short m_dl_max_temp_uamn;
    short m_dl_vad_thd;
    short m_dl_active_thd;
    short m_dl_noise_thd;
    short m_dl_max_psne;
    short m_dl_voise_burst;
    short m_dl_noise_tail;
    short m_dl_rfilter_delay;
    short m_dl_rfilter_tail;
    short m_dl_rfilter;
    short m_dl_dgain;
    short m_dl_sim_M;
    short m_dl_sim_fac;
    short m_dl_dac_limt;
    short m_dl_ns_factor;
    short m_dl_ns_limit;
    short m_dl_dis_snr_thd;
    short m_dl_dis_band_1k;
    short m_dl_dis_limit;
    short m_dl_drc_thd;
    short m_dl_drc_ratio;
    short m_dl_drc_dstep;
    short m_dl_drc_ustep;
    short m_dl_drc_cnt;
	short	m_dl_ns_up_factor;

	short	m_dl_clarity_switch;

	short m_dl_vad_switch;
    short m_dl_complex_switch;
    short m_dl_noise_est_switch;
    short m_dl_nr_switch;
    
    short m_dl_dis_switch;
    short m_dl_amb_noise_est_switch;
    short m_dl_amb_eq_switch;
    short m_dl_acoustic_eq_switch;
    
    short m_dl_drc_switch;
    short m_dl_dac_limit_switch;
    short m_dl_hpf_set;
    short m_dl_lpf_set;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtend2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
    SpecialRectOfDlSettings* m_pArea;
    
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExtend2Dlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTEND2DLG_H__BEB20B42_AC35_432A_9501_E227E8235137__INCLUDED_)
