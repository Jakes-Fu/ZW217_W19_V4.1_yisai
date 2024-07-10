#if !defined(AFX_FILTERMAINFRAME_H__4BF1EC9C_EA00_46BF_BEAC_7742FCA7D7DC__INCLUDED_)
#define AFX_FILTERMAINFRAME_H__4BF1EC9C_EA00_46BF_BEAC_7742FCA7D7DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterMainFrame.h : header file
//
#include "FilterChartView.h"
#include "FilterChartCtrl.h"
#include "FilterDoc.h"


/////////////////////////////////////////////////////////////////////////////
// CFilterMainFrame frame
class CCoolBar;
class CFilterMainFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CFilterMainFrame)
protected:
	CFilterMainFrame();           // protected constructor used by dynamic creation

public:
    CFilterDoc* m_pDoc;


public:
    CCoolBar*  CreateCoolBar(CRuntimeClass*pRT , LPCREATESTRUCT lpCreateStruct ,CSize size );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterMainFrame)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFilterMainFrame();

	// Generated message map functions
	//{{AFX_MSG(CFilterMainFrame)
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateChartctrl(CCmdUI* pCmdUI);
	afx_msg void OnChartctrl();
	afx_msg void OnExit();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:  
    CToolBar   m_wndToolBar;
    CCoolBar * m_pBarCtrl;
    BOOL       m_bShowBar;   

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERMAINFRAME_H__4BF1EC9C_EA00_46BF_BEAC_7742FCA7D7DC__INCLUDED_)
