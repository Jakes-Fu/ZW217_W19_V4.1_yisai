#if !defined(AFX_BASICCHILDFRAME_H__7009AD13_DE9F_4C38_9805_E32D562DAD67__INCLUDED_)
#define AFX_BASICCHILDFRAME_H__7009AD13_DE9F_4C38_9805_E32D562DAD67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BasicChildFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBasicChildFrame frame

class CBasicChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CBasicChildFrame)
protected:
	CBasicChildFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
    void SetMinFrmSize( UINT nMinWidth, UINT nMinHeight );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBasicChildFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CBasicChildFrame();

	// Generated message map functions
	//{{AFX_MSG(CBasicChildFrame)
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    CSize  m_szMinFrmSize;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASICCHILDFRAME_H__7009AD13_DE9F_4C38_9805_E32D562DAD67__INCLUDED_)
