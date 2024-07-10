// BuddiedSliderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "BuddiedSliderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBuddiedSliderCtrl

CBuddiedSliderCtrl::CBuddiedSliderCtrl()
{
	m_pBuddyWnd = NULL;
	m_brWhiteBrush.CreateStockObject( WHITE_BRUSH );
}

CBuddiedSliderCtrl::~CBuddiedSliderCtrl()
{
}


BEGIN_MESSAGE_MAP(CBuddiedSliderCtrl, CCustomDrawSliderCtrl)
	//{{AFX_MSG_MAP(CBuddiedSliderCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBuddiedSliderCtrl message handlers

HWND CBuddiedSliderCtrl::SetBuddy(CWnd *pBuddyWnd)
{
	ASSERT( ::IsWindow(pBuddyWnd->m_hWnd) );
	m_pBuddyWnd = pBuddyWnd;
	pBuddyWnd->SetOwner( this );
	pBuddyWnd->ShowWindow( SW_SHOW );
	HWND hWnd = (HWND) SendMessage( TBM_SETBUDDY, (WPARAM)FALSE, (LPARAM)pBuddyWnd->m_hWnd );
	ReflectedScrollMessage();	// in order to get the initial display of text
	return hWnd;
}

void CBuddiedSliderCtrl::ReflectedScrollMessage(int nStep, int nType, int nReversed)
{
	// update buddy's text, if there is a buddy
	if ( m_pBuddyWnd != NULL )
	{
		int iMin = GetRangeMin();
		int iPos = GetPos();
		int iMax = GetRangeMax();
		CString text;
		if(nType == 0)
		{
			text.Format( _T("%d"), iMax - iPos - nStep);
		}
		else if(nType == 1)
		{
			text.Format( _T("%d"), iPos);
		}
		else
		{
			text.Format( _T("%d"), (iMax-iPos-nStep)*nReversed);
			//text.Format( _T("%d"), iMax - iPos - nStep);
		}
		
		m_pBuddyWnd->SetWindowText( text );
	}
}

void CBuddiedSliderCtrl::SetPos(int nPos, int nStep, int nType, int nReversed, BOOL bSetWndText)
{
	if(nType == 0 || nType == 2)
	{
		nPos =  GetRangeMax() - (nPos + nStep);
	}
	
	CSliderCtrl::SetPos( nPos );

	if(bSetWndText)
	{
		ReflectedScrollMessage(nStep, nType, nReversed);
	}
}



