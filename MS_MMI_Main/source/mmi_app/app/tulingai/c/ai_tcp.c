

#include "std_header.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "mmi_text.h"
#include "mmi_common.h"
#include "window_parse.h"
#include "guitext.h"
#include "guilcd.h"
#include "mmi_menutable.h"
#include "guilistbox.h"
#include "mmi_image.h"
#include "guiedit.h"
#include "mmipub.h"
#include "mmi_appmsg.h"
#include "sig_code.h"
#include "mmiset_export.h"
#include "mmienvset_export.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h"
#include "IN_message.h"
#include "socket_types.h"
#include "socket_api.h"

#include "ai_app.h"

#ifdef WIFI_SUPPORT
//#include "mmiwifi_export.h"
#endif



#define AI_TCP_INTERFACE_MAX 5
#define TCP_PDP_CLOSE_TIMEOUT 5000

AI_TCP_INTERFACE_T  g_ai_tcp_interface_array[AI_TCP_INTERFACE_MAX] = {0};
BOOLEAN                  m_ai_tcp_pdpstate = FALSE;

static uint8 AITcp_TCP_Cancel(AI_TCP_INTERFACE_T * pMe);
static uint8 AITcp_Connect(AI_TCP_INTERFACE_T * pMe);
static int AITCP_Start(AI_TCP_INTERFACE_T *pMe);
static int AITCP_ReStart(AI_TCP_INTERFACE_T *pMe);
static void AITCP_DNSConnect(AI_TCP_INTERFACE_T * pMe);

PUBLIC BOOLEAN  AI_TCP_PostEvtTo_APP(AI_APP_SIG_E sig_id, void* p_data, uint16 type)
{
    uint8 * pstr = PNULL;
    AI_TCP_DATA_SIG_T * psig = PNULL;    
    AI_TCP_DATA_T * cur_data = PNULL;
    
    cur_data=(AI_TCP_DATA_T *)SCI_ALLOC_APPZ(sizeof(AI_TCP_DATA_T));
    if(cur_data == PNULL)
    {
        TCP_LOG("AI__TCP AI TCP POSTEvt ERR Malloc");
        return FALSE;
    }
    cur_data->data_p = p_data;
    cur_data->type = type;

    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(AI_TCP_DATA_SIG_T),SCI_IdentifyThread());
    psig->p_data = cur_data;
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,P_APP);
    return TRUE;
}

static BOOLEAN  AI_TCP_SendSigTo_Task(AI_TCP_INTERFACE_T *pMe,AI_TCP_TASK_SIG_E sig_id, AI_TCP_TASK_DATA_T * p_task_data)
{
    uint8 * pstr = PNULL;
    AI_TCP_TASK_SIG_T * psig = PNULL;
    
    if(pMe->m_tcp_task_id != SCI_INVALID_BLOCK_ID)
    {
        //send signal to AT to write uart
        SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(AI_TCP_TASK_SIG_T),SCI_IdentifyThread());
        psig->p_task_data = p_task_data;
        
        SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,pMe->m_tcp_task_id);
        return TRUE;
    }
    return FALSE;
}

static void AI_TCP_DelCurData(AI_TCP_INTERFACE_T *pMe)
{
    if(pMe->m_tcp_cur_data != NULL)
    {
        if(pMe->m_tcp_cur_data->str != NULL)
        {
            SCI_FREE(pMe->m_tcp_cur_data->str);
        }
        SCI_FREE(pMe->m_tcp_cur_data);
        pMe->m_tcp_cur_data = NULL;
    }
    return;
}

static BOOLEAN AI_TCP_Link_AddData(AI_TCP_INTERFACE_T *pMe,AI_TCP_LINK_DATA_T * pData)
{
    AI_TCP_LINK_NODE_T  * p1 = NULL;
    AI_TCP_LINK_NODE_T  * p2 = NULL;
    
    if(pData ==  NULL)
    {
        return FALSE;
    }
    
    p1=(AI_TCP_LINK_NODE_T *)SCI_ALLOC_APPZ(sizeof(AI_TCP_LINK_NODE_T));
    if(p1 == NULL)
    {
        return FALSE;
    }
    SCI_MEMSET(p1,0,sizeof(AI_TCP_LINK_NODE_T));

    p1->data = pData;
    p1->next_ptr = NULL;
    
    if(pMe->m_tcp_link_head == NULL)
    {
        pMe->m_tcp_link_head = p1;
    }
    else
    {
        p2 = pMe->m_tcp_link_head;
        while(p2->next_ptr != NULL)
        {
            p2 = p2->next_ptr;
        }
        p2->next_ptr = p1;
    }
    
    return TRUE;
}

static BOOLEAN  AI_TCP_Link_GetByPri(AI_TCP_INTERFACE_T *pMe,AI_TCP_LINK_DATA_T ** pPostData,uint8 priority)
{
    AI_TCP_LINK_NODE_T  * p1 = NULL;
    AI_TCP_LINK_NODE_T  * p2 = NULL;
    BOOLEAN res = FALSE;
    
    p2 = p1 = pMe->m_tcp_link_head;
    
    while(p1 != NULL)
    {
        if(p1->data != NULL && p1->data->priority == priority)
        {
            *pPostData = p1->data;
            
            if(p1 == pMe->m_tcp_link_head)
            {
                pMe->m_tcp_link_head = p1->next_ptr;
            }
            else
            {
                p2->next_ptr = p1->next_ptr;
            }
            SCI_FREE(p1);
            res = TRUE;
            break;
        }
        p2 = p1;
        p1 = p2->next_ptr;
    }
        
    return res;
}

static BOOLEAN  AI_TCP_Link_GetMaxPri(AI_TCP_INTERFACE_T *pMe,AI_TCP_LINK_DATA_T ** pPostData)
{
    AI_TCP_LINK_NODE_T  * p1 = NULL;
    AI_TCP_LINK_NODE_T  * p2 = NULL;
    uint8 max_pri = 0;
    BOOLEAN res = FALSE;
    
    p2 = p1 = pMe->m_tcp_link_head;
    
    while(p1 != NULL)
    {
        if(p1->data != NULL && p1->data->priority > max_pri)
        {
            max_pri = p1->data->priority;
        }
        p2 = p1;
        p1 = p2->next_ptr;
    }
    
    res = AI_TCP_Link_GetByPri(pMe,pPostData,max_pri);
    return res;
}

static BOOLEAN  AI_TCP_Link_DelAll(AI_TCP_INTERFACE_T *pMe)
{
    AI_TCP_LINK_NODE_T  * p1 = NULL;
    
    while(pMe->m_tcp_link_head != NULL)
    {
        p1 = pMe->m_tcp_link_head;
        pMe->m_tcp_link_head = p1->next_ptr;
        if(p1->data != NULL)
        {
            if(p1->data->str != NULL)
            {
                SCI_FREE(p1->data->str);
            }
            SCI_FREE(p1->data);
        }
        SCI_FREE(p1);
    }
    return TRUE;
}

static uint16  AI_TCP_Link_Count(AI_TCP_INTERFACE_T *pMe)
{
    AI_TCP_LINK_NODE_T  * p1 = NULL;
    uint16 num = 0;
    
    p1 = pMe->m_tcp_link_head;
    
    while(p1 != NULL)
    {
        num++;
        p1 = p1->next_ptr;
    }
    TCP_LOG("AI__TCP AI_TCP_Link_Count = %d", num);
    return num;
}

static BOOLEAN AI_TCP_RCV_Add(AI_TCP_INTERFACE_T *pMe, uint8 * pRcv, uint32 rcv_len)
{
    AI_TCP_RCV_NODE_T  * p1 = NULL;
    AI_TCP_RCV_NODE_T  * p2 = NULL;
    uint32 len = rcv_len;
    TCP_LOG("AI__TCP AI_TCP_RCV_Add = %d", rcv_len);

    if(len == 0)
    {
        return FALSE;
    }
    
    p1=(AI_TCP_RCV_NODE_T *)SCI_ALLOC_APPZ(sizeof(AI_TCP_RCV_NODE_T));
    if(p1 == NULL)
    {
        return FALSE;
    }
    
    p1->data.pRcv = (uint8 *)SCI_ALLOC_APPZ(len);
    if(p1->data.pRcv == NULL)
    {
        SCI_FREE(p1);
        return FALSE;
    }
    
    SCI_MEMCPY(p1->data.pRcv,pRcv,rcv_len);

    p1->data.len = len;
    
    p1->next_ptr = NULL;

    if(pMe->m_tcp_rcv_head == NULL)
    {
        pMe->m_tcp_rcv_head = p1;
    }
    else
    {
        p2 = pMe->m_tcp_rcv_head;
        while(p2->next_ptr != NULL)
        {
            p2 = p2->next_ptr;
        }
        p2->next_ptr = p1;
    }
    
    return TRUE;
}

static BOOLEAN AI_TCP_RCV_AddExt(AI_TCP_INTERFACE_T *pMe, uint8 * pRcv, uint32 rcv_len)
{
    AI_TCP_RCV_NODE_T  * p1 = NULL;
    AI_TCP_RCV_NODE_T  * p2 = NULL;
    uint32 len = rcv_len;
    TCP_LOG("AI__TCP AI_TCP_RCV_AddExt = %d", rcv_len);

    if(len == 0)
    {
        return FALSE;
    }
    
    p1=(AI_TCP_RCV_NODE_T *)SCI_ALLOC_APPZ(sizeof(AI_TCP_RCV_NODE_T));
    if(p1 == NULL)
    {
        return FALSE;
    }
    p1->data.pRcv = pRcv;
    p1->data.len = len;
    
    p1->next_ptr = NULL;

    if(pMe->m_tcp_rcv_head == NULL)
    {
        pMe->m_tcp_rcv_head = p1;
    }
    else
    {
        p2 = pMe->m_tcp_rcv_head;
        while(p2->next_ptr != NULL)
        {
            p2 = p2->next_ptr;
        }
        p2->next_ptr = p1;
    }
    
    return TRUE;
}

static BOOLEAN  AI_TCP_RCV_Get(AI_TCP_INTERFACE_T *pMe,AI_TCP_RCV_DATA_T * pUDPData)
{
    AI_TCP_RCV_NODE_T  * p1 = NULL;
    
    p1 = pMe->m_tcp_rcv_head;
    TCP_LOG("AI__TCP AI_TCP_RCV_Get ");
    if(p1 != NULL)
    {
      *pUDPData = p1->data;
        pMe->m_tcp_rcv_head = p1->next_ptr;
        SCI_FREE(p1);
        return TRUE;
    }
        
    return FALSE;
}

static BOOLEAN  AI_TCP_RCV_DelAll(AI_TCP_INTERFACE_T *pMe)
{
    AI_TCP_RCV_NODE_T  * p1 = NULL;
    TCP_LOG("AI__TCP AI_TCP_RCV_DelAll ");
    while(pMe->m_tcp_rcv_head != NULL)
    {
        p1 = pMe->m_tcp_rcv_head;
        pMe->m_tcp_rcv_head = p1->next_ptr;
        if(p1->data.pRcv != 0)
        {
            SCI_FREE(p1->data.pRcv);
        }
        SCI_FREE(p1);
    }
    return TRUE;
}
//链表操作结束

static void AITcp_Send_Write_Resp(AI_TCP_INTERFACE_T *pMe,uint8 res,uint32 Errid)
{
    TCP_LOG("AI__TCP AITcp_Send_Write_Resp res =%d , Errid=%d", res, Errid);
    if(res == TCP_SUCCESS)
    {
        AI_TCP_PostEvtTo_APP(AI_APP_SIGNAL_TCP_WRITE_SUCCESS,(void *)pMe,Errid);
    }
    else
    {
        AI_TCP_PostEvtTo_APP(AI_APP_SIGNAL_TCP_WRITE_FAIL,(void *)pMe,Errid);
    }
}

LOCAL void AI_TCP_HandlePdpCloseTimer(
                                uint8 timer_id,
                                uint32 param
                                )
{
    AI_TCP_INTERFACE_T *pMe = (AI_TCP_INTERFACE_T *)param;
    if(timer_id == pMe->m_tcp_pdp_timer_id && 0 != pMe->m_tcp_pdp_timer_id)
    {
        MMK_StopTimer(pMe->m_tcp_pdp_timer_id);
        pMe->m_tcp_pdp_timer_id = 0;
        #if 0
        if(AI_API_TCP_AllowClose(pMe) && MMIAI_UDP_AllowClose())
        {
            AI_UDP_StopPdpCloseTimer();
            MMIAI_Net_Close(pMe);
        }
        #endif
    }
}

LOCAL void AI_TCP_StartPdpCloseTimer(AI_TCP_INTERFACE_T *pMe)
{
    if(0 != pMe->m_tcp_pdp_timer_id)
    {
        MMK_StopTimer(pMe->m_tcp_pdp_timer_id);
        pMe->m_tcp_pdp_timer_id = 0;
    }
    
    pMe->m_tcp_pdp_timer_id = MMK_CreateTimerCallback(TCP_PDP_CLOSE_TIMEOUT, 
                                                                        AI_TCP_HandlePdpCloseTimer, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
}

PUBLIC void AI_TCP_StopPdpCloseTimer(AI_TCP_INTERFACE_T *pMe)
{
    if(0 != pMe->m_tcp_pdp_timer_id)
    {
        MMK_StopTimer(pMe->m_tcp_pdp_timer_id);
        pMe->m_tcp_pdp_timer_id = 0;
    }
}

static void AI_TCP_HandleConnectTimer(
                                uint8 timer_id,
                                uint32 param
                                )
{
    AI_TCP_INTERFACE_T *pMe = (AI_TCP_INTERFACE_T *)param;
    if(timer_id == pMe->m_tcp_connect_timer_id && 0 != pMe->m_tcp_connect_timer_id)
    {
        AITcp_Send_Write_Resp(pMe,TCP_ERROR,TCP_ERR_DISCONNECT);
        MMK_StopTimer(pMe->m_tcp_connect_timer_id);
        pMe->m_tcp_connect_timer_id = 0;
    }
}

static void AI_TCP_StartConnectTimer(AI_TCP_INTERFACE_T *pMe)
{
    if(0 != pMe->m_tcp_connect_timer_id)
    {
        MMK_StopTimer(pMe->m_tcp_connect_timer_id);
        pMe->m_tcp_connect_timer_id = 0;
    }
    
    pMe->m_tcp_connect_timer_id = MMK_CreateTimerCallback(50000, 
                                                                        AI_TCP_HandleConnectTimer, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
}

static void AI_TCP_StopConnectTimer(AI_TCP_INTERFACE_T *pMe)
{
    if(0 != pMe->m_tcp_connect_timer_id)
    {
        MMK_StopTimer(pMe->m_tcp_connect_timer_id);
        pMe->m_tcp_connect_timer_id = 0;
    }
}

static void AI_TCP_HandleReWriteTimer(
                                uint8 timer_id,
                                uint32 param
                                )
{
    AI_TCP_INTERFACE_T *pMe = (AI_TCP_INTERFACE_T *)param;
    if(timer_id == pMe->m_tcp_rewrite_timer_id && 0 != pMe->m_tcp_rewrite_timer_id)
    {
        AI_TCP_PostEvtTo_APP(AI_APP_SIGNAL_TCP_WRITE_RESTART,(void *)pMe,0);
        MMK_StopTimer(pMe->m_tcp_rewrite_timer_id);
        pMe->m_tcp_rewrite_timer_id = 0;
    }
}

static void AI_TCP_StartReWriteTimer(AI_TCP_INTERFACE_T *pMe,uint32 time_ms)
{
    if(0 != pMe->m_tcp_rewrite_timer_id)
    {
        MMK_StopTimer(pMe->m_tcp_rewrite_timer_id);
        pMe->m_tcp_rewrite_timer_id = 0;
    }
    
    pMe->m_tcp_rewrite_timer_id = MMK_CreateTimerCallback(time_ms, 
                                                                        AI_TCP_HandleReWriteTimer, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
}

static void AI_TCP_StopReWriteTimer(AI_TCP_INTERFACE_T *pMe)
{
    if(0 != pMe->m_tcp_rewrite_timer_id)
    {
        MMK_StopTimer(pMe->m_tcp_rewrite_timer_id);
        pMe->m_tcp_rewrite_timer_id = 0;
    }
}

static void AI_TCP_HandleWriteTimer(
                                uint8 timer_id,
                                uint32 param
                                )
{
    AI_TCP_INTERFACE_T *pMe = (AI_TCP_INTERFACE_T *)param;
    if(timer_id == pMe->m_tcp_write_timer_id && 0 != pMe->m_tcp_write_timer_id)
    {
        AITcp_Send_Write_Resp(pMe,TCP_ERROR,TCP_ERR_TIMEOUT);
        MMK_StopTimer(pMe->m_tcp_write_timer_id);
        pMe->m_tcp_write_timer_id = 0;
    }
}

static void AI_TCP_StartWriteTimer(AI_TCP_INTERFACE_T *pMe,uint32 time_ms)
{
    if(0 != pMe->m_tcp_write_timer_id)
    {
        MMK_StopTimer(pMe->m_tcp_write_timer_id);
        pMe->m_tcp_write_timer_id = 0;
    }
    
    pMe->m_tcp_write_timer_id = MMK_CreateTimerCallback(time_ms, 
                                                                        AI_TCP_HandleWriteTimer, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
}

static void AI_TCP_StopWriteTimer(AI_TCP_INTERFACE_T *pMe)
{
    if(0 != pMe->m_tcp_write_timer_id)
    {
        MMK_StopTimer(pMe->m_tcp_write_timer_id);
        pMe->m_tcp_write_timer_id = 0;
    }
}

static void AI_TCP_HandleReadTimer(
                                uint8 timer_id,
                                uint32 param
                                )
{
    AI_TCP_INTERFACE_T *pMe = (AI_TCP_INTERFACE_T *)param;
    if(timer_id == pMe->m_tcp_read_timer_id && 0 != pMe->m_tcp_read_timer_id)
    {
        AITcp_Send_Write_Resp(pMe,TCP_ERROR,TCP_ERR_READ);
        MMK_StopTimer(pMe->m_tcp_read_timer_id);
        pMe->m_tcp_read_timer_id = 0;
    }
}

static void AI_TCP_StartReadTimer(AI_TCP_INTERFACE_T *pMe,uint32 time_ms)
{
    if(0 != pMe->m_tcp_read_timer_id)
    {
        MMK_StopTimer(pMe->m_tcp_read_timer_id);
        pMe->m_tcp_read_timer_id = 0;
    }
    
    pMe->m_tcp_read_timer_id = MMK_CreateTimerCallback(time_ms, 
                                                                        AI_TCP_HandleReadTimer, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
}

static void AI_TCP_StopReadTimer(AI_TCP_INTERFACE_T *pMe)
{
    if(0 != pMe->m_tcp_read_timer_id)
    {
        MMK_StopTimer(pMe->m_tcp_read_timer_id);
        pMe->m_tcp_read_timer_id = 0;
    }
}

static TCPIP_SOCKET_T AI_TCP_SocCreate(AI_TCP_INTERFACE_T *pMe,uint16 bind_port, TCPIP_NETID_T net_id)
{
    TCPIP_SOCKET_T sockfd = TCPIP_SOCKET_INVALID;
    struct sci_sockaddr addr;
    char * ip_str = PNULL;
    TCP_LOG("AI__TCP AI_TCP_SocCreate bind_port=%d,net_id=0x%x", bind_port,net_id);

    sockfd = sci_sock_socket(AF_INET,SOCK_STREAM,0, net_id);
    TCP_LOG("AI__TCP AI TCP sci_sock_socket sockfd =%d", sockfd);
    if(sockfd == TCPIP_SOCKET_INVALID)
    {
        if(pMe->m_tcp_pISocket != TCPIP_SOCKET_INVALID)
        {
            TCP_LOG("AI__TCP AI AI_TCP_SocCreate sci_sock_socketclose");
            sci_sock_socketclose(pMe->m_tcp_pISocket);
            pMe->m_tcp_pISocket = TCPIP_SOCKET_INVALID;
            sockfd = sci_sock_socket(AF_INET,SOCK_STREAM,0, net_id);
            if(sockfd == TCPIP_SOCKET_INVALID)
            {
                TCP_LOG("AI__TCP AI TCP SocketCreate ERR 1");
                return TCPIP_SOCKET_INVALID;
            }
        }
        else
        {
            TCP_LOG("AI__TCP AI TCP SocketCreate ERR 2");
            return TCPIP_SOCKET_INVALID;
        }
    }

    sci_sock_setsockopt(sockfd, SO_NBIO, NULL);
    TCP_LOG("AI__TCP AI TCP SocketCreate OK sockfd= %ld",sockfd);
    return sockfd;
}

static BOOLEAN  AI_TCP_SocWriteEx(AI_TCP_INTERFACE_T *pMe)
{
    BOOLEAN res = FALSE;
    uint32 len =  pMe->m_tcp_get_len;
    int send_len = pMe->m_tcp_get_len;
    int sent_len = 0, total_sent = 0;
    TCP_LOG("AI__TCP AI_TCP_SocWriteEx len=%d", len);
    if(pMe->m_tcp_pISocket == TCPIP_SOCKET_INVALID)
    {
        TCP_LOG("AI__TCP AI_TCP_Send ERR No Socket");
        return FALSE;
    }
    
    while( total_sent < len )
    {
        
        sent_len = sci_sock_send(pMe->m_tcp_pISocket, (pMe->m_tcp_buf_get + total_sent), send_len, 0 );
        TCP_LOG("AI__TCP AI_TCP_SocWriteEx sci_sock_send send_len=%d ,sent_len=%d ", send_len, sent_len );
        if( TCPIP_SOCKET_ERROR == sent_len )
        {  // 检查 socket 错误类型
            int error = sci_sock_errno(pMe->m_tcp_pISocket);
            TCP_LOG("AI__TCP AI_TCP_SocWriteEx sci_sock_errno=%d", error);
            if( EWOULDBLOCK == error || ENOBUFS == error )
            { 
                // 休眠一段时间，再重新尝试发送
                SCI_Sleep(200);
            }
            else
            {  // 其它错误都视为严重错误，终止发送
                TCP_LOG("AI__TCP AI_TCP_SocWriteEx send big error ,stop ");
                break;
            }
        }
        else if( sent_len > 0 )
        {  // 发送成功，累计发送长度
            total_sent += sent_len;
            if(total_sent >= len)
            {
                //发送完成
                res = TRUE;
                
                break;
            }
            send_len -= sent_len;
        }
        else
        {  // 返回值为 0，表示连接已关闭，属于异常终止
            TCP_LOG("AI__TCP AI_TCP_SocWriteEx connect closed exception ");
            break;
        }
    }
    TCP_LOG("AI__TCP AI_TCP_Send Result=%d, sockfd=%ld,Len=%d/%d",res,pMe->m_tcp_pISocket,total_sent,len);
    return res;
}

static void AI_TCP_SocWrite(AI_TCP_INTERFACE_T *pMe)
{
    if (TRUE == AI_TCP_SocWriteEx(pMe))
    {
        TCP_LOG("AI__TCP AI TCP Write finished");
        if(pMe->m_tcp_cur_data != NULL && pMe->m_tcp_cur_data->rcv_handle != NULL)
        {
            pMe->m_tcp_need_rcv = TRUE;
            AI_TCP_StartReadTimer(pMe,10000);
        }
        else
        {
            AITcp_Send_Write_Resp(pMe,TCP_SUCCESS,TCP_SUCCESS);
        }
    }
    else
    {
        AITcp_Send_Write_Resp(pMe,TCP_ERROR,TCP_ERR_NET_WRITE);
    }
}
static void AI_TCP_SocConnected(AI_TCP_INTERFACE_T *pMe,uint32 conn_err)
{    
    BOOLEAN conn_ok = FALSE;
    if(conn_err == 0)
    {
        conn_ok = TRUE;
    }
    
    TCP_LOG("AI__TCP AI TCP Connected(%d) Url=%s,Port=%d,IP=0x%x",conn_ok,pMe->m_tcp_ip_url,pMe->m_tcp_conn_port,pMe->m_tcp_conn_ip);
    
    if (conn_ok)
    {
        AI_TCP_PostEvtTo_APP(AI_APP_SIGNAL_TCP_CONNECT_SUCCESS,(void *)pMe,0);
        AI_TCP_SocWrite(pMe);
    } 
    else
    {
        TCP_LOG("AI__TCP AI TCP Connected Err");
        AITcp_Send_Write_Resp(pMe,TCP_ERROR,TCP_ERR_DISCONNECT);
    }
}

static BOOLEAN  AI_TCP_SocConnect(AI_TCP_INTERFACE_T *pMe,uint32 ip_addr,uint16 port)
{
    int send_res = 0;
    struct sci_sockaddr dest_addr;

    if(pMe->m_tcp_pISocket == TCPIP_SOCKET_INVALID)
    {
        TCP_LOG("AI__TCP AI_TCP_SocConnect ERR No Socket");
        return FALSE;
    }
    TCP_LOG("AI__TCP AI_TCP_SocConnect %s:%d,%d,0x%x",pMe->m_tcp_ip_url,pMe->m_tcp_conn_port,port,ip_addr);
    SCI_MEMSET(&dest_addr,0,sizeof(dest_addr));
    dest_addr.family = AF_INET;
    dest_addr.ip_addr = ip_addr;
    dest_addr.port = htons(port);
    
    send_res = sci_sock_connect(pMe->m_tcp_pISocket, &dest_addr, sizeof(dest_addr));
    if(send_res == TCPIP_SOCKET_ERROR)
    {
        if(EINPROGRESS != sci_sock_errno(pMe->m_tcp_pISocket) )
        {
            TCP_LOG("AI__TCP AI_TCP_Connect ERR sockfd=%ld, errno=%d",pMe->m_tcp_pISocket,sci_sock_errno(pMe->m_tcp_pISocket));
            return FALSE;
        }
    }
    if(pMe->m_tcp_socket_is_connected == FALSE)
    {
        AI_TCP_StartConnectTimer(pMe);
    }
    TCP_LOG("AI__TCP AI_TCP_Connect OK sockfd=%ld,IP=0x%x,port=%d",pMe->m_tcp_pISocket,ip_addr,port);
    return TRUE;
}

static BOOLEAN  AI_TCP_SocConnectStart(AI_TCP_INTERFACE_T *pMe,AI_TCP_TASK_DATA_T * p_tcp_data)
{
    BOOLEAN res = FALSE;
        
    if(p_tcp_data == PNULL || p_tcp_data->str == PNULL || p_tcp_data->str_len == 0)
    {
        TCP_LOG("AI__TCP AI_TCP_Connect_Start ERR No Data");
        return FALSE;
    }
    
    if(p_tcp_data->ip_addr == 0)
    {
        if(p_tcp_data->ip_str == PNULL)
        {
            TCP_LOG("AI__TCP AI_TCP_Connect_Start ERR No IP");
            return FALSE;
        }
        else
        {
            int rv = -1;
            TCPIP_IPADDR_T send_addr = 0;
            struct sci_hostent * host = sci_gethostbyname(p_tcp_data->ip_str);
            //rv = sci_parse_host_ext((char*)p_tcp_data->ip_str,&send_addr,1,MMIAI_Net_GetNetID());
            //rv = sci_parse_host((char*)p_tcp_data->ip_str,&send_addr,1);
            TCP_LOG("AI__TCP AI_TCP_DNS %s",p_tcp_data->ip_str);
            if(host != NULL)
            {
                char * ip_str = NULL;
                if( sizeof(TCPIP_IPADDR_T) == host->h_length )
                {
                    SCI_MEMCPY( &send_addr, host->h_addr_list[0], host->h_length );
                }
                ip_str = inet_ntoa(send_addr);
                p_tcp_data->ip_addr = (uint32)send_addr;
                TCP_LOG("AI__TCP AI_TCP_Connect_Start DNS %s,%s,0x%x,%d",p_tcp_data->ip_str,ip_str,send_addr,p_tcp_data->port);
                res = AI_TCP_SocConnect(pMe,send_addr,p_tcp_data->port);
            }
            else
            {
                TCP_LOG("AI__TCP AI_TCP_Connect_Start DNS ERR");
                return FALSE;
            }
        }
    }    
    else
    {
        TCP_LOG("AI__TCP AI_TCP_Connect_Start ip_addr=0x%x,%s,%d",p_tcp_data->ip_addr,p_tcp_data->ip_str,p_tcp_data->port);
        res = AI_TCP_SocConnect(pMe,p_tcp_data->ip_addr,p_tcp_data->port);
    }
    return res;
}

static BOOLEAN  TCP_Task_HandleSig_Conncet(AI_TCP_INTERFACE_T *pMe,AI_TCP_TASK_DATA_T * p_task_data)
{
    TCP_LOG("AI__TCP AI TCP TASK Connect Url:%s, Port=%d,IP=0x%x",p_task_data->ip_str,p_task_data->port,p_task_data->ip_addr);
    if(FALSE == AI_TCP_SocConnectStart(pMe,p_task_data))
    {
        AITcp_Send_Write_Resp(pMe,TCP_ERROR,TCP_ERR_DISCONNECT);
    }
    else
    {
        if(pMe->m_tcp_socket_is_connected)
        {
            AI_TCP_SocConnected(pMe,0);
        }
    }
    return TRUE;
}

static BOOLEAN  TCP_Task_HandleSig_Rcv(AI_TCP_INTERFACE_T *pMe,AI_TCP_TASK_DATA_T * p_task_data)
{
    TCP_LOG("AI__TCP AI TCP TASK Rcv Len=%d",p_task_data->str_len);
    return TRUE;
}

static BOOLEAN  TCP_Task_HandleSig_Write(AI_TCP_INTERFACE_T *pMe,AI_TCP_TASK_DATA_T * p_task_data)
{
    TCP_LOG("AI__TCP AI TCP TASK Write Len=%d,is_connected=%d",p_task_data->str_len,pMe->m_tcp_socket_is_connected);
    if(pMe->m_tcp_socket_is_connected)
    {
        AI_TCP_SocWrite(pMe);
    }
    else
    {
        AITcp_Send_Write_Resp(pMe,TCP_ERROR,TCP_ERR_NET_WRITE);
    }
    return TRUE;
}

void AI_Tcp_ForceCloseAll()
{
    AI_TCP_INTERFACE_T * pMe = g_ai_app.m_ai_tcp_reg_interface;
    TCP_LOG("AI__TCP AI_Tcp_ForceClose");     
    if(pMe)
    {
        AI_TCP_PostEvtTo_APP(AI_APP_SIGNAL_TCP_DISCONNECTED,(void *)pMe,0);
    }

}
    


void AI_TCP_TaskEntry(uint32 argc, void *argv)
{
    xSignalHeaderRec*   sig_ptr = 0; 
    int ret = 0;
    int left_len=0;
    int recv_len=0;
    uint16 ip_port = 0;
    uint32 rcv_ip_addr = 0;
    char* ip_str=NULL;
    TCPIP_SOCKET_T  so = 0;
    uint32 err = 0;
    uint32 cur_net_id = MMIAI_Net_GetNetID();
    AI_TCP_INTERFACE_T *pMe = (AI_TCP_INTERFACE_T *)argv;
    
    pMe->m_tcp_task_id = SCI_IdentifyThread();
    pMe->m_tcp_pISocket = AI_TCP_SocCreate(pMe, 0, cur_net_id);
    if(pMe->m_tcp_pISocket != TCPIP_SOCKET_INVALID)
    {
        ret = sci_sock_asyncselect(pMe->m_tcp_pISocket, pMe->m_tcp_task_id, AS_CONNECT|AS_READ|AS_WRITE|AS_CLOSE|AS_FULLCLOSED);
    }
    
    pMe->m_tcp_socket_is_connected = FALSE;
    
    TCP_LOG("AI__TCP AI TCP Task Entry taskid=0x%x, socket_id=%d, ret=%d interface=%d", pMe->m_tcp_task_id, pMe->m_tcp_pISocket, ret,pMe->m_tcp_interface_idx);
        
    while(1)
    {
        sig_ptr = SCI_GetSignal(pMe->m_tcp_task_id); 
        switch(sig_ptr->SignalCode)
        {            
            case SOCKET_FULL_CLOSED_EVENT_IND:
                {
                    so = ((SOCKET_FULL_CLOSED_EVENT_IND_SIG_T*)sig_ptr)->socket_id;
                    TCP_LOG("AI__TCP AI SOCKET_FULL_CLOSED_EVENT_IND so=%ld,sockid=%ld",so,pMe->m_tcp_pISocket);
                    if( pMe->m_tcp_pISocket != TCPIP_SOCKET_INVALID && so == pMe->m_tcp_pISocket)
                    {
                        pMe->m_tcp_socket_is_connected = FALSE;
                        AI_TCP_PostEvtTo_APP(AI_APP_SIGNAL_TCP_DISCONNECTED,(void *)pMe,0);
                    }
                }
                break;
                
            case SOCKET_CONNECTION_CLOSE_EVENT_IND:
                {
                    so = ((SOCKET_CONNECTION_CLOSE_EVENT_IND_SIG_T*)sig_ptr)->socket_id;
                    TCP_LOG("AI__TCP AI SOCKET_CONNECTION_CLOSE_EVENT_IND so=%ld,sockid=%ld",so,pMe->m_tcp_pISocket);
                    if( pMe->m_tcp_pISocket != TCPIP_SOCKET_INVALID && so == pMe->m_tcp_pISocket)
                    {
                        pMe->m_tcp_socket_is_connected = FALSE;
                        AI_TCP_PostEvtTo_APP(AI_APP_SIGNAL_TCP_DISCONNECTED,(void *)pMe,0);
                    }
                }
                break;
                
            case SOCKET_WRITE_EVENT_IND:
                {
                    so = ((SOCKET_WRITE_EVENT_IND_SIG_T*)sig_ptr)->socket_id;
                    TCP_LOG("AI__TCP AI SOCKET_WRITE_EVENT_IND so=%ld,sockid=%ld",so,pMe->m_tcp_pISocket);
                    if( pMe->m_tcp_pISocket != TCPIP_SOCKET_INVALID && so == pMe->m_tcp_pISocket)
                    {
                    }
                }
                break;
                
            case SOCKET_CONNECT_EVENT_IND:
                {
                    AI_TCP_StopConnectTimer(pMe);
                    so = ((SOCKET_CONNECT_EVENT_IND_SIG_T*)sig_ptr)->socket_id;
                    err = ((SOCKET_CONNECT_EVENT_IND_SIG_T*)sig_ptr)->error_code;
                    TCP_LOG("AI__TCP AI SOCKET_CONNECT_EVENT_IND so=%ld,sockid=%ld,err=%ld", so, pMe->m_tcp_pISocket, err);
                    if( pMe->m_tcp_pISocket != TCPIP_SOCKET_INVALID && so == pMe->m_tcp_pISocket)
                    {
                        if(0 == err)
                        {
                            pMe->m_tcp_socket_is_connected = TRUE;
                        }
                        AI_TCP_SocConnected(pMe,err);
                    }
                }
                break;
                
            case SOCKET_READ_EVENT_IND:
            case SOCKET_READ_BUFFER_STATUS_EVENT_IND:
                TCP_LOG("AI__TCP AI SOCKET_READ_EVENT_IND ");
                if(sig_ptr->SignalCode == SOCKET_READ_BUFFER_STATUS_EVENT_IND)
                {
                    so = ((SOCKET_READ_BUFFER_STATUS_EVENT_IND_SIG_T*)sig_ptr)->socket_id;
                }
                else
                {
                    so = ((SOCKET_READ_EVENT_IND_SIG_T*)sig_ptr)->socket_id;
                }
                if( pMe->m_tcp_pISocket != TCPIP_SOCKET_INVALID && so == pMe->m_tcp_pISocket)
                {
                    while(1)
                    {
                        TCP_LOG("AI__TCP AI SOCKET_READ_EVENT_IND sci_sock_asyncrecv");
                        recv_len = sci_sock_asyncrecv(pMe->m_tcp_pISocket,  pMe->m_tcp_buf_rcv, pMe->m_tcp_rcv_len, 0, &left_len);
                        if (recv_len > 0)
                        {
                            TCP_LOG("AI__TCP AI TCP RCV: recv_len=%d, left_len=%d",recv_len,left_len);
                            if(pMe->m_tcp_cmd_check_handle != PNULL)
                            {
                                uint32 cur_len = 0;
                                uint32 end_len = 0;
                                uint32 cmd_len = 0;
                                uint8 * pCmd = PNULL;
                                do
                                {
                                    cur_len = pMe->m_tcp_cmd_check_handle(pMe->m_tcp_buf_rcv+end_len,recv_len-end_len,&pCmd,&cmd_len);
                                    if(cur_len > 0)
                                    {
                                        AI_TCP_RCV_AddExt(pMe,pCmd,(uint32)cmd_len);
                                        end_len += cur_len;
                                        if(pMe->m_tcp_need_rcv)
                                        {
                                            if(pMe->m_tcp_cur_data != NULL && pMe->m_tcp_cur_data->rcv_handle != NULL)
                                            {
                                                TCP_LOG("AI__TCP AI SOCKET_READ_EVENT_IND rcv_handle");
                                                if(AI_TCP_RET_OK == pMe->m_tcp_cur_data->rcv_handle((void *)pMe,pCmd,(uint32)cmd_len))
                                                {
                                                    AI_TCP_StopReadTimer(pMe);
                                                    AITcp_Send_Write_Resp(pMe,TCP_SUCCESS,TCP_SUCCESS);
                                                    pMe->m_tcp_need_rcv = FALSE;
                                                }
                                            }
                                        }
                                    }
                                }
                                while(cur_len > 0);
                                    
                                if( 0 == left_len )
                                {
                                    AI_TCP_PostEvtTo_APP(AI_APP_SIGNAL_TCP_RCV_DATA,(void *)pMe,0);
                                    break;
                                }

                            }
                            else
                            {
                                AI_TCP_RCV_Add(pMe,pMe->m_tcp_buf_rcv,(uint32)recv_len);
                                if(pMe->m_tcp_need_rcv)
                                {
                                    if(pMe->m_tcp_cur_data != NULL && pMe->m_tcp_cur_data->rcv_handle != NULL)
                                    {
                                        TCP_LOG("AI__TCP AI SOCKET_READ_EVENT_IND rcv_handle 2");
                                        if(AI_TCP_RET_OK == pMe->m_tcp_cur_data->rcv_handle(pMe,pMe->m_tcp_buf_rcv,(uint32)recv_len))
                                        {
                                            AI_TCP_StopReadTimer(pMe);
                                            AITcp_Send_Write_Resp(pMe,TCP_SUCCESS,TCP_SUCCESS);
                                            pMe->m_tcp_need_rcv = FALSE;
                                        }
                                    }
                                }
                                if( 0 == left_len )
                                {
                                    // socket 无剩余数据，结束接收循环
                                    AI_TCP_PostEvtTo_APP(AI_APP_SIGNAL_TCP_RCV_DATA,(void *)pMe,0);
                                    break;
                                }
                            }
                        }
                        else
                        {
                            if(pMe->m_tcp_need_rcv)
                            {
                                AI_TCP_StopReadTimer(pMe);
                                AITcp_Send_Write_Resp(pMe,TCP_ERROR,TCP_ERR_READ);
                                pMe->m_tcp_need_rcv = FALSE;
                            }
                            else
                            {
                                AI_TCP_PostEvtTo_APP(AI_APP_SIGNAL_TCP_DISCONNECTED,(void *)pMe,0);
                            }
                            break;
                        }
                    }
                }
                break;
                
            case SIG_AI_TCP_CONNET:
                {
                    AI_TCP_TASK_SIG_T* data_ptr=(AI_TCP_TASK_SIG_T*)sig_ptr;
                    TCP_LOG("AI__TCP SIG_AI_TCP_CONNET ");
                    if(data_ptr != PNULL && data_ptr->p_task_data != PNULL)
                    {
                        TCP_Task_HandleSig_Conncet(pMe,data_ptr->p_task_data);
                        if(data_ptr->p_task_data->ip_str != NULL)
                        {
                            SCI_FREE(data_ptr->p_task_data->ip_str);
                        }
                        #if 0
                        if(data_ptr->p_task_data->str != NULL)
                        {
                            SCI_FREE(data_ptr->p_task_data->str);
                        }
                        #endif
                        SCI_FREE(data_ptr->p_task_data);
                    }
                    else
                    {
                        AITcp_Send_Write_Resp(pMe,TCP_ERROR,TCP_ERR_DISCONNECT);
                    }
                    break;
                }
                            
            case SIG_AI_TCP_RCV:
                {
                    AI_TCP_TASK_SIG_T* data_ptr=(AI_TCP_TASK_SIG_T*)sig_ptr;
                    TCP_LOG("AI__TCP SIG_AI_TCP_RCV ");
                    if(data_ptr != PNULL && data_ptr->p_task_data != PNULL)
                    {
                        TCP_Task_HandleSig_Rcv(pMe,data_ptr->p_task_data);
                        if(data_ptr->p_task_data->ip_str != NULL)
                        {
                            SCI_FREE(data_ptr->p_task_data->ip_str);
                        }
                        if(data_ptr->p_task_data->str != NULL)
                        {
                            SCI_FREE(data_ptr->p_task_data->str);
                        }
                        SCI_FREE(data_ptr->p_task_data);
                    }
                    break;
                }
            case SIG_AI_TCP_WRITE:
                {
                    AI_TCP_TASK_SIG_T* data_ptr=(AI_TCP_TASK_SIG_T*)sig_ptr;
                    TCP_LOG("AI__TCP SIG_AI_TCP_WRITE ");
                    if(data_ptr != PNULL && data_ptr->p_task_data != PNULL)
                    {
                        TCP_Task_HandleSig_Write(pMe,data_ptr->p_task_data);
                        if(data_ptr->p_task_data->ip_str != NULL)
                        {
                            SCI_FREE(data_ptr->p_task_data->ip_str);
                        }
                        #if 0
                        if(data_ptr->p_task_data->str != NULL)
                        {
                            SCI_FREE(data_ptr->p_task_data->str);
                        }
                        #endif
                        SCI_FREE(data_ptr->p_task_data);
                    }
                    break;
                }

            }
            SCI_FREE(sig_ptr);
    }
}

static void AI_TCP_TaskClose(AI_TCP_INTERFACE_T *pMe)
{
	TCP_LOG("AI__TCP AI_TCP_TaskClose, taskid=0x%x", pMe->m_tcp_task_id);
	if(SCI_INVALID_BLOCK_ID != pMe->m_tcp_task_id)
	{   
             if(pMe->m_tcp_pISocket != TCPIP_SOCKET_INVALID)
             {
                TCP_LOG("AI__TCP AI AI_TCP_TaskClose sci_sock_socketclose");
        		sci_sock_socketclose(pMe->m_tcp_pISocket);
             }
		pMe->m_tcp_pISocket = TCPIP_SOCKET_INVALID;
		SCI_TerminateThread(pMe->m_tcp_task_id);
		SCI_DeleteThread(pMe->m_tcp_task_id);
		pMe->m_tcp_task_id = SCI_INVALID_BLOCK_ID;
            pMe->m_tcp_socket_is_connected = FALSE;
	}
}

static BLOCK_ID AI_TCP_TaskCreate(AI_TCP_INTERFACE_T *pMe)
{
    TCP_LOG("AI__TCP AI_TCP_TaskCreate, taskid=0x%x", pMe->m_tcp_task_id);
    if(pMe->m_tcp_task_id == SCI_INVALID_BLOCK_ID)
    {
        pMe->m_tcp_task_id = SCI_CreateThread("AI_TCP_THREAD_NAME",
                                                                "AI_TCP_QUEUE_NAME",
                                                                AI_TCP_TaskEntry,
                                                                NULL,
                                                                (void *)pMe,
                                                                1024*10,
                                                                50,
                                                                TX_SYSTEM_NORMALE,	// TX_SYSTEM_HIGH,
                                                                SCI_PREEMPT,
                                                                SCI_AUTO_START
                                                                );
    }
    return pMe->m_tcp_task_id;
}

static void AITcp_SetSocketState(AI_TCP_INTERFACE_T *pMe,AITCP_SOCKET_STATE_E status)
{
    switch(status)
    {
        case TCP_SOCKET_STATE_NULL:
                TCP_LOG("AI__TCP AI TCP STATUS: TCP_SOCKET_STATE_NULL");
            break;
        case TCP_SOCKET_STATE_OPEN:
                TCP_LOG("AI__TCP AI TCP STATUS: TCP_SOCKET_STATE_OPEN");
            break;
        case TCP_SOCKET_STATE_CONNECTING:
                TCP_LOG("AI__TCP AI TCP STATUS: TCP_SOCKET_STATE_CONNECTING");
            break;
        case TCP_SOCKET_STATE_CONNECTED:
                TCP_LOG("AI__TCP AI TCP STATUS: TCP_SOCKET_STATE_CONNECTED");
            break;
        case TCP_SOCKET_STATE_WRITING:
                TCP_LOG("AI__TCP AI TCP STATUS: TCP_SOCKET_STATE_WRITING");
            break;
    }
    pMe->m_tcp_socket_status = status;
}

static AITCP_SOCKET_STATE_E AITcp_GetState(AI_TCP_INTERFACE_T * pMe)
{
        return pMe->m_tcp_socket_status;
}

static BOOLEAN AITcp_IsConnected(AI_TCP_INTERFACE_T * pMe)
{
    AITCP_SOCKET_STATE_E status = pMe->m_tcp_socket_status;
    BOOLEAN res = FALSE;
    
    if(status == TCP_SOCKET_STATE_CONNECTED
        || status == TCP_SOCKET_STATE_WRITING
       )
    {
        res = TRUE;
    }
    return res;
}

static BOOLEAN AITcp_IsUsing(AI_TCP_INTERFACE_T * pMe)
{
    AITCP_SOCKET_STATE_E status = pMe->m_tcp_socket_status;
    BOOLEAN res = FALSE;
    
    if(status == TCP_SOCKET_STATE_CONNECTING
        || status == TCP_SOCKET_STATE_WRITING
        )
    {
        res = TRUE;
    }
    return res;
}

static void AITcp_TCPClose(AI_TCP_INTERFACE_T *pMe)
{
    TCP_LOG("AI__TCP AI TCP Close Socket = %d",pMe->m_tcp_pISocket);
    AI_TCP_TaskClose(pMe);
    return;
}

static uint8 AITcp_TCP_Cancel(AI_TCP_INTERFACE_T *pMe)
{
    int8 cur_socket = pMe->m_tcp_pISocket;
    TCP_LOG("AI__TCP AI TCP Cancel Socket = %d",pMe->m_tcp_pISocket);
    pMe->m_tcp_need_connect = TRUE;
    return 0;
}

static uint8 AITcp_Open(AI_TCP_INTERFACE_T *pMe)
{
    
    if(pMe->m_tcp_task_id == SCI_INVALID_BLOCK_ID)
    {
        if(SCI_INVALID_BLOCK_ID == AI_TCP_TaskCreate(pMe))
        {
            TCP_LOG("AI__TCP AITcp_Open  AI_TCP_TaskCreate fail");
            return 0;
        }
        AITcp_SetSocketState(pMe,TCP_SOCKET_STATE_OPEN);
    }
    TCP_LOG("AI__TCP AITcp_Open  AI_TCP_TaskCreate success");
    return 1;
}

static uint8 AITcp_Close(AI_TCP_INTERFACE_T *pMe)
{
    AITcp_TCP_Cancel(pMe);
    AITcp_TCPClose(pMe);
    return 1;
}

static BOOLEAN AITcp_Connect(AI_TCP_INTERFACE_T *pMe)
{
    AI_TCP_TASK_DATA_T * p_task_data = PNULL;
    BOOLEAN res = FALSE;
    uint32 ip_addr = 0;
    int ip_err;
    uint16 ip_len = 0;
    
    if(pMe->m_tcp_task_id == SCI_INVALID_BLOCK_ID)
    {
        return FALSE;
    }
    
    if(pMe->m_tcp_cur_data == NULL)
    {
        return FALSE;
    }
    if(pMe->m_tcp_cur_data->str == NULL || pMe->m_tcp_cur_data->len == 0)
    {
        return FALSE;
    }
    p_task_data = SCI_ALLOC_APPZ(sizeof(AI_TCP_TASK_DATA_T));
    if (p_task_data == PNULL)
    {
        return FALSE;
    }
    #if 1
        p_task_data->str = pMe->m_tcp_cur_data->str;
        p_task_data->str_len = pMe->m_tcp_cur_data->len;
    #else
    p_task_data->str = SCI_ALLOC_APPZ(pMe->m_tcp_cur_data->len+1);
    if(p_task_data->str == PNULL)
    {
        SCI_FREE(p_task_data);
        return FALSE;
    }
    SCI_MEMCPY(p_task_data->str,pMe->m_tcp_cur_data->str,pMe->m_tcp_cur_data->len);
    p_task_data->str_len = pMe->m_tcp_cur_data->len;
    #endif
    p_task_data->port = pMe->m_tcp_conn_port;
    
    if(pMe->m_tcp_ip_url != PNULL)
    {
        ip_len = strlen(pMe->m_tcp_ip_url);
        p_task_data->ip_str = SCI_ALLOC_APPZ(ip_len+1);
        if (p_task_data->ip_str == PNULL)
        {
            SCI_FREE(p_task_data);
            return FALSE;
        }
        SCI_MEMCPY(p_task_data->ip_str,pMe->m_tcp_ip_url,ip_len);
    }
    p_task_data->ip_addr = pMe->m_tcp_conn_ip;
    p_task_data->pMe = (void * )pMe;
    TCP_LOG("AI__TCP AITcp_Connect ip=%s:%d,ip_addr=0x%x",p_task_data->ip_str,p_task_data->port,p_task_data->ip_addr);
   
    if(FALSE == AI_TCP_SendSigTo_Task(pMe,SIG_AI_TCP_CONNET,p_task_data))
    {
        if (p_task_data->ip_str != PNULL)
        {
            SCI_FREE(p_task_data->ip_str);
        }
        SCI_FREE(p_task_data);
        return FALSE;
    }
    return TRUE;
}

extern BOOLEAN AITcp_Write(AI_TCP_INTERFACE_T *pMe,uint8 *str,uint16 str_len)
{
    AI_TCP_TASK_SIG_T * psig = PNULL;
    AI_TCP_TASK_DATA_T * p_task_data = PNULL;
    
    if(pMe->m_tcp_task_id == SCI_INVALID_BLOCK_ID)
    {
        return FALSE;
    }
    
    p_task_data = SCI_ALLOC_APPZ(sizeof(AI_TCP_TASK_DATA_T));
    if (p_task_data == PNULL)
    {
        return FALSE;
    }

#if 1
    p_task_data->str = str;
    p_task_data->str_len = str_len;
#else
    p_task_data->str = SCI_ALLOC_APPZ(str_len+1);
    if (p_task_data->str == PNULL)
    {
        SCI_FREE(p_task_data);
        return FALSE;
    }
    SCI_MEMCPY(p_task_data->str,str,str_len);
    p_task_data->str_len = str_len;
#endif
    if(FALSE == AI_TCP_SendSigTo_Task(pMe,SIG_AI_TCP_WRITE,p_task_data))
    {
        #if 0
        if (p_task_data->str != PNULL)
        {
            SCI_FREE(p_task_data->str);
        }
        #endif
        SCI_FREE(p_task_data);
        return FALSE;
    }

    return TRUE;
}

extern int AITCP_Close(AI_TCP_INTERFACE_T *pMe)
{       
    AITcp_Close(pMe);
    return 0;
}

static void AITCP_DNSConnect(AI_TCP_INTERFACE_T *pMe)
{
    TCP_LOG("AI__TCP AITCP_DNSConnect ");
    if(pMe->m_tcp_task_id == SCI_INVALID_BLOCK_ID)
    {
        if(0 == AITcp_Open(pMe))
        {
            AITcp_Send_Write_Resp(pMe,TCP_ERROR,TCP_ERR_NOOPEN);
            
            return;
        }
    }
        
    if(pMe->m_tcp_socket_status == TCP_SOCKET_STATE_OPEN)
    {
        AITcp_SetSocketState(pMe,TCP_SOCKET_STATE_DISCONNECTED);
    }
    
    if(TRUE == AITcp_Connect(pMe))
    {
        AITcp_SetSocketState(pMe,TCP_SOCKET_STATE_CONNECTING);
    }
    else
    {
        AITcp_Send_Write_Resp(pMe,TCP_ERROR,TCP_ERR_DISCONNECT);
    }
}

//
extern int AITCP_ConnWrite(AI_TCP_INTERFACE_T *pMe)
{
    int res = 0;
    AITCP_SOCKET_STATE_E status = pMe->m_tcp_socket_status;
    TCP_LOG("AI__TCP AITCP_ConnWrite pMe=0x%x,%s:%d,len=%d",pMe,pMe->m_tcp_ip_url,pMe->m_tcp_conn_port,pMe->m_tcp_get_len);

    if(status == TCP_SOCKET_STATE_CONNECTING)
    {
        TCP_LOG("AI__TCP AITCP ConnWrite Status CONNECTING");
        AITcp_TCP_Cancel(pMe);
    }
    else if(status == TCP_SOCKET_STATE_WRITING)
    {
        TCP_LOG("AI__TCP AITCP ConnWrite Status WRITING");
    }
    else if(status == TCP_SOCKET_STATE_CONNECTED)
    {
        TCP_LOG("AI__TCP AITCP ConnWrite Status CONNECTED");
        if(pMe->m_tcp_pISocket >= 0)
        {
            TCP_LOG("AI__TCP AITCP ConnWrite Status CONNECTED AITcp_Write");
            AITcp_Write(pMe,pMe->m_tcp_buf_get,pMe->m_tcp_get_len);
            AI_TCP_StartWriteTimer(pMe,40000);
            return 0;
        }
    }
    else if(status == TCP_SOCKET_STATE_OPEN)
    {
        TCP_LOG("AI__TCP AITCP ConnWrite Status OPEN");
    }
    else if(status == TCP_SOCKET_STATE_DISCONNECTED)
    {
        TCP_LOG("AI__TCP AITCP ConnWrite Status DISCONNECTED");
        AITcp_TCP_Cancel(pMe);
    }
   else
    {
        TCP_LOG("AI__TCP AITCP ConnWrite Status OTHER = %d",pMe->m_tcp_socket_status);
        AITcp_TCP_Cancel(pMe);
    }
    
    AITCP_DNSConnect(pMe);// will create tcp thread
    AI_TCP_StartWriteTimer(pMe,40000);
    return res;
}

extern void AITCP_CloseAllTimer(AI_TCP_INTERFACE_T *pMe)
{
    AI_TCP_StopReadTimer(pMe);
    AI_TCP_StopWriteTimer(pMe);
    AI_TCP_StopReWriteTimer(pMe);
    return;
}

extern int AITCP_Reset(AI_TCP_INTERFACE_T *pMe)
{
    TCP_LOG("AI__TCP AI TCP Reset pMe->m_tcp_socket_status=%d",pMe->m_tcp_socket_status);
    AITCP_CloseAllTimer(pMe);
    AITCP_Close(pMe);
    AI_TCP_DelCurData(pMe);
    AI_TCP_RCV_DelAll(pMe);
    AI_TCP_Link_DelAll(pMe);
    pMe->m_tcp_conn_ip = 0;
    pMe->m_tcp_conn_port = 0;
    pMe->m_tcp_is_sending = FALSE;
    pMe->m_tcp_need_rcv = FALSE;
    pMe->m_tcp_buf_get = NULL;

    if(pMe->m_tcp_ip_url != NULL)
    {
        SCI_FREE(pMe->m_tcp_ip_url);
        pMe->m_tcp_ip_url = NULL;
    }
    return 0;
}

static int AITCP_End(AI_TCP_INTERFACE_T *pMe,uint16 res,uint16 err_id)
{
    int result = 0;
    AI_TCP_LINK_DATA_T * GetData = pMe->m_tcp_cur_data;
        
    AI_TCP_StopReadTimer(pMe);
    AI_TCP_StopWriteTimer(pMe);
    
    TCP_LOG("AI__TCP AI TCP Send End interface_idx=%d,Handle=0x%x,res=%d,err_id=%d",pMe->m_tcp_interface_idx,pMe->m_tcp_cur_data,res,err_id);
    if(GetData != NULL)
    {
        TCP_LOG("AI__TCP AI TCP Send End Len=%d,Times=%d,timeout=%d,priority=%d",GetData->len,GetData->times,GetData->timeout,GetData->priority);
    }

    if(res == TCP_SUCCESS) //成功
    {
        if(pMe->m_tcp_callback != NULL)
        {
            pMe->m_tcp_callback(pMe,TCP_CB_SEND,TCP_SUCCESS,(void *)GetData);
        }
        AI_TCP_DelCurData(pMe);
        pMe->m_tcp_is_sending = FALSE;
        pMe->m_tcp_need_rcv = FALSE;
        if(AI_TCP_Link_Count(pMe))
        {
            AI_TCP_PostEvtTo_APP(AI_APP_SIGNAL_TCP_WRITE_START,(void *)pMe,0);
        }
    }
    else //失败
    {
        pMe->m_tcp_last_err = err_id;
        if(err_id == TCP_ERR_INCALL)
        {
            if(pMe->m_tcp_callback != NULL)
            {
                pMe->m_tcp_callback(pMe,TCP_CB_SEND,err_id,(void *)GetData);
            }
            AI_TCP_DelCurData(pMe);
            pMe->m_tcp_is_sending = FALSE;
            pMe->m_tcp_need_rcv = FALSE;
        }
        else if(err_id == TCP_ERR_LINKFULL)
        {
            if(pMe->m_tcp_callback != NULL)
            {
                pMe->m_tcp_callback(pMe,TCP_CB_SEND,err_id,NULL);
            }
            pMe->m_tcp_is_sending = FALSE;
            pMe->m_tcp_need_rcv = FALSE;
            if(AI_TCP_Link_Count(pMe))
            {
                AI_TCP_PostEvtTo_APP(AI_APP_SIGNAL_TCP_WRITE_START,(void *)pMe,0);
            }
        }
        else
        {
            uint16 times = 0;
            uint32 timeout = 0;
            if(pMe->m_tcp_cur_data != NULL)
            {
                times = pMe->m_tcp_cur_data->times;
                if(pMe->m_tcp_cur_data->times > 0)
                {
                    pMe->m_tcp_cur_data->times--;
                }
                timeout = pMe->m_tcp_cur_data->timeout;
            }
            
            if(err_id == TCP_ERR_NET
                || err_id == TCP_ERR_NET_NOSYS
                || err_id == TCP_ERR_NET_WRITE
                || err_id == TCP_ERR_NET_READ
                ) //Socket分配失败或网络错误
            {
                AITCP_Close(pMe);
                if(pMe->m_tcp_callback != NULL)
                {
                    pMe->m_tcp_callback(pMe,TCP_CB_DISCONNECTED,pMe->m_tcp_socket_status,NULL);
                }
                times = 0;
            }
            else if(err_id == TCP_ERR_NET_EMFILE)//Socket分配失败底层出错达到上限
            {
                AITCP_Close(pMe);
                if(pMe->m_tcp_callback != NULL)
                {
                    pMe->m_tcp_callback(pMe,TCP_CB_DISCONNECTED,pMe->m_tcp_socket_status,NULL);
                }
                times = 0;
            }
            else if(err_id == TCP_ERR_DISCONNECT)
            {
                AITCP_Close(pMe);
                if(pMe->m_tcp_callback != NULL)
                {
                    pMe->m_tcp_callback(pMe,TCP_CB_DISCONNECTED,pMe->m_tcp_socket_status,NULL);
                }
            }
            else if(err_id == TCP_ERR_TIMEOUT)
            {
                AITCP_Close(pMe);
                if(pMe->m_tcp_callback != NULL)
                {
                    pMe->m_tcp_callback(pMe,TCP_CB_DISCONNECTED,pMe->m_tcp_socket_status,NULL);
                }
                times = 0;
            }
            else if(err_id == TCP_ERR_NOREG
                || err_id == TCP_ERR_NOPS)
            {
                times = 0;
            }
            
            if(times == 0)
            {
                if(pMe->m_tcp_callback != NULL)
                {
                    pMe->m_tcp_callback(pMe,TCP_CB_SEND,err_id,(void *)GetData);
                }
                
                AI_TCP_DelCurData(pMe);
                pMe->m_tcp_is_sending = FALSE;
                pMe->m_tcp_need_rcv = FALSE;
                if(AI_TCP_Link_Count(pMe))
                {
                    AI_TCP_PostEvtTo_APP(AI_APP_SIGNAL_TCP_WRITE_START,(void *)pMe,0);
                }
            }
            else
            {
                if(timeout == 0)
                {
                    AI_TCP_StartReWriteTimer(pMe,200);
                }
                else
                {
                    if(timeout < 200)
                    {
                        timeout = 200;
                    }
                    AI_TCP_StartReWriteTimer(pMe,timeout);
                }
            }

        }
        
    }
    return result;

}

static int AITCP_Start(AI_TCP_INTERFACE_T *pMe)
{    
    if(pMe->m_tcp_is_sending)
    {
        TCP_LOG("AI__TCP AI TCP Send isSending...");
        return AI_TCP_RET_OK;
    }
    TCP_LOG("AI__TCP AITCP_Start");    
    AI_TCP_StopReWriteTimer(pMe);
    AI_TCP_DelCurData(pMe);
    
    if(pMe->m_tcp_is_reg && AI_TCP_Link_GetMaxPri(pMe,&pMe->m_tcp_cur_data))
    {
        TCP_LOG("AI__TCP AITCP_Start 2");  
        pMe->m_tcp_is_sending = TRUE;
        pMe->m_tcp_buf_get = pMe->m_tcp_cur_data->str;
        pMe->m_tcp_get_len = pMe->m_tcp_cur_data->len;
        if(pMe->m_tcp_buf_get == NULL || pMe->m_tcp_cur_data->len == 0)
        {
            AITcp_Send_Write_Resp(pMe,TCP_ERROR,TCP_ERR_MEMFULL);
            return AI_TCP_RET_ERR;
        }
        TCP_LOG("AI__TCP AITCP_Start 3");  
        AITCP_ConnWrite(pMe);
    }
    return AI_TCP_RET_OK;
}

static int AITCP_ReStart(AI_TCP_INTERFACE_T *pMe)
{        
    AI_TCP_StopReWriteTimer(pMe);
    if(pMe->m_tcp_is_reg && pMe->m_tcp_cur_data != NULL)
    {
        pMe->m_tcp_is_sending = TRUE;
        pMe->m_tcp_buf_get = pMe->m_tcp_cur_data->str;
        pMe->m_tcp_get_len = pMe->m_tcp_cur_data->len;
        if(pMe->m_tcp_buf_get == NULL || pMe->m_tcp_cur_data->len == 0)
        {
            AITcp_Send_Write_Resp(pMe,TCP_ERROR,TCP_ERR_MEMFULL);
            return AI_TCP_RET_ERR;
        }
        AITCP_ConnWrite(pMe);
    }
    else
    {
        AITcp_Send_Write_Resp(pMe,TCP_ERROR,TCP_ERR_NOREG);
    }
    return AI_TCP_RET_OK;
}

static int AITCP_GetUrlStr(AI_TCP_INTERFACE_T *pMe,char* host)
{
    uint16 len = strlen(host);
    if(len)
    {
        if(pMe->m_tcp_ip_url != NULL)
        {
            SCI_FREE(pMe->m_tcp_ip_url);
        }
        pMe->m_tcp_ip_url = SCI_ALLOC_APPZ(len+1);
        if(pMe->m_tcp_ip_url !=  NULL)
        {
            SCI_MEMCPY(pMe->m_tcp_ip_url,host,len);
        }
    }

    return AI_TCP_RET_OK;
}

int AITCP_CleanSendLink(AI_TCP_INTERFACE_T *pMe)
{    
    if(pMe->m_tcp_is_reg)
    {
        AITCP_CloseAllTimer(pMe);
        if(pMe->m_tcp_is_sending)
        {
            AITCP_End(pMe,TCP_ERROR,TCP_ERR_NOREG);
            pMe->m_tcp_is_sending = FALSE;
        }
        AI_TCP_DelCurData(pMe);
        while(AI_TCP_Link_GetMaxPri(pMe,&pMe->m_tcp_cur_data))
        {
            if(pMe->m_tcp_callback != NULL)
            {
                pMe->m_tcp_callback(pMe,TCP_CB_SEND,TCP_ERR_CLEAR,(void *)pMe->m_tcp_cur_data);
            }
            AI_TCP_DelCurData(pMe);
        }
    }
    return AI_TCP_RET_OK;
}

static int AITCP_RegStart(AI_TCP_INTERFACE_T *pMe,AI_TCP_REG_DATA_T * pData)
{   
    
    uint32 ip_addr = 0;
    int ip_err;
    TCP_LOG("AI__TCP AITCP_RegStart  start thread");
    if(AITcp_Open(pMe))
    {
        pMe->m_tcp_is_reg = TRUE;
        pMe->m_tcp_conn_port = pData->conn_port;
        pMe->m_tcp_callback = pData->callback;
        AITCP_GetUrlStr(pMe,pData->conn_ip);
        //IP 地址
        #if 0
        ip_err = inet_aton(pData->conn_ip, &ip_addr);
        if(ip_err == 1)
        {
            pMe->m_tcp_conn_ip= ip_addr;
        }
        else
        #endif
        {
            pMe->m_tcp_conn_ip = 0;
        }
        AITcp_SetSocketState(pMe,TCP_SOCKET_STATE_OPEN);
        TCP_LOG("AI__TCP AI TCP REG pMe=0x%x,Url:%s, Port=%d,IP=0x%x,%s",pMe,pData->conn_ip,pData->conn_port,pMe->m_tcp_conn_ip,pMe->m_tcp_ip_url);
        if(pData->callback != NULL)
        {
            pData->callback(pMe,TCP_CB_REG,TCP_SUCCESS,NULL);
        }
    }
    else
    {
        if(pData->callback != NULL)
        {
            pData->callback(pMe,TCP_CB_REG,TCP_ERR_SOCKET,NULL);
        }
    }
    return AI_TCP_RET_OK;
}

static  int AITCP_HandleReg(DPARAM param)
{
    int res = AI_TCP_RET_OK;
    AI_TCP_DATA_SIG_T * pp_getdata = (AI_TCP_DATA_SIG_T*)(param);
    AI_TCP_DATA_T * p_data = pp_getdata->p_data;
    AI_TCP_REG_DATA_T * pData = NULL;
    AI_TCP_INTERFACE_T *pMe = PNULL;
    if(p_data != NULL)
    {
        pData = (AI_TCP_REG_DATA_T *)(p_data->data_p);
    }
    TCP_LOG("AI__TCP AITCP_HandleReg");
    if(pData != NULL)
    {   
        pMe = (AI_TCP_INTERFACE_T *)pData->pMe;
        if(pMe->m_tcp_net_is_init == FALSE)
        {
            if(pData->callback != NULL)
            {
                pData->callback(pMe,TCP_CB_REG,TCP_ERROR,TCP_ERR_NOUIM);
            }
            
            if(pData->conn_ip != NULL)
            {
                SCI_FREE(pData->conn_ip);
            }
            SCI_FREE(pData);
            if(p_data != NULL)
            {
                SCI_FREE(p_data);
            }
            return AI_TCP_RET_ERR;
        }
        //AI_TCP_StopPdpCloseTimer(pMe);
        if(pMe->m_tcp_is_reg)
        {
            if(pData->callback != NULL)
            {
                pData->callback(pMe,TCP_CB_REG,TCP_ERR_REGEXIST,NULL);
            }
            
            if(pData->conn_ip != NULL)
            {
                SCI_FREE(pData->conn_ip);
            }
            SCI_FREE(pData);
            if(p_data != NULL)
            {
                SCI_FREE(p_data);
            }
            return AI_TCP_RET_ERR;
        }
        TCP_LOG("AI__TCP AITCP_RegStart m_ai_tcp_pdpstate=%d", m_ai_tcp_pdpstate );

        if(m_ai_tcp_pdpstate)
        {
            TCP_LOG("AI__TCP AITCP_RegStart");
            AITCP_RegStart(pMe,pData); //start thread task
            if(pData->conn_ip != NULL)
            {
                SCI_FREE(pData->conn_ip);
            }
            SCI_FREE(pData);
        }
        else
        {
            pMe->m_tcp_wait_net = TRUE;
            pMe->m_tcp_reg_data = pData;
            MMIAI_Net_Open();
        }
    }

    if(p_data != NULL)
    {
        SCI_FREE(p_data);
    }

    return res;
}

static  int AITCP_HandleUnReg(DPARAM param)
{
    int res = 0;
    AI_TCP_DATA_SIG_T * pp_getdata = (AI_TCP_DATA_SIG_T *)(param);
    AI_TCP_DATA_T * p_data = pp_getdata->p_data;
    AI_TCP_INTERFACE_T *pMe = PNULL;
    if(p_data != PNULL)
    {
        pMe = (AI_TCP_INTERFACE_T *)p_data->data_p;
    }
    
    if(pMe != PNULL)
    {
        TCP_LOG("AI__TCP AI TCP UNREG OK is_reg=%d,interface_idx=%d",pMe->m_tcp_is_reg,pMe->m_tcp_interface_idx);

        AITCP_CleanSendLink(pMe);
        
        AITCP_Reset(pMe);
        
        if(pMe->m_tcp_callback != NULL)
        {
            pMe->m_tcp_callback(pMe,TCP_CB_UNREG,TCP_SUCCESS,NULL);
        }
        pMe->m_tcp_callback = NULL;
        pMe->m_tcp_is_reg = FALSE;
        
        //AI_TCP_StartPdpCloseTimer(pMe);
    }
    if(p_data != NULL)
    {
        SCI_FREE(p_data);
    }
    return AI_TCP_RET_OK;
}

static int AITCP_HandleRcvData(DPARAM param)
{
    int res = 0;
    AI_TCP_DATA_SIG_T * pp_getdata = (AI_TCP_DATA_SIG_T *)(param);
    AI_TCP_DATA_T * p_data = pp_getdata->p_data;
    AI_TCP_INTERFACE_T *pMe = (AI_TCP_INTERFACE_T *)p_data->data_p;
    AI_TCP_RCV_DATA_T GetData = {0};
    while(AI_TCP_RCV_Get(pMe,&GetData) && GetData.pRcv != NULL)
    {
        int res = 0;
        
        if(pMe->m_tcp_callback != NULL)
        {
            res = pMe->m_tcp_callback(pMe,TCP_CB_RCV,TCP_SUCCESS,(void *)(&GetData));
        }
        if(res != 1)
        {
            if(GetData.pRcv != NULL)
            {
                SCI_FREE(GetData.pRcv);
            }
        }
    }
    
    if(p_data != NULL)
    {
        SCI_FREE(p_data);
    }
    return AI_TCP_RET_OK;
}

static  int AITCP_HandleSend(DPARAM param)
{
    int res = 0;
    AI_TCP_DATA_SIG_T * pp_getdata = (AI_TCP_DATA_SIG_T *)(param);
    AI_TCP_DATA_T * p_data = pp_getdata->p_data;
    AI_TCP_INTERFACE_T *pMe = PNULL;
    AI_TCP_LINK_DATA_T * pData = PNULL;
    AI_LOG("AITCP_HandleSend 1");
    if(p_data != NULL)
    {
        pData = (AI_TCP_LINK_DATA_T *)(p_data->data_p);
        AI_LOG("AITCP_HandleSend 2");
        
    }
    if(pData != NULL)
    {
         AI_LOG("AITCP_HandleSend 31");
        pMe = (AI_TCP_INTERFACE_T *)(pData->pMe); //bug ,pMe is null
        TCP_LOG("AI__TCP AITCP_HandleSend 32 pMe=0x%x %s:%d",pMe,pMe->m_tcp_ip_url,pMe->m_tcp_conn_port);
        if(AI_TCP_Link_AddData(pMe,pData))
        {
            AI_TCP_PostEvtTo_APP(AI_APP_SIGNAL_TCP_WRITE_START,(void *)pMe,0);
        }
        else
        {
            if(pData->str != NULL)
            {
                SCI_FREE(pData->str);
            }
            AITcp_Send_Write_Resp(pMe,TCP_ERROR,TCP_ERR_LINKFULL);
            SCI_FREE(pData);
        }
    }
    if(p_data != NULL)
    {
        SCI_FREE(p_data);
    }
    return res;
}
static  int AITCP_HandleSendStart(DPARAM param)
{
    int res = 0;
   
    AI_TCP_DATA_SIG_T * pp_getdata = (AI_TCP_DATA_SIG_T *)(param);
    AI_TCP_DATA_T * p_data = pp_getdata->p_data;
    AI_TCP_INTERFACE_T *pMe = (AI_TCP_INTERFACE_T *)(p_data->data_p);
    TCP_LOG("AI__TCP AITCP_HandleSendStart 1");
    if(pMe)
    {
        TCP_LOG("AI__TCP AITCP_HandleSendStart pMe=0x%x,is_reg=%d,%s",pMe,pMe->m_tcp_is_reg,pMe->m_tcp_ip_url);
    }
    
    
    AITCP_Start(pMe);

    if(p_data != NULL)
    {
        SCI_FREE(p_data);
    }

    return AI_TCP_RET_OK;
}

static  int AITCP_HandleSendReStart(DPARAM param)
{
    int res = 0;
    AI_TCP_DATA_SIG_T * pp_getdata = (AI_TCP_DATA_SIG_T *)(param);
    AI_TCP_DATA_T * p_data = pp_getdata->p_data;
    AI_TCP_INTERFACE_T *pMe = (AI_TCP_INTERFACE_T *)(p_data->data_p);
    TCP_LOG("AI__TCP AI TCP Send ReStart is_reg=%d",pMe->m_tcp_is_reg);
    
    AITCP_ReStart(pMe);

    if(p_data != NULL)
    {
        SCI_FREE(p_data);
    }

    return AI_TCP_RET_OK;
}

static  int AITCP_HandleSendSuccess(DPARAM param)
{
    int res = 0;
    AI_TCP_DATA_SIG_T * pp_getdata = (AI_TCP_DATA_SIG_T *)(param);
    AI_TCP_DATA_T * p_data = pp_getdata->p_data;
    AI_TCP_INTERFACE_T *pMe = PNULL;
    TCP_LOG("AI__TCP AITCP_HandleSendSuccess");

    if(p_data != NULL)
    {
        pMe = (AI_TCP_INTERFACE_T *)p_data->data_p;
        AITCP_End(pMe,TCP_SUCCESS,p_data->type);
        SCI_FREE(p_data);
    }

    return AI_TCP_RET_OK;
}

static  int AITCP_HandleSendFail(DPARAM param)
{
    int res = 0;
    AI_TCP_DATA_SIG_T * pp_getdata = (AI_TCP_DATA_SIG_T *)(param);
    AI_TCP_DATA_T * p_data = pp_getdata->p_data;
    AI_TCP_INTERFACE_T *pMe = PNULL;
    
    TCP_LOG("AI__TCP AITCP_HandleSendFail");
    
    if(p_data != NULL)
    {
        pMe = (AI_TCP_INTERFACE_T *)p_data->data_p;
        AITCP_End(pMe,TCP_ERROR,p_data->type);
        SCI_FREE(p_data);
    }

    return AI_TCP_RET_OK;
}

extern int AITCP_HandleConnected(DPARAM param)
{
    int res = 0;
    AI_TCP_DATA_SIG_T * pp_getdata = (AI_TCP_DATA_SIG_T *)(param);
    AI_TCP_DATA_T * p_data = pp_getdata->p_data;
    AI_TCP_INTERFACE_T *pMe = (AI_TCP_INTERFACE_T *)p_data->data_p;
    
    TCP_LOG("AI__TCP AI TCP Connected Handle status=%d",pMe->m_tcp_socket_status);
    
    AITcp_SetSocketState(pMe,TCP_SOCKET_STATE_CONNECTED);
    if(pMe->m_tcp_need_connect)
    {
        pMe->m_tcp_need_connect = FALSE;
        
        if(pMe->m_tcp_callback != NULL)
        {
            pMe->m_tcp_callback(pMe,TCP_CB_CONNECTED,pMe->m_tcp_socket_status,NULL);
        }
    }

    if(p_data != NULL)
    {
        SCI_FREE(p_data);
    }

    return AI_TCP_RET_OK;
}

extern int AITCP_HandleDisconnected(DPARAM param)
{
    int res = 0;
    AI_TCP_DATA_SIG_T * pp_getdata = (AI_TCP_DATA_SIG_T *)(param);
    AI_TCP_DATA_T * p_data = pp_getdata->p_data;
    AI_TCP_INTERFACE_T *pMe = (AI_TCP_INTERFACE_T *)p_data->data_p;
    
    TCP_LOG("AI__TCP AI TCP Disconneted Handle status=%d",pMe->m_tcp_socket_status);

    AITCP_Close(pMe);
    if(pMe->m_tcp_is_reg)
    {
        if(pMe->m_tcp_is_sending)
        {
            AITcp_Send_Write_Resp(pMe,TCP_ERROR,TCP_ERR_NOPS);
        }
        
        if(pMe->m_tcp_callback != NULL)
        {
            pMe->m_tcp_callback(pMe,TCP_CB_DISCONNECTED,pMe->m_tcp_socket_status,NULL);
        }
    }
    
    if(p_data != NULL)
    {
        SCI_FREE(p_data);
    }

    return AI_TCP_RET_OK;
}

PUBLIC MMI_RESULT_E  MMIAI_TCP_Handle_AppMsg (PWND app_ptr, uint16 msg_id,DPARAM param)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;
    AI_LOG("MMIAI_TCP_Handle_AppMsg ");
    switch(msg_id)
    {    
        case AI_APP_SIGNAL_TCP_CONNECT_SUCCESS:
            AI_LOG("AI_APP_SIGNAL_TCP_CONNECT_SUCCESS ");
                AITCP_HandleConnected(param);
            return MMI_RESULT_TRUE;
            
        case AI_APP_SIGNAL_TCP_DISCONNECTED:
            AI_LOG("AI_APP_SIGNAL_TCP_DISCONNECTED ");
                AITCP_HandleDisconnected(param);
            return MMI_RESULT_TRUE;
                        
        case AI_APP_SIGNAL_TCP_RCV_DATA:
            AI_LOG("AI_APP_SIGNAL_TCP_RCV_DATA ");
                AITCP_HandleRcvData(param);
            return MMI_RESULT_TRUE;
            
        case AI_APP_SIGNAL_TCP_UNREG:
            AI_LOG("AI_APP_SIGNAL_TCP_UNREG ");
                AITCP_HandleUnReg(param);
            return MMI_RESULT_TRUE;
            
        case AI_APP_SIGNAL_TCP_REG:
            AI_LOG("AI_APP_SIGNAL_TCP_REG ");
                AITCP_HandleReg(param);
            return MMI_RESULT_TRUE;

        case AI_APP_SIGNAL_TCP_WRITE_ADD:
            AI_LOG("AI_APP_SIGNAL_TCP_WRITE_ADD ");
            AITCP_HandleSend(param);
            return MMI_RESULT_TRUE;
            
        case AI_APP_SIGNAL_TCP_WRITE_START:
            AI_LOG("AI_APP_SIGNAL_TCP_WRITE_START ");
            AITCP_HandleSendStart(param);
            return MMI_RESULT_TRUE;
            
        case AI_APP_SIGNAL_TCP_WRITE_RESTART:
            AI_LOG("AI_APP_SIGNAL_TCP_WRITE_RESTART ");
                AITCP_HandleSendReStart(param);
            return MMI_RESULT_TRUE;
            
        case AI_APP_SIGNAL_TCP_WRITE_SUCCESS:
            AI_LOG("AI_APP_SIGNAL_TCP_WRITE_SUCCESS ");
                AITCP_HandleSendSuccess(param);
            return MMI_RESULT_TRUE;
            
        case AI_APP_SIGNAL_TCP_WRITE_FAIL:

            AI_LOG("AI_APP_SIGNAL_TCP_WRITE_FAIL ");
                AITCP_HandleSendFail(param);
            return MMI_RESULT_TRUE;
         default:
                res = MMI_RESULT_FALSE;
            break;
    }
    
    return res;

}

static BOOLEAN MMI_AI_TCP_PdpStateChange(BOOLEAN is_ok,BOOLEAN is_active)
{
    uint16 i = 0;
    AI_TCP_INTERFACE_T * pMe = PNULL;
    TCP_LOG("AI__TCP MMI_AI_TCP_PdpStateChange is_ok=%d, is_active=%d",is_ok,is_active);
    if(is_ok == FALSE)
    {
        for(i = 0; i < AI_TCP_INTERFACE_MAX; i++)
        {
            if(g_ai_tcp_interface_array[i].m_tcp_net_is_init)
            {
                pMe = &g_ai_tcp_interface_array[i];
                AI_API_TCP_UnReg(pMe);
            }
        }
    }

    return TRUE;
}

static BOOLEAN MMI_AI_TCP_PdpStateActive(BOOLEAN is_ok)
{
    uint16 i = 0;
    BOOLEAN res = FALSE;
    AI_TCP_INTERFACE_T * pMe = PNULL;
    TCP_LOG("AI__TCP MMI_AI_TCP_PdpStateActive is_ok=%d",is_ok);
    for(i = 0; i < AI_TCP_INTERFACE_MAX; i++)
    {
        if(g_ai_tcp_interface_array[i].m_tcp_net_is_init)
        {
            pMe = &g_ai_tcp_interface_array[i];
            if(pMe->m_tcp_wait_net)
            {
                res = TRUE;
                pMe->m_tcp_wait_net = FALSE;
                if(is_ok)
                {
                    if(pMe->m_tcp_reg_data != NULL)
                    {
                        AITCP_RegStart(pMe,pMe->m_tcp_reg_data);
                        if(pMe->m_tcp_reg_data->conn_ip != NULL)
                        {
                            SCI_FREE(pMe->m_tcp_reg_data->conn_ip);
                        }
                        SCI_FREE(pMe->m_tcp_reg_data);
                        pMe->m_tcp_reg_data = NULL;
                    }
                }
                else
                {
                    if(pMe->m_tcp_reg_data != NULL)
                    {
                        if(pMe->m_tcp_reg_data->callback != NULL)
                        {
                            pMe->m_tcp_reg_data->callback(pMe,TCP_CB_REG,TCP_ERR_NOPS,NULL);
                        }
                        if(pMe->m_tcp_reg_data->conn_ip != NULL)
                        {
                            SCI_FREE(pMe->m_tcp_reg_data->conn_ip);
                        }
                        SCI_FREE(pMe->m_tcp_reg_data);
                        pMe->m_tcp_reg_data = NULL;
                    }
                }
            }
        }
    }
    return res;
}

static BOOLEAN MMIAI_TCP_PdpStateUpdate(BOOLEAN is_ok,BOOLEAN is_active)
{
    BOOLEAN res = FALSE;
    TCP_LOG("AI__TCP MMIAI_TCP_PdpStateUpdate %d, is_active=%d",is_active);
    if(is_active)
    {
        res = MMI_AI_TCP_PdpStateActive(is_ok);
    }
    if(m_ai_tcp_pdpstate != is_ok)
    {
        m_ai_tcp_pdpstate = is_ok;
        MMI_AI_TCP_PdpStateChange(is_ok,is_active);
    }
    return TRUE;
}
PUBLIC BOOLEAN  MMIAI_TCP_Handle_PDPActiveOK(void)
{
    TCP_LOG("AI__TCP MMIAI_TCP_Handle_PDPActiveOK");
    MMIAI_TCP_PdpStateUpdate(TRUE,TRUE);
    return TRUE;
}

PUBLIC BOOLEAN  MMIAI_TCP_Handle_PDPActiveFail(void)
{
    TCP_LOG("AI__TCP MMIAI_TCP_Handle_PDPActiveFail");
    MMIAI_TCP_PdpStateUpdate(FALSE,TRUE);
    return TRUE;
}

PUBLIC BOOLEAN  MMIAI_TCP_Handle_PDPRespond(void)
{
    TCP_LOG("AI__TCP AI NET MMIAI_TCP_Handle_PDPRespond");
    MMIAI_TCP_PdpStateUpdate(FALSE,FALSE);
    return TRUE;
}


static BOOLEAN AITCP_NetGetInterface(AI_TCP_INTERFACE_T ** ppMe)
{
    uint16 i = 0; 
    for(i = 0; i < AI_TCP_INTERFACE_MAX; i++)
    {
        if(g_ai_tcp_interface_array[i].m_tcp_net_is_init == FALSE)
        {
            g_ai_tcp_interface_array[i].m_tcp_interface_idx = i+1;
            *ppMe = &g_ai_tcp_interface_array[i];
            TCP_LOG("AI__TCP AITCP_NetGetInterface %d",i);
            return TRUE;
        }
    }
    TCP_LOG("AI__TCP AITCP_NetGetInterface FAIL FULL");
    return FALSE;
}

BOOLEAN AITCP_NetOpen(AI_TCP_INTERFACE_T ** ppMe,AICMDCHECKHANDLER cmd_handle,uint16 rcv_len)
{
    if(AITCP_NetGetInterface(ppMe))
    {
        AI_TCP_INTERFACE_T * pMe = *ppMe;
        if(pMe != PNULL && pMe->m_tcp_net_is_init == FALSE)
        {
            TCP_LOG("AI__TCP AITCP_NetOpen interface_idx=%d",pMe->m_tcp_interface_idx);
            if(rcv_len == 0)
            {
                rcv_len = 1024;
            }
            pMe->m_tcp_buf_rcv = SCI_ALLOC_APPZ(rcv_len);
            if(pMe->m_tcp_buf_rcv == PNULL)
            {
                TCP_LOG("AI__TCP AITCP_NetOpen FAIL ALLOC %d",rcv_len);
                return FALSE;
            }
            pMe->m_tcp_net_is_init = TRUE;
            pMe->m_tcp_task_id == SCI_INVALID_BLOCK_ID;
            pMe->m_tcp_pISocket = TCPIP_SOCKET_INVALID;
            pMe->m_tcp_last_err = TCP_ERROR;
            pMe->m_tcp_need_connect = TRUE;
            pMe->m_tcp_rcv_len = rcv_len;
            pMe->m_tcp_cmd_check_handle = cmd_handle;
            return TRUE;
        }
    }
    return FALSE;
}

void AITCP_NetCloseSendMsg()
{
    
}

BOOLEAN AITCP_NetClose(AI_TCP_INTERFACE_T * pMe)
{
    if(pMe != PNULL && pMe->m_tcp_net_is_init == TRUE)
    {
        TCP_LOG("AI__TCP AITCP_NetClose interface_idx=%d",pMe->m_tcp_interface_idx);
        AITCP_CloseAllTimer(pMe);
        AITCP_Close(pMe);
        AI_TCP_DelCurData(pMe);
        AI_TCP_RCV_DelAll(pMe);
        AI_TCP_Link_DelAll(pMe);
        if(pMe->m_tcp_ip_url != NULL)
        {
            SCI_FREE(pMe->m_tcp_ip_url);
            pMe->m_tcp_ip_url = NULL;
        }
        if(pMe->m_tcp_buf_rcv != NULL)
        {
            SCI_FREE(pMe->m_tcp_buf_rcv);
            pMe->m_tcp_buf_rcv = NULL;
        }
        SCI_MEMSET(pMe,0,sizeof(AI_TCP_INTERFACE_T));
    }
    return TRUE;
}

int AITCP_AppInit(void)
{
    TCP_LOG("AI__TCP AITCP_AppInit");


    return 0;
}

int AITCP_AppFree(void)
{
    TCP_LOG("AI__TCP AITCP_AppFree");
    #ifdef AI_PLAT_HYHL_SUPPORT
        HYHL_AppFree();
    #endif
#ifdef AI_PLAT_YX_SUPPORT
       YX_AppFree();
#endif
    return 0;
}

BOOLEAN AITCP_NetInit(void)
{

    return TRUE;
}

extern int AI_API_TCP_SendDataNow(AI_TCP_INTERFACE_T * pMe,AI_TCP_LINK_DATA_T * pData)
{
    int res = 0;
    AI_LOG("AI_API_TCP_SendDataNow ");
    if(pData != NULL)
    {
        if(AI_TCP_Link_AddData(pMe,pData))
        {
            AI_TCP_PostEvtTo_APP(AI_APP_SIGNAL_TCP_WRITE_START,(void *)pMe,0);
        }
        else
        {
            if(pData->str != NULL)
            {
                SCI_FREE(pData->str);
            }
            AITcp_Send_Write_Resp(pMe,TCP_ERROR,TCP_ERR_LINKFULL);
            SCI_FREE(pData);
        }
    }
    return res;
}

extern uint32 AI_API_TCP_SendData(AI_TCP_INTERFACE_T * pMe,uint8 * pData,uint16 Data_len,uint8 priority,uint8 repeat_times,uint32 timeout,AITCPRCVHANDLER rcv_handle)
{
    AI_TCP_LINK_DATA_T * cur_data = NULL;
    uint16 len = Data_len;
    
    if(Data_len == 0 || pData == NULL )
    {
        return 0;
    }

    cur_data=(AI_TCP_LINK_DATA_T *)SCI_ALLOC_APPZ(sizeof(AI_TCP_LINK_DATA_T));
    if(cur_data == NULL)
    {
        TCP_LOG("AI__TCP AI TCP API SendData ERR Malloc");
        return 0;
    }

    cur_data->str= (uint8 *)SCI_ALLOC_APPZ(len +1);
    if(cur_data->str == NULL)
    {
        TCP_LOG("AI__TCP AI TCP API SendData ERR Malloc DataLen=%d",len +1);
        SCI_FREE(cur_data);
        return 0;
    }
    SCI_MEMSET(cur_data->str,0,len+1);
    cur_data->len = len;
    SCI_MEMCPY(cur_data->str,pData,len);
    cur_data->pMe = (void *)pMe;
    cur_data->times = repeat_times;
    cur_data->timeout  =timeout;
    cur_data->priority = priority;
    cur_data->rcv_handle =rcv_handle;
    AI_TCP_PostEvtTo_APP(AI_APP_SIGNAL_TCP_WRITE_ADD,cur_data,len);
    return (uint32)cur_data;
}

int AI_API_TCP_Reg(AI_TCP_INTERFACE_T *pMe,char * str_ip_url,uint16 conn_port,AITCPCBHANDLER callback)
{
    AI_TCP_REG_DATA_T * cur_data = NULL;
    uint16 ip_len = 0;
    if(pMe == NULL)
    {
        TCP_LOG("AI__TCP AI TCP AI_API_TCP_Reg Err No INTERFACE");
        return AI_TCP_RET_ERR;
    }
    TCP_LOG("AI__TCP AI_API_TCP_Reg pMe=0x%x",pMe);
    if(str_ip_url == NULL)
    {
        TCP_LOG("AI__TCP AI TCP AI_API_TCP_Reg Err No ip_str");
        return AI_TCP_RET_ERR;
    }

    ip_len= strlen(str_ip_url);

    if(ip_len == 0)
    {
        TCP_LOG("AI__TCP AI TCP AI_API_TCP_Reg Err ip_len");
        return AI_TCP_RET_ERR;
    }
    
    cur_data=(AI_TCP_REG_DATA_T *)SCI_ALLOC_APPZ(sizeof(AI_TCP_REG_DATA_T));
    if(cur_data == NULL)
    {
        TCP_LOG("AI__TCP AI TCP API REG ERR Malloc");
        return AI_TCP_RET_ERR;
    }
    
    cur_data->conn_ip = (char *)SCI_ALLOC_APPZ(ip_len+1);
    if(cur_data->conn_ip == NULL)
    {
        TCP_LOG("AI__TCP AI TCP API REG ERR Malloc IP Len=%d",ip_len+1);
        SCI_FREE(cur_data);
        return AI_TCP_RET_ERR;
    }
    cur_data->pMe = (void *)pMe;
    SCI_MEMCPY(cur_data->conn_ip,str_ip_url,ip_len);
    cur_data->conn_port = conn_port;
    cur_data->callback = callback;
    AI_TCP_PostEvtTo_APP(AI_APP_SIGNAL_TCP_REG,(void *)cur_data,ip_len);
  
  return AI_TCP_RET_OK;
}

int AI_API_TCP_UnReg(AI_TCP_INTERFACE_T *pMe)
{
    TCP_LOG("AI__TCP AI TCP API UNREG pMe=0x%x",pMe);
    AI_TCP_PostEvtTo_APP(AI_APP_SIGNAL_TCP_UNREG,(void *)pMe,0);
    return AI_TCP_RET_OK;
}

BOOLEAN AI_API_TCP_AllowClose(AI_TCP_INTERFACE_T *pMe)
{
    if(pMe->m_tcp_is_reg == FALSE)
    {
        return TRUE;
    }
    return FALSE;
}



