// ChannelServerView.cpp : implementation of the CChannelServerView class
//

#include "stdafx.h"
#include "ChannelServer.h"
#include "ChannelServerDoc.h"
#include "ChannelServerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChannelServerView

IMPLEMENT_DYNCREATE(CChannelServerView, CView)

BEGIN_MESSAGE_MAP(CChannelServerView, CView)
	//{{AFX_MSG_MAP(CChannelServerView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChannelServerView construction/destruction

CChannelServerView::CChannelServerView()
{
	// TODO: add construction code here

}

CChannelServerView::~CChannelServerView()
{
}

BOOL CChannelServerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CChannelServerView drawing

void CChannelServerView::OnDraw(CDC* pDC)
{
	UNREFERENCED_PARAMETER( pDC );
	CChannelServerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CChannelServerView diagnostics

#ifdef _DEBUG
void CChannelServerView::AssertValid() const
{
	CView::AssertValid();
}

void CChannelServerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CChannelServerDoc* CChannelServerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CChannelServerDoc)));
	return (CChannelServerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChannelServerView message handlers
