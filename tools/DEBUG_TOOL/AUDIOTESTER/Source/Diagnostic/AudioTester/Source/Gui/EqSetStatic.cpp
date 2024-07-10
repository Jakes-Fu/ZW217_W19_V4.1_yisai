// EqSetStatic.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "EqSetStatic.h"
#include <math.h>
#include "ConvertFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEqSetStatic

CEqSetStatic::CEqSetStatic()
{
	m_iYMax = 18;
    m_iYMin = -72;
	
    m_pBKBitmap = NULL;
    m_iXAxisLength = 0;
    m_iYAxisLength = 0;
    
    m_pOldBitmap = NULL;
	
	m_nBandNum = 0;
	m_fMasterGain = 0;
	memset(&m_nChk, 0, sizeof(int) * 8);
	memset(&m_nCf, 0, sizeof(short) * 8);
	memset(&m_fWd, 0, sizeof(int) * 8);
	memset(&m_fRt, 0, sizeof(int) * 8);
	memset(&m_fGn, 0, sizeof(int) * 8);
	memset(&m_iXScale, 0, sizeof(int) * X_SCALE_MODE_COUNT);

	SetColorPen();
}

CEqSetStatic::~CEqSetStatic()
{
	m_pOldBitmap = NULL;
    m_pBKBitmap = NULL;
}


BEGIN_MESSAGE_MAP(CEqSetStatic, CStatic)
	//{{AFX_MSG_MAP(CEqSetStatic)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEqSetStatic message handlers

void CEqSetStatic::OnPaint() 
{
	//CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	m_dMaxXValue = m_nSampleRate / 2;
	CRect rect;
    this->GetClientRect( rect );
    m_ptOrg.x = rect.left + LEFT_EDGE_WITH;
    m_ptOrg.y = rect.bottom - BOTTOM_EDGE_WITH;

	CPaintDC paintdc(this);
    CClientDC dc( this );
    if(m_memDC.GetSafeHdc() == NULL)
    {
        m_memDC.CreateCompatibleDC(&dc);
        if((m_pBKBitmap = new CBitmap()) == NULL) return ;
        m_pBKBitmap->CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
        m_pOldBitmap = m_memDC.SelectObject(m_pBKBitmap);        
        m_memDC.SetBkMode(TRANSPARENT);
	}

    m_memDC.SelectObject(this->GetFont());
	m_memDC.FillSolidRect(&rect,RGB(0,0,0));
    DrawCoordinateAxis( &m_memDC );
    DrawFreqCurves( &m_memDC );

	dc.BitBlt(0,0,rect.Width(),rect.Height(),&m_memDC,0,0,SRCCOPY);
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CEqSetStatic::OnDestroy() 
{
	CStatic::OnDestroy();
	
	// TODO: Add your message handler code here
	if( m_pBKBitmap )
    {
        delete m_pBKBitmap;
        m_pBKBitmap = NULL;
    }
	
}

void CEqSetStatic::EraseBK()
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
        m_memDC.FillSolidRect(&rect,RGB(0,0,0));
    }    
    //force to call onpaint
    Invalidate(FALSE);  
}

void CEqSetStatic::DrawFreqCurves(CDC* pDC)
{
	CalculateFreqGraphyData();

	for(int i = 0; i < m_nBandNum; i++)
	{
		if( m_vAllBandFreqItem[i].size() != 0 )
		{
			DrawFreqCurve( pDC,&m_Pen[i],m_vAllBandFreqItem[i] );
		}
	}

	if( m_vAllFreqItem.size() != 0 )
	{
		DrawFreqCurve( pDC,&m_Pen[8],m_vAllFreqItem );
		DrawKeyPoint(pDC);
	}

}

void CEqSetStatic::DrawFreqCurve( CDC* pDC,CPen* pPen,const std::vector<FreqItem>& vAllFreqItems )
{
	CGdiObject* pOldPen = pDC->SelectObject( pPen );
    
    double dYScale = m_iYAxisLength / (m_iYMax - m_iYMin );    
    double dXScale = m_iXAxisLength / (log10(m_dMaxXValue) 
        -log10(m_dMinXValue)) ;
    
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
    CPoint ptFirst((int)((vAllFreqItems[0].first -log10(m_dMinXValue) )* dXScale ), (int)(dYValue * dYScale)  );
    if( ptFirst.y < 0 )
    {
        ptFirst.y =  m_ptOrg.y;
    }
    ptFirst.x += m_ptOrg.x;
    ptFirst.y = m_ptOrg.y - ptFirst.y;
    pDC->MoveTo( ptFirst );
	FreqItem itm;
	std::vector<FreqItem> tempPoint;
	itm.first = ptFirst.x;
	itm.second = ptFirst.y;
	tempPoint.push_back(itm);

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
        
        CPoint ptSecond((int)((vAllFreqItems[i].first -log10(m_dMinXValue) )* dXScale) , (int)(dYValue * dYScale) );   
        
        ptSecond.x += m_ptOrg.x;
        ptSecond.y = m_ptOrg.y - ptSecond.y;
        
        if( ptSecond.y < m_ptYMaxScalePoint.y )
        {
            ptSecond.y = m_ptYMaxScalePoint.y ;
        }
        pDC->LineTo( ptSecond );
		itm.first = ptSecond.x;
		itm.second = ptSecond.y;
		tempPoint.push_back(itm);
    } 
	pDC->SelectObject( pOldPen );

// 	CPen pen(PS_SOLID, 4, RGB(255,255,255));
// 	CGdiObject* pdPen = pDC->SelectObject( &pen );

// 	iSize = tempPoint.size();
// 	for(i = 0; i < iSize; i++)
// 	{
// 		for(int j = 1; j < X_SCALE_MODE_COUNT; j++)
// 		{
// 			if(tempPoint[i].first == m_iXScale[j])
// 			{
// 				CPoint pt;
// 				pt.x = m_iXScale[j];
// 				pt.y = tempPoint[i].second;
// 				pDC->MoveTo(pt);
// 				pDC->LineTo(pt);
// 				break;
// 			}
// 		}
// 	}
//	pDC->SelectObject( pdPen );
}

void CEqSetStatic::DrawKeyPoint(CDC* pDC)
{
	CPen pen(PS_SOLID, 5, RGB(255,255,255));
	CGdiObject* pOldPen = pDC->SelectObject( &pen );
    
    double dYScale = m_iYAxisLength / (m_iYMax - m_iYMin );    
    double dXScale = m_iXAxisLength / (log10(m_dMaxXValue) 
        -log10(m_dMinXValue)) ;
    
    double dYValue = 0;
	
    for( int i=0; i<m_nBandNum; ++i )
    {           
		if(m_nChk[i] == 0)
		{
			continue;
		}
        dYValue = m_fRt[i];
        dYValue -= m_iYMin;
        
        if( dYValue < 0.0 )
        {
            dYValue = 0.0;
        }
        else if( dYValue >m_iYMax - m_iYMin )
        {
            dYValue = m_iYMax - m_iYMin;
        }
        
        CPoint ptSecond((int)((log10(m_nCf[i]) -log10(m_dMinXValue) )* dXScale) , (int)(dYValue * dYScale) );   
        
        ptSecond.x += m_ptOrg.x;
        ptSecond.y = m_ptOrg.y - ptSecond.y;
        
        if( ptSecond.y < m_ptYMaxScalePoint.y )
        {
            ptSecond.y = m_ptYMaxScalePoint.y ;
        }
		pDC->MoveTo(ptSecond);
        pDC->LineTo( ptSecond );
    } 
	pDC->SelectObject( pOldPen );
}

void CEqSetStatic::DrawCoordinateAxis( CDC* pDC )
{
    CRect rect;
    this->GetClientRect( rect );
    pDC->MoveTo(m_ptOrg);
    CPoint ptLine( m_ptOrg );
    
    CBrush redBrush(RGB(255,0,0));
    CBrush* pOldBrush = pDC->SelectObject( &redBrush );
    //x axis
	CPen pen(PS_SOLID, 0, RGB(0,128,0));
	CGdiObject* pOldPen = pDC->SelectObject( &pen );

    ptLine.Offset(rect.right - RIGHT_EDGE_WITH - LEFT_EDGE_WITH ,0);
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
	pDC->SetTextColor(RGB(192,0,0));
    pDC->TextOut(ptLine.x -10, ptLine.y +10, "Hz" );


    m_iXAxisLength = rect.right - RIGHT_EDGE_WITH - LEFT_EDGE_WITH - ARROW_X_LENGTH - SCALE_X_OFFSET;
    m_iYAxisLength = rect.bottom - BOTTOM_EDGE_WITH - TOP_EDGE_WITH - ARROW_X_LENGTH - SCALE_Y_OFFSET;
	
    CPoint xScale(m_ptOrg);

    CPen dotPen(PS_DOT,0,RGB(192,192,192));    
    pOldPen =  pDC->SelectObject( &dotPen );

    //draw text in x axis
    double dXScale = m_iXAxisLength / 
                    (log10(m_dMaxXValue) -log10(m_dMinXValue)) ;
	
    for( int i=0; i< X_SCALE_MODE_COUNT; ++i )
    {        
        int ix =  (int)(m_dXScale[i] * m_dMaxXValue + 0.5);
		int iXScale =0;
		iXScale = (int)((log10(ix) -log10( m_dMinXValue) ) * dXScale);
        
        xScale.x = m_ptOrg.x + iXScale;
		m_iXScale[i] = xScale.x;
		if(xScale.x < m_ptOrg.x)
		{
			xScale.x = m_ptOrg.x;
		}
        CPoint ptXTextPos( xScale );
        pDC->MoveTo( xScale );    

        ptXTextPos.Offset(8,5);
        
        CString strXText;
        strXText.Format(_T("%d"),ix);
		//
		LOGFONT logfont;
		HFONT hFont, hOldFont;
		strcpy((LPSTR)logfont.lfFaceName,(LPSTR)"Courier");   
		logfont.lfWeight=12;   
		logfont.lfWidth=6;   
		logfont.lfHeight=15;   
		logfont.lfEscapement=-900;   
		logfont.lfUnderline=FALSE;   
		logfont.lfItalic=FALSE;   
		logfont.lfStrikeOut=FALSE;   
		logfont.lfCharSet=GB2312_CHARSET;   
		CFont myFont;
		myFont.CreateFontIndirect(&logfont);   
		hFont = myFont;   
		
		hOldFont=(HFONT__ *)pDC->SelectObject(hFont);   
		CRect rtText(ptXTextPos.x,ptXTextPos.y,ptXTextPos.x+20,ptXTextPos.y+60);
		
		//
		pDC->SetTextColor(RGB(192,0,0));
        pDC->TextOut(ptXTextPos.x, ptXTextPos.y,strXText );
		pDC->SelectObject(hOldFont);

        CPoint ptDot(xScale);
        ptDot.Offset(0,(int)(-m_iYAxisLength-10));
        pDC->LineTo( ptDot );
        
        pDC->MoveTo( xScale );    
        pDC->LineTo( xScale.x,xScale.y - 5 );
        pDC->MoveTo( xScale );    
        pDC->LineTo( xScale.x,xScale.y + 5 ); 
    }   
     pDC->SelectObject( pOldPen );
	 pOldPen = pDC->SelectObject( &pen );
    
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
	pDC->SetTextColor(RGB(192,0,0));
    pDC->TextOut(ptLine.x, ptLine.y, "dB" );
    
    // text in y axis    
    xScale = m_ptOrg;
    double y = m_ptOrg.y;
    double dYValueStep = m_iYAxisLength / ((m_iYMax - m_iYMin)/3 -0.00000001);
    for( i=0; i<(m_iYMax - m_iYMin)/3 +1; ++i )
    { 
		if(i % 2 == 0)
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

			CString strYValue;
			strYValue.Format(_T("%d"), (i*3)+m_iYMin );
			pDC->SetTextColor(RGB(192,0,0));
			pDC->TextOut(xScale.x-LEFT_EDGE_WITH, xScale.y-5 ,strYValue);
		}
		else
		{ 
			pDC->MoveTo( xScale );
			pDC->LineTo( xScale.x -3 ,xScale.y );
			pDC->MoveTo( xScale );    
			pDC->LineTo( xScale.x + 3,xScale.y );
		}
		
        y -= dYValueStep;
        xScale.y = (long)y;   
    }    
    pDC->SelectObject( pOldBrush );
}

void CEqSetStatic::Clear()
{
	m_vAllFreqItem.clear();
	for(int i =0; i < m_nBandNum; i++)
	{
		m_vAllBandFreqItem[i].clear();
	}
}

double CEqSetStatic::m_dMinXValue = 1;
double CEqSetStatic::m_dXScale[X_SCALE_MODE_COUNT] =
{
	0.00004535, 0.000907, 0.00227,   0.004535 , 0.00907   , 0.01814   , 0.0272   , 0.0363   , 0.04535   , 0.11337   , 0.1814   , 0.2721   , 0.72564    , 1
};

void CEqSetStatic::CalculateFreqGraphyData( int nXScaleCount /*=882*/ )
{

	for(int i =0; i < m_nBandNum; i++)
	{
		BOOL band_filter_on = (BOOL)m_nChk[i];
		
		IIR_FILTER_PARA_T filter_para_lcf;
		memset(&filter_para_lcf, 0, sizeof(IIR_FILTER_PARA_T));
		SHORT sgain = 0;
		Filter_CalcEQ_Para(band_filter_on, m_nCf[i], m_fWd[i]*512, m_fRt[i]*10, m_fGn[i]*10, m_nSampleRate, &filter_para_lcf, &sgain);
		m_vAllBandFreqItem[i].clear();
		double dx = pow( m_dMaxXValue / m_dMinXValue   , 1.0 / nXScaleCount );
		double x = m_dMinXValue;

		for( int j=0;j<nXScaleCount;j++ )
		{
			FreqItem tempItem;
			double treble   , bass   , dvalue = 0;
			tempItem.first = log10(x);
			
			dvalue = ::CalcFrequencyResponse(filter_para_lcf.A0,
				filter_para_lcf.A1,
				filter_para_lcf.A2,
				filter_para_lcf.B0,
				filter_para_lcf.B1,
				filter_para_lcf.B2,
				x,
				sgain);
			
			tempItem.second = dvalue;
			m_vAllBandFreqItem[i].push_back( tempItem );
			x *= dx;		
		}
	}
	m_vAllFreqItem.clear();
	for( i = 0; i < nXScaleCount; i++)
	{
		FreqItem allItem;
		for(int j = 0; j < m_nBandNum; j++)
		{
			allItem.first = m_vAllBandFreqItem[j].at(i).first;
			allItem.second += m_vAllBandFreqItem[j].at(i).second;
		}
		allItem.second += (m_fMasterGain+0.5);
		m_vAllFreqItem.push_back(allItem);
	}
}

void CEqSetStatic::SetValue(int nIndex, int nChk, short nCf, float fWd, float fRt, float fGn)
{
	m_nChk[nIndex] = nChk;
	m_nCf[nIndex] = nCf;
	m_fWd[nIndex] = fWd;
	m_fRt[nIndex] = fRt;
	m_fGn[nIndex] = fGn;
}

void CEqSetStatic::SetColorPen()
{
	m_cr[0] = RGB(255,255,0);
	m_cr[1] = RGB(255,0,0);
	m_cr[2] = RGB(128,0,255);
	m_cr[3] = RGB(0,255,255);
	m_cr[4] = RGB(0,0,255);
	m_cr[5] = RGB(128,64,64);
	m_cr[6] = RGB(0,255,0);
	m_cr[7] = RGB(255,128,255);
	m_cr[8] = RGB(0,255,0);

	for(int i = 0; i < 9; i++)
	{
		if(i != 8)
		{
			m_Pen[i].CreatePen(PS_SOLID, 0, m_cr[i]);
		}
		else
		{
			m_Pen[i].CreatePen(PS_SOLID, 2, m_cr[i]);
		}
		
	}
}
