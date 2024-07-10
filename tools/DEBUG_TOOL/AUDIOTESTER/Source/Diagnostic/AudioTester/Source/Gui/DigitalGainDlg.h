#if !defined(AFX_DIGITALGAINDLG_H__8656DE14_9598_46D9_9B45_8829BACA215E__INCLUDED_)
#define AFX_DIGITALGAINDLG_H__8656DE14_9598_46D9_9B45_8829BACA215E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DigitalGainDlg.h : header file
//

#include "BMPCtrlWithSpecRect.h"

/////////////////////////////////////////////////////////////////////////////
// CDigitalGainDlg dialog

class CDigitalGainDlg : public CDialog
{
// Construction
public:
	CDigitalGainDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDigitalGainDlg)
	enum { IDD = IDD_DIALOG_DIGITAL_GAIN };
	double	m_dDpAtt;
	double	m_dGain;
	CString	m_strDpatt;
	CString	m_strGain;
	//}}AFX_DATA

    SpecialRectOfGain* m_pArea;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDigitalGainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDigitalGainDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillfocusEditDpatt();
	afx_msg void OnKillfocusEditGain();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIGITALGAINDLG_H__8656DE14_9598_46D9_9B45_8829BACA215E__INCLUDED_)
