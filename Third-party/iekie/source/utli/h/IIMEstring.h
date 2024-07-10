/*******************************************************************************
	IIMEString.h

    Description: 数据类型定义文件
    
    Author:      Liutingchao、Huangxiaoru
    Date time:   2004-06-21

    Copyright (C) 2000-2004 Beijing Chinese Star Cyber Technology Ltd.
    All rights reserved.
     
	Modefiled by: kewen 2005-1-25
	1. AppendN is added

	Modefiled by: kewen 2005-1-31
	1. remove AppandN

	Modefiled by: kewen 2005-3-3
	1. add: AppendN()
	2. add: CopyN()
	3. add: InsertAt()
	4. add: RemoveAt()

	Modified by kewen Friday, April 22, 2005
		Remove C++ style

	Modified by kewen 2005-6-8
		Add some error check
*******************************************************************************/
#ifndef __IIMESTRING_H__
#define __IIMESTRING_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

IIME_DWORD IIMEString_Length(IIME_PCWSTR pString);

IIME_PWSTR IIMEString_Copy(IIME_PWSTR pDes, IIME_PCWSTR pSrc);

IIME_PWSTR IIMEString_Append(IIME_PWSTR pDes, IIME_PCWSTR pSrc);

IIME_INT IIMEString_Compare(IIME_PCWSTR pSrc1, IIME_PCWSTR pSrc2);

IIME_PWSTR IIMEString_AppendN(IIME_PWSTR wszDes, IIME_PCWSTR wszSrc, IIME_UINT nLen);

IIME_PWSTR IIMEString_CopyN(IIME_PWSTR wszDest, IIME_PCWSTR wszSrc, IIME_UINT nLen);

IIME_PWSTR IIMEString_InsertAt(IIME_PWSTR wszDes, IIME_UINT pos, IIME_WCHAR wch);

IIME_PWSTR IIMEString_RemoveAt(IIME_PWSTR wszDes, IIME_UINT pos);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__IIMESTRING_H__*/
