// VCalendar.cpp: implementation of the CVCalendar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VCalendar.h"
#include "..\MobileCnnt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAX_LINE_NUM          100
#define MAX_LINE_CHAR_NUM     78

const _TCHAR CVCalendar::VCALENDAR_BEGIN[]		=_T("BEGIN:VCALENDAR");
const _TCHAR CVCalendar::VCALENDAR_END[]		=_T("END:VCALENDAR");

const _TCHAR CVCalendar::EVENT_BEGIN[]			=_T("BEGIN:VEVENT");
const _TCHAR CVCalendar::EVENT_END[]			=_T("END:VEVENT");
	
const _TCHAR CVCalendar::VCALENDAR_VERSION[]	=_T("VERSION:1.0");
const _TCHAR CVCalendar::START_DATE[]			=_T("DTSTART:");
const _TCHAR CVCalendar::END_DATE[]				=_T("DTEND:");
const _TCHAR CVCalendar::END_DATE2[]			=_T("DUE:");
const _TCHAR CVCalendar::START_DATE_OPT[]		=_T("DTSTART;");
const _TCHAR CVCalendar::END_DATE_OPT[]			=_T("DTEND;");
const _TCHAR CVCalendar::END_DATE2_OPT[]		=_T("DUE;");

const _TCHAR CVCalendar::DESCRIPTION[]			=_T("DESCRIPTION;ENCODING=QUOTED-PRINTABLE:");
const _TCHAR CVCalendar::SUMMARY[]			    =_T("SUMMARY;ENCODING=QUOTED-PRINTABLE:");

const _TCHAR CVCalendar::DESCRIPTION_OPT[]		=_T("DESCRIPTION;ENCODING=QUOTED-PRINTABLE;");
const _TCHAR CVCalendar::SUMMARY_OPT[]			=_T("SUMMARY;ENCODING=QUOTED-PRINTABLE;");
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVCalendar::CVCalendar()
{
	m_pFile = NULL;
	memset(m_szTempA,0,sizeof(m_szTempA));
	memset(m_szTempW,0,sizeof(m_szTempW));
	memset(m_szTempUA,0,sizeof(m_szTempUA));
	memset(&m_memoStartDate,0,sizeof(m_memoStartDate));
	memset(&m_memoEndDate,0,sizeof(m_memoEndDate));
}

CVCalendar::~CVCalendar()
{
	if(m_pFile != NULL)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}
BOOL CVCalendar::Save(LPCTSTR pszFileName)
{
	if(pszFileName==NULL)
	{
		AfxMessageBox(IDS_ERR_FILE_NAME_EMPTY);
		return FALSE;
	}
	CString str(pszFileName);
	if(str.Right(4).Compare(_T(".vcs"))!=0)
	{
		AfxMessageBox(IDS_ERR_EXT_NAME_VCS);
		return FALSE;
	}
	m_pFile = _tfopen(pszFileName,_T("w"));
	if(m_pFile == NULL)
	{
		AfxMessageBox(IDS_ERR_OPEN_FILE_FAIL);
		return FALSE;
	}
	str.Empty();


		//////////////////////////////////////////////////////////////////////////
		// VCALENDAR_BEGIN
		str.Format(_T("%s"),VCALENDAR_BEGIN);
		memset(m_szTempA,0,sizeof(m_szTempA));
#ifdef _UNICODE
		WideCharToMultiByte(CP_ACP,0,(LPCTSTR)str,-1,m_szTempA,sizeof(m_szTempA),NULL,NULL);
#else
		_tcscpy(m_szTempA, str);
#endif
		fprintf(m_pFile,"%s\r\n",m_szTempA);

		//////////////////////////////////////////////////////////////////////////
		// VCALENDAR_VERSION
		str.Format(_T("%s"),VCALENDAR_VERSION);
		memset(m_szTempA,0,sizeof(m_szTempA));
#ifdef _UNICODE
		WideCharToMultiByte(CP_ACP,0,(LPCTSTR)str,-1,m_szTempA,sizeof(m_szTempA),NULL,NULL);
#else
		_tcscpy(m_szTempA, str);
#endif
		fprintf(m_pFile,"%s\r\n",m_szTempA);

		//////////////////////////////////////////////////////////////////////////
		// EVENT_BEGIN
		str.Format(_T("%s"),EVENT_BEGIN);
		memset(m_szTempA,0,sizeof(m_szTempA));
#ifdef _UNICODE
		WideCharToMultiByte(CP_ACP,0,(LPCTSTR)str,-1,m_szTempA,sizeof(m_szTempA),NULL,NULL);
#else
		_tcscpy(m_szTempA, str);
#endif
		
		fprintf(m_pFile,"%s\r\n",m_szTempA);
        
		//////////////////////////////////////////////////////////////////////////
		// START_DATE
		DateToString(m_strStartDT,m_memoStartDate);
		str.Format(_T("%s%s"),START_DATE,m_strStartDT);
		memset(m_szTempA,0,sizeof(m_szTempA));
#ifdef _UNICODE
		WideCharToMultiByte(CP_ACP,0,(LPCTSTR)str,-1,m_szTempA,sizeof(m_szTempA),NULL,NULL);
#else
		_tcscpy(m_szTempA, str);
#endif
		fprintf(m_pFile,"%s\r\n",m_szTempA);

		//////////////////////////////////////////////////////////////////////////
		// END_DATE
		DateToString(m_strEndDT,m_memoEndDate);
		str.Format(_T("%s%s"),END_DATE,m_strEndDT);
		memset(m_szTempA,0,sizeof(m_szTempA));
#ifdef _UNICODE
		WideCharToMultiByte(CP_ACP,0,(LPCTSTR)str,-1,m_szTempA,sizeof(m_szTempA),NULL,NULL);
#else
		_tcscpy(m_szTempA, str);
#endif
		fprintf(m_pFile,"%s\r\n",m_szTempA);		


		//////////////////////////////////////////////////////////////////////////
		// DESCRIPTION
		str.Format(_T("%s%s"),DESCRIPTION,m_strDescript);
		memset(m_szTempA,0,sizeof(m_szTempA));
		memset(m_szTempUA,0,sizeof(m_szTempUA));
#ifdef _UNICODE
		WideCharToMultiByte(CP_ACP,0,(LPCTSTR)str,-1,m_szTempA,sizeof(m_szTempA),NULL,NULL);
#else
		_tcscpy(m_szTempA, str);
#endif
		
		memcpy(m_szTempUA,m_szTempA,sizeof(m_szTempA));

		unsigned char* pSrc = m_szTempUA;
		char sz[100] = {0};
#ifdef _UNICODE
		WideCharToMultiByte(CP_ACP,0,DESCRIPTION,-1,sz,sizeof(sz),NULL,NULL);
#else
		_tcscpy(m_szTempA, DESCRIPTION);
#endif
		
		UINT i=0;
		for(i =0 ;i<strlen(sz);i++)
			pSrc[i] = 'a';		
		
		memset(m_szTempA,0,sizeof(m_szTempA));		
		char *pDst = m_szTempA;
		
		EncodeQP(pSrc, pDst, strlen((char *)pSrc), MAX_LINE_CHAR_NUM);		

		for(i =0 ;i<strlen(sz);i++)
			pDst[i] = sz[i];
		strcat(m_szTempA,"\r\n");		
		fprintf(m_pFile,"%s",m_szTempA);

		//////////////////////////////////////////////////////////////////////////
		// EVENT_END
		str.Format(_T("%s"),EVENT_END);
		memset(m_szTempA,0,sizeof(m_szTempA));
#ifdef _UNICODE
		WideCharToMultiByte(CP_ACP,0,(LPCTSTR)str,-1,m_szTempA,sizeof(m_szTempA),NULL,NULL);
#else
		_tcscpy(m_szTempA, str);
#endif
		fprintf(m_pFile,"%s\r\n",m_szTempA);

		//////////////////////////////////////////////////////////////////////////
		// VCALENDAR_END
		str.Format(_T("%s"),VCALENDAR_END);
		memset(m_szTempA,0,sizeof(m_szTempA));
#ifdef _UNICODE
		WideCharToMultiByte(CP_ACP,0,(LPCTSTR)str,-1,m_szTempA,sizeof(m_szTempA),NULL,NULL);
#else
		_tcscpy(m_szTempA, str);
#endif
		fprintf(m_pFile,"%s\r\n",m_szTempA);

		fclose(m_pFile);
		m_pFile= NULL;

		return TRUE;


}


BOOL CVCalendar::Parse(LPCTSTR pszFileName)
{
//--------------------------------------------------
#define READ_LINE(){\
		memset(m_szTempW,0,sizeof(m_szTempW));\
		if(_ftscanf(m_pFile,_T("%s"),m_szTempW)==EOF)	\
		{\
			fclose(m_pFile);\
			m_pFile = NULL;\
			return FALSE;\
		}\
}
//--------------------------------------------------
	if(pszFileName==NULL)
	{
		AfxMessageBox(IDS_ERR_FILE_NAME_EMPTY);
		return FALSE;
	}
	CString str(pszFileName);
	if(str.Right(4).Compare(_T(".vcs"))!=0)
	{
		AfxMessageBox(IDS_ERR_EXT_NAME_VCS);
		return FALSE;
	}
	m_pFile = _tfopen(pszFileName,_T("r"));
	if(m_pFile == NULL)
	{
		AfxMessageBox(IDS_ERR_OPEN_FILE_FAIL);
		return FALSE;
	}
	str.Empty();

	memset(m_szTempW,0,sizeof(m_szTempW));
	READ_LINE();
    
	if(_tcscmp(m_szTempW,VCALENDAR_BEGIN)!=0)
	{
		fclose(m_pFile);
		m_pFile = NULL;
		AfxMessageBox(IDS_ERR_OPEN_FILE_FAIL);
		return FALSE;
	}

	READ_LINE();

	int nFind = -1;
	BOOL bSummary = FALSE;
	BOOL bDesp = FALSE;
	m_strDescript.Empty();
	CString strSummary = _T("");
	while(_tcscmp(m_szTempW,VCALENDAR_END) != 0)
	{	
		str=m_szTempW;
		str.TrimLeft();
		str.TrimRight();
		if( str.Find(START_DATE)==0 || str.Find(START_DATE_OPT)==0)
		{		
			nFind = str.ReverseFind(_T(':'));
			if(nFind != -1)
			{
				str = str.Right(str.GetLength()- nFind - 1);
				str.TrimLeft();
				m_strStartDT= str;
				StringToDate(m_memoStartDate,m_strStartDT);	
			}			
		}	
		else if(str.Find(END_DATE)==0 || str.Find(END_DATE2)==0 ||
			    str.Find(END_DATE_OPT) == 0 ||  str.Find(END_DATE2_OPT)==0 )
		{
			nFind = str.ReverseFind(_T(':'));
			if(nFind != -1)
			{
				str = str.Right(str.GetLength()- nFind - 1);
				str.TrimLeft();
				m_strEndDT= str;
				StringToDate(m_memoEndDate,m_strEndDT);	
			}
		}
		else if(str.Find(DESCRIPTION)==0 ||str.Find(DESCRIPTION_OPT) == 0 )
		{
			m_strDescript += str;
			bDesp = TRUE;
		}
		else if(str.Find(SUMMARY)==0 ||str.Find(SUMMARY_OPT) == 0 )
		{
			strSummary += str;
			bSummary = TRUE;
		}
		else if(bDesp)
		{
			if(str.Find(_T(':')) == -1)
			{
				m_strDescript += _T("\r\n");
				m_strDescript += str;
			}
			else
			{
				bDesp = FALSE;
			}
		}
		else if(bSummary)
		{
			if(str.Find(_T(':')) == -1)
			{
				strSummary += _T("\r\n");
				strSummary += str;
			}
			else
			{
				bSummary = FALSE;
			}
		}		
		
		READ_LINE();
	}

	if(m_strDescript.IsEmpty() && !strSummary.IsEmpty())
	{
		m_strDescript = strSummary;
	}

	//PARSE DESCRIPTION		
	nFind = m_strDescript.ReverseFind(_T(':'));
	if(nFind != -1)
	{
		CString strTmp = m_strDescript;
		strTmp.MakeLower();
		BOOL bUtf8 = FALSE;
		if(strTmp.Find(_T("charset=utf-8")) != -1)
		{
			bUtf8 = TRUE;
		}
		m_strDescript = m_strDescript.Right(m_strDescript.GetLength()-nFind-1);
		m_strDescript.TrimRight();
		
		memset(m_szTempA,0,sizeof(m_szTempA));
		memset(m_szTempUA,0,sizeof(m_szTempUA));
		memset(m_szTempW,0,sizeof(m_szTempW));
		
#ifdef _UNICODE
		WideCharToMultiByte(CP_ACP,0,m_strDescript,-1,m_szTempA,sizeof(m_szTempA),NULL,NULL);
#else
		_tcscpy(m_szTempA, m_strDescript);
#endif	
		
		
		DecodeQP(m_szTempA, m_szTempUA, m_strDescript.GetLength());  

		unsigned char szTempUA[VCS_MAX_BUF_LEN] = {0};

		if(bUtf8)
		{
			memcpy(szTempUA,m_szTempUA,VCS_MAX_BUF_LEN);
			memset(m_szTempUA,0,VCS_MAX_BUF_LEN);
			MultiByteToWideChar(CP_UTF8,0,(LPSTR)szTempUA,-1,(LPWSTR)m_szTempUA,sizeof(m_szTempUA)/2);

			WideCharToMultiByte(CP_ACP,0,(LPWSTR)m_szTempUA,-1,(LPSTR)szTempUA,sizeof(szTempUA),NULL,NULL);
			memcpy(m_szTempUA,szTempUA,VCS_MAX_BUF_LEN);
		}
		
#ifdef _UNICODE
		MultiByteToWideChar(CP_ACP,0,(LPSTR)m_szTempUA,-1,(LPTSTR)m_szTempW,sizeof(m_szTempW)/sizeof(_TCHAR));
#else
		_tcscpy(m_szTempW, m_szTempUA);
#endif
		m_strDescript = m_szTempW;
		
	}

	fclose(m_pFile);
	m_pFile = NULL;

	return TRUE;
}

/*
void CVCalendar::SetValues(CStringArray &arrValues)
{
	
}
*/

int CVCalendar::EncodeQP(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen)

{
    int nDstLen;        // 输出的字符计数
    int nLineLen;       // 输出的行长度计数
 
    nDstLen = 0;
    nLineLen = 0;
 
    for (int i = 0; i < nSrcLen; i++, pSrc++)
    {
        // ASCII 33-60, 62-126原样输出，其余的需编码
        if ((*pSrc >= '!') && (*pSrc <= '~') && (*pSrc != '='))
        {
            *pDst++ = (char)*pSrc;
            nDstLen++;
            nLineLen++;
        }
        else
        {
            sprintf(pDst, "=%02X", *pSrc);
            pDst += 3;
            nDstLen += 3;
            nLineLen += 3;
        }
 
        // 输出换行？
        if (nLineLen >= nMaxLineLen - 3)
        {
            sprintf(pDst, "=\r\n");
            pDst += 3;
            nDstLen += 3;
            nLineLen = 0;
        }
    }
 
    // 输出加个结束符
    *pDst = '\0';
 
    return nDstLen;

}


int CVCalendar::DecodeQP(const char* pSrc, unsigned char* pDst, int nSrcLen)

{
    int nDstLen;        // 输出的字符计数
    int i;
 
    i = 0;
    nDstLen = 0;
 
    while (i < nSrcLen)
    {
        if (strncmp(pSrc, "=\r\n", 3) == 0)        // 软回车，跳过
        {
            pSrc += 3;
            i += 3;
        }
        else
        {
            if (*pSrc == '=')        // 是编码字节
            {
                sscanf(pSrc, "=%02X", pDst);
                pDst++;
                pSrc += 3;
                i += 3;
            }
            else        // 非编码字节
            {
                *pDst++ = (unsigned char)*pSrc++;
                i++;
            }
  
            nDstLen++;
        }
    }
 
    // 输出加个结束符
    *pDst = '\0';
 
    return nDstLen;

}

void CVCalendar::DateToString(CString &str,MEMO_DATE &memoDate)
{
	str.Format(_T("%04d%02d%02dT%02d%02d00Z"),
		memoDate.wYear,memoDate.btMonth,memoDate.btDay,(memoDate.btHour +16)%24,memoDate.btMinut);
}

void CVCalendar::StringToDate(MEMO_DATE &memoDate,CString &str)
{
	int nYear =0;
	int nMonth = 0;
	int nDay = 0;
	int nHour = 0;
	int nMinut = 0;
	int nSec  =0;
	
	CString strTmp = str;
	strTmp.TrimRight(_T('Z'));
	_TCHAR *pCh = strTmp.GetBuffer(0);
	_stscanf(pCh,_T("%4d%2d%2dT%2d%2d%2d")
		,&nYear,&nMonth,&nDay,&nHour,&nMinut,&nSec);
	
	memoDate.wYear = (WORD)nYear;
	memoDate.btMonth = (BYTE)nMonth;
	memoDate.btDay = (BYTE)nDay;
	memoDate.btHour = (BYTE)nHour;
	memoDate.btMinut = (BYTE)nMinut;
	memoDate.btHour = (BYTE)((memoDate.btHour + 8)%24);
	strTmp.ReleaseBuffer();
}