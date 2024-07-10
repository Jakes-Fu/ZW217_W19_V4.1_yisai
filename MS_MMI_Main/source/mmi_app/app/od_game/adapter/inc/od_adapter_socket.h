///////////////////////////////////////////////////////////////////////////////
/**
socket管理
*/
///////////////////////////////////////////////////////////////////////////////

#ifndef __OD_ADAPTER_SOCKET_H__
#define __OD_ADAPTER_SOCKET_H__

#include "od_type_def.h"

typedef OD_S32 OD_SOCKET2;

///////////////////////////////////////////////////////////////////////////////

//
typedef enum
{
	//返回给用户，有助于判断socket状态
	OD_SOCK2_SUCCESS            = 0,
	OD_SOCK2_ERROR              = -1,
	OD_SOCK2_BASE                = -100,
	OD_ERR2_SOCK_WOULDBLOCK,
	OD_ERR2_SOCK_SET_OPT_FAIL,
	OD_ERR2_SOCK_CONN_FAIL,
	OD_ERR2_SOCK_RECV_FAIL,
	OD_ERR2_SOCK_WAP_HEAD_NOT_COMPLETE,
	OD_ERR2_SOCK_TOO_LONG_SNDBUF_LEN,
	OD_ERR2_SOCK_RECV_DATA_CONTINUE,
	OD_ERR2_SOCK_CREATE,
	OD_ERR2_SOCK_LIMIT_RESOURCE,
	OD_ERR2_SOCK_GET_HOST_BYNAME_FAIL,
	OD_ERR2_SOCK_SEND_ERR,
	OD_ERR2_NO_SIM
}OD_SocketRetCode2;

typedef enum
{
	OD_SOCK2_NONE_STATUS    = 0,
	OD_SOCK2_NETINITIALIZING,
	OD_SOCK2_NETINITIALIZED,
	OD_SOCK2_HOSTGETTING,
	OD_SOCK2_HOSTGETTED,
	OD_SOCK2_CONNECTING,
	OD_SOCK2_CONNECTED,
	OD_SOCK2_SENDING,
	OD_SOCK2_SENT,
	OD_SOCK2_RECEIVING,
	OD_SOCK2_RECEIVED,
	OD_SOCK2_CLOSING,
	OD_SOCK2_CLOSED
}OD_SocketStatus2;

enum
{
	OD_SOCKET2_HTTP_WITH_HEAD        =0,
	OD_SOCKET2_HTTP_WITHOUT_HEAD,
	OD_SOCKET2_TCP
};


typedef enum
{
    OD_CBM2_OK                  = 0,      /**< success */
    OD_CBM2_ERROR               = -1,    /**< error */
    OD_CBM2_WOULDBLOCK          = -2, /**< would block */
    OD_CBM2_LIMIT_RESOURCE      = -3, /**< limited resource */
    OD_CBM2_INVALID_ACCT_ID     = -4, /**< invalid account id*/
    OD_CBM2_INVALID_AP_ID       = -5,  /**< invalid application id*/
    OD_CBM2_INVALID_SIM_ID      = -6, /**< invalid SIM id */
    OD_CBM2_BEARER_FAIL         = -7,  /**< bearer fail */
    OD_CBM2_DHCP_ERROR          = -8,    /**< DHCP get IP error */
    OD_CBM2_CANCEL_ACT_BEARER   = -9, /**< cancel the account query screen */
    OD_CBM2_DISC_BY_CM          = -10  /**< bearer is deactivated by the connection management */
} OD_cbm_result_error_enum2;


//定义发送缓冲区大小
#define OD_SND_BUFF_MAX2            (1536)

//定义同一时刻socket最大连接数
#define OD_MAX_SOCKET_NUM2     (3)

//定义接收回调函数
typedef OD_S32 (*OD_RECV_CALLBACK2) (OD_SOCKET2 s, OD_SocketStatus2 nStatus, OD_SocketRetCode2 nRetcode, OD_S8* pBuff, OD_U32 nLen);

typedef  void (*OD_GET_HOST_BYNAME_CALLBACK2)(OD_SocketRetCode2, const OD_U8*,OD_S32 socketID);

//定义连接参数结构体
typedef struct
{
	/* domain name. if this field is NULL, it wiil be egnored */
	OD_S8* domain;
	 /* sepcify server ip address. */
	OD_U8 pServerIP[4];
	/* port number */
	OD_U16 nPort;
	 /* callback handler. used to handle data receive and network event */
	OD_RECV_CALLBACK2 fun;
	 /** connection type:
		 OD_CONN_CMWAP
		 OD_CONN_CMNET
		 */
	OD_U16 connType;
}ODConnect_Parameter2;


///////////////////////////////////////////////////////////////////////////////
/**
函数说明：
	设置socket发送的数据的地址和长度
输入参数：
	OD_S8 pBuff，指明发送数据的地址
	OD_S32 nLen，指定发送数据的长度
返回：OD_S32,  =0返回成功， <0返回失败
*/
 OD_S32 Od_SocketSendData2(OD_SOCKET2 s, const OD_S8* pBuff, OD_S32 nLen);

///////////////////////////////////////////////////////////////////////////////
/**
函数说明：
	建立TCP短连接，如果设置发送数据缓冲区，则连接成功后异步发送数据。
	若想再次发送数据，则需要等待确保数据完全接收后重新设置发送数据。
输入参数：
OD_Connect_Parameter* param,  连接参数结构体
返回：OD_S32,  返回值为socket id，>=0成功， <0返回失败
*/
 OD_S32 Od_SocketStart2(ODConnect_Parameter2* param);


///////////////////////////////////////////////////////////////////////////////
/**
函数说明：
	关闭socket连接
输入参数：
	SOCKET s, 套接字id
返回：
*/
 void Od_SocketClose2(OD_SOCKET2 s);

/////////////////////////////////////////////////////////////////////////////
/*
初始化联网，联网前调用

*/
 void Od_SocketEnter2(void);

OD_S32 Od_ReceiveData2(OD_SOCKET2 nSocketID, OD_U8 * buf, OD_U32 len);


OD_BOOL Od_GetIsUseWifi2(void);

void Od_SetIsUseWifi2(OD_BOOL flag);
OD_U8* Od_GetApnInfo2(void);

void Od_BrwHandleSettingAccount(void);

void Od_SetApnProfile2(OD_BOOL flag);

OD_BOOL Od_GetLocalIp2(OD_U8 * myip);
#endif

