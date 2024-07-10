
#include "stdafx.h"
#include "..\MobileCnnt.h"
#include "InfoListView.h"
#include "..\MobileCnntDoc.h"
#include "Phone.h"
#include "InfoWrapper.h"
#include "isms.h"
#include "..\resource.h"
#include "SMSDlg.h"
#include "DlgSelSim.h"

#define PHONE_NUM_CMP_LEN 7

LPCTSTR GetStrRightNChar(LPCTSTR lpszSrc,int nChar)
{
	if(lpszSrc == NULL || nChar <= 0 )
	{
		return NULL;
	}

	int nLen = _tcslen(lpszSrc);
	if(nChar >= nLen)
		return lpszSrc;
	else
		return lpszSrc+(nLen - nChar);
}

void CInfoListView::UpdateSMS(WORD wStore)
{
	
	CListCtrl &lst = GetListCtrl();//lint !e1514

	lst.DeleteAllItems();
	_ASSERTE( m_pIW != NULL );

	// 对于每一个对应的选项，我们总是按PC、PHONE、SIM的顺序来显示

	if( wStore & ID_STORE_PC )
		InsertSMS(ID_STORE_PC);

	if( wStore & ID_STORE_PHONE )
		InsertSMS(ID_STORE_PHONE);

	if( wStore & ID_STORE_SIM )
		InsertSMS(ID_STORE_SIM);

	if( wStore & ID_STORE_SIM2 )
		InsertSMS(ID_STORE_SIM2);

	if( wStore & ID_STORE_SIM3 )
		InsertSMS(ID_STORE_SIM3);
	
	if( wStore & ID_STORE_SIM4 )
		InsertSMS(ID_STORE_SIM4);

	switch(wStore)
	{
	case ID_STORE_INBOX:
		InsertInboxSMS(ID_STORE_PHONE);
		InsertInboxSMS(ID_STORE_SIM);
		InsertInboxSMS(ID_STORE_SIM2);
		InsertInboxSMS(ID_STORE_SIM3);
		InsertInboxSMS(ID_STORE_SIM4);
		break;
	case ID_STORE_SENDS:
		InsertSendsSMS(ID_STORE_PHONE);
		InsertSendsSMS(ID_STORE_SIM);
		InsertSendsSMS(ID_STORE_SIM2);
		InsertSendsSMS(ID_STORE_SIM3);
		InsertSendsSMS(ID_STORE_SIM4);
		break;
	case ID_STORE_OUTBOX:
		InsertOutboxSMS(ID_STORE_PHONE);
		InsertOutboxSMS(ID_STORE_SIM);
		InsertOutboxSMS(ID_STORE_SIM2);
		InsertOutboxSMS(ID_STORE_SIM3);
		InsertOutboxSMS(ID_STORE_SIM4);
		break;
	case ID_STORE_DRAFTS:
		InsertDraftsSMS(ID_STORE_PHONE);
		InsertDraftsSMS(ID_STORE_SIM);
		InsertDraftsSMS(ID_STORE_SIM2);
		InsertDraftsSMS(ID_STORE_SIM3);
		InsertDraftsSMS(ID_STORE_SIM4);
		break;
	default:
		break;
	}

	if(lst.GetItemCount()>0 && m_nFocusItem != -1 && m_nFocusItem < lst.GetItemCount())
	{
		lst.SetItemState (m_nFocusItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		lst.EnsureVisible(m_nFocusItem, TRUE);
	}
	else
		m_nFocusItem = -1;
}

void CInfoListView::InsertSMS(WORD wStore)
{
	
	CListCtrl &lst = GetListCtrl();//lint !e1514

	PSMS psms   = NULL;
	int  nItem  = lst.GetItemCount();
	int  nCount = m_pIW->GetSMSCount(ID_STORE_ALL,FALSE);
	for( int i = 0; i < nCount; ++i )
	{
		psms = m_pIW->GetSMS(i);
		if( wStore == psms->wStore && psms->pPrev == NULL)
		{
			InsertSMS(nItem++, psms);
		}
	}
}

void CInfoListView::InsertSMS(int nItem, _SMS * psms)
{
	_ASSERTE( psms != NULL );
	
	
	CListCtrl &lst = GetListCtrl();//lint !e1514

	CString strStore;
	CString strStatus;

	GetStorePlace(psms->wStore, strStore);
	GetSMSStatus(psms->wStatus, strStatus);

    if(!theApp.m_bConnected && psms->wStore != ID_STORE_PC )
		strStore.LoadString(IDS_DISCONNECTED);//显现未连接
    
	lst.InsertItem(nItem, strStore);

	lst.SetItemText(nItem, 1, strStatus);
    //查电话，找名字
	BOOL bNeedSearch = TRUE;
	if(_tcslen(psms->szPhone) == 0)
		bNeedSearch = FALSE;
	lst.SetItemText(nItem, 2, psms->szName);//名字为空
	int nPBCount = m_pIW->GetPBCount(ID_STORE_ALL);
	PPB pPB = NULL;
	for(int i=0;i<nPBCount && bNeedSearch;i++)
	{
		pPB = m_pIW->GetPB(i);
		if(_tcslen(psms->szPhone)<=PHONE_NUM_CMP_LEN)
		{
			if(_tcslen(pPB->szPhone)<=PHONE_NUM_CMP_LEN )
			{
				if(_tcscmp(psms->szPhone,pPB->szPhone)==0)
				{
					lst.SetItemText(nItem, 2, pPB->szName);//设置名字
					break;
				}		
			}
			else
			{
				if(_tcscmp(psms->szPhone,GetStrRightNChar(pPB->szPhone,PHONE_NUM_CMP_LEN))==0)
				{
					lst.SetItemText(nItem, 2, pPB->szName);//设置名字
					break;
				}
			}
			if(_tcslen(pPB->szHomePhone)<=PHONE_NUM_CMP_LEN )
			{
				if(_tcscmp(psms->szPhone,pPB->szHomePhone)==0)
				{
					lst.SetItemText(nItem, 2, pPB->szName);//设置名字
					break;
				}		
			}
			else
			{
				if(_tcscmp(psms->szPhone,GetStrRightNChar(pPB->szHomePhone,PHONE_NUM_CMP_LEN))==0)
				{
					lst.SetItemText(nItem, 2, pPB->szName);//设置名字
					break;
				}
			}
			if(_tcslen(pPB->szOfficePhone)<=PHONE_NUM_CMP_LEN )
			{
				if(_tcscmp(psms->szPhone,pPB->szOfficePhone)==0)
				{
					lst.SetItemText(nItem, 2, pPB->szName);//设置名字
					break;
				}		
			}
			else
			{
				if(_tcscmp(psms->szPhone,GetStrRightNChar(pPB->szOfficePhone,PHONE_NUM_CMP_LEN))==0)
				{
					lst.SetItemText(nItem, 2, pPB->szName);//设置名字
					break;
				}
			}

	//		if(_tcscmp(psms->szPhone,pPB->szPhone)==0 || 
			//				_tcscmp(psms->szPhone,pPB->szHomePhone)==0 || 
			//				_tcscmp(psms->szPhone,pPB->szOfficePhone)==0 )
			//			{
			//				lst.SetItemText(nItem, 2, pPB->szName);//设置名字
			//				break;
			//			}
			
		}
		else
		{
			LPCTSTR lpszPhone = GetStrRightNChar(psms->szPhone,PHONE_NUM_CMP_LEN);
			if(_tcscmp(lpszPhone,GetStrRightNChar(pPB->szPhone,PHONE_NUM_CMP_LEN))==0 || 
				_tcscmp(lpszPhone,GetStrRightNChar(pPB->szHomePhone,PHONE_NUM_CMP_LEN))==0 || 
				_tcscmp(lpszPhone,GetStrRightNChar(pPB->szOfficePhone,PHONE_NUM_CMP_LEN))==0 )
			{
				lst.SetItemText(nItem, 2, pPB->szName);//设置名字
				break;
			}
		}

	}

	lst.SetItemText(nItem, 3, psms->szPhone);
	lst.SetItemText(nItem, 4, psms->szDate);
	if(psms->pNext == NULL)
		lst.SetItemText(nItem, 5, psms->szContent);
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
		lst.SetItemText(nItem, 5, strContent);
	}

	lst.SetItemData(nItem, (DWORD)psms);
}

void CInfoListView::InsertInboxSMS(WORD wStore)
{
	
	CListCtrl &lst = GetListCtrl();//lint !e1514

	PSMS psms   = NULL;
	int  nItem  = lst.GetItemCount();
	int  nCount = m_pIW->GetSMSCount(ID_STORE_ALL,FALSE);
	for( int i = 0; i < nCount; ++i )
	{
		psms = m_pIW->GetSMS(i);
		if( wStore == psms->wStore && psms->pPrev == NULL && 
			(psms->wStatus == SMS_STATUS_READ || 
			psms->wStatus  == SMS_STATUS_UNREAD) )
		{
			InsertSMS(nItem++, psms);
		}
	}
}

void CInfoListView::InsertOutboxSMS(WORD wStore)
{
	
	CListCtrl &lst = GetListCtrl();//lint !e1514

	PSMS psms   = NULL;
	int  nItem  = lst.GetItemCount();
	int  nCount = m_pIW->GetSMSCount(ID_STORE_ALL,FALSE);
	for( int i = 0; i < nCount; ++i )
	{
		psms = m_pIW->GetSMS(i);
		if( wStore == psms->wStore && psms->pPrev == NULL&& 
			psms->wStatus == SMS_STATUS_UNSEND && 
			psms->bExistSC)
		{
			InsertSMS(nItem++, psms);
		}
	}
}

void CInfoListView::InsertSendsSMS(WORD wStore)
{
	
	CListCtrl &lst = GetListCtrl();//lint !e1514

	PSMS psms   = NULL;
	int  nItem  = lst.GetItemCount();
	int  nCount = m_pIW->GetSMSCount(ID_STORE_ALL,FALSE);
	for( int i = 0; i < nCount; ++i )
	{
		psms = m_pIW->GetSMS(i);
		if( wStore == psms->wStore && psms->pPrev == NULL && 
			psms->wStatus == SMS_STATUS_SEND )
		{
			InsertSMS(nItem++, psms);
		}
	}
}

void CInfoListView::InsertDraftsSMS(WORD wStore)
{
	
	CListCtrl &lst = GetListCtrl();//lint !e1514

	PSMS psms   = NULL;
	int  nItem  = lst.GetItemCount();
	int  nCount = m_pIW->GetSMSCount(ID_STORE_ALL,FALSE);
	for( int i = 0; i < nCount; ++i )
	{
		psms = m_pIW->GetSMS(i);
		if( wStore == psms->wStore && psms->pPrev == NULL && 
			psms->wStatus == SMS_STATUS_UNSEND && 
			!psms->bExistSC)
		{
			InsertSMS(nItem++, psms);
		}
	}
}

void CInfoListView::NewSms() 
{
	// TODO: Add your command handler code here
	CSMSDlg dlg(theApp.m_pSMSFrame);
	dlg.DoModal();
	
	if(theApp.m_pSMSFrame == NULL)
	{
		return;
	}

	CDocument * pDoc = GetDocument();
	_ASSERTE( pDoc != NULL );
	pDoc->SetModifiedFlag(TRUE);

	if(HIWORD(m_dwCategID) == ID_CATEG_SMS)
	{
		UpdateSMS(LOWORD(m_dwCategID));	
		SetHeader(m_dwCategID);
	}
	
	this->SetFocus();
}

void CInfoListView::EditSms( _SMS * psms ) 
{
	// TODO: Add your command handler code here
	CSMSDlg dlg(theApp.m_pSMSFrame);
	dlg.SetEditInfo( SEM_EDIT, psms );
	dlg.DoModal();
	
	UpdateSMS(LOWORD(m_dwCategID));

	CDocument * pDoc = GetDocument();
	_ASSERTE( pDoc != NULL );

	pDoc->SetModifiedFlag(TRUE);
	
	SetHeader(m_dwCategID);

	this->SetFocus();
}

BOOL CInfoListView::DeleteSms( _SMS * psms ) 
{
	// TODO: Add your command handler code here
	CWaitCursor waitCursor;

	_ASSERTE( m_pIW != NULL && psms != NULL );

	BOOL bDel = FALSE;

    CPhone phone;
    phone.SetPort( theApp.m_pPortMain );
	_SMS * pSmsCur =NULL;
	_SMS * pSmsNext = psms->pNext;
	if( psms->wStore == ID_STORE_PHONE || 
		psms->wStore == ID_STORE_SIM   || 
		psms->wStore ==  ID_STORE_SIM2 ||
		psms->wStore ==  ID_STORE_SIM3 ||
		psms->wStore ==  ID_STORE_SIM4)
	{
		bDel = phone.DeleteSMS(psms);	
		if( bDel )
			m_pIW->RemoveSMS(psms);
		else
		{
			AfxMessageBox(IDS_ERR_DELETE);
			this->SetFocus();
			return bDel;
		}
		while(pSmsNext != NULL)
		{
            pSmsCur = pSmsNext;
			pSmsNext = pSmsCur->pNext;
			
			bDel = phone.DeleteSMS(pSmsCur);
			if( bDel )
				m_pIW->RemoveSMS(psms);
			else
			{
				pSmsCur->pPrev=NULL;
				AfxMessageBox(IDS_ERR_DELETE);
				this->SetFocus();
				return bDel;
			}
			
		}
	}
	else
	{
		m_pIW->RemoveSMS(psms);
		while(pSmsNext != NULL)
		{	
			pSmsCur = pSmsNext;
			pSmsNext = pSmsCur->pNext;

			m_pIW->RemoveSMS(pSmsCur);
		}
	}

	//m_nFocusItem = -1;

	CDocument * pDoc = GetDocument();
	_ASSERTE( pDoc != NULL );
	pDoc->SetModifiedFlag(TRUE);
    SetHeader(m_dwCategID);

	this->SetFocus();

	return TRUE;

}

void CInfoListView::OnIlmSmsSend() 
{
	// TODO: Add your command handler code here
	
	_ASSERTE( m_pIW != NULL );

	PSMS pFirstSms = (PSMS)GetFocusItemData();
	_ASSERTE( pFirstSms != NULL );

	ISMS * pSMSFactory = theApp.m_pSMSFactory;
	_ASSERTE( pSMSFactory != NULL );

  	COleDateTime odt = COleDateTime::GetCurrentTime();
	CString strDate = odt.Format(_T("%Y-%m-%d %H:%M:%S")); 
	PSMS psms = pFirstSms;
	int nTotal =0;
	int nRefNum =0;
	while(psms!=NULL)
	{
		nTotal++;
		psms = psms->pNext;
	}
    if(nTotal>1)
		nRefNum = pSMSFactory->GetUnusedRefNum();
	psms = pFirstSms;
	int nSeq=1;

//set sim if has more than one sim
	if(theApp.m_nSIMCount > 1)
	{
		CDlgSelSim dlgSelSim;
		if(dlgSelSim.DoModal() == IDCANCEL)
		{
			this->SetFocus();
			return;
		}
		else
		{
			if(dlgSelSim.m_nSim==0)
			{
				if(!pSMSFactory->SetActiveSim(SMS_SP_SIM))
				{
					AfxMessageBox(IDS_SMS_E_SET_ACTIVE_SIM);
					this->SetFocus();
					return;
				}
			}
			else if(dlgSelSim.m_nSim==1)
			{
				if(!pSMSFactory->SetActiveSim(SMS_SP_SIM2))
				{
					AfxMessageBox(IDS_SMS_E_SET_ACTIVE_SIM);
					this->SetFocus();
					return;
				}
			}
			else if(dlgSelSim.m_nSim==2)
			{
				if(!pSMSFactory->SetActiveSim(SMS_SP_SIM3))
				{
					AfxMessageBox(IDS_SMS_E_SET_ACTIVE_SIM);
					this->SetFocus();
					return;
				}
			}
			else if(dlgSelSim.m_nSim==3)
			{
				if(!pSMSFactory->SetActiveSim(SMS_SP_SIM4))
				{
					AfxMessageBox(IDS_SMS_E_SET_ACTIVE_SIM);
					this->SetFocus();
					return;
				}
			}

		}
	}
	
	if(theApp.m_nSIMState == SIM_PLUGED_ONLY_SIM1)
	{
		if(!pSMSFactory->SetActiveSim(SMS_SP_SIM))
		{
			AfxMessageBox(IDS_SMS_E_SET_ACTIVE_SIM);
			this->SetFocus();
			return;
		}
	}
	else if(theApp.m_nSIMState == SIM_PLUGED_ONLY_SIM2)
	{
		if(!pSMSFactory->SetActiveSim(SMS_SP_SIM2))
		{
			AfxMessageBox(IDS_SMS_E_SET_ACTIVE_SIM);
			this->SetFocus();
			return;
		}
	}
	else if(theApp.m_nSIMState == SIM_PLUGED_ONLY_SIM3)
	{
		if(!pSMSFactory->SetActiveSim(SMS_SP_SIM3))
		{
			AfxMessageBox(IDS_SMS_E_SET_ACTIVE_SIM);
			this->SetFocus();
			return;
		}
	}
	else if(theApp.m_nSIMState == SIM_PLUGED_ONLY_SIM4)
	{
		if(!pSMSFactory->SetActiveSim(SMS_SP_SIM4))
		{
			AfxMessageBox(IDS_SMS_E_SET_ACTIVE_SIM);
			this->SetFocus();
			return;
		}
	}

	CWaitCursor waitCursor;

	while(psms!=NULL)
	{
		SMS_INFO smsInfo;
		memset(&smsInfo,0,sizeof(smsInfo));
		_tcscpy( smsInfo.szNumber, psms->szPhone );
		smsInfo.bIsUcs2 = psms->bUcs2;
		_tcscpy(smsInfo.szContent, psms->szContent);
		smsInfo.nValidDate = psms->nValidDate;
		smsInfo.nContentLen = _tcslen(psms->szContent); 
		if(nTotal>1)
		{
			smsInfo.bUdhi	= TRUE;
			smsInfo.nTotal	= nTotal;
			smsInfo.nSeq	= nSeq;
			smsInfo.nRef	= nRefNum;
			nSeq++;
		}
		
		if( pSMSFactory->SendSMS(&smsInfo) )
		{
			// 更新状态和日期
			psms->wStatus = SMS_STATUS_SEND;
			CString strStatus;
			GetSMSStatus(psms->wStatus, strStatus);


			_tcscpy(psms->szDate, strDate);
            if(psms->pPrev == NULL)
			{
				_ASSERTE( m_nFocusItem != -1 );
				
				CListCtrl &lst = GetListCtrl();//lint !e1514
				lst.SetItemText(m_nFocusItem, 1, strStatus);
				lst.SetItemText(m_nFocusItem, 4, strDate);
			}
		}
		else
		{
			AfxMessageBox(IDS_SENDSMS_FAIL);
			this->SetFocus();
			return;
		}

		psms = psms->pNext;
	}

	AfxMessageBox(IDS_SENDSMS_OK);

	UpdateSMS(LOWORD(m_dwCategID));
	
	CDocument * pDoc = GetDocument();
	_ASSERTE( pDoc != NULL );
	
	pDoc->SetModifiedFlag(TRUE);
	
	SetHeader(m_dwCategID);

	this->SetFocus();
}

void CInfoListView::OnIlmSmsAnswer() 
{
	// TODO: Add your command handler code here
	PSMS psms = (PSMS)GetFocusItemData();
	_ASSERTE( psms != NULL );

	CSMSDlg dlg(theApp.m_pSMSFrame);
	dlg.SetEditInfo(SEM_ANSWER, psms);
	dlg.DoModal();
}

void CInfoListView::OnIlmSmsForward() 
{
	// TODO: Add your command handler code here
	PSMS psms = (PSMS)GetFocusItemData();
	_ASSERTE( psms != NULL );

	CSMSDlg dlg(theApp.m_pSMSFrame);
	dlg.SetEditInfo(SEM_FORWARD, psms);
	dlg.DoModal();

	this->SetFocus();
}