// FilterChartView.cpp : implementation file
//

#include "stdafx.h"
#include "DspLogger.h"
#include "MainFrm.h"
#include "FilterChartView.h"
#include "FilterDoc.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_GRAPHS 4
#define DEF_SCREEN_X  0x10000
#define DEF_SCREEN_Y  0XFFFF
#define DEF_EXPAND_RANGE 0x1000


COLORREF DefaultGraphColors[MAX_GRAPHS] =
{
    RGB(240, 20, 20),
    RGB(24,  128, 24),
    RGB(10,  10, 240),
    RGB(20, 200, 200),
};

/////////////////////////////////////////////////////////////////////////////
// CFilterChartView

IMPLEMENT_DYNCREATE(CFilterChartView, CView)

CFilterChartView::CFilterChartView()
{   
    m_pDoc    = NULL;
    m_pTPSet  = NULL;

    m_screen_x    = DEF_SCREEN_X;
    m_screen_y    = DEF_SCREEN_Y;
    m_expand_range= DEF_EXPAND_RANGE; 

    m_bStart      =  FALSE;
    m_bAutoScroll =  FALSE;
    
    m_bUnsiged    =  TRUE;
    m_bByTime     =  TRUE;

    m_hThreadProc =  NULL;
}

CFilterChartView::~CFilterChartView()
{

    TerminateThread(m_hThreadProc, (ULONG)-1);   
    CloseHandle(m_hThreadProc);
    m_hThreadProc = NULL;   
    
    RemoveAllGraph();
}

BEGIN_MESSAGE_MAP(CFilterChartView, CView)
	//{{AFX_MSG_MAP(CFilterChartView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterChartView drawing

void CFilterChartView::OnDraw(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
//	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

}

/////////////////////////////////////////////////////////////////////////////
// CFilterChartView diagnostics

#ifdef _DEBUG
void CFilterChartView::AssertValid() const
{
	CView::AssertValid();
}

void CFilterChartView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFilterChartView message handlers

int CFilterChartView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;
    
    CRect rect;
    GetClientRect(rect);
    m_graph_wnd.Create(_T("Graph Window"), rect, this, 11000);	
    
    return 0;
}

void CFilterChartView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
    m_pDoc= (CFilterDoc*)GetDocument();
    m_pDoc->m_pChartView= this;

    CMainFrame* pMainFrame= (CMainFrame*)AfxGetMainWnd();
    m_pTPSet= pMainFrame->m_pDspKernel->GetTpSet();
    ASSERT(NULL != m_pTPSet);

    InitGraph();

    CreateGraphThread();
}


void CFilterChartView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);	
	m_graph_wnd.MoveWindow(0, 0, cx, cy);	
}

void CFilterChartView:: InitGraph()
{
 	
    m_graph_wnd.SetAxisProps(_T("X-Axis"), _T(""), 4, GRAPH_X_AXIS, TRUE);
	m_graph_wnd.SetAxisProps(_T("Y-Axis"), _T(""), 4, GRAPH_Y_AXIS, TRUE);
	m_graph_wnd.SetGraphWorldCoords(0, m_screen_x, 0, m_screen_y, TRUE);
    //m_graph_wnd.OperateWithPointView(GRAPH_PO_ENABLE);
    //m_graph_wnd.UpdateWindows(GRAPH_WUV_ALL);

}

int CFilterChartView:: CreateGraph(TP_FILTER_DATA* pFilter_Data, CMathFormula* pMathFml)
{   
    int nCount= m_graph_wnd.GetGraphCount();
    if (nCount >= MAX_GRAPHS)
        return -1;

    //create graph
    CString title;
    title.Format("0x%04X", pFilter_Data->wAddr);
    int graph_index = m_graph_wnd.AddGraph(DefaultGraphColors[nCount], title);
    
    //save graph index    
    m_vecGraphIdx.push_back(graph_index);  
    
    //create filter obj and start it
    CFilter* pFilter= new CFilter();
	if (pFilter)
	{
		m_vecFilter.push_back(pFilter); 
		
		pFilter->SetFilterContext(m_pTPSet);
		pFilter->StartFilter(pFilter_Data);
		if (!m_bStart)
		{
			pFilter->PauseFilter();
		}
		
		//math formula obj for each graph    
		m_vecMathFml.push_back(pMathFml);
	}
    
    
    return graph_index;
}

BOOL CFilterChartView::RemoveGraph(WORD index)
{
    m_graph_wnd.RemoveGraph(index);
        
    for (int i=0; i< m_vecGraphIdx.size(); i++)
    {
        if (m_vecGraphIdx[i] == index)
        {
            m_vecGraphIdx.erase(m_vecGraphIdx.begin()+i);

            delete m_vecMathFml[i];
            m_vecMathFml.erase(m_vecMathFml.begin()+i);
            
            delete m_vecFilter[i];
            m_vecFilter.erase(m_vecFilter.begin()+i);
            
            break;
        }
    }
    return TRUE;
}

BOOL CFilterChartView::RemoveAllGraph()
{
    int i =0;
    for (i=0; i< m_vecMathFml.size(); i++)
    {
        delete m_vecMathFml[i];   
    }
    
    for (i=0; i< m_vecFilter.size(); i++)
    {
        delete m_vecFilter[i];
    }
    m_vecGraphIdx.clear();
    m_vecMathFml.clear();
    m_vecFilter.clear();

    return TRUE;
}

void CFilterChartView:: graph_AddPoint(int graph_index,double x, double y, ULONG idx)
{      
    double X1, X2, Y1, Y2;
    m_graph_wnd.GetGraphWorldCoords(&X1, &X2, &Y1, &Y2);
    
    BOOL bUpdateFlag= FALSE;
    if (x > X2)
    {
        if (m_bAutoScroll)
        {
            
            X1 = x-m_screen_x+m_expand_range;      
            X2 = x+m_expand_range;
        }
        else
        {
            X2 += (x-X1)/5;
        }

        bUpdateFlag = TRUE;
    }
    
    if (y > Y2)
    {
        Y2 += m_expand_range;  
        bUpdateFlag = TRUE;
    }

    if (y < Y1)
    {
        Y1 -= m_expand_range; 
        bUpdateFlag = TRUE;
    }

    if (bUpdateFlag)
    {
        m_graph_wnd.SetGraphWorldCoords(X1, X2, Y1, Y2, TRUE);   
        bUpdateFlag = FALSE;
    }
 
    
    m_graph_wnd.AddPoint(graph_index, x, y, idx);
    
}

BOOL CFilterChartView::CreateGraphThread()
{

    m_hThreadProc = CreateThread( 
        NULL,
        0,
        (LPTHREAD_START_ROUTINE)DrawThread,
        this,
        0,
        NULL);
    
    if( m_hThreadProc == NULL ) 
        return FALSE;

    return TRUE;
}

DWORD CFilterChartView:: DrawThread(LPVOID pParam )
{
    CFilterChartView* pChartView= (CFilterChartView*)pParam;
    pChartView->ThreadProc();
    return 0L;
}

void CFilterChartView::ThreadProc()
{
    DWVECTOR vecTP;
    vecTP.reserve(10000);
    
    while(1)
    { 
        for (int idx= 0; idx< m_vecFilter.size() ; idx++)
        {   
            CFilter*pFilter= m_vecFilter[idx];
            if (0 == pFilter->GetUpdate(vecTP))
            {            
               //Sleep(100);
                continue;
            }  
            
            for(ULONG i=0 ;i < vecTP.size(); i++)         
            {
                DWORD dwIdx  = vecTP.at(i);
 //               WORD  wAddr  = m_pTPSet->GetTPAddr(dwIdx);
                WORD  wData  = m_pTPSet->GetTPData(dwIdx);
                DWORD dwTime = m_pTPSet->GetTPTime(dwIdx);
                
                int   graph_index= m_vecGraphIdx[idx];

                double ly;   
                if (m_bUnsiged)
                {
                    ly= wData;
                }
                else
                {
                    ly= (short)wData;
                }

                if(!DataDeal(idx, graph_index, ly))
                {               
                    continue;
                }

                double lx= m_bByTime ? dwTime: m_graph_wnd.GetGraph(graph_index)->GetSize();
      
                graph_AddPoint(graph_index, lx, ly, dwIdx);            
            }
            
            vecTP.clear();
        } //end for

        Sleep(1000);
    }//end while
}

BOOL CFilterChartView:: DataDeal(int math_index, int graph_index ,double& dbData)
{
	UNREFERENCED_PARAMETER(graph_index);
      CMathFormula* pmathfml= m_vecMathFml[math_index];    
      if ( NULL == pmathfml)    
            return FALSE; 
    
      dbData= pmathfml->math_deal(dbData);
    
    return TRUE;
}


void CFilterChartView:: ResetGraph(int graph_index)
{
    m_graph_wnd.ClearGraph(graph_index);
    for (int i=0; i< m_vecFilter.size(); i++)
    {
        if(m_vecGraphIdx[graph_index] == graph_index)
        {
           m_vecFilter[i]->Reset();   
        }
    }   
}

void CFilterChartView:: StartFilter()
{
    m_bStart =TRUE;
    for (int i=0; i< m_vecFilter.size(); i++)
    {
        m_vecFilter[i]->ResumeFIlter();
    }
}

void CFilterChartView::StopFilter()
{
    m_bStart =FALSE;
    for (int i=0; i< m_vecFilter.size(); i++)
    {
        m_vecFilter[i]->PauseFilter();
    }

}