#if !defined(AFX_DLGLOADMIDS_H__79966C19_78E2_4E95_AD66_887200D6EE1E__INCLUDED_)
#define AFX_DLGLOADMIDS_H__79966C19_78E2_4E95_AD66_887200D6EE1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLoadMids.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLoadMids dialog

class CDlgLoadMids : public CDialog
{
// Construction
public:
	CDlgLoadMids(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgLoadMids)
	enum { IDD = IDD_DLG_LOAD_MID };
	CListCtrl	m_lstOwnershipFiles;
	CString	m_strCityFile;
	//}}AFX_DATA

	CStringArray m_agOwnershipFiles;
	
	BOOL  m_bSelCityFile;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLoadMids)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLoadMids)
	afx_msg void OnSelCity();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkDssLstDstFiles(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditDssLstDstFiles(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOADMIDS_H__79966C19_78E2_4E95_AD66_887200D6EE1E__INCLUDED_)
