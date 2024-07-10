/*
* <dspdata_codec_adp.h>
*
* Copyright (c) 2022 Unisoc Communications Inc.
* History
*      <2022/10/1> <jen li> <create.>
*      This file defines the basic operation interfaces of DSPDATA codec.
*/

#ifndef _VOIP_ADP_H
#define _VOIP_ADP_H

#include "apm_codec.h"
#include "asm.h"
#include "audio_config.h"
#include "voice_process.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Para0 indicates the type of the data to be exchanged,
 * see defination of AUDIO_DATA_EXCHANGE_SUBCMD_ID_E.
 * Each type of data is saved in a separate buffer in audio sharememory so para0
 * is also used as the buffer index.
 */
typedef struct {
    HAUDIO hAudioHandle;
    HAUDIODEV HAudiodev;
    VOICE_READ_NETDATA_FUNC readdataClkFunc;
    void *readparam;
    VOICE_WRITE_NETDATA_FUNC writedataClkFunc;
    void *writeparam;
    uint16 para0;/*the type of the data to be exchanged, see defination of AUDIO_DATA_EXCHANGE_SUBCMD_ID_E.*/
    uint16 para1;
    uint16 para2;
    uint16 para3;
    void *other;
} VOIP_EXT_OPER_START_PARA_T;

typedef struct {
    HAUDIO hAudioHandle;
    HAUDIODEV HAudiodev;
    uint16 para0;
    uint16 para1;
    uint16 para2;
    uint16 para3;
} VOIP_EXT_OPER_STOP_PARA_T;

typedef struct {
    HAUDIO hAudioHandle;
    uint32 sampleRate; /* 0:8000; 1:16000; 2:32000; 3:48000 */
    uint32 uiformat; /*3--amr_nb, 8--amr_wb, 11--pcm,14--evs_primary,15--evs_amrwb_io*/
    uint32 uiFrameTypeIndex;/*0: 4.75kbps_for_amrnb/6.6kbps_for_amrwb*/

} VOIP_EXT_OPER_PARA_SET_T;

typedef struct {
    HAUDIO hAudioHandle;
    HAUDIODEV HAudiodev;
    uint32 para0;
    uint32 para1;
    uint32 para2;
} VOIP_EXT_OPER_COMMON_T;

/**
 * This Function is used to initialize the codec.
 */
PUBLIC HAUDIOCODEC VOIP_RegCodecPlugger (
    HCOLLISION hCollision,
    AUDIO_CODEC_EXT_CFGINFO_T *ptCodecExtCfgInfo);

//VOIP demo
/*
#ifdef VOIP_SUPPORT
LOCAL HAUDIO hvoiphandle = INVALID_HANDLE;
LOCAL int16 *sVoipBuffer = PNULL;
static void Voip_CapCallBack(uint16 *pSrc, uint32 *uiLength, void *param)
{
    SCI_TRACE_LOW("%s uiLength=%d", __func__, *uiLength);
    SCI_MEMCPY(sVoipBuffer, pSrc, *uiLength);
}

static void Voip_PlayCallBack(uint16 *pDest, uint32 *puiLength, void *param)
{
    DSP_CODEC_EXT_OPE_VOICE_DATA_T ptPara;

    SCI_TRACE_LOW("%s puiLength=%d", __func__, *puiLength);
    SCI_MEMCPY(pDest, sVoipBuffer, *puiLength);
}

static int voip_init()
{
    VOIP_EXT_OPER_STOP_PARA_T tStopPara = {0};
    uint32 temp;

    hvoiphandle = AUDIO_CreateNormalHandle(
                    hVoipCodec,
                    SCI_NULL,
                    hAUDDEVDUAL,
                    SCI_NULL,
                    AudioDummyNotifyCallback
                    );
    if( INVALID_HANDLE == hvoiphandle)
    {
        SCI_TRACE_LOW("voip create handle failed!");
        return -1;
    }
    SCI_TRACE_LOW("aud_voip:voip init 0");
    return 0;
}

void voip_start(void)
{
    VOIP_EXT_OPER_START_PARA_T tPara = {0};
    uint32 aud_result;
    voip_init();
    if (!hvoiphandle) {
        SCI_TRACE_LOW("%s hvoiphandle is NULL", __func__);
        return;
    }

    AUD_EnableVoiceProcessConfig(SCI_TRUE, SCI_FALSE, PNULL);

    sVoipBuffer = (int16 *)SCI_ALLOC(1024);
    tPara.hAudioHandle = hvoiphandle;
    tPara.para0 = AUDIO_DATA_EXCHANGE_VOIP;
    tPara.para1 = 8000;
    tPara.para2 = 160;
    tPara.para3 = AUDIO_MODE_APP_TYPE_VOIP_1;
    tPara.readdataClkFunc = Voip_PlayCallBack;
    tPara.writedataClkFunc = Voip_CapCallBack;
    tPara.readparam = 0;
    tPara.writeparam= 0;
    AUDIO_ExeCodecExtOpe(//relative to dsp
        tPara.hAudioHandle,
        ANSI2UINT16("VOIP_EXT_START"),
        4,
        &tPara,
        &aud_result
        );


    SCI_TRACE_LOW("aud_voip:voip start");
}

void voip_stop(void)
{
    VOIP_EXT_OPER_STOP_PARA_T tStopPara = {0};
    uint32 aud_result;

    tStopPara.hAudioHandle = hvoiphandle;
    tStopPara.para0 = AUDIO_DATA_EXCHANGE_VOIP;
    if (!hvoiphandle) {
        SCI_TRACE_LOW("%s hvoiphandle is NULL", __func__);
        return;
    }

    AUDIO_ExeCodecExtOpe(
        tStopPara.hAudioHandle,
        ANSI2UINT16("VOIP_EXT_STOP"),
        0,
        &tStopPara,
        &aud_result
        );

    AUD_EnableVoiceProcessConfig(SCI_FALSE, SCI_FALSE, PNULL);

    AUDIO_CloseHandle(hvoiphandle);
    hvoiphandle = 0;
    SCI_FREE(sVoipBuffer);
    sVoipBuffer = PNULL;

    SCI_TRACE_LOW("aud_voip:voip stop");
}

static uint32 voiploopback_playing = 0;
void voip_loop_test(void)
{
    if(0 == voiploopback_playing){
        voip_init();
        voip_start();
        voiploopback_playing = 1;
    }else {
        voip_stop();
        voiploopback_playing = 0;
    }
}

LOCAL void _AUDIO_Test_thread (uint32 argc, void *argv)
{
    SCI_TRACE_LOW("_AUDIO_Test_thread\n");
    //SCI_Sleep(10000);
    //Datadump_Enable(1,0xffff);
    voip_loop_test();
    //record_test();
    //tone_play();

    //Loop_Start_Local();
    SCI_TRACE_LOW("_AUDIO_Test_thread out\n");
}

#define AUDIO_TEST_TASK_NAME  "AUDIO_TEST_TASK"
#define AUDIO_TEST_QNAME_NAME  "Q_AUDIO_TEST_TASK"
#define AUDIO_TEST_STACK_SIZE (2*1024)
#define AUDIO_TEST_QUEUE_NUM  (2*10)
//#define AUDIO_TEST_TASK_PRIV  (SCI_PRIORITY_NORMAL)  //maybe use high pirv(SMALL VAL).
#define AUDIO_TEST_TASK_PRIV  (20)

PUBLIC void audio_test(void)
{
voip_loop_test();
WDG_TimerStop();
#if 0
    BLOCK_ID audio_test_task_id;

    SCI_TRACE_LOW("audio_pcm_play_test\n");
    audio_test_task_id  = SCI_CreateThread(AUDIO_TEST_TASK_NAME,
                            AUDIO_TEST_QNAME_NAME,
                            _AUDIO_Test_thread,
                            0,
                            0,
                            AUDIO_TEST_STACK_SIZE,
                            AUDIO_TEST_QUEUE_NUM,
                            AUDIO_TEST_TASK_PRIV,
                            SCI_PREEMPT,
                            SCI_AUTO_START);

#endif
}
#endif
*/
#ifdef __cplusplus
}
#endif

#endif //end of _VOIP_ADP_H

//end of file
