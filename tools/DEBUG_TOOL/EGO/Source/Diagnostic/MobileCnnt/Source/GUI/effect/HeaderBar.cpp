// HeaderBar.cpp : implementation file
//

#include "stdafx.h"
#include "HeaderBar.h"

#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const COLORREF CHeaderBar::CLR_FORE_GRND = RGB(255, 255, 255);
const COLORREF CHeaderBar::CLR_BACK_GRND = RGB(98, 115, 156);
/////////////////////////////////////////////////////////////////////////////
// CHeaderBar

CHeaderBar::CHeaderBar()
{
	m_nWidth  = 100;
	m_nHeight = 26;
	m_strText = _T("");

	SetColor( CLR_FORE_GRND, CLR_BACK_GRND );
}

CHeaderBar::~CHeaderBar()
{
	m_brush.DeleteObject();
}

BEGIN_MESSAGE_MAP(CHeaderBar, CControlBar)
	//{{AFX_MSG_MAP(CHeaderBar)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeaderBar message handlers

int CHeaderBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	LOGFONT lf;
	::SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);
    lf.lfHeight = (long)(lf.lfHeight * 1.5);
	//lf.lfWeight = FW_BOLD;

	VERIFY( m_font.CreateFontIndirect(&lf) );

	return 0;
}

void CHeaderBar::OnPaint() 
{
	CPaintDC dc(this);
	
	CRect rcClient;
	GetClientRect(rcClient);

	CBrush brush(CLR_BACK_GRND);

	dc.FillRect( &rcClient, &brush);
	dc.SetBkMode( TRANSPARENT );
	
	dc.SetTextColor( m_clrForeGrnd );
	CFont * pft = dc.SelectObject( &m_font );

	dc.TextOut( 6, 1, m_strText );

	dc.SelectObject( pft );
}

BOOL CHeaderBar::OnEraseBkgnd(CDC* /*pDC*/) 
{
	// TODO: Add your message handler code here and/or call default
//	CRect rect;
//	GetClientRect( &rect );

//	pDC->FillRect( &rect, &m_brush);

	return TRUE; //CControlBar::OnEraseBkgnd(pDC);
}

void CHeaderBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	UNUSED_ALWAYS(pTarget);
	UNUSED_ALWAYS(bDisableIfNoHndler);
	// this function is a pure virtual function, so we must override it.
	// here not used.
}

LRESULT CHeaderBar::OnSizeParent(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS( wParam );

	AFX_SIZEPARENTPARAMS * lpLayout = (AFX_SIZEPARENTPARAMS *)lParam;

	// keep track of the available width for use by CalcFixedLayout later	
	m_nWidth = lpLayout->rect.right - lpLayout->rect.left;

	return CControlBar::OnSizeParent(0, lParam);
}

CSize CHeaderBar::CalcFixedLayout(BOOL /*bStretch*/, BOOL /*bHorz*/)
{
	return CSize(m_nWidth, m_nHeight);
}

BOOL CHeaderBar::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	// 可选用不同的风格
	//cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.dwExStyle |= WS_EX_STATICEDGE;
	//cs.dwExStyle |= WS_EX_WINDOWEDGE;

	return CControlBar::PreCreateWindow(cs);
}

void CHeaderBar::SetColor(COLORREF clrForeGrnd, COLORREF clrBackGrnd)
{
	if( m_clrForeGrnd != clrForeGrnd )
	{
		m_clrForeGrnd = clrForeGrnd;
	}

	if( m_clrBackGrnd != clrBackGrnd )
	{
		m_brush.DeleteObject();
		VERIFY( m_brush.CreateSolidBrush(clrBackGrnd) );

		m_clrBackGrnd = clrBackGrnd;
	}
}

void CHeaderBar::SetText( LPCTSTR lpText )
{
	if( lpText != NULL )
	{
		m_strText = lpText;
	}
	else
	{
		m_strText.Empty();
	}
}