// CategoryView.cpp : implementation file
//

#include "stdafx.h"
#include "..\MobileCnnt.h"
#include "CategoryView.h"
#include "..\MobileCnntDoc.h"
#include "InfoWrapper.h"
#include "MsgIDDef.h"

#ifndef __GLOBALDEF_H__
#include "GlobalDef.h"
#endif // __GLOBALDEF_H__

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCategoryView

IMPLEMENT_DYNCREATE(CCategoryView, CTreeView)

CCategoryView::CCategoryView()
{
	m_bInit   = TRUE;
	m_pImgLst = NULL;
	m_bDelete = FALSE;
}

CCategoryView::~CCategoryView()
{
	if( m_pImgLst != NULL )
	{
		m_pImgLst->DeleteImageList();
		delete m_pImgLst;
		m_pImgLst = NULL;
	}
}

BEGIN_MESSAGE_MAP(CCategoryView, CTreeView)
	//{{AFX_MSG_MAP(CCategoryView)
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCategoryView drawing

void CCategoryView::OnDraw(CDC* /*pDC*/)
{
//	CDocument* pDoc = (CDocument *)GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CCategoryView diagnostics

#ifdef _DEBUG
void CCategoryView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CCategoryView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CMobileSpriteDoc* CCategoryView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSMSDoc)));
	return (CMobileSpriteDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCategoryView message handlers

BOOL CCategoryView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= TVS_HASLINES;
	cs.style |= TVS_LINESATROOT;
	cs.style |= TVS_HASBUTTONS;
	cs.style |= TVS_SHOWSELALWAYS;
	cs.style &= ~TVS_EDITLABELS;

	return CTreeView::PreCreateWindow(cs);
}

void CCategoryView::OnInitialUpdate() 
{	
	// TODO: Add your specialized code here and/or call the base class
	//
	m_bInit = TRUE;

	if( NULL == m_pImgLst )
    {
        m_pImgLst = new CImageList;
        if(m_pImgLst != NULL)
        {
            if( m_pImgLst->Create( 16, 16, ILC_COLOR24 | ILC_MASK, 0, 1 ) )
			{
				CBitmap bmp;
				VERIFY( bmp.LoadBitmap(IDB_BMP_CATEGORY) );
				VERIFY( m_pImgLst->Add( &bmp, RGB(255, 255, 255) ) != -1 );
				
				CTreeCtrl &trCtrl = GetTreeCtrl();//lint !e1514
				trCtrl.SetImageList(m_pImgLst, TVSIL_NORMAL);
			}
        }
    }

	CTreeView::OnInitialUpdate();
}

void CCategoryView::FillCtrlContent()
{
	
	CTreeCtrl &trCtrl = GetTreeCtrl();//lint !e1514

	CInfoWrapper * pInfo = theApp.GetInfoWrapper();
	_ASSERT(pInfo != NULL);

	PCATEGINFO pCI = pInfo->m_pCategInfo;
	_ASSERT(pCI != NULL);
	//
	LockWindowUpdate();

	trCtrl.DeleteAllItems();

	for( ; pCI != NULL; pCI = pCI->pNext )
	{
#ifdef SPRD8800
		//teana
		if(pCI->dwData == MAKELONG(ID_STORE_ALL,   ID_CATEG_PBG) ||
			pCI->dwData == MAKELONG(ID_STORE_ALL,   ID_CATEG_MEMO))
		{
			continue;
		}
#else
		if(!theApp.m_bSupportMemo && pCI->dwData == MAKELONG(ID_STORE_ALL, ID_CATEG_MEMO))
		{
			continue;
		}
#endif
		
		RecursiveFillCtrlContent(pCI, TVI_ROOT);
	}


	UnlockWindowUpdate();

	HTREEITEM hRoot = trCtrl.GetRootItem();
	if( hRoot != NULL )
	{
		CSMSDoc * pDoc = (CSMSDoc *)GetDocument();
		_ASSERTE( pDoc );
		ASSERT_VALID( pDoc );
		
		trCtrl.SelectItem(hRoot);

		DWORD dwData    = (DWORD)trCtrl.GetItemData(hRoot);
		pDoc->m_dwCateg = HIWORD(dwData);
		pDoc->UpdateAllViews( this, WM_CATEGORY_CHANGE, (CObject *)dwData );
	}
}

void CCategoryView::RecursiveFillCtrlContent(LPVOID pItem, HTREEITEM hParent)
{
	if( NULL == pItem )
		return;

	
	CTreeCtrl &trCtrl = GetTreeCtrl();//lint !e1514
   
	PCATEGINFO pCI = (PCATEGINFO)pItem;
		
	if(!theApp.IsPluggedSIM(1))
	{		
		if(pCI->dwData == MAKELONG(ID_STORE_SIM,   ID_CATEG_PB))
			return;
	}
	if(!theApp.IsPluggedSIM(2))
	{		
		if(pCI->dwData == MAKELONG(ID_STORE_SIM2,   ID_CATEG_PB))
			return;
	}
	if(!theApp.IsPluggedSIM(3))
	{		
		if(pCI->dwData == MAKELONG(ID_STORE_SIM3,   ID_CATEG_PB))
			return;
	}
	if(!theApp.IsPluggedSIM(4))
	{		
		if(pCI->dwData == MAKELONG(ID_STORE_SIM4,   ID_CATEG_PB))
			return;
	}


	HTREEITEM htiCur = trCtrl.InsertItem( pCI->szText, pCI->nImage, pCI->nSelectedImage, hParent );
    trCtrl.SetItemData(htiCur, (DWORD)pCI->dwData);    
    for ( PCATEGINFO pCur = pCI->pChild; pCur != NULL; pCur = pCur->pNext )   
    {  
        RecursiveFillCtrlContent(pCur, htiCur);
    }
}

void CCategoryView::OnUpdate(CView* pSender, LPARAM lHint, CObject* /*pHint*/) 
{
	// TODO: Add your specialized code here and/or call the base class
	UNUSED_ALWAYS(pSender);
	if( m_bInit /*|| (pSender != this && lHint == WM_LANGUAGE_CHANGE)*/ )
	{
		m_bInit = FALSE;

		FillCtrlContent();
	}

	if(!m_bInit && lHint == WM_UPDATE_GROUP_CHANGE )
	{
		
		CTreeCtrl &trCtrl = GetTreeCtrl();//lint !e1514
		HTREEITEM htiParent = trCtrl.GetChildItem(TVI_ROOT);

		CInfoWrapper * pInfo = theApp.GetInfoWrapper();
		_ASSERT(pInfo != NULL);

		PCATEGINFO pCI = pInfo->m_pCategInfo;
		_ASSERT(pCI != NULL);

		HTREEITEM htiPB = NULL;
		PCATEGINFO pCIPB = NULL;

		DWORD dwCateg = 0;
		//
		LockWindowUpdate();
        while(pCI->pNext != NULL)
		{
			pCI= pCI->pNext;

			htiParent = trCtrl.GetNextItem(htiParent,TVGN_NEXT );
			dwCateg = trCtrl.GetItemData(htiParent);
			if( HIWORD(dwCateg) == ID_CATEG_PB && LOWORD(dwCateg) == ID_STORE_ALL)
			{
				htiPB = htiParent;
				pCIPB = pCI;

#ifdef _EGO
				if(!theApp.m_bSupportMemo)
				{
					pCI= pCI->pNext;  // skip memo
				}
#endif
			}
		}
		
		if(pCIPB != NULL)
		{
			int nSPNum = 0;  // store place number
			HTREEITEM htiPBSPLast;
			PCATEGINFO pCIPBSPLast;

			HTREEITEM htiPBSPms = NULL;
			PCATEGINFO pCIPBSPms = NULL;

			HTREEITEM htiPBSP;
			PCATEGINFO pCIPBSP;
			htiPBSPLast = trCtrl.GetChildItem(htiPB);
			pCIPBSPLast = pCIPB->pChild;
			htiPBSP = htiPBSPLast;
			pCIPBSP = pCIPBSPLast; 
			while(htiPBSP!= NULL)
			{
				++nSPNum;
				htiPBSPLast = htiPBSP;
				pCIPBSPLast = pCIPBSP; 

				dwCateg = trCtrl.GetItemData(htiPBSP);
				if(LOWORD(dwCateg) == ID_STORE_PHONE)
				{
					htiPBSPms = htiPBSP;
					pCIPBSPms = pCIPBSP; 
				}

				htiPBSP = trCtrl.GetNextSiblingItem(htiPBSP);
				pCIPBSP = pCIPBSP->pNext;
			}

			PCATEGINFO pCISIM1 = NULL;
			HTREEITEM  htiSIM1 = NULL;
			PCATEGINFO pCISIM2 = NULL;
			HTREEITEM  htiSIM2 = NULL;
			PCATEGINFO pCISIM3 = NULL;
			HTREEITEM  htiSIM3 = NULL;
			PCATEGINFO pCISIM4 = NULL;
			HTREEITEM  htiSIM4 = NULL;

			HTREEITEM  htiSP = htiPBSPms;
			m_bDelete = TRUE;
			while( (htiSP = trCtrl.GetNextSiblingItem(htiPBSPms)) != NULL)
			{				
				trCtrl.DeleteItem(htiSP);				
			}
			m_bDelete = FALSE;

			if(theApp.IsPluggedSIM(1))
			{
				pCISIM1 = pCIPBSPms->pNext;				
				htiSIM1 = trCtrl.InsertItem(pCISIM1->szText,pCISIM1->nImage,pCISIM1->nSelectedImage,htiPB);
				trCtrl.SetItemData(htiSIM1,pCISIM1->dwData);
			}

			if(theApp.IsPluggedSIM(2))
			{
				pCISIM2 = pCIPBSPms->pNext->pNext;				
				htiSIM2 = trCtrl.InsertItem(pCISIM2->szText,pCISIM2->nImage,pCISIM2->nSelectedImage,htiPB);
				trCtrl.SetItemData(htiSIM2,pCISIM2->dwData);
			}

			if(theApp.IsPluggedSIM(3))
			{
				pCISIM3 = pCIPBSPms->pNext->pNext->pNext;					
				htiSIM3 = trCtrl.InsertItem(pCISIM3->szText,pCISIM3->nImage,pCISIM3->nSelectedImage,htiPB);
				trCtrl.SetItemData(htiSIM3,pCISIM3->dwData);
			}

			if(theApp.IsPluggedSIM(4))
			{
				pCISIM4 = pCIPBSPms->pNext->pNext->pNext->pNext;					
				htiSIM4 = trCtrl.InsertItem(pCISIM4->szText,pCISIM4->nImage,pCISIM4->nSelectedImage,htiPB);
				trCtrl.SetItemData(htiSIM4,pCISIM4->dwData);
			}
		
 		}
#ifndef SPRD8800
		//M和6800的手机群组数目不同，所以必须删除后重新插入
		HTREEITEM hSelItem = trCtrl.GetSelectedItem();
		DWORD dwData = 0;

		if(hSelItem!= NULL)
		  dwData = trCtrl.GetItemData(hSelItem);
		
		m_bDelete = TRUE;
		trCtrl.DeleteItem(htiParent);
		m_bDelete = FALSE;
		
		PCATEGINFO pCur  = NULL;
        RecursiveFillCtrlContent(pCI, TVI_ROOT);
		
		trCtrl.Expand(htiParent,TVE_EXPAND);
		
		UnlockWindowUpdate();

		if(hSelItem!=NULL && HIWORD(dwData)==ID_CATEG_PBGM)
		{
			HTREEITEM htrItem = trCtrl.GetChildItem(htiParent);		
			for ( pCur = pCI->pChild; pCur != NULL && htrItem != NULL; pCur = pCur->pNext )   
			{  				
				if(dwData == (DWORD)pCur->dwData )
				{
					trCtrl.SelectItem(htrItem);
					break;
				}
				htrItem = trCtrl.GetNextSiblingItem(htrItem);
			}
		}
#endif
	}
}

void CCategoryView::OnDestroy() 
{
	CTreeView::OnDestroy();
	
	// TODO: Add your message handler code here
	if( m_pImgLst != NULL )
	{
		m_pImgLst->DeleteImageList();
		delete m_pImgLst;
		m_pImgLst = NULL;
	}
}

void CCategoryView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_bDelete)
		return;
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CTreeCtrl &trCtrl = GetTreeCtrl();//lint !e1514
    DWORD dwData = trCtrl.GetItemData( pNMTreeView->itemNew.hItem );

	CSMSDoc * pDoc = (CSMSDoc *)GetDocument();
	_ASSERTE( pDoc );
	ASSERT_VALID( pDoc );
		
	pDoc->m_dwCateg = dwData;//HIWORD(dwData);
	pDoc->UpdateAllViews( this, WM_CATEGORY_CHANGE, (CObject *)dwData );

	*pResult = 0;
}

