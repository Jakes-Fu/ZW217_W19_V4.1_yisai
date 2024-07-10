#if !defined(AFX_DLGEQMODE3_H__AEEABCF7_493C_446A_9A05_DB31EDD36356__INCLUDED_)
#define AFX_DLGEQMODE3_H__AEEABCF7_493C_446A_9A05_DB31EDD36356__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEQMode3.h : header file
//
#include "typedef.h"
#include "EqModeStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgEQMode3 dialog

class CDlgEQMode3 : public CDialog
{
// Construction
public:
	CDlgEQMode3(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgEQMode3)
	enum { IDD = IDD_DIALOG_EQ_MODE_3 };
	CEqModeStatic	m_stcF1f1Treble;
	CEqModeStatic	m_stcF1f1Lcf;
	CEqModeStatic	m_stcF1f1Bass;
	CEqModeStatic	m_stcF1f1All;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEQMode3)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	void SetPara(EQ_MODE_NV_PARAM_T EQMode,short eqControl);
	BOOL GetPara(EQ_MODE_NV_PARAM_T &EQMode);

	void ParentShow();

// Implementation
public:
	// Generated message map functions
	//{{AFX_MSG(CDlgEQMode3)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonF1f1Refresh();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
protected:
	float m_ff1g0;
	float m_ff1g1;
	int m_nf1fp;
	float m_ff2g0;
	float m_ff2g1;
	int m_nf2fp;
	int m_nBassf0;
	float m_fBassq;
	float m_fBassboost;
	float m_fBassgain;
	int m_nTreblef0;
	float m_fTrebleq;
	float m_fTrebleboost;
	float m_fTreblegain;

	BOOL m_bInit;
	BOOL m_bFirstDraw;
	
	float m_fMinq;
	float m_fMaxq;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEQMODE3_H__AEEABCF7_493C_446A_9A05_DB31EDD36356__INCLUDED_)
