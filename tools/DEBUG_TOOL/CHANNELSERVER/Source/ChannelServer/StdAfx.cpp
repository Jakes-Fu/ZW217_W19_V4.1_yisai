// stdafx.cpp : source file that includes just the standard includes
//	ChannelServer.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

LONG XX_MulDiv( LONG  a,
			   LONG  b,
			   LONG  c )
{
	INT   s;
	LONG  d;
	s = 1;
	if ( a < 0 ) { a = -a; s = -1; }
	if ( b < 0 ) { b = -b; s = -s; }
	if ( c < 0 ) { c = -c; s = -s; }
	d = (LONG)( c > 0 ? ( (__int64)a * b + ( c >> 1 ) ) / c
		: 0x7FFFFFFFL );
	return ( s > 0 ) ? d : -d;
}


int SplitStr(LPCTSTR lpszStr,CStringArray &agStrs,TCHAR chSeparate /*= _T(',')*/)
{
	agStrs.RemoveAll();
	
	int nLen = _tcslen(lpszStr);
	
	if(nLen == 0)
		return 0;
	
	TCHAR *pTmpStr= new TCHAR[nLen+1];
	if(pTmpStr ==NULL)
		return 0;

	_tcscpy(pTmpStr,lpszStr);
	
	LPTSTR  lpBuf  = (LPTSTR)pTmpStr;
	LPTSTR  lpFind = _tcschr(lpBuf, chSeparate);
	CString strTmp;
	while(lpFind != NULL)
	{
		*lpFind = _T('\0');
		strTmp = lpBuf;
		strTmp.TrimLeft();
		strTmp.TrimRight();
		if(!strTmp.IsEmpty())
			agStrs.Add(strTmp);
		lpBuf = lpFind + 1;
		lpFind = _tcschr(lpBuf, chSeparate);
	}
	
	strTmp = lpBuf;
	strTmp.TrimLeft();
	strTmp.TrimRight();
	if(!strTmp.IsEmpty())
		agStrs.Add(strTmp);

	delete [] pTmpStr;
	
	return agStrs.GetSize();
}

