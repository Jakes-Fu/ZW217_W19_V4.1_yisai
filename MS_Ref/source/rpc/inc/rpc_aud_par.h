/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

// AUTO GENERATED

#define TAG_aud_StreamStart 0x20f7d018
struct IPAR_aud_StreamStart
{
    SND_ITF_T itf;
    HAL_AIF_STREAM_T stream;
    AUD_LEVEL_T cfg;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_aud_StreamStart));

struct OPAR_aud_StreamStart
{
    AUD_ERR_T result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_aud_StreamStart));

#define TAG_aud_StreamStop 0x1c29ffec
struct IPAR_aud_StreamStop
{
    SND_ITF_T itf;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_aud_StreamStop));

struct OPAR_aud_StreamStop
{
    AUD_ERR_T result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_aud_StreamStop));

#define TAG_aud_Setup 0x940784bc
struct IPAR_aud_Setup
{
    SND_ITF_T itf;
    AUD_LEVEL_T cfg;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_aud_Setup));

struct OPAR_aud_Setup
{
    AUD_ERR_T result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_aud_Setup));

#define TAG_aud_StreamRecord 0xafcd013d
struct IPAR_aud_StreamRecord
{
    SND_ITF_T itf;
    HAL_AIF_STREAM_T stream;
    AUD_LEVEL_T cfg;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_aud_StreamRecord));

struct OPAR_aud_StreamRecord
{
    AUD_ERR_T result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_aud_StreamRecord));

#define TAG_aud_StreamRecordStop 0x440eda14
struct IPAR_aud_StreamRecordStop
{
    SND_ITF_T itf;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_aud_StreamRecordStop));

struct OPAR_aud_StreamRecordStop
{
    AUD_ERR_T result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_aud_StreamRecordStop));

#define TAG_hal_zspMusicPlayStart 0xce8bf0bd
struct OPAR_hal_zspMusicPlayStart
{
    HAL_ERR_T result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_hal_zspMusicPlayStart));

#define TAG_hal_zspMusicPlayStop 0x2718f091
struct OPAR_hal_zspMusicPlayStop
{
    HAL_ERR_T result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_hal_zspMusicPlayStop));

#define TAG_aud_SetAudDeviceCFG 0x1b0b9a67
struct IPAR_aud_SetAudDeviceCFG
{
    AUD_DEVICE_CFG_EXT_T cfg;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_aud_SetAudDeviceCFG));

#define TAG_vois_SetCfg 0xe9d0fbdf
struct IPAR_vois_SetCfg
{
    AUD_ITF_T itf;
    VOIS_AUDIO_CFG_T cfg;
    AUD_DEVICE_CFG_EXT_T devicecfg;
    SND_BT_WORK_MODE_T btworkmode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_vois_SetCfg));

struct OPAR_vois_SetCfg
{
    VOIS_ERR_T result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_vois_SetCfg));

#define TAG_vois_SetBtNRECFlag 0xa4d73f30
struct IPAR_vois_SetBtNRECFlag
{
    UINT32 flag;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_vois_SetBtNRECFlag));

#define TAG_vois_TestModeSetup 0x97137f27
struct IPAR_vois_TestModeSetup
{
    SND_ITF_T itf;
    HAL_AIF_STREAM_T stream;
    AUD_LEVEL_T cfg;
    AUD_TEST_T mode;
    UINT32 voismode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_vois_TestModeSetup));

struct OPAR_vois_TestModeSetup
{
    AUD_ERR_T result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_vois_TestModeSetup));

#define TAG_DM_StartAudioEx 0x37317995
struct OPAR_DM_StartAudioEx
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DM_StartAudioEx));

#define TAG_DSPExt_GetVoiceCoderType 0xc222793e
struct OPAR_DSPExt_GetVoiceCoderType
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DSPExt_GetVoiceCoderType));

#define TAG_DM_RestartAudioEx 0x77d483d6
struct OPAR_DM_RestartAudioEx
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DM_RestartAudioEx));

#define TAG_DM_StopAudioEx 0x9cfd69ae
struct OPAR_DM_StopAudioEx
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DM_StopAudioEx));

#define TAG_DM_PlayToneEx 0x742087f6
struct IPAR_DM_PlayToneEx
{
    UINT8 nToneType;
    UINT8 nToneAttenuation;
    UINT16 nDuration;
    DM_SpeakerGain volumn;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_DM_PlayToneEx));

struct OPAR_DM_PlayToneEx
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DM_PlayToneEx));

#define TAG_DM_StopToneEx 0xbb249b21
struct OPAR_DM_StopToneEx
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DM_StopToneEx));

#define TAG_hal_zspVoiceRecordStart 0x1ecdf88b
struct OPAR_hal_zspVoiceRecordStart
{
    HAL_ERR_T result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_hal_zspVoiceRecordStart));

#define TAG_hal_zspVoiceRecordStop 0xa043759e
struct OPAR_hal_zspVoiceRecordStop
{
    HAL_ERR_T result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_hal_zspVoiceRecordStop));

#define TAG_aud_SetBtClk 0x3e50af7d
struct OPAR_aud_SetBtClk
{
    AUD_ERR_T result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_aud_SetBtClk));

#define TAG_aud_ClrBtClk 0xf514b639
struct OPAR_aud_ClrBtClk
{
    AUD_ERR_T result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_aud_ClrBtClk));

#define TAG_DM_AudStreamStart 0xc82b02a0
struct IPAR_DM_AudStreamStart
{
    SND_ITF_T itf;
    HAL_AIF_STREAM_T stream;
    AUD_LEVEL_T cfg;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_DM_AudStreamStart));

struct OPAR_DM_AudStreamStart
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DM_AudStreamStart));

#define TAG_DM_AudStreamStop 0x5e757a68
struct IPAR_DM_AudStreamStop
{
    SND_ITF_T itf;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_DM_AudStreamStop));

struct OPAR_DM_AudStreamStop
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DM_AudStreamStop));

#define TAG_DM_AudSetup 0xfaa5faa3
struct IPAR_DM_AudSetup
{
    SND_ITF_T itf;
    AUD_LEVEL_T cfg;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_DM_AudSetup));

struct OPAR_DM_AudSetup
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DM_AudSetup));

#define TAG_DM_AudStreamRecord 0xd62f0efc
struct IPAR_DM_AudStreamRecord
{
    SND_ITF_T itf;
    HAL_AIF_STREAM_T stream;
    AUD_LEVEL_T cfg;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_DM_AudStreamRecord));

struct OPAR_DM_AudStreamRecord
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DM_AudStreamRecord));

#define TAG_DM_VoisSetCfg 0x6b3f93ca
struct IPAR_DM_VoisSetCfg
{
    AUD_ITF_T itf;
    VOIS_AUDIO_CFG_T cfg;
    AUD_DEVICE_CFG_EXT_T devicecfg;
    SND_BT_WORK_MODE_T btworkmode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_DM_VoisSetCfg));

struct OPAR_DM_VoisSetCfg
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DM_VoisSetCfg));

#define TAG_DM_AudSetAudDeviceCFG 0xbdea98d9
struct IPAR_DM_AudSetAudDeviceCFG
{
    AUD_DEVICE_CFG_EXT_T cfg;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_DM_AudSetAudDeviceCFG));

struct OPAR_DM_AudSetAudDeviceCFG
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DM_AudSetAudDeviceCFG));

#define TAG_DM_VoisSetBtNRECFlag 0xfaea4225
struct IPAR_DM_VoisSetBtNRECFlag
{
    UINT32 flag;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_DM_VoisSetBtNRECFlag));

struct OPAR_DM_VoisSetBtNRECFlag
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DM_VoisSetBtNRECFlag));

#define TAG_DM_VoisTestModeSetup 0x1f954906
struct IPAR_DM_VoisTestModeSetup
{
    SND_ITF_T itf;
    HAL_AIF_STREAM_T stream;
    AUD_LEVEL_T cfg;
    AUD_TEST_T mode;
    UINT32 voismode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_DM_VoisTestModeSetup));

struct OPAR_DM_VoisTestModeSetup
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DM_VoisTestModeSetup));

#define TAG_aud_HeadsetConfig 0x43c78c4d
struct IPAR_aud_HeadsetConfig
{
    UINT32 isInsert;
    UINT32 isBbat;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_aud_HeadsetConfig));

struct OPAR_aud_HeadsetConfig
{
    AUD_ERR_T result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_aud_HeadsetConfig));

#define TAG_DM_ZspMusicPlayStart 0x3fe7d300
struct OPAR_DM_ZspMusicPlayStart
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DM_ZspMusicPlayStart));

#define TAG_DM_ZspMusicPlayStop 0x4859dfbc
struct OPAR_DM_ZspMusicPlayStop
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DM_ZspMusicPlayStop));

#define TAG_aud_SetSharkingMode 0xd5432923
struct IPAR_aud_SetSharkingMode
{
    UINT32 isSharkingMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_aud_SetSharkingMode));

#define TAG_aud_bbatPcmBufferPlayStart 0x4b09eff6
struct IPAR_aud_bbatPcmBufferPlayStart
{
    SND_ITF_T itf;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_aud_bbatPcmBufferPlayStart));

struct OPAR_aud_bbatPcmBufferPlayStart
{
    AUD_ERR_T result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_aud_bbatPcmBufferPlayStart));

#define TAG_aud_bbatPcmBufferPlayStop 0x4889e3fb
struct IPAR_aud_bbatPcmBufferPlayStop
{
    SND_ITF_T itf;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_aud_bbatPcmBufferPlayStop));

struct OPAR_aud_bbatPcmBufferPlayStop
{
    AUD_ERR_T result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_aud_bbatPcmBufferPlayStop));

#define TAG_DM_AudStreamRecordStop 0xe2469570
struct IPAR_DM_AudStreamRecordStop
{
    SND_ITF_T itf;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_DM_AudStreamRecordStop));

struct OPAR_DM_AudStreamRecordStop
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DM_AudStreamRecordStop));

#define TAG_DM_AudSetSharkingMode 0x4c6aff6a
struct IPAR_DM_AudSetSharkingMode
{
    UINT32 isSharkingMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_DM_AudSetSharkingMode));

#define TAG_DM_ZspVoiceVibrateStart 0x4c8dde44
struct IPAR_DM_ZspVoiceVibrateStart
{
    UINT32 time;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_DM_ZspVoiceVibrateStart));

struct OPAR_DM_ZspVoiceVibrateStart
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DM_ZspVoiceVibrateStart));

#define TAG_DM_ZspVoiceVibrateStop 0xbf76903d
struct OPAR_DM_ZspVoiceVibrateStop
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DM_ZspVoiceVibrateStop));

#define TAG_DM_ZspMusicVibrateStart 0xef602d9c
struct IPAR_DM_ZspMusicVibrateStart
{
    UINT32 time;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_DM_ZspMusicVibrateStart));

struct OPAR_DM_ZspMusicVibrateStart
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DM_ZspMusicVibrateStart));

#define TAG_DM_ZspMusicVibrateStop 0x2027253a
struct OPAR_DM_ZspMusicVibrateStop
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DM_ZspMusicVibrateStop));

#define TAG_DM_ZspMusicPlayStartEx 0xf3e17fe9
struct IPAR_DM_ZspMusicPlayStartEx
{
    AUD_AP2CP_PARA_T Aud_Para;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_DM_ZspMusicPlayStartEx));

struct OPAR_DM_ZspMusicPlayStartEx
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DM_ZspMusicPlayStartEx));

#define TAG_DM_ZspMusicPlayStopEx 0x7c7b3493
struct IPAR_DM_ZspMusicPlayStopEx
{
    AUD_AP2CP_PARA_T Aud_Para;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_DM_ZspMusicPlayStopEx));

struct OPAR_DM_ZspMusicPlayStopEx
{
    INT32 result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_DM_ZspMusicPlayStopEx));

#define TAG_DM_AudSetToneMode 0x91f12bb2
struct IPAR_DM_AudSetToneMode
{
    UINT32 isToneMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_DM_AudSetToneMode));

#define TAG_VTSP_startVolteCodec 0x6f760e1d

#define TAG_VTSP_stopVolteCodec 0x8f4c763c

// AUTO GENERATED END
