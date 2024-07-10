#if !defined(AFX_CLOSEWAITDLG_H__93AA5B0A_A81B_41BF_9B7B_03D1AEBF3007__INCLUDED_)
#define AFX_CLOSEWAITDLG_H__93AA5B0A_A81B_41BF_9B7B_03D1AEBF3007__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CloseWaitDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCloseWaitDlg dialog

class CCloseWaitDlg : public CDialog
{
// Construction
public:
	CCloseWaitDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCloseWaitDlg)
	enum { IDD = IDD_CLOSEWAIT };
	CStatic	m_staticTime;
	CStatic	m_staticNotes;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCloseWaitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    UINT m_nWait;

	// Generated message map functions
	//{{AFX_MSG(CCloseWaitDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLOSEWAITDLG_H__93AA5B0A_A81B_41BF_9B7B_03D1AEBF3007__INCLUDED_)
