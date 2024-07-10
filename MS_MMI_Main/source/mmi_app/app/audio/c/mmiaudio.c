#if 0
#include "mmi_app_audio_trc.h"
#ifdef MMISRV_AUDIO_SUPPORT 

/****************************************************************************
** File Name:      mmiaudio.c                                                              *
** Author:          Yintang.ren                                                               *
** Date:             04/05/2011                                                              *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.           *
** Description:    This file is to compatible to the old audio api whitch exist in the mmiaudio.c file 
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE                NAME                 DESCRIPTION                                 *
** 04/05/2011       Yintang.ren         Create
** 
****************************************************************************/

//#include "mmi_appmsg.h"
//#include "mmi_common.h"
#include "mmisrvmgr.h"
#include "mmisrvaud_api.h"
#include "mmisrv_debug.h"
#include "mmiaudio.h"
#include "aud_gen.h"

#define SRVAUD_OPT

typedef enum
{
    MMIAUDIO_HTYPE_DTMF_TONE = MMIAUDIO_MAX + 10,
    MMIAUDIO_HTYPE_CUSTOM_TONE,
    MMIAUDIO_HTYPE_GENERIC_TONE,
    MMIAUDIO_HTYPE_CALL,
    MMIAUDIO_HTYPE_CALL_LB, /* loop back */
    MMIAUDIO_HTYPE_FM,
    MMIAUDIO_HTYPE_RECORD
}MMIAUDIO_HTYPE_EXT_E;

typedef struct
{
    MMISRV_HANDLE_T handle;
    MMIAUDIO_END_CALLBACK_PFUNC cb_func;
}MMIAUDIO_TYPE_T;

typedef struct
{
    MMIAUDIO_TYPE_T app;
    MMIAUDIO_TYPE_T player;
    MMIAUDIO_TYPE_T music;
    MMIAUDIO_TYPE_T kuro;
#ifdef JAVA_SUPPORT    
    MMIAUDIO_TYPE_T java;
#endif
    MMIAUDIO_TYPE_T normal;

    /*-----------------------------*/
    MMIAUDIO_TYPE_T dtmf_tone;
    MMIAUDIO_TYPE_T custom_tone;
    MMIAUDIO_TYPE_T generic_tone;
    MMIAUDIO_TYPE_T call;
    MMIAUDIO_TYPE_T call_lb;    /* call loop back */
    MMIAUDIO_TYPE_T record;
    MMIAUDIO_TYPE_T fm;
    
    MMIAUDIO_HTYPE cur_type;
    BOOLEAN is_voice_lb_enable;
}MMIAUDIO_INTERNAL_INFO_T;


LOCAL MMIAUDIO_INTERNAL_INFO_T s_audio_info = {0};

/* 为了兼容老的机制*/
HAUDIO hVoiceLoopbackRing = 0;

/*****************************************************************************/
//  Description : audio init
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL MMISRV_HANDLE_T GetHandleByType(MMIAUDIO_HTYPE   audio_handle_type)
{
    MMISRV_HANDLE_T handle = 0;
    switch(audio_handle_type)
    {
    case MMIAUDIO_APP:
        handle = s_audio_info.app.handle;
        break;
    case MMIAUDIO_PLAYER:
        handle = s_audio_info.player.handle;
        break;
    case MMIAUDIO_MUSIC:
        handle = s_audio_info.music.handle;
        break;
    case MMIAUDIO_KURO:
        handle = s_audio_info.kuro.handle;
        break;
#ifdef JAVA_SUPPORT
    case MMIAUDIO_JAVA:
        handle = s_audio_info.java.handle;
        break;
#endif
    case MMIAUDIO_NORMAL:
        handle = s_audio_info.normal.handle;
        break;
    case MMIAUDIO_HTYPE_DTMF_TONE:
        handle = s_audio_info.dtmf_tone.handle;
        break;
    case MMIAUDIO_HTYPE_CUSTOM_TONE:
        handle = s_audio_info.custom_tone.handle;
        break;
    case MMIAUDIO_HTYPE_GENERIC_TONE:
        handle = s_audio_info.generic_tone.handle;
        break;
    case MMIAUDIO_HTYPE_CALL:
        handle = s_audio_info.call.handle;
        break;
    case MMIAUDIO_HTYPE_CALL_LB:
        handle = s_audio_info.call_lb.handle;
        break;
    case MMIAUDIO_HTYPE_FM:
        handle = s_audio_info.fm.handle;
        break;
    case MMIAUDIO_HTYPE_RECORD:
        handle = s_audio_info.record.handle;
        break;
    default:
        handle = 0;
        break;
    }
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: GetHandleByType() exit, type=%d, handle=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_127_112_2_18_2_3_26_0,(uint8*)"dd", audio_handle_type, handle));
    
    return handle;
}

/*****************************************************************************/
//  Description : audio type info
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL MMIAUDIO_TYPE_T *GetTypeInfo(MMIAUDIO_HTYPE   audio_handle_type)
{
    MMIAUDIO_TYPE_T *type_ptr = PNULL;
    switch(audio_handle_type)
    {
    case MMIAUDIO_APP:
        type_ptr = &s_audio_info.app;
        break;
    case MMIAUDIO_PLAYER:
        type_ptr = &s_audio_info.player;
        break;
    case MMIAUDIO_MUSIC:
        type_ptr = &s_audio_info.music;
        break;
    case MMIAUDIO_KURO:
        type_ptr = &s_audio_info.kuro;
        break;
#ifdef JAVA_SUPPORT
    case MMIAUDIO_JAVA:
        type_ptr = &s_audio_info.java;
        break;
#endif
    case MMIAUDIO_NORMAL:
        type_ptr = &s_audio_info.normal;
        break;
    case MMIAUDIO_HTYPE_DTMF_TONE:
        type_ptr = &s_audio_info.dtmf_tone;
        break;        
    case MMIAUDIO_HTYPE_CUSTOM_TONE:
        type_ptr = &s_audio_info.custom_tone;
        break;        
    case MMIAUDIO_HTYPE_GENERIC_TONE:
        type_ptr = &s_audio_info.generic_tone;
        break;                
    case MMIAUDIO_HTYPE_CALL:
        type_ptr = &s_audio_info.call;
        break;        
    case MMIAUDIO_HTYPE_CALL_LB:
        type_ptr = &s_audio_info.call_lb;
        break;        
    case MMIAUDIO_HTYPE_FM:
        type_ptr = &s_audio_info.fm;
        break;
    case MMIAUDIO_HTYPE_RECORD:
        type_ptr = &s_audio_info.record;
        break;
    default:
        break;
    }
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: GetTypeInfo() exit, type=%d, type_ptr=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_186_112_2_18_2_3_26_1,(uint8*)"dd", audio_handle_type, type_ptr));
    
    return type_ptr;
}

void SetCurType(MMIAUDIO_HTYPE audio_handle_type)
{
    s_audio_info.cur_type = audio_handle_type;
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: SetCurType() exit, cur_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_194_112_2_18_2_3_26_2,(uint8*)"d", audio_handle_type));
}

/*****************************************************************************/
//  Description : Convert type to format.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL MMISRV_HANDLE_T ConvertTypeToFmt(MMIAUDIO_TYPE_E ring_type)
{
    MMISRVAUD_RING_FMT_E fmt = MMISRVAUD_RING_FMT_NONE;
    switch(ring_type)
    {
    case MMIAUDIO_MIDI:
        fmt = MMISRVAUD_RING_FMT_MIDI;
        break;
    case MMIAUDIO_SMAF:
        fmt = MMISRVAUD_RING_FMT_SMAF;
        break;
    case MMIAUDIO_WAVE:
        fmt = MMISRVAUD_RING_FMT_WAVE;
        break;
    case MMIAUDIO_MP3:
        fmt = MMISRVAUD_RING_FMT_MP3;
        break;
    case MMIAUDIO_WMA:
        fmt = MMISRVAUD_RING_FMT_WMA;
        break;
    case MMIAUDIO_AMR:
        fmt = MMISRVAUD_RING_FMT_AMR;
        break;
    case MMIAUDIO_AAC:
        fmt = MMISRVAUD_RING_FMT_AAC;
        break;
    case MMIAUDIO_M4A:
        fmt = MMISRVAUD_RING_FMT_M4A;
        break;
    case MMIAUDIO_GENERIC_TONE:
    case MMIAUDIO_CUSTOM_TONE:
    case MMIAUDIO_DTMF_TONE:
    default:
        fmt = MMISRVAUD_RING_FMT_NONE;
        break;
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: ConvertTypeToFmt() exit, ring_type=%d, fmt=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_238_112_2_18_2_3_26_3,(uint8*)"dd", ring_type, fmt));
    
    return fmt;
}

/*****************************************************************************/
//  Description : Convert eq mode to audio service eq mode.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL MMISRVAUD_EQ_MODE_E ConvertEqMode(MMISRVAUD_EQ_MODE_E eq_mode)
{
    EQ_EXP_MODE_E eq = EQ_EXP_MODE_PASS;
    switch(eq_mode)
    {
    case MMISRVAUD_EQ_REGULAR:
        eq = EQ_EXP_MODE_PASS;
        break;
    case MMISRVAUD_EQ_CLASSIC:
        eq = EQ_EXP_MODE_CLASSIC;
        break;
    case MMISRVAUD_EQ_ODEUM:
        eq = EQ_EXP_MODE_POP;
        break;
    case MMISRVAUD_EQ_JAZZ:
        eq = EQ_EXP_MODE_VOCAL;
        break;
    case MMISRVAUD_EQ_ROCK:
        eq = EQ_EXP_MODE_POWER;
        break;
    case MMISRVAUD_EQ_SOFT_ROCK:
        eq = EQ_EXP_MODE_ROCK;
        break;
    default:
        break;
    }
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: ConvertEqMode() exit, eq_mode=%d, eq=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_273_112_2_18_2_3_27_4,(uint8*)"dd", eq_mode, eq));
    
    return eq;
}

/*****************************************************************************/
//  Description : Convert device mode to audio service route.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL MMISRVAUD_ROUTE_T ConvertDevMode(AUDIO_DEVICE_MODE_TYPE_E mode)
{
    MMISRVAUD_ROUTE_T route = MMISRVAUD_ROUTE_NONE;
    switch(mode)
    {
    case AUDIO_DEVICE_MODE_HANDHOLD:
        route = AUDIO_DEVICE_MODE_HANDHOLD;
        break;
    case AUDIO_DEVICE_MODE_HANDFREE:
        route = MMISRVAUD_ROUTE_SPEAKER;
        break;
    case AUDIO_DEVICE_MODE_EARPHONE:
        route = MMISRVAUD_ROUTE_EARPHONE;
        break;
    case AUDIO_DEVICE_MODE_EARFREE:
        route = MMISRVAUD_ROUTE_SPEAKER|MMISRVAUD_ROUTE_EARPHONE;
        break;
    case AUDIO_DEVICE_MODE_TVOUT:
        route = MMISRVAUD_ROUTE_TVOUT;
        break;
    default:
        break;
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: ConvertDevMode() exit, mode=%d, route=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_306_112_2_18_2_3_27_5,(uint8*)"dd", mode, route));

    return route;
}

/*****************************************************************************/
//  Description : Convert device mode to audio service route.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_DEVICE_MODE_TYPE_E ConvertRoute(MMISRVAUD_ROUTE_T route)
{
    AUDIO_DEVICE_MODE_TYPE_E mode = AUDIO_DEVICE_MODE_HANDHOLD;
    switch(route)
    {
    case MMISRVAUD_ROUTE_HANDHOLD:
        mode = AUDIO_DEVICE_MODE_HANDHOLD;
        break;
    case MMISRVAUD_ROUTE_SPEAKER:
        mode = AUDIO_DEVICE_MODE_HANDFREE;
        break;
    case MMISRVAUD_ROUTE_BLUETOOTH:
    case MMISRVAUD_ROUTE_EARPHONE:
        mode = AUDIO_DEVICE_MODE_EARPHONE;
        break;
    case MMISRVAUD_ROUTE_SPEAKER|MMISRVAUD_ROUTE_EARPHONE:
        mode = AUDIO_DEVICE_MODE_EARFREE;
        break;
    case MMISRVAUD_ROUTE_TVOUT:
        mode = AUDIO_DEVICE_MODE_TVOUT;
        break;
    default:
        break;
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: ConvertRoute() exit, route=%d, mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_340_112_2_18_2_3_27_6,(uint8*)"dd", route, mode));
    
    return mode;
}

LOCAL MMISRVAUD_PLAY_RATE_E ConvertRate(MMIAUDIO_PLAY_RATE_E rate)
{
    MMISRVAUD_PLAY_RATE_E srv_rate = MMIAUDIO_PLAY_RATE_NORMAL;
    
    switch(rate)
    {
    case MMIAUDIO_PLAY_RATE_SLOW_2:
        srv_rate = MMISRVAUD_PLAY_RATE_SLOW_2;
        break;
    case MMIAUDIO_PLAY_RATE_SLOW_1:
        srv_rate = MMISRVAUD_PLAY_RATE_SLOW_1;
        break;
    case MMIAUDIO_PLAY_RATE_NORMAL:
        srv_rate = MMISRVAUD_PLAY_RATE_NORMAL;
        break;
    case MMIAUDIO_PLAY_RATE_FAST_1:
        srv_rate = MMISRVAUD_PLAY_RATE_FAST_1;
        break;
    case MMIAUDIO_PLAY_RATE_FAST_2:
        srv_rate = MMISRVAUD_PLAY_RATE_FAST_2;
        break;
     default:
        break;
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: ConvertRate() exit, rate=%d, srv_rate=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_370_112_2_18_2_3_27_7,(uint8*)"dd", rate, srv_rate));
    
    return srv_rate;
}

/*****************************************************************************/
//  Description : Conver record format to audio service record format.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL MMISRVAUD_RECORD_FMT_E ConverRecFmt(MMIAUDIO_RECORD_FORMAT_T format)
{
    if(format < MMIAUDIO_RECORD_FORMAT_MAX)
    {
        return (MMISRVAUD_RECORD_FMT_E)format;
    }
    else
    {
        return MMISRVAUD_RECORD_FMT_ADPCM;
    }
}

/*****************************************************************************/
//  Description : Convert MMIAUDIO record type to audo service record source.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL MMISRVAUD_RECORD_SOURCE_E ConverRecType(MMIAUDIO_RECORD_TYPE_E      record_type)
{
    if(record_type < MMIAUDIO_RECORD_MAX)
    {
        return (MMISRVAUD_RECORD_SOURCE_E)record_type;
    }
    else
    {
        return MMIAUDIO_RECORD_NORMAL_APP;
    }
}

/*****************************************************************************/
//  Description : Convert MMIAUDIO record frame lenght to audo service frame len.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL MMISRVAUD_RECORD_FRAME_LEN_E ConverRecFrameLen(MMIAUDIO_RECORD_FRAME_LENGTH_T frame_length)
{
    if(frame_length < MMIAUDIO_RECORD_FRAME_LEN_MAX)
    {
        return (MMISRVAUD_RECORD_FRAME_LEN_E)frame_length;
    }
    else
    {
        return MMISRVAUD_RECORD_FRAME_LEN_DEFAULT;
    }
}

/*****************************************************************************/
//  Description : Convert mmi audio service result to mmiaudio result.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL MMIAUDIO_RESULT ConvertResult(MMISRVAUD_REPORT_RESULT_E result)
{
    MMIAUDIO_RESULT ret_result = MMIAUDIO_RLT_SUCCESS;
    switch(result)
    {
    case MMISRVAUD_REPORT_RESULT_SUCESS:
        ret_result = MMIAUDIO_RLT_SUCCESS;
        break;
    case MMISRVAUD_REPORT_RESULT_NOT_SUPPORT:
        ret_result = MMIAUDIO_RLT_NOT_SUPPORT;
        break;
    case MMISRVAUD_REPORT_RESULT_ERROR:
        ret_result = MMIAUDIO_RLT_ERROR;
        break;
    case MMISRVAUD_REPORT_RESULT_ERROR_UNKNOWN:
        ret_result = MMIAUDIO_RLT_OTHER;
        break;
    default:
        ret_result = MMIAUDIO_RLT_MAX;
        break;
    }
}

/*****************************************************************************/
//  Description : app notify handle function.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL void HandleNotify(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    #define DO_CALLBACK(if_code, name, handle, res)  \
            if_code##(s_audio_info.##name.handle == handle) \
            {   \
                if(s_audio_info.##name.cb_func != PNULL)    \
                {   \
                    s_audio_info.##name.cb_func(res, PNULL);    \
                    s_audio_info.##name.cb_func = PNULL;    \
                }   \
                s_audio_info.##name.handle = 0; \
                s_audio_info.cur_type = MMIAUDIO_NONE;  \
            }   
    
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    MMIAUDIO_RESULT result = MMIAUDIO_RLT_SUCCESS;
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_470_112_2_18_2_3_27_8,(uint8*)"dd", handle, param));
    
    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        result = ConvertResult((MMISRVAUD_REPORT_RESULT_E)param->data);
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: HandleNotify(), result=%d, report_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_476_112_2_18_2_3_27_9,(uint8*)"dd", result, report_ptr));
        
        if(report_ptr != PNULL)
        {            
            _DBG(            //MMISRV_TRACE_LOW:"[MMISRV]: HandleNotify(), report_ptr->report=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_480_112_2_18_2_3_27_10,(uint8*)"d", report_ptr->report));
        
            switch(report_ptr->report)
            {
            case MMISRVAUD_REPORT_END:
                MMISRVAUD_Stop(handle);
                MMISRVMGR_Free(handle);
                
                DO_CALLBACK(if, app, handle, result)
                DO_CALLBACK(else if, player, handle, result)
                DO_CALLBACK(else if, music, handle, result)
                DO_CALLBACK(else if, kuro, handle, result)
                #ifdef JAVA_SUPPORT    
                DO_CALLBACK(else if, java, handle, result)
                #endif
                DO_CALLBACK(else if, normal, handle, result)
                DO_CALLBACK(else if, dtmf_tone, handle, result)
                DO_CALLBACK(else if, custom_tone, handle, result)
                DO_CALLBACK(else if, generic_tone, handle, result)
                DO_CALLBACK(else if, call, handle, result)
                DO_CALLBACK(else if, call_lb, handle, result)
                DO_CALLBACK(else if, record, handle, result)
                DO_CALLBACK(else if, fm, handle, result)
                break;
            case MMISRVAUD_REPORT_PLAY:
            case MMISRVAUD_REPORT_STOP:
            case MMISRVAUD_REPORT_RESUME:
            case MMISRVAUD_REPORT_PAUSE:
            default:
                break;
            }
        
        }
    }
}
  
/*****************************************************************************/
//  Description : audio init
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_Init(void)
{
    BOOLEAN res = FALSE;
    MMISRV_HANDLE_T handle = 0;
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
        
    //MMISRVMGR_Init();
    //MMISRVAUD_RegistService();

    /*
    InitAudioTrackBuffer();
    */
        
    req.ind_data = 0;
    req.is_async = FALSE;
    req.is_auto_resume_off = FALSE;
    req.notify = HandleNotify;
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = 1;
    audio_srv.all_support_route = MMISRVAUD_ROUTE_NONE;
    audio_srv.info.type = MMISRVAUD_TYPE_TONE_DTMF;
    //s_audio_info.dtmf_tone.handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    audio_srv.info.type = MMISRVAUD_TYPE_TONE_CUSTOM;
    //s_audio_info.custom_tone.handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    audio_srv.info.type = MMISRVAUD_TYPE_TONE_GENERIC;
    //s_audio_info.generic_tone.handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    
    MMIAUDIO_SetCurDevMode(AUDIO_DEVICE_MODE_HANDFREE);
    #ifdef MP3_EQ_SUPPORT 
    MMIAUDIO_SetEQMode(MMIAUDIO_EQ_REGULAR);
    #endif
    MMIAUDIO_SetPlayRate(MMIAUDIO_PLAY_RATE_NORMAL);    
}

/*****************************************************************************/
//  Description : Set the audio volume by device mode.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_SetVolume(AUDIO_DEVICE_MODE_TYPE_E  mode, uint32 speaker_vol)
{
    MMISRVAUD_ROUTE_T route = MMISRVAUD_ROUTE_NONE;
    MMISRVAUD_AUD_CFG_T aud_cfg = {0};

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MMIAUDIO_SetVolume() entry, mode=%d, speaker_vol=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_568_112_2_18_2_3_27_11,(uint8*)"dd", mode, speaker_vol));
    
    if(AUDIO_DEVICE_MODE_MAX <= mode)
    {
        mode = AUDIO_DEVICE_MODE_HANDFREE;
    }
    
    if(MMI_AUDIO_VOLUME_MAX < speaker_vol)
    {
        speaker_vol = MMI_AUDIO_VOLUME_MAX;
    }

    route = ConvertDevMode(mode);

    if(MMISRVAUD_GetAudCfg(&aud_cfg))
    {
        if (route != aud_cfg.route)
        {
            //MMISRVAUD_SetRouteNohandle(route);
            SCI_Sleep(25);
            //MMISRVAUD_SetVolumeNoHandle(speaker_vol);
        }
        else if (speaker_vol != aud_cfg.volume)
        {
            //MMISRVAUD_SetVolumeNoHandle(speaker_vol);
        }
     
        #ifdef PLATFORM_SC6600L
        if(MMIAPIENVSET_GetEnhanceRingState())
        {
            MMIAUDIO_StartAGC();
        }
        #endif    
    }
}

/*****************************************************************************/
//  Description : Is voice loop on.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsInVoiceLoop(
                    void
                    )
{
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MMIAUDIO_IsInVoiceLoop() entry, enable=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_612_112_2_18_2_3_27_12,(uint8*)"d", s_audio_info.is_voice_lb_enable));

    return s_audio_info.is_voice_lb_enable;
}


/*****************************************************************************/
//  Description : Get current volume 
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAUDIO_GetCurVolume(void)
{
    uint32 volume = 0;
//    volume = MMISRVAUD_GetVolumeNoHandle();
    
    return MIN(MMI_AUDIO_VOLUME_MAX, volume);
}

/*****************************************************************************/
//  Description : MMI AUDIO Is AGC Support
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsAGCSupport(
                        void
                        )
{
#ifdef PLATFORM_SC6600L
    return AUD_GetAGC_SupportFlag(AUDIO_AGC_MP3);
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description : API of is is audio playing
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_GetIsPlaying(void)
{
    if (MMIAUDIO_NONE == s_audio_info.cur_type)
    {
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: MMIAUDIO_GetIsPlaying() entry, Is not playing!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_653_112_2_18_2_3_27_13,(uint8*)""));
    
        return FALSE;
    }
    else
    {
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: MMIAUDIO_GetIsPlaying() entry, Is playing!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_659_112_2_18_2_3_27_14,(uint8*)""));
    
        return TRUE;
    }
}

/*****************************************************************************/
//  Description : MMIAUDIO_IsAudioPlayEnd
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsAudioPlayEnd(HAUDIO audio_handle)
{
    BOOLEAN res = FALSE;
    MMISRV_HANDLE_T handle = (MMISRV_HANDLE_T)audio_handle;
    BOOLEAN result = FALSE;
    if(handle > 0)
    {
        res = MMISRVAUD_IsAudioEnd(handle, &result);
        if(res)
        {
            res = result;
        }
    }
    return res;
}

/*****************************************************************************/
//  Description : To judge whether the music is supported for seeking.
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsSeekSupport(
                    MMIAUDIO_TYPE_E audio_type
                    )
{
    BOOLEAN result = FALSE; 
	
    switch(audio_type)
    {
    case MMIAUDIO_MP3:
#ifdef MP3_SEEK_SUPPORT
        result = TRUE;
#endif
        break;
    case MMIAUDIO_WMA:
#ifdef WMA_SEEK_SUPPORT
        result = TRUE;
#endif
        break;
    case MMIAUDIO_AAC:
#ifdef AAC_SEEK_SUPPORT
        result = TRUE;
#endif
        break;
    case MMIAUDIO_AMR:
#ifdef AMR_SEEK_SUPPORT
        result = TRUE;
#endif
        break;
    case MMIAUDIO_MIDI:
#ifdef MIDI_SEEK_SUPPORT
        result = TRUE;
#endif
        break;
    case MMIAUDIO_WAVE:
#ifdef WAV_SEEK_SUPPORT
        result = TRUE;
#endif
        break;
    default:
        break;
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MMIAUDIO_IsSeekSupport() exit, audio type = %d, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_731_112_2_18_2_3_28_15,(uint8*)"dd", audio_type,result));

    return result;
}

#if 0
/*****************************************************************************/
//  Description : Handle audio callback message.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIAUDIO_HandleCallBackMsg(
                    MMIAUDIO_CALLBACK_INFO_T *callback_info_ptr
                    )
{

}
#endif

/*****************************************************************************/
//  Description : audio play 
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioPlay(
                            MMIAUDIO_HTYPE   audio_handle_type,
                            uint32           uiOffset,
                            uint32           play_times,
                            MMIAUDIO_END_CALLBACK_PFUNC end_callback_func
                            )
{
    MMISRV_HANDLE_T handle = GetHandleByType(audio_handle_type);
    MMIAUDIO_TYPE_T *type_ptr = GetTypeInfo(audio_handle_type);
    BOOLEAN res = FALSE;
    
    //MMISRV_TRACE_LOW:"[MMISRV]: MMIAUDIO_AudioPlay() entry, handle=%d, type_ptr=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_764_112_2_18_2_3_28_16,(uint8*)"dd", handle, type_ptr);

    if(handle > 0)
    {
        if(type_ptr != PNULL)
        {
            type_ptr->cb_func = end_callback_func;
        }
        MMISRVAUD_SetPlayTimes(handle, play_times);
        res = MMISRVAUD_Play(handle, uiOffset);
        
        if(res)
        {
            s_audio_info.cur_type = audio_handle_type;
        }
        else
        {
            s_audio_info.cur_type = MMIAUDIO_NONE;        
        }
        
    }
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MMIAUDIO_AudioPlay() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_786_112_2_18_2_3_28_17,(uint8*)"d", res));
    
    return res;
}

/*****************************************************************************/
//  Description : audio play 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioPlayAsy(
                            MMIAUDIO_HTYPE   audio_handle_type,
                            uint32           uiOffset,
                            uint32           play_times,
                            MMIAUDIO_END_CALLBACK_PFUNC end_callback_func
                            )
{
    MMISRV_HANDLE_T handle = GetHandleByType(audio_handle_type);
    MMIAUDIO_TYPE_T *type_ptr = GetTypeInfo(audio_handle_type);
    BOOLEAN res = FALSE;
    
    //MMISRV_TRACE_LOW:"[MMISRV]: MMIAUDIO_AudioPlay() entry, handle=%d, type_ptr=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_806_112_2_18_2_3_28_18,(uint8*)"dd", handle, type_ptr);

    if(handle > 0)
    {
        if(type_ptr != PNULL)
        {
            type_ptr->cb_func = end_callback_func;
        }
        MMISRVAUD_SetPlayTimes(handle, play_times);
        res = MMISRVAUD_PlayAsy(handle, uiOffset);
        
        if(res)
        {
            s_audio_info.cur_type = audio_handle_type;
        }
        else
        {
            s_audio_info.cur_type = MMIAUDIO_NONE;        
        }
        
    }
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MMIAUDIO_AudioPlay() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_828_112_2_18_2_3_28_19,(uint8*)"d", res));
    
    return res;}

/*****************************************************************************/
//  Description : API of audio play pause
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioPause(MMIAUDIO_HTYPE audio_handle_type)
{
    MMISRV_HANDLE_T handle = GetHandleByType(audio_handle_type);
    BOOLEAN res = FALSE;
    if(handle > 0)
    {
    #ifdef SRVAUD_OPT
        res = MMISRVMGR_Suspend(handle);
    #else
        res = MMISRVAUD_Pause(handle);
    #endif
    }
    s_audio_info.cur_type = MMIAUDIO_NONE;

    //MMISRV_TRACE_LOW:"[MMISRV]: MMIAUDIO_AudioPause() exit, handle=%d, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_850_112_2_18_2_3_28_20,(uint8*)"dd", handle, res);
    
    return res;
}

/*****************************************************************************/
//  Description : API of audio play resume
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioResume(MMIAUDIO_HTYPE audio_handle_type)
{
    MMISRV_HANDLE_T handle = GetHandleByType(audio_handle_type);
    BOOLEAN res = FALSE;
    if(handle > 0)
    {
    #ifdef SRVAUD_OPT
        res = MMISRVMGR_Resume(handle);
    #else
        res = MMISRVAUD_Resume(handle);
    #endif
    }

    if(res)
    {
        s_audio_info.cur_type = audio_handle_type;
    }
    else
    {
        s_audio_info.cur_type = MMIAUDIO_NONE;        
    }

    //MMISRV_TRACE_LOW:"[MMISRV]: MMIAUDIO_AudioResume() exit, handle=%d, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_881_112_2_18_2_3_28_21,(uint8*)"dd", handle, res);
    
    return res;
}

/*****************************************************************************/
//  Description : stop audio play 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_AudioStop(MMIAUDIO_HTYPE audio_handle_type)
{
    MMISRV_HANDLE_T handle = GetHandleByType(audio_handle_type);
    MMIAUDIO_TYPE_T *type_ptr = PNULL;
    BOOLEAN res = FALSE;
    if(handle > 0)
    {
        res = MMISRVAUD_Stop(handle);
        res = MMISRVMGR_Free(handle);
        if(res)
        {
            type_ptr = GetTypeInfo(audio_handle_type);
            if(type_ptr != PNULL)
            {
                type_ptr->cb_func = PNULL;
                type_ptr->handle = 0;
            }
        }
        s_audio_info.cur_type = MMIAUDIO_NONE;
    }

    //MMISRV_TRACE_LOW:"[MMISRV]: MMIAUDIO_AudioStop() exit, handle=%d, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_911_112_2_18_2_3_28_22,(uint8*)"dd", handle, res);
    
}

/*****************************************************************************/
//  Description : API of audio play seek
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioSeek(
                                  MMIAUDIO_HTYPE    audio_handle_type,
                                  uint32            uiOffset
                                  )
{
    MMISRV_HANDLE_T handle = GetHandleByType(audio_handle_type);
    BOOLEAN res = FALSE;
    if(handle > 0)
    {
        res = MMISRVAUD_Seek(handle, uiOffset);
    }

    //MMISRV_TRACE_LOW:"[MMISRV]: MMIAUDIO_AudioSeek() exit, handle=%d, offset, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_931_112_2_18_2_3_28_23,(uint8*)"dd", handle, uiOffset, res);
    
    return res;
}

/*****************************************************************************/
//  Description : Seek by time offset(ms).
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioSeekByTime(
                        MMIAUDIO_HTYPE audio_handle_type, 
                        uint32 time_offset
                        )
{
    MMISRV_HANDLE_T handle = GetHandleByType(audio_handle_type);
    MMISRVAUD_PLAY_INFO_T audio_info = {0};
    uint32  byte_offset = FALSE;
    BOOLEAN res = FALSE;
    
    if(handle > 0)
    {
        if(MMISRVAUD_GetPlayingInfo(handle, &audio_info))
        {
            if(audio_info.total_time > 0 && audio_info.total_data_length> 0)
            {
                byte_offset = (uint32)((uint64)time_offset * audio_info.total_data_length / audio_info.total_time);
                res = MMISRVAUD_Seek(handle, byte_offset);
            }
        }
    }
    //MMISRV_TRACE_LOW:"[MMISRV]: MMIAUDIO_AudioSeekByTime() exit, handle=0x%x, time_offset=%d, byte_offset=%d, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_961_112_2_18_2_3_28_24,(uint8*)"dddd", handle, time_offset, byte_offset, res);

    return res;
}

/*****************************************************************************/
//  Description : play tone 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_PlayTone(
                MMIAUDIO_TYPE_E tone_type,
                uint32 tone_id,
                uint32 duration,
                uint32 freq
                )
{
    MMISRV_HANDLE_T handle = 0;
    MMISRVAUD_TYPE_U type_info = {0};
    BOOLEAN res = FALSE;
    
    //MMISRV_TRACE_LOW:"[MMISRV]: MMIAUDIO_PlayTone() entry, handle=%d, tone_type=%d, tone_id=%d, duration=%d,freq=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_982_112_2_18_2_3_28_25,(uint8*)"ddddd", handle, tone_type, duration, freq);

    switch(tone_type)
    {
    case MMIAUDIO_GENERIC_TONE:
        type_info.type = MMISRVAUD_TYPE_TONE_GENERIC;
        type_info.tone_generic.duation = duration;
        type_info.tone_generic.type = tone_type;
        type_info.tone_generic.id = tone_id;
        handle = GetHandleByType(MMIAUDIO_HTYPE_GENERIC_TONE);
        break;
    case MMIAUDIO_CUSTOM_TONE:
        type_info.type = MMISRVAUD_TYPE_TONE_CUSTOM;
        type_info.tone_custom.duation = duration;
        type_info.tone_generic.type = tone_type;
        type_info.tone_custom.freq = freq;
        handle = GetHandleByType(MMIAUDIO_HTYPE_CUSTOM_TONE);            
        break;
    case MMIAUDIO_DTMF_TONE:
        type_info.type = MMISRVAUD_TYPE_TONE_DTMF;
        type_info.tone_dtmf.duation = duration;
        type_info.tone_dtmf.type = tone_type;
        type_info.tone_dtmf.id = tone_id;
        handle = GetHandleByType(MMIAUDIO_HTYPE_DTMF_TONE);            
        break;
    default:
        return;
    }
    
    if(handle > 0)
    {
        MMISRVAUD_SetAudioInfo(handle, &type_info);
        res = MMISRVAUD_Play(handle, 0);
    }
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MMIAUDIO_PlayTone() exit, handle=0x%x, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_1017_112_2_18_2_3_28_26,(uint8*)"dd", handle, res));
        
}

/*****************************************************************************/
//  Description : tone stop 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_StopTone(
                MMIAUDIO_TYPE_E tone_type
                )
{
    MMISRV_HANDLE_T handle = 0;
    BOOLEAN res = FALSE;

    switch(tone_type)
    {
    case MMIAUDIO_GENERIC_TONE:
        handle = GetHandleByType(MMIAUDIO_HTYPE_GENERIC_TONE);
        break;
    case MMIAUDIO_CUSTOM_TONE:
        handle = GetHandleByType(MMIAUDIO_HTYPE_CUSTOM_TONE);            
        break;
    case MMIAUDIO_DTMF_TONE:
        handle = GetHandleByType(MMIAUDIO_HTYPE_DTMF_TONE);            
        break;
    default:
        return;;
    }
    
    if(handle > 0)
    {
        res = MMISRVAUD_Stop(handle);
    }

    //MMISRV_TRACE_LOW:"[MMISRV]: MMIAUDIO_StopTone() exit, handle=%d, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_1052_112_2_18_2_3_28_27,(uint8*)"dd", handle, res);
    
}

#define MMIAUDIO_IQ_DATA_BUF_SIZE     (300<<8)
LOCAL HAUDIO s_record_handle;//定义record handle
LOCAL uint32* s_IQ_data_buf           = PNULL;      //IQ DATA buf
/*****************************************************************************/
//  Description : API of start IQ Data Record
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_StartIQData(
                        const wchar *name_ptr, 
                        RECORD_PARAM_T *record_param
                        )
{
#if 0
    s_audio_info.cur_type = MMIAUDIO_APP;
    return FALSE;
#endif
    HAUDIOCODEC hAudioCodec = 0;
    const uint16* pusCodecName = PNULL;
    HAUDIODEV hDevice = 0;
    const uint16* pusDevName = PNULL;
    uint32 result = 0;

    SCI_ASSERT(PNULL != record_param);
    SCI_ASSERT(PNULL != name_ptr);

    hAudioCodec = hRecordCodec;
    hDevice = hDSPDATADEVICE;
    
    s_audio_info.cur_type = MMIAUDIO_APP;

    s_IQ_data_buf = SCI_ALLOC_APP( MMIAUDIO_IQ_DATA_BUF_SIZE << 2);  //300K
    SCI_PASSERT(PNULL != s_IQ_data_buf,("[MMIAUDIO]:MMIAUDIO_StartIQData BUFFER ALLOC FAILED!"));

    s_record_handle = AUDIO_CreateRecordFileHandle(
                                hAudioCodec,
                                pusCodecName,
                                hDevice,
                                pusDevName,
                                (const int16 *)name_ptr,
                                s_IQ_data_buf,
                                MMIAUDIO_IQ_DATA_BUF_SIZE,
                                PNULL
                                );

    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_StartIQData:s_record_handle=%lx"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_1100_112_2_18_2_3_28_28,(uint8*)"d", s_record_handle);

    if (PNULL == s_record_handle)
    {
        if(PNULL != s_IQ_data_buf)
        {
            SCI_FREE(s_IQ_data_buf);
        }
        return FALSE;
    }

    AUDIO_ExeCodecExtOpe(
            s_record_handle,
            ANSI2UINT16("SET_RECORDPARAM"),//扩展操作的标志 
            RECORD_PARAM_SET_COUNT,
            record_param,
            &result
            );

    //AUDIO_StartRecord(s_record_handle);
    AUDIO_Play(s_record_handle, 0);
    return TRUE;
    
}

/*****************************************************************************/
//  Description : API of start record
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_StartRecord(
                    const wchar     *name_ptr,
                    MMIFILE_HANDLE  file_handle,
                    MMIAUDIO_RECORD_FORMAT_T format,
                    MMIAUDIO_RECORD_FRAME_LENGTH_T frame_length,
                    MMIAUDIO_RECORD_TYPE_E      record_type,
                    MMIAUDIO_END_CALLBACK_PFUNC end_callback_func
                    )
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;
    BOOLEAN res = FALSE;
    
    s_audio_info.fm.handle = 0;
    
    req.ind_data = 0;
    req.is_async = FALSE;
    req.is_auto_resume_off = FALSE;
    req.notify = HandleNotify;
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = 1;
    audio_srv.all_support_route = MMISRVAUD_ROUTE_NONE;
    
    audio_srv.info.record_file.type = MMISRVAUD_TYPE_RECORD_FILE;
    audio_srv.info.record_file.name = name_ptr;
    audio_srv.info.record_file.name_len = MMIAPICOM_Wstrlen(name_ptr);    
    audio_srv.info.record_file.fmt = ConverRecFmt(format);
    audio_srv.info.record_file.source = ConverRecType(record_type);
    audio_srv.info.record_file.frame_len = ConverRecFrameLen(frame_length);
        
    handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    if(handle > 0)
    {
        res = MMISRVAUD_Play(handle, 0);
        
        if(!res)
        {
            MMISRVMGR_Free(handle);
        }
        else
        {
            s_audio_info.record.handle = handle;
            s_audio_info.record.cb_func = end_callback_func;
            s_audio_info.cur_type = MMIAUDIO_APP;
        }
    }

    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_StartRecord exit: handle=0x%x, res = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_1181_112_2_18_2_3_28_29,(uint8*)"dd", handle, res);

    return res;
}

/*****************************************************************************/
//  Description : API of end record (close handle)
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_EndRecord(void)
{
    MMISRV_HANDLE_T handle = GetHandleByType(MMIAUDIO_HTYPE_RECORD);
    BOOLEAN res = TRUE;

    if(handle > 0)
    {
        res = MMISRVAUD_Stop(handle);
        res = MMISRVMGR_Free(handle);
    }

    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_EndRecord exit: handle=0x%x, res = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_1201_112_2_18_2_3_28_30,(uint8*)"dd", handle, res);

    return res;
}

/*****************************************************************************/
//  Description : API of start record
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_StopRecord(void)
{
    MMISRV_HANDLE_T handle = GetHandleByType(MMIAUDIO_HTYPE_RECORD);
    MMIAUDIO_TYPE_T *info = PNULL;
    BOOLEAN res = FALSE;

    if(handle > 0)
    {
        res = MMISRVAUD_Stop(handle);
        res = MMISRVMGR_Free(handle);        
        info = GetTypeInfo(MMIAUDIO_HTYPE_RECORD);
        if(info != PNULL)
        {
            info->handle = 0;
            info->cb_func = PNULL;
        }
    }

    if(MMIAUDIO_APP == s_audio_info.cur_type &&
       0 == s_audio_info.app.handle)
    {
        s_audio_info.cur_type = MMIAUDIO_NONE;
    }

    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_StopRecord exit: handle=0x%x, res = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_1234_112_2_18_2_3_29_31,(uint8*)"dd", handle, res);

    return res;
}

/*****************************************************************************/
//  Description : API of pause record
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_PauseRecord(void)
{
    MMISRV_HANDLE_T handle = GetHandleByType(MMIAUDIO_HTYPE_RECORD);
    BOOLEAN res = FALSE;

    if(handle > 0)
    {
    #ifdef SRVAUD_OPT
        res = MMISRVMGR_Suspend(handle);
    #else
        res = MMISRVAUD_Pause(handle);
    #endif
    }

    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_PauseRecord exit: handle=0x%x, res = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_1257_112_2_18_2_3_29_32,(uint8*)"dd", handle, res);

    return res;
}

/*****************************************************************************/
//  Description : API of resume record
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_ResumeRecord(void)
{
    MMISRV_HANDLE_T handle = GetHandleByType(MMIAUDIO_HTYPE_RECORD);
    BOOLEAN res = FALSE;

    if(handle > 0)
    {
    #ifdef SRVAUD_OPT
        res = MMISRVMGR_Resume(handle);
    #else    
        res = MMISRVAUD_Resume(handle);
    #endif
    }

    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_ResumeRecord exit: handle=0x%x, res = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_1280_112_2_18_2_3_29_33,(uint8*)"dd", handle, res);

    return res;
}

/*****************************************************************************/
//  Description : MMIAUDIO Play FM
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_PlayFM(
                    void
                    )
{

    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;
    BOOLEAN res = FALSE;
    
    s_audio_info.fm.handle = 0;
    
    req.ind_data = 0;
    req.is_async = FALSE;
    req.is_auto_resume_off = FALSE;
    req.notify = HandleNotify;
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = 1;
    audio_srv.all_support_route = MMISRVAUD_ROUTE_NONE;
    audio_srv.info.fm.type = MMISRVAUD_TYPE_FM;
        
    handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    if(handle > 0)
    {
        res = MMISRVAUD_Play(handle, 0);
        
        if(!res)
        {
            MMISRVMGR_Free(handle);
        }
        else
        {
            s_audio_info.fm.handle = handle;
            s_audio_info.fm.cb_func = PNULL;
        }
    }

    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_PlayFM exit: handle=0x%x, res = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_1330_112_2_18_2_3_29_34,(uint8*)"dd", handle, res);

    return res;
}

/*****************************************************************************/
//  Description : MMIAUDIO Stop FM
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_StopFM(
                    void
                    )
{
    MMISRV_HANDLE_T handle = GetHandleByType(MMIAUDIO_HTYPE_FM);
    BOOLEAN res = FALSE;
    
    if(handle > 0)
    {
        res = MMISRVAUD_Stop(handle);

        res = MMISRVMGR_Free(handle);

        if(res)
        {
            s_audio_info.fm.handle = 0;
            s_audio_info.fm.cb_func = PNULL;
        }
    }

    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_StopFM exit: handle=0x%x, res = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_1359_112_2_18_2_3_29_35,(uint8*)"dd", handle, res);

    return;
}

/*****************************************************************************/
//  Description : set mute 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_SetMute(BOOLEAN is_mute)
{
    //return MMISRVAUD_SetMuteNohandle(is_mute);
    return FALSE;
}

/*****************************************************************************/
//  Description : Get mute 
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_GetMute(void)
{
    BOOLEAN is_mute = FALSE;
    BOOLEAN res = FALSE;//MMISRVAUD_GetMuteNohandle(&is_mute);
    if(res)
    {
        return is_mute;
    }
    else
    {
        /* if error, return FALSE */
        return FALSE;
    }
}

PUBLIC void MMIAUDIO_SetPlayRate(
                MMIAUDIO_PLAY_RATE_E rate
                )
{
#ifdef AUDIO_RATE_SUPPORT
    MMISRVAUD_PLAY_RATE_E srv_rate = ConvertRate(rate);
    MMISRVAUD_SetPlayRate(srv_rate);
#endif
}

#ifdef MP3_EQ_SUPPORT 
/*****************************************************************************/
//  Description : set eq mode
//  Global resource dependence : hEQExp
//  Author: haiyang.hu
//  Note: 0—低音,1—高音,2—声乐,3—摇滚,4—流行,5—古典,6—强劲,7—常规
/*****************************************************************************/
PUBLIC void MMIAUDIO_SetEQMode(MMIAUDIO_EQ_MODE_E eq_mode)
{  
    BOOLEAN res = FALSE;
    MMISRVAUD_EQ_MODE_E eq = MMISRVAUD_EQ_MODE_INVALID;
    eq = ConvertEqMode(eq_mode);
    if(eq < MMISRVAUD_EQ_MODE_MAX)
    {
        res = FALSE;//MMISRVAUD_SetEQModeNoHandle(eq);
    }
    //MMISRV_TRACE_LOW:"[MMISRV]: MMIAUDIO_SetEQMode() exit, eq_mode=%d, eq=%d, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_1417_112_2_18_2_3_29_36,(uint8*)"ddd", eq_mode, eq, res);
    
}
#endif

/*****************************************************************************/
//  Description : Set half samplerate.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_SetHalfSamplerate(
                BOOLEAN is_set_half
                )
{
#ifndef _WIN32
#ifndef NANDBOOT_SUPPORT
    BOOLEAN res = FALSE;
    MMISRV_HANDLE_T handle = GetHandleByType(MMIAUDIO_PLAYER);
    if(handle > 0)
    {
        res = MMISRVAUD_SetHalfSampleRate(handle);
    }
    return res;
#endif
#endif
}

/*****************************************************************************/
//  Description : Reset the device mode and volume, regardless of the original device mode and volume.
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_SetVolumeDeviceModeExt(AUDIO_DEVICE_MODE_TYPE_E  mode, uint32 speaker_vol)
{
    /* these two functions is the same */
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MMIAUDIO_SetVolumeDeviceModeExt() entry"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_1450_112_2_18_2_3_29_37,(uint8*)""));

    MMIAUDIO_SetVolume(mode, speaker_vol);
}

/*****************************************************************************/
//  Description : set up link mute 
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_SetUpLinkMute(BOOLEAN is_mute)
{
    BOOLEAN res = FALSE;

    //res = MMISRVAUD_SetUpLinkMute(is_mute);

    return res;
}

/*****************************************************************************/
//  Description : set device mode for call
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_SerDeviceModeInCall(
                    AUDIO_DEVICE_MODE_TYPE_E mode
                    )
{
    BOOLEAN res = FALSE;
    MMISRV_HANDLE_T handle = GetHandleByType(MMIAUDIO_HTYPE_CALL);
    MMISRVAUD_ROUTE_T route = ConvertDevMode(mode);

    if(handle > 0 && route != MMISRVAUD_ROUTE_NONE)
    {
        res = MMISRVAUD_SetRoute(handle, route);
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MMIAUDIO_SerDeviceModeInCall() exit, mode=%d,handle=0x%x,res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_1485_112_2_18_2_3_29_38,(uint8*)"ddd", mode, handle, res));
    
    return res;
}

/*****************************************************************************/
//  Description : set current device mode 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_SetCurDevMode(AUDIO_DEVICE_MODE_TYPE_E mode)
{
    MMISRVAUD_ROUTE_T route = MMISRVAUD_ROUTE_NONE;
    route = ConvertDevMode(mode);
    //MMISRVAUD_SetRouteNohandle(route);
}

/*****************************************************************************/
//  Description : MMIAUDIO SwitchDevice
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_SwitchDeviceExt(
                    MMIAUDIO_HTYPE          audio_handle_type,
                    MMIAUDIO_DEVICE_TYPE_E  device
                    )
{
    BOOLEAN res = FALSE;
    MMISRV_HANDLE_T handle =0;
    MMISRVAUD_ROUTE_T route = MMISRVAUD_ROUTE_NONE;
    
    handle = GetHandleByType(audio_handle_type);
    
    if(handle > 0)
    {
        switch(device)
        {
        case MMIAUDIO_DEVICE_BT:
            route = MMISRVAUD_ROUTE_BLUETOOTH;
            break;
        default:
            break;
        }

        res = MMISRVAUD_SetRoute(handle, route);
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MMIAUDIO_SwitchDeviceExt() exit, handle=%d, device=%d,res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_1530_112_2_18_2_3_29_39,(uint8*)"ddd", handle, device, res));
    
    return res;
}

/*****************************************************************************/
//  Description : API of is get audio handle
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC HAUDIO MMIAUDIO_GetAudioHandle(MMIAUDIO_HTYPE audio_handle_type)
{
    HAUDIO handle = (HAUDIO)GetHandleByType(audio_handle_type);
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: MMIAUDIO_GetAudioHandle() exit, handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_1542_112_2_18_2_3_29_40,(uint8*)"d", handle));
    return handle;
}

/*****************************************************************************/
//  Description : MMI AUDIO Get Spectrum Info
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_GetSpectrum(
                    MMIAUDIO_HTYPE  audio_handle_type,
                    uint16          * spec_buf_ptr,         //[OUT]
                    uint16          * spec_num_ptr,         //[OUT] 目前长度无法验证，约定为32 或 33，即buf长度需要大于 32*sizeof(uint16),需要结构体
                    uint16          * max_spec_value_ptr    //[OUT]
                    )
{
    BOOLEAN res = FALSE;
    MMISRVAUD_SPECTRUM_INFO_T info = {0};
    MMISRV_HANDLE_T handle = GetHandleByType(audio_handle_type);
    
    info.max_spec_value_ptr = max_spec_value_ptr;
    info.spec_buf_ptr = spec_buf_ptr;
    info.spec_num_ptr = spec_num_ptr;

    if(handle != 0)
    {
        res = MMISRVAUD_GetSpectrum(handle, &info);
    }

    return res;
}

/*****************************************************************************/
//  Description : API of is permit audio playing
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 播放优先级:MMIAUDIO_APP>MMIAUDIO_PLAYER>MMIAUDIO_NPRMAL
//          同一优先级的允许中断前一播放
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_GetIsPermitPlay(MMIAUDIO_HTYPE audio_htype)
{
    //SCI_TRACE_LOW:"[MMISRV]:MMIAUDIO_GetIsPermitPlay:cur_type=%d, audio_htype=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_1580_112_2_18_2_3_29_41,(uint8*)"dd", s_audio_info.cur_type, audio_htype);

    if (MMIAUDIO_NONE == s_audio_info.cur_type)
    {
        return TRUE;
    }
    else if (audio_htype <= s_audio_info.cur_type)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : API of is get current play audio handle type
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC MMIAUDIO_HTYPE MMIAUDIO_GetAudioHtype(void)
{
    //SCI_TRACE_LOW:"[MMISRV]:MMIAUDIO_GetAudioHtype:cur_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_1602_112_2_18_2_3_29_42,(uint8*)"d", s_audio_info.cur_type);

    return s_audio_info.cur_type;
}

/*****************************************************************************/
//  Description : get current device mode 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC AUDIO_DEVICE_MODE_TYPE_E MMIAUDIO_GetCurDevMode(void)
{
    MMISRVAUD_ROUTE_T route = MMISRVAUD_ROUTE_NONE;
    AUDIO_DEVICE_MODE_TYPE_E mode = 0;
    route = 0;//MMISRVAUD_GetRouteNoHandle();
    mode = ConvertRoute(route);
    return mode;
}

/*****************************************************************************/
//  Description : Get the record eclipse time (count in ms).
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAUDIO_GetRecordEclipseTime(
                void
                )
{
    BOOLEAN res = FALSE;
    uint32 time = 0;
    MMISRV_HANDLE_T handle = s_audio_info.record.handle;
    
    if(handle > 0)
    {
        MMISRVAUD_RECORD_INFO_T info = {0};
        res = MMISRVAUD_GetRecordInfo(handle, &info);
        if(res)
        {
            time = info.total_time;
        }
    }
    return time;
}

/*****************************************************************************/
//  Description : API of create audio file handle
//  Global resource dependence : none
//  Author: 
//  Note: SUCCESS - return the audio handle created
//        FAILURE - return SCI_NULL
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_CreateAudioFileHandle( 
                        BOOLEAN is_a2dp,
                        MMIAUDIO_HTYPE audio_handle_type,
                        MMIAUDIO_TYPE_E ring_type,          // the codec handle which will be used.
                        const wchar *name_ptr               // file name ptr
                        )
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;
    
    req.ind_data = 0;
    req.is_async = FALSE;
    req.is_auto_resume_off = FALSE;
    req.notify = HandleNotify;
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = 1;
    audio_srv.info.ring_file.type = MMISRVAUD_TYPE_RING_FILE;
    audio_srv.info.ring_file.fmt = ConvertTypeToFmt(ring_type);
    audio_srv.info.ring_file.name = name_ptr;
    audio_srv.info.ring_file.name_len = MMIAPICOM_Wstrlen(name_ptr);
    audio_srv.all_support_route = MMISRVAUD_ROUTE_AUTO;
    handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    if(handle > 0)
    {
        MMIAUDIO_TYPE_T *type_ptr = GetTypeInfo(audio_handle_type);
        if(type_ptr != PNULL)
        {
            type_ptr->handle = handle;
            type_ptr->cb_func = PNULL;
            SetCurType(audio_handle_type);
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : close audio handle 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_CloseAudioHandle(MMIAUDIO_HTYPE audio_handle_type)
{
    MMISRV_HANDLE_T handle = 0;
    BOOLEAN res = FALSE;
    MMIAUDIO_TYPE_T *type_ptr = PNULL;
    handle = GetHandleByType(audio_handle_type);
    
    if(handle > 0)
    {
        res = MMISRVMGR_Free(handle);
    }
    
    type_ptr = GetTypeInfo(audio_handle_type);
    if(type_ptr != PNULL)
    {
        type_ptr->cb_func = PNULL;
        type_ptr->handle = 0;
    }
    s_audio_info.cur_type = MMIAUDIO_NONE;
    
    return res;
}

/*****************************************************************************/
//  Description : API of create audio buffer play handle
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_CreateAudioBufHandle(
                     BOOLEAN is_a2dp,
                     MMIAUDIO_HTYPE audio_handle_type,   //the type of audio handle
                     MMIAUDIO_TYPE_E ring_type,    // the codec handle which will be used.
                     const uint8* pucData,   // the music data pointer
                     uint32 uiDataLength    // the music data length
                     )
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;
    
    req.ind_data = 0;
    req.is_async = FALSE;
    req.is_auto_resume_off = FALSE;
    req.notify = HandleNotify;
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = 1;
    audio_srv.info.ring_buf.type = MMISRVAUD_TYPE_RING_BUF;
    audio_srv.info.ring_buf.fmt = ConvertTypeToFmt(ring_type);
    audio_srv.info.ring_buf.data = pucData;
    audio_srv.info.ring_buf.data_len = uiDataLength;

    audio_srv.all_support_route = MMISRVAUD_ROUTE_AUTO;
    handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    if(handle > 0)
    {
        MMIAUDIO_TYPE_T *type_ptr = GetTypeInfo(audio_handle_type);
        if(type_ptr != PNULL)
        {
            type_ptr->handle = handle;
            type_ptr->cb_func = PNULL;
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : API of create audio streaming buffer play handle
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_CreateAudioStreamingHandle(
                     BOOLEAN            is_a2dp,
                     MMIAUDIO_HTYPE     audio_handle_type,  //the type of audio handle
                     MMIAUDIO_TYPE_E    ring_type,          // the codec handle which will be used.
                     const uint8        *pucData,           // the music data pointer
                     uint32             uiDataLength,       // the music data length
                     AUDIO_GETSRCDATA_CALLBACK_PFUNC AudioGetSrcDataCallback
                     )
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;
    
    req.ind_data = 0;
    req.is_async = FALSE;
    req.is_auto_resume_off = FALSE;
    req.notify = HandleNotify;
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = 1;
    audio_srv.info.streaming.type = MMISRVAUD_TYPE_STREAMING;
    audio_srv.info.streaming.data = pucData;
    audio_srv.info.streaming.data_len = uiDataLength;
    audio_srv.info.streaming.fmt = ConvertTypeToFmt(ring_type);
    audio_srv.info.streaming.cb = (void *)AudioGetSrcDataCallback;

    audio_srv.all_support_route = MMISRVAUD_ROUTE_AUTO;
    handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    if(handle > 0)
    {
        MMIAUDIO_TYPE_T *type_ptr = GetTypeInfo(audio_handle_type);
        if(type_ptr != PNULL)
        {
            type_ptr->handle = handle;
            type_ptr->cb_func = PNULL;
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : API of audio play get playing file info
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioGetPlayingFileInfo(MMIAUDIO_HTYPE audio_handle_type, AUDIO_PLAYINFO_T* ptPlayInfo)
{
    MMISRVAUD_PLAY_INFO_T info = {0};
    MMISRV_HANDLE_T handle = GetHandleByType(audio_handle_type);
    BOOLEAN res = FALSE;
    if(handle > 0)
    {
        if(MMISRVAUD_GetPlayingInfo(handle, &info))
        {
            ptPlayInfo->uiTotalTime = info.total_time;
            ptPlayInfo->uiCurrentTime = info.cur_time;
            ptPlayInfo->uiTotalDataLength = info.total_data_length;
            ptPlayInfo->uiCurrentDataOffset = info.cur_data_offset;
            ptPlayInfo->pvOtherInfo = PNULL;
            res = TRUE;
        }
    }
    return res;
}

/*****************************************************************************/
//  Description : API of audio get file info(cann't callback during play, only use after create handle)
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioGetFileInfo(MMIAUDIO_HTYPE audio_handle_type, AUDIO_FORMATINFO_T* ptfileInfo)
{
    MMISRVAUD_CONTENT_INFO_T info = {0};
    MMISRV_HANDLE_T handle = GetHandleByType(audio_handle_type);
    BOOLEAN res = FALSE;
    if(handle > 0)
    {
        if(MMISRVAUD_GetContentInfo(handle, &info))
        {
            ptfileInfo->hCodecHandle = 0;
            ptfileInfo->uiTotalDataLength = info.total_data_length;
            ptfileInfo->uiTotalTime = info.total_time;
            ptfileInfo->uiSamplerate = info.samplerate;
            ptfileInfo->uiChannelNum = info.channel_num;
            ptfileInfo->uiBitrate = info.bitrate;
            ptfileInfo->uiLayer = 0;
            ptfileInfo->eChannelMode = 0;
            
            ptfileInfo->tAudioId3Tag.tag_code_type = info.id3_tag.tag_code_type;
            strncpy(ptfileInfo->tAudioId3Tag.year, info.id3_tag.year, 4);
            ptfileInfo->tAudioId3Tag.res0  = info.id3_tag.res0;
            ptfileInfo->tAudioId3Tag.res1 = info.id3_tag.res1;
            strncpy(ptfileInfo->tAudioId3Tag.tag, info.id3_tag.tag, 3);
            strncpy(ptfileInfo->tAudioId3Tag.title, info.id3_tag.title, 30);
            strncpy(ptfileInfo->tAudioId3Tag.artist, info.id3_tag.artist, 30);
            strncpy(ptfileInfo->tAudioId3Tag.album, info.id3_tag.album, 30);
            strncpy(ptfileInfo->tAudioId3Tag.comment, info.id3_tag.comment, 30);
            strncpy(ptfileInfo->tAudioId3Tag.genre, info.id3_tag.genre, 30);
            strncpy(ptfileInfo->tAudioId3Tag.content, info.id3_tag.content, 30);
            strncpy(ptfileInfo->tAudioId3Tag.track, info.id3_tag.track, 30);
            ptfileInfo->ucFormatInfo[0] = '\0';
            ptfileInfo->pvOtherInfo = PNULL;
            
            res = TRUE;
        }
    }
    return res;
}

#if 0
/*****************************************************************************/
//  Description : Get file info by file name without audio type.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_GetFileInfo(wchar *file_name, MMIAUDIO_TYPE_E audio_type, AUDIO_FORMATINFO_T *ptfileInfo)
{
    HAUDIO handle = 0;
    BOOLEAN res = FALSE;
    HAUDIOCODEC hAudioCodec = 0;
    HAUDIODEV hDevice = 0;
    
    hAudioCodec = GetAudioCodec(audio_type);
    hDevice = GetAudioDevice(FALSE, audio_type);

    handle = AUDIO_CreateFileHandle(
                        hAudioCodec,
                        PNULL,
                        hDevice, 
                        PNULL,
                        (const int16 *)file_name,
                        s_application_track_buf,
                        MMIAUDIO_TRACKBUFSIZE_APP,
                        PNULL
                        );

    if(handle > 0)
    {
        res = AUDIO_IdentifyAudioFormat(handle, ptfileInfo);
        AUDIO_CloseHandle(handle);
    }
    return res;
}
#endif

/*****************************************************************************/
//  Description : MMI AUDIO Open Spectrum
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_OpenSpectrum(MMIAUDIO_HTYPE audio_handle_type)
{
    BOOLEAN res = FALSE;
    MMISRV_HANDLE_T handle = GetHandleByType(audio_handle_type);
    if(handle > 0)
    {
        res = MMISRVAUD_OpenSpectrum(handle);
    }
    return res;
}

/*****************************************************************************/
//  Description : MMI AUDIO Close Spectrum
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_CloseSpectrum(MMIAUDIO_HTYPE audio_handle_type)
{
    BOOLEAN res = FALSE;
    MMISRV_HANDLE_T handle = GetHandleByType(audio_handle_type);
    if(handle > 0)
    {
        res = MMISRVAUD_CloseSpectrum(handle);
    }
    return res;
}

/*****************************************************************************/
//  Description : enable/disable voice loop.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_EnableVoiceLoop(
                    BOOLEAN is_loop_enable,
                    BOOLEAN is_force_dev_mode,
                    AUDIO_DEVICE_MODE_TYPE_E force_dev_mode,
                    uint8   volume
                    )
{
    AUDIO_DEVICE_MODE_TYPE_E dev_mode = AUDIO_DEVICE_MODE_MAX;
    MMISRV_HANDLE_T  handle = GetHandleByType(MMIAUDIO_HTYPE_CALL_LB);

    if(0 == handle)
    {
        return;
    }

    /* 为了兼容老的机制*/
    hVoiceLoopbackRing = *((HAUDIO *)(handle + 4));
    /* 兼容结束*/
    
    s_audio_info.is_voice_lb_enable = is_loop_enable;

    if(is_loop_enable)
    {
        
        if(is_force_dev_mode &&
           (AUDIO_DEVICE_MODE_EARPHONE == force_dev_mode || AUDIO_DEVICE_MODE_HANDHOLD == force_dev_mode))
        {
            dev_mode = force_dev_mode;
        }
        else
        {
            if (GPIO_CheckHeadsetStatus()) 
            {
                dev_mode = AUDIO_DEVICE_MODE_EARPHONE;
            }
            else
            {
                dev_mode = AUDIO_DEVICE_MODE_HANDHOLD;
            }
        }
        MMIAUDIO_SetVolumeDeviceModeExt(dev_mode, volume);
        MMISRVAUD_SetVoiceLB(handle, TRUE);
        MMIAUDIO_SetVolumeDeviceModeExt(dev_mode, volume);
    }
    else
    {
        MMISRVAUD_SetVoiceLB(handle, FALSE);
    }
}

/*****************************************************************************/
//  Description : enable/disable voice codec.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_EnableVoiceCodec(
                    BOOLEAN is_enable
                    )
{
    BOOLEAN res = FALSE;
    MMISRV_HANDLE_T handle = 0;
    
    if(is_enable)
    {      
        MMISRVMGR_SERVICE_REQ_T req = {0};
        MMISRVAUD_TYPE_T audio_srv = {0};
        
        req.ind_data = 0;
        req.is_async = FALSE;
        req.is_auto_resume_off = FALSE;
        req.notify = HandleNotify;
        req.pri = MMISRVAUD_PRI_HIGH;

        audio_srv.duation = 0;
        audio_srv.eq_mode = 0;
        audio_srv.is_mixing_enable = TRUE;
        audio_srv.play_times = 1;
        audio_srv.all_support_route = MMISRVAUD_ROUTE_NONE;
        audio_srv.default_route = MMISRVAUD_ROUTE_EARPHONE;        
        audio_srv.info.voice.type = MMISRVAUD_TYPE_VOICE;
    
        handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, (void *)&audio_srv);
        if(handle > 0)
        {
            res = MMISRVAUD_Play(handle, 0);
            s_audio_info.call.handle = handle;
            s_audio_info.call.cb_func = PNULL;
        }
    }
    else
    {
        handle = GetHandleByType(MMIAUDIO_HTYPE_CALL);
        if(handle > 0)
        {
            MMISRVAUD_Stop(handle);
            res = MMISRVMGR_Free(handle);
            s_audio_info.call.handle = 0;
            s_audio_info.call.cb_func = PNULL;
        }
    }
    
    return res;
}

/*****************************************************************************/
//  Description : Get the record voidce DB.
//  Global resource dependence : none
//  Author: chunyou.zhu
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAUDIO_GetRecordVoiceDB(
                void
                )
{
    return 0;
}

/*****************************************************************************/
//  Description : Set midi param to low chord.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_SetMidiLowPolyNum(
                    MMIAUDIO_HTYPE audio_htype,
                    BOOLEAN is_low_poly_num
                    )
{
}

/*****************************************************************************/
//  Description : MMIAUDIO Start AGC
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_StartAGC(
                        void
                        )
{
#ifdef PLATFORM_SC6600L
/* nothing to be done */

#endif
}

/*****************************************************************************/
//  Description : MMIAUDIO Stop AGC
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_StopAGC(
                        void
                        )
{
#ifdef PLATFORM_SC6600L
/* Nothing to be done */

#endif
}

#else
/****************************************************************************
** File Name:      mmiaudio.c                                             *
** Author:                                                                   *
** Date:           15/01/2006                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    the file define and handle audio play ,including all audio                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2006       wancan.you       Create
******************************************************************************/

#define _MMIAUDIO_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmiaudio.h"
#include "mmiaudio_ctrl.h"

#include "mmimp3_export.h"
#include "mmi_appmsg.h"
#include "mmi_common.h"
#include "sig_code.h"
#include "mmi_signal.h"
#include "aud_gen.h"
#include "audio_config.h"
//#include "audio_api.h"
#include "generic_tone_adp.h"
#include "custom_tone_adp.h"
#include "dtmf_tone_adp.h"
#include "adpcmRecord_adp.h"
#include "agc_exp.h"
#include "ratecontrol_exp.h"
#include "midi_adp.h"
#include "mmibt_export.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#ifndef WIN32
//#include "chip.h"
#include "gpio_prod_api.h"
#endif

#include "mp3_adp.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif

#ifdef PLATFORM_SC6600L
//#include "mmienvset.h"
#include "mmienvset_export.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIAUDIO_TRACKBUFSIZE_PLAYER    (80 * 150)
#define MMIAUDIO_TRACKBUFSIZE_APP       (80 * 150)
#define MMIAUDIO_IQ_DATA_BUF_SIZE       (300<<8)    //300k/4
#ifdef AUDIO_LOW_MEMORY_SUPPORT
#define VB_DECODEOUTPUTBUFSIZ           (MMIAUDIO_TRACKBUFSIZE_APP)
#else
#define VB_DECODEOUTPUTBUFSIZ           (MMIAUDIO_TRACKBUFSIZE_APP + MMIAUDIO_TRACKBUFSIZE_PLAYER)
#endif
#define MMIAUDIO_CALLBACK_NUM (10)  //MMIAUDIO_HTYPE
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL uint32* s_application_track_buf = PNULL;      //application buf
LOCAL uint32* s_player_track_buf      = PNULL;      //MP3 Player buf
LOCAL uint32* s_IQ_data_buf           = PNULL;      //IQ DATA buf
LOCAL MMIAUDIO_HTYPE s_cur_audio_handle_type = MMIAUDIO_NONE;

LOCAL HAUDIO s_app_audio_handle    = PNULL;//定义app audio service handle
LOCAL MMIAUDIO_END_CALLBACK_PFUNC s_app_end_callback_func = PNULL;//app audio play end callback func

LOCAL HAUDIO s_player_audio_handle = PNULL;//定义player audio service handle
LOCAL MMIAUDIO_END_CALLBACK_PFUNC s_player_end_callback_func = PNULL;//player audio play end callback func

LOCAL HAUDIO s_music_audio_handle = PNULL;//定义music audio service handle
LOCAL MMIAUDIO_END_CALLBACK_PFUNC s_music_end_callback_func = PNULL;//music audio play end callback func

LOCAL HAUDIO s_normal_audio_handle = PNULL;//定义normal audio service handle
LOCAL MMIAUDIO_END_CALLBACK_PFUNC s_normal_end_callback_func = PNULL;//normal audio play end callback func

LOCAL HAUDIO s_kuro_audio_handle   = PNULL;//定义kuro audio service handle
LOCAL MMIAUDIO_END_CALLBACK_PFUNC s_kuro_end_callback_func = PNULL; //kuro audio play end callback func
//java 
LOCAL HAUDIO s_java_audio_handle = PNULL;//定义java audio service handle
LOCAL MMIAUDIO_END_CALLBACK_PFUNC s_java_end_callback_func = PNULL;//java audio play end callback func
//wre
#ifdef WRE_SUPPORT
LOCAL HAUDIO s_wre_audio_handle = PNULL;//定义wre audio service handle
LOCAL MMIAUDIO_END_CALLBACK_PFUNC s_wre_end_callback_func = PNULL;//wre audio play end callback func
#endif

LOCAL HAUDIO s_record_handle;//定义record handle
LOCAL MMIAUDIO_END_CALLBACK_PFUNC s_record_end_callback_func = PNULL;//record end callback func

LOCAL HAUDIO s_fm_handle;//定义fm handle
LOCAL MMIAUDIO_END_CALLBACK_PFUNC s_fm_end_callback_func = PNULL;//fm end callback func

LOCAL uint32  s_audio_play_times = 1;
LOCAL MMIAUDIO_CALLBACK_INFO_T s_play_callback_info[MMIAUDIO_CALLBACK_NUM] = {0};

LOCAL MMIAUDIO_EQ_MODE_E        s_audio_cur_eq_mode     = MMIAUDIO_EQ_MAX_MODE;              // Save current EQ mode
LOCAL AUDIO_DEVICE_MODE_TYPE_E  s_audio_cur_dev_mode    = AUDIO_DEVICE_MODE_MAX;        // Save current device mode
LOCAL uint32                    s_audio_cur_speaker_vol = MMI_AUDIO_VOLUME_MAX + 1;   // Save current speaker volume
LOCAL BOOLEAN                   s_audio_cur_mute_mode   = FALSE;                        // Save current mute mode
LOCAL MMIAUDIO_PLAY_RATE_E      s_audio_cur_play_rate   = MMIAUDIO_PLAY_RATE_MAX;       // Save current play rate

#ifdef PLATFORM_SC6600L
LOCAL BOOLEAN s_audio_is_agc_enable  = TRUE;    //AUDIO 默认提供EQ功能
LOCAL BOOLEAN s_audio_is_eq_enable   = TRUE;    //AUDIO 默认提供AGC功能,用户开关在ENV模块
#endif

LOCAL BOOLEAN                   s_is_voice_loop_enable  = FALSE;            //是否开启了voice loop
LOCAL BOOLEAN                   s_is_voice_loop_force_mode  = FALSE;        //voice loop是否强制设置模式

LOCAL uint32 s_record_frame_length[MMIAUDIO_RECORD_FRAME_LEN_MAX] = {32,27,21,20,18,16,14,13}; 
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
extern HAUDIODEV hPCMDev;
#endif

#ifdef BT_A2DP_SUPPORT
    #ifdef WIN32
    extern HAUDIODEV hA2DPDev;
    #endif
#else
    HAUDIODEV hA2DPDev;
#endif
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

LOCAL HAUDIO hDtmfRing;
LOCAL HAUDIO hCustomRing;
LOCAL HAUDIO hGenericRing;
HAUDIO hVoiceRing;
HAUDIO hVoiceLoopbackRing;

uint32 VBDecodeOuputBuf[VB_DECODEOUTPUTBUFSIZ];
static uint32 vb_decode_output_buffer_size = VB_DECODEOUTPUTBUFSIZ;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create audio buffer handle
//  Global resource dependence : none
//  Author: 
//  Note: // SUCCESS - return the audio handle created
          // FAILURE - return SCI_NULL
/*****************************************************************************/
LOCAL HAUDIO CreateAudioBufferHandle(
                    BOOLEAN is_a2dp,
                    MMIAUDIO_TYPE_E ring_type,    // the codec handle which will be used.
                    const uint8* pucData,   // the music data pointer
                    uint32 uiDataLength,    // the music data length
                    uint32* puitrackbuffer,
                    uint32 uiTrackBufferSize
                    );

/*****************************************************************************/
//  Description : Create a2dp  audio file handle
//  Global resource dependence : none
//  Author:
//  Note: 
// SUCCESS - return the audio handle created
// FAILURE - return SCI_NULL
/*****************************************************************************/
LOCAL HAUDIO CreateAudioFileHandle( 
                        BOOLEAN is_a2dp,        //if the ring played by a2dp
                        MMIAUDIO_TYPE_E ring_type,    // the codec handle which will be used.
                        const wchar *name_ptr,               // file name ptr
                        uint32* puitrackbuffer,
                        uint32 uiTrackBufferSize
                        );

/*****************************************************************************/
//  Description : Create audio streaming handle
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: // SUCCESS - return the audio handle created
          // FAILURE - return SCI_NULL
/*****************************************************************************/

LOCAL HAUDIO CreateAudioStreamingHandle(
                    BOOLEAN             is_a2dp,
                    MMIAUDIO_TYPE_E     ring_type,              // the codec handle which will be used.
                    const uint8         *pucData,               // the music data pointer
                    uint32              uiDataLength,           // the music data length
                    uint32              *puitrackbuffer,
                    uint32              uiTrackBufferSize,
                    AUDIO_GETSRCDATA_CALLBACK_PFUNC AudioGetSrcDataCallback
                    );
                    
/*****************************************************************************/
//  Description : audio play stop
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void AudioPlayStop(HAUDIO hAudioHandle);

/*****************************************************************************/
//  Description : get audio type
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMIAUDIO_TYPE_E  GetAudioType(HAUDIOCODEC audio_codec);

/*****************************************************************************/
//  Description : get audio codec
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL HAUDIOCODEC GetAudioCodec(MMIAUDIO_TYPE_E ring_type);

/*****************************************************************************/
//  Description : Init audio track buffer
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void InitAudioTrackBuffer(void);

/*****************************************************************************/
//  Description : Reset application buffer.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void ResetApplicationBuffer(void);

/*****************************************************************************/
//  Description : Reset audio player buffer.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void ResetPlayerBuffer(void);

/*****************************************************************************/
//  Description : get audio device
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL HAUDIODEV GetAudioDevice(BOOLEAN is_a2dp, MMIAUDIO_TYPE_E ring_type);

#ifdef AUDIO_RATE_SUPPORT
/*****************************************************************************/
//  Description : Get play rate
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL RATECONTROL_EXP_RATE_E GetAudioPlayRate(
                                    MMIAUDIO_PLAY_RATE_E rate
                                    );
#endif

#ifdef MP3_EQ_SUPPORT 
/*****************************************************************************/
//  Description : Convert EQ type
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL EQ_EXP_MODE_E ConvertEQType(
                        MMIAUDIO_EQ_MODE_E eq_mode
                        );
#endif
/*****************************************************************************/
//  Description : audio end callback
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void AudioEndCallback(HAUDIO haudiohandle, MMIAUDIO_RESULT result, DPARAM param);

/*****************************************************************************/
//  Description : Audio notify callback function.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void AudioNotifyCallback(
                HAUDIO hAudio, 
                uint32 notify_info, 
                uint32 affix_info
                );


/*****************************************************************************/
//  Description : Audio dummy notify callback function.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void AudioDummyNotifyCallback(
                HAUDIO hAudio, 
                uint32 notify_info, 
                uint32 affix_info
                );

/*****************************************************************************/
//  Description : Audio getting source data callback function, for streaming.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_STREAM_GET_SRC_RESULT_E AudioGetSrcCallback(
                                        HAUDIO hAudio, 
                                        uint8 ** const ppucDataAddr, 
                                        uint32* puiDataLength,
                                        int32 offset
                                        );

/*****************************************************************************/
//  Description : Init audio handle for tones.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void AudioInitConstHandle(
                void
                );

/*****************************************************************************/
//  Description : API of audio play seek
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void HandleAudioPriority(
                HAUDIO hAudioHandle
                );

/*****************************************************************************/
//  Description : AddPlayCallback
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void AddPlayCallback(MMIAUDIO_CALLBACK_INFO_T callback_info);

/*****************************************************************************/
//  Description : RemovePlayCallback
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void RemovePlayCallback(HAUDIO audio_handle);

/*****************************************************************************/
//  Description : IsPlayCallbackValid
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsPlayCallbackValid(MMIAUDIO_CALLBACK_INFO_T callback_info);

/*****************************************************************************/
//  Description : IsAudioPlayEnd
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsAudioPlayEnd(HAUDIO audio_handle);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : audio end callback
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void AudioEndCallback(HAUDIO haudiohandle, MMIAUDIO_RESULT result, DPARAM param)
{
    //SCI_TRACE_LOW:"[MMIAUDIO]:AudioEndCallback:haudiohandle=0x%X, s_cur_audio_handle_type=%d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_2447_112_2_18_2_3_31_43,(uint8*)"dd", haudiohandle, s_cur_audio_handle_type);

    if (PNULL != haudiohandle)
    {
        if (s_app_audio_handle == haudiohandle)
        {
            if(MMIAUDIO_APP == s_cur_audio_handle_type)
            {
                s_cur_audio_handle_type = MMIAUDIO_NONE;
            }
            AUDIO_Stop(s_app_audio_handle);
            AUDIO_CloseHandle(s_app_audio_handle);
            s_app_audio_handle = PNULL;

            if (s_app_end_callback_func != PNULL)
            {
                s_cur_audio_handle_type = MMIAUDIO_NONE;
                (*s_app_end_callback_func)(result, param);
                s_app_end_callback_func = PNULL;
            }
        }
        else if (s_player_audio_handle == haudiohandle)
        {
            if(MMIAUDIO_PLAYER == s_cur_audio_handle_type)
            {
                s_cur_audio_handle_type = MMIAUDIO_NONE;
            }
            AUDIO_Stop(s_player_audio_handle);
            AUDIO_CloseHandle(s_player_audio_handle);
            s_player_audio_handle = PNULL;

            if (s_player_end_callback_func != PNULL)
            {
                (*s_player_end_callback_func)(result, param);
                s_player_end_callback_func = PNULL;
            }
        }
        else if (s_music_audio_handle == haudiohandle)
        {
            if(MMIAUDIO_MUSIC== s_cur_audio_handle_type)
            {
                s_cur_audio_handle_type = MMIAUDIO_NONE;
            }
            AUDIO_Stop(s_music_audio_handle);
            AUDIO_CloseHandle(s_music_audio_handle);
            s_music_audio_handle = PNULL;

            if (s_music_end_callback_func != PNULL)
            {
                (*s_music_end_callback_func)(result, param);
                s_music_end_callback_func = PNULL;
            }
        }
        else if (s_kuro_audio_handle == haudiohandle)
        {
            if(MMIAUDIO_KURO == s_cur_audio_handle_type)
            {
                s_cur_audio_handle_type = MMIAUDIO_NONE;
            }
            AUDIO_Stop(s_kuro_audio_handle);
            AUDIO_CloseHandle(s_kuro_audio_handle);
            s_kuro_audio_handle = PNULL;

            if (s_kuro_end_callback_func != PNULL)
            {
                (*s_kuro_end_callback_func)(result, param);
                s_kuro_end_callback_func = PNULL;
            }
        }
#ifdef JAVA_SUPPORT
        else if (s_java_audio_handle == haudiohandle)
        {
            
            if(MMIAUDIO_JAVA == s_cur_audio_handle_type)
            {
                s_cur_audio_handle_type = MMIAUDIO_NONE;
            }
            AUDIO_Stop(s_java_audio_handle);
            AUDIO_CloseHandle(s_java_audio_handle);
            s_java_audio_handle = PNULL;

            if (s_java_end_callback_func != PNULL)
            {
                (*s_java_end_callback_func)(result, param);
                s_java_end_callback_func = PNULL;
            }
        }
#endif
#ifdef WRE_SUPPORT
		else if (s_wre_audio_handle == haudiohandle)
	   {
		   if(MMIAUDIO_WRE == s_cur_audio_handle_type)
		   {
			   s_cur_audio_handle_type = MMIAUDIO_NONE;
		   }
		   AUDIO_Stop(s_wre_audio_handle);
           AUDIO_CloseHandle(s_wre_audio_handle);
           s_wre_audio_handle = PNULL;
		   if (s_wre_end_callback_func != PNULL)
		   {
			   (*s_wre_end_callback_func)(result, param);
			   s_wre_end_callback_func = PNULL;
		   }

	   }

#endif
        else if (s_normal_audio_handle == haudiohandle)
        {
            if(MMIAUDIO_NORMAL == s_cur_audio_handle_type)
            {
                s_cur_audio_handle_type = MMIAUDIO_NONE;
            }
            AUDIO_Stop(s_normal_audio_handle);
            AUDIO_CloseHandle(s_normal_audio_handle);
            s_normal_audio_handle = PNULL;

            if (s_normal_end_callback_func != PNULL)
            {
                (*s_normal_end_callback_func)(result, param);
                s_normal_end_callback_func = PNULL;
            }
        }
        else if(s_record_handle == haudiohandle)    //录音      
        {
            MMIAUDIO_StopRecord();
            MMIAUDIO_EndRecord();
            if (s_record_end_callback_func != PNULL)
            {
                (*s_record_end_callback_func)(result, param);
                s_record_end_callback_func = PNULL;
            }
        }
    }//end of if (PNULL != haudiohandle)
}

/*****************************************************************************/
//  Description : Mmi audio send a stop msg to itself.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL void AudioSendStopMsg(HAUDIO hAudio)
{
    MMIAUDIO_CALLBACK_INFO_T callback_info = {0};

    callback_info.notify_info = AUDIO_STOPPED_IND;
    callback_info.hAudio = hAudio;
    callback_info.affix_info = AUDIO_NO_ERROR;
    MMIAPICOM_OtherTaskToMMI(VIRTUAL_WIN_ID, MSG_AUDIO_CALLACK, (DPARAM)&callback_info, sizeof(MMIAUDIO_CALLBACK_INFO_T));
}
    
/*****************************************************************************/
//  Description : audio stop callback
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL void AudioStopCallback(HAUDIO haudiohandle, MMIAUDIO_RESULT result, DPARAM param)
{
    //SCI_TRACE_LOW:"[MMIAUDIO]:AudioStopCallback:haudiohandle=0x%X, s_cur_audio_handle_type=%d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_2603_112_2_18_2_3_31_44,(uint8*)"dd", haudiohandle, s_cur_audio_handle_type);
    if (PNULL != haudiohandle)
    {
#if 0
/* 异步*/
        if (s_app_audio_handle == haudiohandle)
        {
            if (s_app_end_callback_func != PNULL)
            {
                (*s_app_end_callback_func)(result, param);
                RemovePlayCallback(s_app_audio_handle);
                s_app_audio_handle = PNULL;
                if(haudiohandle == s_cur_audio_handle_type)
                {
                    s_cur_audio_handle_type = MMIAUDIO_NONE;
                }
            }
        }
#else
/* 同步*/
        if (s_app_audio_handle == haudiohandle)
        {
            if (s_app_end_callback_func != PNULL)
            {
                MMIAUDIO_END_CALLBACK_PFUNC cb_func = s_app_end_callback_func;
                s_app_end_callback_func = PNULL;
                (*cb_func)(result, param);
            }
        }

#endif

/*        
        else if (s_player_audio_handle == haudiohandle)
        {
            if (s_player_end_callback_func != PNULL)
            {
                (*s_player_end_callback_func)(result, param);
            }
        }
        else if (s_music_audio_handle == haudiohandle)
        {
            if (s_music_end_callback_func != PNULL)
            {
                (*s_music_end_callback_func)(result, param);
            }
        }
        else if (s_kuro_audio_handle == haudiohandle)
        {
            if (s_kuro_end_callback_func != PNULL)
            {
                (*s_kuro_end_callback_func)(result, param);
            }
        }
#ifdef JAVA_SUPPORT
        else if (s_java_audio_handle == haudiohandle)
        {
            if (s_java_end_callback_func != PNULL)
            {
                (*s_java_end_callback_func)(result, param);
            }
        }
#endif
        else if (s_normal_audio_handle == haudiohandle)
        {
            if (s_normal_end_callback_func != PNULL)
            {
                (*s_normal_end_callback_func)(result, param);
            }
        }
        else if(s_record_handle == haudiohandle)    //录音      
        {
            if (s_record_end_callback_func != PNULL)
            {
                (*s_record_end_callback_func)(result, param);
            }
        }
*/        
    }//end of if (PNULL != haudiohandle)
}
/*****************************************************************************/
//  Description : Audio notify callback function.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void AudioNotifyCallback(
                HAUDIO hAudio, 
                uint32 notify_info, 
                uint32 affix_info
                )
{
    MmiSignalS *sendSignal = PNULL;
    MMIAUDIO_CALLBACK_INFO_T callback_info = {0};
    
    //SCI_TRACE_LOW:"[MMIAUDIO]:AudioNotifyCallback:enter:hAudio = 0x%X, notify_info = 0x%X, affix_info = %ld"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_2694_112_2_18_2_3_31_45,(uint8*)"ddd",hAudio, notify_info, affix_info);

    if(AUDIO_PLAYEND_IND == notify_info)
    {
        callback_info.hAudio      = hAudio;
        callback_info.notify_info = notify_info;
        callback_info.affix_info  = affix_info;
        AddPlayCallback(callback_info);

        MMIAUDIO_SendLastKeyTpkeyNotify();
    }
    else
    {
        return;
    }

    /*
    MMK_PostMsg((MMI_WIN_ID_T)VIRTUAL_WIN_ID, MSG_AUDIO_CALLACK, 
        (DPARAM)&callback_info, sizeof(MMIAUDIO_CALLBACK_INFO_T));*/
    MmiCreateSignal(APP_AUDIO_END_IND, sizeof(MmiAudioCallbackMsgS), &sendSignal);
    sendSignal->send = P_APP;
    sendSignal->sig.audio_info.hAudio      = hAudio;
    sendSignal->sig.audio_info.notify_info = notify_info;
    sendSignal->sig.audio_info.affix_info  = affix_info;
    MmiSendSignal(P_APP, sendSignal);
}

/*****************************************************************************/
//  Description : Audio dummy notify callback function.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void AudioDummyNotifyCallback(
                HAUDIO hAudio, 
                uint32 notify_info, 
                uint32 affix_info
                )
{
    //do nothing
}

/*****************************************************************************/
//  Description : Audio getting source data callback function, for streaming.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_STREAM_GET_SRC_RESULT_E AudioGetSrcCallback(
                                            HAUDIO hAudio, 
                                            uint8 ** const ppucDataAddr, 
                                            uint32* puiDataLength,
                                            int32 offset
                                            )
{
    //Later
    return AUDIO_STREAM_GET_SRC_SUCCESS;
}

/*****************************************************************************/
//  Description : Handle audio callback message.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIAUDIO_HandleCallBackMsg(
                    MMIAUDIO_CALLBACK_INFO_T *callback_info_ptr
                    )
{
    HAUDIO          haudiohandle = callback_info_ptr->hAudio;
    AUDIO_RESULT_E  audio_result = (AUDIO_RESULT_E)callback_info_ptr->affix_info;
    MMIAUDIO_RESULT mmi_result   = MMIAUDIO_RLT_SUCCESS;

    //SCI_TRACE_MID:"[MMIAUDIO]:MMIAPIAUDIO_HandleCallBackMsg:handle = 0x%X,msg_id = 0x%X, aud_result = %d, s_cur_audio_handle_type = %d, IsEnd=%d,app_handle = 0x%X, player_handle = 0x%X, music_handle = 0x%X, kuro_handle = 0x%X, normal_handle = 0x%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_2762_112_2_18_2_3_32_46,(uint8*)"dddddddddd", haudiohandle,callback_info_ptr->notify_info,audio_result,s_cur_audio_handle_type,IsAudioPlayEnd(haudiohandle),s_app_audio_handle,s_player_audio_handle,s_music_audio_handle,s_kuro_audio_handle,s_normal_audio_handle);

    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAPIAUDIO_HandleCallBackMsg:app_callback = 0x%X,player_callback = 0x%X, music_callback = 0x%X, kuro_callback = 0x%X,normal_callback = 0x%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_2764_112_2_18_2_3_32_47,(uint8*)"ddddd",s_app_end_callback_func,s_player_end_callback_func,s_music_end_callback_func,s_kuro_end_callback_func,s_normal_end_callback_func);
    switch (callback_info_ptr->notify_info)
    {
        case AUDIO_PLAYEND_IND:
            if(IsAudioPlayEnd(haudiohandle))
            {
                RemovePlayCallback(haudiohandle);
                
                if(AUDIO_NO_ERROR == audio_result     &&
                   1 != s_audio_play_times            &&
                   s_app_audio_handle == haudiohandle &&
                   MMIAUDIO_APP == s_cur_audio_handle_type)
                {
                    s_audio_play_times --;
                    if(MMIAUDIO_AudioPlay(
                            s_cur_audio_handle_type,
                            0,
                            s_audio_play_times,
                            s_app_end_callback_func
                            ))
                    {
                        //RemovePlayCallback(haudiohandle);
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAPIAUDIO_HandleCallBackMsg,  send play error"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_2789_112_2_18_2_3_32_48,(uint8*)"");
                        AudioEndCallback(haudiohandle, MMIAUDIO_RLT_ERROR, PNULL);
                    }
                }
                else
                {
                    mmi_result = (AUDIO_NO_ERROR == audio_result)?MMIAUDIO_RLT_SUCCESS:MMIAUDIO_RLT_NOT_SUPPORT;
                    AudioEndCallback(haudiohandle, mmi_result, PNULL);
                }
            }
            break;
#if 0    
        case AUDIO_STOPPED_IND: 
            //if(IsAudioPlayEnd(haudiohandle))
            {
                AudioStopCallback(haudiohandle, MMIAUDIO_RLT_STOP, PNULL);            
            }            
            break;
#endif   
        default:
            break;
    }
}

/*****************************************************************************/
//  Description : AddPlayCallback
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void AddPlayCallback(MMIAUDIO_CALLBACK_INFO_T callback_info)
{ 
    uint32 i = 0;
    
	for(i=0; i<MMIAUDIO_CALLBACK_NUM; i++)
	{
	    if(INVALID_HANDLE == s_play_callback_info[i].hAudio)
	    {
	        s_play_callback_info[i].hAudio      = callback_info.hAudio;
	        s_play_callback_info[i].notify_info = callback_info.notify_info;
	        s_play_callback_info[i].affix_info  = callback_info.affix_info;
	        break;
	    }
	}
}

/*****************************************************************************/
//  Description : RemovePlayCallback
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void RemovePlayCallback(HAUDIO audio_handle)
{ 
    uint32 i = 0;
    
	for(i=0; i<MMIAUDIO_CALLBACK_NUM; i++)
	{
	    if(audio_handle == s_play_callback_info[i].hAudio)
	    {
	        SCI_MEMSET(&s_play_callback_info[i], 0x00, sizeof(MMIAUDIO_CALLBACK_INFO_T));
	        //SCI_TRACE_LOW:"[MMIAUDIO]:RemovePlayCallback: hAudiohandle =0x%x, i = %d"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_2846_112_2_18_2_3_32_49,(uint8*)"dd", audio_handle, i);
	    }
	}
}

/*****************************************************************************/
//  Description : IsPlayCallbackValid
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsPlayCallbackValid(MMIAUDIO_CALLBACK_INFO_T callback_info)
{ 
    uint32  i       = 0;
    BOOLEAN result  = FALSE;
    
	for(i=0; i<MMIAUDIO_CALLBACK_NUM; i++)
	{
	    if(callback_info.hAudio      != INVALID_HANDLE                   &&
	       callback_info.hAudio      == s_play_callback_info[i].hAudio   &&
	       callback_info.notify_info == s_play_callback_info[i].notify_info)
	    {
	        result = TRUE;
	        break;
	    }
	}

    if(result)
    {
	    //SCI_TRACE_LOW:"[MMIAUDIO]:IsPlayCallbackValid: hAudiohandle =0x%x, i = %d, result"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_2873_112_2_18_2_3_32_50,(uint8*)"dd", callback_info.hAudio, i, result);
	}
	
	return result;
}

/*****************************************************************************/
//  Description : IsAudioPlayEnd
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsAudioPlayEnd(HAUDIO audio_handle)
{ 
    MMIAUDIO_CALLBACK_INFO_T callback_info = {0};

    callback_info.hAudio      = audio_handle;
    callback_info.notify_info = AUDIO_PLAYEND_IND;
    
	return IsPlayCallbackValid(callback_info);
}


/*****************************************************************************/
//  Description : MMIAUDIO_IsAudioPlayEnd
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsAudioPlayEnd(HAUDIO audio_handle)
{ 
	return IsAudioPlayEnd(audio_handle);
}

/*****************************************************************************/
//  Description : get audio type
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMIAUDIO_TYPE_E  GetAudioType(HAUDIOCODEC audio_codec)
{
    //SCI_TRACE_LOW:"[MMIAUDIO]:GetAudioCodec:audio_codec=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_2909_112_2_18_2_3_32_51,(uint8*)"d", audio_codec);

    if(hMIDICodec == audio_codec)
    {
        return MMIAUDIO_MIDI;//or MMIAUDIO_SMAF
    }
    
    if(hMP3Codec == audio_codec)
    {
        return MMIAUDIO_MP3;
    }

#ifdef WMA_SUPPORT
    if (hWMACodec == audio_codec)
    {
        return MMIAUDIO_WMA;
    }
#endif    

    if(hAMRCodec == audio_codec)
    {
        return MMIAUDIO_AMR;
    }
    
    if(hGenericCodec == audio_codec)
    {
        return MMIAUDIO_GENERIC_TONE;
    }
    
    if(hCustomCodec == audio_codec)
    {
        return MMIAUDIO_CUSTOM_TONE;
    }
    
    if(hDtmfCodec == audio_codec)
    {
        return MMIAUDIO_DTMF_TONE;
    }

    if(hWAVCodec == audio_codec)
    {
        return MMIAUDIO_WAVE;
    }
    
    if(hAACCodec == audio_codec)
    {
        return MMIAUDIO_AAC;
    }
    
    return MMIAUDIO_TYPE_MAX;
}

/*****************************************************************************/
//  Description : get audio codec
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL HAUDIOCODEC GetAudioCodec(MMIAUDIO_TYPE_E ring_type)
{
    HAUDIOCODEC hAudioCodec = 0;

    //SCI_TRACE_LOW:"[MMIAUDIO]:GetAudioCodec:ring_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_2969_112_2_18_2_3_32_52,(uint8*)"d", ring_type);

    switch(ring_type)
    {
        case MMIAUDIO_MIDI:
            hAudioCodec = hMIDICodec;
            break;

        case MMIAUDIO_SMAF:
            hAudioCodec = hMIDICodec;
            break;

        case MMIAUDIO_MP3:
			
#if defined(PLATFORM_SC6600L) || defined(AUDIO_SC6800H)
            hAudioCodec = hMP3DSPCodec;
#else
		hAudioCodec = hMP3Codec;
#endif
            break;

        case MMIAUDIO_WMA:
#ifdef WMA_SUPPORT
            hAudioCodec = hWMACodec;
#endif
            break;

        case MMIAUDIO_AMR:
            hAudioCodec = hAMRCodec;
            break;

        case MMIAUDIO_GENERIC_TONE:
            hAudioCodec = hGenericCodec;
            break;

        case MMIAUDIO_CUSTOM_TONE:
            hAudioCodec = hCustomCodec;
            break;

        case MMIAUDIO_DTMF_TONE:
            hAudioCodec = hDtmfCodec;
            break;
            
        case MMIAUDIO_WAVE:
            hAudioCodec = hWAVCodec;
            break;
            
        case MMIAUDIO_AAC:
        case MMIAUDIO_M4A:
            hAudioCodec = hAACLCCodec;
            break;
            
        default:
            hAudioCodec = hMIDICodec;
            break;
    }

    return hAudioCodec;
}


/*****************************************************************************/
//  Description : get audio device
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL HAUDIODEV GetAudioDevice(
                    BOOLEAN is_a2dp,
                    MMIAUDIO_TYPE_E ring_type
                    )
{
    HAUDIODEV hDevice = 0;

    switch(ring_type)
    {
        case MMIAUDIO_MP3:
        case MMIAUDIO_AAC:
        case MMIAUDIO_M4A:
        case MMIAUDIO_WMA:
        case MMIAUDIO_MIDI:
        case MMIAUDIO_WAVE:
        case MMIAUDIO_AMR:
            //#ifdef BT_A2DP_SUPPORT
            if (is_a2dp)
            {
                if(MMIBT_HFG_HEADSET == MMIAPIBT_GetActiveHeadsetType())
                {
                    //HFG device play
                    hDevice = hPCMDev; 

                }   
                else
                {
                    //A2DP Device play
                    hDevice = hA2DPDev; 
                }
            }
            else
            //#endif
            {
                hDevice = hARMVB; 
            }
#ifndef AMR_A2DP_SUPPORT
            if(MMIAUDIO_AMR == ring_type)
            {
                hDevice = hLAYER1;
            }
#endif
        break;
        
        default:
            hDevice = hLAYER1;
            break;
    }

    //SCI_TRACE_LOW:"[MMIAUDIO]:GetAudioDevice: ring_type = %d, hDevice = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_3083_112_2_18_2_3_32_53,(uint8*)"dd",ring_type, hDevice);
    return hDevice;
}

/*****************************************************************************/
//  Description : Init audio track buffer
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void InitAudioTrackBuffer(void)
{
    s_application_track_buf = (uint32 *)VBDecodeOuputBuf;
#ifdef AUDIO_LOW_MEMORY_SUPPORT
    s_player_track_buf = (uint32 *)(VBDecodeOuputBuf);
#else
    s_player_track_buf = (uint32 *)(VBDecodeOuputBuf + MMIAUDIO_TRACKBUFSIZE_APP);
#endif
}

/*****************************************************************************/
//  Description : Reset application buffer.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void ResetApplicationBuffer(void)
{
    SCI_MEMSET(s_application_track_buf, 0, (MMIAUDIO_TRACKBUFSIZE_APP*sizeof(uint32)));
}

/*****************************************************************************/
//  Description : Reset audio player buffer.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void ResetPlayerBuffer(void)
{
    SCI_MEMSET(s_player_track_buf, 0, (MMIAUDIO_TRACKBUFSIZE_PLAYER*sizeof(uint32)));
}

/*****************************************************************************/
//  Description : create audio buffer handle
//  Global resource dependence : none
//  Author: 
//  Note: // SUCCESS - return the audio handle created
          // FAILURE - return SCI_NULL
/*****************************************************************************/
LOCAL HAUDIO CreateAudioBufferHandle(
                    BOOLEAN is_a2dp,
                    MMIAUDIO_TYPE_E ring_type,    // the codec handle which will be used.
                    const uint8* pucData,   // the music data pointer
                    uint32 uiDataLength,    // the music data length
                    uint32* puitrackbuffer,
                    uint32 uiTrackBufferSize
                    )
{
    HAUDIOCODEC hAudioCodec = 0;
    uint16* pusCodecName = PNULL;
    HAUDIODEV hDevice = 0;
    uint16* pusDevName = PNULL;
    HAUDIO audiohandle = INVALID_HANDLE;
    HAUDIOCODEC real_audio_codec = 0;
    AUDIO_RESULT_E result = AUDIO_ERROR;
    
    if(PNULL == pucData)
    {
        return audiohandle;
    }

    hAudioCodec = GetAudioCodec(ring_type);
    hDevice = GetAudioDevice(is_a2dp,ring_type);

    audiohandle = AUDIO_CreateBufferHandle(
                        hAudioCodec,    // the codec handle which will be used.
                        pusCodecName,     // the codec name which will be used.
                        hDevice, 
                        pusDevName,     
                        pucData,   // the music data pointer
                        uiDataLength,    // the music data length
                        puitrackbuffer,     // decode data output buffer.
                        uiTrackBufferSize,   // the track buffer size.
                        AudioNotifyCallback  // audio notify callback funtion
                        );

    if(INVALID_HANDLE != audiohandle)
    {
        real_audio_codec = AUDIO_GetCodecHandle(audiohandle);
        if(real_audio_codec != hAudioCodec)
        {
            hDevice = GetAudioDevice(is_a2dp,GetAudioType(real_audio_codec));
            result = AUDIO_SwitchDevice(audiohandle, hDevice, pusDevName);
            //SCI_TRACE_LOW:"[MMIAUDIO]:CreateAudioBufferHandle:ring_type error!!!! orig = %x, new = %x, aud_result = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_3168_112_2_18_2_3_32_54,(uint8*)"ddd",hAudioCodec,real_audio_codec,result);
        }
    }

    //SCI_TRACE_LOW:"[MMIAUDIO]:CreateAudioBufferHandle:ring_type=%d, data_ptr = 0x%x, data_len = %d, audiohandle = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_3173_112_2_18_2_3_32_55,(uint8*)"dddd", ring_type, pucData,uiDataLength,audiohandle);
    return audiohandle;
}

/*****************************************************************************/
//  Description : Create audio file handle
//  Global resource dependence : none
//  Author: 
//  Note: // SUCCESS - return the audio handle created
          // FAILURE - return SCI_NULL
/*****************************************************************************/

LOCAL HAUDIO CreateAudioFileHandle(
                    BOOLEAN is_a2dp,
                    MMIAUDIO_TYPE_E ring_type,    // the codec handle which will be used.
                    const wchar *name_ptr,               // file name ptr
                    uint32* puitrackbuffer,
                    uint32 uiTrackBufferSize
                    )
{
    HAUDIO audiohandle = INVALID_HANDLE;
    HAUDIOCODEC hAudioCodec = 0;
    uint16* pusCodecName = PNULL;
    HAUDIODEV hDevice = 0;
    uint16* pusDevName = PNULL;
    HAUDIOCODEC real_audio_codec = 0;
    AUDIO_RESULT_E result = AUDIO_ERROR;
    
    hAudioCodec = GetAudioCodec(ring_type);
    hDevice = GetAudioDevice(is_a2dp,ring_type);

    audiohandle = AUDIO_CreateFileHandle(
                        hAudioCodec,    // the codec handle which will be used.
                        pusCodecName,     // the codec name which will be used.
                        hDevice, 
                        pusDevName,
                        (const int16 *)name_ptr,
                        puitrackbuffer,     // decode data output buffer.
                        uiTrackBufferSize,    // the track buffer size.
                        AudioNotifyCallback // audio notify callback funtion
                        );

    if(INVALID_HANDLE != audiohandle)
    {
        real_audio_codec = AUDIO_GetCodecHandle(audiohandle);
        if(real_audio_codec != hAudioCodec)
        {
            hDevice = GetAudioDevice(is_a2dp,GetAudioType(real_audio_codec));
            result = AUDIO_SwitchDevice(audiohandle, hDevice, pusDevName);
            //SCI_TRACE_LOW:"[MMIAUDIO]:CreateAudioBufferHandle:ring_type error!!!! orig = %x, new = %x, aud_result = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_3220_112_2_18_2_3_33_56,(uint8*)"ddd",hAudioCodec,real_audio_codec,result);
        }
    }

    //SCI_TRACE_LOW:"[MMIAUDIO]:CreateAudioFileHandle:ring_type=%d, audiohandle = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_3224_112_2_18_2_3_33_57,(uint8*)"dd", ring_type, audiohandle);
    return audiohandle;
}

/*****************************************************************************/
//  Description : Create audio streaming handle
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: // SUCCESS - return the audio handle created
          // FAILURE - return SCI_NULL
/*****************************************************************************/

LOCAL HAUDIO CreateAudioStreamingHandle(
                    BOOLEAN             is_a2dp,
                    MMIAUDIO_TYPE_E     ring_type,              // the codec handle which will be used.
                    const uint8         *pucData,               // the music data pointer
                    uint32              uiDataLength,           // the music data length
                    uint32              *puitrackbuffer,
                    uint32              uiTrackBufferSize,
                    AUDIO_GETSRCDATA_CALLBACK_PFUNC AudioGetSrcDataCallback
                    )
{
    HAUDIOCODEC     hAudioCodec     = 0;
    HAUDIODEV       hDevice         = 0;
    HAUDIO          audiohandle     = INVALID_HANDLE;
    uint16          *pusCodecName   = PNULL;
    uint16          *pusDevName     = PNULL;
    HAUDIOCODEC real_audio_codec = 0;
    AUDIO_RESULT_E result = AUDIO_ERROR;
    
    if(PNULL == pucData)
    {
        return audiohandle;
    }

    hAudioCodec = GetAudioCodec(ring_type);
    hDevice = GetAudioDevice(is_a2dp,ring_type);

    audiohandle =  AUDIO_CreateStreamBufferHandle(
                        hAudioCodec,
                        pusCodecName,
                        hDevice, 
                        pusDevName,
                        pucData,
                        uiDataLength,
                        puitrackbuffer,
                        uiTrackBufferSize,
                        AudioNotifyCallback,
                        AudioGetSrcDataCallback
                        );

    if(INVALID_HANDLE != audiohandle)
    {
        real_audio_codec = AUDIO_GetCodecHandle(audiohandle);
        if(real_audio_codec != hAudioCodec)
        {
            hDevice = GetAudioDevice(is_a2dp,GetAudioType(real_audio_codec));
            result = AUDIO_SwitchDevice(audiohandle, hDevice, pusDevName);
            //SCI_TRACE_LOW:"[MMIAUDIO]:CreateAudioBufferHandle:ring_type error!!!! orig = %x, new = %x, aud_result = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_3280_112_2_18_2_3_33_58,(uint8*)"ddd",hAudioCodec,real_audio_codec,result);
        }
    }

    //SCI_TRACE_LOW:"[MMIAUDIO]:CreateAudioStreamingHandle:ring_type=%d, data_ptr = 0x%x, data_len = %d, audiohandle = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_3285_112_2_18_2_3_33_59,(uint8*)"dddd", ring_type, pucData,uiDataLength,audiohandle);
    return audiohandle;
}

/*****************************************************************************/
//  Description : audio init
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_Init(void)
{
    //init const handles
    AudioInitConstHandle();
    //init audio buf
    InitAudioTrackBuffer();
    //init dev mode
    MMIAUDIO_SetCurDevMode(AUDIO_DEVICE_MODE_HANDFREE);
    //init EQ mode
    #ifdef MP3_EQ_SUPPORT 
    MMIAUDIO_SetEQMode(MMIAUDIO_EQ_REGULAR);
    #endif
    MMIAUDIO_SetPlayRate(MMIAUDIO_PLAY_RATE_NORMAL);
}

/*****************************************************************************/
//  Description : API of create audio buffer play handle
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_CreateAudioBufHandle(
                     BOOLEAN is_a2dp,
                     MMIAUDIO_HTYPE audio_handle_type,   //the type of audio handle
                     MMIAUDIO_TYPE_E ring_type,    // the codec handle which will be used.
                     const uint8* pucData,   // the music data pointer
                     uint32 uiDataLength    // the music data length
                     )
{
    BOOLEAN result = TRUE;
    HAUDIO audio_handle = INVALID_HANDLE;

    if(PNULL == pucData)
    {
        return FALSE;
    }

    switch(audio_handle_type)
    {
        case MMIAUDIO_APP:          
            audio_handle = CreateAudioBufferHandle(
                                is_a2dp,
                                ring_type,
                                pucData,
                                uiDataLength,
                                s_application_track_buf,
                                MMIAUDIO_TRACKBUFSIZE_APP
                                );
            s_app_audio_handle = audio_handle;
            break;

        case MMIAUDIO_PLAYER:       
            audio_handle = CreateAudioBufferHandle(
                                is_a2dp,
                                ring_type,
                                pucData,
                                uiDataLength,
                                s_player_track_buf,
                                MMIAUDIO_TRACKBUFSIZE_PLAYER
                                );

            s_player_audio_handle = audio_handle;
            break;

        case MMIAUDIO_MUSIC:       
            audio_handle = CreateAudioBufferHandle(
                                is_a2dp,
                                ring_type,
                                pucData,
                                uiDataLength,
                                s_player_track_buf,
                                MMIAUDIO_TRACKBUFSIZE_PLAYER
                                );

            s_music_audio_handle = audio_handle;
            break;


        case MMIAUDIO_KURO:       
            audio_handle = CreateAudioBufferHandle(
                                is_a2dp,
                                ring_type,
                                pucData,
                                uiDataLength,
                                s_player_track_buf,
                                MMIAUDIO_TRACKBUFSIZE_PLAYER
                                );

            s_kuro_audio_handle = audio_handle;
            break;

        case MMIAUDIO_NORMAL:           
            audio_handle = CreateAudioBufferHandle(
                                is_a2dp,
                                ring_type,
                                pucData,
                                uiDataLength,
                                s_application_track_buf,
                                MMIAUDIO_TRACKBUFSIZE_APP
                                );

            s_normal_audio_handle = audio_handle;
            break;
#ifdef JAVA_SUPPORT
        case MMIAUDIO_JAVA:
              audio_handle = CreateAudioBufferHandle(
                                is_a2dp,
                                ring_type,
                                pucData,
                                uiDataLength,
                                s_application_track_buf,
                                MMIAUDIO_TRACKBUFSIZE_APP
                                );

            s_java_audio_handle = audio_handle;

            break;
#endif           
#ifdef WRE_SUPPORT
		case MMIAUDIO_WRE:
			audio_handle = CreateAudioBufferHandle(
								is_a2dp,
								ring_type,
								pucData,
								uiDataLength,
								s_application_track_buf,
								MMIAUDIO_TRACKBUFSIZE_APP
								);
			s_wre_audio_handle = audio_handle;
		
			break;
#endif     
        default:
            break;
    }

    if (INVALID_HANDLE == audio_handle)
    {
        result = FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description : API of create audio file handle
//  Global resource dependence : none
//  Author: 
//  Note: SUCCESS - return the audio handle created
//        FAILURE - return SCI_NULL
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_CreateAudioFileHandle( 
                        BOOLEAN is_a2dp,
                        MMIAUDIO_HTYPE audio_handle_type,
                        MMIAUDIO_TYPE_E ring_type,          // the codec handle which will be used.
                        const wchar *name_ptr               // file name ptr
                        )
{
    BOOLEAN result = TRUE;
    HAUDIO audio_handle = INVALID_HANDLE;

    switch(audio_handle_type)
    {
        case MMIAUDIO_APP:
            audio_handle = CreateAudioFileHandle(
                                is_a2dp,
                                ring_type,
                                name_ptr,
                                s_application_track_buf,
                                MMIAUDIO_TRACKBUFSIZE_APP
                                );

            s_app_audio_handle = audio_handle;
            break;

        case MMIAUDIO_PLAYER:
            audio_handle = CreateAudioFileHandle(
                                is_a2dp,
                                ring_type,
                                name_ptr,
                                s_player_track_buf,
                                MMIAUDIO_TRACKBUFSIZE_PLAYER
                                );

            s_player_audio_handle = audio_handle;
            break;

        case MMIAUDIO_MUSIC:
            audio_handle = CreateAudioFileHandle(
                                is_a2dp,
                                ring_type,
                                name_ptr,
                                s_player_track_buf,
                                MMIAUDIO_TRACKBUFSIZE_PLAYER
                                );

            s_music_audio_handle = audio_handle;
            break;
            
        case MMIAUDIO_KURO:
            audio_handle = CreateAudioFileHandle(
                                is_a2dp,
                                ring_type,
                                name_ptr,
                                s_player_track_buf,
                                MMIAUDIO_TRACKBUFSIZE_PLAYER
                                );

            s_kuro_audio_handle = audio_handle;
            break;

        case MMIAUDIO_NORMAL:
            audio_handle = CreateAudioFileHandle(
                                is_a2dp,
                                ring_type,
                                name_ptr,
                                s_application_track_buf,
                                MMIAUDIO_TRACKBUFSIZE_APP
                                );

            s_normal_audio_handle = audio_handle;
            break;
#ifdef JAVA_SUPPORT
        case MMIAUDIO_JAVA:
                 audio_handle = CreateAudioFileHandle(
                                is_a2dp,
                                ring_type,
                                name_ptr,
                                s_application_track_buf,
                                MMIAUDIO_TRACKBUFSIZE_APP
                                );

            s_java_audio_handle = audio_handle;

            break;
#endif
#ifdef WRE_SUPPORT
		case MMIAUDIO_WRE:
			 audio_handle = CreateAudioFileHandle(
								is_a2dp,
								ring_type,
								name_ptr,
								s_application_track_buf,
								MMIAUDIO_TRACKBUFSIZE_APP
								);
		
		s_wre_audio_handle = audio_handle;
		
		break;
#endif
        default:
            break;
    }

    if (INVALID_HANDLE == audio_handle)
    {
        result = FALSE;
    }

    return result;
}


/*****************************************************************************/
//  Description : API of create audio streaming buffer play handle
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_CreateAudioStreamingHandle(
                     BOOLEAN            is_a2dp,
                     MMIAUDIO_HTYPE     audio_handle_type,  //the type of audio handle
                     MMIAUDIO_TYPE_E    ring_type,          // the codec handle which will be used.
                     const uint8        *pucData,           // the music data pointer
                     uint32             uiDataLength,       // the music data length
                     AUDIO_GETSRCDATA_CALLBACK_PFUNC AudioGetSrcDataCallback
                     )
{
    BOOLEAN result = TRUE;
    HAUDIO audio_handle = INVALID_HANDLE;

    if(PNULL == pucData)
    {
        return FALSE;
    }

    switch(audio_handle_type)
    {
        case MMIAUDIO_APP:
            audio_handle = CreateAudioStreamingHandle(
                                is_a2dp,
                                ring_type,
                                pucData,
                                uiDataLength,
                                s_application_track_buf,
                                MMIAUDIO_TRACKBUFSIZE_APP,
                                AudioGetSrcDataCallback
                                );
            s_app_audio_handle = audio_handle;
            break;

        case MMIAUDIO_PLAYER:       
            audio_handle = CreateAudioStreamingHandle(
                                is_a2dp,
                                ring_type,
                                pucData,
                                uiDataLength,
                                s_player_track_buf,
                                MMIAUDIO_TRACKBUFSIZE_PLAYER,
                                AudioGetSrcDataCallback
                                );

            s_player_audio_handle = audio_handle;
            break;

        case MMIAUDIO_MUSIC:       
            audio_handle = CreateAudioStreamingHandle(
                                is_a2dp,
                                ring_type,
                                pucData,
                                uiDataLength,
                                s_player_track_buf,
                                MMIAUDIO_TRACKBUFSIZE_PLAYER,
                                AudioGetSrcDataCallback
                                );

            s_music_audio_handle = audio_handle;
            break;
            
        case MMIAUDIO_KURO:       
            audio_handle = CreateAudioStreamingHandle(
                                is_a2dp,
                                ring_type,
                                pucData,
                                uiDataLength,
                                s_player_track_buf,
                                MMIAUDIO_TRACKBUFSIZE_PLAYER,
                                AudioGetSrcDataCallback
                                );

            s_kuro_audio_handle = audio_handle;
            break;

        case MMIAUDIO_NORMAL:           
            audio_handle = CreateAudioStreamingHandle(
                                is_a2dp,
                                ring_type,
                                pucData,
                                uiDataLength,
                                s_application_track_buf,
                                MMIAUDIO_TRACKBUFSIZE_APP,
                                AudioGetSrcDataCallback
                                );

            s_normal_audio_handle = audio_handle;
            break;
            
        #ifdef JAVA_SUPPORT
        case MMIAUDIO_JAVA:
            audio_handle = CreateAudioStreamingHandle(
                                is_a2dp,
                                ring_type,
                                pucData,
                                uiDataLength,
                                s_application_track_buf,
                                MMIAUDIO_TRACKBUFSIZE_APP,
                                AudioGetSrcDataCallback
                                );

            s_java_audio_handle = audio_handle;

            break;
        #endif
#ifdef WRE_SUPPORT
        case MMIAUDIO_WRE:
            audio_handle = CreateAudioStreamingHandle(
                                is_a2dp,
                                ring_type,
                                pucData,
                                uiDataLength,
                                s_application_track_buf,
                                MMIAUDIO_TRACKBUFSIZE_APP,
                                AudioGetSrcDataCallback
                                );

            s_wre_audio_handle = audio_handle;

            break;
#endif	
        default:
            break;
    }

    if (INVALID_HANDLE == audio_handle)
    {
        result = FALSE;
    }

    return result;
}


/*****************************************************************************/
//  Description : audio play 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioPlay(
                            MMIAUDIO_HTYPE   audio_handle_type,
                            uint32           uiOffset,
                            uint32           play_times,
                            MMIAUDIO_END_CALLBACK_PFUNC end_callback_func
                            )
{
    AUDIO_RESULT_E result = AUDIO_ERROR;
    HAUDIO         hAudioHandle = 0;

    //SCI_TRACE_MID("[MMIAUDIO]:MMIAUDIO_AudioPlay!");
    s_cur_audio_handle_type = audio_handle_type;
    switch(audio_handle_type)
    {
        case MMIAUDIO_APP:
            s_app_end_callback_func = end_callback_func;
            result = AUDIO_Play(s_app_audio_handle, uiOffset);
            hAudioHandle = s_app_audio_handle;
            break;

        case MMIAUDIO_PLAYER:
            s_player_end_callback_func = end_callback_func;
            result = AUDIO_Play(s_player_audio_handle, uiOffset);
            hAudioHandle = s_player_audio_handle;
            break;

        case MMIAUDIO_MUSIC:
            s_music_end_callback_func = end_callback_func;
            result = AUDIO_Play(s_music_audio_handle, uiOffset);
            hAudioHandle = s_music_audio_handle;
            break;

        case MMIAUDIO_NORMAL:
            s_normal_end_callback_func = end_callback_func;
            result = AUDIO_Play(s_normal_audio_handle, uiOffset);
            hAudioHandle = s_normal_audio_handle;
            break;
            
        case MMIAUDIO_KURO:
            s_kuro_end_callback_func = end_callback_func;
            result = AUDIO_Play(s_kuro_audio_handle, uiOffset);
            hAudioHandle = s_kuro_audio_handle;
            break;

#ifdef JAVA_SUPPORT
        case MMIAUDIO_JAVA:
            {
                s_java_end_callback_func = end_callback_func;
                result = AUDIO_Play(s_java_audio_handle, uiOffset);
                hAudioHandle = s_java_audio_handle;
            }
            
            break;
#endif
#ifdef WRE_SUPPORT
		case MMIAUDIO_WRE:
		{
				s_wre_end_callback_func = end_callback_func;
				result = AUDIO_Play(s_wre_audio_handle, uiOffset);
				hAudioHandle = s_wre_audio_handle;
		}
					
		break;
#endif
        default:
            break;
    }

    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_AudioPlay,  audio_handle_type = 0x%x, uiOffset = %d, play_times = %d, func = 0x%x, aud_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_3762_112_2_18_2_3_34_60,(uint8*)"ddddd",audio_handle_type,uiOffset,play_times,end_callback_func,result);
    
    if(AUDIO_NO_ERROR == result)
    {
        s_audio_play_times = play_times;
        s_cur_audio_handle_type = audio_handle_type;
        HandleAudioPriority(hAudioHandle);
        return TRUE;
    }
    else
    {
        s_cur_audio_handle_type = MMIAUDIO_NONE;
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : audio play 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioPlayAsy(
                            MMIAUDIO_HTYPE   audio_handle_type,
                            uint32           uiOffset,
                            uint32           play_times,
                            MMIAUDIO_END_CALLBACK_PFUNC end_callback_func
                            )
{
    AUDIO_RESULT_E result = AUDIO_ERROR;
    HAUDIO         hAudioHandle = 0;

    //SCI_TRACE_MID("[MMIAUDIO]:MMIAUDIO_AudioPlayAsy!!");
    s_cur_audio_handle_type = audio_handle_type;
    switch(audio_handle_type)
    {
        case MMIAUDIO_APP:
            s_app_end_callback_func = end_callback_func;
            result = AUDIO_ASYPlay(s_app_audio_handle, uiOffset);
            hAudioHandle = s_app_audio_handle;
            break;

        case MMIAUDIO_PLAYER:
            s_player_end_callback_func = end_callback_func;
            result = AUDIO_ASYPlay(s_player_audio_handle, uiOffset);
            hAudioHandle = s_player_audio_handle;
            break;

        case MMIAUDIO_MUSIC:
            s_music_end_callback_func = end_callback_func;
            result = AUDIO_ASYPlay(s_music_audio_handle, uiOffset);
            hAudioHandle = s_music_audio_handle;
            break;
            
        case MMIAUDIO_NORMAL:
            s_normal_end_callback_func = end_callback_func;
            result = AUDIO_ASYPlay(s_normal_audio_handle, uiOffset);
            hAudioHandle = s_normal_audio_handle;
            break;
            
        case MMIAUDIO_KURO:
            s_kuro_end_callback_func = end_callback_func;
            result = AUDIO_ASYPlay(s_kuro_audio_handle, uiOffset);
            hAudioHandle = s_kuro_audio_handle;
            break;

#ifdef JAVA_SUPPORT
        case MMIAUDIO_JAVA:
            {
                s_java_end_callback_func = end_callback_func;
                result = AUDIO_Play(s_java_audio_handle, uiOffset);
                hAudioHandle = s_java_audio_handle;
            }
            
            break;
#endif
#ifdef WRE_SUPPORT
		case MMIAUDIO_WRE:
		{
				s_wre_end_callback_func = end_callback_func;
				result = AUDIO_Play(s_wre_audio_handle, uiOffset);
				hAudioHandle = s_wre_audio_handle;
		}
            break;
#endif
        default:
            break;
    }

    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_AudioPlayAsy,  audio_handle_type = 0x%x, uiOffset = %d, play_times = %d, func = 0x%x, aud_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_3850_112_2_18_2_3_34_61,(uint8*)"ddddd",audio_handle_type,uiOffset,play_times,end_callback_func,result);
    
    if(AUDIO_NO_ERROR == result)
    {
        s_audio_play_times = play_times;
        s_cur_audio_handle_type = audio_handle_type;
        HandleAudioPriority(hAudioHandle);
        return TRUE;
    }
    else
    {
        s_cur_audio_handle_type = MMIAUDIO_NONE;
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : Handle Audio Priority 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void HandleAudioPriority(
                HAUDIO hAudioHandle
                )
{
    AUDIO_RESULT_E aud_result = AUDIO_NO_ERROR;
    
    if(MMIAUDIO_IsAppBusy())
    {
        aud_result = AUDIO_ChangeCodecPriority(
                            hAudioHandle,
                            TRUE
                            );
        //SCI_TRACE_LOW:"[MMIAUDIO]:HandleAudioPriority, hAudioHandle = 0x%x, aud_result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_3883_112_2_18_2_3_34_62,(uint8*)"dd",hAudioHandle,aud_result);
    }
}

/*****************************************************************************/
//  Description : stop audio play 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_AudioStop(MMIAUDIO_HTYPE audio_handle_type)
{
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_AudioStop:audio_type=%d, cur_type = %d, app_handle = 0x%X, player_handle = 0x%X, music_handle = 0x%X, kuro_handle = 0x%X,normal_handle = 0x%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_3894_112_2_18_2_3_34_63,(uint8*)"ddddddd", audio_handle_type,s_cur_audio_handle_type,s_app_audio_handle,s_player_audio_handle,s_music_audio_handle,s_kuro_audio_handle,s_normal_audio_handle);

    if (MMIAUDIO_NONE == audio_handle_type)
    {
        return;
    }

    switch(audio_handle_type)
    {
        case MMIAUDIO_APP:
            if(PNULL != s_app_audio_handle)
            {
                AudioPlayStop(s_app_audio_handle);
                #if 0
                /* 异步*/
                AudioSendStopMsg(s_app_audio_handle);
                /*
                RemovePlayCallback(s_app_audio_handle);
                s_app_audio_handle = PNULL;
                if(audio_handle_type == s_cur_audio_handle_type)
                {
                    s_cur_audio_handle_type = MMIAUDIO_NONE;
                }
                */
                #else
                /* 同步*/
                AudioStopCallback(s_app_audio_handle, MMIAUDIO_RLT_STOP, PNULL);            
                RemovePlayCallback(s_app_audio_handle);
                s_app_audio_handle = PNULL;
                if(audio_handle_type == s_cur_audio_handle_type)
                {
                    s_cur_audio_handle_type = MMIAUDIO_NONE;
                } 
                #endif
            }
            break;

        case MMIAUDIO_PLAYER:
            if(PNULL != s_player_audio_handle)
            {
                AudioPlayStop(s_player_audio_handle);
                RemovePlayCallback(s_player_audio_handle);
                s_player_audio_handle = PNULL;
                if(audio_handle_type == s_cur_audio_handle_type)
                {
                    s_cur_audio_handle_type = MMIAUDIO_NONE;
                }
            }
            break;

        case MMIAUDIO_MUSIC:
            if(PNULL != s_music_audio_handle)
            {
                AudioPlayStop(s_music_audio_handle);
                RemovePlayCallback(s_music_audio_handle);
                s_music_audio_handle = PNULL;
                if(audio_handle_type == s_cur_audio_handle_type)
                {
                    s_cur_audio_handle_type = MMIAUDIO_NONE;
                }
            }
            break;
            
        case MMIAUDIO_NORMAL:
            if(PNULL != s_normal_audio_handle)
            {
                AudioPlayStop(s_normal_audio_handle);
                RemovePlayCallback(s_normal_audio_handle);
                s_normal_audio_handle = PNULL;
                if(audio_handle_type == s_cur_audio_handle_type)
                {
                    s_cur_audio_handle_type = MMIAUDIO_NONE;
                }
            }
            break;
            
        case MMIAUDIO_KURO:
            if(PNULL != s_kuro_audio_handle)
            {
                AudioPlayStop(s_kuro_audio_handle);
                RemovePlayCallback(s_kuro_audio_handle);
                s_kuro_audio_handle = PNULL;
                if(audio_handle_type == s_cur_audio_handle_type)
                {
                    s_cur_audio_handle_type = MMIAUDIO_NONE;
                }
            }
            break;

#ifdef JAVA_SUPPORT      
        case MMIAUDIO_JAVA:
            {
                if(PNULL != s_java_audio_handle)
                {
                    AudioPlayStop(s_java_audio_handle);
                    RemovePlayCallback(s_java_audio_handle);
                    s_java_audio_handle = PNULL;
                    if(audio_handle_type == s_cur_audio_handle_type)
                    {
                        s_cur_audio_handle_type = MMIAUDIO_NONE;
                    }
                }
            }

            break;
#endif
#ifdef WRE_SUPPORT
		case MMIAUDIO_WRE:
			{
				if(PNULL != s_wre_audio_handle)
				{
					AudioPlayStop(s_wre_audio_handle);
					RemovePlayCallback(s_wre_audio_handle);
					s_wre_audio_handle = PNULL;
					if(audio_handle_type == s_cur_audio_handle_type)
					{
						s_cur_audio_handle_type = MMIAUDIO_NONE;
					}
				}
			}
		
            break;
#endif
        default:
            break;
    }
}

/*****************************************************************************/
//  Description : close audio handle 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_CloseAudioHandle(MMIAUDIO_HTYPE audio_handle_type)
{
    AUDIO_RESULT_E result = AUDIO_ERROR;

    if(audio_handle_type != s_cur_audio_handle_type)
    {
        //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_CloseAudioHandle, Alert!, s_cur_audio_handle_type = %d, audio_handle_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_4032_112_2_18_2_3_34_64,(uint8*)"dd", s_cur_audio_handle_type, audio_handle_type);
    }
        
    switch(audio_handle_type)
    {
        case MMIAUDIO_APP:
            if (PNULL != s_app_audio_handle)
            {
                result = AUDIO_CloseHandle(s_app_audio_handle);
                s_app_audio_handle = PNULL;
            }
            break;

        case MMIAUDIO_PLAYER:
            if (PNULL != s_player_audio_handle)
            {
                result = AUDIO_CloseHandle(s_player_audio_handle);
                s_player_audio_handle = PNULL;
            }
            break;

        case MMIAUDIO_MUSIC:
            if (PNULL != s_music_audio_handle)
            {
                result = AUDIO_CloseHandle(s_music_audio_handle);
                s_music_audio_handle = PNULL;
            }
            break;
            
        case MMIAUDIO_NORMAL:
            if (PNULL != s_normal_audio_handle)
            {
                result = AUDIO_CloseHandle(s_normal_audio_handle);
                s_normal_audio_handle = PNULL;
            }
            break;

        case MMIAUDIO_KURO:
            if (PNULL != s_kuro_audio_handle)
            {
                result = AUDIO_CloseHandle(s_kuro_audio_handle);
                s_kuro_audio_handle = PNULL;               
            }
            break;

#ifdef JAVA_SUPPORT
        case MMIAUDIO_JAVA:
            {
                if (PNULL != s_java_audio_handle)
                {
                    result = AUDIO_CloseHandle(s_java_audio_handle);
                    s_java_audio_handle = PNULL;               
                }
            }

            break;
#endif
#ifdef WRE_SUPPORT
		case MMIAUDIO_WRE:
			{
				if (PNULL != s_wre_audio_handle)
				{
					result = AUDIO_CloseHandle(s_wre_audio_handle);
					s_wre_audio_handle = PNULL;			   
				}
			}
		
			break;
#endif
        default:
            break;
    }
    //SCI_TRACE_LOW:"[MMIAUDIO]: MMIAUDIO_CloseAudioHandle:audio_handle_type=%d, aud_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_4104_112_2_18_2_3_34_65,(uint8*)"dd", audio_handle_type, result);
    if(AUDIO_NO_ERROR == result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : API of audio play pause
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioPause(MMIAUDIO_HTYPE audio_handle_type)
{
    AUDIO_RESULT_E result = AUDIO_ERROR;
    BOOLEAN        is_play_end = FALSE;

    s_cur_audio_handle_type = MMIAUDIO_NONE;

    switch(audio_handle_type)
    {
        case MMIAUDIO_APP:
            if (PNULL != s_app_audio_handle)
            {
                result = AUDIO_Pause(s_app_audio_handle);
                is_play_end = IsAudioPlayEnd(s_app_audio_handle);
            }
            break;

        case MMIAUDIO_PLAYER:
            if (PNULL != s_player_audio_handle)
            {
                result = AUDIO_Pause(s_player_audio_handle);
                is_play_end = IsAudioPlayEnd(s_player_audio_handle);
            }
            break;

        case MMIAUDIO_MUSIC:
            if (PNULL != s_music_audio_handle)
            {
                result = AUDIO_Pause(s_music_audio_handle);
                is_play_end = IsAudioPlayEnd(s_music_audio_handle);
            }
            break;
            
        case MMIAUDIO_NORMAL:
            if (PNULL != s_normal_audio_handle)
            {
                result = AUDIO_Pause(s_normal_audio_handle);
                is_play_end = IsAudioPlayEnd(s_normal_audio_handle);
            }
            break;

        case MMIAUDIO_KURO:
            if (PNULL != s_kuro_audio_handle)
            {
                result = AUDIO_Pause(s_kuro_audio_handle);
                is_play_end = IsAudioPlayEnd(s_kuro_audio_handle);
            }
            break;

#ifdef JAVA_SUPPORT
        case MMIAUDIO_JAVA:
            {
                if (PNULL != s_java_audio_handle)
                {
                    result = AUDIO_Pause(s_java_audio_handle);
					is_play_end = IsAudioPlayEnd(s_java_audio_handle);
                }
            }

            break;
#endif
#ifdef WRE_SUPPORT
		case MMIAUDIO_WRE:
			{
				if (PNULL != s_wre_audio_handle)
				{
					result = AUDIO_Pause(s_wre_audio_handle);
					is_play_end = IsAudioPlayEnd(s_wre_audio_handle);
				}
			}
		
			break;
#endif
        default:
            break;
    }

    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_AudioPause:audio_handle_type=%d,aud_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_4196_112_2_18_2_3_34_66,(uint8*)"dd", audio_handle_type,result);

    if(AUDIO_NO_ERROR == result && !is_play_end)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : API of audio play resume
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioResume(MMIAUDIO_HTYPE audio_handle_type)
{
    AUDIO_RESULT_E result = AUDIO_ERROR;

    s_cur_audio_handle_type = audio_handle_type;

    switch(audio_handle_type)
    {
        case MMIAUDIO_APP:
            if (PNULL != s_app_audio_handle)
            {
                result = AUDIO_Resume(s_app_audio_handle);
            }
            break;

        case MMIAUDIO_PLAYER:
            if (PNULL != s_player_audio_handle)
            {
                result = AUDIO_Resume(s_player_audio_handle);
            }
            break;

        case MMIAUDIO_MUSIC:
            if (PNULL != s_music_audio_handle)
            {
                result = AUDIO_Resume(s_music_audio_handle);
            }
            break;
            
        case MMIAUDIO_NORMAL:
            if (PNULL != s_normal_audio_handle)
            {
                result = AUDIO_Resume(s_normal_audio_handle);
            }
            break;

        case MMIAUDIO_KURO:
            if (PNULL != s_kuro_audio_handle)
            {
                result = AUDIO_Resume(s_kuro_audio_handle);
            }
            break;

#ifdef JAVA_SUPPORT
        case MMIAUDIO_JAVA:
            {
                if (PNULL != s_java_audio_handle)
                {
                    result = AUDIO_Resume(s_java_audio_handle);
                }
            }
            break;
#endif
#ifdef WRE_SUPPORT
		case MMIAUDIO_WRE:
			{
				if (PNULL != s_wre_audio_handle)
				{
					result = AUDIO_Resume(s_wre_audio_handle);
				}
            }
            break;
#endif
        default:
            break;
    }
    
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_AudioResume:audio_handle_type=%d,aud_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_4279_112_2_18_2_3_35_67,(uint8*)"dd", audio_handle_type,result);
    if(AUDIO_NO_ERROR == result)
    {
        return TRUE;
    }
    else
    {
        s_cur_audio_handle_type = MMIAUDIO_NONE;
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : API of audio play seek
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioSeek(
                                  MMIAUDIO_HTYPE    audio_handle_type,
                                  uint32            uiOffset
                                  )
{
    BOOLEAN         result = FALSE;
    AUDIO_RESULT_E  audio_result = AUDIO_ERROR;
    
    switch(audio_handle_type)
    {
    case MMIAUDIO_APP:
        if (PNULL != s_app_audio_handle)
        {
            audio_result = AUDIO_Seek(s_app_audio_handle, uiOffset);
        }
        break;

    case MMIAUDIO_PLAYER:
        if (PNULL != s_player_audio_handle)
        {
            audio_result = AUDIO_Seek(s_player_audio_handle, uiOffset);
        }
        break;

    case MMIAUDIO_MUSIC:
        if (PNULL != s_music_audio_handle)
        {
            audio_result = AUDIO_Seek(s_music_audio_handle, uiOffset);
        }
        break;

    case MMIAUDIO_NORMAL:
        if (PNULL != s_normal_audio_handle)
        {
            audio_result = AUDIO_Seek(s_normal_audio_handle, uiOffset);
        }
        break;
#ifdef JAVA_SUPPORT        
    case MMIAUDIO_JAVA:
        if (PNULL != s_java_audio_handle)
        {
            audio_result = AUDIO_Seek(s_java_audio_handle, uiOffset);
        }
        break;
#endif        
#ifdef WRE_SUPPORT
	case MMIAUDIO_WRE:
		if (PNULL != s_wre_audio_handle)
		{
			audio_result = AUDIO_Seek(s_wre_audio_handle, uiOffset);
		}
		break;
#endif  
    default:
        break;
    }
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_AudioSeek:audio_handle_type = %d, uiOffset=%ld,audio_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_4352_112_2_18_2_3_35_68,(uint8*)"ddd", audio_handle_type, uiOffset,audio_result);
    
    if (AUDIO_NO_ERROR == audio_result)
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
    return (result);
}
/*****************************************************************************/
//  Description : Seek by time offset(ms).
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioSeekByTime(
                        MMIAUDIO_HTYPE audio_handle_type, 
                        uint32 time_offset
                        )
{
    BOOLEAN result = FALSE;
    uint32  byte_offset = FALSE;
    //AUDIO_FORMATINFO_T audio_info = {0};
    AUDIO_PLAYINFO_T   audio_info = {0};
    
    AUDIO_RESULT_E  audio_result = AUDIO_ERROR;
    HAUDIOCODEC     audio_codec  = 0;
    MMIAUDIO_TYPE_E audio_type   = MMIAUDIO_MIDI;
    HAUDIO          audio_handle = 0;

    audio_handle = MMIAUDIO_GetAudioHandle(audio_handle_type);
    if(NULL != audio_handle)
    {
        audio_codec = AUDIO_GetCodecHandle(audio_handle);
        audio_type = GetAudioType(audio_codec);

        if(MMIAUDIO_IsSeekSupport(audio_type))
        {
            //if(MMIAUDIO_AudioGetFileInfo(audio_handle_type, &audio_info))
            if(MMIAUDIO_AudioGetPlayingFileInfo(audio_handle_type, &audio_info))
            {
                if(audio_info.uiTotalTime > 0     &&
                   audio_info.uiTotalDataLength > 0)
                {
                    byte_offset = (uint32)((uint64)time_offset * audio_info.uiTotalDataLength / audio_info.uiTotalTime);
                    audio_result = AUDIO_Seek(audio_handle, byte_offset);
                }
            }
        }
    }

    //SCI_TRACE_LOW:"[MMIAUDIO]: MMIAUDIO_AudioSeekByTime, audio_handle_type = %d, time_offset = %d, byte_offset = %d, audio_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_4404_112_2_18_2_3_35_69,(uint8*)"dddd",audio_handle_type, time_offset,byte_offset,audio_result);

    if (AUDIO_NO_ERROR == audio_result)
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : To judge whether the music is supported for seeking.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsSeekSupport(
                    MMIAUDIO_TYPE_E audio_type
                    )
{
    BOOLEAN            result = FALSE; 
	
    switch(audio_type)
    {
    case MMIAUDIO_MP3:
#ifdef MP3_SEEK_SUPPORT
        result = TRUE;
#endif
        break;
    case MMIAUDIO_WMA:
#ifdef WMA_SEEK_SUPPORT
        result = TRUE;
#endif
        break;
    case MMIAUDIO_AAC:
#ifdef AAC_SEEK_SUPPORT
        result = TRUE;
#endif
        break;
    case MMIAUDIO_AMR:
#ifdef AMR_SEEK_SUPPORT
        result = TRUE;
#endif
        break;
    case MMIAUDIO_MIDI:
#ifdef MIDI_SEEK_SUPPORT
        result = TRUE;
#endif
        break;
    case MMIAUDIO_WAVE:
#ifdef WAV_SEEK_SUPPORT
        result = TRUE;
#endif
        break;
    default:
        break;
    }

    //SCI_TRACE_LOW:"[MMIAUDIO]: MMIAUDIO_IsSeekSupport, audio type = %d, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_4464_112_2_18_2_3_35_70,(uint8*)"dd", audio_type,result);
    return result;
}

/*****************************************************************************/
//  Description : audio play stop
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void AudioPlayStop(HAUDIO hAudioHandle)
{
    AUDIO_RESULT_E  stop_result = AUDIO_ERROR;
    AUDIO_RESULT_E  close_result = AUDIO_ERROR;
    
    if (PNULL != hAudioHandle)
    {
        stop_result  = AUDIO_Stop(hAudioHandle);
                
        close_result = AUDIO_CloseHandle(hAudioHandle);
    
        hAudioHandle = PNULL;
    }
    //SCI_TRACE_LOW:"[MMIAUDIO]:AudioPlayStop:hAudioHandle = %x, stop_result=%ld,close_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_4486_112_2_18_2_3_35_71,(uint8*)"ddd", hAudioHandle,stop_result, close_result);
}

/*****************************************************************************/
//  Description : API of audio play set speed
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_AudioSetSpeed(MMIAUDIO_HTYPE audio_handle_type, uint32 uiSpeedX)
{
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_AudioSetSpeed:audio_handle_type=%d, uiSpeedX=%ld"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_4495_112_2_18_2_3_35_72,(uint8*)"dd", audio_handle_type, uiSpeedX);

    switch(audio_handle_type)
    {
        case MMIAUDIO_APP:
            if (PNULL != s_app_audio_handle)
            {
                AUDIO_SetSpeed(s_app_audio_handle, uiSpeedX);
            }
            break;

        case MMIAUDIO_PLAYER:
            if (PNULL != s_player_audio_handle)
            {
                AUDIO_SetSpeed(s_player_audio_handle, uiSpeedX);
            }
            break;

        case MMIAUDIO_MUSIC:
            if (PNULL != s_music_audio_handle)
            {
                AUDIO_SetSpeed(s_music_audio_handle, uiSpeedX);
            }
            break;

        case MMIAUDIO_NORMAL:
            if (PNULL != s_normal_audio_handle)
            {
                AUDIO_SetSpeed(s_normal_audio_handle, uiSpeedX);
            }
            break;
#ifdef JAVA_SUPPORT   
        case MMIAUDIO_JAVA:
            if (PNULL != s_java_audio_handle)
            {
                AUDIO_SetSpeed(s_java_audio_handle, uiSpeedX);
            }
            break;
#endif            
#ifdef WRE_SUPPORT
		case MMIAUDIO_WRE:
			if (PNULL != s_wre_audio_handle)
			{
				AUDIO_SetSpeed(s_wre_audio_handle, uiSpeedX);
			}
		break;
#endif  
        default:
            break;
    }    
}

/*****************************************************************************/
//  Description : API of audio get file info(cann't callback during play, only use after create handle)
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioGetFileInfo(MMIAUDIO_HTYPE audio_handle_type, AUDIO_FORMATINFO_T* ptfileInfo)
{
    AUDIO_RESULT_E result = AUDIO_ERROR;

    switch(audio_handle_type)
    {
        case MMIAUDIO_APP:
            result = AUDIO_IdentifyAudioFormat(s_app_audio_handle, ptfileInfo);
            break;

        case MMIAUDIO_PLAYER:
             result = AUDIO_IdentifyAudioFormat(s_player_audio_handle, ptfileInfo);
            break;

        case MMIAUDIO_MUSIC:
            if (PNULL != s_music_audio_handle)
            {
                result = AUDIO_IdentifyAudioFormat(s_music_audio_handle, ptfileInfo);
            }
            break;
            
        case MMIAUDIO_NORMAL:
             result = AUDIO_IdentifyAudioFormat(s_normal_audio_handle, ptfileInfo);
            break;

        case MMIAUDIO_KURO:
            result = AUDIO_IdentifyAudioFormat(s_kuro_audio_handle, ptfileInfo);
            break;

#ifdef JAVA_SUPPORT               
        case MMIAUDIO_JAVA:
            result = AUDIO_IdentifyAudioFormat(s_java_audio_handle, ptfileInfo);
            break;
#endif
#ifdef WRE_SUPPORT
		case MMIAUDIO_WRE:
			result = AUDIO_IdentifyAudioFormat(s_wre_audio_handle, ptfileInfo);
            break;
#endif
        default:
            break;
    }    

    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_AudioGetFileInfo:audio_handle_type=%d,aud_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_4594_112_2_18_2_3_35_73,(uint8*)"dd", audio_handle_type,result);
    if(AUDIO_NO_ERROR == result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : API of audio play get playing file info
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_AudioGetPlayingFileInfo(MMIAUDIO_HTYPE audio_handle_type, AUDIO_PLAYINFO_T* ptPlayInfo)
{
    AUDIO_RESULT_E result = AUDIO_ERROR;
    
    switch(audio_handle_type)
    {
        case MMIAUDIO_APP:
            if (PNULL != s_app_audio_handle)
            {
                result = AUDIO_GetPlayingInfo(s_app_audio_handle, ptPlayInfo);
            }
            break;

        case MMIAUDIO_PLAYER:
            if (PNULL != s_player_audio_handle)
            {
                result = AUDIO_GetPlayingInfo(s_player_audio_handle, ptPlayInfo);
            }
            break;

        case MMIAUDIO_MUSIC:
            if (PNULL != s_music_audio_handle)
            {
                result = AUDIO_GetPlayingInfo(s_music_audio_handle, ptPlayInfo);
            }
            break;

        case MMIAUDIO_NORMAL:
            if (PNULL != s_normal_audio_handle)
            {
                result = AUDIO_GetPlayingInfo(s_normal_audio_handle, ptPlayInfo);
            }
            break;

        case MMIAUDIO_KURO:
            if (PNULL != s_kuro_audio_handle)
            {
                result = AUDIO_GetPlayingInfo(s_kuro_audio_handle, ptPlayInfo);
            }
            break;
#ifdef JAVA_SUPPORT               
        case MMIAUDIO_JAVA:
            if (PNULL != s_java_audio_handle)
            {
                result = AUDIO_GetPlayingInfo(s_java_audio_handle, ptPlayInfo);
            }
            break;
#endif
#ifdef WRE_SUPPORT
		case MMIAUDIO_WRE:
			if (PNULL != s_wre_audio_handle)
			{
				result = AUDIO_GetPlayingInfo(s_wre_audio_handle, ptPlayInfo);
			}
            break;
#endif
        default:
            break;
    }
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_AudioGetPlayingFileInfo:audio_handle_type=%x, cur_time = %d, cur_offset = %d, t_time = %d, t_len = %d, aud_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_4669_112_2_18_2_3_35_74,(uint8*)"dddddd",audio_handle_type, ptPlayInfo->uiCurrentTime,ptPlayInfo->uiCurrentDataOffset,ptPlayInfo->uiTotalTime,ptPlayInfo->uiTotalDataLength,result);
    if(AUDIO_NO_ERROR == result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : MMI AUDIO Open Spectrum
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_OpenSpectrum(MMIAUDIO_HTYPE audio_handle_type)
{
    AUDIO_RESULT_E result = AUDIO_ERROR;
    
    switch(audio_handle_type)
    {
        case MMIAUDIO_APP:
            if (PNULL != s_app_audio_handle)
            {
                result = AUDIO_OpenSpectrum(s_app_audio_handle);
            }
            break;

        case MMIAUDIO_PLAYER:
            if (PNULL != s_player_audio_handle)
            {
                result = AUDIO_OpenSpectrum(s_player_audio_handle);
            }
            break;

        case MMIAUDIO_MUSIC:
            if (PNULL != s_music_audio_handle)
            {
                result = AUDIO_OpenSpectrum(s_music_audio_handle);
            }
            break;

        case MMIAUDIO_NORMAL:
            if (PNULL != s_normal_audio_handle)
            {
                result = AUDIO_OpenSpectrum(s_normal_audio_handle);
            }
            break;

        case MMIAUDIO_KURO:
            if (PNULL != s_kuro_audio_handle)
            {
                result = AUDIO_OpenSpectrum(s_kuro_audio_handle);
            }
            break;

        default:
            break;
    }
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_OpenSpectrum:audio_handle_type=%x, aud_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_4728_112_2_18_2_3_36_75,(uint8*)"dd",audio_handle_type, result);
    if(AUDIO_NO_ERROR == result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : MMI AUDIO Get Spectrum Info
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_GetSpectrum(
                    MMIAUDIO_HTYPE  audio_handle_type,
                    uint16          * spec_buf_ptr,         //[OUT]
                    uint16          * spec_num_ptr,         //[OUT] 目前长度无法验证，约定为32 或 33，即buf长度需要大于 32*sizeof(uint16),需要结构体
                    uint16          * max_spec_value_ptr    //[OUT]
                    )
{
    AUDIO_RESULT_E result = AUDIO_ERROR;
    
    switch(audio_handle_type)
    {
        case MMIAUDIO_APP:
            if (PNULL != s_app_audio_handle)
            {
                result = AUDIO_GetSpectrum(s_app_audio_handle,spec_buf_ptr,spec_num_ptr,max_spec_value_ptr);
            }
            break;

        case MMIAUDIO_PLAYER:
            if (PNULL != s_player_audio_handle)
            {
                result = AUDIO_GetSpectrum(s_player_audio_handle,spec_buf_ptr,spec_num_ptr,max_spec_value_ptr);
            }
            break;

        case MMIAUDIO_MUSIC:
            if (PNULL != s_music_audio_handle)
            {
                result = AUDIO_GetSpectrum(s_music_audio_handle,spec_buf_ptr,spec_num_ptr,max_spec_value_ptr);
            }
            break;

        case MMIAUDIO_NORMAL:
            if (PNULL != s_normal_audio_handle)
            {
                result = AUDIO_GetSpectrum(s_normal_audio_handle,spec_buf_ptr,spec_num_ptr,max_spec_value_ptr);
            }
            break;

        case MMIAUDIO_KURO:
            if (PNULL != s_kuro_audio_handle)
            {
                result = AUDIO_GetSpectrum(s_kuro_audio_handle,spec_buf_ptr,spec_num_ptr,max_spec_value_ptr);
            }
            break;

        default:
            break;
    }
    //SCI_TRACE_LOW("[MMIAUDIO]:MMIAUDIO_GetSpectrum:audio_handle_type=%x, aud_result = %d",audio_handle_type, result);
    if(AUDIO_NO_ERROR == result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : MMI AUDIO Close Spectrum
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_CloseSpectrum(MMIAUDIO_HTYPE audio_handle_type)
{
    AUDIO_RESULT_E result = AUDIO_ERROR;
    
    switch(audio_handle_type)
    {
        case MMIAUDIO_APP:
            if (PNULL != s_app_audio_handle)
            {
                result = AUDIO_CloseSpectrum(s_app_audio_handle);
            }
            break;

        case MMIAUDIO_PLAYER:
            if (PNULL != s_player_audio_handle)
            {
                result = AUDIO_CloseSpectrum(s_player_audio_handle);
            }
            break;

        case MMIAUDIO_MUSIC:
            if (PNULL != s_music_audio_handle)
            {
                result = AUDIO_CloseSpectrum(s_music_audio_handle);
            }
            break;

        case MMIAUDIO_NORMAL:
            if (PNULL != s_normal_audio_handle)
            {
                result = AUDIO_CloseSpectrum(s_normal_audio_handle);
            }
            break;

        case MMIAUDIO_KURO:
            if (PNULL != s_kuro_audio_handle)
            {
                result = AUDIO_CloseSpectrum(s_kuro_audio_handle);
            }
            break;

        default:
            break;
    }
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_CloseSpectrum:audio_handle_type=%x, aud_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_4851_112_2_18_2_3_36_76,(uint8*)"dd",audio_handle_type, result);
    if(AUDIO_NO_ERROR == result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : API of start record
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_StartRecord(
                    const wchar     *name_ptr,
                    MMIFILE_HANDLE  file_handle,    //file handle created, can be null
                    MMIAUDIO_RECORD_FORMAT_T format,
                    MMIAUDIO_RECORD_FRAME_LENGTH_T frame_length,
                    MMIAUDIO_RECORD_TYPE_E      record_type,
                    MMIAUDIO_END_CALLBACK_PFUNC end_callback_func
                    )
{
    HAUDIOCODEC hAudioCodec = 0;
    const uint16* pusCodecName = PNULL;
    HAUDIODEV hDevice = 0;
    const uint16* pusDevName = PNULL;
    RECORD_PARAM_T record_param = {AUD_REC_ADPCM, RECORD_OUTPUT_SAVE_TO_FILE, 0, ADPCM_FRAME_LENGTH, 0,0,0};
    MP3_ENC_CFG_T mp3_record_param = {AUD_REC_MP3, RECORD_OUTPUT_SAVE_TO_FILE, 0, 8000, 60000, 1,  0, 0, 4000 };//andy.he_record 

    RECORD_IF_FROM_NET_T record_from_net = {0};
    uint32          pram_result = 0;
    BOOLEAN         result      = FALSE;
    AUDIO_RESULT_E  aud_result  = AUDIO_ERROR;
    
    if(PNULL == name_ptr || frame_length >= MMIAUDIO_RECORD_FRAME_LEN_MAX)
    {
        return FALSE;
    }

    switch(record_type)
    {
    case MMIAUDIO_RECORD_NORMAL_APP:
        hDevice = hRECORDVB;
        hAudioCodec = hRecordCodec;
        record_from_net.b_if_from_net = FALSE;
        break;
        
    case MMIAUDIO_RECORD_CC:
        hDevice = hLAYER1;
        hAudioCodec = hRecordCodec;
        record_from_net.b_if_from_net = TRUE;
        break;
        
    case MMIAUDIO_RECORD_FM:
        hDevice = hLINEINADDev;
        hAudioCodec = hRecordCodec;
        record_from_net.b_if_from_net = FALSE;
        break;

    default:
        break;
    }

    switch(format)
    {
        case MMIAUDIO_RECORD_FORMAT_AMR:
            record_param.format = AUD_REC_AMR_SW;//andy.he_record   AUD_REC_AMR(两个限制1:无法获取db 2:CC中无法使用)
            record_param.frame_length = s_record_frame_length[frame_length];
            break;

        case MMIAUDIO_RECORD_FORMAT_ADPCM:
            //if(MMIAUDIO_RECORD_DEFAULT_FRAME_LEN == frame_length)
            //record_param.frame_length = ADPCM_FRAME_LENGTH;
        default:
            break;
    }

    if(SFS_INVALID_HANDLE != file_handle)
    {
        s_record_handle = AUDIO_CreateRecordFileHandle_FileCreated(
                                hAudioCodec,
                                pusCodecName,
                                hDevice,
                                pusDevName,
                                (const int16 *)name_ptr,
                                file_handle,
                                s_application_track_buf,
                                MMIAUDIO_TRACKBUFSIZE_APP,
                                AudioNotifyCallback
                                );
    }
    else
    {
        s_record_handle = AUDIO_CreateRecordFileHandle(
                                hAudioCodec,
                                pusCodecName,
                                hDevice,
                                pusDevName,
                                (const int16 *)name_ptr,
                                s_application_track_buf,
                                MMIAUDIO_TRACKBUFSIZE_APP,
                                AudioNotifyCallback
                                );
    }
 
    if (INVALID_HANDLE != s_record_handle)
    {
    if(MMIAUDIO_RECORD_FORMAT_MP3==format) //andy.he_record 
        {
             AUDIO_ExeCodecExtOpe(
            s_record_handle,
            ANSI2UINT16("SET_RECORDPARAM"),//扩展操作的标志 
            RECORD_MP3_PARAM_SET_COUNT,
            &mp3_record_param,
            &pram_result
            );
        }
    else
        {
            AUDIO_ExeCodecExtOpe(
            s_record_handle,
            ANSI2UINT16("SET_RECORDPARAM"),//扩展操作的标志 
            RECORD_PARAM_SET_COUNT,
            &record_param,
            &pram_result
            );
        }
        AUDIO_ExeCodecExtOpe(
            s_record_handle,
            ANSI2UINT16("SET_RECORDIFNET"),//扩展操作的标志 
            RECORD_SET_IF_FROM_NET_COUNT,
            &record_from_net,
            &pram_result
            );

        aud_result = AUDIO_Play(s_record_handle, 0);
        
        if(AUDIO_NO_ERROR != aud_result)
        {
            AUDIO_CloseHandle(s_record_handle);
            s_record_handle = PNULL;
        }
        else
        {
            result = TRUE;
        }
    }

    if(result)
    {
        s_cur_audio_handle_type = MMIAUDIO_APP;
        s_record_end_callback_func = end_callback_func;
    }
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_StartRecord:s_record_handle=0x%lx, file_handle = 0x%x, record_type = %d, aud_result = %d, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5007_112_2_18_2_3_36_77,(uint8*)"ddddd", s_record_handle,file_handle,record_type,aud_result,result);
    
    return result;
}

/*****************************************************************************/
//  Description : API of start record
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_StopRecord(void)
{
    AUDIO_RESULT_E  aud_result  = AUDIO_ERROR;
    BOOLEAN         result      = TRUE;
    
    if (PNULL != s_record_handle)
    {
        aud_result = AUDIO_Stop(s_record_handle);
        if(AUDIO_NO_ERROR == aud_result)
        {
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
        if(MMIAUDIO_APP == s_cur_audio_handle_type &&
           PNULL == s_app_audio_handle)
        {
            s_cur_audio_handle_type = MMIAUDIO_NONE;
        }
    }
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_StopRecord:s_record_handle = %x, aud_result=%ld, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5039_112_2_18_2_3_36_78,(uint8*)"ddd", s_record_handle,aud_result,result);
    return result;
}

/*****************************************************************************/
//  Description : API of end record (close handle)
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_EndRecord(void)
{
    AUDIO_RESULT_E  aud_result  = AUDIO_ERROR;
    BOOLEAN         result      = TRUE;

    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_EndRecord:s_record_handle = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5053_112_2_18_2_3_36_79,(uint8*)"d", s_record_handle);

    if (PNULL != s_record_handle)
    {
        aud_result = AUDIO_CloseHandle(s_record_handle);
        if(PNULL != s_IQ_data_buf)
        {
            SCI_FREE(s_IQ_data_buf);
        }
        s_record_handle = PNULL;
        if(AUDIO_NO_ERROR != aud_result)
        {
            result = FALSE;
        }
    }
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_EndRecord:aud_result=%ld, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5069_112_2_18_2_3_36_80,(uint8*)"dd", aud_result,result);
    return result;
}

/*****************************************************************************/
//  Description : API of pause record
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_PauseRecord(void)
{
    AUDIO_RESULT_E  aud_result  = AUDIO_ERROR;
    BOOLEAN         result      = TRUE;

    if (PNULL != s_record_handle)
    {
        aud_result = AUDIO_Pause(s_record_handle);
        if(AUDIO_NO_ERROR != aud_result)
        {
            result = FALSE;
        }
    }
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_PauseRecord:s_record_handle = %x, aud_result=%ld, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5091_112_2_18_2_3_36_81,(uint8*)"ddd", s_record_handle,aud_result,result);
    return result;
}

/*****************************************************************************/
//  Description : API of resume record
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_ResumeRecord(void)
{
    AUDIO_RESULT_E  aud_result  = AUDIO_ERROR;
    BOOLEAN         result      = TRUE;

    if (PNULL != s_record_handle)
    {
        aud_result = AUDIO_Resume(s_record_handle);
        if(AUDIO_NO_ERROR != aud_result)
        {
            result = FALSE;
        }
    }
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_ResumeRecord:s_record_handle = %x, aud_result=%ld, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5113_112_2_18_2_3_36_82,(uint8*)"ddd", s_record_handle,aud_result,result);
    return result;
}

/*****************************************************************************/
//  Description : API of is is audio playing
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_GetIsPlaying(void)
{
    if (MMIAUDIO_NONE == s_cur_audio_handle_type)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*****************************************************************************/
//  Description : API of is permit audio playing
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 播放优先级:MMIAUDIO_APP>MMIAUDIO_PLAYER>MMIAUDIO_NPRMAL
//          同一优先级的允许中断前一播放
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_GetIsPermitPlay(MMIAUDIO_HTYPE audio_htype)
{
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_GetIsPermitPlay:s_cur_audio_handle_type=%d, audio_htype=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5140_112_2_18_2_3_36_83,(uint8*)"dd", s_cur_audio_handle_type, audio_htype);

    if (MMIAUDIO_NONE == s_cur_audio_handle_type)
    {
        return TRUE;
    }
    else if (audio_htype <= s_cur_audio_handle_type)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : API of is get current play audio handle type
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC MMIAUDIO_HTYPE MMIAUDIO_GetAudioHtype(void)
{
    return s_cur_audio_handle_type;
}

/*****************************************************************************/
//  Description : API of is get audio handle
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC HAUDIO MMIAUDIO_GetAudioHandle(MMIAUDIO_HTYPE audio_handle_type)
{
    HAUDIO audio_handle = NULL;
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_GetAudioHandle:audio_handle_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5172_112_2_18_2_3_36_84,(uint8*)"d", audio_handle_type);

    switch(audio_handle_type)
    {
        case MMIAUDIO_APP:
            audio_handle = s_app_audio_handle;
            break;

        case MMIAUDIO_PLAYER:
            audio_handle = s_player_audio_handle;
            break;

        case MMIAUDIO_MUSIC:
            if (PNULL != s_music_audio_handle)
            {
                audio_handle = s_music_audio_handle;
            }
            break;
            
        case MMIAUDIO_NORMAL:
            audio_handle = s_normal_audio_handle;
            break;

        case MMIAUDIO_KURO:
            audio_handle = s_kuro_audio_handle;
            break;
#ifdef JAVA_SUPPORT   
        case MMIAUDIO_JAVA:
            audio_handle = s_java_audio_handle;
            break;
#endif            
#ifdef WRE_SUPPORT
		case MMIAUDIO_WRE:
			audio_handle = s_wre_audio_handle;
			break;
#endif 
        default:
//            SCI_TRACE_LOW("MMIAUDIO_GetAudioHandle:the audio_handle_type is invalid");
            break;
    }

    return audio_handle;
}


/*****************************************************************************/
//  Description : Init const audio handles.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL void AudioInitConstHandle(void)
{
    hDtmfRing     = AUDIO_CreateNormalHandle(hDtmfCodec,     SCI_NULL, hLAYER1,     SCI_NULL, AudioDummyNotifyCallback);
    hCustomRing   = AUDIO_CreateNormalHandle(hCustomCodec,   SCI_NULL, hLAYER1,     SCI_NULL, AudioDummyNotifyCallback);
    hGenericRing  = AUDIO_CreateNormalHandle(hGenericCodec,  SCI_NULL, hLAYER1,     SCI_NULL, AudioDummyNotifyCallback);
    hVoiceRing    = AUDIO_CreateNormalHandle(hVoiceCodec,    SCI_NULL, hLAYER1,     SCI_NULL, AudioDummyNotifyCallback);

#ifndef WIN32

#ifdef MIC_SAMPLE_SUPPORT
    hMicSample    =	AUDIO_CreateNormalHandle(hMicSampleCodec,SCI_NULL, hARMVBRECORD,SCI_NULL, AudioDummyNotifyCallback);
    SCI_ASSERT(NULL != hMicSample);/*assert verified*/  
#endif

#ifdef CAT_MVOICE_SUPPORT
   hCatMvsample=	AUDIO_CreateNormalHandle(hCatMvSampleCodec,SCI_NULL, hARMVBRECORD,SCI_NULL, AudioDummyNotifyCallback);
   SCI_ASSERT(NULL != hCatMvsample); /*assert verified*/ 
#endif

#endif
	hVoiceLoopbackRing = AUDIO_CreateNormalHandle(hVoiceCodec,    SCI_NULL, hLOOPBACKDEV,     SCI_NULL, AudioDummyNotifyCallback);
//    SCI_ASSERT(NULL != hDtmfRing);  //pclint 552
//    SCI_ASSERT(NULL != hCustomRing);
//    SCI_ASSERT(NULL != hGenericRing);
//    SCI_ASSERT(NULL != hVoiceRing);
//    SCI_ASSERT(NULL != hVoiceLoopbackRing);
}


/*****************************************************************************/
//  Description : play tone 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_PlayTone(
                MMIAUDIO_TYPE_E tone_type,
                uint32 tone_id,
                uint32 duration,
                uint32 freq
                )
{
    GENERIC_TONE_ADP_EXT_OPE_PLAY_PARA_T tGenericPara = {0};
    CUSTOM_TONE_ADP_EXT_OPE_PLAY_PARA_T tCustomPara = {0};
    DTMF_TONE_ADP_EXT_OPE_PLAY_PARA_T tDtmfPara = {0};
    uint32 result = 0;

    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_PlayTone:audio_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5266_112_2_18_2_3_37_85,(uint8*)"d", tone_type);

    switch(tone_type)
    {
        case MMIAUDIO_GENERIC_TONE:
            tGenericPara.tone_id = tone_id;

            tGenericPara.duration = duration;

            tGenericPara.hAudioHandle = hGenericRing;
            
            tGenericPara.volume = 16384;

            AUDIO_ExeCodecExtOpe(
                hGenericRing,
                ANSI2UINT16("EXT_PLAY"),
                GENERIC_TONE_ADP_EXT_OPE_PLAY_PARA_COUNT,
                &tGenericPara,
                &result
                );
            break;

        case MMIAUDIO_CUSTOM_TONE:
            tCustomPara.freq1 = freq;

            tCustomPara.freq2 = freq;

            tCustomPara.freq3 = freq;

            tCustomPara.duration = duration;

            tCustomPara.hAudioHandle = hCustomRing;
            
            tCustomPara.volume = 16384;

            AUDIO_ExeCodecExtOpe(
                hCustomRing,
                ANSI2UINT16("EXT_PLAY"),
                CUSTOM_TONE_ADP_EXT_OPE_PLAY_PARA_COUNT,
                &tCustomPara,
                &result
                );
            break;

        case MMIAUDIO_DTMF_TONE:
            tDtmfPara.tone_id = tone_id;

            tDtmfPara.duration = duration;

            tDtmfPara.is_uplink = TRUE;

            tDtmfPara.hAudioHandle = hDtmfRing;
            
            tDtmfPara.volume = 4096;

            AUDIO_ExeCodecExtOpe(
                hDtmfRing,
                ANSI2UINT16("EXT_PLAY"),
                DTMF_TONE_ADP_EXT_OPE_PLAY_PARA_COUNT,
                &tDtmfPara,
                &result
                );
            break;

        default:
//            SCI_TRACE_LOW("MMIAUDIO_PlayTone:the tone_type is invalid");
            break;
    }
}

/*****************************************************************************/
//  Description : tone stop 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_StopTone(
                MMIAUDIO_TYPE_E tone_type
                )
{
    uint32 result = 0;

    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_StopTone:audio_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5346_112_2_18_2_3_37_86,(uint8*)"d", tone_type);

    switch(tone_type)
    {
        case MMIAUDIO_GENERIC_TONE:
            AUDIO_ExeCodecExtOpe(
                hGenericRing,
                ANSI2UINT16("EXT_STOP"),
                0,
                PNULL,
                &result
                );
            break;

        case MMIAUDIO_CUSTOM_TONE:
            AUDIO_ExeCodecExtOpe(
                hCustomRing,
                ANSI2UINT16("EXT_STOP"),
                0,
                PNULL,
                &result
                );
            break;

        case MMIAUDIO_DTMF_TONE:
            AUDIO_ExeCodecExtOpe(
                hDtmfRing,
                ANSI2UINT16("EXT_STOP"),
                0,
                PNULL,
                &result
                );
            break;

        default:
//            SCI_TRACE_LOW("MMIAUDIO_StopTone:the tone_type is invalid");
            break;
    }
}

#ifdef MP3_EQ_SUPPORT 
/*****************************************************************************/
//  Description : set eq mode
//  Global resource dependence : hEQExp
//  Author: haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAUDIO_SetEQMode(MMIAUDIO_EQ_MODE_E eq_mode)  //0—低音,1—高音,2—声乐,3—摇滚,4—流行,5—古典,6—强劲,7—常规
{  
    BOOLEAN result = SCI_FALSE;

    if(eq_mode >= MMIAUDIO_EQ_MAX_MODE)
    {
        //SCI_TRACE_LOW:"[MMIAUDIO]: MMIAUDIO_SetEQMode(), input error, MMI eq_mode = %d, "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5397_112_2_18_2_3_37_87,(uint8*)"d",eq_mode);
        eq_mode = MMIAUDIO_EQ_REGULAR;
    }

    #ifdef PLATFORM_SC6600L
    if(!s_audio_is_eq_enable)
    {
        return;
    }
    #endif
    //  If just setting eq mode but not setting sampling frequency, then should set argc to 0
    //  If just setting sampling frequency but not setting eq mode, then should set argv to 0xFF
    //  If setting argv to 0xFF and setting argc to 0, then it will set eq mode to PASS(regular mode)
    //  sample frequency: 8000， 11025，12000，16000，22050，24000，32000，44100，48000 (in the unit of Hz)
    //  mmi should not set sample frequency!
    if(eq_mode != s_audio_cur_eq_mode)
    {
    #ifdef PLATFORM_SC6600L
        MMIAUDIO_StopAGC();

        if(eq_mode!=MMIAUDIO_EQ_REGULAR)
        {
            AUD_AGC_LockSwitch(TRUE);   
        }
        else
        {
            AUD_AGC_LockSwitch(FALSE);
        }
    #endif
    
        result = AUD_SetEqMode(ConvertEQType(eq_mode));

    }
    //SCI_TRACE_LOW:"[MMIAUDIO]: MMIAUDIO_SetEQMode(), orig_EQ = %d, new_EQ = %d, aud_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5430_112_2_18_2_3_37_88,(uint8*)"ddd",s_audio_cur_eq_mode, eq_mode, result);

    //check value
    if(result)
    {
        s_audio_cur_eq_mode = eq_mode;
    }

    #ifdef PLATFORM_SC6600L
    if(MMIAPIENVSET_GetEnhanceRingState())
    {
        MMIAUDIO_StartAGC();
    }
    #endif

    return ;
}

/*****************************************************************************/
//  Description : Convert EQ type
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL EQ_EXP_MODE_E ConvertEQType(
                        MMIAUDIO_EQ_MODE_E eq_mode
                        )
{
    EQ_EXP_MODE_E ref_eq_mode = EQ_EXP_MODE_PASS;
    switch(eq_mode)
    {
    case MMIAUDIO_EQ_REGULAR:
        ref_eq_mode = EQ_EXP_MODE_PASS;
        break;
    case MMIAUDIO_EQ_CLASSIC:
        ref_eq_mode = EQ_EXP_MODE_CLASSIC;
        break;
    case MMIAUDIO_EQ_ODEUM:
        ref_eq_mode = EQ_EXP_MODE_POP;
        break;
    case MMIAUDIO_EQ_JAZZ:
        ref_eq_mode = EQ_EXP_MODE_VOCAL;
        break;
    case MMIAUDIO_EQ_ROCK:
        ref_eq_mode = EQ_EXP_MODE_POWER;
        break;
    case MMIAUDIO_EQ_SOFT_ROCK:
        ref_eq_mode = EQ_EXP_MODE_ROCK;
        break;
    default:
        break;
    }
    return ref_eq_mode;
}

#endif

/*****************************************************************************/
//  Description : Set play rate.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAUDIO_SetPlayRate(
                MMIAUDIO_PLAY_RATE_E rate
                )
{
#ifdef AUDIO_RATE_SUPPORT
    AUDIO_RESULT_E result = AUDIO_NO_ERROR;
    RATECONTROL_EXP_PARA_T rate_para = {0}; /*lint !e64*/
    uint32  param_count = 0;

    if(rate >= MMIAUDIO_PLAY_RATE_MAX)
    {
        //SCI_TRACE_LOW:"[MMIAUDIO] MMIAUDIO_SetPlayRate:rate = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5500_112_2_18_2_3_37_89,(uint8*)"d",rate);
        rate = MMIAUDIO_PLAY_RATE_NORMAL;
    }

    if(rate != s_audio_cur_play_rate)
    {
        extern HAUDIOEXP hRateControlExp;
        rate_para.eRateControl = GetAudioPlayRate(rate);
        result = AUDIO_ExeExpSetParaOpe(
                    hRateControlExp,
                    ANSI2UINT16("RATECONTROL_EXP"),
                    param_count, 
                    &rate_para
                    );
    }
    //SCI_TRACE_LOW:"[MMIAUDIO]: MMIAUDIO_SetPlayRate(), orig_RATE = %d, new_RATE = %d, aud_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5515_112_2_18_2_3_37_90,(uint8*)"ddd",s_audio_cur_play_rate, rate, result);

    if(AUDIO_NO_ERROR == result)
    {
        s_audio_cur_play_rate = rate;
    }

#endif
}

#ifdef AUDIO_RATE_SUPPORT
/*****************************************************************************/
//  Description : Get play rate
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL RATECONTROL_EXP_RATE_E GetAudioPlayRate(
                                    MMIAUDIO_PLAY_RATE_E rate
                                    )
{
    RATECONTROL_EXP_RATE_E audio_rate = RATECONTROL_EXP_RATE_MAX;
    switch(rate)
    {
    case MMIAUDIO_PLAY_RATE_SLOW_2:
        audio_rate = RATECONTROL_EXP_RATE_SLOWEST;
        break;
    case MMIAUDIO_PLAY_RATE_SLOW_1:
        audio_rate = RATECONTROL_EXP_RATE_SLOWER;
        break;
    case MMIAUDIO_PLAY_RATE_NORMAL:
        audio_rate = RATECONTROL_EXP_RATE_NORMAL;
        break;
    case MMIAUDIO_PLAY_RATE_FAST_1:
        audio_rate = RATECONTROL_EXP_RATE_FASTER;
        break;
    case MMIAUDIO_PLAY_RATE_FAST_2:
        audio_rate = RATECONTROL_EXP_RATE_FASTEST;
        break;
    default:
        break;
    }
    return audio_rate;
}
#endif
/*****************************************************************************/
//  Description : set volume 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_SetVolume(AUDIO_DEVICE_MODE_TYPE_E  mode, uint32 speaker_vol)
{
    AUDIO_RESULT_E dev_result = AUDIO_NO_ERROR;
    AUDIO_RESULT_E vol_result = AUDIO_NO_ERROR;
    
    if(AUDIO_DEVICE_MODE_MAX <= mode)
    {
        mode = AUDIO_DEVICE_MODE_HANDFREE;
    }
    if(MMI_AUDIO_VOLUME_MAX < speaker_vol)
    {
        speaker_vol = MMI_AUDIO_VOLUME_MAX;
    }

    AUDIO_GetDevMode(&s_audio_cur_dev_mode);        //179354,依ref记录
    AUDIO_GetVolume(&s_audio_cur_speaker_vol);      //179354,依ref记录

    if (mode != s_audio_cur_dev_mode)
    {
        dev_result = AUDIO_SetDevMode(mode);
        SCI_Sleep(25);
        vol_result = AUDIO_SetVolume(speaker_vol);
    }
    else if (speaker_vol != s_audio_cur_speaker_vol)
    {
        vol_result = AUDIO_SetVolume(speaker_vol);
    }
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_SetVolume(), orig_mod = %d,orig_vol = %d,new_mod = %d,new_vol = %d, aud_result_dev = %d,aud_result_vol = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5590_112_2_18_2_3_37_91,(uint8*)"dddddd",s_audio_cur_dev_mode,s_audio_cur_speaker_vol,mode,speaker_vol,dev_result,vol_result);
    
    //check value
    if(AUDIO_NO_ERROR == dev_result)
    {
        s_audio_cur_dev_mode = mode;
    }
    if(AUDIO_NO_ERROR == vol_result)
    {
        s_audio_cur_speaker_vol = speaker_vol;
    }
    
    #ifdef PLATFORM_SC6600L
    if(MMIAPIENVSET_GetEnhanceRingState())
    {
        MMIAUDIO_StartAGC();
    }
    #endif
}

/*****************************************************************************/
//  Description : set current device mode 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_SetCurDevMode(AUDIO_DEVICE_MODE_TYPE_E mode)
{
    AUDIO_RESULT_E result = AUDIO_NO_ERROR;
    
    AUDIO_GetDevMode(&s_audio_cur_dev_mode);    //179354,依ref记录
    
    if(s_audio_cur_dev_mode != mode)
    {
        result = AUDIO_SetDevMode(mode);
    }
    //SCI_TRACE_LOW:"[MMIAUDIO]: MMIAUDIO_SetCurDevMode: orig_dev=%d, new_dev = %d, aud_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5624_112_2_18_2_3_37_92,(uint8*)"ddd",s_audio_cur_dev_mode,mode,result);
    
    if(AUDIO_NO_ERROR == result)
    {
        s_audio_cur_dev_mode = mode;
    }
}

/*****************************************************************************/
//  Description : get current volume 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAUDIO_GetCurVolume(void)
{
    AUDIO_GetVolume(&s_audio_cur_speaker_vol);  //179354,依ref记录
    
    return MIN(MMI_AUDIO_VOLUME_MAX,s_audio_cur_speaker_vol);
}

/*****************************************************************************/
//  Description : get current device mode 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC AUDIO_DEVICE_MODE_TYPE_E MMIAUDIO_GetCurDevMode(void)
{
    AUDIO_GetDevMode(&s_audio_cur_dev_mode);    //179354,依ref记录
    
    return s_audio_cur_dev_mode;
}

/*****************************************************************************/
//  Description : set mute 
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_SetMute(BOOLEAN is_mute)
{
    AUDIO_RESULT_E result = AUDIO_NO_ERROR;
    
    AUDIO_GetDownLinkMute(&s_audio_cur_mute_mode);  //179354,依ref记录
    
    if (is_mute != s_audio_cur_mute_mode)
    {
        result = AUDIO_SetDownLinkMute(is_mute);
    }
    //SCI_TRACE_LOW:"[MMIAUDIO]: MMIAUDIO_SetMute: orig_is_mute=%d, new_is_mute = %d, aud_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5668_112_2_18_2_3_37_93,(uint8*)"ddd",s_audio_cur_mute_mode,is_mute,result);

    if(AUDIO_NO_ERROR == result)
    {
        s_audio_cur_mute_mode = is_mute;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : Get mute 
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_GetMute(void)
{
    AUDIO_GetDownLinkMute(&s_audio_cur_mute_mode);  //179354,依ref记录
    
    return s_audio_cur_mute_mode;
}
//  Description : set up link mute 
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_SetUpLinkMute(BOOLEAN is_mute)
{
    AUDIO_RESULT_E result = AUDIO_NO_ERROR;

    result = AUDIO_SetUpLinkMute(is_mute);

    //SCI_TRACE_LOW:"[MMIAUDIO]: MMIAUDIO_SetUpLinkMute: is_mute=%d,aud_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5701_112_2_18_2_3_38_94,(uint8*)"dd",is_mute,result);
    
    if(AUDIO_NO_ERROR == result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : MMIAUDIO SwitchDevice
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_SwitchDeviceExt(
                    MMIAUDIO_HTYPE          audio_handle_type,
                    MMIAUDIO_DEVICE_TYPE_E  device
                    )
{
    AUDIO_RESULT_E  result = AUDIO_ERROR;
    MMIAUDIO_TYPE_E format_type = MMIAUDIO_TYPE_MAX;
    HAUDIO          hAudioHandle = INVALID_HANDLE;
    HAUDIODEV       hDevice = 0;

    hAudioHandle = MMIAUDIO_GetAudioHandle(audio_handle_type);
    if(PNULL != hAudioHandle)
    {
        format_type = GetAudioType(AUDIO_GetCodecHandle(hAudioHandle));
        switch(device)
        {
            case MMIAUDIO_DEVICE_BT:
                hDevice = GetAudioDevice(TRUE,format_type);
                result = AUDIO_SwitchDevice(hAudioHandle, hDevice, PNULL);
                break;

            case MMIAUDIO_DEVICE_OTHER:
                hDevice = GetAudioDevice(FALSE,format_type);
                result = AUDIO_SwitchDevice(hAudioHandle, hDevice, PNULL);
                break;

            default:
                break;
        }
    }

    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_SwitchDeviceExt,  audio_handle_type = 0x%x, hAudioHandle = 0x%x, format_type = %d, device = %d, aud_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5749_112_2_18_2_3_38_95,(uint8*)"ddddd",audio_handle_type,hAudioHandle,format_type,device,result);

    if(AUDIO_NO_ERROR == result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : API of start IQ Data Record
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_StartIQData(
                        const wchar *name_ptr, 
                        RECORD_PARAM_T *record_param
                        )
{
    HAUDIOCODEC hAudioCodec = 0;
    const uint16* pusCodecName = PNULL;
    HAUDIODEV hDevice = 0;
    const uint16* pusDevName = PNULL;
    uint32 result = 0;

    if(PNULL == record_param || PNULL == name_ptr)
    {
        return FALSE;
    }

    hAudioCodec = hRecordCodec;
    hDevice = hDSPDATADEVICE;
    
    s_cur_audio_handle_type = MMIAUDIO_APP;

    s_IQ_data_buf = SCI_ALLOC_APP(MMIAUDIO_IQ_DATA_BUF_SIZE << 2);  //300K
    if(PNULL == s_IQ_data_buf)
    {
        //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_StartIQData BUFFER ALLOC FAILED!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5789_112_2_18_2_3_38_96,(uint8*)"");
        return FALSE;
    }

    s_record_handle = AUDIO_CreateRecordFileHandle(
                                hAudioCodec,
                                pusCodecName,
                                hDevice,
                                pusDevName,
                                (const int16 *)name_ptr,
                                s_IQ_data_buf,
                                MMIAUDIO_IQ_DATA_BUF_SIZE,
                                AudioNotifyCallback
                                );

    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_StartIQData:s_record_handle=%lx"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5804_112_2_18_2_3_38_97,(uint8*)"d", s_record_handle);

    if (PNULL == s_record_handle)
    {
        if(PNULL != s_IQ_data_buf)
        {
            SCI_FREE(s_IQ_data_buf);
        }
        return FALSE;
    }

    AUDIO_ExeCodecExtOpe(
            s_record_handle,
            ANSI2UINT16("SET_RECORDPARAM"),//扩展操作的标志 
            RECORD_PARAM_SET_COUNT,
            record_param,
            &result
            );

    //AUDIO_StartRecord(s_record_handle);
    AUDIO_Play(s_record_handle, 0);
    return TRUE;
}

/*****************************************************************************/
//  Description : Get the record eclipse time (count in ms).
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAUDIO_GetRecordEclipseTime(
                void
                )
{
    RECORD_TIME_IN_MS_T record_param = {0};
    uint32 result = 0;

    if (PNULL != s_record_handle)
    {
        AUDIO_ExeCodecExtOpe(
            s_record_handle,
            ANSI2UINT16("GET_RECORDTIME"),
            RECORD_GET_RECORD_TIME_COUNT,
            &record_param,
            &result
            );
    }
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_GetRecordEclipseTime, time = %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5849_112_2_18_2_3_38_98,(uint8*)"d",record_param.ui_record_time_in_ms);
    return record_param.ui_record_time_in_ms;
}

/*****************************************************************************/
//  Description : Get the record voidce DB.
//  Global resource dependence : none
//  Author: chunyou.zhu
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAUDIO_GetRecordVoiceDB(
                void
                )
{
    RECORD_VOICEDB_T record_param = {0};
    uint32 result = 0;

    if (PNULL != s_record_handle)
    {
        AUDIO_ExeCodecExtOpe(
            s_record_handle,
            ANSI2UINT16("GET_VOICEDB"),
            RECORD_GET_RECORD_VOICEDB_COUNT,
            &record_param,
            &result
            );
    }
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_GetRecordVoiceDB, db = %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5874_112_2_18_2_3_38_99,(uint8*)"d",record_param.db);
    return record_param.db;
}

/*****************************************************************************/
//  Description : Set half samplerate.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_SetHalfSamplerate(
                BOOLEAN is_set_half
                )
{
#ifndef _WIN32
    #ifndef NANDBOOT_SUPPORT
        AUDIO_RESULT_E result                = AUDIO_NO_ERROR;
        AUDIO_RESULT_E cut_samplerate_result = AUDIO_NO_ERROR;
        BOOLEAN        isEnable = SCI_TRUE;
        
        result = AUDIO_ExeCodecExtOpe(
                    s_player_audio_handle, 
                    ANSI2UINT16("CUT_SAMPLERATE"), 
                    1, 
                    (void*)&isEnable,
                    (void*)&cut_samplerate_result
                    );
    #endif
#endif
}

/*****************************************************************************/
//  Description : Get whether eq mode is normal.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsNormalEQMode(void)
{
#ifdef MP3_EQ_SUPPORT
    return (MMIAUDIO_EQ_REGULAR == s_audio_cur_eq_mode);
#else
    return TRUE;
#endif
}

/*****************************************************************************/
//  Description : enable/disable voice codec.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_EnableVoiceCodec(
                    BOOLEAN is_enable
                    )
{
    AUDIO_RESULT_E result = AUDIO_NO_ERROR;
    
    if(is_enable)
    {        
    	result = AUDIO_Play(hVoiceRing, 0 );
    }
    else
    {
    	result = AUDIO_Stop(hVoiceRing);
    }

    //SCI_TRACE_LOW:"[MMIAUDIO]: MMIAUDIO_EnableVoiceCodec: is_enable=%d, aud_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5935_112_2_18_2_3_38_100,(uint8*)"dd",is_enable,result);
    if (AUDIO_NO_ERROR == result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : set device mode for call
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_SerDeviceModeInCall(
                    AUDIO_DEVICE_MODE_TYPE_E mode
                    )
{
    AUDIO_RESULT_E result = AUDIO_NO_ERROR;
    
    //AUDIO_GetDevMode(&s_audio_cur_dev_mode);

    result = AUDIO_SetDevModeEx(hVoiceRing, mode);

    //SCI_TRACE_LOW:"[MMIAUDIO]: MMIAUDIO_SerDeviceModeInCall: orig_dev=%d, new_dev = %d, aud_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5960_112_2_18_2_3_38_101,(uint8*)"ddd",s_audio_cur_dev_mode,mode,result);
    
    if(AUDIO_NO_ERROR == result)
    {
        s_audio_cur_dev_mode = mode;
    }

    if (AUDIO_NO_ERROR == result)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : enable/disable voice loop.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_EnableVoiceLoop(
                    BOOLEAN is_loop_enable,
                    BOOLEAN is_force_dev_mode,
                    AUDIO_DEVICE_MODE_TYPE_E force_dev_mode,
                    uint8   volume
                    )
{
    AUDIO_DEVICE_MODE_TYPE_E dev_mode = AUDIO_DEVICE_MODE_MAX;
    
    //SCI_TRACE_LOW:"[MMIAUDIO]: MMIAUDIO_EnableVoiceCodec: is_loop_enable=%d, is_force_dev_mode = %d,force_dev_mode = %d,,volume =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_5990_112_2_18_2_3_38_102,(uint8*)"dddd",is_loop_enable,is_force_dev_mode,force_dev_mode,force_dev_mode,volume);

    s_is_voice_loop_enable = is_loop_enable;

    if(is_loop_enable)
    {
        s_is_voice_loop_force_mode = is_force_dev_mode;
        
        if(is_force_dev_mode &&
           (AUDIO_DEVICE_MODE_EARPHONE == force_dev_mode || AUDIO_DEVICE_MODE_HANDHOLD == force_dev_mode))
        {
            dev_mode = force_dev_mode;
        }
        else
        {
            if (GPIO_CheckHeadsetStatus()) 
            {
                dev_mode = AUDIO_DEVICE_MODE_EARPHONE;
            }
            else
            {
                dev_mode = AUDIO_DEVICE_MODE_HANDHOLD;
            }
        }
        MMIAUDIO_SetVolumeDeviceModeExt(dev_mode, volume);
        AUD_EnableVoiceLoopback(hVoiceLoopbackRing, TRUE, 100);
        MMIAUDIO_SetVolumeDeviceModeExt(dev_mode, volume);
    }
    else
    {
        AUD_EnableVoiceLoopback(hVoiceLoopbackRing, FALSE, 100);
        s_is_voice_loop_force_mode = FALSE;
    }
}

/*****************************************************************************/
//  Description : is voice loop on.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsInVoiceLoop(
                    void
                    )
{
    //SCI_TRACE_LOW:"[MMIAUDIO]: MMIAUDIO_IsInVoiceLoop: s_is_voice_loop_enable = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_6033_112_2_18_2_3_38_103,(uint8*)"d",s_is_voice_loop_enable);
    return s_is_voice_loop_enable;
}

/*****************************************************************************/
//  Description : Is Voice Loop Force Mode
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsVoiceLoopForceMode(
                    void
                    )
{
    //SCI_TRACE_LOW:"[MMIAUDIO]: MMIAUDIO_IsVoiceLoopForceMode: s_is_voice_loop_force_mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_6045_112_2_18_2_3_38_104,(uint8*)"d",s_is_voice_loop_force_mode);
    return s_is_voice_loop_force_mode;
}

/*****************************************************************************/
//  Description : audio handle is valid
//  Global resource dependence : none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_SM_GetAudioObject(
    HAUDIO hAudioHandle
    )
{
    if (PNULL != AUDIO_SM_GetAudioObject(hAudioHandle))
    {
        //SCI_TRACE_LOW:"MMIAUDIO_SM_GetAudioObject true"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_6059_112_2_18_2_3_38_105,(uint8*)"");
        return TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAUDIO_SM_GetAudioObject False"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_6064_112_2_18_2_3_38_106,(uint8*)"");
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : Reset the device mode and volume, regardless of the original device mode and volume.
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_SetVolumeDeviceModeExt(AUDIO_DEVICE_MODE_TYPE_E  mode, uint32 speaker_vol)
{
    /*
    AUDIO_RESULT_E dev_result = AUDIO_NO_ERROR;
    AUDIO_RESULT_E vol_result = AUDIO_NO_ERROR;
    
    if(AUDIO_DEVICE_MODE_MAX <= mode)
    {
        mode = AUDIO_DEVICE_MODE_HANDFREE;
    }
    if(MMI_AUDIO_VOLUME_MAX < speaker_vol)
    {
        speaker_vol = MMI_AUDIO_VOLUME_MAX;
    }

    //#ifdef PLATFORM_SC6600L
    #ifndef WIN32
    if(MMIAPIENVSET_GetEnhanceRingState())
    {
        if(MMIAUDIO_EQ_REGULAR == s_audio_cur_eq_mode)
        {
            AUD_AGC_Switch(SCI_TRUE,AUDIO_AGC_MP3);
        }
        else
        {
            AUD_AGC_Switch(SCI_FALSE,NULL);
        }
    }
    #endif

    dev_result = AUDIO_SetDevMode(mode);
    vol_result = AUDIO_SetVolume(speaker_vol);

    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_SetVolumeDeviceModeExt, orig_mod = %d,orig_vol = %d,new_mod = %d,new_vol = %d, aud_result_dev = %d,aud_result_vol = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_6107_112_2_18_2_3_38_107,(uint8*)"dddddd",s_audio_cur_dev_mode,s_audio_cur_speaker_vol,mode,speaker_vol,dev_result,vol_result);
    
    //check value
    if(AUDIO_NO_ERROR == dev_result)
    {
        s_audio_cur_dev_mode = mode;
    }
    if(AUDIO_NO_ERROR == vol_result)
    {
        s_audio_cur_speaker_vol = speaker_vol;
    }
    */
    MMIAUDIO_SetVolume(mode, speaker_vol);
}

/*****************************************************************************/
//  Description : Set midi param to low chord.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_SetMidiLowPolyNum(
                    MMIAUDIO_HTYPE audio_htype,
                    BOOLEAN is_low_poly_num
                    )
{
    AUDIO_RESULT_E aud_result = AUDIO_NO_ERROR;
    MIDI_CODEC_EXT_OPE_PARA_T midi_param = {0};
    HAUDIO          midi_handle = INVALID_HANDLE;
    HAUDIOCODEC     audio_codec = NULL;
    MMIAUDIO_TYPE_E audio_type  = MMIAUDIO_MIDI;
    uint32          result = 0;
    uint16          poly_num = 0;

    midi_handle = MMIAUDIO_GetAudioHandle(audio_htype);
    audio_codec = AUDIO_GetCodecHandle(midi_handle);
    audio_type = GetAudioType(audio_codec);
    poly_num = (is_low_poly_num)?(MMI_MIDI_LOW_POLY_NUM):(MMI_MIDI_NORMAL_POLY_NUM);

    if(MMIAUDIO_MIDI != audio_type)
    {
        return;
    }

    midi_param.hAudioHandle = midi_handle;
    midi_param.uPara.usPolyNum = poly_num;
    aud_result = AUDIO_ExeCodecExtOpe(
                    midi_handle,
                    ANSI2UINT16("SET_POLYNUM"),
                    MIDI_CODEC_EXT_OPE_PARA_COUNT,
                    &midi_param,
                    &result
                    );
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_SetMidiLowPolyNum, audio_htype = %d, midi_handle = 0x%x, poly_num = %d,aud_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_6159_112_2_18_2_3_38_108,(uint8*)"dddd",audio_htype,midi_handle,poly_num,aud_result);
}

/*****************************************************************************/
//  Description : MMI AUDIO Is AGC Support
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_IsAGCSupport(
                        void
                        )
{
#ifdef PLATFORM_SC6600L
    return AUD_GetAGC_SupportFlag(AUDIO_AGC_MP3);
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description : MMIAUDIO Start AGC
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_StartAGC(
                        void
                        )
{
#ifdef PLATFORM_SC6600L
    if(!s_audio_is_agc_enable)
    {
        return;
    }
    //SCI_TRACE_LOW:"[mmiaudio] MMIAUDIO_StartAGC dev_mode = %d, eq_mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_6190_112_2_18_2_3_39_109,(uint8*)"dd",MMIAUDIO_GetCurDevMode(),s_audio_cur_eq_mode);
    if(AUD_GetAGC_SupportFlag(AUDIO_AGC_MP3))
    {
        if(MMIAUDIO_EQ_REGULAR == s_audio_cur_eq_mode)
        {
            AUD_AGC_Switch(SCI_TRUE,AUDIO_AGC_MP3);
        }
        else
        {
            AUD_AGC_Switch(SCI_FALSE,NULL);
        }
    }
#endif
}

/*****************************************************************************/
//  Description : MMIAUDIO Stop AGC
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_StopAGC(
                        void
                        )
{
#ifdef PLATFORM_SC6600L
    //SCI_TRACE_LOW:"[mmiaudio] MMIAUDIO_StopAGC"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_6214_112_2_18_2_3_39_110,(uint8*)"");
    if(AUD_GetAGC_SupportFlag(AUDIO_AGC_MP3))
    {
        AUD_AGC_Switch(SCI_FALSE,NULL);
    }
#endif
}

/*****************************************************************************/
//  Description : MMIAUDIO Enable EQ
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_EnableEQ(
                        BOOLEAN is_enable
                        )
{
#ifdef PLATFORM_SC6600L
    #ifdef MP3_EQ_SUPPORT 
    static MMIAUDIO_EQ_MODE_E s_eq_mode = MMIAUDIO_EQ_MAX_MODE;   //初始化需要常量，static EQ_EXP_MODE_E s_eq_mode = s_audio_cur_eq_mode

    //SCI_TRACE_LOW:"[mmiaudio] MMIAUDIO_EnableEQ is_enable = %d, s_audio_cur_eq_mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_6234_112_2_18_2_3_39_111,(uint8*)"dd",is_enable,s_audio_cur_eq_mode);
    
    if(MMIAUDIO_EQ_MAX_MODE == s_eq_mode)
    {
       s_eq_mode = s_audio_cur_eq_mode;
    }
    
    if(is_enable)
    {
        s_audio_is_eq_enable = TRUE;
        MMIAUDIO_SetEQMode(s_eq_mode);
    }
    else
    {
        s_eq_mode = s_audio_cur_eq_mode;
        MMIAUDIO_SetEQMode(MMIAUDIO_EQ_REGULAR);
        s_audio_is_eq_enable = FALSE;
    }
    #endif
#endif
}

/*****************************************************************************/
//  Description : MMIAUDIO Enable AGC
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_EnableAGC(
                        BOOLEAN is_enable
                        )
{
#ifdef PLATFORM_SC6600L

    //SCI_TRACE_LOW:"[mmiaudio] MMIAUDIO_EnableEQ is_enable = %d, s_audio_cur_eq_mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_6266_112_2_18_2_3_39_112,(uint8*)"dd",is_enable,s_audio_cur_eq_mode);
    s_audio_is_agc_enable = is_enable;
    
    if(is_enable)
    {
        if(MMIAPIENVSET_GetEnhanceRingState())
        {
            MMIAUDIO_StartAGC();
        }
    }
    else
    {
        MMIAUDIO_StopAGC();
    }
#endif
}

/*****************************************************************************/
//  Description : MMIAUDIO Enable EQ & AGC
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_EnableEQAndAGC(
                        BOOLEAN is_enable
                        )
{
#ifdef PLATFORM_SC6600L
    //SCI_TRACE_LOW:"[mmiaudio] MMIAUDIO_EnableEQAndAGC is_enable = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_6292_112_2_18_2_3_39_113,(uint8*)"d",is_enable);

    s_audio_is_agc_enable = is_enable;  //需要先设置状态

    MMIAUDIO_EnableEQ(is_enable);       //PM规则:EQ 优先于 AGC，避免 AGC 开启后又立即被 EQ 关闭
    MMIAUDIO_EnableAGC(is_enable);

#endif
}

/*****************************************************************************/
//  Description : MMIAUDIO Open FM
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_OpenFM(
                    void
                    )
{

}

/*****************************************************************************/
//  Description : MMIAUDIO Play FM
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUDIO_PlayFM(
                    void
                    )
{
    HAUDIOCODEC hAudioCodec = hNullCodec;
    const uint16* pusCodecName = PNULL;
    HAUDIODEV hDevice = hLINEINDev;
    const uint16* pusDevName = PNULL;
    BOOLEAN         result      = FALSE;
    AUDIO_RESULT_E  aud_result  = AUDIO_ERROR;
    
    if(PNULL == s_fm_handle)
    {
        s_fm_handle = AUDIO_CreateNormalHandle(
                            hAudioCodec,
                            pusCodecName,
                            hDevice,
                            pusDevName,
                            AudioNotifyCallback
                            );

        if (INVALID_HANDLE != s_fm_handle)
        {
            aud_result = AUDIO_Play(s_fm_handle, 0);

            if(AUDIO_NO_ERROR != aud_result)
            {
                AUDIO_CloseHandle(s_fm_handle);
                s_fm_handle = PNULL;
            }
            else
            {
                result = TRUE;
            }
        }
    }
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_PlayFM:s_fm_handle=0x%lx, aud_result = %d, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_6353_112_2_18_2_3_39_114,(uint8*)"ddd", s_fm_handle,aud_result,result);
    
    return result;
}

/*****************************************************************************/
//  Description : MMIAUDIO Stop FM
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_StopFM(
                    void
                    )
{
    //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_StopFM:s_fm_handle=0x%lx"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_6366_112_2_18_2_3_39_115,(uint8*)"d", s_fm_handle);
    
    if(PNULL != s_fm_handle)
    {
        //SCI_TRACE_LOW:"[MMIAUDIO]:MMIAUDIO_StopFM:s_fm_handle=0x%lx"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUDIO_6370_112_2_18_2_3_39_116,(uint8*)"d", s_fm_handle);
        AUDIO_Stop(s_fm_handle);
        AUDIO_CloseHandle(s_fm_handle);
        s_fm_handle = PNULL;
    }
}

/*****************************************************************************/
//  Description : MMIAUDIO Close FM
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUDIO_CloseFM(
                    void
                    )
{

}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint32 *MMIAUDIO_GetApplicationTrackBuf(void)
{
    return s_application_track_buf;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAUDIO_GetApplicationTrackBufSize(void)
{
    return MMIAUDIO_TRACKBUFSIZE_APP;
}
#endif
#endif

