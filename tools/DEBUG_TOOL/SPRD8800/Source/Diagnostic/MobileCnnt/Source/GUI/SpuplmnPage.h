#if !defined(AFX_SPUPLMNPAGE_H__1BC70E55_E326_4A7A_86F2_888104F26FE3__INCLUDED_)
#define AFX_SPUPLMNPAGE_H__1BC70E55_E326_4A7A_86F2_888104F26FE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpuplmnPage.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CSpuplmnPage dialog

class CSpuplmnPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSpuplmnPage)

// Construction
public:
	CSpuplmnPage();
	~CSpuplmnPage();

// Dialog Data
	//{{AFX_DATA(CSpuplmnPage)
	enum { IDD = IDD_PAGE_SPUPLMN };
	CListCtrl m_lstUplmn;
	CComboBox m_boxIndex;
	CComboBox m_boxType;
	CString   m_strPlmn;
	CButton   m_btnRead;
	CButton   m_btnSet;
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA
public:
	void ShowRespondContent(LPCSTR szRet);
	void SendCheckRequest();
	void SendSetRequest();
	void ClearAll();

protected:
	BOOL m_bInit;
	
	CBrush  m_brshDlgBckGrnd;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSpuplmnPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSpuplmnPage)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnReadUplmn();
	afx_msg void OnBtnSetUplmn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPUPLMNPAGE_H__1BC70E55_E326_4A7A_86F2_888104F26FE3__INCLUDED_)
