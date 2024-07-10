#if !defined(AFX_DLGEDITCITY_H__5C5014E6_8232_45EA_9390_180BF68660D8__INCLUDED_)
#define AFX_DLGEDITCITY_H__5C5014E6_8232_45EA_9390_180BF68660D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEditCity.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgEditCity dialog

class CDlgEditCity : public CDialog
{
// Construction
public:
	CDlgEditCity(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgEditCity)
	enum { IDD = IDD_DLG_CITY_EDIT };
	CListCtrl	m_lstCity;
	//}}AFX_DATA

	CString m_strCityFile;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEditCity)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEditCity)
	afx_msg void OnImport();
	virtual BOOL OnInitDialog();
	afx_msg void OnImportText();
	afx_msg void OnSaveAs();
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL LoadCity();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEDITCITY_H__5C5014E6_8232_45EA_9390_180BF68660D8__INCLUDED_)
