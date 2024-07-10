#ifndef _GCOMMON__H_
#define _GCOMMON__H_

//For Message
#define   MSG_HEAD_SIZE              3
#define   MSG_LENGTH_SIZE            2
#define   MSG_SERVER_HEAD            1
 
#define   MSG_BROADCAST              1000
#define   MSG_MAX_TYPE               1000
#define   MSG_SEND_TIMES             3

#define   MAX_INTERNAL_BUFFERS       10
#define   MAX_EXTERNAL_BUFFERS       10
#define   MAX_BUFFER_SIZE            4096
#define   RECEIVE_BUFFER_ERROR       -1
#define   WAIT_DATA_TIME             1000

//For CMM
#define   WM_CMM_SENDDATA            WM_USER + 101
#define   WM_CMM_CLOSE               WM_USER + 102


//For SCM
#define   WM_SCM_SENDDATA            WM_USER + 201
#define   WM_SCM_CLOSE               WM_USER + 202

#define   WS32_RECV_BUFFER_SIZE      65536            //define the receive buffer size


#define   WINSOCK_VERSION_MAJOR      2
#define   WINSOCK_VERSION_MINOR      0
#define   WS32_MSG_OPEN              WM_USER + 230
#define   WS32_MSG_CLOSE             WM_USER + 231
#define   WS32_MSG_DONE              WM_USER + 232


//For UARTCM
#define   WM_UARTCM_SENDDATA         WM_USER + 401
#define   WM_UARTCM_CLOSE            WM_USER + 402
#define   WM_UART_CLOSE              WM_USER + 431
#define   WM_UART_DONE               WM_USER + 432

#define UART_RECEIVE_DELAY           10               //Receive data from driver delay
#define UART_INQUEUE_SIZE            4096
#define UART_OUTQUEUE_SIZE           4096
#define ASCII_XON                    0x11
#define ASCII_XOFF                   0x13
#define RECEIVE_BUFFER_SIZE          65536 * 2           //define the receive buffer size
#define SEND_BUFFER_SIZE             65536            //define the send buffer size
#define BUSMONITOR_PORT              0x100

//For Channel Server
#define   WM_MSG_TIMEOUTS            WM_USER + 501


//this struct define the infomation is transmit between .dll and .exe
typedef struct _COMMONDATA
{
   int     MsgType;
   int     iRepID;
   LPBYTE  lpData;
   long    lDataLen;
}*PCOMMONDATA, COMMONDATA;

//Define CallBack Function
typedef void (*CALLBACKFUNCTION)(PCOMMONDATA); 

//Callback Mechanism Type
typedef enum CALLBACKFLAG
{
   CALLBACK_NONE,
   CALLBACK_EVENTS,
   CALLBACK_WINDOW,
   CALLBACK_THREAD,
   CALLBACK_FUNCTION
}CALLBACKFLAG;

//Callback Mechanism Strcture
typedef struct CALLBACKPARA
{
   CALLBACKFLAG       CallBackFlag;    //CALLBACK_EVENT/CALLBACK_WINDOW/CALLBACK_THREAD/CALLBACK_FUNCTION
   HANDLE             Handle;          //Handle of Event/Window
   DWORD              ThreadID;        //Identify o Thread
   CALLBACKFUNCTION   CallBackFun;     //Address of the CallBackFunction
}*PCALLBACKPARA,CALLBACKPARA;

//define physical medium type
typedef enum _CHANNELTYPE
{
   WINSOCKET,
   UART,
   USB,
}CHANNELTYPE;

//This enumrate type define the CommSock work mode
typedef enum _SCMTYPE
{
   CONNTYPE_NONE,
   CONNTYPE_SERVER,
   CONNTYPE_CLIENT,
}SCMTYPE;

//主机信息
typedef struct WSHOSTINFO
{
   char   HostName[32];
   char   HostAddr[16];
   int    HostID;
}WSHOSTINFO,*PWSHOSTINFO;

//define struct of Common Channel Sunsystem Open Para
typedef struct _OPENPARA
{
   CHANNELTYPE  ChannelType;
   SCMTYPE	SCMType;
   int          Port;
   char         IpAddress[16];
   int          Timeouts;
   int          BaudRate;
}*POPENPARA,OPENPARA;


//define struct of buffer transmitting between dll and exe/dll
typedef struct _COMMONBUFFER
{
   LPBYTE lpDataBuffer;       //pointer to the data buffer
   LONG   lDataSize;         //the data length 
   HANDLE hBufferEvent;      //the state of buffer,data ready or not 
}*PCOMMONBUFFER,COMMONBUFFER;


#define  CONVERT_SHORT(Src,Dst) {(Dst) = MAKEWORD(HIBYTE(Src),LOBYTE(Src));}


#define  CONVERT_INT(Src,Dst)   {\
                                 (Dst)  = MAKELONG(MAKEWORD(HIBYTE(HIWORD(Src)),LOBYTE(HIWORD(Src))),\
                                                   MAKEWORD(HIBYTE(LOWORD(Src)),LOBYTE(LOWORD(Src))));\
                                }


//define communiction protocol header
typedef struct Protocol_Header_Tag
{
    DWORD        sequenceNum;    //Protocol Packet Sequence Number
    WORD        length;         //Protocol Packet Length
    BYTE        type;           //Protocol Packet type
    BYTE        subtype;        //Protocol Packet subtype
}Protocol_Header,*Protocol_Header_Ptr;

#define PROTOCOL_HEADER_SIZE   sizeof(Protocol_Header)

// Tool intercomunication relative definitions
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
};

#endif //_GCOMMON__H_
