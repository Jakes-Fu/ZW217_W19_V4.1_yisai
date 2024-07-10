// ColorStatic.cpp : implementation file
//
// implement the background self draw
// anli.wei
// 2006-02-26
// use info: CColorStatic m_stc; ... m_stc.SetBkcolor(...); m_stc.Invalidate();

#include "stdafx.h"
#include "ColorStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorStatic

CColorStatic::CColorStatic()
{
    m_colorBK = RGB(100, 100, 100);
}

CColorStatic::~CColorStatic()
{
}


BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	//{{AFX_MSG_MAP(CColorStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorStatic message handlers

void CColorStatic::SetBkColor(COLORREF colorBK)
{
    m_colorBK = colorBK;
}

void CColorStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

    CRect rc;
    GetClientRect(&rc);

    CBrush bkBrush;
    bkBrush.CreateSolidBrush(m_colorBK);

    dc.FillRect(rc, &bkBrush);

    bkBrush.DeleteObject();
	// Do not call CStatic::OnPaint() for painting messages
}

BOOL CColorStatic::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//
	if(pDC != NULL)
	{
	}
    return FALSE;
	//return CStatic::OnEraseBkgnd(pDC);
}
