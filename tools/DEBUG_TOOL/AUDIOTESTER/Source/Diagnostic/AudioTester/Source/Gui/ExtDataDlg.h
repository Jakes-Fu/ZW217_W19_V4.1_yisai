#if !defined(AFX_EXTDATADLG_H__633187AF_7996_4DF0_BA36_382185A10240__INCLUDED_)
#define AFX_EXTDATADLG_H__633187AF_7996_4DF0_BA36_382185A10240__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtDataDlg.h : header file
//

#include "HexEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CExtDataDlg dialog

class CExtDataDlg : public CDialog
{
// Construction
public:
	CExtDataDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExtDataDlg)
	enum { IDD = IDD_DIALOG_EXTEND_DATA };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExtDataDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    CHexEdit m_wndBuffer;
    LPBYTE m_lpBuffer;
    int m_nDataSize;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTDATADLG_H__633187AF_7996_4DF0_BA36_382185A10240__INCLUDED_)
