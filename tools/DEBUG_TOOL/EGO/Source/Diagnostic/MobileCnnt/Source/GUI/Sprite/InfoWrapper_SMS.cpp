#include "stdafx.h"
#include "..\mobileCnnt.h"
#include "InfoWrapper.h"
#include "GlobalDef.h"


////////// 预设短信息 /////////////////////////
int CInfoWrapper::GetPresetSMSCount()
{
	return m_arrPresetSMS.GetSize();
}

PPRESETSMS CInfoWrapper::GetPresetSMS( int nIdx )
{
	_ASSERTE( nIdx >= 0 && nIdx < m_arrPresetSMS.GetSize() );

	return m_arrPresetSMS[nIdx];
}

void CInfoWrapper::AddPresetSMS( PPRESETSMS pSMS )
{
	_ASSERTE( pSMS != NULL );

	m_arrPresetSMS.Add( pSMS );
}

void CInfoWrapper::DelPresetSMS( int nIdx )
{
	_ASSERTE( nIdx >= 0 && nIdx < m_arrPresetSMS.GetSize() );

	PPRESETSMS pSMS = m_arrPresetSMS[nIdx];
	delete pSMS;
	pSMS = NULL;

	m_arrPresetSMS.RemoveAt(nIdx);
}

void CInfoWrapper::SetCurSelPresetSMS(int nCurSel )
{
	_ASSERTE( nCurSel >= 0 && nCurSel < m_arrPresetSMS.GetSize() );
	
	m_nCurSelPresetSMS = nCurSel;
}

PPRESETSMS CInfoWrapper::GetCurSelPresetSMS(void)
{
	_ASSERTE( m_nCurSelPresetSMS >= 0 && m_nCurSelPresetSMS < m_arrPresetSMS.GetSize() );

	return m_arrPresetSMS[m_nCurSelPresetSMS];
}

BOOL CInfoWrapper::IsPresetSMSExist(LPCTSTR pszCaption)
{
	_ASSERTE( pszCaption != NULL );

	int nSize = m_arrPresetSMS.GetSize();

	PPRESETSMS pSMS = NULL;
	for( int i = 0; i < nSize; ++i )
	{
		pSMS = m_arrPresetSMS[i];
	    if( _tcscmp(pSMS->szCaption, pszCaption) == 0 )
		{
			return  TRUE;
		}
	}
	return  FALSE;
}

void CInfoWrapper::RemoveAllPresetSMS(void)
{
	int nSize = m_arrPresetSMS.GetSize();

	PPRESETSMS pSMS = NULL;
	for( int i = 0; i < nSize; ++i )
	{
		pSMS = m_arrPresetSMS[i];
		delete pSMS;
		pSMS = NULL;
	}

	m_arrPresetSMS.RemoveAll();
}
/////////////////////////////SMS///////////////////////////

int CInfoWrapper::GetSMSCount(WORD wStore, BOOL bLongSms /* = TRUE */)
{
	int nCount=0;
	int nAll = m_arrSMS.GetSize();
	PSMS pSms = NULL;
	if(wStore == ID_STORE_ALL )
	{
		if(!bLongSms)
			return nAll;
		else
		{
			for(int i = 0; i<nAll;i++)
			{
				pSms = m_arrSMS.GetAt(i);
				if(pSms->pPrev == NULL)
					nCount++;
			}
			return nCount;
		}
	}
	
	if( wStore == ID_STORE_PC    || 
		wStore == ID_STORE_PHONE ||
		wStore == ID_STORE_SIM  || 
		wStore == ID_STORE_SIM2 || 
		wStore == ID_STORE_SIM3 ||
		wStore == ID_STORE_SIM4)
	{
		if(bLongSms)
		{
			for(int i=0;i<nAll;i++)
			{
				pSms = m_arrSMS.GetAt(i);
				if(pSms->wStore == wStore && pSms->pPrev == NULL)
				{
					nCount++;
				}
			}
		}
		else
		{
			for(int i=0;i<nAll;i++)
			{
				pSms = m_arrSMS.GetAt(i);
				if(pSms->wStore == wStore)
				{
					nCount++;
				}
			}
		}
	}
	else
	{
		//以下都是bLongSms = TRUE
		for(int i=0;i<nAll;i++)
		{
			pSms = m_arrSMS.GetAt(i);
			if(pSms->wStore != ID_STORE_PC)
			{
				switch(wStore) {
				case ID_STORE_INBOX:
					if( pSms->pPrev == NULL	&& (pSms->wStatus == SMS_STATUS_READ || 
						pSms->wStatus  == SMS_STATUS_UNREAD) )
					{
						nCount++;
					}
					break;
				case ID_STORE_SENDS:
					if( pSms->pPrev == NULL && 
						pSms->wStatus == SMS_STATUS_SEND)
					{
						nCount++;
					}
					break;
				case ID_STORE_OUTBOX:
					if( pSms->pPrev == NULL&& 
						pSms->wStatus == SMS_STATUS_UNSEND && 
						pSms->bExistSC  )
					{
						nCount++;
					}
					break;
				case ID_STORE_DRAFTS:
					if( pSms->pPrev == NULL && 
						pSms->wStatus == SMS_STATUS_UNSEND && 
						!pSms->bExistSC )
					{
						nCount++;
					}
					break;
				default:
					break;
				}
			}
		}
	}
	return nCount;
}

PSMS CInfoWrapper::GetSMS( int nIdx )
{
	_ASSERTE( nIdx >= 0 && nIdx < m_arrSMS.GetSize() );

	return m_arrSMS[nIdx];
}

void CInfoWrapper::AddSMS( PSMS pSMS )
{
	_ASSERTE( pSMS != NULL );

	m_arrSMS.Add( pSMS );
}

void  CInfoWrapper::SetSMSTotalCount(WORD wStore, int nTotalCount, int nUsedCount)
{
	UNREFERENCED_PARAMETER(wStore);
	
	m_nTotalSmsCount = nTotalCount;
	m_nUsedSmsCount = nUsedCount;
}

void CInfoWrapper::RemoveSMS( int nIdx )
{
	_ASSERTE( nIdx >= 0 && nIdx < m_arrSMS.GetSize() );

	PSMS pSMS = m_arrSMS[nIdx];
	delete pSMS;
	pSMS = NULL;

	m_arrSMS.RemoveAt(nIdx);
}

void CInfoWrapper::RemoveSMS( PSMS psms )
{
	_ASSERTE( psms != NULL );

	int nSize = m_arrSMS.GetSize();
	for( int i = 0; i < nSize; ++i )
	{
		if( psms == m_arrSMS[i] )
		{
			delete psms;
			psms = NULL;
			m_arrSMS.RemoveAt(i);
			return;
		}
	}
	
}

void CInfoWrapper::RemovePhoneSIMSMS(void)
{
	int nSize = m_arrSMS.GetSize();

	PSMS pSMS = NULL;
	for( int i = nSize - 1; i >= 0; --i )
	{
		pSMS = m_arrSMS[i];
		if( pSMS->wStore == ID_STORE_PHONE || 
			pSMS->wStore == ID_STORE_SIM || 
			pSMS->wStore == ID_STORE_SIM2 ||
			pSMS->wStore == ID_STORE_SIM3 ||
			pSMS->wStore == ID_STORE_SIM4)
		{
			delete [] pSMS;
			pSMS = NULL;
			m_arrSMS.RemoveAt(i);
		}
	}
}

void CInfoWrapper::RemoveAllSMS(void)
{
	int nSize = m_arrSMS.GetSize();

	PSMS pSMS = NULL;
	for( int i = 0; i < nSize; ++i )
	{
		pSMS = m_arrSMS[i];
		delete pSMS;
		pSMS = NULL;
	}

	m_arrSMS.RemoveAll();
}