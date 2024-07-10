// CoolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CoolDlg.h"
#include "..\resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_BTN_CLOSE 0x1001

const COLORREF CCoolDlg::CLR_DLGBCKGRND = RGB(232, 232, 232);
const COLORREF CCoolDlg::CLR_HOVER      = RGB(30,  60,  160);
const COLORREF CCoolDlg::CLR_CLICKED    = RGB(218, 206, 236);
const COLORREF CCoolDlg::CLR_DISABLED   = RGB(239, 235, 247);
/////////////////////////////////////////////////////////////////////////////
// CCoolDlg dialog

CCoolDlg::CCoolDlg(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	//{{AFX_DATA_INIT(CCoolDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CCoolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCoolDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCoolDlg, CDialog)
	//{{AFX_MSG_MAP(CCoolDlg)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_NCHITTEST()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoolDlg message handlers

BOOL CCoolDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CenterWindow();

	CRect rcDlg;
	GetClientRect(rcDlg);

	CBitmap bmpBtn;
	VERIFY( bmpBtn.LoadBitmap(IDB_DLG_CLSBTN_NORMAL) );
	BITMAP bmpInfo;
	VERIFY( bmpBtn.GetBitmap( &bmpInfo ) );

	CRect rcBtnClose( rcDlg.Width() - bmpInfo.bmWidth - 1, rcDlg.top,
		              0, 0 );
	rcBtnClose.right  = rcBtnClose.left + bmpInfo.bmWidth;
	rcBtnClose.bottom = rcBtnClose.top + bmpInfo.bmHeight;

	VERIFY( m_btnClose.Create( NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
							   rcBtnClose, this, IDC_BTN_CLOSE ) );

	VERIFY( m_btnClose.LoadBitmaps( IDB_DLG_CLSBTN_NORMAL, IDB_DLG_CLSBTN_HOVER,
		                            IDB_DLG_CLSBTN_CLICK ) );
	
	VERIFY( m_bmpDlgTitle.LoadBitmap( IDB_DLG_TITLE ) );
	VERIFY( m_bmpDlgBckGrnd.LoadBitmap(IDB_DLG_BACKGRND) );
    VERIFY( m_brshDlgBckGrnd.CreateSolidBrush(CLR_DLGBCKGRND) );

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCoolDlg::OnBtnClose() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

BOOL CCoolDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);

	CDC memDC;
	VERIFY( memDC.CreateCompatibleDC(pDC) );

	CBitmap * pOldBmp = memDC.SelectObject(&m_bmpDlgTitle);
	BITMAP bmpInfo;
	VERIFY( m_bmpDlgTitle.GetBitmap( &bmpInfo ) );
	const int nTitleHeight = bmpInfo.bmHeight;

	// 本图片我们提供了一个较长的，使用时只取所需的长度部分
	pDC->BitBlt( 0, 0, rect.Width(), bmpInfo.bmHeight, 
		         &memDC, 0, 0, SRCCOPY );	
	
	if( !m_strDlgCaption.IsEmpty() )
	{
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkMode( TRANSPARENT );

		pDC->TextOut( 24, 1, m_strDlgCaption );
	}
	//pDC->StretchBlt( 0, 0, rect.Width(), bmpInfo.bmHeight, 
	//	             &memDC, 
	//				 0, 0, bmpInfo.bmWidth,  bmpInfo.bmHeight, 
	//				 SRCCOPY );
	
	memDC.SelectObject( pOldBmp );

	pOldBmp = memDC.SelectObject(&m_bmpDlgBckGrnd);
   
	rect.top += nTitleHeight;

	VERIFY( m_bmpDlgBckGrnd.GetBitmap(&bmpInfo) );
    pDC->StretchBlt( 0, nTitleHeight, rect.Width(), rect.Height(), 
		             &memDC, 
					 0, 0, bmpInfo.bmWidth,  bmpInfo.bmHeight, 
					 SRCCOPY );
	
	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();

	return TRUE;//CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CCoolDlg::OnCtlColor(CDC* pDC, CWnd* /*pWnd*/, UINT /*nCtlColor*/) 
{
	//HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	//pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor( CLR_DLGBCKGRND );

	// TODO: Return a different brush if the default is not desired
	return (HBRUSH)(m_brshDlgBckGrnd.m_hObject);
}

UINT CCoolDlg::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT nHitTest = CDialog::OnNcHitTest (point );

	BITMAP bmpInfo;
	VERIFY( m_bmpDlgTitle.GetBitmap(&bmpInfo) );

	CRect rcTitle;
	GetClientRect(rcTitle);
	ClientToScreen(rcTitle);

	rcTitle.bottom = rcTitle.top + bmpInfo.bmHeight;

	if( rcTitle.PtInRect(point) )
		return (nHitTest == HTCLIENT) ? HTCAPTION : nHitTest;
	else
		return nHitTest;
}

void CCoolDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_brshDlgBckGrnd.DeleteObject();
	m_bmpDlgBckGrnd.DeleteObject();
	m_bmpDlgTitle.DeleteObject();
}
