#if !defined(AFX_COOLFORMVIEW_H__EBFF7CBD_FE34_473F_8B47_E24CF543551B__INCLUDED_)
#define AFX_COOLFORMVIEW_H__EBFF7CBD_FE34_473F_8B47_E24CF543551B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CoolFormView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCoolFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CCoolFormView : public CFormView
{
protected:
	CCoolFormView( UINT nIDTemplate );           // protected constructor used by dynamic creation

// Form Data
public:
	//{{AFX_DATA(CCoolFormView)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
//	CBitmap m_bmpDlgBckGrnd;
	CBrush  m_brshDlgBckGrnd;
    static const COLORREF CLR_DLGBCKGRND;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolFormView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCoolFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCoolFormView)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLFORMVIEW_H__EBFF7CBD_FE34_473F_8B47_E24CF543551B__INCLUDED_)
