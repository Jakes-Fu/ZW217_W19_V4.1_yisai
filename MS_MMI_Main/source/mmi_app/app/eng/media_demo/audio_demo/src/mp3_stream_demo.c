/****************************************************************************
** File Name:      MP3_STREAM_DEOM.c                                        *
** Author:                                                                  *
** Date:           11/10/2010                                               *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    the file mp3 STREM PLAY                                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2010        hai.li          Create
******************************************************************************/

#define _MP3_STREAM_DEMO_C_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"
//#include "sig_code.h"
#include "mmi_app_eng_trc.h"
#include "audio_prod.h"
#include "mp3_adp.h"

/**--------------------------------------------------------------------------*
 * MACRO DEFINITION 
 */                                                   
#define IDLE      0
#define DECODING  1

#define MP3_DRIVER_BUF_SIZE  (160*150)

#define MP3_RESOUCE_MONE 

#ifdef MP3_RESOUCE_MONE
const unsigned char mp3_demo_source[]= //play amr_stream data.
 #include "mp3_demo_source.h"  //44100HZ 1
#else 
const unsigned char mp3_demo_source[]=
 #include "mp3_demo_source1.h" //22050HZ 2
#endif 

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

typedef struct 
{
    uint32  audioSamplingRate;  /*sample rate, Hz*/
    uint8   audioChannels;      /*channels*/
}  MP3_Info_T;

typedef struct
{
    HAUDIO       mp3_handle;    /*device handle*/
    MP3_Info_T   MP3_Info;  /*mp3 information*/
    int          i_state;

}mp3_dec_ctx_t;

/*******************************************************************************
 *  static variable
 *******************************************************************************/
LOCAL uint32 long_offset_source=0;
LOCAL uint32 source_size=0;
LOCAL mp3_dec_ctx_t s_dec_ctx = {0};
LOCAL MP3_STREAM_BUFFER_T  mp3_header_info = {0};


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern uint32 VBDecodeOuputBuf[];

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :call back function for mp3 driver
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

// for mp3 stream play and stop

/*****************************************************************************/
//  Description :call back function for mp3 driver
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_STREAM_GET_SRC_RESULT_E MP3_callback(//HAUDIO hAudio, uint8** const ppucDataAddr, uint32* const puiDataLength) 
    HAUDIO hAudio, 
    AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
    void *pvOtherInfo
    )
{

    uint32 puiDataLength;

    SCI_ASSERT(ptSrcDataInfo!=PNULL); /*assert verified*/
    //SCI_TRACE_LOW:"mp3_dec, callback"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MP3_STREAM_DEMO_92_112_2_18_2_18_24_296,(uint8*)"");

    puiDataLength = 0;

    if (s_dec_ctx.i_state != DECODING)
    {
        //SCI_TRACE_LOW:"mp3_dec, callback, not in palying status."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MP3_STREAM_DEMO_98_112_2_18_2_18_24_297,(uint8*)"");
        ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength = 0;
        return AUDIO_STREAM_GET_SRC_ERROR;
    }
/*
    if ( 0 )
    {
        //get source process
        return AUDIO_STREAM_GET_SRC_WAIT;
    }
    else*/
    {
        ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr  =(uint8*)(mp3_demo_source+long_offset_source);
        puiDataLength = 512;
        long_offset_source+=512;

        if(long_offset_source >source_size)
        {
#ifdef MP3_RESOUCE_MONE
            long_offset_source=0xa0;//0xd0;
#else
            long_offset_source=0;
#endif
        }

        ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength =puiDataLength;

    }

    return AUDIO_STREAM_GET_SRC_SUCCESS;

}


LOCAL void mp3_dec_dummy_callbak(
    HAUDIO hAudio, 
    uint32 notify_info, 
    uint32 affix_info
)
{
    return;
}


/*****************************************************************************/
//  Description : API of start mp3 stream play
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int MP3_Play_Start(void)
{
    /*dumming*/
    //SCI_TRACE_LOW:"mp3_dec, start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MP3_STREAM_DEMO_148_112_2_18_2_18_24_298,(uint8*)"");

    s_dec_ctx.i_state = DECODING;

    AUDIO_Play(s_dec_ctx.mp3_handle, 0);

    return TRUE;
}

/*****************************************************************************/
//  Description : API of start mp3 stream open
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int MP3_Play_Open(void)
{
    uint32 track_buf_size = 0;

    //SCI_TRACE_LOW:"mp3_dec, open"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MP3_STREAM_DEMO_165_112_2_18_2_18_24_299,(uint8*)"");

    if (s_dec_ctx.i_state == DECODING)
    {
        //SCI_TRACE_LOW:"mp3_dec, open, last operation first not stop"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MP3_STREAM_DEMO_169_112_2_18_2_18_24_300,(uint8*)"");
        return FALSE;
    }

#ifdef MP3_RESOUCE_MONE
    //SCI_TRACE_LOW:"mp3_dec, open MP3_RESOUCE_MONE %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MP3_STREAM_DEMO_174_112_2_18_2_18_24_301,(uint8*)"d",1);
    s_dec_ctx.MP3_Info.audioSamplingRate = 44100;
    s_dec_ctx.MP3_Info.audioChannels = 1;
#else	
    //SCI_TRACE_LOW:"mp3_dec, open MP3_RESOUCE_MONE %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MP3_STREAM_DEMO_178_112_2_18_2_18_24_302,(uint8*)"d",2);
    s_dec_ctx.MP3_Info.audioSamplingRate = 22050;
    s_dec_ctx.MP3_Info.audioChannels = 2;
#endif	
    mp3_header_info.bType = TRUE;
    mp3_header_info.unMp3StreamData.tMp3FormatInfo.uiChannelNum = s_dec_ctx.MP3_Info.audioChannels;
    mp3_header_info.unMp3StreamData.tMp3FormatInfo.uiSampleRate  = s_dec_ctx.MP3_Info.audioSamplingRate ;

    if (s_dec_ctx.MP3_Info.audioSamplingRate <= 12000)
    {
        track_buf_size = MP3_DRIVER_BUF_SIZE >> 2;
    }
    else if (s_dec_ctx.MP3_Info.audioSamplingRate <= 24000)
    {
        track_buf_size = MP3_DRIVER_BUF_SIZE >> 1;
    }
    else
    {
        track_buf_size = MP3_DRIVER_BUF_SIZE;
    }

    s_dec_ctx.mp3_handle = AUDIO_CreateStreamBufferHandle(
                                            hMP3DSPCodec,
                                            PNULL,
                                            hARMVB,
                                            PNULL,
                                            (uint8 *)&mp3_header_info,
                                            sizeof(MP3_STREAM_BUFFER_T),
                                            (uint32 *)(VBDecodeOuputBuf),
                                            track_buf_size,
                                            mp3_dec_dummy_callbak,
                                            MP3_callback
                                            );


    if ( !s_dec_ctx.mp3_handle)
    {
        //SCI_TRACE_LOW:"mp3_dec, open(), create handle fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MP3_STREAM_DEMO_215_112_2_18_2_18_24_303,(uint8*)"");
        return SCI_ERROR;
    }

    return TRUE;

}

/*****************************************************************************/
//  Description : API of  mp3 play demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/

PUBLIC void MP3_Demo_Start(void)
{
    //SCI_TRACE_LOW:"mp3_dec, start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MP3_STREAM_DEMO_229_112_2_18_2_18_24_304,(uint8*)"");
    source_size=sizeof(mp3_demo_source); 

#ifdef MP3_RESOUCE_MONE
    long_offset_source=0xa0;//0xd0;
#else
    long_offset_source=0;
#endif
    //SCI_TRACE_LOW:"mp3_dec, source_len= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MP3_STREAM_DEMO_237_112_2_18_2_18_24_305,(uint8*)"d",source_size);

    MP3_Play_Open();
    MP3_Play_Start();
}

/*****************************************************************************/
//  Description : API of  mp3 stop demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void MP3_Demo_Stop(void)
{
    //SCI_TRACE_LOW:"mp3_dec, close"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MP3_STREAM_DEMO_248_112_2_18_2_18_24_306,(uint8*)"");

    if (s_dec_ctx.i_state == DECODING)
    {
        s_dec_ctx.i_state = IDLE;
        AUDIO_Stop(s_dec_ctx.mp3_handle);
    }

    if (s_dec_ctx.mp3_handle)
    {
        AUDIO_CloseHandle(s_dec_ctx.mp3_handle);
        s_dec_ctx.mp3_handle = NULL;
    }
}


/*****************************************************************************/
//  Description : API of  mp3 pause demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/

PUBLIC void MP3_Demo_Pause(void)
{
    //SCI_TRACE_LOW:"mp3_dec, pause"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MP3_STREAM_DEMO_270_112_2_18_2_18_25_307,(uint8*)"");

    if (s_dec_ctx.i_state == DECODING)
    {
        AUDIO_Pause(s_dec_ctx.mp3_handle);
        s_dec_ctx.i_state = IDLE;
    }

}

/*****************************************************************************/
//  Description : API of  mp3 resume demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void MP3_Demo_Resume(void)
{
    //SCI_TRACE_LOW:"mp3_dec, resume"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MP3_STREAM_DEMO_285_112_2_18_2_18_25_308,(uint8*)"");

    if (s_dec_ctx.i_state == IDLE)
    {
        s_dec_ctx.i_state = DECODING;
        AUDIO_Resume(s_dec_ctx.mp3_handle);
    }
}

