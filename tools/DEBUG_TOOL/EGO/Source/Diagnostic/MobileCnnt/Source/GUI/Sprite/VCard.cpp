// VCard.cpp: implementation of the CVCard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VCard.h"


#include <afx.h>
#include <Afxcoll.h>
#include <atlconv.h>


#include "..\MobileCnnt.h"
#include "GlobalDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAX_LINE_NUM     100

const _TCHAR CVCard::VCARD_BEGIN[]			=_T("BEGIN:VCARD");
const _TCHAR CVCard::VCARD_END[]			=_T("END:VCARD");
	
const _TCHAR CVCard::VCARD_VERSION[]		=_T("VERSION:2.1");
const _TCHAR CVCard::VCARD_REV[]			=_T("REV:");

const _TCHAR CVCard::NAME1[]					=_T("N:;");
const _TCHAR CVCard::NAME2[]					=_T("N:");
const _TCHAR CVCard::NAME3[]					=_T("N;");
const _TCHAR CVCard::FULL_NAME[]			=_T("FN:");
const _TCHAR CVCard::FULL_NAME2[]			=_T("FN;");

const _TCHAR CVCard::TEL_WORK_VOICE[]		=_T("TEL;WORK;VOICE:");
const _TCHAR CVCard::TEL_HOME_VOICE[]		=_T("TEL;HOME;VOICE:");
const _TCHAR CVCard::TEL_CELL_VOICE[]		=_T("TEL;CELL;VOICE:");
const _TCHAR CVCard::TEL_WORK_FAX[]         =_T("TEL;WORK;FAX:");

const _TCHAR CVCard::TEL_WORK_VOICE2[]		=_T("TEL;WORKE:");
const _TCHAR CVCard::TEL_HOME_VOICE2[]		=_T("TEL;HOME:");
const _TCHAR CVCard::TEL_CELL_VOICE2[]		=_T("TEL;CELL:");

const _TCHAR CVCard::EMAIL_PREF_INTERNET[]	=_T("EMAIL;PREF;INTERNET:");
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVCard::CVCard()
{
	m_pFile = NULL;
	memset(m_szTempA,0,sizeof(m_szTempA));
	memset(m_szTempW,0,sizeof(m_szTempW));
}

CVCard::~CVCard()
{
    if(m_pFile != NULL)
	{
		fclose(m_pFile);
	}
}

BOOL CVCard::Save(LPCTSTR pszFileName)
{
	if(pszFileName==NULL)
	{
		AfxMessageBox(IDS_ERR_FILE_NAME_EMPTY);
		return FALSE;
	}
	CString str(pszFileName);
	if(str.Right(4).Compare(_T(".vcf"))!=0)
	{
		AfxMessageBox(IDS_ERR_EXT_NAME_VCF);
		return FALSE;
	}
	m_pFile = _tfopen(pszFileName,_T("w"));
	if(m_pFile == NULL)
	{
		AfxMessageBox(IDS_ERR_OPEN_FILE_FAIL);
		return FALSE;
	}


	memset(m_szTempA,0,sizeof(m_szTempA));
#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP,0,VCARD_BEGIN,-1,m_szTempA,sizeof(m_szTempA),NULL,NULL);
#else
	_tcscpy(m_szTempA,VCARD_BEGIN);
#endif
	
	fprintf(m_pFile,"%s\r\n",m_szTempA);

	memset(m_szTempA,0,sizeof(m_szTempA));
#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP,0,VCARD_VERSION,-1,m_szTempA,sizeof(m_szTempA),NULL,NULL);
#else
	_tcscpy(m_szTempA,VCARD_VERSION);
#endif
	fprintf(m_pFile,"%s\r\n",m_szTempA);

	str.Format(_T("%s%s"),NAME1,m_strFullName);
	memset(m_szTempA,0,sizeof(m_szTempA));
#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP,0,(LPCTSTR)str,-1,m_szTempA,sizeof(m_szTempA),NULL,NULL);
#else
	_tcscpy(m_szTempA,str);
#endif
	fprintf(m_pFile,"%s\r\n",m_szTempA);

	str.Format(_T("%s%s"),FULL_NAME,m_strFullName);
	memset(m_szTempA,0,sizeof(m_szTempA));
#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP,0,(LPCTSTR)str,-1,m_szTempA,sizeof(m_szTempA),NULL,NULL);
#else
	_tcscpy(m_szTempA,str);
#endif

	fprintf(m_pFile,"%s\r\n",m_szTempA);

	str.Format(_T("%s%s"),TEL_WORK_VOICE,m_strTelWorkVoice);
	memset(m_szTempA,0,sizeof(m_szTempA));
#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP,0,(LPCTSTR)str,-1,m_szTempA,sizeof(m_szTempA),NULL,NULL);
#else
	_tcscpy(m_szTempA,str);
#endif
	fprintf(m_pFile,"%s\r\n",m_szTempA);

	str.Format(_T("%s%s"),TEL_HOME_VOICE,m_strTelHomeVoice);
	memset(m_szTempA,0,sizeof(m_szTempA));
#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP,0,(LPCTSTR)str,-1,m_szTempA,sizeof(m_szTempA),NULL,NULL);
#else
	_tcscpy(m_szTempA,str);
#endif
	fprintf(m_pFile,"%s\r\n",m_szTempA);

	str.Format(_T("%s%s"),TEL_CELL_VOICE,m_strTelCellVoice);
	memset(m_szTempA,0,sizeof(m_szTempA));
#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP,0,(LPCTSTR)str,-1,m_szTempA,sizeof(m_szTempA),NULL,NULL);
#else
	_tcscpy(m_szTempA,str);
#endif
	fprintf(m_pFile,"%s\r\n",m_szTempA);

	str.Format(_T("%s%s"),TEL_WORK_FAX,m_strTelWorkFax);
	memset(m_szTempA,0,sizeof(m_szTempA));
#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP,0,(LPCTSTR)str,-1,m_szTempA,sizeof(m_szTempA),NULL,NULL);
#else
	_tcscpy(m_szTempA,str);
#endif
	fprintf(m_pFile,"%s\r\n",m_szTempA);


	str.Format(_T("%s%s"),EMAIL_PREF_INTERNET,m_strEmailPrefInternet);
	memset(m_szTempA,0,sizeof(m_szTempA));
#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP,0,(LPCTSTR)str,-1,m_szTempA,sizeof(m_szTempA),NULL,NULL);
#else
	_tcscpy(m_szTempA,str);
#endif

	fprintf(m_pFile,"%s\r\n",m_szTempA);

	
	CTime _time = CTime::GetCurrentTime();
	m_strRev.Format(_T("%4d-%.2d-%.2dT%.2d:%.2d:%.2dZ"),
		_time.GetYear(),_time.GetMonth(),_time.GetDay(),
		_time.GetHour(),_time.GetMinute(),_time.GetSecond());		

	str.Format(_T("%s%s"),VCARD_REV,m_strRev);
	memset(m_szTempA,0,sizeof(m_szTempA));
#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP,0,(LPCTSTR)str,-1,m_szTempA,sizeof(m_szTempA),NULL,NULL);
#else
	_tcscpy(m_szTempA,str);
#endif

	fprintf(m_pFile,"%s\r\n",m_szTempA);

	memset(m_szTempA,0,sizeof(m_szTempA));
#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP,0,VCARD_END,-1,m_szTempA,sizeof(m_szTempA),NULL,NULL);
#else
	_tcscpy(m_szTempA,VCARD_END);
#endif

	fprintf(m_pFile,"%s\r\n",m_szTempA);

	fclose(m_pFile);
	m_pFile = NULL;

	return TRUE;



}

BOOL CVCard::Parse(LPCTSTR pszFileName)
{
	USES_CONVERSION;
	if(pszFileName==NULL)
	{
		AfxMessageBox(IDS_ERR_FILE_NAME_EMPTY);
		return FALSE;
	}
	CString str(pszFileName);
	if(str.Right(4).Compare(_T(".vcf"))!=0)
	{
		AfxMessageBox(IDS_ERR_EXT_NAME_VCF);
		return FALSE;
	}
	m_pFile =_tfopen(pszFileName, _T("r"));
	
	if(m_pFile == NULL)
	{
		AfxMessageBox(IDS_ERR_OPEN_FILE_FAIL);
		return FALSE;
	}
	str.Empty();

	memset(m_szTempA,0,sizeof(m_szTempA));
	memset(m_szTempW,0,sizeof(m_szTempW));
	fscanf(m_pFile,"%s",m_szTempA);		
#ifdef _UNICODE
//	WideCharToMultiByte(CP_ACP,0,(LPCTSTR)m_szTempA,-1,(LPSTR)m_szTempW,sizeof(m_szTempW)/sizeof(_TCHAR), NULL, NULL);
	MultiByteToWideChar(CP_ACP,0,m_szTempA,-1,m_szTempW,sizeof(m_szTempW)/sizeof(_TCHAR));
#else
	_tcscpy(m_szTempW,m_szTempA);
#endif

	if(_tcscmp(m_szTempW,VCARD_BEGIN)!=0)
	{
		AfxMessageBox(IDS_ERR_OPEN_FILE_FAIL);
		return FALSE;
	}

	memset(m_szTempA,0,sizeof(m_szTempA));
	memset(m_szTempW,0,sizeof(m_szTempW));
	fscanf(m_pFile,"%s",m_szTempA);	
#ifdef _UNICODE
//	WideCharToMultiByte(CP_ACP,0,(LPCTSTR)m_szTempA,-1,(LPSTR)m_szTempW,sizeof(m_szTempW)/sizeof(_TCHAR), NULL, NULL);
	MultiByteToWideChar(CP_ACP,0,m_szTempA,-1,m_szTempW,sizeof(m_szTempW)/sizeof(_TCHAR));
#else
	_tcscpy(m_szTempW,m_szTempA);
#endif

	if(_tcscmp(m_szTempW,VCARD_VERSION)!=0)
	{
		AfxMessageBox(IDS_ERR_OPEN_FILE_FAIL);
		return FALSE;
	}
	
	int nCount =0;
	CStringArray arrLine;
	while(_tcscmp(m_szTempW,VCARD_END)!=0 && nCount < (MAX_LINE_NUM - 2) )
	{
		memset(m_szTempA,0,sizeof(m_szTempA));
		memset(m_szTempW,0,sizeof(m_szTempW));
		fscanf(m_pFile,"%s",m_szTempA);	
#ifdef _UNICODE
//	WideCharToMultiByte(CP_ACP,0,(LPCTSTR)m_szTempA,-1,(LPSTR)m_szTempW,sizeof(m_szTempW)/sizeof(_TCHAR), NULL, NULL);
	MultiByteToWideChar(CP_ACP,0,m_szTempA,-1,m_szTempW,sizeof(m_szTempW)/sizeof(_TCHAR));
#else
	_tcscpy(m_szTempW,m_szTempA);
#endif

		if(_tcslen(m_szTempW) != 0)
		{
			arrLine.Add(m_szTempW);
			nCount++;
		}
	}

	if(nCount >= (MAX_LINE_NUM - 2))
	{
		AfxMessageBox(IDS_ERR_OPEN_FILE_FAIL);
		return FALSE;
	}
	CUIntArray arrNum;
	for(int k=0;k<nCount;k++)
		arrNum.Add(k);

	CStringArray arrTagValue;
	arrTagValue.Add(NAME2);
	arrTagValue.Add(FULL_NAME);
	arrTagValue.Add(TEL_WORK_VOICE);
	arrTagValue.Add(TEL_HOME_VOICE);
	arrTagValue.Add(TEL_CELL_VOICE);
	arrTagValue.Add(TEL_WORK_FAX);
	arrTagValue.Add(EMAIL_PREF_INTERNET);
    
	int nTagValue = arrTagValue.GetSize();
    int nFind = 0;
	for(int i=0; i<nTagValue; i++)
	{
		nCount=arrNum.GetSize();
		for(int j=0;j< nCount;j++)
		{
			str = arrLine.GetAt(arrNum.GetAt(j));
			nFind = str.Find(arrTagValue.GetAt(i));
			if(nFind >= 0)
			{
				str = str.Right(str.GetLength()-nFind-arrTagValue.GetAt(i).GetLength());
				str.TrimRight(';');
				if(i==0)
				{
					nFind = str.Find(';');
					if(nFind != -1)
					{
						str = str.Right(str.GetLength()-nFind-1);
					}
				}
				CString strTemp;
				LPTSTR lpsz = strTemp.GetBufferSetLength(str.GetLength() +1 );
				FoldString(MAP_PRECOMPOSED,str,-1,lpsz,str.GetLength()+1);
				arrTagValue.SetAt(i,str);
				arrNum.RemoveAt(j);
				break;
			}
			else
			{	
#define X_CP_UTF16  0xFFFFFFFF
				if(i==0) //N
				{
					str = arrLine.GetAt(arrNum.GetAt(j));
					nFind = str.Find(NAME3);
					if(nFind >= 0)
					{
						BOOL bDQ = FALSE;
						UINT nCodePage = CP_ACP;
						if(str.Find(_T("ENCODING=QUOTED-PRINTABLE")) != -1)
						{
							bDQ = TRUE;
						}
						if(str.Find(_T("CHARSET=UTF-8")) != -1)
						{
							nCodePage = CP_UTF8;
						}
						else if(str.Find(_T("CHARSET=UTF-7")) != -1)
						{
							nCodePage = CP_UTF7;
						}
						else if(str.Find(_T("CHARSET=UTF-16")) != -1)
						{
							nCodePage = X_CP_UTF16;
						}			

						str = str.Right(str.GetLength()-nFind-_tcslen(NAME3));
						str.TrimRight(';');
						
						nFind = str.Find(':');
						if(nFind != -1)
						{
							str = str.Right(str.GetLength()-nFind-1);
						}

						str.Remove(';');

						if(bDQ)
						{
							int nSZBufSize = str.GetLength()*4;
							char *pszBuf = new char[nSZBufSize];
							memset(pszBuf,0,nSZBufSize);
							int nSZBufSize2 = str.GetLength()*8;
							char *pszBuf2 = new char[nSZBufSize2];
							memset(pszBuf2,0,nSZBufSize2);

							int nWBufSize = str.GetLength()*8;
							TCHAR *pWBuf = new TCHAR[nWBufSize];
							memset(pWBuf,0,nWBufSize*sizeof(TCHAR));

						#ifdef _UNICODE
							WideCharToMultiByte(CP_ACP,0,str.operator LPCTSTR(),-1,pszBuf,nSZBufSize,NULL,NULL);
						#else
							_tcscpy(pszBuf, str.operator LPCTSTR());
						#endif	

							DecodeQP(pszBuf, (BYTE*)pszBuf2, strlen(pszBuf));  

						#ifdef _UNICODE
							if(nCodePage != X_CP_UTF16)
							{
								MultiByteToWideChar(nCodePage,0,(LPSTR)pszBuf2,-1,(LPTSTR)pWBuf,nWBufSize);
							}
							else
							{
								memcpy(pWBuf,pszBuf2,nSZBufSize2);
							}
						#else
							_tcscpy(pWBuf, pszBuf2);
						#endif

							//str = CW2A(str,CP_UTF8);

							str = pWBuf;

							delete [] pszBuf;
							delete [] pszBuf2;
							delete [] pWBuf;

						}

						CString strTemp;
						LPTSTR lpsz = strTemp.GetBufferSetLength(str.GetLength() +1 );
						FoldString(MAP_PRECOMPOSED,str,-1,lpsz,str.GetLength()+1);
						arrTagValue.SetAt(i,str);
						arrNum.RemoveAt(j);
						break;
					}
				}
				else if( i==1) //FN
				{
					str = arrLine.GetAt(arrNum.GetAt(j));
					nFind = str.Find(FULL_NAME2);
					if(nFind >= 0)
					{
						BOOL bDQ = FALSE;
						UINT nCodePage = CP_ACP;
						if(str.Find(_T("ENCODING=QUOTED-PRINTABLE")) != -1)
						{
							bDQ = TRUE;
						}
						if(str.Find(_T("CHARSET=UTF-8")) != -1)
						{
							nCodePage = CP_UTF8;
						}
						if(str.Find(_T("CHARSET=UTF-7")) != -1)
						{
							nCodePage = CP_UTF7;
						}
						if(str.Find(_T("CHARSET=UTF-16")) != -1)
						{
							nCodePage = X_CP_UTF16;
						}

						str = str.Right(str.GetLength()-nFind-_tcslen(FULL_NAME2));
						str.TrimRight(';');
						
						nFind = str.Find(':');
						if(nFind != -1)
						{
							str = str.Right(str.GetLength()-nFind-1);
						}

						str.Remove(';');

						if(bDQ)
						{
							int nSZBufSize = str.GetLength()*4;
							char *pszBuf = new char[nSZBufSize];
							memset(pszBuf,0,nSZBufSize);
							int nSZBufSize2 = str.GetLength()*8;
							char *pszBuf2 = new char[nSZBufSize2];
							memset(pszBuf2,0,nSZBufSize2);

							int nWBufSize = str.GetLength()*8;
							TCHAR *pWBuf = new TCHAR[nWBufSize];
							memset(pWBuf,0,nWBufSize*sizeof(TCHAR));

						#ifdef _UNICODE
							WideCharToMultiByte(CP_ACP,0,str.operator LPCTSTR(),-1,pszBuf,nSZBufSize,NULL,NULL);
						#else
							_tcscpy(pszBuf, str.operator LPCTSTR());
						#endif	

							DecodeQP(pszBuf, (BYTE*)pszBuf2, strlen(pszBuf));  

						#ifdef _UNICODE							
							if(nCodePage != X_CP_UTF16)
							{
								MultiByteToWideChar(nCodePage,0,(LPSTR)pszBuf2,-1,(LPTSTR)pWBuf,nWBufSize);
							}
							else
							{
								memcpy(pWBuf,pszBuf2,nSZBufSize2);
							}						
						#else
							_tcscpy(pWBuf, pszBuf2);
						#endif

							//str = CW2A(str,CP_UTF8);

							str = pWBuf;

							delete [] pszBuf;
							delete [] pszBuf2;
							delete [] pWBuf;

						}
					
						CString strTemp;
						LPTSTR lpsz = strTemp.GetBufferSetLength(str.GetLength() +1 );
						FoldString(MAP_PRECOMPOSED,str,-1,lpsz,str.GetLength()+1);
						arrTagValue.SetAt(i,str);
						arrNum.RemoveAt(j);
						break;
					}
				}
				else if( i==2) //WORK
				{
					str = arrLine.GetAt(arrNum.GetAt(j));
					nFind = str.Find(TEL_WORK_VOICE2);
					if(nFind >= 0)
					{
						str = str.Right(str.GetLength()-nFind-_tcslen(TEL_WORK_VOICE2));
						str.TrimRight(';');
						
						nFind = str.Find(':');
						if(nFind != -1)
						{
							str = str.Right(str.GetLength()-nFind-1);
						}

						str.Remove(';');
					
						CString strTemp;
						LPTSTR lpsz = strTemp.GetBufferSetLength(str.GetLength() +1 );
						FoldString(MAP_PRECOMPOSED,str,-1,lpsz,str.GetLength()+1);
						arrTagValue.SetAt(i,str);
						arrNum.RemoveAt(j);
						break;
					}
				}
				else if( i==3) //HOME
				{
					str = arrLine.GetAt(arrNum.GetAt(j));
					nFind = str.Find(TEL_HOME_VOICE2);
					if(nFind >= 0)
					{
						str = str.Right(str.GetLength()-nFind-_tcslen(TEL_HOME_VOICE2));
						str.TrimRight(';');
						
						nFind = str.Find(':');
						if(nFind != -1)
						{
							str = str.Right(str.GetLength()-nFind-1);
						}

						str.Remove(';');
					
						CString strTemp;
						LPTSTR lpsz = strTemp.GetBufferSetLength(str.GetLength() +1 );
						FoldString(MAP_PRECOMPOSED,str,-1,lpsz,str.GetLength()+1);
						arrTagValue.SetAt(i,str);
						arrNum.RemoveAt(j);
						break;
					}
				}
				else if( i==4) //CELL
				{
					str = arrLine.GetAt(arrNum.GetAt(j));
					nFind = str.Find(TEL_CELL_VOICE2);
					if(nFind >= 0)
					{
						str = str.Right(str.GetLength()-nFind-_tcslen(TEL_CELL_VOICE2));
						str.TrimRight(';');
						
						nFind = str.Find(':');
						if(nFind != -1)
						{
							str = str.Right(str.GetLength()-nFind-1);
						}

						str.Remove(';');
					
						CString strTemp;
						LPTSTR lpsz = strTemp.GetBufferSetLength(str.GetLength() +1 );
						FoldString(MAP_PRECOMPOSED,str,-1,lpsz,str.GetLength()+1);
						arrTagValue.SetAt(i,str);
						arrNum.RemoveAt(j);
						break;
					}
				}
			}
		}
		if(j>=nCount)
			arrTagValue.SetAt(i,_T(""));
	}

	SetValues(arrTagValue);	

	fclose(m_pFile);
	m_pFile = NULL;

	return TRUE;


}

void CVCard::SetValues(CStringArray &arrValues)
{
	m_strFullName = arrValues.GetAt(1);
	if(m_strFullName.IsEmpty())
	{
		m_strFullName = arrValues.GetAt(0);
	}

	m_strTelWorkVoice = arrValues.GetAt(2);
	m_strTelHomeVoice = arrValues.GetAt(3);
	m_strTelCellVoice = arrValues.GetAt(4);
	m_strTelWorkFax = arrValues.GetAt(5);

	m_strEmailPrefInternet = arrValues.GetAt(6);
}

void CVCard::ConvertToPB(_PB * pPB)
{
	_ASSERT(pPB!=NULL);
	int nSize =0,i=0;

	nSize = m_strFullName.GetLength();
	for(i=0;i<nSize;i++)
	   pPB->szName[i] = m_strFullName[i];
	pPB->szName[nSize]='\0';

	nSize = m_strTelCellVoice.GetLength();
	for(i=0;i<nSize;i++)
	   pPB->szPhone[i] = m_strTelCellVoice[i];
	pPB->szPhone[nSize]='\0';

	nSize = m_strTelHomeVoice.GetLength();
	for(i=0;i<nSize;i++)
	   pPB->szHomePhone[i] = m_strTelHomeVoice[i];
	pPB->szHomePhone[nSize]='\0';

	nSize = m_strTelWorkVoice.GetLength();
	for(i=0;i<nSize;i++)
	   pPB->szOfficePhone[i] = m_strTelWorkVoice[i];
	pPB->szOfficePhone[nSize]='\0';

	nSize = m_strTelWorkFax.GetLength();
	for(i=0;i<nSize;i++)
	   pPB->szOfficePhone[i] = m_strTelWorkFax[i];
	pPB->szOfficePhone[nSize]='\0';

	nSize = m_strEmailPrefInternet.GetLength();
	for(i=0;i<nSize;i++)
	   pPB->szEmail[i] = m_strEmailPrefInternet[i];
	pPB->szEmail[nSize]='\0';

	pPB->nGroupIndex =1;
	pPB->wStore = ID_STORE_PC;

}

int CVCard::DecodeQP(const char* pSrc, unsigned char* pDst, int nSrcLen)

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