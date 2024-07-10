// BZip.cpp: implementation of the CBZip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BZip.h"
#include <atlconv.h>
#include <algorithm>

extern "C" {
//#include "../zlib/code/zutil.h"
#include "../zlib/code/zlib.h"
}
#include "../lzma/code/LzmaLib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define  SAFE_DEL(p) do{ \
	if(NULL != p) \
	{ \
		delete [] p; \
		p=NULL;\
	} \
}while(0) 



typedef struct SCI_BZP_FILE_HEAD_Tag
{
    DWORD  magic;
	DWORD  version;
	DWORD  block_head_pos;
    DWORD  block_num;
}SCI_BZP_FILE_HEAD_T;

typedef struct SCI_BZP_BLOCK_HEAD_Tag
{
	DWORD  magic;
	DWORD  type; 
    DWORD  start_pos;
	DWORD  block_len;
	DWORD  des_addr;	
}SCI_BZP_BLOCK_HEAD_T;

#define BZP_BLOCK_CODE    0x636F6465   //"code"
#define BZP_BLOCK_RES     0x7253736F   //"rSso"
#define BZP_BLOCK_LANG    0x6C616E67   //"lang"
#define BZP_BLOCK_MAGIC   0x424C4F43   //"BLOC"
#define BZP_BLOCK_MAGIC2  0x424C4F44   //"BLOD"  not compress
#define BZP_BLOCK_DSP     0x42445350   //"BDSP"
#define BZP_BLOCK_USR     0x75736572   //"user"
#define BZP_BLOCK_KERN    0x494D4147   //"IMAG"
#define BZP_HEAD_MAGIC    0x53505244   //"SPRD"
#define BZP_HEAD_MAGIC2   0x53505252   //"SPRR"  not compress
#define BZP_PAC_MAGIC     0x4E504143   //"NPAC"
#define BZP_EXT_MAGIC_PRE 0x45583030   //"EX00"

#define EXEC_CODE_SIZE	  0xA9000
#define CODE_DES_ADDR     0x042C0000
#define RES_DES_ADDR      (CODE_DES_ADDR + EXEC_CODE_SIZE)

#define DEFAULT_RAM_SIZE  0x2000000            //32M Bytes
#define DEFAULT_BZP_SIZE  0x2000000            //32M Bytes

#define MAX_SEARCH_SIZE   (32*1024)

#define  PARTITION_MAGIC_FIRST   0x50415254    //"PART"
#define  PARTITION_MAGIC_SECOND  0x494D4147    //"IMAG"
#define  PARTITION_MAGIC_END     0x5441494C    //"TAIL"

enum E_PS_TYPE
{
	E_UNFINDE		= 0,
	E_5_PATITION	= 1,
	E_29_PATITION	= 2
};

typedef struct UPDATE_DATA_Tag
{
  DWORD   image_addr;
  DWORD   res_addr;
  DWORD   nv_addr;
  DWORD   dsp_addr;
  DWORD   reserved2;
}UPDATE_DATA_T;

typedef struct DATA_INFO_T
{
	DWORD  dwOrgSize;
	DWORD  dwZipSize;
	LPBYTE pZipBuf;
}DATA_INFO;


#define  CONVERT_SHORT(Dst,Src,bBig) do{if(bBig)(Dst) = MAKEWORD(HIBYTE(Src),LOBYTE(Src));else (Dst)=(Src); }while(0)


#define  CONVERT_INT(Dst,Src,bBig)  do{\
								  if(bBig){\
                                        (Dst)  = MAKELONG(MAKEWORD(HIBYTE(HIWORD(Src)),LOBYTE(HIWORD(Src))),\
                                                          MAKEWORD(HIBYTE(LOWORD(Src)),LOBYTE(LOWORD(Src))));\
								  }\
								  else{ (Dst)=(Src); }\
                                }while(0)


void ConvertBZPFileHead(SCI_BZP_FILE_HEAD_T &head,BOOL bBigEndian)
{
	CONVERT_INT(head.magic,head.magic,bBigEndian);
	CONVERT_INT(head.version,head.version,bBigEndian);
	CONVERT_INT(head.block_head_pos,head.block_head_pos,bBigEndian);
	CONVERT_INT(head.block_num,head.block_num,bBigEndian);

}

void ConvertBZPBlockHead(SCI_BZP_BLOCK_HEAD_T &head,BOOL bBigEndian)
{
	CONVERT_INT(head.magic,head.magic,bBigEndian);
	CONVERT_INT(head.type,head.type,bBigEndian);
	CONVERT_INT(head.start_pos,head.start_pos,bBigEndian);
	CONVERT_INT(head.block_len,head.block_len,bBigEndian);
	CONVERT_INT(head.des_addr,head.des_addr,bBigEndian);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBZip::CBZip()
{
	InitParam();
	m_pBzpBuf = NULL;
	m_dwBzpSize = 0;
	memset(m_szLastErrMsg,0,sizeof(m_szLastErrMsg));
	m_bExtPtnTbl = FALSE;

}

CBZip::~CBZip()
{
	if(m_pBzpBuf != NULL)
	{
		delete [] m_pBzpBuf;
		m_pBzpBuf = NULL;
	}
}
void CBZip::Reset()
{
	if(m_pBzpBuf != NULL)
	{
		delete [] m_pBzpBuf;
		m_pBzpBuf = NULL;
	}
	m_dwBzpSize = 0;

	InitParam();

	memset(m_szLastErrMsg,0,sizeof(m_szLastErrMsg));
	m_bExtPtnTbl = FALSE;
}

void  CBZip::CheckCmpLevel(int &nCmpType, int &nLevel)
{
	if(nCmpType == BZP_ZIP)
	{
		if(nLevel != -1)
		{
			if(nLevel < 0 || nLevel > 9)
			{
				nLevel = 9;
			}
		}
		else
		{
			nLevel = 9;
		}	
	}
	else if(nCmpType == BZP_7Z || nCmpType == BZP_7ZF )
	{
		if(nLevel != -1)
		{
			if(nLevel < 0 || nLevel > 9)
			{
				nLevel = 9;
			}
		}
		else
		{
			nLevel = 2;
		}
	}
}

void  CBZip::SetLimitParam(BZP_PARAM_T &param)
{
	TCHAR szPath[MAX_PATH] = {0};
	::GetModuleFileName(NULL,szPath,MAX_PATH);
	TCHAR *pFind = _tcsrchr(szPath,_T('\\'));
	*pFind = '\0';
	_tcscat(szPath,_T("\\bzp.ini"));
	if(param.dwMaxRam!=0)
	{
		m_tParam.dwMaxRam = param.dwMaxRam;
	}
	else
	{
		m_tParam.dwMaxRam = GetPrivateProfileInt(_T("Settings"),_T("MaxRam"),DEFAULT_RAM_SIZE,szPath);
	}
	if(param.dwMaxBzp!=0)
	{
		m_tParam.dwMaxBzp = param.dwMaxBzp;
	}
	else
	{
		m_tParam.dwMaxBzp = GetPrivateProfileInt(_T("Settings"),_T("MaxBzp"),DEFAULT_RAM_SIZE,szPath);
	}

 	m_tParam.nCmpType = param.nCmpType;
	m_tParam.nUsrCmpType = param.nUsrCmpType;
	m_tParam.nResCmpType = param.nResCmpType;

	if(m_tParam.nUsrCmpType == -1)
	{
		m_tParam.nUsrCmpType = m_tParam.nCmpType;
	}	
	if(m_tParam.nResCmpType == -1)
	{
		m_tParam.nResCmpType = m_tParam.nCmpType;
	}

	m_tParam.nLevel = param.nLevel;
	CheckCmpLevel(m_tParam.nCmpType,m_tParam.nLevel);

	m_tParam.nUsrLevel = param.nUsrLevel;
	m_tParam.nResLevel = param.nResLevel;	
	if(m_tParam.nUsrLevel == -1)
	{
		m_tParam.nUsrLevel = m_tParam.nLevel;
	}
	if(m_tParam.nResLevel == -1)
	{
		m_tParam.nResLevel = m_tParam.nLevel;
	}
	CheckCmpLevel(m_tParam.nUsrCmpType,m_tParam.nUsrLevel);
	CheckCmpLevel(m_tParam.nResCmpType,m_tParam.nResLevel);

	m_tParam.b2ndBzp  = param.b2ndBzp;
	m_tParam.bCat = param.bCat;
	m_tParam.bCat2 = param.bCat2;
	m_tParam.bBigEndian = param.bBigEndian;
	m_tParam.bRawCmpData = param.bRawCmpData;

	m_tParam.dwUsrPacSize = param.dwUsrPacSize;
	m_tParam.dwResPacSize = param.dwResPacSize;
	
}

BOOL CBZip::GenBzpEx(CStringArray &agFile,CStringArray &agFileEx,LPCTSTR pszBzpFile)
{
	BOOL bRet = FALSE;
	
		
	if (0 == agFileEx.GetSize())
	{
		bRet = GenBzp(agFile,pszBzpFile);
	}
	else
	{
		_TCHAR szTmpBzpFile[MAX_PATH*2] = {0};
		_TCHAR szTmpExtBzpFile[MAX_PATH*2] = {0};
		_TCHAR szName[MAX_PATH] = {0};
		CFile  cfile;
		DWORD  dwTmpBzpSize = 0;
		
		GetTempPath(MAX_PATH*2,szTmpBzpFile);
		_tcscpy(szTmpExtBzpFile,szTmpBzpFile);

		_stprintf(szName,_T("tmpBzp%d.bin"),GetTickCount());
		_tcscat(szTmpBzpFile,szName);

		_stprintf(szName,_T("tmpExtBzp%d.bin"),GetTickCount());
		_tcscat(szTmpExtBzpFile,szName);
		do 
		{
			if (!(bRet = GenBzp(agFile,szTmpBzpFile)))	break;
			if (cfile.Open(szTmpBzpFile,CFile::modeRead))
			{
				dwTmpBzpSize = cfile.GetLength();
				cfile.Close();
			}
			if (0 == dwTmpBzpSize)
			{
				bRet = FALSE;
				break;
			}

			if (!(bRet = GenExtBzp(agFileEx,dwTmpBzpSize,szTmpExtBzpFile)))	break;
			bRet = MergeEx(szTmpBzpFile,szTmpExtBzpFile,pszBzpFile);
			
		} while(0);
		
	}

	return bRet;
}
BOOL CBZip::ResOverGenBzp(CStringArray &agFile,LPCTSTR pszBzpFile)
{
	BOOL bRet = FALSE;
	int nFileCount = agFile.GetSize()/2;
	CStringArray agGenFile;
	CString strSplitedFile;
	DWORD	dwOrgGenLen = 0;

	_TCHAR szTmpBzpFile[MAX_PATH*2] = {0};
	_TCHAR szName[MAX_PATH] = {0};
	DWORD  dwTmpBzpSize = 0;
	
	GetTempPath(MAX_PATH*2,szTmpBzpFile);	
	_stprintf(szName,_T("tmpBzp%d.bin"),GetTickCount());
	_tcscat(szTmpBzpFile,szName);


	for(int i = 0;i< nFileCount; i++)
	{
		if(agFile[2*i].CompareNoCase(_T("-splited")) == 0)
		{
			strSplitedFile = agFile[2*i+1];
		}
		else if(agFile[2*i].CompareNoCase(_T("-orglen")) == 0)
		{	
			dwOrgGenLen = _ttoi(agFile[2*i+1]);
		}
		else 
		{
			agGenFile.Add(agFile[2*i]);
			agGenFile.Add(agFile[2*i+1]);
		}
	}

	do 
	{
		
		if (strSplitedFile.IsEmpty())
		{
			bRet = GenBzp(agGenFile,pszBzpFile);
		}
		else
		{
			if (!(bRet = GenBzp(agGenFile,szTmpBzpFile)))	break;
			bRet = ResOverMerge(szTmpBzpFile,strSplitedFile.operator LPCTSTR(),dwOrgGenLen,pszBzpFile);
		}
		
	}while (0);


	return bRet;
}
BOOL CBZip::GenBzp(CStringArray &agFile,LPCTSTR pszBzpFile)
{
	if(m_tParam.bCat)
	{
		if(!m_tParam.b2ndBzp)
		{
			return Merge(agFile,pszBzpFile);
		}
		else
		{
			CStringArray agFile1;
			CStringArray agFile2;
			CStringArray agDspFile;
			_TCHAR szBzp[MAX_PATH*2] = {0};
			GetTempPath(MAX_PATH*2,szBzp);
			_TCHAR szName[MAX_PATH] = {0};
			BOOL bResultBzp = TRUE;
			for(int i = 0; i< (agFile.GetSize()/2); i++)
			{
				_TCHAR szPath[MAX_PATH] = {0};
				if(agFile[2*i].CompareNoCase(_T("ps")) == 0)
				{
					agFile1.Add(_T("ps"));
					_tcscpy(szPath,agFile[2*i+1]);
					agFile1.Add(szPath);
				}
				else if(agFile[2*i].CompareNoCase(_T("dsp")) == 0)
				{
					agFile1.Add(_T("dsp"));
					_tcscpy(szPath,agFile[2*i+1]);
					agFile1.Add(szPath);
				}
				else if(agFile[2*i].CompareNoCase(_T("dspz")) == 0)//@polojiang added on 20111214
				{
					agFile1.Add(_T("dspz"));
					agFile1.Add(agFile[2*i+1]);

				}//@end polojiang added on 20111214
				else if(agFile[2*i].CompareNoCase(_T("kern")) == 0)
				{
					agFile1.Add(_T("kern"));
					_tcscpy(szPath,agFile[2*i+1]);
					agFile1.Add(szPath);
				}
				else if(agFile[2*i].CompareNoCase(_T("kernz")) == 0)
				{
					agFile1.Add(_T("kernz"));
					_tcscpy(szPath,agFile[2*i+1]);
					agFile1.Add(szPath);
				}
				else if(agFile[2*i].CompareNoCase(_T("bzp")) != 0)
				{
					_tcscpy(szPath,agFile[2*i]);
					agFile2.Add(szPath);
					_tcscpy(szPath,agFile[2*i+1]);
					agFile2.Add(szPath);
				}
			}

			if (agFile2.GetSize())
			{
				_stprintf(szName,_T("bzp%d.bin"),GetTickCount());
				_tcscat(szBzp,szName);
				agFile1.Add(_T("bzp"));
				agFile1.Add(szBzp);
				if(m_tParam.bCat2)
				{
					bResultBzp = Merge2(agFile2,szBzp);
				}
				else
				{
					bResultBzp = Bzp(agFile2,szBzp);
				}
				
			}

			if( bResultBzp)
			{
				return Merge(agFile1,pszBzpFile);
			}
			else
			{
				return FALSE;
			}
		}
	}
	else if(m_tParam.bCat2)
	{
		return Merge2(agFile,pszBzpFile);
	}
	else if(m_tParam.bRawCmpData)
	{
		return Cmp(agFile,pszBzpFile);
	}
	else
	{
		return Bzp(agFile,pszBzpFile);
	}
}

BOOL CBZip::UnBzp(LPCTSTR pszBzpFile,LPCTSTR pszDstDir,CStringArray **ppagFile)
{
	if(ppagFile == NULL)
	{
		_tprintf(_T("UnBzp: param [ppagFile] is NULL."));
		_stprintf(m_szLastErrMsg,_T("UnBzp: param [ppagFile] is NULL."));
		return FALSE;
	}
	*ppagFile = new CStringArray();

	if(!GetOrgData(pszBzpFile,m_pBzpBuf,m_dwBzpSize))
	{
		return FALSE;
	}

	if(m_dwBzpSize < sizeof(SCI_BZP_FILE_HEAD_T))
	{
		_tprintf(_T("file [%s] is too small."),pszBzpFile);
		_stprintf(m_szLastErrMsg,_T("file [%s] is too small."),pszBzpFile);

		delete []  m_pBzpBuf;
		m_pBzpBuf = NULL;
		return FALSE;
	}

	InitParam();

	SCI_BZP_FILE_HEAD_T bfh;
	memcpy(&bfh,m_pBzpBuf,sizeof(bfh));
	ConvertBZPFileHead(bfh,TRUE);

	DWORD dwMagic = bfh.magic;
	if(dwMagic == BZP_HEAD_MAGIC || dwMagic == BZP_HEAD_MAGIC2)
	{
		m_tParam.bBigEndian = TRUE;
	}
	else
	{
		CONVERT_INT(dwMagic,dwMagic,TRUE);
		if(dwMagic == BZP_HEAD_MAGIC || dwMagic == BZP_HEAD_MAGIC2)
		{
			m_tParam.bBigEndian = FALSE;
		}
	}

	BOOL bOK = FALSE;
	if(dwMagic == BZP_HEAD_MAGIC || dwMagic == BZP_HEAD_MAGIC2 )
	{
		bOK =  BzpUncompress(pszDstDir,**ppagFile,TRUE);
		if(bOK)
		{
			m_tParam.bCat = FALSE;
		}
	}
	else
	{
		bOK = MergeUncompress(pszDstDir,**ppagFile);
		if(bOK)
		{
			m_tParam.bCat = TRUE;

			TCHAR szPath[MAX_PATH] = {0};
			::GetModuleFileName(NULL,szPath,MAX_PATH);
			TCHAR *pFind = _tcsrchr(szPath,_T('\\'));
			*pFind = '\0';
			_tcscat(szPath,_T("\\bzp.ini"));
			m_tParam.dwMaxRam = GetPrivateProfileInt(_T("Settings"),_T("MaxRam"),DEFAULT_RAM_SIZE,szPath);
			m_tParam.dwMaxBzp = GetPrivateProfileInt(_T("Settings"),_T("MaxBzp"),DEFAULT_RAM_SIZE,szPath);
		}		
	}

	SAFE_DEL(m_pBzpBuf);
	m_dwBzpSize = 0;

	return bOK;
}

void CBZip::Release()
{
	delete this;
}

void CBZip::ReleaseMem(CStringArray *pagFile)
{
	pagFile->RemoveAll();
	delete pagFile;
}

LPCTSTR CBZip::GetLastErrMsg()
{
	return m_szLastErrMsg;
}
BOOL CBZip::Cmp(CStringArray &agFile,LPCTSTR pszBzpFile)
{
	if(agFile.GetSize()/2 == 0 || pszBzpFile == NULL)
	{
		_tprintf(_T("Bzp[-raw]: invalid param, file num is zero."));
		_stprintf(m_szLastErrMsg,_T("Bzp[-raw]: invalid param, file num is zero."));
		return FALSE;
	}

	CString strFile;
	int nFileCount = agFile.GetSize()/2;
	int nFileNum=0;


	for(int i = 0;i< nFileCount; i++)
	{
		if(agFile[2*i].CompareNoCase(_T("file")) == 0)
		{
			strFile = agFile[2*i+1];
			nFileNum++;
		}
	}

	if(nFileNum == 0 || strFile.IsEmpty() )
	{
		_tprintf(_T("Bzp[-raw]: file name is empty."));
		_stprintf(m_szLastErrMsg,_T("Bzp[-raw]: file name is empty."));
		return FALSE;
	}


	BYTE *pBufFileZip  = NULL;
	DWORD dwFileOrgSize = 0;
	DWORD dwFileZipSize = 0;	

	FILE *pFile = NULL;
	CFileFind finder;
	DWORD dwWrite = 0;
	DWORD dwLangID = MAXDWORD;
	//////////////////////////////////////////////////////////////////////////
	// deal with code file
	if(!strFile.IsEmpty())
	{
		if(!GetCmpData(strFile.operator LPCTSTR(),dwFileOrgSize,pBufFileZip,dwFileZipSize,dwLangID))
		{
			goto CMP_ERROR;
		}
	}

	if(finder.FindFile(pszBzpFile))
	{
		DWORD dwAtt = ::GetFileAttributes(pszBzpFile);
		dwAtt &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(pszBzpFile,dwAtt);
	}
	
	pFile = _tfopen(pszBzpFile,_T("wb"));
	
	if(pFile == NULL)
	{
		_tprintf(_T("create out file [%s] failed."),pszBzpFile);
		_stprintf(m_szLastErrMsg,_T("create out file [%s] failed."),pszBzpFile);
		goto CMP_ERROR;
	}

#define CMP_WRITE_FILE(p,t) do{\
	dwWrite = fwrite(p,1,(t),pFile);\
	if(dwWrite != (t))\
	{\
		_tprintf(_T("write file [%s] failed."),pszBzpFile);\
		_stprintf(m_szLastErrMsg,_T("write file [%s] failed."),pszBzpFile);\
		goto CMP_ERROR;\
	}\
	}while(0)\


	if(pBufFileZip != NULL)
	{		
		CMP_WRITE_FILE(pBufFileZip,dwFileZipSize);
	}

	fclose(pFile);
	pFile = NULL;
	
	SAFE_DEL(pBufFileZip);
	
	return TRUE;
	
CMP_ERROR:
	if(pFile != NULL)
	{
		fclose(pFile);
	}
	SAFE_DEL(pBufFileZip);
	return FALSE;	
}

BOOL CBZip::Bzp(CStringArray &agFile,LPCTSTR pszBzpFile,BOOL bExtZ/* = TRUE*/)
{
	if(agFile.GetSize()/2 == 0 || pszBzpFile == NULL)
	{
		_tprintf(_T("Bzp: invalid param, file num is zero."));
		_stprintf(m_szLastErrMsg,_T("Bzp: invalid param, file num is zero."));
		return FALSE;
	}

	CString strCodeFile;
	CString strResFile;
	CString strDspFile;//@polojiang added on 20111214
	CString strKernFile;
	CString strExtFile;//@polojiang added on 20121127
	CStringArray agLangFile;
	int nFileCount = agFile.GetSize()/2;
	int nFileNum=0;
	int nLangNum=0;
	int i=0;
	int nIndex = -1;

	for(i = 0;i< nFileCount; i++)
	{
		if(agFile[2*i].CompareNoCase(_T("res")) == 0)
		{
			strResFile = agFile[2*i+1];
			nFileNum++;
		}
		else if(agFile[2*i].CompareNoCase(_T("code")) == 0)
		{
			strCodeFile = agFile[2*i+1];
			nFileNum++;
		}
		else if(agFile[2*i].CompareNoCase(_T("dspz")) == 0)////@polojiang added on 20111214
		{
			strDspFile = agFile[2*i+1];
			nFileNum++;
		}//@End polojiang added on 20111214
		//else if(agFile[2*i].CompareNoCase(EXTZ_KEY) == 0)////@polojiang added on 20121127
		if( -1 != agFile[2*i].Find(EXTZ_KEY) )////@polojiang added on 20121127
		{
			nIndex = _ttoi(agFile[2*i].Mid(5));
			strExtFile = agFile[2*i+1];
			nFileNum++;
		}//@End polojiang added on 20121127
		else if(agFile[2*i].CompareNoCase(_T("kernz")) == 0)
		{
			strKernFile = agFile[2*i+1];
			nFileNum++;
		}
		else if(agFile[2*i].CompareNoCase(_T("lang")) == 0)
		{
			if(!agFile[2*i+1].IsEmpty())
			{
				agLangFile.Add(agFile[2*i+1]);
				nFileNum ++;
			}
		}
	}

	nLangNum = agLangFile.GetSize();

	if ( 
		strExtFile.IsEmpty() && 
		(nFileNum == 0 || ((strCodeFile.IsEmpty() && strResFile.IsEmpty() && nLangNum ==0) && strDspFile.IsEmpty() && strKernFile.IsEmpty()) )
		)
	{
		_tprintf(_T("Bzp: file name is empty.\n"));
		_stprintf(m_szLastErrMsg,_T("Bzp: file name is empty."));
		return FALSE;
	}

	SCI_BZP_FILE_HEAD_T fileHead		= {0};
	SCI_BZP_BLOCK_HEAD_T blockHeadRes	= {0};
	SCI_BZP_BLOCK_HEAD_T blockHeadCode	= {0};
	SCI_BZP_BLOCK_HEAD_T blockHeadDsp	= {0};
	SCI_BZP_BLOCK_HEAD_T blockHeadExt	= {0};//@polojiang added on 20121127
	SCI_BZP_BLOCK_HEAD_T blockHeadKern	= {0};
	SCI_BZP_BLOCK_HEAD_T *pblockLang    = NULL;
	DATA_INFO            *pdataLang     = NULL;
	if(nLangNum != 0)
	{
		pblockLang = new SCI_BZP_BLOCK_HEAD_T[nLangNum];
		memset(pblockLang,0,sizeof(SCI_BZP_BLOCK_HEAD_T)*nLangNum);
		pdataLang = new DATA_INFO[nLangNum];
		memset(pdataLang,0,sizeof(DATA_INFO)*nLangNum);
		for(int i=0;i< nLangNum; i++)
		{
			pblockLang[i].magic = BZP_BLOCK_MAGIC;
			pblockLang[i].type = BZP_BLOCK_LANG;
		}
	}

	BYTE *pBufCodeZip  = NULL;
	BYTE *pBufResZip   = NULL;
	BYTE *pBufDspZip   = NULL;
	BYTE *pBufExtZip   = NULL;//@polojiang added on 20121127
	BYTE *pBufKernZip  = NULL;

	//BYTE *pDstBuf	   = NULL;
	DWORD dwDstSize    = 0;

	DWORD dwCodeOrgSize = 0;
	DWORD dwCodeZipSize = 0;

	DWORD dwResOrgSize = 0;
	DWORD dwResZipSize = 0;
	
	DWORD dwDspOrgSize = 0;
	DWORD dwDspZipSize = 0;

	DWORD dwExtOrgSize = 0;
	DWORD dwExtZipSize = 0;//@polojiang added on 20121127

	DWORD dwKernOrgSize = 0;
	DWORD dwKernZipSize = 0;

	DWORD dwMaxLangSize = 0;

	BYTE pad[4] = {0xFF,0xFF,0xFF,0xFF};

	DWORD dwReserved = 0;
	
	fileHead.block_num	= nFileNum;
	fileHead.magic		= (m_tParam.nCmpType == BZP_RAW || !bExtZ) ? BZP_HEAD_MAGIC2 : BZP_HEAD_MAGIC;
	fileHead.version	= 0;
	
	blockHeadCode.magic = BZP_BLOCK_MAGIC;
	blockHeadCode.type  = BZP_BLOCK_CODE;
	
	blockHeadRes.magic = BZP_BLOCK_MAGIC;
	blockHeadRes.type  = BZP_BLOCK_RES;

	blockHeadDsp.magic = BZP_BLOCK_MAGIC;
	blockHeadDsp.type  = BZP_BLOCK_DSP;

	blockHeadKern.magic = BZP_BLOCK_MAGIC;
	blockHeadKern.type  = BZP_BLOCK_KERN;
	
	blockHeadExt.magic = (m_tParam.nCmpType == BZP_RAW || !bExtZ) ? BZP_BLOCK_MAGIC2 : BZP_BLOCK_MAGIC; 
	if (-1 != nIndex)
	{
		blockHeadExt.type  = BZP_EXT_MAGIC_PRE | (nIndex/10)<<8 | nIndex%10;
	}
	

	FILE *pFile = NULL;
	DWORD dwPad = 0;
	CFileFind finder;
	DWORD dwWrite = 0;
	DWORD dwLangID = MAXDWORD;
	DWORD dwCurOffset = ((sizeof(SCI_BZP_FILE_HEAD_T) + 3) / 4) * 4;
	DWORD dwTotalOrgSize = 0;
	//////////////////////////////////////////////////////////////////////////
	// deal with code file
	if(!strCodeFile.IsEmpty())
	{
		if(!GetCmpData(strCodeFile.operator LPCTSTR(),dwCodeOrgSize,pBufCodeZip,dwCodeZipSize,dwLangID))
		{
			goto BZP_ERROR;
		}

		blockHeadCode.block_len = dwCodeZipSize;
		blockHeadCode.start_pos = dwCurOffset;
		dwCurOffset = dwCurOffset + ((dwCodeZipSize + 3) / 4) * 4;
		dwTotalOrgSize += dwCodeOrgSize;
	}
	//////////////////////////////////////////////////////////////////////////
	// deal with res file
	if(!strResFile.IsEmpty())
	{
		if(!GetCmpData(strResFile.operator LPCTSTR(),dwResOrgSize,pBufResZip,dwResZipSize,dwLangID))
		{
			goto BZP_ERROR;
		}

		blockHeadRes.block_len = dwResZipSize;	
		blockHeadRes.start_pos = dwCurOffset;
		dwCurOffset = dwCurOffset + ((dwResZipSize + 3) / 4) * 4;
		dwTotalOrgSize += dwResOrgSize;
	}	
	//////////////////////////////////////////////////////////////////////////
	// deal with lang theme file
	for(i=0; i< agLangFile.GetSize();i++)
	{
		dwLangID = 0x10000000;
		if(!GetCmpData(agLangFile[i].operator LPCTSTR(),pdataLang[i].dwOrgSize,pdataLang[i].pZipBuf,pdataLang[i].dwZipSize,dwLangID))
		{
			goto BZP_ERROR;
		}

		
		pblockLang[i].block_len = pdataLang[i].dwZipSize;
		pblockLang[i].start_pos = dwCurOffset;
		dwCurOffset = dwCurOffset + ((pdataLang[i].dwZipSize + 3) / 4) * 4;
		// not calc language size
		// dwTotalOrgSize += pdataLang[i].dwOrgSize;
		pblockLang[i].des_addr = dwLangID;

		if(pdataLang[i].dwOrgSize > dwMaxLangSize)
		{
			dwMaxLangSize = pdataLang[i].dwOrgSize;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// deal with dsp file
	if(!strDspFile.IsEmpty())
	{
		if(!GetCmpData(strDspFile.operator LPCTSTR(),dwDspOrgSize,pBufDspZip,dwDspZipSize,dwLangID))
		{
			goto BZP_ERROR;
		}
		
		blockHeadDsp.block_len = dwDspZipSize;	
		blockHeadDsp.start_pos = dwCurOffset;
		blockHeadDsp.des_addr = dwDspOrgSize;
		dwCurOffset = dwCurOffset + ((dwDspZipSize + 3) / 4) * 4;
		dwTotalOrgSize += dwDspOrgSize;
	}
	//////////////////////////////////////////////////////////////////////////
	//@polojiang added on 20121127
	// deal with ext file
	if(!strExtFile.IsEmpty())
	{
		BOOL bCmpExt = FALSE;
		if (bExtZ)
		{
			bCmpExt = GetCmpData(strExtFile.operator LPCTSTR(),dwExtOrgSize,pBufExtZip,dwExtZipSize,dwLangID);
		}
		else
		{
			bCmpExt = GetCmpDataExt(strExtFile.operator LPCTSTR(),dwExtOrgSize,pBufExtZip,dwExtZipSize,dwLangID);
		}
		
		if(!bCmpExt)
		{
			goto BZP_ERROR;
		}
		
		blockHeadExt.block_len = dwExtZipSize;	
		blockHeadExt.start_pos = dwCurOffset;
		blockHeadExt.des_addr = dwExtOrgSize;
		dwCurOffset = dwCurOffset + ((dwExtZipSize + 3) / 4) * 4;
		dwTotalOrgSize += dwExtOrgSize;
	}
	//////////////////////////////////////////////////////////////////////////
	// deal with kernel file
	if(!strKernFile.IsEmpty())
	{
		if(!GetCmpData(strKernFile.operator LPCTSTR(),dwKernOrgSize,pBufKernZip,dwKernZipSize,dwLangID))
		{
			goto BZP_ERROR;
		}
		
		blockHeadKern.block_len = dwKernZipSize;	
		blockHeadKern.start_pos = dwCurOffset;
		blockHeadKern.des_addr = dwKernOrgSize;
		dwCurOffset = dwCurOffset + ((dwKernZipSize + 3) / 4) * 4;
		dwTotalOrgSize += dwKernOrgSize;
	}

	if(dwTotalOrgSize > m_tParam.dwMaxRam)
	{
		_tprintf(_T("Original total size [0x%X] is over the max ram size [0x%X]."),dwTotalOrgSize,m_tParam.dwMaxRam);
		_stprintf(m_szLastErrMsg,_T("Original total size [0x%X] is over the max ram size [0x%X]."),dwCodeOrgSize + dwResOrgSize,m_tParam.dwMaxRam);
		goto BZP_ERROR;
	}

	//for debug
	dwDstSize = dwCurOffset +
				sizeof(SCI_BZP_BLOCK_HEAD_T) * nFileNum +
				sizeof(DWORD)*2;

	if(dwDstSize>m_tParam.dwMaxBzp)
	{
		_tprintf(_T("Bzp size [0x%X] is over the max bzp size [0x%X]."),dwDstSize,m_tParam.dwMaxBzp);
		_stprintf(m_szLastErrMsg,_T("Bzp size [0x%X] is over the max bzp size [0x%X]."),dwDstSize,m_tParam.dwMaxBzp);
		goto BZP_ERROR;
	}

	if(finder.FindFile(pszBzpFile))
	{
		DWORD dwAtt = ::GetFileAttributes(pszBzpFile);
		dwAtt &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(pszBzpFile,dwAtt);
	}

	pFile = _tfopen(pszBzpFile,_T("wb"));

	if(pFile == NULL)
	{
		_tprintf(_T("create bzp file [%s] failed."),pszBzpFile);
		_stprintf(m_szLastErrMsg,_T("create bzp file [%s] failed."),pszBzpFile);
		goto BZP_ERROR;
	}
	fileHead.block_head_pos = dwCurOffset;
	ConvertBZPFileHead(fileHead,m_tParam.bBigEndian);

#define BZP_WRITE_FILE(p,t) do{\
	dwWrite = fwrite(p,1,(t),pFile);\
	if(dwWrite != (t))\
	{\
		_tprintf(_T("write file [%s] failed."),pszBzpFile);\
		_stprintf(m_szLastErrMsg,_T("write file [%s] failed."),pszBzpFile);\
		goto BZP_ERROR;\
	}\
	}while(0)\


	BZP_WRITE_FILE(&fileHead,sizeof(fileHead));

	if(pBufCodeZip != NULL)
	{		
		BZP_WRITE_FILE(pBufCodeZip,dwCodeZipSize);
		if(dwCodeZipSize%4 != 0)
		{
			dwPad = 4- dwCodeZipSize%4;	
			BZP_WRITE_FILE(pad,dwPad);
		}
	}

	if(pBufResZip != NULL)
	{
		BZP_WRITE_FILE(pBufResZip,dwResZipSize);
		if(dwResZipSize%4 != 0)
		{
			dwPad = 4- dwResZipSize%4;
			BZP_WRITE_FILE(pad,dwPad);
		}
	}

	for(i=0; i< agLangFile.GetSize(); i++)
	{
		BZP_WRITE_FILE(pdataLang[i].pZipBuf,pdataLang[i].dwZipSize);
		if(pdataLang[i].dwZipSize%4 != 0)
		{
			dwPad = 4- pdataLang[i].dwZipSize%4;
			BZP_WRITE_FILE(pad,dwPad);
		}
	}

	if(pBufDspZip != NULL)
	{
		BZP_WRITE_FILE(pBufDspZip,dwDspZipSize);
		if(dwDspZipSize%4 != 0)
		{
			dwPad = 4- dwDspZipSize%4;
			BZP_WRITE_FILE(pad,dwPad);
		}
	}

	if(pBufExtZip != NULL)//@polojiang added on 20121127
	{
		BZP_WRITE_FILE(pBufExtZip,dwExtZipSize);
		if(dwExtZipSize%4 != 0)
		{
			dwPad = 4- dwExtZipSize%4;
			BZP_WRITE_FILE(pad,dwPad);
		}
	}

	if(pBufKernZip != NULL)
	{
		BZP_WRITE_FILE(pBufKernZip,dwKernZipSize);
		if(dwKernZipSize%4 != 0)
		{
			dwPad = 4- dwKernZipSize%4;
			BZP_WRITE_FILE(pad,dwPad);
		}
	}

	if(pBufCodeZip != NULL)
	{
		ConvertBZPBlockHead(blockHeadCode,m_tParam.bBigEndian);
		BZP_WRITE_FILE(&blockHeadCode,sizeof(blockHeadCode));
	}

	if(pBufResZip != NULL)
	{
		ConvertBZPBlockHead(blockHeadRes,m_tParam.bBigEndian);
		BZP_WRITE_FILE(&blockHeadRes,sizeof(blockHeadRes));
	}
	for(i=0; i< agLangFile.GetSize(); i++)
	{
		ConvertBZPBlockHead(pblockLang[i],m_tParam.bBigEndian);
		BZP_WRITE_FILE(&pblockLang[i],sizeof(pblockLang[i]));
	}
	if(pBufDspZip != NULL)
	{
		ConvertBZPBlockHead(blockHeadDsp,m_tParam.bBigEndian);
		BZP_WRITE_FILE(&blockHeadDsp,sizeof(blockHeadDsp));
	}
	if(pBufExtZip != NULL)//@polojiang added on 20121127
	{
		ConvertBZPBlockHead(blockHeadExt,m_tParam.bBigEndian);
		BZP_WRITE_FILE(&blockHeadExt,sizeof(blockHeadExt));
	}
	if(pBufKernZip != NULL)
	{
		ConvertBZPBlockHead(blockHeadKern,m_tParam.bBigEndian);
		BZP_WRITE_FILE(&blockHeadKern,sizeof(blockHeadKern));
	}

	//write MAX RAM and BZP limited param
	CONVERT_INT(dwPad,m_tParam.dwMaxRam,m_tParam.bBigEndian);
	BZP_WRITE_FILE(&dwPad,sizeof(dwPad));

	CONVERT_INT(dwPad,m_tParam.dwMaxBzp,m_tParam.bBigEndian);
	BZP_WRITE_FILE(&dwPad,sizeof(dwPad));
	
	//write 2 DWORD for reserved
	BZP_WRITE_FILE(&dwReserved,sizeof(dwReserved));
	BZP_WRITE_FILE(&dwReserved,sizeof(dwReserved));

	//write MAX lang size
	CONVERT_INT(dwMaxLangSize,dwMaxLangSize,m_tParam.bBigEndian);
	BZP_WRITE_FILE(&dwMaxLangSize,sizeof(dwMaxLangSize));


	fclose(pFile);
	pFile = NULL;

	SAFE_DEL(pBufCodeZip);
	SAFE_DEL(pBufResZip);
	SAFE_DEL(pBufDspZip);
	SAFE_DEL(pBufExtZip);//@polojiang added on 20121127
	SAFE_DEL(pblockLang);
	SAFE_DEL(pBufKernZip);
	for(i=0; i< agLangFile.GetSize(); i++)
	{
		SAFE_DEL(pdataLang[i].pZipBuf);
	}
	SAFE_DEL(pdataLang);

	return TRUE;


BZP_ERROR:
	if(pFile != NULL)
	{
		fclose(pFile);
	}
	SAFE_DEL(pBufCodeZip);
	SAFE_DEL(pBufResZip);
	SAFE_DEL(pblockLang);
	SAFE_DEL(pBufDspZip);
	SAFE_DEL(pBufExtZip);//@polojiang added on 20121127
	SAFE_DEL(pBufKernZip);
	for(i=0; i< agLangFile.GetSize(); i++)
	{
		SAFE_DEL(pdataLang[i].pZipBuf);
	}
	SAFE_DEL(pdataLang);

	return FALSE;
	
}

BOOL CBZip::MergeEx(LPCTSTR pszMidBzpFile,LPCTSTR pszExtBzpFile, LPCTSTR pszXFile)
{
	
	if( pszMidBzpFile == NULL || pszExtBzpFile == NULL || pszXFile == NULL)
	{
		_tprintf(_T("MergeEx: invalid param, file is Null."));
		_stprintf(m_szLastErrMsg,_T("MergeEx: invalid param, file is Null."));
		return FALSE;
	}

	CString strExFiles[MAX_EXT_NUM];
	BOOL bRetExtz = TRUE;
	
	

	CString strPSFile = pszMidBzpFile;
	LPBYTE	pBufPs = NULL;
	DWORD	dwPsSize = 0;

	CString strExtBzpFile = pszExtBzpFile;
	LPBYTE	pBufExtBzp		= NULL;
	DWORD	dwExtBzpSize	= 0;
	DWORD	dwExtOffset		= 0;
	DWORD	dwOffset		= 0;

	FILE*	pFile = NULL;
	BYTE	pad[4] = {0xFF,0xFF,0xFF,0xFF};
	DWORD	dwPad = 0;
	DWORD	dwWrite = 0;
	CFileFind		finder;

	BOOL bFind = FALSE;
	BYTE head[8] = {0};
	BYTE tail[4] = {0};
	DWORD dwMagic = 0;
	DWORD dwEnd = 0;
	LPBYTE lpPos = NULL;
	LPBYTE lpCur = NULL;
	LPBYTE lpEnd = NULL;
	LPBYTE lpDataEnd = NULL;

	//////////////////////////////////////////////////////////////////////////	
	// deal with include ps file
	if (!strPSFile.IsEmpty())
	{
		if(!GetOrgData(strPSFile.operator LPCTSTR(),pBufPs,dwPsSize))
		{
			goto MERGE_EX_ERROR;
		}
	}

	// deal with ext bzp file
	if (!strExtBzpFile.IsEmpty())
	{
		if(!GetOrgData(strExtBzpFile.operator LPCTSTR(),pBufExtBzp,dwExtBzpSize))
		{
			goto MERGE_EX_ERROR;
		}
	}


	dwMagic = PARTITION_MAGIC_FIRST;
	CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
	*(DWORD*)head = dwMagic;

	dwMagic = PARTITION_MAGIC_SECOND;
	CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
	*(DWORD*)(head+4) = dwMagic;

	dwMagic = PARTITION_MAGIC_END;
	CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
	*(DWORD*)tail = dwMagic;

	dwEnd = dwPsSize < MAX_SEARCH_SIZE? dwPsSize : MAX_SEARCH_SIZE;
	lpCur = pBufPs;
	lpEnd = pBufPs+dwEnd;
	lpDataEnd = pBufPs + dwPsSize;

	while(lpCur < lpEnd)
	{
		// find head 
		lpPos = std::search(lpCur,lpEnd, head,head+7);
		if(lpPos < lpEnd)
		{
			if((lpPos + 8+ sizeof(UPDATE_DATA_T) +4) < lpDataEnd)
			{
				// check tail
				if(
					memcmp(lpPos + 8+ sizeof(UPDATE_DATA_T) +MAX_EXT_NUM*sizeof(DWORD), tail,4) == 0 
				  )
				{
					bFind = TRUE;
					break;
				}
				else
				{
					lpCur += 8;
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	if(!bFind)
	{
		_tprintf(_T("cannot find flag in ps file [%s]."),strPSFile.operator LPCTSTR());
		_stprintf(m_szLastErrMsg,_T("cannot file flag in ps file [%s]."),strPSFile.operator LPCTSTR());
		goto MERGE_EX_ERROR;
	}

	lpPos+= 28;
	
	memcpy(lpPos,pBufExtBzp,MAX_EXT_NUM*sizeof(DWORD));
	dwOffset = ((dwPsSize + 3)/4) * 4;

	if(finder.FindFile(pszXFile))
	{
		DWORD dwAtt = ::GetFileAttributes(pszXFile);
		dwAtt &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(pszXFile,dwAtt);
		finder.Close();
	}
	
	pFile = _tfopen(pszXFile,_T("wb"));
	if(pFile == NULL)
	{
		_tprintf(_T("cannot create file [%s]."),pszXFile);
		_stprintf(m_szLastErrMsg,_T("cannot create file [%s]."),pszXFile);
		goto MERGE_EX_ERROR;
	}
	
#define MERGEEX_WRITE_FILE(p,t) do{\
	dwWrite = fwrite(p,1,(t),pFile);\
	if(dwWrite != (t))\
	{\
	_tprintf(_T("write file [%s] failed."),pszXFile);\
	_stprintf(m_szLastErrMsg,_T("write file [%s] failed."),pszXFile);\
	goto MERGE_EX_ERROR;\
	}\
	}while(0)

	MERGEEX_WRITE_FILE(pBufPs,dwPsSize);
	
	
	dwPad = ftell(pFile);
	dwPad = ((dwPad + 3)/4) * 4  - dwPad;
	if(dwPad > 0)
	{
		MERGEEX_WRITE_FILE(pad,dwPad);
	}
	
	if(dwExtBzpSize != 0)
	{
		MERGEEX_WRITE_FILE(pBufExtBzp+MAX_EXT_NUM*sizeof(DWORD),dwExtBzpSize-MAX_EXT_NUM*sizeof(DWORD));
		dwPad = ftell(pFile);
		dwPad = ((dwPad + 3)/4) * 4  - dwPad;
		if(dwPad > 0)
		{
			MERGEEX_WRITE_FILE(pad,dwPad);
		}
	}

	if(pFile)
	{
		fclose(pFile);
		pFile = NULL;
	}
	SAFE_DEL(pBufPs);
	SAFE_DEL(pBufExtBzp);

	return TRUE;

MERGE_EX_ERROR:
	if(pFile)
	{
		fclose(pFile);
		pFile = NULL;
	}
	SAFE_DEL(pBufPs);
	SAFE_DEL(pBufExtBzp);
	
	return FALSE;

}
BOOL CBZip::GenExtBzp(CStringArray &agFileEx,DWORD dwTmpBzpSize, LPCTSTR pszXFile)//@polojiang added on 20121127
{
	int nFileCount = agFileEx.GetSize()/2;
	if( nFileCount > MAX_EXT_NUM || pszXFile == NULL)
	{
		_tprintf(_T("MergeEx: invalid param, max support 24 files."));
		_stprintf(m_szLastErrMsg,_T("MergeEx: invalid param, max support 24 files."));
		return FALSE;
	}
	
	
	BOOL bRetExtz = TRUE;

	FILE*	pFile = NULL;
	BYTE	pad[4] = {0xFF,0xFF,0xFF,0xFF};
	DWORD	dwPad = 0;
	DWORD	dwWrite = 0;
	CFileFind		finder;
	
	BOOL bFind = FALSE;
	BYTE head[8] = {0};
	BYTE tail[4] = {0};
	DWORD dwMagic = 0;
	DWORD dwEnd = 0;
	LPBYTE lpPos = NULL;
	LPBYTE lpCur = NULL;
	LPBYTE lpEnd = NULL;
	LPBYTE lpDataEnd = NULL;
	
	CString strExFiles[MAX_EXT_NUM];
	LPBYTE	pBufExt[MAX_EXT_NUM] = {NULL};
	DWORD	dwExtSize[MAX_EXT_NUM]	= {0};
	DWORD	dwExtOffset[MAX_EXT_NUM] = {0};
	DWORD	dwOffset				= dwTmpBzpSize;
	int i = 0;
	 
	for(i = 0; i< nFileCount; i++)
	{
		CString str = agFileEx[2*i];
		int nIndex = 0;
		CStringArray agExtFiles;	
		_TCHAR szExtBzp[MAX_PATH*2] = {0};
		_TCHAR szName[MAX_PATH] = {0};
		if (-1 != str.Find(EXTZ_KEY))  //-extz
		{
			nIndex = _ttoi(str.Mid(5));
			
			GetTempPath(MAX_PATH*2,szExtBzp);
			agExtFiles.Add(agFileEx[2*i]);
			agExtFiles.Add(agFileEx[2*i+1]);
			
			_stprintf(szName,_T("ext_bzp%d_%d.bin"),i,GetTickCount());
			_tcscat(szExtBzp,szName);
			
			bRetExtz = Bzp(agExtFiles,szExtBzp);
			if (!bRetExtz)
			{
				_tprintf(_T("Gen extz_%d file failed !"),i);
				_stprintf(m_szLastErrMsg,_T("Gen extz_%d file failed !"),i);
				goto GENEXTBZP_ERROR;
			}
			strExFiles[nIndex] = szExtBzp;
			_tprintf(_T("\nExtBzp file [%s]."),strExFiles[nIndex]);
		}
		else	//-ext
		{
			nIndex = _ttoi(str.Mid(4));	
			
			GetTempPath(MAX_PATH*2,szExtBzp);
			CString str;
			str.Format(_T("%s%d"),EXTZ_KEY,nIndex);
			agExtFiles.Add(str);
			agExtFiles.Add(agFileEx[2*i+1]);
			
			_stprintf(szName,_T("ext_bzp%d_%d.bin"),i,GetTickCount());
			_tcscat(szExtBzp,szName);
			
			bRetExtz = Bzp(agExtFiles,szExtBzp,FALSE);
			if (!bRetExtz)
			{
				_tprintf(_T("Gen extz_%d file failed !"),i);
				_stprintf(m_szLastErrMsg,_T("Gen extz_%d file failed !"),i);
				goto GENEXTBZP_ERROR;
			}
			strExFiles[nIndex] = szExtBzp;

			_tprintf(_T("\nExt file [%s]."),strExFiles[nIndex]);
		}
	}
	if(finder.FindFile(pszXFile))
	{
		DWORD dwAtt = ::GetFileAttributes(pszXFile);
		dwAtt &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(pszXFile,dwAtt);
		finder.Close();
	}
	
	pFile = _tfopen(pszXFile,_T("wb"));
	if(pFile == NULL)
	{
		_tprintf(_T("cannot create file [%s]."),pszXFile);
		_stprintf(m_szLastErrMsg,_T("cannot create file [%s]."),pszXFile);
		goto GENEXTBZP_ERROR;
	}
	
#define GENBZPEX_WRITE_FILE(p,t) do{\
	dwWrite = fwrite(p,1,(t),pFile);\
	if(dwWrite != (t))\
	{\
	_tprintf(_T("\nwrite file [%s] failed."),pszXFile);\
	_stprintf(m_szLastErrMsg,_T("write file [%s] failed."),pszXFile);\
	goto GENEXTBZP_ERROR;\
	}\
	}while(0)

	for(i = 0;i< MAX_EXT_NUM; i++)
	{ 
		if (!strExFiles[i].IsEmpty())
		{
			if(!GetOrgData(strExFiles[i].operator LPCTSTR(),pBufExt[i],dwExtSize[i]))
			{
				goto GENEXTBZP_ERROR;
			}
			if( dwExtSize[i] != 0)
			{
				DWORD dwTmp = dwOffset;
				CONVERT_INT(dwTmp,dwOffset,m_tParam.bBigEndian);
				GENBZPEX_WRITE_FILE((LPBYTE)&dwTmp,4);
				dwOffset +=dwExtSize[i];
			}
			else
			{
				GENBZPEX_WRITE_FILE(pad,4);
			}
		}
		else
		{
			GENBZPEX_WRITE_FILE(pad,4);
		}
	}
	for(i = 0;i< MAX_EXT_NUM; i++)
	{
		if( dwExtSize[i] != 0)
		{
			GENBZPEX_WRITE_FILE(pBufExt[i],dwExtSize[i]);
			dwPad = ftell(pFile);
			dwPad = ((dwPad + 3)/4) * 4  - dwPad;
			if(dwPad > 0)
			{
				GENBZPEX_WRITE_FILE(pad,dwPad);
			}
		}
	}


	if(pFile)
	{
		fclose(pFile);
		pFile = NULL;
	}
	for(i = 0;i< MAX_EXT_NUM; i++)
	{
		if( pBufExt[i])
		{
			SAFE_DEL(pBufExt[i]);
		}
	}
	return TRUE;

GENEXTBZP_ERROR:
	if(pFile)
	{
		fclose(pFile);
		pFile = NULL;
	}
	for(i = 0;i< MAX_EXT_NUM; i++)
	{
		if( pBufExt[i])
		{
			SAFE_DEL(pBufExt[i]);
		}
	}

	return FALSE;

}
BOOL CBZip::Merge(CStringArray &agFile, LPCTSTR pszXFile)
{
	if((agFile.GetSize()/2 != 2 && agFile.GetSize()/2 != 3 && agFile.GetSize()/2 != 4)  || pszXFile == NULL)
	{
		_tprintf(_T("Merge: invalid param, only support 2 or 3 files."));
		_stprintf(m_szLastErrMsg,_T("Merge: invalid param, only support 2 or 3 files."));
		return FALSE;
	}

	CString strPSFile;
	CString strBzpFile;
	CString strResFile;
	CString strCodeFile;
	CString strKernFile;
	CString strKernZFile;
	CString strDspFile;
	CString strDspZFile = _T(""); //@polojiang added on 20111214
	CString str2ndFile;
	CString str3rdFile;
	CString str4thFile;
	int nFileCount = agFile.GetSize()/2;
	int nFileNum=0;
	BOOL bResultDspz = TRUE;
	BOOL bResultKernz = TRUE;

	for(int i = 0;i< nFileCount; i++)
	{
		if(agFile[2*i].CompareNoCase(_T("ps")) == 0)
		{
			strPSFile = agFile[2*i+1];
			nFileNum++;
		}
		else if(agFile[2*i].CompareNoCase(_T("res")) == 0)
		{			
			strResFile = agFile[2*i+1];
			nFileNum++;
			str2ndFile = strResFile;
		}
		else if(agFile[2*i].CompareNoCase(_T("code")) == 0)
		{			
			strCodeFile = agFile[2*i+1];
			nFileNum++;
			str2ndFile = strCodeFile;
		}
		else if(agFile[2*i].CompareNoCase(_T("kern")) == 0)
		{			
			strKernFile = agFile[2*i+1];
			nFileNum++;
			str4thFile = strKernFile;
		}
		else if(agFile[2*i].CompareNoCase(_T("kernz")) == 0)
		{	
			CStringArray agKernFile;	
			_TCHAR szKernBzp[MAX_PATH*2] = {0};
			_TCHAR szName[MAX_PATH] = {0};
			strKernZFile = agFile[2*i+1];
			nFileNum++;
			
			GetTempPath(MAX_PATH*2,szKernBzp);
			agKernFile.Add(_T("kernz"));
			agKernFile.Add(strKernZFile);
			
			_stprintf(szName,_T("kern_bzp%d.bin"),GetTickCount());
			_tcscat(szKernBzp,szName);
			
			bResultKernz = Bzp(agKernFile,szKernBzp);
			str4thFile = szKernBzp;
		}
		else if(agFile[2*i].CompareNoCase(_T("dsp")) == 0)
		{			
			strDspFile = agFile[2*i+1];
			nFileNum++;
			str3rdFile = strDspFile;
		}
		else if(agFile[2*i].CompareNoCase(_T("dspz")) == 0)//@polojiang added on 20111214
		{	
			
			CStringArray agDspFile;	
			_TCHAR szDspBzp[MAX_PATH*2] = {0};
			_TCHAR szName[MAX_PATH] = {0};
			strDspZFile = agFile[2*i+1];
			nFileNum++;
			
			GetTempPath(MAX_PATH*2,szDspBzp);
			agDspFile.Add(_T("dspz"));
			agDspFile.Add(strDspZFile);
			
			_stprintf(szName,_T("dsp_bzp%d.bin"),GetTickCount());
			_tcscat(szDspBzp,szName);
			
			bResultDspz = Bzp(agDspFile,szDspBzp);
			str3rdFile = szDspBzp;

			////////////
		}//@end polojiang added on 20111214
		else if(agFile[2*i].CompareNoCase(_T("bzp")) == 0)
		{			
			strBzpFile = agFile[2*i+1];
			nFileNum++;
			str2ndFile = strBzpFile;
		}
	}

	if((nFileNum != 2 && nFileNum != 3 && nFileNum != 4 ) || 
		(strPSFile.IsEmpty() && (str2ndFile.IsEmpty() || str3rdFile.IsEmpty() || str4thFile.IsEmpty())))
	{
		_tprintf(_T("Merge: file name is empty."));
		_stprintf(m_szLastErrMsg,_T("Merge: file name is empty."));
		return FALSE;
	}

	LPBYTE pBufPs = NULL;
	DWORD  dwPsSize = 0;
	LPBYTE pBuf2nd  = NULL;
	DWORD  dw2ndSize  = 0;
	DWORD  dw2ndOffset = 0;
	LPBYTE pBuf3rd  = NULL;
	DWORD  dw3rdSize  = 0;
	DWORD  dw3rdOffset = 0;

	LPBYTE pBuf4th  = NULL;
	DWORD  dw4thSize  = 0;
	DWORD  dw4thOffset = 0;

	DWORD dwOffset= 0;

	FILE *pFile = NULL;
	DWORD dwPad = 0;
	BYTE pad[4] = {0xFF,0xFF,0xFF,0xFF};
	DWORD dwWrite = 0;
	CFileFind finder;
	UPDATE_DATA_T ud;
	memset(&ud,0xFF,sizeof(ud));

	BOOL bFind = FALSE;
	BYTE head[8] = {0};
	BYTE tail[4] = {0};
	DWORD dwMagic = 0;
	DWORD dwEnd = 0;
	LPBYTE lpPos = NULL;
	LPBYTE lpCur = NULL;
	LPBYTE lpEnd = NULL;
	LPBYTE lpDataEnd = NULL;

	//////////////////////////////////////////////////////////////////////////
	if (!bResultDspz)
	{
		_tprintf(_T("Gen dspz file failed !"));
		_stprintf(m_szLastErrMsg,_T("Gen dspz file failed !"));
		goto MERGE_ERROR;
	}

	if (!bResultKernz)
	{
		_tprintf(_T("Gen kernz file failed !"));
		_stprintf(m_szLastErrMsg,_T("Gen kernz file failed !"));
		goto MERGE_ERROR;
	}
	
	// deal with ps file
	if (!strPSFile.IsEmpty())
	{
		if(!GetOrgData(strPSFile.operator LPCTSTR(),pBufPs,dwPsSize))
		{
			goto MERGE_ERROR;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// deal with res/code/bzp file
	if (!str2ndFile.IsEmpty())
	{
		if(!GetOrgData(str2ndFile.operator LPCTSTR(),pBuf2nd,dw2ndSize))
		{
			goto MERGE_ERROR;
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	// deal with dsp file 
	if(!str3rdFile.IsEmpty())
	{
		if(!GetOrgData(str3rdFile.operator LPCTSTR(),pBuf3rd,dw3rdSize))
		{
			goto MERGE_ERROR;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// deal with  kernel file 
	if(!str4thFile.IsEmpty())
	{
		if(!GetOrgData(str4thFile.operator LPCTSTR(),pBuf4th,dw4thSize))
		{
			goto MERGE_ERROR;
		}
	}

	if(dw2ndSize>m_tParam.dwMaxRam)
	{
		_tprintf(_T("Total size [0x%X] is over the max ram size [0x%X]."),dw2ndSize,m_tParam.dwMaxRam);
		_stprintf(m_szLastErrMsg,_T("Total size [0x%X] is over the max ram size [0x%X]."),dw2ndSize,m_tParam.dwMaxRam);
		goto MERGE_ERROR;
	}

	if(dw2ndSize>m_tParam.dwMaxBzp)
	{
		_tprintf(_T("Total size [0x%X] is over the max bzp size [0x%X]."),dw2ndSize,m_tParam.dwMaxBzp);
		_stprintf(m_szLastErrMsg,_T("Total size [0x%X] is over the max bzp size [0x%X]."),dw2ndSize,m_tParam.dwMaxBzp);
		goto MERGE_ERROR;
	}

	dwMagic = PARTITION_MAGIC_FIRST;
	CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
	*(DWORD*)head = dwMagic;

	dwMagic = PARTITION_MAGIC_SECOND;
	CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
	*(DWORD*)(head+4) = dwMagic;

	dwMagic = PARTITION_MAGIC_END;
	CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
	*(DWORD*)tail = dwMagic;

	dwEnd = dwPsSize < MAX_SEARCH_SIZE? dwPsSize : MAX_SEARCH_SIZE;
	lpCur = pBufPs;
	lpEnd = pBufPs+dwEnd;
	lpDataEnd = pBufPs + dwPsSize;

	while(lpCur < lpEnd)
	{
		// find head 
		lpPos = std::search(lpCur,lpEnd, head,head+7);
		if(lpPos < lpEnd)
		{
			if((lpPos + 8+ sizeof(UPDATE_DATA_T) +4) < lpDataEnd)
			{
				// check tail
				if(
					memcmp(lpPos + 8+ sizeof(UPDATE_DATA_T), tail,4) == 0 ||
					memcmp(lpPos + 8+ sizeof(UPDATE_DATA_T) +MAX_EXT_NUM*sizeof(DWORD), tail,4) == 0 
				  )
				{
					bFind = TRUE;
					break;
				}
				else
				{
					lpCur += 8;
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	if(!bFind)
	{
		_tprintf(_T("cannot find flag in ps file [%s]."),strPSFile.operator LPCTSTR());
		_stprintf(m_szLastErrMsg,_T("cannot file flag in ps file [%s]."),strPSFile.operator LPCTSTR());
		goto MERGE_ERROR;
	}

	lpPos+= 8;
	memcpy(&ud,lpPos,sizeof(ud));
	
	dwOffset = ((dwPsSize + 3)/4) * 4;



	if( dw2ndSize != 0)
	{
		dw2ndOffset = dwOffset;
		ud.res_addr = dwOffset;	
		CONVERT_INT(ud.res_addr,ud.res_addr,m_tParam.bBigEndian);	
		dwOffset += ((dw2ndSize + 3)/4) * 4;
	}

	if(dw3rdSize != 0)
	{
		dw3rdOffset = dwOffset;
		ud.dsp_addr = dwOffset;
		CONVERT_INT(ud.dsp_addr,ud.dsp_addr,m_tParam.bBigEndian);
		dwOffset += ((dw3rdSize + 3)/4) * 4;
	}

	if(dw4thSize != 0)
	{
		dw4thOffset = dwOffset;
		ud.image_addr = dwOffset;
		CONVERT_INT(ud.image_addr,ud.image_addr,m_tParam.bBigEndian);
		dwOffset += ((dw4thSize + 3)/4) * 4;
	}
	
	memcpy(lpPos,&ud,sizeof(ud));
	if(finder.FindFile(pszXFile))
	{
		DWORD dwAtt = ::GetFileAttributes(pszXFile);
		dwAtt &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(pszXFile,dwAtt);
	}

	pFile = _tfopen(pszXFile,_T("wb"));
	if(pFile == NULL)
	{
		_tprintf(_T("cannot create file [%s]."),pszXFile);
		_stprintf(m_szLastErrMsg,_T("cannot create file [%s]."),pszXFile);
		goto MERGE_ERROR;
	}

#define MERGE_WRITE_FILE(p,t) do{\
	dwWrite = fwrite(p,1,(t),pFile);\
	if(dwWrite != (t))\
	{\
		_tprintf(_T("write file [%s] failed."),pszXFile);\
		_stprintf(m_szLastErrMsg,_T("write file [%s] failed."),pszXFile);\
		goto MERGE_ERROR;\
	}\
	}while(0)


	MERGE_WRITE_FILE(pBufPs,dwPsSize);


	dwPad = ftell(pFile);
	dwPad = ((dwPad + 3)/4) * 4  - dwPad;
	if(dwPad > 0)
	{
		MERGE_WRITE_FILE(pad,dwPad);
	}

	if(dw2ndSize != 0)
	{
		MERGE_WRITE_FILE(pBuf2nd,dw2ndSize);
		dwPad = ftell(pFile);
		dwPad = ((dwPad + 3)/4) * 4  - dwPad;
		if(dwPad > 0)
		{
			MERGE_WRITE_FILE(pad,dwPad);
		}
	}

	if(dw3rdSize != 0)
	{	
		if(strDspZFile.IsEmpty())
		{			
			SCI_BZP_FILE_HEAD_T  fileHead		= {0};
			SCI_BZP_BLOCK_HEAD_T blockHeadDsp	= {0};
			fileHead.block_head_pos = ((dw3rdSize + 3)/4) * 4 + sizeof(SCI_BZP_FILE_HEAD_T);
			fileHead.block_num = 1;
			fileHead.version = 0;
			fileHead.magic = BZP_HEAD_MAGIC;
			blockHeadDsp.block_len = dw3rdSize;
			blockHeadDsp.magic = BZP_BLOCK_MAGIC2;
			blockHeadDsp.type = BZP_BLOCK_DSP;
			blockHeadDsp.start_pos = sizeof(fileHead);
			
			ConvertBZPFileHead(fileHead,m_tParam.bBigEndian);
			ConvertBZPBlockHead(blockHeadDsp,m_tParam.bBigEndian);
			
			MERGE_WRITE_FILE(&fileHead,sizeof(fileHead));
			MERGE_WRITE_FILE(pBuf3rd,dw3rdSize);
			dwPad = ((dw3rdSize + 3)/4) * 4  - dw3rdSize;
			if(dwPad > 0)
			{
				MERGE_WRITE_FILE(pad,dwPad);
			}
			MERGE_WRITE_FILE(&blockHeadDsp,sizeof(blockHeadDsp));
		}
		else
		{
			MERGE_WRITE_FILE(pBuf3rd,dw3rdSize);
			dwPad = ((dw3rdSize + 3)/4) * 4  - dw3rdSize;
			if(dwPad > 0)
			{
				MERGE_WRITE_FILE(pad,dwPad);
			}
		}

	}

	dwPad = ftell(pFile);
	dwPad = ((dwPad + 3)/4) * 4  - dwPad;
	if(dwPad > 0)
	{
		MERGE_WRITE_FILE(pad,dwPad);
	}
	if(dw4thSize != 0)
	{
		MERGE_WRITE_FILE(pBuf4th,dw4thSize);
	}
	dwPad = ftell(pFile);
	dwPad = ((dwPad + 3)/4) * 4  - dwPad;
	if(dwPad > 0)
	{
		MERGE_WRITE_FILE(pad,dwPad);
	}


	fclose(pFile);
	pFile = NULL;

	SAFE_DEL(pBufPs);
	SAFE_DEL(pBuf2nd);
	SAFE_DEL(pBuf3rd);
	SAFE_DEL(pBuf4th);

	return TRUE;

MERGE_ERROR:
	if(pFile != NULL)
	{
		fclose(pFile);
	}
	SAFE_DEL(pBufPs);
	SAFE_DEL(pBuf2nd);
	SAFE_DEL(pBuf3rd);
	SAFE_DEL(pBuf4th);

	return FALSE;
}

BOOL CBZip::BzpUncompress(LPCTSTR pszDstDir,CStringArray &agFile,BOOL bCheckRes /*= TRUE*/)
{
	USES_CONVERSION;
	agFile.RemoveAll();
	SCI_BZP_FILE_HEAD_T bfh;
	memcpy(&bfh,m_pBzpBuf,sizeof(bfh));
	ConvertBZPFileHead(bfh,m_tParam.bBigEndian);

	if( ( bfh.block_head_pos + sizeof(SCI_BZP_BLOCK_HEAD_T)*bfh.block_num ) < m_dwBzpSize)
	{
		m_tParam.dwMaxRam = *(DWORD*)(m_pBzpBuf+( bfh.block_head_pos + sizeof(SCI_BZP_BLOCK_HEAD_T)*bfh.block_num ));
		m_tParam.dwMaxBzp = *(DWORD*)(m_pBzpBuf+( bfh.block_head_pos + sizeof(SCI_BZP_BLOCK_HEAD_T)*bfh.block_num )+4);
		CONVERT_INT(m_tParam.dwMaxRam,m_tParam.dwMaxRam,m_tParam.bBigEndian);
		CONVERT_INT(m_tParam.dwMaxBzp,m_tParam.dwMaxBzp,m_tParam.bBigEndian);		
	}

	if(bfh.magic == BZP_HEAD_MAGIC2)
	{
		m_tParam.nCmpType = BZP_RAW;
	}

	CString strResFile;
	CString strCodeFile;
	CString strDspFile;
	CString strUsrFile;
	CStringArray agLangFile;
	int nLangIndex = 0;
	TCHAR szIniFile[MAX_PATH] = {0};
	::GetModuleFileName(NULL,szIniFile,MAX_PATH);
	TCHAR *pFind = _tcsrchr(szIniFile,_T('\\'));
	*pFind = '\0';
	_tcscat(szIniFile,_T("\\bzp.ini"));

	for(int i = 0; i< (int)bfh.block_num; i++)
	{
		SCI_BZP_BLOCK_HEAD_T bbh;
		memcpy(&bbh,m_pBzpBuf+bfh.block_head_pos+sizeof(bbh)*i,sizeof(bbh));
		ConvertBZPBlockHead(bbh,m_tParam.bBigEndian);
		if(bbh.type == BZP_BLOCK_RES || 
			bbh.type == BZP_BLOCK_CODE || 
			bbh.type == BZP_BLOCK_LANG ||
			bbh.type == BZP_BLOCK_DSP ||
			bbh.type == BZP_BLOCK_USR)
		{				

			CString strName;
			if(*(m_pBzpBuf+bbh.start_pos+3) == 0x8)
			{
				strName.Format(_T("%s\\%s"),pszDstDir,A2T((char*)(m_pBzpBuf+bbh.start_pos+10)));
			}
			else
			{
				if(bbh.type == BZP_BLOCK_RES)
				{
					strName.Format(_T("%s\\res.bin"),pszDstDir);
				}
				else if(bbh.type == BZP_BLOCK_CODE)
				{
					strName.Format(_T("%s\\code.bin"),pszDstDir);
				}
				else if(bbh.type == BZP_BLOCK_LANG)
				{
					strName.Format(_T("%s\\lang_%d.bin"),pszDstDir,nLangIndex++);
				}
				else if(bbh.type == BZP_BLOCK_DSP)
				{
					strName.Format(_T("%s\\dsp.bin"),pszDstDir);
				}
				else if(bbh.type == BZP_BLOCK_USR)
				{
					strName.Format(_T("%s\\usr.bin"),pszDstDir);
				}
				else
				{
					_tprintf(_T("unknown block type [0x%x] failed."),bbh.type);
					_stprintf(m_szLastErrMsg,_T("unknown block type [0x%x] failed."),bbh.type);		
					return FALSE;
				}
			}			

			SCI_BZP_FILE_HEAD_T header2;
			memcpy(&header2,m_pBzpBuf+bbh.start_pos,sizeof(header2));
			ConvertBZPFileHead(header2,m_tParam.bBigEndian);
			if(header2.magic == BZP_PAC_MAGIC)
			{
				m_tParam.bCat2 = TRUE;
				int nCmpType = 0;
				int nCmpLevel = 9;
				if(!UnSplitCmpFile(strName,m_pBzpBuf+bbh.start_pos, bbh.block_len,nCmpType,nCmpLevel))
				{
					_tprintf(_T("uncompress failed."));
					_stprintf(m_szLastErrMsg,_T("uncompress failed."));			
					return FALSE;
				}


				if(bbh.type == BZP_BLOCK_RES)
				{
					m_tParam.nResCmpType = nCmpType;
					//m_tParam.nResLevel   = nCmpLevel;
					//[polo.jiang]解决不同level压缩后的文件大小不一致的问题
					m_tParam.nResLevel   =  GetPrivateProfileInt(_T("Settings"),_T("ResLevel"),2,szIniFile);
					CheckCmpLevel(m_tParam.nResCmpType,m_tParam.nResLevel);
					m_tParam.dwResPacSize = bbh.des_addr;

				}			
				else if(bbh.type == BZP_BLOCK_USR)
				{
					m_tParam.nUsrCmpType = nCmpType;
					//m_tParam.nUsrLevel   = nCmpLevel;
					//[polo.jiang]解决不同level压缩后的文件大小不一致的问题
					m_tParam.nUsrLevel   =  GetPrivateProfileInt(_T("Settings"),_T("UsrLevel"),2,szIniFile);
					CheckCmpLevel(m_tParam.nUsrCmpType,m_tParam.nUsrLevel);
					m_tParam.dwUsrPacSize = bbh.des_addr;
				}
			}
			else
			{
				LPBYTE pBuf = NULL;
				DWORD dwDstSize = 0;

				int nTmpCmpType = m_tParam.nCmpType;
				if(bbh.magic == BZP_BLOCK_MAGIC2)
				{
					m_tParam.nCmpType = BZP_RAW;
				}

				if(!UnCompressData(pBuf,dwDstSize,m_pBzpBuf+bbh.start_pos, bbh.block_len,m_tParam.nCmpType,m_tParam.nLevel))
				{
					_tprintf(_T("uncompress failed."));
					_stprintf(m_szLastErrMsg,_T("uncompress failed."));		
					SAFE_DEL(pBuf);
					return FALSE;
				}
				if(bbh.magic == BZP_BLOCK_MAGIC2)
				{
					m_tParam.nCmpType = nTmpCmpType;
				}
				
				
				
				FILE *pFile = _tfopen(strName,_T("wb"));
				if(pFile == NULL)
				{
					_tprintf(_T("open file [%s] failed."),strName.operator LPCTSTR());
					_stprintf(m_szLastErrMsg,_T("open file [%s] failed."),strName.operator LPCTSTR());
					SAFE_DEL(pBuf);
					return FALSE;
				}
				else
				{
					fwrite(pBuf,1,dwDstSize,pFile);
				}
				
				fclose(pFile);

				SAFE_DEL(pBuf);
			}			

			if(bbh.type == BZP_BLOCK_RES)
			{
				strResFile = strName;
			}
			else if(bbh.type == BZP_BLOCK_CODE)
			{
				strCodeFile = strName;
			}
			else if(bbh.type == BZP_BLOCK_LANG)
			{
				agLangFile.Add(strName);
			}
			else if(bbh.type == BZP_BLOCK_DSP)
			{
				strDspFile = strName;
			}
			else if(bbh.type == BZP_BLOCK_USR)
			{
				strUsrFile = strName;
			}
			
		}
	}

	if(strResFile.IsEmpty())
	{
		if(bCheckRes)
		{
			_tprintf(_T("res file is empty."));
			_stprintf(m_szLastErrMsg,_T("res file is empty."));
			return FALSE;
		}
	}
	else
	{
		agFile.Add(_T("res"));
		agFile.Add(strResFile);
	}

	if(!strCodeFile.IsEmpty())
	{
		agFile.Add(_T("code"));
		agFile.Add(strCodeFile);
	}

	if(!strDspFile.IsEmpty())
	{
		agFile.Add(_T("dsp"));
		agFile.Add(strDspFile);
	}

	if(!strUsrFile.IsEmpty())
	{
		agFile.Add(_T("usr"));
		agFile.Add(strUsrFile);
	}


	for(int j= 0; j< agLangFile.GetSize(); j++)
	{
		agFile.Add(_T("lang"));
		agFile.Add(agLangFile[j]);
	}

	
	return TRUE;
}



LPBYTE CBZip::GetMergeInfo(BOOL bBigEndian)
{
	DWORD dwEnd = m_dwBzpSize < MAX_SEARCH_SIZE? m_dwBzpSize : MAX_SEARCH_SIZE;
	LPBYTE lpCur = m_pBzpBuf;
	LPBYTE lpEnd = m_pBzpBuf+dwEnd;
	LPBYTE lpDataEnd = m_pBzpBuf + m_dwBzpSize;
	LPBYTE lpPos= NULL;
	BYTE head[8] = {0};
	BYTE tail[4] = {0};
	DWORD dwMagic = 0;
	dwMagic = PARTITION_MAGIC_FIRST;
	CONVERT_INT(dwMagic,dwMagic,bBigEndian);
	*(DWORD*)head = dwMagic;
	
	dwMagic = PARTITION_MAGIC_SECOND;
	CONVERT_INT(dwMagic,dwMagic,bBigEndian);
	*(DWORD*)(head+4) = dwMagic;
	
	dwMagic = PARTITION_MAGIC_END;
	CONVERT_INT(dwMagic,dwMagic,bBigEndian);
	*(DWORD*)tail = dwMagic;
	
	BOOL bFind = FALSE;
	m_bExtPtnTbl = FALSE;
	while(lpCur < lpEnd)
	{
		// find head 
		lpPos = std::search(lpCur,lpEnd, head,head+7);
		if(lpPos < lpEnd)
		{
			if((lpPos + 8+sizeof(UPDATE_DATA_T) +4) < lpDataEnd)
			{
				// check tail
				if(memcmp(lpPos + 8 + sizeof(UPDATE_DATA_T), tail,4) == 0 )
				{
					bFind = TRUE;
					m_bExtPtnTbl = FALSE;
					break;
				}
				else if(memcmp(lpPos + 8+ sizeof(UPDATE_DATA_T) +MAX_EXT_NUM*sizeof(DWORD), tail,4) == 0 )
				{
					bFind = TRUE;
					m_bExtPtnTbl = TRUE;
					break;
				}
				else
				{
					lpCur += 8;
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	
	if(!bFind)
	{
		//_tprintf(_T("cannot find flag in bzp file."));
		//_stprintf(m_szLastErrMsg,_T("cannot find flag in bzp file."));
		return NULL;
	}
	else
	{
		return lpPos;
	}
}
/*
BOOL CBZip::MergeUncompress(LPCTSTR pszDstDir,CStringArray &agFile)
{	

	LPBYTE lpPos= NULL;

	lpPos = GetMergeInfo(TRUE);
	if(lpPos == NULL)
	{
		lpPos = GetMergeInfo(FALSE);
		if(lpPos == NULL)
		{
			_tprintf(_T("cannot find flag in bzp file."));
			_stprintf(m_szLastErrMsg,_T("cannot find flag in bzp file."));
			return FALSE;
		}
		
		m_tParam.bBigEndian = FALSE;
	}
	else
	{
		m_tParam.bBigEndian = TRUE;
	}

	UPDATE_DATA_T ud = {0};
	memcpy(&ud,lpPos+8,sizeof(ud));
	CONVERT_INT(ud.res_addr,ud.res_addr,m_tParam.bBigEndian);
	CONVERT_INT(ud.dsp_addr,ud.dsp_addr,m_tParam.bBigEndian);

	if(ud.res_addr == 0xFFFFFFFF &&  ud.dsp_addr == 0xFFFFFFFF)
	{
		_tprintf(_T("res_addr and dsp_addr are invalidate."));
		_stprintf(m_szLastErrMsg,_T("res_addr and dsp_addr are invalidate."));
		return FALSE;
	}

	CString strPSFile;
	CString strResFile;
	CString strBzpFile;
	CString strDspFile;
	strPSFile.Format(_T("%s\\ps.bin"),pszDstDir);
	FILE *pFile = _tfopen(strPSFile,_T("wb"));
	if(pFile == NULL)
	{
		_tprintf(_T("cannot create file[%s]."),strPSFile.operator LPCTSTR());
		_stprintf(m_szLastErrMsg,_T("cannot create file[%s]."),strPSFile.operator LPCTSTR());
		return FALSE;
	}

	DWORD dwWrite = 0;
	DWORD dwPsSize = 0 ; 

	dwPsSize = (ud.res_addr != 0xFFFFFFFF) ? ud.res_addr : ud.dsp_addr;
//	dwWrite = fwrite(m_pBzpBuf,1,ud.res_addr,pFile);
	dwWrite = fwrite(m_pBzpBuf,1,dwPsSize,pFile);
	fclose(pFile);
	pFile = NULL;
//	if(dwWrite != ud.res_addr)
	if(dwWrite != dwPsSize)
	{
		_tprintf(_T("write file[%s] failed."),strPSFile.operator LPCTSTR());
		_stprintf(m_szLastErrMsg,_T("write file[%s] failed."),strPSFile.operator LPCTSTR());
		return FALSE;
	}

	DWORD dwResSize = 0;
	DWORD dwDspSize = 0;
	DWORD dwExtSize = 0;
	if (ud.res_addr != 0xFFFFFFFF)
	{
		strResFile.Format(_T("%s\\__res.bin"),pszDstDir);
		pFile = _tfopen(strResFile,_T("wb"));
		if(pFile == NULL)
		{
			_tprintf(_T("cannot create file[%s]."),strResFile.operator LPCTSTR());
			_stprintf(m_szLastErrMsg,_T("cannot create file[%s]."),strResFile.operator LPCTSTR());
			return FALSE;
		}
		
	//	DWORD dwResSize = m_dwBzpSize - ud.res_addr;
	//	DWORD dwDspSize = 0;
		dwResSize = m_dwBzpSize - ud.res_addr;
		dwDspSize = 0;
		if (m_bExtPtnTbl)
		{
			//////////////////////////////////////////////////////////////////////////
		}
		else
		{
			if(ud.dsp_addr != 0xFFFFFFFF)
			{
				dwDspSize = m_dwBzpSize - ud.dsp_addr;
				dwResSize = ud.dsp_addr - ud.res_addr;
			}

		}
		
		
		dwWrite = fwrite(m_pBzpBuf+ud.res_addr,1, dwResSize,pFile);
		fclose(pFile);
		pFile = NULL;
		if(dwWrite !=  dwResSize)
		{
			_tprintf(_T("write file[%s] failed."),strResFile.operator LPCTSTR());
			_stprintf(m_szLastErrMsg,_T("write file[%s] failed."),strResFile.operator LPCTSTR());
			return FALSE;
		}
	}
	else
	{
		if (m_bExtPtnTbl)
		{
			//////////////////////////////////////////////////////////////////////////
		}
		else
		{
			if(ud.dsp_addr != 0xFFFFFFFF)
			{
				dwDspSize = m_dwBzpSize - ud.dsp_addr;
				dwResSize = ud.dsp_addr;
			}
		}
	}
	

	if(dwDspSize != 0)
	{
		strDspFile.Format(_T("%s\\__dsp.bin"),pszDstDir);
		pFile = _tfopen(strDspFile,_T("wb"));
		if(pFile == NULL)
		{
			_tprintf(_T("cannot create file[%s]."),strDspFile.operator LPCTSTR());
			_stprintf(m_szLastErrMsg,_T("cannot create file[%s]."),strDspFile.operator LPCTSTR());
			return FALSE;
		}
		
		dwWrite = fwrite(m_pBzpBuf+ud.dsp_addr,1, dwDspSize,pFile);
		fclose(pFile);
		pFile = NULL;
		if(dwWrite !=  dwDspSize)
		{
			_tprintf(_T("write file[%s] failed."),strDspFile.operator LPCTSTR());
			_stprintf(m_szLastErrMsg,_T("write file[%s] failed."),strDspFile.operator LPCTSTR());
			return FALSE;
		}	
	}
	if (dwExtSize)
	{
	}

	agFile.Add(_T("ps"));
	agFile.Add(strPSFile);

	DWORD dwFlag = *(DWORD*)(m_pBzpBuf + ud.res_addr);
	CONVERT_INT(dwFlag,dwFlag,m_tParam.bBigEndian);

	if(dwFlag == BZP_HEAD_MAGIC || dwFlag == BZP_HEAD_MAGIC2)
	{
		memmove(m_pBzpBuf,m_pBzpBuf+ud.res_addr,dwResSize);
		m_dwBzpSize = dwResSize;
		CStringArray agFile2;
		if(!BzpUncompress(pszDstDir,agFile2,TRUE))
		{
			return FALSE;
		}
		else
		{
			m_tParam.b2ndBzp = TRUE;
			for(int k = 0; k< agFile2.GetSize(); k++)
			{
				agFile.Add(agFile2[k]);
			}
		}
	}
	else
	{
		agFile.Add(_T("res"));
		agFile.Add(strResFile);
	}

	if(dwDspSize != 0)
	{
		dwFlag = *(DWORD*)(m_pBzpBuf + ud.dsp_addr);
		CONVERT_INT(dwFlag,dwFlag,m_tParam.bBigEndian);
		
		if(dwFlag == BZP_HEAD_MAGIC || dwFlag == BZP_HEAD_MAGIC2)
		{
			memmove(m_pBzpBuf,m_pBzpBuf+ud.dsp_addr,dwDspSize);
			m_dwBzpSize = dwDspSize;
			CStringArray agFile2;
			if(!BzpUncompress(pszDstDir,agFile2,FALSE))
			{
				return FALSE;
			}
			else
			{			
				for(int k = 0; k< agFile2.GetSize(); k++)
				{
					agFile.Add(agFile2[k]);
				}
			}
		}
		else
		{
			_tprintf(_T("parse dsp data failed."));
			_stprintf(m_szLastErrMsg,_T("parse dsp data failed."));
			return FALSE;
		}
	}	

	return TRUE;
}
*/
BOOL CBZip::MergeUncompress(LPCTSTR pszDstDir,CStringArray &agFile)
{	

	LPBYTE lpPos= NULL;

	lpPos = GetMergeInfo(TRUE);
	if(lpPos == NULL)
	{
		lpPos = GetMergeInfo(FALSE);
		if(lpPos == NULL)
		{
			_tprintf(_T("cannot find flag in bzp file."));
			_stprintf(m_szLastErrMsg,_T("cannot find flag in bzp file."));
			return FALSE;
		}
		
		m_tParam.bBigEndian = FALSE;
	}
	else
	{
		m_tParam.bBigEndian = TRUE;
	}

	UPDATE_DATA_T ud = {0};
	memcpy(&ud,lpPos+8,sizeof(ud));
	CONVERT_INT(ud.image_addr,ud.image_addr,m_tParam.bBigEndian);
	CONVERT_INT(ud.res_addr,ud.res_addr,m_tParam.bBigEndian);
	CONVERT_INT(ud.dsp_addr,ud.dsp_addr,m_tParam.bBigEndian);

	if(ud.res_addr == 0xFFFFFFFF )//&&  ud.dsp_addr == 0xFFFFFFFF)
	{
		_tprintf(_T("res_addr and dsp_addr are invalidate."));
		_stprintf(m_szLastErrMsg,_T("res_addr and dsp_addr are invalidate."));
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	//实际上如存在image bin时，位于第一分区的image的offset其实是相对于第四个分区（dsp）的offset
	CString strPSFile;
	CString strResFile;
	CString strBzpFile;
	CString strDspFile;
	strPSFile.Format(_T("%s\\ps.bin"),pszDstDir);

	FILE *pFile = _tfopen(strPSFile,_T("wb"));
	if(pFile == NULL)
	{
		_tprintf(_T("cannot create file[%s]."),strPSFile.operator LPCTSTR());
		_stprintf(m_szLastErrMsg,_T("cannot create file[%s]."),strPSFile.operator LPCTSTR());
		return FALSE;
	}
	
	DWORD dwWrite = 0;
	DWORD dwPsSize = 0 ; 

	dwPsSize = ud.res_addr;
	dwWrite = fwrite(m_pBzpBuf,1,dwPsSize,pFile);
	fclose(pFile);
	pFile = NULL;
	//	if(dwWrite != ud.res_addr)
	if(dwWrite != dwPsSize)
	{
		_tprintf(_T("write file[%s] failed."),strPSFile.operator LPCTSTR());
		_stprintf(m_szLastErrMsg,_T("write file[%s] failed."),strPSFile.operator LPCTSTR());
		return FALSE;
	}

	DWORD dwResSize = 0;
	DWORD dwDspSize = 0;
	DWORD dwAfterResSize = 0;
	DWORD dwAfterResAddr = 0;
	DWORD dwPtnTbl[5+MAX_EXT_NUM] = {0};
	CString strAfterResFile;
	int i = 0;
	DWORD dwTblNum = 0;
	memcpy((LPBYTE)&dwPtnTbl,lpPos+8,(5+MAX_EXT_NUM)*sizeof(DWORD));
	dwTblNum = m_bExtPtnTbl ? 5+MAX_EXT_NUM : 5;

	strResFile.Format(_T("%s\\__res.bin"),pszDstDir);
	pFile = _tfopen(strResFile,_T("wb"));
	if(pFile == NULL)
	{
		_tprintf(_T("cannot create file[%s]."),strResFile.operator LPCTSTR());
		_stprintf(m_szLastErrMsg,_T("cannot create file[%s]."),strResFile.operator LPCTSTR());
		return FALSE;
	}
	
	//dwResSize = m_dwBzpSize - ud.res_addr;
	// 0 ----image_addr
	// 1 ---- res_addr
	// 2 ---- nv_addr
	// 3 ---- dsp_addr

	if (ud.image_addr != 0xFFFFFFFF)
	{
		dwAfterResAddr = ud.image_addr;
		dwResSize = dwAfterResAddr - ud.res_addr;
	}
		
	for (i = 2; i < dwTblNum;++i) 
	{
		
		if(dwPtnTbl[i] != 0xFFFFFFFF)
		{
			if (i<4 || (i>=4 && ud.image_addr == 0xFFFFFFFF))
			{
				CONVERT_INT(dwPtnTbl[i],dwPtnTbl[i],m_tParam.bBigEndian);
				dwAfterResAddr = dwPtnTbl[i];
				CONVERT_INT(dwPtnTbl[i],dwPtnTbl[i],m_tParam.bBigEndian); 
				dwResSize = dwAfterResAddr - ud.res_addr;
			}
			
			break;
		}
		
	}

	dwResSize = dwResSize ? dwResSize : m_dwBzpSize - ud.res_addr;
	dwAfterResSize = dwAfterResAddr ? m_dwBzpSize - dwAfterResAddr: 0 ;
	
	dwWrite = fwrite(m_pBzpBuf+ud.res_addr,1, dwResSize,pFile);
	fclose(pFile);
	pFile = NULL;
	if(dwWrite !=  dwResSize)
	{
		_tprintf(_T("write file[%s] failed."),strResFile.operator LPCTSTR());
		_stprintf(m_szLastErrMsg,_T("write file[%s] failed."),strResFile.operator LPCTSTR());
		return FALSE;
	}


	if (dwAfterResSize)
	{
		strAfterResFile.Format(_T("%s\\__splited.bin"),pszDstDir);
		pFile = _tfopen(strAfterResFile.operator LPCTSTR(),_T("wb"));
		if(pFile == NULL)
		{
			_tprintf(_T("cannot create file[%s]."),strAfterResFile.operator LPCTSTR());
			_stprintf(m_szLastErrMsg,_T("cannot create file[%s]."),strAfterResFile.operator LPCTSTR());
			return FALSE;
		}
		
		dwWrite = fwrite(m_pBzpBuf+dwAfterResAddr,1, dwAfterResSize,pFile);
		fclose(pFile);
		pFile = NULL;
		if(dwWrite !=  dwAfterResSize)
		{
			_tprintf(_T("write file[%s] failed."),strAfterResFile.operator LPCTSTR());
			_stprintf(m_szLastErrMsg,_T("write file[%s] failed."),strAfterResFile.operator LPCTSTR());
			return FALSE;
		}	
		
		agFile.Add(_T("-splited"));
		agFile.Add(strAfterResFile);
		agFile.Add(_T("-orglen"));
		CString strTmp;
		strTmp.Format(_T("%d"),m_dwBzpSize -dwAfterResSize );
		agFile.Add(strTmp);
	}


	agFile.Add(_T("ps"));
	agFile.Add(strPSFile);
	
	DWORD dwFlag = *(DWORD*)(m_pBzpBuf + ud.res_addr);
	CONVERT_INT(dwFlag,dwFlag,m_tParam.bBigEndian);
	
	if(dwFlag == BZP_HEAD_MAGIC || dwFlag == BZP_HEAD_MAGIC2)
	{
		memmove(m_pBzpBuf,m_pBzpBuf+ud.res_addr,dwResSize);
		m_dwBzpSize = dwResSize;
		CStringArray agFile2;
		if(!BzpUncompress(pszDstDir,agFile2,TRUE))
		{
			return FALSE;
		}
		else
		{
			m_tParam.b2ndBzp = TRUE;
			for(int k = 0; k< agFile2.GetSize(); k++)
			{
				agFile.Add(agFile2[k]);
			}
		}
	}
	else
	{
		agFile.Add(_T("res"));
		agFile.Add(strResFile);
	}

	
	//////////////////////////////////////////////////////////////////////////

	return TRUE;
}
BOOL CBZip::CompressData(LPBYTE &pDstBuf, DWORD &dwDstSize,LPBYTE pSrcBuf, DWORD dwSrcSize, int nCmpType, int nLevel)
{
	if(nCmpType == BZP_ZIP)
	{
		return Zip(pDstBuf,dwDstSize,pSrcBuf,dwSrcSize,nLevel);
	}
	else if(nCmpType == BZP_7Z || nCmpType == BZP_7ZF || nCmpType == BZP_7Z_B3 )
	{
		return Lzma(pDstBuf,dwDstSize,pSrcBuf,dwSrcSize,nLevel,nCmpType);
		
	}
	else
	{
		dwDstSize = dwSrcSize;
		pDstBuf = new BYTE[dwDstSize];
		memcpy(pDstBuf,pSrcBuf,dwDstSize);
		return TRUE;
	}
}
BOOL CBZip::UnCompressData(LPBYTE &pDstBuf, DWORD &dwDstSize,LPBYTE pSrcBuf, DWORD dwSrcSize,int &nCmpType, int &nLevel)
{
	if(nCmpType == BZP_RAW)
	{
		dwDstSize = dwSrcSize;
		pDstBuf = new BYTE[dwDstSize];
		memcpy(pDstBuf,pSrcBuf,dwDstSize);
		return TRUE;
	}
	else if(pSrcBuf[0] == 0x1F && pSrcBuf[1] == 0x8B)
	{
		nCmpType = BZP_ZIP;
		nLevel   = 9;
		return UnZip(pDstBuf,dwDstSize,pSrcBuf,dwSrcSize);
	}
/*	else if((pSrcBuf[0] == 0x5D || pSrcBuf[0] == 0x67 ) && pSrcBuf[1] == 0x00)
	{
		nCmpType = BZP_7Z;
		nLevel   = 2;
		return UnLzma(pDstBuf,dwDstSize,pSrcBuf,dwSrcSize,FALSE);
	}*/
	else if((pSrcBuf[0] == 0x5D || pSrcBuf[0] == 0x67 ) && pSrcBuf[1] == 0x00) //b3
	{
		nCmpType = BZP_7Z_B3;
		nLevel   = 2;
		return UnLzma(pDstBuf,dwDstSize,pSrcBuf,dwSrcSize,nCmpType);
	}
	else if((pSrcBuf[0] == 0x5A) && pSrcBuf[1] == 0x00)//b
	{
		nCmpType = BZP_7Z;
		nLevel   = 2;
		//return UnLzma_Ex(pDstBuf,dwDstSize,pSrcBuf,dwSrcSize,2);
		return UnLzma(pDstBuf,dwDstSize,pSrcBuf,dwSrcSize,nCmpType);
	}
	else if(pSrcBuf[0] == 0x01 && (pSrcBuf[1] == 0x5D || pSrcBuf[1] == 0x67))
	{
		nCmpType = BZP_7ZF;
		nLevel   = 2;
		return UnLzma(pDstBuf,dwDstSize,pSrcBuf,dwSrcSize,nCmpType);
	}
	else
	{		
		return FALSE;		
	}
}

BOOL CBZip::Zip(LPBYTE &pDstBuf, DWORD &dwDstSize,LPBYTE pSrcBuf, DWORD dwSrcSize,int nLevel)
{
	dwDstSize = compressBound(dwSrcSize)*2;
	pDstBuf = new BYTE[dwDstSize];
	memset(pDstBuf,0,dwDstSize);
	//int nLevel = m_tParam.nLevel;

	if(nLevel<1 || nLevel > 9)
	{
		nLevel = 9;
	}

	if(compress2(pDstBuf, &dwDstSize,pSrcBuf,dwSrcSize,nLevel) == Z_OK)
	{
		return TRUE;
	}
	else
	{
		delete [] pDstBuf;
		pDstBuf = NULL;
		dwDstSize = 0;
		return FALSE;
	}
}

BOOL CBZip::UnZip(LPBYTE &pDstBuf, DWORD &dwDstSize,LPBYTE pSrcBuf, DWORD dwSrcSize)
{
 	dwDstSize = m_tParam.dwMaxRam;
 	pDstBuf   = new BYTE[dwDstSize];
	memset(pDstBuf,0,dwDstSize);
	if(uncompress(pDstBuf, &dwDstSize,pSrcBuf,dwSrcSize) == Z_OK)
	{
		return TRUE;
	}
	else
	{
// #ifdef _DEBUG
// 		FILE *pFile = _tfopen(_T("D:\\1.bin"),_T("wb"));
// 		fwrite(pSrcBuf,1,dwSrcSize,pFile);
// 		fclose(pFile);
// #endif
 		delete [] pDstBuf;
 		pDstBuf = NULL;
 		dwDstSize = 0;
		return FALSE;
	}
}

BOOL CBZip::Lzma(LPBYTE &pDstBuf, DWORD &dwDstSize,LPBYTE pSrcBuf, DWORD dwSrcSize,int nLevel,int nCmpType)
{
	dwDstSize = dwSrcSize*2;
	pDstBuf = new BYTE[dwDstSize+LZMA_PROPS_SIZE+8];
	memset(pDstBuf,0,dwDstSize+LZMA_PROPS_SIZE+8);
	//int nLevel = m_tParam.nLevel;
	if(nLevel<0 || nLevel > 9)
	{
		nLevel = 2;
	}
/*
	UINT   outPropsSize = LZMA_PROPS_SIZE;
	LPBYTE outPropps = pDstBuf;
	*(DWORD*)(pDstBuf + LZMA_PROPS_SIZE) = dwSrcSize;
	if( LzmaCompress(pDstBuf+LZMA_PROPS_SIZE+8,(UINT*)&dwDstSize,pSrcBuf,dwSrcSize,outPropps,&outPropsSize,
		             nLevel,1<<24,3,0,2,32,1) == SZ_OK)
	{
*/

	SRes nRet = 0;

	if(nCmpType == BZP_7Z)
	{
		nRet = LzmaCompress2(pDstBuf,(UINT*)&dwDstSize,pSrcBuf,dwSrcSize,nLevel,TRUE);		
	}
	else if(nCmpType == BZP_7ZF)
	{
		nRet = LzmaCompress3(pDstBuf,(UINT*)&dwDstSize,pSrcBuf,dwSrcSize,nLevel);
	}
	else if(nCmpType == BZP_7Z_B3)
	{
		nRet = LzmaCompress2(pDstBuf,(UINT*)&dwDstSize,pSrcBuf,dwSrcSize,nLevel,FALSE);
	}
	else
	{
		return FALSE;
	}	

	if( nRet == SZ_OK)
	{
#if 0
#ifdef _DEBUG
		FILE *pFile = _tfopen(_T("E:\\7z\\file1.lzma"),_T("wb"));
		fwrite(pDstBuf,1,dwDstSize,pFile);
		fclose(pFile);
		LPBYTE pDstBuf2 = NULL;
		DWORD  dwDstSize2 = 0;
		LPBYTE p = new BYTE[dwDstSize+100];
		memcpy(p,pDstBuf,dwDstSize);
		UnLzma(pDstBuf2,dwDstSize2,p,dwDstSize+100,bFilter);
		pFile = _tfopen(_T("E:\\7z\\file1.lzma2"),_T("wb"));
		fwrite(pDstBuf2,1,dwDstSize2,pFile);
		fclose(pFile);
#endif
#endif
		return TRUE;
	}
	else
	{
		delete [] pDstBuf;
		pDstBuf = NULL;
		dwDstSize = 0;
		return FALSE;
	}
}

BOOL CBZip::UnLzma(LPBYTE &pDstBuf, DWORD &dwDstSize,LPBYTE pSrcBuf, DWORD dwSrcSize,int nCmpType)
{
	if(nCmpType == BZP_7ZF) // filter
	{
		dwDstSize = *(DWORD*)(pSrcBuf+LZMA_PROPS_SIZE + 1);
	}
	else
	{
		dwDstSize = *(DWORD*)(pSrcBuf+LZMA_PROPS_SIZE);
	}
	
	//dwDstSize = m_tParam.dwMaxRam;
	pDstBuf = new BYTE[dwDstSize];
	memset(pDstBuf,0,dwDstSize);
/*	UINT outPropsSize = LZMA_PROPS_SIZE;
	BYTE outPropps[LZMA_PROPS_SIZE] = {0};
	DWORD dwSrcSizeTmp = dwSrcSize-LZMA_PROPS_SIZE-8;
	LPBYTE pSrcBufTmp = pSrcBuf+LZMA_PROPS_SIZE+8;
	memcpy(outPropps,pSrcBuf,LZMA_PROPS_SIZE);
	if( LzmaUncompress(pDstBuf,(UINT*)&dwDstSize,pSrcBufTmp,(UINT*)&dwSrcSizeTmp,outPropps,outPropsSize) == SZ_OK)
	{
*/
	SRes nRet = 0;

	if(nCmpType == BZP_7Z)
	{
		nRet = LzmaUncompress2(pDstBuf,(UINT*)&dwDstSize,pSrcBuf,dwSrcSize,TRUE);	
	}
	else if(nCmpType == BZP_7ZF)
	{
		nRet = LzmaUncompress3(pDstBuf,(UINT*)&dwDstSize,pSrcBuf,dwSrcSize);
	}
	else if(nCmpType == BZP_7Z_B3)
	{
		nRet = LzmaUncompress2(pDstBuf,(UINT*)&dwDstSize,pSrcBuf,dwSrcSize,FALSE);
	}
	else
	{
		return FALSE;
	}

	if( nRet == SZ_OK)
	{
		return TRUE;
	}
	else
	{
		delete [] pDstBuf;
		pDstBuf = NULL;
		dwDstSize = 0;
		return FALSE;
	}
}


BOOL CBZip::GetOrgData(LPCTSTR lpszFile,LPBYTE &pOrgBuf, DWORD &dwOrgSize)
{
	pOrgBuf = NULL;
	dwOrgSize = 0;
	FILE *pFile = NULL;
	pFile = _tfopen(lpszFile,_T("rb"));
	if(pFile == NULL)
	{
		_tprintf(_T("open file [%s] failed."),lpszFile);
		_stprintf(m_szLastErrMsg,_T("open file [%s] failed."),lpszFile);
		return FALSE;
	}

	fseek(pFile,0,SEEK_END);

	dwOrgSize = ftell(pFile);
	if(dwOrgSize == 0)
	{
		fclose(pFile);
		pFile = NULL;
		_tprintf(_T("file [%s] is empty."),lpszFile);
		_stprintf(m_szLastErrMsg,_T("file [%s] is empty."),lpszFile);
		return FALSE;
	}

	fseek(pFile,0,SEEK_SET);

	pOrgBuf = new BYTE[dwOrgSize];
	DWORD dwRead = fread(pOrgBuf,1,dwOrgSize,pFile);
	if(dwRead != dwOrgSize)
	{
		fclose(pFile);
		pFile = NULL;
		_tprintf(_T("read file [%s] failed."),lpszFile);
		_stprintf(m_szLastErrMsg,_T("read file [%s] failed."),lpszFile);
		SAFE_DEL(pOrgBuf);
		return FALSE;
	}

	fclose(pFile);
	pFile = NULL;

	return TRUE;
}

BOOL CBZip::GetCmpData(LPCTSTR lpszFile,DWORD &dwOrgSize,LPBYTE &pCmpBuf, DWORD &dwCmpSize, DWORD &dwLangID)
{
	dwOrgSize = 0;
	dwCmpSize = 0;
	pCmpBuf = NULL;

	LPBYTE pOrgBuf = NULL;

	if(!GetOrgData(lpszFile,pOrgBuf,dwOrgSize))
	{
		return FALSE;
	}

	if(dwLangID == 0x10000000)
	{
		if(dwOrgSize < 16)
		{
			_tprintf(_T("language theme file [%s] is too small."),lpszFile);
			return FALSE;
		}
		if(pOrgBuf[0]=='t' && pOrgBuf[1]=='r' && pOrgBuf[2] == 'e' && pOrgBuf[3] == 's' && ((pOrgBuf[14] & 0x2) == 0x2))
		{
			dwLangID = pOrgBuf[15];
		}
		else
		{
			_tprintf(_T("language theme file [%s] is invalid."),lpszFile);
			return FALSE;
		}
	}

	if(!CompressData(pCmpBuf, dwCmpSize,pOrgBuf,dwOrgSize,m_tParam.nCmpType,m_tParam.nLevel))
	{
		_tprintf(_T("compress code file [%s] failed."),lpszFile);
		_stprintf(m_szLastErrMsg,_T("compress code file [%s] failed."),lpszFile);
		SAFE_DEL(pOrgBuf);
		SAFE_DEL(pCmpBuf);
		return FALSE;
	}
	SAFE_DEL(pOrgBuf);


	return TRUE;
}
BOOL CBZip::GetCmpDataExt(LPCTSTR lpszFile,DWORD &dwOrgSize,LPBYTE &pCmpBuf, DWORD &dwCmpSize, DWORD &dwLangID)
{
	dwOrgSize = 0;
	dwCmpSize = 0;
	pCmpBuf = NULL;
	
	LPBYTE pOrgBuf = NULL;
	
	if(!GetOrgData(lpszFile,pOrgBuf,dwOrgSize))
	{
		return FALSE;
	}
	
	if(dwLangID == 0x10000000)
	{
		if(dwOrgSize < 16)
		{
			_tprintf(_T("language theme file [%s] is too small."),lpszFile);
			SAFE_DEL(pOrgBuf);
			return FALSE;
		}
		if(pOrgBuf[0]=='t' && pOrgBuf[1]=='r' && pOrgBuf[2] == 'e' && pOrgBuf[3] == 's' && ((pOrgBuf[14] & 0x2) == 0x2))
		{
			dwLangID = pOrgBuf[15];
		}
		else
		{
			_tprintf(_T("language theme file [%s] is invalid."),lpszFile);
			SAFE_DEL(pOrgBuf);
			return FALSE;
		}
	}
	
	if(!CompressData(pCmpBuf, dwCmpSize,pOrgBuf,dwOrgSize,BZP_RAW,m_tParam.nLevel))
	{
		_tprintf(_T("compress code file [%s] failed."),lpszFile);
		_stprintf(m_szLastErrMsg,_T("compress code file [%s] failed."),lpszFile);
		SAFE_DEL(pOrgBuf);
		SAFE_DEL(pCmpBuf);
		return FALSE;
	}
	SAFE_DEL(pOrgBuf);
	
	
	return TRUE;
}

void CBZip::InitParam()
{
	m_tParam.dwMaxRam = DEFAULT_RAM_SIZE;
	m_tParam.dwMaxBzp = DEFAULT_BZP_SIZE;
	m_tParam.bBigEndian = TRUE;
	m_tParam.bCat = FALSE;
	m_tParam.nLevel = 9;
	m_tParam.nCmpType = BZP_ZIP;
	m_tParam.b2ndBzp = FALSE;
	m_tParam.bCat2 = FALSE;
	m_tParam.nResCmpType = BZP_ZIP;
	m_tParam.nUsrCmpType = BZP_ZIP;
	m_tParam.nUsrLevel = 9;
	m_tParam.nResLevel = 9;
	m_tParam.dwUsrPacSize = 4*1024;
	m_tParam.dwResPacSize = 4*1024;
}

BOOL CBZip::SplitCmpFile(LPCTSTR lpszFile, DWORD &dwSrcSize,LPBYTE &pDstBuf, DWORD &dwDstSize, DWORD dwMaxPacSize,int nCmpType, int nLevel)
{
	dwSrcSize = 0;
	LPBYTE pSrcBuf = NULL;

	if(!GetOrgData(lpszFile,pSrcBuf,dwSrcSize))
	{
		return FALSE;
	}

	if(dwMaxPacSize == 0)
	{
		dwMaxPacSize = 4*1024;
	}

	SCI_BZP_FILE_HEAD_T header = {0};
	int nPacNum = (dwSrcSize + dwMaxPacSize - 1) / dwMaxPacSize;
	DWORD dwMaxSize = dwSrcSize + nPacNum*4 + sizeof(header);

	header.magic = BZP_PAC_MAGIC;
	header.block_num = nPacNum;
	
	LPBYTE pDstCur  = pDstBuf;
	LPBYTE pSrcCur  = pSrcBuf;

	pDstBuf = new BYTE[dwMaxSize];
	pDstCur = pDstBuf + sizeof(header);

	DWORD * pOffset = new DWORD[nPacNum];
	memset(pOffset,0, sizeof(DWORD)*nPacNum);

	DWORD dwLeft = dwSrcSize;
	DWORD dwPacSize = dwMaxPacSize; 
	DWORD dwOffset = sizeof(header);	
	for(int i= 0; i< nPacNum; i++)
	{
		if(dwLeft >= dwMaxPacSize)
		{
			dwPacSize = dwMaxPacSize;
		}
		else
		{
			dwPacSize = dwLeft;
		}
		dwLeft -= dwPacSize;

		LPBYTE pTmp = NULL;
		DWORD  dwTmp = 0;
		if(!CompressData(pTmp,dwTmp,pSrcCur,dwPacSize,nCmpType,nLevel))
		{
			_tprintf(_T("compress code file [%s] failed."),lpszFile);
			_stprintf(m_szLastErrMsg,_T("compress code file [%s] failed."),lpszFile);
			SAFE_DEL(pSrcBuf);
			SAFE_DEL(pDstBuf);
			SAFE_DEL(pOffset);
			SAFE_DEL(pTmp);
			return FALSE;
		}

		pOffset[i] = dwOffset;

		dwOffset += (( dwTmp + 3) & 0xFFFFFFFC);

		memcpy(pDstCur,pTmp,dwTmp);

		pDstCur += (( dwTmp + 3) & 0xFFFFFFFC);

		pSrcCur += dwPacSize;
		SAFE_DEL(pTmp);
	}

	header.block_head_pos = dwOffset;


	for(int j=0; j< nPacNum; j++)
	{
		CONVERT_INT(pOffset[j],pOffset[j],m_tParam.bBigEndian);
	}

	memcpy(pDstCur, pOffset,sizeof(DWORD)*nPacNum);

    dwDstSize = dwOffset + sizeof(DWORD)*nPacNum;

	ConvertBZPFileHead(header,m_tParam.bBigEndian);

	memcpy(pDstBuf,&header,sizeof(header));
	
	SAFE_DEL(pOffset);
	SAFE_DEL(pSrcBuf);

	return TRUE;
}

BOOL CBZip::UnSplitCmpFile(LPCTSTR lpszFile, LPBYTE pBuf,DWORD dwSize,int &nCmpType, int &nLevel)
{
	FILE *pFile = _tfopen(lpszFile,_T("wb"));	
	if(NULL == pFile)
	{
		return FALSE;
	}

	SCI_BZP_FILE_HEAD_T header = {0};

	memcpy(&header,pBuf,sizeof(header));

	ConvertBZPFileHead(header,m_tParam.bBigEndian);

	DWORD *pOffset =  (DWORD *)(pBuf + header.block_head_pos);

	for(UINT j=0; j< header.block_num; j++)
	{
		DWORD dwOffset = pOffset[j];
		CONVERT_INT(dwOffset,dwOffset,m_tParam.bBigEndian);

		LPBYTE pSrcBuf = pBuf + dwOffset;
		DWORD  dwSrcSize = 0;
		if(j != (header.block_num -1))
		{
			DWORD dwOffset2 = pOffset[j+1];
			CONVERT_INT(dwOffset2,dwOffset2,m_tParam.bBigEndian);
			dwSrcSize = dwOffset2 - dwOffset;
		}
		else
		{
			dwSrcSize = header.block_head_pos - dwOffset;
		}

		LPBYTE pDstBuf = NULL;
		DWORD  dwDstSize = 0;

		_ASSERT(pSrcBuf < pBuf + dwSize);

		if(!UnCompressData(pDstBuf,dwDstSize,pSrcBuf,dwSrcSize,nCmpType,nLevel))
		{
			_tprintf(_T("uncompress failed."));
			_stprintf(m_szLastErrMsg,_T("uncompress failed."));		
			SAFE_DEL(pDstBuf);
			return FALSE;
		}

		fwrite(pDstBuf,1,dwDstSize,pFile);

		SAFE_DEL(pDstBuf);
	}

	fclose(pFile);
	return TRUE;
}

BOOL CBZip::Merge2(CStringArray &agFile, LPCTSTR pszXFile)
{
	if((agFile.GetSize()%2 != 0)  || pszXFile == NULL)
	{
		_tprintf(_T("Merge2: invalid param."));
		_stprintf(m_szLastErrMsg,_T("Merge2: invalid param."));
		return FALSE;
	}

	CString strUsrFile;
	CString strResFile;

	int nFileCount = agFile.GetSize()/2;
	int nFileNum=0;

	for(int i = 0;i< nFileCount; i++)
	{
		if(agFile[2*i].CompareNoCase(_T("usr")) == 0)
		{
			strUsrFile = agFile[2*i+1];
			nFileNum++;
		}
		else if(agFile[2*i].CompareNoCase(_T("res")) == 0)
		{			
			strResFile = agFile[2*i+1];
			nFileNum++;
		}
	}

	if(strUsrFile.IsEmpty() && strResFile.IsEmpty())
	{
		_tprintf(_T("Merge2: file name is empty."));
		_stprintf(m_szLastErrMsg,_T("Merge2: file name is empty."));
		return FALSE;
	}

	LPBYTE pUsrBuf = NULL;
	DWORD  dwUsrSize = 0;
	LPBYTE pResBuf = NULL;
	DWORD  dwResSize  = 0;
	DWORD  dwOrgUsrSize = 0;
	DWORD  dwOrgResSize = 0;


	FILE *pFile = NULL;
	DWORD dwPad = 0;
	BYTE pad[4] = {0xFF,0xFF,0xFF,0xFF};
	DWORD dwWrite = 0;
	CFileFind finder;

	SCI_BZP_FILE_HEAD_T  fileHead		= {0};
	SCI_BZP_BLOCK_HEAD_T blockHeadUsr	= {0};
	SCI_BZP_BLOCK_HEAD_T blockHeadRes	= {0};

	//////////////////////////////////////////////////////////////////////////
	// deal with usr file
	if(!strUsrFile.IsEmpty())
	{
		if(!SplitCmpFile(strUsrFile,dwOrgUsrSize,pUsrBuf,dwUsrSize,m_tParam.dwUsrPacSize,m_tParam.nUsrCmpType,m_tParam.nUsrLevel))
		{
			goto MERGE2_ERROR;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// deal with res file
	if(!strResFile.IsEmpty())
	{
		if(!SplitCmpFile(strResFile,dwOrgResSize,pResBuf,dwResSize,m_tParam.dwResPacSize,m_tParam.nResCmpType,m_tParam.nResLevel))
		{
			goto MERGE2_ERROR;
		}
	}


	if(finder.FindFile(pszXFile))
	{
		DWORD dwAtt = ::GetFileAttributes(pszXFile);
		dwAtt &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(pszXFile,dwAtt);
	}

	pFile = _tfopen(pszXFile,_T("wb"));
	if(pFile == NULL)
	{
		_tprintf(_T("cannot create file [%s]."),pszXFile);
		_stprintf(m_szLastErrMsg,_T("cannot create file [%s]."),pszXFile);
		goto MERGE2_ERROR;
	}

#define MERGE2_WRITE_FILE(p,t) do{\
	dwWrite = fwrite(p,1,(t),pFile);\
	if(dwWrite != (t))\
	{\
		_tprintf(_T("write file [%s] failed."),pszXFile);\
		_stprintf(m_szLastErrMsg,_T("write file [%s] failed."),pszXFile);\
		goto MERGE2_ERROR;\
	}\
	}while(0)


	fileHead.block_head_pos = ((dwUsrSize + 3)/4) * 4 + ((dwResSize + 3)/4) * 4 + sizeof(SCI_BZP_FILE_HEAD_T);
	fileHead.block_num = nFileNum;
	fileHead.version = 0;
	fileHead.magic = BZP_HEAD_MAGIC;

	ConvertBZPFileHead(fileHead,m_tParam.bBigEndian);
	MERGE2_WRITE_FILE(&fileHead,sizeof(fileHead));

	if(pUsrBuf != NULL)
	{
		MERGE2_WRITE_FILE(pUsrBuf,dwUsrSize);
		dwPad = ((dwUsrSize + 3)/4) * 4  - dwUsrSize;
		if(dwPad > 0)
		{
			MERGE2_WRITE_FILE(pad,dwPad);
		}
	}

	if(pResBuf != NULL)
	{		
		MERGE2_WRITE_FILE(pResBuf,dwResSize);
		dwPad = ((dwResSize + 3)/4) * 4  - dwResSize;
		if(dwPad > 0)
		{
			MERGE2_WRITE_FILE(pad,dwPad);
		}	
	}	
	
	if(pUsrBuf != NULL)
	{
		blockHeadUsr.des_addr = m_tParam.dwUsrPacSize;
		blockHeadUsr.block_len = dwUsrSize;
		blockHeadUsr.magic = BZP_BLOCK_MAGIC;
		blockHeadUsr.type = BZP_BLOCK_USR;
		blockHeadUsr.start_pos = sizeof(fileHead);	
		
		ConvertBZPBlockHead(blockHeadUsr,m_tParam.bBigEndian);
		
		MERGE2_WRITE_FILE(&blockHeadUsr,sizeof(blockHeadUsr));
	}

	if(pResBuf != NULL)
	{
		blockHeadRes.des_addr = m_tParam.dwResPacSize;
		blockHeadRes.block_len = dwResSize;
		blockHeadRes.magic = BZP_BLOCK_MAGIC;
		blockHeadRes.type = BZP_BLOCK_RES;
		blockHeadRes.start_pos = nFileNum == 1 ? sizeof(fileHead) : (((dwUsrSize + 3)/4) * 4 + sizeof(fileHead) );
		ConvertBZPBlockHead(blockHeadRes,m_tParam.bBigEndian);
		
		MERGE2_WRITE_FILE(&blockHeadRes,sizeof(blockHeadRes));
	}



	fclose(pFile);
	pFile = NULL;

	SAFE_DEL(pUsrBuf);
	SAFE_DEL(pResBuf);

	return TRUE;

MERGE2_ERROR:
	if(pFile != NULL)
	{
		fclose(pFile);
	}
	SAFE_DEL(pUsrBuf);
	SAFE_DEL(pResBuf);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////
//@polo
//////////////////////////////////////////////////////////////////////////
BOOL CBZip::SplitDspZ2File(LPCTSTR pszSrcFile,LPCTSTR lpTmpDspZfile,LPCTSTR lpOtherfile)
{	
	LPBYTE pBufFull = NULL;
	DWORD  dwFullSize = 0;
	if(!GetOrgData(pszSrcFile,pBufFull,dwFullSize))
		return FALSE;

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	DWORD dwEnd = dwFullSize < MAX_SEARCH_SIZE? dwFullSize : MAX_SEARCH_SIZE;
	LPBYTE lpCur = pBufFull;
	LPBYTE lpEnd = pBufFull+dwEnd;
	LPBYTE lpDataEnd = pBufFull + dwFullSize;
	LPBYTE lpPos= NULL;
	BYTE head[8] = {0};
	BYTE tail[4] = {0};
	DWORD dwMagic = 0;
	InitParam();//Default
	dwMagic = PARTITION_MAGIC_FIRST;
	CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
	*(DWORD*)head = dwMagic;

	dwMagic = PARTITION_MAGIC_SECOND;
	CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
	*(DWORD*)(head+4) = dwMagic;

	dwMagic = PARTITION_MAGIC_END;
	CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
	*(DWORD*)tail = dwMagic;

	BOOL bFind = FALSE;
	while(lpCur < lpEnd)
	{
		// find head 
		lpPos = std::search(lpCur,lpEnd, head,head+7);
		if(lpPos < lpEnd)
		{
			if((lpPos + 8+sizeof(UPDATE_DATA_T) +4) < lpDataEnd)
			{
				// check tail
				if(
					memcmp(lpPos + 8 + sizeof(UPDATE_DATA_T), tail,4) ==  0 ||
					memcmp(lpPos + 8+ sizeof(UPDATE_DATA_T) +MAX_EXT_NUM*sizeof(DWORD), tail,4) == 0 
				  )
				{
					bFind = TRUE;
					break;
				}
				else
				{
					lpCur += 8;
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	if(!bFind)
	{
		/*_tprintf(_T("cannot find flag in bzp file."));
		_stprintf(m_szLastErrMsg,_T("cannot find flag in bzp file."));
		return FALSE;*/
		lpCur = pBufFull;
		lpEnd = pBufFull+dwEnd;
		lpDataEnd = pBufFull + dwFullSize;
		lpPos= NULL;
		
		dwMagic = 0;
		m_tParam.bBigEndian = FALSE;	//CHANGED
		dwMagic = PARTITION_MAGIC_FIRST;
		CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
		*(DWORD*)head = dwMagic;
		
		dwMagic = PARTITION_MAGIC_SECOND;
		CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
		*(DWORD*)(head+4) = dwMagic;
		
		dwMagic = PARTITION_MAGIC_END;
		CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
		*(DWORD*)tail = dwMagic;
		
		BOOL bFind = FALSE;
		while(lpCur < lpEnd)
		{
			// find head 
			lpPos = std::search(lpCur,lpEnd, head,head+7);
			if(lpPos < lpEnd)
			{
				if((lpPos + 8+sizeof(UPDATE_DATA_T) +4) < lpDataEnd)
				{
					// check tail
					if(
						memcmp(lpPos + 8 + sizeof(UPDATE_DATA_T), tail,4) ==  0  ||
						memcmp(lpPos + 8+ sizeof(UPDATE_DATA_T) +MAX_EXT_NUM*sizeof(DWORD), tail,4) == 0 
					   )
					{
						bFind = TRUE;
						break;
					}
					else
					{
						lpCur += 8;
					}
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}


		if(!bFind)
		{
			_tprintf(_T("cannot find flag in bzp file."));
			_stprintf(m_szLastErrMsg,_T("cannot find flag in bzp file."));
			return FALSE;
		}
	}

	UPDATE_DATA_T ud = {0};
	memcpy(&ud,lpPos+8,sizeof(ud));
	CONVERT_INT(ud.res_addr,ud.res_addr,m_tParam.bBigEndian);
	CONVERT_INT(ud.dsp_addr,ud.dsp_addr,m_tParam.bBigEndian);
	//////////other///////
	FILE *pFile = _tfopen(lpOtherfile,_T("wb"));
	if(pFile == NULL)
	{
		_tprintf(_T("cannot create file[%s]."),lpOtherfile);
		_stprintf(m_szLastErrMsg,_T("cannot create file[%s]."),lpOtherfile);
		return FALSE;
	}
	
	DWORD dwWrite = 0;
	DWORD dwOtherSize = 0;
	dwOtherSize = (ud.dsp_addr == 0xFFFFFFFF) ? dwFullSize : ud.dsp_addr;
	dwWrite = fwrite(pBufFull,1,dwOtherSize,pFile);
	fclose(pFile);
	pFile = NULL;
	if(dwWrite != dwOtherSize)
	{
		_tprintf(_T("write file[%s] failed."),lpOtherfile);
		_stprintf(m_szLastErrMsg,_T("write file[%s] failed."),lpOtherfile);
		return FALSE;
	}

	////////dspz//////////
	if(ud.dsp_addr != 0xFFFFFFFF)
	{
		pFile = _tfopen(lpTmpDspZfile,_T("wb"));
		if(pFile == NULL)
		{
			_tprintf(_T("cannot create file[%s]."),lpTmpDspZfile);
			_stprintf(m_szLastErrMsg,_T("cannot create file[%s]."),lpTmpDspZfile);
			return FALSE;
		}
		
		dwWrite = 0;
		dwWrite = fwrite(pBufFull + ud.dsp_addr,1,dwFullSize -ud.dsp_addr ,pFile);
		fclose(pFile);
		pFile = NULL;
		if(dwWrite != dwFullSize -ud.dsp_addr)
		{
			_tprintf(_T("write file[%s] failed."),lpTmpDspZfile);
			_stprintf(m_szLastErrMsg,_T("write file[%s] failed."),lpTmpDspZfile);
			return FALSE;
		}
	}

	SAFE_DEL(pBufFull);
	return TRUE;
}

void CBZip::GetLimitParam(BZP_PARAM_T &param)
{
	param = m_tParam;
}

//////////////////////////////////////////////////////////////////////////
BOOL CBZip::RemovePartiton(LPCTSTR pszSrcFile,LPCTSTR lpDestFile,int nTblIndex)
{	
	LPBYTE pBufSrc = NULL;
	DWORD  dwSrcSize = 0;
	if(!GetOrgData(pszSrcFile,pBufSrc,dwSrcSize))
		return FALSE;

	//////////////////////////////////////////////////////////////////////////
	DWORD dwEnd = dwSrcSize < MAX_SEARCH_SIZE? dwSrcSize : MAX_SEARCH_SIZE;
	LPBYTE lpCur = pBufSrc;
	LPBYTE lpEnd = pBufSrc+dwEnd;
	LPBYTE lpDataEnd = pBufSrc + dwSrcSize;
	LPBYTE lpPos= NULL;
	BYTE head[8] = {0};
	BYTE tail[4] = {0};
	DWORD dwMagic = 0;
	DWORD dwWrite = 0;
	DWORD dwDestSize = 0;
	UPDATE_DATA_T ud = {0};
	FILE *pFile = NULL;
	DWORD dwRmvPtnSize = 0 ;
	DWORD dwPtnTbl[5+MAX_EXT_NUM] = {0};

	InitParam();//First using Default
	dwMagic = PARTITION_MAGIC_FIRST;
	CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
	*(DWORD*)head = dwMagic;

	dwMagic = PARTITION_MAGIC_SECOND;
	CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
	*(DWORD*)(head+4) = dwMagic;

	dwMagic = PARTITION_MAGIC_END;
	CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
	*(DWORD*)tail = dwMagic;

	BOOL bFind = FALSE;
	E_PS_TYPE e_pstype = E_UNFINDE;
	int i = 0;
	while(lpCur < lpEnd)
	{
		// find head 
		lpPos = std::search(lpCur,lpEnd, head,head+7);
		if(lpPos < lpEnd)
		{
			if((lpPos + 8+sizeof(UPDATE_DATA_T) +4) < lpDataEnd)
			{
				// check tail
				if( memcmp(lpPos + 8 + sizeof(UPDATE_DATA_T), tail,4) ==  0	)
				{
					e_pstype = E_5_PATITION;
					break;
				}
				else if(memcmp(lpPos + 8+ sizeof(UPDATE_DATA_T) +MAX_EXT_NUM*sizeof(DWORD), tail,4) == 0 )
				{
					e_pstype = E_29_PATITION;
					break;
				}
				else
				{
					lpCur += 8;
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	//if(!bFind)
	if (E_UNFINDE == e_pstype )
	{
		lpCur = pBufSrc;
		lpEnd = pBufSrc+dwEnd;
		lpDataEnd = pBufSrc + dwSrcSize;
		lpPos= NULL;
		
		dwMagic = 0;
		m_tParam.bBigEndian = FALSE;	//Changed to littleEndian
		dwMagic = PARTITION_MAGIC_FIRST;
		CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
		*(DWORD*)head = dwMagic;
		
		dwMagic = PARTITION_MAGIC_SECOND;
		CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
		*(DWORD*)(head+4) = dwMagic;
		
		dwMagic = PARTITION_MAGIC_END;
		CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
		*(DWORD*)tail = dwMagic;
		
		bFind = FALSE;
		while(lpCur < lpEnd)
		{
			// find head 
			lpPos = std::search(lpCur,lpEnd, head,head+7);
			if(lpPos < lpEnd)
			{
				if((lpPos + 8+sizeof(UPDATE_DATA_T) +4) < lpDataEnd)
				{
					// check tail
					if( memcmp(lpPos + 8 + sizeof(UPDATE_DATA_T), tail,4) ==  0	)
					{
						e_pstype = E_5_PATITION;
						break;
					}
					else if(memcmp(lpPos + 8+ sizeof(UPDATE_DATA_T) +MAX_EXT_NUM*sizeof(DWORD), tail,4) == 0 )
					{
						e_pstype = E_29_PATITION;
						break;
					}
					else
					{
						lpCur += 8;
					}
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}


		//if(!bFind)
		if (E_UNFINDE == e_pstype )
		{
			_tprintf(_T("cannot find flag in ps file."));
			_stprintf(m_szLastErrMsg,_T("cannot find flag in ps file."));
			goto REMOVE_ERROR;
		}
	}

	pFile = _tfopen(lpDestFile,_T("wb"));
	if(pFile == NULL)
	{
		_tprintf(_T("cannot create file[%s]."),lpDestFile);
		_stprintf(m_szLastErrMsg,_T("cannot create file[%s]."),lpDestFile);
		goto REMOVE_ERROR;
	}

	switch (e_pstype)
	{
	case E_5_PATITION:
		if (nTblIndex >= 4)
		{
			_tprintf(_T("inputed nTblIndex > partition tables number."));
			_stprintf(m_szLastErrMsg,_T("inputed nTblIndex > partition tables number."));
			goto REMOVE_ERROR;
		}
		memcpy((LPBYTE)&dwPtnTbl,lpPos+8,5*sizeof(DWORD));
		if (dwPtnTbl[nTblIndex] != 0xFFFFFFFF)
		{
			CONVERT_INT(dwPtnTbl[nTblIndex],dwPtnTbl[nTblIndex],m_tParam.bBigEndian);
			
			for (i = nTblIndex+1; i < 5;++i) 
			{
				if(dwPtnTbl[i] != 0xFFFFFFFF)
				{
					CONVERT_INT(dwPtnTbl[i],dwPtnTbl[i],m_tParam.bBigEndian);
					dwRmvPtnSize = dwPtnTbl[i] - dwPtnTbl[nTblIndex];
					CONVERT_INT(dwPtnTbl[i],dwPtnTbl[i],m_tParam.bBigEndian);
					break;
				}
				
			}
			if (0 == dwRmvPtnSize)
			{
				dwRmvPtnSize = dwSrcSize - dwPtnTbl[nTblIndex];
			}
			
			for (i = nTblIndex+1; i < 5;++i) 
			{
				if(dwPtnTbl[i] != 0xFFFFFFFF)
				{
					CONVERT_INT(dwPtnTbl[i],dwPtnTbl[i],m_tParam.bBigEndian);
					dwPtnTbl[i] = dwPtnTbl[i] - dwRmvPtnSize;
					CONVERT_INT(dwPtnTbl[i],dwPtnTbl[i],m_tParam.bBigEndian);
				}
				
			}

			memcpy(lpPos+8,&dwPtnTbl,5*sizeof(DWORD));	
		}
		break;
	case E_29_PATITION:
		memcpy((LPBYTE)&dwPtnTbl,lpPos+8,(5+MAX_EXT_NUM)*sizeof(DWORD));	
		if (dwPtnTbl[nTblIndex] != 0xFFFFFFFF)
		{
			CONVERT_INT(dwPtnTbl[nTblIndex],dwPtnTbl[nTblIndex],m_tParam.bBigEndian);
			
			for (i = nTblIndex+1; i < 5+MAX_EXT_NUM;++i) 
			{
				if(dwPtnTbl[i] != 0xFFFFFFFF)
				{
					CONVERT_INT(dwPtnTbl[i],dwPtnTbl[i],m_tParam.bBigEndian);
					dwRmvPtnSize = dwPtnTbl[i] - dwPtnTbl[nTblIndex];
					CONVERT_INT(dwPtnTbl[i],dwPtnTbl[i],m_tParam.bBigEndian);
					break;
				}
				
			}
			if (0 == dwRmvPtnSize)
			{
				dwRmvPtnSize = dwSrcSize - dwPtnTbl[nTblIndex];
			}
			
			for (i = nTblIndex+1; i < 5+MAX_EXT_NUM;++i) 
			{
				if(dwPtnTbl[i] != 0xFFFFFFFF)
				{
					CONVERT_INT(dwPtnTbl[i],dwPtnTbl[i],m_tParam.bBigEndian);
					dwPtnTbl[i] = dwPtnTbl[i] - dwRmvPtnSize;
					CONVERT_INT(dwPtnTbl[i],dwPtnTbl[i],m_tParam.bBigEndian);
				}
				
			}

			memcpy(lpPos+8,&dwPtnTbl,(5+MAX_EXT_NUM)*sizeof(DWORD));	
		}
		break;
	default:
		goto REMOVE_ERROR;
	}
	
	///first write before Dsp part
	dwDestSize = (dwPtnTbl[nTblIndex] == 0xFFFFFFFF) ? dwSrcSize : dwPtnTbl[nTblIndex];
	
	dwWrite = fwrite(pBufSrc,1,dwDestSize,pFile);
	
	if(dwWrite != dwDestSize)
	{
		_tprintf(_T("write file[%s] failed."),lpDestFile);
		_stprintf(m_szLastErrMsg,_T("write file[%s] failed."),lpDestFile);
		goto REMOVE_ERROR;
	}
	///Second to write after reserved2
	if (dwPtnTbl[nTblIndex] != 0xFFFFFFFF && dwRmvPtnSize != 0)
	{
		dwDestSize = dwSrcSize - (dwPtnTbl[nTblIndex] +dwRmvPtnSize);
		dwWrite = fwrite(pBufSrc+dwPtnTbl[nTblIndex]+dwRmvPtnSize,1,dwDestSize,pFile);	
		if(dwWrite != dwDestSize)
		{
			_tprintf(_T("write file[%s] failed."),lpDestFile);
			_stprintf(m_szLastErrMsg,_T("write file[%s] failed."),lpDestFile);
			goto REMOVE_ERROR;
		}
	}

	if(pFile)
	{
		fclose(pFile);
		pFile = NULL;
	}
	SAFE_DEL(pBufSrc);
	return TRUE;

REMOVE_ERROR:
	if(pFile)
	{
		fclose(pFile);
		pFile = NULL;
	}
	SAFE_DEL(pBufSrc);
	
	return FALSE;
}
BOOL  CBZip::OverWtirePartiton(LPCTSTR lpszSrcFile,LPCTSTR lpNewfile,int nTblIndex,LPCTSTR lpDestFile)
{
	LPBYTE pBufExDspz = NULL;
	DWORD  dwExDspzSize = 0;
	LPBYTE pBufNewBzp  = NULL;
	DWORD  dwNewBzpSize  = 0;
	
	FILE *pFile = NULL;
	CFileFind finder;
	DWORD dwPad = 0;
	BYTE pad[4] = {0xFF,0xFF,0xFF,0xFF};
	DWORD dwWrite = 0;
	UPDATE_DATA_T ud;
	memset(&ud,0xFF,sizeof(ud));
	
	BOOL bFind = FALSE;
	BYTE head[8] = {0};
	BYTE tail[4] = {0};
	DWORD dwMagic = 0;
	DWORD dwEnd = 0;
	LPBYTE lpPos = NULL;
	LPBYTE lpCur = NULL;
	LPBYTE lpEnd = NULL;
	LPBYTE lpDataEnd = NULL;

	_TCHAR szNewBzp[MAX_PATH*2] = {0};
	_TCHAR szName[MAX_PATH] = {0};
	CStringArray agBzpSrcFile;
	BOOL bInsertNewBzp = FALSE;
	E_PS_TYPE e_pstype = E_UNFINDE;
	DWORD dwPtnTbl[5+MAX_EXT_NUM] = {0};
	BOOL bFindTbl = FALSE;
	DWORD dwInsertAddr = 0;
	int i = 0;

	GetTempPath(MAX_PATH*2,szNewBzp);
	_stprintf(szName,_T("NewBzp%d.bin"),GetTickCount());
	_tcscat(szNewBzp,szName);
	
	if (3 == nTblIndex) //dsp table
	{
		agBzpSrcFile.Add(_T("dspz"));
		agBzpSrcFile.Add(lpNewfile);	
	}
	else if (nTblIndex > 4 && nTblIndex < 5+MAX_EXT_NUM)
	{
		CString strFlag;
		strFlag.Format(_T("%s%d"),EXTZ_KEY,nTblIndex-5);
		agBzpSrcFile.Add(strFlag);
		agBzpSrcFile.Add(lpNewfile);	
	}
	else
	{
		goto OVERW_ERROR;
	}
	
		
	if (!Bzp(agBzpSrcFile,szNewBzp) )
	{
		_tprintf(_T("cannot create new Bzp file [%s]."),szNewBzp);
		_stprintf(m_szLastErrMsg,_T("cannot create new Bzp file [%s]."),szNewBzp);
		goto OVERW_ERROR;
	}


	// deal with files except dspz
	if(!GetOrgData(lpszSrcFile,pBufExDspz,dwExDspzSize))
	{
		goto OVERW_ERROR;
	}
	//////////////////////////////////////////////////////////////////////////
	// deal with new dspz file
	if(!GetOrgData(szNewBzp,pBufNewBzp,dwNewBzpSize))
	{
		goto OVERW_ERROR;
	}
	dwMagic = PARTITION_MAGIC_FIRST;
	CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
	*(DWORD*)head = dwMagic;
	
	dwMagic = PARTITION_MAGIC_SECOND;
	CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
	*(DWORD*)(head+4) = dwMagic;
	
	dwMagic = PARTITION_MAGIC_END;
	CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
	*(DWORD*)tail = dwMagic;
	
	dwEnd = dwExDspzSize < MAX_SEARCH_SIZE? dwExDspzSize : MAX_SEARCH_SIZE;
	lpCur = pBufExDspz;
	lpEnd = pBufExDspz+dwEnd;
	lpDataEnd = pBufExDspz + dwExDspzSize;
	
	while(lpCur < lpEnd)
	{
		// find head 
		lpPos = std::search(lpCur,lpEnd, head,head+7);
		if(lpPos < lpEnd)
		{
			if((lpPos + 8+ sizeof(UPDATE_DATA_T) +4) < lpDataEnd)
			{
				// check tail
				if( memcmp(lpPos + 8 + sizeof(UPDATE_DATA_T), tail,4) ==  0	)
				{
					e_pstype = E_5_PATITION;
					break;
				}
				else if(memcmp(lpPos + 8+ sizeof(UPDATE_DATA_T) +MAX_EXT_NUM*sizeof(DWORD), tail,4) == 0 )
				{
					e_pstype = E_29_PATITION;
					break;
				}
				else
				{
					lpCur += 8;
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	
	//if(!bFind)
	if (E_UNFINDE == e_pstype )
	{
		_tprintf(_T("cannot find flag in ps file [%s]."),lpszSrcFile);
		_stprintf(m_szLastErrMsg,_T("cannot file flag in ps file [%s]."),lpszSrcFile);
		goto OVERW_ERROR;
	}

	//////////////////////////////////////////////////////////////////////////
	switch (e_pstype)
	{
	case E_5_PATITION:
		if (nTblIndex >= 4)
		{
			_tprintf(_T("overwrite nTblIndex > partition tables number."));
			_stprintf(m_szLastErrMsg,_T("overwrite nTblIndex > partition tables number."));
			goto OVERW_ERROR;
		}
		memcpy((LPBYTE)&dwPtnTbl,lpPos+8,5*sizeof(DWORD));
		for (i = nTblIndex+1; i < 5;++i) 
		{
			if(dwPtnTbl[i] != 0xFFFFFFFF)
			{
				CONVERT_INT(dwPtnTbl[i],dwPtnTbl[i],m_tParam.bBigEndian);
				dwPtnTbl[nTblIndex] = dwPtnTbl[i];
				dwInsertAddr = dwPtnTbl[nTblIndex];
				CONVERT_INT(dwPtnTbl[i],dwPtnTbl[i],m_tParam.bBigEndian);
				bFindTbl = TRUE;
				bInsertNewBzp = TRUE;
				break;
			}
			
		}
		if (!bFindTbl)
		{
			dwPtnTbl[nTblIndex] = dwExDspzSize;
		}

		CONVERT_INT(dwPtnTbl[nTblIndex],dwPtnTbl[nTblIndex],m_tParam.bBigEndian);
		memcpy(lpPos+8,&dwPtnTbl,5*sizeof(DWORD));
		break;
	case E_29_PATITION:
		memcpy((LPBYTE)&dwPtnTbl,lpPos+8,(5+MAX_EXT_NUM)*sizeof(DWORD));	
		for (i = nTblIndex+1; i < 5+MAX_EXT_NUM;++i) 
		{
			if(dwPtnTbl[i] != 0xFFFFFFFF)
			{
				CONVERT_INT(dwPtnTbl[i],dwPtnTbl[i],m_tParam.bBigEndian);
				dwPtnTbl[nTblIndex] = dwPtnTbl[i];
				dwInsertAddr = dwPtnTbl[nTblIndex];
				CONVERT_INT(dwPtnTbl[i],dwPtnTbl[i],m_tParam.bBigEndian);
				bFindTbl = TRUE;
				bInsertNewBzp = TRUE;
				break;
			}
			
		}
		if (!bFindTbl)
		{
			dwPtnTbl[nTblIndex] = dwExDspzSize;
		}
			
		for (i = nTblIndex+1; i < 5+MAX_EXT_NUM;++i) 
		{
			if(dwPtnTbl[i] != 0xFFFFFFFF)
			{
				CONVERT_INT(dwPtnTbl[i],dwPtnTbl[i],m_tParam.bBigEndian);
				dwPtnTbl[i] = dwPtnTbl[i] + ((dwNewBzpSize + 3)/4) * 4;
				CONVERT_INT(dwPtnTbl[i],dwPtnTbl[i],m_tParam.bBigEndian);
			}
			
		}
		CONVERT_INT(dwPtnTbl[nTblIndex],dwPtnTbl[nTblIndex],m_tParam.bBigEndian);
		memcpy(lpPos+8,&dwPtnTbl,(5+MAX_EXT_NUM)*sizeof(DWORD));

		break;
	default:
		goto OVERW_ERROR;
	}
	
	//////////////////////////////////////////////////////////////////////////
	
	
	if(finder.FindFile(lpDestFile))
	{
		DWORD dwAtt = ::GetFileAttributes(lpDestFile);
		dwAtt &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(lpDestFile,dwAtt);
		finder.Close();
	}
	
	pFile = _tfopen(lpDestFile,_T("wb"));
	if(pFile == NULL)
	{
		_tprintf(_T("cannot create file [%s]."),lpDestFile);
		_stprintf(m_szLastErrMsg,_T("cannot create file [%s]."),lpDestFile);
		goto OVERW_ERROR;
	}

#define OVER_WRITE_FILE(p,t) do{\
	dwWrite = fwrite(p,1,(t),pFile);\
	if(dwWrite != (t))\
	{\
	_tprintf(_T("write file [%s] failed."),lpDestFile);\
	_stprintf(m_szLastErrMsg,_T("write file [%s] failed."),lpDestFile);\
	goto OVERW_ERROR;\
	}\
	}while(0)
	
	if(bInsertNewBzp)
	{
		//////////////////////////////////////////////////////////////////////////

		CONVERT_INT(dwPtnTbl[nTblIndex],dwPtnTbl[nTblIndex],m_tParam.bBigEndian);
		OVER_WRITE_FILE(pBufExDspz,dwInsertAddr);
		dwPad = ((dwInsertAddr + 3)/4) * 4  - dwInsertAddr;
		if(dwPad > 0)
		{
			OVER_WRITE_FILE(pad,dwPad);
		}

		OVER_WRITE_FILE(pBufNewBzp,dwNewBzpSize);
		dwPad = ((dwNewBzpSize + 3)/4) * 4  - dwNewBzpSize;
		if(dwPad > 0)
		{
			OVER_WRITE_FILE(pad,dwPad);
		}

		OVER_WRITE_FILE(pBufExDspz+dwInsertAddr,dwExDspzSize - dwInsertAddr);

	}
	else
	{
		OVER_WRITE_FILE(pBufExDspz,dwExDspzSize);
		dwPad = ((dwExDspzSize + 3)/4) * 4  - dwExDspzSize;
		if(dwPad > 0)
		{
			OVER_WRITE_FILE(pad,dwPad);
		}
		
		OVER_WRITE_FILE(pBufNewBzp,dwNewBzpSize);
		dwPad = ((dwNewBzpSize + 3)/4) * 4  - dwNewBzpSize;
		if(dwPad > 0)
		{
			OVER_WRITE_FILE(pad,dwPad);
		}
		
	}
	
	fclose(pFile);
	pFile = NULL;
	
	SAFE_DEL(pBufExDspz);
	SAFE_DEL(pBufNewBzp);
	return TRUE;

OVERW_ERROR:
	if(pFile != NULL)
	{
		fclose(pFile);
		pFile = NULL;
	}
	SAFE_DEL(pBufExDspz);
	SAFE_DEL(pBufNewBzp);

	return FALSE;
}

BOOL CBZip::IsExtendPtnTbl(LPCTSTR lpszSrcFile)
{
	LPBYTE pBufSrc = NULL;
	DWORD  dwSrcSize = 0;
	if(!GetOrgData(lpszSrcFile,pBufSrc,dwSrcSize))
		return FALSE;

	DWORD dwEnd = dwSrcSize < MAX_SEARCH_SIZE? dwSrcSize : MAX_SEARCH_SIZE;
	LPBYTE lpCur = pBufSrc;
	LPBYTE lpEnd = pBufSrc+dwEnd;
	LPBYTE lpDataEnd = pBufSrc + dwSrcSize;
	LPBYTE lpPos= NULL;
	BYTE head[8] = {0};
	BYTE tail[4] = {0};
	DWORD dwMagic = 0;

	BOOL bBigEndian = TRUE;	//Used to bigEndian
	dwMagic = PARTITION_MAGIC_FIRST;
	CONVERT_INT(dwMagic,dwMagic,bBigEndian);
	*(DWORD*)head = dwMagic;

	dwMagic = PARTITION_MAGIC_SECOND;
	CONVERT_INT(dwMagic,dwMagic,bBigEndian);
	*(DWORD*)(head+4) = dwMagic;

	dwMagic = PARTITION_MAGIC_END;
	CONVERT_INT(dwMagic,dwMagic,bBigEndian);
	*(DWORD*)tail = dwMagic;

	BOOL bExtendPtnTbl = FALSE;
	while(lpCur < lpEnd)
	{
		// find head 
		lpPos = std::search(lpCur,lpEnd, head,head+7);
		if(lpPos < lpEnd)
		{
			if((lpPos + 8+sizeof(UPDATE_DATA_T) +4) < lpDataEnd)
			{
				// check tail
				if(memcmp(lpPos + 8 + sizeof(UPDATE_DATA_T) +MAX_EXT_NUM*sizeof(DWORD), tail,4) == 0 )
				{
					bExtendPtnTbl = TRUE;
					break;
				}
				else
				{
					lpCur += 8;
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	if(!bExtendPtnTbl)
	{
		lpCur = pBufSrc;
		lpEnd = pBufSrc+dwEnd;
		lpDataEnd = pBufSrc + dwSrcSize;
		lpPos= NULL;
		
		dwMagic = 0;
		bBigEndian = FALSE;	//Changed to littleEndian
		dwMagic = PARTITION_MAGIC_FIRST;
		CONVERT_INT(dwMagic,dwMagic,bBigEndian);
		*(DWORD*)head = dwMagic;
		
		dwMagic = PARTITION_MAGIC_SECOND;
		CONVERT_INT(dwMagic,dwMagic,bBigEndian);
		*(DWORD*)(head+4) = dwMagic;
		
		dwMagic = PARTITION_MAGIC_END;
		CONVERT_INT(dwMagic,dwMagic,bBigEndian);
		*(DWORD*)tail = dwMagic;
		
		bExtendPtnTbl = FALSE;
		while(lpCur < lpEnd)
		{
			// find head 
			lpPos = std::search(lpCur,lpEnd, head,head+7);
			if(lpPos < lpEnd)
			{
				if((lpPos + 8+sizeof(UPDATE_DATA_T) +4) < lpDataEnd)
				{
					// check tail
					if(memcmp(lpPos + 8+ sizeof(UPDATE_DATA_T) +MAX_EXT_NUM*sizeof(DWORD), tail,4) == 0 )
					{
						bExtendPtnTbl = TRUE;
						break;
					}
					else
					{
						lpCur += 8;
					}
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}

	}
	SAFE_DEL(pBufSrc);
	
	return bExtendPtnTbl;
}
BOOL CBZip::ResOverMerge(LPCTSTR pszNewBzpFile,LPCTSTR pszSplitFile,DWORD dwOrgLen, LPCTSTR pszBzpFile)
{
	BOOL bRet = FALSE;

	LPBYTE pBufPs = NULL;
	DWORD  dwPsSize = 0;
	LPBYTE pBufSplit  = NULL;
	DWORD  dwSplitpSize  = 0;

	DWORD dwPtnTbl[5+MAX_EXT_NUM] = {0};
	DWORD dwTblNum = 0;
	
	FILE *pFile = NULL;
	CFileFind finder;
	DWORD dwPad = 0;
	BYTE pad[4] = {0xFF,0xFF,0xFF,0xFF};
	DWORD dwWrite = 0;
	
	BOOL bFind = FALSE;
	BYTE head[8] = {0};
	BYTE tail[4] = {0};
	DWORD dwMagic = 0;
	DWORD dwEnd = 0;
	LPBYTE lpPos = NULL;
	LPBYTE lpCur = NULL;
	LPBYTE lpEnd = NULL;
	LPBYTE lpDataEnd = NULL;
	DWORD dwOffset = 0;

	dwMagic = PARTITION_MAGIC_FIRST;
	CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
	*(DWORD*)head = dwMagic;
	
	dwMagic = PARTITION_MAGIC_SECOND;
	CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
	*(DWORD*)(head+4) = dwMagic;
	
	dwMagic = PARTITION_MAGIC_END;
	CONVERT_INT(dwMagic,dwMagic,m_tParam.bBigEndian);
	*(DWORD*)tail = dwMagic;

	do 
	{
		if(!GetOrgData(pszNewBzpFile,pBufPs,dwPsSize))			break;
		if(!GetOrgData(pszSplitFile,pBufSplit,dwSplitpSize))	break;
		dwOffset =  dwPsSize - dwOrgLen;
		
		dwEnd = dwPsSize < MAX_SEARCH_SIZE? dwPsSize : MAX_SEARCH_SIZE;
		lpCur = pBufPs;
		lpEnd = pBufPs+dwEnd;
		lpDataEnd = pBufPs + dwPsSize;
		
		while(lpCur < lpEnd)
		{
			// find head 
			lpPos = std::search(lpCur,lpEnd, head,head+7);
			if(lpPos < lpEnd)
			{
				if((lpPos + 8+ sizeof(UPDATE_DATA_T) +4) < lpDataEnd)
				{
					// check tail
					if( 
						memcmp(lpPos + 8 + sizeof(UPDATE_DATA_T), tail,4) ==  0	||
						memcmp(lpPos + 8+ sizeof(UPDATE_DATA_T) +MAX_EXT_NUM*sizeof(DWORD), tail,4) == 0
					   )
					{
						bFind = TRUE;
						break;
					}
					else
					{
						lpCur += 8;
					}
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		
		if(!bFind)	break;
		memcpy((LPBYTE)&dwPtnTbl,lpPos+8,(5+MAX_EXT_NUM)*sizeof(DWORD));
		dwTblNum = m_bExtPtnTbl ? 5+MAX_EXT_NUM:5;

		for (int i = 0;i<dwTblNum;++i)
		{
			if(dwPtnTbl[i] != 0xFFFFFFFF && i !=1) //1=res Offset
			{
				CONVERT_INT(dwPtnTbl[i],dwPtnTbl[i],m_tParam.bBigEndian);
				dwPtnTbl[i] += dwOffset;
				CONVERT_INT(dwPtnTbl[i],dwPtnTbl[i],m_tParam.bBigEndian);
			}
		}
		memcpy(lpPos+8,&dwPtnTbl,(5+MAX_EXT_NUM)*sizeof(DWORD));
		if(finder.FindFile(pszBzpFile))
		{
			DWORD dwAtt = ::GetFileAttributes(pszBzpFile);
			dwAtt &= ~FILE_ATTRIBUTE_READONLY;
			::SetFileAttributes(pszBzpFile,dwAtt);
			finder.Close();
		}
		
		pFile = _tfopen(pszBzpFile,_T("wb"));
		if(pFile == NULL)	break;

		dwWrite = fwrite(pBufPs,1, dwPsSize,pFile);
		if(dwWrite !=  dwPsSize) break;
		dwWrite = fwrite(pBufSplit,1, dwSplitpSize,pFile);
		if(dwWrite !=  dwSplitpSize) break;
		bRet = TRUE;

	} while(0);

	if (pFile)
	{
		fclose(pFile);
		pFile = NULL;
	}

	SAFE_DEL(pBufPs);
	SAFE_DEL(pBufSplit);


	return bRet;
}