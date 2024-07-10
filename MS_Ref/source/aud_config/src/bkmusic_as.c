/******************************************************************************
 ** File Name:      bkmusic_as.c                                              *
 ** Author:         Cherry.Liu                                                *
 ** DATE:           24/06/2008                                                *
 ** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file serves as a sound device for background music.  *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 24/06/2008    Cherry.Liu        Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_ref_aud_config_trc.h"
#include "os_api.h"
#include "sci_api.h"
#include "audio_config.h"
#include "vb_drv.h"
#include "aud_gen.h"
#include "adm.h"
#include "layer1_audio.h"
#include "bkmusic_as.h"

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
#define DEFAULT_OUTPUT_PCM_LEN 160//words
#define DEFAULT_UPLINK_GAIN   0x16a7//@cherry needs check
#define DEFAULT_DOWNLINK_GAIN 0x5a7//@cherry needs check

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL AUDIO_DEVICE_HANDLE_T *s_background_music_device_ptr;
LOCAL volatile BOOLEAN   s_is_background_music_on =  SCI_FALSE;
LOCAL uint32 s_magic_gain_type                    =  0x3;//MAGIC_PARAM_TYPE_E:MAGIC_BOTH_VALID
LOCAL uint16 s_magic_uplink_gain                  =  DEFAULT_UPLINK_GAIN;
LOCAL uint16 s_magic_downlink_gain                =  DEFAULT_DOWNLINK_GAIN;

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is to start device
//  Author:         Cherry.Liu
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E BKMUSIC_Play (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting);
/*****************************************************************************/
//  Description:    This function is to stop device
//  Author:         Cherry.Liu
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E BKMUSIC_Stop (DEV_SETTING_T *ptDevSetting);
/*****************************************************************************/
//  Description:    This function is to pause device
//  Author:         Cherry.Liu
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E BKMUSIC_Pause (DEV_SETTING_T *ptDevSetting);
/*****************************************************************************/
//  Description:    This function is to resume device
//  Author:         Cherry.Liu
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E BKMUSIC_Resume (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting);
/*****************************************************************************/
//  Description:    This function is to fetch pcm data from track buffer.
//  Author:         Cherry.Liu
//  Note:
//****************************************************************************/
LOCAL BOOLEAN BKMUSIC_ReadDataFromBuffer (void);

/*****************************************************************************/
//  Description:    get appropriate output samplerate.
//  Author:         Shujing.Dong
//  Note:
//****************************************************************************/
LOCAL uint32 BKMUSIC_GetAppropriateOutputSamplerate (uint32 uiSampleRate);

/**---------------------------------------------------------------------------*
 **                         Public function definitions                       *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is to register background music device to audio service.
//  Author:         Cherry.Liu
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV BKMUSIC_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("BKMusic"), strlen ("BKMusic") * 2 + 2);
    tDeviceInfo.bIsNeedTrack     = SCI_TRUE;
    tDeviceInfo.uiMinFetchLength = DEFAULT_OUTPUT_PCM_LEN;
    tDeviceInfo.tAudioDevOpe.pOpen   = BKMUSIC_Play;
    tDeviceInfo.tAudioDevOpe.pClose  = BKMUSIC_Stop;
    tDeviceInfo.tAudioDevOpe.pPause  = BKMUSIC_Pause;
    tDeviceInfo.tAudioDevOpe.pResume = BKMUSIC_Resume;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate    = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate    = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pStartRecord      = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pStopRecord       = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pRegRecCallback   = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pUnRegRecCallback = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pReadData         = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate =
        BKMUSIC_GetAppropriateOutputSamplerate;

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);
    s_background_music_device_ptr = AUDIO_DM_GetDeviceHandleStruct (hDevice);  // Record device structure

    return hDevice;
}

/*****************************************************************************/
//  Description:    This function is to set the volume of background music.
//  Author:         Cherry.Liu
//  Note:
//****************************************************************************/
PUBLIC void  BKMUSIC_SetGain (
    uint32  eParamType,
    uint16  uiUplinkGain,
    uint16  uiDownlinkGain
)
{
#ifndef _OLD_MIX_VOICE_INTERFACE
    if (eParamType == 0x100)
    {
        eParamType = 0x3;
    }
#endif
    s_magic_gain_type     =  eParamType;
    s_magic_uplink_gain   =  uiUplinkGain;
    s_magic_downlink_gain =  uiDownlinkGain;

    //SCI_TRACE_LOW:"bkmusic_as.c,[BKMUSIC_SetGain]: the state and gain value! s_is_background_music_on:%d,eParamType:0x%08x,uiUplinkGain:0x%08x,uiDownlinkGain:0x%08x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BKMUSIC_AS_142_112_2_18_1_2_8_70,(uint8*)"dddd",s_is_background_music_on,eParamType,uiUplinkGain,uiDownlinkGain);

    if (s_is_background_music_on) //set the volume
    {

        //SCI_TRACE_LOW:"bkmusic_as.c,[BKMUSIC_SetGain]:set the gains down!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BKMUSIC_AS_147_112_2_18_1_2_8_71,(uint8*)"");

#ifdef _OLD_MIX_VOICE_INTERFACE
         LAYER1_SetMagicVoiceParam (eParamType,uiUplinkGain,uiDownlinkGain);
#else
       LAYER1_SetMixVoiceParam (eParamType,uiUplinkGain,uiDownlinkGain);
#endif
    }

}


/**---------------------------------------------------------------------------*
 **                         Local function definitions                       *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is to start device
//  Author:         Cherry.Liu
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E BKMUSIC_Play (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
    if (8000 != uiSampleRate)
    {
        //SCI_TRACE_LOW:"BKMUSIC_Play input para error. uiSampleRate:0."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BKMUSIC_AS_169_112_2_18_1_2_8_72,(uint8*)"");
        return AUDIO_PARAM_ERROR;
    }

    if (!s_is_background_music_on)
    {
#ifdef _OLD_MIX_VOICE_INTERFACE
        //SCI_TRACE_LOW:"bkmusic_as.c,[BKMUSIC_Play]:call LAYER1_StartMagicVoiceFunc!s_magic_gain_type:0x%08x,s_magic_uplink_gain:0x%08x,s_magic_downlink_gain:0x%08x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BKMUSIC_AS_177_112_2_18_1_2_8_73,(uint8*)"ddd",s_magic_gain_type,s_magic_uplink_gain,s_magic_downlink_gain);

        LAYER1_StartMagicVoiceFunc (
            (MAG_CALLBACK) BKMUSIC_ReadDataFromBuffer,
            s_magic_gain_type,
            s_magic_uplink_gain,
            s_magic_downlink_gain);
#else
        //SCI_TRACE_LOW:"bkmusic_as.c,[BKMUSIC_Play]:call LAYER1_StartMixVoiceFunc!s_magic_gain_type:0x%08x,s_magic_uplink_gain:0x%08x,s_magic_downlink_gain:0x%08x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BKMUSIC_AS_186_112_2_18_1_2_8_74,(uint8*)"ddd",s_magic_gain_type,s_magic_uplink_gain,s_magic_downlink_gain);
{  
        uint32 eParamType=0;

    if (s_magic_gain_type == 0x100)
    {
        eParamType = 0x3;
    }
    else
    {
              eParamType= s_magic_gain_type;
    }


        LAYER1_StartMixVoiceFunc (
            (MAG_CALLBACK) BKMUSIC_ReadDataFromBuffer,
            eParamType,
            s_magic_uplink_gain,
            s_magic_downlink_gain);
}
#endif
        s_is_background_music_on = SCI_TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"bkmusic_as.c,[BKMUSIC_Play]:already open!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BKMUSIC_AS_211_112_2_18_1_2_8_75,(uint8*)"");
    }

    return AUDIO_NO_ERROR;
}
/*****************************************************************************/
//  Description:    This function is to stop device
//  Author:         Cherry.Liu
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E BKMUSIC_Stop (DEV_SETTING_T *ptDevSetting)
{

    if (s_is_background_music_on)
    {
#ifdef _OLD_MIX_VOICE_INTERFACE
        LAYER1_StopMagicVoiceFunc();
#else
        LAYER1_StopMixVoiceFunc();
#endif
        s_is_background_music_on = SCI_FALSE;

        //SCI_TRACE_LOW:"bkmusic_as.c,[BKMUSIC_Stop]:call LAYER1_StopMixVoiceFunc!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BKMUSIC_AS_232_112_2_18_1_2_8_76,(uint8*)"");

    }
    else
    {
        //SCI_TRACE_LOW:"bkmusic_as.c,[BKMUSIC_Stop]:already off!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BKMUSIC_AS_237_112_2_18_1_2_8_77,(uint8*)"");
    }

    return AUDIO_NO_ERROR;
}
/*****************************************************************************/
//  Description:    This function is to pause device
//  Author:         Cherry.Liu
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E BKMUSIC_Pause (DEV_SETTING_T *ptDevSetting)
{
    if (s_is_background_music_on)
    {
#ifdef _OLD_MIX_VOICE_INTERFACE
        LAYER1_StopMagicVoiceFunc();
#else
        LAYER1_StopMixVoiceFunc();
#endif
        s_is_background_music_on = SCI_FALSE;

        //SCI_TRACE_LOW:"bkmusic_as.c,[BKMUSIC_Pause]:call LAYER1_StopMixVoiceFunc!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BKMUSIC_AS_257_112_2_18_1_2_8_78,(uint8*)"");

    }
    else
    {
        //SCI_TRACE_LOW:"bkmusic_as.c,[BKMUSIC_Pause]:already off!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BKMUSIC_AS_262_112_2_18_1_2_8_79,(uint8*)"");
    }

    return AUDIO_NO_ERROR;
}
/*****************************************************************************/
//  Description:    This function is to resume device
//  Author:         Cherry.Liu
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E BKMUSIC_Resume (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
    if (8000 != uiSampleRate)
    {
        //SCI_TRACE_LOW:"BKMUSIC_Resume input para error. uiSampleRate:0."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BKMUSIC_AS_275_112_2_18_1_2_8_80,(uint8*)"");
        return AUDIO_PARAM_ERROR;
    }

    if (!s_is_background_music_on)
    {

#ifdef _OLD_MIX_VOICE_INTERFACE
        //SCI_TRACE_LOW:"bkmusic_as.c,[BKMUSIC_Resume]:call LAYER1_StartMagicVoiceFunc!s_magic_gain_type:0x%08x,s_magic_uplink_gain:0x%08x,s_magic_downlink_gain:0x%08x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BKMUSIC_AS_284_112_2_18_1_2_8_81,(uint8*)"ddd",s_magic_gain_type,s_magic_uplink_gain,s_magic_downlink_gain);


        LAYER1_StartMagicVoiceFunc (
            (MAG_CALLBACK) BKMUSIC_ReadDataFromBuffer,
            s_magic_gain_type,
            s_magic_uplink_gain,
            s_magic_downlink_gain);
#else
        //SCI_TRACE_LOW:"bkmusic_as.c,[BKMUSIC_Resume]:call LAYER1_StartMixVoiceFunc!s_magic_gain_type:0x%08x,s_magic_uplink_gain:0x%08x,s_magic_downlink_gain:0x%08x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BKMUSIC_AS_294_112_2_18_1_2_8_82,(uint8*)"ddd",s_magic_gain_type,s_magic_uplink_gain,s_magic_downlink_gain);


        LAYER1_StartMixVoiceFunc (
            (MAG_CALLBACK) BKMUSIC_ReadDataFromBuffer,
            s_magic_gain_type,
            s_magic_uplink_gain,
            s_magic_downlink_gain);
#endif

        s_is_background_music_on = SCI_TRUE;

    }
    else
    {
        //SCI_TRACE_LOW:"bkmusic_as.c,[BKMUSIC_Resume]:already open!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BKMUSIC_AS_309_112_2_18_1_2_8_83,(uint8*)"");
    }

    return AUDIO_NO_ERROR;
}
/*****************************************************************************/
//  Description:    This function is to get track buffer.
//  Author:         Cherry.Liu
//  Note:
//****************************************************************************/
LOCAL ADM_DecOutputBuf_T *BKMUSIC_GetAudioDataBuf (void)
{
    ADM_TRACK_T *track_ptr = SCI_NULL;
    uint16 n = 0;

    //SCI_TRACE_LOW("bkmusic_as.c,[BKMUSIC_GetAudioDataBuf]:uiTotalTrackon = 0x%x\r\n",  s_background_music_device_ptr->uiTotalTrackon);

    if (s_background_music_device_ptr->uiTotalTrackon == 0)
    {
        return SCI_NULL;
    }

    track_ptr = s_background_music_device_ptr->ptTrackList;

    while (track_ptr)
    {
        n = track_ptr->uiTrackNum;

        if (!g_adm_track_controller[n].track_on)
        {
            track_ptr = track_ptr->ptNextTrack;
            continue;
        }

        return &g_adm_dec_output_buf[n];
    }

    return SCI_NULL;
}

/*****************************************************************************/
//  Description:    This function is to fetch pcm data from track buffer.
//  Author:         Cherry.Liu
//  Note:
//****************************************************************************/
LOCAL BOOLEAN BKMUSIC_ReadDataFromBuffer (void)
{
    uint32 buffer_size=0,tail_count=0,head_count=0;
    uint16 data_buf[DEFAULT_OUTPUT_PCM_LEN];
    int16 *src_data_ptr = SCI_NULL;
    ADM_DecOutputBuf_T *pcm_data_buf = SCI_NULL;
    int16 *data_ptr = SCI_NULL;

    //Check if buf lacks data
    pcm_data_buf = BKMUSIC_GetAudioDataBuf();

    if (!pcm_data_buf)
    {
        //SCI_TRACE_LOW:"bkmusic_as.c,[BKMUSIC_ReadDataFromBuffer]: pcm_data_buf is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BKMUSIC_AS_365_112_2_18_1_2_8_84,(uint8*)"");
        return SCI_TRUE ;
    }

    if (pcm_data_buf->data_count < DEFAULT_OUTPUT_PCM_LEN)

    {
        //SCI_TRACE_LOW:"bkmusic_as.c,[BKMUSIC_ReadDataFromBuffer]: The num of pcm data is less than 160 words!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BKMUSIC_AS_372_112_2_18_1_2_8_85,(uint8*)"");
        return SCI_TRUE ;
    }

    //copy DEFAULT_OUTPUT_PCM_LEN data to a buf
    buffer_size = pcm_data_buf->buffer_size;

    switch (pcm_data_buf->channel_choise_e)
    {
        case DEV_DATA_OUTPUT_CHANNEL_CHOICE_NORMAL:
            {
                data_ptr = pcm_data_buf->data_buffer_ptr;
            }
            break;
        case DEV_DATA_OUTPUT_CHANNEL_CHOICE_LEFT:
            {
                data_ptr = pcm_data_buf->data_buffer_ptr;
            }
            break;
        case DEV_DATA_OUTPUT_CHANNEL_CHOICE_RIGHT:
            {
                data_ptr = pcm_data_buf->data_buffer_ptr_r;
            }
            break;
        default:
            {
                ASM_PASSERT_WARNING (SCI_FALSE, ("channel_choise_e:%d.", pcm_data_buf->channel_choise_e));
            }
            break;
    }

    if(SCI_NULL == data_ptr)
    {
        return SCI_TRUE;
    }
    src_data_ptr = data_ptr + pcm_data_buf->data_out_pos;

    if (pcm_data_buf->data_out_pos + DEFAULT_OUTPUT_PCM_LEN > buffer_size)
    {
        tail_count = buffer_size - pcm_data_buf->data_out_pos;
        head_count = DEFAULT_OUTPUT_PCM_LEN - tail_count;

        if (DEV_DATA_OUTPUT_CHANNEL_CHOICE_NORMAL==pcm_data_buf->channel_choise_e)
        {
            uint32 i=0;

            for (i=0; i<tail_count; i++)
            {
                pcm_data_buf->data_buffer_ptr[pcm_data_buf->data_out_pos + i] =
                    (pcm_data_buf->data_buffer_ptr[pcm_data_buf->data_out_pos + i] +
                     pcm_data_buf->data_buffer_ptr_r[pcm_data_buf->data_out_pos + i]) >>1;
            }

            for (i=0; i<head_count; i++)
            {
                pcm_data_buf->data_buffer_ptr[i] =
                    (pcm_data_buf->data_buffer_ptr[i] +
                     pcm_data_buf->data_buffer_ptr_r[i]) >>1;
            }
        }

        // Write data to data buffer in memory.
        SCI_MEM16CPY ( (uint16 *) data_buf, (uint16 *) src_data_ptr, tail_count <<1);
        SCI_MEM16CPY ( (uint16 *) (data_buf + tail_count), (uint16 *) data_ptr, head_count <<1);

    }
    else
    {
        if (DEV_DATA_OUTPUT_CHANNEL_CHOICE_NORMAL==pcm_data_buf->channel_choise_e)
        {
            uint32 i=0;

            for (i=0; i<DEFAULT_OUTPUT_PCM_LEN; i++)
            {
                pcm_data_buf->data_buffer_ptr[pcm_data_buf->data_out_pos + i] =
                    (pcm_data_buf->data_buffer_ptr[pcm_data_buf->data_out_pos + i] +
                     pcm_data_buf->data_buffer_ptr_r[pcm_data_buf->data_out_pos + i]) >>1;
            }
        }

        SCI_MEM16CPY ( (uint16 *) data_buf, (uint16 *) src_data_ptr, DEFAULT_OUTPUT_PCM_LEN <<1);
    }


    // update pointer and counter.
    pcm_data_buf->data_count   -= DEFAULT_OUTPUT_PCM_LEN;

    pcm_data_buf->data_out_pos += DEFAULT_OUTPUT_PCM_LEN;

    if (pcm_data_buf->data_out_pos >= buffer_size)
    {
        pcm_data_buf->data_out_pos -= buffer_size;
    }

    //send data to share memory
#ifdef _OLD_MIX_VOICE_INTERFACE
    LAYER1_WriteVoiceData ( (uint16 *) data_buf,DEFAULT_OUTPUT_PCM_LEN);
#else
    if (0 != (s_magic_gain_type & AUD_BKMUSIC_UPLINK_VALID))
    {
        LAYER1_SetMixVoiceUplinkData((uint16 *) data_buf,DEFAULT_OUTPUT_PCM_LEN);
    }
    if (0 != (s_magic_gain_type & AUD_BKMUSIC_DOWNLINK_VALID))
    {
        LAYER1_SetMixVoiceDownlinkData((uint16 *) data_buf,DEFAULT_OUTPUT_PCM_LEN);
    }
#endif
    return SCI_FALSE ;
}

/*****************************************************************************/
//  Description:    get appropriate output samplerate.
//  Author:         Shujing.Dong
//  Note:
//****************************************************************************/
LOCAL uint32 BKMUSIC_GetAppropriateOutputSamplerate (uint32 uiSampleRate)
{
    return 8000;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}

#endif  // End of a2dp_as.c

