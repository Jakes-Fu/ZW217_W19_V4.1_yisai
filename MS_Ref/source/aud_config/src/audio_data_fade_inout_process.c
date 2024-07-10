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
 ** 04/11/2012  Shujing.Dong    Create.                                   *
 ** 09/23/2022  Rui.huang       add to w217.                                   *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "audio_data_fade_inout_process.h"
//#include "audio_nv_arm.h"
#include "asm.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif

#define AUDIO_DATA_FADE_INOUT_PROCESS_MAX 16
/*
#define AUDIO_DATA_FADE_INOUT_INPUT_GAIN_NV 43
#define AUDIO_DATA_FADE_INOUT_OUTPUT_GAIN_NV 44
#define AUDIO_DATA_FADE_INOUT_MAX_GAIN_NV 47
*/

typedef struct
{
    BOOLEAN bIsUsed;
    uint8 res0;
    uint16 usFadeInOutHandle;
    uint16 usCurFadeInGain;
    uint16 usCurFadeOutGain;
    uint32 uiState;// FADE_STATE_NORMAL or FADE_STATE_IN or FADE_STATE_OUT, default value in construct: FADE_STATE_NORMAL
}AUDIO_DATA_FADE_INOUT_PROCESS_T;

AUDIO_DATA_FADE_INOUT_PROCESS_T tAudioDataFadeInoutProcess[AUDIO_DATA_FADE_INOUT_PROCESS_MAX] = {0};


LOCAL BOOLEAN bGainInfoConfig = SCI_FALSE;

//config of fade in and fade out
#define AUDIO_DATA_NORMALIZING_VALUE 4096
LOCAL uint16 usNormalizingFadeInStep = 0;
LOCAL uint16 usNormalizingFadeOutStep = 0;
LOCAL uint16 usNormalizingFadeInOutMax = 0;

PUBLIC AUDIO_DATA_FADE_INOUT_CONFIG_T* AudioDataFadeInOut_Construct(
    uint32 sample_rate)
{
    uint32 i=0;
    AUDIO_DATA_FADE_INOUT_CONFIG_T *ptConfig = PNULL;

    // fade inout init
    ptConfig =\
        (AUDIO_DATA_FADE_INOUT_CONFIG_T *)SCI_ALLOC(sizeof(AUDIO_DATA_FADE_INOUT_CONFIG_T));
    if (PNULL == ptConfig){
        SCI_TRACE_LOW("[FADE] AudioDataFadeInOut_Construct cfg malloc error");
        return PNULL;
    }

    if(!bGainInfoConfig)
    {
/*
            NV_AUDIO_ARM_E arm_nv_id =  NV_AUDIO_ARM_HANDSET_E;
            char *mode_name = NULL;
            AUDIO_NV_ARM_MODE_INFO_T *ptAudMode = PNULL;
            uint16 usNvConfigToFiles;

            ptAudMode = (AUDIO_NV_ARM_MODE_INFO_T *)AUDIO_ALLOC(sizeof(AUDIO_NV_ARM_MODE_INFO_T));
            if(PNULL==ptAudMode)
            {
                SCI_TRACE_LOW("[FADE] AudioDataFadeInOut_Construct, alloc fail, size: %d",sizeof(AUDIO_NV_ARM_MODE_INFO_T));
                return AUDIO_ERROR;
            }

            mode_name = AUDIONVARM_GetAudioModeNameById(arm_nv_id);
            if(PNULL==mode_name)
            {
                SCI_TRACE_LOW("[FADE] AudioDataFadeInOut_Construct arm_nv_id:%d.", arm_nv_id);
                SCI_FREE(ptAudMode);
                return AUDIO_ERROR;
            }

            if(AUDIO_NV_ARM_NO_ERROR != AUDIONVARM_GetModeParam((const char *) mode_name, (AUDIO_NV_ARM_MODE_INFO_T *)(ptAudMode)))
            {
                SCI_FREE(ptAudMode);
                SCI_TRACE_LOW("[FADE] AudioDataFadeInOut_Construct failed to get mode param.");
                return AUDIO_ERROR;
            }

            usNormalizingFadeInStep = ptAudMode->tAudioNvArmModeStruct.reserve[AUDIO_DATA_FADE_INOUT_INPUT_GAIN_NV];
            usNormalizingFadeOutStep = ptAudMode->tAudioNvArmModeStruct.reserve[AUDIO_DATA_FADE_INOUT_OUTPUT_GAIN_NV];
            usNormalizingFadeInOutMax = ptAudMode->tAudioNvArmModeStruct.reserve[AUDIO_DATA_FADE_INOUT_MAX_GAIN_NV];

            SCI_FREE(ptAudMode);
*/

            usNormalizingFadeInStep = 0x8;      //bug 2119654
            usNormalizingFadeOutStep = 0x20;
            usNormalizingFadeInOutMax = 0x1000;
            if((0==usNormalizingFadeInStep) ||(0==usNormalizingFadeOutStep))
            {
                SCI_TRACE_LOW("[FADE] AudioDataFadeInOut_Construct step config error, %d, %d.", usNormalizingFadeInStep, usNormalizingFadeOutStep);
                SCI_FREE(ptConfig);
                ptConfig = PNULL;
                return PNULL;
            }

            bGainInfoConfig = SCI_TRUE;
    }

    for(i=0; i<AUDIO_DATA_FADE_INOUT_PROCESS_MAX; i++)
    {
        if(!tAudioDataFadeInoutProcess[i].bIsUsed)
        {
            SCI_MEMSET((void *)&(tAudioDataFadeInoutProcess[i]), 0, sizeof(AUDIO_DATA_FADE_INOUT_PROCESS_T));
            tAudioDataFadeInoutProcess[i].bIsUsed = SCI_TRUE;
            tAudioDataFadeInoutProcess[i].usCurFadeInGain = 0;
            tAudioDataFadeInoutProcess[i].usCurFadeOutGain = usNormalizingFadeInOutMax;
            ptConfig->usFadeInOutHandle = i;
            ptConfig->usFadeInSamples = (usNormalizingFadeInOutMax/usNormalizingFadeInStep)*sample_rate/8000;
            ptConfig->usFadeOutSamples = (usNormalizingFadeInOutMax/usNormalizingFadeOutStep)*sample_rate/8000;
            SCI_TRACE_LOW("[FADE] AudioDataFadeInOut_Process {%d %d %d}",
                ptConfig->usFadeInOutHandle,
                ptConfig->usFadeInSamples,
                ptConfig->usFadeOutSamples);

            return ptConfig;
        }
    }

    SCI_FREE(ptConfig);
    ptConfig = PNULL;
    return PNULL;
}

PUBLIC void AudioDataFadeInOut_Deconstruct(
    AUDIO_DATA_FADE_INOUT_CONFIG_T** pptConfig
    )
{
    AUDIO_DATA_FADE_INOUT_CONFIG_T* ptConfig;
    if (PNULL == pptConfig) {
        SCI_TRACE_LOW("[FADE] AudioDataFadeInOut_Deconstruct already deconsturct ptConfig");
        return;
    }
    if (PNULL == *pptConfig) {
        SCI_TRACE_LOW("[FADE] AudioDataFadeInOut_Deconstruct already deconsturct *ptConfig");
        return;
    }
    ptConfig = *pptConfig;

    if(ptConfig->usFadeInOutHandle>=AUDIO_DATA_FADE_INOUT_PROCESS_MAX)
    {
        SCI_TRACE_LOW("[FADE] AudioDataFadeInOut_Deconstruct usFadeInOutHandle is invalid: %d.",
            ptConfig->usFadeInOutHandle);
        return;
    }
    if(!tAudioDataFadeInoutProcess[ptConfig->usFadeInOutHandle].bIsUsed)
    {
        SCI_TRACE_LOW("[FADE] AudioDataFadeInOut_Deconstruct usFadeInOutHandle is not useing");
        return;
    }
    tAudioDataFadeInoutProcess[ptConfig->usFadeInOutHandle].bIsUsed = SCI_FALSE;

    SCI_FREE(ptConfig);
    *pptConfig = PNULL;
}

PUBLIC AUDIO_RESULT_E AudioDataFadeInOut_Process(
    AUDIO_DATA_FADE_INOUT_INFO_T *ptDataInfo
    )
{
    uint32 uiProcessFlag = 0; //  bit0: left; bit1: right;
    uint32 uiIndex = 0;
    uint32 uiCurDataGain = 0;
    uint32 usFadeInOutHandle = 0;
    uint32 uiState = 0;// FADE_STATE_NORMAL or FADE_STATE_IN or FADE_STATE_OUT, default value in construct: FADE_STATE_NORMAL
    int16 *sSrcDataLeft = PNULL;
    int16 *sSrcDataRight = PNULL;
    int16 *sDestDataLeft = PNULL;
    int16 *sDestDataRight = PNULL;
    uint32 uiDataLen = 0; // unit: words(2bytes)
    int32  iTempData = 0;

    if(!bGainInfoConfig)
    {
        SCI_TRACE_LOW("[FADE] AudioDataFadeInOut_Process Please config parameters first.");
        return AUDIO_ERROR;
    }

    if(PNULL==ptDataInfo)
    {
        SCI_TRACE_LOW("[FADE] AudioDataFadeInOut_Process input para is null.");
        return AUDIO_ERROR;
    }

    usFadeInOutHandle = ptDataInfo->usFadeInOutHandle;
    uiState = ptDataInfo->uiState;
    sSrcDataLeft = ptDataInfo->sSrcDataLeft;
    sSrcDataRight = ptDataInfo->sSrcDataRight;
    sDestDataLeft = ptDataInfo->sDestDataLeft;
    sDestDataRight = ptDataInfo->sDestDataRight;
    uiDataLen = ptDataInfo->uiDataLen;

    if((sSrcDataLeft!=PNULL) && (sDestDataLeft!=PNULL))
    {
        uiProcessFlag |= 1;
    }
    if((sSrcDataRight!=PNULL) && (sDestDataRight!=PNULL))
    {
        uiProcessFlag |= 2;
    }
    if(FADE_STATE_IN==uiState)
    {
        if(uiProcessFlag==1)//only left
        {
            for(uiIndex=0; uiIndex<uiDataLen; uiIndex++)
            {
                if(tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeInGain<usNormalizingFadeInOutMax)
                {
                    tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeInGain += usNormalizingFadeInStep;
                    if(tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeInGain>=usNormalizingFadeInOutMax)
                    {
                        sDestDataLeft[uiIndex] =
                            (((int32)sSrcDataLeft[uiIndex] * usNormalizingFadeInOutMax) >> 12);// data/AUDIO_DATA_NORMALIZING_VALUE;
                    }
                    else
                    {
                        sDestDataLeft[uiIndex] =
                            (((int32)sSrcDataLeft[uiIndex] * tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeInGain) >> 12);///AUDIO_DATA_NORMALIZING_VALUE;
                    }
                }
                else
                {
                    sDestDataLeft[uiIndex] =
                         (((int32)sSrcDataLeft[uiIndex] * usNormalizingFadeInOutMax) >> 12);// data/AUDIO_DATA_NORMALIZING_VALUE;
                }
            }
        }
        else if(uiProcessFlag==2)// only right
        {
            for(uiIndex=0; uiIndex<uiDataLen; uiIndex++)
            {
                if(tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeInGain<usNormalizingFadeInOutMax)
                {
                    tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeInGain += usNormalizingFadeInStep;
                    if(tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeInGain>=usNormalizingFadeInOutMax)
                    {
                        sDestDataRight[uiIndex] =
                             (((int32)sSrcDataRight[uiIndex] * usNormalizingFadeInOutMax) >> 12);// data/AUDIO_DATA_NORMALIZING_VALUE;
                    }
                    else
                    {
                        sDestDataRight[uiIndex] =
                            (((int32)sSrcDataRight[uiIndex] * tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeInGain) >> 12);///AUDIO_DATA_NORMALIZING_VALUE;
                    }
                }
                else
                {
                    sDestDataRight[uiIndex] = sSrcDataRight[uiIndex];
                }
            }
        }
        else if(uiProcessFlag==3) //left and right
        {
            for(uiIndex=0; uiIndex<uiDataLen; uiIndex++)
            {
                if(tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeInGain<usNormalizingFadeInOutMax)
                {
                    tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeInGain += usNormalizingFadeInStep;
                    if(tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeInGain>=usNormalizingFadeInOutMax)
                    {
                        sDestDataLeft[uiIndex] =
                            (((int32)sSrcDataLeft[uiIndex] * usNormalizingFadeInOutMax) >> 12);// data/AUDIO_DATA_NORMALIZING_VALUE;
                        sDestDataRight[uiIndex] =
                            (((int32)sSrcDataRight[uiIndex] * usNormalizingFadeInOutMax) >> 12);// data/AUDIO_DATA_NORMALIZING_VALUE;
                    }
                    else
                    {
                        sDestDataLeft[uiIndex] =
                            (((int32)sSrcDataLeft[uiIndex] * tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeInGain) >> 12);///AUDIO_DATA_NORMALIZING_VALUE;
                        sDestDataRight[uiIndex] =
                            (((int32)sSrcDataRight[uiIndex] * tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeInGain) >> 12);///AUDIO_DATA_NORMALIZING_VALUE;
                    }
                }
                else
                {
                        sDestDataLeft[uiIndex] =
                            ((sSrcDataLeft[uiIndex] * usNormalizingFadeInOutMax) >> 12);// data/AUDIO_DATA_NORMALIZING_VALUE;
                        sDestDataRight[uiIndex] =
                            ((sSrcDataRight[uiIndex] * usNormalizingFadeInOutMax) >> 12);// data/AUDIO_DATA_NORMALIZING_VALUE;
                }
            }
        }
    }
    else if(FADE_STATE_OUT==uiState)
    {
        if(uiProcessFlag==1)//only left
        {
            for(uiIndex=0; uiIndex<uiDataLen; uiIndex++)
            {
                if(tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeOutGain>usNormalizingFadeOutStep)
                {
                    tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeOutGain -= usNormalizingFadeOutStep;
                    sDestDataLeft[uiIndex] =
                            (((int32)sSrcDataLeft[uiIndex] * tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeOutGain) >> 12);///AUDIO_DATA_NORMALIZING_VALUE;
                }
                else
                {
                    sDestDataLeft[uiIndex] = 0;
                }
            }
        }
        else if(uiProcessFlag==2)// only right
        {
            for(uiIndex=0; uiIndex<uiDataLen; uiIndex++)
            {
                if(tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeOutGain>usNormalizingFadeOutStep)
                {
                    tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeOutGain -= usNormalizingFadeOutStep;
                    sDestDataRight[uiIndex] =
                            (((int32)sSrcDataRight[uiIndex] * tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeOutGain) >> 12);///AUDIO_DATA_NORMALIZING_VALUE;
                }
                else
                {
                    sDestDataRight[uiIndex] = 0;
                }
            }
        }
        else if(uiProcessFlag==3) //left and right
        {
            for(uiIndex=0; uiIndex<uiDataLen; uiIndex++)
            {
                if(tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeOutGain>usNormalizingFadeOutStep)
                {
                    tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeOutGain -= usNormalizingFadeOutStep;
                    sDestDataLeft[uiIndex] =
                            (((int32)sSrcDataLeft[uiIndex] * tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeOutGain) >> 12);///AUDIO_DATA_NORMALIZING_VALUE;
                    sDestDataRight[uiIndex] =
                            (((int32)sSrcDataRight[uiIndex] * tAudioDataFadeInoutProcess[usFadeInOutHandle].usCurFadeOutGain) >> 12);///AUDIO_DATA_NORMALIZING_VALUE;
                }
                else
                {
                    sDestDataLeft[uiIndex] = 0;
                    sDestDataRight[uiIndex] = 0;
                }
            }
        }
    }
    else//FADE_STATE_NORMAL
    {
        if(uiProcessFlag==1)//only left
        {
            for(uiIndex=0; uiIndex<uiDataLen; uiIndex++)
            {
                sDestDataLeft[uiIndex] =
                    (((int32)sSrcDataLeft[uiIndex] * usNormalizingFadeInOutMax) >> 12);// data/AUDIO_DATA_NORMALIZING_VALUE;
            }
        }
        else if(uiProcessFlag==2)// only right
        {
            for(uiIndex=0; uiIndex<uiDataLen; uiIndex++)
            {
                sDestDataRight[uiIndex] =
                    (((int32)sSrcDataRight[uiIndex] * usNormalizingFadeInOutMax) >> 12);// data/AUDIO_DATA_NORMALIZING_VALUE;
            }
        }
        else if(uiProcessFlag==3) //left and right
        {
            for(uiIndex=0; uiIndex<uiDataLen; uiIndex++)
            {
                sDestDataLeft[uiIndex] =
                    (((int32)sSrcDataLeft[uiIndex] * usNormalizingFadeInOutMax) >> 12);// data/AUDIO_DATA_NORMALIZING_VALUE;
                sDestDataRight[uiIndex] =
                    (((int32)sSrcDataRight[uiIndex] * usNormalizingFadeInOutMax) >> 12);// data/AUDIO_DATA_NORMALIZING_VALUE;
            }
        }
    }
    return AUDIO_NO_ERROR;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
