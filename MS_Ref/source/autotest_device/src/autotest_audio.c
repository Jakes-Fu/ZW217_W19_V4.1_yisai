/****************************************************************************
** File Name:      autotest_audio.c                                             *
** Author:                                                                   *
** Date:           06/12/2011                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    the file autotest audio
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011        xudong zhang           Create
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "audio_api.h"
#include "asm.h"
#include "adm.h"
#include "wav_adp.h"
#include "dsp_codec_adp.h"
#include "audio_api.h"
#include "audio_prod.h"
#include "aud_gen.h"
#include "audio_config.h"
#include "adpcmrecord_adp.h"
#include "datadump.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifndef SCI_FAILURE
#define     SCI_FAILURE     0x01
#endif
#ifndef SCI_OK
#define     SCI_OK          0x00
#endif


/*buffer size*/
#define PLAY_SAMPLERATE     (16000)         // need resample from 48k
#define PLAY_BUF_SIZE       (5760)         // (180*16*2)

#define RECD_SAMPLERATE     (16000)
#define RECD_BUF_SIZE       (3200)          // 16000*2/50*5 > RECD_GET_SIZE

#define RECD_GET_BLOCK      (400)
#define RECD_GET_COUNT      (4)             // RECD_GET_BLOCK*4

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*******************************************************************************
 *  type declare
 *******************************************************************************/
#define STATE_STOP      0
#define STATE_START     1
#define STATE_DATAOK    2

typedef struct
{
    HAUDIO handle;
    uint32 fs;
    uint32 ch;

    uint8 *buf;    // buffer for stream cb
    uint32 size;    // buffer size
    uint32 state;   // play: stop; start
                    // recd: stop; start without enough data; enough data
}play_strem_ctx_t;

typedef struct
{
    HAUDIO handle;
    uint32 fs;
    uint32 ch;

    uint8 *buf;     // buffer for stream cb
    uint32 size;    // buffer size
    uint32 vaild_size;    // buffer size with vaild data
    uint32 pwrite;  // buffer write point (for recd)
    uint32 state;   // play: stop; start
                    // recd: stop; start without enough data; enough data

    AUDIO_DEVICE_HANDLE_T* pdev;  // device point for cb
}recd_strem_ctx_t;

typedef struct
{
    play_strem_ctx_t play;
    recd_strem_ctx_t recd;
}audiotest_ctx_t;

/*******************************************************************************
 *  static variable
 *******************************************************************************/
static audiotest_ctx_t audiotest_ctl = {0};
static uint8 *recd_get_buf = PNULL;

// need changed to 48k
#ifdef CHIP_ENDIAN_LITTLE
const unsigned char sin1k_44k[] = {
0x92,0x02,0xcb,0x0b,0xd0,0x14,0x1d,0x1d,0xfc,0x24,0x17,0x2c,0x4a,0x32,0x69,0x37,
0x92,0x3b,0x4e,0x3e,0x22,0x40,0x56,0x40,0x92,0x3f,0x12,0x3d,0x88,0x39,0x10,0x35,
0xf0,0x2e,0x51,0x28,0xce,0x20,0x7f,0x18,0xd5,0x0f,0xdA,0x06,0xdf,0xfd,0xa4,0xf4,
0xa2,0xeb,0x39,0xe3,0x57,0xdb,0x3d,0xd4,0x1f,0xce,0xe2,0xc8,0xb1,0xc4,0xc0,0xc1,
0xec,0xbf,0xc1,0xbf,0xa4,0xc0,0xf2,0xc2,0x18,0xc6,0xc2,0xca,0xc8,0xd0,0x36,0xd7,
0xbb,0xde,0xe6,0xe6,0xa5,0xef,0xa6,0xf8};
#else
const unsigned char sin1k_44k[] = {
0x02,0x92,0x0b,0xcb,0x14,0xd0,0x1d,0x1d,0x24,0xfc,0x2c,0x17,0x32,0x4a,0x37,0x69,
0x3b,0x92,0x3e,0x4e,0x40,0x22,0x40,0x56,0x3f,0x92,0x3d,0x12,0x39,0x88,0x35,0x10,
0x2e,0xf0,0x28,0x51,0x20,0xce,0x18,0x7f,0x0f,0xd5,0x06,0xdA,0xfd,0xdf,0xf4,0xa4,
0xeb,0xa2,0xe3,0x39,0xdb,0x57,0xd4,0x3d,0xce,0x1f,0xc8,0xe2,0xc4,0xb1,0xc1,0xc0,
0xbf,0xec,0xbf,0xc1,0xc0,0xa4,0xc2,0xf2,0xc6,0x18,0xca,0xc2,0xd0,0xc8,0xd7,0x36,
0xde,0xbb,0xe6,0xe6,0xef,0xa5,0xf8,0xa6};
#endif

extern uint32 VBDecodeOuputBuf[];

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION                                   */
/*---------------------------------------------------------------------------*/
LOCAL uint32 Autotest_Resmapler_48ktoLower(
    uint8 *src,
    uint32 srclen,
    uint8 *dst,
    uint32 dst_samplerate)
{
    uint32 cnt = 0;
    uint8 *src_ptr, *dst_ptr;
    uint32 resample_ratio = (uint32)(48000/dst_samplerate);

    src_ptr = src;
    dst_ptr = dst;

    while(srclen > cnt){
        *(dst_ptr) = *(src_ptr);
        *(dst_ptr+1) = *(src_ptr+1);
        dst_ptr += 2;
        src_ptr += 2*resample_ratio;
        cnt += 2*resample_ratio;
    }
    return (uint32)(cnt/resample_ratio);
}

LOCAL uint32 Autotest_Databuf_Set(
    uint8 *src,
    uint32 srclen,
    uint8 *dst,
    uint32 dstlen)
{
    uint8 *src_ptr;
    uint8 *dest_ptr;
    uint32 need_to_copy;

    SCI_ASSERT(PNULL != src);
    SCI_ASSERT(PNULL != dst);

    SCI_ASSERT(0 <= srclen);
    SCI_ASSERT(0 <= dstlen);

    // force resample from 48k to 16k
    srclen = Autotest_Resmapler_48ktoLower(src, srclen, dst, PLAY_SAMPLERATE);

    dest_ptr = dst + srclen;
    src_ptr = dst;
    need_to_copy = dstlen - srclen;

    while (need_to_copy >= srclen) {
        SCI_MEMCPY(dest_ptr, src_ptr, srclen);
        dest_ptr += srclen;
        need_to_copy -= srclen;
    }

    if (need_to_copy > 0)
        SCI_MEMCPY(dest_ptr, src_ptr, need_to_copy);

    return SCI_OK;
}

// playback stream
LOCAL AUDIO_STREAM_GET_SRC_RESULT_E playback_dev_stream_cb(
    HAUDIO hAudio,
    AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
    void *pvOtherInfo)
{
    AUDIO_STREAM_CURSRCDATA_INFO_T* info_p =\
        &(ptSrcDataInfo->tStreamCurSrcDataInfo);
    play_strem_ctx_t* play_p = &(audiotest_ctl.play);

    if (audiotest_ctl.play.buf) {
        //SCI_TRACE_LOW("[AUTOTEST][playback_dev_stream_cb][E] update %d",
        //    play_p->size);

        info_p->uiDataLength                = play_p->size;
        info_p->pucDataAddr                 =(uint8*)(play_p->buf);
        info_p->tOffsetTimeInfo.eOffsetType = AUDIO_SRCDATA_INFO_OFFSET_TIME;
        info_p->tOffsetTimeInfo.uOffsetTime.uiCurTime = 0;
        return AUDIO_STREAM_GET_SRC_SUCCESS;
    }

    SCI_TRACE_LOW("[AUTOTEST][playback_dev_stream_cb][E] callback");
    return AUDIO_STREAM_GET_SRC_ERROR;
}

LOCAL void playback_dev_dummy_cb(
    HAUDIO hAudio,
    uint32 notify_info,
    uint32 affix_info
)
{
    return;
}

LOCAL uint32 Speaker_Play_Open(uint32 is_stereo)
{
    WAV_STREAM_FORMATINFO_T* wav_format_info_ptr = PNULL;
    WAV_STREAM_BUFFER_T autotest_wav_header_info = {0};
    play_strem_ctx_t* play_p = &(audiotest_ctl.play);

    SCI_ASSERT (PNULL != play_p->buf);
    SCI_ASSERT (0 < play_p->size);

    if (play_p->state != STATE_STOP) {
        SCI_TRACE_LOW("[AUTOTEST][%s][E] re-open!", __FUNCTION__);
        return SCI_FAILURE;
    }

// play
    play_p->fs = PLAY_SAMPLERATE;
    play_p->ch = (is_stereo>0)?2:1;

    autotest_wav_header_info.bType = SCI_TRUE;
    wav_format_info_ptr = &(autotest_wav_header_info.unWavStreamData.tWavFormatInfo);
    wav_format_info_ptr->eSubtype           = WAV_PCM;
    wav_format_info_ptr->uiBitPerSample     = 16;
    wav_format_info_ptr->uiBlockAlign       = 2;
    wav_format_info_ptr->uiChannelNum       = play_p->ch;
    wav_format_info_ptr->uiDataSize         = sizeof(WAV_STREAM_BUFFER_T);
    wav_format_info_ptr->uiSampleRate       = play_p->fs;

    if (!play_p->handle) {
        play_p->handle = AUDIO_CreateStreamBufferHandle(
                                    hWAVCodec,
                                    PNULL,
                                    hAUDDEV,
                                    PNULL,
                                    (uint8 *)&autotest_wav_header_info,
                                    sizeof(WAV_STREAM_BUFFER_T),
                                    (uint32 *)(VBDecodeOuputBuf),
                                    9600,
                                    playback_dev_dummy_cb,
                                    playback_dev_stream_cb);
    }
    if (!play_p->handle) {
        SCI_TRACE_LOW("[AUTOTEST][%s][E] create handle!", __FUNCTION__);
        return SCI_FAILURE;
    }

    SCI_TRACE_LOW("[AUTOTEST][%s] success", __FUNCTION__);
    return SCI_OK;
}

LOCAL uint32 Speaker_Play_Start(void)
{
    play_strem_ctx_t* play_p = &(audiotest_ctl.play);

    if (play_p->state != STATE_STOP) {
        SCI_TRACE_LOW("[AUTOTEST][%s] re-open!", __FUNCTION__);
        return SCI_OK;
    }

    play_p->state = STATE_START;
    AUDIO_Play(play_p->handle, 0);
    SCI_TRACE_LOW("[AUTOTEST][%s] success", __FUNCTION__);
    return SCI_OK;
}

// record stream
LOCAL AUDIO_STREAM_GET_SRC_RESULT_E record_dev_stream_cb(
    HAUDIO hAudio,
    AUDIO_GETSRCDATA_INFO_T * const ptSrcDataInfo,
    void *pvOtherInfo)
{
    return AUDIO_STREAM_GET_SRC_SUCCESS;
}

LOCAL void record_dev_dummy_cb(
    HAUDIO hAudio,
    uint32 notify_info,
    uint32 affix_info
)
{
    return;
}

LOCAL void record_stream_cb(
    uint32 datalen)
{
    static uint32 i=0;
    uint32 datalen_read = 0;
    recd_strem_ctx_t* recd_p = &(audiotest_ctl.recd);

    if ((PNULL == recd_p->pdev) || ((datalen<<1) > recd_p->size)) {
        SCI_TRACE_LOW("\n[%s][%d] invalid param %d %d %d %d\n",
            __FUNCTION__, SCI_GetTickCount(), i++,
            datalen, recd_p->pwrite, recd_p->size);
        recd_p->state = STATE_START;
        recd_p->pwrite = 0;
        return;
    }

    if (recd_p->size < recd_p->pwrite + (datalen<<1)) {
        recd_p->vaild_size = recd_p->pwrite;
        recd_p->pwrite = 0;
    }

    recd_p->pdev->tDeviceInfo.tAudioDevOpe.pReadData(
                ((uint8 *)recd_p->buf + recd_p->pwrite),
                datalen, &datalen_read);

    recd_p->pwrite += datalen_read;

    if ((recd_p->pwrite >= RECD_GET_BLOCK * RECD_GET_COUNT) &&
        (STATE_DATAOK != recd_p->state)) {
        recd_p->state = STATE_DATAOK;
    }

    SCI_TRACE_LOW("\n[%s][%d] in %d %d %d %d\n",
        __FUNCTION__, SCI_GetTickCount(), i++,
        datalen, recd_p->pwrite, recd_p->vaild_size);
}


LOCAL uint32 Mic_Play_Start(
    uint8 enable,
    uint32 format,
    void* cb
)
{
    recd_strem_ctx_t* recd_p = &(audiotest_ctl.recd);
    RECORD_PARAM_T record_upload_param = {0};
    AUDIO_OBJECT_T* audio_obj = SCI_NULL;
    static uint32 cb_vector = NULL;
    uint32 param_result = 0;

    if (SCI_TRUE == enable) {
        if (cb == PNULL) {
            return SCI_FAILURE;
        }

        if(PNULL == recd_p->buf) {
            recd_p->buf = (uint8 *)SCI_ALLOC(RECD_BUF_SIZE);
            if(PNULL == recd_p->buf) {
                return SCI_FAILURE;
            }
            recd_p->size = RECD_BUF_SIZE;
            recd_p->vaild_size = RECD_BUF_SIZE;
        }

        recd_p->fs = RECD_SAMPLERATE;
        recd_p->ch = 1; // only support mono

        if (!recd_p->handle) {
            recd_p->handle = AUDIO_CreateStreamBufferHandle(
                                            hRecordCodec,
                                            PNULL,
                                            hAUDDEVRECORD,
                                            PNULL,
                                            PNULL,
                                            PNULL,
                                            PNULL,
                                            PNULL,
                                            record_dev_dummy_cb,
                                            record_dev_stream_cb);
        }
        if (!recd_p->handle) {
            SCI_TRACE_LOW("[AUTOTEST][%s][E] create handle!", __FUNCTION__);
            return SCI_FAILURE;
        }

        record_upload_param.format = AUD_REC_PCM;
        record_upload_param.output_channel = RECORD_OUTPUT_BUFFER;
        record_upload_param.samplerate = recd_p->fs;

        AUDIO_ExeCodecExtOpe(recd_p->handle,
                                ANSI2UINT16("SET_RECORDPARAM"),
                                RECORD_PARAM_SET_COUNT,
                                &record_upload_param,
                                &param_result);
        recd_p->pwrite = 0;

        if (NULL == cb_vector) {
            audio_obj = AUDIO_SM_GetAudioObject(recd_p->handle);
            if(NULL != audio_obj)
            {
                recd_p->pdev = AUDIO_DM_GetDeviceHandleStruct(audio_obj->hDevice);
                if (PNULL != recd_p->pdev) {
                    cb_vector =\
                        recd_p->pdev->tDeviceInfo.tAudioDevOpe.pRegRecCallback(\
                                        record_stream_cb);
                }
            }
        }

        AUDIO_SetOutputSampleRate(recd_p->handle, recd_p->fs);
        AUDIO_SetDevMode(AUDIO_DEVICE_MODE_HANDFREE);
        if (STATE_STOP == recd_p->state) {
            AUDIO_Play(recd_p->handle,0);
        }
        AUDIO_SetVolume(9);
    } else {
        if (cb_vector == PNULL) {
            return SCI_FAILURE;
        }
        recd_p->pdev->tDeviceInfo.tAudioDevOpe.pUnRegRecCallback(
                    cb_vector);
        cb_vector = PNULL;

        if ((STATE_STOP != recd_p->state) &&
            (NULL != recd_p->handle)) {
            AUDIO_Stop(recd_p->handle);
            recd_p->state = STATE_STOP;

            AUDIO_CloseHandle(recd_p->handle);
            recd_p->handle = NULL;
        }

        if (recd_p->buf) {
            SCI_FREE(recd_p->buf);
        }
        recd_p->buf = PNULL;
        recd_p->size = 0;
        recd_p->pwrite = 0;
        recd_p->pdev = PNULL;
    }
    return SCI_OK;
}

/*---------------------------------------------------------------------------*/
/*                          PUBLIC FUNCTION                                  */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : API of  AUDDEV play demo
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
PUBLIC uint32 Autotest_Speaker_Start(
    uint8 *srcdata,
    uint32 datalen,
    uint8 dev_mode,
    uint32 is_stereo
)
{
    play_strem_ctx_t* play_p = &(audiotest_ctl.play);

    if ((PNULL == srcdata) ||
        (datalen == 0) ||
        (dev_mode != 0)) {
        SCI_TRACE_LOW("[AUTOTEST][%s][E] state!", __FUNCTION__);
        return SCI_FAILURE;
    }

    if(PNULL == play_p->buf) {
        play_p->buf = (uint8 *)SCI_ALLOC(PLAY_BUF_SIZE);
        if(PNULL == play_p->buf) {
            SCI_TRACE_LOW("[AUTOTEST][%s][E] malloc!", __FUNCTION__);
            return SCI_FAILURE;
        }
        SCI_TRACE_LOW("[AUTOTEST][%s] alloc 0x%x", __FUNCTION__, play_p->buf);
        play_p->size = PLAY_BUF_SIZE;
    }

    Autotest_Databuf_Set(srcdata, datalen,
        play_p->buf, play_p->size);

    AUDIO_SetDevMode(AUDIO_DEVICE_MODE_HANDFREE);
    AUDIO_SetVolume(9);
    Speaker_Play_Open(is_stereo);
    Speaker_Play_Start();

    SCI_TRACE_LOW("[AUTOTEST][%s] success %d", __FUNCTION__, play_p->state);
    return SCI_OK;
}

PUBLIC uint32 Autotest_Speaker_Start_Local(uint8 dev_mode)
{
    play_strem_ctx_t* play_p = &(audiotest_ctl.play);

    if ((dev_mode != 0) ||
        (play_p->state != STATE_STOP)) {
        SCI_TRACE_LOW("[AUTOTEST][%s][E] state!", __FUNCTION__);
        return SCI_FAILURE;
    }

    if(PNULL == play_p->buf) {
        play_p->buf = (uint8 *)SCI_ALLOC(PLAY_BUF_SIZE);
        if(PNULL == play_p->buf) {
            SCI_TRACE_LOW("[AUTOTEST][%s][E] malloc!", __FUNCTION__);
            return SCI_FAILURE;
        }
        SCI_TRACE_LOW("[AUTOTEST][%s] alloc 0x%x", __FUNCTION__, play_p->buf);
        play_p->size = PLAY_BUF_SIZE;
    }

    Autotest_Databuf_Set((uint8*)sin1k_44k, 88,
        play_p->buf, play_p->size);

    AUDIO_SetDevMode(AUDIO_DEVICE_MODE_HANDFREE);
    AUDIO_SetVolume(9);
    Speaker_Play_Open(0);
    Speaker_Play_Start();

    SCI_TRACE_LOW("[AUTOTEST][%s] success", __FUNCTION__);
    return SCI_OK;
}

/*****************************************************************************/
//  Description : API of  AUDDEV stop demo
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
PUBLIC uint32 Autotest_Speaker_Stop(void)
{
    play_strem_ctx_t* play_p = &(audiotest_ctl.play);

    if ((STATE_STOP != play_p->state) &&
        (NULL != play_p->handle)) {
        AUDIO_Stop(play_p->handle);
        play_p->state = STATE_STOP;

        AUDIO_CloseHandle(play_p->handle);
        play_p->handle = NULL;
    }

    if (play_p->buf) {
        SCI_TRACE_LOW("[AUTOTEST][%s] free 0x%x", __FUNCTION__, play_p->buf);
        SCI_FREE(play_p->buf);
    }
    play_p->buf = PNULL;
    play_p->size = 0;

    SCI_TRACE_LOW("[AUTOTEST][%s] success", __FUNCTION__);

    return SCI_OK;
}

/*****************************************************************************/
//  Description : API of  hAUDDEVRECORD play demo
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
PUBLIC uint32 Autotest_Mic_Start(uint8 dev_mode)
{
    uint8 ret;
//    recd_strem_ctx_t* recd_p = &(audiotest_ctl.recd);

    if (dev_mode != AUDIO_DEVICE_MODE_HANDFREE) {
        return SCI_FAILURE;
    }
    if(PNULL == recd_get_buf)
        recd_get_buf = SCI_ALLOC(RECD_GET_BLOCK * RECD_GET_COUNT);
    if(PNULL == recd_get_buf) {
        SCI_TRACE_LOW("[AUTOTEST][%s][E] malloc!", __FUNCTION__);
        return SCI_FAILURE;
    }

    ret = Mic_Play_Start(SCI_TRUE, AUD_REC_PCM, record_stream_cb);
    if (SCI_FAILURE == ret) {
        SCI_TRACE_LOW("[AUTOTEST][%s][E] enable failed!", __FUNCTION__);
        return SCI_FAILURE;
    }
    return SCI_OK;
}

/*****************************************************************************/
//  Description : API of  hAUDDEVRECORD stop demo
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
PUBLIC uint32 Autotest_Mic_Stop(void)
{
    uint8 ret;

    ret = Mic_Play_Start(SCI_FALSE, AUD_REC_PCM, PNULL);
    if (SCI_FAILURE == ret) {
        SCI_TRACE_LOW("[AUTOTEST][%s][E] enable failed!", __FUNCTION__);
        return SCI_FAILURE;
    }
    if(PNULL != recd_get_buf)
        SCI_FREE(recd_get_buf);
    recd_get_buf = PNULL;
    return SCI_OK;
}


PUBLIC uint8 Autotest_GetMicState(void)
{
    recd_strem_ctx_t* recd_p = &(audiotest_ctl.recd);
    if (recd_p->state == STATE_DATAOK)
        return SCI_OK;
    else
        return SCI_FAILURE;
}

PUBLIC uint16* Autotest_GetDataAddr(uint32 index)
{
    recd_strem_ctx_t* recd_p = &(audiotest_ctl.recd);
    if (PNULL == recd_get_buf) {
        SCI_TRACE_LOW("[AUTOTEST][%s] success PNULL == recd_get_buf", __FUNCTION__);
        return PNULL;
    }
    if ((STATE_DATAOK == recd_p->state) && (RECD_GET_COUNT > index)) {
        if (index == 0) {
            uint32 block1, block2;

            // recd_get_buf get latest data from recd_p->buf
            block1 = block2 = RECD_GET_BLOCK * RECD_GET_COUNT;
            block1 = (block1 < recd_p->pwrite)?\
                        block1 : recd_p->pwrite;
            block2 = block2 - block1;

            if (0 < block2) {
                SCI_MEMCPY((uint8 *)(recd_get_buf),
                        (uint8 *)(recd_p->buf + recd_p->vaild_size - block2),
                        block2);
                SCI_MEMCPY((uint8 *)(recd_get_buf + block2),
                        (uint8 *)recd_p->buf,
                        block1);
            } else {
                SCI_MEMCPY((uint8 *)recd_get_buf,
                        (uint8 *)(recd_p->buf + recd_p->pwrite - block1),
                        block1);
            }
        }
        return (uint16 *)(&recd_get_buf[RECD_GET_BLOCK*index]);
    } else {
        return PNULL;
    }
}



