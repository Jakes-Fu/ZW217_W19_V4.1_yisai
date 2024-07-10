// EqGraphStatic.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "EqGraphStatic.h"

#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LEFT_EDGE_WITH 40
#define RIGHT_EDGE_WITH 10
#define BOTTOM_EDGE_WITH 10
#define TOP_EDGE_WITH 10

/////////////////////////////////////////////////////////////////////////////
// CEqGraphStatic

CEqGraphStatic::CEqGraphStatic()
{
    m_iYMax = 18;
    m_iYMin = -18;

    m_pDataSource = NULL;
    m_pBKBitmap = NULL;
    m_iXAxisLength = 0;
    m_iYAxisLength = 0;
    
    m_pOldBitmap = NULL;
}

CEqGraphStatic::~CEqGraphStatic()
{
    m_pDataSource = NULL;
    m_pOldBitmap = NULL;
    m_pBKBitmap = NULL;
}


BEGIN_MESSAGE_MAP(CEqGraphStatic, CStatic)
	//{{AFX_MSG_MAP(CEqGraphStatic)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEqGraphStatic message handlers
//redraw the background
void CEqGraphStatic::EraseBK()
{   
    if(m_memDC.GetSafeHdc() == NULL)
    {
        CRect rect;
        this->GetClientRect( rect );
        rect.bottom += 6;
        rect.right += 6;
        
        CClientDC dc( this );
        m_memDC.CreateCompatibleDC(&dc);
        if((m_pBKBitmap = new CBitmap()) == NULL) return ;
        m_pBKBitmap->CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
        m_pOldBitmap = m_memDC.SelectObject(m_pBKBitmap);        
        m_memDC.SetBkMode(TRANSPARENT);
        //draw background 
        m_memDC.FillSolidRect(&rect,RGB(212,208,200));
    }    
    //force to call onpaint
    Invalidate();  
}

void CEqGraphStatic::OnPaint() 
{
//	CPaintDC dc(this); // device context for painting
	
    CRect rect;
    this->GetClientRect( rect );
    m_ptOrg.x = rect.left + LEFT_EDGE_WITH;
    m_ptOrg.y = rect.bottom - BOTTOM_EDGE_WITH;
    CPaintDC paintdc(this);
    if(m_memDC.GetSafeHdc() != NULL)
    {
        paintdc.BitBlt(0,0,rect.Width(),rect.Height(),&m_memDC,0,0,SRCCOPY);
    }
    CClientDC dc( this );
    dc.SelectObject(this->GetFont());
    DrawCoordinateAxis( &dc );
    DrawFreqCurves( &dc );
}

void CEqGraphStatic::DrawFreqCurves( CClientDC* pDC )
{
    CPen bluePen( PS_SOLID,0, RGB(0,0,255) );
    CPen redPen( PS_SOLID,0, RGB(255,0,0) );
    CPen greenPen( PS_SOLID,0, RGB(0,255,0) );
    CPen yellowPen( PS_SOLID,0,RGB( 255,255,0 ) );

    const std::vector<SpecialRectOfEQ::FreqItem>& vAllBassFreqItems = m_pDataSource->GetBassFreqItem();
    DrawFreqCurve( pDC,&redPen,vAllBassFreqItems );

    const std::vector<SpecialRectOfEQ::FreqItem>& vAllTrebleFreqItems = m_pDataSource->GetTrebleFreqItem();
    DrawFreqCurve( pDC,&greenPen,vAllTrebleFreqItems );

    const std::vector<SpecialRectOfEQ::FreqItem>& vAllMidFreqItems = m_pDataSource->GetMidFreqItem();
    if( vAllMidFreqItems.size() != 0 )
    {
        DrawFreqCurve( pDC,&yellowPen,vAllMidFreqItems );
    }

    const std::vector<SpecialRectOfEQ::FreqItem>& vAllFreqItems = m_pDataSource->GetFreqItem();
    DrawFreqCurve( pDC,&bluePen,vAllFreqItems );
}

void CEqGraphStatic::DrawFreqCurve( CClientDC* pDC,CPen* pPen,const std::vector<SpecialRectOfEQ::FreqItem>& vAllFreqItems )
{
    CGdiObject* pOldPen = pDC->SelectObject( pPen );
    
    double dYScale = m_iYAxisLength / (m_iYMax - m_iYMin );    
    double dXScale = m_iXAxisLength / (log10( SpecialRectOfEQ::m_dMaxXValue) 
        -log10( SpecialRectOfEQ::m_dMinXValue)) ;
    
    int iSize = vAllFreqItems.size();
    
    if( iSize <=0 )
        return;
    double dYValue = vAllFreqItems[0].second ;
    dYValue -= m_iYMin;
    if( dYValue < 0.0 )
    {
        dYValue = 0.0;
    }
    else if( dYValue > m_iYMax - m_iYMin )
    {
        dYValue = m_iYMax - m_iYMin;
    }
    CPoint ptFirst((int)((vAllFreqItems[0].first -log10( SpecialRectOfEQ::m_dMinXValue) )* dXScale ), (int)(dYValue * dYScale)  );
    if( ptFirst.y < 0 )
    {
        ptFirst.y =  m_ptOrg.y;
    }
    ptFirst.x += m_ptOrg.x;
    ptFirst.y = m_ptOrg.y - ptFirst.y;
    pDC->MoveTo( ptFirst );
//    int iMaxY = 0;
    for( int i=1; i<iSize; ++i )
    {              
        dYValue = vAllFreqItems[i].second;
        dYValue -= m_iYMin;
        
        if( dYValue < 0.0 )
        {
            dYValue = 0.0;
        }
        else if( dYValue >m_iYMax - m_iYMin )
        {
            dYValue = m_iYMax - m_iYMin;
        }
        
        CPoint ptSecond((int)((vAllFreqItems[i].first -log10( SpecialRectOfEQ::m_dMinXValue) )* dXScale) , (int)(dYValue * dYScale) );   
        
        ptSecond.x += m_ptOrg.x;
        ptSecond.y = m_ptOrg.y - ptSecond.y;
        
        if( ptSecond.y < m_ptYMaxScalePoint.y )
        {
            ptSecond.y = m_ptYMaxScalePoint.y ;
        }
        pDC->LineTo( ptSecond );
    }    
    pDC->SelectObject( pOldPen );
}

void CEqGraphStatic::DrawCoordinateAxis( CClientDC* pDC )
{
    CRect rect;
    this->GetClientRect( rect );
    pDC->MoveTo(m_ptOrg);
    CPoint ptLine( m_ptOrg );
    
    CBrush redBrush(RGB(255,0,0));
    CBrush* pOldBrush = pDC->SelectObject( &redBrush );
    //x axis
    ptLine.Offset(rect.right - RIGHT_EDGE_WITH - LEFT_EDGE_WITH ,0 );
    pDC->LineTo( ptLine );   
#define ARROW_X_LENGTH 15
#define ARROW_X_WIDTH 3
#define SCALE_X_OFFSET 5
#define SCALE_Y_OFFSET 5
#define SCALE_Y_COUNT 19
    // x arrow
    CPoint arrow[3];
    arrow[0] = ptLine;
    ptLine.Offset(-ARROW_X_LENGTH,-ARROW_X_WIDTH );
    arrow[1] = ptLine;
    ptLine.Offset(0,ARROW_X_WIDTH*2 );
    arrow[2] = ptLine;
    pDC->Polygon(arrow,3 );
    
    pDC->SetBkMode(TRANSPARENT);
    ptLine =arrow[0];
    ptLine.Offset(3,-8 );
    pDC->TextOut(ptLine.x, ptLine.y, "f" );


    m_iXAxisLength = rect.right - RIGHT_EDGE_WITH - LEFT_EDGE_WITH - ARROW_X_LENGTH - SCALE_X_OFFSET;
    m_iYAxisLength = rect.bottom - BOTTOM_EDGE_WITH - TOP_EDGE_WITH - ARROW_X_LENGTH - SCALE_Y_OFFSET;
    CPoint xScale(m_ptOrg);

    CPen dotPen(PS_DOT,0,RGB(0,0,0));    
    CGdiObject* pOldPen =  pDC->SelectObject( &dotPen );
    //draw text in x axis
    double dXScale = m_iXAxisLength / 
                    (log10( SpecialRectOfEQ::m_dMaxXValue) -log10( SpecialRectOfEQ::m_dMinXValue)) ;
    for( int i=0; i<X_SCALE_COUNT; ++i )
    {        
        int ix =  (int)(SpecialRectOfEQ::m_dXScale[i] * SpecialRectOfEQ::m_dMaxXValue);
        int iXScale = (int)((log10(ix) -log10( SpecialRectOfEQ::m_dMinXValue) ) * dXScale);
        xScale.x = m_ptOrg.x + iXScale;
        CPoint ptXTextPos( xScale );
        pDC->MoveTo( xScale );    

        ptXTextPos.Offset(-8,5);
        
        CString strXText;
        strXText.Format(_T("%d"),ix);
        pDC->TextOut(ptXTextPos.x, ptXTextPos.y,strXText );

        CPoint ptDot(xScale);
        ptDot.Offset(0,(int)(-m_iYAxisLength-10));
        pDC->LineTo( ptDot );
        
        pDC->MoveTo( xScale );    
        pDC->LineTo( xScale.x,xScale.y - 5 );
        pDC->MoveTo( xScale );    
        pDC->LineTo( xScale.x,xScale.y + 5 );
        //xScale.Offset(m_iXAxisLength/X_SCALE_COUNT,0 );    
    }   
     pDC->SelectObject( pOldPen );
    
    //y axis
    ptLine = m_ptOrg;
    pDC->MoveTo( m_ptOrg );
    ptLine.Offset(0, -( rect.bottom - TOP_EDGE_WITH - BOTTOM_EDGE_WITH ) );
    pDC->LineTo( ptLine );

    //y Arrow
    arrow[0] = ptLine;
    ptLine.Offset(-3,15 );
    arrow[1] = ptLine;
    ptLine.Offset(6,0 );
    arrow[2] = ptLine;
    pDC->Polygon(arrow,3 );

    ptLine =arrow[0];
    ptLine.Offset(-8,-12 );
    pDC->TextOut(ptLine.x, ptLine.y, "db" );
    
    // text in y axis    
    xScale = m_ptOrg;
    double y = m_ptOrg.y;
    //double dYValueStep = dMaxYValue / 4;
    double dYValueStep = m_iYAxisLength / ((m_iYMax - m_iYMin)/2 -0.00000001);
    for( i=0; i<(m_iYMax - m_iYMin)/2 +1; ++i )
    {       
        pDC->MoveTo( xScale );    
        m_ptYMaxScalePoint= xScale;
        CGdiObject* pOldPen1 =  pDC->SelectObject( &dotPen );
        CPoint ptDot(xScale);
        ptDot.Offset((int)(m_iXAxisLength+10),0);
        pDC->LineTo( ptDot );
        pDC->SelectObject( pOldPen1 );
        
        pDC->MoveTo( xScale );
        pDC->LineTo( xScale.x -5 ,xScale.y );
        pDC->MoveTo( xScale );    
        pDC->LineTo( xScale.x + 5,xScale.y );

        //CPoint ptText(xScale );
        //ptText.Offset( -RIGHT_EDGE_WITH,0 );
        CString strYValue;
        strYValue.Format(_T("%d"), (i*2)+m_iYMin );
        pDC->TextOut(xScale.x-LEFT_EDGE_WITH+12, xScale.y-5 ,strYValue);
        y -= dYValueStep;
        xScale.y = (long)y;
        
    }        
    pDC->SelectObject( pOldBrush );
}

void CEqGraphStatic::OnDestroy() 
{
	CStatic::OnDestroy();
	
    if( m_pBKBitmap )
    {
        delete m_pBKBitmap;
        m_pBKBitmap = NULL;
    }
}
