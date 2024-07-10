#include "od_adapter_socket.h"
#include "os_api.h"
#include "mn_type.h"
#include "app_tcp_if.h"
#include "socket_types.h"
#include "sig_code.h"
#include "socket_api.h"
#include "sci_api.h"
#include "od_adapter_log.h"
#include "od_adapter_gprs.h"
#include "od_adapter_nv.h"
#if defined(__OD_SC6530_SWITCH__) || defined(__OD_SC6531_SWITCH__) 

#include "mmipdp_export.h"
#include "mmiconnection_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#include "mmibrowser_setting.h"

#endif
#include "mmiset_export.h"
#else

#include "mmipdp_api.h"
#include "mmiconnection.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_api.h"
#endif
#include "mmiset.h"
#endif
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#define Od_GetGprsAccountData od_get_gprs_account_data
//#define OD_SOCKET2_WITH_THREAD
#define OD_CONNECT_PROTECT_TIME (10000)
#define OD_CONNECT_REPEAT_TIME  (1000)
#define OD_CONNECT_REPEAT_COUNT  (5)
#define OD_CREAT_SOCKET_ASYNC_TIME (100)
#define OD_APN_INFO_LEN (128)

#ifdef __OD_SC7701__
#define __OD_SC6800_PDA2_SWITCH__
#endif


typedef enum e_sock_msg_cb
{
    sock_cb_connected,
    sock_cb_send,
    sock_cb_recv,
    sock_cb_close,
    sock_cb_connreset,
    sock_cb_error
} e_msg_cb;

typedef struct
{
    OD_SOCKET2             socket_id;
    OD_S32                   handle;
    OD_S8*                g_RecBuffer;
    OD_S8*                g_SendBuffer;
    OD_S32                g_RecvBuffLen;
    OD_S32                g_SendBuffLen;
    OD_S32                g_SendBuffOffset;
    OD_BOOL                   IsHead ;
    OD_U16                connType ;
    OD_RECV_CALLBACK2     g_RecvHandler;
    OD_U8                 server_ip[4];
    OD_U16                server_port;
    OD_SocketStatus2       socket_status;
    OD_SocketRetCode2          socket_retcode;
    OD_S8                            domain[64];
    OD_GET_HOST_BYNAME_CALLBACK2     pf_GetHostByNameFun;
    OD_U8                   appID;

} socket_info;


typedef struct
{
    OD_U8                actived_net_type;

    OD_U8                wap_proxy_ip[4];
    OD_U16               wap_proxy_port;
    OD_U32               accountID;
} conn_setting_struct;

typedef struct
{
    SIGNAL_VARS
    OD_S32 data1;
    OD_S32 data2;
} OD_THREAD_SIG_T;



#define REQUEST_ID_BASE 567

//static EMA_INIT_NETWORK_CALLBACK pf_NetInitCallback;
static conn_setting_struct g_connSetting;
static OD_BOOL g_auto_gprs_flag = OD_FALSE;
static OD_BOOL g_od_apn_suc_flag = OD_FALSE;
//static OD_BOOL g_bNetInitialized = OD_FALSE;
static OD_BOOL ema_net_initialized = OD_FALSE;

static socket_info mysocket_free[OD_MAX_SOCKET_NUM2] = {0};
static const OD_S8                g_HttpOk[] = "HTTP/";
#ifdef MMI_WIFI_SUPPORT
static OD_BOOL s_wifi_flag=OD_NULL;
#endif

typedef void (*od_network_func_type)(OD_S32 result);

typedef enum
{
    OD_GPRS_DEACTIVE_OK = 0,   //DEACTIVE完成
    OD_GPRS_ACTIVE_OK,     //ACTIVE完成
    OD_GPRS_ACEIVE_FAILED,
    OD_GPRS_ACTIVING,
    OD_GPRS_DEACTIVEING        //DEACTIVE进行中
} e_od_gprs_state; //GPRS的状态

typedef struct
{
    OD_S16 connect_timer_id;
    OD_S16 connect_repeat_timer;
    OD_S16 connect_repeat_count;
    OD_S16 send_repeat_timer;

    OD_S16 rec_repeat_timer;

    OD_U8 account_id;
    OD_S32 connect_soc_id;
    OD_U32 pdp_protect_timer_id;
    od_network_func_type net_active_cb;
    e_od_gprs_state gprs_state;
    OD_S32 active_sim;
    OD_U32 net_id;
} t_od_gprs_cntx;

typedef enum
{
    OD_NETWORK_THREAD_MSG_MIN = 0x104,
    OD_NETWORK_THREAD_MSG_DNS = OD_NETWORK_THREAD_MSG_MIN,  //0x104
    OD_NETWORK_THREAD_MSG_SOC_NOTIFY, //0x105
    OD_NETWORK_THREAD_MSG_SOC_DISCONNECT, //0x106
    OD_NETWORK_THREAD_MSG_SOC_CONNECT, //0x107
    OD_NETWORK_THREAD_MSG_NETWORK_EXIT, //0x108
    OD_NETWORK_THREAD_MSG_MAX
} OD_NETWORK_THREAD_MSG_TYPE_E;







typedef struct
{
    OD_U8 ip[4];
    OD_S32 len;
    OD_S32 id;
} t_sh_dns_ip;

#define OD_NETWORK_SELECT_STACK_SIZE                    1024*8 //
#define OD_NETWORK_SELECT_QUEUE_NUM                     1

#define OD_NETWORK_SELECT_TASK_PRIORITY                31//(SCI_PRIORITY_IDLE)//29 //

#define SELECT_TIMEOUT   10//0x7fffffff    // unit:0.1s
#define SELECT_TIMEGAP   500    // unit:ms
#define SOCKET_SELECT_MAX_NUMBER OD_MAX_SOCKET_NUM2

#define OD_MAX_RECONNECT_TIMES  8
#ifdef  OD_SOCKET2_WITH_THREAD
static OD_U32 g_od_network_select_task_id = SCI_INVALID_BLOCK_ID;
#endif
static t_od_gprs_cntx g_od_gprs_cntx = {0};
static OD_BOOL g_exit_task_flag = OD_TRUE,s_local_apn_flag=OD_FALSE;
static OD_S8   s_socket_id=-1;
static OD_ACCOUNT_DATA_STRUCT s_apn_collect= {0};
static OD_U8 OdApnInfo[OD_APN_INFO_LEN]= {0};
static OD_BOOL is_roaming;
static void od_acitvie_gprs_with_sim(MN_DUAL_SYS_E dual_sys, OD_U8 account_id);
static OD_U32 od_network_select_task_create(void);
static void Od_NetFree(void);
static void od_pdp_active_cb(OD_S32 result);

static OD_S32 od_get_pdp_state(void);
static OD_S32 Od_SockSendInternal(OD_SOCKET2 nSocketID, OD_U8* pBuff, OD_U32 nBuffLen);
static void Od_SendSignaltoNetworkThread(
    OD_NETWORK_THREAD_MSG_TYPE_E type,
    BLOCK_ID task_id,
    OD_S32 data1,
    OD_S32 data2
);
static void od_impsock_polling(void* fd_send, void* fd_recv, void* fd_except);
static OD_S32 Od_GetFreeSlot_count(void);
static void od_network_select_task_main(OD_U32 argc, void * argv);
static OD_S32 Od_SocketConnectInternal(OD_SOCKET2 nSocketID, const OD_U8* pServerIP, const OD_U16 nPort);
static OD_U8  Od_ReadNVConfig(OD_U8* buffer,OD_U16 len);
static OD_U8  Od_WriteNVConfig(OD_U8* buffer,OD_U16 len);

static void Od_SetNetMode(void);
static void Od_NetInitialize(void );
static OD_BOOL Od_GetSocketHostByName(OD_S32 request_id, OD_GET_HOST_BYNAME_CALLBACK2 pCallback);
static void  socket_get_host_name_callback(OD_SocketRetCode2 ret, const OD_U8* pServerIP,OD_S32 socketID);
static void Od_ResetAndDeactivePdpContext(OD_U8 sim);
static void Od_socket_notify_internal(OD_S32 socket_id, e_msg_cb msgId);
static OD_S32 Od_Mysocket_find_free_slot(void)
{
    OD_S32 i = 0;
    for (i = 0; i < OD_MAX_SOCKET_NUM2; i++)
    {
        if (mysocket_free[i].socket_id < 0)
        {
            break;
        }
    }
    return i;
}




static OD_S32 Od_SocketCreateInternal(OD_S32 i)
{
    OD_SOCKET2 hTcpSocket = -1;

    hTcpSocket = sci_sock_socket(AF_INET, SOCK_STREAM, 0,g_od_gprs_cntx.net_id);
    OD_LOG("Od_SocketCreateInternal-hTcpSocket=%d i=%d\n",hTcpSocket,i);
    if(hTcpSocket < 0)
    {

        return OD_ERR2_SOCK_CREATE;
    }
    sci_sock_setsockopt(hTcpSocket, SO_NBIO, NULL);
    if (i < OD_MAX_SOCKET_NUM2)
    {
        mysocket_free[i].socket_id = hTcpSocket;

        //memset(tmp_Sockbuff[i].recvBuff, 0, OD_REC_BUFF_MAX);
        //memset(tmp_Sockbuff[i].sendBuff, 0, OD_REC_BUFF_MAX);
        //mysocket_free[i].g_RecBuffer = (OD_S8*)tmp_Sockbuff[i].recvBuff;
        //mysocket_free[i].g_SendBuffer = (OD_S8*)tmp_Sockbuff[i].sendBuff;

    }
    else
    {
        i = OD_ERR2_SOCK_LIMIT_RESOURCE;
    }

    return i;
}

static OD_S32 Od_SocketConnect(OD_SOCKET2 s, const OD_U8* pServerIP, const OD_U16 nPort, OD_RECV_CALLBACK2 fun)
{

    if(fun != OD_NULL)
        mysocket_free[s].g_RecvHandler = fun;
    g_od_gprs_cntx.connect_repeat_count = 0;
    return Od_SocketConnectInternal(s, pServerIP, nPort);
}


static void od_connect_timeout_cb(void)
{

    OD_LOG("od_connect_timeout_cb\n");
    if(mysocket_free[g_od_gprs_cntx.connect_soc_id].g_RecvHandler != OD_NULL)
    {
        mysocket_free[g_od_gprs_cntx.connect_soc_id].g_RecvHandler(g_od_gprs_cntx.connect_soc_id,OD_SOCK2_CLOSING,OD_SOCK2_SUCCESS,OD_NULL,0);
    }
    Od_SocketClose2(g_od_gprs_cntx.connect_soc_id);

}
static void od_connect_repeat_timeout_cb(void)
{
    OD_LOG("od_connect_repeat_timeout_cb--ID=%d",g_od_gprs_cntx.connect_soc_id);
	if(g_od_gprs_cntx.connect_repeat_timer>0)
	{
		Od_StopTimer(g_od_gprs_cntx.connect_repeat_timer);
		g_od_gprs_cntx.connect_repeat_timer = 0;
	}
    Od_SocketConnectInternal(g_od_gprs_cntx.connect_soc_id, mysocket_free[g_od_gprs_cntx.connect_soc_id].server_ip, mysocket_free[g_od_gprs_cntx.connect_soc_id].server_port);
    OD_LOG("od_connect_repeat_timeout_cb--connect_repeat_timer=%d",g_od_gprs_cntx.connect_repeat_timer);
}

static OD_S32 Od_SocketConnectInternal(OD_SOCKET2 nSocketID, const OD_U8* pServerIP, const OD_U16 nPort)
{
    OD_S32 nResult = -1;
    OD_U8 val;
    struct sci_sockaddr sAddr;
    struct sci_sockaddr sProxyAddr;
    //OD_U8 szReqBuff[32] = {'\0'};

    val = TRUE;

    OD_LOG("Od_SocketConnectInternal-enter\n");

    memset(&sAddr, 0, sizeof(struct sci_sockaddr));
    sAddr.family = AF_INET;
#if defined(WIN32) || defined( CHIP_ENDIAN_LITTLE)
	OD_LOG("Od_SocketConnectInternal-pServerIP: %d.%d.%d.%d",pServerIP[0],pServerIP[1],pServerIP[2],pServerIP[3]);

    sAddr.ip_addr = pServerIP[3]<<24|pServerIP[2]<<16| pServerIP[1]<<8|pServerIP[0];
    sAddr.port =  htons(nPort);
#else
    sAddr.ip_addr = pServerIP[0]<<24|pServerIP[1]<<16| pServerIP[2]<<8|pServerIP[3];
    sAddr.port = nPort;     // htons(80);/*short,network byte order*/

    /*hostbyteorder*/
#endif


    //nResult = soc_connect(nSocketID, &sAddr);


    nResult = sci_sock_connect(mysocket_free[nSocketID].socket_id,&sAddr,sizeof(struct sci_sockaddr));

    OD_LOG("Od_SocketConnectInternal-nResult=%d\n",nResult);

    mysocket_free[nSocketID].socket_status = OD_SOCK2_CONNECTING;
    if( nResult < 0 )
    {
        OD_S32 error = (OD_S32)sci_sock_errno(mysocket_free[nSocketID].socket_id);
        OD_LOG("Od_SocketConnectInternal-error=%d repeat_count=%d\n",error,g_od_gprs_cntx.connect_repeat_count);

        if(EINPROGRESS == error || EWOULDBLOCK == error || 0 == error)
        {
            //性能限制
            //g_od_gprs_cntx.connect_timer_id = Od_CreateTimer(OD_CONNECT_PROTECT_TIME,od_connect_timeout_cb);
            g_od_gprs_cntx.connect_soc_id = nSocketID;
#ifdef  OD_SOCKET2_WITH_THREAD
            return OD_ERR2_SOCK_WOULDBLOCK;
#else
            if(g_od_gprs_cntx.connect_repeat_count < OD_CONNECT_REPEAT_COUNT)
            {
            	
				if(g_od_gprs_cntx.connect_repeat_timer>0)
				{
					Od_StopTimer(g_od_gprs_cntx.connect_repeat_timer);
				}
                g_od_gprs_cntx.connect_repeat_timer= Od_CreateTimer(OD_CONNECT_REPEAT_TIME,od_connect_repeat_timeout_cb);
                g_od_gprs_cntx.connect_repeat_count++;
                return OD_ERR2_SOCK_WOULDBLOCK;
            }
#endif

        }
        //else
        {
            {
                OD_LOG("reset gprs");
                Od_ResetAndDeactivePdpContext(g_od_gprs_cntx.active_sim);
            }
            if(mysocket_free[nSocketID].g_RecvHandler != OD_NULL)
                mysocket_free[nSocketID].g_RecvHandler(nSocketID,OD_SOCK2_CONNECTING,OD_ERR2_SOCK_CONN_FAIL,OD_NULL,0);
            else
                Od_SocketClose2(nSocketID);
            return OD_ERR2_SOCK_CONN_FAIL;
        }

    }
    else
    {

        mysocket_free[nSocketID].socket_status = OD_SOCK2_CONNECTED;
        Od_socket_notify_internal(nSocketID, sock_cb_connected);
        return OD_SOCK2_SUCCESS;
    }
}

static OD_S32 Od_SendDataInternal(OD_SOCKET2 nSocketID)
{
    OD_S32 nResult = -1;
    if(mysocket_free[nSocketID].g_SendBuffLen <= 0)return -1;

    mysocket_free[nSocketID].socket_status = OD_SOCK2_SENDING;

    nResult = Od_SockSendInternal(nSocketID,(OD_U8*)mysocket_free[nSocketID].g_SendBuffer+mysocket_free[nSocketID].g_SendBuffOffset,
                                   mysocket_free[nSocketID].g_SendBuffLen - mysocket_free[nSocketID].g_SendBuffOffset);
    if(nResult > 0)
    {
        mysocket_free[nSocketID].g_SendBuffOffset += nResult;
        OD_LOG("Od_SendDataInternal-off=0x%x g_SendBuffLen=0x%x",mysocket_free[nSocketID].g_SendBuffOffset,mysocket_free[nSocketID].g_SendBuffLen);
        if(mysocket_free[nSocketID].g_SendBuffOffset == mysocket_free[nSocketID].g_SendBuffLen)
        {
            mysocket_free[nSocketID].socket_status = OD_SOCK2_SENT;
            mysocket_free[nSocketID].g_SendBuffOffset = 0;
            mysocket_free[nSocketID].g_SendBuffLen = 0;

            if(mysocket_free[nSocketID].g_RecvHandler != OD_NULL)
                mysocket_free[nSocketID].g_RecvHandler(nSocketID,OD_SOCK2_SENT,OD_SOCK2_SUCCESS,OD_NULL,0);
        }
    }

    return nResult;
}
static void od_send_repeat_timeout_cb(void)
{
    OD_LOG("od_send_repeat_timeout_cb--ID=%d",g_od_gprs_cntx.connect_soc_id);
	if(g_od_gprs_cntx.send_repeat_timer>0)
	{
		Od_StopTimer(g_od_gprs_cntx.send_repeat_timer);
	}

    Od_socket_notify_internal(g_od_gprs_cntx.connect_soc_id, sock_cb_send);
    OD_LOG("od_send_repeat_timeout_cb--send_repeat_timer=%d",g_od_gprs_cntx.send_repeat_timer);
}
static void od_rec_repeat_timeout_cb(void)
{
    OD_LOG("od_rec_repeat_timeout_cb--ID=%d",g_od_gprs_cntx.connect_soc_id);
    if(g_od_gprs_cntx.rec_repeat_timer>0)
    {
			Od_StopTimer(g_od_gprs_cntx.rec_repeat_timer);
    }
    g_od_gprs_cntx.rec_repeat_timer= Od_CreateTimer(OD_CONNECT_REPEAT_TIME,od_rec_repeat_timeout_cb);
    Od_socket_notify_internal(g_od_gprs_cntx.connect_soc_id, sock_cb_recv);
}

static OD_S32 Od_SockSendInternal(OD_SOCKET2 nSocketID, OD_U8* pBuff, OD_U32 nBuffLen)
{
    OD_S32 nResult = 0 ;

    OD_S32 errno;

    mysocket_free[nSocketID].IsHead = OD_TRUE;
    g_od_gprs_cntx.connect_soc_id = nSocketID;
    mysocket_free[nSocketID].socket_status = OD_SOCK2_SENDING;

    nResult = sci_sock_send(mysocket_free[nSocketID].socket_id,pBuff,nBuffLen,0);

    if( nResult < 0 )
    {
        errno = sci_sock_errno(mysocket_free[nSocketID].socket_id);




        if(errno > 0 && (EWOULDBLOCK != errno && ENOBUFS != errno))
        {
            //error handle
            {
                OD_LOG("reset gprs send");
                Od_ResetAndDeactivePdpContext(g_od_gprs_cntx.active_sim);
            }
            if(mysocket_free[nSocketID].g_RecvHandler != OD_NULL)
                mysocket_free[nSocketID].g_RecvHandler(nSocketID,OD_SOCK2_SENDING,OD_ERR2_SOCK_SEND_ERR,OD_NULL,0);
            else
                Od_SocketClose2(nSocketID);

            nResult = OD_ERR2_SOCK_SEND_ERR;
            return nResult;
        }
        else
        {
            mysocket_free[nSocketID].socket_status = OD_SOCK2_SENDING;
            g_od_gprs_cntx.connect_soc_id = nSocketID;
#ifndef OD_SOCKET2_WITH_THREAD
			if(g_od_gprs_cntx.send_repeat_timer>0)
			{
				Od_StopTimer(g_od_gprs_cntx.send_repeat_timer);
			}
            g_od_gprs_cntx.send_repeat_timer= Od_CreateTimer(OD_CONNECT_REPEAT_TIME,od_send_repeat_timeout_cb);
#endif
            nResult = OD_ERR2_SOCK_WOULDBLOCK;
            return nResult;
        }
    }
    else if(nResult== nBuffLen )
    {
        mysocket_free[nSocketID].socket_status = OD_SOCK2_SENT;
        OD_LOG("Od_SockSendInternal-finish");
        g_od_gprs_cntx.connect_soc_id = nSocketID;
#ifndef OD_SOCKET2_WITH_THREAD
		if(g_od_gprs_cntx.rec_repeat_timer>0)
			Od_StopTimer(g_od_gprs_cntx.rec_repeat_timer);
        g_od_gprs_cntx.rec_repeat_timer= Od_CreateTimer(OD_CONNECT_REPEAT_TIME,od_rec_repeat_timeout_cb);
#endif
        return nResult;
    }


#ifndef OD_SOCKET2_WITH_THREAD
	if(g_od_gprs_cntx.rec_repeat_timer>0)
		Od_StopTimer(g_od_gprs_cntx.rec_repeat_timer);
    g_od_gprs_cntx.send_repeat_timer= Od_CreateTimer(OD_CONNECT_REPEAT_TIME,od_send_repeat_timeout_cb);
#endif
    return nResult;
}
OD_S32 Od_ReceiveData2(OD_SOCKET2 nSocketID, OD_U8 * buf, OD_U32 len)
{
    OD_S32 nResult = 0 ;
    OD_S32 errno;

    nResult = sci_sock_recv(mysocket_free[nSocketID].socket_id, (OD_U8*)buf, len, 0);
    OD_LOG("Od_ReceiveData2--nResult=%d",nResult);
    if ( nResult < 0  )
    {
        errno = sci_sock_errno(mysocket_free[nSocketID].socket_id);
        OD_LOG("Od_ReceiveData2--errno=%d",errno);
        //error handle
        if(errno > 0 && (EWOULDBLOCK != errno && ENOBUFS != errno))
        {
            {
                OD_LOG("reset gprs rec");
                Od_ResetAndDeactivePdpContext(g_od_gprs_cntx.active_sim);
            }
            return OD_ERR2_SOCK_RECV_FAIL;
        }
        return OD_ERR2_SOCK_WOULDBLOCK;
    }
    else if(nResult == 0)
    {
        //Od_SocketClose2(nSocketID);
    }
    return nResult;


}





void Od_SocketClose2(OD_SOCKET2 s)
{
    OD_LOG("Od_SocketClose2 ENTER s=%d\n",s);
    if((s<0)||(s>=OD_MAX_SOCKET_NUM2))
        return;

    if(mysocket_free[s].socket_id >=0)
    {
        sci_sock_shutdown(mysocket_free[s].socket_id, SD_BOTH) ; // tanml add on 20150911 close tcp
        sci_sock_socketclose(mysocket_free[s].socket_id);
    }


    if(g_od_gprs_cntx.connect_timer_id>0)
    {
        OD_LOG("Od_SocketClose2 ENTER -connect_timer_id=%d\n",g_od_gprs_cntx.connect_timer_id);
        Od_StopTimer(g_od_gprs_cntx.connect_timer_id);
        g_od_gprs_cntx.connect_timer_id = 0;
    }
    if(g_od_gprs_cntx.connect_repeat_timer>0)
    {
        OD_LOG("Od_SocketClose2 ENTER -connect_repeat_timer=%d\n",g_od_gprs_cntx.connect_repeat_timer);
        Od_StopTimer(g_od_gprs_cntx.connect_repeat_timer);
        g_od_gprs_cntx.connect_repeat_timer = 0;
    }
    if(g_od_gprs_cntx.send_repeat_timer>0)
    {
        OD_LOG("Od_SocketClose2 ENTER -send_repeat_timer=%d\n",g_od_gprs_cntx.send_repeat_timer);
        Od_StopTimer(g_od_gprs_cntx.send_repeat_timer);
        g_od_gprs_cntx.send_repeat_timer = 0;
    }
    if(g_od_gprs_cntx.rec_repeat_timer>0)
    {
        OD_LOG("Od_SocketClose2 ENTER -rec_repeat_timer=%d\n",g_od_gprs_cntx.rec_repeat_timer);
        Od_StopTimer(g_od_gprs_cntx.rec_repeat_timer);
        g_od_gprs_cntx.rec_repeat_timer = 0;
    }
    //memset(&mysocket_free[s],0,sizeof(mysocket_free[s]));
    mysocket_free[s].socket_id = -1;
    mysocket_free[s].g_RecvHandler = OD_NULL;

    mysocket_free[s].socket_status = OD_SOCK2_CLOSED;
    Od_NetFree();
    return ;
}

static void Od_ReadDefaultConnSettingInternal(void)
{
    //g_connSetting.actived_net_type = EMA_CONN_CMWAP;
    Od_SetNetMode();


}



static void Od_SetNetMode(void)
{
    OD_U8 data = 0;

    OD_U8* apn;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    OD_U8 account_id = 0,sim_id;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/


    sim_id = Od_AutoChangeSim()-1;

    g_od_gprs_cntx.active_sim =sim_id;
    //g_od_gprs_cntx.account_id = g_connSetting.accountID;
    g_od_gprs_cntx.net_active_cb = od_pdp_active_cb;

}




OD_S32 Od_SocketSendData2(OD_SOCKET2 s, const OD_S8* pBuff, OD_S32 nLen)
{

    if(pBuff==NULL || nLen == NULL)
        return 0;
	
    return Od_SockSendInternal(s,pBuff,nLen);

}


static void Od_socket_notify_internal(OD_S32 socket_id, e_msg_cb msgId)

{
    OD_ACCOUNT_DATA_STRUCT* nv_buf;
	OD_U16 len;
    OD_U16 i=0;
    OD_LOG("Od_socket_notify_internal--msgId=%d socket_id=%d\n",msgId,socket_id);


    switch (msgId)
    {
        case sock_cb_send:
        {
            //mysocket_free[socket_id].socket_status = OD_SOCK_SENT;
            if(mysocket_free[socket_id].g_RecvHandler != OD_NULL)
                mysocket_free[socket_id].g_RecvHandler(socket_id,OD_SOCK2_SENT,OD_SOCK2_SUCCESS,0,0);

            break;
        }

        case sock_cb_recv:
        {

            //mysocket_free[socket_id].socket_status = OD_SOCK2_RECEIVED;
            if(mysocket_free[socket_id].g_RecvHandler != OD_NULL)
                mysocket_free[socket_id].g_RecvHandler(socket_id,OD_SOCK2_RECEIVED,OD_SOCK2_SUCCESS,0,0);


            break;
        }
        case sock_cb_connected:
        {

            mysocket_free[socket_id].socket_status = OD_SOCK2_CONNECTED;

            if(g_od_gprs_cntx.connect_timer_id>0)
            {
                Od_StopTimer(g_od_gprs_cntx.connect_timer_id);
                g_od_gprs_cntx.connect_timer_id = 0;
            }
            if(mysocket_free[socket_id].g_RecvHandler != OD_NULL)
            {
                mysocket_free[socket_id].socket_retcode=OD_SOCK2_SUCCESS;

                mysocket_free[socket_id].g_RecvHandler(socket_id,mysocket_free[socket_id].socket_status,mysocket_free[socket_id].socket_retcode,OD_NULL,0);
            }
            if(!is_roaming&&(g_od_apn_suc_flag==OD_FALSE))
            {
            	len = sizeof(OD_ACCOUNT_DATA_STRUCT)*OD_APN_CONFIG_MAX;
                nv_buf = od_game_malloc(len);
                if(nv_buf)
                {
                    if(Od_ReadNVConfig(nv_buf,len))
                    {
                        //查找同PLMN，替换不一致apn
                        for(i; i<OD_APN_CONFIG_MAX; i++)
                        {
                            if(strcmp(s_apn_collect.plmn, (nv_buf+i)->plmn)==0)
                            {
                                if(strcmp(s_apn_collect.apn, (nv_buf+i)->apn)!=0)
                                {
                                    memset(nv_buf+i, 0, sizeof(OD_ACCOUNT_DATA_STRUCT));
                                    strcpy((nv_buf+i)->plmn,s_apn_collect.plmn);
                                    strcpy((nv_buf+i)->apn,s_apn_collect.apn);
                                    strcpy((nv_buf+i)->username,s_apn_collect.username);
                                    strcpy((nv_buf+i)->password,s_apn_collect.password);
                                    Od_WriteNVConfig(nv_buf,len);
                                    OD_LOG("Od_socket_notify_internal--apn exit");
                                }
                                g_od_apn_suc_flag = OD_TRUE;

                                break;
                            }
                        }
                        if(i>=OD_APN_CONFIG_MAX)
                        {
                            // 查找空白位置或替换旧plmn
                            for(i=0; i<OD_APN_CONFIG_MAX; i++)
                            {
                                if(strlen((nv_buf+i)->plmn)==0)
                                {

                                    break;
                                }
                            }
                            if(i>=OD_APN_CONFIG_MAX)
                                i=0;
                            memset(nv_buf+i, 0, sizeof(OD_ACCOUNT_DATA_STRUCT));
                            strcpy((nv_buf+i)->plmn,s_apn_collect.plmn);
                            strcpy((nv_buf+i)->apn,s_apn_collect.apn);
                            strcpy((nv_buf+i)->username,s_apn_collect.username);
                            strcpy((nv_buf+i)->password,s_apn_collect.password);
                            Od_WriteNVConfig(nv_buf,len);
                            OD_LOG("Od_socket_notify_internal--apn new");
                            g_od_apn_suc_flag = OD_TRUE;

                        }
                    }
                    od_game_free(nv_buf);
                }
            }


            break;
        }
        case sock_cb_close:
        {
            if(g_od_gprs_cntx.connect_timer_id>0)
            {
                Od_StopTimer(g_od_gprs_cntx.connect_timer_id);
                g_od_gprs_cntx.connect_timer_id = 0;
                // g_exit_task_flag = FALSE;
                // ema_net_initialized = OD_FALSE;
                OD_LOG("gprs bad--reset");
            }
            if(mysocket_free[socket_id].socket_status == OD_SOCK2_CONNECTING)
            {
                memset(mysocket_free[socket_id].server_ip,0,sizeof(mysocket_free[socket_id].server_ip));
            }
            mysocket_free[socket_id].socket_status = OD_SOCK2_CLOSING;

            if(mysocket_free[socket_id].g_RecvHandler != OD_NULL)
            {
                mysocket_free[socket_id].g_RecvHandler(socket_id,mysocket_free[socket_id].socket_status,OD_SOCK2_SUCCESS,OD_NULL,0);
            }


            break;
        }
        default:
            Od_SocketClose2(socket_id);
            break;
    }


}

void Od_SocketEnter2(void)
{

    if (ema_net_initialized == OD_TRUE)
    {

        return;
    }

    Od_NetInitialize();
    g_od_gprs_cntx.connect_timer_id = 0;
    g_od_gprs_cntx.connect_repeat_timer=0;
    g_od_gprs_cntx.send_repeat_timer =0;
    g_od_gprs_cntx.rec_repeat_timer=0;
    ema_net_initialized = OD_TRUE;
}

extern void _ema_Close_account_id(void);

static void Od_NetFree(void)
{


    //假如没有可用socket，则释放所有资源
    if (Od_GetFreeSlot_count() >= OD_MAX_SOCKET_NUM2)
    {
        g_exit_task_flag = FALSE;
        ema_net_initialized = OD_FALSE;
#ifndef OD_SOCKET2_WITH_THREAD
        if(g_auto_gprs_flag == OD_FALSE)
        {
            Od_ResetAndDeactivePdpContext(g_od_gprs_cntx.active_sim);
        }
#endif
    }



}

static void Od_SocketExit(void)
{
    OD_S32 i = 0;

    for (i = 0; i < OD_MAX_SOCKET_NUM2; i++)
    {
        if (mysocket_free[i].socket_id != -1)
        {
            Od_SocketClose2(i);
        }
    }

    {
        Od_NetFree();
    }
}




static void Od_NetInitialize(/*LVPF_INIT_NETWORK_CALLBACK pCallback*/)
{

    OD_S8 i = 0;

    for (i = 0; i < OD_MAX_SOCKET_NUM2; i++)
    {
        mysocket_free[i].socket_id = -1;
        mysocket_free[i].socket_status = OD_SOCK2_NONE_STATUS;
    }



    //Od_ReadDefaultConnSettingInternal();

}

static OD_S32 Od_GetFreeSlot_count(void)
{
    OD_S32 i,count = 0;
    for (i = 0; i < OD_MAX_SOCKET_NUM2; i++)
    {
        if (mysocket_free[i].socket_id < 0)
            count ++;
    }

    return count;
}
static void  Od_GprsActive(void)
{

    g_auto_gprs_flag = OD_TRUE;
}
static void  Od_GprsDeactive(void)
{

    g_auto_gprs_flag = OD_FALSE;
    g_exit_task_flag = FALSE;
    ema_net_initialized = OD_FALSE;

    Od_ResetAndDeactivePdpContext(g_od_gprs_cntx.active_sim);

}
#ifdef MMI_WIFI_SUPPORT

OD_BOOL Od_GetIsUseWifi2(void)
{
    return s_wifi_flag;
}
void Od_SetIsUseWifi2(OD_BOOL flag)
{
    s_wifi_flag = flag;
}
#endif
void od_socket_creat_timer2(void)
{
    if(s_socket_id>=0)
    {
        Od_GetSocketHostByName(s_socket_id,socket_get_host_name_callback);
        s_socket_id = -1;
    }
}

static OD_S32 Od_SocketStart2_local(OD_S32 handle,ODConnect_Parameter2* param)
{
    OD_SOCKET2 socketID;
    OD_S32 pdp_state = od_get_pdp_state();
    OD_S32 i = 0;


    if(param == NULL)
        return 0;
    //
    //if (Od_GetFreeSlot_count() >= OD_MAX_SOCKET_NUM2)
    Od_SocketEnter2();


    socketID = Od_Mysocket_find_free_slot();


    if(socketID>=OD_MAX_SOCKET_NUM2)
    {

        if(param->fun != OD_NULL)
        {
            param->fun(-1,OD_SOCK2_NETINITIALIZING,OD_ERR2_SOCK_LIMIT_RESOURCE,OD_NULL,0);
            return -1;
        }
    }
    s_socket_id=socketID;
    Od_SetNetMode();

    for (i = 0; i < OD_MAX_SOCKET_NUM2; i++)
    {
        if (strcmp(mysocket_free[i].domain,param->domain)==0)
        {
            OD_LOG("strcmp--domain=%s server_ip=%x\n",mysocket_free[i].domain,*(OD_U32*)mysocket_free[i].server_ip);
            if(*(OD_U32*)mysocket_free[i].server_ip)
            {
                OD_LOG("Od_SocketStart2_local--dns already exist i=%d\n",i);
                memcpy(mysocket_free[socketID].server_ip,mysocket_free[i].server_ip,4);
                break;
            }
        }
    }

    if(i == OD_MAX_SOCKET_NUM2)
    {
        memset(mysocket_free[socketID].server_ip,0,sizeof(mysocket_free[socketID].server_ip));
    }
    mysocket_free[socketID].server_port = param->nPort;
    mysocket_free[socketID].g_RecvHandler = param->fun;
    mysocket_free[socketID].connType = param->connType;
    mysocket_free[socketID].handle = handle;
    mysocket_free[socketID].socket_status = OD_SOCK2_NETINITIALIZING;

    OD_LOG("Od_SocketStart2_local--param->domain=%s socketID=%d\n",param->domain,socketID);
    memset(mysocket_free[socketID].domain,0,sizeof(mysocket_free[socketID].domain));
    if(param->domain != OD_NULL)
        memcpy(mysocket_free[socketID].domain, param->domain,strlen((char*)param->domain));
#ifdef  OD_SOCKET2_WITH_THREAD

    od_network_select_task_create(); //sunvov
#endif
#ifdef MMI_WIFI_SUPPORT
    if(MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus() || Od_GetIsUseWifi2())
    {
        OD_LOG("Od_SocketStart2_local-use wifi");
        if(g_od_gprs_cntx.pdp_protect_timer_id != 0)
        {
            MMK_StopTimer(g_od_gprs_cntx.pdp_protect_timer_id);
            g_od_gprs_cntx.pdp_protect_timer_id =0;
        }
        //Od_SetIsUseWifi2(OD_TRUE);

        g_od_gprs_cntx.net_id =  MMIAPIWIFI_GetNetId(); //defult WIFI

        //Od_GetSocketHostByName(socketID,socket_get_host_name_callback);
        Od_CreateTimer(OD_CREAT_SOCKET_ASYNC_TIME,od_socket_creat_timer2);
        return socketID;

    }
#endif

    if(pdp_state != OD_GPRS_ACTIVE_OK )
    {
        OD_LOG("Od_SocketStart2_local-gprs_state=%d",g_od_gprs_cntx.gprs_state);
        if(g_od_gprs_cntx.gprs_state != OD_GPRS_DEACTIVE_OK )
        {
            OD_LOG("Od_SocketStart2_local-GPRS reset");
            Od_ResetAndDeactivePdpContext(g_od_gprs_cntx.active_sim);
        }
        od_acitvie_gprs_with_sim( g_od_gprs_cntx.active_sim, g_od_gprs_cntx.account_id);
    }
    else
    {

        //Od_GetSocketHostByName(socketID,socket_get_host_name_callback);
        Od_CreateTimer(OD_CREAT_SOCKET_ASYNC_TIME,od_socket_creat_timer2);


    }
    return socketID;
}
typedef void (*od_network_func_type)(OD_S32 result);
static void od_pdp_active_cb(OD_S32 result)
{
    OD_SOCKET2 socketID;
    OD_LOG("od_pdp_active_cb--result=%d\n",result);
    if(result == 0)
    {
        for(socketID=0; socketID<OD_MAX_SOCKET_NUM2; socketID++)
        {
            if(mysocket_free[socketID].g_RecvHandler)
                mysocket_free[socketID].g_RecvHandler(socketID,OD_SOCK2_NETINITIALIZED,OD_SOCK2_ERROR,OD_NULL,0);
        }
        Od_SocketExit();
    }
    else if(result == 1)
    {

        for(socketID=0; socketID<OD_MAX_SOCKET_NUM2; socketID++)
        {

            if(mysocket_free[socketID].socket_status == OD_SOCK2_NETINITIALIZING)
            {


                Od_GetSocketHostByName(socketID,socket_get_host_name_callback);


            }
        }

    }

}

OD_S32 Od_SocketStart2(ODConnect_Parameter2* param)
{
#ifdef GPRS_SUPPORT
    return Od_SocketStart2_local(0,param);
#else
    return 0;
#endif
}
static OD_BOOL Od_GetSocketHostByName(OD_S32 request_id, OD_GET_HOST_BYNAME_CALLBACK2 pCallback)
{
    //OD_S32 ret=0;
    OD_U8  addr_len=0;
    //OD_U32   nwk_account_id = LCM_LM_NWK_ACCOUNT_ID;
    OD_U32 ipAddr=0;
    OD_S32 socketID=request_id;
    struct sci_hostent *hostent = NULL;
    char* name = NULL;
    OD_S32 len = 0;
    OD_LOG("Od_GetSocketHostByName-socketID=%d len=%d domain=%s\n",socketID,strlen((char*)mysocket_free[socketID].domain),mysocket_free[socketID].domain);
    if(strlen((char*)mysocket_free[socketID].domain) == 0)
    {

        pCallback(OD_ERR2_SOCK_GET_HOST_BYNAME_FAIL,OD_NULL,socketID);
        return OD_FALSE;
    }

    mysocket_free[socketID].pf_GetHostByNameFun = pCallback;
    mysocket_free[socketID].socket_status = OD_SOCK2_HOSTGETTING;



    if(*(OD_U32*)mysocket_free[socketID].server_ip)
    {

        OD_LOG("Od_GetSocketHostByName-not dns action\n");
        pCallback(OD_SOCK2_SUCCESS,mysocket_free[socketID].server_ip,socketID);
    }
    else
    {
    	int count ;
    	for(count =0 ; count < 5; count++){ // tanml add the retry 5 times for get host by name
	        hostent = sci_gethostbyname(mysocket_free[socketID].domain);
	        OD_LOG("Od_GetSocketHostByName-hostent=%d domain=%s count=%d\n",hostent,mysocket_free[socketID].domain,count);

	        if (hostent != NULL)
	        {
	            memset(mysocket_free[socketID].server_ip,0,4);
	            memcpy(mysocket_free[socketID].server_ip,hostent->h_addr_list[0],4);

	            pCallback(OD_SOCK2_SUCCESS,mysocket_free[socketID].server_ip,socketID);
				return OD_TRUE;
	        }
			SCI_Sleep(20);
    	}
		if(count >= 5)
		{
	    	pCallback(OD_ERR2_SOCK_GET_HOST_BYNAME_FAIL,OD_NULL,socketID);
			return OD_FALSE;
		}
    }
    return OD_TRUE;

}



static void  socket_get_host_name_callback(OD_SocketRetCode2 ret, const OD_U8* pServerIP,OD_S32 socketID)
{
    OD_SOCKET2 s = -1;
    OD_LOG("socket_get_host_name_callback-ret=%d state=%d rec_h=%d,pServerIP=%s\n",ret,mysocket_free[socketID].socket_status,mysocket_free[socketID].g_RecvHandler,pServerIP);
    if(mysocket_free[socketID].socket_status == OD_SOCK2_CLOSED)
    {

        return;
    }

    if(ret == OD_SOCK2_SUCCESS)
    {
        mysocket_free[socketID].socket_status = OD_SOCK2_HOSTGETTED;
        if(mysocket_free[socketID].g_RecvHandler != OD_NULL)
            mysocket_free[socketID].g_RecvHandler(socketID,mysocket_free[socketID].socket_status,OD_SOCK2_SUCCESS,pServerIP,4);
        s = Od_SocketCreateInternal(socketID);
        OD_LOG("socket_get_host_name_callback-  creat socket=%d \n",s);
        if((s == OD_ERR2_SOCK_LIMIT_RESOURCE) || (s == OD_ERR2_SOCK_CREATE))
        {
            if(mysocket_free[socketID].g_RecvHandler != OD_NULL)
                mysocket_free[socketID].g_RecvHandler(-1,mysocket_free[socketID].socket_status,s,OD_NULL,0);
            return;
        }


        Od_SocketConnect(s,pServerIP,mysocket_free[socketID].server_port,mysocket_free[socketID].g_RecvHandler);
    }
    else
    {


        {
            if(mysocket_free[socketID].g_RecvHandler  != OD_NULL)
            {
                mysocket_free[socketID].g_RecvHandler (-1,mysocket_free[socketID].socket_status,OD_ERR2_SOCK_GET_HOST_BYNAME_FAIL,OD_NULL,0);
            }
            return;
        }
    }
}



///////////////////////
static OD_BOOL Od_check_pdp_state(void)
{
    MN_GPRS_CON_STATE_T *ctx_state = 0;
    OD_S32 ctx_num = 0;
    OD_S32 state = g_od_gprs_cntx.gprs_state;
    OD_S32 i;
    OD_BOOL ret = OD_FALSE;

    ctx_state = SCI_ALLOC_APP(sizeof(MN_GPRS_CON_STATE_T));
    if(MNGPRS_ReadPdpContextStateEx(g_od_gprs_cntx.active_sim, &ctx_state, &ctx_num) == ERR_MNGPRS_NO_ERR)
    {
        if(ctx_num)
            ret = OD_TRUE;
    }

    if(ctx_state)
    {
        SCI_FREE(ctx_state);
    }


    return ret;
}

static OD_S32 od_get_pdp_state(void)
{
    MN_GPRS_CON_STATE_T *ctx_state = 0;
    OD_S32 ctx_num = 0;
    OD_S32 state = g_od_gprs_cntx.gprs_state;
    OD_S32 i;
    MMIPDP_LINK_STATE_E link_state;
#ifdef MMI_WIFI_SUPPORT
    if(MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus() || Od_GetIsUseWifi2())
    {
        return OD_GPRS_ACTIVE_OK;
    }
#endif
    link_state = MMIAPIPDP_GetLinkState(MMI_MODULE_OD_GAME);

    switch(link_state)
    {
        case MMIPDP_LINK_STATE_ACTIVED:
            state = OD_GPRS_ACTIVE_OK;
            break;
        case MMIPDP_LINK_STATE_DEACTIVED:
            state = OD_GPRS_DEACTIVE_OK;
            break;
        case MMIPDP_LINK_STATE_ACTIVING:
            state = OD_GPRS_ACTIVING;
            break;
        case MMIPDP_LINK_STATE_DEACTIVING:
            state = OD_GPRS_DEACTIVEING;
            break;
        default:
            state = OD_GPRS_DEACTIVE_OK;
            break;
    }


    g_od_gprs_cntx.gprs_state = state;

    //OD_LOG("od_get_pdp_state-state=%d link_state=%d\n",state,link_state);
    return state;
}




OD_BOOL Od_HandlePdpProtectTimerMsg2(OD_U8 timer_id)
{
    if(timer_id == g_od_gprs_cntx.pdp_protect_timer_id)
    {

        //关闭保护定时器
        OD_LOG("Od_HandlePdpProtectTimerMsg2--pdp timeout\n");

        MMK_StopTimer(g_od_gprs_cntx.pdp_protect_timer_id);
        g_od_gprs_cntx.pdp_protect_timer_id = 0;

        if(g_od_gprs_cntx.net_active_cb)
        {
            g_od_gprs_cntx.net_active_cb(0);
        }

        return OD_TRUE;
    }
    return OD_FALSE;

}


static void Od_ProcessGprsActiveCnf(
    MMIPDP_CNF_INFO_T *msg_ptr
)
{


    if(msg_ptr==PNULL)
    {
        OD_LOG("warning:Od_ProcessGprsActiveCnf param PNULL!");
        return;
    }

    OD_LOG("Od_ProcessGprsActiveCnf--result=%d\n",msg_ptr->result);



    if(MMIPDP_RESULT_SUCC == msg_ptr->result)//succ
    {

        g_od_gprs_cntx.gprs_state = OD_GPRS_ACTIVE_OK;

        g_od_gprs_cntx.net_id =  TCPIP_DEFAULT_NETID_FOR_GPRS;
        //g_od_gprs_cntx.net_id = msg_ptr->nsapi;  //backup
        if(g_od_gprs_cntx.net_active_cb)
        {
            g_od_gprs_cntx.net_active_cb(1);
        }

    }
    else
    {

        g_od_gprs_cntx.gprs_state = OD_GPRS_ACEIVE_FAILED;
        //EMA_set_event(g_EMA_account_activ_event, EMA_ACCOUNT_ACTIVE_EVENT, MBMOS_OR);
        if(g_od_gprs_cntx.net_active_cb)
        {
            g_od_gprs_cntx.net_active_cb(0);
        }
        //EMA_PostOpennetMessage(EMA_OPENNET_MSG_ACTIVE_IND, 0, 0);
    }
}



static void Od_HandleNetworkPsMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    if(msg_ptr == OD_NULL)
    {
        return;
    }


    /*    if(OD_GPRS_DEACTIVE_OK == g_od_gprs_cntx.gprs_state)
        {
            return MMI_RESULT_FALSE;
        }*/
    OD_LOG("Od_HandleNetworkPsMsg--msg_id=%d state=%d\n",msg_ptr->msg_id,g_od_gprs_cntx.gprs_state);

    if(g_od_gprs_cntx.gprs_state == OD_GPRS_DEACTIVE_OK )
    {
        return ;
    }
    switch( msg_ptr->msg_id )
    {
        case MMIPDP_ACTIVE_CNF:
        {
            OD_LOG("Od_HandleNetworkPsMsg--pdp_protect_timer_id=%d gprs_state=%d\n",g_od_gprs_cntx.pdp_protect_timer_id,g_od_gprs_cntx.gprs_state);

            //关闭保护定时器
            if(0 != g_od_gprs_cntx.pdp_protect_timer_id)
            {
                MMK_StopTimer(g_od_gprs_cntx.pdp_protect_timer_id);
                g_od_gprs_cntx.pdp_protect_timer_id = 0;
            }
            //   if(g_od_gprs_cntx.gprs_state != OD_GPRS_ACTIVING)
            //{
            //return ;
            //}
            Od_ProcessGprsActiveCnf(msg_ptr);
            break;
        }
        case MMIPDP_DEACTIVE_CNF:

            if(g_od_gprs_cntx.gprs_state != OD_GPRS_DEACTIVEING)
            {
                //根据ps要求，此处进行容错，忽略deactive pdp cnf

                break;
            }
            g_od_gprs_cntx.gprs_state = OD_GPRS_DEACTIVE_OK;//设置GPRS的状态
            g_od_gprs_cntx.net_active_cb = NULL;

            break;

        default:
            break;
    }

    return ;
}

static OD_U8  Od_ReadNVConfig(OD_U8* buffer_ptr,OD_U16 len)
{
    OD_U16 nv_return = 0;
    memset(buffer_ptr,0,len);
    OD_MMINV_READ(MMINV_OD_NV_APN_CONFIG, buffer_ptr, nv_return);  
    if (MN_RETURN_SUCCESS != nv_return)
    {
    	OD_MMINV_WRITE(MMINV_OD_NV_APN_CONFIG, buffer_ptr);
    }  
    return OD_TRUE;
}

static OD_U8  Od_WriteNVConfig(OD_U8* buffer,OD_U16 len)
{
    OD_MMINV_WRITE(MMINV_OD_NV_APN_CONFIG,buffer);
    return OD_TRUE;
}

static void od_acitvie_gprs_with_sim(MN_DUAL_SYS_E dual_sys, OD_U8 account_id)
{


    OD_U8* apn;
    OD_U8 setting_index;
#ifdef BROWSER_SUPPORT
    MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr ;
#endif
    BOOLEAN result = 0;
    MMIPDP_ACTIVE_INFO_T app_info = {0};
    char*                                       plmn;
    OD_ACCOUNT_DATA_STRUCT*                    link = NULL;
    OD_BOOL apn_flag = OD_TRUE;
    OD_ACCOUNT_DATA_STRUCT * nv_buf=OD_NULL ;
	OD_U16 len;
	
    OD_LOG("od_acitvie_gprs_with_sim apn_suc_flag=%d local_apn_flag=%d",g_od_apn_suc_flag,s_local_apn_flag);
    plmn        = od_game_get_plmn_str(dual_sys + 1);
	is_roaming = (MMIPHONE_ROAMING_STATUS_NO_ROMING == MMIAPIPHONE_GetRoamingStatus((MN_DUAL_SYS_E)dual_sys))?OD_FALSE:OD_TRUE;
	OD_LOG("od_acitvie_gprs_with_sim is_roaming=%d",is_roaming);
    //check nv config
    if(g_od_apn_suc_flag==OD_FALSE)
    {
    	if(!is_roaming)
		{
	    	len = sizeof(OD_ACCOUNT_DATA_STRUCT)*OD_APN_CONFIG_MAX;
	        nv_buf = od_game_malloc(len);
	        if(nv_buf)
        {
            if(Od_ReadNVConfig(nv_buf,len))
            {
                for(setting_index=0; setting_index<OD_APN_CONFIG_MAX; setting_index++)
                {
                    if(strcmp(plmn, (nv_buf+setting_index)->plmn)==0)
                    {
                        app_info.auth_type =(nv_buf+setting_index)->auth_type;
                        app_info.apn_ptr = (char*)(nv_buf+setting_index)->apn;
                        app_info.user_name_ptr = (char*)(nv_buf+setting_index)->username;
                        app_info.psw_ptr = (char*)(nv_buf+setting_index)->password;
                        apn_flag = OD_FALSE;
                        break;
                    }

                }
            }

	        }
    	}
        if(apn_flag)
        {
            if (!is_roaming&& plmn )
            {
                link    = Od_GetGprsAccountData(plmn);
                if ( link!=NULL )
                {
                    OD_LOG("od_gprs_update_ActiveGprs link=%s apn=%s",link->name,link->apn);
                    app_info.auth_type = link->auth_type;
                    app_info.apn_ptr = link->apn;
                    app_info.user_name_ptr = link->username;
                    app_info.psw_ptr = link->password;
                }
            }
            if(s_local_apn_flag || link == NULL)
            {
#ifdef BROWSER_SUPPORT
#if defined(PLATFORM_SC6530)
                setting_index = MMIDORADO_GetNetSettingIndex(dual_sys);
#else
                setting_index = MMIDORADO_GetNetSettingIndex(dual_sys);
#endif

#if 1//defined(__OD_SC6800_PDA2_SWITCH__)||(defined(__OD_SC6620_PDA_SWITCH__)&&defined(NETWORK_ACCOUNT_ADAPTIVE_SUPPORT))||defined(PLATFORM_SC6530)
                setting_item_ptr = MMICONNECTION_GetLinkSettingItemByIndex(dual_sys , setting_index);
#else
                setting_item_ptr = MMICONNECTION_GetLinkSettingItemByIndex( setting_index);
#endif
                app_info.auth_type = setting_item_ptr->auth_type;
                app_info.apn_ptr = (char*)setting_item_ptr->apn;
                app_info.user_name_ptr = (char*)setting_item_ptr->username;
                app_info.psw_ptr = (char*)setting_item_ptr->password;
#else
                memset(&app_info,0,sizeof(MMIPDP_ACTIVE_INFO_T));
#endif
            }
        }
    }
    else
    {
        app_info.auth_type = s_apn_collect.auth_type;
        app_info.apn_ptr = s_apn_collect.apn;
        app_info.user_name_ptr =s_apn_collect.username;
        app_info.psw_ptr = s_apn_collect.password;
    }
    OD_LOG("od_acitvie_gprs_with_sim--apn=%s\n",s_apn_collect.apn);
    app_info.app_handler = MMI_MODULE_OD_GAME;

    app_info.dual_sys = dual_sys;
    app_info.priority = 3;
#if defined(PLATFORM_SC6800H)||defined(PLATFORM_SC6530)
    // app_info.ps_service_rat = MN_UNSPECIFIED;
#endif
    app_info.ps_service_rat = MN_TD_PREFER;
    app_info.ps_interface = MMIPDP_INTERFACE_GPRS;
    app_info.handle_msg_callback = Od_HandleNetworkPsMsg;
    if(!is_roaming&&(g_od_apn_suc_flag==OD_FALSE))
    {
        memset(&s_apn_collect,0,sizeof(OD_ACCOUNT_DATA_STRUCT));
        s_apn_collect.auth_type = app_info.auth_type;
        OD_LOG("od_acitvie_gprs_with_sim--apn_ptr=%s\n",app_info.apn_ptr);
        strcpy(s_apn_collect.plmn,plmn);
        strcpy(s_apn_collect.apn,app_info.apn_ptr);
        strcpy(s_apn_collect.username,app_info.user_name_ptr);
        strcpy(s_apn_collect.password,app_info.psw_ptr);
    }
    MNGPRS_ReadStorageExtReqQosEx(dual_sys,MN_GPRS_STORAGE_ALL,BROWSER_E,&app_info.qos_info);
	OD_LOG("od_acitvie_gprs_with_sim: appInfo--->auth_type=%d, apn_ptr=%s, user_name_ptr=%s, psw_ptr=%s, dual_sys=%d",app_info.auth_type,app_info.apn_ptr ,app_info.user_name_ptr,app_info.psw_ptr,app_info.dual_sys);
    result = MMIAPIPDP_Active(&app_info);
    OD_LOG("od_acitvie_gprs_with_sim--result=%d\n",result);

    if(result == FALSE)
    {

        if(g_od_gprs_cntx.pdp_protect_timer_id != 0)
        {
            MMK_StopTimer(g_od_gprs_cntx.pdp_protect_timer_id);
            g_od_gprs_cntx.pdp_protect_timer_id =0;
        }

        g_od_gprs_cntx.gprs_state = OD_GPRS_ACEIVE_FAILED;

        if(g_od_gprs_cntx.net_active_cb)
        {
            g_od_gprs_cntx.net_active_cb(0);
        }

    }
    else
    {
        g_od_gprs_cntx.gprs_state = OD_GPRS_ACTIVING;
        MMK_StopTimer(g_od_gprs_cntx.pdp_protect_timer_id);
        if(0 == g_od_gprs_cntx.pdp_protect_timer_id)
        {
            //在这里起这个timer更加准确，原来是放在brw_netconnect中起的
            g_od_gprs_cntx.pdp_protect_timer_id =  MMK_CreateWinTimer(VIRTUAL_WIN_ID,120*1000,  FALSE); //20000
        }
    }
    OD_LOG("od_acitvie_gprs_with_sim--gprs_state=%d\n",g_od_gprs_cntx.gprs_state);
    if(nv_buf)
        od_game_free(nv_buf);
}
static OD_BOOL Od_PdpProcessCheck(void )
{
    return g_od_gprs_cntx.net_active_cb?TRUE:FALSE;
}
static void Od_ResetAndDeactivePdpContext(OD_U8 sim)
{
    OD_LOG("Od_ResetAndDeactivePdpContext state=%d\n",g_od_gprs_cntx.gprs_state);
    if(g_od_gprs_cntx.pdp_protect_timer_id != 0)
    {
        MMK_StopTimer(g_od_gprs_cntx.pdp_protect_timer_id);
        g_od_gprs_cntx.pdp_protect_timer_id =0;
    }


    if(g_od_gprs_cntx.gprs_state == OD_GPRS_DEACTIVEING || g_od_gprs_cntx.gprs_state == OD_GPRS_DEACTIVE_OK \
       || g_od_gprs_cntx.gprs_state == OD_GPRS_ACEIVE_FAILED)
    {

        return;
    }

    g_od_gprs_cntx.gprs_state = OD_GPRS_DEACTIVEING;
    MMIAPIPDP_Deactive(MMI_MODULE_OD_GAME);

}
#ifdef  OD_SOCKET2_WITH_THREAD

static OD_U32 od_network_select_task_create(void)
{
    OD_U32 state;
    if(g_od_network_select_task_id != SCI_INVALID_BLOCK_ID)
    {
        // SCI_ResumeThread(g_od_network_select_task_id);
        //SCI_GetThreadInfo(g_od_network_select_task_id, OD_NULL, OD_NULL, &state,OD_NULL, OD_NULL);

        //if(TX_SEMAPHORE_SUSP == state || TX_MUTEX_SUSP == state || TX_SUSPENDED == state)
        {
            // OD_LOG(" od_network_select_task -state=%d",state);
            //SCI_ThreadExit();

        }
        // if(g_exit_task_flag== OD_FALSE)
        //{
        //  SCI_TerminateThread(g_od_network_select_task_id);
        //}
        //else
        {
            return 0;
        }
    }

    g_exit_task_flag = TRUE;
    g_od_network_select_task_id = (OD_U32)SCI_CreateAppThread(//SCI_CreateAppThread(
                                       "T_P_SELECT",
                                       "Q_P_SELECT",
                                       od_network_select_task_main,
                                       0,
                                       0,
                                       OD_NETWORK_SELECT_STACK_SIZE,
                                       OD_NETWORK_SELECT_QUEUE_NUM,
                                       OD_NETWORK_SELECT_TASK_PRIORITY,
                                       SCI_PREEMPT,
                                       SCI_AUTO_START);
    SCI_ASSERT(g_od_network_select_task_id!=SCI_INVALID_BLOCK_ID);




    return g_od_network_select_task_id;
}


static void od_network_select_task_main(OD_U32 argc, void * argv)
{
    sci_fd_set fd_send, fd_recv, fd_except;
    MMIPDP_CNF_INFO_T msg_ptr;
    od_impsock_polling(&fd_send,&fd_recv,&fd_except);

    g_od_network_select_task_id = SCI_INVALID_BLOCK_ID;

    if(g_auto_gprs_flag == OD_FALSE)
    {
        Od_ResetAndDeactivePdpContext(g_od_gprs_cntx.active_sim);
    }
    SCI_ThreadExit();
}
static void fd_init(void* fd_send, void* fd_recv, void* fd_except)
{
    OD_S32 i=0;

    SCI_FD_ZERO((sci_fd_set *)fd_send);
    SCI_FD_ZERO((sci_fd_set *)fd_recv);
    SCI_FD_ZERO((sci_fd_set *)fd_except);

    for(i=0; i < OD_MAX_SOCKET_NUM2; i++)
    {
        if (mysocket_free[i].socket_id != -1)
        {
            SCI_FD_SET(mysocket_free[i].socket_id, fd_except);
            //  OD_LOG("fd_init--select state=%d i=%d",mysocket_free[i].socket_status,i);
            //only when connecting or sending, can we poll select
            if ( (mysocket_free[i].socket_status== OD_SOCK2_CONNECTING)
                 || (mysocket_free[i].socket_status == OD_SOCK2_SENDING) )
            {

                SCI_FD_SET(mysocket_free[i].socket_id, fd_send);
                //OD_LOG("fd_init--select send-id=0x%x",mysocket_free[i].socket_id);
            }

            SCI_FD_SET(mysocket_free[i].socket_id, fd_recv);
        }
    }
}



static void od_impsock_polling(void* fd_send, void* fd_recv, void* fd_except)
{
    OD_S32 i=0;
    OD_S32 sel_result;
    OD_S32 errno;
    OD_LOG("od_impsock_polling--ENTER\n");

    while(g_exit_task_flag && od_get_pdp_state() != OD_GPRS_ACTIVE_OK)
    {

        SCI_SLEEP(SELECT_TIMEGAP);
    }
    OD_LOG("od_impsock_polling--pdp actived\n");
    while(g_exit_task_flag)
    {

        fd_init(fd_send, fd_recv, fd_except);

        sel_result = sci_sock_select(fd_recv, fd_send, fd_except, SELECT_TIMEOUT);
        //OD_LOG("od_impsock_polling--sel_result=%d\n",sel_result);

        if(sel_result) //>0 means number of ready descriptors in fd set
        {
            for(i=0; i< SOCKET_SELECT_MAX_NUMBER&&mysocket_free[i].socket_id ; i++)
            {
                e_msg_cb msg;
                if(SCI_FD_ISSET(mysocket_free[i].socket_id, fd_except))//error: out of band, or erros except for EWOULDBLOCK
                {
                    errno = sci_sock_errno(mysocket_free[i].socket_id); //todo: when select thread is preempted, errno may be changed. it's global variable


                    OD_LOG("FD EXCEPT--err=%d",errno);
                    if(errno > 0 && (EWOULDBLOCK != errno && ENOBUFS != errno&&EINPROGRESS != errno)) //todo: this is not the fact. just for temperary fix
                    {
                        //  if(errno == ECONNRESET)
                        //  {
                        //      msg = sock_cb_connreset;
                        //  }
                        //  else
                        {
                            msg = sock_cb_close;
                            //sci_sock_socketclose(mysocket_free[i].socket_id);
                            Od_socket_notify_internal(i, msg);
                        }
                        continue;//neglect the select of this socket
                    }
                }

                if(SCI_FD_ISSET(mysocket_free[i].socket_id, fd_recv)) //can recv now
                {

                    msg = sock_cb_recv;
                    Od_socket_notify_internal(i, msg);

                }

                if(SCI_FD_ISSET(mysocket_free[i].socket_id, fd_send)) //two cases: connect successfully, can send
                {
                    //OD_LOG("od_impsock_polling--fd_send socket_status=%d id=0x%x\n", mysocket_free[i].socket_status,mysocket_free[i].socket_id);
                    if(mysocket_free[i].socket_status== OD_SOCK2_CONNECTING) //connect successfully
                    {
                        msg = sock_cb_connected;

                        Od_socket_notify_internal(i, msg);
                        //EMA_network_reset_select_sock_flag(all_net_channel[i].socket, state_connecting);
                    }
                    else if(mysocket_free[i].socket_status== OD_SOCK2_SENDING) //can send now
                    {
                        msg = sock_cb_send;

                        Od_socket_notify_internal(i, msg);
                        //EMA_network_reset_select_sock_flag(all_net_channel[i].socket, state_sending);
                    }
                }
            }
        }
        else if(sel_result == 0) //timp out
        {
            //please don't add trace here, if you want, add and test in your local !
            //IMP_Trace("od_impsock_polling - sel_result == 0");
        }
        else //errors
        {

        }

        SCI_SLEEP(SELECT_TIMEGAP);

    }
    OD_LOG("od_impsock_polling--exit\n");
}
#endif

OD_U8* Od_GetApnInfo2(void)
{
    memset(OdApnInfo,0,OD_APN_INFO_LEN);
    sprintf(OdApnInfo,"%s#%s#%s",s_apn_collect.apn,s_apn_collect.username,s_apn_collect.password);
    return OdApnInfo;

}
#ifdef BROWSER_SUPPORT

static void od_BrwSetNetIndexSettingCallback(MN_DUAL_SYS_E dual_sys, uint32 index)
{
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        MMIBROWSER_NV_SETTING_T *nv_ptr = MMIBROWSER_GetNVSetting();
        MN_DUAL_SYS_E   sim_type = 0;

        sim_type = MMIBROWSER_GetSim();

        if ((dual_sys == sim_type) &&(index != nv_ptr->sim_net_index[dual_sys]))
        {
            MMIBROWSER_SetSettingChange(TRUE);
        }
        MMIBROWSER_SetNetIndexSetting(dual_sys, index);
    }

    return;
}
#endif

void Od_OpenSettingAccount2(void)
{
#ifdef BROWSER_SUPPORT
    MMIAPICONNECTION_CreateLinklistWin(MMICONNECTION_ACCOUNT_BROWSER, MMIDORADO_GetNetSettingIndex((MN_DUAL_SYS_E)g_od_gprs_cntx.active_sim),
                                       g_od_gprs_cntx.active_sim, od_BrwSetNetIndexSettingCallback);
    s_local_apn_flag=OD_TRUE;
#endif
}

void Od_SetApnProfile2(OD_BOOL flag)
{
    s_local_apn_flag=flag;
}
OD_BOOL Od_GetLocalIp2(OD_U8 * myip)
{
    long result;
    char* addr = NULL;
    result = sci_gethostaddress();
    if (result == 0)
    {
        OD_LOG("Od_GetLocalIp2--fail");
        return OD_FALSE;
    }

    /* Only support IP4 */
    addr = (char*)&result;
    sprintf(myip, "%d.%d.%d.%d", addr[0], addr[1], addr[2], addr[3]);
    OD_LOG("Od_GetLocalIp2--ip=%s",myip);
    return TRUE;
}
