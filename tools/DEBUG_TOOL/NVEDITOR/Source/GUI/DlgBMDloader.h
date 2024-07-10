#if !defined(AFX_DLGBMDLOADER_H__ACB54A52_6A2E_4AD2_BEBC_A20A0B655E73__INCLUDED_)
#define AFX_DLGBMDLOADER_H__ACB54A52_6A2E_4AD2_BEBC_A20A0B655E73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBMDloader.h : header file
//

#define BM_PASS_MSG   2011
#define BM_FAIL_MSG   2012
#define BM_STOP_MSG   2013

/////////////////////////////////////////////////////////////////////////////
// CDlgBMDloader dialog

class CDlgBMDloader : public CDialog
{
// Construction
public:
	CDlgBMDloader(CWnd* pParent = NULL);   // standard constructor

	void SetStep(LPCTSTR step);
	void SetState(LPCTSTR state);
	void SetProg(LPCTSTR prog);
	
	int  m_nState;

// Dialog Data
	//{{AFX_DATA(CDlgBMDloader)
	enum { IDD = IDD_DIALOG_BMDOWNLOAD };
	CProgressCtrl	m_prgrsCtrl;
	CString	m_strState;
	CString	m_strStep;
	CString	m_strProg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBMDloader)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBMDloader)
	virtual BOOL OnInitDialog();
	afx_msg void OnStop();
	//}}AFX_MSG
	afx_msg void OnPass();
	afx_msg void OnFail();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBMDLOADER_H__ACB54A52_6A2E_4AD2_BEBC_A20A0B655E73__INCLUDED_)
