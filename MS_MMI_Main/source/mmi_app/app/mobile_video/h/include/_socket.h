//
//	_Socket.h
//
#ifndef	__SOCKET_API_H
#define	__SOCKET_API_H

#include "_Types.h"

#ifdef	__cplusplus
extern "C"
{
#endif

typedef _BOOLEAN (*SocketCallbackProc)(_HANDLE, _UINT, _UINT32, _UINT32, _VOID*);

extern _HANDLE Socket_Create(_INT,SocketCallbackProc , _VOID*);
extern _VOID Socket_Release(_HANDLE hHandle);
extern _BOOLEAN Socket_Connect(_HANDLE, _CHAR*, _UINT);
extern _VOID Socket_Close(_HANDLE);
extern _INT Socket_Receive(_HANDLE, _VOID*, _INT, _INT*);
extern _INT Socket_Send(_HANDLE, _VOID*, _INT, _INT*);
extern _BOOLEAN Socket_Select(_HANDLE);
extern _BOOLEAN Socket_GetHostByName(_CHAR*);

extern _BOOLEAN Socket_DNSConvert(_CHAR* urlName, _UINT nPort, _UINT nwk_account_id);
extern _BOOLEAN Socket_Connect_ByDSN(_HANDLE hSock, _CHAR* urlName, _UINT nPort, _UINT nwk_account_id);
extern _BOOLEAN Socket_Connect_Sucess(_HANDLE hSock);


typedef enum
{
	RMEAPP_IAP_CMNET,
	RMEAPP_IAP_CMWAP,
	RMEAPP_IAPCount
}TRMEAPP_IAP;

typedef enum
{
	ENetworkStatus_Starting,
	ENetworkStatus_OK,
	ENetworkStatus_Stopping,
	ENetworkStatus_Stop_OK,
	ENetworkStatus_Error,
	ENetworkStatusCount
}TNetworkStatus;

typedef _BOOLEAN (*NetworkCallbackProc)(_HANDLE hNetwork, _VOID* pvoid, _INT nNetworkstatus, _INT nPara);

extern _HANDLE Network_Init(NetworkCallbackProc, _VOID* pvoid);
extern _BOOLEAN  Network_Release(_HANDLE);

extern _BOOLEAN Network_Start(_HANDLE, TRMEAPP_IAP);
extern _BOOLEAN Network_Stop(_HANDLE);
extern _BOOLEAN Network_IsValid();

#ifdef	__cplusplus
}
#endif

#endif
