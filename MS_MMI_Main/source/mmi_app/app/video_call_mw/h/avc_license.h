/*************************************************************
 * File  :  avc_license.h
 * Module: RTSA call C application.
 *
 * This is a part of the RTSA call Service SDK.
 * Copyright (C) 2020 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#ifndef __AVC_LICENSE_H__
#define __AVC_LICENSE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "avc.h"

#define DEFAULT_CREDENTIAL  "4919e6ecb4463a23b4ef757891f9a72651d53cb48b6961daf161ec6930cb3a77"

/**
 * license activate.
 * @param[in]     str_appid
 * @param[in]     str_customer_key
 * @param[in]     str_customer_secret
 * @param[in]     str_license_key
 *
 * @param[in out]    str_certificate
 * @param[in out]    str_certificate_len
 * @param[in]        net_id
 *
 * @return
 * - = 0: Success
 * - < 0: Failure
 */
int avc_license_activate(const char *str_appid,
                         const char *str_customer_key,
                         const char *str_customer_secret,
                         const char *str_license_key,
                         char *str_certificate,
                         uint32_t *str_certificate_len,
                         uint32_t net_id);

#ifdef __cplusplus
}
#endif
#endif /* __AVC_LICENSE_H__ */

