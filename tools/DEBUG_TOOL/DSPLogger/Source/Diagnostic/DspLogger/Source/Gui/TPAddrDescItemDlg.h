#if !defined(AFX_TPADDRDESCITEMDLG_H__06114B31_2745_46B8_9424_E5C39FAC077D__INCLUDED_)
#define AFX_TPADDRDESCITEMDLG_H__06114B31_2745_46B8_9424_E5C39FAC077D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TPAddrDescItemDlg.h : header file
//

#include "ColorStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CTPAddrDescItemDlg dialog

class CTPAddrDescItemDlg : public CDialog
{
// Construction
public:
	CTPAddrDescItemDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTPAddrDescItemDlg)
	enum { IDD = IDD_DLG_ADD_ADDR_DESC };
	CColorStatic m_stcColor;
	CString	m_strAddr;
	CString m_strData;
	CString	m_strDesc;
	CString	m_strFilter;
	//}}AFX_DATA

    DWORD   m_dwColor;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTPAddrDescItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTPAddrDescItemDlg)
	virtual BOOL OnInitDialog();
    afx_msg void OnBtnColor(void);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPADDRDESCITEMDLG_H__06114B31_2745_46B8_9424_E5C39FAC077D__INCLUDED_)
