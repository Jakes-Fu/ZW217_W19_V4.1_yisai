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
 *    Benjamin Cabé - Please refer to git log
 *    Fabien Fleutot - Please refer to git log
 *    Simon Bernard - Please refer to git log
 *    Julien Vermillard - Please refer to git log
 *    Axel Lorente - Please refer to git log
 *    Toby Jaffey - Please refer to git log
 *    Bosch Software Innovations GmbH - Please refer to git log
 *    Pascal Rieux - Please refer to git log
 *    Christian Renz - Please refer to git log
 *    Ricky Liu - Please refer to git log
 *
 *******************************************************************************/

/*
 Copyright (c) 2013, 2014 Intel Corporation

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
     * Neither the name of Intel Corporation nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 THE POSSIBILITY OF SUCH DAMAGE.

 David Navarro <david.navarro@intel.com>
 Bosch Software Innovations GmbH - Please refer to git log

*/

#include "lwm2mclient.h"
#include "liblwm2m.h"
#include "internals.h"

#include "commandline.h"
#if(defined WITH_TINYDTLS) 
#include "dtlsconnection.h"
#elif (defined CONFIG_LWM2M_TLS_SUPPORT)
#include "mbedconnection.h"
#else
#include "connection.h"
#endif


#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include "lwm2m_api.h"

//#include "vfs.h"  //modify by xiameng.yang
//#include "osi_log.h"  //modify by xiameng.yang

#define FOTA_NOTIFY_DATA_FILE_NAME "/FOTA_NOTIFY_DATA.bin"

#define MAX_PACKET_SIZE 1440
#define DEFAULT_SERVER_IPV6 "[::1]"
#define DEFAULT_SERVER_IPV4 "127.0.0.1"

int g_reboot = 0;
extern lwm2m_fota_state_t g_fota_state;
//#define LWM2M_FOTA_THREAD 1

#define OBJ_COUNT 10

// only backup security and server objects
# define BACKUP_OBJECT_COUNT 2
lwm2m_object_t * backupObjectArray[BACKUP_OBJECT_COUNT];

typedef struct
{
    lwm2m_object_t * securityObjP;
    lwm2m_object_t * serverObject;
    int sock;
#if (defined WITH_TINYDTLS) || (defined CONFIG_LWM2M_TLS_SUPPORT)
    dtls_connection_t * connList;  
#else
    connection_t * connList;
#endif
    lwm2m_context_t * lwm2mH;
    int addressFamily;
} client_data_t;

#if (defined WITH_TINYDTLS) || (defined CONFIG_LWM2M_TLS_SUPPORT)

extern mbedtls_ssl_context * g_sslContext;
#endif

#ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
#if LWIP_RFACK
void lwm2m_ack_callback(void *param, u32_t seqnum, u16_t acked);
#endif
#endif

#ifdef CONFIG_LWM2M_OCEANCONNECT_CUS_SUPPORT
extern dtls_handshake_state_t gHandshakeState;
#endif

static int prv_quit(char * buffer,
                     void * user_data)
{
    //LOG_ARG(" LWM2MDEBUG lwm2m prv_quit !\n");
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *)user_data;
    LOG_ARG(" LWM2MDEBUG lwm2m prv_quit !\n");
    lwm2mH->quit = 1;
    return 1;
}

int handle_value_changed(lwm2m_context_t * lwm2mH,
                          lwm2m_uri_t * uri,
                          int type,
                          uint8_t notify_type,
                          uint8_t seq_num,
                          uint8_t rai_mode,
                          const char * value,
                          size_t valueLength)
{
    lwm2m_object_t * object = (lwm2m_object_t *)LWM2M_LIST_FIND(lwm2mH->objectList, uri->objectId);

    if (NULL != object)
    {
        if (object->writeFunc != NULL)
        {
            lwm2m_data_t * dataP;
            int result = 0;
            ipso_res_t *ipso_res ;

            dataP = lwm2m_data_new(1);
            if (dataP == NULL)
            {
                LOG_ARG(" LWM2MDEBUG Internal allocation failure !\n");
                return -1;
            }
            dataP->id = uri->resourceId;
            #ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
            dataP->notify_type = (notify_type == NOTIFY_CON? 0 : 1);  // 0-- default value CON, 1 -- NON 
            dataP->seq_num = seq_num;
            dataP->rai_mode = rai_mode;
            LOG_ARG(" LWM2MDEBUG notify changed %d %d %d %d", dataP->id, dataP->notify_type, dataP->seq_num, dataP->rai_mode);
            #endif
            ipso_res = NULL;
            lwm2m_get_ipso_obj(uri->objectId,uri->resourceId,&ipso_res,lwm2mH->ref);
            if (ipso_res == NULL)
            {
                lwm2m_data_free(1, dataP);
                return -1;
            }

            if (lwm2m_is_dynamic_ipso(lwm2mH->ref))
            {
                dataP->type = ipso_res->value_type = type;
            }
            else
            {
                dataP->type = ipso_res->value_type;
            }
            if ((dataP->type+3) == LWM2M_TYPE_STRING)
                lwm2m_data_encode_nstring(value, valueLength, dataP);
            else if ((dataP->type+3) == LWM2M_TYPE_OPAQUE)
                lwm2m_data_encode_opaque((uint8_t *)value, valueLength, dataP);
            else if ((dataP->type+3) == LWM2M_TYPE_INTEGER)
                lwm2m_data_encode_int(atoi(value), dataP);
            else if ((dataP->type+3) == LWM2M_TYPE_FLOAT) {
                double vd;
                utils_textToFloat((uint8_t *)value, valueLength, &vd);
                lwm2m_data_encode_float(vd, dataP);
            }
            else if ((dataP->type+3) == LWM2M_TYPE_BOOLEAN)
                lwm2m_data_encode_bool(atoi(value), dataP);
            
            if (uri->objectId <= LWM2M_CONN_STATS_OBJECT_ID)
                result = object->writeFunc(uri->instanceId, 1, dataP, object);
            else if (object->notifyFunc != NULL)
                result = object->notifyFunc(uri->instanceId, 1, dataP, object);
            
            if (COAP_405_METHOD_NOT_ALLOWED == result)
            {
                switch (uri->objectId)
                {
                case LWM2M_DEVICE_OBJECT_ID:
                    result = device_change(dataP, object);
                    break;
                default:
                    break;
                }
            }

            if (COAP_204_CHANGED != result)
            {
                LOG_ARG(" LWM2MDEBUG Failed to change value!\n");
                result = -1;
            }
            else
            {
                LOG_ARG(" LWM2MDEBUG value changed!\n");
                lwm2m_resource_value_changed(lwm2mH, uri);
                result = 0;
            }
            lwm2m_data_free(1, dataP);
            return result;
        }
        else
        {
            LOG_ARG(" LWM2MDEBUG write not supported for specified resource!\n");
        }
    }
    else
    {
        LOG_ARG(" LWM2MDEBUG Object not found !\n");
    }
    return -1;
}
// After calling connection_free() to free all connections, the servers session pointer needs to be set to null.
void lwm2m_set_all_servers_session_to_null(lwm2m_context_t * lwm2mH)
{
    if(lwm2mH->bootstrapServerList != NULL)
    {
        lwm2m_server_t * bootstrapServer = lwm2mH->bootstrapServerList;
        while(bootstrapServer != NULL)
        {
            bootstrapServer->sessionH = NULL;
            bootstrapServer = bootstrapServer->next;
        }
    }
    if(lwm2mH->serverList != NULL)
    {
        lwm2m_server_t * server = lwm2mH->serverList;
        while(server != NULL)
        {
            server->sessionH = NULL;
            server = server->next;
        }
    }
}

#if(defined WITH_TINYDTLS) || (defined CONFIG_LWM2M_TLS_SUPPORT)
void * lwm2m_connect_server(uint16_t secObjInstID,
                            void * userData)
{
  client_data_t * dataP;
  lwm2m_list_t * instance;
  dtls_connection_t * newConnP = NULL;
  lwm2m_object_t  * securityObj;
  dataP = (client_data_t *)userData;
  securityObj = dataP->securityObjP;
  
  instance = LWM2M_LIST_FIND(dataP->securityObjP->instanceList, secObjInstID);
  if (instance == NULL) return NULL;
  
  
  newConnP = connection_create(dataP->connList, dataP->sock, securityObj, instance->id, dataP->lwm2mH, dataP->addressFamily);
  if (newConnP == NULL)
  {
      LOG_ARG(" LWM2MDEBUG Connection creation failed.\n");
      return NULL;
  }
  
  dataP->connList = newConnP;
  return (void *)newConnP;
}
#else
void * lwm2m_connect_server(uint16_t secObjInstID,
                            void * userData)
{
    client_data_t * dataP;
    char * uri;
    char * host;
    char * port;
    connection_t * newConnP = NULL;

    dataP = (client_data_t *)userData;

    uri = get_server_uri(dataP->securityObjP, secObjInstID);

    if (uri == NULL) return NULL;

    // parse uri in the form "coaps://[host]:[port]"
    if (0==strncmp(uri, "coaps://", strlen("coaps://"))) {
        host = uri+strlen("coaps://");
    }
    else if (0==strncmp(uri, "coap://",  strlen("coap://"))) {
        host = uri+strlen("coap://");
    }
    else {
        goto exit;
    }
    port = strrchr(host, ':');
    if (port == NULL) goto exit;
    // remove brackets
    if (host[0] == '[')
    {
        host++;
        if (*(port - 1) == ']')
        {
            *(port - 1) = 0;
        }
        else goto exit;
    }
    // split strings
    *port = 0;
    port++;

    OSI_LOGXI(OSI_LOGPAR_SS, 0x10007b61, "Opening connection to server at %s:%s", host, port);
    newConnP = connection_create(dataP->connList, dataP->sock, host, port, dataP->addressFamily);
    if (newConnP == NULL) {
        LOG_ARG(" LWM2MDEBUG Connection creation failed.");
    }
    else {
        newConnP->lwm2mH = dataP->lwm2mH;
        dataP->connList = newConnP;
    }

exit:
    lwm2m_free(uri);
    return (void *)newConnP;
}
#endif


/* Close fota socket      */
/* Release fota connection*/
int clean_fota_message(lwm2m_context_t * lwm2mH)
{
    OSI_LOGXI(OSI_LOGPAR_SSI, 0x10009858, "clean_fota_message serverip:%s, port:%s socket: %d", 
                        lwm2mH->fota_context.server_ip, lwm2mH->fota_context.port, lwm2mH->fota_context.fota_socket);

    if(NULL == lwm2mH)
    {
        return -1;
    }

    if(lwm2mH->fota_context.fota_socket)
    {
        close(lwm2mH->fota_context.fota_socket);
        lwm2mH->fota_context.fota_socket = 0;
    }
    memset(lwm2mH->fota_context.server_ip, 0, sizeof(lwm2mH->fota_context.server_ip));
    memset(lwm2mH->fota_context.port, 0, sizeof(lwm2mH->fota_context.port));
    
    /* Free fota connection */
    if(lwm2mH->fota_context.fota_conn)
    {
        #if (defined WITH_TINYDTLS) || (defined CONFIG_LWM2M_TLS_SUPPORT)
        connection_free((dtls_connection_t*)lwm2mH->fota_context.fota_conn);
        #else
        connection_free((connection_t *)lwm2mH->fota_context.fota_conn);
        #endif
    }
    lwm2mH->fota_context.fota_conn = NULL;

    /* Free fota Uri*/
    if(lwm2mH->fota_context.uri)
    {
        lwm2m_free(lwm2mH->fota_context.uri);
        lwm2mH->fota_context.uri = NULL;
    }
    

    return 0;
}


#if (defined WITH_TINYDTLS) || (defined CONFIG_LWM2M_TLS_SUPPORT)

/*  Parse 'uri' from server,   */
/*  Create new fota connection */ 
dtls_connection_t * create_fota_connection(char * uri, client_data_t * client)
{
   
    
    int    socket   = 0;
    char * port     = NULL;      //server fota port
    char * serverip = NULL;  //server ip
    char * p        = NULL;
    int    ip_len   = 0;
    int    port_len = 0;
    int   coaps     = 0;
    const char * localport  = "8888";
    struct addrinfo hints;
    dtls_connection_t * newConnP    = NULL;
    lwm2m_fota_t * pFota    = &(client->lwm2mH->fota_context);
    struct addrinfo   * servinfo    = NULL;
    lwm2m_object_t    * securityObj = client->securityObjP;
    
     if( NULL == uri  ||
        client->lwm2mH->fota_context.fota_socket > 0)
    {
        return NULL;
    }
    
    OSI_LOGXI(OSI_LOGPAR_S, 0x10009859, "create_fota_connection uri=%s", uri);

    //coap://[3ffe:2a00:100:7031::31]:1021/firmwarePackage/ff49888ff00098823ffsk
    //coaps://180.16.22.24:5683/firmwarePackage/ff49888ff00098823ffsk

    pFota->encrypt = 0; 

    /* Get Fota server 'ip' & 'port' */ 
    if (0 == strncmp(uri, "coap://",  strlen("coap://"))) 
    {
        coaps = 0;
        serverip = uri + strlen("coap://");
    }
    else if(0 == strncmp(uri, "coaps://",  strlen("coaps://")))
    {
        coaps = 1;
        serverip = uri + strlen("coaps://");    
    }
    else
    {
        return NULL;
    }

        /*ipv6*/
    if (serverip[0] == '[' && (port = strstr(serverip, "]:")))
    {
        serverip++;                 
        ip_len = port - serverip;
        p = strchr(port, '/');
        port += 2;
        port_len = p - port;
    }
    else
    {
        /*ipv4*/
        port = strchr(serverip, ':');
        if(NULL == port)
        {
            OSI_LOGE(0x100094a6, "create_fota_connection port error.\n");
            return NULL;
        }
        
        ip_len = port - serverip;
        port++;
        p = strchr(port, '/');
        port_len = p - port;
    }

    if(ip_len < 46 && port_len < 16)
    {
        strncpy(client->lwm2mH->fota_context.server_ip, serverip, ip_len);
        strncpy(client->lwm2mH->fota_context.port, port, port_len);
        serverip = client->lwm2mH->fota_context.server_ip;
        port     = client->lwm2mH->fota_context.port;
        serverip[ip_len] = '\0';
        port[port_len]   = '\0';
        pFota->encrypt = (coaps == 1)? 1 : 0;
    }
    else
    {
        OSI_LOGE(0x100094a7, "create_fota_connection ip_len:%d port_len:%d error.\n", ip_len, port_len);
        return NULL;
    }

    /* Create fota socket */
    socket = create_socket(localport, client->addressFamily);
    if (socket > 0)
    {
        LOG_ARG(" LWM2MDEBUG opening data socket success: %d", socket);
        #ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
        #if LWIP_RFACK
        int ret = lwip_setRFAckCallback(socket, lwm2m_ack_callback, client->lwm2mH);
        if(ret < 0)
            LOG_ARG(" LWM2MDEBUG set socket rf ack return %d", ret);
        #endif
        #endif
    }
    else
    {
        LOG_ARG(" LWM2MDEBUG opening data socket[%d] failed: %d", socket, errno);
        return NULL;
    }

    OSI_LOGXI(OSI_LOGPAR_SSI, 0x1000985a, "create_fota_connection serverip:%s, port:%s socket: %d", serverip, port, socket);

    /* Get addr info */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = client->addressFamily;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    if (0 != getaddrinfo(serverip, port, &hints, &servinfo) || servinfo == NULL)
    {
        close(socket);
        return NULL;
    }

    /* Create fota mbed connection */
    newConnP = connection_create_ext((dtls_connection_t *)pFota->fota_conn, socket, securityObj,
                                      0/*Instance id*/, servinfo, client->lwm2mH, client->addressFamily);
    if (newConnP == NULL) 
    {
        LOG_ARG(" LWM2MDEBUG Connection creation failed.");
        return NULL;
    }
    else 
    {
        client->lwm2mH->fota_context.fota_socket = socket;
        newConnP->lwm2mH = client->lwm2mH;
        client->lwm2mH->fota_context.fota_conn = newConnP;
        LOG_ARG(" LWM2MDEBUG create fota connection ok socket=%d, conn=0x%x", socket, newConnP);
    }

    /* release resource */
    if (NULL != servinfo)
    {
        freeaddrinfo(servinfo);
    }
    
    return (void*)newConnP;
}

#else

connection_t * create_fota_connection(char * uri, client_data_t * client)
{

  
    
    int    socket   = 0;
    char * port     = NULL;      //server fota port
    char * serverip = NULL;  //server ip
    char * p        = NULL;
    int    ip_len   = 0;
    int    port_len = 0;
    int   coaps     = 0;
    const char * localport  = "8888";
    //struct addrinfo hints;
    connection_t * newConnP    = NULL;
    lwm2m_fota_t * pFota    = &(client->lwm2mH->fota_context);
    struct addrinfo   * servinfo    = NULL;
    //lwm2m_object_t    * securityObj = client->securityObjP;
    
      if( NULL == uri  ||
        client->lwm2mH->fota_context.fota_socket > 0)
    {
        return NULL;
    }
    OSI_LOGXI(OSI_LOGPAR_S, 0x10009859, "create_fota_connection uri=%s", uri);

    //coap://[3ffe:2a00:100:7031::31]:1021/firmwarePackage/ff49888ff00098823ffsk
    //coaps://180.16.22.24:5683/firmwarePackage/ff49888ff00098823ffsk

    pFota->encrypt = 0; 

    /* Get Fota server 'ip' & 'port' */ 
    if (0 == strncmp(uri, "coap://",  strlen("coap://"))) 
    {
        coaps = 0;
        serverip = uri + strlen("coap://");
    }
    else if(0 == strncmp(uri, "coaps://",  strlen("coaps://")))
    {
        coaps = 1;
        serverip = uri + strlen("coaps://");    
    }
    else
    {
        return NULL;
    }

        /*ipv6*/
    if (serverip[0] == '[' && (port = strstr(serverip, "]:")))
    {
        serverip++;                 
        ip_len = port - serverip;
        p = strchr(port, '/');
        port += 2;
        port_len = p - port;
    }
    else
    {
        /*ipv4*/
        port = strchr(serverip, ':');
        if(NULL == port)
        {
            OSI_LOGE(0x100094a6, "create_fota_connection port error.\n");
            return NULL;
        }
        
        ip_len = port - serverip;
        port++;
        p = strchr(port, '/');
        port_len = p - port;
    }

    if(ip_len <= 46 && port_len <= 16)
    {
        strncpy(client->lwm2mH->fota_context.server_ip, serverip, ip_len);
        strncpy(client->lwm2mH->fota_context.port, port, port_len);
        serverip = client->lwm2mH->fota_context.server_ip;
        port     = client->lwm2mH->fota_context.port;
        serverip[ip_len] = '\0';
        port[port_len]   = '\0';
        pFota->encrypt = (coaps == 1)? 1 : 0;
    }
    else
    {
        OSI_LOGE(0x100094a7, "create_fota_connection ip_len:%d port_len:%d error.\n", ip_len, port_len);
        return NULL;
    }

    /* Create fota socket */
    socket = create_socket(localport, client->addressFamily);
    if (socket > 0)
    {
        LOG_ARG(" LWM2MDEBUG opening data socket success: %d", socket);
        #ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
        #if LWIP_RFACK
        int ret = lwip_setRFAckCallback(socket, lwm2m_ack_callback, client->lwm2mH);
        if(ret < 0)
            LOG_ARG(" LWM2MDEBUG set socket rf ack return %d", ret);
        #endif
        #endif
    }
    else
    {
        LOG_ARG(" LWM2MDEBUG opening data socket[%d] failed: %d", socket, errno);
        return NULL;
    }

    OSI_LOGXI(OSI_LOGPAR_SSI, 0x1000985a, "create_fota_connection serverip:%s, port:%s socket: %d", serverip, port, socket);

    #if 0
    /* Get addr info */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = client->addressFamily;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    if (0 != getaddrinfo(serverip, port, &hints, &servinfo) || servinfo == NULL)
    {
        close(socket);
        return NULL;
    }
    #endif
    
    /* Create fota connection */
    newConnP =  connection_create((connection_t *) pFota->fota_conn, socket, serverip, port,
                                    client->addressFamily);
    if (newConnP == NULL) 
    {
        LOG_ARG(" LWM2MDEBUG Connection creation failed.");
        return NULL;
    }
    else 
    {
        client->lwm2mH->fota_context.fota_socket = socket;
        newConnP->lwm2mH = client->lwm2mH;
        client->lwm2mH->fota_context.fota_conn = newConnP;
        LOG_ARG(" LWM2MDEBUG create fota connection ok socket=%d, conn=0x%x", socket, newConnP);
    }

    /* release resource */
    if (NULL != servinfo)
    {
        freeaddrinfo(servinfo);
    }
    
    return (void*)newConnP;
}

#endif



#ifdef LWM2M_FOTA_THREAD
static void lwm2m_fota_thread(void* c)
{

    int numBytes = 0;
    struct sockaddr_storage addr;
    socklen_t addrLen = sizeof(addr);
    
    client_data_t * client = (client_data_t *)c;
    uint8_t buffer[MAX_PACKET_SIZE];
    lwm2m_fota_t * pFota = &client->lwm2mH->fota_context;

    char s[INET6_ADDRSTRLEN];

    in_port_t port = 0;
    dtls_connection_t * connP = (dtls_connection_t *)pFota->fota_conn;

    LOG_ARG(" LWM2MDEBUG  lwm2m_fota_thread Enter.\n");
    
    while(g_fota_state == LWM2M_FOTA_STATE_DOWNLOADING)
    {
        LOG_ARG("recv encrypt:%d.\n", pFota->encrypt);
        if (pFota->encrypt)
        {
            numBytes = recvfrom(pFota->fota_socket, buffer, MAX_PACKET_SIZE, MSG_PEEK, (struct sockaddr *)&addr, &addrLen);
        }
        else
        {
            numBytes = recvfrom(pFota->fota_socket, buffer, MAX_PACKET_SIZE, 0, (struct sockaddr *)&addr, &addrLen);
        }
    
        if(numBytes > 0)
        {
        
#ifdef LWIP_IPV4_ON
            if (AF_INET == addr.ss_family)
            {
                struct sockaddr_in *saddr = (struct sockaddr_in *)&addr;
                inet_ntop(saddr->sin_family, &saddr->sin_addr, s, INET_ADDRSTRLEN);
                port = saddr->sin_port;
            }
#endif

#ifdef LWIP_IPV6_ON
            if (AF_INET6 == addr.ss_family)
            {
                struct sockaddr_in6 *saddr = (struct sockaddr_in6 *)&addr;
                inet_ntop(saddr->sin6_family, &saddr->sin6_addr, s, INET6_ADDRSTRLEN);
                port = saddr->sin6_port;
            }
#endif
             OSI_LOGXI(OSI_LOGPAR_ISI, 0x10007699, "%d bytes received from [%s]:%hu", numBytes, s, ntohs(port));
             output_buffer(stderr, buffer, numBytes, 0);
             
             if (0 != connection_handle_packet(connP, buffer, numBytes))
             {
                LOG_ARG(" LWM2MDEBUG error handling message %d",result);
             }
             LOG_ARG("fota recv numBytes = %d", numBytes);
             conn_s_updateRxStatistic(client->lwm2mH, numBytes, false);
             
        }
        else if(numBytes < 0)
        {
            /* fota error handle */
            if(errno == 9)
            {
                // close the invalid sock
                close(pFota->fota_socket);
                pFota->fota_socket = 0;
                notify_fota_state(LWM2M_FOTA_STATE_IDLE, LWM2M_FOTA_RESULT_CONNECTION_LOST, client->lwm2mH->ref);
                LOG("fota thread error.\n");
                break;
            }
        }
        else
        {
            LOG("fota thread recv 0.\n");
        }
    }

    OSI_LOGW(0x100094ab, " lwm2m_fota_thread exit.\n");
    lwm2m_Thread_stop();
}


int create_fota_thread(client_data_t * client)
{
    if(NULL == client )
    {
        return LWM2M_RET_ERROR;
    }

    int iRet = 0;
    lwm2m_fota_t * pFota = &client->lwm2mH->fota_context;
    
    //pFota->thread = \
     //       (uint32_t)osiThreadCreate("lwm2m_fota_thread", lwm2m_fota_thread, (void *)client, OSI_PRIORITY_NORMAL, 3072 * 4, 32);

    pFota->thread = (uint32_t) lwm2m_Thread_start("lwm2m_fota",lwm2m_fota_thread,(void *)client);
    if (0 == pFota->thread)
    {
        iRet = LWM2M_RET_ERROR;
    }
    else
    {
        iRet = LWM2M_RET_OK;
    }

    return iRet;
}
#endif


/* Prosess msg from fota socket */
int fota_msg_prosess(client_data_t * client, unsigned char* buffer)
{

    lwm2m_fota_t * pFota = &client->lwm2mH->fota_context;
    int fota_socket = pFota->fota_socket;
    int numBytes = 0;

    struct sockaddr_storage addr;
    socklen_t addrLen;

    addrLen = sizeof(addr);
    if (pFota->encrypt)
    {
        numBytes = recvfrom(fota_socket, buffer, MAX_PACKET_SIZE, MSG_PEEK, (struct sockaddr *)&addr, &addrLen);
    }
    else
    {
        numBytes = recvfrom(fota_socket, buffer, MAX_PACKET_SIZE, 0, (struct sockaddr *)&addr, &addrLen);
    }
    
    if (0 > numBytes)
    {
        OSI_LOGXI(OSI_LOGPAR_IS, 0x10007bba, "Error in recvfrom(): %d %s", errno, strerror(errno));
        if(errno == 9)
        {
            close(fota_socket);
            pFota->fota_socket = 0;
            LOG(" notify_fota_state error.");
            notify_fota_state(LWM2M_FOTA_STATE_IDLE, LWM2M_FOTA_RESULT_CONNECTION_LOST, client->lwm2mH->ref);
        }
    }
    else if (0 < numBytes)
    {
    
#if defined(LWIP_IPV4_ON) && defined(LWIP_IPV6_ON)
        char s[INET6_ADDRSTRLEN];
#else
#ifdef LWIP_IPV4_ON
        char s[INET_ADDRSTRLEN];
#else
        char s[INET6_ADDRSTRLEN];
#endif
#endif
        in_port_t port = 0;

#if defined (WITH_TINYDTLS) || defined (CONFIG_LWM2M_TLS_SUPPORT) 
        dtls_connection_t * connP = (dtls_connection_t *)pFota->fota_conn;
#else
        connection_t * connP = (connection_t *)pFota->fota_conn;
#endif

#ifdef LWIP_IPV4_ON
        if (AF_INET == addr.ss_family)
        {
            struct sockaddr_in *saddr = (struct sockaddr_in *)&addr;
            inet_ntop(saddr->sin_family, &saddr->sin_addr, s, INET_ADDRSTRLEN);
            port = saddr->sin_port;
        }
#endif

#ifdef LWIP_IPV6_ON
        if (AF_INET6 == addr.ss_family)
        {
            struct sockaddr_in6 *saddr = (struct sockaddr_in6 *)&addr;
            inet_ntop(saddr->sin6_family, &saddr->sin6_addr, s, INET6_ADDRSTRLEN);
            port = saddr->sin6_port;
        }
#endif

        OSI_LOGXI(OSI_LOGPAR_ISI, 0x10007699, "%d bytes received from [%s]:%hu", numBytes, s, ntohs(port));
        output_buffer(stderr, buffer, numBytes, 0);

        if (NULL != pFota->fota_conn)
        {
            #if defined (WITH_TINYDTLS) || defined (CONFIG_LWM2M_TLS_SUPPORT) 
            if (0 != connection_handle_packet(connP, buffer, numBytes))
            {
                 //LOG_ARG(" LWM2MDEBUG error handling message %d",result);
            }
            #else
            lwm2m_handle_packet(client->lwm2mH, buffer, numBytes, connP);
            #endif
            
            LOG_ARG("numBytes=%d",numBytes);
            conn_s_updateRxStatistic(client->lwm2mH, numBytes, false);
        }
        else
        {
            LOG_ARG(" LWM2MDEBUG received bytes ignored!");
        }
    }
    else
    {
        LOG_ARG(" LWM2MDEBUG 0 in recvfrom()");
    }

    return 0;
}

lwm2m_ret_t QuitHandshake(){
#if(defined WITH_TINYDTLS) || (defined CONFIG_LWM2M_TLS_SUPPORT)

    if(g_sslContext == NULL){
        LOG_ARG(" LWM2MDEBUG QuitHandshake g_sslContext == NULL");
        return 0;
    }
    g_sslContext->isquit = 1; 
#endif
    return 0;
}

void lwm2m_close_connection(void * sessionH,
                            void * userData)
{
    client_data_t * app_data;
#if(defined WITH_TINYDTLS) || (defined CONFIG_LWM2M_TLS_SUPPORT)
    dtls_connection_t * targetP;
#else
    connection_t * targetP;
#endif

    app_data = (client_data_t *)userData;
#if(defined WITH_TINYDTLS) || (defined CONFIG_LWM2M_TLS_SUPPORT)
    targetP = (dtls_connection_t *)sessionH;
#else
    targetP = (connection_t *)sessionH;
#endif

    LOG_ARG(" LWM2MDEBUG lwm2m_close_connection targetP:%p", targetP);
    LOG_ARG(" LWM2MDEBUG lwm2m_close_connection app_data->connList:%p", app_data->connList);
    /* transaction->peerH = connection. When free connection should free transaction list.*/
    prv_deleteTransactionList(targetP->lwm2mH);
    if (targetP == app_data->connList)
    {
        app_data->connList = targetP->next;
        //lwm2m_free(targetP);
        targetP->next = NULL;
        connection_free(targetP);
    }
    else
    {
#if(defined WITH_TINYDTLS) || (defined CONFIG_LWM2M_TLS_SUPPORT)
        dtls_connection_t * parentP;
#else
        connection_t * parentP;
#endif

        parentP = app_data->connList;
        while (parentP != NULL && parentP->next != targetP)
        {
            LOG_ARG(" LWM2MDEBUG lwm2m_close_connection parentP:%p", parentP);
            parentP = parentP->next;
        }
        if (parentP != NULL)
        {
            parentP->next = targetP->next;
            targetP->next = NULL;
            //lwm2m_free(targetP);
            connection_free(targetP);
        }
    }
}

static int prv_output_servers(char * buffer,
                               void * user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *) user_data;
    lwm2m_server_t * targetP;

    targetP = lwm2mH->bootstrapServerList;

    if (lwm2mH->bootstrapServerList == NULL)
    {
        LOG_ARG(" LWM2MDEBUG No Bootstrap Server.");
    }
    else
    {
        LOG_ARG(" LWM2MDEBUG Bootstrap Servers:");
        for (targetP = lwm2mH->bootstrapServerList ; targetP != NULL ; targetP = targetP->next)
        {
            LOG_ARG(" LWM2MDEBUG  - Security Object ID %d", targetP->secObjInstID);
            LOG_ARG(" LWM2MDEBUG \tHold Off Time: %lu s", (unsigned long)targetP->lifetime);
            LOG_ARG(" LWM2MDEBUG \tstatus: ");
            switch(targetP->status)
            {
            case STATE_DEREGISTERED:
                LOG_ARG(" LWM2MDEBUG DEREGISTERED");
                break;
            case STATE_BS_HOLD_OFF:
                LOG_ARG("CLIENT HOLD OFF");
                break;
            case STATE_BS_INITIATED:
                LOG_ARG( "BOOTSTRAP INITIATED");
                break;
            case STATE_BS_PENDING:
                LOG_ARG("BOOTSTRAP PENDING");
                break;
            case STATE_BS_FINISHED:
                LOG_ARG("BOOTSTRAP FINISHED");
                break;
            case STATE_BS_FAILED:
                LOG_ARG("BOOTSTRAP FAILED");
                break;
            default:
                LOG_ARG("INVALID (%d)", (int)targetP->status);
            }
        }
    }

    if (lwm2mH->serverList == NULL)
    {
        LOG_ARG( "No LWM2M Server.");
    }
    else
    {
        LOG_ARG("LWM2M Servers:");
        for (targetP = lwm2mH->serverList ; targetP != NULL ; targetP = targetP->next)
        {
            LOG_ARG( " - Server ID %d", targetP->shortID);
            LOG_ARG("\tstatus: ");
            switch(targetP->status)
            {
            case STATE_DEREGISTERED:
                LOG_ARG("DEREGISTERED");
                break;
            case STATE_REG_PENDING:
                LOG_ARG("REGISTRATION PENDING");
                break;
            case STATE_REGISTERED:
                LOG_ARG( "REGISTERED\tlocation: \"%s\"\tLifetime: %lus", targetP->location, (unsigned long)targetP->lifetime);
                break;
            case STATE_REG_UPDATE_PENDING:
                LOG_ARG("REGISTRATION UPDATE PENDING");
                break;
            case STATE_DEREG_PENDING:
                LOG_ARG("DEREGISTRATION PENDING");
                break;
            case STATE_REG_FAILED:
                LOG_ARG( "REGISTRATION FAILED");
                break;
            default:
                LOG_ARG("INVALID (%d)", (int)targetP->status);
            }
        }
    }
    return 0;
}

static int prv_change(char * buffer,
                       void * user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *) user_data;
    lwm2m_uri_t uri;
    uint8_t *urlstring;
    lwm2m_tpye_t type;
    uint32_t value_length;
    uint8_t notify_type;
    uint8_t seq_num;
    uint8_t rai_mode;
    //char * end = NULL;
    int result;
    int value[6];
    OSI_LOGXI(OSI_LOGPAR_SI, 0x10007b78, "prv_change buffer=%s", buffer);

    
    lwm2m_parse_buffer(buffer,value,6,NULL);
    urlstring = (uint8_t *)value[0]; buffer += strlen((char *)value[0])+1;
    type = (lwm2m_tpye_t)atoi((const char *)value[1]);buffer += strlen((char *)value[1])+1;
    value_length = (uint32_t)atoi((const char *)value[2]);buffer += strlen((char *)value[2])+1;
    notify_type = (uint8_t)atoi((const char *)value[3]);buffer += strlen((char *)value[3])+1;
    seq_num = (uint8_t)atoi((const char *)value[4]);buffer += strlen((char *)value[4])+1;
     rai_mode = (uint8_t)atoi((const char *)value[5]);buffer += strlen((char *)value[5])+1;

    result = lwm2m_stringToUri((char *)urlstring, strlen((char *)urlstring), &uri);
    if (result == 0) goto syntax_error;

    if (value_length == 0)
    {
        LOG_ARG("report change!\n");
        lwm2m_resource_value_changed(lwm2mH, &uri);
    }
    else
    {
#ifndef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT    
        if(notify_type == 0)
        {
            lwm2mH->notifyMode = NOTIFY_NON;
        }else if(notify_type == 1)
        {
            lwm2mH->notifyMode = NOTIFY_CON;
        }
#endif
        if (handle_value_changed(lwm2mH, &uri, type, notify_type, seq_num, rai_mode, buffer, value_length)<0)
            goto syntax_error;
    }

#ifndef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT    
    return (lwm2mH->notifyMode == NOTIFY_NON ? 0 : 1);
#else
    return (notify_type == NOTIFY_NON ? 0 : 1);
#endif

syntax_error:
    LOG_ARG( "Syntax error !\n");
    return -1;
}

static int prv_object_list(char * buffer,
                            void * user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *)user_data;
    lwm2m_object_t * objectP;

    for (objectP = lwm2mH->objectList; objectP != NULL; objectP = objectP->next)
    {
        if (objectP->instanceList == NULL)
        {
            LOG_ARG("/%d ", objectP->objID);
        }
        else
        {
            lwm2m_list_t * instanceP;

            for (instanceP = objectP->instanceList; instanceP != NULL ; instanceP = instanceP->next)
            {
                LOG_ARG("/%d/%ld  ", objectP->objID, instanceP->id);
            }
        }
    }
    return 0;
}

static int prv_instance_dump(lwm2m_object_t * objectP,
                              uint16_t id)
{
    int numData;
    lwm2m_data_t * dataArray;
    uint16_t res;

    numData = 0;
    res = objectP->readFunc(id, &numData, &dataArray, objectP);
    if (res != COAP_205_CONTENT)
    {
        LOG_ARG( "Error");
        print_status(stdout, res);
        return -1;
    }

    dump_tlv(stdout, numData, dataArray, 0);
    return 0;
}


static int prv_object_dump(char * buffer,
                            void * user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *) user_data;
    lwm2m_uri_t uri;
    char * end = NULL;
    int result;
    lwm2m_object_t * objectP;

    end = get_end_of_arg(buffer);
    if (end[0] == 0) goto syntax_error;

    result = lwm2m_stringToUri(buffer, end - buffer, &uri);
    if (result == 0) goto syntax_error;
    if (uri.flag & LWM2M_URI_FLAG_RESOURCE_ID) goto syntax_error;

    objectP = (lwm2m_object_t *)LWM2M_LIST_FIND(lwm2mH->objectList, uri.objectId);

    if (objectP == NULL)
    {
        LOG_ARG( "Object not found.\n");
        return -1;
    }

    if (uri.flag & LWM2M_URI_FLAG_INSTANCE_ID)
    {
        prv_instance_dump(objectP, uri.instanceId);
    }
    else
    {
        lwm2m_list_t * instanceP;

        for (instanceP = objectP->instanceList; instanceP != NULL ; instanceP = instanceP->next)
        {
            LOG_ARG("Instance %ld:", instanceP->id);
            prv_instance_dump(objectP, instanceP->id);
        }
    }
    return 0;

syntax_error:
    LOG_ARG("Syntax error !\n");
    return -1;
}

#ifdef CONFIG_LWM2M_ONENET_SUPPORT
static int prv_set_flag(char * buffer,
                            void * user_data)
{
    uint8_t flag;
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *)user_data;
    if (buffer[0] == 0) goto syntax_error;

    flag = (uint8_t) atoi(buffer);
    lwm2mH->sendFlagOnenet = flag;
    return 1;
syntax_error:
    LOG_ARG("Syntax error !\n");
    return 1;
}
#endif
void update_lifetime(lwm2m_context_t * lwm2mH, uint16_t shortServerID, uint32_t lifetime);

static int prv_update(char * buffer,
                       void * user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *)user_data;
    uint32_t value[2];  
    uint32_t lifetime;
    uint8_t withObjects;
    uint16_t shortServerID;
    lwm2m_server_t *targetP;
     int res;
    
    if (buffer[0] == 0) goto syntax_error;
    //uint32_t value[2];    
    lwm2m_parse_buffer(buffer,value,2,NULL);
    lifetime = atoi((const char *)value[0]);
    withObjects = atoi((const char *)value[1]);

    shortServerID = 0;
    //lwm2m_server_t *targetP;
    targetP = lwm2mH->serverList;
    if (targetP != NULL)
    {
        shortServerID = targetP->shortID;
    }
 
    update_lifetime(lwm2mH, shortServerID, lifetime);
     res = lwm2m_update_registration(lwm2mH, shortServerID, withObjects);
    if (res != 0)
    {
        LOG_ARG( "Registration update error");
        return 1;
    }
    return 1;

syntax_error:
    LOG_ARG("Syntax error");
    return 1;
}

#ifdef CONFIG_LWM2M_CTWING_SUPPORT
void update_bindingtype(lwm2m_context_t * lwm2mH, uint16_t shortServerID, lwm2m_binding_t binding);
static int prv_update_binding(char * buffer,
                       void * user_data)
{
    uint32_t value[1];
    uint32_t binding;
    lwm2m_server_t *targetP;
    uint16_t shortServerID ;
    lwm2m_binding_t bindingType;
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *)user_data;
    if (buffer[0] == 0) goto syntax_error;
   // uint32_t value[1];
    lwm2m_parse_buffer(buffer,value,1,NULL);
    binding = atoi((const char *)value[0]);

    shortServerID = 0;
    //lwm2m_server_t *targetP;
    targetP = lwm2mH->serverList;
    if (targetP != NULL)
    {
        shortServerID = targetP->shortID;
    }
    bindingType = BINDING_U;
    if(binding == 0)
    {
        bindingType = BINDING_U;
    }else if(binding == 1)
    {
        bindingType = BINDING_UQ;
    }
    update_bindingtype(lwm2mH, shortServerID, bindingType);
    lwm2m_update_bindingtype(lwm2mH, shortServerID);
    return 1;
        syntax_error:
    LOG_ARG("Syntax error !\n");
    return -1;
}
#endif

static int s_fota_num_retry = 0;
static int s_last_fota_num = 0;
static long s_fota_download_tv;

static long s_fota_download_start_time;
static int s_long_time_download_index = 0;
lwm2m_fota_notify_data_t g_fota_notify_data;

static uint8_t * prv_get_uri_path(uint8_t* uri)
{
    uint8_t * path;

    if ( ( 0 == strncmp((char*)uri, "coaps://", strlen("coaps://"))) || 
         ( 0 == strncmp((char*)uri, "coap://",  strlen("coap://"))) )
    {
        //"coap://192.168.188.200:8763/test/text1.file"
        path = (uint8_t *)strchr((const char *)uri, '/');
        if(path == NULL) return path;
        path = path + 2;
        path = (uint8_t *)strchr((const char *)path, '/');
    }
    else
    {
        //"/test/text1.file"
        path = uri;
    }

    return path;
}

static int prv_download_fota(char * buffer, void * user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *)user_data;
    int value[1];
    uint8_t * path;
    uint16_t mid ;
    uint8_t *uri;
    coap_packet_t message;
    uint8_t temp_token[COAP_TOKEN_LEN];
     time_t tv_sec ;
     size_t allocLen;
     lwm2m_watcher_t * watcherP;

    #if (defined WITH_TINYDTLS) || (defined CONFIG_LWM2M_TLS_SUPPORT)
        dtls_connection_t * fota_con = NULL;
    #else
        connection_t * fota_con = NULL;
    #endif
    
    path = prv_get_uri_path((uint8_t*)buffer);
    lwm2m_parse_buffer((char*)path, value, 1, NULL);
    
    
    
    mid = lwm2mH->nextMID++;
    uri = (uint8_t *)value[0];
    
    LOG_ARG(" prv_download_fota buffer=%s uri=%s\n", buffer, uri);

    //coap_packet_t message;
    coap_init_message(&message, COAP_TYPE_CON, COAP_GET, mid);
    coap_set_header_uri_path(&message, (const char *)uri);
    // generate a token
    //uint8_t temp_token[COAP_TOKEN_LEN];
    tv_sec = lwm2m_gettime();

    // initialize first 6 bytes, leave the last 2 random
    temp_token[0] = mid;
    temp_token[1] = mid >> 8;
    temp_token[2] = tv_sec;
    temp_token[3] = tv_sec >> 8;
    temp_token[4] = tv_sec >> 16;
    temp_token[5] = tv_sec >> 24;
    // use just the provided amount of bytes
    coap_set_header_token(&message, temp_token, COAP_TOKEN_LEN);
    //size_t allocLen;

    allocLen = coap_serialize_get_size(&message);
    if (allocLen == 0) return -1;

    if(lwm2mH->fota_upgrade_observed == NULL || lwm2mH->fota_upgrade_observed->watcherList == NULL)
        return 2;
     watcherP = lwm2mH->fota_upgrade_observed->watcherList;
    if (watcherP == NULL || watcherP->active != true)
        return 2;

    LOG_ARG("prv_download_fota  block2Num %d long_time_index %d",lwm2mH->fota_context.block2Num,s_long_time_download_index);
    if(s_last_fota_num != lwm2mH->fota_context.block2Num)
    {
        s_fota_num_retry = 0;
    }else
    {
        s_fota_num_retry++;
    }
    s_last_fota_num = lwm2mH->fota_context.block2Num;
    s_fota_download_tv = lwm2m_gettime();
    if(s_long_time_download_index %20 < 15 || lwm2mH->fota_context.block2Num == 0)
    {
        memcpy(lwm2mH->fota_context.token ,temp_token, COAP_TOKEN_LEN);
        if(lwm2mH->fota_context.uri == NULL)
        {
            lwm2mH->fota_context.uri = lwm2m_malloc(strlen((const char*)uri)+1);
            strcpy((char*)(lwm2mH->fota_context.uri), (const char*)uri);

            fota_con = create_fota_connection(buffer, (client_data_t *)lwm2mH->userData);
            if(NULL == fota_con)
            {
                OSI_LOGE(0x100094ac, " create_fota_connection error!");
                return -1;
            }

            #ifdef LWM2M_FOTA_THREAD
            if(0 != create_fota_thread((client_data_t *)(lwm2mH->userData)))
            {
                OSI_LOGE(0x100094ad, " lwm2m_fota_thread create error.");
                return -1;
            }
            #endif
        }
        coap_set_header_content_type(&message,LWM2M_CONTENT_OPAQUE);
        coap_set_header_block2(&message, lwm2mH->fota_context.block2Num, 0, REST_MAX_CHUNK_SIZE);
        lwm2mH->sendflag = 0;
        if(lwm2mH->fota_context.block2Num == 0)
        {
            s_fota_download_start_time = lwm2m_gettime();
        }

        LOG_ARG( "message_send COAP_GET lwm2mH->fota_context.block2Num %d",lwm2mH->fota_context.block2Num);
        //message_send(lwm2mH, &message, watcherP->server->sessionH);
        message_send(lwm2mH, &message, (void *)lwm2mH->fota_context.fota_conn);
    }
    return 1;
}

static int prv_notify_fota(char * buffer, void * user_data)
{
    int32_t fota_state ;
    uint32_t fota_resulte;
    coap_packet_t message;
    lwm2m_observed_t * targetP ;
    lwm2m_watcher_t * watcherP ;
    char send_buffer[10];
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *)user_data;
    int value[2];
    lwm2m_parse_buffer(buffer, value, 2, NULL);
    fota_state = (uint32_t)atoi((const char *)value[0]);
    fota_resulte = (uint32_t)atoi((const char *)value[1]);
    //coap_packet_t message;
    LOG_ARG("prv_notify_fota %d %d",fota_state, fota_resulte);

    if(fota_state == LWM2M_FOTA_STATE_IDLE)
    {
        switch (fota_resulte)
        {
            case LWM2M_FOTA_RESULT_CONNECTION_LOST:
            {
                lwm2mPostEvent(lwm2mH->ref, FOTA_DOWNLOADING_IND, 0, lwm2mH->fota_context.block2bufferSize, 0);
                lwm2mPostEvent(lwm2mH->ref, FOTA_DOWNLOAD_FAILED_IND, 0, 0, 0);
            }
            break;
            case LWM2M_FOTA_RESULT_INTEGRITY_FAILUER:
            {
                lwm2mPostEvent(lwm2mH->ref, FOTA_DOWNLOADING_IND, 0, lwm2mH->fota_context.block2bufferSize, 0);
                lwm2mPostEvent(lwm2mH->ref, FOTA_DOWNLOAD_SUCCESS_IND, 0, 0, 0);
                lwm2mPostEvent(lwm2mH->ref, FOTA_PACKAGE_CHECK_IND, 0, -1, 0);
            }
            break;
            case LWM2M_FOTA_RESULT_SUCCESS:
            {
                lwm2mPostEvent(lwm2mH->ref, FOTA_UPGRADE_OK_IND, 0, 0, 0);
            }
            break;
#ifdef CONFIG_LWM2M_OCEANCONNECT_CUS_SUPPORT
            case LWM2M_FOTA_RESULT_FIRMWARE_UPDATE_FAILED:
            {
                if(lwm2mH->fota_context.uri != NULL)
                {
                    free(lwm2mH->fota_context.uri);
                    lwm2mH->fota_context.uri = NULL;
                }
            }
            break;
#endif
            default:
            break;
        }
        /* Reset Fota context */
        /* if (fota_state == LWM2M_FOTA_STATE_IDLE) means Fota update is already finished.*/
        /* The result maybe ok, connection lost, check failuer ...            */
        /* For next Fota update, we need clean fota context and release connection now.   */
        clean_fota_message(lwm2mH);     
    }
    else if(fota_state == LWM2M_FOTA_STATE_DOWNLOADING)
    {
        lwm2mPostEvent(lwm2mH->ref, FOTA_DOWNLOADING_IND, 0, lwm2mH->fota_context.block2bufferSize, 0);
    }
    else if(fota_state == LWM2M_FOTA_STATE_DOWNLOADED)
    {
        lwm2mPostEvent(lwm2mH->ref, FOTA_DOWNLOADING_IND, 0, lwm2mH->fota_context.block2bufferSize, 0);
        lwm2mPostEvent(lwm2mH->ref, FOTA_DOWNLOAD_SUCCESS_IND, 0, 0, 0);
        lwm2mPostEvent(lwm2mH->ref, FOTA_PACKAGE_CHECK_IND, 0, 1, 0);
    }

     targetP = NULL;
     watcherP = NULL;
    for (targetP = lwm2mH->observedList ; targetP != NULL ; targetP = targetP->next)
    {
        if(targetP->uri.objectId == LWM2M_FIRMWARE_UPDATE_OBJECT_ID && targetP->uri.instanceId == 0 && targetP->uri.resourceId == 3)
        {
             watcherP = targetP->watcherList;
             break;
        }
    }
    LOG_ARG("prv_notify_fota watcherP %p",watcherP);
    if(watcherP == NULL) return -1;
    //char send_buffer[10];
    snprintf(send_buffer, 9,"%ld", fota_state);

    coap_init_message(&message, COAP_TYPE_NON, COAP_205_CONTENT, 0);
    //coap_set_header_content_type(&message, watcherP->format);
    coap_set_header_content_type(&message, LWM2M_CONTENT_TEXT);
    coap_set_payload(&message, send_buffer, strlen(send_buffer));

    watcherP->lastTime = lwm2m_gettime();
    watcherP->lastMid = lwm2mH->nextMID++;
    message.mid = watcherP->lastMid;

    coap_set_header_token(&message, watcherP->token, watcherP->tokenLen);
    coap_set_header_observe(&message, watcherP->counter++);
    (void)message_send(lwm2mH, &message, watcherP->server->sessionH);

    if(fota_state == LWM2M_FOTA_STATE_UPDATING)
    {
        lwm2m_fota_notify_data_t fota_notify_data;
        fota_notify_data.lastFotaOpened = true;
        fota_notify_data.tokenLen =  watcherP->tokenLen;
        fota_notify_data.counter = watcherP->counter;
        memcpy(fota_notify_data.token, watcherP->token, watcherP->tokenLen);
        fota_notify_data.lastMid = watcherP->lastMid;
        fota_notify_data.serverType = lwm2mH->serverType;
        if (0)// xiameng.yang  vfs_file_write((const char *)FOTA_NOTIFY_DATA_FILE_NAME, &fota_notify_data, sizeof(fota_notify_data)) != sizeof(fota_notify_data))
        {
            LOG_ARG("write fota notify data file failed.");
            return -1;
        }
        //osiThreadSleep(1500);
        SCI_Sleep(1500);
        lwm2mPostEvent(lwm2mH->ref, FOTA_UPGRADING_IND, 0, 0, 0);
    }
    return 1;
}

static int prv_notify_fota_idel_state(lwm2m_context_t *lwm2mH, lwm2m_fota_notify_data_t* fota_notify_data)
{
    
    coap_packet_t message;
    char send_buffer[10];
    LOG_ARG("notify fota idel state");
    set_fota_state_result(LWM2M_FOTA_STATE_IDLE, LWM2M_FOTA_RESULT_SUCCESS);
    snprintf(send_buffer, 9, "%d", 0);
    coap_init_message(&message, COAP_TYPE_NON, COAP_205_CONTENT, 0);
    coap_set_header_content_type(&message, LWM2M_CONTENT_TEXT);
    coap_set_payload(&message, send_buffer, strlen(send_buffer));
    message.mid = fota_notify_data->lastMid+1;
    coap_set_header_token(&message, fota_notify_data->token, fota_notify_data->tokenLen);
    coap_set_header_observe(&message, fota_notify_data->counter++);
    message_send(lwm2mH, &message, lwm2mH->serverList->sessionH);
    g_fota_notify_data.lastFotaOpened = false;
    lwm2mPostEvent(lwm2mH->ref, FOTA_UPGRADE_OK_IND, 0, 0, 0);
    //vfs_unlink((const char *)FOTA_NOTIFY_DATA_FILE_NAME);   //xiameng.yang
    return 0;
}

extern int32_t getBatteryLevel();
static void update_battery_level(lwm2m_context_t * context)
{
    static time_t next_change_time = 0;
    time_t tv_sec;

    tv_sec = lwm2m_gettime();
    if (tv_sec < 0) return;

    if (next_change_time < tv_sec)
    {
        char value[15];
        int valueLength;
        lwm2m_uri_t uri;
        int level = (int)getBatteryLevel();//rand() % 100;

        if (0 > level) level = -level;
        if (lwm2m_stringToUri("/3/0/9", 6, &uri))
        {
            valueLength = sprintf(value, "%d", level);
            LOG_ARG( "New Battery Level: %d\n", level);
            handle_value_changed(context, &uri, LWM2M_TYPE_INTEGER - 3, 0, 0, 0, value, valueLength);
        }
        level = rand() % 20;
        if (0 > level) level = -level;
        next_change_time = tv_sec + level + 10;
    }
}

bool add_mult_objinsts(lwm2m_context_t *contextP, uint16_t objid, uint32_t instCount, uint8_t *bitmap);

static int prv_add(char * buffer,
                    void * user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *)user_data;
    int value[4]; 
    uint16_t objid;
    uint32_t instCount ;
    uint8_t *bitmap;
    uint16_t resCount;
    lwm2m_parse_buffer(buffer,value,4,NULL);
    objid = (uint16_t)atoi((const char *)value[0]);
    instCount = (uint32_t)atoi((const char *)value[1]);
    bitmap = (uint8_t *)value[2];
    resCount = (uint16_t)atoi((const char *)value[3]);
    
    if (!lwm2m_isObjSupport(objid, lwm2mH->ref))
        lwm2m_registNewObj(objid, resCount, lwm2mH->ref);

    if (!add_mult_objinsts(lwm2mH,objid,instCount,bitmap))
    {
        LOG_ARG( "Creating object %d failed." ,objid);
        return -1;
    }
    return 0;
}

static int prv_remove(char * buffer,
                       void * user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *)user_data;
    int res;
    int value[1];
     uint16_t objid;
    lwm2m_parse_buffer(buffer,value,1,NULL);
     objid = (uint16_t)atoi((const char *)value[0]);

    res = lwm2m_remove_object(lwm2mH, objid);
    if (res != 0)
    {
        LOG_ARG("Removing object %d failed: ", objid);
        return -1;
    }
    else
    {
        LOG_ARG("Object %d removed", objid);
    }
    return 0;
}

#ifdef LWM2M_BOOTSTRAP

static int prv_initiate_bootstrap(char * buffer,
                                   void * user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *)user_data;
    lwm2m_server_t * targetP;

    // HACK !!!
    lwm2mH->state = STATE_BOOTSTRAP_REQUIRED;
    targetP = lwm2mH->bootstrapServerList;
    while (targetP != NULL)
    {
        targetP->lifetime = 0;
        targetP = targetP->next;
    }
    return 0;
}

static int prv_display_objects(char * buffer,
                                void * user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *)user_data;
    lwm2m_object_t * object;

    for (object = lwm2mH->objectList; object != NULL; object = object->next){
        if (NULL != object) {
            switch (object->objID)
            {
            case LWM2M_SECURITY_OBJECT_ID:
                display_security_object(object);
                break;
            case LWM2M_SERVER_OBJECT_ID:
                display_server_object(object);
                break;
            case LWM2M_ACL_OBJECT_ID:
                break;
            case LWM2M_DEVICE_OBJECT_ID:
                display_device_object(object);
                break;
            case LWM2M_CONN_MONITOR_OBJECT_ID:
                break;
            case LWM2M_FIRMWARE_UPDATE_OBJECT_ID:
                display_firmware_object(object);
                break;
            case LWM2M_LOCATION_OBJECT_ID:
                display_location_object(object);
                break;
            case LWM2M_CONN_STATS_OBJECT_ID:
                break;
            case TEST_OBJECT_ID:
                display_test_object(object);
                break;
            }
        }
    }
    return 0;
}

static int prv_display_backup(char * buffer,
        void * user_data)
{
   int i;
   for (i = 0 ; i < BACKUP_OBJECT_COUNT ; i++) {
       lwm2m_object_t * object = backupObjectArray[i];
       if (NULL != object) {
           switch (object->objID)
           {
           case LWM2M_SECURITY_OBJECT_ID:
               display_security_object(object);
               break;
           case LWM2M_SERVER_OBJECT_ID:
               display_server_object(object);
               break;
           default:
               break;
           }
       }
   }
   return 0;
}

static void prv_backup_objects(lwm2m_context_t * context)
{
    uint16_t i;
    lwm2m_object_t * objectSrc = NULL;

    for (i = 0; i < BACKUP_OBJECT_COUNT; i++) {
        if (NULL != backupObjectArray[i]) {
            switch (backupObjectArray[i]->objID)
            {
            case LWM2M_SECURITY_OBJECT_ID:
            case LWM2M_SERVER_OBJECT_ID:
                backupObjectArray[i]->cleanFunc(backupObjectArray[i]);
                lwm2m_free(backupObjectArray[i]);
                break;
            default:
                break;
            }
        }
        backupObjectArray[i] = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));
        memset(backupObjectArray[i], 0, sizeof(lwm2m_object_t));
    }

    /*
     * Backup content of objects 0 (security) and 1 (server)
     */
    objectSrc = (lwm2m_object_t *)LWM2M_LIST_FIND(context->objectList, LWM2M_SECURITY_OBJECT_ID);
    if(objectSrc != NULL)
        copy_security_object(backupObjectArray[0], objectSrc);
    objectSrc = (lwm2m_object_t *)LWM2M_LIST_FIND(context->objectList, LWM2M_SERVER_OBJECT_ID);
    if(objectSrc != NULL)
        copy_server_object(backupObjectArray[1], objectSrc);
}

static void update_bootstrap_info(lwm2m_client_state_t * previousBootstrapState,
        lwm2m_context_t * context)
{
    if (*previousBootstrapState != context->state)
    {
        *previousBootstrapState = context->state;
        switch(context->state)
        {
            case STATE_BOOTSTRAPPING:
#ifdef WITH_LOGS
                LOG_ARG( "[BOOTSTRAP] backup security and server objects");
#endif
                prv_backup_objects(context);
                break;
            default:
                break;
        }
    }
}

static void close_backup_object()
{
    int i;
    for (i = 0; i < BACKUP_OBJECT_COUNT; i++) {
        if (NULL != backupObjectArray[i]) {
            switch (backupObjectArray[i]->objID)
            {
            case LWM2M_SECURITY_OBJECT_ID:
            case LWM2M_SERVER_OBJECT_ID:
                backupObjectArray[i]->cleanFunc(backupObjectArray[i]);
                lwm2m_free(backupObjectArray[i]);
                backupObjectArray[i] = NULL;
                break;
            default:
                break;
            }
        }
    }
}
#endif

#ifdef CONFIG_LWM2M_ONENET_SUPPORT
static int prv_observe_rsp_onenet(char *buffer, void *user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *) user_data;
    uint16_t msgid ;
    uint8_t coapRet;
    uint8_t isAutoAck ;
    st_request_t * request;
    int value[3];
    lwm2m_parse_buffer(buffer, value, 3, NULL);
     msgid = (uint16_t)atoi((const char *)value[0]);
    coapRet = (uint8_t)atoi((const char *)value[1]);
    isAutoAck = (uint8_t)atoi((const char *)value[2]);
    SCI_TRACE_LOW( "  LWM2MDEBUG prv_observe_rsp_onenet  [%s  %d]\n",__FUNCTION__,__LINE__);
    LOG_ARG("msgid:%d ret:0x%x isAutoAck:%d", msgid, coapRet,isAutoAck);

    lwm2m_lock(lwm2mH->lockRequest, LWM2M_CONFIG_LOCK_INFINITY);
    request = (st_request_t *)LWM2M_LIST_FIND(lwm2mH->requestList, msgid);
    lwm2m_unlock(lwm2mH->lockRequest);

    if(request != NULL && (request->type == CALLBACK_TYPE_OBSERVE || request->type == CALLBACK_TYPE_OBSERVE_CANCEL))
    {
        if(observe_asynAckNodata(lwm2mH, request, coapRet) == COAP_NO_ERROR)
        {
            SCI_TRACE_LOW( "  LWM2MDEBUG   [%s  %d]\n",__FUNCTION__,__LINE__);
            packet_asynRemoveRequest(lwm2mH, request->mid, &request);
            if(isAutoAck)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
    if(isAutoAck)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

static int prv_discover_rsp_onenet(char *buffer, void *user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *) user_data;
    uint16_t msgid;
    uint8_t coapRet;
    uint8_t isAutoAck;
    st_request_t * request;
    int value[4];
    lwm2m_parse_buffer(buffer, value, 3, NULL);
    msgid = (uint16_t)atoi((const char *)value[0]);buffer += strlen((char *)value[0])+1;
    coapRet = (uint8_t)atoi((const char *)value[1]);buffer += strlen((char *)value[1])+1;
    isAutoAck = (uint8_t)atoi((const char *)value[2]);buffer += strlen((char *)value[2])+1;
    OSI_LOGXI(OSI_LOGPAR_IIIS, 0x10007b88, "[prv_discover_rsp_onenet] msgid:%d, ret:0x%x, isAutoAck:%d, buffer:%s", msgid, coapRet, isAutoAck, buffer);

    lwm2m_lock(lwm2mH->lockRequest, LWM2M_CONFIG_LOCK_INFINITY);
     request = (st_request_t *)LWM2M_LIST_FIND(lwm2mH->requestList, msgid);
    lwm2m_unlock(lwm2mH->lockRequest);

    if(request != NULL && request->type == CALLBACK_TYPE_DISCOVER)
    {
        if(prv_notifyDiscoverReport(lwm2mH, request, coapRet, (uint8_t *)buffer) == COAP_NO_ERROR)
        {
            packet_asynRemoveRequest(lwm2mH, request->mid, &request);
            if(isAutoAck)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
    if(isAutoAck)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

bool uri_exist_onenet(const lwm2m_uri_t * uri1,const lwm2m_uri_t * uri2)
{
    if(uri1 == NULL || uri2 == NULL)
        return false;

    if(uri1->objectId != uri2->objectId)
        return false;

    if(LWM2M_URI_IS_SET_INSTANCE(uri1)){
        if(!LWM2M_URI_IS_SET_INSTANCE(uri2)){
            return false;
        }

        if(uri1->instanceId != uri2->instanceId){
            return false;
        }
    }

    if(LWM2M_URI_IS_SET_RESOURCE(uri1)){
        if(!LWM2M_URI_IS_SET_RESOURCE(uri2)){
            return false;
        }

        if(uri1->resourceId != uri2->resourceId){
            return false;
        }
    }

    return true;
}

static int prv_notify_onenet(char *buffer, void *user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *) user_data;
    lwm2m_observed_t *observeP = NULL;
    uint16_t msgid;
    uint8_t *urlstring ;
    uint8_t result ;
    uint8_t valuetype ;
    uint32_t valuelen ;
    uint8_t coapType ;
    int ret;
    int value[6];
    lwm2m_uri_t uri;
    lwm2m_parse_buffer(buffer, value, 6, NULL);
     msgid = (uint16_t)atoi((const char *)value[0]);buffer += strlen((char *)value[0])+1;
    urlstring = (uint8_t *)value[1]; buffer += strlen((char *)value[1])+1;
     result = (uint8_t)atoi((const char *)value[2]);buffer += strlen((char *)value[2])+1;
     valuetype = (uint8_t)atoi((const char *)value[3]);buffer += strlen((char *)value[3])+1;
     valuelen = (uint32_t)atoi((const char *)value[4]);buffer += strlen((char *)value[4])+1;
    coapType = (uint8_t)atoi((const char *)value[5]);buffer += strlen((char *)value[5])+1;

    OSI_LOGXI(OSI_LOGPAR_ISI, 0x10007b89, "[prv_notify_onenet] msgid:%d, uri:%s, ret:0x%x", msgid, urlstring, result);

    ret = lwm2m_stringToUri((char *)urlstring, strlen((char *)urlstring), &uri);
    if (ret == 0)
        goto syntax_error;
    
    for (observeP=lwm2mH->observedList; observeP != NULL; observeP=observeP->next)
    {
        if(uri_exist_onenet(&observeP->uri, &uri))
            break;
    }

    if(observeP != NULL)
    {
        uint8_t ret;
        if (result == ONENET_NOTIFY_CONTENT && coapType == 1)
            lwm2mH->notifyMode = NOTIFY_CON;
        else
            lwm2mH->notifyMode = NOTIFY_NON;

         ret = prv_notifyObserveReport(lwm2mH, observeP, msgid, &uri, result, (uint8_t *)buffer, valuelen, valuetype);
        if(ret == COAP_NO_ERROR || ret == COAP_231_CONTINUE)
        {
            if(lwm2mH->notifyMode == NOTIFY_NON)
                lwm2mPostEvent(lwm2mH->ref, DATA_SEND_IND, LWM2M_EVENT_NOTIFY_SEND_SUCCESS, msgid, 0);

            return 1;
        }
    }

    lwm2mPostEvent(lwm2mH->ref, DATA_SEND_IND, LWM2M_EVENT_NOTIFY_FAILED, msgid, 0);
    return 1;

syntax_error:

    LOG("[prv_notify_onenet] Syntax error !");
    lwm2mPostEvent(lwm2mH->ref, DATA_SEND_IND, LWM2M_EVENT_NOTIFY_FAILED, msgid, 0);
    return 1;
}

static int prv_read_rsp_onenet(char *buffer, void *user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *) user_data;

    int ret;
    int value[5];
    lwm2m_uri_t uri;
    uint16_t msgid ;
    uint8_t *urlstring;
    uint8_t result ;
    uint8_t valuetype ;
    uint32_t valuelen ;
    st_request_t * request;
    lwm2m_parse_buffer(buffer, value, 5, NULL);
     msgid = (uint16_t)atoi((const char *)value[0]);buffer += strlen((char *)value[0])+1;
    urlstring = (uint8_t *)value[1]; buffer += strlen((char *)value[1])+1;
     result = (uint8_t)atoi((const char *)value[2]);buffer += strlen((char *)value[2])+1;
     valuetype = (uint8_t)atoi((const char *)value[3]);buffer += strlen((char *)value[3])+1;
    valuelen = (uint32_t)atoi((const char *)value[4]);buffer += strlen((char *)value[4])+1;

    OSI_LOGXI(OSI_LOGPAR_ISI, 0x10007b8a, "[prv_read_rsp_onenet] msgid:%d, uri:%s, ret:0x%x", msgid, urlstring, result);

    ret = lwm2m_stringToUri((char *)urlstring, strlen((char *)urlstring), &uri);
    if (ret == 0)
        goto syntax_error;
    
    lwm2m_lock(lwm2mH->lockRequest, LWM2M_CONFIG_LOCK_INFINITY);
     request = (st_request_t *)LWM2M_LIST_FIND(lwm2mH->requestList, msgid);
    lwm2m_unlock(lwm2mH->lockRequest);

    if(request != NULL && request->type == CALLBACK_TYPE_READ)
    {
        uint8_t ret = prv_notifyReadReport(lwm2mH, request, &uri, result, (uint8_t *)buffer, valuelen, valuetype);
        if(ret == COAP_NO_ERROR || ret == COAP_231_CONTINUE)
        {
            if(ret == COAP_NO_ERROR)
                packet_asynRemoveRequest(lwm2mH, request->mid, &request);
            return 0;
        }
    }

    return -1;

syntax_error:

    LOG("Syntax error !");
    return -1;
}

static int prv_write_rsp_onenet(char *buffer, void *user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *) user_data;
    uint16_t msgid;
    uint8_t coapRet;
    st_request_t * request ;

    int value[2];
    lwm2m_parse_buffer(buffer, value, 2, NULL);
     msgid = (uint16_t)atoi((const char *)value[0]);
    coapRet = (uint8_t)atoi((const char *)value[1]);
    OSI_LOGXI(OSI_LOGPAR_II, 0x10007b8b, "[prv_write_rsp_onenet] msgid:%d, ret:0x%x", msgid, coapRet);

    lwm2m_lock(lwm2mH->lockRequest, LWM2M_CONFIG_LOCK_INFINITY);
     request = (st_request_t *)LWM2M_LIST_FIND(lwm2mH->requestList, msgid);
    lwm2m_unlock(lwm2mH->lockRequest);

    if(request != NULL && request->type == CALLBACK_TYPE_WRITE)
    {
        if(object_asynAckNodata(lwm2mH, request, coapRet) == COAP_NO_ERROR)
        {
            packet_asynRemoveRequest(lwm2mH, request->mid, &request);
            return 0;
        }
    }

    return -1;
}

static int prv_execute_rsp_onenet(char *buffer, void *user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *) user_data;
    uint16_t msgid;
    uint8_t coapRet;
    st_request_t * request;
    
    int value[2];
    lwm2m_parse_buffer(buffer, value, 2, NULL);
    msgid = (uint16_t)atoi((const char *)value[0]);
    coapRet = (uint8_t)atoi((const char *)value[1]);
    OSI_LOGXI(OSI_LOGPAR_II, 0x10007b8c, "[prv_execute_rsp_onenet] msgid:%d, ret:0x%x", msgid, coapRet);

    lwm2m_lock(lwm2mH->lockRequest, LWM2M_CONFIG_LOCK_INFINITY);
    request = (st_request_t *)LWM2M_LIST_FIND(lwm2mH->requestList, msgid);
    lwm2m_unlock(lwm2mH->lockRequest);

    if(request != NULL && request->type == CALLBACK_TYPE_EXECUTE)
    {
        if(object_asynAckNodata(lwm2mH, request, coapRet) == COAP_NO_ERROR)
        {
            packet_asynRemoveRequest(lwm2mH, request->mid, &request);
            return 0;
        }
    }

    return -1;
}

static int prv_parameter_rsp_onenet(char *buffer, void *user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *) user_data;

    int value[2];
    uint16_t msgid;
    uint8_t coapRet;
    st_request_t * request;
    lwm2m_parse_buffer(buffer, value, 2, NULL);
    msgid = (uint16_t)atoi((const char *)value[0]);
    coapRet = (uint8_t)atoi((const char *)value[1]);
    OSI_LOGXI(OSI_LOGPAR_II, 0x10007b8d, "[prv_parameter_rsp_onenet] msgid:%d, ret:0x%x", msgid, coapRet);

    lwm2m_lock(lwm2mH->lockRequest, LWM2M_CONFIG_LOCK_INFINITY);
     request = (st_request_t *)LWM2M_LIST_FIND(lwm2mH->requestList, msgid);
    lwm2m_unlock(lwm2mH->lockRequest);

    if(request != NULL && request->type == CALLBACK_TYPE_OBSERVE_PARAMS)
    {
        if(object_asynAckNodata(lwm2mH, request, coapRet) == COAP_NO_ERROR)
        {
            packet_asynRemoveRequest(lwm2mH, request->mid, &request);
            return 0;
        }
    }

    return -1;
}

static int prv_clear_fwstate_onenet(char *buffer, void *user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *) user_data;
    uint8_t fwFlag;
    int value[1];
    lwm2m_parse_buffer(buffer, value, 1, NULL);
     fwFlag = (uint8_t)atoi((const char *)value[0]);
    LOG_ARG("[prv_clear_fwstate_onenet] fwFlag:%d", fwFlag);
    lwm2mH->fwstate = (fwFlag == 1);
    return 0;
}
#ifdef CONFIG_LWM2M_ONENET_UPDATE
extern int32_t reset_fotaIDIL(lwm2m_context_t* context, uint32_t msgid);
#ifdef CONFIG_LWM2M_ONENET_SOTA
static int prv_notify_sota_result_onenet(char *buffer, void *user_data)
{
    lwm2m_context_t * lwm2mH = (lwm2m_context_t *) user_data;
    int value[1];
    cissys_event_t id
    lwm2m_object_t * objectP;
    firmware_data_t * data;
    lwm2m_parse_buffer(buffer, value, 1, NULL);
    if(lwm2mH->isupdate_mcu != true)
    {
        lwm2mPostEvent(lwm2mH->ref, SOTA_RSP_IND, 0, 0, 0);
        return 1;
    }
     id = (uint8_t)atoi((const char *)value[0]);
    LOG_ARG( "sota result event id:%d", id);
    //lwm2m_object_t * objectP;
    objectP = (lwm2m_object_t *)LWM2M_LIST_FIND(lwm2mH->objectList, LWM2M_FIRMWARE_UPDATE_OBJECT_ID);
    if (NULL == objectP)
    {
        LOG_ARG("objectP is NULL");
        return 1;
    }
    data = (firmware_data_t*)(objectP->userData);
    cissys_EventHandler(id,lwm2mH, data);
    return 1;
}
#endif
#endif
#endif

void print_usage(void)
{
    LOG_ARG("Usage: lwm2mclient [OPTION]");
    LOG_ARG("Launch a LWM2M client.");
    LOG_ARG( "Options:");
    LOG_ARG( "  -n NAME\tSet the endpoint name of the Client. Default: testlwm2mclient");
    LOG_ARG( "  -l PORT\tSet the local UDP port of the Client. Default: 56830");
    LOG_ARG("  -h HOST\tSet the hostname of the LWM2M Server to connect to. Default: localhost");
    LOG_ARG("  -p PORT\tSet the port of the LWM2M Server to connect to. Default: "LWM2M_STANDARD_PORT_STR"");
    LOG_ARG( "  -4\t\tUse IPv4 connection. Default: IPv6 connection");
    LOG_ARG( "  -t TIME\tSet the lifetime of the Client. Default: 300");
    LOG_ARG( "  -b\t\tBootstrap requested.");
    LOG_ARG( "  -c\t\tChange battery level over time.");
    LOG_ARG( "  -e TYPE\tSet the connect server type.0-OC 1-AEP");
#if(defined WITH_TINYDTLS) || (defined CONFIG_LWM2M_TLS_SUPPORT)  
    LOG_ARG( "  -i STRING\tSet the device management or bootstrap server PSK identity. If not set use none secure mode");
    LOG_ARG("  -s HEXSTRING\tSet the device management or bootstrap server Pre-Shared-Key. If not set use none secure mode");    
#endif
}

void clean_all_objects(lwm2m_context_t * context);

#ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
#if LWIP_RFACK
void lwm2m_ack_callback(void *param, u32_t seqnum, u16_t acked)
{
    lwm2m_context_t *contextP = (lwm2m_context_t *)param;
    uint16_t obj_id = seqnum >> 16;
    uint8_t seq_num = seqnum &0xFF;
    int ack_type = (seqnum >> 8) & 0xF;
    int event = 0;
    client_data_t  *data;
    dtls_connection_t * connList ;
    if(contextP->ref >= MAX_REF_NUM || contextP->ref < 0)
    {
        LOG_ARG("lwm2m_ack_callback invalid ref %d", contextP->ref);
        return ;
    }
    data = contextP->userData;
    if(data == NULL || data->connList == NULL)
    {
        LOG_ARG("data or connList is NULL");
        return ;
    }
    connList = data->connList;
    
    if(ack_type == NOTIFY_NON)
    {
        event = (acked == 0?LWM2M_EVENT_NOTIFY_SENT_SUCCESS : LWM2M_EVENT_NOTIFY_FAILED);
#ifdef CONFIG_LWM2M_OCEANCONNECT_CUS_SUPPORT
        // In secure mode,  post event when rehandshake completed. 
        if(connList->issecure == 0 || (connList->issecure != 0 && lwm2m_get_handshake_state() == HANDSHAKE_COMPLETED))
#endif
            lwm2mPostEvent(contextP->ref, OBJ_OBSERVE_NOTIFY_IND,event,obj_id, ack_type << 8 | seq_num);
    }
    return;
}
#endif
#endif

#ifdef CONFIG_NET_CTIOTSM_SUPPORT
static bool prvIndexToFd(uint8_t index, uint16_t *paramFd, uint16_t *priKeyFd)
{
    if (index < 0 || index > 9)
        return false;
    *paramFd = 2 * index;
    *priKeyFd = 2 * index + 1;
    return true;
}

const char* cparam_str=
"93de051d62bf718ff5ed0704487d01d6e1e4086909dc3280e8c4e4817c66dddd21fe8dda4f21e60"
"7631065125c395bbc1c1c00cbfa6024350c464cd70a3ea6169f78397ad89a10c0c8a2a8d643da02"
"69b237bdbca56d4e41b3d12de22271052b36284b6d55084183f5aad7d004b1f199b6a72b9ee111a"
"565028578b18f44db873722755292130b08d2aab97fd34ec120ee265948d19c17abf9b7213baf82"
"d65b85aef3d078640c98597b6027b441a01ff1dd2c190f5e93c454806c11d8806141a7cf28d519b"
"e3da65f3170153d278ff247efba98a71a08116215bba5c999a7c717509b092e845c1266ba0d262c"
"bee6ed0736a96fa347c8bd856dc76b84ebeb963b791d54b125fa002ecf9c8397ec494185d57fb86"
"32fd9f7929bcd370b0ee4826f51229b842390f24eb7d75c652b1262a2f670d1331baedbd9604844"
"71c84c3664d667c596c90a3350403c662de40f0ddf1e20413c11c5ef7b765bcadd78627615b3c8b"
"7b5efd38d2362de8bc8addb55ef498b6d2ad1d9553f5724261f54ffb50984f153ab48e114e136d9"
"0b1d2c6970bde34276911743d7a0f8b6d4c0c58e42884e58d0eb6e32bc61be35832ae58606b78b2"
"4e1298aade56f18a150f9670f31193b7f83a6edf3cec238b87c2474e689fda598e9fa8ac570465a"
"f26f984a823d63e245050333de717e89b946f7b788d083080df803ca2d92a8e951236d4259590dc"
"8d74e57e07bbe50b69a61089a90237376a92bb18a91f9cb2f70f3735d629547cb5fd3d6b98a723e"
"13ceacaffba25e957b7a40dad52ce2f32a1714f7df3bcf1ffd603f9be241b9b736a567c09124170"
"b551205a0b36f9b4a3bb9087c5bc22e0b547eeee2c3c580e6f21f83f7a622f2e61ded8df8cd985d"
"208e65f3b5c83562876f73b8c98f2ee34436737e67955236a46793e3b0fe32762457461946aedbc"
"b3531574483679d688313f9677d45ff91f3b779baebf3544d6856c07b9f2e58ed9159f14ff8b838"
"47e2bf7b12e698b0066d79651594d2af09869a95db701cb9da6d60ccb8203945dd1bdab6c334527"
"7eb21cf9c9cdc10342c9a4705bfe7e76774";

static int prv_hexstr_to_hex(uint8_t *hexstr,uint32_t hexstrlen,uint8_t*
hex,uint32_t* hexlen){

    int i;
    int len_tmp = 0;
    len_tmp = hexstrlen % 2;
    char *h = (char*)hexstr;
    char *b = (char*)hex;
     char xlate[] = "0123456789ABCDEF";
    uint32_t needhexlen = len_tmp ? (hexstrlen / 2 + 1): (hexstrlen / 2);
    if(*hexlen < needhexlen)
        return -1;
    //char xlate[] = "0123456789ABCDEF";
    if(len_tmp == 1) {
        char *l = strchr(xlate, toupper((unsigned char)(*h)));
        *b = l - xlate;//((l - xlate) << 4);//
        h++;
        b++;
    }
    for ( i = 0; i < (needhexlen - len_tmp); h += 2, ++b, i++)
    {
        char *l = strchr(xlate, toupper((unsigned char)(*h)));
        char *r = strchr(xlate, toupper((unsigned char)(*(h+1))));

        if (!r || !l)
        {
            LOG_ARG("Failed to parse  HEXSTRING,i=%d,%d,%d",i,*h,*(h+1));
            return -1;
        }

        *b = ((l - xlate) << 4) + (r - xlate);
    }
    *hexlen = len_tmp ? (hexstrlen / 2 + 1): (hexstrlen / 2);
    return 0;
}


#endif


int lwm2mclient_main(int argc, char *argv[])
{
    int ref = -1;
    int res = -1;
    uint8_t isquit = 0;
    int ipc_socket = -1;
    int serverType = 0;
    client_data_t data;
    int result;
    lwm2m_context_t * lwm2mH = NULL;
    const char * localPort = "56830";
    const char * server = NULL;
    const char * serverPort = LWM2M_STANDARD_PORT_STR;
    char * name = "testlwm2mclient";
    int lifetime = 300;
    int timeout = 60;
    int batterylevelchanging = 0;
    time_t reboot_time = 0;
    int opt;
    bool bootstrapRequested = false;
    bool serverPortChanged = false;
    uint32_t clientHoldOffTime = 1;
    uint8_t portstr[20]={0};
    char *h ;
    char *b ;
    char xlate[] = "0123456789ABCDEF";

    char cmdline[350]="";
    int i;
    struct addrinfo hints;
    struct addrinfo *servinfo = NULL;
    // for DM standard begin
    char *dmKey = NULL;
    char *dmAppKey = NULL;
    char *dmVersion = NULL;
    // end
#ifdef CONFIG_LWM2M_ONENET_SUPPORT
#ifdef CONFIG_LWM2M_ONENET_DM_SUPPORT
    char *OnenetDmUpdeviceName = NULL;
    char *OnenetDmPwd = NULL;
    bool dm_CustomizedRead = false;
#endif
#ifdef CONFIG_LWM2M_ONENET_UPDATE
    uint8_t fotaclearstate = 0;
#endif
    uint8_t rsp_timeout = 0;
    uint8_t ack_timeout = COAP_RESPONSE_TIMEOUT_ONENET;
    char * authcode = NULL;
#endif
    uint8_t sendFlag = 0;
    uint8_t bindingMode = 0;
#ifdef LWM2M_BOOTSTRAP
    lwm2m_client_state_t previousState = STATE_INITIAL;
#endif
#ifdef CONFIG_NET_CTIOTSM_SUPPORT
        uint8_t isSM9Require = 0;
        uint8_t type = 0;
        uint8_t index = 0;
        uint8_t authtype = 0;
#endif
    char * pskId = NULL;
    char * psk = NULL;
    uint16_t pskLen = 0;
    char * pskBuffer = NULL;
    char serverUri[50];
    int serverId = 1;
    lwm2m_object_t * objArray[7] = {NULL};
#ifdef CONFIG_LWM2M_OCEANCONNECT_CUS_SUPPORT
    int fotaMode = 0;
    uint8_t handshakeTimeout = 1; 
    bool disableUpdate = false; 
#endif

    /*
     * The function start by setting up the command line interface (which may or not be useful depending on your project)
     *
     * This is an array of commands describes as { name, description, long description, callback, userdata }.
     * The firsts tree are easy to understand, the callback is the function that will be called when this command is typed
     * and in the last one will be stored the lwm2m context (allowing access to the server settings and the objects).
     */
    command_desc_t commands[] =
    {
            {"list", "List known servers.", NULL, prv_output_servers, NULL},
            {"change", "Change the value of resource.", " change URI [DATA]"
                                                        "   URI: uri of the resource such as /3/0, /3/0/2"
                                                        "   DATA: (optional) new value", prv_change, NULL},
            {"update", "Trigger a registration update", " update SERVER"
                                                        "   SERVER: short server id such as 123", prv_update, NULL},
#ifdef CONFIG_LWM2M_CTWING_SUPPORT
            {"update_binding", "Trigger a registration binding update", " update_binding BINDING"
                                                        "   BINDING: 0-U 1-UQ ", prv_update_binding, NULL},
#endif
#ifdef LWM2M_BOOTSTRAP
            {"bootstrap", "Initiate a DI bootstrap process", NULL, prv_initiate_bootstrap, NULL},
            {"dispb", "Display current backup of objects/instances/resources"
                    "\t(only security and server objects are backupped)", NULL, prv_display_backup, NULL},
#endif
            {"ls", "List Objects and Instances", NULL, prv_object_list, NULL},
#ifdef LWM2M_BOOTSTRAP
            {"disp", "Display current objects/instances/resources", NULL, prv_display_objects, NULL},
#endif
            {"dump", "Dump an Object", "dump URI"
                                       "URI: uri of the Object or Instance such as /3/0, /1", prv_object_dump, NULL},
            {"add", "Add support of object", NULL, prv_add, NULL},
            {"rm", "Remove support of object", NULL, prv_remove, NULL},
            {"quit", "Quit the client gracefully.", NULL, prv_quit, NULL},

            {"download_fota", "Down load coap URI data", NULL, prv_download_fota, NULL},
            {"notify_fota", "Notify fota event", NULL, prv_notify_fota, NULL},
#ifdef CONFIG_LWM2M_ONENET_SUPPORT
            {"setflag", "Set the flag of sending data", NULL, prv_set_flag, NULL},
            {"OnenetObserve", "OneNet observe response", NULL, prv_observe_rsp_onenet, NULL},
            {"OnenetDiscover", "OneNet discover response", NULL, prv_discover_rsp_onenet, NULL},
            {"OnenetNotify", "OneNet notify", NULL, prv_notify_onenet, NULL},
            {"OnenetRead", "OneNet read response", NULL, prv_read_rsp_onenet, NULL},
            {"OnenetWrite", "OneNet write response", NULL, prv_write_rsp_onenet, NULL},
            {"OnenetExec", "OneNet execute response", NULL, prv_execute_rsp_onenet, NULL},
            {"OnenetParam", "OneNet parameter response", NULL, prv_parameter_rsp_onenet, NULL},
            {"OnenetFwState", "OneNet clear fwstate", NULL, prv_clear_fwstate_onenet, NULL},
#ifdef CONFIG_LWM2M_ONENET_SOTA
            {"OnenetSotaResult", "Onenet notify sota result", NULL, prv_notify_sota_result_onenet, NULL},
#endif
#endif
            {"^C", "Quit the client abruptly (without sending a de-register message).", NULL, NULL, NULL},

            COMMAND_END_LIST
    };
    SCI_TRACE_LOW( " LWM2MDEBUG enter ------------> :[%s  %d]\n",__FUNCTION__,__LINE__);     
    memset(&data, 0, sizeof(client_data_t));
    data.addressFamily = AF_INET6;
    //char cmdline[350]="";
    for ( i=0;i<argc;i++){
      strcat(cmdline,argv[i]);
      strcat(cmdline," ");
    }
    OSI_LOGXI(OSI_LOGPAR_S, 0x07090005, "%s\n", cmdline);

    opt = 1;
    
    while (opt < argc)
    {
        if (argv[opt] == NULL
            || argv[opt][0] != '-'
            || argv[opt][2] != 0)
        {
            print_usage();
            return 0;
        }
        
        switch (argv[opt][1])
        {
        
        case 'b':
            bootstrapRequested = true;
            if (!serverPortChanged) serverPort = LWM2M_BSSERVER_PORT_STR;
            break;
        case 'O':
            opt++;
            if (opt >= argc)
            {
                print_usage();
                return 0;
            }
            if (1 != sscanf(argv[opt], "%lu", &clientHoldOffTime))
            {
                print_usage();
                return 0;
            }
            break;
#ifdef CONFIG_LWM2M_OCEANCONNECT_CUS_SUPPORT
        case 'F':
            opt++;
            if (opt >= argc)
            {
                print_usage();
                return 0;
            }
            if (1 != sscanf(argv[opt], "%d", &fotaMode))
            {
                print_usage();
                return 0;
            }
            break;
        case 'U':
            disableUpdate = true;
            break;
#endif
        case 'c':
            batterylevelchanging = 1;
            break;
        case 't':
            opt++;
            if (opt >= argc)
            {
                print_usage();
                return 0;
            }
            if (1 != sscanf(argv[opt], "%d", &lifetime))
            {
                print_usage();
                return 0;
            }
            break;
#if(defined WITH_TINYDTLS) || (defined CONFIG_LWM2M_TLS_SUPPORT)
        case 'i':
            opt++;
            if (opt >= argc)
            {
                print_usage();
                return 0;
            }
            pskId = argv[opt];
            SCI_TRACE_LOW(" LWM2MDEBUG pskId :%s",pskId);
            break;
        case 's':
            opt++;
            if (opt >= argc)
            {
                print_usage();
                return 0;
            }
            psk = argv[opt];
            SCI_TRACE_LOW(" LWM2MDEBUG psk :%s",psk);
            break;
#ifdef CONFIG_LWM2M_OCEANCONNECT_CUS_SUPPORT
        case 'N':
            opt++;
            if (opt >= argc)
            {
                print_usage();
                return 0;
            }            
            handshakeTimeout= atoi(argv[opt]);
            break;
#endif         
#endif                        
        case 'n':
            opt++;
            if (opt >= argc)
            {
                print_usage();
                return 0;
            }
            name = argv[opt];
            break;
        case 'l':
            opt++;
            if (opt >= argc)
            {
                print_usage();
                return 0;
            }
            localPort = argv[opt];
            break;
        case 'h':
            opt++;
            if (opt >= argc)
            {
                print_usage();
                return 0;
            }
            server = argv[opt];
            break;
        case 'p':
            opt++;
            if (opt >= argc)
            {
                print_usage();
                return 0;
            }
            serverPort = argv[opt];
            serverPortChanged = true;
            break;
        case '4':
            data.addressFamily = AF_INET;
            break;
        case 'f':
            opt++;
            ref = atoi(argv[opt]);
            break;
        case 'k':
            opt++;
            ipc_socket = atoi(argv[opt]);
            break;
        case 'o':
            opt++;
            timeout = atoi(argv[opt]);
            break;
        case 'u':
            opt++;
            sendFlag= atoi(argv[opt]);
            break;
        // for DM standard begin
        case 'w':
            opt++;
            if (opt >= argc)
            {
                print_usage();
                return 0;
            }
            dmKey = argv[opt];
            break;
        case 'a':
            opt++;
            if (opt >= argc)
            {
                print_usage();
                return 0;
            }
            dmAppKey = argv[opt];
            break;
        case 'v':
            opt++;
            if (opt >= argc)
            {
                print_usage();
                return 0;
            }
            dmVersion = argv[opt];
            break;
        case 'e':
            opt++;
            serverType = atoi(argv[opt]);
            break;
        case 'B':
            opt++;
            bindingMode = atoi(argv[opt]);
            break;
#ifdef CONFIG_LWM2M_ONENET_SUPPORT
#ifdef CONFIG_LWM2M_ONENET_DM_SUPPORT
        case 'D':
            opt++;
            OnenetDmUpdeviceName = argv[opt];
            break;
        case 'W':
            opt++;
            OnenetDmPwd = argv[opt];
            break;
        case 'd':
            dm_CustomizedRead = true;
            break;
#endif
#ifdef CONFIG_LWM2M_ONENET_UPDATE
        case 'm':
            opt++;
            fotaclearstate = atoi(argv[opt]);
            break;
#endif
        case 'H':
            opt++;
            rsp_timeout = atoi(argv[opt]);
            break;
        case 'K':
            opt++;
            ack_timeout = atoi(argv[opt]);
            break;
        case 'A':
            opt++;
            authcode = argv[opt];
            SCI_TRACE_LOW(" LWM2MDEBUG authcode :%s",authcode);
            break;
#endif
#ifdef CONFIG_NET_CTIOTSM_SUPPORT

        case 'S'://isSM9requre
            opt++;
            isSM9Require = atoi(argv[opt]);
            break;
        case 'T'://type
            opt++;
            type = atoi(argv[opt]);
            break;
        case 'I'://index
            opt++;
            index = atoi(argv[opt]);
            break;
        case 'M'://authtype 0:no sm9 2:sm9 auth
            opt++;
            authtype = atoi(argv[opt]);
            break;
#endif
        default:
            print_usage();
            return 0;
        }
        opt += 1;
    }
    //fupdateStatus_t fupdateStatus = FUPDATE_STATUS_NOT_READY;
    if (0)// xiameng.yang vfs_file_read((const char *)FOTA_NOTIFY_DATA_FILE_NAME, &g_fota_notify_data, sizeof(g_fota_notify_data)) != sizeof(g_fota_notify_data))
    {
        LOG_ARG("read fota notify data file failed.");
    }
    else
    {
        //fupdateStatus = fupdateGetStatus();
        //LOG_ARG("lastFotaOpened is %d, fupdateStatus is %d", g_fota_notify_data.lastFotaOpened, fupdateStatus);
    }
    //CFW_GPRS_PDPCONT_INFO_V2 sPdpCont;
    if(0)//xiameng.yang netif_default == NULL)
    {
        LOG_ARG("netif_default is NULL");
        return -1;
    }
    #if 0
    uint8_t sim = (netif_default->sim_cid & 0xf0) >> 4;
    uint8_t cid = netif_default->sim_cid & 0x0f; 
    if (CFW_GprsGetPdpCxtV2(cid, &sPdpCont, sim) == 0)
    {
        if(sPdpCont.PdnType == CFW_GPRS_PDP_TYPE_IP)
        {
            data.addressFamily  = AF_INET;
        }
        else if(sPdpCont.PdnType == CFW_GPRS_PDP_TYPE_IPV6)
        {
            data.addressFamily  = AF_INET6;
        }
        else if(sPdpCont.PdnType == CFW_GPRS_PDP_TYPE_IPV4V6)
        {
            data.addressFamily  = AF_UNSPEC;
        }
    }
    #endif
    if (!server)
    {
        server = (AF_INET == data.addressFamily ? DEFAULT_SERVER_IPV4 : DEFAULT_SERVER_IPV6);
    }
    /*
     * Invalid domain name check in advance to avoid enter the main func endless loop 
     */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = data.addressFamily;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    if (0 != getaddrinfo(server, serverPort, &hints, &servinfo) || servinfo == NULL)
    {
        OSI_LOGXI(OSI_LOGPAR_SS, 0x10007b9d, "getaddrinfo return failed %s %s\n",server, serverPort);
        if(servinfo != NULL)
            freeaddrinfo(servinfo);
        return -1;
    }
    if(1)//sPdpCont.PdnType == CFW_GPRS_PDP_TYPE_IPV4V6)
    {
        data.addressFamily = servinfo->ai_family;
        SCI_TRACE_LOW(" LWM2MDEBUG  data.addressFamily :%d   [  %s  %d]  \r\n",data.addressFamily,__FUNCTION__,__LINE__);
        
    }    
    freeaddrinfo(servinfo);
    servinfo = NULL;
    /*
     *This call an internal function that create an IPV6 socket on the port 5683.
     */
     
    //uint8_t portstr[20]={0};
    res = utils_intToText(atoi(localPort)+ref,portstr,strlen(localPort));
    if (res <= 0) return -1;

    /*
     * Now the main function fill an array with each object, this list will be later passed to liblwm2m.
     * Those functions are located in their respective object file.
     */
#if(defined WITH_TINYDTLS) || (defined CONFIG_LWM2M_TLS_SUPPORT)
    if (psk != NULL)
    {
#if(defined CONFIG_LWM2M_CTWING_SUPPORT) || (defined CONFIG_LWM2M_ONENET_SUPPORT)
        if(serverType == LWM2M_SERVER_AEP || serverType == LWM2M_SERVER_ONENET)
        {
            pskLen = strlen(psk);
            pskBuffer = malloc(pskLen+1);

            if (NULL == pskBuffer)
            {
                LOG_ARG( "Failed to create PSK binary buffer");
                return -1;
            }
            memset(pskBuffer, 0, pskLen+1);
            memcpy(pskBuffer,psk,pskLen);
            LOG_ARG( "create PSK binary buffer");
         }
         else
#endif
        {
            int i;
            int len_tmp = 0;
            len_tmp = strlen(psk) % 2;
            pskLen = len_tmp ? (strlen(psk) / 2 + 1): (strlen(psk) / 2);
            pskBuffer = malloc(pskLen);

            if (NULL == pskBuffer)
            {
                LOG_ARG("Failed to create PSK binary buffer");
                return -1;
            }
            LOG_ARG( "Failed to create PSK binary buffer");
            memset(pskBuffer, 0, pskLen);
            // Hex string to binary
            h = psk;
            b = pskBuffer;
            //char xlate[] = "0123456789ABCDEF";
            if(len_tmp == 1) {
                char *l = strchr(xlate, toupper((unsigned char)(*h)));
                *b = l - xlate;//((l - xlate) << 4);//
                h++;
                b++;
            }
            LOG_ARG( "Failed to create PSK binary buffer");
            for ( i = 0; i < (pskLen - len_tmp); h += 2, ++b, i++)
            {
                char *l = strchr(xlate, toupper((unsigned char)(*h)));
                char *r = strchr(xlate, toupper((unsigned char)(*(h+1))));

                if (!r || !l)
                {
                    LOG_ARG("Failed to parse Pre-Shared-Key HEXSTRING");
                    goto error_exit;
                }

                *b = ((l - xlate) << 4) + (r - xlate);
            }
        }
    }
#endif

    /*
     * The liblwm2m library is now initialized with the functions that will be in
     * charge of communication
     */
    LOG_ARG( "  PSK binary buffer  :%s   \n",pskBuffer);
    lwm2mH = lwm2m_init(&data, serverType);
    if (NULL == lwm2mH)
    {
        LOG_ARG("lwm2m_init() failed");
        goto error_exit;
    }
#ifdef CONFIG_NET_CTIOTSM_SUPPORT
        lwm2mH->isSM9Require = (bool)isSM9Require;
        lwm2mH->type = type;
        lwm2mH->index =  index;
        LOG_ARG("lwm2m_configs[ref]->authtype=%d,isSM9Require=%d,index=%d",authtype,isSM9Require,index);
        lwm2mH->authtype = authtype;
#endif

#if(defined WITH_TINYDTLS) || (defined CONFIG_LWM2M_TLS_SUPPORT)

    if (psk != NULL || pskId != NULL)
        sprintf (serverUri, "coaps://%s:%s", server, serverPort);
    else
        sprintf (serverUri, "coap://%s:%s", server, serverPort);
#else
    sprintf (serverUri, "coap://%s:%s", server, serverPort);
#endif

#ifdef CONFIG_LWM2M_ONENET_UPDATE
    lwm2mH->fw_flag = (fotaclearstate == 1);
    LOG_ARG("lwm2mH->fw_flag=%d",lwm2mH->fw_flag);

    if(lwm2mH->serverType == LWM2M_SERVER_ONENET){
        st_request_t* request;

        lwm2mH->fw_sys_state = FOTA_STATE_IDIL;
        lwm2mH->fw_request.block1_mid = 0;
        lwm2mH->fw_request.single_mid = 0;
        lwm2mH->fw_request.block1_more = 0;
        lwm2mH->fw_request.block1_num = 0;
        lwm2mH->fw_request.block1_size = REST_MAX_CHUNK_SIZE;
        request = (st_request_t *)lwm2m_malloc(sizeof(st_request_t));
        if (request == NULL)
        {
            reset_fotaIDIL(lwm2mH, LWM2M_EVENT_FIRMWARE_UPDATE_FAILED);
            goto error_exit;
        }
        memset(request, 0, sizeof(struct st_request));

        lwm2mH->fw_request.tv_t = 0;
        lwm2mH->fw_request.write_state = PACKAGE_WRITE_IDIL;
        //contextP->fw_request.ack_send_count = 0;
        lwm2mH->fw_request.single_packet_write = false;
        lwm2mH->fw_request.need_async_ack = false;
        lwm2mH->fw_request.response_ack = request;
        lwm2mH->fota_created = true;
    }
#endif//CONFIG_LWM2M_ONENET_UPDATE
#ifdef LWM2M_BOOTSTRAP
    objArray[0] = get_security_object(serverId, serverUri, pskId, pskBuffer, pskLen, bootstrapRequested, clientHoldOffTime);
#else
    objArray[0] = get_security_object(serverId, serverUri, pskId, pskBuffer, pskLen, false, 1);
#endif
    if (NULL == objArray[0])
    {
        LOG_ARG("Failed to create security object");
        goto error_exit;
    }
    data.securityObjP = objArray[0];

#ifdef CONFIG_LWM2M_OCEANCONNECT_CUS_SUPPORT
    lwm2mH->disableUpdate = disableUpdate; 
    if(disableUpdate)
    {
        lifetime = 86400;
    }
#endif

    if(bindingMode == 0)
    {
        objArray[1] = get_server_object(serverId, "U", lifetime, false);
    }else
    {
        objArray[1] = get_server_object(serverId, "UQ", lifetime, false);
    }
    if (NULL == objArray[1])
    {
        LOG_ARG("Failed to create server object");
        goto error_exit;
    }

    objArray[2] = get_object_device(lwm2mH);
    if (NULL == objArray[2])
    {
        LOG_ARG("Failed to create Device object");
        goto error_exit;
    }

    objArray[3] = get_object_firmware(lwm2mH);
    if (NULL == objArray[3])
    {
        LOG_ARG( "Failed to create Firmware object");
        goto error_exit;
    }

    objArray[4] = get_object_conn_m(lwm2mH);
    if (NULL == objArray[4])
    {
        LOG_ARG("Failed to create connectivity monitoring object");
        goto error_exit;
    }

#ifdef CONFIG_LWM2M_ONENET_DM_SUPPORT
    if(lwm2mH->serverType == LWM2M_SERVER_ONENET && 
        OnenetDmUpdeviceName != NULL &&
        OnenetDmPwd != NULL)
    {
        objArray[5] = get_object_DM_Onenet(lwm2mH);
        if (NULL == objArray[5])
        {
            LOG_ARG("Failed to create Onenet DM object");
            goto error_exit;
        }
    }
#endif

    lwm2mH->ref = ref;
    lwm2mH->sendflag = sendFlag;
#ifdef CONFIG_LWM2M_OCEANCONNECT_CUS_SUPPORT
    lwm2mH->fota_context.fotaMode = fotaMode;
    lwm2mH->handshakeTimeout = handshakeTimeout;
#endif

//#if(defined WITH_TINYDTLS) || (defined CONFIG_LWM2M_TLS_SUPPORT)
    data.lwm2mH = lwm2mH;
//#endif
    
    /*
     * We configure the liblwm2m library with the name of the client - which shall be unique for each client -
     * the number of objects we will be passing through and the objects array
     */
#ifdef CONFIG_LWM2M_ONENET_DM_SUPPORT
    if(lwm2mH->serverType == LWM2M_SERVER_ONENET && 
        OnenetDmUpdeviceName != NULL &&
        OnenetDmPwd != NULL)
        result = lwm2m_configure(lwm2mH, name, NULL, NULL, 6, objArray);
    else
#endif    
        result = lwm2m_configure(lwm2mH, name, NULL, NULL, 5, objArray);
    if (result != 0)
    {
        LOG_ARG("lwm2m_configure() failed: 0x%X", result);
        goto error_exit;
    }

    // for DM standard begin
    if (dmKey != NULL)
    {
        lwm2mH->dmSecretKey = lwm2m_strdup(dmKey);
    }

    if (dmAppKey!= NULL)
    {
        lwm2mH->dmAppkey = lwm2m_strdup(dmAppKey);
    }

    if (dmVersion != NULL)
    {
        lwm2mH->dmVersion = lwm2m_strdup(dmVersion);
    }
    // end

#ifdef CONFIG_LWM2M_ONENET_SUPPORT
#ifdef CONFIG_LWM2M_ONENET_DM_SUPPORT
    if (OnenetDmUpdeviceName != NULL)
    {
        lwm2mH->DMprivData = lwm2m_strdup(OnenetDmUpdeviceName);
    }

    if (OnenetDmPwd != NULL)
    {
        lwm2mH->dm_pwd = lwm2m_strdup(OnenetDmPwd);
    }

    lwm2mH->dm_CustomizedRead = dm_CustomizedRead;
#endif
    if(lwm2mH->serverType == LWM2M_SERVER_ONENET)
    {
        lwm2mH->rsp_timeout = rsp_timeout;
        lwm2mH->ack_timeout = ack_timeout;
        if(authcode != NULL)
            lwm2mH->authCode = lwm2m_strdup(authcode);
    }
#endif
    /**
     * Initialize value changed callback.
     */
    init_value_change(lwm2mH);

    /*
     * As you now have your lwm2m context complete you can pass it as an argument to all the command line functions
     * precedently viewed (first point)
     */
    for ( i = 0 ; commands[i].name != NULL ; i++)
    {
        commands[i].userData = (void *)lwm2mH;
    }
    OSI_LOGXI(OSI_LOGPAR_SS, 0x10007ba8, "LWM2M Client \"%s\" started on port %s", name, portstr);
    /*
     * We now enter in a while loop that will handle the communications from the server
     */
    while (lwm2m_configs[ref]->isquit < 2) //lwm2mH->quit
    {
        struct timeval tv;
        fd_set readfds;
        fd_set exceptfds;
        int maxfd = 0;
        SCI_TRACE_LOW( " LWM2MDEBUG ENTER  while  ---------> [%s  %d]\n",__FUNCTION__,__LINE__);
        if (g_reboot)
        {
            time_t tv_sec;

            tv_sec = lwm2m_gettime();

            if (0 == reboot_time)
            {
                reboot_time = tv_sec + 15;
            }
            if (reboot_time < tv_sec)
            {
                /*
                 * Message should normally be lost with reboot ...
                 */
                LOG_ARG("reboot time expired, rebooting ...");
                system_reboot();
            }
            else
            {
                tv.tv_sec = 1;
            }
        }
        else if (batterylevelchanging) 
        {
            update_battery_level(lwm2mH);
            tv.tv_sec = 5;
        }
        else 
        {
            tv.tv_sec = timeout;
        }
        if(lwm2m_configs[ref]->isquit !=0)
        {
            if(lwm2m_configs[ref]->isquit == 1)
            {
                lwm2m_deregister(lwm2mH);
            }
            tv.tv_sec = 3;
            lwm2m_configs[ref]->isquit++;
        }
        if(g_fota_state == LWM2M_FOTA_STATE_DOWNLOADING)
        {
            long now = lwm2m_gettime();
            LOG_ARG("LWM2M_FOTA_STATE_DOWNLOADING  now %d long_time_index %d start_time %d",now,s_long_time_download_index, s_fota_download_start_time);
            if(s_fota_download_start_time != 0)
            {
                s_long_time_download_index = (now - s_fota_download_start_time)/10;
            }

            if(s_long_time_download_index %20 < 15)
            {
                if(now - s_fota_download_tv > 20)
                {
                    if(lwm2mH->fota_context.uri != NULL)
                    {
                        LOG_ARG("LWM2M_FOTA_STATE_DOWNLOADING s_fota_num_retry %d", s_fota_num_retry);
                        lwm2m_start_fota_download(lwm2mH->fota_context.uri, lwm2mH->ref);
                        s_fota_download_tv = lwm2m_gettime();
                    }
                }
                if(s_fota_num_retry > 3)
                {
                    notify_fota_state(LWM2M_FOTA_STATE_IDLE, LWM2M_FOTA_RESULT_CONNECTION_LOST, lwm2mH->ref);
                }
            }
            tv.tv_sec = 2;
        }

        if(0) //xiameng.yang g_fota_notify_data.lastFotaOpened == true && fupdateStatus == FUPDATE_STATUS_FINISHED && lwm2mH->state == STATE_READY &&  g_fota_notify_data.serverType == lwm2mH->serverType)
        {
            prv_notify_fota_idel_state(lwm2mH, &g_fota_notify_data);
        }

        tv.tv_usec = 0;

        SCI_TRACE_LOW(" LWM2MDEBUG  ipc_socket :0x%x    [  %s  %d]  \r\n",ipc_socket,__FUNCTION__,__LINE__);  
        SCI_TRACE_LOW(" LWM2MDEBUG  data.sock :0x%x    [  %s  %d]  \r\n",data.sock,__FUNCTION__,__LINE__);  
        FD_ZERO(&readfds);
        FD_ZERO(&exceptfds);
        FD_SET(ipc_socket, &readfds);

        if(data.sock > 0)
        {
            FD_SET(data.sock, &readfds);
            FD_SET(data.sock, &exceptfds);
            SCI_TRACE_LOW(" LWM2MDEBUG  data.sock   : 0x%x    [%s  %d]\n",data.sock,__FUNCTION__,__LINE__);            
        }
        else
        {
            OSI_LOGXI(OSI_LOGPAR_S, 0x10007baa, "Trying to bind LWM2M Client to port %s", portstr);
            data.sock = create_socket((const char *)portstr, data.addressFamily);
            SCI_TRACE_LOW(" LWM2MDEBUG  data.sock   : 0x%x    [%s  %d]\n",data.sock,__FUNCTION__,__LINE__);

            if (data.sock > 0)
            {
                LOG_ARG( "opening data socket success: %d", data.sock);
                #ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
                #if LWIP_RFACK
                int ret = lwip_setRFAckCallback(data.sock, lwm2m_ack_callback, lwm2mH);
                if(ret < 0)
                    LOG_ARG( "set socket rf ack return %d", ret);
                #endif
                #endif
                FD_SET(data.sock, &readfds);
                FD_SET(data.sock, &exceptfds);
            }
            else
            {
                LOG_ARG("opening data socket[%d] failed: %d", data.sock, errno);
                goto error_exit;
            }
        }

#ifndef LWM2M_FOTA_THREAD

        /* if We do not use muti-thread for fota. */
        /* We add fota socket to select here.     */
        if(lwm2mH->fota_context.fota_socket > 0)
        {
            FD_SET(lwm2mH->fota_context.fota_socket, &readfds);
            FD_SET(lwm2mH->fota_context.fota_socket, &exceptfds);
            OSI_LOGW(0x100094b2, "lwm2m fota socket[%d] add select maxfd[%d]", lwm2mH->fota_context.fota_socket, maxfd);
        }
#endif
        OSI_LOGI(0x10007bc8, "max fd  set  data.sock > ipc_socket :   %d  !.  ",(data.sock > ipc_socket));
        maxfd = (data.sock > ipc_socket ? data.sock : ipc_socket);
        maxfd = (lwm2mH->fota_context.fota_socket > maxfd ? lwm2mH->fota_context.fota_socket : maxfd);
        maxfd = maxfd + 1;
        if(lwm2mH->quit ==0)
        {
        /*
         * This function does two things:
         *  - first it does the work needed by liblwm2m (eg. (re)sending some packets).
         *  - Secondly it adjusts the timeout value (default 60s) depending on the state of the transaction
         *    (eg. retransmission) and the time between the next operation
         */
        result = lwm2m_step(lwm2mH, &(tv.tv_sec));
        LOG_ARG(" -> State: ");
        switch (lwm2mH->state)
        {
        case STATE_INITIAL:
            LOG_ARG( "STATE_INITIAL");
            break;
        case STATE_BOOTSTRAP_REQUIRED:
           LOG_ARG("STATE_BOOTSTRAP_REQUIRED");
            // Bug 1888496. result != 0 that means bootstrap/registration failed.
            // If server reject client, then exit lwm2m thread.
            // If server unreachable, clear state and continue to bootstrap/registration.
            if(lwm2mH->maxRetriesReached)
            {
                // close the invalid sock
                close(data.sock);
                data.sock = 0;

                // reset the lwm2m status to initial
                connection_free(data.connList);
                data.connList = NULL;
                lwm2m_set_all_servers_session_to_null(lwm2mH);
                lwm2m_delete_context_list(lwm2mH);
                lwm2mH->state = STATE_INITIAL;
                result = 0;
                lwm2mH->maxRetriesReached = false;
            }
            break;
        case STATE_BOOTSTRAPPING:
            LOG_ARG( "STATE_BOOTSTRAPPING");
            if(lwm2mH->maxRetriesReached)
            {
                // close the invalid sock
                close(data.sock);
                data.sock = 0;

                // reset the lwm2m status to initial
                connection_free(data.connList);
                data.connList = NULL;
                lwm2m_set_all_servers_session_to_null(lwm2mH);
                lwm2m_delete_context_list(lwm2mH);
                lwm2mH->state = STATE_INITIAL;
                result = 0;
                lwm2mH->maxRetriesReached = false;
            }
            break;
        case STATE_REGISTER_REQUIRED:
            if(1 != lwm2mH->tlspeer_refused){
                connection_free(data.connList);
                data.connList = NULL;
                lwm2m_set_all_servers_session_to_null(lwm2mH);
                lwm2m_delete_context_list(lwm2mH);
                lwm2mH->state = STATE_INITIAL;
                result = 0;
            }

            LOG_ARG("STATE_REGISTER_REQUIRED");
            break;
        case STATE_REGISTERING:
            LOG_ARG( "STATE_REGISTERING");
            break;
        case STATE_READY:
            LOG_ARG( "STATE_READY");
            break;
        default:
            LOG_ARG("Unknown...");
            break;
        }
        if (result != 0)
        {
            LOG_ARG("lwm2m_step() failed: 0x%X", result);
            goto error_exit;
        }
#ifdef LWM2M_BOOTSTRAP
        update_bootstrap_info(&previousState, lwm2mH);
#endif
        }

        LOG_ARG(" goto sm9 \r\n" );
#ifdef CONFIG_NET_CTIOTSM_SUPPORT

        if((LWM2M_SERVER_AEP == lwm2mH->serverType) && (STATE_BOOTSTRAPPING == lwm2mH->state) && (STATE_BS_FINISHED == bootstrap_getStatus(lwm2mH)) && (1 == lwm2mH->isSM9Require)){

            client_data_t *userdata = (client_data_t *)(lwm2mH->userData);
            lwm2m_object_t * object = userdata->securityObjP;
            if (NULL != object) {
                lwm2m_data_t * dataP;
                int size = 2;
                uint16_t paramFd;
                uint16_t priKeyFd;

                dataP = lwm2m_data_new(size);
                if (dataP == NULL){
                    LOG_ARG("lwm2m_data_new failed\n");
                    goto error_exit;
                }
                dataP[0].id = LWM2M_SECURITY_SERVER_PUBLIC_KEY_ID;
                dataP[1].id = LWM2M_SECURITY_SECRET_KEY_ID;

                lwm2m_list_t * instancelist = object->instanceList;
                lwm2m_list_t * instance = LWM2M_LIST_FIND((lwm2m_list_t*)instancelist,0);
                 LOG_ARG( " +++++ Security object, instances=%p", instance);
                if (instance != NULL)
                {
                    if (object->readFunc(instance->id, &size, &dataP, object) != COAP_205_CONTENT)
                    {
                        lwm2m_data_free(size, dataP);
                        LOG_ARG("object->readFunc failed\n");
                        goto error_exit;
                    }
                    uint8_t gmem[SM2_ECS_IBS_TVAR_MEM_SIZE] = {0};
                    uint8_t pmem[SM2_ECS_IBS_PARAM_MEM_SIZE] = {0};

                    xlib_init();
                    sm2_init_param_ex(gmem, sizeof(gmem), pmem, sizeof(pmem));

                    uint8_t ret = sm2_load_prvkey(lwm2mH->skey, sizeof(lwm2mH->skey), 0x800);
                    if(ret != 0){
                        LOG_ARG("sm2_load_prvkey failed\n");
                        lwm2m_data_free(size, dataP);
                        goto error_exit;
                    }

                    /*ret = sm2_load_pubkey(lwm2mH->pkey, lwm2mH->plen);
                    if(ret != 0){
                        OSI_LOGI(0, "sm2_load_pubkey failed\n");
                        goto error_exit;
                    }*/
                    uint8_t plain_SM9prvkey[512]={0};
                    uint16_t sm9_len=512;
                    ret = sm2_dec(dataP[1].value.asBuffer.buffer, dataP[1].value.asBuffer.length, plain_SM9prvkey, &sm9_len);
                    if(ret != 0){
                        LOG_ARG( "sm2_dec failed \n");
                        lwm2m_data_free(size, dataP);
                        goto error_exit;
                    }
                    if (!prvIndexToFd(lwm2mH->index, &paramFd, &priKeyFd))
                    {
                        LOG_ARG( "AT+CTSIBCIMPORTKEY IndexToFd error");
                        lwm2m_data_free(size, dataP);
                        goto error_exit;
                    }
                    LOG_ARG("sm2_dec dataP[0].value.asBuffer.length=%d dataP[1].value.asBuffer.length=%d ,sm9_len=%d\n",dataP[0].value.asBuffer.length,dataP[1].value.asBuffer.length,sm9_len);

                    uint32_t cparam_hexlen = strlen(cparam_str)/2 +2;
                    uint8_t* cparam_hex= lwm2m_malloc(cparam_hexlen);
                    int hexres = prv_hexstr_to_hex((uint8_t*)cparam_str,strlen(cparam_str),cparam_hex,&cparam_hexlen);
                    if(hexres < 0){
                        lwm2m_free(cparam_hex);
                        lwm2m_data_free(size, dataP);
                        goto error_exit;}
                    if(1 == lwm2mH->type){//require sign key

                        int32_t ret1 = import_key_dev(3, 1, plain_SM9prvkey, sm9_len, 0);//param1:sign private key;param2:fd1
                        if(ret1 != sm9_len){
                            LOG_ARG( "sm9 import_key_dev private key failed ret1=%d\n",ret1);
                            lwm2m_data_free(size, dataP);
                            lwm2m_free(cparam_hex);
                            goto error_exit;
                        }
                        //ret1 = import_key_dev(2, paramFd,dataP[0].value.asBuffer.buffer,dataP[0].value.asBuffer.length,0);
           
                        ret1 = import_key_dev(2, 0, cparam_hex,cparam_hexlen,0);//param1:verify public key;param2:fd0
                        lwm2m_free(cparam_hex);
                        if(ret1 != cparam_hexlen){
                            LOG_ARG("sm9 import_key_dev public key failed ret1=%d",ret1);
                            lwm2m_data_free(size, dataP);
                            goto error_exit;
                        }
                    }else if(2 == lwm2mH->type){//require encrypt key
                        int32_t ret1 = import_key_dev(5, 3, plain_SM9prvkey, sm9_len, 0);//param1:decrypt private key;param2:fd3
                        if(ret1 != sm9_len){
                            LOG_ARG("sm9 import_key_dev private key failed ret1=%d\n",ret1);
                            lwm2m_data_free(size, dataP);
                            lwm2m_free(cparam_hex);
                            goto error_exit;
                        }
                        //ret1 = import_key_dev(4, paramFd,dataP[0].value.asBuffer.buffer,dataP[0].value.asBuffer.length,0);
                        ret1 = import_key_dev(4, 2,cparam_hex,cparam_hexlen,0);//param1:emcrypt public key;param2:fd2
                        lwm2m_free(cparam_hex);
                        if(ret1 != cparam_hexlen){
                            LOG_ARG("sm9 import_key_dev public key failed ret1=%d\n",ret1);
                            lwm2m_data_free(size, dataP);
                            goto error_exit;
                        }
                    }else{
                        LOG_ARG("%d == lwm2mH->type is not supported\n",lwm2mH->type);
                        lwm2m_data_free(size, dataP);
                        lwm2m_free(cparam_hex);
                        goto error_exit;
                    }
                    isquit = 1;
                    lwm2m_data_free(size, dataP);
                    goto error_exit;
                }
            }
        }
#endif
        /*
         * This part will set up an interruption until an event happen on SDTIN or the socket until "tv" timed out (set
         * with the precedent function)
         */
        LOG_ARG("lwm2m_select timeout:%ld",tv.tv_sec);
        result = select(maxfd, &readfds, NULL, &exceptfds, &tv);
        LOG_ARG( "lwm2m_select result = %d",result);
        if (result < 0)
        {
            SCI_Sleep(tv.tv_sec*1000);
            if (errno != EINTR)
            {
              LOG_ARG("Error in select(): %d %s", errno, strerror(errno));
            }
        }
        else if (result > 0)
        {
            uint8_t buffer[MAX_PACKET_SIZE];
            int numBytes = 0;

            if (FD_ISSET(data.sock, &exceptfds))
            {
                LOG_ARG("Error set data.sock");

                // close the invalid sock
                close(data.sock);
                data.sock = 0;

                // reset the timeout for re-connection
                timeout = 30;

                // reset the lwm2m status to initial
                connection_free(data.connList);
                data.connList = NULL;
                lwm2m_set_all_servers_session_to_null(lwm2mH);
                lwm2m_delete_context_list(lwm2mH);
                lwm2mH->state = STATE_INITIAL;

                // notify the app return to at
                lwm2mPostEvent(lwm2mH->ref, EVETN_IND,LWM2M_EVENT_REG_FAILED,0,0);
            }

#ifndef LWM2M_FOTA_THREAD
            if (FD_ISSET(lwm2mH->fota_context.fota_socket, &exceptfds))
            {
                //OSI_LOGI(0x10007bb9, "Error set data.sock");
                // close the invalid sock
                close(lwm2mH->fota_context.fota_socket);
                lwm2mH->fota_context.fota_socket = 0;
                LOG_ARG( "lwm2m select error. close socket %d", lwm2mH->fota_context.fota_socket);
                notify_fota_state(LWM2M_FOTA_STATE_IDLE, LWM2M_FOTA_RESULT_CONNECTION_LOST, lwm2mH->ref);
            }
#endif
            
            /*
             * If an event happens on the socket
             */
            SCI_TRACE_LOW( "  LWM2MDEBUG      [%s  %d]\n",__FUNCTION__,__LINE__);
            if (FD_ISSET(data.sock, &readfds))
            {
                struct sockaddr_storage addr;
                socklen_t addrLen;

                addrLen = sizeof(addr);
                SCI_TRACE_LOW( "  LWM2MDEBUG      [%s  %d]\n",__FUNCTION__,__LINE__);
                /*
                 * We retrieve the data received
                 
                 */
                  
                if (psk != NULL || pskId != NULL)
                    numBytes = recvfrom(data.sock, buffer, MAX_PACKET_SIZE, MSG_PEEK, (struct sockaddr *)&addr, &addrLen);
                else 
                    numBytes = recvfrom(data.sock, buffer, MAX_PACKET_SIZE, 0, (struct sockaddr *)&addr, &addrLen);
                if (0 > numBytes)
                {
                    OSI_LOGXI(OSI_LOGPAR_IS, 0x10007bba, "Error in recvfrom(): %d %s", errno, strerror(errno));

                    if(errno == 9)
                    {
                        // close the invalid sock
                        close(data.sock);
                        data.sock = 0;

                        // reset the timeout for re-connection
                        timeout = 30;

                        // reset the lwm2m status to initial
                        connection_free(data.connList);
                        data.connList = NULL;
                        lwm2m_set_all_servers_session_to_null(lwm2mH);
                        lwm2m_delete_context_list(lwm2mH);
                        lwm2mH->state = STATE_INITIAL;

                        // notify the app return to at
                        if(bootstrapRequested == false)
                            lwm2mPostEvent(lwm2mH->ref, SERVER_REGCMD_RESULT_IND, 0, 0, 0);
                    }
                }
                else if (0 < numBytes)
                {
#if defined(LWIP_IPV4_ON) && defined(LWIP_IPV6_ON)
                    char s[INET6_ADDRSTRLEN];
#else
#ifdef LWIP_IPV4_ON
                    char s[INET_ADDRSTRLEN];
#else
                    char s[INET6_ADDRSTRLEN];
#endif
#endif
                    in_port_t port = 0;

#if defined (WITH_TINYDTLS) || defined (CONFIG_LWM2M_TLS_SUPPORT) 
                    dtls_connection_t * connP;
#else
                    connection_t * connP;
#endif
#ifdef LWIP_IPV4_ON
                    if (AF_INET == addr.ss_family)
                    {
                        struct sockaddr_in *saddr = (struct sockaddr_in *)&addr;
                        inet_ntop(saddr->sin_family, &saddr->sin_addr, s, INET_ADDRSTRLEN);
                        port = saddr->sin_port;
                    }
#endif
#ifdef LWIP_IPV6_ON
                    if (AF_INET6 == addr.ss_family)
                    {
                        struct sockaddr_in6 *saddr = (struct sockaddr_in6 *)&addr;
                        inet_ntop(saddr->sin6_family, &saddr->sin6_addr, s, INET6_ADDRSTRLEN);
                        port = saddr->sin6_port;
                    }
#endif
                    OSI_LOGXI(OSI_LOGPAR_ISI, 0x10007699, "%d bytes received from [%s]:%hu", numBytes, s, ntohs(port));

                    /*
                     * Display it in the STDERR
                     */
                    output_buffer(stderr, buffer, numBytes, 0);

                    connP = connection_find(data.connList, &addr, addrLen);
                    if (connP != NULL)
                    {
                        /*
                         * Let liblwm2m respond to the query depending on the context
                         */
#if defined (WITH_TINYDTLS) || defined (CONFIG_LWM2M_TLS_SUPPORT) 
                        if (0 != connection_handle_packet(connP, buffer, numBytes))
                        {
                             LOG_ARG("error handling message %d",result);
                        }
#else
                        lwm2m_handle_packet(lwm2mH, buffer, numBytes, connP);

#endif
                        LOG_ARG("numBytes=%d",numBytes);
                        conn_s_updateRxStatistic(lwm2mH, numBytes, false);
                    }
                    else
                    {
                        LOG_ARG( "received bytes ignored!");
                    }
                }
                else
                {
                    LOG_ARG("0 in recvfrom()");
                }
            }

            /*
             * If the event happened on the SDTIN
             */
            SCI_TRACE_LOW( "  LWM2MDEBUG      [%s  %d]\n",__FUNCTION__,__LINE__);
            if (FD_ISSET(ipc_socket, &readfds))
            {
                numBytes = read(ipc_socket, buffer, MAX_PACKET_SIZE - 1);
                SCI_TRACE_LOW(" [ LWM2MDEBUG]   ipc received  [%s %d]\n",__FUNCTION__,__LINE__);
                if (numBytes > 1)
                {
                    int ret;
                    buffer[numBytes] = 0;
                    SCI_TRACE_LOW(" [ LWM2MDEBUG]   ipc received  [%s %d]\n",__FUNCTION__,__LINE__);
                    /*
                     * We call the corresponding callback of the typed command passing it the buffer for further arguments
                     */
                     ret = handle_command(commands, (char*)buffer);
                    if (ret < 0)
                        lwm2mPostEvent(lwm2mH->ref,OBJ_OPERATE_RSP_IND,-1,-1,0);
                    else if (ret == 0)
                        lwm2mPostEvent(lwm2mH->ref,OBJ_OPERATE_RSP_IND,0,-1,0);
                    else
                        OSI_LOGXI(OSI_LOGPAR_SI, 0x10007bbe, "commands:%s ignore reponse:%d",(char *)commands,ret);
                }
            }

#ifndef LWM2M_FOTA_THREAD 
            if (FD_ISSET(lwm2mH->fota_context.fota_socket, &readfds))
            {  
                fota_msg_prosess(&data, buffer);
            }
#endif

        }
    }
    isquit = lwm2m_configs[ref]->isquit;

error_exit:

    /*
     * Finally when the loop is left smoothly - asked by user in the command line interface - we unregister our client from it
     */
#if(defined WITH_TINYDTLS) || (defined CONFIG_LWM2M_TLS_SUPPORT)
    free(pskBuffer);
#endif
    LOG_ARG( "exit");

#ifdef CONFIG_LWM2M_OCEANCONNECT_CUS_SUPPORT
    gHandshakeState = HANDSHAKE_INIT;
#endif

#ifdef LWM2M_BOOTSTRAP
    close_backup_object();
#endif
    if(lwm2mH != NULL)
    {
        clean_all_objects(lwm2mH);
        lwm2m_close(lwm2mH);
    }
    if(objArray[0])
        lwm2m_free(objArray[0]);
    if(objArray[1])
        lwm2m_free(objArray[1]);
    if(objArray[2])
        lwm2m_free(objArray[2]);
    if(objArray[3])
        lwm2m_free(objArray[3]);
    if(objArray[4])
        lwm2m_free(objArray[4]);
#ifdef CONFIG_LWM2M_ONENET_DM_SUPPORT
    if(objArray[5])
        lwm2m_free(objArray[5]);
#endif
    close(data.sock);

#ifdef MEMORY_TRACE
    if (isquit == 1)
    {
        trace_print(0, 1);
    }
#endif

    return isquit==0?-1:1;
}

