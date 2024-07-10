/******************************************************************************
 ** File Name:      custom_tone_adp.h                                       *
 ** Author:         Shujing.Dong                                             *
 ** DATE:           12/13/2005                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME            DESCRIPTION                               *
 ** 12/13/2005      Shujing.Dong    Create.                                   *
 ******************************************************************************/
#ifndef _CUSTOM_TONE_ADP_H_
#define _CUSTOM_TONE_ADP_H_
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "apm_codec.h"
#include "asm.h" 
#include "audio_config.h"
/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                            Data structure                                 *
 **---------------------------------------------------------------------------*/
typedef struct 
{
	HAUDIO  hAudioHandle; 
    uint32  freq1;      // Freq to be played. 0 or 250 <= freq1 <= 3400Hz
    uint32  freq2;      // Freq to be played. 0 or 250 <= freq2 <= 3400Hz
    uint32  freq3;      // Freq to be played. 0 or 250 <= freq3 <= 3400Hz
    uint32  volume;   // volume of the tone to be generated. 0~4096, 0 is mute, and 4096 is max
    uint32  duration;    // Number of milliseconds of playing this tone.    
}CUSTOM_TONE_ADP_EXT_OPE_PLAY_PARA_T;
/**---------------------------------------------------------------------------*
 **                         Const variables                                   *
 **---------------------------------------------------------------------------*/
#define CUSTOM_TONE_ADP_EXT_OPE_PLAY_PARA_COUNT 5
/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/
   
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This Function is used to initialize the audio server. 
//  Author:         Shujing.Dong
//  Note:           
/*****************************************************************************/
PUBLIC HAUDIOCODEC CUSTOM_TONE_RegCodecPlugger( //return codec handle
    HCOLLISION hCollision,               //collision handle
    AUDIO_CODEC_EXT_CFGINFO_T* ptCodecExtCfgInfo
    );
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
