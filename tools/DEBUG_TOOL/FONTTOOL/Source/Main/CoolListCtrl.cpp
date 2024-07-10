// CoolListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "fonttool.h"
#include "CoolListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCoolListCtrl

CCoolListCtrl::CCoolListCtrl()
{
}

CCoolListCtrl::~CCoolListCtrl()
{
}


BEGIN_MESSAGE_MAP(CCoolListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CCoolListCtrl)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_NOTIFY(HDN_BEGINTRACK, 0, OnBegintrack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoolListCtrl message handlers

void CCoolListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	SetFocus();
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CCoolListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	SetFocus();
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CCoolListCtrl::OnBegintrack(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED_ALWAYS(pNMHDR);
//	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	SetFocus();
	*pResult = 0;
}
