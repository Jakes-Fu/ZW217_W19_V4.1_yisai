// BmpToArrayDoc.cpp : implementation of the CBmpToArrayDoc class
//

#include "stdafx.h"
#include "BmpToArray.h"

#include "BmpToArrayDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBmpToArrayDoc

IMPLEMENT_DYNCREATE(CBmpToArrayDoc, CDocument)

BEGIN_MESSAGE_MAP(CBmpToArrayDoc, CDocument)
	//{{AFX_MSG_MAP(CBmpToArrayDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBmpToArrayDoc construction/destruction

CBmpToArrayDoc::CBmpToArrayDoc()
{
	// TODO: add one-time construction code here

}

CBmpToArrayDoc::~CBmpToArrayDoc()
{
}

BOOL CBmpToArrayDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CBmpToArrayDoc serialization

void CBmpToArrayDoc::Serialize(CArchive& ar)
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
// CBmpToArrayDoc diagnostics

#ifdef _DEBUG
void CBmpToArrayDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBmpToArrayDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBmpToArrayDoc commands
