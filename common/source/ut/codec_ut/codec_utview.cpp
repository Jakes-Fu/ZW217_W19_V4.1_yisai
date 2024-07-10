// CODEC_UTView.cpp : implementation of the CCODEC_UTView class
//

#include "stdafx.h"
#include "codec_ut.h"

#include "codec_utdoc.h"
#include "codec_utview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCODEC_UTView

IMPLEMENT_DYNCREATE(CCODEC_UTView, CView)

BEGIN_MESSAGE_MAP(CCODEC_UTView, CView)
	//{{AFX_MSG_MAP(CCODEC_UTView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCODEC_UTView construction/destruction

CCODEC_UTView::CCODEC_UTView()
{
	// TODO: add construction code here

}

CCODEC_UTView::~CCODEC_UTView()
{
}

BOOL CCODEC_UTView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCODEC_UTView drawing

void CCODEC_UTView::OnDraw(CDC* pDC)
{
	CCODEC_UTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CCODEC_UTView printing

BOOL CCODEC_UTView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCODEC_UTView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCODEC_UTView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCODEC_UTView diagnostics

#ifdef _DEBUG
void CCODEC_UTView::AssertValid() const
{
	CView::AssertValid();
}

void CCODEC_UTView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCODEC_UTDoc* CCODEC_UTView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCODEC_UTDoc)));
	return (CCODEC_UTDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCODEC_UTView message handlers