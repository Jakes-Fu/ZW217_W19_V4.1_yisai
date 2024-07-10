/*******************************************************************************
 * Copyright (c) 2009, 2018 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs, Allan Stockdill-Mander - initial implementation
 *    Ian Craggs - fix for bug #409702
 *    Ian Craggs - allow compilation for OpenSSL < 1.0
 *    Ian Craggs - fix for bug #453883
 *    Ian Craggs - fix for bug #480363, issue 13
 *    Ian Craggs - SNI support
 *    Ian Craggs - fix for issues #155, #160
 *******************************************************************************/

/**
 * @file
 * \brief SSL  related functions
 *
 */
#include "paho_config.h"
#if defined(MBEDTLS) || defined(OPENSSL)

#include "SocketBuffer.h"
#include "MQTTClient.h"
#include "MQTTProtocolOut.h"
#include "TLSSocket.h"

/*zyzyzy 20180929 #include "Log.h"*/
/*zyzyzy 20180929 #include "StackTrace.h"*/
#include "Socket.h"

/*zyzyzy 20180929 #include "Heap.h"*/

#include <string.h>

extern Sockets s;

int SSL_set_session(SSL *ssl, SSL_SESSION *session)
{
    return 1;
}

SSL_SESSION *SSL_get1_session(SSL *ssl)
{
    //SSL_SESSION session = 0;
    return NULL;
}

static void paho_debug(void *ctx, int level,
                       const char *file, int line,
                       const char *param)
{
    ((void)level);
    SCI_TRACE_LOW("pahomqtt_tls :%s", param);
}

#if 0
static int SSLSocket_createContext(networkHandles *net, MQTTClient_SSLOptions *opts)
{
    int ret = -1;
    const char *pers = "paho_client";
    SCI_TRACE_LOW("goto SSLSocket_createContext");
    

    net->ssl = malloc(sizeof(mbedtls_ssl_context));
    if (net->ssl == NULL)
    {
        SCI_TRACE_LOW("net->ssl MALLOC FAILED:%d!\n", sizeof(mbedtls_ssl_context));
        goto err;
    }
    mbedtls_ssl_init(net->ssl);

    net->ctx = malloc(sizeof(SSL_CTX));
    if (net->ctx == NULL)
    {
        SCI_TRACE_LOW("net->ctx MALLOC FAILED:%d!\n", sizeof(SSL_CTX));
        goto err;
    }
    
    net->ctx->server_fd.fd = -1;

    mbedtls_ssl_config_init(&net->ctx->conf);

    mbedtls_x509_crt_init(&net->ctx->cacert);

    mbedtls_ctr_drbg_init(&net->ctx->ctr_drbg);

    mbedtls_entropy_init(&net->ctx->entropy);

    mbedtls_pk_init(&net->ctx->pkey);

    mbedtls_x509_crt_init(&net->ctx->clicert);

    if ((ret = mbedtls_ctr_drbg_seed(&net->ctx->ctr_drbg, mbedtls_entropy_func, &net->ctx->entropy,
                                     (const unsigned char *)pers, strlen(pers))) != 0)
    {
        SCI_TRACE_LOW("mbedtls_ctr_drbg_seed failed...,ret=%d\n", ret);
        goto err;
    }
    ret = mbedtls_x509_crt_parse(&net->ctx->cacert, (const unsigned char *)opts->trustStore,
                                 strlen(opts->trustStore) + 1);
    if (ret < 0)
    {

        SCI_TRACE_LOW("mbedtls_x509_crt_parse ca_pem failed...,ret=0x%x\n", -ret);
        goto err;
    }

    ret = mbedtls_x509_crt_parse(&net->ctx->clicert, (const unsigned char *)opts->keyStore,
                                 strlen(opts->keyStore) + 1);
    if (ret < 0)
    {
        SCI_TRACE_LOW("mbedtls_x509_crt_parse cert_pem failed...,ret=-0x%x\n", -ret);
        goto err;
    }

    ret = mbedtls_pk_parse_key(&net->ctx->pkey, (const unsigned char *)opts->privateKey,
                               strlen(opts->privateKey) + 1, NULL, 0);

    if (ret < 0)
    {
        SCI_TRACE_LOW("mbedtls_pk_parse_key key_pem failed...,ret=-0x%x\n", -ret);
        goto err;
    }

    mbedtls_ssl_conf_ca_chain(&net->ctx->conf, &net->ctx->cacert, NULL);
    ret = mbedtls_ssl_conf_own_cert(&net->ctx->conf, &net->ctx->clicert, &net->ctx->pkey);
    if (ret < 0)
    {
        SCI_TRACE_LOW("mbedtls_ssl_conf_own_cert failed...,ret=-0x%x\n", -ret);
        goto err;
    }

    if ((ret = mbedtls_ssl_setup(net->ssl, &net->ctx->conf)) != 0)
    {
        SCI_TRACE_LOW("mbedtls_ssl_setup failed ret = %d\n", ret);
        goto err;
    }

    if ((ret = mbedtls_ssl_config_defaults(&net->ctx->conf,
                                           MBEDTLS_SSL_IS_CLIENT,
                                           MBEDTLS_SSL_TRANSPORT_STREAM,
                                           MBEDTLS_SSL_PRESET_DEFAULT)) != 0)
    {
        SCI_TRACE_LOW("mbedtls_ssl_config_defaults failed ret = %d\n", ret);
        goto err;
    }

    if (opts->enabledCipherSuites)
    {
        mbedtls_ssl_conf_ciphersuites(&net->ctx->conf, (const int *)opts->enabledCipherSuites);
    }
    mbedtls_ssl_conf_authmode(&net->ctx->conf, MBEDTLS_SSL_VERIFY_NONE);

    mbedtls_ssl_conf_rng(&net->ctx->conf, mbedtls_ctr_drbg_random, &net->ctx->ctr_drbg);
    mbedtls_debug_set_threshold(0);
    mbedtls_ssl_conf_dbg(&net->ctx->conf, paho_debug, NULL);

    //mbedtls_timing_delay_context *timer = malloc(sizeof (mbedtls_timing_delay_context));
    mbedtls_ssl_conf_read_timeout(&net->ctx->conf, 30000);
    //mbedtls_ssl_set_timer_cb(sock->ssl, timer, mbedtls_timing_set_delay,
    //  mbedtls_timing_get_delay );

    mbedtls_ssl_set_bio(net->ssl, &net->ctx->server_fd, mbedtls_net_send, mbedtls_net_recv, mbedtls_net_recv_timeout);
    //mbedtls_ssl_conf_min_version(&(tlssock_ptr->conf), MBEDTLS_SSL_MAJOR_VERSION_3, (int)min_ver);
    //mbedtls_ssl_conf_max_version(&(tlssock_ptr->conf), MBEDTLS_SSL_MAJOR_VERSION_3, (int)max_ver);

    //exit:
    return ret;
err:
    if (net->ssl)
    {
        if (net->ssl->p_timer)
        {
            free(net->ssl->p_timer);
            net->ssl->p_timer = NULL;
        }
        mbedtls_ssl_free(net->ssl);
        free(net->ssl);
        net->ssl = NULL;
    }
    if (net->ctx)
    {

        mbedtls_net_free(&net->ctx->server_fd);

        mbedtls_entropy_free(&net->ctx->entropy);

        mbedtls_ssl_config_free(&net->ctx->conf);

        mbedtls_x509_crt_free(&net->ctx->cacert);

        mbedtls_ctr_drbg_free(&net->ctx->ctr_drbg);

        mbedtls_x509_crt_free(&net->ctx->clicert);

        mbedtls_pk_free(&net->ctx->pkey);

        free(net->ctx);
        net->ctx = NULL;
    }
    return ret;
}

int SSLSocket_setSocketForSSL(networkHandles *net, MQTTClient_SSLOptions *opts,
                              const char *hostname, size_t hostname_len)
{
    int rc = 1;
    SCI_TRACE_LOW("pahomqtt SSLSocket_setSocketForSSL");
    

    if ((rc = SSLSocket_createContext(net, opts)) == 0)
    {

        //int i;
        SCI_TRACE_LOW("pahomqtt SSLSocket_setSocketForSSL,opts->enableServerCertAuth=%d", opts->enableServerCertAuth);

        if (opts->enableServerCertAuth)
            mbedtls_ssl_conf_authmode(&net->ctx->conf, MBEDTLS_SSL_VERIFY_REQUIRED);

        net->ctx->server_fd.fd = net->socket;
        rc = 1;
    }
    SCI_TRACE_LOW("pahomqtt SSLSocket_setSocketForSSL,return %d", rc);
    return rc;
}
#endif

//modify by xiameng.yang  ------------>
void _mbedtls_debug(void *ctx, int level, const char *file, int line, const char *str)
{
    ((void) level);
    SCI_TRACE_LOW(" mbedtls [%s:%d] %s", file, line, str);    
}
unsigned int mqtt_avRandom()
{
    return (((unsigned int)rand() << 16) + rand());
}

int mqtt_ssl_random(void *p_rng, unsigned char *output, size_t output_len)
{
    uint32_t rnglen = output_len;
    uint8_t   rngoffset = 0;

    while (rnglen > 0) 
    {
        *(output + rngoffset) = (unsigned char)mqtt_avRandom() ;
        rngoffset++;
        rnglen--;
    }
    return 0;
}
int mqtt_ssl_parse_crt(mbedtls_x509_crt *crt)
{
    char buf[1024] = {0};
    mbedtls_x509_crt *local_crt = crt;
    int i = 0;
    while (local_crt) 
    {
        mbedtls_x509_crt_info(buf, sizeof(buf) - 1, "", local_crt);
        {
            char str[512];
            const char *start, *cur;
            start = buf;
            for (cur = buf; *cur != '\0'; cur++) 
	   {
                if (*cur == '\n') 
	       {
                    size_t len = cur - start + 1;
                    if (len > 511) 
		  {
                        len = 511;
                    }
                    memcpy(str, start, len);
                    str[len] = '\0';
                    start = cur + 1;
                    SCI_TRACE_LOW("[mqtt_mbedtls] mbedtls %s", str);
                }
            }
        }
        SCI_TRACE_LOW("[mqtt_mbedtls] mbedtls crt content:%u", (uint32_t)strlen(buf));
        local_crt = local_crt->next;
        i++;
    }
    return i;
}

int mqtt_real_confirm(int verify_result)
{
    SCI_TRACE_LOW("[mqtt_mbedtls] mbedtls certificate verification result: 0x%02x", verify_result);
    #if defined(FORCE_SSL_VERIFY)
    if ((verify_result & MBEDTLS_X509_BADCERT_EXPIRED) != 0) 
    {
        SCI_TRACE_LOW("[mqtt_mbedtls] mbedtls ! fail ! ERROR_CERTIFICATE_EXPIRED");
        return -1;
    }

    if ((verify_result & MBEDTLS_X509_BADCERT_REVOKED) != 0) 
    {
        SCI_TRACE_LOW("[mqtt_mbedtls] mbedtls ! fail ! server certificate has been revoked");
        return -1;
    }

    if ((verify_result & MBEDTLS_X509_BADCERT_CN_MISMATCH) != 0) 
    {
        SCI_TRACE_LOW("[mqtt_mbedtls] mbedtls ! fail ! CN mismatch");
        return -1;
    }

    if ((verify_result & MBEDTLS_X509_BADCERT_NOT_TRUSTED) != 0)
    {
       SCI_TRACE_LOW("[mqtt_mbedtls] mbedtls ! fail ! self-signed or not signed by a trusted CA");
        return -1;
    }
    #endif

    return 0;
}


extern char * g_host;
static int SSLSocket_createContext(networkHandles *net, const char *ca_crt, size_t ca_len,
                         mbedtls_x509_crt *crt509_cli, const char *cli_crt, size_t cli_len,
                         mbedtls_pk_context *pk_cli, const char *cli_key, size_t key_len,  const char *cli_pwd, size_t pwd_len)
{
    int ret = -1;
    
     /* Initialize the RNG and the session data*/
    #if 1 //defined(MBEDTLS_DEBUG_C)
    mbedtls_debug_set_threshold(0);
    #endif

    net->ssl = malloc(sizeof(mbedtls_ssl_context));
        SCI_TRACE_LOW(" MQTT sizeof(mbedtls_ssl_context):  %d [ %s  %d]",sizeof(mbedtls_ssl_context),__FUNCTION__,__LINE__);
   
    
    if (net->ssl == NULL)
    {
        SCI_TRACE_LOW("net->ssl MALLOC FAILED:%d!\n", sizeof(mbedtls_ssl_context));
        //return ret;
    }
    mbedtls_ssl_init(net->ssl);   


    net->ctx = malloc(sizeof(SSL_CTX));
    //net->ctx = ctx;
    if (net->ctx == NULL)
    {
        SCI_TRACE_LOW("net->ctx MALLOC FAILED:%d!\n", sizeof(SSL_CTX));
        //return ret;
    }
   
    memset(net->ctx, 0, sizeof(SSL_CTX));
    net->ctx->server_fd.fd = -1;

    mbedtls_ssl_config_init(&net->ctx->conf);

    mbedtls_x509_crt_init(&net->ctx->cacert);

    SCI_TRACE_LOW("[mqtt_mbedtls] mbedtls Loading the CA root certificate ...");
    if (NULL != ca_crt) 
    {
        if (0 != (ret = mbedtls_x509_crt_parse(&net->ctx->cacert, (const unsigned char *)ca_crt, ca_len))) 
        {
            SCI_TRACE_LOW("[mqtt_mbedtls] mbedtls failed ! x509parse_crt returned -0x%04x", -ret);
            return ret;
        }
    }
    
    //mqtt_ssl_parse_crt(&net->ctx->cacert);
    SCI_TRACE_LOW("[mqtt_mbedtls] mbedtls ok (%d skipped)", ret);

    #if defined(MBEDTLS_X509_CRT_PARSE_C)
    #if defined(MBEDTLS_CERTS_C)
    if(&net->ctx->clicert !=NULL)
    {
        mbedtls_x509_crt_init(&net->ctx->clicert);
    }
    mbedtls_pk_init(&net->ctx->pkey);
    #endif
    if (cli_crt != NULL && cli_key != NULL) 
    {
        #if defined(MBEDTLS_CERTS_C)
        SCI_TRACE_LOW("[mqtt_mbedtls] mbedtls start prepare client cert .");
        ret = mbedtls_x509_crt_parse(&net->ctx->clicert, (const unsigned char *) cli_crt, cli_len);
        #else
        {
            ret = 1;
            SCI_TRACE_LOW("[mqtt_mbedtls] MBEDTLS_CERTS_C not defined.");
        }
        #endif
        if (ret != 0) 
        {
            SCI_TRACE_LOW("[mqtt_mbedtls] mbedtls failed!  mbedtls_x509_crt_parse returned -0x%x\n", -ret);
            return ret;
        }

       #if defined(MBEDTLS_CERTS_C)
       SCI_TRACE_LOW("[mqtt_mbedtls] mbedtls start mbedtls_pk_parse_key[%s]", cli_pwd);
       ret = mbedtls_pk_parse_key(&net->ctx->pkey, (const unsigned char *) cli_key, key_len, (const unsigned char *) cli_pwd, pwd_len);
       #else
       {
           ret = 1;
           SCI_TRACE_LOW("[mqtt_mbedtls] mbedtls MBEDTLS_CERTS_C not defined.");
       }
       #endif

        if (ret != 0)
        {
            SCI_TRACE_LOW("[mqtt_mbedtls] mbedtls failed\n  !  mbedtls_pk_parse_key returned -0x%x\n", -ret);
            return ret;
        }
    }
    #endif /* MBEDTLS_X509_CRT_PARSE_C */    
    
    return 0;
}

int SSLSocket_setSocketForSSL(networkHandles *net, MQTTClient_SSLOptions *opts,
                              const char *hostname, size_t hostname_len)
{
    char *  ca_crt= NULL;
    size_t ca_len =0;
    mbedtls_x509_crt *crt509_cli =NULL;
    char *cli_crt =NULL;
    size_t cli_len =0;
    mbedtls_pk_context *pk_cli =NULL;
    char *cli_key =NULL;
    size_t key_len =0;
    char *cli_pwd =NULL;
    size_t pwd_len =0;
    int ret = 0;
    const char * host = g_host;     
    
    SCI_TRACE_LOW("pahomqtt SSLSocket_setSocketForSSL   hostname : %s    [ %s  %d]    \n",hostname,__FUNCTION__,__LINE__);
    

    if ((ret = SSLSocket_createContext(net, ca_crt,ca_len,crt509_cli,cli_crt,cli_len,pk_cli,cli_key,key_len,cli_pwd,pwd_len)) != 0)
    {
        SCI_TRACE_LOW("[mqtt mbedtls] mbedtls failed ! ssl_client_init returned -0x%04x", -ret);
         return -ret;    
    }

    net->ctx->server_fd.fd = net->socket;
    
    // 2. Setup stuff
    SCI_TRACE_LOW("[mqtt mbedtls] mbedtls  . Setting up the SSL/TLS structure...");
    if ((ret = mbedtls_ssl_config_defaults(&net->ctx->conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM,
                                           MBEDTLS_SSL_PRESET_DEFAULT)) != 0) 
    {
        SCI_TRACE_LOW("[mqtt mbedtls] mbedtls failed! mbedtls_ssl_config_defaults returned %d", ret);
        return ret;
    }

    mbedtls_ssl_conf_max_version(&net->ctx->conf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_3);
    mbedtls_ssl_conf_min_version(&net->ctx->conf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_3);

    SCI_TRACE_LOW("[mqtt mbedtls] mbedtls ok");

     if (ca_crt != NULL) 
    {
        #if defined(FORCE_SSL_VERIFY)
              mbedtls_ssl_conf_authmode(&net->ctx->conf, MBEDTLS_SSL_VERIFY_REQUIRED);
        #else
              mbedtls_ssl_conf_authmode(&net->ctx->conf, MBEDTLS_SSL_VERIFY_OPTIONAL);
        #endif
    } 
    else 
    {
        mbedtls_ssl_conf_authmode(&net->ctx->conf, MBEDTLS_SSL_VERIFY_NONE);
    }

    #if 0//defined(MBEDTLS_X509_CRT_PARSE_C)
        mbedtls_ssl_conf_ca_chain(&net->ctx->conf, &net->ctx->cacert, NULL);
    
        if ((ret = mbedtls_ssl_conf_own_cert(&net->ctx->conf, &net->ctx->clicert, &net->ctx->pkey)) != 0) 
        {
            SCI_TRACE_LOW("[mqtt mbedtls] mbedtls failed\n  ! mbedtls_ssl_conf_own_cert returned %d\n", ret);
            return ret;
        }
    #endif
    mbedtls_ssl_conf_rng(&net->ctx->conf, mqtt_ssl_random, NULL);
    mbedtls_ssl_conf_dbg(&net->ctx->conf, _mbedtls_debug, NULL);

    if ((ret = mbedtls_ssl_setup(net->ssl, &net->ctx->conf)) != 0)
    {
        SCI_TRACE_LOW("[mqtt mbedtls] mbedtls failed! mbedtls_ssl_setup returned %d", ret);
        return ret;
    }   
    
    if(host != NULL)
    {            
        mbedtls_ssl_set_hostname(net->ssl, host);
    }   
    
    mbedtls_ssl_conf_read_timeout(&net->ctx->conf, MQTT_TLS_READ_TIMEOUT);
    mbedtls_ssl_set_bio(net->ssl, &net->ctx->server_fd.fd, mbedtls_net_send, mbedtls_net_recv, mbedtls_net_recv_timeout);
    //mbedtls_ssl_set_bio(net->ssl, &net->ctx->server_fd.fd, mbedtls_net_send, mbedtls_net_recv, NULL);

  
    SCI_TRACE_LOW("mqtt ret : %d   [ %s  %d]\n ",ret,__FUNCTION__,__LINE__);
    return ret;
}

//modify by xiameng.yang < ------------


/*
 * Return value: 1 - success, TCPSOCKET_INTERRUPTED - try again, anything else is failure
 */
int SSLSocket_connect(SSL *ssl, int sock, const char *hostname, int verify, int (*cb)(const char *str, size_t len, void *u), void *u)
{
    int rc = -1;
    uint32_t ret ;
    
    SCI_TRACE_LOW("pahomqtt goto handshake  ssl->conf  0x%x    hostname :%s  [ %s  %d]",ssl->conf,hostname,__FUNCTION__,__LINE__);
    //mbedtls_ssl_set_bio(ssl, &sock, mbedtls_net_send, mbedtls_net_recv, mbedtls_net_recv_timeout);
    if ((rc = mbedtls_ssl_handshake(ssl)) != 0)
    {
        if (rc == MBEDTLS_ERR_SSL_WANT_READ || rc == MBEDTLS_ERR_SSL_WANT_WRITE)
        {
            rc = TCPSOCKET_INTERRUPTED;
        }
        SCI_TRACE_LOW("pahomqtt  handshake,+++return :%d", rc);
        return rc;
    }
	SCI_TRACE_LOW("[mqtt_mbedtls] mbedtls handshake ok");

    if (verify)
    {
        char *hostname_plus_null;
        hostname_plus_null = malloc(strlen(hostname) + 1u);
        MQTTStrncpy(hostname_plus_null, hostname, strlen(hostname) + 1u);
        if ((rc = mbedtls_ssl_set_hostname(ssl, hostname_plus_null)) == 0)
        {
            rc = 1;
        }
        else
        {

            SCI_TRACE_LOW("mbedtls_ssl_set_hostname failed...,ret=-0x%x\n", -rc);
        }
        free(hostname_plus_null);
    }
    ret = mbedtls_ssl_get_verify_result(ssl);
    if (ret != 0)
    {
        char vrfy_buf[512];

        SCI_TRACE_LOW(" pahomqtt  handshake failed\n");

        mbedtls_x509_crt_verify_info(vrfy_buf, sizeof(vrfy_buf), "  ! ", ret);

        SCI_TRACE_LOW("%s\n", vrfy_buf);
    }
    else
    {
        rc = 1;
        SCI_TRACE_LOW(" pahomqtt  handshake ok\n");
    }
    SCI_TRACE_LOW("pahomqtt  handshake,return :%d,verify=%d", rc, verify);
    return rc;
}

/**
 *  Reads one byte from a socket
 *  @param socket the socket to read from
 *  @param c the character read, returned
 *  @return completion code
 */
int SSLSocket_getch(SSL *ssl, int socket, char *c)
{
    int rc = SOCKET_ERROR;
    SCI_TRACE_LOW("SSLSocket_getch enter,SocketBuffer_getQueuedChar(socket, c)=%d", SocketBuffer_getQueuedChar(socket, c));
    //if ((rc = SocketBuffer_getQueuedChar(socket, c)) != SOCKETBUFFER_INTERRUPTED)
    //goto exit;
    mbedtls_ssl_set_bio(ssl, &socket, mbedtls_net_send, mbedtls_net_recv, NULL);
    if ((rc = mbedtls_ssl_read(ssl, (unsigned char *)c, (size_t)1)) < 0)
    {
        SCI_TRACE_LOW("mbedtls_ssl_read error = -0X%x", -rc);
        if (rc == MBEDTLS_ERR_SSL_WANT_READ || rc == MBEDTLS_ERR_SSL_WANT_WRITE)
        {
            rc = TCPSOCKET_INTERRUPTED;
            SocketBuffer_interrupted(socket, 0);
        }
    }
    else if (rc == 0)
        rc = SOCKET_ERROR; /* The return value from recv is 0 when the peer has performed an orderly shutdown. */
    else if (rc == 1)
    {
        SCI_TRACE_LOW("mbedtls_ssl_read111 rc=%d,c=%d", rc, *c);
        SocketBuffer_queueChar(socket, *c);
        rc = TCPSOCKET_COMPLETE;
    }
    else
    {
        SCI_TRACE_LOW("mbedtls_ssl_read rc=%d,c=%d", rc, *c);
    }
    //exit:

    return rc;
}

/**
 *  Attempts to read a number of bytes from a socket, non-blocking. If a previous read did not
 *  finish, then retrieve that data.
 *  @param socket the socket to read from
 *  @param bytes the number of bytes to read
 *  @param actual_len the actual number of bytes read
 *  @return completion code
 */
char *SSLSocket_getdata(SSL *ssl, int socket, size_t bytes, size_t *actual_len)
{
    int rc;
    char *buf;

    if (bytes == 0)
    {
        buf = SocketBuffer_complete(socket);
        goto exit;
    }

    buf = SocketBuffer_getQueuedData(socket, bytes, actual_len);
    *actual_len = 0;
    SCI_TRACE_LOW("+++pahomqtttls SSLSocket_getdata,pahomqtt,actual_len=%d", *actual_len);
    mbedtls_ssl_set_bio(ssl, &socket, mbedtls_net_send, mbedtls_net_recv, NULL);
    if ((rc = mbedtls_ssl_read(ssl, (unsigned char *)(buf + (*actual_len)), (int)(bytes - (*actual_len)))) < 0)
    {
        SCI_TRACE_LOW("mbedtls_ssl_read error = -0X%x", -rc);
        if (rc != MBEDTLS_ERR_SSL_WANT_READ && rc != MBEDTLS_ERR_SSL_WANT_WRITE)
        {
            buf = NULL;
            goto exit;
        }
    }
    else if (rc == 0) /* rc 0 means the other end closed the socket */
    {
        buf = NULL;
        goto exit;
    }
    else
        *actual_len += rc;

    SCI_TRACE_LOW("+++pahomqtttls SSLSocket_getdata,pahomqtt,actual_len=%d,bytes=%d,rc=%d", *actual_len, bytes, rc);

    if (*actual_len == bytes)
    {
        SocketBuffer_complete(socket);
        /* if we read the whole packet, there might still be data waiting in the SSL buffer, which
		isn't picked up by select.  So here we should check for any data remaining in the SSL buffer, and
		if so, add this socket to a new "pending SSL reads" list.
		*/
        SCI_TRACE_LOW("+++SSLSocket_getdata,pahomqtt,mbedtls_ssl_get_bytes_avail(ssl)=%d", mbedtls_ssl_get_bytes_avail(ssl));
        if (mbedtls_ssl_get_bytes_avail(ssl) > 0) //(SSL_pending(ssl) > 0) /* return no of bytes pending */
            SSLSocket_addPendingRead(socket);
    }
    else /* we didn't read the whole packet */
    {
        SocketBuffer_interrupted(socket, *actual_len);
        SCI_TRACE_LOW("pahomqtt SSL_read: %d bytes expected but %d bytes now received", bytes, *actual_len);
    }
exit:
    return buf;
}

static void SSLSocket_destroyContext(networkHandles *net)
{
    if (net->ssl)
    {
        if (net->ssl->p_timer)
        {
            free(net->ssl->p_timer);
            net->ssl->p_timer = NULL;
        }
        mbedtls_ssl_free(net->ssl);
        free(net->ssl);
        net->ssl = NULL;
    }
    if (net->ctx)
    {

        mbedtls_net_free(&net->ctx->server_fd);

        mbedtls_entropy_free(&net->ctx->entropy);

        mbedtls_ssl_config_free(&net->ctx->conf);

        mbedtls_x509_crt_free(&net->ctx->cacert);

        mbedtls_ctr_drbg_free(&net->ctx->ctr_drbg);

        mbedtls_x509_crt_free(&net->ctx->clicert);

        mbedtls_pk_free(&net->ctx->pkey);

        free(net->ctx);
        net->ctx = NULL;
    }
}

static List pending_reads = {NULL, NULL, NULL, 0, 0};

int SSLSocket_close(networkHandles *net)
{
    int rc = 1;

    /* clean up any pending reads for this socket */
    if (pending_reads.count > 0 && ListFindItem(&pending_reads, &net->socket, intcompare))
        ListRemoveItem(&pending_reads, &net->socket, intcompare);

    SSLSocket_destroyContext(net);

    return rc;
}

/* No SSL_writev() provided by OpenSSL. Boo. */
int SSLSocket_putdatas(SSL *ssl, int socket, char *buf0, size_t buf0len, int count, char **buffers, size_t *buflens, int *frees)
{
    int rc = 0;
    int i;
    char *ptr;
    iobuf iovec;

    iovec.iov_len = (ULONG)buf0len;
    for (i = 0; i < count; i++)
        iovec.iov_len += (ULONG)buflens[i];

    ptr = iovec.iov_base = (char *)malloc(iovec.iov_len);
    memcpy(ptr, buf0, buf0len);
    ptr += buf0len;
    for (i = 0; i < count; i++)
    {
        memcpy(ptr, buffers[i], buflens[i]);
        ptr += buflens[i];
    }
    SCI_TRACE_LOW("disc pahomqtttls SSLSocket_putdatas,socket=%d", socket);
    mbedtls_ssl_set_bio(ssl, &socket, mbedtls_net_send, mbedtls_net_recv, NULL);
    if ((rc = mbedtls_ssl_write(ssl, iovec.iov_base, iovec.iov_len)) == iovec.iov_len)
        rc = TCPSOCKET_COMPLETE;
    else
    {
        if (rc == MBEDTLS_ERR_SSL_WANT_WRITE)
        {
            int *sockmem = (int *)malloc(sizeof(int));
            int free = 1;

            SCI_TRACE_LOW("Partial write: incomplete write of %d bytes on SSL socket %d",
                            iovec.iov_len, socket);
            SocketBuffer_pendingWrite(socket, ssl, 1, &iovec, &free, iovec.iov_len, 0);
            *sockmem = socket;
            ListAppend(s.write_pending, sockmem, sizeof(int));
            FD_SET(socket, &(s.pending_wset));
            rc = TCPSOCKET_INTERRUPTED;
        }
        else
            rc = SOCKET_ERROR;
    }

    if (rc != TCPSOCKET_INTERRUPTED)
        {
            if(iovec.iov_base != NULL)
            {
                free(iovec.iov_base);
            }
        }
    else
    {
        int i;
        if (NULL != buf0)
        {
            free(buf0);
            buf0 = NULL;
        }
        for (i = 0; i < count; ++i)
        {
            if (frees[i])
            {
                free(buffers[i]);
                buffers[i] = NULL;
            }
        }
    }

    return rc;
}

void SSLSocket_addPendingRead(int sock)
{

    if (ListFindItem(&pending_reads, &sock, intcompare) == NULL) /* make sure we don't add the same socket twice */
    {
        int *psock = (int *)malloc(sizeof(sock));
        *psock = sock;
        ListAppend(&pending_reads, psock, sizeof(sock));
    }
    else
        SCI_TRACE_LOW("SSLSocket_addPendingRead: socket %d already in the list", sock);
}

int SSLSocket_getPendingRead(void)
{
    int sock = -1;

    if (pending_reads.count > 0)
    {
        sock = *(int *)(pending_reads.first->content);
        ListRemoveHead(&pending_reads);
    }
    return sock;
}

int SSLSocket_continueWrite(pending_writes *pw)
{
    int rc = 0;

    if ((rc = mbedtls_ssl_write(pw->ssl, pw->iovecs[0].iov_base, pw->iovecs[0].iov_len)) == pw->iovecs[0].iov_len)
    {
        /* topic and payload buffers are freed elsewhere, when all references to them have been removed */
        free(pw->iovecs[0].iov_base);
        SCI_TRACE_LOW("SSL continueWrite: partial write now complete for socket %d", pw->socket);
        rc = 1;
    }
    else
    {
        SCI_TRACE_LOW("SSL continueWrite: haven't finished writing the payload yet");
        //int sslerror = SSLSocket_error("SSL_write", pw->ssl, pw->socket, rc, NULL, NULL);
        if (rc == MBEDTLS_ERR_SSL_WANT_WRITE)
            rc = 0; /* indicate we haven't finished writing the payload yet */
    }

    return rc;
}

void SSLSocket_handleOpensslInit(int bool_value)
{
}

void SSLSocket_terminate(void)
{
}

int SSLSocket_initialize(void)
{
    return 1;
}

#endif
