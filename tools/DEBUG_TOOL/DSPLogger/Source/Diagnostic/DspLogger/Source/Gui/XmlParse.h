// XmlParse.h: interface for the CXmlParse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLPARSE_H__DA4A4059_D745_4BB6_83C5_EF438D8C1318__INCLUDED_)
#define AFX_XMLPARSE_H__DA4A4059_D745_4BB6_83C5_EF438D8C1318__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XMLite.h"

typedef struct _CHART_INFO_T
{
	TCHAR szID[MAX_PATH];		//internal ID
	TCHAR szAlias[MAX_PATH];	//use for GUI display
	TCHAR szDesc[MAX_PATH];		//use for addr description	
	WORD  wAddrCond;
	WORD  wAddr;	
	WORD  wDataCond;
	WORD  wData;
	DWORD dwColor;				// line RGB data
	_CHART_INFO_T()
    {
        memset(this,0,sizeof(_CHART_INFO_T));
		wDataCond = 5;
		wData = 0xFFFF;
    }
}CHART_INFO_T,*PCHART_INFO_PTR;

typedef struct _GRAPH_INFO_T
{

	CHART_INFO_T*		pChartGraph;
	DWORD               dwChartCount;

	_GRAPH_INFO_T()
    {
        memset(this,0,sizeof(_GRAPH_INFO_T));
    }
	void Clear()
	{
		if(pChartGraph)
		{
			delete [] pChartGraph;
			pChartGraph = NULL;
		}
		dwChartCount = 0;
	}

}GRAPH_INFO_T,*PGRAPH_INFO_PTR;

class CXmlParse  
{
public:
	CXmlParse();
	virtual ~CXmlParse();
	BOOL	LoadXml(LPCTSTR lpszConfigName,PGRAPH_INFO_PTR& pGraphInfo);
	void	Release(PGRAPH_INFO_PTR pGraphInfo);
protected:	
    BOOL  _Init(LPCTSTR lpszConfigName);
private:
	XNode       m_xmlConfig;

};

#endif // !defined(AFX_XMLPARSE_H__DA4A4059_D745_4BB6_83C5_EF438D8C1318__INCLUDED_)
