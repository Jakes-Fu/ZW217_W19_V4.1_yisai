/*************************************************************
 *
 * This is a part of the Agora Video Call Service SDK.
 * Copyright (C) 2021 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#ifndef __AVC_DECODE_H__
#define __AVC_DECODE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "avc.h"

avc_pipeline_node_t* avc_decode_create(avc_msg_notify_cb message_callback);
void                 avc_decode_destroy(void);

#ifdef __cplusplus
}
#endif
#endif /* __AVC_DECODE_H__ */
