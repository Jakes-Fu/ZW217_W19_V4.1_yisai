/*************************************************************************
IIMEString.c

     Description: 

     Author:      Ke Wen
     Data time:   6/13/2005 1:35:15 PM

     Copyright (C) 2000-2005 Beijing Chinese Star Cyber Technology Ltd.
     All rights reserved.

**************************************************************************/

#include "IIMEstd.h"
#include "IIMEstring.h"

 IIME_DWORD IIMEString_Length(IIME_PCWSTR pString)
{
	int n = 0;
	if (pString == 0)
		return n;

	while (*(pString++) != 0)
		n++;

	return n;
}

 IIME_PWSTR IIMEString_Copy(IIME_PWSTR pDes, IIME_PCWSTR pSrc)
{
	IIME_PWSTR pRet = pDes;
	
	if (pDes != 0 && pSrc != 0){
		while (*pSrc != 0)
			*(pDes++) = *(pSrc++);

		*pDes = 0;
	}

	return pRet;
}

 IIME_PWSTR IIMEString_Append(IIME_PWSTR pDes, IIME_PCWSTR pSrc)
{
	IIME_PWSTR pRet = pDes;

	if (pDes != 0 && pSrc != 0){
		while (*pDes != 0)
			pDes++;

		while (*pSrc != 0)
			*(pDes++) = *(pSrc++);

		*pDes = 0;
	}

	return pRet;
}

 IIME_INT IIMEString_Compare(IIME_PCWSTR pSrc1, IIME_PCWSTR pSrc2)
{
	while (*pSrc1 && *pSrc2)
	{
		if (*pSrc1 < *pSrc2)
		{
			return -1;
		}
		else if(*pSrc1 > *pSrc2)
		{
			return 1;
		}

		pSrc1++;
		pSrc2++;
	}

	if (*pSrc1)
	{
		return 1;
	}

	if (*pSrc2)
	{
		return - 1;
	}

	return 0;
}

 IIME_PWSTR IIMEString_AppendN(IIME_PWSTR wszDes, IIME_PCWSTR wszSrc, IIME_UINT nLen)
{
	IIME_UINT i;
	IIME_PWSTR wszTemp = wszDes;

	if (wszDes != 0 && wszSrc != 0){
		while (*wszTemp)
			wszTemp++;
	    
		for (i = 0; i < nLen; i++)
			wszTemp[i] = wszSrc[i];
		
		wszTemp[i] = 0;
	}

	return wszDes;
}

 IIME_PWSTR IIMEString_CopyN(IIME_PWSTR wszDest, IIME_PCWSTR wszSrc, IIME_UINT nLen)
{
	IIME_UINT i;

	if (wszDest != 0 && wszSrc != 0){
		for (i=0; i<nLen; i++)
			wszDest[i] = wszSrc[i];

		wszDest[i] = 0;
	}

	return wszDest;
}

 IIME_PWSTR IIMEString_InsertAt(IIME_PWSTR wszDes, IIME_UINT pos, IIME_WCHAR wch)
{
	IIME_PWSTR p = wszDes + pos;

	if (wszDes != 0){
		while (*p != 0){
			IIME_WCHAR chTemp = *p;
			*p = wch;
			wch = chTemp;

			p++;
		}

		*p = wch;
		*(p+1) = 0;
	}

	return wszDes;
}

 IIME_PWSTR IIMEString_RemoveAt(IIME_PWSTR wszDes, IIME_UINT pos)
{
	IIME_PWSTR p = wszDes + pos;

	if (wszDes != 0){
		while (*p != 0){
			*p = *(p + 1);

			p++;
		}

		*p = 0;
	}

	return wszDes;
}

