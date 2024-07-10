#include "stdafx.h"
#include "MobileCnnt.h"

#include "SMSDoc.h"
#include "InfoWrapper.h"
#include "SMSCenterDlg.h"
#include "SMSPresetDlg.h"

#include "IPB.h"
#include "IPBG.h"
#include "IMemo.h"

#include "GlobalDef.h"
#include "msgiddef.h"
#include "MainFrm.h"

#include "ProgThread.h"
#include "SMSSendModeDlg.h"
#include "DlgSMSStore.h"
#include "DlgPBType.h"


typedef struct _TH_PARAM_T
{
	CSMSDoc * pDoc;
	CMobileCnntApp * pApp;
	CSMSFrame       * pMfr;
	CInfoWrapper     * pInf;
	SMS_STORE_PLACE_E  eStore;
	int                nSmsCount;
}TH_PARAM,*TH_PARAM_PTR;


extern UINT g_uDlgFlag;

void CSMSDoc::OnOptionLoadMSAll() 
{
	// TODO: Add your command handler code here
	//////////////////////////////////////////////////////////////////////////
	//debug
//	return;
	//end
	
	MSG msg;

	CSMSFrame *pMfr = theApp.m_pSMSFrame;

	CWaitCursor waitCursor;
	pMfr->EnableWindow(FALSE);

	m_bLoadAll = theApp.TestIsConnected();
	CString strResult;
	strResult.Format(IDS_LOAD_COMPLETE,_T(""));
	if( m_bLoadAll )
	{
		if(theApp.m_bConnected)
 			OnOptionLoadPBG();
 		if(theApp.m_bConnected)
    		OnOptionLoadPb();
		if(theApp.m_bConnected && theApp.m_bSupportMemo)
 			OnOptionLoadMemo();
 		if(theApp.m_bConnected)
 			OnOptionLoadSms();

	}

	m_bLoadAll = FALSE;

    pMfr->EnableWindow(TRUE);

	while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	if(theApp.m_bConnected)
	{		
		//AfxMessageBox(strResult);
		CString strTitle;
		strTitle.LoadString(IDS_SMS_TITLE);
		MessageBox(theApp.m_pSMSFrame->m_hWnd,strResult,strTitle,MB_OK|MB_ICONINFORMATION);
	}
}

void CSMSDoc::OnOptionLoadSms() 
{
	// TODO: Add your command handler code here
	CWaitCursor waitCursor;

    CSMSFrame *pMfr = theApp.m_pSMSFrame;
	CCoolStatusBar *pSb = &pMfr->m_wndStatusBar;
	CString strSTB;
	CString strOldSTB;
	strSTB.LoadString(IDS_STATUS_SMS_START);
	strOldSTB = pSb->GetPaneText(0);
	pSb->SetPaneText(0,strSTB);

	CMobileCnntApp * pApp = (CMobileCnntApp *)AfxGetApp();
	_ASSERTE( pApp != NULL );

	if( !pApp->TestIsConnected() )
	{
		pSb->SetPaneText(0,strOldSTB);
		return;
	}

	pMfr->EnableWindow(FALSE);

	CInfoWrapper * pIW = pApp->GetInfoWrapper();
	_ASSERTE( pIW != NULL );

	pIW->RemovePhoneSIMSMS();
	
	UpdateAllViews(NULL, WM_CATEGORY_CHANGE, (CObject*)m_dwCateg);

    ISMS * pSMS = pApp->m_pSMSFactory;
    _ASSERTE( pSMS != NULL );
    
    if( !pSMS->SetSMSMode(SMS_MODE_PDU) )
    {
        AfxMessageBox(IDS_ERR_SMSMODE);
		pSb->SetPaneText(0,strOldSTB);
		pMfr->EnableWindow(TRUE);
        return;
    }

#ifdef SPRD8800
	//teana hu 2009.08.19
	int nTotalSMS = 0;
	int nUsedSMS = 0;
	pSMS->QueryCapacity( nTotalSMS, nUsedSMS );
#endif
	//

	//pSMS->SetUIThreadForProgress(m_pProgThread);
	pSMS->SetWndForProgress(pMfr->m_hWnd);

    CString strPlace;
	BOOL bRead = FALSE;
	MSG msg;
	CString strProgTitle;
#ifdef SPRD8800
	static const WORD wStoreNum = 1;
	SMS_STORE_PLACE_E arrStore[wStoreNum] = { SMS_SP_SIM};
#else
	static const WORD wStoreNum = 5;
	SMS_STORE_PLACE_E arrStore[wStoreNum] = { SMS_SP_MS,SMS_SP_SIM,SMS_SP_SIM2,SMS_SP_SIM3,SMS_SP_SIM4 };
#endif
 	int nCount = 0;
	BOOL arrEmpty[wStoreNum]= {FALSE};

	CString strErrorPlace = _T("");

	BOOL bFailed = FALSE;

	for( WORD wStore = 0; wStore < wStoreNum && theApp.m_bConnected; ++wStore )
	{
		if(arrStore[wStore] == SMS_SP_SIM && !theApp.IsPluggedSIM(1))
		{
			continue;
		}
		else if(arrStore[wStore] == SMS_SP_SIM2 && !theApp.IsPluggedSIM(2))
		{
			continue;
		}
		else if(arrStore[wStore] == SMS_SP_SIM3 && !theApp.IsPluggedSIM(3))
		{
			continue;
		}
		else if(arrStore[wStore] == SMS_SP_SIM4 && !theApp.IsPluggedSIM(4))
		{
			continue;
		}
	

		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);	
		}

#ifdef SPRD8800
		if(wStore == 0 )
		{
			// @hongliang.xin 2011-3-9 
// 			if( theApp.m_nMMIVer== MMI_VERSION_2 && theApp.m_nSIMState == SIM_PLUGED_BOTH &&
// 				!pSMS->SetActiveSim(SMS_SP_SIM) )
// 			{
// 				continue;
// 			}
			strPlace.LoadString(IDS_STORE_SIM);			
		}
#else
		if(wStore ==  0)//Phone
		{
			strPlace.LoadString(IDS_STORE_PHONE);			
		}
		else if(wStore == 1 )
		{
			// @hongliang.xin 2011-3-9 
			if( theApp.m_nMMIVer== MMI_VERSION_2 &&
				theApp.m_nSIMCount > 1 &&
				!pSMS->SetActiveSim(SMS_SP_SIM) )
			{
				continue;
			}
			strPlace.LoadString(IDS_STORE_SIM);			
		}
		else if(wStore == 2)
		{
			if(theApp.m_nMMIVer== MMI_VERSION_2 && !pSMS->SetActiveSim(SMS_SP_SIM2))
			{
				continue;
			}

			strPlace.LoadString(IDS_STORE_SIM2);
		}
		else if(wStore == 3)
		{
			if(theApp.m_nMMIVer== MMI_VERSION_2 && !pSMS->SetActiveSim(SMS_SP_SIM3))
			{
				continue;
			}

			strPlace.LoadString(IDS_STORE_SIM3);
		}
		else if(wStore == 4)
		{
			if(theApp.m_nMMIVer== MMI_VERSION_2 && !pSMS->SetActiveSim(SMS_SP_SIM4))
			{
				continue;
			}

			strPlace.LoadString(IDS_STORE_SIM4);
		}
#endif		
	
		strProgTitle.Format(IDS_PROG_TITLE_SMS,strPlace);
		pSb->SetPaneText(0,strProgTitle);
		TRACE(_T("SMS in %s:%s\n"),strPlace,strProgTitle);	
			

		if( pSMS->SelectStorePlace(arrStore[wStore]) && 
			pSMS->ReadAllSMS(nCount) )
		{
			if(nCount == 0)
			{
				arrEmpty[wStore] = TRUE;
			}
			while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);	
			}
			
			PSMS      pNewSms = NULL;
			PSMS_INFO pGetSms = NULL;

			//标识短信是否处理过，主要应付长短信的合并
			int *pFlag = (int *) new int[nCount];

			if(pFlag == NULL)
			{
				AfxMessageBox(IDS_ERR_RES_NOENOUGH);
				PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, 0);
				pSb->SetPaneText(0,strOldSTB);
                pMfr->EnableWindow(TRUE);	
				return;
			}
			//标识：0，没有处理过；1,处理过
			memset(pFlag,0,sizeof(int)*nCount);
			CString strTitle = "sms";
			for( int i = 0; i < nCount && theApp.m_bConnected; ++i )
			{				
				while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);	
				}
				

				if(pFlag[i] != 0)//处理过
					continue;

				pGetSms = pSMS->GetSMS(i);				
				_ASSERTE( NULL != pGetSms );

				pFlag[i]=1;
				if(pGetSms->bUdhi && pGetSms->nTotal>1)
				{
					if(LoadSplishSMS(pGetSms,pFlag,nCount))
						continue;
				}
				
				pNewSms = new SMS;
				if( pNewSms == NULL )
				{
					AfxMessageBox(IDS_ERR_RES_NOENOUGH);
					pSb->SetPaneText(0,strOldSTB);
					PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, 0);
					pMfr->EnableWindow(TRUE);
					return;
				}
				
                pNewSms->nIdx = pGetSms->nIndex;

				
				TRACE(_T("-------------------Read %d    ID:%d\n"),i,pNewSms->nIdx);
				
                switch( pGetSms->eStorePlace )
                {
                case SMS_SP_MS:
                    pNewSms->wStore = ID_STORE_PHONE;
                    break;
                case SMS_SP_SIM:
                    pNewSms->wStore = ID_STORE_SIM;
                    break;
				case SMS_SP_SIM2:
                    pNewSms->wStore = ID_STORE_SIM2;
                    break;
				case SMS_SP_SIM3:
                    pNewSms->wStore = ID_STORE_SIM3;
                    break;
				case SMS_SP_SIM4:
                    pNewSms->wStore = ID_STORE_SIM4;
                    break;
                default:
                    _ASSERTE( 0 );
                }

                switch( pGetSms->eStatus )
                {
                case SMS_STAT_UNREAD:
                    pNewSms->wStatus = SMS_STATUS_UNREAD;
                    break;
                case SMS_STAT_READ:
                    pNewSms->wStatus = SMS_STATUS_READ;
                    break;
                case SMS_STAT_UNSEND:					
                    pNewSms->wStatus = SMS_STATUS_UNSEND;		
                    break;
                case SMS_STAT_SEND:
                    pNewSms->wStatus = SMS_STATUS_SEND;
                    break;
                default:
                    _ASSERTE( 0 );
                }
				pNewSms->bUcs2 = pGetSms->bIsUcs2;
				pNewSms->bExistSC = pGetSms->bExistSC;
                _tcscpy(pNewSms->szPhone,   pGetSms->szNumber);
                _tcscpy(pNewSms->szDate,    pGetSms->szDate);
                _tcscpy(pNewSms->szContent, pGetSms->szContent );
                //[Chinese]判断是否为状态报告,若是，则短信的内容为状态报告代码
				if(pGetSms->bRSS && _tcslen(pGetSms->szContent)==1)
				{
					int nSRP = _ttoi(pGetSms->szContent);
					if(nSRP >= SMS_RECEIVED_BY_SME && nSRP <= SMS_INVALID_STATUS_REPORT)
					{
						CString str;
						str.LoadString(IDS_SMS_SRP_RECEIVED_BY_SME + nSRP);
						_tcscpy(pNewSms->szContent, str );
					}
				}			

				pIW->AddSMS(pNewSms);
				bRead = TRUE;

				while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);	
				}
			}

		
			while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END,0);	
			
#ifdef SPRD8800
			if(wStore == 0)
			{
				strPlace.LoadString(IDS_STORE_SIM);			
			}
#else
			if(wStore ==  0)//Phone
			{
				strPlace.LoadString(IDS_STORE_PHONE);			
			}
			else if(wStore == 1)
			{
				strPlace.LoadString(IDS_STORE_SIM);			
			}
			else if(wStore == 2)
			{
				strPlace.LoadString(IDS_STORE_SIM2);
			}
			else if(wStore == 3)
			{
				strPlace.LoadString(IDS_STORE_SIM3);
			}
			else if(wStore == 4)
			{
				strPlace.LoadString(IDS_STORE_SIM4);
			}
#endif			

			strProgTitle.Format(IDS_PROG_TITLE_SMS_COMPLETE,strPlace);
			pSb->SetPaneText(0,strProgTitle);
			TRACE(_T("SMS in %s:%s\n"),strPlace,strProgTitle);				

			delete [] pFlag;
			
		}
		else
		{
			bFailed = TRUE;
			strErrorPlace += strPlace;
			strErrorPlace += _T(",");
			//AfxMessageBox(IDS_LOAD_FAILED);
			//pSb->SetPaneText(0,strOldSTB);
			PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, 0);
			//pMfr->EnableWindow(TRUE);
			//return;
		}
	}

	if(bFailed)
	{
		strErrorPlace.TrimRight(_T(','));
		CString strPrt;
		CString strCat;
		strCat.LoadString(IDS_CATEG_SMS);
		strPrt.Format(IDS_LOAD_FAILED,strCat,strErrorPlace);
		AfxMessageBox(strPrt);

	}
	else
	{
#ifdef SPRD8800
		//teana hu 2009.08.19
		if(nTotalSMS == nUsedSMS)
		{
			CString strTmp;
			strTmp.LoadString(IDS_FULL_SMS_PROMPT);
			MessageBox(theApp.m_pSMSFrame->GetSafeHwnd(),strTmp,_T("SMS"),MB_OK|MB_ICONWARNING);
		}
		pIW->SetSMSTotalCount(ID_CATEG_SMS, nTotalSMS, nUsedSMS);
		//
#endif
	}

	strSTB.LoadString(IDS_STATUS_SMS_END);	
	pSb->SetPaneText(0,strSTB);
	while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	SetModifiedFlag(TRUE);

	if( bRead && HIWORD(m_dwCateg) == ID_CATEG_SMS )
	{
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		UpdateAllViews(NULL, WM_CATEGORY_CHANGE, (CObject*)m_dwCateg);

		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	CString strResult;
	if(arrEmpty[0]== TRUE && arrEmpty[1] == TRUE)
	{
		CString str;
		str.LoadString(IDS_EMPTY_RECORD);
		strResult.Format(IDS_LOAD_COMPLETE,str);
	}
	else
	{
		strResult.Format(IDS_LOAD_COMPLETE,_T(""));
	}	

	if(!m_bLoadAll)
	{
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		pMfr->EnableWindow(TRUE);

		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(theApp.m_bConnected)
		{
			//AfxMessageBox(strResult);
			CString strTitle;
			strTitle.LoadString(IDS_SMS_TITLE);
			MessageBox(theApp.m_pSMSFrame->m_hWnd,strResult,strTitle,MB_OK|MB_ICONINFORMATION);
		}
	}
	
	UpdateAllViews(NULL, WM_CATEGORY_CHANGE, (CObject*)m_dwCateg);

	if(!theApp.m_bConnected)
		AfxMessageBox(IDS_ERR_NOT_CONNECT);

}

void CSMSDoc::OnOptionLoadPb() 
{
	// TODO: Add your command handler code here
	CWaitCursor waitCursor;

    CSMSFrame *pMfr = theApp.m_pSMSFrame;
	CCoolStatusBar *pSb = &pMfr->m_wndStatusBar;
	CString strSTB;
	CString strOldSTB;
	strSTB.LoadString(IDS_STATUS_PB_START);
	strOldSTB = pSb->GetPaneText(0);
	pSb->SetPaneText(0,strSTB);

	CMobileCnntApp * pApp = (CMobileCnntApp *)AfxGetApp();
	_ASSERTE( pApp != NULL );

	if( !pApp->TestIsConnected() )
	{
		pSb->SetPaneText(0,strOldSTB);
		return;
	}

	pMfr->EnableWindow(FALSE);
	
    if( !m_bLoadAll)
	{
		m_bLoadPB = TRUE;
#ifndef SPRD8800
		OnOptionLoadPBG();
#endif
		m_bLoadPB = FALSE;
	}

	CInfoWrapper * pIW = pApp->GetInfoWrapper();
	_ASSERTE( pIW != NULL );

	pIW->RemovePhoneSIMPB();

//	int nPBGMNum = pIW->GetPBGCount(ID_STORE_ALL);

	IPB * pPBFactory = pApp->m_pPBFactory;
	_ASSERTE( pPBFactory != NULL );

	BOOL bRead = FALSE;
	MSG msg;
	CString strProgTitle;
#ifdef SPRD8800
	static const WORD wStoreNum = 1;
	WORD arrStore[wStoreNum] = { ID_STORE_SIM};
#else
	static const WORD wStoreNum = 5;
	WORD arrStore[wStoreNum] = { ID_STORE_PHONE,ID_STORE_SIM,ID_STORE_SIM2,ID_STORE_SIM3, ID_STORE_SIM4 };
#endif
	
    BOOL arrEmpty[wStoreNum] = {FALSE};
	int nUsed, nTatol;
	CString strPlace;

	//teana hu 2009-02-23
	BOOL bFailed = FALSE;
	CString strErrorPlace = _T("");

	for( WORD wStore = 0; wStore < wStoreNum && theApp.m_bConnected; ++wStore )
	{
		if(arrStore[wStore] == ID_STORE_SIM && !theApp.IsPluggedSIM(1))
		{
			continue;
		}
		else if(arrStore[wStore] == ID_STORE_SIM2 && !theApp.IsPluggedSIM(2))
		{
			continue;
		}
		else if(arrStore[wStore] == ID_STORE_SIM3 && !theApp.IsPluggedSIM(3))
		{
			continue;
		}
		else if(arrStore[wStore] == ID_STORE_SIM4 && !theApp.IsPluggedSIM(4))
		{
			continue;
		}
		
		//设置状态栏			
		strPlace = _T("");
#ifdef SPRD8800
		if(wStore ==  0)
		{
			strPlace.LoadString(IDS_STORE_SIM);				
 		}
#else
		if(wStore ==  0) //Phone 
		{
			strPlace.LoadString(IDS_STORE_PHONE);				
 		}
		else if(wStore ==  1)
		{
			strPlace.LoadString(IDS_STORE_SIM);				
 		}
		else if(wStore ==  2)
		{
			strPlace.LoadString(IDS_STORE_SIM2);
		}
		else if(wStore ==  3)
		{
			strPlace.LoadString(IDS_STORE_SIM3);
		}
		else if(wStore ==  4)
		{
			strPlace.LoadString(IDS_STORE_SIM4);
		}
#endif

		if( pPBFactory->SelectStorePlace(arrStore[wStore]) && 
			pPBFactory->GetPBNum(&nUsed, &nTatol, TRUE) )
		{ 
			if(nUsed == 0)
			{
				arrEmpty[wStore] = TRUE;
			}
						
			while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			
			if(nUsed > 0)
				PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_BEGIN,nUsed);
			else
				PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_BEGIN,100);
			
			

			strProgTitle.Format(IDS_PROG_TITLE_PB,strPlace);
			TRACE(_T("PB in %s:%s\n"),strPlace,strProgTitle);
			pSb->SetPaneText(0,strProgTitle); 			

			PPB pNewPb = NULL;
			PPB pGetPb = NULL;
			for( int i = 0; i < nUsed && theApp.m_bConnected; ++i )
			{
				while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_PROCEED, i+1);
				
				pGetPb = pPBFactory->GetPB(i);
				if( NULL == pGetPb )
					continue;				

				pNewPb = new PB;
				if( pNewPb == NULL )
				{
					AfxMessageBox(IDS_ERR_RES_NOENOUGH);
					PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END,0);	
					pMfr->EnableWindow(TRUE);
					return;
				}
				
				memcpy(pNewPb, pGetPb, sizeof(PB));

				if(theApp.m_nMMIVer == MMI_VERSION_2 && wStore != 0)
				{

					if(arrStore[wStore] == ID_STORE_SIM)
						pNewPb->nGroupIndex = ID_PGM_SIM1 ; //SIM1;
					else if(arrStore[wStore] == ID_STORE_SIM2)
						pNewPb->nGroupIndex = ID_PGM_SIM2; //sim2
					else if(arrStore[wStore] == ID_STORE_SIM3)
						pNewPb->nGroupIndex = ID_PGM_SIM3; //sim3
					else
						pNewPb->nGroupIndex = ID_PGM_SIM4; //sim4

					PPBGROUP ppbg = pIW->GetPBGByIdInPhone(pNewPb->nGroupIndex);
					if(ppbg != NULL)
					{
						ppbg->nMemberNum++;
					}
				}
			
				pIW->AddPB(pNewPb);
				bRead = TRUE;
			}

			
			strProgTitle.Format(IDS_PROG_TITLE_PB_COMPLETE,strPlace);				
			pSb->SetPaneText(0,strProgTitle); 
			TRACE(_T("PB in %s:%s\n"),strPlace,strProgTitle);

			if(nUsed == 0)
				PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, 100);
			else
				PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, 0);

			while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}			
		}
		else
		{
			//CString strPrt;
#ifdef SPRD8800			
			//strPrt.Format(IDS_LOAD_PB_FAILED, _T("SIM Card"));
			strErrorPlace += _T("SIM Card");
#else
			
			//strPrt.Format(IDS_LOAD_PB_FAILED, strPlace);
			strErrorPlace += strPlace;		
#endif
			
			strErrorPlace += _T(",");
			bFailed = TRUE;
			
		}
	}

	if(bFailed)
	{
		strErrorPlace.TrimRight(_T(','));
		CString strPrt;
		CString strCat;
		strCat.LoadString(IDS_CATEG_PB);
		strPrt.Format(IDS_LOAD_FAILED, strCat,strErrorPlace);	
		AfxMessageBox(strPrt);
	}

	strSTB.LoadString(IDS_STATUS_PB_END);	
	pSb->SetPaneText(0,strSTB);	

	SetModifiedFlag(TRUE);

	if( bRead && HIWORD(m_dwCateg)  == ID_CATEG_PB )
	{
		UpdateAllViews(NULL, WM_CATEGORY_CHANGE, (CObject*)m_dwCateg);
	}
	CString strResult;
	if(!bFailed)
	{
#ifdef SPRD8800
		if(arrEmpty[0]== TRUE /*&& arrEmpty[1] == TRUE*/)
#else
		if( arrEmpty[0] == TRUE && arrEmpty[1] == TRUE && 
			arrEmpty[2] == TRUE && arrEmpty[3] == TRUE &&
			arrEmpty[4] == TRUE)
#endif
		{
			CString str;
			str.LoadString(IDS_EMPTY_RECORD);
			strResult.Format(IDS_LOAD_COMPLETE,str);
		}
		else
		{
			strResult.Format(IDS_LOAD_COMPLETE,_T(""));
		}
	}
	
	if(!m_bLoadAll)
	{
		pMfr->EnableWindow(TRUE);
	}

	if( !m_bLoadAll && !m_bSync && theApp.m_bConnected && !bFailed)
	{
		//AfxMessageBox(strResult);
		CString strTitle;
		strTitle.LoadString(IDS_SMS_TITLE);
		MessageBox(theApp.m_pSMSFrame->m_hWnd,strResult,strTitle,MB_OK|MB_ICONINFORMATION);
	}

	if(!theApp.m_bConnected)
	{
		//AfxMessageBox(IDS_ERR_NOT_CONNECT);
		CString strTitle;
		strTitle.LoadString(IDS_SMS_TITLE);
		strResult.LoadString(IDS_ERR_NOT_CONNECT);
		MessageBox(theApp.m_pSMSFrame->m_hWnd,strResult,strTitle,MB_OK|MB_ICONWARNING);
	}
}

void CSMSDoc::OnOptionLoadMemo() 
{
	// TODO: Add your command handler code here
	CWaitCursor waitCursor;
	
    CSMSFrame *pMfr = theApp.m_pSMSFrame;
	CCoolStatusBar *pSb = &pMfr->m_wndStatusBar;
	CString strSTB;
	CString strOldSTB;
	strSTB.LoadString(IDS_STATUS_MEMO_START);
	strOldSTB = pSb->GetPaneText(0);
	pSb->SetPaneText(0,strSTB);

	CMobileCnntApp * pApp = (CMobileCnntApp *)AfxGetApp();
	_ASSERTE( pApp != NULL );

	if( !m_bLoadAll && !pApp->TestIsConnected() )
	{
		pSb->SetPaneText(0,strOldSTB);
		return;
	}

	pMfr->EnableWindow(FALSE);

	IMemo * pMemoFactory = pApp->m_pMemoFactory;
	_ASSERTE( pMemoFactory != NULL );

	CInfoWrapper * pIW = pApp->GetInfoWrapper();
	_ASSERTE( pIW != NULL );

	pIW->RemovePhoneMEMO();
	MSG msg;
	CString strProgTitle;
	BOOL bRead = FALSE;
	int nUsed = 0;
	int nTatol=0;
	if( pMemoFactory->SelectStorePlace(ID_STORE_PHONE) && 
			pMemoFactory->GetMemoNum(&nUsed, &nTatol) )
	{
		if(nUsed > 0)
			PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_BEGIN, nUsed);
		else
			PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_BEGIN, 100);
		
		strProgTitle.LoadString(IDS_PROG_TITLE_MEMO);
		TRACE(_T("MEMO :%s\n"),strProgTitle);
		pSb->SetPaneText(0,strProgTitle); 

		PMEMO pNewMemo = NULL;
		PMEMO pGetMemo = NULL;
		for( int i = 0; i < nUsed && theApp.m_bConnected; ++i )
		{
			while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);	
			}
	
			PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_PROCEED, i+1);
			
			
			pGetMemo = pMemoFactory->GetMemo(i);
			if( NULL == pGetMemo )
				continue;			

			pNewMemo = new MEMO;
			if( NULL == pNewMemo )
			{
				AfxMessageBox(IDS_ERR_RES_NOENOUGH);
			    PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, 0);
				pMfr->EnableWindow(TRUE);
				return;
			}
			
			memcpy(pNewMemo, pGetMemo, sizeof(MEMO));
			pIW->AddMEMO(pNewMemo);
			bRead = TRUE;
		}

		strProgTitle.LoadString(IDS_STATUS_MEMO_END);
		pSb->SetPaneText(0,strProgTitle); 
		TRACE(_T("MEMO :%s\n"),strProgTitle);

		if(nUsed == 0)
			PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, 100);
		else
			PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, 0);

		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
	}
	
	strSTB.LoadString(IDS_STATUS_MEMO_END);	
	pSb->SetPaneText(0,strSTB);	
	
	SetModifiedFlag(TRUE);

	if( bRead && HIWORD(m_dwCateg)  == ID_CATEG_MEMO )
	{		
		UpdateAllViews(NULL, WM_CATEGORY_CHANGE, (CObject*)m_dwCateg);
	}

	CString strResult;
	if(nUsed == 0)
	{
		CString str;
		str.LoadString(IDS_EMPTY_RECORD);
		strResult.Format(IDS_LOAD_COMPLETE,str);
	}
	else
	{
		strResult.Format(IDS_LOAD_COMPLETE,_T(""));
	}

	if(!m_bLoadAll)
	{
		pMfr->EnableWindow(TRUE);
		if(theApp.m_bConnected && !m_bSync)
		{
			//AfxMessageBox(strResult);
			CString strTitle;
			strTitle.LoadString(IDS_SMS_TITLE);
			MessageBox(theApp.m_pSMSFrame->m_hWnd,strResult,strTitle,MB_OK|MB_ICONINFORMATION);
		}
	}

	if(!theApp.m_bConnected)
	{
		//AfxMessageBox(IDS_ERR_NOT_CONNECT);
		CString strTitle;
		strTitle.LoadString(IDS_SMS_TITLE);
		strResult.LoadString(IDS_ERR_NOT_CONNECT);
		MessageBox(theApp.m_pSMSFrame->m_hWnd,strResult,strTitle,MB_OK|MB_ICONWARNING);
	}
}


void CSMSDoc::OnOptionLoadPBG() 
{
	// TODO: Add your command handler code here
	CWaitCursor waitCursor;
	
    CSMSFrame *pMfr = theApp.m_pSMSFrame;
	CCoolStatusBar *pSb = &pMfr->m_wndStatusBar;
	CString strSTB;
	CString strOldSTB;
	strSTB.LoadString(IDS_STATUS_PBG_START);
	strOldSTB = pSb->GetPaneText(0);
	pSb->SetPaneText(0,strSTB);
	
	CMobileCnntApp * pApp = (CMobileCnntApp *)AfxGetApp();
	_ASSERTE( pApp != NULL );
	
	if(!pApp->TestIsConnected() )
	{
		pSb->SetPaneText(0,strOldSTB);
		return;
	}

	pMfr->EnableWindow(FALSE);

	CInfoWrapper * pIW = pApp->GetInfoWrapper();
	_ASSERTE( pIW != NULL );
	
	pIW->RemovePhonePBG();
	
	IPBG * pPBGFactory = pApp->m_pPBGFactory;
	_ASSERTE( pPBGFactory != NULL );
	
	BOOL bRead = FALSE;	//是否下载到了内容
	
	int nUsed =0;
	int nTatol = 0;
	MSG msg;
	CString strProgTitle;
	if( pPBGFactory->SelectStorePlace(ID_STORE_PHONE) && 
		pPBGFactory->GetPBGNum(&nUsed, &nTatol) )
	{		

		if(nUsed > 0)
			PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_BEGIN, nUsed);
		else
			PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_BEGIN, 100);
		
		strProgTitle.LoadString(IDS_PROG_TITLE_PBG);
		pSb->SetPaneText(0,strProgTitle);
		TRACE(_T("PBG :%s\n"),strProgTitle);		


		PPBGROUP pNewPBG = NULL;
		PPBGROUP pGetPBG = NULL;
		for( int i = 0; i < nUsed && theApp.m_bConnected; ++i )
		{
			while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);	
			}
		
			PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_PROCEED, i+1);
			

			pGetPBG = pPBGFactory->GetPBG(i);
			if( NULL == pGetPBG )
			{
				break;		
			}
			
			pNewPBG = new PBGROUP;
			if( pNewPBG == NULL )
			{
				AfxMessageBox(IDS_ERR_RES_NOENOUGH);
				PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, 0);
				pMfr->EnableWindow(TRUE);
				return;
			}
			
			memcpy(pNewPBG, pGetPBG, sizeof(PBGROUP));
			pIW->AddPBG(pNewPBG);
			bRead = TRUE;
		}
		
		if(nUsed == 0)
			PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, 100);
        else
			PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, 0);

		strProgTitle.LoadString(IDS_PROG_TITLE_PBG_COMPLETE);
		pSb->SetPaneText(0,strProgTitle);
		TRACE(_T("PBG :%s\n"),strProgTitle);		
	}

	if(theApp.m_nMMIVer == MMI_VERSION_2)
	{
		for(int k=1; k<= MAX_SIM_NUM; k++)
		{
			if(!theApp.IsPluggedSIM(k))
				continue;

			PPBGROUP pSimPBG = NULL;
			CString strName =_T("");
			pSimPBG = new PBGROUP;
			if( pSimPBG == NULL )
			{
				AfxMessageBox(IDS_ERR_RES_NOENOUGH);
				PostMessage(pMfr->m_hWnd,WM_PROGRESS,PROG_END, 0);
				pMfr->EnableWindow(TRUE);
				return;
			} 
			pSimPBG->bUcs2 =FALSE;
			pSimPBG->bFixedGroup = TRUE;
			pSimPBG->wStore = ID_STORE_PHONE ;// PHONE
			
			pSimPBG->nID = ID_PGM_SIM1 - k +1;
			strName.Format(_T("SIM%d"),k);
			_tcscpy(pSimPBG->szName,strName);						
			pIW->AddPBG(pSimPBG);			
		}
	}
	
	strSTB.LoadString(IDS_STATUS_PBG_END);	
	pSb->SetPaneText(0,strSTB);	
	
	SetModifiedFlag(TRUE);
	
	if( bRead && HIWORD(m_dwCateg) == ID_CATEG_PBG )
	{		
		UpdateAllViews(NULL, WM_CATEGORY_CHANGE, (CObject*)m_dwCateg);
	}
	
	pIW->UpdateCategPBG();
	UpdateAllViews(NULL, WM_UPDATE_GROUP_CHANGE, NULL);

	CString strResult;
	if(nUsed == 0)
	{
		CString str;
		str.LoadString(IDS_EMPTY_RECORD);
		strResult.Format(IDS_LOAD_COMPLETE,str);
	}
	else
	{
		strResult.Format(IDS_LOAD_COMPLETE,_T(""));
	}
    if(!m_bLoadAll && !m_bLoadPB)
		pMfr->EnableWindow(TRUE);

	if(!m_bLoadAll && !m_bLoadPB && !m_bSync && theApp.m_bConnected)		
	{		
		//AfxMessageBox(strResult);
		CString strTitle;
		strTitle.LoadString(IDS_SMS_TITLE);
		MessageBox(theApp.m_pSMSFrame->m_hWnd,strResult,strTitle,MB_OK|MB_ICONINFORMATION);
	}

	if(!theApp.m_bConnected)
	{
		//AfxMessageBox(IDS_ERR_NOT_CONNECT);
		CString strTitle;
		strTitle.LoadString(IDS_SMS_TITLE);
		strResult.LoadString(IDS_ERR_NOT_CONNECT);
		MessageBox(theApp.m_pSMSFrame->m_hWnd,strResult,strTitle,MB_OK|MB_ICONWARNING);
	}
}

void CSMSDoc::OnOptionSmsCenter() 
{
	// TODO: Add your command handler code here
	CSMSCenterDlg dlg(theApp.m_pSMSFrame);
	dlg.DoModal();
}

void CSMSDoc::OnOptionSmsSendMode() 
{
	// TODO: Add your command handler code here
	CSMSSendModeDlg dlg(theApp.m_pSMSFrame);
	dlg.DoModal();
	
}

void CSMSDoc::OnUpdateOptionSmsSendMode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMobileCnntApp * pApp = (CMobileCnntApp *)AfxGetApp();
	pCmdUI->Enable( pApp->m_bConnected );
	
}

void CSMSDoc::OnOptionSmsStore()
{
	CDlgSMSStore dlg(theApp.m_pSMSFrame);
	dlg.DoModal();
}

void CSMSDoc::OnUpdateOptionSmsStore(CCmdUI* pCmdUI)
{
	CMobileCnntApp * pApp = (CMobileCnntApp *)AfxGetApp();
	pCmdUI->Enable( pApp->m_bConnected );
}

void CSMSDoc::OnOptionSmsPreset() 
{
	// TODO: Add your command handler code here
	PresetSMS();
}

void CSMSDoc::PresetSMS()
{
	CSMSPresetDlg dlg(theApp.m_pSMSFrame);
	dlg.DoModal();
}

void CSMSDoc::OnUpdateOptionLoadMSAll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMobileCnntApp * pApp = (CMobileCnntApp *)AfxGetApp();
	pCmdUI->Enable( pApp->m_bConnected );
}

void CSMSDoc::OnUpdateOptionLoadMemo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMobileCnntApp * pApp = (CMobileCnntApp *)AfxGetApp();
	pCmdUI->Enable( pApp->m_bConnected );
}

void CSMSDoc::OnUpdateOptionLoadPb(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMobileCnntApp * pApp = (CMobileCnntApp *)AfxGetApp();
	pCmdUI->Enable( pApp->m_bConnected );
}

void CSMSDoc::OnUpdateOptionLoadPBG(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMobileCnntApp * pApp = (CMobileCnntApp *)AfxGetApp();
	pCmdUI->Enable( pApp->m_bConnected );	
}

void CSMSDoc::OnUpdateOptionLoadSms(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMobileCnntApp * pApp = (CMobileCnntApp *)AfxGetApp();
	pCmdUI->Enable( pApp->m_bConnected );
}

void CSMSDoc::OnUpdateOptionSmsCenter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMobileCnntApp * pApp = (CMobileCnntApp *)AfxGetApp();
	pCmdUI->Enable( pApp->m_bConnected );
}
/*
 *	获得分散的短信，并合并成一条长短信
 *  @Param pFirstSI 第一条短信指针
 *  @Param pFlag    所有短信处理情况标识数组指针
 *  @Param nCount   所有短信的数量
 *  @Return 
 */
BOOL CSMSDoc::LoadSplishSMS(PSMS_INFO pFirstSI,int *pFlag, int nCount)
{
	_ASSERT(pFirstSI != NULL && pFlag != NULL);
	_ASSERT(pFirstSI->bUdhi);

	CMobileCnntApp * pApp = (CMobileCnntApp *)AfxGetApp();
	_ASSERTE( pApp != NULL );

	CInfoWrapper * pIW = pApp->GetInfoWrapper();
	_ASSERTE( pIW != NULL );

    ISMS * pSMS = pApp->m_pSMSFactory;
    _ASSERTE( pSMS != NULL );

    //备份所有短信处理情况标识数组	
	int *pFlagBak = (int *)new int[nCount];
	if(pFlagBak == NULL)
		return FALSE;
	memcpy(pFlagBak,pFlag,sizeof(int)*nCount);

    //长短信的总条数
	int nItem = pFirstSI->nTotal;

	//保存长短信
	PSMS_INFO * ppNextSI = (PSMS_INFO *)new PSMS_INFO[nItem];
	PSMS_INFO pNextSI = NULL;
	int i,j,nRealItem;
	for(j=0;j<nItem;j++)
		ppNextSI[j]=NULL;

	ppNextSI[0] = pFirstSI;

	nRealItem =1;
	for( i = 0; i < nCount; ++i )
	{
		if(pFlag[i] != 0)
			continue;
		pNextSI= pSMS->GetSMS(i);			
		_ASSERTE( NULL != pNextSI );
		if(pNextSI->bUdhi && pNextSI->nRef == pFirstSI->nRef &&
			pNextSI->nTotal == pFirstSI->nTotal )
		{
			//ppNextSI[nRealItem++]=pNextSI;
			//pFlag[i]=1;

			//插入排序,将短信按nSeq从小到大排序
			int k = 0;
			BOOL bExist = FALSE;
			for( k=0;k<nRealItem;k++)
			{
				if(pNextSI->nSeq <= ((PSMS_INFO)ppNextSI[k])->nSeq )
				{
					if(pNextSI->nSeq == ((PSMS_INFO)ppNextSI[k])->nSeq)
					{
						bExist = TRUE;
					}
				
					break;				
				}
			}
			if(bExist)
				continue;
			else
			{
				if(k>=nRealItem && !bExist)
				{
					ppNextSI[nRealItem++]=pNextSI;
					pFlag[i]=1;
				}
				else
				{	
					int t=0;
					for(t=nRealItem-1;t>=k/*&& t>=0*/;t--)
					{
						ppNextSI[t+1]=ppNextSI[t];
					}
					ppNextSI[k]=pNextSI;
					pFlag[i]=1;
					nRealItem++;
				}
			}
		}

	}

	PSMS pSmsNext = NULL;
	PSMS pNewSms  = NULL;
	for(i=0;i<nRealItem;i++)
	{
		if(i == 0)
		{
			pNewSms = new SMS;
			if( pNewSms == NULL )
			{
				AfxMessageBox(IDS_ERR_RES_NOENOUGH);
				delete [] pFlagBak;
				delete [] ppNextSI;
				return FALSE;
			}
			memset(pNewSms,0,sizeof(SMS));
		}
		
        pNewSms->nIdx = ppNextSI[i]->nIndex;
        switch( ppNextSI[i]->eStorePlace )
        {
        case SMS_SP_MS:
            pNewSms->wStore = ID_STORE_PHONE;
            break;
        case SMS_SP_SIM:
            pNewSms->wStore = ID_STORE_SIM;
            break;
		case SMS_SP_SIM2:
			pNewSms->wStore = ID_STORE_SIM2;
            break;
		case SMS_SP_SIM3:
			pNewSms->wStore = ID_STORE_SIM3;
            break;
		case SMS_SP_SIM4:
			pNewSms->wStore = ID_STORE_SIM4;
            break;
        default:
            _ASSERTE( 0 );
        }

        switch( ppNextSI[i]->eStatus )
        {
        case SMS_STAT_UNREAD:
            pNewSms->wStatus = SMS_STATUS_UNREAD;
            break;
        case SMS_STAT_READ:
            pNewSms->wStatus = SMS_STATUS_READ;
            break;
        case SMS_STAT_UNSEND:
            pNewSms->wStatus = SMS_STATUS_UNSEND;	
            break;
        case SMS_STAT_SEND:
            pNewSms->wStatus = SMS_STATUS_SEND;
            break;
        default:
            _ASSERTE( 0 );
        }
		pNewSms->bUcs2 = ppNextSI[i]->bIsUcs2;
		pNewSms->bExistSC = ppNextSI[i]->bExistSC;
        _tcscpy(pNewSms->szPhone,   ppNextSI[i]->szNumber);
        _tcscpy(pNewSms->szDate,    ppNextSI[i]->szDate);
        _tcscpy(pNewSms->szContent, ppNextSI[i]->szContent );

		if(i!=nRealItem-1)
		{
			pSmsNext = new SMS;
			if( pSmsNext == NULL )
			{
				AfxMessageBox(IDS_ERR_RES_NOENOUGH);
				delete [] ppNextSI;
				delete [] pFlagBak;
				return FALSE;
			}
			memset(pSmsNext,0,sizeof(SMS));
		}
		else
		{
			pSmsNext = NULL;
		}


		pNewSms->pNext = pSmsNext;
		if(i==0)
		{
			pNewSms->pPrev = NULL;
		}
        pIW->AddSMS(pNewSms);
		
		if(pSmsNext != NULL)
		    pSmsNext->pPrev = pNewSms;

		pNewSms = pSmsNext;

	}
	delete [] ppNextSI;
	delete [] pFlagBak;
	return TRUE;
}

void CSMSDoc::OnOptionPBType()
{
	CDlgPBType dlg(theApp.m_pSMSFrame);
	dlg.DoModal();
}

void CSMSDoc::OnUpdateOptionPBType(CCmdUI* pCmdUI)
{
	CMobileCnntApp * pApp = (CMobileCnntApp *)AfxGetApp();
	pCmdUI->Enable( pApp->m_bConnected );
}

