#ifndef  _MV_NET_INTERFACE_H
#define  _MV_NET_INTERFACE_H

#include "_Types.h"
#include "_Socket.h"

#ifndef INVALID_SOCKET
#define INVALID_SOCKET	((long) (-1))
#endif
#if 0
typedef enum tagSockworkStatus
{
	Msg_Socket_DNS_OK,
	Msg_Socket_DNS_Error,
	Msg_Socket_Connect,
	Msg_Socket_Close,
	Msg_Socket_OnReceive,
	Msg_Socket_OnSend,
	Msg_Socket_OnError
}SockworkStatus;
#endif
typedef enum tagSockworkStatus
{
	Msg_Socket_Connect,	
	Msg_Socket_Close,	
	Msg_Socket_OnReceive,	
	Msg_Socket_OnSend,
	Msg_Socket_BufFree,
	Msg_Socket_OnError=99,
	Msg_Socket_OnIdle,
	Msg_Socket_DNS_OK,	
	Msg_Socket_DNS_Error,	
}SockworkStatus;


typedef struct tagSocketCallback{
		SocketCallbackProc  CM_SocketCallback;
		int pSocketParam;
}SocketCallback;

typedef struct tagNetWorkCallback{
	  NetworkCallbackProc  pNetworkCallbackProc;
	   int pNetworkparam;
}NetWorkCallback;

_HANDLE CM_Socket_Create(
										_UINT nSocketPort, 
										_INT nSOcketType,
										SocketCallbackProc pCallbackProc, 
										_VOID* pCallbackProcParam
);

PUBLIC void MV_socket_task(uint32 argc, void* argv);

#define MV_USERNAME_MAX_LEN  		256
#define MV_APN_MAX_LEN                     128

#define MV_APN_CMWAP_INFO    		"cmwap"
#define MV_APN_CMNET_INFO                "cmnet"

_HANDLE CM_Network_Init(NetworkCallbackProc pCallbackProc, _VOID* pVOID);

_BOOLEAN CM_Socket_Release(_HANDLE hSocket);

_BOOLEAN CM_Socket_Connect(_HANDLE hSOcket, _CHAR*dwIPv4Addr, _UINT nHostPort);

_BOOLEAN CM_Socket_Close(_HANDLE hSOcket);

_INT CM_Socket_Receive(_HANDLE hSocket, _VOID* pBuf, _INT nBufSize);

_INT CM_Socket_Send(_HANDLE hSocket, _VOID* pBuf, _INT nBufSize);
			
_BOOLEAN CM_Network_Release(_HANDLE hNetwork);

_BOOLEAN CM_Network_Start(_HANDLE hNetwork, TRMEAPP_IAP nIAP);

_BOOLEAN CM_Network_Stop(_HANDLE hNetwork);

_INT CM_GetNetworkStatus(_HANDLE hNetwork);


/*****************************************************************************/
//  Description : MV_ActivePdpContext
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_ActivePdpContext(void);

/*****************************************************************************/
//  Description : MV_DeactivePdpContext
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_DeactivePdpContext(void);
#endif