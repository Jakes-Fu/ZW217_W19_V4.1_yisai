#ifndef _IXMLCONFIGPARSE_H__
#define _IXMLCONFIGPARSE_H__

#include "BMAFExport.h"
#include "BMAGlobal.h"

class IXmlConfigParse
{
public:
    virtual ~IXmlConfigParse()=0;
/** initialize the configure file
  *
  * @param lpszFileName: the basic configure file name
  * @param nFlag: if "1", the lpszFileName is spectial one,
  *               program will not find other configure file in the same directory
  *               if "0", program will find all the (*.xml) configue file except "BMAConfigSchema.xml",
  *               and load them by modified time from new to old
  * @return: true, if initialize successfule; false, otherwise
  */
	virtual	BOOL 	Init(LPCTSTR lpszFileName, int nFlag = 0)=0;
	virtual	DWORD	GetProductCount()=0;
//	virtual	void	GetProductNameList(CStringArray &arrProductNameList)=0;
	virtual	void	GetProductNameList(LPTSTR pProductNameList,DWORD dwSize,DWORD &dwRealSize)=0;
	virtual PPRODUCT_INFO_T  GetProdInfo(LPCTSTR lpszProductName)=0;
	virtual LPTSTR  GetConfigFile(LPCTSTR lpszProductName)=0;
	virtual void    Release()=0;

};

BMAF_EXPORT_API BOOL CreateXmlConfigParse( IXmlConfigParse ** pXmlConfigParse );

#endif //_IXMLCONFIGPARSE_H__