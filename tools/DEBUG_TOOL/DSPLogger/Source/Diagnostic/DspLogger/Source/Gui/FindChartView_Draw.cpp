// ChartView.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "FindChartView.h"

#include "FindDoc.h"
#include "MainFrm.h"

#include "LUA_FUNC.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//I need use it to find point
const DWORD CFindChartView::DEFAULT_POINT_CLR=RGB(254,254,254);
const DWORD CFindChartView::DEFAULT_BAK_CLR  =RGB(30, 30, 30);
const DWORD CFindChartView::DEFAULT_AXIS_CLR =RGB(150,150,150);
const DWORD CFindChartView::DEFAULT_FONT_CLR =RGB(150,150,150);
const int   CFindChartView::AREA_INTERVAL    = 8;

void CFindChartView::DrawCoordinate(CDC *pDC)
{
    if( 0 == m_pDoc->m_nFindDataNum )
    {
        return;
    }

    CPen pen;
    CPen * pOld = NULL;
	int i;
	//x axis
	pen.CreatePen(PS_DASH, 1, DEFAULT_AXIS_CLR);
    for(i = 0; i < m_pDoc->m_nFindDataNum; ++i )
    {
        pOld = pDC->SelectObject(&pen);
		
        pDC->MoveTo(m_nXStart, m_arrYOrig[i]);
        pDC->LineTo(m_nXStart+m_dwX, m_arrYOrig[i]);      
		
        pDC->SelectObject(pOld);
        
		
    }
	pen.DeleteObject();

	//titles	
	CFont Font;
	Font.CreateFont( 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Arial" );
	pDC->SetBkColor(DEFAULT_BAK_CLR);
	pDC->SetTextColor(DEFAULT_FONT_CLR);
	pDC->SelectObject(Font);
	CString str;	
	for (i = 0; i < m_pDoc->m_nFindDataNum; ++i)
	{	
		// y axis title
		WORD wMin= (WORD)(m_MaxMinData[i]);
		WORD wMax= (WORD)(m_MaxMinData[i]>>16);
		str.Format("y:(%04x-%04x) ",wMin,wMax);

		CRect rc;
		GetClientRect(rc);
		
		// TODO: calculate the total size of this view
		DWORD dwDefaultX = MIN_CX < rc.Width() ? rc.Width() : MIN_CX;
		int nxTitle = dwDefaultX/2;//(m_nXStart+m_dwX-100)/2;
		int nyTitle	= Y_AXIS_DEFAULT_OFFSET;

		
		if(i==0)
		{
			if (!m_pDoc->m_bAutoRefresh)
			{
				pDC->TextOut(m_nXStart,8,str);
			}
		}
		else
		{
			if (!m_pDoc->m_bAutoRefresh)
			{
				pDC->TextOut(m_nXStart,m_arrYOrig[i-1]+Y_AXIS_DEFAULT_OFFSET,str);
			}
			nyTitle += m_arrYOrig[i-1];
		}
		
		
		

		// x axis title
		if( m_arrByIndex[i] )
		{	
			str.Format("x:(0-%d) by index  ",m_nMaxTotalTP);
			pDC->TextOut(m_nXStart+m_dwX-100 ,m_arrYOrig[i]+Y_AXIS_DEFAULT_OFFSET,str);	
			
		}
		else
		{	
			str.Format("x:(%d-%d) by time ",m_dwMinTime,m_dwMaxTime);
			pDC->TextOut(m_nXStart+m_dwX-100 ,m_arrYOrig[i]+Y_AXIS_DEFAULT_OFFSET,str);
		}	

		// addr title	
		if (strlen(m_pDoc->m_arrFindData[i].szAlias))
		{
			str.Format("%s [%s]",m_pDoc->m_arrFindData[i].szAlias,m_pDoc->m_arrFindData[i].szAddrValue);
		}
		else
		{
			str.Format("%s",m_pDoc->m_arrFindData[i].szAddrValue);
		}	
		pDC->TextOut(nxTitle ,nyTitle,str);

	}

	Font.DeleteObject();
}

void CFindChartView::DrawWave( CDC * pDC )
{
    CPen   pen;
    CPen * pOld;

    for( int i = 0; i < m_pDoc->m_nFindDataNum; ++i )
    {
		VEC_FIND vecTp, vecTime;
		GetTPandTime(i,vecTp,vecTime);
        if( vecTp.size() == 0 )
        {
            continue;
        }
        pen.CreatePen(PS_SOLID, 1, m_pDoc->m_arrFindColor[i]);
        pOld = pDC->SelectObject(&pen);        
		
		WORD  wMinData, wMaxData ;
        if (m_bUseSameY)
        {	
			wMinData =m_wMinData;
			wMaxData =m_wMaxData;
        }
		else
		{
			wMinData= (WORD)(m_MaxMinData[i]);
			wMaxData= (WORD)(m_MaxMinData[i]>>16);
		}
		

		WORD  wData    = GETTPDATA(vecTp[0]);
		#if DO_LUA_DATA_PROCESSING
		WORD  wAddr	   = GETTPADDR(vecTp[0]);
		int	new_data;
		lua_data((int)wAddr,(int)wData,&new_data);
		wData=(WORD)new_data;
		// ----- end of lua processing data

		#endif
        if( !m_arrSigned[i] )
        {
            wMaxData -= wMinData;
        }       
        
        short sData;

        DWORD dwMaxTime =m_dwMaxTime; 
		DWORD dwMinTime =m_dwMinTime;

        DWORD dwTime = vecTime[0];
        dwMaxTime   -= dwMinTime;
        dwTime      -= dwMinTime;

        int   x     = m_nXStart;
        int   y     = m_arrYOrig[i];
        int   xOld  = 0;
        int   yOld  = 0;
        int   nBndr =  m_arrYBoundary[i] - AREA_INTERVAL;

		//first point
		if( m_arrByIndex[i] )
		{
			x = m_nXStart;
		}
		else
		{
			if (dwMaxTime)
			{
				x = m_nXStart + (int)((float)dwTime / dwMaxTime * m_dwX);
			}
			
		}
		
        if( m_arrSigned[i] )
        {
            sData = (short)wData;
			if (wMaxData)
			{
				y = m_arrYOrig[i] - (int)((float)sData / wMaxData * nBndr / 2);
			}
        }
        else
        {
            wData -= wMinData;
			if (wMaxData)
			{
				y = m_arrYOrig[i] - (int)((float)wData / wMaxData * nBndr);
			}
            
        }
        

		//add point in map  @andy.bian modify 06.8.29 
		MoveTo(pDC,x, y); 
        UINT nTotal = vecTp.size();
        for( UINT j = 1; j < nTotal; ++j )
        {
            xOld = x;
            yOld = y;

            wData = GETTPDATA(vecTp[j]);
			#if DO_LUA_DATA_PROCESSING
			WORD wAddr = GETTPADDR(vecTp[j]);
			int new_data;
			lua_data((int)wAddr,(int)wData,&new_data);
			wData=(WORD)new_data;
			#endif
            
            if( m_arrByIndex[i] )
            {
				if(m_nMaxTotalTP)
				{
					x = m_nXStart + (int)((float)(j)/ m_nMaxTotalTP* m_dwX);
				}
            }
            else
            {
                dwTime = vecTime[j];
                dwTime -= dwMinTime;
				if (dwMaxTime)
				{
					x = m_nXStart + (int)((float)dwTime / dwMaxTime * m_dwX);
				}
            }
         
            if( m_arrSigned[i] )
            {
                sData = (short)wData;
				if (wMaxData)
				{
					y = m_arrYOrig[i] - (int)((float)sData / wMaxData * nBndr / 2);
				}

            }
            else
            {
                wData -= wMinData;
				if (wMaxData)
				{
					y = m_arrYOrig[i] - (int)((float)wData / wMaxData * nBndr);
				}
           
            }
			if (xOld!=x || yOld!=y)
			{
				MoveTo(pDC,xOld, yOld);
				pDC->LineTo(x, y );
			}
            
        }        
        
        pDC->SelectObject(pOld);
        pen.DeleteObject();
    }

}

