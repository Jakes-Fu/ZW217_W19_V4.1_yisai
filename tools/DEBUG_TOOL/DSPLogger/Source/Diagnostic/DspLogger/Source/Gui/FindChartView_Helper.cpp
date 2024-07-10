// ChartView.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "FindChartView.h"

#include "MainFrm.h"
#include "FindDoc.h"
#include "DspLoggerDoc.h"

#include "LUA_FUNC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//find items from TPSet
void CFindChartView::FindItems()
{
//    CWaitCursor waitCursor;

    PTP_FIND_DATA pFindData= NULL;
    ULONG lCount, lIdx ,lStartPos;
    
    pFindData  = m_pDoc->m_arrFindData;    
    m_nFindNum = m_pDoc->m_nFindDataNum; 
    
    for( int i = 0; i < m_nFindNum; ++i )
    {
        lStartPos = 0;
        lCount = m_pSet->Find( &pFindData[i], 1 ,&lStartPos, &m_arrSigned[i]);         
        m_pDoc->m_arrFindData[i].dwFindCount= lCount;   
        
        for( ULONG m = 0; m < lCount; ++m )
        {
            lIdx = m_pSet->GetIndexByFindIdx(m);
            m_arrIndex[i].push_back( lIdx );
        }
        m_pSet->ClearFind();
    }	
}

//Find update data from TPSet
void CFindChartView::FindUpdate()
{
	//teana hu 2009.03.24
	if(m_pSet->GetTpCount() <= m_uLastTPCount)
	{
		return;
	}
	m_uLastTPCount = m_pSet->GetTpCount();
	
//    CWaitCursor waitCursor;
    
    PTP_FIND_DATA pFindData= NULL;    
    ULONG lCount, lIdx, dwStartPos;

    pFindData  = m_pDoc->m_arrFindData;
    m_nFindNum = m_pDoc->m_nFindDataNum; 
    
    for( int i = 0; i < m_nFindNum; ++i )
    {
        //clear previous find items   
        dwStartPos= pFindData[i].dwReserve1;
        if (dwStartPos == 0 )
        {
			VEC_FIND vecTmpIndex;
			m_arrIndex[i].swap(vecTmpIndex);
			m_arrIndex[i].reserve(DRAW_MAX_POINTS);
			m_pDoc->m_arrFindData[i].dwFindCount  = 0;
			
        }
		
        lCount			= m_pSet->Find( &pFindData[i], 1 ,&pFindData[i].dwReserve1, &m_arrSigned[i]); 
        ULONG uSize		= m_arrIndex[i].size();
		ULONG uOffset	= 0;
		if (  uSize + lCount > m_pDoc->m_dwDrawMaxPoints && lCount < m_pDoc->m_dwDrawMaxPoints)
		{		
			VEC_FIND vecTmpIndex;
			m_arrIndex[i].swap(vecTmpIndex);
			m_arrIndex[i].reserve(DRAW_MAX_POINTS);
            ULONG u = uSize + lCount - m_pDoc->m_dwDrawMaxPoints;
			for( ; u < uSize; ++u )
			{
				m_arrIndex[i].push_back( vecTmpIndex[u]);
			}
			
		}
		else if (  lCount > m_pDoc->m_dwDrawMaxPoints)
		{
			uOffset = lCount - m_pDoc->m_dwDrawMaxPoints;
			VEC_FIND vecTmpIndex;
			m_arrIndex[i].swap(vecTmpIndex);
			m_arrIndex[i].reserve(DRAW_MAX_POINTS);
		}
		
        for( ULONG m = uOffset; m < lCount; ++m )
        {
            lIdx = m_pSet->GetIndexByFindIdx(m);
            m_arrIndex[i].push_back( lIdx);
        }
		m_pDoc->m_arrFindData[i].dwFindCount += lCount; 
        m_pSet->ClearFind();
    }	
}

void CFindChartView::GetTPandTime( int nNum, VEC_FIND& vecTp, VEC_FIND& vecTime )
{
	vecTp.clear();
	vecTime.clear();
	UINT nSize = m_arrIndex[nNum].size();
	for (UINT  i = 0 ; i < nSize; ++i)
	{
		vecTp.push_back( m_pSet->GetTp(m_arrIndex[nNum][i]) );
		vecTime.push_back( m_pSet->GetTPTime(m_arrIndex[nNum][i]) );
	}

}
//get max and min data from nNum chart set
void CFindChartView::GetMaxMinData( int nNum, WORD &wMax, WORD &wMin ,VEC_FIND& vecTp)
{
    _ASSERTE( nNum >= 0 && nNum < MAX_FIND_DATA_NUM );
  
    if (nNum < 0 || nNum >= MAX_FIND_DATA_NUM)
        return;

    //下面的代码已调试过，可正常使用，不要删除
    wMax = wMin = 0;
    WORD  wData = 0;

    const VEC_FIND &vec = vecTp;
    UINT nSize = vec.size();
    if( !nSize )
        return;

	wData = GETTPDATA(vec[0]);
	// ----- lua processing data
	#if DO_LUA_DATA_PROCESSING
	WORD  wAddr	   = GETTPADDR(vec[0]);
	int	new_data;
	lua_data((int)wAddr,(int)wData,&new_data);
	wData=(WORD)new_data;
	#endif
	// ----- end of lua processing data	
    if( m_arrSigned[nNum] ) //add by xiaoguang.feng
    {
		short sData = (short)wData;
		wData = (sData < 0 ? -sData : sData); 
    }

    wMax = wMin=wData;
    for( UINT i = 1; i < nSize; ++i )
    {
        wData = GETTPDATA(vec[i]);
		// ----- lua processing data
		#if DO_LUA_DATA_PROCESSING
		WORD  wAddr	   = GETTPADDR(vec[i]);
		int	new_data;
		lua_data((int)wAddr,(int)wData,&new_data);
		wData=(WORD)new_data;
		#endif
		// ----- end of lua processing data	
        if( m_arrSigned[nNum] )
        {
            short sData = (short)wData;
            wData = (sData < 0 ? -sData : sData);
        }

        if( wMax < wData )
            wMax = wData;
        if( wData < wMin )
            wMin = wData;
    }
}

void CFindChartView::GetMaxMinTime( int nNum, DWORD &dwMax, DWORD &dwMin,VEC_FIND& vecTime )
{
    _ASSERTE( nNum >= 0 && nNum < MAX_FIND_DATA_NUM );

    if (nNum < 0 || nNum >= MAX_FIND_DATA_NUM)
        return;

    dwMax = dwMin = 0;
	//DWORD  dwTime = 0;
    const VEC_FIND &vec = vecTime;
    UINT nSize = vec.size();
    if( !nSize )
        return;

    dwMin = vec[0];
    dwMax = vec[nSize - 1];
}

void CFindChartView::ClearFindInfo(void)
{
    for( int i = 0; i < m_nFindNum; ++i )
    {
		 VEC_FIND vecTmpIndex;
		 m_arrIndex[i].swap(vecTmpIndex);
		 m_arrIndex[i].reserve(DRAW_MAX_POINTS);
    }
    m_nFindNum = 0;
}

void CFindChartView::InitForDraw(int nCurveNum)
{
	// init cross point in subchart
	for (int nNum=0; nNum< MAX_FIND_DATA_NUM; nNum++)
	{	
		m_nCross[nNum]=(UINT) -1;
	}

    m_dwX = m_dwDefaultX;
    m_dwY = m_dwDefaultY;

    CSize siz;
    siz.cx = m_nXStart + m_dwX + MARGIN + MARGIN;
    siz.cy = m_nYStart + m_dwY + MARGIN + MARGIN;
    SetScrollSizes(MM_TEXT, siz);

    // 缺省，y方向均分 
    m_nXScale = 1;
	int	nBoundary = m_dwY / nCurveNum;
    for( int i = 0; i < MAX_FIND_DATA_NUM; ++i )
    {
        m_arrYScale[i]    = 1;
        m_arrYInitBndr[i] = nBoundary;
        m_arrYBoundary[i] = nBoundary;
    }

}

void CFindChartView::CalculateYOrigin()
{
	CString str;	
	if( m_nFindNum )
	{
		int nCount  = m_nYStart;
		int nOffset = 0;
		for( int i = 0; i < m_nFindNum; ++i )
		{
			m_arrYBoundary[i] = m_arrYInitBndr[i] + (m_arrYScale[i] - 1) * Y_SCALE_UNIT;
			nCount += m_arrYBoundary[i];
			nOffset = m_arrSigned[i] ? m_arrYBoundary[i] / 2 : 0;
			m_arrYOrig[i] = nCount - nOffset;
		}
	}
}


__inline UINT CFindChartView::PixelToIndex( int nFind, long &lPixel )
{
    UINT nIdx = 0;
    if( m_arrByIndex[nFind] )
    {
        nIdx = (float)(lPixel - m_nXStart) / m_dwX * m_nMaxTotalTP +0.5;
		lPixel=m_nXStart+(float)nIdx/m_nMaxTotalTP*m_dwX;
    }
    else
    {
        DWORD dwMaxTime =m_dwMaxTime; 
		DWORD dwMinTime =m_dwMinTime;
       
        dwMaxTime -= dwMinTime;
        
        DWORD dwTime = (float)(lPixel - m_nXStart) / m_dwX * dwMaxTime+ dwMinTime ;
        
        BOOL bFind = FALSE;
		VEC_FIND vecTp, vecTime;
		GetTPandTime(nFind,vecTp,vecTime);

		int nTotal = vecTp.size();
		//find nearest point in x axis
        for( int i = 0; i < nTotal-1; ++i )
        {	
            if( dwTime >= vecTime[i] && dwTime <= vecTime[i+1] )
            {
				if ((dwTime -vecTime[i]) >= (vecTime[i+1]- dwTime))
				{
					dwTime = vecTime[i+1] - dwMinTime;
					nIdx   = i+1;
				}
				else
				{
					dwTime = vecTime[i] - dwMinTime;
					nIdx   = i;
				}					
                lPixel = m_nXStart + (int)((float)dwTime / dwMaxTime * m_dwX);
                bFind  = TRUE;
                break;
            }
        }
		
        if( !bFind )
        {
            nIdx = 0;
        }
    }

    return nIdx;
}

__inline long CFindChartView::IndexToPixel( int nFind, UINT nIndex )
{
    long lPixel = 0;

    if( m_arrByIndex[nFind] )
    {
        lPixel = m_nXStart + (int)((float)(nIndex)/ m_nMaxTotalTP * m_dwX);
    }
    else
    {
        DWORD dwMaxTime =m_dwMaxTime; 
		DWORD dwMinTime =m_dwMinTime;
      
        dwMaxTime -= dwMinTime;
		VEC_FIND vecTp, vecTime;
		GetTPandTime(nFind,vecTp,vecTime);
        DWORD dwTime = vecTime[nIndex] - dwMinTime;

        lPixel = m_nXStart + (int)((float)dwTime / dwMaxTime * m_dwX);
    }

    return lPixel;
}

__inline int CFindChartView::GetYPixelByIdx( int nFind, UINT nIdx )
{
    int y = 0;
	WORD  wMaxData , wMinData;

    int nBndr =  m_arrYBoundary[nFind] - AREA_INTERVAL;
	VEC_FIND vecTp, vecTime;
	GetTPandTime(nFind,vecTp,vecTime);

    WORD wData = GETTPDATA(vecTp[nIdx]);
	// ----- lua processing data
	#if DO_LUA_DATA_PROCESSING
	WORD  wAddr	   = GETTPADDR(vecTp[nIdx]);
	int	new_data;
	lua_data((int)wAddr,(int)wData,&new_data);
	wData=(WORD)new_data;
	#endif
	// ----- end of lua processing data	

	if (m_bUseSameY || m_bDrawTogether)
	{	
		wMinData =m_wMinData;
		wMaxData =m_wMaxData;
	}
	else
	{
		wMinData= (WORD)(m_MaxMinData[nFind]);
		wMaxData= (WORD)(m_MaxMinData[nFind]>>16);
	}
	

    if( m_arrSigned[nFind] )
    {
        short sData = (short)wData;
        y = m_arrYOrig[nFind] - (int)((float)sData / wMaxData * nBndr / 2);
    }
    else
    {
        wMaxData -= wMinData;
        wData    -= wMinData;
        y = m_arrYOrig[nFind] - (int)((float)wData / wMaxData * nBndr);
    }

    return y;
}




__inline UINT CFindChartView::GetNearestYIdx(int nFind,UINT nIdx,long &lPixel)
{
	_ASSERTE(nFind>=0 && nFind< MAX_FIND_DATA_NUM);
    if (nFind < 0 || nFind >= MAX_FIND_DATA_NUM)
        return 0;

    //X axis by index
	if( m_arrByIndex[nFind] ) return nIdx;

	//X axis by time
	VEC_FIND vecTp, vecTime;
	GetTPandTime(nFind,vecTp,vecTime);

	int nTotal = vecTp.size();
	_ASSERTE(nIdx<nTotal);
    if (nIdx >= nTotal) return 0;

	DWORD dwTime = vecTime[nIdx];
	
	long lLen= labs(lPixel-GetYPixelByIdx( nFind, nIdx ));
	int nNearestIdx=nIdx;

	//find nearest point on y axis
	int nIndex=nIdx+1;
	while (nIndex < nTotal && vecTime[nIndex]==dwTime)//lint !e661
	{	
		if( lLen > labs(lPixel- GetYPixelByIdx( nFind, nIndex )) )
		{
			lLen= labs(lPixel- GetYPixelByIdx( nFind, nIndex ));
			nNearestIdx= nIndex;
		}
		nIndex++;
	}
	
	nIndex=nIdx-1;
	while (nIndex >=0 && vecTime[nIndex]==dwTime)//lint !e661
	{	
		if( lLen > labs(lPixel- GetYPixelByIdx( nFind, nIndex )) )
		{
			lLen= labs(lPixel- GetYPixelByIdx( nFind, nIndex ));
			nNearestIdx= nIndex;
		}
		nIndex--;
	}	

	return nNearestIdx;
}


void CFindChartView::OnGotoChartPoint(ULONG lIdx)
{
    int  nFind   = -1;
    UINT nVecIdx = 0;
    for( int i = 0; i < m_nFindNum; ++i )
    {
        const VEC_FIND &vec = m_arrIndex[i];
        for( UINT m = 0; m < vec.size(); ++m )
        {
            if( vec[m] == lIdx )
            {
                nFind   = i;
                nVecIdx = m;
                break;
            }
        }
    }

	//Do not find point
	if (-1== nFind) return;
	//on together mode only can goto chart one
	if (m_bDrawTogether && nFind>0) return;

    int x = IndexToPixel(nFind, nVecIdx);
    int y = GetYPixelByIdx(nFind, nVecIdx);
			
    m_nCross[nFind]=nVecIdx;
	m_nCurCross = lIdx;

	//target pos
    CRect rc;
    GetClientRect(rc);
	int xpos= x>rc.Width()/2  ? x-rc.Width()/2  : x;
	int ypos= y>rc.Height()/2 ? y-rc.Height()/2 : y;
    
    int nHPos = GetScrollPos(SB_HORZ);
    int nVPos = GetScrollPos(SB_VERT);
	
    int  nDeltaX = 0;
    int  nDeltaY = 0;

//////////////////////////////////////////////////////////////////////////
	if(xpos< nHPos || xpos > nHPos+rc.Width()/2)
	{
		nDeltaX = xpos - nHPos;
	}
	else
	{
		nDeltaX=0;
	}
	TRACE("nDeltax: %d \n", nDeltaX);


	if( y > rc.Height())
	{
		nDeltaY = ypos - nVPos;
	}
	else
	{		
		nDeltaY=0;
		nVPos  =0;
	}
	TRACE("nDeltay: %d \n", nDeltaY);

	nHPos += nDeltaX;
	nVPos += nDeltaY;
	SetScrollPos(SB_HORZ, nHPos);
	SetScrollPos(SB_VERT, nVPos);
	ScrollWindow( nDeltaX, nDeltaY);	
    InvalidateRect(rc, FALSE);
	
}

void CFindChartView::GotoFindItem( LPPOINT pPt )
{
    _ASSERTE( pPt != NULL );

    CRect rc;
    GetClientRect(rc);

    if( pPt->x < m_nXStart || pPt->x > m_nXStart + m_dwX ||
        pPt->y < m_nYStart || pPt->y > m_nYStart + m_dwY )
    {
        InvalidateRect(rc, FALSE);
        return;
    }

    int nFind = -1;
    for( int i = 0; i < m_nFindNum; ++i )
    {
        int nBndr  = m_arrYBoundary[i] - AREA_INTERVAL;
        int nUpper = m_arrSigned[i] ? nBndr / 2 :  nBndr;
        int nLower = m_arrSigned[i] ? nBndr / 2 :  0;
        
        if( pPt->y >= m_arrYOrig[i] - nUpper && pPt->y <= m_arrYOrig[i] + nLower )
        {
            nFind = i;
            break;
        }
    }

    if( nFind > -1 )
    {
		VEC_FIND vecTp, vecTime;
		GetTPandTime(nFind,vecTp,vecTime);
        if( vecTp.size() == 0 )
        {
            return;
        }
		//andy.bian @2006-9-6
		//find nearest point on x axis
		UINT nIdx    = PixelToIndex(nFind, pPt->x);       
		//find nearest point on y axis
		UINT nVecIdx = GetNearestYIdx(nFind,nIdx, pPt->y);
		
		//save cross point index
        m_nCross[nFind] = nVecIdx;

		
		//current select index
		UINT nCurItem=m_arrIndex[nFind][nVecIdx];

		//current active Cross
		m_nCurCross=nCurItem;

		//goto selected item in findlist view
		static UC_HINT uch;
        uch.wParam = 0;
        uch.lParam = nCurItem;
        m_pDoc->UpdateAllViews(this, WM_GOTO_ITEM_BY_CHART, (CObject *)&uch);
        
		//set select subchart active
		int nSelectChart = nFind;
		m_pDoc->UpdateAllViews(this, WM_SELECT_CHART, (CObject *)&nSelectChart);

		//goto selected item in TestPointsView
		CMainFrame* pMainFrame= (CMainFrame*)AfxGetMainWnd();
		CDspLoggerDoc * pDspLoggerDoc =(CDspLoggerDoc*) pMainFrame->m_pView->GetDocument();
		pDspLoggerDoc->UpdateAllViews(this, WM_GOTO_ITEM_BY_CHART, (CObject *)&uch);



    }

    InvalidateRect(rc, FALSE);    
}

void CFindChartView::FindMaxMin()
{	
	UINT  nTotal;

	m_wMaxData =0 ;
	m_wMinData =0xFFFF;
	m_dwMaxTime=0;
	m_dwMinTime=0xFFFFFFFF; 
	m_nMaxTotalTP=0;
	
	for (int i = 0; i < m_pDoc->m_nFindDataNum; ++i )
	{		
	    if(0==m_arrIndex[i].size()) continue;
		
		//find max and min data
		VEC_FIND vecTp, vecTime;
		GetTPandTime(i,vecTp,vecTime);

		WORD wtemMaxData,wtemMinData;
        GetMaxMinData(i, wtemMaxData, wtemMinData,vecTp);
		m_MaxMinData[i]=((DWORD)wtemMaxData)<<16 | wtemMinData;
		if(m_wMaxData < wtemMaxData)  m_wMaxData=wtemMaxData ;
		if(m_wMinData > wtemMinData)  m_wMinData=wtemMinData ;		
		
        //find max and min time
		DWORD dwtemMaxTime,dwtemMinTime;
        GetMaxMinTime(i, dwtemMaxTime, dwtemMinTime,vecTime);
		if(m_dwMaxTime < dwtemMaxTime) m_dwMaxTime=dwtemMaxTime;
		if(m_dwMinTime > dwtemMinTime) m_dwMinTime=dwtemMinTime;

		//total points max
		nTotal= vecTp.size();
		if(m_nMaxTotalTP < nTotal) m_nMaxTotalTP=nTotal ;

	}
}

void CFindChartView::DrawCross(CDC *pDC)
{
    const static int nWidth = 8;

	//draw cross in subchart
	CPen   pen;
	CPen * pOld;
	pen.CreatePen(PS_SOLID, 1, RGB(255,255,255));
    for( int nNum = 0; nNum< m_nFindNum; ++nNum )
    {
		if(m_nCross[nNum]== -1) continue;		
		
		int x = IndexToPixel(nNum, m_nCross[nNum]);
		int y = GetYPixelByIdx(nNum, m_nCross[nNum]);
		
		if( x >= m_nXStart && y >= m_nYStart )
		{
			
			
			pOld = pDC->SelectObject(&pen);
			pDC->MoveTo(x - nWidth, y);
			pDC->LineTo(x + nWidth, y);
			
			pDC->MoveTo(x, y - nWidth);
			pDC->LineTo(x, y + nWidth);
			
			pDC->SelectObject(pOld);
			
		} 
		
	}
	pen.DeleteObject();
}


void CFindChartView::DrawCross_Together( CDC * pDC )
{
	if(m_nCross[0]== -1) return;	
    
	const static int nWidth = 8;
	int x=0 , y=0;
	x = IndexToPixel(0, m_nCross[0]);
	y = GetYPixelByIdx( 0, m_nCross[0]);
	CPen   pen;
	CPen * pOld;
	pen.CreatePen(PS_SOLID, 1, RGB(255,255,255));
	if( x >= m_nXStart && y >= m_nYStart )
	{
		
		
		pOld = pDC->SelectObject(&pen);
		pDC->MoveTo(x - nWidth, y);
		pDC->LineTo(x + nWidth, y);
		
		pDC->MoveTo(x, y - nWidth);
		pDC->LineTo(x, y + nWidth);
		
		pDC->SelectObject(pOld);
		
	} 
	pen.DeleteObject();

}

