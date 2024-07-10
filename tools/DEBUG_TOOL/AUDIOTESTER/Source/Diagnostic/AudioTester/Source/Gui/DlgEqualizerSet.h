#if !defined(AFX_DLGEQUALIZERSET_H__4E562A10_451B_4474_B2DF_BAC0AB6B0B98__INCLUDED_)
#define AFX_DLGEQUALIZERSET_H__4E562A10_451B_4474_B2DF_BAC0AB6B0B98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEqualizerSet.h : header file
//
#include "typedef.h"
#include "AudioTesterSheet.h"
#include "BuddiedSliderCtrl.h"
#include "EqSetStatic.h"

static CString strPresets[] = 
{
	_T("Regular"),
	_T("Classic"),
	_T("Odeum"),
	_T("Jazz"),
	_T("Rock"),
	_T("Softrock")
};

#define MAX_STEP_SLIDER_V 90
#define MIN_STEP_SLIDER_V 0
#define MAX_STEP_SLIDER_H 22050
#define MIN_STEP_SLIDER_H 0
#define STEP_SLIDER_V 72
/////////////////////////////////////////////////////////////////////////////
// CDlgEqualizerSet dialog

class CDlgEqualizerSet : public CDialog
{
// Construction
public:
	CDlgEqualizerSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgEqualizerSet)
	enum { IDD = IDD_DIALOG_EQUALIZER_SET };
		// NOTE: the ClassWizard will add data members here
	int m_nCBEqType;
	int m_nCBBandNum;
	int m_nCBLowShelve;
	int m_nCBHighShelve;
	int m_nCBSmooth;
	int m_nLevelN;
	CListBox m_lstPresets;
	int m_nLevelStep;
	int m_nSampleRate;
	float m_fLeft;
	float m_fRight;
	float m_fRight1;
	float m_fRight2;
	float m_fRight3;
	float m_fRight4;
	float m_fRight5;
	float m_fRight6;
	float m_fRight7;
	float m_fRight8;
	float m_fRight9;
	float m_fRight10;
	float m_fRight11;
	float m_fRight12;
	float m_fRight13;
	float m_fRight14;
	float m_fRight15;
	float m_fRight16;
	float m_fLowShelfCutoff;
	float m_fHighShelfCutoff;
	float m_fCentralFrequency1;
	float m_fCentralFrequency2;
	float m_fCentralFrequency3;
	float m_fCentralFrequency4;
	float m_fCentralFrequency5;
	float m_fCentralFrequency6;
	float m_fCentralFrequency7;
	float m_fCentralFrequency8;
	float m_fWidth1;
	float m_fWidth2;
	float m_fWidth3;
	float m_fWidth4;
	float m_fWidth5;
	float m_fWidth6;
	float m_fWidth7;
	float m_fWidth8;
	float m_fGain1;
	float m_fGain2;
	float m_fGain3;
	float m_fGain4;
	float m_fGain5;
	float m_fGain6;
	float m_fGain7;
	float m_fGain8;
	float m_fMasterGain;
	CEqSetStatic m_stcGraph;
	CBuddiedSliderCtrl m_sctrlLeft;
	CBuddiedSliderCtrl m_sctrlRight;
	CBuddiedSliderCtrl m_sctrlRight1;
	CBuddiedSliderCtrl m_sctrlRight2;
	CBuddiedSliderCtrl m_sctrlRight3;
	CBuddiedSliderCtrl m_sctrlRight4;
	CBuddiedSliderCtrl m_sctrlRight5;
	CBuddiedSliderCtrl m_sctrlRight6;
	CBuddiedSliderCtrl m_sctrlRight7;
	CBuddiedSliderCtrl m_sctrlRight8;
	CBuddiedSliderCtrl m_sctrlRight9;
	CBuddiedSliderCtrl m_sctrlRight10;
	CBuddiedSliderCtrl m_sctrlRight11;
	CBuddiedSliderCtrl m_sctrlRight12;
	CBuddiedSliderCtrl m_sctrlRight13;
	CBuddiedSliderCtrl m_sctrlRight14;
	CBuddiedSliderCtrl m_sctrlRight15;
	CBuddiedSliderCtrl m_sctrlRight16;
	CBuddiedSliderCtrl m_sctrlLowShelfCutoff;
	CBuddiedSliderCtrl m_sctrlHighShelfCutoff;
	CBuddiedSliderCtrl m_sctrlCentralFrequency1;
	CBuddiedSliderCtrl m_sctrlCentralFrequency2;
	CBuddiedSliderCtrl m_sctrlCentralFrequency3;
	CBuddiedSliderCtrl m_sctrlCentralFrequency4;
	CBuddiedSliderCtrl m_sctrlCentralFrequency5;
	CBuddiedSliderCtrl m_sctrlCentralFrequency6;
	CBuddiedSliderCtrl m_sctrlCentralFrequency7;
	CBuddiedSliderCtrl m_sctrlCentralFrequency8;
	CButton m_rdConstantWidth;
	CButton m_rdConstantQ;
	CButton m_chk1;
	CButton m_chk2;
	CButton m_chk3;
	CButton m_chk4;
	CButton m_chk5;
	CButton m_chk6;
	CButton m_chk7;
	CButton m_chk8;
	//}}AFX_DATA

public:
	void SetValue(VOLUME_STRUCT& volume_data);
	void FetchValue(VOLUME_STRUCT& volume_data);

	void SetEQValue(EQ_STRUCT_G_T& eq_data, TUN_STRUCT_G_T& tun_data);
	void FetchEQValue(EQ_STRUCT_G_T& eq_data, TUN_STRUCT_G_T& tun_data);

	void SetIndex(int nEQIndex, int nTunIndex){m_nEQIndex = nEQIndex; m_nTunIndex = nTunIndex;}

	void SetSampleRate(int nSamplerate){m_nSampleRate = nSamplerate;}
	void SetSerialPara(BOOL bSerialPara){m_bSerialPara = bSerialPara;}
	void FetchPresetsIndex(int &nIndex){nIndex = m_nPrestesIndex;}

private:
	void GetEQValue();
	void GetValue(VOLUME_STRUCT& volume_data);
	void InitValue(VOLUME_STRUCT& volume_data);
	void InitEQValue(EQ_STRUCT_G_T& eq_data, TUN_STRUCT_G_T& tun_data);
	void ChangeValue();
	void ChangeState();
	void ChangeSctrlState();
	void ChangeEQValue(int nIndex);
	void InitSLider();
	void SetSliderPos();
	void ReflashGraph();

private:
	void InitToolbar();

public:
	CAudioTesterSheet *m_pSheet;

private:
	CToolBar m_toolBarEQ;

	EQ_STRUCT_G_T m_eq_data;
	TUN_STRUCT_G_T m_tun_data;
	VOLUME_STRUCT m_VolumeData;

	int m_nEQIndex;
	int m_nTunIndex;

	int m_nOldIndex;
	int m_nNewIndex;

	float m_fMasterGainBak;

	BOOL m_bInit;
	COLORREF m_crBack;
	BOOL m_bSetTunBuddy;
	int m_nOldLevelStep;
	BOOL m_bSerialPara;
	int m_nPrestesIndex;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEqualizerSet)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEqualizerSet)
	virtual BOOL OnInitDialog();
 	afx_msg void OnSelchangeListPresets();
 	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnOK();
	afx_msg void OnButtonEqGLoadCurrentModeFromFlash();
	afx_msg void OnButtonEqGLoadCurrentModeFromPhone();
	afx_msg void OnButtonEqGWriteCurrentModeToFlash();
	afx_msg void OnButtonEqGWriteCurrentModeToPhone();
	afx_msg void OnSelchangeComboEqType();
	afx_msg void OnSelchangeComboBandNum();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
	afx_msg void OnCheck4();
	afx_msg void OnCheck5();
	afx_msg void OnCheck6();
	afx_msg void OnCheck7();
	afx_msg void OnCheck8();
	afx_msg void OnBtnEqReflash();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnReleasedcaptureSliderRight1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderRight(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderRight2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderRight3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderRight4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderRight5(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderRight6(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderRight7(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderRight8(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderLowShelfCutoff(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderLeft(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderHighShelfCutoff(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderCentralFrequency8(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderCentralFrequency7(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderCentralFrequency6(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderCentralFrequency5(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderCentralFrequency4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderCentralFrequency3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderCentralFrequency2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderCentralFrequency1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditRight1();
	afx_msg void OnChangeEditRight();
	afx_msg void OnChangeEditRight2();
	afx_msg void OnChangeEditRight3();
	afx_msg void OnChangeEditRight4();
	afx_msg void OnChangeEditRight5();
	afx_msg void OnChangeEditRight6();
	afx_msg void OnChangeEditRight7();
	afx_msg void OnChangeEditRight8();
	afx_msg void OnChangeEditLowShelfCotoff();
	afx_msg void OnChangeEditLeft();
	afx_msg void OnChangeEditHighShelfCotoff();
	afx_msg void OnChangeEditCentralFrequency1();
	afx_msg void OnChangeEditCentralFrequency2();
	afx_msg void OnChangeEditCentralFrequency3();
	afx_msg void OnChangeEditCentralFrequency4();
	afx_msg void OnChangeEditCentralFrequency5();
	afx_msg void OnChangeEditCentralFrequency6();
	afx_msg void OnChangeEditCentralFrequency7();
	afx_msg void OnChangeEditCentralFrequency8();
	afx_msg void OnChangeEditGain1();
	afx_msg void OnChangeEditGain2();
	afx_msg void OnChangeEditGain3();
	afx_msg void OnChangeEditGain4();
	afx_msg void OnChangeEditGain5();
	afx_msg void OnChangeEditGain6();
	afx_msg void OnChangeEditGain7();
	afx_msg void OnChangeEditGain8();
	afx_msg void OnChangeEditWidth1();
	afx_msg void OnChangeEditWidth2();
	afx_msg void OnChangeEditWidth3();
	afx_msg void OnChangeEditWidth4();
	afx_msg void OnChangeEditWidth5();
	afx_msg void OnChangeEditWidth6();
	afx_msg void OnChangeEditWidth7();
	afx_msg void OnChangeEditWidth8();
	afx_msg void OnChangeEditMatserGain();
	afx_msg void OnButtonLoadFromDefault();
	afx_msg void OnButtonSaveToDefault();
	afx_msg void OnChangeEditLevelStep();
	afx_msg void OnChangeEditLevelN();
	afx_msg UINT OnNcHitTest(CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEQUALIZERSET_H__4E562A10_451B_4474_B2DF_BAC0AB6B0B98__INCLUDED_)
