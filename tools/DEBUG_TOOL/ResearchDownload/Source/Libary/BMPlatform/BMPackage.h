#ifndef _BMPKG_H__
#define _BMPKG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "DPDefine.h"

#define MAX_BM_PKG_SIZE     65536  // 64K
#define HDLC_HEADER         0x7e
#define MAX_PACKET_NUM      3

struct PACKAGE_INFO
{
	char nTotalPackages;
	char nValidPackages;
	short bInvalidLen : 1;
	short bNoTail : 1;
	short bCrcError : 1;
	short Reserved : 13;
};

// Boot Mode Package
struct BM_HEADER
{
	unsigned short type;
	unsigned short len;
};

struct BM_PACKAGE
{
	BM_HEADER header;
	void* data;
};

typedef DWORD (WINAPI *FuncLogDataPtr)(LPVOID, LPBYTE);

class CBMPkg
{
public:
	CBMPkg();
	virtual ~CBMPkg();

public:
	/**
	 *  Package input data to a protocol package
	 *  @param [in] lpInput		: pointer to DIAG_PACKAGE struct to be packaged                            
	 *  @param [in]	nInputSize	: size of lpInput, just the same as size of DIAG_PACKAGE
	 *  @param [out] lppMemBlock: Pointer to a buffer contains the package
	 *  @param [out] lpSize		: Pointer to a buffer contains size of the output package
	 *
	 *  @return 0 means success,otherwise return error code
	 */
	virtual int Package( BM_PACKAGE* lpPkg ,void* lpOutBuf,int* lpOutSize,BOOL bTransCode = TRUE);
	/**
	 *  Unpackage a block of buffer
	 *  @param [in] lpBuffer: input buffer pointer
	 *  @param [in] uSize	: input buffer size
	 *
	 *  @return the count of packages found
	 */
	virtual unsigned int Append( void* lpBuffer,unsigned int uSize );	 
	/**
	 *  Get packages found in Append
	 *  @param [out] lpPackage: pointer to a buffer to contain returned package pointers
	 *  @param [in]	size	  : size of lpPackage
	 *
	 *  @return the count of packages returned
	 */
    virtual void * GetPackages( );
	/**
	 *  Clear internal buffer
	 */
	virtual void  Clear();  

	virtual PACKAGE_INFO & GetError();

	virtual void SetCrc(BOOL bCrc);

	virtual void EnableCheckBaudCrc(BOOL bEanble);
	virtual int  GetCheckBaudCrcType();
    virtual BOOL IsNeed2ndEnumPort();
	virtual void DisableTransCode(BOOL bDisTransCode){ m_bDisableTransCode = bDisTransCode; }
    virtual void SetLogDataCallback(FuncLogDataPtr pFuncLogDataPtr, LPVOID lpPara);
    virtual BOOL IsLogType(){return m_bIsLogType;}
	
protected:	
	bool AddPackage();
	bool CheckLength();
	void Copy( unsigned char* lpData,int& nStartPos,const unsigned char* lpCopy,const int nSize,BOOL bTransCode = TRUE );	
	void LockPakcageList( bool bLock );
	void RsvDataByTransCode(void* lpBuffer,unsigned int uSize);
	void RsvDataByNotTransCode(void* lpBuffer,unsigned int uSize);

protected:
	enum BMPKG_Status
	{
		BPS_NoValidData,
		BPS_Header,
		BPS_LOTYPE,
		BPS_HITYPE,
		BPS_LOLEN,
		BPS_HILEN,
		BPS_RSVDATA,
		BPS_LOCRC,
		BPS_HICRC,
		BPS_Unpacking,
		BPS_Escape
	};

	unsigned char	    m_RingBuf[MAX_BM_PKG_SIZE];	// Internal buffer 
	unsigned int	    m_nDataSize;				// Internal data size
	BMPKG_Status        m_Status;					// Unpack status
	LPBYTE        		m_lpPkg;
	
	LPBYTE              m_Packages[MAX_PACKET_NUM];	// The unpacked packages
	int                 m_nCurPacketIndex;
	int                 m_nPacketCount;
	unsigned short		m_nDataLen;

	CRITICAL_SECTION	m_Section;
	PACKAGE_INFO		m_PackageInfo;
	unsigned int		m_nMinPackageSize; 
	volatile BOOL		m_bDisableTransCode;
	BOOL                m_bCrc;
	BOOL                m_bBigEndian;
	BOOL                m_bEnableCheckBaudCrc;
	int                 m_nCheckBaudCrc;
    char                m_szFDLVer[MAX_PATH];
    FuncLogDataPtr      m_FuncLogDataPtr;
    LPVOID              m_lpParam;
    BOOL                m_bIsLogType;

};


#endif // _DIAGPACKAGE_H__
