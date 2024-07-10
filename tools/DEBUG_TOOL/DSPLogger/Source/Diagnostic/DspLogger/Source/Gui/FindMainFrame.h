#if !defined(AFX_FINDFRAME_H__5DF224C3_4A1D_4841_8A45_E17C602E79D0__INCLUDED_)
#define AFX_FINDFRAME_H__5DF224C3_4A1D_4841_8A45_E17C602E79D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFindMainFrame frame
class CFindMainFrame : public CFrameWnd 
{
	DECLARE_DYNCREATE(CFindMainFrame)
protected:
	CFindMainFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	//frame index
	int m_nFrameIndex;

// Operations
public:
	VOID SetInputItem();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindMainFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

    virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
// Implementation
protected:
	virtual ~CFindMainFrame();

    //virtual void ActivateFrame(int nCmdShow = -1);
    //virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	// Generated message map functions
	//{{AFX_MSG(CFindMainFrame)
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnShowChartCtrl();
	afx_msg void OnShowFind();
	afx_msg void OnUpdateShowChartCtrl(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowFind(CCmdUI* pCmdUI);
	afx_msg void OnFindClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    CSplitterWnd m_Splitter;
    CToolBar     m_wndToolBar;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDFRAME_H__5DF224C3_4A1D_4841_8A45_E17C602E79D0__INCLUDED_)
