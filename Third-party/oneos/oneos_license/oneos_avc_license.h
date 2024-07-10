/*************************************************************
 * File  :  avc_license.c
 * Module: RTSA call C application.
 *
 * This is a part of the RTSA call Service SDK.
 * Copyright (C) 2020 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#ifndef _ONEOS_AVC_LICENSE_H_
#define _ONEOS_AVC_LICENSE_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern  "C"
{
#endif
typedef enum {
    AVC_MSG_JOIN_CHANNEL_SUCCESS = 0x1000,
    AVC_MSG_USER_JOINED,
    AVC_MSG_USER_OFFLINE,
    AVC_MSG_CONNECTION_LOST,
    AVC_MSG_INVALID_APP_ID,
    AVC_MSG_INVALID_CHANNEL_NAME,
    AVC_MSG_INVALID_TOKEN,
    AVC_MSG_TOKEN_EXPIRED,
    AVC_MSG_FIRST_VIDEO_FRAME_RECV,

    AVC_MSG_LICENSE_PARAM_INVALID,
    AVC_MSG_LICENSE_DNS_PARSE_FAILED,
    AVC_MSG_LICENSE_NET_FAILED,
    AVC_MSG_LICENSE_ACTIVATE_FAILED,

    AVC_MSG_KEY_FRAME_REQUEST,
    AVC_MSG_BWE_TARGET_BITRATE_UPDATE,
} avc_msg_e;
int oneos_avc_license_activate(const char *str_appid,
                         const char *str_customer_key,
                         const char *str_customer_secret,
                         const char *str_license_key,
                         char *str_certificate,
                         uint32_t *str_certificate_len,
                         uint32_t net_id);

#ifdef __cplusplus
}
#endif
#endif  /* _ONEOS_AVC_LICENSE_H_ */