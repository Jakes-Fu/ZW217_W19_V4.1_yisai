/*************************************************************************
* MediaCoder - The universal audio/video transcoder
*
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*************************************************************************/

#include "atlstr.h"
#include "cfilename.h"

void CFileName::GetDir(CString& result)
{
	int i = FileName.ReverseFind('\\');
	if (i == -1) return;
	result += FileName.Left(i + 1);
}

void CFileName::GetName(CString& result)
{
	int i = FileName.ReverseFind('\\');
	if (i == -1)
		result +=FileName;
	else
		result += FileName.Mid(i + 1);
}

void CFileName::GetExtName(CString& result)
{
	int i = FileName.ReverseFind('.');
	if (i == -1) return;
	result += FileName.Mid(i + 1);
}

void CFileName::SetExtName(TCHAR* pszExt)
{
	int i = FileName.ReverseFind('.');
	if (i != -1) {
		FileName.Delete(i + 1, FileName.GetLength() - i);
	} else {
		FileName += '.';
	}
	FileName += pszExt;
}

void CFileName::RemoveExtName()
{
	int i = FileName.ReverseFind('.');
	if (i != -1) {
		FileName.Delete(i, FileName.GetLength() - i + 1);
	}
}

void CFileName::RemoveDriveLetter()
{
	if (FileName.Mid(1, 2) == ":\\") FileName.Delete(0, 3);
}

void CFileName::RemovePathName()
{
	int i = FileName.ReverseFind('\\');
	if (i >= 0) FileName.Delete(0, i + 1);
}

void CFileName::operator=( CString& str )
{
	FileName = str;
}

void CFileName::operator=( TCHAR* pszData )
{
	FileName = pszData;
}
