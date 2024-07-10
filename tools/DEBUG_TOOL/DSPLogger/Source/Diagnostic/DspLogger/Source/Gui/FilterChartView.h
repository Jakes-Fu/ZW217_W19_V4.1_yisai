#if !defined(AFX_FILTERCHARTVIEW_H__F9152646_B76D_48A0_989C_EEBE4E2D9255__INCLUDED_)
#define AFX_FILTERCHARTVIEW_H__F9152646_B76D_48A0_989C_EEBE4E2D9255__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterChartView.h : header file
//

#include "graphwnd.h"
#include "DspKernelExport.h"
#include "MathFormula.h"
#include "Filter.h"

typedef vector<CMathFormula*> VEC_MATHFML;
typedef vector<CFilter*> VEC_FILTER;
typedef vector<DWORD>VEC_DWORD;
   
/////////////////////////////////////////////////////////////////////////////
// CFilterChartView view
class CFilter;
class CFilterDoc;
class CFilterChartView : public CView
{
protected:
	CFilterChartView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFilterChartView)

public:
    CFilterDoc* m_pDoc;
    ITPSet*     m_pTPSet;
    CGraphWnd   m_graph_wnd;

   
public:
   void InitGraph();    
   int  CreateGraph(TP_FILTER_DATA* pFilter_Data, CMathFormula* pMathFml);
   BOOL RemoveGraph(WORD index);  
   BOOL RemoveAllGraph();

   void ResetGraph(int graph_index);
   void StartFilter();
   void StopFilter();

public:
    DOUBLE      m_screen_x;
    DOUBLE      m_screen_y;
    DOUBLE      m_expand_range;

    BOOL        m_bAutoScroll;    

    BOOL        m_bUnsiged;
    BOOL        m_bByTime;

private:
   BOOL CreateGraphThread();
   void ThreadProc();
   static DWORD WINAPI DrawThread(LPVOID pParam );

   BOOL DataDeal(int math_index, int graph_index ,double& dbData);
   
   void graph_AddPoint(int graph_index,double x, double y ,ULONG idx);
       
public:   
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFilterChartView)
    public:
    virtual void OnInitialUpdate();
    protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFilterChartView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CFilterChartView)
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
        
private:    
    HANDLE      m_hThreadProc; 

    VEC_MATHFML m_vecMathFml;
    VEC_FILTER  m_vecFilter;
    VEC_DWORD   m_vecGraphIdx;

    BOOL        m_bStart;

};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERCHARTVIEW_H__F9152646_B76D_48A0_989C_EEBE4E2D9255__INCLUDED_)
