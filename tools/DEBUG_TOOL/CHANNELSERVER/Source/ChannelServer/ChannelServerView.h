// ChannelServerView.h : interface of the CChannelServerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANNELSERVERVIEW_H__49A99460_7DB4_4F4B_935F_F5CC48F7E303__INCLUDED_)
#define AFX_CHANNELSERVERVIEW_H__49A99460_7DB4_4F4B_935F_F5CC48F7E303__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CChannelServerView : public CView
{
protected: // create from serialization only
	CChannelServerView();
	DECLARE_DYNCREATE(CChannelServerView)

// Attributes
public:
	CChannelServerDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChannelServerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChannelServerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CChannelServerView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ChannelServerView.cpp
inline CChannelServerDoc* CChannelServerView::GetDocument()
   { return (CChannelServerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANNELSERVERVIEW_H__49A99460_7DB4_4F4B_935F_F5CC48F7E303__INCLUDED_)
