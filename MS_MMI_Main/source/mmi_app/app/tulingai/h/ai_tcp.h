/*****************************************************************************
** File Name:      mmicc_id.h                                                *
** Author:                                                                   *
** Date:           2006/09/26                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe cc win and control id       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/26     kelly.li        Create                                    *
******************************************************************************/

#ifndef _MMI_AI_TCP_H_
#define _MMI_AI_TCP_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
 
#define WIN_ID_DEF(win_id, win_id_name)          win_id,
 
// window ID
typedef enum
{
    MMI_AI_CHAT_WIN_ID_START = (MMI_MODULE_TULINGAI << 16),

#include "tuling_id.def"


    MMI_TULING_MAX_WIN_ID
}MMI_TULING_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMI_TULING_CTRL_ID_START = MMI_TULING_MAX_WIN_ID,//(MMI_MODULE_CC << 16),
    //MMI_TULING_TEST_MENU_CTRL_ID,
    MMIAI_CHAT_LIST_CTRL_ID,
    MMI_TULING_MAX_CTRL_ID
}AI_CHAT_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#define AI_TCP_RCV_SIZE 10240
    
#define AI_TCP_RET_OK 0
#define AI_TCP_RET_ERR 1


#define TCP_LOG            SCI_TRACE_LOW


typedef enum _AI_TCP_TASK_SIGNAL
{
	SIG_AI_TCP_START = 0xF501,
	SIG_AI_TCP_CONNET,
	SIG_AI_TCP_WRITE,
	SIG_AI_TCP_RCV,
	SIG_AI_TCP_MAX
}AI_TCP_TASK_SIG_E;

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
}AI_TCP_ERR_TYPE_E;

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
}AI_TCP_CB_TYPE_E;

typedef enum _AI_TCP_SOCKET_STATE_e
{
    TCP_SOCKET_STATE_NULL,
    TCP_SOCKET_STATE_OPEN,  //底层未分配Sockefd
    TCP_SOCKET_STATE_CONNECTING,    //正在连接
    TCP_SOCKET_STATE_CONNECTED, //连接成功
    TCP_SOCKET_STATE_WRITING,    //正在写数据
    TCP_SOCKET_STATE_DISCONNECTED  //连接已断开但已分配Socketfd
}AITCP_SOCKET_STATE_E;

typedef int (*AITCPCBHANDLER) (void *pUser,AI_TCP_CB_TYPE_E cb_type,uint16 wParam, void * dwParam);
typedef int (*AITCPRCVHANDLER) (void *pUser,uint8 * pRcv,uint32 Rcv_len);
typedef uint32 (*AICMDCHECKHANDLER)(uint8 * pData, int Data_len,uint8 **ppCmdBuf, uint32 * cmd_len);

typedef struct _AI_TCP_RCV_DATA_t
{
    void *pMe;
    uint8          * pRcv;
    uint32         len;
} AI_TCP_RCV_DATA_T;

typedef struct _AI_TCP_RCV_NODE_t
{
       AI_TCP_RCV_DATA_T  data;
       struct _AI_TCP_RCV_NODE_t * next_ptr;
} AI_TCP_RCV_NODE_T;

typedef struct _AI_TCP_REG_DATA_t
{
    void *pMe;
    char          * conn_ip;
    uint16         conn_port;
    AITCPCBHANDLER callback;
} AI_TCP_REG_DATA_T;

typedef struct _AI_TCP_LINK_DATA_t
{
    void * pMe;
    uint8          * str;
    uint32         len;
    uint8 times;
    uint8 priority;
    uint32 timeout;
    AITCPRCVHANDLER rcv_handle;
} AI_TCP_LINK_DATA_T;

typedef struct _AI_TCP_LINK_t
{
       AI_TCP_LINK_DATA_T *  data;
       struct _AI_TCP_LINK_t * next_ptr;
} AI_TCP_LINK_NODE_T;


typedef struct _AI_TCP_DATA_t
{
    uint16 type;
    void* data_p;
} AI_TCP_DATA_T;

typedef struct
{
    SIGNAL_VARS
    AI_TCP_DATA_T * p_data;
} AI_TCP_DATA_SIG_T;

typedef struct _AI_TCP_TASK_DATA_t
{
    void * pMe;
    uint8 * str;
    uint32 str_len;
    uint16 port;
    uint32 ip_addr;
    uint8  * ip_str;
} AI_TCP_TASK_DATA_T;

typedef struct
{
    SIGNAL_VARS
    AI_TCP_TASK_DATA_T * p_task_data;
} AI_TCP_TASK_SIG_T;

typedef struct _AI_TCP_INTERFACE_t 
{
    BLOCK_ID m_tcp_task_id;
    uint16 m_tcp_interface_idx;
    BOOLEAN m_tcp_net_is_init;
    AICMDCHECKHANDLER m_tcp_cmd_check_handle;
    AITCP_SOCKET_STATE_E  m_tcp_socket_status;
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

    AI_TCP_LINK_DATA_T * m_tcp_cur_data;
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
    AI_TCP_RCV_NODE_T  * m_tcp_rcv_head;
    AI_TCP_LINK_NODE_T  * m_tcp_link_head;
    uint32                 m_tcp_rcv_needlen;
    AITCPCBHANDLER   m_tcp_callback;

    uint8                       m_tcp_pdp_timer_id;
    uint8                       m_tcp_connect_timer_id;
    uint8                       m_tcp_write_timer_id;
    uint8                       m_tcp_rewrite_timer_id;
    uint8                       m_tcp_read_timer_id;
    BOOLEAN                  m_tcp_wait_net;
    AI_TCP_REG_DATA_T * m_tcp_reg_data;
}AI_TCP_INTERFACE_T;

extern int AITCP_AppInit(void);
extern int AITCP_AppFree(void);
extern MMI_RESULT_E  MMIAI_TCP_Handle_AppMsg (PWND app_ptr, uint16 msg_id,DPARAM param);
extern BOOLEAN AITCP_NetInit(void);
extern BOOLEAN AITCP_NetOpen(AI_TCP_INTERFACE_T ** ppMe,AICMDCHECKHANDLER cmd_handle,uint16 rcv_len);
extern BOOLEAN AITCP_NetClose(AI_TCP_INTERFACE_T * pMe);
extern void AITCP_CloseAllTimer(AI_TCP_INTERFACE_T *pMe);
extern int AITCP_Reset(AI_TCP_INTERFACE_T *pMe);
extern int AI_API_TCP_Reg(AI_TCP_INTERFACE_T *pMe,char * str_ip_url,uint16 conn_port,AITCPCBHANDLER callback);
extern int AI_API_TCP_UnReg(AI_TCP_INTERFACE_T *pMe);
extern uint32 AI_API_TCP_SendData(AI_TCP_INTERFACE_T *pMe,uint8 * pData,uint16 Data_len,uint8 priority,uint8 repeat_times,uint32 timeout,AITCPRCVHANDLER rcv_handle);
extern int AI_API_TCP_SendDataNow(AI_TCP_INTERFACE_T *pMe,AI_TCP_LINK_DATA_T * pData);
extern int AITCP_CleanSendLink(AI_TCP_INTERFACE_T *pMe);
extern BOOLEAN AI_API_TCP_AllowClose(AI_TCP_INTERFACE_T *pMe);
extern BOOLEAN  MMIAI_TCP_Handle_PDPActiveOK(void);
extern BOOLEAN  MMIAI_TCP_Handle_PDPActiveFail(void);
extern BOOLEAN  MMIAI_TCP_Handle_PDPRespond(void);
extern void AI_Tcp_ForceCloseAll(void);



 
#ifdef   __cplusplus
    }
#endif

#endif //_MMISET_ID_H_
