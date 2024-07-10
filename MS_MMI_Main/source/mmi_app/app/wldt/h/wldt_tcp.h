#ifndef __WLDT_TCP_H__
#define __WLDT_TCP_H__

#ifdef __cplusplus
    extern "C"
    {
#endif
#include "mmk_app.h"
#include "mn_type.h"
#include "mmi_nv.h"
#include "sci_types.h"
#include "mmi_common.h"
#include <stdio.h>
#include "nvitem.h"
#ifdef _RTOS
#include "sci_api.h" /*@tony.yao  replase sci_mem.h with sci_api.h*/
#else
#include "tasks_id.h"
#endif
#include "mn_api.h"
#include "mn_events.h"
#include "mn_type.h"
#include "nv_item_id.h"
#include "sio.h"
#include "sig_code.h"
#include "os_api.h"
#include "mmi_signal_ext.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h"
#include "IN_message.h"
#include "socket_types.h"


#define WLDT_TCP_RCV_SIZE 10240

#define WLDT_TCP_RET_OK 0
#define WLDT_TCP_RET_ERR 1

#define WLDT_TCP_LOG            WLDT_TCPTrace


typedef enum _WLDT_TCP_TASK_SIGNAL
{
	SIG_WLDT_TCP_START = 0xF501,
	SIG_WLDT_TCP_CONNET,
	SIG_WLDT_TCP_WRITE,
	SIG_WLDT_TCP_RCV,
	SIG_WLDT_TCP_MAX
}WLDT_TCP_TASK_SIG_E;

typedef enum
{
    WLDT_TCP_SUCCESS = 0,
    WLDT_TCP_ERROR,
    WLDT_TCP_ERR_REGEXIST, //已注册
    WLDT_TCP_ERR_NOUIM, //无卡
    WLDT_TCP_ERR_NOPS,   //无网络
    WLDT_TCP_ERR_SOCKET,
    WLDT_TCP_ERR_DISCONNECT,
    WLDT_TCP_ERR_NET,
    WLDT_TCP_ERR_NET_EMFILE,
    WLDT_TCP_ERR_NET_NOSYS,
    WLDT_TCP_ERR_NET_WRITE,
    WLDT_TCP_ERR_NET_READ,
    WLDT_TCP_ERR_LINKFULL,
    WLDT_TCP_ERR_MEMFULL,
    WLDT_TCP_ERR_NOREG,
    WLDT_TCP_ERR_NOOPEN,
    WLDT_TCP_ERR_NODATA,
    WLDT_TCP_ERR_IP,
    WLDT_TCP_ERR_TIMEOUT,
    WLDT_TCP_ERR_READ,
    WLDT_TCP_ERR_DATA,
    WLDT_TCP_ERR_CLEAR,
    WLDT_TCP_ERR_INCALL,
    WLDT_TCP_ERR_MAX
}WLDT_TCP_ERR_TYPE_E;

typedef enum
{
    WLDT_TCP_CB_NULL,
    WLDT_TCP_CB_REG,
    WLDT_TCP_CB_SEND,
    WLDT_TCP_CB_RCV,
    WLDT_TCP_CB_UNREG,
    WLDT_TCP_CB_DISCONNECTED,
    WLDT_TCP_CB_CONNECTED,
    WLDT_TCP_CB_MAX
}WLDT_TCP_CB_TYPE_E;

typedef enum _WLDT_WLDT_TCP_SOCKET_STATE_e
{
    WLDT_TCP_SOCKET_STATE_NULL,
    WLDT_TCP_SOCKET_STATE_OPEN,  //底层未分配Sockefd
    WLDT_TCP_SOCKET_STATE_CONNECTING,    //正在连接
    WLDT_TCP_SOCKET_STATE_CONNECTED, //连接成功
    WLDT_TCP_SOCKET_STATE_WRITING,    //正在写数据
    WLDT_TCP_SOCKET_STATE_DISCONNECTED  //连接已断开但已分配Socketfd
}WLDTWLDT_TCP_SOCKET_STATE_E;

typedef int (*WLDTTCPCLHANDLER) (void *pUser,WLDT_TCP_CB_TYPE_E cb_type,uint16 wParam, void * dwParam);
typedef int (*WLDTTCPRCVHANDLER) (void *pUser,uint8 * pRcv,uint32 Rcv_len);
typedef uint32 (*WLDTCMDCHECKHANDLER)(uint8 * pData, int Data_len,uint8 **ppCmdBuf, uint32 * cmd_len);

typedef struct _WLDT_TCP_RCV_DATA_t
{
    void *pMe;
    uint8          * pRcv;
    uint32         len;
} WLDT_TCP_RCV_DATA_T;

typedef struct _WLDT_TCP_RCV_NODE_t
{
       WLDT_TCP_RCV_DATA_T  data;
       struct _WLDT_TCP_RCV_NODE_t * next_ptr;
} WLDT_TCP_RCV_NODE_T;

typedef struct _WLDT_TCP_REG_DATA_t
{
    void *pMe;
    char          * conn_ip;
    uint16         conn_port;
    WLDTTCPCLHANDLER callback;
} WLDT_TCP_REG_DATA_T;

typedef struct _WLDT_TCP_LINK_DATA_t
{
    void * pMe;
    uint8          * str;
    uint32         len;
    uint8 times;
    uint8 priority;
    uint32 timeout;
    WLDTTCPRCVHANDLER rcv_handle;
} WLDT_TCP_LINK_DATA_T;

typedef struct _WLDT_TCP_LINK_t
{
       WLDT_TCP_LINK_DATA_T *  data;
       struct _WLDT_TCP_LINK_t * next_ptr;
} WLDT_TCP_LINK_NODE_T;


typedef struct _WLDT_TCP_DATA_t
{
    uint16 type;
    void* data_p;
} WLDT_TCP_DATA_T;

typedef struct
{
    SIGNAL_VARS
    WLDT_TCP_DATA_T * p_data;
} WLDT_TCP_DATA_SIG_T;

typedef struct _WLDT_TCP_TASK_DATA_t
{
    void * pMe;
    uint8 * str;
    uint32 str_len;
    uint16 port;
    uint32 ip_addr;
    uint8  * ip_str;
} WLDT_TCP_TASK_DATA_T;

typedef struct
{
    SIGNAL_VARS
    WLDT_TCP_TASK_DATA_T * p_task_data;
} WLDT_TCP_TASK_SIG_T;

typedef struct _WLDT_TCP_INTERFACE_t {
    BLOCK_ID m_tcp_task_id;
    uint16 m_tcp_interface_idx;
    BOOLEAN m_tcp_net_is_init;
    WLDTCMDCHECKHANDLER m_tcp_cmd_check_handle;
    WLDTWLDT_TCP_SOCKET_STATE_E  m_tcp_socket_status;
    TCPIP_SOCKET_T            m_tcp_pISocket;             // Pointer to socket
    TCPIP_SOCKET_T            m_tcp_pISocket_close;             // Pointer to socket
    uint16                m_tcp_last_err;

    BOOLEAN		   m_tcp_need_connect;
    BOOLEAN            m_tcp_socket_is_connected;
    char *               m_tcp_ip_url;
    uint32               m_tcp_conn_ip;
    uint16                m_tcp_conn_port;
    uint8		        m_tcp_reconn_times;
    BOOLEAN		        m_tcp_need_read;

    WLDT_TCP_LINK_DATA_T * m_tcp_cur_data;
    uint8 *               m_tcp_buf_get;
    uint32               m_tcp_get_len;          // length of pszMsg (calculated when set)
    uint8 *                m_tcp_buf_rcv;
    uint32                 m_tcp_rcv_len;              // Index used for sending TCP data
    BOOLEAN                 m_tcp_is_reading;
    uint32               m_tcp_get_curlen;              // Index used for sending TCP data

    uint8				m_tcp_connwrite;
    uint8				m_tcp_closewrite;
    uint8				m_tcp_closeconn;

    BOOLEAN             m_tcp_is_sending;
    BOOLEAN             m_tcp_need_rcv;

    BOOLEAN             m_tcp_is_reg;
    WLDT_TCP_RCV_NODE_T  * m_tcp_rcv_head;
    WLDT_TCP_LINK_NODE_T  * m_tcp_link_head;
    uint32                 m_tcp_rcv_needlen;
    WLDTTCPCLHANDLER   m_tcp_callback;

    uint8                       m_tcp_pdp_timer_id;
    uint8                       m_tcp_connect_timer_id;
    uint8                       m_tcp_write_timer_id;
    uint8                       m_tcp_rewrite_timer_id;
    uint8                       m_tcp_read_timer_id;
    BOOLEAN                  m_tcp_wait_net;
    WLDT_TCP_REG_DATA_T * m_tcp_reg_data;
}WLDT_TCP_INTERFACE_T;

extern int WldtTCP_AppInit(void);
extern int WldtTCP_AppFree(void);
extern MMI_RESULT_E  MMIWLDT_TCP_Handle_AppMsg (PWND app_ptr, uint16 msg_id,DPARAM param);
extern BOOLEAN WldtTCP_NetInit(void);
extern BOOLEAN WldtTCP_NetReset(void);
extern BOOLEAN WldtTCP_NetOpen(WLDT_TCP_INTERFACE_T ** ppMe,WLDTCMDCHECKHANDLER cmd_handle,uint16 rcv_len);
extern BOOLEAN WldtTCP_NetClose(WLDT_TCP_INTERFACE_T * pMe);
extern void WldtTCP_CloseAllTimer(WLDT_TCP_INTERFACE_T *pMe);
extern int WldtTCP_Reset(WLDT_TCP_INTERFACE_T *pMe);
extern int WLDT_API_TCP_Reg(WLDT_TCP_INTERFACE_T *pMe,char * str_ip_url,uint16 conn_port,WLDTTCPCLHANDLER callback);
extern int WLDT_API_TCP_UnReg(WLDT_TCP_INTERFACE_T *pMe);
extern uint32 WLDT_API_TCP_SendData(WLDT_TCP_INTERFACE_T *pMe,uint8 * pData,uint16 Data_len,uint8 priority,uint8 repeat_times,uint32 timeout,WLDTTCPRCVHANDLER rcv_handle);
extern int WLDT_API_TCP_SendDataNow(WLDT_TCP_INTERFACE_T *pMe,WLDT_TCP_LINK_DATA_T * pData);
extern int WLDTTCP_CleanSendLink(WLDT_TCP_INTERFACE_T *pMe);
extern BOOLEAN WLDT_API_TCP_AllowClose(WLDT_TCP_INTERFACE_T *pMe);
extern BOOLEAN  MMIWLDT_TCP_Handle_PDPActiveOK(void);
extern BOOLEAN  MMIWLDT_TCP_Handle_PDPActiveFail(void);
extern BOOLEAN  MMIWLDT_TCP_Handle_PDPRespond(void);


#ifdef __cplusplus
    }
#endif

#endif

