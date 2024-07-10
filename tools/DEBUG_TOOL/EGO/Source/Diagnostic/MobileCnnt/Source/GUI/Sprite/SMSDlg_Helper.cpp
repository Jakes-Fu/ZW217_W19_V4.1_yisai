#include "stdafx.h"
#include "..\mobilecnnt.h"
#include "SMSDlg.h"
#include "SMSPresetDlg.h"
#include "InfoWrapper.h"
#include "InfoListView.h"
#include "GlobalDef.h"
#include "isms.h"
#include "..\MobileCnntDoc.h"
#include "MsgIDDef.h"

BOOL CSMSDlg::StrIsNumeric(LPCTSTR pszStr)
{
	_ASSERTE( pszStr != NULL );

	return StrIsNumeric( pszStr, _tcslen(pszStr) );
}

BOOL CSMSDlg::StrIsNumeric(LPCTSTR pszStr, int nLen)
{
	_ASSERTE( pszStr != NULL && nLen > 0 );

	for( int i = 0; i < nLen; ++i, ++pszStr)
	{
		if( (*pszStr != '+') && (*pszStr < '0' || *pszStr > '9') )
			return FALSE;
	}

	return TRUE;
}

BOOL CSMSDlg::StrIsValidate(LPCTSTR pszStr)
{
	_ASSERTE( pszStr != NULL );

	if( !StrIsNumeric(pszStr) )
	{
		CString strValue;
		if( !m_mapNameToPhone.Lookup(pszStr, strValue) )
		{
			VERIFY( strValue.LoadString(IDS_INVALIDATE) );
			
			CString strPrompt;
			strPrompt.Format( strValue, pszStr );
			AfxMessageBox(strPrompt);
			return FALSE;
		}
	}

	return TRUE;
}

int CSMSDlg::GetFullCharCount( LPCTSTR pszBuf )
{
    int     nCount  = 0;
    LPCTSTR pszNext = NULL;
    while( 1 )
    {
        pszNext = CharNext(pszBuf);
		//If pszBuf points to the terminating null character, 
		//the return value is equal to pszBuf.
        if( pszNext == pszBuf ) 
        {
            break;
        }
        pszBuf = pszNext;
        ++nCount;
    } 
    return nCount;
}

BOOL CSMSDlg::EmptyPrompt(UINT nID)
{
	CWnd * pWnd = GetDlgItem(nID);
	_ASSERTE( pWnd != NULL );
	
	CString strText;
	pWnd->GetWindowText(strText);
	strText.TrimRight(_T(':'));
	
	CString strFormat;
	VERIFY( strFormat.LoadString(IDS_PROMPT_EMPTY) );
	
	CString strPrompt;
	strPrompt.Format( strFormat, strText );

	CString strTitle;
	this->GetWindowText(strTitle);
	MessageBox(strPrompt,strTitle,MB_OK|MB_ICONWARNING);	
//	AfxMessageBox(strPrompt);

	return FALSE;
}

void CSMSDlg::SetEditInfo( int nEditMode, _SMS * psms )
{
	_ASSERTE( psms != NULL );

	m_EditMode = (SMS_EDIT_MODE)nEditMode;
	m_pSMS     = psms;
}

BOOL CSMSDlg::SendSMS(LPCTSTR pszPhoneNum, BOOL bMore)
{
	CWaitCursor wc;
	CComboBox * pCmb = (CComboBox *)GetDlgItem(IDC_CMB_SEND_VALID_DATE);
	_ASSERTE( pCmb != NULL );

	int nItem = m_arrContent.GetSize();
	int nRefNum=0;
	CString  strContent;
// 	BOOL bMore = FALSE;	
	if(nItem>1)
	{
#ifdef SPRD8800
		//teana hu 2009.05.13
		m_pSMSFactory->SendLongSmsPreSet("2");
#endif
//		bMore = TRUE;
		nRefNum = m_pSMSFactory->GetUnusedRefNum();
	}
	CSMSFrame *pMfr = theApp.m_pSMSFrame;
	CCoolStatusBar *pSb = &pMfr->m_wndStatusBar;
	CString strSTB;
	
	::PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_BEGIN,nItem);
	MSG msg;
	SMS_INFO smsInfo;
	
    for(int i = 0; i< nItem || nItem==0;i++)
	{
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);	
		}

		wc.Restore();
		
		::PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_PROCEED, i+1);

#ifndef SPRD8800 
		if(i!=0)
			Sleep(5000);
#endif	
		wc.Restore();
		if(i == nItem - 1)
		{
#ifdef SPRD8800
		//teana hu 2009.05.13
		m_pSMSFactory->SendLongSmsPreSet("0");
#endif
		}

		//PSMS_INFO pSmsInfo = new SMS_INFO;
		//if(pSmsInfo==NULL)
		//{
		//	::PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, 0);
		//	return FALSE;
		//}
		//memset(pSmsInfo,0,sizeof(SMS_INFO));
		ZeroMemory(&smsInfo,sizeof(SMS_INFO));

		smsInfo.nValidDate = pCmb->GetItemData( pCmb->GetCurSel() );
		_tcscpy(smsInfo.szNumber, pszPhoneNum);
		
		if(nItem!=0)
		{
			smsInfo.bIsUcs2 = m_edtContent.HasUcs2Char();
			
			strContent = m_arrContent.GetAt(i);
			_tcscpy(smsInfo.szContent, strContent);
			
			
			smsInfo.nContentLen = strContent.GetLength();
		}

		if(nItem>1)
		{
			smsInfo.bUdhi  = TRUE;
			smsInfo.nTotal = nItem;
			smsInfo.nSeq   = i+1;
			smsInfo.nRef   = nRefNum;
		}
#ifdef SPRD8800
		if(bMore == FALSE)
		{
			if(i < nItem - 1)
			{
				bMore = TRUE;
			}
		}
#endif
		

		_ASSERTE( m_pIW != NULL );
//        if(i>0 && i==nItem-1)
//			bMore = FALSE;
		//for text mode
		//if(m_bStateReport)
		//	m_pSMSFactory->SetSRR(TRUE);
		//for pdu mode
		if(m_bStateReport)
			smsInfo.bRSS = TRUE;
		if( !m_pSMSFactory->SendSMS(&smsInfo,bMore) )
		{
			//重发三次
			BOOL bAgain = FALSE;
//			for(int n=0;n<3;n++)
//			{
//				strSTB.Format(_T("[%d]==>>[%d]"),i+1,n+1);
//				pSb->SetPaneText(0,strSTB);
//
//				while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
//				{
//					TranslateMessage(&msg);
//					DispatchMessage(&msg);	
//				}
//				
//				if(theApp.TestIsConnected(FALSE) &&
//					m_pSMSFactory->SendSMS(&smsInfo,FALSE))
//				{
//					bAgain = TRUE;
//					break;
//				}
//			}

			if(!bAgain)
			{
				//AfxMessageBox(IDS_SENDSMS_FAIL);
				CString strTitle;
				this->GetWindowText(strTitle);
				CString strPrompt;
				strPrompt.LoadString(IDS_SENDSMS_FAIL);
				MessageBox(strPrompt,strTitle,MB_OK|MB_ICONWARNING);
				wc.Restore();

				::PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, 0);
				return FALSE;
			}
		}
		if(nItem !=0)
		{
			strSTB.Format(_T("[%d/%d]"),i+1,nItem);
			pSb->SetPaneText(0,strSTB);
		}
		else
		{
			strSTB.Format(_T("[%d/%d]"),i+1,nItem+1);
			pSb->SetPaneText(0,strSTB);
			break;
		}

	}

//	if(nItem > 1)
//	{
//#ifdef SPRD8800
//		//teana hu 2009.05.13
//		m_pSMSFactory->SendLongSmsPreSet("0");
//#endif
//	}
    ::PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, 0);
//	if(!bMore)
	{
		//AfxMessageBox(IDS_SENDSMS_OK);
		CString strTitle;
		this->GetWindowText(strTitle);
		CString strPrompt;
		strPrompt.LoadString(IDS_SENDSMS_OK);
		MessageBox(strPrompt,strTitle,MB_OK|MB_ICONINFORMATION);
		wc.Restore();
	}
	
	return TRUE;
}

BOOL CSMSDlg::SaveSMS(LPCTSTR pszName, LPCTSTR pszPhoneNum, BOOL bSend)
{
//	_ASSERTE( pszPhoneNum != NULL && _tcslen(pszPhoneNum) > 0 );

	// !!!! 由于受底层接口限制，目前只能保存在计算机 !!!!
    
	WORD wStore = ID_STORE_PC;

	WORD wStatus = bSend ? SMS_STATUS_SEND : SMS_STATUS_UNSEND;
	BOOL bNew    = FALSE;
	PSMS pSMS    = m_pSMS;

	if( NULL == pSMS || (pSMS->wStore != ID_STORE_PC && wStore == ID_STORE_PC) )
	{
		bNew = TRUE;
	}

	CComboBox * pCmb = (CComboBox *)GetDlgItem(IDC_CMB_SEND_VALID_DATE);
	_ASSERTE( pCmb != NULL );

	COleDateTime odt = COleDateTime::GetCurrentTime();

	CString strDate = odt.Format(_T("%Y-%m-%d %H:%M:%S"));

	int nItem = m_arrContent.GetSize();
	PSMS pPrev = NULL;
	if(bNew)
	{
		for(int i=0;i<nItem;i++)
		{
			pSMS = new SMS;
			if( NULL == pSMS )
			{
				AfxMessageBox(IDS_ERR_RES_NOENOUGH);
				return FALSE;
			}

			pSMS->nValidDate = pCmb->GetItemData( pCmb->GetCurSel() );
			pSMS->wStore  = wStore;
			pSMS->wStatus = wStatus;
			_tcscpy(pSMS->szName, pszName);
			_tcscpy(pSMS->szPhone, pszPhoneNum);
			_tcscpy(pSMS->szDate, strDate);
			_tcscpy(pSMS->szContent, m_arrContent.GetAt(i));
			pSMS->bUcs2 = m_edtContent.HasUcs2Char();

			pSMS->pPrev = pPrev;
			pSMS->pNext = NULL;
			if(pPrev != NULL)
			   pPrev->pNext = pSMS;
			pPrev = pSMS;

			_ASSERTE( m_pIW != NULL );
			if( bNew )
				m_pIW->AddSMS( pSMS );	

		}
		return TRUE;
	}
    
	int nItemOld=0;
	while(pSMS != NULL)
	{
		++nItemOld;
		pSMS = pSMS->pNext;
	}
    pSMS  = m_pSMS;
	for(int i=0;i<nItem;i++)
	{
		if(pSMS == NULL)
		{
			bNew = TRUE;
			pSMS = new SMS;
			if( NULL == pSMS )
			{
				AfxMessageBox(IDS_ERR_RES_NOENOUGH);
				return FALSE;
			}
		}

		pSMS->nValidDate = pCmb->GetItemData( pCmb->GetCurSel() );
		pSMS->wStore  = wStore;
		pSMS->wStatus = wStatus;
		_tcscpy(pSMS->szName, pszName);
		_tcscpy(pSMS->szPhone, pszPhoneNum);
		_tcscpy(pSMS->szDate, strDate);
		_tcscpy(pSMS->szContent, m_arrContent.GetAt(i));
		pSMS->bUcs2 = m_edtContent.HasUcs2Char();

		pSMS->pPrev = pPrev;

		if(bNew)
		   pSMS->pNext = NULL;
		if(pPrev != NULL)
		   pPrev->pNext = pSMS;
		pPrev = pSMS;

		if(!bNew)
			pSMS = pSMS->pNext;

		_ASSERTE( m_pIW != NULL );
		if( bNew )
			m_pIW->AddSMS( pSMS );
	}
    PSMS pNext = NULL;
    if(nItemOld>nItem)
	{
		pPrev->pNext = NULL;
		while(pSMS != NULL)
		{
			pNext = pSMS->pNext;
			m_pIW->RemoveSMS(pSMS);
			pSMS = pNext;
		}
	}
//	CInfoListView *pInfoLV = (CInfoListView *)GetParent();
//	CMobileSpriteDoc *pDoc =(CMobileSpriteDoc *)pInfoLV->GetDocument();
//	pDoc->UpdateAllViews(pInfoLV,WM_UPDATE_ITEM_CONTENT,NULL);
//	pDoc->UpdateAllViews(pInfoLV, WM_CATEGORY_CHANGE, (CObject*)ID_CATEG_SMS);

	return TRUE;
}