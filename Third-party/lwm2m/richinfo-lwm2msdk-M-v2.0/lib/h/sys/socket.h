/* Declarations of socket constants, types, and functions.
   Copyright (C) 1991-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef	_SYS_SOCKET_H
#define	_SYS_SOCKET_H	1
#include "socket_api.h"
#include "fake.h"

#if defined(__cplusplus)
#define	__BEGIN_DECLS		extern "C" {
#define	__END_DECLS		}
#else
#define	__BEGIN_DECLS
#define	__END_DECLS
#endif

#ifdef IPV6_SUPPORT
#define IP_V6 1
#endif

#define TO_SCI_SOCKET_ID(fd) ((fd << 1) & 0xFFFFFFFE)//((fd & 0x7FFFFFFE) | fd << 31)
#define TO_SOCKET_FD(id)     ((id >> 1) & 0x7FFFFFFF)//((id & 0x7FFFFFFF) |(id >>31))

#ifndef AI_PASSIVE
#define AI_PASSIVE	0x0001	/* Socket address is intended for `bind'.  */
#endif

#ifndef _SS_SIZE
#define _SS_SIZE	128
#endif
#ifndef _SS_PADSIZE
#define _SS_PADSIZE	(_SS_SIZE - (2 * sizeof (__ss_aligntype)))
#endif

#if ULONG_MAX > 0xffffffff
#ifndef __ss_aligntype
#define __ss_aligntype	__uint64_t
#endif
#else
#ifndef __ss_aligntype
#define __ss_aligntype	unsigned int
#endif
#endif

typedef	unsigned short    __uint16_t;

#ifndef _IN_PORT_T
typedef	__uint16_t	in_port_t;
#define	_IN_PORT_T
#endif

#define INET_ADDRSTRLEN    16    /* length of printfed IPv4 addr */
#define INET6_ADDRSTRLEN   46    /* max length of printed IPv6 addr */

#define	__SOCKADDR_COMMON(sa_prefix) \
  sa_family_t sa_prefix##family


typedef unsigned int  socklen_t;
typedef long		ssize_t;
/* Type used for the number of file descriptors.  */
typedef unsigned long int nfds_t;
typedef unsigned char sa_family_t;
typedef  uint16     u_short;

__BEGIN_DECLS

struct in_addr2        //modify cmccdm before was in_addr
{
   unsigned long  s_addr;
};

struct sockaddr_in
{
   short    sin_family;
   u_short  sin_port;
   struct   in_addr2  sin_addr;  //modify cmccdm before was in_addr
   char     sin_zero[8];
};

struct sockaddr_storage
  {
    __SOCKADDR_COMMON (ss_);	/* Address family, etc.  */
    __ss_aligntype __ss_align;	/* Force desired alignment.  */
    char __ss_padding[_SS_PADSIZE];
  };

/* IPv6 address */
struct in6_addr
{
    union
    {
    uint8_t __u6_addr8[16];
#ifdef __USE_MISC
    uint16_t __u6_addr16[8];
    uint32_t __u6_addr32[4];
#endif
    } __in6_u;
#define s6_addr			__in6_u.__u6_addr8
#ifdef __USE_MISC
# define s6_addr16		__in6_u.__u6_addr16
# define s6_addr32		__in6_u.__u6_addr32
#endif
};

struct sockaddr_in6
{
   unsigned short        sin6_family;   /* AF_INET6 */
   unsigned short        sin6_port;     /* transport layer port # */
   unsigned long         sin6_flowinfo; /* IPv6 flow information */
   struct in6_addr       sin6_addr;     /* IPv6 address */
   int            sin6_scope_id; /* set of interfaces for a scope */
};


/* Data structure describing a polling request.  */
struct pollfd
{
    int fd;			/* File descriptor to poll.  */
    short int events;		/* Types of events poller cares about.  */
    short int revents;		/* Types of events that actually occurred.  */
};

/* Structure for scatter/gather I/O.  */
struct iovec
{
	void *iov_base;	/* Pointer to data.  */
	size_t iov_len;	/* Length of data.  */
};

/* Description of data base entry for a single host.  */
struct hostent
{
	char *h_name;			/* Official name of host.  */
	char **h_aliases;		/* Alias list.  */
	int h_addrtype;		/* Host address type.  */
	int h_length;			/* Length of address.  */
	char **h_addr_list;		/* List of addresses from name server.  */
#ifdef __USE_MISC
# define	h_addr	h_addr_list[0] /* Address, for backward compatibility.*/
#endif
};

/* Structure describing messages sent by
   `sendmsg' and received by `recvmsg'.  */
struct msghdr
{
void *msg_name;		/* Address to send to/receive from.  */
socklen_t msg_namelen;	/* Length of address data.  */

struct iovec *msg_iov;	/* Vector of data to send/receive into.  */
size_t msg_iovlen;		/* Number of elements in the vector.  */

void *msg_control;		/* Ancillary data (eg BSD filedesc passing). */
size_t msg_controllen;	/* Ancillary data buffer length.
			   !! The type should be socklen_t but the
			   definition of the kernel is incompatible
			   with this.  */

int msg_flags;		/* Flags on received message.  */
};

struct sockaddr
{
   uint16     sa_family;     /* address family */
#ifdef IP_V6   /* V6 only or dual stacks */
   char     sa_data[32];      /* big enough for unpacked sockaddr_in6 */
#else          /* Ancient IPv4 only version */
   char     sa_data[14];      /* up to 14 bytes of direct address */
#endif
};

struct addrinfo {
  int     ai_flags;     /* AI_PASSIVE, AI_CANONNAME,
                           AI_NUMERICHOST, .. */
  int     ai_family;    /* AF_xxx */
  int     ai_socktype;  /* SOCK_xxx */
  int     ai_protocol;  /* 0 or IPPROTO_xxx for IPv4 and IPv6 */
  int     ai_addrlen;   /* length of ai_addr */
  char   *ai_canonname; /* canonical name for nodename */
  struct sockaddr  *ai_addr; /* binary address */
  struct addrinfo  *ai_next; /* next structure in linked list */
};

/* This is the type we use for generic socket address arguments.

   With GCC 2.7 and later, the funky union causes redeclarations or
   uses with any of the listed types to be allowed without complaint.
   G++ 2.7 does not support transparent unions so there we want the
   old-style declaration, too.  */
#if defined __cplusplus || !defined __USE_GNU
# define __SOCKADDR_ARG		struct sockaddr *__restrict
# define __CONST_SOCKADDR_ARG	const struct sockaddr *
#else
/* Add more `struct sockaddr_AF' types here as necessary.
   These are all the ones I found on NetBSD and Linux.  */
# define __SOCKADDR_ALLTYPES \
  __SOCKADDR_ONETYPE (sockaddr) \
  __SOCKADDR_ONETYPE (sockaddr_at) \
  __SOCKADDR_ONETYPE (sockaddr_ax25) \
  __SOCKADDR_ONETYPE (sockaddr_dl) \
  __SOCKADDR_ONETYPE (sockaddr_eon) \
  __SOCKADDR_ONETYPE (sockaddr_in) \
  __SOCKADDR_ONETYPE (sockaddr_in6) \
  __SOCKADDR_ONETYPE (sockaddr_inarp) \
  __SOCKADDR_ONETYPE (sockaddr_ipx) \
  __SOCKADDR_ONETYPE (sockaddr_iso) \
  __SOCKADDR_ONETYPE (sockaddr_ns) \
  __SOCKADDR_ONETYPE (sockaddr_un) \
  __SOCKADDR_ONETYPE (sockaddr_x25)

# define __SOCKADDR_ONETYPE(type) struct type *__restrict __##type##__;
typedef union { __SOCKADDR_ALLTYPES
	      } __SOCKADDR_ARG __attribute__ ((__transparent_union__));
# undef __SOCKADDR_ONETYPE
# define __SOCKADDR_ONETYPE(type) const struct type *__restrict __##type##__;
typedef union { __SOCKADDR_ALLTYPES
	      } __CONST_SOCKADDR_ARG __attribute__ ((__transparent_union__));
# undef __SOCKADDR_ONETYPE
#endif


PUBLIC void socket_SetNetId(unsigned int id);
PUBLIC unsigned int socket_getNetId(void);
struct hostent *gethostbynameBlock(const char *hostname);

/**
  * @Note: don't change API name /input/ ouput 
*/

/* Create a new socket of type TYPE in domain DOMAIN, using
   protocol PROTOCOL.  If PROTOCOL is zero, one is chosen automatically.
   Returns a file descriptor for the new socket, or -1 for errors.  */

PUBLIC int dm_socket (int __domain, int __type, int __protocol);

int dm_bind(int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len);

/* Open a connection on socket FD to peer at ADDR (which LEN bytes long).
   For connectionless socket types, just set the default address to send to
   and the only address from which to accept transmissions.
   Return 0 on success, -1 for errors.

   This function is a cancellation point and therefore not marked with
    .  */
PUBLIC int dm_connect (int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len);

/* Read N bytes into BUF through socket FD.
   If ADDR is not NULL, fill in *ADDR_LEN bytes of it with tha address of
   the sender, and store the actual size of the address in *ADDR_LEN.
   Returns the number of bytes read or -1 for errors.

   This function is a cancellation point and therefore not marked with
    .  */
PUBLIC ssize_t dm_recvfrom (int __fd, void *__restrict __buf, size_t __n,
			 int __flags, __SOCKADDR_ARG __addr,
			 socklen_t *__restrict __addr_len);

/* Send N bytes of BUF on socket FD to peer at address ADDR (which is
   ADDR_LEN bytes long).  Returns the number sent, or -1 for errors.

   This function is a cancellation point and therefore not marked with
    .  */
PUBLIC ssize_t dm_sendto (int __fd, const void *__buf, size_t __n,
		       int __flags, __CONST_SOCKADDR_ARG __addr,
		       socklen_t __addr_len);

PUBLIC int dm_close(int __fd);

__END_DECLS

#endif /* sys/socket.h */
