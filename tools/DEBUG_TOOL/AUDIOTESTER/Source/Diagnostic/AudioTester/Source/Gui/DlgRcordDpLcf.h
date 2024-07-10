#if !defined(AFX_DLGRCORDDPLCF_H__7886D3EB_1C25_42DA_87A1_632783B1D9A0__INCLUDED_)
#define AFX_DLGRCORDDPLCF_H__7886D3EB_1C25_42DA_87A1_632783B1D9A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRcordDpLcf.h : header file
//
#include "typedef.h"
#include "DPLcfStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgRcordDpLcf dialog

class CDlgRcordDpLcf : public CDialog
{
// Construction
public:
	CDlgRcordDpLcf(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRcordDpLcf)
	enum { IDD = IDD_DIALOG_RECORD_DP_LCF };
		// NOTE: the ClassWizard will add data members here
	int m_nCBDPLcfSw;
	int m_nDPLcfFpL;
	int m_nDPLcfFpR;
	int m_nDPLcfGainL;
	int m_nDPLcfGainR;
	CDPLcfStatic m_stcGraph;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRcordDpLcf)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetValue(EQ_STRUCT_G_T& eq_data);
	void FetchValue(EQ_STRUCT_G_T& eq_data);
		
private:
	void InitValue(EQ_STRUCT_G_T& eq_data);
	void GetValue(EQ_STRUCT_G_T& eq_data);
	void ReflashGraph();
		
private:
	EQ_STRUCT_G_T m_eq_data;
	int m_nDPLcfGainLBak;
	int m_nDPLcfGainRBak;
	BOOL m_bInit;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRcordDpLcf)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeEditDpLcfFpL();
	afx_msg void OnChangeEditDpLcfFpR();
	afx_msg void OnChangeEditDpLcfGainL();
	afx_msg void OnChangeEditDpLcfGainR();
	afx_msg void OnSelchangeComboDpLcfSw();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRCORDDPLCF_H__7886D3EB_1C25_42DA_87A1_632783B1D9A0__INCLUDED_)
