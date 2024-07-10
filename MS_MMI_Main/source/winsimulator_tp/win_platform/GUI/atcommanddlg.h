#if !defined(AFX_ATCOMMANDDLG_H__D8AD045D_38F5_4634_89D3_D459F9FAD728__INCLUDED_)
#define AFX_ATCOMMANDDLG_H__D8AD045D_38F5_4634_89D3_D459F9FAD728__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ATCommandDlg.h : header file
//
#include "sio.h"

/////////////////////////////////////////////////////////////////////////////
// CATCommandDlg dialog

class CATCommandDlg : public CDialog
{
// Interface
public:
    void SendATResponse(CString strATResponse);

// Construction
public:
	CATCommandDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CATCommandDlg)
	enum { IDD = IDD_AT };
	CEdit	m_wndResponse;
	CEdit	m_wndRequest;
	CString	m_strResponse;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CATCommandDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CATCommandDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
    afx_msg void OnClose();

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Function
private:
    void         SendATCommand(CString strCommand);

// Variable 
private:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATCOMMANDDLG_H__D8AD045D_38F5_4634_89D3_D459F9FAD728__INCLUDED_)
