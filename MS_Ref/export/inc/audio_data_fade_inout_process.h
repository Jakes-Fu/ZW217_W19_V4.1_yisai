/******************************************************************************
 ** File Name:      audio_data_fade_inout_process.c                                                 *
 ** Author:         Shujing.Dong                                          *
 ** DATE:           04/11/2012                                          *
 ** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines interfaces to process data to fade in and out   *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/11/2012  Shujing.Dong    Create.                                       *
 ** 09/23/2022  Rui.huang       add to w217.                                   *
 ******************************************************************************/
#ifndef _AUDIO_DATA_FADE_INOUT_PROCESS_H
#define _AUDIO_DATA_FADE_INOUT_PROCESS_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "audio_api.h"

#ifdef __cplusplus
    extern   "C"
    {
#endif

#define FADE_STATE_NORMAL 0
#define FADE_STATE_IN     1
#define FADE_STATE_OUT  2

#define AUDIO_DATA_FADE_INOUT_PROCESS_INVALID_HANDLE 0xFFFFFFFF

typedef struct
{
    uint16 usFadeInOutHandle; //[OUT] fadeinout handle, valid: 0--15; invalid:AUDIO_DATA_FADE_INOUT_PROCESS_INVALID_HANDLE
    uint16 usFadeInSamples;//[OUT] samples in fading in.
    uint16 usFadeOutSamples;//[OUT] samples in fading out.
}AUDIO_DATA_FADE_INOUT_CONFIG_T;

typedef struct
{
    uint16 usFadeInOutHandle;//return value of AudioDataFadeInOut_Construct
    uint16 res0;
    uint32 uiState;// FADE_STATE_NORMAL or FADE_STATE_IN or FADE_STATE_OUT, default value in construct: FADE_STATE_NORMAL
    int16 *sSrcDataLeft;
    int16 *sSrcDataRight;
    int16 *sDestDataLeft;
    int16 *sDestDataRight;
    uint32 uiDataLen; // unit: words(2bytes)
}AUDIO_DATA_FADE_INOUT_INFO_T;


PUBLIC AUDIO_DATA_FADE_INOUT_CONFIG_T* AudioDataFadeInOut_Construct(
    uint32 sapmle_rate);

PUBLIC void AudioDataFadeInOut_Deconstruct(
    AUDIO_DATA_FADE_INOUT_CONFIG_T** pptConfig);

PUBLIC AUDIO_RESULT_E AudioDataFadeInOut_Process(
    AUDIO_DATA_FADE_INOUT_INFO_T *ptDataInfo);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif  // __AUDIO_DATA_FADE_INOUT_PROCESS_H

// End of audio_data_fade_inout_process.h
