#if !defined(AFX_SAUTOATTPAGE_H__26123EA7_478A_4D8C_9E4F_A920766197DD__INCLUDED_)
#define AFX_SAUTOATTPAGE_H__26123EA7_478A_4D8C_9E4F_A920766197DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SautoattPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSautoattPage dialog

class CSautoattPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSautoattPage)

// Construction
public:
	CSautoattPage();
	~CSautoattPage();

// Dialog Data
	//{{AFX_DATA(CSautoattPage)
	enum { IDD = IDD_PAGE_SAUTOATT };
	CButton m_btnSet;
	CButton m_btnRead;
	CComboBox m_boxMode;
	CString   m_strAttch;
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA
public:
	void ShowRespondContent(LPCSTR szRet);
	void SendCheckRequest();
	void SendSetRequest();
	void ClearAll();

private:
	BOOL m_bInit;
	
	CBrush  m_brshDlgBckGrnd;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSautoattPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSautoattPage)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAutoattRead();
	afx_msg void OnBtnAutoattSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAUTOATTPAGE_H__26123EA7_478A_4D8C_9E4F_A920766197DD__INCLUDED_)
