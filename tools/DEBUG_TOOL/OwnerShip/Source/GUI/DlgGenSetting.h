#if !defined(AFX_DLGGENSETTING_H__5B514E41_B8C2_4903_B594_385D197A8FD1__INCLUDED_)
#define AFX_DLGGENSETTING_H__5B514E41_B8C2_4903_B594_385D197A8FD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGenSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGenSetting dialog

class CDlgGenSetting : public CDialog
{
// Construction
public:
	CDlgGenSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGenSetting)
	enum { IDD = IDD_DLG_GEN_SETTING };
	CListCtrl	m_lstDstFiles;
	//}}AFX_DATA

	CStringArray m_agDstFiles;
	CUIntArray m_agFlag;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGenSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGenSetting)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkDgsLstOwnershipFiles(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditDgsLstOwnershipFiles(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGENSETTING_H__5B514E41_B8C2_4903_B594_385D197A8FD1__INCLUDED_)
