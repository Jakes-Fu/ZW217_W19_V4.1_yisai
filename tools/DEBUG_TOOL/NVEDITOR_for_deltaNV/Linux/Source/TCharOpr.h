// TCharOpr.h: interface for the CTCharOpr class.
//
//////////////////////////////////////////////////////////////////////


#pragma once
#include <string.h>
#include <ctype.h> 
#ifdef WIN32
#include <TCHAR.h>
#else
#include "TCHAR.h"
#include "typedef.h"
#endif

class CTCharOpr  
{
public:
	static void StrTrim (TCHAR* pStr);		    /// 去除字符串中所有空格
	static void StrRTrim(TCHAR* pStr);		    /// 去除字符串右边空格	
	static void StrRTrim(TCHAR* pStr,TCHAR ch,BOOL bRmSpace = TRUE); 	/// 去除字符串右边空格及指定字符	
	static void StrLTrim(TCHAR* pStr);		    /// 去除字符串左边空格
	static void StrLTrim(TCHAR* pStr,TCHAR ch);  	/// 去除字符串左边空格及指定字符
    static void MakeUpper(TCHAR* pStr);         	/// 字符串转大写
    static void MakeLower(TCHAR* pStr);         /// 字符串转小写
    static BOOL IsValidValue(TCHAR* lpszValue); /// 判断是否为有效的数值10进制或16进制
public:
	CTCharOpr();
	virtual ~CTCharOpr();

};
