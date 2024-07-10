#if !defined(AFX_TOOLTIPDLG1_H__87021A26_BDEB_4CC4_8D0F_6EBACCDCAE9C__INCLUDED_)
#define AFX_TOOLTIPDLG1_H__87021A26_BDEB_4CC4_8D0F_6EBACCDCAE9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolTipDlg1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolTipDlg dialog

class CToolTipDlg : public CDialog
{
// Construction
public:
	CToolTipDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CToolTipDlg)
	enum { IDD = IDD_DLG_TOOLTIP };
	CButton	m_btnOK;
	CString	m_strToolTip;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolTipDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolTipDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLTIPDLG1_H__87021A26_BDEB_4CC4_8D0F_6EBACCDCAE9C__INCLUDED_)
