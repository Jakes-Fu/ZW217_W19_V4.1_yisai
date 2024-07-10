#if !defined(AFX_EQMODESTATIC_H__9B229D1D_F609_4B8C_B2CE_FE968E6BDB3E__INCLUDED_)
#define AFX_EQMODESTATIC_H__9B229D1D_F609_4B8C_B2CE_FE968E6BDB3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EqModeStatic.h : header file
//
#include "BMPCtrlWithSpecRect.h"
/////////////////////////////////////////////////////////////////////////////
// CEqModeStatic window

class CEqModeStatic : public CStatic
{
public:
	enum
	{
		Draw_Lcf,
		Draw_Bass,
		Draw_Treble,
		Draw_All,
		Draw_Lcf_L_R,
		Draw_All_L_R
	};

// Construction
public:
	CEqModeStatic();

// Attributes
public:
	int m_iYMax;
    int m_iYMin;

	int m_nType;
	int m_nSoundEqVer;

// Operations
public:
// 	void EraseBK();
	void SetData( EQ_MODE_NV_PARAM_T& EqData );

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

	EQ_MODE_NV_PARAM_T m_Data;

	typedef std::pair<double,double>FreqItem;
    //all frequence curve x value and y value
    std::vector<FreqItem>m_vAllFreqItem;
    std::vector<FreqItem>m_vAllBassFreqItem;
    std::vector<FreqItem>m_vAllTrebleFreqItem;
    std::vector<FreqItem>m_vAllLcfFreqItem;

	std::vector<FreqItem>m_vAllRFreqItem;
    std::vector<FreqItem>m_vAllLcfRFreqItem;


	double m_dMinXValue;
	double m_dMaxXValue;
	double m_dXScale[X_SCALE_MODE_COUNT];

	BOOL m_bModified;

private:
	// draw x,y axis and its scale
	// draw frequence curve
	void DrawFreqCurves(CDC* pDC);

    void DrawFreqCurve( CDC* pDC,CPen* pPen,const std::vector<SpecialRectOfEQMode::FreqItem>& vAllFreqItems );
	void DrawCoordinateAxis(CDC* pDC);
	
	void CalculateFreqGraphyData( int nXScaleCount = 22050 );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEqModeStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEqModeStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEqModeStatic)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EQMODESTATIC_H__9B229D1D_F609_4B8C_B2CE_FE968E6BDB3E__INCLUDED_)
