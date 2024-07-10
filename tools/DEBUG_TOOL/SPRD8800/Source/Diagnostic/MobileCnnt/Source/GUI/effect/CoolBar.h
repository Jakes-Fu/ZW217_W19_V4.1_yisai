// 
// FileName: CoolBar.h
// anli.wei 整理完成
// Copyright (C) 2004 Spreadtrum Corporation
// All rights reserved.

#if !defined(__COOLBAR_H__)
#define __COOLBAR_H__

#ifndef __SIZINGCONTROLBAR_H__
	#include "SizingControlBar.h"
#endif // __SIZINGCONTROLBAR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


/////////////////////////////////////////////////////////////////////////
// CSCBButton (button info) helper class

class CSCBButton
{
public:
    CSCBButton();

    void Move(const CPoint & ptTo) { m_ptOrg = ptTo; };
    LPCRECT GetRect() 
	{ 
		m_rcBtn = CRect( m_ptOrg, CSize(13, 13) );
		return m_rcBtn; 
	} 

    void Paint(CDC * pDC);

    BOOL m_bPushed;
    BOOL m_bRaised;

	COLORREF m_clrForeGrnd;

protected:
    CPoint   m_ptOrg;
	CRect    m_rcBtn;
};

/////////////////////////////////////////////////////////////////////////
// CSizingControlBar control bar

class CCoolBar : public CSizingControlBar
{
    DECLARE_DYNCREATE(CCoolBar);

// Operations
public:
	// 目前我们只使用一个child，且不使用lpszLabel和hIcon
    BOOL AddPage( CRuntimeClass * pRtc, CCreateContext * pContext = NULL,
		          LPCTSTR lpszLabel  =  NULL, HICON hIcon  = NULL  );

	void SetNotifyWindow(HWND hNotifyWnd);
	HWND GetNotifyWindow() const { return m_hNotifyWnd; }

	void SetHasGripper( BOOL bHasGripper = TRUE ) { m_bHasGripper = bHasGripper; }
    void SetGripperInfo( LPCTSTR pszText, COLORREF clrForeGrnd, COLORREF clrBackGrnd );
// Overrides
public:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCoolBar)
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
    //}}AFX_VIRTUAL
	
    virtual BOOL Create( LPCTSTR lpszWindowName,  CWnd* pParentWnd, 
		                 const CSize &sizeDefault, UINT nID, 
						 BOOL  bHasGripper = FALSE, 
						 DWORD dwStyle     = WS_CHILD | WS_VISIBLE | CBRS_TOP );

    virtual BOOL Create( LPCTSTR lpszWindowName, CWnd* pParentWnd, UINT nID, 
		                 DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP);
	
public:
	
	CCoolBar();
    virtual ~CCoolBar();
    
protected:
    // implementation helpers
    virtual void NcPaintGripper(CDC* pDC, LPCRECT pRect);
    virtual void NcCalcClient(LPRECT pRect, UINT nDockBarID);

protected:
	BOOL    m_bHasGripper;   // 是否显示为平面风格(带标题栏)
	BOOL    m_bActive;       // a child is has focus
    int     m_nGripper;      // Gripper的象素数(高度－竖放 或 宽度－横放)
	HWND	m_hNotifyWnd;	 // 消息通知窗口，默认为父窗口
    
	COLORREF m_clrGripperFore;
	COLORREF m_clrGripperBack;

    CSCBButton m_btnHide;
    
	CString  m_strGripperText;
   
// Generated message map functions
protected:
    //{{AFX_MSG(CCoolBar)
    afx_msg UINT OnNcHitTest(CPoint point);
    afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult );
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
private:
	static const COLORREF CLR_GRIPPER_FORE;
	static const COLORREF CLR_GRIPPER_BACK;
};

#endif // !defined(__COOLBAR_H__)

