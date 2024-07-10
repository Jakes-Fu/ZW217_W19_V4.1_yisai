// ����ȫ���õ���һЩID 
// anli.wei
// 

#if !defined(__GLOBALDEF_H__)
#define __GLOBALDEF_H__

#pragma once
const DWORD DEFAULT_BAUDRATE = 115200;

const WORD ID_CATEG_SMS   	= 0;
const WORD ID_CATEG_PB    	= 1;
const WORD ID_CATEG_MEMO  	= 2;
//const WORD ID_CATEG_SCHDL 	= 3;
const WORD ID_CATEG_PBG     = 3;
const WORD ID_CATEG_PBGM    = 4;

// ע�⣺�����ȡֵҪ��֤��ALL֮�⣬�໥֮�����ֵΪ0
const WORD ID_STORE_PC        = 0x0001;
const WORD ID_STORE_PHONE     = 0x0002;
const WORD ID_STORE_SIM       = 0x0004;
const WORD ID_STORE_SIM2      = 0x0008;
const WORD ID_STORE_INBOX     = 0x0010; // �ռ���
const WORD ID_STORE_OUTBOX    = 0x0020; // ������
const WORD ID_STORE_SENDS     = 0x0040; // �ѷ���Ϣ
const WORD ID_STORE_DRAFTS    = 0x0080; // �ݸ���
const WORD ID_STORE_SIM3      = 0x0100;
const WORD ID_STORE_SIM4      = 0x0200;
const WORD ID_STORE_ALL       = 0xFFFF;

// ���ŵ�״̬
const WORD SMS_STATUS_UNKNOWN= 0x0000;  // δ֪
const WORD SMS_STATUS_READ   = 0x0001;  // �Ѷ�
const WORD SMS_STATUS_UNREAD = 0x0002;  // δ��
const WORD SMS_STATUS_SEND   = 0x0003;  // �ѷ���
const WORD SMS_STATUS_UNSEND = 0x0004;  // δ����

//
const int MAX_SMS_CAPTION_LEN    = 50; // ���ű��ⳤ��
const int MAX_PERSON_NAME_LEN    = 240; // �˵����Ƶĳ���
const int MAX_PHONE_NUM_LEN      = 20; // �绰����ĳ���
const int MAX_EMAIL_LEN          = 40; // 
const int MAX_GROUP_LEN          = 16;
const int MAX_DATE_LEN           = 19;
const int MAX_SMS_LEN            = 140; // ascii(8bit ����)���
const int MAX_SMS_UCS2_LEN       = 70;  // ����ucs2�ַ����
const int MAX_MEMO_LEN           = 50;
const int MAX_SCHDL_LEN          = 50;
const int MAX_GROUP_NAME_LEN     = 16;

const int MAX_PERSON_NAME_LEN_V0 = 12; // �ɰ汾�ĳ���


const int MAX_GROUP_NUM = 20;

const int MAX_LONG_SMS_LEN       = 765;  //153 * 5 ������ ASCII
const int MAX_LONG_SMS_UCS2_LEN  = 335;  //(70 - 3) *5   ������ UCS2


const WORD ID_STORE_PBGM[MAX_GROUP_NUM]={0x0100,0x0200,0x0300,0x0400,
										0x0500,0x0600,0x0700,0x0800,
										0x0900,0x0a00,0x0b00,0x0c00,
										0x0d00,0x0e00,0x0f00,0x1000,
										0x1100,0x1200,0x1300,0x1400};

const int ID_PGM_SIM1    = MAX_GROUP_NUM-1;
const int ID_PGM_SIM2    = MAX_GROUP_NUM-2;
const int ID_PGM_SIM3    = MAX_GROUP_NUM-3;
const int ID_PGM_SIM4    = MAX_GROUP_NUM-4;

#define MMI_VERSION_1        1		//1.00
#define MMI_VERSION_2        2		//2.0x
#define MMI_VERSION_1_1      101	//1.01

//==========================================================================//
// ע�⣺����Ժ�İ汾����Ҫ����Ľṹ������Ҫ�����µ���Ŀ��Ϊ�˱��ִ洢
//       �汾�ļ��ݣ���Ҫ��������Ŀ���ڱ��ṹ��ĺ��棬������Ҫɾ�����е�
//       ĳ��Ŀ!!!
//==========================================================================//
typedef struct _DATE_TIME_T
{
	int nYear;
	int nMonth;
	int nDay;
	int nHour;
	int nMinut;
	int nSecond;
}DATE_TIME_T,*PDATE_TIME_T;
// 
typedef struct _SMS
{
	_SMS() { memset( this, 0, sizeof(_SMS) ); }

	_SMS * pPrev;
	_SMS * pNext;

	WORD wStore;     // ������ο�GlobalDef.h
	WORD wStatus;
    UINT nIdx;       // �洢��MS��SIM���ж��ŵ�����
    int  nValidDate; // ��Ч��(��λ��Сʱ)
	BOOL bUcs2;
	BOOL bExistSC;   // ���wStatus = UNSEND��FALSE�����ڲݸ��䣻TRUE�����ڷ�����

	_TCHAR   szName[MAX_PERSON_NAME_LEN + 1];
	_TCHAR   szPhone[MAX_PHONE_NUM_LEN + 1];
	_TCHAR   szDate[MAX_DATE_LEN + 1];
	_TCHAR   szContent[ MAX_SMS_LEN + 21];

} SMS, * PSMS;

typedef struct _PB
{
	_PB() { memset( this, 0, sizeof(_PB) ); }

	_PB * pPrev;
	_PB * pNext;

	WORD wStore;     // ������ο�GlobalDef.h
	WORD wReserve;

	UINT   nID;
	UINT   nGroupIndex;
	_TCHAR szName[MAX_PERSON_NAME_LEN + 1];
	_TCHAR szPhone[MAX_PHONE_NUM_LEN*2];
	_TCHAR szOfficePhone[MAX_PHONE_NUM_LEN*2];
	_TCHAR szHomePhone[MAX_PHONE_NUM_LEN*2];
	_TCHAR szFax[MAX_PHONE_NUM_LEN*2];
	_TCHAR szEmail[MAX_EMAIL_LEN*2];

} PB, * PPB;

namespace PB_V0
{
	typedef struct _PB
	{
		_PB() { memset( this, 0, sizeof(_PB) ); }
		
		_PB * pPrev;
		_PB * pNext;
		
		WORD wStore;     // ������ο�GlobalDef.h
		WORD wReserve;
		
		UINT    nID;
		UINT    nGroupIndex;
		_TCHAR szName[MAX_PERSON_NAME_LEN_V0 + 1];
		_TCHAR szPhone[MAX_PHONE_NUM_LEN + 1];
		_TCHAR szOfficePhone[MAX_PHONE_NUM_LEN + 1];
		_TCHAR szHomePhone[MAX_PHONE_NUM_LEN + 1];
		_TCHAR szFax[MAX_PHONE_NUM_LEN + 1];
		_TCHAR szEmail[MAX_EMAIL_LEN + 1];
		
	} PB, * PPB;
}

typedef struct _MEMO_DATE
{
	WORD wYear;
	BYTE btMonth;
	BYTE btDay;
	BYTE btHour;
	BYTE btMinut;
}MEMO_DATE;

typedef struct _MEMO
{
	_MEMO() { memset( this, 0, sizeof(_MEMO) ); }

	_MEMO * pPrev;
	_MEMO * pNext;

	WORD wStore;     // ������ο�GlobalDef.h
    WORD wReserve;

	UINT nID;

	WORD nMemoMode;

	MEMO_DATE memoTime;
	MEMO_DATE memoStartDate;
	MEMO_DATE memoEndDate;

	BYTE  fre;
	DWORD nServiceID;

	BOOL bUsed;
	BOOL bUcs2;
	_TCHAR szContent[ MAX_MEMO_LEN + 1];

} MEMO, * PMEMO;

typedef struct _SCHDL
{
	_SCHDL() { memset( this, 0, sizeof(_SCHDL) ); }

	_SCHDL * pPrev;
	_SCHDL * pNext;

	UINT  nID;
	WORD wStore;     // ������ο�GlobalDef.h

    WORD wYear;
	BYTE btMonth;
	BYTE btDay;
	BYTE btHour;
	BYTE btMinut;

	BOOL bUsed;
	BOOL bUcs2;
	_TCHAR szContent[ MAX_SCHDL_LEN + 1];

} SCHDL, * PSCHDL;

typedef struct _PBGROUP
{
	_PBGROUP() { memset( this, 0, sizeof(_PBGROUP) ); }

	_PBGROUP * pPrev;
	_PBGROUP * pNext;

	UINT  nID;
	WORD wStore;     // ������ο�GlobalDef.h
	
	BOOL bFixedGroup;
	BOOL bUcs2;
	int  nMemberNum;
	_TCHAR szName[ MAX_GROUP_NAME_LEN + 1];

} PBGROUP, * PPBGROUP;

typedef struct _PROGRESS_PARAM_T
{
	_TCHAR szTitle[MAX_PATH]; //����
	int    nTotalNum;         //�������Ļ���
	int    nCurNum;           //��ǰ����
}PROGRESS_PARAM_T, *PPROGRESS_PARAM_T;

#endif // __GLOBALDEF_H__
