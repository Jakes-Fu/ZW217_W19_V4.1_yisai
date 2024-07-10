// SMSView1.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "SMSView.h"
#include "InfoListView.h"
#include "SMSDoc.h"
#include "Sprite\MsgIDDef.h"
#include "GlobalDef.h"
#include "effect\ChildFrameWnd.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMSView

IMPLEMENT_DYNCREATE(CSMSView, CView)

CSMSView::CSMSView()
{
	m_dwCategID       = 0x00;
	m_dwOldCategID    = 0x00;
	m_pFrameWnd       = NULL;
}

CSMSView::~CSMSView()
{
	m_pFrameWnd       = NULL;
}


BEGIN_MESSAGE_MAP(CSMSView, CView)
	//{{AFX_MSG_MAP(CSMSView)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMSView drawing

void CSMSView::OnDraw(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	//CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CSMSView diagnostics

#ifdef _DEBUG
void CSMSView::AssertValid() const
{
	CView::AssertValid();
}

void CSMSView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CMobileSpriteView printing
BOOL CSMSView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

BOOL CSMSView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSMSView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSMSView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CSMSView message handlers

void CSMSView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

	if(theApp.m_pSMSFrame != NULL)
	{
		theApp.m_pSMSFrame->m_pSMSView = this;
	}

	// TODO: Add your specialized code here and/or call the base class
	//
	if( m_pFrameWnd != NULL )
		return;

	CRuntimeClass * prtc = RUNTIME_CLASS( CChildFrameWnd );
	_ASSERTE( prtc != NULL );

	m_pFrameWnd = (CChildFrameWnd *)prtc->CreateObject();
	if( NULL == m_pFrameWnd )
	{
		AfxMessageBox(IDS_RESOURCE_NOT_ENOUGHT);
		return;
	}

	CCreateContext context;
	ZeroMemory( &context, sizeof(context) );
	CDocument * pDoc = GetDocument();
	_ASSERTE( pDoc != NULL );

	context.m_pCurrentDoc     = pDoc;
	context.m_pNewViewClass   = RUNTIME_CLASS( CInfoListView );
	context.m_pNewDocTemplate = pDoc->GetDocTemplate();
	context.m_pCurrentFrame   = (CFrameWnd *)AfxGetMainWnd();
	CRect rect(0, 0, 0, 0);
	if( !m_pFrameWnd->Create( NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 
		                      rect, this, NULL, 0, &context) )
	{
		AfxMessageBox(IDS_RESOURCE_NOT_ENOUGHT);
		return;
	}
}

void CSMSView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if( m_pFrameWnd != NULL )
	{
		m_pFrameWnd->MoveWindow( -1, -1, cx + 4, cy + 4, FALSE );
	}
	
}

void CSMSView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( m_pFrameWnd != NULL )
 		m_pFrameWnd->UpdateWindow();

	if( pSender != this && lHint == WM_CATEGORY_CHANGE )
	{
		if( (DWORD)pHint == m_dwCategID )
   			return;

		_ASSERTE( m_pFrameWnd != NULL );

		m_dwCategID = (DWORD)pHint;

		switch( HIWORD(m_dwCategID) )
		{
		case ID_CATEG_SMS:
		case ID_CATEG_MEMO:	
			m_pFrameWnd->ShowContentBar(TRUE);
			if(theApp.m_pSMSFrame != NULL)
			{
				theApp.m_pSMSFrame->m_bShowContentBar = TRUE;
			}
			break;
		case ID_CATEG_PB:
		case ID_CATEG_PBG:
		case ID_CATEG_PBGM:
			m_pFrameWnd->ShowContentBar(FALSE);
			if(theApp.m_pSMSFrame != NULL)
			{
				theApp.m_pSMSFrame->m_bShowContentBar = FALSE;
			}
			break;
		default:
			TRACE(_T("CSMSView::m_dwCategID is [%.8X]\r\n"),m_dwCategID);
			_ASSERTE( 0 );
			break;
		}	
	}
	
}
