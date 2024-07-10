#if !defined(AFX_COOLSTATIC_H__10CB3BD0_61A6_4B51_A5D7_04DEF531CC68__INCLUDED_)
#define AFX_COOLSTATIC_H__10CB3BD0_61A6_4B51_A5D7_04DEF531CC68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CoolStatic.h : header file
//
#include "dal_display.h"
#ifndef MMI_MAX_LAYER_NUM
#define MMI_MAX_LAYER_NUM   8
#endif

typedef struct INNER_BMP_INFO_T
{
	HBITMAP hBmp;
	LPBYTE  pBuf;
} INNER_BMP_INFO;

#define X_GDI

/////////////////////////////////////////////////////////////////////////////
// CCoolStatic window

class CCoolStatic : public CStatic
{
// Construction
public:
	CCoolStatic();

// Attributes
public:
	CBitmap			m_bmpBackground;

	int				m_iDocHeight;
	int				m_iDocWidth;
	int				m_iLineHeight;
	int             m_iLineWidth;
	BOOL			m_bScrollBarMessage;

	BOOL            m_bDrawBorder;
	BOOL            m_bDrawRect;
	BOOL            m_bDrawLayerInfo;

// Operations
public:
	BOOL Init(DISPLAY_BLEND_LAYER_T *pLayerInfo, DWORD dwLcdHeight, DWORD dwLcdWidth);
	void SetBackgroundBitmap( BOOL bInvalidate  = FALSE  );
	void ResetScrollBar();
	void LayerInvalidate();
	void LayerInvalidateRect(WORD left,WORD top,WORD right,WORD bottom);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolStatic)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCoolStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCoolStatic)
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	int   GetUsedLayerNum();
	void  CalcCanvasSize();
	int   GetMaxSide();
#ifdef X_GDI
	void  DrawLayers(CDC *pDC ,int x, int y, CRect rect);
	void  DrawLayerInfo(CDC *pDC, int x, int y, int nLayerIndex);
#else 
	void  DrawLayers(Graphics &g ,int x, int y, CRect rect);
#endif
	void  ClearBmpInfo();

	CString GetDataTypeString(int type);

private:
	DISPLAY_BLEND_LAYER_T * m_pLayerInfo;
	INNER_BMP_INFO          m_bmpInfo[MMI_MAX_LAYER_NUM];

	DWORD m_dwLCDWidth;
	DWORD m_dwLCDHeight;

	HBITMAP                 m_hCanvas;
	BOOL                    m_bDraw;
	CRect                   m_rcInvalidate;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLSTATIC_H__10CB3BD0_61A6_4B51_A5D7_04DEF531CC68__INCLUDED_)
