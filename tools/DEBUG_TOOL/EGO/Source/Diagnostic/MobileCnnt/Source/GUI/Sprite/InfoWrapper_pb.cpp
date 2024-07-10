#include "stdafx.h"
#include "..\mobilecnnt.h"
#include "InfoWrapper.h"

static void FirstLetter(int   nCode,   WCHAR & strLetter)  
{  
	     if(nCode   >=   1601   &&   nCode   <   1637)   strLetter   =   'A';  
	else if(nCode   >=   1637   &&   nCode   <   1833)   strLetter   =   'B';  
	else if(nCode   >=   1833   &&   nCode   <   2078)   strLetter   =   'C';  
	else if(nCode   >=   2078   &&   nCode   <   2274)   strLetter   =   'D';  
	else if(nCode   >=   2274   &&   nCode   <   2302)   strLetter   =   'E';  
	else if(nCode   >=   2302   &&   nCode   <   2433)   strLetter   =   'F';  
	else if(nCode   >=   2433   &&   nCode   <   2594)   strLetter   =   'G';  
	else if(nCode   >=   2594   &&   nCode   <   2787)   strLetter   =   'H';  
	else if(nCode   >=   2787   &&   nCode   <   3106)   strLetter   =   'J';  
	else if(nCode   >=   3106   &&   nCode   <   3212)   strLetter   =   'K';  
	else if(nCode   >=   3212   &&   nCode   <   3472)   strLetter   =   'L';  
	else if(nCode   >=   3472   &&   nCode   <   3635)   strLetter   =   'M';  
	else if(nCode   >=   3635   &&   nCode   <   3722)   strLetter   =   'N';  
	else if(nCode   >=   3722   &&   nCode   <   3730)   strLetter   =   'O';  
	else if(nCode   >=   3730   &&   nCode   <   3858)   strLetter   =   'P';  
	else if(nCode   >=   3858   &&   nCode   <   4027)   strLetter   =   'Q';  
	else if(nCode   >=   4027   &&   nCode   <   4086)   strLetter   =   'R';  
	else if(nCode   >=   4086   &&   nCode   <   4390)   strLetter   =   'S';  
	else if(nCode   >=   4390   &&   nCode   <   4558)   strLetter   =   'T';  
	else if(nCode   >=   4558   &&   nCode   <   4684)   strLetter   =   'W';  
	else if(nCode   >=   4684   &&   nCode   <   4925)   strLetter   =   'X';  
	else if(nCode   >=   4925   &&   nCode   <   5249)   strLetter   =   'Y';  
	else if(nCode   >=   5249   &&   nCode   <   5590)   strLetter   =   'Z';
	else strLetter=' ';
} 

static void  GetFirstLetter(WCHAR * strName, WCHAR* strFirstLetter, int nLen)  
{  
	unsigned char   ucHigh,   ucLow;  
	int  nCode;  
	TCHAR strRet;  
	memset(strFirstLetter, 0, nLen* sizeof(WCHAR));  
	int len = 0;


	WCHAR *pStr = strName;
	while(*pStr)
	{	
		if(*pStr <= 0xFF)
		{
			if(*pStr != _T('(') && *pStr != _T(')'))
			{
				strFirstLetter[len] = *pStr; 
				len++;
			}
		}
		else
		{
			//omit
			if(*pStr== 0xFF08 || *pStr == 0xFF09)
			{
				pStr++;
				continue;
			}

			TCHAR tmpW[2] = {0};
			char  tmpA[10] = {0};
			tmpW[0] = *pStr;
			WideCharToMultiByte(CP_ACP,0,tmpW,1,tmpA,9,NULL,NULL);

			ucHigh = (unsigned char)tmpA[0];  
			ucLow  = (unsigned char)tmpA[1];  
			if( ucHigh < 0xa1 || ucLow < 0xa1)  
			{
				strFirstLetter[len] = *pStr;  	
				len++; 
			}
			else
			{
				// Treat code by section-position as an int type parameter,  
				// so make following change to nCode. 
				nCode = (ucHigh - 0xa0)*100 + ucLow - 0xa0;  
				FirstLetter(nCode, strRet);  
				if(strRet != ' ')
				{
					strFirstLetter[len] = strRet;  	
					len++;  
				}
				else
				{
					strFirstLetter[len] = *pStr;  	
					len++; 
				}
			}
		}

		pStr++;
	}
}
  

static bool cmp_pb(const PPB &pa,const PPB &pb)
{ 
	TCHAR szA[256] = {0};
	TCHAR szB[256] = {0};
	GetFirstLetter(pa->szName,szA,255);
	GetFirstLetter(pb->szName,szB,255);
 
	if(_tcsicmp(szA,szB) < 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

int CInfoWrapper::GetPBCount(WORD wStore)
{
	int nCount=0;
	int nAll = m_arrPB.size();
	if(wStore == ID_STORE_ALL)
		return nAll;
	
	PPB pPB = NULL;
	for(int i=0;i<nAll;i++)
	{
		pPB = m_arrPB[i];
		if(pPB->wStore == wStore)
			nCount++;
	}
	return nCount;
}

PPB CInfoWrapper::GetPB( int nIdx )
{
	_ASSERTE( nIdx >= 0 && nIdx < m_arrPB.size() );

	return m_arrPB[nIdx];
}

void CInfoWrapper::AddPB( PPB pPB )
{
	_ASSERTE( pPB != NULL );

	//m_arrPB.Add( pPB );
	m_arrPB.push_back(pPB);

	std::sort(m_arrPB.begin(),m_arrPB.end(),cmp_pb);
}

void CInfoWrapper::SetCurSelPB(int nIdx)
{
	_ASSERTE( nIdx >= 0 && nIdx < m_arrPB.size() );

	m_nCurSelPB = nIdx;
}

PPB CInfoWrapper::GetCurSelPB(void)
{
	return m_arrPB[m_nCurSelPB];
}

void CInfoWrapper::RemovePB( int nIdx )
{
	_ASSERTE( nIdx >= 0 && nIdx < m_arrPB.size() );

	PPB pPB = m_arrPB[nIdx];
	delete pPB;
    pPB = NULL;
	//m_arrPB.RemoveAt(nIdx);
	std::vector<PPB>::iterator it;
	it = m_arrPB.begin();
	it += nIdx;
	m_arrPB.erase(it);
}

void CInfoWrapper::RemovePB( PPB ppb )
{
	_ASSERTE( ppb != NULL );

	int nSize = m_arrPB.size();
	for( int i = 0; i < nSize; ++i )
	{
		if( ppb == m_arrPB[i] )
		{
			//delete ppb;
			//ppb = NULL;
			RemovePB(i);
			return;
		}
	}
	_ASSERTE(0);
}

void CInfoWrapper::RemovePhoneSIMPB(void)
{
	int nSize = m_arrPB.size();

	PPB pPb = NULL;
	for( int i = nSize - 1; i >= 0; --i )
	{
		pPb = m_arrPB[i];
		if( pPb->wStore == ID_STORE_PHONE ||
			pPb->wStore == ID_STORE_SIM  || 
			pPb->wStore == ID_STORE_SIM2 ||
			pPb->wStore == ID_STORE_SIM3 ||
			pPb->wStore == ID_STORE_SIM4)
		{
		    //delete pPb;
			//pPb = NULL;
			//m_arrPB.RemoveAt(i);
			RemovePB(i);
		}
	}
}

void CInfoWrapper::RemoveAllPB(void)
{
	int nSize = m_arrPB.size();

	PPB pPB = NULL;
	for( int i = 0; i < nSize; ++i )
	{
		pPB = m_arrPB[i];
		delete pPB;
		pPB = NULL;
	}

	m_arrPB.clear();
}