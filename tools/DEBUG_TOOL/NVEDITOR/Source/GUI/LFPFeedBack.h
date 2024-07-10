#if !defined(AFX_LFPFEEDBACK_H__8F446B19_C3C6_4A33_8E9E_FA29AC03DCA7__INCLUDED_)
#define AFX_LFPFEEDBACK_H__8F446B19_C3C6_4A33_8E9E_FA29AC03DCA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LFPFeedBack.h : header file
//

#define WM_UPDTAE_STATE (WM_USER + 2011)

/////////////////////////////////////////////////////////////////////////////
// CLFPFeedBack dialog

class CLFPFeedBack : public CDialog
{
// Construction
public:
	CLFPFeedBack(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLFPFeedBack)
	enum { IDD = IDD_LFP_FEEDBACK };
	CListCtrl	m_listLog;
	CButton	m_ctlCancel;
	CProgressCtrl	m_ctlProgress;
	CStatic	m_ctlInfo;
	//}}AFX_DATA
    CString m_strInfo;
    int     m_nRange;

    int m_nTick;
	
	BOOL   m_bInit;

	BOOL   m_bRead;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLFPFeedBack)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLFPFeedBack)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnMessage(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
    void UpdateFeedBack( const CString &strInfo, int nPos, BOOL bDisableCancel = FALSE, LPCTSTR lpszLog = NULL );
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LFPFEEDBACK_H__8F446B19_C3C6_4A33_8E9E_FA29AC03DCA7__INCLUDED_)
