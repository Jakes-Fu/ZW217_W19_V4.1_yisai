// MobileCnntView.cpp : implementation of the CMobileCnntView class
//

#include "stdafx.h"
#include "MobileCnnt.h"

#include "MobileCnntDoc.h"
#include "MobileCnntView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMobileCnntView

IMPLEMENT_DYNCREATE(CMobileCnntView, CView)

BEGIN_MESSAGE_MAP(CMobileCnntView, CView)
	//{{AFX_MSG_MAP(CMobileCnntView)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMobileCnntView construction/destruction

CMobileCnntView::CMobileCnntView()
{
	// TODO: add construction code here

}

CMobileCnntView::~CMobileCnntView()
{
}

BOOL CMobileCnntView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMobileCnntView drawing

void CMobileCnntView::OnDraw(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	CMobileCnntDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMobileCnntView printing

BOOL CMobileCnntView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMobileCnntView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMobileCnntView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMobileCnntView diagnostics

#ifdef _DEBUG
void CMobileCnntView::AssertValid() const
{
	CView::AssertValid();
}

void CMobileCnntView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMobileCnntDoc* CMobileCnntView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMobileCnntDoc)));
	return (CMobileCnntDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMobileCnntView message handlers

BOOL CMobileCnntView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	UNREFERENCED_PARAMETER(pDC);
	
	return TRUE;
	
//	return CView::OnEraseBkgnd(pDC);
}
