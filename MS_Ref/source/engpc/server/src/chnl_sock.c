#include "sci_types.h"
#include "os_api.h"
#include "socket_api.h"

#include "sprd_fts_log.h"

#include "chnl.h"
#include "chnl_sock.h"
#include "chnl_wifi.h"
#include "chnl_pc.h"
#include "dal_power.h"

//engpc thread: cp response to pc tool
#define P_ENGPC_PC_SOCK_STACK_SIZE     (1024*4)
#define P_ENGPC_PC_SOCK_QUEUE_NUM      15
#define P_ENGPC_PC_SOCK_TASK_PRIORITY  78 //31

LOCAL BLOCK_ID s_engpc_pc_sock_task = SCI_INVALID_BLOCK_ID;
TCPIP_SOCKET_T s_sock_srv = TCPIP_SOCKET_INVALID, s_sock_diag = TCPIP_SOCKET_ERROR;
#define ENGPC_SOCK_PORT (10000)

LOCAL BOOLEAN GetLocalIpAddr(char *ip_ptr,uint32 buf_len)
{
    unsigned long ip_u32 = 0;
    
    ip_u32 = (unsigned long)sci_gethostaddress();
    {
        typedef union
        {
            uint32 whole;
            struct 
            {
                uint32 byte0 : 8;
                uint32 byte1 : 8;
                uint32 byte2 : 8;
                uint32 byte3 : 8;
            }all_part;
        }DEPART_T;
        
        DEPART_T depart = {0};
        depart.whole = ip_u32;

        _snprintf(
            ip_ptr,
            buf_len,
            "%d.%d.%d.%d",
            depart.all_part.byte0,
            depart.all_part.byte1,
            depart.all_part.byte2,
            depart.all_part.byte3
            );
    }
    return TRUE;
}

PUBLIC int pc_sock_diag_open()
{
    struct sci_sockaddr addr = {0};
    int addr_len = 0;
    int ret = -1;

    if(s_sock_srv == TCPIP_SOCKET_INVALID){
        ENG_LOG("s_sock_srv is invalidate!!!!");
        return -1;
    }

    do{
        ENG_LOG("%s: sock accept start: s_sock_srv = 0x%x", __FUNCTION__, s_sock_srv);
        s_sock_diag = sci_sock_accept(s_sock_srv, (struct sci_sockaddrext*)&addr, &addr_len);
        ENG_LOG("%s: sock accept end: accept ret %x, error %d", __FUNCTION__, s_sock_diag, sci_sock_errno(s_sock_diag));
        if(s_sock_srv == TCPIP_SOCKET_INVALID){
            ENG_LOG("s_sock_srv is invalidate!!!!");
            return -1;
        }else  if (s_sock_diag == TCPIP_SOCKET_ERROR){
            ENG_LOG("%s: accept fail!", __FUNCTION__);
            continue;
        }else{
            ENG_LOG("%s: accept ok!", __FUNCTION__);
            return 0;
        }
    }while(1);


    return -3;
}

PUBLIC void pc_sock_diag_close()
{
    activeCurConnectType(PC_CONNECT_COM);

    if(s_sock_diag != TCPIP_SOCKET_ERROR){
        sci_sock_socketclose(s_sock_diag);
        s_sock_diag = TCPIP_SOCKET_ERROR;
    }
}

PUBLIC int pc_sock_read_data(CHNL_DATA_TYPE type, char* buff, int len){
    int ret = 0;

    ENG_LOG("%s: read fd = %x", __FUNCTION__, s_sock_diag);

    if(s_sock_diag != TCPIP_SOCKET_ERROR){
        int status;
        ret = sci_sock_recv(s_sock_diag, buff, len, 0);
        status = chnl_wifi_isConnect();
        if(status == WIFI_STATUS_DISCONNECTING || status == WIFI_STATUS_DISCONNECTED){
            ENG_LOG("wifi is disconnect!!!!break");
        }
/*

        do{
            sci_fd_set  fd_read;
            long        time_out;
            int         return_val = 0;
            int32      socket;
            int status;

            socket = s_sock_diag;
            SCI_FD_ZERO( &fd_read );
            SCI_FD_SET( (long)socket, &fd_read );
            time_out = 5; //0.5s

            // socket select
            return_val = sci_sock_select( &fd_read, NULL, NULL, time_out );
            status = chnl_wifi_isConnect();
            if(status == WIFI_STATUS_DISCONNECTING || status == WIFI_STATUS_DISCONNECTED){
                ENG_LOG("wifi is disconnect!!!!break");
                break;
            }
            else if( 0 < return_val)
            {
                if ( SCI_FD_ISSET( socket, &fd_read ) ) // check whether socket in read field
                {
                    ret = sci_sock_recv(socket, buff, len, 0);
                    break;
                }
            }
            else if ( 0 == return_val )   // time out
            {
                continue ;
            }
            else  // error
            {
                ENG_LOG(" - select() error: %d ", sci_sock_errno(s_sock_diag));
                continue ;
            }
        }while(TRUE);
*/
    }
        
    ENG_LOG("%s: return %d", __FUNCTION__, ret);
    printData(findNameByChnlType(CHNL_DIAG_PC_CP), CP_DIAG, buff, ret, PRINT_NUM_MAX, PRINT_BRK_FLAG);

    return ret;
}

#define PC_SOCK_WRITE_RETRY    60

PUBLIC int pc_sock_write_data(CHNL_DATA_TYPE type, char* buff, int len){
    int ret = 0;
    int offset = 0;
    int retry = PC_SOCK_WRITE_RETRY*5;

    ENG_LOG("%s: write fd = %x, len = %d", __FUNCTION__, s_sock_diag, len);
    printData(findNameByChnlType(CHNL_DIAG_PC_CP), CP_DIAG, buff, len, PRINT_NUM_MAX, PRINT_BRK_FLAG);

    do{
        if(s_sock_diag == TCPIP_SOCKET_ERROR)
            break;

        ret = sci_sock_send(s_sock_diag, buff+offset, len, 0);
        if (ret < 0){
            if(retry-- <=0){
                ENG_LOG("error: spipe write fail! break");
                break;
            }else{
                SCI_Sleep(500);
                ENG_LOG("error: spipe write fail! retry, error = %d", ret);
            }
        }else{
            len -= ret;
            offset += ret;
        }

    }while(len > 0);

    ENG_LOG("%s: write %x", __FUNCTION__, offset);
    return offset;
}

void ping_cb(
    TCPIP_PING_RESULT_E     res,            /* ping result, 0 - succeed; other - fail */
    uint32                              time_delay,     /* ping time delay, only valid when success, unit: ms */
    TCPIP_PING_HANDLE       ping_handle,  /* ping handle */
    uint8                                ttl,
    char*                               ipaddr
    /*uint16                          ipaddr_size*/)
{
  ENG_LOG("res = %d, time_delay=%d, ping_handle=0x%x, ttl=%d, ipaddr = %s", res, time_delay, ping_handle, ttl, ipaddr);  
}

void testPing()
{
    char addr[]="172.20.10.6";
    sci_ping_request(addr, sizeof(addr), 1000, ping_cb, 0);
}

int eng_at_getip_func(char *req, char *rsp)
{
    char ipAddr[64] = {0};

    strcpy(rsp, "+SPGETIP:");
    if(GetLocalIpAddr(ipAddr, sizeof(ipAddr))){
        ENG_LOG("%s: ip = %s", __FUNCTION__, ipAddr);
        strcat(rsp, ipAddr);
    }else{
        ENG_LOG("%s: get ip fail!!", __FUNCTION__);
        strcat(rsp, "ERROR");
    }

    return strlen(rsp);
}
    
LOCAL int waitForWifiConnect(int timeout){
    int out = timeout;
    while(out-- > 0){
        if(chnl_wifi_isConnect() == WIFI_STATUS_CONNECTED){
            ENG_LOG("wifi is connected! break!");
            return 0;
        }
        SCI_Sleep(1000);
    }

    return -1;
}

void chnl_sock_close()
{
    if(s_sock_srv != TCPIP_SOCKET_INVALID){
        sci_sock_socketclose(s_sock_srv);
        s_sock_srv = TCPIP_SOCKET_INVALID;
    }
}

extern int wifi_hal_getNetId();
#define WIFI_CONNECT_TIMEOUT (5*60)
PUBLIC void chnl_sock_cb(uint32 argc, void* argv )
{
    int  err_type = 0;
    TCPIP_NETID_T netid = 0;
    struct sci_sockaddr addr = {0};
    int time = 60;
    int ret = -1;
    char ipAddr[32] = {0};
    int reconnect = 0;

    do{
        if(waitForWifiConnect(WIFI_CONNECT_TIMEOUT) != 0){
            ENG_LOG("waitForWifiConnect fail!!!!");
            continue;
        }
        ENG_LOG("wait for wifi connect succ!");
        netid = wifi_hal_getNetId();
        if (TCPIP_NETID_NULL == netid)
        {
            ENG_LOG("netid is NULL!!!!");
            //return ;
        }

        if(GetLocalIpAddr(ipAddr, sizeof(ipAddr))){
            ENG_LOG("%s: ip = %s", __FUNCTION__, ipAddr);
        }else{
            ENG_LOG("%s: get ip fail!!", __FUNCTION__);
        }

        do{
            if (s_sock_srv == TCPIP_SOCKET_INVALID){
                s_sock_srv = sci_sock_socket(AF_INET, SOCK_STREAM, 0, netid);
            }else{
                ret = sci_sock_modifynetid(s_sock_srv, netid);
                ENG_LOG("modifynetId return %d, error %d", ret, sci_sock_errno(s_sock_srv));
                break;
            }

            ENG_LOG("%s: s_sock_srv = 0x%x", __FUNCTION__, s_sock_srv);
            if (TCPIP_SOCKET_INVALID != s_sock_srv){
                //if (reconnect == 0){
                    addr.ip_addr = 0;
                    addr.family = AF_INET;
                    addr.port = (uint16)htons(ENGPC_SOCK_PORT);

                    ret = sci_sock_bind(s_sock_srv, (struct sci_sockaddr*)&addr, sizeof(addr));
                    ENG_LOG("%s: sock bind: %x bind to %s, error %d", __FUNCTION__, s_sock_srv, 
                                        TCPIPDEBUG_GetIpPortInfo((struct sockaddr*)&addr), sci_sock_errno(s_sock_srv));
                //}
                if (ret >= 0){
                    ENG_LOG("%s: sock listen start: s_sock_srv = 0x%x", __FUNCTION__, s_sock_srv);
                    ret = sci_sock_listen(s_sock_srv, 5);
                    ENG_LOG("%s: sock listen end: ret = %d, error %d", __FUNCTION__, ret, sci_sock_errno(s_sock_srv));
                    if (ret >= 0){
                        break;
                    }
                }
            }

            SCI_Sleep(1000);
            if (time-- < 0){
                ENG_LOG("%s fail: ",  __FUNCTION__);
                time = 60;
            }

        }while(1);

        chnl_pc_sock_diag_init();
        reconnect = 1;
   } while(TRUE);
}

extern int isConnectAp();
void chnl_sock_init(int type){
    int connAp = isConnectAp();
    int mode = POWER_GetResetMode();
#ifdef FT_WIFI_SUPPORT_CUSTOM_XTC
    if(connAp == 1 || type == CHNL_TYPE_DEBUG){
#else
    if((connAp == 1 && mode != NORMAL_MODE) || type == CHNL_TYPE_DEBUG){
#endif
        if( SCI_INVALID_BLOCK_ID == s_engpc_pc_sock_task )
        {
            s_engpc_pc_sock_task = SCI_CreateThread(
                "ENGPC_PC_SOCK_THREAD",
                "ENGPC_PC_SOCK_THREAD",
                chnl_sock_cb,
                NULL,
                NULL,
                P_ENGPC_PC_SOCK_STACK_SIZE,
                P_ENGPC_PC_SOCK_QUEUE_NUM,
                P_ENGPC_PC_SOCK_TASK_PRIORITY,
                SCI_PREEMPT,
                SCI_AUTO_START );
        }
    }else{
        ENG_LOG("connAp = %d, type = %d, mode = %d", connAp, type, mode);
        return ;
    }
}

#define checkisdigit(c) ((c) >= '0' && (c) <= '9')
#define PING_RETRY_TIMES        4   /* ping retry times */
LOCAL int32 s_ping_send_times = 0;
LOCAL int32 s_ping_recv_times = 0;
LOCAL char g_addr[100] = {0};
LOCAL int32 ping_times = 0;

PUBLIC int32 PingTestStart(
    char*           faddr_ptr,
    char           ip_type,
    uint32          ping_times,
    BOOLEAN         need_pdp,
    TCPIP_PING_CALLBACK_EX_FPTR    callback_fptr);

PUBLIC void ping_callback(
        int32 res,/* ping result, 0 - succeed; other - fail */
        uint32 time_delay,/* ping time delay, only valid when success, unit: ms */
        TCPIP_PING_HANDLE ping_handle,/* ping handle */
        uint8 ttl,
        char* ipaddr)
{
    uint8* resp_data_ptr = NULL;
    
    resp_data_ptr = SCI_ALLOC_APPZ(500);
    if (NULL == resp_data_ptr)
    {
        SCI_TRACE_LOW("ping_callback alloc faile");
    }
    SCI_TRACE_LOW("ping_callback res: %d, time_delay: %d, ping_handle: %d, ttl: %d, ipaddr: %s",
                res, time_delay, ping_handle, ttl, ipaddr);
    switch (res) {
        case PINGRES_SUCCESS:
            s_ping_send_times++;
            s_ping_recv_times++;
            if (resp_data_ptr)
            {
                snprintf(resp_data_ptr, 500, "%s %s %s %s%d %s%d\n","From",ipaddr, "response:", "time=", time_delay, "TTL=",ttl);
                SIO_ATC_WriteCmdRes(resp_data_ptr, strlen(resp_data_ptr));
            }
            break;
        case PINGRES_DNS_TIMEOUT:
            if (resp_data_ptr)
            {
                snprintf(resp_data_ptr, 500, "%s%s\n","Can not find the host: ",g_addr);
                SIO_ATC_WriteCmdRes(resp_data_ptr, strlen(resp_data_ptr));
            }
            break;
        case PINGRES_DNS_ERROR:
            if (resp_data_ptr)
            {
                snprintf(resp_data_ptr, 500, "%s%s\n","Can not find the host: ",g_addr);
                SIO_ATC_WriteCmdRes(resp_data_ptr, strlen(resp_data_ptr));
            }
            break;
        case PINGRES_ICMP_TIMEOUT:
            s_ping_send_times++;
            if (resp_data_ptr)
            {
                snprintf(resp_data_ptr, 500, "%s\n","Reqeust timeout");
                SIO_ATC_WriteCmdRes(resp_data_ptr, strlen(resp_data_ptr));
            }
            break;
        case PINGRES_ICMP_ERROR:
            if (resp_data_ptr)
            {
                snprintf(resp_data_ptr, 500, "%s\n","Reqeust error");
                SIO_ATC_WriteCmdRes(resp_data_ptr, strlen(resp_data_ptr));
            }
            break;
        default:
            if (resp_data_ptr)
            {
                snprintf(resp_data_ptr, 500, "%s\n","Other error");
                SIO_ATC_WriteCmdRes(resp_data_ptr, strlen(resp_data_ptr));
            }
            break;
    }

    if (s_ping_send_times >= ping_times || s_ping_send_times == 0)
    {
        snprintf(resp_data_ptr, 500, "    %s%d%s%d\n","Ping result infomation: send=",s_ping_send_times, ",receive=", s_ping_recv_times);
        SIO_ATC_WriteCmdRes(resp_data_ptr, strlen(resp_data_ptr));
        ping_times = 0;
        PingTestStop();
    }
    
    if (resp_data_ptr)
        SCI_FREE(resp_data_ptr);
}

/* cmd: at+ping="url",times,iptype */
/* at+ping="www.aipv6.com",3,1   //ipv6 */
/* at+ping="www.aipv4.com",3,0   //ipv4 */
int eng_at_testping_func(char *req, char *rsp)
{
    char times[4] = {0};
    char ip_type = 0;
    char* resp_data_ptr = NULL;
    int32 req_len = 0;
    char* ep = req;
    char* endp = NULL;
    int32 temp_len = 0;
    int i = 0;
    
    resp_data_ptr = SCI_ALLOC_APPZ(500);
    if (NULL == resp_data_ptr)
    {
        SCI_TRACE_LOW("eng_at_testping_func alloc faile");
        sprintf(rsp, "+TESTPING faile\n");
        return strlen(rsp);
    }
    
    if(NULL != req)
    {
        req_len = strlen(req);
        temp_len = strlen("at+ping=");
        SCI_TRACE_LOW("[ENG_LOG] line:%d, req: %s, req_len: %d", __LINE__, req, req_len);
        if (req_len < temp_len)
        {
            SCI_TRACE_LOW("[ENG_LOG] req is error ");
            sprintf(rsp, "at+ping faile\n");
            return strlen(rsp);
        }
        
        /* skip at+ping= */
        temp_len = strlen("at+ping=");
        ep = req + temp_len;
        req_len -= temp_len;
        if (req_len <= 0)
        {
            SCI_TRACE_LOW("[ENG_LOG] req is error ");
            sprintf(rsp, "at+ping faile\n");
            return strlen(rsp);
        }

        /* skip " */
        if (ep[i] != '"')
        {
            SCI_TRACE_LOW("[ENG_LOG] req is error ");
            sprintf(rsp, "at+ping faile\n");
            return strlen(rsp);
        }
        ep++;
        req_len--;

        /* get addr */
        SCI_TRACE_LOW("[ENG_LOG] line:%d, req: %s, req_len: %d", __LINE__, ep, req_len);
        memset(g_addr, 0, 100);
        for (i = 0; req_len > 0 && ep[i] != '"' && i < 50; i++)
        {
            g_addr[i] = ep[i];
            req_len --;
        }
        if (req_len <= 0 || ep[i] != '"')
        {
            SCI_TRACE_LOW("[ENG_LOG] req is error ");
            sprintf(rsp, "at+ping faile\n");
            return strlen(rsp);
        }
        /* skip ", */
        ep += (i + 2);
        req_len -= 2;
        
        /* get times */
        SCI_TRACE_LOW("[ENG_LOG] line:%d, req: %s, req_len: %d", __LINE__, ep, req_len);
        for (i = 0; req_len > 0 && ep[i] != ',' && i < 3; i++)
        {
            if (checkisdigit(ep[i]))
                times[i] = ep[i];
            req_len --;
        }
        if (req_len <= 0 || ep[i] != ',')
        {
            SCI_TRACE_LOW("[ENG_LOG] req is error ");
            sprintf(rsp, "at+ping faile\n");
            return strlen(rsp);
        }
        ping_times = strtoul(times, &endp, 0);
        if (ping_times <= 0)
            ping_times = PING_RETRY_TIMES;
        ep += (i + 1);
        req_len --;
        if (req_len <= 0)
        {
            SCI_TRACE_LOW("[ENG_LOG] req is error ");
            sprintf(rsp, "at+ping faile\n");
            return strlen(rsp);
        }
        
        /* get ip_type */
        SCI_TRACE_LOW("[ENG_LOG] line:%d, req: %s, req_len: %d", __LINE__, ep, req_len);
        if (ep[0] == '1')
            ip_type = 1;
        else
            ip_type = 0;
        
        SCI_TRACE_LOW("[ENG_LOG] addr: %s, times: %d, ip_type: %d",g_addr,ping_times,ip_type);
    }
    else
    {
        SCI_TRACE_LOW("[ENG_LOG] req is null ");
        sprintf(rsp, "+TESTPING faile\n");
        return strlen(rsp);
    }

    s_ping_send_times = 0;
    s_ping_recv_times = 0;

    if (0 == PingTestStart(g_addr, ip_type, ping_times, TRUE, ping_callback)) 
    {
        sprintf(rsp,"\n%s %s%s\n","Ping",g_addr,":");
        SCI_TRACE_LOW("[ENG_LOG]line: %d, rsp: %s ",__LINE__,rsp);
    }
    else
    {
        sprintf(rsp,"\n%s %s %s\n","Ping",g_addr, "faile");
        SCI_TRACE_LOW("[ENG_LOG]line: %d, rsp: %s ",__LINE__,rsp);
    }

    return strlen(rsp);
}

