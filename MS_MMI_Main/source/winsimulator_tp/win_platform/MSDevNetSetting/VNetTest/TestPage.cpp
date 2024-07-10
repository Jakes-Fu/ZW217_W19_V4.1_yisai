// TestPage.cpp : implementation file
//

#include "stdafx.h"
#include "VNetTest.h"
#include "TestPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestPage property page

IMPLEMENT_DYNCREATE(CTestPage, CPropertyPage)

CTestPage::CTestPage() : CPropertyPage(CTestPage::IDD)
{
	//{{AFX_DATA_INIT(CTestPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CTestPage::~CTestPage()
{
}

void CTestPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTestPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestPage message handlers
