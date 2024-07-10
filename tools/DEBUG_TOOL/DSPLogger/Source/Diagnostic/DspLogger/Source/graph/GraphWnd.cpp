#include "stdafx.h"
#include "GraphWnd.h"
#include "gen_prop_dlgs.h"

//head file extend
#include "../Gui/Resource.h"
#include "../Gui/DspLogger.h"
#include "../Gui/MainFrm.h"
#include "../Gui/FindDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphWnd

CGraphWnd::CGraphWnd(long maximum_graphs/* = 32*/)
{
    graph_frame = NULL;
    m_graphCount = 0;
    m_graph_maximum = maximum_graphs;
    m_graphs = new PCGraphProps[m_graph_maximum];

    for (int i=0; i<m_graph_maximum && m_graphs; i++) 
    {
		m_graphs[i] = NULL;
    }
    m_bAutoUpdateTimer = FALSE;
    m_nGraphSelect     = -1;
    m_opt = OPT_POINT; 
	axis_prop_page = NULL;
	graph_prop_page = NULL;
    
}

CGraphWnd::~CGraphWnd()
{
    for (int i=0; i<m_graph_maximum; i++) 
    {
	if (m_graphs[i] != NULL) 
	{
	    delete m_graphs[i];
	    m_graphs[i] = NULL;
	};
    };
    delete[] m_graphs;
}


BEGIN_MESSAGE_MAP(CGraphWnd, CWnd)
	//{{AFX_MSG_MAP(CGraphWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGraphWnd message handlers

int CGraphWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
	    return -1;
    
    CRuntimeClass* run_time_class =RUNTIME_CLASS(CGraphFrame);

    ASSERT(run_time_class != NULL);

    if (run_time_class != NULL)
    {	   
	graph_frame = reinterpret_cast<CGraphFrame*>(run_time_class->CreateObject());	
    };

    CRect rect;
    GetClientRect(rect);
    BOOL b = graph_frame->Create(_T(""), rect, this, m_bAutoUpdateTimer);
    ASSERT(b);

    return 0;
}

void CGraphWnd::OnSize(UINT nType, int cx, int cy) 
{
    CWnd::OnSize(nType, cx, cy);
    
    if (graph_frame != NULL)
    {
	graph_frame->MoveWindow(0, 0, cx, cy);
    };
}

BOOL CGraphWnd::OnEraseBkgnd(CDC* pDC) 
{
	UNREFERENCED_PARAMETER(pDC);
    return FALSE;
}

BOOL CGraphWnd::Create(LPCTSTR lpszWindowName, const RECT& rect, CWnd* pParentWnd, UINT nID, 
    DWORD dwStyle/* = WS_CHILD | WS_VISIBLE*/, BOOL bAutoUpdateTimer/* = FALSE*/)
{
    m_bAutoUpdateTimer = bAutoUpdateTimer;
    return CWnd::Create(NULL, lpszWindowName, dwStyle, rect, pParentWnd, nID);
}

int CGraphWnd::AddGraph(COLORREF newColor/* = 0*/, const TCHAR* title/* = ""*/, BOOL bRedraw/* = TRUE*/,
    BOOL b_sort_x/* = FALSE*/, BOOL b_keep_same_x/* = TRUE*/)
{
    int index = -1;
    //find first non-NULL element in m_graphCount array
    for (int i=0; i<m_graph_maximum; i++)
    {
		if (m_graphs[i]==NULL)
		{
			index = i;
			break;
		}
    }
    if (index == -1) return -1;
	
    m_graphs[index] = new CGraphProps(newColor, title, TRUE, TRUE, -1, b_sort_x, b_keep_same_x);
	if (NULL == m_graphs[index])
	{
		return -1;
	}
    m_graphCount+=1;
	
    if (graph_frame != NULL)
    {
		SGraphChange sgc;
		sgc.graphnum = index;
		sgc.bRedraw = bRedraw;
		sgc.main_wnd_ptr = this;
		
		graph_frame->UpdateViews(GRAPH_GRAPH_ADDED, &sgc);
    }

    return index;
}

int CGraphWnd::RemoveGraph(int index, BOOL bRedraw/* = TRUE*/)
{
    if (index<0 || index>m_graph_maximum) return -1;
    if (m_graphs[index] == NULL) return -1;

    delete m_graphs[index];
    m_graphs[index] = NULL;
    m_graphCount-=1;

    if (graph_frame != NULL)
    {
	SGraphChange sgc;
	sgc.graphnum = index;
	sgc.bRedraw = bRedraw;
	sgc.main_wnd_ptr = this;

	graph_frame->UpdateViews(GRAPH_GRAPH_REMOVED, &sgc);
    };
    return index;
}

CGraphProps* CGraphWnd::GetFirstGraph(int* index)
{
    *index = -1;
    for (int i=0; i<m_graph_maximum; i++)
    {
	if (m_graphs[i]!=NULL)
	{
	    *index = i;
	    break;
	};
    };
    if (*index == -1) return NULL;

    return m_graphs[*index];
}

CGraphProps* CGraphWnd::GetNextGraph(int* index)
{
    for (int i= (*index)+1; i<m_graph_maximum; i++)
    {
	if (m_graphs[i]!=NULL)
	{
	    *index = i;
	    break;
	};
    };
    if (i == m_graph_maximum)
    {
	*index = -1;
	return NULL;
    };
    return m_graphs[*index];
}



int  CGraphWnd::AddPoint(int graphnum, double x, double y, int index)
{
    int ind, res;
    CGraphProps* graph = GetGraph(graphnum);
    if (graph == NULL) return -1;
    
    ind = graph->AddPoint(x, y, TRUE, &res ,index);
 
    
    if (graph_frame != NULL)
    {
        SGraphChange sgc;
        sgc.graphnum = graphnum;
        sgc.index = ind;
        sgc.bRedraw = TRUE;
        sgc.main_wnd_ptr = this;
        
        graph_frame->UpdateViews(GRAPH_POINT_ADDED, &sgc);
    };
    return ind;
}

int CGraphWnd::InsertPoint(int graphnum, double x, double y, int insertPos/* = -1*/)
{
    int ind, res;
    CGraphProps* graph = GetGraph(graphnum);
    if (graph == NULL) return -1;
    if (insertPos == -1) 
    {
        ind = graph->AddPoint(x, y, TRUE, &res);
    } else
    {
        ind = graph->InsertPoint(insertPos, x, y, TRUE);
    };
    
    if (graph_frame != NULL)
    {
        SGraphChange sgc;
        sgc.graphnum = graphnum;
        sgc.index = ind;
        sgc.bRedraw = TRUE;
        sgc.main_wnd_ptr = this;
        
        graph_frame->UpdateViews(GRAPH_POINT_ADDED, &sgc);
    };
    return ind;
}

int CGraphWnd::EditPoint(int graphnum, int index, double x, double y, BOOL bRedraw)
{
    CGraphProps* graph = GetGraph(graphnum);
    if (graph == NULL) return -1;
    int res = graph->EditPoint(index, x, y, TRUE);

    if (graph_frame != NULL)
    {
	SGraphChange sgc;
	sgc.graphnum = graphnum;
	sgc.index = index;
	sgc.bRedraw = bRedraw;
	sgc.main_wnd_ptr = this;

	graph_frame->UpdateViews(GRAPH_POINT_CHANGED, &sgc);
    };
    return res;
}

int CGraphWnd::RemovePoint(int graphnum, int index, BOOL bRedraw)
{
    CGraphProps* graph = GetGraph(graphnum);
    if (graph == NULL) return -1;
    graph->RemovePoint(index, FALSE);

    if (graph_frame != NULL)
    {
	SGraphChange sgc;
	sgc.graphnum = graphnum;
	sgc.index = index;
	sgc.bRedraw = bRedraw;
	sgc.main_wnd_ptr = this;

	graph_frame->UpdateViews(GRAPH_POINT_REMOVED, &sgc);
    };
    return index;
}

void CGraphWnd::ClearGraph(int graphnum, BOOL bRedraw)
{
    CGraphProps* graph;
    if (graphnum != -1)
    {
	graph = GetGraph(graphnum);
	if (graph == NULL) return;
	graph->RemoveAll();
    } else
    {
	int index;
	graph = GetFirstGraph(&index);
	while (graph!=NULL)
	{
	    graph->RemoveAll();
	    graph = GetNextGraph(&index);
	};
    };
    if (graph_frame != NULL)
    {
	SGraphChange sgc;
	sgc.graphnum = graphnum;
	sgc.bRedraw = bRedraw;
	sgc.main_wnd_ptr = this;

	graph_frame->UpdateViews(GRAPH_GRAPH_CLEARED, &sgc);
    };
}

void CGraphWnd::SetGraphWorldCoords(double x1, double x2, double y1, double y2, BOOL bRedraw)
{//lint !e578
    if (graph_frame!=NULL)
    {
	SGraphixProps sgp;
	sgp.x1 = x1; sgp.x2 = x2;
	sgp.y1 = y1; sgp.y2 = y2;
	sgp.flags = GetGraphFlags();
	sgp.bRedraw = bRedraw;
	sgp.main_wnd_ptr = this;

	graph_frame->UpdateViews(GRAPH_GRAPHIX_PROP, &sgp);
    };
}

void CGraphWnd::SetGraphFlags(DWORD new_flags, BOOL bRedraw)
{
    if (graph_frame!=NULL)
    {
	SGraphixProps sgp;
	GetGraphWorldCoords(&sgp.x1, &sgp.x2, &sgp.y1, &sgp.y2);
	sgp.flags = new_flags;
	sgp.bRedraw = bRedraw;
	sgp.main_wnd_ptr = this;

	graph_frame->UpdateViews(GRAPH_GRAPHIX_PROP, &sgp);
    };
}

DWORD CGraphWnd::GetGraphFlags()
{
    return graph_frame->GetGraphFlags();
}

void CGraphWnd::GetGraphWorldCoords(double* x1, double* x2, double* y1, double* y2)
{//lint !e578
    graph_frame->GetGraphWorldCoords(x1, x2, y1, y2);
}

void CGraphWnd::SetAxisProps(const TCHAR* _title, const TCHAR* _UOM, int _precision, BOOL bXAxis, BOOL bRedraw)
{
    CAxisProps* props;
    if (bXAxis) props = &x_axis;
	    else props = &y_axis;
    props->SetNewTitle(_title);
    props->SetNewUOM(_UOM);
    props->SetNewPrecision(_precision);

    if (graph_frame != NULL)
    {
	SAxisPropsChange sapc;
	sapc.bXAxis = bXAxis;
	sapc.bRedraw = bRedraw;
	sapc.main_wnd_ptr = this;
	graph_frame->UpdateViews(GRAPH_GRAPHIX_AXISPROPS, &sapc);
    };
}

void CGraphWnd::FormatAxisOutput(double value, BOOL bXAxis, int format_level, CString& res_str)
{
    CString str;
    CAxisProps* props;
    if (bXAxis) props = &x_axis;
	    else props = &y_axis;
    props->GetFormattedOutput(value, format_level, &res_str);
}

BOOL CGraphWnd::GetBoundRect(double* minx, double* maxx, double* miny, double* maxy)
{
    int grcount = GetGraphCount();
    if (grcount == 0) return FALSE;
    int index;
    BOOL b = FALSE;
    CGraphProps* grprop = GetFirstGraph(&index);
    while(grprop!=NULL)
    {
	if (grprop->GetSize() == 0 || !grprop->IsVisible()) 
	{
	    grprop = GetNextGraph(&index);
	    continue;
	};
	if (!b)
	{//first time
	    *minx = grprop->min_x;
	    *maxx = grprop->max_x;
	    *miny = grprop->min_y;
	    *maxy = grprop->max_y;
	    b = TRUE;
	} else
	{
	    if (grprop->min_x<*minx) *minx = grprop->min_x;
	    if (grprop->max_x>*maxx) *maxx = grprop->max_x;
	    if (grprop->min_y<*miny) *miny = grprop->min_y;
	    if (grprop->max_y>*maxy) *maxy = grprop->max_y;
	};
	grprop = GetNextGraph(&index);
    };
    return b;
}

CAxisProps* CGraphWnd::GetAxisProps(unsigned long axis_enum)
{
    CAxisProps* res_props = NULL;
    if (axis_enum == GRAPH_X_AXIS)
    {
		res_props = &x_axis;
    } 
	else
    {
		res_props = &y_axis;
    }
    return res_props;
}

void CGraphWnd::AppendMenuItems(CMenu* menu)
{
	UNREFERENCED_PARAMETER(menu);
}

void CGraphWnd::AppendPropertyPage(CPropertySheet* prop_sheet)
{
    axis_prop_page = new CAxisPropertyPage();
	do 
	{
		if (NULL == axis_prop_page)	break;

		CAxisProps* axis = GetAxisProps(GRAPH_X_AXIS);
		if(NULL ==axis)				break;
		axis_prop_page->m_x_precision = axis->GetPrecision();
		axis_prop_page->m_x_title = axis->GetTitle();
		axis_prop_page->m_x_uom = axis->GetUOM();

		axis = GetAxisProps(GRAPH_Y_AXIS);
		if(NULL ==axis)				break;
		axis_prop_page->m_y_precision = axis->GetPrecision();
		axis_prop_page->m_y_title = axis->GetTitle();
		axis_prop_page->m_y_uom = axis->GetUOM();

		graph_prop_page = new CGraphicsPropertyPage(this);
		if (graph_prop_page)
		{
			prop_sheet->AddPage(axis_prop_page);
			prop_sheet->AddPage(graph_prop_page);
		}

	} while(0);
    
}

void CGraphWnd::ReleasePropertyPage(UINT dialog_status)
{
    if (dialog_status == IDOK)
    {
	SetAxisProps(axis_prop_page->m_x_title, axis_prop_page->m_x_uom,
	    axis_prop_page->m_x_precision, GRAPH_X_AXIS, FALSE);
	SetAxisProps(axis_prop_page->m_y_title, axis_prop_page->m_y_uom,
	    axis_prop_page->m_y_precision, GRAPH_Y_AXIS, FALSE);

	for (int i=0; i<graph_prop_page->grprops.GetSize(); i++)
	{
	    CGraphProps* grprop = graph_prop_page->grprops.GetAt(i); 
	    int index = grprop->GetIndex();
	    CGraphProps* grp = GetGraph(index);
	    grp->SetGraphProps(grprop);
	};
    };

    delete axis_prop_page;
    delete graph_prop_page;
}

void CGraphWnd::UpdateWindows(unsigned long what_to_update)
{
    if (graph_frame != NULL)
    {
	graph_frame->UpdateWindows(what_to_update);
    };
}

void CGraphWnd::OperateWithPointView(unsigned long pview_operations)
{
    if (graph_frame != NULL)
    {
	graph_frame->OperateWithPointView(pview_operations);
    };
}

void CGraphWnd::DrawGraphToDC(CDC* dest_dc, CRect& rect_to_draw)
{
    if (graph_frame != NULL)
    {
	graph_frame->DrawGraphToDC(dest_dc, rect_to_draw);
    };
}


//extend graph method
//goto selected item in TestPointsView
void CGraphWnd::GotoChartPoint(ULONG index)
{
    CMainFrame* pMainFrame    = (CMainFrame*)AfxGetMainWnd();
	CTestPointsView* m_pView  = pMainFrame->m_pView;
    if(NULL != m_pView)
    {   
        m_pView->GotoItem(index);   
    }
}

