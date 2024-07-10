// BZip.h: interface for the CBZip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BZIP_H__72292599_1D32_4FEA_B212_E79E91D98541__INCLUDED_)
#define AFX_BZIP_H__72292599_1D32_4FEA_B212_E79E91D98541__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IBZip.h"

#define	MAX_EXT_NUM		24
const TCHAR EXT_KEY[]	= _T("-ext");
const TCHAR EXTZ_KEY[]	= _T("-extz");

class CBZip : public IBZip  
{
public:
	CBZip();
	virtual ~CBZip();
	virtual void     SetLimitParam(BZP_PARAM_T &param);
	virtual BOOL     GenBzp(CStringArray &agFile,LPCTSTR pszBzpFile);
	virtual BOOL     GenBzpEx(CStringArray &agFile,CStringArray &agFileEx,LPCTSTR pszBzpFile);
	virtual BOOL     ResOverGenBzp(CStringArray &agFile,LPCTSTR pszBzpFile);
	virtual BOOL     UnBzp(LPCTSTR pszBzpFile,LPCTSTR pszDstDir,CStringArray **ppagFile);	
	virtual void     Release();
	virtual void     Reset();
	virtual void     ReleaseMem(CStringArray *pagFile);
	virtual LPCTSTR  GetLastErrMsg();
	virtual BOOL	 SplitDspZ2File(LPCTSTR pszSrcFile,LPCTSTR lpTmpDspZfile,LPCTSTR lpOtherfile);
	BOOL	 RemoveDspZFile(LPCTSTR pszSrcFile,LPCTSTR lpDestFile);
	BOOL     OverWtireDspZ(LPCTSTR pszSrcFile,LPCTSTR lpNewDspfile,LPCTSTR lpDestFile);
	void     GetLimitParam(BZP_PARAM_T &param);

	BOOL	 RemovePartiton(LPCTSTR pszSrcFile,LPCTSTR lpDestFile,int nTblIndex);
	BOOL     OverWtirePartiton(LPCTSTR lpszSrcFile,LPCTSTR lpNewfile,int nExtIndex,LPCTSTR lpDestFile);
	BOOL	 IsExtendPtnTbl(LPCTSTR lpszSrcFile);
private:
	BOOL     Bzp(CStringArray &agFile,LPCTSTR pszBzpFile,BOOL bExtZ = TRUE);
	BOOL     Merge(CStringArray &agFile, LPCTSTR pszXFile);
	BOOL     MergeEx(LPCTSTR pszMidBzpFile,LPCTSTR pszExtBzpFile, LPCTSTR pszXFile);
	BOOL	 GenExtBzp(CStringArray &agFileEx,DWORD dwTmpBzpSize, LPCTSTR pszXFile);
	BOOL	 ResOverMerge(LPCTSTR pszNewBzpFile,LPCTSTR pszSplitFile,DWORD dwOrgLen, LPCTSTR pszBzpFile);
	BOOL     Merge2(CStringArray &agFile, LPCTSTR pszXFile);
	BOOL     Cmp(CStringArray &agFile,LPCTSTR pszBzpFile);

	BOOL     BzpUncompress(LPCTSTR pszDstDir,CStringArray &agFile,BOOL bCheckRes = TRUE);
	BOOL     MergeUncompress(LPCTSTR pszDstDir,CStringArray &agFile);

	BOOL     GetOrgData(LPCTSTR lpszFile,LPBYTE &pOrgBuf, DWORD &dwOrgSize);
	BOOL     GetCmpData(LPCTSTR lpszFile,DWORD &dwOrgSize,LPBYTE &pCmpBuf, DWORD &dwCmpSize, DWORD &dwLangID);
	BOOL     GetCmpDataExt(LPCTSTR lpszFile,DWORD &dwOrgSize,LPBYTE &pCmpBuf, DWORD &dwCmpSize, DWORD &dwLangID);

	BOOL     CompressData(LPBYTE &pDstBuf, DWORD &dwDstSize,LPBYTE pSrcBuf, DWORD dwSrcSize,int nCmpType, int nLevel);
	BOOL     UnCompressData(LPBYTE &pDstBuf, DWORD &dwDstSize,LPBYTE pSrcBuf, DWORD dwSrcSize,int &nCmpType, int &nLevel);

	BOOL     Zip(LPBYTE &pDstBuf, DWORD &dwDstSize,LPBYTE pSrcBuf, DWORD dwSrcSize,int nLevel);
	BOOL     UnZip(LPBYTE &pDstBuf, DWORD &dwDstSize,LPBYTE pSrcBuf, DWORD dwSrcSize);
	
	BOOL     Lzma(LPBYTE &pDstBuf, DWORD &dwDstSize,LPBYTE pSrcBuf, DWORD dwSrcSize,int nLevel,int nCmpType);
	BOOL     UnLzma(LPBYTE &pDstBuf, DWORD &dwDstSize,LPBYTE pSrcBuf, DWORD dwSrcSize,int nCmpType);


	void     InitParam();
	
	BOOL     SplitCmpFile(LPCTSTR lpszFile, DWORD &dwSrcSize,LPBYTE &pDstBuf, DWORD &dwDstSize, DWORD dwMaxPacSize,int nCmpType, int nLevel);

	BOOL     UnSplitCmpFile(LPCTSTR lpszFile, LPBYTE pBuf,DWORD dwSize,int &nCmpType, int &nLevel);

	void     CheckCmpLevel(int &nCmpType, int &nLevel);
	
	LPBYTE   GetMergeInfo(BOOL bBigEndian);
	
private:
	BZP_PARAM_T m_tParam;
	LPBYTE		m_pBzpBuf;
	DWORD		m_dwBzpSize;
	BOOL		m_bExtPtnTbl;
	
	_TCHAR m_szLastErrMsg[MAX_PATH*2];

};

#endif // !defined(AFX_BZIP_H__72292599_1D32_4FEA_B212_E79E91D98541__INCLUDED_)
