#if !defined(AFX_LCFSTATIC_H__64F92D5E_77A7_4491_9D65_5FBFF762FC4B__INCLUDED_)
#define AFX_LCFSTATIC_H__64F92D5E_77A7_4491_9D65_5FBFF762FC4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LcfStatic.h : header file
//
#include <vector>

#define LEFT_EDGE_WITH 30
#define RIGHT_EDGE_WITH 10
#define BOTTOM_EDGE_WITH 35
#define TOP_EDGE_WITH 10
#define X_SCALE_MODE_COUNT 14
/////////////////////////////////////////////////////////////////////////////
// CLcfStatic window

class CLcfStatic : public CStatic
{
// Construction
public:
	CLcfStatic();

public:
	int m_iYMax;
    int m_iYMin;
	
	// Operations
public:
	void EraseBK();
	void SetValue(SHORT nF1fp, SHORT nF2fp, float fF1g0, float fF1g1, float fF2g0, float fF2g1, int nLcfSw, int nFiltertype,int nFp);
	void SetSampleRate(int nSampleRate){m_nSampleRate = nSampleRate;}
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
    double m_dMaxXValue;
	
    typedef std::pair<double,double>FreqItem;

    std::vector<FreqItem>m_vAllLcfFreqItem;
    
    // x axis value
    static double m_dXScale[X_SCALE_MODE_COUNT];

	SHORT m_nF1fp;
	SHORT m_nF2fp;
	float m_fF1g0;
	float m_fF1g1;
	float m_fF2g0;
	float m_fF2g1;
	int m_nLcfSw;
	int m_nFilterType;
	int m_nFp;

	int m_nSampleRate;
	
public:
   // int* m_pSampleRate;
	
private:
	// draw x,y axis and its scale
	// draw frequence curve
	void DrawFreqCurves(CClientDC* pDC);
	
    void DrawFreqCurve( CClientDC* pDC,CPen* pPen,const std::vector<FreqItem>& vAllFreqItems );
	void DrawCoordinateAxis(CClientDC* pDC);
	void CalculateFreqGraphyData( int nXScaleCount = 4410);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLcfStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLcfStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLcfStatic)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LCFSTATIC_H__64F92D5E_77A7_4491_9D65_5FBFF762FC4B__INCLUDED_)
