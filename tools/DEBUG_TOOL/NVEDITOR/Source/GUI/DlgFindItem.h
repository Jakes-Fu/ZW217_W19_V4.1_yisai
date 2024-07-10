#if !defined(AFX_DLGFINDITEM_H__88F92539_D8C5_44A6_B070_2CD71A5A867E__INCLUDED_)
#define AFX_DLGFINDITEM_H__88F92539_D8C5_44A6_B070_2CD71A5A867E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFindItem.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CDlgFindItem dialog

class CDlgFindItem : public CDialog
{
// Construction
public:
	CDlgFindItem(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFindItem)
	enum { IDD = IDD_DIALOG_FIND };
	BOOL	m_bFullMatch;
	BOOL	m_bItem;
	BOOL	m_bValue;
	CString	m_strTarget;
	BOOL	m_bChanged;
	//}}AFX_DATA

	DWORD  m_dwFlag;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFindItem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFindItem)
	afx_msg void OnFind();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFINDITEM_H__88F92539_D8C5_44A6_B070_2CD71A5A867E__INCLUDED_)
