#if !defined(AFX_CHARTVIEW_H__406E19BB_C949_4B13_AB50_78CACBE9A893__INCLUDED_)
#define AFX_CHARTVIEW_H__406E19BB_C949_4B13_AB50_78CACBE9A893__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChartView.h : header file
//
#pragma warning(push,3)
#include <vector>
#pragma warning(pop)
#pragma warning( disable : 4018)
#include "DspKernelExport.h"

#define MIN_CX 320
#define MIN_CY 240 //1090 for 8 subChart
typedef std::vector<DWORD> VEC_FIND;
#define Y_AXIS_DEFAULT_OFFSET	3
/////////////////////////////////////////////////////////////////////////////
// CFindChartView view
class CFindDoc;
class CFindChartView : public CScrollView
{
protected:
	CFindChartView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFindChartView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindChartView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void GetTPandTime( int nNum, VEC_FIND& arrTp, VEC_FIND& arrTime );
	void OnGotoChartPoint( ULONG lIdx );
    void GotoFindItem( LPPOINT pPt );

	void DrawCross( CDC * pDC );
    void ClearFindInfo(void);
    
	void FindItems(void);
    void FindUpdate(void);

    void InitForDraw(int nCurveNum);
	void CalculateYOrigin(void);

	// draw operations @andy.bian 06.8.25
	void DrawCoordinate( CDC * pDC );
    void DrawWave( CDC * pDC );
	
	// draw graph together
	void DrawCoordinate_Together( CDC * pDC );
    void DrawWave_Together( CDC * pDC );
	void DrawCross_Together( CDC * pDC );

	void FindMaxMin();
    void GetMaxMinData( int nNum, WORD &wMax, WORD &wMin,VEC_FIND& vecTp );
    void GetMaxMinTime( int nNum, DWORD &dwMax, DWORD &dwMin ,VEC_FIND& vecTime);

    void OnUpdateChartView( int nAct, LPARAM lParam, int nIdx );

    __inline UINT PixelToIndex( int nFind, long &lPixel );
    __inline long IndexToPixel( int nFind, UINT nIndex );
    __inline int  GetYPixelByIdx( int nFind, UINT nIdx );
	//find nearest point on y asix @andy.bian 2006-9-4 
	__inline UINT GetNearestYIdx(int nFind,UINT nIdx,long &lPixel);	
	__inline void MoveTo(CDC *pDC,int x, int y)
	{
		pDC->MoveTo(x,y);
		
		if(m_bDrawTestPoint)
		{
			pDC->Draw3dRect(x,y,2,2, DEFAULT_POINT_CLR, DEFAULT_POINT_CLR );
		}
	}
   void SaveCurTP();

	virtual ~CFindChartView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	// Generated message map functions
	//{{AFX_MSG(CFindChartView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    CFindDoc * m_pDoc;
    ITPSet   * m_pSet;

	BOOL  m_bDrawTestPoint;
	BOOL  m_bUCAChange;
	int  m_nCurCross;	//current cross		
    UINT  m_nCross[MAX_FIND_DATA_NUM];//select point in subchart
	
	// draw graph together
	BOOL  m_bDrawTogether; 
	UINT  m_nCurChartTogether;
	
	//draw chart use same y 
	BOOL  m_bUseSameY;

	WORD  m_wMaxData ;
	WORD  m_wMinData ;
    DWORD m_dwMaxTime;
	DWORD m_dwMinTime;
	UINT  m_nMaxTotalTP;
	DWORD m_MaxMinData[MAX_FIND_DATA_NUM];

    
    
    int      m_nFindNum; 
    VEC_FIND m_arrIndex [MAX_FIND_DATA_NUM];

    BOOL m_arrSigned [MAX_FIND_DATA_NUM];
    BOOL m_arrByIndex[MAX_FIND_DATA_NUM];


    int    m_nXStart;
    int    m_nYStart;
    DWORD  m_dwDefaultX;
    DWORD  m_dwDefaultY;
    DWORD  m_dwX;
    DWORD  m_dwY;
    
    DWORD  m_arrYInitBndr[MAX_FIND_DATA_NUM];
    DWORD  m_arrYBoundary[MAX_FIND_DATA_NUM];

    int    m_arrYOrig[MAX_FIND_DATA_NUM];
    int    m_arrYScale[MAX_FIND_DATA_NUM];
    int    m_nXScale;
	
	ULONG  m_uLastTPCount;

    static const DWORD DEFAULT_BAK_CLR;
	static const DWORD DEFAULT_POINT_CLR;//andy.bian add 06.8.29
	static const DWORD DEFAULT_AXIS_CLR;
	static const DWORD DEFAULT_FONT_CLR;

    static const int AREA_INTERVAL;
    static const int X_SCALE_UNIT;
    static const int Y_SCALE_UNIT;
    static const int MARGIN;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTVIEW_H__406E19BB_C949_4B13_AB50_78CACBE9A893__INCLUDED_)
