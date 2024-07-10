/*
 * Copyright (c) 2014-2016 Alibaba Group. All rights reserved.
 * License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
//#include <sys/select.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <netdb.h>
//#include <unistd.h>
//#include <pthread.h>
#include "mbedtls_import_coap.h"
#include "tcpip/socket_api.h"
#include "mbedtls_import_dtls.h"//modified by joker.yang of sprd

//#define //HAL_UDP_TRACE(fmt,...)     SCI_TRACE_LOW("%s::%s:"fmt,"joker-UDP",__FUNCTION__,##__VA_ARGS__)

//设置net_id
static unsigned int s_net_id = 0;
void  HAL_UDP_set_net_id(unsigned int id) {
    //HAL_UDP_TRACE("set netid = %d",id);
    s_net_id = id;
}

int ALIPAY_MBEDTLS_SYMBOL(HAL_UDP_create)(void *p_socket)
{
    int flag = 1;
    int ret = -1;
    int sockfd = -1;
    BLOCK_ID task_id = SCI_IdentifyThread();

    if(NULL == p_socket){
        //HAL_UDP_TRACE("error, input param p_socket is NULL");
        return -1;
    }

    if((sockfd = sci_sock_socket(AF_INET, SOCK_DGRAM, 0,s_net_id)) < 0){
        //HAL_UDP_TRACE("error, socket create failed sockfd = %d",sockfd);
        return -1;
    }
	
    ret = sci_sock_setsockopt(sockfd, SO_NBIO, 0);
    if(ret < 0){
        //HAL_UDP_TRACE("error, set socket opt failed ret = %d",ret);
        sci_sock_socketclose(sockfd);
        return -1;
    }
    
    ret = sci_sock_asyncselect(sockfd, task_id, AS_READ); 
    if(0 != ret){
        //HAL_UDP_TRACE("error, sci_sock_asyncselect failed ret = %d",ret);
        return -1;
    }

    *(int *)p_socket  = sockfd;
    //HAL_UDP_TRACE("success");
    return 0;
}

void ALIPAY_MBEDTLS_SYMBOL(HAL_UDP_close)(void *p_socket)
{

    int socket_id = -1;
    //HAL_UDP_TRACE("enter");
    if(NULL != p_socket){
        socket_id = *(int *)p_socket;
        sci_sock_socketclose(socket_id);
    }
}

int ALIPAY_MBEDTLS_SYMBOL(HAL_UDP_write)(void               *p_socket,
                       const coap_address_t    *p_remote,
                       const unsigned char    *p_data,
                       unsigned int            datalen)
{
    int rc = -1;
    int socket_id = -1;
    struct sci_sockaddr remote_addr;

    if(NULL == p_socket) {
        //HAL_UDP_TRACE("error, input param p_socket is NULL");
        return -1;
    }

    socket_id = *((int *)p_socket);
    remote_addr.family = AF_INET;
    remote_addr.port = htons(p_remote->port);
    if(1 != (rc = inet_aton(p_remote->addr, &remote_addr.ip_addr)))
    {
        //HAL_UDP_TRACE("inet_aton failed");
        return -1;
    }
    rc = sci_sock_sendto(socket_id, p_data, (size_t)datalen, 0,(const struct sci_sockaddr *)&remote_addr);
    //HAL_UDP_TRACE("rc  = %d",rc);
    if(-1 == rc)
    {
        return -1;
    }
    return rc;
}

static int register_socket_read_event(int socket_id){
    int ret = 0;
    BLOCK_ID task_id = SCI_IdentifyThread();
    ret = sci_sock_asyncselect(socket_id, task_id, AS_READ); 
    //HAL_UDP_TRACE("register socket read event returned %d",ret);
    return ret;
}
int ALIPAY_MBEDTLS_SYMBOL(HAL_UDP_read)(void                *p_socket,
                              coap_address_t   *p_remote,
                              unsigned char   *p_data,
                              unsigned int     datalen)
{
    int socket_id = -1;
    struct sci_sockaddr from;
    int count = -1;
    int left = 0;

    if(NULL == p_remote  || NULL == p_data || NULL == p_socket)
    {
        //HAL_UDP_TRACE("error, params check failed");
        return -1;
    }

    socket_id = *(int *)p_socket;
    //count = sci_sock_recvfrom(socket_id, p_data, (size_t)datalen, 0, &from);
    count = sci_sock_asyncrecvfrom(socket_id, p_data, (size_t)datalen, 0, &from,&left); 
    if(left != 0){
        //HAL_UDP_TRACE("socket read is not completed...");
        register_socket_read_event(socket_id);
    }
    //HAL_UDP_TRACE("read count  = %d",count);
    if(-1 == count)
    {
        return -1;
    }
    memcpy(p_remote->addr,inet_ntoa(from.ip_addr),NETWORK_ADDR_LEN);
    p_remote->port = ntohs(from.port);
    return count;
}

int ALIPAY_MBEDTLS_SYMBOL(HAL_UDP_readTimeout)( void *p_socket,
                              coap_address_t *p_remote, unsigned char  *p_data,
                              unsigned int datalen,     unsigned int timeout )
{		
    int ret;
    sci_fd_set read_fds;
    int socket_id = -1;

    if(NULL == p_socket || NULL == p_data){
        //HAL_UDP_TRACE("error, params check failed");
        return -1;
    }
    socket_id = *(int *)p_socket;

    if( socket_id < 0 ){
        //HAL_UDP_TRACE("error, socketid = %d",socket_id);
        return -1;
    }

    SCI_FD_ZERO( &read_fds );
    SCI_FD_SET( socket_id, &read_fds );

#if 1//wait till readable
    //HAL_UDP_TRACE("select: start(socket_id = %d)",socket_id);
    ret = sci_sock_select( &read_fds, NULL, NULL, timeout/100 );
    //HAL_UDP_TRACE("select: end timeout = %d,ret = %d",timeout,ret);

    /* Zero fds ready means we timed out */
    if( ret == 0 ){
        //HAL_UDP_TRACE("receive timeout");
        register_socket_read_event(socket_id);
        return -2; // receive timeout
    }

    if( ret < 0 ) {
        //HAL_UDP_TRACE("receive failed");
        register_socket_read_event(socket_id);
        return -4; //receive failed
    }
#else//注册异步消息
    BLOCK_ID task_id = SCI_IdentifyThread();
    ret = sci_sock_asyncselect(socket_id, task_id, AS_READ|AS_WRITE|AS_CLOSE); 
    if(0 != result)
    {
        DTLS_TRC("joker.yang sci_sock_asyncselect failed ret = %d",ret);
        return -1;
    }
    xSignalHeaderRec* 	sig_ptr = 0;
    sig_ptr = SCI_GetSignal( task_id );
    switch(sig_ptr->SignalCode)
    {
        // 收到 WRITE消息，再次尝试发送数据
        case SOCKET_WRITE_EVENT_IND:
        break;
        // 收到READ消息，接收数据
        case SOCKET_READ_EVENT_IND:
        break;
        case SOCKET_CONNECTION_CLOSE_EVENT_IND:
        break;
        default:
        break;
    }
#endif
    /* This call will not block */
    return ALIPAY_MBEDTLS_SYMBOL(HAL_UDP_read)(p_socket, p_remote, p_data, datalen);
}

int ALIPAY_MBEDTLS_SYMBOL(HAL_UDP_resolveAddress)(const char *p_host,  char addr[NETWORK_ADDR_LEN])
{
    struct sci_sockaddr     target_addr;
    struct sci_hostent*     info_ptr = NULL;

    // get host IP address by name
    info_ptr = sci_gethostbyname( p_host );
    if ( NULL == info_ptr )
    {
        //HAL_UDP_TRACE("failed");
        return -1;
    }
    target_addr.port = 0;
    target_addr.family = AF_INET;
    memcpy( &target_addr.ip_addr, info_ptr->h_addr_list[0], info_ptr->h_length ); 
    memcpy(addr,inet_ntoa(target_addr.ip_addr),NETWORK_ADDR_LEN);
    //HAL_UDP_TRACE("host=%s    addr = %s",p_host,addr);
    return 0;
}

