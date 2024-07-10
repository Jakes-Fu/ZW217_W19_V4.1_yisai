#if !defined(AFX_COOLSTATIC_H__57937073_5957_4D1D_A9F4_7FC40CBC0000__INCLUDED_)
#define AFX_COOLSTATIC_H__57937073_5957_4D1D_A9F4_7FC40CBC0000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CoolStatic.h : header file
//
#include "Global.h"
/////////////////////////////////////////////////////////////////////////////
// CCoolStatic window

class CCoolStatic : public CStatic
{
// Construction
public:
	CCoolStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCoolStatic();
	BOOL SaveCharByPose(PBDF_CHAR_T pChar,int nX,int nY, int nPtValue);
	int  GetPtValue(LPBYTE lpbDotMatrix,int nX,int nY);
	BOOL MoveSelRegion(PBDF_CHAR_T pChar,int nSrcLUX,int nSrcLUY, 
		               int nWidth,int nHeight,int nDstLUX,int nDstLUY);

	void Set2Step(BOOL b2Step);
	
	BYTE GetBits(BYTE b,int nPos,int nNumBits);
	BOOL SetBits(BYTE *pb,int nPos,int nNumBits,BYTE value);

	// Generated message map functions
protected:
	void DrawGrid(CPaintDC *pDC, CRect &rect, int dMin, int nBaseline = -1);
	int  DrawGlyph(CPaintDC *pDC, CRect &rect, int dMin);
	void DrawSide(CPaintDC *pDC, CRect &rect,BOOL bDown = FALSE);
//	void DrawRegion(CPaintDC *pDC, CRect &rect);
	void AdjustDmin(int nDiff, int* dMin);
	void AdjustRect(int nDiff, CRect &rc);

	//{{AFX_MSG(CCoolStatic)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	_inline WORD  Conv16Endian(WORD wValue)
	{
		return MAKEWORD(HIBYTE(wValue), LOBYTE(wValue));
	}

public:
	UINT m_uUnicode;
	int m_nFBBX;
	int m_nFBBY;
	BOOL  m_bDrawGrid;
	UINT  m_uBkColor;
	UINT  m_uBkColor2;
	BOOL  m_bIsDown;
	PBDF_CHAR_T m_pChar;
	GLYPH_BODY_PTR m_pGlyph;
	BOOL  m_b2Step;
	BOOL m_bSlider;
	int  m_nDiff;
	BOOL m_bOnePixel;
	int  m_nMaxBearingY;
	
	BOOL m_bAlpha;

	BOOL m_bEdit;
	BOOL m_bNormalGlyf; // the id of glyph is not got from gsub.
private:
	int m_nRegionLUX;
	int m_nRegionLUY;
	int m_nRegionWidth;
	int m_nRegionHeight;
	BYTE * m_pRegion;
	BYTE * m_pRegionLeft;	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLSTATIC_H__57937073_5957_4D1D_A9F4_7FC40CBC0000__INCLUDED_)
