#if !defined(AFX_DLGRECORDDPEQ_H__DD49709D_7D71_4FA9_829F_07EC1AF50A57__INCLUDED_)
#define AFX_DLGRECORDDPEQ_H__DD49709D_7D71_4FA9_829F_07EC1AF50A57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRecordDpEq.h : header file
//
#include "typedef.h"
#include "BuddiedSliderCtrl.h"
#include "RecordEqStc.h"


#define MAX_STEP_SLIDER_V 90
#define MIN_STEP_SLIDER_V 0
#define MAX_STEP_SLIDER_H 22050
#define MIN_STEP_SLIDER_H 0
#define STEP_SLIDER_V 72
/////////////////////////////////////////////////////////////////////////////
// CDlgRecordDpEq dialog

class CDlgRecordDpEq : public CDialog
{
// Construction
public:
	CDlgRecordDpEq(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRecordDpEq)
	enum { IDD = IDD_DIALOG_RECORD_DP_EQ };
		// NOTE: the ClassWizard will add data members here
	CBuddiedSliderCtrl m_SliderBoost1;
	CBuddiedSliderCtrl m_SliderBoost2;
	CBuddiedSliderCtrl m_SliderBoost3;
	CBuddiedSliderCtrl m_SliderBoost4;
	CBuddiedSliderCtrl m_SliderBoost5;
	CBuddiedSliderCtrl m_SliderBoost6;
	float m_fBoost1;
	float m_fBoost2;
	float m_fBoost3;
	float m_fBoost4;
	float m_fBoost5;
	float m_fBoost6;
	CButton m_chkSw1;
	CButton m_chkSw2;
	CButton m_chkSw3;
	CButton m_chkSw4;
	CButton m_chkSw5;
	CButton m_chkSw6;
	CBuddiedSliderCtrl m_SliderFO1;
	CBuddiedSliderCtrl m_SliderFO2;
	CBuddiedSliderCtrl m_SliderFO3;
	CBuddiedSliderCtrl m_SliderFO4;
	CBuddiedSliderCtrl m_SliderFO5;
	CBuddiedSliderCtrl m_SliderFO6;
	int m_nFO1;
	int m_nFO2;
	int m_nFO3;
	int m_nFO4;
	int m_nFO5;
	int m_nFO6;
	int m_nDF1;
	int m_nDF2;
	int m_nDF3;
	int m_nDF4;
	int m_nDF5;
	int m_nDF6;
	float m_fGain1;
	float m_fGain2;
	float m_fGain3;
	float m_fGain4;
	float m_fGain5;
	float m_fGain6;
	float m_fMasterGain;
	int m_nSampleRate;
	CRecordEqStc m_stcGraph;
	int m_nCBIndexStereo;
	int m_nCBIndexSW;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRecordDpEq)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetValue(EQ_STRUCT_G_T& eq_data);
	void FetchValue(EQ_STRUCT_G_T& eq_data);

private:
	void GetValue(EQ_STRUCT_G_T& eq_data);
	void InitValue(EQ_STRUCT_G_T& eq_data);
	void RefreshControls();
	void InitSLider();
	void SetSliderPos();
	void ReflashGraph();



private:
	EQ_STRUCT_G_T m_eq_data;

	float m_fMasterGainBak;
	COLORREF m_crBack;
	BOOL m_bInit;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRecordDpEq)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCheckSw1();
	afx_msg void OnCheckSw2();
	afx_msg void OnCheckSw3();
	afx_msg void OnCheckSw4();
	afx_msg void OnCheckSw5();
	afx_msg void OnCheckSw6();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditBoost1();
	afx_msg void OnChangeEditBoost2();
	afx_msg void OnChangeEditBoost3();
	afx_msg void OnChangeEditBoost4();
	afx_msg void OnChangeEditBoost5();
	afx_msg void OnChangeEditBoost6();
	afx_msg void OnChangeEditFo1();
	afx_msg void OnChangeEditFo2();
	afx_msg void OnChangeEditFo3();
	afx_msg void OnChangeEditFo4();
	afx_msg void OnChangeEditFo5();
	afx_msg void OnChangeEditFo6();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnReleasedcaptureSliderBoost1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderBoost2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderBoost3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderBoost4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderBoost5(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderBoost6(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderFo1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderFo2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderFo3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderFo4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderFo5(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderFo6(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRECORDDPEQ_H__DD49709D_7D71_4FA9_829F_07EC1AF50A57__INCLUDED_)
