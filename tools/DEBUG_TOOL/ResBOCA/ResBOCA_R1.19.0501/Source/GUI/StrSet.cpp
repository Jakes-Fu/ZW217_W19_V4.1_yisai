// StrSet.cpp: implementation of the CStrSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResOver.h"
#include "StrSet.h"
#include "GlobalDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStrSet::CStrSet()
{
	m_bAllUnicode = TRUE;
	m_bStrNoFlag = FALSE;
	m_nPrecomposedFlag = 0;
	m_nLangNum = 0;
	
	m_prcConverter = NULL;
}

CStrSet::~CStrSet()
{
	ClearAll();
	m_prcConverter= NULL;
}


BOOL CStrSet::AddString(LPCTSTR pszID,CStringArray &agRow)
{
	if(pszID== NULL)
	{
		return FALSE;
	}
	
    PX_TEXT_INFO_T pTextInfo = new X_TEXT_INFO_T;
    if( pTextInfo == NULL )
        return FALSE;
	
	_tcscpy(pTextInfo->szID,pszID);
	
    //语言的指针所占的字节数
	int nSize=agRow.GetSize()* sizeof(char *);
	
	int nLen=0;
	int nCodeType = 0;
	
	DWORD dwFlag = 0;
	if(m_bAllUnicode)
	{
		dwFlag |= 0x1;
	}
	
	if(m_bStrNoFlag)
	{
		dwFlag |= 0x2;
	}
	
	for(int i=0; i<agRow.GetSize();i++)
	{
		nLen = agRow.GetAt(i).GetLength();
		
		_TCHAR *pText = new _TCHAR[nLen+1];
		memset(pText,0,sizeof(_TCHAR)*(nLen+1));
		_tcscpy(pText,agRow.GetAt(i));
		pTextInfo->agText[i].pszRawText = pText;
		pTextInfo->agText[i].dwRawSize = (nLen+1)*sizeof(_TCHAR);			
		m_prcConverter->ConvertTextString(pTextInfo->agText[i].pszRawText,
			(LPBYTE&)(pTextInfo->agText[i].pbResText),
			(int&)(pTextInfo->agText[i].dwResSize),
			nCodeType,FALSE,m_nPrecomposedFlag,dwFlag);
		pTextInfo->agText[i].wFlag = (WORD)nCodeType;
		nSize+=(int)(pTextInfo->agText[i].dwResSize);
		
		int nIndex = GetTextRepeatIndex(pTextInfo->agText[i].pszRawText,i,TRUE);
		if(nIndex != -1)
		{
			pTextInfo->agText[i].wRepeat = 0;
			pTextInfo->agText[i].wRepeat = TEXT_SET_REPEAT(pTextInfo->agText[i].wRepeat);
			pTextInfo->agText[i].dwRepeatIndex = (DWORD)nIndex;
		}
	} 
	
	pTextInfo->dwIsChecked = 1;
	pTextInfo->dwIsExist = 1;  	
    
    m_mapOrgStr.SetAt( pszID, pTextInfo );		
	m_agOrgStrID.Add(pszID);
	
    return TRUE;

}

BOOL CStrSet::RepString(LPCTSTR pszID,CStringArray *pagRow)
{
	if(pszID== NULL)
	{
		return FALSE;
	}
	
	CStringArray agRow;

	if(pagRow == NULL)
	{
		GetOrgStr(pszID,agRow);
	}
	else
	{
		agRow.Copy(*pagRow);
	}

	
    PX_TEXT_INFO_T pTextInfo = new X_TEXT_INFO_T;
    if( pTextInfo == NULL )
        return FALSE;
	
	_tcscpy(pTextInfo->szID,pszID);
	
    //语言的指针所占的字节数
	int nSize=agRow.GetSize()* sizeof(char *);
	
	int nLen=0;
	int nCodeType = 0;
	
	DWORD dwFlag = 0;
	if(m_bAllUnicode)
	{
		dwFlag |= 0x1;
	}
	
	if(m_bStrNoFlag)
	{
		dwFlag |= 0x2;
	}
	
	for(int i=0; i<agRow.GetSize();i++)
	{
		nLen = agRow.GetAt(i).GetLength();
		
		_TCHAR *pText = new _TCHAR[nLen+1];
		memset(pText,0,sizeof(_TCHAR)*(nLen+1));
		_tcscpy(pText,agRow.GetAt(i));
		pTextInfo->agText[i].pszRawText = pText;
		pTextInfo->agText[i].dwRawSize = (nLen+1)*sizeof(_TCHAR);			
		m_prcConverter->ConvertTextString(pTextInfo->agText[i].pszRawText,
			(LPBYTE&)(pTextInfo->agText[i].pbResText),
			(int&)(pTextInfo->agText[i].dwResSize),
			nCodeType,FALSE,m_nPrecomposedFlag,dwFlag);
		pTextInfo->agText[i].wFlag = (WORD)nCodeType;
		nSize+=(int)(pTextInfo->agText[i].dwResSize);
		
		int nIndex = GetTextRepeatIndex(pTextInfo->agText[i].pszRawText,i,FALSE);
		if(nIndex != -1)
		{
			pTextInfo->agText[i].wRepeat = 0;
			pTextInfo->agText[i].wRepeat = TEXT_SET_REPEAT(pTextInfo->agText[i].wRepeat);
			pTextInfo->agText[i].dwRepeatIndex = (DWORD)nIndex;
		}
	} 
	
	pTextInfo->dwIsChecked = 1;
	pTextInfo->dwIsExist = 1;  	
	
    
    m_mapDstStr.SetAt( pszID, pTextInfo );	
	m_agDstStrID.Add(pszID);
	
    return TRUE;
	
}

int  CStrSet::GetTextRepeatIndex(LPCTSTR pszStr, int nLangIdx,BOOL bOrg)
{
	int nSize = 0;
	int nIndex = -1;

	if(bOrg)
	{
		nSize = m_agOrgStrID.GetSize();
		
		if(m_mapOrgStrInt[nLangIdx].Lookup(pszStr,nIndex))
		{
			return nIndex;
		}
		else
		{
			nIndex = -1;
			m_mapOrgStrInt[nLangIdx].SetAt(pszStr,nSize);
		}
	}
	else
	{
		nSize = m_agDstStrID.GetSize();
		
		if(m_mapDstStrInt[nLangIdx].Lookup(pszStr,nIndex))
		{
			return nIndex;
		}
		else
		{
			nIndex = -1;
			m_mapDstStrInt[nLangIdx].SetAt(pszStr,nSize);
		}
	}

	
	return nIndex;
}

void CStrSet::CalcTextCrossRepeat(int nBaseLangIndex,BOOL bOrg)
{
	int nSize = 0;
	int nIndex = -1;
	int i=0,j=0;
	
	if(bOrg)
	{
		nSize = m_agOrgStrID.GetSize();
		for(i = 0; i< nSize; i++)
		{
			PX_TEXT_INFO_T pTextInfo = NULL;
			// not find ID, concinue
			if( !m_mapOrgStr.Lookup(m_agOrgStrID[i], pTextInfo) )
				continue;
			
			for(j=0; j<m_nLangNum;j++)
			{
				// the lang is the base lang, continue
				if( j == nBaseLangIndex)
					continue;
				
				// the lang string is repeat
				if(TEXT_IS_REPEAT(pTextInfo->agText[j].wRepeat))
				{
					// the lang string is repeat but repeat index is not base lang
					if(!TEXT_INDEX_IS_BASE_EN(pTextInfo->agText[j].wRepeat))
					{
						int nIndex2 = (int)pTextInfo->agText[j].dwRepeatIndex;
						
						PX_TEXT_INFO_T pTextInfo2 = NULL;
						
						if( m_mapOrgStr.Lookup(m_agOrgStrID[nIndex2], pTextInfo2) )
						{	
							if(TEXT_INDEX_IS_BASE_EN(pTextInfo2->agText[j].wRepeat))
							{
								// update index and repeat flag
								pTextInfo->agText[j].wRepeat = pTextInfo2->agText[j].wRepeat;
								pTextInfo->agText[j].dwRepeatIndex = pTextInfo2->agText[j].dwRepeatIndex;
							}
						}			
					}
					
					continue;								
				}
				
				nIndex = -1;
				
				if(m_mapOrgStrInt[nBaseLangIndex].Lookup(pTextInfo->agText[j].pszRawText,nIndex) && nIndex != -1)
				{
					pTextInfo->agText[j].wRepeat=TEXT_SET_REPEAT(pTextInfo->agText[j].wRepeat);
					pTextInfo->agText[j].wRepeat=TEXT_SET_BASE_EN_FLAG(pTextInfo->agText[j].wRepeat);
					pTextInfo->agText[j].dwRepeatIndex = (DWORD)nIndex;
				}
			}	
		}
	}
	else
	{
		nSize = m_agDstStrID.GetSize();
		for(i = 0; i< nSize; i++)
		{
			PX_TEXT_INFO_T pTextInfo = NULL;
			// not find ID, concinue
			if( !m_mapDstStr.Lookup(m_agDstStrID[i], pTextInfo) )
				continue;
			
			for(j=0; j<m_nLangNum;j++)
			{
				// the lang is the base lang, continue
				if( j == nBaseLangIndex)
					continue;
				
				// the lang string is repeat
				if(TEXT_IS_REPEAT(pTextInfo->agText[j].wRepeat))
				{
					// the lang string is repeat but repeat index is not base lang
					if(!TEXT_INDEX_IS_BASE_EN(pTextInfo->agText[j].wRepeat))
					{
						int nIndex2 = (int)pTextInfo->agText[j].dwRepeatIndex;
						
						PX_TEXT_INFO_T pTextInfo2 = NULL;
						
						if( m_mapDstStr.Lookup(m_agDstStrID[nIndex2], pTextInfo2) )
						{	
							if(TEXT_INDEX_IS_BASE_EN(pTextInfo2->agText[j].wRepeat))
							{
								// update index and repeat flag
								pTextInfo->agText[j].wRepeat = pTextInfo2->agText[j].wRepeat;
								pTextInfo->agText[j].dwRepeatIndex = pTextInfo2->agText[j].dwRepeatIndex;
							}
						}			
					}
					
					continue;								
				}
				
				nIndex = -1;
				
				if(m_mapDstStrInt[nBaseLangIndex].Lookup(pTextInfo->agText[j].pszRawText,nIndex) && nIndex != -1)
				{
					pTextInfo->agText[j].wRepeat=TEXT_SET_REPEAT(pTextInfo->agText[j].wRepeat);
					pTextInfo->agText[j].wRepeat=TEXT_SET_BASE_EN_FLAG(pTextInfo->agText[j].wRepeat);
					pTextInfo->agText[j].dwRepeatIndex = (DWORD)nIndex;
				}
			}	
		}
	}

}
void CStrSet::ClearAll()
{
	Clear(TRUE);
	Clear(FALSE);
}

void CStrSet::Clear(BOOL bOrg)
{
	if(bOrg)
	{
		POSITION pos;
		PX_TEXT_INFO_T pTextInfo = NULL;
		CString strKey;
		for( pos = m_mapOrgStr.GetStartPosition(); pos != NULL; )
		{
			m_mapOrgStr.GetNextAssoc( pos, strKey, pTextInfo );
			for(int i=0;i<m_nLangNum;i++)
			{
				m_prcConverter->ReleaseMemory(pTextInfo->agText[i].pbResText);
				if(pTextInfo->agText[i].pszRawText)
				{
					//TRACE(_T("%s\n"),pTextInfo->agText[i].pszRawText);
					delete [] pTextInfo->agText[i].pszRawText;
				}	
			}
			delete pTextInfo;
		}
		m_mapOrgStr.RemoveAll();
		m_agOrgStrID.RemoveAll();
		for(int i=0; i< 256;i++)
		{
			m_mapOrgStrInt[i].RemoveAll();
		}
	}
	else
	{
		POSITION pos;
		PX_TEXT_INFO_T pTextInfo = NULL;
		CString strKey;
		for( pos = m_mapDstStr.GetStartPosition(); pos != NULL; )
		{
			m_mapDstStr.GetNextAssoc( pos, strKey, pTextInfo );
			for(int i=0;i<m_nLangNum;i++)
			{
				m_prcConverter->ReleaseMemory(pTextInfo->agText[i].pbResText);
				if(pTextInfo->agText[i].pszRawText)
				{
					//TRACE(_T("%s\n"),pTextInfo->agText[i].pszRawText);
					delete [] pTextInfo->agText[i].pszRawText;
				}	
			}
			delete pTextInfo;
		}
		m_mapDstStr.RemoveAll();
		m_agDstStrID.RemoveAll();
		for(int i=0; i< 256;i++)
		{
			m_mapDstStrInt[i].RemoveAll();
		}
	}
}

BOOL CStrSet::GetOrgStr( LPCTSTR pszID, CStringArray &agRow )
{	
    agRow.RemoveAll();
	
    PX_TEXT_INFO_T pTextInfo = NULL;
    m_mapOrgStr.Lookup(pszID, pTextInfo);
	
	if(pTextInfo != NULL)
	{
		for(int i = 0; i < m_nLangNum; i++)
		{
			agRow.Add(pTextInfo->agText[i].pszRawText);
		}	
	}	
	
    return agRow.GetSize();
}

BOOL CStrSet::GetStrData(LPBYTE &pBuf, DWORD &dwSize, DWORD dwBase,BOOL bBigEdn)
{
	dwSize = CalcTextInfo();
    int nCount = m_agDstStrID.GetSize();
    if( nCount == 0 )
        return TRUE;

	pBuf = new BYTE[dwSize];
	memset(pBuf,0,dwSize);
	LPBYTE pCur = pBuf;
	LPBYTE pEnd = pBuf + dwSize;
///////////////////////////////////////////////////////////////////////////////
#define SAVE_DATA(des, st)      do{			\
    if( pCur + st > pEnd )					\
    {                                       \
        _ASSERTE(0);                        \
		delete []pBuf;						\
		delete []pKspText;					\
		pBuf = NULL;						\
		dwSize = 0;							\
        return FALSE;                       \
    }										\
	else									\
	{										\
		memcpy(pCur,des,st);				\
		pCur += st;							\
	}}while(0)								
///////////////////////////////////////////////////////////////////////////////

	PKSP_TEXT_T pKspText = (PKSP_TEXT_T)new KSP_TEXT_T[m_nLangNum];
	if(NULL== pKspText)
	  return FALSE;
	memset(pKspText,0,sizeof(KSP_TEXT_T)*m_nLangNum);

	int i = 0,j=0;
	DWORD dwCurOffset = (DWORD)(dwBase + sizeof(KSP_TEXT_T)*m_nLangNum);
	for(i = 0; i < m_nLangNum; i++)
	{			
		pKspText[i].pTxtTable = (char *)dwCurOffset;
		dwCurOffset = dwCurOffset + (DWORD)(sizeof(char *) * nCount);		
		pKspText[i].nbText = 0;// (uint16)m_nLangNum;
	}

	PX_TEXT_INFO_T pTxtInfo = NULL;
	PX_TEXT_INFO_T pTxtInfo2 = NULL;
	// compute each language text data size;
	for(i=0; i< m_nLangNum; i++)
	{
		pKspText[i].nbText = 0;
		DWORD dwLangTextSize = 0;
		
		for(j = 0; j< nCount; j++)
		{
			pTxtInfo = GetTextInfo(j);
			if(pTxtInfo != NULL && pTxtInfo->dwIsChecked)
			{
				if(!TEXT_IS_REPEAT(pTxtInfo->agText[i].wRepeat))
				{
					dwLangTextSize += Addr4ByteAlign(pTxtInfo->agText[i].dwResSize);
				}
			}
			
		}
		dwLangTextSize /= 4;
		pKspText[i].nbText = (uint16)dwLangTextSize;
	
	}

    if( bBigEdn )
    {
		for(i=0;i<m_nLangNum;i++)
		{			
			pKspText[i].pTxtTable = (char *)ConvEdn_INT((DWORD)pKspText[i].pTxtTable);
			pKspText[i].nbText = LitEdnToBigEdn(pKspText[i].nbText);
		}        
    }

	SAVE_DATA(pKspText, sizeof(KSP_TEXT_T)*m_nLangNum); //lint !e670

    if( bBigEdn )
    {
 		for(i=0;i<m_nLangNum;i++)
		{			
			pKspText[i].pTxtTable = (char *)ConvEdn_INT((DWORD)pKspText[i].pTxtTable);
			pKspText[i].nbText = BigEdnToLitEdn(pKspText[i].nbText);
		} 
    }

  
	uint32 nAddr = dwCurOffset; //lint !e661
    nAddr = Addr4ByteAlign(nAddr);

    pTxtInfo = NULL;
	uint32 nNullAddr = 0; 
	int nLangBase = 0;

	for(j=0; j < m_nLangNum; j++)
	{		
		for( i = 0; i < nCount; ++i )
		{
			pTxtInfo = GetTextInfo(i);

			if(pTxtInfo == NULL || pTxtInfo->dwIsChecked == FALSE)
			{
				SAVE_DATA(&nNullAddr, sizeof(nNullAddr));
				continue;
			}
		
			if(!TEXT_IS_REPEAT(pTxtInfo->agText[j].wRepeat))
			{
				if( bBigEdn ) 
					nAddr = ConvEdn_INT(nAddr);

				SAVE_DATA(&nAddr, sizeof(nAddr));

				if( bBigEdn ) 
					nAddr = ConvEdn_INT(nAddr);

				pTxtInfo->agText[j].dwOffset = nAddr;

				nAddr += pTxtInfo->agText[j].dwResSize;			 

				nAddr = Addr4ByteAlign(nAddr);
			}
			else
			{
				uint32 nAddTmp = 0;

				pTxtInfo2 = GetTextInfo(pTxtInfo->agText[j].dwRepeatIndex);
				if(TEXT_INDEX_IS_BASE_EN(pTxtInfo->agText[j].wRepeat))
				{
					nAddTmp = pTxtInfo2->agText[nLangBase].dwOffset;
				}
				else
				{
					nAddTmp = pTxtInfo2->agText[j].dwOffset;
				}					

				if( bBigEdn ) 
					nAddTmp = ConvEdn_INT(nAddTmp);				

				SAVE_DATA(&nAddTmp, sizeof(nAddTmp));

				if( bBigEdn ) 
					nAddTmp = ConvEdn_INT(nAddTmp);
	
				pTxtInfo->agText[j].dwOffset = nAddTmp;

			}
		}
	
	}


    BYTE arrAlign[] = { 0x00, 0x00, 0x00, 0x00 };
    int  nSize = 0;
	
	int nHeadSkip = 2;
	if(m_bStrNoFlag)
	{
		nHeadSkip = 0;
	}

	for( j = 0; j < m_nLangNum; j++ )
	{		
		for( i = 0; i < nCount; ++i )
		{
			pTxtInfo = GetTextInfo(i);

			if(pTxtInfo != NULL /*&& pTxtInfo->dwIsChecked*/)
			{
				// not write the repeated string
				if(TEXT_IS_REPEAT(pTxtInfo->agText[j].wRepeat))
				{
					continue;
				}
				nSize = pTxtInfo->agText[j].dwResSize;

				if(nSize != 0 && pTxtInfo->agText[j].pbResText != NULL)
				{
					if(pTxtInfo->agText[j].wFlag == 0) //ASCII
					{			
						SAVE_DATA(pTxtInfo->agText[j].pbResText,nSize);
					}
					else //Unicode
					{	
						//都要存为Big Endian，都需要转换
						// @hongliang.xin 2008-12-9 changed from James Zhang
						// arm big, win little
						if(bBigEdn) //转换
							LitEdnToBigEdn((LPWORD)(pTxtInfo->agText[j].pbResText +nHeadSkip),(nSize-nHeadSkip)/2);

						SAVE_DATA(pTxtInfo->agText[j].pbResText,pTxtInfo->agText[j].dwResSize);
						
						if(bBigEdn)//转换回来
							LitEdnToBigEdn((LPWORD)(pTxtInfo->agText[j].pbResText +nHeadSkip),(nSize-nHeadSkip)/2);
						
					}	 

					nAddr = Addr4ByteAlign(nSize);
					SAVE_DATA(arrAlign, nAddr - nSize);
				
				}
			}
		}
	
	}

	delete [] pKspText;
	
    return TRUE;

}

DWORD CStrSet::CalcTextInfo()
{
	DWORD uSize = 0;
	
	int nCount = m_agDstStrID.GetSize();
	PX_TEXT_INFO_T pTextInfo = NULL;
	for(int i=0;i<nCount;i++)
	{
		CString strID = m_agDstStrID[i];
		m_mapDstStr.Lookup(strID,pTextInfo);		
		
		for(int j = 0; j < m_nLangNum; j++)
		{
			
			//语言的指针所占的字节数
			uSize += sizeof(char *);
			if(!TEXT_IS_REPEAT(pTextInfo->agText[j].wRepeat))
			{
				uSize += Addr4ByteAlign(pTextInfo->agText[j].dwResSize);
			}
			
		}
	}
	
	if( nCount > 0 )
	{        
		uSize += sizeof(KSP_TEXT_T) * m_nLangNum;
	}

	return uSize;
}

PX_TEXT_INFO_T CStrSet::GetTextInfo(int nIndex)
{
	PX_TEXT_INFO_T pTextInfo = NULL;
	CString strID = m_agOrgStrID[nIndex];
    m_mapDstStr.Lookup(strID,pTextInfo);
	return pTextInfo;
}

WORD CStrSet::BigEdnToLitEdn( WORD wBigEdn )
{
	return MAKEWORD(HIBYTE(wBigEdn), LOBYTE(wBigEdn)); 
}

BOOL CStrSet::BigEdnToLitEdn( LPWORD pwBigEdn, int nNum )
{
	_ASSERTE( pwBigEdn != NULL );
	if(pwBigEdn == NULL)
		return FALSE;
	
	for( int i = 0; i < nNum; ++i, ++pwBigEdn )
	{
		*pwBigEdn = BigEdnToLitEdn(*pwBigEdn);
	}
	return TRUE;
}

WORD CStrSet::LitEdnToBigEdn( WORD wLitEdn )
{
	return MAKEWORD(HIBYTE(wLitEdn), LOBYTE(wLitEdn)); 
}

BOOL CStrSet::LitEdnToBigEdn( LPWORD pwLitEdn, int nNum )
{
	_ASSERTE( pwLitEdn != NULL );
	if(pwLitEdn == NULL)
		return FALSE;
	
	for( int i = 0; i < nNum; ++i, ++pwLitEdn )
	{
		*pwLitEdn = LitEdnToBigEdn(*pwLitEdn);
	}
	
	return TRUE;
}

DWORD CStrSet::ConvEdn_INT( DWORD dwEdn )
{
	return MAKELONG( MAKEWORD(HIBYTE(HIWORD(dwEdn)), LOBYTE(HIWORD(dwEdn))),
		MAKEWORD(HIBYTE(LOWORD(dwEdn)), LOBYTE(LOWORD(dwEdn)))); 
}