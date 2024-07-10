/*
 * <codec_param_nv.h> - <codec gain nv operation.>
 *
 * Copyright (c) 2019 Unisoc Communications Inc.
 * History
 *      <2022/03/15> <Xiaoyu.He> <create>
 *      codec nv operation.
 */

#ifndef _CODEC_PARAM_NV_H_
#define _CODEC_PARAM_NV_H_

#include "sci_types.h"
#include "audio_api.h"
#include "admm_III.h"


#ifdef __cplusplus
extern "C" {
#endif

//#define     NVID_AUD_CODEC_NV       0x27a
/// Number of audio gain steps.
#define     NV_AUDIO_GAINS_QTY      (16)


typedef enum {
    NV_AUDIO_CODEC_HANDSET_E,
    NV_AUDIO_CODEC_HANDSFREE_E,
    NV_AUDIO_CODEC_HEADSET_E,
    NV_AUDIO_CODEC_HEADSET_TVOUT_E,

    NV_AUDIO_CODEC_HANDSET_WB_E,
    NV_AUDIO_CODEC_HANDSFREE_WB_E,
    NV_AUDIO_CODEC_HEADSET_WB_E,
    NV_AUDIO_CODEC_HEADSET_TVOUT_WB_E,

    NV_AUDIO_CODEC_LOOPBHANDSFREE_E,
    NV_AUDIO_CODEC_LOOPBHANDSET_E,
    NV_AUDIO_CODEC_LOOPBHEADSET_E,
    NV_AUDIO_CODEC_LOOPBHEADSET_TVOUT_E,

    NV_AUDIO_CODEC_BTHS_E,
    NV_AUDIO_CODEC_BTHSNREC_E,
    NV_AUDIO_CODEC_BTHS_D16K_E,
    NV_AUDIO_CODEC_BTHSNREC_D16K_E,

    NV_AUDIO_CODEC_HANDSET_MUSIC_E,
    NV_AUDIO_CODEC_HANDSFREE_MUSIC_E,
    NV_AUDIO_CODEC_HEADSET_MUSIC_E,
    NV_AUDIO_CODEC_HEADFREE_MUSIC_E,

    NV_AUDIO_CODEC_MAX
}NV_AUDIO_CODEC_E_;

typedef uint32 NV_AUDIO_CODEC_E;

typedef struct
{
    /// Input analog gain.
    int16                           ana;                          //0x00000000
    /// Input ADC gain.
    int16                           adc;                          //0x00000002
} NV_AUDIO_IN_GAIN_T; //Size : 0x4

typedef struct
{
    /// Output analog gain.
    int16                           ana;                 //0x00000000
    /// Output DAC gain.
    int16                           dac;                 //0x00000001
    /// Output ALG gain.
    int16                           alg;                 //0x00000002
    /// Reserved.
    int16                           reserved;        //0x00000004
} NV_AUDIO_OUT_GAIN_T; //Size : 0x4

typedef struct
{
    NV_AUDIO_IN_GAIN_T        inGains;                // MIC gains during a voice call/RECORD.
    NV_AUDIO_OUT_GAIN_T     outGains[NV_AUDIO_GAINS_QTY]; // Output gains.
} NV_AUDIO_CODEC_GAIN_T; //Size : 0x54

typedef struct
{
    uint8 ucModeName[16];
    NV_AUDIO_CODEC_GAIN_T CODEC_param_NV;
}CODEC_PARAM_NV_T;


void CODEC_CreatParamList(void);

char *CODEC_GetDevModeNameByIndex(int32 index);

void CODEC_RefreshParamList(void);

char *CODEC_PARAM_GetNameByIndex(int32 index);

int32 CODEC_PARAM_GetParamFromMemByName(
    char *name_ptr,
    CODEC_PARAM_NV_T *codec_param_nv_ptr);

int32 CODEC_PARAM_GetParamFromMemByIndex(
    int32 index,
    CODEC_PARAM_NV_T *codec_param_nv_ptr);

int32 CODEC_PARAM_SetParamToMem(
    CODEC_PARAM_NV_T *codec_param_nv_ptr);

int32 CODEC_PARAM_GetParamFromFlashByName(
    char *name_ptr,
    CODEC_PARAM_NV_T *codec_param_nv_ptr);

int32 CODEC_PARAM_GetParamFromFlashByIndex(
    int32 index,
    CODEC_PARAM_NV_T *codec_param_nv_ptr);

int32 CODEC_PARAM_SetParamToFlash(
    CODEC_PARAM_NV_T *codec_param_nv_ptr);

char * CODEC_PARAM_GetDevModeNameById(
    NV_AUDIO_CODEC_E codec_mode_e);

#ifdef __cplusplus
}
#endif

#endif
