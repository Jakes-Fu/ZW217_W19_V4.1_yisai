#if !defined(AFX_DLGSELGROUP_H__8F11943E_7E1B_47CE_AB27_4CB7D12E51B3__INCLUDED_)
#define AFX_DLGSELGROUP_H__8F11943E_7E1B_47CE_AB27_4CB7D12E51B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelGroup.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CDlgSelGroup dialog

class CDlgSelGroup : public CDialog
{
// Construction
public:
	CDlgSelGroup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelGroup)
	enum { IDD = IDD_DLG_SEL_GROUP };
	CComboBox	m_cmbGroup;
	CButton	m_btnCancel;
	CButton	m_btnOK;
	CListCtrl	m_lstGM;
	int		m_nGroup;
	//}}AFX_DATA

   	CUIntArray m_arrSel;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelGroup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitLstCtrl(void);
	void InitCmbCtrl(void);
	// Generated message map functions
	//{{AFX_MSG(CDlgSelGroup)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmbGroup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELGROUP_H__8F11943E_7E1B_47CE_AB27_4CB7D12E51B3__INCLUDED_)
