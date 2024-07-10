#if !defined(AFX_ALCSTATIC_H__5157936A_342D_4FDC_AB84_742941FFF64E__INCLUDED_)
#define AFX_ALCSTATIC_H__5157936A_342D_4FDC_AB84_742941FFF64E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ALCStatic.h : header file
//
#define LEFT_EDGE_WITH 40
#define RIGHT_EDGE_WITH 10
#define BOTTOM_EDGE_WITH 20
#define TOP_EDGE_WITH 10
#define SCALE_MODE_COUNT 4

typedef struct _ALC_CURVE_DATA
{
	int y1;
	int y2;
	int y3;
	int y4;
}ALC_CURVE_DATA;
/////////////////////////////////////////////////////////////////////////////
// CALCStatic window

class CALCStatic : public CStatic
{
// Construction
public:
	CALCStatic();

// Attributes
public:

// Operations
public:

private:
	// the logic origin
    CPoint m_ptOrg;
    
    //the logic length of x axis
    double m_iXAxisLength;
    
    //the logic length of y axis
    double m_iYAxisLength;
	
	// background DC
    CDC m_memDC;
	
    //old bit map object
    CBitmap* m_pOldBitmap;
	
    //background bitmap
    CBitmap* m_pBKBitmap;
	
	CPoint m_ptYMaxScalePoint;
	
	static double m_dMinXValue;
    static double m_dMaxXValue;

	ALC_CURVE_DATA m_data;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CALCStatic)
	//}}AFX_VIRTUAL

public:
	void SetData(ALC_CURVE_DATA data);

private:
	void DrawCoordinateAxis(CDC* pDC);
	void DrawCurve(CDC* pDC);

// Implementation
public:
	virtual ~CALCStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CALCStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALCSTATIC_H__5157936A_342D_4FDC_AB84_742941FFF64E__INCLUDED_)
