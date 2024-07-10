#if !defined(AFX_RECORDSTATIC_H__EF954F92_FF1F_42CE_9818_B8CCB4E25F12__INCLUDED_)
#define AFX_RECORDSTATIC_H__EF954F92_FF1F_42CE_9818_B8CCB4E25F12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecordStatic.h : header file
//
#define LEFT_EDGE_WITH 40
#define RIGHT_EDGE_WITH 10
#define BOTTOM_EDGE_WITH 20
#define TOP_EDGE_WITH 10
/////////////////////////////////////////////////////////////////////////////
// CRecordStatic window

class CRecordStatic : public CStatic
{
// Construction
public:
	CRecordStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRecordStatic();


private:
	void DrawCoordinateAxis(CDC* pDC);
	void DrawCurve(CDC* pDC);
	void DrawArrow(CDC* pDC, CPoint p1, CPoint p2, int nStep);

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

public:
	BOOL m_bCompressor;

	// Generated message map functions
protected:
	//{{AFX_MSG(CRecordStatic)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDSTATIC_H__EF954F92_FF1F_42CE_9818_B8CCB4E25F12__INCLUDED_)
