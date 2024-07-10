#ifndef __GLOBAL__H_
#define __GLOBAL__H_

//***************************************************************************
//  Reference List
//  2003.1.27       Used in DLoader
//  2003.2.20       Used in NVEdit
//  2003.2.20       Used in Calibration
//***************************************************************************

#define  CONVERT_SHORT(Src,Dst) {(Dst) = MAKEWORD(HIBYTE(Src),LOBYTE(Src));}


#define  CONVERT_INT(Src,Dst)   {\
                                 (Dst)  = MAKELONG(MAKEWORD(HIBYTE(HIWORD(Src)),LOBYTE(HIWORD(Src))),\
                                                   MAKEWORD(HIBYTE(LOWORD(Src)),LOBYTE(LOWORD(Src))));\
                                }

#define SAFE_DELETE(p)  \
	do \
{\
    if (p != NULL) \
    {\
	delete p; \
	   p=NULL;\
	}\
} while(0) 

#define SAFE_DELETE_ARRAY(p)  \
	do \
{\
    if (p != NULL) {\
	delete []p; \
	   p=NULL;\
	}\
} while(0) 

#define SAFE_CLOSE_HANDLE(p)  \
	do \
{\
    if (p != NULL) \
    {\
	CloseHandle(p) ; \
	   p=NULL;\
	}\
} while(0) 

#define SAFE_UNMAP_FILE(p)  \
	do \
{\
    if (p != NULL) \
    {\
	   ::UnmapViewOfFile( p ) ; \
	   p=NULL;\
	}\
} while(0) 


#define INVALID_THREAD_VALUE    0xFFFFFFFF
#define INVALID_CH_NO           0xFFFFFFFF

#define FACTORY_MODE            0x2

//For Message
#define MSG_SHOW_LOG            WM_USER + 1
#define MSG_LINE_STATE          WM_USER + 2
#define MSG_DL_LOG              WM_USER + 3

#define MSG_CH_OPEN             WM_USER + 4
#define MSG_CH_CLOSE            WM_USER + 5
#define MSG_CH_STATUS           WM_USER + 6
#define MSG_DL_CONNECT          WM_USER + 7
#define MSG_DL_CHECKBAUD        WM_USER + 8

#define MSG_START_DOWNLOAD      WM_USER + 17
#define MSG_CONTINUE_DOWNLOAD   WM_USER + 18
#define MSG_END_DOWNLOAD        WM_USER + 19

//For Channel

#define CH_CLOSED               0
#define CH_ACTIVE               1
#define CH_INACTIVE             2
#define CH_ERROR                3
#define CH_ACTIVE_CLR           RGB(0,170,0)
#define CH_INACTIVE_CLR         RGB(190,190,190)
#define CH_ERROR_CLR            RGB(255,0, 0)

#define FLAG_BYTE               0x7E
#define DEFAULT_BAUDRATE			115200
#define DEFAULT_HOST_BAUDRATE		921600
#define CHANGE_BAUD_NOT_AVAILABLE       0
#define NOT_CHANGE_BAUD_MODE            0                
#define MAX_REP_ID_LEN          36  //wchar length, total 72 bytes
#define SPARSE_HEADER_MAGIC	0xed26ff3a

//For Global
typedef struct SENDER_TO_VIEW_TAG
{
    int     CH_ID;             //Channel Identify
    int     CH_State;          //Channel State
    LPVOID  lpDLComm;          //Pointer of DLComm
    LPVOID  lpLogView;         //Pointer of log View
}SENDER_TO_VIEW, *SENDER_TO_VIEW_PTR;

typedef struct MSG_LOG_TAG
{
    int     iMsgCode;
    int     iDestination;
}MSG_LOG, *MSG_LOG_PTR;

//For Timer
#define  CONNECT_TIMER                   2000
#define  CONNECT_TIME_INTERVAL           200

//For Send
#define   WAIT_TIME                      5000

#define MAX_INFO_SIZE   0x100
#define WM_SEND_FILE    WM_USER + 1

typedef enum CMD_PKT_TYPE
{
    BSL_PKT_TYPE_MIN,                     /* the bottom of the DL packet type range */
    
    /* Link Control */    
    BSL_CMD_CONNECT = BSL_PKT_TYPE_MIN,   /* 0x00  
    
    /* Data Download */
    BSL_CMD_START_DATA,                   /* 0x01 the start flag of the data downloading  */
    BSL_CMD_MIDST_DATA,                   /* 0x02 the midst flag of the data downloading  */
    BSL_CMD_END_DATA,                     /* 0x03 the end flag of the data downloading */
    BSL_CMD_EXEC_DATA,                    /* 0x04 Execute from a certain address */
    /* End of Data Download command*/
    
    BSL_CMD_NORMAL_RESET,                 /* 0x05 Reset to normal mode */
    BSL_CMD_READ_FLASH,                   /* 0x06 Read flash content */
    BSL_CMD_READ_CHIP_TYPE,               /* 0x07 Read chip type */
    BSL_CMD_READ_NVITEM,				  /* 0x08 Lookup a nvitem in specified area */
    BSL_CMD_CHANGE_BAUD,                  /* 0x09 Change baudrate */
    BSL_CMD_ERASE_FLASH,                  /* 0x0A Erase an area of flash */
    BSL_CMD_REPARTITION,                  /* 0x0B Repartition nand flash */
    BSL_CMD_READ_FLASH_TYPE,              /* 0x0C Read flash type */
    BSL_CMD_READ_FLASH_INFO,              /* 0x0D Read flash infomation */
    BSL_CMD_READ_SECTOR_SIZE = 0xF,       /* 0x0F Read Nor flash sector size*/
    BSL_CMD_READ_START =0x10,             /* 0x10 Read flash start */
    BSL_CMD_READ_MIDST,                   /* 0x11 Read flash midst */
	BSL_CMD_READ_END,                     /* 0x12 Read flash end */
    
    BSL_CMD_KEEP_CHARGE		= 0x13,       /* keep charge */
	BSL_CMD_EXTTABLE		= 0x14,		  /* 0x14 Set ExtTable */
	BSL_CMD_READ_FLASH_UID	= 0x15,		  /* 0x15 Read flash UID */
	BSL_CMD_READ_SOFTSIM_EID= 0x16,		  /* 0x16 Read softSIM EID */
	BSL_CMD_POWER_OFF       = 0x17,		  /* 0x17 Power Off */
	BSL_CMD_CHECK_ROOT      = 0x19,       /* 0x19 Check Root */
    BSL_CMD_READ_CHIP_UID   = 0x1A,         /* 0x1A Read Chip UID */
    BSL_CMD_ENABLE_WRITE_FLASH  = 0x1B,     /* 0x1B Enable flash */
    BSL_CMD_ENABLE_SECUREBOOT   = 0x1C,     /* 0x1C Enable secure boot */   
	BSL_CMD_IDENTIFY_START		= 0x1D,     /* 0x1D Identify start */   
	BSL_CMD_IDENTIFY_END		= 0x1E,     /* 0x1E Identify end */   
	BSL_CMD_READ_CU_REF			= 0x1F,		/* 0x1F Read CU ref */ 
	BSL_CMD_READ_REFINFO		= 0x20,		/* 0x20 Read Ref Info */
	BSL_CMD_DISABLE_TRANSCODE	= 0x21,		/* 0x21 Use the non-escape function */
	BSL_CMD_WRITE_APR_INFO		= 0x22,		/* 0x22 Write pac file build time to miscdata for APR */
	BSL_CMD_CUST_DUMMY			= 0x23,		/* 0x23 Customized Dummy */
	BSL_CMD_READ_RF_TRANSCEIVER_TYPE	= 0x24,		/* 0x24 Read RF transceiver type */
	BSL_CMD_ENABLE_DEBUG_MODE	= 0x25,		/* 0x25 Enable debug mode */
    BSL_CMD_DDR_CHECK	    	= 0x26,     /* 0x26 DDR check */
    BSL_CMD_SELF_REFRESH		= 0x27,     /* 0x27 Self Refresh */
    BSL_CMD_ENABLE_RAW_DATA		= 0x28,     /* 0x28 Enable Raw Data */
    BSL_CMD_READ_NAND_BLOCK_INFO= 0x29,     /* 0x29 ReadNandBlockInfo */

    BSL_CMD_SET_FIRST_MODE		= 0x2A,     /* 0x2A Set First Mode */
    BSL_CMD_SET_RANDOM_DATA		= 0x2B,     /* 0x2B Set Random Data */

    BSL_CMD_SET_TIME_STAMP      = 0x2C,     /* 0x2C Set Time Stamp used for second hand memory check*/
    BSL_CMD_READ_PARTITION      = 0x2D,     /* 0x2D Read partition information from phone*/

    BSL_CMD_MIDST_RAW_START		= 0x31,     /* 0x31 Midst Raw Start */
    BSL_CMD_FLUSH_DATA  		= 0x32,     /* 0x2A Flush Data */
    BSL_CMD_MIDST_RAW_START2	= 0x33,     /* 0x33 Midst Raw Start for V2, only send once */
    BSL_CMD_ENABLE_UBOOT_LOG    = 0x34,     /* 0x34 Enable 0xFF type uboot log*/
	BSL_CMD_DUMP_UBOOT_LOG		= 0x35,		/* 0x35 request dump uboot log*/

	BSL_CMD_DISABLE_SELINUX		= 0x40,		/* 0x40 Disable Selinux*/

	/* valid : 0x2B == 0xB9 */
	
    BSL_CMD_CHECK_BAUD	    = FLAG_BYTE,	    /* CheckBaud command,for internal use */
    BSL_CMD_END_PROCESS     = 0x7F,             /* 0x7F End flash process */
	  /* End of the Command can be received by phone*/

    /* Start of the Command can be transmited by phone*/
    BSL_REP_TYPE_MIN = 0x80,

    BSL_REP_ACK = BSL_REP_TYPE_MIN,     /* The operation acknowledge */     
    BSL_REP_VER,                        /* 0x81 */

    /* the operation not acknowledge */
    /* system  */    
    BSL_REP_INVALID_CMD,                /* 0x82 */
    BSL_REP_UNKNOW_CMD,                 /* 0x83 */
    BSL_REP_OPERATION_FAILED,           /* 0x84 */

    /* Link Control*/    
    BSL_REP_NOT_SUPPORT_BAUDRATE,       /* 0x85 */

    /* Data Download */ 
    BSL_REP_DOWN_NOT_START ,            /* 0x86 */
    BSL_REP_DOWN_MULTI_START,           /* 0x87 */
    BSL_REP_DOWN_EARLY_END,             /* 0x88 */
    BSL_REP_DOWN_DEST_ERROR,            /* 0x89 */
    BSL_REP_DOWN_SIZE_ERROR,            /* 0x8A */
    BSL_REP_VERIFY_ERROR,               /* 0x8B */
    BSL_REP_NOT_VERIFY,                 /* 0x8C */

    /* Phone Internal Error */
    BSL_PHONE_NOT_ENOUGH_MEMORY,        /* 0x8D */
    BSL_PHONE_WAIT_INPUT_TIMEOUT,       /* 0x8E */

    /* Phone Internal return value */
    BSL_PHONE_SUCCEED,                  /* 0x8F */
    BSL_PHONE_VALID_BAUDRATE,           /* 0x90 */
    BSL_PHONE_REPEAT_CONTINUE,          /* 0x91 */
    BSL_PHONE_REPEAT_BREAK,             /* 0x92 */

    /* End of the Command can be transmited by phone*/
    BSL_REP_READ_FLASH,                 /* 0x93 */
    BSL_REP_READ_CHIP_TYPE,             /* 0x94 */
    BSL_REP_READ_NVITEM,                /* 0x95 */

    BSL_REP_INCOMPATIBLE_PARTITION,		/* 0x96 */ 
    BSL_REP_UNKNOWN_DEVICE,				/* 0x97 */
    BSL_REP_INVALID_DEVICE_SIZE,		/* 0x98 */
    BSL_REP_ILLEGAL_SDRAM,              /* 0x99 */
    BSL_WRONG_SDRAM_PARAMETER,          /* 0x9A */
    BSL_REP_READ_FLASH_INFO,            /* 0x9B */
    BSL_REP_READ_SECTOR_SIZE =0x9C,     /* 0x9C */
    BSL_REP_READ_FLASH_TYPE  =0x9D,     /* 0x9D */
	BSL_REP_READ_FLASH_UID	 =0x9E,     /* 0x9E */
	BSL_REP_READ_SOFTSIM_EID =0x9F,     /* 0x9F */

    /* information returned from FDL when downloading fixed NV */
    BSL_ERROR_CHECKSUM			= 0xA0,
    BSL_CHECKSUM_DIFF			= 0xA1,
    BSL_WRITE_ERROR				= 0xA2,
    BSL_CHIPID_NOT_MATCH		= 0xA3,
    BSL_FLASH_CFG_ERROR			= 0xA4,
    BSL_REP_DOWN_STL_SIZE_ERROR = 0xA5,
	BSL_REP_PHONE_IS_ROOTED	    = 0xA7,			/* 0xA7 Phone has been rooted */
    BSL_REP_SEC_VERIFY_ERROR    = 0xAA,			/* 0xAA Security data verify error */
    BSL_REP_READ_CHIP_UID       = 0xAB,			/* 0xAB Received Chip UID */
    BSL_REP_NOT_ENABLE_WRITE_FLASH	= 0xAC,		/* 0xAC Not enable to write flash */
    BSL_REP_ENABLE_SECUREBOOT_ERROR	= 0xAD,		/* 0xAD Enable secure boot fail */
	BSL_REP_IDENTIFY_START			= 0xAE,     /* 0xAE Identify start */   
	BSL_REP_IDENTIFY_END			= 0xAF,     /* 0xAF Identify end */ 
	BSL_REP_READ_CU_REF				= 0xB0,		/* 0xB0 Report CU ref  */
	BSL_REP_READ_REFINFO			= 0xB1,		/* 0xB1 Read Ref Info  */
	BSL_REP_CUST_DUMMY				= 0xB2,		/* 0xB2 Response Customized Dummy */
	BSL_REP_FLASH_WRITTEN_PROTECTION= 0xB3,		/* 0xB3 Flash written protection */
	BSL_REP_FLASH_INITIALIZING_FAIL	= 0xB4,		/* 0xB4 Flash initializing failed */
	BSL_REP_RF_TRANSCEIVER_TYPE		= 0xB5,		/* 0xB5 RF transceiver type */
	BSL_REP_DDR_CHECK_ERROR			= 0xB6,		/* 0xB6 DDR Check error */
    BSL_REP_SELF_REFRESH_ERROR  	= 0xB7,		/* 0xB7 Self Refresh error */
    BSL_REP_READ_NAND_BLOCK_INFO  	= 0xB8,		/* 0xB8 Read Nand Block Info rep */
    BSL_REP_RANDOM_DATA_ERROR       = 0xB9,     /* 0xB9 Set Random Data rep */
    BSL_REP_READ_PARTITION          = 0xBA,     /* 0xBA Read partition infor from phone */
	BSL_REP_DUMP_UBOOT_LOG			= 0xBB,		/* 0xBA Dump uboot log data */

    BSL_REP_UNSUPPROT_COMMAND       = 0xFE,     /* 0xFE Software has not supported this feature */
    BSL_REP_LOG                     = 0xFF,     //FDL can output some log info use this type

    BSL_PKT_TYPE_MAX = 0x100,

    BSL_UART_SEND_ERROR,
    BSL_REP_DECODE_ERROR,       // Decode or verify received buffer error
    BSL_REP_INCOMPLETE_DATA,     // Received buffer is not in the format we want
    BSL_REP_READ_ERROR,
    BSL_REP_TOO_MUCH_DATA,
    BSL_USER_CANCEL,
    BSL_REP_SIZE_ZERO,
	BSL_REP_PORT_ERROR,
    BSL_REP_OLD_MEMORY_REGISTER_CHANGE_ERROR,		/* Old Memory: Register Changed error */
    BSL_REP_OLD_MEMORY_EMMC_HAS_FLAG_ERROR,		    /* Old Memory: EMMC has flag error */
    BSL_REP_OLD_MEMORY_RPMB_WRITTEN_ERROR,		    /* Old Memory: RPMB written error */
    BSL_REP_OLD_MEMORY_REGISTER_EMMC_ERROR,		    /* Old Memory: Register Changed & EMMC has flag error */
    BSL_REP_OLD_MEMORY_REGISTER_RPMB_ERROR,		    /* Old Memory: Register Changed & RPMB written error */
    BSL_REP_OLD_MEMORY_EMMC_RPMB_ERROR,		        /* Old Memory: EMMC has flag & RPMB written error */
    BSL_REP_OLD_MEMORY_REGISTER_EMMC_RPMB_ERROR,	/* Old Memory: Register Changed & EMMC has flag & RPMB written error */
    BSL_REP_OLD_MEMORY_UNSUPPORT_ERROR,	        	/* Old Memory: Software has not supported this feature */
    BSL_ERROR_CRC_CHECK,
	BSL_REP_CREATE_READBACK_FILE_ERROR,	        	/* Failed to create readback file. */
    BSL_REP_96FMT_ERROR,                            /* The value formatted from 0x96 is invalid.*/
}pkt_type_s;

// V3
/*
typedef struct _DA_INFO_T_
{
	DWORD   dwVersion;
	BOOL    bDisableTransCode; 
    BYTE    bIsOldMemory;        // 前3个bit分别对应：1. 疑似寄存器改过; 2.疑似EMMC分区有标记值; 3.疑似RPMB 写过。
    BYTE    bSupportRawData;	
    BYTE    bReserve[2];	//Reserve
    DWORD   dwFlushSize;	//unit is M

    BOOL    bSupportRandomNum;
    BYTE    bRandomData[32];
    DWORD   dwReserve[51];	//Reserve

	_DA_INFO_T_()
	{
		memset(this,0,sizeof(_DA_INFO_T_));
		dwVersion = 3;
		bDisableTransCode = FALSE;
        bSupportRandomNum = FALSE;
	}
	void Clear()
	{
		memset(this,0,sizeof(_DA_INFO_T_));
		dwVersion = 3;
		bDisableTransCode = FALSE;
        bSupportRandomNum = FALSE;
	}
}DA_INFO_T,*PDA_INFO_PTR;
*/

#define DA_INFO_MAGIC       0x7477656E  //'newt'
#define TIME_STAMP_SIGN     0x7370      //'sp'

/// 0x96数据包的回复做成TLV格式，每个标识通过TLV形式回复，通过type进行区分
typedef enum E_DA_INFO_TYPE
{
    E_TYPE_DISABLE_TRANS_CODE = 0,      // bDisableTransCode
    E_TYPE_IS_OLD_MEMORY,               // bIsOldMemory
    E_TYPE_SUPPORT_RAW_DATA,            // bSupportRawData
    E_TYPE_FLUSH_SIZE,                  // dwFlushSize
    E_TYPE_RANDOM_DATA,                 // bRandomData
    E_TYPE_TIME_STAMP,                  // 新定义的time stamp类型，用于时间戳的读取
	E_TYPE_FLASH_TYPE,					// 返回flash type， 用于确定下载SPL文件
	E_TYPE_SUPPORT_DUMP_UBOOT_LOG,		// 返回是否支持Dump Uboot log功能
			
    E_TYPE_MAX = 0xFF
};

//V4
typedef struct _DA_INFO_T_
{
    DWORD   dwVersion;
    BOOL    bDisableTransCode; 
    BYTE    bIsOldMemory;        // 前3个bit分别对应：1. 疑似寄存器改过; 2.疑似EMMC分区有标记值; 3.疑似RPMB 写过。
    BYTE    bSupportRawData;	
    BYTE    bReserve[2];	//Reserve
    DWORD   dwFlushSize;	//unit is M

    DWORD   dwSupportRandomNum;  // 0: Not Support; 1: AES; 2: RSA2038
    BYTE    bRandomData[256];    // for AES, this only used 32 byte
    
    _DA_INFO_T_()
    {
        Clear();
    }

    void Clear()
    {
        memset(this,0,sizeof(_DA_INFO_T_));
        dwVersion = 4;
    }

}DA_INFO_T,*PDA_INFO_PTR;




typedef struct Packet_Header_Tag
{
    short   PacketType;
    short   DataSize;    
}Packet_Header,*P_Packet_Header;

//For Packet Struct
#define MAX_DATA_LEN       0x3000
#define PACHET_HDR_LEN     sizeof(Packet_Header)
#define MAX_PACKET_LEN     (2 * MAX_DATA_LEN) + PACHET_HDR_LEN
#define START_BSL_PKT_LEN   (PACHET_HDR_LEN + 2 * sizeof(DWORD))
#define ERASE_FLASH_PKT_LEN  (PACHET_HDR_LEN + 2 * sizeof(DWORD))  
#define READ_NVITEM_PKT_LEN  ( 2 * sizeof(DWORD) + sizeof( unsigned short) ) 
#define READ_NVITEM_REP_DATA_LEN   sizeof(DWORD) + sizeof( unsigned short )

#define PKT_TYPE_POS       0x00
#define PKT_LEN_POS        0x02
#define PKT_DATA_POS       0x04

#define BSL_PHONE                  1
#define BSL_WIN_TOOL               2

//#define MAX_MAP_SIZE      (0x4000000)  //64M
#define MAX_MAP_SIZE		(0x400000)  //4M
#define MAX_REF_INFO_LEN	(0x2000)	//8K

typedef struct BMFileInfo_TAG
{
    unsigned __int64 llCodeSize;    // File size
	unsigned __int64 llCodeOffset;  // File offset ,use for load data from the pac
    unsigned __int64 llBase;        // Where to download
    unsigned __int64 llOprSize;     
    LPVOID  lpCode;                 // File pointer
    _TCHAR  szFileType[MAX_PATH];
    _TCHAR  szFileName[MAX_PATH];   // File name
    BOOL    bLoadCodeFromFile;   
    
    DWORD   dwMaxLength;
    HANDLE  hFDLCode;
    HANDLE  hFDLCodeMapView;
    BOOL    bChangeCode; 
    BOOL    bNeedCloseFile;
	
	_TCHAR  szFileID[MAX_PATH];
	
	DWORD   dwFirstMapSize;

	_TCHAR  szRepID[MAX_REP_ID_LEN];
    DWORD   dwCheckSum;
    
    BMFileInfo_TAG()
    {
        memset(this,0,sizeof(BMFileInfo_TAG));
    }
}BMFileInfo, *PBMFileInfo;

// The time for a worker change a chip
#define CHANGE_CHIP_TIMEOUT    10000

typedef enum OPERATION_STATUS_MSG
{
    BM_BEGIN = WM_APP + 201,
    BM_FILE_BEGIN ,
    BM_OPERATION_BEGIN,
    BM_CHECK_BAUDRATE,
    BM_CONNECT,
    BM_ERASE_FLASH,
    BM_DOWNLOAD,
    BM_DOWNLOAD_PROCESS,
    BM_READ_FLASH,
    BM_READ_FLASH_PROCESS,
    BM_RESET,
    BM_READ_CHIPTYPE,
    BM_READ_NVITEM,
    BM_CHANGE_BAUD,
    BM_OPERATION_END,
    BM_FILE_END,
    BM_END,
	BM_UN_TRANSCODE,
    BM_SET_FIRST_MODE
}opr_status_msg;

//error

#define BM_S_OK                         ((DWORD)0x00000000L)

#define BM_S_FALSE                      ((DWORD)0x00000001L)

#define BM_E_FAILED                     ((DWORD)0x80046001L)

#define BM_E_UNEXPECTED                 ((DWORD)0x80046002L)

#define BM_E_OUTOFMEMORY                ((DWORD)0x80046003L)

#define BM_E_NOINTERFACE                ((DWORD)0x80046004L)

#define BM_E_INVALIDARG                 ((DWORD)0x80046005L)

#define BM_E_NOTIMPL                    ((DWORD)0x80046006L)

#define BM_E_OUTOFRANGE                 ((DWORD)0x80046007L)

#define BM_E_INVALID_OPERATION          ((DWORD)0x80046008L)

#define BM_E_OPR_NOTREG                 ((DWORD)0x80046009L)

#define BM_E_FILEINFO_ERROR             ((DWORD)0x80047001L)

#define BM_E_CREATETHREAD_FAILED        ((DWORD)0x80047002L)

#define BM_E_CHANNEL_FAILED             ((DWORD)0x80047003L)

#define BM_E_REG_OBSERVER_FAILED        ((DWORD)0x80047004L)

//error
#endif //__GLOBAL__H_