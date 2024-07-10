
#ifndef _MET_MEX_EXPORT_H_
#define _MET_MEX_EXPORT_H_


#ifdef MET_MEX_SUPPORT
#include "mexlib.h"


#ifdef __cplusplus
extern   "C"
{
#endif

#define MEX_HOOK_MSG_FROM_MMITASK        0
#define MEX_HOOK_MSG_FROM_SMSCORE        1
#define MEX_HOOK_MSG_FROM_ALARM          2

#define MEX_MSG_ID_WILL_PLAY_ALARMTONE   (0xFFFF - 1)
#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#define MEX_MSG_ID_CC_CALL_RING_IND      (0xFFFF - 2)
#define MEX_MSG_ID_SMS_DELIVER_MSG_IND   (0xFFFF - 3)
#endif

typedef struct _STB_Icon_Status_T
{
    int32 icon_id;
    int32 image_id;
    int32 is_show;
}STB_ICON_STATUS_T;

typedef enum _STB_KEY_E
{
    STB_ICON,
		STB_IMAGE,
		STB_ISSHOW,
		STB_MAX
}STB_KEY_E;


void mex_log( int32 iFilter, const char *pText, ... );
int MexPhs_HookResult(uint16 msgId, void* MsgStruct, int mod_src, void* peerBuff, int msg_from);
BOOLEAN MetMex_ConflictPromt(void);
void MexPhs_OutCallConnectedCB(void* info);        

BOOLEAN MexPhs_IsOutCalling(void);
void MexPhs_OutCallEventCB(void* info);     
void MexPhs_NetworkCallDropped(void *info);

PUBLIC void MMIMEX_PhsOutCallDroped(void *info);

uint32 MexPhs_GetSaveSmsBox(void);
void MexPhs_DelSmsRsp(void *dummy, int32 mod, int16 result);
void MexPhs_SaveSmsCB(void *data, int32 mod, int16 result);

void MexPhs_OutCallEndedCB(void *info);

void MexPhs_ReadSmsContentCB(void *data, int32 mod, int16 result);

BOOLEAN MetMex_ShowPlatformEngineerMode(void);

void MetMex_SpreadStartupInit(void);

bool MexSystem_IsSysKernalRunning(void);

bool MexSystem_IsSysRunning(void);

void MetMex_SmsReceivePromt(void);
uint32 MexPhs_GetSaveSmsBox(void);
bool MexPhs_IsSmsSend(void);
void MexPhs_SmsSendRsp(void* number, int32 mod, uint16 uResult);

void mex_voice_record_stop(void);
PUBLIC void MexMix_IsEarphonePlugIn(bool bIsPlugIn);

#ifdef MET_MEX_MERIG_SUPPORT
void MetMexOpenWin(void);
#endif // MET_MEX_MERIG_SUPPORT
#ifdef MET_MEX_QQ_SUPPORT
void MetMexQQOpenWin(void);
#endif // MET_MEX_QQ_SUPPORT
#ifdef MET_MEX_ZT_SUPPORT
void MetMexZTOpenWin(void);
#endif // MET_MEX_ZT_SUPPORT
#ifdef MET_MEX_FRUITNINJA_SUPPORT
void MetMexFruitNinjaOpenWin(void);
#endif // MET_MEX_FRUITNINJA_SUPPORT
#ifdef MET_MEX_WEIBO_SUPPORT
void MetMexWeiboOpenWin(void);
#endif // MET_MEX_WEIBO_SUPPORT
#ifdef MET_MEX_WEATHER_SUPPORT
void MetMexWeatherOpenWin(void);
#endif  // MET_MEX_WEATHER_SUPPORT
#ifdef MET_MEX_WOMIVIDEO_SUPPORT
void MetMexWomiVideoOpenWin(void);
#endif  //MET_MEX_WOMIVIDEO_SUPPORT
#ifdef MET_MEX_TB_SUPPORT
void MetMexTBOpenWin(void);
#endif // MET_MEX_TB_SUPPORT
#ifdef MET_MEX_AQ_SUPPORT
void MetMexAQOpenWin(void);
#endif // MET_MEX_AQ_SUPPORT
#ifdef MET_MEX_DOUBLE_QQ_SUPPORT
void MetMexQQ1OpenWin(void);
void MetMexQQ2OpenWin(void);
#endif // MET_MEX_DOUBLE_QQ_SUPPORT
#ifdef MET_MEX_MSN_SUPPORT
void MetMexMSNOpenWin(void);
#endif 
#ifdef MET_MEX_FETION_SUPPORT
void MetMexFETIONOpenWin(void);
#endif
#ifdef MET_MEX_AB_SUPPORT
void MetMexABOpenWin(void);
#endif // MET_MEX_AQ_SUPPORT
#ifdef MET_MEX_CQZ_SUPPORT
void MetMexCQZOpenWin(void);
#endif
#ifdef MET_MEX_JSCB_SUPPORT
void MetMexJSCBOpenWin(void);
#endif
#ifdef MET_MEX_TOMCAT_SUPPORT
void MetMexTomCatOpenWin(void);
#endif // MET_MEX_AQ_SUPPORT

int32 MexInterface_CheckSetDivertCall(int32 is_true);
int32 MexInterface_CheckCancelDivertCall(int32 is_true);
int32 MexInterface_CheckQueryDivertCall(int32 is_true,int32 status);

void MexInterface_SetStbIcon(int32 icon_id,STB_KEY_E key,int32 data);
int32 MexInterface_GetStbIcon(int32 icon_id,STB_KEY_E key);


void MetMex_DispatchSignal(MmiTimerExpiryS* signal_ptr);
PUBLIC int MMIMex_Phs_HookResult(uint16 msgId, void* MsgStruct, int mod_src, void* peerBuff, int msg_from);
PUBLIC uint32 MMIMEX_GetSaveSmsBox(void);


// 封装函数
PUBLIC void MMIMEX_RecordStop(void);
PUBLIC BOOLEAN MMIMEX_ConflictPromt(void);
PUBLIC BOOLEAN MMIMEX_IsOutCalling(void);

PUBLIC BOOLEAN MMIMEX_IsSysRunning(void);

PUBLIC BOOLEAN MMIMEX_IsSysKernalRunning(void);

PUBLIC BOOLEAN MMIMEX_ShowPlatformEngineerMode(void);

PUBLIC void MMIMEX_PhsOutCallConnectedCB(void *info);
PUBLIC void MMIMEX_PhsOutCallEventCB(void *info);

PUBLIC void MMIMEX_SmsReceivePromt(void);

PUBLIC void MMIMEX_OutCallEndedCB(void *info);

PUBLIC BOOLEAN MMIMEX_DelSmsRsp(int32 mod, int16 result);


PUBLIC void MMIMEX_ReadSmsContentCB(void *data, int32 mod, int16 result);

PUBLIC void MMIMEX_SaveSmsCB(void *data, int32 mod, int16 result);

PUBLIC void MMIMEX_PhsSetSmsOperStatus(void);

PUBLIC uint32 MexPhs_GetSaveSmsBox(void);

PUBLIC void MMIMEX_IsEarphonePlugIn(bool bIsPlugIn);

PUBLIC BOOLEAN MMIMEX_SendSmsCB(uint16 fail_count);


///////////////////////////////////////////////////
PUBLIC void Mex_AudPly_play_handle(void);
/*停止按键*/
PUBLIC void Mex_AudPly_stop_handle(void);
/*下一曲按键*/
PUBLIC void Mex_AudPly_next_handle(void);
/*上一曲按键*/
PUBLIC void Mex_AudPly_prev_handle(void);
/*增大音量按键*/
PUBLIC void Mex_AudPly_inc_volume_handle(void);
/*减小音量按键*/
PUBLIC void Mex_AudPly_dec_volume_handle(void);
/*当前是否处于停止播放状态*/
PUBLIC BOOLEAN Mex_Func_get_audply_is_stoped(void);

uint32* MetMex_GetVBDecodeBuf(void);

uint32 MetMex_GetVBDecodeBufSize(void);



PUBLIC int MexPhs_Alarm_HookResult(void);




PUBLIC uint32 MEX_CHR_CheckBatteryStatus (void);

///////////////////////////////////////////////////
#ifdef   __cplusplus
}
#endif

#endif   /* MET_MEX_SUPPORT */


#endif  /* _MET_MEX_EXPORT_H_ */
