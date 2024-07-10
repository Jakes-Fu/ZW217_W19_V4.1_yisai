#if !defined(AFX_EQGRAPHSTATIC_H__0BED40FD_E312_4DE3_AE56_743F10F959C4__INCLUDED_)
#define AFX_EQGRAPHSTATIC_H__0BED40FD_E312_4DE3_AE56_743F10F959C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EqGraphStatic.h : header file
//
#pragma  warning(push,3)
#include <vector>
#pragma  warning(pop)
#include "BMPCtrlWithSpecRect.h"

/////////////////////////////////////////////////////////////////////////////
// CEqGraphStatic window

class SpecialRectOfEQ;

class CEqGraphStatic : public CStatic
{
// Construction
public:
	CEqGraphStatic();
    
    int m_iYMax;
    int m_iYMin;
    
    // the data source from which we draw the curve
    SpecialRectOfEQ* m_pDataSource;
    
    //redraw the background
    void EraseBK();
    // Attributes
private:
    // the logic origin
    CPoint m_ptOrg;
    
    //the logic length of x axis
    double m_iXAxisLength;
    
    //the logic length of y axis
    double m_iYAxisLength;
    
    CPoint m_ptYMaxScalePoint;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEqGraphStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEqGraphStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEqGraphStatic)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	
private:
    // draw frequence curve
	void DrawFreqCurves(CClientDC* pDC);

    void DrawFreqCurve( CClientDC* pDC,CPen* pPen,const std::vector<SpecialRectOfEQ::FreqItem>& vData );

    // draw x,y axis and its scale
	void DrawCoordinateAxis(CClientDC* pDC);

    // background DC
    CDC m_memDC;

    //old bit map object
    CBitmap* m_pOldBitmap;

    //background bitmap
    CBitmap* m_pBKBitmap;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EQGRAPHSTATIC_H__0BED40FD_E312_4DE3_AE56_743F10F959C4__INCLUDED_)
