/*==================================================================
=  �ļ���:CIniFile�ඨ���ļ�                                       =  
=  ��Ҫ����:���Զ�ȡ.�޸ı�����ֵ,���������µ���,�µı���          =
=  �޸�����:2002-12-28                                             = 
=  ����:��Ƥ                                                       =
=  E_Mail:peijikui@sd365.com  QQ:122281932                         =                            = 
====================================================================*/
#ifndef _CINIFILE_H_
#define _CINIFILE_H_
#include "stdafx.h"
#include <afxtempl.h>

//�û��ӿ�˵��:�ڳ�Ա����SetVarStr��SetVarInt������,��iType������,������û��ƶ��Ĳ�����ini�ļ��в�����,
//���д���µı���.��iType��������,������û��ƶ��Ĳ�����ini�ļ��в�����,�Ͳ�д���µı���������ֱ�ӷ���FALSE;

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


    //�õ�������������ֵ
	BOOL GetVarInt(const CString &strSection,const CString &strVarName ,int &iValue);
	//�õ������ַ�������ֵ
	BOOL GetVarStr(const CString &strSection,const CString &strVarName ,CString & strValue);
	//�������ñ�����������ֵ
	BOOL SetVarInt(const CString &strSection,const CString &strVarName ,const int &iValue,const int iType = 0);
	//�������ñ����ַ�������ֵ
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