/****************************************************************************
 ** File Name:      cafsocket.h                                             *
 ** Author:         James.Zhang                                             *
 ** Date:           10/16/2008                                              *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
 ** Description:    This file is used to describe the application modual.   *
 ****************************************************************************
 **                         Important Edit History                          *
 ** ------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                             *
 ** 10/2008        James.Zhang       Create
 ** 
 ****************************************************************************/

#ifndef  _CAFSOCKET_H_    
#define  _CAFSOCKET_H_  

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
   extern   "C"
   {
#endif

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "cafhelp.h"

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

#define ISOCKET_SUCCESS          (0)   //successful operation
#define ISOCKET_ERROR            (-1)   //unsuccessful operation
#define ISOCKET_WOULDBLOCK       (-2)

//socket error
#define ISOCKET_ENOBUFS        1
#define ISOCKET_ETIMEDOUT      2
#define ISOCKET_EISCONN        3
#define ISOCKET_EOPNOTSUPP     4
#define ISOCKET_ECONNABORTED   5
#define ISOCKET_EWOULDBLOCK    6
#define ISOCKET_ECONNREFUSED   7
#define ISOCKET_ECONNRESET     8
#define ISOCKET_ENOTCONN       9
#define ISOCKET_EALREADY       10
#define ISOCKET_EINVAL         11
#define ISOCKET_EMSGSIZE       12
#define ISOCKET_EPIPE          13
#define ISOCKET_EDESTADDRREQ   14
#define ISOCKET_ESHUTDOWN      15
#define ISOCKET_ENOPROTOOPT    16
#define ISOCKET_EHAVEOOB       17
#define ISOCKET_ENOMEM         18
#define ISOCKET_EADDRNOTAVAIL  19
#define ISOCKET_EADDRINUSE     20
#define ISOCKET_EAFNOSUPPORT   21
#define ISOCKET_EINPROGRESS    22
#define ISOCKET_ELOWER         23    // lower layer (IP) error 
#define ISOCKET_ENOTSOCK       24    // Includes sockets which closed while blocked 
#define ISOCKET_EIEIO          27    // bad input/output on Old Macdonald's farm :-)
#define ISOCKET_ETOOMANYREFS   28
#define ISOCKET_EFAULT         29
#define ISOCKET_ENETUNREACH    30
#define ISOCKET_UNKNOWERR      255

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
typedef struct INETMGR_T INETMGR_T;
typedef struct ISOCKET_T ISOCKET_T;

/*
 * Family Address families.
 */
#define  CAF_AF_UNSPEC   0     /* unspecified */
#define  CAF_AF_NS       1     /* local to host (pipes, portals) */
#define  CAF_AF_INET     2     /* internetwork: UDP, TCP, etc. */

#define  CAF_INADDR_ANY              (uint32)0x00000000

/*
 * Socket Types
 */
#define     ISOCKET_SOCK_STREAM    1     /* stream socket */
#define     ISOCKET_SOCK_DGRAM     2     /* datagram socket */
#define     ISOCKET_SOCK_RAW       3     /* raw-protocol interface */

/*
 * Protocols
 */
#define IPPROTO_IP              0               /* dummy for IP */
#define IPPROTO_ICMP            1               /* control message protocol */
#define IPPROTO_IGMP            2               /* internet group management protocol */
#define IPPROTO_GGP             3               /* gateway^2 (deprecated) */
#define IPPROTO_TCP             6               /* tcp */
#define IPPROTO_PUP             12              /* pup */
#define IPPROTO_UDP             17              /* user datagram protocol */
#define IPPROTO_IDP             22              /* xns idp */
#define IPPROTO_ND              77              /* UNOFFICIAL net disk proto */

#define IPPROTO_RAW             255             /* raw IP packet */
#define IPPROTO_MAX             256

/*
 * Option flags per-socket.
 */
#define     ISOCKET_SO_DEBUG       0x0001      /* turn on debugging info recording */
#define     ISOCKET_SO_ACCEPTCONN  0x0002      /* socket has had listen() */
#define     ISOCKET_SO_REUSEADDR   0x0004      /* allow local address reuse */
#define     ISOCKET_SO_KEEPALIVE   0x0008      /* keep connections alive */
#define     ISOCKET_SO_DONTROUTE   0x0010      /* just use interface addresses */
#define     ISOCKET_SO_BROADCAST   0x0020      /* permit sending of broadcast msgs */
#define     ISOCKET_SO_USELOOPBACK 0x0040      /* bypass hardware when possible */
#define     ISOCKET_SO_LINGER      0x0080      /* linger on close if data present */
#define     ISOCKET_SO_OOBINLINE   0x0100      /* leave received OOB data in line */
#define     ISOCKET_SO_TCPSACK     0x0200      /* Allow TCP SACK (Selective acknowledgment) */
#define     ISOCKET_SO_WINSCALE    0x0400      /* Set scaling window option */ 
#define     ISOCKET_SO_TIMESTAMP   0x0800      /* Set TCP timestamp option */ 
#define     ISOCKET_SO_BIGCWND     0x1000      /* Large initial TCP Congenstion window */ 
#define     ISOCKET_SO_HDRINCL     0x2000      /* user access to IP hdr for SOCK_RAW */
#define     ISOCKET_SO_NOSLOWSTART 0x4000      /* suppress slowstart on this socket */
#define     ISOCKET_SO_FULLMSS     0x8000      /* force packets to all be MAX size */

/* for compatability with second-rate stacks: */
#define     ISOCKET_SO_EXPEDITE     SO_NOSLOWSTART
#define     ISOCKET_SO_THROUGHPUT   SO_FULLMSS

/*
 * Additional options, not kept in so_options.
 */
#define     ISOCKET_SO_SNDBUF      0x1001      /* send buffer size */
#define     ISOCKET_SO_RCVBUF      0x1002      /* receive buffer size */
#define     ISOCKET_SO_SNDLOWAT    0x1003      /* send low-water mark */
#define     ISOCKET_SO_RCVLOWAT    0x1004      /* receive low-water mark */
#define     ISOCKET_SO_SNDTIMEO    0x1005      /* send timeout */
#define     ISOCKET_SO_RCVTIMEO    0x1006      /* receive timeout */
#define     ISOCKET_SO_ERROR       0x1007      /* get error status and clear */
#define     ISOCKET_SO_TYPE        0x1008      /* get socket type */
#define     ISOCKET_SO_HOPCNT      0x1009      /* Hop count to get to dst   */
#define     ISOCKET_SO_MAXMSG      0x1010      /* get TCP_MSS (max segment size) */

/* ...And some netport additions to setsockopt: */
#define     ISOCKET_SO_RXDATA      0x1011      /* get count of bytes in sb_rcv */
#define     ISOCKET_SO_TXDATA      0x1012      /* get count of bytes in sb_snd */
#define     ISOCKET_SO_MYADDR      0x1013      /* return my IP address */
#define     ISOCKET_SO_NBIO        0x1014      /* set socket into NON-blocking mode */
#define     ISOCKET_SO_BIO         0x1015      /* set socket into blocking mode */
#define     ISOCKET_SO_NONBLOCK    0x1016      /* set/get blocking mode via optval param */
#define     ISOCKET_SO_CALLBACK    0x1017      /* set/get zero_copy callback routine */

/*
 * Max connect number
*/
#define ISOSKET_SOMAXCONN           5

/*
 * Shutdown how
*/

#define ISOCKET_SHUTDOWN_RD         0
#define ISOCKET_SHUTDOWN_WR         1
#define ISOCKET_SHUTDOWN_RDWR       2



/* Berkeley style "Socket address" */
typedef struct
{
   uint16    family;     /* address family */
   uint16    port;		 /* port number */
   uint32    ip_addr;    /* ip address */  
   char      sa_data[8]; /* up to 14 bytes of direct address */
}ISOCKET_ADDR_T;

#define ISOCKET_FD_SETSIZE          12


typedef struct 
{
    uint32          fd_count;                       /* how many are SET? */
    ISOCKET_T         *fd_array[ISOCKET_FD_SETSIZE];   /* an array of SOCKETs */
                                                    /* current system only support 12 */
}ISOCKET_FD_SET_T;


struct  hostent {
    char    * h_name;               /* official name of host */
    char    ** h_aliases;           /* alias list */
    short   h_addrtype;             /* host address type */
    short   h_length;               /* length of address */
    char    ** h_addr_list;         /* list of addresses */
#define h_addr  h_addr_list[0]      /* address, for backward compat */
};


CAF_INTERFACE(INETMGR_T)
{
    INHERIT_IUNKNOWN(INETMGR_T);
    struct hostent *(*GetHostByName)(INETMGR_T * netmgr_ptr, const char * psz);
    ISOCKET_T     *(*OpenSocket)(INETMGR_T * netmgr_ptr, int32 family, int32 type, int32 protocol);
    int32         (*Select)(INETMGR_T * netmgr_ptr, int32 nfds, ISOCKET_FD_SET_T* in, ISOCKET_FD_SET_T* out, ISOCKET_FD_SET_T* ex, int32 timeout );
    int32         (*GetHostName)(INETMGR_T * netmgr_ptr, char * psz, int32 namelen);
};

// Define pointer to point the virtual table of the control base class
struct INETMGR_T
{
    CAF_VTBL_T(INETMGR_T)  *vtbl_ptr;
};

#define FD_CLR(socket_ptr, set) do { \
    uint32 i; \
    for (i = 0; i < ((ISOCKET_FD_SET_T *)(set))->fd_count ; i++) { \
        if (((ISOCKET_FD_SET_T *)(set))->fd_array[i] == socket_ptr) { \
            while (i < ((ISOCKET_FD_SET_T *)(set))->fd_count-1) { \
                ((ISOCKET_FD_SET_T *)(set))->fd_array[i] = \
                    ((ISOCKET_FD_SET_T *)(set))->fd_array[i+1]; \
                i++; \
            } \
            ((ISOCKET_FD_SET_T *)(set))->fd_count--; \
            break; \
        } \
    } \
} while(0)

#define FD_SET(socket_ptr, set) do { \
    if (((ISOCKET_FD_SET_T *)(set))->fd_count < ISOCKET_FD_SETSIZE) \
        ((ISOCKET_FD_SET_T *)(set))->fd_array[((ISOCKET_FD_SET_T *)(set))->fd_count++]=(socket_ptr);\
} while(0)

#define FD_ZERO(set) (((ISOCKET_FD_SET_T *)(set))->fd_count=0)

static __inline int32 FD_ISSET(ISOCKET_T *socket_ptr, ISOCKET_FD_SET_T *set) 
{
    uint32   i;
    for (i = 0; i < set->fd_count ; i++) 
    {
        if (set->fd_array[i] == socket_ptr)
            return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
// 	Description : netmgr query interface
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
static __inline CAF_RESULT_E INETMGR_QueryInterface( INETMGR_T *netmgr_ptr, CAF_GUID_T guid, void ** object_pptr ) 
{
    CAF_ASSERT( PNULL != netmgr_ptr );
    return CAF_GET_VTBLPTR( netmgr_ptr, INETMGR_T )->QueryInterface( netmgr_ptr, guid, object_pptr );
}

/*****************************************************************************/
// 	Description : netmgr add ref
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
static __inline uint32 INETMGR_AddRef( INETMGR_T *netmgr_ptr ) 
{
    CAF_ASSERT( PNULL != netmgr_ptr );
    return CAF_GET_VTBLPTR( netmgr_ptr, INETMGR_T )->AddRef( netmgr_ptr );
}

/*****************************************************************************/
// 	Description : netmgr release
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
static __inline uint32 INETMGR_Release( INETMGR_T *netmgr_ptr ) 
{
    CAF_ASSERT( PNULL != netmgr_ptr );
    return CAF_GET_VTBLPTR( netmgr_ptr, INETMGR_T )->Release( netmgr_ptr );
}

/*****************************************************************************/
// 	Description : CAF encapsulation of GetHostByName
//	Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//	Note:
/*****************************************************************************/
static __inline struct hostent * INETMGR_GetHostByName( INETMGR_T *netmgr_ptr, const char * psz) 
{
    CAF_ASSERT( PNULL != netmgr_ptr );
    return CAF_GET_VTBLPTR( netmgr_ptr, INETMGR_T )->GetHostByName( netmgr_ptr, psz );
}

/*****************************************************************************/
// 	Description : CAF encapsulation of OpenSocket
//	Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//	Note:
/*****************************************************************************/
static __inline ISOCKET_T * INETMGR_OpenSocket( INETMGR_T *netmgr_ptr, int32 family, int32 type, int32 protocol)
{
    CAF_ASSERT( PNULL != netmgr_ptr );
    return CAF_GET_VTBLPTR( netmgr_ptr, INETMGR_T )->OpenSocket( netmgr_ptr, family, type, protocol );
}

/*****************************************************************************/
// 	Description : CAF encapsulation of Select
//	Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//	Note:
/*****************************************************************************/
static __inline int32 INETMGR_Select( INETMGR_T *netmgr_ptr, int32 nfds, ISOCKET_FD_SET_T* in, ISOCKET_FD_SET_T* out, ISOCKET_FD_SET_T* ex, int32 timeout)
{
    CAF_ASSERT( PNULL != netmgr_ptr );
    return CAF_GET_VTBLPTR( netmgr_ptr, INETMGR_T )->Select( netmgr_ptr, nfds, in, out, ex, timeout );
}

/*****************************************************************************/
// 	Description : CAF encapsulation of GetSockOpt
//	Global resource dependence : 
//  Author:Wei.zhou
//	Note:
/*****************************************************************************/
static __inline int32 INETMGR_GetHostName( INETMGR_T *netmgr_ptr, char * psz, int32 namelen)
{
    CAF_ASSERT( PNULL != netmgr_ptr );
    return CAF_GET_VTBLPTR( netmgr_ptr, INETMGR_T )->GetHostName( netmgr_ptr, psz, namelen );
}


CAF_INTERFACE(ISOCKET_T)
{
    INHERIT_IUNKNOWN(ISOCKET_T);
    int32         (*Accept)(ISOCKET_T * socket_ptr, ISOCKET_T **socket_pptr);
    int32         (*Bind)(ISOCKET_T * socket_ptr, ISOCKET_ADDR_T* addr_ptr, int32 addr_len );
    int32         (*Close)(ISOCKET_T * socket_ptr);
    int32         (*Connect)(ISOCKET_T * socket_ptr, ISOCKET_ADDR_T* addr_ptr, int32 addr_len );
    int32         (*GetPeerName)(ISOCKET_T * socket_ptr, ISOCKET_ADDR_T* addr_ptr, int32* addr_len_ptr );
    int32         (*GetSockName)(ISOCKET_T * socket_ptr, ISOCKET_ADDR_T* addr_ptr, int32* addr_len_ptr );
    int32         (*GetOpt)(ISOCKET_T * socket_ptr, int32 level, int32 opt_name, char* opt_ptr, int32* opt_len_ptr );
    int32         (*GetLastError)(ISOCKET_T * socket_ptr);
    int32         (*IOCtl)(ISOCKET_T * socket_ptr, int32 cmd, uint32* param );
    int32         (*Listen)(ISOCKET_T * socket_ptr, int32 back_log ); 
    int32         (*Recv)(ISOCKET_T * socket_ptr, char* buf_ptr, int32 len, int32 flag ); 
    int32         (*RecvFrom)(ISOCKET_T * socket_ptr, char* buf_ptr, int32 len, int32 flag, ISOCKET_ADDR_T* from, int32* from_len );
    int32         (*Send)(ISOCKET_T* socket_ptr, const char *buf_ptr, int32 len, int32 flag );    
    int32         (*SendTo)(ISOCKET_T* socket_ptr, const char* buf_ptr, int32 len, int32 flag, const ISOCKET_ADDR_T* to, int32 to_len );
    int32         (*SetOpt)( ISOCKET_T* socket_ptr, int32 level, int32 opt_name, const char *opt_ptr, int32 opt_len );
    int32         (*Shutdown)( ISOCKET_T* socket_ptr, int32 how );
    int32         (*RegAsynSocketEvent)(ISOCKET_T *socket_ptr, CAF_HANDLE_T dest_handle, uint16 msg_id);
};

// Define pointer to poit the virtual table of the control base class
struct ISOCKET_T
{
    CAF_VTBL_T(ISOCKET_T)  *vtbl_ptr;
};

/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : socket query interface
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
static __inline CAF_RESULT_E ISOCKET_QueryInterface( ISOCKET_T *socket_ptr, CAF_GUID_T guid, void ** object_pptr ) 
{
    CAF_ASSERT( PNULL != socket_ptr );
    return CAF_GET_VTBLPTR( socket_ptr, ISOCKET_T )->QueryInterface( socket_ptr, guid, object_pptr );
}

/*****************************************************************************/
// 	Description : socket add ref
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
static __inline uint32 ISOCKET_AddRef( ISOCKET_T *socket_ptr ) 
{
    CAF_ASSERT( PNULL != socket_ptr );
    return CAF_GET_VTBLPTR( socket_ptr, ISOCKET_T )->AddRef( socket_ptr );
}

/*****************************************************************************/
// 	Description : socket release
//	Global resource dependence : 
//  Author:wei.zhou
//	Note:
/*****************************************************************************/
static __inline uint32 ISOCKET_Release( ISOCKET_T *socket_ptr ) 
{
    CAF_ASSERT( PNULL != socket_ptr );
    return CAF_GET_VTBLPTR( socket_ptr, ISOCKET_T )->Release( socket_ptr );
}

/*****************************************************************************/
// 	Description : CAF encapsulation of Accept
//	Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//	Note:
/*****************************************************************************/
static __inline int32 ISOCKET_Accept( ISOCKET_T* socket_ptr, ISOCKET_T** socket_pptr)
{
    CAF_ASSERT( PNULL != socket_ptr );
    return CAF_GET_VTBLPTR( socket_ptr, ISOCKET_T )->Accept( socket_ptr, socket_pptr);
}

/*****************************************************************************/
// 	Description : CAF encapsulation of Bind
//	Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//	Note:
/*****************************************************************************/
static __inline int32 ISOCKET_Bind( ISOCKET_T* socket_ptr, ISOCKET_ADDR_T* addr_ptr, int32 addr_len )
{
    CAF_ASSERT( PNULL != socket_ptr );
    return CAF_GET_VTBLPTR( socket_ptr, ISOCKET_T )->Bind( socket_ptr, addr_ptr, addr_len );
}

/*****************************************************************************/
// 	Description : CAF encapsulation of CloseSocket
//	Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//	Note:
/*****************************************************************************/
static __inline int32 ISOCKET_CloseSocket( ISOCKET_T* socket_ptr )
{
    CAF_ASSERT( PNULL != socket_ptr );
    return CAF_GET_VTBLPTR( socket_ptr, ISOCKET_T )->Close( socket_ptr );
}

/*****************************************************************************/
// 	Description : CAF encapsulation of Connect
//	Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//	Note:
/*****************************************************************************/
static __inline int32 ISOCKET_Connect( ISOCKET_T* socket_ptr, ISOCKET_ADDR_T* addr_ptr, int32 addr_len )
{
    CAF_ASSERT( PNULL != socket_ptr );
    return CAF_GET_VTBLPTR( socket_ptr, ISOCKET_T )->Connect( socket_ptr, addr_ptr, addr_len );
}

/*****************************************************************************/
// 	Description : CAF encapsulation of GetPeerName
//	Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//	Note:
/*****************************************************************************/
static __inline int32 ISOCKET_GetPeerName( ISOCKET_T* socket_ptr, ISOCKET_ADDR_T* addr_ptr, int32* addr_len_ptr )
{
    CAF_ASSERT( PNULL != socket_ptr );
    return CAF_GET_VTBLPTR( socket_ptr, ISOCKET_T )->GetPeerName( socket_ptr, addr_ptr, addr_len_ptr );
}

/*****************************************************************************/
// 	Description : CAF encapsulation of GetSockName
//	Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//	Note:
/*****************************************************************************/
static __inline int32 ISOCKET_GetSockName( ISOCKET_T* socket_ptr, ISOCKET_ADDR_T* addr_ptr, int32* addr_len_ptr )
{
    CAF_ASSERT( PNULL != socket_ptr );
    return CAF_GET_VTBLPTR( socket_ptr, ISOCKET_T )->GetSockName( socket_ptr, addr_ptr, addr_len_ptr );
}

/*****************************************************************************/
// 	Description : CAF encapsulation of GetSockOpt
//	Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//	Note:
/*****************************************************************************/
static __inline int32 ISOCKET_GetSockOpt( ISOCKET_T* socket_ptr, int32 level, int32 opt_name, char* opt_ptr, int32* opt_len_ptr )
{
    CAF_ASSERT( PNULL != socket_ptr );
    return CAF_GET_VTBLPTR( socket_ptr, ISOCKET_T )->GetOpt( socket_ptr, level, opt_name, opt_ptr, opt_len_ptr );
}

/*****************************************************************************/
// 	Description : CAF encapsulation of Listen
//	Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//	Note:
/*****************************************************************************/
static __inline int32 ISOCKET_Listen( ISOCKET_T* socket_ptr, int32 back_log )
{
    CAF_ASSERT( PNULL != socket_ptr );
    return CAF_GET_VTBLPTR( socket_ptr, ISOCKET_T )->Listen( socket_ptr, back_log );
}

/*****************************************************************************/
// 	Description : CAF encapsulation of Recv
//	Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//	Note:
/*****************************************************************************/
static __inline int32 ISOCKET_Recv( ISOCKET_T* socket_ptr, char* buf_ptr, int32 len, int32 flag )
{
    CAF_ASSERT( PNULL != socket_ptr );
    return CAF_GET_VTBLPTR( socket_ptr, ISOCKET_T )->Recv( socket_ptr, buf_ptr, len, flag );
}

/*****************************************************************************/
// 	Description : CAF encapsulation of RecvFrom
//	Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//	Note:
/*****************************************************************************/
static __inline int32 ISOCKET_RecvFrom( ISOCKET_T* socket_ptr, char* buf_ptr, int32 len, int32 flag, ISOCKET_ADDR_T* from, int32* from_len )
{
    CAF_ASSERT( PNULL != socket_ptr );
    return CAF_GET_VTBLPTR( socket_ptr, ISOCKET_T )->RecvFrom( socket_ptr, buf_ptr, len, flag, from, from_len );
}

/*****************************************************************************/
// 	Description : CAF encapsulation of Send
//	Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//	Note:
/*****************************************************************************/
static __inline int32 ISOCKET_Send( ISOCKET_T* socket_ptr, const char *buf_ptr, int32 len, int32 flag )
{
    CAF_ASSERT( PNULL != socket_ptr );
    return CAF_GET_VTBLPTR( socket_ptr, ISOCKET_T )->Send( socket_ptr, buf_ptr, len, flag );
}

/*****************************************************************************/
// 	Description : CAF encapsulation of SendTo
//	Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//	Note:
/*****************************************************************************/
static __inline int32 ISOCKET_SendTo( ISOCKET_T* socket_ptr, const char* buf_ptr, int32 len, int32 flag, const ISOCKET_ADDR_T* to, int32 to_len )
{
    CAF_ASSERT( PNULL != socket_ptr );
    return CAF_GET_VTBLPTR( socket_ptr, ISOCKET_T )->SendTo( socket_ptr, buf_ptr, len, flag, to, to_len );
}

/*****************************************************************************/
// 	Description : CAF encapsulation of SetSockOpt
//	Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//	Note:
/*****************************************************************************/
static __inline int32 ISOCKET_SetSockOpt( ISOCKET_T* socket_ptr, int32 level, int32 opt_name, const char *opt_ptr, int32 opt_len )
{
    CAF_ASSERT( PNULL != socket_ptr );
    return CAF_GET_VTBLPTR( socket_ptr, ISOCKET_T )->SetOpt( socket_ptr, level, opt_name, opt_ptr, opt_len );
}

/*****************************************************************************/
// 	Description : CAF encapsulation of Shutdown
//	Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//	Note:
/*****************************************************************************/
static __inline int32 ISOCKET_Shutdown( ISOCKET_T* socket_ptr, int32 how )
{
    CAF_ASSERT( PNULL != socket_ptr );
    return CAF_GET_VTBLPTR( socket_ptr, ISOCKET_T )->Shutdown( socket_ptr, how );
}

/*****************************************************************************/
// 	Description : CAF encapsulation of GetLastError
//	Global resource dependence : 
//  Author:James.Zhang
//  Modify:Wei.zhou
//	Note:
/*****************************************************************************/
static __inline int32 ISOCKET_GetLastError( ISOCKET_T* socket_ptr )
{
    CAF_ASSERT( PNULL != socket_ptr );
    return CAF_GET_VTBLPTR( socket_ptr, ISOCKET_T )->GetLastError( socket_ptr );
}

/*****************************************************************************/
// 	Description : CAF asynchronous event register
//	Global resource dependence : 
//  Author:Wei.zhou
//	Note:
/*****************************************************************************/
static __inline int32 ISOCKET_RegAsynSocketEvent(ISOCKET_T *socket_ptr, CAF_HANDLE_T dest_handle, uint16 msg_id)
{
    CAF_ASSERT( PNULL != socket_ptr );
    return CAF_GET_VTBLPTR( socket_ptr, ISOCKET_T )->RegAsynSocketEvent( socket_ptr, dest_handle, msg_id );
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif //_CAFSOCKET_H_
