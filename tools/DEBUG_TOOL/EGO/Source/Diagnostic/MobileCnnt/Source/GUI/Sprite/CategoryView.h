#if !defined(AFX_CATEGORYVIEW_H__6400B3D4_D986_4F98_97AF_4FCC3AAA065D__INCLUDED_)
#define AFX_CATEGORYVIEW_H__6400B3D4_D986_4F98_97AF_4FCC3AAA065D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CategoryView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCategoryView view
class CMobileSpriteDoc;
class CCategoryView : public CTreeView
{
protected:
	CCategoryView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCategoryView)

// Attributes
public:
	CMobileSpriteDoc * GetDocument();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCategoryView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* /*pDC*/);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* /*pHint*/);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCategoryView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	void RecursiveFillCtrlContent( LPVOID pItem, HTREEITEM hParent );
	void FillCtrlContent(void);
	//{{AFX_MSG(CCategoryView)
	afx_msg void OnDestroy();
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bInit;
	BOOL m_bDelete;

	CImageList * m_pImgLst;
};

#ifndef _DEBUG  // debug version in CategoryView.cpp
inline CMobileSpriteDoc* CCategoryView::GetDocument()
   { return (CMobileSpriteDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATEGORYVIEW_H__6400B3D4_D986_4F98_97AF_4FCC3AAA065D__INCLUDED_)
