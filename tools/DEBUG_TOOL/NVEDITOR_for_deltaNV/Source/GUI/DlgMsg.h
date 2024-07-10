#if !defined(AFX_DLGMSG_H__77C24985_5A8D_4AE5_95D8_CBB75E0E6450__INCLUDED_)
#define AFX_DLGMSG_H__77C24985_5A8D_4AE5_95D8_CBB75E0E6450__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMsg.h : header file
//
#include "ResizableDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMsg dialog

class CDlgMsg : public CResizableDialog
{
// Construction
public:
	CDlgMsg(CWnd* pParent = NULL);   // standard constructor
	~CDlgMsg();

	CStringArray m_agItem;
	BOOL bShowCancelBtn;
// Dialog Data
	//{{AFX_DATA(CDlgMsg)
	enum { IDD = IDD_DIALOG_MSG };
	CStatic	m_stcIcon;
	CListBox	m_lst;
	CString	m_strTitle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMsg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void FillList();

	// Generated message map functions
	//{{AFX_MSG(CDlgMsg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMSG_H__77C24985_5A8D_4AE5_95D8_CBB75E0E6450__INCLUDED_)
