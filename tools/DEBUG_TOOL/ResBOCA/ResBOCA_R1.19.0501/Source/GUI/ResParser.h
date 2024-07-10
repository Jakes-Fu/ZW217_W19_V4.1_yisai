// ResParser.h: interface for the CResParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESPARSER_H__C6A36591_D4F4_457E_A2C1_3E60873EEA87__INCLUDED_)
#define AFX_RESPARSER_H__C6A36591_D4F4_457E_A2C1_3E60873EEA87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(push,3)
#include <vector>
#include <algorithm>
#include <afxtempl.h>
#pragma warning(pop)

#include "IResConverter.h"
#include "IBZip.h"
#include "SpreadSheetET.h"
#include "SpreadSheetBE.h"
#include "StrSet.h"

#include "GlobalDef.h"


#define  CONV_SHORT(w)    (MAKEWORD(HIBYTE(w),LOBYTE(w)))


#define  CONV_DWORD(dw)   (MAKELONG(MAKEWORD(HIBYTE(HIWORD(dw)),LOBYTE(HIWORD(dw))),\
                                    MAKEWORD(HIBYTE(LOWORD(dw)),LOBYTE(LOWORD(dw)))))

#define ALIGN4_SIZE(d)  ((DWORD)((DWORD)(d)+3) / 4 * 4)

#define SAFE_DEL(p) \
do \
{\
    if (p != NULL) \
	{\
       delete []p; \
	   p=NULL;\
	}\
} while(0) \

typedef struct _REPL_ITEM_INFO_T
{
	DWORD dwOffset;
	DWORD dwSize;
	LPBYTE pBuf;	
	WORD   wWidth;
	WORD   wHeight;			
	WORD   wLeft;
	WORD   wTop;
	WORD   wRight;
	WORD   wBottom;
	WORD   wItemType;
	WORD   wCmpType;
	DWORD  dwFrame;
	DWORD  dwColor;
	TCHAR  szFile[_MAX_PATH];
}REPL_ITEM_INFO_T;

typedef struct _ITEM_INFO_T{
	REPL_ITEM_INFO_T org;
	REPL_ITEM_INFO_T rep;
	WORD  wExist;
	BOOL  wModified;
	BOOL  bNotUsed;
}ITEM_INFO_T;


typedef struct _RES_ITEM
{
	BOOL  isModified;
	UINT  uResType;	
	UINT  uModuleID;
	UINT  uResIndex;
	TCHAR szDesc[256];
	TCHAR szNewDesc[256];
	BOOL  bLogo;
	ITEM_INFO_T itemInfo[32];

	_RES_ITEM()
	{
		memset(this,0,sizeof(_RES_ITEM));
	}

	void Clear()
	{
		for(int i = 0 ;i< 32; i++)
		{
			SAFE_DEL(itemInfo[i].org.pBuf);
			SAFE_DEL(itemInfo[i].rep.pBuf);
		}
	}

}RES_ITEM, *RES_ITEM_PTR;

typedef struct _ITEM_STAT_T
{
	UINT nItemIndex;
	UINT nThemeIndex;
	int  nDeltSize;
}ITEM_STAT_T;

// #define MAX_DESC_LEN 36
// typedef struct _REPL_T
// {
// 	BYTE bResType;
// 	BYTE bMduIndex;
// 	BYTE bCmpType;
// 	BYTE reserved2;
// 	DWORD dwResIndex;
// 	DWORD dwReserved;
// 	char szDesc[MAX_DESC_LEN];
// }REPL_T, *REPL_PTR;

typedef struct _CHANGE_SEL_DATA
{
    HBITMAP hBmp;
    BOOL    bFreshBg;
	_TCHAR  szProperty[1024];
	MMI_RES_TYPE_E eResType;
	_TCHAR  szImgPath[_MAX_PATH];	
	_CHANGE_SEL_DATA()
	{
		memset(this,0,sizeof(_CHANGE_SEL_DATA));
	}
} CHANGE_SEL_DATA;

typedef struct _IM_ITEM
{
	BOOL bNotUsed;
	int  iIMOrgSize;
	int  iIMRepSize;
	int  iCount;
	int  index[30];
}IM_ITEM,*IM_ITEM_PTR;


typedef CMap<CString, LPCTSTR, DWORD, DWORD> COLOR_MAP;
typedef CMap<CString, LPCTSTR, IM_ITEM,IM_ITEM> IM_MAP;
typedef CMap< int, int, CString, LPCTSTR> IMITEM_MAP;

typedef CStrSet* CStrSet_Ptr;

class CResParser  
{
public:
	CResParser();
	virtual ~CResParser();
    
	BOOL LoadResFile(LPCTSTR lpszFile);
	BOOL SaveResFile(LPCTSTR lpszFile);

	BOOL LoadThmFolder(LPCTSTR lpszFolder);
	BOOL ExportThmFolder(LPCTSTR lpszFolder);
	
	BOOL ImportString(LPCTSTR lpszFile);
	BOOL ExportString(LPCTSTR lpszFile);

	CString GetResItemInfo(int nItemIndex,int nThemeIndex = 0);
	CString GetResInfo();
	CString GetResTypeString(UINT uResType);

	BOOL RelaceImag(int nItemIndex,int nThemeIndex, LPCTSTR lpszImagFile);
	BOOL RelaceAnim(int nItemIndex,int nThemeIndex, LPCTSTR lpszAnimFolder);
	BOOL RelaceRing(int nItemIndex,int nThemeIndex, LPCTSTR lpszRingFile);
	BOOL RelaceColor(int nItemIndex,int nThemeIndex, DWORD dwColor);
	BOOL RelaceData(int nItemIndex,int nThemeIndex, LPCTSTR lpszDataFile);
	BOOL IsModified();
	BOOL IsOverMaxSize();
	BOOL IsReady(){return m_vResItem.size()>0?TRUE:FALSE;}
	BOOL IsStrExported(){return m_vStrSet.size()>0?TRUE:FALSE;}

	BOOL UndoResItem(int nItemIndex,int nThemeIndex);	

	void GetResItemStat(std::vector<ITEM_STAT_T> &vItemStat);

	void SetCmdParam(DWORD dwMaxSize) { m_bCmd = TRUE; m_dwCmdMaxSize = dwMaxSize;}


	void ModifyDataLDB(); // not used

	void ModifyLang();
	void ModifyIMLdb(LPCTSTR lpszIM,BOOL bDelete);
	void ModifyIM();

	BOOL DeleteResItem(int nItemIndex,int nThemeIndex = 0);
	CString GetIMLang(int nItemIndex,int nThemeIndex);
	
	BOOL TestStrNoFlag(LPBYTE pResBuf);

public:
	std::vector<RES_ITEM_PTR> m_vResItem;
	
	BOOL  m_bBigEndian;
	UINT  m_uThemeNum;
	CString m_strResFile;
	UINT  m_uExtTblNum;
	BOOL  m_bThemeOnly;
	CStringArray m_agThemeName;
	CString      m_strTmpFolder;
	
	DWORD m_dwMaxResSize;
	BOOL  m_bCheckImagWH;
	BOOL  m_bCheckAnimWH;
	BOOL  m_bCheckAnimFrame;
	BOOL  m_bCheckRingType;
	BOOL  m_bShowWarning;

	int          m_nExcelParserType;

	CStringArray m_agIMName;
	IM_MAP       m_mapIM;
	
	CString      m_strErrMsg;
	
	HWND         m_hWnd;

private:
    void Clear();
	BOOL LoadResInfo();
	BOOL LoadLogo(DWORD dwOffset, DWORD dwSize,int nIndex);
	BOOL LoadRepl(DWORD dwOffset, DWORD dwSize);
	BOOL LoadResItem(RES_ITEM_PTR pri);
	BOOL LoadImage(RES_ITEM_PTR pri);
	BOOL LoadAnim(RES_ITEM_PTR pri);
	BOOL LoadRing(RES_ITEM_PTR pri);
	BOOL LoadData(RES_ITEM_PTR pri);
	BOOL LoadColor(RES_ITEM_PTR pri);
	BOOL LoadCscs(DWORD dwOffset, DWORD dwSize);
	BOOL LoadLang(DWORD dwOffset, DWORD dwSize);

	BOOL LoadColorFromFile(LPCTSTR lpszFile, COLOR_MAP &mapColor );
	BOOL ExportColorToFile(LPCTSTR lpszFile);
	BOOL ExportImgInfoToFile(LPCTSTR lpszFile);

	BOOL SaveResItem(int nItemIndex,int nThemeIndex);
	BOOL SaveImage(int nItemIndex,int nThemeIndex);
	BOOL SaveAnim(int nItemIndex,int nThemeIndex);
	BOOL SaveRing(int nItemIndex,int nThemeIndex);
	BOOL SaveData(int nItemIndex,int nThemeIndex);
	BOOL SaveColor(int nItemIndex,int nThemeIndex);
	BOOL SaveLogo(int nItemIndex);

	BOOL SaveResInfo();
	BOOL SaveLang(DWORD dwOffset, DWORD dwSize);
	BOOL SaveCscs(DWORD dwOffset, DWORD dwSize,DWORD* pCscs);

	BOOL SaveString(int nThemeIndex);

	void ModifyImagOffset(LPBYTE pMduBaseBuf,int nDeltAlign, UINT nPos);
	void ModifyAnimOffset(LPBYTE pMduBaseBuf,int nDeltAlign, UINT nPos);
	void ModifyRingOffset(LPBYTE pMduBaseBuf,int nDeltAlign, UINT nPos);
	void ModifyFontOffset(LPBYTE pMduBaseBuf,int nDeltAlign, UINT nPos);
	void ModifyTextOffset(LPBYTE pMduBaseBuf,int nDeltAlign, UINT nPos);
	void ModifyDataOffset(LPBYTE pMduBaseBuf,int nDeltAlign, UINT nPos);

	void ModifyTextOffset2(LPBYTE pMduBaseBuf,int nDeltAlign, UINT nLangID,DWORD dwEnStart,DWORD dwEnEnd);

	void ModifyTablOffset(int nDeltAlign, UINT nPos);

	LPBYTE MidifyHdrOffset(int nDeltAlign, int nThemeIndex, RES_ITEM_PTR pRI);
	
	CString GetImgTypeString(UINT uImgType);
    CString GetRingTypeString(UINT uRingType);
	CString GetImgCmpTypeString(UINT uImgCmpType);
	CString GetAnimCmpTypeString(UINT uAnimCmpType);
	CString GetImgFileExtString(UINT uImgCmpType,UINT uImgType);

	void GetResStat(UINT &nItemCount,UINT &nItemReplaced);

	LPCTSTR PreOpenResFile(LPCTSTR lpszFile);
	BOOL    PostSaveResFile();

	void SplitLine(char *pLine, CStringArray &agItem);

	BOOL  EmptyLangText(int nItemIndex,int nThemeIndex);
	BOOL  EmptyMduLangText(int nThemeIndex,RES_ITEM_PTR pRI);

	DWORD GetMduLangTextSize(LPBYTE pResBuf,int nMduID,int nLangID, DWORD &dwEnDataStart,DWORD &dwEnDataEnd,DWORD &dwOffsetStart);

	DWORD GetLangTextSize(LPBYTE pResBuf,int nLangID);

	int   GetRingFiles(LPCTSTR lpszCSCFolder,CStringArray &agFiles,CStringArray &agDesc,CUIntArray &agIndex,CUIntArray &agUnusedcIndex);

	void  GetIMInfo();

	void  CalcModifedName();

	void * CreateSpreadSheet();

	BOOL  ExpMduString(CSpreadSheet *pSpread,LPBYTE pResBuf,int nMduID,CStrSet_Ptr pStrSet);
	BOOL  ImpMduString(int nMduID,CStrSet_Ptr pStrSet);
	BOOL  SaveMduString(int nMduID,CStrSet_Ptr pStrSet);

	int   GetLangNames(CStringArray &agNames);
	int   GetLangCount();

	BOOL  LoadStrTable(LPCTSTR lpszFile,CStringArray &agLangNames);
	void  ClearStrTable();

	int   GetMduInfo(LPBYTE pResBuf,CUIntArray &agMduIDIdx);

	LPBYTE GetMduBase(LPBYTE pResBuf,int nMudID,int nThemeID);

	BOOL  IsStringModified();

private:
	BYTE * m_pResBuf;
	DWORD m_dwResSize;

	BYTE * m_pNewBuf;
	int    m_nNewSize;

	DWORD  m_dwTextOrgSize;
	DWORD  m_dwTextRepSize;

	DWORD  m_dwLDBOrgSize;
	DWORD  m_dwLDBRepSize;

	DWORD  m_dwCurTotalSize;

//	BYTE * m_pTmpBuf;

	IResConverter * m_pRC;

	IBZip         * m_pBzp;
	BOOL            m_bBzp;
	CStringArray    m_agBzpFiles;
	CString         m_strBzpFile;

	BOOL            m_bCmd;
	DWORD           m_dwCmdMaxSize;
	BOOL            m_bCscs;
	//BOOL            m_bAllowEmpty;

	BOOL            m_bLoadThmFolder;


	BOOL            m_bReadOnce;

	STRING_MAP      m_mapString;
	std::vector<CStrSet_Ptr> m_vStrSet;
	BOOL            m_bRmCrossRepeat;
	BOOL			m_bAllUnicode;
	BOOL			m_bStrNoFlag;

};

#endif // !defined(AFX_RESPARSER_H__C6A36591_D4F4_457E_A2C1_3E60873EEA87__INCLUDED_)
