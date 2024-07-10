
#include "stdafx.h"
#include "GraphPanel.h"
#include "f2doff.h"
#include "gruler.h"
#include "coords.h"
#include "graphwnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphPanel

CGraphPanel::CGraphPanel(FVRuler* ver_ruler, FHRuler* hor_ruler)
{
    offscreen = NULL;
    vruler = ver_ruler;
    hruler = hor_ruler;
    CurrentCoordsX = NULL;
    CurrentCoordsY = NULL;
    bkColor = RGB(255, 255, 255);
    m_grflags = (GRAPH_AUTOSCALE | GRAPH_DRAW_AXIS);
    //zoom
    CRect r(10, 10, 50, 50);
    bZoomActive = FALSE;
    bCreateZoomRect = FALSE;
    zoomrect = new CRectTracker(&r, CRectTracker::solidLine | CRectTracker::resizeInside | CRectTracker::hatchInside);
    //panning
    m_bPanning = FALSE;
    m_PanStart.x=m_PanStart.y = 0L;
    m_PanStop.x=m_PanStop.y   = 0L;
	bZoomDrag = FALSE;
	tipCount = 0;
	graph_prop_page = NULL;
}

CGraphPanel::~CGraphPanel()
{
    delete CurrentCoordsX;
    delete CurrentCoordsY;
    delete offscreen;
    delete zoomrect;
}


BEGIN_MESSAGE_MAP(CGraphPanel, CWnd)
	//{{AFX_MSG_MAP(CGraphPanel)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGraphPanel message handlers

BOOL CGraphPanel::OnEraseBkgnd(CDC* pDC) 
{
	UNREFERENCED_PARAMETER(pDC);
    return FALSE;
}

void CGraphPanel::OnSize(UINT nType, int cx, int cy) 
{
    InitCoords(GetX1(), GetX2(), GetY1(), GetY2());

    CWnd::OnSize(nType, cx, cy);

    if(offscreen)
    {// update offscreen 
	CRect r;	
	GetClientRect(&r);
	CSize s = r.Size();
	offscreen->ResizeDib( s.cx, s.cy );
    }
}

int CGraphPanel::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
	    return -1;
	
    CRect r;
    GetClientRect(&r);
    offscreen=new FOffScreen(r.Width(), r.Height());
	if (NULL == offscreen)
	{
		return -1;
	}

    EnableToolTips(TRUE);
    //create tool tip object
    tooltip.Create(this);
    tooltip.Activate(TRUE);
    tooltip.AddTool(this,  (LPCTSTR)_T(""));
    tooltip.SendMessage(TTM_SETMAXTIPWIDTH, 0, SHRT_MAX);
    tooltip.SendMessage(TTM_SETDELAYTIME, TTDT_AUTOPOP, SHRT_MAX);
    tooltip.SendMessage(TTM_SETDELAYTIME, TTDT_INITIAL, 100);
    tooltip.SendMessage(TTM_SETDELAYTIME, TTDT_RESHOW, 100);

    return 0;
}

void CGraphPanel::DoRedraw(CDC* dc, LPCRECT r)
{
    if (!offscreen)
    {
        return;
    };
    
    CRect rect;
    if (r == NULL)
    {
        GetClientRect(&rect);
    }
    else
    {
        rect.CopyRect(r);
    };
    if(rect.IsRectEmpty()) return;
    
    if( !offscreen->IsDIBReady() )
    {
        UpdateGraphWindow(NULL);
    };
    
    offscreen->DrawDib(dc->m_hDC, &rect);
    
    //draw zoom rect
    if (bZoomDrag || (bZoomActive && !bCreateZoomRect))
    {
        zoomrect->Draw(dc);

        bZoomDrag =FALSE;
    }
}

void CGraphPanel::OnPaint() 
{
    CPaintDC dc(this); // device context for painting
	
    PAINTSTRUCT *ps=&dc.m_ps;
    CRect rect(ps->rcPaint);

    DoRedraw(&dc, rect);
}

void CGraphPanel::DrawAxis(CDC* pDC, CRect& rect_to_draw)
{
    int i;
    if (CurrentCoordsX == NULL || CurrentCoordsY == NULL) return;

    //create new set of CCoordinates
    CCoordinates coords_x(rect_to_draw.left, rect_to_draw.right, CurrentCoordsX->v1(), CurrentCoordsX->v2());
    CCoordinates coords_y(rect_to_draw.bottom, rect_to_draw.top, CurrentCoordsY->v1(), CurrentCoordsY->v2());

    //draw axis and zero line first
    CPen BlackDashPen(PS_DOT, 1, RGB(0, 0, 0));
    CPen bkPen(PS_SOLID, 1, bkColor);
    CPen* OldPen = (CPen*)pDC->SelectObject(&BlackDashPen);
    pDC->SetBkMode( TRANSPARENT);
	    //horizontal
    double Delta;
    double* wp;
    double* sp;
    int count, maxexp, NMax;
    NMax = hruler->GetNMax(pDC, rect_to_draw);
    coords_x.DivideInterval(NMax, &maxexp, &Delta, &count, &sp, &wp);
    for (i=0; i<count; i++)
    {
	    int x = (int)sp[i];
	    pDC->MoveTo(x, rect_to_draw.bottom);
	    pDC->LineTo(x, rect_to_draw.top);
    };
    if (sp!=NULL) delete sp;
    if (wp!=NULL) delete wp;
    //vertical
    NMax = vruler->GetNMax(pDC, rect_to_draw);
    coords_y.DivideInterval(NMax, &maxexp, &Delta, &count, &sp, &wp);
    for (i=0; i<count; i++)
    {
	    int x = (int)sp[i];
	    pDC->MoveTo(rect_to_draw.right,x);
	    pDC->LineTo(rect_to_draw.left,x);
    };
    if (sp!=NULL) delete sp;
    if (wp!=NULL) delete wp;

    pDC->SelectObject(OldPen);
}

void CGraphPanel::UpdateGraphWindow(LPCRECT rect)
{
    if(!offscreen)
    {
		return;
    };
    CRect r;
    if (rect==NULL) GetClientRect(&r);
	else r.CopyRect(rect);

    //do paint here
    CDC* dc = offscreen->GetDibCDC();
    if (dc != NULL)
    {
	CRect rect_to_draw;
	GetClientRect(rect_to_draw);
	DrawToDC(dc, rect_to_draw);
	offscreen->SetReady(TRUE);
    };

    CDC* cdc = GetDC();
    DoRedraw(cdc, &r);
    ReleaseDC(cdc);
}

void CGraphPanel::DrawToDC(CDC* dc_to_draw, CRect& rect_to_draw)
{
    //fill background
    CBrush bkbrush(bkColor);
    CBrush* oldbrush = (CBrush*)dc_to_draw->SelectObject(&bkbrush);

    CRect bkrect(rect_to_draw);
    bkrect.bottom+=1; bkrect.right+=1; bkrect.left-=1; bkrect.top -= 1;
    dc_to_draw->Rectangle(bkrect);
    dc_to_draw->SelectObject(oldbrush);

    //draw points
    DrawPoints(dc_to_draw, rect_to_draw);

    //axis
    if ((m_grflags & GRAPH_DRAW_AXIS) == GRAPH_DRAW_AXIS) DrawAxis(dc_to_draw, rect_to_draw);
}

void CGraphPanel::InitCoords(double x1, double x2, double y1, double y2)
{//lint !e578
    if (CurrentCoordsX!=NULL)
    {
		delete CurrentCoordsX;
		CurrentCoordsX = NULL;
    }
    if (CurrentCoordsY!=NULL)
    {
		delete CurrentCoordsY;
		CurrentCoordsY = NULL;
    }

    CRect r;
    GetClientRect(&r);
    CurrentCoordsX = new CCoordinates(r.left, r.right, x1, x2);
    CurrentCoordsY = new CCoordinates(r.bottom, r.top, y1, y2);
}

void CGraphPanel::DrawCross(CDC* dc, int x, int y)
{
    
    CPen   pen;
    CPen * pOld;
    pen.CreatePen(PS_SOLID, 2, RGB(0,0,0));
    
    pOld = dc->SelectObject(&pen);
    dc->MoveTo(x - 5, y);
    dc->LineTo(x + 5, y);
    
    dc->MoveTo(x, y - 5);
    dc->LineTo(x, y + 5);
    
    dc->SelectObject(pOld);

}

void CGraphPanel::DrawPoints(CDC* dc, CRect& rect_to_draw)
{
    //draw all the points
    SSinglePoint ssp, ssp1;
    int x, y, x1, y1, grnum;//lint !e578
    if (CurrentCoordsX == NULL || CurrentCoordsY == NULL) return;
    CGraphWnd* main_wnd = get_main_graph_window();
    if (main_wnd == NULL) return;

    //save current state of device context
    int saved_dc = dc->SaveDC();

    //set new clipping region
    dc->IntersectClipRect(rect_to_draw);

    //create new set of CCoordinates
    CCoordinates coords_x(rect_to_draw.left, rect_to_draw.right, CurrentCoordsX->v1(), CurrentCoordsX->v2());
    CCoordinates coords_y(rect_to_draw.bottom, rect_to_draw.top, CurrentCoordsY->v1(), CurrentCoordsY->v2());

    CGraphProps* grprop = main_wnd->GetFirstGraph(&grnum);
    while (grprop!=NULL)
    {
	if (grprop->GetSize() ==0 || !grprop->IsVisible()) 
	{
		grprop = main_wnd->GetNextGraph(&grnum);
		continue;
	};
	//set color
	CPen* pen = grprop->GetPen();
	CBrush* brush = grprop->GetBrush();
	CPen* oldpen = (CPen*)dc->SelectObject(pen);
	CBrush* oldbrush = (CBrush*)dc->SelectObject(brush);

	grprop->GetPoint(0, &ssp);
	x = (int)coords_x.WtoX(ssp.x);
	y = (int)coords_y.WtoX(ssp.y);
	if ((long)(m_grflags & GRAPH_SQUAREPOINTS)!=0) DrawSquarePoint(dc, x, y);
	for (int i=1; i<grprop->GetSize(); i++)
	{
	    grprop->GetPoint(i, &ssp);
	    x1 = (int)coords_x.WtoX(ssp.x);
	    y1 = (int)coords_y.WtoX(ssp.y);
	    if (x != x1 || y != y1)
	    {
		if ((m_grflags & GRAPH_SQUAREPOINTS)!=0) DrawSquarePoint(dc, x1, y1);
		if ((m_grflags & GRAPH_GRAPH_SCATTER) == 0)
		{
		    dc->MoveTo(x, y);
		    dc->LineTo(x1, y1);
		};
		x = x1; y = y1;
	    };
	};

    //drow cross point
    SSinglePoint* pssp = grprop->GetCrossPoint();
    int xcross = (int)coords_x.WtoX(pssp->x);
    int ycross = (int)coords_y.WtoX(pssp->y);   
    DrawCross(dc, xcross, ycross);


	dc->SelectObject(oldpen);
	dc->SelectObject(oldbrush);
	grprop->ReleasePen(pen);
	grprop->ReleaseBrush(brush);

	grprop = main_wnd->GetNextGraph(&grnum);
    };
    dc->RestoreDC(saved_dc);
}

int CGraphPanel::GetSquareSide(CDC* dest_dc)
{
    const double square_size = 1/35.0;

    int log_pix = GetDeviceCaps(dest_dc->m_hAttribDC, LOGPIXELSY);
    return (int)(square_size*log_pix);
}

void CGraphPanel::DrawSquarePoint(CDC* dc, int x, int y)
{
    int square_pix_size = GetSquareSide(dc);

    dc->Rectangle(  x - square_pix_size, 
		    y - square_pix_size, 
		    x+square_pix_size+1, 
		    y+square_pix_size+1);
}

void CGraphPanel::SetMinX(double x, BOOL bRedraw)
{
    if (__min(GetX1(), GetX2()) == GetX1()) 
    {
	SetWorldCoords(x, GetX2(), GetY1(), GetY2(), bRedraw);
    } else
    {
	SetWorldCoords(GetX1(), x, GetY1(), GetY2(), bRedraw);
    };
}

void CGraphPanel::SetMaxX(double x, BOOL bRedraw)
{
    if (__max(GetX1(), GetX2()) == GetX1()) 
    {
	SetWorldCoords(x, GetX2(), GetY1(), GetY2(), bRedraw);
    } else
    {
	SetWorldCoords(GetX1(), x, GetY1(), GetY2(), bRedraw);
    };
}

void CGraphPanel::SetMinY(double y, BOOL bRedraw)
{
    if (__min(GetY1(), GetY2()) == GetY1()) 
    {
	SetWorldCoords(GetX1(), GetX2(), y, GetY2(), bRedraw);
    } else
    {
	SetWorldCoords(GetX1(), GetX2(), GetY1(), y, bRedraw);
    };
}

void CGraphPanel::SetMaxY(double y, BOOL bRedraw)
{
    if (__max(GetY1(), GetY2()) == GetY1()) 
    {
	SetWorldCoords(GetX1(), GetX2(), y, GetY2(), bRedraw);
    } else
    {
	SetWorldCoords(GetX1(), GetX2(), GetY1(), y, bRedraw);
    };
}

void CGraphPanel::SetWorldCoords(double _x1, double _x2, double _y1, double _y2, BOOL bRedraw)
{//lint !e578
    if (_x1 != GetX1() || _x2 != GetX2() || _y1 != GetY1() || _y2 != GetY2())
    {
	InitCoords(_x1, _x2, _y1, _y2);
	if (vruler != NULL)
	{
	    vruler->SetMinMax(_y1, _y2, FALSE);
	    if (bRedraw)
	    {
		vruler->RedrawWindow();
	    };
	};
	if (hruler != NULL)
	{
	    hruler->SetMinMax(_x1, _x2, FALSE);
	    if (bRedraw)
	    {
		hruler->RedrawWindow();
	    };
	};
	if (bRedraw)
	{
	    UpdateGraphWindow(NULL);
	};
    };
}

double CGraphPanel::GetX1()
{
    return CurrentCoordsX == NULL ? 0 : CurrentCoordsX->v1();
}

double CGraphPanel::GetX2()
{
    return CurrentCoordsX == NULL ? 10 : CurrentCoordsX->v2();
}

double CGraphPanel::GetY1()
{
    return CurrentCoordsY == NULL ? 0 : CurrentCoordsY->v1();
}

double CGraphPanel::GetY2()
{
    return CurrentCoordsY == NULL ? 10 : CurrentCoordsY->v2();
}

void CGraphPanel::GetWorldCoords(double* x1, double* x2, double* y1, double* y2)
{//lint !e578
    *x1 = GetX1();
    *x2 = GetX2();
    *y1 = GetY1();
    *y2 = GetY2();
}

LRESULT CGraphPanel::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
    if (message == WM_COMMAND && (wParam>GRAPH_RBMC_FIRST && wParam<GRAPH_RBMC_LAST))
    {//this is right button menu commands
	EnumerateParentWindows(OnRBMenuCommandForAllParents, (void*)wParam);
    };

    if (message == CM_GRAPH_NOTIFICATION)
    {
		switch (wParam)
		{
		case GRAPH_GRAPH_CLEARED:
		case GRAPH_GRAPH_REMOVED:
		case GRAPH_POINT_REMOVED:
		case GRAPH_POINT_CHANGED:
			{
				SGraphChange* sgc = (SGraphChange*)lParam;
				if (sgc->bRedraw)
				{
					UpdateGraphWindow(NULL);
				};
			}; break;
		case GRAPH_POINT_ADDED:
			{
				AddPoint((SGraphChange*)lParam);
				//return NULL;
			}; break;
		case GRAPH_GRAPH_ADDED:
			{
			}; break;
		case GRAPH_GRAPHIX_PROP:
			{
				SGraphixProps* sgp = (SGraphixProps*)lParam;
				SetGraphixFlags(sgp->flags, sgp->bRedraw);
				SetWorldCoords(sgp->x1, sgp->x2, sgp->y1, sgp->y2, sgp->bRedraw);
			}; break;
		case GRAPH_GRAPHIX_AXISPROPS:
			{
				SAxisPropsChange* sapc = (SAxisPropsChange*)lParam;
				UpdateTitles(sapc->bXAxis, sapc->bRedraw);
			}; break;
		case GRAPH_GRAPHIX_UPDATE:
			{
				if ((lParam & GRAPH_WUV_GRAPH) != 0)
				{
					UpdateGraphWindow(NULL);
				};
				if ((lParam & GRAPH_WUV_RULERS) != 0)
				{
					hruler->RedrawWindow();
					vruler->RedrawWindow();
				};
			}; break;
		};
    };	
    return CWnd::WindowProc(message, wParam, lParam);
}

void CGraphPanel::AddPoint(SGraphChange* sgc)
{
    //get main view pointer
    if (sgc->main_wnd_ptr == NULL) return;
    SSinglePoint ssp;
    CGraphProps* sgp = sgc->main_wnd_ptr->GetGraph(sgc->graphnum);
    if (!sgp->IsVisible())
    {
	//if graph is not visible - do not do anything here
	return;
    };
    sgp->GetPoint(sgc->index, &ssp);
    BOOL bNeedRedraw = FALSE;
    if ((unsigned long)(m_grflags & GRAPH_AUTOSCALE))
    {
	//if grafix is autoscaled - just recalc coordinates and redraw the window
	if (ssp.x<__min(GetX1(), GetX2())) 
	{
	    SetMinX(ssp.x, FALSE);
	    bNeedRedraw = TRUE;
	};
	if (ssp.x>__max(GetX1(), GetX2())) 
	{
	    SetMaxX(ssp.x, FALSE);
	    bNeedRedraw = TRUE;
	};
	if (ssp.y<__min(GetY1(), GetY2())) 
	{
	    SetMinY(ssp.y, FALSE);
	    bNeedRedraw = TRUE;
	};
	if (ssp.y>__max(GetY1(), GetY2())) 
	{
	    SetMaxY(ssp.y, FALSE);
	    bNeedRedraw = TRUE;
	};
    };
    //draw new point 
    if (bNeedRedraw) 
    {
        if (sgc->bRedraw)
        {
            UpdateGraphWindow(NULL);
        };
        return;
    };

    if (!sgc->bRedraw || offscreen == NULL) return;

    //if point is inside the graph - we need invalidate the whole picture
    if (sgc->index!=0 && sgc->index!=sgp->GetSize()-1)
    {
        if (sgc->bRedraw)
        {
            UpdateGraphWindow(NULL);
        };
        return;
    };

    int x, y, x1, y1;//lint !e578
    x = (int)CurrentCoordsX->WtoX(ssp.x);
    y = (int)CurrentCoordsY->WtoX(ssp.y);

    CDC* dc = offscreen->GetDibCDC();
    if (dc != NULL)
    {
        CDC* cdc = GetDC();
        
        CPen* pen = sgp->GetPen();
        CBrush* brush = sgp->GetBrush();
        
        CPen* oldpen = (CPen*)dc->SelectObject(pen);
        CBrush* oldbrush = (CBrush*)dc->SelectObject(brush);
        
        if (sgp->GetSize()>1)
        {
            if (sgc->index == 0) sgp->GetPoint(1, &ssp);
            if (sgc->index == sgp->GetSize()-1) sgp->GetPoint(sgc->index-1, &ssp);
            x1 = (int)CurrentCoordsX->WtoX(ssp.x);
            y1 = (int)CurrentCoordsY->WtoX(ssp.y);
            if (x != x1 || y != y1)
            {
                if ((m_grflags & GRAPH_GRAPH_SCATTER) == 0)
                {
                    dc->MoveTo(x, y);
                    dc->LineTo(x1, y1);
                };
                
                if ((m_grflags & GRAPH_SQUAREPOINTS) != 0) 
                {
                    DrawSquarePoint(dc, x, y);
                };
                
                CRect update_rect(x, y, x1, y1);
                update_rect.NormalizeRect();
                update_rect.InflateRect(GetSquareSide(dc) + 2, GetSquareSide(dc) + 2);
                DoRedraw(cdc, update_rect);
            };
        }
        else
        {
            if ((m_grflags & GRAPH_SQUAREPOINTS) != 0) 
            {
                DrawSquarePoint(dc, x, y);
                
                CRect update_rect(x, y, x, y);
                update_rect.NormalizeRect();
                update_rect.InflateRect(GetSquareSide(dc) + 2, GetSquareSide(dc) + 2);
                DoRedraw(cdc, update_rect);
            };
        };
        
        dc->SelectObject(oldpen);
        dc->SelectObject(oldbrush);
        
        sgp->ReleasePen(pen);
        sgp->ReleaseBrush(brush);
        
        ReleaseDC(cdc);
    };
}

DWORD CGraphPanel::GetGraphixFlags()
{
    return m_grflags;
}

DWORD CGraphPanel::SetGraphixFlags(DWORD new_flags, BOOL bRedraw)
{
    DWORD old_flags = m_grflags;
    if (old_flags != new_flags)
    {
	m_grflags = new_flags;
	if (bRedraw)
	{
	    UpdateGraphWindow(NULL);
	};
    };
    return old_flags;
}

void CGraphPanel::UpdateTitles(BOOL bXAxis, BOOL bRedraw)
{
    CString str;
    CGraphWnd* grview = get_main_graph_window();

    if (grview == NULL) return;
    if (bXAxis)
    {
	    grview->GetAxisProps(GRAPH_X_AXIS)->GetFullTitle(&str);
	    hruler->SetNewTitles((char*)(LPCTSTR)grview->GetAxisProps(GRAPH_X_AXIS)->GetUOM(),
		    (char*)(LPCTSTR)str);
	    if (bRedraw) hruler->RedrawWindow();
    } else
    {
	    grview->GetAxisProps(GRAPH_Y_AXIS)->GetFullTitle(&str);
	    vruler->SetNewTitles((char*)(LPCTSTR)grview->GetAxisProps(GRAPH_Y_AXIS)->GetUOM(),
		    (char*)(LPCTSTR)str);
	    if (bRedraw) vruler->RedrawWindow();
    };
}

void CGraphPanel::OnMouseMove(UINT nFlags, CPoint point) 
{
    //tooltips
    tipCount -= 1;
    if (tipCount<=0)
    {
        tooltip.Activate((m_grflags & GRAPH_SHOW_TOOLTIP) == GRAPH_SHOW_TOOLTIP);
        if ((m_grflags & GRAPH_SHOW_TOOLTIP) == GRAPH_SHOW_TOOLTIP)
        {
            CString str;
            GetToolTipString(point, str);
            tooltip.UpdateTipText(str, this);
        };
        tipCount = MAX_TIP_COUNT;
    };

    //panning
    if (m_bPanning)
    {
        m_PanStart = m_PanStop;
        m_PanStop = point;
        double dx = CurrentCoordsX->XtoW(m_PanStart.x) - CurrentCoordsX->XtoW(m_PanStop.x);
        double dy = CurrentCoordsY->XtoW(m_PanStart.y) - CurrentCoordsY->XtoW(m_PanStop.y); 
        
        SetWorldCoords( GetX1()+dx, GetX2()+dx, GetY1()+dy, GetY2()+dy, TRUE);
    }
    else
    {
        //if on-the-fly zoom is active - redraw new rectangle
        if (bCreateZoomRect)
        {          
            new_zoom_2 = point;
            CRect rect(new_zoom_1.x, new_zoom_1.y, new_zoom_2.x, new_zoom_2.y);
            rect.NormalizeRect();
            
            if (rect.Height() < 20 || rect.Width() < 20)
            {
                return;
            }

            zoomrect->m_rect = rect;
 
            bZoomDrag = TRUE;
            Invalidate(FALSE);

        };
    };
    CWnd::OnMouseMove(nFlags, point);
}

void CGraphPanel::OnLButtonUp(UINT nFlags, CPoint point) 
{
    //panning
    if (m_bPanning)
    {
      m_bPanning = FALSE;
      m_PanStop = point;
      double dx = CurrentCoordsX->XtoW(m_PanStart.x) - CurrentCoordsX->XtoW(m_PanStop.x);
      double dy = CurrentCoordsY->XtoW(m_PanStart.y) - CurrentCoordsY->XtoW(m_PanStop.y); 
      SetWorldCoords(GetX1()+dx, GetX2()+dx, GetY1()+dy, GetY2()+dy, TRUE);
    } 
    else
    {
	//zoom
	if (bCreateZoomRect)
	{
	    CRect old_rect(new_zoom_1.x, new_zoom_1.y, new_zoom_2.x, new_zoom_2.y);
	    old_rect.NormalizeRect();

	    new_zoom_2 = point;
	    bCreateZoomRect = FALSE;

	    CRect rect(new_zoom_1.x, new_zoom_1.y, new_zoom_2.x, new_zoom_2.y);
	    rect.NormalizeRect();
	    zoomrect->m_rect = rect;

	    if (abs(new_zoom_1.x-new_zoom_2.x)>10 && abs(new_zoom_1.y-new_zoom_2.y)>10)
	    {
		    bZoomActive = TRUE;
	    };

	    old_rect.UnionRect(&old_rect, &rect);
	    old_rect.InflateRect(2, 2);
	    InvalidateRect(&old_rect, FALSE);
	};
    };
    CWnd::OnLButtonUp(nFlags, point);
}

void CGraphPanel::OnLButtonDown(UINT nFlags, CPoint point) 
{
   //select point
    SHORT flag_shift  = GetKeyState(VK_SHIFT);
    SHORT flag_control= GetKeyState(VK_CONTROL);
    CGraphWnd* main_wnd = get_main_graph_window();

    if ( flag_shift>=0 && flag_control>=0 && OPT_POINT == main_wnd->m_opt)
    {
        SelectPoint(point);
    }   
    else if(flag_shift < 0 || (OPT_DRAG == main_wnd->m_opt && flag_control >=0))
    {
        //panning
        m_bPanning = TRUE;
        m_PanStart = point;
        m_PanStop  = point;
    } 
    else 
    {    
        //zoom
        if (!bZoomActive)
        {
            bCreateZoomRect = TRUE;
            new_zoom_1 = new_zoom_2 = point;
        };
        
        if (bZoomActive)
        {
            CRect r1, r2;
            zoomrect->GetTrueRect(&r1);
            if (zoomrect->Track(this, point))
            {
                zoomrect->GetTrueRect(&r2);
                InvalidateRect(&r1, FALSE);
                InvalidateRect(&r2, FALSE);
            };
        };
    };
    


    CWnd::OnLButtonDown(nFlags, point);
}

void CGraphPanel::SelectPoint(CPoint &point)
{
    //select point
    int grnum;
    CGraphProps* grprop = NULL;
   
    double x= CurrentCoordsX->XtoW(point.x);
    double y= CurrentCoordsY->XtoW(point.y);    
    
    CGraphWnd* main_wnd = get_main_graph_window();
    int graph_index= main_wnd->GetGraphSelect();
    if ( graph_index== -1)
    {
        grprop= main_wnd->GetFirstGraph(&grnum);
        main_wnd->SetGraphSelect(grnum);
    }
    else
    {
        grprop = main_wnd->GetGraph(graph_index);
    }

    if (NULL == grprop)
    {
        grprop= main_wnd->GetFirstGraph(&grnum);
        main_wnd->SetGraphSelect(grnum);   
    }

    if (NULL!= grprop && grprop->GetSize() !=0 && grprop->IsVisible()) 
    {
        
        SSinglePoint ssp;
        int index=  grprop->GetNearestPoint(x,y,&ssp);
        if (-1 != index)
        {
            grprop->SetCrossPoint(ssp);            
            main_wnd->UpdateWindows((unsigned long)GRAPH_WUV_ALL);

            main_wnd->SetGraphSelect(graph_index ,index);

            main_wnd->GotoChartPoint(ssp.idx);
        }

    }

}

BOOL CGraphPanel::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    if (bZoomActive)
    {
        if (zoomrect->SetCursor(this, nHitTest))
        {
            //#define IDC_HAND  MAKEINTRESOURCE(32649)
            SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32649)));
            return TRUE;
        };
    };
    if (m_bPanning)
    {
        SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32649)));
        return TRUE;
    }
    return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CGraphPanel::ApplyZoom()
{
    if (!bZoomActive) return;
    bZoomActive = FALSE;
    CRect r;
    zoomrect->GetTrueRect(&r);
    //convert this rectangle to world coords
    double x1 = CurrentCoordsX->XtoW(r.left);
    double x2 = CurrentCoordsX->XtoW(r.right);
    double y1 = CurrentCoordsY->XtoW(r.bottom);//lint !e578
    double y2 = CurrentCoordsY->XtoW(r.top);

    SetWorldCoords(x1, x2, y1, y2, TRUE);
}

void CGraphPanel::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
    if (bZoomActive) ApplyZoom();
    CWnd::OnLButtonDblClk(nFlags, point);
}

BOOL CGraphPanel::PreTranslateMessage(MSG* pMsg) 
{
    tooltip.RelayEvent(pMsg);
    return CWnd::PreTranslateMessage(pMsg);
}

void CGraphPanel::GetToolTipString(CPoint pt, CString& tip_str)
{
    if (CurrentCoordsX == NULL || CurrentCoordsY == NULL) return;
    CGraphWnd* main_wnd = get_main_graph_window();
    if (main_wnd == NULL) return;

    double v1, v2;
    v1 = CurrentCoordsX->XtoW(pt.x);
    v2 = CurrentCoordsY->XtoW(pt.y);

    CString str;
    main_wnd->FormatAxisOutput(v1, TRUE, 1, str);
    CString str1;
    main_wnd->FormatAxisOutput(v2, FALSE, 1, str1);

    tip_str.Format(_T("x=%s, y=%s"), str, str1);
}

void CGraphPanel::OnRButtonUp(UINT nFlags, CPoint point) 
{
    CWnd::OnRButtonUp(nFlags, point);

    ClientToScreen(&point);

    CMenu* menu = new CMenu();
	if (menu)
	{
		menu->CreatePopupMenu();
		
		//append common menu items from parent windows
		EnumerateParentWindows(AppendMenuForAllParents, (void*)menu);
		
		menu->TrackPopupMenu(TPM_CENTERALIGN, point.x, point.y, this);
		
		delete menu;
	}
    
}//lint !e533

void CGraphPanel::AppendMenuItems(CMenu* menu)
{
    //Point marks
    UINT menu_flags = MF_STRING | MF_ENABLED;
    if ((m_grflags & GRAPH_SQUAREPOINTS) == GRAPH_SQUAREPOINTS)
    {
	menu_flags |= MF_CHECKED;
    };
    menu->AppendMenu(menu_flags, GRAPH_RBMC_TOGGLE_POINT_MARKS, _T("Point marks"));

    //Scatter graph
    menu_flags = MF_STRING | MF_ENABLED;
    if ((m_grflags & GRAPH_GRAPH_SCATTER) == GRAPH_GRAPH_SCATTER)
    {
	menu_flags |= MF_CHECKED;
    };
    menu->AppendMenu(menu_flags, GRAPH_RBMC_TOGGLE_SCATTER_MODE, _T("Scatter graph"));

    //Point marks
    menu_flags = MF_STRING | MF_ENABLED;
    if ((m_grflags & GRAPH_SHOW_TOOLTIP) == GRAPH_SHOW_TOOLTIP)
    {
	menu_flags |= MF_CHECKED;
    };
    menu->AppendMenu(menu_flags, GRAPH_RBMC_TRACE_MOUSE, _T("Trace mouse coordinates"));

    //Point marks
    menu_flags = MF_STRING | MF_ENABLED;
    if ((m_grflags & GRAPH_DRAW_AXIS) == GRAPH_DRAW_AXIS)
    {
	menu_flags |= MF_CHECKED;
    };
    menu->AppendMenu(menu_flags, GRAPH_RBMC_SHOW_AXIS, _T("Axis"));

    //separator
    menu->AppendMenu(MF_SEPARATOR, 0, _T(""));

    //Zoom
    CMenu sub_menu;
    sub_menu.CreateMenu();
    menu->AppendMenu(MF_POPUP | MF_ENABLED | MF_STRING, (UINT)sub_menu.m_hMenu, _T("Zoom"));
    //Zoom tool
    menu_flags = MF_STRING | MF_ENABLED;
    if (bZoomActive)
    {
	menu_flags |= MF_CHECKED;
    };
    sub_menu.AppendMenu(menu_flags, GRAPH_RBMC_ZOOM_TOOL, _T("Zoom tool"));

    //Apply zoom
    menu_flags = MF_STRING;
    if (bZoomActive)
    {
	menu_flags |= MF_ENABLED;
    } else
    {
	menu_flags |= MF_GRAYED;
    };
    sub_menu.AppendMenu(menu_flags, GRAPH_RBMC_APPLY_ZOOM, _T("Apply"));
    sub_menu.Detach();

    //Fit
//    sub_menu.CreateMenu();
//    menu->AppendMenu(MF_POPUP | MF_ENABLED | MF_STRING, (UINT)sub_menu.m_hMenu, _T("Fit..."));
//    sub_menu.AppendMenu(MF_STRING | MF_ENABLED, GRAPH_RBMC_FIT_WIDTH, _T("Width"));
//    sub_menu.AppendMenu(MF_STRING | MF_ENABLED, GRAPH_RBMC_FIT_HEIGHT, _T("Height"));
//    sub_menu.AppendMenu(MF_STRING | MF_ENABLED, GRAPH_RBMC_FIT_PAGE, _T("Page"));
//    sub_menu.Detach();

    //separator
    menu->AppendMenu(MF_SEPARATOR, 0, _T(""));
    //properties
    menu->AppendMenu(MF_STRING | MF_ENABLED, GRAPH_RBMC_VIEW_PROPERTIES, _T("Properties"));
}

void CGraphPanel::ToggleGraphFlag(unsigned long graph_flag, BOOL bUpdate)
{
    DWORD win_flags = GetGraphixFlags();
    if ((win_flags & graph_flag) == graph_flag)
    {
	win_flags = win_flags & (~graph_flag);
    } else
    {
	win_flags = win_flags | graph_flag;
    };
    SetGraphixFlags(win_flags, bUpdate);
}

void CGraphPanel::OnRBMenuCommand(UINT command_id)
{
    switch (command_id)
    {
	case GRAPH_RBMC_TOGGLE_POINT_MARKS:
	{
	    ToggleGraphFlag(GRAPH_SQUAREPOINTS, TRUE);
	}; break;
	case GRAPH_RBMC_TRACE_MOUSE:
	{
	    ToggleGraphFlag(GRAPH_SHOW_TOOLTIP, FALSE);
	}; break;
	case GRAPH_RBMC_SHOW_AXIS:
	{
	    ToggleGraphFlag(GRAPH_DRAW_AXIS, TRUE);
	}; break;
	case GRAPH_RBMC_TOGGLE_SCATTER_MODE:
	{
	    ToggleGraphFlag(GRAPH_GRAPH_SCATTER, TRUE);
	}; break;
	case GRAPH_RBMC_ZOOM_TOOL:
	{
	    if (!bZoomActive)
	    {
		bZoomActive = TRUE;
		zoomrect->m_rect.left = 5;
		zoomrect->m_rect.top = 5;
		zoomrect->m_rect.right = 50;
		zoomrect->m_rect.bottom = 50;

	    } else
	    {
		bZoomActive = FALSE;
	    };
	    CRect r;
	    zoomrect->GetTrueRect(&r);
	    InvalidateRect(&r, FALSE);
	}; break;
	case GRAPH_RBMC_APPLY_ZOOM:
	{
	    ApplyZoom();
	}; break;
	case GRAPH_RBMC_FIT_WIDTH:
	case GRAPH_RBMC_FIT_HEIGHT:
	case GRAPH_RBMC_FIT_PAGE:
	{
	    DoFit(command_id);
	}; break;
	case GRAPH_RBMC_VIEW_PROPERTIES:
	{
	    CPropertySheet prop_sheet(_T("Properties"), this);
	    EnumerateParentWindows(AppendPropertyPageForAllParents, (void*)&prop_sheet);

	    UINT result = prop_sheet.DoModal();

	    EnumerateParentWindows(ReleasePropertyPageForAllParents, (void*)result);

	    if (result == IDOK)
	    {
		CGraphWnd* main_wnd = get_main_graph_window();
		main_wnd->UpdateWindows((unsigned long)GRAPH_WUV_ALL);
	    };

	}; break;
    };
}

void CGraphPanel::DoFit(UINT fitType)
{
    CGraphWnd* main_wnd = get_main_graph_window();
    if (main_wnd == NULL) return;
    double x1, x2, y1, y2;//lint !e578
    if (!main_wnd->GetBoundRect(&x1, &x2, &y1, &y2)) return;
    if (x1 == x2) x1 = x2 = x1*0.9;
    if (y1 == y2) y1 = y2 = y1*0.9;
    switch (fitType)
    {
	case GRAPH_RBMC_FIT_WIDTH:
	{
	    y1 = GetY1(); y2 = GetY2();
	}; break;
	case GRAPH_RBMC_FIT_HEIGHT:
	{
	    x1 = GetX1(); x2 = GetX2();
	}; break;
	case GRAPH_RBMC_FIT_PAGE:
	{
	}; break;
    };
    SetWorldCoords(x1, x2, y1, y2, TRUE);
}

void CGraphPanel::AppendPropertyPage(CPropertySheet* prop_sheet)
{
    graph_prop_page = new CGraphPropertyPage();
	if (graph_prop_page)
	{
		graph_prop_page->m_x1 = GetX1();
		graph_prop_page->m_x2 = GetX2();
		graph_prop_page->m_y1 = GetY1();
		graph_prop_page->m_y2 = GetY2();
		graph_prop_page->m_PointMarks = (GetGraphixFlags() & GRAPH_SQUAREPOINTS) != 0;
		graph_prop_page->m_MouseCoords = (GetGraphixFlags() & GRAPH_SHOW_TOOLTIP) != 0;
		graph_prop_page->m_ShowAxis = (GetGraphixFlags() & GRAPH_DRAW_AXIS) != 0;
		
		prop_sheet->AddPage(graph_prop_page);
	}
    
}

void CGraphPanel::ReleasePropertyPage(UINT dialog_status)
{
    if (dialog_status == IDOK)
    {//apply settings
	SetWorldCoords(graph_prop_page->m_x1, graph_prop_page->m_x2, 
	    graph_prop_page->m_y1, graph_prop_page->m_y2, FALSE);
	DWORD flags = GetGraphixFlags();
	if (graph_prop_page->m_PointMarks)
	{
	    flags |= GRAPH_SQUAREPOINTS;
	} else
	{
	    flags &= ~GRAPH_SQUAREPOINTS;
	};

	if (graph_prop_page->m_MouseCoords)
	{
	    flags |= GRAPH_SHOW_TOOLTIP;
	} else
	{
	    flags &= ~GRAPH_SHOW_TOOLTIP;
	};

	if (graph_prop_page->m_ShowAxis)
	{
	    flags |= GRAPH_DRAW_AXIS;
	} else
	{
	    flags &= ~GRAPH_DRAW_AXIS;
	};
	SetGraphixFlags(flags, FALSE);
    };
    delete graph_prop_page;
}

/////////////////////////////////////////////////////////////////////////////
// CGraphPropertyPage property page

IMPLEMENT_DYNCREATE(CGraphPropertyPage, CPropertyPage)

CGraphPropertyPage::CGraphPropertyPage() : CPropertyPage(CGraphPropertyPage::IDD)
{
	//{{AFX_DATA_INIT(CGraphPropertyPage)
	m_x1 = 0.0;
	m_x2 = 0.0;
	m_y1 = 0.0;
	m_y2 = 0.0;
	m_PointMarks = FALSE;
	m_MouseCoords = FALSE;
	m_ShowAxis = FALSE;
	//}}AFX_DATA_INIT
}

CGraphPropertyPage::~CGraphPropertyPage()
{
}

void CGraphPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGraphPropertyPage)
	DDX_Text(pDX, IDC_X1, m_x1);
	DDX_Text(pDX, IDC_X2, m_x2);
	DDX_Text(pDX, IDC_Y1, m_y1);
	DDX_Text(pDX, IDC_Y2, m_y2);
	DDX_Check(pDX, IDC_GRAPH_POINTMARKS, m_PointMarks);
	DDX_Check(pDX, IDC_GRAPH_MOUSECOORDS, m_MouseCoords);
	DDX_Check(pDX, IDC_GEN_GRAPH_PROPS_AXIS, m_ShowAxis);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGraphPropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CGraphPropertyPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphPropertyPage message handlers
