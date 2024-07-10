// CoolStatic.cpp : implementation file
//

#include "stdafx.h"
#include "CoolStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ROW_LAYER_NUM   2
#define COL_LAYER_NUM   4



/////////////////////////////////////////////////////////////////////////////
// CCoolStatic

CCoolStatic::CCoolStatic()
{
	m_iDocHeight			= 0;
	m_iDocWidth				= 0;
	m_iLineHeight			= 8;
	m_iLineWidth			= 8;
	m_bScrollBarMessage		= FALSE;  // Only relevant when calculating the scrollbar
	m_bDraw                 = FALSE;

	memset(m_bmpInfo,0,sizeof(m_bmpInfo));
	m_pLayerInfo = NULL;
	m_bDrawBorder = TRUE;
	m_bDrawRect = TRUE;
	m_bDrawLayerInfo = TRUE;
}

CCoolStatic::~CCoolStatic()
{
	// just for test
	if( m_bmpBackground.GetSafeHandle() != NULL )
		m_bmpBackground.DeleteObject();

	m_pLayerInfo = NULL;


}


BEGIN_MESSAGE_MAP(CCoolStatic, CStatic)
	//{{AFX_MSG_MAP(CCoolStatic)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoolStatic message handlers

void CCoolStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect		rFrame;
	GetClientRect( rFrame );

	int iDocHeight = m_iDocHeight;
	int iDocWidght = m_iDocWidth;

	int x =  GetScrollPos( SB_HORZ );
	int y =  GetScrollPos( SB_VERT );

#ifdef X_GDI	
	// Double-buffering
	CDC*		pDCMem		= new CDC;
	CBitmap*	pOldBitmap	= NULL;
	CBitmap		bmpCanvas;

	pDCMem->CreateCompatibleDC( &dc );
	
	bmpCanvas.CreateCompatibleBitmap( &dc, rFrame.Width(), rFrame.Height() );
	
	pOldBitmap = pDCMem->SelectObject( &bmpCanvas );	

	// START DRAW -------------------------------------------------
	// If there is a bitmap loaded, use it
	// Otherwise, paint the background black
	pDCMem->FillSolidRect( rFrame, RGB(0,0,0) );	
	
	UINT nMode		= pDCMem->SetBkMode( TRANSPARENT );	
	
	DrawLayers(pDCMem,x,y,rFrame);
	
	pDCMem->SetBkMode( nMode );
	
	pDCMem->Draw3dRect( rFrame, RGB(0,0,0), RGB(0,0,0) );	// Border
	
	// END DRAW   -------------------------------------------------
	dc.BitBlt( 0, 0, rFrame.Width(), rFrame.Height(), pDCMem, 0, 0, SRCCOPY );
	pDCMem->SelectObject( pOldBitmap );
	delete pDCMem;
#else
	BITMAPINFOHEADER stBmpInfoHeader = { 0 };   
	int nBytesPerLine = ((rFrame.Width()* 32 + 31) & (~31)) >> 3;
	stBmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);   
	stBmpInfoHeader.biWidth = rFrame.Width();   
	stBmpInfoHeader.biHeight = rFrame.Height();   
	stBmpInfoHeader.biPlanes = 1;
	stBmpInfoHeader.biBitCount = 32;   
	stBmpInfoHeader.biCompression = BI_RGB;   
	stBmpInfoHeader.biClrUsed = 0;   
	stBmpInfoHeader.biSizeImage = nBytesPerLine * rFrame.Height();	
	PUINT32 pBGBits = NULL;  
	DWORD   dwBGBufSize = nBytesPerLine * rFrame.Height();
    HBITMAP hBmpMem = ::CreateDIBSection(NULL, (PBITMAPINFO)&stBmpInfoHeader, DIB_RGB_COLORS, (LPVOID*)&pBGBits, NULL, 0);
	
	HGDIOBJ		hbmpOld = NULL;
	HDC hdcTemp = GetDC()->m_hDC;
	HDC hdcMemory = CreateCompatibleDC(hdcTemp);
	if(hBmpMem)
	{
		memset(pBGBits,0,dwBGBufSize);
		HGDIOBJ hbmpOld = ::SelectObject( hdcMemory, hBmpMem); 
		
		Graphics graph(hdcMemory);
		graph.SetSmoothingMode(SmoothingModeNone);			

		SolidBrush bs(Color(255,0,0,255));

		graph.FillRectangle(&bs,rFrame.left,rFrame.top,rFrame.Width(),rFrame.Height());

		//Image *img = Image::FromFile(L"F:\\IMAGE_WALLPAPER_4.jpg");
		//graph.DrawImage(img,0,0,img->GetWidth(),img->GetHeight());

		
		//Point points[] = { Point(0, 0),	Point(m_cxImage, 0),Point(0, m_cyImage)};
		//graph.DrawImage(m_pBGImage, points,3);
		
		//CachedBitmap cbBG((Bitmap*)m_pBGImage,&graph);
		//graph.DrawCachedBitmap(&cbBG,0,0);
	
		DrawLayers(graph,x,y,rFrame);

		graph.ReleaseHDC(hdcMemory);		
		::SelectObject( hdcMemory, hbmpOld);  
		
	}
	DeleteDC(hdcMemory);
	hdcMemory=NULL;
	DeleteDC(hdcTemp);
	hdcTemp=NULL;
	DeleteObject(hBmpMem);
#endif	
	// Has the total document height changed?
	if( iDocHeight != m_iDocHeight || iDocWidght != m_iDocWidth)
	{
		BOOL bInvalidate = ( ( m_iDocHeight < rFrame.Height() ) != ( iDocHeight < rFrame.Height() ) ) ||
			               ( ( m_iDocWidth  < rFrame.Width() ) != ( iDocWidght < rFrame.Width() ) );
		
		//m_iDocHeight = iLastNodePos;
		//m_iDocWidth  = iLastNodePos;

		ResetScrollBar();

		// If the scrollbar has just been hidden/shown, repaint
		if( bInvalidate )
			Invalidate();
	}
}

void CCoolStatic::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int iScrollBarPos = GetScrollPos( SB_VERT );

	CRect rFrame;

	GetClientRect( rFrame );

	switch( nSBCode )
	{
		case SB_LINEUP:
			iScrollBarPos = max( iScrollBarPos - m_iLineHeight, 0 );
		break;

		case SB_LINEDOWN:
			iScrollBarPos = min( iScrollBarPos + m_iLineHeight, GetScrollLimit( SB_VERT ) );
		break;

		case SB_PAGEUP:
			iScrollBarPos = max( iScrollBarPos - rFrame.Height(), 0 );
		break;

		case SB_PAGEDOWN:
			iScrollBarPos = min( iScrollBarPos + rFrame.Height(), GetScrollLimit( SB_VERT ) );
		break;

		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
		{
			SCROLLINFO si;

			ZeroMemory( &si, sizeof(SCROLLINFO) );

			si.cbSize	= sizeof(SCROLLINFO);
			si.fMask	= SIF_TRACKPOS;

			if( GetScrollInfo( SB_VERT, &si, SIF_TRACKPOS ) )
				iScrollBarPos = si.nTrackPos;
			else
				iScrollBarPos = (UINT)nPos;
			break;
		}
	}		

	SetScrollPos( SB_VERT, iScrollBarPos );
	
	Invalidate();
	
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CCoolStatic::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int iScrollBarPos = GetScrollPos( SB_HORZ );

	CRect rFrame;

	GetClientRect( rFrame );

	switch( nSBCode )
	{
		case SB_LINELEFT:
			iScrollBarPos = max( iScrollBarPos - m_iLineWidth, 0 );
		break;

		case SB_LINERIGHT:
			iScrollBarPos = min( iScrollBarPos + m_iLineWidth, GetScrollLimit( SB_HORZ ) );
		break;

		case SB_PAGELEFT:
			iScrollBarPos = max( iScrollBarPos - rFrame.Height(), 0 );
		break;

		case SB_PAGERIGHT:
			iScrollBarPos = min( iScrollBarPos + rFrame.Height(), GetScrollLimit( SB_HORZ ) );
		break;

		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
		{
			SCROLLINFO si;

			ZeroMemory( &si, sizeof(SCROLLINFO) );

			si.cbSize	= sizeof(SCROLLINFO);
			si.fMask	= SIF_TRACKPOS;

			if( GetScrollInfo( SB_HORZ, &si, SIF_TRACKPOS ) )
				iScrollBarPos = si.nTrackPos;
			else
				iScrollBarPos = (UINT)nPos;
			break;
		}
	}		

	SetScrollPos( SB_HORZ, iScrollBarPos );
	
	Invalidate();
	
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CCoolStatic::OnSize(UINT nType, int cx, int cy) 
{	
	// Setting the scroll sends its own size message. Prevent it thus avoiding an ugly loop.
	// Other than that, resizing the control means that the tree height may change (word-wrap).
	if( !m_bScrollBarMessage )
		ResetScrollBar();
	
	CStatic::OnSize(nType, cx, cy);
	
}

void CCoolStatic::SetBackgroundBitmap( BOOL bInvalidate /* = FALSE  */)
{
	//CFileDialog fd( TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST, _T("Bitmap Files (*.bmp)|*.bmp||"), this );

	// If the user clicked 'ok'
	//if( fd.DoModal() == IDOK )
	{
		// If there is a bitmap already loaded, delete it
	    if( m_bmpBackground.GetSafeHandle() != NULL )
			m_bmpBackground.DeleteObject();
		
		// Load the bitmap from the file selected
		HBITMAP hBitmap =	(HBITMAP)LoadImage( NULL, /*fd.GetPathName()*/_T("D:\\bk1024.bmp"), IMAGE_BITMAP, 
												0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE );

		// Attach it to the CBitmap object
		m_bmpBackground.Attach( hBitmap );

		// Repaint if so desired
		if( bInvalidate )
			Invalidate();
	}
}

void CCoolStatic::ResetScrollBar()
{
	// Flag to avoid a call from OnSize while resetting the scrollbar
	m_bScrollBarMessage = TRUE;

	CRect rFrame;

	GetClientRect( rFrame );

	// Need for scrollbars?
	if( rFrame.Height() > m_iDocHeight + 8 )
	{
		ShowScrollBar( SB_VERT, FALSE );	// Hide it
		SetScrollPos( SB_VERT, 0 );
	}
	else
	{
		SCROLLINFO	si;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_PAGE | SIF_RANGE;
		si.nPage = rFrame.Height();
		si.nMax = m_iDocHeight + 8;
		si.nMin = 0 ;

		SetScrollInfo( SB_VERT, &si );
		EnableScrollBarCtrl( SB_VERT, TRUE );
	}

	if( rFrame.Width() > m_iDocWidth + 8 )
	{
		ShowScrollBar( SB_HORZ, FALSE );	// Hide it
		SetScrollPos( SB_HORZ, 0 );
	}
	else
	{
		SCROLLINFO	si;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_PAGE | SIF_RANGE;
		si.nPage = rFrame.Width();
		si.nMax = m_iDocWidth + 8;
		si.nMin = 0 ;

		SetScrollInfo( SB_HORZ, &si );
		EnableScrollBarCtrl( SB_HORZ, TRUE );
	}

	m_bScrollBarMessage = FALSE;
}

LRESULT CCoolStatic::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if( message == WM_NCHITTEST || message == WM_NCLBUTTONDOWN || message == WM_NCLBUTTONDBLCLK )
		return ::DefWindowProc( m_hWnd, message, wParam, lParam );
	
	return CStatic::WindowProc(message, wParam, lParam);
}

BOOL CCoolStatic::Init(DISPLAY_BLEND_LAYER_T *pLayerInfo, DWORD dwLcdHeight, DWORD dwLcdWidth)
{
	m_bDraw  = FALSE;
	m_pLayerInfo = pLayerInfo;
	m_dwLCDHeight = dwLcdHeight;
	m_dwLCDWidth = dwLcdWidth;

	int dwMaxSide = GetMaxSide();

	m_iDocWidth =  ROW_LAYER_NUM * dwMaxSide;
	m_iDocHeight = m_iDocWidth;

	ClearBmpInfo();

	for(int i=0; i< MMI_MAX_LAYER_NUM; i++)
	{
		BYTE *pBuf = new BYTE[sizeof(BITMAPINFOHEADER)];
		memset(pBuf,0,sizeof(BITMAPINFOHEADER));
		BITMAPINFOHEADER *bmih = (PBITMAPINFOHEADER)pBuf;
		bmih->biHeight = -(long)dwLcdHeight;
		bmih->biWidth  = (long)dwLcdWidth;
		bmih->biBitCount = 32;
		bmih->biPlanes = 1;
		bmih->biSize = sizeof(BITMAPINFOHEADER);
		bmih->biCompression = BI_RGB;
		bmih->biClrUsed = 0;
		int biWidthBytes = bmih->biWidth*4;
		bmih->biSizeImage = -biWidthBytes*bmih->biHeight;	

		m_bmpInfo[i].hBmp = ::CreateDIBSection(NULL,(BITMAPINFO*)pBuf,DIB_RGB_COLORS,(LPVOID *)&m_bmpInfo[i].pBuf,0,0);
		
		delete [] pBuf;
	}
	m_bDraw = TRUE;

	return TRUE;
}

void  CCoolStatic::ClearBmpInfo()
{
	for(int i=0; i< MMI_MAX_LAYER_NUM; i++)
	{
		if(m_bmpInfo[i].hBmp)
		{
			DeleteObject(m_bmpInfo[i].hBmp);
		}
		// m_bmpInfo[i].pBuf is deleted in 	DeleteObject(m_bmpInfo[i].hBmp);
	}

}

int  CCoolStatic::GetUsedLayerNum()
{
	int nNum = 0;
	for(int i=0; i< MMI_MAX_LAYER_NUM; i++)
	{
		if(m_pLayerInfo[i].layer_info.is_enable > 0)
		{
			nNum++;
		}
	}
	return nNum;
}

void  CCoolStatic::CalcCanvasSize()
{
	int nUsedLayerNum =  GetUsedLayerNum();
	if(nUsedLayerNum == 0)
	{
		nUsedLayerNum = 4;
	}
	int dwMaxSide = GetMaxSide();
	m_iDocWidth =  ROW_LAYER_NUM * dwMaxSide;
	m_iDocHeight = ((nUsedLayerNum + ROW_LAYER_NUM -1)/ROW_LAYER_NUM) * dwMaxSide;
}

int CCoolStatic::GetMaxSide()
{
	return (int)((m_dwLCDHeight > m_dwLCDWidth ? m_dwLCDHeight : m_dwLCDWidth) + 50);
}

#ifdef X_GDI
void  CCoolStatic::DrawLayers(CDC *pDC,int x, int y, CRect rect)
#else
void  CCoolStatic::DrawLayers(Graphics &g,int x, int y, CRect rect)
#endif
{
	if(!m_bDraw)
	{
		return;
	}
	int nUsedNum = GetUsedLayerNum();
	CalcCanvasSize();

	int nMaxSide = GetMaxSide();

	int nFrameW = rect.Width();
	int nFrameH = rect.Height();

	int nFirstW = 0;
	int nFirstH = 0;

	CRect rcFrameInDoc;
	rcFrameInDoc.left = x;
	rcFrameInDoc.right = (x + rect.Width()) < m_iDocWidth ? (x + rect.Width()) : m_iDocWidth;
	rcFrameInDoc.top = y;
	rcFrameInDoc.bottom = (y + rect.Height()) < m_iDocHeight ? (y+ rect.Height()) : m_iDocHeight;

	


	int nRows = (nUsedNum + ROW_LAYER_NUM-1) / ROW_LAYER_NUM;
	for(int i = 0; i< nRows; i++)
	{	
		for(int j= 0; j< ROW_LAYER_NUM && (i*ROW_LAYER_NUM +j) < nUsedNum; j++)
		{	
			int nLyrDispW = 0;
			int nLyrDispH = 0;
			int nLyrX = 0;
			int nLyrY = 0;
			int nFrmX = 0;
			int nFrmY = 0;			

			CRect rcLayer;
			rcLayer.left = nMaxSide*j + (nMaxSide - m_dwLCDWidth)/8;
			rcLayer.right = rcLayer.left + m_dwLCDWidth;
			rcLayer.top = nMaxSide*i + (nMaxSide - m_dwLCDHeight)/2;
			rcLayer.bottom = rcLayer.top + m_dwLCDHeight;

			CRect rcInvalidate;
			rcInvalidate.left = rcLayer.left + m_rcInvalidate.left;
			rcInvalidate.right = rcInvalidate.left + m_rcInvalidate.Width();
			rcInvalidate.top = rcLayer.top + m_rcInvalidate.top;
			rcInvalidate.bottom = rcInvalidate.top + m_rcInvalidate.Height();

			CRect rcDisp;
			if(!rcDisp.IntersectRect(rcFrameInDoc,rcLayer))
			{
				continue; // no overlapped rect
			}	

			nLyrDispW = rcDisp.Width();
			nLyrDispH = rcDisp.Height();
			nLyrX     = rcDisp.left - rcLayer.left;
			nLyrY     = rcDisp.top - rcLayer.top;
			nFrmX     = rcDisp.left - rcFrameInDoc.left;
			nFrmY     = rcDisp.top  - rcFrameInDoc.top;
		
			CRect rcImgInFram;
			rcImgInFram.left = nFrmX;
			rcImgInFram.right = nFrmX + nLyrDispW;
			rcImgInFram.top = nFrmY;
			rcImgInFram.bottom = nFrmY+nLyrDispH;

	
			CRect rcInvliadteDisp;
			BOOL bInvalidate = rcInvliadteDisp.IntersectRect(rcInvalidate,rcDisp);
			if(bInvalidate)
			{
				rcInvliadteDisp.left -= rcLayer.left;
				rcInvliadteDisp.right -= rcLayer.left;
				rcInvliadteDisp.top -= rcLayer.top;
				rcInvliadteDisp.bottom-= rcLayer.top;
			}

			CRect rcRect; // layer local rect
			rcRect.left = m_pLayerInfo[i*ROW_LAYER_NUM + j].layer_info.pos.x + rcLayer.left;
			rcRect.top = m_pLayerInfo[i*ROW_LAYER_NUM + j].layer_info.pos.y + rcLayer.top;
			rcRect.right = rcRect.left + m_pLayerInfo[i*ROW_LAYER_NUM + j].layer_info.width;
			rcRect.bottom = rcRect.top + m_pLayerInfo[i*ROW_LAYER_NUM + j].layer_info.height;

			CRect rcRectang; 
			

#ifdef X_GDI
			//CBrush br(RGB(0,0,255));
			//pDC->FillRect(rcImgInFram,&br);

			// draw layer image
			CDC*	pDCTemp = new CDC;	
			
			pDCTemp->CreateCompatibleDC( pDC );		

			if(bInvalidate)
			{
				int w = rcInvliadteDisp.Width();
				int h = rcInvliadteDisp.Height();

				DWORD * pDst = (DWORD * )m_bmpInfo[i*ROW_LAYER_NUM + j].pBuf + rcInvliadteDisp.left + rcInvliadteDisp.top* m_dwLCDWidth;
				DWORD * pSrc = (DWORD * )m_pLayerInfo[i*ROW_LAYER_NUM + j].layer_info.buf_addr + rcInvliadteDisp.left + rcInvliadteDisp.top* m_dwLCDWidth;

				for(int iH = 0;iH< h; iH++)
				{
					memcpy(pDst,pSrc,w*4);
					pDst += m_dwLCDWidth;
					pSrc += m_dwLCDWidth;					
				}			
			}
			
//			memcpy(m_bmpInfo[i*ROW_LAYER_NUM + j].pBuf,(LPBYTE)m_pLayerInfo[i*ROW_LAYER_NUM + j].layer_info.buf_addr,m_dwLCDHeight*m_dwLCDWidth*4);

			// Select the bitmap into the temp device context
			HBITMAP hOldBmp = (HBITMAP) pDCTemp->SelectObject( m_bmpInfo[i*ROW_LAYER_NUM + j].hBmp );
		
			pDC->StretchBlt( nFrmX, nFrmY, nLyrDispW, nLyrDispH, pDCTemp, nLyrX, nLyrY, nLyrDispW, nLyrDispH, SRCCOPY );
				
			pDCTemp->SelectObject( hOldBmp );
				
			delete pDCTemp;	

			//draw border
			COLORREF colorBorder = RGB(0,255,0);
			if(m_bDrawBorder)
			{
				//draw left border
				if(rcDisp.left == rcLayer.left && rcDisp.left > rcFrameInDoc.left)
				{				
					pDC->Draw3dRect(nFrmX-1,nFrmY-1,1,nLyrDispH+2,colorBorder,colorBorder);					
				}
				//draw right border
				if(rcDisp.right == rcLayer.right && rcDisp.right < rcFrameInDoc.right)
				{
					pDC->Draw3dRect(nFrmX+nLyrDispW,nFrmY-1,1,nLyrDispH+2,colorBorder,colorBorder);	
				}
				//draw top border
				if(rcDisp.top == rcLayer.top && rcDisp.top > rcFrameInDoc.top)
				{
					pDC->Draw3dRect(nFrmX-1,nFrmY-1,nLyrDispW+2,1,colorBorder,colorBorder);					
				}
				//draw bottom border
				if(rcDisp.bottom == rcLayer.bottom && rcDisp.bottom < rcFrameInDoc.bottom)
				{
					pDC->Draw3dRect(nFrmX-1,nFrmY+nLyrDispH,nLyrDispW+2,1,colorBorder,colorBorder);						
				}
			}
			//draw RECT
			COLORREF colorRect = RGB(255,0,0);
			if(m_bDrawRect && rcRectang.IntersectRect(rcFrameInDoc,rcRect))
			{
				int nRctX = rcRectang.left- rcFrameInDoc.left;
				int nRctY = rcRectang.top- rcFrameInDoc.top;
				int nRctW = rcRectang.Width();
				int nRctH = rcRectang.Height();
				//draw left side
				if(rcRectang.left > rcFrameInDoc.left)
				{				
					pDC->Draw3dRect(nRctX-1,nRctY-1,1,nRctH+2,colorRect,colorRect);					
				}
				//draw right side
				if(rcRectang.right < rcFrameInDoc.right)
				{
					pDC->Draw3dRect(nRctX+nRctW,nRctY-1,1,nRctH+2,colorRect,colorRect);	
				}
				//draw top side
				if(rcRectang.top > rcFrameInDoc.top)
				{
					pDC->Draw3dRect(nRctX-1,nRctY-1,nRctW+2,1,colorRect,colorRect);					
				}
				//draw bottom side
				if(rcRectang.bottom < rcFrameInDoc.bottom)
				{
					pDC->Draw3dRect(nRctX-1,nRctY+nRctH,nRctW+2,1,colorRect,colorRect);						
				}
			}

			if(m_bDrawLayerInfo)
			{
				DrawLayerInfo(pDC,rcLayer.right - rcFrameInDoc.left, 
					          rcLayer.top- rcFrameInDoc.top,i*ROW_LAYER_NUM + j);
			}
#else
			Color color(0xFF0000FF);
			SolidBrush bs(color);
			g.FillRectangle(&bs,rcImgInFram.left,rcImgInFram.top,rcImgInFram.Width(),rcImgInFram.Height());
			
			Bitmap bmpLayer(m_bmpInfo[i].hBmp,NULL);
			//CachedBitmap cbmp(&bmpLayer,&g);		
			//g.DrawCachedBitmap(&cbmp, m_rcLCD.left,m_rcLCD.top);
			g.DrawImage(&bmpLayer,Rect(nFrmX,nFrmY,nLyrDispW,nLyrDispH),nLyrX,nLyrY,nLyrDispW,nLyrDispW,UnitPixel);
			
			//draw border
			if(m_bDrawBorder)
			{
				Color color(0xFF00FF00);
				Pen pen(color);
				//draw left border
				if(rcDisp.left == rcLayer.left && rcDisp.left > rcFrameInDoc.left)
				{											
					g.DrawRectangle(&pen,nFrmX-1,nFrmY-1,1,nLyrDispH+2);
				}
				//draw right border
				if(rcDisp.right == rcLayer.right && rcDisp.right < rcFrameInDoc.right)
				{					
					g.DrawRectangle(&pen,nFrmX+nLyrDispW+1,nFrmY-1,1,nLyrDispH+2);
				}
				//draw top border
				if(rcDisp.top == rcLayer.top && rcDisp.top > rcFrameInDoc.top)
				{				
					g.DrawRectangle(&pen,nFrmX-1,nFrmY-1,nLyrDispW+3,1);
				}
				//draw bottom border
				if(rcDisp.bottom == rcLayer.bottom && rcDisp.bottom < rcFrameInDoc.bottom)
				{					
					g.DrawRectangle(&pen,nFrmX-1,nFrmY+nLyrDispH+1,nLyrDispW+3,1);
				}
			}

#endif		
		}

	}

}

void CCoolStatic::LayerInvalidate()
{
	m_rcInvalidate.left = 0;
	m_rcInvalidate.right = m_dwLCDWidth;
	m_rcInvalidate.top   = 0;
	m_rcInvalidate.bottom = m_dwLCDHeight;
	CRect rcFrame;
	GetClientRect(rcFrame);
	InvalidateRect(rcFrame);

}
void CCoolStatic::LayerInvalidateRect(WORD left,WORD top,WORD right,WORD bottom)
{
	m_rcInvalidate.left = left;
	m_rcInvalidate.right = right;
	m_rcInvalidate.top   = top;
	m_rcInvalidate.bottom = bottom;

	CRect rcFrame;
	GetClientRect(rcFrame);
	InvalidateRect(rcFrame);
}


BOOL CCoolStatic::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
//	CRect rcFrame;
//	GetClientRect(rcFrame);
//	CBrush br(RGB(0,0,0));
//	pDC->FillRect(rcFrame,&br);
//	return CStatic::OnEraseBkgnd(pDC);
	return TRUE;
}

BOOL CCoolStatic::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	OnVScroll( ( zDelta > 0 )? SB_LINEUP:SB_LINEDOWN, 0, NULL );
	return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}
#ifdef X_GDI
void  CCoolStatic::DrawLayerInfo(CDC *pDC, int x, int y, int nLayerIndex)
{
	pDC->SetTextColor(RGB(255,255,255));
	CString strText;
	CRect rect;
	rect.left = x+5;
	rect.top = y;
	rect.right = x+200;
	rect.bottom = y+m_dwLCDHeight;
	//Draw left
	strText.Format(_T("Left: %d\nRight: %d\nWidth: %d\nHeight: %d\nAddr: 0x%08X\nType: %s"),
		m_pLayerInfo[nLayerIndex].layer_info.pos.x,
		m_pLayerInfo[nLayerIndex].layer_info.pos.y,
		m_pLayerInfo[nLayerIndex].layer_info.width,
		m_pLayerInfo[nLayerIndex].layer_info.height,
		m_pLayerInfo[nLayerIndex].layer_info.is_enable,
		GetDataTypeString(m_pLayerInfo[nLayerIndex].layer_info.layer_data.type));
	pDC->DrawText(strText,rect,DT_LEFT);
}
#endif

CString CCoolStatic::GetDataTypeString(int type)
{
	CString strTmp;
	strTmp.Format(_T("(%d)"),type);
	switch(type)
	{
	case DATA_TYPE_ARGB888:
		strTmp.Format(_T("ARGB888(%d)"),type);
		break;
	case DATA_TYPE_ARGB666:
		strTmp.Format(_T("ARGB666(%d)"),type);	
		break;
	case DATA_TYPE_ARGB565:	
		strTmp.Format(_T("ARGB565(%d)"),type);	
	    break;
	case DATA_TYPE_RGB565:	
		strTmp.Format(_T("RGB565(%d)"),type);
	    break;
	case DATA_TYPE_ARGB555:
		strTmp.Format(_T("ARGB555(%d)"),type);	
		break;
	case DATA_TYPE_GREY:
		strTmp.Format(_T("GREY(%d)"),type);
		break;
	case DATA_TYPE_YUV422:
		strTmp.Format(_T("YUV422(%d)"),type);	
	    break;
	case DATA_TYPE_YUV420:
		strTmp.Format(_T("YUV420(%d)"),type);
	    break;
	case DATA_TYPE_YUV400:
		strTmp.Format(_T("YUV400(%d)"),type);	
		break;
	case DATA_TYPE_PMARGB888:
		strTmp.Format(_T("PMARGB888(%d)"),type);		
		break;
	case DATA_TYPE_PMARGB666:
		strTmp.Format(_T("PMARGB666(%d)"),type);
		break;
	case DATA_TYPE_PMARGB565:
		strTmp.Format(_T("PMARGB565(%d)"),type);	
	    break;
	default:
	    break;
	}

	return strTmp;
}
