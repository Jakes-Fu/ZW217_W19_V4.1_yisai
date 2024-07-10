#if !defined(AFX_SMSFRAME_H__82991681_1B41_4B61_8EC7_7E09AE143284__INCLUDED_)
#define AFX_SMSFRAME_H__82991681_1B41_4B61_8EC7_7E09AE143284__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SMSFrame.h : header file
//
#include "SMSView.h"
#include "effect\\CoolStatusBar.h"
#include "CoolToolBar.h"


class CSMSToolBar
#ifdef SPRD8800
 : public CToolBar 
#else
 : public CCoolToolBar
#endif
{
public:
    CComboBox m_cmbox_channel;
    CStatic   m_staic_status;
    
public:
    BOOL CreateBar(CWnd* pParentWnd);

private:
    int  CreateItems( );

};
/////////////////////////////////////////////////////////////////////////////
// CSMSFrame frame

class CSMSFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CSMSFrame)
protected:
	CSMSFrame();           // protected constructor used by dynamic creation

// Attributes
public:

public:
	CSMSToolBar m_wndToolBar;
	BOOL        m_bShowContentBar;
	BOOL		m_bShowCategBar;
	CCoolStatusBar  m_wndStatusBar;


// Operations
public:
	CSMSView* m_pSMSView;

	CCoolBar  * m_pCategoryBar;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMSFrame)
	public:
	protected:
		virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSMSFrame();

	// Generated message map functions
	//{{AFX_MSG(CSMSFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnViewContentBar();
	afx_msg void OnUpdateViewContentBar(CCmdUI* pCmdUI);
	afx_msg void OnViewCategBar();
	afx_msg void OnUpdateViewCategBar(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnProgressMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSFRAME_H__82991681_1B41_4B61_8EC7_7E09AE143284__INCLUDED_)



