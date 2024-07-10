#if !defined(AFX_KEYINFOFRAME_H__019F6ACA_494C_47D4_801A_603AA4B469A0__INCLUDED_)
#define AFX_KEYINFOFRAME_H__019F6ACA_494C_47D4_801A_603AA4B469A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyInfoFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKeyInfoFrame frame

class CKeyInfoFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CKeyInfoFrame)
protected:
	CKeyInfoFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

private:
	CToolBar     m_wndToolBar;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyInfoFrame)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);

// Implementation
protected:
	virtual ~CKeyInfoFrame();

	// Generated message map functions
	//{{AFX_MSG(CKeyInfoFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYINFOFRAME_H__019F6ACA_494C_47D4_801A_603AA4B469A0__INCLUDED_)
