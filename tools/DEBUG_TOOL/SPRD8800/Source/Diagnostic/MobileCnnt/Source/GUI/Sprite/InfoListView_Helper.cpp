
#include "stdafx.h"
#include "..\MobileCnnt.h"
#include "..\MainFrm.h"
#include "InfoListView.h"
#include "..\MobileCnntDoc.h"
#include "InfoWrapper.h"
#include "msgiddef.h"

#ifndef __GLOBALDEF_H__
#include "GlobalDef.h"
#endif // __GLOBALDEF_H__

const int CInfoListView::DEL_PB_INTERVAL = 100; // ms 删除多条PB之间的间隔
const int CInfoListView::DEL_SMS_INTERVAL = 100; // ms 删除多条PB之间的间隔
const int CInfoListView::DEL_MEMO_INTERVAL = 100; // ms 删除多条PB之间的间隔

void CInfoListView::OnNew() 
{
	// TODO: Add your command handler code here
	switch( HIWORD(m_dwCategID) )
	{
	case ID_CATEG_SMS:
		NewSms();
		break;
	case ID_CATEG_PB:
		NewPb();
		break;
	case ID_CATEG_MEMO:
		NewMemo();
		break;
	//case ID_CATEG_SCHDL:
	//	NewSchdl();
	//	break;
	default:
		break;
	}
}

void CInfoListView::OnCommand(UINT uID)
{
	switch( uID )
	{
	case ID_EDIT_NEW_SMS:
		NewSms();
		break;
	case ID_EDIT_NEW_PB:
		NewPb();
		break;
	case ID_EDIT_NEW_MEMO:
		NewMemo();
		break;
	//case ID_EDIT_NEW_SCHDL:
	//	NewSchdl();
	//	break;
	case ID_EDIT_EDIT:
		OnEdit();
		break;
	case ID_EDIT_COPY:
		OnCopy();
		break;
	case ID_EDIT_DELETE:
		OnDelete();
		break;
	case ID_EDIT_SELECT_ALL:
		OnSelectAll();
		break;
	default:
		break;
	}
}

void CInfoListView::OnEdit() 
{
	// TODO: Add your command handler code here

	DWORD dwData = GetFocusItemData();
   
	switch( HIWORD(m_dwCategID) )
	{
	case ID_CATEG_SMS:
		EditSms( (PSMS)dwData );
		break;
	case ID_CATEG_PB:
		EditPb( (PPB)dwData );
		break;
	case ID_CATEG_MEMO:
		EditMemo( (PMEMO)dwData );
		break;
	//case ID_CATEG_SCHDL:
	//	EditSchdl( (PSCHDL)dwData );
	//	break;
	case ID_CATEG_PBG:
		EditPBG( (PPBGROUP)dwData );
		break;
	default:
		break;
	}
}

void CInfoListView::OnCopy() 
{
	// TODO: Add your command handler code here
	if( !OpenClipboard() )
		return;
	if( !EmptyClipboard() )
	{
		CloseClipboard();
		return;
	}

	BeginWaitCursor();

	
	CListCtrl &lst = GetListCtrl();//lint !e1514
	CHeaderCtrl * pHC = lst.GetHeaderCtrl();
	_ASSERTE( pHC != NULL );

	const int nClmnCount = pHC->GetItemCount();
	
	POSITION pos   = lst.GetFirstSelectedItemPosition();
	_ASSERTE( pos != NULL );
	const int nItemCount = lst.GetSelectedCount();
	
	DWORD dwTotal = 0;
	static const nMaxText = 1024;
	_TCHAR szText[nMaxText];
	int nItem=0;
	
	while(pos) 
	{		
		nItem = lst.GetNextSelectedItem(pos);
		for( int nClmn = 0; nClmn < nClmnCount; ++nClmn )
		{
			lst.GetItemText( nItem, nClmn, szText, nMaxText );
			dwTotal += _tcslen(szText);
			dwTotal += 1; // add ","
		}
	}
	
	_TCHAR szCL[] = _T("\r\n");
	dwTotal *= sizeof(_TCHAR);
	dwTotal += sizeof(szCL) * nItemCount;
	HGLOBAL hGbl = GlobalAlloc(GHND, dwTotal);
	if( NULL == hGbl )
	{
		CloseClipboard();
		EndWaitCursor();
		return;
	}
	
    LPTSTR pGblBuf = (LPTSTR)GlobalLock( hGbl );
	if( NULL == pGblBuf )
	{
		GlobalFree(hGbl);
		CloseClipboard();
		EndWaitCursor();
		return;
	}
	
	int    nSize = 0;
	LPTSTR pBuf  = pGblBuf;
	pos   = lst.GetFirstSelectedItemPosition();
	while(pos) 
	{		
		nItem = lst.GetNextSelectedItem(pos);
		for( int nClmn = 0; nClmn < nClmnCount; ++nClmn )
		{
			lst.GetItemText( nItem, nClmn, szText, nMaxText );
			nSize = _tcslen(szText);
			_tcscpy(pBuf, szText);
			_tcscat(pBuf, _T(","));
			pBuf += (nSize + 1);
		}
		pBuf--;
		_tcscpy(pBuf, szCL);
		pBuf += _tcslen(szCL);
	}

	GlobalUnlock(hGbl);
	if( NULL == SetClipboardData(CF_UNICODETEXT, hGbl) )
	{
		GlobalFree(hGbl);
	}

	CloseClipboard();

	EndWaitCursor();
}

void CInfoListView::OnDelete() 
{
	// TODO: Add your command handler code here
	CSMSFrame *pMfr = theApp.m_pSMSFrame;
	CWaitCursor waitCursor;
	pMfr->EnableWindow(FALSE);

	CDocument * pDoc = GetDocument();
	_ASSERTE( pDoc != NULL );

	
	CListCtrl &lst = GetListCtrl();//lint !e1514

	POSITION pos = lst.GetFirstSelectedItemPosition();
	_ASSERTE( pos != NULL );
    const int nSelCount = lst.GetSelectedCount();
	
	CString strWarn;
	strWarn.LoadString(IDS_DEL_CONFIRM);

	CMainFrame *pmf = (CMainFrame *)AfxGetMainWnd();

	if( MessageBox(strWarn,pmf->GetAppTile(),MB_ICONWARNING|MB_YESNO) == IDNO )
	{
		pMfr->EnableWindow(TRUE);
		return;
	}

	CUIntArray arrSel;
	arrSel.SetSize(nSelCount);
	int i=0;
	m_nFocusItem = -1;
	for( i = nSelCount - 1; i >= 0; --i )
	{
		arrSel[i] = lst.GetNextSelectedItem(pos);		
		if((int)arrSel[i]>m_nFocusItem)
			m_nFocusItem = arrSel[i];
	}
    
	const WORD wCateg = HIWORD(m_dwCategID);
	const WORD wStore = LOWORD(m_dwCategID);
	LockWindowUpdate();

	DWORD dwData = 0;
	BOOL bDel = TRUE;
	BOOL bMore= FALSE;
	if(nSelCount>1)
		bMore = TRUE;
	::PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_BEGIN,nSelCount);
	MSG msg;
	for( i = 0; i < nSelCount; ++i )
	{
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		::PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_PROCEED, i+1);

		dwData = lst.GetItemData(arrSel[i]);
		switch( wCateg )
		{
		case ID_CATEG_SMS:
			bDel = DeleteSms((PSMS)dwData);
			if(bMore && i<nSelCount-1)
			{
				dwData = lst.GetItemData(arrSel[i+1]);			
				if(((PSMS)dwData)->wStore != ID_STORE_PC)
					Sleep( DEL_SMS_INTERVAL );
			}
			break;
		case ID_CATEG_PB:
			bDel = DeletePb((PPB)dwData);
			if(bMore && i<nSelCount-1)
			{
				dwData = lst.GetItemData(arrSel[i+1]);
				//if(((PPB)dwData)->wStore != ID_STORE_PC)
				//	Sleep( DEL_PB_INTERVAL );
			}
			break;
		case ID_CATEG_MEMO:
			bDel = DeleteMemo((PMEMO)dwData);
			if(bMore && i<nSelCount-1)
			{
				dwData = lst.GetItemData(arrSel[i+1]);
				//if(((PMEMO)dwData)->wStore != ID_STORE_PC)
				//	Sleep( DEL_MEMO_INTERVAL );
			}
			break;
		//case ID_CATEG_SCHDL:
		//	DeleteSchdl((PSCHDL)dwData);
		//	break;
		default:
			break;
		}
		if(!bDel)
			break;
		else
		{
			m_nFocusItem--;
		}
	}
	::PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, 0);
    
	switch( wCateg )
	{
	case ID_CATEG_SMS:
		UpdateSMS( wStore );
		pDoc->UpdateAllViews(this, WM_UPDATE_ITEM_CONTENT, (CObject *)NULL);
		break;
	case ID_CATEG_PB:
		UpdatePB( wStore );
#ifndef SPRD8800
		m_pIW->UpdateCategPBG();
		pDoc->UpdateAllViews(NULL, WM_UPDATE_GROUP_CHANGE, NULL);
#endif
		
		break;
	case ID_CATEG_MEMO:
		UpdateMemo( wStore );
		pDoc->UpdateAllViews(this, WM_UPDATE_ITEM_CONTENT, (CObject *)NULL);
		break;
	//case ID_CATEG_SCHDL:
	//	UpdateSchdl( wStore );
	//	break;
	default:
		break;
	}
    
	UnlockWindowUpdate();
	pDoc->SetModifiedFlag(TRUE);
	pMfr->EnableWindow(TRUE);
}

void CInfoListView::OnSelectAll() 
{
	// TODO: Add your command handler code here
	
	CListCtrl &lst = GetListCtrl();//lint !e1514
	lst.SetFocus();

	int nCount = lst.GetItemCount();
	for( int i = 0; i < nCount; ++i )
	{
		lst.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	}

	CSMSFrame *pMfr = theApp.m_pSMSFrame;
	CCoolStatusBar *pSb = &pMfr->m_wndStatusBar;
	CString strText;
	CString strPre;
	strPre.LoadString(IDS_SEL_ITEM_NUM);
	if(nCount > 0)
	{
		strText.Format(_T("%s%d"),strPre,nCount);
		pSb->SetPaneText(0,strText);
	}
}

void CInfoListView::GetStorePlace(WORD wStore, CString &strStore)
{
	switch( wStore )
	{
	case ID_STORE_PC:
		VERIFY( strStore.LoadString(IDS_STORE_PC) );
		break;
	case ID_STORE_PHONE:
		VERIFY( strStore.LoadString(IDS_STORE_PHONE) );
		break;
	case ID_STORE_SIM:
		VERIFY( strStore.LoadString(IDS_STORE_SIM) );
		break;
	case ID_STORE_SIM2:
		VERIFY( strStore.LoadString(IDS_STORE_SIM2) );
		break;
	case ID_STORE_SIM3:
		VERIFY( strStore.LoadString(IDS_STORE_SIM3) );
		break;
	case ID_STORE_SIM4:
		VERIFY( strStore.LoadString(IDS_STORE_SIM4) );
		break;
	default:
		strStore.Empty();
		break;
	}
}

void CInfoListView::GetSMSStatus( WORD wStatus, CString &strStatus )
{
	switch( wStatus )
	{
	case SMS_STATUS_READ:
		VERIFY( strStatus.LoadString(IDS_REC_READ) );
		break;
	case SMS_STATUS_UNREAD:
		VERIFY( strStatus.LoadString(IDS_REC_UNREAD) );
		break;
	case SMS_STATUS_SEND:
		VERIFY( strStatus.LoadString(IDS_STO_SEND) );
		break;
	case SMS_STATUS_UNSEND:
		VERIFY( strStatus.LoadString(IDS_STO_UNSEND) );
		break;
	}
}


