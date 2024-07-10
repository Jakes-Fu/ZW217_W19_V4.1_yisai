// CoolStatic.cpp : implementation file
//

#include "stdafx.h"
#include "FontTool.h"
#include "CoolStatic.h"
#include "FontToolDlg.h"
#include "Font_Interface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCoolStatic

CCoolStatic::CCoolStatic()
{
	m_nFBBX = 0;
	m_nFBBY = 0;
    m_bIsDown = FALSE;
	m_bDrawGrid = FALSE;
	//m_uBkColor = RGB(212,208,200);
	m_uBkColor = RGB(255,255,255);
	m_uBkColor2 = RGB(248,250,146);

	m_pChar = NULL;
	m_pRegion = NULL;
	m_b2Step = FALSE;
	m_bSlider = FALSE;
	m_pRegionLeft = NULL;
	m_nDiff = 0;
	m_nRegionLUX = 0;
	m_nRegionLUY = 0;
	m_nRegionWidth = 0;
	m_nRegionHeight = 0;
	
	m_bOnePixel = TRUE;
	
	m_pGlyph = NULL;

	m_bEdit = FALSE;

	m_nMaxBearingY = 0;

	m_bAlpha = FALSE;

	m_uUnicode = 0;

	m_bNormalGlyf = TRUE;
}

CCoolStatic::~CCoolStatic()
{
	if(m_pRegion != NULL)
	{
		delete [] m_pRegion;
		m_pRegion = NULL;
	}

	if(m_pRegionLeft != NULL)
	{
		delete [] m_pRegionLeft;
		m_pRegionLeft = NULL;
	}
	m_pChar  = NULL;
	m_pGlyph = NULL;
}


BEGIN_MESSAGE_MAP(CCoolStatic, CStatic)
	//{{AFX_MSG_MAP(CCoolStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoolStatic message handlers

void CCoolStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
// 	if(m_nFBBX == 0 || m_nFBBY == 0)
// 		return;
	CRect rect;
	GetClientRect(&rect);
	int nWidth = rect.Width();
	int nHeight = rect.Height();

	if(m_bSlider)
	{
		AdjustRect(m_nDiff, rect);
	}
	
	CDC memdc;
	CBitmap map;
	memdc.CreateCompatibleDC(&dc);

	map.CreateCompatibleBitmap(&dc, nWidth, nHeight);
	CBitmap *pold = memdc.SelectObject(&map);
	CBrush brush;
	if(m_bNormalGlyf)
		brush.CreateSolidBrush(m_uBkColor);
	else
		brush.CreateSolidBrush(m_uBkColor2);
	memdc.FillRect(rect, &brush);
//	DrawSide((CPaintDC *)&memdc,rect,m_bIsDown);
	DrawSide(&dc,rect,m_bIsDown);
	
	if(m_nFBBX != 0 && m_nFBBY != 0)
	{
		int dx = rect.Width()/m_nFBBX;
		int dy = rect.Height()/m_nFBBY;

		int dMin = (dx < dy)?dx:dy;

		if(m_bOnePixel && dMin>1)
		{
			dMin = 1;
		}

		if(m_bSlider)
		{
			dMin = 0;
		}

		int nBaseline = DrawGlyph((CPaintDC *)&memdc,rect,dMin);

		if(m_bDrawGrid && dMin > 1)
		{
			DrawGrid((CPaintDC *)&memdc,rect,dMin,nBaseline);
		}

		
	}

	dc.BitBlt(0,0,nWidth,nHeight,&memdc,0,0,SRCCOPY);
	map.DeleteObject();
	memdc.SelectObject(pold);
	memdc.DeleteDC();

	// Do not call CStatic::OnPaint() for painting messages
}

void CCoolStatic::DrawGrid(CPaintDC *pDC, CRect &rect, int dMin, int nBaseline/* = -1*/)
{
	if(m_nFBBX == 0 || m_nFBBY == 0)
		return;
	CPen pen;
	pen.CreatePen(PS_SOLID, 0, RGB(192,192,192));
	CPen *oldPen = pDC->SelectObject(&pen);

	//draw grid.
	CRect rcCenter(&rect);
	int wOff = (rect.Width() - dMin*m_nFBBX)%2;
	int hOff = (rect.Height() - dMin*m_nFBBY)%2;
	rcCenter.left += (rect.Width() - dMin*m_nFBBX)/2 ;
	rcCenter.right -= ((rect.Width() - dMin*m_nFBBX)/2 +wOff);
	rcCenter.bottom -= (rect.Height() - dMin*m_nFBBY)/2;
	rcCenter.top += ((rect.Height() - dMin*m_nFBBY)/2 + hOff);
		
	for(int i = 0; i <= m_nFBBX; i++)
	{
		pDC->MoveTo(rcCenter.left + i * dMin, rcCenter.bottom);
		pDC->LineTo(rcCenter.left + i * dMin, rcCenter.bottom - dMin * m_nFBBY);
	}

	for(i = 0; i <= m_nFBBY; i++)
	{
		pDC->MoveTo(rcCenter.left, rcCenter.bottom - i * dMin);
		pDC->LineTo(rcCenter.left + dMin * m_nFBBX , rcCenter.bottom - i * dMin);
	}

	pDC->SelectObject(oldPen);

	if(nBaseline != -1)
	{
		CPen penRed;
		penRed.CreatePen(PS_SOLID, 0, RGB(255,0,0));
		CPen *oldPen2 = pDC->SelectObject(&penRed);
		
		pDC->MoveTo(rcCenter.left, rcCenter.bottom - (nBaseline + 1) * dMin);
		pDC->LineTo(rcCenter.left + dMin * m_nFBBX , rcCenter.bottom - (nBaseline + 1 ) * dMin);
		
		pDC->SelectObject(oldPen2);
		
	}
	
}

int CCoolStatic::DrawGlyph(CPaintDC *pDC, CRect &rect, int dMin)
{
	int nBaseline = -1;
	if(m_pGlyph == NULL)
	{		
		return nBaseline;
	}
	else if(m_pGlyph->bitmap  == NULL)
	{
		return nBaseline;
	}

	if(m_bEdit && (m_pChar == NULL || m_pChar->bBitmap == NULL) )
	{
		return nBaseline;
	}

	CRect rcCenter(&rect);
	double fWRate = 1.0;
	double fHRate = 1.0;
	if(dMin != 0)
	{
		int wOff = (rect.Width() - dMin*m_nFBBX)%2;
		int hOff = (rect.Height() - dMin*m_nFBBY)%2;
		rcCenter.left += (rect.Width() - dMin*m_nFBBX)/2 ;
		rcCenter.right -= ((rect.Width() - dMin*m_nFBBX)/2 +wOff);
		rcCenter.bottom -= (rect.Height() - dMin*m_nFBBY)/2;
		rcCenter.top += ((rect.Height() - dMin*m_nFBBY)/2 + hOff);
		if(!m_bEdit)
		{
			rcCenter.top += (m_nFBBY*5/6-m_pGlyph->y)*dMin;
			//rcCenter.top +=(m_nMaxBearingY-m_pGlyph->y)*dMin;
			rcCenter.left += (m_pGlyph->x)*dMin;

			//nBaseline = m_nFBBY*5/6-m_pGlyph->y;
			nBaseline = (m_nFBBY - m_nFBBY*5/6);
		}
	}
	else
	{
		if(m_nFBBX > m_nFBBY)
		{
			int nHeight = (int)(((double)m_nFBBY / (double)m_nFBBX) * (double)rcCenter.Height()); //lint !e414
			if(nHeight > rcCenter.Height())
			{
				nHeight = rcCenter.Height();
			}

			int hOff = (rect.Height() - nHeight)%2;
			rcCenter.bottom -= (rect.Height() - nHeight)/2;
			rcCenter.top += ((rect.Height() - nHeight)/2 + hOff);

			fHRate =  (double)((double)nHeight / (double)m_nFBBY);//lint !e414
			fWRate =  (double)((double)rcCenter.Width() / (double)m_nFBBX);//lint !e414
		}
		else
		{
			int nWidth = (int)(((double)m_nFBBX / (double)m_nFBBY) * (double)rcCenter.Width());//lint !e414
			if(nWidth > rcCenter.Width())
			{
				nWidth = rcCenter.Width();
			}
			int wOff = (rect.Width() - nWidth)%2;	
			rcCenter.left += (rect.Width() - nWidth)/2 ;
			rcCenter.right -= ((rect.Width() - nWidth)/2 +wOff);

			fHRate =  (double)((double)rcCenter.Height() / (double)m_nFBBY);//lint !e414
			fWRate =  (double)((double)nWidth  / (double)m_nFBBX);//lint !e414
	
		}

		if(!m_bEdit)
		{
			rcCenter.top += (LONG)((m_nFBBY*5.0/6.0-m_pGlyph->y)*fHRate);
			//rcCenter.top += (LONG)((m_nMaxBearingY-m_pGlyph->y)*fHRate);
			rcCenter.left += (LONG)((m_pGlyph->x) *fWRate);

			//nBaseline = (m_nFBBY*5/6-m_pGlyph->y);
			nBaseline = (m_nFBBY - m_nFBBY*5/6);
		}
	}

	BYTE *pBitMap = m_pGlyph->bitmap;
	int height = m_pGlyph->height;
	int width  = m_pGlyph->width;
	int nFullW = (m_pGlyph->width+7) / 8;
	if(m_bAlpha)
	{
		nFullW = m_pGlyph->width;
	}
	
	BYTE bTmp = 0;
	LPBYTE pCurLine = NULL;

	int nPich = 8;
	if(m_bAlpha)
	{
		nPich = 1;
	}

	if(m_bEdit && m_pChar != NULL)
	{
		pBitMap = m_pChar->bDotMatrix;
		height = m_nFBBY;
		width  = m_nFBBX;
		nFullW = m_nFBBX;
		nPich = 1;
	}


	for(int i= 0; i<height; i++)
	{
		pCurLine = pBitMap + (nFullW * i);
		int nCount = 0;
		for( int j=0; j<nFullW; j++)
		{
			bTmp = *(pCurLine + j);
			
			for(int k=0; k<nPich && nCount < width; k++)
			{
				nCount++;
				BYTE b = bTmp;

				if(!m_bAlpha && !m_bEdit)
				{
					b = (BYTE)(b<<k);
		     		b = (BYTE)(b>>7);

					if(b==1)
					{
						if(dMin != 0)
						{
							CRect rcBlack(rcCenter.left + dMin * (j*8 + k), rcCenter.top + dMin * i, rcCenter.left +dMin * (j*8 + k +1), rcCenter.top  + dMin * (i+1));
							CBrush brush;
							brush.CreateSolidBrush(RGB(0,0,0));
							pDC->FillRect(rcBlack, &brush);	
						}
						else
						{
							CRect rcBlack(rcCenter.left + (int)((j*8 + k)*fWRate), rcCenter.top + (int)(fHRate * i), rcCenter.left +(int)(fWRate * (j*8 + k +1)), rcCenter.top  + (int)(fHRate * (i+1)));
							CBrush brush;
							brush.CreateSolidBrush(RGB(0,0,0));
							pDC->FillRect(rcBlack, &brush);	
						}	
					}
				}
				else
				{		
					BYTE color = (BYTE)(b);
					color = (BYTE)(255 - color);	
					if(color == 0xFF)
						continue;

					if(dMin != 0)
					{
						CRect rcBlack(rcCenter.left + dMin * (j*nPich + k), rcCenter.top + dMin * i, rcCenter.left +dMin * (j*nPich + k +1), rcCenter.top  + dMin * (i+1));
						CBrush brush;
						brush.CreateSolidBrush(RGB(color,color,color));
						pDC->FillRect(rcBlack, &brush);	
					}
					else
					{
						CRect rcBlack(rcCenter.left + (int)((j*nPich + k)*fWRate), rcCenter.top + (int)(fHRate * i), rcCenter.left +(int)(fWRate * (j*nPich + k +1)), rcCenter.top  + (int)(fHRate * (i+1)));
						CBrush brush;
						brush.CreateSolidBrush(RGB(color,color,color));
						pDC->FillRect(rcBlack, &brush);	
					}

				}
			}
		}

	}

	return nBaseline;
}
void CCoolStatic::DrawSide(CPaintDC *pDC, CRect &rect,BOOL bDown /*= FALSE*/)
{
	if(m_nFBBX == 0 || m_nFBBY == 0)
		return;

	if(bDown)
	{
		pDC->Draw3dRect(rect.left - 2, rect.top - 2, rect.Width() + 4, rect.Height() + 4, RGB(64,64,64), RGB(212,208,200));
		pDC->Draw3dRect(rect.left - 3, rect.top - 3, rect.Width() + 6, rect.Height() + 6, RGB(128,128,128), RGB(255,255,255));
	}
	else
	{
		pDC->Draw3dRect(rect.left - 2, rect.top - 2, rect.Width() + 4, rect.Height() + 4, RGB(255,255,255), RGB(128,128,128));
		pDC->Draw3dRect(rect.left-3, rect.top-3, rect.Width()+ 6,  rect.Height() + 6, RGB(212,208,200), RGB(64,64,64));
	}
}

void CCoolStatic::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bIsDown = TRUE;
	Invalidate();
	CStatic::OnLButtonDown(nFlags, point);
}

BOOL CCoolStatic::SaveCharByPose(PBDF_CHAR_T pChar,int nX,int nY, int nPtValue)
{
	if(m_nFBBX == 0 || pChar == NULL || nX < 0 || nY< 0 || nY >= m_nFBBY || nX >= m_nFBBX)
		return FALSE;

	pChar->bDotMatrix[nY*m_nFBBX + nX] = (BYTE)nPtValue;

	return TRUE;

}
BOOL CCoolStatic::MoveSelRegion(PBDF_CHAR_T pChar,int nSrcLUX,int nSrcLUY, 
								int nWidth,int nHeight,int nDstLUX,int nDstLUY)
{
	int nPtValue = 0;
	int nTmpPtValue = 0;

	int i=0;
	int j=0;
	//erease the src
	if(!m_b2Step)
	{
		m_pRegion = new BYTE[m_nFBBX*m_nFBBY];
		m_nRegionLUX = nSrcLUX;
		m_nRegionLUY = nSrcLUY;

		memcpy(m_pRegion,pChar->bDotMatrix,m_nFBBX*m_nFBBY);
		for(i=0; i< nHeight; i++)
		{
			for(j= 0; j< nWidth; j++)
			{
				SaveCharByPose(pChar,nSrcLUX+j,nSrcLUY+i,0);
			}
		}
		m_pRegionLeft = new BYTE[m_nFBBX*m_nFBBY];
		memcpy(m_pRegionLeft,pChar->bDotMatrix,m_nFBBX*m_nFBBY);

	}

	memcpy(pChar->bDotMatrix,m_pRegionLeft,m_nFBBX*m_nFBBY);
	//draw dst
	for(i=0; i< nHeight; i++)
	{		
		for(j= 0; j< nWidth; j++)
		{			
			nPtValue = GetPtValue(m_pRegion,m_nRegionLUX+j,m_nRegionLUY+i);
			nTmpPtValue = GetPtValue(m_pRegionLeft,nDstLUX+j,nDstLUY+i);
			//nPtValue = nTmpPtValue;
			SaveCharByPose(pChar,nDstLUX+j,nDstLUY+i,nPtValue);
		}
	}

	return TRUE;

}

int  CCoolStatic::GetPtValue(LPBYTE lpbDotMatrix,int nX,int nY)
{
	if(lpbDotMatrix == NULL)
	{
		return 0;
	}

	return lpbDotMatrix[nY*m_nFBBX + nX];

// 	int nFullW = (int)(( m_nFBBX * nPich + 7 )/ 8 );
// 	BYTE bCurByte = *(lpbDotMatrix + ( nY * nFullW + (int)(nX*nPich / 8) ));
// 
// 	int nDt = (nX*nPich) % 8;
// 	if(nDt != 0)
// 	{
// 		bCurByte = (BYTE)(bCurByte << nDt);
// 		if((8-nDt) < nPich)
// 		{
// 			BYTE bNextByte = *(lpbDotMatrix + ( nY * nFullW + (int)(nX*nPich / 8)  + 1) );
// 		
// 			bCurByte |= (BYTE)(bNextByte>>(8-nDt));
// 
// 		}
// 	}
// 
// 	bCurByte = (BYTE)(bCurByte>>(8-nPich));

// 	int nFullW = m_nFBBX / 8;
// 	if(m_nFBBX % 8 != 0)
// 		nFullW++;
// 	
// 	BYTE bCurByte = *(lpbDotMatrix + ( nY * nFullW + (int)(nX / 8) ));
// 	int nDt = nX % 8;
// 	BYTE bBase = 1;
// 	bBase = (BYTE)(bBase << (7-nDt));
// 
// 	bCurByte = (BYTE)(bCurByte & bBase);
// 	bCurByte = (BYTE)(bCurByte>>(7-nDt));

//	return (int)bCurByte;
}

void CCoolStatic::Set2Step(BOOL b2Step)
{
	m_b2Step = b2Step;
	if(!b2Step && m_pRegion != NULL)
	{
		if(m_pRegion != NULL)
		{
			delete [] m_pRegion;
			m_pRegion = NULL;
		}

		if(m_pRegionLeft != NULL)
		{
			delete [] m_pRegionLeft;
			m_pRegionLeft = NULL;
		}

	}
}

void CCoolStatic::AdjustDmin(int nDiff, int* dMin)
{
	*dMin += nDiff;
}
void CCoolStatic::AdjustRect(int nDiff, CRect &rc)
{
	rc.left -= nDiff;
	rc.right += nDiff;
	rc.top   -= nDiff;
	rc.bottom  += nDiff;
}
//BOOL CCoolStatic::OnEraseBkgnd(CDC* pDC) 
//{
//	// TODO: Add your message handler code here and/or call default
//	if(m_nFBBX == 0 || m_nFBBY == 0)
//		return CStatic::OnEraseBkgnd(pDC);
//
//	CRect rect;
//	GetClientRect(&rect);
//
//	int dx = rect.Width()/m_nFBBX;
//	int dy = rect.Height()/m_nFBBY;
//
//	int dMin = (dx < dy)?dx:dy;
//
//	if(m_bSlider)
//	{
//		AdjustDmin(m_nDiff, &dMin);
//	}
//	
//	CDC memdc;
//	CBitmap map;
//	memdc.CreateCompatibleDC(pDC);
//	int nWidth = rect.Width();
//	int nHeight = rect.Height();
//	map.CreateCompatibleBitmap(pDC, nWidth, nHeight);
//	CBitmap *pold = memdc.SelectObject(&map);
//	CBrush brush;
//	brush.CreateSolidBrush(m_uBkColor);
//	memdc.FillRect(rect, &brush);
//	DrawSide((CPaintDC *)&memdc,rect,m_bIsDown);
////	DrawSide(&dc,rect,m_bIsDown);
//	
//	if(m_bDrawGrid)
//	{
//		DrawGrid((CPaintDC *)&memdc,rect,dMin);
////		DrawGrid(&dc,rect,dMin);
//	}
//
//	DrawGlyph((CPaintDC *)&memdc,rect,dMin);
////	DrawGlyph(&dc,rect,dMin);
//
//	pDC->BitBlt(0,0,nWidth,nHeight,&memdc,0,0,SRCCOPY);
//	memdc.SelectObject(pold);
//	memdc.DeleteDC();
//	
//	return CStatic::OnEraseBkgnd(pDC);
//}


/**
 *  | 1 | 0 | 1 | 0 | 1 | 0 | 1 | 0 |
 *  0   1   2   3   4   5   6   7
 *      |___________|
 *          010
 *  nPos = 1
 *  nNumBits=3
 *  return: 010
 *
 */
BYTE CCoolStatic::GetBits(BYTE b,int nPos,int nNumBits)
{
	if(nPos<0 || nPos >=8 )
		return 0;
	if( ( nPos + nNumBits)>8 || nNumBits < 0 )
		return 0;

	return (BYTE)(((BYTE)(b<<nPos))>>(8-nNumBits));
}

BOOL CCoolStatic::SetBits(BYTE *pb,int nPos,int nNumBits,BYTE value)
{
	if(nPos<0 || nPos >=8 )
		return FALSE;
	if( ( nPos + nNumBits)>8 || nNumBits < 0 )
		return FALSE;

	BYTE b = *pb;

	value = (BYTE)(value<<(8-nPos-nNumBits));

	value = (BYTE)(value | (BYTE)((((BYTE)(b>>(8-nPos)))<<(8-nPos))));
	value = (BYTE)(value | (BYTE)((((BYTE)(b<<(nNumBits+nPos)))>>(nNumBits+nPos))));

	*pb = value;

	return TRUE;
}


