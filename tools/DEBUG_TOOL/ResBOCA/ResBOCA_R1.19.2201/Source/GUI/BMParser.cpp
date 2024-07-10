// BMParser.cpp: implementation of the CBMParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResOver.h"
#include "BMParser.h"
#include "ResParser.h"
#include <atlconv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBMParser::CBMParser()
{
	Clear();
}

CBMParser::~CBMParser()
{

}

BOOL CBMParser::DecodeBKMK(LPBYTE pBuf,DWORD dwSize)
{
	if(dwSize<8 || pBuf == NULL)
	{
		return FALSE;
	}
	Clear();
	//parse head
	memcpy(&m_tHead,pBuf,8);
	if(m_tHead.magic != BKMK_MAGIC)
	{
		Clear();
		return FALSE;
	}
// 	if(m_tHead.bigendian)
// 	{
// 		m_tHead.version = CONV_SHORT(m_tHead.version);
// 	}
	if(m_tHead.version != 1)
	{
		Clear();
		return FALSE;
	}
	if(m_tHead.nCount > BKMK_MAX_NUM)
	{
		Clear();
		return FALSE;
	}

	if(m_tHead.nCount == 0)
	{
		return TRUE;
	}

	//parse offset
	if(dwSize < (sizeof(DWORD)*m_tHead.nCount + 8))
	{
		Clear();
		return FALSE;
	}

	DWORD *pOffset = new DWORD[m_tHead.nCount];
	memcpy(pOffset,pBuf+8,sizeof(DWORD)*m_tHead.nCount);
	
	UINT nCount = m_tHead.nCount;
	LPBYTE pCur = NULL;
	LPBYTE pEnd = pBuf + dwSize;
	BOOKMARK_T bm;
	for(UINT i = 0; i< nCount; i++)
	{
// 		if(m_tHead.bigendian)
// 		{
// 			m_tHead.offset[i] = CONV_DWORD(m_tHead.offset[i]);
// 		}
		bm.Init();
		pCur = pBuf + pOffset[i];
		if((pCur + 8) > pEnd) //8 is the least len for each group parameter
		{
			m_tHead.nCount = (BYTE)i;
			if(pOffset != NULL)
			{
				delete pOffset;
				pOffset = NULL;
			}
			return FALSE;
		}

		bm.active_flag = TRUE;


		
			bm.nMcc = *(WORD*)pCur;
			pCur += sizeof(WORD);
			bm.nMnc = *(WORD*)pCur;
			pCur += sizeof(WORD);

		//memcpy(&bm.nMcc,pCur,4);
		//pCur += 4;

#define DECODE_STR(buf,size,flag) if(!DecodeString(pCur,pEnd,buf,size,flag) || pCur > pEnd) return FALSE	

		if(!DecodeString(pCur,pEnd,bm.name,BKMK_NAME_LEN,TRUE) || pCur > pEnd) 
		{
			m_tHead.nCount = (BYTE)i;
			if(pOffset != NULL)
			{
				delete pOffset;
				pOffset = NULL;
			}
			return FALSE;
		}

		if(!DecodeString(pCur,pEnd,bm.url,BKMK_URL_LEN,FALSE) || pCur > pEnd) 
		{
			m_tHead.nCount = (BYTE)i;
			if(pOffset != NULL)
			{
				delete pOffset;
				pOffset = NULL;
			}
			return FALSE;
		}

		//DECODE_STR(bm.name,BKMK_NAME_LEN,TRUE);

		//DECODE_STR(bm.url,BKMK_URL_LEN,FALSE);

		m_vBKMK.push_back(bm);
	}

	if(nCount % 10 != 0 )
	{
		UINT nPad = 10 - nCount%10;
		for(UINT j= 0; j< nPad;j++)
		{
			BOOKMARK_T bm;
			bm.Init();
			m_vBKMK.push_back(bm);
		}
	}

	if(pOffset != NULL)
	{
		delete pOffset;
		pOffset = NULL;
	}


	return TRUE;
}

BOOL CBMParser::EncodeBKMK(LPBYTE &pBuf,DWORD &dwSize)
{
	// force to make it little endian
//	m_tHead.bigendian = FALSE;
	pBuf = NULL;
	dwSize = 0;
	DWORD dwMaxSize = (sizeof(BOOKMARK_T)+sizeof(DWORD))*m_tHead.nCount + 8;
	pBuf = new BYTE[dwMaxSize];
	memset(pBuf,0,dwMaxSize);
	LPBYTE pCur = pBuf;

	*(DWORD*)pCur=BKMK_MAGIC;
	pCur += 4;


	WORD ver = m_tHead.version;
// 	if(m_tHead.bigendian)
// 	{
// 		ver = CONV_SHORT(ver);
// 	}
	*(WORD*)pCur=ver;
	pCur += 2;


//	*pCur = m_tHead.bigendian;
	*(WORD*)(pCur)  = m_tHead.nCount;
	pCur += 2;

	DWORD *pOffset = (DWORD*)pCur;
	pCur += m_tHead.nCount*4;

	DWORD dwOffset = pCur-pBuf;
	for(UINT i= 0; i< m_vBKMK.size();i++)
	{
		if(m_vBKMK[i].active_flag)
		{
			*pOffset = /*m_tHead.bigendian ? CONV_DWORD(dwOffset) :*/ dwOffset;
			pCur = pBuf+dwOffset;

			*(WORD*)pCur = m_vBKMK[i].nMcc;
			pCur += sizeof(WORD);
			*(WORD*)pCur = m_vBKMK[i].nMnc;
			pCur += sizeof(WORD);

			EncodeString(pCur,m_vBKMK[i].name,TRUE);
			EncodeString(pCur,m_vBKMK[i].url,FALSE);
			dwOffset = pCur - pBuf;
			pOffset++;
		}
	}

	dwSize = pCur - pBuf;

	return TRUE;
}

BOOL CBMParser::DecodeString(LPBYTE &pBuf, LPBYTE pEnd, WCHAR *pString,WORD wStrMaxLen,BOOL bUnicode/*=FALSE*/)
{	
	USES_CONVERSION;
	WORD wLen = *(WORD*)pBuf;
	pBuf+=2;
// 	if(m_tHead.bigendian)
// 	{
// 		wLen = CONV_SHORT(wLen);
// 	}
	if((pBuf+wLen) > pEnd)
	{
		return FALSE;
	}
	WORD wMin = wLen > wStrMaxLen ? wStrMaxLen : wLen;
	if(bUnicode)
	{
		memcpy(pString, pBuf,wMin*2);
// 		if(m_tHead.bigendian)
// 		{
// 			for(WORD k = 0; k< wMin; k++)
// 			{
// 				pString[k] = CONV_SHORT(pString[k]);
// 			}
// 		}
	}
	else
	{
		char szTmp[514] = {0};
		memcpy(szTmp, pBuf,wMin);
		_tcscpy(pString,A2W(szTmp));
	}
	pBuf+= wLen;

	return TRUE;
}

BOOL CBMParser::EncodeString(LPBYTE &pBuf, WCHAR *pString,BOOL bUnicode/*=FALSE*/)
{
	USES_CONVERSION;
	WORD wLen = (WORD)(_tcslen(pString)+1);
	if(bUnicode)
	{
		wLen *= 2;
	}
	WORD wSize = (WORD)(((wLen + 3 + 2)/4)*4 - 2);  // not include size itself, but calc align with it
	*(WORD*)pBuf = /*m_tHead.bigendian?CONV_SHORT(wSize):*/wSize;
	pBuf += 2;
	if(bUnicode)
	{
		for(WORD i = 0; i< (wLen/2); i++)
		{
			*(WORD*)(pBuf+i*2) = /*m_tHead.bigendian?CONV_SHORT(pString[i]):*/pString[i];
		}
	}
	else
	{
		strcpy((char*)pBuf,W2A(pString));
	}

	pBuf += wSize;

	return TRUE;
}

VOID CBMParser::Clear()
{
	memset(&m_tHead,0,sizeof(m_tHead));
	// m_tHead.bigendian = TRUE;
	m_tHead.version = BKMK_VER;
	m_vBKMK.clear();

// 	BOOKMARK_T bm;
// 	bm.Init();
// 	m_vBKMK.push_back(bm);
}