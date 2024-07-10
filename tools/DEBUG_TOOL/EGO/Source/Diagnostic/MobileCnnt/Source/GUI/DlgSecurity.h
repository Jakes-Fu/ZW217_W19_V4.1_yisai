#if !defined(AFX_DLGSECURITY_H__8C9102E6_9415_4480_BDD6_20C059BE1303__INCLUDED_)
#define AFX_DLGSECURITY_H__8C9102E6_9415_4480_BDD6_20C059BE1303__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSecurity.h : header file
//
typedef enum
{
	FOR_PIN1,
	FOR_FDN
};
/////////////////////////////////////////////////////////////////////////////
// CDlgSecurity dialog

class CDlgSecurity : public CDialog
{
// Construction
public:
	CDlgSecurity(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSecurity)
	enum { IDD = IDD_DLG_SECURITY };
	CButton m_btnPinOn;
	CButton m_btnPinOff;
	CButton m_btnFdnOn;
	CButton m_btnFdnOff;
	CString m_strPin1;
	CString m_strPin2;
	CString m_strPin2RemainNum;
	CString m_strPin1RemainNum;
	CEdit   m_edtPin1;
	CEdit   m_edtPin2;
	CButton m_btnNodifyPin1;
	CButton m_btnUnlockPin2;
	CButton m_btnUnlockPin1;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


public:
	void ShowRespondContent(LPCSTR szRet);
	void ClearAll();
	void EnableButton(BOOL bPin1OrPin2);
	void EnableWindowAll(BOOL bEnable);
	void SendCheckRequest();

private:
	void SendSetRequest(UINT uFlag);
	void CheckPinRemainNum(BOOL bPin1OrPin2); //TRUE: for PIN1; FALSE: for PIN2

private:
	UINT m_uPin2RemainNum;
	UINT m_uPin1RemainNum;
	BOOL m_bFirstCheck;
	BOOL m_bPin1OrPin2; //TRUE: for PIN1; FALSE: for PIN2
	UINT m_uCount;
	CBrush  m_brshDlgBckGrnd;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSecurity)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSecurity)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnModifyPin1();
	afx_msg void OnBtnPin2Modify();
	afx_msg void OnBtnUnlockPin2();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnPin1Apply();
	afx_msg void OnBtnPin2Apply();
	afx_msg void OnBtnUnlockPin1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSECURITY_H__8C9102E6_9415_4480_BDD6_20C059BE1303__INCLUDED_)
