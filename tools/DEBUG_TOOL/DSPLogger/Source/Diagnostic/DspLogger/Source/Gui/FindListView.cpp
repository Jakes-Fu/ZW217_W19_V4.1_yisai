// FindListView.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "FindListView.h"
#include "DspKernelExport.h"

#include "mainfrm.h"
#include "FindMainFrame.h"

#include "FindDoc.h"
#include "DspLoggerDoc.h"

#include "LUA_FUNC.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFindDoc *g_pFindDoc = NULL;
/////////////////////////////////////////////////////////////////////////////
// CFindListView

IMPLEMENT_DYNCREATE(CFindListView, CListView)

CFindListView::CFindListView()
{
	m_pDoc			= NULL;
    m_pSet			= NULL;
	m_nSelectIdx	= 0;
    m_dwStartPos    = 0;
    m_dwItemCount   = 0;
	m_uLastTPCount	= 0;
	m_vecColorIdx.reserve(100000);
	m_vecFind.reserve(100000);
	
	//teana hu 2011.06.01
	memset(m_arrSigned, 0, sizeof(BOOL) * MAX_FIND_DATA_NUM);
}

CFindListView::~CFindListView()
{
	m_vecColorIdx.clear();
}//lint !e1540


BEGIN_MESSAGE_MAP(CFindListView, CListView)
	//{{AFX_MSG_MAP(CFindListView)
	ON_WM_DESTROY()
    ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindListView drawing

void CFindListView::OnDraw(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	//CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CFindListView diagnostics

#ifdef _DEBUG
void CFindListView::AssertValid() const
{
	CListView::AssertValid();
}

void CFindListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFindListView message handlers

void CFindListView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();

    m_pDoc = (CFindDoc *)GetDocument();
    ASSERT_VALID( m_pDoc );	
	g_pFindDoc = m_pDoc;

	CListCtrl &lst = GetListCtrl();
	DWORD dwStyle = lst.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
    lst.SetExtendedStyle(dwStyle);

//     lst.InsertColumn(0, _T("Index"), LVCFMT_LEFT, 60);
//     lst.InsertColumn(1, _T("No."), LVCFMT_LEFT, 60);
//     lst.InsertColumn(2, _T("Addr"), LVCFMT_LEFT, 40);
//     lst.InsertColumn(3, _T("Data"), LVCFMT_LEFT, 40);
//     lst.InsertColumn(4, _T("Val-Signed"), LVCFMT_LEFT, 80);
// 	   lst.InsertColumn(5, _T("Val-Unsigned"), LVCFMT_LEFT, 80);
//     lst.InsertColumn(6, _T("Time"), LVCFMT_LEFT, 60);

    lst.InsertColumn(0, _T("No."), LVCFMT_LEFT, 60);
    lst.InsertColumn(1, _T("Addr"), LVCFMT_LEFT, 40);
    lst.InsertColumn(2, _T("Data"), LVCFMT_LEFT, 40);
    lst.InsertColumn(3, _T("Val-Signed"), LVCFMT_LEFT, 80);
	lst.InsertColumn(4, _T("Val-Unsigned"), LVCFMT_LEFT, 80);
    lst.InsertColumn(5, _T("Time"), LVCFMT_LEFT, 60);
	lst.InsertColumn(6, _T("Description"), LVCFMT_LEFT, 60);

    m_pSet = ((CMainFrame*)AfxGetMainWnd())->m_pDspKernel->GetTpSet();
    _ASSERTE( m_pSet != NULL );

}

BOOL CFindListView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= LVS_REPORT | LVS_SINGLESEL | LVS_OWNERDATA | LVS_SHOWSELALWAYS;

	return CListView::PreCreateWindow(cs);
}

void CFindListView::OnDestroy() 
{
	CListView::OnDestroy();
	
	ClearFind();
	
	g_pFindDoc = NULL;
}
void CFindListView::ClearFind()
{
	std::vector<DWORD>	vecTempFindIdx;
	std::vector<DWORD>	vecTempColorIdx;
	m_vecFind.swap(vecTempFindIdx);
	m_vecColorIdx.swap(vecTempColorIdx);
}
void CFindListView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{    
    if (lHint== WM_UPDATE_CLEAR)
    {
		ClearFind();
        m_dwItemCount= 0;
        GetListCtrl().SetItemCount(0);
    }

	if(lHint == WM_UPDATE_CLEAR_STATE)
	{
		m_uLastTPCount = 0;
		m_dwStartPos = 0;
	}

	if( pSender != NULL && lHint == WM_UPDATE_FIND  )
    {
        ClearFind();
        m_dwStartPos =0;
        m_dwItemCount=0;

        FindItems();
		
		//set current select item		
		for( UINT i = 0; i < m_vecFind.size(); ++i )
		{
			if( m_vecFind[i] == m_nSelectIdx )
			{				
				CListCtrl &lst = GetListCtrl();
				lst.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
				lst.EnsureVisible(i, FALSE);
				SetFocus();
				break;
			}
		}		
    }

    if(pSender != NULL && lHint==WM_AUTO_REFRESH )
    {
        FindItems(TRUE);
    }

    if( pSender != NULL && lHint == WM_GOTO_ITEM_BY_CHART )
    {
        PUC_HINT puch = (PUC_HINT)pHint;
		
		for( UINT i = 0; i < m_vecFind.size(); ++i )
		{
			if( m_vecFind[i] == puch->lParam )
			{
				m_nSelectIdx= puch->lParam;
				CListCtrl &lst = GetListCtrl();
				lst.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
				lst.EnsureVisible(i, FALSE);
				SetFocus();
				break;
			}
		}	       
    }

	//teana hu 2009.10.16
	if(pSender != NULL && lHint == WM_FIND_ITEM_IN_DOCVIEW)
	{		
		PUC_HINT pUcHint = (PUC_HINT)pHint;
		int nIndex = pUcHint->wParam;
		
		int nCount = m_vecFind.size();
		if(nCount <= 0)
		{
			return;
		}
		
		int nFindId = -1;
		for(int i = 0; i < nCount; i++)
		{
			if(nIndex == m_vecFind[i])
			{
				nFindId = i;
				break;
			}
		}

		if(nFindId == -1)
		{
			return;
		}
		
		CListCtrl &listctrl = GetListCtrl();
		listctrl.LockWindowUpdate();
		listctrl.SetFocus();
		nCount = listctrl.GetItemCount();
		if (nCount > 0)
		{
			listctrl.EnsureVisible(nCount-1, FALSE);
		}
		
		listctrl.EnsureVisible(nFindId>4 ? nFindId-4 : nFindId, FALSE);
		listctrl.SetItemState(nFindId,LVIS_SELECTED,LVIS_SELECTED);
		listctrl.UnlockWindowUpdate();

	}

	//teana hu 2011.06.01
	if( pSender != NULL && lHint == WM_UPDATE_CHART )
    {
        PUC_HINT puch = (PUC_HINT)pHint;		
		switch(puch->eAct)
		{
		case UCA_UNSIGNED:
			m_arrSigned[puch->nIdx] = FALSE;
			break;
		case UCA_SIGNED:
			m_arrSigned[puch->nIdx] = TRUE;
			break;
		default:
		//	_ASSERTE( 0 );
			break;
		}
    }

}

void CFindListView::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	LV_ITEM* pItem = &(pDispInfo->item);

    if(pItem->mask & LVIF_TEXT)
    {
        ULONG lIdx = m_vecFind[pItem->iItem];
        // Text needed
        DWORD dwTp = m_pSet->GetTp(lIdx);
		DWORD wData;
		DWORD wAddr;
		int	new_data;
        switch(pItem->iSubItem)
        {
        /*case 0: // index
            sprintf(pItem->pszText,"%d", pItem->iItem);
            break;*/
        case 0:  // No.
            // Item label text needed
            sprintf(pItem->pszText,"%d", lIdx);
            break;
            
        case 1: // address
            // Address label text needed
            sprintf(pItem->pszText,"%04X", GETTPADDR(dwTp));
            break;
            
        case 2: // data
            // Data label text needed  
            sprintf(pItem->pszText,"%04X", GETTPDATA(dwTp));
            break;
            
        case 3: // val-Signed
		case 4: //val-UnSigned
            // Value label text needed  
			wData = GETTPDATA(dwTp);
			// ----- lua processing data
#if DO_LUA_DATA_PROCESSING
			wAddr = GETTPADDR(dwTp);
			lua_data((int)wAddr,(int)wData,&new_data);
			wData = new_data;
#endif

			if( 3 == pItem->iSubItem)
			{
				sprintf(pItem->pszText,"%d", (short)wData);
			}
			else
			{
				sprintf(pItem->pszText,"%d", wData);
			}
            break;
            
        case 5: // time
            sprintf(pItem->pszText,"%ld", m_pSet->GetTPTime(lIdx));
            break;
		case 6: // description
			g_theApp.LookUpDescription(GETTPADDR(dwTp),GETTPDATA(dwTp),pItem->pszText);
            break;
        }
    }
	
	*pResult = 0;
}

void CFindListView::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

    // Take the default processing unless we set this to something else below.
    *pResult = CDRF_DODEFAULT;

    switch( pLVCD->nmcd.dwDrawStage )
    {
    case CDDS_PREPAINT:
        *pResult = CDRF_NOTIFYITEMDRAW;
        break;
    case CDDS_ITEMPREPAINT:
        {
            //WORD wAddr = m_pSet->GetTPAddr(m_vecFind[pLVCD->nmcd.dwItemSpec]);
			//WORD wData = m_pSet->GetTPData(m_vecFind[pLVCD->nmcd.dwItemSpec]);
            //pLVCD->clrText = (m_mapColor.find(std::make_pair(wAddr, wData))->second);
			int nIndex = m_vecColorIdx[pLVCD->nmcd.dwItemSpec];
			pLVCD->clrText = m_pDoc->m_arrFindColor[nIndex];
        }
        break;
    }
}

void CFindListView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMITEMACTIVATE pnc = (LPNMITEMACTIVATE)pNMHDR;

	CMainFrame* pMainFrame= (CMainFrame*)AfxGetMainWnd();
    CDspLoggerDoc * pDspLoggerDoc =(CDspLoggerDoc*) pMainFrame->m_pView->GetDocument();
    ASSERT_VALID( pDspLoggerDoc );

    if( pnc->iItem >= 0 )
    {
        _ASSERTE( pnc->iItem < m_vecFind.size() );

        static UC_HINT ucHint;
        ucHint.nIdx   = 0;
        ucHint.wParam = m_vecFind[pnc->iItem];

        //all views of DspLoggerDocment
		pDspLoggerDoc->UpdateAllViews( this, WM_GOTO_ITEM_BY_FIND,(CObject *)&ucHint);

		//all views of FindDocment
		m_pDoc->UpdateAllViews(this,WM_GOTO_ITEM_BY_FIND,(CObject *)&ucHint);

    }

	*pResult = 0;
}

void CFindListView::FindItems(BOOL bAutoReflash)
{
//    CWaitCursor waitCursor;

    if(m_pDoc->m_nFindDataNum <= 0 ) 
	{
	    GetListCtrl().SetItemCount(0);
		return;
	}

	if(bAutoReflash)
	{
		if((m_pSet->GetTpCount() <= m_uLastTPCount))
		{
			return;
		}
		m_uLastTPCount = m_pSet->GetTpCount();
	}
	else
	{
		m_vecColorIdx.clear();
	}
	if (m_dwStartPos == 0)
	{
		ClearFind();
	}

    ULONG lCount = m_pSet->Find(m_pDoc->m_arrFindData, m_pDoc->m_nFindDataNum, &m_dwStartPos, m_arrSigned);


 	ULONG uSize		= m_vecFind.size();
	ULONG uOffset	= 0;
	if (bAutoReflash && uSize + lCount > m_pDoc->m_dwDrawMaxPoints && lCount < m_pDoc->m_dwDrawMaxPoints)
	{
		std::vector<DWORD>	vecTempFindIdx;
		std::vector<DWORD>	vecTempColorIdx;
		m_vecFind.swap(vecTempFindIdx);
		m_vecColorIdx.swap(vecTempColorIdx);
		ClearFind();

		ULONG u = uSize + lCount - m_pDoc->m_dwDrawMaxPoints;
		for( ; u < uSize; ++u )
		{
			m_vecFind.push_back(vecTempFindIdx[u]);
			m_vecColorIdx.push_back(vecTempColorIdx[u]);
		}
	}
	else if( lCount >= m_pDoc->m_dwDrawMaxPoints)
	{
		ClearFind();
		uOffset = lCount - m_pDoc->m_dwDrawMaxPoints;
	}

    for( ULONG i = uOffset; i < lCount; ++i )
    {   
        m_vecFind.push_back(m_pSet->GetIndexByFindIdx(i));
		m_vecColorIdx.push_back(m_pSet->GetColorIndexByFindIdx(i));
    }
	
    m_pSet->ClearFind();
	m_dwItemCount = m_vecFind.size();
	
   // m_dwItemCount +=lCount;
	
    GetListCtrl().SetItemCount(m_dwItemCount);
    GetListCtrl().EnsureVisible(m_dwItemCount-1,FALSE);
}
