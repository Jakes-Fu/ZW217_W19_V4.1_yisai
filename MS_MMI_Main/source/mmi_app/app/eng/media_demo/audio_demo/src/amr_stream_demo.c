/****************************************************************************
** File Name:      _AMR_STREAM_DEMO_C_.c                                    *
** Author:                                                                  *
** Date:           11/10/2010                                               *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    the file AMR STREM PLAY     
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME            DESCRIPTION                               *
** 10/2010        hai.li          Create
******************************************************************************/

#define _AMR_STREAM_DEMO_C_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_eng_trc.h"
#include "audio_api.h"
#include "asm.h"
#include "amr_adp.h"
#include "dsp_codec_adp.h"
#include "audio_config.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MAX_AMR_TESTBUF  32
#define IDLE          0
#define DECODING      1
#define PAUSE         2

/*buffer size*/
#define AMR_BUF_SIZE   (1024*8)

const uint8 amr_ok[]= //play aac_stream data.
 #include "amr_demo_source.h"  
 
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern uint32      VBDecodeOuputBuf[];
//extern HAUDIOCODEC hAMRCodec;
//extern HAUDIODEV   hARMVB;

 
/*******************************************************************************
 *  type declare
 *******************************************************************************/
typedef struct
{
    HAUDIO  pcm_handle; /*device handle*/
    int     i_state;
}pcm_dec_ctx_t;

/*******************************************************************************
 *  static variable
 *******************************************************************************/
LOCAL uint32 long_offset_source=0;
LOCAL uint32 source_size=0;
LOCAL pcm_dec_ctx_t s_dec_ctx = {0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

// for stream play and stop

/*****************************************************************************/
//  Description :call back function for amr driver
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL AUDIO_STREAM_GET_SRC_RESULT_E amr_callback(//HAUDIO hAudio, uint8** const ppucDataAddr, uint32* const puiDataLength) 
    HAUDIO hAudio, 
    AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
    void *pvOtherInfo
    )
{
    uint32 puiDataLength;
    
    SCI_ASSERT(ptSrcDataInfo!=PNULL);/*assert verified*/

    //SCI_TRACE_LOW:"amr_dec, callback"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_STREAM_DEMO_73_112_2_18_2_18_19_261,(uint8*)"");

    ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr  = PNULL;
    puiDataLength = 0;
    
    if (s_dec_ctx.i_state != DECODING)
    {
        //SCI_TRACE_LOW:"amr_dec, callback, not in palying status."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_STREAM_DEMO_80_112_2_18_2_18_19_262,(uint8*)"");
        ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength = 0;
        return AUDIO_STREAM_GET_SRC_ERROR;
    }
    /*
    if ( 0 )
    {
        //get source error process
        return AUDIO_STREAM_GET_SRC_WAIT;
    }
    else*/
    {
 
        ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr  =(uint8*) (amr_ok+long_offset_source);	
        if(long_offset_source+512 >source_size)
        {
            //SCI_TRACE_LOW:"amr_dec, long_offset_source=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_STREAM_DEMO_96_112_2_18_2_18_19_263,(uint8*)"d", long_offset_source);
            puiDataLength = source_size-long_offset_source;    
            long_offset_source=source_size;
            if(puiDataLength==0)
            {
                long_offset_source=6;
                ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr  = (uint8*) (amr_ok+long_offset_source);	
                puiDataLength = 512;
                long_offset_source+=512;
            }
            // if(long_offset_source >=source_size)long_offset_source=6; 
        }
        else
        {
            puiDataLength = 512;
            long_offset_source+=512;	
        }
        ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength =puiDataLength;

    }

    //SCI_TRACE_LOW:"amr_dec, *puiDataLength=%d,long_offset_source=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_STREAM_DEMO_117_112_2_18_2_18_19_264,(uint8*)"dd",puiDataLength,long_offset_source);
    return AUDIO_STREAM_GET_SRC_SUCCESS;
}

/*
 * module open process
 */
LOCAL void amr_dec_dummy_callbak(
    HAUDIO hAudio, 
    uint32 notify_info, 
    uint32 affix_info
)
{
       return;
}
 

/*****************************************************************************/
//  Description : API of start AMR stream play
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int AMR_Play_Start(void)
{
    /*dumming*/
    //SCI_TRACE_LOW:"wav_dec, start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_STREAM_DEMO_140_112_2_18_2_18_19_265,(uint8*)"");

    if(s_dec_ctx.i_state == DECODING)
    {
        return TRUE;
    }
    s_dec_ctx.i_state = DECODING;
    AUDIO_Play(s_dec_ctx.pcm_handle, 0);
    return TRUE;
}


/*****************************************************************************/
//  Description : API of start AMR stream open
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int AMR_Play_Open(void)
{
    //SCI_TRACE_LOW:"amr_dec, open"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_STREAM_DEMO_158_112_2_18_2_18_20_266,(uint8*)"");

    if (s_dec_ctx.i_state == DECODING)
    {
        //SCI_TRACE_LOW:"amr_dec, open, last operation first not stop"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_STREAM_DEMO_162_112_2_18_2_18_20_267,(uint8*)"");
        return SCI_ERROR;
    }

    s_dec_ctx.pcm_handle = AUDIO_CreateStreamBufferHandle(
                                        hAMRCodec,
                                        PNULL,
                                        hARMVB,
                                        PNULL,
                                        (uint8 *)PNULL,
                                        0,
                                        (uint32 *)(VBDecodeOuputBuf),
                                        AMR_BUF_SIZE,
                                        amr_dec_dummy_callbak,
                                        amr_callback
                                        );
    if ( !s_dec_ctx.pcm_handle)
    {
        //SCI_TRACE_LOW:"wav_dec, open(), create handle fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_STREAM_DEMO_180_112_2_18_2_18_20_268,(uint8*)"");
        return SCI_ERROR;
    }
           
    return TRUE;
}

/*****************************************************************************/
//  Description : API of  AMR play demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void AMR_Demo_Start(void)
{
    //SCI_TRACE_LOW:"amr_dec, start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_STREAM_DEMO_192_112_2_18_2_18_20_269,(uint8*)"");
    
    source_size=sizeof(amr_ok); 

    memset(&s_dec_ctx, 0, sizeof(pcm_dec_ctx_t));

    long_offset_source=0x6;

    //SCI_TRACE_LOW:"amr_dec,source_len= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_STREAM_DEMO_200_112_2_18_2_18_20_270,(uint8*)"d",source_size);
    AUDIO_SetVolume(5);
    AMR_Play_Open();
    AMR_Play_Start();
}

/*****************************************************************************/
//  Description : API of  AMR stop demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void AMR_Demo_Stop(void)
{
   //SCI_TRACE_LOW:"amr_dec, close"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_STREAM_DEMO_211_112_2_18_2_18_20_271,(uint8*)"");

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
//  Description : API of  AMR pause demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/

PUBLIC void AMR_Demo_Pause(void)
{
    //SCI_TRACE_LOW:"amr_dec, pause"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_STREAM_DEMO_234_112_2_18_2_18_20_272,(uint8*)"");
 
    if (s_dec_ctx.i_state == DECODING)
    {
        AUDIO_Pause(s_dec_ctx.pcm_handle);
        s_dec_ctx.i_state = PAUSE;
    }

}

/*****************************************************************************/
//  Description : API of  AMR resume demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void AMR_Demo_Resume(void)
{
    //SCI_TRACE_LOW:"amr_dec, resume"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AMR_STREAM_DEMO_249_112_2_18_2_18_20_273,(uint8*)"");
    
    if (PAUSE == s_dec_ctx.i_state)
    {
        s_dec_ctx.i_state = DECODING;
        AUDIO_Resume(s_dec_ctx.pcm_handle);
    }
}
