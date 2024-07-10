// ContentView.cpp : implementation file
//

#include "stdafx.h"
#include "..\mobileCnnt.h"
#include "ContentView.h"
#include "InfoWrapper.h"
#include "GlobalDef.h"
#include "MsgIDDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContentView

IMPLEMENT_DYNCREATE(CContentView, CEditView)

CContentView::CContentView()
{
	m_pIW = NULL;
}

CContentView::~CContentView()
{
	m_pIW = NULL;
}


BEGIN_MESSAGE_MAP(CContentView, CEditView)
	//{{AFX_MSG_MAP(CContentView)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContentView drawing

void CContentView::OnDraw(CDC* /*pDC*/)
{
//	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CContentView diagnostics

#ifdef _DEBUG
void CContentView::AssertValid() const
{
	CEditView::AssertValid();
}

void CContentView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CContentView message handlers

void CContentView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CEdit &edtCtrl = GetEditCtrl();
	edtCtrl.SetWindowText(_T(""));

	if( pSender != NULL && lHint == WM_UPDATE_ITEM_CONTENT && pHint != NULL )
	{
		UpdateContent(pHint);
	}
}

void CContentView::OnInitialUpdate() 
{
	CEditView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CMobileCnntApp * pApp = (CMobileCnntApp *)AfxGetApp();
	_ASSERTE( pApp != NULL );

	m_pIW = pApp->GetInfoWrapper();
}

BOOL CContentView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= ES_READONLY;
	cs.style |= ES_MULTILINE;
	cs.style |= ES_AUTOVSCROLL;

	return CEditView::PreCreateWindow(cs);
}

void CContentView::UpdateContent(CObject * pHint)
{
	_ASSERTE( pHint != NULL );

	PUIC_HINT pUic = (PUIC_HINT)pHint;

	WORD wCateg = HIWORD(pUic->dwFlag);
	
	CEdit &edtCtrl = GetEditCtrl();

	switch(wCateg)
	{
	case ID_CATEG_SMS:
		{		
			PSMS psms = (PSMS)pUic->dwData;
			if(psms->pNext == NULL)
				edtCtrl.SetWindowText(psms->szContent);
			else
			{
				CString str,strContent;
				str = psms->szContent;
				strContent = str;
				PSMS ps = psms->pNext;
				while(ps!=NULL)
				{
					str = ps->szContent;
					strContent += str;
					ps=ps->pNext;
				}
				edtCtrl.SetWindowText(strContent);
			}
		}
		break;
	case ID_CATEG_PB:
		break;
	case ID_CATEG_MEMO:
		{
			PMEMO pMemo = (PMEMO)pUic->dwData;
			edtCtrl.SetWindowText(pMemo->szContent);
		}
		break;
	case ID_CATEG_PBG:
		break;
	case ID_CATEG_PBGM:
		break;
	default:
		_ASSERTE(0);
		break;
	}
}

HBRUSH CContentView::CtlColor(CDC* pDC, UINT /*nCtlColor*/) 
{
	// TODO: Change any attributes of the DC here
	pDC->SetBkMode(TRANSPARENT);
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return GetSysColorBrush(COLOR_WINDOW);
}
