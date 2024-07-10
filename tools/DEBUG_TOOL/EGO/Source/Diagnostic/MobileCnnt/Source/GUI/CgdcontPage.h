#if !defined(AFX_CGDCONTPAGE_H__308E6834_97EF_4ACA_B30D_E5AFA91B1ABF__INCLUDED_)
#define AFX_CGDCONTPAGE_H__308E6834_97EF_4ACA_B30D_E5AFA91B1ABF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CgdcontPage.h : header file
//


const CString CGDCONT_TYPE[]=
{
	_T("IP"),
	_T("Other")
};

const CString CGDCONT_CNNT_IN[]=
{
	_T("cmnet"),
	_T("cmwap"),
	_T("Other")
};

const CString CGDCONT_COMP[]=
{
	_T("No"),
	_T("Yes")
};

typedef enum
{
	FOR_CGDCONT_READ,
	FOR_CGDCONT_TEST
};

#define LST_CGDCONT_READ_NUM 16
/////////////////////////////////////////////////////////////////////////////
// CCgdcontPage dialog

class CCgdcontPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCgdcontPage)

// Construction
public:
	CCgdcontPage();
	~CCgdcontPage();

// Dialog Data
	//{{AFX_DATA(CCgdcontPage)
	enum { IDD = IDD_PAGE_CGDCONT };
	CListCtrl m_lstRead;
	CListCtrl m_lstTest;
	CComboBox m_boxID;
	CComboBox m_boxType;
	CComboBox m_boxCnntIn;
	CComboBox m_boxAddr;
	CComboBox m_boxDComp;
	CComboBox m_boxHComp;
	CEdit     m_edtAddr;
	CButton   m_btnRead;
	CButton   m_btnTest;
	CButton   m_btnSet;
	CButton   m_btnAddr;
	CString	  m_strAddr;
	CString   m_strEdtType;
	CString   m_strEdtCnntIn;
	CEdit	  m_edtType;
	CEdit     m_edtCnntIn;
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA
public:
	void ShowRespondContent(LPCSTR szRet);
	void SendCheckRequest(UINT uFlag);
	void SendSetRequest();
	void ClearAll();

protected:
	BOOL m_bInit;
	
	CBrush  m_brshDlgBckGrnd;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCgdcontPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCgdcontPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCheckCgdcontAddr();
	afx_msg void OnBtnReadCgdcont();
	afx_msg void OnBtnTestCgdcont();
	afx_msg void OnBtnSetCgdcont();
	afx_msg void OnSelchangeComboCgdcontType();
	afx_msg void OnSelchangeComboCgdcontCnntIn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CGDCONTPAGE_H__308E6834_97EF_4ACA_B30D_E5AFA91B1ABF__INCLUDED_)
