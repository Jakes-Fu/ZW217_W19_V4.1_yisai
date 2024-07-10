#if !defined(AFX_SPTESTPAGE_H__0D20CD1A_9547_4E09_9EAB_612B96D58DFA__INCLUDED_)
#define AFX_SPTESTPAGE_H__0D20CD1A_9547_4E09_9EAB_612B96D58DFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SptestPage.h : header file
//

typedef enum
{
	FTA,
	PROTECT,
	NETWORK_REAL,
	AOC,
	GSM_SIM,
	PROTOCOL_VERSION,
	PS,
	UE
};

#define NUM_AUTOATT 8
/////////////////////////////////////////////////////////////////////////////
// CSptestPage dialog

class CSptestPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSptestPage)

// Construction
public:
	CSptestPage();
	~CSptestPage();

// Dialog Data
	//{{AFX_DATA(CSptestPage)
	enum { IDD = IDD_PAGE_SPTEST };
	CListCtrl m_lstRead;
	CComboBox m_boxType;
	CComboBox m_boxItem;
	CButton   m_btnRead;
	CButton   m_btnSet;
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

protected:
	void SetItemBox(DWORD dwType);


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSptestPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
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
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSptestPage)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboSptestType();
	afx_msg void OnBtnReadSptest();
	afx_msg void OnBtnSetSptest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPTESTPAGE_H__0D20CD1A_9547_4E09_9EAB_612B96D58DFA__INCLUDED_)
