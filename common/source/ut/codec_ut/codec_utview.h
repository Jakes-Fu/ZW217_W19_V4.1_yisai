// CODEC_UTView.h : interface of the CCODEC_UTView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CODEC_UTVIEW_H__DA6323EB_9B85_4609_AEBA_2008692C7F3D__INCLUDED_)
#define AFX_CODEC_UTVIEW_H__DA6323EB_9B85_4609_AEBA_2008692C7F3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCODEC_UTView : public CView
{
protected: // create from serialization only
	CCODEC_UTView();
	DECLARE_DYNCREATE(CCODEC_UTView)

// Attributes
public:
	CCODEC_UTDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCODEC_UTView)
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
	virtual ~CCODEC_UTView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCODEC_UTView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CODEC_UTView.cpp
inline CCODEC_UTDoc* CCODEC_UTView::GetDocument()
   { return (CCODEC_UTDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CODEC_UTVIEW_H__DA6323EB_9B85_4609_AEBA_2008692C7F3D__INCLUDED_)
