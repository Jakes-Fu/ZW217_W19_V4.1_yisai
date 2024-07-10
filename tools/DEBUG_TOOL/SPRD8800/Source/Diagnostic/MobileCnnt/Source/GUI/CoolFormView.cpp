// CoolFormView.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "CoolFormView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const COLORREF CCoolFormView::CLR_DLGBCKGRND = RGB(240, 240, 240);
/////////////////////////////////////////////////////////////////////////////
// CCoolFormView

CCoolFormView::CCoolFormView( UINT nIDTemplate )
	: CFormView(nIDTemplate)
{
	//{{AFX_DATA_INIT(CCoolFormView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	VERIFY( m_bmpDlgBckGrnd.LoadBitmap(IDB_DLG_BACKGRND) );
    VERIFY( m_brshDlgBckGrnd.CreateSolidBrush(CLR_DLGBCKGRND) );

}

CCoolFormView::~CCoolFormView()
{
	m_brshDlgBckGrnd.DeleteObject();
//	m_bmpDlgBckGrnd.DeleteObject();
}

void CCoolFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCoolFormView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCoolFormView, CFormView)
	//{{AFX_MSG_MAP(CCoolFormView)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoolFormView diagnostics

#ifdef _DEBUG
void CCoolFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCoolFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCoolFormView message handlers

HBRUSH CCoolFormView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nCtlColor);

	pDC->SetBkColor( CLR_DLGBCKGRND );

	// TODO: Return a different brush if the default is not desired
	return (HBRUSH)(m_brshDlgBckGrnd.m_hObject);
}

BOOL CCoolFormView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);

	pDC->FillRect(&rect,&m_brshDlgBckGrnd);
	
// 	CDC memDC;
// 	VERIFY( memDC.CreateCompatibleDC(pDC) );
// 	
// 	CBitmap * pOldBmp = memDC.SelectObject(&m_bmpDlgBckGrnd);
// 	BITMAP bmpInfo;
// 	
// 	VERIFY( m_bmpDlgBckGrnd.GetBitmap(&bmpInfo) );
//     pDC->StretchBlt( 0, 0, rect.Width(), rect.Height(), 
// 		&memDC, 
// 		0, 0, bmpInfo.bmWidth,  bmpInfo.bmHeight, 
// 		SRCCOPY );
// 	
// 	memDC.SelectObject(pOldBmp);
// 	memDC.DeleteDC();
	
	return TRUE;//CDialog::OnEraseBkgnd(pDC);
}

void CCoolFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
}
