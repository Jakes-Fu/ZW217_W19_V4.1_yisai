#if !defined(AFX_CHILDFRAMEWND_H__EBB552DF_94B2_4C9C_8082_3498B2D04B9A__INCLUDED_)
#define AFX_CHILDFRAMEWND_H__EBB552DF_94B2_4C9C_8082_3498B2D04B9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// childframewnd.h : header file
//
#include "HeaderBar.h"
#include "CoolBar.h"
/////////////////////////////////////////////////////////////////////////////
// CChildFrameWnd frame

class CChildFrameWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CChildFrameWnd)
protected:
	CChildFrameWnd();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	BOOL ContentBarIsVisible(void) { return m_ContentBar.GetStyle() & WS_VISIBLE; }
	void ShowContentBar( BOOL bShow );
	void SetHeaderText(LPCTSTR lpText) 
	{ 
		m_HeaderBar.SetText(lpText); 
		m_HeaderBar.Invalidate(); 
	}
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrameWnd)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CChildFrameWnd();

	// Generated message map functions
	//{{AFX_MSG(CChildFrameWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CHeaderBar m_HeaderBar;
	CCoolBar   m_ContentBar;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRAMEWND_H__EBB552DF_94B2_4C9C_8082_3498B2D04B9A__INCLUDED_)
