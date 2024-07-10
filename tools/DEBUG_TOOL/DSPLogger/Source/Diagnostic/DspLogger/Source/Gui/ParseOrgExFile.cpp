// ParseOrgExFile.cpp: implementation of the ParseOrgExFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ParseOrgExFile.h"
#include <Windows.h>
#include <stdio.h>
#include <iostream.h>
#include "dsplogger.h"
//#include "MainFrm.h"
#include "DlgPlaybackOrg.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//#include <sys/timeb.h>
//#include <time.h>

ParseOrgExFile::ParseOrgExFile()
{

}

ParseOrgExFile::~ParseOrgExFile()
{

}


BOOL ParseOrgExFile::LoadFile(LPCTSTR lpszFileName,LPBYTE *ppBuf, DWORD* pSize)
{

	HANDLE	hFile	= INVALID_HANDLE_VALUE;
	LPBYTE	pFileBuf= NULL;
	DWORD	dwSize	= 0;
	DWORD	dwRead	= 0;
	BOOL    bRet	= FALSE;
	do 
	{
		if (NULL == lpszFileName)				break;
		hFile = ::CreateFile( lpszFileName, GENERIC_READ, FILE_SHARE_READ,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
			NULL );
		if( hFile == INVALID_HANDLE_VALUE )		break;
		dwSize = GetFileSize(hFile, NULL);
		pFileBuf = new BYTE[dwSize];
		if( NULL == pFileBuf )					break;
		memset( pFileBuf,0,dwSize);
		if( ReadFile(hFile, pFileBuf, dwSize, &dwRead, NULL) && dwSize == dwRead )
		{
			*ppBuf = pFileBuf;
			*pSize = dwSize;
			bRet = TRUE;
		}
		else
		{
			delete pFileBuf;
			pFileBuf = NULL;
		}

	} while(0);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hFile);
		hFile = NULL;
	}
	return TRUE;
}
BOOL ParseOrgExFile::IsOrgExtendFile(LPCTSTR lpOrgFile)
{
	FILE *fp = NULL;
	BOOL bIsOrgExFile = FALSE;
	BYTE byHead[16] = {0};
	do 
	{
		fp = _tfopen(lpOrgFile, _T("rb"));
		if(fp == NULL)		break;
		if(fread(byHead, 1,16, fp) != NULL)
		{
			if(byHead[0] == 'D'&& byHead[1] == 'S' && byHead[2] == 'P' && byHead[3] == 'F')
			{
				bIsOrgExFile = TRUE;
			}
		}

	} while(0);

	
	if(fp)
	{
		fclose(fp);
		fp = NULL;
	}
	return bIsOrgExFile;
}
BOOL ParseOrgExFile::PlaybackOrgEx(LPCTSTR lpOrgFile,LPVOID lpPara)
{


	CDlgPlaybackOrg *pWnd = (CDlgPlaybackOrg *)lpPara;
	BOOL bRet = FALSE;
	BOOL bLittleEndian = TRUE;
	
	FILE *fp = NULL;
	BYTE byHead[16] = {0};
	LPBYTE pTPDataBuf = NULL;
	
	do 
	{
		if (NULL == pWnd)	break;
		fp = _tfopen(lpOrgFile, _T("rb"));
		if(fp == NULL)		break;
		
		pTPDataBuf = new BYTE[64*1024+4]; 
		if (NULL == pTPDataBuf)	break;
		if(fread(byHead, 1,sizeof(ORG_HEADER), fp))
		{
			if(byHead[0] != 'D'&&byHead[1] != 'S' && byHead[2] != 'P' &&byHead[3] != 'F')
			{
				break;
			}
			else
			{
				bLittleEndian = (byHead[8] == 0)? TRUE: FALSE;
			}
		}
		bRet = TRUE;
		DWORD dwCacheTPNum = 0;
		BYTE  byCacheTP[4] = {0};
		BOOL  bFistPkg = TRUE;
		DWORD dwStopPCTime = 0;
		
		while (!feof(fp))
		{
			
			DSP_ORG_HEADER	orgHead;
			DWORD dwDataSize = 0;
			fread(&orgHead, 1,sizeof(DSP_ORG_HEADER), fp);
			
			if (dwCacheTPNum)
			{
				memcpy(pTPDataBuf,byCacheTP,dwCacheTPNum);
			}
			dwDataSize = fread(pTPDataBuf+dwCacheTPNum, 1,orgHead.dwLen-sizeof(DSP_ORG_HEADER), fp);
			if (dwDataSize == orgHead.dwLen-sizeof(DSP_ORG_HEADER))
			{
				if (bFistPkg)
				{
					pWnd->SetPlaybackDataEx(NULL, 0,1, orgHead.clock_time.dwMicroSec);  //begin timer
					bFistPkg = FALSE;
				}
				dwStopPCTime = orgHead.clock_time.dwMicroSec;
				if (bLittleEndian)
				{
					dwDataSize += dwCacheTPNum;
					int nNum = dwDataSize / 4;
					dwCacheTPNum = dwDataSize % 4;
					
					if(dwCacheTPNum != 0)
					{
						memcpy(&byCacheTP, pTPDataBuf+dwDataSize-dwCacheTPNum, dwCacheTPNum);
					}
					
					LitEdnToBigEdn((DWORD*)pTPDataBuf,nNum);
				}
				
				pWnd->SetPlaybackDataEx(pTPDataBuf, dwDataSize, 0,orgHead.clock_time.dwMicroSec);
			}
			
		}
		//处理小端时最后可能存在剩余不满4个字节的data
		if(dwCacheTPNum && bLittleEndian)
		{
			switch(dwCacheTPNum) {
			case 1:
				pTPDataBuf[3] = byCacheTP[0];
				pTPDataBuf[2] = 0;
				pTPDataBuf[1] = 0;
				pTPDataBuf[0] = 0;
				break;
			case 2:
				pTPDataBuf[3] = byCacheTP[0];
				pTPDataBuf[2] = byCacheTP[1];
				pTPDataBuf[1] = 0;
				pTPDataBuf[0] = 0;
				break;
			case 3:
				pTPDataBuf[3] = byCacheTP[0];
				pTPDataBuf[2] = byCacheTP[1];
				pTPDataBuf[1] = byCacheTP[2];
				pTPDataBuf[0] = 0;
				break;
			default:
				break;
			}
			pWnd->SetPlaybackDataEx(pTPDataBuf, 4, 0,dwStopPCTime);
		}

		if (!bFistPkg && dwStopPCTime)
		{
			pWnd->SetPlaybackDataEx(NULL, 0,2, dwStopPCTime); //end timer
		}
		
		
	} while(0);
	
	if(fp)
	{
		fclose(fp);
		fp = NULL;
	}
	if (pTPDataBuf)
	{
		delete pTPDataBuf;
		pTPDataBuf = NULL;
	}
	return TRUE;
}


BOOL ParseOrgExFile::LitEdnToBigEdn( LPDWORD pwLitEdn, int nNum )
{
	_ASSERTE( pwLitEdn != NULL );
	if(pwLitEdn == NULL)
		return FALSE;
	
	for( int i = 0; i < nNum; ++i, ++pwLitEdn )
	{
		*pwLitEdn = ConvEdn_DWORD(*pwLitEdn);
	}
	
	return TRUE;
}

DWORD ParseOrgExFile::ConvEdn_DWORD( DWORD dwEdn )
{
	return MAKELONG( MAKEWORD(HIBYTE(HIWORD(dwEdn)), LOBYTE(HIWORD(dwEdn))),
		MAKEWORD(HIBYTE(LOWORD(dwEdn)), LOBYTE(LOWORD(dwEdn)))); 
}
