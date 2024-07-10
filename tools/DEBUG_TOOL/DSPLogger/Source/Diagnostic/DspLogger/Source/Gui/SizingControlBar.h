// 
// FileName: SizingControlBar.h
// anli.wei 整理完成
// Copyright (C) 2004 Spreadtrum Corporation
// All rights reserved.

#if !defined(__SIZINGCONTROLBAR_H__)
#define __SIZINGCONTROLBAR_H__

#ifndef __AFXPRIV_H__
	#include <afxpriv.h>    // for CDockContext
#endif // __AFXPRIV_H__

#ifndef __AFXTEMPL_H__
	#include <afxtempl.h>   // for CTypedPtrArray
#endif // __AFXTEMPL_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////
// CSizingControlBar control bar styles

#define SCBS_EDGELEFT       0x00000001
#define SCBS_EDGERIGHT      0x00000002
#define SCBS_EDGETOP        0x00000004
#define SCBS_EDGEBOTTOM     0x00000008
#define SCBS_EDGEALL        0x0000000F
#define SCBS_SHOWEDGES      0x00000010

/////////////////////////////////////////////////////////////////////////
// CSizingControlBar control bar
class CSizingControlBar;
typedef CTypedPtrArray<CPtrArray, CSizingControlBar*> CSCBArray;

class CSizingControlBar : public CControlBar
{
    DECLARE_DYNAMIC(CSizingControlBar);

public:
    
    virtual BOOL Create( LPCTSTR lpszWindowName,  CWnd* pParentWnd, 
		                 const CSize &sizeDefault, UINT nID, 
						 BOOL  bHasGripper = FALSE,
						 DWORD dwStyle     = WS_CHILD | WS_VISIBLE | CBRS_TOP );

    virtual BOOL Create( LPCTSTR lpszWindowName, CWnd* pParentWnd, UINT nID, 
		                 DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP);

public:
    const BOOL  IsFloating()     const;
    const BOOL  IsHorzDocked()   const;
    const BOOL  IsVertDocked()   const;
    const BOOL  IsSideTracking() const;
    const DWORD GetSCBStyle()    const { return m_dwSCBStyle; }
    
    void SetSCBStyle(DWORD dwSCBStyle) { m_dwSCBStyle = (dwSCBStyle & ~SCBS_EDGEALL); }
	void SetDragShowContent( BOOL bShow ) { m_bDragShowContent = bShow; }

	void SetStretchCursor( HCURSOR hHorz, HCURSOR hVert )
	{	m_hHorzCursor = hHorz; m_hVertCursor = hVert; }
// Overrides
public:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSizingControlBar)
    virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
    virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
    //}}AFX_VIRTUAL

public:
	CSizingControlBar();
    virtual ~CSizingControlBar();
    
protected:
	// CSCBDockBar dummy class for access to protected members
	class CSCBDockBar : public CDockBar
	{
		friend class CSizingControlBar;
	};

    // implementation helpers

	BOOL GetEdgeRect(const CRect &rcWnd, UINT nHitTest, CRect& rcEdge);
    UINT GetEdgeHTCode(int nEdge);

	void GetRowInfo(int& nFirst, int& nLast, int& nThis);

    void GetRowSizingBars(CSCBArray& arrSCBars);
    void GetRowSizingBars(CSCBArray& arrSCBars, int& nThis);

    BOOL NegotiateSpace(int nLengthTotal, BOOL bHorz);

	void AlignControlBars();

    void StartTracking(UINT nHitTest, const CPoint &pt);
    void StopTracking();

    void OnTrackUpdateSize(CPoint& point);
    void OnInvertTracker();

	virtual void NcPaintGripper(CDC* pDC, LPCRECT pRect);
    virtual void NcCalcClient(LPRECT pRect, UINT nDockBarID);   
  
// Attributes
protected:
	BOOL    m_bTracking;
    BOOL    m_bKeepSize;
    BOOL    m_bParentSizing;
    BOOL    m_bDragShowContent;

	int     m_nTrackPosMin;
    int     m_nTrackPosMax;
    int     m_nTrackPosOld;
    int     m_nTrackEdgeOffset;
	
	UINT    m_nDockBarID;
    UINT    m_htEdge;

	DWORD   m_dwSCBStyle;

    CSize   m_sizeHorz;
    CSize   m_sizeVert;
    CSize   m_sizeFloat;
    CSize   m_sizeMinHorz;
    CSize   m_sizeMinVert;
    CSize   m_sizeMinFloat;
  
	HCURSOR m_hHorzCursor;
	HCURSOR m_hVertCursor;
	CWnd  * m_pActiveWnd;

	const int m_nMargin;
// Generated message map functions
protected:
    //{{AFX_MSG(CSizingControlBar)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnNcPaint();
    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
    afx_msg UINT OnNcHitTest(CPoint point);
    afx_msg void OnCaptureChanged(CWnd *pWnd);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
    afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnClose();
    //}}AFX_MSG
   
    DECLARE_MESSAGE_MAP()
};

#endif // !defined(__SIZINGCONTROLBAR_H__)

