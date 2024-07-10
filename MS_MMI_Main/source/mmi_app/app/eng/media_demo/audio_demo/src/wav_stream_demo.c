/****************************************************************************
** File Name:      WAV_STREAM_DEOM.c                                             *
** Author:                                                                   *
** Date:           11/10/2010                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    the file WAV STREM PLAY     
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2010        hai.li          Create
******************************************************************************/

#define _WAV_STREAM_DEMO_C_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_eng_trc.h"
#include "audio_api.h"
#include "asm.h"
#include "wav_adp.h"

/**--------------------------------------------------------------------------*
 * MACRO DEFINITION 
 */   
#define IDLE          0
#define DECODING      1
#define PAUSE         2

/*buffer size*/
#define PCM_DRIVER_BUF_SIZE        (80*150)

//#define WAV_RESOUCE_PCM 
 
#ifdef WAV_RESOUCE_PCM
 const  unsigned char wav_demo_source[]= //play WAV_PCM wav stream data.
 #include "wav_demo_source.h"  
#else 
 const  unsigned char wav_demo_source[]= //play WAV_IMAADPCM wav stream data.
 #include "wav_demo_source1.h"  
#endif 

/*******************************************************************************
 *  type declare
 *******************************************************************************/
//typedef unsigned long long uint64_t;
//typedef unsigned long uint64_t;

typedef struct 
{
	uint32 	audioSamplingRate;   	/*sample rate, Hz*/
	uint8  	audioChannels;       	/*channels*/
}PCM_Info_T;

typedef struct
{
    HAUDIO      pcm_handle; /*device handle*/
    PCM_Info_T  pcm_Info;   /*pcm information*/
    int         i_state;
}wav_dec_ctx_t;

/*******************************************************************************
 *  static variable
 *******************************************************************************/
LOCAL uint32 long_offset_source=0;
LOCAL uint32 source_size=0;
LOCAL wav_dec_ctx_t s_dec_ctx = {0};
LOCAL WAV_STREAM_BUFFER_T wav_header_info = {0};

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern uint32 VBDecodeOuputBuf[];
extern HAUDIOCODEC hWAVCodec;
extern HAUDIODEV hARMVB;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :call back function for wav driver
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_STREAM_GET_SRC_RESULT_E pcm_callback(//HAUDIO hAudio, uint8** const ppucDataAddr, uint32* const puiDataLength) 
    HAUDIO hAudio, 
    AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
    void *pvOtherInfo
    )
{
    uint32 puiDataLength;

    SCI_ASSERT(ptSrcDataInfo!=PNULL); /*assert verified*/

    //SCI_TRACE_LOW:"wav_dec, callback"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_85_112_2_18_2_18_25_309,(uint8*)"");

    ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr  = PNULL;
    puiDataLength = 0;

    if (s_dec_ctx.i_state != DECODING)
    {
        //SCI_TRACE_LOW:"wav_dec, callback, not in palying status."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_92_112_2_18_2_18_25_310,(uint8*)"");
        ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength = 0;
        return AUDIO_STREAM_GET_SRC_ERROR;
    }
/*
    if( 0 )
    {
        //get source error process
        return AUDIO_STREAM_GET_SRC_WAIT;
    }
    else*/
    {
        ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr  = (uint8 *)(wav_demo_source+long_offset_source);	

        if(long_offset_source+2048 >source_size)
        {
            puiDataLength = source_size-long_offset_source;    
            long_offset_source=source_size; 
        }
        else
        {
            puiDataLength = 2048;
            long_offset_source+=2048;
        }
 
        if(long_offset_source >=source_size)
        {
#ifdef WAV_RESOUCE_PCM
            long_offset_source=0x2e;
#else
            long_offset_source=0x3C;
#endif

            if(puiDataLength==0)
            {
                ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr  = (uint8 *)(wav_demo_source+long_offset_source);    
                puiDataLength =2048; 
                long_offset_source+=2048;	
            }
        }
        ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength =puiDataLength;
    }

    //SCI_TRACE_LOW:"wav_dec, i_len %d, offset=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_134_112_2_18_2_18_25_311,(uint8*)"dd", puiDataLength,long_offset_source );
    return AUDIO_STREAM_GET_SRC_SUCCESS;
}


LOCAL void pcm_dec_dummy_callbak(
    HAUDIO hAudio, 
    uint32 notify_info, 
    uint32 affix_info
)
{
    return;
}

/*****************************************************************************/
//  Description : API of start wav stream play
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int WAV_Play_Start(void)
{
    /*dumming*/
    //SCI_TRACE_LOW:"wav_dec, start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_155_112_2_18_2_18_25_312,(uint8*)"");

    if(s_dec_ctx.i_state == DECODING)
    {
        return TRUE;
    }
    s_dec_ctx.i_state = DECODING;
    AUDIO_Play(s_dec_ctx.pcm_handle, 0);
    return TRUE;
}

/*****************************************************************************/
//  Description : API of start wav stream open
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int WAV_Play_Open(void)
{

    WAV_STREAM_FORMATINFO_T* wav_format_info_ptr = PNULL;
    //SCI_TRACE_LOW:"wav_dec, open"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_174_112_2_18_2_18_25_313,(uint8*)"");

    if (s_dec_ctx.i_state == DECODING)
    {
        //SCI_TRACE_LOW:"wav_dec, open, last operation first not stop"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_178_112_2_18_2_18_25_314,(uint8*)"");
        return SCI_ERROR;
    }


    wav_header_info.bType  = SCI_TRUE;
    wav_format_info_ptr = &(wav_header_info.unWavStreamData.tWavFormatInfo);
#ifdef WAV_RESOUCE_PCM
    wav_format_info_ptr->eSubtype = WAV_PCM;
    wav_format_info_ptr->uiBitPerSample  = 16;
    wav_format_info_ptr->uiBlockAlign    = 4;
    wav_format_info_ptr->uiChannelNum    = 2;
    wav_format_info_ptr->uiDataSize      =0x382de0;
    wav_format_info_ptr->uiSampleRate    = 44100;
#else 
    wav_format_info_ptr->eSubtype = WAV_IMAADPCM;
    wav_format_info_ptr->uiBitPerSample  = 4;
    wav_format_info_ptr->uiBlockAlign    = 256;
    wav_format_info_ptr->uiChannelNum    = 1;
    wav_format_info_ptr->uiDataSize      =0x1ae08;
    wav_format_info_ptr->uiSampleRate    = 8000;
#endif 

    //SCI_TRACE_LOW:"wav_dec, open, uiBitPerSample[%d],uiBlockAlign[%d],uiChannelNum[%d],uiDataSize[%d],uiSampleRate[%d],eSubtype[%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_203_112_2_18_2_18_25_315,(uint8*)"dddddd",wav_format_info_ptr->uiBitPerSample, wav_format_info_ptr->uiBlockAlign, wav_format_info_ptr->uiChannelNum,wav_format_info_ptr->uiDataSize, wav_format_info_ptr->uiSampleRate, wav_format_info_ptr->eSubtype);

      s_dec_ctx.pcm_handle = AUDIO_CreateStreamBufferHandle(
                                                            hWAVCodec,
                                                            PNULL,
                                                            hARMVB,
                                                            PNULL,
                                                            (uint8 *)&wav_header_info,
                                                            sizeof(WAV_STREAM_BUFFER_T),
                                                            (uint32 *)(VBDecodeOuputBuf),
                                                            PCM_DRIVER_BUF_SIZE,
                                                            pcm_dec_dummy_callbak,
                                                            pcm_callback);

    if ( !s_dec_ctx.pcm_handle)
    {
        //SCI_TRACE_LOW:"wav_dec, open(), create handle fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_219_112_2_18_2_18_25_316,(uint8*)"");
        return SCI_ERROR;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : API of  wav play demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void WAV_Demo_Start(void)
{
    //SCI_TRACE_LOW:"wav_dec, start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_231_112_2_18_2_18_25_317,(uint8*)"");
    source_size=sizeof(wav_demo_source); 

    memset(&s_dec_ctx, 0, sizeof(wav_dec_ctx_t));

#ifdef WAV_RESOUCE_PCM
    long_offset_source=0x2e;
#else	
    long_offset_source=0x3C;
#endif	

    //SCI_TRACE_LOW:"wav_dec,source_len= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_242_112_2_18_2_18_25_318,(uint8*)"d",source_size);
    AUDIO_SetVolume(5);
    WAV_Play_Open();
    WAV_Play_Start();
}

/*****************************************************************************/
//  Description : API of  WAV stop demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void WAV_Demo_Stop(void)
{
    //SCI_TRACE_LOW:"wav_dec, close"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_253_112_2_18_2_18_25_319,(uint8*)"");

    if (s_dec_ctx.i_state != IDLE)
    {
        AUDIO_Stop(s_dec_ctx.pcm_handle);
        s_dec_ctx.i_state = IDLE;
    }

    if (s_dec_ctx.pcm_handle)
    {
        AUDIO_CloseHandle(s_dec_ctx.pcm_handle);
        s_dec_ctx.pcm_handle = NULL;
    }

}


/*****************************************************************************/
//  Description : API of  wav pause demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/

PUBLIC void WAV_Demo_Pause(void)
{
    //SCI_TRACE_LOW:"wav_dec, pause"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_276_112_2_18_2_18_25_320,(uint8*)"");

    if (s_dec_ctx.i_state == DECODING)
    {
        AUDIO_Pause(s_dec_ctx.pcm_handle);
        s_dec_ctx.i_state = PAUSE;
    }

}

/*****************************************************************************/
//  Description : API of  WAV resume demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void WAV_Demo_Resume(void)
{
    //SCI_TRACE_LOW:"wav_dec, resume"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WAV_STREAM_DEMO_291_112_2_18_2_18_25_321,(uint8*)"");
    if (PAUSE == s_dec_ctx.i_state)
    {
        s_dec_ctx.i_state = DECODING;
        AUDIO_Resume(s_dec_ctx.pcm_handle);
    }
}


