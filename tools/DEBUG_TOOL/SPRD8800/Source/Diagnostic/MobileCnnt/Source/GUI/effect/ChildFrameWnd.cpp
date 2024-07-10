// ChildFrameWnd.cpp : implementation file
//

#include "stdafx.h"
#include "childframewnd.h"

#include "..\MobileCnnt.h"
#include "..\Sprite\ContentView.h"
#include "..\MSInfoView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrameWnd

IMPLEMENT_DYNCREATE(CChildFrameWnd, CFrameWnd)

CChildFrameWnd::CChildFrameWnd()
{
}

CChildFrameWnd::~CChildFrameWnd()
{
}


BEGIN_MESSAGE_MAP(CChildFrameWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CChildFrameWnd)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrameWnd message handlers

int CChildFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_HeaderBar.Create( NULL, NULL, WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | CBRS_TOP,
		                CRect(0,0,0,0), this, AFX_IDW_STATUS_BAR );

	m_HeaderBar.SetBarStyle(CBRS_ALIGN_TOP);

	CString strName;
	VERIFY( strName.LoadString(IDS_CONTENT_NAME) );
	if (!m_ContentBar.Create(strName, this, CSize(180, 80), 0, TRUE))
	{
		TRACE0("Failed to create category bar\n");
	    return -1;
	}
	HINSTANCE hInstance = AfxGetApp()->m_hInstance;
	HCURSOR hHorz = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CUR_HORZ));
	HCURSOR hVert = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CUR_VERT));
	m_ContentBar.SetStretchCursor( hHorz, hVert );

	CCreateContext * pContext = (CCreateContext *)(lpCreateStruct->lpCreateParams);
	if( !m_ContentBar.AddPage(RUNTIME_CLASS(CContentView), pContext) )
	{
		TRACE0("Failed to create category view\n");
	    return -1;
	}

	m_ContentBar.SetBarStyle( CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );

	m_ContentBar.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_ContentBar, AFX_IDW_DOCKBAR_BOTTOM);

	return 0;
}

void CChildFrameWnd::ShowContentBar(BOOL bShow)
{
	ShowControlBar(&m_ContentBar, bShow, FALSE);
}
