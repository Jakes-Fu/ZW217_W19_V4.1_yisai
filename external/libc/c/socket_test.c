#include "in.h"
#include "socket.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h"

LOCAL unsigned int my_ntp_netId = 0;


#define NTP_TIMESTAMP_DELTA 2208988800ull

#define HTTP_DEFAULT_PORT 80
#define HTTPS_SSL_DEFAULT_PORT 443
#define HTTPS_WTLS_DEFAULT_PORT 9203
#define HTTPS_WSP_DEFAULT_PORT 9201
#define RTSP_DEFAULT_PORT 554


typedef struct
{
    unsigned char li_vn_mode;

    unsigned char stratum;
    unsigned char poll;
    unsigned char precision;

    unsigned int rootDelay;
    unsigned int rootDispersion;
    unsigned int refId;

    unsigned int refTm_s;
    unsigned int refTm_f;

    unsigned int origTm_s;
    unsigned int origTm_f;

    unsigned int rxTm_s;
    unsigned int rxTm_f;

    unsigned int txTm_s;
    unsigned int txTm_f;

    
}ntp_packet;


void error(char * msg)
{
    SCI_TRACE_LOW("[grpc error] %s\n",msg);
}

LOCAL void my_HandlePdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if(PNULL == msg_ptr)
    {   
        error("pdp handle msg error");
        return;
    }

    SCI_TRACE_LOW("[grpc debug %s     %d]msg is %d, result %d",__FUNCTION__, __LINE__, msg_ptr->msg_id, msg_ptr->result);
    switch(msg_ptr->msg_id) 
    {
        case MMIPDP_ACTIVE_CNF:
            if(MMIPDP_RESULT_SUCC == msg_ptr->result)
            {
                my_ntp_netId = msg_ptr->nsapi;
                grpc_SetNetId(my_ntp_netId);
                SCI_TRACE_LOW("[grpc  debug %s     %d] PDP is actived, netId is %d\n",__FUNCTION__, __LINE__, my_ntp_netId);
                my_ntp_client();
            }
            else
            {
                SCI_TRACE_LOW("[grpc  error %s      %d]\n",__FUNCTION__,__LINE__);
            }
            break;

    case MMIPDP_DEACTIVE_CNF:
        SCI_TRACE_LOW("[grpc  debug %s      %d]deactive pdp CNF\n",__FUNCTION__,__LINE__);
        my_ntp_netId = 0;
        break;

    case MMIPDP_DEACTIVE_IND:
        SCI_TRACE_LOW("[grpc  debug %s      %d]deactive pdp IND\n",__FUNCTION__,__LINE__);
        my_ntp_netId = 0;
        break;

    default:
    break;
    }
}

LOCAL my_PDP_Active(MMIPDP_MSG_CALLBACK pdp_callback)
{
    BOOLEAN return_val = FALSE;
    uint8  linkset_num = 0;
    int dual_sys = 0;
    int item_index = 0;
    MMIPDP_ACTIVE_INFO_T app_info = {0};
    MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr = PNULL;

    linkset_num = MMIAPICONNECTION_GetLinkSettingNum(dual_sys);
    if (0 < linkset_num)
    {
        setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, item_index);
        if (setting_item_ptr != PNULL)
        {        
            app_info.app_handler = MMIAUT_GetModuleID();
            app_info.apn_ptr = (char*)setting_item_ptr->apn;
            app_info.user_name_ptr = (char*)setting_item_ptr->username;
            app_info.psw_ptr = (char*)setting_item_ptr->password;
            app_info.dual_sys = dual_sys;
            app_info.priority = 3;
            app_info.ps_service_rat = MN_UNSPECIFIED;
            app_info.handle_msg_callback = pdp_callback;
            app_info.ps_service_type = BROWSER_E;
            app_info.storage = MN_GPRS_STORAGE_ALL;
            my_ntp_netId = 0;
            return_val = MMIAPIPDP_Active(&app_info);
        }
    }
    return return_val;
}


PUBLIC void Start_test(void)
{
    /**
      * PDP active
     */
     SCI_TRACE_LOW("[grpc debug %s       %d]pdp active start\n",__FUNCTION__,__LINE__);
     my_PDP_Active(my_HandlePdpMsg);
}

#if 0
int main(void)
#else
PUBLIC int my_ntp_client(void)
#endif
{
    int socketfd;
    ntp_packet packet;
    time_t txTm;
    int n = 0;

    char *host_name = "baidu.com"; // host-name

    struct addrinfo hints;
    struct addrinfo *result = NULL, *resp;
    int s;
    
    memset(&packet, 0, sizeof(ntp_packet));
    *((char*)&packet + 0) = 0x1b;

    SCI_TRACE_LOW("[grpc debug %s       %d]\n",__FUNCTION__,__LINE__);
    if (PNULL == gethostbynameBlock(host_name)) //add for getaddrinfo => in46_reshost() => dns_query_entry_type() input netif is 0, so can not send upd to dns server
    {
        SCI_TRACE_LOW("gethostbynameBlock error");
        return -1;
    }
    
    //TODO change
    /* Call getaddrinfo */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;     /* ipv4 or ipv6 */
    hints.ai_socktype = SOCK_STREAM; /* stream socket */
    hints.ai_flags = AI_PASSIVE;     /* for wildcard IP address */
    
    SCI_TRACE_LOW("[grpc debug %s       %d]\n",__FUNCTION__,__LINE__);
    s = getaddrinfo(host_name, "http", &hints, &result);
    if (s != 0)
    {
        SCI_TRACE_LOW("getaddrinfo error, error: %s",gai_strerror(s));
        return -1;
    }
    
    SCI_TRACE_LOW("[grpc debug %s       %d]family is %d, ai_addr: %p\n",__FUNCTION__,__LINE__,result->ai_family,result->ai_addr);

    socketfd = socket(result->ai_family, SOCK_STREAM, 0);
    if (socketfd == -1)
    {
        error("socket error");
        if (result) {
            freeaddrinfo(result);
        }
        return -1;
    }
#if 0
    if (bind(socketfd, (const struct sockaddr *)(result->ai_addr), (socklen_t)(result->ai_addrlen)) < 0)
    {
        error("bind error");
        if (result) {
            freeaddrinfo(result);
        }
        return -1;
    }
#endif
    if (connect(socketfd, (struct sockaddr *)(result->ai_addr), (socklen_t)(result->ai_addrlen)) < 0)
    {
        error("connet error");
        if (result) {
            freeaddrinfo(result);
        }
        close(socketfd);
        return -1;
    }

    n = send(socketfd, (char *)&packet, sizeof(ntp_packet), 0);
    if (n < 0)
    {
        error("write error");
        if (result) {
            freeaddrinfo(result);
        }
        return -1;
    }

    n = recv(socketfd, (char *)&packet, sizeof(ntp_packet), 0);
    if (n < 0)
    {
        error("read error");
        if (result) {
            freeaddrinfo(result);
        }
        close(socketfd);
        return -1;
    }

    packet.txTm_s = ntohl(packet.txTm_s);
    packet.txTm_f = ntohl(packet.txTm_f);

    txTm = (time_t) (packet.txTm_s - NTP_TIMESTAMP_DELTA);

    SCI_TRACE_LOW("[debug grpc, time is %d]%s       %d\n",ctime((const time_t*)&txTm));
    if (result) {
        freeaddrinfo(result);
    }

    close(socketfd);
    return 0;

}
