#if !defined(AFX_GOTOITEMDLG_H__F8E80BCF_3566_402C_8466_F95A460DDF0C__INCLUDED_)
#define AFX_GOTOITEMDLG_H__F8E80BCF_3566_402C_8466_F95A460DDF0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GotoItemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGotoItemDlg dialog

class CGotoItemDlg : public CDialog
{
// Construction
public:
	CGotoItemDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGotoItemDlg)
	enum { IDD = IDD_DLG_GOTO_ITEM };
	UINT	m_nIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGotoItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGotoItemDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GOTOITEMDLG_H__F8E80BCF_3566_402C_8466_F95A460DDF0C__INCLUDED_)
