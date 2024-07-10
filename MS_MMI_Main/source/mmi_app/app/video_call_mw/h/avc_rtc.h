/*************************************************************
 *
 * This is a part of the Agora Video Call Service SDK.
 * Copyright (C) 2021 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#ifndef __AVC_RTC_H__
#define __AVC_RTC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "avc.h"

avc_pipeline_node_t* avc_rtc_create(avc_config_t *p_config,
                                    avc_msg_notify_cb message_callback);
void                 avc_rtc_destroy(void);

#ifdef __cplusplus
}
#endif
#endif /* __AVC_RTC_H__ */
