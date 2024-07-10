#if !defined(AFX_FREQUENCYGRAPHYSTATIC_H__BA456038_0800_4329_903B_95DC1F92C938__INCLUDED_)
#define AFX_FREQUENCYGRAPHYSTATIC_H__BA456038_0800_4329_903B_95DC1F92C938__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrequencyGraphyStatic.h : header file
//
#pragma  warning(push,3)
#include <vector>
#pragma  warning(pop)


#include "BMPCtrlWithSpecRect.h"
/////////////////////////////////////////////////////////////////////////////
// CFrequencyGraphyStatic window
class SpecialRectOfCoefficient;
class CFrequencyGraphyStatic : public CStatic
{
// Construction
public:
	CFrequencyGraphyStatic();

    //zoom in and zoom out ,not support now
    double m_iXCoefficient;
    double m_iYCoefficient;
    
    int m_iYMax;
    int m_iYMin;

    // the data source from which we draw the curve
    SpecialRectOfCoefficient* m_pDataSource;
    //redraw the background
    void EraseBK();

protected:
	CPoint ConvertCurvePtToPt(double x, double y);

// Attributes
protected:
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
	//{{AFX_VIRTUAL(CFrequencyGraphyStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFrequencyGraphyStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFrequencyGraphyStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
protected:
    // draw frequence curve
	void DrawFreqCurve(CClientDC* pDC);

    // draw x,y axis and its scale
	void DrawCoordinateAxis(CDC* pDC);

    // background DC
    CDC m_memDC;

    //old bit map object
    CBitmap* m_pOldBitmap;

    //background bitmap
    CBitmap* m_pBKBitmap;
};

class SpecialRectOfCoefficientEx;
class CFrequencyGraphyStaticX : public CStatic
{
	// Construction
public:
	CFrequencyGraphyStaticX();
	
    //zoom in and zoom out ,not support now
    double m_iXCoefficient;
    double m_iYCoefficient;
    
    int m_iYMax;
    int m_iYMin;
	
    // the data source from which we draw the curve
    SpecialRectOfCoefficientEx* m_pDataSource;
    //redraw the background
    void EraseBK();
	
protected:
	CPoint ConvertCurvePtToPt(double x, double y);
	
	// Attributes
protected:
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
	//{{AFX_VIRTUAL(CFrequencyGraphyStatic)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CFrequencyGraphyStaticX();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CFrequencyGraphyStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
protected:
    // draw frequence curve
	void DrawFreqCurve(CClientDC* pDC);
	
    // draw x,y axis and its scale
	void DrawCoordinateAxis(CDC* pDC);
	
    // background DC
    CDC m_memDC;
	
    //old bit map object
    CBitmap* m_pOldBitmap;
	
    //background bitmap
    CBitmap* m_pBKBitmap;
};

class CFirAutoDlg;

class CFrequencyGraphyStaticEx : public CStatic
{
public:
    typedef std::pair<double,double>FreqItem;
    
    // Construction
public:
    CFrequencyGraphyStaticEx();

    static const double PI ;
    static const double m_dMinXValue;
    static const double m_dMaxXValue;
    // x axis value
#define X_SCALE_COUNT_FIR 26
    static int m_iXScale[X_SCALE_COUNT_FIR];
    
    
    //zoom in and zoom out ,not support now
    double m_iXCoefficient;
    double m_iYCoefficient;
    
    int m_iYMax;
    int m_iYMin;
    
    //redraw the background
    void EraseBK();

public:
    CFirAutoDlg* m_pDataSource;
	
	//teana hu 2011.02.17
	int m_nOriYMax1;
	int m_nOriYMax2;
    
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
	//teana hu 2011.02.17
	CPoint GetOriMaxY();
	void SetYMinMaxValue(int nYMin, int nYMax){m_iYMin = nYMin; m_iYMax = nYMax;}
    
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFrequencyGraphyStatic)
    //}}AFX_VIRTUAL
    
    // Implementation
public:
    virtual ~CFrequencyGraphyStaticEx();
    
    // Generated message map functions
protected:
    //{{AFX_MSG(CFrequencyGraphyStatic)
    afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    //}}AFX_MSG
    
    DECLARE_MESSAGE_MAP()
private:
    // draw frequence curve
    
    void DrawFreqCurve(CClientDC* pDC,CPen&,std::vector<FreqItem>&);
    
    void DrawOriFreqCurve( CClientDC* pDC );
    void DrawTarFreqCurve( CClientDC* pDC );
    void DrawRealFreqCurve( CClientDC* pDC );
    void DrawModifiedFreqCurve( CClientDC* pDC );
    void DrawHighFreqCurve( CClientDC* pDC );
    void DrawLowFreqCurve( CClientDC* pDC );
    void DrawFinalFreqCurve( CClientDC* pDC );

    // draw x,y axis and its scale
    void DrawCoordinateAxis(CClientDC* pDC);

	//teana hu 2009.09.04
	void DrawMoveTarPoint(CClientDC* pDC );
	CPoint ConvertCurvePtToPt(double x, double y);
    
    // background DC
    CDC m_memDC;
    
    //old bit map object
    CBitmap* m_pOldBitmap;
    
    //background bitmap
    CBitmap* m_pBKBitmap;

	//teana hu 2009.09.04
	CPoint m_tarPt;
	int m_nTarIndex;

public:
	std::vector<FreqItem> m_vTarFreqItems;
	std::vector<FreqItem> m_vTarMovePt;

	BOOL m_bCrossCursor;
};

class CFrequencyGraphyStaticEx2 : public CFrequencyGraphyStatic
{
public:
	CFrequencyGraphyStaticEx2();

	//{{AFX_MSG(CFrequencyGraphyStatic)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()

public:
	void SetYMinMaxValue(int nYMin, int nYMax){m_iYMin = nYMin; m_iYMax = nYMax;}

protected:
	void DrawFreqLine(CDC* pDC);

public:
	int m_nSel;

public:
	double m_arrPowerData[X_SCALE_COUNT_FIR];
	CWnd* m_pRelateWnd;
};

class CFirAutoDlgEx;
class CFrequencyGraphyStaticExX : public CStatic
{
public:
    typedef std::pair<double,double>FreqItem;
    
    // Construction
public:
    CFrequencyGraphyStaticExX();

    static const double PI ;
    static const double m_dMinXValue;
    static const double m_dMaxXValue;
    // x axis value
#define X_SCALE_COUNT_FIR_EX 65
    static int m_iXScale[X_SCALE_COUNT_FIR_EX];
    
    
    //zoom in and zoom out ,not support now
    double m_iXCoefficient;
    double m_iYCoefficient;
    
    int m_iYMax;
    int m_iYMin;
    
    //redraw the background
    void EraseBK();

public:
    CFirAutoDlgEx* m_pDataSource;
	
	//teana hu 2011.02.17
	int m_nOriYMax1;
	int m_nOriYMax2;
    
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
	//teana hu 2011.02.17
	CPoint GetOriMaxY();
	void SetYMinMaxValue(int nYMin, int nYMax){m_iYMin = nYMin; m_iYMax = nYMax;}
    
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFrequencyGraphyStatic)
    //}}AFX_VIRTUAL
    
    // Implementation
public:
    virtual ~CFrequencyGraphyStaticExX();
    
    // Generated message map functions
protected:
    //{{AFX_MSG(CFrequencyGraphyStatic)
    afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    //}}AFX_MSG
    
    DECLARE_MESSAGE_MAP()
private:
    // draw frequence curve
    
    void DrawFreqCurve(CClientDC* pDC,CPen&,std::vector<FreqItem>&);
    
    void DrawOriFreqCurve( CClientDC* pDC );
    void DrawTarFreqCurve( CClientDC* pDC );
    void DrawRealFreqCurve( CClientDC* pDC );
    void DrawModifiedFreqCurve( CClientDC* pDC );
    void DrawHighFreqCurve( CClientDC* pDC );
    void DrawLowFreqCurve( CClientDC* pDC );
    void DrawFinalFreqCurve( CClientDC* pDC );

    // draw x,y axis and its scale
    void DrawCoordinateAxis(CClientDC* pDC);

	//teana hu 2009.09.04
	void DrawMoveTarPoint(CClientDC* pDC );
	CPoint ConvertCurvePtToPt(double x, double y);
    
    // background DC
    CDC m_memDC;
    
    //old bit map object
    CBitmap* m_pOldBitmap;
    
    //background bitmap
    CBitmap* m_pBKBitmap;

	//teana hu 2009.09.04
	CPoint m_tarPt;
	int m_nTarIndex;

public:
	std::vector<FreqItem> m_vTarFreqItems;
	std::vector<FreqItem> m_vTarMovePt;

	BOOL m_bCrossCursor;
};

class CFrequencyGraphyStaticEx3 : public CFrequencyGraphyStaticX
{
public:
	CFrequencyGraphyStaticEx3();
	
	//{{AFX_MSG(CFrequencyGraphyStaticEx3)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
		
public:
	void SetYMinMaxValue(int nYMin, int nYMax){m_iYMin = nYMin; m_iYMax = nYMax;}
	
protected:
	void DrawFreqLine(CDC* pDC);
	
public:
	int m_nSel;
	
public:
	double m_arrPowerData[X_SCALE_COUNT_FIR_EX];
	CWnd* m_pRelateWnd;

	static int m_iXScale[X_SCALE_COUNT_FIR_EX];
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FREQUENCYGRAPHYSTATIC_H__BA456038_0800_4329_903B_95DC1F92C938__INCLUDED_)
