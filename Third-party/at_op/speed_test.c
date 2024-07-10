/****************************************************************************
** File Name:      speed_test.c                                           *
** Author:         qin.he                                                 *
** Date:           2021/10/22                                              *
** Copyright:      2021 uinsoc, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2021/10/22  qin.he          create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/

#include "speed_test.h"
#include "sci_api.h"


/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define SPEED_TEST_TRACE(fmt, args ...)  SCI_TRACE_LOW("[speed test] " fmt, ##args);

#define P_SPEED_TEST_STACK_SIZE     (1024*16)
#define P_SPEED_TEST_QUEUE_NUM      15    
#define P_SPEED_TEST_TASK_PRIORITY  231  // PRI_DAPS_SNTP_IO_TASK //231

/**
  * Application layer recv buffer len of plaint text
*/
#define SPEED_TEST_RECV_MAX  4096
#define SPEED_TEST_RECV_PAD 16

#define SPEED_TEST_DNS_TIMEOUT 20000

#define SPEED_TEST_TIME   20//20S

/**-------------------------------------------------------------------------*
**                         STRUCTURE DEFINITION                             *
**--------------------------------------------------------------------------*/
typedef struct _speed_test_recorder
{
    uint32                           app_moduleId;  //Input: reference mmi_res_prj_def.h,
    
    int                                sock_handle;  //Output
    struct sci_sockaddr         sock_addr; //Output
    struct sci_sockaddr6       sock_addr6;
    BOOLEAN                       ServerIsIPv6; //TRUE: ipv6
    
    uint32                           net_id;  //Output,Network Interface, come from PDP actived, we can get  IP/IP mask, DNS
    uint32                           timeout;
}speed_test_recorder_t;

/**-------------------------------------------------------------------------*
**                         STATIC VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/
LOCAL speed_test_recorder_t g_speed_test_recorder;
LOCAL BLOCK_ID g_speed_test_task_id = SCI_INVALID_BLOCK_ID;
LOCAL int tcp_max_seg = 0;
LOCAL uint8 b_is_async = TRUE;
LOCAL uint8 b_is_exit = FALSE;

/**
 *   Web server URL
*/
#if 1
CONST LOCAL char* g_speed_test_url_ptr = "106.75.145.63";

/**
  *  Client HTTP Request Message
*/
CONST LOCAL char *g_speed_test_httpPacket = "POST /smartwatchmgmt/qiniu/uploadFile HTTP/1.1\r\n"
             "Host: watchmgmt.module.okii.com\r\n" \
             "Content-Type: multipart/form-data; boundary=--------------------------4456822111001448549885455\r\n"
             "Content-Length: 314572800\r\n"
             "Connection: keep-alive\r\n\r\n"
             "--------------------------4456822111001448549885455\r\n"
             "Content-Disposition: form-data; name=\"file\"; filename=\"D:\\upload_speed_test_file.tmp\"\r\n"
             "Content-Type: application/octet-stream\r\n\r\n"
             "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
             "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
             "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
             "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
             "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
             "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
             "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
             "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
             "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
             "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  ";
#else
CONST LOCAL char* g_speed_test_url_ptr = "58.251.89.163";


/**
  *  Client HTTP Request Message
*/
CONST LOCAL char *g_speed_test_httpPacket = "POST / HTTP/1.1\r\n"
             "Host: upload.qiniup.com\r\n" \
             "Content-Type: multipart/form-data; boundary=--------------------------4456822111001448549885455\r\n"
             "Content-Length: 314572800\r\n"
             "Connection: keep-alive\r\n\r\n"
             "--------------------------4456822111001448549885455\r\n"
             "Content-Disposition: form-data; name=\"file\"; filename=\"D:\\upload_speed_test_file.tmp\"\r\n"
             "Content-Type: application/octet-stream\r\n\r\n"
             "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
             "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
             "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
             "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
             "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
             "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
             "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
             "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
             "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
             "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  ";
#endif

CONST LOCAL char *g_speed_test_httpBody = 
    "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
    "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
    "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
    "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
    "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
    "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
    "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
    "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
    "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
    "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
    "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
    "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
    "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  "
    "HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  HelloWord  ";

/**-------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :Create  a Task to handle SOCKET events 
//  Global resource dependence : 
//  Author: qin.he
//  Note: 
/*****************************************************************************/
LOCAL void speed_test_StartTask( void );


/**-------------------------------------------------------------------------*
**                         IMPLEMENTATION                                   *
**--------------------------------------------------------------------------*/

LOCAL int speed_test_GetHostByName(char *url_ptr,  uint32 net_id)
{
    struct sci_hostent  *hostent_ptr = PNULL;
    TCPIP_HOST_HANDLE handle;
        
    SPEED_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);

    if (url_ptr == PNULL)
    {
        SPEED_TEST_TRACE("[%s:%d] url is null",__FUNCTION__,__LINE__);
        return -1;
    }
    
    SPEED_TEST_TRACE("[%s:%d] url_ptr: %s",__FUNCTION__,__LINE__,url_ptr);
    handle = sci_async_gethostbyname(url_ptr, SCI_IdentifyThread(), SPEED_TEST_DNS_TIMEOUT, net_id);
    if (0 == handle)
    {
        return -1;    
    }
    SPEED_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
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
    int keepalive = 1;

    SPEED_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);

    if (g_speed_test_recorder.ServerIsIPv6)
    {
        sock_handle = sci_sock_socket(AF_INET6, SOCK_STREAM, 0, net_id);
    }
    else
    {
        sock_handle = sci_sock_socket(AF_INET, SOCK_STREAM, 0, net_id);
    }
    
    if (-1 == sock_handle)
    {
        SPEED_TEST_TRACE("[%s:%d] create socket error",__FUNCTION__,__LINE__);
        return -1;
    }

    result = sci_sock_setsockopt(sock_handle, TCP_NODELAY, &sock_flag);
    if (0 != result)
    {
        sci_sock_socketclose(sock_handle);
        sock_handle = 0;
        SPEED_TEST_TRACE("[%s:%d] set opt error",__FUNCTION__,__LINE__);
        return -1;
    }

    sock_flag = 0;
    result = sci_sock_setsockopt(sock_handle, SO_TIMESTAMP, &sock_flag);
    if (0 != result)
    {
        sci_sock_socketclose(sock_handle);
        sock_handle = 0;
        SPEED_TEST_TRACE("[%s:%d] set opt error",__FUNCTION__,__LINE__);
        return -1;
    }
    
    result = sci_sock_getsockopt(sock_handle, TCP_MAXSEG, &tcp_max_seg);
    if (0 != result)
    {
        sci_sock_socketclose(sock_handle);
        sock_handle = 0;
        SPEED_TEST_TRACE("[%s:%d] set opt error",__FUNCTION__,__LINE__);
        return -1;
    }

     result = sci_sock_getsockopt(sock_handle, SO_KEEPALIVE, &keepalive);
    if (0 != result)
    {
        sci_sock_socketclose(sock_handle);
        sock_handle = 0;
        SPEED_TEST_TRACE("[%s:%d] set opt error",__FUNCTION__,__LINE__);
        return -1;
    }
    

    if (TRUE == b_is_async)
    {
        //HOT TIPS,  use async socket
        result = sci_sock_asyncselect(sock_handle, SCI_IdentifyThread(), AS_CONNECT|AS_READ|AS_WRITE|AS_CLOSE |AS_FULLCLOSED);
        if (0 != result)
        {
            sci_sock_socketclose(sock_handle);
            sock_handle = 0;
            SPEED_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
            return -1;
        }
        
        result = sci_sock_setsockopt(sock_handle, SO_NBIO, &nonBlockingFlag);
        if (0 != result)
        {
            sci_sock_socketclose(sock_handle);
            sock_handle = 0;
            SPEED_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
            return -1;
        }
    }

    SPEED_TEST_TRACE("[%s:%d] tcp_max_seg: %d",__FUNCTION__,__LINE__,tcp_max_seg);
    return sock_handle;
}

/*****************************************************************************/
//  Description : This is Socket API
//  Global resource dependence : 
//  Author: qin.he
//  Note: 
/*****************************************************************************/
LOCAL int socket_connect(int so, struct sci_sockaddrext* addr, int addrlen)
{
    int result = 0;

    SPEED_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
    if (so == -1 || addr == PNULL || addrlen == 0)
    {
        SPEED_TEST_TRACE("[%s:%d] input error",__FUNCTION__,__LINE__);
        return -1;
    }
    result = sci_sock_connect(so, addr, addrlen);
    SPEED_TEST_TRACE("[%s:%d] result: %d",__FUNCTION__,__LINE__,result);
    
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
    
    SPEED_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
    if (so == -1 || buf == NULL || len == 0)
    {
        SPEED_TEST_TRACE("[%s:%d] input error",__FUNCTION__,__LINE__);
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

    SPEED_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
    if (so == -1 || buf == NULL || len == 0)
    {
        SPEED_TEST_TRACE("[%s:%d] input error",__FUNCTION__,__LINE__);
        return -1;
    }
    result = sci_sock_recv(so, buf, len, 0);
    SPEED_TEST_TRACE("[%s:%d] result: %d",__FUNCTION__,__LINE__,result);
    
    return result;
}

/*****************************************************************************/
//  Description : This is Socket API
//  Global resource dependence : 
//  Author: qin.he
//  Note: 
/*****************************************************************************/
LOCAL int socket_close(int so)
{
    int result = 0;

    SPEED_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
    if (so == -1)
    {
        SPEED_TEST_TRACE("[%s:%d] input error",__FUNCTION__,__LINE__);
        return -1;
    }

    result = sci_sock_socketclose(so);
    SPEED_TEST_TRACE("[%s:%d] result: %d",__FUNCTION__,__LINE__,result);
    if (-1 == result)
    {
        return -1;
    }

    return result;
}

/*****************************************************************************/
//  Description : Task entry function
//                                                  
//  Global resource dependence : 
//  Author: qin.he
//  Note: 
/*****************************************************************************/
#define TPS   10      /* see below note of TX_SLEEP_TIME */
#define cticks 	( SCI_GetTickCount() * TPS / 1000 )
LOCAL void speed_test_entry( uint32 argc, void* argv )
{
    xSignalHeaderRec *sig_ptr = PNULL;
    int   send_len = 0;
    unsigned int  total_len = 0;
    unsigned long tmo = 0;
    int result = 0;
    uint8  is_first_pack = TRUE;

    SPEED_TEST_TRACE("[%s:%d]  step1: dns parser",__FUNCTION__,__LINE__);
    speed_test_GetHostByName(g_speed_test_url_ptr, g_speed_test_recorder.net_id);
    
    while(!b_is_exit)//( g_mmidemo_socket_task_id != SCI_INVALID_BLOCK_ID )/*lint !e716*/
    {
        SPEED_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
        // Get sig From Queue
        sig_ptr = SCI_GetSignal( SCI_IdentifyThread());   
        if (sig_ptr == PNULL)
        {
            SPEED_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
            break;
        }

        // Process sig
        switch( sig_ptr->SignalCode )
        {
            case SOCKET_ASYNC_GETHOSTBYNAME_CNF:
            {    
                //Step3: Name - TCP connect
                unsigned char str_ptr[16] = {0};
                struct sci_sockaddrext *addr_ptr = NULL;
                int size = 0;
                ASYNC_GETHOSTBYNAME_CNF_SIG_T* dns = (ASYNC_GETHOSTBYNAME_CNF_SIG_T*)sig_ptr;

                SPEED_TEST_TRACE("SOCKET_ASYNC_GETHOSTBYNAME_CNF");
                if (dns->error_code != 0)
                {
                    goto ERROR;
                }


                if (dns->hostent.h_cntv4 > 0)
                {
                    g_speed_test_recorder.ServerIsIPv6 = FALSE;
                    SCI_MEMSET(&(g_speed_test_recorder.sock_addr), 0, sizeof(struct sci_sockaddr));
                    g_speed_test_recorder.sock_addr.family = AF_INET; 
                    g_speed_test_recorder.sock_addr.port = htons(80); 
                    SCI_MEMCPY(&(g_speed_test_recorder.sock_addr.ip_addr), dns->hostent.h_addr_list[0], sizeof(g_speed_test_recorder.sock_addr.ip_addr));
                    addr_ptr = (struct sci_sockaddrext *)&(g_speed_test_recorder.sock_addr);
                    size = sizeof( struct sci_sockaddr);
                }
                else 
                {
                    if (dns->hostent.h_cntv6 > 0)
                    {
                        g_speed_test_recorder.ServerIsIPv6 = TRUE;
                        SCI_MEMSET(&(g_speed_test_recorder.sock_addr6), 0, sizeof(struct sci_sockaddr6));
                        g_speed_test_recorder.sock_addr6.sin6_family = AF_INET6;
                        g_speed_test_recorder.sock_addr6.sin6_port = htons(80);//htons(443);
                        SCI_MEMCPY(&(g_speed_test_recorder.sock_addr6.sin6_addr), dns->hostent.h_addr6_list[0], sizeof(g_speed_test_recorder.sock_addr6.sin6_addr));
                        addr_ptr = (struct sci_sockaddrext *)&(g_speed_test_recorder.sock_addr6);
                        size = sizeof( struct sci_sockaddr6);
                    }
                    else
                    {
                        SCI_MEMSET(&g_speed_test_recorder, 0, sizeof(speed_test_recorder_t));
                        g_speed_test_task_id = SCI_INVALID_BLOCK_ID;
                        SCI_ThreadExit() ;
                    }
                }

                SPEED_TEST_TRACE("[%s:%d]  step2: connect",__FUNCTION__,__LINE__);
                //Creater  TCP socket 
                g_speed_test_recorder.sock_handle = socket_open(g_speed_test_recorder.net_id);
                if (g_speed_test_recorder.sock_handle == -1)
                {
                    SPEED_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
                    break;
                } 

                //TCP  connect
                result = socket_connect(g_speed_test_recorder.sock_handle,  (struct sci_sockaddrext *)addr_ptr,  size);
                if ((result < 0) && (TRUE != b_is_async))
                {
                    SPEED_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
                    socket_close(g_speed_test_recorder.sock_handle);
                    goto ERROR;
                }

                if (TRUE != b_is_async)
                {
                    sci_fd_set write_fds;

                    is_first_pack = TRUE;

                    SPEED_TEST_TRACE("[%s:%d]  step3: send data",__FUNCTION__,__LINE__);
                    SCI_FD_ZERO(&write_fds);
                    SCI_FD_SET(g_speed_test_recorder.sock_handle, &write_fds );

                    tmo = cticks + (g_speed_test_recorder.timeout * TPS);
                    while (tmo > cticks)
                    {
                        result = sci_sock_select(NULL, &write_fds, NULL, 100);
                        if (result > 0)
                        {
                            if (is_first_pack)
                            {
                                send_len = socket_send(g_speed_test_recorder.sock_handle, g_speed_test_httpPacket, strlen(g_speed_test_httpPacket));
                                total_len += send_len;
                                is_first_pack = FALSE;
                                SPEED_TEST_TRACE(" total_len: %d, send_len: %d",total_len,send_len);
                            }
                            else
                            {
                                send_len = socket_send(g_speed_test_recorder.sock_handle, g_speed_test_httpBody, strlen(g_speed_test_httpBody));
                                total_len += send_len;
                                SPEED_TEST_TRACE(" total_len: %d, send_len: %d",total_len,send_len);
                            }
                        }
                    }

                    SPEED_TEST_TRACE("total_len: %d, speed: %d",total_len, total_len/(g_speed_test_recorder.timeout));
                    SPEED_TEST_TRACE("[%s:%d]  step5: close socket",__FUNCTION__,__LINE__);
                    socket_close(g_speed_test_recorder.sock_handle);
                    //SCI_PASSERT (0, ("line: %d, total_len: %d, speed: %d", __LINE__,total_len, total_len/(10)));
                    goto ERROR;
                }
                break;
            }

            case SOCKET_CONNECT_EVENT_IND:
            { 
                //Step4: Name - Application data Send
                SPEED_TEST_TRACE("SOCKET_CONNECT_EVENT_IND");
                SPEED_TEST_TRACE("[%s:%d]  step3: send data",__FUNCTION__,__LINE__);
                tmo = cticks + (g_speed_test_recorder.timeout * TPS);
                send_len = socket_send(g_speed_test_recorder.sock_handle, g_speed_test_httpPacket, strlen(g_speed_test_httpPacket));
                total_len += send_len;
                SPEED_TEST_TRACE(" total_len: %d, send_len: %d",total_len,send_len);
                while((tmo > cticks) 
                  && ((send_len = socket_send(g_speed_test_recorder.sock_handle, g_speed_test_httpBody, strlen(g_speed_test_httpBody))) > 0))
                {
                    total_len += send_len;
                    SPEED_TEST_TRACE(" total_len: %d, send_len: %d",total_len,send_len);
                }
                break;
            }
                
            case SOCKET_WRITE_EVENT_IND:
            {
                SPEED_TEST_TRACE("SOCKET_WRITE_EVENT_IND");
                while((tmo > cticks) 
                    && ((send_len = socket_send(g_speed_test_recorder.sock_handle, g_speed_test_httpBody, strlen(g_speed_test_httpBody))) > 0))
                {
                    total_len += send_len;
                    SPEED_TEST_TRACE(" total_len: %d, send_len: %d",total_len,send_len);
                }
                if (tmo <= cticks)
                {
                    SPEED_TEST_TRACE("total_len: %d, speed: %d",total_len, total_len/(g_speed_test_recorder.timeout));
                    SPEED_TEST_TRACE("[%s:%d]  step5: close socket",__FUNCTION__,__LINE__);
                    socket_close(g_speed_test_recorder.sock_handle);
                    //SCI_PASSERT (0, ("line: %d, total_len: %d, speed: %d", __LINE__,total_len, total_len/(MMIDEMO_TEST_TIME)));
                    goto ERROR;
                }
                break;
            }
            
            case SOCKET_READ_EVENT_IND:
            {     
                int recv_len = 0;
                char* recvbuf = NULL;

                SPEED_TEST_TRACE("SOCKET_READ_EVENT_IND");
                while (1)
                {
                    recv_len = 0;
                    recvbuf = SCI_ALLOC(SPEED_TEST_RECV_MAX+SPEED_TEST_RECV_PAD);
                    if(recvbuf == NULL)
                    {
                        break;
                    }
                    SCI_MEMSET(recvbuf, 0,  SPEED_TEST_RECV_MAX+SPEED_TEST_RECV_PAD);
                    recv_len = socket_recv( g_speed_test_recorder.sock_handle,  recvbuf, SPEED_TEST_RECV_MAX);
                    SCI_FREE(recvbuf);
                    recvbuf= NULL;
                    if ( recv_len == -1 )        // error occured 
                    {
                         break;
                    }
                    else if ( recv_len == 0 )    // socket connection is broken
                    {
                        break;
                    }
                    if (recv_len < SPEED_TEST_RECV_MAX)
                    {
                         break;   
                    }
                }
                if ( g_speed_test_recorder.sock_handle  != -1)
                {
                    SPEED_TEST_TRACE("[%s:%d]  step5: close socket",__FUNCTION__,__LINE__);
                    socket_close(g_speed_test_recorder.sock_handle);
                    g_speed_test_recorder.sock_handle = -1;
                }
            }
                break;

            //if socket recv has too much data, will send this message,
            case SOCKET_READ_BUFFER_STATUS_EVENT_IND:
            {     
                int recv_len = 0;
                char* recvbuf = NULL;

                SPEED_TEST_TRACE("SOCKET_READ_BUFFER_STATUS_EVENT_IND");
                while (1)
                {
                    recv_len = 0;
                    recvbuf = SCI_ALLOC(SPEED_TEST_RECV_MAX+SPEED_TEST_RECV_PAD);
                    if(recvbuf == NULL)
                    {
                        break;
                    }
                    SCI_MEMSET(recvbuf, 0,  SPEED_TEST_RECV_MAX+SPEED_TEST_RECV_PAD);
                    recv_len = socket_recv( g_speed_test_recorder.sock_handle,  recvbuf, SPEED_TEST_RECV_MAX);
                    SCI_FREE(recvbuf);
                    recvbuf= NULL;
                    if ( recv_len == -1 )        // error occured 
                    {
                         break;
                    }
                    else if ( recv_len == 0 )    // socket connection is broken
                    {
                        break;
                    }
                    if (recv_len < SPEED_TEST_RECV_MAX)
                    {
                         break;   
                    }
                }
                if ( g_speed_test_recorder.sock_handle  != -1)
                {
                    SPEED_TEST_TRACE("[%s:%d]  step5: close socket",__FUNCTION__,__LINE__);
                     socket_close(g_speed_test_recorder.sock_handle);
                     g_speed_test_recorder.sock_handle = -1;
                }
            }
            break;

            case SOCKET_CONNECTION_CLOSE_EVENT_IND:
            case SOCKET_FULL_CLOSED_EVENT_IND:
                //Step6: Name - Close socket
                SPEED_TEST_TRACE("SOCKET_CONNECTION_CLOSE_EVENT_IND/SOCKET_FULL_CLOSED_EVENT_IND");
                if (sig_ptr != PNULL)
                {
                    SCI_FREE( sig_ptr );
                    sig_ptr = PNULL;
                }
                SCI_MEMSET(&g_speed_test_recorder, 0, sizeof(speed_test_recorder_t));
                g_speed_test_task_id = SCI_INVALID_BLOCK_ID;
                b_is_exit = TRUE;
                break;
            default:
                break;
        } 
        if (sig_ptr != PNULL)
        {
            SCI_FREE( sig_ptr );
            sig_ptr = PNULL;
        }
    } // while(1)
ERROR:
    if (sig_ptr != PNULL)
    {
        SCI_FREE( sig_ptr );
        sig_ptr = PNULL;
    } 
    SCI_MEMSET(&g_speed_test_recorder, 0, sizeof(speed_test_recorder_t));
    g_speed_test_task_id = SCI_INVALID_BLOCK_ID;
    b_is_exit = TRUE;
    SPEED_TEST_TRACE("[exit]");
    SCI_ThreadExit();
}

/*****************************************************************************/
//  Description : Task function
//                                                  
//  Global resource dependence : 
//  Author: qin.he
//  Note: 
/*****************************************************************************/
LOCAL void speed_test_StartTask( void )
{
    if( SCI_INVALID_BLOCK_ID != g_speed_test_task_id )
    {
        // task has already started 
        SPEED_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__);
        return;
    }

    b_is_exit = FALSE;
    SPEED_TEST_TRACE("[%s:%d]",__FUNCTION__,__LINE__); 
    g_speed_test_task_id = SCI_CreateThread(
        "T_P_speedtest",
        "Q_P_speedtest",
        speed_test_entry,
        0,
        NULL,
        P_SPEED_TEST_STACK_SIZE,
        P_SPEED_TEST_QUEUE_NUM,
        P_SPEED_TEST_TASK_PRIORITY,
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
PUBLIC void speed_test_main(uint8 is_async, uint32 netid, uint32 timeout)
{
    b_is_async = is_async;
    memset(&g_speed_test_recorder, 0, sizeof(speed_test_recorder_t));
    
    g_speed_test_recorder.ServerIsIPv6 = FALSE;
    g_speed_test_recorder.sock_handle = -1;
    g_speed_test_recorder.net_id = netid;
    if(timeout == 0)
        g_speed_test_recorder.timeout = SPEED_TEST_TIME;
    else
       g_speed_test_recorder.timeout =  timeout;
    SPEED_TEST_TRACE("[%s:%d]  net_id:0x%x, timeout: %d",__FUNCTION__,__LINE__, g_speed_test_recorder.net_id, g_speed_test_recorder.timeout);
    speed_test_StartTask();
}

/*****************************************************************************/
//  Description :  exit function
//  Global resource dependence : 
//  Author: qin.he
//  Note: 
/*****************************************************************************/
PUBLIC int speed_test_exit(void)
{
    if ( g_speed_test_recorder.sock_handle  != -1)
    {
        SPEED_TEST_TRACE("[%s:%d]  step5: close socket",__FUNCTION__,__LINE__);
         socket_close(g_speed_test_recorder.sock_handle);
         g_speed_test_recorder.sock_handle = -1;
    }
    return 0;
}


