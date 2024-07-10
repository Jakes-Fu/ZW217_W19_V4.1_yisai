// IHListBox.cpp : implementation file
//

#include "stdafx.h"
#include "nveditor.h"
#include "IHListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIHListBox

CIHListBox::CIHListBox()
{
}

CIHListBox::~CIHListBox()
{
}


BEGIN_MESSAGE_MAP(CIHListBox, CListBox)
	//{{AFX_MSG_MAP(CIHListBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIHListBox message handlers

int CIHListBox::AddString( LPCTSTR lpszItem )
{
	int nResult = CListBox::AddString( lpszItem );
	
	RefushHorizontalScrollBar();
	
	return nResult;
}

int CIHListBox::InsertString( int nIndex, LPCTSTR lpszItem )
{
	int nResult = CListBox::InsertString( nIndex, lpszItem );
	
	RefushHorizontalScrollBar();
	
	return nResult;
}


void CIHListBox::RefushHorizontalScrollBar( void )
{
	CDC *pDC = this->GetDC();
	if ( NULL == pDC )
	{
		return;
	}
	
	int nCount = this->GetCount();
	if ( nCount < 1 )
	{
		this->SetHorizontalExtent( 0 );
		return;
	}
	
	int nMaxExtent = 10;
	CString szText;
	for ( int i = 0; i < nCount; ++i )
	{
		this->GetText( i, szText );
		CSize cs = pDC->GetTextExtent( szText );
		if ( cs.cx > nMaxExtent )
		{
			nMaxExtent += cs.cx;
		}
	}
	
	this->SetHorizontalExtent( nMaxExtent );
}
