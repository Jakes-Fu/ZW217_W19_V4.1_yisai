#ifndef _COMMSOCK__H_
#define _COMMSOCK__H_

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the COMMSOCK_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// COMMSOCK_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef COMMSOCK_EXPORTS
#define COMMSOCK_API __declspec(dllexport)
#else
#define COMMSOCK_API __declspec(dllimport)
#endif

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
   CALLBACK_THREAD0,
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

//主机信息
typedef struct WSHOSTINFO
{
   char   HostName[32];
   char   HostAddr[16];
   int    HostID;
}WSHOSTINFO,*PWSHOSTINFO;

//define communiction protocol header
typedef struct Protocol_Header_Tag
{
    DWORD        sequenceNum;    //Protocol Packet Sequence Number
    WORD        length;         //Protocol Packet Length
    BYTE        type;           //Protocol Packet type
    BYTE        subtype;        //Protocol Packet subtype
}Protocol_Header,*Protocol_Header_Ptr;

#define FIRST_IPADDRESS(x)  ((x>>24) & 0xff)
#define SECOND_IPADDRESS(x) ((x>>16) & 0xff)
#define THIRD_IPADDRESS(x)  ((x>>8) & 0xff)
#define FOURTH_IPADDRESS(x) (x & 0xff)


#define PROTOCOL_HEADER_SIZE sizeof(Protocol_Header)

#define     MSG_REG         99
#define     MSG_UNREG       100

#define WM_SCM_SENDDATA WM_USER + 201

typedef enum
	{
//		DIAG_TRACE_A          = 151,    /* trace message*/
        DIAG_LOG_A            = 152,    /* log message*/
        PS_L2_A,                        /* L2 layer anonymous message.*/
//		L1_IND_A,                       /* L1 anonymous message.*/
		IP_PACKET_A           = 154,
		DIAG_AT_A			  = 155,     /* AT Command message*/
		PPP_PACKET_A,
        DIAG_MOBILETV_DSP_LOG_F = 157  /* MobileTV DSP Log message */
	} anony_enum_type;


class WinSock_32;
////////////////////////////////////////////////////////////
class COMMSOCK_API CCommSock  
{
public:
	CCommSock();
	virtual ~CCommSock();

	//export function
public:
///////////////////////////////////////////////////////////  
//Init:
//Initialize Channel Manager,Set the data call-back mechanism
//
//PARAMETER:
//		pCallbackPara:The Address of the Callback mechaniskm structure
//      
//RETURN:
//		BOOL: TRUE/FALSE
///////////////////////////////////////////////////////////
   BOOL Init(PCALLBACKPARA pCallbackPara);

///////////////////////////////////////////////////////////  
//Open:
//Open channel manager at special mode WINSOCKET/UART/USB
//
//PARAMETER:
//		pOpenPara:The Address of the Open Parameter structure
//      
//RETURN:
//		BOOL: TRUE/FALSE
///////////////////////////////////////////////////////////
   BOOL Open(POPENPARA pOpenPara,BOOL bUsedTools = TRUE);

///////////////////////////////////////////////////////////  
//Close:
//Close Channel Manager Module
//
//PARAMETER:
//      NONE
//
//RETURN:
//      BOOL: TRUE/FALSE
///////////////////////////////////////////////////////////
   BOOL Close();

///////////////////////////////////////////////////////////  
//Reset:
//Reset Channel Manager Module
//
//PARAMETER:
//      NONE
//
//RETURN:
//      BOOL: TRUE/FALSE
///////////////////////////////////////////////////////////
   BOOL Reset();

///////////////////////////////////////////////////////////  
//Send:
//Send Data
//
//PARAMETER:
//      lpData  : The Address of data to be sent
//      DataSize: The length of the data to be sent
//      ClientID: The client identify that the data to be sent
//      MsgType: The Message Type
//RETURN:
//		BOOL: TRUE/FALSE
///////////////////////////////////////////////////////////
   BOOL Send(void *lpData, ULONG DataSize,int ClientID = 0, int MsgType = 1000);

/*********************************************************************
RegMsgType:
	This function is designed to register anonymous message type
	
	Parameter:
      ClientID:    :Identify the register 
	  eMsgType     :message type of the register 
	Return: 
	  TRUE         : Reset Succeed 
	  FALSE        : Reset False
*********************************************************************/
   BOOL RegMsgType(UINT iClientID,int MsgType);

/*********************************************************************
UnRegMsgType:
	This function is designed to register anonymous message type
	
	Parameter:
      ClientID:    :Identify the register 
	  MsgType     :message type of the register 
	Return: 
	  TRUE         : Reset Succeed 
	  FALSE        : Reset False
*********************************************************************/
   BOOL UnRegMsgType(UINT iClientID,int MsgType);

/*********************************************************************
COMMSOCK_GetClientInfo:
	This function is used to get client info the client infomation 

	Parameter:
      pWsHostInfo  : Address to be save host info
      piHostNum    : Be used to return host num

	Return: 
	  TRUE         : Reset Succeed 
	  FALSE        : Reset False
*********************************************************************/
   BOOL GetClientInfo(PWSHOSTINFO pWsHostInfo, int *piHostNum);

///////////////////////////////////////////////////////////  
//DestoryData:
//Destory data 
//
//PARAMETER:
//      ppData: the pointer points to the data buffer
//
//RETURN:
//		NONE
///////////////////////////////////////////////////////////
   void DestroyData(PCOMMONDATA *ppData);

private:
	//Callback Parameter
	CALLBACKPARA  m_CallbackPara;    
	//Open Parameter
	OPENPARA      m_OpenPara;        
               
	//Thread 
	HANDLE        m_hSCMThreadState;      //Internal Thread State Event
	HANDLE        m_hSCMThread;          //Internal Thread Handle
	DWORD         m_dwSCMThreadID;       //Internal thread ID
    
	//Class Winsock pointer
	WinSock_32   *m_clsWinSock32;
    
	int           m_iTimeOuts;

    //For Purpose
    BOOL          m_bUsedTools;
    
    //Added at 2002-
    char          *m_lpLastRemainDataBuf;
    int           m_nLastRemainDataSize;
private:
	//Thread
   	DWORD         SCMThreadFunc(WPARAM wParam,LPARAM lParam);//CMM thread body
	static DWORD  GetSCMThreadFunc(LPVOID lpParam);
	BOOL          CreateSCMThread();    //Create CMM thread
    BOOL          ProcessData(WPARAM wParam,LPARAM lParam);
};
#endif //_COMMSOCK__H_