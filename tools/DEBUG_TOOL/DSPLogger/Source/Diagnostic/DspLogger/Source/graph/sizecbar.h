/////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998, 1999 by Cristi Posea
// All rights reserved
//
// Use and distribute freely, except: don't remove my name from the
// source or documentation (don't take credit for my work), mark your
// changes (don't get me blamed for your possible bugs), don't alter
// or remove this notice.
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// This class is intended to be used as a base class. Do not simply add
// your code to this file - instead create a new class derived from
// CSizingBar and put there what you need.
// Modify this file only to fix bugs, and don't forget to send me a copy.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc.,
// and I'll try to keep a version up to date.  I can be reached at:
//    cristip@dundas.com
//
// More details at MFC Programmer's SourceBook
// http://www.codeguru.com/docking/docking_window.shtml or search
// www.codeguru.com for my name if the article was moved.
//
/////////////////////////////////////////////////////////////////////////

#if !defined(__SIZECBAR_H__)
#define __SIZECBAR_H__

#include <afxpriv.h>    // for CDockContext
#include <afxtempl.h>   // for CArray

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////
// CSCBBtn (button info) helper class

class CSCBBtn
{
public:
    CSCBBtn();

    void Move(CPoint ptTo) {ptOrg = ptTo; };
    CRect GetRect() { return CRect(ptOrg, CSize(11, 11)); };
    void Paint(CDC* pDC);

    BOOL    bPushed;
    BOOL    bRaised;

protected:
    CPoint  ptOrg;
};

/////////////////////////////////////////////////////////////////////////
// CSCBDockBar dummy class for access to protected members

class CSCBDockBar : public CDockBar
{
    friend class CSizingBar;
};

/////////////////////////////////////////////////////////////////////////
// CSizingBar control bar styles

#define SCBS_EDGELEFT       0x00000001
#define SCBS_EDGERIGHT      0x00000002
#define SCBS_EDGETOP        0x00000004
#define SCBS_EDGEBOTTOM     0x00000008
#define SCBS_EDGEALL        0x0000000F
#define SCBS_SHOWEDGES      0x00000010
#define SCBS_GRIPPER        0x00000020

/////////////////////////////////////////////////////////////////////////
// CSizingBar control bar

#ifndef baseCSizingBar
#define baseCSizingBar CControlBar
#endif

class CSizingBar;
typedef CTypedPtrArray <CPtrArray, CSizingBar*> CSCBArr;

class CSizingBar : public baseCSizingBar
{
    DECLARE_DYNAMIC(CSizingBar);

// Construction
protected:
    CSizingBar();

public:
    virtual BOOL Create(LPCTSTR lpszWindowName, CWnd* pParentWnd,
        CSize sizeDefault, BOOL bHasGripper, UINT nID,
        DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP);

// Attributes
public:
    CSize m_szHorz;
    CSize m_szVert;
    CSize m_szFloat;
    const BOOL IsFloating() const;
    const BOOL IsHorzDocked() const;
    const BOOL IsVertDocked() const;
    const BOOL IsSideTracking() const;

// Operations
public:
    virtual void LoadState(LPCTSTR lpszProfileName);
    virtual void SaveState(LPCTSTR lpszProfileName);
    static void GlobalLoadState(LPCTSTR lpszProfileName);
    static void GlobalSaveState(LPCTSTR lpszProfileName);

// Overridables
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Overrides
public:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSizingBar)
    public:
    virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
    virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);
    virtual BOOL DestroyWindow();
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CSizingBar();
    
protected:
    // implementation helpers
    UINT GetEdgeHTCode(int nEdge);
    BOOL GetEdgeRect(CRect rcWnd, UINT nHitTest, CRect& rcEdge);
    virtual void StartTracking(UINT nHitTest);
    virtual void StopTracking();
    virtual void OnTrackUpdateSize(CPoint& point);
    virtual void OnTrackInvertTracker();
    virtual void NcPaintGripper(CDC* pDC, CRect rcClient);

    virtual void AlignControlBars();
    const int FindSizingBar(CControlBar* pBar) const;
    void GetRowInfo(int& nFirst, int& nLast, int& nThis);
    void GetRowSizingBars(CSCBArr& arrSCBars);
    BOOL NegociateSpace(int nLengthAvail, BOOL bHorz);

protected:
    static CSCBArr    m_arrBars;

    DWORD   m_dwSCBStyle;
    UINT    m_htEdge;

    CSize   m_szMin;
    CSize   m_szMinT;
    CSize   m_szMaxT;
    CSize   m_szOld;
    CPoint  m_ptOld;
    BOOL    m_bTracking;
    BOOL    m_bKeepSize;
    BOOL    m_bParentSizing;
    BOOL    m_bDragShowContent;
    UINT    m_nDockBarID;
    int     m_cxEdge;
    int     m_cyGripper;

    CSCBBtn m_biHide;

// Generated message map functions
protected:
    //{{AFX_MSG(CSizingBar)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnNcPaint();
    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
    afx_msg UINT OnNcHitTest(CPoint point);
    afx_msg void OnCaptureChanged(CWnd *pWnd);
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
    afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
    afx_msg void OnPaint();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

#endif // !defined(__SIZECBAR_H__)

