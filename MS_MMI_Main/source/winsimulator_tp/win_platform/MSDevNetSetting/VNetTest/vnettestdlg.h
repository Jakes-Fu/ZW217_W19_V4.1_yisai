// VNetTestDlg.h : header file
//

#if !defined(AFX_VNETTESTDLG_H__D6C3DCB5_11A4_4431_B464_C2F817FD1777__INCLUDED_)
#define AFX_VNETTESTDLG_H__D6C3DCB5_11A4_4431_B464_C2F817FD1777__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TestSheet.h"

/////////////////////////////////////////////////////////////////////////////
// CVNetTestDlg dialog

class CVNetTestDlg : public CDialog
{
// Construction
public:
	CVNetTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CVNetTestDlg)
	enum { IDD = IDD_VNETTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVNetTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
    CTestSheet m_sheet;

	// Generated message map functions
	//{{AFX_MSG(CVNetTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VNETTESTDLG_H__D6C3DCB5_11A4_4431_B464_C2F817FD1777__INCLUDED_)
