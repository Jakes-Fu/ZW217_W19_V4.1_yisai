/****************************************************************************
** File Name:      AAC_STREAM_DEOM.c                                                                           *
** Author:                                                                                                                      *
** Date:           11/10/2010                                                                                            *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.                                *
** Description:    the file AAC STREM PLAY     
*****************************************************************************
**                         Important Edit History                                                                         *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                                                     *
** 10/2010        hai.li             Create
******************************************************************************/

#define _AAC_STREAM_DEMO_C_
/**--------------------------------------------------------------------------*
 **                         Include Files                                                                                       *
 **--------------------------------------------------------------------------*/
#include "mmi_app_eng_trc.h"
#include "audio_api.h"
#include "asm.h"
#include "aac_lc_adp.h"
#include "sci_types.h"
//#include "aac_common.h"

/**--------------------------------------------------------------------------*
* MACRO DEFINITION 
*/  

#define IDLE             0
#define DECODING         1
#define PAUSE            2

/*buffer size*/
#define AAC_DRIVER_BUF_SIZE        (80*150)

const uint8 aac_demo_source[]= //play aac_stream data.
 #include "aac_demo_source.h"  

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                                                           *
 **--------------------------------------------------------------------------*/

extern uint32      VBDecodeOuputBuf[];
extern HAUDIOCODEC hAACLCCodec;
extern HAUDIODEV   hARMVB;

/*******************************************************************************
 *  type declare
 *******************************************************************************/
typedef struct
{
    HAUDIO  audio_handle;    /*audio handle*/
    int     i_state;
}aac_dec_ctx_t;

//typedef unsigned long long uint64_t;
typedef unsigned __int64 uint64_t;
typedef union
{
    struct  
    {
        //adts_fixed_header()   
         uint64_t syncword:12;  /*lint !e46*/ 
        //12 bslbf 
         uint64_t ID:1;  /*lint !e46*/ 
        //1 bslbf  
         uint64_t layer:2; /*lint !e46*/ 
        // 2 uimsbf 
         uint64_t protection_absent:1; /*lint !e46*/ 
        //1 bslbf 
         uint64_t profile:2;  /*lint !e46*/ 
        //2  uimsbf 
         uint64_t sampling_frequency_index:4;   /*lint !e46*/ 
        //4 uimsbf
         uint64_t private_bit:1;   /*lint !e46*/
        //1 bslbf
         uint64_t channel_configuration:3; /*lint !e46*/
        // 3 uimsbf 
         uint64_t original:1;/*lint !e46*/
         //copy;  1 bslbf 
         uint64_t home:1;   /*lint !e46*/ //1  bslbf

         //adts_variable_header()   
         uint64_t copyright_identification_bit:1; /*lint !e46*/ //  1 bslbf 
         uint64_t copyright_identification_start:1; /*lint !e46*/ // 1  bslbf 
         uint64_t frame_length:13;   /*lint !e46*/ // 13  bslbf
         uint64_t adts_buffer_fullness:11; /*lint !e46*/ // 11  bslbf 
         uint64_t number_of_raw_data_blocks_in_frame:2;  /*lint !e46*/ // 2  uimsfb
    }mBits; 
    uint8  usValue[8];
}AAC_STREAM_ADTS_HEAD_T; //lint !e46

/*******************************************************************************
 *  static variable
 *******************************************************************************/
LOCAL uint32 long_offset_source=0;
LOCAL uint32 source_size=0;
LOCAL aac_dec_ctx_t s_dec_ctx = {0};

LOCAL AAC_LC_HEADER_T header_info = {0};
LOCAL AAC_STREAM_ADTS_HEAD_T aac_adts_header_info;	


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

LOCAL int32 AACLC_InitAACStreamHead(
    uint8* stream_source_head,
    uint32 stream_source_size,
    AAC_LC_HEADER_T*  aac_header_init,//AAC_STREAM_HEAD_T * aac_header_init,
    uint32 *aac_frame_len)
{
    int32  rel = 0,i;
    int8   adts_m4a_file=0;
    uint8* header=PNULL;
    int16 profile, sr_index, channel_configuration ;
    AAC_STREAM_ADTS_HEAD_T* aac_adts_header=&aac_adts_header_info;
    static const uint32 adts_sample_rates[] = {96000,88200,64000,48000,44100,32000,24000,22050,16000,12000,11025,8000,7350,0,0,0};

    //SCI_TRACE_LOW:"aac_dec,[AACLC_InitAACStreamHead] IN! "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_111_112_2_18_2_18_17_218,(uint8*)"");

    SCI_PASSERT(stream_source_head!=PNULL, ("aac_dec,[AACLC_InitAACStreamHead]stream_source_head is NULL.")); /*assert verified*/
    SCI_PASSERT(stream_source_size>0, ("aac_dec,[AACLC_InitAACStreamHead]stream_source_size =0.")); /*assert verified*/
    
    /* parsing the file type */
    {
        header=(uint8*)stream_source_head;
        if (stream_source_size <9 )
        {
            //SCI_TRACE_LOW:"aac_dec,[AACLC_InitAACStreamHead] read head info error=! stream_source_size=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_121_112_2_18_2_18_17_219,(uint8*)"d", stream_source_size);
            return 1;
        }

        if (header[4] == 'f' && header[5] == 't' && header[6] == 'y' && header[7] == 'p')
        {
            adts_m4a_file = 0;  // m4a format
            //SCI_TRACE_LOW:"aac_dec,[AACLC_InitAACStreamHead]:m4a format"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_128_112_2_18_2_18_17_220,(uint8*)"");
        }
        else
        {
            if (header[0] == 'A' && header[1] == 'D' && header[2] == 'I' && header[3] == 'F')
            {
                adts_m4a_file = 2;  // adif format
                //SCI_TRACE_LOW:"aac_dec,[AACLC_InitAACStreamHead]:adif format cannot support!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_135_112_2_18_2_18_17_221,(uint8*)"");
                return 2;
            }
            else
            {
                adts_m4a_file = 1;  // adts format	
                //SCI_TRACE_LOW:"aac_dec,[AACLC_InitAACStreamHead]:adts format!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_141_112_2_18_2_18_17_222,(uint8*)"");
            }
        }
    }

    //get aac stream info
    if (1 == adts_m4a_file)   // aac adts format
    {   
        //SCI_TRACE_LOW:"aac_dec,[AACLC_InitAACStreamHead]:aac format adts"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_149_112_2_18_2_18_17_223,(uint8*)"");

        /* get the head info. */
        for(i=0;i<8;i++)
        {
            aac_adts_header->usValue[i]=*((uint8 *)(stream_source_head+i));
        }

        if (aac_adts_header->mBits.syncword != 0xFFF)   /* the decoder cann't support the ADIF format */
        {
            //SCI_TRACE_LOW:"aac_dec,[AACLC_InitAACStreamHead]:the syncword is err!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_159_112_2_18_2_18_17_224,(uint8*)"");
            return 1;
        }
        else
        {
            // get profile type.
            profile = aac_adts_header->mBits.profile;
            if (1 != profile)  // not AAC-LC
            {
                //SCI_TRACE_LOW:"aac_dec,[AACLC_InitAACStreamHead]:not AAC-LC,profile=%d !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_168_112_2_18_2_18_17_225,(uint8*)"d",profile);
                return 1;
            }
            else
            {
                profile=2;
            }

            // get sample rate.
            sr_index = aac_adts_header->mBits.sampling_frequency_index;
            if (sr_index >= 11 || sr_index < 0  )
            {
                //SCI_TRACE_LOW:"aac_dec,[AACLC_InitAACStreamHead]:sr_index > 11, sr_index=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_180_112_2_18_2_18_17_226,(uint8*)"d",sr_index);
                return 1;
            }

            //SCI_TRACE_LOW:"aac_dec,[AACLC_InitAACStreamHead]: sr_index=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_184_112_2_18_2_18_17_227,(uint8*)"d",sr_index);
            
            // get channel configuration.
            channel_configuration = aac_adts_header->mBits.channel_configuration;
            if (channel_configuration > 2 || channel_configuration==0)
            {
                //SCI_TRACE_LOW:"aac_dec,[AACLC_InitAACStreamHead]: channel_configuration is err =%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_190_112_2_18_2_18_17_228,(uint8*)"d",channel_configuration);
                return 1;
            }

            //SCI_TRACE_LOW:"aac_dec,[AACLC_InitAACStreamHead]: channel_configuration=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_194_112_2_18_2_18_17_229,(uint8*)"d",channel_configuration);
        }

        if(PNULL!=aac_header_init)
        {
            //aac_header_init->mBits.profile=profile;
            //aac_header_init->mBits.sample_rate_index=sr_index;
            //aac_header_init->mBits.channel_count=channel_configuration;
            
            header_info.profile=profile;
            header_info.sample_rate=adts_sample_rates[sr_index];
            header_info.channel_count=channel_configuration;
        }

        if(PNULL!=aac_frame_len)
        {
            *aac_frame_len=aac_adts_header->mBits.frame_length;
            //SCI_TRACE_LOW:"aac_dec,[AACLC_InitAACStreamHead], aac_frame_len=0x%X."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_211_112_2_18_2_18_17_230,(uint8*)"d", *aac_frame_len);
        }

       // SCI_ASSERT((profile==2));
       // SCI_ASSERT((sr_index>=0 && sr_index<12));
       // SCI_ASSERT((channel_configuration>0 && channel_configuration<3));
    }
    else   // aac m4a format
    {
        //SCI_TRACE_LOW:"aac_dec,[AACLC_InitAACStreamHead]:aac  m4a format cannot support!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_220_112_2_18_2_18_17_231,(uint8*)"");
        return 1;
    }

    return rel;
}

LOCAL void aac_dec_dummy_callback(HAUDIO hAudio, uint32 notify_info, uint32 affix_info)
{
    return;
}

/*
 * call back function for aac decoder of audio service to fetch the source data.
 */
LOCAL AUDIO_STREAM_GET_SRC_RESULT_E aac_demo_playCallback( //(HAUDIO hAudio, uint8** ppucDataAddr, uint32* puiDataLength, int32 offset)
    HAUDIO hAudio, 
    AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
    void *pvOtherInfo
    )
{
    uint32 puiDataLength,off_set=0;
    int32 ret=1;
    
    SCI_ASSERT(ptSrcDataInfo!=PNULL); /*assert verified*/
    //SCI_TRACE_LOW:"aac_dec, callback"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_244_112_2_18_2_18_17_232,(uint8*)"");

    puiDataLength = 0;

    if (s_dec_ctx.i_state != DECODING)
    {
        //SCI_TRACE_LOW:"aac_dec, callback, not in palying status."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_250_112_2_18_2_18_17_233,(uint8*)"");
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
        ret=AACLC_InitAACStreamHead((uint8*)(aac_demo_source+long_offset_source), 9, PNULL, &puiDataLength);
        if(ret)
        {
            //SCI_TRACE_LOW:"aac_dec, callback, AACLC_InitAACStreamHead= %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_265_112_2_18_2_18_17_234,(uint8*)"d",ret);
            return AUDIO_STREAM_GET_SRC_ERROR;
        }
            
        //SCI_TRACE_LOW:"aac_dec, callback, long_offset_source=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_269_112_2_18_2_18_17_235,(uint8*)"d", long_offset_source);
        //SCI_TRACE_LOW:"aac_dec, callback, puiDataLength=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_270_112_2_18_2_18_17_236,(uint8*)"d", puiDataLength);

        if(aac_adts_header_info.mBits.protection_absent)
        {
            off_set=7;
        }
        else 
        {
            off_set=9;
        }
            
        ptSrcDataInfo->tStreamCurSrcDataInfo.pucDataAddr  = (uint8 *)(aac_demo_source+long_offset_source+off_set);
        ptSrcDataInfo->tStreamCurSrcDataInfo.uiDataLength =(uint32)(puiDataLength-off_set);

        long_offset_source+=(puiDataLength);

        if(long_offset_source >=source_size)
        {
            long_offset_source=0; 
        }
    }

    return AUDIO_STREAM_GET_SRC_SUCCESS; 
}

/*
 * module open process
 */
 LOCAL void aac_dec_dummy_callbak(
    HAUDIO hAudio, 
    uint32 notify_info, 
    uint32 affix_info
)
{
    //SCI_TRACE_LOW:"aac_dec, aac_dec_dummy_callbak"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_303_112_2_18_2_18_18_237,(uint8*)"");
}

/*****************************************************************************/
//  Description : API of start wav stream play
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int AAC_Play_Start(void)
{
    /*dumming*/
    //SCI_TRACE_LOW:"aac_dec, start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_313_112_2_18_2_18_18_238,(uint8*)"");

    if(s_dec_ctx.i_state == DECODING)
    {
        return TRUE;
    }
    s_dec_ctx.i_state = DECODING;

    AUDIO_Play(s_dec_ctx.audio_handle, 0);

    return TRUE;
}

/*****************************************************************************/
//  Description : API of start wav stream open
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int AAC_Play_Open(void)
{
    int ret=0;

    //SCI_TRACE_LOW:"aac_dec, open"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_334_112_2_18_2_18_18_239,(uint8*)"");

    if (s_dec_ctx.i_state == DECODING)
    {
        //SCI_TRACE_LOW:"aac_dec, open, last operation first not stop"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_338_112_2_18_2_18_18_240,(uint8*)"");
        return SCI_ERROR;
    }

#if 1        
    ret=AACLC_InitAACStreamHead((uint8*)aac_demo_source, 9, &header_info,PNULL);
    if(ret)
    {
        //SCI_TRACE_LOW:"aac_dec, AAC_Play_Open, AACLC_InitAACStreamHead= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_346_112_2_18_2_18_18_241,(uint8*)"d",ret);
        return SCI_ERROR;
    }
#endif
  
     /* try to create aac handle */
    s_dec_ctx.audio_handle = AUDIO_CreateStreamBufferHandle(
                                hAACLCCodec,    
                                PNULL,     
                                hARMVB, 
                                PNULL,     
                                (uint8 *)&header_info,
                                sizeof(AAC_LC_HEADER_T), 
                                (uint32 *)(VBDecodeOuputBuf),    
                                AAC_DRIVER_BUF_SIZE,
                                aac_dec_dummy_callback,
                                aac_demo_playCallback);

                                
    if ( !s_dec_ctx.audio_handle)
    {
        //SCI_TRACE_LOW:"aac_dec, open(), create handle fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_367_112_2_18_2_18_18_242,(uint8*)"");
        return SCI_ERROR;
    }
           
    return TRUE;
}

/*****************************************************************************/
//  Description : API of  AAC play demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void AAC_Demo_Start(void)
{
    //SCI_TRACE_LOW:"aac_dec, start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_379_112_2_18_2_18_18_243,(uint8*)"");
    
    source_size=sizeof(aac_demo_source); 

    memset(&s_dec_ctx, 0, sizeof(aac_dec_ctx_t));

    long_offset_source=0;

    //SCI_TRACE_LOW:"aac_dec,source_len= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_387_112_2_18_2_18_18_244,(uint8*)"d",source_size);

    AUDIO_SetVolume(5);
    AAC_Play_Open();
    AAC_Play_Start();
}

/*****************************************************************************/
//  Description : API of  AAC stop demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void AAC_Demo_Stop(void)
{
    //SCI_TRACE_LOW:"aac_dec, close"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_399_112_2_18_2_18_18_245,(uint8*)"");

    if (s_dec_ctx.i_state != IDLE)
    {
        AUDIO_Stop(s_dec_ctx.audio_handle);
        s_dec_ctx.i_state = IDLE;
    }

    if (s_dec_ctx.audio_handle)
    {
        AUDIO_CloseHandle(s_dec_ctx.audio_handle);
        s_dec_ctx.audio_handle = NULL;
    }
}


/*****************************************************************************/
//  Description : API of  AAC pause demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void AAC_Demo_Pause(void)
{
    //SCI_TRACE_LOW:"aac_dec, pause"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_420_112_2_18_2_18_18_246,(uint8*)"");
 
    if (s_dec_ctx.i_state == DECODING)
    {
        AUDIO_Pause(s_dec_ctx.audio_handle);
        s_dec_ctx.i_state = PAUSE;
    }
}

/*****************************************************************************/
//  Description : API of  AAC resume demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void AAC_Demo_Resume(void)
{
    //SCI_TRACE_LOW:"aac_dec, resume"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,AAC_STREAM_DEMO_434_112_2_18_2_18_18_247,(uint8*)"");
    
    if (PAUSE == s_dec_ctx.i_state)
    {
        s_dec_ctx.i_state = DECODING;
        AUDIO_Resume(s_dec_ctx.audio_handle);
    }
}


