#if !defined(AFX_DLGPROGRESS_H__4F676623_80CA_4D87_921B_EF17669C2E5E__INCLUDED_)
#define AFX_DLGPROGRESS_H__4F676623_80CA_4D87_921B_EF17669C2E5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProgress.h : header file
//
#include "..\resource.h"
struct _PROGRESS_PARAM_T;
/////////////////////////////////////////////////////////////////////////////
// CDlgProgress dialog

class CDlgProgress : public CDialog
{
// Construction
public:
	CDlgProgress(CWnd* pParent = NULL);   // standard constructor

	void ProgBegin(int nTotalNum);
    void ProgProceed(int nCurNum);
	void ProgEnd();
	void ChangTitle(LPCTSTR lpszTitle);

// Dialog Data
	//{{AFX_DATA(CDlgProgress)
	enum { IDD = IDD_DLG_PROGRESS };
	CString	m_strTitle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgProgress)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
    //afx_msg void OnProgress(WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

private:
	int m_nTotalNum;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROGRESS_H__4F676623_80CA_4D87_921B_EF17669C2E5E__INCLUDED_)
