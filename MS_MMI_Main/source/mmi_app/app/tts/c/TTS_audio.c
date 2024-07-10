
#include "mmi_app_tts_trc.h"
#if defined(TTS_SUPPORT) || defined(HERO_ENGINE_TTS_SUPPORT )
#ifndef _TTS_AUDIO_C_
#define _TTS_AUDIO_C_ 

#include "sci_types.h"


#include "mmiaudio.h"
#include "audio_config.h"
#ifndef _WIN32
#include "audio_api.h"
#else
//#include "mmiaudio_simu.h"
#endif
#include "generic_tone_adp.h"
#include "custom_tone_adp.h"
#include "dtmf_tone_adp.h"
#include "pcm_adp.h"

#include "mmi_appmsg.h"
#include "mmi_common.h"
#include "mmi_nv.h"

//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiaudio_ctrl.h"
#include "wav_adp.h"

#include "mmitts_export.h"
#include "TTS_audio.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

extern MMITTS_STRING_T              g_tts_text;

const WAV_STREAM_BUFFER_T head_info = {SCI_TRUE, 0, 0, 0, WAV_PCM_BIGENDIAN, 8000, 0x10, 1, sizeof(WAV_STREAM_BUFFER_T), 84, NULL};//IMPORTANT!!!

static BOOLEAN TTS_handleIsOpen=FALSE;

extern uint32  g_tts_pcm_stream_id;

/*****************************************************************************/
// 	Description : 播放录音（音量参数）
//	Global resource dependence :                                
//  Author: 
//	Note:
/*****************************************************************************/
BOOLEAN MMITTS_CreatePcmStreamHandle(MMISET_ALL_RING_TYPE_E     ring_type,AUDIO_GETSRCDATA_CALLBACK_PFUNC pCallbackfunc)
{
    BOOLEAN error_code = TRUE;

    BOOLEAN is_creat_handle_success = FALSE;

    //SCI_TRACE_LOW:"enter funciton MMITTS_CreatePCMStreamHandle()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_AUDIO_57_112_2_18_3_3_37_0,(uint8*)"");
    
	if (!MMIAPISET_IsPermitPlayRing/*MMISET_IsPermitPlayRing*/(ring_type))
		return FALSE; 

    // 先停止所有铃声，这样才能确保创建handle成功
    //MMIAPISET_StopAllVibrator();
    MMIAPISET_StopAllRing(FALSE);

    g_tts_pcm_stream_id ++; 

    is_creat_handle_success = MMIAUDIO_CreateAudioStreamingHandle(		FALSE,                     
																MMIAUDIO_APP,
																MMIAUDIO_WAVE,	//important!!!
																(const uint8*)&head_info,		// the music data pointer
																sizeof(head_info),    // the music data length
																pCallbackfunc
                                                            );

    if (!is_creat_handle_success)
    {
        return FALSE;
    }
#if 0
	//V2上没有此接口，在create handle时直接把callback填进去
	MMIAUDIO_SetPcmPlayParam( 
								MMIAUDIO_APP,
								pCallbackfunc);	
#endif

	//其他需要恢复的工作在create handle之后做，确保close handle时配对
//	MMIDEFAULT_EnableKeyRing(FALSE);
// 	MMIDEFAULT_EnableTpRing(FALSE);
    MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_TTS, FALSE);
	MMIAUDIO_PauseBgPlay(MMIBGPLAY_MODULE_TTS);

	MMIAPISET_SetCurRingType(ring_type);                       

	TTS_handleIsOpen=TRUE;

    return error_code;
}

/*****************************************************************************/
// 	Description : stop callback
//	Global resource dependence :                                

//	Note:for_tts
/*****************************************************************************/

BOOLEAN MMITTS_IsCreatePcmStreamHandle(void)
{
	if( MMIAUDIO_GetAudioHandle(MMIAUDIO_APP))
		return TRUE;
	else
		return FALSE;
}

/*****************************************************************************/
// 	Description : stop callback
//	Global resource dependence :                                

//	Note:for_tts
/*****************************************************************************/
BOOLEAN MMIAPITTS_GetAudioIsOpen(void)
{
	return TTS_handleIsOpen;
}

/*****************************************************************************/
// 	Description : stop callback

//	Note:for_tts
/*****************************************************************************/
extern BOOLEAN			 g_tts_is_running;
BOOLEAN MMITTS_IsRepeatePlayText(void);
void MMITTS_STOP_CALLBACK(MMIAUDIO_RESULT result, DPARAM param)
{	
    	//SCI_TRACE_LOW:"enter funciton MMITTS_STOP_CALLBACK() g_tts_is_running=%d"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_AUDIO_129_112_2_18_3_3_37_1,(uint8*)"d",g_tts_is_running);

      if(result != MMIAUDIO_RLT_STOP)
      {
        	/*end audio*/
        	if (g_tts_is_running)
        	{
        		g_tts_is_running = FALSE;
        		MMITTS_MemoryRelease();
        		MMI_RestoreFreq(); 
        	}
        	if (MMIAPITTS_GetAudioIsOpen())
        		MMITTS_StopAndClosePcmStreamHandle(g_tts_text.ring_type);
        	
        	/*next play*/
        	if (MMITTS_IsRepeatePlayText())
        	{
        		MMIAPITTS_SynthTextByTask();
        	}
        	else
        		MMK_PostMsg( /*VIRTUAL_WIN_ID*/MMK_GetFocusWinId(), MSG_TTS_OVER, PNULL,0 );
      }
}


/*****************************************************************************/
// 	Description : 播放录音（音量参数）
//	Global resource dependence :                                
//  Author: 
//	Note:
/*****************************************************************************/
void MMIAPITTS_PlayPcmStream(uint32 pcm_stream_id)
{
    AUDIO_DEVICE_MODE_TYPE_E mode = AUDIO_DEVICE_MODE_HANDHOLD;

    if (pcm_stream_id != g_tts_pcm_stream_id)	
    {
        //SCI_TRACE_LOW:"play_stream_id = %d, cur_stream_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_AUDIO_164_112_2_18_3_3_37_2,(uint8*)"dd", pcm_stream_id, g_tts_pcm_stream_id);
        return;
    }
		 
    //SCI_TRACE_LOW:"enter funciton MMIAPITTS_PlayPcmStream()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_AUDIO_168_112_2_18_3_3_37_3,(uint8*)"");

    if (!MMIAUDIO_GetAudioHandle(MMIAUDIO_APP))  
    {
    	//SCI_TRACE_LOW:"MMIAPITTS_PlayPcmStream() Handle error !!!!"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_AUDIO_172_112_2_18_3_3_37_4,(uint8*)"");
		return;
    }

    if (g_tts_is_running==FALSE)
		return;
    

    if(MMISET_RING_TYPE_CALL == g_tts_text.ring_type)
    {
        //设置声音设备模式
        MMIAPISET_SetAudioMode(TRUE);
        MMIAPISET_GetDeviceMode(&mode);
        MMIAPISET_SetAudioMode(FALSE);
    }
    else
    {
        MMIAPISET_GetDeviceMode(&mode);
    }

    
    if (0 == g_tts_text.tts_vol)
    {
        MMIAUDIO_SetCurDevMode(mode);
        MMIAUDIO_SetMute(TRUE);
    }
    else
    {
        MMIAUDIO_SetMute(FALSE);
        //MMIAPISET_GetDeviceMode(&mode);//@fen.xie MS00179222:前面已根据g_tts_text.ring_type设置,此处不可再get mode!
        MMIAUDIO_SetVolumeDeviceModeExt(mode, (uint32)g_tts_text.tts_vol);
    }
	//问题出现在这里===================================================
	//注意HandleRingMsg--AUDIO_PLAYEND_IND--AudioEndCallback的执行
	//根据audio类型pcm的情况进行处理了吗?
    MMIAUDIO_AudioPlay(
                                        MMIAUDIO_APP,
                                        0,
                                        1,//play_times,
                                        MMITTS_STOP_CALLBACK  
                                    );

 
}

/*****************************************************************************/
// 	Description : 通过调用该函数来<停止>并<关闭>
//	Global resource dependence :                                
//  Author: 
//	Note:
/*****************************************************************************/
BOOLEAN MMITTS_StopAndClosePcmStreamHandle(MMISET_ALL_RING_TYPE_E     ring_type)
{
	//SCI_TRACE_LOW:"enter funciton MMITTS_StopAndClosePcmStreamHandle()"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_AUDIO_223_112_2_18_3_3_37_5,(uint8*)"");
#if 0
	//V2上没有此接口，不需要重置callback
	MMIAUDIO_SetPcmPlayParam( 
								MMIAUDIO_APP,
								PNULL );	
#endif
	MMIAPISET_StopRing(ring_type);
	MMIAUDIO_CloseAudioHandle(MMIAUDIO_APP);
	if (TTS_handleIsOpen)
	{
	    TTS_handleIsOpen=FALSE;
//	    MMIDEFAULT_EnableKeyRing(TRUE);
// 	    MMIDEFAULT_EnableTpRing(TRUE);
        MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_TTS, TRUE);
	    MMIAUDIO_ResumeBgPlay(MMIBGPLAY_MODULE_TTS);
	}

    return TRUE;
}
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif
