/******************************************************************************
 ** File Name:      audio_mtv.c                                            *
 ** Author:         Cherry.Liu                                                 *
 ** DATE:           18/9/2008                                                 *
 ** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    an adapter layer of mtv device for Audio Service.*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 18/9/2008      Cherry.Liu        Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_ref_aud_config_trc.h"
#include "sci_types.h"
#include "os_api.h"

#include "audio_api.h"
#include "aud_gen.h"
#include "asm.h"
#include "adm.h"
#include "apm_codec.h"
#include "adm_dp.h"

#include "audio_mtv.h"
#include "audio_config.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                  *
 **---------------------------------------------------------------------------*/
#define MTV_SAMPLE_RATE         (8000)

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL uint32         s_play_track_num = ADM_INVALID_TRACK;
LOCAL uint32        s_mtv_audio_open    =0;
LOCAL HAUDIOCODEC     hMtvCodecHandle  = INVALID_HANDLE;
LOCAL HAUDIO         hMtvRing        = INVALID_HANDLE;
LOCAL uint32*       puiTrackBuffer;
extern HCOLLISION     hCollision_MTV;
extern HAUDIODEV hMTVVB;

const AUDIO_CODEC_EXT_CFGINFO_T AUDIO_MTV_Codec_ExtCfgInfo = 
    {AUDIO_CODEC_PROD_HIGHPRIORITY-1,   AUDIO_CODEC_PROD_LOWPRIORITY,       80,      20,        40,        0, SCI_TRUE, 0, 0 };

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL AUDIO_RESULT_E MTV_Play(AUDIO_OBJECT_T* ptAudioObj);
LOCAL AUDIO_RESULT_E MTV_Stop(AUDIO_OBJECT_T* ptAudioObj);
LOCAL           uint32 MTV_GetSampleRate(AUDIO_OBJECT_T* ptAudioObj);
//LOCAL AUDIO_RESULT_E MTV_SetSampleRate(HAUDIO hAudioHandle, uint32 uiSamplerate);

/**---------------------------------------------------------------------------*
 **                     Local Function Definitions                             *
 **---------------------------------------------------------------------------*/
/*lint -save -e527  */
/*****************************************************************************/
//  Description:    This function plays midi.
//  Author:         Cherry.Liu
//    Note:           
/*****************************************************************************/
LOCAL AUDIO_RESULT_E MTV_Play(
    AUDIO_OBJECT_T* ptAudioObj
    )
{   
    //SCI_TRACE_LOW:"audio_mtv.c,[MTV_Play]! ptAudioObj:0x%08x,uiOffset:0x%08x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_MTV_80_112_2_18_1_2_6_58,(uint8*)"dd",ptAudioObj, ptAudioObj->uiCurSrcOffset);
    if(SCI_NULL == ptAudioObj) 
    {
        ASM_PASSERT_WARNING(SCI_FALSE,("SCI_NULL == ptAudioObj."));
        return AUDIO_ERROR;
    }
    s_play_track_num = ptAudioObj->uiTrackNum;
    SCI_ASSERT(ADM_INVALID_TRACK != s_play_track_num);/*assert verified*/

    return AUDIO_NO_ERROR;
}    

/*****************************************************************************/
//  Description:     
//  Author:         Cherry.Liu
//  Note:           
/*****************************************************************************/
LOCAL AUDIO_RESULT_E MTV_Stop(
    AUDIO_OBJECT_T* ptAudioObj
    )
{
    //SCI_TRACE_LOW:"InfoNES_Sound_threadx.cpp: NES_Stop"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_MTV_99_112_2_18_1_2_6_59,(uint8*)"");
    if(SCI_NULL == ptAudioObj) 
    {
      ASM_PASSERT_WARNING(SCI_FALSE,("SCI_NULL == ptAudioObj."));    
      return AUDIO_ERROR;
    }
    
        return AUDIO_NO_ERROR;    
}

/*****************************************************************************/
//  Description:     
//  Author:         Cherry.Liu
//  Note:           
/*****************************************************************************/
LOCAL uint32 MTV_GetSampleRate(
    AUDIO_OBJECT_T* ptAudioObj
    )
{
    if(SCI_NULL == ptAudioObj) 
    {
      SCI_PASSERT(SCI_FALSE,("SCI_NULL == ptAudioObj.")); /*assert verified*/   
      return AUDIO_ERROR;
    }

    if(ptAudioObj->uiOutputSamplerate == 0)
    {
        return MTV_SAMPLE_RATE;//8000
    }
    else
    {
        return ptAudioObj->uiOutputSamplerate;
    }
}

/*****************************************************************************/
//  Description:    This function is used to set sample rate.
//  Author:         Cherry.Liu
//    Note:           
/*****************************************************************************/
#if 0 // disable set samplerate
LOCAL AUDIO_RESULT_E MTV_SetSampleRate(
    HAUDIO hAudioHandle, 
    uint32 uiSamplerate
    )
{
    AUDIO_OBJECT_T* audio_obj = PNULL;

    audio_obj = AUDIO_SM_GetAudioObject(hAudioHandle);
    
    if(audio_obj == SCI_NULL)    
    {
        // This handle is not exist in the linklist.
        SCI_PASSERT(SCI_FALSE, ("%s, %d PNULL==audio_obj.", __MODULE__,__LINE__));/*assert verified*/
        return AUDIO_UNOPENED_HANDLE;
    }
    
    audio_obj->uiOutputSamplerate = uiSamplerate;
        
    return AUDIO_NO_ERROR;
}
#endif // 0

LOCAL AUDIO_RESULT_E MTV_IdentifyFormat(
    AUDIO_OBJECT_T* ptAudioObj
)
{
    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//  Description : Audio notify callback function.
//  Global resource dependence : none
//  Author: kai.wan
//  Note: 
/*****************************************************************************/
LOCAL void AudioMTVNotifyCallback(
                HAUDIO hAudio, 
                uint32 notify_info, 
                uint32 affix_info
                )
{
    //SCI_TRACE_LOW:"[AUDIO_MTV]:AudioMTVNotifyCallback:enter:hAudio = 0x%X, notify_info = 0x%X, affix_info = %ld"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_MTV_176_112_2_18_1_2_6_60,(uint8*)"ddd",hAudio, notify_info, affix_info);
}



/**---------------------------------------------------------------------------*
 **                     Public Function Definitions                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This Function is used to initialize the audio server. 
//  Author:         Cherry.Liu
//  Note:           
/*****************************************************************************/
PUBLIC HAUDIOCODEC MTV_RegCodecPlugger(
    HCOLLISION hCollision, 
    uint32 uiPriority
    )
{
    AUDIO_CODEC_T codec_mtv = {0};
    
    codec_mtv.pusCodecName = ANSI2UINT16("MTV_PLAY");
    codec_mtv.hCollisionHandle = hCollision;
    codec_mtv.pusCodecDescribe = SCI_NULL;
    codec_mtv.bVerifyAudioFormat = SCI_FALSE;
    codec_mtv.uiTrackOnPoint   = ADM_DEFAULT_TRACKON_POINT;
    codec_mtv.tExtOperation.ptExtFirstOpe     = SCI_NULL;
    codec_mtv.tExtOperation.pusExtOpeDescribe = SCI_NULL;

    codec_mtv.tExtCfgInfo = AUDIO_MTV_Codec_ExtCfgInfo;

    codec_mtv.tGenOperation.pConstruct        = SCI_NULL;
    codec_mtv.tGenOperation.pDeconstruct      = SCI_NULL;
    codec_mtv.tGenOperation.pPlayConstruct    = SCI_NULL;
    codec_mtv.tGenOperation.pStopDeconstruct  = SCI_NULL; 
    codec_mtv.tGenOperation.pResumeConstruct  = SCI_NULL;
    codec_mtv.tGenOperation.pPauseDeconstruct = SCI_NULL;
    
    codec_mtv.tGenOperation.pPlay = MTV_Play;
    codec_mtv.tGenOperation.pStop = MTV_Stop;
    codec_mtv.tGenOperation.pPause  = SCI_NULL;
    codec_mtv.tGenOperation.pResume = SCI_NULL;
    codec_mtv.tGenOperation.pSeek   = SCI_NULL;
    codec_mtv.tGenOperation.pSendStreamData = SCI_NULL;
    codec_mtv.tGenOperation.pSetSpeed       = SCI_NULL;
    codec_mtv.tGenOperation.pSetDigitalGain = SCI_NULL;
    codec_mtv.tGenOperation.pGetDigitalGain = SCI_NULL;
//    codec_mtv.tGenOperation.pGetFileInfo      = SCI_NULL;
    codec_mtv.tGenOperation.pIdentifyAudioFormat = MTV_IdentifyFormat;
    codec_mtv.tGenOperation.pGetPlayingInfo = SCI_NULL;
    codec_mtv.tGenOperation.pGetOutputSampleRate = MTV_GetSampleRate;
//    codec_mtv.tGenOperation.pSetSampleRate = MTV_SetSampleRate;

    codec_mtv.tExtOperation.ptExtFirstOpe = SCI_NULL;
    hMtvCodecHandle = AUDIO_PM_RegCodecPlugger(&codec_mtv);
    return hMtvCodecHandle;
}
/*****************************************************************************/
/*                                                                            */
/*       MTV_SoundInit() : mtv Sound Initialize                          */
/*                                                                            */
/*****************************************************************************/
PUBLIC void MTV_SoundInit(
    int samples, 
    int sample_rate 
    )
{
    uint32    uiTrackBufferSize = 0 ;
    uint32* puiOldTrackBuffer = SCI_NULL;

    //SCI_TRACE_LOW:"audio_mtv.c,[MTV_SoundInit] samples:%d,sample_rate:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_MTV_239_112_2_18_1_2_7_61,(uint8*)"dd",samples,sample_rate);

    //check input paras
    if(0==sample_rate)
    {
        //SCI_TRACE_LOW:"audio_mtv.c,[MTV_SoundInit] error 0==sample_rate."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_MTV_244_112_2_18_1_2_7_62,(uint8*)"");
        return;
    }
    s_mtv_audio_open = 0;
    uiTrackBufferSize = sample_rate*4 + 4;
    
    // register mtv codec 
    if (hMtvCodecHandle == INVALID_HANDLE)
    {
        hMtvCodecHandle  = MTV_RegCodecPlugger(hCollision_MTV, AUDIO_CODEC_PROD_HIGHPRIORITY);    
    }

    //create mtv handle
    if (hMtvRing == INVALID_HANDLE)
    {
        //alloc memory
        puiTrackBuffer = (uint32 *)SCI_ALLOC (uiTrackBufferSize);
        SCI_ASSERT (puiTrackBuffer != SCI_NULL);/*assert verified*/

        
        //create audio handle
        puiOldTrackBuffer = puiTrackBuffer;
        puiTrackBuffer = (uint32 *)(((uint32)puiTrackBuffer + 0x3) & ~0x3);
        uiTrackBufferSize -= 4;

        hMtvRing = AUDIO_CreateBufferHandle (hMtvCodecHandle, 
                                             SCI_NULL, 
                                             hMTVVB, 
                                             SCI_NULL, 
                                             SCI_NULL, 
                                             0, 
                                             (uint32 *)puiTrackBuffer, 
                                             (uiTrackBufferSize/4),
                                             AudioMTVNotifyCallback);
        
        SCI_ASSERT (hMtvRing != SCI_NULL);/*assert verified*/

        //record track buffer ptr
        puiTrackBuffer = puiOldTrackBuffer;
    }    
}
/*****************************************************************************/
/*                                                                            */
/*       MTV_SoundExit() : mtv Sound exit                          */
/*                                                                            */
/*****************************************************************************/
PUBLIC void MTV_SoundExit( 
    void 
    )
{

    //SCI_TRACE_LOW:"audio_mtv.c,[MTV_SoundExit]hMtvRing:0x%08x,puiTrackBuffer:0x%08x!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_MTV_293_112_2_18_1_2_7_63,(uint8*)"dd",hMtvRing,puiTrackBuffer);
    
    //Close handle
    if (hMtvRing != INVALID_HANDLE)
    {
        AUDIO_CloseHandle(hMtvRing);
        hMtvRing = INVALID_HANDLE;
    }
    
    //free memory
    if (puiTrackBuffer != SCI_NULL)
    {
        SCI_FREE(puiTrackBuffer);
    }
    s_mtv_audio_open = 0;    
}

/*****************************************************************************/
/*                                                                            */
/*        MTV_SoundOpen() : MTV Sound Open                           */
/*                                                                            */
/*        samples: PCM number per calling Soundoutput                          */
/*        sample_rate: PCM samplerate                                          */
/*****************************************************************************/
PUBLIC int MTV_SoundOpen( 
    int samples, 
    int sample_rate 
    )
{
    AUDIO_RESULT_E audio_result;
    if(s_mtv_audio_open == 1)
    {
        return 0;
    }
    s_mtv_audio_open = 1;    

    SCI_ASSERT (hMtvRing != INVALID_HANDLE);/*assert verified*/
    //SCI_ASSERT (sample_rate != 0);
    if(0==sample_rate)
    {
        //SCI_TRACE_LOW:"MTV_SoundOpen error 0==sample_rate."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_MTV_329_112_2_18_1_2_7_64,(uint8*)"");
        return 0;
    }
    //SCI_TRACE_LOW:"audio_mtv.c,[MTV_SoundOpen] samples %d, sample_rate %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_MTV_332_112_2_18_1_2_7_65,(uint8*)"dd", samples, sample_rate);

    // set the samplerate
    AUDIO_SetOutputSampleRate (hMtvRing, sample_rate);

    //begin to play
    audio_result = AUDIO_Play (hMtvRing, 0);

    SCI_ASSERT( audio_result == AUDIO_NO_ERROR);/*assert verified*/

    // Always Successful

    return 1;
}

/*****************************************************************************/
/*                                                                            */
/*        MTV_SoundClose() : MTV Sound Close                             */
/*                                                                            */
/*****************************************************************************/
PUBLIC void MTV_SoundClose(
    void
    )
{
    AUDIO_RESULT_E audio_result;
    
    //SCI_TRACE_LOW:"audio_mtv.c,[MTV_SoundClose]hMtvRing:0x%08x!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_MTV_355_112_2_18_1_2_7_66,(uint8*)"d",hMtvRing);
    if(s_mtv_audio_open == 0)
    {
        return ;
    }
    s_mtv_audio_open = 0;
    if(hMtvRing != INVALID_HANDLE)
    {
        audio_result = AUDIO_Stop (hMtvRing);
        SCI_ASSERT( audio_result == AUDIO_NO_ERROR);/*assert verified*/
    }
    else
    {
        //SCI_TRACE_LOW:"audio_mtv.c,[MTV_SoundClose] hMtvRing == INVALID_HANDLE!maybe mtv is stopped already!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_MTV_368_112_2_18_1_2_7_67,(uint8*)"");
    }
}

/*****************************************************************************/
/*                                                                            */
/*       MTV_SoundOutput() :   MTV Sound Output samples in word           */
/*       system need to write the PCM in ringbuffer                          */
/*                                                                           */
/*        samples: PCM number per calling Soundoutput                         */
/*        buff_ptr: PCM buffer pointer                                         */
/*****************************************************************************/
PUBLIC void MTV_SoundOutput( 
    int samples,
    int16 *buff_ptr
    )
{    
    SCI_ASSERT (hMtvRing != INVALID_HANDLE);/*assert verified*/
    SCI_ASSERT (s_play_track_num != ADM_INVALID_TRACK);/*assert verified*/
    if(SCI_NULL==buff_ptr)
    {
        //SCI_TRACE_LOW:"MTV_SoundOutput SCI_NULL==buff_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_MTV_385_112_2_18_1_2_7_68,(uint8*)"");
        return;
    }
    // Write back to track buffer managed by ADM
    if (samples != 0)
    {
        AUDIO_DP_WriteRingBuffer(    s_play_track_num, 
                                    (int16*)buff_ptr, 
                                    (int16*)buff_ptr, 
                                    samples);        
    }
}

/*****************************************************************************/
/*                                                                            */
/*       MTV_SoundOutputStereo() :  MTV Sound Output samples in word           */
/*       system need to write the PCM in ringbuffer                          */
/*                                                                           */
/*        samples: PCM number per calling Soundoutput                         */
/*        lbuff_ptr: left channel PCM buffer pointer                                         */
/*        rbuff_ptr: right channel PCM buffer pointer                                         */
/*****************************************************************************/
PUBLIC void MTV_SoundOutputStereo( 
    int samples, 
    int16 *lbuff_ptr, 
    int16 *rbuff_ptr
    )
{    
    if((SCI_NULL == lbuff_ptr) || (SCI_NULL == rbuff_ptr))
    {
        //SCI_TRACE_LOW:"MTV_SoundOutputStereo 0x%x, 0x%x."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AUDIO_MTV_410_112_2_18_1_2_7_69,(uint8*)"dd", lbuff_ptr, rbuff_ptr);
        return;
    }
    SCI_ASSERT(hMtvRing != INVALID_HANDLE);/*assert verified*/
    SCI_ASSERT (s_play_track_num != ADM_INVALID_TRACK);/*assert verified*/
    
    // Write back to track buffer managed by ADM
    if (samples != 0)
    {
        AUDIO_DP_WriteRingBuffer(    s_play_track_num, 
                                    (int16*)lbuff_ptr, 
                                    (int16*)rbuff_ptr, 
                                    samples);        
    }
}

/*****************************************************************************/
/*                                                                           */
/*       MTV_SoundGetPCMNum() :  MTV Sound Output samples in word          */
/*       system need to write the PCM in ringbuffer                          */
/*                                                                           */
/*       return PCM in halfword num                                              */
/*****************************************************************************/
PUBLIC int MTV_SoundGetPCMNum(
    void
    )
{
    SCI_ASSERT (s_play_track_num != ADM_INVALID_TRACK);/*assert verified*/

    return (AUDIO_DP_GetDecoderDataLen (s_play_track_num));
}
/*lint -restore */  
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

