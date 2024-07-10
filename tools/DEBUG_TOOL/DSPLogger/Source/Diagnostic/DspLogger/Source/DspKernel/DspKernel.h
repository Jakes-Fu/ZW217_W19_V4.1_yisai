/*============================================================================*
 *
 *                          Copyright (c) 2002 Spreadtrum
 *                              All Rights Reserved
 *
 *-----------------------------------------------------------------------------
 *
 * File name:
 *      DspKernel.h
 *
 * Description:
 *      DspKernel.dll provides the kernel debugging functions to communicate 
 *      with the DSP module. All of these operations are encapsulated into the
 *      class CDspKernel.
 *
 *      This header file provides the declarations of all the exported functions
 *      of CDspKernel
 *
 * Note:
 *      1. This DLL project is compiled in 4 bytes' alignment.
 *      2. It depends on the SciUart.dll.
 *
 *============================================================================*/


#ifndef __DSPKERNEL_H_
#define __DSPKERNEL_H_

#pragma warning( disable : 4244 )
/*============================================================================*
 *                          Header files
 *============================================================================*/
#include "IPort.h"
#include "TPSet.h"
#define __TCHAR_DEFINED
#include "atlbase.h"
#include "atlconv.h"
#include "../include/ChannelItf.h"
#include "../include/CommSock.h"
#include "ICommChannel.h"
#include "../include/DiagChan.h"
#include "TimerHelper.h"
#include "ProcessByte.h"

typedef enum
{
	FRAME_HEAD_TRUE,
	FRAME_HEAD_FALSE,
	FRAME_HEAD_PART_HIGH,
	FRAME_HEAD_PART_LOW
};

//teana hu 2009.05.15
typedef enum
{
	TP_DATA,
	IQ_MEMORY_DATA,
	DUMP_DATA,
	AUDIO_MEM_DATA,
	LOG_SN_INFO
};

#define FRAME_HEAD_SIZE 4

static BYTE DataFlag[] = {
	0x0,
	0x1,
	0x2,
	0xa,
	0xb
};

typedef enum
{
	FRAME_DATA_ASCEND,
	FRAME_DATA_DESCEND,
	FRAME_DATA_EQUAL,
	FRAME_DATA_A,
	FRAME_DATA_B
};

#define SwapDword(x) (DWORD)((x << 24) | ((x & 0x0000FF00) << 8) | ((x & 0x00FF0000) >> 8) | (x >> 24))

#define ITC_REQ_TYPE	209
#define ITC_REQ_SUBTYPE 100
#define ITC_REP_SUBTYPE 101

#define ITC_TOOL_NAME_LEN 32
typedef struct tag_ITC_Req_T
{
	DWORD dwReqType;	// Request type
	char szToolName[ITC_TOOL_NAME_LEN];	// target tool name
}ITC_Req_T;

typedef struct tag_ITC_Rep_T
{
	DWORD dwReqType;	// Request type
	DWORD dwRetCode;	// Error code
}ITC_Rep_T;

enum ITC_Req_Type_E
{
	Reg_Tool,
		Unreg_Tool,
		Query_Tool,
		Logel_ClearLog,
		Logel_SaveLog,
		Logel_Rec_keynote,
		Script_Resume,
		Query_Endian
};

#define TP_CACHE_NUM 600
/*============================================================================*
 *                          Declaration of CDspKernel
 *============================================================================*/

class CDspKernel:public IDspKernel
{
public:
	CDspKernel();
    virtual ~CDspKernel();//lint !e1510

//  =============================================================================
//  IDspKernel interface functions
//  =============================================================================
public:

    // -----------------------------------------------------------------------------
    //      Open the UART,connect to the dsp
    // -----------------------------------------------------------------------------
   	virtual BOOL Connect(
        HWND	hWnd,					//handle of the window to receive message
        UINT	uConnType,				// Communication type,maybe uart,socket,etc
        LPVOID	lpOpenArgs,				// argument for open communicatin channel
        DWORD	dwMsgID = 0
        ); 
    
    // -----------------------------------------------------------------------------
    //      Close the UART,disconnect to the dsp and release resourses
    // -----------------------------------------------------------------------------
    virtual BOOL DisConnect();
    
    // -----------------------------------------------------------------------------
    // Load previous dsplogger file
    // -----------------------------------------------------------------------------
    virtual BOOL Load(					// test points count read from file
        LPCTSTR lpFileName				// the destinate file name
        );

    // -----------------------------------------------------------------------------
    // Export to text file
    // -----------------------------------------------------------------------------
    virtual BOOL Export(
        LPCTSTR		lpszSrcFile,        // Source file full path name,if NULL,export current file
        LPCTSTR		lpszDesFile,        // Destinate file full path name,can not be null
        LONGLONG	llStart,			// From which to export
        LONGLONG	llCount,            // How many test points to be exported
		DWORD		dwType				//file type, *.log, *.org....
        );

	// -----------------------------------------------------------------------------
    // save tp to file
    // -----------------------------------------------------------------------------
    virtual BOOL SaveTP(
        LPCTSTR lpszDesFile				// Destinate file full path name,can not be null
		);

    // -----------------------------------------------------------------------------
    // Start parsing and logging test points
    // -----------------------------------------------------------------------------
    virtual BOOL Start();
    
    // -----------------------------------------------------------------------------
    // Pause parsing and logging test points
    // -----------------------------------------------------------------------------
    virtual BOOL Pause();
    
    // -----------------------------------------------------------------------------
    // Clear all test points
    // -----------------------------------------------------------------------------
    virtual BOOL Clear();
    
    // -----------------------------------------------------------------------------
    // resume parsing and logging test points
    // -----------------------------------------------------------------------------
    virtual BOOL Resume();
    
    // -----------------------------------------------------------------------------
    // Stop parsing and logging test points
    // -----------------------------------------------------------------------------
    virtual BOOL Stop(); 
    // -----------------------------------------------------------------------------
    //  set dynamic display
    // -----------------------------------------------------------------------------
	void SetDynDisplay(BOOL bDisplay);
    // -----------------------------------------------------------------------------
    // return test point set pointer
    // -----------------------------------------------------------------------------
    virtual ITPSet* GetTpSet();
    // -----------------------------------------------------------------------------
    // Release IDspkernel object
    // -----------------------------------------------------------------------------    
    virtual void Release();
	
	virtual void SetFrameHead(DWORD dwFrameHead);
	
	virtual void SetGenerateFlag(DWORD dwGenerateTPTime, BOOL bGenerateOrgFile);

	virtual void SetFilePreFix(LPSTR lpPreFix);

	virtual void SetLogFilePath( char* lpszPath );

	virtual void SetPlaybackData(void* pData, DWORD dwSize, DWORD dwReserve=0);

	virtual BOOL Is24HourGap();

	virtual void EnableLogSN( BOOL bEnable = FALSE) {m_bLogSN = bEnable;}
	virtual void SetUnFilterAddrFlag(BOOL bNotFilter = FALSE){m_bNotFilterAddr = bNotFilter;}

	virtual void SetPlaybackDataEx(void* pData, DWORD dwSize, DWORD dwReserve=0,DWORD dwPCTime=0 );
	virtual void SetDspLogType( int nType = 0 );

	virtual int	 OnChannelEvent( unsigned int event,void* lpEventData );
	virtual int	 OnChannelData(LPVOID lpData, ULONG reserved );
	virtual void WINAPI  SetCBDescripTionFunc(PFCBLookUpDescription pFunc);

public:
// -----------------------------------------------------------------------------
// Name:
//      Receive
//
// Function:
//      Receive a Dsp frame from the uart
// 
// Discription:
//      not return until finishing all the data or timeout
//      
// -----------------------------------------------------------------------------
	UINT Receive(
		void* pData,    //data pointer
		DWORD dwSize    //data block size
		);

	
// =============================================================================	
	HANDLE		m_hThreadEvent;

private:

// -----------------------------------------------------------------------------
// Name:
//      CreateScene
//
// Function:
//      Create the scene to be ready to receive the message
//      
// Note:
//      This function do not open uart
// -----------------------------------------------------------------------------
    BOOL CreateScene(
        HWND hWnd      //handle of the gui window to receive the message
                    );

// -----------------------------------------------------------------------------
// Name:
//      DestroyScene
//
// Function:
//      Destroy the scene created by CreateScene
//      
// -----------------------------------------------------------------------------
    BOOL DestroyScene();

// -----------------------------------------------------------------------------
// Name:
//      ConvertEndian
//
// Function:
//      little_endian and big_endian convert	
//      
// -----------------------------------------------------------------------------
	DWORD ConvertEndian(DWORD  t) 
	{ 
		DWORD tResult = 0; 
		for (int  i = 0; i < sizeof(DWORD); ++i) 
		{ 
			tResult <<= 8; 
			tResult |= (t & 0xFF) ; 			
			t >>= 8; 
		}
		return  tResult; 
	}
	void SetEndian( int nEndian )
	{
		m_nEndian = nEndian;
	}
	
	void SetMemSize(int nMemSize)
	{
		m_nMemDumpSize = nMemSize;
	}

	BOOL	ConnectToCS(LPBYTE lpOpenPara);
	BOOL	ConnectToChannel(DWORD dwThreadID, LPBYTE lpOpenPara);
	void	GeneratePreFix();
	void	GenerateTPDirectory();//Tp Folder name

	//teana hu 2009.11.26
	DWORD	AddFrameHead(LPBYTE lpSrc, DWORD dwSize);
	BOOL	IsDataFlag(BYTE bData);
	int		ProcessFrameData(LPBYTE lpSrc, DWORD dwSize);
	BOOL	ExeDiagCmd( BYTE type,BYTE subtype,LPBYTE lpData,DWORD dwLen,DWORD dwTimeout,PRT_BUFF** lppBuff );
	BOOL	QueryEndian();

	void	ProcessDataStream(LPBYTE lpSrc, DWORD dwSize, BOOL bPause = FALSE);
	void	ProcessToolTime(LPBYTE pSrcBuf, DWORD dwSize, CProcessByte& pb);
	void	ProcessPhoneTime(LPBYTE pSrcBuf, DWORD dwSize,CProcessByte& pb);
	void	ProcessD0A0Time(LPBYTE pSrcBuf, DWORD dwSize,CProcessByte& pb);
	void	ProcessOrgStream(LPBYTE pSrcBuf, DWORD dwSize, DWORD dwPCTime,BOOL bPause = FALSE);
	DWORD	GetTimeGap(DWORD dwStartTime,DWORD dwCurrentTime);
	void	AddFlagTime(BOOL bStart,DWORD dwPCTime =0);

	//teana hu 2011.03.18 for beijing test
	void	GetMemSizeFromRegister();
	BOOL	SaveMemFile(const BYTE* pData, DWORD dwDataLen);
	BOOL	SaveDumpFile(const BYTE* pData, DWORD dwDataLen);
	BOOL	SaveAudioFile(const BYTE* pData, DWORD dwDataLen);
	void	ProcessTpPakge(const BYTE* pData, DWORD dwDataLen);
	BOOL	LogSNFile(char* szLog);
	void	CloseDumpFile();
	void	FlushCacheData(int nFileType);
	void	GenerateSaveFile(int nFileType = IQ_MEMORY_DATA);
private:
    //Handle of gui window
    HWND	m_hGuiWnd;
    //message thread handle
    HANDLE	m_hThread;
	//message id
	UINT	m_dwMsgId;
    UINT	m_dwGuiMsgId;
	//message thread id
	UINT	m_dwThreadId;
	//connect type
	UINT	m_uChanType;
	
	//tag of the uart ready status
    BOOL	m_bPause;
	BOOL	m_bDynDisplay;
	BOOL	m_bLogSN;
	BOOL	m_bNotFilterAddr;
	
    CMassTPSet			m_TPSet;
    CTimerHelper		m_Timer;
	ICommChannel*		m_pPort;
	IProtocolChannel*	m_pChannel;

	BYTE	m_FrameHeadBuf[FRAME_HEAD_SIZE];
	DWORD	m_dwFrameHead;
	DWORD	m_dwFrameHeadCount;
	DWORD	m_dwGenerateTPTime;
	DWORD	m_dwD0A0Time;
	WORD	m_wD0A0[2];
	DWORD	m_dwTPCache[TP_CACHE_NUM];

	BOOL	m_bGenerateOrgFile;
	BOOL	m_bD0A0;
	FILE*	m_fpDump;
	FILE*	m_fpMemory;
	FILE*	m_fpAudio;
	FILE*	m_fpLogSN;
	char	m_szPrefix[MAX_PATH];
	char	m_szFilePath[MAX_PATH];
	char	m_szMemoryFile[MAX_PATH];
	char	m_szDumpFile[MAX_PATH];
	char	m_szAudioFile[MAX_PATH];
	char	m_szTPDirectory[MAX_PATH];
	char	m_szLogSign[MAX_PATH];

	char	m_szLogSNFile[MAX_PATH];
	
	int		m_nTPCacheIndex;
	int		m_nDsplogType;
	int		m_nEndian;
	int		m_preBuf;
	int		m_nAddLen;
	
	//teana hu 2010.11.25
	BYTE	m_byCacheTP[4];
	DWORD	m_dwCacheTPNum;

	BYTE	m_byCacheMem[4];
	DWORD	m_dwCacheMemNum;

	BYTE	m_byCacheDmp[4];
	DWORD	m_dwCacheDmpNum;

	BYTE	m_byCacheAudio[4];
	DWORD	m_dwCacheAudioNum;

	//teana hu 2011.03.18 for beijing test
	int		m_nMemDumpSize;
	DWORD	m_dwMemSize;
	DWORD	m_dwDumpSize;
	DWORD	m_dwAudioSize;

	CHANNEL_ATTRIBUTE m_ca;

	DWORD	m_dwStartTime;
	DWORD	m_dwGap;

	//dump file lock
	CRITICAL_SECTION m_CSFile;

	PFCBLookUpDescription m_pDesCallBack;
};


#endif  // __DSPKERNEL_H_