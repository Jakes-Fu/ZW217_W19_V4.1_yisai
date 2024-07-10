#ifndef _GLOBALSTRUCT_H__
#define _GLOBALSTRUCT_H__

#pragma warning(push,3)
#include  <afxtempl.h>
#pragma warning(pop)

#include "mmi_resource_def.h"
#include "mmi_text_def.h"
#include "img_def.h"
#include "anim_def.h"

const UINT X_PL_D  	= 0;  //　D platform 
const UINT X_PL_DM  = 1;  //  D platform modularized
const UINT X_PL_M  	= 2;  //  M platform and ther platform

const WORD IMG_TYPE_NORMAL	= 0; // BMP
const WORD IMG_TYPE_OSD		= 1; // OSD

//const UINT IMG_CMP_NO		= 0; // no compress
//const UINT IMG_CMP_BMP		= 1; // compress by bmp CRL
//const UINT IMG_CMP_OSD		= 2; // compress by bmp CRL
//const UINT IMG_CMP_JPEG		= 3; // compress by JPEG
//const UINT IMG_CMP_JPEG_SCI	= 4; // compress by JPEG

//const UINT ANIM_CMP_MBMP		= IMG_CMP_BMP; // compress by motion bmp
//const UINT ANIM_CMP_MJPEG		= IMG_CMP_JPEG; // compress by motion JPEG

const UINT TN_NO_STATE  =0;
const UINT TN_NOT_SEL	=1;
const UINT TN_PART_SEL	=2;
const UINT TN_ALL_SEL	=3;

const UINT MAX_PRD_NUM = 32;

const UINT DEF_JEPG_QUALITY = 75;
const UINT DEF_MJPEG_RATE = 24;

const _TCHAR SZ_REP_FLAG[] = _T("_##");
const _TCHAR SZ_DIR_REP_FLAG[] = _T("##");


#define	RES_TAG_REPLACE  (0x1)

typedef enum {
	//	XNT_VERSION,
	XNT_PRODUCT,
	XNT_SKIN,
	XNT_TOOL_USED,
	XNT_LANG,
	XNT_IMAG,
	XNT_ANIM,
	XNT_RING,
	XNT_TEXT,
	XNT_FONT,
	XNT_OTHR,
	XNT_COLOR,
	XNT_RECT,
	XNT_STRING,
	XNT_TAGS,
	XNT_CSC
}E_XRM_NODE_TYPE;

typedef struct _PRD_SKN_INFO_T
{
	E_XRM_NODE_TYPE eType; // SKIN or PROJECT
	_TCHAR          szName[_MAX_PATH]; // Product or Skin name 
	_TCHAR          szVersion[_MAX_PATH]; // Skin Version
	DWORD           dwSkinNameId;
	DWORD           dwIndex;
	DWORD           dwCheckState;
}PRD_SKN_INFO,*PRD_SKN_INFO_PTR;

typedef struct _XRM_PARSE_INFO_T
{
	E_XRM_NODE_TYPE eParentType; // SKIN or PROJECT
	E_XRM_NODE_TYPE eType;
	LPVOID			pData; 
	LPVOID		   *ppData;	// 
	DWORD			nCount;	// Number of IMAG,ANIM,RING,TEXT,FONT or OTHER resource items
	DWORD           nIndex; // Product or Skin index depended on eType	
}XRM_PARSE_INFO,*XRM_PARSE_INFO_PTR;


//JPEG SIZE LIMIT
typedef struct _JPEG_SIZE_LIMIT_T
{
	BOOL bEnable;
	int  nHeigthBase;
	int  nWidthBase;
	int  nHeightMin;
	int  nWidthMin;
	_JPEG_SIZE_LIMIT_T()
	{
		bEnable = TRUE;
		nHeightMin = 0;
		nWidthMin = 0;
		nHeigthBase = 8;
		nWidthBase = 16;
	}
}JPEG_SIZE_LIMIT_T,*JPEG_SIZE_LIMIT_PTR;

//Tool
typedef struct _X_TOOL_INFO_T
{
	WORD wMajorVersion; 
    WORD wMinorVersion; 
	WORD wBuildNumber; 
	WORD  bAddrIsOffset;
    WORD  bBigEndian;
	DWORD dwStartAddress;   // MMI资源的起始地址
	DWORD dwMaxLimitSize;   // 整个MMI资源的最大限制
	DWORD dwUsedSize;       // 当前已经使用的空间
	_TCHAR  szReserved[MAX_PATH+1]; // 其他信息

	_X_TOOL_INFO_T()
	{
		ZeroMemory(this,sizeof(_X_TOOL_INFO_T));
	}

	CString ToString()
	{
		CString str;
		str.Format(_T("%d\t%d\t%d\t%d\r\n%d\t0x%X\t%d\t%d\r\n%s\r\n"),
			wMajorVersion,wMinorVersion,wBuildNumber,bAddrIsOffset,
			bBigEndian,dwStartAddress,dwMaxLimitSize,dwUsedSize,
			_tcslen(szReserved)==0?_T("NULL"):szReserved);
		return str;
	}
} X_TOOL_INFO_T, * PX_TOOL_INFO_T;

//XRM
typedef struct _X_RESOURCE_MODULE
{
	int  	nCheckState;
	_TCHAR	szName[_MAX_PATH+1];
	PRD_SKN_INFO agPrdSknInfo[MAX_PRD_NUM];
	int     nPrdSkinCount;	
	
	_X_RESOURCE_MODULE()
	{
		ZeroMemory(this,sizeof(_X_RESOURCE_MODULE));
	}

}X_RESOURCE_MODULE,*X_RESOURCE_MODULE_PTR;

typedef CMap<CString,LPCTSTR,X_RESOURCE_MODULE_PTR,X_RESOURCE_MODULE_PTR> MAP_XRM;

//Language
typedef struct _X_LANG_ITEM_T
{
	_TCHAR szName[MAX_PATH+1];
	_TCHAR szReserved[MAX_PATH+1];	
	DWORD  dwIndex;	
	DWORD  dwCheckFlag; // 0: not check; 1: check
	DWORD  dwCodeNum; // not used

	_X_LANG_ITEM_T()
	{
		ZeroMemory(this,sizeof(_X_LANG_ITEM_T));
	}

	CString ToString()
	{
		CString str;
		str.Format(_T("%s\r\n%s\r\n%d\r\n%d\r\n"),szName,
			_tcslen(szReserved)==0?_T("NULL"):szReserved,
			dwIndex,dwCheckFlag);
		return str;
	}
}X_LANG_ITEM_T, *PX_LANG_ITEM_T;

typedef struct _X_LANG_INFO_T
{
	X_LANG_ITEM_T agLangItem[MAX_LANG_NUM];
	DWORD dwLangUsedNum;
	DWORD dwEnglishIndex; //not used 
	_TCHAR szReserved[MAX_PATH+1];
	
	_X_LANG_INFO_T()
	{
		ZeroMemory(this,sizeof(_X_LANG_INFO_T));
	}
}X_LANG_INFO_T, *PX_LANG_INFO_T;

//IMAG
// 注意: dwRawSize记录的不一定是原始图像文件的大小。
// 比如说我们导入一副24位的图片，由于mmi目前只支持
// 16位的图片，程序内部会将它转化为16位，dwRawSize
// 记录的是转化成16位后的图片大小，而不是原始24位图
// 片的大小
typedef struct _X_IMAG_T
{
	_TCHAR szID[MAX_PATH+1];
	_TCHAR szURL[MAX_PATH+1];
	_TCHAR szReserved[MAX_PATH+1];
	_TCHAR szComment[MAX_PATH+1];
	WORD   wType;
	WORD   wCompressed;
	WORD   wWidth;
	WORD   wHeight;
	DWORD  dwRawSize;
	DWORD  dwResSize;
	LPBYTE pRawStream;
	LPBYTE pResStream;
	DWORD  dwIsChecked;
	DWORD  dwQuality;
	DWORD  dwUsrParam;
	DWORD  dwIsExist;
	DWORD  dwIsDithering;
	DWORD  dwSambFlag;

	_X_IMAG_T()
	{
		ZeroMemory(this,sizeof(_X_IMAG_T));
	}

	CString ToString()
	{
		CString str;
		str.Format(_T("%s\r\n%s\r\n%s\r\n%d\t%d\t%d\t%d\r\n%d\t%d\r\n"),szID,szURL,
			_tcslen(szReserved)==0?_T("NULL"):szReserved,
			wType,wCompressed,wWidth,wHeight,dwRawSize,dwResSize);
		return str;
	}	
}X_IMAG_T, *PX_IMAG_T;

//ANIM
typedef struct _X_ANIM_T
{
	_TCHAR szID[MAX_PATH+1];
	_TCHAR szURL[MAX_PATH+1];
	_TCHAR szReserved[MAX_PATH+1];
	_TCHAR szComment[MAX_PATH+1];
	WORD   wType;
	WORD   wFrameNum;
	WORD   wWidth;
	WORD   wHeight;
	DWORD  dwKeyColor;
	DWORD  dwRawSize;
	DWORD  dwResSize;
	LPBYTE pRawStream;
	LPBYTE pResStream;
	DWORD  dwIsChecked;
	DWORD  dwCmpType;
	DWORD  dwQuality;
	DWORD  dwRate;
	DWORD  dwIsExist;
	DWORD  dwIsDithering;


	_X_ANIM_T()
	{
		ZeroMemory(this,sizeof(_X_ANIM_T));
	}

	CString ToString()
	{
		CString str;
		str.Format(_T("%s\r\n%s\r\n%s\r\n%d\t%d\t%d\t%d\r\n0x%.6X\t%d\t%d\r\n"),szID,szURL,
			_tcslen(szReserved)==0?_T("NULL"):szReserved,
			wType,wFrameNum,wWidth,wHeight,dwKeyColor,dwRawSize,dwResSize);
		return str;
	}
}X_ANIM_T, *PX_ANIM_T;


//RING
typedef struct _X_RING_T
{
	_TCHAR szID[MAX_PATH+1];
	_TCHAR szURL[MAX_PATH+1];
	_TCHAR szReserved[MAX_PATH+1];
	_TCHAR szComment[MAX_PATH+1];
	WORD   wType;
	DWORD  dwRawSize;
	DWORD  dwResSize;
	LPBYTE pRawStream;
	LPBYTE pResStream;
	DWORD  dwIsChecked;
	DWORD  dwIsExist;

	_X_RING_T()
	{
		ZeroMemory(this,sizeof(_X_RING_T));
	}

	CString ToString()
	{
		CString str;
		str.Format(_T("%s\r\n%s\r\n%s\r\n%d\t%d\t%d\r\n"),szID,szURL,
			_tcslen(szReserved)==0?_T("NULL"):szReserved,
			wType,dwRawSize,dwResSize);
		return str;
	}
}X_RING_T, *PX_RING_T;

//TEXT
typedef struct _X_TEXT_T
{
	_TCHAR szURL[MAX_PATH+1];
	_TCHAR szReserved[MAX_PATH+1];
	DWORD  dwTotalRawSize;
	DWORD  dwTotalResSize;
	BOOL   bHasCheck;
	DWORD  dwIDCount;

	_X_TEXT_T()
	{
		ZeroMemory(this,sizeof(_X_TEXT_T));
	}
	CString ToString()
	{
		CString str;
		str.Format(_T("%s\r\n%s\r\n%d\t%d\r\n%d\r\n"),szURL,
			_tcslen(szReserved)==0?_T("NULL"):szReserved,
			dwTotalRawSize,dwTotalResSize,bHasCheck);
		return str;
	}

}X_TEXT_T, *PX_TEXT_T;
//TEXT ITEM
#define TEXT_SET_BASE_EN_FLAG(w)   ((WORD)((w)|(WORD)0x100))
#define TEXT_SET_REPEAT(w)		   ((WORD)((w)|(WORD)1))

#define TEXT_INDEX_IS_BASE_EN(w)   (((w)&0x100)==0x100)
#define TEXT_IS_REPEAT(w)		   (((w)&1)==1)
typedef struct _X_TEXT_ITEM_T
{
	_TCHAR *pszRawText;
	BYTE   *pbResText;
	WORD   wFlag;     //Unicode 1
	WORD   wLangIndex;
	DWORD  dwRawSize;
	DWORD  dwResSize;	
	WORD   wReserved;
	WORD   wRepeat;   // high byte is 1, mean base on EN; low byte is 1,mean repeat is true.
	DWORD  dwRepeatIndex;
	DWORD  dwOffset;

	_X_TEXT_ITEM_T()
	{
		ZeroMemory(this,sizeof(_X_TEXT_ITEM_T));
	}

}X_TEXT_ITEM_T, *PX_TEXT_ITEM_T;
//TEXT ITEM
typedef struct _X_TEXT_INFO_T
{
	X_TEXT_ITEM_T agText[MAX_LANG_NUM];
	DWORD  dwIsChecked;
	DWORD  dwIsExist;   // not used
	DWORD  dwReserved;
	_TCHAR szID[MAX_PATH+1];
	_X_TEXT_INFO_T()
	{
		ZeroMemory(this,sizeof(_X_TEXT_INFO_T));
	}

}X_TEXT_INFO_T, *PX_TEXT_INFO_T;
//XRM TEXT NODE
typedef struct _X_STR_NODE_T
{
	_TCHAR szID[MAX_PATH+1];
	_TCHAR szDefaultString[MAX_PATH*2];
	_X_STR_NODE_T()
	{
		ZeroMemory(this,sizeof(_X_STR_NODE_T));
	}

}X_STR_NODE_T, *PX_STR_NODE_T;
//FONT Database
typedef struct _X_FONT_DB_T
{
	_TCHAR szURL[MAX_PATH+1];      // arm
	_TCHAR szURLWin[MAX_PATH+1];   // win
	_TCHAR szReserved[MAX_PATH+1]; 
	DWORD  dwLangIndex;	
	DWORD  dwRawSize;			   // win
	DWORD  dwResSize;			   // arm
	LPBYTE pRawStream;			   // win
	LPBYTE pResStream;			   // arm
	DWORD  dwIsChecked;
	
	_X_FONT_DB_T()
	{
		ZeroMemory(this,sizeof(_X_FONT_DB_T));
	}
	CString ToString()
	{
		CString str;
		str.Format(_T("\t\t%s\r\n\t\t%s\r\n\t\t%d\t%d\t%d\r\n"),szURL,
			_tcslen(szURLWin)==0?_T("NULL"):szURLWin,
			_tcslen(szReserved)==0?_T("NULL"):szReserved,
			dwLangIndex,dwRawSize,dwResSize);
		return str;
	}
}X_FONT_DB_T, *PX_FONT_DB_T;

//FONT
typedef struct _X_FONT_T
{
	_TCHAR szID[MAX_PATH+1];
	_TCHAR szType[MAX_PATH+1];
	_TCHAR szReserved[MAX_PATH+1];
	_TCHAR szComment[MAX_PATH+1];
	X_FONT_DB_T agFontDB[MAX_LANG_NUM];
	DWORD  dwUsedDBNum;
	DWORD  dwType;
	DWORD  dwIsChecked;
	DWORD  dwIsExist;

	_X_FONT_T()
	{
		ZeroMemory(this,sizeof(_X_FONT_T));
	}

	CString ToString()
	{
		CString str;
		str.Format(_T("%s\r\n%s\r\n%s\r\n%d\t%d\r\n"),szID,szType,
			_tcslen(szReserved)==0?_T("NULL"):szReserved,dwUsedDBNum,dwType);
		for(int i=0;i<(int)dwUsedDBNum;i++)
		{
			str+=agFontDB[i].ToString();
		}
		return str;
	}
}X_FONT_T, *PX_FONT_T;

//Other
typedef struct _X_OTHER_T
{
	_TCHAR szID[MAX_PATH+1];
	_TCHAR szURL[MAX_PATH+1];
	_TCHAR szReserved[MAX_PATH+1];
	_TCHAR szComment[MAX_PATH+1];
	WORD   wType;
	DWORD  dwRawSize;
	DWORD  dwResSize;
	LPBYTE pRawStream;
	LPBYTE pResStream;
	DWORD  dwIsChecked;
	DWORD  dwIsExist;
	
	_X_OTHER_T()
	{
		ZeroMemory(this,sizeof(_X_OTHER_T));
	}
	
	CString ToString()
	{
		CString str;
		str.Format(_T("%s\r\n%s\r\n%s\r\n%d\t%d\t%d\r\n"),szID,szURL,
			_tcslen(szReserved)==0?_T("NULL"):szReserved,
			wType,dwRawSize,dwResSize);
		return str;
	}
}X_OTHER_T, *PX_OTHER_T;

//COLOR
typedef struct _MMI_COLOR_T
{
	DWORD dwColor;
	BYTE  bValidate;
	BYTE  reserved[3];
	_MMI_COLOR_T()
	{
		memset(this,sizeof(_MMI_COLOR_T),0);
	}

}MMI_COLOR_T,*MMI_COLOR_PTR;
typedef struct _X_COLOR_T
{
	_TCHAR szID[MAX_PATH+1];
	_TCHAR szComment[MAX_PATH+1];
	MMI_COLOR_T  tColor;
	DWORD  dwRawSize;
	DWORD  dwResSize;
	DWORD  dwIsChecked;
	DWORD  dwIsExist;
	
	_X_COLOR_T()
	{
		ZeroMemory(this,sizeof(_X_COLOR_T));
	}
	
	CString ToString()
	{
		CString str;
		str.Format(_T("%s\r\n%s\r\n0x%08X\r\n"),szID,szComment,tColor.dwColor);
		return str;
	}
}X_COLOR_T, *PX_COLOR_T;

//Rect
typedef struct _X_MMI_RECT_T
{
	short   wLeft;
	short   wTop;
	short   wRight;
	short   wBottom;
	BYTE   bValidate;
	BYTE  reserved[3];

	_X_MMI_RECT_T()
	{
		memset(this,sizeof(_X_MMI_RECT_T),0);
	}

}X_MMI_RECT_T,*PX_MMI_RECT_T;

typedef struct _X_RECT_T
{
	_TCHAR szID[MAX_PATH+1];
	_TCHAR szComment[MAX_PATH+1];
	X_MMI_RECT_T rect;
	DWORD  dwRawSize;
	DWORD  dwResSize;
	DWORD  dwIsChecked;
	DWORD  dwIsExist;
	
	_X_RECT_T()
	{
		ZeroMemory(this,sizeof(_X_RECT_T));
	}
	
	CString ToString()
	{
		CString str;
		str.Format(_T("%s\r\n%s\r\n%d\r\n%d\r\n%d\r\n%d\r\n"),szID,szComment,
			rect.wLeft,rect.wTop,rect.wRight,rect.wBottom);
		return str;
	}
}X_RECT_T, *PX_RECT_T;

typedef struct _X_RES_TAG_T
{
	_TCHAR szID[MAX_PATH+1];
	_TCHAR szDesc[MAX_PATH+1];
	MMI_RES_TYPE_E eResType;
	DWORD  dwFlag;
	DWORD  dwModuleID;
	DWORD  dwResIndex;
	DWORD  dwReserved;
	
	_X_RES_TAG_T()
	{
		ZeroMemory(this,sizeof(_X_RES_TAG_T));
	}
	
	CString ToString()
	{
		CString str;
		str.Format(_T("%s\r\n%s\r\n"),szID,szDesc);
		return str;
	}
}X_RES_TAG_T, *PX_RES_TAG_T;

//RESOURCE
typedef struct _X_RESOURCE_INFO_T
{
	LPVOID lpXResT;
	int    nCount;
	MMI_RES_TYPE_E eType;
	
	_X_RESOURCE_INFO_T()
	{
		ZeroMemory(this,sizeof(_X_RESOURCE_INFO_T));
	}
}X_RESOURCE_INFO_T, *PX_RESOURCE_INFO_T;

// IMAG or ANIM param

typedef struct _RES_PARAM_T
{
	UINT	uCmpType;
	UINT	uQuality;
	UINT	uImagType;
	UINT    uRate;
	LPCTSTR lpszFileName;
	LPCTSTR lpszPrd;
	LPCTSTR lpszID;
	LPCTSTR lpszURL;
	BOOL    bCheckSize;
	UINT    uKeyColor;   // used only by animation
	int     nOldSize;    // before replace
	int     nNewSize;    // after replace
	LPVOID  lpResData;   // 
	HBITMAP hBmp;
	MMI_RES_TYPE_E eResType;
	int     nWidth;
	int     nHeight;
	int     nChecktate;   // 0, not check; 1,checked
	int     nItem;
	int     nSubItem;
	CStringArray agString; // only used by text and font
	int     nExist;
	int     nCount; // resource count

	BOOL    bAllUnicode;
	int     nPrecomposedFlag;
	UINT    uMCU; // YUV type for jpeg
	UINT    uQL;  // quality level type for jpeg

	int     nDither; // 0, not dither; 1, dither for image
	UINT    uSabmFlag;

	BOOL    bLogo;

	int     nUserData; // 0~255
	
	
	_RES_PARAM_T()
	{
		uCmpType = 0; 
		uQuality = DEF_JEPG_QUALITY;
		uImagType = IMG_TYPE_NORMAL;
		uRate     = DEF_MJPEG_RATE;
		lpszFileName = NULL;
		lpszPrd      = NULL;
		lpszID       = NULL;
		lpszURL      = NULL;
		bCheckSize   = FALSE;
		uKeyColor    = 0; 
		nOldSize     = 0;
		nNewSize     = 0;
		eResType     = MMI_RT_IMAGE;
		lpResData    = NULL;
		hBmp         = NULL;
		nWidth       = 0;
		nHeight      = 0;
		nChecktate   = 0;
		nItem        = -1;
		nSubItem     = 0;
		nExist       = 0;
		nCount       = 0;
		bAllUnicode  = FALSE;
		nPrecomposedFlag = 0; // 混合型
		agString.RemoveAll();
		uMCU = 1;
		uQL  = 4;
		nDither = 0;
		uSabmFlag = SABM_TILE;
		bLogo = FALSE;
		nUserData= 0;
	}

	_RES_PARAM_T(_RES_PARAM_T * prpt)
	{

		uCmpType		= prpt->uCmpType;		
		uQuality		= prpt->uQuality;		
		uImagType		= prpt->uImagType;		
		uRate			= prpt->uRate;		
		lpszFileName	= prpt->lpszFileName;	
		lpszPrd			= prpt->lpszPrd;
		lpszID          = prpt->lpszID;
		lpszURL         = prpt->lpszURL;
		bCheckSize		= prpt->bCheckSize;		
		uKeyColor		= prpt->uKeyColor;	
		nOldSize		= prpt->nOldSize;		
		nNewSize		= prpt->nNewSize;
		eResType        = prpt->eResType;
		lpResData       = prpt->lpResData;
		hBmp            = prpt->hBmp;
		nWidth          = prpt->nWidth;
		nHeight         = prpt->nHeight;
		nChecktate      = prpt->nChecktate;
		nItem			= prpt->nItem;
		nSubItem        = prpt->nSubItem;
		nExist          = prpt->nExist;
		nCount          = prpt->nCount;
		bAllUnicode     = prpt->bAllUnicode;
		nPrecomposedFlag= prpt->nPrecomposedFlag;
		agString.Copy(prpt->agString);
		uMCU            = prpt->uMCU;
		uQL             = prpt->uQL;
		nDither         = prpt->nDither;
		uSabmFlag       = prpt->uSabmFlag;
		bLogo           = prpt->bLogo;
		nUserData       = prpt->nUserData;
	}

	_RES_PARAM_T & operator=( const _RES_PARAM_T & rpt )
    {
        if( this == &rpt )
            return *this;
		
		uCmpType		= rpt.uCmpType;		
		uQuality		= rpt.uQuality;		
		uImagType		= rpt.uImagType;		
		uRate			= rpt.uRate;		
		lpszFileName	= rpt.lpszFileName;	
		lpszPrd			= rpt.lpszPrd;	
		lpszID          = rpt.lpszID;
		lpszURL         = rpt.lpszURL;
		bCheckSize		= rpt.bCheckSize;		
		uKeyColor		= rpt.uKeyColor;	
		nOldSize		= rpt.nOldSize;		
		nNewSize		= rpt.nNewSize;
		eResType        = rpt.eResType;
		lpResData       = rpt.lpResData;
		hBmp            = rpt.hBmp;
		nWidth          = rpt.nWidth;
		nHeight         = rpt.nHeight;
		nChecktate      = rpt.nChecktate;
		nItem			= rpt.nItem;
		nSubItem        = rpt.nSubItem;
		nExist          = rpt.nExist;
		nCount          = rpt.nCount;
		bAllUnicode     = rpt.bAllUnicode;
		nPrecomposedFlag= rpt.nPrecomposedFlag;
		agString.Copy(rpt.agString);
		uMCU            = rpt.uMCU;
		uQL             = rpt.uQL;
		nDither         = rpt.nDither;
		uSabmFlag       = rpt.uSabmFlag;
		bLogo           = rpt.bLogo;
		nUserData       = rpt.nUserData;

        return *this;
    }

}RES_PARAM_T,*RES_PARAM_PTR;

typedef struct _EXT_TABLE_INFO_T
{
	BYTE tags[4];
	DWORD dwOffset;
	DWORD dwSize;
	_EXT_TABLE_INFO_T()
	{
		memset(this,0,sizeof(_EXT_TABLE_INFO_T));
	}
}EXT_TABLE_INFO_T,*EXT_TABLE_INFO_PTR;

#define MAX_DESC_LEN 36

typedef struct _MMI_TAG_T
{
	BYTE bResType;
	BYTE bMduIndex;
	BYTE reserved1;
	BYTE reserved2;
	DWORD dwResIndex;
	DWORD dwReserved;
	char szDesc[MAX_DESC_LEN];
}MMI_TAGS_T, *MMI_TAG_PTR;

#define MAX_TAG_DESC_LEN 256

typedef struct _MMI_TAG2_T
{
	BYTE bResType;
	BYTE bMduIndex;
	BYTE bNotUsed;
	BYTE reserved2;
	DWORD dwResIndex;
	DWORD dwReserved;
	char szDesc[MAX_TAG_DESC_LEN];
}MMI_TAG2_T, *MMI_TAG2_PTR;

#define MAX_LANG_DESC_LEN 20

typedef struct _MMI_LANG_TAG_T
{
	WORD wLangIndex;
	WORD wReserved;
	char szDesc[MAX_LANG_DESC_LEN];
}MMI_LANG_TAG_T, *MMI_LANG_TAG_PTR;



//for error
#define X_FALSE                         ((LONG)0xFFFFFFFF)
#define X_E_CREATE_DOMDOCUMENT			((LONG)0x8000F001)
#define X_E_SET_DOM_VALIDATE_PARSE		((LONG)0x8000F002)

#define X_E_FILE_NOT_EXIST              ((LONG)0x8000F003)
#define X_E_FILE_TOO_SMALL              ((LONG)0x8000F004)
#define X_E_OUT_OF_MEMORY				((LONG)0x8000F005)
#define X_E_READ_FILE   				((LONG)0x8000F006)

#define X_E_NOT_BMP_TYPE				((LONG)0x8000F007)
#define X_E_INAVLIDATE_PARAM			((LONG)0x8000F008)

#define X_E_BMP_BIT_NOT_SUPPORT         ((LONG)0x8000F009)
#define X_E_NOT_MOVIE_TYPE              ((LONG)0x8000F00A)
#define X_E_COMPRESS_BMP_FAIL			((LONG)0x8000F00B)
#define X_E_BACKUP_ANIM_FAIL			((LONG)0x8000F00C)

#define X_E_COMPRESS_ANIM_FAIL			((LONG)0x8000F00D)
#define X_E_DELETE_DIR_FAIL  			((LONG)0x8000F00E)

#define X_E_ANIM_FRAME_SIZE_DIFFERENT   ((LONG)0x8000F00F)
#define X_E_COMPRESS_BMP_FAIL			((LONG)0x8000F00B)
#define X_E_BACKUP_ANIM_FAIL			((LONG)0x8000F00C)

#define X_E_COMPRESS_ANIM_FAIL			((LONG)0x8000F00D)
#define X_E_DELETE_DIR_FAIL  			((LONG)0x8000F00E)

#define X_E_WRITE_FILE_FAIL				((LONG)0x8000F00F)

#define X_E_COMPRESS_OSD_FAIL			((LONG)0x8000F010)
#define X_E_COMPRESS_OSD_OVER_SIZE		((LONG)0x8000F011)
#define X_E_DITHERING_FAIL              ((LONG)0x8000F012)
#define X_E_NO_ALPHA                    ((LONG)0x8000F013)
#define X_E_COLOR_FULL                  ((LONG)0x8000F014)

//for msg
// 操作过程指示消息, wParam表过程的状态， lParam 表位置
#define WM_PROGRESS         (WM_USER+603)     
// 过程的状态                                           
const int PROG_BEGIN   = 0;
const int PROG_PROCEED = 1;
const int PROG_END     = 2;
const int PROG_TITLE   = 3;


typedef struct _X_SKIN_INFO_T
{
	PRD_SKN_INFO agSkin[MAX_PRD_NUM];
	DWORD dwSkinNum;
	_TCHAR szReserved[MAX_PATH+1];
	
	_X_SKIN_INFO_T()
	{
		ZeroMemory(this,sizeof(_X_SKIN_INFO_T));
	}
}X_SKIN_INFO_T, *X_SKIN_INFO_PTR;

typedef struct _GLOBAL_INFO_T
{
	X_LANG_INFO_T tLang;
	TCHAR szResVer[20];
	DWORD   dwMaxMdulNum;

	DWORD   dwGlobalImgCmpType;
	DWORD   dwGlobalImgCmpParam;
	DWORD   dwGlobalAnimCmpType;
	DWORD   dwGlobalAnimCmpParam;

	DWORD   dwDefaultImgCmpType;
	DWORD   dwDefaultImgCmpParam;
	DWORD   dwDefaultAnimCmpType;
	DWORD   dwDefaultAnimCmpParam;

	DWORD   dwGlobalJpgCmpRateType;
	DWORD   dwDefaultJpgCmpRateType;

	DWORD   dwKeyColor;

	DWORD   dwMaxRingFileSize;

	TCHAR    szResDir[_MAX_PATH];

	X_SKIN_INFO_T tSkin;

	_GLOBAL_INFO_T()
	{
		ZeroMemory(this,sizeof(_GLOBAL_INFO_T));
	}

}GLOBAL_INFO,*GLOBAL_INFO_PTR;



typedef struct _IMG_PARAM_T
{
	LPBYTE	pSrcData;
	DWORD	dwSrcSize;
	LPBYTE	pDstData;
	DWORD	dwDstSize;
	DWORD   dwWidth;
	DWORD   dwHeight;
	BYTE	nImgType;
	BYTE	nImgCmpType;
	BYTE    nQualityType;       // 0:Q40,1:Q50, 2:Q70, 3:Q85, 4:Q95
	BYTE    nYUVType;      // 0:420, 1:411, 2:444, 3:422, 4:400
	int     nDither;  // 0, not dither; 1, dither for image
	BOOL    bReservedHead; // only for jpeg
	DWORD   dwSabmFlag;
	int     nUserData;
	_IMG_PARAM_T()
	{
		ZeroMemory(this,sizeof(_IMG_PARAM_T));
	}
}IMG_PARAM_T,*IMG_PARAM_PTR;

typedef struct _ANIM_PARAM_T
{
	LPBYTE	pDstData;
	DWORD	dwDstSize;
	DWORD   dwWidth;
	DWORD   dwHeight;
	DWORD   dwFramNum;	
	DWORD   dwFramRate;
	DWORD   dwKeyColor;
	DWORD   dwSrcSize;
	BYTE	nImgType;
	BYTE	nAnimCmpType;
	BYTE    nQualityType;       // 0:Q40,1:Q50, 2:Q70, 3:Q85, 4:Q95
	BYTE    nYUVType;      // 0:420, 1:411, 2:444, 3:422, 4:400
	int     nDither;  // 0, not dither; 1, dither for image
	int     nUserData;
	_ANIM_PARAM_T()
	{
		ZeroMemory(this,sizeof(_ANIM_PARAM_T));
	}
}ANIM_PARAM_T,*ANIM_PARAM_PTR;

typedef CStringArray* PSTRARRAY;

typedef CMap<CString, LPCTSTR, PSTRARRAY, PSTRARRAY> STRING_MAP;

#endif //__GLOBALSTRUCT_H