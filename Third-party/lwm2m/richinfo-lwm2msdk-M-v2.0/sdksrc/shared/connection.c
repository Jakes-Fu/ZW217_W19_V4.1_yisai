/*******************************************************************************
 *
 * Copyright (c) 2013, 2014 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    David Navarro, Intel Corporation - initial API and implementation
 *    Pascal Rieux - Please refer to git log
 *    
 *******************************************************************************/

#include <stdlib.h>
#include <string.h>
//#include <ctype.h>
#include "connection.h"
#if 1 //modify by unisoc
#include "lwm2mclient.h"
#endif

// from commandline.c
//void output_buffer(FILE * stream, uint8_t * buffer, int length, int indent);
extern int getaddrinfo(CONST char *nodename, CONST char *servname, CONST struct addrinfo *hints, struct addrinfo **res);
extern void freeaddrinfo(struct addrinfo *ai);

int create_socket(const char * portStr, int addressFamily)
{
    int s = -1;
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *p;
    int ret = 0;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = addressFamily;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    ret = getaddrinfo(NULL, portStr, &hints, &res);
    if (0 != ret)
    {
        LOG_M("Failed portStr:%s, addressFamily=%d, ret=%d\n", portStr, addressFamily, ret);
        return -1;
    }

    for(p = res ; p != NULL && s == -1 ; p = p->ai_next)
    {
        s = dm_socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (s >= 0)
        {
            if (-1 == dm_bind(s, p->ai_addr, p->ai_addrlen))
            {
                dm_close(s);
                s = -1;
            }
        }

        if (s >= 0)
        {
            ret = sci_sock_asyncselect(TO_SCI_SOCKET_ID(s), SCI_IdentifyThread(), AS_READ);
            if (0 != ret)
            {
                dm_close(s);
                s = -1;
            }
        }
    }

    freeaddrinfo(res);

    LOG_M("create_socket socket=%x\n", s);
    return s;
}

connection_t * connection_find(connection_t * connList,
                               struct sockaddr_storage * addr,
                               size_t addrLen)
{
    connection_t * connP;
    connP = connList;
    LOG_M("connP=%p", connP);
    while (connP != NULL)
    {        
#if 0 //modify by unisoc
        if ((connP->addrLen == addrLen)
         && (memcmp(&(connP->addr), addr, addrLen) == 0))
#else
        if (memcmp(&(connP->addr), addr, connP->addrLen) == 0)
#endif
        {
            return connP;
        }
        connP = connP->next;
        LOG_M("connP=%p", connP);
    }

    LOG_M("connection_find connP=%p", connP);
    return connP;
}

connection_t * connection_new_incoming(connection_t * connList,
                                       int sock,
                                       struct sockaddr * addr,
                                       size_t addrLen)
{
    connection_t * connP;

    connP = (connection_t *)malloc(sizeof(connection_t));
    if (connP != NULL)
    {
        connP->sock = sock;    
        #if 0 //unisoc modified
        memcpy(&(connP->addr), addr, addrLen);
        #else
        if (sizeof(connP->addr) >= addrLen)
        {
            memcpy(&(connP->addr), addr, addrLen);
        }
        else
        {
            LOG_M("Error sizeof(connP->addr)=%d < addrLen=%d", sizeof(connP->addr), addrLen);
        }
        #endif
        connP->addrLen = addrLen;
        connP->next = connList;
    }
    LOG_M("connection_new_incoming connP=%p", connP);

    return connP;
}

connection_t * connection_create(connection_t * connList,
                                 int sock,
                                 char * host,
                                 char * port,
                                 int addressFamily)
{
    struct addrinfo hints;
    struct addrinfo *servinfo = NULL;
    struct addrinfo *p;
    int s;
    struct sockaddr *sa = NULL;
    socklen_t sl = 0;
    connection_t * connP = NULL;
    int ret = 0;
#if 1 //unisoc modified
    struct hostent *host_ptr = NULL;
#endif
    LOG_S("connection_create");
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = addressFamily;
    hints.ai_socktype = SOCK_DGRAM;
#if 1 //unisoc modified
    if (addressFamily != AF_INET)
    {
        LOG_M("Error addressFamily =%d (is not AF_INET)!!!\n", addressFamily);
    }
    host_ptr = gethostbynameBlock(host); //add for getaddrinfo => in46_reshost() => dns_query_entry_type() input netif is 0, so can not send upd to dns server
    if (host_ptr == NULL)
    {
        LOG_S("gethostbynameBlock error!!!");
    }
#endif
    ret = getaddrinfo(host, port, &hints, &servinfo);
    if (0 != ret || servinfo == NULL)
    {
        LOG_M("Failed  host=%s, port=%s, ret=%d, addressFamily=%d\n", host, port, ret, addressFamily);
        return NULL;
    }

    // we test the various addresses
    s = -1;
    for(p = servinfo ; p != NULL && s == -1 ; p = p->ai_next)
    {
        s = dm_socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (s >= 0)
        {
            sa = p->ai_addr;
            sl = p->ai_addrlen;
            if (-1 == dm_connect(s, p->ai_addr, p->ai_addrlen))
            {
                dm_close(s);
                s = -1;
            }
        }
    }
    if (s >= 0)
    {
        connP = connection_new_incoming(connList, sock, sa, sl);
        dm_close(s);
    }
    if (NULL != servinfo) {
        /*free(servinfo);*/
        freeaddrinfo(servinfo);
    }
    LOG_M("connection_create connP=%p", connP);
    return connP;
}

void connection_free(connection_t * connList)
{
    while (connList != NULL)
    {
        connection_t * nextP;

        nextP = connList->next;
        free(connList);

        connList = nextP;
    }
}

int connection_send(connection_t *connP,
                    uint8_t * buffer,
                    size_t length)
{
    int nbSent;
    size_t offset;
    int retry = 0;

#if 1 //unisoc modified
    LOG_M( "Sending %d bytes", length);
#endif

#ifdef WITH_LOGS
    char s[INET6_ADDRSTRLEN];
    in_port_t port;

    s[0] = 0;

    if (AF_INET == connP->addr.sin6_family)
    {
        struct sockaddr_in *saddr = (struct sockaddr_in *)&connP->addr;
        inet_ntop(saddr->sin_family, &saddr->sin_addr, s, INET6_ADDRSTRLEN);
        port = saddr->sin_port;
    }
    else if (AF_INET6 == connP->addr.sin6_family)
    {
        struct sockaddr_in6 *saddr = (struct sockaddr_in6 *)&connP->addr;
        inet_ntop(saddr->sin6_family, &saddr->sin6_addr, s, INET6_ADDRSTRLEN);
        port = saddr->sin6_port;
    }
#if 0 //unisoc modified
    fprintf(stderr, "Sending %d bytes to [%s]:%hu\r\n", length, s, ntohs(port));
#else
    LOG_M( "Sending %d bytes to [%s]:%hu\r\n", length, s, ntohs(port));
#endif
    //output_buffer(stderr, buffer, length, 0);
#endif

    if (socket_getNetId() == 0)
    {
        LOG_S("Warning net id is 0!!!\n");
        return 0;
    }

    offset = 0;
    while (offset != length)
    {
        nbSent = dm_sendto(connP->sock, buffer + offset, length - offset, 0, (struct sockaddr *)&(connP->addr), connP->addrLen);
        if (nbSent == -1) return -1;
        offset += nbSent;
        if (nbSent == 0)
        {
            retry++;
            if (retry > 3)
            {
                LOG_S("Error status\n");
                break;
            }
            SCI_Sleep(10);
        }
    }
    return 0;
}

uint8_t lwm2m_buffer_send(void * sessionH,
                          uint8_t * buffer,
                          size_t length,
                          void * userdata)
{
    connection_t * connP = (connection_t*) sessionH;

    if (connP == NULL)
    {
#if 0 //unisoc modified
        fprintf(stderr, "#> failed sending %lu bytes, missing connection\r\n", length);
#else
        LOG_M("#> failed sending %lu bytes, missing connection\r\n", length);
#endif
        return COAP_500_INTERNAL_SERVER_ERROR ;
    }

    if (-1 == connection_send(connP, buffer, length))
    {
#if 0 //unisoc modified
        fprintf(stderr, "#> failed sending %lu bytes\r\n", length);
#else
        LOG_M("#> failed sending %lu bytes\r\n", length);
#endif
        return COAP_500_INTERNAL_SERVER_ERROR ;
    }

    return COAP_NO_ERROR;
}

bool lwm2m_session_is_equal(void * session1,
                            void * session2,
                            void * userData)
{
    return (session1 == session2);
}
