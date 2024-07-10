/*************************************************************
 * File  :  avc_license.c
 * Module: RTSA call C application.
 *
 * This is a part of the RTSA call Service SDK.
 * Copyright (C) 2020 Agora IO
 * All rights reserved.
 *
 *************************************************************/

#include "os_util.h"
#include "cms_lic_interface.h"
#include "oneos_avc_license.h"

#define LOGI_AL(TAG, fmt, ...) os_kprintf("I/ %s" fmt " at %s@Line%u\n", TAG, ##__VA_ARGS__, __PRETTY_FUNCTION__, __LINE__)
#define LOGD_AL(TAG, fmt, ...) os_kprintf("D/ %s" fmt " at %s@Line%u\n", TAG, ##__VA_ARGS__, __PRETTY_FUNCTION__, __LINE__)
#define LOGE_AL(TAG, fmt, ...) os_kprintf("E/ %s" fmt " at %s@Line%u\n", TAG, ##__VA_ARGS__, __PRETTY_FUNCTION__, __LINE__)
#define LOGW_AL(TAG, fmt, ...) os_kprintf("W/ %s" fmt " at %s@Line%u\n", TAG, ##__VA_ARGS__, __PRETTY_FUNCTION__, __LINE__)

#define TAG "[license]"
static int cms_rtc_scode = 1003;

static int32_t __base64_encode(const char *src, int32_t src_len, char *encoded)
{
    const char basis_64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    int i;
    char *p = encoded;
    for (i = 0; i < src_len - 2; i += 3)
    {
        *p++ = basis_64[(src[i] >> 2) & 0x3F];
        *p++ = basis_64[((src[i] & 0x3) << 4) |
                        ((src[i + 1] & 0xF0) >> 4)];
        *p++ = basis_64[((src[i + 1] & 0xF) << 2) |
                        ((src[i + 2] & 0xC0) >> 6)];
        *p++ = basis_64[src[i + 2] & 0x3F];
    }
    if (i < src_len)
    {
        *p++ = basis_64[(src[i] >> 2) & 0x3F];
        if (i == (src_len - 1))
        {
            *p++ = basis_64[((src[i] & 0x3) << 4)];
            *p++ = '=';
        }
        else
        {
            *p++ = basis_64[((src[i] & 0x3) << 4) |
                            ((src[i + 1] & 0xF0) >> 4)];
            *p++ = basis_64[((src[i + 1] & 0xF) << 2)];
        }
        *p++ = '=';
    }

    *p++ = '\0';
    return (p - encoded);
}

#define MAX_BUF_LEN (1024)
int oneos_avc_license_activate(const char *str_appid,
                               const char *str_customer_key,
                               const char *str_customer_secret,
                               const char *str_license_key,
                               char *str_certificate,
                               uint32_t *str_certificate_len,
                               uint32_t net_id)
{
    int ret;
    uint16_t lic_size = 0;
    uint8_t *recv_buf = NULL;

    LOGI_AL(TAG, "call into oneos_avc_license_activate...");

    if (NULL == str_certificate ||
        NULL == str_certificate_len)
    {
        LOGE_AL(TAG, "param invalid.");
        return -AVC_MSG_LICENSE_PARAM_INVALID;
    }

    recv_buf = (uint8_t *)malloc(MAX_BUF_LEN);
    if (NULL == recv_buf)
    {
        LOGE_AL(TAG, "malloc error");
        return -AVC_MSG_LICENSE_NET_FAILED;
    }

    memset(recv_buf, 0x0, MAX_BUF_LEN);
    ret = cms_lic_get_external_lic(cms_rtc_scode, recv_buf, MAX_BUF_LEN, &lic_size);
    if (ret)
    {
        LOGE_AL(TAG, "cert not found!");
        ret = -AVC_MSG_LICENSE_NET_FAILED;
        goto L_END;
    }

    LOGI_AL(TAG, "lic_size[%d]", lic_size);
    __base64_encode(recv_buf, lic_size, str_certificate);

    *str_certificate_len = strlen(str_certificate);
    // str_certificate[lic_size - 1] = '\0';
    LOGI_AL(TAG, "cert_len[%d]:cert[%s]", *str_certificate_len, str_certificate);
L_END:
    free(recv_buf);
    return ret;
}
