#include "stdafx.h"
#include "..\mobilecnnt.h"
#include "InfoWrapper.h"
#include "mmiatc.h"


int CInfoWrapper::GetMEMOCount(WORD wStore)
{
	int nCount=0;
	int nAll = m_arrMEMO.GetSize();
	if(wStore == ID_STORE_ALL)
		return nAll;
	
	PMEMO pMEMO= NULL;
	for(int i=0;i<nAll;i++)
	{
		pMEMO = m_arrMEMO.GetAt(i);
		if(pMEMO->wStore == wStore)
			nCount++;
	}
	return nCount;
}

PMEMO CInfoWrapper::GetMEMO( int nIdx )
{
	_ASSERTE( nIdx >= 0 && nIdx < m_arrMEMO.GetSize() );

	return m_arrMEMO[nIdx];
}

//DEL PMEMO CInfoWrapper::GetFirstUnusedMEMOInPhone( void )
//DEL {
//DEL 	PMEMO pMemo = NULL;
//DEL 	int   nCount = m_arrMEMO.GetSize();
//DEL 	for( int i = 0; i < nCount; ++i )
//DEL 	{
//DEL 		pMemo = m_arrMEMO[i];
//DEL 		if( pMemo->wStore == ID_STORE_PHONE && !pMemo->bUsed )
//DEL 			return pMemo;
//DEL 	}
//DEL 	return NULL;
//DEL }

void CInfoWrapper::AddMEMO( PMEMO pMEMO )
{
	_ASSERTE( pMEMO != NULL );

	m_arrMEMO.Add( pMEMO );
}

void CInfoWrapper::RemoveMEMO( int nIdx )
{
	_ASSERTE( nIdx >= 0 && nIdx < m_arrMEMO.GetSize() );

	PMEMO pMEMO = m_arrMEMO[nIdx];
	delete pMEMO;
	pMEMO = NULL;
	m_arrMEMO.RemoveAt(nIdx);
}

void CInfoWrapper::RemoveMEMO( PMEMO pMEMO )
{
	_ASSERTE( pMEMO != NULL );

	int nSize = m_arrMEMO.GetSize();
	for( int i = 0; i < nSize; ++i )
	{
		if( pMEMO == m_arrMEMO[i] )
		{
			delete pMEMO;
			pMEMO = NULL;
			m_arrMEMO.RemoveAt(i);
			return;
		}
	}
	_ASSERTE(0);
}

void CInfoWrapper::RemovePhoneMEMO(void)
{
	int nSize = m_arrMEMO.GetSize();

	PMEMO pMemo = NULL;
	for( int i = nSize - 1; i >= 0; --i )
	{
		pMemo = m_arrMEMO[i];
		if( pMemo->wStore == ID_STORE_PHONE || pMemo->wStore == ID_STORE_SIM )
		{
			delete pMemo;
			pMemo = NULL;
			m_arrMEMO.RemoveAt(i);
		}
	}
}

void CInfoWrapper::RemoveAllMEMO(void)
{
	int nSize = m_arrMEMO.GetSize();

	PMEMO pMEMO = NULL;
	for( int i = 0; i < nSize; ++i )
	{
		pMEMO = m_arrMEMO[i];
		delete pMEMO;
		pMEMO = NULL;
	}

	m_arrMEMO.RemoveAll();
}

CString CInfoWrapper::GetMemoMode(int nIdx)
{
	_ASSERTE(nIdx>=0 && nIdx < m_arrMemoMode.GetSize());
	
	return m_arrMemoMode.GetAt(nIdx);
}

CString CInfoWrapper::GetMemoFre(BYTE fre)
{
	static _TCHAR szBuf[512] = {0};
	memset(szBuf,0,sizeof(_TCHAR)*512);

	int nIdx = 1;
	BYTE mask[7]= {MASK_B0,MASK_B1,MASK_B2,MASK_B3,MASK_B4,MASK_B5,MASK_B6};
	for(int i=0; i< 7; i++)
	{
		if(fre & mask[i])
		{
			_tcscat(szBuf,m_arrMemoFre.GetAt(i).operator LPCTSTR());
			_tcscat(szBuf,_T(","));
		}
	}
	if(_tcslen(szBuf) > 0)
	{
		szBuf[_tcslen(szBuf)-1] = '\0';
	}
	return szBuf;
}