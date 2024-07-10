// CoolStatic.cpp : implementation file
//

#include "stdafx.h"
#include "CoolStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCoolStatic

CCoolStatic::CCoolStatic()
{
	m_nColor = RGB(255,255,255);
}

CCoolStatic::~CCoolStatic()
{
}


BEGIN_MESSAGE_MAP(CCoolStatic, CStatic)
	//{{AFX_MSG_MAP(CCoolStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoolStatic message handlers

void CCoolStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(&rect);
	CBrush brush;
	brush.CreateSolidBrush(m_nColor);
	dc.FillRect(&rect,&brush);
	// Do not call CStatic::OnPaint() for painting messages
}
void CCoolStatic::SetColor(UINT rgb)
{
	m_nColor = rgb;
}