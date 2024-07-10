/**
 ***********************************************************************************************************************
 * Copyright (c) 2020, China Mobile Communications Group Co.,Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
 * an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations under the License.
 *
 * @file        socket.c
 *
 * @brief       Implement socket functions
 *
 * @revision
 * Date         Author          Notes
 * 2020-03-21   OneOS Team      First Version
 ***********************************************************************************************************************
 */
#include "os_types.h"
#include "sys/socket.h"
#include "netdb.h"
#include "app_tcp_if.h"

// extern uint32 MMIAPIPDP_GetPublicPdpLinkNetid(void);
int os_socket(int domain, int type, int protocol)
{
    int sci_domain = domain;
    int sci_type = type;
    int sci_protocol = protocol;
    //TCP : protocol must set to 0 for 8910ff socket
    if (sci_protocol == IPPROTO_TCP)
    {
        sci_protocol = 0;
    }

    int fd = sci_sock_socket(sci_domain, sci_type, sci_protocol, 0);
    SCI_TRACE_LOW("oneos:sci_domain=%d, sci_type=%d, sci_protocol=%d, fd=0x%x", sci_domain, sci_type, sci_protocol, fd);
    return fd;
}

int os_closesocket(int fd)
{
    TCPIP_SOCKET_T sci_so = (TCPIP_SOCKET_T)fd;
    SCI_TRACE_LOW("oneos:os_closesocket fd=0x%x", fd);
    return sci_sock_socketclose(sci_so);
}

int os_connect(int fd, const struct sockaddr *name, socklen_t namelen)
{
    TCPIP_SOCKET_T sci_so = (TCPIP_SOCKET_T)fd;

    struct sockaddr_in *sa1 =  (struct sockaddr_in *)name;
    struct sci_sockaddr sci_sock;
    sci_sock.family = sa1->sin_family;
    sci_sock.port = sa1->sin_port;
    sci_sock.ip_addr = sa1->sin_addr.s_addr;
    SCI_TRACE_LOW("oneos:family=%d, port=%d, addr=%s", sci_sock.family, sci_sock.port, inet_ntoa(sci_sock.ip_addr));
    return sci_sock_connect(sci_so, (struct sci_sockaddr*)&sci_sock, namelen);
}
int os_sendto(int fd, const void *data, size_t size, int flags, const struct sockaddr *to, socklen_t tolen)
{
    TCPIP_SOCKET_T sci_so = (TCPIP_SOCKET_T)fd;
    char* sci_buf = (char*)data;
    int sci_len = (int)size;
    int sci_flag = flags;
    struct sci_sockaddr* sci_to = (struct sci_sockaddr*)to;

    return sci_sock_sendto(sci_so, sci_buf, sci_len, sci_flag, sci_to);
}
int os_send(int fd, const void *data, size_t size, int flags)
{
    TCPIP_SOCKET_T sci_so = (TCPIP_SOCKET_T)fd;
    char* sci_buf = (char*)data;
    int sci_len = (int)size;
    int sci_flag = flags;

    return sci_sock_send(sci_so, sci_buf, sci_len, sci_flag);
}
int os_recvfrom(int fd, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen)
{
    TCPIP_SOCKET_T sci_so = (TCPIP_SOCKET_T)fd;
    char* sci_buf = (char*)mem;
    int sci_len = (int)len;
    int sci_flag = flags;
    struct sci_sockaddr* sci_from = (struct sci_sockaddr*)from;

    return sci_sock_recvfrom(sci_so, sci_buf, sci_len, sci_flag, sci_from);
}
int os_recv(int fd, void *mem, size_t len, int flags)
{
    TCPIP_SOCKET_T sci_so = (TCPIP_SOCKET_T)fd;
    char* sci_buf = (char*)mem;
    int sci_len = (int)len;
    int sci_flag = flags;
    // SCI_TRACE_LOW("oneos:os_recv fd=0x%x", fd);
    return sci_sock_recv(sci_so, sci_buf, sci_len, sci_flag);
}

/*
isreadset: 1,need set readset
iswriteset: 1,need set iswriteset
isexceptset: 1,need set isexceptset
timeout_ms: timeout, ms
*/
int os_select(int fd, os_bool_t isreadset, os_bool_t iswriteset, os_bool_t isexceptset, int timeout_ms)
{
    struct sci_fd_set sci_readset;
    struct sci_fd_set* in = OS_NULL;
    struct sci_fd_set sci_writeset;
    struct sci_fd_set* out = OS_NULL;
    struct sci_fd_set sci_exceptset;
    struct sci_fd_set* ex = OS_NULL;
    /*==============================================
     * for 8910ff platform
     * set select timeout to 0,
     * when > 0, "System Timer Thread" occupy cpu to 5%
     *=============================================*/
    long tv = 0;

    if (isreadset == OS_TRUE)
    {
        SCI_FD_ZERO(&sci_readset);
        SCI_FD_SET(fd, &sci_readset);
        in = &sci_readset;
        // SCI_TRACE_LOW("oneos:fd=0x%x, isreadset=%d", fd, SCI_FD_ISSET(fd, &sci_readset));
    }
    if (iswriteset == OS_TRUE)
    {
        SCI_FD_ZERO(&sci_writeset);
        SCI_FD_SET(fd, &sci_writeset);
        out = &sci_writeset;
    }
    if (isexceptset == OS_TRUE)
    {
        SCI_FD_ZERO(&sci_exceptset);
        SCI_FD_SET(fd, &sci_exceptset);
        ex = &sci_exceptset;
    }

    return sci_sock_select(in, out, ex, tv);
}

int os_setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen)
{
    /* support SO_SNDTIMEO only
    setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&interval, sizeof(struct timeval));
    */
    if (optname == SO_SNDTIMEO)
    {
        int is_block = 0;
        return sci_sock_setsockopt((TCPIP_SOCKET_T)fd, SO_NONBLOCK, (void *)(&is_block));
    }
    else
    {
        return OS_PARAM_ERROR;
    }
}
struct hostent *os_gethostbyname(const char *name)
{
    return sci_gethostbyname(name);
}

int os_getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)
{
    int ai_family;
    int port_nr = 0;
    TCPIP_IPADDR_T addr;
    unsigned long total_size;
    unsigned long namelen = 0;
    struct addrinfo* ai = NULL;
    struct sockaddr_storage* sa = NULL;
    struct sockaddr_in* sa4 = NULL;
    struct sci_hostent* hostent = NULL;

    if (NULL == res)
    {
        return EAI_FAIL;
    }

    *res = NULL;
    if ((NULL == node) && (NULL == service))
    {
        return EAI_NONAME;
    }

    if (NULL != hints)
    {
        ai_family = hints->ai_family;
        // SCI_TRACE_LOW("oneos:ai_family=%d", ai_family);
        if (ai_family != AF_UNSPEC && ai_family != AF_INET)
        {
            return EAI_FAMILY;
        }
    }


    if (service != NULL)
    {
        /* service name specified: convert to port number */
        port_nr = atoi(service);
        // SCI_TRACE_LOW("oneos:port_nr=%d", port_nr);
        if ((port_nr <= 0) || (port_nr > 0xffff))
        {
            return EAI_SERVICE;
        }
    }

    if (node != NULL)
    {
        /* service location specified, try to resolve */
        if ((hints != NULL) && (hints->ai_flags & AI_NUMERICHOST))
        {
            /* no DNS lookup, just parse for an address string */
            if (!inet_aton(node, &addr))
            {
                return EAI_NONAME;
            }

            if (AF_INET == ai_family)
            {
                return EAI_NONAME;
            }
        }
        else
        {
            // SCI_TRACE_LOW("oneos:call sci_gethostbyname...");
            hostent = sci_gethostbyname(node);
            if (NULL == hostent)
            {
                return EAI_FAIL;
            }
            addr = (*((struct in_addr *)(hostent->h_addr))).s_addr;
        }
    }
    else
    {
        return EAI_NONAME;
    }

    // SCI_TRACE_LOW("oneos:end call sci_gethostbyname...");
    total_size = sizeof(struct addrinfo) + sizeof(struct sockaddr_storage);
    if (node != NULL)
    {
        namelen = strlen(node);
        if (namelen > MAX_DNS_LEN)
        {
            /* invalid name length */
            // SCI_TRACE_LOW("oneos:invalid name length");
            return EAI_FAIL;
        }
        SCI_ASSERT(total_size + namelen + 1 > total_size);
        total_size += namelen + 1;
    }

    SCI_ASSERT(total_size <= sizeof(struct addrinfo) + sizeof(struct sockaddr_storage) + MAX_DNS_LEN + 1);
    ai = SCI_CALLOC(1, total_size);
    if (NULL == ai)
    {
        // SCI_TRACE_LOW("oneos:ai calloc failed...");
        return EAI_MEMORY;
    }

    /* cast through void* to get rid of alignment warnings */
    sa = (struct sockaddr_storage *)(void *)((unsigned char *)ai + sizeof(struct addrinfo));
    sa4 = (struct sockaddr_in *) sa;
    /* set up sockaddr */

    sa4->sin_addr.s_addr = addr;
    // SCI_TRACE_LOW("oneos:addr=%s", inet_ntoa(sa4->sin_addr.s_addr));
    //sa4->sin_addr = *((struct in_addr *)(hostent->h_addr));//addr;
    sa4->sin_family = AF_INET;
    sa4->sin_port   = htons((unsigned short)port_nr);
    ai->ai_family   = AF_INET;

        /* set up addrinfo */
    if (hints != NULL)
    {
        /* copy socktype & protocol from hints if specified */
        ai->ai_socktype = hints->ai_socktype;
        ai->ai_protocol = hints->ai_protocol;
    }
    if (node != NULL)
    {
        /* copy nodename to canonname if specified */
        ai->ai_canonname = ((char *) ai + sizeof(struct addrinfo) + sizeof(struct sockaddr_storage));
        memcpy(ai->ai_canonname, node, namelen);
        ai->ai_canonname[namelen] = 0;
    }
    ai->ai_addrlen = sizeof(struct sockaddr_storage);
    ai->ai_addr = (struct sockaddr *) sa;
    ai->ai_next = NULL;

    *res = ai;

    // SCI_TRACE_LOW("oneos:end sci_sock_getaddrinfo...");
    return 0;
}

void os_freeaddrinfo(struct addrinfo *ai)
{
    struct addrinfo *next = NULL;
    while (ai != NULL)
    {
        next = ai->ai_next;
        free(ai);
        ai = next;
    }
}