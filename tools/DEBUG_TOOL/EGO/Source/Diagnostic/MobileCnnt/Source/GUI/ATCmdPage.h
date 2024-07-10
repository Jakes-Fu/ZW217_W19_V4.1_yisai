#if !defined(AFX_ATCMDPAGE_H__DE915A5F_BF11_4492_8243_CEF997029AC9__INCLUDED_)
#define AFX_ATCMDPAGE_H__DE915A5F_BF11_4492_8243_CEF997029AC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ATCmdPage.h : header file
//
#define MAX_LIST_COUNT 10000
/////////////////////////////////////////////////////////////////////////////
// CATCmdPage dialog

class CATCmdPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CATCmdPage)

// Construction
public:
	CATCmdPage();
	~CATCmdPage();

// Dialog Data
	//{{AFX_DATA(CATCmdPage)
	enum { IDD = IDD_PAGE_CLIENT };
	CString m_strCmd;
	CListBox m_lstResult;
	CString m_strFile;
	CButton m_btnFile;
	CButton m_btnLine;
	CButton m_btnBrowse;
	CButton m_btnStopCheck;
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA
public:
	void ClearAll();
	void SendCmd();
	void SendCmdEx();
	void ShowRespondContent(LPCSTR szRet);
private:
	void AddTextToList(CString strText);
	void BinaryToHexChar(LPBYTE lpSrc, int nSrcSize, LPSTR lpDes, int &nDeSize, BOOL bBigEndian = TRUE);
	

private:
	
	CBrush  m_brshDlgBckGrnd;
	BOOL m_bInit;
	BOOL m_bStopCheck;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CATCmdPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CATCmdPage)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnSend();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnCmdBrowse();
	afx_msg void OnRadioScript();
	afx_msg void OnRadioLine();
	afx_msg void OnBtnClear();
	afx_msg void OnBtnStopCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATCMDPAGE_H__DE915A5F_BF11_4492_8243_CEF997029AC9__INCLUDED_)
