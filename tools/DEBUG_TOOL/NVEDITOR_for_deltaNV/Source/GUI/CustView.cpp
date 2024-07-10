// CustView.cpp : implementation file
//

#include "stdafx.h"
#include "nveditor.h"
#include "CustView.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustView
void WINAPI CBSyncEditorView()
{
	CMainFrame * pMF= (CMainFrame *)AfxGetMainWnd();	
	pMF->GetCustView()->UpdatedMsgAllViews(WM_REFRESH_CUST_TREE,NULL);	
}
IMPLEMENT_DYNCREATE(CCustView, CNVEditorView)

CCustView::CCustView()
{
	m_bLoadCustBase = FALSE;
}

CCustView::~CCustView()
{
}


BEGIN_MESSAGE_MAP(CCustView, CNVEditorView)
	//{{AFX_MSG_MAP(CCustView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustView drawing

void CCustView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	UNUSED_ALWAYS(pDoc);
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CCustView diagnostics

#ifdef _DEBUG
void CCustView::AssertValid() const
{
	CView::AssertValid();
}

void CCustView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCustView message handlers
void CCustView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( pSender != NULL && lHint == WM_CREATE_CUST_TREE && pHint )
    {
		CreateCustTree((CFileParser*)pHint,FALSE);
    }
	else if( pSender != NULL && lHint == WM_DESTROY_CUST_TREE )
	{
		DestoryCustTree();
	}
	else if ( pSender != NULL && lHint == WM_REFRESH_CUST_TREE )
	{
		if (!m_bLoadCustBase)
		{
			CreateCustTree((CFileParser*)pHint,FALSE);
		}
		else
		{
			m_List.Refresh();
		}
		
	}
	else if( pSender != NULL && lHint == WM_SAVE_CUST_BASE && pHint)
	{
		SaveCustBaseNV((LPCTSTR)pHint,FALSE);
	}
	else if( pSender != NULL && lHint == WM_SAVEAS_CUST_BASE && pHint)
	{
		SaveCustBaseNV((LPCTSTR)pHint,TRUE);
	}
}
void CCustView::CreateCustTree(CFileParser* pFileParse, BOOL bNeedMinEdior /*= TRUE*/)
{
	if (NULL == pFileParse)
	{
		return;
	}
	((CMainFrame*)AfxGetMainWnd())->ShowCustPane(TRUE);	
	CRect rcWindow;
	m_bLoadCustBase = TRUE;
	GetClientRect(&rcWindow);
	OnSize(0, rcWindow.Width(), rcWindow.Height());
    
	m_pProject = pFileParse;
	m_List.SetCustMode(TRUE);
	m_List.SetProject (m_pProject);	
	m_List.Refresh();	
	if (bNeedMinEdior)
	{
		((CMainFrame*)AfxGetMainWnd())->MinSplitPane(E_EDITOR_PANE);
	}
		
}

void CCustView::DestoryCustTree()
{
	m_List.CloseProject();
	m_bLoadCustBase = FALSE;
	m_List.SetWindowPos (NULL, -1, -1, 0, 0, SWP_HIDEWINDOW);
	((CMainFrame*)AfxGetMainWnd())->ShowCustPane(FALSE);
}
int CCustView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	m_List.SetSyncCBFunc(CBSyncEditorView);
    m_List.Create(WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),this,0x100);
    m_List.InitializeGrid();
	m_List.SetModifyID(FALSE);

	return 0;
}

void CCustView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_List.GetSafeHwnd () && m_bLoadCustBase)
    {
		m_List.SetWindowPos (0, -1, -1, cx, cy, SWP_SHOWWINDOW);
    }
	
}

BOOL CCustView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	if(FALSE == m_bLoadCustBase)
    {
        //改变视窗背景色 
        CBrush Brush;
        Brush.CreateSolidBrush (::GetSysColor (COLOR_SCROLLBAR));
        CBrush *pOldBrush = pDC->SelectObject (&Brush);
        
        CRect rcClip;
        pDC->GetClipBox (&rcClip);
        pDC->PatBlt (rcClip.left , rcClip.top , rcClip.Width ( ) ,
            rcClip.Height( ) , PATCOPY );
        
        pDC->SelectObject (pOldBrush);
        
        return TRUE;
    }
	return CView::OnEraseBkgnd(pDC);
}

void CCustView::UpdatedMsgAllViews( int nMsgID, CObject* pObj/* = NULL*/ )
{
	CDocument * pDoc = NULL;
	pDoc = (CDocument *)GetDocument();
	if (pDoc)
	{
		pDoc->UpdateAllViews(this,nMsgID,pObj);
	}
}

void  CCustView::SaveCustBaseNV(LPCTSTR lpPrjFile, BOOL bSaveAs/* = FALSE*/)
{
	if (!m_bLoadCustBase ||NULL == lpPrjFile )
	{
		return;
	}
	if ( !bSaveAs && SUPER_PERMISSION != theApp.GetPermission())
	{
		return;
	}
	CString strCustBaseNVFile = GetStrFilePath(lpPrjFile,FALSE,TRUE);
	strCustBaseNVFile += g_szCustBaseFile;
	if( IsExistFile(strCustBaseNVFile.operator LPCTSTR()) || m_List.IsExistCustNV(TRUE))
	{
		m_List.ExportCustNV(strCustBaseNVFile.operator LPCTSTR(),TRUE);
	}	
}