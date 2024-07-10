#include "stdafx.h"
#include "..\mobilecnnt.h"
#include "InfoWrapper.h"	
	
int CInfoWrapper::GetPBGCount(WORD wStore)
{
	int nCount=0;
	int nAll = m_arrPBG.GetSize();
	if(wStore == ID_STORE_ALL)
		return nAll;
	
	PPBGROUP pPBG = NULL;
	for(int i=0;i<nAll;i++)
	{
		pPBG = m_arrPBG.GetAt(i);
		if(pPBG->wStore == wStore)
			nCount++;
	}
	return nCount;
}

int CInfoWrapper::GetPBGMemoberCount(int nIdx)
{
	_ASSERT(nIdx>=0 && nIdx < m_arrPBG.GetSize());
	int nMember = 0;
	int nCount = GetPBCount(ID_STORE_ALL);
	PPB pPB = NULL;
	for(int i=0;i<nCount; i++)
	{
		pPB = GetPB(i);
		if(pPB->wStore != ID_STORE_PC && pPB->nGroupIndex == GetPBG(nIdx)->nID)
		{
			nMember++;
		}
	}
	if(m_arrPBG.GetAt(nIdx)->nMemberNum != nMember)
	{
		m_arrPBG.GetAt(nIdx)->nMemberNum = nMember;
	}
	return nMember;
}

PPBGROUP CInfoWrapper::GetPBG( int nIdx )
{
	//_ASSERTE( nIdx >= 0 && nIdx < m_arrPBG.GetSize() );
    if(!(nIdx >= 0 && nIdx < m_arrPBG.GetSize()))
		nIdx = 0;
	return m_arrPBG[nIdx];
}
PPBGROUP CInfoWrapper::GetPBGByIdInPhone( UINT nID )
{
	int nCount = m_arrPBG.GetSize();
	int i=0;
	for(i=0;i<nCount;i++)
	{
		if(m_arrPBG.GetAt(i)->nID == nID)
			break;
	}
    if(i<nCount)
		return m_arrPBG.GetAt(i);
	else
		return NULL;
}
int CInfoWrapper::GetPBGIndex(UINT nPbgID)
{
	int nCount = m_arrPBG.GetSize();
	int i=0;
	for(i=0;i<nCount;i++)
	{
		if(m_arrPBG.GetAt(i)->nID == nPbgID)
			break;
	}
	if(i<nCount)
	{
		return i;
	}
	else
	{
		return -1;
	}

}

int CInfoWrapper::GetPBGIndexByName(LPCTSTR lpszName)
{
	int nCount = m_arrPBG.GetSize();
	int i=0;
	for(i=0;i<nCount;i++)
	{
		if(_tcsicmp(m_arrPBG.GetAt(i)->szName,lpszName)==0)
			break;
	}
	if(i<nCount)
	{
		return i;
	}
	else
	{
		return -1;
	}	
}

void CInfoWrapper::AddPBG( PPBGROUP pPBG )
{
	_ASSERTE( pPBG != NULL );

	m_arrPBG.Add( pPBG );
}

void CInfoWrapper::SetCurSelPBG(int nIdx)
{
	_ASSERTE( nIdx >= 0 && nIdx < m_arrPBG.GetSize() );

	m_nCurSelPBG = nIdx;
}

PPBGROUP CInfoWrapper::GetCurSelPBG(void)
{
	return m_arrPBG[m_nCurSelPBG];
}

void CInfoWrapper::RemovePBG( int nIdx )
{
	_ASSERTE( nIdx >= 0 && nIdx < m_arrPBG.GetSize() );

	PPBGROUP pPBG = m_arrPBG[nIdx];
	delete pPBG;
	pPBG = NULL;

	m_arrPBG.RemoveAt(nIdx);
}

void CInfoWrapper::RemovePBG( PPBGROUP pPBG)
{
	_ASSERTE( pPBG != NULL );

	int nSize = m_arrPBG.GetSize();
	for( int i = 0; i < nSize; ++i )
	{
		if( pPBG == m_arrPBG[i] )
		{
			delete pPBG;
			pPBG = NULL;
			m_arrPBG.RemoveAt(i);
			return;
		}
	}
	_ASSERTE(0);
}
void CInfoWrapper::RemovePhonePBG(void)
{
	int nSize = m_arrPBG.GetSize();

	PPBGROUP pPBG = NULL;
	for( int i = nSize - 1; i >= 0; --i )
	{
		pPBG = m_arrPBG[i];
		if( pPBG->wStore == ID_STORE_PHONE )
		{
			delete pPBG;
			pPBG = NULL;
			m_arrPBG.RemoveAt(i);
		}
	}
}
void CInfoWrapper::RemoveAllPBG(void)
{
	int nSize = m_arrPBG.GetSize();

	PPBGROUP pPBG = NULL;
	for( int i = 0; i < nSize; ++i )
	{
		pPBG = m_arrPBG[i];
		delete pPBG;
		pPBG = NULL;
	}

	m_arrPBG.RemoveAll();
}