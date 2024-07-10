// PropertySheetEx.cpp : implementation file
//

#include "stdafx.h"
#include "ChannelServer.h"
#include "PropertySheetEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertySheetEx

IMPLEMENT_DYNAMIC(CPropertySheetEx, CPropertySheet)

CPropertySheetEx::CPropertySheetEx(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropertySheetEx::CPropertySheetEx(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CPropertySheetEx::~CPropertySheetEx()
{
}


BEGIN_MESSAGE_MAP(CPropertySheetEx, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetEx)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertySheetEx message handlers
