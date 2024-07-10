// XmlConfigParse.h: interface for the CXmlConfigParse class.
//
//////////////////////////////////////////////////////////////////////
#include "IXmlConfigParse.h"
#include "BMAFExport.h"
#include "XMLite.h"
#include "BMAGlobal.h"

#if !defined(AFX_XMLCONFIGPARSE_H__ED2C6C55_63AC_4CF2_9635_79D9D4D4E5A5__INCLUDED_)
#define AFX_XMLCONFIGPARSE_H__ED2C6C55_63AC_4CF2_9635_79D9D4D4E5A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXmlConfigParse: public IXmlConfigParse
{
public:
	CXmlConfigParse();
	virtual ~CXmlConfigParse();
	CXmlConfigParse(LPCTSTR strFileName);
	BOOL Success();//导入XML文件是否成功，在调用CXmlConfigParse(LPCSTR strFileName)后调用


public:
	virtual	BOOL 	Init(LPCTSTR lpszFileName,int nFlag = 0);
	virtual	DWORD	GetProductCount();
	virtual	void	GetProductNameList(LPTSTR pProductNameList,DWORD dwSize,DWORD &dwRealSize);
	virtual PPRODUCT_INFO_T  GetProdInfo(LPCTSTR lpszProductName);
	virtual LPTSTR  GetConfigFile(LPCTSTR lpszProductName);
	virtual void    Release();

protected:	
	BOOL  LoadAllProductInfo(LPCTSTR lpszConfigBase);
    BOOL  _Init(LPCTSTR lpszConfigName);
	BOOL  _LoadProductInfo(LPCTSTR lpszConfigName,BOOL bChangePrdName= FALSE);
	void  _Close();
	void  Clear();
	void  SetCaliFlag(PFILE_INFO_T pFIT,LPCTSTR lpszCaliFlag);

private:
	XNode       m_xmlConfig;
    typedef CMap<CString, LPCTSTR, UINT, UINT>   PRODUCT_ENABLE_MAP;
	PRODUCT_ENABLE_MAP m_mapProductEnable;
	typedef CMap<CString, LPCTSTR, CString, LPCTSTR>   PRODUCT_CONFIG_FILE_MAP;
	PRODUCT_CONFIG_FILE_MAP m_mapPrdCfg;

	PRODUCT_MAP m_mapProductInfo;
	CStringArray m_aProductList;
	PPRODUCT_INFO_T m_pCurProduct;
	CString m_strBaseConfig;


	BOOL        m_bOK;

};

#endif // !defined(AFX_XMLCONFIGPARSE_H__ED2C6C55_63AC_4CF2_9635_79D9D4D4E5A5__INCLUDED_)
