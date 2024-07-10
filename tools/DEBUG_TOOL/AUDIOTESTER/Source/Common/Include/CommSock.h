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

#include "cmddef.h"
#include "Common.h"

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