// CODEC_UTDoc.cpp : implementation of the CCODEC_UTDoc class
//

#include "stdafx.h"
#include "codec_ut.h"

#include "codec_utdoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCODEC_UTDoc

IMPLEMENT_DYNCREATE(CCODEC_UTDoc, CDocument)

BEGIN_MESSAGE_MAP(CCODEC_UTDoc, CDocument)
	//{{AFX_MSG_MAP(CCODEC_UTDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCODEC_UTDoc construction/destruction

CCODEC_UTDoc::CCODEC_UTDoc()
{
	// TODO: add one-time construction code here

}

CCODEC_UTDoc::~CCODEC_UTDoc()
{
}

BOOL CCODEC_UTDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCODEC_UTDoc serialization

void CCODEC_UTDoc::Serialize(CArchive& ar)
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
// CCODEC_UTDoc diagnostics

#ifdef _DEBUG
void CCODEC_UTDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCODEC_UTDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCODEC_UTDoc commands