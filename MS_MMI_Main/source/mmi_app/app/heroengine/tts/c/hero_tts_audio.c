/*****************************************************************************
** File Name:      hero_tts_audio.c                                       *
** Author:                                                                   *
** Date:                                                                     *
** Copyright:      All Rights Reserved.                                      *
** Description:    tts                                               *
*****************************************************************************/

#include "mmi_app_tts_trc.h"
#ifdef HERO_ENGINE_TTS_SUPPORT

#include "sci_types.h"


//#include "mmiaudio.h"
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
//#include "mmirecord_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
//#include "mmiaudio_ctrl.h"
#include "wav_adp.h"

#include "mmitts_export.h"
#include "hero_tts_api.h"
#include "hero_tts_audio.h"

#include "mmisrvaud_api.h"
#include "mmisrvmgr.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

LOCAL uint32  s_tts_pcm_stream_id = 0;

LOCAL const WAV_STREAM_BUFFER_T head_info = {SCI_TRUE, 0, 0, 0, WAV_PCM_BIGENDIAN, 8000, 0x10, 1, sizeof(WAV_STREAM_BUFFER_T), 84, NULL};//IMPORTANT!!!

//LOCAL BOOLEAN TTS_handleIsOpen=FALSE;

LOCAL MMISRV_HANDLE_T s_tts_audio_handle = 0;

/*****************************************************************************/
// 	Description : 播放录音（音量参数）
//	Global resource dependence :                                
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN TTS_CreatePcmStreamHandle(TTS_ALL_RING_TYPE_E ring_type, AUDIO_GETSRCDATA_CALLBACK_PFUNC pCallbackfunc,MMISRVAUD_ROUTE_T all_support_route)
{
    BOOLEAN                 error_code = TRUE;
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T        audio_srv = {0};
    MMISRV_HANDLE_T         audio_handle = TTS_GetAudiohandle();
    
    //SCI_TRACE_LOW:"enter funciton TTS_CreatePcmStreamHandle()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_AUDIO_66_112_2_18_3_3_48_225,(uint8*)"");

    if(audio_handle > 0)
    {
        MMISRVAUD_Stop(audio_handle);
        MMISRVMGR_Free(audio_handle);
        TTS_SetAudiohandle(0);
    }

    s_tts_pcm_stream_id ++; 



    req.notify   = TTS_HandleNotify;
    
    if(ring_type == TTS_RING_TYPE_CALL)
    {
        req.pri      = MMISRVAUD_PRI_HIGH;
    }
    //else if(ring_type == TTS_RING_TYPE_NEW_SMS)
    //{
    //    req.pri      = MMISRVAUD_PRI_LOW_P10;
    //}
    else
    {
        req.pri      = MMISRVAUD_PRI_NORMAL;
    }
    
    audio_srv.volume  = TTS_GetTTSVolume();
	audio_srv.volume_type = TTS_GetTTSVolumeType();
    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = 1;
    audio_srv.info.streaming.type = MMISRVAUD_TYPE_STREAMING;
    audio_srv.info.streaming.data = (uint8 *)(&head_info);
    audio_srv.info.streaming.data_len = sizeof(head_info);
    audio_srv.info.streaming.fmt = MMISRVAUD_RING_FMT_WAVE;
    audio_srv.info.streaming.cb = (void *)pCallbackfunc; /*lint !e611*/
    audio_srv.all_support_route = all_support_route;
    
    audio_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    if (0 == audio_handle)
    {
        error_code = FALSE;
    }
    else
    {
        TTS_SetAudiohandle(audio_handle);
    }
#if 0
	//V2上没有此接口，在create handle时直接把callback填进去
	MMIAUDIO_SetPcmPlayParam( 
								MMIAUDIO_APP,
								pCallbackfunc);	
#endif


    return error_code;
}


/*****************************************************************************/
// 	Description : judge the audio handle
//	Global resource dependence :                                
//	Note:for_tts
/*****************************************************************************/
PUBLIC BOOLEAN TTSAPI_GetAudioIsOpen(void)
{
    MMISRV_HANDLE_T audio_handle = TTS_GetAudiohandle();
    
    if(audio_handle > 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }  
}

/*****************************************************************************/
// 	Description : stop callback

//	Note:for_tts
/*****************************************************************************/
LOCAL void TTS_STOP_CALLBACK(MMISRVAUD_REPORT_RESULT_E result, DPARAM param)
{	
      //SCI_TRACE_LOW:"enter funciton TTS_STOP_CALLBACK() g_tts_is_running=%d"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_AUDIO_190_112_2_18_3_3_48_226,(uint8*)"d", TTS_GetTTSRunStatus());

      if(result != MMISRVAUD_REPORT_RESULT_STOP)
      {
        	/*end audio*/
        	if (TTS_GetTTSRunStatus())
        	{
        		TTS_SetTTSRunStatus(FALSE);
        		TTS_MemoryRelease();
        		MMI_RestoreFreq(); 
        	}
        	if (TTSAPI_GetAudioIsOpen())
        		TTS_StopAndClosePcmStreamHandle();

			// 20181107 start
			{
				extern void tiho_start_notify_stop_tts(void);
				tiho_start_notify_stop_tts();
			}
			// 20181107 end
        	
        	/*next play*/
        	if (TTS_IsRepeatePlayText())
        	{
        		if(TTS_IsRingCall())
        		{
					if(TTS_IsRingCallRepeate()) // 1.号码姓名重复播报
					{
						SCI_Sleep(200);
					}
					else // 2.播放系统铃声
					{
						TTSAPI_NotifyTTSOver();
						SCI_Sleep(200);
						if(MMICC_IsExistIncommingCall())
						{
							extern void HeroPlayCustomRing(void);
							HeroPlayCustomRing();
						}
						return;
					}
        		}
        		TTSAPI_SynthTextByTask();
        	}
        	else
            {
                TTSAPI_NotifyTTSOver();
            }
      }
}


/*****************************************************************************/
// 	Description : 播放录音（音量参数）
//	Global resource dependence :                                
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void TTSAPI_PlayPcmStream(uint32 pcm_stream_id)
{
    MMISRV_HANDLE_T audio_handle = TTS_GetAudiohandle();
    MMISRVAUD_VOLUME_T ring_vol  = TTS_GetTTSVolume();
    
    if (pcm_stream_id != s_tts_pcm_stream_id)	
    {
        //SCI_TRACE_LOW:"play_stream_id = %d, cur_stream_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_AUDIO_228_112_2_18_3_3_48_227,(uint8*)"dd", pcm_stream_id, s_tts_pcm_stream_id);
        return;
    }
		 
    //SCI_TRACE_LOW:"enter funciton VTTSAPI_PlayPcmStream()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_AUDIO_232_112_2_18_3_3_48_228,(uint8*)"");

    if (0 == audio_handle)  
    {
    	//SCI_TRACE_LOW:"TTSAPI_PlayPcmStream() Handle error !!!!"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_AUDIO_236_112_2_18_3_3_48_229,(uint8*)"");
		return;
    }

    if (TTS_GetTTSRunStatus() == FALSE)
		return;

    if(ring_vol != MMISRVAUD_GetVolume(audio_handle))
    {
        MMISRVAUD_SetVolume(audio_handle, ring_vol);
    }
	AUDIO_SetKeyTone(1);
    MMISRVAUD_Play(audio_handle, 0);

}

/*****************************************************************************/
// 	Description : 通过调用该函数来<停止>并<关闭>
//	Global resource dependence :                                
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void TTS_StopAndClosePcmStreamHandle(void)
{
    MMISRV_HANDLE_T audio_handle = TTS_GetAudiohandle();

	//SCI_TRACE_LOW:"enter funciton TTS_StopAndClosePcmStreamHandle()"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_AUDIO_295_112_2_18_3_3_48_230,(uint8*)"");
#if 0
	//V2上没有此接口，不需要重置callback
	MMIAUDIO_SetPcmPlayParam( 
								MMIAUDIO_APP,
								PNULL );	
#endif
	if(audio_handle > 0)
	{
	    MMISRVAUD_Stop(audio_handle);
	    MMISRVMGR_Free(audio_handle);
	    TTS_SetAudiohandle(0);
	}
}

/*****************************************************************************/
//  Description : 获得pcm stream id 值
//  Global resource dependence : none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC uint32 TTS_GetPcmStreamId(void)
{
    return s_tts_pcm_stream_id;
}

/*****************************************************************************/
//  Description : 获得audio_handle值
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T TTS_GetAudiohandle(void)
{
    return s_tts_audio_handle;
}

/*****************************************************************************/
//  Description : 设置audio_handle值
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T TTS_SetAudiohandle(MMISRV_HANDLE_T audio_handle)
{
    return s_tts_audio_handle = audio_handle;
}

/*****************************************************************************/
//  Description : notify handle function for TTS
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN TTS_HandleNotify(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_AUDIO_355_112_2_18_3_3_48_231,(uint8*)"dd", handle, param));
    
    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: HandleNotify() report_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_AUDIO_360_112_2_18_3_3_48_232,(uint8*)"d", report_ptr));
        
        if(report_ptr != PNULL)
        {            
            _DBG(            //MMISRV_TRACE_LOW:"[MMISRV]: HandleNotify(), report_ptr->report=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TTS_IFLYTEK_AUDIO_364_112_2_18_3_3_48_233,(uint8*)"d", report_ptr->report));
        
            switch(report_ptr->report)
            {
            case MMISRVAUD_REPORT_END:
                TTS_STOP_CALLBACK(report_ptr->data1, PNULL);
                break;

            default:
                break;
            }
        
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
