#if !defined(AFX_DLGGENQUHAO_H__8D66DCB1_46A9_4FEB_B672_5E5FE2E89458__INCLUDED_)
#define AFX_DLGGENQUHAO_H__8D66DCB1_46A9_4FEB_B672_5E5FE2E89458__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGenQuhao.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGenQuhao dialog

class CDlgGenQuhao : public CDialog
{
// Construction
public:
	CDlgGenQuhao(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGenQuhao)
	enum { IDD = IDD_DLG_QUHAO };
	CString	m_strCityQuhaoFile;
	CString	m_strQuhaoFile;
	CString	m_strQuhaoCityFile;
	CString	m_strCityOutFile;
	CString	m_strCityInFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGenQuhao)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGenQuhao)
	afx_msg void OnBtnCqBrow();
	afx_msg void OnBtnQcBrow();
	afx_msg void OnBtnQBrow();
	afx_msg void OnBtnCBrow();
	virtual void OnOK();
	afx_msg void OnBtnCOutBrow();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGENQUHAO_H__8D66DCB1_46A9_4FEB_B672_5E5FE2E89458__INCLUDED_)
