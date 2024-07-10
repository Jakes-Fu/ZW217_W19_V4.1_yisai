#ifndef __NETDB_H__
#define __NETDB_H__
#include <stddef.h>

#define AF_INET         2

// Socket Types
#define SOCK_STREAM     1           /* stream socket */
#define SOCK_DGRAM      2           /* datagram socket */
#define SOCK_RAW        3           /* raw-protocol interface */

// Socket option flags
#define SO_DEBUG        0x0001      /* turn on debugging info recording */
#define SO_ACCEPTCONN   0x0002      /* socket has had listen() */
#define SO_REUSEADDR    0x0004      /* allow local address reuse */
#define SO_KEEPALIVE    0x0008      /* keep connections alive */
#define SO_DONTROUTE    0x0010      /* just use interface addresses */
#define SO_BROADCAST    0x0020      /* permit sending of broadcast msgs */
#define SO_USELOOPBACK  0x0040      /* bypass hardware when possible */
#define SO_LINGER       0x0080      /* linger on close if data present */
#define SO_OOBINLINE    0x0100      /* leave received OOB data in line */
#define SO_TCPSACK      0x0200      /* Allow TCP SACK (Selective acknowledgment) */
#define SO_WINSCALE     0x0400      /* Set scaling window option */
#define SO_TIMESTAMP    0x0800      /* Set TCP timestamp option */
#define SO_BIGCWND      0x1000      /* Large initial TCP Congenstion window */
#define SO_HDRINCL      0x2000      /* user access to IP hdr for SOCK_RAW */
#define SO_NOSLOWSTART  0x4000      /* suppress slowstart on this socket */
#define SO_FULLMSS      0x8000      /* force packets to all be MAX size */


// for compatability with second-rate stacks/
#define SO_EXPEDITE     SO_NOSLOWSTART
#define SO_THROUGHPUT   SO_FULLMSS


// Additional options, not kept in so_options.
#define SO_SNDBUF       0x1001      /* send buffer size */
#define SO_RCVBUF       0x1002      /* receive buffer size */
#define SO_SNDLOWAT     0x1003      /* send low-water mark */
#define SO_RCVLOWAT     0x1004      /* receive low-water mark */
#define SO_SNDTIMEO     0x1005      /* send timeout */
#define SO_RCVTIMEO     0x1006      /* receive timeout */
#define SO_ERROR        0x1007      /* get error status and clear */
#define SO_TYPE         0x1008      /* get socket type */
#define SO_HOPCNT       0x1009      /* Hop count to get to dst   */
#define SO_MAXMSG       0x1010      /* get maximum message size, NOTE: it's only for
                                       message-oriented socket types (e.g. SOCK_DGRAM),
                                       no meaning for stream oriented sockets. */

#define  SOL_SOCKET  0xfff    /* options for socket level */
#define IPPROTO_IP      0
#define IPPROTO_ICMP    1
#define IPPROTO_TCP     6
#define IPPROTO_UDP     17

#define EAI_NONAME  200
#define EAI_SERVICE 201
#define EAI_FAIL    202
#define EAI_MEMORY  203
#define EAI_FAMILY  204

/* input flags for struct addrinfo */
#define AI_PASSIVE      0x01
#define AI_CANONNAME    0x02
#define AI_NUMERICHOST  0x04
#define AI_NUMERICSERV  0x08
#define AI_V4MAPPED     0x10
#define AI_ALL          0x20
#define AI_ADDRCONFIG   0x40

#define MAX_DNS_LEN     256

#define SCI_FD_SETSIZE  12

#if !defined(socklen_t) && !defined(SOCKLEN_T_DEFINED)
typedef unsigned int socklen_t;
#endif

#if !defined(sa_family_t) && !defined(SA_FAMILY_T_DEFINED)
typedef unsigned char sa_family_t;
#endif

/* If your port already typedef's in_port_t, define IN_PORT_T_DEFINED to prevent this code from redefining it. */
#if !defined(in_port_t) && !defined(IN_PORT_T_DEFINED)
typedef unsigned short in_port_t;
#endif

#if !defined(in_addr_t) && !defined(IN_ADDR_T_DEFINED)
typedef unsigned int in_addr_t;
#endif

struct in_addr
{
    in_addr_t s_addr;
};

struct hostent
{
    char  *h_name;            /* Official name of the host. */
    char **h_aliases;         /* A pointer to an array of pointers to alternative host names, */
                              /* terminated by a null pointer. */
    int    h_addrtype;        /* Address type. */
    int    h_length;          /* The length, in bytes, of the address. */
    char **h_addr_list;       /* A pointer to an array of pointers to network addresses */
                              /* (in network byte order) for the host,terminated by a null pointer */
#define h_addr h_addr_list[0] /* for backward compatibility */
};

struct sockaddr
{
    unsigned char sa_len;
    sa_family_t sa_family;
    char        sa_data[14];
};

struct sockaddr_in
{
    unsigned char  sin_len;
    sa_family_t    sin_family;
    in_port_t      sin_port;
    struct in_addr sin_addr;
#define SIN_ZERO_LEN 8
    char           sin_zero[SIN_ZERO_LEN];
};

struct addrinfo
{
    int              ai_flags;     /* Input flags. */
    int              ai_family;    /* Address family of socket. */
    int              ai_socktype;  /* Socket type. */
    int              ai_protocol;  /* Protocol of socket. */
    socklen_t        ai_addrlen;   /* Length of socket address. */
    struct sockaddr *ai_addr;      /* Socket address of socket. */
    char            *ai_canonname; /* Canonical name of service location. */
    struct addrinfo *ai_next;      /* Pointer to next in list. */
};

struct sockaddr_storage
{
    unsigned char  s2_len;
    sa_family_t ss_family;
    char        s2_data1[2];
    unsigned int s2_data2[3];
};

struct timeval {
    long    tv_sec;         /* seconds */
    long    tv_usec;        /* and microseconds */
};

#if 0// defined(OS_USING_POSIX) && defined(OS_USING_VFS)

#ifdef FD_SETSIZE
#undef FD_SETSIZE
#endif

#define FD_SETSIZE      VFS_FD_MAX
#endif

// #ifndef   FD_SETSIZE
// #define  FD_SETSIZE  32
// #endif

// /* Number of bits in a byte. */
// #define   NBBY    8

// typedef long    fd_mask;

// /* Bits per mask. */
// #define   NFDBITS (sizeof (fd_mask) * NBBY)

// #ifndef   howmany
// #define  howmany(x,y)    (((x) + ((y) -1 )) / (y))
// #endif

// /* We use a macro for fd_set so that including Sockets.h afterwards can work. */
// typedef struct _types_fd_set
// {
//     fd_mask fds_bits[howmany(FD_SETSIZE, NFDBITS)];
// } _types_fd_set;

// #define fd_set _types_fd_set

// #define   FD_SET(n, p)    ((p)->fds_bits[(n)/NFDBITS] |= (1L << ((n) % NFDBITS)))
// #define   FD_CLR(n, p)    ((p)->fds_bits[(n)/NFDBITS] &= ~(1L << ((n) % NFDBITS)))
// #define   FD_ISSET(n, p)  ((p)->fds_bits[(n)/NFDBITS] & (1L << ((n) % NFDBITS)))
// #define   FD_ZERO(p)       memset((void*)(p), 0, sizeof(*(p)))

#define PP_HTONS(x) ((((x)&0x00ffUL) << 8) | (((x)&0xff00UL) >> 8))
#endif