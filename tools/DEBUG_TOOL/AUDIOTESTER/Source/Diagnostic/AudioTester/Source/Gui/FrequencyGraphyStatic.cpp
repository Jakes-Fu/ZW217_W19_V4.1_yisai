// FrequencyGraphyStatic.cpp : implementation file
//

#include "stdafx.h"
#include "Audiotester.h"
#include "FirAutoDlg.h"
#include "FirAutoDlgEx.h"
#include "FrequencyGraphyStatic.h"
#include "BMPCtrlWithSpecRect.h"

#include<math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define LEFT_EDGE_WITH 40
#define RIGHT_EDGE_WITH 10
#define BOTTOM_EDGE_WITH 10
#define TOP_EDGE_WITH 10
#define INVALID_VALUE 0xFFFF
/////////////////////////////////////////////////////////////////////////////
// CFrequencyGraphyStatic
#define Y_AXIS_OFFSET 20
//#define Y_AXIS_MIN_DB -18
//#define Y_AXIS_MAX_DB 18
CFrequencyGraphyStatic::CFrequencyGraphyStatic()
:m_iXCoefficient(1.0),m_iYCoefficient(1.0),m_iYMax(18),m_iYMin(-18),m_pDataSource(NULL)
,m_iXAxisLength(0),m_iYAxisLength(0),m_pOldBitmap(NULL),m_pBKBitmap(NULL)
{
    extern int g_nYOffSize;
    m_iYMin += g_nYOffSize;
    m_iYMax += g_nYOffSize;
}

CFrequencyGraphyStatic::~CFrequencyGraphyStatic()
{
    if(m_memDC.GetSafeHdc() != NULL)
    {
        m_memDC.SelectObject(m_pOldBitmap);
        if(m_pBKBitmap != NULL)
            delete m_pBKBitmap;
    }   
}


BEGIN_MESSAGE_MAP(CFrequencyGraphyStatic, CStatic)
	//{{AFX_MSG_MAP(CFrequencyGraphyStatic)
	ON_WM_PAINT()	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrequencyGraphyStatic message handlers

void CFrequencyGraphyStatic::OnPaint() 
{
	//CPaintDC dc(this); // device context for painting
    CRect rect;
    this->GetClientRect( rect );
    m_ptOrg.x = rect.left + LEFT_EDGE_WITH;
    m_ptOrg.y = rect.bottom - BOTTOM_EDGE_WITH;
    CPaintDC paintdc(this);
    if(m_memDC.GetSafeHdc() != NULL)
        paintdc.BitBlt(0,0,rect.Width(),rect.Height(),&m_memDC,0,0,SRCCOPY);
    CClientDC dc( this );
    dc.SelectObject(this->GetFont());
    DrawCoordinateAxis( &dc );
    DrawFreqCurve( &dc );
}

// draw x,y axis and its scale
void CFrequencyGraphyStatic::DrawCoordinateAxis(CDC *pDC)
{     
    CRect rect;
    this->GetClientRect( rect );
    rect.right *= (long)m_iXCoefficient;
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
                    (log10( SpecialRectOfCoefficient::m_dMaxXValue) -log10( SpecialRectOfCoefficient::m_dMinXValue)) ;
    for( int i=0; i<X_SCALE_COUNT_FIR; i+=2 )
    {        
        int iXScale = (int)((log10( SpecialRectOfCoefficient::m_iXScale[i] ) -log10( SpecialRectOfCoefficient::m_dMinXValue) )
            * dXScale);
        xScale.x = m_ptOrg.x + iXScale;
        CPoint ptXTextPos( xScale );
        pDC->MoveTo( xScale );    
        if( i % 4 == 0 )
        {
            ptXTextPos.Offset(-8,5);
        }
        else
        {
            ptXTextPos.Offset(2,-10);
        }
        
        CString strXText;
        strXText.Format(_T("%d"),SpecialRectOfCoefficient::m_iXScale[i] );
        pDC->TextOut(ptXTextPos.x, ptXTextPos.y,strXText );

        
        CPoint ptDot(xScale);
        ptDot.Offset(0,(int)(-m_iYAxisLength-10));
        pDC->LineTo( ptDot );
        
        
        
        pDC->MoveTo( xScale );    
        pDC->LineTo( xScale.x,xScale.y - 5 );
        pDC->MoveTo( xScale );    
        pDC->LineTo( xScale.x,xScale.y + 5 );
        //xScale.Offset(m_iXAxisLength/X_SCALE_COUNT_FIR,0 );    
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
#define Y_LINE_COUNT 18
    //double dYValueStep = dMaxYValue / 4;
    //double dYValueStep = m_iYAxisLength / ((m_iYMax - m_iYMin)/2 -0.00000001);
	double dYValueStep = (m_iYMax - m_iYMin) / (double)Y_LINE_COUNT;
	int iYValue = m_iYMin;
    //for( i=0; i<(m_iYMax - m_iYMin)/dYValueStep +1; ++i )
	for( i=0; i<Y_LINE_COUNT+1; ++i )
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
		strYValue.Format(_T("%d"), iYValue );
        pDC->TextOut(xScale.x-LEFT_EDGE_WITH+12, xScale.y-5 ,strYValue);
		iYValue = ( i + 1 ) * dYValueStep + m_iYMin;
        xScale.y = m_ptOrg.y - (double)( iYValue - m_iYMin ) / ( m_iYMax - m_iYMin ) * m_iYAxisLength;
    }        

    pDC->SelectObject( pOldBrush );
}

CPoint CFrequencyGraphyStatic::ConvertCurvePtToPt(double x, double y)
{
    double dYScale = m_iYAxisLength / (m_iYMax - m_iYMin );  
	double dxMin = log10( SpecialRectOfCoefficient::m_dMinXValue );
	double dxMax = log10( SpecialRectOfCoefficient::m_dMaxXValue );
    double dXScale = m_iXAxisLength / ( dxMax - dxMin );
    
    double dYValue = y;
    dYValue -= m_iYMin;
    if( dYValue < 0.0 )
    {
        dYValue = 0.0;
    }
    else if( dYValue > m_iYMax - m_iYMin )
    {
        dYValue = m_iYMax - m_iYMin;
    }
    CPoint pt((int)( ( log10(x) - dxMin )* dXScale) , (int)( dYValue * dYScale ) );
    if( pt.y < 0 )
    {
        pt.y =  m_ptOrg.y;
    }
    pt.x += m_ptOrg.x;
    pt.y = m_ptOrg.y - pt.y;

	if( pt.y < m_ptYMaxScalePoint.y )
	{
		pt.y = m_ptYMaxScalePoint.y ;
	}

	return pt;
}

//redraw the background
void CFrequencyGraphyStatic::EraseBK()
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
// draw frequence curve
void CFrequencyGraphyStatic::DrawFreqCurve(CClientDC *pDC)
{
    ASSERT( m_pDataSource );   
    
    CPen bluePen( PS_SOLID,0, RGB(0,0,255) );

    CGdiObject* pOldPen = pDC->SelectObject( &bluePen );
 
    double dYScale = m_iYAxisLength / (m_iYMax - m_iYMin );  

    double dXScale = m_iXAxisLength / (log10( SpecialRectOfCoefficient::m_dMaxXValue) 
                    -log10( SpecialRectOfCoefficient::m_dMinXValue)) ;
    
    const std::vector<SpecialRectOfCoefficient::FreqItem>& vAllFreqItems = m_pDataSource->GetFreqItem();
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
    CPoint ptFirst((int)((vAllFreqItems[0].first -log10( SpecialRectOfCoefficient::m_dMinXValue) )* dXScale ), (int)(dYValue * dYScale)  );
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
      
        CPoint ptSecond((int)((vAllFreqItems[i].first -log10( SpecialRectOfCoefficient::m_dMinXValue) )* dXScale) , (int)(dYValue * dYScale) );   
            
        ptSecond.x += m_ptOrg.x;
        ptSecond.y = m_ptOrg.y - ptSecond.y;
        
        if( ptSecond.y < m_ptYMaxScalePoint.y )
        {
            ptSecond.y = m_ptYMaxScalePoint.y ;
        }
        //TRACE("\n%d,%d\n",ptSecond.x,ptSecond.y );
        pDC->LineTo( ptSecond );
    }    
    pDC->SelectObject( pOldPen );
}


/////////////////////////////////////////////////////////////////////////////
// CFrequencyGraphyStaticX
CFrequencyGraphyStaticX::CFrequencyGraphyStaticX()
:m_iXCoefficient(1.0),m_iYCoefficient(1.0),m_iYMax(18),m_iYMin(-18),m_pDataSource(NULL)
,m_iXAxisLength(0),m_iYAxisLength(0),m_pOldBitmap(NULL),m_pBKBitmap(NULL)
{
    extern int g_nYOffSize;
    m_iYMin += g_nYOffSize;
    m_iYMax += g_nYOffSize;
}

CFrequencyGraphyStaticX::~CFrequencyGraphyStaticX()
{
    if(m_memDC.GetSafeHdc() != NULL)
    {
        m_memDC.SelectObject(m_pOldBitmap);
        if(m_pBKBitmap != NULL)
            delete m_pBKBitmap;
    }   
}


BEGIN_MESSAGE_MAP(CFrequencyGraphyStaticX, CStatic)
	//{{AFX_MSG_MAP(CFrequencyGraphyStatic)
	ON_WM_PAINT()	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrequencyGraphyStatic message handlers

void CFrequencyGraphyStaticX::OnPaint() 
{
	//CPaintDC dc(this); // device context for painting
    CRect rect;
    this->GetClientRect( rect );
    m_ptOrg.x = rect.left + LEFT_EDGE_WITH;
    m_ptOrg.y = rect.bottom - BOTTOM_EDGE_WITH;
    CPaintDC paintdc(this);
    if(m_memDC.GetSafeHdc() != NULL)
        paintdc.BitBlt(0,0,rect.Width(),rect.Height(),&m_memDC,0,0,SRCCOPY);
    CClientDC dc( this );
    dc.SelectObject(this->GetFont());
    DrawCoordinateAxis( &dc );
    DrawFreqCurve( &dc );
}

// draw x,y axis and its scale
void CFrequencyGraphyStaticX::DrawCoordinateAxis(CDC *pDC)
{     
    CRect rect;
    this->GetClientRect( rect );
    rect.right *= (long)m_iXCoefficient;
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
                    (log10( SpecialRectOfCoefficientEx::m_dMaxXValue) -log10( SpecialRectOfCoefficientEx::m_dMinXValue)) ;
    for( int i=0; i<X_SCALE_COUNT_FIR_EX; i+=2 )
    {        
        int iXScale = (int)((log10( SpecialRectOfCoefficientEx::m_iXScale[i] ) -log10( SpecialRectOfCoefficientEx::m_dMinXValue) )
            * dXScale);
        xScale.x = m_ptOrg.x + iXScale;
        CPoint ptXTextPos( xScale );
        pDC->MoveTo( xScale );    
        if( i % 4 == 0 )
        {
            ptXTextPos.Offset(-8,5);
        }
        else
        {
            ptXTextPos.Offset(2,-10);
        }
        
        CString strXText;
        strXText.Format(_T("%d"),SpecialRectOfCoefficientEx::m_iXScale[i] );
        pDC->TextOut(ptXTextPos.x, ptXTextPos.y,strXText );

        
        CPoint ptDot(xScale);
        ptDot.Offset(0,(int)(-m_iYAxisLength-10));
        pDC->LineTo( ptDot );
        
        
        
        pDC->MoveTo( xScale );    
        pDC->LineTo( xScale.x,xScale.y - 5 );
        pDC->MoveTo( xScale );    
        pDC->LineTo( xScale.x,xScale.y + 5 );
        //xScale.Offset(m_iXAxisLength/X_SCALE_COUNT_FIR,0 );    
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
#define Y_LINE_COUNT 18
    //double dYValueStep = dMaxYValue / 4;
    //double dYValueStep = m_iYAxisLength / ((m_iYMax - m_iYMin)/2 -0.00000001);
	double dYValueStep = (m_iYMax - m_iYMin) / (double)Y_LINE_COUNT;
	int iYValue = m_iYMin;
    //for( i=0; i<(m_iYMax - m_iYMin)/dYValueStep +1; ++i )
	for( i=0; i<Y_LINE_COUNT+1; ++i )
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
		strYValue.Format(_T("%d"), iYValue );
        pDC->TextOut(xScale.x-LEFT_EDGE_WITH+12, xScale.y-5 ,strYValue);
		iYValue = ( i + 1 ) * dYValueStep + m_iYMin;
        xScale.y = m_ptOrg.y - (double)( iYValue - m_iYMin ) / ( m_iYMax - m_iYMin ) * m_iYAxisLength;
    }        

    pDC->SelectObject( pOldBrush );
}

CPoint CFrequencyGraphyStaticX::ConvertCurvePtToPt(double x, double y)
{
    double dYScale = m_iYAxisLength / (m_iYMax - m_iYMin );  
	double dxMin = log10( SpecialRectOfCoefficientEx::m_dMinXValue );
	double dxMax = log10( SpecialRectOfCoefficientEx::m_dMaxXValue );
    double dXScale = m_iXAxisLength / ( dxMax - dxMin );
    
    double dYValue = y;
    dYValue -= m_iYMin;
    if( dYValue < 0.0 )
    {
        dYValue = 0.0;
    }
    else if( dYValue > m_iYMax - m_iYMin )
    {
        dYValue = m_iYMax - m_iYMin;
    }
    CPoint pt((int)( ( log10(x) - dxMin )* dXScale) , (int)( dYValue * dYScale ) );
    if( pt.y < 0 )
    {
        pt.y =  m_ptOrg.y;
    }
    pt.x += m_ptOrg.x;
    pt.y = m_ptOrg.y - pt.y;

	if( pt.y < m_ptYMaxScalePoint.y )
	{
		pt.y = m_ptYMaxScalePoint.y ;
	}

	return pt;
}

//redraw the background
void CFrequencyGraphyStaticX::EraseBK()
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
// draw frequence curve
void CFrequencyGraphyStaticX::DrawFreqCurve(CClientDC *pDC)
{
    ASSERT( m_pDataSource );   
    
    CPen bluePen( PS_SOLID,0, RGB(0,0,255) );

    CGdiObject* pOldPen = pDC->SelectObject( &bluePen );
 
    double dYScale = m_iYAxisLength / (m_iYMax - m_iYMin );  

    double dXScale = m_iXAxisLength / (log10( SpecialRectOfCoefficientEx::m_dMaxXValue) 
                    -log10( SpecialRectOfCoefficientEx::m_dMinXValue)) ;
    
    const std::vector<SpecialRectOfCoefficientEx::FreqItem>& vAllFreqItems = m_pDataSource->GetFreqItem();
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
    CPoint ptFirst((int)((vAllFreqItems[0].first -log10( SpecialRectOfCoefficientEx::m_dMinXValue) )* dXScale ), (int)(dYValue * dYScale)  );
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
      
        CPoint ptSecond((int)((vAllFreqItems[i].first -log10( SpecialRectOfCoefficientEx::m_dMinXValue) )* dXScale) , (int)(dYValue * dYScale) );   
            
        ptSecond.x += m_ptOrg.x;
        ptSecond.y = m_ptOrg.y - ptSecond.y;
        
        if( ptSecond.y < m_ptYMaxScalePoint.y )
        {
            ptSecond.y = m_ptYMaxScalePoint.y ;
        }
        //TRACE("\n%d,%d\n",ptSecond.x,ptSecond.y );
        pDC->LineTo( ptSecond );
    }    
    pDC->SelectObject( pOldPen );
}

/////////////////////////////////////////////////////////////////////////////
// CFrequencyGraphyStaticEx

const double CFrequencyGraphyStaticEx::PI = 3.1415926;
const double CFrequencyGraphyStaticEx::m_dMinXValue = 100.0;
const double CFrequencyGraphyStaticEx::m_dMaxXValue = 4000.0;

int CFrequencyGraphyStaticEx::m_iXScale[X_SCALE_COUNT_FIR] = 
{ 200   , 224   , 250   , 281   , 315   , 355   , 400   , 447   , 500   , 561   , 630   , 710   , 800   , 894   , 1000   , 1118   , 1250   , 1424   , 1580   , 1789   , 2000   , 2236   , 2500   , 2806   , 3150   , 3520 } ;


CFrequencyGraphyStaticEx::CFrequencyGraphyStaticEx()
:m_iXCoefficient(1.0),m_iYCoefficient(1.0),m_iYMax(30),m_iYMin(-30)
,m_iXAxisLength(0),m_iYAxisLength(0),m_pOldBitmap(NULL),m_pBKBitmap(NULL)
{
    extern int g_nYOffSize;
    m_iYMin += g_nYOffSize;
    m_iYMax += g_nYOffSize;
	m_pDataSource = NULL;
	m_nTarIndex = -1;
	m_tarPt.x = 0;
	m_tarPt.y = 0;
	m_vTarMovePt.clear();
	m_bCrossCursor = FALSE;
	//teana hu 2011.02.17
	m_nOriYMax1 = 0;
	m_nOriYMax2 = 0;
}

CFrequencyGraphyStaticEx::~CFrequencyGraphyStaticEx()
{
    if(m_memDC.GetSafeHdc() != NULL)
    {
        m_memDC.SelectObject(m_pOldBitmap);
        if(m_pBKBitmap != NULL)
            delete m_pBKBitmap;
    }  
}


BEGIN_MESSAGE_MAP(CFrequencyGraphyStaticEx, CStatic)
	//{{AFX_MSG_MAP(CFrequencyGraphyStaticEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()	
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrequencyGraphyStaticEx message handlers

void CFrequencyGraphyStaticEx::OnPaint() 
{
	//CPaintDC dc(this); // device context for painting
    CRect rect;
    this->GetClientRect( rect );
    m_ptOrg.x = rect.left + LEFT_EDGE_WITH;
    m_ptOrg.y = rect.bottom - BOTTOM_EDGE_WITH;
	rect.bottom += 6;
    rect.right += 6;

    CPaintDC paintdc(this);
    if(m_memDC.GetSafeHdc() != NULL)
        paintdc.BitBlt(0,0,rect.Width(),rect.Height(),&m_memDC,0,0,SRCCOPY);
    CClientDC dc( this );
    dc.SelectObject(this->GetFont());

    EraseBK();

	dc.FillSolidRect(&rect,RGB(212,208,200));

    DrawCoordinateAxis( &dc );
    
	if(m_pDataSource->m_bAuto)
	{
		DrawOriFreqCurve( &dc );
	}
	else
	{
		if(m_pDataSource->m_nLoadOriginalFlag != NO_LOAD_ORG)
		{	
			CPoint pt;
			pt = GetOriMaxY();
			int nHigYValue = m_pDataSource->GetHighEQYValue(pt.x);
			if(nHigYValue != INVALID_VALUE)
			{
				int nDiff = pt.y - nHigYValue;
				m_pDataSource->ModifySpecFreqValue(nDiff);
			}
			DrawOriFreqCurve( &dc );
		}
	}
    DrawTarFreqCurve( &dc );
    DrawRealFreqCurve( &dc );
    DrawModifiedFreqCurve( &dc );
    DrawHighFreqCurve( &dc );
    DrawLowFreqCurve( &dc );
    DrawFinalFreqCurve( &dc );

	//teana hu 2009.09.04
//	DrawMoveTarPoint(&dc);
}

// draw x,y axis and its scale
void CFrequencyGraphyStaticEx::DrawCoordinateAxis(CClientDC *pDC)
{     
    CRect rect;
    this->GetClientRect( rect );
    rect.right *= (long)m_iXCoefficient;
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
                    (log10( m_dMaxXValue) -log10( m_dMinXValue)) ;
    for( int i=0; i<X_SCALE_COUNT_FIR; i+=2 )
    {        
        int iXScale = (int)((log10( m_iXScale[i] ) -log10( m_dMinXValue) )
            * dXScale);
        xScale.x = m_ptOrg.x + iXScale;
        CPoint ptXTextPos( xScale );
        pDC->MoveTo( xScale );    
        if( i % 4 == 0 )
        {
            ptXTextPos.Offset(-8,5);
        }
        else
        {
            ptXTextPos.Offset(2,-10);
        }
        
        CString strXText;
        strXText.Format(_T("%d"),m_iXScale[i] );
        pDC->TextOut(ptXTextPos.x, ptXTextPos.y,strXText );

        
        CPoint ptDot(xScale);
        ptDot.Offset(0,(int)(-m_iYAxisLength-10));
        pDC->LineTo( ptDot );
        
        
        
        pDC->MoveTo( xScale );    
        pDC->LineTo( xScale.x,xScale.y - 5 );
        pDC->MoveTo( xScale );    
        pDC->LineTo( xScale.x,xScale.y + 5 );
        //xScale.Offset(m_iXAxisLength/X_SCALE_COUNT_FIR,0 );    
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
    //double dYValueStep = m_iYAxisLength / ((m_iYMax - m_iYMin)/2 -0.00000001);
	double dYValueStep = (m_iYMax - m_iYMin) / (double)Y_LINE_COUNT;
	int iYValue = m_iYMin;
    //for( i=0; i<(m_iYMax - m_iYMin)/2 +1; ++i )
	for( i=0; i<Y_LINE_COUNT+1; ++i )
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
//         strYValue.Format(_T("%d"), (i*2)+m_iYMin );
//         pDC->TextOut(xScale.x-LEFT_EDGE_WITH+12, xScale.y-5 ,strYValue);
//         xScale.Offset(0,(int)(-dYValueStep)); 
		
		strYValue.Format(_T("%d"), iYValue );
        pDC->TextOut(xScale.x-LEFT_EDGE_WITH+12, xScale.y-5 ,strYValue);
		iYValue = ( i + 1 ) * dYValueStep + m_iYMin;
        xScale.y = m_ptOrg.y - (double)( iYValue - m_iYMin ) / ( m_iYMax - m_iYMin ) * m_iYAxisLength;
        
    }        
    pDC->SelectObject( pOldBrush );
}
//redraw the background
void CFrequencyGraphyStaticEx::EraseBK()
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
//    Invalidate();  
}

// draw frequence curve
void CFrequencyGraphyStaticEx::DrawFreqCurve(CClientDC *pDC,CPen& pen,std::vector<FreqItem>& vAllFreqItems)
{
    CGdiObject* pOldPen = pDC->SelectObject( &pen );
 
    double dYScale = m_iYAxisLength / (m_iYMax - m_iYMin );    
	
    double dXScale = m_iXAxisLength / (log10( m_dMaxXValue) 
                    -log10( m_dMinXValue)) ;
    
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
    CPoint ptFirst((int)((vAllFreqItems[0].first -log10( m_dMinXValue) )* dXScale) , (int)(dYValue * dYScale ) );
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
      
        CPoint ptSecond((int)((vAllFreqItems[i].first -log10( m_dMinXValue) )* dXScale) , (int)(dYValue * dYScale) );   
            
        ptSecond.x += m_ptOrg.x;
        ptSecond.y = m_ptOrg.y - ptSecond.y;
        
        if( ptSecond.y < m_ptYMaxScalePoint.y )
        {
            ptSecond.y = m_ptYMaxScalePoint.y ;
        }
        //TRACE("\n%d,%d\n",ptSecond.x,ptSecond.y );
        pDC->LineTo( ptSecond );
    }    
    pDC->SelectObject( pOldPen );
}

void CFrequencyGraphyStaticEx::DrawOriFreqCurve( CClientDC* pDC )
{
    if( !m_pDataSource->m_bOriReady || !m_pDataSource->m_bShowOri )
    {
        return;
    }

    std::vector<FreqItem> vAllFreqItems;
    m_pDataSource->GetOriEq( &vAllFreqItems );
    CPen GreenPen( PS_SOLID,0, RGB(0,127,0) );

    DrawFreqCurve( pDC,GreenPen,vAllFreqItems );  
}

void CFrequencyGraphyStaticEx::DrawTarFreqCurve( CClientDC* pDC )
{
    if( !m_pDataSource->m_bTarReady || !m_pDataSource->m_bShowTar )
    {
        return;
    }

//    std::vector<FreqItem> vAllFreqItems;
	//teana hu 2009.09.04
	m_vTarFreqItems.clear();
    m_pDataSource->GetTarEq( &m_vTarFreqItems );
    CPen yellowPen( PS_SOLID,0, RGB(255,255,0) );
    
    DrawFreqCurve( pDC,yellowPen,m_vTarFreqItems );    
}

void CFrequencyGraphyStaticEx::DrawRealFreqCurve( CClientDC* pDC )
{
    if( !m_pDataSource->m_bReady || !m_pDataSource->m_bShowReal )
    {
        return;
    }

    std::vector<FreqItem> vAllFreqItems;
    m_pDataSource->GetRealEq( &vAllFreqItems );
    CPen bluePen( PS_SOLID,0, RGB(0,0,255) );
    
    DrawFreqCurve( pDC,bluePen,vAllFreqItems );    
}

void CFrequencyGraphyStaticEx::DrawModifiedFreqCurve( CClientDC* pDC )
{
    if( !m_pDataSource->m_bReady || !m_pDataSource->m_bShowModified )
    {
        return;
    }
    
    std::vector<FreqItem> vAllFreqItems;
    m_pDataSource->GetModifiedEq( &vAllFreqItems );
    CPen bluePen( PS_SOLID,0, RGB(127,0,0) );
    
    DrawFreqCurve( pDC,bluePen,vAllFreqItems );    
}

void CFrequencyGraphyStaticEx::DrawHighFreqCurve( CClientDC* pDC )
{
    if( !m_pDataSource->m_bSpecReady || !m_pDataSource->m_bShowSpec )
    {
        return;
    }
    
    std::vector<FreqItem> vAllFreqItems;
    m_pDataSource->GetHighEq( &vAllFreqItems );
    CPen bluePen( PS_SOLID,0, RGB(255,0,255) );
    
    DrawFreqCurve( pDC,bluePen,vAllFreqItems );        
}

void CFrequencyGraphyStaticEx::DrawLowFreqCurve( CClientDC* pDC )
{
    if( !m_pDataSource->m_bSpecReady || !m_pDataSource->m_bShowSpec )
    {
        return;
    }
    
    std::vector<FreqItem> vAllFreqItems;
    m_pDataSource->GetLowEq( &vAllFreqItems );
    CPen bluePen( PS_SOLID,0, RGB(255,0,255) );
    
    DrawFreqCurve( pDC,bluePen,vAllFreqItems );    
}

void CFrequencyGraphyStaticEx::DrawFinalFreqCurve( CClientDC* pDC )
{
    if( !m_pDataSource->m_bReady || !m_pDataSource->m_bShowFinal )
    {
        return;
    }
    
    std::vector<FreqItem> vAllFreqItems;
    m_pDataSource->GetFinalEq( &vAllFreqItems );
    CPen bluePen( PS_SOLID,0, RGB(0,0,127) );
    
    DrawFreqCurve( pDC,bluePen,vAllFreqItems );    
}

CPoint CFrequencyGraphyStaticEx::ConvertCurvePtToPt(double x, double y)
{
    double dYScale = m_iYAxisLength / (m_iYMax - m_iYMin );    
    double dXScale = m_iXAxisLength / (log10( m_dMaxXValue) 
                    -log10( m_dMinXValue)) ;
    
    double dYValue = y;
    dYValue -= m_iYMin;
    if( dYValue < 0.0 )
    {
        dYValue = 0.0;
    }
    else if( dYValue > m_iYMax - m_iYMin )
    {
        dYValue = m_iYMax - m_iYMin;
    }
    CPoint pt((int)((x -log10( m_dMinXValue) )* dXScale) , (int)(dYValue * dYScale ) );
    if( pt.y < 0 )
    {
        pt.y =  m_ptOrg.y;
    }
    pt.x += m_ptOrg.x;
    pt.y = m_ptOrg.y - pt.y;

	if( pt.y < m_ptYMaxScalePoint.y )
	{
		pt.y = m_ptYMaxScalePoint.y ;
	}

	return pt;
}

void CFrequencyGraphyStaticEx::DrawMoveTarPoint( CClientDC* pDC )
{
	CPen yellowPen( PS_SOLID,5, RGB(255,255,0) );

	CGdiObject* pOldPen = pDC->SelectObject( &yellowPen );  
	int nCount = m_vTarMovePt.size();
	for(int i = 0; i < nCount; i++)
	{
		CPoint pt(m_vTarMovePt[i].first, m_vTarMovePt[i].second);
		pDC->MoveTo( pt );
		pDC->LineTo( pt );
	}
    pDC->SelectObject( pOldPen );

}

void CFrequencyGraphyStaticEx::OnLButtonDown(UINT nFlags,  CPoint point) 
{ 
	int nCount = m_vTarFreqItems.size();
	m_nTarIndex = -1;
	m_bCrossCursor = FALSE;

	for(int i = 0; i < nCount; i++)
	{
		CPoint pt = ConvertCurvePtToPt(m_vTarFreqItems[i].first, m_vTarFreqItems[i].second);

		if(fabs(point.x - pt.x) <= 5 && fabs(point.y - pt.y) <= 5)
		{
			m_nTarIndex = i;
			m_bCrossCursor = TRUE;
			::SetCursor(LoadCursor(NULL, IDC_CROSS));
			m_tarPt.x = point.x;
			m_tarPt.y = point.y;
			break;
		}
	}

     CStatic::OnLButtonDown(nFlags, point);
}

void CFrequencyGraphyStaticEx::OnLButtonUp(UINT nFlags,  CPoint point) 
{
	if(m_nTarIndex != -1)
	{
		m_bCrossCursor = FALSE;
		::SetCursor(LoadCursor(NULL, IDC_ARROW));

		int nDiff = point.y - m_tarPt.y;
		double dDiff = ((double)nDiff) / (m_iYAxisLength / (m_iYMax - m_iYMin ));
		m_vTarFreqItems[m_nTarIndex].second -= dDiff;

		if(m_pDataSource != NULL)
		{
			m_pDataSource->ResetTarEq(m_nTarIndex, m_vTarFreqItems[m_nTarIndex].second);
		}

		FreqItem fi;
        fi.first = point.x;
        fi.second = point.y;
        m_vTarMovePt.push_back( fi );

		Invalidate();
		
	}
    CStatic::OnLButtonUp(nFlags, point);
}

void CFrequencyGraphyStaticEx::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bCrossCursor)
	{
		::SetCursor(LoadCursor(NULL, IDC_CROSS));
	}
	else
	{
		::SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
	CStatic::OnMouseMove(nFlags, point);
}

CPoint CFrequencyGraphyStaticEx::GetOriMaxY()
{
	std::vector<FreqItem> vAllFreqItems;
	m_pDataSource->GetOriEq( &vAllFreqItems );

	int nSize = vAllFreqItems.size();
	int nMaxValue = -1000;
	CPoint pt;

	for( int i=0;i<nSize;i++ )
	{
		int nValue = vAllFreqItems[i].second;
		if(nValue > nMaxValue)
		{
			nMaxValue = nValue;
			pt.y = nValue;
			pt.x = vAllFreqItems[i].first;
		}
	}

	return pt;
}

/////////////////////////////////////////////////////////////////////////////
// CFrequencyGraphyStaticEx2
#define RADIUS 3
#define SELECT_RADIUS 5

BEGIN_MESSAGE_MAP(CFrequencyGraphyStaticEx2, CFrequencyGraphyStatic)
	//{{AFX_MSG_MAP(CFrequencyGraphyStatic)
	ON_WM_PAINT()	
	ON_WM_LBUTTONDOWN()	
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CFrequencyGraphyStaticEx2::CFrequencyGraphyStaticEx2()
{
	//m_iYMin = -66;
	//m_iYMax = 19;
	m_iYMin = -5;
	m_iYMax = 5;
	m_nSel = -1;

	m_pRelateWnd = NULL;
}

void CFrequencyGraphyStaticEx2::DrawFreqLine( CDC* pDC )
{
    CPen bluePen( PS_SOLID,0, RGB(127,0,0) );
	CGdiObject* pOldPen = pDC->SelectObject( &bluePen );
	POINT pt = ConvertCurvePtToPt( CFrequencyGraphyStaticEx::m_iXScale[0],m_arrPowerData[0] );
	pDC->MoveTo( pt );
	
	for( int i=0;i<X_SCALE_COUNT_FIR;i++ )
	{
		POINT pt = ConvertCurvePtToPt( CFrequencyGraphyStaticEx::m_iXScale[i],m_arrPowerData[i] );
		pDC->LineTo( pt );
		int radius = RADIUS;
		if( i == m_nSel )
		{
			radius = SELECT_RADIUS;
		}
		pDC->Ellipse( pt.x - radius,pt.y - radius,pt.x + radius,pt.y + radius );
	}
	pDC->SelectObject( pOldPen );
}

void CFrequencyGraphyStaticEx2::OnPaint() 
{
	//CPaintDC dc(this); // device context for painting
    CRect rect;
    this->GetClientRect( rect );
    m_ptOrg.x = rect.left + LEFT_EDGE_WITH;
    m_ptOrg.y = rect.bottom - BOTTOM_EDGE_WITH;

	rect.bottom += 6;
    rect.right += 6;

    CPaintDC paintdc(this);
    CClientDC dc( this );
    if(m_memDC.GetSafeHdc() == NULL)
    {
        //CRect rect;
        //this->GetClientRect( rect );
        //rect.bottom += 6;
        //rect.right += 6;
        
        //CClientDC dc( this );
        m_memDC.CreateCompatibleDC(&dc);
        if((m_pBKBitmap = new CBitmap()) == NULL) return ;
        m_pBKBitmap->CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
        m_pOldBitmap = m_memDC.SelectObject(m_pBKBitmap);        
        m_memDC.SetBkMode(TRANSPARENT);
	}

	//draw background 
    m_memDC.FillSolidRect(&rect,RGB(212,208,200));
    
    m_memDC.SelectObject(this->GetFont());
    DrawCoordinateAxis( &m_memDC );
    DrawFreqLine( &m_memDC );

	dc.BitBlt(0,0,rect.Width(),rect.Height(),&m_memDC,0,0,SRCCOPY);
}

void CFrequencyGraphyStaticEx2::OnLButtonDown(UINT nFlags,  CPoint point) 
{ 
	//teana hu 2011.02.18
// 	if(((CFrequencyGraphyStaticEx *)m_pRelateWnd)->m_pDataSource->m_nLoadOriginalFlag == NO_LOAD_ORG)
// 	{
// 		return;
// 	}
	//
	for(int i = 0; i < X_SCALE_COUNT_FIR; i++)
	{
		CPoint pt = ConvertCurvePtToPt( CFrequencyGraphyStaticEx::m_iXScale[i],m_arrPowerData[i] );

		if(fabs(point.x - pt.x) <= RADIUS && fabs(point.y - pt.y) <= RADIUS)
		{
			m_nSel = i;
			EraseBK();
			//Invalidate();
			break;
		}
	}

	//teana hu 2011.03.04
	((CFrequencyGraphyStaticEx *)m_pRelateWnd)->m_pDataSource->SetFirListHighLight(m_nSel);
	//
    CStatic::OnLButtonDown(nFlags, point);
}

void CFrequencyGraphyStaticEx2::OnLButtonUp(UINT nFlags,  CPoint point) 
{
	if( -1 != m_nSel )
	{
		m_nSel = -1;
		
		// Redraw
	    for( int i=0;i<X_SCALE_COUNT_FIR;i++ )
		{
			m_pDataSource->m_vPo.m_vPoValue[i] = m_arrPowerData[i];
		}
		if(((CFrequencyGraphyStaticEx *)m_pRelateWnd)->m_pDataSource->m_nLoadOriginalFlag != NO_LOAD_ORG)
		{
			((CFrequencyGraphyStaticEx *)m_pRelateWnd)->m_pDataSource->m_nLoadOriginalFlag = MODIFY_ORG;
		}
		((CFrequencyGraphyStaticEx *)m_pRelateWnd)->m_pDataSource->m_bDefaultFir = FALSE;
		m_pDataSource->CalculateCoefficients();
		double dX[65] = {0};
		((CFrequencyGraphyStaticEx *)m_pRelateWnd)->m_pDataSource->GetOriEqXValue( dX );
		m_pDataSource->CalculateFreqGraphyDataEx(dX,65);

		if( m_pRelateWnd )
		{
			// Redraw coefficient curve
			m_pRelateWnd->Invalidate();
			((CFrequencyGraphyStaticEx *)m_pRelateWnd)->m_pDataSource->SetListData();
		}

	}
    CStatic::OnLButtonUp(nFlags, point);
}

void CFrequencyGraphyStaticEx2::OnMouseMove(UINT nFlags, CPoint point)
{
	if( -1 != m_nSel )
	{
		// Drag a point
		m_arrPowerData[m_nSel] = (double)( m_ptOrg.y - point.y ) / m_iYAxisLength * (m_iYMax - m_iYMin ) + m_iYMin; 
		Invalidate();
	}

	CStatic::OnMouseMove(nFlags, point);
}

BOOL CFrequencyGraphyStaticEx2::OnEraseBkgnd( CDC* /*pDC*/ )
{
	// No need to erase background.
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFrequencyGraphyStaticExX

const double CFrequencyGraphyStaticExX::PI = 3.1415926;
const double CFrequencyGraphyStaticExX::m_dMinXValue = 100.0;
const double CFrequencyGraphyStaticExX::m_dMaxXValue = 4000.0;

int CFrequencyGraphyStaticExX::m_iXScale[X_SCALE_COUNT_FIR_EX] = 
{ 
	100, 106, 112, 119, 126, 133, 141, 150, 159, 168, 178, 189, 200, 
	212, 224 ,238, 252, 267, 283, 300, 318, 336, 356, 378, 400,
	424, 449, 476, 504, 534, 566, 599, 635, 673, 713, 755, 800,
	848, 898, 951, 1008, 1068, 1131, 1199, 1270, 1345, 1425, 1510, 1600,
	1695, 1796, 1903, 2016, 2136, 2263, 2397, 2540, 2691, 2851, 3020, 3200,
	3390, 3592, 3806, 4000
};

CFrequencyGraphyStaticExX::CFrequencyGraphyStaticExX()
:m_iXCoefficient(1.0),m_iYCoefficient(1.0),m_iYMax(30),m_iYMin(-30)
,m_iXAxisLength(0),m_iYAxisLength(0),m_pOldBitmap(NULL),m_pBKBitmap(NULL)
{
    extern int g_nYOffSize;
    m_iYMin += g_nYOffSize;
    m_iYMax += g_nYOffSize;
	m_pDataSource = NULL;
	m_nTarIndex = -1;
	m_tarPt.x = 0;
	m_tarPt.y = 0;
	m_vTarMovePt.clear();
	m_bCrossCursor = FALSE;
	//teana hu 2011.02.17
	m_nOriYMax1 = 0;
	m_nOriYMax2 = 0;
}

CFrequencyGraphyStaticExX::~CFrequencyGraphyStaticExX()
{
    if(m_memDC.GetSafeHdc() != NULL)
    {
        m_memDC.SelectObject(m_pOldBitmap);
        if(m_pBKBitmap != NULL)
            delete m_pBKBitmap;
    }  
}


BEGIN_MESSAGE_MAP(CFrequencyGraphyStaticExX, CStatic)
	//{{AFX_MSG_MAP(CFrequencyGraphyStaticEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()	
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrequencyGraphyStaticExX message handlers

void CFrequencyGraphyStaticExX::OnPaint() 
{
	//CPaintDC dc(this); // device context for painting
    CRect rect;
    this->GetClientRect( rect );
    m_ptOrg.x = rect.left + LEFT_EDGE_WITH;
    m_ptOrg.y = rect.bottom - BOTTOM_EDGE_WITH;
	rect.bottom += 6;
    rect.right += 6;

    CPaintDC paintdc(this);
    if(m_memDC.GetSafeHdc() != NULL)
        paintdc.BitBlt(0,0,rect.Width(),rect.Height(),&m_memDC,0,0,SRCCOPY);
    CClientDC dc( this );
    dc.SelectObject(this->GetFont());

    EraseBK();

	dc.FillSolidRect(&rect,RGB(212,208,200));

    DrawCoordinateAxis( &dc );
    
	if(m_pDataSource->m_bAuto)
	{
		DrawOriFreqCurve( &dc );
	}
	else
	{
		if(m_pDataSource->m_nLoadOriginalFlag != NO_LOAD_ORG)
		{	
			CPoint pt;
			pt = GetOriMaxY();
			int nHigYValue = m_pDataSource->GetHighEQYValue(pt.x);
			if(nHigYValue != INVALID_VALUE)
			{
				int nDiff = pt.y - nHigYValue;
				m_pDataSource->ModifySpecFreqValue(nDiff);
			}
			DrawOriFreqCurve( &dc );
		}
	}
    DrawTarFreqCurve( &dc );
    DrawRealFreqCurve( &dc );
    DrawModifiedFreqCurve( &dc );
    DrawHighFreqCurve( &dc );
    DrawLowFreqCurve( &dc );
    DrawFinalFreqCurve( &dc );

	//teana hu 2009.09.04
//	DrawMoveTarPoint(&dc);
}

// draw x,y axis and its scale
void CFrequencyGraphyStaticExX::DrawCoordinateAxis(CClientDC *pDC)
{     
    CRect rect;
    this->GetClientRect( rect );
    rect.right *= (long)m_iXCoefficient;
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
                    (log10( m_dMaxXValue) -log10( m_dMinXValue)) ;
    for( int i=0; i<X_SCALE_COUNT_FIR_EX; i+=2 )
    {        
        int iXScale = (int)((log10( m_iXScale[i] ) -log10( m_dMinXValue) )
            * dXScale);
        xScale.x = m_ptOrg.x + iXScale;
        CPoint ptXTextPos( xScale );
        pDC->MoveTo( xScale );    
        if( i % 4 == 0 )
        {
            ptXTextPos.Offset(-8,5);
        }
        else
        {
            ptXTextPos.Offset(2,-10);
        }
        
        CString strXText;
        strXText.Format(_T("%d"),m_iXScale[i] );
        pDC->TextOut(ptXTextPos.x, ptXTextPos.y,strXText );

        
        CPoint ptDot(xScale);
        ptDot.Offset(0,(int)(-m_iYAxisLength-10));
        pDC->LineTo( ptDot );
        
        
        
        pDC->MoveTo( xScale );    
        pDC->LineTo( xScale.x,xScale.y - 5 );
        pDC->MoveTo( xScale );    
        pDC->LineTo( xScale.x,xScale.y + 5 );
        //xScale.Offset(m_iXAxisLength/X_SCALE_COUNT_FIR,0 );    
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
    //double dYValueStep = m_iYAxisLength / ((m_iYMax - m_iYMin)/2 -0.00000001);
	double dYValueStep = (m_iYMax - m_iYMin) / (double)Y_LINE_COUNT;
	int iYValue = m_iYMin;
    //for( i=0; i<(m_iYMax - m_iYMin)/2 +1; ++i )
	for( i=0; i<Y_LINE_COUNT+1; ++i )
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
//         strYValue.Format(_T("%d"), (i*2)+m_iYMin );
//         pDC->TextOut(xScale.x-LEFT_EDGE_WITH+12, xScale.y-5 ,strYValue);
//         xScale.Offset(0,(int)(-dYValueStep)); 
		
		strYValue.Format(_T("%d"), iYValue );
        pDC->TextOut(xScale.x-LEFT_EDGE_WITH+12, xScale.y-5 ,strYValue);
		iYValue = ( i + 1 ) * dYValueStep + m_iYMin;
        xScale.y = m_ptOrg.y - (double)( iYValue - m_iYMin ) / ( m_iYMax - m_iYMin ) * m_iYAxisLength;
        
    }        
    pDC->SelectObject( pOldBrush );
}
//redraw the background
void CFrequencyGraphyStaticExX::EraseBK()
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
//    Invalidate();  
}

// draw frequence curve
void CFrequencyGraphyStaticExX::DrawFreqCurve(CClientDC *pDC,CPen& pen,std::vector<FreqItem>& vAllFreqItems)
{
    CGdiObject* pOldPen = pDC->SelectObject( &pen );
 
    double dYScale = m_iYAxisLength / (m_iYMax - m_iYMin );    
	
    double dXScale = m_iXAxisLength / (log10( m_dMaxXValue) 
                    -log10( m_dMinXValue)) ;
    
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
    CPoint ptFirst((int)((vAllFreqItems[0].first -log10( m_dMinXValue) )* dXScale) , (int)(dYValue * dYScale ) );
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
      
        CPoint ptSecond((int)((vAllFreqItems[i].first -log10( m_dMinXValue) )* dXScale) , (int)(dYValue * dYScale) );   
            
        ptSecond.x += m_ptOrg.x;
        ptSecond.y = m_ptOrg.y - ptSecond.y;
        
        if( ptSecond.y < m_ptYMaxScalePoint.y )
        {
            ptSecond.y = m_ptYMaxScalePoint.y ;
        }
        //TRACE("\n%d,%d\n",ptSecond.x,ptSecond.y );
        pDC->LineTo( ptSecond );
    }    
    pDC->SelectObject( pOldPen );
}

void CFrequencyGraphyStaticExX::DrawOriFreqCurve( CClientDC* pDC )
{
    if( !m_pDataSource->m_bOriReady || !m_pDataSource->m_bShowOri )
    {
        return;
    }

    std::vector<FreqItem> vAllFreqItems;
    m_pDataSource->GetOriEq( &vAllFreqItems );
    CPen GreenPen( PS_SOLID,0, RGB(0,127,0) );

    DrawFreqCurve( pDC,GreenPen,vAllFreqItems );  
}

void CFrequencyGraphyStaticExX::DrawTarFreqCurve( CClientDC* pDC )
{
    if( !m_pDataSource->m_bTarReady || !m_pDataSource->m_bShowTar )
    {
        return;
    }

//    std::vector<FreqItem> vAllFreqItems;
	//teana hu 2009.09.04
	m_vTarFreqItems.clear();
    m_pDataSource->GetTarEq( &m_vTarFreqItems );
    CPen yellowPen( PS_SOLID,0, RGB(255,255,0) );
    
    DrawFreqCurve( pDC,yellowPen,m_vTarFreqItems );    
}

void CFrequencyGraphyStaticExX::DrawRealFreqCurve( CClientDC* pDC )
{
    if( !m_pDataSource->m_bReady || !m_pDataSource->m_bShowReal )
    {
        return;
    }

    std::vector<FreqItem> vAllFreqItems;
    m_pDataSource->GetRealEq( &vAllFreqItems );
    CPen bluePen( PS_SOLID,0, RGB(0,0,255) );
    
    DrawFreqCurve( pDC,bluePen,vAllFreqItems );    
}

void CFrequencyGraphyStaticExX::DrawModifiedFreqCurve( CClientDC* pDC )
{
    if( !m_pDataSource->m_bReady || !m_pDataSource->m_bShowModified )
    {
        return;
    }
    
    std::vector<FreqItem> vAllFreqItems;
    m_pDataSource->GetModifiedEq( &vAllFreqItems );
    CPen bluePen( PS_SOLID,0, RGB(127,0,0) );
    
    DrawFreqCurve( pDC,bluePen,vAllFreqItems );    
}

void CFrequencyGraphyStaticExX::DrawHighFreqCurve( CClientDC* pDC )
{
    if( !m_pDataSource->m_bSpecReady || !m_pDataSource->m_bShowSpec )
    {
        return;
    }
    
    std::vector<FreqItem> vAllFreqItems;
    m_pDataSource->GetHighEq( &vAllFreqItems );
    CPen bluePen( PS_SOLID,0, RGB(255,0,255) );
    
    DrawFreqCurve( pDC,bluePen,vAllFreqItems );        
}

void CFrequencyGraphyStaticExX::DrawLowFreqCurve( CClientDC* pDC )
{
    if( !m_pDataSource->m_bSpecReady || !m_pDataSource->m_bShowSpec )
    {
        return;
    }
    
    std::vector<FreqItem> vAllFreqItems;
    m_pDataSource->GetLowEq( &vAllFreqItems );
    CPen bluePen( PS_SOLID,0, RGB(255,0,255) );
    
    DrawFreqCurve( pDC,bluePen,vAllFreqItems );    
}

void CFrequencyGraphyStaticExX::DrawFinalFreqCurve( CClientDC* pDC )
{
    if( !m_pDataSource->m_bReady || !m_pDataSource->m_bShowFinal )
    {
        return;
    }
    
    std::vector<FreqItem> vAllFreqItems;
    m_pDataSource->GetFinalEq( &vAllFreqItems );
    CPen bluePen( PS_SOLID,0, RGB(0,0,127) );
    
    DrawFreqCurve( pDC,bluePen,vAllFreqItems );    
}

CPoint CFrequencyGraphyStaticExX::ConvertCurvePtToPt(double x, double y)
{
    double dYScale = m_iYAxisLength / (m_iYMax - m_iYMin );    
    double dXScale = m_iXAxisLength / (log10( m_dMaxXValue) 
                    -log10( m_dMinXValue)) ;
    
    double dYValue = y;
    dYValue -= m_iYMin;
    if( dYValue < 0.0 )
    {
        dYValue = 0.0;
    }
    else if( dYValue > m_iYMax - m_iYMin )
    {
        dYValue = m_iYMax - m_iYMin;
    }
    CPoint pt((int)((x -log10( m_dMinXValue) )* dXScale) , (int)(dYValue * dYScale ) );
    if( pt.y < 0 )
    {
        pt.y =  m_ptOrg.y;
    }
    pt.x += m_ptOrg.x;
    pt.y = m_ptOrg.y - pt.y;

	if( pt.y < m_ptYMaxScalePoint.y )
	{
		pt.y = m_ptYMaxScalePoint.y ;
	}

	return pt;
}

void CFrequencyGraphyStaticExX::DrawMoveTarPoint( CClientDC* pDC )
{
	CPen yellowPen( PS_SOLID,5, RGB(255,255,0) );

	CGdiObject* pOldPen = pDC->SelectObject( &yellowPen );  
	int nCount = m_vTarMovePt.size();
	for(int i = 0; i < nCount; i++)
	{
		CPoint pt(m_vTarMovePt[i].first, m_vTarMovePt[i].second);
		pDC->MoveTo( pt );
		pDC->LineTo( pt );
	}
    pDC->SelectObject( pOldPen );

}

void CFrequencyGraphyStaticExX::OnLButtonDown(UINT nFlags,  CPoint point) 
{ 
	int nCount = m_vTarFreqItems.size();
	m_nTarIndex = -1;
	m_bCrossCursor = FALSE;

	for(int i = 0; i < nCount; i++)
	{
		CPoint pt = ConvertCurvePtToPt(m_vTarFreqItems[i].first, m_vTarFreqItems[i].second);

		if(fabs(point.x - pt.x) <= 5 && fabs(point.y - pt.y) <= 5)
		{
			m_nTarIndex = i;
			m_bCrossCursor = TRUE;
			::SetCursor(LoadCursor(NULL, IDC_CROSS));
			m_tarPt.x = point.x;
			m_tarPt.y = point.y;
			break;
		}
	}

     CStatic::OnLButtonDown(nFlags, point);
}

void CFrequencyGraphyStaticExX::OnLButtonUp(UINT nFlags,  CPoint point) 
{
	if(m_nTarIndex != -1)
	{
		m_bCrossCursor = FALSE;
		::SetCursor(LoadCursor(NULL, IDC_ARROW));

		int nDiff = point.y - m_tarPt.y;
		double dDiff = ((double)nDiff) / (m_iYAxisLength / (m_iYMax - m_iYMin ));
		m_vTarFreqItems[m_nTarIndex].second -= dDiff;

		if(m_pDataSource != NULL)
		{
			m_pDataSource->ResetTarEq(m_nTarIndex, m_vTarFreqItems[m_nTarIndex].second);
		}

		FreqItem fi;
        fi.first = point.x;
        fi.second = point.y;
        m_vTarMovePt.push_back( fi );

		Invalidate();
		
	}
    CStatic::OnLButtonUp(nFlags, point);
}

void CFrequencyGraphyStaticExX::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bCrossCursor)
	{
		::SetCursor(LoadCursor(NULL, IDC_CROSS));
	}
	else
	{
		::SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
	CStatic::OnMouseMove(nFlags, point);
}

CPoint CFrequencyGraphyStaticExX::GetOriMaxY()
{
	std::vector<FreqItem> vAllFreqItems;
	m_pDataSource->GetOriEq( &vAllFreqItems );

	int nSize = vAllFreqItems.size();
	int nMaxValue = -1000;
	CPoint pt;

	for( int i=0;i<nSize;i++ )
	{
		int nValue = vAllFreqItems[i].second;
		if(nValue > nMaxValue)
		{
			nMaxValue = nValue;
			pt.y = nValue;
			pt.x = vAllFreqItems[i].first;
		}
	}

	return pt;
}

/////////////////////////////////////////////////////////////////////////////
// CFrequencyGraphyStaticEx3
int CFrequencyGraphyStaticEx3::m_iXScale[X_SCALE_COUNT_FIR_EX] = 
{ 100, 106, 112, 119, 126, 133, 141, 150, 159, 168, 178, 189, 200, 
212, 224 ,238, 252, 267, 283, 300, 318, 336, 356, 378, 400,
424, 449, 476, 504, 534, 566, 599, 635, 673, 713, 755, 800,
848, 898, 951, 1008, 1068, 1131, 1199, 1270, 1345, 1425, 1510, 1600,
1695, 1796, 1903, 2016, 2136, 2263, 2397, 2540, 2691, 2851, 3020, 3200,
3390, 3592, 3806, 4000};

BEGIN_MESSAGE_MAP(CFrequencyGraphyStaticEx3, CFrequencyGraphyStaticX)
	//{{AFX_MSG_MAP(CFrequencyGraphyStatic)
	ON_WM_PAINT()	
	ON_WM_LBUTTONDOWN()	
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CFrequencyGraphyStaticEx3::CFrequencyGraphyStaticEx3()
{
	//m_iYMin = -66;
	//m_iYMax = 19;
	m_iYMin = -5;
	m_iYMax = 5;
	m_nSel = -1;

	m_pRelateWnd = NULL;
}

void CFrequencyGraphyStaticEx3::DrawFreqLine( CDC* pDC )
{
    CPen bluePen( PS_SOLID,0, RGB(127,0,0) );
	CGdiObject* pOldPen = pDC->SelectObject( &bluePen );
	POINT pt = ConvertCurvePtToPt( m_iXScale[0],m_arrPowerData[0] );
	pDC->MoveTo( pt );
	
	for( int i=0;i<X_SCALE_COUNT_FIR_EX;i++ )
	{
		POINT pt = ConvertCurvePtToPt( m_iXScale[i],m_arrPowerData[i] );
		pDC->LineTo( pt );
		int radius = RADIUS;
		if( i == m_nSel )
		{
			radius = SELECT_RADIUS;
		}
		pDC->Ellipse( pt.x - radius,pt.y - radius,pt.x + radius,pt.y + radius );
	}
	pDC->SelectObject( pOldPen );
}

void CFrequencyGraphyStaticEx3::OnPaint() 
{
	//CPaintDC dc(this); // device context for painting
    CRect rect;
    this->GetClientRect( rect );
    m_ptOrg.x = rect.left + LEFT_EDGE_WITH;
    m_ptOrg.y = rect.bottom - BOTTOM_EDGE_WITH;

	rect.bottom += 6;
    rect.right += 6;

    CPaintDC paintdc(this);
    CClientDC dc( this );
    if(m_memDC.GetSafeHdc() == NULL)
    {
        //CRect rect;
        //this->GetClientRect( rect );
        //rect.bottom += 6;
        //rect.right += 6;
        
        //CClientDC dc( this );
        m_memDC.CreateCompatibleDC(&dc);
        if((m_pBKBitmap = new CBitmap()) == NULL) return ;
        m_pBKBitmap->CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
        m_pOldBitmap = m_memDC.SelectObject(m_pBKBitmap);        
        m_memDC.SetBkMode(TRANSPARENT);
	}

	//draw background 
    m_memDC.FillSolidRect(&rect,RGB(212,208,200));
    
    m_memDC.SelectObject(this->GetFont());
    DrawCoordinateAxis( &m_memDC );
    DrawFreqLine( &m_memDC );

	dc.BitBlt(0,0,rect.Width(),rect.Height(),&m_memDC,0,0,SRCCOPY);
}

void CFrequencyGraphyStaticEx3::OnLButtonDown(UINT nFlags,  CPoint point) 
{ 
	for(int i = 0; i < X_SCALE_COUNT_FIR_EX; i++)
	{
		CPoint pt = ConvertCurvePtToPt( m_iXScale[i],m_arrPowerData[i] );

		if(fabs(point.x - pt.x) <= RADIUS && fabs(point.y - pt.y) <= RADIUS)
		{
			m_nSel = i;
			EraseBK();
			//Invalidate();
			break;
		}
	}

	//teana hu 2011.03.04
	((CFrequencyGraphyStaticExX *)m_pRelateWnd)->m_pDataSource->SetFirListHighLight(m_nSel);
	//
    CStatic::OnLButtonDown(nFlags, point);
}

void CFrequencyGraphyStaticEx3::OnLButtonUp(UINT nFlags,  CPoint point) 
{
	if( -1 != m_nSel )
	{
		m_nSel = -1;
		
		// Redraw
	    for( int i=0;i<X_SCALE_COUNT_FIR_EX;i++ )
		{
			m_pDataSource->m_vPoValue[i] = m_arrPowerData[i];
		}
		if(((CFrequencyGraphyStaticExX *)m_pRelateWnd)->m_pDataSource->m_nLoadOriginalFlag != NO_LOAD_ORG)
		{
			((CFrequencyGraphyStaticExX *)m_pRelateWnd)->m_pDataSource->m_nLoadOriginalFlag = MODIFY_ORG;
		}
		((CFrequencyGraphyStaticExX *)m_pRelateWnd)->m_pDataSource->m_bDefaultFir = FALSE;
		m_pDataSource->CalculateCoefficients();
		double dX[65] = {0};
		((CFrequencyGraphyStaticExX *)m_pRelateWnd)->m_pDataSource->GetOriEqXValue( dX );
		m_pDataSource->CalculateFreqGraphyDataEx(dX,65);

		if( m_pRelateWnd )
		{
			// Redraw coefficient curve
			m_pRelateWnd->Invalidate();
			((CFrequencyGraphyStaticExX *)m_pRelateWnd)->m_pDataSource->SetListData();
		}

	}
    CStatic::OnLButtonUp(nFlags, point);
}

void CFrequencyGraphyStaticEx3::OnMouseMove(UINT nFlags, CPoint point)
{
	if( -1 != m_nSel )
	{
		// Drag a point
		m_arrPowerData[m_nSel] = (double)( m_ptOrg.y - point.y ) / m_iYAxisLength * (m_iYMax - m_iYMin ) + m_iYMin; 
		Invalidate();
	}

	CStatic::OnMouseMove(nFlags, point);
}

BOOL CFrequencyGraphyStaticEx3::OnEraseBkgnd( CDC* /*pDC*/ )
{
	// No need to erase background.
	return TRUE;
}