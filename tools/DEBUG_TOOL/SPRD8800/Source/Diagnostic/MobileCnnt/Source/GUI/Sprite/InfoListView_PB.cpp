#include "stdafx.h"
#include "..\MobileCnnt.h"
#include "..\MobileCnntDoc.h"
#include "InfoListView.h"
#include "GlobalDef.h"
#include "InfoWrapper.h"
#include "IPB.h"
#include "PBDlg.h"
#include "msgiddef.h"
#include "VCard.h"
#include "FileDlg.h"
#include "DlgSyncPB.h"
#include "SMSDlg.h"

static void RemoveInvalidChar(CString &str)
{
	// \ / : ? * " < > |
    str.Remove(_T('\\'));
	str.Remove(_T('/'));
	str.Remove(_T(':'));
	str.Remove(_T('?'));
	str.Remove(_T('*'));
	str.Remove(_T('"'));
	str.Remove(_T('<'));
	str.Remove(_T('>'));
	str.Remove(_T('|'));
}

void CInfoListView::UpdatePB(WORD wStore)
{
	
	CListCtrl &lst = GetListCtrl();//lint !e1514

	lst.DeleteAllItems();
	
	if( wStore & ID_STORE_PC )
		InsertPB(ID_STORE_PC);

	if( wStore & ID_STORE_PHONE )
		InsertPB(ID_STORE_PHONE);

	if( wStore & ID_STORE_SIM )
		InsertPB(ID_STORE_SIM);
	
	if( wStore & ID_STORE_SIM2 )
		InsertPB(ID_STORE_SIM2);

	if( wStore & ID_STORE_SIM3 )
		InsertPB(ID_STORE_SIM3);

	if( wStore & ID_STORE_SIM4 )
		InsertPB(ID_STORE_SIM4);

	if(lst.GetItemCount()>0 && m_nFocusItem != -1 && m_nFocusItem < lst.GetItemCount())
	{
		lst.SetItemState (m_nFocusItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		lst.EnsureVisible(m_nFocusItem, TRUE);
	}
	else
		m_nFocusItem = -1;
}

void CInfoListView::InsertPB(WORD wStore)
{
	_ASSERTE( m_pIW != NULL );

	PPB pPB    = NULL;
	m_nNoNameID =1;
	int nCount = m_pIW->GetPBCount(ID_STORE_ALL);
	for( int i = 0; i < nCount; ++i )
	{
		pPB = m_pIW->GetPB(i);
		if( pPB->wStore == wStore )
			InsertPB(pPB);
	}
}

void CInfoListView::InsertPB(_PB * pPB, BOOL bNew /* = TRUE */, int nItem /* = -1 */)
{
	_ASSERTE( pPB != NULL );

	
	CListCtrl &lst = GetListCtrl();//lint !e1514

	_ASSERTE( m_pIW != NULL );

	CString strStore;
	CString strDate;

	GetStorePlace(pPB->wStore, strStore);
	if(!theApp.m_bConnected && pPB->wStore != ID_STORE_PC )
		strStore.LoadString(IDS_DISCONNECTED);//отожн╢а╛╫с

		
	if( bNew )
	{
		nItem = lst.GetItemCount();
		VERIFY( lst.InsertItem(nItem, strStore) != -1 );
	}
	else
	{
		VERIFY( lst.SetItemText(nItem, 0, strStore) );
	}
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
				lst.SetItemText(nItem, theApp.m_bUSIM?7:6, _T("==")); 
			}
			
		}
		else
		{
			lst.SetItemText(nItem, theApp.m_bUSIM?7:6,_T("=="));
		}
	}
		
		
	lst.SetItemData(nItem, (DWORD)pPB);
}

void CInfoListView::NewPb() 
{
	// TODO: Add your command handler code here
	CPBDlg dlg(theApp.m_pSMSFrame);
	dlg.m_bNew = TRUE;

	if(LOWORD(m_dwCategID)==ID_STORE_SIM4)
	{
		dlg.m_nStore = 5;
	}
	else if(LOWORD(m_dwCategID)==ID_STORE_SIM3)
	{
		dlg.m_nStore = 4;
	}
	else if(LOWORD(m_dwCategID)==ID_STORE_SIM2)
	{
		dlg.m_nStore = 3;
	}
	else if(LOWORD(m_dwCategID)==ID_STORE_SIM)
	{
		dlg.m_nStore = 2;
	}
	else if(LOWORD(m_dwCategID)==ID_STORE_PHONE)
	{
		dlg.m_nStore = 1;
	}
	else
	{
		dlg.m_nStore = 0;
	}
	
	int nGroupNum = m_pIW->GetPBGCount(ID_STORE_ALL);
	if(nGroupNum == 0)
	{
#ifdef _EGO
		if(theApp.m_bConnected)
		{
			CWaitCursor wc;
			CSMSFrame *pMfr = theApp.m_pSMSFrame;
			pMfr->SendMessage(WM_COMMAND,ID_OPTION_LOAD_PB);
			nGroupNum = m_pIW->GetPBGCount(ID_STORE_ALL);
		}
#endif
	}

	if(nGroupNum!=0)
	{
		for(int i=0;i<nGroupNum;i++)
			dlg.m_arrGroupName.Add(m_pIW->GetPBG(i)->szName);
	}

	if( dlg.DoModal() == IDOK )
	{
		CDocument * pDoc = GetDocument();
		_ASSERTE( pDoc != NULL );
		pDoc->SetModifiedFlag(TRUE);

		if(HIWORD(m_dwCategID) == ID_CATEG_PB)
		{
			UpdatePB(LOWORD(m_dwCategID));
#ifndef SPRD8800
			if(m_pIW->GetPBGCount(ID_STORE_ALL) > 0 && dlg.m_pPB->wStore != ID_STORE_PC)
			{
				PPBGROUP pPBG = m_pIW->GetPBGByIdInPhone(dlg.m_pPB->nGroupIndex);
				if(pPBG != NULL)
				{
					pPBG->nMemberNum++;
				}		
				m_pIW->UpdateCategPBG();
				pDoc->UpdateAllViews(NULL, WM_UPDATE_GROUP_CHANGE, NULL);
			}
#endif
			
			SetHeader(m_dwCategID);
		}
	}

	this->SetFocus();
}

void CInfoListView::EditPb(_PB * pPB)
{
	_ASSERTE( pPB != NULL );

	CPBDlg dlg(theApp.m_pSMSFrame);

	dlg.m_bNew = FALSE;
	dlg.m_pPB  = pPB;
	
	UINT nOldGroupIndex = pPB->nGroupIndex;

	int nGroupNum = m_pIW->GetPBGCount(ID_STORE_ALL);
	if(nGroupNum == 0)
	{
#ifdef _EGO
		if(theApp.m_bConnected)
		{
			CWaitCursor wc;
			CSMSFrame *pMfr = theApp.m_pSMSFrame;
			pMfr->SendMessage(WM_COMMAND,ID_OPTION_LOAD_PB);
			nGroupNum = m_pIW->GetPBGCount(ID_STORE_ALL);
		}
#endif
	}
	if(nGroupNum!=0)
	{
		for(int i=0;i<nGroupNum;i++)
			dlg.m_arrGroupName.Add(m_pIW->GetPBG(i)->szName);
	}

	if( dlg.DoModal() == IDOK )
	{
// 		if(dlg.GetSafeHwnd() == NULL)
// 		{
// 			return;
// 		}	
		
		UpdatePB(LOWORD(m_dwCategID));

		CDocument * pDoc = GetDocument();
		_ASSERTE( pDoc != NULL );

		pDoc->SetModifiedFlag(TRUE);
#ifndef SPRD8800
		if(dlg.m_pPB->nGroupIndex != nOldGroupIndex && dlg.m_pPB->wStore == ID_STORE_PHONE)
		{
			PPBGROUP pPBG = m_pIW->GetPBGByIdInPhone(nOldGroupIndex);
			if(pPBG != NULL) pPBG->nMemberNum--;
		
			pPBG = m_pIW->GetPBGByIdInPhone(dlg.m_pPB->nGroupIndex);
			if(pPBG != NULL) pPBG->nMemberNum++;

			m_pIW->UpdateCategPBG();
			pDoc->UpdateAllViews(NULL, WM_UPDATE_GROUP_CHANGE, NULL);
		}
#endif		
		SetHeader(m_dwCategID);
	}

	this->SetFocus();
}

BOOL CInfoListView::DeletePb(_PB * pPB)
{
	CWaitCursor waitCursor;

	_ASSERTE( pPB != NULL && m_pIW != NULL );

	IPB * pPBFactory = theApp.m_pPBFactory;
	_ASSERTE( pPBFactory != NULL );

	if( pPB->wStore == ID_STORE_PHONE || 
		pPB->wStore == ID_STORE_SIM   || 
		pPB->wStore == ID_STORE_SIM2  ||
		pPB->wStore == ID_STORE_SIM3  ||
		pPB->wStore == ID_STORE_SIM4)
	{

		if( !pPBFactory->DeletePB(pPB) )
		{
			AfxMessageBox(IDS_ERR_DELETE);
			return FALSE;
		}
	}
	

	CDocument * pDoc = GetDocument();
	_ASSERTE( pDoc != NULL );

	pDoc->SetModifiedFlag(TRUE);	
	if(m_pIW->GetPBGCount(ID_STORE_ALL) > 0 && pPB->wStore != ID_STORE_PC)
	{
		PPBGROUP pPBG = m_pIW->GetPBGByIdInPhone(pPB->nGroupIndex);

		if(pPBG != NULL) pPBG->nMemberNum--;

		m_pIW->RemovePB(pPB);
	}
	else
	{
		m_pIW->RemovePB(pPB);
	}

	//m_nFocusItem = -1;

	SetHeader(m_dwCategID);

	this->SetFocus();

	return TRUE;
}

void CInfoListView::ExportVCard(LPCTSTR pstrDirName)
{
	BeginWaitCursor();
	CSMSFrame *pMfr = theApp.m_pSMSFrame;
	pMfr->EnableWindow(FALSE);
	
	
	CListCtrl &lst = GetListCtrl();//lint !e1514

	
	POSITION pos   = lst.GetFirstSelectedItemPosition();
	_ASSERTE( pos != NULL );

	static const nMaxText = 256;
	_TCHAR szText[nMaxText];
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
		
		CVCard vcard;
		lst.GetItemText( nItem, 1, szText, nMaxText ); 
		vcard.m_strFullName = szText;
		lst.GetItemText( nItem, 2, szText, nMaxText );
		vcard.m_strTelCellVoice = szText;
		lst.GetItemText( nItem, 3, szText, nMaxText );
		vcard.m_strTelWorkVoice = szText;
		lst.GetItemText( nItem, 4, szText, nMaxText );
		vcard.m_strTelHomeVoice = szText;	
		if(theApp.m_bUSIM)
		{
			lst.GetItemText( nItem, 5, szText, nMaxText );
			vcard.m_strTelWorkFax = szText;	

			lst.GetItemText( nItem, 6, szText, nMaxText );
			vcard.m_strEmailPrefInternet = szText;
		}
		else
		{
			lst.GetItemText( nItem, 5, szText, nMaxText );
			vcard.m_strEmailPrefInternet = szText;
		}
		CString strFileName = vcard.m_strFullName;
		RemoveInvalidChar(strFileName);
		CString str;
		str.Format(_T("%s/%s_%.3d.vcf"),pstrDirName,strFileName,nIndex);
		vcard.Save(str);
	}	
    ::PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, 0);
    pMfr->EnableWindow(TRUE);
	EndWaitCursor();

	this->SetFocus();
}
void CInfoListView::OnImportVcard() 
{
	// TODO: Add your command handler code here
	CString strTitle;
	strTitle.LoadString(IDS_SMS_TITLE);
	CString strMsg;

	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
		OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST,_T("VCard files (*.vcf) |*.vcf|All files(*.*)|*.*||"));
	CString strFiles(_T(""),256*500);
	dlg.m_ofn.lpstrFile = strFiles.GetBuffer(256*500);
	dlg.m_ofn.nMaxFile = 256*500;
	strFiles.ReleaseBuffer();
	if(dlg.DoModal() == IDCANCEL)
	{
		if(CommDlgExtendedError() == FNERR_BUFFERTOOSMALL)
		{
			strMsg.LoadString(IDS_ERR_FN_BUFFER_TOO_SMALL);		
			MessageBox(strMsg,strTitle,MB_OK|MB_ICONWARNING);
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
	CVCard vcard;
	CString strT;
	PPB pPB = NULL;
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
		if(!vcard.Parse(filename))
		{
			arrFailedFile += filename;
			arrFailedFile += _T("\n");
			continue;
		}
        pPB = new PB;
		vcard.ConvertToPB(pPB); 
		if( _tcslen(pPB->szName) != 0 ||
			_tcslen(pPB->szPhone) != 0 ||
			_tcslen(pPB->szOfficePhone) != 0 ||
			_tcslen(pPB->szHomePhone) != 0 ||
			_tcslen(pPB->szEmail) != 0 ||
			_tcslen(pPB->szFax) != 0)
		{
			m_pIW->AddPB(pPB);
		}
		else
		{
			delete pPB;
			pPB = NULL;
			arrFailedFile += filename;
			arrFailedFile += _T("\n");
			continue;
		}
	}
	::PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, 100);

	if(!arrFailedFile.IsEmpty())
	{
		strMsg.Format(IDS_ERR_IMPORT_FAIL_BELLOW,arrFailedFile);		
		MessageBox(strMsg,strTitle,MB_OK|MB_ICONWARNING);
	}
	
	UpdatePB(LOWORD(m_dwCategID));

	CDocument * pDoc = GetDocument();
	_ASSERTE( pDoc != NULL );

	pDoc->SetModifiedFlag(TRUE);

	SetHeader(m_dwCategID);
	pMfr->EnableWindow(TRUE);

	this->SetFocus();
}
void CInfoListView::OnExportToVcard() 
{
	// TODO: Add your command handler code here
    CFileDlg fDlg(FALSE);
	fDlg.SetHwnOwner(this->m_hWnd);
    
	CString strPath = fDlg.GetSelectDir();
	if(!strPath.IsEmpty())
		ExportVCard(strPath);

	this->SetFocus();
}

void CInfoListView::OnUpdateExportToVcard(CCmdUI* /*pCmdUI*/) 
{
	// TODO: Add your command update UI handler code here
	
}

void CInfoListView::OnSyncPb() 
{
	// TODO: Add your command handler code here
	CWaitCursor waitCursor;
	CDocument * pDoc = GetDocument();
	_ASSERTE( pDoc != NULL );  
	
	CString strTitle;
	strTitle.LoadString(IDS_SMS_TITLE);
	CString strMsg;

	
	
	int nMaxInBM = 0;
	int nUsedInBM = 0;
	if( !theApp.TestIsConnected(FALSE) )	
	{
		strMsg.LoadString(IDS_ERR_NOT_CONNECT);
		MessageBox(strMsg,strTitle,MB_OK|MB_ICONWARNING);
		return;
	}
	((CSMSDoc *)pDoc)->SetSyncFlag(TRUE);
	SendMessage(WM_COMMAND,ID_OPTION_LOAD_PB,0);

	((CSMSDoc *)pDoc)->SetSyncFlag(FALSE);
	
	if( !theApp.m_pPBFactory->SelectStorePlace(ID_STORE_PHONE))
	{
		strMsg.LoadString(IDS_ERR_SET_MS_PLACE_FAIL);
		MessageBox(strMsg,strTitle,MB_OK|MB_ICONWARNING);
		return;
	}
	if( !theApp.m_pPBFactory->GetPBNum(&nUsedInBM,&nMaxInBM) )
	{
		strMsg.LoadString(IDS_ERR_PB_GET_PHONE_INFO_FAIL);
		MessageBox(strMsg,strTitle,MB_OK|MB_ICONWARNING);
		return;
	}

	CDlgSyncPB dlg(theApp.m_pSMSFrame);
	_PB * pPB;
	int nCount = m_pIW->GetPBCount(ID_STORE_ALL);
	if(nCount == 0)
		return;
    int i=0,j=0;
	for(i = 0; i < nCount; i++)
	{
		pPB = m_pIW->GetPB(i);
		if(pPB->wStore == ID_STORE_PC)
			dlg.m_arrPBInPC.Add(pPB);
		else
			dlg.m_arrPBInMS.Add(pPB);
	}
	int nCountPC = dlg.m_arrPBInPC.GetSize();
	int nCountMS = dlg.m_arrPBInMS.GetSize();
	int nCmp = 0;
	int nCmpCount = 0;

	for(i=0;i<nCountPC;i++)
	{
		nCmpCount = 0;
        for(j=0;j<nCountMS;j++)
		{
			nCmp =ComparePB(dlg.m_arrPBInPC.GetAt(i),dlg.m_arrPBInMS.GetAt(j));
			if(nCmp == TOTAL_DIFF)
			{
				nCmpCount++;
				continue;
			}

			if(nCmp == TOTAL_SAME)
			{
				dlg.m_arrPBInPC.RemoveAt(i);
				dlg.m_arrPBInMS.RemoveAt(j);
				nCountMS--;
				nCountPC--;
				i--;
				j--;
				break;
			}
			if(nCmp == REPLACE_SAME)
			{
				_PB * pPB1 = dlg.m_arrPBInPC.GetAt(i);
				_PB * pPB2 = dlg.m_arrPBInMS.GetAt(j);
				dlg.m_arrPBInPC.RemoveAt(i);
				dlg.m_arrPBInMS.RemoveAt(j);
				nCountMS--;
				nCountPC--;
				i--;
				j--;	

				pPB = new PB;
				CombinePB(pPB1,pPB2,pPB);

				pPB->nGroupIndex = pPB2->nGroupIndex;

				m_pIW->RemovePB(pPB1);

				if( !theApp.TestIsConnected(FALSE) )
				{
					delete pPB;
					pPB = NULL;

					strMsg.LoadString(IDS_ERR_NOT_CONNECT);
					MessageBox(strMsg,strTitle,MB_OK|MB_ICONWARNING);					
					return;
				}
				if( !theApp.m_pPBFactory->DeletePB(pPB2) )
				{			
					delete pPB;
					pPB = NULL;
					strMsg.LoadString(IDS_ERR_DELETE);
					MessageBox(strMsg,strTitle,MB_OK|MB_ICONWARNING);	
					return;
				}
				nUsedInBM--;
				
				PPBGROUP pPBG = m_pIW->GetPBGByIdInPhone(pPB2->nGroupIndex);
				if(pPBG != NULL) pPBG->nMemberNum--;

				m_pIW->RemovePB(pPB2);

				pPB->wStore = ID_STORE_PC;
				m_pIW->AddPB(pPB);

				_PB * pPBms = new PB;
				memcpy(pPBms,pPB,sizeof(PB));
				pPBms->wStore = ID_STORE_PHONE;
				
				if( _tcslen(pPBms->szPhone) == 0 &&
					_tcslen(pPBms->szHomePhone) == 0 &&
					_tcslen(pPBms->szOfficePhone) == 0 )
				{
					_tcscpy(pPBms->szPhone,_T("0"));
				}

				if( !theApp.TestIsConnected(FALSE) )
				{
					delete pPBms;
					pPBms = NULL;
					strMsg.LoadString(IDS_ERR_NOT_CONNECT);
					MessageBox(strMsg,strTitle,MB_OK|MB_ICONWARNING);	
					return;
				}
				if( !theApp.m_pPBFactory->AddPB(pPBms) )
				{
					delete pPBms;
					pPBms = NULL;
					strMsg.LoadString(IDS_ERR_ADD_FAIL);
					MessageBox(strMsg,strTitle,MB_OK|MB_ICONWARNING);	
					return;
				}
				m_pIW->AddPB(pPBms);

				pPBG = m_pIW->GetPBGByIdInPhone(pPBms->nGroupIndex);
				if(pPBG != NULL) pPBG->nMemberNum++;
				
				nUsedInBM++;
				if(nUsedInBM >= nMaxInBM)
				{
					strMsg.LoadString(IDS_ERR_SYN_PHONE_FULL);
					MessageBox(strMsg,strTitle,MB_OK|MB_ICONWARNING);	

					UpdatePB(LOWORD(m_dwCategID));
					
					pDoc->SetModifiedFlag(TRUE);
#ifndef SPRD8800
					m_pIW->UpdateCategPBG();
					pDoc->UpdateAllViews(NULL, WM_UPDATE_GROUP_CHANGE, NULL);
#endif	
					return;
				}				
				break;		
			}
		}

		if(i==-1)         
			continue;

		if( nCmpCount>=nCountMS && nCountPC != 0 && (nCmp == TOTAL_DIFF || nCountMS == 0 ) )
		{
			if(nUsedInBM >= nMaxInBM)
			{
				strMsg.LoadString(IDS_ERR_SYN_PHONE_FULL);
				MessageBox(strMsg,strTitle,MB_OK|MB_ICONWARNING);

				UpdatePB(LOWORD(m_dwCategID));
				
				pDoc->SetModifiedFlag(TRUE);	
#ifndef SPRD8800 
				m_pIW->UpdateCategPBG();
				pDoc->UpdateAllViews(NULL, WM_UPDATE_GROUP_CHANGE, NULL);	
#endif
				return;
			}
			
			if( !theApp.TestIsConnected(FALSE) )
			{
				strMsg.LoadString(IDS_ERR_NOT_CONNECT);
				MessageBox(strMsg,strTitle,MB_OK|MB_ICONWARNING);
				return;
			}
			
			pPB = new PB;
			memcpy(pPB,dlg.m_arrPBInPC.GetAt(i),sizeof(PB));
			pPB->wStore = ID_STORE_PHONE;
			if(m_pIW->GetPBGCount(ID_STORE_ALL)>MAX_SIM_NUM)
				pPB->nGroupIndex = m_pIW->GetPBG(0)->nID;
			else
				pPB->nGroupIndex = 0;

			if( _tcslen(pPB->szPhone) == 0 &&
					_tcslen(pPB->szHomePhone) == 0 &&
					_tcslen(pPB->szOfficePhone) == 0 )
			{
				_tcscpy(pPB->szPhone,_T("0"));
			}

			if( !theApp.m_pPBFactory->AddPB(pPB) )
			{
				delete pPB;
				pPB = NULL;
				strMsg.LoadString(IDS_ERR_ADD_FAIL);
				MessageBox(strMsg,strTitle,MB_OK|MB_ICONWARNING);
				return;
			}
			m_pIW->AddPB(pPB);

			PPBGROUP pPBG = m_pIW->GetPBGByIdInPhone(pPB->nGroupIndex);
			if(pPBG != NULL) pPBG->nMemberNum++;
			dlg.m_arrPBInPC.RemoveAt(i);
			nCountPC--;
			i--;
			nUsedInBM++;

		}
	}
	nCountPC = dlg.m_arrPBInPC.GetSize();
	nCountMS = dlg.m_arrPBInMS.GetSize();
	for(i=0;i<nCountMS;i++)
	{
        for(j=0;j<nCountPC;j++)
		{
			nCmp = ComparePB(dlg.m_arrPBInMS.GetAt(i),dlg.m_arrPBInPC.GetAt(j));
			if(nCmp != TOTAL_DIFF)
				break;
		}
		if(j>=nCountPC)
		{
			pPB = new PB;
			memcpy(pPB,dlg.m_arrPBInMS.GetAt(i),sizeof(PB));
			pPB->wStore = ID_STORE_PC;
			m_pIW->AddPB(pPB);
			dlg.m_arrPBInMS.RemoveAt(i);
			nCountMS--;
			i--;
		}
	}
 	nCountPC = dlg.m_arrPBInPC.GetSize();
	nCountMS = dlg.m_arrPBInMS.GetSize();
	if(nCountPC != 0 && nCountMS!=0)
	{
		dlg.DoModal();
	}
	else if(!m_bSaveToFile)
	{
		strMsg.LoadString(IDS_SYNC_PB_SUCCESS);
		MessageBox(strMsg,strTitle,MB_OK|MB_ICONINFORMATION);
	}

	if(!m_bSaveToFile && HIWORD(m_dwCategID) == ID_CATEG_PB)
	{
		UpdatePB(LOWORD(m_dwCategID));
		SetHeader(m_dwCategID);
	}

	pDoc->SetModifiedFlag(TRUE);
#ifndef SPRD8800
	m_pIW->UpdateCategPBG();
	pDoc->UpdateAllViews(NULL, WM_UPDATE_GROUP_CHANGE, NULL);
#endif	

	this->SetFocus();	
	
}

int  CInfoListView::ComparePB(_PB * pPBSrc,_PB * pPBDst)
{
	_ASSERTE(pPBSrc!=NULL && pPBDst!=NULL);

	if(_tcscmp(pPBSrc->szName,pPBDst->szName)!=0)
		return TOTAL_DIFF;
	int n=0;

	if(_tcscmp(pPBSrc->szPhone,pPBDst->szPhone)!=0 
		&& _tcslen(pPBSrc->szPhone)!=0 
		&& _tcslen(pPBDst->szPhone)!=0 )
		return REPLACE_DIFF;
	if(_tcscmp(pPBSrc->szPhone,pPBDst->szPhone)==0 )
		n++;

	if(_tcscmp(pPBSrc->szHomePhone,pPBDst->szHomePhone)!=0 
		&& _tcslen(pPBSrc->szHomePhone)!=0 
		&& _tcslen(pPBDst->szHomePhone)!=0 )
		return REPLACE_DIFF;
	if(_tcscmp(pPBSrc->szHomePhone,pPBDst->szHomePhone)==0 )
		n++;

	if(_tcscmp(pPBSrc->szOfficePhone,pPBDst->szOfficePhone)!=0 
		&& _tcslen(pPBSrc->szOfficePhone)!=0 
		&& _tcslen(pPBDst->szOfficePhone)!=0 )
		return REPLACE_DIFF;
	if(_tcscmp(pPBSrc->szOfficePhone,pPBDst->szOfficePhone)==0 )
		n++;

	if(_tcscmp(pPBSrc->szEmail,pPBDst->szEmail)!=0 
		&& _tcslen(pPBSrc->szEmail)!=0 
		&& _tcslen(pPBDst->szEmail)!=0 )
		return REPLACE_DIFF;
	if(_tcscmp(pPBSrc->szEmail,pPBDst->szEmail)==0 )
		n++;

	if(n==4)
		return TOTAL_SAME;
	else
		return REPLACE_SAME;
}

_PB * CInfoListView::CombinePB(_PB * pPB1,_PB *pPB2,_PB * pPBDst)
{
	_ASSERTE(pPB1 != NULL && pPB2 != NULL && pPBDst != NULL);

	memcpy(pPBDst,pPB1,sizeof(_PB));

	if(_tcslen(pPB2->szPhone)!=0)
		_tcscpy(pPBDst->szPhone,pPB2->szPhone);
	if(_tcslen(pPB2->szHomePhone)!=0)
		_tcscpy(pPBDst->szHomePhone,pPB2->szHomePhone);
	if(_tcslen(pPB2->szOfficePhone)!=0)
		_tcscpy(pPBDst->szOfficePhone,pPB2->szOfficePhone);
	if(_tcslen(pPB2->szEmail)!=0)
		_tcscpy(pPBDst->szEmail,pPB2->szEmail);
	
	return pPBDst;

}

BOOL CInfoListView::ChangePBStorePlace( WORD wStore)
{
	CSMSFrame *pMfr = theApp.m_pSMSFrame;
	CWaitCursor waitCursor;
	pMfr->EnableWindow(FALSE);
//	int nPBG = m_pIW->GetPBGCount(ID_STORE_ALL);

	CDocument * pDoc = GetDocument();
	_ASSERTE( pDoc != NULL );
    	
	IPB *pPBFactory  = theApp.m_pPBFactory;
	
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
		if( !pPBFactory->SelectStorePlace(wStore))
		{
			AfxMessageBox(IDS_ERR_SET_MS_PLACE_FAIL);
			pMfr->EnableWindow(TRUE);
			return FALSE;
		}
		if( !pPBFactory->GetPBNum(&nUsed,&nMax) )
		{
			if(ID_STORE_PHONE == wStore)
				AfxMessageBox(IDS_ERR_PB_GET_PHONE_INFO_FAIL);
			else
				AfxMessageBox(IDS_ERR_PB_GET_SIM_INFO_FAIL);
			pMfr->EnableWindow(TRUE);
			return FALSE;
		}	
		if(nUsed>=nMax)
		{
			if(ID_STORE_PHONE == wStore)
				AfxMessageBox(IDS_ERR_PB_MS_FULL);
			else
				AfxMessageBox(IDS_ERR_PB_SIM_FULL);
			pMfr->EnableWindow(TRUE);
			return FALSE;
		}
	}

    BOOL bRet = TRUE;
	CListCtrl *pList = NULL;
	pList =  (CListCtrl *)&GetListCtrl();
    PPB pPB = NULL;
	PPB pNewPB = NULL;
	int nItem = 0;
	int nProgIndex = 0;
	POSITION pos = pList->GetFirstSelectedItemPosition(); //iterator for the CListCtrl

    
	::PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_BEGIN, pList->GetSelectedCount());

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
		pPB = (PPB)(pList->GetItemData(nItem));

		pNewPB = new PB;
		memcpy(pNewPB,pPB,sizeof(PB));
		pNewPB->wStore = wStore;

		if( _tcslen(pNewPB->szPhone) == 0 &&
			_tcslen(pNewPB->szHomePhone) == 0 &&
			_tcslen(pNewPB->szOfficePhone) == 0 )
		{
			_tcscpy(pNewPB->szPhone,_T("0"));
		}

		if(ID_STORE_PC != wStore)
		{
			int nNewCount = 1;
			CStringArray agNumer;
			if(wStore == ID_STORE_PHONE)
			{
				if(m_pIW->GetPBGCount(ID_STORE_ALL) > MAX_SIM_NUM)
				{
					pNewPB->nGroupIndex = m_pIW->GetPBG(0)->nID;
				}
				else
				{
					pNewPB->nGroupIndex = 0;
				}
			}
			else
			{
				if(!theApp.m_bUSIM)
				{
					memset(pNewPB->szHomePhone,0,sizeof(pNewPB->szPhone));	
					memset(pNewPB->szOfficePhone,0,sizeof(pNewPB->szOfficePhone));
					memset(pNewPB->szFax,0,sizeof(pNewPB->szFax));
					memset(pNewPB->szEmail,0,sizeof(pNewPB->szEmail));
					
					if(_tcslen(pPB->szPhone) == 0 )
					{
						nNewCount--;
					}
					else
					{
						agNumer.Add(pPB->szPhone);
					}

					if(_tcslen(pPB->szHomePhone)!=0)
					{
						agNumer.Add(pPB->szHomePhone);
						nNewCount++;
					}
					if(_tcslen(pPB->szOfficePhone)!=0)
					{
						agNumer.Add(pPB->szOfficePhone);
						nNewCount++;
					}
					if(_tcslen(pPB->szFax)!=0)
					{
						agNumer.Add(pPB->szFax);
						nNewCount++;
					}

					if( nNewCount == 0 )
					{
						_tcscpy(pNewPB->szPhone,_T("0"));
						nNewCount = 1;
					}
					else
					{
						_tcscpy(pNewPB->szPhone,agNumer.GetAt(0).operator LPCTSTR());
					}
				
				}		

				if(wStore == ID_STORE_SIM)
				{
					pNewPB->nGroupIndex = ID_PGM_SIM1;
				}
				else if(wStore == ID_STORE_SIM2)
				{
					pNewPB->nGroupIndex = ID_PGM_SIM2;
				}
				else if(wStore == ID_STORE_SIM3)
				{
					pNewPB->nGroupIndex = ID_PGM_SIM3;
				}
				else if(wStore == ID_STORE_SIM4)
				{
					pNewPB->nGroupIndex = ID_PGM_SIM4;
				}
				
			}
			for(int j=0; j< nNewCount; j++)
			{
				if( j >= 1 )
				{					
					PPB pTmp = pNewPB;
					pNewPB = new PB;
					memcpy(pNewPB,pTmp,sizeof(PB));
					_tcscpy(pNewPB->szPhone,agNumer.GetAt(j).operator LPCTSTR());						
				
				}
				if(!pPBFactory->AddPB(pNewPB))
				{
					if( !theApp.TestIsConnected(FALSE) )
					{
						AfxMessageBox( IDS_ERR_NOT_CONNECT );
						delete pNewPB;
						bRet = FALSE;
						break;
					}
					else
					{
						// try again
						if(!pPBFactory->AddPB(pNewPB))
						{
							AfxMessageBox( IDS_ERR_PB_MODIFY_FAIL );
							delete pNewPB;
							bRet = FALSE;
							break;
						}
						else
						{
							m_pIW->AddPB(pNewPB);
#ifdef _EGO
							PPBGROUP pPBG = m_pIW->GetPBGByIdInPhone(pNewPB->nGroupIndex);
							if(pPBG!=NULL) pPBG->nMemberNum++;
#endif
							nUsed++;
							if(nUsed>=nMax)
							{
								AfxMessageBox(IDS_ERR_PB_MS_FULL);
								bRet = FALSE;
								break;
							}
						}
					}
				}
				else
				{
					m_pIW->AddPB(pNewPB);
#ifdef _EGO
					PPBGROUP pPBG = m_pIW->GetPBGByIdInPhone(pNewPB->nGroupIndex);
					if(pPBG!=NULL) pPBG->nMemberNum++;			
#endif
					nUsed++;
					if(nUsed>=nMax)
					{
						AfxMessageBox(IDS_ERR_PB_MS_FULL);
						bRet = FALSE;
						break;
					}
				}
			}

			if(!bRet)
			{
				break;
			}
		}
		else //store in pc
		{
#ifdef _EGO
			if(m_pIW->GetPBGCount(ID_STORE_ALL) > MAX_SIM_NUM)
			{
				pNewPB->nGroupIndex = m_pIW->GetPBG(0)->nID;
			}
			else
			{
				pNewPB->nGroupIndex = 0;
			} 
#endif

            m_pIW->AddPB(pNewPB);
		}
	} 

	::PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, 0);

	DWORD dwCategID = m_dwCategID;	
	if(m_dwDragCategID != 0)
	{
		dwCategID= m_dwDragCategID;
	}
	UpdatePB(LOWORD(dwCategID));
	
	pDoc->SetModifiedFlag(TRUE);
#ifndef SPRD8800
	m_pIW->UpdateCategPBG();
	pDoc->UpdateAllViews(NULL, WM_UPDATE_GROUP_CHANGE, NULL);
#endif	
	
	SetHeader(dwCategID);

	pMfr->EnableWindow(TRUE);
	
	return bRet;
}
// wGroupID is the index of the ID based on zero
BOOL CInfoListView::ChangePBGroup( WORD wGroupID)
{
	CSMSFrame *pMfr = theApp.m_pSMSFrame;
	CWaitCursor waitCursor;
	pMfr->EnableWindow(FALSE);

	int nPBG = m_pIW->GetPBGCount(ID_STORE_ALL);
	//if wGroupID is SIM, return
	if( nPBG>0 && 
		( m_pIW->GetPBG(nPBG-1)->nID == ID_PGM_SIM1 ||
		  m_pIW->GetPBG(nPBG-1)->nID == ID_PGM_SIM2 || 
		  m_pIW->GetPBG(nPBG-1)->nID == ID_PGM_SIM3 ||
		  m_pIW->GetPBG(nPBG-1)->nID == ID_PGM_SIM4) )
	{
		BOOL bSIm = FALSE;
		if(!m_bFromMenu)
		{
			if(HIWORD(m_dwDragCategID) == ID_CATEG_PB && LOWORD(m_dwDragCategID) == ID_STORE_SIM )
				bSIm = TRUE;
			if(HIWORD(m_dwDragCategID) == ID_CATEG_PBGM && LOWORD(m_dwDragCategID)>>8 == nPBG )
				bSIm = TRUE;
		}
		else
		{
			if(HIWORD(m_dwCategID) == ID_CATEG_PB && LOWORD(m_dwCategID) == ID_STORE_SIM )
				bSIm = TRUE;
			if(HIWORD(m_dwCategID) == ID_CATEG_PBGM && LOWORD(m_dwCategID)>>8 == nPBG )
				bSIm = TRUE;
		}

		//if( wGroupID == (nPBG - 1) || bSIm )
		if( bSIm || 
			m_pIW->GetPBG(wGroupID)->nID == ID_PGM_SIM1 ||  
			m_pIW->GetPBG(wGroupID)->nID == ID_PGM_SIM2 ||
			m_pIW->GetPBG(wGroupID)->nID == ID_PGM_SIM3 ||
			m_pIW->GetPBG(wGroupID)->nID == ID_PGM_SIM4)
        {
			//AfxMessageBox(IDS_WARN_CHANGE_SIM_GROUP);
			CString strTitle;
			strTitle.LoadString(IDS_SMS_TITLE);
			CString strResult;
			strResult.LoadString(IDS_WARN_CHANGE_SIM_GROUP);
			MessageBox(strResult,strTitle,MB_OK|MB_ICONWARNING);

			pMfr->EnableWindow(TRUE);
      		return FALSE;
		}

	}
	CSMSDoc * pDoc = (CSMSDoc *)GetDocument();
	_ASSERTE( pDoc != NULL );
    	
	IPB *pPBFactory  = theApp.m_pPBFactory;

	if( !theApp.TestIsConnected(FALSE) )
	{
		AfxMessageBox( IDS_ERR_NOT_CONNECT );
		pMfr->EnableWindow(TRUE);
		return FALSE;
	}
	BOOL bRet = TRUE;
	CListCtrl *pList = NULL;
	pList =  (CListCtrl *)&GetListCtrl();
    PPB pPB = NULL;
	int nItem = 0;
	//get the true id in phone
	int nPBGID = 0;
	PPBGROUP pPBG = m_pIW->GetPBG(wGroupID);
	if(pPBG) nPBGID = pPBG->nID;
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
		pPB = (PPB)(pList->GetItemData(nItem));
		
		int nOldGroupIndex = pPB->nGroupIndex;
		if(nOldGroupIndex == nPBGID)
			continue;

		
		
		pPB->nGroupIndex = nPBGID;
		if(!pPBFactory->ModifyPB(pPB))
		{
			pPB->nGroupIndex= nOldGroupIndex;
			if( !theApp.TestIsConnected(FALSE) )
			{
				AfxMessageBox( IDS_ERR_NOT_CONNECT );
				bRet = FALSE;
				break;
			}
			else
			{
				AfxMessageBox( IDS_ERR_PB_MODIFY_FAIL );				
				bRet = FALSE;
				break;
			}
		}
		else
		{
			PPBGROUP pPBG = m_pIW->GetPBGByIdInPhone(nOldGroupIndex);
			if(pPBG!=NULL)	pPBG->nMemberNum--;

			pPBG = m_pIW->GetPBGByIdInPhone(pPB->nGroupIndex);
			if(pPBG!=NULL) 	pPBG->nMemberNum++;
		}
	} 
	::PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, 0);
	
	if(HIWORD(m_dwCategID)== ID_CATEG_PB)
		UpdatePB(LOWORD(m_dwCategID));
	
	pDoc->SetModifiedFlag(TRUE);
#ifndef SPRD8800
	m_pIW->UpdateCategPBG();
	pDoc->UpdateAllViews(NULL, WM_UPDATE_GROUP_CHANGE, NULL);
#endif	
	
	SetHeader(m_dwCategID);

    pMfr->EnableWindow(TRUE);

	return bRet;
}

void CInfoListView::OnOptionCallFromPB()
{
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	if(pFrm != NULL)
	{
		pFrm->SwitchToForm(IDD_FORMVIEW_CALL);
		pFrm->m_pCallView->m_strNum = m_strPhoneNum;
		pFrm->m_pCallView->UpdateData(FALSE);
	}
}

void CInfoListView::OnOptionSendSMSFromPB()
{
	CSMSDlg dlg(theApp.m_pSMSFrame);
	dlg.m_strPhoneNum = m_strPhoneNum;
	
	dlg.DoModal();
}