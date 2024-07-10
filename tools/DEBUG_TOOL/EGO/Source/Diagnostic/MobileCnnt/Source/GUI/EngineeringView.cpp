// EngineeringView.cpp : implementation file
//

#include "stdafx.h"
#include "mobilecnnt.h"
#include "EngineeringView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEngineeringView

IMPLEMENT_DYNCREATE(CEngineeringView, CFormView)

CEngineeringView::CEngineeringView()
	: CCoolFormView(CEngineeringView::IDD)
{
	//{{AFX_DATA_INIT(CEngineeringView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pInfo.RemoveAll();
	m_pCgdcontPage = NULL;
	m_pSppsratePage = NULL;
	m_pSpfrqPage = NULL;
	m_pSautoattPage = NULL;
	m_pSptestPage = NULL;
	m_pSpidPage = NULL;
	m_pSpuplmnPage = NULL;
	m_pSysconfigPage = NULL;
	m_pATCmdPage = NULL;
	m_pDspPage = NULL;
	m_uOldIndex = 0;
}

CEngineeringView::~CEngineeringView()
{
	m_pCgdcontPage = NULL;
	m_pSppsratePage = NULL;
	m_pSpfrqPage = NULL;
	m_pSautoattPage = NULL;
	m_pSptestPage = NULL;
	m_pSpidPage = NULL;
	m_pSpuplmnPage = NULL;
	m_pSysconfigPage = NULL;
	m_pATCmdPage = NULL;
	m_pDspPage = NULL;
}

void CEngineeringView::DoDataExchange(CDataExchange* pDX)
{
	CCoolFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEngineeringView)
	DDX_Control(pDX, IDC_LST_ITEM, m_lst);
	DDX_Control(pDX, IDC_PAGE_FRAME, m_PageFrame);
	DDX_Control(pDX, IDC_CAPTION_BAR, m_CaptionBarCtrl);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEngineeringView, CCoolFormView)
	//{{AFX_MSG_MAP(CEngineeringView)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_LST_ITEM, OnSelchangeLstItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEngineeringView diagnostics

#ifdef _DEBUG
void CEngineeringView::AssertValid() const
{
	CFormView::AssertValid();
}

void CEngineeringView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEngineeringView message handlers

HBRUSH CEngineeringView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
#ifdef SPRD8800
	pDC->SetBkMode(TRANSPARENT);
	return GetSysColorBrush(COLOR_WINDOW);
#endif
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return hbr;
}

void CEngineeringView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	for(int i =0; i < CONFIG_PAGE_NUM; i++)
	{
		m_lst.SetItemHeight(i, ITEM_HEIGHT);
	}
	
	CString strText;
	strText.LoadString(IDS_CGDCONT);
	SetListText(strText);
	strText.LoadString(IDS_SPPSRATE);
	SetListText(strText);
	strText.LoadString(IDS_SPFRQ);
	SetListText(strText);
	strText.LoadString(IDS_SAUTOATT);
	SetListText(strText);
	strText.LoadString(IDS_SPTEST);
	SetListText(strText);
	strText.LoadString(IDS_SPID);
	SetListText(strText);
	strText.LoadString(IDS_SPUPLMN);
	SetListText(strText);
	strText.LoadString(IDS_TDGSM);
	SetListText(strText);
	strText.LoadString(IDS_AT_CMD);
	SetListText(strText);
	strText.LoadString(IDS_DSP);
	SetListText(strText);

	m_lst.SetCurSel(0);
	
	//////////////////////////////////////////////////////////////////////////
	m_PageFrame.GetWindowRect(m_FrameRect);
	ScreenToClient(m_FrameRect);
	m_FrameRect.DeflateRect(2, 2);
	m_FrameRect.top += 5;

	// Set some styles for indicator bar
	m_CaptionBarCtrl.m_textClr     = ::GetSysColor(COLOR_3DFACE);
	m_CaptionBarCtrl.m_fontWeight  = FW_BOLD;
	m_CaptionBarCtrl.m_fontSize    = 14;
	m_CaptionBarCtrl.m_csFontName  = "Verdana";
	m_CaptionBarCtrl.SetConstantText(m_csLogoText);

	//////////////////////////////////////////////////////////////////////////
	char szText[100] = {0};
	strText.LoadString(IDS_CGDCONT);
#ifdef _UNICODE
	WideCharToMultiByte( CP_ACP, 0, strText, -1, szText, 100, NULL, NULL);
#else
	strcpy(szText,strText);
#endif
	m_pCgdcontPage = (CCgdcontPage *)AddPage(RUNTIME_CLASS(CCgdcontPage), szText, IDD_PAGE_CGDCONT, "");
	strText.LoadString(IDS_SPPSRATE);
#ifdef _UNICODE
	WideCharToMultiByte( CP_ACP, 0, strText, -1, szText, 100, NULL, NULL);
#else
	strcpy(szText,strText);
#endif
	m_pSppsratePage = (CSppsratePage *)AddPage(RUNTIME_CLASS(CSppsratePage), szText, IDD_PAGE_SPPSRATE, "");
	strText.LoadString(IDS_SPFRQ);
#ifdef _UNICODE
	WideCharToMultiByte( CP_ACP, 0, strText, -1, szText, 100, NULL, NULL);
#else
	strcpy(szText,strText);
#endif
	m_pSpfrqPage = (CSpfrqPage *)AddPage(RUNTIME_CLASS(CSpfrqPage), szText, IDD_PAGE_SPFRQ, "");
	strText.LoadString(IDS_SAUTOATT);
#ifdef _UNICODE
	WideCharToMultiByte( CP_ACP, 0, strText, -1, szText, 100, NULL, NULL);
#else
	strcpy(szText,strText);
#endif
	m_pSautoattPage = (CSautoattPage *)AddPage(RUNTIME_CLASS(CSautoattPage), szText, IDD_PAGE_SAUTOATT, "");
	strText.LoadString(IDS_SPTEST);
#ifdef _UNICODE
	WideCharToMultiByte( CP_ACP, 0, strText, -1, szText, 100, NULL, NULL);
#else
	strcpy(szText,strText);
#endif
	m_pSptestPage = (CSptestPage *)AddPage(RUNTIME_CLASS(CSptestPage), szText, IDD_PAGE_SPTEST, "");
	strText.LoadString(IDS_SPID);
#ifdef _UNICODE
	WideCharToMultiByte( CP_ACP, 0, strText, -1, szText, 100, NULL, NULL);
#else
	strcpy(szText,strText);
#endif
	m_pSpidPage = (CSpidPage *)AddPage(RUNTIME_CLASS(CSpidPage), szText, IDD_PAGE_SPID, "");
	strText.LoadString(IDS_SPUPLMN);
#ifdef _UNICODE
	WideCharToMultiByte( CP_ACP, 0, strText, -1, szText, 100, NULL, NULL);
#else
	strcpy(szText,strText);
#endif
	m_pSpuplmnPage = (CSpuplmnPage *)AddPage(RUNTIME_CLASS(CSpuplmnPage), szText, IDD_PAGE_SPUPLMN, "");
	strText.LoadString(IDS_TDGSM);
#ifdef _UNICODE
	WideCharToMultiByte( CP_ACP, 0, strText, -1, szText, 100, NULL, NULL);
#else
	strcpy(szText,strText);
#endif
	m_pSysconfigPage = (CSysconfigPage *)AddPage(RUNTIME_CLASS(CSysconfigPage), szText, IDD_PAGE_SYSCONFIG, "");
	strText.LoadString(IDS_AT_CMD);
#ifdef _UNICODE
	WideCharToMultiByte( CP_ACP, 0, strText, -1, szText, 100, NULL, NULL);
#else
	strcpy(szText,strText);
#endif
	m_pATCmdPage = (CATCmdPage *)AddPage(RUNTIME_CLASS(CATCmdPage), szText, IDD_PAGE_CLIENT, "");
	
	strText.LoadString(IDS_DSP);
#ifdef  _UNICODE
	WideCharToMultiByte( CP_ACP, 0, strText, -1, szText, 100, NULL, NULL);
#else
	strcpy(szText,strText);
#endif
	m_pDspPage = (CDspPage *)AddPage(RUNTIME_CLASS(CDspPage), szText, IDD_PAGE_DSP, "");

	if (m_pInfo.GetAt(0))
	{
		ShowPage(m_pInfo.GetAt(0));
		m_uOldIndex = 0;
	}
	
}

void CEngineeringView::SetListText(CString strText)
{
	m_lst.AddString(strText);
	
	int nCount = m_lst.GetCount();
	m_lst.SetTopIndex(nCount - 1);
	if (nCount > 0)
	{
		m_lst.SetCurSel(nCount-1);
	}
}

CWnd* CEngineeringView::AddPage(CRuntimeClass  *pWndClass, 
							   const char *pCaption, UINT nID /* = 0 */, 
							   CWnd *pWndParent /* = NULL */)
{
	CWnd* pWnd = NULL;

	if (m_hWnd)
	{
		// Can't add once the window has been created
		ASSERT(0);
		return pWnd;
	}
	
	// Create the specified page object
	if (pWndClass) pWnd = (CWnd*) pWndClass->CreateObject();	

	PAGE_INFO *pInfo = new PAGE_INFO;
	pInfo->bViewClass = TRUE;			// Default to generic CWnd or CView class
	if(pWnd)							// If it is a dialog or dialog-derived class
	{									// Set bViewClass = FALSE
		if (pWnd->IsKindOf(RUNTIME_CLASS(CDialog)))	pInfo->bViewClass = FALSE;
	}

	pInfo->nID = nID;					// ID of the page
	pInfo->pWnd = pWnd;					// Pointer to the page
	pInfo->csCaption = pCaption;		// Caption of page in the tree control
	pInfo->pWndParent = pWndParent;		// Parent of the page if it has one
	pInfo->csParentCaption.Empty();		// Parent caption ignored
	m_pInfo.Add(pInfo);					// Add to the page list

	return pWnd;
}

CWnd* CEngineeringView::AddPage(CRuntimeClass *pWndClass, const char *pCaption, 
							   UINT nID, const char *pParentCaption)
{
	CWnd* pWnd = NULL;
	
//	if (m_hWnd)
//	{
//		// Can't add once the window has been created
//		ASSERT(0);
//		return pWnd;
//	}

	// Create the specified page object
	if (pWndClass) pWnd = (CWnd*) pWndClass->CreateObject();
	PAGE_INFO *pInfo = new PAGE_INFO;
	pInfo->bViewClass = TRUE;			// Default to generic CWnd or CView class
	if (pWnd)							// If it is a dialog or dialog-derived class
	{									// Set bViewClass = FALSE
		if (pWnd->IsKindOf(RUNTIME_CLASS(CDialog)))	pInfo->bViewClass = FALSE;
	}
	pInfo->nID = nID;					// ID of the page
	pInfo->pWnd = pWnd;					// Point to the page
	pInfo->csCaption = pCaption;		// Caption of page in the tree control
	pInfo->pWndParent = NULL;			// Parent page is not specified yet
	pInfo->csParentCaption = pParentCaption;	// Parent caption to be inserted under
	m_pInfo.Add(pInfo);					// Add to to page list

	return pWnd;
}

void CEngineeringView::ShowPage(const PAGE_INFO *pInfo, UINT nShow /* = SW_SHOW */)
{
	if (!pInfo) return;

	if (pInfo->pWnd)						// If the page is valid
	{
		if (!::IsWindow(pInfo->pWnd->m_hWnd))
		{									// Window has not been created, create it
			CreatePage(pInfo);
			pInfo->pWnd->SetWindowPos(&m_lst,0,0,0,0,0);
			pInfo->pWnd->MoveWindow(m_FrameRect.left, m_FrameRect.top, m_FrameRect.Width(), m_FrameRect.Height());
			if( pInfo->pWnd->IsKindOf( RUNTIME_CLASS(CView) ) )
				((CView*)pInfo->pWnd)->OnInitialUpdate();
		}
		
		pInfo->pWnd->ShowWindow(nShow);		// Show or hide the window
		if (nShow == SW_SHOW)				// Change the tree selection
		{
			pInfo->pWnd->SetFocus();
		}
	}
	if (nShow == SW_SHOW) 					// Update caption bar
		m_CaptionBarCtrl.SetWindowText(pInfo->csCaption);
}

BOOL CEngineeringView::CreatePage(const PAGE_INFO *pInfo)
{
	BOOL bCode = FALSE;

	if (!pInfo || !pInfo->pWnd) return(FALSE);	// If no page is specified, return NULL
	if (!::IsWindow(pInfo->pWnd->m_hWnd))		// If the window has not yet been created,
	{
		if (pInfo->pWnd->IsKindOf(RUNTIME_CLASS(CDialog)))	// If the page window is kind of dialog window
		{
			CDialog *pDlg = (CDialog*)pInfo->pWnd;
			bCode = pDlg->Create(pInfo->nID, this);
			pDlg->ModifyStyle(WS_CAPTION, 0);
		}
		else if (pInfo->pWnd->IsKindOf(RUNTIME_CLASS(CWnd)))	// generic CWnd derived Window
		{
			CWnd *pWnd = (CWnd*)pInfo->pWnd;
			bCode = CreateWnd(pInfo->pWnd);						// Create Window 
			pWnd->ModifyStyle(WS_BORDER|WS_THICKFRAME, 0);		// Remoce border and thick frame styles
		}
	}
	return(bCode);
}

BOOL CEngineeringView::CreateWnd(CWnd *pWnd, CCreateContext *pContext /* = NULL */)
{
	CCreateContext context;	
	if (pContext == NULL)	
	{
		// If no context specified, generate one from the currently selected
		//  client if possible
		context.m_pCurrentFrame = (CFrameWnd*) this;
		context.m_pCurrentDoc = NULL;
		context.m_pNewViewClass = NULL;
		pContext = &context;

	}
	
    ASSERT_KINDOF(CWnd, pWnd);
	ASSERT(pWnd->m_hWnd == NULL);       // Not yet created
	
	DWORD dwStyle = AFX_WS_DEFAULT_VIEW;
	CRect rect;
	// Create with the right size and position
	if (!pWnd->Create(NULL, NULL, dwStyle, rect, this, 0, pContext))
	{
		TRACE0("Warning: couldn't create wnd in CSettingsDialog.\n");
		// pWnd will be cleaned up by PostNcDestroy
		return FALSE;
	}
	return(TRUE);
}

BOOL CEngineeringView::DestroyPages()
{
	for (int i=0; i<m_pInfo.GetSize(); i++)
	{
		PAGE_INFO *pInfo = (PAGE_INFO *)m_pInfo.GetAt(i);
		if (pInfo && pInfo->pWnd)
		{
			if (::IsWindow(pInfo->pWnd->m_hWnd))
			{
				pInfo->pWnd->DestroyWindow();		// Destroy the windows
				if (!(pInfo->bViewClass))  
					delete(pInfo->pWnd);	// Delete the C++ object
			}
		}
	}

	return(true);
}

BOOL CEngineeringView::RefreshData()
{
	PAGE_INFO *pInfoErr = NULL, *pInfo;
	
	UNREFERENCED_PARAMETER(pInfoErr);
	// First, UpdateData... 
	for (int i = 0; i < m_pInfo.GetSize(); i++)
	{
		pInfo = (PAGE_INFO *)m_pInfo.GetAt(i);
		if (pInfo && pInfo->pWnd)
		{
			if (::IsWindow(pInfo->pWnd->m_hWnd))
				pInfo->pWnd->UpdateData(TRUE);
		}
	}
	return (TRUE);		
}

void CEngineeringView::OnDestroy() 
{
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	for (int i=0; i<m_pInfo.GetSize(); i++)
	{
		PAGE_INFO *pInfo = (PAGE_INFO *)m_pInfo.GetAt(i);
		if (pInfo) 
		{
			delete pInfo;
		}
	}
	m_pInfo.RemoveAll();
	
}

void CEngineeringView::OnSelchangeLstItem() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_lst.GetCurSel();

	ShowPage(m_pInfo.GetAt(m_uOldIndex), SW_HIDE);
	ShowPage(m_pInfo.GetAt(nIndex), SW_SHOW);

	m_uOldIndex = nIndex;
	
}

void CEngineeringView::ShowRespondContent(LPCSTR szRet)
{
	CString strtext = szRet;
	if(strtext.Find(_T("+SPUPLMN:")) != -1)
	{
		if(m_pSpuplmnPage != NULL)
		{
			m_pSpuplmnPage->ShowRespondContent(szRet);
		}
	}
	if(strtext.Find(_T("TMSI:")) != -1 || strtext.Find(_T("IMEISV:")) != -1)
	{
		if(m_pSpidPage != NULL)
		{
			m_pSpidPage->ShowRespondContent(szRet);
		}
	}
	if(strtext.Find(_T("+CGDCONT:")) != -1 )
	{
		if(m_pCgdcontPage != NULL)
		{
			m_pCgdcontPage->ShowRespondContent(szRet);
		}
	}
	if(strtext.Find(_T("+SPPSRATE:")) != -1)
	{
		if(m_pSppsratePage != NULL)
		{
			m_pSppsratePage->ShowRespondContent(szRet);
		}
	}
	if(strtext.Find(_T("+SPFRQ:")) != -1)
	{
		if(m_pSpfrqPage != NULL)
		{
			m_pSpfrqPage->ShowRespondContent(szRet);
		}
	}
	if(strtext.Find(_T("+SAUTOATT:")) != -1)
	{
		if(m_pSautoattPage != NULL)
		{
			m_pSautoattPage->ShowRespondContent(szRet);
		}
	}
	if(strtext.Find(_T("+SPTEST:")) != -1)
	{
		if(m_pSptestPage != NULL)
		{
			m_pSptestPage->ShowRespondContent(szRet);
		}
	}

	if(strtext.Find(_T("^SYSCONFIG:")) != -1)
	{
		if(m_pSysconfigPage != NULL)
		{
			m_pSysconfigPage->ShowRespondContent(szRet);
		}
	}
	
	//teana hu 2009.11.02
	if(m_pATCmdPage != NULL)
	{
		m_pATCmdPage->ShowRespondContent(szRet);
	}
}

void CEngineeringView::ClearAll()
{
	if(m_pCgdcontPage->m_hWnd != NULL)
	{
		m_pCgdcontPage->ClearAll();
	}
	if(m_pSpidPage->m_hWnd != NULL)
	{
		m_pSpidPage->ClearAll();
	}
	if(m_pSpuplmnPage->m_hWnd != NULL)
	{
		m_pSpuplmnPage->ClearAll();
	}
	if(m_pSppsratePage->m_hWnd != NULL)
	{
		m_pSppsratePage->ClearAll();
	}
	if(m_pSpfrqPage->m_hWnd != NULL)
	{
		m_pSpfrqPage->ClearAll();
	}
	if(m_pSautoattPage->m_hWnd != NULL)
	{
		m_pSautoattPage->ClearAll();
	}
	if(m_pSptestPage->m_hWnd != NULL)
	{
		m_pSptestPage->ClearAll();
	}
	if(m_pSysconfigPage->m_hWnd != NULL)
	{
		m_pSysconfigPage->ClearAll();
	}
	if(m_pATCmdPage->m_hWnd != NULL)
	{
		m_pATCmdPage->ClearAll();
	}
	if(m_pDspPage->m_hWnd != NULL)
	{
		m_pDspPage->ClearAll();
	}
	
}