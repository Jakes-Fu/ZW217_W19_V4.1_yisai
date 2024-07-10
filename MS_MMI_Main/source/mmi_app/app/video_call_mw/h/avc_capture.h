/*************************************************************
 *
 * This is a part of the Agora Video Call Service SDK.
 * Copyright (C) 2021 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#ifndef __AVC_CAPTURE_H__
#define __AVC_CAPTURE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "avc.h"

avc_pipeline_node_t* avc_capture_create(avc_config_t *p_config, avc_msg_notify_cb message_callback);
void                 avc_capture_destroy(void);

#ifdef __cplusplus
}
#endif
#endif /* __AVC_CAPTURE_H__ */
