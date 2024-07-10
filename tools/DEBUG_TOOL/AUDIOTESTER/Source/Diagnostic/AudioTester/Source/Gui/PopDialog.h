#if !defined(AFX_POPDIALOG_H__9F9BC4FA_70C4_478E_8035_70503A5D81EC__INCLUDED_)
#define AFX_POPDIALOG_H__9F9BC4FA_70C4_478E_8035_70503A5D81EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PopDialog.h : header file
//
#include "BMPCtrlWithSpecRect.h"

/////////////////////////////////////////////////////////////////////////////
// CPopDialog dialog

class CPopDialog : public CDialog
{
// Construction
public:
	CPopDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPopDialog)
	enum { IDD = IDD_DIALOG_POP };
	int		m_nPopSwitch;
	short	m_nDaLimit;
	BOOL	m_bWriteToPhone;
	//}}AFX_DATA

    SpecialRectOfPop* m_pArea;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPopDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPDIALOG_H__9F9BC4FA_70C4_478E_8035_70503A5D81EC__INCLUDED_)
