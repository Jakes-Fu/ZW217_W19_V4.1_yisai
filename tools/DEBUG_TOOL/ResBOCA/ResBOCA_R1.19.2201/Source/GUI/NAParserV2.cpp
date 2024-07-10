// NAParserV2.cpp: implementation of the CNAParserV2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResOver.h"
#include "NAParserV2.h"
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

CNAParserV2::CNAParserV2()
{
	Clear();
}

CNAParserV2::~CNAParserV2()
{

}

BOOL CNAParserV2::DecodeNTAC(LPBYTE pBuf,DWORD dwSize)
{
	if(dwSize<8 || pBuf == NULL)
	{
		return FALSE;
	}
	Clear();
	//parse head
	memcpy(&m_tHead,pBuf,sizeof(NTAC_FILE_HEAD_T));
	if(m_tHead.magic != NTAC_MAGIC)
	{
		Clear();
		return FALSE;
	}
// 	if(m_tHead.bigendian)
// 	{
// 		m_tHead.version = CONV_SHORT(m_tHead.version);
// 	}
	if(m_tHead.version != NTAC_VER_V2)
	{
		Clear();
		return FALSE;
	}
	if(m_tHead.nCount > NTAC_MAX_NUM)
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

	NTAC_T ntac;
	for(UINT i = 0; i< nCount; i++)
	{
		ntac.Init();
// 		if(m_tHead.bigendian)
// 		{
// 			m_tHead.offset[i] = CONV_DWORD(m_tHead.offset[i]);
// 		}

		pCur = pBuf + pOffset[i];
		if((pCur + 44) > pEnd) //44 is the least len for each group parameter
		{
			m_tHead.nCount = (BYTE)i;
			delete [] pOffset;
			return FALSE;
		}
		memcpy(&ntac.reserved,pCur,18);
		pCur += 18;
// 		if(m_tHead.bigendian)
// 		{
// 			m_tNTAC[i].reserved = CONV_SHORT(m_tNTAC[i].reserved);
// 			m_tNTAC[i].mcc = CONV_SHORT(m_tNTAC[i].mcc);
// 			m_tNTAC[i].mnc = CONV_SHORT(m_tNTAC[i].mnc);
// 			m_tNTAC[i].port = CONV_SHORT(m_tNTAC[i].port);
// 		}

		ntac.active_flag = TRUE;

#define DECODE_STR(buf,size,flag) if(!DecodeString(pCur,pEnd,buf,size,flag) || pCur > pEnd) return FALSE		

		DECODE_STR(ntac.name,NTAC_NAME_LEN,TRUE);
		DECODE_STR(ntac.gateway,NTAC_PROXY_LEN,FALSE);
		DECODE_STR(ntac.dns1,NTAC_IP_LEN,FALSE);
		DECODE_STR(ntac.dns2,NTAC_IP_LEN,FALSE);
		DECODE_STR(ntac.username,NTAC_USERNAME_LEN,FALSE);
		DECODE_STR(ntac.password,NTAC_PASSWORD_LEN,FALSE);
		DECODE_STR(ntac.apn,NTAC_APN_LEN,FALSE);
		DECODE_STR(ntac.homepage,NTAC_HOMEPAGE_LEN,FALSE);
		DECODE_STR(ntac.mvno_value,NTAC_SPN_LEN,FALSE);

		m_vNTAC.push_back(ntac);
	}

	delete [] pOffset;

	return TRUE;
}

BOOL CNAParserV2::EncodeNTAC(LPBYTE &pBuf,DWORD &dwSize)
{
	// force to make it little endian
	//m_tHead.bigendian = FALSE;

	pBuf = NULL;
	dwSize = 0;
	DWORD dwMaxSize = (sizeof(NTAC_T)+sizeof(DWORD))*m_tHead.nCount + 8;
	pBuf = new BYTE[dwMaxSize];
	memset(pBuf,0,dwMaxSize);
	LPBYTE pCur = pBuf;

	*(DWORD*)pCur=NTAC_MAGIC;
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
	for(UINT i= 0; i< m_vNTAC.size();i++)
	{
		if(m_vNTAC[i].active_flag)
		{
			*pOffset = /*m_tHead.bigendian ? CONV_DWORD(dwOffset) : */dwOffset;
			pCur = pBuf+dwOffset;
			*(WORD*)pCur= /*m_tHead.bigendian ? CONV_SHORT(m_tNTAC[i].reserved) : */m_vNTAC[i].reserved;
			pCur+=2;
			*(WORD*)pCur= /*m_tHead.bigendian ? CONV_SHORT(m_tNTAC[i].mcc) :*/ m_vNTAC[i].uiorder;
			pCur+=2;
			*(WORD*)pCur= /*m_tHead.bigendian ? CONV_SHORT(m_tNTAC[i].mcc) :*/ m_vNTAC[i].mcc;
			pCur+=2;
			*(WORD*)pCur= /*m_tHead.bigendian ? CONV_SHORT(m_tNTAC[i].mnc) :*/ m_vNTAC[i].mnc;
			pCur+=2; 
			*(WORD*)pCur= /*m_tHead.bigendian ? CONV_SHORT(m_tNTAC[i].port) : */m_vNTAC[i].port;
			pCur+=2;
			memcpy(pCur,&m_vNTAC[i].access_type,8);
			pCur+=8;

			EncodeString(pCur,m_vNTAC[i].name,TRUE);
			EncodeString(pCur,m_vNTAC[i].gateway,FALSE);
			EncodeString(pCur,m_vNTAC[i].dns1,FALSE);
			EncodeString(pCur,m_vNTAC[i].dns2,FALSE);
			EncodeString(pCur,m_vNTAC[i].username,FALSE);
			EncodeString(pCur,m_vNTAC[i].password,FALSE);
			EncodeString(pCur,m_vNTAC[i].apn,FALSE);
			EncodeString(pCur,m_vNTAC[i].homepage,FALSE);	
			EncodeString(pCur,m_vNTAC[i].mvno_value,FALSE);
			dwOffset = pCur - pBuf;
			pOffset++;
		}
	}

	dwSize = pCur - pBuf;

	return TRUE;
}

BOOL CNAParserV2::DecodeString(LPBYTE &pBuf, LPBYTE pEnd, WCHAR *pString,WORD wStrMaxLen,BOOL bUnicode/*=FALSE*/)
{	
	USES_CONVERSION;
	WORD wLen = *(WORD*)pBuf;
	pBuf+=2;
	//if(m_tHead.bigendian)
	//{
	//	wLen = CONV_SHORT(wLen);
	//}
	if((pBuf+wLen) > pEnd)
	{
		return FALSE;
	}
	WORD wMin = wLen > wStrMaxLen ? wStrMaxLen : wLen;
	if(bUnicode)
	{
		memcpy(pString, pBuf,wMin*2);
		//if(m_tHead.bigendian)
		//{
		//	for(WORD k = 0; k< wMin; k++)
		//	{
		//		pString[k] = CONV_SHORT(pString[k]);
		//	}
		//}
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

BOOL CNAParserV2::EncodeString(LPBYTE &pBuf, WCHAR *pString,BOOL bUnicode/*=FALSE*/)
{
	USES_CONVERSION;
	WORD wLen = (WORD)(_tcslen(pString)+1);
	if(bUnicode)
	{
		wLen *= 2;
	}
	WORD wSize = (WORD)(((wLen + 3 + 2)/4)*4 - 2);  // not include size itself, but calc align with it
	*(WORD*)pBuf =/* m_tHead.bigendian?CONV_SHORT(wSize):*/wSize;
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

VOID CNAParserV2::Clear()
{
	memset(&m_tHead,0,sizeof(m_tHead));
	// m_tHead.bigendian = TRUE;
	m_tHead.version = NTAC_VER_V2;

	m_vNTAC.clear();

// 	NTAC_T ntac;
// 	ntac.Init();
// 	m_vNTAC.push_back(ntac);
}
BOOL CNAParserV2::CreateNtacSheet(LPCTSTR lpszFileName)
{
	CSpreadSheet * pSpread = NULL;
	BOOL bResult = FALSE;
	pSpread = new CSpreadSheet();
	if(!pSpread->Init())
	{
		delete pSpread;
		pSpread = NULL;
		pSpread = new CSpreadSheetET();
		if(!pSpread->Init())
		{
			delete pSpread;
			pSpread = NULL;
			return FALSE;
		}
	}
	CStringArray agRow;
	CStringArray agRowData;
	CString str;
	UINT nNtacNum = 0;
	if(pSpread->OpenExcel(lpszFileName,FALSE))
	{
		agRow.RemoveAll();
		agRow.Add(_T("Setting Name"));	//WCHAR name[NTAC_NAME_LEN+1];//连接名称 最大28
		agRow.Add(_T("Uiorder"));			//WCHAR SPN	最大32
		
		//agRow.Add(_T("SPN"));		//WORD  SPN
		//added
		agRow.Add(_T("MVNO Type"));		//BYTE  mvno_type
		agRow.Add(_T("MVNO Value"));	//WCHAR MVNO VALUE

		
		agRow.Add(_T("MCC"));			//WORD  mcc;//mcc country code, 一般都是3位
		agRow.Add(_T("MNC"));			//WORD  mnc;//mnc network code
		agRow.Add(_T("Account Type"));	//BYTE  account_type;//帐户类型0:浏览器，1:MMS，2:JAVA，3:DCD // uint8
		agRow.Add(_T("APN"));			//WCHAR apn[NTAC_APN_LEN+1];//接入点名称       最大30
		agRow.Add(_T("Access Type"));	//BYTE  access_type;//连接类型:0 表示wap1.2（WSP）, 1表示wap2.0(HTTP)//如果选择Wap1.2，port默认变为9201 , 默认2.0
		agRow.Add(_T("Access Option"));	//BYTE  access_option;//存取选项,0:可编辑;1:只读
		agRow.Add(_T("Proxy Server IP")); //WCHAR gateway[NTAC_PROXY_LEN+1];//代理服务器IP地址 默认0.0.0.0 最大128
		agRow.Add(_T("Port"));			//WORD  port;//代理服务器端口号 默认80
		agRow.Add(_T("User Name"));		//WCHAR username[NTAC_USERNAME_LEN+1];//用户名      最大30
		agRow.Add(_T("Password"));		//WCHAR password[NTAC_PASSWORD_LEN+1];//密码        最大30
		agRow.Add(_T("First DNS"));		//WCHAR dns1[NTAC_IP_LEN+1];//首选dns 默认0.0.0.0 最大15
		agRow.Add(_T("Second DNS"));	//WCHAR dns2[NTAC_IP_LEN+1];//备选dns 默认0.0.0.0 最大15
		agRow.Add(_T("Home Page"));		//WCHAR homepage[NTAC_HOMEPAGE_LEN+1];//主页地址    最大512
		agRow.Add(_T("Auth Type"));		//BYTE  auth_type;//鉴定类型0:PAP，1:CHAP: //uint8
		agRow.Add(_T("Reserved"));
		agRow.Add(_T("Visible"));			//BYTE  Visible
		agRow.Add(_T("Roaming"));			//BYTE  Roaming
		agRow.Add(_T("roaming_protocol"));	//BYTE  roaming_protocol
		
		if(pSpread->CreateSheet(_T("NTAC"),0))
		{			
			pSpread->WriteRow(agRow,0);	
			NTAC_T ntac;
			for(UINT i= 0; i< m_vNTAC.size();i++)
			{
				
				agRowData.RemoveAll();	
				ntac.Init();
				if(m_vNTAC[i].active_flag)
				{
					agRowData.Add(m_vNTAC[i].name);
					//agRowData.Add(m_vNTAC[i].spn);
					str.Format(_T("%d"),m_vNTAC[i].uiorder);
					agRowData.Add(str);

					str.Format(_T("%d"),m_vNTAC[i].mvno_type);
					agRowData.Add(str);

					agRowData.Add(m_vNTAC[i].mvno_value);

					str.Format(_T("%d"),m_vNTAC[i].mcc);
					agRowData.Add(str);
					str.Format(_T("%d"),m_vNTAC[i].mnc);
					agRowData.Add(str);
					str.Format(_T("%d"),m_vNTAC[i].account_type);
					agRowData.Add(str);
					agRowData.Add(m_vNTAC[i].apn);
					str.Format(_T("%d"),m_vNTAC[i].access_type);
					agRowData.Add(str);
					str.Format(_T("%d"),m_vNTAC[i].access_option);
					agRowData.Add(str);
					agRowData.Add(m_vNTAC[i].gateway);
					str.Format(_T("%d"),m_vNTAC[i].port);
					agRowData.Add(str);
					agRowData.Add(m_vNTAC[i].username);
					agRowData.Add(m_vNTAC[i].password);
					agRowData.Add(m_vNTAC[i].dns1);
					agRowData.Add(m_vNTAC[i].dns2);
					agRowData.Add(m_vNTAC[i].homepage);
					str.Format(_T("%d"),m_vNTAC[i].auth_type);
					agRowData.Add(str);
					
					str.Format(_T("%d"),m_vNTAC[i].reserved);
					agRowData.Add(str);

					str.Format(_T("%d"),m_vNTAC[i].Visible);
					agRowData.Add(str);

					str.Format(_T("%d"),m_vNTAC[i].Roaming);
					agRowData.Add(str);

					str.Format(_T("%d"),m_vNTAC[i].roaming_protocol);
					agRowData.Add(str);
					
					pSpread->WriteRow(agRowData,0);	
					++nNtacNum;
				}
			}
			
		}

		if(pSpread->CreateSheet(_T("NTACHdr"),1))
		{		
			agRow.RemoveAll();
			agRow.Add(_T("magic"));	
			agRow.Add(_T("version"));	
			agRow.Add(_T("nCount"));
			pSpread->WriteRow(agRow,1);	

			agRowData.RemoveAll();	
			str.Format(_T("%d"),m_tHead.magic);
			agRowData.Add(str);
			str.Format(_T("%d"),m_tHead.version);
			agRowData.Add(str);
			str.Format(_T("%d"),nNtacNum);
			agRowData.Add(str);
			pSpread->WriteRow(agRowData,1);	
		}

		bResult = TRUE;
	}
	
	pSpread->Close();
	delete pSpread;	
	pSpread = NULL;
	
	return bResult;
}
UINT CNAParserV2::ParseNtacSheet(LPCTSTR lpszFileName)
{
	UINT nRet = E_OK;
	CSpreadSheet * pSpread = NULL;
	int nCols = 0;
	int nRows = 0;
	pSpread = new CSpreadSheet();
	if(!pSpread->Init())
	{
		delete pSpread;
		pSpread = NULL;
		pSpread = new CSpreadSheetET();
		if(!pSpread->Init())
		{
			delete pSpread;
			pSpread = NULL;
			nRet = E_INIT_XLS_ERR;
			return nRet;
		}
	}

	do 
	{
		if(!pSpread->OpenExcel(lpszFileName,TRUE))	{nRet = E_OPEN_XLS_ERR;break;}
		if(!pSpread->OpenSheet(_T("NTAC")))			{nRet = E_OPEN_SHEET_ERR;break;}
		pSpread->GetBounds(nCols,nRows);
		CStringArray aRow;
		aRow.RemoveAll();
		
		if(!pSpread->ReadRow(0,aRow))				{nRet = E_READ_ROW_ERR;break;}
		int i = 0;
		CString tt2 = aRow.GetAt(i);
		if (	//检测列名	
			(_tcsicmp(aRow.GetAt(i++),_T("Setting Name")) != 0)		||
			(_tcsicmp(aRow.GetAt(i++),_T("Uiorder")) != 0)			||
			(_tcsicmp(aRow.GetAt(i++),_T("MVNO Type")) != 0)		||
			(_tcsicmp(aRow.GetAt(i++),_T("MVNO Value")) != 0)		||
			(_tcsicmp(aRow.GetAt(i++),_T("MCC")) != 0)				||		
			(_tcsicmp(aRow.GetAt(i++),_T("MNC")) != 0)				||			
			(_tcsicmp(aRow.GetAt(i++),_T("Account Type")) != 0)		||
			(_tcsicmp(aRow.GetAt(i++),_T("APN")) != 0)				||			
			(_tcsicmp(aRow.GetAt(i++),_T("Access Type")) != 0)		||
			(_tcsicmp(aRow.GetAt(i++),_T("Access Option")) != 0)	||	
			(_tcsicmp(aRow.GetAt(i++),_T("Proxy Server IP")) != 0)	||
			(_tcsicmp(aRow.GetAt(i++),_T("Port")) != 0)				||			
			(_tcsicmp(aRow.GetAt(i++),_T("User Name")) != 0)		||		
			(_tcsicmp(aRow.GetAt(i++),_T("Password")) != 0)			||		
			(_tcsicmp(aRow.GetAt(i++),_T("First DNS")) != 0)		||		
			(_tcsicmp(aRow.GetAt(i++),_T("Second DNS")) != 0)		||	
			(_tcsicmp(aRow.GetAt(i++),_T("Home Page")) != 0)		||		
			(_tcsicmp(aRow.GetAt(i++),_T("Auth Type")) != 0)		||		
			(_tcsicmp(aRow.GetAt(i++),_T("Reserved")) != 0)			||
			(_tcsicmp(aRow.GetAt(i++),_T("Visible")) != 0)			||
			(_tcsicmp(aRow.GetAt(i++),_T("Roaming")) != 0)			||
			(_tcsicmp(aRow.GetAt(i++),_T("roaming_protocol")) != 0)		
			)
		{
			CString tt = aRow.GetAt(i);
			nRet = E_INVALID_XLS_ERR;
			break;
		}
		if (!DecodeXls2Ntac(pSpread))
		{
			nRet = E_DEC_XLS_ERR;
		}

	} while(0);

	pSpread->Close();
	delete pSpread;
	pSpread = NULL;
	return nRet;
}
BOOL CNAParserV2::DecodeXls2Ntac(CSpreadSheet * pSpread)
{
	int nCols = 0;
	int nRows = 0;
	CStringArray aRow;
	//CString str;
	Clear();
	pSpread->GetBounds(nCols,nRows);
	m_tHead.magic = NTAC_MAGIC;
	m_tHead.version = NTAC_VER_V2;
	m_tHead.nCount = (unsigned short)(nRows-1);

	if(m_tHead.nCount > NTAC_MAX_NUM)
	{
		Clear();
		return FALSE;
	}
	
	if(m_tHead.nCount == 0)
	{
		return TRUE;
	}

	NTAC_T ntac;
	BOOL bResult = TRUE;
	for(int i = 1; i< nRows; i++)
	{
		aRow.RemoveAll();
		ntac.Init();
		if(pSpread->ReadRow(i,aRow))
		{
			ntac.active_flag = TRUE;
			_tcscpy(ntac.name,aRow.GetAt(0));
			ntac.uiorder =(unsigned short)_ttoi(aRow.GetAt(1));

			if (0 == aRow.GetAt(2).CompareNoCase(_T("spn")))
			{
				ntac.mvno_type = MMICONNECTION_MVNOTYPE_SPN;
			}
			else if(0 == aRow.GetAt(2).CompareNoCase(_T("gid")))
			{
				ntac.mvno_type = MMICONNECTION_MVNOTYPE_GID;
			}
			else if(0 == aRow.GetAt(2).CompareNoCase(_T("pnn")))
			{
				ntac.mvno_type = MMICONNECTION_MVNOTYPE_PNN;
			}
			else if(0 == aRow.GetAt(2).CompareNoCase(_T("imsi")))
			{
				ntac.mvno_type = MMICONNECTION_MVNOTYPE_IMSI;
			}
			else if(0 == aRow.GetAt(2).CompareNoCase(_T("imsir")))
			{
				ntac.mvno_type = MMICONNECTION_MVNOTYPE_IMSIR;
			}
			else
			{
				ntac.mvno_type = (unsigned short)_ttoi(aRow.GetAt(2));
			}

			_tcscpy(ntac.mvno_value,aRow.GetAt(3));
			ntac.mcc = (unsigned short)_ttoi(aRow.GetAt(4));
			ntac.mnc = (unsigned short)_ttoi(aRow.GetAt(5));
			ntac.account_type = (BYTE)_ttoi(aRow.GetAt(6));		
			_tcscpy(ntac.apn,aRow.GetAt(7));
			ntac.access_type = (BYTE)_ttoi(aRow.GetAt(8));		
			ntac.access_option = (BYTE)_ttoi(aRow.GetAt(9));	
			_tcscpy(ntac.gateway,aRow.GetAt(10));
			ntac.port = (unsigned short)_ttoi(aRow.GetAt(11));
		
			_tcscpy(ntac.username,aRow.GetAt(12));
			_tcscpy(ntac.password,aRow.GetAt(13));
			_tcscpy(ntac.dns1,aRow.GetAt(14));
			_tcscpy(ntac.dns2,aRow.GetAt(15));
			
			_tcscpy(ntac.homepage,aRow.GetAt(16));
			ntac.auth_type = (BYTE)_ttoi(aRow.GetAt(17));		
			ntac.reserved =(unsigned short)_ttoi(aRow.GetAt(18));

			if (0 == aRow.GetAt(19).CompareNoCase(_T("true")))
			{
				ntac.Visible = 0x01;
			}
			else if (0 == aRow.GetAt(19).CompareNoCase(_T("false")))
			{
				ntac.Visible = 0x00;
			}
			else
			{
				ntac.Visible = (BYTE)_ttoi(aRow.GetAt(19));
			}

			if (0 == aRow.GetAt(20).CompareNoCase(_T("true")))
			{
				ntac.Roaming = 0x01;
			}
			else if (0 == aRow.GetAt(20).CompareNoCase(_T("false")))
			{
				ntac.Roaming = 0x00;
			}
			else
			{
				ntac.Roaming = (BYTE)_ttoi(aRow.GetAt(20));
			}
			ntac.roaming_protocol = (BYTE)_ttoi(aRow.GetAt(21));	

			m_vNTAC.push_back(ntac);
		}
		else
		{
			bResult = FALSE;
			break;
		}
	}

	return bResult;

}