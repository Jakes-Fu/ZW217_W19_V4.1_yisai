/****************************************************************************
** File Name:      socket_test.c                                           *
** Author:         qin.he                                                 *
** Date:           2022/10/1                                              *
** Copyright:      2022 uinsoc, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2022/10/1   qin.he          create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/

#include "socket_test.h"
#include "sci_api.h"


/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define SOCKET_TEST_TRACE(fmt, args ...)  SCI_TRACE_LOW("[socket test ] " fmt, ##args);

/**
 * This is for thread. 
 *  P_SOCKET_TASK_PRIORITY is for Temp.
*/
#define P_SOCKET_TEST_STACK_SIZE     (1024*16)
#define P_SOCKET_TEST_QUEUE_NUM      200   
#define P_SOCKET_TEST_TASK_PRIORITY  70  // PRI_DAPS_SNTP_IO_TASK //231

/**
  * Application layer recv buffer len of plaint text
*/
#define SOCKET_TEST_RECV_MAX  4096
#define SOCKET_TEST_RECV_PAD 16

#define SOCKET_TEST_DNS_TIMEOUT 20000

#define SOCKET_TEST_TIME   20//20S

#define TPS   10      /* see below note of TX_SLEEP_TIME */
#define cticks 	( SCI_GetTickCount() * TPS / 1000 )

/**-------------------------------------------------------------------------*
**                         STRUCTURE DEFINITION                             *
**--------------------------------------------------------------------------*/
typedef struct _socket_test_recorder
{    
    int                                       sock_handle;  //Output
    struct sci_sockaddr             sock_addr; //Output
    struct sci_sockaddr6           sock_addr6;
    BOOLEAN                            ServerIsIPv6; //TRUE: ipv6
    BOOLEAN                            IsLoopback;
    
    uint32                                 net_id;  //Output,Network Interface, come from PDP actived, we can get  IP/IP mask, DNS
    uint32                                 send_interval;

    int                                       server_sock_handle;
    int                                       server_acept_handle;
}socket_test_recorder_t;

/**-------------------------------------------------------------------------*
**                         STATIC VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/
LOCAL socket_test_recorder_t* g_socket_test_recorder_ptr=NULL;
LOCAL BLOCK_ID g_socket_client_task_id = SCI_INVALID_BLOCK_ID;
LOCAL BLOCK_ID g_socket_server_task_id = SCI_INVALID_BLOCK_ID;
LOCAL int tcp_max_seg = 0;
LOCAL uint8 b_is_async = TRUE;
LOCAL uint8 b_is_exit = FALSE;

CONST LOCAL char* g_socket_test_url_ptr =  "106.75.145.63";//"www.baidu.com";

/**
  *  Client HTTP Request Message
*/
CONST LOCAL char *g_socket_test_httpPacket = "GET HTTP/1.1\r\n"
             "Host: www.baidu.com\r\n"
             "Accept: */*\r\n"
             "Connection: keep-alive\r\n\r\n";

CONST LOCAL char * server_send= "Hello,Client \r\n";
CONST LOCAL char * client_send = "Hello,Server \r\n";
/**-------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :Create  a Task to handle SOCKET events 
//  Global resource dependence : 
//  Author: qin.he
//  Note: 
/*****************************************************************************/
LOCAL void socket_test_StartTask( void );


/**-------------------------------------------------------------------------*
**                         IMPLEMENTATION                                   *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : This is Socket API  
//         This API Just create DNS request.
//         The platform will send SOCKET_ASYNC_GETHOSTBYNAME_CNF to socket_entry Thread
//          after get DNS response. 
//  Global resource dependence : 
//  Author: qin.he
//  Note: 
/*****************************************************************************/
LOCAL int socket_test_GetHostByName(char *url_ptr,  uint32 net_id)
{
    struct sci_hostent  *hostent_ptr = PNULL;
    TCPIP_HOST_HANDLE handle;
        
    if (url_ptr == PNULL)
    {
        return -1;
    }
    
    handle = sci_async_gethostbyname(url_ptr, SCI_IdentifyThread(), SOCKET_TEST_DNS_TIMEOUT, net_id);
    if (0 == handle)
    {
        return -1;    
    }
    return 0;
}

/*****************************************************************************/
//  Description : This is Socket API  
//  Global resource dependence : 
//  Author: qin.he
//  Note: 
/*****************************************************************************/
LOCAL int socket_open(TCPIP_NETID_T net_id)
{
    int sock_handle = 0;
    int result;
    unsigned long nonBlockingFlag = 1;
    int sock_flag = 1; 
    int value = 1;

    //Create TCP socket fd by Server IP address type
    if (g_socket_test_recorder_ptr->ServerIsIPv6)
    {
        sock_handle = sci_sock_socket(AF_INET6, SOCK_STREAM, 0, net_id);
    }
    else
    {
        sock_handle = sci_sock_socket(AF_INET, SOCK_STREAM, 0, net_id);
    }
    
    if (-1 == sock_handle)
    {
        SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
        return -1;
    }

    result = sci_sock_setsockopt(sock_handle, TCP_NODELAY, &sock_flag);
    if (0 != result)
    {
        sci_sock_socketclose(sock_handle);
        sock_handle = 0;
        SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
        return -1;
    }

    sock_flag = 0;
    result = sci_sock_setsockopt(sock_handle, SO_TIMESTAMP, &sock_flag);
    if (0 != result)
    {
        sci_sock_socketclose(sock_handle);
        sock_handle = 0;
        SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
        return -1;
    }
    
    result = sci_sock_getsockopt(sock_handle, TCP_MAXSEG, &tcp_max_seg);
    if (0 != result)
    {
        sci_sock_socketclose(sock_handle);
        sock_handle = 0;
        SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
        return -1;
    }

    value = 1;
    result = sci_sock_setsockopt(sock_handle, SO_KEEPALIVE, &value);
    if (0 != result)
    {
        sci_sock_socketclose(sock_handle);
        sock_handle = 0;
        SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
        return -1;
    }

    value = 5;
    result = sci_sock_setsockopt(sock_handle, TCP_KEEPIDLE, &value);
    if (0 != result)
    {
        sci_sock_socketclose(sock_handle);
        sock_handle = 0;
        SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
        return -1;
    }

    value = 5;
    result = sci_sock_setsockopt(sock_handle, TCP_KEEPINTVL, &value);
    if (0 != result)
    {
        sci_sock_socketclose(sock_handle);
        sock_handle = 0;
        SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
        return -1;
    }

    value = 20;
    result = sci_sock_setsockopt(sock_handle, TCP_KEEPCNT, &value);
    if (0 != result)
    {
        sci_sock_socketclose(sock_handle);
        sock_handle = 0;
        SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
        return -1;
    }

    if (TRUE == b_is_async)
    {
        //HOT TIPS,  use async socket
        SOCKET_TEST_TRACE("[%s:%d] task_id: %x",__FUNCTION__,__LINE__,SCI_IdentifyThread());
        result = sci_sock_asyncselect(sock_handle, SCI_IdentifyThread(), AS_CONNECT|AS_READ|AS_WRITE|AS_CLOSE |AS_FULLCLOSED);
        if (0 != result)
        {
            sci_sock_socketclose(sock_handle);
            sock_handle = 0;
            SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
            return -1;
        }
        
        result = sci_sock_setsockopt(sock_handle, SO_NBIO, &nonBlockingFlag);
        if (0 != result)
        {
            sci_sock_socketclose(sock_handle);
            sock_handle = 0;
            SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
            return -1;
        }
    }

    SOCKET_TEST_TRACE("[%s:%d] , tcp_max_seg: %d\n",__FUNCTION__,__LINE__,tcp_max_seg);
    return sock_handle;
}

/*****************************************************************************/
//  Description : This is Socket API
//              The platform will send SOCKET_CONNECT_EVENT_IND to socket_entry Thread
//  Global resource dependence : 
//  Author: qin.he
//  Note: 
/*****************************************************************************/
LOCAL int socket_connect(int so, struct sci_sockaddrext* addr, int addrlen)
{
    int result = 0;

    SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
    if (so == -1 || addr == PNULL || addrlen == 0)
    {
        SOCKET_TEST_TRACE("[%s:%d], input error\n",__FUNCTION__,__LINE__);
        return -1;
    }
    result = sci_sock_connect(so, addr, addrlen);
    SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
    return result;
}

/*****************************************************************************/
//  Description : This is Socket API  
//  Global resource dependence : 
//  Author: qin.he
//  Note: 
/*****************************************************************************/
LOCAL int socket_send(int so, char* buf, int len)
{
    int result = 0;
    
    SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
    if (so == -1 || buf == NULL || len == 0)
    {
        SOCKET_TEST_TRACE("[%s:%d], input error\n",__FUNCTION__,__LINE__);
        return -1;
    }
    
    if (len > tcp_max_seg && tcp_max_seg != 0)
    {
        result = sci_sock_send(so, buf, tcp_max_seg, 0);
    }
    else
    {
        result = sci_sock_send(so, buf, len, 0);
    }
    return result;
}

/*****************************************************************************/
//  Description : This is Socket API  
//  Global resource dependence : 
//  Author: qin.he
//  Note: 
/*****************************************************************************/
LOCAL int socket_recv(int so, char* buf, int len)
{
    int result = 0;
    
    SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
    if (so == -1 || buf == NULL || len == 0)
    {
        SOCKET_TEST_TRACE("[%s:%d], input error\n",__FUNCTION__,__LINE__);
        return -1;
    }
    result = sci_sock_recv(so, buf, len, 0);
    SOCKET_TEST_TRACE("[%s:%d], recv len: %d\n",__FUNCTION__,__LINE__, result);
    return result;
}

/*****************************************************************************/
//  Description : This is Socket API
//              The platform will send SOCKET_FULL_CLOSED_EVENT_IND to socket_entry Thread
//  Global resource dependence : 
//  Author: qin.he
//  Note: 
/*****************************************************************************/
LOCAL int socket_close(int so)
{
    int result = 0;

    SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
    if (so == -1 || so == 0)
    {
        SOCKET_TEST_TRACE("[%s:%d], input error\n",__FUNCTION__,__LINE__);
        return -1;
    }

    result = sci_sock_socketclose(so);
    if (-1 == result)
    {
        return -1;
    }
    SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
    return result;
}

LOCAL void socket_server_entry(  uint32 argc, void* argv)
{
    struct sci_sockaddr bind_addr_client = {0};
    struct sci_sockaddr bind_addr_server = {0};

    int result_bind_server=0;
    int result_listen_server=0;
    int32  net_id_both;
    char buffer_server[100];
    int buff_len = 100;
    uint32	recv_server_length = 0;

   SOCKET_TEST_TRACE("[%s:%d]  enter",__FUNCTION__,__LINE__);
    net_id_both =	sci_getLoopBackNetid();
    bind_addr_server.family=AF_INET;
    bind_addr_server.port=htons(1998);
    bind_addr_server.ip_addr =inet_addr("127.0.0.1");
    bind_addr_client.family=AF_INET;
    bind_addr_client.port=htons(5000);
    bind_addr_client.ip_addr =inet_addr( "127.0.0.1");
    if(g_socket_test_recorder_ptr == NULL)
    {
        SOCKET_TEST_TRACE("[%s:%d] NULL ptr",__FUNCTION__,__LINE__);
        SCI_ThreadExit ();
    }

    g_socket_test_recorder_ptr->server_sock_handle = sci_sock_socket(AF_INET, SOCK_STREAM, 0, net_id_both);
    if (-1 == g_socket_test_recorder_ptr->server_sock_handle)
    {
        SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
    }
    result_bind_server=sci_sock_bind(g_socket_test_recorder_ptr->server_sock_handle, &bind_addr_server, sizeof(struct sci_sockaddr));

    if( -1==result_bind_server)
    {	
        SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
    }
    result_listen_server=sci_sock_listen(g_socket_test_recorder_ptr->server_sock_handle, 5);
    if( -1==result_listen_server)
    {// listen failed
        SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
    }
    g_socket_test_recorder_ptr->server_acept_handle = sci_sock_accept(g_socket_test_recorder_ptr->server_sock_handle,&bind_addr_client, sizeof(struct sci_sockaddr));
    if(g_socket_test_recorder_ptr->server_acept_handle==-1)
    {// accept failed
        SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
    }

    while(!b_is_exit && g_socket_test_recorder_ptr != NULL)
    {
        recv_server_length = sci_sock_recv(g_socket_test_recorder_ptr->server_acept_handle,buffer_server,buff_len,0);
        if(TCPIP_SOCKET_ERROR == recv_server_length)
        {//type of errpor
            int error = sci_sock_errno(g_socket_test_recorder_ptr->server_acept_handle);
            if( EWOULDBLOCK == error )
            {//
                SCI_Sleep( 1000 );
                SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
            }
            else
            {	//
                SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
                break;
            }
        }
        else if(recv_server_length>0)    //receive DATA success
        {
            LOCAL uint32	send_server_length = 0;

            SOCKET_TEST_TRACE("[%s:%d] socket test received data  is  %s\n",__FUNCTION__,__LINE__,buffer_server);
            send_server_length = sci_sock_send(g_socket_test_recorder_ptr->server_acept_handle,server_send,strlen(server_send),0);
            if( TCPIP_SOCKET_ERROR == send_server_length)
            {	
                int error = sci_sock_errno(g_socket_test_recorder_ptr->server_acept_handle);
                if( EWOULDBLOCK == error || ENOBUFS == error )
                {
                    SCI_Sleep( 1000);
                    SCI_TRACE_LOW("[%s:%d] SLEEPING",__FUNCTION__,__LINE__);
                }
                else
                {
                    SCI_TRACE_LOW("[%s:%d] THERE IS A BIG ERROR",__FUNCTION__,__LINE__);
                    break;
                }
            }
        }
    }
    SOCKET_TEST_TRACE("[%s:%d]  exit",__FUNCTION__,__LINE__);
    SCI_ThreadExit ();
}

/*****************************************************************************/
//  Description : Task entry function
//                                                  
//  Global resource dependence : 
//  Author: qin.he
//  Note: 
/*****************************************************************************/
LOCAL void socket_client_entry( uint32 argc, void* argv )
{
    xSignalHeaderRec *sig_ptr = PNULL;
    char* recvbuf = NULL;
    int   send_len = 0;
    unsigned int  total_len = 0;
    int result = 0;
    sci_fd_set write_fds;
    sci_fd_set read_fds;
    int recv_len = 0;
    unsigned char str_ptr[16] = {0};
    struct sci_sockaddrext *addr_ptr = NULL;
    int size = 0;
    ASYNC_GETHOSTBYNAME_CNF_SIG_T* dns = (ASYNC_GETHOSTBYNAME_CNF_SIG_T*)sig_ptr;
    struct sci_sockaddr bind_addr_client = {0};
    unsigned long tmo = 0;

    SOCKET_TEST_TRACE("[%s:%d]  enter",__FUNCTION__,__LINE__);
    if(g_socket_test_recorder_ptr == NULL)
    {
        SOCKET_TEST_TRACE("[%s:%d] NULL ptr",__FUNCTION__,__LINE__);
        goto  ERROR;
    }
    if( g_socket_test_recorder_ptr->IsLoopback )
    {
        SCI_SLEEP(2);
        bind_addr_client.family=AF_INET;
        bind_addr_client.port=htons(5000);
        bind_addr_client.ip_addr =inet_addr( "127.0.0.1");
        g_socket_test_recorder_ptr->ServerIsIPv6 = FALSE;
        SCI_MEMSET(&(g_socket_test_recorder_ptr->sock_addr), 0, sizeof(struct sci_sockaddr));
        g_socket_test_recorder_ptr->sock_addr.family = AF_INET; 
        g_socket_test_recorder_ptr->sock_addr.port = htons(1998); 
        g_socket_test_recorder_ptr->sock_addr.ip_addr = inet_addr( "127.0.0.1");
        addr_ptr = (struct sci_sockaddrext *)&(g_socket_test_recorder_ptr->sock_addr);
        size = sizeof( struct sci_sockaddr);

        g_socket_test_recorder_ptr->sock_handle = socket_open(g_socket_test_recorder_ptr->net_id);
        if (g_socket_test_recorder_ptr->sock_handle == -1)
        {
            SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
            goto ERROR;
        } 
        result=sci_sock_bind(g_socket_test_recorder_ptr->sock_handle, &bind_addr_client, sizeof(struct sci_sockaddr));
        if( -1==result)
        {
           SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
           goto ERROR;
        }
        result = socket_connect(g_socket_test_recorder_ptr->sock_handle,  (struct sci_sockaddrext *)addr_ptr,  size);
        if ((result < 0) && (TRUE != b_is_async))
        {
            SOCKET_TEST_TRACE(" [%s:%d] input error\n",__FUNCTION__,__LINE__);
            goto ERROR;
        }
        
        while ( !b_is_async && !b_is_exit && (tmo <= cticks) && (g_socket_test_recorder_ptr != NULL))
        {
            
            SCI_FD_ZERO(&write_fds);
            SCI_FD_ZERO(&read_fds);
            SCI_FD_SET(g_socket_test_recorder_ptr->sock_handle, &write_fds );
            SCI_FD_SET(g_socket_test_recorder_ptr->sock_handle, &read_fds );
            SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
            result = sci_sock_select(NULL, &write_fds, NULL, 100);
            if (result > 0)
            {
                send_len = socket_send(g_socket_test_recorder_ptr->sock_handle, client_send, strlen(client_send));
                tmo = cticks + (g_socket_test_recorder_ptr->send_interval * TPS);
                SOCKET_TEST_TRACE("[%s:%d] send_len: %d, send data: %s",__FUNCTION__,__LINE__,send_len,client_send);
            }

            result = sci_sock_select(&read_fds, NULL, NULL, 100);
            if (result > 0)
            {
                recv_len = 0;
                recvbuf = SCI_ALLOC(SOCKET_TEST_RECV_MAX+SOCKET_TEST_RECV_PAD);
                if(recvbuf == NULL)
                {
                    break;
                }
                SCI_MEMSET(recvbuf, 0,  SOCKET_TEST_RECV_MAX+SOCKET_TEST_RECV_PAD);
                recv_len = socket_recv( g_socket_test_recorder_ptr->sock_handle,  recvbuf, SOCKET_TEST_RECV_MAX);
                SOCKET_TEST_TRACE("[%s:%d] recv_len: %d, recv data: %s",__FUNCTION__,__LINE__,send_len,recvbuf);
                SCI_FREE(recvbuf);
                recvbuf=NULL;
                
            }
        }
    }
    else
    {
        socket_test_GetHostByName(g_socket_test_url_ptr, g_socket_test_recorder_ptr->net_id);
    }
    SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
    while(!b_is_exit && (g_socket_test_recorder_ptr != NULL))//( g_mmidemo_socket_task_id != SCI_INVALID_BLOCK_ID )/*lint !e716*/
    {
        // Get sig From Queue
        SCI_TRACE_LOW("qin.he add debug[%s:%d] task_id:%x",__FUNCTION__,__LINE__,SCI_IdentifyThread());
        sig_ptr = SCI_GetSignal( SCI_IdentifyThread());   
        SOCKET_TEST_TRACE(" [%s:%d] get sig_ptr: %p\n",__FUNCTION__,__LINE__);
        // Process sig
        switch( sig_ptr->SignalCode )
        {
            case SOCKET_ASYNC_GETHOSTBYNAME_CNF:
            {    
                //Step3: Name - TCP connect
                SOCKET_TEST_TRACE("[%s:%d] SOCKET_ASYNC_GETHOSTBYNAME_CNF\n",__FUNCTION__,__LINE__);
                dns = (ASYNC_GETHOSTBYNAME_CNF_SIG_T*)sig_ptr;
                if (dns->error_code != 0)
                {
                    SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
                    goto ERROR;
                }

                //Priority access server's IPv4 address
                if (dns->hostent.h_cntv4 > 0)
                {
                    g_socket_test_recorder_ptr->ServerIsIPv6 = FALSE;
                    SCI_MEMSET(&(g_socket_test_recorder_ptr->sock_addr), 0, sizeof(struct sci_sockaddr));
                    g_socket_test_recorder_ptr->sock_addr.family = AF_INET; 
                    g_socket_test_recorder_ptr->sock_addr.port = htons(80); 
                    SCI_MEMCPY(&(g_socket_test_recorder_ptr->sock_addr.ip_addr), dns->hostent.h_addr_list[0], sizeof(g_socket_test_recorder_ptr->sock_addr.ip_addr));
                    addr_ptr = (struct sci_sockaddrext *)&(g_socket_test_recorder_ptr->sock_addr);
                    size = sizeof( struct sci_sockaddr);
                }
                else 
                {
                    if (dns->hostent.h_cntv6 > 0)
                    {
                        g_socket_test_recorder_ptr->ServerIsIPv6 = TRUE;
                        SCI_MEMSET(&(g_socket_test_recorder_ptr->sock_addr6), 0, sizeof(struct sci_sockaddr6));
                        g_socket_test_recorder_ptr->sock_addr6.sin6_family = AF_INET6;
                        g_socket_test_recorder_ptr->sock_addr6.sin6_port = htons(80);//htons(443);
                        SCI_MEMCPY(&(g_socket_test_recorder_ptr->sock_addr6.sin6_addr), dns->hostent.h_addr6_list[0], sizeof(g_socket_test_recorder_ptr->sock_addr6.sin6_addr));
                        addr_ptr = (struct sci_sockaddrext *)&(g_socket_test_recorder_ptr->sock_addr6);
                        size = sizeof( struct sci_sockaddr6);
                    }
                    else
                    {
                        SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
                        goto ERROR;
                    }
                }

                SOCKET_TEST_TRACE("[%s:%d]: TCP connect", __FUNCTION__,__LINE__);
                //Creater  TCP socket 
                g_socket_test_recorder_ptr->sock_handle = socket_open(g_socket_test_recorder_ptr->net_id);
                if (g_socket_test_recorder_ptr->sock_handle == -1)
                {
                    SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
                    goto ERROR;
                } 

                //TCP  connect
                result = socket_connect(g_socket_test_recorder_ptr->sock_handle,  (struct sci_sockaddrext *)addr_ptr,  size);
                if ((result < 0) && (TRUE != b_is_async))
                {
                    SOCKET_TEST_TRACE(" [%s:%d] input error\n",__FUNCTION__,__LINE__);
                    goto ERROR;
                }

                while ( b_is_async && !b_is_exit && (tmo <= cticks) )
                {
                    
                    SCI_FD_ZERO(&write_fds);
                    SCI_FD_ZERO(&read_fds);
                    SCI_FD_SET(g_socket_test_recorder_ptr->sock_handle, &write_fds );
                    SCI_FD_SET(g_socket_test_recorder_ptr->sock_handle, &read_fds );

                    SOCKET_TEST_TRACE("[%s:%d]: send data",__FUNCTION__,__LINE__);
                    result = sci_sock_select(NULL, &write_fds, NULL, 100);
                    if (result > 0)
                    {
                        if( g_socket_test_recorder_ptr->IsLoopback )
                        {
                            send_len = socket_send(g_socket_test_recorder_ptr->sock_handle, client_send, strlen(client_send));
                        }
                        else
                        {
                            send_len = socket_send(g_socket_test_recorder_ptr->sock_handle, g_socket_test_httpPacket, strlen(g_socket_test_httpPacket));
                        }
                        tmo = cticks + (g_socket_test_recorder_ptr->send_interval * TPS);
                        SOCKET_TEST_TRACE("[%s:%d] send_len: %d, send data: %s",__FUNCTION__,__LINE__,send_len,client_send);
                    }

                    SOCKET_TEST_TRACE("[%s:%d]: recvd data",__FUNCTION__,__LINE__);
                    result = sci_sock_select(&read_fds, NULL, NULL, 100);
                    if (result > 0)
                    {
                        recv_len = 0;
                        recvbuf = SCI_ALLOC(SOCKET_TEST_RECV_MAX+SOCKET_TEST_RECV_PAD);
                        if(recvbuf == NULL)
                        {
                            break;
                        }
                        SCI_MEMSET(recvbuf, 0,  SOCKET_TEST_RECV_MAX+SOCKET_TEST_RECV_PAD);
                        recv_len = socket_recv( g_socket_test_recorder_ptr->sock_handle,  recvbuf, SOCKET_TEST_RECV_MAX);
                        SOCKET_TEST_TRACE("[%s:%d] recv_len: %d, recv data: %s",__FUNCTION__,__LINE__,send_len,recvbuf);
                        SCI_FREE(recvbuf);
                        recvbuf= NULL;
                    }
                }
            }
            break;

            case SOCKET_CONNECT_EVENT_IND:
            case SOCKET_WRITE_EVENT_IND:
            { 
                SOCKET_TEST_TRACE("[%s:%d] SOCKET_CONNECT_EVENT_IND/SOCKET_WRITE_EVENT_IND",__FUNCTION__,__LINE__);
                if( g_socket_test_recorder_ptr->IsLoopback )
                {
                    send_len = socket_send(g_socket_test_recorder_ptr->sock_handle, client_send, strlen(client_send));
                }
                else
                {
                    send_len = socket_send(g_socket_test_recorder_ptr->sock_handle, g_socket_test_httpPacket, strlen(g_socket_test_httpPacket));
                }
                tmo = cticks + (g_socket_test_recorder_ptr->send_interval * TPS);
                SOCKET_TEST_TRACE("[%s:%d] send_len: %d, send data: %s",__FUNCTION__,__LINE__,send_len,client_send);
            }
            break;

            case SOCKET_READ_EVENT_IND:
            case SOCKET_READ_BUFFER_STATUS_EVENT_IND:
            {     
                int recv_len = 0;
                SOCKET_TEST_TRACE("[%s:%d]SOCKET_READ_EVENT_IND",__FUNCTION__,__LINE__); 
                recvbuf = SCI_ALLOC(SOCKET_TEST_RECV_MAX+SOCKET_TEST_RECV_PAD);
                if(recvbuf == NULL)
                {
                    break;
                }
                SCI_MEMSET(recvbuf, 0,  SOCKET_TEST_RECV_MAX + SOCKET_TEST_RECV_PAD);
                recv_len = socket_recv( g_socket_test_recorder_ptr->sock_handle,  recvbuf, SOCKET_TEST_RECV_MAX);
                if ( recv_len == -1 )        // error occured 
                {
                     break;
                }
                else if ( recv_len == 0 )    // socket connection is broken
                {
                    break;
                }
                else
                {
                    SOCKET_TEST_TRACE("[%s:%d] recv_len: %d, recv data: %s",__FUNCTION__,__LINE__,send_len,recvbuf);
                    SCI_FREE(recvbuf);
                    recvbuf = NULL;
                    SCI_SLEEP(g_socket_test_recorder_ptr->send_interval*1000);
                     if( g_socket_test_recorder_ptr->IsLoopback )
                    {
                        send_len = socket_send(g_socket_test_recorder_ptr->sock_handle, client_send, strlen(client_send));
                    }
                    else
                    {
                        send_len = socket_send(g_socket_test_recorder_ptr->sock_handle, g_socket_test_httpPacket, strlen(g_socket_test_httpPacket));
                    }
                    tmo = cticks + (g_socket_test_recorder_ptr->send_interval * TPS);
                    SOCKET_TEST_TRACE("[%s:%d] send_len: %d, send data: %s",__FUNCTION__,__LINE__,send_len,client_send);
                }
                break;
            }

            case SOCKET_CONNECTION_CLOSE_EVENT_IND:
            case SOCKET_FULL_CLOSED_EVENT_IND:
                SOCKET_TEST_TRACE("[%s:%d]SOCKET_CONNECTION_CLOSE_EVENT_IND/SOCKET_FULL_CLOSED_EVENT_IND",__FUNCTION__,__LINE__); 
                goto  ERROR;
            default:
                break;
        } 
        if (sig_ptr != PNULL)
        {
            SOCKET_TEST_TRACE(" [%s:%d] free sig_ptr: %p\n",__FUNCTION__,__LINE__);
            SCI_FREE( sig_ptr );
            sig_ptr = PNULL;
        }
    } // while(1)
ERROR:
    if (sig_ptr != PNULL)
    {
        SOCKET_TEST_TRACE(" [%s:%d] free sig_ptr: %p\n",__FUNCTION__,__LINE__);
        SCI_FREE( sig_ptr );
        sig_ptr = PNULL;
    } 
    
    if(g_socket_test_recorder_ptr != NULL)
    {
        SCI_FREE(g_socket_test_recorder_ptr);
        g_socket_test_recorder_ptr = NULL;
    }

    SOCKET_TEST_TRACE("[%s:%d]  exit",__FUNCTION__,__LINE__);
    g_socket_client_task_id = SCI_INVALID_BLOCK_ID;
    SCI_ThreadExit();
}

/*****************************************************************************/
//  Description : Task function
//                                                  
//  Global resource dependence : 
//  Author: ak.wang
//  Note: 
/*****************************************************************************/
LOCAL void socket_test_StartTask( void )
{
    if( SCI_INVALID_BLOCK_ID != g_socket_client_task_id )
    {
        // task has already started 
        SOCKET_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
        return;
    }
    if(g_socket_test_recorder_ptr == NULL)
    {
        SOCKET_TEST_TRACE("[%s:%d] NUL PRT",__FUNCTION__,__LINE__);
        return;
    }

    b_is_exit = FALSE;
    if( g_socket_test_recorder_ptr->IsLoopback )
    {
        g_socket_server_task_id = SCI_CreateThread(
        "T_P_socket_server",
        "Q_P_socket_server",
        socket_server_entry,
        0,
        NULL,
        P_SOCKET_TEST_STACK_SIZE,
        P_SOCKET_TEST_QUEUE_NUM,
        P_SOCKET_TEST_TASK_PRIORITY,
        SCI_PREEMPT,
        SCI_AUTO_START );
    }
    g_socket_client_task_id = SCI_CreateThread(
        "T_P_socket_client",
        "Q_P_socket_client",
        socket_client_entry,
        0,
        NULL,
        P_SOCKET_TEST_STACK_SIZE,
        P_SOCKET_TEST_QUEUE_NUM,
        P_SOCKET_TEST_TASK_PRIORITY,
        SCI_PREEMPT,
        SCI_AUTO_START );


    return;
}

/*****************************************************************************/
//  Description :  main function
//  Global resource dependence : 
//  Author: qin.he
//  Note: 
/*****************************************************************************/
PUBLIC void socket_test_main(uint8 is_async, uint32 netid, uint32 interval)
{
    b_is_async = is_async;
    if(g_socket_test_recorder_ptr == NULL)
    {
        g_socket_test_recorder_ptr=SCI_ALLOC(sizeof(socket_test_recorder_t));
    }
    if(g_socket_test_recorder_ptr == NULL)
    {
        SOCKET_TEST_TRACE("[%s:%d] alloc fail",__FUNCTION__,__LINE__);
        return;
    }

    memset(g_socket_test_recorder_ptr, 0, sizeof(socket_test_recorder_t));
    
    g_socket_test_recorder_ptr->ServerIsIPv6 = FALSE;
    g_socket_test_recorder_ptr->sock_handle = -1;
    g_socket_test_recorder_ptr->server_acept_handle = -1;
    g_socket_test_recorder_ptr->server_sock_handle = -1;
    g_socket_test_recorder_ptr->send_interval = interval;
    if(netid == 0xffffffff)
    {
        g_socket_test_recorder_ptr->net_id = sci_getLoopBackNetid();
        g_socket_test_recorder_ptr->IsLoopback = TRUE;
    }
    else
    {
        g_socket_test_recorder_ptr->net_id = netid;
        g_socket_test_recorder_ptr->IsLoopback = FALSE;
    }
    SOCKET_TEST_TRACE("[%s:%d] net_id:0x%x, 0x%x, send_interval: %d",__FUNCTION__,__LINE__, netid,g_socket_test_recorder_ptr->net_id,interval);
    socket_test_StartTask();
}

/*****************************************************************************/
//  Description :  exit function
//  Global resource dependence : 
//  Author: qin.he
//  Note: 
/*****************************************************************************/
PUBLIC int socket_test_exit(void)
{
    SOCKET_TEST_TRACE("socket_test_exit enter");
    socket_close(g_socket_test_recorder_ptr->sock_handle);
    socket_close(g_socket_test_recorder_ptr->server_acept_handle);
    socket_close(g_socket_test_recorder_ptr->server_sock_handle);
    g_socket_client_task_id = SCI_INVALID_BLOCK_ID;
    g_socket_server_task_id = SCI_INVALID_BLOCK_ID;
    g_socket_test_recorder_ptr->ServerIsIPv6 = FALSE;
    g_socket_test_recorder_ptr->sock_handle = -1;
    g_socket_test_recorder_ptr->server_acept_handle = -1;
    g_socket_test_recorder_ptr->server_sock_handle = -1;
    SOCKET_TEST_TRACE("socket_test_exit exit");
    return 0;
}


