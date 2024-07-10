/****************************************************************************
** File Name:      socket.c                                              *
** Author:         	                                          *
** Date:           15/10/2019                                              *
** Copyright:                                                              *
** Description:      socket                                        *
****************************************************************************/
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#if 1//unisoc added
// MACOR platform interface
#include "socket_types.h"
#include "sci_types.h"
#include "os_api.h"
#include "sfs.h"
#endif
#include "socket.h" ///for up layer
#ifdef __cplusplus
    extern   "C"
    {
#endif

#include <errno.h>

/**
   * Marco
   */
#define SOCKET_DEBUG
#ifdef SOCKET_DEBUG
#define DEBUG  SCI_TRACE_LOW
#else
#define DEBUG
#endif

//#define POLLIN		01              /* There is data to read.  */
//#define POLLPRI		02              /* There is urgent data to read.  */
//#define POLLOUT		04              /* Writing now will not block.  */

#define SOCKERRNO         (errno) 
#define SET_SOCKERRNO(x)  (errno = (x))
/**
  * global value
  */
LOCAL unsigned int g_socket_nitId = 0;
LOCAL BOOLEAN g_socket_data_save = FALSE;
static struct hostent g_hostent = {0};
PUBLIC void socketSaveDataToFile(char* buff_ptr, int32 len);

/**
  * static funciton
  */
LOCAL int socket_ErrorMap(int socket_error)
{
	int error_no = socket_error;
	
	switch(socket_error)
	{
		case ENOBUFS:
			error_no = 9917;
			break;

		case ETIMEDOUT:
			error_no = 9938;
			break;

		case EISCONN:
			error_no = 9904;
			break;

		case EOPNOTSUPP:
			error_no = 9929;
			break;

		case ECONNABORTED:
			error_no = 9906;
			break;

		case EWOULDBLOCK:
			error_no = 9930;
			break;

		case ECONNREFUSED:
			error_no = 9908;
			break;

		case ECONNRESET:
			error_no = 9909;
			break;

		case ENOTCONN:
			error_no = 9925;
			break;

		case EALREADY:
			error_no = 9907;
			break;

		case EINVAL:
			error_no = 9943;
			break;

		case EMSGSIZE:
			error_no = 9913;
			break;

		case EPIPE:
			error_no = 9950;
			break;

		case EDESTADDRREQ:
			error_no = 9910;
			break;

		//case ESHUTDOWN:
			//error_no = 15;
			//break;

		case ENOPROTOOPT:
			error_no = 9921;
			break;

		//case EHAVEOOB:
			//error_no = 17;
			//break;

		case ENOMEM:
			error_no = 9971;
			break;

		case EADDRNOTAVAIL:
			error_no = 9903;
			break;

		case EADDRINUSE:
			error_no = 9902;
			break;

		case EAFNOSUPPORT:
			error_no = 9901;
			break;

		case EINPROGRESS:
			error_no = 9928;
			break;

		//case ELOWER:
			//error_no = 23;
			//break;

		case ENOTSOCK:
			error_no = 9923;
			break;

		//case EIEIO:
			//error_no = 27;
			//break;

		//case ETOOMANYREFS:
			//error_no = 28;
			//break;

		case EFAULT:
			error_no = 9948;
			break;

		case ENETUNREACH:
			error_no = 9916;
			break;
		default:
			break;
	}
	DEBUG("socket [%s:%d] socket error: %d map to %d\n ",__FUNCTION__,__LINE__,socket_error, error_no);
	return error_no;
}

/**
  *	@Note:Up layer interface. 
  */
PUBLIC void socket_SetNetId(unsigned int id)
{
    g_socket_nitId = id;
}

#ifndef CONST_CAST
#define CONST_CAST(type, val)   ( (type)val )
#endif

PUBLIC unsigned int socket_getNetId(void)
{
    return g_socket_nitId;
}


struct hostent *gethostbynameBlock(const char *hostname)
{
    struct hostent *host_ptr = PNULL;
    int count = 12;
    DEBUG("[socket debug %s       %d]\n",__FUNCTION__,__LINE__);
    while (count--)
    {
        host_ptr = gethostbyname_block(hostname, TCPIPNETIF_GetNetifWithNetid(g_socket_nitId));
        if (host_ptr != NULL)
        {
            DEBUG("[socket debug %s       %d] family: %d\n",__FUNCTION__,__LINE__, host_ptr->h_addrtype);
            break;
        }
    }

    return host_ptr;
}



PUBLIC struct hostent * curl_gethostbyname(char *name_ptr)
{
	struct sci_hostent* sci_info_ptr = NULL;
    memset(&g_hostent, 0, sizeof(struct hostent));
    DEBUG("[socket debug %s     %d ] >>>>>>>>  \n",__FUNCTION__,__LINE__);
    sci_info_ptr = sci_gethostbyname_ext(name_ptr,socket_getNetId());
    
    DEBUG("[socket debug %s       %d]\n",__FUNCTION__,__LINE__);
    if (NULL != sci_info_ptr && sci_info_ptr->h_addr_list != NULL)
    {
        DEBUG("[socket debug %s      %d,name =%s ]\n",__FUNCTION__,__LINE__,sci_info_ptr->h_name);
        if(sci_info_ptr->h_name != NULL)
        {
            g_hostent.h_name = sci_info_ptr->h_name;  
        }
        g_hostent.h_addr_list = sci_info_ptr->h_addr_list;
        g_hostent.h_addrtype = sci_info_ptr->h_addrtype;
        if(sci_info_ptr->h_aliases != NULL)
        {
            g_hostent.h_aliases = sci_info_ptr->h_aliases;
        }
        g_hostent.h_length = sci_info_ptr->h_length;
        DEBUG("[socket debug %s       %d]\n",__FUNCTION__,__LINE__);        
    }
    return &g_hostent;

}





/*
 * SPLIBC_he2ai()
 *
 * This function returns a pointer to the first element of a newly allocated
 * addrinfo struct linked list filled with the data of a given hostent.
 * addrinfo is meant to work like the addrinfo struct does for a IPv6
 * stack, but usable also for IPv4, all hosts and environments.
 *
 * The memory allocated by this function *MUST* be free'd later on calling
 * Curl_freeaddrinfo().  For each successful call to this function there
 * must be an associated call later to Curl_freeaddrinfo().
 *
 *   addrinfo defined in "lib/addrinfo.h"
 *
 *     struct addrinfo {
 *       int                   ai_flags;
 *       int                   ai_family;
 *       int                   ai_socktype;
 *       int                   ai_protocol;
 *       curl_socklen_t        ai_addrlen;   * Follow rfc3493 struct addrinfo *
 *       char                 *ai_canonname;
 *       struct sockaddr      *ai_addr;
 *       struct addrinfo *ai_next;
 *     };
 *
 *   hostent defined in <netdb.h>
 *
 *     struct hostent {
 *       char    *h_name;
 *       char    **h_aliases;
 *       int     h_addrtype;
 *       int     h_length;
 *       char    **h_addr_list;
 *     };
 *
 *   for backward compatibility:
 *
 *     #define h_addr  h_addr_list[0]
 */

static struct addrinfo *threax_he2ai(const struct hostent *he, int port)
{
    struct addrinfo *ai = NULL;
    struct addrinfo *prevai = NULL;
    struct addrinfo *firstai = NULL;
    struct sockaddr_in *addr;
#ifdef ENABLE_IPV6
    struct sockaddr_in6 *addr6;
#endif
    int result = 0;
    int i;
    char *curr;

    if(!he)
    /* no input == no output! */
    return NULL;

    if ((he->h_name == NULL) || (he->h_addr_list == NULL))
    {
        DEBUG("[socket debug  ] [%s  %d ]]",__FUNCTION__,__LINE__);
        return NULL;    
    }

    //SCI_ASSERT((he->h_name != NULL) && (he->h_addr_list != NULL));

    for(i = 0; (curr = he->h_addr_list[i]) != NULL; i++) {
        size_t ss_size;
        size_t namelen = strlen(he->h_name) + 1; /* include zero termination */
#ifdef ENABLE_IPV6
        if(he->h_addrtype == AF_INET6)
          ss_size = sizeof(struct sockaddr_in6);
        else
#endif
          ss_size = sizeof(struct sockaddr_in);

        /* allocate memory to hold the struct, the address and the name */
        ai = SCI_ALLOCAZ( sizeof(struct addrinfo) + ss_size + namelen);
        if(!ai) {
          result = 27;//CURLE_OUT_OF_MEMORY;
         DEBUG("[socket debug] calloc error");
          break;
        }
        /* put the address after the struct */
        ai->ai_addr = (void *)((char *)ai + sizeof(struct addrinfo));
        /* then put the name after the address */
        ai->ai_canonname = (char *)ai->ai_addr + ss_size;
        memcpy(ai->ai_canonname, he->h_name, namelen);

        if(!firstai)
          /* store the pointer we want to return from this function */
          firstai = ai;

        if(prevai)
          /* make the previous entry point to this */
          prevai->ai_next = ai;

        ai->ai_family = he->h_addrtype;

        /* we return all names as STREAM, so when using this address for TFTP
           the type must be ignored and conn->socktype be used instead! */
        ai->ai_socktype = SOCK_STREAM;

        ai->ai_addrlen = (int)ss_size;

        /* leave the rest of the struct filled with zero */

        switch(ai->ai_family) {
            case AF_INET:
              addr = (void *)ai->ai_addr; /* storage area for this info */

              memcpy(&addr->sin_addr, curr, sizeof(struct in_addr));
              addr->sin_family = (int)(he->h_addrtype);
              addr->sin_port = htons((unsigned short)port);
              DEBUG("[socket debug] ip4:%s", inet_ntoa(addr->sin_addr.s_addr));
              break;

#ifdef ENABLE_IPV6
            case AF_INET6:
              addr6 = (void *)ai->ai_addr; /* storage area for this info */

              memcpy(&addr6->sin6_addr, curr, sizeof(struct in6_addr));
              addr6->sin6_family = (int)(he->h_addrtype);
              addr6->sin6_port = htons((unsigned short)port);
              break;
#endif
            default:
                break;
        }

        prevai = ai;
    }

    if(result) {
        freeaddrinfo(firstai);
        firstai = NULL;
    }
    
      DEBUG("[socket debug] result:%d, firstai:0x%x", result, firstai);

    return firstai;
}

int SPLIBC_getaddrinfo(CONST char *nodename,
            CONST char *servname,
            CONST struct addrinfo *hints,
            struct addrinfo **res)
{
    struct hostent *h = NULL;
    struct addrinfo *ai = NULL;
    int port = 0;
    
    if (res == NULL )
    {	
    	return -1;
    }

    DEBUG("[jsong_debug] nodename:%s, servname:%s", nodename, servname);
    SCI_TRACE_LOW("[jsong_debug] nodename:%s, servname:%s", nodename, servname);

    if (servname != NULL)
        port = atoi(servname);
    DEBUG("[jsong_debug] port:%d", port);
    h = curl_gethostbyname((void *)nodename);
    
    //SCI_DEBUG("[jsong_debug] h->h_addr_list:0x%x", h->h_addr_list);

    if(h->h_addr_list != NULL) 
    {
    	ai = threax_he2ai(h, port);
    }
    else
    {
        DEBUG("[jsong_debug]    error");
        return -1;
    }
    DEBUG("[jsong_debug]ai:0x%x", ai);

    if (ai != NULL)
    {
    	*res = ai;
    }
    DEBUG("[jsong_debug]*res:0x%x", *res);
    
    return 0;
}

void SPLIBC_freeaddrinfo(struct addrinfo *cahead)
{
    struct addrinfo *canext;
    struct addrinfo *ca;

    DEBUG("[jsong_debug]");

    for(ca = cahead; ca; ca = canext) {
    canext = ca->ai_next;
    SCI_FREE(ca);
    }
    
}


/**
  * @Note: don't change API name /input/ ouput 
*/

/* Create a new socket of type TYPE in domain DOMAIN, using
   protocol PROTOCOL.  If PROTOCOL is zero, one is chosen automatically.
   Returns a file descriptor for the new socket, or -1 for errors.  */
PUBLIC int socket (int __domain, int __type, int __protocol)  
{
    int sock_id = -1;
    DEBUG("[socket debug %s     %d]>>>>>>>>  __domain: %d, __type: %d, __protocol: %d, netid: %d\n",
        __FUNCTION__,__LINE__,  __domain, __type, __protocol, socket_getNetId());
    if (__domain == 0)
    {
        __domain = AF_INET;
    }
    __protocol = 0; //unisoc added
    if (__type == SOCK_STREAM)
    {
        sock_id = sci_sock_socket(__domain, __type, __protocol, socket_getNetId());
        if (sock_id  != -1)
        {
            DEBUG("[socket ok %s     %d]sci_sock_asyncselect start\n",__FUNCTION__,__LINE__);
            //sci_sock_asyncselect(sock_id, SCI_IdentifyThread(), AS_READ | AS_WRITE | AS_CONNECT | AS_CLOSE);
            DEBUG("[socket ok %s     %d]sci_sock_asyncselect end\n",__FUNCTION__,__LINE__);
        }
        else
        {
            DEBUG("[socket error %s     %d]\n",__FUNCTION__,__LINE__);
        }
    }
    else if (__type == SOCK_DGRAM)
    {
        sock_id = sci_sock_socket(__domain, __type, __protocol, socket_getNetId());
        if (sock_id != -1)
        {
            //sci_sock_asyncselect(sock_id, SCI_IdentifyThread(), AS_READ | AS_WRITE);
            DEBUG("[socket ok %s     %d]\n",__FUNCTION__,__LINE__);
        }
        else
        {
            DEBUG("[socket error %s     %d]\n",__FUNCTION__,__LINE__);
        }
    }
    //set the socket is block mod
    //if (sci_sock_setsockopt(sock_id, SO_BIO, PNULL) == -1)
    //{
        //DEBUG("[socket error %s     %d]\n",__FUNCTION__,__LINE__);
       // sci_sock_socketclose(sock_id);
    //}
    DEBUG("[socket debug %s     %d]sock_id is 0x%x , fd:0x%x <<<<<<<<<  \n", __FUNCTION__, __LINE__, sock_id, TO_SOCKET_FD(sock_id));
    return TO_SOCKET_FD(sock_id);
}

PUBLIC int bind(int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len)
{
    int ret = 0;
    DEBUG("[socket debug %s     %d ] __fd:0x%x, id:0x%x >>>>>>>>  \n",__FUNCTION__,__LINE__,  __fd, TO_SCI_SOCKET_ID(__fd));

    ret= sci_sock_bind(TO_SCI_SOCKET_ID(__fd), (struct sockaddr*)__addr, __len);
    
    SET_SOCKERRNO(sock_errno(__fd));

    return ret;

}

PUBLIC int select(int fd_count, fd_set* read_fds, fd_set* write_fds, 
	fd_set* error_fds, struct timeval* tv) 
{
    long time_out = (PNULL == tv) ? -1 : tv->tv_sec * 10; // Note : unit is 0.1s
    DEBUG("[socket debug %s     %d    ]  time_out  : %d  \n",__FUNCTION__,__LINE__,time_out);

    return sci_sock_select(read_fds, write_fds, error_fds, time_out);
}

PUBLIC int accept(int __fd, struct sockaddr *addr, socklen_t *addrlen)
{
    int ret = 0;

    ret = sci_sock_accept(TO_SCI_SOCKET_ID(__fd), addr, addrlen);

    SET_SOCKERRNO(sock_errno(__fd));

    return ret ;

}
 
PUBLIC int listen(int __fd, int backlog)
{
    int ret = 0;
    
    ret = sci_sock_listen(TO_SCI_SOCKET_ID(__fd), backlog);

    SET_SOCKERRNO(sock_errno(__fd));

    return ret ;
}

PUBLIC int sock_errno(int __fd)
{
	return sci_sock_errno(TO_SCI_SOCKET_ID(__fd));
}
#if 0
PUBLIC void sock_seterrno(int __fd, int __errno)
{
	sci_sock_seterrno(TO_SCI_SOCKET_ID(__fd), __errno);
}
#endif
/* Set socket FD's option OPTNAME at protocol level LEVEL
   to *OPTVAL (which is OPTLEN bytes long).
   Returns 0 on success, -1 for errors.  */
PUBLIC int setsockopt (int __fd, int __level, int __optname,
		       const void *__optval, socklen_t __optlen)  
{
    DEBUG("[socket debug %s     %d]>>>>>>>>  __level: %d,  __optname: 0x%x\n",__FUNCTION__,__LINE__, __level, __optname);
    DEBUG("[socket debug %s     %d  __fd:0x%x, id:0x%x >>>>>>>>  \n",__FUNCTION__,__LINE__, __fd, TO_SCI_SOCKET_ID(__fd));
    return sci_sock_setsockopt(TO_SCI_SOCKET_ID(__fd), __optname, (void *)__optval);
}

/* Put the current value for socket FD's option OPTNAME at protocol level LEVEL
   into OPTVAL (which is *OPTLEN bytes long), and set *OPTLEN to the value's
   actual length.  Returns 0 on success, -1 for errors.  */
PUBLIC int getsockopt (int __fd, int __level, int __optname,
		       void *__restrict __optval,
		       socklen_t *__restrict __optlen)  
{
    int ret = 0;
    int error_no = 0;
    DEBUG("[socket debug %s     %d]>>>>>>>>   __level: %d,  __optname: 0x%x\n",__FUNCTION__,__LINE__, __level, __optname);
    DEBUG("[socket debug %s     %d  __fd:0x%x, id:0x%x >>>>>>>>  \n",__FUNCTION__,__LINE__, __fd, TO_SCI_SOCKET_ID(__fd));
    ret = sci_sock_getsockopt(TO_SCI_SOCKET_ID(__fd), __optname,  __optval);
    if (SO_ERROR == __optname)
    {
	//error_no = socket_ErrorMap(*((int*)__optval));
	//*((int*)__optval) = error_no;
    }
    return ret;
}

/* Open a connection on socket FD to peer at ADDR (which LEN bytes long).
   For connectionless socket types, just set the default address to send to
   and the only address from which to accept transmissions.
   Return 0 on success, -1 for errors.

   This function is a cancellation point and therefore not marked with
    .  */
PUBLIC int connect (int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len)
{
    int ret = 0;

    DEBUG("[socket debug %s     %d ] __fd:0x%x, id:0x%x >>>>>>>>  \n",__FUNCTION__,__LINE__, __fd, TO_SCI_SOCKET_ID(__fd));
    
    ret =  sci_sock_connect(TO_SCI_SOCKET_ID(__fd), __addr, __len);

    SET_SOCKERRNO(sock_errno(__fd));

    return ret ;
}

/* Read N bytes into BUF through socket FD.
   If ADDR is not NULL, fill in *ADDR_LEN bytes of it with tha address of
   the sender, and store the actual size of the address in *ADDR_LEN.
   Returns the number of bytes read or -1 for errors.

   This function is a cancellation point and therefore not marked with
    .  */
PUBLIC ssize_t recvfrom (int __fd, void *__restrict __buf, size_t __n,
			 int __flags, __SOCKADDR_ARG __addr,
			 socklen_t *__restrict __addr_len)
{
    int ret = 0;
    
    DEBUG("[socket debug %s     %d  __fd:0x%x, id:0x%x >>>>>>>>  \n",__FUNCTION__,__LINE__, __fd, TO_SCI_SOCKET_ID(__fd));

    ret =  sci_sock_recvfrom(TO_SCI_SOCKET_ID(__fd), __buf, __n, __flags,  __addr);

    SET_SOCKERRNO(sock_errno(__fd));

    return ret ;
}

/* Send N bytes of BUF on socket FD to peer at address ADDR (which is
   ADDR_LEN bytes long).  Returns the number sent, or -1 for errors.

   This function is a cancellation point and therefore not marked with
    .  */
PUBLIC ssize_t sendto (int __fd, const void *__buf, size_t __n,
		       int __flags, __CONST_SOCKADDR_ARG __addr,
		       socklen_t __addr_len)
{
    int ret = 0;

    DEBUG("[socket debug %s     %d  __fd:0x%x, id:0x%x >>>>>>>>  \n",__FUNCTION__,__LINE__, __fd, TO_SCI_SOCKET_ID(__fd));

    ret =  sci_sock_sendto(TO_SCI_SOCKET_ID(__fd), __buf, __n, __flags, __addr);

    SET_SOCKERRNO(sock_errno(__fd));

    return ret ;
}

/* Receive a message as described by MESSAGE from socket FD.
   Returns the number of bytes read or -1 for errors.

   This function is a cancellation point and therefore not marked with
    .  */
PUBLIC ssize_t recvmsg (int __fd, struct msghdr *__message, int __flags)
{
    int i = 0;
    ssize_t  recv = 0;
    ssize_t  recv_size = 0;
    
    SCI_TRACE_LOW("[socket debug %s     %d  __fd:0x%x, socket:0x%x  msg_iovlen=%d  __flags=0x%x>>>>>>>>  \n",__FUNCTION__,__LINE__, __fd, TO_SCI_SOCKET_ID(__fd), __message->msg_iovlen, __flags);
    socketSaveDataToFile("\nstart socket recv data\n",  strlen("\nstart socket recv data\n"));	
    
    for (i = 0; i< __message->msg_iovlen; i++)
    {
        SCI_TRACE_LOW(" socket debug  __message->msg_iov[i].iov_len : %d",__message->msg_iov[i].iov_len);
        recv = sci_sock_recv(TO_SCI_SOCKET_ID(__fd), __message->msg_iov[i].iov_base, __message->msg_iov[i].iov_len, __flags);

        SET_SOCKERRNO(sock_errno(__fd));
        
        if (recv == -1)
        {
            if (recv_size != 0)
            {
                DEBUG("socket debug %s: %d, recv_size: %d\n",__FUNCTION__,__LINE__,recv_size);
                return recv_size;
            }
            else
            {
                return -1;
            }
        } 
        if (recv > 0)
        {
            recv_size += recv;
        }
        socketSaveDataToFile(__message->msg_iov[i].iov_base,  __message->msg_iov[i].iov_len);	
    }
    socketSaveDataToFile("\nend socket recv data\n",  strlen("\nend socket recv data\n"));
    DEBUG("socket debug %s: %d, recv_size: %d\n",__FUNCTION__,__LINE__,recv_size);
    return recv_size;
}


/* Send a message described MESSAGE on socket FD.
   Returns the number of bytes sent, or -1 for errors.

   This function is a cancellation point and therefore not marked with
    .  */
PUBLIC ssize_t sendmsg (int __fd, const struct msghdr *__message,
			int __flags)
{
    int i = 0;
    ssize_t  send_size = 0;
    DEBUG("[socket debug %s     %d  __fd:0x%x, id:0x%x >>>>>>>>  \n",__FUNCTION__,__LINE__, __fd, TO_SCI_SOCKET_ID(__fd));
    socketSaveDataToFile("\nstart socket send data\n",  strlen("\nstart socket send data\n"));
    for (i = 0; i< __message->msg_iovlen; i++)
    {
        ssize_t send = 0;
        send = sci_sock_send(TO_SCI_SOCKET_ID(__fd), __message->msg_iov[i].iov_base, __message->msg_iov[i].iov_len, __flags);

        SET_SOCKERRNO(sock_errno(__fd));

        if (send == -1)
        {   
            if (send_size == 0)
            {
                return -1;
            }

            return send_size;
        }
      
        send_size += send; 
        socketSaveDataToFile(__message->msg_iov[i].iov_base,  __message->msg_iov[i].iov_len);
    }
    socketSaveDataToFile("\nend socket send data\n",  strlen("\nend socket send data\n"));
    DEBUG("socket debug %s: %d , send_size : %d\n",__FUNCTION__,__LINE__,send_size);
    return send_size;
}

/* Send N bytes of BUF to socket FD.  Returns the number sent or -1.

   This function is a cancellation point and therefore not marked with
    .  */
PUBLIC ssize_t send (int __fd, const void *__buf, size_t __n, int __flags)
{
    int ret = 0;

    DEBUG("[socket debug %s     %d  __fd:0x%x, id:0x%x ----->>>>>>>>  \n",__FUNCTION__,__LINE__, __fd, TO_SCI_SOCKET_ID(__fd));

    ret =  sci_sock_send(TO_SCI_SOCKET_ID(__fd), (void *)__buf, __n, __flags);

    SET_SOCKERRNO(sock_errno(__fd));

    return ret ;
}

/* Read N bytes into BUF from socket FD.
   Returns the number read or -1 for errors.

   This function is a cancellation point and therefore not marked with
    .  */
PUBLIC ssize_t recv (int __fd, void *__buf, size_t __n, int __flags)
{
    int ret = 0;

    DEBUG("[socket debug %s     %d  __fd:0x%x, id:0x%x ----->>>>>>>>  \n",__FUNCTION__,__LINE__, __fd, TO_SCI_SOCKET_ID(__fd));

    ret =   sci_sock_recv(TO_SCI_SOCKET_ID(__fd), __buf, __n, __flags);

    SET_SOCKERRNO(sock_errno(__fd));

    return ret ;
}


PUBLIC ssize_t read(int __fd, void *men, size_t len)
{
    struct sci_sockaddrext    from;
    return sci_sock_recvfrom(TO_SCI_SOCKET_ID(__fd), men, len, NULL, &from);
}


PUBLIC ssize_t writev(int __fd, const struct iovec *iov, int iovcnt)
{
    struct msghdr msg;

    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov = CONST_CAST(struct iovec *, iov);
    msg.msg_iovlen = iovcnt;
    msg.msg_control = NULL;
    msg.msg_controllen = 0;
    msg.msg_flags = 0;

    return sendmsg(__fd, &msg, 0);
}

int fcntl(int s, int cmd, int val)
{
    DEBUG("[socket] socket fcntl(%d, CMD: %d, val:%d) \n", s, cmd, val);
    return sci_sock_fcntl(TO_SCI_SOCKET_ID(s), cmd, val);
}

/* Put the local address of FD into *ADDR and its length in *LEN.  */
PUBLIC int getsockname (int __fd, __SOCKADDR_ARG __addr,
			socklen_t *__restrict __len) 
{
    int ret = sci_sock_getsockname(TO_SCI_SOCKET_ID(__fd), __addr);

    DEBUG("[socket debug %s     %d ] __fd:0x%x, id:0x%x >>>>>>>>  \n",__FUNCTION__,__LINE__,  __fd, TO_SCI_SOCKET_ID(__fd));
    DEBUG("[socket debug]ret:%d,  ip:%s", ret, inet_ntoa(((struct sci_sockaddr*) __addr)->ip_addr));
    return ret;
}

PUBLIC int close(int __fd)
{
    int ret = 0;
    DEBUG("[socket debug %s     %d  ]>>>>>>>>  \n",__FUNCTION__,__LINE__);

    DEBUG("[socket debug %s     %d  __fd:0x%x, id:0x%x >>>>>>>>  \n",__FUNCTION__,__LINE__, __fd, TO_SCI_SOCKET_ID(__fd));

    ret =  sci_sock_socketclose(TO_SCI_SOCKET_ID(__fd));

    SET_SOCKERRNO(sock_errno(__fd));

    return ret ;
}

PUBLIC int shutdown (int __fd, int __how) 
{
    int ret = 0;
    DEBUG("[socket debug %s     %d ] >>>>>>>>  \n",__FUNCTION__,__LINE__);

    DEBUG("[socket debug %s     %d  __fd:0x%x, id:0x%x >>>>>>>>  \n",__FUNCTION__,__LINE__, __fd, TO_SCI_SOCKET_ID(__fd));

    ret =  sci_sock_shutdown(TO_SCI_SOCKET_ID(__fd), __how);

    SET_SOCKERRNO(sock_errno(__fd));

    return ret ;
}

PUBLIC int socketpair (int __domain, int __type, int __protocol,
		       int __fds[2]) 
{
    DEBUG("[socket debug %s     %d]need to do  \n",__FUNCTION__,__LINE__);
    return 0;
}

PUBLIC int inet_pton_std(int af, const char * src, void * dst)
{
    int ret = inet_pton(af, src, dst);
    //mapping return value to standand defination 
    if (ret == 0)
        return 1;
    else if (ret == 1)
        return -1;
    else
        return 0;
}

PUBLIC int poll( struct pollfd *fds, nfds_t nfds, int timeout)
{
	sci_fd_set read_fds;
	sci_fd_set write_fds;
	int left_len = 0;
	int i = 0;
	int ret = 0;

	DEBUG("[socket debug %s     %d]nfds: %d , timeout: %d\n",__FUNCTION__,__LINE__,nfds,timeout);
	SCI_FD_ZERO(&read_fds);
	SCI_FD_ZERO(&write_fds);
	
	for (i = 0; i < nfds; i++)
	{
		DEBUG("[socket debug %s     %d]i : %d, fd: 0x%x, 0x%x, events: %d\n",__FUNCTION__,__LINE__,i, fds[i].fd, TO_SCI_SOCKET_ID(fds[i].fd), fds[i].events);
		if (fds[i].events&POLLIN)
		{
			SCI_FD_SET(TO_SCI_SOCKET_ID(fds[i].fd), &read_fds );
		}
		if (fds[i].events&POLLOUT)
		{
			SCI_FD_SET(TO_SCI_SOCKET_ID(fds[i].fd), &write_fds );
		}
	}
	
	ret = sci_sock_select(&read_fds, &write_fds, NULL, timeout);
	DEBUG("[socket debug %s     %d]ret : %d\n",__FUNCTION__,__LINE__,ret);
	if (ret > 0)
	{
		for (i = 0; i < nfds; i++)
		{
			if (SCI_FD_ISSET(TO_SCI_SOCKET_ID(fds[i].fd), &read_fds) > 0)
			{
				fds[i].revents = fds[i].revents|POLLIN;
			}
			if (SCI_FD_ISSET(TO_SCI_SOCKET_ID(fds[i].fd), &write_fds) > 0)
			{
				fds[i].revents = fds[i].revents|POLLOUT;
			}
			DEBUG("[socket debug %s     %d]i: %d, revents : 0x%x\n",__FUNCTION__,__LINE__,i, fds[i].revents);
		}
	}
	return ret;
}

PUBLIC void socketSaveDataToFile(char* buff_ptr, int32 len)
{
    uint16     file_name[]   = {'E',':','\\','s','c','o','k','.','t','x','t',0};
    SFS_HANDLE file_handle   = 0;
    uint32 	   bytes_written = 0;

    if(buff_ptr == PNULL || len < 1)
    {
        return;
    }

    //save socket data to file
    if(g_socket_data_save)
    {
        file_handle = SFS_CreateFile(file_name, SFS_MODE_OPEN_ALWAYS | SFS_MODE_WRITE| SFS_MODE_APPEND,0,0);/*lint !e655*/      
        if (file_handle == 0)
        {
            DEBUG("socket socketSaveDataToFile create file error!");
        }
        else
        {
            SFS_WriteFile(file_handle, buff_ptr, len, &bytes_written, 0);   
            SFS_CloseFile(file_handle);
        }
    }   
}
#ifdef   __cplusplus
    }
#endif

