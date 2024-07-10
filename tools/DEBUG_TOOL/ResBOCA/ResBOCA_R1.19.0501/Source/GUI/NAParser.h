// NAParser.h: interface for the CNAParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NAPARSER_H__0EB648BA_98ED_4249_8425_79006B31CAD2__INCLUDED_)
#define AFX_NAPARSER_H__0EB648BA_98ED_4249_8425_79006B31CAD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(push,3)
#include <vector>
#pragma warning(pop)
#include "SpreadSheet.h"
#include "SpreadSheetET.h"

#define NTAC_MAGIC  0x4341544F//NTAC

#define NTAC_SPN_LEN		32
#define NTAC_NAME_LEN		28
#define NTAC_IP_LEN			15
#define NTAC_USERNAME_LEN   30
#define NTAC_PASSWORD_LEN   30
#define NTAC_APN_LEN		30
#define NTAC_HOMEPAGE_LEN	512
#define NTAC_MAX_NUM        0xFFFF
#define NTAC_PROXY_LEN      128

#define NTAC_VER            1

typedef struct _NTAC_FILE_HEAD_T
{
	DWORD magic;
	WORD  version;
	WORD  nCount;	
	_NTAC_FILE_HEAD_T()
	{
		memset(this,0,sizeof(_NTAC_FILE_HEAD_T));
	}
}NTAC_FILE_HEAD_T;

typedef struct _NTAC_T
{
	DWORD active_flag; // only used by tools
	WORD  reserved;//��������:0 ��ʾIPV4, 1��ʾIPV6, 2��ʾIPV4V6
	WORD  uiorder;
	WORD  mcc;//mcc country code, һ�㶼��3λ
    WORD  mnc;//mnc network code
    WORD  port;//����������˿ں� Ĭ��80
    BYTE  access_type;//��������:0 ��ʾwap1.2��WSP��, 1��ʾwap2.0(HTTP)//���ѡ��Wap1.2��portĬ�ϱ�Ϊ9201 , Ĭ��2.0
    BYTE  access_option;//��ȡѡ��,0:�ɱ༭;1:ֻ��
    BYTE  auth_type;//��������0:PAP��1:CHAP: //uint8
    BYTE  account_type;//�ʻ�����0:�������1:MMS��2:JAVA��3:DCD // uint8
	WCHAR name[NTAC_NAME_LEN+1];//�������� ���28
	WCHAR gateway[NTAC_PROXY_LEN+1];//���������IP��ַ Ĭ��0.0.0.0 ���128
	WCHAR dns1[NTAC_IP_LEN+1];//��ѡdns Ĭ��0.0.0.0 ���15
    WCHAR dns2[NTAC_IP_LEN+1];//��ѡdns Ĭ��0.0.0.0 ���15
    WCHAR username[NTAC_USERNAME_LEN+1];//�û���      ���30
    WCHAR password[NTAC_PASSWORD_LEN+1];//����        ���30
    WCHAR apn[NTAC_APN_LEN+1];//���������       ���30
    WCHAR homepage[NTAC_HOMEPAGE_LEN+1];//��ҳ��ַ    ���512
	WCHAR spn[NTAC_SPN_LEN+1];//SPN   ���32

	_NTAC_T()
	{
		Init();
	}
	
	void Init()
	{
		memset(this,0,sizeof(_NTAC_T));
		active_flag = 0;
		uiorder = 0;
		mcc = 0;
		mnc = 0;
		port = 80;
		access_type = 1;
		access_option = 0;
		auth_type = 0;
		account_type = 0;
		_tcscpy(gateway,_T("0.0.0.0"));
		_tcscpy(dns1,_T("0.0.0.0"));
		_tcscpy(dns2,_T("0.0.0.0"));
	}

}NTAC_T;

typedef enum
{
    MN_PAP       = 0,
    MN_CHAP      = 1,
	MN_PAP_CHAP	 = 2,
	MN_AUTH_NONE = 3
} MN_PCO_AUTH_TYPE_E;


typedef enum
{
    MMICONNECTION_ACCOUNT_BROWSER,	
    MMICONNECTION_ACCOUNT_MMS,
    MMICONNECTION_ACCOUNT_JAVA,
    MMICONNECTION_ACCOUNT_DCD,
    MMICONNECTION_ACCOUNT_MAX,
}MMICONNECTION_ACCOUNT_TYPE_E;

typedef enum {
	E_OK=0,
	E_INIT_XLS_ERR,
	E_OPEN_XLS_ERR,
	E_OPEN_SHEET_ERR,
	E_READ_ROW_ERR,
	E_INVALID_XLS_ERR,
	E_DEC_XLS_ERR,
	E_Count 		
}NAPASE_STATUS;  
class CNAParser  
{
public:
	CNAParser();
	virtual ~CNAParser();
	BOOL DecodeNTAC(LPBYTE pBuf,DWORD dwSize);
	BOOL EncodeNTAC(LPBYTE &pBuf,DWORD &dwSize);
	BOOL CreateNtacSheet(LPCTSTR lpszFileName);
	UINT ParseNtacSheet(LPCTSTR lpszFileName);

public:
	std::vector<NTAC_T> m_vNTAC;
	NTAC_FILE_HEAD_T m_tHead;

private: 
	BOOL DecodeString(LPBYTE &pBuf, LPBYTE pEnd, WCHAR *pString,WORD wStrMaxLen,BOOL bUnicode=FALSE);
	BOOL EncodeString(LPBYTE &pBuf, WCHAR *pString,BOOL bUnicode=FALSE);
	BOOL DecodeXls2Ntac(CSpreadSheet * pSpread);
	VOID Clear();
};

#endif // !defined(AFX_NAPARSER_H__0EB648BA_98ED_4249_8425_79006B31CAD2__INCLUDED_)
