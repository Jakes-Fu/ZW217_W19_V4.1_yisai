// XmlParse.cpp: implementation of the CXmlParse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XmlParse.h"
#include <atlconv.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXmlParse::CXmlParse()
{
	
}

CXmlParse::~CXmlParse()
{
	m_xmlConfig.Close();
}
void	CXmlParse::Release(PGRAPH_INFO_PTR pGraphInfo)
{
	if (pGraphInfo)
	{
		pGraphInfo->Clear();
		SAFE_DELETE(pGraphInfo);
	}
}
BOOL CXmlParse::LoadXml(LPCTSTR lpszConfigName,PGRAPH_INFO_PTR& pGraphInfo)
{
	USES_CONVERSION;
	BOOL bOK = FALSE;
	m_xmlConfig.Close();
	if(!_Init(lpszConfigName))
	{
		return FALSE;
	}

	LPXNode lpChild = NULL;
	lpChild = m_xmlConfig.GetChild(_T("ChartList") );
	
	if(lpChild == NULL) 
	{
		return FALSE;
	}
	
	int i = 0;
	CString strValue;
	XNodes xnChartNodes;
	xnChartNodes = lpChild->GetChilds(_T("Chart") );
	int  nChartCount = xnChartNodes.size();
	if ( nChartCount)
	{
		pGraphInfo = new GRAPH_INFO_T;
		if (NULL == pGraphInfo)
		{
			return FALSE;
		}
		pGraphInfo->pChartGraph = new CHART_INFO_T[nChartCount];
		if ( NULL == pGraphInfo->pChartGraph)
		{
			SAFE_DELETE(pGraphInfo);
			return FALSE;
		}
		bOK = TRUE;
		memset(pGraphInfo->pChartGraph,0,sizeof(CHART_INFO_T)*nChartCount);
		pGraphInfo->dwChartCount = nChartCount;
		for( i = 0; i < nChartCount; ++i)
		{		
			PCHART_INFO_PTR pIT = pGraphInfo->pChartGraph + i;
			strValue= xnChartNodes[i]->GetChildValue(_T("ID"));
			_tcscpy(pIT->szID,strValue);
			
			strValue= xnChartNodes[i]->GetChildValue(_T("Alias"));
			_tcscpy(pIT->szAlias,strValue);
			

			LPXNode lpAddrNode = NULL;
			lpAddrNode = xnChartNodes[i]->GetChild(_T("Addr"));
			if (lpAddrNode)
			{
				strValue = lpAddrNode->GetAttrValue(_T("condition"));
				_stscanf(strValue,_T("%d"),&(pIT->wAddrCond));
				if (pIT->wAddrCond>6 || pIT->wAddrCond<0)
				{
					pIT->wAddrCond = 0;
				}
				
				strValue = lpAddrNode->GetAttrValue(_T("data"));
				strValue.MakeLower();
				if(strValue.Find(_T("0x")) == 0 )
				{
					_stscanf(strValue,_T("0x%x"),&(pIT->wAddr));
				}
				else
				{
					_stscanf(strValue,_T("%d"),&(pIT->wAddr));
				}
			}

			LPXNode lpDataNode = NULL;
			lpDataNode = xnChartNodes[i]->GetChild(_T("Data"));
			if (lpDataNode)
			{
				strValue = lpDataNode->GetAttrValue(_T("condition"));
				_stscanf(strValue,_T("%d"),&(pIT->wDataCond));

				if (pIT->wDataCond>6 || pIT->wDataCond<0)
				{
					pIT->wAddrCond = 5;
				}
				
				strValue = lpDataNode->GetAttrValue(_T("data"));
				strValue.MakeLower();
				if (0 == strValue.CompareNoCase(_T("****")))
				{
					pIT->wData = 0xFFFF;
				}
				else
				{
					if(strValue.Find(_T("0x")) == 0 )
					{
						_stscanf(strValue,_T("0x%x"),&(pIT->wData));
					}
					else
					{
						_stscanf(strValue,_T("%d"),&(pIT->wData));
					}
				}
				
			}
	
			strValue = xnChartNodes[i]->GetChildValue(_T("RGB"));
			strValue.MakeLower();
			if(strValue.Find(_T("0x")) == 0 )
			{
				_stscanf(strValue,_T("0x%x"),&(pIT->dwColor));
			}
			else
			{
				_stscanf(strValue,_T("%d"),&(pIT->dwColor));
			}

			strValue= xnChartNodes[i]->GetChildValue(_T("Description"));
			_tcscpy(pIT->szDesc,strValue);
		}
		xnChartNodes.clear();
	   
	}

	
	return bOK;
}

BOOL CXmlParse::_Init(LPCTSTR lpszConfigName)
{
	_ASSERTE(lpszConfigName != NULL);
	
	// Open file
	CString strText;
	CString strNotes;
	CFile file;
	if ( !file.Open( lpszConfigName, CFile::modeRead ) )
	{
        _tprintf(_T("Unable to open xml config file"));
		return FALSE;
	}
	int nFileLen = (int)file.GetLength();
	
	// Allocate buffer for binary file data
	char* pBuffer = new char[nFileLen + 1];
	memset(pBuffer,0,nFileLen + 1);
	nFileLen = file.Read( pBuffer, nFileLen );
	file.Close();
	pBuffer[nFileLen] = '\0';
	
	/*
	_TCHAR *pBufferU = NULL;
	if(pBuffer[0] != (UCHAR)0xFF || pBuffer[1] != (UCHAR)0xFE)//not unicode 
	{
		pBufferU = new _TCHAR[nFileLen+1];
		memset(pBufferU,0,sizeof(_TCHAR)*(nFileLen + 1));
		MultiByteToWideChar(CP_ACP,0,pBuffer,nFileLen+1,pBufferU,nFileLen+1);
		strText.Empty();
		strText = pBufferU;
	}
	else
	{
		strText = (LPTSTR)pBuffer;
	}*/
	strText = (LPTSTR)pBuffer;
	
	delete [] pBuffer;
	pBuffer = NULL;

	
	CString strXmlRet;
	strXmlRet = m_xmlConfig.Load(strText);
	strText.Empty();
	
	if(strXmlRet.IsEmpty())
	{
		return FALSE;
	}
	return TRUE;
}