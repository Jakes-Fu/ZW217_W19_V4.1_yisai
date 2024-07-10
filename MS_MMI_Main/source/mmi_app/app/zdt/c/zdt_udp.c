
#ifndef  _MMI_ZDT_UDP_C  
#define _MMI_ZDT_UDP_C  

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
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

#include "zdt_app.h"
#ifdef WIFI_SUPPORT
#include "wifisupp_api.h"
#endif

#define ZDT_UDP_TRACE            SCI_TRACE_LOW

#define ZDT_UDP_MAX_BUF_LEN 5024
#define UDP_PDP_CLOSE_TIMEOUT       5000  

LOCAL BOOLEAN                  s_zdt_udp_is_init = FALSE;
LOCAL uint8                       s_zdt_udp_pdp_close_timer_id = 0;

volatile BLOCK_ID zdt_udp_task_id = SCI_INVALID_BLOCK_ID;
LOCAL TCPIP_SOCKET_T zdt_udp_socket_id = TCPIP_SOCKET_INVALID;
LOCAL uint16 s_udp_socket_bind_port = 0;

LOCAL uint8 zdt_udp_sendbuf[ZDT_UDP_MAX_BUF_LEN+1]={0};
LOCAL uint8 zdt_udp_rcvbuf[ZDT_UDP_MAX_BUF_LEN+1]={0};

LOCAL BOOLEAN s_udp_is_poping_data = FALSE;
LOCAL BOOLEAN s_udp_pop_data_wait_net = FALSE;
LOCAL ZDT_UDP_DATA_T *        s_udp_cur_pop_pdata = PNULL;

LOCAL ZDT_UDP_DATA_LINK_NODE_T  *    s_udp_data_link_head = PNULL;

LOCAL BOOLEAN                  s_zdt_udp_pdpstate       = FALSE;

LOCAL void MMIZDT_UDP_PopSend_Result(uint8  result);
LOCAL BOOLEAN  MMIZDT_UDP_Send(ZDT_UDP_DATA_T * p_udp_data);
LOCAL BOOLEAN MMIZDT_UDP_PopSend(void);

PUBLIC BOOLEAN  MMIZDT_UDP_SendSigTo_APP(ZDT_APP_SIG_E sig_id, ZDT_UDP_DATA_T * p_udp_data)
{
    uint8 * pstr = PNULL;
    ZDT_UDP_SIG_T * psig = PNULL;
            
    //send signal to AT to write uart
    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(ZDT_UDP_SIG_T),SCI_IdentifyThread());
    psig->p_udp_data = p_udp_data;
    
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,P_APP);
    return TRUE;
}

PUBLIC BOOLEAN  MMIZDT_UDP_SendSigTo_Task(ZDT_UDP_TASK_SIG_E sig_id, ZDT_UDP_DATA_T * p_udp_data)
{
    uint8 * pstr = PNULL;
    ZDT_UDP_SIG_T * psig = PNULL;
    
    if(zdt_udp_task_id != SCI_INVALID_BLOCK_ID)
    {
        //send signal to AT to write uart
        SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(ZDT_UDP_SIG_T),SCI_IdentifyThread());
        psig->p_udp_data = p_udp_data;
        
        SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,zdt_udp_task_id);
        return TRUE;
    }
    return FALSE;
}

LOCAL void ZDT_UDP_HandlePdpCloseTimer(
                                uint8 timer_id,
                                uint32 param
                                )
{
    MN_DUAL_SYS_E sys_sim = 0;
    BOOLEAN       sim_ok =  FALSE;

    if(timer_id == s_zdt_udp_pdp_close_timer_id && 0 != s_zdt_udp_pdp_close_timer_id)
    {
        MMK_StopTimer(s_zdt_udp_pdp_close_timer_id);
        s_zdt_udp_pdp_close_timer_id = 0;
    }
#if 0
    if(ZDT_API_TCP_AllowClose() && MMIZDT_UDP_AllowClose())
    {
        ZDT_TCP_StopPdpCloseTimer();
        MMIZDT_Net_Close();
    }
#endif

}

LOCAL void ZDT_UDP_StartPdpCloseTimer(void)
{
    if(0 != s_zdt_udp_pdp_close_timer_id)
    {
        MMK_StopTimer(s_zdt_udp_pdp_close_timer_id);
        s_zdt_udp_pdp_close_timer_id = 0;
    }
    
    s_zdt_udp_pdp_close_timer_id = MMK_CreateTimerCallback(UDP_PDP_CLOSE_TIMEOUT, 
                                                                        ZDT_UDP_HandlePdpCloseTimer, 
                                                                        PNULL, 
                                                                        FALSE);
}

PUBLIC void ZDT_UDP_StopPdpCloseTimer(void)
{
    if(0 != s_zdt_udp_pdp_close_timer_id)
    {
        MMK_StopTimer(s_zdt_udp_pdp_close_timer_id);
        s_zdt_udp_pdp_close_timer_id = 0;
    }
}


//不 分配内存
LOCAL BOOLEAN ZDT_UDP_Link_Add(ZDT_UDP_DATA_T * p_data)
{
    ZDT_UDP_DATA_LINK_NODE_T  * p1 = PNULL;
    ZDT_UDP_DATA_LINK_NODE_T  * p2 = PNULL;
    
    if(p_data == PNULL 
        || p_data->str == PNULL 
        || p_data->str_len ==  0 
        || p_data->ip_str == PNULL
        )
    {
        return FALSE;
    }
    
    p1=(ZDT_UDP_DATA_LINK_NODE_T *)SCI_ALLOC_APPZ(sizeof(ZDT_UDP_DATA_LINK_NODE_T));
    if(p1 == PNULL)
    {
        return FALSE;
    }
    
    p1->p_data = p_data;    
    p1->next_ptr = PNULL;
    
    if(s_udp_data_link_head == PNULL)
    {
        s_udp_data_link_head = p1;
    }
    else
    {
        p2 = s_udp_data_link_head;
        while(p2->next_ptr != PNULL)
        {
            p2 = p2->next_ptr;
        }
        p2->next_ptr = p1;
    }
    
    return TRUE;
}

//不释放内存
LOCAL BOOLEAN   ZDT_UDP_Link_Get(ZDT_UDP_DATA_T ** pp_data)
{
    ZDT_UDP_DATA_LINK_NODE_T  * p1 = PNULL;
    ZDT_UDP_DATA_LINK_NODE_T  * p2 = PNULL;
    BOOLEAN res = FALSE;
    
    p2 = p1 = s_udp_data_link_head;
    
    while(p1 != PNULL)
    {
        if(p1->p_data != PNULL && p1->p_data->ip_str != PNULL && p1->p_data->str != PNULL)
        {
            *pp_data = p1->p_data;
            if(p1 == s_udp_data_link_head)
            {
                s_udp_data_link_head = p1->next_ptr;
            }
            else
            {
                p2->next_ptr = p1->next_ptr;
            }
            res = TRUE;
            SCI_FREE(p1);
            break;
        }
        p2 = p1;
        p1 = p2->next_ptr;
    }
    return res;
}

LOCAL BOOLEAN  ZDT_UDP_Link_DelAll(void)
{
    ZDT_UDP_DATA_LINK_NODE_T  * p1 = PNULL;
    while(s_udp_data_link_head != PNULL)
    {
        p1 = s_udp_data_link_head;
        s_udp_data_link_head = p1->next_ptr;
        if(p1->p_data != PNULL)
        {
            if(p1->p_data->str != PNULL)
            {
                SCI_FREE(p1->p_data->str);
            }
            if(p1->p_data->ip_str != PNULL)
            {
                SCI_FREE(p1->p_data->ip_str);
            }
            SCI_FREE(p1->p_data);
        }
        SCI_FREE(p1);
    }
    return TRUE;
}

LOCAL uint16  ZDT_UDP_Link_Count(void)
{
    uint16 num = 0;
    ZDT_UDP_DATA_LINK_NODE_T  * p1 = s_udp_data_link_head;
    while(p1 != PNULL)
    {
        num++;
        p1 = p1->next_ptr;
    }
    return num;
}

LOCAL ZDT_UDP_DATA_T *  ZDT_UDP_Link_PopData(void)
{
    uint8 * pstr = PNULL;
    ZDT_UDP_DATA_T * p_udp_data = PNULL;
    BOOLEAN res = FALSE;
    
    res = ZDT_UDP_Link_Get(&p_udp_data);
    
    if(res == FALSE || p_udp_data == NULL)
    {
        ZDT_LOG("ZDT_UDP_Link_PopData Empty !!!");
        return PNULL;
    }
    
    if(p_udp_data->str_len == 0 
        || p_udp_data->str == PNULL
        || p_udp_data->ip_str == PNULL
        )
    {
        if(p_udp_data->str != PNULL)
        {
            SCI_FREE(p_udp_data->str);
        }
        if(p_udp_data->ip_str != PNULL)
        {
            SCI_FREE(p_udp_data->ip_str);
        }
        SCI_FREE(p_udp_data);
        ZDT_LOG("ZDT_UDP_Link_PopData ERR Len");
        return PNULL;
    }
    
   return p_udp_data;
}

LOCAL BOOLEAN  ZDT_UDP_Send(uint8* buf, uint16 len,uint32 ip_addr,uint16 port)
{
    int send_res = 0;
    struct sci_sockaddr dest_addr;

    if(zdt_udp_socket_id == TCPIP_SOCKET_INVALID)
    {
        ZDT_UDP_TRACE("ZDT_UDP_Send ERR No Socket");
        return FALSE;
    }
    
    SCI_MEMSET(&dest_addr,0,sizeof(dest_addr));
    SCI_MEMSET(zdt_udp_sendbuf,0,ZDT_UDP_MAX_BUF_LEN+1);
    if(len > ZDT_UDP_MAX_BUF_LEN)
    {
        len = ZDT_UDP_MAX_BUF_LEN;
    }
    SCI_MEMCPY(zdt_udp_sendbuf,buf,len);
    
    dest_addr.family = AF_INET;
    dest_addr.ip_addr = ip_addr;
    dest_addr.port = htons(port);
    
    if(len > 0)
    {
        send_res = sci_sock_sendto (zdt_udp_socket_id, (char *)zdt_udp_sendbuf,len, 0, (struct sci_sockaddr *)&dest_addr);
        if(send_res < 0)
        {
            //失败
            
            ZDT_UDP_TRACE("ZDT_UDP_Send ERR sockfd=%ld, errno=%d",zdt_udp_socket_id,sci_sock_errno(zdt_udp_socket_id));
            return FALSE;
        }
    }
    ZDT_UDP_TRACE("ZDT_UDP_Send OK sockfd=%ld,Len=%d/%d,IP=0x%x,port=%d",zdt_udp_socket_id,send_res,len,ip_addr,port);
    return TRUE;
}

LOCAL BOOLEAN  ZDT_UDP_Send_Start(ZDT_UDP_DATA_T * p_udp_data)
{
    BOOLEAN res = FALSE;
        
    if(p_udp_data == PNULL || p_udp_data->str == PNULL || p_udp_data->str_len == 0)
    {
        ZDT_UDP_TRACE("ZDT_UDP_Send_Start ERR No Data");
        return FALSE;
    }
    
    if(p_udp_data->ip_addr == 0)
    {
        if(p_udp_data->ip_str == PNULL)
        {
            ZDT_UDP_TRACE("ZDT_UDP_Send_Start ERR No IP");
            return FALSE;
        }
        else
        {
            int rv = -1;
            uint32 send_addr = 0;
            rv = sci_parse_host_ext((char*)p_udp_data->ip_str,&send_addr,1,MMIZDT_Net_GetNetID());
            if(rv == 0)
            {
                char * ip_str = inet_ntoa(send_addr);
                ZDT_UDP_TRACE("ZDT_UDP_Send_Start DNS %s,%s,0x%x,%d",p_udp_data->ip_str,ip_str,send_addr,p_udp_data->port);
                res = ZDT_UDP_Send(p_udp_data->str,p_udp_data->str_len,send_addr,p_udp_data->port);
            }
            else
            {
                ZDT_UDP_TRACE("ZDT_UDP_Send_Start DNS ERR");
                return FALSE;
            }
        }
    }    
    else
    {
        ZDT_UDP_TRACE("ZDT_UDP_Send_Start ip_addr=0x%x,%s,%d",p_udp_data->ip_addr,p_udp_data->ip_str,p_udp_data->port);
        res = ZDT_UDP_Send(p_udp_data->str,p_udp_data->str_len,p_udp_data->ip_addr,p_udp_data->port);
    }
    return res;
}

LOCAL BOOLEAN ZDT_UDP_RcvSig(uint8 *str,uint16 str_len,uint32 ip_addr,uint16 port)
{
    ZDT_UDP_DATA_T * p_udp_data = PNULL;
    char * ip_str = NULL;
    uint16 ip_len = 0;
    
    p_udp_data = SCI_ALLOC_APPZ(sizeof(ZDT_UDP_DATA_T));
    if (p_udp_data == PNULL)
    {
        return FALSE;
    }
    
    p_udp_data->str = SCI_ALLOC_APPZ(str_len+1);
    if (p_udp_data->str == PNULL)
    {
        SCI_FREE(p_udp_data);
        return FALSE;
    }
    SCI_MEMCPY(p_udp_data->str,str,str_len);
    p_udp_data->str_len = str_len;
    
    p_udp_data->port = port;
    p_udp_data->ip_addr = ip_addr;
    
    ip_str = inet_ntoa(ip_addr);
    ip_len = strlen(ip_str);
    p_udp_data->ip_str = SCI_ALLOC_APPZ(ip_len+1);
    if (p_udp_data->ip_str == PNULL)
    {
        SCI_FREE(p_udp_data->str);
        SCI_FREE(p_udp_data);
        return FALSE;
    }
    SCI_MEMCPY(p_udp_data->ip_str,ip_str,ip_len);
    ZDT_UDP_TRACE("ZDT_UDP_RcvSig p_udp_data=0x%x,ip_str=%s,ip_addr=0x%x,port=%d,str_len=%d",p_udp_data,p_udp_data->ip_str,p_udp_data->ip_addr,p_udp_data->port,p_udp_data->str_len);

    #if 1
        //发送到APP
        MMIZDT_UDP_SendSigTo_APP(ZDT_APP_SIGNAL_UDP_RCV,p_udp_data);
    #else
    {
        // 发送到UDP TASK
        ZDT_UDP_SIG_T * psig = PNULL;
        SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig, SIG_ZDT_UDP_RCV, sizeof(ZDT_UDP_SIG_T),P_APP);
        psig->p_udp_data = p_udp_data;
        
        SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,zdt_udp_task_id);
    }
    #endif
    return TRUE;
}

LOCAL BOOLEAN  ZDT_UDP_Rcv(ZDT_UDP_DATA_T * p_udp_data)
{
    ZDT_UDP_TRACE("ZDT_UDP_Rcv p_udp_data=0x%x,ip_str=%s,ip_addr=0x%x,port=%d,str_len=%d",p_udp_data,p_udp_data->ip_str,p_udp_data->ip_addr,p_udp_data->port,p_udp_data->str_len);
    return TRUE;
}

LOCAL TCPIP_SOCKET_T ZDT_UDP_SocketCreate(uint16 bind_port, TCPIP_NETID_T net_id)
{
    TCPIP_SOCKET_T sockfd = TCPIP_SOCKET_INVALID;
    struct sci_sockaddr addr;
    char * ip_str = PNULL;
    ZDT_UDP_TRACE("UDP SocketCreate bind_port=%d,net_id=0x%x", bind_port,net_id);

    sockfd = sci_sock_socket(AF_INET,SOCK_DGRAM,0, net_id);
    if(sockfd == TCPIP_SOCKET_INVALID)
    {
        if(zdt_udp_socket_id != TCPIP_SOCKET_INVALID)
        {
            sci_sock_socketclose(zdt_udp_socket_id);
            zdt_udp_socket_id = TCPIP_SOCKET_INVALID;
            sockfd = sci_sock_socket(AF_INET,SOCK_DGRAM,0, net_id);
            if(sockfd == TCPIP_SOCKET_INVALID)
            {
                ZDT_UDP_TRACE("UDP SocketCreate ERR 1");
                return TCPIP_SOCKET_INVALID;
            }
        }
        else
        {
            ZDT_UDP_TRACE("UDP SocketCreate ERR 2");
            return TCPIP_SOCKET_INVALID;
        }
    }
        
    sci_sock_setsockopt(sockfd, SO_NBIO, NULL);

    if(bind_port > 0)
    {
        memset(&addr,0,sizeof(addr));
        addr.family = AF_INET;
        addr.ip_addr = sci_gethostaddress();
        addr.port = htons(bind_port);
        ip_str = inet_ntoa(addr.ip_addr);
        ZDT_UDP_TRACE("UDP socket bind host_ip=0x%x,%s,bind_port=%ld",addr.ip_addr,ip_str,bind_port);
        SCI_MEMSET((void*)addr.sa_data, 0, 8*sizeof(char));
        if(sci_sock_bind(sockfd, &addr,sizeof(addr)) < 0)
        {
            ZDT_UDP_TRACE("UDP socket bind Err bind_port=%d",bind_port);
            return TCPIP_SOCKET_INVALID;
        }
    }
    ZDT_UDP_TRACE("UDP SocketCreate OK sockfd= %ld",sockfd);
    return sockfd;
}

void ZDT_UDP_TaskEntry(uint32 argc, void *argv)
{
    xSignalHeaderRec*   sig_ptr = 0; 
    int ret = 0;
    int left_len=0;
    int recv_len=0;
    uint16 ip_port = 0;
    uint32 rcv_ip_addr = 0;
    char* ip_str=NULL;
    uint32 cur_net_id = MMIZDT_Net_GetNetID();
    struct sci_sockaddr  fromaddr;
    
    zdt_udp_task_id = SCI_IdentifyThread();
    zdt_udp_socket_id = ZDT_UDP_SocketCreate(s_udp_socket_bind_port, cur_net_id);
    if(zdt_udp_socket_id != TCPIP_SOCKET_INVALID)
    {
        ret = sci_sock_asyncselect(zdt_udp_socket_id, zdt_udp_task_id, AS_READ);
    }

    ZDT_UDP_TRACE("UDP Task Entry taskid=0x%x, socket_id=%d, ret=%d", zdt_udp_task_id, zdt_udp_socket_id, ret);
        
    while(1)
    {
        sig_ptr = SCI_GetSignal(zdt_udp_task_id); 
        switch(sig_ptr->SignalCode)
        {            
            case SOCKET_READ_EVENT_IND:
            case SOCKET_READ_BUFFER_STATUS_EVENT_IND:
                do
                {
                    memset(&fromaddr,0,sizeof(fromaddr));
                    memset(zdt_udp_rcvbuf, 0, ZDT_UDP_MAX_BUF_LEN);
                    recv_len = sci_sock_asyncrecvfrom (zdt_udp_socket_id,  zdt_udp_rcvbuf, ZDT_UDP_MAX_BUF_LEN, 0, (struct sci_sockaddr *)&fromaddr, &left_len);
                    ip_port = ntohs(fromaddr.port);
                    rcv_ip_addr = fromaddr.ip_addr;
                    if (recv_len > 0)
                    {
                        ZDT_UDP_RcvSig((uint8 *)zdt_udp_rcvbuf,(uint16)recv_len,rcv_ip_addr,ip_port);
                    }
                }while(left_len);
                break;
                
            case SIG_ZDT_UDP_SEND:
                {
                    ZDT_UDP_SIG_T* data_ptr=(ZDT_UDP_SIG_T*)sig_ptr;
                    if(data_ptr->p_udp_data != PNULL)
                    {
                        if(ZDT_UDP_Send_Start(data_ptr->p_udp_data))
                        {

                            MMIZDT_UDP_PopSend_Result(0);
                        }
                        else
                        {
                            MMIZDT_UDP_PopSend_Result(1);
                        }
                    }
                    else
                    {
                        ZDT_UDP_TRACE("UDP Task SIG_ZDT_UDP_SEND ERR NULL DATA");
                    }
                    break;
                }
            
            case SIG_ZDT_UDP_RCV:
                {
                    ZDT_UDP_SIG_T* data_ptr=(ZDT_UDP_SIG_T*)sig_ptr;
                    ZDT_UDP_Rcv(data_ptr->p_udp_data);
                    break;
                }

            }
            SCI_FREE(sig_ptr);
    }
}

LOCAL void ZDT_UDP_TaskClose(void)
{
	ZDT_LOG("ZDT_UDP_TaskClose, taskid=0x%x", zdt_udp_task_id);
	if(SCI_INVALID_BLOCK_ID != zdt_udp_task_id)
	{   
             if(zdt_udp_socket_id != TCPIP_SOCKET_INVALID)
             {
        		sci_sock_socketclose(zdt_udp_socket_id);
             }
		zdt_udp_socket_id = TCPIP_SOCKET_INVALID;
		SCI_TerminateThread(zdt_udp_task_id);
		SCI_DeleteThread(zdt_udp_task_id);
		zdt_udp_task_id = SCI_INVALID_BLOCK_ID;
	}
}

LOCAL BLOCK_ID ZDT_UDP_TaskCreate(void)
{
    ZDT_LOG("ZDT_UDP_TaskCreate, taskid=0x%x", zdt_udp_task_id);
    if(zdt_udp_task_id == SCI_INVALID_BLOCK_ID)
    {
        zdt_udp_task_id = SCI_CreateThread("ZDT_UDP_THREAD_NAME",
                                                                "ZDT_UDP_QUEUE_NAME",
                                                                ZDT_UDP_TaskEntry,
                                                                NULL,
                                                                NULL,
                                                                1024*10,
                                                                50,
                                                                TX_SYSTEM_NORMALE,	// TX_SYSTEM_HIGH,
                                                                SCI_PREEMPT,
                                                                SCI_AUTO_START
                                                                );
    }
    return zdt_udp_task_id;
}

LOCAL BOOLEAN ZDT_UDP_PdpStateChange(BOOLEAN is_ok)
{
    ZDT_LOG("ZDT_UDP_PdpStateChange %d",is_ok);
    if(is_ok == FALSE)
    {
        if(s_udp_is_poping_data)
        {
            MMIZDT_UDP_PopSend_Result(6);
        }
    }

    return TRUE;
}

LOCAL BOOLEAN ZDT_UDP_PdpStateUpdate(BOOLEAN is_ok,BOOLEAN is_active)
{
    ZDT_LOG("ZDT_UDP_PdpStateUpdate %d, is_active=%d",is_active);
    if(s_zdt_udp_pdpstate != is_ok)
    {
        s_zdt_udp_pdpstate = is_ok;
        ZDT_UDP_PdpStateChange(is_ok);
    }
    if(is_active)
    {
        if(s_udp_pop_data_wait_net)
        {
            s_udp_pop_data_wait_net = FALSE;
            if(is_ok)
            {
                if(s_udp_is_poping_data)
                {
                    if(s_udp_cur_pop_pdata != PNULL)
                    {
                        ZDT_UDP_TaskCreate();
                        if(MMIZDT_UDP_Send(s_udp_cur_pop_pdata) == FALSE)
                        {
                            MMIZDT_UDP_PopSend_Result(3);
                        }
                    }
                    else
                    {
                        MMIZDT_UDP_PopSend_Result(4);
                    }
                }
            }
            else
            {
                if(s_udp_is_poping_data)
                {
                    MMIZDT_UDP_PopSend_Result(5);
                }
            }
        }
    }

    return TRUE;
}

BOOLEAN MMIZDT_UDP_Init(void)
{
    if(s_zdt_udp_is_init == FALSE)
    {
        s_zdt_udp_is_init = TRUE;
        MMIZDT_UDP_PopSend();
    }
    return TRUE;
}
LOCAL BOOLEAN  MMIZDT_UDP_Send(ZDT_UDP_DATA_T * p_udp_data)
{
    return MMIZDT_UDP_SendSigTo_Task(SIG_ZDT_UDP_SEND,p_udp_data);
}

LOCAL BOOLEAN  MMIZDT_UDP_ReSend(ZDT_UDP_DATA_T * p_udp_data)
{
    return MMIZDT_UDP_SendSigTo_APP(ZDT_APP_SIGNAL_UDP_RESEND,p_udp_data);
}

LOCAL void MMIZDT_UDP_PopSend_Result(uint8  result)
{
    if(result == 0)
    {
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_UDP_SEND_OVER,PNULL,0);
    }
    else
    {
        MMIZDT_SendSigTo_APP(ZDT_APP_SIGNAL_UDP_SEND_OVER,(uint8 *)"1",1);
    }
}

LOCAL BOOLEAN MMIZDT_UDP_PopSend(void)
{
    ZDT_UDP_DATA_T * p_udp_data = PNULL;
    BOOLEAN res = FALSE;
    
    if(s_udp_is_poping_data ||FALSE == s_zdt_udp_is_init)
    {
        ZDT_LOG("MMIZDT_SMS_PopSend ERR Busy is_poping_data=%d",s_udp_is_poping_data);
        return PNULL;
    }

    p_udp_data = ZDT_UDP_Link_PopData();
    if(p_udp_data != PNULL)
    {
        ZDT_UDP_StopPdpCloseTimer();
        s_udp_is_poping_data = TRUE;
        s_udp_cur_pop_pdata = p_udp_data;
        if(s_zdt_udp_pdpstate)
        {
            ZDT_UDP_TaskCreate();
            res = MMIZDT_UDP_Send(p_udp_data);
            if(res == FALSE)
            {
                MMIZDT_UDP_PopSend_Result(2);
            }
        }
        else
        {
            ZDT_LOG("MMIZDT_SMS_PopSend PDP ACTIVE");
            s_udp_pop_data_wait_net = TRUE;
            MMIZDT_Net_Open();
        }
    }
    return res;
}

PUBLIC BOOLEAN MMIZDT_UDP_IsSending(void)
{
    return s_udp_is_poping_data;
}

PUBLIC BOOLEAN MMIZDT_UDP_AllowClose(void)
{
    if(s_udp_is_poping_data == FALSE && ZDT_UDP_Link_Count() == 0)
    {
        return TRUE;
    }
    return FALSE;
}

PUBLIC BOOLEAN MMIZDT_UDP_AppSend(uint8 *str,uint16 str_len,char * ip_str,uint16 port)
{
    ZDT_UDP_DATA_T * p_udp_data = PNULL;
    BOOLEAN res = FALSE;
    uint32 ip_addr = 0;
    int ip_err;
    uint16 ip_len = 0;

    if(s_zdt_udp_is_init == FALSE)
    {
        ZDT_LOG("MMIZDT_UDP_AppSend ERR NOT INIT");
        return FALSE;
    }
    
    if(str == PNULL || str_len == 0
       || ip_str == PNULL || port == 0)
    {
        return FALSE;
    }
    
    p_udp_data = SCI_ALLOC_APPZ(sizeof(ZDT_UDP_DATA_T));
    if (p_udp_data == PNULL)
    {
        return FALSE;
    }
    
    p_udp_data->str = SCI_ALLOC_APPZ(str_len+1);//free it in AT task
    if (p_udp_data->str == PNULL)
    {
        SCI_FREE(p_udp_data);
        return FALSE;
    }
    SCI_MEMCPY(p_udp_data->str,str,str_len);
    p_udp_data->str_len = str_len;
    
    p_udp_data->port = port;

    ip_len = strlen(ip_str);
    p_udp_data->ip_str = SCI_ALLOC_APPZ(ip_len+1);
    if (p_udp_data->ip_str == PNULL)
    {
        SCI_FREE(p_udp_data->str);
        SCI_FREE(p_udp_data);
        return FALSE;
    }
    SCI_MEMCPY(p_udp_data->ip_str,ip_str,ip_len);

    //IP 地址
    ip_err = inet_aton(ip_str, &ip_addr);
    if(ip_err > 0)
    {
        p_udp_data->ip_addr = ip_addr;
    }
    else
    {
        p_udp_data->ip_addr = 0;
    }
       
    res = MMIZDT_UDP_SendSigTo_APP(ZDT_APP_SIGNAL_UDP_SEND,p_udp_data);
    if(res == FALSE)
    {
        SCI_FREE(p_udp_data->ip_str);
        SCI_FREE(p_udp_data->str);
        SCI_FREE(p_udp_data);
    }
    return res;

}

BOOLEAN  MMIZDT_UDP_Handle_Send(DPARAM param)
{
    BOOLEAN res = FALSE;
    ZDT_UDP_SIG_T * pp_getdata = (ZDT_UDP_SIG_T *)(param);
    ZDT_UDP_DATA_T * p_getdata = pp_getdata->p_udp_data;
    if(p_getdata == PNULL)
    {
        ZDT_LOG("MMIZDT_UDP_Handle_Send ERR NULL DATA");
        return FALSE;
    }
    
    res = ZDT_UDP_Link_Add(p_getdata);
    if(res == FALSE)
    {
        if(p_getdata->ip_str != NULL)
        {
            SCI_FREE(p_getdata->ip_str);
        }
        if(p_getdata->str != NULL)
        {
            SCI_FREE(p_getdata->str);
        }
        SCI_FREE(p_getdata);
        ZDT_LOG("MMIZDT_UDP_Handle_Send ERR LINK ADD");
    }
        
    ZDT_LOG("MMIZDT_UDP_Handle_Send is_poping_data=%d",s_udp_is_poping_data);
    if(s_udp_is_poping_data == FALSE)
    {
        MMIZDT_UDP_PopSend();
    }

    return TRUE;
}

BOOLEAN  MMIZDT_UDP_Handle_ReSend(DPARAM param)
{
    return TRUE;
}

BOOLEAN  MMIZDT_UDP_Handle_Rcv(DPARAM param)
{
    BOOLEAN res = FALSE;
    ZDT_UDP_SIG_T * pp_getdata = (ZDT_UDP_SIG_T *)(param);
    ZDT_UDP_DATA_T * p_udp_data = pp_getdata->p_udp_data;
    
    if(p_udp_data != PNULL)
    {
        ZDT_LOG("MMIZDT_UDP_Handle_Rcv p_udp_data=0x%x,ip_str=%s,ip_addr=0x%x,port=%d,str_len=%d",p_udp_data,p_udp_data->ip_str,p_udp_data->ip_addr,p_udp_data->port,p_udp_data->str_len);
        if(p_udp_data->ip_str != NULL)
        {
            SCI_FREE(p_udp_data->ip_str);
        }
        if(p_udp_data->str != NULL)
        {
            SCI_FREE(p_udp_data->str);
        }
        SCI_FREE(p_udp_data);
    }
    return TRUE;
}

BOOLEAN  MMIZDT_UDP_Handle_SendOver(DPARAM param)
{
    MMI_ZDT_SIG_T * pp_getdata = (MMI_ZDT_SIG_T *)param;
    MMI_ZDT_DATA_T * p_getdata = &(pp_getdata->data);
    uint16 udp_link_coundt = ZDT_UDP_Link_Count();
    
    if(p_getdata->len == 0)
    {
        //成功
        ZDT_LOG("MMIZDT_UDP_Handle_SendOver OK,is_poping =%d,p_getdata=0x%x",s_udp_is_poping_data,s_udp_cur_pop_pdata);
        if(s_udp_is_poping_data)
        {
            if(s_udp_cur_pop_pdata != PNULL)
            {
                if(s_udp_cur_pop_pdata->ip_str != PNULL)
                {
                    SCI_FREE(s_udp_cur_pop_pdata->ip_str);
                }

                if(s_udp_cur_pop_pdata->str != PNULL)
                {
                    SCI_FREE(s_udp_cur_pop_pdata->str);
                }
                SCI_FREE(s_udp_cur_pop_pdata);
            }
            s_udp_is_poping_data = FALSE;
            MMIZDT_UDP_PopSend();
        }

    }
    else
    {
        //失败
        ZDT_LOG("MMIZDT_UDP_Handle_SendOver FAIL, is_poping =%d,p_getdata=0x%x",s_udp_is_poping_data,s_udp_cur_pop_pdata);
        if(s_udp_is_poping_data)
        {
            if(s_udp_cur_pop_pdata != PNULL)
            {
                if(s_udp_cur_pop_pdata->ip_str != PNULL)
                {
                    SCI_FREE(s_udp_cur_pop_pdata->ip_str);
                }

                if(s_udp_cur_pop_pdata->str != PNULL)
                {
                    SCI_FREE(s_udp_cur_pop_pdata->str);
                }
                SCI_FREE(s_udp_cur_pop_pdata);
            }
            s_udp_is_poping_data = FALSE;
            MMIZDT_UDP_PopSend();
        }
    }
    
    if(udp_link_coundt == 0)
    {
        ZDT_UDP_TaskClose();
        ZDT_UDP_StartPdpCloseTimer();
    }

    if(p_getdata->str != NULL)
    {
        SCI_FREE(p_getdata->str);
    }

    return TRUE;
}

PUBLIC BOOLEAN  MMIZDT_UDP_Handle_PDPActiveOK(void)
{
    ZDT_LOG("MMIZDT_UDP_Handle_PDPActiveOK");
    ZDT_UDP_PdpStateUpdate(TRUE,TRUE);
    return TRUE;
}

PUBLIC BOOLEAN  MMIZDT_UDP_Handle_PDPActiveFail(void)
{
    ZDT_LOG("MMIZDT_UDP_Handle_PDPActiveFail");
    ZDT_UDP_PdpStateUpdate(FALSE,TRUE);
    return TRUE;
}

PUBLIC BOOLEAN  MMIZDT_UDP_Handle_PDPRespond(void)
{
    ZDT_LOG("ZDT NET MMIZDT_UDP_Handle_PDPRespond");
    ZDT_UDP_PdpStateUpdate(FALSE,FALSE);
    return TRUE;
}

PUBLIC MMI_RESULT_E  MMIZDT_UDP_Handle_AppMsg (PWND app_ptr, uint16 msg_id,DPARAM param)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;
    switch(msg_id)
    {                       
        case ZDT_APP_SIGNAL_UDP_SEND:
                MMIZDT_UDP_Handle_Send(param);
            break;
            
        case ZDT_APP_SIGNAL_UDP_RESEND:
                MMIZDT_UDP_Handle_ReSend(param);
            break;
            
        case ZDT_APP_SIGNAL_UDP_RCV:
                MMIZDT_UDP_Handle_Rcv(param);
            break;
            
        case ZDT_APP_SIGNAL_UDP_SEND_OVER:
                MMIZDT_UDP_Handle_SendOver(param);
            break;
            
        default:
                res = MMI_RESULT_FALSE;
            break;
    }
    return res;
}

PUBLIC BOOLEAN MMIZDT_UDP_APP_Init(void)
{
    return TRUE;
}

PUBLIC BOOLEAN  MMIZDT_UDP_Test(void)
{
    #if 1
        return MMIZDT_UDP_AppSend((uint8 *)"UDPTEST",7,"120.24.172.55",9104);
    #else
    uint8 mac_addr[6] = {0x00,0x03,0x7f,0xc8,0xf5,0x4b};
    uint8 mac_addr2[6] = {0x00,0x03,0x7f,0xca,0xac,0x91};
    char sendbuf[ZDT_UDP_MAX_BUF_LEN+1]={0};
    uint16 send_len = 0;
    sprintf(sendbuf, "ML:%02x:%02x:%02x:%02x:%02x:%02x,1,%02x:%02x:%02x:%02x:%02x:%02x",
        mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5],
        mac_addr2[0], mac_addr2[1], mac_addr2[2], mac_addr2[3], mac_addr2[4], mac_addr2[5]);

    send_len = strlen(sendbuf);
    return MMIZDT_UDP_AppSend(sendbuf,send_len,"120.24.172.55",8800);
    #endif
    
}

PUBLIC BOOLEAN  MMIZDT_UDP_TestDomain(void)
{
    #if 1
        return MMIZDT_UDP_AppSend((uint8 *)"UDPTEST",7,"ecell.top",9104);
    #else
    uint8 mac_addr[6] = {0x00,0x03,0x7f,0xc8,0xf5,0x4b};
    uint8 mac_addr2[6] = {0x00,0x03,0x7f,0xca,0xac,0x91};
    char sendbuf[ZDT_UDP_MAX_BUF_LEN+1]={0};
    uint16 send_len = 0;
    sprintf(sendbuf, "ML:%02x:%02x:%02x:%02x:%02x:%02x,1,%02x:%02x:%02x:%02x:%02x:%02x",
        mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5],
        mac_addr2[0], mac_addr2[1], mac_addr2[2], mac_addr2[3], mac_addr2[4], mac_addr2[5]);

    send_len = strlen(sendbuf);
    
    return MMIZDT_UDP_AppSend(sendbuf,send_len,"ecell.top",8800);
    #endif
}

#endif
