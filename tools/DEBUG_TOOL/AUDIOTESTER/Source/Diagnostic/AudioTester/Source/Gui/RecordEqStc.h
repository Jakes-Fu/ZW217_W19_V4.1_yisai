#if !defined(AFX_RECORDEQSTC_H__00F5877E_F13A_4D66_A1A8_1B9882D7B6D3__INCLUDED_)
#define AFX_RECORDEQSTC_H__00F5877E_F13A_4D66_A1A8_1B9882D7B6D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecordEqStc.h : header file
//
#include <vector>

#define LEFT_EDGE_WITH 30
#define RIGHT_EDGE_WITH 10
#define BOTTOM_EDGE_WITH 35
#define TOP_EDGE_WITH 10
#define X_SCALE_MODE_COUNT 14
#define NUM_CURVE 6
/////////////////////////////////////////////////////////////////////////////
// CRecordEqStc window

class CRecordEqStc : public CStatic
{
// Construction
public:
	CRecordEqStc();

public:
	int m_iYMax;
    int m_iYMin;
	
	// Operations
public:
	void EraseBK();
	void SetValue(int nIndex, int nChkSw, int nFO, int nDF, float fBoost, float fGain);
	void Clear();
	void SetMasterGain(float fMasterGain){m_fMasterGain = fMasterGain;}
	
	
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
	
    std::vector<FreqItem>m_vAllBandFreqItem[8];
	std::vector<FreqItem>m_vAllFreqItem;
    
    // x axis value
    static double m_dXScale[X_SCALE_MODE_COUNT];
	
	int m_nChkSw[NUM_CURVE];
	int m_nFO[NUM_CURVE];
	int m_nDF[NUM_CURVE];
	float m_fBoost[NUM_CURVE];
	float m_fGain[NUM_CURVE];

	COLORREF m_cr[7];
	CPen m_Pen[7];
	int m_iXScale[X_SCALE_MODE_COUNT];

	float m_fMasterGain;
	
private:
	// draw x,y axis and its scale
	// draw frequence curve
	void DrawFreqCurves(CDC* pDC);
	
    void DrawFreqCurve( CDC* pDC,CPen* pPen,const std::vector<FreqItem>& vAllFreqItems );
	void DrawKeyPoint(CDC* pDC);
	void DrawCoordinateAxis(CDC* pDC);
	void CalculateFreqGraphyData( int nXScaleCount = 882 );
	void SetColorPen();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordEqStc)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRecordEqStc();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRecordEqStc)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDEQSTC_H__00F5877E_F13A_4D66_A1A8_1B9882D7B6D3__INCLUDED_)
