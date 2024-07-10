// XListCtrl.cpp  Version 1.4 - article available at www.codeproject.com
//
// Author:  Hans Dietrich
//          hdietrich@gmail.com
//
// History
//     Version 1.4 - 2006 September 1
//     - See article for changes
//
//     Version 1.3 - 2005 February 9
//     - See article for changes
//
//     Version 1.0 - 2002 February 4
//     - Initial public release
//
// License:
//     This software is released into the public domain.  You are free to use
//     it in any way you like, except that you may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this 
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXListCtrl

BEGIN_MESSAGE_MAP(CXListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CXListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ctor
CXListCtrl::CXListCtrl()
{
}

///////////////////////////////////////////////////////////////////////////////
// dtor
CXListCtrl::~CXListCtrl()
{
	m_mapTextColor.clear();
	m_mapBKColor.clear();
}

///////////////////////////////////////////////////////////////////////////////
// PreSubclassWindow
void CXListCtrl::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();

	// for Dialog based applications, this is a good place
	// to subclass the header control because the OnCreate()
	// function does not get called.
}

///////////////////////////////////////////////////////////////////////////////
// OnCreate
int CXListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
	{
		ASSERT(FALSE);
		return -1;
	}

	// When the CXListCtrl object is created via a call to Create(), instead
	// of via a dialog box template, we must subclass the header control
	// window here because it does not exist when the PreSubclassWindow()
	// function is called.
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// OnCustomDraw
void CXListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;

	switch(pLVCD->nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT:
        *pResult = CDRF_NOTIFYITEMDRAW;
        break;
    case CDDS_ITEMPREPAINT:
        {
			int nIdx = pLVCD->nmcd.dwItemSpec;
			pLVCD->clrText = m_mapTextColor[(WORD)nIdx];
			pLVCD->clrTextBk = m_mapBKColor[(WORD)nIdx];

			if(pLVCD->clrTextBk == 0)
			{
				pLVCD->clrTextBk = RGB(255,255,255);
			}
        }
		break;
    case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
        if( pLVCD->iSubItem == 3 )
        {
		    pLVCD->clrTextBk = pLVCD->nmcd.lItemlParam;//RGB(0,200,10);
        }
		*pResult = CDRF_DODEFAULT;
		break;
    default:
	    *pResult = 0;
    }

}


///////////////////////////////////////////////////////////////////////////////
// SetItemColors
void CXListCtrl::SetItemColors(int nItem,								//+++
							   COLORREF crText, 
							   COLORREF crBackground)
{
	m_mapTextColor[(WORD)nItem] = crText;
	m_mapBKColor[(WORD)nItem] = crBackground;
}

BOOL CXListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//return TRUE;
	
	return CListCtrl::OnEraseBkgnd(pDC);
}
