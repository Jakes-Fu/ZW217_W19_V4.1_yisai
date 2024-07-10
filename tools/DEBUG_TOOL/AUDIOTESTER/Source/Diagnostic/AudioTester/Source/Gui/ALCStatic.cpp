// ALCStatic.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "ALCStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CALCStatic

CALCStatic::CALCStatic()
{	
    m_pBKBitmap = NULL;
    m_iXAxisLength = 0;
    m_iYAxisLength = 0;
    
    m_pOldBitmap = NULL;

	memset(&m_data, 0, sizeof(ALC_CURVE_DATA));
}

CALCStatic::~CALCStatic()
{
	m_pOldBitmap = NULL;
    m_pBKBitmap = NULL;
}


BEGIN_MESSAGE_MAP(CALCStatic, CStatic)
	//{{AFX_MSG_MAP(CALCStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CALCStatic message handlers

void CALCStatic::OnPaint() 
{
	//CPaintDC dc(this); // device context for painting
	
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

void CALCStatic::DrawCoordinateAxis( CDC* pDC )
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
    pDC->TextOut(ptLine.x - 50, ptLine.y +10, "Vin(dB)" );

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
    pDC->TextOut(ptLine.x, ptLine.y, "Vout(dB)" );
    
    pDC->SelectObject( pOldBrush );
	pDC->SelectObject(pOldPen);
}

#define BASE_LENGTH 10
void CALCStatic::DrawCurve(CDC* pDC)
{
	CPen pen(PS_SOLID, 1, RGB(0,0,0));
	CGdiObject* pOldPen = pDC->SelectObject( &pen );
	CPen penPt(PS_SOLID, 5, RGB(0,0,0));

	double x = m_ptOrg.x;
    double y = m_ptOrg.y;
	double dXScale = m_iXAxisLength / ((m_data.y4 - m_data.y1) + (m_data.y4 - m_data.y3)*2 + BASE_LENGTH) ;
    double dYValueStep = m_iYAxisLength / ((m_data.y4 - m_data.y1) + BASE_LENGTH);

	CPoint ptFirst;
	ptFirst.x = x + dXScale * ((m_data.y2- m_data.y1)/2 + BASE_LENGTH);
	ptFirst.y = y - BASE_LENGTH * dYValueStep;

	pDC->MoveTo(CPoint(ptFirst.x, y));
	pDC->LineTo(ptFirst);

	pOldPen = pDC->SelectObject( &penPt );
	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptFirst);
	
	pOldPen = pDC->SelectObject( &pen );
	CPoint ptSecond;
	ptSecond.x = x + (BASE_LENGTH + m_data.y2 - m_data.y1) * dXScale;
	ptSecond.y = y - (BASE_LENGTH + m_data.y2 - m_data.y1) * dYValueStep;

	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptSecond);
	pDC->TextOut(ptSecond.x - 10, ptSecond.y + 10, _T("¦Â"));

	pOldPen = pDC->SelectObject( &penPt );
	pDC->MoveTo(ptSecond);
	pDC->LineTo(ptSecond);

	pOldPen = pDC->SelectObject( &pen );
	ptFirst = ptSecond;
	ptSecond.x = x + (BASE_LENGTH + m_data.y3 - m_data.y1) * dXScale;
	ptSecond.y = y - (BASE_LENGTH + m_data.y3 - m_data.y1) * dYValueStep;

	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptSecond);
	pDC->TextOut(ptSecond.x + 15, ptSecond.y - 10, _T("¦Á"));

	pOldPen = pDC->SelectObject( &penPt );
	pDC->MoveTo(ptSecond);
	pDC->LineTo(ptSecond);

	pOldPen = pDC->SelectObject( &pen );
	ptFirst = ptSecond;
	ptSecond.x = x + (BASE_LENGTH + m_data.y4 - m_data.y1 + (m_data.y4 - m_data.y3)*2) * dXScale;
	ptSecond.y = y - (BASE_LENGTH + m_data.y4 - m_data.y1) * dYValueStep;

	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptSecond);

	pOldPen = pDC->SelectObject( &penPt );
	pDC->MoveTo(ptSecond);
	pDC->LineTo(ptSecond);

	CPen dotPen(PS_DOT, 1, RGB(0,0,255));
	pOldPen = pDC->SelectObject( &dotPen );

	ptFirst.x = x;
	ptFirst.y = y - BASE_LENGTH * dYValueStep;

	ptSecond.x = x + dXScale * ((m_data.y2- m_data.y1)/2 + BASE_LENGTH);
	ptSecond.y = y - BASE_LENGTH * dYValueStep;

	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptSecond);

	CString strText;
	strText.Format(_T("A %d"), m_data.y1);
	pDC->TextOut(ptFirst.x - LEFT_EDGE_WITH, ptSecond.y, strText);

	ptFirst.x = ptSecond.x;
	ptFirst.y = y;

	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptSecond);

	ptFirst.x = x;
	ptFirst.y = y - (BASE_LENGTH + m_data.y2 - m_data.y1) * dYValueStep;
	
	ptSecond.x = x + (BASE_LENGTH + m_data.y2 - m_data.y1) * dXScale;
	ptSecond.y = y - (BASE_LENGTH + m_data.y2 - m_data.y1) * dYValueStep;
	
	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptSecond);
	strText.Format(_T("B %d"), m_data.y2);
	pDC->TextOut(ptFirst.x - LEFT_EDGE_WITH, ptSecond.y, strText);

	ptFirst.x = ptSecond.x;
	ptFirst.y = y;

	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptSecond);

	ptFirst.x = x;
	ptFirst.y = y - (BASE_LENGTH + m_data.y3 - m_data.y1) * dYValueStep;

	ptSecond.x = x + (BASE_LENGTH + m_data.y3 - m_data.y1) * dXScale;
	ptSecond.y = y - (BASE_LENGTH + m_data.y3 - m_data.y1) * dYValueStep;
	
	pDC->MoveTo(ptFirst);
	pDC->LineTo(CPoint(ptSecond.x + 50, ptSecond.y));
	strText.Format(_T("C %d"), m_data.y3);
	pDC->TextOut(ptFirst.x - LEFT_EDGE_WITH, ptSecond.y, strText);

	ptFirst.x = ptSecond.x;
	ptFirst.y = y;

	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptSecond);

	ptFirst.x = x;
	ptFirst.y = y - (BASE_LENGTH + m_data.y4 - m_data.y1) * dYValueStep;

	ptSecond.x = x + (BASE_LENGTH + m_data.y4 - m_data.y1 + (m_data.y4 - m_data.y3)*2) * dXScale;
	ptSecond.y = y - (BASE_LENGTH + m_data.y4 - m_data.y1) * dYValueStep;
	
	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptSecond);
	strText.Format(_T("D %d"), m_data.y4);
	pDC->TextOut(ptFirst.x - LEFT_EDGE_WITH, ptSecond.y, strText);

	ptFirst.x = ptSecond.x;
	ptFirst.y = y;

	pDC->MoveTo(ptFirst);
	pDC->LineTo(ptSecond);
	
	pDC->SelectObject(pOldPen);

}

void CALCStatic::SetData(ALC_CURVE_DATA data)
{
	memcpy(&m_data, &data, sizeof(ALC_CURVE_DATA));
}