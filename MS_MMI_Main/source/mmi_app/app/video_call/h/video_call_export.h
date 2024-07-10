/*************************************************************
 * File  :  video_call_demo.h
 * Module:  agora_video_call API demo
 *
 * This is a part of the RTSA call Service SDK.
 * Copyright (C) 2020 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#ifndef VIDEO_CALL_EXPORT_H_
#define VIDEO_CALL_EXPORT_H_

#ifdef __cplusplus
extern  "C"
{
#endif

PUBLIC void MMIVideo_Call_MainWin();
PUBLIC BOOLEAN MMIVIDEO_IsOpenVideoWin(void);
PUBLIC void MMIVIDEO_Incoming(char *appid, char *channel_name);

#ifdef __cplusplus
}
#endif
#endif  /* _VIDEO_CALL_DEMO_H_ */