#include "stdafx.h"
#include "MAParser.h"

CMAParser::CMAParser(void)
{
}

CMAParser::~CMAParser(void)
{
}
VOID CMAParser::Clear()
{
	memset(&m_tHead,0,sizeof(m_tHead));
	m_tHead.version = MTVA_VER;
	m_vMTVA.clear();


}
BOOL CMAParser::EncodeMTVA(LPBYTE &pBuf,DWORD &dwSize)
{	
	pBuf = NULL;
	dwSize = 0;
	DWORD dwMaxSize = (sizeof(MTVA_T))*m_tHead.nCount + sizeof(MTVA_FILE_HEAD_T);
	pBuf = new BYTE[dwMaxSize];
	memset(pBuf,0,dwMaxSize);
	LPBYTE pCur = pBuf;

	*(DWORD*)pCur=MTVA_MAGIC;
	pCur += sizeof(DWORD);

	DWORD ver = m_tHead.version;
	*(DWORD*)pCur=ver;
	pCur += sizeof(DWORD);

	*(DWORD*)(pCur)  = m_tHead.nCount;
	pCur += sizeof(DWORD);

	pCur += sizeof(DWORD);//reserved

	for(UINT i= 0; i< m_vMTVA.size();i++)
	{
		*(DWORD*)(pCur)  = m_vMTVA[i].nMcc;
		pCur += sizeof(DWORD);
		*(DWORD*)(pCur)  = m_vMTVA[i].nMnc;
		pCur += sizeof(DWORD);

		for(int j = 0; j < ECallCount; j++)
		{
			memcpy(pCur, m_vMTVA[i].ECall[j], strlen(m_vMTVA[i].ECall[j]));
			pCur += ECallSize;
		}
		for(int k = 0; k < VMailCount; k++)
		{
			memcpy(pCur, m_vMTVA[i].VMail[k], strlen(m_vMTVA[i].VMail[k]));
			pCur += VMailSize;
		}
		memcpy(pCur, m_vMTVA[i].Url, strlen(m_vMTVA[i].Url));
		pCur += URLSize;
		/*
		if(m_vBKMK[i].active_flag)
		{
			*pOffset = dwOffset;
			pCur = pBuf+dwOffset;

			EncodeString(pCur,m_vBKMK[i].name,TRUE);
			EncodeString(pCur,m_vBKMK[i].url,FALSE);
			dwOffset = pCur - pBuf;
			pOffset++;
		}*/
	}

	dwSize = pCur - pBuf;

	return TRUE;

}
BOOL CMAParser::DecodeMTVA(LPBYTE pBuf,DWORD dwSize)
{
	int i = sizeof(_MTVA_T);
	if((dwSize - sizeof(MTVA_FILE_HEAD_T)) % sizeof(_MTVA_T) != 0 || pBuf == NULL)
	{
		return FALSE;
	}
	Clear();

	memcpy(&m_tHead,pBuf,sizeof(MTVA_FILE_HEAD_T));
	if(m_tHead.magic != MTVA_MAGIC)
	{
		Clear();
		return FALSE;
	}
	if(m_tHead.version != MTVA_VER)
	{
		Clear();
		return FALSE;
	}
	if(m_tHead.nCount > MTVA_MAX_NUM)
	{
		Clear();
		return FALSE;
	}
	if(m_tHead.nCount == 0)
	{
		return TRUE;
	}

	UINT nCount = m_tHead.nCount;
	LPBYTE pCur = pBuf + sizeof(MTVA_FILE_HEAD_T);
	MTVA_T bm;
	for(UINT i = 0; i< nCount; i++)
	{
		bm.Init();
		bm.nMcc = *(DWORD*)pCur;
		pCur += sizeof(DWORD);
		bm.nMnc = *(DWORD*)pCur;
		pCur += sizeof(DWORD);

		for(UINT j = 0; j < ECallCount; j++)
		{
			memcpy(bm.ECall[j], pCur, sizeof(bm.ECall[j]));
			pCur += sizeof(bm.ECall[j]);
		}
		for(UINT k = 0; k < VMailCount; k++)
		{
			memcpy(bm.VMail[k], pCur, sizeof(bm.VMail[k]));
			pCur += sizeof(bm.VMail[k]);
		}

		memcpy(bm.Url, pCur, sizeof(bm.Url));
		pCur += sizeof(bm.Url);

		m_vMTVA.push_back(bm);
	}

	return TRUE;
}