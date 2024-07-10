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
#define DEFAULT_BAUDRATE        115200
#define CHANGE_BAUD_NOT_AVAILABLE       0
#define NOT_CHANGE_BAUD_MODE            0                


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
    BSL_PKT_TYPE_MIN,                   /* the bottom of the DL packet type range */
   
    /* Start of the Command can be received by phone*/
    /* System */
    //BSL_CMD_RESET = BSL_PKT_TYPE_MIN,   /* reset system */
    //BSL_CMD_CFG,                        /* configure communicating protocol */
    //BSL_CMD_STOP,                       /* Terminate download */
    //BSL_CMD_VER_REQ,                    /* request version infomation */
    
    /* Link Control */    
    BSL_CMD_CONNECT = BSL_PKT_TYPE_MIN,
    //DL_CMD_LINE_REQ,                    /* link acknowledge */        
    

    /* Data Download */
    BSL_CMD_START_DATA,                  /* the start flag of the data downloading  */
    BSL_CMD_MIDST_DATA,                  /* the midst flag of the data downloading  */
    BSL_CMD_END_DATA,                    /* the end flag of the data downloading */
    //BSL_CMD_CHK_DATA,                    /* request verify the downloading data */
    BSL_CMD_EXEC_DATA,                   /* Execute from a certain address */
    /* End of Data Download command*/
    
    BSL_CMD_NORMAL_RESET,    
	BSL_CMD_READ_FLASH,
    BSL_CMD_READ_CHIP_TYPE,
    BSL_CMD_READ_NVITEM,
    BSL_CMD_CHANGE_BAUD,
    BSL_CMD_ERASE_FLASH,
    /* End of the Command can be received by phone*/
    
    
    /* Start of the Command can be transmited by phone*/
    BSL_REP_TYPE_MIN = 0x80,

    BSL_REP_ACK = BSL_REP_TYPE_MIN,      /* The operation acknowledge */     
    BSL_REP_VER,

    /* the operation not acknowledge */
    /* system  */    
    BSL_REP_INVALID_CMD,
    BSL_REP_UNKNOW_CMD,
    BSL_REP_OPERATION_FAILED,

    /* Link Control*/    
    BSL_REP_NOT_SUPPORT_BAUDRATE,

    /* Data Download */ 
    BSL_REP_DOWN_NOT_START ,
    BSL_REP_DOWN_MULTI_START,
    BSL_REP_DOWN_EARLY_END,
    BSL_REP_DOWN_DEST_ERROR,
    BSL_REP_DOWN_SIZE_ERROR,    
    BSL_REP_VERIFY_ERROR,
	BSL_REP_NOT_VERIFY,

    /* Phone Internal Error */
    BSL_PHONE_NOT_ENOUGH_MEMORY,    
    BSL_PHONE_WAIT_INPUT_TIMEOUT,

    /* Phone Internal return value */
    BSL_PHONE_SUCCEED,
    BSL_PHONE_VALID_BAUDRATE,    
    BSL_PHONE_REPEAT_CONTINUE,
    BSL_PHONE_REPEAT_BREAK,

    /* End of the Command can be transmited by phone*/
    BSL_REP_READ_FLASH,
    BSL_REP_READ_CHIP_TYPE, 
    BSL_REP_READ_NVITEM,

    BSL_PKT_TYPE_MAX = 0x100,

    BSL_UART_SEND_ERROR,
    BSL_REP_DECODE_ERROR,       // Decode or verify received buffer error
    BSL_REP_INCOMPLETE_DATA,     // Received buffer is not in the format we want
    BSL_REP_READ_ERROR,
    BSL_REP_TOO_MUCH_DATA,
    BSL_USER_CANCEL
}pkt_type_s;


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
#define READ_NVITEM_PKT_LEN  (PACHET_HDR_LEN + 2 * sizeof(DWORD) + sizeof( unsigned short) ) 
#define READ_NVITEM_REP_DATA_LEN   sizeof(DWORD) + sizeof( unsigned short )

#define PKT_TYPE_POS       0x00
#define PKT_LEN_POS        0x02
#define PKT_DATA_POS       0x04

#define BSL_PHONE                  1
#define BSL_WIN_TOOL               2

typedef struct BMFileInfo_TAG
{
    DWORD   dwCodeSize;             // File size
    DWORD   dwBase;                 // Where to download
    LPVOID  lpCode;                 // File pointer
    DWORD    dwOprSize;
    char    szFileType[MAX_PATH];
    char    szFileName[MAX_PATH];   // File name
    BOOL    bLoadCodeFromFile;   
    
    DWORD   dwMaxLength;
    HANDLE  hFDLCode;
    HANDLE  hFDLCodeMapView;
    BOOL    bChangeCode;        
    
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
    BM_END
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
//error
#endif //__GLOBAL__H_