#ifdef SXH_APPLE_SUPPORT
#ifndef __APPLE_EXPORT_H__
#define __APPLE_EXPORT_H__

#include "apple_id.h"
#include "apple_smd_um.h"
#include "dal_player.h"
#define APPLE_BASE2_MSG_ID (MSG_USER_START+2)


extern MMI_WIN_ID_T SXM_WIN_ID;


extern void AppleEnterIdleScrNotify_SPT(void);
extern void AppleExitIdleScrNotify_SPT(void);
extern void APPLE_StartSMDApp(uint32 appId);
extern void APPLE_StartUMvchat(void);
extern void APPLE_StartApple(void);

void Apple_RegWinIdNameArr(void);
extern 	void SMDMenuTv1(void);
extern  void SMDMenuTv2(void);
extern  void SMDMenuTv3(void);


extern int OEMDPLAYER_GetCurrentFrameData(DPLAYER_HANDLE     player_handle,  
    DPLAYER_IMAGE_DATA_T    * frame_data_ptr);

void AppleIDLEMsgInd(void *data);

uint32 Apple_Get_ps_mode();
void APPLE_SRVMGR_Init();
int APPLE_GetIsUseWifi(void);

void Apple_SetIsUseWifi(int bUseWifi);


#endif
#endif
