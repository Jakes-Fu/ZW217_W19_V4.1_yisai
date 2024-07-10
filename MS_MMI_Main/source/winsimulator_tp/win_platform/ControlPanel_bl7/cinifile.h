/*==================================================================
=  文件名:CIniFile类定义文件                                       =  
=  主要功能:可以读取.修改变量数值,可以设置新的组,新的变量          =
=  修改日期:2002-12-28                                             = 
=  作者:阿皮                                                       =
=  E_Mail:peijikui@sd365.com  QQ:122281932                         =                            = 
====================================================================*/
#ifndef _CINIFILE_H_
#define _CINIFILE_H_
#include "stdafx.h"
#include <afxtempl.h>

//用户接口说明:在成员函数SetVarStr和SetVarInt函数中,当iType等于零,则如果用户制定的参数在ini文件中不存在,
//则就写入新的变量.当iType不等于零,则如果用户制定的参数在ini文件中不存在,就不写入新的变量，而是直接返回FALSE;

class CIniFile 
{
public:
	CIniFile();
	virtual ~CIniFile();
//private:
//	CIniFile(const CIniFile &);
//	CIniFile & operator = (const CIniFile &);
public:
    //virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError = NULL );
    virtual BOOL OpenIniFile(LPCTSTR lpszFileName, CFileException* pError = NULL );

    virtual BOOL Flush();
    virtual void Close();


    //得到变量整数型数值
	BOOL GetVarInt(const CString &strSection,const CString &strVarName ,int &iValue);
	//得到变量字符串型数值
	BOOL GetVarStr(const CString &strSection,const CString &strVarName ,CString & strValue);
	//重新设置变量整数型数值
	BOOL SetVarInt(const CString &strSection,const CString &strVarName ,const int &iValue,const int iType = 0);
	//重新设置变量字符串型数值
	BOOL SetVarStr(const CString &strSection,const CString &strVarName, const CString &strValue,const int iType = 0);
	
private:
	BOOL GetVar(const CString &strSection,const CString &strVarName,CString &strValue);
	BOOL SetVar(const CString &strSection,const CString &strVarName,const CString &strValue,const int iType = 1);
	int  SearchLine(const CString &strSection,const CString &strVarName);

private:
	CArray <CString,CString> m_aFileContainer;
	BOOL  m_bInitialized;
    BOOL  m_bModified;
    CStdioFile m_sfFile;
    CString m_csFile;
	
};

#endif