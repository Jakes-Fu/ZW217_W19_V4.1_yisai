#ifndef __ZDT_TCP_H__
#define __ZDT_TCP_H__

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


#define TCP_RCV_SIZE 10240

#define ZDT_TCP_RET_OK 0
#define ZDT_TCP_RET_ERR 1

#ifdef FILE_LOG_SUPPORT
#define TCP_LOG			Trace_Log_Buf_Print
#else
#define TCP_LOG            SCI_TRACE_LOW
#endif

typedef enum _ZDT_TCP_TASK_SIGNAL
{
	SIG_ZDT_TCP_START = 0xF501,
	SIG_ZDT_TCP_CONNET,
	SIG_ZDT_TCP_WRITE,
	SIG_ZDT_TCP_RCV,
	SIG_ZDT_TCP_MAX
}ZDT_TCP_TASK_SIG_E;

typedef enum
{
    TCP_SUCCESS = 0,
    TCP_ERROR,
    TCP_ERR_REGEXIST, //已注册
    TCP_ERR_NOUIM, //无卡
    TCP_ERR_NOPS,   //无网络
    TCP_ERR_SOCKET,
    TCP_ERR_DISCONNECT,
    TCP_ERR_NET,
    TCP_ERR_NET_EMFILE,
    TCP_ERR_NET_NOSYS,
    TCP_ERR_NET_WRITE,
    TCP_ERR_NET_READ,
    TCP_ERR_LINKFULL,
    TCP_ERR_MEMFULL,
    TCP_ERR_NOREG,
    TCP_ERR_NOOPEN,
    TCP_ERR_NODATA,
    TCP_ERR_IP,
    TCP_ERR_TIMEOUT,
    TCP_ERR_READ,
    TCP_ERR_DATA,
    TCP_ERR_CLEAR,
    TCP_ERR_INCALL,
    TCP_ERR_MAX
}TCP_ERR_TYPE_E;

typedef enum
{
    TCP_CB_NULL,
    TCP_CB_REG,
    TCP_CB_SEND,
    TCP_CB_RCV,
    TCP_CB_UNREG,
    TCP_CB_DISCONNECTED,
    TCP_CB_CONNECTED,
    TCP_CB_MAX
}TCP_CB_TYPE_E;

typedef enum _ZDT_TCP_SOCKET_STATE_e
{
    TCP_SOCKET_STATE_NULL,
    TCP_SOCKET_STATE_OPEN,  //底层未分配Sockefd
    TCP_SOCKET_STATE_CONNECTING,    //正在连接
    TCP_SOCKET_STATE_CONNECTED, //连接成功
    TCP_SOCKET_STATE_WRITING,    //正在写数据
    TCP_SOCKET_STATE_DISCONNECTED  //连接已断开但已分配Socketfd
}ZDTTCP_SOCKET_STATE_E;

typedef int (*TCPCLHANDLER) (void *pUser,TCP_CB_TYPE_E cb_type,uint16 wParam, void * dwParam);
typedef int (*TCPRCVHANDLER) (void *pUser,uint8 * pRcv,uint32 Rcv_len);
typedef uint32 (*CMDCHECKHANDLER)(uint8 * pData, int Data_len,uint8 **ppCmdBuf, uint32 * cmd_len);

typedef struct _ZDT_TCP_RCV_DATA_t
{
    void *pMe;
    uint8          * pRcv;
    uint32         len;
} ZDT_TCP_RCV_DATA_T;

typedef struct _ZDT_TCP_RCV_NODE_t
{
       ZDT_TCP_RCV_DATA_T  data;
       struct _ZDT_TCP_RCV_NODE_t * next_ptr;
} ZDT_TCP_RCV_NODE_T;

typedef struct _ZDT_TCP_REG_DATA_t
{
    void *pMe;
    char          * conn_ip;
    uint16         conn_port;
    TCPCLHANDLER callback;
} ZDT_TCP_REG_DATA_T;

typedef struct _ZDT_TCP_LINK_DATA_t
{
    void * pMe;
    uint8          * str;
    uint32         len;
    uint8 times;
    uint8 priority;
    uint32 timeout;
    TCPRCVHANDLER rcv_handle;
} ZDT_TCP_LINK_DATA_T;

typedef struct _ZDT_TCP_LINK_t
{
       ZDT_TCP_LINK_DATA_T *  data;
       struct _ZDT_TCP_LINK_t * next_ptr;
} ZDT_TCP_LINK_NODE_T;


typedef struct _ZDT_TCP_DATA_t
{
    uint16 type;
    void* data_p;
} ZDT_TCP_DATA_T;

typedef struct
{
    SIGNAL_VARS
    ZDT_TCP_DATA_T * p_data;
} ZDT_TCP_DATA_SIG_T;

typedef struct _ZDT_TCP_TASK_DATA_t
{
    void * pMe;
    uint8 * str;
    uint32 str_len;
    uint16 port;
    uint32 ip_addr;
    uint8  * ip_str;
} ZDT_TCP_TASK_DATA_T;

typedef struct
{
    SIGNAL_VARS
    ZDT_TCP_TASK_DATA_T * p_task_data;
} ZDT_TCP_TASK_SIG_T;

typedef struct _ZDT_TCP_INTERFACE_t {
    BLOCK_ID m_tcp_task_id;
    uint16 m_tcp_interface_idx;
    BOOLEAN m_tcp_net_is_init;
    CMDCHECKHANDLER m_tcp_cmd_check_handle;
    ZDTTCP_SOCKET_STATE_E  m_tcp_socket_status;
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

    ZDT_TCP_LINK_DATA_T * m_tcp_cur_data;
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
    ZDT_TCP_RCV_NODE_T  * m_tcp_rcv_head;
    ZDT_TCP_LINK_NODE_T  * m_tcp_link_head;
    uint32                 m_tcp_rcv_needlen;
    TCPCLHANDLER   m_tcp_callback;

    uint8                       m_tcp_pdp_timer_id;
    uint8                       m_tcp_connect_timer_id;
    uint8                       m_tcp_write_timer_id;
    uint8                       m_tcp_rewrite_timer_id;
    uint8                       m_tcp_read_timer_id;
    BOOLEAN                  m_tcp_wait_net;
    ZDT_TCP_REG_DATA_T * m_tcp_reg_data;
}ZDT_TCP_INTERFACE_T;

extern int ZdtTCP_AppInit(void);
extern int ZdtTCP_AppFree(void);
extern MMI_RESULT_E  MMIZDT_TCP_Handle_AppMsg (PWND app_ptr, uint16 msg_id,DPARAM param);
extern BOOLEAN ZdtTCP_NetInit(void);
extern BOOLEAN ZdtTCP_NetReset(void);
extern BOOLEAN ZdtTCP_NetOpen(ZDT_TCP_INTERFACE_T ** ppMe,CMDCHECKHANDLER cmd_handle,uint16 rcv_len);
extern BOOLEAN ZdtTCP_NetClose(ZDT_TCP_INTERFACE_T * pMe);
extern void ZdtTCP_CloseAllTimer(ZDT_TCP_INTERFACE_T *pMe);
extern int ZdtTCP_Reset(ZDT_TCP_INTERFACE_T *pMe);
extern int ZDT_API_TCP_Reg(ZDT_TCP_INTERFACE_T *pMe,char * str_ip_url,uint16 conn_port,TCPCLHANDLER callback);
extern int ZDT_API_TCP_UnReg(ZDT_TCP_INTERFACE_T *pMe);
extern uint32 ZDT_API_TCP_SendData(ZDT_TCP_INTERFACE_T *pMe,uint8 * pData,uint16 Data_len,uint8 priority,uint8 repeat_times,uint32 timeout,TCPRCVHANDLER rcv_handle);
extern int ZDT_API_TCP_SendDataNow(ZDT_TCP_INTERFACE_T *pMe,ZDT_TCP_LINK_DATA_T * pData);
extern int ZDTTCP_CleanSendLink(ZDT_TCP_INTERFACE_T *pMe);
extern BOOLEAN ZDT_API_TCP_AllowClose(ZDT_TCP_INTERFACE_T *pMe);
extern BOOLEAN  MMIZDT_TCP_Handle_PDPActiveOK(void);
extern BOOLEAN  MMIZDT_TCP_Handle_PDPActiveFail(void);
extern BOOLEAN  MMIZDT_TCP_Handle_PDPRespond(void);


#ifdef __cplusplus
    }
#endif

#endif

