#if !defined(AFX_DLGEQMODE2_H__B4E62C1F_10A1_4C06_BAEA_987A43F8E286__INCLUDED_)
#define AFX_DLGEQMODE2_H__B4E62C1F_10A1_4C06_BAEA_987A43F8E286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEQMode2.h : header file
//
#include "typedef.h"
#include "EqModeStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgEQMode2 dialog

class CDlgEQMode2 : public CDialog
{
// Construction
public:
	CDlgEQMode2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgEQMode2)
	enum { IDD = IDD_DIALOG_EQ_MODE_2 };
	CEqModeStatic	m_stcTreble;
	CEqModeStatic	m_stcLcf;
	CEqModeStatic	m_stcBass;
	CEqModeStatic	m_stcAll;
	short	m_nFPL;
	short	m_nFPR;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEQMode2)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


public:
	void SetPara(EQ_MODE_NV_PARAM_T EQMode,BUTTERWORTH_LCF_T lcf_data);
	BOOL GetPara(EQ_MODE_NV_PARAM_T &EQMode,BUTTERWORTH_LCF_T& lcf_data);
	void UpdateGain( double lcf_gain_l,double lcf_gain_r );

	void ParentShow();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgEQMode2)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonButterworthRefresh();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	int m_nBassf0;
	float m_fBassq;
	float m_fBassboost;
	float m_fBassgain;
	int m_nTreblef0;
	float m_fTrebleq;
	float m_fTrebleboost;
	float m_fTreblegain;
	short	m_nLcfGainL;
	short	m_nLcfGainR;

	BOOL m_bInit;
	BOOL m_bFirstDraw;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEQMODE2_H__B4E62C1F_10A1_4C06_BAEA_987A43F8E286__INCLUDED_)
