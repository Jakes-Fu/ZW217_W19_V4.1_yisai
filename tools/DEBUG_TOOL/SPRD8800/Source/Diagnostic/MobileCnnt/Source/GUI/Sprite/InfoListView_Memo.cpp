#include "stdafx.h"
#include "..\MobileCnnt.h"
#include "..\MobileCnntDoc.h"
#include "InfoListView.h"
#include "InfoWrapper.h"
#include "IMemo.h"
#include "MemoDlg.h"
#include "VCalendar.h"
#include "FileDlg.h"

void CInfoListView::UpdateMemo(WORD wStore)
{
	
	CListCtrl &lst = GetListCtrl();//lint !e1514

	lst.DeleteAllItems();
	
	if( wStore & ID_STORE_PC )
		InsertMemo(ID_STORE_PC);

	if( wStore & ID_STORE_PHONE )
		InsertMemo(ID_STORE_PHONE);

	if(lst.GetItemCount()>0 && m_nFocusItem != -1 && m_nFocusItem < lst.GetItemCount())
	{
		lst.SetItemState (m_nFocusItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		lst.EnsureVisible(m_nFocusItem, TRUE);
	}
	else
		m_nFocusItem = -1;
}

void CInfoListView::InsertMemo(WORD wStore)
{
	_ASSERTE( m_pIW != NULL );

	PMEMO pMemo  = NULL;
	int   nCount = m_pIW->GetMEMOCount(ID_STORE_ALL);
	for( int i = 0; i < nCount; ++i )
	{
		pMemo = m_pIW->GetMEMO(i);
		if( pMemo->wStore == wStore )
			InsertMemo(pMemo);
	}
}

void CInfoListView::InsertMemo(_MEMO * pMemo, BOOL bNew /* = TRUE */, int nItem /* = -1*/)
{
	_ASSERTE( pMemo != NULL );

	
	CListCtrl &lst = GetListCtrl();//lint !e1514

	_ASSERTE( m_pIW != NULL );

	CString strStore;
	CString strDate;

	GetStorePlace(pMemo->wStore, strStore);
	
	if(!theApp.m_bConnected && pMemo->wStore != ID_STORE_PC )
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

	static const int nStatusLen = 12;
	_TCHAR szUsed[ nStatusLen ]   = { 0 };
	_TCHAR szUnused[ nStatusLen ] = { 0 };
	_TCHAR szInvlid[ nStatusLen ] = { 0 };

	lst.SetItemText(nItem, 1, pMemo->szContent);


	strDate.Format( _T("%04d-%02d-%02d"), pMemo->memoStartDate.wYear, 
					    pMemo->memoStartDate.btMonth, pMemo->memoStartDate.btDay );

	lst.SetItemText(nItem, 2, strDate);

	strDate.Format( _T("%04d-%02d-%02d"), pMemo->memoEndDate.wYear, 
					    pMemo->memoEndDate.btMonth, pMemo->memoEndDate.btDay );

	lst.SetItemText(nItem, 3, strDate);

	strDate.Format( _T("%02d:%02d"), pMemo->memoTime.btHour,  pMemo->memoTime.btMinut );

	lst.SetItemText(nItem, 4, strDate);

/*	if( pMemo->bUsed )
	{
		if( pMemo->wStore == ID_STORE_PHONE )
			lst.SetItemText(nItem, 1, szUsed);
		else
			lst.SetItemText(nItem, 1, szInvlid);
	}
	else
	{
		lst.SetItemText(nItem, 1, szUnused);
		//strDate.Empty();
	}
*///memo中的“bUsed”属性对用户没用
	
	
 	_ASSERTE( m_pIW != NULL );
	lst.SetItemText(nItem, 5, m_pIW->GetMemoMode(pMemo->nMemoMode - 1));

	if(pMemo->nMemoMode == 3 /*MEMO_MODE_EVERYWEEK*/)
	{
		lst.SetItemText(nItem, 6, m_pIW->GetMemoFre(pMemo->fre));
	}
	else
	{
		lst.SetItemText(nItem, 6, _T(""));
	}
	
	

	lst.SetItemData(nItem, (DWORD)pMemo);
}

void CInfoListView::NewMemo() 
{
	// TODO: Add your command handler code here
	CMemoDlg dlg(theApp.m_pSMSFrame);
	dlg.m_bNew = TRUE;
	if(LOWORD(m_dwCategID) == ID_STORE_PHONE)
	{
		dlg.m_nStore = 1;
	}
	else
	{
		dlg.m_nStore = 0;
	}
	if( dlg.DoModal() == IDOK )
	{
		CDocument * pDoc = GetDocument();
		_ASSERTE( pDoc != NULL );

		pDoc->SetModifiedFlag(TRUE);
		if(HIWORD(m_dwCategID)==ID_CATEG_MEMO)
		{
			UpdateMemo(LOWORD(m_dwCategID));
			SetHeader(m_dwCategID);
		}
	}
}

void CInfoListView::EditMemo( _MEMO * pMemo ) 
{
	// TODO: Add your command handler code here
	_ASSERTE( pMemo != NULL );

	CMemoDlg dlg(theApp.m_pSMSFrame);
	
	dlg.m_bNew  = FALSE;
	dlg.m_pMemo = pMemo;

	if( dlg.DoModal() == IDOK )
	{
		UpdateMemo(LOWORD(m_dwCategID));

		CDocument * pDoc = GetDocument();
		_ASSERTE( pDoc != NULL );

		pDoc->SetModifiedFlag(TRUE);

		
		SetHeader(m_dwCategID);
	}
}

BOOL CInfoListView::DeleteMemo( _MEMO * pMemo ) 
{
	// TODO: Add your command handler code here
	CWaitCursor waitCursor;

	_ASSERTE( pMemo != NULL && m_pIW != NULL );

	IMemo * pMemoFactory = theApp.m_pMemoFactory;
	_ASSERTE( pMemoFactory != NULL );

	if( pMemo->wStore == ID_STORE_PHONE)
	{
		if( !theApp.TestIsConnected(FALSE) )
		{
			AfxMessageBox(IDS_ERR_DELETE);
			return FALSE;
		}
		if( !pMemoFactory->DeleteMemo(pMemo) )//lint !e1514
		{
			AfxMessageBox(IDS_ERR_DELETE);
			return FALSE;
		}
	}
	
	m_pIW->RemoveMEMO(pMemo);

	//m_nFocusItem = -1;

	SetHeader(m_dwCategID);

	return TRUE;
}

void CInfoListView::ExportVCalendar(LPCTSTR pstrDirName)
{
	BeginWaitCursor();

	CSMSFrame *pMfr = theApp.m_pSMSFrame;
	pMfr->EnableWindow(FALSE);
	
	
	CListCtrl &lst = GetListCtrl();//lint !e1514

	POSITION pos   = lst.GetFirstSelectedItemPosition();
	_ASSERTE( pos != NULL );

	int nIndex = 0;
	int nItem = 0;

	::PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_BEGIN, lst.GetSelectedCount());
	
    MSG msg;
	while(pos) 
	{
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}		
		++nIndex;
		::PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_PROCEED, nIndex);

		nItem = lst.GetNextSelectedItem(pos);
		
		CVCalendar vcalendar;
		PMEMO pMemo = (PMEMO)lst.GetItemData(nItem);
		vcalendar.m_memoStartDate.wYear = pMemo->memoStartDate.wYear;
		vcalendar.m_memoStartDate.btMonth = pMemo->memoStartDate.btMonth;
		vcalendar.m_memoStartDate.btDay = pMemo->memoStartDate.btDay;
		vcalendar.m_memoStartDate.btHour = pMemo->memoTime.btHour;
		vcalendar.m_memoStartDate.btMinut = pMemo->memoTime.btMinut;

		vcalendar.m_memoEndDate.wYear = pMemo->memoEndDate.wYear;
		vcalendar.m_memoEndDate.btMonth = pMemo->memoEndDate.btMonth;
		vcalendar.m_memoEndDate.btDay = pMemo->memoEndDate.btDay;
		vcalendar.m_memoEndDate.btHour = pMemo->memoTime.btHour;
		vcalendar.m_memoEndDate.btMinut = pMemo->memoTime.btMinut;

		vcalendar.m_strDescript = pMemo->szContent;
		CString str;
		str.Format(_T("%s/%.4d-%.2d-%.2dT%.2d-%.2d_%.3d.vcs"),pstrDirName,
			pMemo->memoTime.wYear,pMemo->memoTime.btMonth,pMemo->memoTime.btDay,
			pMemo->memoTime.btHour,pMemo->memoTime.btMinut,++nIndex);
		vcalendar.Save(str);
	}	
    ::PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, nIndex);
    pMfr->EnableWindow(TRUE);
	
	EndWaitCursor();
}

/*
void CInfoListView::ImportVCalendar(LPCTSTR pstrDirName)
{
}
*/

void CInfoListView::OnExportToVcalendar() 
{
	// TODO: Add your command handler code here
    CFileDlg fDlg(FALSE);
	fDlg.SetHwnOwner(this->m_hWnd);
    
	CString strPath = fDlg.GetSelectDir();
	if(!strPath.IsEmpty())
		ExportVCalendar(strPath);	

}

void CInfoListView::OnImportVcalendar() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
		OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST,_T("VCalendar files (*.vcs) |*.vcs|All files(*.*)|*.*||"));
	CString strFiles(_T(""),256*500); //最多可以有500个长为256个字符的文件
	dlg.m_ofn.lpstrFile = strFiles.GetBuffer(256*500);
	dlg.m_ofn.nMaxFile = 256*500;
	strFiles.ReleaseBuffer();
	
	if(dlg.DoModal() == IDCANCEL)
	{
		if(CommDlgExtendedError() == FNERR_BUFFERTOOSMALL)
		{
			AfxMessageBox(IDS_ERR_FN_BUFFER_TOO_SMALL);
		}
		return;
	}
	CSMSFrame *pMfr = theApp.m_pSMSFrame;
	CWaitCursor waitCursor;
	pMfr->EnableWindow(FALSE);

	POSITION pos = dlg.GetStartPosition();
	if(pos == NULL)
	{
		pMfr->EnableWindow(TRUE);
		return;
	}
	CString filename;
	CString arrFailedFile;
	CVCalendar vcalendar;
	PMEMO pMemo = NULL;
	::PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_BEGIN,500);
	int nIndex =0;
    MSG msg;
	while(pos) 
	{
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}		
		if(nIndex+1 <= 500 )
			::PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_PROCEED, ++nIndex);

		filename = dlg.GetNextPathName(pos);
		if(!vcalendar.Parse(filename))
		{
			arrFailedFile += filename;
			arrFailedFile += "\n";
		}
        pMemo = new MEMO;
		pMemo->memoTime.wYear = (WORD)(vcalendar.m_memoStartDate.wYear);
		pMemo->memoTime.btMonth = (BYTE)(vcalendar.m_memoStartDate.btMonth);
		pMemo->memoTime.btDay = (BYTE)(vcalendar.m_memoStartDate.btDay);
		pMemo->memoTime.btHour = (BYTE)(vcalendar.m_memoStartDate.btHour);
		pMemo->memoTime.btMinut = (BYTE)(vcalendar.m_memoStartDate.btMinut);

		pMemo->memoStartDate.wYear = (WORD)(vcalendar.m_memoStartDate.wYear);
		pMemo->memoStartDate.btMonth = (BYTE)(vcalendar.m_memoStartDate.btMonth);
		pMemo->memoStartDate.btDay = (BYTE)(vcalendar.m_memoStartDate.btDay);
		pMemo->memoStartDate.btHour = (BYTE)(vcalendar.m_memoStartDate.btHour);
		pMemo->memoStartDate.btMinut = (BYTE)(vcalendar.m_memoStartDate.btMinut);

		pMemo->memoEndDate.wYear = (WORD)(vcalendar.m_memoEndDate.wYear);
		pMemo->memoEndDate.btMonth = (BYTE)(vcalendar.m_memoEndDate.btMonth);
		pMemo->memoEndDate.btDay = (BYTE)(vcalendar.m_memoEndDate.btDay);
		pMemo->memoEndDate.btHour = (BYTE)(vcalendar.m_memoEndDate.btHour);
		pMemo->memoEndDate.btMinut = (BYTE)(vcalendar.m_memoEndDate.btMinut);


		CString strCont = vcalendar.m_strDescript;
		int nSize = strCont.GetLength();
		if(strCont.GetLength()>MAX_MEMO_LEN)
		{
			strCont = strCont.Left(MAX_MEMO_LEN);
			nSize = MAX_MEMO_LEN;
		}
        _TCHAR *pCh = strCont.GetBuffer(0);
		for(int i = 0;i<nSize;i++)
		{
			pMemo->szContent[i] =pCh[i];
		}
        strCont.ReleaseBuffer();
		pMemo->szContent[nSize]='\0';
		pMemo->wStore = ID_STORE_PC;
		pMemo->nMemoMode = 1;
		CCoolEdit ce;
		pMemo->bUcs2 = ce.HasUcs2Char(pMemo->szContent);
		m_pIW->AddMEMO(pMemo);
	}
	::PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, 100);

	if(!arrFailedFile.IsEmpty())
	{
		CString strError;
		strError.Format(IDS_ERR_IMPORT_FAIL_BELLOW,arrFailedFile);
		AfxMessageBox(strError);
	}	
    
	UpdateMemo(LOWORD(m_dwCategID));

	CDocument * pDoc = GetDocument();
	_ASSERTE( pDoc != NULL );

	pDoc->SetModifiedFlag(TRUE);

	SetHeader(m_dwCategID);
    pMfr->EnableWindow(TRUE);
}

int  CInfoListView::CompareMemo(_MEMO * pMemoSrc,_MEMO * pMemoDst)
{
	if(pMemoSrc->memoTime.wYear != pMemoDst->memoTime.wYear)
		return TOTAL_DIFF;

	if(pMemoSrc->memoTime.btMonth != pMemoDst->memoTime.btMonth)
		return TOTAL_DIFF;

	if(pMemoSrc->memoTime.btDay != pMemoDst->memoTime.btDay)
		return TOTAL_DIFF;

	if(pMemoSrc->memoTime.btHour != pMemoDst->memoTime.btHour)
		return TOTAL_DIFF;

	if(pMemoSrc->memoTime.btMinut != pMemoDst->memoTime.btMinut)
		return TOTAL_DIFF;

	if(_tcscmp(pMemoSrc->szContent,pMemoDst->szContent)!=0)
		return TOTAL_DIFF;

	return TOTAL_SAME;


}
BOOL CInfoListView::ChangeMemoStorePlace( WORD wStore)
{
	CSMSFrame *pMfr = theApp.m_pSMSFrame;
	CWaitCursor waitCursor;
	pMfr->EnableWindow(FALSE);

//	int nPBG = m_pIW->GetPBGCount(ID_STORE_ALL);

	CDocument * pDoc = GetDocument();
	_ASSERTE( pDoc != NULL );
    
	IMemo *pMemoFactory  = theApp.m_pMemoFactory;
	
	if( !theApp.TestIsConnected(FALSE) )
	{
		AfxMessageBox( IDS_ERR_NOT_CONNECT );
		pMfr->EnableWindow(TRUE);
		return FALSE;
	}
	int nUsed = 0;
	int	nMax  = 0;
	if(ID_STORE_PC != wStore)
	{
		if( !pMemoFactory->SelectStorePlace(wStore))
		{
			AfxMessageBox(IDS_ERR_SET_MS_PLACE_FAIL);
			pMfr->EnableWindow(TRUE);
			return FALSE;
		}
		if( !pMemoFactory->GetMemoNum(&nUsed,&nMax) )
		{
		    AfxMessageBox(IDS_ERR_MEMO_GET_PHONE_INFO_FAIL);
			pMfr->EnableWindow(TRUE);
			return FALSE;
		}	
		if(nUsed>=nMax)
		{
			AfxMessageBox(IDS_ERR_MEMO_MS_FULL);
			pMfr->EnableWindow(TRUE);
			return FALSE;
		}
	}
    BOOL bRet = TRUE;
	CListCtrl *pList = NULL;
	pList =  (CListCtrl *)&GetListCtrl();
    PMEMO pMemo = NULL;
	PMEMO pNewMemo = NULL;
	int nItem = 0;
    int nProgIndex = 0;
	
	POSITION pos = pList->GetFirstSelectedItemPosition(); //iterator for the CListCtrl
	
	::PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_BEGIN,pList->GetSelectedCount());
	
    MSG msg;
	while(pos) 
	{
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		::PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_PROCEED, ++nProgIndex);

		nItem = pList->GetNextSelectedItem(pos);
		pMemo = (PMEMO)(pList->GetItemData(nItem));

		pNewMemo = new MEMO;
		memcpy(pNewMemo,pMemo,sizeof(MEMO));
		pNewMemo->wStore = wStore;
		if(ID_STORE_PHONE == wStore)
		{
			COleDateTime tPhoneCur = COleDateTime::GetCurrentTime();
			tPhoneCur -= theApp.m_tsPcPhone;

			COleDateTime tUserSet((int)pNewMemo->memoStartDate.wYear,(int)pNewMemo->memoStartDate.btMonth,(int)pNewMemo->memoStartDate.btDay,
				           (int)pNewMemo->memoTime.btHour,(int)pNewMemo->memoTime.btMinut,0);
			//差值120秒
			COleDateTimeSpan tsDel(120);

			if((tUserSet - tPhoneCur)<tsDel && pNewMemo->nMemoMode == 1) // 只提醒一次
			{
				delete pNewMemo;
				AfxMessageBox(IDS_ERR_OUT_OF_DATE);
				continue;
			}
			COleDateTimeSpan tsDel2(0);
			COleDateTime tUserSet2((int)pNewMemo->memoEndDate.wYear,(int)pNewMemo->memoEndDate.btMonth,(int)pNewMemo->memoEndDate.btDay,
				           (int)pNewMemo->memoTime.btHour,(int)pNewMemo->memoTime.btMinut,0);

			if((tUserSet - tPhoneCur)<tsDel2)
			{
				delete pNewMemo;
				AfxMessageBox(IDS_ERR_MEMO_END_DATE);
				continue;
			}

			if(!pMemoFactory->AddMemo(pNewMemo))
			{
				if( !theApp.TestIsConnected(FALSE) )
				{
					AfxMessageBox( IDS_ERR_NOT_CONNECT );
					delete pNewMemo;
					bRet = FALSE;
					break;
				}
				else
				{
					AfxMessageBox( IDS_ERR_MEMO_MODIFY_FAIL );
					delete pNewMemo;
					bRet = FALSE;
					break;
				}
			}
			else
			{
				m_pIW->AddMEMO(pNewMemo);
				nUsed++;
				if(nUsed>=nMax)
				{
					AfxMessageBox(IDS_ERR_MEMO_MS_FULL);
					bRet = FALSE;
					break;
				}
			}
		}
		else //store in pc
		{
            m_pIW->AddMEMO(pNewMemo);
		}
	} 

	::PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, 0);

	UpdateMemo(LOWORD(m_dwDragCategID));
	
	pDoc->SetModifiedFlag(TRUE);	
	SetHeader(m_dwDragCategID);

    pMfr->EnableWindow(TRUE);

	return bRet;
}