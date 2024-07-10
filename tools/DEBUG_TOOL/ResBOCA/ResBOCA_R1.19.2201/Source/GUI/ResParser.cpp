// ResParser.cpp: implementation of the CResParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResOver.h"
#include "ResParser.h"
#include <atlconv.h>

#include "mmi_resource_def.h"
#include "mmi_image_def.h"
#include "mmi_anim_def.h"
#include "mmi_ring_def.h"
#include "mmi_other_def.h"
#include "mmi_font_def.h"
#include "mmi_text_def.h"
#include "img_def.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DEFAULT_RES_INFO_OFFSET     (0x10)
#define THEME_RES_INFO_OFFSET       (0x18)
#define THEME_NUM_OFFSET            (0x118)
#define LOGO_OFFSET                 (0x11C)

#define GET_THEME_NUM(p)   ((BYTE)*(p+THEME_NUM_OFFSET))

#define  CONV_SHORT_EX(w)    if(m_bBigEndian) w = (MAKEWORD(HIBYTE(w),LOBYTE(w)))


#define  CONV_DWORD_EX(dw)   if(m_bBigEndian) dw = (MAKELONG(MAKEWORD(HIBYTE(HIWORD(dw)),LOBYTE(HIWORD(dw))),\
                                    MAKEWORD(HIBYTE(LOWORD(dw)),LOBYTE(LOWORD(dw)))))


#define BUF_SIZE  (10*1024*1024)

#define IMAG_TIPS   (m_bCheckImagWH ? _T("\r\nYOU CAN ONLY repalce the image with the same image type, width and height!!!") : _T(""))
#define ANIM_TIPS   ((m_bCheckAnimWH&&m_bCheckAnimFrame) ? _T("\r\nYOU CAN ONLY repalce the anim with the same image type, width, height and frames!!!") : _T(""))
#define RING_TIPS   (m_bCheckRingType ? _T("\r\nYOU CAN ONLY repalce the ring with the same ring type!!!") : _T(""))

#define CHECK_ORG_PTR(p)   if((p)>(m_pResBuf+m_dwResSize)) return FALSE


#define SHOW_MESSAGE(msg,flag) do{ \
	if(m_bCmd)\
	{\
		_tprintf(_T("%s\n"),msg);\
	}\
	else if(flag&&!m_bLoadThmFolder)\
	{\
		AfxMessageBox(msg);\
	}\
}while(0)


static void ConvImagFlag(IMG_FLAG_T &imgFlag)
{

	imgFlag.nHeight = CONV_SHORT(imgFlag.nHeight);
	imgFlag.nWidth = CONV_SHORT(imgFlag.nWidth);
	imgFlag.nType = CONV_SHORT(imgFlag.nType);
	imgFlag.nOtherInfo = CONV_SHORT(imgFlag.nOtherInfo);
	imgFlag.nSize = CONV_DWORD(imgFlag.nSize);
}

static void ConvAnimFlag(ANIM_FLAG_T &animFlag)
{
	animFlag.nHeight = CONV_SHORT(animFlag.nHeight);
	animFlag.nWidth = CONV_SHORT(animFlag.nWidth);
	animFlag.nType = CONV_SHORT(animFlag.nType);
	animFlag.nFrameNum = CONV_SHORT(animFlag.nFrameNum);
	animFlag.nSize = CONV_DWORD(animFlag.nSize);
}

static void ConvRingFlag(MMIAUD_RING_DATA_INFO_T &ringFlag)
{
	ringFlag.data_len = CONV_DWORD(ringFlag.data_len);
	ringFlag.type = (MMIAUDIO_TYPE_E)CONV_DWORD(ringFlag.type); //lint !e572
	ringFlag.data_ptr = (uint8*)CONV_DWORD(((DWORD)ringFlag.data_ptr));
}

static void ConvDataFlag(OTHER_DATA_INFO_T &dataFlag)
{
	dataFlag.data_len = CONV_DWORD(dataFlag.data_len);
	dataFlag.type = (OTHER_DATA_TYPE_E)CONV_DWORD(dataFlag.type); //lint !e572
	dataFlag.data_ptr = (uint8*)CONV_DWORD(((DWORD)dataFlag.data_ptr));
}

static void ConvColorFlag(MMI_COLOR_T &colorFlag)
{
	colorFlag.dwColor = CONV_DWORD(colorFlag.dwColor);
}

static bool CmpItemStat(const ITEM_STAT_T &a,const ITEM_STAT_T &b)
{ 
	return a.nDeltSize>b.nDeltSize;
}

static DWORD GetMMITagSize(MMI_TAG2_T &tag)
{
	DWORD dwSize = (DWORD)(sizeof(MMI_TAG2_T) - MAX_TAG_DESC_LEN*sizeof(char) + strlen(tag.szDesc) + 1);
	return ( (dwSize + 3) & ~3 );
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CResParser::CResParser()
{
	m_dwMaxResSize = MAXDWORD;
	m_pResBuf = NULL;
	m_dwResSize = 0;
	m_strResFile = _T("");
	m_pNewBuf = NULL;
	m_nNewSize = 0;
	m_vResItem.clear();
	m_bBigEndian = FALSE;
	m_uThemeNum = 0;
	m_uExtTblNum = 0;
	m_bThemeOnly = FALSE;
//	m_pTmpBuf = NULL;

	m_bCheckImagWH = TRUE;
	m_bCheckAnimWH = TRUE;
	m_bCheckAnimFrame = TRUE;
	m_bCheckRingType = TRUE;
	m_bShowWarning = TRUE;

	m_dwCurTotalSize = 0;

	m_pRC = NULL;
	CreateResConverter(&m_pRC, 2);
	
	m_pBzp= NULL;
	CreateBZPFactory(&m_pBzp);

	m_bBzp= FALSE;

	m_bCmd = FALSE;
	m_dwCmdMaxSize = MAXDWORD;
	m_bCscs = FALSE;
	//m_bAllowEmpty = FALSE;

	m_dwTextOrgSize = 0;
	m_dwTextRepSize = 0;
	
	m_dwLDBOrgSize  = 0;
	m_dwLDBRepSize  = 0;

	m_bLoadThmFolder = FALSE;

	m_nExcelParserType = -1;
	m_bReadOnce = TRUE;
	m_bRmCrossRepeat = FALSE;
	m_bAllUnicode = TRUE;
	m_bStrNoFlag = FALSE;
	
	m_hWnd = NULL;
}

CResParser::~CResParser()
{
/*lint -save -e1551 */
	Clear();
	if(m_pRC != NULL)
	{
		m_pRC->Release();
		m_pRC = NULL;
	}
	m_pResBuf = NULL;
	m_pNewBuf = NULL;
//	m_pTmpBuf = NULL;

	if(m_pBzp != NULL)
	{
		m_pBzp->Release();
		m_pBzp = NULL;
	}
	m_hWnd = NULL;
/*lint -restore */
}

BOOL CResParser::LoadResFile(LPCTSTR lpszFile)
{
	Clear();

	m_vResItem.reserve(200);

	if(lpszFile == NULL)
		return FALSE;

	lpszFile = PreOpenResFile(lpszFile);
	if(lpszFile == NULL)
	{
		return FALSE;
	}

	
	HANDLE hFile = CreateFile(lpszFile,
		GENERIC_READ, 
		FILE_SHARE_READ,
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CString strErr;
		strErr.Format(_T("Can not open file [%s].\r\nMaybe it is used by other program!!!"),lpszFile);
		AfxMessageBox(strErr.operator LPCTSTR());
		return FALSE;
	}
	
	m_dwResSize = GetFileSize(hFile,NULL);
	if(m_dwResSize < 0x130)
	{
		CloseHandle(hFile);
		CString strErr;
		strErr.Format(_T("[%s] is too small!"),lpszFile);
		AfxMessageBox(strErr.operator LPCTSTR());
		return FALSE;
	}
	m_pResBuf = new BYTE[m_dwResSize + MAX_TAG_DESC_LEN ];
	if(m_pResBuf == NULL)
	{
		CloseHandle(hFile);
		CString strErr;
		strErr.Format(_T("Memery is full!"));
		AfxMessageBox(strErr.operator LPCTSTR());
		return FALSE;
	}
	memset(m_pResBuf,0,m_dwResSize+ MAX_TAG_DESC_LEN);
	DWORD dwRead;
	ReadFile(hFile,m_pResBuf,m_dwResSize,&dwRead,NULL);	
	CloseHandle(hFile);

	if(LoadResInfo())
	{
		m_strResFile = lpszFile;
		m_nNewSize = (int)m_dwResSize;
		return TRUE;
	}
	else
	{
		Clear();
		return FALSE;
	}	
}

BOOL CResParser::SaveResFile(LPCTSTR lpszFile)
{
	CString strErr;

	if(!IsModified())
	{
		strErr.Format(_T("[WARNING]: none of resource is replaced."));
		SHOW_MESSAGE(strErr,FALSE);
		return FALSE;
	}

	int nNewSizeTmp = m_nNewSize;

	//CalcModifedName();

	m_nNewSize -= (int)(m_dwTextOrgSize - m_dwTextRepSize);
	//m_nNewSize -= (int)(m_dwLDBOrgSize - m_dwLDBRepSize);


	_ASSERT(m_nNewSize > 0 );

	if(m_pResBuf == NULL)
	{
		m_nNewSize = nNewSizeTmp;
		return FALSE;
	}

	SAFE_DEL(m_pNewBuf);
	m_pNewBuf = new BYTE[(UINT)(m_nNewSize + BUF_SIZE)];  // 多出10M余量
	if(m_pNewBuf == NULL)
	{
		m_nNewSize = nNewSizeTmp;
		return FALSE;
	}

//	SAFE_DEL(m_pTmpBuf);
//	m_pTmpBuf = new BYTE[(UINT)(m_nNewSize + BUF_SIZE)];
//	if(m_pTmpBuf == NULL)
//	{
//		return FALSE;
//	}
//	memset(m_pTmpBuf,0,m_nNewSize + BUF_SIZE);

	memset(m_pNewBuf,0,m_nNewSize + BUF_SIZE);
	memcpy(m_pNewBuf,m_pResBuf,m_dwResSize);

	m_dwCurTotalSize = m_dwResSize;


	UINT nCount =  m_vResItem.size();

	for(UINT i = 0; i< m_uThemeNum; i++)
	{
		for(UINT j = 0; j< nCount ; j++)
		{
			if(!SaveResItem(j,i))
			{
				SAFE_DEL(m_pNewBuf);
				m_nNewSize = nNewSizeTmp;
				return FALSE;
			}
		}
	}

	/*
	if(!SaveResInfo())
	{
		SAFE_DEL(m_pNewBuf);
		m_nNewSize = nNewSizeTmp;
		return FALSE;
	}
	*/

	SaveString(0);

	_ASSERT(m_dwCurTotalSize == (DWORD)m_nNewSize);
	if(m_dwCurTotalSize != (DWORD)m_nNewSize)
	{
		strErr.Format(_T("[ERROR]: Replace error."));
		SHOW_MESSAGE(strErr,FALSE);
		m_nNewSize = nNewSizeTmp;
		return FALSE;
	}

	

	if(m_bCmd)
	{
		if(m_dwCurTotalSize > m_dwCmdMaxSize)
		{
			strErr.Format(_T("[ERROR]: new size [0x%x] is over the max size [0x%x]."),m_dwCurTotalSize,m_dwCmdMaxSize );
			SHOW_MESSAGE(strErr,TRUE);
			m_nNewSize = nNewSizeTmp;
			return FALSE;
		}
	}

	CString strFile;
	if(lpszFile != NULL)
	{
		strFile= lpszFile;
		if(m_bBzp)
		{
			m_strBzpFile = lpszFile;
			strFile = m_strResFile;
		}
	}
	else
	{
		strFile = m_strResFile;
	}

	CFileFind finder;
	if(finder.FindFile(strFile.operator LPCTSTR()))
	{
		DWORD dwAtt = ::GetFileAttributes(strFile.operator LPCTSTR());
		dwAtt &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strFile.operator LPCTSTR(),dwAtt);
	}

	BOOL bOK = TRUE;
	FILE *pFile = _tfopen(strFile.operator LPCTSTR(),_T("wb"));
	if(pFile != NULL)
	{
		DWORD dwWirte = fwrite(m_pNewBuf,1,m_nNewSize,pFile);
		fflush(pFile);
		fclose(pFile);
		if(dwWirte != (DWORD)m_nNewSize)
		{
			strErr.Format(_T("[ERROR]: Write file failed. [%s]"),strFile.operator LPCTSTR() );
			SHOW_MESSAGE(strErr,TRUE);
			bOK = FALSE;
		}		
	}
	else
	{
		strErr.Format(_T("[ERROR]: Can not open file [%s].\nMaybe it is used by other program!"),strFile.operator LPCTSTR());
		SHOW_MESSAGE(strErr,TRUE);
		m_nNewSize = nNewSizeTmp;
		return FALSE;
	}
	
	if(bOK)
	{
		if(m_bBzp)
		{
			m_nNewSize = nNewSizeTmp;
			return PostSaveResFile();
		}
	}
	m_nNewSize = nNewSizeTmp;
	return bOK;
}

BOOL CResParser::LoadResInfo()
{
	USES_CONVERSION;

	CString strErr;

	if(m_pResBuf == NULL)
		return FALSE;

	if( (m_pResBuf[0] != 's' && m_pResBuf[0] != 't') ||
		m_pResBuf[1] != 'r' ||
		m_pResBuf[2] != 'e' ||
		m_pResBuf[3] != 's' )
	{		
		strErr.Format(_T("[ERROR]: It is not res bin format!"));
		SHOW_MESSAGE(strErr,TRUE);
		return FALSE;
	}

	if(m_pResBuf[0] == 't')
	{
		m_bThemeOnly = TRUE;
	}
	else
	{
		m_bThemeOnly = FALSE;
	}

	m_bBigEndian = (m_pResBuf[12]==1)? TRUE: FALSE;

	if(m_pResBuf[14] == FLAG_STR_NO_FLAG && m_pResBuf[15] == 1)
	{
		m_bStrNoFlag = TRUE;
	}
	else
	{
		m_bStrNoFlag = FALSE;
	}

	if(!m_bStrNoFlag)
	{
		TestStrNoFlag(m_pResBuf);
	}

	int nTblNum = m_pResBuf[13];
	if(0 == nTblNum )
	{		
		strErr.Format(_T("[ERROR]: There is no replacemant infomation!"));
		SHOW_MESSAGE(strErr,TRUE);
		return FALSE; // 没有需要替换的资源
	}
	m_uExtTblNum = (UINT)nTblNum;

	DWORD dwOffset = 0;
	DWORD dwSize = 0;
	LPBYTE pCurBuf = NULL;

	m_agThemeName.RemoveAll();

	if(m_bThemeOnly)
	{		
		m_uThemeNum = 1;
		DWORD dwTotalSize = *(DWORD*)(m_pResBuf+16);
		CONV_DWORD_EX(dwTotalSize);
		pCurBuf = m_pResBuf + dwTotalSize;

		dwOffset = *(DWORD*)(m_pResBuf + 20);
		CONV_DWORD_EX(dwOffset);
		dwOffset -= sizeof(MMI_MODULE_VER_INFO_T);
		MMI_MODULE_VER_INFO_T * pVer = (MMI_MODULE_VER_INFO_T *)(m_pResBuf + dwOffset);
#ifdef _UNICODE
		m_agThemeName.Add(A2W((char*)pVer->resource_name_array));
#else
		m_agThemeName.Add((char*)pVer->resource_name_array);
#endif 

	}
	else
	{
		BYTE bThemeNum = *(m_pResBuf + THEME_NUM_OFFSET);
		m_uThemeNum = bThemeNum+1;
		pCurBuf = m_pResBuf + LOGO_OFFSET;

		for(UINT j = 0 ; j< m_uThemeNum; j++)
		{		
			DWORD dwThemeOffset = *(DWORD*)(m_pResBuf+16+j*8);
			CONV_DWORD_EX(dwThemeOffset);

			dwOffset = *(DWORD*)(m_pResBuf +dwThemeOffset+ 20);
			CONV_DWORD_EX(dwOffset);
			dwOffset -= sizeof(MMI_MODULE_VER_INFO_T);
			MMI_MODULE_VER_INFO_T * pVer = (MMI_MODULE_VER_INFO_T *)(m_pResBuf + dwThemeOffset + dwOffset);
#ifdef _UNICODE
			m_agThemeName.Add(A2W((char*)pVer->resource_name_array));
#else
			m_agThemeName.Add((char*)pVer->resource_name_array);
#endif 
		}
	} 
	

	BOOL nLogoNum = 0;

	for(int i = 0 ; i< nTblNum; i++)
	{
		if( pCurBuf[0] == 'l' &&
			pCurBuf[1] == 'o' &&
			pCurBuf[2] == 'g' &&
			pCurBuf[3] == 'o' )
		{
			dwOffset = *(DWORD*)(pCurBuf+4);
			CONV_DWORD_EX(dwOffset);
			dwSize = *(DWORD*)(pCurBuf+8);
			CONV_DWORD_EX(dwSize);
			if(!LoadLogo(dwOffset, dwSize,nLogoNum))
			{
				return FALSE;
			}
			nLogoNum ++;
		 }
		 else if( pCurBuf[0] == 'r' &&
			 pCurBuf[1] == 'e' &&
			 pCurBuf[2] == 'p' &&
			 pCurBuf[3] == 'l' )
		 {
			 dwOffset = *(DWORD*)(pCurBuf+4);
			 CONV_DWORD_EX(dwOffset);
			 dwSize = *(DWORD*)(pCurBuf+8);
			 CONV_DWORD_EX(dwSize);
			 if(!LoadRepl(dwOffset, dwSize))
			 {
				 return FALSE;
			 }
		 }
		 else if( pCurBuf[0] == 'c' &&
			 pCurBuf[1] == 's' &&
			 pCurBuf[2] == 'c' &&
			 pCurBuf[3] == 's' )
		 {
			 m_bCscs = TRUE;
			 dwOffset = *(DWORD*)(pCurBuf+4);
			 CONV_DWORD_EX(dwOffset);
			 dwSize = *(DWORD*)(pCurBuf+8);
			 CONV_DWORD_EX(dwSize);
			 if(!LoadCscs(dwOffset, dwSize))
			 {
				 return FALSE;
			 }
		 }
		 else if( pCurBuf[0] == 'l' &&
			 pCurBuf[1] == 'a' &&
			 pCurBuf[2] == 'n' &&
			 pCurBuf[3] == 'g' )
		 {
			 dwOffset = *(DWORD*)(pCurBuf+4);
			 CONV_DWORD_EX(dwOffset);
			 dwSize = *(DWORD*)(pCurBuf+8);
			 CONV_DWORD_EX(dwSize);
			 if(!LoadLang(dwOffset, dwSize))
			 {
				 return FALSE;
			 }
		 }

		 pCurBuf+= 12;
	}

	return  TRUE;
}


BOOL CResParser::LoadLogo(DWORD dwOffset, DWORD dwSize,int nIndex)
{
	if(m_pResBuf == NULL)
		return FALSE;

    if(dwOffset == 0)
		return TRUE;
	
	if((dwOffset+ALIGN4_SIZE(dwSize)+sizeof(IMG_FLAG_T)) > m_dwResSize)
	{
		AfxMessageBox(_T("Logo offset and size error!"));
		return FALSE;
	}

	BYTE *pImagBuf = m_pResBuf + dwOffset;
	//DWORD dwImagSize = dwSize;

	RES_ITEM_PTR pRI = new RES_ITEM; //lint !e429

	CString str;
	str.Format(_T("LOGO_%d"),m_vResItem.size());
	_tcscpy(pRI->szDesc,str);

	pRI->uResType = 0; // IMAGE
	pRI->itemInfo[0].wExist = TRUE;
	pRI->itemInfo[0].org.dwOffset = dwOffset;
	pRI->itemInfo[0].org.dwSize = dwSize;
	pRI->itemInfo[0].org.pBuf = new BYTE[dwSize];
	memcpy(pRI->itemInfo[0].org.pBuf,pImagBuf,dwSize);
	pRI->bLogo = TRUE;

	IMG_FLAG_T tImgFlag = {0};
	memcpy(&tImgFlag,pImagBuf + ALIGN4_SIZE(dwSize),sizeof(tImgFlag));

	if(m_bBigEndian)
	{
		ConvImagFlag(tImgFlag);
	}

	pRI->itemInfo[0].org.wHeight   = tImgFlag.nHeight;
	pRI->itemInfo[0].org.wWidth    =  tImgFlag.nWidth;
	pRI->itemInfo[0].org.wCmpType  = LOBYTE(tImgFlag.nType);
	pRI->itemInfo[0].org.wItemType = HIBYTE(tImgFlag.nType);
	pRI->uResIndex = (UINT)nIndex;

	WORD wCmpType = pRI->itemInfo[0].org.wCmpType;
	WORD wItemType = pRI->itemInfo[0].org.wItemType;

	if(m_bBigEndian)
	{
		if( wCmpType == IMG_CMP_BMP_16_565_RAW || wCmpType == IMG_CMP_BMP_16_565_RLE||
			wCmpType == IMG_CMP_BMP_OSD        || wCmpType == IMG_CMP_BMP_8_RAW ||
			wCmpType == IMG_CMP_BMP_8_SCI	   || wCmpType == IMG_CMP_BMP_CK ||
			wCmpType == IMG_CMP_BMP_OPT)
		{
			BOOL bBmp = TRUE;
			if(wCmpType == IMG_CMP_BMP_OPT)
			{
				if( pRI->itemInfo[0].org.pBuf[0] == (BYTE)'S' &&
					pRI->itemInfo[0].org.pBuf[1] == (BYTE)'J' &&
					pRI->itemInfo[0].org.pBuf[2] == (BYTE)'P' &&
					pRI->itemInfo[0].org.pBuf[3] == (BYTE)'G')
				{
					bBmp = FALSE;
				}

			}

			if(bBmp)
			{
				m_pRC->BmpConvertToLitEndian(pRI->itemInfo[0].org.pBuf,
											 pRI->itemInfo[0].org.dwSize,
											 wCmpType,
											 wCmpType);
			}

		}
	}

	if(!m_strTmpFolder.IsEmpty())
	{
		CString strExt = GetImgFileExtString(wCmpType,wItemType).operator LPCTSTR();
		if(wCmpType == IMG_CMP_BMP_OPT)
		{
			if( pRI->itemInfo[0].org.pBuf[0] == (BYTE)'S' &&
				pRI->itemInfo[0].org.pBuf[1] == (BYTE)'J' &&
				pRI->itemInfo[0].org.pBuf[2] == (BYTE)'P' &&
				pRI->itemInfo[0].org.pBuf[3] == (BYTE)'G')
			{
				strExt=_T("JPG");
			}

		}
		CString strFile;
		strFile.Format(_T("%s\\%s.%s"),m_strTmpFolder.operator LPCTSTR(),pRI->szDesc,strExt);

		IMG_PARAM_T ipt;
		ipt.dwDstSize = dwSize;
		ipt.pDstData = pRI->itemInfo[0].org.pBuf;
		ipt.nImgType = (BYTE)pRI->itemInfo[0].org.wItemType;
		ipt.nImgCmpType = (BYTE)pRI->itemInfo[0].org.wCmpType;

		HRESULT hr = m_pRC->ExpImagFile(strFile.operator LPCTSTR(),&ipt);
		if(SUCCEEDED(hr))
		{
			_tcscpy(pRI->itemInfo[0].org.szFile,strFile.operator LPCTSTR());
		}
		m_pRC->ReleaseMemory(ipt.pSrcData);
	}
	m_vResItem.push_back(pRI);
/*lint -save -e429 */
	return TRUE;
}/*lint -restore */

BOOL CResParser::LoadRepl(DWORD dwOffset, DWORD dwSize)
{
	USES_CONVERSION;

	if(m_pResBuf == NULL)
		return FALSE;

	if((dwOffset+dwSize)> m_dwResSize)
	{
		return FALSE;
	}

	BYTE *pBuf = m_pResBuf + dwOffset;

	// 去掉版本信息
	pBuf += 4; 
	dwSize -=4; 
	
	int nResItemCount = dwSize / sizeof(MMI_TAGS_T);
	MMI_TAGS_T repl = {0};
	for(int i = 0; i< nResItemCount; i++)
	{
		RES_ITEM_PTR pRI = new RES_ITEM;	
		memcpy(&repl,pBuf,sizeof(repl));
		pRI->uResType = repl.bResType;
		pRI->uModuleID = repl.bMduIndex;
		pRI->uResIndex = repl.dwResIndex;
		_tcscpy(pRI->szDesc,A2W(repl.szDesc));
		
		CONV_DWORD_EX(pRI->uResIndex);

		if(LoadResItem(pRI))
		{
			m_vResItem.push_back(pRI);
		}
		else
		{
			delete pRI;
		}
		
		pBuf += sizeof(MMI_TAGS_T);
	}
	
	return TRUE;
}
BOOL CResParser::LoadCscs(DWORD dwOffset, DWORD dwSize)
{
	USES_CONVERSION;
	
	if(m_pResBuf == NULL)
		return FALSE;
	
	if((dwOffset+dwSize)> m_dwResSize)
	{
		return FALSE;
	}
	
	BYTE *pBuf = m_pResBuf + dwOffset;
	
	// 去掉版本信息
	pBuf += 4; 
	dwSize -=4; 
	
	//int nResItemCount = dwSize / sizeof(MMI_TAGS_T);
	DWORD dwLeftSize = dwSize;
	MMI_TAG2_T repl = {0};
	while(((int)dwLeftSize)>0)
	{
		RES_ITEM_PTR pRI = new RES_ITEM;	
		memcpy(&repl,pBuf,sizeof(repl));
		pRI->uResType = repl.bResType;
		pRI->uModuleID = repl.bMduIndex;
		pRI->uResIndex = repl.dwResIndex;
		_tcscpy(pRI->szDesc,A2W(repl.szDesc));
		
		CONV_DWORD_EX(pRI->uResIndex);

		pRI->itemInfo[0].wExist = (WORD)((repl.bNotUsed == 0) ? 1 : 0);
		
		if(pRI->itemInfo[0].wExist)
		{
			if(LoadResItem(pRI))
			{
				pRI->itemInfo[0].wExist = 1;
			}
			else
			{
				pRI->itemInfo[0].wExist = 0;
			}
		}

		m_vResItem.push_back(pRI);
		
		DWORD dwTmpSize = GetMMITagSize(repl);
		pBuf += dwTmpSize;
		dwLeftSize-= dwTmpSize;
	}

	//GetIMInfo();

	return TRUE;
}

BOOL CResParser::LoadLang(DWORD dwOffset, DWORD dwSize)
{
	USES_CONVERSION;
	
	if(m_pResBuf == NULL)
		return FALSE;
	
	if((dwOffset+dwSize)> m_dwResSize)
	{
		return FALSE;
	}
	
	BYTE *pBuf = m_pResBuf + dwOffset;
	
	// 版本信息
	DWORD dwVer = *(DWORD*)pBuf;
	if(dwVer != 0)
	{
		return FALSE;
	}

	// 去掉版本信息
	pBuf += 4; 
	dwSize -=4; 	

	m_dwTextOrgSize = 0;

	DWORD dwLeftSize = dwSize;
	MMI_LANG_TAG_T repl = {0};
	while(((int)dwLeftSize)>0)
	{
		RES_ITEM_PTR pRI = new RES_ITEM;	
		memcpy(&repl,pBuf,sizeof(repl));	
		CONV_SHORT_EX(repl.wLangIndex);
		CONV_SHORT_EX(repl.wReserved);
		pRI->uResIndex = repl.wLangIndex;
		_tcscpy(pRI->szDesc,A2W(repl.szDesc));
		
		pRI->uResType = MMI_RT_MAX_NUM;

		if(repl.wReserved == 1)
		{
			pRI->itemInfo[0].wExist = 0;
		}
		else
		{
			pRI->itemInfo[0].wExist = 1;
			pRI->itemInfo[0].org.dwSize = GetLangTextSize(m_pResBuf,(int)pRI->uResIndex);
			m_dwTextOrgSize += pRI->itemInfo[0].org.dwSize;
			m_dwTextRepSize = m_dwTextOrgSize;
		}
		
		m_vResItem.push_back(pRI);
		
		DWORD dwTmpSize = sizeof(repl);
		pBuf += dwTmpSize;
		dwLeftSize-= dwTmpSize;
	}
	
	return TRUE;
}

BOOL CResParser::LoadResItem(RES_ITEM_PTR pri)
{
	BOOL bOK = FALSE;
	switch(pri->uResType)
	{
	case MMI_RT_IMAGE:
		bOK = LoadImage(pri);
		break;
	case MMI_RT_ANIM:
		bOK = LoadAnim(pri);
		break;
	case MMI_RT_RING:
		bOK = LoadRing(pri);
	    break;
	case MMI_RT_COLOR:
		bOK = LoadColor(pri);
		break;
	case MMI_RT_OTHER:
		bOK = LoadData(pri);
		break;
	default:
	    break;
	}
	return bOK;
}
BOOL CResParser::LoadImage(RES_ITEM_PTR pri)
{
	if(m_pResBuf == NULL)
	{
		return FALSE;
	}
	for(UINT i =  0; i< m_uThemeNum ; i++)
	{
		BYTE *pBaseBuf = NULL;
		if(m_bThemeOnly)
		{
			pBaseBuf = m_pResBuf;
		}
		else
		{
			DWORD dwThemeOffset = * (DWORD*)(m_pResBuf+ DEFAULT_RES_INFO_OFFSET + i*8); //lint !e613
			CONV_DWORD_EX(dwThemeOffset);

			pBaseBuf = m_pResBuf + dwThemeOffset;//lint !e613
		}
		CHECK_ORG_PTR(pBaseBuf);


		CHECK_ORG_PTR(pBaseBuf + (20 + pri->uModuleID * 4));
		DWORD dwiMduOffset = * (DWORD*)(pBaseBuf + (20 + pri->uModuleID * 4));//lint !e613
		CONV_DWORD_EX(dwiMduOffset);//lint !e613
		pBaseBuf += dwiMduOffset;//lint !e613

		CHECK_ORG_PTR(pBaseBuf+8);

		DWORD dwImgOffset = * (DWORD*)pBaseBuf;
		DWORD dwImgNum = * (DWORD*)(pBaseBuf+4);
		DWORD dwImgSize = * (DWORD*)(pBaseBuf+8);
		CONV_DWORD_EX(dwImgOffset);
		CONV_DWORD_EX(dwImgNum);
		CONV_DWORD_EX(dwImgSize);

		if(dwImgNum == 0)
		{
			pri->itemInfo[i].wExist = 0;
			continue;
		}

		_ASSERT(pri->uResIndex < dwImgNum);
		if(pri->uResIndex >= dwImgNum)
		{
			return FALSE;
		}

		CHECK_ORG_PTR(pBaseBuf + dwImgOffset+4);

		DWORD dwImgFlagOffset = * (DWORD*)(pBaseBuf + dwImgOffset);
		DWORD dwImgDataOffset = * (DWORD*)(pBaseBuf + dwImgOffset+4);
		CONV_DWORD_EX(dwImgFlagOffset);
		CONV_DWORD_EX(dwImgDataOffset);

		BYTE *pImgflagBuf = pBaseBuf+ (dwImgFlagOffset + pri->uResIndex*sizeof(IMG_FLAG_T));
		CHECK_ORG_PTR(pImgflagBuf);

		CHECK_ORG_PTR(pBaseBuf+ (dwImgDataOffset + pri->uResIndex*sizeof(DWORD)));
		DWORD imgDataOfffset =* (DWORD*)(pBaseBuf+ (dwImgDataOffset + pri->uResIndex*sizeof(DWORD)));
		CONV_DWORD_EX(imgDataOfffset);

		if(imgDataOfffset == 0)
		{
			pri->itemInfo[i].wExist = 0;
			continue;
		}
		else
		{
			pri->itemInfo[i].wExist = 1;
		}
		
		CHECK_ORG_PTR(pImgflagBuf+sizeof(IMG_FLAG_T));
		IMG_FLAG_T tImgFlag = {0};
		memcpy(&tImgFlag,pImgflagBuf,sizeof(IMG_FLAG_T));
		if(m_bBigEndian)
		{
			ConvImagFlag(tImgFlag);
		}

		CHECK_ORG_PTR(pBaseBuf+imgDataOfffset+tImgFlag.nSize);

		pri->itemInfo[i].org.wHeight = tImgFlag.nHeight;
		pri->itemInfo[i].org.wWidth= tImgFlag.nWidth;
		pri->itemInfo[i].org.wCmpType = LOBYTE(tImgFlag.nType);
		pri->itemInfo[i].org.wItemType = HIBYTE(tImgFlag.nType);
		pri->itemInfo[i].org.dwSize = tImgFlag.nSize;
		pri->itemInfo[i].org.pBuf = new BYTE[tImgFlag.nSize];		

		WORD wItemType = pri->itemInfo[i].org.wItemType;
		WORD wCmpType = pri->itemInfo[i].org.wCmpType;
		
		memcpy(pri->itemInfo[i].org.pBuf,pBaseBuf+imgDataOfffset,pri->itemInfo[i].org.dwSize);

		if(m_bBigEndian)
		{
			if( wCmpType == IMG_CMP_BMP_16_565_RAW || wCmpType == IMG_CMP_BMP_16_565_RLE||
				wCmpType == IMG_CMP_BMP_OSD        || wCmpType == IMG_CMP_BMP_8_RAW ||
				wCmpType == IMG_CMP_BMP_8_SCI	   || wCmpType == IMG_CMP_BMP_CK ||
				wCmpType == IMG_CMP_BMP_OPT)
			{
				BOOL bBmp = TRUE;
				if(wCmpType == IMG_CMP_BMP_OPT)
				{
					if( pri->itemInfo[i].org.pBuf[0] == (BYTE)'S' &&
						pri->itemInfo[i].org.pBuf[1] == (BYTE)'J' &&
						pri->itemInfo[i].org.pBuf[2] == (BYTE)'P' &&
						pri->itemInfo[i].org.pBuf[3] == (BYTE)'G')
					{
						bBmp = FALSE;
					}
				}

				if(bBmp)
				{
					m_pRC->BmpConvertToLitEndian(pri->itemInfo[i].org.pBuf,
												 pri->itemInfo[i].org.dwSize,
												 wCmpType,
												 wCmpType);
				}

			}
		}
		
		if(!m_strTmpFolder.IsEmpty())
		{
			CString strExt = GetImgFileExtString(wCmpType,wItemType).operator LPCTSTR();
			if(wCmpType == IMG_CMP_BMP_OPT)
			{
				if( pri->itemInfo[i].org.pBuf[0] == (BYTE)'S' &&
					pri->itemInfo[i].org.pBuf[1] == (BYTE)'J' &&
					pri->itemInfo[i].org.pBuf[2] == (BYTE)'P' &&
					pri->itemInfo[i].org.pBuf[3] == (BYTE)'G')
				{
					strExt = _T("JPG");
				}
			}
			CString strFile;
			CString strPathName = pri->szDesc;
			if(m_bCscs)
			{
				int nFind = strPathName.ReverseFind('\\');
				if(nFind != -1)
				{
					CString strPath;
					strPath.Format(_T("%s\\%s"),m_strTmpFolder.operator LPCTSTR(),strPathName.Left(nFind));
					g_theApp.CreateDeepDirectory(strPath);
				}
				
				/*strFile.Format(_T("%s\\%s"),m_strTmpFolder.operator LPCTSTR(),
					strPathName.operator LPCTSTR());*/
				strFile.Format(_T("%s\\%s.%s"),m_strTmpFolder.operator LPCTSTR(),
					strPathName.operator LPCTSTR(),strExt);
			}
			else
			{
				strFile.Format(_T("%s\\%s_%s.%s"),m_strTmpFolder.operator LPCTSTR(),
					strPathName.operator LPCTSTR(),
					m_agThemeName[i].operator LPCTSTR(),
				strExt);	
			}
		

			IMG_PARAM_T ipt;
			ipt.dwDstSize = pri->itemInfo[i].org.dwSize;
			ipt.pDstData = pri->itemInfo[i].org.pBuf;
			ipt.nImgType = (BYTE)wItemType;
			ipt.nImgCmpType = (BYTE)wCmpType;
			ipt.dwWidth = pri->itemInfo[i].org.wWidth;
			ipt.dwHeight = pri->itemInfo[i].org.wHeight;

			HRESULT hr = m_pRC->ExpImagFile(strFile.operator LPCTSTR(),&ipt);
			if(SUCCEEDED(hr))
			{
				_tcscpy(pri->itemInfo[i].org.szFile,strFile.operator LPCTSTR());
			}

			m_pRC->ReleaseMemory(ipt.pSrcData);
		}		

	}

	return TRUE;

	
}
BOOL CResParser::LoadAnim(RES_ITEM_PTR pri)
{
	if(m_pResBuf == NULL)
	{
		return FALSE;
	}

	for(UINT i =  0; i< m_uThemeNum ; i++)
	{
		BYTE *pBaseBuf = NULL;
		if(m_bThemeOnly)
		{
			pBaseBuf = m_pResBuf;
		}
		else
		{
			DWORD dwThemeOffset = * (DWORD*)(m_pResBuf+ DEFAULT_RES_INFO_OFFSET + i*8); 
			CONV_DWORD_EX(dwThemeOffset);

			pBaseBuf = m_pResBuf + dwThemeOffset;
		}

		CHECK_ORG_PTR(pBaseBuf);

		DWORD dwiMduOffset = * (DWORD*)(pBaseBuf + (20 + pri->uModuleID * 4));
		CONV_DWORD_EX(dwiMduOffset);
		pBaseBuf += dwiMduOffset;

		CHECK_ORG_PTR(pBaseBuf + MMI_RT_ANIM * 12+8);

		BYTE *pAnimInfo = pBaseBuf + MMI_RT_ANIM * 12;
		DWORD dwAnimOffset = * (DWORD*)pAnimInfo;
		DWORD dwAnimNum = * (DWORD*)(pAnimInfo+4);
		DWORD dwAnimSize = * (DWORD*)(pAnimInfo+8);
		CONV_DWORD_EX(dwAnimOffset);
		CONV_DWORD_EX(dwAnimNum);
		CONV_DWORD_EX(dwAnimSize);	

		if(dwAnimNum == 0)
		{
			pri->itemInfo[i].wExist = 0;
			continue;
		}


		_ASSERT(pri->uResIndex < dwAnimNum);
		if(pri->uResIndex >= dwAnimNum)
		{
			return FALSE;
		}

		CHECK_ORG_PTR(pBaseBuf + dwAnimOffset+4);

		DWORD dwAnimFlagOffset = * (DWORD*)(pBaseBuf + dwAnimOffset);
		DWORD dwAnimDataOffset = * (DWORD*)(pBaseBuf + dwAnimOffset+4);
		CONV_DWORD_EX(dwAnimFlagOffset);
		CONV_DWORD_EX(dwAnimDataOffset);

		BYTE *pAnimflagBuf = pBaseBuf+ (dwAnimFlagOffset + pri->uResIndex*sizeof(IMG_FLAG_T));
		CHECK_ORG_PTR(pAnimflagBuf);

		CHECK_ORG_PTR(pBaseBuf+ (dwAnimDataOffset + pri->uResIndex*sizeof(DWORD)));
		DWORD dwAnimDataOfffset =* (DWORD*)(pBaseBuf+ (dwAnimDataOffset + pri->uResIndex*sizeof(DWORD)));
		CONV_DWORD_EX(dwAnimDataOfffset);

		if(dwAnimDataOfffset == 0)
		{
			pri->itemInfo[i].wExist = 0;
			continue;
		}
		else
		{
			pri->itemInfo[i].wExist = 1;
		}

		CHECK_ORG_PTR(pAnimflagBuf+sizeof(ANIM_FLAG_T));		
		ANIM_FLAG_T tAnimFlag = {0};
		memcpy(&tAnimFlag,pAnimflagBuf,sizeof(ANIM_FLAG_T));
		if(m_bBigEndian)
		{
			ConvAnimFlag(tAnimFlag);
		}

		CHECK_ORG_PTR(pBaseBuf+dwAnimDataOfffset+tAnimFlag.nSize);

		pri->itemInfo[i].org.wHeight = tAnimFlag.nHeight;
		pri->itemInfo[i].org.wWidth= tAnimFlag.nWidth;
		pri->itemInfo[i].org.wCmpType = LOBYTE(tAnimFlag.nType);
		pri->itemInfo[i].org.wItemType = HIBYTE(tAnimFlag.nType);
		pri->itemInfo[i].org.dwFrame = tAnimFlag.nFrameNum;
		pri->itemInfo[i].org.dwSize = tAnimFlag.nSize;
		pri->itemInfo[i].org.pBuf = new BYTE[tAnimFlag.nSize];
		
		WORD wItemType = pri->itemInfo[i].org.wItemType;
		WORD wCmpType = pri->itemInfo[i].org.wCmpType;

		memcpy(pri->itemInfo[i].org.pBuf,pBaseBuf+dwAnimDataOfffset,pri->itemInfo[i].org.dwSize);

		if(m_bBigEndian)
		{
			if( wCmpType == ANIM_CMP_BMP_MBMP)
			{
				m_pRC->AnimConvertToLitEndian(pri->itemInfo[i].org.pBuf,
											 pri->itemInfo[i].org.dwSize);

			}
		}

		if(!m_strTmpFolder.IsEmpty())
		{
			CString strFile;
			strFile.Format(_T("%s\\%s_%s"),m_strTmpFolder.operator LPCTSTR(),
				pri->szDesc,
				m_agThemeName[i].operator LPCTSTR());

			ANIM_PARAM_T apt;
			apt.dwDstSize = pri->itemInfo[i].org.dwSize;
			apt.pDstData = pri->itemInfo[i].org.pBuf;
			apt.nImgType = (BYTE)wItemType;
			apt.nAnimCmpType = (BYTE)wCmpType;
			apt.dwHeight     = pri->itemInfo[i].org.wHeight;
			apt.dwWidth      = pri->itemInfo[i].org.wWidth;
			apt.dwFramNum    = pri->itemInfo[i].org.dwFrame;

			HRESULT hr = m_pRC->ExpAnimFolder(strFile.operator LPCTSTR(),&apt);
			if(SUCCEEDED(hr))
			{
				_tcscpy(pri->itemInfo[i].org.szFile,strFile.operator LPCTSTR());
			}
		}
	}

	return TRUE;
}
BOOL CResParser::LoadRing(RES_ITEM_PTR pri)
{
	if(m_pResBuf == NULL)
	{
		return FALSE;
	}

	for(UINT i =  0; i< m_uThemeNum ; i++)
	{
		BYTE *pBaseBuf = NULL;
		if(m_bThemeOnly)
		{
			pBaseBuf = m_pResBuf;
		}
		else
		{
			DWORD dwThemeOffset = * (DWORD*)(m_pResBuf+ DEFAULT_RES_INFO_OFFSET + i*8);
			CONV_DWORD_EX(dwThemeOffset);

			pBaseBuf = m_pResBuf + dwThemeOffset;
		}

		CHECK_ORG_PTR(pBaseBuf);

		DWORD dwiMduOffset = * (DWORD*)(pBaseBuf + (20 + pri->uModuleID * 4));
		CONV_DWORD_EX(dwiMduOffset);
		pBaseBuf += dwiMduOffset;

		CHECK_ORG_PTR(pBaseBuf + MMI_RT_RING * 12 + 8);	

		BYTE *pRingInfo = pBaseBuf + MMI_RT_RING * 12;
		DWORD dwRingOffset = * (DWORD*)pRingInfo;
		DWORD dwRingNum = * (DWORD*)(pRingInfo+4);
		DWORD dwRingSize = * (DWORD*)(pRingInfo+8);
		CONV_DWORD_EX(dwRingOffset);
		CONV_DWORD_EX(dwRingNum);
		CONV_DWORD_EX(dwRingSize);

		if(dwRingNum == 0)
		{
			pri->itemInfo[i].wExist = 0;
			continue;
		}

		_ASSERT(pri->uResIndex < dwRingNum);
		if(pri->uResIndex >= dwRingNum)
		{
			return FALSE;
		}


		BYTE *pRingInfoBuf = pBaseBuf+ (dwRingOffset + pri->uResIndex*sizeof(MMIAUD_RING_DATA_INFO_T));
		CHECK_ORG_PTR(pRingInfoBuf+sizeof(MMIAUD_RING_DATA_INFO_T));

		MMIAUD_RING_DATA_INFO_T tRingFlag;
		memcpy(&tRingFlag,pRingInfoBuf,sizeof(tRingFlag));
		if(m_bBigEndian)
		{
			ConvRingFlag(tRingFlag);
		}

		DWORD dwRingDataOfffset = (DWORD)(tRingFlag.data_ptr);	

		if(dwRingDataOfffset == 0)
		{
			pri->itemInfo[i].wExist = 0;
			continue;
		}
		else
		{
			pri->itemInfo[i].wExist = 1;
		}

		CHECK_ORG_PTR(pBaseBuf+dwRingDataOfffset+tRingFlag.data_len);
		
		pri->itemInfo[i].org.wItemType = (WORD)tRingFlag.type;
		//pri->wItemtype = HIBYTE(tRingFlag.nType);
		pri->itemInfo[i].org.pBuf = new BYTE[tRingFlag.data_len];
		pri->itemInfo[i].org.dwSize = tRingFlag.data_len;

		memcpy(pri->itemInfo[i].org.pBuf,pBaseBuf+dwRingDataOfffset,pri->itemInfo[i].org.dwSize);

		CString strFile;
		CString strPathName = pri->szDesc;
		if(m_bCscs)
		{
			int nFind = strPathName.ReverseFind('\\');
			if(nFind != -1)
			{
				CString strPath;
				strPath.Format(_T("%s\\%s"),m_strTmpFolder.operator LPCTSTR(),strPathName.Left(nFind));
				g_theApp.CreateDeepDirectory(strPath);
			}

			strFile.Format(_T("%s\\%s"),m_strTmpFolder.operator LPCTSTR(),
				strPathName.operator LPCTSTR());

			CFileFind finder;
			if(finder.FindFile(strFile))
			{
				DWORD dwAtt = GetFileAttributes(strFile);
				dwAtt &= ~FILE_ATTRIBUTE_READONLY;
				SetFileAttributes(strFile,dwAtt);
			}
			FILE *pFile = _tfopen(strFile,_T("wb"));
			fwrite(pri->itemInfo[i].org.pBuf,pri->itemInfo[i].org.dwSize,1,pFile);
			fclose(pFile);

			_tcscpy(pri->itemInfo[i].org.szFile,strFile.operator LPCTSTR());
		}
	}

	return TRUE;
}

BOOL CResParser::LoadData(RES_ITEM_PTR pri)
{
	if(m_pResBuf == NULL)
	{
		return FALSE;
	}

	for(UINT i =  0; i< m_uThemeNum ; i++)
	{
		BYTE *pBaseBuf = NULL;
		if(m_bThemeOnly)
		{
			pBaseBuf = m_pResBuf;
		}
		else
		{
			DWORD dwThemeOffset = * (DWORD*)(m_pResBuf+ DEFAULT_RES_INFO_OFFSET + i*8);
			CONV_DWORD_EX(dwThemeOffset);

			pBaseBuf = m_pResBuf + dwThemeOffset;
		}

		CHECK_ORG_PTR(pBaseBuf);

		DWORD dwiMduOffset = * (DWORD*)(pBaseBuf + (20 + pri->uModuleID * 4));
		CONV_DWORD_EX(dwiMduOffset);
		pBaseBuf += dwiMduOffset;

		CHECK_ORG_PTR(pBaseBuf + MMI_RT_OTHER * 12 + 8);	

		BYTE *pOtherDataInfo = pBaseBuf + MMI_RT_OTHER * 12;
		DWORD dwOtherDataOffset = * (DWORD*)pOtherDataInfo;
		DWORD dwOtherDataNum = * (DWORD*)(pOtherDataInfo+4);
		DWORD dwOtherDataSize = * (DWORD*)(pOtherDataInfo+8);
		CONV_DWORD_EX(dwOtherDataOffset);
		CONV_DWORD_EX(dwOtherDataNum);
		CONV_DWORD_EX(dwOtherDataSize);

		if(dwOtherDataNum == 0)
		{
			pri->itemInfo[i].wExist = 0;
			continue;
		}

		_ASSERT(pri->uResIndex < dwOtherDataNum);
		if(pri->uResIndex >= dwOtherDataNum)
		{
			return FALSE;
		}


		BYTE *pOtherDataInfoBuf = pBaseBuf+ (dwOtherDataOffset + pri->uResIndex*sizeof(OTHER_DATA_INFO_T));
		CHECK_ORG_PTR(pOtherDataInfoBuf+sizeof(OTHER_DATA_INFO_T));

		OTHER_DATA_INFO_T tDataFlag;
		memcpy(&tDataFlag,pOtherDataInfoBuf,sizeof(tDataFlag));
		if(m_bBigEndian)
		{
			ConvDataFlag(tDataFlag);
		}

		DWORD dwOtherDataOfffset = (DWORD)(tDataFlag.data_ptr);	

		if(dwOtherDataOfffset == 0)
		{
			pri->itemInfo[i].wExist = 0;
			continue;
		}
		else
		{
			pri->itemInfo[i].wExist = 1;
		}

		CHECK_ORG_PTR(pBaseBuf+dwOtherDataOfffset+tDataFlag.data_len);
		
		pri->itemInfo[i].org.wItemType = (WORD)tDataFlag.type;
		//pri->wItemtype = HIBYTE(tRingFlag.nType);
		pri->itemInfo[i].org.pBuf = new BYTE[tDataFlag.data_len];
		pri->itemInfo[i].org.dwSize = tDataFlag.data_len;

		memcpy(pri->itemInfo[i].org.pBuf,pBaseBuf+dwOtherDataOfffset,pri->itemInfo[i].org.dwSize);

		CString strFile;
		CString strPathName = pri->szDesc;
		if(m_bCscs)
		{
			int nFind = strPathName.ReverseFind('\\');
			if(nFind != -1)
			{
				CString strPath;
				strPath.Format(_T("%s\\%s"),m_strTmpFolder.operator LPCTSTR(),strPathName.Left(nFind));
				g_theApp.CreateDeepDirectory(strPath);
			}
			
			strFile.Format(_T("%s\\%s"),m_strTmpFolder.operator LPCTSTR(),
				strPathName.operator LPCTSTR());
			
			CFileFind finder;
			if(finder.FindFile(strFile))
			{
				DWORD dwAtt = GetFileAttributes(strFile);
				dwAtt &= ~FILE_ATTRIBUTE_READONLY;
				SetFileAttributes(strFile,dwAtt);
			}
			FILE *pFile = _tfopen(strFile,_T("wb"));
			fwrite(pri->itemInfo[i].org.pBuf,pri->itemInfo[i].org.dwSize,1,pFile);
			fclose(pFile);
			
			_tcscpy(pri->itemInfo[i].org.szFile,strFile.operator LPCTSTR());
		}
	}

	return TRUE;
}

BOOL CResParser::LoadColor(RES_ITEM_PTR pri)
{
	if(m_pResBuf == NULL)
	{
		return FALSE;
	}

	for(UINT i =  0; i< m_uThemeNum ; i++)
	{
		BYTE *pBaseBuf = NULL;
		if(m_bThemeOnly)
		{
			pBaseBuf = m_pResBuf;
		}
		else
		{
			DWORD dwThemeOffset = * (DWORD*)(m_pResBuf+ DEFAULT_RES_INFO_OFFSET + i*8);
			CONV_DWORD_EX(dwThemeOffset);

			pBaseBuf = m_pResBuf + dwThemeOffset;
		}

		CHECK_ORG_PTR(pBaseBuf);

		DWORD dwiMduOffset = * (DWORD*)(pBaseBuf + (20 + pri->uModuleID * 4));
		CONV_DWORD_EX(dwiMduOffset);
		pBaseBuf += dwiMduOffset;

		CHECK_ORG_PTR(pBaseBuf + MMI_RT_COLOR * 12 + 8);

		BYTE *pColorInfo = pBaseBuf + MMI_RT_COLOR * 12;
		DWORD dwColorOffset = * (DWORD*)pColorInfo;
		DWORD dwColorNum = * (DWORD*)(pColorInfo+4);
		DWORD dwColorSize = * (DWORD*)(pColorInfo+8);
		CONV_DWORD_EX(dwColorOffset);
		CONV_DWORD_EX(dwColorNum);
		CONV_DWORD_EX(dwColorSize);

		if(dwColorNum == 0)
		{
			pri->itemInfo[i].wExist = 0;
			continue;
		}

		_ASSERT(pri->uResIndex < dwColorNum);
		if(pri->uResIndex >= dwColorNum)
		{
			return FALSE;
		}


		BYTE *pColorInfoBuf = pBaseBuf+ (dwColorOffset + pri->uResIndex*sizeof(MMI_COLOR_T));

		CHECK_ORG_PTR(pColorInfoBuf);


		MMI_COLOR_T tColorFlag;
		memcpy(&tColorFlag,pColorInfoBuf,sizeof(tColorFlag));
		if(m_bBigEndian)
		{
			ConvColorFlag(tColorFlag);
		}	
		
		CHECK_ORG_PTR(pColorInfoBuf + sizeof(MMI_COLOR_T));
		pri->itemInfo[i].wExist = tColorFlag.bValidate;	
		pri->itemInfo[i].org.dwSize = sizeof(MMI_COLOR_T);
		pri->itemInfo[i].org.dwColor = tColorFlag.dwColor;
		pri->itemInfo[i].org.pBuf = new BYTE[pri->itemInfo[i].org.dwSize];
		memcpy(pri->itemInfo[i].org.pBuf,pColorInfoBuf,pri->itemInfo[i].org.dwSize );		
	
	}

	return TRUE;
}

BOOL CResParser::RelaceImag(int nItemIndex,int nThemeIndex, LPCTSTR lpszImagFile)
{
	m_strErrMsg.Empty();
	if(m_pRC == NULL || lpszImagFile == NULL)
		return FALSE;

	UndoResItem(nItemIndex,nThemeIndex);	
	IMG_PARAM_T ipt;
	ipt.nImgCmpType = (BYTE)m_vResItem[nItemIndex]->itemInfo[nThemeIndex].org.wCmpType;

	if( (ipt.nImgCmpType == IMG_CMP_JPG_SCI || 
		ipt.nImgCmpType == IMG_CMP_BMP_OPT || 
		ipt.nImgCmpType == IMG_CMP_BMP_SJPG )&& 
		m_vResItem[nItemIndex]->itemInfo[nThemeIndex].org.pBuf[0] == 'S' &&
		m_vResItem[nItemIndex]->itemInfo[nThemeIndex].org.pBuf[1] == 'J' &&
		m_vResItem[nItemIndex]->itemInfo[nThemeIndex].org.pBuf[2] == 'P' &&
		m_vResItem[nItemIndex]->itemInfo[nThemeIndex].org.pBuf[3] == 'G')
	{
		WORD wYUVType = *(WORD*)(m_vResItem[nItemIndex]->itemInfo[nThemeIndex].org.pBuf+8);
		wYUVType = CONV_SHORT(wYUVType);
		ipt.nYUVType = (BYTE)wYUVType;

		WORD wQL = *(WORD*)(m_vResItem[nItemIndex]->itemInfo[nThemeIndex].org.pBuf+10);
		wQL = CONV_SHORT(wQL);
		ipt.nQualityType = (BYTE)wQL;
	}
	else
	{
		ipt.nYUVType = JPG_YUV422;
		ipt.nQualityType = JPG_Q95;
	}
	
	HRESULT hr = m_pRC->LoadImagFile(lpszImagFile,&ipt);
	if(FAILED(hr))
	{
		m_pRC->ReleaseMemory(ipt.pDstData);
		m_pRC->ReleaseMemory(ipt.pSrcData);

		if(hr == X_E_NO_ALPHA)
		{
			m_strErrMsg.Format(_T("[ERROR]: There is no alpha in this png file [%s]!\nPlease change to another one with alpha.\n"),lpszImagFile);
		}
		else if(hr == X_E_COLOR_FULL)
		{
			m_strErrMsg.Format(_T("[ERROR]: There is too much color in this png file [%s]!\nPlease change to another one with less color [65535].\n"),lpszImagFile);
		}
		else
		{
			m_strErrMsg.Format(_T("[ERROR]: Load image file failed, [%s]!\nMaybe it is not a png file!\n"),lpszImagFile);
		}		
		SHOW_MESSAGE(m_strErrMsg,TRUE);
		return FALSE;
	}
	
	RES_ITEM_PTR pRI= m_vResItem[nItemIndex];

	if(m_bCheckImagWH)
	{
		if( ipt.dwHeight != pRI->itemInfo[nThemeIndex].org.wHeight ||
			ipt.dwWidth != pRI->itemInfo[nThemeIndex].org.wWidth )
		{
			m_pRC->ReleaseMemory(ipt.pDstData);
			m_pRC->ReleaseMemory(ipt.pSrcData);
			m_strErrMsg.Format(_T("[ERROR]: The width/height[%d/%d] of the image file [%s] does not match[%d/%d]!\n"),
				ipt.dwWidth,ipt.dwHeight,lpszImagFile,
				pRI->itemInfo[nThemeIndex].org.wWidth,pRI->itemInfo[nThemeIndex].org.wHeight);			
			SHOW_MESSAGE(m_strErrMsg,TRUE);
			return FALSE;
		}
	}
	
	if( ipt.dwDstSize == pRI->itemInfo[nThemeIndex].org.dwSize &&
		memcmp(ipt.pDstData,pRI->itemInfo[nThemeIndex].org.pBuf,ipt.dwDstSize)==0 )
	{
		m_pRC->ReleaseMemory(ipt.pDstData);
		m_pRC->ReleaseMemory(ipt.pSrcData);
		m_strErrMsg.Format(_T("[Warning]: Image file [%s] is the same as the original image file!\n"),lpszImagFile);			
		SHOW_MESSAGE(m_strErrMsg,m_bShowWarning);	
		return TRUE;
	}

	pRI->itemInfo[nThemeIndex].rep.wHeight = (WORD)ipt.dwHeight;
	pRI->itemInfo[nThemeIndex].rep.wWidth = (WORD)ipt.dwWidth;
	pRI->itemInfo[nThemeIndex].rep.wCmpType = (WORD)ipt.nImgCmpType;
	pRI->itemInfo[nThemeIndex].rep.wItemType = (WORD)ipt.nImgType;

	pRI->itemInfo[nThemeIndex].rep.pBuf = new BYTE[ipt.dwDstSize];
	memcpy(pRI->itemInfo[nThemeIndex].rep.pBuf, ipt.pDstData,ipt.dwDstSize);

	pRI->itemInfo[nThemeIndex].rep.dwSize = ipt.dwDstSize;
	pRI->itemInfo[nThemeIndex].wModified = TRUE;
	pRI->isModified = TRUE;
	_tcscpy(pRI->itemInfo[nThemeIndex].rep.szFile,lpszImagFile);

	m_nNewSize += (int)(ALIGN4_SIZE(ipt.dwDstSize) - ALIGN4_SIZE(pRI->itemInfo[nThemeIndex].org.dwSize));

	m_pRC->ReleaseMemory(ipt.pDstData);
	m_pRC->ReleaseMemory(ipt.pSrcData);
	

	return TRUE;
}
BOOL CResParser::RelaceAnim(int nItemIndex,int nThemeIndex, LPCTSTR lpszAnimFolder)
{
	m_strErrMsg.Empty();
	if(m_pRC == NULL || lpszAnimFolder == NULL)
		return FALSE;

	UndoResItem(nItemIndex,nThemeIndex);
	
	ANIM_PARAM_T apt;
	apt.nAnimCmpType = (BYTE)m_vResItem[nItemIndex]->itemInfo[nThemeIndex].org.wCmpType;
	
	HRESULT hr = m_pRC->LoadAnimFolder(lpszAnimFolder,&apt);
	if(FAILED(hr))
	{
		m_pRC->ReleaseMemory(apt.pDstData);
		m_strErrMsg.Format(_T("Load anim folder failed!%s\n"),ANIM_TIPS);
		SHOW_MESSAGE(m_strErrMsg,TRUE);
		return FALSE;
	}

	RES_ITEM_PTR pRI= m_vResItem[nItemIndex];

	if(m_bCheckAnimWH)
	{
		if( apt.dwHeight != pRI->itemInfo[nThemeIndex].org.wHeight ||
			apt.dwWidth != pRI->itemInfo[nThemeIndex].org.wWidth )
		{
			m_pRC->ReleaseMemory(apt.pDstData);
			m_strErrMsg.Format(_T("Anim width/height does not match!\n"));		
			SHOW_MESSAGE(m_strErrMsg,TRUE);
			return FALSE;
		}
	}

	if(m_bCheckAnimFrame)
	{
		if( apt.dwFramNum != pRI->itemInfo[nThemeIndex].org.dwFrame)
		{
			m_pRC->ReleaseMemory(apt.pDstData);
			m_strErrMsg.Format(_T("Anim frame number does not match!"));	
			SHOW_MESSAGE(m_strErrMsg,TRUE);
			return FALSE;
		}
	}


	pRI->itemInfo[nThemeIndex].rep.wHeight = (WORD)apt.dwHeight;
	pRI->itemInfo[nThemeIndex].rep.wWidth = (WORD)apt.dwWidth;
	pRI->itemInfo[nThemeIndex].rep.dwFrame = (WORD)apt.dwFramNum;

	pRI->itemInfo[nThemeIndex].rep.wCmpType = (WORD)apt.nAnimCmpType;
	pRI->itemInfo[nThemeIndex].rep.wItemType = (WORD)apt.nImgType;

	pRI->itemInfo[nThemeIndex].rep.pBuf = new BYTE[apt.dwDstSize];
	memcpy(pRI->itemInfo[nThemeIndex].rep.pBuf, apt.pDstData,apt.dwDstSize);

	pRI->itemInfo[nThemeIndex].rep.dwSize = apt.dwDstSize;
	pRI->itemInfo[nThemeIndex].wModified = TRUE;
	pRI->isModified = TRUE;
	_tcscpy(pRI->itemInfo[nThemeIndex].rep.szFile,lpszAnimFolder);

	m_nNewSize += (int)(ALIGN4_SIZE(apt.dwDstSize) - ALIGN4_SIZE(pRI->itemInfo[nThemeIndex].org.dwSize));

	m_pRC->ReleaseMemory(apt.pDstData);

	return TRUE;
}
BOOL CResParser::RelaceRing(int nItemIndex,int nThemeIndex, LPCTSTR lpszRingFile)
{
	USES_CONVERSION;
	m_strErrMsg.Empty();

	if(m_pRC == NULL || lpszRingFile == NULL)
		return FALSE;
	UndoResItem(nItemIndex,nThemeIndex);


	RES_ITEM_PTR pRI = m_vResItem[nItemIndex];
	if(pRI->itemInfo[nThemeIndex].wExist == 0)
	{
		return FALSE;
	}

	CString strErr;

	X_RING_T xrt;
	
	HRESULT hr = m_pRC->LoadRingFile(lpszRingFile,(LPVOID )&xrt);
	if(FAILED(hr))
	{
		m_pRC->ReleaseXRingT((LPVOID )&xrt);		
		m_strErrMsg.Format(_T("[Error]: Load ring file [%s] failed!\nCan not open it!\n"),lpszRingFile);
		SHOW_MESSAGE(m_strErrMsg,TRUE);
		return FALSE;
	}	

	if(m_bCheckRingType && xrt.wType != MMIAUDIO_TYPE_MAX)
	{
		if(xrt.wType != pRI->itemInfo[nThemeIndex].org.wItemType)
		{
			m_pRC->ReleaseXRingT((LPVOID )&xrt);
			m_strErrMsg.Format(_T("[Error]: Ring type of ring file [%s] does not match the orininal!\n"),lpszRingFile);
			SHOW_MESSAGE(m_strErrMsg,TRUE);
			return FALSE;
		}
	}

	CString strNewRingName = lpszRingFile;
	int nFind = strNewRingName.ReverseFind('\\');
	if(nFind != -1)
		strNewRingName = strNewRingName.Right(strNewRingName.GetLength()-nFind-1);
	
	CString strDesc = pRI->szDesc;
	CString strOldRingName = pRI->szDesc;
	nFind = strDesc.ReverseFind('\\');
	if(nFind != -1)
	{
		strOldRingName = strDesc.Right(strDesc.GetLength()-nFind-1);
		strDesc = strDesc.Left(nFind+1);
	}	


	if( xrt.dwResSize == pRI->itemInfo[nThemeIndex].org.dwSize &&
		memcmp(xrt.pResStream,pRI->itemInfo[nThemeIndex].org.pBuf,xrt.dwResSize)==0 &&
		strNewRingName.CompareNoCase(strOldRingName) == 0)
	{
		m_pRC->ReleaseXRingT((LPVOID )&xrt);
		
		m_strErrMsg.Format(_T("[Warning]: Ring file [%s] is the same as the original ring file!\n"),lpszRingFile);			
		SHOW_MESSAGE(m_strErrMsg,m_bShowWarning);

		return TRUE;
	}

	pRI->itemInfo[nThemeIndex].rep.wItemType = xrt.wType;

	pRI->itemInfo[nThemeIndex].rep.pBuf = new BYTE[xrt.dwResSize];
	memcpy(pRI->itemInfo[nThemeIndex].rep.pBuf, xrt.pResStream,xrt.dwResSize);

	pRI->itemInfo[nThemeIndex].rep.dwSize = xrt.dwResSize;
	pRI->itemInfo[nThemeIndex].wModified = TRUE;
	pRI->isModified = TRUE;
	_tcscpy(pRI->itemInfo[nThemeIndex].rep.szFile,lpszRingFile);

	m_nNewSize += (int)(ALIGN4_SIZE(xrt.dwResSize) - ALIGN4_SIZE(pRI->itemInfo[nThemeIndex].org.dwSize));

	m_pRC->ReleaseXRingT((LPVOID )&xrt);

	if(strNewRingName.CompareNoCase(strOldRingName) != 0)
	{
		strDesc += strNewRingName;		

		CString strExt = _T("");
		int nFind = strDesc.ReverseFind('.');
		if(nFind != -1)
		{
			strExt = strDesc.Right(strDesc.GetLength()-nFind);
			strDesc = strDesc.Left(nFind);
		}
		

		RES_ITEM_PTR pRI2= NULL;
		UINT nSize = m_vResItem.size();
		int nNext = 0;

		UINT i=0;
		CString strDescTmp = strDesc;
		do 
		{				
			
			CString strTmp= strDesc + strExt;

			//strDesc = strDescTmp;			
			nNext++;
			CString strNext;
			strNext.Format(_T("%d"),nNext);

			for(i=0;i<nSize; i++)
			{
				pRI2 = m_vResItem[i];

				if(i == (UINT)nItemIndex || pRI2->uResType != MMI_RT_RING || pRI2->itemInfo[0].wExist==0)
					continue;
				
				if(_tcslen(pRI2->szNewDesc)!=0)
				{
					if(strTmp.CompareNoCase(pRI2->szNewDesc)==0)
					{
						strDesc = strDescTmp + strNext;
						break;
					}
				}
				else
				{
					if(strTmp.CompareNoCase(pRI2->szDesc)==0)
					{					
						strDesc = strDescTmp + strNext;	
						break;
					}
				}			
			}

		} while(i<nSize);


		strDesc += strExt;

		_tcscpy(pRI->szNewDesc,strDesc);
	}
	
	


	return TRUE;
}
BOOL CResParser::RelaceData(int nItemIndex,int nThemeIndex, LPCTSTR lpszDataFile)
{
	m_strErrMsg.Empty();
	if(m_pRC == NULL || lpszDataFile == NULL)
		return FALSE;
	UndoResItem(nItemIndex,nThemeIndex);

	X_OTHER_T xdt;
	
	HRESULT hr = m_pRC->LoadOthrFile(lpszDataFile,(LPVOID )&xdt);
	if(FAILED(hr))
	{
		m_pRC->ReleaseXOthrT((LPVOID )&xdt);
		m_strErrMsg.Format(_T("[Error]: Load data file [%s] failed!\n"),lpszDataFile);
		SHOW_MESSAGE(m_strErrMsg,TRUE);
		return FALSE;
	}

	RES_ITEM_PTR pRI = m_vResItem[nItemIndex];

	if( xdt.dwResSize == pRI->itemInfo[nThemeIndex].org.dwSize &&
		memcmp(xdt.pResStream,pRI->itemInfo[nThemeIndex].org.pBuf,xdt.dwResSize)==0)
	{
		m_pRC->ReleaseXOthrT((LPVOID )&xdt);
	
		m_strErrMsg.Format(_T("[Warning]: Data file [%s] is the same as the original data file!\n"),lpszDataFile);			
		SHOW_MESSAGE(m_strErrMsg,m_bShowWarning);

		return TRUE;
	}	

	pRI->itemInfo[nThemeIndex].rep.wItemType = xdt.wType;

	pRI->itemInfo[nThemeIndex].rep.pBuf = new BYTE[xdt.dwResSize];
	memcpy(pRI->itemInfo[nThemeIndex].rep.pBuf, xdt.pResStream,xdt.dwResSize);

	pRI->itemInfo[nThemeIndex].rep.dwSize = xdt.dwResSize;
	pRI->itemInfo[nThemeIndex].wModified = TRUE;
	pRI->isModified = TRUE;
	_tcscpy(pRI->itemInfo[nThemeIndex].rep.szFile,lpszDataFile);

	m_nNewSize += (int)(ALIGN4_SIZE(xdt.dwResSize) - ALIGN4_SIZE(pRI->itemInfo[nThemeIndex].org.dwSize));
	
	m_pRC->ReleaseXOthrT((LPVOID )&xdt);

	//CString strTmp = pRI->szDesc;
	//if(strTmp.Right(4).CompareNoCase(_T(".ldb")) == 0)
	//	ModifyIM();

	return TRUE;
}

BOOL CResParser::RelaceColor(int nItemIndex,int nThemeIndex, DWORD dwColor)
{
	if(m_pRC == NULL)
		return FALSE;

	UndoResItem(nItemIndex,nThemeIndex);

	RES_ITEM_PTR pRI = m_vResItem[nItemIndex];
	pRI->itemInfo[nThemeIndex].rep.pBuf = new BYTE[sizeof(MMI_COLOR_T)];
	pRI->itemInfo[nThemeIndex].rep.dwSize = sizeof(MMI_COLOR_T);

	memcpy(pRI->itemInfo[nThemeIndex].rep.pBuf,pRI->itemInfo[nThemeIndex].org.pBuf,sizeof(MMI_COLOR_T));

	pRI->itemInfo[nThemeIndex].rep.dwColor = dwColor;
	DWORD dwTmp = dwColor;
	CONV_DWORD_EX(dwTmp);

	*(DWORD*)(pRI->itemInfo[nThemeIndex].rep.pBuf) = dwTmp;

	pRI->itemInfo[nThemeIndex].wModified = TRUE;
	pRI->isModified = TRUE;
	
	return TRUE;
}

BOOL CResParser::SaveResItem(int nItemIndex,int nThemeIndex)
{
	RES_ITEM_PTR pRI = m_vResItem[nItemIndex];
	if(pRI->itemInfo[nThemeIndex].wModified == 0)
	{
		return TRUE;
	}

	BOOL bOK = FALSE;
	switch(pRI->uResType)
	{
	case MMI_RT_IMAGE:
		if(pRI->bLogo)
		{
			if(nThemeIndex == 0)
			{
				bOK = SaveLogo(nItemIndex);
			}
			else
			{
				bOK = TRUE;
			}
		}
		else
		{
			bOK = SaveImage(nItemIndex,nThemeIndex);
		}
		break;
	case MMI_RT_ANIM:
		bOK = SaveAnim(nItemIndex,nThemeIndex);
		break;
	case MMI_RT_RING:
		bOK = SaveRing(nItemIndex,nThemeIndex);
	    break;
	case MMI_RT_COLOR:
		bOK = SaveColor(nItemIndex,nThemeIndex);
	    break;
	case MMI_RT_OTHER:
		bOK = SaveData(nItemIndex,nThemeIndex);
	    break;
	case MMI_RT_MAX_NUM:
		if( nThemeIndex == 0)
		{			
			bOK = EmptyLangText(nItemIndex,nThemeIndex);	
		}
		else
		{
			bOK = TRUE;
		}

	    break;
	default:
	    break;
	}
	return bOK;
}
BOOL CResParser::SaveLogo(int nItemIndex)
{
	if(m_pNewBuf == NULL)
	{
		return FALSE;
	}
	RES_ITEM_PTR pRI = m_vResItem[nItemIndex];

	int nDeltAlign =  (int)ALIGN4_SIZE(pRI->itemInfo[0].rep.dwSize) - 
		              (int)ALIGN4_SIZE(pRI->itemInfo[0].org.dwSize);

	LPBYTE pCurBuf = NULL;
	if(m_bThemeOnly)
	{
		DWORD dwTotalSize = *(DWORD*)(m_pNewBuf+16);
		CONV_DWORD_EX(dwTotalSize);
		pCurBuf = m_pNewBuf + dwTotalSize + pRI->uResIndex*12;

	}
	else
	{
		pCurBuf = m_pNewBuf + LOGO_OFFSET + pRI->uResIndex*12;
	} 

	DWORD dwOffset = *(DWORD*)(pCurBuf+4);
	DWORD dwSize = *(DWORD*)(pCurBuf+8);
	CONV_DWORD_EX(dwOffset);
	CONV_DWORD_EX(dwSize);

	DWORD dwSizeTmp = 0;
	

	if(nDeltAlign != 0)
	{
		ModifyTablOffset(nDeltAlign,pRI->uResIndex+1);

		dwSizeTmp = pRI->itemInfo[0].rep.dwSize;
		CONV_DWORD_EX(dwSizeTmp);
		*(DWORD*)(pCurBuf+8) = dwSizeTmp;

		DWORD dwTotalOffset = dwOffset+ALIGN4_SIZE(dwSize);
		
		//修改 Imag Flag
		IMG_FLAG_T imgFlag = {0};
		imgFlag.nHeight = pRI->itemInfo[0].rep.wHeight;
		imgFlag.nWidth  = pRI->itemInfo[0].rep.wWidth;
		imgFlag.nType   = MAKEWORD((BYTE)pRI->itemInfo[0].rep.wCmpType,(BYTE)pRI->itemInfo[0].rep.wItemType);
		WORD wOther = *(WORD*)(m_pNewBuf + dwTotalOffset + 6);
		CONV_SHORT_EX(wOther);
		imgFlag.nOtherInfo = wOther;
		imgFlag.nSize = pRI->itemInfo[0].rep.dwSize;
		if(m_bBigEndian)
		{
			ConvImagFlag(imgFlag);
		}
		
		//*(DWORD*)(m_pNewBuf + dwTotalOffset + 8) = dwSizeTmp;
		memcpy(m_pNewBuf + dwTotalOffset,&imgFlag,sizeof(IMG_FLAG_T));
	
		memmove(m_pNewBuf+(dwTotalOffset+ (DWORD)nDeltAlign),
			    m_pNewBuf+ dwTotalOffset,
				m_dwCurTotalSize - dwTotalOffset);		
		memset(m_pNewBuf+dwOffset,0, ALIGN4_SIZE(pRI->itemInfo[0].rep.dwSize));

	}

	WORD wCmpType = pRI->itemInfo[0].rep.wCmpType;

	if(m_bBigEndian)
	{
		if( wCmpType == IMG_CMP_BMP_16_565_RAW || wCmpType == IMG_CMP_BMP_16_565_RLE||
			wCmpType == IMG_CMP_BMP_OSD        || wCmpType == IMG_CMP_BMP_8_RAW ||
			wCmpType == IMG_CMP_BMP_8_SCI)
		{
			m_pRC->BmpConvertToBigEndian(pRI->itemInfo[0].rep.pBuf,
				                         pRI->itemInfo[0].rep.dwSize,
										 wCmpType,
										 wCmpType);

		}
	}

	memcpy(m_pNewBuf + dwOffset, pRI->itemInfo[0].rep.pBuf, pRI->itemInfo[0].rep.dwSize);

	if(m_bBigEndian)
	{
		if( wCmpType == IMG_CMP_BMP_16_565_RAW || wCmpType == IMG_CMP_BMP_16_565_RLE||
			wCmpType == IMG_CMP_BMP_OSD        || wCmpType == IMG_CMP_BMP_8_RAW ||
			wCmpType == IMG_CMP_BMP_8_SCI)
		{
			m_pRC->BmpConvertToLitEndian(pRI->itemInfo[0].rep.pBuf,
				                         pRI->itemInfo[0].rep.dwSize,
										 wCmpType,
										 wCmpType);

		}
	}

	m_dwCurTotalSize += (DWORD)nDeltAlign;


	return TRUE;
}
BOOL CResParser::SaveImage(int nItemIndex,int nThemeIndex)
{
	if(m_pNewBuf == NULL)
	{
		return FALSE;
	}

	RES_ITEM_PTR pRI = m_vResItem[nItemIndex];
	if(pRI->itemInfo[nThemeIndex].wExist == 0 ||
	   (pRI->itemInfo[nThemeIndex].wModified == 0 && 
	    !pRI->itemInfo[nThemeIndex].bNotUsed))
	{
		return TRUE;
	}

	DWORD dwOrgSize = pRI->itemInfo[nThemeIndex].org.dwSize;
	DWORD dwRepSize = pRI->itemInfo[nThemeIndex].rep.dwSize;

	int nDeltAlign =  (int)ALIGN4_SIZE(dwRepSize) - (int)ALIGN4_SIZE(dwOrgSize);

	DWORD dwOffsetTmp =0;
	DWORD dwSizeTmp =0;
	int i = 0;

	BYTE *pBaseBuf = MidifyHdrOffset(nDeltAlign,nThemeIndex,pRI);	

	// 记录没有改变编译以前的IMG数据
	DWORD dwImgOffset = * (DWORD*)pBaseBuf;
	DWORD dwImgNum = * (DWORD*)(pBaseBuf+4);
	DWORD dwImgSize = * (DWORD*)(pBaseBuf+8);
	CONV_DWORD_EX(dwImgOffset);
	CONV_DWORD_EX(dwImgNum);
	CONV_DWORD_EX(dwImgSize);
	
	_ASSERT(pRI->uResIndex < dwImgNum);
	
	DWORD dwImgFlagOffset = * (DWORD*)(pBaseBuf + dwImgOffset);
	DWORD dwImgDataOffset = * (DWORD*)(pBaseBuf + dwImgOffset+4);
	CONV_DWORD_EX(dwImgFlagOffset);
	CONV_DWORD_EX(dwImgDataOffset);
	
	BYTE *pImgflagBuf = pBaseBuf+ (dwImgFlagOffset + pRI->uResIndex*sizeof(IMG_FLAG_T));
	
	DWORD imgDataOfffset =* (DWORD*)(pBaseBuf+ (dwImgDataOffset + pRI->uResIndex*sizeof(DWORD)));
	CONV_DWORD_EX(imgDataOfffset);


	// 先修改偏移，然后再移动内存，插入数据
	if(nDeltAlign != 0)
	{
		// 修改IMG 偏移	
		ModifyImagOffset(pBaseBuf,nDeltAlign,pRI->uResIndex + 1);
		// 修改Anim 偏移
		ModifyAnimOffset(pBaseBuf,nDeltAlign,0);
		// 修改RING 偏移
		ModifyRingOffset(pBaseBuf,nDeltAlign,0);
		// 修改TEXT 偏移
		ModifyTextOffset(pBaseBuf,nDeltAlign,0);
		// 修改FONT偏移
		ModifyFontOffset(pBaseBuf,nDeltAlign,0);		
		// 修改Data 偏移
		ModifyDataOffset(pBaseBuf,nDeltAlign,0);

		// 修改资源类型偏移	
		for( i = MMI_RT_ANIM ; i< MMI_RT_MAX_NUM; i++)
		{
			dwOffsetTmp = *(DWORD*)(pBaseBuf + i*12);
			CONV_DWORD_EX(dwOffsetTmp);
			if(dwOffsetTmp != 0)
			{
				dwOffsetTmp += (DWORD)nDeltAlign;
				CONV_DWORD_EX(dwOffsetTmp);
				*(DWORD*)(pBaseBuf + i*12) = dwOffsetTmp;
			}
		}
		// 修改IMG 的总size
		dwSizeTmp = dwImgSize;
		if(dwSizeTmp != 0)
		{
			dwSizeTmp += (DWORD)nDeltAlign;
			CONV_DWORD_EX(dwSizeTmp);
			*(DWORD*)(pBaseBuf+8) = dwSizeTmp;
		}
//	    //修改IMG Flag 中的Size
// 		dwSizeTmp = dwRepSize;
// 		CONV_DWORD_EX(dwSizeTmp);
// 		*(DWORD*)(pImgflagBuf+ 8) = dwSizeTmp;

		//修改 Imag Flag
		IMG_FLAG_T imgFlag = {0};
		if(pRI->itemInfo[nThemeIndex].wModified && !pRI->itemInfo[nThemeIndex].bNotUsed)
		{
			imgFlag.nHeight = pRI->itemInfo[nThemeIndex].rep.wHeight;
			imgFlag.nWidth  = pRI->itemInfo[nThemeIndex].rep.wWidth;
			imgFlag.nType   = MAKEWORD((BYTE)pRI->itemInfo[nThemeIndex].rep.wCmpType,(BYTE)pRI->itemInfo[nThemeIndex].rep.wItemType);
			WORD wOther = *(WORD*)(pImgflagBuf + 6);
			CONV_SHORT_EX(wOther);
			imgFlag.nOtherInfo = wOther;
			imgFlag.nSize = pRI->itemInfo[nThemeIndex].rep.dwSize;
			if(m_bBigEndian)
			{
				ConvImagFlag(imgFlag);
			}
			memcpy(pImgflagBuf,&imgFlag,sizeof(IMG_FLAG_T));
		}
		else
		{
			memcpy(&imgFlag,pImgflagBuf,sizeof(IMG_FLAG_T));
			imgFlag.nSize = 0;
			memcpy(pImgflagBuf,&imgFlag,sizeof(IMG_FLAG_T));
			//修改偏移为0
			* (DWORD*)(pBaseBuf+ (dwImgDataOffset + pRI->uResIndex*sizeof(DWORD))) = 0;
		}

		//移动内存
		DWORD dwTotalOffset = (DWORD)((pBaseBuf+imgDataOfffset) - m_pNewBuf) + ALIGN4_SIZE(dwOrgSize);
		memmove(m_pNewBuf+(dwTotalOffset+(DWORD)nDeltAlign),m_pNewBuf + dwTotalOffset, m_dwCurTotalSize - dwTotalOffset);
		memset(m_pNewBuf+dwTotalOffset-ALIGN4_SIZE(dwOrgSize),0, ALIGN4_SIZE(dwRepSize));
	}

	if(pRI->itemInfo[nThemeIndex].wModified && !pRI->itemInfo[nThemeIndex].bNotUsed)
	{
		WORD wCmpType = pRI->itemInfo[nThemeIndex].rep.wCmpType;
		
		if(m_bBigEndian)
		{
			if( wCmpType == IMG_CMP_BMP_16_565_RAW || wCmpType == IMG_CMP_BMP_16_565_RLE||
				wCmpType == IMG_CMP_BMP_OSD        || wCmpType == IMG_CMP_BMP_8_RAW ||
				wCmpType == IMG_CMP_BMP_8_SCI)
			{
				m_pRC->BmpConvertToBigEndian(pRI->itemInfo[nThemeIndex].rep.pBuf,
					pRI->itemInfo[nThemeIndex].rep.dwSize,
					wCmpType,
					wCmpType);
				
			}
		}
		
		memcpy(pBaseBuf+imgDataOfffset,pRI->itemInfo[nThemeIndex].rep.pBuf,dwRepSize);
		
		if(m_bBigEndian)
		{
			if( wCmpType == IMG_CMP_BMP_16_565_RAW || wCmpType == IMG_CMP_BMP_16_565_RLE||
				wCmpType == IMG_CMP_BMP_OSD        || wCmpType == IMG_CMP_BMP_8_RAW ||
				wCmpType == IMG_CMP_BMP_8_SCI)
			{
				m_pRC->BmpConvertToLitEndian(pRI->itemInfo[nThemeIndex].rep.pBuf,
					pRI->itemInfo[nThemeIndex].rep.dwSize,
					wCmpType,
					wCmpType);
				
			}
		}
	}

	m_dwCurTotalSize += (DWORD)nDeltAlign;

	return TRUE;
}
BOOL CResParser::SaveAnim(int nItemIndex,int nThemeIndex)
{
	if(m_pNewBuf == NULL)
	{
		return FALSE;
	}

	RES_ITEM_PTR pRI = m_vResItem[nItemIndex];
	if(pRI->itemInfo[nThemeIndex].wExist == 0 || pRI->itemInfo[nThemeIndex].wModified == 0)
	{
		return TRUE;
	}

	DWORD dwOrgSize = pRI->itemInfo[nThemeIndex].org.dwSize;
	DWORD dwRepSize = pRI->itemInfo[nThemeIndex].rep.dwSize;

	int nDeltAlign =  (int)ALIGN4_SIZE(dwRepSize) - (int)ALIGN4_SIZE(dwOrgSize);

	DWORD dwOffsetTmp =0;
	DWORD dwSizeTmp =0;
	int i = 0;

	BYTE *pBaseBuf = MidifyHdrOffset(nDeltAlign,nThemeIndex,pRI);

	// 记录没有改变编译以前的ANIM数据
	DWORD dwAnimOffset = * (DWORD*)(pBaseBuf+MMI_RT_ANIM*12);
	DWORD dwAnimNum = * (DWORD*)(pBaseBuf+MMI_RT_ANIM*12+4);
	DWORD dwAnimSize = * (DWORD*)(pBaseBuf+MMI_RT_ANIM*12+8);
	CONV_DWORD_EX(dwAnimOffset);
	CONV_DWORD_EX(dwAnimNum);
	CONV_DWORD_EX(dwAnimSize);
	
	_ASSERT(pRI->uResIndex < dwAnimNum);
	
	DWORD dwAnimFlagOffset = * (DWORD*)(pBaseBuf + dwAnimOffset);
	DWORD dwAnimDataOffset = * (DWORD*)(pBaseBuf + dwAnimOffset+4);
	CONV_DWORD_EX(dwAnimFlagOffset);
	CONV_DWORD_EX(dwAnimDataOffset);
	
	BYTE *pAnimflagBuf = pBaseBuf+ (dwAnimFlagOffset + pRI->uResIndex*sizeof(ANIM_FLAG_T));
	
	DWORD animDataOfffset =* (DWORD*)(pBaseBuf+ (dwAnimDataOffset + pRI->uResIndex*sizeof(DWORD)));
	CONV_DWORD_EX(animDataOfffset);


	// 先修改偏移，然后再移动内存，插入数据
	if(nDeltAlign != 0)
	{
		// 不需要修改IMG 偏移			
		// 修改Anim 偏移
		ModifyAnimOffset(pBaseBuf,nDeltAlign,pRI->uResIndex + 1);
		// 修改RING 偏移
		ModifyRingOffset(pBaseBuf,nDeltAlign,0);
		// 修改TEXT 偏移
		ModifyTextOffset(pBaseBuf,nDeltAlign,0);
		// 修改FONT偏移
		ModifyFontOffset(pBaseBuf,nDeltAlign,0);		
		// 修改Data 偏移
		ModifyDataOffset(pBaseBuf,nDeltAlign,0);

		// 修改资源类型偏移	
		for( i = MMI_RT_RING ; i< MMI_RT_MAX_NUM; i++)
		{
			dwOffsetTmp = *(DWORD*)(pBaseBuf + i*12);
			CONV_DWORD_EX(dwOffsetTmp);
			if(dwOffsetTmp != 0)
			{
				dwOffsetTmp += (DWORD)nDeltAlign;
				CONV_DWORD_EX(dwOffsetTmp);
				*(DWORD*)(pBaseBuf + i*12) = dwOffsetTmp;
			}
		}
		// 修改Anim 的总size
		dwSizeTmp = dwAnimSize;
		if(dwSizeTmp != 0)
		{
			dwSizeTmp += (DWORD)nDeltAlign;
			CONV_DWORD_EX(dwSizeTmp);
			*(DWORD*)(pBaseBuf+8) = dwSizeTmp;
		}
// 		// 修改Anim Flag 中的Size
// 		dwSizeTmp = dwRepSize;
// 		CONV_DWORD_EX(dwSizeTmp);
// 		*(DWORD*)(pAnimflagBuf+ 8) = dwSizeTmp;

		ANIM_FLAG_T animFlag = {0};
		animFlag.nHeight = pRI->itemInfo[nThemeIndex].rep.wHeight;
		animFlag.nWidth  = pRI->itemInfo[nThemeIndex].rep.wWidth;
		animFlag.nType   = MAKEWORD((BYTE)pRI->itemInfo[nThemeIndex].rep.wCmpType,(BYTE)pRI->itemInfo[nThemeIndex].rep.wItemType);
		animFlag.nFrameNum = (WORD)pRI->itemInfo[nThemeIndex].rep.dwFrame;
		animFlag.nSize = pRI->itemInfo[nThemeIndex].rep.dwSize;
		if(m_bBigEndian)
		{
			ConvAnimFlag(animFlag);
		}
		memcpy(pAnimflagBuf,&animFlag,sizeof(ANIM_FLAG_T));


		//移动内存
		DWORD dwTotalOffset = (DWORD)((pBaseBuf+animDataOfffset) - m_pNewBuf) + ALIGN4_SIZE(dwOrgSize);
		memmove(m_pNewBuf+(dwTotalOffset+(DWORD)nDeltAlign),m_pNewBuf + dwTotalOffset, m_dwCurTotalSize - dwTotalOffset);
		memset(m_pNewBuf+dwTotalOffset-ALIGN4_SIZE(dwOrgSize),0, ALIGN4_SIZE(dwRepSize));
	}

	WORD wCmpType = pRI->itemInfo[i].rep.wCmpType;

	if(m_bBigEndian)
	{
		if( wCmpType == ANIM_CMP_BMP_MBMP)
		{
			m_pRC->AnimConvertToBigEndian(pRI->itemInfo[i].rep.pBuf,
										 pRI->itemInfo[i].rep.dwSize);

		}
	}

	memcpy(pBaseBuf+animDataOfffset,pRI->itemInfo[nThemeIndex].rep.pBuf,dwRepSize);

	if(m_bBigEndian)
	{
		if( wCmpType == ANIM_CMP_BMP_MBMP)
		{
			m_pRC->AnimConvertToLitEndian(pRI->itemInfo[i].rep.pBuf,
										 pRI->itemInfo[i].rep.dwSize);

		}
	}

	m_dwCurTotalSize += (DWORD)nDeltAlign;

	return TRUE;
}
BOOL CResParser::SaveRing(int nItemIndex,int nThemeIndex)
{
	if(m_pNewBuf == NULL)
	{
		return FALSE;
	}

	RES_ITEM_PTR pRI = m_vResItem[nItemIndex];
	if(   pRI->itemInfo[nThemeIndex].wExist == 0 || 
		( pRI->itemInfo[nThemeIndex].wModified == 0 &&
		 !pRI->itemInfo[nThemeIndex].bNotUsed) )
	{
		return TRUE;
	}


	DWORD dwOrgSize = pRI->itemInfo[nThemeIndex].org.dwSize;
	DWORD dwRepSize = pRI->itemInfo[nThemeIndex].rep.dwSize;

	int nDeltAlign =  (int)ALIGN4_SIZE(dwRepSize) - (int)ALIGN4_SIZE(dwOrgSize);

	DWORD dwOffsetTmp =0;
	DWORD dwSizeTmp =0;
	int i = 0;

	BYTE *pBaseBuf = MidifyHdrOffset(nDeltAlign,nThemeIndex,pRI);

	// 记录没有改变编译以前的RING数据
	DWORD dwRingOffset = * (DWORD*)(pBaseBuf+MMI_RT_RING*12);
	DWORD dwRingNum = * (DWORD*)(pBaseBuf+MMI_RT_RING*12+4);
	DWORD dwRingSize = * (DWORD*)(pBaseBuf+MMI_RT_RING*12+8);
	CONV_DWORD_EX(dwRingOffset);
	CONV_DWORD_EX(dwRingNum);
	CONV_DWORD_EX(dwRingSize);
	
	_ASSERT(pRI->uResIndex < dwRingNum);

	
	BYTE *pRingInfoBuf = pBaseBuf+ (dwRingOffset + pRI->uResIndex*sizeof(MMIAUD_RING_DATA_INFO_T));
	
	DWORD ringDataOfffset =* (DWORD*)(pRingInfoBuf+8);
	CONV_DWORD_EX(ringDataOfffset);


	// 先修改偏移，然后再移动内存，插入数据
	if(nDeltAlign != 0)
	{
		// 不需要修改IMG 偏移			
		// 不需要修改Anim 偏移		
		// 修改RING 偏移
		ModifyRingOffset(pBaseBuf,nDeltAlign,pRI->uResIndex + 1);
		// 修改TEXT 偏移
		ModifyTextOffset(pBaseBuf,nDeltAlign,0);
		// 修改FONT偏移
		ModifyFontOffset(pBaseBuf,nDeltAlign,0);		
		// 修改Data 偏移
		ModifyDataOffset(pBaseBuf,nDeltAlign,0);

		// 修改资源类型偏移	
		for( i = MMI_RT_TEXT ; i< MMI_RT_MAX_NUM; i++)
		{
			dwOffsetTmp = *(DWORD*)(pBaseBuf + i*12);
			CONV_DWORD_EX(dwOffsetTmp);
			if(dwOffsetTmp != 0)
			{
				dwOffsetTmp += (DWORD)nDeltAlign;
				CONV_DWORD_EX(dwOffsetTmp);
				*(DWORD*)(pBaseBuf + i*12) = dwOffsetTmp;
			}
		}
		// 修改RING 的总size
		dwSizeTmp = dwRingSize;
		if(dwSizeTmp != 0)
		{
			dwSizeTmp += (DWORD)nDeltAlign;
			CONV_DWORD_EX(dwSizeTmp);
			*(DWORD*)(pBaseBuf+8) = dwSizeTmp;
		}
		
		if(!pRI->itemInfo[nThemeIndex].bNotUsed && pRI->itemInfo[nThemeIndex].wModified)
		{
			// 修改Ring Info 中的type
			DWORD wType = pRI->itemInfo[nThemeIndex].rep.wItemType;
			CONV_DWORD_EX(wType);
			*(DWORD*)(pRingInfoBuf) = wType;
			// 修改Ring Info 中的Size
			dwSizeTmp = dwRepSize;
			CONV_DWORD_EX(dwSizeTmp);
			*(DWORD*)(pRingInfoBuf+4) = dwSizeTmp;
		}
		else
		{
			*(DWORD*)(pRingInfoBuf+4) = 0;  // size = 0
			*(DWORD*)(pRingInfoBuf+8) = 0;  // offset = 0
		}
		

		//移动内存
		DWORD dwTotalOffset = (DWORD)((pBaseBuf+ringDataOfffset) - m_pNewBuf) + ALIGN4_SIZE(dwOrgSize);
		memmove(m_pNewBuf+(dwTotalOffset+(DWORD)nDeltAlign),m_pNewBuf + dwTotalOffset, m_dwCurTotalSize - dwTotalOffset);
		memset(m_pNewBuf+dwTotalOffset-ALIGN4_SIZE(dwOrgSize),0, ALIGN4_SIZE(dwRepSize));
	}

	if(!pRI->itemInfo[nThemeIndex].bNotUsed && pRI->itemInfo[nThemeIndex].wModified)
	{
		memcpy(pBaseBuf+ringDataOfffset,pRI->itemInfo[nThemeIndex].rep.pBuf,dwRepSize);
	}
	

	m_dwCurTotalSize += (DWORD)nDeltAlign;

	return TRUE;
}

BOOL CResParser::SaveData(int nItemIndex,int nThemeIndex)
{
	if(m_pNewBuf == NULL)
	{
		return FALSE;
	}

	RES_ITEM_PTR pRI = m_vResItem[nItemIndex];
	if(pRI->itemInfo[nThemeIndex].wExist == 0 || 
		(pRI->itemInfo[nThemeIndex].wModified == 0 
		&& !pRI->itemInfo[nThemeIndex].bNotUsed) )
	{
		return TRUE;
	}

	DWORD dwOrgSize = pRI->itemInfo[nThemeIndex].org.dwSize;
	DWORD dwRepSize = pRI->itemInfo[nThemeIndex].rep.dwSize;

	int nDeltAlign =  (int)ALIGN4_SIZE(dwRepSize) - (int)ALIGN4_SIZE(dwOrgSize);

	DWORD dwOffsetTmp =0;
	DWORD dwSizeTmp =0;
	int i = 0;

	BYTE *pBaseBuf = MidifyHdrOffset(nDeltAlign,nThemeIndex,pRI);

	// 记录没有改变编译以前的Data数据
	DWORD dwDataOffset = * (DWORD*)(pBaseBuf+MMI_RT_OTHER*12);
	DWORD dwDataNum = * (DWORD*)(pBaseBuf+MMI_RT_OTHER*12+4);
	DWORD dwDataSize = * (DWORD*)(pBaseBuf+MMI_RT_OTHER*12+8);
	CONV_DWORD_EX(dwDataOffset);
	CONV_DWORD_EX(dwDataNum);
	CONV_DWORD_EX(dwDataSize);
	
	_ASSERT(pRI->uResIndex < dwDataNum);

	
	BYTE *pDataInfoBuf = pBaseBuf+ (dwDataOffset + pRI->uResIndex*sizeof(OTHER_DATA_INFO_T));
	
	DWORD otherDataOfffset =* (DWORD*)(pDataInfoBuf+8);
	CONV_DWORD_EX(otherDataOfffset);


	// 先修改偏移，然后再移动内存，插入数据
	if(nDeltAlign != 0)
	{
		// 不需要修改IMG 偏移			
		// 不需要修改Anim 偏移	
		// 不需要修改Ring 偏移	
		// 不需要修改Text 偏移
		// 不需要修改Font 偏移		
		// 修改Data 偏移
		ModifyDataOffset(pBaseBuf,nDeltAlign,pRI->uResIndex + 1);	

		// 修改资源类型偏移	
		for( i = MMI_RT_COLOR ; i< MMI_RT_MAX_NUM; i++)
		{
			dwOffsetTmp = *(DWORD*)(pBaseBuf + i*12);
			CONV_DWORD_EX(dwOffsetTmp);
			if(dwOffsetTmp != 0)
			{
				dwOffsetTmp += (DWORD)nDeltAlign;
				CONV_DWORD_EX(dwOffsetTmp);
				*(DWORD*)(pBaseBuf + i*12) = dwOffsetTmp;
			}
		}
		// 修改Data 的总size
		dwSizeTmp = dwDataSize;
		if(dwSizeTmp != 0)
		{
			dwSizeTmp += (DWORD)nDeltAlign;
			CONV_DWORD_EX(dwSizeTmp);
			*(DWORD*)(pBaseBuf+8) = dwSizeTmp;
		}

		if(!pRI->itemInfo[nThemeIndex].bNotUsed && pRI->itemInfo[nThemeIndex].wModified)
		{
			// 修改Other Data Info 中的type
			DWORD wType = pRI->itemInfo[nThemeIndex].rep.wItemType;
			CONV_DWORD_EX(wType);
			*(DWORD*)(pDataInfoBuf) = wType;
			// 修改Ring Info 中的Size
			dwSizeTmp = dwRepSize;
			CONV_DWORD_EX(dwSizeTmp);
			*(DWORD*)(pDataInfoBuf+4) = dwSizeTmp;		
		}
		else
		{
			*(DWORD*)(pDataInfoBuf+4) = 0;
			*(DWORD*)(pDataInfoBuf+8) = 0;
		}

		//移动内存
		DWORD dwTotalOffset = (DWORD)((pBaseBuf+otherDataOfffset) - m_pNewBuf) + ALIGN4_SIZE(dwOrgSize);
		memmove(m_pNewBuf+(dwTotalOffset+(DWORD)nDeltAlign),m_pNewBuf + dwTotalOffset, m_dwCurTotalSize - dwTotalOffset);
		memset(m_pNewBuf+dwTotalOffset-ALIGN4_SIZE(dwOrgSize),0, ALIGN4_SIZE(dwRepSize));
	}

	if(!pRI->itemInfo[nThemeIndex].bNotUsed && pRI->itemInfo[nThemeIndex].wModified)
	{
		memcpy(pBaseBuf+otherDataOfffset,pRI->itemInfo[nThemeIndex].rep.pBuf,dwRepSize);
	}	

	m_dwCurTotalSize += (DWORD)nDeltAlign;

	return TRUE;
}
BOOL CResParser::SaveColor(int nItemIndex,int nThemeIndex)
{
	if(m_pNewBuf == NULL)
	{
		return FALSE;
	}

	RES_ITEM_PTR pRI = m_vResItem[nItemIndex];
	if(pRI->itemInfo[nThemeIndex].wExist == 0 || pRI->itemInfo[nThemeIndex].wModified == 0)
	{
		return TRUE;
	}

	BYTE *pBaseBuf = NULL;
	if(m_bThemeOnly)
	{
		pBaseBuf = m_pNewBuf;
	}
	else
	{
		// 获得偏移改变前的Theme资源编译
		DWORD dwThemeOffset = * (DWORD*)(m_pNewBuf+ DEFAULT_RES_INFO_OFFSET + nThemeIndex*8);
		CONV_DWORD_EX(dwThemeOffset);	
		pBaseBuf = m_pNewBuf + dwThemeOffset;
	}


	DWORD dwiMduOffset = * (DWORD*)(pBaseBuf + (20 + pRI->uModuleID * 4)); // 20: res_hdr(16) + total_size(4)
	CONV_DWORD_EX(dwiMduOffset);
	pBaseBuf += dwiMduOffset;

	// 记录没有改变编译以前的Color数据
	DWORD dwColorOffset = * (DWORD*)(pBaseBuf+MMI_RT_COLOR*12);
	DWORD dwColorNum = * (DWORD*)(pBaseBuf+MMI_RT_COLOR*12+4);
	DWORD dwColorSize = * (DWORD*)(pBaseBuf+MMI_RT_COLOR*12+8);
	CONV_DWORD_EX(dwColorOffset);
	CONV_DWORD_EX(dwColorNum);
	CONV_DWORD_EX(dwColorSize);
	
	_ASSERT(pRI->uResIndex < dwColorNum);

	
	BYTE *pColorInfoBuf = pBaseBuf+ (dwColorOffset + pRI->uResIndex*sizeof(MMI_COLOR_T));

	*(DWORD*)pColorInfoBuf = *(DWORD*)pRI->itemInfo[nThemeIndex].rep.pBuf;


	return TRUE;
}

CString CResParser::GetResTypeString(UINT uResType)
{
	switch(uResType)
	{
	case MMI_RT_IMAGE:
		return _T("IMAGE");

	case MMI_RT_ANIM:
		return _T("ANIM");

	case MMI_RT_RING:
		return _T("RING");

	case MMI_RT_TEXT:
		return _T("TEXT");

	case MMI_RT_FONT:
		return _T("FONT");
	
	case MMI_RT_OTHER:
		return _T("DATA");
		
	case MMI_RT_COLOR:
		return _T("COLOR");

	case MMI_RT_RECT:
		return _T("RECT");	
	
	default:
		_ASSERTE(0); //lint !e506 !e774 delete dx 2015-08-05
	    break;
	}

	return _T("");
}

void CResParser::Clear()
{
	UINT j= 0;
	//int  i= 0;
	UINT nCount = m_vResItem.size();
	for(j= 0; j< nCount; j++)
	{
		RES_ITEM_PTR pRI = m_vResItem[j];
		pRI->Clear();
		delete pRI;	
	}

	m_vResItem.clear();

	SAFE_DEL(m_pResBuf);
	m_dwResSize = 0;

	SAFE_DEL(m_pNewBuf);
	m_nNewSize = 0;

//	SAFE_DEL(m_pTmpBuf);

	m_agThemeName.RemoveAll();
	m_uThemeNum = 0;

	m_bBzp = FALSE;
	m_agBzpFiles.RemoveAll();

	m_bCscs = FALSE;

	m_dwTextOrgSize = 0;
	m_dwTextRepSize = 0;
	
	m_dwLDBOrgSize  = 0;
	m_dwLDBRepSize  = 0;

	m_agIMName.RemoveAll();
	m_mapIM.RemoveAll();

	m_bLoadThmFolder = FALSE;

	m_bRmCrossRepeat = FALSE;
	m_bAllUnicode = TRUE;
	m_bStrNoFlag = FALSE;


	ClearStrTable();
	
	for(j = 0; j< m_vStrSet.size();j++)
	{	
		CStrSet_Ptr pStrSet = m_vStrSet[j];
		if(pStrSet != NULL)
		{
			pStrSet->ClearAll();
			delete pStrSet;
			pStrSet = NULL;
		}
	}
	m_vStrSet.clear();
}

CString CResParser::GetResItemInfo(int nItemIndex,int nThemeIndex /*= 0*/)
{
	CString strInfo = _T("");
	if(nItemIndex < 0 || nItemIndex > (int)m_vResItem.size())
	{
		return strInfo;
	}
	if(nThemeIndex < 0 || nThemeIndex > (int)m_uThemeNum)
	{
		return strInfo;
	}	

	//DWORD dwColor = 0;
	RES_ITEM_PTR pRI = m_vResItem[nItemIndex];
	ITEM_INFO_T &iteminfo = pRI->itemInfo[nThemeIndex];
	BOOL bModified = pRI->itemInfo[nThemeIndex].wModified;
	REPL_ITEM_INFO_T &riteminfo = bModified? iteminfo.rep : iteminfo.org;
	WORD wCmpType =  riteminfo.wCmpType;
	WORD wItemType = riteminfo.wItemType;



	switch(pRI->uResType)
	{
	case MMI_RT_IMAGE:
		strInfo.Format(_T("ImageType:\t%s\r\nCmpType:  \t%s\r\nWidth:    \t%d\r\nHeight:   \t%d\r\nOrgSize:  \t%d\r\nSizeDelt: \t%d\r\nModified: \t%s\r\n%s"),
					   GetImgTypeString(wItemType).operator LPCTSTR() ,
					   GetImgCmpTypeString(wCmpType).operator LPCTSTR(),
					   riteminfo.wWidth,
					   riteminfo.wHeight,
					   ALIGN4_SIZE(iteminfo.org.dwSize),
					   !bModified ? 0 : (ALIGN4_SIZE(iteminfo.rep.dwSize)-ALIGN4_SIZE(iteminfo.org.dwSize)),
					   !bModified ? _T("False") : _T("True"),
					   IMAG_TIPS);
		break;
	case MMI_RT_ANIM:
		strInfo.Format(_T("ImageType:\t%s\r\nCmpType:  \t%s\r\nWidth:    \t%d\r\nHeight:   \t%d\r\nFrameNum: \t%d\r\nOrgSize:  \t%d\r\nSizeDelt: \t%d\r\nModified: \t%s\r\n%s"),
				       GetImgTypeString(wItemType).operator LPCTSTR(),
					   GetAnimCmpTypeString(wCmpType).operator LPCTSTR(),
					   riteminfo.wWidth,
					   riteminfo.wHeight,
					   riteminfo.dwFrame,
					   ALIGN4_SIZE(iteminfo.org.dwSize),
					   !bModified  ? 0 : (ALIGN4_SIZE(iteminfo.rep.dwSize)-ALIGN4_SIZE(iteminfo.org.dwSize)),
					   !bModified  ? _T("False") : _T("True"),
					   ANIM_TIPS);
		break;
	case MMI_RT_RING:
		strInfo.Format(_T("RingType:\t%s\r\nSize:     \t%d\r\nSizeDelt: \t%d\r\nModified: \t%s\r\n%s"),
					   GetRingTypeString(wItemType).operator LPCTSTR(),						
					   ALIGN4_SIZE(iteminfo.org.dwSize),
					   iteminfo.rep.dwSize==0 ? 0 : (ALIGN4_SIZE(iteminfo.rep.dwSize)-ALIGN4_SIZE(iteminfo.org.dwSize)),
					   !bModified ? _T("False") : _T("True"),
					   RING_TIPS);
	    break;
	case MMI_RT_FONT:
	    break;
	case MMI_RT_TEXT:
		break;
	case MMI_RT_OTHER:
		strInfo.Format(_T("Size:     \t%d\r\nSizeDelt: \t%d\r\nModified: \t%s\r\n"),				
					   ALIGN4_SIZE(iteminfo.org.dwSize),
					   iteminfo.rep.dwSize==0 ? 0 : (ALIGN4_SIZE(iteminfo.rep.dwSize)-ALIGN4_SIZE(iteminfo.org.dwSize)),
					   !bModified ? _T("False") : _T("True"));
		break;
	case MMI_RT_COLOR:	
		strInfo.Format(_T("Color:    \t0x%08X\r\nModified: \t%s"),
				       riteminfo.dwColor,
					   !bModified ? _T("False") : _T("True"));
	    break;
	case MMI_RT_RECT:
	    break;
	default:
	    break;
	}

	return strInfo;
}


	
CString CResParser::GetImgTypeString(UINT uImgType)
{
	CString strInfo = _T("");

	switch(uImgType)
	{
	case IMG_BMP:
		strInfo = _T("BMP");
		break;
	case IMG_GIF:
		strInfo = _T("GIF");
		break;
	case IMG_JPG:
		strInfo = _T("JPG");
	    break;
	case IMG_PNG:
		strInfo = _T("PNG");
	    break;
	default:
	    break;
	}
	return strInfo;
}
CString CResParser::GetImgCmpTypeString(UINT uImgCmpType)
{
	switch(uImgCmpType)
	{
	case IMG_CMP_BMP_16_565_RAW:
		return _T("IMG_CMP_BMP_16_565_RAW");

	case IMG_CMP_BMP_16_565_RLE:
		return _T("IMG_CMP_BMP_16_565_RLE");

	case IMG_CMP_BMP_OSD:
		return _T("IMG_CMP_BMP_OSD");

	case IMG_CMP_BMP_JPG:
		return _T("IMG_CMP_BMP_JPG");

	case IMG_CMP_BMP_SJPG:
		return _T("IMG_CMP_BMP_SJPG");
	
	case IMG_CMP_BMP_32_RAW:
		return _T("IMG_CMP_BMP_32_RAW");
	
	case IMG_CMP_BMP_32_SCI:
		return _T("IMG_CMP_BMP_32_SCI");

	case IMG_CMP_GIF_RAW:
		return _T("IMG_CMP_GIF_RAW");
	
	case IMG_CMP_GIF_SCI:
		return _T("IMG_CMP_GIF_SCI");

	case IMG_CMP_JPG_RAW:
		return _T("IMG_CMP_JPG_RAW");

	case IMG_CMP_JPG_SCI:
		return _T("IMG_CMP_JPG_SCI");

	case IMG_CMP_PNG_RAW:
		return _T("IMG_CMP_PNG_RAW");

	case IMG_CMP_PNG_ARGB:
		return _T("IMG_CMP_PNG_ARGB");

	case IMG_CMP_JPG_STD:
		return _T("IMG_CMP_JPG_STD");

	case IMG_CMP_BMP_8_RAW:
		return _T("IMG_CMP_BMP_8_RAW");

	case IMG_CMP_BMP_8_SCI:
		return _T("IMG_CMP_BMP_8_SCI");

	case IMG_CMP_PNG_SABM:
		return _T("IMG_CMP_PNG_SABM");

	case IMG_CMP_BMP_SABM:
		return _T("IMG_CMP_BMP_SABM");

	case IMG_CMP_BMP_CK:
		return _T("IMG_CMP_BMP_CK");

	case IMG_CMP_BMP_OPT:
		return _T("IMG_CMP_BMP_OPT");
		
	case IMG_CMP_BMP_32_ARGB888:
		return _T("IMG_CMP_BMP_32_ARGB888");

	case IMG_CMP_PNG_ARGB888:
		return _T("IMG_CMP_PNG_ARGB888");
		
	case IMG_CMP_BMP_32_ARGB_RLE:
		return _T("IMG_CMP_BMP_32_ARGB_RLE");
		
	case IMG_CMP_PNG_ARGB_RLE:
		return _T("IMG_CMP_PNG_ARGB_RLE");

	case IMG_CMP_RAW:
		return _T("IMG_CMP_RAW");

	default:
		return _T("Unknown Compress");
	}
}

CString CResParser::GetImgFileExtString(UINT uImgCmpType,UINT uImgType)
{
	switch(uImgCmpType)
	{
	case IMG_CMP_BMP_16_565_RAW:
	case IMG_CMP_BMP_16_565_RLE:
	case IMG_CMP_BMP_OSD:
	case IMG_CMP_BMP_JPG:
	case IMG_CMP_BMP_32_RAW:	
	case IMG_CMP_BMP_32_SCI:
	case IMG_CMP_BMP_8_RAW:
	case IMG_CMP_BMP_8_SCI:	
	case IMG_CMP_BMP_SABM:
	case IMG_CMP_BMP_CK:
	case IMG_CMP_BMP_OPT:	
	case IMG_CMP_BMP_32_ARGB888:	
	case IMG_CMP_BMP_32_ARGB_RLE:	
		return _T("BMP");

	case IMG_CMP_BMP_SJPG:
	case IMG_CMP_JPG_RAW:
	case IMG_CMP_JPG_SCI:
	case IMG_CMP_JPG_STD:
		return _T("JPG");

	case IMG_CMP_GIF_RAW:	
	case IMG_CMP_GIF_SCI:
		return _T("GIF");

	case IMG_CMP_PNG_ARGB:
	case IMG_CMP_PNG_SABM:
	case IMG_CMP_PNG_ARGB888:
	case IMG_CMP_PNG_ARGB_RLE:
	case IMG_CMP_PNG_RAW:
		return _T("PNG");

	case IMG_CMP_RAW:
		return GetImgTypeString(uImgType);

	default:
		return _T("");
	}
}

CString CResParser::GetResInfo()
{
	UINT nItemCount = 0;
	UINT nItemReplaced = 0;
	UINT nLeft = 0;
	GetResStat(nItemCount,nItemReplaced);
	nLeft = nItemCount - nItemReplaced;
	CString strInfo = _T("");
	strInfo.Format(_T("BigEndian:\t%s\r\nResSize:\t%d\r\nNewSize:\t%d\r\nDeltSize:\t%d\r\nTotalItems:\t%d\r\nReplaced: \t%d\r\nLeft:   \t%d\r\nMaxResSize:\t0x%08X"),		           
				   m_bBigEndian?_T("True"):_T("False"),
				   m_dwResSize,
				   m_nNewSize,
				   (UINT)m_nNewSize-m_dwResSize,
				   nItemCount,
				   nItemReplaced,
				   nLeft,
				   m_dwMaxResSize);
	return strInfo;
}

void CResParser::ModifyImagOffset(LPBYTE pMduBaseBuf,int nDeltAlign, UINT nPos)
{
	if( pMduBaseBuf == NULL || nDeltAlign == 0 )
		return;

	UINT i = 0;
	DWORD dwOffsetTmp = 0;

	LPBYTE pBaseBuf = pMduBaseBuf + MMI_RT_IMAGE*12;

    DWORD dwOffset = * (DWORD*)pBaseBuf;
	DWORD dwNum = * (DWORD*)(pBaseBuf+4);
	DWORD dwSize = * (DWORD*)(pBaseBuf+8);
	CONV_DWORD_EX(dwOffset);
	CONV_DWORD_EX(dwNum);
	CONV_DWORD_EX(dwSize);
	
	if(dwNum == 0 || dwOffset==0)
	{
		return;
	}

	if(nPos >= dwNum)
	{
		return; // it is the last one;
	}
	
	_ASSERT(nPos < dwNum);	

	DWORD dwDataOffset = * (DWORD*)(pMduBaseBuf + dwOffset+4);
	CONV_DWORD_EX(dwDataOffset);


	for(i = nPos; i< dwNum; i++)
	{
		dwOffsetTmp = *(DWORD*)(pMduBaseBuf+ (dwDataOffset + i*sizeof(DWORD)));
		CONV_DWORD_EX(dwOffsetTmp);
		if(dwOffsetTmp != 0)
		{
			dwOffsetTmp += (DWORD)nDeltAlign;
			CONV_DWORD_EX(dwOffsetTmp);
			*(DWORD*)(pMduBaseBuf+ (dwDataOffset + i*sizeof(DWORD))) = dwOffsetTmp;
		}
	}

	if(nPos == 0)
	{
		//修改IMAG资源开头的FLAG和DATA的偏移
		dwOffsetTmp = *(DWORD*)(pMduBaseBuf+ dwOffset);
		CONV_DWORD_EX(dwOffsetTmp);
		if(dwOffsetTmp != 0)
		{
			dwOffsetTmp += (DWORD)nDeltAlign;
			CONV_DWORD_EX(dwOffsetTmp);
			*(DWORD*)(pMduBaseBuf+ dwOffset) = dwOffsetTmp;
		}

		dwOffsetTmp = *(DWORD*)(pMduBaseBuf+ dwOffset+4);
		CONV_DWORD_EX(dwOffsetTmp);
		if(dwOffsetTmp != 0)
		{
			dwOffsetTmp += (DWORD)nDeltAlign;
			CONV_DWORD_EX(dwOffsetTmp);
			*(DWORD*)(pMduBaseBuf+ dwOffset+4) = dwOffsetTmp;
		}
	}
}

void CResParser::ModifyAnimOffset(LPBYTE pMduBaseBuf,int nDeltAlign, UINT nPos)
{
	if( pMduBaseBuf == NULL || nDeltAlign == 0 )
		return;

	UINT i = 0;
	DWORD dwOffsetTmp = 0;

	LPBYTE pBaseBuf = pMduBaseBuf + MMI_RT_ANIM*12;

    DWORD dwOffset = *(DWORD*)(pBaseBuf);
	DWORD dwNum    = *(DWORD*)(pBaseBuf+4);
	DWORD dwSize   = *(DWORD*)(pBaseBuf+8);
	CONV_DWORD_EX(dwOffset);
	CONV_DWORD_EX(dwNum);
	CONV_DWORD_EX(dwSize);

	if(dwNum == 0 || dwOffset==0)
	{
		return;
	}

	if(nPos >= dwNum)
	{
		return; // it is the last one;
	}
	
	_ASSERT(nPos < dwNum);	

	DWORD dwDataOffset = *(DWORD*)(pMduBaseBuf + dwOffset+4);
	CONV_DWORD_EX(dwDataOffset);


	for(i = nPos; i< dwNum; i++)
	{
		dwOffsetTmp = *(DWORD*)(pMduBaseBuf+ (dwDataOffset + (UINT)i*sizeof(DWORD)));
		CONV_DWORD_EX(dwOffsetTmp);
		if(dwOffsetTmp != 0)
		{
			dwOffsetTmp += (DWORD)nDeltAlign;
			CONV_DWORD_EX(dwOffsetTmp);
			* (DWORD*)(pMduBaseBuf+ (dwDataOffset + (UINT)i*sizeof(DWORD))) = dwOffsetTmp;
		}
	}

	if(nPos == 0)
	{
		//修改ANIM资源开头的FLAG和DATA的偏移
		dwOffsetTmp = *(DWORD*)(pMduBaseBuf+ dwOffset);
		CONV_DWORD_EX(dwOffsetTmp);
		if(dwOffsetTmp != 0)
		{
			dwOffsetTmp += (DWORD)nDeltAlign;
			CONV_DWORD_EX(dwOffsetTmp);
			*(DWORD*)(pMduBaseBuf+ dwOffset) = dwOffsetTmp;
		}

		dwOffsetTmp = *(DWORD*)(pMduBaseBuf+ dwOffset+4);
		CONV_DWORD_EX(dwOffsetTmp);
		if(dwOffsetTmp != 0)
		{
			dwOffsetTmp += (DWORD)nDeltAlign;
			CONV_DWORD_EX(dwOffsetTmp);
			*(DWORD*)(pMduBaseBuf+ dwOffset+4) = dwOffsetTmp;
		}
	}

}
void CResParser::ModifyRingOffset(LPBYTE pMduBaseBuf,int nDeltAlign, UINT nPos)
{
	if( pMduBaseBuf == NULL || nDeltAlign == 0 )
		return;

	UINT i = 0;
	DWORD dwOffsetTmp = 0;

	LPBYTE pBaseBuf = pMduBaseBuf + MMI_RT_RING*12;

    DWORD dwOffset = *(DWORD*)(pBaseBuf);
	DWORD dwNum    = *(DWORD*)(pBaseBuf+4);
	DWORD dwSize   = *(DWORD*)(pBaseBuf+8);
	CONV_DWORD_EX(dwOffset);
	CONV_DWORD_EX(dwNum);
	CONV_DWORD_EX(dwSize);

	if(dwNum == 0 || dwOffset==0)
	{
		return;
	}

	if(nPos >= dwNum)
	{
		return; // it is the last one;
	}
	
	_ASSERT(nPos < dwNum);	


	for(i = nPos; i< dwNum; i++)
	{
		dwOffsetTmp = *(DWORD*)(pMduBaseBuf+ (dwOffset + (UINT)i*sizeof(MMIAUD_RING_DATA_INFO_T) + 8));
		CONV_DWORD_EX(dwOffsetTmp);
		if(dwOffsetTmp != 0)
		{
			dwOffsetTmp += (DWORD)nDeltAlign;
			CONV_DWORD_EX(dwOffsetTmp);
			*(DWORD*)(pMduBaseBuf+ (dwOffset + (UINT)i*sizeof(MMIAUD_RING_DATA_INFO_T) + 8)) = dwOffsetTmp;
		}
	}

}
void CResParser::ModifyFontOffset(LPBYTE pMduBaseBuf,int nDeltAlign, UINT nPos)
{
	if( pMduBaseBuf == NULL || nDeltAlign == 0 )
		return;

	UINT i = 0;
	DWORD dwOffsetTmp = 0;

	LPBYTE pBaseBuf = pMduBaseBuf + MMI_RT_FONT*12;

    DWORD dwOffset = *(DWORD*)(pBaseBuf);
	DWORD dwNum    = *(DWORD*)(pBaseBuf+4);
	DWORD dwSize   = *(DWORD*)(pBaseBuf+8);
	CONV_DWORD_EX(dwOffset);
	CONV_DWORD_EX(dwNum);
	CONV_DWORD_EX(dwSize);

	if(dwNum == 0 || dwOffset==0)
	{
		return;
	}

	if(nPos >= dwNum)
	{
		return; // it is the last one;
	}
	
	_ASSERT(nPos < dwNum);	

	int nLangNum = 0;
	if(dwNum > 1)
	{
		DWORD dwFirstOffset= *(DWORD*)(pMduBaseBuf+ (dwOffset  + 4));
		CONV_DWORD_EX(dwFirstOffset);
		DWORD dwSecondOffset = *(DWORD*)(pMduBaseBuf+ (dwOffset + sizeof(GUI_FONT_INFO_EX_T) + 4));
		CONV_DWORD_EX(dwSecondOffset);
		nLangNum = (dwSecondOffset - dwFirstOffset) / sizeof(GUI_FONT_DB_T);
	}
	else
	{
		DWORD dwFirstOffset= *(DWORD*)(pMduBaseBuf+ (dwOffset  + 4));
		CONV_DWORD_EX(dwFirstOffset);

		DWORD dwDataOffset =  *(DWORD*)(pMduBaseBuf+ dwFirstOffset + 4 );
		CONV_DWORD_EX(dwDataOffset);
		nLangNum = (dwDataOffset - dwFirstOffset) / sizeof(GUI_FONT_DB_T);
	}

	_ASSERT(nLangNum != 0);


	for(i = nPos; i< dwNum; i++)
	{
		DWORD dwDBOffset = *(DWORD*)(pMduBaseBuf+ (dwOffset + (UINT)i*sizeof(GUI_FONT_INFO_EX_T) + 4));
		CONV_DWORD_EX(dwDBOffset);

		for(int j = 0 ;j< nLangNum ; j++)
		{
			dwOffsetTmp = *(DWORD*)(pMduBaseBuf+ (dwDBOffset + (UINT)i*sizeof(GUI_FONT_DB_T) + 4));
			CONV_DWORD_EX(dwOffsetTmp);
			if(dwOffsetTmp != 0)
			{
				dwOffsetTmp += (DWORD)nDeltAlign;
				CONV_DWORD_EX(dwOffsetTmp);
				*(DWORD*)(pMduBaseBuf+ (dwDBOffset + (UINT)i*sizeof(GUI_FONT_DB_T) + 4)) = dwOffsetTmp;
			}
		}
	}
	for(i = nPos; i< (int)dwNum; i++)
	{
		dwOffsetTmp = *(DWORD*)(pMduBaseBuf+ (dwOffset + (UINT)i*sizeof(GUI_FONT_INFO_EX_T) + 4));
		CONV_DWORD_EX(dwOffsetTmp);
		if(dwOffsetTmp != 0)
		{
			dwOffsetTmp += (DWORD)nDeltAlign;
			CONV_DWORD_EX(dwOffsetTmp);
			*(DWORD*)(pMduBaseBuf+ (dwOffset + (UINT)i*sizeof(GUI_FONT_INFO_EX_T) + 4)) = dwOffsetTmp;
		}
	}

}
void CResParser::ModifyTextOffset(LPBYTE pMduBaseBuf,int nDeltAlign, UINT nPos)
{
	UNUSED_ALWAYS(nPos);
	if( pMduBaseBuf == NULL || nDeltAlign == 0 )
		return;

	//字符串资源，暂时不能替换，所以需要修改所有偏移
	_ASSERT(nPos == 0);

	UINT i = 0;
	DWORD dwOffsetTmp = 0;

	LPBYTE pBaseBuf = pMduBaseBuf + MMI_RT_TEXT*12;

    DWORD dwOffset = *(DWORD*)(pBaseBuf);
	DWORD dwNum    = *(DWORD*)(pBaseBuf+4);
	DWORD dwSize   = *(DWORD*)(pBaseBuf+8);
	CONV_DWORD_EX(dwOffset);
	CONV_DWORD_EX(dwNum);
	CONV_DWORD_EX(dwSize);

	if(dwNum == 0 || dwOffset==0)
	{
		return;
	}

	if(nPos >= dwNum)
	{
		return; // it is the last one;
	}
	
	_ASSERT(nPos < dwNum);	

	int nLangNum = 0;

	DWORD dwFirstOffset= *(DWORD*)(pMduBaseBuf+ dwOffset);
	CONV_DWORD_EX(dwFirstOffset);

	nLangNum = (dwFirstOffset - dwOffset) / sizeof(KSP_TEXT_T);
	_ASSERT(nLangNum != 0);

	int nRealLang = nLangNum;

	for(i = 0; i< (UINT)nLangNum; i++)
	{
		dwOffsetTmp = *(DWORD*)(pMduBaseBuf+ (dwOffset + (UINT)i*sizeof(KSP_TEXT_T)));
		CONV_DWORD_EX(dwOffsetTmp);
		if(dwOffsetTmp == 0)
		{
			nRealLang--;
		}
	}

	for(i = 0; i< (UINT)(dwNum*nRealLang); i++)
	{
		dwOffsetTmp = *(DWORD*)(pMduBaseBuf+ (dwFirstOffset + (UINT)i*4));
		CONV_DWORD_EX(dwOffsetTmp);
		if(dwOffsetTmp != 0)
		{
			dwOffsetTmp += (DWORD)nDeltAlign;
			CONV_DWORD_EX(dwOffsetTmp);
			*(DWORD*)(pMduBaseBuf+ (dwFirstOffset + (UINT)i*4)) = dwOffsetTmp;
		}
	}

	for(i = 0; i< (UINT)nLangNum; i++)
	{
		dwOffsetTmp = *(DWORD*)(pMduBaseBuf+ (dwOffset + (UINT)i*sizeof(KSP_TEXT_T)));
		CONV_DWORD_EX(dwOffsetTmp);
		if(dwOffsetTmp != 0)
		{
			dwOffsetTmp += (DWORD)nDeltAlign;
			CONV_DWORD_EX(dwOffsetTmp);
			*(DWORD*)(pMduBaseBuf+ (dwOffset + (UINT)i*sizeof(KSP_TEXT_T)))= dwOffsetTmp;
		}
	}

}

void CResParser::ModifyTextOffset2(LPBYTE pMduBaseBuf,int nDeltAlign, UINT nLangID,DWORD dwEnStart,DWORD dwEnEnd)
{
	if( pMduBaseBuf == NULL || nDeltAlign == 0 )
		return;
	
	//字符串资源，暂时不能替换，所以需要修改所有偏移
	_ASSERT(nLangID != 0);
	
	UINT i = 0;
	DWORD dwOffsetTmp = 0;
	
	LPBYTE pBaseBuf = pMduBaseBuf + MMI_RT_TEXT*12;
	
    DWORD dwOffset = *(DWORD*)(pBaseBuf);
	DWORD dwNum    = *(DWORD*)(pBaseBuf+4);
	DWORD dwSize   = *(DWORD*)(pBaseBuf+8);
	CONV_DWORD_EX(dwOffset);
	CONV_DWORD_EX(dwNum);
	CONV_DWORD_EX(dwSize);
	
	if(dwNum == 0 || dwOffset==0)
	{
		return;
	}
	
	UINT nLangNum = 0;
	
	DWORD dwFirstOffset= *(DWORD*)(pMduBaseBuf+ dwOffset);
	CONV_DWORD_EX(dwFirstOffset);
	
	nLangNum = (dwFirstOffset - dwOffset) / sizeof(KSP_TEXT_T);
	_ASSERT(nLangNum != 0);

	if(nLangNum == 0 || nLangID >= nLangNum)
	{
		return;
	}	

	UINT nRealLang = nLangNum;
	
	CUIntArray agLangUsedID;
	for(i = 0; i< (UINT)nLangNum; i++)
	{
		dwOffsetTmp = *(DWORD*)(pMduBaseBuf+ (dwOffset + (UINT)i*sizeof(KSP_TEXT_T)));
		CONV_DWORD_EX(dwOffsetTmp);
		if(dwOffsetTmp == 0)
		{
			nRealLang--;
		}
		else
		{
			agLangUsedID.Add(i);
		}
	}

	if(nRealLang == 0)
		return;

	DWORD *pOffset = (DWORD*)(pMduBaseBuf+ dwFirstOffset);


	for(i=0; i< nRealLang; i++)
	{
		UINT nCurlangID = agLangUsedID[i];
		if(nCurlangID==nLangID)
			continue;
		
		for(UINT j=0; j< dwNum; j++)
		{
			dwOffsetTmp = *(pOffset + i*dwNum+ j);
			CONV_DWORD_EX(dwOffsetTmp);

			if(dwOffsetTmp != 0)
			{	
				if(nCurlangID<nLangID)
				{					
					dwOffsetTmp -= dwNum*4;	
				}
				else
				{
					if(dwOffsetTmp >= dwEnStart && dwOffsetTmp <= dwEnEnd)
					{
						dwOffsetTmp -= dwNum*4;
					}
					else
					{
						dwOffsetTmp += nDeltAlign;
					}
				}
				CONV_DWORD_EX(dwOffsetTmp);
				*(pOffset + i*dwNum+ j) = dwOffsetTmp;
			}
		}

		if(nCurlangID>nLangID)
		{
			dwOffsetTmp = *(DWORD*)(pMduBaseBuf+ (dwOffset + (UINT)nCurlangID*sizeof(KSP_TEXT_T)));
			CONV_DWORD_EX(dwOffsetTmp);
			if(dwOffsetTmp != 0)
			{
				dwOffsetTmp -= (DWORD)(dwNum*4);
				CONV_DWORD_EX(dwOffsetTmp);
				*(DWORD*)(pMduBaseBuf+ (dwOffset + (UINT)nCurlangID*sizeof(KSP_TEXT_T)))= dwOffsetTmp;
			}
		}	
	}	
}


void CResParser::ModifyDataOffset(LPBYTE pMduBaseBuf,int nDeltAlign, UINT nPos)
{
	if( pMduBaseBuf == NULL || nDeltAlign == 0 )
		return;

	UINT i = 0;
	DWORD dwOffsetTmp = 0;

	LPBYTE pBaseBuf = pMduBaseBuf + MMI_RT_OTHER*12;

    DWORD dwOffset = *(DWORD*)(pBaseBuf);
	DWORD dwNum    = *(DWORD*)(pBaseBuf+4);
	DWORD dwSize   = *(DWORD*)(pBaseBuf+8);
	CONV_DWORD_EX(dwOffset);
	CONV_DWORD_EX(dwNum);
	CONV_DWORD_EX(dwSize);
	if(dwNum == 0 || dwOffset==0)
	{
		return;
	}

	if(nPos >= dwNum)
	{
		return; // it is the last one;
	}
		
	_ASSERT(nPos < dwNum);	

	for(i = nPos; i< dwNum; i++)
	{
		dwOffsetTmp = *(DWORD*)(pMduBaseBuf+ (dwOffset + (UINT)i*sizeof(OTHER_DATA_INFO_T) + 8));
		CONV_DWORD_EX(dwOffsetTmp);
		if(dwOffsetTmp != 0)
		{
			dwOffsetTmp += (DWORD)nDeltAlign;
			CONV_DWORD_EX(dwOffsetTmp);
			*(DWORD*)(pMduBaseBuf+ (dwOffset + (UINT)i*sizeof(OTHER_DATA_INFO_T) + 8)) = dwOffsetTmp;
		}
	}

}


void CResParser::ModifyTablOffset(int nDeltAlign, UINT nPos)
{
	_ASSERT(m_pNewBuf != NULL);
	if(m_pNewBuf == NULL)
	{
		return;
	}

	UINT nTblNum = m_pNewBuf[13]; 
	if(0 == nTblNum  || nDeltAlign == 0 || nPos >= nTblNum)
	{
		return; // 没有需要替换的资源
	}

	LPBYTE pBaseBuf = NULL;
	if(m_bThemeOnly)
	{
		DWORD dwTotalSize = *(DWORD*)(m_pNewBuf+16);
		CONV_DWORD_EX(dwTotalSize);
		pBaseBuf = m_pNewBuf + dwTotalSize;

	}
	else
	{
		pBaseBuf = m_pNewBuf + LOGO_OFFSET;
	} 
	
	DWORD dwOffset = 0;

	for(UINT i = nPos ; i< nTblNum; i++)
	{
		dwOffset = *(DWORD*)(pBaseBuf+ i*12 + 4);
		CONV_DWORD_EX(dwOffset);
		dwOffset += (DWORD)nDeltAlign;
		CONV_DWORD_EX(dwOffset);
		*(DWORD*)(pBaseBuf+ (UINT)i*12 + 4) = dwOffset;
	}
}

BOOL CResParser::IsModified()
{
	BOOL bModified = FALSE;
	UINT nCount = m_vResItem.size();
	int nLangExistCount = 0;
	int nLangUsedCount = 0;
	UINT i=0;
	for(i = 0; i< nCount; i++)
	{
		RES_ITEM_PTR pRI = m_vResItem[i];
		if(pRI->isModified || pRI->itemInfo[0].bNotUsed)
		{
			bModified = TRUE;
			break;
		}

		//if(_tcslen(pRI->szNewDesc)!=0)
		//{
		//	bModified = TRUE;
		//	break;
		//}


		if(pRI->uResType == MMI_RT_MAX_NUM && pRI->itemInfo[0].wExist)
		{
			nLangExistCount++;
			if(!pRI->itemInfo[0].bNotUsed)
			{
				nLangUsedCount++;
			}
		}
	}

	if(!bModified && nLangUsedCount < nLangExistCount)
	{
		bModified = TRUE;
	}

	if(!bModified )
	{
		bModified = IsStringModified();			
	}
	
	return bModified;
}

BOOL CResParser::IsStringModified()
{
	BOOL bModified = FALSE;
	if(m_vStrSet.size()>0)
	{
		for(UINT i=0;i<m_vStrSet.size();i++)
		{
			if(m_vStrSet[i]->m_agDstStrID.GetSize() > 0 )
			{
				bModified = TRUE;
				break;
			}			
		}		
	}

	return bModified;
}

BOOL CResParser::UndoResItem(int nItemIndex,int nThemeIndex)
{
	USES_CONVERSION;
	if(nItemIndex > (int)m_vResItem.size() || nItemIndex < 0 ||
		nThemeIndex > (int)m_uThemeNum || nThemeIndex < 0)
	{
		return FALSE;
	}

	RES_ITEM_PTR pRI = m_vResItem[nItemIndex];

	if(pRI->itemInfo[nThemeIndex].wModified==0)
	{
		return TRUE;
	}

	int nDeltAlign = ALIGN4_SIZE(pRI->itemInfo[nThemeIndex].rep.dwSize) -
		             ALIGN4_SIZE(pRI->itemInfo[nThemeIndex].org.dwSize) ;

    /*
	if(pRI->uResType == MMI_RT_RING && _tcslen(pRI->szNewDesc) != 0 )
	{
		DWORD dwOldLen = strlen(T2A(pRI->szDesc))+1;
		DWORD dwNewLen = strlen(T2A(pRI->szNewDesc))+1;
		m_nNewSize -= (int)(ALIGN4_SIZE(dwNewLen) - ALIGN4_SIZE(dwOldLen)); 
	}
    */
	
	pRI->itemInfo[nThemeIndex].wModified = FALSE;
	SAFE_DEL(pRI->itemInfo[nThemeIndex].rep.pBuf);
	pRI->itemInfo[nThemeIndex].rep.dwSize = 0;
	memset(pRI->itemInfo[nThemeIndex].rep.szFile,0,_MAX_PATH);
	pRI->itemInfo[nThemeIndex].wModified = 0;

	pRI->itemInfo[nThemeIndex].bNotUsed = FALSE;

	m_nNewSize -= nDeltAlign;

	for(UINT i = 0; i< m_uThemeNum; i++)
	{
		if(m_vResItem[nItemIndex]->itemInfo[i].wModified == 1)
		{
			return TRUE;
		}
	}

	m_vResItem[nItemIndex]->isModified = FALSE;
	return TRUE;
}

LPBYTE CResParser::GetMduBase(LPBYTE pResBuf,int nMudID,int nThemeID)
{
	if(pResBuf == NULL)
	{
		return NULL;
	}	
	
	BYTE *pBaseBuf = NULL;
	if(m_bThemeOnly)
	{
		pBaseBuf = pResBuf;
	}
	else
	{
		// 获得Theme资源偏移
		DWORD dwThemeOffset = * (DWORD*)(pResBuf+ DEFAULT_RES_INFO_OFFSET + nThemeID*8);
		CONV_DWORD_EX(dwThemeOffset);	
		pBaseBuf = pResBuf + dwThemeOffset;
	}		
	
	DWORD dwiMduOffset = * (DWORD*)(pBaseBuf + (20 + nMudID * 4)); // 20: res_hdr(16) + total_size(4)
	CONV_DWORD_EX(dwiMduOffset);
	pBaseBuf += dwiMduOffset;	

	return pBaseBuf;
}

LPBYTE CResParser::MidifyHdrOffset(int nDeltAlign, int nThemeIndex, RES_ITEM_PTR pRI)
{
	_ASSERT(m_pNewBuf!= NULL);
	if(m_pNewBuf == NULL)
	{
		return NULL;
	}

	DWORD dwOffsetTmp =0;
	DWORD dwSizeTmp =0;
	int i = 0;

	BYTE *pBaseBuf = NULL;
	if(m_bThemeOnly)
	{
		pBaseBuf = m_pNewBuf;
	}
	else
	{
		// 获得偏移改变前的Theme资源编译
		DWORD dwThemeOffset = * (DWORD*)(m_pNewBuf+ DEFAULT_RES_INFO_OFFSET + nThemeIndex*8);
		CONV_DWORD_EX(dwThemeOffset);	
		pBaseBuf = m_pNewBuf + dwThemeOffset;
	}
	
	if(nDeltAlign != 0)
	{
		// 修改ExtTable的偏移
		ModifyTablOffset(nDeltAlign,0);


		if(!m_bThemeOnly)
		{
			// 修改RES头中索引Theme的Size
			dwSizeTmp = * (DWORD*)(m_pNewBuf+ DEFAULT_RES_INFO_OFFSET + nThemeIndex*8 + 4);
			CONV_DWORD_EX(dwSizeTmp);
			dwSizeTmp += (DWORD)nDeltAlign;
			CONV_DWORD_EX(dwSizeTmp);		
			* (DWORD*)(m_pNewBuf+ DEFAULT_RES_INFO_OFFSET + nThemeIndex*8 + 4) = dwSizeTmp;
		}
		// 修改当前THEME实体中THEME的Size
		dwSizeTmp = * (DWORD*)(pBaseBuf+16);
		CONV_DWORD_EX(dwSizeTmp);
		dwSizeTmp += (DWORD)nDeltAlign;
		CONV_DWORD_EX(dwSizeTmp);		
		* (DWORD*)(pBaseBuf+16) = dwSizeTmp;

		// 修改当前Theme以后所有Theme的偏移
		for(i=(nThemeIndex+1); i< (int)m_uThemeNum; i++)
		{
			dwOffsetTmp = * (DWORD*)(m_pNewBuf+ DEFAULT_RES_INFO_OFFSET + i*8);
			CONV_DWORD_EX(dwOffsetTmp);
			if(dwOffsetTmp != 0)
			{
				dwOffsetTmp += (DWORD)nDeltAlign;
				CONV_DWORD_EX(dwOffsetTmp);
				* (DWORD*)(m_pNewBuf+ DEFAULT_RES_INFO_OFFSET + i*8) = dwOffsetTmp;			
			}
		}	

	}

	DWORD dwFirstMduOffset= * (DWORD*)(pBaseBuf + 20);
	CONV_DWORD_EX(dwFirstMduOffset);
	int nMduNum = ( dwFirstMduOffset  - (128*2+4) - 20 ) / 4;

	_ASSERT(nMduNum%32 == 0);


	// 修改当前模块以后所有模块的偏移
	if(nDeltAlign != 0)
	{
		for(i = pRI->uModuleID + 1; i< nMduNum; i++)
		{
			dwOffsetTmp = * (DWORD*)(pBaseBuf + (20 + i * 4));
			CONV_DWORD_EX(dwOffsetTmp);
			if(dwOffsetTmp != 0)
			{
				dwOffsetTmp += (UINT)nDeltAlign;
				CONV_DWORD_EX(dwOffsetTmp);
				* (DWORD*)(pBaseBuf + (20 + i * 4)) = dwOffsetTmp;
			}
		}
	}

	DWORD dwiMduOffset = * (DWORD*)(pBaseBuf + (20 + pRI->uModuleID * 4)); // 20: res_hdr(16) + total_size(4)
	CONV_DWORD_EX(dwiMduOffset);
	pBaseBuf += dwiMduOffset;

	return pBaseBuf;

}


CString CResParser::GetRingTypeString(UINT uRingType)
{
	switch(uRingType)
	{
	case MMIAUDIO_MIDI:
		return _T("MMIAUDIO_MIDI");
	case MMIAUDIO_SMAF:
		return _T("MMIAUDIO_SMAF");
	case MMIAUDIO_WAVE:
		return _T("MMIAUDIO_WAVE");	
	case MMIAUDIO_MP3:
		return _T("MMIAUDIO_MP3");
	case MMIAUDIO_AAC:
		return _T("MMIAUDIO_AAC");
	case MMIAUDIO_M4A:
		return _T("MMIAUDIO_M4A");
	case MMIAUDIO_AMR:
		return _T("MMIAUDIO_AMR");
	case MMIAUDIO_WMA:
		return _T("MMIAUDIO_WMA");
	default:
		return _T("UNKNOWN");
	}
}
CString CResParser::GetAnimCmpTypeString(UINT uAnimCmpType)
{
	switch(uAnimCmpType)
	{
	case ANIM_CMP_BMP_MBMP:
		return _T("ANIM_CMP_BMP_MBMP");
	case ANIM_CMP_BMP_MJPG:
		return _T("ANIM_CMP_BMP_MJPG");
	case ANIM_CMP_PNG_ARGB:
		return _T("ANIM_CMP_PNG_ARGB");	
	case ANIM_CMP_BMP_ARGB:
		return _T("ANIM_CMP_BMP_ARGB");	
	case ANIM_CMP_PNG_ARGB888:
		return _T("ANIM_CMP_PNG_ARGB888");	
	case ANIM_CMP_BMP_ARGB888:
		return _T("ANIM_CMP_BMP_ARGB888");	
	default:
		return _T("UNKNOWN");
	}
}

void CResParser::GetResStat(UINT &nItemCount,UINT &nItemReplaced)
{
	nItemCount = 0;
	nItemReplaced = 0;

	UINT nCount = m_vResItem.size();
	for(UINT i= 0; i< nCount; i++)
	{
		RES_ITEM_PTR pRI = m_vResItem[i];
		for(UINT j= 0; j< m_uThemeNum; j++)
		{
			if(pRI->itemInfo[j].wExist)
			{
				nItemCount++;
				if(pRI->itemInfo[j].wModified)
				{
					nItemReplaced++;
				}
			}
		}
	}
}

void CResParser::GetResItemStat(std::vector<ITEM_STAT_T> &vItemStat)
{
	vItemStat.clear();
	UINT nCount = m_vResItem.size();
	for(UINT i= 0; i< nCount; i++)
	{
		RES_ITEM_PTR pRI = m_vResItem[i];
		for(UINT j= 0; j< m_uThemeNum; j++)
		{
			if(pRI->itemInfo[j].wModified)
			{
				ITEM_STAT_T ist = {0};
				ist.nItemIndex = i;
				ist.nThemeIndex = j;
				ist.nDeltSize  = (int)(ALIGN4_SIZE(pRI->itemInfo[j].rep.dwSize) - 
					             ALIGN4_SIZE(pRI->itemInfo[j].org.dwSize));
				vItemStat.push_back(ist);
			}	
		}
	}
	if(vItemStat.size()> 0)
	{
		std::sort(vItemStat.begin(),vItemStat.end(),CmpItemStat);
	}
}

BOOL CResParser::IsOverMaxSize()
{
	return ((UINT)m_nNewSize > m_dwMaxResSize);
}

LPCTSTR CResParser::PreOpenResFile(LPCTSTR lpszFile)
{
	HANDLE hFile = CreateFile(lpszFile,
		GENERIC_READ, 
		FILE_SHARE_READ,
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CString strErr;
		strErr.Format(_T("Can not open file [%s].\r\nMaybe it is used by other program!!!"),lpszFile);
		AfxMessageBox(strErr.operator LPCTSTR());
		return NULL;
	}
	
	DWORD dwSize = GetFileSize(hFile,NULL);
	if(dwSize < 0x4)
	{
		CloseHandle(hFile);
		CString strErr;
		strErr.Format(_T("[%s] is too small!"),lpszFile);
		AfxMessageBox(strErr.operator LPCTSTR());
		return NULL;
	}

	BYTE buf[4] = {0};
	LPBYTE pBuf = buf;

	DWORD dwRead;
	ReadFile(hFile,pBuf,4,&dwRead,NULL);	
	CloseHandle(hFile);

	if( (pBuf[0] == 's' || pBuf[0] == 't') &&
		pBuf[1] == 'r' &&
		pBuf[2] == 'e' &&
		pBuf[3] == 's' )
	{		
		return lpszFile;
	}
	else
	{
		CStringArray * pagFiles = NULL;
		if(!m_pBzp->UnBzp(lpszFile,m_strTmpFolder,&pagFiles))
		{
			if(pagFiles)
			{
				m_pBzp->ReleaseMem(pagFiles);
			}
			CString strErr = m_pBzp->GetLastErrMsg();
			if(!strErr.IsEmpty())
			{
				AfxMessageBox(strErr);
			}
			return NULL;
		}
		else
		{
			_TCHAR szBuf[_MAX_PATH] = {0};
			for(int i= 0; i< pagFiles->GetSize(); i++)
			{
				_tcscpy(szBuf,pagFiles->GetAt(i));				
				m_agBzpFiles.Add(szBuf);
			}
			m_pBzp->ReleaseMem(pagFiles);
			m_strBzpFile = lpszFile;
			if(m_agBzpFiles.GetSize() == 0)
			{
				return NULL;
			}
			else
			{
				m_bBzp = TRUE;
				for(int i=0;i<m_agBzpFiles.GetSize()/2;i++)
				{
					if(m_agBzpFiles[2*i].CompareNoCase(_T("res"))==0)
					{
						return m_agBzpFiles[2*i+1];
					}
				}
				return NULL;
			}
		}
	}
}

BOOL CResParser::PostSaveResFile()
{
	BOOL bRet = FALSE;
	if(m_strBzpFile.IsEmpty())
	{
		return FALSE;
	}

	CFileFind finder;
	if(finder.FindFile(m_strBzpFile.operator LPCTSTR()))
	{
		DWORD dwAtt = ::GetFileAttributes(m_strBzpFile.operator LPCTSTR());
		dwAtt &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(m_strBzpFile.operator LPCTSTR(),dwAtt);
	}

	bRet =	m_pBzp->ResOverGenBzp(m_agBzpFiles,m_strBzpFile.operator LPCTSTR());
			//m_pBzp->GenBzp(m_agBzpFiles,m_strBzpFile.operator LPCTSTR());


	if(!bRet)	
	{
		CString strErr = m_pBzp->GetLastErrMsg();
		if(!strErr.IsEmpty())
		{
			AfxMessageBox(strErr);
		}
	}
	return bRet;
}

void CResParser::SplitLine(char *pLine, CStringArray &agItem)
{	
	USES_CONVERSION;
	agItem.RemoveAll();
	if(pLine == NULL)
	{
		return;
	}

	if(strlen(pLine) == 0)
	{
		return;
	}

	CString strLine(A2T(pLine));
	strLine.TrimLeft();
	strLine.TrimRight();
	if(strLine.GetLength() == 0)
	{
		return;
	}

	CString strID;
	CString strColor;

	int nFind = strLine.Find(_T(','));
	if(nFind == -1)
	{
		return;
	}
	else
	{
		strID = strLine.Left(nFind);
		strID.TrimRight();
		strLine = strLine.Right(strLine.GetLength()-nFind-1);
		strLine.TrimLeft();

	}
	nFind = strLine.Find(_T(','));
	if(nFind == -1)
	{
		strColor = strLine;
	}
	else
	{
		strColor = strLine.Left(nFind);
		strColor.TrimRight();	
	}
	
	agItem.Add(strID);
	agItem.Add(strColor);
}

BOOL CResParser::ExportColorToFile(LPCTSTR lpszFile)
{
	USES_CONVERSION;

	if(lpszFile == NULL)
	{
		return FALSE;
	}

	CFileFind finder;

	if(finder.FindFile(lpszFile))
	{
		DWORD dwAtt = GetFileAttributes(lpszFile);
		dwAtt &= ~FILE_ATTRIBUTE_READONLY;
		SetFileAttributes(lpszFile,dwAtt);
	}

	FILE *pFile = _tfopen(lpszFile,_T("wt"));
	if(pFile == NULL)
	{
		return FALSE;
	}
	
	fprintf(pFile,"%-35s,Color\n","ID");

	int nCount = m_vResItem.size();
	for(int i = 0; i< nCount; i++)
	{		
		RES_ITEM_PTR pRI = m_vResItem[i];	
		if(pRI->uResType == MMI_RT_COLOR)
		{			
			fprintf(pFile,"%-35s,0x%08X\n",T2A(pRI->szDesc),pRI->itemInfo[0].org.dwColor);	
		}
	}

	fclose(pFile);
	pFile = NULL;
	return TRUE;
}

BOOL CResParser::ExportImgInfoToFile(LPCTSTR lpszFile)
{
	USES_CONVERSION;
	
	if(lpszFile == NULL)
	{
		return FALSE;
	}
	
	CFileFind finder;
	
	if(finder.FindFile(lpszFile))
	{
		DWORD dwAtt = GetFileAttributes(lpszFile);
		dwAtt &= ~FILE_ATTRIBUTE_READONLY;
		SetFileAttributes(lpszFile,dwAtt);
	}
	
	FILE *pFile = _tfopen(lpszFile,_T("wt"));
	if(pFile == NULL)
	{
		return FALSE;
	}
	
	fprintf(pFile,"%-35s,%-10s,%-20s,%-20s\n","ID","TYPE","WIDTH","HEIGHT");
	
	int nCount = m_vResItem.size();
	for(int i = 0; i< nCount; i++)
	{		
		RES_ITEM_PTR pRI = m_vResItem[i];	
		if(pRI->uResType == MMI_RT_IMAGE && !pRI->bLogo)
		{			
			CString strImgType = GetImgTypeString(pRI->itemInfo[0].org.wItemType) ;
			fprintf(pFile,"%-35s,%-10s,%-20d,%-20d\n",
				T2A(pRI->szDesc),
				T2A((LPTSTR)strImgType.operator LPCTSTR()),
				pRI->itemInfo[0].org.wWidth,
				pRI->itemInfo[0].org.wHeight);	
		}
	}
	
	fclose(pFile);
	pFile = NULL;
	return TRUE;
}

BOOL CResParser::LoadColorFromFile(LPCTSTR lpszFile, COLOR_MAP &mapColor )
{
	if(lpszFile == NULL)
	{
		return FALSE;
	}

	CString strErr;

	FILE *pFile = _tfopen(lpszFile,_T("rt"));
	if(pFile == NULL)
	{
		strErr.Format(_T("[WARNING]: can not open color file. [%s]"),lpszFile);
		SHOW_MESSAGE(strErr,TRUE);
		return FALSE;
	}
	char szBuf[MAX_PATH] = {0};
	if(fgets(szBuf,MAX_PATH,pFile) == NULL)
	{
		strErr.Format(_T("[WARNING]: color file is empty. [%s]"),lpszFile);
		SHOW_MESSAGE(strErr,TRUE);
		fclose(pFile);
		return TRUE;
	}

	CStringArray agItem;

	SplitLine(szBuf,agItem);

	if(agItem.GetSize() != 2)
	{
		strErr.Format(_T("[WARNING]: color file table title must be \"ID,Color\". [%s]"),lpszFile);
		SHOW_MESSAGE(strErr,TRUE);
		fclose(pFile);
		return FALSE;
	}

	if(agItem[0].CompareNoCase(_T("ID")) != 0 ||
		agItem[1].CompareNoCase(_T("Color")) != 0)
	{
		strErr.Format(_T("[WARNING]: color file table title must be \"ID,Color\". [%s]"),lpszFile);
		SHOW_MESSAGE(strErr,TRUE);
		fclose(pFile);
		return FALSE;
	}

	memset(szBuf,0,sizeof(szBuf));
	while(fgets(szBuf,MAX_PATH,pFile) != NULL)
	{		
		SplitLine(szBuf,agItem);
		if(agItem.GetSize() == 2)
		{
			DWORD dwColor = 0;
			CString strColor = agItem[1];
			strColor.MakeLower();
			_stscanf(strColor,_T("0x%08x"),&dwColor);
			mapColor.SetAt(agItem[0],dwColor);

			strErr.Format(_T("[INFO]: load color %s,0x%08x"),agItem[0],dwColor);
			SHOW_MESSAGE(strErr,FALSE);
	
		}
	}
	
	fclose(pFile);
	return TRUE;
}

BOOL CResParser::ExportThmFolder(LPCTSTR lpszFolder)
{
	int nCount = m_vResItem.size();

	int nThemeIndex = 0;

	CFileFind finder;

	CString strColorFile;
	strColorFile.Format(_T("%s\\color.txt"),lpszFolder);
	ExportColorToFile(strColorFile);

	CString strImageDir;
	strImageDir.Format(_T("%s\\image"),lpszFolder);
	if(!CreateDirectory(strImageDir,NULL))
	{
		if( GetLastError() != ERROR_ALREADY_EXISTS )
		{		
			return FALSE;
		}
	}
	
	CString strImgInfoFile;
	strImgInfoFile.Format(_T("%s\\image_info.csv"),lpszFolder);
	ExportImgInfoToFile(strImgInfoFile);

	for(int i= 0; i< nCount; i++)
	{
		RES_ITEM_PTR pRI = m_vResItem[i];		
		//WORD wItemType = pRI->itemInfo[nThemeIndex].org.wItemType;

		if(pRI->uResType == MMI_RT_IMAGE && !pRI->bLogo)
		{		
			CString strDstFile = pRI->itemInfo[nThemeIndex].org.szFile;
			int nFind = strDstFile.ReverseFind('.');
			if(nFind != -1)
			{
				CString strExt = strDstFile.Right(strDstFile.GetLength()-nFind);
				
				strDstFile.Format(_T("%s\\%s%s"),strImageDir,pRI->szDesc,strExt);
				
				CString strPath = strDstFile;
				nFind = strPath.ReverseFind('\\');
				if(nFind != -1)
				{
					strPath = strPath.Left(nFind);
					g_theApp.CreateDeepDirectory(strPath);
				}

				CopyFile(pRI->itemInfo[nThemeIndex].org.szFile,strDstFile,FALSE);
			}
		}
	}

	return TRUE;

}


int  CResParser::GetRingFiles(LPCTSTR lpszCSCFolder,CStringArray &agFiles,
							  CStringArray &agDesc,CUIntArray &agIndex,
							  CUIntArray &agUnusedcIndex)
{
	agFiles.RemoveAll();
	agDesc.RemoveAll();
	agIndex.RemoveAll();
	agUnusedcIndex.RemoveAll();

	int nCount = m_vResItem.size();
	CString strPath;
	CString strExt;
	int nTmp = 0;

	CMap<CString,LPCTSTR,int,int> mapExt;
	CMap<CString,LPCTSTR,int,int> mapIndex;
	int nRing = 0;
	CUIntArray agRingIndex;
	for(int i=0;i<nCount; i++)
	{
		RES_ITEM_PTR pRI = m_vResItem[i];		
		//WORD wItemType = pRI->itemInfo[0].org.wItemType;
		
		if(pRI->uResType == MMI_RT_RING && pRI->itemInfo[0].wExist && !pRI->itemInfo[0].bNotUsed)
		{
			agRingIndex.Add(i);
			nRing++;
			CString strTmp = pRI->szDesc;		
			int nFind = 0;
			if(strPath.IsEmpty())
			{
				nFind = strTmp.ReverseFind('\\');
				strPath = strTmp;
				if(nFind != -1)
				{
					strPath = strTmp.Left(nFind);
				}
			}	
			nFind = strTmp.ReverseFind('.');
			//strExt=_T(".*");
			if(nFind!=-1)
			{
				strExt = strTmp.Right(strTmp.GetLength()-nFind);
				strExt.MakeLower();
				
				if(!mapExt.Lookup(strExt,nTmp))
				{
					mapExt.SetAt(strExt,1);
				}
			}
			//break;
			strTmp.MakeLower();
			if(!mapIndex.Lookup(strTmp,nTmp))
			{
				mapIndex.SetAt(strTmp,i);
			}

		}
	}

	if(strPath.IsEmpty())
		return 0;

	agIndex.SetSize(nRing);

	strPath.TrimLeft('\\');

	CString strSearch;
	strSearch.Format(_T("%s\\%s\\*%s"),lpszCSCFolder,strPath,strExt);

	WIN32_FIND_DATA wfd = { 0 };
	HANDLE hFind = ::FindFirstFile(strSearch, &wfd);
	BOOL bFind = FALSE;
	if(hFind != NULL)
		bFind = TRUE;

	int nFindRing = 0;	

	while( bFind)
	{		
		//LPCTSTR pExt = _tcsrchr(wfd.cFileName, _T('.'));
		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			continue;
		}
		else // if( pExt != NULL && _tcsicmp(pExt, SZ_FILE_EXT) == 0 )
		{
			CString strExt=  wfd.cFileName;
			int nFind = strExt.ReverseFind('.');
			if(nFind!=-1)
			{
				strExt = strExt.Right(strExt.GetLength()-nFind);
			}	
			strExt.MakeLower();
			if(mapExt.Lookup(strExt,nTmp))
			{
				CString strPathName;
				strPathName.Format(_T("%s\\%s\\%s"),lpszCSCFolder,strPath,wfd.cFileName);
				agFiles.Add(strPathName);
				
				CString strDesc;
				strDesc.Format(_T("%s\\%s"),strPath,wfd.cFileName);
				agDesc.Add(strDesc);

				int nIndex = 0;
				strDesc.MakeLower();
				if(mapIndex.Lookup(strDesc,nIndex))
				{
					agIndex[nFindRing] = nIndex;
					for(int j=0;j<agRingIndex.GetSize();j++)
					{
						if(agRingIndex[j] == (UINT)nIndex)
						{
							agRingIndex.RemoveAt(j);
							break;
						}
					}
				}
				else
				{
					agIndex[nFindRing] = (UINT)(-1);
				}

				nFindRing++;
				if(agFiles.GetSize()>= nRing)
					break;
			}
		} 
		
		bFind = ::FindNextFile(hFind, &wfd);
	}
	if(hFind)
		::FindClose(hFind);

	for(int i=0;i<agIndex.GetSize();i++)
	{
		if(agIndex[i]==-1)
		{
			agIndex[i] = agRingIndex[0];
			agRingIndex.RemoveAt(0);
		}
	}

	agUnusedcIndex.Copy(agRingIndex);

	return agFiles.GetSize();

}


BOOL CResParser::LoadThmFolder(LPCTSTR lpszFolder)
{
	int nCount = m_vResItem.size();

	BOOL bReplaceOK = FALSE;

	int nThemeIndex = 0;

	CFileFind finder;

	CString strColorFile;
	strColorFile.Format(_T("%s\\color.txt"),lpszFolder);

	COLOR_MAP mapColor;

	CString strErr;

	if(finder.FindFile(strColorFile))
	{
		LoadColorFromFile(strColorFile,mapColor);
	}	
	else
	{
		strErr.Format(_T("[WARNING]: not find color file. [%s]"),strColorFile);
		SHOW_MESSAGE(strErr,FALSE);
	}

	int nImgCount = 0;
	int nColorCount = 0;
	int nImgRepCount = 0;
	int nColorRepCount = 0;

	for(int i= 0; i< nCount; i++)
	{
		RES_ITEM_PTR pRI = m_vResItem[i];		
		WORD wItemType = pRI->itemInfo[0].org.wItemType;

		if(pRI->uResType == MMI_RT_IMAGE && !pRI->bLogo)
		{
			CString strFilter;		

			if( wItemType == IMG_BMP )
			{
				strFilter = _T(".bmp");
			}
			else if( wItemType == IMG_GIF )
			{
				strFilter = _T(".gif");
			}
			else if( wItemType == IMG_JPG )
			{
				strFilter = _T(".jpg");
			}
			else if( wItemType == IMG_PNG )
			{
				strFilter = _T(".png");
			}
			else
			{
				strFilter = _T(".*");
			}

			nImgCount++;

			CString strPath;
			CString strTmp = pRI->szDesc;
			if(strTmp.Right(4).CompareNoCase(strFilter)!=0)
			{
				strPath.Format(_T("%s\\image\\%s%s"),lpszFolder,pRI->szDesc,strFilter);
			}

			if(finder.FindFile(strPath))
			{
				finder.Close();
				bReplaceOK = RelaceImag(i,nThemeIndex,strPath.operator LPCTSTR());
				if(bReplaceOK)
				{
					nImgRepCount++;
					strErr.Format(_T("[INFO]: image [%s] replaced success."),pRI->szDesc);
					SHOW_MESSAGE(strErr,FALSE);
				}
				else
				{
					strErr.Format(_T("[WARNING]: image [%s] replaced failed."),strPath);
					SHOW_MESSAGE(strErr,FALSE);
				}
			}
			else
			{
				strErr.Format(_T("[WARNING]: not find image file. [%s]"),strPath);
				SHOW_MESSAGE(strErr,FALSE);
			}
			
		}
		else if(pRI->uResType == MMI_RT_COLOR)
		{		
			nColorCount++;
			DWORD dwColor = 0;
			if(mapColor.Lookup(pRI->szDesc,dwColor))
			{
				nColorRepCount++;
				//dwColor = g_theApp.Color24To565(dwColor);
				bReplaceOK = RelaceColor(i,nThemeIndex,dwColor);
				strErr.Format(_T("[INFO]: color [%s] replaced success."),pRI->szDesc);
				SHOW_MESSAGE(strErr,FALSE);
			}
			else
			{
				strErr.Format(_T("[WARNING]: not find color ID [%s]."),pRI->szDesc);
				SHOW_MESSAGE(strErr,FALSE);
			}
			
		}
	}

	strErr.Format(_T("\n\n\t------------------------------"));
	SHOW_MESSAGE(strErr,FALSE);
	strErr.Format(_T("\t  Replace images %d/%d."),nImgRepCount,nImgCount);
	SHOW_MESSAGE(strErr,FALSE);
	strErr.Format(_T("\t  Replace color %d/%d."),nColorRepCount,nColorCount);
	SHOW_MESSAGE(strErr,FALSE);
	strErr.Format(_T("\t------------------------------\n\n"));
	SHOW_MESSAGE(strErr,FALSE);

	return bReplaceOK;
}

BOOL  CResParser::EmptyLangText(int nItemIndex,int nThemeIndex)
{
	if(m_pNewBuf == NULL)
	{
		return FALSE;
	}
	
	RES_ITEM_PTR pRI = m_vResItem[nItemIndex];

	if(	nThemeIndex != 0||
		pRI->uResType != MMI_RT_MAX_NUM ||
		pRI->itemInfo[nThemeIndex].wExist == 0 || 
		(pRI->itemInfo[nThemeIndex].wModified == 0 
		&& !pRI->itemInfo[nThemeIndex].bNotUsed) )
	{
		return TRUE;
	}
	

	BYTE *pBaseBuf = NULL;
	if(m_bThemeOnly)
	{
		pBaseBuf = m_pNewBuf;
	}
	else
	{
		DWORD dwThemeOffset = * (DWORD*)(m_pNewBuf+ DEFAULT_RES_INFO_OFFSET + nThemeIndex*8); //lint !e613
		CONV_DWORD_EX(dwThemeOffset);
		
		pBaseBuf = m_pNewBuf + dwThemeOffset;//lint !e613
	}

	DWORD * pMduOffset = (DWORD*)(pBaseBuf + 20);
	DWORD dwFirstMduOffset = *pMduOffset;//lint !e613
	CONV_DWORD_EX(dwFirstMduOffset);//lint !e613

	int nMaxMduNum = (dwFirstMduOffset - 20 - sizeof(MMI_MODULE_VER_INFO_T))/4;
	int nUsedMduNum = 0;
	CUIntArray agMudIndexs;
	int i=0;
	for(i=0 ;i< nMaxMduNum; i++)
	{
		if(pMduOffset[i]!=0)
		{
			nUsedMduNum++;
			agMudIndexs.Add(i);
		}
	}

	for(i=0 ;i< nUsedMduNum; i++)
	{
		pRI->uModuleID = agMudIndexs[i];
		EmptyMduLangText(nThemeIndex,pRI);
	}	

	return TRUE;
}

BOOL CResParser::EmptyMduLangText(int nThemeIndex,RES_ITEM_PTR pRI)
{
	if(m_pNewBuf == NULL)
	{
		return FALSE;
	}

	DWORD dwEnDataStart = 0;		
	DWORD dwEnDataEnd = 0;
	DWORD textDataOfffset = 0;	
	DWORD dwOrgSize = GetMduLangTextSize(m_pNewBuf,(int)pRI->uModuleID,(int)pRI->uResIndex,
										 dwEnDataStart,dwEnDataEnd,
	                                     textDataOfffset);
	DWORD dwRepSize = 0;
	
	int nDeltAlign =  (int)ALIGN4_SIZE(dwRepSize) - (int)ALIGN4_SIZE(dwOrgSize);

	if(nDeltAlign == 0)
	{
		return TRUE;
	}
	
	DWORD dwOffsetTmp =0;
	DWORD dwSizeTmp =0;
	UINT i = 0;
	
	BYTE *pBaseBuf = MidifyHdrOffset(nDeltAlign,nThemeIndex,pRI);
	
	// 记录没有改变编译以前的TEXT数据
	DWORD dwTextOffset = * (DWORD*)(pBaseBuf+MMI_RT_TEXT*12);
	DWORD dwTextNum = * (DWORD*)(pBaseBuf+MMI_RT_TEXT*12+4);
	DWORD dwTextSize = * (DWORD*)(pBaseBuf+MMI_RT_TEXT*12+8);
	CONV_DWORD_EX(dwTextOffset);
	CONV_DWORD_EX(dwTextNum);
	CONV_DWORD_EX(dwTextSize);
	
	// 先修改偏移，然后再移动内存，插入数据
	if(nDeltAlign != 0)
	{
		// 不需要修改IMG 偏移			
		// 不需要修改Anim 偏移		
		// 不需要修改RING 偏移	
		// 不需要修改FONT偏移
		// 修改TEXT 偏移
		ModifyTextOffset2(pBaseBuf,nDeltAlign,pRI->uResIndex,dwEnDataStart,dwEnDataEnd);
		// 修改Data 偏移
		ModifyDataOffset(pBaseBuf,nDeltAlign,0);
		
		// 修改资源类型偏移	
		for( i = MMI_RT_OTHER; i< MMI_RT_MAX_NUM; i++)
		{
			dwOffsetTmp = *(DWORD*)(pBaseBuf + i*12);
			CONV_DWORD_EX(dwOffsetTmp);
			if(dwOffsetTmp != 0)
			{
				dwOffsetTmp += (DWORD)nDeltAlign;
				CONV_DWORD_EX(dwOffsetTmp);
				*(DWORD*)(pBaseBuf + i*12) = dwOffsetTmp;
			}
		}
		// 修改Text 的总size
		dwSizeTmp = dwTextSize;
		if(dwSizeTmp != 0)
		{
			dwSizeTmp += (DWORD)nDeltAlign;
			CONV_DWORD_EX(dwSizeTmp);
			*(DWORD*)(pBaseBuf+MMI_RT_TEXT*12+8) = dwSizeTmp;
		}

		BYTE *pLangInfoBuf = pBaseBuf+ (dwTextOffset + pRI->uResIndex*sizeof(KSP_TEXT_T));
		DWORD textIndexOfffset = * (DWORD*)pLangInfoBuf;
		CONV_DWORD_EX(textIndexOfffset);

		// 修改Lang Info 中的Offset 和 Size	
		*(DWORD*)(pLangInfoBuf) = 0;
		*(WORD*)(pLangInfoBuf+4) = 0;		
		
		//移动内存
		//DWORD dwTotalOffset = (DWORD)((pBaseBuf+ringDataOfffset) - m_pNewBuf) + ALIGN4_SIZE(dwOrgSize);
		//memmove(m_pNewBuf+(dwTotalOffset+(DWORD)nDeltAlign),m_pNewBuf + dwTotalOffset, m_dwCurTotalSize - dwTotalOffset);
		//memset(m_pNewBuf+dwTotalOffset-ALIGN4_SIZE(dwOrgSize),0, ALIGN4_SIZE(dwRepSize));
		//移动内存,分两次完成
		//先移动Data，即字符串的数据
		DWORD dwOffsetStart = (DWORD)((pBaseBuf+textDataOfffset) - m_pNewBuf);
		DWORD dwOffsetEnd = dwOffsetStart + dwOrgSize-dwTextNum*4;
		memmove(m_pNewBuf+dwOffsetStart,m_pNewBuf + dwOffsetEnd, m_dwCurTotalSize - dwOffsetEnd);		

		//再移动Offset，即字符串的索引
		dwOffsetStart = (DWORD)((pBaseBuf+textIndexOfffset) - m_pNewBuf);
		dwOffsetEnd = dwOffsetStart + dwTextNum*4;
		memmove(m_pNewBuf+dwOffsetStart,m_pNewBuf + dwOffsetEnd, m_dwCurTotalSize - dwOffsetEnd);	
	}	
	m_dwCurTotalSize += (DWORD)nDeltAlign;
	
	return TRUE;	
}

DWORD CResParser::GetLangTextSize(LPBYTE pResBuf,int nLangID)
{
	DWORD dwLangTextSize = 0;
	if(pResBuf == NULL)
	{
		return dwLangTextSize;
	}
	int nThemeIndex = 0;	
	
	BYTE *pBaseBuf = NULL;
	if(m_bThemeOnly)
	{
		pBaseBuf = pResBuf;
	}
	else
	{
		DWORD dwThemeOffset = * (DWORD*)(pResBuf+ DEFAULT_RES_INFO_OFFSET + nThemeIndex*8); //lint !e613
		CONV_DWORD_EX(dwThemeOffset);
		
		pBaseBuf = pResBuf + dwThemeOffset;//lint !e613
	}
	
	DWORD * pMduOffset = (DWORD*)(pBaseBuf + 20);
	DWORD dwFirstMduOffset = *pMduOffset;//lint !e613
	CONV_DWORD_EX(dwFirstMduOffset);//lint !e613
	
	int nMaxMduNum = (dwFirstMduOffset - 20 - sizeof(MMI_MODULE_VER_INFO_T))/4;
	int nUsedMduNum = 0;
	CUIntArray agMudIndexs;
	int i=0;
	for(i=0 ;i< nMaxMduNum; i++)
	{
		if(pMduOffset[i]!=0)
		{
			nUsedMduNum++;
			agMudIndexs.Add(i);
		}
	}

	DWORD dwEnDataStart = 0;		
	DWORD dwEnDataEnd = 0;
	DWORD textDataOfffset = 0;	
	
	for(i=0 ;i< nUsedMduNum; i++)
	{
		dwLangTextSize += GetMduLangTextSize(pResBuf,agMudIndexs[i],nLangID,dwEnDataStart,dwEnDataEnd,textDataOfffset);
	}	
	
	return dwLangTextSize;
}

DWORD CResParser::GetMduLangTextSize(LPBYTE pResBuf,int nMduID,int nLangID, DWORD &dwEnDataStart,DWORD &dwEnDataEnd,DWORD &dwOffsetStart)
{
	BYTE *pMduBaseBuf = NULL;
	int nThemeIndex=0;
	if(m_bThemeOnly)
	{
		pMduBaseBuf = pResBuf;
	}
	else
	{
		DWORD dwThemeOffset = * (DWORD*)(pResBuf+ DEFAULT_RES_INFO_OFFSET + nThemeIndex*8); //lint !e613
		CONV_DWORD_EX(dwThemeOffset);
		
		pMduBaseBuf = pResBuf + dwThemeOffset;//lint !e613
	}

	DWORD dwiMduOffset = * (DWORD*)(pMduBaseBuf + (20 + nMduID * 4));//lint !e613
	CONV_DWORD_EX(dwiMduOffset);//lint !e613
	pMduBaseBuf += dwiMduOffset;//lint !e613

	
	BYTE *pBaseBuf = pMduBaseBuf + MMI_RT_TEXT*12;
	
    DWORD dwOffset = *(DWORD*)(pBaseBuf);
	DWORD dwNum    = *(DWORD*)(pBaseBuf+4);
	DWORD dwSize   = *(DWORD*)(pBaseBuf+8);
	CONV_DWORD_EX(dwOffset);
	CONV_DWORD_EX(dwNum);
	CONV_DWORD_EX(dwSize);
	
	if(dwNum == 0 || dwOffset==0)
	{
		return 0;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 确定英文区域字符串的offset范围
	// 对于跨语言重复的字符串，前几个offset可能指向英文区域
	UINT i= 0;
	DWORD dwEnIndexOffset = *(DWORD*)(pMduBaseBuf+ dwOffset);
	CONV_DWORD_EX(dwEnIndexOffset);
	DWORD *pEnDataOffset = (DWORD*)(pMduBaseBuf+ dwEnIndexOffset);
	
	dwEnDataStart = *pEnDataOffset;	
	CONV_DWORD_EX(dwEnDataStart);
	
	dwEnDataEnd = dwEnDataStart;
	for(i= 1; i<dwNum; i++)
	{
		DWORD dwTmp = *(pEnDataOffset+i);
		CONV_DWORD_EX(dwTmp);
		
		if(dwTmp > dwEnDataEnd )
		{
			dwEnDataEnd = dwTmp;	
		}	
	}	

	DWORD textStartOfffset = 0;
	DWORD textEndOfffset   = 0;

	if(nLangID == 0)
	{
		textStartOfffset = dwEnDataStart;
		textEndOfffset   = dwEnDataEnd;
	}
	else
	{  
		//处理当前LangID	
		BYTE *pLangInfoBuf = pMduBaseBuf+ (dwOffset + nLangID*sizeof(KSP_TEXT_T));	
		DWORD textIndexOfffset = * (DWORD*)pLangInfoBuf;
		CONV_DWORD_EX(textIndexOfffset);	
		DWORD *pLangDataOffset = (DWORD*)(pMduBaseBuf+ textIndexOfffset);
		
		// 确定LangID区域的开始offset
		textStartOfffset = *pLangDataOffset;
		CONV_DWORD_EX(textStartOfffset);
		
		if(textStartOfffset >=dwEnDataStart && textStartOfffset<= dwEnDataEnd  )
		{
			for(i=1; i<dwNum; i++)
			{
				textStartOfffset = * (DWORD*)(pLangDataOffset+i);
				CONV_DWORD_EX(textStartOfffset);
				if(textStartOfffset > dwEnDataEnd  )
				{
					break;
				}
			}

			if(i>=dwNum)
			{
				return 0;
			}
		}	

		// 确定LangID区域的结束的offset
		textEndOfffset = textStartOfffset;
		for(i=1; i<dwNum; i++)
		{
			DWORD dwTmp = * (DWORD*)(pLangDataOffset+i);
			CONV_DWORD_EX(dwTmp);
			if(dwTmp > textEndOfffset)
			{
				textEndOfffset = dwTmp;
			}
		}
	}

	DWORD dwStrSize = 0;
	LPBYTE pLastStr = pMduBaseBuf+textEndOfffset;
	if(*pLastStr == 0x80)
	{
		dwStrSize = *(WORD*)(pLastStr+2);
		CONV_SHORT_EX(dwStrSize);
		dwStrSize += 4; // flag(1)+reserved(1)+size(2)
	}
	else
	{
		while(*(pLastStr++) != 0)
			dwStrSize++;
		dwStrSize += 1; // '\0'
	}
	dwStrSize = ALIGN4_SIZE(dwStrSize);
	
	DWORD dwCalcLangToTalSize= textEndOfffset - textStartOfffset + dwStrSize + dwNum*4;
	
	//////////////////////////////////////////////////////////////////////////
	
	int nLangNum = 0;
	
	DWORD dwFirstOffset= *(DWORD*)(pMduBaseBuf+ dwOffset);
	CONV_DWORD_EX(dwFirstOffset);
	
	nLangNum = (dwFirstOffset - dwOffset) / sizeof(KSP_TEXT_T);
	_ASSERT(nLangNum != 0);	
	
	PKSP_TEXT_T pt = (KSP_TEXT_T*)(pMduBaseBuf+ (dwOffset + (UINT)nLangID*sizeof(KSP_TEXT_T)));
	DWORD dwLangOffset = (DWORD)pt->pTxtTable;
	WORD  wLangSize = (WORD)pt->nbText;
	CONV_DWORD_EX(dwLangOffset);
	CONV_SHORT_EX(wLangSize);

	DWORD  dwLangToTalSize = 0;

	DWORD dwLangSize = wLangSize;
	if(dwLangOffset != NULL && dwLangSize != 0)
	{
		dwLangToTalSize = dwNum*4+dwLangSize*4;
	}

	_ASSERT(dwLangToTalSize == dwCalcLangToTalSize);

	dwOffsetStart = textStartOfffset;

	return dwCalcLangToTalSize;

}

void CResParser::ModifyDataLDB()
{
	typedef CMap<CString, LPCTSTR, int, int>   LDB_MAP;
	LDB_MAP mapLDB;

	m_dwTextRepSize = 0;

	int nCount = m_vResItem.size();
	for(int i = 0; i< nCount; i++)
	{
		if( m_vResItem[i]->uResType != MMI_RT_MAX_NUM ||
			m_vResItem[i]->itemInfo[0].wExist == 0 || 
			m_vResItem[i]->itemInfo[0].bNotUsed)
		{
			continue;
		}
		CString strLang = m_vResItem[i]->szDesc;

		m_dwTextRepSize+= GetLangTextSize(m_pResBuf,(int)m_vResItem[i]->uResIndex);

		//ENGLISH
		if(strLang.CompareNoCase(_T("ENGLISH"))==0)
		{
			mapLDB.SetAt(_T("l0109b00.ldb"),1);
			mapLDB.SetAt(_T("l0109b01.ldb"),1);
			mapLDB.SetAt(_T("l0109b02.ldb"),1);
		}
		//ARABIC
		else if(strLang.CompareNoCase(_T("ARABIC"))==0)
		{
			mapLDB.SetAt(_T("l0101b00.ldb"),1);
			mapLDB.SetAt(_T("l0101b01.ldb"),1);
			mapLDB.SetAt(_T("l0101b02.ldb"),1);
			mapLDB.SetAt(_T("l0101b03.ldb"),1);
			mapLDB.SetAt(_T("l0101b04.ldb"),1);
			mapLDB.SetAt(_T("l0101b05.ldb"),1);
			mapLDB.SetAt(_T("l0101b06.ldb"),1);
			mapLDB.SetAt(_T("l0101b07.ldb"),1);
			mapLDB.SetAt(_T("l0101b08.ldb"),1);
			mapLDB.SetAt(_T("l0101b09.ldb"),1);
		}
		//FRENCH
		else if(strLang.CompareNoCase(_T("FRENCH"))==0)
		{
			mapLDB.SetAt(_T("l0112b00.ldb"),1);
			mapLDB.SetAt(_T("l0112b01.ldb"),1);
			mapLDB.SetAt(_T("l0112b02.ldb"),1);
		}
		//GERMAN
		else if(strLang.CompareNoCase(_T("GERMAN"))==0)
		{
			mapLDB.SetAt(_T("l0107b00.ldb"),1);
			mapLDB.SetAt(_T("l0107b01.ldb"),1);
			mapLDB.SetAt(_T("l0107b02.ldb"),1);
		}
		//SPANISH
		else if(strLang.CompareNoCase(_T("SPANISH"))==0)
		{
			mapLDB.SetAt(_T("l0110b00.ldb"),1);
			mapLDB.SetAt(_T("l0110b01.ldb"),1);
			mapLDB.SetAt(_T("l0110b02.ldb"),1);
		}
		//PERSIAN
		else if(strLang.CompareNoCase(_T("PERSIAN"))==0)
		{
			mapLDB.SetAt(_T("l0141b00.ldb"),1);
			mapLDB.SetAt(_T("l0141b01.ldb"),1);
			mapLDB.SetAt(_T("l0141b02.ldb"),1);
			mapLDB.SetAt(_T("l0141b03.ldb"),1);
			mapLDB.SetAt(_T("l0141b04.ldb"),1);
			mapLDB.SetAt(_T("l0141b05.ldb"),1);
			mapLDB.SetAt(_T("l0141b06.ldb"),1);
			mapLDB.SetAt(_T("l0141b07.ldb"),1);
		}
		//HINDI
		else if(strLang.CompareNoCase(_T("HINDI"))==0)
		{
			mapLDB.SetAt(_T("l0157b00.ldb"),1);
			mapLDB.SetAt(_T("l0157b01.ldb"),1);
		}
		//INDONESIAN
		else if(strLang.CompareNoCase(_T("INDONESIAN"))==0)
		{
			mapLDB.SetAt(_T("l0133b00.ldb"),1);
			mapLDB.SetAt(_T("l0133b01.ldb"),1);
			mapLDB.SetAt(_T("l0133b02.ldb"),1);
			mapLDB.SetAt(_T("l0133b03.ldb"),1);
		}
		//PORTUGUESE
		else if(strLang.CompareNoCase(_T("PORTUGUESE"))==0)
		{
			mapLDB.SetAt(_T("l0122b00.ldb"),1);
			mapLDB.SetAt(_T("l0122b01.ldb"),1);
			mapLDB.SetAt(_T("l0122b02.ldb"),1);
		}
		//RUSSIAN
		else if(strLang.CompareNoCase(_T("PORTUGUESE"))==0)
		{
			mapLDB.SetAt(_T("l0125b00.ldb"),1);
			mapLDB.SetAt(_T("l0125b01.ldb"),1);
			mapLDB.SetAt(_T("l0125b02.ldb"),1);
			mapLDB.SetAt(_T("l0125b03.ldb"),1);
			mapLDB.SetAt(_T("l0125b04.ldb"),1);
			mapLDB.SetAt(_T("l0125b05.ldb"),1);		
		}
		//SWAHILI
		else if(strLang.CompareNoCase(_T("SWAHILI"))==0)
		{
			mapLDB.SetAt(_T("l0165b00.ldb"),1);
			mapLDB.SetAt(_T("l0165b01.ldb"),1);
			mapLDB.SetAt(_T("l0165b02.ldb"),1);
			mapLDB.SetAt(_T("l0165b03.ldb"),1);
			mapLDB.SetAt(_T("l0165b04.ldb"),1);
			mapLDB.SetAt(_T("l0165b05.ldb"),1);
			mapLDB.SetAt(_T("l0165b06.ldb"),1);
		}
		//THAI
		else if(strLang.CompareNoCase(_T("THAI"))==0)
		{
			mapLDB.SetAt(_T("l0130b00.ldb"),1);
			mapLDB.SetAt(_T("l0130b01.ldb"),1);
			mapLDB.SetAt(_T("l0130b02.ldb"),1);
			mapLDB.SetAt(_T("l0130b03.ldb"),1);
		}
		//TURKISH
		else if(strLang.CompareNoCase(_T("TURKISH"))==0)
		{
			mapLDB.SetAt(_T("l0131b00.ldb"),1);
			mapLDB.SetAt(_T("l0131b01.ldb"),1);
			mapLDB.SetAt(_T("l0131b02.ldb"),1);
			mapLDB.SetAt(_T("l0131b03.ldb"),1);
			mapLDB.SetAt(_T("l0131b04.ldb"),1);
			mapLDB.SetAt(_T("l0131b05.ldb"),1);
			mapLDB.SetAt(_T("l0131b06.ldb"),1);
			mapLDB.SetAt(_T("l0131b07.ldb"),1);
		}
		//VIETNAMESE
		else if(strLang.CompareNoCase(_T("VIETNAMESE"))==0)
		{
			mapLDB.SetAt(_T("l0142b00.ldb"),1);
		}
	}	

	m_dwLDBRepSize = 0;

	for(int i = 0; i< nCount; i++)
	{
		if( m_vResItem[i]->uResType != MMI_RT_OTHER ||
			m_vResItem[i]->itemInfo[0].wExist == 0 )
		{
			continue;
		}

		CString strName = m_vResItem[i]->szDesc;
		if(strName.Right(4).CompareNoCase(_T(".ldb")) != 0)
		{
			continue;
		}

		int nFind = strName.ReverseFind('\\');
		if(nFind != -1)
		{
			strName = strName.Right(strName.GetLength()-nFind-1);			
		}
		strName.TrimLeft();
		strName.TrimRight();
		int nTmp;
		if(!mapLDB.Lookup(strName,nTmp))
		{
			m_vResItem[i]->itemInfo[0].bNotUsed = TRUE;
		}
		else
		{
			m_vResItem[i]->itemInfo[0].bNotUsed = FALSE;	
			m_dwLDBRepSize += ALIGN4_SIZE(m_vResItem[i]->itemInfo[0].org.dwSize);
		}
	}
}

BOOL CResParser::SaveResInfo()
{
	if(m_pNewBuf == NULL)
		return FALSE;

    int nTblNum = m_pNewBuf[13];
	if(0 == nTblNum )
	{	
		return TRUE; // 没有需要替换的资源
	}

	DWORD dwOffset = 0;
	DWORD dwSize = 0;
	LPBYTE pCurBuf = NULL;

	if(m_bThemeOnly)
	{	
		DWORD dwTotalSize = *(DWORD*)(m_pNewBuf+16);
		CONV_DWORD_EX(dwTotalSize);
		pCurBuf = m_pNewBuf + dwTotalSize;
	}
	else
	{
		pCurBuf = m_pNewBuf + LOGO_OFFSET;
	} 	

	for(int i = 0 ; i< nTblNum; i++)
	{
		 if( pCurBuf[0] == 'c' &&
			 pCurBuf[1] == 's' &&
			 pCurBuf[2] == 'c' &&
			 pCurBuf[3] == 's' )
		 {
			 dwOffset = *(DWORD*)(pCurBuf+4);
			 CONV_DWORD_EX(dwOffset);
			 dwSize = *(DWORD*)(pCurBuf+8);
			 CONV_DWORD_EX(dwSize);
			 if(!SaveCscs(dwOffset, dwSize, (DWORD*)pCurBuf))
			 {
				 return FALSE;
			 }
		 }
		 else if( pCurBuf[0] == 'l' &&
			 pCurBuf[1] == 'a' &&
			 pCurBuf[2] == 'n' &&
			 pCurBuf[3] == 'g' )
		 {
			 dwOffset = *(DWORD*)(pCurBuf+4);
			 CONV_DWORD_EX(dwOffset);
			 dwSize = *(DWORD*)(pCurBuf+8);
			 CONV_DWORD_EX(dwSize);
			 if(!SaveLang(dwOffset, dwSize))
			 {
				 return FALSE;
			 }
		 }

		 pCurBuf+= 12;
	}

	return  TRUE;
}

BOOL CResParser::SaveLang(DWORD dwOffset, DWORD dwSize)
{	
	if(m_pNewBuf == NULL)
		return FALSE;
	
	if((dwOffset+dwSize)> (DWORD)m_nNewSize)
	{
		return FALSE;
	}
	
	BYTE *pBuf = m_pNewBuf + dwOffset;
	
	// 去掉版本信息
	pBuf += 4; 
	dwSize -=4; 	

	int nCount = m_vResItem.size();
	CUIntArray agState;
	for(int i=0; i< nCount;i++)
	{
		if(m_vResItem[i]->uResType != MMI_RT_MAX_NUM)
			continue;

		if(m_vResItem[i]->itemInfo[0].wExist == 0 || m_vResItem[i]->itemInfo[0].bNotUsed)
		{
			agState.Add(1);
		}
		else
		{				
			agState.Add(0);
		}
	}
	
	DWORD dwLeftSize = dwSize;
	//MMI_LANG_TAG_T repl = {0};
	int i = 0;
	while(((int)dwLeftSize)>0)
	{	
		WORD wState= (WORD)agState[i];
		CONV_SHORT_EX(wState);
		*(WORD*)(pBuf+2) = wState;
		
		DWORD dwTmpSize = sizeof(MMI_LANG_TAG_T);
		pBuf += dwTmpSize;
		dwLeftSize-= dwTmpSize;
		i++;
	}
	
	return TRUE;

}

BOOL CResParser::SaveCscs(DWORD dwOffset, DWORD dwSize,DWORD* pCscs)
{
	USES_CONVERSION;

	if(m_pNewBuf == NULL)
		return FALSE;
	
// 	if((dwOffset+dwSize)> (DWORD)m_nNewSize)
// 	{
// 		return FALSE;
// 	}
	
	BYTE *pBuf = m_pNewBuf + dwOffset;
	
	// 去掉版本信息
	pBuf += 4; 
	dwSize -=4; 	
	
	int nCount = m_vResItem.size();
	CUIntArray agState;
	CUIntArray agIndex;
	RES_ITEM_PTR pRI = NULL;

	for(int i=0; i< nCount;i++)
	{
		pRI = m_vResItem[i];
		if(pRI->uResType == MMI_RT_MAX_NUM)
			continue;
		
		agIndex.Add(i);
		if(pRI->itemInfo[0].wExist == 0 || pRI->itemInfo[0].bNotUsed)
		{
			agState.Add(1);
		}
		else
		{				
			agState.Add(0);
		}
	}

	DWORD dwDeltSize = 0;
	
	DWORD dwLeftSize = dwSize;
	MMI_TAG2_T repl = {0};
	int i = 0;
	while(((int)dwLeftSize)>0)
	{	
		memcpy(&repl,pBuf,sizeof(repl));
		BYTE bNotUsed = (BYTE)agState[i];
		*(BYTE*)(pBuf+2) = bNotUsed;

		int nIndex = agIndex[i];

		pRI = m_vResItem[nIndex];

		DWORD dwTmpSize = GetMMITagSize(repl);
		dwLeftSize-= dwTmpSize;
		if(pRI->itemInfo[0].wExist == 1 && !pRI->itemInfo[0].bNotUsed && _tcslen(pRI->szNewDesc)!=0)
		{
			DWORD dwOldLen = strlen(T2A(pRI->szDesc))+1;
			DWORD dwNewLen = strlen(T2A(pRI->szNewDesc))+1;
			int nDeltSize  = (int)(ALIGN4_SIZE(dwNewLen) - ALIGN4_SIZE(dwOldLen)); 
			dwDeltSize += (DWORD)nDeltSize;

			DWORD dwTotalOffset = (DWORD)(pBuf - m_pNewBuf) + dwTmpSize;		
			memmove(m_pNewBuf+(dwTotalOffset+(DWORD)nDeltSize),m_pNewBuf + dwTotalOffset, m_dwCurTotalSize - dwTotalOffset);
			memset(m_pNewBuf+dwTotalOffset-dwTmpSize+12,0, ALIGN4_SIZE(dwNewLen));
			strcpy((char*)(m_pNewBuf+dwTotalOffset-dwTmpSize+12),T2A(pRI->szNewDesc));

			m_dwCurTotalSize += (DWORD)nDeltSize;
			pBuf += (dwTmpSize+(DWORD)nDeltSize);
		
		}
		else
		{
			pBuf += dwTmpSize;			
		}		
		
		i++;
	}

	DWORD dwSize2 = dwSize+dwDeltSize;
	CONV_DWORD_EX(dwSize2);

	*(pCscs+2) = dwSize2;
	
	return TRUE;
}

BOOL CResParser::DeleteResItem(int nItemIndex,int nThemeIndex /*= 0*/)
{
	UndoResItem(nItemIndex,nThemeIndex);

	RES_ITEM_PTR pRI = m_vResItem[nItemIndex];

	int nDeltAlign = 0 - ALIGN4_SIZE(pRI->itemInfo[nThemeIndex].org.dwSize);	
	
	pRI->itemInfo[nThemeIndex].wModified = TRUE;
	SAFE_DEL(pRI->itemInfo[nThemeIndex].rep.pBuf);
	pRI->itemInfo[nThemeIndex].rep.dwSize = 0;
	memset(pRI->itemInfo[nThemeIndex].rep.szFile,0,_MAX_PATH);
	pRI->itemInfo[nThemeIndex].bNotUsed = TRUE;
	pRI->isModified = TRUE;
	m_nNewSize += nDeltAlign;
	return TRUE;
}

CString CResParser::GetIMLang(int nItemIndex,int nThemeIndex)
{
	UNUSED_ALWAYS(nThemeIndex);
	CString strTmp = m_vResItem[nItemIndex]->szDesc;
	if(strTmp.Right(4).CompareNoCase(_T(".ldb")) != 0)
		return _T("");
	
	int nFind = strTmp.ReverseFind('\\');
	if(nFind != -1)
		strTmp = strTmp.Left(nFind);
	
	nFind = strTmp.ReverseFind('\\');
	if(nFind != -1)
		strTmp = strTmp.Right(strTmp.GetLength()-nFind -1);	

	return strTmp;
}

void  CResParser::GetIMInfo()
{
	m_dwLDBOrgSize = 0;
	m_dwLDBRepSize = 0;
	m_mapIM.RemoveAll();

	UINT nSize = m_vResItem.size();

	for(UINT i = 0; i< nSize; i++)
	{		
		if(	m_vResItem[i]->uResType == MMI_RT_OTHER && 
			m_vResItem[i]->itemInfo[0].wExist /*&& 
			!m_vResItem[i]->itemInfo[0].bNotUsed*/) 
		{
			CString strIMLang = GetIMLang(i,0);	
			if(strIMLang.IsEmpty())
				continue;		
				
			DWORD dwIMSize = ALIGN4_SIZE(m_vResItem[i]->itemInfo[0].org.dwSize);
			m_dwLDBOrgSize += dwIMSize;

			IM_ITEM im={0};
			if(!m_mapIM.Lookup(strIMLang,im))
			{				
				m_agIMName.Add(strIMLang);
				im.index[im.iCount++] = i;
				im.iIMOrgSize = dwIMSize;
				im.iIMRepSize = dwIMSize;
			}
			else
			{
				im.iIMOrgSize += dwIMSize;
				im.iIMRepSize += dwIMSize;
				im.index[im.iCount++] = i;
			}
			m_mapIM.SetAt(strIMLang,im);
		}	
	}
	
	m_dwLDBRepSize = m_dwLDBOrgSize;
}

void  CResParser::ModifyIMLdb(LPCTSTR lpszIM,BOOL bDelete)
{
	IM_ITEM im = {0};

	if(!m_mapIM.Lookup(lpszIM,im))
		return;

	for(int i= 0; i< im.iCount; i++)
	{
		int nIndex = im.index[i];
		m_vResItem[nIndex]->itemInfo[0].bNotUsed = bDelete;
	}

	im.bNotUsed = bDelete;

	m_mapIM.SetAt(lpszIM,im);
}

void CResParser::ModifyIM()
{
	m_dwLDBRepSize = 0;
	//IM_ITEM im = {0};
	
	for(int i=0; i<m_agIMName.GetSize();i++)
	{
		CString strIM = m_agIMName[i];
		IM_ITEM im = {0};
		if(m_mapIM.Lookup(strIM,im))
		{
			if(!im.bNotUsed)
			{
				m_dwLDBRepSize += im.iIMOrgSize;
				DWORD nSize=0; 
				for(int j=0;j<im.iCount;j++)
				{
					RES_ITEM_PTR pRI= m_vResItem[im.index[j]];
					if(pRI->itemInfo[0].wModified)
					{
						nSize+= pRI->itemInfo[0].rep.dwSize;
					}
					else
					{
						nSize+= pRI->itemInfo[0].org.dwSize;
					}
				}
				im.iIMRepSize = (int)nSize;
				m_mapIM.SetAt(strIM,im);
			}
		}
	}
}

void CResParser::ModifyLang()
{
	m_dwTextRepSize = 0;
	
	int nCount = m_vResItem.size();
	for(int i = 0; i< nCount; i++)
	{
		if( m_vResItem[i]->uResType != MMI_RT_MAX_NUM ||
			m_vResItem[i]->itemInfo[0].wExist == 0 || 
			m_vResItem[i]->itemInfo[0].bNotUsed)
		{
			continue;
		}		
		m_dwTextRepSize+= GetLangTextSize(m_pResBuf,(int)m_vResItem[i]->uResIndex);
	}
}

void  CResParser::CalcModifedName()
{
	USES_CONVERSION;

	int nCount = m_vResItem.size();
	for(int i = 0; i< nCount; i++)
	{
		RES_ITEM_PTR pRI = m_vResItem[i];
		if( pRI->itemInfo[0].wExist == 0 || 
			pRI->itemInfo[0].bNotUsed)
		{
			continue;
		}
		
		if(_tcslen(pRI->szNewDesc) == 0)
			continue;

		DWORD dwOldLen = strlen(T2A(pRI->szDesc))+1;
		DWORD dwNewLen = strlen(T2A(pRI->szNewDesc))+1;
		m_nNewSize += (int)(ALIGN4_SIZE(dwNewLen) - ALIGN4_SIZE(dwOldLen)); 
	}
}

int CResParser::GetLangCount()
{
	BYTE *pMduBaseBuf = NULL;
	int nThemeIndex=0;
	int nMduID = 0;
	if(m_bThemeOnly)
	{
		pMduBaseBuf = m_pResBuf;
	}
	else
	{
		DWORD dwThemeOffset = * (DWORD*)(m_pResBuf+ DEFAULT_RES_INFO_OFFSET + nThemeIndex*8); //lint !e613
		CONV_DWORD_EX(dwThemeOffset);
		
		pMduBaseBuf = m_pResBuf + dwThemeOffset;//lint !e613
	}
	
	DWORD dwiMduOffset = * (DWORD*)(pMduBaseBuf + (20 + nMduID * 4));//lint !e613
	CONV_DWORD_EX(dwiMduOffset);//lint !e613
	pMduBaseBuf += dwiMduOffset;//lint !e613
	
	
	BYTE *pBaseBuf = pMduBaseBuf + MMI_RT_TEXT*12;
	
    DWORD dwOffset = *(DWORD*)(pBaseBuf);
	DWORD dwNum    = *(DWORD*)(pBaseBuf+4);
	DWORD dwSize   = *(DWORD*)(pBaseBuf+8);
	CONV_DWORD_EX(dwOffset);
	CONV_DWORD_EX(dwNum);
	CONV_DWORD_EX(dwSize);
	
	if(dwNum == 0 || dwOffset==0)
	{
		return 0;
	}
	
	int nLangNum = 0;
	
	DWORD dwFirstOffset= *(DWORD*)(pMduBaseBuf+ dwOffset);
	CONV_DWORD_EX(dwFirstOffset);
	
	nLangNum = (dwFirstOffset - dwOffset) / sizeof(KSP_TEXT_T);

	return nLangNum;
}

int CResParser::GetLangNames(CStringArray &agNames)
{
	agNames.RemoveAll();
	int nCount = m_vResItem.size();
	for(int i = 0; i< nCount; i++)
	{
		RES_ITEM_PTR pRI = m_vResItem[i];
		if(pRI->uResType == MMI_RT_MAX_NUM)
		{
			agNames.Add(pRI->szDesc);
		}
	}
	if(agNames.GetSize()==0)
	{
		int nLangNum = GetLangCount();
		for(int j=0;j<nLangNum;j++)
		{
			CString strLangName;
			strLangName.Format(_T("LANG%d"),j);
			agNames.Add(strLangName);
		}
	}

	return agNames.GetSize();
}

void * CResParser::CreateSpreadSheet()
{
	CSpreadSheet * pSpread = NULL;
	
	if(m_nExcelParserType == 0)
	{
		pSpread = new CSpreadSheet();
		if(!pSpread->Init(m_bReadOnce))
		{			
			delete pSpread;
			return NULL;		
		}
	}
	else if(m_nExcelParserType == 1)
	{
		pSpread = new CSpreadSheetET();
		if(!pSpread->Init(m_bReadOnce))
		{			
			delete pSpread;
			return NULL;		
		}
	}
	else if(m_nExcelParserType == 2)
	{
		pSpread = new CSpreadSheetBE();
		if(!pSpread->Init(m_bReadOnce))
		{			
			delete pSpread;
			return NULL;		
		}
	}
	else
	{
		pSpread = new CSpreadSheet();
		if(!pSpread->Init(m_bReadOnce))
		{
			delete pSpread;
			pSpread = new CSpreadSheetET();
			if(!pSpread->Init(m_bReadOnce))
			{
				delete pSpread;
				pSpread = new CSpreadSheetBE();
				if(!pSpread->Init(m_bReadOnce))
				{
					delete pSpread;
					return NULL;
				}
			}
		}
	}
	
	return (void*)pSpread;
}

int CResParser::GetMduInfo(LPBYTE pResBuf,CUIntArray &agMduIDIdx)
{
	int i=0;
	int nThemeIndex = 0;	
	
	BYTE *pBaseBuf = NULL;
	if(m_bThemeOnly)
	{
		pBaseBuf = pResBuf;
	}
	else
	{
		DWORD dwThemeOffset = * (DWORD*)(pResBuf+ DEFAULT_RES_INFO_OFFSET + nThemeIndex*8); //lint !e613
		CONV_DWORD_EX(dwThemeOffset);
		
		pBaseBuf = pResBuf + dwThemeOffset;//lint !e613
	}
	
	DWORD * pMduOffset = (DWORD*)(pBaseBuf + 20);
	DWORD dwFirstMduOffset = *pMduOffset;//lint !e613
	CONV_DWORD_EX(dwFirstMduOffset);//lint !e613
	
	int nMaxMduNum = (dwFirstMduOffset - 20 - sizeof(MMI_MODULE_VER_INFO_T))/4;
	int nUsedMduNum = 0;
	agMduIDIdx.RemoveAll();
	
	for(i=0 ;i< nMaxMduNum; i++)
	{
		if(pMduOffset[i]!=0)
		{
			nUsedMduNum++;
			agMduIDIdx.Add(i);
		}
	}

	return agMduIDIdx.GetSize();

}

void  CResParser::ClearStrTable()
{
	POSITION pos = NULL;
    CString strKey;
	PSTRARRAY pagstr = NULL;
	
    for( pos = m_mapString.GetStartPosition(); pos != NULL; )
    {	
		m_mapString.GetNextAssoc( pos, strKey, pagstr );
		pagstr->RemoveAll();
		delete pagstr;	
    }
    m_mapString.RemoveAll();
}

BOOL CResParser::ImportString(LPCTSTR lpszFile)
{
	CWaitCursor wait;
	ClearStrTable();
	CStringArray agLangNames;
	GetLangNames(agLangNames);
	if(!LoadStrTable(lpszFile,agLangNames))
		return FALSE;


	CUIntArray agMudIndexs;
	int nUsedMduNum = GetMduInfo(m_pResBuf,agMudIndexs);

	::PostMessage(m_hWnd,WM_PROGRESS,PROG_BEGIN,nUsedMduNum);

	MSG msg;
	for(int i=0 ;i< nUsedMduNum; i++)
	{
		::PostMessage(m_hWnd,WM_PROGRESS,PROG_PROCEED,i+1);
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		wait.Restore();

		CStrSet_Ptr pStrSet = m_vStrSet[i];	
		pStrSet->Clear(FALSE);
		pStrSet->m_bAllUnicode = m_bAllUnicode;

		ImpMduString(agMudIndexs[i],pStrSet);
	}

	::PostMessage(m_hWnd,WM_PROGRESS,PROG_END,100);

	wait.Restore();

	return TRUE;
}

BOOL CResParser::ExportString(LPCTSTR lpszFile)
{	
	CWaitCursor wait;
	if(m_pResBuf == NULL)
		return TRUE;

	CSpreadSheet *pSpread = (CSpreadSheet *)CreateSpreadSheet();
	if(pSpread==NULL)
		return FALSE;
	
	pSpread->OpenExcel(lpszFile,FALSE);
	pSpread->CreateSheet(_T("DEFAULT"));

	CStringArray agRow;
	agRow.Add(_T("ID"));

	CStringArray agLangNames;
	int nLangNum = GetLangNames(agLangNames);
	int i=0;
	for(i=0;i<nLangNum;i++)
	{
		agRow.Add(agLangNames.GetAt(i));
	}
	pSpread->WriteRow(agRow);
	
	CUIntArray agMudIndexs;
	int nUsedMduNum = GetMduInfo(m_pResBuf,agMudIndexs);

	int nCount = m_vStrSet.size();
	BOOL bExp = FALSE;
	if(nCount == nUsedMduNum)
	{
		bExp = TRUE;
	}
	
	for(i=0 ;i< nUsedMduNum; i++)
	{		
		CStrSet_Ptr pStrSet = NULL;
		if(!bExp)
		{
			pStrSet = new CStrSet();
			pStrSet->m_prcConverter = m_pRC;
			pStrSet->m_bAllUnicode = m_bAllUnicode;
			pStrSet->m_bStrNoFlag = m_bStrNoFlag;
			pStrSet->m_nLangNum = nLangNum;
			m_vStrSet.push_back(pStrSet);
		}
		else
		{
			pStrSet = m_vStrSet[i];
			pStrSet->Clear(TRUE);
		}		
		
		ExpMduString(pSpread,m_pResBuf,agMudIndexs[i],pStrSet);
		wait.Restore();
	}
	
	pSpread->Close();

	if(pSpread != NULL)
	{
		delete pSpread;
		pSpread = NULL;
	}
	
	return TRUE;
}

BOOL  CResParser::ExpMduString(CSpreadSheet *pSpread,LPBYTE pResBuf,int nMduID,CStrSet_Ptr pStrSet)
{
	CWaitCursor wait;

	BYTE *pMduBaseBuf = NULL;
	int nThemeIndex=0;
	if(m_bThemeOnly)
	{
		pMduBaseBuf = pResBuf;
	}
	else
	{
		DWORD dwThemeOffset = * (DWORD*)(pResBuf+ DEFAULT_RES_INFO_OFFSET + nThemeIndex*8); //lint !e613
		CONV_DWORD_EX(dwThemeOffset);
		
		pMduBaseBuf = pResBuf + dwThemeOffset;//lint !e613
	}

	DWORD dwiMduOffset = * (DWORD*)(pMduBaseBuf + (20 + nMduID * 4));//lint !e613
	CONV_DWORD_EX(dwiMduOffset);//lint !e613
	pMduBaseBuf += dwiMduOffset;//lint !e613

	
	BYTE *pBaseBuf = pMduBaseBuf + MMI_RT_TEXT*12;
	
    DWORD dwOffset = *(DWORD*)(pBaseBuf);
	DWORD dwNum    = *(DWORD*)(pBaseBuf+4);
	DWORD dwSize   = *(DWORD*)(pBaseBuf+8);
	CONV_DWORD_EX(dwOffset);
	CONV_DWORD_EX(dwNum);
	CONV_DWORD_EX(dwSize);
	
	if(dwNum == 0 || dwOffset==0)
	{
		return 0;
	}
	
	int nLangNum = 0;
	
	DWORD dwFirstOffset= *(DWORD*)(pMduBaseBuf+ dwOffset);
	CONV_DWORD_EX(dwFirstOffset);
	
	nLangNum = (dwFirstOffset - dwOffset) / sizeof(KSP_TEXT_T);
	_ASSERT(nLangNum != 0);	

#define MAX_BUF_SIZE 1024*1024
	LPBYTE pBuf = new BYTE[MAX_BUF_SIZE];

	DWORD dwEnStart = 0;
	DWORD dwEnEnd   = 0;

	//////////////////////////////////////////////////////////////////////////
	// 确定英文区域字符串的offset范围
	// 对于跨语言重复的字符串，前几个offset可能指向英文区域
	DWORD dwEnIndexOffset = *(DWORD*)(pMduBaseBuf+ dwOffset);
	CONV_DWORD_EX(dwEnIndexOffset);
	DWORD *pEnDataOffset = (DWORD*)(pMduBaseBuf+ dwEnIndexOffset);
	
	dwEnStart = *pEnDataOffset;	
	CONV_DWORD_EX(dwEnStart);
	
	dwEnEnd = dwEnStart;
	for(UINT j= 1; j<dwNum; j++)
	{
		DWORD dwTmp = *(pEnDataOffset+j);
		CONV_DWORD_EX(dwTmp);		
		if(dwTmp > dwEnEnd )
		{
			dwEnEnd = dwTmp;	
		}	
	}

	::PostMessage(m_hWnd,WM_PROGRESS,PROG_BEGIN,dwNum);

	MSG msg;
	WORD wTmp =  0;
	for(int j=0;j<dwNum; j++)
	{		
		::PostMessage(m_hWnd,WM_PROGRESS,PROG_PROCEED,j+1);
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		wait.Restore();
		
		CStringArray agRow;
		CString strID;
		strID.Format(_T("%03d-%08d"),nMduID,j);
		agRow.Add(strID);
		for(int i= 0; i< nLangNum; i++)
		{
			PKSP_TEXT_T pt = (KSP_TEXT_T*)(pMduBaseBuf+ (dwOffset + (UINT)i*sizeof(KSP_TEXT_T)));
			DWORD dwLangOffset = (DWORD)pt->pTxtTable;
			WORD  wLangSize = (WORD)pt->nbText;
			CONV_DWORD_EX(dwLangOffset);
			CONV_SHORT_EX(wLangSize);
			
			DWORD dwDataOffset = *(DWORD*)(pMduBaseBuf+ (dwLangOffset + j*4));
			CONV_DWORD_EX(dwDataOffset);

			if(i>0 && dwDataOffset >= dwEnStart && dwDataOffset <= dwEnEnd)
			{
				m_bRmCrossRepeat = TRUE;
			}

			LPBYTE pData = (pMduBaseBuf+ dwDataOffset);

			memset(pBuf,0,MAX_BUF_SIZE);

			if( m_bStrNoFlag || *pData == (BYTE)0x80 )
			{
				WORD wLen = 0;
				if(m_bStrNoFlag)
				{
					wLen = *(WORD*)(pData);
				}
				else
				{
					wLen = *(WORD*)(pData+2);
				}
				
				CONV_SHORT_EX(wLen);
				if(wLen==0)
				{
					agRow.Add(_T(""));
				}
				else
				{
					WORD *pCur = NULL;
					if(m_bStrNoFlag)
					{
						pCur = (WORD *)(pData+2);
					}
					else
					{
						pCur = (WORD *)(pData+4);
					}
					
					for(int k=0; k<wLen/2;k++)
					{
						wTmp = *(pCur+k);
						CONV_SHORT_EX(wTmp);
						*(WORD*)(pBuf+k*2) =  wTmp;
					}
					agRow.Add((_TCHAR*)pBuf);					
				}
			}
			else
			{
				m_bAllUnicode = FALSE;
				LPBYTE pACur = pBuf;
				while(*pData != '\0')
				{
					*pACur = *pData;
					pACur++;
					*pACur = 0;
					pACur++;
					pData++;
				}

				agRow.Add((_TCHAR*)pBuf);	
			}
		}
		pSpread->WriteRow(agRow);
		agRow.RemoveAt(0);
		pStrSet->AddString(strID,agRow);
	}

	::PostMessage(m_hWnd,WM_PROGRESS,PROG_END,100);

	wait.Restore();

	SAFE_DEL(pBuf);

	return TRUE;
}

BOOL CResParser::LoadStrTable(LPCTSTR lpszFile,CStringArray &agLangNames)
{	
	if(lpszFile== NULL)
	{
		return FALSE;
	}	
	
	CWaitCursor wait;

	BOOL bOK = TRUE;
	
    CSpreadSheet * pSpread = NULL;	
	pSpread = (CSpreadSheet *)CreateSpreadSheet();
	if(pSpread== NULL)
	{
		return FALSE;
	}	
	
	if(!pSpread->OpenExcel(lpszFile))
	{
		_tprintf(_T("\n[%s] open failed.\n"),lpszFile);
		pSpread->Close();
		delete pSpread;
		return FALSE;
	}
	
	
	int nLangNum = agLangNames.GetSize();	
	CString strSheetName=_T("DEFAULT");	
	
	if(!pSpread->OpenSheet(strSheetName))
	{
		pSpread->Close();
		delete pSpread;
		return FALSE;
	}
	
	int nCols = 0;
	int nRows = 0;
	
	pSpread->GetBounds(nCols,nRows);
	
	//check row count
	if(nRows <= 1)
	{
		pSpread->Close();
		delete pSpread;
		return FALSE;
	}
	
	CStringArray aRow;
	CString strID = _T("");
	CString strCell = _T("");
	int i,j;
	
	///检测列数
	if(nCols <= 1)
	{
		pSpread->Close();
		delete pSpread;
		return FALSE;
	}	
	
	CUIntArray agLangColIndex;
	
	int nCurRow = 0;
	for( i = 0; i< nRows; i++)
	{
		if(pSpread->ReadRow(i,aRow))
		{
			nCurRow = i+1;
			break;
		}			
	}
	if( i >= nRows)
	{
		return FALSE;
	}
	
	for( i = 0; i< aRow.GetSize(); i++)
	{
		strCell = aRow[i];
		strCell.TrimLeft();
		strCell.TrimRight();
		if(!strCell.IsEmpty())
			break;
	}
	if(i>=aRow.GetSize())
	{
		pSpread->Close();
		delete pSpread;
		return FALSE; // empty sheet
	}
	
	//检测列名	
	for(i = 0; i < nCols; i++)
	{
		strCell= aRow.GetAt(i);
		
		if(i== 0)
		{
			if(strCell.CompareNoCase(_T("ID"))!=0)
			{
				_tprintf(_T("\n[Error] [%s] col 0 '%s' is not 'ID' in the header of sheet [%s].\n"),lpszFile,strCell,strSheetName);
				bOK = FALSE;
			}
		}
		else
		{
			for(j=0;j<(int)agLangNames.GetSize();j++)
			{
				if(strCell.CompareNoCase(agLangNames[j])==0)	
				{
					agLangColIndex.Add(i);
					break;
				}
			}
			if(agLangColIndex.GetSize() == nLangNum )
			{
				break;
			}
		}		
	}
	
	if(!bOK || agLangColIndex.GetSize() != nLangNum )
	{
		pSpread->Close();
		delete pSpread;
		
		return FALSE;
	}
	//检测结束	
	agLangColIndex.RemoveAll();
	for(j= 0; j< nLangNum; j++)
	{
		//for(i = 2; i < nCols; i++)
		for(i = 1; i < nCols; i++)
		{
			strCell= aRow.GetAt(i);
			if(strCell.CompareNoCase(agLangNames[j])==0)	
			{
				agLangColIndex.Add(i);
				break;
			}
		}
	}
	
	PSTRING_MAP pStrMap = &m_mapString;		
	
	CStringArray arrLine;
	CString strWarning;
	
	CStringArray aText;
	
	STRID_MAPS  mapStrID;	
	
	wait.Restore();
	::PostMessage(m_hWnd,WM_PROGRESS,PROG_BEGIN,nRows);
	MSG msg;
	for(i = nCurRow; i< nRows;i++)
	{
		::PostMessage(m_hWnd,WM_PROGRESS,PROG_PROCEED,i+1);
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		wait.Restore();

		aText.RemoveAll();
		
		if(!pSpread->ReadRow(i,aRow))
		{
			continue;
		}
		
		//ID
		strID = aRow.GetAt(0);
		if(strID.IsEmpty())
			continue;
		
		CString strTmp;
		if(mapStrID.Lookup(strID,strTmp))
		{				
			_tprintf(_T("\n[Error][%s] has the same ID (%s) in the sheet [%s].\n"),lpszFile,strID,strSheetName);
			bOK = FALSE;
			continue;	
		}
		else
		{
			mapStrID.SetAt(strID,strID);
		}
		
		PSTRARRAY pagstr = NULL;
		if(pStrMap->Lookup(strID,pagstr))
		{
			continue;
		}			
		
		for(j=0;j<agLangColIndex.GetSize();j++)
		{
			int nIdx= agLangColIndex.GetAt(j);	
			aText.Add(aRow[nIdx]);			
		}	
		
		pagstr = new CStringArray();
		pagstr->Copy(aText);
		pStrMap->SetAt(strID,pagstr);
	}

	::PostMessage(m_hWnd,WM_PROGRESS,PROG_END,100);
	
	wait.Restore();

	mapStrID.RemoveAll();
	
	
	pSpread->Close();
	delete pSpread; 
	
	return bOK;
}

BOOL  CResParser::ImpMduString(int nMduID,CStrSet_Ptr pStrSet)
{
	UNUSED_ALWAYS(nMduID);

	CWaitCursor wait;

	CStringArray &agID = pStrSet->m_agOrgStrID;

	int nNum = agID.GetSize();
	PSTRARRAY pagRow = NULL;

	for(int i = 0; i< nNum; i++)
	{
		CString strID = agID[i];
		
		if(m_mapString.Lookup(strID,pagRow))
		{
			pStrSet->RepString(strID,pagRow);
		}
		else
		{
			pStrSet->RepString(strID,NULL);
		}	
	}

	if(m_bRmCrossRepeat)
	{
		pStrSet->CalcTextCrossRepeat(0,FALSE);
	}

	return TRUE;
}

BOOL CResParser::SaveString(int nThemeIndex)
{
	UNUSED_ALWAYS(nThemeIndex);

	if(!IsStringModified())
		return TRUE;

	CUIntArray agMudIndexs;
	int nUsedMduNum = GetMduInfo(m_pResBuf,agMudIndexs);
	
	for(int i=0 ;i< nUsedMduNum; i++)
	{		
		CStrSet_Ptr pStrSet = m_vStrSet[i];	
		SaveMduString(agMudIndexs[i],pStrSet);
	}

	return TRUE;
}

BOOL CResParser::SaveMduString(int nMduID,CStrSet_Ptr pStrSet)
{
	if(m_pNewBuf == NULL)
	{
		return FALSE;
	}

	LPBYTE pMduBase = GetMduBase(m_pNewBuf,nMduID,0);
	// 记录没有改变以前的Text数据
	DWORD dwTextOffset = * (DWORD*)(pMduBase+MMI_RT_TEXT*12);
	DWORD dwTextNum = * (DWORD*)(pMduBase+MMI_RT_TEXT*12+4);
	DWORD dwTextSize = * (DWORD*)(pMduBase+MMI_RT_TEXT*12+8);
	CONV_DWORD_EX(dwTextOffset);
	CONV_DWORD_EX(dwTextNum);
	CONV_DWORD_EX(dwTextSize);


	LPBYTE pStrData = NULL;
	DWORD dwDataSize = 0;
	DWORD dwBase = dwTextOffset;
	int nThemeIndex = 0;
	pStrSet->GetStrData(pStrData,dwDataSize,dwBase,m_bBigEndian);

	DWORD dwOrgSize = dwTextSize;
	DWORD dwRepSize = dwDataSize;

	int nDeltAlign =  (int)ALIGN4_SIZE(dwRepSize) - (int)ALIGN4_SIZE(dwOrgSize);

	DWORD dwOffsetTmp =0;
	DWORD dwSizeTmp =0;
	int i = 0;

	RES_ITEM ri;
	ri.uModuleID = nMduID;
	ri.uResType = MMI_RT_TEXT;
	ri.itemInfo[0].org.dwSize = dwOrgSize;
	ri.itemInfo[0].rep.dwSize = dwRepSize;

	BYTE *pBaseBuf = MidifyHdrOffset(nDeltAlign,nThemeIndex,&ri);	
	BYTE *pTextInfoBuf = pBaseBuf+ dwTextOffset;

	// 先修改偏移，然后再移动内存，插入数据
	if(nDeltAlign != 0)
	{
		// 不需要修改IMG 偏移			
		// 不需要修改Anim 偏移		
		// 不需要修改RING 偏移
		// 不需要修改TEXT 偏移	
		// 修改FONT偏移
		ModifyFontOffset(pBaseBuf,nDeltAlign,0);		
		// 修改Data 偏移
		ModifyDataOffset(pBaseBuf,nDeltAlign,0);

		// 修改资源类型偏移	
		for( i = MMI_RT_FONT ; i< MMI_RT_MAX_NUM; i++)
		{
			dwOffsetTmp = *(DWORD*)(pBaseBuf + i*12);
			CONV_DWORD_EX(dwOffsetTmp);
			if(dwOffsetTmp != 0)
			{
				dwOffsetTmp += (DWORD)nDeltAlign;
				CONV_DWORD_EX(dwOffsetTmp);
				*(DWORD*)(pBaseBuf + i*12) = dwOffsetTmp;
			}
		}
		// 修改Text的总size
		dwSizeTmp = dwTextSize;
		if(dwSizeTmp != 0)
		{
			dwSizeTmp += (DWORD)nDeltAlign;
			CONV_DWORD_EX(dwSizeTmp);
			*(DWORD*)(pBaseBuf+8) = dwSizeTmp;
		}
		

		//移动内存
		DWORD dwTotalOffset = (DWORD)(pTextInfoBuf - m_pNewBuf) + ALIGN4_SIZE(dwOrgSize);
		memmove(m_pNewBuf+(dwTotalOffset+(DWORD)nDeltAlign),m_pNewBuf + dwTotalOffset, m_dwCurTotalSize - dwTotalOffset);
		memset(m_pNewBuf+dwTotalOffset-ALIGN4_SIZE(dwOrgSize),0, ALIGN4_SIZE(dwRepSize));
	}


	memcpy(pTextInfoBuf,pStrData,dwRepSize);
	m_dwCurTotalSize += (DWORD)nDeltAlign;
	m_nNewSize += (DWORD)nDeltAlign;

	SAFE_DEL(pStrData);

	return TRUE;
}

BOOL CResParser::TestStrNoFlag(LPBYTE pResBuf)
{
	CWaitCursor wait;

	int nMduID=0;
	BYTE *pMduBaseBuf = NULL;
	int nThemeIndex=0;
	if(m_bThemeOnly)
	{
		pMduBaseBuf = pResBuf;
	}
	else
	{
		DWORD dwThemeOffset = * (DWORD*)(pResBuf+ DEFAULT_RES_INFO_OFFSET + nThemeIndex*8); //lint !e613
		CONV_DWORD_EX(dwThemeOffset);
		
		pMduBaseBuf = pResBuf + dwThemeOffset;//lint !e613
	}

	DWORD dwiMduOffset = * (DWORD*)(pMduBaseBuf + (20 + nMduID * 4));//lint !e613
	CONV_DWORD_EX(dwiMduOffset);//lint !e613
	pMduBaseBuf += dwiMduOffset;//lint !e613

	
	BYTE *pBaseBuf = pMduBaseBuf + MMI_RT_TEXT*12;
	
    DWORD dwOffset = *(DWORD*)(pBaseBuf);
	DWORD dwNum    = *(DWORD*)(pBaseBuf+4);
	DWORD dwSize   = *(DWORD*)(pBaseBuf+8);
	CONV_DWORD_EX(dwOffset);
	CONV_DWORD_EX(dwNum);
	CONV_DWORD_EX(dwSize);
	
	if(dwNum == 0 || dwOffset==0)
	{
		return FALSE;
	}
	
	int nLangNum = 0;
	
	DWORD dwFirstOffset= *(DWORD*)(pMduBaseBuf+ dwOffset);
	CONV_DWORD_EX(dwFirstOffset);
	
	nLangNum = (dwFirstOffset - dwOffset) / sizeof(KSP_TEXT_T);
	_ASSERT(nLangNum != 0);	

	DWORD dwEnStart = 0;
	DWORD dwEnEnd   = 0;

	//////////////////////////////////////////////////////////////////////////
	// 确定英文区域字符串的offset范围
	// 对于跨语言重复的字符串，前几个offset可能指向英文区域
	DWORD dwEnIndexOffset = *(DWORD*)(pMduBaseBuf+ dwOffset);
	CONV_DWORD_EX(dwEnIndexOffset);
	DWORD *pEnDataOffset = (DWORD*)(pMduBaseBuf+ dwEnIndexOffset);
	
	dwEnStart = *pEnDataOffset;	
	CONV_DWORD_EX(dwEnStart);
	
	dwEnEnd = dwEnStart;
	int nTestIndex = 0;
	for(UINT j= 1; j<dwNum; j++)
	{
		DWORD dwTmp = *(pEnDataOffset+j);
		CONV_DWORD_EX(dwTmp);		
		if(dwTmp - dwEnStart > 4 )
		{
			dwEnEnd = dwTmp;
			nTestIndex = j;
			break;
		}	
		else
		{
			dwEnStart = dwTmp;
			dwEnEnd = dwEnStart;
		}
	}

	MSG msg;
	WORD wTmp =  0;
	UINT i = 0;

	PKSP_TEXT_T pt = (KSP_TEXT_T*)(pMduBaseBuf+ (dwOffset + (UINT)i*sizeof(KSP_TEXT_T)));
	DWORD dwLangOffset = (DWORD)pt->pTxtTable;
	WORD  wLangSize = (WORD)pt->nbText;
	CONV_DWORD_EX(dwLangOffset);
	CONV_SHORT_EX(wLangSize);
	
	DWORD dwDataOffset = *(DWORD*)(pMduBaseBuf+ (dwLangOffset + nTestIndex*4));
	CONV_DWORD_EX(dwDataOffset);

//	if(i>0 && dwDataOffset >= dwEnStart && dwDataOffset <= dwEnEnd)
//	{
//		m_bRmCrossRepeat = TRUE;
//	}

	LPBYTE pData = (pMduBaseBuf+ dwDataOffset);

	if(*pData != 0x80 && *pData == 0 )
	{
		m_bStrNoFlag = TRUE;
	}

	return m_bStrNoFlag;
}
