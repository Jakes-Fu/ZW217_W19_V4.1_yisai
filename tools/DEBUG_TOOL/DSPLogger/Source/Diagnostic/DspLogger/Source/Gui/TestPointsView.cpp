/*============================================================================*
 *
 *                          Copyright (c) 2002 Spreadtrum
 *                              All Rights Reserved
 *
 *============================================================================*/


/*============================================================================*
 *                          Edit History
 *-----------------------------------------------------------------------------
 * 2002-11-13   Kai Liu     Created
 *============================================================================*/
#include "stdafx.h"
#include "dsplogger.h"
#include "TestPointsView.h"
#include "mainfrm.h"

#include "FindTpDlg.h"
#include "dsploggerdoc.h"
#include "FindDoc.h"
#include <sys/timeb.h>


#include "GotoItemDlg.h"

#include "MarkDlg.h"
#include "EdtBtnCtrl.h"

/*@ Liu Kai CR4895 2003-9-23 */
#include "TackleDlg.h"

#define ADDR_INDEX 1
#define DATA_INDEX 2
#define DESC_INDEX 3
#define TIME_INDEX 4
#define PC_TIME_INDEX 5
#define MARK_INDEX 6

#define EVT_REFRESH       1
#define CLR_NORMAL        RGB(0,0,0)
#define CLR_BAD_ADDR      RGB(255,0,0)
#define CLR_FLAG		  RGB(255,0,255)

#define WM_REFRESH_LST (WM_USER+239)
#define ID_EVENT_AUTO_FINDUP	2000
#define ID_EVENT_AUTO_FINDDOWN	2001


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SEL_EDT_ID 195
extern CFindDoc *g_pFindDoc;
/////////////////////////////////////////////////////////////////////////////
// CTestPointsView

IMPLEMENT_DYNCREATE(CTestPointsView, CMassListView)

CTestPointsView::CTestPointsView()
{
	//{{AFX_DATA_INIT(CTestPointsView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    ( (CMainFrame *)AfxGetMainWnd() )->m_pView = this;

    m_bAutoScroll = TRUE;
    m_bShowAll    = TRUE;
    m_nItemCount  = 0;
    m_lBaseIdx    = 0;
    m_bUseFilter  = FALSE;

    m_pTPSet = NULL;
	m_dwStartTime = 0;
	m_bHasPCTime = FALSE;

	m_nFindUpTimer		= 0;
	m_nFindDownTimer	= 0;
}

CTestPointsView::~CTestPointsView()
{
    
}//lint !e1540

BEGIN_MESSAGE_MAP(CTestPointsView, CMassListView)
	//{{AFX_MSG_MAP(CTestPointsView)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_AUTO_SCROLL, OnAutoScroll)
	ON_UPDATE_COMMAND_UI(ID_AUTO_SCROLL, OnUpdateAutoScroll)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	ON_COMMAND(ID_EDIT_ADDR_DESC, OnEditAddrDesc)
	ON_COMMAND(ID_TP_ADDR_CHART, OnTpAddrChart)
	ON_COMMAND(ID_GOTO_ITEM, OnGotoItem)
	ON_COMMAND(ID_MARK_ITEM, OnMarkItem)
	ON_COMMAND(ID_UNMARK_ITEM, OnUnmarkItem)
	ON_COMMAND(ID_GOTO_MARK, OnGotoMark)
	ON_COMMAND(ID_MARK_UNMARK_ITEM, OnMarkUnmarkItem)
	ON_COMMAND(ID_NEXT_MARK, OnNextMark)
	ON_COMMAND(ID_PRE_MARK, OnPreMark)
	ON_COMMAND(ID_REMOVE_ALL_MARKS, OnRemoveAllMarks)
	ON_COMMAND(ID_SHOW_ALL, OnShowAll)
	ON_UPDATE_COMMAND_UI(ID_TP_ADDR_CHART, OnUpdateTpAddrChart)
	ON_COMMAND(ID_FILTER, OnFilter)
	ON_UPDATE_COMMAND_UI(ID_FILTER, OnUpdateFilter)
	ON_COMMAND(ID_TP_FIND, OnTpFind)
	ON_COMMAND(ID_TP_FIND_DOWN, OnTpFindDown)
	ON_COMMAND(ID_TP_FIND_UP, OnTpFindUp)
	ON_UPDATE_COMMAND_UI(ID_FIND_TP, OnUpdateFindTp)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeleditLstProductInfo)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI_RANGE(ID_TP_FIND,ID_GOTO_MARK,OnUpdateUIRange)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
    ON_MESSAGE(WM_REFRESH_LST, OnRefreshLst)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestPointsView diagnostics

#ifdef _DEBUG
void CTestPointsView::AssertValid() const
{
	CListView::AssertValid();
}

void CTestPointsView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

LRESULT CTestPointsView::OnRefreshLst(WPARAM, LPARAM)
{
    GetListCtrl().Invalidate(FALSE);

    return 0L;
}
/////////////////////////////////////////////////////////////////////////////
// CTestPointsView message handlers

void CTestPointsView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();

    m_pFrame = (CMainFrame*)AfxGetMainWnd();
    m_pList = &GetListCtrl();
    m_pDoc = (CDspLoggerDoc*)GetDocument();
    m_pTPSet = m_pFrame->m_pDspKernel->GetTpSet();

    DWORD dwStyle = m_pList->GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
    m_pList->SetExtendedStyle(dwStyle);
    CFont *pFont = ( (CDspLoggerApp *)AfxGetApp() )->GetAppFont();
	m_pList->SetFont(pFont);

	m_pList->InsertColumn(0,"NO",LVCFMT_LEFT,100);	
	m_pList->InsertColumn(ADDR_INDEX,"ADDRESS",LVCFMT_LEFT,100);
	m_pList->InsertColumn(DATA_INDEX,"DATA",LVCFMT_LEFT,100);
    m_pList->InsertColumn(DESC_INDEX,"DESCRIPTION",LVCFMT_LEFT,150);
    m_pList->InsertColumn(TIME_INDEX,"TIME",LVCFMT_LEFT,150);
	//teana hu 2009.11.11
	m_pList->InsertColumn(PC_TIME_INDEX,"PC TIME",LVCFMT_LEFT,150);
	//
    m_pList->InsertColumn(MARK_INDEX,"MARK",LVCFMT_LEFT,200);

    m_dlgDesc.Create(IDD_DLG_TP_ADDR_DESC,this);
	m_dlgFind.Create(IDD_DIALOG_FIND_TESTPOINT,this);
    m_dlgFind.m_pView = this;

}

void CTestPointsView::RefreshList(ULONG nCount)
{
    CListCtrl &lst = GetListCtrl();

//    ULONG nCount = m_pFrame->m_pDspKernel->GetTpSet()->GetTpCount();
	if(nCount == 0)
	{
		nCount = m_pFrame->m_pDspKernel->GetTpSet()->GetTpCount();
		m_lBaseIdx = 0; 
	}
  
    if( nCount == lst.GetItemCount())
    {
        return;
    }

    lst.SetItemCountEx(nCount, LVSICF_NOINVALIDATEALL | LVSICF_NOSCROLL);
    if(m_bAutoScroll)
    {		  
		lst.EnsureVisible( nCount - 1, FALSE );
	}

}

void CTestPointsView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
    CMenu menu;
    menu.LoadMenu( IDR_LIST_POP );
    CMenu* pSubMenu = menu.GetSubMenu( 0 );

    // Initialize autoscroll menu items
    UINT uCheck;
    if(m_bAutoScroll)
        uCheck = MF_CHECKED;
    else
        uCheck = MF_UNCHECKED;
    pSubMenu->CheckMenuItem(ID_AUTO_SCROLL,uCheck);

    uCheck = m_bShowAll ? MF_CHECKED : MF_UNCHECKED;
    pSubMenu->CheckMenuItem(ID_SHOW_ALL, uCheck);

    CListCtrl &lst = GetListCtrl();
    ULONG nCount = lst.GetItemCount();
    if(nCount == 0)
    {
        // No items in list,hide "Goto item"
        // menu item
        pSubMenu->EnableMenuItem(ID_GOTO_ITEM,MF_BYCOMMAND | MF_GRAYED);
    }

    // Initialize mark menu items
    POSITION pos = m_pList->GetFirstSelectedItemPosition();
    int nIndex = m_pList->GetNextSelectedItem(pos);
    int nMarkIndex,nEleIndex;
    CString strMark;
    if(nCount == 0||m_bUseFilter)
    {
        // No items in list now,hide all mark menu items
        pSubMenu->EnableMenuItem(ID_MARK_ITEM,MF_BYCOMMAND | MF_GRAYED);
        pSubMenu->EnableMenuItem(ID_UNMARK_ITEM,MF_BYCOMMAND | MF_GRAYED);
        pSubMenu->EnableMenuItem(ID_GOTO_MARK,MF_BYCOMMAND | MF_GRAYED);
        pSubMenu->EnableMenuItem(ID_GOTO_ITEM,MF_BYCOMMAND | MF_GRAYED);
    }
    else if(m_MarkManage.FindElement(nIndex,nMarkIndex,nEleIndex,strMark))
    {
        // This item have been marked before
        // Hide "Mark item" menu item
        pSubMenu->EnableMenuItem(ID_MARK_ITEM,MF_BYCOMMAND | MF_GRAYED);
    }
    else
    {
        // This item have not been marked before,
        // hide "Unmark item" menu item
        pSubMenu->EnableMenuItem(ID_UNMARK_ITEM,MF_BYCOMMAND | MF_GRAYED);
    }
    
    pSubMenu->TrackPopupMenu( 
        TPM_LEFTALIGN |TPM_RIGHTBUTTON,
        point.x, 
        point.y,
        pWnd
        );
}

void CTestPointsView::OnAutoScroll() 
{
    m_bAutoScroll = !m_bAutoScroll;	
}

void CTestPointsView::OnUpdateAutoScroll(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bAutoScroll );	
}

void CTestPointsView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);
    
    CListCtrl& wndTestPtList = GetListCtrl();

    m_nItemCount = wndTestPtList.GetCountPerPage();
}

// Return the index of found item,-1 means not found one
LONGLONG CTestPointsView::FindTp(CString& strAddr,CString& strData,BOOL bUp,CString& strDesc)
{
    CWaitCursor wc;
	
	LONGLONG nStart;
    POSITION pos	= GetListCtrl().GetFirstSelectedItemPosition();
	if (NULL== pos) 
	{
		nStart=0;
	}
	else
	{
		nStart = GetListCtrl().GetNextSelectedItem( pos );
		
		bUp ? nStart-- : nStart++;		
	}

    LONGLONG llFind = m_pTPSet->Find(nStart,strAddr.GetBuffer(0),strData.GetBuffer(0),bUp,strDesc.operator LPCTSTR());
    if(llFind == NOT_FIND_TP)
    {
        ShowNotFound();
    }
    else
    {
        ShowFound(llFind);
    }

    return llFind;
}

void CTestPointsView::Clear()
{
    GetListCtrl().SetItemCount(0);
    
	//GetTPSet()->Clear();

	m_pFrame->m_pDspKernel->Clear();
    m_vecFindIdx.clear();
    m_MarkManage.RemoveAllMarkElements();
	m_bHasPCTime = FALSE;
	m_dwStartTime = 0;
}

void CTestPointsView::OnDestroy() 
{
	CListView::OnDestroy();

    m_dlgFind.DestroyWindow();
    m_dlgDesc.DestroyWindow();
}

BOOL CTestPointsView::PreCreateWindow(CREATESTRUCT& cs) 
{
    cs.style |= LVS_REPORT | LVS_SINGLESEL | LVS_OWNERDATA | LVS_SHOWSELALWAYS;
    
    return CListView::PreCreateWindow(cs);
}

void CTestPointsView::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
    LV_ITEM* pItem = &(pDispInfo->item);

    ULONG ullIndex = 0L;

    if( m_bUseFilter )
    {
        ullIndex = m_vecFindIdx[pItem->iItem + m_lBaseIdx];
    }
    else
    {
        ullIndex = pItem->iItem + m_lBaseIdx;
    }
	
    if(ullIndex > m_pTPSet->GetTpCount())
    {
        return;
    }

	WORD wAddr = 0;
	wAddr = m_pTPSet->GetTPAddr(ullIndex); 
	WORD wData = 0;
	wData = m_pTPSet->GetTPData(ullIndex);
	DWORD dwTime = 0;
	dwTime = m_pTPSet->GetTPTime(ullIndex);

	//TRACE("item index : %d",ullIndex);
	
    if(pItem->mask & LVIF_TEXT)
    {
        // Text needed
        switch(pItem->iSubItem)
        {
            case 0:
            {
                // Item label text needed
                sprintf(pItem->pszText,"%d",ullIndex);
            }
            break;

            case ADDR_INDEX:
            {
                sprintf(pItem->pszText,"%04X",wAddr);
                pItem->lParam = m_dlgDesc.LookupColor(wAddr, wData);
            }
            break;

            case DATA_INDEX:
            {
                sprintf(pItem->pszText,"%04X",wData);
            }
            break;

            case DESC_INDEX:
            {  
                m_dlgDesc.LookupDesc(wAddr,wData,pItem->pszText);
				//teana hu 2009.11.11
				if(wAddr == 0 && wData == 0 && dwTime >= 0)
				{
					pItem->pszText = "Start Logging";
				}
				else if(wAddr == 0 && wData == 1 && dwTime > 0)
				{
					pItem->pszText = "Stop Logging";
				}
            }
            break;

            case TIME_INDEX:
            {
				if(m_pFrame->m_dwGenerateTPTime == D0A0_TIME)
				{
					sprintf(pItem->pszText, "%02d:%02d:%02d:%03d", (dwTime/(1000*60*60)),(dwTime/(1000*60))%60,(dwTime/1000)%60,dwTime%1000 );
				}
				else
				{
					sprintf(pItem->pszText,"%u",dwTime);  
				}                              
            }
            break;

			case PC_TIME_INDEX:
			{
				CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
				if(pFrm != NULL && pFrm->m_dwGenerateTPTime == TOOL_TIME)
				{
					DWORD dwHour = 0;
					if(m_pTPSet->GetTPAddr(0) == 0 && m_pTPSet->GetTPData(0) == 0)
					{
						m_bHasPCTime = TRUE;
						m_dwStartTime = m_pTPSet->GetTPTime(0);
					}
					char szText[128] = {0};
					if(wAddr == 0 && (wData == 0 || wData == 1))
					{
						m_bHasPCTime = TRUE;
					}
					else
					{
						dwTime = dwTime + m_dwStartTime;	
					}
					
					if(m_bHasPCTime)
					{
						dwHour = dwTime/(1000*60*60);
						if (pFrm->m_pDspKernel->Is24HourGap())
						{
							dwHour = dwHour>=24 ? dwHour-24*(dwHour/24):dwHour;
						}
						else
						{
							dwHour = dwHour>12 ? dwHour-12*(dwHour/12):dwHour;
						}
						
						sprintf(szText, "%02d:%02d:%02d:%03d", dwHour,(dwTime/(1000*60))%60,(dwTime/1000)%60,dwTime%1000 );
						pItem->pszText = szText;
					}
				}			
			}
			break;

            case MARK_INDEX:
            {
                int nMarkIndex,nEleIndex;
                CString strMark;
                if(m_MarkManage.FindElement(ullIndex,nMarkIndex,nEleIndex,strMark))
                {
                    // Have mark
                    strcpy(pItem->pszText,strMark);
                }
                else
                {
                    // Have no mark
                    pItem->pszText[0] = 0;
                }
            }
            break;

        }
    }
	
	*pResult = 0;
}

void CTestPointsView::OnEditAddrDesc() 
{
	m_dlgDesc.CenterWindow();
	m_dlgDesc.ShowWindow(SW_SHOW);
}

void CTestPointsView::OnTpAddrChart() 
{
	/* old chart */
	//CTPAddrSampleDlg* pDlg = new CTPAddrSampleDlg(this);
	
	//    POSITION pos = m_pList->GetFirstSelectedItemPosition();
	//    LONGLONG nStart = m_pList->GetNextSelectedItemEx(pos);
	//    pDlg->SetFrom(nStart);
    //pDlg->SetFrom(0);
    //pDlg->Create(IDD_DLG_ADDR_SAMPLE,GetDesktopWindow());
	
    //pDlg->ShowWindow(SW_SHOW);
	
}

void CTestPointsView::OnGotoItem() 
{
	CGotoItemDlg dlg;
    if(dlg.DoModal() == IDOK)
    {
        GotoItem(dlg.m_nIndex - 1);
    }
}

void CTestPointsView::GotoItem(int nIndex)
{
    // Check parameter first
    if(nIndex < 0)
    {
        return;
    }

    CListCtrl &lst = GetListCtrl();
    int nCount = lst.GetItemCount();

	CString strValue;
	strValue = lst.GetItemText(nCount - 1, 0);

	if(nIndex > atoi(strValue))
	{
		return;
	}

	strValue = lst.GetItemText(0, 0);

	nIndex = nIndex - atoi(strValue);

	//Disables drawing until all operator finished
	//@andybian 2006-9-1
	lst.LockWindowUpdate();
	lst.EnsureVisible(nCount-1, FALSE);
    //makesure select item show at fourth 
    lst.EnsureVisible(nIndex>20 ? nIndex-20 : nIndex, FALSE);
    lst.SetItemState(nIndex,LVIS_SELECTED,LVIS_SELECTED);
	lst.UnlockWindowUpdate();

}

void CTestPointsView::OnCustomDraw(NMHDR *pNMHDR, LRESULT *pResult)
{
    NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

    // Take the default processing unless we set this to something else below.
    *pResult = CDRF_DODEFAULT;

    // First thing - check the draw stage. If it's the control's prepaint
    // stage, then tell Windows we want messages for every item.

    switch(pLVCD->nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT:
        *pResult = CDRF_NOTIFYITEMDRAW;
        break;
    case CDDS_ITEMPREPAINT:
        {
			ULONG uIdx = 0;
			if( m_bUseFilter )
			{
				uIdx = m_vecFindIdx[pLVCD->nmcd.dwItemSpec+ m_lBaseIdx];
			}
			else
			{
				uIdx = pLVCD->nmcd.dwItemSpec + m_lBaseIdx;
			}
			
			WORD wAddr;
			WORD wData;
			DWORD dwTime = 0;
			ITPSet* pSet = m_pFrame->m_pDspKernel->GetTpSet();
			if(uIdx > pSet->GetTpCount())
			{
				wAddr = 0xff00;
				wData = 0;
			}
			else
			{
				wAddr = pSet->GetTPAddr(uIdx);
				wData = pSet->GetTPData(uIdx);
				dwTime = pSet->GetTPTime(uIdx);
			}
			
#ifdef  DSPLOGGER_AUDIO
			
			pLVCD->clrText = m_dlgDesc.LookupColor(wAddr, wData);//CLR_NORMAL;
			
#else
			if(wAddr >= 0xff00)
			{
				// Not a valid address,use another color
				pLVCD->clrText = CLR_BAD_ADDR;
			}
			else if(wAddr == 0x0 && (wData == 0 || wData == 1) && dwTime >= 0)
			{
				pLVCD->clrText = CLR_FLAG;
			}
			else
			{
				pLVCD->clrText = m_dlgDesc.LookupColor(wAddr, wData);//CLR_NORMAL;        
			}
			
#endif	
        }
		break;
    case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
        if( pLVCD->iSubItem == 3 )
        {
		    pLVCD->clrTextBk = pLVCD->nmcd.lItemlParam;//RGB(0,200,10);
        }
		*pResult = CDRF_DODEFAULT;
		break;
    default:
	    *pResult = 0;
    }
}

void CTestPointsView::OnMarkItem() 
{
    POSITION pos = m_pList->GetFirstSelectedItemPosition();
    ULONG nIndex = m_pList->GetNextSelectedItem(pos);
    if(nIndex == -1)
    {
        // Have not selected item
        return;
    }

	CMarkDlg dlg(&m_MarkManage);
    if(IDOK != dlg.DoModal())
    {
        return;
    }

    CString strMark = dlg.GetMarkString();
    if(strMark.GetLength() == 0)
    {
        // Use default
        strMark = DEFAULT_MARK_STRING;
    }

    m_MarkManage.AddElement(nIndex,strMark);

    m_pList->RedrawItems(nIndex,nIndex);
}

void CTestPointsView::OnUnmarkItem() 
{
    POSITION pos = m_pList->GetFirstSelectedItemPosition();
    ULONG nIndex = m_pList->GetNextSelectedItem(pos);
    if(nIndex == NOT_FIND_TP)
    {
        // Have not selected item
        return;
    }

    int nMarkIndex,nEleIndex;
    CString strMark;
    if(m_MarkManage.FindElement(nIndex,nMarkIndex,nEleIndex,strMark))
    {
        m_MarkManage.RemoveElement(nIndex,nMarkIndex);
    }

    m_pList->RedrawItems(nIndex,nIndex);
}

void CTestPointsView::OnGotoMark() 
{
    CMarkDlg dlg(&m_MarkManage,FALSE);
    if(IDOK != dlg.DoModal())
    {
        return;
    }

    CString strMark = dlg.GetMarkString();
    if(strMark.IsEmpty())
    {
        return;
    }

    m_MarkManage.ChangeMark(strMark);

    ULONG nIndex = m_MarkManage.GetElement();
    if(nIndex == -1)
    {
        return;
    }
    GotoItem(nIndex);
}

void CTestPointsView::OnMarkUnmarkItem() 
{
    POSITION pos = m_pList->GetFirstSelectedItemPosition();
    ULONG nIndex = m_pList->GetNextSelectedItem(pos);
    if(nIndex == -1)
    {
        // Have not selected item
        return;
    }

    int nMarkIndex,nEleIndex;
    CString strMark;
    if(m_MarkManage.FindElement(nIndex,nMarkIndex,nEleIndex,strMark))
    {
        OnUnmarkItem();
    }
    else
    {
        OnMarkItem();
    }
}

void CTestPointsView::OnNextMark() 
{
	//@andy.bian 2006-8-29  
    POSITION pos = m_pList->GetFirstSelectedItemPosition();
    ULONG nIndex = m_pList->GetNextSelectedItem(pos);
    if(nIndex == -1)
    {
        // Have not selected item
        return;
    }
	
    int nMarkIndex,nEleIndex;
    CString strMark;
    if(m_MarkManage.FindElement(nIndex,nMarkIndex,nEleIndex,strMark))
	{
		m_MarkManage.ChangeCurMark(nMarkIndex,nEleIndex);
		GotoItem(m_MarkManage.Next(TRUE));
	}
	else
	{
		//go to default current mark
		GotoItem(m_MarkManage.Next(TRUE));
	}
}

void CTestPointsView::OnPreMark() 
{
	//@andy.bian 2006-8-29  
    POSITION pos = m_pList->GetFirstSelectedItemPosition();
    ULONG nIndex = m_pList->GetNextSelectedItem(pos);
    if(nIndex == -1)
    {        
		// Have not selected item
        return;
    }

    int nMarkIndex,nEleIndex;
    CString strMark;
    if(m_MarkManage.FindElement(nIndex,nMarkIndex,nEleIndex,strMark))
	{
		m_MarkManage.ChangeCurMark(nMarkIndex,nEleIndex);
		GotoItem(m_MarkManage.Previous(TRUE));
	}
	else
	{
		//go to default current mark
		GotoItem(m_MarkManage.Previous(TRUE));
	}

}

void CTestPointsView::OnRemoveAllMarks() 
{
	m_MarkManage.RemoveAllMarkElements();
	GetListCtrl().Invalidate();

}


/*@ Liu Kai CR4895 2003-9-23 */
inline
void CTestPointsView::CheckFilter(DWORD dwAddr,DWORD dwData,long lIndex)
{
    char szFilter[100];

    if(m_dlgDesc.FindFilter(dwAddr, dwData, szFilter))
    {
        CTackleDlg* pDlg = new CTackleDlg(this);
		if (pDlg)
		{
			pDlg->m_strFilter = szFilter;
			pDlg->m_lIndex = lIndex;
			pDlg->Create(IDD_DIALOG_TACKLE,this);
			pDlg->ShowWindow(SW_SHOW);
		}
        
    }
}

void CTestPointsView::OnShowAll() 
{
    if( m_bUseFilter )
    {
	    ULONG uCount = m_pTPSet->GetTpCount();

        CListCtrl & lst = GetListCtrl();

        m_vecFindIdx.clear();
        m_bUseFilter = FALSE;
        lst.SetItemCount(uCount);
    }

    m_bShowAll = TRUE;
}

void CTestPointsView::OnUpdateTpAddrChart(CCmdUI* pCmdUI) 
{
    CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
    pCmdUI->Enable( pFrm->m_bStop );
}

void CTestPointsView::OnFilter() 
{
	if( m_bShowAll )
    {
        CFindTpDlg dlg;
        
        dlg.SetFindData( m_pDoc->m_arrFindData, m_pDoc->m_nFindDataNum, MAX_FIND_DATA_NUM );
        if( dlg.DoModal() == IDOK )
        {
            CWaitCursor waitCursor;
            DWORD  dwStartPos =0;
            m_vecFindIdx.clear();
                       
            m_pDoc->m_nFindDataNum = dlg.m_nFindNum;
			//teana hu 2011.06.01
			BOOL *parrSinged = new BOOL[m_pDoc->m_nFindDataNum];
			if (parrSinged)
			{
				memset(parrSinged, 0, sizeof(BOOL) * m_pDoc->m_nFindDataNum);
				//
				ULONG uCount = m_pTPSet->Find(m_pDoc->m_arrFindData, m_pDoc->m_nFindDataNum,&dwStartPos, parrSinged );
				//teana hu 2011.06.01
				if(parrSinged != NULL)
				{
					delete [] parrSinged;
					parrSinged = NULL;
				}
				//
				for( ULONG i = 0; i < uCount; ++i )
				{
					m_vecFindIdx.push_back(m_pTPSet->GetIndexByFindIdx(i));
				}
				m_pTPSet->ClearFind();
				
				CListCtrl &lst = GetListCtrl();
				
				m_bShowAll   = FALSE;
				m_bUseFilter = TRUE;
				lst.SetItemCount(uCount);
				m_bShowAll   = FALSE;
			}
			
        }
    }
    else
    {
        m_bShowAll = TRUE;
		OnShowAll();
    }

}

void CTestPointsView::OnUpdateFilter(CCmdUI* pCmdUI) 
{
    CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
    pCmdUI->Enable( pFrm->m_bStop );
	pCmdUI->SetCheck(!m_bShowAll);
	
}

void CTestPointsView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	if( pSender != NULL && lHint == WM_GOTO_ITEM_BY_FIND && !m_bUseFilter )
    {
        PUC_HINT pUcHint = (PUC_HINT)pHint;

        GotoItem( pUcHint->wParam );

        SetFocus();
    }	

    if( pSender != NULL && lHint == WM_GOTO_ITEM_BY_CHART&& !m_bUseFilter )
    {
        PUC_HINT pUcHint = (PUC_HINT)pHint;

        GotoItem( pUcHint->lParam );

        SetFocus();
    }

}
void CTestPointsView::OnTpFind() 
{
    m_dlgFind.ShowWindow(SW_SHOW);
	
	m_dlgFind.m_cmbAddr.SetFocus();
	
}

void CTestPointsView::OnTpFindDown() 
{	
	//find test point down 
	m_dlgFind.m_bDire=FALSE;
	m_dlgFind.OnOK();
	KillAutoFindTimer();
	
}

void CTestPointsView::OnTpFindUp() 
{
	//find test point up
	m_dlgFind.m_bDire=TRUE;
	m_dlgFind.OnOK();
	KillAutoFindTimer();
}

//set submenu(Mind and Mark submenu) status in filter mode 
//@andybian 2006-8-30
void CTestPointsView::OnUpdateUIRange(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_bUseFilter);

}


void CTestPointsView::OnUpdateFindTp(CCmdUI* pCmdUI) 
{
	bool bFull=true;
	for(int i=0; i<MAX_CHART_WND_NUM; i++)
	{
		if( g_theApp.m_pFindWnd[i] == NULL )
		{
			bFull=false;
			break;
		}
	}
	pCmdUI->Enable(!bFull);

}

void CTestPointsView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMITEMACTIVATE pnia = (LPNMITEMACTIVATE)pNMHDR;

	CEdtBtnCtrl *pEdt = NULL;
	//CEdit *pEdt = NULL;
	int nItem = pnia->iItem;
	int nSubItem = pnia->iSubItem;
	
	//teana hu 2009.10.16
	if(nSubItem == MARK_INDEX)
	{
		OnMarkItem() ;
		return;
	}
	else if(nSubItem == DESC_INDEX)
	{
		pEdt = new CEdtBtnCtrl;	
		CListCtrl &lstCtrl = GetListCtrl();

		if(NULL != pEdt)
		{
			pEdt->m_nItem = nItem;
			pEdt->m_nSubItem = nSubItem;
			pEdt->m_strText = lstCtrl.GetItemText(nItem,nSubItem);
			if(!pEdt->Create(IDD_EDT_BTN_DLG,FromHandle(lstCtrl.m_hWnd)))
			{
				AfxMessageBox(_T("Error to create EdtBtnCtrl"));
				return;
			}
		}
		else
		{
			return;
		}
		
		CRect rect;
		lstCtrl.GetSubItemRect(nItem,nSubItem,LVIR_BOUNDS,rect);
		CRect rtList;
		lstCtrl.GetClientRect(&rtList);
		if(rect.right>rtList.right)
			rect.right = rtList.right;
		if(rect.left < rtList.left)
			rect.left = rtList.left;
		if(rect.bottom>rtList.bottom)
			rect.bottom = rtList.bottom;
		if(rect.top < rtList.top)
			rect.top = rtList.top;
		
		pEdt->SetWindowPos(&wndTop,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top-1,NULL);

		pEdt->EnableWindow(FALSE);
		
		pEdt->ShowWindow(SW_SHOW);
		
		*pResult = 0;
	}
	else	//teana hu 2009.10.16
	{
		static UC_HINT ucHint;
        ucHint.nIdx   = 0;
        ucHint.wParam = nItem;

        //all views of DspLoggerDocment
		//teana hu 2009.12.23 updated for all chart frame
		for(int i=0; i<MAX_CHART_WND_NUM; i++)
		{
			if( g_theApp.m_pFindWnd[i] != NULL )
			{
				CFindDoc *pDoc = (CFindDoc *)(g_theApp.m_pFindWnd[i]->GetActiveDocument());
				if(pDoc != NULL)
				{
					pDoc->UpdateAllViews( this, WM_FIND_ITEM_IN_DOCVIEW,(CObject *)&ucHint);
				}
			}
		}
		
		//teana hu 2009.12.21
		if(g_theApp.m_pKeyInfoDoc != NULL)
		{
			g_theApp.m_pKeyInfoDoc->UpdateAllViews( this, WM_FIND_ITEM_IN_DOCVIEW,(CObject *)&ucHint);
		}

		//related with armlogel
		if(m_pFrame->m_dwGenerateTPTime == D0A0_TIME)
		{
			//teana hu 2012.04.09
			ULONG nTick = m_pFrame->m_pDspKernel->GetTpSet()->GetTPTime(nItem);
			Logel_RelatedLog(nTick);
			//
		}
		else
		{
			CListCtrl &lst = GetListCtrl();
			CString strPCTime = lst.GetItemText(nItem, PC_TIME_INDEX);
			if(!strPCTime.IsEmpty())
			{
				LPSTR lpszTime = (LPSTR)(LPCTSTR)strPCTime;
				ULONG nHour,nMinute,nSecond,nMilSecond ;
				sscanf( lpszTime, "%d:%02d:%02d:%03d", &nHour, &nMinute, &nSecond, &nMilSecond );
				ULONG nTick = 1000*60*60 * nHour + 1000 * 60 * nMinute + 1000 * nSecond + nMilSecond;
				Logel_RelatedLog(nTick);
			}
		}
	}
}

void CTestPointsView::OnEndlabeleditLstProductInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
//	CListCtrl &lstCtrl = GetListCtrl();
//	lstCtrl.SetItemText(pDispInfo->item.iItem,pDispInfo->item.iSubItem,pDispInfo->item.pszText);
//	lstCtrl.SetItemText(0, 3, "HAHAHAHHA");

	 m_pList->RedrawItems(pDispInfo->item.iItem,pDispInfo->item.iSubItem);
	*pResult = 0;
}

LPCTSTR CTestPointsView::GetItemMark(int nMarkIndex, int& nItem)
{
	CString strMark = "";
	m_MarkManage.GetMarkStringEx(nMarkIndex, nItem, strMark);
	return strMark;
}

ULONG CTestPointsView::GetMarkCount()
{
	return m_MarkManage.GetAllElementCount();
}

void CTestPointsView::AddMarkItem(int nItem, LPSTR lpMark)
{
	CString strMark = lpMark;
	m_MarkManage.AddElement(nItem, strMark);
}

BOOL CALLBACK CTestPointsView::GetFindLogelWndCallBack(HWND hwnd,LPARAM lParam)
{
	char szTitle[MAX_PATH];
    ::GetWindowText(hwnd,szTitle,MAX_PATH);

    if(strstr(szTitle,"Spreadtrum Logel"))
    {
		COPYDATASTRUCT cs;
        cs.dwData = WM_FIND_RELATED_ITEM;
		char szTmp[50] = {0};
		sprintf(szTmp, "%d", (ULONG)lParam);
		cs.cbData = strlen(szTmp) + 1;
        cs.lpData = szTmp;
		//teana hu 2012.08.20
        ::PostMessage(hwnd, WM_COPYDATA, NULL, LPARAM(&cs));
		//
		
        return FALSE;
    }

    return TRUE;
}

void CTestPointsView::Logel_RelatedLog(ULONG ulPCTime)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
    ::EnumDesktopWindows(NULL,GetFindLogelWndCallBack,LPARAM(ulPCTime));

}

void CTestPointsView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (
		(ID_EVENT_AUTO_FINDUP == nIDEvent)		||
		(ID_EVENT_AUTO_FINDDOWN == nIDEvent)
		)
	{
		m_dlgFind.OnOK();		
	}
//	CMassListView::OnTimer(nIDEvent);
}


void CTestPointsView::SetAutoFindTimer(BOOL bFindUp /*=TRUE*/)
{
	if (bFindUp)  //FindUp
	{
		if (m_nFindUpTimer)
		{
			KillTimer(m_nFindUpTimer);
			m_nFindUpTimer = 0;
		}
		m_dlgFind.m_bDire=TRUE;
		m_nFindUpTimer = SetTimer(ID_EVENT_AUTO_FINDUP, 200, NULL);
	}
	else		//FindDown
	{
		if (m_nFindDownTimer)
		{
			KillTimer(m_nFindDownTimer);
			m_nFindDownTimer = 0;
		}
		m_dlgFind.m_bDire=FALSE;
		m_nFindDownTimer = SetTimer(ID_EVENT_AUTO_FINDDOWN, 200, NULL);
	}
}

void CTestPointsView::KillAutoFindTimer()
{
	if (m_nFindUpTimer)
	{
		KillTimer(m_nFindUpTimer);
		m_nFindUpTimer = 0;
	}
	if (m_nFindDownTimer)
	{
		KillTimer(m_nFindDownTimer);
		m_nFindDownTimer = 0;
	}
}
