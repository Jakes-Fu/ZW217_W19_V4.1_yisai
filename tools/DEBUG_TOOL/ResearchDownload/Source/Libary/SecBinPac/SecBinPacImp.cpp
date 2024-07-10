// SecBinPacImp.cpp: implementation of the CSecBinPacImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SecBinPac.h"
#include "SecBinPacImp.h"
#include "SafeProtector.h"
extern "C"
{
#include "crc16.h"
}
#include <memory>
#include "Sparce2Raw.h"

#pragma comment(lib, "Sparce2Raw.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define SPARSE_HEADER_MAGIC	0xed26ff3a

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DWORD   CSecBinPacImp::m_nCounter = 0;
CSecBinPacImp::CSecBinPacImp()
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_strReleaseDir = _T("");
	m_strCfgPath = _T("");
	m_strPacSTVer = _T("");
	m_bIsExistEmptyFile = FALSE;

    m_bThreadRcv    = TRUE;
    m_dwRcvThreadID = 0;        
    m_hRcvWindow    = NULL; 
    m_dwMsgID       = WM_BINPAC_PROG_MSG;
    m_dwEncryptKey  = DEFAULE_XOR_KEY;
	m_hSparce2Img = NULL;
	m_hRead = NULL;
	m_nSparce2ImgLen = 0;
	InitializeCriticalSection( &m_csLocker );
	LoadSettings();
}

CSecBinPacImp::~CSecBinPacImp()
{
	Close();
	DeleteCriticalSection( &m_csLocker );
}
void CSecBinPacImp::SetSafeKey(DWORD dwKey/* = 0*/)
{
    m_dwEncryptKey  = DEFAULE_XOR_KEY;
    if (dwKey)
    {
        m_dwEncryptKey = dwKey;
    }
}

void CSecBinPacImp::SetPacSTVersion(LPCTSTR lpVersion) //_tsclen(lpVersion) <= 21
{
	if(lpVersion)
	{
		m_strPacSTVer = lpVersion;
	}
}
void CSecBinPacImp::SetReceiver(DWORD dwHandle, DWORD dwMsgID, BOOL bThread/* = FALSE*/)
{
	m_dwMsgID = dwMsgID;

	m_bThreadRcv = bThread;
	
	if(bThread)
	{
		m_dwRcvThreadID  = dwHandle;
	}
	else
	{
		m_hRcvWindow = (HWND)dwHandle;
	}
}

void CSecBinPacImp::PostMessageToUplevel(DWORD dwMsgID, WPARAM wParam, LPARAM lParam  )
{
	
    if( (m_hRcvWindow == NULL) && ( m_dwRcvThreadID == 0) )
    {
        return;
    }
	
    if ( m_bThreadRcv )  
    {
        if( m_dwRcvThreadID != 0 )
        {
            PUMP_MESSAGES();
            PostThreadMessage( m_dwRcvThreadID,  dwMsgID, wParam , lParam );   
        }
    }
    else 
    {
        if( m_hRcvWindow != NULL )
        {
            PUMP_MESSAGES();
            PostMessage( m_hRcvWindow, dwMsgID, wParam , lParam );            
        }
    }
}

BOOL CSecBinPacImp::Open(LPCTSTR lpszFileName, BOOL bOpenFlag /*= FALSE*/)
{
	if (NULL == lpszFileName || 0 == _tcslen(lpszFileName))
	{
		return FALSE;
	}
    m_strPacFile = lpszFileName;
	
	if(!bOpenFlag)
	{
		m_hFile = CreateFile(lpszFileName,
			GENERIC_WRITE, FILE_SHARE_READ,
			NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(INVALID_HANDLE_VALUE == m_hFile)
		{
			CString strError;
			strError.Format(_T("Can not create file [%s]"),lpszFileName);
			TRACE(strError);
			return FALSE;
		}
	}
	else
	{
		m_hFile = CreateFile(lpszFileName,
			GENERIC_READ, FILE_SHARE_READ,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(INVALID_HANDLE_VALUE ==  m_hFile)
		{
			CString strError;
			strError.Format(_T("Can not open file [%s]\n"),lpszFileName);
			TRACE(strError);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CSecBinPacImp::DeleteDirectory(LPCTSTR lpszDirName)// DeleteDirectory(_T("c:\\aaa"))
{
	_ASSERTE(lpszDirName != NULL);
	if(lpszDirName== NULL)
		return TRUE;
	
	CFileFind tempFind;
	BOOL IsFinded = TRUE;
	IsFinded = (BOOL)tempFind.FindFile(lpszDirName);
	if(!IsFinded)
	{
		tempFind.Close();
		return TRUE;
	}
	tempFind.Close();
	
    _TCHAR tempFileFind[MAX_PATH] = {0};
	_stprintf(tempFileFind,_T("%s\\*.*"),lpszDirName);
    IsFinded=(BOOL)tempFind.FindFile(tempFileFind);
    while(IsFinded)
    {
        IsFinded=(BOOL)tempFind.FindNextFile();
        if(!tempFind.IsDots())
        {
            _TCHAR foundFileName[MAX_PATH] = {0};
            _tcscpy(foundFileName,tempFind.GetFileName().GetBuffer(MAX_PATH));
            if(tempFind.IsDirectory())
            {
                _TCHAR tempDir[MAX_PATH] = {0};
                _stprintf(tempDir,_T("%s\\%s"),lpszDirName,foundFileName);
                DeleteDirectory(tempDir);
            }
            else
            {
                _TCHAR tempFileName[MAX_PATH] = {0};
                _stprintf(tempFileName,_T("%s\\%s"),lpszDirName,foundFileName);
                DeleteFile(tempFileName);
            }
        }
    }
    tempFind.Close();
    if(!RemoveDirectory(lpszDirName))
    {
		//CString strError;
		//strError.Format(_T("Delete directory [%s] faild!"),lpszDirName);        
        //return FALSE;
		TRACE(_T("Delete directory [%s] faild!"),lpszDirName);
    }
    return TRUE;
}
unsigned int CSecBinPacImp::Unpacket(LPCTSTR lpszReleaseDirPath,BIN_PACKET_HEADER_T &bph,FILE_T ** ppFileArray,BOOL bFactoryMode/* = FALSE*/)
{
	unsigned int nRet		= E_SUCCEED;
	LPBYTE lpOrgFTBuf		= NULL;
	LPBYTE lpDecryptedBuf	= NULL;
	LPBYTE pBuf				= NULL; 
	FILE_T* paFile			= NULL;
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	DWORD dwBufSize			= sysInfo.dwAllocationGranularity ?  sysInfo.dwAllocationGranularity : MAX_RW_SIZE;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CWaitCursor wait;
	LoadSettings();

	PostMessageToUplevel(m_dwMsgID,PROG_BEGIN,100);
	do 
	{
		if( m_hFile == INVALID_HANDLE_VALUE || ppFileArray == NULL)
		{
			nRet = ERR_INVALIDE_PARAMETERS;
			break;
		}
		pBuf = new BYTE[dwBufSize];
		if (NULL == pBuf)
		{
			nRet = ERR_INSUFFICIENT_MEMORY;
			break;
		}

		LARGE_INTEGER liPacketSize;
		if( !GetFileSizeEx(m_hFile,&liPacketSize))
		{
			nRet = ERR_CHECK_PAC_SIZE_FAIL;
			break;
		}
		
		//check packet size, it must large than the header struct
		if( liPacketSize.QuadPart < (LONGLONG)sizeof(BIN_PACKET_HEADER_T) )
		{
			nRet = ERR_CHECK_PAC_SIZE_FAIL;
			break;
		}
		UINT uReadSize = 0;
#if _MSC_VER < 1300  //VC 6.0
		CFile file((int)m_hFile);
#else
		CFile file(m_hFile);
#endif 
		
		WORD wCRC1 = 0;
		WORD wCRC2 = 0;
		BOOL bCRC  = FALSE;	
		uReadSize  = file.Read(&bph,sizeof(BIN_PACKET_HEADER_T));
		if(uReadSize != sizeof(BIN_PACKET_HEADER_T))
		{
			nRet = ERR_READ_PAC_FAIL;
			break;
		}

		if(bph.dwMagic == PAC_MAGIC)
		{
			bCRC = TRUE;
			wCRC1 = crc16(wCRC1,(BYTE*)&bph,sizeof(bph)-sizeof(WORD));
			if(wCRC1 != 0)
			{
				nRet = ERR_PAC_CRC_FAIL;
				break;
			}
		}

		if (bFactoryMode)
		{
			bph.dwIsNvBackup = FALSE; //The Backup NV Function should be disable in Production Line.
		}
		
		CString strPrdName = bph.szPrdName;
		if(strPrdName.Find(_T("PAC_")) != 0)
		{
			strPrdName.Insert(0,_T("PAC_"));
			_tcscpy(bph.szPrdName,strPrdName);
		}
		
		
		// check packet version
		if( uReadSize !=sizeof(BIN_PACKET_HEADER_T) 
			|| (_tcscmp(bph.szVersion, SZ_BINPAC_VER1)!=0 && _tcscmp(bph.szVersion, SZ_BINPAC_VER2)!=0 && !m_strPacSTVer.IsEmpty() && m_strPacSTVer.Compare(bph.szVersion)) 
			|| (_tcscmp(bph.szVersion, SZ_BINPAC_VER1)!=0 && _tcscmp(bph.szVersion, SZ_BINPAC_VER2)!=0 && m_strPacSTVer.IsEmpty())
			)
		{
			nRet = ERR_UNSUPPORT_PAC_VERSION;
			break;
		}
		
		// check packet size recorded by itself
		LARGE_INTEGER liPacInfoSize;
		liPacInfoSize.HighPart = bph.dwHiSize;
		liPacInfoSize.LowPart  = bph.dwLoSize;

		if(liPacInfoSize.QuadPart != liPacketSize.QuadPart)
		{
			nRet = ERR_INVALID_PAC_SIZE;	//Bin packet's size is not correct,maybe it has been destructed!
			break;
		}
		
		// check the number of files packeted by the packet
		if(bph.nFileCount == 0)
		{
			nRet = ERR_NOT_FIND_FILES_IN_PAC;	//"There is no files in packet!"
			break;	
		}

		CString strDisk;
		//[[ create temp download file directory
		CString strReleaseDir;
		if(lpszReleaseDirPath != NULL && _tcslen(lpszReleaseDirPath)>=2)
		{
			CString strTemp = lpszReleaseDirPath;
			strTemp.TrimRight(_T('\\'));
			EnterCriticalSection( &m_csLocker); 
			m_strReleaseDir.Format(_T("%s\\_DownloadFiles%X_%X_%X"),strTemp.operator LPCTSTR(),GetCurrentThreadId(),::GetTickCount(),m_nCounter);
			++m_nCounter;
			Sleep(200);
			LeaveCriticalSection( &m_csLocker );
			strDisk = strTemp.Left(2); 
		}
		else
		{
			_TCHAR szFilePath[_MAX_PATH] = {0};
			if(::GetTempPath(_MAX_PATH,szFilePath)==0)
			{
				nRet = ERR_GET_TEMP_PATH;//"Can not get temp path!"
				break;
			}
			EnterCriticalSection( &m_csLocker); 
			m_strReleaseDir.Format(_T("%s_DownloadFiles%X_%X_%X"),szFilePath,GetCurrentThreadId(),::GetTickCount(),m_nCounter);
			Sleep(200);
			++m_nCounter;
			LeaveCriticalSection( &m_csLocker );
			strDisk = szFilePath;
			strDisk = strDisk.Left(2);
		}	
		
		CString strErr;
	
		
		CFileFind find;
		if(find.FindFile(m_strReleaseDir))
		{
			DeleteDirectory(m_strReleaseDir);
		}
		find.Close();
		
		if(!CreateDirectory(m_strReleaseDir,NULL))
		{
			if( GetLastError() != ERROR_ALREADY_EXISTS )
			{
			
				nRet = ERR_CREATE_DIRECTORY;//Can not create directory
				break;
			}
		}
		strReleaseDir.Format(_T("%s\\"),m_strReleaseDir.operator LPCTSTR());
	//]] create temp download file directory

		uReadSize = 0;
		paFile = new FILE_T[(UINT)bph.nFileCount];
		if(paFile == NULL)
		{		
			nRet = ERR_INSUFFICIENT_MEMORY;
			break;
		}
		memset(paFile,0,sizeof(FILE_T)*((UINT)bph.nFileCount));
		if (bph.wEncrypted)
		{

            CSafeProtector cProtector;
            cProtector.SetSafeKey(m_dwEncryptKey);
			lpOrgFTBuf = new BYTE[bph.dwFTEncryptedSize];
			if (NULL == lpOrgFTBuf)
			{
				nRet = ERR_INSUFFICIENT_MEMORY;
				break;
			}

			uReadSize = file.Read(lpOrgFTBuf,bph.dwFTEncryptedSize);
			if(uReadSize != bph.dwFTEncryptedSize)
			{
				nRet = ERR_READ_PAC_FAIL;
				break;
			}
			DWORD dwDecryptedSize = 0;
			if (!cProtector.Decrypted(lpOrgFTBuf,bph.dwFTEncryptedSize,lpDecryptedBuf,dwDecryptedSize))
			{
				nRet = ERR_SAFE_DECRYPT_FAIL;
				break;
			}
			if (dwDecryptedSize<((UINT)bph.nFileCount) * sizeof(FILE_T))
			{
				nRet = ERR_PARSE_PAC_INFO_FAIL;
				break;
			}

			memcpy(paFile,lpDecryptedBuf,(UINT)bph.nFileCount * sizeof(FILE_T));

            WORD wFTOrgCRC = 0;
            wFTOrgCRC = crc16(wFTOrgCRC,lpDecryptedBuf,(UINT)bph.nFileCount * sizeof(FILE_T));
           
            if ((UINT)bph.nFileCount * sizeof(FILE_T)!=bph.dwFTOrgSize ||  wFTOrgCRC != bph.wFTCRC)
            {
                nRet = ERR_INVALID_DECRYPTED_DATA;
                break;
            }
			if(bCRC)
			{
				wCRC2 = crc16(wCRC2,lpOrgFTBuf,bph.dwFTEncryptedSize);
			}
		}
		else
		{
			uReadSize = file.Read(paFile,(UINT)bph.nFileCount * sizeof(FILE_T));
			if(uReadSize != ((UINT)bph.nFileCount) * sizeof(FILE_T))
			{
				nRet = ERR_READ_PAC_FAIL;
				break;
			}
			if(bCRC)
			{
				wCRC2 = crc16(wCRC2,(BYTE*)paFile,(UINT)bph.nFileCount * sizeof(FILE_T));
			}
		}

		FILE_T* pFT		  = NULL;
		int nOtherFileNum = 0;
		BOOL bBreaked	  = FALSE;
		
		
		for( int i = 0; i < bph.nFileCount; ++i )
		{
			wait.Restore();
			pFT = paFile + i;
			// check file size if validate
			LARGE_INTEGER liImageSize;
			liImageSize.HighPart = pFT->dwHiFileSize;
			liImageSize.LowPart = pFT->dwLoFileSize;
			if(liImageSize.QuadPart>=liPacketSize.QuadPart)
			{

				nRet = ERR_READ_PAC_FAIL;
				bBreaked = TRUE;
				break;
			}
			
			if(liImageSize.QuadPart == 0)
			{
				if (bFactoryMode)
				{
					if (pFT->nFileFlag != 0 && pFT->dwCanOmitFlag == 0)  //It needs a file
					{
						m_bIsExistEmptyFile = TRUE;
					}
				}
				continue;
			}
			BOOL bFastMode		= FALSE;
			int nIndex			= 1;
			CString strFilePath = strReleaseDir;
			strFilePath			+= pFT->szFileName;

			while(find.FindFile(strFilePath))
			{
				find.Close();
				strFilePath.Format(_T("%s%s(%d)"),strReleaseDir,pFT->szFileName,nIndex);	
				++nIndex;
			}


			BOOL bSparce = FALSE;

			if (_tcsicmp(pFT->szFileID, _T("Super")) == 0)
			{
				DWORD dwSparceMagic;
				UINT64 nCurPos = file.GetPosition();
				uReadSize = file.Read(&dwSparceMagic, sizeof(dwSparceMagic));
				if(uReadSize != sizeof(dwSparceMagic))
				{
					nRet = ERR_READ_PAC_FAIL;
					bBreaked = TRUE;
					break;
				}

				if (nCurPos != file.Seek(-4, FILE_CURRENT) )
				{
					nRet = ERR_SEEK_FILE;
					bBreaked = TRUE;
					break;
				}

				if (dwSparceMagic == SPARSE_HEADER_MAGIC)
				{
					m_hSparce2Img = CreateEvent(NULL, TRUE, FALSE, NULL);
					DWORD dwThreadID;
					m_hRead = file.m_hFile;
					HANDLE hThread = CreateThread(NULL, 0, ThreadFunc, this, 0, &dwThreadID);
					if (hThread == NULL)
					{
						nRet = ERR_READ_PAC_FAIL;
						bBreaked = TRUE;
						break;
					}

					BOOL bLoad = FALSE;
					int nTry = 20;
					while(nTry-- > 0)
					{
						if (WAIT_OBJECT_0 == WaitForSingleObject(m_hSparce2Img, 2000))
						{
							pFT->dwLoFileSize = DWORD(m_nSparce2ImgLen);
							pFT->dwHiFileSize = DWORD((m_nSparce2ImgLen >> 32) & 0xFFFFFFFF);
							if (nCurPos != file.Seek(nCurPos, FILE_BEGIN))
							{
								nRet = ERR_SEEK_FILE;
								bBreaked = TRUE;
								break;
							}
							bSparce = TRUE;
							strFilePath += FLAG_FILE_SUFFIX;
							bLoad = TRUE;
							break;
						}

						PostMessageToUplevel(m_dwMsgID,PROG_PROCEED,(UINT)((100 * nCurPos)/liPacketSize.QuadPart));
					}

					if (!bLoad)
					{
						nRet = ERR_READ_PAC_FAIL;
						bBreaked = TRUE;
						break;
					}
					
					
				}
				else
				{
					bSparce = FALSE;
				}

			}


			if (_tcsicmp(pFT->szFileID,_T("PS")) && _tcsicmp(pFT->szFileID,_T("USERIMG")))  // skip PS and USERIMG,due to LCD config info ia in PS/USERIMG file
			{
				if ( m_dwLoadPolicy && m_dwMaxDataLen && liImageSize.QuadPart>=m_dwMaxDataLen*1024*1024 )
				{
					strFilePath += FLAG_FILE_SUFFIX;
					nIndex = 1;
					while(find.FindFile(strFilePath))
					{
						find.Close();
						strFilePath.Format(_T("%s%s(%d)"),strReleaseDir,pFT->szFileName,nIndex);
						strFilePath += FLAG_FILE_SUFFIX;
						++nIndex;
					}
					
					bFastMode = TRUE;
				}
			}			
			
			memset(pFT->szFileName,0,sizeof(pFT->szFileName));		
			_tcscpy(pFT->szFileName,strFilePath);

			if(i == bph.nFileCount-1 )
			{
				m_strCfgPath = strFilePath;
			}
			
			// other files,such as UDISK_IMG
			if( _tcslen(pFT->szFileID)==0 && (0 ==_tcslen(pFT->szFileName) || i == bph.nFileCount-1) )
			{
				++nOtherFileNum;
			}
			
			HANDLE hImgFile		= NULL;
			DWORD dwWritten		= 0;
			
			hImgFile = CreateFile(strFilePath, GENERIC_WRITE,0, NULL, CREATE_ALWAYS, 
				FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);        
			
			if (hImgFile == INVALID_HANDLE_VALUE) 
			{	
				nRet = ERR_CREAT_FILE;
				bBreaked = TRUE;
				hImgFile = NULL;
				break;
			}
			
			LARGE_INTEGER  liLeft;
			liLeft.QuadPart = liImageSize.QuadPart;
			DWORD  dwRWSize =  sysInfo.dwAllocationGranularity ?  sysInfo.dwAllocationGranularity : MAX_RW_SIZE;
			
			BOOL   bFlag = FALSE;
			
			while(liLeft.QuadPart>0)
			{
				memset(pBuf,0,dwRWSize);	
				if (bFastMode && 2 == m_dwLoadPolicy) //don't generate the temporary download image
				{
					LARGE_INTEGER liImageOffset;
					liImageOffset.HighPart = pFT->dwHiDataOffset;
					liImageOffset.LowPart =  pFT->dwLoDataOffset;

					ULONGLONG llOffset =  liImageOffset.QuadPart + liImageSize.QuadPart;
					if (llOffset != file.Seek(llOffset,0) )
					{
						nRet = ERR_SEEK_FILE;
						bBreaked = TRUE;
						break;
					}
					break;

				}

				PostMessageToUplevel(m_dwMsgID,PROG_PROCEED,(UINT)((100 * file.GetPosition())/liPacketSize.QuadPart));
				
				if( liLeft.QuadPart<dwRWSize )
				{
					dwRWSize = (DWORD)liLeft.QuadPart;
				}

				uReadSize = file.Read(pBuf,dwRWSize);
				if(uReadSize != dwRWSize)
				{
					nRet = ERR_READ_PAC_FAIL;
					bBreaked = TRUE;
					break;
				}

				if(bCRC)
				{
					wCRC2 = crc16(wCRC2,(BYTE*)pBuf,dwRWSize);
				}
				
				if(i == bph.nFileCount-1 )
				{
					if(dwRWSize == liLeft.QuadPart)
					{
						if(pBuf[dwRWSize-1] == 0)
						{
							dwRWSize--;
							bFlag = TRUE;
						}
					}
				}

				if (!bSparce)
				{
					if ( !bFastMode || (0 == m_dwLoadPolicy)) //generate the temporary download image
					{
						dwWritten = 0;
						BOOL bOk = WriteFile(hImgFile, pBuf, dwRWSize, &dwWritten, NULL); 			
						if(!bOk || dwWritten != dwRWSize)
						{	
							nRet = ERR_WRITE_FILE;
							bBreaked = TRUE;
							break;
						}
					}
				}			
						
				
				
				if(bFlag )
				{				
					dwRWSize++;			
				}
				
				liLeft.QuadPart -= dwRWSize;
			}

			SAFE_CLOSE_HANDLE(hImgFile);
			
			if(bBreaked)
			{
				break;
			}
			else
			{
				if (bFastMode)
				{
					DWORD dwAtt = 0;						
					dwAtt = ::GetFileAttributes(strFilePath.operator LPCTSTR());
					dwAtt += FILE_ATTRIBUTE_HIDDEN; //set hidden attribute 
					::SetFileAttributes(strFilePath.operator LPCTSTR(),dwAtt);	
				}
				
			}
		}
		if(bBreaked)
		{
			break;
		}
	
			
		bph.nFileCount = bph.nFileCount-nOtherFileNum;		
		*ppFileArray = new FILE_T[(UINT)bph.nFileCount];
		if( NULL == *ppFileArray)
		{
			nRet = ERR_INSUFFICIENT_MEMORY;
			break;
		}
		memcpy(*ppFileArray,paFile,sizeof(FILE_T)*(UINT)bph.nFileCount);
		
		if(bCRC)
		{
			wCRC2 = crc16(wCRC2,(BYTE*)&bph.wCRC2,sizeof(WORD));
			if(wCRC2 != 0)
			{	
				if ( 0 == m_dwMaxDataLen ||  2 != m_dwLoadPolicy)
				{
					nRet = ERR_PAC_CRC_FAIL;
					break;
				}

			}
		}

	}while(0);
	PostMessageToUplevel(m_dwMsgID,PROG_END,100);	
	SAFE_DELETE_ARRAY(pBuf);
	SAFE_DELETE_ARRAY(lpOrgFTBuf);
	SAFE_DELETE_ARRAY(lpDecryptedBuf);
	SAFE_DELETE_ARRAY(paFile);	
	return nRet;
}

DWORD WINAPI CSecBinPacImp::ThreadFunc(LPVOID pParam)
{
	CSecBinPacImp* pPac = (CSecBinPacImp*)pParam;
	CSparce2Raw csr;
	pPac->m_nSparce2ImgLen = csr.GetImgSize(pPac->m_hRead);
	SetEvent(pPac->m_hSparce2Img);
	return 0;
}

BOOL CSecBinPacImp::IsMapFromPac(LPCTSTR lpDLFile)
{
	BOOL bRet = FALSE;
	CString strFileName(lpDLFile);
	if( strFileName.GetLength() > MAX_FLAG_LEN && 0 == strFileName.Right(MAX_FLAG_LEN).Compare(FLAG_FILE_SUFFIX) )
	{
		bRet = TRUE;
	}
	return bRet;

}

unsigned int CSecBinPacImp::Packet(const PBIN_PACKET_HEADER_T pbph,LPCTSTR lpszCfgFile, FILE_T * pFileArray,LPCTSTR lpszOrgPacFile/* = NULL*/,BOOL bSafeMode/* =FALSE */)
{
	unsigned int nRet		= E_SUCCEED;
	LPBYTE lpOrgFTBuf		= NULL;
	LPBYTE lpEncryptedBuf	= NULL;
	LPBYTE pCfgText			= NULL;	
	LPBYTE pBuf				= NULL;
	HANDLE hFileOrgPac		= NULL;
	BIN_PACKET_HEADER_T bph;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CWaitCursor wait;

	do 
	{
		if(m_hFile == INVALID_HANDLE_VALUE || pbph == NULL || lpszCfgFile == NULL || pFileArray == NULL)
		{
			nRet = ERR_INVALIDE_PARAMETERS;
			break;
		}
			
		memcpy(&bph,pbph,sizeof(bph));
		if (!m_strPacSTVer.IsEmpty())
		{
			_tcsncpy(bph.szVersion,m_strPacSTVer.operator LPCTSTR(),MAX_BPH_VER_LEN);
		}
		else
		{
			bSafeMode ? _tcscpy(bph.szVersion,SZ_BINPAC_VER2) : _tcscpy(bph.szVersion,SZ_BINPAC_VER1);
		}
        
		bph.nFileCount = pbph->nFileCount + 1; // add a configure file
		bph.dwFileOffset = sizeof(bph);

#if _MSC_VER < 1300  //VC 6.0
		CFile file((int)m_hFile);
#else
		CFile file(m_hFile);
#endif 	
		int nFileCount		= bph.nFileCount;
		CString strPrdName	= bph.szPrdName;
		FILE_T* pFT			= NULL;
		FILE_T  ft;	
		LARGE_INTEGER liOffset;
		liOffset.QuadPart = (ULONGLONG)sizeof(bph);
		WORD wCRC1 = 0;
		WORD wCRC2 = 0;

		DWORD dwOrgFTSize		= sizeof(FILE_T)*((UINT)nFileCount);
		DWORD dwEncryptedSize	= dwOrgFTSize;
		DWORD dwOrgOffet		= 0;
		BOOL  bNeedLoadOrgPac	= FALSE;
		
        CSafeProtector cProtector;
        cProtector.SetSafeKey(m_dwEncryptKey);
       
		lpOrgFTBuf = new BYTE[dwOrgFTSize];
		if (NULL == lpOrgFTBuf)
		{
			nRet = ERR_INSUFFICIENT_MEMORY;
			break;
		}
        if(bSafeMode)
        {
            dwEncryptedSize = cProtector.GetEncryptedSize(dwOrgFTSize);
            if ( 0xFFFFFFFF == dwEncryptedSize )
            {
                nRet = ERR_GET_ENCRYPTED_SIZE_FAIL;
                break;
            }
        }
		
		liOffset.QuadPart += dwEncryptedSize;
		memset(lpOrgFTBuf,0,dwOrgFTSize);		
		
		//[[ record bin file information
		for( int i = 0; i< nFileCount-1; ++i )
		{
			pFT = pFileArray+i;		
			memset(&ft,0,sizeof(FILE_T));
			ft.dwSize = sizeof(FILE_T);
			
			if( 0 != _tcslen(pFT->szFileName) )
			{
				CString strFileName(pFT->szFileName);
				int nFind = strFileName.ReverseFind(_T('\\'));
				strFileName = strFileName.Right(strFileName.GetLength()-nFind -1);
				if ( IsMapFromPac(pFT->szFileName) )
				{
					bNeedLoadOrgPac = TRUE;
					memcpy(&ft,pFT,sizeof(FILE_T));
					ft.dwHiDataOffset = liOffset.HighPart;
					ft.dwLoDataOffset = liOffset.LowPart;
					strFileName = strFileName.Left(strFileName.GetLength()- MAX_FLAG_LEN);
				}
				else
				{
					LARGE_INTEGER liImageSize;
					liImageSize.QuadPart = GetFileSizeEx(pFT->szFileName);
					pFT->dwHiFileSize = liImageSize.HighPart;
					pFT->dwLoFileSize = liImageSize.LowPart;
					pFT->dwHiDataOffset = liOffset.HighPart;
					pFT->dwLoDataOffset = liOffset.LowPart;
					memcpy(&ft,pFT,sizeof(FILE_T));
				}
				memset(ft.szFileName,0,sizeof(ft.szFileName));
				_tcscpy(ft.szFileName,strFileName);
			}
			else
			{
				pFT->dwHiFileSize = 0;
				pFT->dwLoFileSize = 0;
				pFT->dwHiDataOffset = 0;
				pFT->dwLoDataOffset = 0;
				memcpy(&ft,pFT,sizeof(FILE_T));
			}		
			
			memcpy(lpOrgFTBuf+dwOrgOffet,&ft,sizeof(ft));	
			liOffset.QuadPart += pFT->dwLoFileSize;
			liOffset.HighPart += pFT->dwHiFileSize;
			dwOrgOffet += sizeof(ft);
		}
		//]] record bin file information
		
		
		//[[ record configure file information
		
		DWORD nCfgSize = 0;
		memset(&ft,0,sizeof(FILE_T));
		ft.dwSize = sizeof(FILE_T);
		CString str;
		str.Format(_T("%s.xml"),strPrdName.operator LPCTSTR());
		_tcscpy(ft.szFileName,str);
		
		if(!GetSpcPrdConfig(lpszCfgFile,&pCfgText,nCfgSize))
		{
			nRet = ERR_LOAD_FILE;
			break;
		}
		ft.dwLoFileSize	= nCfgSize;
		ft.dwHiDataOffset = liOffset.HighPart;
		ft.dwLoDataOffset = liOffset.LowPart;
		ft.nFileFlag	= 2; // means configure file
		//]] record configure file information
		
		memcpy(lpOrgFTBuf+dwOrgOffet,&ft,sizeof(ft));
		dwOrgOffet += sizeof(ft); 
		
		if(bSafeMode)
		{
            if (!cProtector.Encrypted(lpOrgFTBuf,dwOrgFTSize,lpEncryptedBuf,dwEncryptedSize))
            {
                nRet = ERR_SAFE_ENCRYPT_FAIL;
                break;
            }

			//Write bph
			bph.wEncrypted			= 1;
			bph.dwFTOrgSize			= dwOrgFTSize;
			bph.dwFTEncryptedSize	= dwEncryptedSize;
			bph.wFTCRC				= 0;
			bph.wFTCRC				= crc16(bph.wFTCRC,lpOrgFTBuf,dwOrgFTSize);
			file.Write(&bph,sizeof(bph));

			// write Encrypted FT
			file.Write(lpEncryptedBuf,dwEncryptedSize);
			wCRC2 = crc16(wCRC2,lpEncryptedBuf,dwEncryptedSize);
		}
		else
		{
			//Write bph
			file.Write(&bph,sizeof(bph));

			// write FT
			file.Write(lpOrgFTBuf,dwOrgFTSize);
			wCRC2 = crc16(wCRC2,lpOrgFTBuf,dwOrgFTSize);
		}	

		//Load org pac
		if (bNeedLoadOrgPac)
		{
			if ( NULL == lpszOrgPacFile)
			{
				nRet		= ERR_OPEN_ORG_PAC;
				break;
			}

			hFileOrgPac = CreateFile(lpszOrgPacFile,GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if ( hFileOrgPac == INVALID_HANDLE_VALUE)
			{
				nRet		= ERR_OPEN_ORG_PAC;
				hFileOrgPac = NULL;
				break;
			}	
		}
		//[[ write file data
		BOOL bBreaked = FALSE;
		pBuf = new BYTE[MAX_RW_SIZE];
		if ( NULL == pBuf )
		{
			nRet = ERR_INSUFFICIENT_MEMORY;
			break;
		}
		for( int j=0; j<nFileCount-1; ++j )
		{	
			wait.Restore();
			pFT = pFileArray+j;
			if( 0 != _tcslen(pFT->szFileName) )
			{
				HANDLE hImgFile		  = NULL;
				BOOL   bNeedCloseFile = FALSE;
				if ( IsMapFromPac(pFT->szFileName) )
				{
					
					if ( NULL == hFileOrgPac )
					{
						nRet		= ERR_OPEN_ORG_PAC;
						hFileOrgPac = NULL;
						bBreaked	= TRUE;
						break;
					}	
					if (::SetFilePointer(hFileOrgPac,pFT->dwLoDataOffset,(PLONG)&(pFT->dwHiDataOffset),FILE_BEGIN) == INVALID_SET_FILE_POINTER)
					{
						nRet = ERR_SEEK_FILE;
						bBreaked = TRUE;
						break;
					}
					hImgFile = hFileOrgPac;
				}
				else
				{
					hImgFile = CreateFile(pFT->szFileName,GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if (hImgFile == INVALID_HANDLE_VALUE)
					{
						hImgFile	= NULL;
						nRet		= ERR_OPEN_FILE;
						bBreaked	= TRUE;
						break;
					}
					bNeedCloseFile = TRUE;
				}


				LARGE_INTEGER liLeft;
				liLeft.HighPart     = pFT->dwHiFileSize;
				liLeft.LowPart		= pFT->dwLoFileSize;
				DWORD dwReadSize	= MAX_RW_SIZE;
				DWORD dwRetSize		= 0;
				while( liLeft.QuadPart > 0 )
				{
					memset(pBuf,0,MAX_RW_SIZE);
					if(liLeft.QuadPart < MAX_RW_SIZE)
					{
						dwReadSize = (DWORD)liLeft.QuadPart;
					}

					if( !ReadFile(hImgFile,pBuf,dwReadSize,&dwRetSize,NULL) || dwRetSize != dwReadSize)
					{
						nRet	 = ERR_READ_FILE;
						bBreaked = TRUE;
						break;
					}
					liLeft.QuadPart -= dwReadSize;
					file.Write(pBuf,dwReadSize);
					wCRC2 = crc16(wCRC2,(BYTE*)pBuf,dwReadSize);
				}

				if(bNeedCloseFile)
				{
					SAFE_CLOSE_HANDLE(hImgFile);
				}
				if (bBreaked)
				{
					break;
				}							
			}
		}
		
		if (bBreaked)
		{
			break;
		}
		
		file.Write(pCfgText,nCfgSize); // write configure file
		wCRC2 = crc16(wCRC2,(BYTE*)pCfgText,nCfgSize);

		//]] write file data

		//write packet header
		file.Flush();
		LARGE_INTEGER liPacSize;
		liPacSize.QuadPart = file.SeekToEnd();
		bph.dwHiSize = liPacSize.HighPart;
		bph.dwLoSize = liPacSize.LowPart;
		if (bph.dwHiSize)
		{
			if (!m_strPacSTVer.IsEmpty())
			{
				_tcsncpy(bph.szVersion,m_strPacSTVer.operator LPCTSTR(),MAX_BPH_VER_LEN);
			}
			else
			{
				_tcscpy(bph.szVersion,SZ_BINPAC_VER2);
			}
		}
		wCRC1 = crc16(wCRC1,(BYTE*)&bph,sizeof(bph) -sizeof(DWORD));

		bph.wCRC1 = wCRC1;
		bph.wCRC2 = wCRC2;
		
		file.SeekToBegin();
		file.Write(&bph,sizeof(bph));
		file.Flush();
		
	} while (0);
		
	SAFE_DELETE_ARRAY(pBuf);
	SAFE_DELETE_ARRAY(lpOrgFTBuf);
	SAFE_DELETE_ARRAY(lpEncryptedBuf);
	SAFE_DELETE_ARRAY(pCfgText);
	SAFE_CLOSE_HANDLE(hFileOrgPac);
	
	
	return nRet;
}
BOOL CSecBinPacImp::GetSpcPrdConfig(LPCTSTR lpszCfgFile,LPBYTE *ppbText, DWORD &nCount)
{
    
    FILE*   pFile   = _tfopen(lpszCfgFile,_T("rb"));
    LPBYTE  pBuf    = NULL;
    DWORD   dwSize  = 0;
	
    if(pFile == NULL)
        return FALSE;
    fseek(pFile,0,SEEK_END);
    dwSize = ftell(pFile);
    fseek(pFile,0,SEEK_SET);	

	try
	{
		pBuf = new BYTE[dwSize];
	}
	catch(const std::bad_alloc)
	{
		fclose(pFile);
		return FALSE;
	}   

	
    DWORD dwRead = fread(pBuf,1,dwSize,pFile);
    fclose(pFile);
	
    if(dwRead != dwSize)
    {
        SAFE_DELETE_ARRAY(pBuf);
        return FALSE;
    }
	
    *ppbText = pBuf;
    nCount   = dwSize;
    return TRUE;
}

void CSecBinPacImp::Close()
{
	if(m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}

	if (m_hSparce2Img != NULL)
	{
		CloseHandle(m_hSparce2Img);
		m_hSparce2Img = NULL;
	}
}

void CSecBinPacImp::ReleaseMem(FILE_T * paFile)
{
	if(paFile != NULL)
	{
		delete [] paFile;
	}
}

void CSecBinPacImp::GetReleaseDir(TCHAR* szReleaseDir, int nSize)
{
	_tcsncpy(szReleaseDir,m_strReleaseDir.operator LPCTSTR(), nSize);
}

void CSecBinPacImp::GetConfigFilePath(TCHAR* szXMLFile, int nSize)
{
	_tcsncpy(szXMLFile,m_strCfgPath.operator LPCTSTR(), nSize);
}
void CSecBinPacImp::GetErrString(DWORD dwErrorCode,TCHAR* lpszOutString,int nSize)
{
    _tcsncpy(lpszOutString,ErrorToString(dwErrorCode).operator LPCTSTR(), nSize);
}
void CSecBinPacImp::Release()
{
	Close();
	delete this;
}


void CSecBinPacImp::LoadSettings()
{
	_TCHAR szConfigFile[MAX_PATH];
	GetModuleFilePath( NULL, szConfigFile );
	_tcscat( szConfigFile,  _T("\\BinPack.ini") );

	m_dwMaxDataLen = GetPrivateProfileInt(_T("Setting"),_T("MaxDataLength"),0,szConfigFile);
	m_dwLoadPolicy = GetPrivateProfileInt(_T("Setting"),_T("LoadPolicy"),0,szConfigFile);
}