#if !defined(AFX_LEFTWND_H__53D42C7D_8025_4D68_BF00_9AE5D0D161D6__INCLUDED_)
#define AFX_LEFTWND_H__53D42C7D_8025_4D68_BF00_9AE5D0D161D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LeftWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFindLeftWnd frame
class CCoolBar;
class CFindLeftWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CFindLeftWnd)
protected:
	CFindLeftWnd();           // protected constructor used by dynamic creation

// Attributes
public:


// Operations
public:
	void SetInputItem();

    BOOL IsShowBar(void);
    void ShowOrHideBar(void);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindLeftWnd)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFindLeftWnd();

	// Generated message map functions
	//{{AFX_MSG(CFindLeftWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    CCoolBar * m_pBarExt;
    BOOL       m_bShowBar;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEFTWND_H__53D42C7D_8025_4D68_BF00_9AE5D0D161D6__INCLUDED_)
