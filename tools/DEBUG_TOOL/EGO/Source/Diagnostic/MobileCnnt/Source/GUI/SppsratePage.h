#if !defined(AFX_SPPSRATEPAGE_H__2AE7A4ED_F0FF_40C8_A5DB_D51A20ABCC8C__INCLUDED_)
#define AFX_SPPSRATEPAGE_H__2AE7A4ED_F0FF_40C8_A5DB_D51A20ABCC8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SppsratePage.h : header file
//
const CString SRATE_TYPE[]=
{
	_T("Wap"),
	_T("MMS"),
	_T("Download"),
	_T("Streaming"),
	_T("IM"),
	_T("POC")
};

typedef enum
{
	FOR_SRATE_READ,
	FOR_SRATE_TEST
};
#define NUM_SPRATE 6
/////////////////////////////////////////////////////////////////////////////
// CSppsratePage dialog

class CSppsratePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSppsratePage)

// Construction
public:
	CSppsratePage();
	~CSppsratePage();

// Dialog Data
	//{{AFX_DATA(CSppsratePage)
	enum { IDD = IDD_PAGE_SPPSRATE };
	CListCtrl m_lstRead;
	CListCtrl m_lstTest;
	CComboBox m_boxType;
	UINT	  m_uSpeedUp;
	UINT	  m_uSpeedDown;
	CButton   m_btnRead;
	CButton   m_btnTest;
	CButton   m_btnSet;
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
	
	CBrush  m_brshDlgBckGrnd;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSppsratePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSppsratePage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnReadSrate();
	afx_msg void OnBtnTestSrate();
	afx_msg void OnBtnSetSrate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPPSRATEPAGE_H__2AE7A4ED_F0FF_40C8_A5DB_D51A20ABCC8C__INCLUDED_)
