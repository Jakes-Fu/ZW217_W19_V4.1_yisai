/*************************************************************
 * File  :  avc_license.c
 * Module: RTSA call C application.
 *
 * This is a part of the RTSA call Service SDK.
 * Copyright (C) 2020 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#include "agora_video_call.h"
#include "agora_rtc_api.h"
#include "avc_license.h"
#include "socket_api.h"

#define TAG                 "[license]"

#define MAX_CONN_TIMES      (50)  //最大轮询次数
#define CONN_RETRY_TIMEGAP  (100) //CONNECT 轮询间隔
#define HTTP_MAX_BUF_LEN    (4096)
#define MAX_BUF_LEN         (1024)


static int32_t __base64_encode(const char *src, int32_t src_len, char *encoded)
{
    const char basis_64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int i;
    char *p = encoded;
    for (i = 0; i < src_len - 2; i += 3) {
        *p++ = basis_64[(src[i] >> 2) & 0x3F];
        *p++ = basis_64[((src[i] & 0x3) << 4) |
                        ((src[i + 1] & 0xF0) >> 4)];
        *p++ = basis_64[((src[i + 1] & 0xF) << 2) |
                        ((src[i + 2] & 0xC0) >> 6)];
        *p++ = basis_64[src[i + 2] & 0x3F];
    }
    if (i < src_len) {
        *p++ = basis_64[(src[i] >> 2) & 0x3F];
        if (i == (src_len - 1)) {
            *p++ = basis_64[((src[i] & 0x3) << 4)];
            *p++ = '=';
        } else {
            *p++ = basis_64[((src[i] & 0x3) << 4) |
                            ((src[i + 1] & 0xF0) >> 4)];
            *p++ = basis_64[((src[i + 1] & 0xF) << 2)];
        }
        *p++ = '=';
    }

    *p++ = '\0';
    return (p - encoded);
}

extern struct sci_hostent* avc_dns_parse(char *name_ptr, uint32_t netid);
extern void avc_dns_parse_free_hostent(struct sci_hostent* hostent);
int avc_license_activate(const char *str_appid,
                         const char *str_customer_key,
                         const char *str_customer_secret,
                         const char *str_license_key,
                         char *str_certificate,
                         uint32_t *str_certificate_len,
                         uint32_t net_id)
{
    struct sci_hostent *hostent;
    uint32_t ip;
    TCPIP_SOCKET_T sock;
    char *str_authorization = NULL;
    char *str_authorization_base64 = NULL;
    int32_t str_authorization_base64_len = 0;
    char *http_buffer = NULL;
    int32_t http_buffer_len;
    char *str_page = NULL;
    char *str_post = NULL;
    int32_t str_post_len;
    char str_hostname[] = "api.agora.io";
    struct sci_sockaddr server_addr = {0};
    int i;
    int rval;
    char *cert;
    int idx = 0;
    const char *str_credential = DEFAULT_CREDENTIAL;

    if (NULL == str_appid ||
        NULL == str_customer_key ||
        NULL == str_customer_secret ||
        NULL == str_license_key ||
        NULL == str_certificate ||
        NULL == str_certificate_len) {
        LOGE(TAG, "param invalid.");
        return -AVC_MSG_LICENSE_PARAM_INVALID;
    }

    hostent = avc_dns_parse(str_hostname, net_id);
    if (NULL == hostent) {
        LOGE(TAG, "dns parse failed.");
        return -AVC_MSG_LICENSE_DNS_PARSE_FAILED;
    }

    ip = (uint32_t)hostent->h_addr_list[0][3] +
         ((uint32_t)hostent->h_addr_list[0][2] << 8) +
         ((uint32_t)hostent->h_addr_list[0][1] << 16) +
         ((uint32_t)hostent->h_addr_list[0][0] << 24);
    LOGE(TAG, "ip=%0x", ip);

    avc_dns_parse_free_hostent(hostent);

    sock = sci_sock_socket(AF_INET, SOCK_STREAM, 0, net_id);
    if (TCPIP_SOCKET_INVALID == sock) {
        LOGE(TAG, "socket create failed.");
        return -AVC_MSG_LICENSE_NET_FAILED;
    }

    str_authorization        = (char *)malloc(MAX_BUF_LEN);
    str_authorization_base64 = (char *)malloc(MAX_BUF_LEN);
    http_buffer              = (char *)malloc(HTTP_MAX_BUF_LEN);
    str_page                 = (char *)malloc(MAX_BUF_LEN);
    str_post                 = (char *)malloc(MAX_BUF_LEN);

    assert(str_authorization && str_authorization_base64 && http_buffer && str_page && str_post);

    snprintf(str_page, MAX_BUF_LEN, "/dev/v2/apps/%s/licenses", str_appid);
    str_post_len = snprintf(str_post, MAX_BUF_LEN, "{ \"credential\": \"%s\", \"custom\": \"%s\" }", str_credential, str_license_key);
    snprintf(str_authorization, MAX_BUF_LEN, "%s:%s", str_customer_key, str_customer_secret);
    __base64_encode(str_authorization, strlen(str_authorization), str_authorization_base64);

    LOGE(TAG, "post: %s", str_post);

    http_buffer_len = snprintf(http_buffer, HTTP_MAX_BUF_LEN,
                               "POST %s HTTP/1.1\r\n"
                               "Authorization: Basic %s\r\n"
                               "Host: %s\r\n"
                               "Content-type: application/json\r\n"
                               "Accept: */*\r\n"
                               "Content-length: %d\r\n\r\n"
                               "%s",
                               str_page, str_authorization_base64, str_hostname, str_post_len, str_post);

    server_addr.family  = AF_INET;
    server_addr.ip_addr = htonl(ip);
    server_addr.port    = htons(80);

    rval = sci_sock_setsockopt(sock, SO_NBIO, NULL);
    if (TCPIP_SOCKET_ERROR == rval) {
        LOGE(TAG, "setsockopt failed.");
        rval = -AVC_MSG_LICENSE_NET_FAILED;
        goto L_END;
    }

    for (i = 0; i < MAX_CONN_TIMES; i++) {
        sci_sock_connect(sock, &server_addr, sizeof(server_addr));
        rval = sci_sock_errno(sock);
        LOGE(TAG, "connect err=%d", rval);
        if (EINPROGRESS != rval) break;
        LOGE(TAG, "loop connect[%d]", i);
        SCI_Sleep(CONN_RETRY_TIMEGAP);
    }

    rval = sci_sock_connect(sock, &server_addr, sizeof(server_addr));
    if (TCPIP_SOCKET_ERROR == rval) {
        if (EINPROGRESS != (rval = sci_sock_errno(sock))) {
            LOGE(TAG, "connect failed err=%d.", rval);
            rval = -AVC_MSG_LICENSE_NET_FAILED;
            goto L_END;
        }
    }

    rval = sci_sock_send(sock, http_buffer, http_buffer_len, 0);
    if (rval <= 0) {
        LOGE(TAG, "sock send failed.");
        rval = -AVC_MSG_LICENSE_NET_FAILED;
        goto L_END;
    }

    rval = -AVC_MSG_LICENSE_NET_FAILED;
    for (i = 0; i < MAX_CONN_TIMES; i++) {
        rval = sci_sock_recv(sock, http_buffer, HTTP_MAX_BUF_LEN, 0);
        if (TCPIP_SOCKET_ERROR == rval || 0 == rval) {
            if (EWOULDBLOCK != sci_sock_errno(sock)) {
                break;
            }

            SCI_Sleep(CONN_RETRY_TIMEGAP);
            LOGE(TAG, "recv loop");
            continue;
        }

        http_buffer[rval] = '\0';
        LOGE(TAG, "receive packet, len=%d. %s", rval, http_buffer);

        cert = strstr(http_buffer, "{\"cert\":\"");
        if (NULL == cert) {
            LOGE(TAG, "cert not found");
            break;
        }

        cert += 9;
        while ((str_certificate[idx++] = *cert++) != '\"');
        *str_certificate_len = idx - 1;
        str_certificate[idx - 1] = 0;

        LOGE(TAG, "cert[%d][%c]:%s", *str_certificate_len, *str_certificate, str_certificate);
        LOGE(TAG, "cert len=%d, [%c,%c]", strlen(str_certificate), str_certificate[630], str_certificate[631]);
        rval = 0;
        break;
    }

L_END:
    sci_sock_socketclose(sock);

    free(str_authorization);
    free(str_authorization_base64);
    free(http_buffer);
    free(str_page);
    free(str_post);

    return rval;
}
