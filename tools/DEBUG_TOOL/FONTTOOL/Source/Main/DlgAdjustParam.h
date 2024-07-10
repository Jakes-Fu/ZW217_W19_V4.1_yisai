#if !defined(AFX_DLGADJUSTPARAM_H__C5A08FB8_CADA_4794_925A_D2809459939F__INCLUDED_)
#define AFX_DLGADJUSTPARAM_H__C5A08FB8_CADA_4794_925A_D2809459939F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAdjustParam.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAdjustParam dialog

class CDlgAdjustParam : public CDialog
{
// Construction
public:
	CDlgAdjustParam(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAdjustParam)
	enum { IDD = IDD_DLG_ADJUST_PARAM };
	int		m_nBXDelta;
	int		m_nBYDelta;
	int		m_nBXYRange;
	int		m_nFBBX;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAdjustParam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAdjustParam)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADJUSTPARAM_H__C5A08FB8_CADA_4794_925A_D2809459939F__INCLUDED_)
