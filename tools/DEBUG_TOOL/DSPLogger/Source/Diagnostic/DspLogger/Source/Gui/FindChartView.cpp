// ChartView.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "FindChartView.h"

#include "FindDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//const int CFindChartView::X_SCALE_UNIT = 100;
//const int CFindChartView::Y_SCALE_UNIT = 60;
const int CFindChartView::X_SCALE_UNIT = 50;
const int CFindChartView::Y_SCALE_UNIT = 2;
const int CFindChartView::MARGIN       = 8;
/////////////////////////////////////////////////////////////////////////////
// CFindChartView

IMPLEMENT_DYNCREATE(CFindChartView, CScrollView)

CFindChartView::CFindChartView()
{	
    m_pSet = NULL;

	m_nCurCross   = -1;
	m_bUCAChange  = FALSE;
    m_nFindNum    = 0;
	m_pDoc        = NULL;
	m_bDrawTestPoint = FALSE;
	m_bDrawTogether  = FALSE;
	m_bUseSameY		 = FALSE;
	m_nCurChartTogether=1;
    memset(m_arrSigned , 0, sizeof(m_arrSigned));
    memset(m_arrByIndex, 0, sizeof(m_arrByIndex));
	memset(m_MaxMinData, 0, sizeof(m_MaxMinData));
	m_uLastTPCount = 0;

	for (int i=0 ; i < MAX_FIND_DATA_NUM ; ++i)
	{
		m_arrIndex[i].reserve(DRAW_MAX_POINTS);
	}

    
}//lint !e1401

CFindChartView::~CFindChartView()
{
    ClearFindInfo();
}

BEGIN_MESSAGE_MAP(CFindChartView, CScrollView)
	//{{AFX_MSG_MAP(CFindChartView)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindChartView drawing

void CFindChartView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

    m_pDoc = (CFindDoc *)GetDocument();
    _ASSERTE( m_pDoc != NULL );

    m_pSet = ((CMainFrame*)AfxGetMainWnd())->m_pDspKernel->GetTpSet();
    ASSERT(NULL != m_pSet);

    m_nXStart = MARGIN;
    m_nYStart = MARGIN;

    CRect rc;
    GetClientRect(rc);

	// TODO: calculate the total size of this view
    m_dwDefaultX = MIN_CX < rc.Width() ? rc.Width() : MIN_CX;
    m_dwDefaultY = MIN_CY < rc.Height() ? rc.Height() : MIN_CY;
   
    m_dwDefaultX -= (MARGIN + MARGIN + m_nXStart + 24);
    m_dwDefaultY -= (MARGIN + MARGIN + m_nYStart + 24);

    InitForDraw(DEFAULT_FIND_DATA_NUM);    
	if(m_pSet != NULL)
	{
		m_pSet->SetCurveNum(DEFAULT_FIND_DATA_NUM);
	}
    
    CalculateYOrigin();
}

void CFindChartView::OnDraw(CDC* pDC)
{	
	CRect rcClip;
    pDC->GetClipBox(rcClip);
	
    CBitmap * pOldBmp = NULL;
    CDC     * pDrawDC = pDC;
    CBitmap   bmp;
    CDC       dcMem;
    if( dcMem.CreateCompatibleDC(pDC) )
    {
        if( bmp.CreateCompatibleBitmap(pDC, rcClip.Width(), rcClip.Height()) )
        {
            dcMem.SetViewportOrg( -rcClip.left, -rcClip.top );
           
            pDrawDC = &dcMem;
            
            pOldBmp = dcMem.SelectObject(&bmp);
        }
    }

    static CBrush brhBk( DEFAULT_BAK_CLR );
    pDrawDC->FillRect(rcClip, &brhBk);

	if (m_bDrawTogether)
	{
		//draw graph together
			
		DrawCoordinate_Together( pDrawDC );
		DrawWave_Together( pDrawDC );
		DrawCross_Together( pDrawDC );
	}	
	else
	{
		//draw graph
	
		DrawCoordinate( pDrawDC );
		DrawWave( pDrawDC );
		DrawCross( pDrawDC );
	}
	
	if (m_bUCAChange == TRUE)
	{   
		//lock draw wnd
		LockWindowUpdate();
	}

    if (pDrawDC != pDC)
	{
		dcMem.SetViewportOrg(0, 0);
		pDC->BitBlt( rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(),
			pDrawDC, 0, 0, SRCCOPY );
		
		dcMem.SelectObject(pOldBmp);
        dcMem.DeleteDC();
        bmp.DeleteObject();   
	}

	//go to current Cross
	if (m_bUCAChange == TRUE)
	{
		m_bUCAChange=FALSE;
		OnGotoChartPoint( m_nCurCross );
		//unlock draw wnd
		UnlockWindowUpdate();

	}


}

/////////////////////////////////////////////////////////////////////////////
// CFindChartView diagnostics

#ifdef _DEBUG
void CFindChartView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CFindChartView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFindChartView message handlers
void CFindChartView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	if( pSender != NULL && lHint == WM_UPDATE_FIND  )							
    {
        ClearFindInfo();
        LockWindowUpdate();
        FindItems();
        CalculateYOrigin();
		FindMaxMin();
        Invalidate(FALSE);
        UnlockWindowUpdate();
    }	

    if (pSender !=NULL && lHint == WM_AUTO_REFRESH)
    {
        LockWindowUpdate();
        FindUpdate();
        CalculateYOrigin();
        FindMaxMin();
        Invalidate(FALSE);
        UnlockWindowUpdate();
    }

	if(lHint == WM_UPDATE_CLEAR_STATE)
	{
		m_uLastTPCount = 0;
    
		for( int i = 0; i < m_nFindNum; ++i )
		{
			m_pDoc->m_arrFindData[i].dwReserve1 = 0;
		}
	}
        
    if( pSender != NULL && lHint == WM_UPDATE_CHART )
    {
        PUC_HINT puch = (PUC_HINT)pHint;		
		OnUpdateChartView(puch->eAct, puch->lParam, puch->nIdx);		
    }

    if( pSender != NULL && lHint == WM_GOTO_ITEM_BY_FIND )
    {
        PUC_HINT puch = (PUC_HINT)pHint;
        OnGotoChartPoint( puch->wParam );
    }

    if (lHint== WM_UPDATE_CLEAR)
    {
		for(int i = 0; i < m_nFindNum; i++)
		{
			m_pDoc->m_arrFindData[i].dwFindCount = 0;
		}
        ClearFindInfo();
		
        Invalidate(FALSE);		
    }

	if (pSender != NULL && lHint == WM_SAVE_CURRENT_TP)
	{
		SaveCurTP();
	}
    
}

void CFindChartView::OnUpdateChartView( int nAct, LPARAM lParam, int nIdx )
{
    BOOL bUpdate = FALSE;
    switch(nAct)
    {
    case UCA_X_SCALE:
        {
			bUpdate = (m_nXScale != lParam);
			m_nXScale = lParam;
			
			m_dwX = m_dwDefaultX + (m_nXScale-1) * X_SCALE_UNIT;
			CSize siz;
			siz.cx = m_nXStart + m_dwX + MARGIN + MARGIN;
			siz.cy = m_nYStart + m_dwY + MARGIN + MARGIN;
			
			SetScrollSizes(MM_TEXT, siz);
			
			m_bUCAChange=TRUE;
        }
        
        break;
    case UCA_Y_SCALE:
        {
            int nDelta = lParam - m_arrYScale[nIdx];
            bUpdate = (nDelta != 0);

            CSize siz;
            siz.cx = m_nXStart + m_dwX + MARGIN + MARGIN;
            siz.cy = m_nYStart + m_dwY + nDelta * Y_SCALE_UNIT;
            m_dwY = siz.cy;
            SetScrollSizes(MM_TEXT, siz);            
			m_arrYScale[nIdx] = lParam;
            CalculateYOrigin();

		    m_bUCAChange=TRUE;
        }
        break;
    case UCA_UNSIGNED:
        bUpdate = m_arrSigned[nIdx];
        m_arrSigned[nIdx] = FALSE;
        CalculateYOrigin();
		
		m_bUCAChange=TRUE;
        break;
    case UCA_SIGNED:
        bUpdate = !m_arrSigned[nIdx];
        m_arrSigned[nIdx] = TRUE;
        CalculateYOrigin();
        FindMaxMin(); //add by xiaoguang.feng

		m_bUCAChange=TRUE;
        break;
    case UCA_BYTIME:
        bUpdate = m_arrByIndex[nIdx];
        m_arrByIndex[nIdx] = FALSE;

		m_bUCAChange=TRUE;
		break;
    case UCA_BYINDEX:
        bUpdate = !m_arrByIndex[nIdx];
        m_arrByIndex[nIdx] = TRUE;
		
		m_bUCAChange=TRUE;
        break;
    case UCA_DRAWTP:		
		bUpdate = (m_bDrawTestPoint!=(BOOL)lParam);
		m_bDrawTestPoint=(BOOL)lParam;
	
        break;
    case UCA_DRAWTOGETHER:		
		bUpdate = (m_bDrawTogether!=(BOOL)lParam);
		m_bDrawTogether=(BOOL)lParam;
        CalculateYOrigin();
		
		m_bUCAChange=TRUE;				
        break;
    case UCA_RESET:		
		SetScrollPos(SB_HORZ, 0);
		SetScrollPos(SB_VERT, 0);
		bUpdate=TRUE;				
        break;
		
	case UCA_DRAWONSAMEY:
		bUpdate = (m_bUseSameY!=(BOOL)lParam);
		m_bUseSameY=(BOOL)lParam;		
		break;
		
	case UCA_CHANGE_CURVE_NUM:
		InitForDraw(nIdx);
		m_pDoc->SetCurveNum(nIdx);
		m_pSet->SetCurveNum(nIdx);
		break;
    
    default:
        _ASSERTE( 0 );
        break;
    }

    if( bUpdate )
    {
        Invalidate(FALSE);
    }
}

BOOL CFindChartView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= WS_CLIPCHILDREN;

    HCURSOR hCursor = g_theApp.LoadCursor(IDC_CUR_ARROW);
    _ASSERTE( hCursor != NULL );

    cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS, hCursor, NULL, NULL);

	return CScrollView::PreCreateWindow(cs);
}

BOOL CFindChartView::OnEraseBkgnd(CDC* pDC) 
{
	UNREFERENCED_PARAMETER(pDC);
	//CScrollView::OnEraseBkgnd(pDC);
	return TRUE;
}

void CFindChartView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CPoint pt = GetScrollPosition();
    pt += point;	
	GotoFindItem( &pt );	
	CScrollView::OnLButtonDown(nFlags, point);
}

void CFindChartView::SaveCurTP()
{
    if(m_pDoc->m_nFindDataNum > 0 )
    {
		
		CFileDialog filedlg( FALSE, _T("log"), 
			NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			_T("dsplogger files (*.log)|*.log|"), NULL );
		
		if( filedlg.DoModal() != IDOK )
		{
			return;
		}		
		CString strLogFile = filedlg.GetPathName();

		FILE* pf = fopen( strLogFile,"w" );
		if( NULL == pf )
		{
			// Could not open file
			return ;
		}
		for (int i=0 ; i < MAX_FIND_DATA_NUM ; ++i)
		{
			VEC_FIND vecTp, vecTime;
			GetTPandTime(i,vecTp,vecTime);
			if( vecTp.size() == 0 )
			{
				continue;
			}
			LONGLONG count = vecTp.size();
			for(LONGLONG j = 0;j < count; j++)
			{
				fprintf(pf,"%04X    %04X    %d\n",GETTPADDR(vecTp[j]),GETTPDATA(vecTp[j]),vecTime[j]);
			}
		}
		
		fclose( pf );		
	}
}



void CFindChartView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default 
	int nCurPos = GetScrollPos(SB_HORZ) ;
	int nPrevPos = nCurPos;

	int nMax = GetScrollLimit(SB_HORZ);
	CRect rt;
	GetClientRect(&rt);
	TRACE("------1--------CurPos: %d\n", nCurPos);
	TRACE("---------------Max: %d\n", nMax);
	TRACE("---------------Width: %d\n", rt.Width());
	TRACE("---------------Pos: %d\n", nPos);

	int nStep = nMax / rt.Width();

	if(nStep <= 0)
	{
		nStep = 10;
	}

	BOOL bThum = FALSE;

	
	switch(nSBCode)
	{
	case SB_LEFT:			
		nCurPos = 0;								
		break;
	case SB_RIGHT:			
		nCurPos = GetScrollLimit(SB_HORZ)-1;								
		break;
	case SB_LINELEFT:		
		nCurPos = max(nCurPos - nStep, 0);							
		break;
	case SB_LINERIGHT:		
		nCurPos = min(nCurPos + nStep, nMax - 1);
		break;
	case SB_PAGELEFT:	
		nCurPos = max(nCurPos - rt.Width(), 0);								
		break;
	case SB_PAGERIGHT:		
		nCurPos = min(nCurPos + rt.Width(), nMax - 1);
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:  
		nCurPos = nPos;	
		bThum = TRUE;
		break;
	case SB_ENDSCROLL:
		break;
	}
	
	if(!bThum)
	{
		SetScrollPos(SB_HORZ, nCurPos);
		ScrollWindow(nPrevPos - nCurPos, 0) ;
	}
	else
	{
		CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
	}

	
//	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}
