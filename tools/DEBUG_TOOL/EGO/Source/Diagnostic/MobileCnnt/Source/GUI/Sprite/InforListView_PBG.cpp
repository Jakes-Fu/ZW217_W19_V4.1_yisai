#include "stdafx.h"
#include "..\MobileCnnt.h"
#include "..\MobileCnntDoc.h"
#include "InfoListView.h"
#include "GlobalDef.h"
#include "InfoWrapper.h"
#include "IPBG.h"
#include "PbgDlg.h"
#include "msgiddef.h"

void CInfoListView::UpdatePBG(WORD wStore)
{
	
	CListCtrl &lst = GetListCtrl();//lint !e1514

	lst.DeleteAllItems();

	if( wStore & ID_STORE_PHONE )
		InsertPBG(ID_STORE_PHONE);

}
void CInfoListView::UpdatePBGM(WORD wStore)
{

	
	CListCtrl &lst = GetListCtrl();//lint !e1514

	lst.DeleteAllItems();
	int nCount = m_pIW->GetPBGCount(ID_STORE_ALL);
	for(int i =0;i<nCount;i++)
	{
		if(wStore == ID_STORE_PBGM[i])
		{
		   InsertPbgMember(ID_STORE_PBGM[i]);
		   break;
		}
	}
	if(lst.GetItemCount()>0 && m_nFocusItem != -1 && m_nFocusItem < lst.GetItemCount())
	{
		lst.SetItemState (m_nFocusItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		lst.EnsureVisible(m_nFocusItem, TRUE);
	}
	else
		m_nFocusItem = -1;
}

void CInfoListView::InsertPBG(WORD wStore)
{
	_ASSERTE( m_pIW != NULL );

	PPBGROUP pPBG    = NULL;
	int nCount = m_pIW->GetPBGCount(ID_STORE_ALL);
	for( int i = 0; i < nCount; ++i )
	{
		pPBG = m_pIW->GetPBG(i);
		if( pPBG->wStore == wStore )
			InsertPBG(pPBG);
	}
}

void CInfoListView::InsertPBG(_PBGROUP *pPBG, BOOL bNew /*= TRUE*/, int nItem /*= -1*/)
{
	_ASSERTE( pPBG != NULL );

	
	CListCtrl &lst = GetListCtrl();//lint !e1514

	_ASSERTE( m_pIW != NULL );

	CString strStore;
	CString strDate;

	GetStorePlace(pPBG->wStore, strStore);

	if(!theApp.m_bConnected && pPBG->wStore != ID_STORE_PC )
		strStore.LoadString(IDS_DISCONNECTED);//显现未连接
	
	if( bNew )
	{
		nItem = lst.GetItemCount();
		VERIFY( lst.InsertItem(nItem, strStore) != -1 );
	}
	else
	{
		//VERIFY( lst.SetItemText(nItem, 0, strStore) );
	}
 
	lst.SetItemText(nItem, 1, pPBG->szName);

	int nCount = m_pIW->GetPBGMemoberCount(nItem);
	CString str;
	str.Format(_T("%d"),/*pPBG->nMemberNum*/nCount);
	lst.SetItemText(nItem, 2, str);


	lst.SetItemData(nItem, (DWORD)pPBG);
}
/*
void CInfoListView::NewPb() 
{
	// TODO: Add your command handler code here
	CPBDlg dlg;
	dlg.m_bNew = TRUE;
	if( dlg.DoModal() == IDOK )
	{
		UpdatePB(LOWORD(m_dwCategID));

		CDocument * pDoc = GetDocument();
		_ASSERTE( pDoc != NULL );

		pDoc->SetModifiedFlag(TRUE);
	}
}
*/

void CInfoListView::EditPBG(_PBGROUP * pPBG)
{
	_ASSERTE( pPBG != NULL );

	CMainFrame * pMF = (CMainFrame*)AfxGetMainWnd();

	CPbgDlg dlg(theApp.m_pSMSFrame);
	
	dlg.m_bNew = FALSE;
	dlg.m_pPBG  = pPBG;
	if(pPBG->bFixedGroup)
	{
		//AfxMessageBox(IDS_WARN_FIXED_NOT_EDIT);
		CString str;
		str.LoadString(IDS_WARN_FIXED_NOT_EDIT);
		MessageBox(str,pMF->GetAppTile(),MB_ICONWARNING|MB_OK);
		return;
	}

	if( dlg.DoModal() == IDOK )
	{
		UpdatePBG(LOWORD(m_dwCategID));

		CDocument * pDoc = GetDocument();
		_ASSERTE( pDoc != NULL );

		pDoc->SetModifiedFlag(TRUE);
		m_pIW->UpdateCategPBG();
		pDoc->UpdateAllViews(NULL, WM_UPDATE_GROUP_CHANGE, NULL);
	}
}

/*
void CInfoListView::DeletePb(_PB * pPB)
{
	CWaitCursor waitCursor;

	_ASSERTE( pPB != NULL && m_pIW != NULL );

	CMobileSpriteApp * pApp = (CMobileSpriteApp *)AfxGetApp();
	IPB * pPBFactory = pApp->m_pPBFactory;
	_ASSERTE( pPBFactory != NULL );

	if( pPB->wStore == ID_STORE_PHONE || pPB->wStore == ID_STORE_SIM )
	{
		if( !pPBFactory->DeletePB(pPB) )
		{
			AfxMessageBox(IDS_ERR_DELETE);
			return;
		}
	}
	
	m_pIW->RemovePB(pPB);

	m_nFocusItem = -1;
}*/



void CInfoListView::InsertPbgMember(WORD wStore)
{
	_ASSERTE( m_pIW != NULL );

	PPB pPB    = NULL;
	int nCount = m_pIW->GetPBCount(ID_STORE_ALL);
	int nPBG   = m_pIW->GetPBGCount(ID_STORE_ALL);
	int nIndex = -1;
	for( int j =0; j < nPBG;j++ )
	{
		if(wStore == ID_STORE_PBGM[j])
		{
			nIndex = j;
			break;
		}

	}
	m_nNoNameID = 1;
	for( int i = 0; i < nCount; ++i )
	{
		pPB = m_pIW->GetPB(i);
		if( pPB->wStore != ID_STORE_PC && pPB->nGroupIndex == m_pIW->GetPBG(nIndex)->nID )
			InsertPbgMember(pPB);
	}
}

void CInfoListView::InsertPbgMember(_PB *pPB, BOOL bNew /*= TRUE*/, int nItem /*= -1*/)
{
	_ASSERTE( pPB != NULL );
    
	
	CListCtrl &lst = GetListCtrl();//lint !e1514

	_ASSERTE( m_pIW != NULL );

	CString strStore;
	CString strDate;

	GetStorePlace(pPB->wStore, strStore);
	
	if(!theApp.m_bConnected && pPB->wStore != ID_STORE_PC )
		strStore.LoadString(IDS_DISCONNECTED);//显现未连接
	
	if( bNew )
	{
		nItem = lst.GetItemCount();
		VERIFY( lst.InsertItem(nItem, strStore) != -1 );
	}
	else
	{
		VERIFY( lst.SetItemText(nItem, 0, strStore) );
	}
    static int nNoNameID = 1;
	if(_tcslen(pPB->szName)!=0)
		lst.SetItemText(nItem, 1, pPB->szName);
	else
	{
		CString str;
		str.Format(IDS_UNKNOWN,m_nNoNameID++);
		lst.SetItemText(nItem, 1, str);
	}
	lst.SetItemText(nItem, 2, pPB->szPhone);
	lst.SetItemText(nItem, 3, pPB->szOfficePhone);
	lst.SetItemText(nItem, 4, pPB->szHomePhone);
	
	if(theApp.m_bUSIM)
	{
		lst.SetItemText(nItem, 5, pPB->szFax);
		lst.SetItemText(nItem, 6, pPB->szEmail);
	}
	else
	{
		lst.SetItemText(nItem, 5, pPB->szEmail);
	}

	
	if(m_pIW->GetPBGCount(ID_STORE_ALL)!=0 )
	{
		if(pPB->wStore != ID_STORE_PC)
		{
			PPBGROUP pPBG = m_pIW->GetPBGByIdInPhone(pPB->nGroupIndex);
			if(pPBG != NULL)
			{
				lst.SetItemText(nItem, theApp.m_bUSIM?7:6, pPBG->szName); 
			}
			else
			{
				lst.SetItemText(nItem, theApp.m_bUSIM?7:6,_T("=="));
			}

			
		}
		else
			lst.SetItemText(nItem, theApp.m_bUSIM?7:6,_T("=="));
	}

	lst.SetItemData(nItem, (DWORD)pPB);
}
