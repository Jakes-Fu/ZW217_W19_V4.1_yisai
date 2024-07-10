#if !defined(AFX_SYSCONFIGPAGE_H__5A6C41E5_8B98_4F30_972B_1678A1C3ECF5__INCLUDED_)
#define AFX_SYSCONFIGPAGE_H__5A6C41E5_8B98_4F30_972B_1678A1C3ECF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysconfigPage.h : header file
//
typedef enum
{
	SYSCONFIG_MODE,
	SYSCONFIG_ORDER,
	SYSCONFIG_ROAM,
	SYSCONFIG_DOMAIN
};

/////////////////////////////////////////////////////////////////////////////
// CSysconfigPage dialog

class CSysconfigPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSysconfigPage)
// Construction
public:
	CSysconfigPage();
	~CSysconfigPage();

// Dialog Data
	//{{AFX_DATA(CSysconfigPage)
	enum { IDD = IDD_PAGE_SYSCONFIG };
	CListCtrl m_lstRead;
	CComboBox m_boxMode;
	CComboBox m_boxOrder;
	CComboBox m_boxRoam;
	CComboBox m_boxDomain;
	CButton   m_btnRead;
	CButton   m_btnSet;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
public:
	void ShowRespondContent(LPCSTR szRet);
	void SendCheckRequest();
	void SendSetRequest();
	void ClearAll();

private:
	CString MapIndexToString(UINT uIndexType, UINT uIndexItem);

private:
	BOOL m_bInit;

	CBrush  m_brshDlgBckGrnd;
    static const COLORREF CLR_DLGBCKGRND;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysconfigPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSysconfigPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnReadSysconfig();
	afx_msg void OnBtnSetSysconfig();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSCONFIGPAGE_H__5A6C41E5_8B98_4F30_972B_1678A1C3ECF5__INCLUDED_)
