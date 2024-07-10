#if !defined(AFX_DLGTXNVPARAM_H__C9C9A541_A6F7_4E2F_8957_D8B0FD9ED2A0__INCLUDED_)
#define AFX_DLGTXNVPARAM_H__C9C9A541_A6F7_4E2F_8957_D8B0FD9ED2A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTxNVParam.h : header file
//
#include "typedef.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTxNVParam dialog

class CDlgTxNVParam : public CDialog
{
// Construction
public:
	CDlgTxNVParam(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTxNVParam)
	enum { IDD = IDD_DIALOG_NXP_NV_CONTROL_PARAM };
		// NOTE: the ClassWizard will add data members here
	int m_nCBOperatingMode;
	int m_nMode;
	int m_nMode2;
	int m_nTuningMode;
	int m_nInputGainMic0;
	int m_nInputGainMic1;
	int m_nOutputGain;
	int m_nNLMS0LbTaps;
	int m_nNLMS0LbTwoalpha;
	int m_nNLMS0LbErl;
	int m_nNLMS0HbTaps;
	int m_nNLMS0HbTwoalpha;
	int m_nNLMS0HbErl;
	int m_nNLMS0PresetCoefs;
	int m_nNLMS0Offset;
	int m_nNLMS1LbTaps;
	int m_nNLMS1LbTwoalpha;
	int m_nNLMS1LbErl;
	int m_nNLMS1HbTaps;
	int m_nNLMS1HbTwoalpha;
	int m_nNLMS1HbErl;
	int m_nNLMS1PresetCoefs;
	int m_nNLMS1Offset;
	int m_nCalMicpowFloorMin;
	int m_nWgThreshold;
	int m_nMpThreshold;
	int m_nFsbTaps;
	int m_nFsbTwoalpha;
	int m_nFsbRefGain;
	int m_nGscTaps;
	int m_nGscTwoalpha;
	int m_nGscErl;
	int m_nGscOffset;
	int m_nDnnsEchoGammaHi;
	int m_nDnnsEchoGammaLo;
	int m_nDnnsEchoAlphaRev;
	int m_nDnnsEchoTailPortion;
	int m_nDnnsNlAtten;
	int m_nDnnsNoiseGammaS;
	int m_nDnnsNoiseGammaN;
	int m_nDnnsNoiseGainMinS;
	int m_nDnnsNoiseGainMinN;
	int m_nDnnsNoiseBiasComp;
	int m_nDnnsGainEta;
	int m_nDnnsAcThreshold;
	int m_nDnnsWbThreshold;
	int m_nDnnsLostBeamThreshold;
	int m_nPcdBeta;
	int m_nPcdThreshold;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTxNVParam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

public:
	void SetValue(LVNV_ControlParams_st NvControlParams);
	void FetchValue(LVNV_ControlParams_st& NvControlParams);
	
private:
	void InitValue(LVNV_ControlParams_st NvControlParams);
	void GetValue(LVNV_ControlParams_st& NvControlParams);

private:
	LVNV_ControlParams_st m_NvControlParams;


	// Generated message map functions
	//{{AFX_MSG(CDlgTxNVParam)
	afx_msg void OnButtonTxNvFsbInitTable0();
	afx_msg void OnButtonTxNvFsbInitTable1();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTXNVPARAM_H__C9C9A541_A6F7_4E2F_8957_D8B0FD9ED2A0__INCLUDED_)
