// EqModeStatic.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "EqModeStatic.h"
#include <math.h>
#include "ConvertFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define LEFT_EDGE_WITH 30
#define RIGHT_EDGE_WITH 10
#define BOTTOM_EDGE_WITH 40
#define TOP_EDGE_WITH 10
/////////////////////////////////////////////////////////////////////////////
// CEqModeStatic

CEqModeStatic::CEqModeStatic()
{
	m_iYMax = 18;
    m_iYMin = -72;

    m_pBKBitmap = NULL;
    m_iXAxisLength = 0;
    m_iYAxisLength = 0;
    
    m_pOldBitmap = NULL;

	m_dMinXValue = 1;
	m_dMaxXValue = 22050.0;
	m_dXScale[0] = 0.00004535;
	m_dXScale[1] = 0.000907;
	m_dXScale[2] = 0.00227;
	m_dXScale[3] = 0.004535;
	m_dXScale[4] = 0.00907;
	m_dXScale[5] = 0.01814;
	m_dXScale[6] = 0.0272;
	m_dXScale[7] = 0.0363;
	m_dXScale[8] = 0.04535;
	m_dXScale[9] = 0.11337;
	m_dXScale[10] = 0.1814;
	m_dXScale[11] = 0.2721;
	m_dXScale[12] = 0.72564;
    m_dXScale[13] = 1;

	m_nType = Draw_All;
	m_nSoundEqVer = sound_eq_ver_4;

	m_bModified = TRUE;
}

CEqModeStatic::~CEqModeStatic()
{
    m_pOldBitmap = NULL;
    m_pBKBitmap = NULL;
}


BEGIN_MESSAGE_MAP(CEqModeStatic, CStatic)
	//{{AFX_MSG_MAP(CEqModeStatic)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEqModeStatic message handlers

void CEqModeStatic::OnPaint() 
{
//	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect;
    this->GetClientRect( rect );
    m_ptOrg.x = rect.left + LEFT_EDGE_WITH;
    m_ptOrg.y = rect.bottom - BOTTOM_EDGE_WITH;
    CPaintDC paintdc(this);

	if(m_memDC.GetSafeHdc() == NULL)
    {
        CClientDC dc( this );
        m_memDC.CreateCompatibleDC(&dc);
        if((m_pBKBitmap = new CBitmap()) == NULL) return ;
        m_pBKBitmap->CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
        m_pOldBitmap = m_memDC.SelectObject(m_pBKBitmap);        
        m_memDC.SetBkMode(TRANSPARENT);
	}

	if( m_bModified )
	{
		m_memDC.SelectObject( GetFont() );
		m_memDC.FillSolidRect(&rect,RGB(212,208,200));
		DrawCoordinateAxis( &m_memDC );
		DrawFreqCurves( &m_memDC );
		m_bModified = FALSE;
	}
	paintdc.BitBlt(0,0,rect.Width(),rect.Height(),&m_memDC,0,0,SRCCOPY);

}

// void CEqModeStatic::EraseBK()
// {   
//     if(m_memDC.GetSafeHdc() == NULL)
//     {
//         CRect rect;
//         this->GetClientRect( rect );
// //        rect.bottom += 6;
// //        rect.right += 6;
//         
//         CClientDC dc( this );
//         m_memDC.CreateCompatibleDC(&dc);
//         if((m_pBKBitmap = new CBitmap()) == NULL) return ;
//         m_pBKBitmap->CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
//         m_pOldBitmap = m_memDC.SelectObject(m_pBKBitmap);        
//         m_memDC.SetBkMode(TRANSPARENT);
//         //draw background 
//         m_memDC.FillSolidRect(&rect,RGB(212,208,200));
//     }    
//     //force to call onpaint
//     Invalidate();  
// }

void CEqModeStatic::DrawFreqCurves(CDC* pDC)
{
	CPen bluePen( PS_SOLID,0, RGB(0,0,255) );
    CPen redPen( PS_SOLID,0, RGB(255,0,0) );
    CPen greenPen( PS_SOLID,0, RGB(0,255,0) );
    CPen yellowPen( PS_SOLID,0,RGB( 255,255,0 ) );

	if( Draw_Bass == m_nType )
	{
		DrawFreqCurve( pDC,&yellowPen,m_vAllBassFreqItem );
	}
	else if( Draw_Treble == m_nType )
	{
		DrawFreqCurve( pDC,&redPen,m_vAllTrebleFreqItem );
	}
	else if( Draw_Lcf == m_nType )
	{	
		if( m_vAllLcfFreqItem.size() != 0 )
		{
			DrawFreqCurve( pDC,&greenPen,m_vAllLcfFreqItem );
		}
	}
	else if( Draw_All == m_nType )
	{
		DrawFreqCurve( pDC,&bluePen,m_vAllFreqItem );
	}
	else if( Draw_Lcf_L_R == m_nType )
	{
		DrawFreqCurve( pDC,&greenPen,m_vAllLcfFreqItem );
		DrawFreqCurve( pDC,&bluePen,m_vAllLcfRFreqItem );
	}
	else
	{
		DrawFreqCurve( pDC,&greenPen,m_vAllFreqItem );
		DrawFreqCurve( pDC,&bluePen,m_vAllRFreqItem );
	}
}

void CEqModeStatic::DrawFreqCurve( CDC* pDC,CPen* pPen,const std::vector<SpecialRectOfEQMode::FreqItem>& vAllFreqItems )
{
	CGdiObject* pOldPen = pDC->SelectObject( pPen );
    
    double dYScale = m_iYAxisLength / ( m_iYMax - m_iYMin );    
    double dXScale = m_iXAxisLength / (log10( SpecialRectOfEQMode::m_dMaxXValue) 
        -log10( SpecialRectOfEQMode::m_dMinXValue)) ;
    
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
    CPoint ptFirst((int)((vAllFreqItems[0].first -log10( SpecialRectOfEQMode::m_dMinXValue) )* dXScale ), (int)(dYValue * dYScale)  );
    if( ptFirst.y < 0 )
    {
        ptFirst.y =  m_ptOrg.y;
    }
    ptFirst.x += m_ptOrg.x;
    ptFirst.y = m_ptOrg.y - ptFirst.y;
    pDC->MoveTo( ptFirst );
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
        
        CPoint ptSecond((int)((vAllFreqItems[i].first -log10( SpecialRectOfEQMode::m_dMinXValue) )* dXScale) , (int)(dYValue * dYScale+0.5) );   
        
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

void CEqModeStatic::DrawCoordinateAxis( CDC* pDC )
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
    pDC->TextOut(ptLine.x, ptLine.y, "Hz" );


    m_iXAxisLength = rect.right - RIGHT_EDGE_WITH - LEFT_EDGE_WITH - ARROW_X_LENGTH - SCALE_X_OFFSET;
    m_iYAxisLength = rect.bottom - BOTTOM_EDGE_WITH - TOP_EDGE_WITH - ARROW_X_LENGTH - SCALE_Y_OFFSET;
	
    CPoint xScale(m_ptOrg);

    CPen dotPen(PS_DOT,0,RGB(0,0,0));    
    CGdiObject* pOldPen =  pDC->SelectObject( &dotPen );

    //draw text in x axis
    double dXScale = m_iXAxisLength / 
                    (log10( SpecialRectOfEQMode::m_dMaxXValue) -log10( SpecialRectOfEQMode::m_dMinXValue)) ;
	
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

	int nStrPos = 10000;
    //for( int i=0; i< X_SCALE_MODE_COUNT; ++i )
	for( int i=X_SCALE_MODE_COUNT-1;i>=0;i-- )
    {        
        int ix =  (int)(SpecialRectOfEQMode::m_dXScale[i] * SpecialRectOfEQMode::m_dMaxXValue + 0.5);
		int iXScale =0;
		iXScale = (int)((log10(ix) -log10( SpecialRectOfEQMode::m_dMinXValue) ) * dXScale);
        
        xScale.x = m_ptOrg.x + iXScale;
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
		hOldFont=(HFONT__ *)pDC->SelectObject(hFont);   
		CRect rtText(ptXTextPos.x,ptXTextPos.y,ptXTextPos.x+20,ptXTextPos.y+60);
		
		//
		if( nStrPos - logfont.lfHeight > ptXTextPos.x )
		{
			pDC->TextOut(ptXTextPos.x, ptXTextPos.y,strXText );
			nStrPos = ptXTextPos.x;
		}
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
    pDC->TextOut(ptLine.x, ptLine.y, "dB" );
    
    // text in y axis    
    xScale = m_ptOrg;
    double y = m_ptOrg.y;
    double dYValueStep = m_iYAxisLength / ((m_iYMax - m_iYMin)/6 -0.00000001);
    for( i=0; i<(m_iYMax - m_iYMin)/6 +1; ++i )
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
        strYValue.Format(_T("%d"), (i*6)+m_iYMin );
        pDC->TextOut(xScale.x-LEFT_EDGE_WITH, xScale.y-5 ,strYValue);
		
        y -= dYValueStep;
        xScale.y = (long)(y+0.5);   
    }    
    pDC->SelectObject( pOldBrush );
}

void CEqModeStatic::OnDestroy() 
{
	CStatic::OnDestroy();
	
	// TODO: Add your message handler code here
	if( m_pBKBitmap )
    {
        delete m_pBKBitmap;
        m_pBKBitmap = NULL;
    }
	
}

void CEqModeStatic::SetData( EQ_MODE_NV_PARAM_T& EqData )
{
	memcpy( &m_Data,&EqData,sizeof( EQ_MODE_NV_PARAM_T ) );
	m_Data.agc_in_gain = 20 * (double)log10((double)(EqData.agc_in_gain/(double)4096.0));
	m_Data.agc_in_gain = (short)(m_Data.agc_in_gain * 10) / 10.0;

	CalculateFreqGraphyData();

	// Data is changed
	m_bModified = TRUE;
}

void CEqModeStatic::CalculateFreqGraphyData( int nXScaleCount )
{
    m_vAllFreqItem.clear();
    m_vAllBassFreqItem.clear();
    m_vAllTrebleFreqItem.clear();
    m_vAllLcfFreqItem.clear();
	m_vAllRFreqItem.clear();
    m_vAllLcfRFreqItem.clear();
	
    double dx = pow( m_dMaxXValue / m_dMinXValue   , 1.0 / nXScaleCount );
    double x = m_dMinXValue;

	BOOL bDrawAll = ( Draw_All == m_nType || Draw_All_L_R == m_nType );
	
	IIR_FILTER_PARA_T filter_para_lcf,filter_para_lcf_r;
	FILTER_LCF_CALC_PARA_T filter_lcf_calc_para;
	short lcf_out_gain = 0,lcf_out_gain_r = 0;
	// calculate lcf curve
	if( Draw_Lcf == m_nType || Draw_Lcf_L_R == m_nType || bDrawAll )
	{
		if( sound_eq_ver_3 == m_nSoundEqVer )
		{
			Filter_CalcF1f1Coeffs(m_Data.lcf_filter.f1_g0, m_Data.lcf_filter.f1_g1, m_Data.lcf_filter.f1_fp, m_Data.lcf_filter.f2_g0, m_Data.lcf_filter.f2_g1, m_Data.lcf_filter.f2_fp, SAMPLE_FREQUENCY, &filter_para_lcf);
		}
		else
		{
			if( Draw_Lcf == m_nType || Draw_All == m_nType )
			{
				// F1F1 mode
				filter_lcf_calc_para.isFilterOn = TRUE;
				filter_lcf_calc_para.eLcfParaType = FILTER_LCFPARA_F1F1;
				memcpy( &filter_lcf_calc_para.unlcfPara.lcfPara,&m_Data.lcf_filter,sizeof( LCF_FILTER_NV_PARAM_T ) );
				Filter_CalcLCF_Ex( &filter_lcf_calc_para,0,SAMPLE_FREQUENCY,&filter_para_lcf,&lcf_out_gain );
			}
			else
			{
				// Butterworth mode
				// 此模式下f1_fp复用为fp_l,f1_g0复用为lcf_gain_l,
				// f2_fp复用为fp_r,f2_g0复用为lcf_gain_r
				filter_lcf_calc_para.isFilterOn = TRUE;
				filter_lcf_calc_para.eLcfParaType = FILTER_LCFPARA_BUTTERWORTH;
				filter_lcf_calc_para.unlcfPara.fp = m_Data.lcf_filter.f1_fp;
				Filter_CalcLCF_Ex( &filter_lcf_calc_para,m_Data.lcf_filter.f1_g0,SAMPLE_FREQUENCY,&filter_para_lcf,&lcf_out_gain );
				
				filter_lcf_calc_para.unlcfPara.fp = m_Data.lcf_filter.f2_fp;
				Filter_CalcLCF_Ex( &filter_lcf_calc_para,m_Data.lcf_filter.f2_g0,SAMPLE_FREQUENCY,&filter_para_lcf_r,&lcf_out_gain_r );
			}
		}
	}

	IIR_FILTER_PARA_T filter_para_bass;
	short bass_gain = 0;
	if( Draw_Bass == m_nType || bDrawAll )
	{
		if( sound_eq_ver_3 == m_nSoundEqVer )
		{
			Filter_CalcEqCoeffs(m_Data.eq_bass.bass_f0, m_Data.eq_bass.bass_q, m_Data.eq_bass.bass_boostdB, m_Data.eq_bass.bass_gaindB, SAMPLE_FREQUENCY, &filter_para_bass);
		}
		else
		{
			Filter_CalcEqCoeffs_Ex(m_Data.eq_bass.bass_f0, m_Data.eq_bass.bass_q, m_Data.eq_bass.bass_boostdB, m_Data.eq_bass.bass_gaindB, SAMPLE_FREQUENCY, &filter_para_bass,&bass_gain );
		}
	}
	
	IIR_FILTER_PARA_T filter_para_treble;
	short treble_gain = 0;
	if( Draw_Treble == m_nType || bDrawAll )
	{
		if( sound_eq_ver_3 == m_nSoundEqVer )
		{
			Filter_CalcEqCoeffs(m_Data.eq_treble.treble_f0, m_Data.eq_treble.treble_q, m_Data.eq_treble.treble_boostdB, m_Data.eq_treble.treble_gaindB, SAMPLE_FREQUENCY, &filter_para_treble );
		}
		else
		{
			Filter_CalcEqCoeffs_Ex(m_Data.eq_treble.treble_f0, m_Data.eq_treble.treble_q, m_Data.eq_treble.treble_boostdB, m_Data.eq_treble.treble_gaindB, SAMPLE_FREQUENCY, &filter_para_treble,&treble_gain );
		}
	}

    for( int i=0;i<nXScaleCount;i++ )
    {
        FreqItem tempItem;
        double treble   , bass   , lcf = 0,lcf_r = 0;
		tempItem.first = log10(x);
		
		if( sound_eq_ver_3 == m_nSoundEqVer )
		{
			if( Draw_Lcf == m_nType || bDrawAll )
			{
				lcf = ::CalcFrequencyResponse(filter_para_lcf.A0,
					filter_para_lcf.A1,
					filter_para_lcf.A2,
					filter_para_lcf.B0,
					filter_para_lcf.B1,
					filter_para_lcf.B2,
					x);
			}

			if( Draw_Bass == m_nType || bDrawAll )
			{
				bass = ::CalcFrequencyResponse(filter_para_bass.A0,
					filter_para_bass.A1,
					filter_para_bass.A2,
					filter_para_bass.B0,
					filter_para_bass.B1,
					filter_para_bass.B2,
					x);
			}

			if( Draw_Treble == m_nType || bDrawAll )
			{
				treble = ::CalcFrequencyResponse(filter_para_treble.A0,
					filter_para_treble.A1,
					filter_para_treble.A2,
					filter_para_treble.B0,
					filter_para_treble.B1,
					filter_para_treble.B2,
					x);
			}

			if( bDrawAll )
			{
				tempItem.second = ::CalcFrequencyResponse( bass   , treble   , lcf );
			}
		}
		else
		{
			if( Draw_Lcf == m_nType || Draw_Lcf_L_R == m_nType || bDrawAll )
			{
				lcf = ::CalcFrequencyResponse(filter_para_lcf.A0,
					filter_para_lcf.A1,
					filter_para_lcf.A2,
					filter_para_lcf.B0,
					filter_para_lcf.B1,
					filter_para_lcf.B2,
					x,
					lcf_out_gain
					);
			}

			if( Draw_Lcf_L_R == m_nType || Draw_All_L_R == m_nType )
			{
				lcf_r = ::CalcFrequencyResponse(filter_para_lcf_r.A0,
					filter_para_lcf_r.A1,
					filter_para_lcf_r.A2,
					filter_para_lcf_r.B0,
					filter_para_lcf_r.B1,
					filter_para_lcf_r.B2,
					x,
					lcf_out_gain_r
					);
			}

			if( Draw_Bass == m_nType || bDrawAll )
			{
				bass = ::CalcFrequencyResponse(filter_para_bass.A0,
					filter_para_bass.A1,
					filter_para_bass.A2,
					filter_para_bass.B0,
					filter_para_bass.B1,
					filter_para_bass.B2,
					x,
					bass_gain,
					32768);
			}

			if( Draw_Treble == m_nType || bDrawAll )
			{
				treble = ::CalcFrequencyResponse(filter_para_treble.A0,
					filter_para_treble.A1,
					filter_para_treble.A2,
					filter_para_treble.B0,
					filter_para_treble.B1,
					filter_para_treble.B2,
					x,
					treble_gain,
					32768);
			}

			if( bDrawAll )
			{
				tempItem.second = ::CalcFrequencyResponse( bass   , treble   , lcf );
			}
		}
		
		if( bDrawAll )
		{
			tempItem.second += m_Data.agc_in_gain;
			m_vAllFreqItem.push_back( tempItem );
		}

		if( Draw_All_L_R == m_nType )
		{
			tempItem.second = ::CalcFrequencyResponse( bass   , treble   , lcf_r );
			tempItem.second += m_Data.agc_in_gain;
			m_vAllRFreqItem.push_back( tempItem );
		}

		if( Draw_Treble == m_nType )
		{
			tempItem.second = treble;
			m_vAllTrebleFreqItem.push_back( tempItem );
		}

		if( Draw_Bass == m_nType )
		{
			tempItem.second = bass;
			m_vAllBassFreqItem.push_back( tempItem );
        }

        if( Draw_Lcf == m_nType || Draw_Lcf_L_R == m_nType )
		{
			tempItem.second = lcf;
			m_vAllLcfFreqItem.push_back( tempItem );
		}

		if( Draw_Lcf_L_R == m_nType )
		{
			tempItem.second = lcf_r;
			m_vAllLcfRFreqItem.push_back( tempItem );
		}
        x *= dx;
    }
}