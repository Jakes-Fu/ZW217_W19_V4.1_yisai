/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: comm_socket.c
 *	Author		: sunguochen    
 *	Description	: 
 *	Vesion		: 1.0        
 *	Date			: 2010-4-2  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-2      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/

#include "mmi_push_mail.h"
#include "std_header.h"

#include "comm_socket.h"
#include "comm_platform.h"
#include "comm_interface.h"
#include "mail_main.h"

#ifdef WIN32
//#include "winsock.h"
#endif

comm_sock_list_t comm_sock_list = {0};



/*==========================================================
 * Function 	: comm_sock_listnum
 * Author		: sunguochen      
 * Description : 已打开的socket 数量
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
 ==========================================================*/
comm_uint32 comm_sock_listnum(void)
{

    return comm_sock_list.comm_sock_num;

}


/*==========================================================
 * Function 	: comm_sock_listadd
 * Author		: sunguochen      
 * Description : 已打开的socket 数量
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
 ==========================================================*/
comm_boolean comm_sock_listadd(int handle, void * host_session)
{

    int i;

    for(i = 0; i < COMM_SOCK_MAX_NUM; i++)
    {
        if(0 == comm_sock_list.comm_sock_list[i].sock_handle)
        {
            //socket svae
            comm_sock_list.comm_sock_list[i].sock_handle = handle;
            comm_sock_list.comm_sock_list[i].sock_is_notified = FALSE;         
            comm_sock_list.comm_sock_list[i].sock_state= COMM_SOCK_NONE;
            comm_sock_list.comm_sock_list[i].host_session = host_session;
            memset(&(comm_sock_list.comm_sock_list[i].sock_addr), 0, sizeof(comm_sockaddr));
            comm_sock_list.comm_sock_num ++;
            return TRUE;
        }
    }

    if(i >= COMM_SOCK_MAX_NUM)
    {
        //opened sock is max,just for test,can do not assert
        //COMM_ASSERT(0);
        return FALSE;
    }

    return FALSE;

}



/*==========================================================
 * Function 	: comm_sock_listdel
 * Author		: sunguochen      
 * Description : 已打开的socket 数量
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
 ==========================================================*/
void comm_sock_listdel(int handle)
{

    int i;

    for(i = 0; i < COMM_SOCK_MAX_NUM; i++)
    {
        if(handle == comm_sock_list.comm_sock_list[i].sock_handle)
        {
            //socket delete
            memset(&(comm_sock_list.comm_sock_list[i]), 0, sizeof(comm_sock_node_t));

            if(comm_sock_list.comm_sock_num)
            {
                comm_sock_list.comm_sock_num --;
            }
            break;
        }
    }

}



/*==========================================================
 * Function 	: comm_sock_listnode
 * Author		: sunguochen      
 * Description : 获取指定socket的信息
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/6/2010 
 ==========================================================*/
comm_sock_node_t* comm_sock_listnode(int handle)
{
    int i;
    comm_sock_node_t* sock_node = NULL;

    for(i = 0; i < COMM_SOCK_MAX_NUM; i++)
    {
        if(handle == comm_sock_list.comm_sock_list[i].sock_handle)
        {
            //socket id
            sock_node = &(comm_sock_list.comm_sock_list[i]);
            //COMM_TRACE:"Mail:comm_sock_listnode handle:%d index:%d node:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_SOCKET_134_112_2_18_3_26_11_17,(uint8*)"ddd", handle, i, sock_node);
            break;
        }
    }

    return sock_node;
}


/*==========================================================
 * Function 	: comm_sock_isssl
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/2/2010 
 ==========================================================*/
void comm_sock_addssl(int handle, SSL_HANDLE ssl_handle)
{
    comm_sock_node_t *sock_node = NULL;

    sock_node = comm_sock_listnode(handle);

    if(!sock_node)
    {
        //COMM_ASSERT(FALSE);
        return;
    }

    sock_node->is_ssl = TRUE;
    sock_node->ssl_handle = ssl_handle;
    
}


/*==========================================================
 * Function 	: comm_sock_isssl
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/2/2010 
 ==========================================================*/
comm_boolean comm_sock_isssl(int handle)
{
    comm_sock_node_t *sock_node = NULL;

    sock_node = comm_sock_listnode(handle);

    if(!sock_node)
    {
        //COMM_ASSERT(FALSE);
        return FALSE;
    }

    return sock_node->is_ssl;
    
}



/*==========================================================
 * Function 	: comm_sock_sessionbyssl
 * Author		: sunguochen      
 * Description : 通过ssl获取session
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/2/2010 
 ==========================================================*/
void * comm_sock_sessionbyssl(SSL_HANDLE ssl_handle)
{

    int i;
    comm_sock_node_t* sock_node = NULL;

    for(i = 0; i < COMM_SOCK_MAX_NUM; i++)
    {
        if(0 != comm_sock_list.comm_sock_list[i].ssl_handle)
        {
            //socket id
            sock_node = &(comm_sock_list.comm_sock_list[i]);
            break;
        }
    }

    //COMM_TRACE:"Mail:  socket sess node: %d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_SOCKET_219_112_2_18_3_26_11_18,(uint8*)"d", sock_node);

    if(sock_node)
    {
        //COMM_TRACE:"Mail:  ssl session handle %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_SOCKET_223_112_2_18_3_26_11_19,(uint8*)"d", sock_node->host_session);
        return sock_node->host_session;
    }
    else
    {
        //COMM_TRACE:"Mail: no ssl session\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_SOCKET_228_112_2_18_3_26_11_20,(uint8*)"");
        return NULL;
    }

}

    

/*==========================================================
 * Function 	: comm_sock_sslhandler
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/2/2010 
 ==========================================================*/
SSL_HANDLE comm_sock_sslhandler(int handle)
{
    comm_sock_node_t *sock_node = NULL;

    sock_node = comm_sock_listnode(handle);

    //COMM_TRACE:"Mail:  comm_sock_sslhandler sess node: %d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_SOCKET_250_112_2_18_3_26_11_21,(uint8*)"d", sock_node);

    if(!sock_node)
    {
        //COMM_ASSERT(FALSE);
        return 0;
    }

    return sock_node->ssl_handle;
    
}



/*==========================================================
 * Function 	: comm_sock_init
 * Author		: sunguochen      
 * Description : comm sock initial
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
 ==========================================================*/
void comm_sock_init(void)
{

    memset(&comm_sock_list, 0, sizeof(comm_sock_list_t));

}



/*==========================================================
 * Function 	: comm_sock_finale
 * Author		: sunguochen      
 * Description : comm sock finale
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
 ==========================================================*/
void comm_sock_finale(void)
{
    int i;

    if(0 == comm_sock_list.comm_sock_num)
    {

        memset(&comm_sock_list, 0, sizeof(comm_sock_list_t));
        return;

    }

    for(i = 0; i<COMM_SOCK_MAX_NUM; i++)
    {
        if(comm_sock_list.comm_sock_list[i].sock_handle)
        {
            //socket is not closed,close socket
            comm_sock_socketclose(comm_sock_list.comm_sock_list[i].sock_handle);
        }
    }

    memset(&comm_sock_list, 0, sizeof(comm_sock_list_t));

}


/*==========================================================
 * Function 	: comm_sock_open
 * Author		: sunguochen      
 * Description : create socket
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
// Note :   PARAM1: family (IN) -- protocol family, 
//                  for TCP/UDP, this value should be AF_INET
//          PARAM2: type (IN) -- protocol type;
//                  for TCP, type is SOCK_STREAM; for UDP, type is SOCK_DGRAM
//          PARAM3: proto (IN) -- protocol name;
//                  for TCP/UDP, this can be default 0;
//          PARAM4: netid (IN) -- net interface ID;
//          RETURN: if successful, return an none-zero socket; else -1
//          net_id is used to indicate the net interface which the socket data
//          want to go through. Normally, net_id can be 0, then the socket will
//          use the first usable net interface to send/recv its data; 
//          this can cause problem in multi-homing mode, net_id can be get when
//          the network channel start up, e.g. in GPRS, it is PDP active! 
 ==========================================================*/
int comm_sock_open(int family, int type, int proto, comm_uint32 net_id, void * host_session)
{
    int sock_handle = 0;
    int result;
#ifndef WIN32      
    unsigned long nonBlockingFlag = 1;
#endif
#ifdef WIN32     
    int arg=1;
#endif
#ifdef WIN32    
    sock_handle = socket(AF_INET, SOCK_STREAM, 0);
#else
    sock_handle = sci_sock_socket(family, type, proto, net_id);
#endif

    if(!sock_handle)
    {
        //socket create fail
        return sock_handle;
    }
#ifdef WIN32
    result = ioctlsocket(sock_handle, FIONBIO, &arg);
#else
    result = sci_sock_setsockopt(sock_handle, SO_NBIO, &nonBlockingFlag);
#endif
    if(result) 
    {
        sci_sock_socketclose(sock_handle);
        sock_handle = 0;
    }

    if(sock_handle)
    {
        comm_sock_listadd(sock_handle, host_session);
    }

    return sock_handle;

}



/*==========================================================
 * Function 	: comm_sock_connect
 * Author		: sunguochen      
 * Description : socket connect
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
    // Note :   socket connect, for TCP client to connect to server,
    //          PARAM1: s (IN) -- socket, 
    //          PARAM2: addr_ptr (IN) -- address to connect
    //          PARAM3: addr_len (IN) -- address length,
    //                  this value is in fact no use, set with 0 
    //          RETURN: 0 for success, -1 for failure      
 ==========================================================*/
int comm_sock_connect(long s, comm_sockaddr* addr, int addrlen)
{
    int result = 0;
    comm_sock_node_t* sock_node = NULL;

#ifdef WIN32
    struct sockaddr_in dest_addr; /* 目的地址*/  
    dest_addr.sin_family = AF_INET; /* host byte order */  
    dest_addr.sin_port = addr->port; /* short, network byte order */ 
    dest_addr.sin_addr.s_addr = addr->ip_addr;
    
    result = connect(s, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));  
#else    
    result = sci_sock_connect(s, (struct sci_sockaddr*)addr, addrlen);
#endif
    sock_node = comm_sock_listnode(s);

    if(sock_node)
    {
        //save socket connet address
        memcpy(&(sock_node->sock_addr), addr, sizeof(comm_sockaddr));
        if(result)
        {
            sock_node->sock_state = COMM_SOCK_CONNECTING;
        }
        else
        {
            sock_node->sock_state = COMM_SOCK_CONNECTED;            
        }    
        
    }

    return result;

}


/*==========================================================
 * Function 	: comm_sock_send
 * Author		: sunguochen      
 * Description : socket send ( for TCP )
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
// Note :   PARAM1: s (IN) -- socket, 
//          PARAM2: buf (IN) -- send data pointer
//          PARAM3: len (IN) -- send data length
//          PARAM4: flag (IN) -- socket send option, default with 0
//          RETURN: if success, return data length sent; else -1 for failure     
 ==========================================================*/
int comm_sock_send(long s, char* buf, int len, int flags)
{
    int result = 0;
    comm_sock_node_t* sock_node = NULL;

#ifdef WIN32
    result = send(s, buf, len, flags);
#else    
    result = sci_sock_send(s, buf, len, flags);
#endif
    sock_node = comm_sock_listnode(s);

    if(sock_node)
    {
        if(result)
        {
            //sending
            sock_node->sock_state = COMM_SOCK_TRANSMIT;
        }        
    }

    return result;
    
}



/*==========================================================
 * Function 	: comm_sock_sendto
 * Author		: sunguochen      
 * Description : socket send ( for UDP  & TCP)
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
// Note :   PARAM1: s (IN) -- socket, 
//          PARAM2: buf (IN) -- send data pointer
//          PARAM3: len (IN) -- send length
//          PARAM4: flag (IN) -- socket send option, default with 0
//          PARAM5: to (IN) -- data destination address pointer
//          RETURN: if success, return data length sent; else -1 for failure   
 ==========================================================*/
PUBLIC int comm_sock_sendto(long s, char* buf, int len, int flag, comm_sockaddr* to)
{
    int result = 0;
    comm_sock_node_t* sock_node = NULL;
    
    result = sci_sock_sendto(s, buf, len, flag, (struct sci_sockaddr*)to);

    sock_node = comm_sock_listnode(s);

    if(sock_node)
    {
        if(result)
        {
            //sending
            sock_node->sock_state = COMM_SOCK_TRANSMIT;
        }        
    }

    return result;

}


/*==========================================================
 * Function 	: comm_sock_recv
 * Author		: sunguochen      
 * Description : socket receive ( for TCP )
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
// Note :   PARAM1: s (IN) -- socket, 
//          PARAM2: buf (OUT) -- receive buffer pointer
//          PARAM3: len (IN) -- receive buffer length
//          PARAM4: flag (IN) -- socket receive option, default with 0
//          RETURN: if success, return data length received; else -1 for failure   
 ==========================================================*/
int comm_sock_recv(long s, char* buf, int len, int flags)
{
    int result = 0;
    comm_sock_node_t* sock_node = NULL;
#ifdef WIN32
    result = recv(s, buf, len, flags);
#else    
    result = sci_sock_recv(s, buf, len, flags);
#endif
    sock_node = comm_sock_listnode(s);

    if((result > 0) && sock_node->is_ssl && sock_node->ssl_handle)
    {
        //ssl socket decrypt data
        //COMM_TRACE:"Mail: start decrypt data len :%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_SOCKET_534_112_2_18_3_26_12_22,(uint8*)"d", result);
        comm_TraceFile(buf, result);
        SSL_DecryptPasser(sock_node->ssl_handle, (comm_uint8 *)buf, result);
        mail_SetSSLPasserState(TRUE);
        //ssl 时不返回数据
        return 0;
        
    }
    
    if(sock_node)
    {
        if(result)
        {
            //sending
            sock_node->sock_state = COMM_SOCK_TRANSMIT;
        }        
    }

    return result;
    
}


/*==========================================================
 * Function 	: comm_sock_recvfrom
 * Author		: sunguochen      
 * Description : socket receive ( for UDP and TCP)
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
// Note :   PARAM1: s (IN) -- socket, 
//          PARAM2: buf (OUT) -- receive buffer pointer
//          PARAM3: len (IN) -- receive buffer length
//          PARAM4: flag (IN) -- socket receive option, default with 0
//          PARAM5: from (OUT) -- data source address pointer
//          RETURN: if success, return data length received; else -1 for failure    
 ==========================================================*/
int comm_sock_recvfrom(long s, char* buf, int len, int flag, comm_sockaddr* from)
{
    int result = 0;
    comm_sock_node_t* sock_node = NULL;
    
    result = sci_sock_recvfrom(s, buf, len, flag, (struct sci_sockaddr*)from);

    sock_node = comm_sock_listnode(s);

    if(result && sock_node->is_ssl && sock_node->ssl_handle)
    {
        //ssl socket decrypt data
        //COMM_TRACE:"Mail: start decrypt data len :%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_SOCKET_583_112_2_18_3_26_12_23,(uint8*)"d", result);
        SSL_DecryptPasser(sock_node->ssl_handle, (comm_uint8 *)buf, result);
        mail_SetSSLPasserState(TRUE);        
    }
    
    if(sock_node)
    {
        if(result)
        {
            //sending
            sock_node->sock_state = COMM_SOCK_TRANSMIT;
        }        
    }

    return result;

}


/*==========================================================
 * Function 	: comm_sock_shutdown
 * Author		: sunguochen      
 * Description : socket shutdown ( for TCP )
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
// Note :   PARAM1: s (IN) -- socket, 
//          PARAM2: how (IN) -- shutdown type, include:
//                  SD_RECV, SD_SEND and SD_BOTH
//          RETURN: 0 for success, -1 for failure 
 ==========================================================*/
int comm_sock_shutdown(long s, int how)
{
    int result = 0;
    
    result = sci_sock_shutdown( s, SD_BOTH);

    return result;

}


/*==========================================================
 * Function 	: comm_sock_select
 * Author		: sunguochen      
 * Description : socket select
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
// Note :   PARAM1: in (IN) -- socket list for read event watching, 
//          PARAM2: out (IN) -- socket list for write event watching,
//          PARAM3: ex (IN) -- socket list for exception event watching,
//          PARAM4: tv (IN) -- watching time-out value, unit: ticks = 0.1s
//          RETURN: > 0: watching event occur;
//                  = 0: watching event time-out;
//                  =-1: failure 
 ==========================================================*/
int comm_sock_select(
                                        comm_fd_set* in,  
                                        comm_fd_set* out,
                                        comm_fd_set* ex,
                                        long tv )   /* ticks to wait */
{
    int result = 0;
    
    result = sci_sock_select(in, out, ex, tv);

    return result;

}


/*==========================================================
 * Function 	: COMM_FD_SET
 * Author		: sunguochen      
 * Description : add socket to socket list
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
// Note :   PARAM1: s (IN) -- socket,
//          PARAM2: set (IN) -- socket list pointer
 ==========================================================*/
void COMM_FD_SET(long s, comm_fd_set* set)
{

    SCI_FD_SET(s, (sci_fd_set*)set);

}


/*==========================================================
 * Function 	: COMM_FD_CLR
 * Author		: sunguochen      
 * Description : clear socket from socket list
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
// Note :   PARAM1: s (IN) -- socket,
//          PARAM2: set (IN) -- socket list pointer 
 ==========================================================*/
void COMM_FD_CLR(long s, comm_fd_set* set)
{

    SCI_FD_CLR(s, (sci_fd_set*)set);

}


/*==========================================================
 * Function 	: COMM_FD_ISSET
 * Author		: sunguochen      
 * Description : check if socket in socket list
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
// Note :   PARAM1: s (IN) -- socket,
//          PARAM2: set (IN) -- socket list pointer 
//          RETURN: 1 - is in socket list; 0 - not in socket list
 ==========================================================*/
int COMM_FD_ISSET(long s, comm_fd_set* set)
{

    return SCI_FD_ISSET(s, (sci_fd_set*)set);

}


/*==========================================================
 * Function 	: COMM_FD_ZERO
 * Author		: sunguochen      
 * Description : clear socket list
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
// Note :   PARAM1: set (IN) -- socket list,
 ==========================================================*/
void COMM_FD_ZERO(comm_fd_set* set)
{
    SCI_FD_ZERO((sci_fd_set*)set);
}


/*==========================================================
 * Function 	: comm_sock_errno
 * Author		: sunguochen      
 * Description : get last error of a given socket
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
// Note :   PARAM1: s (IN) -- socket,
//          RETURN: error number of socket s
//                  error number definition are as E** in socket_types.h
//          Return value will be undefined if the socket has not previously 
//          reported an error. 
 ==========================================================*/
int comm_sock_errno(long s)
{
    int result = 0;

    if(!s)
    {
        return 0;
    }

#ifdef WIN32
    result = WSAGetLastError();
#else    
    result = sci_sock_errno(s);
#endif

    //COMM_TRACE:"MAIL: comm_sock_errno sock :%x err :%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_SOCKET_753_112_2_18_3_26_12_24,(uint8*)"dd", s, result);

    return result;

}


/*==========================================================
 * Function 	: comm_sock_socketclose
 * Author		: sunguochen      
 * Description : socket close
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
// Note :   PARAM1: s (IN) -- socket,
//          RETURN: 0 for success, -1 for failure 
 ==========================================================*/
int comm_sock_socketclose(long s)
{
    int result = 0;
    
    SSL_HANDLE ssl_handler = 0;

    ssl_handler = comm_sock_sslhandler(s);

    if(ssl_handler) 
    {
        SSL_RESULT_E ssl_result;
        ssl_result = SSL_Close(ssl_handler, SSL_ASYNC);
        //COMM_TRACE:"Mail: close ssl sock handle : %d  result:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_SOCKET_782_112_2_18_3_26_12_25,(uint8*)"dd", ssl_handler, ssl_result);
    }
    
    result = sci_sock_socketclose(s);
    //COMM_TRACE:"Mail: close sock result:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_SOCKET_786_112_2_18_3_26_12_26,(uint8*)"d", result);

    comm_sock_listdel(s);

    return result;

}



/*==========================================================
 * Function 	: comm_sock_setsockopt
 * Author		: sunguochen      
 * Description : set socket options
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
// Note :   PARAM1: s (IN) -- socket, 
//          PARAM2: name (IN) -- option name, 
//                  macros with SO_ defined in tcpip_types.h
//          PARAM3: arg (IN) -- option value pointer
//                  for sock options without arg (e.g. SO_NBIO), this can be 0
//          RETURN: 0 for success, -1 for failure    
 ==========================================================*/
 int comm_sock_setsockopt(long s, int name, void* arg)
{
    int result = 0;

    result = sci_sock_setsockopt(s, name, arg);

    return result;

}


/*==========================================================
 * Function 	: comm_sock_getsockopt
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
// Note :   PARAM1: s (IN) -- socket, 
//          PARAM2: name (IN) -- option name
//                  macros with SO_ defined in tcpip_types.h
//          PARAM3: arg (OUT) -- option value pointer
//          RETURN: 0 for success, -1 for failure   
 ==========================================================*/
 int comm_sock_getsockopt(long s, int name, void* arg)
{
    int result = 0;

    result = sci_sock_getsockopt(s, name, arg);

    return result;

}


/*==========================================================
 * Function 	: comm_sock_setasyncnotify
 * Author		: sunguochen      
 * Description : 设置socket 事件通知
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
// Note :   Call async select to set watch for given socket, when event occurs,
//          TCPIP will send message to the given task to inform the event,
//          Repeatly function call on same socket will cover events set before!
//          PARAM1: s (IN) -- socket
//          PARAM2: task_id (IN) -- task ID which is to deal with the socket event 
//          PARAM3: is_open (IN) -- 打开或者取消通知
 ==========================================================*/
int comm_sock_asyncnotify(long s, comm_uint32 task_id, comm_boolean is_open)
{

    int result = 0;
    comm_sock_node_t* sock_node = NULL;
    
    sock_node = comm_sock_listnode(s);
    
    if(sock_node)
    {
        if(is_open)
        {
            //async
            sock_node->sock_is_notified = TRUE;
        }
        else
        {
            //polling            
            sock_node->sock_is_notified = FALSE;            
        }
    }

#ifdef WIN32
    sock_node->sock_is_notified = FALSE;
    return -1;
#endif

    if(is_open)
    {
        result = sci_sock_asyncselect(s, task_id, AS_READ|AS_WRITE|AS_CONNECT|AS_CLOSE|AS_ACCEPT);
    }
    else
    {
        result = sci_sock_asyncselect(s, task_id, AS_NULL );        
    }

    return result;    
    
}


/*==========================================================
 * Function 	: comm_SockGetHostByUrl
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/
void comm_SockGetHostByUrl(char* url, char** host_ptr, comm_uint32 *host_len)
{
    struct sci_hostent * host = NULL;

    comm_uint32 addr_len = 0;

    host = sci_gethostbyname(url);

    if(!host)
    {
        * host_ptr= NULL;
        *host_len = 0;
    }

    addr_len = strlen(host->h_name);

    *host_ptr = comm_alloc(addr_len);

    strcpy(*host_ptr, host->h_name);

    *host_len = addr_len;


}



/*==========================================================
 * Function 	: comm_sock_sslshake
 * Author		: sunguochen      
 * Description : ssl 创建及连接交互
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/2/2010 
 ==========================================================*/
comm_boolean comm_sock_sslshake(long s, SSL_CALLBACK_T *cb_ptr)
{
    SSL_HANDLE ssl_handle = 0;
    
    void * app_handler = NULL;
    comm_sock_node_t *sock_node = NULL;
    char *addr_str = NULL;
    int    sock_port= 0;
    SSL_RESULT_E  shake_result = 0;
    
    sock_node = comm_sock_listnode(s);
    
    if(!sock_node)
    {
        return FALSE;
    }
    
    app_handler= (void *)mail_AppHandler();
    
    //create ssl handle
    ssl_handle = SSL_Create(app_handler, s, SSL_ASYNC);
    
    if(!ssl_handle)
    {
        //COMM_TRACE:"Mail: sock create ssl error \n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_SOCKET_967_112_2_18_3_26_13_27,(uint8*)"");
        return FALSE;
    }
    else
    {
        //COMM_TRACE:"Mail: sock ssl handle :%d \n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_SOCKET_972_112_2_18_3_26_13_28,(uint8*)"d", ssl_handle);
    }

    comm_sock_addssl(s, ssl_handle);
    
    SSL_AsyncRegCallback(ssl_handle, cb_ptr);
    
    //select ssl protocol
    SSL_ProtocolChoose(ssl_handle, SSL_PROTOCOLSSL, SSL_ASYNC);
    
#ifdef WIN32
    addr_str = "10.0.0.172";
#else
    addr_str = inet_ntoa(sock_node->sock_addr.ip_addr);
#endif
    
    sock_port = sock_node->sock_addr.port;
    
    //COMM_TRACE:"Mail: sock ssl addr: %s %d \n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_SOCKET_990_112_2_18_3_26_13_29,(uint8*)"sd", addr_str, sock_port);
    shake_result= SSL_HandShake(ssl_handle, addr_str, sock_port, SSL_ASYNC);
    
    //COMM_TRACE:"Mail: sock ssl shake result: %d \n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_SOCKET_993_112_2_18_3_26_13_30,(uint8*)"d", shake_result);

    return TRUE;

}



/*==========================================================
 * Function 	: comm_sock_EncrytData
 * Author		: sunguochen      
 * Description : 开始数据加密
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History       : created  6/2/2010 
 ==========================================================*/
comm_boolean comm_sock_EncrytData(long s, char* buf, int len)
{

    comm_sock_node_t* sock_node = NULL;

    sock_node = comm_sock_listnode(s);
    
    if(sock_node->is_ssl && sock_node->ssl_handle)
    {
        //ssl socket encrypt data
        //COMM_TRACE:"Mail: start encrypt data len :%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_SOCKET_1019_112_2_18_3_26_13_31,(uint8*)"d", len);
        SSL_EncryptPasser(sock_node->ssl_handle, (comm_uint8 *)buf, len);
        mail_SetSSLPasserState(TRUE);
        return TRUE;
        
    }
    else
    {
        //COMM_TRACE:"Mail: start encrypt error :%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_SOCKET_1027_112_2_18_3_26_13_32,(uint8*)"d", len);
        return FALSE;        
    }
}
 




