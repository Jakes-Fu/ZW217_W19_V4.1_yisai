#if !defined(AFX_ALERTMSGDLG_H__04A4A45C_4EC6_49BB_BFBE_22E54DE6F5E5__INCLUDED_)
#define AFX_ALERTMSGDLG_H__04A4A45C_4EC6_49BB_BFBE_22E54DE6F5E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlertMsgDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlertMsgDlg dialog

class CAlertMsgDlg : public CDialog
{
// Construction
public:
	CAlertMsgDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlertMsgDlg)
	enum { IDD = IDD_DIALOG_ALERT_MSG };
	CString	m_strAlertMsg;
	BOOL	m_bNotNeedNextTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlertMsgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlertMsgDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void ChangeFont();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALERTMSGDLG_H__04A4A45C_4EC6_49BB_BFBE_22E54DE6F5E5__INCLUDED_)
