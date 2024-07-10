/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: comm_socket.h
 *	Author		: sunguochen    
 *	Description	: 
 *	Vesion		: 1.0        
 *	Date			: 2010-4-2  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-2      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/

#ifndef _COMM_SOCKET_H_
#define _COMM_SOCKET_H_

//#ifdef WIN32
//#include "winsock.h"
//#else
#include "socket_api.h"
//#endif
#include "socket_types.h"
#include "comm_platform.h"

#include "ssl_api.h"

#define COMM_SOCK_MAX_NUM  5



// Address families.
#define COMM_AF_UNSPEC      AF_UNSPEC
#define COMM_AF_NS              AF_NS
#define COMM_AF_INET           AF_INET
#define COMM_AF_MAX            AF_MAX

// Socket Types
#define COMM_SOCK_STREAM     SOCK_STREAM     /* stream socket */
#define COMM_SOCK_DGRAM      SOCK_DGRAM       /* datagram socket */
#define COMM_SOCK_RAW            SOCK_RAW       /* raw-protocol interface */



// socket error code
#define COMM_ENOBUFS                 ENOBUFS      
#define COMM_ETIMEDOUT             ETIMEDOUT
#define COMM_EISCONN                 EISCONN
#define COMM_EOPNOTSUPP          EOPNOTSUPP
#define COMM_ECONNABORTED      ECONNABORTED
#ifdef WIN32
#define COMM_EWOULDBLOCK        WSAEWOULDBLOCK
#else
#define COMM_EWOULDBLOCK        EWOULDBLOCK
#endif
#define COMM_ECONNREFUSED       ECONNREFUSED
#define COMM_ECONNRESET           ECONNRESET
#define COMM_ENOTCONN              ENOTCONN
#define COMM_EALREADY                EALREADY
#define COMM_EINVAL                     EINVAL
#define COMM_EMSGSIZE                 EMSGSIZE
#define COMM_EPIPE                       EPIPE
#define COMM_EDESTADDRREQ        EDESTADDRREQ
#define COMM_ESHUTDOWN             ESHUTDOWN
#define COMM_ENOPROTOOPT         ESHUTDOWN
#define COMM_EHAVEOOB                EHAVEOOB
#define COMM_ENOMEM                   ENOMEM
#define COMM_EADDRNOTAVAIL      EADDRNOTAVAIL
#define COMM_EADDRINUSE            EADDRINUSE
#define COMM_EAFNOSUPPORT       EAFNOSUPPORT
#ifdef WIN32
#define COMM_EINPROGRESS          WSAEISCONN
#else
#define COMM_EINPROGRESS          EINPROGRESS
#endif
#define COMM_ELOWER                   ELOWER         /* lower layer (IP) error */
#define COMM_ENOTSOCK               ENOTSOCK          /* Includes sockets which closed while blocked */
#define COMM_EIEIO                       EIEIO          /* bad input/output on Old Macdonald's farm :-) */
#define COMM_ETOOMANYREFS       ETOOMANYREFS
#define COMM_EFAULT                    EFAULT
#define COMM_ENETUNREACH         ENETUNREACH


// socket shutdown options
#define COMM_RECV         SD_RECV
#define COMM_SEND         SD_RECV
#define COMM_BOTH         SD_RECV


typedef struct sci_sockaddr  comm_sockaddr;
typedef sci_fd_set               comm_fd_set;


#define comm_inet_aton(a,b)   inet_aton(a,b)
#define comm_inet_ntoa(a)      inet_ntoa(a)


typedef enum
{
    COMM_SOCK_NONE,
    COMM_SOCK_NOCONNECT,
    COMM_SOCK_CONNECTING,
    COMM_SOCK_CONNECTED,
    COMM_SOCK_TRANSMIT,    
    COMM_SOCK_MAX

}COMM_SOCK_STATE_E;


typedef struct _comm_sock_node
{
    int                                 sock_handle;
    COMM_SOCK_STATE_E     sock_state;
    comm_sockaddr               sock_addr;
    SSL_HANDLE                   ssl_handle; 
    void  *                           host_session;                
    comm_boolean                is_ssl;
    comm_boolean                sock_is_notified;    

}comm_sock_node_t;


//opened socket list
typedef struct _comm_sock_list
{
    comm_uint32            comm_sock_num;
    comm_sock_node_t   comm_sock_list[COMM_SOCK_MAX_NUM];

}comm_sock_list_t;



/*==========================================================
 * Function 	: comm_sock_init
 * Author		: sunguochen      
 * Description : comm sock initial
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
 ==========================================================*/
void comm_sock_init(void);



/*==========================================================
 * Function 	: comm_sock_finale
 * Author		: sunguochen      
 * Description : comm sock finale
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/2/2010 
 ==========================================================*/
void comm_sock_finale(void);



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
int comm_sock_open(int family, int type, int proto, comm_uint32 net_id, void * host_session);


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
int comm_sock_connect(long s, comm_sockaddr* addr, int addrlen);


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
int comm_sock_send(long s, char* buf, int len, int flags);


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
PUBLIC int comm_sock_sendto(long s, char* buf, int len, int flag, comm_sockaddr* to);



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
int comm_sock_recv(long s, char* buf, int len, int flags);


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
int comm_sock_recvfrom(long s, char* buf, int len, int flag, comm_sockaddr* from);


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
int comm_sock_shutdown(long s, int how);


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
                                        long tv );


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
void COMM_FD_SET(long s, comm_fd_set* set);


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
void COMM_FD_CLR(long s, comm_fd_set* set);


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
int COMM_FD_ISSET(long s, comm_fd_set* set);


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
void COMM_FD_ZERO(comm_fd_set* set);


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
int comm_sock_errno(long s);


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
int comm_sock_socketclose(long s);



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
 int comm_sock_setsockopt(long s, int name, void* arg);


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
 int comm_sock_getsockopt(long s, int name, void* arg);


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
int comm_sock_asyncnotify(long s, comm_uint32 task_id, comm_boolean is_open);



/*==========================================================
 * Function 	: comm_SockGetHostByUrl
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/
void comm_SockGetHostByUrl(char* url, char** host_ptr, comm_uint32 *host_len);



/*==========================================================
 * Function 	: comm_sock_sslshake
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/2/2010 
 ==========================================================*/
comm_boolean comm_sock_sslshake(long s, SSL_CALLBACK_T *cb_ptr);


/*==========================================================
 * Function 	: comm_sock_EncrytData
 * Author		: sunguochen      
 * Description : 开始数据加密
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History       : created  6/2/2010 
 ==========================================================*/
comm_boolean comm_sock_EncrytData(long s, char* buf, int len);



/*==========================================================
 * Function 	: comm_sock_sessionbyssl
 * Author		: sunguochen      
 * Description : 通过ssl获取session
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/2/2010 
 ==========================================================*/
void * comm_sock_sessionbyssl(SSL_HANDLE ssl_handle);



/*==========================================================
 * Function 	: comm_sock_sslhandler
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/2/2010 
 ==========================================================*/
SSL_HANDLE comm_sock_sslhandler(int handle);



#endif

