#ifndef _SEC_BINPACK_API_H_
#define _SEC_BINPACK_API_H_

/// µ¼³öºê
#ifdef SECBINPACK_EXPORTS
#define SECBINPACK_API extern "C" __declspec(dllexport)
#else
#define SECBINPACK_API extern "C" __declspec(dllimport)
#endif


#define _MAX_BLOCK_NUM   5
#define _MAX_NV_BACKUP_FALG_NUM 5
#define PAC_MAGIC       (0xFFFAFFFA)
#define PROG_BEGIN 0
#define PROG_PROCEED 1
#define PROG_END 2
#define WM_BINPAC_PROG_MSG             (WM_USER + 1033)
const int MAX_FLAG_LEN = 5;
const TCHAR FLAG_FILE_SUFFIX[MAX_FLAG_LEN+1] = _T(".flag");


#define PUMP_MESSAGES()  \
    do \
{\
    MSG msg;\
    while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))\
{\
	TranslateMessage(&msg);\
	DispatchMessage(&msg);\
}\
} while(0) 

#pragma pack(push,4)
struct _X_NV_BACKUP_FLAG_T
{
	_TCHAR szFlagName[MAX_PATH];
	DWORD  dwCheck;
};
struct _X_NV_BACKUP_ITEM_T
{
	_X_NV_BACKUP_ITEM_T()
    {
        memset(this,0,sizeof(_X_NV_BACKUP_ITEM_T));
    }
	_TCHAR szItemName[MAX_PATH];
	WORD   wIsBackup;
	WORD   wIsUseFlag;	
	DWORD  dwID;	
	DWORD  dwFlagCount;
	_X_NV_BACKUP_FLAG_T nbftArray[_MAX_NV_BACKUP_FALG_NUM];
};

/************************************************************************/
/* FILE_T struct storing file information                               */
/************************************************************************/

typedef struct _FILE_T
{
	_FILE_T()
	{
		memset(this,0,sizeof(_FILE_T));
		dwSize = sizeof(_FILE_T);
	}
	
    DWORD  dwSize;				// size of this struct itself
	_TCHAR szFileID[256];		// file ID,such as FDL,Fdl2,NV and etc.
	_TCHAR szFileName[256];     // file name,in the packet bin file,it only stores file name
	                            // but after unpacketing, it stores the full path of bin file
	_TCHAR szFileVersion[252];  // Reserved now; V1->V2 : 256*2 --> 252*2
	DWORD  dwHiFileSize;        // hight file size
	DWORD  dwHiDataOffset;      // hight file size
	DWORD  dwLoFileSize;           // file size
	int    nFileFlag;           // if "0", means that it need not a file, and 
	                            // it is only an operation or a list of operations, such as file ID is "FLASH"
	                            // if "1", means that it need a file
	DWORD  nCheckFlag;          // if "1", this file must be downloaded; 
	                            // if "0", this file can not be downloaded;										
	DWORD  dwLoDataOffset;        // the offset from the packet file header to this file data
	DWORD  dwCanOmitFlag;		// if "1", this file can not be downloaded and not check it as "All files" 
								//   in download and spupgrade tool.
	DWORD  dwAddrNum;
	DWORD  dwAddr[5];
	DWORD  dwReserved[249];     // Reserved for future,not used now
}FILE_T/*, *PFILE_T*/;

/************************************************************************/
/* BIN_PACKET_HEADER_T struct storing packet header information         */
/************************************************************************/
typedef struct _BIN_PACKET_HEADER_T
{
	_BIN_PACKET_HEADER_T()
	{
		memset(this,0,sizeof(_BIN_PACKET_HEADER_T));
		dwMagic = PAC_MAGIC;
	}
	_TCHAR szVersion[22];		// packet struct version; V1->V2 : 24*2 -> 22*2
	DWORD  dwHiSize;			// the whole packet hight size;
	DWORD  dwLoSize;            // the whole packet low size;
	_TCHAR szPrdName[256];		// product name
	_TCHAR szPrdVersion[256];   // product version
	int    nFileCount;          // the number of files that will be downloaded, the file may be an operation
	DWORD  dwFileOffset;		// the offset from the packet file header to the array of FILE_T struct buffer
	DWORD  dwMode;
	DWORD  dwFlashType;
	DWORD  dwNandStrategy;
	DWORD  dwIsNvBackup;
	DWORD  dwNandPageType;
	_TCHAR szPrdAlias[100];    // product alias
	DWORD  dwOmaDmProductFlag;
	DWORD  dwIsOmaDM;
	DWORD  dwIsPreload;
	//Added start
	WORD   wEncrypted;
	WORD   wFTCRC;
	DWORD  dwFTOrgSize;
	DWORD  dwFTEncryptedSize;
	//Added end
	DWORD  dwReserved[197];	
	DWORD  dwMagic;
	WORD   wCRC1;
	WORD   wCRC2;
}BIN_PACKET_HEADER_T,*PBIN_PACKET_HEADER_T;
#pragma pack(pop)	
/************************************************************************/
/* Class CBinPack, it packets the bin files and unpackets the packet    */
/************************************************************************/


class IBinPack
{
public:
	virtual ~IBinPack() = 0;
    virtual void SetSafeKey(DWORD dwKey = 0) = 0;
    virtual void SetReceiver(DWORD dwHandle, DWORD dwMsgID, BOOL bThread = FALSE) = 0;

	/** Open bin packet file for read or write
	*
	* @param lpszFileName: packet file name
	* @param bOpenFlag: if true, open for read, otherwise open for write
	* @return: true,if open successfully;false,otherwise
	*/
	virtual BOOL Open(LPCTSTR lpszFileName, BOOL bOpenFlag = FALSE) = 0;
	
	/** Packet bin files into one file with certain struct
	*
	* @param pbph: the packet header
	* @param lpszCfgFile: the name of configure file (*.xml)
	* @param pFileArray: point to an array of FILE_T struct
	* @param bSafeMode:  if set SafeMode, pac is encrypted
	* @return: 0,if packet successfully;otherwise,packet fail
	*/
    virtual unsigned int Packet(const PBIN_PACKET_HEADER_T pbph,LPCTSTR lpszCfgFile, FILE_T * pFileArray,LPCTSTR lpszOrgPacFile = NULL,BOOL bSafeMode =FALSE) = 0;
	
	
	/** Unpacket the packet to bin files into one specified directory 
	*
	* @param lpszReleaseDirPath: the directory to release bin files
	*                            If it equals NULL, program will create the directory to system temp directory.
	*                            Suggest to set it null.
	* @param bph: store the packet header
	* @param ppFileArray: store the bin files information into FILE_T struct
	*                     it must be release by the "ReleaseMem" function
	* @param bFactoryMode: whether it is factory download mode
	* @return: 0,if unpacket successfully; otherwise,unpacket fail
	*/
	virtual unsigned int Unpacket(LPCTSTR lpszReleaseDirPath,BIN_PACKET_HEADER_T &bph,FILE_T ** ppFileArray,BOOL bFactoryMode = FALSE) = 0;
	
	/** Close the opened file
	* You can use it or not, for it will be invoked in the destruct function
	*
	*/
	virtual void Close() = 0;
	
	/** Release the memory newed by Unpacket function
	*
	* @param paFile: point to FILE_T buffer
	*/
	virtual void ReleaseMem(FILE_T * paFile) = 0;
	
	/** Get released directory
	*
	* @return: the path of released path
	*/
	virtual void GetReleaseDir(TCHAR* szReleaseDir, int nSize) = 0;
	
	/** Get xml configure file path
	*
	* @return: the path of xml configure
	*/
	virtual void GetConfigFilePath(TCHAR* szXMLFile, int nSize) = 0;

	virtual void Release() = 0;
    virtual BOOL IsExistEmptyFile()=0;
	virtual BOOL IsMapFromPac(LPCTSTR lpDLFile) = 0;
    virtual void GetErrString(DWORD dwErrorCode,TCHAR* lpszOutString,int nSize)=0;
	virtual void SetPacSTVersion(LPCTSTR lpVersion) = 0; //_tsclen(lpVersion) <= 21

};

SECBINPACK_API BOOL CreateSecPacParse( IBinPack** pSecPacParse );

#endif // _SEC_BINPACK_API_H_
