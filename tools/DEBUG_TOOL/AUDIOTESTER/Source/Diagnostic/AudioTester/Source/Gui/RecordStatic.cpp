// RecordStatic.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "RecordStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecordStatic

CRecordStatic::CRecordStatic()
{
	m_pBKBitmap = NULL;
    m_iXAxisLength = 0;
    m_iYAxisLength = 0;
    
    m_pOldBitmap = NULL;
	m_bCompressor = TRUE;
}

CRecordStatic::~CRecordStatic()
{
	m_pOldBitmap = NULL;
    m_pBKBitmap = NULL;
}


BEGIN_MESSAGE_MAP(CRecordStatic, CStatic)
	//{{AFX_MSG_MAP(CRecordStatic)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordStatic message handlers

void CRecordStatic::OnPaint() 
{
//	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
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
	m_memDC.FillSolidRect(&rect,RGB(212,208,200));
    DrawCoordinateAxis( &m_memDC );
	DrawCurve( &m_memDC );
	
	dc.BitBlt(0,0,rect.Width(),rect.Height(),&m_memDC,0,0,SRCCOPY);
	
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CRecordStatic::DrawCoordinateAxis( CDC* pDC )
{
    CRect rect;
    this->GetClientRect( rect );
    pDC->MoveTo(m_ptOrg);
    CPoint ptLine( m_ptOrg );
    
    CBrush redBrush(RGB(255,0,0));
    CBrush* pOldBrush = pDC->SelectObject( &redBrush );
    //x axis
	CPen pen(PS_SOLID, 0, RGB(0,0,0));
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
    pDC->TextOut(ptLine.x - 50, ptLine.y +10, "time" );

    m_iXAxisLength = rect.right - RIGHT_EDGE_WITH - LEFT_EDGE_WITH - ARROW_X_LENGTH - SCALE_X_OFFSET;
    m_iYAxisLength = rect.bottom - BOTTOM_EDGE_WITH - TOP_EDGE_WITH - ARROW_X_LENGTH - SCALE_Y_OFFSET;
	
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
    //pDC->TextOut(ptLine.x, ptLine.y, "DP input gain" );
    
    pDC->SelectObject( pOldBrush );
	pDC->SelectObject(pOldPen);
}

#define BASE_LENGTH 10
#define POINT_STEP 4
void CRecordStatic::DrawCurve(CDC* pDC)
{
	CPen pen(PS_SOLID, 2, RGB(0,0,0));
	CGdiObject* pOldPen = pDC->SelectObject( &pen );
	CPen penPt(PS_DOT, 1, RGB(0,0,0));

	double x = m_ptOrg.x;
    double y = m_ptOrg.y;
	double dXScale = m_iXAxisLength / POINT_STEP ;
    double dYValueStep = m_iYAxisLength / POINT_STEP;

	CPoint ptFirst;
	ptFirst.x = x;
	ptFirst.y = y - 3 * dYValueStep;

	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptFirst);
	
	pOldPen = pDC->SelectObject( &pen );
	CPoint ptSecond;
	ptSecond.x = x + 2 * dXScale / 3;
	ptSecond.y = y - 3 * dYValueStep;

	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptSecond);
	//pDC->TextOut(ptSecond.x - 10, ptSecond.y + 10, _T("¦Â"));

	pOldPen = pDC->SelectObject( &pen );
	ptFirst = ptSecond;
	ptSecond.x = x + dXScale;
	ptSecond.y = y - dYValueStep;

	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptSecond);
	//pDC->TextOut(ptSecond.x + 15, ptSecond.y - 10, _T("¦Á"));


	pOldPen = pDC->SelectObject( &pen );
	ptFirst = ptSecond;
	ptSecond.x = x + 2 * dXScale;
	ptSecond.y = y - dYValueStep;

	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptSecond);

	ptFirst = ptSecond;
	ptSecond.x = x + 3 * dXScale;
	ptSecond.y = y - 3 * dYValueStep;
	
	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptSecond);

	ptFirst = ptSecond;
	ptSecond.x = x + 7 * dXScale / 2;
	ptSecond.y = y - 3 * dYValueStep;
	
	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptSecond);

	//text out y
	pDC->TextOut(x - 10, y - 5, _T("0"));
	pDC->TextOut(x - 10, y - dYValueStep - 5, _T("1"));
	pDC->TextOut(x - 40, ptFirst.y - 5, _T("DP"));
	pDC->TextOut(x - 40, ptFirst.y + 10, _T("input"));
	pDC->TextOut(x - 40, ptFirst.y + 25, _T("gain"));

	pOldPen = pDC->SelectObject( &penPt );

	ptFirst.x = x;
	ptFirst.y = y - dYValueStep;
	ptSecond.x = x + dXScale;
	ptSecond.y = y - dYValueStep;
	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptSecond);

	ptFirst.x = x + 2*dXScale/3;
	ptFirst.y = y - 3*dYValueStep - 10;
	ptSecond.x = x + 2*dXScale/3;
	ptSecond.y = y - dYValueStep + 10;
	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptSecond);

	ptFirst.x = x + dXScale;
	ptFirst.y = y - 3*dYValueStep - 10;
	ptSecond.x = x + dXScale;
	ptSecond.y = y - dYValueStep + 10;
	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptSecond);

	ptFirst.x = x + 5*dXScale/3;
	ptFirst.y = y - 3*dYValueStep - 10;
	ptSecond.x = x + 5*dXScale/3;
	ptSecond.y = y - dYValueStep + 10;
	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptSecond);

	ptFirst.x = x + 2*dXScale;
	ptFirst.y = y - 3*dYValueStep - 10;
	ptSecond.x = x + 2*dXScale;
	ptSecond.y = y - dYValueStep + 10;
	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptSecond);

	ptFirst.x = x + 3*dXScale;
	ptFirst.y = y - 3*dYValueStep - 10;
	ptSecond.x = x + 3*dXScale;
	ptSecond.y = y - dYValueStep + 10;
	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptSecond);

	if(m_bCompressor)
	{
		pDC->TextOut(x + 2*dXScale/3 - 10, y - dYValueStep + 10, _T("Compress_"));
		pDC->TextOut(x + 2*dXScale/3 - 10, y - dYValueStep + 20, _T("attack"));
		pDC->TextOut(x + 5*dXScale/3 - 10, y - dYValueStep + 10, _T("Compress_"));
		pDC->TextOut(x + 5*dXScale/3 - 10, y - dYValueStep + 20, _T("hold"));
		pDC->TextOut(x + 2*dXScale + 10, y - 3*dYValueStep - 20, _T("Compress_"));
		pDC->TextOut(x + 2*dXScale + 10, y - 3*dYValueStep - 10, _T("release"));
	}
	else
	{
		pDC->TextOut(x + 2*dXScale/3 - 10, y - dYValueStep + 10, _T("Expander_"));
		pDC->TextOut(x + 2*dXScale/3 - 10, y - dYValueStep + 20, _T("attack"));
		pDC->TextOut(x + 5*dXScale/3 - 10, y - dYValueStep + 10, _T("Expander_"));
		pDC->TextOut(x + 5*dXScale/3 - 10, y - dYValueStep + 20, _T("hold"));
		pDC->TextOut(x + 2*dXScale + 10, y - 3*dYValueStep - 20, _T("Expander_"));
		pDC->TextOut(x + 2*dXScale + 10, y - 3*dYValueStep - 10, _T("release"));
	}

	ptFirst.x = x + 2*dXScale/3 - 10;
	ptFirst.y = y - dYValueStep + 5;
	ptSecond.x = x + 2*dXScale/3;
	ptSecond.y = y - dYValueStep + 5;
	DrawArrow(pDC, ptFirst, ptSecond, -5);

	ptFirst.x = x + dXScale + 10;
	ptFirst.y = y - dYValueStep + 5;
	ptSecond.x = x + dXScale;
	ptSecond.y = y - dYValueStep + 5;
	DrawArrow(pDC, ptFirst, ptSecond, 5);

	ptFirst.x = x + 5*dXScale/3 - 10;
	ptFirst.y = y - dYValueStep + 5;
	ptSecond.x = x + 5*dXScale/3;
	ptSecond.y = y - dYValueStep + 5;
	DrawArrow(pDC, ptFirst, ptSecond, -5);

	ptFirst.x = x + 2*dXScale + 10;
	ptFirst.y = y - dYValueStep + 5;
	ptSecond.x = x + 2*dXScale;
	ptSecond.y = y - dYValueStep + 5;
	DrawArrow(pDC, ptFirst, ptSecond, 5);

	ptFirst.x = x + 2*dXScale - 10;
	ptFirst.y = y - 3*dYValueStep - 5;
	ptSecond.x = x + 2*dXScale;
	ptSecond.y = y - 3*dYValueStep - 5;
	DrawArrow(pDC, ptFirst, ptSecond, -5);

	ptFirst.x = x + 3*dXScale + 10;
	ptFirst.y = y - 3*dYValueStep - 5;
	ptSecond.x = x + 3*dXScale;
	ptSecond.y = y - 3*dYValueStep - 5;
	DrawArrow(pDC, ptFirst, ptSecond, 5);

	pDC->SelectObject(pOldPen);

}

void CRecordStatic::DrawArrow(CDC* pDC, CPoint p1, CPoint p2, int nStep)
{
	CPen pen(PS_DOT, 1, RGB(0,0,0));
	CGdiObject* pOldPen = pDC->SelectObject( &pen );

	pDC->MoveTo(p1);
	pDC->LineTo(p2);

	CPoint pt1, pt2;
	pt1.x = p2.x + nStep;
	pt1.y = p2.y + 3;

	pt2.x = pt1.x;
	pt2.y = p2.y - 3;

	pDC->MoveTo(pt1);
	pDC->LineTo(p2);
	pDC->MoveTo(pt2);
	pDC->LineTo(p2);


	pDC->SelectObject(pOldPen);

}

void CRecordStatic::OnDestroy() 
{
	CStatic::OnDestroy();
	
	// TODO: Add your message handler code here
	if( m_pBKBitmap )
    {
        delete m_pBKBitmap;
        m_pBKBitmap = NULL;
    }
}
