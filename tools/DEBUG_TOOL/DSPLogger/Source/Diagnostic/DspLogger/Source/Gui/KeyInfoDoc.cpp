// KeyInfoDoc.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "KeyInfoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyInfoDoc

IMPLEMENT_DYNCREATE(CKeyInfoDoc, CDocument)

CKeyInfoDoc::CKeyInfoDoc()
{
}

BOOL CKeyInfoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CKeyInfoDoc::~CKeyInfoDoc()
{
}


BEGIN_MESSAGE_MAP(CKeyInfoDoc, CDocument)
	//{{AFX_MSG_MAP(CKeyInfoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyInfoDoc diagnostics

#ifdef _DEBUG
void CKeyInfoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CKeyInfoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CKeyInfoDoc serialization

void CKeyInfoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CKeyInfoDoc commands

