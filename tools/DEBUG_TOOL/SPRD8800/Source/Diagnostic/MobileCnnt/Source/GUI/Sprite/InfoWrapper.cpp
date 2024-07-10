// InfoWrapper.cpp: implementation of the CInfoWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\mobileCnnt.h"
#include "InfoWrapper.h"
#include "GlobalDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int CInfoWrapper::IDX_CATEG_SMS         = 0;
const int CInfoWrapper::IDX_CATEG_PB          = 1;
const int CInfoWrapper::IDX_CATEG_MEMO        = 2;
const int CInfoWrapper::IDX_CATEG_PBG         = 3;

const int CInfoWrapper::IDX_CATEG_SMS_PC      = 4;
const int CInfoWrapper::IDX_CATEG_SMS_INBOX   = 5;
const int CInfoWrapper::IDX_CATEG_SMS_OUTBOX  = 6;
const int CInfoWrapper::IDX_CATEG_SMS_SENDS   = 7;
const int CInfoWrapper::IDX_CATEG_SMS_DRAFTS  = 8;

const int CInfoWrapper::IDX_CATEG_PB_PC       = 9;
const int CInfoWrapper::IDX_CATEG_PB_PHONE    = 10;
const int CInfoWrapper::IDX_CATEG_PB_SIM      = 11;
const int CInfoWrapper::IDX_CATEG_PB_SIM2     = 12;
const int CInfoWrapper::IDX_CATEG_PB_SIM3     = 13;
const int CInfoWrapper::IDX_CATEG_PB_SIM4     = 14;

const int CInfoWrapper::IDX_CATEG_MEMO_PC     = 15;
const int CInfoWrapper::IDX_CATEG_MEMO_PHONE  = 16;

const int CInfoWrapper::IDX_CATEG_PBGM[] = {17,18,
                                            19,20,21,22,23,
											24,25,26,27,28,
											29,30,31,32,33,34,35,36};

const int CInfoWrapper::MAX_CATEG_ITEM_COUNT  = 37;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInfoWrapper::CInfoWrapper()
{
	m_pArrImg     = NULL;
	m_pArrID      = NULL;
	m_dwCategItem = 0x10000000;

	m_nCurSelPresetSMS = -1;
	m_nCurSelPB        = -1;
	m_bSyncDlg         = FALSE;
	
	m_pCategInfo = NULL;
	memset(m_arrSMSClmn,0,sizeof(m_arrSMSClmn));	
	memset(m_arrSMSClmn,0,sizeof(m_arrSMSClmn));
	memset(m_arrSMSClmnWidth,0,sizeof(m_arrSMSClmnWidth));
	memset(m_arrPBClmn,0,sizeof(m_arrPBClmn));
	memset(m_arrPBClmnWidth,0,sizeof(m_arrPBClmnWidth));
	memset(m_arrMEMOClmn,0,sizeof(m_arrMEMOClmn));
	memset(m_arrMEMOClmnWidth,0,sizeof(m_arrMEMOClmnWidth));
	memset(m_arrPBGClmn,0,sizeof(m_arrPBGClmn));

	memset(m_arrPBGClmnWidth,0,sizeof(m_arrPBGClmnWidth));
	memset(m_arrPBGMClmn,0,sizeof(m_arrPBGMClmn));         
	memset(m_arrPBGMClmnWidth,0,sizeof(m_arrPBGMClmnWidth));     
	memset(m_szTipsInfo,0,sizeof(m_szTipsInfo));      
	m_nCurSelPBG = -1;
	m_nTotalSmsCount = 0;
	m_nUsedSmsCount = 0;
}

CInfoWrapper::~CInfoWrapper()
{
	if(m_pArrImg != NULL)
	{
		delete []m_pArrImg;
		m_pArrImg = NULL;
	}
	if(m_pArrID != NULL)
	{
		delete []m_pArrID;
		m_pArrID = NULL;
	}

	if(m_pCategInfo != NULL)
	{
		delete []m_pCategInfo;
		m_pCategInfo = NULL;
	}
}

BOOL CInfoWrapper::MakeID()
{
	_ASSERTE( m_pArrID == NULL && NULL == m_pArrImg );

	m_pArrImg = new int[MAX_CATEG_ITEM_COUNT];
	if( NULL == m_pArrImg )
	{
		TRACE0("new fail 1 in CInfoWrapper::MakeID()\r\n");
		return FALSE;
	}

	m_pArrID = new DWORD[MAX_CATEG_ITEM_COUNT];
	if( m_pArrID == NULL )
	{
		TRACE0("new fail 2 in CInfoWrapper::MakeID()\r\n");
		return FALSE;
	}

	m_pArrImg[IDX_CATEG_SMS]   = 0;
	m_pArrID[IDX_CATEG_SMS]    = MAKELONG(ID_STORE_ALL, ID_CATEG_SMS);
	m_pArrImg[IDX_CATEG_PB]    = 2;
	m_pArrID[IDX_CATEG_PB]     = MAKELONG(ID_STORE_ALL, ID_CATEG_PB);
	m_pArrImg[IDX_CATEG_MEMO]  = 4;
	m_pArrID[IDX_CATEG_MEMO]   = MAKELONG(ID_STORE_ALL, ID_CATEG_MEMO);
	m_pArrImg[IDX_CATEG_PBG] = 6;
	m_pArrID[IDX_CATEG_PBG]  = MAKELONG(ID_STORE_ALL, ID_CATEG_PBG);

	m_pArrImg[IDX_CATEG_SMS_PC]    = 8;
	m_pArrID[IDX_CATEG_SMS_PC]     = MAKELONG(ID_STORE_PC,    ID_CATEG_SMS);

	m_pArrImg[IDX_CATEG_SMS_INBOX]  = 14;
	m_pArrID[IDX_CATEG_SMS_INBOX]   = MAKELONG(ID_STORE_INBOX, ID_CATEG_SMS);
	m_pArrImg[IDX_CATEG_SMS_OUTBOX]	= 16;
	m_pArrID[IDX_CATEG_SMS_OUTBOX]  = MAKELONG(ID_STORE_OUTBOX,ID_CATEG_SMS);
	m_pArrImg[IDX_CATEG_SMS_SENDS]  = 18;
	m_pArrID[IDX_CATEG_SMS_SENDS]   = MAKELONG(ID_STORE_SENDS, ID_CATEG_SMS);
	m_pArrImg[IDX_CATEG_SMS_DRAFTS] = 20;
	m_pArrID[IDX_CATEG_SMS_DRAFTS]  = MAKELONG(ID_STORE_DRAFTS,ID_CATEG_SMS);

	m_pArrImg[IDX_CATEG_PB_PC]    = 8;
	m_pArrID[IDX_CATEG_PB_PC]     = MAKELONG(ID_STORE_PC,    ID_CATEG_PB);
	m_pArrImg[IDX_CATEG_PB_PHONE] = 10;
	m_pArrID[IDX_CATEG_PB_PHONE]  = MAKELONG(ID_STORE_PHONE, ID_CATEG_PB);
	m_pArrImg[IDX_CATEG_PB_SIM]   = 12;
	m_pArrID[IDX_CATEG_PB_SIM]    = MAKELONG(ID_STORE_SIM,   ID_CATEG_PB);
	m_pArrImg[IDX_CATEG_PB_SIM2]   = 14;
	m_pArrID[IDX_CATEG_PB_SIM2]    = MAKELONG(ID_STORE_SIM2,   ID_CATEG_PB);
	m_pArrImg[IDX_CATEG_PB_SIM3]   = 16;
	m_pArrID[IDX_CATEG_PB_SIM3]    = MAKELONG(ID_STORE_SIM3,   ID_CATEG_PB);
	m_pArrImg[IDX_CATEG_PB_SIM4]   = 18;
	m_pArrID[IDX_CATEG_PB_SIM4]    = MAKELONG(ID_STORE_SIM4,   ID_CATEG_PB);

	m_pArrImg[IDX_CATEG_MEMO_PC]    = 8;
	m_pArrID[IDX_CATEG_MEMO_PC]     = MAKELONG(ID_STORE_PC,    ID_CATEG_MEMO);
	m_pArrImg[IDX_CATEG_MEMO_PHONE] = 10;
	m_pArrID[IDX_CATEG_MEMO_PHONE]  = MAKELONG(ID_STORE_PHONE, ID_CATEG_MEMO);

/*  m_pArrImg[IDX_CATEG_PBG_A]    = 8;
	m_pArrID[IDX_CATEG_PBG_A]     = MAKELONG(ID_STORE_A,    ID_CATEG_PBGM);
	m_pArrImg[IDX_CATEG_PBG_B] = 10;
	m_pArrID[IDX_CATEG_PBG_B]  = MAKELONG(ID_STORE_B, ID_CATEG_PBGM);
	m_pArrImg[IDX_CATEG_PBG_C]     = 12;
	m_pArrID[IDX_CATEG_PBG_C]      = MAKELONG(ID_STORE_C,    ID_CATEG_PBGM);
	m_pArrImg[IDX_CATEG_PBG_D]  = 14;
	m_pArrID[IDX_CATEG_PBG_D]   = MAKELONG(ID_STORE_D, ID_CATEG_PBGM);
	m_pArrImg[IDX_CATEG_PBG_E]    = 16;
	m_pArrID[IDX_CATEG_PBG_E]     = MAKELONG(ID_STORE_E,   ID_CATEG_PBGM);
*/
	for(int i=0; i < MAX_GROUP_NUM; i++)
	{
		m_pArrImg[IDX_CATEG_PBGM[i]]    = 8;
		m_pArrID[IDX_CATEG_PBGM[i]]     = MAKELONG(ID_STORE_PBGM[i],    ID_CATEG_PBGM);
	}
	return TRUE;
}

BOOL CInfoWrapper::Init()
{
	if( !MakeID() )
		return FALSE;

	m_pCategInfo = new CATEGINFO[MAX_CATEG_ITEM_COUNT];
	if( NULL == m_pCategInfo )
		return FALSE;

	memset(m_pCategInfo, 0, sizeof(CATEGINFO) * MAX_CATEG_ITEM_COUNT);

	int        i    = 0;
	PCATEGINFO pci  = NULL;
	
	// 以下形成链表结构
	pci         = m_pCategInfo + IDX_CATEG_SMS;
	pci->pChild = m_pCategInfo + IDX_CATEG_SMS_PC;
	pci         = pci->pChild;
	for( i = IDX_CATEG_SMS_PC; i <= IDX_CATEG_SMS_DRAFTS; ++i )
	{
		pci->dwData         = m_pArrID[i];
		pci->nImage         = m_pArrImg[i];
		pci->nSelectedImage = m_pArrImg[i] + 1;
		pci->pNext          = pci + 1;
		pci                 = pci->pNext;
	}
	m_pCategInfo[IDX_CATEG_SMS_DRAFTS].pNext = NULL;

	pci         = m_pCategInfo + IDX_CATEG_PB;
	pci->pChild = m_pCategInfo + IDX_CATEG_PB_PC;
	pci         = pci->pChild;
	for( i = IDX_CATEG_PB_PC; i <= IDX_CATEG_PB_SIM4; ++i )
	{
		pci->dwData         = m_pArrID[i];
		pci->nImage         = m_pArrImg[i];
		pci->nSelectedImage = m_pArrImg[i] + 1;
		pci->pNext          = pci + 1;
		pci                 = pci->pNext;
	}
	m_pCategInfo[IDX_CATEG_PB_SIM4].pNext = NULL;

	pci         = m_pCategInfo + IDX_CATEG_MEMO;
	pci->pChild = m_pCategInfo + IDX_CATEG_MEMO_PC;
	pci         = pci->pChild;
	for( i = LOWORD(IDX_CATEG_MEMO_PC); i <= LOWORD(IDX_CATEG_MEMO_PHONE); ++i )
	{
		pci->dwData         = m_pArrID[i];
		pci->nImage         = m_pArrImg[i];
		pci->nSelectedImage = m_pArrImg[i] + 1;
		pci->pNext          = pci + 1;
		pci                 = pci->pNext;
	}
	m_pCategInfo[IDX_CATEG_MEMO_PHONE].pNext = NULL;

/*	pci         = m_pCategInfo + IDX_CATEG_PBG;
	pci->pChild = m_pCategInfo + IDX_CATEG_PBG_PC;
	pci         = pci->pChild;
	for(i = IDX_CATEG_PBG_PC; i <= IDX_CATEG_PBG_PHONE; ++i )
	{
		pci->dwData         = m_pArrID[i];
		pci->nImage         = m_pArrImg[i];
		pci->nSelectedImage = m_pArrImg[i] + 1;
		pci->pNext          = pci + 1;
		pci                 = pci->pNext;
	}
	m_pCategInfo[IDX_CATEG_PBG_PHONE].pNext = NULL;
*/
	for( i = IDX_CATEG_SMS; i <= IDX_CATEG_PBG; ++i )
	{
		pci                 = m_pCategInfo + i;
		pci->dwData         = m_pArrID[i];
		pci->nImage         = m_pArrImg[i];
		pci->nSelectedImage = m_pArrImg[i] + 1;
		pci->pNext          = pci + 1;
	}
	pci->pNext = NULL;

	Refresh();

	return TRUE;
}

void CInfoWrapper::Refresh()
{
	_TCHAR szPC[MAX_CATEG_TEXT_LEN]    = { 0 };
	_TCHAR szPhone[MAX_CATEG_TEXT_LEN] = { 0 };
	_TCHAR szSIM[MAX_CATEG_TEXT_LEN]   = { 0 };
	_TCHAR szSIM2[MAX_CATEG_TEXT_LEN]   = { 0 };
	_TCHAR szSIM3[MAX_CATEG_TEXT_LEN]   = { 0 };
	_TCHAR szSIM4[MAX_CATEG_TEXT_LEN]   = { 0 };
	_TCHAR szTmp[MAX_CATEG_TEXT_LEN]   = { 0 };

	HINSTANCE hRes = AfxGetResourceHandle();

	PCATEGINFO pci = m_pCategInfo;
	pci = m_pCategInfo + IDX_CATEG_SMS;
	VERIFY( LoadString(hRes, IDS_CATEG_SMS, pci->szText, MAX_CATEG_TEXT_LEN) > 0 );

	pci = m_pCategInfo + IDX_CATEG_PB;
	VERIFY( LoadString(hRes, IDS_CATEG_PB, pci->szText, MAX_CATEG_TEXT_LEN) > 0 );

	pci = m_pCategInfo + IDX_CATEG_MEMO;
	VERIFY( LoadString(hRes, IDS_CATEG_MEMO, pci->szText, MAX_CATEG_TEXT_LEN) > 0 );

	pci = m_pCategInfo + IDX_CATEG_PBG;
	VERIFY( LoadString(hRes, IDS_CATEG_PBG, pci->szText, MAX_CATEG_TEXT_LEN) > 0 );

	VERIFY( LoadString(hRes, IDS_STORE_PC,    szPC,    MAX_CATEG_TEXT_LEN) > 0 );
	VERIFY( LoadString(hRes, IDS_STORE_PHONE, szPhone, MAX_CATEG_TEXT_LEN) > 0 );
	VERIFY( LoadString(hRes, IDS_STORE_SIM,   szSIM,   MAX_CATEG_TEXT_LEN) > 0 );
	VERIFY( LoadString(hRes, IDS_STORE_SIM2,  szSIM2,  MAX_CATEG_TEXT_LEN) > 0 );
	VERIFY( LoadString(hRes, IDS_STORE_SIM3,  szSIM3,  MAX_CATEG_TEXT_LEN) > 0 );
	VERIFY( LoadString(hRes, IDS_STORE_SIM4,  szSIM4,  MAX_CATEG_TEXT_LEN) > 0 );

	pci = m_pCategInfo + IDX_CATEG_SMS_PC;
	_tcscpy(pci->szText, szPC);

	VERIFY( LoadString(hRes, IDS_STORE_INBOX,   szTmp,   MAX_CATEG_TEXT_LEN) > 0 );
	pci = m_pCategInfo + IDX_CATEG_SMS_INBOX;
	_tcscpy(pci->szText, szTmp);

	pci = m_pCategInfo + IDX_CATEG_SMS_OUTBOX;
	VERIFY( LoadString(hRes, IDS_STORE_OUTBOX,   szTmp,   MAX_CATEG_TEXT_LEN) > 0 );
	_tcscpy(pci->szText, szTmp);

	pci = m_pCategInfo + IDX_CATEG_SMS_SENDS;
	VERIFY( LoadString(hRes, IDS_STORE_SENDS,   szTmp,   MAX_CATEG_TEXT_LEN) > 0 );
	_tcscpy(pci->szText, szTmp);

	pci = m_pCategInfo + IDX_CATEG_SMS_DRAFTS;
	VERIFY( LoadString(hRes, IDS_STORE_DRAFTS,   szTmp,   MAX_CATEG_TEXT_LEN) > 0 );
	_tcscpy(pci->szText, szTmp);

	pci = m_pCategInfo + IDX_CATEG_PB_PC;
	_tcscpy(pci->szText, szPC);

	pci = m_pCategInfo + IDX_CATEG_PB_PHONE;
	_tcscpy(pci->szText, szPhone);

	pci = m_pCategInfo + IDX_CATEG_PB_SIM;
	_tcscpy(pci->szText, szSIM);

	pci = m_pCategInfo + IDX_CATEG_PB_SIM2;
	_tcscpy(pci->szText, szSIM2);

	pci = m_pCategInfo + IDX_CATEG_PB_SIM3;
	_tcscpy(pci->szText, szSIM3);
	
	pci = m_pCategInfo + IDX_CATEG_PB_SIM4;
	_tcscpy(pci->szText, szSIM4);

	pci = m_pCategInfo + IDX_CATEG_MEMO_PC;
	_tcscpy(pci->szText, szPC);

	pci = m_pCategInfo + IDX_CATEG_MEMO_PHONE;
	_tcscpy(pci->szText, szPhone);

	//pci = m_pCategInfo + IDX_CATEG_PBG_PC;
	//_tcscpy(pci->szText, szPC);

	//pci = m_pCategInfo + IDX_CATEG_PBG_PHONE;
	//_tcscpy(pci->szText, szPhone);

	InitColumns(hRes);
}

void CInfoWrapper::InitColumns(HINSTANCE hRes)
{
	static const int nMaxLen = 512;
	_TCHAR szValue[ nMaxLen ] = { 0 };

	VERIFY( LoadString(hRes, IDS_SMS_COLUMN, szValue, nMaxLen) > 0 );

	const _TCHAR cFlag = _T(',');
	LPTSTR pText = szValue;
	LPTSTR pFind = _tcschr(szValue, cFlag);
    for( int i = 0; pFind != NULL; ++i)
	{
		*pFind = '\0';
        _tcscpy(m_arrSMSClmn[i], pText);
		pText = pFind + 1;

		pFind = _tcschr(pText, cFlag);
	}
	_ASSERTE( i == MAX_SMS_COLUMN - 1 );
	_tcscpy(m_arrSMSClmn[i], pText);
	m_arrSMSClmnWidth[0] = 60;  // storeplace
	m_arrSMSClmnWidth[1] = 60;  // status
	m_arrSMSClmnWidth[2] = 80;  // name
	m_arrSMSClmnWidth[3] = 100; // phone num
	m_arrSMSClmnWidth[4] = 120; // date
	m_arrSMSClmnWidth[5] = 160; // content

	VERIFY( LoadString(hRes, IDS_PB_COLUMN, szValue, nMaxLen) > 0 );
	pText = szValue;
	pFind = _tcschr(szValue, cFlag);
    for( i = 0; pFind != NULL; ++i)
	{
		*pFind = '\0';
        _tcscpy(m_arrPBClmn[i], pText);
		pText = pFind + 1;

		pFind = _tcschr(pText, cFlag);
	}
	_ASSERTE( i == MAX_PB_COLUMN - 1 );
	_tcscpy(m_arrPBClmn[i], pText);
	m_arrPBClmnWidth[0] = 60; // place
	m_arrPBClmnWidth[1] = 80; // name
	m_arrPBClmnWidth[2] = 100; // phone
	m_arrPBClmnWidth[3] = 100; // office phone
	m_arrPBClmnWidth[4] = 100; // home phone
	m_arrPBClmnWidth[5] = 100; // home phone
	m_arrPBClmnWidth[6] = 80; // email
	m_arrPBClmnWidth[7] = 60; // group

	VERIFY( LoadString(hRes, IDS_MEMO_COLUMN, szValue, nMaxLen) > 0 );
	pText = szValue;
	pFind = _tcschr(szValue, cFlag);
    for( i = 0; pFind != NULL; ++i)
	{
		*pFind = '\0';
        _tcscpy(m_arrMEMOClmn[i], pText);
		pText = pFind + 1;

		pFind = _tcschr(pText, cFlag);
	}
	_ASSERTE( i == MAX_MEMO_COLUMN - 1 );
	_tcscpy(m_arrMEMOClmn[i], pText);
	m_arrMEMOClmnWidth[0] = 80; // place
	m_arrMEMOClmnWidth[1] = 160; // title
	m_arrMEMOClmnWidth[2] = 100; // date
	m_arrMEMOClmnWidth[3] = 100; // time
	m_arrMEMOClmnWidth[4] = 100; // mode
	m_arrMEMOClmnWidth[5] = 100; // fre
	// content
	m_arrMEMOClmnWidth[6] = 100; /*lint !e415 */

	VERIFY( LoadString(hRes, IDS_PBGM_COLUMN, szValue, nMaxLen) > 0 );
	pText = szValue;
	pFind = _tcschr(szValue, cFlag);
    for( i = 0; pFind != NULL; ++i)
	{
		*pFind = '\0';
        _tcscpy(m_arrPBGMClmn[i], pText);
		pText = pFind + 1;

		pFind = _tcschr(pText, cFlag);
	}
	_ASSERTE( i == MAX_PBGM_COLUMN - 1 );
	_tcscpy(m_arrPBGMClmn[i], pText);
	m_arrPBGMClmnWidth[0] = 80; // place
	m_arrPBGMClmnWidth[1] = 80; // Name
	m_arrPBGMClmnWidth[2] = 100; // phone
	m_arrPBGMClmnWidth[3] = 100; // office phone
	m_arrPBGMClmnWidth[4] = 100; // home phone
	m_arrPBGMClmnWidth[5] = 100; // home phone
	m_arrPBGMClmnWidth[6] = 80; // email

	VERIFY( LoadString(hRes, IDS_PBG_COLUMN, szValue, nMaxLen) > 0 );
	pText = szValue;
	pFind = _tcschr(szValue, cFlag);
    for( i = 0; pFind != NULL; ++i)
	{
		*pFind = '\0';
        _tcscpy(m_arrPBGClmn[i], pText);
		pText = pFind + 1;

		pFind = _tcschr(pText, cFlag);
	}
	_ASSERTE( i == MAX_PBG_COLUMN - 1 );
	_tcscpy(m_arrPBGClmn[i], pText);
	m_arrPBGClmnWidth[0] = 80; // place
	m_arrPBGClmnWidth[1] = 80; // Name
	m_arrPBGClmnWidth[2] = 80; // Member Num

	VERIFY( LoadString(hRes, IDS_MEMO_MODE, szValue, nMaxLen) > 0 );
	pText = szValue;
	pFind = _tcschr(szValue, cFlag);
    for( i = 0; pFind != NULL; ++i)
	{
		*pFind = '\0';
        m_arrMemoMode.Add(pText);
		pText = pFind + 1;
		pFind = _tcschr(pText, cFlag);
	}
	m_arrMemoMode.Add(pText);

	memset(szValue,0,sizeof(_TCHAR)*nMaxLen);
	VERIFY( LoadString(hRes, IDS_MEMO_FRE, szValue, nMaxLen) > 0 );
	pText = szValue;
	pFind = _tcschr(szValue, cFlag);
    for( i = 0; pFind != NULL; ++i)
	{
		*pFind = '\0';
        m_arrMemoFre.Add(pText);
		pText = pFind + 1;
		pFind = _tcschr(pText, cFlag);
	}
	m_arrMemoFre.Add(pText);

}

BOOL CInfoWrapper::UpdateCategPBG(void)
{
    int nCount = m_arrPBG.GetSize();
	if( m_arrPBG.GetSize() > 0 )
	{
		PCATEGINFO pci = m_pCategInfo;
		CString str;		
		for(int i = 0 ; i < nCount; i++)
		{
			pci = m_pCategInfo + IDX_CATEG_PBGM[i];
//			str.Format(_T("%s(%d)"),m_arrPBG.GetAt(i)->szName,
//				m_arrPBG.GetAt(i)->nMemberNum);
//			_tcscpy(pci->szText, str);
			_tcscpy(pci->szText,m_arrPBG.GetAt(i)->szName);
		}

		pci         = m_pCategInfo + IDX_CATEG_PBG;
		pci->pChild = m_pCategInfo + IDX_CATEG_PBGM[0];
		pci         = pci->pChild;
		for(i = IDX_CATEG_PBGM[0]; i <= IDX_CATEG_PBGM[nCount-1]; ++i )
		{
			pci->dwData         = m_pArrID[i];
			pci->nImage         = m_pArrImg[i];
			pci->nSelectedImage = m_pArrImg[i] + 1;
			pci->pNext          = pci + 1;
			pci                 = pci->pNext;
		}
		m_pCategInfo[IDX_CATEG_PBGM[nCount-1]].pNext = NULL;

		return TRUE;
	}
	return FALSE;
}
void CInfoWrapper::Release()
{
	delete []m_pArrImg;
	m_pArrImg = NULL;
	delete []m_pArrID;
	m_pArrID = NULL;
	delete []m_pCategInfo;
	m_pCategInfo = NULL;

	RemoveAllPB();
	RemoveAllPresetSMS();
	RemoveAllSMS();
	RemoveAllMEMO();
	RemoveAllPBG();
	//RemoveAllSCHDL();
}

int CInfoWrapper::GetColumnCount(DWORD dwFlag)
{
	dwFlag = HIWORD(dwFlag);

	if( dwFlag == m_dwCategItem && !m_bSyncDlg)
		return 0;

	m_dwCategItem = dwFlag;

	switch( dwFlag )
	{
	case ID_CATEG_SMS:
		return MAX_SMS_COLUMN;
	case ID_CATEG_PB:
		return MAX_PB_COLUMN;
	case ID_CATEG_MEMO:
		return MAX_MEMO_COLUMN;
	case ID_CATEG_PBG:
		return MAX_PBG_COLUMN;
	case ID_CATEG_PBGM:
		return MAX_PBGM_COLUMN;	
	default:
		_ASSERTE(0);
	}
	return 0;
}

LPCTSTR CInfoWrapper::GetColumn(DWORD dwFlag, int nIdx)
{
	dwFlag = HIWORD(dwFlag);

	switch( dwFlag )
	{
	case ID_CATEG_SMS:
		_ASSERTE( nIdx >= 0 && nIdx < MAX_SMS_COLUMN );
		return m_arrSMSClmn[nIdx]; //lint !e661
	case ID_CATEG_PB:
		_ASSERTE( nIdx >= 0 && nIdx < MAX_PB_COLUMN );
		return m_arrPBClmn[ nIdx ];//lint !e661
	case ID_CATEG_MEMO:
		_ASSERTE( nIdx >= 0 && nIdx < MAX_MEMO_COLUMN );
		return m_arrMEMOClmn[ nIdx ];//lint !e661
	case ID_CATEG_PBG:
		_ASSERTE( nIdx >= 0 && nIdx < MAX_PBG_COLUMN );
		return m_arrPBGClmn[ nIdx ];//lint !e661
	case ID_CATEG_PBGM:
		_ASSERTE( nIdx >= 0 && nIdx < MAX_PBGM_COLUMN );
		return m_arrPBGMClmn[ nIdx ];//lint !e661
	default:
		_ASSERTE(0);
	}
	return NULL;
}

int CInfoWrapper::GetColumnWidth(DWORD dwFlag, int nIdx)
{
	dwFlag = HIWORD(dwFlag);

	switch( dwFlag )
	{
	case ID_CATEG_SMS:
		_ASSERTE( nIdx >= 0 && nIdx < MAX_SMS_COLUMN );
		return m_arrSMSClmnWidth[nIdx];//lint !e661
	case ID_CATEG_PB:
		_ASSERTE( nIdx >= 0 && nIdx < MAX_PB_COLUMN );
		return m_arrPBClmnWidth[ nIdx ];//lint !e661
	case ID_CATEG_MEMO:
		_ASSERTE( nIdx >= 0 && nIdx < MAX_MEMO_COLUMN );
		return m_arrMEMOClmnWidth[ nIdx ];//lint !e661
	case ID_CATEG_PBG:
		_ASSERTE( nIdx >= 0 && nIdx < MAX_PBG_COLUMN );
		return m_arrPBGClmnWidth[ nIdx ];//lint !e661
	case ID_CATEG_PBGM:
		_ASSERTE( nIdx >= 0 && nIdx < MAX_PBGM_COLUMN );
		return m_arrPBGMClmnWidth[ nIdx ];//lint !e661
	default:
		_ASSERTE(0);
	}
	return NULL;
}

LPCTSTR CInfoWrapper::GetTipsInfo(DWORD dwFlag)
{
	WORD hiWord = HIWORD(dwFlag);
	WORD loWord = LOWORD(dwFlag);
    int i =0;
//	int nUsedCount = 0;
	CString strTemp;
	static const _TCHAR szSep[] = _T("|");
	switch( hiWord )
	{
	case ID_CATEG_SMS:
		_tcscpy(m_szTipsInfo, m_pCategInfo[IDX_CATEG_SMS].szText);
		switch( loWord )
		{
		case ID_STORE_PC:
			_tcscat(m_szTipsInfo, szSep);
			_tcscat(m_szTipsInfo, m_pCategInfo[IDX_CATEG_SMS_PC].szText);
			strTemp.Format(_T("(%d)"),GetSMSCount(ID_STORE_PC));
			_tcscat(m_szTipsInfo,strTemp);
			break;
		case ID_STORE_INBOX:
			_tcscat(m_szTipsInfo, szSep);
			_tcscat(m_szTipsInfo, m_pCategInfo[IDX_CATEG_SMS_INBOX].szText);
			strTemp.Format(_T("(%d)"),GetSMSCount(ID_STORE_INBOX));
			
			_tcscat(m_szTipsInfo,strTemp);
			break;
		case ID_STORE_OUTBOX:
			_tcscat(m_szTipsInfo, szSep);
			_tcscat(m_szTipsInfo, m_pCategInfo[IDX_CATEG_SMS_OUTBOX].szText);
			strTemp.Format(_T("(%d)"),GetSMSCount(ID_STORE_OUTBOX));
			_tcscat(m_szTipsInfo,strTemp);
			break;
		case ID_STORE_SENDS:
			_tcscat(m_szTipsInfo, szSep);
			_tcscat(m_szTipsInfo, m_pCategInfo[IDX_CATEG_SMS_SENDS].szText);
			strTemp.Format(_T("(%d)"),GetSMSCount(ID_STORE_SENDS));
			_tcscat(m_szTipsInfo,strTemp);
			break;
		case ID_STORE_DRAFTS:
			_tcscat(m_szTipsInfo, szSep);
			_tcscat(m_szTipsInfo, m_pCategInfo[IDX_CATEG_SMS_DRAFTS].szText);
			strTemp.Format(_T("(%d)"),GetSMSCount(ID_STORE_DRAFTS));
			_tcscat(m_szTipsInfo,strTemp);
			break;
		default:			
			strTemp.Format(_T("(%d)"),GetSMSCount(ID_STORE_ALL));
			_tcscat(m_szTipsInfo,strTemp);
			break;
		}
		break;
	case ID_CATEG_PB:
		_tcscpy(m_szTipsInfo, m_pCategInfo[IDX_CATEG_PB].szText);
		switch( loWord )
		{
		case ID_STORE_PC:
			_tcscat(m_szTipsInfo, szSep);
			_tcscat(m_szTipsInfo, m_pCategInfo[IDX_CATEG_PB_PC].szText);
			strTemp.Format(_T("(%d)"),GetPBCount(ID_STORE_PC));
			_tcscat(m_szTipsInfo,strTemp);
			break;
		case ID_STORE_PHONE:
			_tcscat(m_szTipsInfo, szSep);
			_tcscat(m_szTipsInfo, m_pCategInfo[IDX_CATEG_PB_PHONE].szText);
			strTemp.Format(_T("(%d)"),GetPBCount(ID_STORE_PHONE));
			_tcscat(m_szTipsInfo,strTemp);
			break;
		case ID_STORE_SIM:
			_tcscat(m_szTipsInfo, szSep);
			_tcscat(m_szTipsInfo, m_pCategInfo[IDX_CATEG_PB_SIM].szText);
			strTemp.Format(_T("(%d)"),GetPBCount(ID_STORE_SIM));
			_tcscat(m_szTipsInfo,strTemp);
			break;
		case ID_STORE_SIM2:
			_tcscat(m_szTipsInfo, szSep);
			_tcscat(m_szTipsInfo, m_pCategInfo[IDX_CATEG_PB_SIM2].szText);
			strTemp.Format(_T("(%d)"),GetPBCount(ID_STORE_SIM2));
			_tcscat(m_szTipsInfo,strTemp);
			break;
		case ID_STORE_SIM3:
			_tcscat(m_szTipsInfo, szSep);
			_tcscat(m_szTipsInfo, m_pCategInfo[IDX_CATEG_PB_SIM3].szText);
			strTemp.Format(_T("(%d)"),GetPBCount(ID_STORE_SIM3));
			_tcscat(m_szTipsInfo,strTemp);
			break;
		case ID_STORE_SIM4:
			_tcscat(m_szTipsInfo, szSep);
			_tcscat(m_szTipsInfo, m_pCategInfo[IDX_CATEG_PB_SIM4].szText);
			strTemp.Format(_T("(%d)"),GetPBCount(ID_STORE_SIM4));
			_tcscat(m_szTipsInfo,strTemp);
			break;
		default:
			strTemp.Format(_T("(%d)"),GetPBCount(ID_STORE_ALL));
			_tcscat(m_szTipsInfo,strTemp);
			break;
		}
		break;
	case ID_CATEG_MEMO:
		_tcscpy(m_szTipsInfo, m_pCategInfo[IDX_CATEG_MEMO].szText);
		switch( loWord )
		{
		case ID_STORE_PC:
			_tcscat(m_szTipsInfo, szSep);
			_tcscat(m_szTipsInfo, m_pCategInfo[IDX_CATEG_MEMO_PC].szText);
			strTemp.Format(_T("(%d)"),GetMEMOCount(ID_STORE_PC));
			_tcscat(m_szTipsInfo,strTemp);
			break;
		case ID_STORE_PHONE:
			_tcscat(m_szTipsInfo, szSep);
			_tcscat(m_szTipsInfo, m_pCategInfo[IDX_CATEG_MEMO_PHONE].szText);
			strTemp.Format(_T("(%d)"),GetMEMOCount(ID_STORE_PHONE));
			_tcscat(m_szTipsInfo,strTemp);
			break;
		default:
			strTemp.Format(_T("(%d)"),GetMEMOCount(ID_STORE_ALL));
			_tcscat(m_szTipsInfo,strTemp);
			break;
		}
		break;
	case ID_CATEG_PBG:
		_tcscpy(m_szTipsInfo, m_pCategInfo[ID_CATEG_PBG].szText);
		strTemp.Format(_T("(%d)"),m_arrPBG.GetSize());
		_tcscat(m_szTipsInfo, strTemp);
		break;
	case ID_CATEG_PBGM:
		_tcscpy(m_szTipsInfo, m_pCategInfo[ID_CATEG_PBG].szText);
		for(i = 0; i< MAX_GROUP_NUM;i++)
		{
			if(loWord == ID_STORE_PBGM[i])
			{
				_tcscat(m_szTipsInfo, szSep);
				//_tcscat(m_szTipsInfo, m_pCategInfo[IDX_CATEG_PBGM[i]].szText);
				_tcscat(m_szTipsInfo, m_pCategInfo[IDX_CATEG_PBGM[i]].szText);

				//strTemp.Format(_T("(%d)"),m_arrPBG.GetAt(i)->nMemberNum);
				//@hongliang 2007-4-24 重新计算群组成员个数，不用nMemberNum
				strTemp.Format(_T("(%d)"),GetPBGMemoberCount(i));
				_tcscat(m_szTipsInfo, strTemp);
				break;		
			}
		}
/*		switch( loWord )
		{
		case ID_STORE_A:
			_tcscat(m_szTipsInfo, szSep);
			_tcscat(m_szTipsInfo, m_pCategInfo[IDX_CATEG_PBG_A].szText);
			break;
		case ID_STORE_B:
			_tcscat(m_szTipsInfo, szSep);
			_tcscat(m_szTipsInfo, m_pCategInfo[IDX_CATEG_PBG_B].szText);
			break;
		case ID_STORE_C:
			_tcscat(m_szTipsInfo, szSep);
			_tcscat(m_szTipsInfo, m_pCategInfo[IDX_CATEG_PBG_C].szText);
			break;
		case ID_STORE_D:
			_tcscat(m_szTipsInfo, szSep);
			_tcscat(m_szTipsInfo, m_pCategInfo[IDX_CATEG_PBG_D].szText);
			break;
		case ID_STORE_E:
			_tcscat(m_szTipsInfo, szSep);
			_tcscat(m_szTipsInfo, m_pCategInfo[IDX_CATEG_PBG_E].szText);
			break;
		}*/
		break;
	default:
		break;
	}
	
	return m_szTipsInfo;
}
