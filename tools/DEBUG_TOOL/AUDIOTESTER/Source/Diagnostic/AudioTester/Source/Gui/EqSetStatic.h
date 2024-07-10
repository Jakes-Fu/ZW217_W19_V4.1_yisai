#if !defined(AFX_EQSETSTATIC_H__FE9B227E_21B0_4B49_B946_95C14671BFD4__INCLUDED_)
#define AFX_EQSETSTATIC_H__FE9B227E_21B0_4B49_B946_95C14671BFD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EqSetStatic.h : header file
//
#include <vector>

#define LEFT_EDGE_WITH 30
#define RIGHT_EDGE_WITH 10
#define BOTTOM_EDGE_WITH 35
#define TOP_EDGE_WITH 10
#define X_SCALE_MODE_COUNT 14
/////////////////////////////////////////////////////////////////////////////
// CEqSetStatic window

class CEqSetStatic : public CStatic
{
// Construction
public:
	CEqSetStatic();

public:
	int m_iYMax;
    int m_iYMin;
	
	// Operations
public:
	void EraseBK();
	void SetValue(int nIndex, int nChk, short nCf, float fWd, float fRt, float fGn);
	void SetBandNum(int nBandNum){m_nBandNum = nBandNum;}
	void SetMasterGain(float fMasterGain){m_fMasterGain = fMasterGain;}
	void Clear();
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
	
    std::vector<FreqItem>m_vAllBandFreqItem[8];
	std::vector<FreqItem>m_vAllFreqItem;
    
    // x axis value
    static double m_dXScale[X_SCALE_MODE_COUNT];
	
	int m_nBandNum;
	int m_nChk[8];
	short m_nCf[8];
	float m_fWd[8];
	float m_fRt[8];
	float m_fGn[8];
	float m_fMasterGain;

	COLORREF m_cr[9];
	CPen m_Pen[9];
	int m_iXScale[X_SCALE_MODE_COUNT];

	int m_nSampleRate;
	
public:
	// int* m_pSampleRate;
	
private:
	// draw x,y axis and its scale
	// draw frequence curve
	void DrawFreqCurves(CDC* pDC);
	
    void DrawFreqCurve( CDC* pDC,CPen* pPen,const std::vector<FreqItem>& vAllFreqItems );
	void DrawKeyPoint(CDC* pDC);
	void DrawCoordinateAxis(CDC* pDC);
	void CalculateFreqGraphyData( int nXScaleCount = 882 );
	void SetColorPen();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEqSetStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEqSetStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEqSetStatic)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EQSETSTATIC_H__FE9B227E_21B0_4B49_B946_95C14671BFD4__INCLUDED_)
