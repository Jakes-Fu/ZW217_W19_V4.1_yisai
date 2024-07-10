// CoolToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "CoolToolBar.h"
#include "..\\MobileCnnt.h"
#include "..\\MobileCnntDoc.h"
#include "..\\MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const COLORREF CCoolToolBar::CLR_TB_BCKGRND = RGB(232, 232, 232);
const int CCoolToolBar::IMG_CX = 32;
const int CCoolToolBar::IMG_CY = 32;
/////////////////////////////////////////////////////////////////////////////
// CCoolToolBar

CCoolToolBar::CCoolToolBar()
{
	m_pImgLst         = NULL;
	m_pDisabledImgLst = NULL;
	m_pHotImgLst      = NULL;

//	VERIFY( m_bmpBckGrnd.LoadBitmap(IDB_TB_BACKGRND) );
    VERIFY( m_brshBckGrnd.CreateSolidBrush(CLR_TB_BCKGRND) );
}

CCoolToolBar::~CCoolToolBar()
{
	if( m_pImgLst != NULL )
	{
		m_pImgLst->DeleteImageList();
		delete m_pImgLst;
		m_pImgLst = NULL;
	}

	if( m_pDisabledImgLst != NULL )
	{
		m_pDisabledImgLst->DeleteImageList();
		delete m_pDisabledImgLst;
		m_pDisabledImgLst = NULL;
	}

	if( m_pHotImgLst != NULL )
	{
		m_pHotImgLst->DeleteImageList();
		delete m_pHotImgLst;
		m_pHotImgLst = NULL;
	}

	m_brshBckGrnd.DeleteObject();
	m_bmpBckGrnd.DeleteObject();

}


BEGIN_MESSAGE_MAP(CCoolToolBar, CToolBar)
	//{{AFX_MSG_MAP(CCoolToolBar)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoolToolBar message handlers

BOOL CCoolToolBar::SetBitmaps(UINT uBmpID, UINT uDisabledBmpID,UINT uHotBmpID)
{
	_ASSERTE( m_pImgLst == NULL && m_pDisabledImgLst == NULL );

	try{
		m_pImgLst = new CImageList;
		VERIFY( m_pImgLst->Create( IMG_CX, IMG_CY, ILC_COLOR24 | ILC_MASK, 0, 1 ) );
		
		CBitmap bmp;
		VERIFY( bmp.LoadBitmap(uBmpID) );
		
		VERIFY( m_pImgLst->Add(&bmp, RGB(232,232,232) ) != -1 );
		
		m_pDisabledImgLst = new CImageList;
		VERIFY( m_pDisabledImgLst->Create( IMG_CX, IMG_CY, ILC_COLOR24 | ILC_MASK, 0, 1 ) );
		
		bmp.DeleteObject();
		VERIFY( bmp.LoadBitmap(uDisabledBmpID) );
		
		VERIFY( m_pDisabledImgLst->Add(&bmp, RGB(232,232,232) ) != -1 );

		m_pHotImgLst = new CImageList;
		VERIFY( m_pHotImgLst->Create( IMG_CX, IMG_CY, ILC_COLOR24 | ILC_MASK, 0, 1 ) );
		bmp.DeleteObject();
		VERIFY( bmp.LoadBitmap(uHotBmpID) );		
		VERIFY( m_pHotImgLst->Add(&bmp, RGB(232,232,232) ) != -1 );
		
		CToolBarCtrl & tbCtrl = GetToolBarCtrl();
		
		tbCtrl.SetImageList( m_pImgLst );
		tbCtrl.SetDisabledImageList(m_pDisabledImgLst);
		tbCtrl.SetHotImageList(m_pHotImgLst);
		//tbCtrl.SetStyle(TBSTYLE_FLAT);
	}
	catch(...)
	{
		TRACE0("!! CCoolToolBar::SetBitmaps occur fatal error !!\r\n");

		return  FALSE;
	}

	return TRUE;
}


BOOL CCoolToolBar::OnEraseBkgnd(CDC* pDC)
{	
//#ifndef SPRD8800
	CRect rect;
	GetClientRect(rect);

	CBrush brush(CLR_TB_BCKGRND);
	pDC->FillRect(&rect,&brush);
//#endif
	
	return TRUE;
}

void CCoolToolBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
#ifdef SPRD8800
	CRect rect_dial;
	CRect rect_call;
	GetItemRect(1, &rect_dial);
	GetItemRect(3, &rect_call);

	CMobileCnntDoc *pDoc = (CMobileCnntDoc *)(((CMainFrame *)AfxGetMainWnd())->GetActiveDocument());
	if(pDoc == NULL)
	{
		return;
	}
	if(!pDoc->m_bEnableDialUP)
	{
		if(rect_dial.PtInRect(point))
		{
			AfxMessageBox(IDS_DISENABLE_DIAL);
		}
	}

	if(!pDoc->m_bEnableCall)
	{
		if(rect_call.PtInRect(point))
		{
			AfxMessageBox(IDS_DISENABLE_CALL);
		}
	}
#endif	
	
	
	CToolBar::OnLButtonDown(nFlags, point);
}
