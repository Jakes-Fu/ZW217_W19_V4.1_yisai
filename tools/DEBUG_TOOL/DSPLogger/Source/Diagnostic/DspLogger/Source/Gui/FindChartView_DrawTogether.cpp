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

void CFindChartView::DrawCoordinate_Together(CDC *pDC)
{
    if( 0 == m_pDoc->m_nFindDataNum ) return;

    CPen pen;
    CPen * pOld = NULL;
	pen.CreatePen(PS_DASH, 1, DEFAULT_AXIS_CLR);
	pOld = pDC->SelectObject(&pen);
	
	pDC->MoveTo(m_nXStart, m_arrYOrig[0]);
	pDC->LineTo(m_nXStart+m_dwX, m_arrYOrig[0]);

	pDC->SelectObject(pOld);
	pen.DeleteObject();	

	//titles
	CFont Font;
	Font.CreateFont( 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Arial" );

	pDC->SetBkColor(DEFAULT_BAK_CLR);
	pDC->SetTextColor(DEFAULT_FONT_CLR);
	pDC->SelectObject(Font);
	CString str;
		
	str.Format("y:(%04x-%04x) ",m_wMinData,m_wMaxData);
	if (!m_pDoc->m_bAutoRefresh)
	{
		pDC->TextOut(m_nXStart ,Y_AXIS_DEFAULT_OFFSET,str);
	}

	if( m_arrByIndex[0] )
	{
		str.Format("x:(0-%d) by index ",m_nMaxTotalTP);
		pDC->TextOut(m_nXStart+m_dwX-100, m_arrYOrig[0]+Y_AXIS_DEFAULT_OFFSET,str);
	}
	else
	{
		str.Format("x:(%d-%d) by time ",m_dwMinTime,m_dwMaxTime);
		pDC->TextOut(m_nXStart+m_dwX-100, m_arrYOrig[0]+Y_AXIS_DEFAULT_OFFSET,str);
	}  
	Font.DeleteObject();
}

void CFindChartView::DrawWave_Together( CDC * pDC )
{
    CPen   pen;
    CPen * pOld;

	for(int i = 0; i < m_pDoc->m_nFindDataNum; ++i )
    {
		VEC_FIND vecTp, vecTime;
		GetTPandTime(i,vecTp,vecTime);
        if( vecTp.size() == 0 )
        {
            continue;
        }

        pen.CreatePen(PS_SOLID, 1, m_pDoc->m_arrFindColor[i]);
        pOld = pDC->SelectObject(&pen);    

        int   x		= 0;
        int   y		= 0;
        int   xOld  = 0;
        int   yOld  = 0;
        int   nBndr = m_arrYBoundary[0]-AREA_INTERVAL;
		
        WORD  wMaxData =m_wMaxData;
		WORD  wMinData =m_wMinData;
        WORD  wData    = GETTPDATA(vecTp[0]);
		// ----- lua processing data
		#if DO_LUA_DATA_PROCESSING
		WORD  wAddr	   = GETTPADDR(vecTp[0]);
		int	new_data;
		lua_data((int)wAddr,(int)wData,&new_data);
		wData=(WORD)new_data;
		#endif
		// ----- end of lua processing data
        if( !m_arrSigned[i] )
        {
            wMaxData -= wMinData;
        }       
        
        DWORD dwMaxTime =m_dwMaxTime; 
		DWORD dwMinTime =m_dwMinTime;
        DWORD dwTime = vecTime[0];
        dwMaxTime   -= dwMinTime;
        dwTime      -= dwMinTime;
		
		//first point
		if( m_arrByIndex[0] )
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
		
        if( m_arrSigned[0] )
        {
            short sData = (short)wData;
			if (wMaxData)
			{
				y = m_arrYOrig[0] - (int)((float)sData / wMaxData * nBndr / 2);
			}
        }
        else
        {
            wData -= m_wMinData;
			if (wMaxData)
			{
				y = m_arrYOrig[0] - (int)((float)wData / wMaxData * nBndr);
			}
        }
		MoveTo(pDC ,x, y); 

		
		UINT nTotal=	vecTp.size();
        for( UINT j = 1; j < nTotal; ++j )
        {
            xOld = x;
            yOld = y;

            wData = GETTPDATA(vecTp[j]);
			// ----- lua processing data
			#if DO_LUA_DATA_PROCESSING
			WORD  wAddr	   = GETTPADDR(vecTp[j]);
			int	new_data;
			lua_data((int)wAddr,(int)wData,&new_data);
			wData=(WORD)new_data;
			#endif
			// ----- end of lua processing data            
            
            if( m_arrByIndex[0] )
            {
				if (m_nMaxTotalTP)
				{
					x = m_nXStart + (int)((float)(j)/ m_nMaxTotalTP * m_dwX);
				}
            }
            else
            {
                dwTime = vecTime[j];
                dwTime -= m_dwMinTime;
				if (dwMaxTime)
				{
					x = m_nXStart + (int)((float)dwTime / dwMaxTime * m_dwX);
				}
            }
         
            if( m_arrSigned[0] )
            {
                short sData = (short)wData;
				if (wMaxData)
				{
					y = m_arrYOrig[0] - (int)((float)sData / wMaxData * nBndr / 2);
				}

            }
            else
            {
                wData -= m_wMinData;
				if (wMaxData)
				{
					y = m_arrYOrig[0] - (int)((float)wData / wMaxData * nBndr);
				}
           
            }

			if (xOld!=x || yOld!=y)
			{
				MoveTo(pDC,xOld, yOld);
				pDC->LineTo(x,  y);
			}
            
        }        
        
        pDC->SelectObject(pOld);
        pen.DeleteObject();
    }

}

