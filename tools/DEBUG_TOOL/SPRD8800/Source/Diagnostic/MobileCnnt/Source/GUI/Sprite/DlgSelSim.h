#if !defined(AFX_DLGSELSIM_H__25E7549F_5D45_4247_86DB_FB2E10D31B45__INCLUDED_)
#define AFX_DLGSELSIM_H__25E7549F_5D45_4247_86DB_FB2E10D31B45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelSim.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSelSim dialog

class CDlgSelSim : public CDialog
{
// Construction
public:
	CDlgSelSim(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelSim)
	enum { IDD = IDD_DLG_SEL_SIM };
	CButton	m_btnOK;
	CButton	m_btnCancel;
	int		m_nSim;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelSim)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelSim)
	afx_msg void OnSelsimBtnOk();
	afx_msg void OnSelsimBtnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELSIM_H__25E7549F_5D45_4247_86DB_FB2E10D31B45__INCLUDED_)
