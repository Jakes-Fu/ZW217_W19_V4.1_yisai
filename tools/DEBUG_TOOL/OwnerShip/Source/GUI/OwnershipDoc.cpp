// OwnershipDoc.cpp : implementation of the COwnershipDoc class
//

#include "stdafx.h"
#include "Ownership.h"

#include "OwnershipDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COwnershipDoc

IMPLEMENT_DYNCREATE(COwnershipDoc, CDocument)

BEGIN_MESSAGE_MAP(COwnershipDoc, CDocument)
	//{{AFX_MSG_MAP(COwnershipDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COwnershipDoc construction/destruction

COwnershipDoc::COwnershipDoc()
{
	// TODO: add one-time construction code here

}

COwnershipDoc::~COwnershipDoc()
{
}

BOOL COwnershipDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// COwnershipDoc serialization

void COwnershipDoc::Serialize(CArchive& ar)
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
// COwnershipDoc diagnostics

#ifdef _DEBUG
void COwnershipDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COwnershipDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COwnershipDoc commands
