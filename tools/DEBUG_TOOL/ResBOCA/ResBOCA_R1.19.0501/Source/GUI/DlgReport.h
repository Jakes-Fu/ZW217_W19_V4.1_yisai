#if !defined(AFX_DLGREPORT_H__90532A08_D562_4962_BB9E_1A2C081B5B73__INCLUDED_)
#define AFX_DLGREPORT_H__90532A08_D562_4962_BB9E_1A2C081B5B73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgReport.h : header file
//
#include "ResParser.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgReport dialog

class CDlgReport : public CDialog
{
// Construction
public:
	CDlgReport(CWnd* pParent = NULL);   // standard constructor
	~CDlgReport();
// Dialog Data
	//{{AFX_DATA(CDlgReport)
	enum { IDD = IDD_DLG_STAT };
	CListCtrl	m_lstReport;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgReport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	CResParser* m_pResParser;


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgReport)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGREPORT_H__90532A08_D562_4962_BB9E_1A2C081B5B73__INCLUDED_)
