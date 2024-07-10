#if !defined(AFX_DLGSENDPRG_H__B232E89C_E6A5_461C_8812_3474CD358516__INCLUDED_)
#define AFX_DLGSENDPRG_H__B232E89C_E6A5_461C_8812_3474CD358516__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSendPrg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSendPrg dialog

class CDlgSendPrg : public CDialog
{
// Construction
public:
	CDlgSendPrg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSendPrg)
	enum { IDD = IDD_DLG_SEND_SMS_PROC };
	CProgressCtrl	m_prgSend;
	CString	m_strSendItem;
	CString	m_strTo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSendPrg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSendPrg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSENDPRG_H__B232E89C_E6A5_461C_8812_3474CD358516__INCLUDED_)
