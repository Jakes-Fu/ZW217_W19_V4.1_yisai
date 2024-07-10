
#ifndef _CALIBRATION_H
#define	_CALIBRATION_H

//#define AGC_SIZE     2 * 142 * 2   
//#define APC_SIZE     90 * 2

// Merge NV related

#define MAX_NV_CORE_NAME	(16)
#define MAX_NVITEM_IMG_SIZE (64*1024*1024)
#define INDEX_NV_ID		(0x0)
#define PADDING_NV_ID	(0xFFFE)

#define MAX_PADDING_SIZE	(512)
static WORD g_INDEX_NV_ID	= 0x0;
static WORD g_PADDING_NV_ID = 0xFFFE;

#define INDEX_MAGIC			(0x58444E49)  //INDX
#define INDEX_HEADER_VER1	(0x0)
typedef struct _MULTI_CORE_NV_HEADER_T_
{
    DWORD dwMagic;
    WORD  wVersion;
    WORD  wCountCore;	// the count of nv core
    _MULTI_CORE_NV_HEADER_T_()
    {
        init();
    }
    void init()
    {
        memset(this,0,sizeof(_MULTI_CORE_NV_HEADER_T_));
        dwMagic	= INDEX_MAGIC;
        wVersion = INDEX_HEADER_VER1;
    }
}MULTI_CORE_NV_HEADER_T,*PMULTI_CORE_NV_HEADER_PTR;

typedef struct _NV_CORE_INFO_T_
{
    char szCoreName[MAX_NV_CORE_NAME];		// nv core name,
    DWORD  dwDataOffset;					// the offset of nv data 
    DWORD  dwDataSize;						// the length of nv data
    _NV_CORE_INFO_T_()
    {
        memset(this,0,sizeof(_NV_CORE_INFO_T_));
    }

}NV_CORE_INFO_T,*PNV_CORE_INFO_PTR;


#define NV_MULTI_LANG_ID   (405)
#define GSM_CALI_ITEM_ID   (0x2)
#define GSM_IMEI_ITEM_ID   (0x5)
#define XTD_CALI_ITEM_ID   (0x516)
#define LTE_CALI_ITEM_ID   (0x9C4)
#define BT_ITEM_ID         (0x191)
#define WCDMA_CALI_ITEM_ID   (0x12D)

#define BT_ADDR_LEN  6
typedef struct BT_CONFIG_T
{
	BYTE  bt_addr[BT_ADDR_LEN];
	WORD  xtal_dac;
}BT_CONFIG;

enum {
	E_GSM_CALI		= 0,    //bit 0
	E_TD_CALI		= 1,    //bit 1
	E_WCDMA_CALI	= 2,    //bit 2
	E_LTE_CALI		= 3,    //bit 3
    E_CALI_COUNT
};

struct TCALIINFO
{
    WORD    wDefNvID;
    WORD    wDefCheckID;
    TCHAR   szCaliKeyWord[MAX_PATH];
};
const TCALIINFO g_CaliFlagTable[] =		
{
    { GSM_CALI_ITEM_ID,     GSM_CALI_ITEM_ID,     _T("Calibration")       },    // GSM 
    { XTD_CALI_ITEM_ID,     GSM_CALI_ITEM_ID,     _T("TD_Calibration")    },    // TD  ,check flag same as GSM
    { WCDMA_CALI_ITEM_ID,   WCDMA_CALI_ITEM_ID,   _T("W_Calibration")     },    // WCDMA 
    { LTE_CALI_ITEM_ID,     LTE_CALI_ITEM_ID,     _T("LTE_Calibration")   },    // LTE
};

CString GetErrorDesc( UINT dwID );

///////////////////////////////////////////////////////////////////////
BOOL  XFindNVOffset(WORD wId,LPBYTE lpCode,DWORD dwCodeSize,
                     DWORD& dwOffset,DWORD& dwLength,BOOL bBigEndian = TRUE, BOOL bCoreNV=FALSE);

BOOL  XFindNVOffsetEx(WORD wId,LPBYTE lpCode,DWORD dwCodeSize,
                      DWORD& dwOffset,DWORD& dwLength,BOOL &bBigEndian, BOOL bModule);

BOOL  XFindNVOffsetExMerge(WORD wId,LPBYTE lpCode,DWORD dwCodeSize,
                      DWORD& dwOffset,DWORD& dwLength,BOOL &bBigEndian, BOOL bModule, BOOL bCoreNV=FALSE);

DWORD GSMCaliPreserve( WORD wID,LPBYTE lpCode, DWORD dwCodeSize, 
                       LPBYTE lpReadBuffer, DWORD dwReadSize,
                       BOOL bOldReplaceNew , BOOL bContinue );

DWORD XTDCaliPreserve(  WORD wID,LPBYTE lpCode, DWORD dwCodeSize, 
                        LPBYTE lpReadBuffer, DWORD dwReadSize,
                        BOOL bOldReplaceNew , BOOL bContinue );

DWORD XPreserveNVItem( WORD wID, LPBYTE lpCode, DWORD dwCodeSize, 
					   LPBYTE lpReadBuffer, DWORD dwReadSize,
					   BOOL bOldReplaceNew=FALSE, BOOL bContinue=FALSE);

BOOL  XCheckGSMCali(WORD wId,LPBYTE lpPhoBuf,DWORD dwPhoSize,CString &strErr,BOOL bModule);

BOOL XCheckNVStruct(const LPBYTE lpPhoBuf, DWORD dwPhoSize,BOOL bBigEndian, BOOL bModule);
BOOL XCheckNVStructEx(LPBYTE lpPhoBuf, DWORD dwPhoSize,BOOL &bBigEndian,BOOL bModule);
BOOL XCheckNVStructMerge(const LPBYTE lpPhoBuf, DWORD dwPhoSize,BOOL bBigEndian, BOOL bModule, BOOL bCoreNV=FALSE);
BOOL XCheckNVStructExMerge(LPBYTE lpPhoBuf, DWORD dwPhoSize,BOOL &bBigEndian,BOOL bModule, BOOL bCoreNV=FALSE);

//BOOL  XSetRandomBT( WORD wId, LPBYTE lpCode, DWORD dwCodeSize, const BT_CONFIG &bt);

DWORD  XPreserveIMEIs( CUIntArray *pIMEIID,LPBYTE lpCode, DWORD dwCodeSize, 
					  LPBYTE lpReadBuffer, DWORD dwReadSize, int &nFailedIMEIIndex,
					  BOOL bOldReplaceNew=FALSE, BOOL bContinue=FALSE);
/*
DWORD  LTECaliPreserve(  LPBYTE lpCode, DWORD dwCodeSize, 
					  LPBYTE lpReadBuffer, DWORD dwReadSize,
                        BOOL bOldReplaceNew , BOOL bContinue );
*/
BOOL  XCheckLTECali(WORD wId,LPBYTE lpPhoBuf,DWORD dwPhoSize,CString &strErr,BOOL bModule);
BOOL  XCheckWCDMACali(WORD wId,LPBYTE lpPhoBuf,DWORD dwPhoSize,CString &strErr,BOOL bModule);
BOOL  CalibrationCheck(WORD wId,LPBYTE lpPhoBuf,DWORD dwPhoSize,CString &strErr,int nCaliType);//nCaliType=0:GSM,1:TD,2:WCDMA,3:LTE

BOOL ClearGSMCaliFlag(WORD wId,LPBYTE lpNVBuf,DWORD dwNVSize);
BOOL ClearWCDMACaliFlag(WORD wId,LPBYTE lpNVBuf,DWORD dwNVSize);
BOOL ClearLTECaliFlag(WORD wId,LPBYTE lpNVBuf,DWORD dwNVSize);
void GSM_VerBToB( LPBYTE pFileBuf,DWORD dwOffsetFile,BOOL bFileBigEndian,LPBYTE pModuBuf,DWORD dwOffsetModu,BOOL bModuBigEndian );

BOOL IsMultiCore(const LPBYTE lpBuf, const DWORD& dwSize,PMULTI_CORE_NV_HEADER_PTR& pCoreHeader,PNV_CORE_INFO_PTR& ppCoreItem);
BOOL MergeNormalNV(LPBYTE lpCodeSrc, DWORD dwCodeSizeSrc, LPBYTE lpCodeDest, DWORD & dwCodeSizeDest, LPBYTE lpCodeFinal, DWORD & dwCodeSizeFinal, BOOL bCoreNV = FALSE);
DWORD MergeNV(LPBYTE lpCodeSrc, DWORD dwCodeSizeSrc, LPBYTE lpCodeDest, DWORD &dwCodeSizeDest);

//////////////////////////////////////////////////////////////////////////
               
#endif // _CALIBRATION_H