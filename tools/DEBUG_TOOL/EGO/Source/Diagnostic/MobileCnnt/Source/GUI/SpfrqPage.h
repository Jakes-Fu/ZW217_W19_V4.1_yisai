#if !defined(AFX_SPFRQPAGE_H__36C6E254_7EF9_48BE_9DAE_E624E8F84DCE__INCLUDED_)
#define AFX_SPFRQPAGE_H__36C6E254_7EF9_48BE_9DAE_E624E8F84DCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpfrqPage.h : header file
//

typedef enum
{
	FOR_SPFRQ_READ,
	FOR_SPFRQ_TEST
};
#define NUM_SPFRQ 4
/////////////////////////////////////////////////////////////////////////////
// CSpfrqPage dialog

class CSpfrqPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSpfrqPage)

// Construction
public:
	CSpfrqPage();
	~CSpfrqPage();

// Dialog Data
	//{{AFX_DATA(CSpfrqPage)
	enum { IDD = IDD_PAGE_SPFRQ };
	CListCtrl m_lstRead;
	CListCtrl m_lstTest;
	CComboBox m_boxOpr;
	CComboBox m_boxIndex;
	CButton   m_btnRead;
	CButton   m_btnTest;
	CButton   m_btnSet;
	CEdit     m_edtFrenq;
	CEdit     m_edtCell1;
	CEdit     m_edtCell2;
	CEdit     m_edtCell3;
	UINT	  m_uFrenq;
	UINT	  m_uCell1;
	UINT	  m_uCell2;
	UINT	  m_uCell3;

		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA
public:
	void ShowRespondContent(LPCSTR szRet);
	void SendCheckRequest(UINT uFlag);
	void SendSetRequest();
	void ClearAll();

private:
	BOOL m_bInit;
	UINT m_uIndex;

	CBrush  m_brshDlgBckGrnd;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSpfrqPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSpfrqPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnReadSpfrq();
	afx_msg void OnBtnTestSpfrq();
	afx_msg void OnBtnSetSpfrq();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPFRQPAGE_H__36C6E254_7EF9_48BE_9DAE_E624E8F84DCE__INCLUDED_)
