// GenDeltaNVIndex.cpp: implementation of the CGenDeltaNVIndex class.
//
//////////////////////////////////////////////////////////////////////

#include "GenDeltaNVIndex.h"
#include "TCHAR.h"
#include <algorithm>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGenDeltaNVIndex::CGenDeltaNVIndex()
{

}

CGenDeltaNVIndex::~CGenDeltaNVIndex()
{

}

BOOL CGenDeltaNVIndex::GenDeltaIndex(LPBYTE pDeltaBuf,DWORD& dwSize, LPCTSTR lpIndexFile)
{
	BOOL bOK = FALSE;
	CMapDeltaNV mapOffset;
	CMapIndex mapIndex;
	LPBYTE lpBuf = NULL;

	do 
	{
		if (!ParserDeltaNV(pDeltaBuf,dwSize,mapOffset))
		{
			break;
		}
		if (!ParserIndexXml(lpIndexFile,mapOffset,mapIndex))
		{
			break;
		}

		CMapIndex::iterator it;
		BYTE byteEnd[2]={0xff,0xff};
		int nSize = mapIndex.size()*sizeof(DELTA_NV_INFO_T) + DELTA_NV_FILE_NAME_LENGTH + DELTA_NV_VERSION_LENGTH + DELTA_NV_DATA_LENGTH+2;
		lpBuf = new BYTE[nSize];
		if (NULL == lpBuf)
		{
			break;
		}
		memset(lpBuf,0,nSize);
		int nOffset = DELTA_NV_FILE_NAME_LENGTH + DELTA_NV_VERSION_LENGTH;
		memcpy(lpBuf,DELTA_INDEX_NAME,strlen(DELTA_INDEX_NAME));
		*(DWORD*)(lpBuf + nOffset) = nSize;
		nOffset += DELTA_NV_DATA_LENGTH;
		
		for(it = mapIndex.begin(); it != mapIndex.end(); it++)
		{
			DELTA_NV_INFO_T tDeltaInfo = it->second;
			for (int i=0; i<E_OPERATOR_COUNT;++i)
			{
				if (0xFFFFFFFF != tDeltaInfo.agOffset[i])
				{
					tDeltaInfo.agOffset[i] += nSize;
				}
			}
			memcpy(lpBuf + nOffset,&tDeltaInfo,sizeof(DELTA_NV_INFO_T));
			nOffset += sizeof(DELTA_NV_INFO_T);	
		}
		memcpy(lpBuf+nOffset,byteEnd,2);
		memmove(pDeltaBuf+nSize,pDeltaBuf,dwSize);
		memcpy(pDeltaBuf,lpBuf,nSize);
		dwSize += nSize;
		bOK = TRUE;
	} while (0);

	if (lpBuf)
	{
		delete[] lpBuf;
		lpBuf = NULL;
	}
	return bOK;
}


BOOL CGenDeltaNVIndex::ParserDeltaNV(LPBYTE pDeltaBuf,DWORD dwSize,CMapDeltaNV& mapOffsetInfo)
{
	mapOffsetInfo.clear();
	if (NULL == pDeltaBuf ||  28 > dwSize)
	{
		return FALSE;
	}
	for (int nOffset=0; nOffset < (int)dwSize;)
	{
		std::string strName((char*)(pDeltaBuf+nOffset));
		int nLength = *(DWORD*)(pDeltaBuf+nOffset + DELTA_NV_FILE_NAME_LENGTH + DELTA_NV_VERSION_LENGTH);
		transform(strName.begin(), strName.end(), strName.begin(), ::tolower);
		mapOffsetInfo.insert(CMapDeltaNV::value_type(strName, nOffset));
		nOffset += nLength;
	}
	return TRUE;
}

/*
<Operators version="1">
	<Operator carrier="Claro">
		<Item mcc="712", mnc="03"/>	
		<Item mcc="370", mnc="02"/>	
	</Operator>

	<Operator carrier="Default">
		<Item mcc="520", mnc="18"/>	
		<Item mcc="720", mnc="18"/>	
	</Operator>
</Operators>
*/
BOOL CGenDeltaNVIndex::GetOperatorOffset(const string strOperator,CMapDeltaNV& mapOffsetInfo,DELTA_NV_INFO_T& tDeltaInfo)
{
	tDeltaInfo.Clear();
	BOOL bFind = FALSE;
	for(int i=0;i<E_OPERATOR_COUNT;++i)
	{
		std::string strKey = strOperator + agStrPostfix[i];
		CMapDeltaNV::iterator it = mapOffsetInfo.find(strKey);
		if (it == mapOffsetInfo.end())
		{
			printf("[Warning] not find %s delta nv.\n",strKey.c_str());	
		}
		else
		{
			tDeltaInfo.agOffset[i] = it->second;
			bFind = TRUE;
		}

	}
	return bFind;
}
BOOL CGenDeltaNVIndex::ParserIndexXml(LPCTSTR lpXmlFile, CMapDeltaNV& mapOffsetInfo,CMapIndex& mapIndex)
{

	pugi::xml_document doc;
	unsigned int option = pugi::parse_default;
	option &= ~pugi::parse_eol;

	mapIndex.clear();
	pugi::xml_parse_result res = doc.load_file(lpXmlFile, pugi::parse_default);
	if (!res)
	{
		_tprintf(_T("Load XML [%s] fail.\n"),lpXmlFile);
		return FALSE;
	}

	pugi::xml_node root = doc.child(_T("Operators"));
	if (!root)
	{
		_tprintf(_T("Invalid xml,No find Operators Element in XML[%s].\n"),lpXmlFile);
		return FALSE;
	}

	const TCHAR* pTStrValue = NULL; 
	pTStrValue = root.attribute(_T("version")).value();
	if (!pTStrValue)
	{
		_tprintf(_T("Invalid xml,No find Verison attribute in XML[%s].\n"),lpXmlFile);
		return FALSE;
	}

	if (0 != _tcsicmp(pTStrValue,_T("1")))
	{
		_tprintf(_T("Current tool just support version = 1,But this version is %s,Please upgrade nv tool.\n"),pTStrValue);
		return FALSE;
	}

	for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling())
	{
		
		if ( _tcsicmp(child.name(),_T("Operator")) )
		{
			continue;
		}
		pTStrValue = child.attribute(_T("carrier")).value();
		if (NULL == pTStrValue)
		{
			continue;
		}
		DELTA_NV_INFO_T tDeltaInfo;
		std::string strOperator(pTStrValue);
		transform(strOperator.begin(), strOperator.end(), strOperator.begin(), ::tolower);
		if (!GetOperatorOffset(strOperator,mapOffsetInfo,tDeltaInfo))
		{
			continue;
		}
	
		for (pugi::xml_node item = child.first_child(); item; item = item.next_sibling())
		{	
			const TCHAR* pTMccValue = item.attribute(_T("mcc")).value();
			const TCHAR* pTMncValue = item.attribute(_T("mnc")).value();
			if (NULL == pTMccValue || NULL == pTMncValue)
			{
				continue;
			}
			int nMcc = _ttoi(pTMccValue);
			int nMnc = _ttoi(pTMncValue);
			if(_tcslen(pTMncValue)>2)
			{
				nMnc |= 0x8000;
			}
			tDeltaInfo.nMccMnc = nMcc<<16 | nMnc;
			mapIndex.insert(CMapIndex::value_type(tDeltaInfo.nMccMnc,tDeltaInfo));
		}

	}

	return TRUE;
}