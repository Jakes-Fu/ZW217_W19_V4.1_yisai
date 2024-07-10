/******************************************************************************
 ** File Name:      armvb_as.c                                                                *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           01/07/2006                                                *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    Voiceband(ARM Controlled) audioservice adapter layer.*
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/07/2006     Benjamin.Wang    Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "audio_config.h"

#include "adm.h"
#include "armvb_as.h"
#include "audio_api.h"
#include "aud_gen.h"
#include "audio_nv_arm.h"
#include "admm_III.h"
#include "audio_drvapi.h"
#include "track_mixer.h"

#include "audio_drvapi.h"
#include "audio_hal.h"
#include "voice_process.h"
#include "codec_param_nv.h"

#ifdef CVSRCD_ENABLE
#include "record_process.h"
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
#ifndef __arm
#ifndef __MODULE__
#define __MODULE__ "armvb_as.c"
#endif
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#define VB_MAX_DA_VALUE (32767)
#define VB_MIN_DA_VALUE (-32768)
#define TMP_FIFO_SIZE         (160)    //half-word (8*1024/4)//
#define TMP_FIFO_SIZE_MAX    (160*6)  //half-word (7*1024)//
#define VB_CAPTURE_TRANS_SIZE    (1*TMP_FIFO_SIZE)
#define MULTITRACK_BUFFER_SIZE  (4*1024)


/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef struct
{
    uint32 pRead;            /*!<hw read pointer.*/
    uint32 pWrite;           /*!<device write pointer.*/
    uint32 size;             /*!<Total data count.*/
    int16* data_ptr;         /*!<Left channel buffer pointer.*/
    int16* data_align_ptr;   /*!<Left channel buffer pointer.*/
}AUDDEV_PLAYBACK_BUF;

typedef struct {
    uint16* data_ptr;
    uint16* data_align_ptr;  /*!<Left channel buffer pointer.*/
    uint32 size;
    uint32 pRead;            /*!<device read pointer.*/
    uint32 pWrite;           /*!<hw write pointer.*/
    uint16* tmp_buf;           /*!tmp buffer for stereo to mono*/
}AUDDEV_RECORD_BUF;

typedef struct
{
    uint32 dg_level;
    uint32 samplerate;
    uint32 chanelnum;
    uint32 pathdevice;
    uint32 mute;
    uint32 codec_gain1;
    uint32 codec_gain2;
    uint32 volume;
    uint32 app_type;
}AUDDEV_PLAYBACK;


/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
//lint -e551

AUDDEV_PLAYBACK s_auddev_playinfo;
AUDDEV_PLAYBACK s_auddev_recordinfo;

LOCAL AUDIO_DEVICE_HANDLE_T *g_armvb_device_ptr;

AUDDEV_PLAYBACK_BUF s_armvb_output_buf;
int32 *s_vb_da0_buffer_multitrack = NULL;  //[MULTITRACK_BUFFER_SIZE];
int32 *s_vb_da1_buffer_multitrack = NULL;  //[MULTITRACK_BUFFER_SIZE];
volatile BOOLEAN  s_is_vb_mute = SCI_FALSE;
LOCAL volatile BOOLEAN s_is_vb_stop = SCI_FALSE;
#define AUDDEV_RECORD_OUTPUT_COUNT_MAX 2
BOOLEAN s_b_armvb_recording = SCI_FALSE;
BOOLEAN s_auddev_dual_is_opened = SCI_FALSE;
volatile uint32 s_record_callback_count = 0;
uint32 s_ad_read_size = 0;
LOCAL uint32 s_armvb_hw_per_read = 0;   //half-word
AUDDEV_RECORD_BUF s_vb_arm_record_buf;
LOCAL volatile ADM_RECCALLBACK s_armrecord_callback[AUDDEV_RECORD_OUTPUT_COUNT_MAX] = {0};  // default NULL
AUDIO_CH_E g_channelNum = AUDIO_CH_MONO;
LOCAL volatile ADM_RECCALLBACK s_uplink_callback = 0;
LOCAL volatile ADM_PLAYCALLBACK s_downlink_callback = 0;
SCI_MUTEX_PTR  armvb_mutex = NULL;


#define COF_VALUE (1024)

#define RECORD_MUTE_TIME 100
#define RECORD_FADEIN_TIME 150
typedef struct
{
    uint32 mute_samples;
    uint32 fadein_samples;
    uint32 process_samples;
    uint32 ch;
    uint32 started;
}AUDDEV_FADEIN_CTL_T;
static AUDDEV_FADEIN_CTL_T s_fadein_ctl;

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
#define ALIGN_UP_WITH_32(d) (((uint32)d + 0x3F) & ~(uint32)0x3F);

/**---------------------------------------------------------------------------*
 **                         Local Function Definitions                              *
 **---------------------------------------------------------------------------*/
uint32 AUDDEV_Mode2Dev(AUDIO_DEVICE_MODE_TYPE_E eMode)
{
    uint32 retdev = AUDIO_HP;

    switch(eMode)
    {
        case      AUDIO_DEVICE_MODE_HANDHOLD:
        retdev = AUDIO_EAR;
        break;
        case      AUDIO_DEVICE_MODE_HANDFREE:
        retdev = AUDIO_SPK;
        break;
        case      AUDIO_DEVICE_MODE_EARPHONE:
        retdev = AUDIO_HP;
        break;
        case      AUDIO_DEVICE_MODE_EARFREE:
        retdev = AUDIO_HP;
        break;
        case      AUDIO_DEVICE_MODE_TVOUT:
        retdev = AUDIO_HP;
        break;
        case      AUDIO_DEVICE_MODE_BLUEPHONE:
        retdev = AUDIO_HP;
        break;
        default:
        retdev = AUDIO_SPK;
        break;
    }

    return retdev;
}

LOCAL char* _ARMVB_ArmvbNv_GetName(AUDIO_DEVICE_MODE_TYPE_E aud_dev_mode)
{
	NV_AUDIO_ARM_E arm_nv_id = NV_AUDIO_ARM_HANDSET_E;

	switch (aud_dev_mode) {
	case AUDIO_DEVICE_MODE_HANDHOLD:
		arm_nv_id = NV_AUDIO_ARM_HANDSET_E;
		break;
	case AUDIO_DEVICE_MODE_HANDFREE:
		arm_nv_id = NV_AUDIO_ARM_HANDSFREE_E;
		break;
	case AUDIO_DEVICE_MODE_EARPHONE:
		arm_nv_id = NV_AUDIO_ARM_HEADSET_E;
		break;
	case AUDIO_DEVICE_MODE_EARFREE:
		arm_nv_id = NV_AUDIO_ARM_EARFREE_E;
		break;
	case AUDIO_DEVICE_MODE_TVOUT:
		arm_nv_id = NV_AUDIO_ARM_HEADSET_E;
		break;
	case AUDIO_DEVICE_MODE_BLUEPHONE:
		arm_nv_id = NV_AUDIO_ARM_BTHS_E;
		break;
	default:
		arm_nv_id = NV_AUDIO_ARM_HANDSFREE_E;
		break;
	}
	return AUDIONVARM_GetAudioModeNameById(arm_nv_id);
}

uint32 AUDDEV_Record_Mode2Dev(AUDIO_DEVICE_MODE_TYPE_E eMode)
{
    uint32 retdev = AUDIO_HP;

    switch(eMode)
    {
        case      AUDIO_DEVICE_MODE_HANDHOLD:
        retdev = AUDIO_MIC;
        break;
        case      AUDIO_DEVICE_MODE_HANDFREE:
        retdev = AUDIO_MIC;
        break;
        case      AUDIO_DEVICE_MODE_EARPHONE:
        retdev = AUDIO_HP_MIC;
        break;
        case      AUDIO_DEVICE_MODE_EARFREE:
        retdev = AUDIO_HP_MIC;
        break;
        case      AUDIO_DEVICE_MODE_TVOUT:
        retdev = AUDIO_MIC;
        break;
        case      AUDIO_DEVICE_MODE_BLUEPHONE:
        retdev = AUDIO_MIC;
        break;
        default:
        retdev = AUDIO_MIC;
        break;
    }

    return retdev;
}

PUBLIC char * AUDDEV_ArmvbNv_GetName(AUDIO_DEVICE_MODE_TYPE_E aud_dev_mode)
{
    uint32 arm_nv_id = NV_AUDIO_ARM_HANDSET_E;

    switch(aud_dev_mode)
    {
        case      AUDIO_DEVICE_MODE_HANDHOLD:
        arm_nv_id = AUDIO_EAR;
        break;
        case      AUDIO_DEVICE_MODE_HANDFREE:
        arm_nv_id = AUDIO_SPK;
        break;
        case      AUDIO_DEVICE_MODE_EARPHONE:
        arm_nv_id = AUDIO_HP;
        break;
        case      AUDIO_DEVICE_MODE_EARFREE:
        arm_nv_id = NV_AUDIO_ARM_EARFREE_E;
        break;
        case      AUDIO_DEVICE_MODE_TVOUT:
        arm_nv_id = AUDIO_HP;
        break;
        case      AUDIO_DEVICE_MODE_BLUEPHONE:
        arm_nv_id = AUDIO_HP;
        break;
        default:
        arm_nv_id = AUDIO_SPK;
        break;
    }

    return AUDIONVARM_GetAudioModeNameById(arm_nv_id);
}

/*****************************************************************************/
//  Description:    adjust the address of left channel and right channel to output .
//  Author:         Shujing.Dong
//  Note:
//****************************************************************************/
LOCAL int32 AUDDEV_DrvOutUpdate(void)
{
    AUDIO_RESULT_E ret = 0;
    AUDIO_CODEC_PATH_T codecpath = {0};
    AUDIO_CTL_ARG_U arg;

    codecpath.device = s_auddev_playinfo.pathdevice;
    codecpath.gain1 = s_auddev_playinfo.codec_gain1;
    codecpath.gain2 = s_auddev_playinfo.codec_gain2;
    codecpath.mute = s_auddev_playinfo.mute;

    arg.addr_path = (uint32 *)&codecpath;
    ret = AUDIO_HAL_Ioctl(AUDIO_CODEC_CMD_OUTPUT_PATH_SET, (uint32)&arg);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c output err:%d\n",ret);
    }
    SCI_TRACE_LOW("[DrvOut] pathdevice %d, gain1:0x%x, gain2:0x%x, mute:%d\n", s_auddev_playinfo.pathdevice,
        s_auddev_playinfo.codec_gain1, s_auddev_playinfo.codec_gain2, s_auddev_playinfo.mute);
    return ret;
}

LOCAL int32 AUDDEV_DrvInUpdate(void)
{
    AUDIO_RESULT_E ret = 0;
    AUDIO_CODEC_PATH_T codecpath = {0};
    AUDIO_CTL_ARG_U arg;

    codecpath.device = s_auddev_recordinfo.pathdevice;
    codecpath.gain1 = s_auddev_recordinfo.codec_gain1;
    codecpath.gain2 = s_auddev_recordinfo.codec_gain2;
    codecpath.mute = s_auddev_recordinfo.mute;

    arg.addr_path = (uint32 *)&codecpath;
    ret = AUDIO_HAL_Ioctl(AUDIO_CODEC_CMD_INPUT_PATH_SET, (uint32)&arg);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c input err:%d\n",ret);
    }
    SCI_TRACE_LOW("[DrvIn] pathdevice %d, gain1:0x%x, gain2:0x%x, mute:%d\n", s_auddev_recordinfo.pathdevice,
        s_auddev_recordinfo.codec_gain1, s_auddev_recordinfo.codec_gain2, s_auddev_recordinfo.mute);

    return ret;
}



uint32 AUDDEV_GetRemainedPcmCount(void)
{
    return s_armvb_output_buf.size/2;
}

void AUDDEV_PLAYBACK_Callback (void)
{
    uint32 data_count;
    int16* dac_buffer;
    int need_downmix = 0;
    uint32 tick_cnt;

    //SCI_TRACE_LOW("\n[%s][%d] in\n", __FUNCTION__, SCI_GetTickCount());

    AUDIO_HAL_DAC_Get_Read_Point(&s_armvb_output_buf.pRead);
    if (s_armvb_output_buf.pRead == s_armvb_output_buf.pWrite) {
        SCI_TRACE_LOW("[%s][%d] undder run\n", __FUNCTION__, SCI_GetTickCount());
        // skip one (buffer_size/2)
        s_armvb_output_buf.pWrite += s_armvb_output_buf.size>>1;
        if(s_armvb_output_buf.pWrite >= s_armvb_output_buf.size){
            s_armvb_output_buf.pWrite = 0;
        }
    }

    dac_buffer = (int16*)((uint32)s_armvb_output_buf.data_align_ptr +\
                            s_armvb_output_buf.pWrite);
    if (s_auddev_playinfo.chanelnum == AUDIO_CH_MONO) {
        need_downmix = 1;
        data_count = s_armvb_output_buf.size>>2;
    } else {
        need_downmix = 0;
        data_count = s_armvb_output_buf.size>>3;
    }
    _Track_Mixer(g_armvb_device_ptr->ptTrackList,
                 s_vb_da0_buffer_multitrack,
                 s_vb_da1_buffer_multitrack,
                 dac_buffer,
                 SCI_NULL,
                 data_count,
                 1,
                 need_downmix,
                 s_auddev_playinfo.mute,
                 s_auddev_playinfo.dg_level);

    s_armvb_output_buf.pWrite += s_armvb_output_buf.size>>1;
    if(s_armvb_output_buf.pWrite >= s_armvb_output_buf.size){
        s_armvb_output_buf.pWrite = 0;
    }
    AUDIO_HAL_DAC_Set_Write_Point(s_armvb_output_buf.pWrite);
}

LOCAL void AUDDEV_SetDepopMute(
    BOOLEAN is_mute
)
{
    //SCI_TRACE_LOW("[armvb_as] line[%d] AUDDEV_SetDepopMute \n", __LINE__);
    return;
}
/*****************************************************************************/
//  Description:    Set Arm VB Downlink Mute or Cancel the mute_set.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL void AUDDEV_SetDownlinkMute (
    BOOLEAN is_mute
)
{
    AUDIO_RESULT_E ret = 0;
    AUDIO_CTL_ARG_U arg;

    arg.mute = is_mute;
    s_auddev_playinfo.mute = is_mute;

    ret = AUDIO_HAL_Ioctl(AUDIO_CODEC_CMD_OUTPUT_MUTE_SET, (uint32)&arg);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Play mute err:%d\n",ret);
    }
}

LOCAL char* _AUDDEV_CodecNv_MusicGetName(AUDIO_DEVICE_MODE_TYPE_E aud_dev_mode)
{
    NV_AUDIO_CODEC_E_ codec_nv_id = NV_AUDIO_CODEC_HANDSFREE_MUSIC_E;
#if 0  // only handsfree
    switch (aud_dev_mode) {
        case AUDIO_DEVICE_MODE_HANDHOLD:
            index = NV_AUDIO_CODEC_HANDSET_MUSIC_E;
            break;
        case AUDIO_DEVICE_MODE_HANDFREE:
            index = NV_AUDIO_CODEC_HANDSFREE_MUSIC_E;
            break;
        case AUDIO_DEVICE_MODE_EARPHONE:
            index = NV_AUDIO_CODEC_HEADSET_MUSIC_E;
            break;
        case AUDIO_DEVICE_MODE_EARFREE:
            index = NV_AUDIO_CODEC_HEADFREE_MUSIC_E;
            break;
        default:
            index = NV_AUDIO_CODEC_HANDSFREE_MUSIC_E;
            break;
    }
#endif
    return CODEC_PARAM_GetNameByIndex(codec_nv_id);
}

/*****************************************************************************/
//  Description:    AUDDEV logic device set device volume.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E AUDDEV_SetVolumeType (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType)
{
    return AUDIO_NO_ERROR;
}

LOCAL uint16 Vol2DgLevel(uint32  uiVol)
{
    uint16 dg_level;

    switch (uiVol)
    {
    case 0: //mute
        dg_level = 0;
        break;
    case 1:
        dg_level = 1034; // -24db
        break;
    case 2:
        dg_level = 1460; // -21db
        break;
    case 3:
        dg_level = 2063; // -18db
        break;
    case 4:
        dg_level = 2914; // -15db
        break;
    case 5:
        dg_level = 4115; // -12db
        break;
    case 6:
        dg_level = 5813; // -9db
        break;
    case 7:
        dg_level = 8211; // -6db
        break;
    case 8:
        dg_level = 11599; // -3db
        break;
    case 9:
        dg_level = 16384; // 0db
        break;
    default:
        dg_level = 16384;
        break;
    }

    return dg_level;
}

void AUDDEV_SetVolume (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType)
{
    AUDIO_DEVICE_MODE_TYPE_E cur_dev_mode = AUDIO_DEVICE_MODE_HANDHOLD;
    char *mode_name = NULL;
    CODEC_PARAM_NV_T *codec_parm = NULL;

    AUDIO_GetDevMode(&cur_dev_mode);
    mode_name = _AUDDEV_CodecNv_MusicGetName(cur_dev_mode);
    SCI_TRACE_LOW("[armvb_as] AUDDEV_SetVolume uiVol:0x%x eAppType:0x%x %s\n", uiVol, eAppType, mode_name);
    codec_parm = (CODEC_PARAM_NV_T *)AUDIO_ALLOC(sizeof(CODEC_PARAM_NV_T));
    if (PNULL==codec_parm) {
        SCI_TRACE_LOW("armvb_as.c malloc failed %d\n", __LINE__);
        return;
    }

    CODEC_PARAM_GetParamFromMemByName(mode_name, codec_parm);
    uiVol &= 0xF;
    s_auddev_playinfo.dg_level = Vol2DgLevel(uiVol);
    s_auddev_playinfo.codec_gain1 = codec_parm->CODEC_param_NV.outGains[uiVol].ana;
    s_auddev_playinfo.codec_gain2 = codec_parm->CODEC_param_NV.outGains[uiVol].dac;
    AUDDEV_DrvOutUpdate();
    SCI_FREE(codec_parm);
}


/*****************************************************************************/
//  Description:    AUDDEV logic device set device mode.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL void AUDDEV_SetDevMode (AUDIO_DEVICE_MODE_TYPE_E eMode)
{
    uint32 Device = AUDIO_SPK;

    Device = AUDDEV_Mode2Dev(eMode);
    s_auddev_playinfo.pathdevice = Device;
    AUDDEV_DrvOutUpdate();
}


/*****************************************************************************/
//  Description:    Open voiceband.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E AUDDEV_Play (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    uint32 buffer_size;
    AUDIO_CTL_ARG_U arg;
    AUDIO_DEVICE_MODE_TYPE_E cur_eDev_mode = ptDevSetting->eDev_mode;

    SCI_TRACE_LOW("%s,in,samplerate %d; ch %d\n",
        __FUNCTION__,uiSampleRate,ptDevSetting->eChannelChoice);

    if ((AUDIO_DEVICE_MODE_BLUEPHONE == cur_eDev_mode ) || (AUDIO_DEVICE_MODE_TVOUT == cur_eDev_mode))
    {
        SCI_TRACE_LOW("AUDDEV_Play mode is BT/TVOUT, change to HANDFREE.\n");
        cur_eDev_mode = AUDIO_DEVICE_MODE_HANDFREE;
    }


    //ptDevSetting->eChannelChoice = DEV_DATA_OUTPUT_CHANNEL_CHOICE_LEFT;

    if (ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_NORMAL) {
        s_auddev_playinfo.chanelnum = AUDIO_CH_STEREO;
    } else if ((ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_LEFT) ||
        (ptDevSetting->eChannelChoice == DEV_DATA_OUTPUT_CHANNEL_CHOICE_RIGHT)){
        s_auddev_playinfo.chanelnum = AUDIO_CH_MONO;
    } else {
        SCI_TRACE_LOW("armvb_as.c ch err\n");
    }

    arg.channel = AUDIO_CH_STEREO; // force stereo
    ret = AUDIO_HAL_Ioctl(AUDIO_DAI_CMD_CHANNEL_SET, (uint32)&arg);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Play AUDIO_DAI_CMD_CHANNEL_SET err:%d\n",ret);
    }

    s_auddev_playinfo.samplerate = uiSampleRate;
    ret = AUDIO_HAL_Open(AUDIO_HAL_PLAYBACK, uiSampleRate);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Play open AUDIO_HAL_PLAYBACK err:%d\n",ret);
    }

    AUDDEV_SetDevMode(cur_eDev_mode);
    AUDDEV_SetVolume (ptDevSetting->uiVol, ptDevSetting->eAppType);
    AUDDEV_SetDownlinkMute(ptDevSetting->bDownlink_mute);

    // samplerate * 2chananel * 20ms * 16bit * buffernum / 1000
    buffer_size = (s_auddev_playinfo.samplerate *\
                    s_auddev_playinfo.chanelnum *\
                    20 * 2 * 2 ) / 1000;
    buffer_size = ALIGN_UP_WITH_32(buffer_size);

    SCI_MEMSET(&s_armvb_output_buf, 0, sizeof(AUDDEV_PLAYBACK_BUF));
    s_armvb_output_buf.data_ptr = (uint16 *)AUDIO_ALLOC(buffer_size+64);
    if(s_armvb_output_buf.data_ptr == NULL){
        SCI_TRACE_LOW("armvb_as.c malloc failed %d\n", __LINE__);
        return AUDIO_NO_ENOUGH_MEMORY;
    }
    SCI_MEMSET(s_armvb_output_buf.data_ptr, 0, buffer_size+64);

    s_armvb_output_buf.data_align_ptr =
        (int16*)ALIGN_UP_WITH_32(s_armvb_output_buf.data_ptr);
    s_armvb_output_buf.pRead = 0;   // point that hw read
    s_armvb_output_buf.pWrite = 0;   // point that device write
    s_armvb_output_buf.size = buffer_size;
    SCI_TRACE_LOW("armvb_as.c buf:0x%x,0x%x,%d\n",
        s_armvb_output_buf.data_ptr,
        s_armvb_output_buf.data_align_ptr,
        s_armvb_output_buf.size);

    ret = AUDIO_HAL_DAC_Set_Buf_Info(
            s_armvb_output_buf.data_align_ptr, 0, s_armvb_output_buf.size);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Play set buffer info err:%d\n",ret);
        return ret;
    }
    if (NULL == s_vb_da0_buffer_multitrack) {
        s_vb_da0_buffer_multitrack = AUDIO_ALLOC(MULTITRACK_BUFFER_SIZE);
        if (NULL == s_vb_da0_buffer_multitrack) {
            SCI_TRACE_LOW("armvb_as.c malloc failed %d\n", __LINE__);
            SCI_FREE(s_armvb_output_buf.data_ptr);
            s_armvb_output_buf.data_ptr = NULL;
            return AUDIO_NO_ENOUGH_MEMORY;
        }
    }

    if (NULL == s_vb_da1_buffer_multitrack) {
        s_vb_da1_buffer_multitrack = AUDIO_ALLOC(MULTITRACK_BUFFER_SIZE);
        if (NULL == s_vb_da1_buffer_multitrack) {
            SCI_TRACE_LOW("armvb_as.c malloc failed %d\n", __LINE__);
            SCI_FREE(s_armvb_output_buf.data_ptr);
            SCI_FREE(s_vb_da0_buffer_multitrack);
            s_armvb_output_buf.data_ptr = NULL;
            s_armvb_output_buf.data_align_ptr = NULL;
            s_vb_da0_buffer_multitrack = NULL;
            return AUDIO_NO_ENOUGH_MEMORY;
        }
    }

    ret = AUDIO_HAL_Register_Playback((AUDIO_HAL_CALLBACK_FUN)AUDDEV_PLAYBACK_Callback);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Play Register_Playback err:%d\n",ret);
        return ret;
    }

    //usleep(10000);

    return ret;
}

/*****************************************************************************/
//  Description:    Close voiceband
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E AUDDEV_Stop (DEV_SETTING_T *ptDevSetting)
{
    uint32 ret = 0;

    SCI_TRACE_LOW("AUDDEV_Stop\n");

    ret = AUDIO_HAL_Close(AUDIO_HAL_PLAYBACK);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c hal close err:%d\n",ret);
        SCI_PASSERT(0, ("AUDIO_HAL_Close fail"));
    }

    if ( s_armvb_output_buf.data_ptr ) {
        SCI_FREE(s_armvb_output_buf.data_ptr);
        s_armvb_output_buf.data_ptr = NULL;
        s_armvb_output_buf.data_align_ptr = NULL;
    }
    if (s_vb_da0_buffer_multitrack) {
        SCI_FREE(s_vb_da0_buffer_multitrack);
        s_vb_da0_buffer_multitrack = NULL;
    }

    if (s_vb_da1_buffer_multitrack) {
        SCI_FREE(s_vb_da1_buffer_multitrack);
        s_vb_da1_buffer_multitrack = NULL;
    }

    return ret;
}

LOCAL void AUDDEV_SetSampleRate (uint32 sample_rate)
{
    //SCI_TRACE_LOW("[armvb_as] line[%d] file[%s] func[%s]\n",
    //    __LINE__, __FILE__, __FUNCTION__);
    return;
}

LOCAL uint32 AUDDEV_GetAppropriateOutputSamplerate(uint32 uisamplerate)
{
    if (AUDIO_HAL_SUCCESS == AUDIO_HAL_FS_CHECK(uisamplerate)) {
        SCI_TRACE_LOW("%s sa:%d\n",
            __FUNCTION__, uisamplerate);
        return uisamplerate;
    } else {
        SCI_TRACE_LOW("%s not support sa:%d\n",
            __FUNCTION__, uisamplerate);
        return 0;
    }
}

LOCAL int32 AUDDEV_EnableOutput(void)
{
    int32 ret;
    ret = AUDIO_HAL_DAC_Trigger();
    if (AUDIO_HAL_SUCCESS != ret) {
        SCI_TRACE_LOW("AUDDEV_EnableOutput Trigger err:%d\n",ret);
    }
    return ret;
}

/*****************************************************************************/
//  Description:    Register voiceband controlled by arm to audio service.
//  Author:         Benjamin.Wang
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV AUDDEV_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

    SCI_TRACE_LOW("AUDDEV_RegAudioDevice\n");
    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("AUDDEV"), strlen ("AUDDEV") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_TRUE;
    tDeviceInfo.uiMinFetchLength = TMP_FIFO_SIZE;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = AUDDEV_Play;
    tDeviceInfo.tAudioDevOpe.pClose = AUDDEV_Stop;
    tDeviceInfo.tAudioDevOpe.pPause = AUDDEV_Stop;
    tDeviceInfo.tAudioDevOpe.pResume = AUDDEV_Play;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;

    tDeviceInfo.tAudioDevOpe.pSetVolume = AUDDEV_SetVolume;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = AUDDEV_SetDevMode;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = AUDDEV_SetDownlinkMute;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=AUDDEV_GetRemainedPcmCount;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;//AUDDEV_GetRemainedPcmTime;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = AUDDEV_GetAppropriateOutputSamplerate;
    tDeviceInfo.tAudioDevOpe.pSetNotchton=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pEnableOutput=AUDDEV_EnableOutput;
    //tDeviceInfo.tAudioDevOpe.pSetVolGain = SCI_NULL;//_AUDDEV_SetVolGain;

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);
    g_armvb_device_ptr = AUDIO_DM_GetDeviceHandleStruct (hDevice);  // Record device structure
    SCI_TRACE_LOW("[armvb_as] g_armvb_device_ptr:0x%x \n", g_armvb_device_ptr);
    return hDevice;
}

//****************************************************************************/
// record device
//****************************************************************************/
/********************fade in process begin***********************************/
LOCAL void AUDDEV_Record_Fadein_Prepare(
    AUDDEV_FADEIN_CTL_T *ctl_p,
    uint32 samplerate,
    uint32 channel,
    uint32 mute_time,
    uint32 fadein_time)
{
    if (PNULL == ctl_p)
        return;

    ctl_p->ch = channel;
    ctl_p->mute_samples = mute_time * samplerate * channel / 1000;
    ctl_p->fadein_samples = fadein_time * samplerate * channel / 1000;
    ctl_p->process_samples = 0;
    ctl_p->started = 0;

    SCI_TRACE_LOW("[FADEIN][ctl=0x%x] prepare %d %d, sr=%d ch=%d\n",
        ctl_p, ctl_p->mute_samples, ctl_p->fadein_samples, samplerate, channel);
}

LOCAL void AUDDEV_Record_Fadein_Next(
    AUDDEV_FADEIN_CTL_T *ctl_p,
    uint32 process_len)
{
    uint32 sample_len;

    if (PNULL == ctl_p)
        return;

    if((ctl_p->fadein_samples + ctl_p->mute_samples <= ctl_p->process_samples) ||
        (0 == ctl_p->started)) {
        return;
    }

    sample_len = ctl_p->fadein_samples + ctl_p->mute_samples - ctl_p->process_samples;
    sample_len = (sample_len < process_len)? sample_len : process_len;
    ctl_p->process_samples += sample_len;

    SCI_TRACE_LOW("[FADEIN][ctl=0x%x] next %d %d %d (%d)\n",
        ctl_p,
        ctl_p->mute_samples,
        ctl_p->fadein_samples,
        ctl_p->process_samples,
        process_len);
}

LOCAL void AUDDEV_Record_Fadein_Process(
    AUDDEV_FADEIN_CTL_T *ctl_p,
    int16 *p_data,
    uint32 process_len)
{
    uint32 i = 0;
    uint32 mute_sample_len = 0, fadein_sample_len = 0;
    uint32 gain1 = 0, gain2 = ctl_p->fadein_samples;
    int16 *p_data_tmp;

    if (PNULL == ctl_p || PNULL == p_data)
        return;

    if(ctl_p->fadein_samples + ctl_p->mute_samples <= ctl_p->process_samples)
        return;

    p_data_tmp = p_data;

    // mute
    if (ctl_p->mute_samples > ctl_p->process_samples) {
        mute_sample_len = ctl_p->mute_samples - ctl_p->process_samples;
        mute_sample_len = (mute_sample_len < process_len)? mute_sample_len : process_len;
        SCI_MEMSET(p_data_tmp, 0, mute_sample_len << 1);
        process_len -= mute_sample_len;
        p_data_tmp += mute_sample_len;
        SCI_TRACE_LOW("[FADEIN][ctl=0x%x][0x%x] mute process %d %d %d\n", ctl_p, p_data,
            ctl_p->mute_samples,
            ctl_p->process_samples,
            mute_sample_len);
    }

    // fadein
    fadein_sample_len = (ctl_p->fadein_samples + ctl_p->mute_samples) -\
                            (ctl_p->process_samples + mute_sample_len);
    if ((fadein_sample_len > 0) && (process_len > 0)) {
        fadein_sample_len = (fadein_sample_len < process_len)? fadein_sample_len : process_len;
        for (i = 0; i < fadein_sample_len; i++) {
            int32 sample = (int32)(*p_data_tmp);
            gain1 = ctl_p->process_samples - ctl_p->mute_samples + i;
            sample = (sample * (int32)gain1 / (int32)gain2);
            *(p_data_tmp++) = (int16)sample;
        }
        SCI_TRACE_LOW("[FADEIN][ctl=0x%x][0x%x] fadein process %d %d %d\n", ctl_p, p_data,
            ctl_p->fadein_samples,
            ctl_p->process_samples,
            fadein_sample_len);
    }

    ctl_p->started = 1;
}
/********************fade in process end*************************************/

/*****************************************************************************/
//  Description:    Set Arm VB Downlink Mute or Cancel the mute_set.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
LOCAL void AUDDEV_SetUplinkMute (
    BOOLEAN is_mute
)
{
    AUDIO_RESULT_E ret = 0;
    AUDIO_CTL_ARG_U arg;

    arg.mute = is_mute;
    s_auddev_recordinfo.mute = is_mute;

    ret = AUDIO_HAL_Ioctl(AUDIO_CODEC_CMD_INPUT_MUTE_SET, (uint32)&arg);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Record mute err:%d\n",ret);
    }
}

/*****************************************************************************/
//  Description:    AUDDEV logic device set device volume.
//  Author:         Johnson.sun
//  Note:
//****************************************************************************/
//LOCAL AUDIO_RESULT_E AUDDEV_Record_SetVolumeType (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType)
//{
   // return AUDIO_NO_ERROR;
//}

LOCAL void AUDDEV_Record_SetVolume (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType)
{
    AUDIO_DEVICE_MODE_TYPE_E cur_dev_mode = AUDIO_DEVICE_MODE_HANDHOLD;
    char *mode_name = NULL;
    CODEC_PARAM_NV_T *codec_parm = NULL;

    AUDIO_GetDevMode(&cur_dev_mode);
    mode_name = _AUDDEV_CodecNv_MusicGetName(cur_dev_mode);
    SCI_TRACE_LOW("[%s] uiVol:%d eAppType:0x%x, %s",__FUNCTION__, uiVol, eAppType, mode_name);
    codec_parm = (CODEC_PARAM_NV_T *)AUDIO_ALLOC(sizeof(CODEC_PARAM_NV_T));
    if (PNULL==codec_parm) {
        SCI_TRACE_LOW("armvb_as.c malloc failed %d\n", __LINE__);
        return;
    }
    CODEC_PARAM_GetParamFromMemByName(mode_name, codec_parm);
    s_auddev_recordinfo.dg_level = uiVol;
    s_auddev_recordinfo.codec_gain1 = codec_parm->CODEC_param_NV.inGains.adc;
    s_auddev_recordinfo.codec_gain2 = codec_parm->CODEC_param_NV.inGains.ana;
    AUDDEV_DrvInUpdate();
    SCI_FREE(codec_parm);

/*
#ifdef CVSRCD_ENABLE
    {
        AUDIO_DEVICE_MODE_TYPE_E dev_mode = AUDIO_DEVICE_MODE_HANDHOLD;
        char *mode_name = NULL;
        AUDIO_GetDevMode (&dev_mode);
        mode_name = _ARMVB_ArmvbNv_GetName(dev_mode);
        CVSRCD_Init(mode_name, 0);
    }
#endif
*/
    return ;
}

/*****************************************************************************/
//  Description:    ARM VB Sets paras for changes from nv
//  Author:         Cherry.Liu
//  Note:           豕?nv dev mode?Darm?角1?米?2?那y?邦??,?辰米¯車?∩?o‘那y辰?那12?那y谷迆D∫!
//****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDDEV_Record_SetDevModeExp (AUDIO_DEVICE_MODE_TYPE_E eMode)
{
    AUDIO_RESULT_E  result = AUDIO_NO_ERROR;
    return result;
}

LOCAL AUDIO_RESULT_E AUDDEV_Record_PostOpen(void *ptParam)
{
    AUDIO_RESULT_E result = AUDIO_NO_ERROR;

    return result;
}

LOCAL AUDIO_RESULT_E AUDDEV_Record_PreClose(void *ptParam)
{
    AUDIO_RESULT_E result = AUDIO_NO_ERROR;
    return result;
}
/*****************************************************************************/
//  Description:    AUDDEV RECORD logic device set device mode.
//  Author:
//  Note:
//****************************************************************************/
LOCAL void AUDDEV_Record_SetDevMode (AUDIO_DEVICE_MODE_TYPE_E eMode)
{
    uint32 Device = AUDIO_SPK;
    SCI_TRACE_LOW("%s,%d\n",__FUNCTION__, eMode);
    //change nv to new mode

    Device = AUDDEV_Record_Mode2Dev(eMode);
    s_auddev_recordinfo.pathdevice = Device;
    AUDDEV_DrvInUpdate();

    if (s_b_armvb_recording) {
#ifdef CVSRCD_ENABLE
{
        AUDIO_DEVICE_MODE_TYPE_E dev_mode = AUDIO_DEVICE_MODE_HANDHOLD;
        char *mode_name = NULL;

        CVSRCD_DeInit();
        SCI_Sleep(10);

        AUDIO_GetDevMode (&dev_mode);
        mode_name = _ARMVB_ArmvbNv_GetName(dev_mode);
        CVSRCD_Init(mode_name, s_auddev_recordinfo.samplerate);
}
#endif
    }

    return;
}
/*****************************************************************************/
//  Description:    AUDDEV RECORD logic device reg Rec callback
//  Author:
//  Note:
//****************************************************************************/
LOCAL uint32 AUDDEV_Record_RegRecCallback (
    ADM_RECCALLBACK pCallback
)
{
    int32 i = 0;

    SCI_PASSERT((pCallback != SCI_NULL), /*assert verified*/
        ("AUDDEV_Record_RegRecCallback: pCallback should not be NULL!"));

    // Find the vacant position and save it.
    for(i = 0; i < AUDDEV_RECORD_OUTPUT_COUNT_MAX; i++)
    {
        if(s_armrecord_callback[i] == SCI_NULL)
        {
            s_record_callback_count++;
            s_armrecord_callback[i] = pCallback;
            SCI_TRACE_LOW("\n%s,%d: %d 0x%x\n",
                __FUNCTION__,__LINE__,
                i, (uint32) s_armrecord_callback[i]);
            return (uint32) s_armrecord_callback[i];
        }
    }

    return (uint32)SCI_NULL;
}

/*****************************************************************************/
//  Description:    AUDDEV RECORD logic device unreg Rec callback
//  Author:
//  Note:
//****************************************************************************/
LOCAL void AUDDEV_Record_UnRegRecCallback (
    uint32 callback_vector
)
{
    int32 i = 0;
    // Find the occupied position by callback function and clear it.
    for(i = 0; i < AUDDEV_RECORD_OUTPUT_COUNT_MAX; i++)
    {
        if((uint32)s_armrecord_callback[i] == callback_vector)
        {
            //SCI_PASSERT((s_record_callback_count>0),  /*assert verified*/
            //    ("count:%d, addr:0x%x.", s_record_callback_count, callback_vector));
            s_record_callback_count--;
            s_armrecord_callback[i] = SCI_NULL;
            break;
        }
    }
    SCI_TRACE_LOW("%s,%d\n",__FUNCTION__, s_record_callback_count);
     return;
}

LOCAL void AUDDEV_Record_CopyAudioData(uint16 *puiDestData,
            const uint16 *puiLeftData,
            const uint16 *puiRightData,
            uint32 uiOffset,
            uint32 uiSampleCount)
{
    if (!puiRightData)
    {
        SCI_MEMCPY(puiDestData, puiLeftData+uiOffset, uiSampleCount<<1);
    }
    else
    {
        int16 *left_ptr = (int16 *)puiLeftData + uiOffset;
        int16 *right_ptr = (int16 *)puiRightData + uiOffset;
        while (uiSampleCount)
        {
            int32 sample = ((*left_ptr + *right_ptr) /2);
            *puiDestData = (uint16)sample;
            puiDestData++;
            left_ptr++;
            right_ptr++;
            uiSampleCount--;
        }
    }
}

/*****************************************************************************/
//  Description:    AUDDEV RECORD logic device read data interface
//  Author:
//  Note:
//****************************************************************************/
LOCAL void AUDDEV_Record_ReadData_Internal(uint16 *puiDestData, uint32 sampleLen, uint32 *puiDataLen)
{
    uint32 i;
    SCI_ASSERT (PNULL != puiDestData);  /*assert verified*/
    SCI_ASSERT (PNULL != s_vb_arm_record_buf.tmp_buf);  /*assert verified*/

    if ((s_vb_arm_record_buf.size>>1) != (sampleLen<<2)) {
        SCI_TRACE_LOW("[%s] unsupport sampleLen=[%d] \n", __FUNCTION__,sampleLen);
        *puiDataLen = 0;
        return;
    }

    SCI_MEMCPY(s_vb_arm_record_buf.tmp_buf,
                (((int8*)s_vb_arm_record_buf.data_align_ptr) + s_vb_arm_record_buf.pRead),
                (s_vb_arm_record_buf.size>>1));

    // force mono
    for (i = 0; i < sampleLen; i++) {
        *(puiDestData+i) = *(s_vb_arm_record_buf.tmp_buf+i*2);
    }

    *puiDataLen = sampleLen<<1;
    SCI_TRACE_LOW("[AUDDEV][%s] addr[0x%x] sampleLen[%d] puiDataLen[%d] \n",
        __FUNCTION__, puiDestData, sampleLen, *puiDataLen);
}

LOCAL void AUDDEV_Record_ReadData(uint16 *puiDestData, uint32 sampleLen, uint32 *pbytelen)
{
    SCI_TRACE_LOW("[AUDDEV][%s] 0x%x [%d] \n", __FUNCTION__, puiDestData, sampleLen);

    AUDDEV_Record_ReadData_Internal(puiDestData, sampleLen, pbytelen);

    //Datadump_Output(1, puiDestData, *pbytelen);

/*
    // fade in
    AUDDEV_Record_Fadein_Process(&s_fadein_ctl,
                                (int16 *)puiDestData,
                                ((*pbytelen) >> 1)); // mono
*/

    //Datadump_Output(2, puiDestData, *pbytelen);
}


/*****************************************************************************/
//  Description:    Record DMA Callback Reg to drv
//  Author:
//  Note:
//****************************************************************************/
void AUDDEV_Record_Callback (void)
{
    uint32 i=0;
    uint32 samplelen;
    samplelen = s_vb_arm_record_buf.size>>3; // 16bit + stereo + 2buf

    AUDIO_HAL_ADC_Get_Write_Point(&s_vb_arm_record_buf.pWrite);
    if (s_vb_arm_record_buf.pWrite == s_vb_arm_record_buf.pRead) {
        SCI_TRACE_LOW("\n[%s][%d] over run\n", __FUNCTION__, SCI_GetTickCount());
        // skip one (buffer_size/2)
        return;
    }

    // cb
    if (s_record_callback_count>0) {
        for (i=0; i<s_record_callback_count; i++) {
            if (PNULL!=s_armrecord_callback[i]) {
                s_armrecord_callback[i](samplelen);
            }
        }
    }
/*
    // fade in next
    AUDDEV_Record_Fadein_Next(&s_fadein_ctl, samplelen); // 16bit
*/
    // update
    s_vb_arm_record_buf.pRead += samplelen << 2; // 16bit + stereo
    if(s_vb_arm_record_buf.pRead >= s_vb_arm_record_buf.size){
        s_vb_arm_record_buf.pRead = 0;
    }
    AUDIO_HAL_ADC_Set_Read_Point(s_vb_arm_record_buf.pRead);
}

/*****************************************************************************/
//  Description:    Open device to record.
//  Author:
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E AUDDEV_Record_Play (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
    AUDIO_RESULT_E ret = 0;
    uint32 buffer_size;
    AUDIO_CTL_ARG_U arg;

    AUDIO_DEVICE_MODE_TYPE_E cur_eDev_mode = ptDevSetting->eDev_mode;
    char * mode_name = NULL;

    SCI_TRACE_LOW("%s,%d entry,samplerate %d; ch %d\n",
        __FUNCTION__, __LINE__,uiSampleRate,ptDevSetting->eChannelChoice);

    if ((AUDIO_DEVICE_MODE_BLUEPHONE == cur_eDev_mode ) || (AUDIO_DEVICE_MODE_TVOUT == cur_eDev_mode))
    {
        SCI_TRACE_LOW("AUDDEV_Record_Play mode is BT/TVOUT, change to HANDFREE.\n");
        cur_eDev_mode = AUDIO_DEVICE_MODE_HANDFREE;
    }

    cur_eDev_mode = AUDIO_DEVICE_MODE_HANDFREE;
    mode_name = AUDDEV_ArmvbNv_GetName(cur_eDev_mode);
    if (PNULL == mode_name) {
        SCI_TRACE_LOW("AUDDEV_Record_Play mode:%d\n",cur_eDev_mode);
        return AUDIO_PARAM_ERROR;
    }

    if (s_b_armvb_recording){
        SCI_TRACE_LOW("AUDDEV_Record_Play device id busy\n");
        return AUDIO_HAL_SUCCESS;
    }

    s_auddev_recordinfo.chanelnum = AUDIO_CH_STEREO;        // force mono from dm
    arg.channel = AUDIO_CH_STEREO;                          // force stereo for dev
    ret = AUDIO_HAL_Ioctl(AUDIO_DAI_CMD_CHANNEL_SET, (uint32)&arg);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Record_Play set channel err:%d\n",ret);
    }

    s_auddev_recordinfo.samplerate = uiSampleRate;
    ret = AUDIO_HAL_Open(AUDIO_HAL_CAPTURE, uiSampleRate);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Record_Play set samplerate err:%d\n",ret);
    }

    AUDDEV_SetUplinkMute(ptDevSetting->bUplink_mute);
    AUDDEV_Record_SetDevMode(ptDevSetting->eDev_mode);
    AUDDEV_Record_SetVolume(ptDevSetting->uiVol, ptDevSetting->eAppType);

    // samplerate * ch * 16bit * 20ms * buffernum / 1000;
    buffer_size = (uiSampleRate * s_auddev_recordinfo.chanelnum * 2 * 20 * 2) / 1000;
    buffer_size = ALIGN_UP_WITH_32(buffer_size);

    SCI_MEMSET(&s_vb_arm_record_buf, 0, sizeof(AUDDEV_RECORD_BUF));
    s_vb_arm_record_buf.tmp_buf = (uint16 *)AUDIO_ALLOC(buffer_size>>1);
    if(s_vb_arm_record_buf.tmp_buf == NULL){
        SCI_TRACE_LOW("armvb_as.c malloc failed %d\n", __LINE__);
        return AUDIO_NO_ENOUGH_MEMORY;
    }
    s_vb_arm_record_buf.data_ptr = (uint16 *)AUDIO_ALLOC(buffer_size+64);
    if(s_vb_arm_record_buf.data_ptr == NULL){
        SCI_TRACE_LOW("armvb_as.c malloc failed %d\n", __LINE__);
        SCI_FREE(s_vb_arm_record_buf.tmp_buf);
        return AUDIO_NO_ENOUGH_MEMORY;
    }
    SCI_MEMSET(s_vb_arm_record_buf.data_ptr, 0, buffer_size+64);
    s_vb_arm_record_buf.data_align_ptr =
        (int16*)ALIGN_UP_WITH_32(s_vb_arm_record_buf.data_ptr);
    s_vb_arm_record_buf.pWrite = 0;
    s_vb_arm_record_buf.pRead = 0;
    s_vb_arm_record_buf.size = buffer_size;

    SCI_TRACE_LOW("armvb_as.c buf:0x%x,0x%x,%d\n",
        s_vb_arm_record_buf.data_ptr,
        s_vb_arm_record_buf.data_align_ptr,
        s_vb_arm_record_buf.size);

    ret = AUDIO_HAL_ADC_Set_Buf_Info(
            s_vb_arm_record_buf.data_align_ptr, 0, s_vb_arm_record_buf.size);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Record_Play set input buf info err:%d\n",ret);
    }
/*
    AUDDEV_Record_Fadein_Prepare(&s_fadein_ctl,
                                s_auddev_recordinfo.samplerate,
                                1,  // readdata force to mono
                                RECORD_MUTE_TIME,
                                RECORD_FADEIN_TIME);
*/

    ret = AUDIO_HAL_Register_Capture((AUDIO_HAL_CALLBACK_FUN)AUDDEV_Record_Callback);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDIO_HAL_Register_Capture %d\n",ret);
    }

    arg.mute = 0;
    ret = AUDIO_HAL_Ioctl(AUDIO_CODEC_CMD_INPUT_MUTE_SET, (uint32)&arg);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Record_Play unmute err:%d\n",ret);
    }

#ifdef CVSRCD_ENABLE
    CVSRCD_DeInit();
    if (uiSampleRate == 8000) {
        CVSRCD_Init(mode_name, s_auddev_recordinfo.samplerate);
    }
#endif

    SCI_Sleep(1);

    ret = AUDIO_HAL_ADC_Trigger();
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Record_Play triger err:%d\n",ret);
    }

    s_b_armvb_recording = SCI_TRUE;
    //s_record_callback_count = 0;

    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//  Description:    Close device to stop record.
//  Author:
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E AUDDEV_Record_Stop (DEV_SETTING_T *ptDevSetting)
{
    AUDIO_RESULT_E ret = 0;

    SCI_TRACE_LOW("AUDDEV_Record_Stop \n");

    if (s_b_armvb_recording)
    {
        AUDIO_HAL_Close(AUDIO_HAL_CAPTURE);
        //SCI_ASSERT(0);
        if(s_vb_arm_record_buf.data_ptr)
        {
            SCI_FREE(s_vb_arm_record_buf.tmp_buf);
            s_vb_arm_record_buf.tmp_buf = PNULL;
            SCI_FREE(s_vb_arm_record_buf.data_ptr);
            s_vb_arm_record_buf.data_ptr = PNULL;
            s_vb_arm_record_buf.data_align_ptr = PNULL;
            s_vb_arm_record_buf.pWrite = 0;
            s_vb_arm_record_buf.pRead = 0;
        }

#ifdef CVSRCD_ENABLE
        CVSRCD_DeInit();
#endif
    }

    s_record_callback_count = 0;
    s_b_armvb_recording = SCI_FALSE;

    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//  Description:    Register device controlled by arm to audio service.
//  Author:
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV AUDDEV_Record_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

    SCI_TRACE_LOW("AUDDEV_Record_RegAudioDevice\n");
    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("AUDDEV_RECORD"), strlen ("AUDDEV_RECORD") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = TMP_FIFO_SIZE;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = AUDDEV_Record_Play;
    tDeviceInfo.tAudioDevOpe.pClose = AUDDEV_Record_Stop;
    tDeviceInfo.tAudioDevOpe.pPause = AUDDEV_Record_Stop;
    tDeviceInfo.tAudioDevOpe.pResume = AUDDEV_Record_Play;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;

    tDeviceInfo.tAudioDevOpe.pSetVolume = AUDDEV_Record_SetVolume;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = AUDDEV_Record_SetDevMode;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pRegRecCallback = AUDDEV_Record_RegRecCallback;
    tDeviceInfo.tAudioDevOpe.pUnRegRecCallback = AUDDEV_Record_UnRegRecCallback;
    tDeviceInfo.tAudioDevOpe.pReadData = AUDDEV_Record_ReadData;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = AUDDEV_GetAppropriateOutputSamplerate;
#ifdef AUD_ARM_RECORD_EXP
    tDeviceInfo.tAudioDevOpe.pPreClose = AUDDEV_Record_PreClose;
    tDeviceInfo.tAudioDevOpe.pPostOpen = AUDDEV_Record_PostOpen;
#endif

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);

    return hDevice;
}

LOCAL void AUDDEV_Uplink_ReadData(uint16 *puiDestData, uint32 uiCount, uint32 *puiDataLen)
{
    int i;

    SCI_ASSERT(uiCount <= s_vb_arm_record_buf.size/2);
    memcpy(puiDestData, s_vb_arm_record_buf.data_align_ptr+ s_vb_arm_record_buf.pRead/2, uiCount);

    *puiDataLen = uiCount;

    return;
}

LOCAL void AUDDEV_Downlink_WriteData(const uint16 *puiSrcData, uint32 uiCount, uint32 *puiDataLen)
{
    int16* dac_buffer;
    int i;

    SCI_ASSERT(uiCount <= s_vb_arm_record_buf.size/2);
    dac_buffer = (int16*)((uint32)s_armvb_output_buf.data_align_ptr +\
                            s_armvb_output_buf.pWrite);

    for (i=0; i<uiCount/2; i++)
    {
        dac_buffer[i] = MULT16_16_P14((int16)puiSrcData[i], s_auddev_playinfo.dg_level);
    }

    *puiDataLen = uiCount;
}

/*****************************************************************************/
//  Description:    uplink DMA Callback Reg to drv
//  Author:
//  Note:
//****************************************************************************/
void AUDDEV_Uplink_Callback (void)
{
    uint32 write_point = 0;

    if (s_uplink_callback)
    {
        s_ad_read_size = s_vb_arm_record_buf.size/2;//VB_CAPTURE_TRANS_SIZE;

        s_uplink_callback(s_ad_read_size);
    }
    else
    {
        SCI_TRACE_LOW("%s,%d, s_uplink_callback=Null",__FUNCTION__,__LINE__);
    }
    s_vb_arm_record_buf.pRead += s_vb_arm_record_buf.size/2; // 16bit + stereo
    if(s_vb_arm_record_buf.pRead >= s_vb_arm_record_buf.size){
        s_vb_arm_record_buf.pRead = 0;
    }
    AUDIO_HAL_ADC_Set_Read_Point(s_vb_arm_record_buf.pRead);
}

void AUDDEV_Downlink_Callback (void)
{
    if (s_downlink_callback)
    {
        s_downlink_callback();
    }
    else
    {
        SCI_TRACE_LOW("%s,%d, s_downlink_callback=Null",__FUNCTION__,__LINE__);
    }
    s_armvb_output_buf.pWrite += s_armvb_output_buf.size>>1;
    if(s_armvb_output_buf.pWrite >= s_armvb_output_buf.size){
        s_armvb_output_buf.pWrite = 0;
    }
    AUDIO_HAL_DAC_Set_Write_Point(s_armvb_output_buf.pWrite);
}

LOCAL void AUDDEV_Dual_SetUplinkMute (BOOLEAN is_mute)
{
    SCI_TRACE_LOW("%s %d\n",__FUNCTION__, is_mute);
    AUDDEV_SetUplinkMute(is_mute);
}

LOCAL void AUDDEV_Dual_SetDownlinkMute (BOOLEAN is_mute)
{
    SCI_TRACE_LOW("%s %d\n",__FUNCTION__, is_mute);
    AUDDEV_SetDownlinkMute(is_mute);
}

/*****************************************************************************/
//  Description:    AUDDEV RECORD logic device set device mode.
//  Author:
//  Note:
//****************************************************************************/
LOCAL void AUDDEV_Dual_SetDevMode (AUDIO_DEVICE_MODE_TYPE_E eMode)
{
    AUDDEV_SetDevMode(eMode);
    AUDDEV_Record_SetDevMode(eMode);
#ifdef VOICE_PROCESS_NEW
    voice_process_set_param(0, AUDIO_MODE_APP_TYPE_CUR_MODE, AUDIO_DEVICE_MODE_HANDFREE, AUDIO_VOLUME_CUR);
#endif
}

/*****************************************************************************/
//  Description:    AUDDEV Dual logic device set device volume.
//  Author:
//  Note:
//****************************************************************************/
LOCAL void AUDDEV_Dual_SetVolume (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType)
{
    s_auddev_playinfo.volume = uiVol;
    s_auddev_playinfo.app_type = eAppType;
    AUDDEV_SetVolume(uiVol, eAppType);          // spk Volume
    AUDDEV_Record_SetVolume(uiVol, eAppType);   // mic Volume
#ifdef VOICE_PROCESS_NEW
    voice_process_set_param(0, AUDIO_MODE_APP_TYPE_CUR_MODE, AUDIO_DEVICE_MODE_HANDFREE, uiVol);
#endif
}

LOCAL void AUDDEV_Dual_SetSamplerat (uint32 uiSampleRate)
{
    AUDIO_RESULT_E ret = 0;
    AUDIO_CODEC_PATH_T codecpath = {0};
    AUDIO_CTL_ARG_U arg;
    uint32 buffer_size;

    SCI_TRACE_LOW("%s entry,samplerate %d\n", __FUNCTION__,uiSampleRate);
    SCI_GetMutex (armvb_mutex, SCI_WAIT_FOREVER);
    if (s_auddev_dual_is_opened == SCI_FALSE)
    {
        SCI_TRACE_LOW("%s,%d device not opened\n", __FUNCTION__, __LINE__);
        SCI_PutMutex (armvb_mutex);
        return;
    }

    SCI_DisableIRQ();
    ret = AUDIO_HAL_Close(AUDIO_HAL_PLAYBACK);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c hal close err:%d",ret);
    }

    if (s_b_armvb_recording)
    {
        AUDIO_HAL_Close(AUDIO_HAL_CAPTURE);
        if(s_vb_arm_record_buf.data_ptr)
        {
            SCI_FREE(s_vb_arm_record_buf.data_ptr);
            s_vb_arm_record_buf.data_ptr = PNULL;
            s_vb_arm_record_buf.data_align_ptr = PNULL;
        }

        s_record_callback_count = 0;
        s_b_armvb_recording = SCI_FALSE;
    }
    SCI_RestoreIRQ();

    if ( s_armvb_output_buf.data_ptr ) {
        SCI_FREE(s_armvb_output_buf.data_ptr);
        s_armvb_output_buf.data_ptr = NULL;
        s_armvb_output_buf.data_align_ptr = NULL;
    }

    s_auddev_playinfo.chanelnum = AUDIO_CH_MONO;
    arg.channel = AUDIO_CH_MONO; // force stereo
    ret = AUDIO_HAL_Ioctl(AUDIO_DAI_CMD_CHANNEL_SET, (uint32)&arg);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Play AUDIO_DAI_CMD_CHANNEL_SET err:%d\n",ret);
    }

    s_auddev_playinfo.samplerate = uiSampleRate;
    ret = AUDIO_HAL_Open(AUDIO_HAL_PLAYBACK, uiSampleRate);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Play open AUDIO_HAL_PLAYBACK err:%d\n",ret);
    }

    // samplerate * 2chananel * 20ms * 16bit * buffernum / 1000
    buffer_size = (s_auddev_playinfo.samplerate *\
                    s_auddev_playinfo.chanelnum *\
                    20 * 2 * 2 ) / 1000;
    buffer_size = ALIGN_UP_WITH_32(buffer_size);

    SCI_MEMSET(&s_armvb_output_buf, 0, sizeof(AUDDEV_PLAYBACK_BUF));
    s_armvb_output_buf.data_ptr = (uint16 *)AUDIO_ALLOC(buffer_size+64);
    if(s_armvb_output_buf.data_ptr == NULL){
        SCI_PASSERT(0, ("armvb_as.c malloc failed"));
    }
    SCI_MEMSET(s_armvb_output_buf.data_ptr, 0, buffer_size+64);

    s_armvb_output_buf.data_align_ptr =
        (int16*)ALIGN_UP_WITH_32(s_armvb_output_buf.data_ptr);
    s_armvb_output_buf.pRead = 0;   // point that hw read
    s_armvb_output_buf.pWrite = 0;   // point that device write
    s_armvb_output_buf.size = buffer_size;
    SCI_TRACE_LOW("armvb_as.c buf:0x%x,0x%x,%d\n",
        s_armvb_output_buf.data_ptr,
        s_armvb_output_buf.data_align_ptr,
        s_armvb_output_buf.size);

    ret = AUDIO_HAL_DAC_Set_Buf_Info(
            s_armvb_output_buf.data_align_ptr, 0, s_armvb_output_buf.size);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Play set buffer info err:%d\n",ret);
    }

    ret = AUDIO_HAL_Register_Playback((AUDIO_HAL_CALLBACK_FUN)AUDDEV_Downlink_Callback);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Play Register_Playback err:%d\n",ret);
    }

    if (s_b_armvb_recording){
        SCI_TRACE_LOW("AUDDEV_Record_Play device id busy\n");
    }

    s_auddev_recordinfo.chanelnum = AUDIO_CH_MONO;        // force mono from dm
    arg.channel = AUDIO_CH_MONO;                          // force stereo for dev
    ret = AUDIO_HAL_Ioctl(AUDIO_DAI_CMD_CHANNEL_SET, (uint32)&arg);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Record_Play set channel err:%d\n",ret);
    }

    s_auddev_recordinfo.samplerate = uiSampleRate;
    ret = AUDIO_HAL_Open(AUDIO_HAL_CAPTURE, uiSampleRate);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Record_Play set samplerate err:%d\n",ret);
    }

    AUDDEV_Dual_SetDownlinkMute(s_auddev_playinfo.mute);//bDownlink_mute
    AUDDEV_SetVolume(s_auddev_playinfo.volume, s_auddev_playinfo.app_type);          // spk Volume
    AUDDEV_SetDevMode(AUDIO_DEVICE_MODE_HANDFREE);
    AUDDEV_Record_SetVolume(s_auddev_playinfo.volume, s_auddev_playinfo.app_type);   // mic Volume
    AUDDEV_Dual_SetUplinkMute(s_auddev_recordinfo.mute);
    AUDDEV_Record_SetDevMode(AUDIO_DEVICE_MODE_HANDFREE);

    // samplerate * ch * 16bit * 20ms * buffernum / 1000;
    buffer_size = (uiSampleRate * s_auddev_recordinfo.chanelnum * 2 * 20 * 2) / 1000;
    buffer_size = ALIGN_UP_WITH_32(buffer_size);

    SCI_MEMSET(&s_vb_arm_record_buf, 0, sizeof(AUDDEV_RECORD_BUF));
    s_vb_arm_record_buf.data_ptr = (uint16 *)AUDIO_ALLOC(buffer_size+64);
    if(s_vb_arm_record_buf.data_ptr == NULL){
        SCI_PASSERT(0, ("armvb_as.c malloc failed"));
    }
    SCI_MEMSET(s_vb_arm_record_buf.data_ptr, 0, buffer_size+64);
    s_vb_arm_record_buf.data_align_ptr =
        (int16*)ALIGN_UP_WITH_32(s_vb_arm_record_buf.data_ptr);
    s_vb_arm_record_buf.pWrite = 0;
    s_vb_arm_record_buf.pRead = 0;
    s_vb_arm_record_buf.size = buffer_size;

    SCI_TRACE_LOW("armvb_as.c buf:0x%x,0x%x,%d\n",
        s_vb_arm_record_buf.data_ptr,
        s_vb_arm_record_buf.data_align_ptr,
        s_vb_arm_record_buf.size);

    ret = AUDIO_HAL_ADC_Set_Buf_Info(
            s_vb_arm_record_buf.data_align_ptr, 0, s_vb_arm_record_buf.size);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Record_Play set input buf info err:%d\n",ret);
    }

    ret = AUDIO_HAL_Register_Capture((AUDIO_HAL_CALLBACK_FUN)AUDDEV_Uplink_Callback);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDIO_HAL_Register_Capture %d\n",ret);
    }

    AUDIO_HAL_DAC_Trigger();
    ret = AUDIO_HAL_ADC_Trigger();

    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Record_Play triger err:%d\n",ret);
    }

    s_b_armvb_recording = SCI_TRUE;
    SCI_PutMutex (armvb_mutex);
}

LOCAL AUDIO_RESULT_E AUDDEV_Dual_Play (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
    AUDIO_RESULT_E result = 0;
    int32 ret = AUDIO_HAL_SUCCESS;
    uint32 buffer_size;
    AUDIO_CTL_ARG_U arg;
    char * mode_name = NULL;
    uint32 voiceCoderType = 0;
    AUDIO_DEVICE_MODE_TYPE_E cur_eDev_mode = ptDevSetting->eDev_mode;
    AUDIO_CODEC_PATH_T codecpath = {0};

    SCI_TRACE_LOW("%s,%d entry,samplerate %d; ch %d\n",
        __FUNCTION__, __LINE__,uiSampleRate,ptDevSetting->eChannelChoice);

    SCI_GetMutex (armvb_mutex, SCI_WAIT_FOREVER);
    if (s_auddev_dual_is_opened)
    {
        SCI_PutMutex (armvb_mutex);
        return AUDIO_HAL_SUCCESS;
    }
    uiSampleRate = VoiceProcess_get_samplerate(uiSampleRate);
    s_auddev_playinfo.chanelnum = AUDIO_CH_MONO;
    arg.channel = AUDIO_CH_MONO;
    ret = AUDIO_HAL_Ioctl(AUDIO_DAI_CMD_CHANNEL_SET, (uint32)&arg);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Play AUDIO_DAI_CMD_CHANNEL_SET err:%d\n",ret);
    }

    s_auddev_playinfo.samplerate = uiSampleRate;
    AUDDEV_Dual_SetDevMode(AUDIO_DEVICE_MODE_HANDFREE);
    ret = AUDIO_HAL_Open(AUDIO_HAL_PLAYBACK, uiSampleRate);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Play open AUDIO_HAL_PLAYBACK err:%d\n",ret);
    }

    // samplerate * 2chananel * 20ms * 16bit * buffernum / 1000
    buffer_size = (s_auddev_playinfo.samplerate *\
                    s_auddev_playinfo.chanelnum *\
                    20 * 2 * 2 ) / 1000;
    buffer_size = ALIGN_UP_WITH_32(buffer_size);

    SCI_MEMSET(&s_armvb_output_buf, 0, sizeof(AUDDEV_PLAYBACK_BUF));
    s_armvb_output_buf.data_ptr = (uint16 *)AUDIO_ALLOC(buffer_size+64);
    if(s_armvb_output_buf.data_ptr == NULL){
        SCI_PutMutex (armvb_mutex);
        SCI_TRACE_LOW("armvb_as.c malloc failed %d\n", __LINE__);
        return AUDIO_NO_ENOUGH_MEMORY;
    }
    SCI_MEMSET(s_armvb_output_buf.data_ptr, 0, buffer_size+64);

    s_armvb_output_buf.data_align_ptr =
        (int16*)ALIGN_UP_WITH_32(s_armvb_output_buf.data_ptr);
    s_armvb_output_buf.pRead = s_armvb_output_buf.size/2;   // point that hw read
    s_armvb_output_buf.pWrite = s_armvb_output_buf.size/2;   // point that device write
    s_armvb_output_buf.size = buffer_size;
    SCI_TRACE_LOW("armvb_as.c buf:0x%x,0x%x,%d\n",
        s_armvb_output_buf.data_ptr,
        s_armvb_output_buf.data_align_ptr,
        s_armvb_output_buf.size);

    ret = AUDIO_HAL_DAC_Set_Buf_Info(
            s_armvb_output_buf.data_align_ptr, 0, s_armvb_output_buf.size);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_PutMutex (armvb_mutex);
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Play set buffer info err:%d\n",ret);
        return ret;
    }

    ret = AUDIO_HAL_Register_Playback((AUDIO_HAL_CALLBACK_FUN)AUDDEV_Downlink_Callback);
    if(AUDIO_HAL_SUCCESS != ret){
         SCI_PutMutex (armvb_mutex);
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Play Register_Playback err:%d\n",ret);
        return ret;
    }

    if (s_b_armvb_recording){
        SCI_PutMutex (armvb_mutex);
        SCI_TRACE_LOW("AUDDEV_Record_Play device id busy\n");
        return AUDIO_HAL_SUCCESS;
    }

    s_auddev_recordinfo.chanelnum = AUDIO_CH_MONO;        // force mono from dm
    arg.channel = AUDIO_CH_MONO;                          // force stereo for dev
    ret = AUDIO_HAL_Ioctl(AUDIO_DAI_CMD_CHANNEL_SET, (uint32)&arg);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Record_Play set channel err:%d\n",ret);
    }

    s_auddev_recordinfo.samplerate = uiSampleRate;
    ret = AUDIO_HAL_Open(AUDIO_HAL_CAPTURE, uiSampleRate);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Record_Play set samplerate err:%d\n",ret);
    }

    AUDDEV_Dual_SetDownlinkMute(1);//bDownlink_mute
    AUDDEV_Dual_SetDevMode(ptDevSetting->eDev_mode);
    AUDDEV_Dual_SetVolume(ptDevSetting->uiVol, ptDevSetting->eAppType);
    AUDDEV_Dual_SetUplinkMute(ptDevSetting->bUplink_mute);

    // samplerate * ch * 16bit * 20ms * buffernum / 1000;
    buffer_size = (uiSampleRate * s_auddev_recordinfo.chanelnum * 2 * 20 * 2) / 1000;
    buffer_size = ALIGN_UP_WITH_32(buffer_size);

    SCI_MEMSET(&s_vb_arm_record_buf, 0, sizeof(AUDDEV_RECORD_BUF));
    s_vb_arm_record_buf.data_ptr = (uint16 *)AUDIO_ALLOC(buffer_size+64);
    if(s_vb_arm_record_buf.data_ptr == NULL){
        SCI_PutMutex (armvb_mutex);
        SCI_FREE(s_armvb_output_buf.data_ptr);
        SCI_TRACE_LOW("armvb_as.c malloc failed %d\n", __LINE__);
        return AUDIO_NO_ENOUGH_MEMORY;
    }
    SCI_MEMSET(s_vb_arm_record_buf.data_ptr, 0, buffer_size+64);
    s_vb_arm_record_buf.data_align_ptr =
        (int16*)ALIGN_UP_WITH_32(s_vb_arm_record_buf.data_ptr);
    s_vb_arm_record_buf.pWrite = 0;
    s_vb_arm_record_buf.pRead = 0;
    s_vb_arm_record_buf.size = buffer_size;

    SCI_TRACE_LOW("armvb_as.c buf:0x%x,0x%x,%d\n",
        s_vb_arm_record_buf.data_ptr,
        s_vb_arm_record_buf.data_align_ptr,
        s_vb_arm_record_buf.size);

    ret = AUDIO_HAL_ADC_Set_Buf_Info(
            s_vb_arm_record_buf.data_align_ptr, 0, s_vb_arm_record_buf.size);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Record_Play set input buf info err:%d\n",ret);
    }

    ret = AUDIO_HAL_Register_Capture((AUDIO_HAL_CALLBACK_FUN)AUDDEV_Uplink_Callback);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDIO_HAL_Register_Capture %d\n",ret);
    }

    AUDIO_HAL_DAC_Trigger();
    ret = AUDIO_HAL_ADC_Trigger();

    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_Record_Play triger err:%d\n",ret);
    }

    AUDDEV_Dual_SetDownlinkMute(ptDevSetting->bDownlink_mute);//bDownlink_mute
    s_b_armvb_recording = SCI_TRUE;
    s_auddev_dual_is_opened = SCI_TRUE;
    //s_record_callback_count = 0;
    SCI_PutMutex (armvb_mutex);

    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//  Description:    Close device to stop record.
//  Author:
//  Note:
//****************************************************************************/
LOCAL AUDIO_RESULT_E AUDDEV_Dual_Stop (DEV_SETTING_T *ptDevSetting)
{
    AUDIO_RESULT_E ret = 0;

    SCI_TRACE_LOW("AUDDEV_Dual_Stop");

    SCI_GetMutex (armvb_mutex, SCI_WAIT_FOREVER);
    ret = AUDIO_HAL_Close(AUDIO_HAL_PLAYBACK);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c hal close err:%d",ret);
    }

    if (s_b_armvb_recording)
    {
        AUDIO_HAL_Close(AUDIO_HAL_CAPTURE);
        if(s_vb_arm_record_buf.data_ptr)
        {
            SCI_FREE(s_vb_arm_record_buf.data_ptr);
            s_vb_arm_record_buf.data_ptr = PNULL;
            s_vb_arm_record_buf.data_align_ptr = PNULL;
        }

        s_record_callback_count = 0;
        s_b_armvb_recording = SCI_FALSE;
    }

    if ( s_armvb_output_buf.data_ptr ) {
        SCI_FREE(s_armvb_output_buf.data_ptr);
        s_armvb_output_buf.data_ptr = NULL;
        s_armvb_output_buf.data_align_ptr = NULL;
    }
    s_auddev_dual_is_opened = SCI_FALSE;
    SCI_PutMutex (armvb_mutex);

    return AUDIO_NO_ERROR;
}

/*****************************************************************************/
//  Description:    AUDDEV DUAL logic device reg uplink callback
//  Author:
//  Note:
//****************************************************************************/
LOCAL uint32 AUDDEV_Dual_RegUplinkCallback (
    ADM_RECCALLBACK pCallback
)
{
    SCI_TRACE_LOW("%s,%d\n",__FUNCTION__,__LINE__);

    // Find the vacant position and save it.
    if(s_uplink_callback == SCI_NULL)
    {
        s_uplink_callback = pCallback;
        return (uint32)s_uplink_callback;
    }

    return (uint32)SCI_NULL;
}

/*****************************************************************************/
//  Description:    AUDDEV DUAL logic device unreg Uplink callback
//  Author:
//  Note:
//****************************************************************************/
LOCAL void AUDDEV_Dual_UnRegUplinkCallback (
    uint32 callback_vector
)
{
    SCI_TRACE_LOW("%s,%d\n",__FUNCTION__,__LINE__);

    // Find the occupied position by callback function and clear it.
    if((uint32)s_uplink_callback == callback_vector)
    {
        s_uplink_callback = SCI_NULL;
    }

     return;
}

/*****************************************************************************/
//  Description:    AUDDEV DUAL logic device reg downlink callback
//  Author:
//  Note:
//****************************************************************************/
LOCAL uint32 AUDDEV_Dual_RegDownlinkCallback (
    ADM_RECCALLBACK pCallback
)
{
    SCI_TRACE_LOW("%s,%d\n",__FUNCTION__,__LINE__);

    // Find the vacant position and save it.
    if(s_downlink_callback == SCI_NULL)
    {
        s_downlink_callback = pCallback;
        return (uint32)s_downlink_callback;
    }

    return (uint32)SCI_NULL;
}

/*****************************************************************************/
//  Description:    AUDDEV DUAL logic device unreg Downlink callback
//  Author:
//  Note:
//****************************************************************************/
LOCAL void AUDDEV_Dual_UnRegDownlinkCallback (
    uint32 callback_vector
)
{
    SCI_TRACE_LOW("%s,%d\n",__FUNCTION__,__LINE__);

    // Find the occupied position by callback function and clear it.
    if((uint32)s_downlink_callback == callback_vector)
    {
        s_downlink_callback = SCI_NULL;
    }

     return;
}


/*****************************************************************************/
//  Description:    Register device controlled by arm to audio service.
//  Author:
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV AUDDEV_Dual_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

    SCI_TRACE_LOW("AUDDEV_Dual_RegAudioDevice\n");
    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("AUDDEV_Dual"), strlen ("AUDDEV_Dual") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = TMP_FIFO_SIZE;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = AUDDEV_Dual_Play;
    tDeviceInfo.tAudioDevOpe.pClose = AUDDEV_Dual_Stop;
    tDeviceInfo.tAudioDevOpe.pPause = AUDDEV_Dual_Stop;
    tDeviceInfo.tAudioDevOpe.pResume = AUDDEV_Dual_Play;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;

    tDeviceInfo.tAudioDevOpe.pSetVolume = AUDDEV_Dual_SetVolume;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = AUDDEV_Dual_SetDevMode;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = AUDDEV_Dual_SetUplinkMute;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = AUDDEV_Dual_SetDownlinkMute;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pRegRecCallback = AUDDEV_Dual_RegUplinkCallback;
    tDeviceInfo.tAudioDevOpe.pUnRegRecCallback = AUDDEV_Dual_UnRegUplinkCallback;
    tDeviceInfo.tAudioDevOpe.pRegPlayCallback = AUDDEV_Dual_RegDownlinkCallback;
    tDeviceInfo.tAudioDevOpe.pUnRegPlayCallback = AUDDEV_Dual_UnRegDownlinkCallback;
    tDeviceInfo.tAudioDevOpe.pReadData = AUDDEV_Uplink_ReadData;
    tDeviceInfo.tAudioDevOpe.pWriteData = AUDDEV_Downlink_WriteData;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = AUDDEV_GetAppropriateOutputSamplerate;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = AUDDEV_Dual_SetSamplerat;

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);

    AUDIO_HAL_Register_Capture((AUDIO_HAL_CALLBACK_FUN)AUDDEV_Uplink_Callback);

    armvb_mutex = SCI_CreateMutex ("AUDDEV_Dual", SCI_INHERIT);
    if (!armvb_mutex) {
        SCI_TRACE_LOW("armvb_mutex create fail");
    }

#ifdef VOICE_PROCESS_NEW
	SCI_TRACE_LOW("voice_process_init START");
	voice_process_init();
#endif

    return hDevice;
}

//****************************************************************************/
// tone play device
//****************************************************************************/

LOCAL AUDIO_RESULT_E AUDDEV_Tone_Play (uint32 uiSampleRate, DEV_SETTING_T *ptDevSetting)
{
    AUDIO_RESULT_E ret = 0;
    AUDIO_CTL_ARG_U arg;
    AUDIO_CODEC_PATH_T codecpath = {0};

    arg.mute = 1;
    ret = AUDIO_HAL_Ioctl(AUDIO_CODEC_CMD_INPUT_MUTE_SET, (uint32)&arg);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_tone_play mute err:%d\n",ret);
    }
    arg.channel = 1;//stream->channelNb;
    ret = AUDIO_HAL_Ioctl(AUDIO_DAI_CMD_CHANNEL_SET, (uint32)&arg);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_tone_play set channel err:%d\n",ret);
    }

    codecpath.device = AUDIO_SPK;
    codecpath.gain1 = 0xbf;
    codecpath.gain2 = 0;
    codecpath.mute = 0;
    arg.addr_path = (uint32)&codecpath;
    ret = AUDIO_HAL_Ioctl(AUDIO_CODEC_CMD_OUTPUT_PATH_SET, (uint32)&arg);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_tone_play set input path err:%d\n",ret);
    }
/*
    audio-dai will set a buffer when did not set AUDIO_HAL_DAC_Set_Buf_Info
*/
    //AUDIO_HAL_DAC_Set_Buf_Info(stream->startAddress,0,stream->length);

    if(8000 == uiSampleRate){
        if (NULL == s_vb_da0_buffer_multitrack) {
            s_vb_da0_buffer_multitrack = AUDIO_ALLOC(MULTITRACK_BUFFER_SIZE);
            if (NULL == s_vb_da0_buffer_multitrack) {
                SCI_TRACE_LOW("armvb_as.c malloc failed %d\n", __LINE__);
                return AUDIO_NO_ENOUGH_MEMORY;
            }
        }

        if (NULL == s_vb_da1_buffer_multitrack) {
            s_vb_da1_buffer_multitrack = AUDIO_ALLOC(MULTITRACK_BUFFER_SIZE);
            if (NULL == s_vb_da1_buffer_multitrack) {
                SCI_TRACE_LOW("armvb_as.c malloc failed %d\n", __LINE__);
                SCI_FREE(s_vb_da0_buffer_multitrack);
                s_vb_da0_buffer_multitrack = NULL;
                return AUDIO_NO_ENOUGH_MEMORY;
            }
        }

        ret = AUDIO_HAL_Register_Playback((AUDIO_HAL_CALLBACK_FUN)AUDDEV_PLAYBACK_Callback);
        if(AUDIO_HAL_SUCCESS != ret){
            SCI_TRACE_LOW("armvb_as.c AUDDEV_tone_play Register_Playback err:%d\n",ret);
            return ret;
        }
    }

    ret = AUDIO_HAL_Open(AUDIO_HAL_TONE, 8000);
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_tone_play open AUDIO_HAL_PLAYBACK err:%d\n",ret);
    }

    arg.tone.mBits.gain = AUDIO_AIF_TONE_GAIN_M9_DB;
    arg.tone.mBits.type = AUDIO_AIF_DTMF_0;//ptDevSetting->usKeyTone;//AUDIO_AIF_DTMF_0;
    arg.tone.mBits.cmd = TONE_CMD_START;
    AUDIO_HAL_Ioctl(AUDIO_DAI_CMD_TONE, (uint32)&arg);

    ret = AUDIO_HAL_DAC_Trigger();
    if(AUDIO_HAL_SUCCESS != ret){
        SCI_TRACE_LOW("armvb_as.c AUDDEV_tone_play Trigger err:%d\n",ret);
        return ret;
    }


    //osiDelayUS(10000);

    return AUDIO_NO_ERROR;
}

LOCAL AUDIO_RESULT_E AUDDEV_Tone_Stop (DEV_SETTING_T *ptDevSetting)
{
    AUDIO_RESULT_E ret = 0;
    AUDIO_CTL_ARG_U arg;
    AUDIO_CODEC_PATH_T codecpath = {0};

    arg.tone.mBits.gain = AUDIO_AIF_TONE_GAIN_M9_DB;
    arg.tone.mBits.type = ptDevSetting->usKeyTone;//AUDIO_AIF_DTMF_0;
    arg.tone.mBits.cmd = TONE_CMD_STOP;
    AUDIO_HAL_Ioctl(AUDIO_DAI_CMD_TONE, (uint32)&arg);

    AUDIO_HAL_Close(AUDIO_HAL_TONE);
    if (s_vb_da0_buffer_multitrack) {
        SCI_FREE(s_vb_da0_buffer_multitrack);
        s_vb_da0_buffer_multitrack = NULL;
    }

    if (s_vb_da1_buffer_multitrack) {
        SCI_FREE(s_vb_da1_buffer_multitrack);
        s_vb_da1_buffer_multitrack = NULL;
    }

    return AUDIO_NO_ERROR;
}

LOCAL void AUDDEV_Tone_SetDevMode (AUDIO_DEVICE_MODE_TYPE_E eMode)
{
    AUDDEV_SetDevMode(eMode);
    return;
}

LOCAL void AUDDEV_Tone_SetVolume (uint32  uiVol, AUDIO_ARM_APP_TYPE_E eAppType)
{


    SCI_TRACE_LOW("%s,%s,%d \n", __FILE__,__FUNCTION__,__LINE__);

    return ;
}

/*****************************************************************************/
//  Description:    Register device controlled by arm to audio service.
//  Author:
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV AUDDEV_Tone_RegAudioDevice (void)
{
    AUDIO_DEVICE_T tDeviceInfo = {0};
    HAUDIODEV hDevice = INVALID_HANDLE;

    SCI_TRACE_LOW("AUDDEV_Tone_RegAudioDevice\n");
    SCI_MEM16CPY ( (uint16 *) tDeviceInfo.usDeviceName, ANSI2UINT16 ("AUDDEV_TONE"), strlen ("AUDDEV_TONE") * 2 + 2);
    tDeviceInfo.bIsNeedTrack = SCI_FALSE;
    tDeviceInfo.uiMinFetchLength = TMP_FIFO_SIZE;
    tDeviceInfo.ucDevOpenCnt = 0;
    tDeviceInfo.tAudioDevOpe.pOpen = AUDDEV_Tone_Play;
    tDeviceInfo.tAudioDevOpe.pClose = AUDDEV_Tone_Stop;
    tDeviceInfo.tAudioDevOpe.pPause = AUDDEV_Tone_Stop;
    tDeviceInfo.tAudioDevOpe.pResume = AUDDEV_Tone_Play;
    tDeviceInfo.tAudioDevOpe.pSetSamplerate = SCI_NULL;

    tDeviceInfo.tAudioDevOpe.pSetVolume = AUDDEV_Tone_SetVolume;
    tDeviceInfo.tAudioDevOpe.pSetDevMode = AUDDEV_Tone_SetDevMode;
    tDeviceInfo.tAudioDevOpe.pSetDigitalGain = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetUplinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDownlinkMute = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pSetDepopMute=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pRegRecCallback = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pUnRegRecCallback = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pReadData = SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmCount=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetRemainedPcmTime=SCI_NULL;
    tDeviceInfo.tAudioDevOpe.pGetAppropriateOutputSamplerate = AUDDEV_GetAppropriateOutputSamplerate;

    hDevice = AUDIO_DM_RegAudioDevice (&tDeviceInfo);

    return hDevice;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif  // End of asm_hc.c

