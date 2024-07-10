#if !defined(AFX_CONTENTVIEW_H__0BBC4829_6B93_48A1_A26F_E09C9D687077__INCLUDED_)
#define AFX_CONTENTVIEW_H__0BBC4829_6B93_48A1_A26F_E09C9D687077__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ContentView.h : header file
//

class CInfoWrapper;  
/////////////////////////////////////////////////////////////////////////////
// CContentView view
class CContentView : public CEditView
{
public:
	CContentView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CContentView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContentView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* /*pDC*/);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CContentView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	void UpdateContent(CObject * pHint);
	//{{AFX_MSG(CContentView)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CInfoWrapper * m_pIW;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTENTVIEW_H__0BBC4829_6B93_48A1_A26F_E09C9D687077__INCLUDED_)
