// InfoListView.cpp : implementation file
//

#include "stdafx.h"
#include "..\MobileCnnt.h"
#include "..\MainFrm.h"
#include "InfoListView.h"
#include "..\MobileCnntDoc.h"
#include "..\SMSDoc.h"
#include "MsgIDDef.h"
#include "InfoWrapper.h"
#include "..\effect\ChildFrameWnd.h"
#include "PBDlg.h"
#include "IMEMO.h"

#ifndef __GLOBALDEF_H__
#include "GlobalDef.h"
#endif // __GLOBALDEF_H__

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int CInfoListView::MAX_CONTENT_LEN  = 24;
/////////////////////////////////////////////////////////////////////////////
// CInfoListView

IMPLEMENT_DYNCREATE(CInfoListView, CListView)

CInfoListView::CInfoListView()
{
	m_nNoNameID  = 1;
	m_nFocusItem = -1;
	m_dwCategID  = 0xFF;

	m_bDragging = FALSE;
	m_nDragIndex = -1;
	m_nDropIndex = -1;
	m_pDragImage = NULL;

	m_bFromMenu = FALSE;
    m_pProgThread = NULL;
	
	m_bSaveToFile = FALSE;
	
	m_pIW = NULL;
	m_pMemo = NULL;
	m_dwDragCategID = 0;
	m_hDropItem = NULL;
	m_pDropWnd = NULL;
	m_pTreeView = NULL;
	m_bFocusItemLock = FALSE;
}//lint !e1401

CInfoListView::~CInfoListView()
{
	if( m_CoolMenu.m_hMenu != NULL )
		m_CoolMenu.DestroyMenu();
	
	if(m_pDragImage)
		delete m_pDragImage;
}

BEGIN_MESSAGE_MAP(CInfoListView, CListView)
	//{{AFX_MSG_MAP(CInfoListView)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_ILM_SMS_ANSWER, OnIlmSmsAnswer)
	ON_COMMAND(ID_ILM_SMS_FORWARD, OnIlmSmsForward)
	ON_COMMAND(ID_ILM_SMS_SEND, OnIlmSmsSend)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	ON_COMMAND(ID_EXPORT_TO_VCARD, OnExportToVcard)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_TO_VCARD, OnUpdateExportToVcard)
	ON_COMMAND(ID_SYNC_PB, OnSyncPb)
	ON_COMMAND(ID_EXPORT_TO_VCALENDAR, OnExportToVcalendar)
	ON_COMMAND(ID_IMPORT_VCALENDAR, OnImportVcalendar)
	ON_COMMAND(ID_IMPORT_VCARD, OnImportVcard)
	ON_COMMAND(ID_EXTRACT_NUMBER, OnExtractNumber)
	ON_COMMAND(ID_SYNC_MEMO, OnSyncMemo)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_COPY_MOVE_TO_PC, OnCopyMoveToPc)
	ON_UPDATE_COMMAND_UI(ID_COPY_MOVE_TO_PC, OnUpdateCopyMoveToPc)
	ON_COMMAND(ID_COPY_MOVE_TO_MS, OnCopyMoveToMs)
	ON_UPDATE_COMMAND_UI(ID_COPY_MOVE_TO_MS, OnUpdateCopyMoveToMs)
	ON_COMMAND(ID_COPY_MOVE_TO_SIM, OnCopyMoveToSim)
	ON_UPDATE_COMMAND_UI(ID_COPY_MOVE_TO_SIM, OnUpdateCopyMoveToSim)
	ON_COMMAND(ID_COPY_MOVE_TO_SIM2, OnCopyMoveToSim2)
	ON_UPDATE_COMMAND_UI(ID_COPY_MOVE_TO_SIM2, OnUpdateCopyMoveToSim2)
	ON_COMMAND(ID_COPY_MOVE_TO_SIM3, OnCopyMoveToSim3)
	ON_UPDATE_COMMAND_UI(ID_COPY_MOVE_TO_SIM3, OnUpdateCopyMoveToSim3)
	ON_COMMAND(ID_COPY_MOVE_TO_SIM4, OnCopyMoveToSim4)
	ON_UPDATE_COMMAND_UI(ID_COPY_MOVE_TO_SIM4, OnUpdateCopyMoveToSim4)
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(ID_CALL_FROM_PB, OnOptionCallFromPB)
	ON_COMMAND(ID_SEND_SMS_FROM_PB, OnOptionSendSMSFromPB)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_GROUP_ONE,ID_GROUP_TWENTY,OnGroupChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoListView drawing

void CInfoListView::OnDraw(CDC* /*pDC*/)
{
//	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CInfoListView diagnostics

#ifdef _DEBUG
void CInfoListView::AssertValid() const
{
	CListView::AssertValid();
}

void CInfoListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CSMSDoc* CInfoListView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSMSDoc)));
	return (CSMSDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInfoListView message handlers

BOOL CInfoListView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= LVS_REPORT;
	cs.style |= LVS_ALIGNTOP;
	cs.style |= LVS_SHOWSELALWAYS;
	cs.style &= ~LVS_NOSORTHEADER;

	return CListView::PreCreateWindow(cs);
}

void CInfoListView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	
	CListCtrl &lst = GetListCtrl();//lint !e1514

	DWORD dwExStyle = lst.GetExtendedStyle();
	dwExStyle |= LVS_EX_GRIDLINES;
	dwExStyle |= LVS_EX_FULLROWSELECT;

	lst.SetExtendedStyle(dwExStyle);

	CHeaderCtrl * pHeaderCtrl = lst.GetHeaderCtrl();
	pHeaderCtrl->ModifyStyle(HDS_BUTTONS, HDS_HOTTRACK);

	m_pIW = theApp.GetInfoWrapper();
	_ASSERTE( m_pIW != NULL );

}

void CInfoListView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pSender != this && lHint == WM_CATEGORY_CHANGE )
	{
		//if( (DWORD)pHint == m_dwCategID )
		//	return;

		m_dwCategID = (DWORD)pHint;
		if(LOWORD(m_dwCategID) == 0x00)
			m_dwCategID = m_dwCategID | 0x00FF;
		UpdateView( m_dwCategID );
	}
}

void CInfoListView::UpdateView(DWORD dwFlag)
{
	CChildFrameWnd * pParentWnd = (CChildFrameWnd *)GetParent();
	_ASSERTE( pParentWnd != NULL );
	ASSERT_KINDOF( CChildFrameWnd, pParentWnd ); // this is necessary for debug

    UpdateColumn(dwFlag);
	switch( HIWORD(dwFlag) )
	{
	case ID_CATEG_SMS:
		UpdateSMS(LOWORD(dwFlag));
		break;
	case ID_CATEG_PB:
		UpdatePB(LOWORD(dwFlag));
		break;
	case ID_CATEG_MEMO:
		UpdateMemo(LOWORD(dwFlag));
		break;
	case ID_CATEG_PBG:
		UpdatePBG(LOWORD(dwFlag));
		break;
	case ID_CATEG_PBGM:
		UpdatePBGM(LOWORD(dwFlag));
		break;
	default:
		_ASSERTE( 0 );
		break;
	}

	CInfoWrapper * piw = theApp.GetInfoWrapper();
	_ASSERTE( piw != NULL );

	pParentWnd->SetHeaderText( piw->GetTipsInfo(dwFlag) );
	
	CSMSFrame *pMfr = theApp.m_pSMSFrame;
	CStatusBar *pSb = &pMfr->m_wndStatusBar;
	pSb->SetPaneText(0,piw->GetTipsInfo(dwFlag));	

}

void CInfoListView::SetHeader( DWORD dwFlag )
{
	CChildFrameWnd * pParentWnd = (CChildFrameWnd *)GetParent();
	_ASSERTE( pParentWnd != NULL );
	ASSERT_KINDOF( CChildFrameWnd, pParentWnd ); 

	CInfoWrapper * piw = theApp.GetInfoWrapper();
	_ASSERTE( piw != NULL );
	
	pParentWnd->SetHeaderText( piw->GetTipsInfo(dwFlag) );
}

void CInfoListView::UpdateColumn(DWORD dwFlag)
{
	_ASSERTE( m_pIW != NULL );

	int nNum = m_pIW->GetColumnCount(dwFlag);
	// 需要改变显示的提示
	if( nNum > 0 )
	{
		
		CListCtrl &lst = GetListCtrl();//lint !e1514
		
		CHeaderCtrl * pHeaderCtrl = lst.GetHeaderCtrl();
		
		int nClmns = pHeaderCtrl->GetItemCount();
		for( int i = nClmns; i >= 0; --i )
		{
			lst.DeleteColumn(i);
		}

//		int nIdx = 0;
		
		for( i = 0; i < nNum; ++i )
		{
			lst.InsertColumn( i, m_pIW->GetColumn(dwFlag, i), LVCFMT_LEFT, 
				              m_pIW->GetColumnWidth(dwFlag, i) );
		}

		if(!theApp.m_bUSIM)
		{
			if(HIWORD(dwFlag)==ID_CATEG_PB)
			{
				lst.DeleteColumn(nNum-3); // remove fax
			}
			else if(HIWORD(dwFlag)==ID_CATEG_PBGM)
			{
				lst.DeleteColumn(nNum-2); // remove fax
			}
		}
	}
}

void CInfoListView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	LPNMITEMACTIVATE pNMIA = (LPNMITEMACTIVATE)pNMHDR;

	m_nFocusItem = pNMIA->iItem;


	m_strPhoneNum = GetListCtrl().GetItemText(m_nFocusItem, 2);

	CMenu * pMenu = NULL;
	
	if( m_CoolMenu.m_hMenu != NULL )
		VERIFY( m_CoolMenu.DestroyMenu() );
	

	CSMSFrame * pFrame = theApp.m_pSMSFrame;
	_ASSERTE( pFrame != NULL );

	VERIFY( m_CoolMenu.LoadPopupMenu(IDR_MNU_INFOLIST, NULL) );

    pMenu = m_CoolMenu.GetSubMenu(0);
    ASSERT(pMenu != NULL);
	
	//teana hu 2010.10.22
	if(!theApp.m_bLoadAudioPage)
	{
		pMenu->DeleteMenu(ID_CALL_FROM_PB, MF_BYCOMMAND);
	}

	m_CoolMenu.ResetPopupMenu(pMenu);
	PreTrackPopopMenu( pMenu, pNMIA->iItem );
	


	POINT pt = pNMIA->ptAction;
	ClientToScreen(&pt);

	pMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);

	*pResult = 1;
}

void CInfoListView::PreTrackPopopMenu(CMenu *pMenu, int nItem)
{
	_ASSERTE( pMenu != NULL );
  
	// general
	if( nItem == -1 )
	{
		pMenu->EnableMenuItem(ID_EDIT_EDIT, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_EDIT_COPY, MF_BYCOMMAND | MF_GRAYED);
	}
	
	CListCtrl & lst = GetListCtrl();//lint !e1514

	int nSelCount = lst.GetSelectedCount();
	if( nSelCount == 0 )
		pMenu->EnableMenuItem(ID_EDIT_DELETE, MF_BYCOMMAND | MF_GRAYED);
	else if( nSelCount > 1 ) // multi select 
		pMenu->EnableMenuItem(ID_EDIT_EDIT, MF_BYCOMMAND | MF_GRAYED);

	if( lst.GetItemCount() == 0 )
		pMenu->EnableMenuItem(ID_EDIT_SELECT_ALL, MF_BYCOMMAND | MF_GRAYED);


	WORD wCateg = HIWORD(m_dwCategID);
	//////////////////////
	if(wCateg == ID_CATEG_SMS)
	{
		//teana
		pMenu->DeleteMenu(ID_SEND_SMS_FROM_PB,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_CALL_FROM_PB,   MF_BYCOMMAND);
		
		pMenu->DeleteMenu(24,   MF_BYPOSITION);
		pMenu->DeleteMenu(23,   MF_BYPOSITION);
		pMenu->DeleteMenu(22,   MF_BYPOSITION);
		
		pMenu->DeleteMenu(ID_SYNC_MEMO,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_IMPORT_VCALENDAR,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_EXPORT_TO_VCALENDAR,   MF_BYCOMMAND);
		pMenu->DeleteMenu(18, MF_BYPOSITION);
		
		
		pMenu->DeleteMenu(ID_SYNC_PB,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_IMPORT_VCARD,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_EXPORT_TO_VCARD,   MF_BYCOMMAND);
		pMenu->DeleteMenu(14, MF_BYPOSITION);
		
		if( nItem == -1 || !theApp.m_bConnected)
		{
			pMenu->EnableMenuItem(ID_ILM_SMS_SEND,     MF_BYCOMMAND | MF_GRAYED);
			pMenu->EnableMenuItem(ID_ILM_SMS_ANSWER,   MF_BYCOMMAND | MF_GRAYED);
			pMenu->EnableMenuItem(ID_ILM_SMS_FORWARD,  MF_BYCOMMAND | MF_GRAYED);
			pMenu->EnableMenuItem(ID_EXTRACT_NUMBER,  MF_BYCOMMAND | MF_GRAYED);

		}
		else
		{
			pMenu->EnableMenuItem(ID_ILM_SMS_SEND,   MF_BYCOMMAND | MF_GRAYED);
			pMenu->EnableMenuItem(ID_ILM_SMS_ANSWER, MF_BYCOMMAND | MF_GRAYED);
			if( nSelCount > 1 )
				pMenu->EnableMenuItem(ID_ILM_SMS_FORWARD, MF_BYCOMMAND | MF_GRAYED);	


			_ASSERTE( m_pIW != NULL ); 

			PSMS psms = (PSMS)lst.GetItemData(nItem);
			// 发件箱
			if( nSelCount == 1 && psms->wStatus == SMS_STATUS_UNSEND && psms->bExistSC )
				pMenu->EnableMenuItem(ID_ILM_SMS_SEND, MF_BYCOMMAND | MF_ENABLED);

			// 收件箱
			if( nSelCount == 1 && psms->wStatus == SMS_STATUS_READ || psms->wStatus == SMS_STATUS_UNREAD )
				pMenu->EnableMenuItem(ID_ILM_SMS_ANSWER, MF_BYCOMMAND | MF_ENABLED);
		}
		return;
	}
	if(wCateg == ID_CATEG_PB)
	{
#ifdef _EGO
		pMenu->DeleteMenu(ID_CALL_FROM_PB,   MF_BYCOMMAND);
#endif
		if(LOWORD(m_dwCategID) == ID_STORE_ALL)
		{
			pMenu->DeleteMenu(24,   MF_BYPOSITION);//群组
			pMenu->DeleteMenu(23,   MF_BYPOSITION);//位置
			pMenu->DeleteMenu(22,   MF_BYPOSITION);
		}
		if(LOWORD(m_dwCategID) != ID_STORE_PHONE )
			pMenu->DeleteMenu(24,   MF_BYPOSITION); //群组

		if(LOWORD(m_dwCategID) == ID_STORE_PHONE)
		{
			int nPBG = m_pIW->GetPBGCount(ID_STORE_ALL);
			if(nPBG>0)
			{
				CMenu *pSubMenuPBG = pMenu->GetSubMenu(24);
				int nPBGMenu = pSubMenuPBG->GetMenuItemCount();
				int i;
				for(i = 0; i< nPBG;i++)
				{
					if( m_pIW->GetPBG(i)->nID == ID_PGM_SIM1 ||
						m_pIW->GetPBG(i)->nID == ID_PGM_SIM2 ||
						m_pIW->GetPBG(i)->nID == ID_PGM_SIM3 ||
						m_pIW->GetPBG(i)->nID == ID_PGM_SIM4) 
					{
						nPBG = i;
						break;
					}
					pSubMenuPBG->ModifyMenu(ID_GROUP_ONE + i,MF_BYCOMMAND| MF_STRING,ID_GROUP_ONE + i,m_pIW->GetPBG(i)->szName);
				}
				for(i= nPBGMenu -1; i>=nPBG;i--)
					pSubMenuPBG->DeleteMenu(i,MF_BYPOSITION);
			}
			else
			{
				pMenu->EnableMenuItem(24,MF_BYPOSITION | MF_GRAYED);
			}		
		}
		
		CMenu *pSubMenu = pMenu->GetSubMenu(23);
		if(pSubMenu != NULL)
		{
			if(LOWORD(m_dwCategID)==ID_STORE_PC)
				pSubMenu->EnableMenuItem(ID_COPY_MOVE_TO_PC,MF_BYCOMMAND | MF_GRAYED);
			else if(LOWORD(m_dwCategID)==ID_STORE_PHONE)
				pSubMenu->EnableMenuItem(ID_COPY_MOVE_TO_MS,MF_BYCOMMAND | MF_GRAYED);
			else if(LOWORD(m_dwCategID)==ID_STORE_SIM)
				pSubMenu->EnableMenuItem(ID_COPY_MOVE_TO_SIM,MF_BYCOMMAND | MF_GRAYED);
			else if(LOWORD(m_dwCategID)==ID_STORE_SIM2)
				pSubMenu->EnableMenuItem(ID_COPY_MOVE_TO_SIM2,MF_BYCOMMAND | MF_GRAYED);
			else if(LOWORD(m_dwCategID)==ID_STORE_SIM3)
				pSubMenu->EnableMenuItem(ID_COPY_MOVE_TO_SIM3,MF_BYCOMMAND | MF_GRAYED);
			else if(LOWORD(m_dwCategID)==ID_STORE_SIM4)
				pSubMenu->EnableMenuItem(ID_COPY_MOVE_TO_SIM4,MF_BYCOMMAND | MF_GRAYED);
			
			if(!theApp.IsPluggedSIM(1))
			{		
				if(theApp.m_bConnected)
					pSubMenu->DeleteMenu(ID_COPY_MOVE_TO_SIM,MF_BYCOMMAND);
			}
			if(!theApp.IsPluggedSIM(2))
			{		
				if(theApp.m_bConnected)
					pSubMenu->DeleteMenu(ID_COPY_MOVE_TO_SIM2,MF_BYCOMMAND);
			}
			if(!theApp.IsPluggedSIM(3))
			{		
				if(theApp.m_bConnected)
					pSubMenu->DeleteMenu(ID_COPY_MOVE_TO_SIM3,MF_BYCOMMAND);
			}
			if(!theApp.IsPluggedSIM(4))
			{		
				if(theApp.m_bConnected)
					pSubMenu->DeleteMenu(ID_COPY_MOVE_TO_SIM4,MF_BYCOMMAND);
			}
		}
				
		if(lst.GetSelectedCount() < 1)
		{
			if(LOWORD(m_dwCategID) == ID_STORE_PHONE )
			{
				pMenu->EnableMenuItem(24,   MF_BYPOSITION | MF_GRAYED);
				pMenu->EnableMenuItem(23,   MF_BYPOSITION | MF_GRAYED);
			}
			else if( LOWORD(m_dwCategID) == ID_STORE_SIM  || 
				     LOWORD(m_dwCategID) == ID_STORE_SIM2 ||
					 LOWORD(m_dwCategID) == ID_STORE_SIM3 ||
					 LOWORD(m_dwCategID) == ID_STORE_SIM4 ||
				     LOWORD(m_dwCategID) == ID_STORE_PC)
			{
				pMenu->EnableMenuItem(23,   MF_BYPOSITION | MF_GRAYED);
			}
		}	
		
		pMenu->DeleteMenu(ID_SYNC_MEMO,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_IMPORT_VCALENDAR,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_EXPORT_TO_VCALENDAR,   MF_BYCOMMAND);		
		pMenu->DeleteMenu(18, MF_BYPOSITION);
		pMenu->DeleteMenu(ID_EXTRACT_NUMBER,     MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_OPTION_SMS_PRESET, MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_ILM_SMS_FORWARD,  MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_ILM_SMS_ANSWER,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_ILM_SMS_SEND,     MF_BYCOMMAND);
		pMenu->DeleteMenu(8, MF_BYPOSITION);
		if( nItem == -1 )
		{
			pMenu->EnableMenuItem(ID_EXPORT_TO_VCARD, MF_BYCOMMAND | MF_GRAYED);
#ifndef _EGO
			//teana
			pMenu->EnableMenuItem(ID_CALL_FROM_PB,   MF_BYCOMMAND | MF_GRAYED);
#endif
			pMenu->EnableMenuItem(ID_SEND_SMS_FROM_PB,   MF_BYCOMMAND | MF_GRAYED);
		}
			
		if( !theApp.m_bConnected || m_pIW->GetPBCount(ID_STORE_ALL) == 0)
		{
			pMenu->EnableMenuItem(ID_SYNC_PB, MF_BYCOMMAND | MF_GRAYED);
		}	

		return;
	}
	if(wCateg == ID_CATEG_MEMO)
	{
		//teana
		pMenu->DeleteMenu(ID_CALL_FROM_PB,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_SEND_SMS_FROM_PB,   MF_BYCOMMAND);
		
		pMenu->DeleteMenu(24,   MF_BYPOSITION);
		if(LOWORD(m_dwCategID) == ID_STORE_ALL)
		{			
			pMenu->DeleteMenu(23,   MF_BYPOSITION);
			pMenu->DeleteMenu(22,   MF_BYPOSITION);
		}
		else
		{
			CMenu *pSubMenu = pMenu->GetSubMenu(23);
			pSubMenu->DeleteMenu(ID_COPY_MOVE_TO_SIM4,MF_BYCOMMAND);
			pSubMenu->DeleteMenu(ID_COPY_MOVE_TO_SIM3,MF_BYCOMMAND);
			pSubMenu->DeleteMenu(ID_COPY_MOVE_TO_SIM2,MF_BYCOMMAND);
			pSubMenu->DeleteMenu(ID_COPY_MOVE_TO_SIM,MF_BYCOMMAND);
			if(LOWORD(m_dwCategID)==ID_STORE_PC)
				pSubMenu->EnableMenuItem(ID_COPY_MOVE_TO_PC,MF_BYCOMMAND | MF_GRAYED);
			else if(LOWORD(m_dwCategID)==ID_STORE_PHONE)
				pSubMenu->EnableMenuItem(ID_COPY_MOVE_TO_MS,MF_BYCOMMAND | MF_GRAYED);
		}
		
		if(lst.GetSelectedCount() < 1)
		{
			if(LOWORD(m_dwCategID) != ID_STORE_ALL )
			{
				pMenu->EnableMenuItem(23,   MF_BYPOSITION | MF_GRAYED);
			}
		}	

		pMenu->DeleteMenu(ID_SYNC_PB,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_IMPORT_VCARD,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_EXPORT_TO_VCARD,   MF_BYCOMMAND);
		pMenu->DeleteMenu(14, MF_BYPOSITION);
		pMenu->DeleteMenu(ID_EXTRACT_NUMBER,     MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_OPTION_SMS_PRESET, MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_ILM_SMS_FORWARD,  MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_ILM_SMS_ANSWER,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_ILM_SMS_SEND,     MF_BYCOMMAND);
		pMenu->DeleteMenu(8, MF_BYPOSITION);
		
		if( nItem == -1 )
			pMenu->EnableMenuItem(ID_EXPORT_TO_VCALENDAR, MF_BYCOMMAND | MF_GRAYED);
		if( !theApp.m_bConnected || m_pIW->GetMEMOCount(ID_STORE_ALL) == 0)
			pMenu->EnableMenuItem(ID_SYNC_MEMO, MF_BYCOMMAND | MF_GRAYED);
		return;
	}
	if( wCateg == ID_CATEG_PBG )
	{
		//teana
		pMenu->DeleteMenu(ID_CALL_FROM_PB,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_SEND_SMS_FROM_PB,   MF_BYCOMMAND);

		pMenu->DeleteMenu(24,   MF_BYPOSITION);
		pMenu->DeleteMenu(23,   MF_BYPOSITION);
		pMenu->DeleteMenu(22,   MF_BYPOSITION);

		pMenu->DeleteMenu(ID_SYNC_MEMO,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_IMPORT_VCALENDAR,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_EXPORT_TO_VCALENDAR,   MF_BYCOMMAND);		
		pMenu->DeleteMenu(18, MF_BYPOSITION);
		pMenu->DeleteMenu(ID_SYNC_PB,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_IMPORT_VCARD,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_EXPORT_TO_VCARD,   MF_BYCOMMAND);
		pMenu->DeleteMenu(14, MF_BYPOSITION);
		pMenu->DeleteMenu(ID_EXTRACT_NUMBER,     MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_OPTION_SMS_PRESET, MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_ILM_SMS_FORWARD,  MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_ILM_SMS_ANSWER,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_ILM_SMS_SEND,     MF_BYCOMMAND);
		pMenu->DeleteMenu(8, MF_BYPOSITION);
		
		pMenu->EnableMenuItem(ID_EDIT_NEW, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_EDIT_DELETE, MF_BYCOMMAND | MF_GRAYED);
	}

	if( wCateg == ID_CATEG_PBGM )
	{
		//teana
		pMenu->DeleteMenu(ID_CALL_FROM_PB,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_SEND_SMS_FROM_PB,   MF_BYCOMMAND);

		int nPBG = m_pIW->GetPBGCount(ID_STORE_ALL);
		if(nPBG>0)
		{
			CMenu *pSubMenuPBG = pMenu->GetSubMenu(24);
			int nPBGMenu = pSubMenuPBG->GetMenuItemCount();
			int i;
			for(i = 0; i< nPBG;i++)
			{
				if( m_pIW->GetPBG(i)->nID == ID_PGM_SIM1 || 
					m_pIW->GetPBG(i)->nID == ID_PGM_SIM2 ||
					m_pIW->GetPBG(i)->nID == ID_PGM_SIM3 ||
					m_pIW->GetPBG(i)->nID == ID_PGM_SIM4)
				{
					nPBG = i;
					break;
				}
				pSubMenuPBG->ModifyMenu(ID_GROUP_ONE + i,MF_BYCOMMAND,ID_GROUP_ONE + i,m_pIW->GetPBG(i)->szName);
			}
			for(i= nPBGMenu-1; i>= nPBG;i--)
				pSubMenuPBG->DeleteMenu(i,MF_BYPOSITION);
		}
		else
		{
			pMenu->EnableMenuItem(24,MF_BYPOSITION | MF_GRAYED);
		}	
		
		pMenu->DeleteMenu(23,   MF_BYPOSITION);
		pMenu->DeleteMenu(ID_SYNC_MEMO,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_IMPORT_VCALENDAR,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_EXPORT_TO_VCALENDAR,   MF_BYCOMMAND);		
		pMenu->DeleteMenu(18, MF_BYPOSITION);
		pMenu->DeleteMenu(ID_SYNC_PB,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_IMPORT_VCARD,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_EXPORT_TO_VCARD,   MF_BYCOMMAND);
		pMenu->DeleteMenu(14, MF_BYPOSITION);
		pMenu->DeleteMenu(ID_EXTRACT_NUMBER,     MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_OPTION_SMS_PRESET, MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_ILM_SMS_FORWARD,  MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_ILM_SMS_ANSWER,   MF_BYCOMMAND);
		pMenu->DeleteMenu(ID_ILM_SMS_SEND,     MF_BYCOMMAND);
		pMenu->DeleteMenu(8, MF_BYPOSITION);
		
		pMenu->EnableMenuItem(ID_EDIT_NEW, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_EDIT_DELETE, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_EDIT_EDIT, MF_BYCOMMAND | MF_GRAYED);


	}
}

void CInfoListView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	LPNMITEMACTIVATE pNMIA = (LPNMITEMACTIVATE)pNMHDR;

	int nItem = pNMIA->iItem;
	m_nFocusItem = nItem;
	if(  nItem!= -1 )
	{
		m_bFocusItemLock = TRUE;
		OnEdit();
		m_bFocusItemLock = FALSE;

		SetFocus();
	}
	*pResult = 1;
}

void CInfoListView::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	//TRACE0("begin Draw menu in infolistview \r\n");
	if( lpDrawItemStruct->CtlType == ODT_MENU && m_CoolMenu.m_hMenu != NULL )
		m_CoolMenu.DrawItem(lpDrawItemStruct);

	CListView::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CInfoListView::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if( lpMeasureItemStruct->CtlType == ODT_MENU && m_CoolMenu.m_hMenu != NULL )
		m_CoolMenu.MeasureItem(lpMeasureItemStruct);

	CListView::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

BOOL CInfoListView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	return CListView::OnEraseBkgnd(pDC);
}

DWORD CInfoListView::GetFocusItemData()
{
	
	CListCtrl &lst = GetListCtrl();//lint !e1514

	_ASSERTE( m_nFocusItem != -1 );

	return lst.GetItemData(m_nFocusItem);
}

void CInfoListView::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if(!m_bFocusItemLock)
	{
		m_nFocusItem = pNMListView->iItem;
	}

	// TODO: Add your control notification handler code here
	CDocument * pDoc = GetDocument();
	_ASSERTE( pDoc != NULL );
	if( pNMListView->iItem != -1 && pNMListView->uNewState > 0 )
	{
		static UIC_HINT uicHint;
		uicHint.dwFlag = MAKELONG(0, HIWORD(m_dwCategID));
		uicHint.dwData = pNMListView->lParam;

		pDoc->UpdateAllViews(this, WM_UPDATE_ITEM_CONTENT, (CObject *)&uicHint);
	}
	else
	{
		pDoc->UpdateAllViews(this, WM_UPDATE_ITEM_CONTENT, (CObject *)NULL);
	}

	*pResult = 1;
}



void CInfoListView::OnExtractNumber() 
{
	// TODO: Add your command handler code here
	
	CListCtrl &lst = GetListCtrl();//lint !e1514
	CHeaderCtrl * pHC = lst.GetHeaderCtrl();
	_ASSERTE( pHC != NULL );

//	const int nClmnCount = pHC->GetItemCount();
	
	POSITION pos   = lst.GetFirstSelectedItemPosition();
	_ASSERTE( pos != NULL );

    const int nSelCount = lst.GetSelectedCount();

	CUIntArray arrSel;
	arrSel.SetSize(nSelCount);
	int i=0;
	for( i = 0; i < nSelCount; ++i )
	{
		arrSel[i] = lst.GetNextSelectedItem(pos);
	}

//	DWORD dwTotal = 0;
	static const nMaxText = 256;
	_TCHAR szText[nMaxText];

	for( i = 0; i < nSelCount; ++i)
	{
		lst.GetItemText( arrSel.GetAt(i), 3, szText, nMaxText );
		CPBDlg dlg(theApp.m_pSMSFrame);
		dlg.m_bNew = TRUE;
		dlg.m_strPersonPhone = szText;

		int nGroupNum = m_pIW->GetPBGCount(ID_STORE_ALL);
		if(nGroupNum!=0)
		{
			for(int j=0;j<nGroupNum;j++)
				dlg.m_arrGroupName.Add(m_pIW->GetPBG(j)->szName);
		}

		if( dlg.DoModal() == IDOK )
		{
			if(HIWORD(m_dwCategID) == ID_CATEG_PB)
				UpdatePB(LOWORD(m_dwCategID));

			CDocument * pDoc = GetDocument();
			_ASSERTE( pDoc != NULL );

			pDoc->SetModifiedFlag(TRUE);
#ifndef SPRD8800
			if(m_pIW->GetPBGCount(ID_STORE_ALL) > 0 && dlg.m_pPB->wStore != ID_STORE_PC)
			{
				PPBGROUP pPBG = m_pIW->GetPBGByIdInPhone(dlg.m_pPB->nGroupIndex);
				if(pPBG != NULL) pPBG->nMemberNum++;
				
				m_pIW->UpdateCategPBG();
				pDoc->UpdateAllViews(NULL, WM_UPDATE_GROUP_CHANGE, NULL);
			}
#endif
		}
		else
		{
			if((i < nSelCount-1) && AfxMessageBox(IDS_WARN_EXTRACT_PB_NUM_CANCEL,MB_OKCANCEL)==IDOK)
				break;
		}
	}
}

void CInfoListView::OnSyncMemo() 
{
	// TODO: Add your command handler code here
	CWaitCursor waitCursor;

	CDocument * pDoc = GetDocument();
	_ASSERTE( pDoc != NULL );
	
	int nMaxInBM = 0;
	int nUsedInBM = 0;
	if( !theApp.TestIsConnected(FALSE) )
	{
		AfxMessageBox( IDS_ERR_NOT_CONNECT );
		return;
	}

	((CSMSDoc *)pDoc)->SetSyncFlag(TRUE);
	SendMessage(WM_COMMAND,ID_OPTION_LOAD_MEMO,0);
	((CSMSDoc *)pDoc)->SetSyncFlag(FALSE);

	if( !theApp.m_pMemoFactory->SelectStorePlace(ID_STORE_PHONE))
	{
		AfxMessageBox(IDS_ERR_DELETE);
		return;
	}
	if( !theApp.m_pMemoFactory->GetMemoNum(&nUsedInBM,&nMaxInBM) )
	{
		AfxMessageBox(IDS_ERR_DELETE);
		return;
	}
	
	CTypedPtrArray<CPtrArray, PMEMO>arrMEMOInPC;
	CTypedPtrArray<CPtrArray, PMEMO>arrMEMOInMS;
	int nCount = m_pIW->GetMEMOCount(ID_STORE_ALL);
	if(nCount == 0)
		return;
	int i=0,j=0;
	PMEMO pMemo = NULL;
	for(i=0;i<nCount;i++)
	{
		pMemo = m_pIW->GetMEMO(i);
		if(pMemo->wStore == ID_STORE_PC)
			arrMEMOInPC.Add(pMemo);
		else
			arrMEMOInMS.Add(pMemo);
	}
	int nCountPC=0,nCountMS=0;
	nCountPC = arrMEMOInPC.GetSize();
	nCountMS = arrMEMOInMS.GetSize();

	int nCmp = 0;

	
	for(i=0;i<nCountPC;i++)
	{
        for(j=0;j<nCountMS;j++)
		{
			nCmp =CompareMemo(arrMEMOInPC.GetAt(i),arrMEMOInMS.GetAt(j));
			if(nCmp == TOTAL_SAME)
			{
				arrMEMOInPC.RemoveAt(i);
				arrMEMOInMS.RemoveAt(j);
				nCountMS--;
				nCountPC--;
				i--;
				j--;
				break;
			}
		}
		if(i==-1)
			continue;
		if(j>=nCountMS && nCountPC != 0 && (nCmp == TOTAL_DIFF || nCountMS == 0 ))
		{
			if(nUsedInBM >= nMaxInBM)
			{
				AfxMessageBox(IDS_ERR_SYN_PHONE_FULL);
				UpdateMemo(LOWORD(m_dwCategID));
				pDoc->SetModifiedFlag(TRUE);	
				return;
			}
			
			if( !theApp.TestIsConnected(FALSE) )
			{
				AfxMessageBox(IDS_ERR_DELETE);
				return;
			}

			pMemo = new MEMO;
			memcpy(pMemo,arrMEMOInPC.GetAt(i),sizeof(MEMO));
			pMemo->wStore = ID_STORE_PHONE;

			if( !theApp.m_pMemoFactory->AddMemo(pMemo) )
			{
				AfxMessageBox(IDS_ERR_DELETE);
				delete pMemo;
				return;
			}	
			m_pIW->AddMEMO(pMemo);
			arrMEMOInPC.RemoveAt(i);
			nCountPC--;
			i--;
			nUsedInBM++;			
		}
	}
	nCountPC = arrMEMOInPC.GetSize();
	nCountMS = arrMEMOInMS.GetSize();

	for(i=0;i<nCountMS;i++)
	{
        for(j=0;j<nCountPC;j++)
		{
			nCmp = CompareMemo(arrMEMOInMS.GetAt(i),arrMEMOInPC.GetAt(j));
			if(nCmp != TOTAL_DIFF)
				break;
		}
		if(j>=nCountPC)
		{
			pMemo = new MEMO;
			memcpy(pMemo,arrMEMOInMS.GetAt(i),sizeof(MEMO));
			pMemo->wStore = ID_STORE_PC;
			m_pIW->AddMEMO(pMemo);
			arrMEMOInMS.RemoveAt(i);
			nCountMS--;
			i--;
		}
	}
	nCountPC = arrMEMOInPC.GetSize();
	nCountMS = arrMEMOInMS.GetSize();

	if(nCountPC == 0 && nCountMS==0 && !m_bSaveToFile)
		AfxMessageBox(IDS_SYNC_MEMO_SUCCESS);

	if(!m_bSaveToFile && HIWORD(m_dwCategID) == ID_CATEG_MEMO)
	{
		UpdateMemo(LOWORD(m_dwCategID));
		pDoc->SetModifiedFlag(TRUE);

		SetHeader(m_dwCategID);
	}
}

void CInfoListView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UNUSED_ALWAYS(pNMHDR);
	CSMSFrame *pMfr = theApp.m_pSMSFrame;
	CCoolStatusBar *pSb = &pMfr->m_wndStatusBar;
	
	CListCtrl &lst = GetListCtrl();//lint !e1514
	int nNum = lst.GetSelectedCount();
	CString strText;
	CString strPre;
	strPre.LoadString(IDS_SEL_ITEM_NUM);
	if(nNum > 0)
	{
		strText.Format(_T("%s%d"),strPre,nNum);
		pSb->SetPaneText(0,strText);
	}
	*pResult = 0;
}

void CInfoListView::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// save the index of the item being dragged in m_nDragIndex
	m_nDragIndex = pNMListView->iItem;
	//// Create a drag image
	POINT pt;
	int nOffset = -10; //offset in pixels for drag image
	pt.x = nOffset;
	pt.y = nOffset;
	
	CListCtrl &lst = GetListCtrl();//lint !e1514

	if(m_pDragImage)
	{
		delete m_pDragImage;
		m_pDragImage = NULL;
	}

	m_pDragImage = lst.CreateDragImage(m_nDragIndex, &pt);
	ASSERT(m_pDragImage); //make sure it was created
	//We will call delete later (in LButtonUp) to clean this up
	
	CBitmap bitmap;
	if(lst.GetSelectedCount() > 1) //more than 1 item in list is selected
		bitmap.LoadBitmap(IDB_BMP_DRAG_MULTI);
	else
		bitmap.LoadBitmap(IDB_BMP_DRAG_ONE);
	m_pDragImage->Replace(0, &bitmap, &bitmap);	
	
	//// Change the cursor to the drag image
	////	(still must perform DragMove() in OnMouseMove() to show it moving)
	m_pDragImage->BeginDrag(0, CPoint(nOffset, nOffset - 4));
	m_pDragImage->DragEnter(GetDesktopWindow(), pNMListView->ptAction);

	// set dragging flag
	m_bDragging = TRUE;
	m_hDropItem = NULL;
	m_nDropIndex = -1;
	m_pDropWnd = (CListView *)this;
	m_dwDragCategID = m_dwCategID;
	m_pTreeView = NULL;
	// capture all mouse messages
	SetCapture ();
	
	*pResult = 0;
}

void CInfoListView::DropItemOnTree (CTreeCtrl* pDropTree)
{
	// remove hilight from drop target
	pDropTree->SelectDropTarget (NULL);

//	VERIFY (pDropTree->SelectItem (m_hDropItem));
	
	if(m_hDropItem != NULL)
	{
		//if the tree node is what we want
		DWORD dwCat = pDropTree->GetItemData(m_hDropItem);
		
		switch( HIWORD(m_dwDragCategID) ) // the list
		{
		case ID_CATEG_PB:
			switch(HIWORD(dwCat)) // the tree
			{
			case ID_CATEG_PB:
				if( LOWORD(m_dwDragCategID) != 0 && ( LOWORD(m_dwDragCategID) & LOWORD(dwCat) ) == 0  )
					ChangePBStorePlace( LOWORD(dwCat));
				break;
			case ID_CATEG_PBGM:
				if( LOWORD(m_dwDragCategID) != 0 && LOWORD(m_dwDragCategID) != ID_STORE_PC)
				{
					m_bFromMenu = FALSE;
				    ChangePBGroup( (WORD)((LOWORD(dwCat)>>8) - 1));
				}
				break;
			default:				
				break;
			}
			break;
		case ID_CATEG_MEMO:
			switch(HIWORD(dwCat)) // the tree
			{
			case ID_CATEG_MEMO:
				if( LOWORD(m_dwDragCategID) != 0 && ( LOWORD(m_dwDragCategID) & LOWORD(dwCat) ) == 0  )
					ChangeMemoStorePlace( LOWORD(dwCat));
				break;
			default:				
				break;
			}
			break;
		case ID_CATEG_PBG:			
			break;
		case ID_CATEG_PBGM:
			switch(HIWORD(dwCat)) // the tree
			{
			case ID_CATEG_PBGM:
				if( LOWORD(m_dwDragCategID) != 0 && ( LOWORD(m_dwDragCategID) != LOWORD(dwCat) )   )
				{
					m_bFromMenu = FALSE;
					ChangePBGroup( (WORD)((LOWORD(dwCat)>>8) - 1));
				}
				break;
			default:				
				break;
			}
			break;
		default:
			_ASSERTE( 0 );
			break;
		}

		
	}
	
/*	
	char szLabel[256];
	LV_ITEM lvi;
	TV_ITEM tvi;
	TV_INSERTSTRUCT tvis;
	
	ZeroMemory (&lvi, sizeof (LV_ITEM));
	lvi.iItem		= m_nDragIndex;
	lvi.mask		= TVIF_IMAGE | TVIF_TEXT;
	lvi.pszText		= szLabel;
	lvi.cchTextMax	= 255;
	// get item that was dragged
	VERIFY (m_List.GetItem (&lvi));
	
	tvi.mask			= TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	tvi.pszText			= lvi.pszText;
	tvi.cchTextMax		= lvi.cchTextMax;
	tvi.iImage			= lvi.iImage;
	tvi.iSelectedImage	= lvi.iImage;
	
	// if cursor was over an tree item, insert new item above it
	if (m_hDropItem)
	{
		HTREEITEM hItem = pDropTree->GetNextItem (m_hDropItem, TVGN_PREVIOUS);
		tvis.hInsertAfter	= (hItem) ? hItem : TVI_FIRST;
	}
	// if cursor is not over an item but still inside tree, insert new item at the end
	else
		tvis.hInsertAfter	= TVI_LAST;
	
	tvis.hParent		= NULL;
	tvis.item			= tvi;
	// insert item into CTreeCtrl
	HTREEITEM hNewItem = pDropTree->InsertItem (&tvis);
	ASSERT (hNewItem);
	// select the new item we just inserted
	VERIFY (pDropTree->SelectItem (hNewItem));
	// delete the original item (move operation)
	VERIFY (m_List.DeleteItem (m_nDragIndex));*/

}

void CInfoListView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListCtrl &lstctrl = GetListCtrl();

	POSITION pos = lstctrl.GetFirstSelectedItemPosition();	
	int nItem = lstctrl.GetNextSelectedItem(pos);
	
	CString strText;
	strText = lstctrl.GetItemText( nItem, 1);
	CString strStatus;
	strStatus.LoadString(IDS_REC_UNREAD);
	if(strStatus == strText)
	{
		strStatus.LoadString(IDS_REC_READ);
		lstctrl.SetItemText(nItem, 1, strStatus);
		PSMS psms = (PSMS)(lstctrl.GetItemData(nItem));
		psms->wStatus = SMS_STATUS_READ;
	}

	CListView::OnLButtonDown(nFlags, point);
    
}

void CInfoListView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bDragging)
	{
		// end dragging
		VERIFY (m_pDragImage->DragLeave (GetDesktopWindow ()));	
		m_pDragImage->EndDrag ();
		if(m_pDragImage!=NULL)
		{
		    delete m_pDragImage; //must delete it because it was created at the beginning of the drag
			m_pDragImage = NULL;
		}
		// stop intercepting all mouse messages
		VERIFY (::ReleaseCapture ());
		m_bDragging = FALSE;
		
		CPoint pt (point);
		ClientToScreen (&pt);
		// get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT (pDropWnd);
		// if window is CTreeCtrl
		if (pDropWnd->IsKindOf (RUNTIME_CLASS (CTreeView)) && GetCursor() != LoadCursor(NULL, IDC_NO))			
			DropItemOnTree (&((CTreeView*)pDropWnd)->GetTreeCtrl());
		// if window is CListCtrl
		//else if (pDropWnd->IsKindOf (RUNTIME_CLASS (CListCtrl)))
		//	DropItemOnList ((CListCtrl*)pDropWnd);
		if(m_pTreeView != NULL)
			((CTreeView*)m_pTreeView)->GetTreeCtrl().SelectDropTarget(NULL);
	}
	
	CListView::OnLButtonUp(nFlags, point);
}

void CInfoListView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bDragging)
	{
		//// Move the drag image
		CPoint pt(point);	//get our current mouse coordinates
		ClientToScreen(&pt); //convert to screen coordinates
		// move the drag image
		m_pDragImage->DragMove (pt);
		// unlock window updates
		m_pDragImage->DragShowNolock (FALSE);
		
		//// Get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT(pDropWnd); //make sure we have a window
		
		//// If we drag outside current window we need to adjust the highlights displayed
		if (pDropWnd != m_pDropWnd)
		{
			// turn off hilight for drop target in tree control
			if (m_hDropItem)
			{
				(((CTreeView*)pDropWnd)->GetTreeCtrl()).SelectDropTarget (NULL);
				m_hDropItem = NULL;				
			}
			// turn off hilight for drop target in list control
			/*if (m_nDropIndex != -1)
			{
				CListCtrl* pList = (CListCtrl*)m_pDropWnd;
				VERIFY (pList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED));
				// redraw item
				VERIFY (pList->RedrawItems (m_nDropIndex, m_nDropIndex));
				pList->UpdateWindow ();
				m_nDropIndex = -1;
			}*/
		}
		
		// Save current window pointer as the CListCtrl we are dropping onto
		m_pDropWnd = pDropWnd;
		
		// Convert from screen coordinates to drop target client coordinates
		pDropWnd->ScreenToClient(&pt);
		
		// if window is CTreeCtrl
		if (pDropWnd->IsKindOf (RUNTIME_CLASS (CTreeView)))
		{
			m_pTreeView = pDropWnd;
			
			CTreeCtrl &tc = ((CTreeView*)pDropWnd)->GetTreeCtrl();//lint !e1514
			UINT uFlags;
			// get the item that is below cursor
			m_hDropItem = tc.HitTest (pt, &uFlags);
			
			if(m_hDropItem != NULL)
			{
				// highlight it
				tc.SelectDropTarget (m_hDropItem);
				// if not leaf node, expand it 
				if( tc.ItemHasChildren(m_hDropItem))
				{
					SetCursor(LoadCursor(NULL, IDC_NO));
					tc.Expand(m_hDropItem, TVE_EXPAND);
					tc.EnsureVisible(m_hDropItem);
					tc.UpdateWindow();
				}
				else
				{
					//if the tree node is what we want
					DWORD dwCat = tc.GetItemData(m_hDropItem);
					
					switch( HIWORD(m_dwDragCategID) ) // the list
					{
					case ID_CATEG_SMS:
						SetCursor(LoadCursor(NULL, IDC_NO));
						break;
					case ID_CATEG_PB:
						switch(HIWORD(dwCat)) // the tree
						{
							case ID_CATEG_PB:
								if( LOWORD(m_dwDragCategID) != 0 && ( LOWORD(m_dwDragCategID) & LOWORD(dwCat) ) == 0  )
								{
									SetCursor(LoadCursor(NULL, IDC_ARROW));													
								}
								else
								{
									SetCursor(LoadCursor(NULL, IDC_NO));
								}
								break;
							case ID_CATEG_PBGM:
								if(LOWORD(m_dwDragCategID) == ID_STORE_PC ||
									LOWORD(m_dwDragCategID) == ID_STORE_SIM ||
									LOWORD(m_dwDragCategID) == ID_STORE_SIM2 ||
									LOWORD(m_dwDragCategID) == ID_STORE_SIM3 ||
									LOWORD(m_dwDragCategID) == ID_STORE_SIM4)
								{
									SetCursor(LoadCursor(NULL, IDC_NO));
								}
								else
								{
									//SetCursor(LoadCursor(NULL, IDC_ARROW));								
									int nPBGID = (LOWORD(dwCat)>> 8) - 1;
									if(nPBGID >= 0 && nPBGID < m_pIW->GetPBGCount(ID_STORE_ALL) &&
										(m_pIW->GetPBG(nPBGID)->nID == ID_PGM_SIM1 ||
										 m_pIW->GetPBG(nPBGID)->nID == ID_PGM_SIM2 ||
										 m_pIW->GetPBG(nPBGID)->nID == ID_PGM_SIM3 ||
										 m_pIW->GetPBG(nPBGID)->nID == ID_PGM_SIM4 ) ||
										 LOWORD(m_dwDragCategID) == ID_STORE_ALL)
									{
										SetCursor(LoadCursor(NULL, IDC_NO));
									}
									else
									{
										SetCursor(LoadCursor(NULL, IDC_ARROW));
									}
									
								}
								break;
							default:
								SetCursor(LoadCursor(NULL, IDC_NO));
								break;
						}
						break;
					case ID_CATEG_MEMO:
						switch(HIWORD(dwCat)) // the tree
						{
						case ID_CATEG_MEMO:
							if( LOWORD(m_dwDragCategID) != 0 && ( LOWORD(m_dwDragCategID) & LOWORD(dwCat) ) == 0  )
								SetCursor(LoadCursor(NULL, IDC_ARROW));
							else
								SetCursor(LoadCursor(NULL, IDC_NO));
							break;
						default:
							SetCursor(LoadCursor(NULL, IDC_NO));
							break;
						}
						break;
					case ID_CATEG_PBG:
						SetCursor(LoadCursor(NULL, IDC_NO));
						break;
					case ID_CATEG_PBGM:
						switch(HIWORD(dwCat)) // the tree
						{
						case ID_CATEG_PBGM:
							if( LOWORD(m_dwDragCategID) != 0 && ( LOWORD(m_dwDragCategID) != LOWORD(dwCat) )  )
							{
								int nDragPBGID = (LOWORD(m_dwDragCategID)>> 8) - 1;
								if( nDragPBGID >= 0 && 
									nDragPBGID < m_pIW->GetPBGCount(ID_STORE_ALL) &&
									(m_pIW->GetPBG(nDragPBGID)->nID == ID_PGM_SIM1 ||
									 m_pIW->GetPBG(nDragPBGID)->nID == ID_PGM_SIM2 ||
									 m_pIW->GetPBG(nDragPBGID)->nID == ID_PGM_SIM3 ||
									 m_pIW->GetPBG(nDragPBGID)->nID == ID_PGM_SIM4 ))
								{
									SetCursor(LoadCursor(NULL, IDC_NO));
								}
								else
								{
									int nPBGID = (LOWORD(dwCat)>> 8) - 1;
									if( nPBGID >= 0 && 
										nPBGID < m_pIW->GetPBGCount(ID_STORE_ALL) &&
										(m_pIW->GetPBG(nPBGID)->nID == ID_PGM_SIM1 ||
										 m_pIW->GetPBG(nPBGID)->nID == ID_PGM_SIM2 ||
										 m_pIW->GetPBG(nPBGID)->nID == ID_PGM_SIM3 ||
										 m_pIW->GetPBG(nPBGID)->nID == ID_PGM_SIM4 ))
									{ // can not to change group to SIMx
										SetCursor(LoadCursor(NULL, IDC_NO));
									}
									else
									{									
										SetCursor(LoadCursor(NULL, IDC_ARROW));
									}
								}
							}
							else
							{
								SetCursor(LoadCursor(NULL, IDC_NO));
							}
							break;
						default:
							SetCursor(LoadCursor(NULL, IDC_NO));
							break;
						}
						break;
					default:
						_ASSERTE( 0 );
						break;
					}
				}
				
			}
			else
			{
				SetCursor(LoadCursor(NULL, IDC_NO));
			}

		}
		else
		{
			//If we are not hovering over a CListCtrl, change the cursor
			// to note that we cannot drop here
			SetCursor(LoadCursor(NULL, IDC_NO));
		}
		// lock window updates
		VERIFY (m_pDragImage->DragShowNolock (TRUE));
	}
	
	CListView::OnMouseMove(nFlags, point);
}
void CInfoListView::ChangeStorePlace(WORD wStore)
{	
	switch( HIWORD(m_dwCategID) ) // the list
	{
	case ID_CATEG_PB:
		if( LOWORD(m_dwCategID) != 0 && ( LOWORD(m_dwCategID) & wStore ) == 0  )
			ChangePBStorePlace( wStore );
		break;
	case ID_CATEG_MEMO:
		if( LOWORD(m_dwCategID) != 0 && ( LOWORD(m_dwCategID) & wStore ) == 0  )
			ChangeMemoStorePlace( wStore);
		break;
	default:				
		break;
	}
}

void CInfoListView::OnCopyMoveToPc() 
{
	// TODO: Add your command handler code here
	ChangeStorePlace(ID_STORE_PC);
}

void CInfoListView::OnUpdateCopyMoveToPc(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
	CListCtrl &lst = GetListCtrl();//lint !e1514
	BOOL bEnable = TRUE;
	if(lst.GetSelectedCount()== 0 || 
		( LOWORD(m_dwCategID) & ID_STORE_PC ) != 0 ||
        LOWORD(m_dwCategID) == ID_STORE_ALL ||
		lst.GetItemCount() == 0	)
		bEnable = FALSE;
	pCmdUI->Enable( bEnable );
}

void CInfoListView::OnCopyMoveToMs() 
{
	// TODO: Add your command handler code here
	ChangeStorePlace(ID_STORE_PHONE);
}

void CInfoListView::OnUpdateCopyMoveToMs(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
	CListCtrl &lst = GetListCtrl();//lint !e1514
	BOOL bEnable = TRUE;
	if(lst.GetSelectedCount()== 0 || 
		( LOWORD(m_dwCategID) & ID_STORE_PHONE ) != 0 ||
        LOWORD(m_dwCategID) == ID_STORE_ALL ||
		lst.GetItemCount() == 0	)
		bEnable = FALSE;
	pCmdUI->Enable( bEnable );
}

void CInfoListView::OnCopyMoveToSim() 
{
	// TODO: Add your command handler code here
	ChangeStorePlace(ID_STORE_SIM);
}

void CInfoListView::OnUpdateCopyMoveToSim(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
	CListCtrl &lst = GetListCtrl();//lint !e1514
	BOOL bEnable = TRUE;
	if(lst.GetSelectedCount()== 0 || 
		( LOWORD(m_dwCategID) & ID_STORE_SIM ) != 0 ||
        LOWORD(m_dwCategID) == ID_STORE_ALL ||
		lst.GetItemCount() == 0	||
		HIWORD(m_dwCategID) == ID_CATEG_MEMO)
		bEnable = FALSE;
	pCmdUI->Enable( bEnable );
}

void CInfoListView::OnCopyMoveToSim2() 
{
	// TODO: Add your command handler code here
	ChangeStorePlace(ID_STORE_SIM2);
}

void CInfoListView::OnUpdateCopyMoveToSim2(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
	CListCtrl &lst = GetListCtrl();//lint !e1514
	BOOL bEnable = TRUE;
	if(lst.GetSelectedCount()== 0 || 
		( LOWORD(m_dwCategID) & ID_STORE_SIM2 ) != 0 ||
        LOWORD(m_dwCategID) == ID_STORE_ALL ||
		lst.GetItemCount() == 0	||
		HIWORD(m_dwCategID) == ID_CATEG_MEMO)
		bEnable = FALSE;
	pCmdUI->Enable( bEnable );
}

void CInfoListView::OnCopyMoveToSim3() 
{
	// TODO: Add your command handler code here
	ChangeStorePlace(ID_STORE_SIM3);
}

void CInfoListView::OnUpdateCopyMoveToSim3(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
	CListCtrl &lst = GetListCtrl();//lint !e1514
	BOOL bEnable = TRUE;
	if(lst.GetSelectedCount()== 0 || 
		( LOWORD(m_dwCategID) & ID_STORE_SIM3 ) != 0 ||
        LOWORD(m_dwCategID) == ID_STORE_ALL ||
		lst.GetItemCount() == 0	||
		HIWORD(m_dwCategID) == ID_CATEG_MEMO)
		bEnable = FALSE;
	pCmdUI->Enable( bEnable );
}

void CInfoListView::OnCopyMoveToSim4() 
{
	// TODO: Add your command handler code here
	ChangeStorePlace(ID_STORE_SIM4);
}

void CInfoListView::OnUpdateCopyMoveToSim4(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
	CListCtrl &lst = GetListCtrl();//lint !e1514
	BOOL bEnable = TRUE;
	if(lst.GetSelectedCount()== 0 || 
		( LOWORD(m_dwCategID) & ID_STORE_SIM4 ) != 0 ||
        LOWORD(m_dwCategID) == ID_STORE_ALL ||
		lst.GetItemCount() == 0	||
		HIWORD(m_dwCategID) == ID_CATEG_MEMO)
		bEnable = FALSE;
	pCmdUI->Enable( bEnable );
}

void CInfoListView::OnGroupChange(UINT nID)
{
	m_bFromMenu = TRUE;
	ChangePBGroup( (WORD)(nID - ID_GROUP_ONE ));
}
