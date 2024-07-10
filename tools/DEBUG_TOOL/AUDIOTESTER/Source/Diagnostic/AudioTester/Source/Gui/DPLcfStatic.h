#if !defined(AFX_DPLCFSTATIC_H__DB0C6C9E_CFA7_4DF8_8921_BB11646B7672__INCLUDED_)
#define AFX_DPLCFSTATIC_H__DB0C6C9E_CFA7_4DF8_8921_BB11646B7672__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DPLcfStatic.h : header file
//
#include <vector>
#define LEFT_EDGE_WITH 30
#define RIGHT_EDGE_WITH 10
#define BOTTOM_EDGE_WITH 35
#define TOP_EDGE_WITH 10
#define X_SCALE_MODE_COUNT 14
/////////////////////////////////////////////////////////////////////////////
// CDPLcfStatic window

class CDPLcfStatic : public CStatic
{
// Construction
public:
	CDPLcfStatic();

public:
	int m_iYMax;
    int m_iYMin;
	
	// Operations
public:
	void EraseBK();
	void SetValue(int nLcfSw, int nFp_l, int nFp_r, int nGain_l, int nGain_r);
	
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
	
    typedef std::pair<double,double>FreqItem;
	
    std::vector<FreqItem>m_vAllLcfFreqItem;
    
    // x axis value
    static double m_dXScale[X_SCALE_MODE_COUNT];
	
	int m_nLcfSw;
	int m_nFilterType;
	int m_nFp_l;
	int m_nFp_r;
	int m_nGain_l;
	int m_nGain_r;
	
public:
	// int* m_pSampleRate;
	
private:
	// draw x,y axis and its scale
	// draw frequence curve
	void DrawFreqCurves(CClientDC* pDC);
	
    void DrawFreqCurve( CClientDC* pDC,CPen* pPen,const std::vector<FreqItem>& vAllFreqItems );
	void DrawCoordinateAxis(CClientDC* pDC);
	void CalculateFreqGraphyData(int nGain, int nFp, int nXScaleCount = 4410);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDPLcfStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDPLcfStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDPLcfStatic)
	afx_msg void OnNcRButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DPLCFSTATIC_H__DB0C6C9E_CFA7_4DF8_8921_BB11646B7672__INCLUDED_)
