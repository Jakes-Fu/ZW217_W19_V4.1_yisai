// SecBinPacImp.h: interface for the CSecBinPacImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SECBINPACIMP_H__49AB2537_868B_448B_9764_1FC539C2B629__INCLUDED_)
#define AFX_SECBINPACIMP_H__49AB2537_868B_448B_9764_1FC539C2B629__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SecBinPackApi.h"
#include "ErrorToString.h"
#include "ProcessMutex.h"

static const _TCHAR SZ_BINPAC_VER1[24] = _T("BP_R1.0.0");
static const _TCHAR SZ_BINPAC_VER2[24] = _T("BP_R2.0.1"); //support safe pac and 64bit size

#define MAX_RW_SIZE   (0xA00000)   //10M
#define MAX_BPH_VER_LEN (21)

class CSecBinPacImp  : public IBinPack
{
public:
	CSecBinPacImp();
	virtual ~CSecBinPacImp();
    virtual void SetSafeKey(DWORD dwKey = 0);
	virtual void SetPacSTVersion(LPCTSTR lpVersion); //_tsclen(lpVersion) <= 21
	virtual void SetReceiver(DWORD dwHandle, DWORD dwMsgID, BOOL bThread = TRUE);
	virtual BOOL Open(LPCTSTR lpszFileName, BOOL bOpenFlag = FALSE);
    virtual unsigned int Packet(const PBIN_PACKET_HEADER_T pbph,LPCTSTR lpszCfgFile, FILE_T* pFileArray,LPCTSTR lpszOrgPacFile = NULL,BOOL bSafeMode =FALSE);
	virtual unsigned int Unpacket(LPCTSTR lpszReleaseDirPath,BIN_PACKET_HEADER_T &bph,FILE_T** ppFileArray,BOOL bFactoryMode = FALSE);
	virtual void Close();
	virtual void ReleaseMem(FILE_T * paFile);
	virtual void GetReleaseDir(TCHAR* szReleaseDir, int nSize);
	virtual void GetConfigFilePath(TCHAR* szXMLFile, int nSize);
	virtual void Release();
    virtual BOOL IsExistEmptyFile(){ 
        return m_bIsExistEmptyFile; 
    };
    virtual void GetErrString(DWORD dwErrorCode,TCHAR* lpszOutString,int nSize);
	virtual BOOL IsMapFromPac(LPCTSTR lpDLFile);

	static DWORD WINAPI ThreadFunc(LPVOID pParam);

private:
	HWND	m_hRcvWindow;
	DWORD	m_dwRcvThreadID;
	DWORD	m_dwMsgID;
	BOOL	m_bThreadRcv;

	HANDLE	m_hFile;  
	CString	m_strPacFile;
	CString m_strReleaseDir;
	CString m_strCfgPath;
	CString m_strPacSTVer;
	BOOL	m_bIsExistEmptyFile;
    DWORD   m_dwEncryptKey;
	DWORD   m_dwLoadPolicy;
	DWORD   m_dwMaxDataLen;
	static DWORD  m_nCounter;
	CProcessMutex m_cLocker;
	CRITICAL_SECTION m_csLocker;
	HANDLE	m_hSparce2Img;
	HANDLE  m_hRead;
	UINT64 m_nSparce2ImgLen;
private:
	void PostMessageToUplevel(DWORD dwMsgID, WPARAM wParam, LPARAM lParam );
	BOOL GetSpcPrdConfig(LPCTSTR lpszCfgFile,LPBYTE *ppbText, DWORD &nCount);
	BOOL DeleteDirectory(LPCTSTR lpszDirName);
	void LoadSettings();
	

};

#endif // !defined(AFX_SECBINPACIMP_H__49AB2537_868B_448B_9764_1FC539C2B629__INCLUDED_)
