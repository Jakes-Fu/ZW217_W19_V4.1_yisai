#if !defined(AFX_DLGNXPRX_H__0A80D03A_A99D_41AE_9794_B05BB1BB503B__INCLUDED_)
#define AFX_DLGNXPRX_H__0A80D03A_A99D_41AE_9794_B05BB1BB503B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNxpRx.h : header file
//
#include "typedef.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgNxpRx dialog

class CDlgNxpRx : public CDialog
{
// Construction
public:
	CDlgNxpRx(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgNxpRx)
	enum { IDD = IDD_DIALOG_NXP_RX };
		// NOTE: the ClassWizard will add data members here
	int m_nCBMute;
	int m_nCBVolOperatingMode;
	int m_nCBOperatingMode;
	int m_nCBNlppOperatingMode;
	int m_nCBVoiceOperatingMode;
	int m_nCBEqOperatingMode;
	int m_nCBFensOperatingMode;
	int m_nVolGain;
	int m_nFensLimitNs;
	int m_nMode;
	int m_nNlppLimit;
	int m_nNlppHpfCornerfreq;
//	int m_nNoiseSensitivity;
	int m_nAvlRelease;
	int m_nAvlMaxGainLin;
	int m_nSpdetectThreshold;
	int m_nAvlTargetLevelLin;
	int m_nVoiceMode;
	int m_nAvlReleaseMax;
	int m_nAvlNominalGain;
	int m_nAvlLimitMaxoutput;
	int m_nAvlMinGainLin;
	int m_nGainLinMax;
	int m_nAvlAttack;
	int m_nEqLength;
	int m_nNoiseSensitivity;
	int m_nCBDrcOperatingMode;
	int m_nDrcNumknees;
	int m_nDrcAttackTime;
	int m_nDrcReleaseTime;
	int m_nCBDrcLimiterOperatingMode;
	int m_nDrcLimitLevel;
	int m_nCBNgOperatingMode;
	int m_nNgNumknees;
	int m_nNgAttackTime;
	int m_nNgReleaseTime;
	int m_nCBNgLimiterOperatingMode;
	int m_nNgLimitLevel;
	int m_nCBHPFOperatingMode;
	int m_nHPFCornerFreq;
	int m_nCBCNGOperatingMode;
	int m_nCNGVolume;
	int m_nCBWMOperatingMode;
	int m_nWMMode;
	int m_nWMAVLMinGainLin;
	int m_nWMAVLMaxGainLin;
	int m_nWMAVLTargetLevelLin;
	int m_nWMAVLAttack;
	int m_nWMAVLRelease;
	int m_nWMAVLLimitMaxOutPut;
	int m_nWMSpDetectThreshold;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNxpRx)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	void SetValue(LVVE_CTRL_PARAM_NV_T lvve_data);
	void FetchValue(LVVE_CTRL_PARAM_NV_T& lvve_data);
	void SetLVVEType(DWORD dwLVVEType){m_dwLVVEType = dwLVVEType;}
		
private:
	void InitValue(LVVE_CTRL_PARAM_NV_T lvve_data);
	void GetValue(LVVE_CTRL_PARAM_NV_T& lvve_data);
	void EnableControl();

private:
	LVVE_CTRL_PARAM_NV_T m_lvve_data;
	DWORD m_dwLVVEType;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNxpRx)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonRxPeqCoefs();
	afx_msg void OnButtonNoiseSensitivity();
	afx_msg void OnButtonRxDrcCompressorInputLevel();
	afx_msg void OnButtonRxDrcCompressorOutputLevel();
	afx_msg void OnButtonRxNgCompressorInputLevel();
	afx_msg void OnButtonRxNgCompressorOutputLevel();
	afx_msg UINT OnNcHitTest(CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNXPRX_H__0A80D03A_A99D_41AE_9794_B05BB1BB503B__INCLUDED_)
