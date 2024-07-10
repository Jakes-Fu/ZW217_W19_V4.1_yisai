// BmpToArrayView.cpp : implementation of the CBmpToArrayView class
//

#include "stdafx.h"
#include "BmpToArray.h"

#include "BmpToArrayDoc.h"
#include "BmpToArrayView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBmpToArrayView

IMPLEMENT_DYNCREATE(CBmpToArrayView, CFormView)

BEGIN_MESSAGE_MAP(CBmpToArrayView, CFormView)
	//{{AFX_MSG_MAP(CBmpToArrayView)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBmpToArrayView construction/destruction

CBmpToArrayView::CBmpToArrayView()
	: CFormView(CBmpToArrayView::IDD)
{
	//{{AFX_DATA_INIT(CBmpToArrayView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_pDlgMain = NULL;
	m_pDlgOSD  = NULL;
	m_pDlg3D   = NULL;
}

CBmpToArrayView::~CBmpToArrayView()
{
	if (m_pDlgMain != NULL)
    {
        delete m_pDlgMain;
        m_pDlgMain = NULL;
    }
	
	if (m_pDlgOSD != NULL)
    {
        delete m_pDlgOSD;
        m_pDlgOSD = NULL;
    }

	if(m_pDlg3D != NULL)
	{
		delete m_pDlg3D;
		m_pDlg3D = NULL;
	}
}

void CBmpToArrayView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBmpToArrayView)
	DDX_Control(pDX, IDC_TAB_MAIN, m_tabMain);
	//}}AFX_DATA_MAP
}

BOOL CBmpToArrayView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CBmpToArrayView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ModifyStyleEx(this->m_hWnd,WS_EX_CLIENTEDGE,0,TRUE);

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

//	m_tabMain.DeleteAllTabs();

	m_pDlgMain = new CDlgMain;
	m_pDlgMain->Create(CDlgMain::IDD,&m_tabMain);
	m_tabMain.AddTab(m_pDlgMain,_T("General"));
	
	if(g_theApp.m_bShowOSD)
	{
		m_pDlgOSD = new CDlgOSD;
		m_pDlgOSD->Create(CDlgOSD::IDD,&m_tabMain);
		m_tabMain.AddTab(m_pDlgOSD,_T("OSD"));
	}
	
	m_pDlg3D = new CDlg3D;
	m_pDlg3D->Create(CDlg3D::IDD, &m_tabMain);
	m_tabMain.AddTab(m_pDlg3D, _T("3D Table"));

}

/////////////////////////////////////////////////////////////////////////////
// CBmpToArrayView printing

BOOL CBmpToArrayView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBmpToArrayView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBmpToArrayView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CBmpToArrayView::OnPrint(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CBmpToArrayView diagnostics

#ifdef _DEBUG
void CBmpToArrayView::AssertValid() const
{
	CFormView::AssertValid();
}

void CBmpToArrayView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CBmpToArrayDoc* CBmpToArrayView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBmpToArrayDoc)));
	return (CBmpToArrayDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBmpToArrayView message handlers



