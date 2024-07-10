// CoolMenu.cpp : implementation file
//
#include "stdafx.h"
#include "CoolMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int   CCoolMenu::SEPARATOR_HEIGHT = 8;
const DWORD CCoolMenu::CLR_BACKGRND     = RGB(232, 232, 232);
const DWORD CCoolMenu::CLR_GRAY         = RGB(128, 128, 128);
const DWORD CCoolMenu::CLR_HOT_FORE     = RGB(49, 105, 198);
const DWORD CCoolMenu::CLR_HOT_BACK     = RGB(198, 211, 239);

/////////////////////////////////////////////////////////////////////////////
// CCoolMenu

CCoolMenu::CCoolMenu()
{
	m_bInitMenu = FALSE;
	m_pToolBar  = NULL;

	m_sizeKey   = CSize(60, 16);
	m_sizeImg   = CSize(16, 16);
}

CCoolMenu::~CCoolMenu()
{
	ReleaseItemInfo();
}

void CCoolMenu::InitMenu( CMenu *pMenu, CToolBar *pToolBar /* = NULL */)
{
	_ASSERTE( pMenu != NULL );

	if( m_bInitMenu )
		return;

	InitToolBar(pToolBar);
	
	CMenu * pSub = NULL;
	int nCount = pMenu->GetMenuItemCount();
	for( int i = 0; i < nCount; ++i )
	{
		pSub = pMenu->GetSubMenu(i);
		if( pSub != NULL )
			RecursiveInit( pSub );
	}

	m_bInitMenu = TRUE;
}

void CCoolMenu::RecursiveInit(CMenu *pMenu)
{
	_ASSERTE( pMenu != NULL );
	
	PITEM_INFO pItemInfo = NULL;
	CMenu *    pSub      = NULL;
	UINT       uStyle    = 0;
	int nCount = pMenu->GetMenuItemCount();
	for( int i = 0; i < nCount; ++i )
	{
		pSub = pMenu->GetSubMenu(i);
		if( pSub != NULL )
			RecursiveInit(pSub);

		pItemInfo = new ITEM_INFO;
		if( NULL == pItemInfo )
			return;

		m_arrItemInfo.AddTail(pItemInfo);

		pItemInfo->uOriginalState = pMenu->GetMenuState(i, MF_BYPOSITION);
		UINT uID = pMenu->GetMenuItemID(i);
		if( uID > 0 )
		{
			MENUITEMINFO mii = { 0 };
			mii.cbSize = sizeof(mii);
			mii.fMask  = MIIM_CHECKMARKS;
			
			VERIFY( pMenu->GetMenuItemInfo(i, &mii, TRUE) );
			
			pItemInfo->hChecked   = mii.hbmpChecked;
			pItemInfo->hUnchecked = mii.hbmpUnchecked;
			pMenu->GetMenuString(i, pItemInfo->szText, MAX_ITEM_TEXT_LEN, MF_BYPOSITION);

			//pItemInfo->nImgIdx = GetImgIdxByID(uID);
			pItemInfo->nImgIdx = -1;  //@Hongliang.Xin
		}
		pItemInfo->uID = uID;
		
		uStyle = MF_BYPOSITION | MF_OWNERDRAW;
		if( NULL == pSub )
			uStyle |=  pItemInfo->uOriginalState;

		pMenu->ModifyMenu(i, uStyle, pItemInfo->uID,LPCTSTR(pItemInfo));
	}
}

BOOL CCoolMenu::LoadPopupMenu(UINT uID, CToolBar *pToolBar /* = NULL */)
{
	if( m_hMenu != NULL )
		DestroyMenu();

	ReleaseItemInfo();

	InitToolBar(pToolBar);

	BOOL bLoad = LoadMenu(uID);
	if( bLoad )
	{
		CMenu * pSub = NULL;
		int nCount = GetMenuItemCount();
		for( int i = 0; i < nCount; ++i )
		{
			pSub = GetSubMenu(i);
			if( pSub != NULL )
				RecursiveInit( pSub );
		}
	}

	return bLoad;
}

void CCoolMenu::ResetPopupMenu(CMenu * pMenu)
{
	PITEM_INFO pii = NULL;
	POSITION pos = m_arrItemInfo.GetHeadPosition();
	for( ; pos != NULL; )
	{
		pii = m_arrItemInfo.GetNext(pos);
		pMenu->EnableMenuItem( pii->uID, pii->uOriginalState | MF_BYCOMMAND);
	}
}

void CCoolMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	PITEM_INFO pItemInfo = (PITEM_INFO)lpMIS->itemData;

	CWnd * pMainWnd = AfxGetMainWnd();
	_ASSERTE( pMainWnd != NULL );

	CDC	*pDC = pMainWnd->GetDC();

	CSize sizeText = pDC->GetTextExtent(pItemInfo->szText);

	// ¼ÆËã¿ì½Ý¼üµÄ¿í¶È
	static _TCHAR szKey[] = _T("Ctrl+C");
	m_sizeKey  = pDC->GetTextExtent( szKey );

	pMainWnd->ReleaseDC(pDC);

	lpMIS->itemWidth = m_sizeImg.cx + sizeText.cx;
	
	if( pItemInfo->uID > 0 )
		lpMIS->itemHeight = m_sizeImg.cy + 4;
	else
		lpMIS->itemHeight = SEPARATOR_HEIGHT;
}

void CCoolMenu::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	//TRACE0("begin Draw menu in CCoolMenu \r\n");
	//TRACE2("Draw [ID:%d] state %d \r\n", lpDIS->itemID,lpDIS->itemState);

	CDC * pDC  = CDC::FromHandle(lpDIS->hDC);
	CRect rect = lpDIS->rcItem;

	pDC->SetBkMode(TRANSPARENT);

	DrawRect(pDC, rect, ODS_DEFAULT );

#ifdef _UNICODE
	DrawRect(pDC, rect, lpDIS->itemState);	
	DrawRect(     pDC, rect, lpDIS->itemState, lpDIS->itemID == ID_SEPARATOR);
    DrawBmp(      pDC, rect, lpDIS->itemData, lpDIS->itemState & ODS_CHECKED );
	DrawText(     pDC, rect, lpDIS->itemData, lpDIS->itemState & ODS_GRAYED);
	DrawSeparator(pDC, rect, lpDIS->itemData );
#endif

}

void CCoolMenu::DrawRect( CDC * pDC, LPCRECT pRect, UINT uState /* = ODS_DEFAULT*/,
						  BOOL bSeparator /* = FALSE */)
{
	CRect rect(pRect);

	if( uState & ODS_DEFAULT)
	{
		DrawBack(pDC, rect, CLR_BACKGRND);
	}
	else if( !bSeparator && (uState & ODS_SELECTED) && !(uState & ODS_GRAYED) )
	{
		DrawBack(pDC, rect, CLR_HOT_BACK);
		pDC->Draw3dRect(rect, CLR_HOT_FORE, CLR_HOT_FORE);
	}
}

void CCoolMenu::DrawBmp(CDC *pDC, LPCRECT pRect, DWORD dwItemData, BOOL bChecked/* = FALSE */)
{
	_ASSERTE( pDC != NULL );

	PITEM_INFO pItemInfo = (PITEM_INFO)dwItemData;
    CRect rect(pRect); 

	if( bChecked && pItemInfo->uID > 0 )
	{
		CRect rc( rect );
		rc.left += 1;
		rc.top  += 1;
		rc.right  = rc.left + m_sizeImg.cx + 2;
		rc.bottom = rc.top + m_sizeImg.cy + 2;
		//pDC->DrawEdge(rc, EDGE_SUNKEN, BF_RECT);
		pDC->Draw3dRect(rc, RGB(126,126,126), RGB(126,126,126) );
		
		rc.top += 3;
		static const _TCHAR szChecked[] = _T("¡Ì");
		pDC->DrawText(szChecked, rc, DT_CENTER);
	}

	CPoint pt( rect.left + 2, rect.top + 2 );
	if( m_pToolBar != NULL && pItemInfo->uID > 0 && pItemInfo->nImgIdx >= 0 )
	{
		m_imgLst.Draw(pDC, pItemInfo->nImgIdx, pt, ILD_TRANSPARENT);
	}
}

void CCoolMenu::DrawText( CDC * pDC, LPCRECT pRect, DWORD dwItemData, BOOL bGray /* = FALSE */)
{
	_ASSERTE( pDC != NULL );

	PITEM_INFO pItemInfo = (PITEM_INFO)dwItemData;
    CRect rect(pRect);

	static _TCHAR cSkip       = _T('\t');
	static _TCHAR szKey[ 16 ] = { 0 };

	LPTSTR pFind = _tcschr(pItemInfo->szText, cSkip);
	if( pFind != NULL )
	{
		*pFind = _T('\0');
		_tcscpy(szKey, pFind + 1);
	}

	if( bGray )
		pDC->SetTextColor( CLR_GRAY );

	rect.left += (m_sizeImg.cx + 8);
	rect.top  += (m_sizeImg.cy - m_sizeKey.cy) / 2 + 3;

	pDC->DrawText( pItemInfo->szText, -1, rect, DT_LEFT | DT_VCENTER);

	if( pFind != NULL )
	{
		rect.left = rect.right - m_sizeKey.cx - 8;
		pDC->DrawText( szKey, -1, rect, DT_CENTER | DT_VCENTER);

		*pFind = cSkip;
	}
}

void CCoolMenu::DrawSeparator(CDC *pDC, LPCRECT pRect, DWORD dwItemData)
{
	_ASSERTE( pDC != NULL );

	PITEM_INFO pItemInfo = (PITEM_INFO)dwItemData;
    CRect rect(pRect);

	if( pItemInfo->uID == 0 )
	{
		rect.left   += m_sizeImg.cx + 4;
		rect.top    += (SEPARATOR_HEIGHT / 2 - 1);
		rect.bottom = rect.top + 2;

		pDC->Draw3dRect(rect, RGB(64,0,128), RGB(255,255,255));
	}
}

__inline int CCoolMenu::GetImgIdxByID(UINT uCmdID)
{
	if( NULL == m_pToolBar )
		return -1;

	int nImg = -1;
	int nIdx = m_pToolBar->CommandToIndex(uCmdID);
	if( nIdx >= 0 )
	{
		UINT uID    = 0;
		UINT uStyle = 0;
		m_pToolBar->GetButtonInfo( nIdx, uID, uStyle, nImg);
		_ASSERTE( uID == uCmdID );
	}

	return nImg;
}

__inline void CCoolMenu::DrawBack(CDC *pDC, LPCRECT pRect, COLORREF clrBack)
{
	CRect rect(pRect);
/*  »­×ó±ß±³¾°£¬ÔÝÎ´ÓÃ
	rect.right = rect.left + m_sizeImg.cx + 4;
	rect.bottom = rect.top + m_sizeImg.cy + 4;
	pDC->FillSolidRect( rect, CLR_HOT_FORE );

	rect = pRect;
	rect.left += m_sizeImg.cx + 4;
*/
	pDC->FillSolidRect(rect, clrBack);
}

void CCoolMenu::InitToolBar(CToolBar *pToolBar)
{
	m_pToolBar = pToolBar;
	if( pToolBar != NULL )
	{
		CToolBarCtrl & tbc = pToolBar->GetToolBarCtrl();
		CImageList * pil = tbc.GetImageList();
		_ASSERTE( pil != NULL );

		if( !m_imgLst.Create(pil) )
			m_pToolBar = NULL;
		
		//TRACE1("image list count is :%d\r\n", m_imgLst.GetImageCount());
		IMAGEINFO imgInfo;
		VERIFY( m_imgLst.GetImageInfo(0, &imgInfo) );

		CRect rc = imgInfo.rcImage;
		m_sizeImg.cx = rc.Width();
		m_sizeImg.cy = rc.Height();
	}
}

void CCoolMenu::ReleaseItemInfo()
{
	m_imgLst.DeleteImageList();

	PITEM_INFO pii = NULL;
	for( ; !m_arrItemInfo.IsEmpty(); )
	{
		pii = m_arrItemInfo.RemoveTail();
		delete pii;
	}
}
