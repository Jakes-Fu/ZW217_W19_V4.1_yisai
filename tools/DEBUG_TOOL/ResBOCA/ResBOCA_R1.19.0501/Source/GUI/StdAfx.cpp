// stdafx.cpp : source file that includes just the standard includes
//	ResOver.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

CString GetFileExt(LPCTSTR szFileName)
{
	CString strFileName(szFileName);
	CString strExt;
	int nFind = strFileName.ReverseFind(_T('.'));
	if(nFind != -1)
	{
		strExt = strFileName.Right(strFileName.GetLength() - nFind -1);
	}
	 
	return strExt;
}



