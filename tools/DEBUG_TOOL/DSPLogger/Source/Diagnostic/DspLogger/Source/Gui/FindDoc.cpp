// FindDoc.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "FindDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//subchart color
#define CLR_1   RGB(240, 20, 20)
#define CLR_2   RGB(24,  128, 24)
#define CLR_3   RGB(10, 10, 240)
#define CLR_4   RGB(20, 200, 200)
#define CLR_5   RGB(255, 68, 255)
#define CLR_6   RGB(105, 147, 188)
#define CLR_7   RGB(87, 80, 175)
#define CLR_8   RGB(255, 128, 64)
#define CLR_9   RGB(193,166,170)
#define CLR_10   RGB(153,211,148)
#define CLR_11   RGB(196,226,232)
#define CLR_12   RGB(236,123,140)
#define CLR_13   RGB(151,208,187)
#define CLR_14   RGB(105,6,138)
#define CLR_15   RGB(216,201,143)
#define CLR_16   RGB(228,252,175)
#define CLR_17   RGB(132,210,213)
#define CLR_18   RGB(194,166,165)
#define CLR_19   RGB(203,224,203)
#define CLR_20   RGB(108,134,162)

/////////////////////////////////////////////////////////////////////////////
// CFindDoc

IMPLEMENT_DYNCREATE(CFindDoc, CDocument)

CFindDoc::CFindDoc()
{
    m_nFindDataNum = 0;
	m_nCurveNum = DEFAULT_FIND_DATA_NUM;

    m_arrFindColor[0] = CLR_1;
    m_arrFindColor[1] = CLR_2;
    m_arrFindColor[2] = CLR_3;
    m_arrFindColor[3] = CLR_4;
	m_arrFindColor[4] = CLR_5;
    m_arrFindColor[5] = CLR_6;
    m_arrFindColor[6] = CLR_7;
    m_arrFindColor[7] = CLR_8;
	m_arrFindColor[8] = CLR_9;
    m_arrFindColor[9] = CLR_10;
    m_arrFindColor[10] = CLR_11;
    m_arrFindColor[11] = CLR_12;
	m_arrFindColor[12] = CLR_13;
    m_arrFindColor[13] = CLR_14;
    m_arrFindColor[14] = CLR_15;
    m_arrFindColor[15] = CLR_16;
	m_arrFindColor[16] = CLR_17;
    m_arrFindColor[17] = CLR_18;
    m_arrFindColor[18] = CLR_19;
    m_arrFindColor[19] = CLR_20;
	m_bAutoRefresh	   = FALSE;
	LoadSetting();

/*	
	m_arrFindColor[4] = CLR_1;
	m_arrFindColor[5] = CLR_1;
	m_arrFindColor[6] = CLR_1;
	m_arrFindColor[7] = CLR_1;
*/
}//lint !e1401

BOOL CFindDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CFindDoc::~CFindDoc()
{
}


BEGIN_MESSAGE_MAP(CFindDoc, CDocument)
	//{{AFX_MSG_MAP(CFindDoc)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindDoc diagnostics

#ifdef _DEBUG
void CFindDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFindDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFindDoc serialization

void CFindDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{

	}
	else
	{
	
	}
}
void CFindDoc::LoadSetting() 
{
	_TCHAR szConfigFile[_MAX_PATH]={0};
	_stprintf(szConfigFile,_T("%s\\DspLogger.ini"),GetModuleDir(FALSE));
	m_dwDrawMaxPoints=GetPrivateProfileInt( _T("Settings"), _T("DrawMaxPoints"), 20*10000, szConfigFile );
}

/////////////////////////////////////////////////////////////////////////////
// CFindDoc commands
BOOL CFindDoc::AddFindData(WORD wAddrFlag,CString strAddr,WORD wDataFlag, CString strData, DWORD dwFlag /*= FLAG_GENERAL*/,CHAR* pszTpAlias/*=NULL*/)
{
	if (m_nFindDataNum == m_nCurveNum) return FALSE;

	int nFindItem=-1;
	BOOL bDesc = FALSE;
	if(dwFlag == FLAG_DUALITY)
	{
		strAddr.Replace("*", "0");
		strData.Replace("*", "F");
	}
	else if(dwFlag == FLAG_DESCRIPTION)
	{
		for( int i = 0; i < m_nFindDataNum; ++i )
		{
			if( m_arrFindData[i].wAddrFlag == wAddrFlag && 
				strAddr.CompareNoCase(m_arrFindData[i].szDescription) == 0 )
			{
				nFindItem= i;						
			}
		}
		bDesc = TRUE;
	}

	if(!bDesc)
	{
		for( int i = 0; i < m_nFindDataNum; ++i )
		{
			if( m_arrFindData[i].wAddrFlag == wAddrFlag && 
				strAddr.CompareNoCase(m_arrFindData[i].szAddrValue) == 0 )
			{
				if( m_arrFindData[i].wDataFlag == wDataFlag &&
					strData.CompareNoCase(m_arrFindData[i].szDataValue) == 0 )
				{

					if ( pszTpAlias && strlen(pszTpAlias) )
					{
						if (0 == strcmp(m_arrFindData[i].szAlias,pszTpAlias))
						{
							nFindItem = i;	
						}
						
					}
					else
					{
						if ( 0 == strlen(m_arrFindData[i].szAlias) )
						{
							nFindItem = i;	
						}
					}

					break;
				}
			}
		}
	}

	if(nFindItem != -1) return FALSE; // has been added
 	if (pszTpAlias && strlen(pszTpAlias))
 	{
 		strncpy(m_arrFindData[m_nFindDataNum].szAlias,pszTpAlias,TP_ALIAS_SIZE);
 	}
	
    m_arrFindData[m_nFindDataNum].wAddrFlag = wAddrFlag;
    strcpy(m_arrFindData[m_nFindDataNum].szAddrValue, strAddr);
    m_arrFindData[m_nFindDataNum].wDataFlag = wDataFlag;
    strcpy(m_arrFindData[m_nFindDataNum].szDataValue, strData);

    m_arrFindData[m_nFindDataNum].dwFindCount = 0;
    m_arrFindData[m_nFindDataNum].dwReserve1  = 0;
    m_arrFindData[m_nFindDataNum].dwReserve2  = 0;
	
	if(dwFlag == FLAG_DUALITY)
	{
		m_arrFindData[m_nFindDataNum].dwReserve2  = TP_A_LESS_VALUE_LESS_B;
	}
	else if(dwFlag == FLAG_DESCRIPTION)
	{
		strcpy(m_arrFindData[m_nFindDataNum].szDescription, strAddr);
		m_arrFindData[m_nFindDataNum].dwReserve2  = TP_DESCRIPTION;
	}
    
	m_nFindDataNum++;
	return TRUE;			
}

BOOL CFindDoc::DelFindData(WORD wAddrFlag,CString strAddr,WORD wDataFlag, CString strData,CHAR* pszTpAlias/*=NULL*/)
{
	int nFindItem=-1;
	BOOL bDesc = FALSE;

	if(wAddrFlag > TP_DESCRIPTION)
	{
		strAddr.Replace("*", "0");
		strData.Replace("*", "F");
	}
	else if(wAddrFlag == TP_DESCRIPTION)
	{
		for( int i = 0; i < m_nFindDataNum; ++i )
		{
			if( m_arrFindData[i].wAddrFlag == wAddrFlag && 
				strAddr.CompareNoCase(m_arrFindData[i].szDescription) == 0 )
			{
				nFindItem= i;						
			}
		}
		bDesc = TRUE;
	}
	
	if(!bDesc)
	{
		for( int i = 0; i < m_nFindDataNum; ++i )
		{
			if( m_arrFindData[i].wAddrFlag == wAddrFlag && 
				strAddr.CompareNoCase(m_arrFindData[i].szAddrValue) == 0 )
			{
				if( m_arrFindData[i].wDataFlag == wDataFlag &&
					strData.CompareNoCase(m_arrFindData[i].szDataValue) == 0 )
				{
					if ( pszTpAlias && strlen(pszTpAlias) )
					{
						if (0 == strcmp(m_arrFindData[i].szAlias,pszTpAlias))
						{
							nFindItem = i;	
							strcpy(m_arrFindData[i].szAlias,_T(""));
						}
						
					}
					else
					{
						if ( 0 == strlen(m_arrFindData[i].szAlias) )
						{
							nFindItem = i;	
						}
					}
										
				}
			}
		}
	}
	
	if(nFindItem == -1) return FALSE;
	
	for( int i = nFindItem; i < m_nFindDataNum; ++i )
	{
		memcpy(m_arrFindData + i, m_arrFindData + i + 1, sizeof(TP_FIND_DATA));
	}

	m_nFindDataNum--;

	return TRUE;			
}

int CFindDoc::GetFindDataIdx(WORD wAddrFlag,CString strAddr,WORD wDataFlag, CString strData)
{
	int nFindItem=-1;

	if(wAddrFlag > TP_DESCRIPTION)
	{
		strAddr.Replace("*", "0");
		strData.Replace("*", "F");
	}
	else if(wAddrFlag == TP_DESCRIPTION)
	{
		for( int i = 0; i < m_nFindDataNum; ++i )
		{
			if( m_arrFindData[i].wAddrFlag == wAddrFlag && 
				strAddr.CompareNoCase(m_arrFindData[i].szDescription) == 0 )
			{
				return i;						
			}
		}
	}

	for( int i = 0; i < m_nFindDataNum; ++i )
	{
		if( m_arrFindData[i].wAddrFlag == wAddrFlag && 
			strAddr.CompareNoCase(m_arrFindData[i].szAddrValue) == 0 )
		{
			if( m_arrFindData[i].wDataFlag == wDataFlag &&
				strData.CompareNoCase(m_arrFindData[i].szDataValue) == 0 )
			{
				nFindItem= i;	
				break;
			}
		}
	}
		
	return nFindItem;
}

void CFindDoc::OnButtonClear() 
{
    UpdateAllViews(NULL,WM_UPDATE_CLEAR);
	
}

void CFindDoc::SetCurveNum(int nCurveNum)
{
	m_nCurveNum = nCurveNum;
}