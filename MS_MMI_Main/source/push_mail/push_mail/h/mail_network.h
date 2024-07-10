/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: comm_network.h
 *	Author		: sunguochen    
 *	Description	: 
 *	Vesion		: 1.0        
 *	Date			: 2010-4-6  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-6      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/

 #ifndef _MAIL_NETWORK_H_
 #define _MAIL_NETWORK_H_


#include "os_api.h"
#include "comm_socket.h"
#include "mmiconnection_export.h"
#include "mmipdp_export.h"
#include "comm_platform.h"
#include "comm_debug.h"

#include "mail_ret.h"

#include "ssl_api.h"

//可同时创建的sock session数量，目前为5
#define MAIL_NET_SESSION_NUM          5


//PDP激活重试次数
#define MAIL_NET_PDPACTIVE_TIMES      3

//PDP激活中重试次数
#define MAIL_NET_PDPACTIVING_RETRY_TIMES      3

#define MAIL_PROXY_BUFF_SIZE  1024

#define MAIL_SEND_BUFF_SIZE  16384 


#define MAIL_RECV_BUFF_SIZE  16384 

typedef enum
{
    MAIL_GPRS_DEACTIVE,   	       //DEACTIVE完成
    MAIL_GPRS_ACTIVING,		       //ACTIVing
    MAIL_GPRS_ACTIVED,		//ACTIVE完成
    MAIL_GPRS_DEACTIVING,		//DEACTIVE进行中
    MAIL_GPRS_PDP_FAIL,
    MAIL_GPRS_PDP_MAX    
    
}MAIL_PDP_STATE_E;//GPRS的状态


typedef enum
{
    MAIL_SESSION_NONE,
    MAIL_SESSION_PDP_ACTIVING,
    MAIL_SESSION_CONNECTING,
    MAIL_SESSION_PROXYING,
    MAIL_SESSION_PROXYRESULT,    
    MAIL_SESSION_SENDING,
    MAIL_SESSION_RECEIVING,
    MAIL_SESSION_CLOSING,
    MAIL_SESSION_SSLSHAKING,    //ssl shake
    MAIL_SESSION_MAX

}MAIL_SESSION_STATE_E;//GPRS的状态


typedef enum {
    MAIL_NET_NONE = 0, 
    MAIL_NET_PDP_OK,
    MAIL_NET_PDP_FAIL,
    MAIL_NET_SOCK_OK,
    MAIL_NET_SOCK_FAIL,    
    MAIL_NET_CONNECT_OK,    
    MAIL_NET_CONNECT_FAIL,
    MAIL_NET_SEND_OK,    
    MAIL_NET_SEND_FAIL,
    MAIL_NET_RECV_OK,    
    MAIL_NET_RECV_FAIL,
    MAIL_NET_SERVER_FAIL,
    MAIL_NET_CLOSED,
    MAIL_NET_PROXY_FAIL,
    MAIL_NET_SERVER_CLOSED,    
    MAIL_NET_SSL_FAIL,
    MAIL_NET_USER_CANCEL,     
    MAIL_NET_MAX    

} MAIL_NET_RESULT_E;



typedef enum
{
    MAIL_SESSION_STOP_NONE,    
    MAIL_SESSION_STOP_SENDING,
    MAIL_SESSION_STOP_RECVING,
    MAIL_SESSION_STOP_ALL,
    MAIL_SESSION_STOP_MAX

}MAIL_SESSION_STOP_STATE_E;//当前操作



typedef struct _mail_net_buf_t
{
    char *                      data_buff;
    comm_uint32                 sent_len;
    comm_uint32                 total_len;
    comm_boolean               need_free;
    comm_boolean               need_encrpt;
    
    struct _mail_net_buf_t*  next;

}mail_net_buf_t;


typedef struct _mail_net_sock_t
{
    int                       accountid;
    MAIL_SESSION_STOP_STATE_E curr_action;
    comm_sockaddr               proxy_addr;    
    comm_sockaddr               server_addr;
    //MAIL_PDP_STATE_E          pdp_state;
    MAIL_SESSION_STATE_E   session_state;
    MAIL_NET_RESULT_E        net_result;
    int                                 sock_handle;
    SSL_HANDLE                   ssl_handle;    
    comm_boolean                sock_is_polling;
    comm_boolean                net_need_proxy;
    comm_boolean                sock_is_ssl;
    comm_boolean                sock_is_resending;       
    char*                             server_connect_cmd;
    char*                             server_connect_result;//added by liangshun 20110118 for MS00223083
                                                                          //server_connect_result记录"220 China Mobile push proxy v1.0 ready"
    void*                             socketInd;    //sock 响应
    void*                             userData;     //pop3 session infomation notify function

    void*                             dataInd;       //data arrival infomation notify function

    mail_net_buf_t*              send_data;   //sending data buffer
    mail_net_buf_t*              recv_data;    //receiving data buffer
    int                                 unrecved_data_num;
     
    void*                             underFlowInd;

    int                                 limit;

    char *pend_buff;                    
    int   pend_buff_len;                
    int   pend_buff_size;               
    comm_uint32 networkId;
    char* domainName;

    unsigned char binAddr[4];           
    comm_uint16 binAddrLen;

}mail_net_sock_t;


typedef struct _mail_net_session_list_t_
{

    int		                  net_id;                  //actived pdp id
    
    int                               session_num;//session num
    int                               pdp_active_times; //pdp active times
    int                               pdp_activing_retry_times; //pdp激活中重试次数
    MAIL_PDP_STATE_E       g_mail_pdp_state;//pdp state    
    comm_boolean              mail_pdp_need_deactive;
    comm_boolean              mail_pdp_need_active;
    
    mail_net_sock_t*       session_list[MAIL_NET_SESSION_NUM];//session list
    
}mail_net_session_list_t;


typedef mail_net_sock_t MailNetSession;

typedef void MailNetSocketInfoInd(int result, mail_net_sock_t* netInstance, void* userData);

typedef void MailNetReceiveDataInd(mail_net_sock_t* netInstance, char* buf, int length, void* userData);

typedef void MailNetUnderflowInd(mail_net_sock_t* netInstance, void* userData);




/*==========================================================
 * Function 	: mail_NetInitial
 * Author		: sunguochen      
 * Description : 网络初始化
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/15/2010 
 ==========================================================*/
void mail_NetInitial(void);



/*==========================================================
 * Function 	: mail_NetFinally
 * Author		: sunguochen      
 * Description : 应用退出最后时调用，
                     确定所有session 已经结束
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/15/2010 
 ==========================================================*/
void mail_NetFinally(void);



/*==========================================================
 * Function 	: mail_NetSessionValid
 * Author		: sunguochen      
 * Description : 判断socket session 是否合法
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/4/2010 
 ==========================================================*/
comm_boolean mail_NetSessionValid(mail_net_sock_t* sock);



/*==========================================================
 * Function 	: mail_NetCreateRetrSession
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/12/2010 
 ==========================================================*/
mail_net_sock_t* mail_NetCreateRetrSession(
                                                  int accountid,
                                                  void* rspFn, 
                                                  void* receiveDataInd, 
                                                  void* userData);



/*==========================================================
 * Function 	: mail_NetCreateSendSession
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/12/2010 
 ==========================================================*/
mail_net_sock_t* mail_NetCreateSendSession(
                                                  int    accuntid,
                                                  void* rspFn, 
                                                  void* receiveDataInd, 
                                                  void* userData);



/*==========================================================
 * Function 	: mail_NetCloseSession
 * Author		: sunguochen      
 * Description : 关闭连接
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/15/2010 
 ==========================================================*/
void mail_NetCloseSession(mail_net_sock_t* net_id);



/*==========================================================
 * Function 	: mail_NetSendData
 * Author		: sunguochen      
 * Description : 发送数据
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/15/2010 
 ==========================================================*/
void mail_NetSendData(mail_net_sock_t* net_id, comm_boolean freeData, char* buf, int length);



/*==========================================================
 * Function 	: mail_NetReceiveData
 * Author		: sunguochen      
 * Description : 接收数据
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/15/2010 
 ==========================================================*/
void mail_NetReceiveData(mail_net_sock_t* session);


/*==========================================================
 * Function 	: mail_NetPdpDeactive
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/15/2010 
 ==========================================================*/
void mail_NetPdpDeactive(void);



/*==========================================================
 * Function 	: mail_NetPdpDeactiveDelay
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/15/2010 
 ==========================================================*/
void mail_NetPdpDeactiveDelay(void);



/*==========================================================
 * Function 	: mail_NetPdpRetry
 * Author		: sunguochen      
 * Description :  pdp多次重试机制
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  9/8/2010 
 ==========================================================*/
comm_boolean mail_NetPdpRetry(mail_net_sock_t * sock_session);



/*==========================================================
 * Function 	: mail_NetNotifyHandler
 * Author		: sunguochen      
 * Description : 网络事件处理
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/16/2010 
 ==========================================================*/
void mail_NetNotifyHandler(int32 sock_session);



/*==========================================================
 * Function 	: mail_NetStateQuery
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/20/2010 
 ==========================================================*/
void mail_NetStateQuery(mail_net_sock_t* net_session);


/*==========================================================
 * Function 	: mail_NetRegisterFlowInd
 * Author		: sunguochen      
 * Description : 注册网络通知事件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
void mail_NetRegisterFlowInd(mail_net_sock_t* session, MailNetUnderflowInd* underFlowInd, int limit);



/*==========================================================
 * Function 	: mail_NetDeregisterFlowInd
 * Author		: sunguochen      
 * Description : 取消注册网络通知事件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/27/2010 
 ==========================================================*/
void mail_NetDeregisterFlowInd(mail_net_sock_t* session);


/*==========================================================
 * Function 	: mail_StopNetAction
 * Author		: sunguochen      
 * Description :  停止当前收发操作
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/28/2010 
 ==========================================================*/
void mail_StopNetAction(MAIL_SESSION_STOP_STATE_E stop_type);



/*==========================================================
 * Function 	: mail_NetDelAllSendingBuf
 * Author		: sunguochen      
 * Description :删除全部发送队列
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/19/2010 
 ==========================================================*/
void mail_NetDelAllSendingBuf(mail_net_sock_t* net_id);



/*==========================================================
 * Function 	: mail_NetDelAllRecvingBuf
 * Author		: sunguochen      
 * Description :删除全部接收队列
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/19/2010 
 ==========================================================*/
void mail_NetDelAllRecvingBuf(mail_net_sock_t* net_id);



/*==========================================================
 * Function 	: mail_HandleStopAction
 * Author		: sunguochen      
 * Description : 停止当前进行的收发操作
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/28/2010 
 ==========================================================*/
void mail_HandleStopAction(MAIL_SESSION_STOP_STATE_E stop_type);



/*==========================================================
 * Function 	: mail_NetDoSend
 * Author		: sunguochen      
 * Description : 发送数据
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/19/2010 
 ==========================================================*/
void mail_NetDoSend(mail_net_sock_t* net_id);



/*==========================================================
 * Function 	: mail_NetDataSentProcess
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/19/2010 
 ==========================================================*/
void mail_NetDataSentProcess(int sock_handle);



/*==========================================================
 * Function 	: mail_NetDataRecvProcess
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/19/2010 
 ==========================================================*/
void mail_NetDataRecvProcess(int sock_handle);



/*==========================================================
 * Function 	: mail_NetConnectedProcess
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/19/2010 
 ==========================================================*/
void mail_NetConnectedProcess(comm_uint32 sock_handle, comm_uint32 result);



/*==========================================================
 * Function 	: mail_NetUnrecvedDataNum
 * Author		: sunguochen      
 * Description : 获取未接收的数据个数
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/18/2010 
 ==========================================================*/
int mail_NetUnrecvedDataNum(mail_net_sock_t* session);



/*==========================================================
 * Function 	: mail_NetworkErrorProcess
 * Author		: sunguochen      
 * Description : 通知错误值给收发处理
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/24/2010 
 ==========================================================*/
void mail_NetworkErrorProcess(int32 sock_session);



/*==========================================================
 * Function 	: mail_NetConnectionClosedProcess
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/19/2010 
 ==========================================================*/
void mail_NetConnectionClosedProcess(int sock_handle);



/*==========================================================
 * Function 	: mail_NetStartSSL
 * Author		: sunguochen      
 * Description : 开始ssl过程
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/2/2010 
 ==========================================================*/
comm_boolean mail_NetStartSSL(int sock_handle);



/*==========================================================
 * Function 	: mail_sslDescryptOk
 * Author		: sunguochen      
 * Description : 数据解密完毕
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/4/2010 
 ==========================================================*/
void mail_sslDescryptOk(mail_net_sock_t* net_sess);



/*==========================================================
 * Function 	: mail_SecurityPostMessage
 * Author		: sunguochen      
 * Description : ssl 事件处理函数
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/2/2010 
 ==========================================================*/
void mail_sslEventProcess(mail_net_sock_t *sock_session, comm_uint32 message_id);





 #endif

