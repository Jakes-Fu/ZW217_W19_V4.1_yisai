// IBZip.h: interface for the IBZip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IBZIP_H__FDD86934_3ADA_4F15_AED9_22DBA9CB8C13__INCLUDED_)
#define AFX_IBZIP_H__FDD86934_3ADA_4F15_AED9_22DBA9CB8C13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SciDllExport.h"

#define BZP_ZIP 0
#define BZP_7Z  1
#define BZP_RAW 2
#define BZP_7ZF 3
#define BZP_7Z_B3 4

typedef struct _BZP_PARAM_T
{
	DWORD dwMaxRam;
	DWORD dwMaxBzp;
	int   nCmpType;
	int   nLevel;
	BOOL  bCat;
	BOOL  bCat2;	
	BOOL  b2ndBzp;
	BOOL  bRawCmpData;
	BOOL  bBigEndian;
	int   nUsrLevel;
	int   nUsrCmpType;
	DWORD dwUsrPacSize;
	int   nResLevel;
	int   nResCmpType;
	DWORD dwResPacSize;
}BZP_PARAM_T;

class IBZip  
{
public:
	IBZip();
	virtual ~IBZip();

	virtual void     SetLimitParam(BZP_PARAM_T &param)=0;
	virtual BOOL     GenBzp(CStringArray &agFile,LPCTSTR pszBzpFile)=0;
	virtual BOOL     UnBzp(LPCTSTR pszBzpFile,LPCTSTR pszDstDir,CStringArray **ppagFile)=0;
	virtual void     Release()=0;
	virtual void     Reset()=0;
	virtual void     ReleaseMem(CStringArray *pagFile)=0;
	virtual LPCTSTR  GetLastErrMsg()=0;
	virtual BOOL     GenBzpEx(CStringArray &agFile,CStringArray &agFileEx,LPCTSTR pszBzpFile)=0;
	virtual BOOL     ResOverGenBzp(CStringArray &agFile,LPCTSTR pszBzpFile)=0;

};


SCIDLL_EXPORT_API bool CreateBZPFactory ( IBZip ** ppBZP);

#endif // !defined(AFX_IBZIP_H__FDD86934_3ADA_4F15_AED9_22DBA9CB8C13__INCLUDED_)
