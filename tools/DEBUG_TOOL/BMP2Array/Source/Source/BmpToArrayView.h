// BmpToArrayView.h : interface of the CBmpToArrayView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMPTOARRAYVIEW_H__55275989_7DE7_4DC0_BFF4_4DC4A96EDD27__INCLUDED_)
#define AFX_BMPTOARRAYVIEW_H__55275989_7DE7_4DC0_BFF4_4DC4A96EDD27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XTabCtrl.h"
#include "DlgMain.h"
#include "DlgOSD.h"
#include "Dlg3D.h"

class CBmpToArrayView : public CFormView
{
protected: // create from serialization only
	CBmpToArrayView();
	DECLARE_DYNCREATE(CBmpToArrayView)

public:
	//{{AFX_DATA(CBmpToArrayView)
	enum { IDD = IDD_BMPTOARRAY_FORM };
	CXTabCtrl	m_tabMain;
	//}}AFX_DATA

// Attributes
public:
	CBmpToArrayDoc* GetDocument();

protected:
	CDlgMain * m_pDlgMain;
	CDlgOSD  * m_pDlgOSD;
	CDlg3D   * m_pDlg3D;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBmpToArrayView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBmpToArrayView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBmpToArrayView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BmpToArrayView.cpp
inline CBmpToArrayDoc* CBmpToArrayView::GetDocument()
   { return (CBmpToArrayDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMPTOARRAYVIEW_H__55275989_7DE7_4DC0_BFF4_4DC4A96EDD27__INCLUDED_)
