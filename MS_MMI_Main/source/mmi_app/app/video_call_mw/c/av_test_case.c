/*************************************************************
 * File  :  avc_platform_w217.c
 * Module: RTSA call C vendor component wrapper.
 *
 * This is a part of the RTSA call Service SDK.
 * Copyright (C) 2020 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#include "avc_platform.h"
#include "avc_ringbuf.h"

#include "dal_jpeg.h"
#include "jpeg_interface.h"
#include "isp_drv_uws6121e.h"
#include "dal_dcamera.h"
#include "video_display.h"
#include "dal_img.h"
#include "dal_recorder.h"

#include "mmi_app_eng_trc.h"
#include "std_header.h"
#include "sfs.h"
#include "ffs.h"
#include "dsp_codec_adp.h"
#include "dal_audio.h"
#include "aud_gen.h"
#include "audio_api.h"
#include "asm.h"
#include "audio_config.h"
#include "wav_adp.h"
#include "g722.h"
#include "dal_lcd.h"
#include "mmk_type.h"
#include "guiimg.h"
#include "guilcd.h"
#include "ref_lcd.h"
#include "tjpgd.h"
#include "fps_cc.h"
#include "voip_adp.h"

#define TAG "[AV-TEST]"

typedef struct {
    ringbuf_handle_t ringbuf_audio_handle;
} avc_platform_t;

static avc_platform_t g_avc_platform = {0};

#define PCM_RECORD_BUFFER_SIZE    (320 * 16)
#define AUDIO_DATA_EXCHANGE_VOIP  (1)

static bool __is_memory_lack_threshold_trigger(void);
static HAUDIO hvoiphandle = INVALID_HANDLE;
extern HAUDIODEV hAUDDEVDUAL;
extern HAUDIOCODEC hVoipCodec;
static void Voip_CapCallBack(uint16 *pSrc, uint32 *uiLength, void *param)
{
    LOGT(TAG, "[VideoCallAudio] record callback. addr=%x, len=%d", pSrc, *uiLength);
    assert(pSrc && uiLength);

    __is_memory_lack_threshold_trigger();

    if (-1 == avc_ringbuf_write(g_avc_platform.ringbuf_audio_handle, pSrc, *uiLength)) {
        LOGE(TAG, "audio helper buffer overrun");
    }
}

static void Voip_PlayCallBack(uint16 *pDest, uint32 *puiLength, void *param)
{
    uint8 pcm_data[320];

    LOGT(TAG, "Addr=0x%x, len=%u", pDest, *puiLength);

    if (-1 == avc_ringbuf_read(pcm_data, sizeof(pcm_data), g_avc_platform.ringbuf_audio_handle)) {
        memset(pcm_data, 0, sizeof(pcm_data));
    }

    memcpy(pDest, pcm_data, sizeof(pcm_data));
}

static void AudioDummyNotifyCallback(HAUDIO hAudio, uint32 notify_info, uint32 affix_info)
{}

static int voip_init()
{
    VOIP_EXT_OPER_STOP_PARA_T tStopPara = {0};
    uint32 temp;

    hvoiphandle = AUDIO_CreateNormalHandle(hVoipCodec, SCI_NULL, hAUDDEVDUAL, SCI_NULL, AudioDummyNotifyCallback);
    if (INVALID_HANDLE == hvoiphandle) {
        LOGE(TAG, "voip create handle failed!");
        return -1;
    }

    LOGT(TAG, "aud_voip:voip init 0");
    return 0;
}

static void voip_start(void)
{
    VOIP_EXT_OPER_START_PARA_T tPara = {0};
    uint32 aud_result;
    int32 err;

    if (hvoiphandle != INVALID_HANDLE) {
        LOGW(TAG, "voip started. duplicated process, ignore");
        return;
    }

    voip_init();
    if (!hvoiphandle) {
        LOGT(TAG, "hvoiphandle is NULL");
        assert(hvoiphandle);
        return;
    }

    AUD_EnableVoiceProcessConfig(SCI_TRUE, SCI_FALSE, PNULL);

    tPara.hAudioHandle = hvoiphandle;
    tPara.para0 = AUDIO_DATA_EXCHANGE_VOIP;
    tPara.para1 = 8000;
    tPara.para2 = 160;
    tPara.para3 = AUDIO_MODE_APP_TYPE_VOIP_1;
    tPara.readdataClkFunc = Voip_PlayCallBack;
    tPara.writedataClkFunc = Voip_CapCallBack;
    tPara.readparam = 0;
    tPara.writeparam= 0;
    err = AUDIO_ExeCodecExtOpe(//relative to dsp
                               tPara.hAudioHandle,
                               ANSI2UINT16("VOIP_EXT_START"),
                               4,
                               &tPara,
                               &aud_result);

    LOGT(TAG, "aud_voip:voip start 1 Sa=%d", tPara.para1);
    assert(err == 0);

    AUDIO_SetVolume(9);
    AUDIO_SetDevMode(AUDIO_DEVICE_MODE_HANDFREE);
}

static void voip_stop(void)
{
    VOIP_EXT_OPER_STOP_PARA_T tStopPara = {0};
    uint32 aud_result;

    tStopPara.hAudioHandle = hvoiphandle;
    tStopPara.para0 = AUDIO_DATA_EXCHANGE_VOIP;
    if (!hvoiphandle) {
        LOGE(TAG, "hvoiphandle null");
        return;
    }

    AUDIO_ExeCodecExtOpe(tStopPara.hAudioHandle, ANSI2UINT16("VOIP_EXT_STOP"), 0, &tStopPara, &aud_result);
    AUD_EnableVoiceProcessConfig(SCI_FALSE, SCI_FALSE, PNULL);
    AUDIO_CloseHandle(hvoiphandle);
    hvoiphandle = 0;
    LOGT(TAG, "aud_voip:voip stop");
}

#define MIN_FREE_MEMORY_SIZE (1024 * 64)
static bool __is_memory_lack_threshold_trigger(void)
{
    int memory_free = SCI_GetSystemSpaceTotalAvalidMem();
    bool lack = (memory_free <= MIN_FREE_MEMORY_SIZE);
    if (lack) {
        LOGW(TAG, "[MEM] lack memory[%d]", memory_free);
    } else {
        LOGT(TAG, "[MEM] free memory=%d", memory_free);
    }
    return lack;
}

static int __raw_yuv422_uyvy_capture_callback(void *frame_ptr)
{
    ISP_FRAME_T *pframe = (ISP_FRAME_T *)frame_ptr;

    LOGD(TAG, "Get ISP Frame. y=0x%x,u=0x%x,v=0x%x,w=%d,h=%d,type=%d",
         pframe->yaddr, pframe->uaddr, pframe->vaddr, pframe->width, pframe->height, pframe->type);

    __is_memory_lack_threshold_trigger();

    return 0;
}

static void __start_capture_video_data()
{
    DCAMERA_PREVIEW_PARAM_T param;
    int rval;
    memset(&param, 0, sizeof(param));

    param.capture_mode    = 0;
    param.dc_atv_callback = NULL;
    param.disp_mode       = 0x01;
    param.lcd_block_id    = 0;
    param.lcd_id          = 0;
    param.rotation_mode   = 0;
    param.panorama_dir    = 0;

    param.target_rect.x   = 0;
    param.target_rect.y   = 0;
    param.target_rect.w   = 128;
    param.target_rect.h   = 128;

    param.disp_rect.x     = 0;
    param.disp_rect.y     = 0;
    param.disp_rect.w     = 128;
    param.disp_rect.h     = 128;

    rval = DCAMERA_Open(DCAMERA_OP_VIDEOCALL);
    LOGD(TAG, "open camera. rval=%d", rval);
    assert(DCAMERA_OP_SUCCESS == rval);

    rval = DCAMERA_RigisterVideoChatCallback(__raw_yuv422_uyvy_capture_callback);
    LOGD(TAG, "yuv callback. rval=%d", rval);
    assert(DCAMERA_OP_SUCCESS == rval);

    rval = DCAMERA_StartPreview(&param);
    LOGD(TAG, "start preview. rval=%d", rval);
    assert(DCAMERA_OP_SUCCESS == rval);
}

static void __capture_video_stop(void)
{
    int rval;
    LOGD(TAG, "platform stop capture video");
    rval = DCAMERA_StopPreview();
    LOGT(TAG, "DCAMERA_StopPreview err=%d", rval);
    assert(rval == 0);

    rval = DCAMERA_UnRigisterVideoChatCallback();
    LOGT(TAG, "DCAMERA_UnRigisterVideoChatCallback err=%d", rval);
    assert(rval == 0);

    rval = DCAMERA_Close();
    LOGT(TAG, "DCAMERA_Close err=%d", rval);
    assert(rval == 0);
}

static void __audio_ringbuf_create()
{
    g_avc_platform.ringbuf_audio_handle = avc_ringbuf_create(32 * 1024 * 2);//申请64K的缓存，用于存放采集到的audio数据
    assert(g_avc_platform.ringbuf_audio_handle);
}

static void __audio_ringbuf_destroy()
{
    if (g_avc_platform.ringbuf_audio_handle) {
        avc_ringbuf_destroy(g_avc_platform.ringbuf_audio_handle);
        g_avc_platform.ringbuf_audio_handle = NULL;
    }
}

void av_test_case_start()
{
    LOGT(TAG, "test case start 1");
    __audio_ringbuf_create();
    LOGT(TAG, "test case start 2");
    voip_start();
    LOGT(TAG, "test case start 3");
    __start_capture_video_data();
    LOGT(TAG, "test case start 4");
}

void av_test_case_stop()
{
    LOGT(TAG, "test case stop 1");
    voip_stop();
    LOGT(TAG, "test case stop 2");
    __capture_video_stop();
    LOGT(TAG, "test case stop 3");
    __audio_ringbuf_destroy();
    LOGT(TAG, "test case stop 4");
}