// MobileCnntView.h : interface of the CMobileCnntView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOBILECNNTVIEW_H__4383735D_2285_4715_91D2_3C3C3A8AF4A9__INCLUDED_)
#define AFX_MOBILECNNTVIEW_H__4383735D_2285_4715_91D2_3C3C3A8AF4A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMobileCnntView : public CView
{
protected: // create from serialization only
	CMobileCnntView();
	DECLARE_DYNCREATE(CMobileCnntView)

// Attributes
public:
	CMobileCnntDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMobileCnntView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMobileCnntView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	

// Generated message map functions
protected:
	//{{AFX_MSG(CMobileCnntView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MobileCnntView.cpp
inline CMobileCnntDoc* CMobileCnntView::GetDocument()
   { return (CMobileCnntDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOBILECNNTVIEW_H__4383735D_2285_4715_91D2_3C3C3A8AF4A9__INCLUDED_)
