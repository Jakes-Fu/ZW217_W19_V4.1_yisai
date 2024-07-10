// TestSheet.cpp : implementation file
//

#include "stdafx.h"
#include "VNetTest.h"
#include "TestSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestSheet

IMPLEMENT_DYNAMIC(CTestSheet, CPropertySheet)

CTestSheet::CTestSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
    AddPage(&m_TestPage);
}

CTestSheet::CTestSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
    AddPage(&m_TestPage);
}

CTestSheet::~CTestSheet()
{
}


BEGIN_MESSAGE_MAP(CTestSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CTestSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestSheet message handlers
