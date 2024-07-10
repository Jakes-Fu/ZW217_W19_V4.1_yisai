#if !defined(AFX_RIGHTWND_H__25F0BAE0_9218_4DD1_B79A_A29E6FB932D4__INCLUDED_)
#define AFX_RIGHTWND_H__25F0BAE0_9218_4DD1_B79A_A29E6FB932D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RightWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFindRightWnd frame
class CCoolBar;
class CFindRightWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CFindRightWnd)
protected:
	CFindRightWnd();           // protected constructor used by dynamic creation

// Attributes
public:

	// Operations
public:

    BOOL IsShowBar(void);
    void ShowOrHideBar(void);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindRightWnd)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFindRightWnd();

	// Generated message map functions
	//{{AFX_MSG(CFindRightWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    CCoolBar * m_pBarExt;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RIGHTWND_H__25F0BAE0_9218_4DD1_B79A_A29E6FB932D4__INCLUDED_)
