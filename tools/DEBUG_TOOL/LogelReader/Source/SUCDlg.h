// SUCDlg.h : header file
//

#if !defined(AFX_SUCDLG_H__CF0CEDA8_809F_4BA5_9278_8D2DA3751059__INCLUDED_)
#define AFX_SUCDLG_H__CF0CEDA8_809F_4BA5_9278_8D2DA3751059__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tab1Dlg.h"
#include "Tab2Dlg.h"
#include "DeviceSendDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CSUCDlg dialog

class CSUCDlg : public CDialog
{
// Construction
public:
	void DoTab(int nTab);
	void SetDlgState(CWnd *pWnd,BOOL bShow);
	void GetDisplayPage();
	void SetPage();
	char m_szFileDir[MAX_PATH];
	CTab2Dlg* m_pTab2Dlg;
	CTab1Dlg* m_pTab1Dlg;
	CDeviceSendDlg* m_pDeviceDlg;
	CSUCDlg(CWnd* pParent = NULL);	// standard constructor


public:

// Dialog Data
	//{{AFX_DATA(CSUCDlg)
	enum { IDD = IDD_SUC_DIALOG };
	CTabCtrl	m_Tab;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSUCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSUCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUCDLG_H__CF0CEDA8_809F_4BA5_9278_8D2DA3751059__INCLUDED_)
