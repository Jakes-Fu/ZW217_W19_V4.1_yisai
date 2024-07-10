// NVEditorDoc.cpp : implementation of the CNVEditorDoc class
//

#include "stdafx.h"
#include "NVEditor.h"

#include "NVEditorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNVEditorDoc

IMPLEMENT_DYNCREATE(CNVEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CNVEditorDoc, CDocument)
	//{{AFX_MSG_MAP(CNVEditorDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNVEditorDoc construction/destruction

CNVEditorDoc::CNVEditorDoc()
{
	// TODO: add one-time construction code here

}

CNVEditorDoc::~CNVEditorDoc()
{
}

BOOL CNVEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CNVEditorDoc serialization

void CNVEditorDoc::Serialize(CArchive& ar)
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
// CNVEditorDoc diagnostics

#ifdef _DEBUG
void CNVEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNVEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNVEditorDoc commands
