//***************************************************************************
//  This file defines all export functions and symbols
//***************************************************************************

/*============================================================================*
 *                          DLL exporting symbols
 *============================================================================*/
#ifndef _DSP_KERNEL_EXPORT_H
#define _DSP_KERNEL_EXPORT_H

#ifdef DSPKERNEL_EXPORTS
#define DSPKERNEL_API __declspec(dllexport)
#else
#define DSPKERNEL_API __declspec(dllimport)
#endif  // DSPKERNEL_EXPORTS

#include "../include/DiagChan.h"

#define TP_INVALID_INDEX 0xffffffff

#define TP_EQU          0
#define TP_LESS         1
#define TP_LESS_EQU     2
#define TP_GREAT        3
#define TP_GREAT_EQU    4
#define TP_LIKE         5
#define TP_UNLIKE		6
#define TP_FLAG_MAX_NUM 7
#define TP_LIKE_FLAG    '*'

#define TP_A_LESS_VALUE_LESS_B 7
#define TP_DESCRIPTION 8
		
#define TP_ALIAS_SIZE	(32)
#define TP_ADDR_SIZE 2 // byte
#define TP_DATA_SIZE 2 // byte
#define TP_ITEM_SIZE 4 // Test point item size

#define WM_CLEAR_STATE 0x1027
#define WM_REFLASH_STATUSBAR 0x1028
#define WM_SAVE_MARK 0x1029
#define WM_NOTICE_ENDIAN 0x1030
#define WM_DEV_HOUND	0x1031
#define MAX_LUA_DESCTRIPTION_LEN	512

#define SAFE_CLOSE_FILE(fp)  \
	do \
{\
    if (fp) \
    {\
	   fclose(fp) ; \
	   fp=NULL;\
	}\
} while(0) 


typedef enum
{
	E_COMMON_DSP,	//type = 0x9D, subtype=0x00,0x01,0x02
	E_LTE_DSP,		//type = 0x9D, subtype=0x10,0x11,0x12
	E_TG_DSP,		//type = 0x9D, subtype=0x20,0x21,0x22
	E_W_DSP,		//type = 0x9D, subtype=0x30,0x31,0x32
	E_AG_DSP		//type = 0x9D, subtype=0x40,0x41,0x42
};
enum 
{
    TOOL_CHANNEL_UART,
    TOOL_CHANNEL_SOCKET_CLIENT,
};

enum
{
	FILE_LOG_UNDES,
	FILE_LOG_ADDDES,
	FILE_ORG
};

enum
{
	PHONE_TIME,
	TOOL_TIME,
	D0A0_TIME
};

typedef struct _TP_FIND_DATA
{
	char  szAlias[TP_ALIAS_SIZE];
    WORD  wAddrFlag;
    WORD  wDataFlag;
    char  szAddrValue[TP_ADDR_SIZE * 2 + 2 + 1]; // "0xa7" or "a7"
    char  szDataValue[TP_DATA_SIZE * 2 + 2 + 1]; // "0x88" or "88"
	char  szDescription[256];

    DWORD dwFindCount;
    DWORD dwReserve1; 
    DWORD dwReserve2;
	_TP_FIND_DATA()
	{
		 memset(this,0,sizeof(_TP_FIND_DATA));
	}
} TP_FIND_DATA, *PTP_FIND_DATA;

#define GETTPADDR(x) (LOBYTE(LOWORD(x)) << 8 | HIBYTE(LOWORD(x)))
#define GETTPDATA(x) (LOBYTE(HIWORD(x)) << 8 | HIBYTE(HIWORD(x)))

typedef BOOL (WINAPI*PFCALLBACK)(int nAddr, int nData, LPSTR lpDesc, int nMatchMode);
BOOL WINAPI CBGetDescription(int nAddr, int nData, LPSTR lpDesc, int nMatchMode);

typedef void (WINAPI*PFCBLookUpDescription)(int nAddr, int nData,char* szDesCription,int nMaxLen = MAX_LUA_DESCTRIPTION_LEN);
void WINAPI CBLookUpDescription(int nAddr, int nData,char* szDesCription,int nMaxLen = MAX_LUA_DESCTRIPTION_LEN);

class ITPSet
{
public:
    virtual void   SetLogFilePath( char* lpszPath ) = 0;
	virtual void   SetLogFileName( char* pLogName ) = 0;	
    virtual void   SetMaxCount( ULONG lCount )      = 0;
    
	virtual void   SaveRecent(BOOL bOnlySaveRct )   = 0;

	virtual ULONG  GetTpCount() = 0;   
    virtual ULONG  GetTp(ULONG nIndex) = 0;    
    virtual WORD   GetTPAddr(ULONG nIndex) = 0;    
    virtual WORD   GetTPData(ULONG nIndex) = 0;    
    virtual DWORD  GetTPTime(ULONG nIndex) = 0;  
	    
	virtual void   Clear() = 0;
        
    virtual LPCSTR GetTpFlagDesc( int nFlag ) = 0;

    virtual ULONG  Find(PTP_FIND_DATA pFindData, DWORD nNum , DWORD* pStartPos, BOOL *pbSign) = 0;
    virtual ULONG  GetIndexByFindIdx( ULONG lIdx ) = 0;
	virtual DWORD  GetColorIndexByFindIdx(ULONG lIdx) = 0;
	virtual BOOL   GetSignedByFindIdx(ULONG lIdx) = 0;
    virtual void   ClearFind(void) = 0;

    virtual ULONG  Find(ULONG nStart,char* pAddr,char* pData,int bUp,LPCTSTR pDesc) = 0;    
    virtual ULONG  FindNext() = 0;    
    virtual ULONG  FindPre()  = 0;
	virtual void   SetCurveNum(int nCurveNum) = 0;

	virtual void   SetStartPos(DWORD dwStartPos) = 0;
	virtual void   SetEndPos(DWORD dwEndPos) = 0;
	virtual void   SetFilePreFix(LPSTR lpPreFix) = 0;
	virtual LPSTR  GetTPFileName() = 0;
	virtual void   SetTPDirectory(LPSTR lpDirectory) = 0; 
	virtual LPSTR  GetTPDirectory() = 0;
	virtual void WINAPI  SetCBFunc(PFCALLBACK Func) = 0;
	virtual void   SetLogSign(LPSTR lpSign) = 0;

};

class IDspKernel : public IProtocolObserver
{
public:

    // -----------------------------------------------------------------------------
    //      Open the UART,connect to the dsp
    // -----------------------------------------------------------------------------
   	virtual BOOL Connect(
        HWND   hWnd,                //handle of the window to receive message
        UINT uConnType,             // Communication type,maybe uart,socket,etc
        LPVOID lpOpenArgs,          // argument for open communicatin channel
        DWORD  dwMsgID = 0
        ) = 0; 
    
    // -----------------------------------------------------------------------------
    //      Close the UART,disconnect to the dsp and release resourses
    // -----------------------------------------------------------------------------
    virtual BOOL DisConnect() = 0;

    // -----------------------------------------------------------------------------
    // Load previous dsplogger file
    // -----------------------------------------------------------------------------
    virtual BOOL Load(                      // test points count read from file
        LPCTSTR lpFileName           //the destinate file name
        ) = 0;
    
    // -----------------------------------------------------------------------------
    // Export to text file
    // -----------------------------------------------------------------------------
    virtual BOOL Export(
        LPCTSTR lpszSrcFile,        // Source file full path name,if NULL,export current file
        LPCTSTR lpszDesFile,        // Destinate file full path name,can not be null
        LONGLONG llStart,           // From which to export
        LONGLONG llCount,           // How many test points to be exported
		DWORD	dwType				//file type, *.log, *.org....
        ) = 0;

	// -----------------------------------------------------------------------------
    // save tp to file
    // -----------------------------------------------------------------------------
    virtual BOOL SaveTP(
        LPCTSTR lpszDesFile        // Destinate file full path name,can not be null
		)=0;

    // -----------------------------------------------------------------------------
    // Start parsing and logging test points
    // -----------------------------------------------------------------------------
    virtual BOOL Start() = 0;


    // -----------------------------------------------------------------------------
    // Pause parsing and logging test points
    // -----------------------------------------------------------------------------
    virtual BOOL Pause() = 0;
    
    // -----------------------------------------------------------------------------
    // resume parsing and logging test points
    // -----------------------------------------------------------------------------
    virtual BOOL Resume() = 0;

    // -----------------------------------------------------------------------------
    // Stop parsing and logging test points
    // -----------------------------------------------------------------------------
    virtual BOOL Stop() = 0; 

    // -----------------------------------------------------------------------------
    // Clear all test points
    // -----------------------------------------------------------------------------
    virtual BOOL Clear() = 0;
    
    // -----------------------------------------------------------------------------
    // set dynamic display
    // -----------------------------------------------------------------------------
	virtual void SetDynDisplay(BOOL bDisplay)=0;

    // -----------------------------------------------------------------------------
    // return test point set pointer
    // -----------------------------------------------------------------------------
    virtual ITPSet* GetTpSet() = 0;
	
    // -----------------------------------------------------------------------------
    // Release IDspkernel object
    // -----------------------------------------------------------------------------
    virtual void Release() = 0;
	
	virtual void SetFrameHead(DWORD dwFrameHead) = 0;

	virtual void SetGenerateFlag(DWORD dwGenerateTPTime, BOOL bGenerateOrgFile) = 0;

	virtual void SetFilePreFix(LPSTR lpPreFix) = 0;

	virtual void SetLogFilePath( char* lpszPath ) = 0;

	virtual void SetPlaybackData(void* pData, DWORD dwSize, DWORD dwReserve=0) = 0;
	
	virtual void SetEndian( int nEndian ) = 0;

	virtual void EnableLogSN( BOOL bEnable = FALSE) = 0;
	virtual void SetUnFilterAddrFlag(BOOL bNotFilter = FALSE) = 0;
	
	virtual void SetMemSize(int nMemSize) = 0;

	virtual BOOL Is24HourGap()= 0;

	virtual void SetPlaybackDataEx(void* pData, DWORD dwSize, DWORD dwReserve=0,DWORD dwPCTime=0 ) = 0;

	virtual void SetDspLogType( int nType ) = 0;
	virtual void WINAPI  SetCBDescripTionFunc(PFCBLookUpDescription pFunc) = 0;

public:
	virtual int OnChannelEvent( unsigned int event,void* lpEventData ) = 0;
	virtual int OnChannelData(LPVOID lpData, ULONG reserved ) = 0;

};

// Export functions
DSPKERNEL_API IDspKernel* CreateDspKernel();

#endif