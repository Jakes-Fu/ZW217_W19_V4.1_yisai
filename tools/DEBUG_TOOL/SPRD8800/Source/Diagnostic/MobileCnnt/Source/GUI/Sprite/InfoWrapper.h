// InfoWrapper.h: interface for the CInfoWrapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOWRAPPER_H__F3F34871_F360_4192_9510_E826AB7FD347__INCLUDED_)
#define AFX_INFOWRAPPER_H__F3F34871_F360_4192_9510_E826AB7FD347__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __GLOBALDEF_H__
#include "GlobalDef.h"
#endif

#pragma warning(push,3)
#include <vector>
#include <algorithm>
#pragma warning(pop)

const int MAX_CATEG_TEXT_LEN     = 32;

typedef struct _CATEG_INFO
{
	_CATEG_INFO() { memset( this, 0, sizeof(_CATEG_INFO) ); }
	
	int    nImage;
	int    nSelectedImage;
	DWORD  dwData;
	_TCHAR szText[ MAX_CATEG_TEXT_LEN ];
	
	_CATEG_INFO * pNext;
	_CATEG_INFO * pChild;    
} CATEGINFO, * PCATEGINFO;

typedef struct _PRESET_SMS
{
	_PRESET_SMS() { memset( this, 0, sizeof(_PRESET_SMS) ); }

	_PRESET_SMS * pPrev;
	_PRESET_SMS * pNext;

	_TCHAR szCaption[MAX_SMS_CAPTION_LEN + 1];
	_TCHAR szContent[MAX_SMS_LEN + 1];
	// 注意：如果需要增加新条目，请不要改变上面的顺序，新增部分放下面
} PRESETSMS, * PPRESETSMS;

class CInfoWrapper  
{
public:
	BOOL Init(void);
	void Refresh(void);
	BOOL UpdateCategPBG(void);
	
	int GetColumnCount( DWORD dwFlag );
	LPCTSTR GetColumn(DWORD dwFlag, int nIdx);
	int GetColumnWidth(DWORD dwFlag, int nIdx);

	void Release(void);

	LPCTSTR GetTipsInfo(DWORD dwFlag);

	////////// 预设短信息 /////////////////////////
	int GetPresetSMSCount();
	PPRESETSMS GetPresetSMS( int nIdx );
	void AddPresetSMS( PPRESETSMS pSMS );
	void DelPresetSMS( int nIdx );
	void SetCurSelPresetSMS(int nCurSel );
	PPRESETSMS GetCurSelPresetSMS(void);
	BOOL IsPresetSMSExist(LPCTSTR pszCaption);
	void RemoveAllPresetSMS(void);
	// SMS
	int  GetSMSCount(WORD wStore, BOOL bLongSms = TRUE);
	void  SetSMSTotalCount(WORD wStore, int nTotalCount, int nUsedCount);
	PSMS GetSMS( int nIdx );
	void AddSMS( PSMS pSMS );
	void RemoveSMS( int nIdx );
	void RemoveSMS( PSMS psms );
	void RemovePhoneSIMSMS(void);
	void RemoveAllSMS(void);

	// pb
	int  GetPBCount(WORD wStore);
	PPB  GetPB( int nIdx );
	void AddPB( PPB pPB );
	void SetCurSelPB(int nIdx);
	PPB  GetCurSelPB(void);
	void RemovePB( int nIdx );
	void RemovePB( PPB ppb );
	void RemovePhoneSIMPB(void);
	void RemoveAllPB(void);

	// MEMO
	int   GetMEMOCount(WORD wStore);
	PMEMO GetMEMO( int nIdx ); 
	void  AddMEMO( PMEMO pMEMO );
	void  RemoveMEMO( int nIdx );
	void  RemoveMEMO( PMEMO pMEMO );
	void  RemovePhoneMEMO(void);
	void  RemoveAllMEMO(void);
	CString GetMemoMode(int nIdx);
	CString GetMemoFre(BYTE fre);

/*	// SCHDL
	int    GetSCHDLCount();
	PSCHDL GetSCHDL( int nIdx );
	PSCHDL GetFirstUnusedSCHDLInPhone( void );
	void   AddSCHDL( PSCHDL pSCHDL );
	void   RemoveSCHDL( int nIdx );
	void   RemoveSCHDL( PSCHDL pSCHDL );
	void   RemovePhoneSCHDL(void);
	void   RemoveAllSCHDL(void);*/

	// PBG
	int      GetPBGCount(WORD wStore);
	int      GetPBGMemoberCount(int nIdx);
	PPBGROUP GetPBG( int nIdx );
	void     AddPBG( PPBGROUP pPBG );
	void     SetCurSelPBG(int nIdx);
	PPBGROUP GetCurSelPBG(void);
	void     RemovePBG( int nIdx );
	void     RemovePBG( PPBGROUP pPBG );
	void     RemovePhonePBG(void);
	void     RemoveAllPBG(void);
	PPBGROUP GetPBGByIdInPhone( UINT nID );
	int      GetPBGIndex(UINT nPbgID);
	int      GetPBGIndexByName(LPCTSTR lpszName);

public:
	PCATEGINFO m_pCategInfo;
	BOOL       m_bSyncDlg;
	int		   m_nTotalSmsCount;
	int		   m_nUsedSmsCount;	
	
public:
	
	CInfoWrapper();
	~CInfoWrapper();
protected:
	BOOL MakeID(void);
	void InitColumns(HINSTANCE hRes);

private:
	int   * m_pArrImg;
	DWORD * m_pArrID;
	DWORD   m_dwCategItem;
	enum{
		MAX_SMS_COLUMN   = 6,
		MAX_PB_COLUMN    = 8,
		MAX_MEMO_COLUMN  = 7,
		MAX_SCHDL_COLUMN = 4,
		MAX_PBG_COLUMN   = 3,
		MAX_PBGM_COLUMN  = 7,
		MAX_TIPSINFO_LEN = 128,
	};
	_TCHAR m_arrSMSClmn[MAX_SMS_COLUMN][MAX_CATEG_TEXT_LEN];
	int    m_arrSMSClmnWidth[MAX_SMS_COLUMN];
	_TCHAR m_arrPBClmn[MAX_PB_COLUMN][MAX_CATEG_TEXT_LEN];
	int    m_arrPBClmnWidth[MAX_PB_COLUMN];
	_TCHAR m_arrMEMOClmn[MAX_MEMO_COLUMN][MAX_CATEG_TEXT_LEN];
	int    m_arrMEMOClmnWidth[MAX_MEMO_COLUMN];
	//_TCHAR m_arrSCHDLClmn[MAX_SCHDL_COLUMN][MAX_CATEG_TEXT_LEN];
	//int    m_arrSCHDLClmnWidth[MAX_SCHDL_COLUMN];
	_TCHAR m_arrPBGClmn[MAX_PBG_COLUMN][MAX_CATEG_TEXT_LEN];
	int    m_arrPBGClmnWidth[MAX_PBG_COLUMN];

	_TCHAR m_arrPBGMClmn[MAX_PBGM_COLUMN][MAX_CATEG_TEXT_LEN];
	int    m_arrPBGMClmnWidth[MAX_PBGM_COLUMN];
	
	_TCHAR m_szTipsInfo[MAX_TIPSINFO_LEN];

	////////// 预设短信息 /////////////////////////
    CTypedPtrArray<CPtrArray, PPRESETSMS>m_arrPresetSMS;
	int m_nCurSelPresetSMS;
	// SMS
	CTypedPtrArray<CPtrArray, PSMS>m_arrSMS;
	// PB
	//CTypedPtrArray<CPtrArray, PPB>m_arrPB;
	std::vector<PPB> m_arrPB;
	int m_nCurSelPB;
	// MEMO
	CTypedPtrArray<CPtrArray, PMEMO>m_arrMEMO;
	// SCHDL
	CTypedPtrArray<CPtrArray, PSCHDL>m_arrSCHDL;
	// PBG
	CTypedPtrArray<CPtrArray, PPBGROUP>m_arrPBG;
	int m_nCurSelPBG;

	CStringArray     m_arrMemoMode;
	CStringArray     m_arrMemoFre;

	static const int IDX_CATEG_SMS;
	static const int IDX_CATEG_PB;
	static const int IDX_CATEG_MEMO;
	static const int IDX_CATEG_SCHDL;
	static const int IDX_CATEG_PBG;

	static const int IDX_CATEG_SMS_PC;
	static const int IDX_CATEG_SMS_INBOX;
	static const int IDX_CATEG_SMS_OUTBOX;
	static const int IDX_CATEG_SMS_SENDS;
	static const int IDX_CATEG_SMS_DRAFTS;

	static const int IDX_CATEG_PB_PC;
	static const int IDX_CATEG_PB_PHONE;
	static const int IDX_CATEG_PB_SIM;
	static const int IDX_CATEG_PB_SIM2;
	static const int IDX_CATEG_PB_SIM3;
	static const int IDX_CATEG_PB_SIM4;
	
	static const int IDX_CATEG_MEMO_PC;
	static const int IDX_CATEG_MEMO_PHONE;

	static const int IDX_CATEG_SCHDL_PC;
	static const int IDX_CATEG_SCHDL_PHONE;
	
	static const int IDX_CATEG_PBGM[MAX_GROUP_NUM];

	

	static const int MAX_CATEG_ITEM_COUNT;
};

#endif // !defined(AFX_INFOWRAPPER_H__F3F34871_F360_4192_9510_E826AB7FD347__INCLUDED_)
