/*************************************************************************
* MediaCoder - The universal audio/video transcoder
*
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*************************************************************************/

#include <shlobj.h>
#include <stdio.h>
#include "common.h"

// Fill a WAVEFORMATEX structure
int FillWaveFormatExStructure(WAVEFORMATEX *pWaveFormatEx, int nSampleRate, int nBitsPerSample, int nChannels)
{
	pWaveFormatEx->cbSize = 0;
	pWaveFormatEx->nSamplesPerSec = nSampleRate;
	pWaveFormatEx->wBitsPerSample = nBitsPerSample;
	pWaveFormatEx->nChannels = nChannels;
	pWaveFormatEx->wFormatTag = 1;
	pWaveFormatEx->nBlockAlign = (pWaveFormatEx->wBitsPerSample / 8) * pWaveFormatEx->nChannels;
	pWaveFormatEx->nAvgBytesPerSec = pWaveFormatEx->nBlockAlign * pWaveFormatEx->nSamplesPerSec;
	return 0;
}

int TimeStr2Int(TCHAR* timestr)
{
	TCHAR *p = strchr(timestr, ':');
	if (p) {
		return atoi(timestr) * 60 + atoi(p + 1);
	} else {
		return atoi(timestr);
	}
}

int FileExist(TCHAR* filename)
{
	WIN32_FIND_DATA f;
	HANDLE hFind = FindFirstFile(filename,&f);
	if (hFind != INVALID_HANDLE_VALUE) {
		FindClose(hFind);
		return 1;
	} else {
		return 0;
	}
}

int RetrieveFileSize(TCHAR* filename)
{
	HANDLE hFile = CreateFile(filename,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
	DWORD sizelo, sizehi;
	if (hFile==INVALID_HANDLE_VALUE) return -1;
	sizelo = GetFileSize(hFile,&sizehi);
	CloseHandle(hFile);
	return (sizehi << 22) | (sizelo >> 10);
}

TCHAR* strredup(TCHAR* buf, TCHAR* str, int len)
{
	if (len == 0) len = strlen(str);
	if (!buf) {
		buf = (TCHAR*)malloc(len + 1);
	} else {
		int buflen = strlen(buf);
		if (buflen < len) {
			if (buf) free(buf);
			buf = (TCHAR*)malloc(len + 1);
		}
	}
	memcpy(buf, str, len);
	buf[len] = 0;
	return buf;
}

int BrowseForPath(TCHAR *buffer)
{
	LPITEMIDLIST idlist;
	BROWSEINFO udtBI;
	TCHAR *p;

	ZeroMemory(&udtBI,sizeof(BROWSEINFO));
	udtBI.hwndOwner=0;
	udtBI.lpszTitle="Choose a folder";
	udtBI.ulFlags= BIF_RETURNONLYFSDIRS;
	idlist=SHBrowseForFolder(&udtBI);
	if (!idlist || !SHGetPathFromIDList(idlist,buffer))
		return 0;
	CoTaskMemFree(idlist);
	//update GUI
	p=buffer;
	while(*(++p));
	if (*(p-1)!='\\') {
		*(WORD*)p='\\';
	}
	return (int)(p+1-buffer);
}

int LoadTextFile(TCHAR* filename, TCHAR** buffer, int bufsize)
{
	DWORD bytes;
	HANDLE hFile=CreateFile(filename,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
	int len;

	if (hFile==INVALID_HANDLE_VALUE) return -1;
	len=GetFileSize(hFile,0);
	if (len+1>bufsize) {
		bufsize=len+1;
		*buffer=(TCHAR*)realloc(*buffer, bufsize);
	}
	*(*buffer+len)=0;
    ReadFile(hFile, *buffer, len, &bytes, 0);
	CloseHandle(hFile);
	return bytes;
}

int SetHandleInheritance(LPHANDLE ph, BOOL fInherit)
{
	HANDLE hOld = *ph;
	if (!DuplicateHandle(GetCurrentProcess(), hOld, GetCurrentProcess(), ph, 0, fInherit, DUPLICATE_SAME_ACCESS)) {
		return -1;
	}
	CloseHandle(hOld);
	return 0;
}
