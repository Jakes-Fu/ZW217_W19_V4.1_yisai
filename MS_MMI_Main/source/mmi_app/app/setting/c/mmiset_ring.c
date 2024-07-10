/****************************************************************************
** File Name:      mmiset_ring.c                                             *
** Author:                                                                   *
** Date:           25/05/2004                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2004       Jassmine.Meng       Create
******************************************************************************/

#define _MMISET_RING_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "tb_dal.h"
#include "mmiset_text.h"
#include "guilistbox.h"
#include "mmicc_export.h"
#include "mmiidle_export.h"
#include "mmk_timer.h"
#include "mmialarm_export.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "mmi_textfun.h"
#include "mmiudisk_export.h"
#include "mmisrvaud_api.h"
//#include "mmiset.h"
#include "mmiset_internal.h"
#include "mmitv_out.h"
#ifndef WIN32
#include "gpio_prod_api.h"
#endif
#include "mmipub.h"
#include "mmiset_nv.h"
#include "mmiset_set_ring.h"
#include "mmiset_image.h"
//#include "mmiset_call.h"
#include "mmiset_call_internal.h"
#include "mmi_nv.h"
#include "mmienvset_export.h"
#include "mmi_ring.h"
#include "mmiphone_export.h"
#include "mmifm_export.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "guires.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
//#include "mmieng.h"
#include "mmieng_export.h"
//#include "mmienvset.h"
#include "mmi_resource.h"
#if defined(TTS_SUPPORT) || defined(HERO_ENGINE_TTS_SUPPORT )
#include "mmitts_export.h"
#endif
#if defined(MMI_PDA_SUPPORT)
#include "mmicc_id.h"
#endif
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#ifdef WRE_SUPPORT
#include "mmiwre_export.h"
#endif
#include "mmiidle_statusbar.h"

#ifdef MCARE_V31_SUPPORT
#include "McfPb.h"
#include "Mcare_Interface.h"
#endif

#ifdef MMI_ETWS_SUPPORT
#include "mmisms_export.h"
#include "gpio_prod_api.h"
#endif
#include "mmicc_internal.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/    
#define MMISET_VIBRA_DEFAULT_TIME            1000
#define MMISET_DEFAULT_SMS_NUM               3
#define MMISET_CONTROL_RING_NUM              MMISET_RING_TYPE_FM_PLAYER
#define MMISET_VOLUME_IMG_DISPLAY_MARGEN     8

#define MMISET_RING_TIME_MSG    60000

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL MMISET_RING_CALLBACK_PFUNC    s_ring_callback_func = PNULL;  //铃声播放完毕后的回调函数

#ifdef MMISRV_AUDIO_SUPPORT
LOCAL MMISRV_HANDLE_T s_cur_app_handle = 0;  //当前播放铃声的类型
LOCAL MMISRV_HANDLE_T s_vibrate_handle = 0;
#else
LOCAL MMIAUDIO_HTYPE            s_cur_audio_htype = MMIAUDIO_NONE;  //当前播放铃声的类型
#endif

LOCAL BOOLEAN                   s_is_need_earfree_mode = FALSE;//是否需要耳机下也支持speaker播放

//LOCAL BOOLEAN               s_set_is_stop_mp3 = FALSE;                          //是否停止预览或者来电的mp3铃声
LOCAL uint8   s_mmiset_vibra_timer_id = 0;    //设置中振动器振动的时间id

LOCAL BOOLEAN               s_is_from_idle = FALSE;                 //是否是在idle情况下改变铃声提示方式
LOCAL BOOLEAN                     s_is_vibrate = FALSE;            //是否震动
LOCAL MMISET_ALL_RING_TYPE_E      s_is_playing_vibrate_type = MMISET_RING_TYPE_NONE;  //正在振动的类型
LOCAL MMISET_ALL_RING_TYPE_E      s_cur_real_ring_type = MMISET_RING_TYPE_NONE;       //正在播放铃声的真实类型
LOCAL uint8                       s_last_vol = MMISET_VOL_ONE;              //用于静音与非静音间切换

#ifdef MMI_ETWS_SUPPORT
typedef enum MMISET_SOS_VIBRATE_STATE_
{
    FIRST_VIBRATE_2SECOND_START_1,
    FIRST_PAUSE_VIBRATE_2,
    SECOND_VIBRATE_1SECOND_START_3,
    SECOND_PAUSE_VIBRATE_4,
    THIRD_VIBRATE_1SECOND_START_5,
    THIRD_PAUSE_VIBRATE_6,
    SOS_VIBRATE_STATE_MAX
} MMISET_SOS_VIBRATE_STATE_E;

LOCAL uint8 s_sos_vibrator_time_id = 0;
LOCAL uint8 s_sos_vibrator_state = 0;
LOCAL uint8 s_sos_torch_time_id = 0;
LOCAL BOOLEAN s_is_sos_vibrator = FALSE;
LOCAL BOOLEAN s_is_mp3_playing = FALSE;
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
MMISET_ALL_RING_TYPE_E      g_is_playing_ring_type = MMISET_RING_TYPE_MAX;     //正在播放铃声的类型


/*****************************************************************************/
//  Description : Get the handle by name.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
extern MMISRV_HANDLE_T MMISRVAUD_GetHandleByName(char *client_name);

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : if permit play ring in call
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsPermitPlayRingInCall(MMISET_ALL_RING_TYPE_E ring_type);

/*****************************************************************************/
//  Description : play call ring by ring group id
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void PlayRing(
                    MN_DUAL_SYS_E               dual_sys,   //sim id
                    uint16                       ring_id,    // 固定铃声的index,如果是更多音乐，ID指定为MMISET_MAX_RING_ID
                    uint8                       ring_vol,   // the ring volume 
                    uint32                      play_times,
                    MMISET_ALL_RING_TYPE_E      ring_type,  //in: the type of all ring
                    MMISET_RING_CALLBACK_PFUNC  callback_func,
                    uint8                       mode_id
                    );

#ifdef MMISRV_AUDIO_SUPPORT
/*****************************************************************************/
//  Description : after play ring ,callback function,at first free ring_data.data_ptr
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void CallBackFunction(MMISRVAUD_REPORT_RESULT_E result, DPARAM param);

#else
/*****************************************************************************/
//  Description : after play ring ,callback function,at first free ring_data.data_ptr
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void CallBackFunction(MMIAUDIO_RESULT result, DPARAM param);
#endif

/*****************************************************************************/
//  Description : play message ring with ring volume
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
//LOCAL void PlayMsgRingByVol(
//                            uint8                      ring_vol,
//                            uint32                     play_times, //the times of play ring
//                            MMISET_ALL_RING_TYPE_E     ring_type,   //in: the type of all ring
//                            MMISET_RING_CALLBACK_PFUNC callback_func
//                            );

/*****************************************************************************/
//  Description : play fixed ring by id
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL void PlayFixedRing(
                         uint16                       ring_id,    // 固定铃声的index,
                         MMISET_ALL_RING_TYPE_E      ring_type,  //in: the type of all ring
                         uint32                      play_times,
                         uint8                       ring_vol,   // the ring volume 
                         MMISET_RING_CALLBACK_PFUNC  callback_func
                         );

/*****************************************************************************/
//  Description : play key tone with ring volume
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
//LOCAL void PlayKeyToneByVol(
//                            uint8                      ring_vol,
//                            uint32                     play_times, //the times of play ring
//                            MMISET_ALL_RING_TYPE_E     ring_type,   //in: the type of all ring
//                            MMISET_RING_CALLBACK_PFUNC callback_func
//                            );

/*****************************************************************************/
//  Description : play power on or off ring with ring volume
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
//LOCAL void PlayPowerRingByVol(
//                              uint8                      ring_vol,
//                              uint32                     play_times, //the times of play ring
//                              MMISET_ALL_RING_TYPE_E     ring_type,   //in: the type of all ring
//                              MMISET_RING_CALLBACK_PFUNC callback_func
//                               );

/*****************************************************************************/
//  Description : set the type of message ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
//LOCAL void SetMsgRingType(
//                          MMISET_MSG_RING_TYPE_E  msg_ring_type//in
//                           );

/*****************************************************************************/
//  Description : when permit paly dsp tone,play generic tone
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void PlayGenericTone(
                           AUD_GENERIC_TONE_ID_E    tone_id,
                           uint32   duration
                           );
/*****************************************************************************/
//  Description : set fixed message ring info
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
//LOCAL  void SetFixedMsgRingInfo(
//                                MN_DUAL_SYS_E   dual_sys,
//                                uint8   msg_ring_id //the id of message ring
//                                 );
/*****************************************************************************/
//  Description : set the ring of key
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
//LOCAL void SetKeyToneRing(
//                          MMISET_OTHER_RING_TYPE_E      key_ring//in
//                           );

/*****************************************************************************/
//  Description : set the ring of flip ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
//LOCAL void SetFlipRing(
//                       MMISET_OTHER_RING_TYPE_E     flip_ring//in
//                        );

/*****************************************************************************/
//  Description : stop generic tone
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void StopGenericTone(void);

/*****************************************************************************/
//  Description : to STOP the vibrator
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
//LOCAL void RingCallBack(
//                        MMIAUDIO_RESULT result,
//                        DPARAM param
//                        );

/*****************************************************************************/
//  Description : get the info about fixed ring(call,msg,key,power,flip,alarm,
//                  battery,dc,delete warning)of the resource
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void GetRingInfo(
                       uint16                    ring_id,           //in: the ring id
                       MMISET_ALL_RING_TYPE_E    ring_type,         //in: the type of all ring
                       MMIAUD_RING_DATA_INFO_T   *ring_info_ptr  //out: the info of all ring
                       );

/*****************************************************************************/
//  Description : get if permit minute hint ring in call
//  Global resource dependence : none
//  Author: bin.ji
//  Note:2004.12.21
/*****************************************************************************/
LOCAL BOOLEAN GetPermitMinhintRingIncall(void);

/*****************************************************************************/
//  Description : get if permit minute hint ring in call
//  Global resource dependence : none
//  Author: bin.ji
//  Note:2004.12.21
/*****************************************************************************/
LOCAL BOOLEAN GetPermitCallRingIncall(void);

/*****************************************************************************/
//  Description : get if permit minute hint ring in call
//  Global resource dependence : none
//  Author: bin.ji
//  Note:2004.12.21
/*****************************************************************************/
LOCAL BOOLEAN GetPermitMsgRingIncall(void);

/*****************************************************************************/
//  Description : get if permit paly dsp tone,play generic tone
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDspPlayRing(void);

/*****************************************************************************/
//Description : refurbish idle icon display
//Global resource dependence : none
//Author: maqianlin
//Note:
/*****************************************************************************/
LOCAL void SetIdleCallRingTypeIcon(
                                   MMISET_CALL_RING_TYPE_E  call_ring_type
                                   );

/*****************************************************************************/
//  Description : get is permit vibrate
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 实际上所有的震动都对应相应的铃声类型(ring_type)
/*****************************************************************************/
LOCAL BOOLEAN GetisPertmitVibrate(MMISET_ALL_RING_TYPE_E  ring_type);

/*****************************************************************************/
//  Description : get audio mode is earfree
//  Global resource dependence : none
//  Author: jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetAudioMode(void);

/*****************************************************************************/
//  Description : handle playing through receiver 
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleReceiverMode(
                    MMISET_ALL_RING_TYPE_E  ring_type,
                    uint8                   volume
                    );

#ifdef MMISRV_AUDIO_SUPPORT

/*****************************************************************************/
// 	Description : Audio call back function called by audio service.
//	Global resource dependence : 
//   Author: Yintang.Ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleAudioCallBack(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param);

/*****************************************************************************/
//  Description : Get pri by ring type.
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetRingPri(MMISET_ALL_RING_TYPE_E  ring_type);

#endif

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get multim volume
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetMultimVolume(void)
{
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    uint8               vol = 0;

    MMINV_READ(MMINV_SET_MULTIM_VOLUME,&vol,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        vol = MMISET_VOL_DEFAULT_MULTIM;
        MMINV_WRITE(MMINV_SET_MULTIM_VOLUME,&vol);
    }
    //SCI_TRACE_LOW:"[MMISET]:MMIAPISET_GetMultimVolume VOL=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_349_112_2_18_2_51_38_270,(uint8*)"d",vol);
    return vol;
}

/*****************************************************************************/
//  Description : set multim volume
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetMultimVolume(uint8 vol)
{
    uint8 volume = MMISET_VOL_ZERO;

    if(MMISET_VOL_ZERO == vol)
    {
        //设置静音时保存上一次的音量值
        volume = MMIAPISET_GetMultimVolume();
        if(MMISET_VOL_ZERO != volume)
        {
            s_last_vol = volume;
        }
    }
    SCI_TRACE_LOW("MMIAPISET_SetMultimVolume,vol = %d",vol);

    if(MMISET_VOL_MAX >= vol)
    {
        MMINV_WRITE(MMINV_SET_MULTIM_VOLUME,&vol);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMISET]:MMIAPISET_SetMultimVolume error!! VOL=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_377_112_2_18_2_51_38_271,(uint8*)"d",vol);
    }
    
}

/*****************************************************************************/
//  Description : 静音与非静音切换,返回切换后的声音
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_MultimMuteSwitch(void)
{
    uint8 vol = MMIAPISET_GetMultimVolume();

    if(MMISET_VOL_ZERO == vol)
    {
        vol = s_last_vol;
        MMIAPISET_SetMultimVolume(vol);
    }
    else
    {
        s_last_vol = vol;
        vol = MMISET_VOL_ZERO;
        MMIAPISET_SetMultimVolume(vol);
    }

    return vol;
}

/*****************************************************************************/
//  Description : get last multim volume(当前音量是0 时才有效)
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetLastMultimVolume(void)
{
    return s_last_vol;
}


/*****************************************************************************/
//  Description : Get play time by ring type.
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL uint16 GetPlayTime(MMISET_ALL_RING_TYPE_E type, wchar *name_ptr, uint32 name_len)
{   
    uint time = 0;
    MMISRVAUD_CONTENT_INFO_T content = {0};
    if(PNULL == name_ptr || 0 == name_len)
    {
        return 0;
    }
    switch(type)
    {
    case MMISET_RING_TYPE_MSG:
        /* if file total time is longer than the max value, return the max value, else return 0 to play once */
        MMISRVAUD_GetFileContentInfo(name_ptr, name_len, &content);
        if(content.total_time > MMISET_RING_TIME_MSG)
        {
            time = MMISET_RING_TIME_MSG;
        }
        break;
    default:
        break;
    }
    return time;
}

/*****************************************************************************/
//  Description : Get play time by ring type.
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_StopMsgRing(void)
{
    if(MMISET_RING_TYPE_MSG == g_is_playing_ring_type)
    {
        MMIAPISET_StopAppRing();
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*****************************************************************************/
//  Description : init ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
void MMISET_RingInit(void)
{
    BOOLEAN                     is_dsp_play_ring = TRUE;
    MN_RETURN_RESULT_E          return_value = MN_RETURN_FAILURE;
    BOOLEAN                     is_connect_prompt = TRUE;
                
        
    //init if permit dsp play ring
    MMINV_READ(MMINV_SET_DSP_PLAY_RING,&is_dsp_play_ring,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_dsp_play_ring = TRUE;
        MMINV_WRITE(MMINV_SET_DSP_PLAY_RING,&is_dsp_play_ring);
    }
    
    //init if connect prompt open
    MMINV_READ(MMINV_SET_CONNECT_PROMPT_STATUS,&is_connect_prompt,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_connect_prompt = TRUE;
        MMINV_WRITE(MMINV_SET_CONNECT_PROMPT_STATUS,&is_connect_prompt);
    }
}
/*****************************************************************************/
//  Description : is phone in vibrate status, used in cc recently
//  Global resource dependence : none
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsVibratorNow(void)
{
    return s_is_vibrate;
}
/*****************************************************************************/
//  Description : set vibrator
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetVibrator(
                                  BOOLEAN                 is_open,
                                  MMISET_ALL_RING_TYPE_E  ring_type//in: the type of all vibrator
                                  )
{
    if (is_open)//打开振动
    {
        if (MMIAPISET_IsPermitPlayRing(ring_type))//是否允许拨铃或者振动
        {
            uint16 pri = GetRingPri(ring_type);
#ifdef VIRTUAL_VIBRATE_FUNC
            s_vibrate_handle = MMISRVAUD_StartVibrate(pri, MMIENVSET_PREVIEW_PLAY_RING_DUATION, 0, 1, PNULL);
#else			
            s_vibrate_handle = MMISRVAUD_StartVibrate(pri, 0xFFFF, 0, 1, PNULL);
#endif
            s_is_vibrate = TRUE;
            s_is_playing_vibrate_type = ring_type; 
        }
    }
    else//关闭振动
    {
        if (s_is_playing_vibrate_type == ring_type)
        {
            if(s_vibrate_handle != 0)
            {
#ifdef VIRTUAL_VIBRATE_FUNC
                MMISRVAUD_StopVibrate();//
#else
                MMISRVAUD_StopVibrate(s_vibrate_handle); //s_vibrate_handle
#endif
                s_vibrate_handle = 0;
            }
            s_is_vibrate = FALSE;
        }
    }
}

/*****************************************************************************/
//  Description : Vibrator call back
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void VibrateCallback(void)
{
#ifndef VIRTUAL_VIBRATE_FUNC
    s_is_playing_vibrate_type = MMISET_RING_TYPE_NONE;
#endif

    MMIAPISET_StopVibrator(MMISET_RING_TYPE_MSG);//maryxiao cr231219
     s_is_vibrate = FALSE;
    MMIENVSET_StopOnetimeVibTimer();
    MMIAPIENVSET_SetVibStatus(FALSE); 
}

/*****************************************************************************/
//  Description : set vibrator periodly
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetVibratorEx(
                                  BOOLEAN                 is_open,
                                  MMISET_ALL_RING_TYPE_E  ring_type,//in: the type of all vibrator
                                  BOOLEAN is_period, //是否间断不停的振动
                                  uint32 duration   //振动间隔
                                  )
{
    if (is_open)//打开振动
    {
        uint16 pri = GetRingPri(ring_type);
        if(is_period)
        {
            s_vibrate_handle = MMISRVAUD_StartVibrate(pri, duration, 1000, 0xFFFF, VibrateCallback);
        }
        else
        {
            s_vibrate_handle = MMISRVAUD_StartVibrate(pri, duration, 0, 1, VibrateCallback);
        }
        s_is_vibrate = is_open;
        s_is_playing_vibrate_type = ring_type; 
    }
    else//关闭振动
    {
        if (s_is_playing_vibrate_type == ring_type)
        {
            if(s_vibrate_handle != 0)
            {
#ifdef VIRTUAL_VIBRATE_FUNC
                MMISRVAUD_StopVibrate();//
#else
                MMISRVAUD_StopVibrate(s_vibrate_handle); // s_vibrate_handle
#endif
                s_vibrate_handle = 0;
            }
            s_is_vibrate = is_open;
            s_is_playing_vibrate_type = MMISET_RING_TYPE_NONE;
        }
    }
    //SCI_TRACE_LOW:"MMIAPISET_SetVibratorEx: is_open = %d, ring_type = %d,is_period = %d, duration = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_523_112_2_18_2_51_39_272,(uint8*)"dddd",is_open,ring_type,is_period,duration);
}

/*****************************************************************************/
//  Description : Stop all vibrator
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_StopAllVibrator(void)
{  
    SCI_TRACE_LOW("MMIAPISET_StopAllVibrator s_vibrate_handle is %d",
        s_vibrate_handle);
    if(s_vibrate_handle != 0)
    {
#ifdef VIRTUAL_VIBRATE_FUNC
        MMISRVAUD_StopVibrate();//
#else
        MMISRVAUD_StopVibrate(s_vibrate_handle); //
#endif
        s_vibrate_handle = 0;
    }
    s_is_vibrate = FALSE;
    s_is_playing_vibrate_type = MMISET_RING_TYPE_NONE;
}

//yangyu add begin
#ifndef TULING_AI_SUPPORT
PUBLIC void AI_API_ForceToStopPlay()
{
    #ifdef WIN32
    int a = 1;
    return;
    #endif
}
#endif
//yangyu end

/*****************************************************************************/
//  Description : play ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
void MMIAPISET_PlayRing(
                        MN_DUAL_SYS_E              dual_sys,
                        BOOLEAN                    is_preview, 
                        uint8                      group_id,   //group_id or alarm_id,来电铃声才关心此参数!
                        uint32                     play_times, //the times of play ring
                        MMISET_ALL_RING_TYPE_E     ring_type,  //in: the type of all ring
                        MMISET_RING_CALLBACK_PFUNC callback_func
                        )
{
    MMISET_ALL_RING_TYPE_E      ring_type_temp  =   MMISET_RING_TYPE_CALL;
    uint8                       ring_id         =   0;
    uint8                       ring_vol        =   0 ;
#ifdef MMI_ETWS_SUPPORT
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
#endif

    if(MMISET_RING_TYPE_KEY  == ring_type ||
       MMISET_RING_TYPE_TKEY == ring_type)
    {
        if(MMIAPISET_IsPlayingRingType(ring_type))
        {
            return;
        }
    }
    
    if (is_preview)//setting中预览来电方式等需要调用此函数
    {
        ring_type_temp = MMISET_RING_TYPE_OTHER;
    }
    else
    {
        ring_type_temp = ring_type;
    }
    
    //播放铃声以及短信息时需要耳机+speaker模式
    if ((MMISET_RING_TYPE_CALL == ring_type) || (MMISET_RING_TYPE_MSG == ring_type) 
		#ifdef MMI_INDIAN_SOS_SUPPORT
		||(MMISET_RING_TYPE_SOS == ring_type)
		#endif
		)
    {
        MMIAPISET_SetAudioMode(TRUE);
    }
    if(MMIAPISET_IsPermitPlayRing(ring_type_temp)) 
    {
        //停掉所有铃声和振动
		MMI_CheckAllocatedMemInfo();  //zhouyanxian bug112641
	
        MMIAPISET_StopAppRing();
		
		MMI_CheckAllocatedMemInfo();  //zhouyanxian bug112641	
	
        MMIAPISET_SetCurRingType(ring_type_temp);
        
        if (MMISET_RING_TYPE_CALL_IN_CALL == ring_type)
        {
            PlayGenericTone(AUD_GENERIC_WARNING_TONE, MMISET_CALL_IN_CALL_PLAY_RING_TIME);
        }
        else if (MMISET_RING_TYPE_MSG_IN_CALL == ring_type)
        {
            PlayGenericTone(AUD_GENERIC_WARNING_TONE, MMISET_DEFAULT_DSP_PLAY_RING_TIME);
        }
        else if(MMISET_RING_TYPE_MINHINT_IN_CALL == ring_type)
        {
            PlayGenericTone(AUD_GENERIC_WARNING_TONE, MMISET_DEFAULT_DSP_PLAY_RING_TIME);
        }
        else if((MMISET_RING_TYPE_BAT == ring_type) && (MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE) || MMIAPICC_IsInState(CC_MO_CONNECTING_STATE)))
        {
            PlayGenericTone(AUD_GENERIC_WARNING_TONE, MMISET_DEFAULT_DSP_PLAY_RING_TIME);
        }
	#ifdef DPHONE_SUPPORT 
        else if(MMISET_RING_TYPE_HOOK_HANDFREE_TONE == ring_type)
        {
            PlayGenericTone(AUD_GENERIC_DIAL_TONE, AUD_PLAY_FOREVER);
        }
        else if (MMISET_RING_TYPE_HANGUP_REMIND == ring_type)
        {
            PlayGenericTone(AUD_GENERIC_WARNING_TONE, AUD_PLAY_FOREVER);
        }
	#endif 
#ifdef MMI_ETWS_SUPPORT
        else if (MMISET_RING_TYPE_ETWS == ring_type)
        {
            uint8 mode_id = MMIAPIENVSET_GetPlayRingModeId();

            ring_id = MMIENVSET_GetActiveModeRingId(dual_sys, MMISET_RING_TYPE_ETWS);

            SCI_MEMSET(&plmn_info, 0x00, sizeof(plmn_info));
            plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

            ring_vol = MMIAPIENVSET_GetActiveModeRingVol( MMISET_RING_TYPE_ETWS);

            // 播放铃声,对ring_id=0的判断在playring中处理
            PlayRing(dual_sys, ring_id,ring_vol,play_times,ring_type,callback_func,mode_id);
        }
        else if (MMISET_RING_TYPE_ETWS_PWS == ring_type)
        {
            uint8 mode_id = MMIAPIENVSET_GetPlayRingModeId();

            ring_id = MMIENVSET_GetActiveModeRingId(dual_sys, MMISET_RING_TYPE_ETWS_PWS);

            SCI_MEMSET(&plmn_info, 0x00, sizeof(plmn_info));
            plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);


            ring_vol = MMIAPIENVSET_GetActiveModeRingVol( MMISET_RING_TYPE_MSG);

            // 播放铃声,对ring_id=0的判断在playring中处理
            PlayRing(dual_sys, ring_id,ring_vol,play_times,ring_type,callback_func,mode_id);
        }
#endif
        else
        {
            uint8 mode_id = MMIAPIENVSET_GetPlayRingModeId();
            ring_id = MMIENVSET_GetActiveModeRingId(dual_sys, ring_type);
            ring_vol = MMIAPIENVSET_GetActiveModeRingVol( ring_type);

#if defined(MMI_OCR_AUDIO_PLAY_TEST_LONG)// wuxx 20210805
             if(MMISET_RING_TYPE_OCR_AUDIO_TEST == ring_type)
             {
                 ring_id = group_id;
                 ring_vol = MMISET_VOL_MAX; // MMISET_VOL_MAX  9
             }
 #endif

            
            // 播放铃声,对ring_id=0的判断在playring中处理
            PlayRing(dual_sys, ring_id,ring_vol,play_times,ring_type,callback_func,mode_id);
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPISET_PlayRing:NO PERMITPLAYRING"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_625_112_2_18_2_51_39_273,(uint8*)"");
    }
    //还原变量
    MMIAPISET_SetAudioMode(FALSE);
}
//yangyu add begin
PUBLIC void MMIAPISET_UiPlayRingByVolume(
                        MN_DUAL_SYS_E              dual_sys,
                        BOOLEAN                    is_preview, 
                        uint8                      group_id,   //group_id or alarm_id,来电铃声才关心此参数!
                        uint32                     play_times, //the times of play ring
                        MMISET_ALL_RING_TYPE_E     ring_type,  //in: the type of all ring
                        MMISET_RING_CALLBACK_PFUNC callback_func,
                        uint8 volume
                        
                        )
{
    MMISET_ALL_RING_TYPE_E      ring_type_temp  =   MMISET_RING_TYPE_CALL;
    uint8                       ring_id         =   0;
    uint8                       ring_vol        =   0 ;
#ifdef MMI_ETWS_SUPPORT
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
#endif
    AI_API_ForceToStopPlay();//yangyu add for die bug

    ring_vol = volume;

    if(MMISET_RING_TYPE_KEY  == ring_type ||
       MMISET_RING_TYPE_TKEY == ring_type)
    {
    	// TODO: LJF
        if((MMIAPISET_IsPlayingRingType(ring_type)) || (MMIAPICC_IsInState(CC_MO_CONNECTING_STATE)))
        {
            SCI_TRACE_LOW("MMIAPISET_PlayRing: playing ring type or in outgoing state, return!!");
            return;
        }
    }
    if(MMISET_RING_TYPE_DC  == ring_type ||
       MMISET_RING_TYPE_DC_COUNT == ring_type
#ifdef CAMERA_SUPPORT
       /*|| MMISET_RING_TYPE_DC_REC_START == ring_type
       || MMISET_RING_TYPE_DC_REC_STOP == ring_type*/
#endif
	   )
    {
        //ARMVB_SetToneMode(1);
    }
    if (is_preview)//setting中预览来电方式等需要调用此函数
    {
        ring_type_temp = MMISET_RING_TYPE_OTHER;
    }
    else
    {
        ring_type_temp = ring_type;
    }
    
    //播放铃声以及短信息时需要耳机+speaker模式
    if ((MMISET_RING_TYPE_CALL == ring_type) || (MMISET_RING_TYPE_MSG == ring_type)
		#ifdef MMI_INDIAN_SOS_SUPPORT
		||(MMISET_RING_TYPE_SOS == ring_type)
		#endif
              )
    {
        MMIAPISET_SetAudioMode(TRUE);
    }
    if(MMIAPISET_IsPermitPlayRing(ring_type_temp)) 
    {
        //停掉所有铃声和振动
		MMI_CheckAllocatedMemInfo();  //zhouyanxian bug112641
	
        MMIAPISET_StopAppRing();
		
		MMI_CheckAllocatedMemInfo();  //zhouyanxian bug112641	
	
        MMIAPISET_SetCurRingType(ring_type_temp);
        
        if (MMISET_RING_TYPE_CALL_IN_CALL == ring_type)
        {
            MN_PLMN_T cur_plmn_ptr;
            
            MMIAPIPHONE_GetCurPLMN(&cur_plmn_ptr, dual_sys);

            if( 505 == cur_plmn_ptr.mcc )// The MCC of Australia is 505 // "AUS"
            {
                //change call waiting tone frequency and duration according to Spec in Australlia. frequency 425, 375ms on and 375ms off.
                PlayGenericTone(AUD_GENERIC_WARNING_TONE, MMISET_CALL_IN_CALL_PLAY_RING_TIME_AUS);
            }
            else
            {
                PlayGenericTone(AUD_GENERIC_WARNING_TONE, MMISET_CALL_IN_CALL_PLAY_RING_TIME);
            }
        }
        else if (MMISET_RING_TYPE_MSG_IN_CALL == ring_type)
        {
            PlayGenericTone(AUD_GENERIC_WARNING_TONE, MMISET_DEFAULT_DSP_PLAY_RING_TIME);
        }
        else if(MMISET_RING_TYPE_MINHINT_IN_CALL == ring_type)
        {
            PlayGenericTone(AUD_GENERIC_WARNING_TONE, MMISET_DEFAULT_DSP_PLAY_RING_TIME);
        }
        else if((MMISET_RING_TYPE_BAT == ring_type) && (MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE) || MMIAPICC_IsInState(CC_MO_CONNECTING_STATE)))
        {
            PlayGenericTone(AUD_GENERIC_WARNING_TONE, MMISET_DEFAULT_DSP_PLAY_RING_TIME);
        }
	#ifdef DPHONE_SUPPORT 
        else if(MMISET_RING_TYPE_HOOK_HANDFREE_TONE == ring_type)
        {
            PlayGenericTone(AUD_GENERIC_DIAL_TONE, AUD_PLAY_FOREVER);
        }
        else if (MMISET_RING_TYPE_HANGUP_REMIND == ring_type)
        {
            PlayGenericTone(AUD_GENERIC_WARNING_TONE, AUD_PLAY_FOREVER);
        }
	#endif 
#ifdef MMI_ETWS_SUPPORT
        else if (MMISET_RING_TYPE_ETWS == ring_type)
        {
            uint8 mode_id = MMIAPIENVSET_GetPlayRingModeId();

            ring_id = MMIENVSET_GetActiveModeRingId(dual_sys, MMISET_RING_TYPE_ETWS);

            SCI_MEMSET(&plmn_info, 0x00, sizeof(plmn_info));
            plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

            //ring_vol = MMIAPIENVSET_GetActiveModeRingVol( MMISET_RING_TYPE_ETWS);
            if(ring_type == MMISET_RING_TYPE_KEY)
            {
                ring_id = 1;

            }

            // 播放铃声,对ring_id=0的判断在playring中处理
            PlayRing(dual_sys, ring_id,ring_vol,play_times,ring_type,callback_func,mode_id);
            if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
                MMICC_SetMtPresetCodecRoute(0);
        }
        else if (MMISET_RING_TYPE_ETWS_PWS == ring_type)
        {
            uint8 mode_id = MMIAPIENVSET_GetPlayRingModeId();

            ring_id = MMIENVSET_GetActiveModeRingId(dual_sys, MMISET_RING_TYPE_ETWS_PWS);

            SCI_MEMSET(&plmn_info, 0x00, sizeof(plmn_info));
            plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
            if(ring_type == MMISET_RING_TYPE_KEY)
            {
                ring_id = 1;

            }


            //ring_vol = MMIAPIENVSET_GetActiveModeRingVol( MMISET_RING_TYPE_MSG);

            // 播放铃声,对ring_id=0的判断在playring中处理
            PlayRing(dual_sys, ring_id,ring_vol,play_times,ring_type,callback_func,mode_id);
            if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
                MMICC_SetMtPresetCodecRoute(0);
        }
#endif    
        else
        {
            uint8 mode_id = MMIAPIENVSET_GetPlayRingModeId();
            ring_id = MMIENVSET_GetActiveModeRingId(dual_sys, ring_type);
            //ring_vol = MMIAPIENVSET_GetActiveModeRingVol( ring_type);
            if(ring_type == MMISET_RING_TYPE_KEY)
            {
                ring_id = 1;

            }
#ifdef MMI_TEST_1K_SUPPORT
            if(MMISET_RING_TYPE_FACTORY== ring_type)
            {
                ring_id = 1;
                ring_vol = 9;//MMIAPIENVSET_GetActiveModeRingVol( MMISET_RING_TYPE_CALL);
                PlayFixedRing(ring_id,ring_type,play_times,ring_vol,callback_func);
            }
            else
#endif
            // 播放铃声,对ring_id=0的判断在playring中处理
            PlayRing(dual_sys, ring_id,ring_vol,play_times,ring_type,callback_func,mode_id);
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPISET_PlayRing:NO PERMITPLAYRING"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_625_112_2_18_2_51_39_273,(uint8*)"");
    }
    //还原变量
    MMIAPISET_SetAudioMode(FALSE);
}

//yangyu end

/*****************************************************************************/
//  Description : play ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:仅针对MMISET_RING_TYPE_CALL
/*****************************************************************************/
PUBLIC void MMIAPISET_PlayCallRingById(
                                       uint16                       ring_id,
                                       uint32                     play_times, //the times of play ring
                                       MMISET_RING_CALLBACK_PFUNC callback_func
                                       )
{ 
    MMISET_ALL_RING_TYPE_E      ring_type_temp  =   MMISET_RING_TYPE_CALL;
    uint8                       ring_vol        =   0 ;    
    
    
    //播放铃声以及短信息时需要耳机+speaker模式
    MMIAPISET_SetAudioMode(TRUE);
   
    if(MMIAPISET_IsPermitPlayRing(ring_type_temp)) 
    {
        //停掉所有铃声和振动
        MMIAPISET_StopAppRing();
        
        MMIAPISET_SetCurRingType(ring_type_temp);
            
        //ring_id = MMIENVSET_GetActiveModeRingId( ring_type);
        ring_vol = MMIAPIENVSET_GetActiveModeRingVol( ring_type_temp);
        
        // 播放铃声,对ring_id=0的判断在playring中处理
        PlayRing(0,ring_id,ring_vol,play_times,ring_type_temp,callback_func,MMIAPIENVSET_GetActiveModeId());/*lint !e64*/

    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPISET_PlayRing:NO PERMITPLAYRING"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_664_112_2_18_2_51_39_274,(uint8*)"");
    }
    //还原变量
    MMIAPISET_SetAudioMode(FALSE);
}

/*****************************************************************************/
//  Description : play fixed ring by id
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
#ifdef MMISRV_AUDIO_SUPPORT
LOCAL void PlayFixedRing(
                         uint16                      ring_id,    // 固定铃声的index,
                         MMISET_ALL_RING_TYPE_E      ring_type,  //in: the type of all ring
                         uint32                      play_times,
                         uint8                       ring_vol,   // the ring volume 
                         MMISET_RING_CALLBACK_PFUNC  callback_func
                         )
{
    MMIAUD_RING_DATA_INFO_T     ring_data = {MMISRVAUD_RING_FMT_MIDI, 0, NULL};/*lint !e64*/
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;
    BOOLEAN result = FALSE;
    
    //SCI_TRACE_LOW:"[MMISET]:PlayFixedRing, ring_type = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_689_112_2_18_2_51_39_275,(uint8*)"d",ring_type);

    GetRingInfo(ring_id,ring_type,&ring_data);
    
    if (PNULL != ring_data.data_ptr)
    {
        s_ring_callback_func = callback_func;

        if (MMISET_RING_FIXED_DEFAUT_NUM < ring_id)
        {
            //SCI_TRACE_LOW:"PlayFixedRing ring_id = %d is error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_699_112_2_18_2_51_39_276,(uint8*)"d", ring_id);
            ring_id=1;
        }

        handle = MMISRVAUD_GetHandleByName("TONE SRV");
        if(handle > 0)
        {
            SCI_TRACE_LOW("Free tone handle when play key ring !!");
            MMISRVMGR_Free(handle);
        }

        req.is_auto_free = TRUE;
        req.notify = HandleAudioCallBack;
        req.pri = GetRingPri(MMIAPISET_GetCurRingType());

        if (MMIENVSET_PREVIEW_PLAY_DEFAUT_TIMES == play_times)
        {
            audio_srv.play_times = MMIENVSET_PREVIEW_PLAY_RING_TIMES;
            audio_srv.duation = MMIENVSET_PREVIEW_PLAY_RING_DUATION;
        }
        else
        {
            audio_srv.play_times = play_times;
        }

        audio_srv.volume = ring_vol;
        audio_srv.info.ring_buf.type = MMISRVAUD_TYPE_RING_BUF;
        audio_srv.info.ring_buf.fmt = ring_data.type;
        audio_srv.info.ring_buf.data = ring_data.data_ptr;
        audio_srv.info.ring_buf.data_len = ring_data.data_len;

        if(MMISET_RING_TYPE_KEY == ring_type || MMISET_RING_TYPE_TKEY == ring_type)
        {
#ifdef XT_SPEAKER_EAR_COMBO			
            audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE;	
#else
            audio_srv.all_support_route = MMISRVAUD_ROUTE_HANDHOLD | MMISRVAUD_ROUTE_EARPHONE;
			#endif
            audio_srv.is_standing = TRUE;
        }
#ifdef MMI_ETWS_SUPPORT
        else if(MMISET_RING_TYPE_ETWS == ring_type || MMISET_RING_TYPE_ETWS_PWS == ring_type)
        {
            audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER;
        }
#endif
        else if(GetAudioMode())
        {
            audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARFREE;
        }
        else
        {
            audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE;
// Bug 1638429
#if 0
            if( MMISET_RING_TYPE_CALL == ring_type )
            {
                audio_srv.all_support_route |= MMISRVAUD_ROUTE_BLUETOOTH; // Bug 1630851
            }
#endif
        }
#ifdef MMI_KEY_LOCK_SUPPORT
        if(MMISET_RING_TYPE_KEY_LOCK == ring_type)
        {
            audio_srv.is_standing = TRUE;
        }
#endif

        handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
        s_cur_app_handle = handle;
        if(handle > 0)
        {
            //if(MMISET_RING_TYPE_KEY == ring_type || MMISET_RING_TYPE_TKEY == ring_type)
            //{
            //    result = MMISRVAUD_PlayAsy(s_cur_app_handle, 0);
            //}
            //else			
            {
                result = MMISRVAUD_Play(s_cur_app_handle, 0);       
            }
            
            if(result)
            {
                s_cur_real_ring_type = ring_type;                
            }
            else
            {
                MMISRVMGR_Free(s_cur_app_handle);
                CallBackFunction(MMISRVAUD_REPORT_RESULT_ERROR, PNULL);
                s_cur_app_handle = 0;
                s_ring_callback_func = PNULL;
            }
        }
        else
        {
            CallBackFunction(MMISRVAUD_REPORT_RESULT_NOT_SUPPORT, PNULL);
            s_ring_callback_func = PNULL;
        }
    }
    else
    {
        //提示文件格式错误
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
        s_ring_callback_func = PNULL;
    }   
}
#else
LOCAL void PlayFixedRing(
                         uint16                      ring_id,    // 固定铃声的index,
                         MMISET_ALL_RING_TYPE_E      ring_type,  //in: the type of all ring
                         uint32                      play_times,
                         uint8                       ring_vol,   // the ring volume 
                         MMISET_RING_CALLBACK_PFUNC  callback_func
                         )
{
    MMIAUD_RING_DATA_INFO_T     ring_data = {AUD_MIDI_RING, 0, NULL};/*lint !e64*/

    //SCI_TRACE_LOW:"[MMISET]:PlayFixedRing, ring_type = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_776_112_2_18_2_51_40_277,(uint8*)"d",ring_type);

    if (MMISET_RING_FIXED_DEFAUT_NUM < ring_id)
    {
        //SCI_TRACE_LOW:"PlayFixedRing ring_id = %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_780_112_2_18_2_51_40_278,(uint8*)"d", ring_id);
        ring_id=1;
    }
    
    GetRingInfo(ring_id,ring_type,&ring_data);
    s_ring_callback_func = callback_func;
    s_cur_real_ring_type = ring_type;
    
    
    if (PNULL != ring_data.data_ptr)
    {
        MMIAPISET_InitAudioDeviceExt(ring_type,ring_vol);
        
        MMIAPISET_InitEqMode();
        
        MMIAPISET_InitPlayRate();
        
        if(MMIAUDIO_CreateAudioBufHandle(
                FALSE,
                s_cur_audio_htype,
                (MMIAUDIO_TYPE_E)ring_data.type,
                ring_data.data_ptr,
                ring_data.data_len
                ))
        {
            #ifdef PLATFORM_SC6600L     //6600L专用，铃声播放音质问题
            if(MMISET_RING_TYPE_POWER == ring_type      &&
               MMIAUDIO_MIDI          == ring_data.type)
            {
                MMIAUDIO_SetMidiLowPolyNum(s_cur_audio_htype, TRUE);
            }
            #endif

            if(MMISET_RING_TYPE_KEY == ring_type ||
               MMISET_RING_TYPE_TKEY == ring_type)
            {
                if(!MMIAUDIO_AudioPlayAsy(
                        s_cur_audio_htype,
                        0,
                        play_times,
                        callback_func//CallBackFunction
                        ))
                {
                    CallBackFunction(MMIAUDIO_RLT_ERROR, PNULL);
                    MMIAUDIO_CloseAudioHandle(s_cur_audio_htype);
                }
            }
            else
            {
                if(!MMIAUDIO_AudioPlay(
                        s_cur_audio_htype,
                        0,
                        play_times,
                        callback_func//CallBackFunction
                        ))
                {
                    CallBackFunction(MMIAUDIO_RLT_ERROR, PNULL);
                    MMIAUDIO_CloseAudioHandle(s_cur_audio_htype);
                }
            }
        }
        else
        {
            CallBackFunction(MMIAUDIO_RLT_NOT_SUPPORT, PNULL);
        }
    }
    else
    {
        //提示文件格式错误
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
    }   
}
#endif

/*****************************************************************************/
//  Description : play call ring by ring group id
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void PlayRing(
                    MN_DUAL_SYS_E               dual_sys,
                    uint16                       ring_id,    // 固定铃声的index,如果是更多音乐，ID指定为MMISET_MAX_RING_ID
                    uint8                       ring_vol,   // the ring volume 
                    uint32                      play_times,
                    MMISET_ALL_RING_TYPE_E      ring_type,  //in: the type of all ring
                    MMISET_RING_CALLBACK_PFUNC  callback_func,
                    uint8 mode_id
                    )
{
    MMISET_CALL_RING_T          call_ring_info                          =   {MMISET_CALL_RING_FIXED, 0, 0};    
    MMISET_MSG_RING_T           msg_ring_info                           =   {MMISET_MSG_RING_FIXED, 0, 0};
    BOOLEAN                     alm_is_fixed                            =   TRUE;
    uint16                      alm_ring_id                             =   0;
    uint16                      alm_fname_len                           =   0;
    wchar                       alm_fname[MMIFILE_FULL_PATH_MAX_LEN+2]  =   {0};
    
    if (((ring_type==MMISET_RING_TYPE_TKEY)&&(0== ring_id))//bug 1668325
        || ((ring_type!=MMISET_RING_TYPE_TKEY)&&(0 != ring_id)))
    {
        //播放Mp3
        if (MMISET_MAX_RING_ID == ring_id)
        {
            switch(ring_type) 
            {
            case MMISET_RING_TYPE_CALL:
                call_ring_info = MMIAPISET_GetCallRingInfo(dual_sys,mode_id);
               // SCI_ASSERT(MMISET_CALL_RING_MORE_RING == call_ring_info.call_ring_type);
		   if(MMISET_CALL_RING_MORE_RING != call_ring_info.call_ring_type)
		   {
		        //SCI_TRACE_LOW:"mmisetting_assert PlayRing CALL"
		        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_888_112_2_18_2_51_40_279,(uint8*)"");
		        return;
		    } 

           
                if (!MMIAPIFMM_PlayMusicFile(g_is_playing_ring_type,FALSE,call_ring_info.more_ring_info.name_wstr,call_ring_info.more_ring_info.name_wstr_len,play_times,0,ring_vol,callback_func))
                {
                    //play more call ring fail,then fixed ring and reset the nv
                    s_is_need_earfree_mode = TRUE;
                    PlayFixedRing(MMISET_RING_DEFAULT_ID,ring_type,play_times,ring_vol,callback_func);
                    MMIAPIENVSET_SetFixedCallRingId(dual_sys,MMISET_RING_DEFAULT_ID,mode_id);
                }
                break;
                
            case MMISET_RING_TYPE_MSG:
                msg_ring_info = MMIAPISET_GetMsgRingInfo(dual_sys,mode_id);
                //SCI_ASSERT(MMISET_MSG_RING_MORE_RING == msg_ring_info.msg_ring_type);   
		   if(MMISET_MSG_RING_MORE_RING != msg_ring_info.msg_ring_type)
		   {
		        //SCI_TRACE_LOW:"mmisetting_assert PlayRing MSG"
		        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_907_112_2_18_2_51_40_280,(uint8*)"");
		        return;
		    }                                                                                                                             
                if (!MMIAPIFMM_PlayMusicFile(g_is_playing_ring_type,FALSE,msg_ring_info.more_ring_info.name_wstr,msg_ring_info.more_ring_info.name_wstr_len,play_times,0,ring_vol,callback_func))
                {
                    //play more msg ring fail,then fixed ring and reset the nv
                    PlayFixedRing(MMISET_RING_DEFAULT_ID,ring_type,play_times,ring_vol,callback_func);
                    MMIAPIENVSET_SetFixedMsgRingId(dual_sys,MMISET_RING_DEFAULT_ID,mode_id);
                }
                break;
                
            case MMISET_RING_TYPE_ALARM:
                MMIAPIALM_GetFirstAlarmRing(&alm_is_fixed, &alm_ring_id, &alm_fname_len, alm_fname);
                if(alm_is_fixed)
                {
                    //shipeng.lv add for cr151066 at sprd
                    //MMIAPISET_PreviewRing(MN_DUAL_SYS_1, alm_ring_id,ring_vol,play_times,ring_type,callback_func);
                    PlayFixedRing(MMISET_RING_DEFAULT_ID,ring_type,play_times,ring_vol,callback_func);
                    //end shipeng
                }
                else
                {
                    if(!MMIAPIFMM_PlayMusicFile(g_is_playing_ring_type, FALSE,alm_fname, alm_fname_len, MMIENVSET_PREVIEW_PLAY_RING_TIMES,
                        0,ring_vol, callback_func))
                    {
                        
                    //shipeng.lv add for cr151066 at sprd
                    PlayFixedRing(MMISET_RING_DEFAULT_ID,ring_type,play_times,ring_vol,callback_func);
                    //MMIAPISET_PreviewRing(MN_DUAL_SYS_1, MMISET_RING_DEFAULT_ID,ring_vol,play_times,ring_type,PNULL);
                    //end shipeng
               
                    }
                }
                break;
                
            default:
                //SCI_ASSERT(0);
                break;
            }
        }
        else
        {
            //play fixed ring 
            PlayFixedRing(ring_id,ring_type,play_times,ring_vol,callback_func);
        }
    }
    else//声音为0不播放铃声，但是需要调用回调函数
    {
        if(callback_func != PNULL)
        {
            callback_func(MMISRVAUD_REPORT_RESULT_SUCESS, PNULL);
            s_ring_callback_func = PNULL;
        }
    }
}

/*****************************************************************************/
//  Description : MMI_APISET_LivePaperPlayRing
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMI_APISET_LivePaperPlayRing(
                         MMI_RING_ID_T     label,
                         uint8             ring_vol, // the ring volume
                         uint32            play_times
                         )
{
    //MMIAUD_RING_DATA_INFO_T     ring_data = {AUD_MIDI_RING, 0, NULL};/*lint !e64*/

    if (MMIAPISET_IsPermitPlayRing(MMISET_RING_TYPE_OTHER))
    {
        //停掉所有铃声和振动
        //StopAllVibrator();
        MMIAPISET_StopAppRing();        
        MMIAPISET_SetCurRingType(MMISET_RING_TYPE_OTHER);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMISET]:MMI_APISET_LivePaperPlayRing, not PermitPlayRing"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_984_112_2_18_2_51_40_281,(uint8*)"");
        return;
    }

    PlayFixedRing(label, MMISET_RING_TYPE_OTHER, play_times, ring_vol, PNULL);

}
/*****************************************************************************/
//  Description : after play ring ,callback function,at first free ring_data.data_ptr
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
#ifdef MMISRV_AUDIO_SUPPORT
LOCAL void CallBackFunction(MMISRVAUD_REPORT_RESULT_E result, DPARAM param)
{
    // Call ring callback function.
    if (s_ring_callback_func != PNULL)
    {
        (*s_ring_callback_func)(result, param);
    }
}
#else
LOCAL void CallBackFunction(MMIAUDIO_RESULT result, DPARAM param)
{

    // Call ring callback function.
    if (s_ring_callback_func != PNULL)
    {
        (*s_ring_callback_func)(result, param);
    }
}
#endif

/*****************************************************************************/
//  Description : get the mode of device
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note: only use for play ring not voice
/*****************************************************************************/
#ifdef MMISRV_AUDIO_SUPPORT
PUBLIC void MMIAPISET_GetDeviceMode(
                                    AUDIO_DEVICE_MODE_TYPE_E    *mode_ptr//out
                                    )
{
    /* return an invalid value. */
    *mode_ptr = AUDIO_DEVICE_MODE_MAX;
}
#else
PUBLIC void MMIAPISET_GetDeviceMode(
                                    AUDIO_DEVICE_MODE_TYPE_E    *mode_ptr//out
                                    )
{
   // SCI_ASSERT(PNULL != mode_ptr);
   if(PNULL == mode_ptr)
   {
        //SCI_TRACE_LOW:"mmisetting_assert MMIAPISET_GetDeviceMode"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_1036_112_2_18_2_51_40_282,(uint8*)"");
        return;
    } 
#ifdef DPHONE_SUPPORT 
	*mode_ptr = MMIAUDIO_GetCurDevMode();
#else  
    *mode_ptr = AUDIO_DEVICE_MODE_HANDHOLD;
    
    //目前只有手持和耳机模式
#ifndef WIN32
    if (MMIAPITVOUT_GetStartingStatus()) //when tv out, audio is output to tv.
    {
        *mode_ptr = AUDIO_DEVICE_MODE_TVOUT;
        return;
    }
    
    if (GPIO_CheckHeadsetStatus()) 
    {
        //耳机模式
        if (GetAudioMode())
        {
            MMIAPISET_SetAudioMode(FALSE);
            *mode_ptr = AUDIO_DEVICE_MODE_EARFREE;
        }
        else
        {
            *mode_ptr = AUDIO_DEVICE_MODE_EARPHONE;
        }
    }
    else
    {
        *mode_ptr = AUDIO_DEVICE_MODE_HANDFREE;
    }
#endif
#endif
}
#endif
/*****************************************************************************/
//  Description : stop ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
#ifdef MMISRV_AUDIO_SUPPORT
PUBLIC void MMIAPISET_StopRing(MMISET_ALL_RING_TYPE_E     ring_type)
{
    SCI_TRACE_LOW("[MMISET]: MMIAPISET_StopRing ring_type is %d",ring_type);
    MMIAPISET_StopAppRing();
}
#else
PUBLIC void MMIAPISET_StopRing(
                               MMISET_ALL_RING_TYPE_E     ring_type//in: the type of all ring
                               )
{
    //SCI_TRACE_LOW:"[MMISET]: MMIAPISET_StopRing enter--ring_type is %d and g_is_playing_ring_type is %d,s_cur_audio_htype = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_1088_112_2_18_2_51_40_283,(uint8*)"ddd",ring_type,g_is_playing_ring_type,s_cur_audio_htype);
    
    if ((MMISET_RING_TYPE_CALL_IN_CALL    == ring_type) ||
        (MMISET_RING_TYPE_MSG_IN_CALL     == ring_type) ||
        (MMISET_RING_TYPE_MINHINT_IN_CALL == ring_type))
    {
        StopGenericTone();
    }
    
    if(MMIAUDIO_GetIsPlaying() &&
        (g_is_playing_ring_type == ring_type))
    {
        if (s_set_is_stop_mp3)
        {
            //StopMp3Ring();
            s_set_is_stop_mp3 = FALSE;
        }
        
        if (MMIAUDIO_PLAYER != s_cur_audio_htype
            && MMIAUDIO_KURO != s_cur_audio_htype
        ) //这里只负责停止铃声

        {
            MMIAUDIO_AudioStop(s_cur_audio_htype);
        }
    }
    MMIAPISET_ClearCurRingType(ring_type);
}
#endif

/*****************************************************************************/
//  Description : stop all ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
#ifdef MMISRV_AUDIO_SUPPORT
PUBLIC void MMIAPISET_StopAllRing(
                                  BOOLEAN     is_include_mp3  //是否包括mp3的背景播放
                                  )
{
    SCI_TRACE_LOW("MMIAPISET_StopAllRinge is %d",is_include_mp3);
    MMIAPISET_StopAppRing();
}
#else
PUBLIC void MMIAPISET_StopAllRing(
                                  BOOLEAN     is_include_mp3  //是否包括mp3的背景播放
                                  )
{
    /*
    if ((MMISET_RING_TYPE_CALL_IN_CALL    == g_is_playing_ring_type) ||
        (MMISET_RING_TYPE_MSG_IN_CALL     == g_is_playing_ring_type) ||
        (MMISET_RING_TYPE_MINHINT_IN_CALL == g_is_playing_ring_type))
    {
        StopGenericTone();
    }
    */

    MMIAUDIO_StopTone(MMIAUDIO_GENERIC_TONE);
    MMIAUDIO_StopTone(MMIAUDIO_CUSTOM_TONE);
    //MMIAUDIO_StopTone(MMIAUDIO_DTMF_TONE);

    if (MMIAUDIO_PLAYER != s_cur_audio_htype
        && MMIAUDIO_KURO != s_cur_audio_htype
        && MMIAUDIO_MUSIC != s_cur_audio_htype
#ifdef JAVA_SUPPORT
	    &&MMIAUDIO_JAVA != s_cur_audio_htype
#endif               
#ifdef WRE_SUPPORT
        &&MMIAUDIO_WRE != s_cur_audio_htype		
#endif
        ) //这里只负责停止铃声,mp3之类的是不能停的。

    {
        //SCI_TRACE_LOW:"MMIAPISET_StopAllRing s_cur_audio_htype= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_1159_112_2_18_2_51_41_284,(uint8*)"d",s_cur_audio_htype);
#ifdef MMI_RECORD_SUPPORT
        if(MMIAPIRECORD_IsOpened())
        {
            MMIAPIRECORD_StopRecordOrPlay();
        }
		MMIENG_StopENGRecord();
#ifdef OLD_CC_SUPPORT //for 217 ram cutdown:remove old cc
        MMIAPICC_StopCCRecord();
#else
        MMICC_StopRecordInCall();
#endif
#endif

#if defined(TTS_SUPPORT) || defined(HERO_ENGINE_TTS_SUPPORT)
        MMIAPITTS_StopPlayText();
#endif

        MMIAUDIO_AudioStop(s_cur_audio_htype);

        MMIAPISET_ClearCurRingType(MMIAPISET_GetCurRingType());
    }
}
#endif

/*****************************************************************************/
//  Description : play ring with ring id
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_PlayRingById(
                                   BOOLEAN                      is_earfree, //是否需要耳机下也支持speaker播放
                                   uint16                       ring_id,    //固定铃声的index,如果是更多音乐，ring_id设为MMISET_MAX_RING_ID
                                   uint32                       play_times, //the times of play ring
                                   MMISET_ALL_RING_TYPE_E       ring_type,  //in: the type of all ring
                                   MMISET_RING_CALLBACK_PFUNC   callback_func
                                   )
{
    uint8   ring_vol = MMIAPIENVSET_GetActiveModeRingVol(ring_type);
    MMIAPISET_PlayRingByIdEx(is_earfree, ring_id, ring_vol, play_times, ring_type, callback_func);
}

/*****************************************************************************/
//  Description : play ring with ring id
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_PlayRingByIdEx(
									BOOLEAN                      is_earfree, //是否需要耳机下也支持speaker播放
									uint16                       ring_id,    //固定铃声的index,如果是更多音乐，ring_id设为MMISET_MAX_RING_ID
									uint8   ring_vol,//音量
									uint32                       play_times, //the times of play ring
									MMISET_ALL_RING_TYPE_E       ring_type,  //in: the type of all ring
									MMISET_RING_CALLBACK_PFUNC   callback_func
                                   )
{
    if ((is_earfree) && ((MMISET_RING_TYPE_ALARM == ring_type)
	#ifdef MMI_INDIAN_SOS_SUPPORT
	||(MMISET_RING_TYPE_SOS == ring_type)
	#endif
		))
    {
        MMIAPISET_SetAudioMode(TRUE);
    }
    
    //通过id播放的铃声都是other类型的
    if (MMIAPISET_IsPermitPlayRing(MMISET_RING_TYPE_OTHER))
    {
        //停掉所有铃声和振动
        MMIAPISET_StopAppRing();
        
        MMIAPISET_SetCurRingType(MMISET_RING_TYPE_OTHER);
        
        PlayRing(MMIAPISET_GetActiveSim(), ring_id,ring_vol,play_times,ring_type,callback_func,MMIAPIENVSET_GetActiveModeId());
        //PlayRing(0,ring_id,ring_vol,play_times,ring_type,callback_func);
    }
    
    if ((is_earfree) && ((MMISET_RING_TYPE_ALARM == ring_type)
	#ifdef MMI_INDIAN_SOS_SUPPORT
	||(MMISET_RING_TYPE_SOS == ring_type)
	#endif
		))
    {
        MMIAPISET_SetAudioMode(FALSE);
    }
}

/*****************************************************************************/
//  Description : play call ring with ring volume
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_PlayCallRingByVol(
                                        uint8                      ring_vol,
                                        uint8                        ring_id,
                                        uint32                     play_times, //the times of play ring
                                        MMISET_ALL_RING_TYPE_E     ring_type,   //in: the type of all ring
                                        MMISET_RING_CALLBACK_PFUNC callback_func
                                        )
{
    MMIAPISET_PlayRingByIdEx(FALSE, ring_id, ring_vol, play_times, ring_type, callback_func);
}

/*****************************************************************************/
//  Description : set the type of call ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetCallRingType(
                                      MMISET_CALL_RING_TYPE_E  call_ring_type,//in
                                      MN_DUAL_SYS_E            dual_sys
                                      )
{
    MMIENVSET_SetCurModeOptValue(CALL_RING_TYPE, (uint8)call_ring_type, dual_sys);
    
    
    //SCI_TRACE_LOW:"MMISET_SetCallRingType:call_ring_type is %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_1256_112_2_18_2_51_41_285,(uint8*)"d",call_ring_type);
    
     
    // 刷新待机画面的图标
    SetIdleCallRingTypeIcon(call_ring_type);
}

/*****************************************************************************/
//  Description : set the type of message ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
//LOCAL void SetMsgRingType(
//                          MMISET_MSG_RING_TYPE_E  msg_ring_type//in
//                          )
//{
//    MMIENVSET_SetActiveModeOptValue(MSG_RING_TYPE, (uint8)msg_ring_type, MMIAPISET_GetActiveSim());
//}   

/*****************************************************************************/
//  Description : get the volume of call ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
uint8 MMIAPISET_GetCallRingVolume(void)
{
    uint8   ring_vol    =   0;
    ring_vol = MMIAPIENVSET_GetActiveModeRingVol( MMISET_RING_TYPE_CALL);
    
    return ring_vol;
}

/*****************************************************************************/
//  Description : set the volume of call ring communication
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetCallRingVolume(
                                    uint8     call_volume//in
                                    )
{
    MMIENVSET_SetActiveModeOptValue(CALL_RING_VOL, call_volume, MMIAPISET_GetActiveSim());
}

/*****************************************************************************/
//  Description : get the volume of call communication
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetCallVolume(void)
{
    return  MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_1, COMM_VOL);
    
}

/*****************************************************************************/
//  Description : set the volume of call communication
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetCallVolume(
                                    uint8     call_volume//in
                                    )
{
    MMIENVSET_SetActiveModeOptValue(COMM_VOL, call_volume, MMIAPISET_GetActiveSim());
}

/*****************************************************************************/
//  Description : set the ring of key
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
//LOCAL void SetKeyToneRing(
//                          MMISET_OTHER_RING_TYPE_E      key_ring//in
//                          )
//{
//    MMISET_OTHER_RING_T     key_ring_info = {0};
//    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
//    
//    //get the information about key
//    SCI_MEMSET(&key_ring_info,0,sizeof(MMISET_OTHER_RING_T));
//    
//    MMINV_READ(MMINV_SET_KEY_RING_INFO,&key_ring_info,return_value);
//    if (MN_RETURN_SUCCESS != return_value)
//    {
//        key_ring_info.other_ring_type = MMISET_OTHER_RING1;
//        key_ring_info.other_ring_volume = MMISET_VOL_THREE;
//    }
//    
//    key_ring_info.other_ring_type = key_ring;
//    MMINV_WRITE(MMINV_SET_KEY_RING_INFO,&key_ring_info);
//}

/*****************************************************************************/
//  Description : set the ring of flip ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
//LOCAL void SetFlipRing(
//                       MMISET_OTHER_RING_TYPE_E     flip_ring//in
//                       )
//{
//    MMINV_WRITE(MMINV_SET_FLIP_RING,&flip_ring);
// }

/*****************************************************************************/
//  Description : get if open low voltage warning
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetIsOpenVoltageWarning(void)
{
    uint8       is_open     =   0;
    BOOLEAN     result      =   TRUE;
    
    is_open = MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_1, VOLTAGE_WARN);
    
    result = (is_open == 1)?FALSE:TRUE;
    
    return  result;
}

/*****************************************************************************/
//  Description : set open or close low voltage warning
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_IsOpenVoltageWarning(
//                                        BOOLEAN  is_open
//                                        )
//{
//    MMINV_WRITE(MMINV_SET_VOLTAGE_WARNING,&is_open);
// }

/*****************************************************************************/
//  Description : to append one item to listbox by text id
//  Global resource dependence : 
//  Author:louis.wei
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_AppendListItemByTextId(                                   
                                             MMI_TEXT_ID_T        text_id,
                                             MMI_TEXT_ID_T        left_softkey_id,
                                             MMI_CTRL_ID_T      ctrl_id,
                                             GUIITEM_STYLE_E      item_style
                                             )
{
    MMIAPISET_AppendListItemByTextIdExt( text_id, 
										left_softkey_id, 
#ifdef MMI_GUI_STYLE_MINISCREEN
										IMAGE_NULL,
#else
										TXT_NULL,
#endif
										STXT_RETURN,
										ctrl_id, 
										item_style );    
}

/*****************************************************************************/
//  Description : to append one item to listbox by text buffer and set soft keys
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_AppendListItemByTextBuffer(                                    
                                                const wchar *        text_ptr,
                                                uint16               text_len,
                                                uint32               user_data,
                                                MMI_TEXT_ID_T        left_softkey_id,
                                                MMI_TEXT_ID_T        middle_softkey_id,
                                                MMI_TEXT_ID_T        right_softkey_id,
                                                MMI_CTRL_ID_T        ctrl_id,
                                                GUIITEM_STYLE_E      item_style
                                                )
{
    GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
    
    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = text_ptr;/*lint !e605*/
    item_data.item_content[0].item_data.text_buffer.wstr_len = text_len;
    
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = middle_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    
    item_t.user_data = user_data;
    
    GUILIST_AppendItem( ctrl_id, &item_t );      
}

//@zhaohui add 
/*****************************************************************************/
//  Description : to append one item to listbox by text id and set soft keys
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_AppendListItemByTextIdExt(                                    
                                                MMI_TEXT_ID_T        text_id,
                                                MMI_TEXT_ID_T        left_softkey_id,
                                                MMI_TEXT_ID_T        middle_softkey_id,
                                                MMI_TEXT_ID_T        right_softkey_id,
                                                MMI_CTRL_ID_T        ctrl_id,
                                                GUIITEM_STYLE_E      item_style
                                                )
{
    GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
    
    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;
    
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = middle_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    
    GUILIST_AppendItem( ctrl_id, &item_t );      
}


/*****************************************************************************/
//  Description : to append a group of items to listbox by prefix text id and num.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_AppendFixedItems(
                                                                        MMI_TEXT_ID_T        prefix_txt_id,
                                                                        uint32                      item_num,      
                                                                        MMI_TEXT_ID_T        left_softkey_id,
                                                                        MMI_TEXT_ID_T        middle_softkey_id,
                                                                        MMI_TEXT_ID_T        right_softkey_id,
                                                                        MMI_CTRL_ID_T         ctrl_id,
                                                                        GUIITEM_STYLE_E      item_style
                                                                        )
{
    #define MMISET_PUB_LIST_STR_LEN    30 

    uint8                       i = 0;
    MMI_STRING_T        string = {0};
    wchar wbuff[MMISET_PUB_LIST_STR_LEN + 4]={0};
    int8 buff[3 + 1]={0};

    if (item_num > 999)
    {
        return;
    }
    
    MMIRES_GetText( prefix_txt_id, 0, &string );
    /* 容错*/
    if(string.wstr_len > MMISET_PUB_LIST_STR_LEN)
    {
        string.wstr_len = MMISET_PUB_LIST_STR_LEN;
    }
    MMIAPICOM_Wstrncpy(wbuff, string.wstr_ptr, string.wstr_len);
    
    //add items to list box
    for (i = 0; i < item_num; i++)
    {   
        sprintf(buff, "%d", i + 1);
        MMIAPICOM_StrToWstr((uint8 *)buff, wbuff + string.wstr_len);
        //MMIAPICOM_Wstrcpy(list[i], wbuff);
        MMIAPISET_AppendListItemByTextBuffer(
                                                    wbuff,
                                                    string.wstr_len + strlen(buff),
                                                    0,
                                                    left_softkey_id, 
                                                    middle_softkey_id, 
                                                    right_softkey_id, 
                                                    ctrl_id, 
                                                    item_style
                                                    );
    }
}

/*****************************************************************************/
//  Description : 根据radio list选中的item，设置来电提示方式,消息提示方式，按键音和翻盖提示音
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_SetSelectRadioItem(
//                                      MMI_CTRL_ID_T         ctrl_id,
//                                      MMISET_ALL_RING_TYPE_E   ring_type
//                                      )
//{
//    uint16      cur_selection = 0;
//    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
//    
//    //get the selected item
//    GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
//    
//    switch (ring_type)
//    {
//    case MMISET_RING_TYPE_CALL:
//        //来电提示 if cur_selection is valid
//        SCI_ASSERT((MMISET_MAX_CALL_TYPE > cur_selection));
//        
//        MMIAPISET_SetCallRingType((MMISET_CALL_RING_TYPE_E)cur_selection, dual_sys);
//        break;
//        
//    case MMISET_RING_TYPE_MSG:
//        //消息提示 if cur_selection is valid
//        SCI_ASSERT((MMISET_MAX_MSG_TYPE > cur_selection));
//        
//        SetMsgRingType((MMISET_MSG_RING_TYPE_E)cur_selection);
//        break;
//        
//    case MMISET_RING_TYPE_KEY:
//        //按键音 if cur_selection is valid
//        SCI_ASSERT((MMISET_MAX_OTHER_RING > cur_selection));
//        
//        SetKeyToneRing((MMISET_OTHER_RING_TYPE_E)cur_selection);
//        break;
//        
//    case MMISET_RING_TYPE_FLIP:
//        //翻盖提示音 if cur_selection is valid
//        SCI_ASSERT((MMISET_MAX_OTHER_RING > cur_selection));
//        
//        SetFlipRing((MMISET_OTHER_RING_TYPE_E)cur_selection);
//        break;
//        
//    default:
//        SCI_TRACE_LOW("MMISET_SetSelectRadioItem:ring_type is error!");
//        break;
//    }
//}

/*****************************************************************************/
//  Description : 添加低电压报警音的开启关闭的单选list items
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_AppendVoltageWarningListItem(
//                                                MMI_CTRL_ID_T         ctrl_id
//                                                )
//{
//    uint16      cur_selection = 0;
//    
//    //add items to listbox
//    MMIAPISET_AppendListItemByTextId( TXT_OPEN, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
//    MMIAPISET_AppendListItemByTextId( TXT_CLOSE, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
//    
//    if (MMIAPISET_GetIsOpenVoltageWarning())
//    {
//        cur_selection = 0;
//    }
//    else
//    {
//        cur_selection = 1;
//    }
//    
//    //set selected item
//    GUILIST_SetSelectedItem( ctrl_id, cur_selection, TRUE );
//    
//    //set current item
//    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
//}



/*****************************************************************************/
//  Description : 根据radio list当前阴影处于的来电提示方式，
//                  来播放相应的铃声，振动或者静音等
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_PlayCallTypeByListItem(
//                                          MMI_CTRL_ID_T   ctrl_id
//                                          )
//{
//    uint16      cur_item = 0;
//    
//    //get the current item
//    cur_item = GUILIST_GetCurItemIndex(ctrl_id);
//    
//    //play ring or vibra
//    switch (cur_item)
//    {
//    case 0:       
//        //play ring
//        MMIAPISET_PlayRing(MMIAPISET_GetActiveSim(), TRUE,0,1,MMISET_RING_TYPE_CALL,PNULL);
//        break;
//        
//    case 1:       
//        //vibra
//        if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_OTHER))
//        {
//            MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_OTHER);
//            if (g_mmiset_vibra_timer_id != 0)
//            {
//                MMK_StopTimer(g_mmiset_vibra_timer_id);
//                g_mmiset_vibra_timer_id = 0;
//            }
//            g_mmiset_vibra_timer_id = MMK_CreateTimer(MMISET_VIBRA_DEFAULT_TIME,FALSE);
//        }
//        break;
//        
//    case 2:      
//        //silent
//        MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_OTHER);
//        break;
//        
//    case 3:      
//        if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_OTHER))
//        {
//            //vibra before ring
//            MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_OTHER);
//            if (g_mmiset_vibra_timer_id != 0)
//            {
//                MMK_StopTimer(g_mmiset_vibra_timer_id);
//                g_mmiset_vibra_timer_id = 0;
//            }
//            g_mmiset_vibra_timer_id = MMK_CreateTimer(MMISET_VIBRA_DEFAULT_TIME,FALSE);
//        }
//        break;
//        
//    case 4:
//        //vibra and ring      
//        if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_OTHER))
//        {
//            MMIAPISET_PlayRing(MMIAPISET_GetActiveSim(), TRUE,0,1,MMISET_RING_TYPE_CALL,RingCallBack);
//            MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_OTHER);
//        }
//        break;
//        
//    default:
//        SCI_TRACE_LOW("MMISET_PlayCallTypeByListItem:cur_item %d is error!",cur_item);
//        break;
//    }
//}

/*****************************************************************************/
//  Description : to STOP the vibrator
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
//LOCAL void RingCallBack(MMIAUDIO_RESULT result, DPARAM param)
//{
//    MMIAPISET_AlertRingPlayResult(result, param);
//    MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
//}

/*****************************************************************************/
//  Description : 根据radio list当前阴影处于的消息提示方式，
//                  来播放相应的铃声，振动或者静音等
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_PlayMsgTypeByListItem(
//                                         MMI_CTRL_ID_T    ctrl_id
//                                         )
//{
//    uint16      cur_item = 0;
//    
//    //get the current item
//    cur_item = GUILIST_GetCurItemIndex(ctrl_id);
//    
//    //play ring or vibra
//    switch (cur_item)
//    {
//    case 0:        
//        //play ring
//        MMIAPISET_PlayRing(MMIAPISET_GetActiveSim(), TRUE,0,1,MMISET_RING_TYPE_MSG,PNULL);
//        break;
//        
//    case 1:      
//        //vibra
//        if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_OTHER))
//        {
//            MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_OTHER);
//            if (g_mmiset_vibra_timer_id != 0)
//            {
//                MMK_StopTimer(g_mmiset_vibra_timer_id);
//                g_mmiset_vibra_timer_id = 0;
//            }
//            g_mmiset_vibra_timer_id = MMK_CreateTimer(MMISET_VIBRA_DEFAULT_TIME,FALSE);
//        }
//        break;
//        
//    case 2:      
//        //silent
//        MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_OTHER);
//        break;
//        
//    case 3:
//        //vibra and ring        
//        if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_OTHER))
//        {
//            MMIAPISET_PlayRing(MMIAPISET_GetActiveSim(), TRUE,0,1,MMISET_RING_TYPE_MSG,RingCallBack);
//            MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_OTHER);
//        }
//        break;
//        
//    default:
//        SCI_TRACE_LOW("MMISET_PlayMsgTypeByListItem:cur_item %d is error!",cur_item);
//        break;
//    }
//}

/*****************************************************************************/
//  Description : 根据radio list当前阴影处于的按键音，
//                  来播放相应的铃声或者静音等
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_PlayKeyToneByListItem(
//                                         MMI_CTRL_ID_T    ctrl_id
//                                         )
//{
//    uint16      cur_item = 0;
//    
//    //get the current item
//    cur_item = GUILIST_GetCurItemIndex(ctrl_id);
//    
//    //play ring or vibra
//    switch (cur_item)
//    {
//    case 0:    
//        //silent
//        break;
//        
//    case 1: 
//        //play ring1
//        MMIAPISET_PlayRingById(FALSE,MMISET_KEY_RING1_ID,1,MMISET_RING_TYPE_KEY,PNULL);
//        break;
//        
//    case 2:     
//        //play ring2
//        MMIAPISET_PlayRingById(FALSE,MMISET_KEY_RING2_ID,1,MMISET_RING_TYPE_KEY,PNULL);
//        break;
//        
//    default:
//        SCI_TRACE_LOW("MMISET_PlayKeyToneByListItem:cur_item %d is error!",cur_item);
//        break;
//    }
//}

/*****************************************************************************/
//  Description : 根据radio list当前阴影处于的按键音，
//                  来播放相应的铃声或者静音等
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_PlayFlipRingByListItem(
//                                          MMI_CTRL_ID_T   ctrl_id
//                                          )
//{
//    uint16      cur_item = 0;
//    
//    //get the current item
//    cur_item = GUILIST_GetCurItemIndex(ctrl_id);
//    
//    //play ring or vibra
//    switch (cur_item)
//    {
//    case 0:       
//        //silent
//        break;
//        
//    case 1:       
//        //play ring1
//        MMIAPISET_PlayRingById(FALSE,MMISET_FLIP_RING1_ID,1,MMISET_RING_TYPE_FLIP,PNULL);
//        break;
//        
//    case 2:      
//        //play ring2
//        MMIAPISET_PlayRingById(FALSE,MMISET_FLIP_RING2_ID,1,MMISET_RING_TYPE_FLIP,PNULL);
//        break;
//        
//    default:
//        SCI_TRACE_LOW("MMISET_PlayFlipRingByListItem:cur_item %d is error!",cur_item);
//        break;
//    }
//}
//

/*****************************************************************************/
//  Description : to get volume picture position
//  Global resource dependence : 
//  Author: xiangjie
//  Note: 
/*****************************************************************************/
//GUI_POINT_T GetVolumePicturePoint(uint8 volume)
//{
//    GUI_POINT_T         point = {0};
//    
//    switch (volume)
//    {
//    case MMISET_VOL_ONE:
//        point.x = MMISET_RING_VOL_LEVEL_ONE_X;
//        point.y = MMISET_RING_VOL_LEVEL_ONE_Y;
//        break;
//        
//    case MMISET_VOL_TWO:
//        point.x = MMISET_RING_VOL_LEVEL_TWO_X;
//        point.y = MMISET_RING_VOL_LEVEL_TWO_Y;
//        break; 
//        
//    case MMISET_VOL_THREE:
//        point.x = MMISET_RING_VOL_LEVEL_THREE_X;
//        point.y = MMISET_RING_VOL_LEVEL_THREE_Y;
//        break;
//        
//    case MMISET_VOL_FOUR:
//        point.x = MMISET_RING_VOL_LEVEL_FOUR_X;
//        point.y = MMISET_RING_VOL_LEVEL_FOUR_Y;
//        break; 
//        
//    case MMISET_VOL_FIVE:
//        point.x = MMISET_RING_VOL_LEVEL_FIVE_X;
//        point.y = MMISET_RING_VOL_LEVEL_FIVE_Y;
//        break;
//        
//    case MMISET_VOL_SIX:
//        point.x = MMISET_RING_VOL_LEVEL_SIX_X;
//        point.y = MMISET_RING_VOL_LEVEL_SIX_Y;
//        break; 
//        
//    case MMISET_VOL_SEVEN:
//        point.x = MMISET_RING_VOL_LEVEL_SEVEN_X;
//        point.y = MMISET_RING_VOL_LEVEL_SEVEN_Y;
//        break;
//        
//    default:
//        SCI_TRACE_LOW("GetVolumePicturePoint:volume %d is error!",volume);
//        break;
//    }   
//    
//    return point;
//}
#ifndef MMI_ENVSET_MINI_SUPPORT
/*****************************************************************************/
//  Description :肯据所给总级别连续画出对应个数的背景block
//  Global resource dependence : 
//  Author: murphy xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_DisplayVolumeBlockBgPicture(
                                                uint8	   blocknum,
                                                MMI_WIN_ID_T win_id,MMI_IMAGE_ID_T img_id)
{

    uint32              i            = 0;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    // GUI_RECT_T          image_rect   = MMITHEME_GetFullScreenRect();//{0, 0, MMI_MAINSCREEN_WIDTH, MMI_MAINSCREEN_HEIGHT};
    GUI_RECT_T          display_rect = MMITHEME_GetClientRectEx(win_id);//{0, MMI_TITLE_HEIGHT, MMI_MAINSCREEN_WIDTH, MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT};
    GUI_POINT_T         dis_point    = {0};
    uint16              adj_bar_width = 0;
    uint16              adj_bar_height = 0;
    uint16              vol_width = 0;
    uint16              vol_height = 0;
    uint16              icon_space = 0;

    //GUILCD_GetLogicWidthHeight(GUI_LCD_0,&lcd_width,&lcd_height);
    GUIRES_GetImgWidthHeight(&adj_bar_width,&adj_bar_height, img_id,win_id);
    GUIRES_GetImgWidthHeight(&vol_width,&vol_height, IMAGE_ADJUST_VOL_BG,win_id);
    if(MMIPHONE_ADJUST_VOL_MAX_STEPS * adj_bar_width < display_rect.right - display_rect.left)
    {
        icon_space = (display_rect.right - display_rect.left - (MMIPHONE_ADJUST_VOL_MAX_STEPS * adj_bar_width))/(MMIPHONE_ADJUST_VOL_MAX_STEPS + 1);
    }
    //dis_point.x = icon_space;//(lcd_width - MMIPHONE_ADJUST_VOL_LEVEL_BOX_WIDHT)>>1;
    if(adj_bar_height + vol_height < display_rect.bottom - display_rect.top)
    {
        dis_point.y = display_rect.top + (display_rect.bottom - display_rect.top - (adj_bar_height + vol_height))/2;
    }
    else
    {
        dis_point.y = display_rect.top + MMISET_VOLUME_IMG_DISPLAY_MARGEN;
    }
    for (i = 0; i < blocknum; i ++)
    {
        dis_point.x = icon_space + (icon_space + adj_bar_width)*i;

        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            img_id,
            &lcd_dev_info);
    }
}
#endif
#if (!defined (MMI_ENVSET_MINI_SUPPORT) ||defined(EBOOK_TTS_SELF_VOLUME) )
/*****************************************************************************/
//  Description : 根据音量的大小显示音量图片
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_DisplayVolumePicture(
                                           uint8   volume,
                                           MMI_WIN_ID_T win_id
                                           )
{
#ifdef MONOCHROME_LCD_SUPPORT 
    MMISET_DrawAdjustVolumePicture(volume,win_id);
#else
    //uint32              i               =   0;
    GUI_LCD_DEV_INFO    lcd_dev_info    =   {GUI_LCD_0, GUI_BLOCK_0};
    //GUI_RECT_T          image_rect      =   MMITHEME_GetFullScreenRect();
    GUI_RECT_T          display_rect    =   MMITHEME_GetClientRectEx(win_id);
#ifdef GUIF_SOFTKEY_PDASTYLE
    uint16              softkey_width    =  0;
    uint16              softkey_height    =  0;
#endif
    GUI_POINT_T         dis_point       =   {0};
    //uint16      lcd_width = 0;
    //uint16      lcd_height = 0;
	uint16		vol_bg_left =0;
	uint16      vol_bg_top =0;
	uint16		vol_bg_image_width =0;
	uint16		vol_bg_image_height =0;
	uint16		vol_bg_plus_width =0;
	uint16		vol_bg_plus_height =0;

	//GUILCD_GetLogicWidthHeight(GUI_LCD_0,&lcd_width,&lcd_height);
	GUIRES_GetImgWidthHeight(&vol_bg_image_width,&vol_bg_image_height, IMAGE_ADJUST_VOL_BG,win_id);

	GUIRES_GetImgWidthHeight(&vol_bg_plus_width,&vol_bg_plus_height, IMAGE_ADJUST_VOL_PLUG,win_id);

	
	vol_bg_left = display_rect.left + (display_rect.right - display_rect.left - vol_bg_image_width)/2;//水平居中
	
#ifdef GUIF_SOFTKEY_PDASTYLE
    {
        GUIRES_GetImgWidthHeight(&softkey_width,&softkey_height, IMAGE_PDASOFTKEY_BG,win_id);
        display_rect.bottom -= softkey_height;//加上softkey的高度 
    }
#endif    
	vol_bg_top = display_rect.bottom - vol_bg_image_height;//位于softkey上面

#if 0
    GUIRES_DisplayImg(PNULL,
        &display_rect,
        &image_rect,
        win_id,
        IMAGE_COMMON_BG,
        &lcd_dev_info);
#endif
    MMIAPISET_DisplayVolumeBlockBgPicture(MMIPHONE_ADJUST_VOL_MAX_STEPS,win_id,IMAGE_ADJUST_VOL_BLOCK_SINGLE_BG);
    //音量喇叭图标
    dis_point.x = vol_bg_left;
    dis_point.y = vol_bg_top;
    GUIRES_DisplayImg(&dis_point,
        PNULL,
        PNULL,
        win_id,
        IMAGE_ADJUST_VOL_BG,
        &lcd_dev_info);

    MMIAPISET_DisplayVolumeBlockBgPicture(volume,win_id,IMAGE_ADJUST_VOL_BLOCK_FG);
    //音量调节增加按钮
    dis_point.x =  display_rect.right - vol_bg_plus_width;
    dis_point.y =  (vol_bg_top + ((vol_bg_image_height - vol_bg_plus_height) >> 1)); //音量调节增加/减少按钮的上边坐标;
    GUIRES_DisplayImg(&dis_point,
        PNULL,
        PNULL,
        win_id,
        IMAGE_ADJUST_VOL_PLUG,
        &lcd_dev_info);
    //音量调节减少按钮
    dis_point.x = 0;
    //dis_point.y = (vol_bg_top + ((MMIPHONE_ADJUST_VOL_BG_HEIGHT - MMIPHONE_ADJUST_VOL_PLUG_HEIGHT) >> 1)); //音量调节增加/减少按钮的上边坐标;
    GUIRES_DisplayImg(&dis_point,
        PNULL,
        PNULL,
        win_id,
        IMAGE_ADJUST_VOL_MINUS,
        &lcd_dev_info);
#endif
}
#endif
/*****************************************************************************/
//  Description : 左右方向键调节音量的大小
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_TuneUpVolume(
//                                MMI_MESSAGE_ID_E           msg_id,
//                                MMISET_ALL_VOLUME_TYPE_E   volume_type
//                                )
//{
//    
//    BOOLEAN     is_volume_change = FALSE;//音量是否变化
//    uint8       cur_volume = 0;
//    
//    //get current volume
//    if (MMISET_RING_VOLUME_KEY == volume_type)
//    {
//        cur_volume = g_cur_key_volume;
//    }
//    else
//    {
//        cur_volume = g_cur_volume;
//    }
//    
//    switch (msg_id)
//    {
//    case MSG_APP_LEFT:
//        if (MMISET_VOL_ZERO < cur_volume)
//        {
//            cur_volume--;
//            is_volume_change = TRUE;
//        }
//        break;
//        
//    case MSG_APP_RIGHT:
//        if (MMISET_VOL_SEVEN > cur_volume)
//        {
//            cur_volume++;
//            is_volume_change = TRUE;
//        }
//        break;
//        
//    default:
//        SCI_TRACE_LOW("MMISET_TuneUpVolume:msg_id %d is error!",msg_id);
//        break;
//    }
//    
//    //set current volume
//    if (MMISET_RING_VOLUME_KEY == volume_type)
//    {        
//        g_cur_key_volume = cur_volume;     
//    }
//    else
//    {       
//        g_cur_volume = cur_volume;        
//    }
//    
//    if (is_volume_change)
//    {
//        //display picture
//        MMIAPISET_DisplayVolumePicture(cur_volume,0);
//        
//        //play ring
//        switch (volume_type)
//        {
//        case MMISET_RING_VOLUME_CALL:
//            MMIAPISET_PlayCallRingByVol(cur_volume,1,1,MMISET_RING_TYPE_CALL,PNULL);
//            break;
//            
//        case MMISET_RING_VOLUME_MSG:
//            PlayMsgRingByVol(cur_volume,1,MMISET_RING_TYPE_MSG,PNULL);
//            break;
//            
//        case MMISET_RING_VOLUME_COMM:
//            break;
//            
//        case MMISET_RING_VOLUME_KEY:
//            PlayKeyToneByVol(cur_volume,1,MMISET_RING_TYPE_KEY,PNULL);
//            break;
//            
//        case MMISET_RING_VOLUME_POWER:
//            PlayPowerRingByVol(cur_volume,1,MMISET_RING_TYPE_POWER,PNULL);
//            break;
//            
//        default:
//            SCI_TRACE_LOW("MMISET_TuneUpVolume:volume_type %d is error!",volume_type);
//            break;
//        }
//    }
//}
/*****************************************************************************/
//  Description : get call ring info including file name and ring type
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC MMISET_CALL_RING_T MMIAPISET_GetCallRingInfo(MN_DUAL_SYS_E dual_sys,uint8 mode_id)
{
    MMISET_CALL_RING_T  call_ring_info = {MMISET_CALL_RING_FIXED, 0, 0};
    call_ring_info = MMIAPIENVSET_GetCallRingInfo(dual_sys,mode_id);
    //SCI_TRACE_LOW:"MMIAPISET_GetCallRingInfo enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_2084_112_2_18_2_51_43_286,(uint8*)"");
    //if call ring is more ring then judge if the file is exited       
    
    if (MMISET_CALL_RING_MORE_RING == call_ring_info.call_ring_type)
    {
        if(!MMIAPIFMM_IsFileExist( call_ring_info.more_ring_info.name_wstr,call_ring_info.more_ring_info.name_wstr_len))
        {
            //the file does not exit, change nv info
            call_ring_info.call_ring_type = MMISET_CALL_RING_FIXED;
            call_ring_info.fix_ring_index = MMISET_RING_DEFAULT_ID;
            SCI_MEMSET(&call_ring_info.more_ring_info,0,sizeof(MMISET_CALL_MORE_RING_T));
            MMIAPIENVSET_SetFixedCallRingId(dual_sys,MMISET_RING_DEFAULT_ID,mode_id);
            //SCI_TRACE_LOW:"MMIAPISET_GetCallRingInfo: the more call ring is not exited"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_2096_112_2_18_2_51_43_287,(uint8*)"");
            
            return (call_ring_info);
        }
    }
    //SCI_TRACE_LOW:"MMIAPISET_GetCallRingInfo out"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_2101_112_2_18_2_51_43_288,(uint8*)"");
    return call_ring_info;    
}


/*****************************************************************************/
//  Description : get msg ring info including ring class and index and ring colum
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC MMISET_MSG_RING_T  MMIAPISET_GetMsgRingInfo(MN_DUAL_SYS_E dual_sys, uint8 mode_id )
{
    MMISET_MSG_RING_T       msg_ring_info   =   {MMISET_MSG_RING_FIXED, 0, 0};
    msg_ring_info = MMIAPIENVSET_GetMsgRingInfo(dual_sys,mode_id);   
    
    //if msg ring is more ring then judge if the file is exited
    if (MMISET_MSG_RING_MORE_RING == msg_ring_info.msg_ring_type)
    {
        if (!MMIAPIFMM_IsFileExist( msg_ring_info.more_ring_info.name_wstr,msg_ring_info.more_ring_info.name_wstr_len))
        {
            msg_ring_info.msg_ring_type = MMISET_MSG_RING_FIXED;
            msg_ring_info.fix_ring_index = MMISET_MSG_RING_DEFAULT_ID;
            SCI_MEMSET(&msg_ring_info.more_ring_info, 0 , sizeof(MMISET_CALL_MORE_RING_T));
            MMIAPIENVSET_SetFixedMsgRingId(dual_sys,MMISET_MSG_RING_DEFAULT_ID,mode_id);
           // MMINV_WRITE(MMINV_SET_SIM2_MSG_RING_INFO,&msg_ring_info);
        }
    }    

    return (msg_ring_info);
}

/*****************************************************************************/
//  Description : 将所有信息铃声的种类放入列表中，包括固定铃声和更多音乐
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_AppendMsgRingClassItem(
                                          MN_DUAL_SYS_E dual_sys,
                                          MMI_CTRL_ID_T ctrl_id
                                          )
{
    
    uint16                      cur_selection   =   0;
    MMISET_MSG_RING_T           msg_ring_info   =   {MMISET_MSG_RING_FIXED, 0, 0};
    
    //append item
    MMIAPISET_AppendListItemByTextId( TXT_RING_FIXED, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_COMMON_MORE_RING, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    //get msg ring class
    msg_ring_info = MMIAPISET_GetMsgRingInfo(dual_sys,MMIENVSET_GetCurModeId());
    cur_selection = msg_ring_info.msg_ring_type;
    
    GUILIST_SetSelectedItem( ctrl_id, cur_selection, TRUE );
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}
/*****************************************************************************/
//  Description : 将所有来电铃声的种类放入列表中，包括固定铃声和更多音乐
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_AppendCallRingCalssItem(MN_DUAL_SYS_E dual_sys, MMI_CTRL_ID_T  ctrl_id )
{
    
    uint16                      cur_selection   =   0;
    MMISET_CALL_RING_T          ring_info       =   {MMISET_CALL_RING_FIXED, 0, 0};
    
    //append item
    MMIAPISET_AppendListItemByTextId( TXT_RING_FIXED, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_COMMON_MORE_RING, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    //get call ring
    ring_info = MMIAPISET_GetCallRingInfo(dual_sys,MMIENVSET_GetCurModeId());
    
    cur_selection = ring_info.call_ring_type;
    
    //set selected item
    GUILIST_SetSelectedItem( ctrl_id, cur_selection, TRUE );
    
    //set current item
    GUILIST_SetCurItemIndex( ctrl_id,cur_selection );
}

/*****************************************************************************/
//  Description : 根据list当前阴影处于的铃声来播放来电铃声和消息铃声
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_PlayRingByListItem(
                                      MMI_CTRL_ID_T         ctrl_id,
                                      MMISET_ALL_RING_TYPE_E   ring_type
                                      )
{
    uint8                       ring_id     =   0;
    uint16                      cur_item    =   0;
    uint8                       ring_vol    =   0;
    
    //get the current item
    cur_item = GUILIST_GetCurItemIndex(ctrl_id);
    
    ring_id = (uint8)(cur_item + 1);
    
    ring_vol = MMIAPIENVSET_GetCurModeRingVol(ring_type);
    
    //if volume is 0, play the music with volume 3
    if (0 == ring_vol)
    {
        ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
    }
    
    //play ring
    MMIAPISET_PlayCallRingByVol(ring_vol,ring_id, 1, ring_type, PNULL);
}

/*****************************************************************************/
//  Description : 设置完更多信息铃声后，将信息存入nv
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetMoreMsgRingInfo(
                                      MN_DUAL_SYS_E dual_sys,
                                      MMI_CTRL_ID_T   ctrl_id,
                                      MMISET_CALL_MORE_RING_T ring_info
                                      )
{
    BOOLEAN  is_success = TRUE;

    is_success = MMIAPIENVSET_SetMoreMsgRingInfo(dual_sys,ring_info,MMIENVSET_GetCurModeId());
       
    //append set msg ring list
    MMISET_AppendMsgRingClassItem(dual_sys, ctrl_id);   
    return is_success;
}
/*****************************************************************************/
//  Description : 设置完更多来电铃声后，将信息存入nv
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
//PUBLIC void MMIAPISET_SetMoreCallRingInfo(
//                                          MN_DUAL_SYS_E dual_sys,
//                                          MMISET_CALL_MORE_RING_T ring_info
//                                          )
//{
////    MMISET_CALL_RING_T      call_ring_info = {MMISET_CALL_RING_FIXED, 0, 0};
////    
////    //write the selected call ring id into NV
////    call_ring_info.call_ring_type = MMISET_CALL_RING_MORE_RING;
////    SCI_ASSERT(ring_info.name_len < MMISET_CALL_RING_NAME_MAX_LEN);
////    SCI_MEMCPY(call_ring_info.more_ring_info.name,ring_info.name,ring_info.name_len);
////    call_ring_info.more_ring_info.name_len = ring_info.name_len;
//    
//    if (MN_DUAL_SYS_1 == dual_sys)
//    {
//       //  MMINV_WRITE(MMINV_SET_CALL_RING_INFO,&call_ring_info);
//        MMIAPIENVSET_SetMoreCallRingInfo(dual_sys,ring_info);
//    }
//#ifdef MMI_MULTI_SIM_SYS_DUAL
//    else
//    {
//        // MMINV_WRITE(MMINV_SET_SIM2_CALL_RING_INFO,&call_ring_info);
//        MMIAPIENVSET_SetMoreCallRingInfo(dual_sys,ring_info);
//    }
//#endif     
//    // SCI_TRACE_LOW("MMIAPISET_SetMoreCallRingInfo: file name length is %d",call_ring_info.more_ring_info.name_len); 
//}
/*****************************************************************************/
//  Description : 设置完固定铃声后，将ID信息存如nv
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_SetFixedCallRingId(
//                                      MN_DUAL_SYS_E dual_sys,
//                                      uint16 ring_id
//                                      )
//{
//    // MMISET_CALL_RING_T      call_ring_info = {MMISET_CALL_RING_FIXED, 0, 0};
//    
//    //write the selected call ring id into NV
////    call_ring_info.call_ring_type = MMISET_CALL_RING_FIXED;
////    call_ring_info.fix_ring_index = ring_id;
////    SCI_MEMSET(&call_ring_info.more_ring_info, 0 ,sizeof(MMISET_CALL_MORE_RING_T));
//    if (dual_sys == MN_DUAL_SYS_1)
//    {
//        // MMINV_WRITE(MMINV_SET_CALL_RING_INFO, &call_ring_info);
//        MMIAPIENVSET_SetFixedCallRingId(dual_sys,ring_id);
//    }
//    else
//    {
//#ifndef MMI_MULTI_SIM_SYS_DUAL
//        SCI_PASSERT(0,("MMISET_SetFixedCallRingId dual_sys = %d",dual_sys));
//#else
////        MMINV_WRITE(MMINV_SET_SIM2_CALL_RING_INFO, &call_ring_info);
//        MMIAPIENVSET_SetFixedCallRingId(dual_sys,ring_id);
// #endif
//    }
//}

/*****************************************************************************/
//  Description : 根据radio list来设置来消息铃声
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetMsgRingByRadioItem(
                                         MN_DUAL_SYS_E  dual_sys,
                                         MMI_CTRL_ID_T  ctrl_id
                                         )
{
    uint8       ring_id         =   0;
    uint16      cur_selection   =   0;
    
    //get the selected item
    cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
    
    ring_id = (uint8)(cur_selection + 1);
    
   // SetFixedMsgRingInfo(dual_sys, ring_id);
    MMIAPIENVSET_SetFixedMsgRingId(dual_sys,ring_id,MMIENVSET_GetCurModeId());
}

/*****************************************************************************/
//  Description : get the info about fixed ring(call,msg,key,power,flip,alarm,
//                battery,dc,delete warning)of the resource
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void GetRingInfo(
                       uint16                    ring_id,           //in: the ring id
                       MMISET_ALL_RING_TYPE_E    ring_type,         //in: the type of all ring
                       MMIAUD_RING_DATA_INFO_T   *ring_info_ptr  //out: the info of all ring
                       )
{
    if(PNULL == ring_info_ptr)
    {
        //SCI_TRACE_LOW:"mmisetting_assert GetRingInfo"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_2327_112_2_18_2_51_43_289,(uint8*)"");
        return;
    }     
    // ring_id是从1开始的，所以获取数据时都需要减去1
    switch (ring_type)
    {
        case MMISET_RING_TYPE_CALL:        

            MMI_GetRingInfo(
                (ring_id + MMI_MIDI_RING_START_ID - 1),
                ring_info_ptr);

            break;
        case MMISET_RING_TYPE_MSG:     

            MMI_GetRingInfo(
                    (ring_id + MMI_SMS_RING_START_ID - 1),
                    ring_info_ptr);

            break;
            
        case MMISET_RING_TYPE_KEY: 
#ifdef KEY_RING_THROUGH_RECEIVER
            MMI_GetRingInfo(
                (ring_id + MMI_KEY_RING_LOUND_START_ID - 1),
                ring_info_ptr);
#else
            MMI_GetRingInfo(        
                (ring_id + MMI_KEY_RING_START_ID - 1),
                ring_info_ptr);
#endif
            break;
        case MMISET_RING_TYPE_TKEY:
#ifdef TP_RING_THROUGH_RECEIVER
            MMI_GetRingInfo(
#ifndef MMI_LOW_MEMORY_RING        
                (MMI_KEY_RING_LOUND_START_ID +1),
#else
                MMI_KEY_RING_LOUND_START_ID,
#endif
                ring_info_ptr);
#else
            MMI_GetRingInfo(
                (MMI_KEY_RING_START_ID +1),
                ring_info_ptr);
#endif
            ring_info_ptr->type = (MMISRVAUD_RING_FMT_E)MMISRVAUD_RING_FMT_WAVE;/*lint !e64*/
            break;    
            
        case MMISET_RING_TYPE_POWER:               
            MMI_GetRingInfo(
                (ring_id + MMI_POWER_RING_START_ID - 1),
                ring_info_ptr);
            break;
#ifdef CMCC_UI_STYLE
        case MMISET_RING_TYPE_POWER_OFF:
            MMI_GetRingInfo(
                (ring_id + MMI_POWER_RING_OFF_ID - 1),
                ring_info_ptr);
            break;
#endif

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.       
        case MMISET_RING_TYPE_FLIP:
        case MMISET_RING_TYPE_FLIP_DOWN:     
            MMI_GetRingInfo(
                (ring_id + MMI_FLIP_RING_START_ID - 1),
                ring_info_ptr);
            
            ring_info_ptr->type = (MMISRVAUD_RING_FMT_E)MMISRVAUD_RING_FMT_WAVE;/*lint !e64*/
            break;
#endif
            
#ifdef LCD_SLIDE_SUPPORT		
        case MMISET_RING_TYPE_SLIDE_ON:
        case MMISET_RING_TYPE_SLIDE_OFF:     
            MMI_GetRingInfo(
                (ring_id + MMI_SLIDE_RING_START_ID - 1),
                ring_info_ptr);
            
            ring_info_ptr->type = (MMISRVAUD_RING_FMT_E)MMISRVAUD_RING_FMT_WAVE;/*lint !e64*/
            break;
#endif	
            
        case MMISET_RING_TYPE_ALARM:     
            MMI_GetRingInfo(
                (ring_id + MMI_MIDI_RING_START_ID - 1),
                ring_info_ptr);
            break;

#ifdef ASP_SUPPORT
        case MMISET_RING_TYPE_ASP:
            MMI_GetRingInfo(
                (ring_id  + MMI_ASP_RING_START_ID - 1),
                ring_info_ptr);        
            
            ring_info_ptr->type = (MMISRVAUD_RING_FMT_E)MMISRVAUD_RING_FMT_WAVE;
            break;
#endif 

        case MMISET_RING_TYPE_DC:         
            MMI_GetRingInfo(
                (ring_id + MMI_DC_RING_START_ID - 1),
                ring_info_ptr);        
            
            ring_info_ptr->type = (MMISRVAUD_RING_FMT_E)MMISRVAUD_RING_FMT_WAVE;/*lint !e64*/
            break;

        case MMISET_RING_TYPE_DL:
            MMI_GetRingInfo(
                (ring_id + MMI_DL_RING_START_ID - 1),
                ring_info_ptr);        
            ring_info_ptr->type = (MMISRVAUD_RING_FMT_E)MMISRVAUD_RING_FMT_WAVE;/*lint !e64*/
            break;

        case MMISET_RING_TYPE_DC_COUNT:     
            MMI_GetRingInfo(
                (ring_id + MMI_DC_RING_COUNT_ID - 1),
                ring_info_ptr);        
            
            ring_info_ptr->type = (MMISRVAUD_RING_FMT_E)MMISRVAUD_RING_FMT_WAVE;/*lint !e64*/
            break;

        case MMISET_RING_TYPE_DC_FINAL_COUNT:     
            MMI_GetRingInfo(
                (ring_id + MMI_DC_RING_FINAL_COUNT_ID - 1),
                ring_info_ptr);        
            
            ring_info_ptr->type = (MMISRVAUD_RING_FMT_E)MMISRVAUD_RING_FMT_WAVE;/*lint !e64*/
            break;  

#ifdef MMI_INDIAN_SOS_SUPPORT
        case MMISET_RING_TYPE_SOS:
            MMI_GetRingInfo(
                (ring_id + MMI_SET_SOS_ALERT_RING_ID - 1),
                ring_info_ptr);        
            
            //ring_info_ptr->type = (MMISRVAUD_RING_FMT_E)MMISRVAUD_RING_FMT_WAVE;/*lint !e64*/		
        break;
#endif

        case MMISET_RING_TYPE_BAT:
            
            MMI_GetRingInfo(
                (ring_id + MMI_BAT_RING_START_ID - 1),
                ring_info_ptr);

            break;

        case MMISET_RING_TYPE_CHARGE:
        {  
            MMI_GetRingInfo(
                (ring_id + MMI_CHARGE_RING_START_ID - 1),
                ring_info_ptr);
            break;
        }

#ifdef MMI_AZAN_SUPPORT
        case MMISET_RING_TYPE_AZAN:
            MMI_GetRingInfo((ring_id + MMI_AZAN_RING_START_ID - 1), ring_info_ptr);
            break;
#endif
#ifdef MMI_KEY_LOCK_SUPPORT
        case MMISET_RING_TYPE_KEY_LOCK:
            MMI_GetRingInfo(
                (ring_id + MMI_KEY_LOCK_RING_START_ID - 1),
                ring_info_ptr);
            break;
#endif

#if defined(MMI_OCR_AUDIO_PLAY_TEST_LONG)// wuxx 20210805
                        case MMISET_RING_TYPE_OCR_AUDIO_TEST:
                        {
                            //if(ocr_temp)
                            {
                                MMI_GetRingInfo(
                                    (ring_id + MMI_OCR_AUDIO_TEST_START_ID - 1),
                                    ring_info_ptr);
                            }
                            ring_info_ptr->type = (MMISRVAUD_RING_FMT_E)MMISRVAUD_RING_FMT_MP3;
                        }
                            break;
#endif

        case MMISET_RING_TYPE_OTHER:
            MMI_GetRingInfo(
                (ring_id + MMI_MIDI_RING_START_ID - 1),
                ring_info_ptr); 
            break;

#ifdef MMI_ETWS_SUPPORT
        case MMISET_RING_TYPE_ETWS:
            MMI_GetRingInfo(
                (ring_id + MMI_ETWS_RING_START_ID - 1),
                ring_info_ptr);
            break;

        case MMISET_RING_TYPE_ETWS_PWS:
            MMI_GetRingInfo(
                (ring_id + MMI_ETWS_PWS_RING_START_ID - 1),
                ring_info_ptr);
            break;
#endif
        default:
            //SCI_TRACE_LOW:"GetRingInfo:ring_type is error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_2495_112_2_18_2_51_44_290,(uint8*)"");
            break;
    }
}

/*****************************************************************************/
//  Description : 添加固定铃声铃声list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
#if 0
PUBLIC void MMISET_AppendFixedCallRingItem(
                                           MN_DUAL_SYS_E dual_sys,
                                           MMI_CTRL_ID_T    ctrl_id,
                                           MMISET_SELECT_WIN_PARAM_T     *win_param_ptr
                                           )
{
    uint8                       i = 0,ring_id = 0,ring_vol = 0;
    uint16                      cur_selection   =   0;
    MMISET_CALL_RING_T          ring_info       =   {MMISET_CALL_RING_FIXED, 0, 0};
    
    //add items to list box
    for (i = 0; i < MMISET_RING_FIXED_DEFAUT_NUM; i++)
    {   
        switch(i)
        {
        case 0:
            MMIAPISET_AppendListItemByTextId( TXT_COMMON_RING_RING1, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
            break;
        case 1:
            MMIAPISET_AppendListItemByTextId( TXT_COMMON_RING_RING2, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
            break;
        case 2:
            MMIAPISET_AppendListItemByTextId( TXT_RING3, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
            break;
        case 3:
            MMIAPISET_AppendListItemByTextId( TXT_RING4, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
            break;
        case 4:
            MMIAPISET_AppendListItemByTextId( TXT_RING5, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
            break;
        case 5:
            MMIAPISET_AppendListItemByTextId( TXT_RING6, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
            break;
        case 6:
            MMIAPISET_AppendListItemByTextId( TXT_RING7, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
            break;
        case 7:
            MMIAPISET_AppendListItemByTextId( TXT_RING8, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
            break;      
        default:
            break;
        }
    }
    
    if ( 0 != win_param_ptr->win_id)
    {
        if (0 != win_param_ptr->selected_ring_id)
        {
            cur_selection = win_param_ptr->selected_ring_id - 1;
        } 
        else
        {
            cur_selection = 0;
        }
        GUILIST_SetSelectedItem( ctrl_id, cur_selection, TRUE );
        //set current item
        GUILIST_SetCurItemIndex( ctrl_id, cur_selection );  
    }
    else
    {
        //get selected ring
        ring_info = MMIAPISET_GetCallRingInfo(dual_sys,MMIENVSET_GetCurModeId());
        
        if ( MMISET_CALL_RING_FIXED == ring_info.call_ring_type)
        {
            cur_selection = ring_info.fix_ring_index - 1;
            
            GUILIST_SetSelectedItem( ctrl_id, cur_selection, TRUE );
            //set current item
            GUILIST_SetCurItemIndex( ctrl_id, cur_selection );      
        }
        else
        {
            cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        }       
    }
    
    ring_id = (uint8)(cur_selection + 1);
    
    //get ring volume
    ring_vol = MMIAPIENVSET_GetCurModeRingVol(MMISET_RING_TYPE_CALL);
    
    //if volume is 0, play the music with volume 3
    if (0 == ring_vol)
    {
        ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
    }
    
    //play ring
    MMIAPISET_PlayCallRingByVol(ring_vol,ring_id, 1, MMISET_RING_TYPE_CALL, PNULL);
    
}
#else
PUBLIC void MMISET_AppendFixedCallRingItem(
                                           MN_DUAL_SYS_E dual_sys,
                                           MMI_CTRL_ID_T    ctrl_id,
                                           MMISET_SELECT_WIN_PARAM_T     *win_param_ptr
                                           )
{
    // uint8                       i = 0;
	uint8	ring_id = 0,ring_vol = 0;
    uint16                      cur_selection   =   0;
    MMISET_CALL_RING_T          ring_info       =   {MMISET_CALL_RING_FIXED, 0, 0};

    GUILIST_RemoveAllItems(ctrl_id);

#if 0
    MMI_STRING_T        string = {0};
    wchar wbuff[LIST_STR_LEN + 4]={0};
    char buff[3 + 1]={0};

    GUILIST_RemoveAllItems(ctrl_id);
    
    MMIRES_GetText( TXT_RING, 0, &string );
    /* 容错*/
    if(string.wstr_len > LIST_STR_LEN)
    {
        string.wstr_len = LIST_STR_LEN;
    }
    MMIAPICOM_Wstrncpy(wbuff, string.wstr_ptr, string.wstr_len);
    
    //add items to list box
    for (i = 0; i < MMISET_RING_FIXED_DEFAUT_NUM; i++)
    {   
        sprintf(buff, "%d", i + 1);
        MMIAPICOM_StrToWstr((uint8 *)buff, wbuff + string.wstr_len);
        MMIAPICOM_Wstrcpy(s_ring_list[i], wbuff);
        MMIAPISET_AppendListItemByTextBuffer(
                                                    s_ring_list[i],
                                                    string.wstr_len + strlen(buff),
                                                    0,
                                                    TXT_COMMON_OK, 
                                                    TXT_NULL, 
                                                    STXT_RETURN, 
                                                    ctrl_id, 
                                                    GUIITEM_STYLE_ONE_LINE_RADIO
                                                    );
    }
#endif    
    MMIAPISET_AppendFixedItems(
                                    TXT_RING,
                                    MMISET_RING_FIXED_DEFAUT_NUM,
                                    TXT_COMMON_OK,
                                    TXT_NULL,
                                    STXT_RETURN,
                                    ctrl_id,
                                    GUIITEM_STYLE_ONE_LINE_RADIO
                                    );

    if ( 0 != win_param_ptr->win_id)
    {
        if (0 != win_param_ptr->selected_ring_id)
        {
            cur_selection = win_param_ptr->selected_ring_id - 1;
        } 
        else
        {
            cur_selection = 0;
        }
        GUILIST_SetSelectedItem( ctrl_id, cur_selection, TRUE );
        //set current item
        GUILIST_SetCurItemIndex( ctrl_id, cur_selection );  
    }
    else
    {
        //get selected ring
        ring_info = MMIAPISET_GetCallRingInfo(dual_sys,MMIENVSET_GetCurModeId());
        
        if ( MMISET_CALL_RING_FIXED == ring_info.call_ring_type)
        {
            cur_selection = ring_info.fix_ring_index - 1;
            
            GUILIST_SetSelectedItem( ctrl_id, cur_selection, TRUE );
            //set current item
            GUILIST_SetCurItemIndex( ctrl_id, cur_selection );      
        }
        else
        {
            cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        }       
    }
    
    ring_id = (uint8)(cur_selection + 1);
    
    //get ring volume
    ring_vol = MMIAPIENVSET_GetCurModeRingVol(MMISET_RING_TYPE_CALL);
    
    //if volume is 0, play the music with volume 3
    if (0 == ring_vol)
    {
        ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
    }
    
    //play ring
    MMIAPISET_PlayCallRingByVol(ring_vol,ring_id, 1, MMISET_RING_TYPE_CALL, PNULL);
    
}

/*****************************************************************************/
//  Description : 获取铃声的名字
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISET_GetRingName(uint16 idx, wchar *ring_name, uint32 len)
{
    uint16 id = idx;
    MMI_STRING_T        string = {0};
    wchar wbuff[30 + 4]={0};
    char buff[5 + 1]={0};   

    if(PNULL == ring_name)
        return 0;
    
    if(len <= 5)
        return 0;
    
    if(id > MMISET_RING_FIXED_DEFAUT_NUM || 0 == id )
    {
        id = 1;
    }
    
    MMIRES_GetText( TXT_RING, 0, &string );

    if(string.wstr_len > len - 5)
    {
        string.wstr_len = len - 5;
    }
    MMIAPICOM_Wstrncpy(wbuff, string.wstr_ptr, string.wstr_len);
    
    sprintf(buff, "%d", id);
    MMIAPICOM_StrToWstr((uint8 *)buff, wbuff + string.wstr_len);
    MMIAPICOM_Wstrncpy(ring_name, wbuff, string.wstr_len + strlen(buff));

    return string.wstr_len + strlen(buff);
}
#endif
/*****************************************************************************/
//  Description : play ring by ring data and data length
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
#ifdef MMISRV_AUDIO_SUPPORT
PUBLIC void MMIAPISET_PlayRingByPtr(
                                    MMISET_ALL_RING_TYPE_E      ring_type,
                                    uint8                       ring_vol,   // the ring volume
                                    uint8                       *ring_data_ptr,
                                    uint32                      ring_data_len,
                                    uint32                      play_times,
                                    MMISRVAUD_RING_FMT_E audio_type,
                                    MMISET_RING_CALLBACK_PFUNC  callback_func
                                    )
{        
    //MMIAUD_RING_DATA_INFO_T     ring_data = {MMISRVAUD_RING_FMT_MIDI, 0, NULL};/*lint !e64*/
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;

    if (
#ifndef MMI_MMS_ADJUST_VOLUME_SUPPORT        
        MMISET_VOL_ZERO == ring_vol || 
#endif        
        PNULL == ring_data_ptr || !MMIAPISET_IsPermitPlayRing(ring_type))
    {
        if(callback_func != PNULL)
        {
            callback_func(MMISRVAUD_REPORT_RESULT_ERROR, PNULL);
        }
    }

    MMIAPISET_StopAppRing();

    req.is_auto_free = TRUE;
    req.notify = HandleAudioCallBack;
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.volume = ring_vol;
    audio_srv.play_times = play_times;
    audio_srv.info.ring_buf.type = MMISRVAUD_TYPE_RING_BUF;
    audio_srv.info.ring_buf.fmt = audio_type;
    audio_srv.info.ring_buf.data = ring_data_ptr;
    audio_srv.info.ring_buf.data_len = ring_data_len;
    if(GetAudioMode())
    {
        audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARFREE;
    }
    else
    {
        audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE;
    }
    
    handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    if(handle > 0)
    {
        s_cur_app_handle = handle;
        if(MMISRVAUD_Play(s_cur_app_handle, 0))
        {
            MMIAPISET_SetCurRingType(ring_type);
            s_ring_callback_func = callback_func;    
        }
        else
        {
            MMISRVMGR_Free(s_cur_app_handle);
            s_cur_app_handle = 0;
            if(callback_func != PNULL)
            {
                callback_func(MMISRVAUD_REPORT_RESULT_ERROR, PNULL);
            }
        }
    }
    else
    {
        if(callback_func != PNULL)
        {
            callback_func(MMISRVAUD_REPORT_RESULT_ERROR, PNULL);
        }                
    }
}
#else
PUBLIC void MMIAPISET_PlayRingByPtr(
                                    MMISET_ALL_RING_TYPE_E      ring_type,
                                    uint8                       ring_vol,   // the ring volume
                                    uint8                       *ring_data_ptr,
                                    uint32                      ring_data_len,
                                    uint32                      play_times,
                                    MMIAUDIO_TYPE_E             audio_type,
                                    MMISET_RING_CALLBACK_PFUNC  callback_func
                                    )
{
    // Appended by zgy, cr97443
    //AUDIO_DEVICE_MODE_TYPE_E          old_mode = AUDIO_DEVICE_MODE_HANDHOLD;
    // AUDIO_DEVICE_MODE_TYPE_E          mode  =   AUDIO_DEVICE_MODE_HANDHOLD;
    
    //SCI_ASSERT(PNULL != ring_data_ptr);
    
    if (MMIAPISET_IsPermitPlayRing(ring_type))
    {
        //停掉所有铃声和振动
        MMIAPISET_StopAllRing(TRUE);
        MMIAPISET_SetCurRingType(ring_type);
    }
    else
    {
        return;
    }
    
    s_ring_callback_func = callback_func;
#ifndef MMI_MMS_ADJUST_VOLUME_SUPPORT
    if (MMISET_VOL_ZERO != ring_vol)
#endif        
    {
        if (PNULL != ring_data_ptr)
        {
            MMIAPISET_InitAudioDevice(ring_vol);
            MMIAPISET_InitEqMode();
            MMIAPISET_InitPlayRate();
            
            if(MMIAUDIO_CreateAudioBufHandle(
                    FALSE,
                    s_cur_audio_htype,
                    audio_type,
                    ring_data_ptr,
                    ring_data_len
                    ))
            {
                if(!MMIAUDIO_AudioPlay(
                        s_cur_audio_htype,
                        0,
                        play_times,
                        callback_func//CallBackFunction
                        ))
                {
                    CallBackFunction(MMIAUDIO_RLT_ERROR, PNULL);
                    MMIAUDIO_CloseAudioHandle(s_cur_audio_htype);
                }
            }
            else
            {
                CallBackFunction(MMIAUDIO_RLT_NOT_SUPPORT, PNULL);
            }
        }
        else
        {
            //提示文件格式错误
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
        }
    }
    else//声音为0不播放铃声，但是需要调用回调函数
    {
        CallBackFunction(MMIAUDIO_RLT_SUCCESS,PNULL);
    }
}
#endif

#ifdef MMISRV_AUDIO_SUPPORT
/*****************************************************************************/
//  Description : play a music file.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_PlayMusicFile(
                        MMISET_ALL_RING_TYPE_E         ring_type,
                        BOOLEAN                        is_a2dp,
                        BOOLEAN                        is_consume,
                        const wchar                    *full_path_ptr,
                        uint16                         full_path_len,
                        uint32                         play_times,
                        uint32                         play_offset,
                        uint32                         vol,
                        MMISET_RING_CALLBACK_PFUNC    call_back
                        )
{
    BOOLEAN             result = FALSE;
    MMISRVAUD_RING_FMT_E     music_type = 0;
    //AUDIO_FORMATINFO_T  audio_info = {0};
    uint16              file_name[MMIFILE_FILE_NAME_MAX_LEN+2] = {0};
    uint16              file_name_len = 0;

#ifdef DRM_SUPPORT
    BOOLEAN                 is_audio_drm_file = FALSE;
    BOOLEAN                 is_drm_valid = FALSE;
    MMISRVAUD_RING_FMT_E         drm_audio_type = MMISRVAUD_RING_FMT_MAX;
#endif


    //MMIAUD_RING_DATA_INFO_T     ring_data = {MMISRVAUD_RING_FMT_MIDI, 0, NULL};/*lint !e64*/
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;

    if((PNULL == full_path_ptr) || (0 == full_path_len) || !MMIAPISET_IsPermitPlayRing(ring_type))
    {
        //SCI_TRACE_LOW:"MMIAPISET_PlayMusicFile param error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_2927_112_2_18_2_51_45_291,(uint8*)"");

        if(PNULL != call_back)
        {
            call_back(MMISRVAUD_REPORT_RESULT_NOT_SUPPORT, PNULL);
        }        
        return FALSE;
    }

    MMIAPISET_StopAppRing();

    MMIAPIFMM_SplitFullPath(full_path_ptr, full_path_len, 
                          PNULL, PNULL, PNULL, PNULL,
                          file_name, &file_name_len);
    
    music_type = MMIAPICOM_GetMusicType( file_name, file_name_len );

#ifdef DRM_SUPPORT
    if(MMIAPIMP3_IsDrmFile(
            full_path_ptr, full_path_len,
            &drm_audio_type,&is_audio_drm_file,&is_drm_valid
            ))
    {
        if(is_audio_drm_file && is_drm_valid)
        {
            music_type = drm_audio_type;
        }
    }    
#endif
    
    req.is_auto_free = TRUE;
    req.notify = HandleAudioCallBack;
    req.pri = MMISRVAUD_PRI_NORMAL;
    audio_srv.duation = GetPlayTime(ring_type, full_path_ptr, full_path_len); 

    if (MMIENVSET_PREVIEW_PLAY_DEFAUT_TIMES == play_times)
    {
        audio_srv.play_times = MMIENVSET_PREVIEW_PLAY_RING_TIMES;
        audio_srv.duation = MIN(MMIENVSET_PREVIEW_PLAY_RING_DUATION,audio_srv.duation);
    }
    else
    {
        audio_srv.play_times = play_times;
    }
    audio_srv.volume = vol;
    audio_srv.info.ring_file.type = MMISRVAUD_TYPE_RING_FILE;
    audio_srv.info.ring_file.fmt = music_type;
    audio_srv.info.ring_file.name = full_path_ptr; /*lint !e605*/
    audio_srv.info.ring_file.name_len = full_path_len;
    if(GetAudioMode())
    {
        audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARFREE;
    }
    else
    {
        audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE;
    }

    s_ring_callback_func = call_back;
    handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    if (handle > 0)
    {
        s_cur_app_handle = handle;

        if(MMISRVAUD_Play(s_cur_app_handle, 0))
        {
            MMIAPISET_SetCurRingType(ring_type);
            s_ring_callback_func = call_back;    
            s_cur_app_handle = handle;
            #ifdef DRM_SUPPORT
            if(is_audio_drm_file && is_consume)
            {
                MMIAPIDRM_ConsumeRights(SFS_INVALID_HANDLE, full_path_ptr);
            }
            #endif
            
            result = TRUE;
        }
        else
        {
            MMISRVMGR_Free(s_cur_app_handle);
            s_cur_app_handle = 0;
            if(PNULL != call_back)
            {
                call_back(MMISRVAUD_REPORT_RESULT_ERROR, PNULL);
            }
        }
    }
    else
    {
        if(PNULL != call_back)
        {
            call_back(MMISRVAUD_REPORT_RESULT_NOT_SUPPORT, PNULL);
        }
    }
    
    //SCI_TRACE_LOW:"MMIAPISET_PlayMusicFile, ring_type= %d, play_times = %d, vol = %d, call_back = 0x%x,music_type = %d, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_3015_112_2_18_2_51_45_292,(uint8*)"dddddd", ring_type, play_times, vol, call_back,music_type,result);

    return result;
}
#endif
/*****************************************************************************/
//  Description : handle playing through receiver 
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
#ifdef MMISRV_AUDIO_SUPPORT
LOCAL BOOLEAN HandleReceiverMode(
                    MMISET_ALL_RING_TYPE_E  ring_type,
                    uint8                   volume
                    )
{
    return TRUE;
}
#else
LOCAL BOOLEAN HandleReceiverMode(
                    MMISET_ALL_RING_TYPE_E  ring_type,
                    uint8                   volume
                    )
{
    BOOLEAN is_through_receiver = FALSE;
    AUDIO_DEVICE_MODE_TYPE_E  mode = AUDIO_DEVICE_MODE_HANDFREE;

    switch(ring_type)
    {
    case MMISET_RING_TYPE_KEY:
        #ifdef KEY_RING_THROUGH_RECEIVER
        is_through_receiver = TRUE;
        #endif
        break;
        
    case MMISET_RING_TYPE_TKEY:
        #ifdef TP_RING_THROUGH_RECEIVER
        is_through_receiver = TRUE;
        #endif
        break;
        
    default:
        break;
    }

    //SCI_TRACE_LOW:"[MMISET]:HandleReceiverMode, ring_type = %d, volume = %d, is_through_receiver = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_3178_112_2_18_2_51_45_295,(uint8*)"ddd",ring_type,volume,is_through_receiver);

    if(is_through_receiver)
    {
        mode   = AUDIO_DEVICE_MODE_HANDHOLD;
        volume = MMISET_VOL_MAX;
        
        #ifndef WIN32
        if (GPIO_CheckHeadsetStatus()) 
        {
            mode = AUDIO_DEVICE_MODE_EARPHONE;
            volume = MMISET_VOL_ONE;
        }
        #endif
        
        {
            MMIAUDIO_SetMute(FALSE);
            MMIAUDIO_SetVolume(mode, volume);
        }
    }
    return is_through_receiver;
}
#endif

/*****************************************************************************/
//  Description : Is the current ring playing through receiver 
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
#ifdef MMISRV_AUDIO_SUPPORT
PUBLIC BOOLEAN MMIAPISET_IsThroughReceiver(
                    void
                    )
{
    //maybe not needed.
    return TRUE;
}
#else
PUBLIC BOOLEAN MMIAPISET_IsThroughReceiver(
                    void
                    )
{
    BOOLEAN is_through_receiver = FALSE;

    switch(s_cur_real_ring_type)
    {
    case MMISET_RING_TYPE_KEY:
        #ifdef KEY_RING_THROUGH_RECEIVER
        if(MMIAPISET_IsPlayingRingType(MMISET_RING_TYPE_KEY) ||
           MMIAPISET_IsPlayingRingType(MMISET_RING_TYPE_OTHER))
        {
            is_through_receiver = TRUE;
        }
        #endif
        break;
        
    case MMISET_RING_TYPE_TKEY:
        #ifdef TP_RING_THROUGH_RECEIVER
        if(MMIAPISET_IsPlayingRingType(MMISET_RING_TYPE_TKEY) ||
           MMIAPISET_IsPlayingRingType(MMISET_RING_TYPE_OTHER))
        {
            is_through_receiver = TRUE;
        }
        #endif
        break;
        
    default:
        break;
    }

    //SCI_TRACE_LOW:"[MMISET]:MMIAPISET_IsThroughReceiver, ring_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_3247_112_2_18_2_51_45_296,(uint8*)"d",MMIAPISET_GetCurRingType());

    return is_through_receiver;
}
#endif

/*****************************************************************************/
//  Description :Init audio device before play.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
#ifdef MMISRV_AUDIO_SUPPORT
PUBLIC void MMIAPISET_InitAudioDeviceExt(
                    MMISET_ALL_RING_TYPE_E  ring_type,
                    uint8                   volume
                    )
{
    if(s_cur_app_handle != 0)
    {
        MMISRVAUD_SetVolume(s_cur_app_handle, volume);
    }
}
#else
PUBLIC void MMIAPISET_InitAudioDeviceExt(
                    MMISET_ALL_RING_TYPE_E  ring_type,
                    uint8                   volume
                    )
{
    //SCI_TRACE_LOW:"[MMISET]: MMIAPISET_InitAudioDeviceExt: ring_type = %d,volume=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_3274_112_2_18_2_51_46_297,(uint8*)"dd",ring_type,volume);
#ifdef DPHONE_SUPPORT 
#else
    if(!HandleReceiverMode(ring_type,volume))
#endif
    {
        MMIAPISET_InitAudioDevice(volume);
    }
}
#endif

/*****************************************************************************/
//  Description :Init audio device before play.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
#ifdef MMISRV_AUDIO_SUPPORT
PUBLIC void MMIAPISET_InitBtAudioDevice(
                BOOLEAN is_bt_play,
                uint8   volume
                )
{
    //do nothing
}
#else
PUBLIC void MMIAPISET_InitBtAudioDevice(
                BOOLEAN is_bt_play,
                uint8   volume
                )
{
    AUDIO_DEVICE_MODE_TYPE_E  mode = AUDIO_DEVICE_MODE_HANDFREE;
    
    //SCI_TRACE_LOW:"[MMISET]: MMIAPISET_InitBtAudioDevice: is_bt_play = %d, volume=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_3305_112_2_18_2_51_46_298,(uint8*)"dd",is_bt_play, volume);

    MMIAPISET_GetDeviceMode(&mode);

    if(is_bt_play)
    {
        mode = AUDIO_DEVICE_MODE_EARPHONE;
    }
    
    if (0 == volume)
    {
        MMIAUDIO_SetCurDevMode(mode);
        MMIAUDIO_SetMute(TRUE);
    }
    else
    {
        MMIAUDIO_SetMute(FALSE);
        MMIAUDIO_SetVolume(mode, (uint32)volume);
    }
}
#endif

/*****************************************************************************/
//  Description :Init audio device before play.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
#ifdef MMISRV_AUDIO_SUPPORT
PUBLIC void MMIAPISET_InitAudioDevice(
                uint8  volume
                )
{
    MMISRVAUD_SetVolume(s_cur_app_handle, volume);
}
#else
PUBLIC void MMIAPISET_InitAudioDevice(
                uint8  volume
                )
{
    AUDIO_DEVICE_MODE_TYPE_E  mode = AUDIO_DEVICE_MODE_HANDFREE;

    //SCI_TRACE_LOW:"[MMISET]: MMIAPISET_InitAudioDevice: volume=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_3345_112_2_18_2_51_46_299,(uint8*)"d",volume);

    if (0 == volume)
    {
        MMIAPISET_GetDeviceMode(&mode);
        MMIAUDIO_SetCurDevMode(mode);
        MMIAUDIO_SetMute(TRUE);
    }
    else
    {
        MMIAUDIO_SetMute(FALSE);
        MMIAPISET_GetDeviceMode(&mode);
        MMIAUDIO_SetVolume(mode, (uint32)volume);
    }
}
#endif

/*****************************************************************************/
//  Description :Init audio EQ mode before play, to regular mode.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
#ifdef MMISRV_AUDIO_SUPPORT
PUBLIC void MMIAPISET_InitEqMode(
                void
                )
{
    //do nothing.
}
#else
PUBLIC void MMIAPISET_InitEqMode(
                void
                )
{
    #ifdef MP3_EQ_SUPPORT 
    //SCI_TRACE_LOW("[MMISET]: MMIAPISET_InitEqMode");
    //MMIAUDIO_SetEQMode(MMIAPIMP3_GetMp3EqMode());
    #endif
}
#endif

/*****************************************************************************/
//  Description :Init audio play rate before play, to MMIAUDIO_PLAY_RATE_NORMAL.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
#ifdef MMISRV_AUDIO_SUPPORT
PUBLIC void MMIAPISET_InitPlayRate(
                void
                )
{
    //do nothing.
}
#else
PUBLIC void MMIAPISET_InitPlayRate(
                void
                )
{
    MMIAUDIO_SetPlayRate(MMIAUDIO_PLAY_RATE_NORMAL);
}
#endif

/*****************************************************************************/
//  Description : if permit play ring in call
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsPermitPlayRingInCall(MMISET_ALL_RING_TYPE_E ring_type)
{
    BOOLEAN result = FALSE;

    if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        if (MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE))//已经建立通话,即通话中
        {
            switch (ring_type)
            {
            case MMISET_RING_TYPE_BAT:
            case MMISET_RING_TYPE_RECORDER_RECORD:
            case MMISET_RING_TYPE_CALL_IN_CALL:
            case MMISET_RING_TYPE_MSG_IN_CALL:
            case MMISET_RING_TYPE_MINHINT_IN_CALL:
            case MMISET_RING_TYPE_ALARM:
#ifdef MMI_ETWS_SUPPORT
            case MMISET_RING_TYPE_ETWS:
            case MMISET_RING_TYPE_ETWS_PWS:
#endif
                result = TRUE;
                break;
                
            default:
                result = FALSE;
                break;
            }
        }
        else//正在建立通话
        {
            switch (ring_type)
            {
            case MMISET_RING_TYPE_CALL:
            case MMISET_RING_TYPE_BAT:
            case MMISET_RING_TYPE_RECORDER_RECORD:
            case MMISET_RING_TYPE_CALL_IN_CALL:
            case MMISET_RING_TYPE_MSG_IN_CALL:
            case MMISET_RING_TYPE_MINHINT_IN_CALL:
#ifdef MMI_ETWS_SUPPORT
            case MMISET_RING_TYPE_ETWS:
            case MMISET_RING_TYPE_ETWS_PWS:
#endif
                result = TRUE;
                break;
                
            default:
                result = FALSE;
                break;
            }
        }
    }
    else
    {
        result = FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description : if permit play ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNormalPri(MMISET_ALL_RING_TYPE_E ring_type)
{
    BOOLEAN res = FALSE;
    switch(ring_type)
    {
    case MMISET_RING_TYPE_CALL:
    case MMISET_RING_TYPE_RECORDER_PLAY:
    case MMISET_RING_TYPE_RECORDER_RECORD:
    case MMISET_RING_TYPE_MSG:
    case MMISET_RING_TYPE_POWER:
    #ifdef CMCC_UI_STYLE
    case MMISET_RING_TYPE_POWER_OFF:
    #endif
    case MMISET_RING_TYPE_ALARM:
    case MMISET_RING_TYPE_OTHER:
    case MMISET_RING_TYPE_BAT:
    case MMISET_RING_TYPE_CHARGE:
    case MMISET_RING_TYPE_DC:
    case MMISET_RING_TYPE_DC_COUNT: 
    case MMISET_RING_TYPE_DC_FINAL_COUNT:        
#ifdef MMI_INDIAN_SOS_SUPPORT
    case MMISET_RING_TYPE_SOS:
#endif
#ifdef MMI_ETWS_SUPPORT
    case MMISET_RING_TYPE_ETWS:
    case MMISET_RING_TYPE_ETWS_PWS:
#endif
        res = TRUE;
        break;
    default:
        res = FALSE;
    }
    return res;
}

/*****************************************************************************/
//  Description : if permit play ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsPermitPlayRing(
                                          MMISET_ALL_RING_TYPE_E  ring_type  //in: 即将播放铃声的类型
                                          )
{
    BOOLEAN     result = FALSE;
#ifndef MMISRV_AUDIO_SUPPORT    
    MMIAUDIO_HTYPE audio_htype = MMIAUDIO_APP;
#endif

    if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        switch (ring_type)
        {
        case MMISET_RING_TYPE_MINHINT_IN_CALL:
            result = GetPermitMinhintRingIncall();
            break;
            
        case MMISET_RING_TYPE_CALL_IN_CALL:
            result = GetPermitCallRingIncall();
            break;
            
        case MMISET_RING_TYPE_MSG_IN_CALL:
            result = GetPermitMsgRingIncall();
            break;
            
        default:
            result = IsPermitPlayRingInCall(ring_type);
            break;
        }
    }
    else if(MMIAPIPHONE_GetPowerAnimMpeg4Flag())
    {
        result = FALSE;
    }
//#ifndef CARD_LOG
#if defined(ENG_SUPPORT) && !defined(CARD_LOG)
    else if(MMIAPIENG_GetIQDataStatus())
    {
        result = FALSE;
    }
#endif
#if 0 //def WRE_SUPPORT//del by leichi 20120417 bug:the alarm is dummy when wre is playing media
    else if(MMIWRE_IsMMPlaying(ring_type))
    {
        result = FALSE;
    }
#endif
    else
    {
        if (!s_is_vibrate)
        {
            if(g_is_playing_ring_type != MMISET_RING_TYPE_MAX)
            {
                if(IsNormalPri(g_is_playing_ring_type))
                {
                    if(IsNormalPri(ring_type))
                    {
                        result = TRUE;
                    }
                    else
                    {
                        result = FALSE;
                    }
                }
                else
                {
                    result = TRUE;
                }
                
            }
            else
            {
                result = TRUE;
            }
        }
        else//no ring is playing
        {
            if (s_is_vibrate    && //是否在播放振动
                MMISET_RING_TYPE_AUDIO_PLAYER != ring_type  &&
                MMISET_RING_TYPE_KURO_PLAYER  != ring_type)
            {
                if ((MMISET_CONTROL_RING_NUM < s_is_playing_vibrate_type) || 
                    (MMISET_CONTROL_RING_NUM < ring_type))
                {
                    result = TRUE;
                }
                else
                {                   
                    result = GetisPertmitVibrate(ring_type);
                }
            }
            else
            {
                result = TRUE;
            }
        }
    }
    
    //SCI_TRACE_LOW:"MMIAPISET_IsPermitPlayRing: ring_type=0x%x, g_is_playing_ring_type = 0x%x,s_is_vibrate=0x%x, s_is_playing_vibrate_type=0x%x,result= 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_3594_112_2_18_2_51_46_300,(uint8*)"ddddd",ring_type,g_is_playing_ring_type,s_is_vibrate,s_is_playing_vibrate_type,result);
    return (result);
}

/*****************************************************************************/
//  Description : if permit play ring(只给sms,cc,alarm用来判断是否能够播放振动调用的)
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsPermitPlayRingForVibrate(
                                                    MMISET_ALL_RING_TYPE_E     ring_type  //in: 即将播放铃声的类型
                                                    )
{
    BOOLEAN     result  =   FALSE;
    
    result = MMIAPISET_IsPermitPlayRing(ring_type);
    
    if (result)
    {
        //停掉所有铃声和振动
        MMIAPISET_StopAppRing();
#ifdef VIRTUAL_VIBRATE_FUNC
        MMISRVAUD_StopVibrate();//
#else
        MMISRVAUD_StopVibrate(s_vibrate_handle);
#endif

#ifdef MCARE_V31_SUPPORT
		if(McareV31_IsMcareRunning() && McfRecord_GetRcordStatue())
		{
			McfRecord_StopRcordException();
		}
#endif
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : get if permit minute hint ring in call
//  Global resource dependence : none
//  Author: bin.ji
//  Note:2004.12.21
/*****************************************************************************/
LOCAL BOOLEAN GetPermitMinhintRingIncall(void)
{
    BOOLEAN is_permit = TRUE;

    if( MMIAPISET_GetMinuteReminderTime() == CC_CALL_REMINDER_TIME_OFF)
    {
        is_permit = FALSE;
    }
    
    return (is_permit);
}

/*****************************************************************************/
//  Description : set if permit minute hint ring in call
//  Global resource dependence : none
//  Author: bin.ji
//  Note:2004.12.21
/*****************************************************************************/
PUBLIC void MMIAPISET_SetPermitMinhintRingIncall(BOOLEAN  is_permit, uint8 minutes)
{
    uint8 call_reminder_time = CC_CALL_REMINDER_TIME_DEFAULT;

    if (minutes > CC_CALL_REMINDER_TIME_60S)
    {
        minutes = CC_CALL_REMINDER_TIME_60S;
    }

    if( is_permit && MMIAPISET_GetMinuteReminderTime() == CC_CALL_REMINDER_TIME_OFF)
    {
        call_reminder_time = minutes;
        MMISET_SetMinuteReminderTime(call_reminder_time);
    }
    else if( !is_permit && MMIAPISET_GetMinuteReminderTime() != CC_CALL_REMINDER_TIME_OFF)
    {
        call_reminder_time = CC_CALL_REMINDER_TIME_OFF;
        MMISET_SetMinuteReminderTime(call_reminder_time);
    }
}

/*****************************************************************************/
//  Description : get if permit minute hint ring in call
//  Global resource dependence : none
//  Author: bin.ji
//  Note:2004.12.21
/*****************************************************************************/
LOCAL BOOLEAN GetPermitCallRingIncall(void)
{
    BOOLEAN                 is_permit = TRUE;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_SET_PERMIT_CALL_RING_INCALL,&is_permit,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_permit = TRUE;
        MMINV_WRITE(MMINV_SET_PERMIT_CALL_RING_INCALL,&is_permit);
    }
    
    return (is_permit);
}

/*****************************************************************************/
//  Description : set if permit minute hint ring in call
//  Global resource dependence : none
//  Author: bin.ji
//  Note:2004.12.21
/*****************************************************************************/
PUBLIC void MMIAPISET_SetPermitCallRingIncall(BOOLEAN  is_permit)
{
    MMINV_WRITE(MMINV_SET_PERMIT_CALL_RING_INCALL,&is_permit);
}

/*****************************************************************************/
//  Description : get if permit minute hint ring in call
//  Global resource dependence : none
//  Author: bin.ji
//  Note:2004.12.21
/*****************************************************************************/
LOCAL BOOLEAN GetPermitMsgRingIncall(void)
{
    BOOLEAN                 is_permit = TRUE;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_SET_PERMIT_MSG_RING_INCALL,&is_permit,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_permit = TRUE;
        MMINV_WRITE(MMINV_SET_PERMIT_MSG_RING_INCALL,&is_permit);
    }
    
    return (is_permit);
}

/*****************************************************************************/
//  Description : set if permit minute hint ring in call
//  Global resource dependence : none
//  Author: bin.ji
//  Note:2004.12.21
/*****************************************************************************/
PUBLIC void MMIAPISET_SetPermitMsgRingIncall(BOOLEAN  is_permit)
{
    MMINV_WRITE(MMINV_SET_PERMIT_MSG_RING_INCALL,&is_permit);
}

/*****************************************************************************/
//  Description : get if permit paly dsp tone,play generic tone
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDspPlayRing(void)
{
    BOOLEAN                 is_dsp_play_ring    =   TRUE;
    MN_RETURN_RESULT_E      return_value        =   MN_RETURN_FAILURE;
    
    //get if permit dsp play ring from nv
    MMINV_READ(MMINV_SET_DSP_PLAY_RING,&is_dsp_play_ring,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_dsp_play_ring = TRUE;
        MMINV_WRITE(MMINV_SET_DSP_PLAY_RING,&is_dsp_play_ring);
    }
    
    return (is_dsp_play_ring);
}

/*****************************************************************************/
//  Description : set if permit paly dsp tone,play generic tone
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetDspPlayRing(
                                     BOOLEAN  is_permit
                                     )
{
    MMINV_WRITE(MMINV_SET_DSP_PLAY_RING,&is_permit);
}

/*****************************************************************************/
//  Description : when permit paly dsp tone,play generic tone
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void PlayGenericTone(
                           AUD_GENERIC_TONE_ID_E    tone_id,
                           uint32   duration
                           )
{
    BOOLEAN                 is_dsp_play_ring    =   TRUE;
    
    //get if permit dsp play ring from nv
    is_dsp_play_ring = GetDspPlayRing();
    
    if (is_dsp_play_ring)
    {
#ifdef MMISRV_AUDIO_SUPPORT
        MMISRVAUD_PlayTone(MMISRVAUD_TYPE_TONE_GENERIC, tone_id, duration, 2000);
#else
        MMIAUDIO_PlayTone(MMIAUDIO_GENERIC_TONE, tone_id, duration,MMIAUDIO_TONE_DEFAULT_FREQ);
#endif
    }
    else
    {
        //SCI_TRACE_LOW:"PlayGenericTone:is_dsp_play_ring is FALSE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_3777_112_2_18_2_51_47_301,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : stop generic tone
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void StopGenericTone(void)
{
#ifdef MMISRV_AUDIO_SUPPORT
    MMISRVAUD_StopTone(MMISRVAUD_TYPE_TONE_GENERIC);
#else
    MMIAUDIO_StopTone(MMIAUDIO_GENERIC_TONE);
#endif
}

/*****************************************************************************/
//Description : 预览铃声 
//Global resource dependence : 
//Author: maqianlin
//Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_PreviewRing(
                                  MN_DUAL_SYS_E dual_sys,
                                  uint16                       ring_id,    // the ring id
                                  uint8                       ring_vol,   // the ring volume 
                                  uint32                      play_times, // play times
                                  MMISET_ALL_RING_TYPE_E      ring_type,  // in: the type of all ring
                                  MMISET_RING_CALLBACK_PFUNC  callback_func
                                  )
{
    if (MMIAPISET_IsPermitPlayRing(MMISET_RING_TYPE_OTHER))
    {
        //停掉所有铃声和振动
        MMIAPISET_StopAppRing();
        
        MMIAPISET_SetCurRingType(MMISET_RING_TYPE_OTHER);
        
        PlayRing(dual_sys, ring_id,ring_vol, play_times,ring_type,callback_func,MMIENVSET_GetCurModeId());
    }
}


/*****************************************************************************/
//Description : 判断是否震动计时器时间到 
//Global resource dependence : 
//Author: maqianlin
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsVibratTimerOut(DPARAM param)
{
    if (PNULL == param)
    {
        MMI_ASSERT_WARNING("PNULL != param");
        return FALSE;
    }
   
    if(s_mmiset_vibra_timer_id == (*(uint8 *)param))
    {
        return TRUE;
    }
    else
    {
        return FALSE; 
    }
}


/*****************************************************************************/
//Description : 判断震动计时器是否开启 
//Global resource dependence : 
//Author: maqianlin
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsVibratTimerActive(void)
{
    if ( 0 == s_mmiset_vibra_timer_id )
    {
        return FALSE;
    }
    else
    {
        return TRUE; 
    }
}

/*****************************************************************************/
//Description : 开启震动计时器 
//Global resource dependence : 
//Author: maqianlin
//Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_StartVibratTimer(void)
{
    s_mmiset_vibra_timer_id = MMK_CreateTimer(MMISET_VIBRA_DEFAULT_TIME,FALSE);
}


/*****************************************************************************/
//Description : 关闭震动计时器 
//Global resource dependence : 
//Author: maqianlin
//Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_StopVibratTimer(void)
{
    SCI_TRACE_LOW("MMIAPISET_StopVibratTimer");
    MMK_StopTimer(s_mmiset_vibra_timer_id);
    s_mmiset_vibra_timer_id = 0;
}

/*****************************************************************************/
//Description : 开启震动
//Global resource dependence : 
//Author: maqianlin
//Note:
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_StartVibrator(
                                       MMISET_ALL_RING_TYPE_E  ring_type
                                       )
{
#ifdef  VIRTUAL_VIBRATE_FUNC		
	uint16 pri= GetRingPri(ring_type); 
#endif
    
    if (MMIAPISET_IsPermitPlayRing(ring_type))
    {
        s_is_vibrate = TRUE;
        s_is_playing_vibrate_type = ring_type; 
        
#ifndef WIN32
#ifdef  VIRTUAL_VIBRATE_FUNC
        MMISRVAUD_StartVibrate(pri, MMI_10SECONDS, 0, MMIENVSET_PREVIEW_PLAY_DEFAUT_TIMES, PNULL);
#else
        GPIO_SetVibrator(TRUE);
#endif
#endif
        
    }
    
}


/*****************************************************************************/
//Description : 关闭震动 
//Global resource dependence : 
//Author: maqianlin
//Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_StopVibrator(
                                   MMISET_ALL_RING_TYPE_E  ring_type
                                   )
{
    // 判断是否正在震动并且是指定关闭的铃声类型
    if (s_is_playing_vibrate_type == ring_type && s_is_vibrate == TRUE)
    {
#ifndef WIN32
#ifdef VIRTUAL_VIBRATE_FUNC
	MMISRVAUD_StopVibrate();
#else
        GPIO_SetVibrator(FALSE);
#endif
#endif
        s_is_playing_vibrate_type = MMISET_RING_TYPE_NONE;
        s_is_vibrate = FALSE;
    }
    
}

/*****************************************************************************/
//Description : 振动
//Global resource dependence : 
//Author: maqianlin
//Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetAutoStopVibrator(
                                      BOOLEAN                 is_open,
                                      MMISET_ALL_RING_TYPE_E  ring_type,//in: the type of all vibrator
                                      uint32 duration   //振动间隔
                                       )
{
    
    if (MMIAPISET_IsPermitPlayRing(ring_type))
    {
        if(is_open)
        {
#ifndef WIN32
#ifndef VIRTUAL_VIBRATE_FUNC 
            GPIO_SetVibrator_Ext(TRUE, duration, 0, PNULL);
#endif
#endif
        }
        else
        {
#ifdef VIRTUAL_VIBRATE_FUNC
            MMISRVAUD_StopVibrate();
#else
            GPIO_SetVibrator(FALSE);
#endif
        }
    }
}


/*****************************************************************************/
//Description : refurbish idle icon display
//Global resource dependence : none
//Author: maqianlin
//Note:
/*****************************************************************************/
LOCAL void SetIdleCallRingTypeIcon(
                                   MMISET_CALL_RING_TYPE_E  call_ring_type
                                   )
{
    //refurbish idle icon display
    switch (call_ring_type)
    {
    case MMISET_CALL_RING:
        MAIN_SetIdleSilentState(FALSE);
        MAIN_SetIdleShakeState(FALSE);
        break;
        
    case MMISET_CALL_VIBRA:
    case MMISET_CALL_VIBRA_BEFORE_RING:
    case MMISET_CALL_VIBRA_AND_RING:
        MAIN_SetIdleSilentState(FALSE);
        MAIN_SetIdleShakeState(TRUE);
        break;
        
    case MMISET_CALL_SILENT:
        MAIN_SetIdleShakeState(FALSE);
        MAIN_SetIdleSilentState(TRUE);
        break;
        
    default:
        
        break;
    }
    
}

/*****************************************************************************/
//Description : 用于在启动当前环境设置模式时同时修改待机状态下的来电提示方式图标
//Global resource dependence : none
//Author: maqianlin
//Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ActiveModeSetCallRingTypeIcon(uint8 mode_id)
{
    
    MMISET_CALL_RING_TYPE_E  call_ring_type     =   MMISET_MAX_CALL_TYPE;
    
    // 获得启动的环境模式中设置的来电提示方式
    call_ring_type = (MMISET_CALL_RING_TYPE_E)MMIAPIENVSET_GetModeOptValue(mode_id,MN_DUAL_SYS_1, CALL_RING_TYPE);

    // refurbish idle icon display
    SetIdleCallRingTypeIcon(call_ring_type);
}

/*****************************************************************************/
//  Description : to STOP the vibrator
//  Global resource dependence : 
//  Author:maql
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_RingCallBack(MMISRVAUD_REPORT_RESULT_E result, DPARAM param)
{
    MMIAPISET_AlertRingPlayResult(result, param);
    
    //MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
    MMIAPISET_StopVibrator(MMISET_RING_TYPE_OTHER);
    MMIENVSET_StopOnetimeVibTimer();
    MMIAPIENVSET_SetVibStatus(FALSE);
}

#ifdef MMISRV_AUDIO_SUPPORT
/*****************************************************************************/
//  Description : Get pri by ring type.
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetRingPri(MMISET_ALL_RING_TYPE_E  ring_type)
{
    uint16 pri = 0;        
    
    switch(ring_type)
    {
#ifdef MMI_INDIAN_SOS_SUPPORT
    case MMISET_RING_TYPE_SOS:
#endif
#ifdef MMI_ETWS_SUPPORT
    case MMISET_RING_TYPE_ETWS:
    case MMISET_RING_TYPE_ETWS_PWS:
#endif
    case MMISET_RING_TYPE_CALL:
        pri = MMISRVAUD_PRI_HIGH;
        break;
        
    case MMISET_RING_TYPE_KEY:
    case MMISET_RING_TYPE_TKEY:
#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.     
    case MMISET_RING_TYPE_FLIP:
#endif    
    case MMISET_RING_TYPE_FLIP_DOWN:
#ifdef LCD_SLIDE_SUPPORT
    case MMISET_RING_TYPE_SLIDE_ON:
    case MMISET_RING_TYPE_SLIDE_OFF:        
#endif
    case MMISET_RING_TYPE_DL:
#ifdef MMI_KEY_LOCK_SUPPORT
    case MMISET_RING_TYPE_KEY_LOCK:
#endif
        pri = MMISRVAUD_PRI_LOW;
        break;

    case MMISET_RING_TYPE_RECORDER_PLAY:
    case MMISET_RING_TYPE_RECORDER_RECORD:
    case MMISET_RING_TYPE_MSG:
    case MMISET_RING_TYPE_POWER:
    #ifdef CMCC_UI_STYLE
    case MMISET_RING_TYPE_POWER_OFF:
    #endif
    case MMISET_RING_TYPE_ALARM:
    case MMISET_RING_TYPE_OTHER:
    case MMISET_RING_TYPE_BAT:
    case MMISET_RING_TYPE_CHARGE:
    case MMISET_RING_TYPE_DC:
    case MMISET_RING_TYPE_DC_COUNT: 
    case MMISET_RING_TYPE_DC_FINAL_COUNT:                
    default:
        pri = MMISRVAUD_PRI_NORMAL;
        break;
    }
    
    return pri;
}
#endif

/*****************************************************************************/
//  Description : API of get ring current type
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC MMISET_ALL_RING_TYPE_E MMIAPISET_GetCurRingType(void)
{
    return g_is_playing_ring_type;
}

/*****************************************************************************/
//  Description : API of set ring current type
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetCurRingType(MMISET_ALL_RING_TYPE_E ring_type)
{
    //SCI_TRACE_LOW:"[MMISET]:MMIAPISET_SetCurRingType, ring_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_4086_112_2_18_2_51_47_302,(uint8*)"d",ring_type);
    g_is_playing_ring_type = ring_type;
}

/*****************************************************************************/
//  Description : API of clear current ring  type
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_ClearCurRingType(MMISET_ALL_RING_TYPE_E ring_type)
{
    //SCI_TRACE_LOW:"[MMISET]:MMIAPISET_ClearCurRingType, ring_type = %d, g_is_playing_ring_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_4096_112_2_18_2_51_47_303,(uint8*)"dd",ring_type,g_is_playing_ring_type);
    if(g_is_playing_ring_type == ring_type)
    {
        g_is_playing_ring_type = MMISET_RING_TYPE_MAX;
        s_cur_real_ring_type   = MMISET_RING_TYPE_MAX;
    }
}

/*****************************************************************************/
//  Description : Whether the the ring type is the playing ring.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
#ifdef MMISRV_AUDIO_SUPPORT
PUBLIC BOOLEAN MMIAPISET_IsPlayingRingType(
                    MMISET_ALL_RING_TYPE_E ring_type
                    )
{
    BOOLEAN     result  =   FALSE;

    if( ring_type < MMISET_RING_TYPE_MAX && g_is_playing_ring_type == ring_type && s_cur_app_handle != 0)
    {
        result = TRUE;
    }

    //SCI_TRACE_LOW:"[MMISET]:MMIAPISET_IsCurRingType, ring_type=%d, g_ring=%d, s_cur_app_handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_4120_112_2_18_2_51_47_304,(uint8*)"ddd",ring_type,g_is_playing_ring_type,s_cur_app_handle);
    return result;
}
#else
PUBLIC BOOLEAN MMIAPISET_IsPlayingRingType(
                    MMISET_ALL_RING_TYPE_E ring_type
                    )
{
    BOOLEAN     result  =   FALSE;

    if( g_is_playing_ring_type == ring_type &&
        PNULL != MMIAUDIO_GetAudioHandle(MMIAPISET_GetRingHtype(ring_type)))
    {
        result = TRUE;
    }

    //SCI_TRACE_LOW:"[MMISET]:MMIAPISET_IsCurRingType, ring_type = %d, g_ring = %d, s_audio = %d, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_4136_112_2_18_2_51_47_305,(uint8*)"dddd",ring_type,g_is_playing_ring_type,s_cur_audio_htype,result);
    return result;
}
#endif

/*****************************************************************************/
//  Description : get is permit vibrate
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 实际上所有的震动都对应相应的铃声类型(ring_type)
/*****************************************************************************/
#ifdef MMISRV_AUDIO_SUPPORT
LOCAL BOOLEAN GetisPertmitVibrate(MMISET_ALL_RING_TYPE_E  ring_type)
{
    BOOLEAN         result          =   FALSE;

    if(IsNormalPri(s_is_playing_vibrate_type))
    {
        if(IsNormalPri(ring_type))
        {
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
    }
    else
    {
        result = TRUE;
    }    
    return result;
}
#else
LOCAL BOOLEAN GetisPertmitVibrate(MMISET_ALL_RING_TYPE_E  ring_type)
{
    BOOLEAN         result          =   FALSE;
    MMIAUDIO_HTYPE  auiod_htype     =   MMIAUDIO_APP;    
    
    
    auiod_htype = MMIAPISET_GetRingHtype(ring_type);
    
    if (auiod_htype <= MMIAPISET_GetRingHtype(s_is_playing_vibrate_type))
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
    
    return result;
}
#endif

/*****************************************************************************/
//  Description : alert ring play result
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_AlertRingPlayResult(MMISRVAUD_REPORT_RESULT_E result, DPARAM param)
{
    //SCI_TRACE_LOW:"[MMISET]:MMIAPISET_AlertRingPlayResult: result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_4195_112_2_18_2_51_47_306,(uint8*)"d",result);
    
    if (MMISRVAUD_REPORT_RESULT_NOT_SUPPORT== result)
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
    }
    else if ((MMISRVAUD_REPORT_RESULT_ERROR == result))
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_PLAY_ERROR);
    }
}

/*****************************************************************************/
//  Description : whether the key ring is silent
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
//PUBLIC BOOLEAN MMIAPISET_IsKeyRingSilent(void)
//{
//    uint8 value =  0;
//    value = MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_1, KEY_RING_TYPE);
//    SCI_TRACE_LOW("[MMI SET]: key ring value = %d", value);
//    return (MMISET_OTHER_SILENT == value);
// }

/*****************************************************************************/
//  Description : set audio mode is earfree
//  Global resource dependence : none
//  Author: jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetAudioMode(
                                   BOOLEAN  is_earfree  //是否需要耳机下也支持speaker播放
                                   )
{
    s_is_need_earfree_mode = is_earfree;
}

/*****************************************************************************/
//  Description : get audio mode is earfree
//  Global resource dependence : none
//  Author: jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetAudioMode(void)
{
    return (s_is_need_earfree_mode);
}
/*****************************************************************************/
// 	Description : 将NV中保存的铃声的种类放入列表中，包括固定铃声和更多音乐
//	Global resource dependence : 
//  Author:jianhui.luo
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_AppendNVCallRingCalssItem(MN_DUAL_SYS_E dual_sys, MMI_CTRL_ID_T  ctrl_id)
{
    
    uint16                      cur_selection = 0;
    MMISET_CALL_RING_T			ring_info = {MMISET_CALL_RING_FIXED, 0, 0};
    
    //append item
    MMIAPISET_AppendListItemByTextId( TXT_RING_FIXED, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_COMMON_MORE_RING, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    //get NV call ring
    ring_info = MMIAPIENVSET_GetCallRingInfo(dual_sys,MMIENVSET_GetCurModeId());
    cur_selection = ring_info.call_ring_type;
    
    //set selected item
    GUILIST_SetSelectedItem( ctrl_id, cur_selection, TRUE );
    
    //set current item
    GUILIST_SetCurItemIndex( ctrl_id,cur_selection );
}
#ifdef TOUCH_PANEL_SUPPORT 
/*****************************************************************************/
//  Description :根据触笔的坐标调整音量
//  Global resource dependence : 
//  Author: baokun
//  Note:
/*****************************************************************************/
PUBLIC uint8  MMIAPISET_TPAdjustVolume(GUI_POINT_T tp_point, uint8 cur_vol, uint8 max_vol, MMI_WIN_ID_T win_id)
{
    GUI_RECT_T          display_rect = MMITHEME_GetClientRectEx(win_id); //@CR NEWMS00166849, has been corrected by NEWMS00166034 's owner
    uint16              adj_bar_width = 0;
	uint16              adj_bar_height = 0;
    uint16              vol_width = 0;
	uint16              vol_height = 0;
	uint16              icon_space = 0;
	GUI_RECT_T          button_rect = {0};
	uint8               ret_vol = cur_vol;
	uint8               i = 0;
#ifdef GUIF_SOFTKEY_PDASTYLE
    uint16              softkey_width = 0;
    uint16              softkey_height = 0;
#endif
    GUIRES_GetImgWidthHeight(&adj_bar_width,&adj_bar_height, IMAGE_ADJUST_VOL_BLOCK_SINGLE_BG,win_id);
    GUIRES_GetImgWidthHeight(&vol_width,&vol_height, IMAGE_ADJUST_VOL_BG,win_id);

    if(max_vol * adj_bar_width < display_rect.right - display_rect.left)
    {
        icon_space = (display_rect.right - display_rect.left - (max_vol * adj_bar_width))/(max_vol + 1);
    }
    //point in volume bar
    if(adj_bar_height + vol_height < display_rect.bottom - display_rect.top)
    {
        button_rect.top = display_rect.top + (display_rect.bottom - display_rect.top - (adj_bar_height + vol_height))/2;
    }
    else
    {
        button_rect.top = display_rect.top;
    }

    button_rect.bottom = button_rect.top + adj_bar_height;

    for (i = 0; i < max_vol; i ++)
    {
        button_rect.left = icon_space + (icon_space + adj_bar_width)*i;
        button_rect.right = button_rect.left + adj_bar_width;
        if(GUI_PointIsInRect(tp_point, button_rect))
        {
            return i + 1;
        }
    }
    //check pont in plus button
    GUIRES_GetImgWidthHeight(&adj_bar_width,&adj_bar_height, IMAGE_ADJUST_VOL_PLUG,win_id);
    GUIRES_GetImgWidthHeight(&vol_width,&vol_height, IMAGE_ADJUST_VOL_BG,win_id);
    button_rect.right = display_rect.right;
    button_rect.left =  display_rect.right - adj_bar_width;
    if(vol_height > adj_bar_height)
    {
        button_rect.bottom = display_rect.bottom  - (vol_height - adj_bar_height)/2;
    }
    else
    {
        button_rect.bottom = display_rect.bottom;
    }
	
#ifdef GUIF_SOFTKEY_PDASTYLE
    {
        GUIRES_GetImgWidthHeight(&softkey_width,&softkey_height, IMAGE_PDASOFTKEY_BG,win_id);
        button_rect.bottom -= softkey_height;//加上softkey的高度
    }
#endif 

    button_rect.top = button_rect.bottom - adj_bar_height;
    if(GUI_PointIsInRect(tp_point, button_rect))
    {
        if(cur_vol < max_vol)
        {
            ret_vol = cur_vol+1;
        }
    }
    //point in minus button
    GUIRES_GetImgWidthHeight(&adj_bar_width,&adj_bar_height, IMAGE_ADJUST_VOL_MINUS,win_id);
    button_rect.left =  0;
    button_rect.right = display_rect.left + adj_bar_width;

    if(GUI_PointIsInRect(tp_point, button_rect))
    {
        if(cur_vol > 0)
        {
            ret_vol = cur_vol - 1;
        }
    }

    return ret_vol;	
}
#endif
#ifdef MONOCHROME_LCD_SUPPORT 
#define  VOLUME_ADJUST_START_POS_XOFF  19
#define  VOLUME_ADJUST_START_POS_YOFF  0

/*****************************************************************************/
// 	Description : DPHONE 音量调节绘制
//	Global resource dependence : 
//  Author:yiwen.man
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_DrawAdjustVolumePicture(uint8   volume,
								MMI_WIN_ID_T win_id)
{
    uint32              i               =   0;
    GUI_LCD_DEV_INFO    lcd_dev_info    =   {GUI_LCD_0, GUI_BLOCK_0};
	GUI_LCD_DEV_INFO	   lcd_dev_info_fill = {GUI_LCD_0, GUI_BLOCK_0};
    GUI_RECT_T          display_rect    =   {0, MMI_TITLE_HEIGHT, MMI_MAINSCREEN_RIGHT_MAX_PIXEL, MMI_MAINSCREEN_BOTTOM_MAX_PIXEL - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT};
    GUI_POINT_T         dis_point       =   {0};

    uint32 image_id = (IMAGE_RING_VOL_LEVEL_ONE-1);


	
    LCD_FillRect(&lcd_dev_info_fill, display_rect, MMI_WHITE_COLOR);
	
    dis_point.x = VOLUME_ADJUST_START_POS_XOFF;
    dis_point.y = VOLUME_ADJUST_START_POS_YOFF;

    if (volume <= MMISET_VOL_MAX)
    {
        image_id += volume;
		   
	    GUIRES_DisplayImg(&dis_point,
	        PNULL,
	        PNULL,
	        win_id,
	        image_id,
	        &lcd_dev_info);
	}

}

#endif

#ifdef MMISRV_AUDIO_SUPPORT


/*****************************************************************************/
// 	Description : Audio call back function called by audio service.
//	Global resource dependence : 
//   Author: Yintang.Ren
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleAudioCallBack(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    //SCI_TRACE_LOW:"[MMISET]: HandleAudioCallBack() entry, handle=0x%x, param=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_4406_112_2_18_2_51_48_307,(uint8*)"dd", handle, param);
    
    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        
        if(report_ptr != PNULL)
        {            
            //MMISRV_TRACE_LOW:"[MMISRV]: HandleNotify(), report_ptr->report=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_RING_4414_112_2_18_2_51_48_308,(uint8*)"d", report_ptr->report);
        
            switch(report_ptr->report)
            {
            case MMISRVAUD_REPORT_END:
                g_is_playing_ring_type = MMISET_RING_TYPE_MAX;
                s_cur_real_ring_type   = MMISET_RING_TYPE_MAX;    
                s_cur_app_handle = 0;
                if(s_ring_callback_func != PNULL)
                {
                    (*s_ring_callback_func)(MMISRVAUD_REPORT_RESULT_SUCESS, param); /*lint !e64*/
                    s_ring_callback_func = PNULL;
                }
                break;
            default:
                break;
            }
        }
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : Stop app ring.
//	Global resource dependence : 
//   Author: Yintang.Ren
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_StopAppRing(void)
{
    if(s_cur_app_handle != 0)
    {
        MMISRVMGR_Free(s_cur_app_handle);
        s_cur_app_handle = 0;
    }
    
    g_is_playing_ring_type = MMISET_RING_TYPE_MAX;    
    s_cur_real_ring_type   = MMISET_RING_TYPE_MAX; 
	CallBackFunction(MMISRVAUD_REPORT_RESULT_STOP, PNULL);
    s_ring_callback_func = PNULL;
}

/*****************************************************************************/
// 	Description : If app ring is in playing state.
//	Global resource dependence : 
//   Author: Yintang.Ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsPlayingAppRing(void)
{
    if(g_is_playing_ring_type != MMISET_RING_TYPE_MAX && s_cur_app_handle != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif
#ifdef MMI_MMS_ADJUST_VOLUME_SUPPORT
/*****************************************************************************/
// 	Description : Set multi and real ausrv sound by app  by multi ptr buf
//	Global resource dependence : 
//    Author: CBK
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetMultimRealVolume(uint8 vol)
{
   MMIAPISET_SetMultimVolume(vol);
  
#ifdef MMISRV_AUDIO_SUPPORT	
    MMISRVAUD_SetVolume(s_cur_app_handle, vol);
#endif
}
#endif

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//  Description : handle counted time timer over msg
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_HandleSOSTorchTimer(uint8 timer_id, uint32 param)
{
    MMISET_ETWS_RING_T*  etws_ring_info = (MMISET_ETWS_RING_T*)param;

    MMK_StopTimer(s_sos_torch_time_id);
    s_sos_torch_time_id = 0;

    if( TRUE == etws_ring_info->is_need_torch)
    {
//#ifndef WIN32
#if 0
        if(TRUE == GPIO_GetTorchStatus())
        {
            SCI_TRACE_LOW("[MMISET] MMIAPISET_HandleSOSTorchTimer, before GPIO_SetTorch(FALSE)");
            GPIO_SetTorch(FALSE);
        }
        else
        {
            SCI_TRACE_LOW("[MMISET] MMIAPISET_HandleSOSTorchTimer, before GPIO_SetTorch(TRUE)");
            GPIO_SetTorch(TRUE);
        }
#endif
     }

    s_sos_torch_time_id  = MMK_CreateTimerCallback(MMI_1SECONDS,  MMIAPISET_HandleSOSTorchTimer, etws_ring_info, TRUE);

}

/*****************************************************************************/
//  Description : handle counted time timer over msg
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_HandleSOSVibrateTimer(uint8 timer_id, uint32 param)
{
    MMISET_ETWS_RING_T*  etws_ring_info = (MMISET_ETWS_RING_T*)param;

    MMK_StopTimer(s_sos_vibrator_time_id);
    s_sos_vibrator_time_id = 0;

    SCI_TRACE_LOW("MMIAPISET_HandleSOSVibrateTimer  s_sos_vibrator_state=%d", s_sos_vibrator_state);

    if( PNULL == etws_ring_info  )
    {
        SCI_TRACE_LOW("MMIAPISET_HandleSOSVibrateTimer etws_ring_info = PNULL!! ");
        return ;
    }

    // NOT PWS
    if( FALSE == etws_ring_info->is_personal )
    {

        SCI_TRACE_LOW("MMIAPISET_HandleSOSVibrateTimer  is_personal=%d", etws_ring_info->is_personal);
        switch (s_sos_vibrator_state)
        {
        case FIRST_VIBRATE_2SECOND_START_1:
            s_sos_vibrator_time_id = MMK_CreateTimerCallback(MMI_500MSECONDS,  MMIAPISET_HandleSOSVibrateTimer, (uint32)etws_ring_info, TRUE);
            MMIAPISET_SetVibrator(FALSE, MMISET_RING_TYPE_ETWS);
            MMIAPISET_StopAllRing(TRUE);
            s_sos_vibrator_state = FIRST_PAUSE_VIBRATE_2;
            break;

        case FIRST_PAUSE_VIBRATE_2:
            s_sos_vibrator_time_id = MMK_CreateTimerCallback(MMI_1SECONDS,  MMIAPISET_HandleSOSVibrateTimer, (uint32)etws_ring_info, TRUE);
            MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_ETWS);
            MMIAPISET_PlayRing(etws_ring_info->dual_sys, FALSE, 0, 1, MMISET_RING_TYPE_ETWS,PNULL);
            s_sos_vibrator_state = SECOND_VIBRATE_1SECOND_START_3;
            break;

        case SECOND_VIBRATE_1SECOND_START_3:
            s_sos_vibrator_time_id = MMK_CreateTimerCallback(MMI_500MSECONDS,  MMIAPISET_HandleSOSVibrateTimer, (uint32)etws_ring_info, TRUE);
            MMIAPISET_SetVibrator(FALSE, MMISET_RING_TYPE_ETWS);
            MMIAPISET_StopAllRing(TRUE);
            s_sos_vibrator_state = SECOND_PAUSE_VIBRATE_4;
            break;

        case SECOND_PAUSE_VIBRATE_4:
            s_sos_vibrator_time_id = MMK_CreateTimerCallback(MMI_1SECONDS,  MMIAPISET_HandleSOSVibrateTimer, (uint32)etws_ring_info, TRUE);
            MMIAPISET_SetVibrator(TRUE, MMISET_RING_TYPE_ETWS);
            MMIAPISET_PlayRing(etws_ring_info->dual_sys, FALSE, 0, 1, MMISET_RING_TYPE_ETWS,PNULL);
            s_sos_vibrator_state = THIRD_VIBRATE_1SECOND_START_5;
            break;

        case THIRD_VIBRATE_1SECOND_START_5:
            s_sos_vibrator_time_id = MMK_CreateTimerCallback(MMI_500MSECONDS,  MMIAPISET_HandleSOSVibrateTimer, (uint32)etws_ring_info, TRUE);
            MMIAPISET_SetVibrator(FALSE, MMISET_RING_TYPE_ETWS);
            MMIAPISET_StopAllRing(TRUE);
            s_sos_vibrator_state = THIRD_PAUSE_VIBRATE_6;
            break;

        case THIRD_PAUSE_VIBRATE_6:
            s_sos_vibrator_time_id = MMK_CreateTimerCallback(MMI_2SECONDS,  MMIAPISET_HandleSOSVibrateTimer, (uint32)etws_ring_info, TRUE);
            MMIAPISET_SetVibrator(TRUE, MMISET_RING_TYPE_ETWS);
            MMIAPISET_PlayRing(etws_ring_info->dual_sys, FALSE, 0, 1, MMISET_RING_TYPE_ETWS,PNULL);
            s_sos_vibrator_state = FIRST_VIBRATE_2SECOND_START_1;
            break;

        default:
            break;
        }
    }
    else if(   (TRUE == etws_ring_info->is_personal )  &&
               ((MMISET_MSG_VIBRA_AND_RING == etws_ring_info->ring_type)
            || (MMISET_MSG_RING == etws_ring_info->ring_type)
            || (MMISET_MSG_VIBRA == etws_ring_info->ring_type ) ) )
    {
        SCI_TRACE_LOW("MMIAPISET_HandleSOSVibrateTimer  is_personal=%d, ring_type = %d", etws_ring_info->is_personal, etws_ring_info->ring_type);
        switch (s_sos_vibrator_state)
        {
        case FIRST_VIBRATE_2SECOND_START_1:
            s_sos_vibrator_time_id = MMK_CreateTimerCallback(MMI_500MSECONDS,  MMIAPISET_HandleSOSVibrateTimer, (uint32)etws_ring_info, TRUE);
            if(  (MMISET_MSG_VIBRA_AND_RING == etws_ring_info->ring_type ) || ( MMISET_MSG_VIBRA == etws_ring_info->ring_type ) )
            {
                MMIAPISET_SetVibrator(FALSE, MMISET_RING_TYPE_ETWS_PWS);//bug1917966
            }
            MMIAPISET_StopAllRing(TRUE);
            s_sos_vibrator_state = FIRST_PAUSE_VIBRATE_2;
            break;

        case FIRST_PAUSE_VIBRATE_2:
            s_sos_vibrator_time_id = MMK_CreateTimerCallback(MMI_1SECONDS,  MMIAPISET_HandleSOSVibrateTimer, (uint32)etws_ring_info, TRUE);
            if(  (MMISET_MSG_VIBRA_AND_RING == etws_ring_info->ring_type ) || ( MMISET_MSG_VIBRA == etws_ring_info->ring_type ) )
            {
                MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_ETWS_PWS);
            }
            if(  (MMISET_MSG_VIBRA_AND_RING == etws_ring_info->ring_type ) || ( MMISET_MSG_RING == etws_ring_info->ring_type ) )
            {
                MMIAPISET_PlayRing(etws_ring_info->dual_sys, FALSE, 0, 1, MMISET_RING_TYPE_ETWS_PWS,PNULL);
            }
            s_sos_vibrator_state = SECOND_VIBRATE_1SECOND_START_3;
            break;

        case SECOND_VIBRATE_1SECOND_START_3:
            s_sos_vibrator_time_id = MMK_CreateTimerCallback(MMI_500MSECONDS,  MMIAPISET_HandleSOSVibrateTimer, (uint32)etws_ring_info, TRUE);
            if(  (MMISET_MSG_VIBRA_AND_RING == etws_ring_info->ring_type ) || ( MMISET_MSG_VIBRA == etws_ring_info->ring_type ) )
            {
                MMIAPISET_SetVibrator(FALSE, MMISET_RING_TYPE_ETWS_PWS);
            }
            MMIAPISET_StopAllRing(TRUE);
            s_sos_vibrator_state = SECOND_PAUSE_VIBRATE_4;
            break;

        case SECOND_PAUSE_VIBRATE_4:
            s_sos_vibrator_time_id = MMK_CreateTimerCallback(MMI_1SECONDS,  MMIAPISET_HandleSOSVibrateTimer, (uint32)etws_ring_info, TRUE);
            if(  (MMISET_MSG_VIBRA_AND_RING == etws_ring_info->ring_type ) || ( MMISET_MSG_VIBRA == etws_ring_info->ring_type ) )
            {
                MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_ETWS_PWS);
            }
            if(  (MMISET_MSG_VIBRA_AND_RING == etws_ring_info->ring_type ) || ( MMISET_MSG_RING == etws_ring_info->ring_type ) )
            {
                MMIAPISET_PlayRing(etws_ring_info->dual_sys, FALSE, 0, 1, MMISET_RING_TYPE_ETWS_PWS,PNULL);
            }
            s_sos_vibrator_state = THIRD_VIBRATE_1SECOND_START_5;
            break;

        case THIRD_VIBRATE_1SECOND_START_5:
            s_sos_vibrator_time_id = MMK_CreateTimerCallback(MMI_500MSECONDS,  MMIAPISET_HandleSOSVibrateTimer, (uint32)etws_ring_info, TRUE);
            if(  (MMISET_MSG_VIBRA_AND_RING == etws_ring_info->ring_type ) || ( MMISET_MSG_VIBRA == etws_ring_info->ring_type ) )
            {
                MMIAPISET_SetVibrator(FALSE, MMISET_RING_TYPE_ETWS_PWS);
            }
            MMIAPISET_StopAllRing(TRUE);
            s_sos_vibrator_state = THIRD_PAUSE_VIBRATE_6;
            break;

        case THIRD_PAUSE_VIBRATE_6:
            s_sos_vibrator_time_id = MMK_CreateTimerCallback(MMI_2SECONDS,  MMIAPISET_HandleSOSVibrateTimer, (uint32)etws_ring_info, TRUE);
            if(  (MMISET_MSG_VIBRA_AND_RING == etws_ring_info->ring_type ) || ( MMISET_MSG_VIBRA == etws_ring_info->ring_type ) )
            {
                MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_ETWS_PWS);
            }
            if(  (MMISET_MSG_VIBRA_AND_RING == etws_ring_info->ring_type ) || ( MMISET_MSG_RING == etws_ring_info->ring_type ) )
            {
                MMIAPISET_PlayRing(etws_ring_info->dual_sys, FALSE, 0, 1, MMISET_RING_TYPE_ETWS_PWS,PNULL);
            }
            s_sos_vibrator_state = FIRST_VIBRATE_2SECOND_START_1;
            break;

        default:
            break;
        }


    }

}

PUBLIC void MMIAPISET_SetSOSVibrate(MN_DUAL_SYS_E dual_sys, BOOLEAN is_personal, BOOLEAN is_need_torch)

{
    MMISET_MSG_RING_TYPE_E ring_type = MMISET_MAX_MSG_TYPE;
    MMISET_ETWS_RING_T*   etws_ring_info = PNULL;
    MMISRV_HANDLE_T handle = 0;
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};

    etws_ring_info = (MMISET_ETWS_RING_T *)SCI_ALLOCAZ(sizeof(MMISET_ETWS_RING_T));
    SCI_MEMSET(etws_ring_info,0,sizeof(MMISET_ETWS_RING_T));

    SCI_MEMSET(&plmn_info, 0x00, sizeof(plmn_info));
    plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

   SCI_TRACE_LOW("[MMISET] MMIAPISET_SetSOSVibrate, is_personal = %d", is_personal);

    MMIAPISET_StopAllRing(TRUE);
    MMIAPISET_StopSOSVibrate();
    s_is_sos_vibrator = TRUE;
    s_is_mp3_playing = FALSE;

    if(MMIMP3_AUDIO_PLAYING == MMIAPIMP3_GetAudioState())
    {
         s_is_mp3_playing = TRUE;
         MMIAPIMP3_PauseMp3(FALSE);
    }

    // PWS support
    if( TRUE == is_personal )
    {
		// Get ring type
        ring_type = (MMISET_MSG_RING_TYPE_E)MMIAPIENVSET_GetActiveModeOptValue(dual_sys, MSG_RING_TYPE);
#if 0
        //if(!MMISet_GetNotificationAlert())
        {
            if(MMISET_MSG_RING==ring_type)
            {
                ring_type=MMISET_MSG_SILENT ;
            }
            else if(MMISET_MSG_VIBRA_AND_RING==ring_type)
            {
                ring_type=MMISET_MSG_VIBRA;
            }
        }
#endif
        etws_ring_info->is_personal = is_personal;
        etws_ring_info->dual_sys = dual_sys;
        etws_ring_info->ring_type = ring_type;

        if( (MMISET_MSG_VIBRA_AND_RING == ring_type )
            || (MMISET_MSG_RING == ring_type)
            || (MMISET_MSG_VIBRA == ring_type ) )
        {
            s_sos_vibrator_time_id = MMK_CreateTimerCallback(MMI_2SECONDS,  MMIAPISET_HandleSOSVibrateTimer, (uint32)etws_ring_info, TRUE);
            s_sos_vibrator_state = FIRST_VIBRATE_2SECOND_START_1;

            if(  (MMISET_MSG_VIBRA_AND_RING == ring_type ) || ( MMISET_MSG_VIBRA == ring_type ) )
            {
                MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_ETWS_PWS);
            }
            if(  (MMISET_MSG_VIBRA_AND_RING == ring_type ) || ( MMISET_MSG_RING == ring_type ) )
            {
                MMIAPISET_PlayRing(dual_sys, FALSE, 0, 1, MMISET_RING_TYPE_ETWS_PWS,PNULL);
            }
        }
    }
    else
    {

        etws_ring_info->is_personal = is_personal;
        etws_ring_info->dual_sys = dual_sys;
        etws_ring_info->ring_type = MMISET_MSG_VIBRA_AND_RING;

        s_sos_vibrator_time_id = MMK_CreateTimerCallback(MMI_2SECONDS,  MMIAPISET_HandleSOSVibrateTimer, (uint32)etws_ring_info, TRUE);
        s_sos_vibrator_state = FIRST_VIBRATE_2SECOND_START_1;

        MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_ETWS);
        MMIAPISET_PlayRing(dual_sys, FALSE, 0, 1, MMISET_RING_TYPE_ETWS,PNULL);
    }

    if(TRUE == is_need_torch)
    {
        etws_ring_info->is_need_torch = TRUE;
//#ifndef WIN32
#if 0
        SCI_TRACE_LOW("[MMISET] MMIAPISET_SetSOSVibrate, GPIO_SetTorch(TRUE)");
        GPIO_SetTorch(TRUE);
#endif
        s_sos_torch_time_id  = MMK_CreateTimerCallback(MMI_1SECONDS,  MMIAPISET_HandleSOSTorchTimer, etws_ring_info, TRUE);
    }
    else
    {
        etws_ring_info->is_need_torch = FALSE;
    }
    SCI_TRACE_LOW("[MMISET] MMIAPISET_SetSOSVibrate, etws_ring_info->is_need_torch = %d", etws_ring_info->is_need_torch);

    SCI_TRACE_LOW("[MMISET] MMIAPISET_SetSOSVibrate, is_personal, %d, ring_type = %d", is_personal, ring_type);

}

/*****************************************************************************/
//  Description : stop SOS vibrate
//  Global resource dependence : none
//  Author: candice chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_StopSOSVibrate(void)
{
            SCI_TRACE_LOW("[MMISET] MMIAPISET_StopSOSVibrate");

    s_is_sos_vibrator = FALSE;

    MMK_StopTimer(s_sos_vibrator_time_id);
    s_sos_vibrator_time_id = 0;

	MMIAPISET_SetVibrator(FALSE, MMISET_RING_TYPE_ETWS);
	MMIAPISET_SetVibrator(FALSE, MMISET_RING_TYPE_ALARM);
    MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_NONE);
    MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_ETWS_PWS); //bug1907059
	MMIAPISET_StopAllRing(FALSE);

     if( (TRUE == s_is_mp3_playing)  && (MMIMP3_AUDIO_PAUSED == MMIAPIMP3_GetAudioState()) )
     {
        s_is_mp3_playing = FALSE;
        MMIAPIMP3_ResumeMp3(FALSE);
     }

    if( 0 != s_sos_torch_time_id)
    {
        MMK_StopTimer(s_sos_torch_time_id);
        s_sos_torch_time_id = 0;

//#ifndef WIN32
#if 0
        if( TRUE == GPIO_GetTorchStatus() )
        {
            SCI_TRACE_LOW("[MMISET] MMIAPISET_StopSOSVibrate, GPIO_SetTorch(FALSE)");
            GPIO_SetTorch(FALSE);
        }
#endif
    }
}

/*****************************************************************************/
//  Description : check SOS vibrate
//  Global resource dependence : none
//  Author: candice chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsSOSVibrate(void)
{
    if( TRUE == s_is_sos_vibrator )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif


#if defined(MMI_OCR_AUDIO_PLAY_TEST_LONG)// wuxx 20210805
// OCR 语音播放, 可作为调音频参数用
PUBLIC BOOLEAN  MMIAPI_Play_Ocr_Audio_Test(void)
{
   // if(0 == MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_1, DIAL_RING_TYPE))
   {
        SCI_TraceLow("wuxx, MMIAPI_Play_Ocr_Audio_Test");
     	MMIAPISET_PlayRing(MN_DUAL_SYS_1,FALSE,1,10000,MMISET_RING_TYPE_OCR_AUDIO_TEST,PNULL);
        return TRUE;
    }
    return FALSE;
}

//关闭所有铃声
////MMIAPISET_StopAllRing(TRUE);

#endif


