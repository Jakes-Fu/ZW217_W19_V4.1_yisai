#if !defined(AFX_SPIDPAGE_H__6E1DE7CE_D8E0_4AD0_9E17_B933D97D3802__INCLUDED_)
#define AFX_SPIDPAGE_H__6E1DE7CE_D8E0_4AD0_9E17_B933D97D3802__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpidPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpidPage dialog

class CSpidPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSpidPage)

// Construction
public:
	CSpidPage();
	~CSpidPage();

// Dialog Data
	//{{AFX_DATA(CSpidPage)
	enum { IDD = IDD_PAGE_SPID };
	CString m_strTmsi;
	CString m_strPtmsi;
	CString m_strImeisy;
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA
public:
	void SendCheckRequest();
	void ShowRespondContent(LPCSTR szRet);
	void ClearAll();

	CBrush  m_brshDlgBckGrnd;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSpidPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSpidPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSpidRear();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPIDPAGE_H__6E1DE7CE_D8E0_4AD0_9E17_B933D97D3802__INCLUDED_)
