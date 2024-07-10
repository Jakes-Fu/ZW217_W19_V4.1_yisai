#if !defined(AFX_DIALOGEX_H__98229B31_9513_486F_BA15_8888A8728C4E__INCLUDED_)
#define AFX_DIALOGEX_H__98229B31_9513_486F_BA15_8888A8728C4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CoolDlg.h : header file
//
#include "CoolButton.h"

/////////////////////////////////////////////////////////////////////////////
// CCoolDlg dialog

class CCoolDlg : public CDialog
{
// Construction
public:
	CCoolDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCoolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnClose();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* /*pWnd*/, UINT /*nCtlColor*/);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	CBitmap m_bmpDlgTitle;
	CBitmap m_bmpDlgBckGrnd;
	CBrush  m_brshDlgBckGrnd;

	CString m_strDlgCaption;

	static const COLORREF CLR_DLGBCKGRND;
	static const COLORREF CLR_HOVER;
	static const COLORREF CLR_CLICKED;
	static const COLORREF CLR_DISABLED;
private:
	CCoolButton m_btnClose;
};

//
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGEX_H__98229B31_9513_486F_BA15_8888A8728C4E__INCLUDED_)
