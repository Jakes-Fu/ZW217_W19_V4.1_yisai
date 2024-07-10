/*************************************************************
 * File  :  video_call_demo.h
 * Module:  agora_video_call API demo
 *
 * This is a part of the RTSA call Service SDK.
 * Copyright (C) 2020 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#ifndef _VIDEO_CALL_H_
#define _VIDEO_CALL_H_

#ifdef __cplusplus
extern  "C"
{
#endif

#include "agora_video_call.h"
#include "zdt_net.h"

typedef void (*VIDEO_CALL_STATUS_CALLBACK)(avc_msg_t *);

//int video_call_start();
PUBLIC int video_call_start(char *appid, char *channel_name, VIDEO_CALL_STATUS_CALLBACK status_callback);
PUBLIC int video_call_stop(void);
PUBLIC BOOLEAN  Video_Call_Device_Idle_Check(void);

#ifdef __cplusplus
}
#endif
#endif  /* _VIDEO_CALL_DEMO_H_ */