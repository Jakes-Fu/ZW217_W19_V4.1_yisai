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
	static void StrTrim (TCHAR* pStr);		    /// ȥ���ַ��������пո�
	static void StrRTrim(TCHAR* pStr);		    /// ȥ���ַ����ұ߿ո�	
	static void StrRTrim(TCHAR* pStr,TCHAR ch,BOOL bRmSpace = TRUE); 	/// ȥ���ַ����ұ߿ո�ָ���ַ�	
	static void StrLTrim(TCHAR* pStr);		    /// ȥ���ַ�����߿ո�
	static void StrLTrim(TCHAR* pStr,TCHAR ch);  	/// ȥ���ַ�����߿ո�ָ���ַ�
    static void MakeUpper(TCHAR* pStr);         	/// �ַ���ת��д
    static void MakeLower(TCHAR* pStr);         /// �ַ���תСд
    static BOOL IsValidValue(TCHAR* lpszValue); /// �ж��Ƿ�Ϊ��Ч����ֵ10���ƻ�16����
public:
	CTCharOpr();
	virtual ~CTCharOpr();

};
