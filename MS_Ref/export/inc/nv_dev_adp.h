/*
 * <nv_dev_adp.h> - <nv adapter layer.>
 *
 * Copyright (c) 2019 Unisoc Communications Inc.
 * History
 *      <2011/01/16> <shujing.dong> <creating>
 *      <2019/11/11> <elon li> <refactoring>
 *      This file defines corresponding relations between nv and logic device..
 */

#ifndef _NV_DEV_ADP_H_
#define _NV_DEV_ADP_H_

#include "asm.h"
#include "audio_api.h"
#include "audio_nv_arm.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32 NV_DEV_ADP_E;
typedef uint32 NV_DSP_TYPE_E;

typedef enum {
	NV_DEV_ADP_LAYER1,
	NV_DEV_ADP_RECORD,
	NV_DEV_ADP_MP4RECORD,
	NV_DEV_ADP_LOOPBACK,
	NV_DEV_ADP_MAX
} TypeNvDevType_e;
typedef uint32 NV_DEV_TYPE_E;

typedef enum {
	NV_NXP_APP_TYPE_VOICE,
	NV_NXP_APP_TYPE_LOOPBACK,
	NV_NXP_APP_TYPE_VOIP,
	NV_DEV_APP_TYPE_MAX
} TypeNvNxpAppType_e;
typedef uint32 NV_NXP_APP_TYPE_E;

typedef enum {
	NV_CVS_APP_TYPE_VOICE,
	NV_CVS_APP_TYPE_LOOPBACK,
	NV_CVS_APP_TYPE_VOIP,
	NV_DEV_APP_TYPE_MAX_CVS
} TypeNvCvsAppType_e;
typedef uint32 NV_CVS_APP_TYPE_E;

typedef enum
{
    NV_DSP_TYPE_GSM,
    NV_DSP_TYPE_TD,
    NV_DSP_TYPE_WB,
    NV_DSP_TYPE_SWB,
    NV_DSP_TYPE_MAX
} TypeNvDspType_e;
typedef uint32 NV_DSP_TYPE_E;

#if 0
PUBLIC AUDIO_NV_DSP_RESULT_E NV_DEV_LOOPBACK_GetDspCodecModeParam (
	AUDIO_DEVICE_MODE_TYPE_E eDev_mode,
	AUDIO_NV_DSP_MODE_INFO_T *ptAudMode
);
#endif
PUBLIC char *NV_DEV_GetValidDspAudioNvName (
	NV_DEV_ADP_E eNvDevAdp,
	AUDIO_DEVICE_MODE_TYPE_E eDev_mode,
	uint16 flag //0: gsm, 1: td.
);



#ifdef __cplusplus
}
#endif

#endif //end of _NV_DEV_ADP_H_
//end of file
