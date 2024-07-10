// FilterDoc.cpp : implementation file
//

#include "stdafx.h"
#include "../Gui/DspLogger.h"
#include "FilterDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterDoc

IMPLEMENT_DYNCREATE(CFilterDoc, CDocument)

CFilterDoc::CFilterDoc()
{
    m_pChartView = NULL;
    m_pChartCtrl = NULL;


}

BOOL CFilterDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CFilterDoc::~CFilterDoc()
{
}//lint !e1540


BEGIN_MESSAGE_MAP(CFilterDoc, CDocument)
	//{{AFX_MSG_MAP(CFilterDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterDoc diagnostics

#ifdef _DEBUG
void CFilterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFilterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFilterDoc serialization

void CFilterDoc::Serialize(CArchive& ar)
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
// CFilterDoc commands

