// ChannelServerDoc.cpp : implementation of the CChannelServerDoc class
//

#include "stdafx.h"
#include "ChannelServer.h"
#include "ChannelServerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChannelServerDoc

IMPLEMENT_DYNCREATE(CChannelServerDoc, CDocument)

BEGIN_MESSAGE_MAP(CChannelServerDoc, CDocument)
	//{{AFX_MSG_MAP(CChannelServerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChannelServerDoc construction/destruction

CChannelServerDoc::CChannelServerDoc()
{
	// TODO: add one-time construction code here

}

CChannelServerDoc::~CChannelServerDoc()
{
}

BOOL CChannelServerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChannelServerDoc serialization

void CChannelServerDoc::Serialize(CArchive& ar)
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
// CChannelServerDoc diagnostics

#ifdef _DEBUG
void CChannelServerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CChannelServerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChannelServerDoc commands
