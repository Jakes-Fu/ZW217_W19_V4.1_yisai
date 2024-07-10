#if !defined(AFX_DLGNXPTX_H__1247B540_9BE0_477D_BE2A_5400E80A67DC__INCLUDED_)
#define AFX_DLGNXPTX_H__1247B540_9BE0_477D_BE2A_5400E80A67DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNxpTx.h : header file
//
#include "typedef.h"
#include "DlgTxNVParam.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgNxpTx dialog

class CDlgNxpTx : public CDialog
{
// Construction
public:
	CDlgNxpTx(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgNxpTx)
	enum { IDD = IDD_DIALOG_NXP_TX };
		// NOTE: the ClassWizard will add data members here
	int m_nCBGeneralOperMode;
	int m_nCBMute;
	int m_nCBVolMode;
	int m_nCBBdMode;
	int m_nCBHpfMode;
	int m_nCBOperMode;
	int m_nCBWhisperOperMode;
	int m_nCBEqOperatingMod;
	int m_nBulkDelay;
	int m_nVolGain;
	int m_nMode;
	int m_nHpfFreq;
	int m_nTuningMode;
	int m_nOutputGain;
	int m_nInputGain;
	int m_nNLMSLimit;
	int m_nNLMSLbTaps;
	int m_nNLMSLbAlpha;
	int m_nPcdThreshold;
	int m_nPcdErl;
	int m_nPcdErlStep;
	int m_nPcdTaps;
	int m_nPcdMinimumErl;
	int m_nPcdGammaERescue;
	int m_nWhisperMode;
	int m_nNLMSLbErl;
	int m_nTargetLevelLi;
	int m_nAvlMaxgainlin;
	int m_nAvlMingainlin;
	int m_nAvlAttack;
	int m_nAvlLimitMaxoutput;
	int m_nAvlRelease;
	int m_nSpdetectThreshold;
	int m_nNLMSHbTaps;
	int m_nEqLength;
	int m_nNLMSHbAlpha;
	int m_nNLMSHbErl;
	int m_nNLMSPresetCoefs;
	int m_nNLMSOffset;
	int m_nSpdetNear;
	int m_nSpdetAct;
	int m_nNfeBlocks;
	int m_nGammaAlpha;
	int m_nGammaN;
	int m_nLimitNs;
	int m_nCniGain;
	int m_nNlAtten;
	int m_nGammaLow;
	int m_nGammaDt;
	int m_nGammaHigh;
	int m_nTailPortion;
	int m_nTailAlpha;
	int m_nSpdetFar;
	int m_nSpdetMic;
	int m_nSpdetXClip;
	int m_nCBDrcOperatingMode;
	int m_nDrcNumknees;
	int m_nDrcAttackTime;
	int m_nDrcReleaseTime;
	int m_nCBDrcLimiterOperatingMode;
	int m_nDrcLimitLevel;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNxpTx)
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
	//{{AFX_MSG(CDlgNxpTx)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonTxPeqCoefs();
	afx_msg void OnButtonTxNvParam();
	afx_msg void OnButtonTxDrcCompressorInputLevel();
	afx_msg void OnButtonTxDrcCompressorOutputLevel();
	afx_msg UINT OnNcHitTest(CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNXPTX_H__1247B540_9BE0_477D_BE2A_5400E80A67DC__INCLUDED_)
